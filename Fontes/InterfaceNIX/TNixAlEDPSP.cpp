// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixAlEDPSP.h"
#include "VTInterfaceNIX.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\InfoZcc\VTInfoZccManager.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrecho.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSubestacao.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// fun��o global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTNixAl* __fastcall NewObjNixAlEDPSP(VTApl *apl, VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2,
	bool al_com_corrente, int patamar)
{
	return (new TNixAlEDPSP(apl, primario, z0, z1, z2, al_com_corrente, patamar));
}

// ---------------------------------------------------------------------------
__fastcall TNixAlEDPSP::TNixAlEDPSP(VTApl *apl, VTPrimario *primario, strIMP &z0, strIMP &z1, strIMP &z2,
	bool al_com_corrente, int patamar) : TNixAl()
{
	// vari�veis locais
	VTBarra *barra = primario->BarraInicial;

	// salva ponteiro p/ Primario
	this->primario = primario;
	this->apl = apl;
	// inicia dados
	strAL.al = "AL";
	strAL.cod_alim = primario->Rede->Codigo;
	strAL.nome_alim = primario->Rede->Codigo;
	strAL.nome_se = (primario->Subestacao) ? primario->Subestacao->Rede->Codigo : AnsiString("");
	if (!LeZccDoArquivo())
	{
		strAL.r1 = 0.01;
		strAL.x1 = 0.5;
		strAL.r2 = 0.01;
		strAL.x2 = 0.5;
		strAL.r0 = 0.01;
		strAL.x0 = 0.04;
	}
	strAL.rf = 40.0;
	strAL.ta = 4;
	strAL.no_num = (barra == NULL) ? -1 : barra->Id;
	// strAL.unidade   = 1;    //ohm
	strAL.unidade = 0; // pu FKM-2016.02.11
	// strAL.carga     = CorrenteCarga();
	// strAL.carga     = (al_com_corrente) ? CorrenteCarga() : 0; //FKM-2016.04.27
	if (al_com_corrente)
	{
		strAL.carga = CorrenteCarga(patamar);
		if (IsDoubleZero(strAL.carga))
		{
			strAL.carga = 0;
		}
		else
		{
			// nada a fazer
		}
	}
	else
	{
		strAL.carga = 0;
	}
}

// ---------------------------------------------------------------------------
__fastcall TNixAlEDPSP::~TNixAlEDPSP(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
double __fastcall TNixAlEDPSP::CorrenteCarga(int patamar)
{
	// vari�veis locais
	complex<double>s_complexo;
	double s_mva;
	double corrente_a;
	VTLigacao *ligacao;

	// determina Ligacao incial do Primario
	if ((ligacao = primario->LigacaoInicial) == NULL)
		return (0);

	if (patamar < 0)
	{ // determina demanda m�xima
		s_complexo = ligacao->resflow->S_mva_max_allpat();
		s_mva = Abs(s_complexo);
	}
	else // FKM 2016.05.23 - pedido do Nakata
	{
		s_complexo = ligacao->resflow->S_mva[0][patamar];
		s_mva = Abs(s_complexo);
	}
	corrente_a = (s_mva * 1000.) / (sqrt(3.) * ligacao->pbarra1->vnom);

	return (corrente_a);
}

// ---------------------------------------------------------------------------
bool __fastcall TNixAlEDPSP::LeZccDoArquivo(void)
{
	VTInfoZccManager *infoZccManager = (VTInfoZccManager*)apl->GetObject(__classid(VTInfoZccManager));
	bool sucesso = false;
	strInfoZcc *info = NULL;
	int externId;

	try
	{
		externId = StrToInt(primario->Rede->Extern_id);
	}
	catch (Exception &e)
	{
		externId = -1;
    }
	if ((infoZccManager != NULL) &&  (externId > 0))
	{
		info = infoZccManager->ExisteInfoZcc(externId);
	}
	if (info != NULL)
	{
		strAL.r1 = info->z1_pu.r;
		strAL.x1 = info->z1_pu.x;
		strAL.r2 = info->z1_pu.r;
		strAL.x2 = info->z1_pu.x;
		strAL.r0 = info->z0_pu.r;
		strAL.x0 = info->z0_pu.x;
		sucesso = true;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TNixAlEDPSP::LinhaArqCsv(void)
{
	// vari�veis locais
	AnsiString separador = ";";
	AnsiString txt;

	txt = strAL.al + separador;
	if (strAL.cod_alim.Length() > 20)
	{
		txt += strAL.cod_alim.SetLength(20) + separador;
	}
	else
	{
		txt += strAL.cod_alim + separador;
	}
	txt += strAL.nome_alim + separador;
	txt += strAL.nome_se + separador;
	txt += DoubleToStr("%5.4f", strAL.r1) + separador;
	txt += DoubleToStr("%5.4f", strAL.x1) + separador;
	txt += DoubleToStr("%5.4f", strAL.r2) + separador;
	txt += DoubleToStr("%5.4f", strAL.x2) + separador;
	txt += DoubleToStr("%5.4f", strAL.r0) + separador;
	txt += DoubleToStr("%5.4f", strAL.x0) + separador;
	txt += DoubleToStr("%2.1f", strAL.rf) + separador;
	txt += DoubleToStr("%2.1f", strAL.ta) + separador;
	txt += IntToStr(strAL.no_num) + separador;
	txt += IntToStr(strAL.unidade) + separador;
	// FKM 2016.05.10- mandar 0.0 zoa o nix
	if (IsDoubleZero(strAL.carga))
	{
		txt += "0" + separador;
	}
	else
	{
		txt += DoubleToStr("%2.1f", strAL.carga) + separador;
	}
	return (txt);
}
