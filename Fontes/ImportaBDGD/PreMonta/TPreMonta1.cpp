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
#include "TPreMonta1.h"
#include <Entidades\TSe.h>
#include <Entidades\TPri.h>
#include <Entidades\TSec.h>

// ---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta1(VTApl *apl)
{
	return (new TPreMonta1(apl));
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta1::TPreMonta1(VTApl *apl) : TPreMonta(apl)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta1::~TPreMonta1(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Barra(TBarraBDGD *barraBT)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Capacitor(TCapacitorBDGD *capBT)
{
	// vari�veis locais
	strCAPACITOR str;

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
//		// insere Capacitor na Barra MT
//		str.bar_id = et->barra_mt->Id;
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
bool __fastcall TPreMonta1::InsereSec_Chave(TChaveBDGD *chaveBDGD)
{
	// nada a fazer
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Consumidor(TCargaBDGD *cargaBT)
{
	// vari�veis locais
	VTCarga *carga;

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
//		if ((carga = CriaCargaConsumidorBT(sdi, et->barra_mt->Id)) == NULL)
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
bool __fastcall TPreMonta1::InsereSec_ConsIP(TCargaIP *cargaIP)
{
//	// vari�veis locais
//	VTBarra *barra;
//	VTCarga *carga = NULL;

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
//		if ((carga = CriaCargaConsumidorIP(sdi, et->barra_mt->Id)) == NULL)
//			return (false);
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
bool __fastcall TPreMonta1::InsereSec_Gerador(TGeradorBDGD *geradorBT)
{
	// vari�veis locais
	double s_mva, p_mw, q_mvar;
	strGERADOR str;
	VTCarga *carga;

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
//		// insere Gerador na Barra MT
//		str.bar_id = et->barra_mt->Id;
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
bool __fastcall TPreMonta1::InsereSec_Ramal(TRamalLigacao *ramal)
{
	return(true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSec_Rede(TRedeBT *redeBT)
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
		if ((sec = NewObjSec()) != NULL)  //destruido no destrutor do TPri
		{
			sec->id = redeBT->Id_Rede_BT;
			sec->codigo =redeBT->Codigo;
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
bool __fastcall TPreMonta1::InsereSec_TrafoMTBT(TTrafoDistribuicao *trafoMTBT)
{
	VTBarra *barra;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("SDBT", trafoMTBT->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", "Trafo BT");
		if (plog)
			plog->DefineValor("C�digo", trafoMTBT->Codigo);

		// verifica Barras do Trafo
		if(trafoMTBT->Barra_1 != NULL)
		{
			if(pri->ExisteBarra(trafoMTBT->Barra_1->Id) == NULL)
			{
				InserePri_Barra(trafoMTBT->Barra_1);
			}
			barra = monta->ExisteBarra(trafoMTBT->Barra_1->Id);
		}
		if (barra == NULL)
		{
			if (plog)
				plog->Add("N�o existe barra1 do trafo");
			return (true);
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
bool __fastcall TPreMonta1::InsereSec_Trecho(TTrechoBDGD *trechoBT)
{
	// nada a fazer
	return (true);
}





// -----------------------------------------------------------------------------
// eof
