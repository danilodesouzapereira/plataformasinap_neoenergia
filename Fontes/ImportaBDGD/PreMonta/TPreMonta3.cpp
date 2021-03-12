// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Cartografia\VTScg.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTDominio.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTrecho.h>
#include <DLL_Inc\Funcao.h>
#include "TPreMonta3.h"
#include <..\Commons\VTCommons.h>
#include <Dominio\TCapElo.h>
#include <Dominio\TCorrenteNom.h>
#include <Dominio\TFasesConexao.h>
#include <Dominio\TTipoUnidade.h>
#include <Dominio\TTipoPotAparente.h>
#include <Dominio\TTipoPotReativa.h>
#include <Entidades\TPri.h>
#include <Entidades\TSe.h>
#include <Entidades\TSec.h>

// ---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta3(VTApl *apl)
{
	return (new TPreMonta3(apl));
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta3::TPreMonta3(VTApl *apl) : TPreMonta(apl)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta3::~TPreMonta3(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Barra(TBarraBDGD *barraBT)
{
	// variáveis locais
	strBARRA str;
	VTBarra *barra;
	double utm_x, utm_y;
	double lat, lon;

	try
	{ // configura log
		if (plog)
			plog->DefineValor("Eqpto", "Barra");
		if (plog)
			plog->DefineValor("Código", barraBT->Codigo);
		// remove barra do primário (deve ser a barra secundária do MTBT)
		// cria uma nova Barra
		str.bar_id = barraBT->Id;
		str.codigo = barraBT->Codigo;
		str.extern_id = barraBT->Codigo;
		//str.extern_id = IntToStr(barraBT->Id);
		str.vnom = 0.220; // Default
		if (sec != NULL)
		{
		   str.vnom = sec->vnom_v * 0.001; // kV
		}
        // PENDENTE DVC
		lat = lon = 0.0;
		if(barraBT != NULL)
		{
			lat = barraBT->Latitude;
			lon = barraBT->Longitude;
		}
		// UTM
		scg->LatLon_to_NorthEast(lat, lon, utm_y, utm_x);
		// ATENÇÃO: as coordenadas do barramento estão em cm mas a unidade indicada é metro
		str.utm.x = int(utm_x *100.); //
		str.utm.y = int(utm_y * 100.); //
		// esquemático
		str.esq.x1 = str.utm.x;
		str.esq.y1 = str.utm.y;
		str.esq.x2 = str.utm.x;
		str.esq.y2 = str.utm.y;
		InsereBarraStart();
		// insere Barra em VTMonta
		if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
		{ // salva Barra na Sec
			if (sec != NULL)
			{
				sec->InsereBarra(barra);
			}
		}
		InsereBarraStop();
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
bool __fastcall TPreMonta3::InsereSec_Capacitor(TCapacitorBDGD *capBT)
{
// variaveis locais
	bool sucesso = true;

	try
	{ // cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Capacitor");
		if (plog)
			plog->DefineValor("Código", capBT->Codigo);
		// verifica se o Capacitor série pertence ao Circuito selecionado
		if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(capBT->Id_Rede);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Trecho não está associado a rede secundária");
			return (true);
		}
		if(capBT->Barra_2 == NULL)
		{
			// Capacitor serie
			sucesso = InsereSec_Cap_Serie(capBT);
		}
		else
		{
			// Capacitor paralelo - se assume que a barra1 deve estar sempre definida
			sucesso = InsereSec_Cap_Paralelo(capBT);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Cap_Paralelo(TCapacitorBDGD *cap)
{
	// variáveis locais
	strCAPACITOR str;
	VTBarra *barra = NULL;

	// Insere a barra
	if(cap->Barra_1 != NULL)
	{
//		if(sec->ExisteBarra(cap->Barra_1->Id) == NULL)
//		{
//				InsereSec_Barra(cap->Barra_1);
//		}
		barra = sec->ExisteBarra(cap->Barra_1->Id);
	}
	if(barra == NULL)
	{
	   if (plog)
		   plog->Add("Não existe barra de conexão");
		   return (true);
	}
	// preenche estrutura strCAPACITOR
	str.cap_id = cap->Id;
	str.codigo = cap->Codigo;
	str.bar_id = barra->Id;
	str.ligacao = ConverteTipoLigacao(cap->Tipo_Unidade->Unidade);
	if (str.ligacao == lgINDEFINIDA)
		{
			if (plog)
				plog->Add("Tipo de ligação inválida");
			str.ligacao = lgEST_AT;
		}
	str.vnom = sec->vnom_v * 0.001;
	str.q_mvar = cap->Tipo_Q->Pot_Reativa_kVAr * 0.001;
	str.esq.posbar = 50.;
	str.esq.dxbar = 0;
	str.esq.dybar = -DYBAR;
	// insere Capacitor paralelo em VTMonta
	if (monta->InsereCapacitor(str) == NULL)
			return (false);
}


// ---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Cap_Serie(TCapacitorBDGD *cap)
{
	// variáveis locais
	strCAPSERIE str;
	VTBarra *barra1, *barra2;

	barra1 = barra2 = NULL;

	// Insere a barra 1 e 2
    if(cap->Barra_1 != NULL)
	{
		if(sec->ExisteBarra(cap->Barra_1->Id) == NULL)
		{
				InsereSec_Barra(cap->Barra_1);
		}
		barra1 = sec->ExisteBarra(cap->Barra_1->Id);
	}
	if(cap->Barra_2 != NULL)
	{
		if(sec->ExisteBarra(cap->Barra_2->Id) == NULL)
		{
				InsereSec_Barra(cap->Barra_2);
		}
		barra2 = sec->ExisteBarra(cap->Barra_2->Id);
	}
	if((barra1 == NULL) || (barra2 == NULL))
	{
	   if (plog)
		   plog->Add("Não existe barra de conexão");
		   return (true);
	}
	// preenche estrutura strCAPACITOR
	str.id = cap->Id;
	str.rede_id = cap->Id_Rede;
	str.codigo = cap->Codigo;
	str.bar1_id = barra1->Id;
	str.bar2_id = barra2->Id;
	str.vnom = sec->vnom_v * 0.001;
	if(cap->Tipo_Q != NULL)
	{
		str.snom = cap->Tipo_Q->Pot_Reativa_kVAr * 0.001 ;
	}
	str.esq.posbar1 = 50.;
	str.esq.posbar2 = 50.;
	// insere Capacitor série em VTMonta
	if (monta->InsereCapserie(str) == NULL)
			return (false);
}


// ---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Chave(TChaveBDGD *chaveBDGD)
{
	// variáveis locais
	strCHAVE str;
	VTBarra *barra1, *barra2;
	VTTipoChave *tipo_ch = NULL;
	VTChave *chave;

	try
	{ // cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Chave");
		if (plog)
			plog->DefineValor("Código", chaveBDGD->Codigo);
		// verifica se a Chave pertence ao Circuito selecionado
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Não está associada a rede primária");
			return (true);
		}
		// verifica se Barras iguais
		if (chaveBDGD->Barra_1 == chaveBDGD->Barra_2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		barra1 = barra2 = NULL;
		// Insere as barras 1 e 2
		if(chaveBDGD->Barra_1 != NULL)
		{
			if(sec->ExisteBarra(chaveBDGD->Barra_1->Id) == NULL)
			{
				InsereSec_Barra(chaveBDGD->Barra_1);
			}
			// verifica se existem as Barras
			barra1 = sec->ExisteBarra(chaveBDGD->Barra_1->Id);
		}

		if(chaveBDGD->Barra_2 != NULL)
		{
			if(sec->ExisteBarra(chaveBDGD->Barra_2->Id) == NULL)
			{
				InsereSec_Barra(chaveBDGD->Barra_2);
			}
			// verifica se existem as Barras
			barra2 = sec->ExisteBarra(chaveBDGD->Barra_2->Id);
		}
		if ((barra1 == NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as duas barras");
			return (true);
		}
		if ((barra1 != NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(chaveBDGD->Barra_2->Id));
			return (true);
		}
		else if ((barra1 == NULL) && (barra2 != NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(chaveBDGD->Barra_1->Id));
			return (true);
		}
		// preenche estrutura strCHAVE
		str.chv_id = chaveBDGD->Id;
		str.rede_id = chaveBDGD->Id_Rede;   // Rede BT
		str.codigo = chaveBDGD->Codigo;
		if(chaveBDGD->Tipo_Unidade != NULL)
		{
			str.tipo_gis = chaveBDGD->Tipo_Unidade->Descricao;
			//str.tipo_gis = "";
			//tipo_ch = tipos->ExisteTipoChave(chaveBDGD->Id);
			tipo_ch = tipos->ExisteTipoChave(chaveBDGD->Tipo_Unidade->Descricao);
		}
		if(chaveBDGD->Elo_Fusivel != NULL)
		{
			str.elo_fusivel = chaveBDGD->Elo_Fusivel->Codigo;
		}
		str.bar1_id = barra1->Id;
		str.bar2_id = barra2->Id;
		str.estado = chaveBDGD->Estado_Normal;
		if(chaveBDGD->Corrente_Nom != NULL)
		{
			str.corn = chaveBDGD->Corrente_Nom->Corrente_A;
		}
		//
		str.tipochave.id = (tipo_ch != NULL) ? tipo_ch->Id : ID_NULO;
		str.tipochave.tipo = (tipo_ch != NULL) ? tipo_ch->Tipo : chaveINDEFINIDA;
		//str.tipochave.codigo = (tipo_ch != NULL) ? tipo_ch->Codigo : "Indefinida";
		str.tipochave.codigo = (chaveBDGD->Tipo_Unidade != NULL) ? chaveBDGD->Tipo_Unidade->Descricao : AnsiString("Indefinida");
		str.telecomandada = chaveBDGD->Telecomandada;
		str.tecno_comunicacao = comDESCONHECIDO;
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		// insere Chave em VTMonta
		if ((chave = (VTChave*)monta->InsereChave(str)) != NULL)
			return (true);
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
bool __fastcall TPreMonta3::InsereSec_Consumidor(TCargaBDGD *cargaBT)
{
	// variáveis locais
	VTCarga *carga;
	VTBarra *barra;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Carga");
		// Verifica a rede
		if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(cargaBT->Id_Rede);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Carga não está associada a rede secundária");
			return (true);
		}
		// Insere a barra
		if (cargaBT->Barra != NULL)
		{
//			if(sec->ExisteBarra(cargaBT->Barra->Id) == NULL)
//			{
//				InsereSec_Barra(cargaBT->Barra);
//			}
			// verifica se existem as Barras
			barra = sec->ExisteBarra(cargaBT->Barra->Id);
		}
		if (barra == NULL)
		{
			if (plog)
				plog->Add("Não existe barra de carga");
			return (true);
		}
		// Cria carga do consumidor
		CriaCargaConsumidorBT(cargaBT, barra->Id);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

//---------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_ConsIP(TCargaIP *cargaIP)
{
	// variáveis locais
	VTBarra *barra;
	VTCarga *carga = NULL;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Carga IP");
		if ((pri == NULL) || (pri->id != cargaIP->Id_Rede_MT))
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(cargaIP->Id_Rede_BT);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Carga não está associada a rede secundária");
			return (true);
		}
		// Insere a barra
		if (cargaIP->Barra != NULL)
		{
//			if(sec->ExisteBarra(cargaIP->Barra->Id) == NULL)
//			{
//				InsereSec_Barra(cargaIP->Barra);
//			}
			// verifica se existem as Barras
			barra = sec->ExisteBarra(cargaIP->Barra->Id);
		}
		if (barra == NULL)
		{
			if (plog)
				plog->Add("Não existe barra de iluminação pública");
			return (true);
		}
		// cria Carga de IP
		CriaCargaConsumidorIP(cargaIP, barra->Id);
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
bool __fastcall TPreMonta3::InsereSec_Gerador(TGeradorBDGD *geradorBT)
{
	// variáveis locais
	double s_mva, p_mw, q_mvar;
	strGERADOR str;
	VTBarra *barra;
	sec = NULL;

	try
	{ // configura Resumo
		if (plog)
			plog->DefineValor("Eqpto", "Gerador");
		if (plog)
			plog->DefineValor("Código", geradorBT->Codigo);
		if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(geradorBT->Id_Rede);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Trecho não está associado a rede secundária");
			return (true);
		}
		// Insere as barras
		if (geradorBT->Barra != NULL)
		{
//			if(sec->ExisteBarra(geradorBT->Barra->Id) == NULL)
//			{
//				InsereSec_Barra(geradorBT->Barra);
//			}
//			// verifica se existem as Barras
			barra = sec->ExisteBarra(geradorBT->Barra->Id);
		}
		if (barra == NULL)
		{
			if (plog)
				plog->Add("Não existe barra do gerador BT");
			return (true);
		}
		// inicia estrutura strGerador
		str.obra = false;
		str.ger_id = geradorBT->Id;
		str.bar_id = geradorBT->Barra->Id;
		str.codigo = geradorBT->Codigo;
		str.extern_id = geradorBT->Codigo;
		str.tiposup = supPV;               // Default
		str.tipo_geracao = gerPCH;         // Default
		str.ligacao = lgEST_AT;            // Default
		str.vnom = (geradorBT->Tensao->Tensao_V) * 0.001;
		str.snom_mva = (geradorBT->P_Inst_kW / 0.92) * 0.001;
		str.smax_mva = str.snom_mva * 1.2;
		str.fplim = 0.9;
		str.z0.r = geradorBT->R0;
		str.z0.x = geradorBT->X0;
		str.z1.r = geradorBT->R1;
		str.z1.x = geradorBT->X1;
		// define curva de geração própria
		str.curva.id = -1;
		str.curva.tipica = false;
		str.curva.num_pat = pat01HORA;
		str.curva.num_valor = nvCURVA_PQVT;
		// define pot.ativa e reativa a partir de Snom e considerando fatpot = 0.92
		s_mva = str.snom_mva;
		p_mw = geradorBT->P_Inst_kW * 0.001;
		q_mvar = sqrt((s_mva * s_mva) - (p_mw * p_mw));
		for (int np = 0; np < pat01HORA; np++)
		{ // define hora inicial e final do patamar
			str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
			str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
			// define valores de ponta/fora de ponta
			if (pat_edp[np].ponta)
			{ // demanda de ponta
				str.curva.patamar[np].valor[indP] = p_mw;
				str.curva.patamar[np].valor[indQ] = q_mvar;
				str.curva.patamar[np].valor[indV] = 1.;
				str.curva.patamar[np].valor[indT] = 0.;
			}
			else
			{ // demanda fora de ponta
				str.curva.patamar[np].valor[indP] = p_mw;
				str.curva.patamar[np].valor[indQ] = q_mvar;
				str.curva.patamar[np].valor[indV] = 1.;
				str.curva.patamar[np].valor[indT] = 0.;
			}
		}
		// define coordenadas p/ esquemático
		str.esq.dxbar = -2000;
		str.esq.dybar = 0;
		// insere Gerador em VTMonta
		if (!monta->InsereGerador(str))
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Ramal(TRamalLigacao *ramal)
{
	// variáveis locais
    VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	sec = NULL;
	int fases_tag;
	strTRECHO str_tre;
	VTArranjo *arranjo;
	VTBarra *barra1, *barra2;
	VTTrecho *trecho;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Ramal de Ligação");
		if (plog)
			plog->DefineValor("Código", ramal->Codigo);
//		// verifica a qual rede sec o trecho pertence
		if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(ramal->Id_Rede);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Ramal de Ligação não está associado a rede secundária");
			return (true);
		}
        // verifica se Barras iguais
		if (ramal->Barra_1 == ramal->Barra_2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		barra1 = barra2 = NULL;
		// Insere as barras 1 e 2
		if (ramal->Barra_1 != NULL)
		{
			if(sec->ExisteBarra(ramal->Barra_1->Id) == NULL)
			{
				InsereSec_Barra(ramal->Barra_1);
			}
			// verifica se existem as Barras
			barra1 = sec->ExisteBarra(ramal->Barra_1->Id);
		}
		if (ramal->Barra_1 != NULL)
		{
			if(sec->ExisteBarra(ramal->Barra_2->Id) == NULL)
			{
				InsereSec_Barra(ramal->Barra_2);
			}
			// verifica se existem as Barras
			barra2 = sec->ExisteBarra(ramal->Barra_2->Id);
		}
		if ((barra1 == NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as duas barras");
			return (true);
		}
		if ((barra1 != NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(ramal->Barra_2->Id));
			return (true);
		}
		else if ((barra1 == NULL) && (barra2 != NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(ramal->Barra_1->Id));
			return (true);
		}
		// determina fases do Trecho e inclui o neutro
		if (ramal->Fases_Con == NULL)
		{
			if (plog)
				plog->Add("Fases indefinidas");
			// assume fases ABCN
			ramal->Fases_Con = commons->GetFasesConexao(1);
		}
		fases_tag = fases->ExisteFase(ramal->Fases_Con->Codigo) | faseN;
		// verifica se existe Arranjo (criando um novo Arranjo se for necessário)
		if ((arranjo = ExisteArranjo(ramal->Arranjo->Id, fases_tag, true)) == NULL)
		{
			if (plog)
				plog->Add("Não existe arranjo");
			//assume default
			arranjo = arranjoBT;
			// verifica se precisa trocar as fases do arranjoBT
			if (arranjo->Fases != fases_tag)
			{
				arranjo = ArranjoBTcerto(fases_tag);
			}
		}
		if (IsDoubleZero(ramal->Comprimento_m))
		{ // verifica se comprimento nulo
			ramal->Comprimento_m = 1.;
		}
//		// preenche estrutura strTRECHO
		str_tre.tre_id = ramal->Id;
		str_tre.rede_id = ramal->Id_Rede;
		str_tre.codigo = ramal->Codigo;
		str_tre.bar1_id = ramal->Barra_1->Id;
		str_tre.bar2_id = ramal->Barra_2->Id;
		str_tre.fases = fases_tag;
		str_tre.comp_km = ramal->Comprimento_m * 0.001;
		str_tre.esq.posbar1 = 50.;
		str_tre.esq.posbar2 = 50.;
		// insere Trecho com o Arranjo definido
		if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, arranjo)) == NULL)
		{
			return(false);
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

// -------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta3::InsereSec_Rede(TRedeBT *redeBT)
{
	// variáveis locais
	strREDE str;
	int id_rede_mt;
	VTBarra *barra_mt, *barra_bt;
	VTRede *rede;
	VTTrecho *trecho;

	try
		{ // configura Log
		if (plog)
			plog->DefineValor("Rede", redeBT->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", "Rede BT");
		if (plog)
			plog->DefineValor("Código", redeBT->Codigo);
		// verifica se a Rede é alimentada pela Rede primária
		if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		id_rede_mt = redeBT->Id_Rede_MT;
		// insere Rede em VTMonta
		str.dominio_id = dominio->Id;
		str.rede_id = redeBT->Id_Rede_BT;
		str.mrede_id = ID_NULO;
		str.cluster_id = ID_NULO;
		str.barini_id = NULL;
		str.color = clWhite;
		str.codigo = redeBT->Codigo;
		str.extern_id = redeBT->Codigo;
		//str.extern_id = IntToStr(redeBT->Id_Rede_BT);
		str.estatica = true;
		str.radial = false;
		str.tiporede.id = ID_NULO;
		str.tiporede.segmento = redeSEC;
		str.tiporede.codigo = "";
		// insere Rede em Monta
		if ((rede = (VTRede*)monta->InsereRede(str)) == NULL)
			return (false);
		// cria objeto Sec e insere em Pri
		if ((sec = NewObjSec()) != NULL)    //destruido no destrutor do TPri
			{
			sec->id = redeBT->Id_Rede_BT;
			sec->rede = rede;
			sec->carga_eqv = NULL;
			sec->barra_mt = NULL;
			sec->barra_bt = NULL;
			sec->vnom_v = redeBT->Tensao_SecMTBT->Tensao_V;
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
bool __fastcall TPreMonta3::InsereSec_TrafoMTBT(TTrafoDistribuicao *trafoMTBT)
{
	bool sucesso = false;
    sec = NULL;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("SDBT", IntToStr(trafoMTBT->Id_Rede_BT));
		if (plog)
			plog->DefineValor("Eqpto", "Trafo BT");
		if (plog)
			plog->DefineValor("Código", trafoMTBT->Codigo);
        if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(trafoMTBT->Id_Rede_BT);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Trafo não está associado a rede secundária");
		}
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
		// verifica se a a potencia nominal é nula
		if (IsDoubleZero(trafoMTBT->S_Nom_kVA))
		{
			if (plog)
				plog->Add("Potência nominal nula");
			trafoMTBT->S_Nom_kVA = 75.;
		}
		// verifica se a Perda no Ferro é nula
		if (IsDoubleZero(trafoMTBT->Perda_Fe_W))
		{
			if (plog)
				plog->Add("Perda no Ferro nula");
			trafoMTBT->Perda_Fe_W = 75000.;
		}
		// verifica se a Perda Total é nula
		if (IsDoubleZero(trafoMTBT->Perda_Total_W))
		{
			if (plog)
				plog->Add("Perda no Ferro nula");
			trafoMTBT->Perda_Total_W = 75000.;
		}
		// verifica se é um trafo de 2 ou 3 enrolamentos
		if ((trafoMTBT->Barra_3 != NULL) && (trafoMTBT->Barra_3 != trafoMTBT->Barra_2))
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
bool __fastcall TPreMonta3::InsereSec_Trecho(TTrechoBDGD *trechoBT)
{
	// variáveis locais
    VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	sec = NULL;
	int fases_tag;
	strTRECHO str_tre;
	VTArranjo *arranjo;
	VTBarra *barra1, *barra2;
	VTTrecho *trecho;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trecho");
		if (plog)
			plog->DefineValor("Código", trechoBT->Codigo);
//		// verifica a qual rede sec o trecho pertence
		if (pri == NULL)
			{
			if (plog)
				plog->Add("Não pertence à rede primária");
			return (true);
			}
		sec = pri->ExisteSec(trechoBT->Id_Rede);
		if (sec == NULL)
		{
			if (plog)
				plog->Add("Trecho não está associado a rede secundária");
			return (true);
		}
        // verifica se Barras iguais
		if (trechoBT->Barra_1 == trechoBT->Barra_2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		barra1 = barra2 = NULL;
		// Insere as barras 1 e 2
		if (trechoBT->Barra_1 != NULL)
		{
			if(sec->ExisteBarra(trechoBT->Barra_1->Id) == NULL)
			{
				InsereSec_Barra(trechoBT->Barra_1);
			}
			// verifica se existem as Barras
			barra1 = monta->ExisteBarra(trechoBT->Barra_1->Id);
		}
		if (trechoBT->Barra_1 != NULL)
		{
			if(sec->ExisteBarra(trechoBT->Barra_2->Id) == NULL)
			{
				InsereSec_Barra(trechoBT->Barra_2);
			}
			// verifica se existem as Barras
			barra2 = monta->ExisteBarra(trechoBT->Barra_2->Id);
		}
		if ((barra1 == NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as duas barras");
			return (true);
		}
		if ((barra1 != NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(trechoBT->Barra_2->Id));
			return (true);
		}
		else if ((barra1 == NULL) && (barra2 != NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(trechoBT->Barra_1->Id));
			return (true);
		}
		// determina fases do Trecho e inclui o neutro
		if (trechoBT->Fases_Con == NULL)
		{
			if (plog)
				plog->Add("Fases indefinidas");
			// assume fases ABCN
			trechoBT->Fases_Con = commons->GetFasesConexao(1) ;
		}
		fases_tag = fases->ExisteFase(trechoBT->Fases_Con->Codigo) | faseN;
		// verifica se existe Arranjo (criando um novo Arranjo se for necessário)
		if ((arranjo = ExisteArranjo(trechoBT->Arranjo->Id, fases_tag)) == NULL)
		{
			if (plog)
				plog->Add("Não existe arranjo");
			//assume default
			arranjo = arranjoBT;
			// verifica se precisa trocar as fases do arranjoBT
			if (arranjo->Fases != fases_tag)
			{
				arranjo = ArranjoBTcerto(fases_tag);
			}
		}
		if (IsDoubleZero(trechoBT->Comprimento_m))
		{ // verifica se comprimento nulo
			trechoBT->Comprimento_m = 1.;
		}
//		// preenche estrutura strTRECHO
		str_tre.tre_id = trechoBT->Id;
		str_tre.rede_id = sec->id;
		str_tre.codigo = trechoBT->Codigo;
		str_tre.bar1_id = trechoBT->Barra_1->Id;
		str_tre.bar2_id = trechoBT->Barra_2->Id;
		str_tre.fases = fases_tag;
		str_tre.comp_km = trechoBT->Comprimento_m * 0.001;
		str_tre.esq.posbar1 = 50.;
		str_tre.esq.posbar2 = 50.;
		// insere Trecho com o Arranjo definido
		if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, barra1, barra2, arranjo)) == NULL)
		{
			return(false);
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


// -----------------------------------------------------------------------------
// eof
