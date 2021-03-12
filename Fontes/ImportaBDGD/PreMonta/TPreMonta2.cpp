// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTDominio.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTrecho.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
#include "TPreMonta2.h"
#include <Entidades\TPri.h>
#include <Entidades\TSe.h>
#include <Entidades\TSec.h>

// ---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta2(VTApl *apl)
{
	return (new TPreMonta2(apl));
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta2::TPreMonta2(VTApl *apl) : TPreMonta(apl)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta2::~TPreMonta2(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Barra(TBarraBDGD *barraBT)
{
//	//verifica se existe et
//	if(et == NULL)
//		return (false);
//	// incrementa n�mero de Barras da ET e verifica se � a primeira
//	et->num_barra++;
//	if (et->num_barra == 1)
//	{ // cria Trafos da ET e insere em VTMonta
//		InsereSec_Trafo(et);
//	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Capacitor(TCapacitorBDGD *capBT)
{
//	// vari�veis locais
//	strCAPACITOR str;
//
//	try
//	{ // cofigura Log
//		if (plog)
//			plog->DefineValor("Eqpto", "Capacitor");
//		if (plog)
//			plog->DefineValor("C�digo", sdi.codigo);
//		// verifica se o Capacitor s�rie pertence � Rede secund�ria selecionada
//		if ((et == NULL) || (et->id != sdi.redebt_id))
//		{
//			if (plog)
//				plog->Add("N�o est� associado a rede secund�ria");
//			return (true);
//		}
//		// preenche estrutura strCAPACITOR
//		str.cap_id = sdi.id;
//		str.codigo = sdi.codigo;
//		// insere Capacitor na Barra BT
//		str.bar_id = et->barra_bt->Id;
//		str.ligacao = lgTRIANG;
//		str.vnom = et->sec_vnom_kv;
//		str.q_mvar = sdi.qnon_kvar * 0.001;
//		str.esq.posbar = 50.;
//		str.esq.dxbar = 0;
//		str.esq.dybar = -DYBAR;
//		// insere Capacitor s�rie em VTMonta
//		if (monta->InsereCapacitor(str) == NULL)
//			return (false);
//	}
//	catch (Exception &e)
//	{
//		if (plog)
//			plog->Add(ERRO_DE_SISTEMA);
//		return (false);
//	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Chave(TChaveBDGD *chaveBDGD)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Consumidor(TCargaBDGD *cargaBT)
{
//	// vari�veis locais
//	TPtoCargaBT *ptc;
//	VTCarga *carga;
//
//	try
//	{ // configura Log
//		if (plog)
//			plog->DefineValor("Eqpto", "Carga");
//		// verifica se o Consumidor  pertence � Rede secund�ria selecionada
//		if ((et == NULL) || (et->id != sdi.redebt_id))
//		{
//			if (plog)
//				plog->Add("N�o est� associado a rede secund�ria");
//			return (true);
//		}
//		// cria Carga tempor�ria do consumidor
//		if ((carga = CriaCargaConsumidorBT(sdi, et->barra_bt->Id)) == NULL)
//		{
//			return (true);
//		}
//		// atualiza demanda da Carga equivalente da ET
//		AtualizaCargaEquivalente(et, carga);
//		// retira Carga da sua Barra e destr�i objeto
//		carga->pbarra->RemoveEqbar(carga);
//		delete carga;
//	}
//	catch (Exception &e)
//	{
//		if (plog)
//			plog->Add(ERRO_DE_SISTEMA);
//		return (false);
//	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_ConsIP(TCargaIP *cargaIP)
{

//	// vari�veis locais
//	VTBarra *barra;
//	VTCarga *carga = NULL;
//
//	try
//	{ // configura Log
//		if (plog)
//			plog->DefineValor("Eqpto", "Carga IP");
//		if (plog)
//			plog->DefineValor("C�digo", IntToStr(sdi.id));
//		// verifica se o Consumidor  pertence � Rede secund�ria selecionada
//		if (et == NULL)
//		{
//			if (plog)
//				plog->Add("N�o est� associado a rede secund�ria");
//			return (true);
//		}
//		// cria Carga tempor�ria de IP
//		carga = CriaCargaConsumidorIP(sdi, et->barra_bt->Id);
//		// atualiza demanda da Carga equivalente da ET
//		AtualizaCargaEquivalente(et, carga);
//		// retira Carga da sua Barra e destr�i objeto
//		carga->pbarra->RemoveEqbar(carga);
//		DLL_DeleteEqpto(carga);
//	}
//	catch (Exception &e)
//	{
//		if (plog)
//			plog->Add(ERRO_DE_SISTEMA);
//		return (false);
//	}
	return (true);
}


// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Gerador(TGeradorBDGD *geradorBT)
{
	// vari�veis locais
//	double s_mva, p_mw, q_mvar;
//	strGERADOR str;
//	VTCarga *carga;
//
//	try
//	{ // configura Resumo
//		if (plog)
//			plog->DefineValor("Eqpto", "Gerador");
//		if (plog)
//			plog->DefineValor("C�digo", sdi.codigo);
//		// prote��o
//		if ((et == NULL) || (et->id != sdi.circ_id))
//		{
//			if (plog)
//				plog->Add("N�o est� associado a rede secund�ria");
//			return (true);
//		}
//		// inicia estrutura strGerador
//		str.obra = false;
//		str.ger_id = sdi.id;
//		// insere Gerador na Barra BT
//		str.bar_id = et->barra_bt->Id;
//		str.codigo = sdi.codigo;
//		str.extern_id = "";
//		str.tiposup = supPV;
//		str.tipo_geracao = gerPCH;
//		str.ligacao = lgEST_AT;
//		str.vnom = et->sec_vnom_kv;
//		str.snom_mva = sdi.snom_kva * 0.001;
//		str.smax_mva = str.snom_mva * 1.2;
//		str.fplim = 0.9;
//		str.z0.r = 0.;
//		str.z0.x = 0.05;
//		str.z1.r = 0.;
//		str.z1.x = 0.05;
//		// define curva de gera��o pr�pria
//		str.curva.id = -1;
//		str.curva.tipica = false;
//		str.curva.num_pat = pat01HORA;
//		str.curva.num_valor = nvCURVA_PQVT;
//		// define pot.ativa e reativa a partir de Snom e considerando fatpot = 0.92
//		s_mva = str.snom_mva;
//		p_mw = s_mva * 0.92;
//		q_mvar = sqrt((s_mva * s_mva) - (p_mw * p_mw));
//		for (int np = 0; np < pat01HORA; np++)
//		{ // define hora inicial e final do patamar
//			str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
//			str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
//			// define valores de ponta/fora de ponta
//			if (pat_edp[np].ponta)
//			{ // demanda de ponta
//				str.curva.patamar[np].valor[indP] = p_mw;
//				str.curva.patamar[np].valor[indQ] = q_mvar;
//				str.curva.patamar[np].valor[indV] = 1.;
//				str.curva.patamar[np].valor[indT] = 0.;
//			}
//			else
//			{ // demanda fora de ponta
//				str.curva.patamar[np].valor[indP] = p_mw;
//				str.curva.patamar[np].valor[indQ] = q_mvar;
//				str.curva.patamar[np].valor[indV] = 1.;
//				str.curva.patamar[np].valor[indT] = 0.;
//			}
//		}
//		// define coordenadas p/ esquem�tico
//		str.esq.dxbar = -2000;
//		str.esq.dybar = 0;
//		// insere Gerador em VTMonta
//		if (!monta->InsereGerador(str))
//			return (false);
//	}
//	catch (Exception &e)
//	{
//		if (plog)
//			plog->Add(ERRO_DE_SISTEMA);
//		return (false);
//	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Ramal(TRamalLigacao *ramal)
{
	return(true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Rede(TRedeBT *redeBT)
{
	try
	{ // configura Log
		if (plog)
			plog->DefineValor("SDBT", redeBT->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", "Rede BT");
		if (plog)
			plog->DefineValor("C�digo", redeBT->Codigo);
		// reinicia ponteiro p/ secund�rio corrente
		sec = NULL;
		// verifica se a Rede � alimentada pela Rede prim�ria
		if (pri == NULL)
		{
			if (plog)
				plog->Add("N�o pertence � rede prim�ria");
			return (true);
		}
		// cria objeto Sec e insere em Pri
		if ((sec = NewObjSec()) != NULL)      //destruido no destrutor do TPri
		{
			sec->id = redeBT->Id_Rede_BT;
			sec->codigo = redeBT->Codigo;
			sec->rede = NULL;
			sec->barra_mt = NULL;
			sec->barra_bt = NULL;
			pri->InsereSec(sec);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_TrafoMTBT(TTrafoDistribuicao *trafoMTBT)
{
	bool sucesso = false;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("SDBT", IntToStr(trafoMTBT->Id_Rede_BT));
		if (plog)
			plog->DefineValor("Eqpto", "Trafo BT");
		if (plog)
			plog->DefineValor("C�digo", trafoMTBT->Codigo);
		// verifica Barra MT do Trafo
		if(trafoMTBT->Barra_1 != NULL)
		{
			if(pri->ExisteBarra(trafoMTBT->Barra_1->Id) == NULL)
			{
				InserePri_Barra(trafoMTBT->Barra_1);
			}
		}
		// verifica Barras de BT do Trafo
		if(trafoMTBT->Barra_2 != NULL)
		{
			InsereSec_Barra(trafoMTBT->Barra_2);
		}
		// verifica Barras do Trafo
		if(trafoMTBT->Barra_3 != NULL)
		{
			InsereSec_Barra(trafoMTBT->Barra_3);
		}
		// verifica se a a potencia nominal � nula
		if (IsDoubleZero(trafoMTBT->S_Nom_kVA))
		{
			if (plog)
				plog->Add("Pot�ncia nominal nula");
			trafoMTBT->S_Nom_kVA = 75.;
		}
		// verifica se a Perda no Ferro � nula
		if (IsDoubleZero(trafoMTBT->Perda_Fe_W))
		{
			if (plog)
				plog->Add("Perda no Ferro nula");
			trafoMTBT->Perda_Fe_W = 75000.;
		}
		// verifica se a Perda Total � nula
		if (IsDoubleZero(trafoMTBT->Perda_Total_W))
		{
			if (plog)
				plog->Add("Perda no Ferro nula");
			trafoMTBT->Perda_Total_W = 75000.;
		}
		// verifica se � um trafo de 2 ou 3 enrolamentos
		if (trafoMTBT->Barra_3 != NULL)
			sucesso = InsereSec_Trafo3E(trafoMTBT);
		else
			sucesso = InsereSec_Trafo2E(trafoMTBT);

	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSec_Trecho(TTrechoBDGD *trechoBT)
{
	// nada a fazer
	return (true);
}



// -----------------------------------------------------------------------------
// eof
