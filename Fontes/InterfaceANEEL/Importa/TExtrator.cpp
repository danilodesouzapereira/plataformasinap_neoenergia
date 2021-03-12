//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SysUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Log\VTLog.h>
#include <DLL_Inc\Funcao.h>
#include "TExtrator.h"
#include "VTPreMonta.h"

//---------------------------------------------------------------------------
VTExtrator* NewObjExtrator(VTApl *apl)
   {
   return(new TExtrator(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExtrator::TExtrator(VTApl *apl)
   {
   //salva ponteiros p/ objeto
   this->apl = apl;
   //define versão
   versao = 1;
	}

//---------------------------------------------------------------------------
__fastcall TExtrator::~TExtrator(void)
   {
   //nada a fazer
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaBT(TStrings *campos)
	{
	//variáveis locais
	int          nc = 0;
	sdi_CARGA_BT sdi;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "CargaBT");
		//valida número de dados recebidos
		if (campos->Count < 20)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase        = campos->Strings[nc++];
		sdi.ramal          = campos->Strings[nc++];
		sdi.codigo         = campos->Strings[nc++];
		sdi.fases          = campos->Strings[nc++];
		sdi.codigo_pto     = campos->Strings[nc++];
		sdi.rede_associada = StrToInt(campos->Strings[nc++]);
		sdi.tipo_medicao   = StrToInt(campos->Strings[nc++]);
		sdi.tipologia_curva = campos->Strings[nc++];
		for (int n = 0; n < 12; n++)
			{
			sdi.energia_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		//insere CargaBT em PreMonta
		if (! pre_monta->InsereCargaBT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}
//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaMT(TStrings *campos)
	{
	//variáveis locais
	int          nc = 0;
	sdi_CARGA_MT sdi;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "CargaMT");
		//valida número de dados recebidos
		if (campos->Count < 19)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase        = campos->Strings[nc++];
		sdi.circuito       = campos->Strings[nc++];
		sdi.codigo         = campos->Strings[nc++];
		sdi.fases          = campos->Strings[nc++];
		sdi.codigo_pto     = campos->Strings[nc++];
		sdi.rede_associada = StrToInt(campos->Strings[nc++]);
		sdi.tipologia_curva = campos->Strings[nc++];
		for (int n = 0; n < 12; n++)
			{
			sdi.energia_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		//insere CargaMT em PreMonta
		if (! pre_monta->InsereCargaMT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereChaveBT(TStrings *campos)
	{
	//variáveis locais
	int          nc = 0;
	sdi_CHAVE_BT sdi;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "ChaveBT");
		//valida número de dados recebidos
		if (campos->Count < 7)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase         = campos->Strings[nc++];
		sdi.codigo_rede     = campos->Strings[nc++];
		sdi.codigo_segmento = campos->Strings[nc++];
		sdi.codigo_pto1     = campos->Strings[nc++];
		sdi.codigo_pto2     = campos->Strings[nc++];
		sdi.fases           = campos->Strings[nc++];
		sdi.estado          = StrToInt(campos->Strings[nc++]);
		//insere ChaveBT em PreMonta
		if (! pre_monta->InsereChaveBT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereChaveMT(TStrings *campos)
	{
	//variáveis locais
	sdi_CHAVE_MT sdi;
	int          nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "ChaveMT");
		//valida número de dados recebidos
		if (campos->Count < 7)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase         = campos->Strings[nc++];
		sdi.circuito        = campos->Strings[nc++];
		sdi.codigo_segmento = campos->Strings[nc++];
		sdi.codigo_pto1     = campos->Strings[nc++];
		sdi.codigo_pto2     = campos->Strings[nc++];
		sdi.fases           = campos->Strings[nc++];
		sdi.estado          = StrToInt(campos->Strings[nc++]);
		//insere ChaveMT em PreMonta
		if (! pre_monta->InsereChaveMT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCircuito(TStrings *campos)
	{
	//variáveis locais
	sdi_CIRCUITO sdi;
	int          nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "Circuito");
		//valida número de dados recebidos
		if (campos->Count < 31)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase     = campos->Strings[nc++];
		sdi.subestacao  = campos->Strings[nc++];
		sdi.codigo      = campos->Strings[nc++];
		sdi.vnom_kv     = StrToDouble(campos->Strings[nc++]);
		sdi.vope_pu     = StrToDouble(campos->Strings[nc++]);
		sdi.codigo_pto  = campos->Strings[nc++];
		sdi.tipico      = StrToInt(campos->Strings[nc++]);
		for (int n = 0; n < 12; n++)
			{
			sdi.energia_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		for (int n = 0; n < 12; n++)
			{
			sdi.perda_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		//insere Circuito em PreMonta
		if (! pre_monta->InsereCircuito(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCondutor(TStrings *campos)
	{
	//variáveis locais
	sdi_CONDUTOR sdi;
	int          nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "Condutor");
		//valida número de dados recebidos
		if (campos->Count < 6)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase = campos->Strings[nc++];
		sdi.codigo  = campos->Strings[nc++];
		sdi.r_ohm_km = StrToDouble(campos->Strings[nc++]);
		sdi.x_ohm_km = StrToDouble(campos->Strings[nc++]);
		sdi.i_max_amp = StrToDouble(campos->Strings[nc++]);
		sdi.descricao = campos->Strings[nc++];
		//insere Condutor em PreMonta
		if (! pre_monta->InsereCondutor(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCoordenada(TStrings *campos)
	{
	//variáveis locais
	sdi_COORDENADA sdi;
	int            nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "Coordenada");
		//valida número de dados recebidos
		if (campos->Count < 4)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase = campos->Strings[nc++];
		sdi.codigo  = campos->Strings[nc++];
		sdi.utm_m.x = StrToDouble(campos->Strings[nc++]);
		sdi.utm_m.y = StrToDouble(campos->Strings[nc++]);
		//insere Coordenada em PreMonta
		if (! pre_monta->InsereCoordenada(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereGeradorBT(TStrings *campos)
	{
	//variáveis locais
	sdi_GERADOR_BT sdi;
	int            nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "GeradorBT");
		//valida número de dados recebidos
		if (campos->Count < 17)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase     = campos->Strings[nc++];
		sdi.ramal       = campos->Strings[nc++];
		sdi.codigo      = campos->Strings[nc++];
		sdi.codigo_pto  = campos->Strings[nc++];
		sdi.fases       = campos->Strings[nc++];
		for (int n = 0; n < 12; n++)
			{
			sdi.energia_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		//insere GeradorBT em PreMonta
		if (! pre_monta->InsereGeradorBT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereGeradorMT(TStrings *campos)
	{
	//variáveis locais
	sdi_GERADOR_MT sdi;
	int            nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "GeradorMT");
		//valida número de dados recebidos
		if (campos->Count < 17)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase     = campos->Strings[nc++];
		sdi.circuito    = campos->Strings[nc++];
		sdi.codigo      = campos->Strings[nc++];
		sdi.codigo_pto  = campos->Strings[nc++];
		sdi.fases       = campos->Strings[nc++];
		for (int n = 0; n < 12; n++)
			{
			sdi.energia_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		//insere GeradorMT em PreMonta
		if (! pre_monta->InsereGeradorMT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereRamalBT(TStrings *campos)
	{
	//variáveis locais
	sdi_RAMAL_BT sdi;
	int          nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "Ramal");
		//valida número de dados recebidos
		if (campos->Count < 8)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase         = campos->Strings[nc++];
		sdi.rede            = campos->Strings[nc++];
		sdi.codigo          = campos->Strings[nc++];
		sdi.codigo_pto1     = campos->Strings[nc++];
		sdi.codigo_pto2     = campos->Strings[nc++];
		sdi.fases           = campos->Strings[nc++];
		sdi.codigo_condutor = campos->Strings[nc++];
		sdi.comprimento_km  = StrToDouble(campos->Strings[nc++]);
		//insere SegmentoBT em PreMonta
		if (! pre_monta->InsereRamalBT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorMT(TStrings *campos)
	{
	//variáveis locais
	sdi_REGULADOR sdi;
	int           nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "Regulador");
		//valida número de dados recebidos
		if (campos->Count < 15)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.circuito        = campos->Strings[nc++];
		sdi.codigo          = campos->Strings[nc++];
		sdi.sequencial      = StrToInt(campos->Strings[nc++]);
		sdi.codigo_pto1     = campos->Strings[nc++];
		sdi.codigo_pto2     = campos->Strings[nc++];
		sdi.snom_kva        = StrToDouble(campos->Strings[nc++]);
		sdi.tipo            = StrToInt(campos->Strings[nc++]);
		sdi.vreg_pu         = StrToDouble(campos->Strings[nc++]);
		sdi.fases_pri       = campos->Strings[nc++];
		sdi.fases_sec       = campos->Strings[nc++];
		sdi.r_perc          = StrToDouble(campos->Strings[nc++]);
		sdi.xhl_perc        = StrToDouble(campos->Strings[nc++]);
		sdi.perda_total_w   = StrToDouble(campos->Strings[nc++]);
		sdi.perda_vazio_w   = StrToDouble(campos->Strings[nc++]);
		sdi.energia_mwhano  = StrToDouble(campos->Strings[nc++]);
		//insere ReguladorMT em PreMonta
		if (! pre_monta->InsereReguladorMT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereSegmentoBT(TStrings *campos)
	{
	//variáveis locais
	sdi_SEGMENTO_BT sdi;
	int             nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "SegmentoBT");
		//valida número de dados recebidos
		if (campos->Count < 8)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase         = campos->Strings[nc++];
		sdi.rede            = campos->Strings[nc++];
		sdi.codigo          = campos->Strings[nc++];
		sdi.codigo_pto1     = campos->Strings[nc++];
		sdi.codigo_pto2     = campos->Strings[nc++];
		sdi.fases           = campos->Strings[nc++];
		sdi.codigo_condutor = campos->Strings[nc++];
		sdi.comprimento_km  = StrToDouble(campos->Strings[nc++]);
		//insere SegmentoBT em PreMonta
		if (! pre_monta->InsereSegmentoBT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereSegmentoMT(TStrings *campos)
	{
	//variáveis locais
	sdi_SEGMENTO_MT sdi;
	int             nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "SegmentoMT");
		//valida número de dados recebidos
		if (campos->Count < 8)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase         = campos->Strings[nc++];
		sdi.circuito        = campos->Strings[nc++];
		sdi.codigo          = campos->Strings[nc++];
		sdi.codigo_pto1     = campos->Strings[nc++];
		sdi.codigo_pto2     = campos->Strings[nc++];
		sdi.fases           = campos->Strings[nc++];
		sdi.codigo_condutor = campos->Strings[nc++];
		sdi.comprimento_km  = StrToDouble(campos->Strings[nc++]);
		sdi.classificacao   = campos->Strings[nc++];
		//insere SegmentoMT em PreMonta
		if (! pre_monta->InsereSegmentoMT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafoAT_MT(TStrings *campos)
	{
	//variáveis locais
	sdi_TRAFO_AT_MT sdi;
	int             nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "TrafoAT_MT");
		//valida número de dados recebidos
		if (campos->Count < 25)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase           = campos->Strings[nc++];
		sdi.subestacao        = campos->Strings[nc++];
		sdi.codigo            = campos->Strings[nc++];
		sdi.sequencial        = StrToInt(campos->Strings[nc++]);
		sdi.tipo              = StrToInt(campos->Strings[nc++]);
		sdi.vnom_pri_kv       = StrToDouble(campos->Strings[nc++]);
		sdi.vnom_sec_kv       = StrToDouble(campos->Strings[nc++]);
		sdi.vnom_ter_kv       = StrToDouble(campos->Strings[nc++]);
		sdi.snom_mva          = StrToDouble(campos->Strings[nc++]);
		sdi.perda_vazio_perc  = StrToDouble(campos->Strings[nc++]);
		sdi.perda_total_perc  = StrToDouble(campos->Strings[nc++]);
		for (int n = 0; n < 12; n++)
			{
			sdi.energia_mwhmes[n] = StrToDouble(campos->Strings[nc++]);
			}
		sdi.nivel_tensao      = campos->Strings[nc++];
		sdi.propriedade       = campos->Strings[nc++];
		//insere TrafoAT_MT em PreMonta
		if (! pre_monta->InsereTrafoAT_MT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafoMT_BT(TStrings *campos)
	{
	//variáveis locais
	sdi_TRAFO_MT_BT sdi;
	int             nc = 0;

	try{//configura Log
		if (plog) plog->DefineValor("Eqpto", "TrafoMT_BT");
		//valida número de dados recebidos
		if (campos->Count < 22)
			{
			if (plog) plog->Add("Número de campos menor do que o esperado");
			return(false);
			}
		sdi.codbase           = campos->Strings[nc++];
		sdi.circuito          = campos->Strings[nc++];
		sdi.codigo_trafo      = campos->Strings[nc++];
		sdi.sequencial        = StrToInt(campos->Strings[nc++]);
		sdi.codigo_pto1       = campos->Strings[nc++];
		sdi.codigo_pto2       = campos->Strings[nc++];
		sdi.snom_kva          = StrToDouble(campos->Strings[nc++]);
		sdi.mrt               = StrToInt(campos->Strings[nc++]);
		sdi.tipo              = StrToInt(campos->Strings[nc++]);
		sdi.fases_pri         = campos->Strings[nc++];
		sdi.fases_sec         = campos->Strings[nc++];
		sdi.fases_ter         = campos->Strings[nc++];
		sdi.vff_sec_kv        = StrToDouble(campos->Strings[nc++]);
		sdi.vtap_pu           = StrToDouble(campos->Strings[nc++]);
		sdi.r_perc            = StrToDouble(campos->Strings[nc++]);
		sdi.xhl_perc          = StrToDouble(campos->Strings[nc++]);
		sdi.xht_perc          = StrToDouble(campos->Strings[nc++]);
		sdi.xlt_perc          = StrToDouble(campos->Strings[nc++]);
		sdi.perda_total_perc  = StrToDouble(campos->Strings[nc++]);
		sdi.perda_vazio_perc  = StrToDouble(campos->Strings[nc++]);
		sdi.classificacao     = campos->Strings[nc++];
		sdi.propriedade       = campos->Strings[nc++];
		//insere TrafoMT_BT em PreMonta
		if (! pre_monta->InsereTrafoMT_BT(sdi)) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereVersao(TStrings *campos)
	{
	//atualiza  Resumo
	if (plog) plog->DefineValor("Eqpto", "Versão");
	//valida número de dados recebidos
	if (campos->Count < 4)
		{
		if (plog) plog->Add("Número de campos menor do que o esperado");
		return(false);
		}
	//valida campos da versão do arquivo
	if (campos->Strings[0].CompareIC("Sinap")           != 0) return(false);
	if (campos->Strings[1].CompareIC("Interface ANEEL") != 0) return(false);
	//verifica versão 1.0
	if (campos->Strings[2].CompareIC("Rev1.0") == 0) return(true);
	//versão inválida
   if (plog) plog->Add("Arquivo com versão inválida");
	return(false);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::Sucesso(void)
	{
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::TrataFimDados(void)
	{
	//nada a fazer: sinaliza PreMonta
	if (! pre_monta->TrataFimDados()) return(false);
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::TrataIniciaDados(AnsiString cod_dominio)
   {
   //inicia flag que indica sucesso/falha na montagem da rede
   sucesso = true;
   //inicia ponteiros p/ objetos
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
   //reinicia PreMonta
   pre_monta->TrataIniciaDados(cod_dominio);

   return(true);
	}



/*
//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereArranjo(TStrings *campos)
	{
	//variáveis locais
	int         nc;
	sdi_ARRANJO sdi;

	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Arranjo");
	//valida número de dados recebidos
	if (campos->Count < 6)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.tipo      = arranjoZ0Z1;
      sdi.id        = StrToInteger(campos->Strings[nc=0]); 
      sdi.cod       = campos->Strings[++nc];
		sdi.extern_id = "";
		sdi.fases     = "ABCN";
		//resistência e reatância
		sdi.z1_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		//corrente máxima
		sdi.iadm_amp  = StrToDouble(campos->Strings[++nc]);
		}catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Barra
	if (! pre_monta->InsereArranjo(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereBarra(TStrings *campos)
   {
   //variáveis locais
   int       nc;
   sdi_BARRA sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Barra");
   //valida número de dados recebidos
   if (campos->Count < 4)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id        = StrToInteger(campos->Strings[nc=0]);
      sdi.cod       = campos->Strings[++nc];              
      sdi.extern_id = campos->Strings[++nc];
      sdi.vnom_kv   = StrToDouble(campos->Strings[++nc]);
      //converte coordenadas utm de m p/ cm
      sdi.utm_m.x   = StrToDouble(campos->Strings[++nc]);
      sdi.utm_m.y   = StrToDouble(campos->Strings[++nc]);
      sdi.esq.x1    = StrToInteger(campos->Strings[++nc]);
      sdi.esq.y1    = StrToInteger(campos->Strings[++nc]);
      sdi.esq.x2    = StrToInteger(campos->Strings[++nc]);
      sdi.esq.y2    = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Barra
   if (! pre_monta->InsereBarra(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExtrator::InsereBarraStart(void)
   {
   pre_monta->InsereBarraStart();
   }

//---------------------------------------------------------------------------
void __fastcall TExtrator::InsereBarraStop(void)
   {
   pre_monta->InsereBarraStop();
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCapacitor(TStrings *campos)
	{
	//variáveis locais
	int           nc;
	sdi_CAPACITOR sdi;

	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Capacitor");
	//valida número de dados recebidos
	if (campos->Count < 10)
		{
		if (plog) plog->Add("Número de campos menor do que o esperado");
		return(false);
		}
	try{//lê campos
      sdi.id          = StrToInteger(campos->Strings[nc=0]);
      sdi.id_barra    = StrToInteger(campos->Strings[++nc]);
      sdi.cod         = campos->Strings[++nc];
      sdi.extern_id   = campos->Strings[++nc];
      sdi.vnom_kv     = StrToDouble(campos->Strings[++nc]);
      sdi.snom_kva    = StrToDouble(campos->Strings[++nc]);
      sdi.ligacao     = campos->Strings[++nc];
		//esquematico
      sdi.esq.posbar  = StrToDouble(campos->Strings[++nc]);
      sdi.esq.dxbar   = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar   = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Capacitor
   if (! pre_monta->InsereCapacitor(sdi)) return(false);
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCapacitorSerie(TStrings *campos)
   {
   //variáveis locais
   int          nc;
   sdi_CAPSERIE sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Capacitor Série");
   //valida número de dados recebidos
   if (campos->Count < 8)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id             = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar1        = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar2        = StrToInteger(campos->Strings[++nc]);
      sdi.cod            = campos->Strings[++nc];              
      sdi.extern_id      = campos->Strings[++nc];
      sdi.vnom_kv        = StrToDouble(campos->Strings[++nc]);
      sdi.snom_kva       = StrToDouble(campos->Strings[++nc]); 
      sdi.z_ohm.r        = StrToDouble(campos->Strings[++nc]);
      sdi.z_ohm.x        = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar1    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar2    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar3    = 0;
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Capacitor
   if (! pre_monta->InsereCapacitorSerie(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaIP(TStrings *campos)
   {
   //variáveis locais
   int          nc;
   sdi_CARGA_IP sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Carga IP");
   //valida número de dados recebidos
   if (campos->Count < 10)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id           = StrToInteger(campos->Strings[nc=0]);
      sdi.id_barra     = StrToInteger(campos->Strings[++nc]);
      sdi.cod          = campos->Strings[++nc];              
      sdi.extern_id    = campos->Strings[++nc];
      sdi.fases        = campos->Strings[++nc];
      sdi.num_lampada  = StrToInteger(campos->Strings[++nc]);
      sdi.p_w          = StrToDouble(campos->Strings[++nc]); 
      sdi.fp_pu        = StrToDouble(campos->Strings[++nc]); 
      //esquematico
      sdi.esq.posbar   = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.dxbar    = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar    = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere ConsumidorBT
   if (pre_monta->InsereCargaIP(sdi) == NULL) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaTipica(TStrings *campos)
   {
   //variáveis locais
   int       nc;
   sdi_CARGA sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Carga Típica");
   //valida número de dados recebidos
   if (campos->Count < 13)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id           = StrToInteger(campos->Strings[nc=0]);
      sdi.id_barra     = StrToInteger(campos->Strings[++nc]);
      sdi.cod          = campos->Strings[++nc];              
      sdi.extern_id    = campos->Strings[++nc];
      sdi.grupo_tarifa = campos->Strings[++nc];
      sdi.classe       = campos->Strings[++nc];              
      sdi.fases        = campos->Strings[++nc];              
      sdi.i_cte_pu     = StrToDouble(campos->Strings[++nc]); 
      sdi.s_cte_pu     = StrToDouble(campos->Strings[++nc]); 
      sdi.z_cte_pu     = StrToDouble(campos->Strings[++nc]); 
      sdi.consumo_kwhm = StrToDouble(campos->Strings[++nc]); 
      //esquematico
      sdi.esq.posbar   = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.dxbar    = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar    = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
			if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
			return(false);
         }
   //insere ConsumidorBT
   if (pre_monta->InsereCargaTipica(sdi) == NULL) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereChave(TStrings *campos)
   {
   //variáveis locais
   int       nc;
   sdi_CHAVE sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Chave");
   //valida número de dados recebidos
   if (campos->Count < 13)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id                = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar1           = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar2           = StrToInteger(campos->Strings[++nc]);
      sdi.tipochave.id      = StrToInteger(campos->Strings[++nc]);
      sdi.tipochave.tipo    = StrToInteger(campos->Strings[++nc]);
      sdi.tipochave.cod     = campos->Strings[++nc];
      sdi.cod               = campos->Strings[++nc];
      sdi.extern_id         = campos->Strings[++nc];
      sdi.iadm_amp          = StrToDouble(campos->Strings[++nc]);
      sdi.estado            = StrToInteger(campos->Strings[++nc]);
      sdi.protecao_ativa    = bool(StrToInteger(campos->Strings[++nc]));
      sdi.telecomandada     = bool(StrToInteger(campos->Strings[++nc]));
      sdi.tecno_comunicacao = StrToInteger(campos->Strings[++nc]);

sdi.tecno_comunicacao = 0;

      //esquematico
      sdi.esq.posbar1    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar2    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar3    = 0;
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Chave
   if (! pre_monta->InsereChave(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereFiltro(TStrings *campos)
	{
	//variáveis locais
	int        nc;
	sdi_FILTRO sdi;

	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Filtro");
	//valida número de dados recebidos
	if (campos->Count < 10)
		{
		if (plog) plog->Add("Número de campos menor do que o esperado");
		return(false);
		}
	try{//lê campos
		sdi.id         = StrToInteger(campos->Strings[nc=0]);
		sdi.id_bar     = StrToInteger(campos->Strings[++nc]);
		sdi.cod        = campos->Strings[++nc];
		sdi.extern_id  = campos->Strings[++nc];
		sdi.vnom_kv    = StrToDouble(campos->Strings[++nc]);
		sdi.snom_kva   = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r    = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x    = StrToDouble(campos->Strings[++nc]);
      //esquematico
      sdi.esq.posbar = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.dxbar  = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar  = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Capacitor
   if (! pre_monta->InsereFiltro(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereGerador(TStrings *campos)
   {
   //variáveis locais
   int         nc;
   sdi_GERADOR sdi;

	//atualiza  Resumo
   if (plog) plog->DefineValor("Eqpto", "Gerador");
   //valida número de dados recebidos (sem considerar a curva)
   if (campos->Count < 16)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê dados
      sdi.id             = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar         = StrToInteger(campos->Strings[++nc]);
      sdi.cod            = campos->Strings[++nc];
      sdi.extern_id      = campos->Strings[++nc];
      sdi.tipo_geracao   = StrToInteger(campos->Strings[++nc]);
      sdi.vnom_kv        = StrToDouble(campos->Strings[++nc]);
      sdi.snom_kva       = StrToDouble(campos->Strings[++nc]); 
      sdi.fplim_pu       = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.r        = StrToDouble(campos->Strings[++nc]); 
      sdi.z0_pu.x        = StrToDouble(campos->Strings[++nc]); 
      sdi.z1_pu.r        = StrToDouble(campos->Strings[++nc]); 
      sdi.z1_pu.x        = StrToDouble(campos->Strings[++nc]); 
      sdi.ligacao        = StrToInteger(campos->Strings[++nc]);
      sdi.tipo_sup       = StrToInteger(campos->Strings[++nc]);
		//esquematico
      sdi.esq.posbar  = StrToDouble(campos->Strings[++nc]);
      sdi.esq.dxbar   = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar   = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Suprimento
   if (! pre_monta->InsereGerador(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereMutua(TStrings *campos)
   {
   //variáveis locais
   int       nc;
   sdi_MUTUA sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Mútua");
   //valida número de dados recebidos
   if (campos->Count < 12)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id          = StrToInteger(campos->Strings[nc=0]);
      sdi.id_tre1     = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar_tre1 = StrToInteger(campos->Strings[++nc]);
      sdi.id_tre2     = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar_tre2 = StrToInteger(campos->Strings[++nc]);
      sdi.cod         = campos->Strings[++nc];              
      sdi.extern_id   = campos->Strings[++nc];
      sdi.z0.r        = StrToDouble(campos->Strings[++nc]);
      sdi.z0.x        = StrToDouble(campos->Strings[++nc]); 
      sdi.z1.r        = StrToDouble(campos->Strings[++nc]); 
      sdi.z1.x        = StrToDouble(campos->Strings[++nc]); 
      //esquematico
      sdi.postre1     = StrToDouble(campos->Strings[++nc]); 
      sdi.postre2     = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Capacitor
   if (! pre_monta->InsereMutua(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReator(TStrings *campos)
   {
   //variáveis locais
   int        nc;
   sdi_REATOR sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Reator");
   //valida número de dados recebidos
   if (campos->Count < 11)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id         = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.cod        = campos->Strings[++nc];
      sdi.extern_id  = campos->Strings[++nc];
      sdi.ligacao    = StrToInteger(campos->Strings[++nc]);
      sdi.vnom_kv    = StrToDouble(campos->Strings[++nc]); 
      sdi.q_kvar     = StrToDouble(campos->Strings[++nc]); 
		//esquematico
      sdi.esq.posbar = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.dxbar  = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar  = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Reator
   if (! pre_monta->InsereReator(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereRede(TStrings *campos)
   {
   //variáveis locais
   int     nc;
   sdi_REDE sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Rede");
   //valida número de dados recebidos
   if (campos->Count < 6)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id           = StrToInteger(campos->Strings[nc=0]);
      sdi.tiporede.id  = StrToInteger(campos->Strings[++nc]);
      sdi.tiporede.seg = StrToInteger(campos->Strings[++nc]);
      sdi.tiporede.cod = campos->Strings[++nc];
      sdi.cod          = campos->Strings[++nc];
      sdi.extern_id    = campos->Strings[++nc];
      sdi.area         = campos->Strings[++nc];
      if (campos->Count >= 9)
         {//lê informação de cor
         sdi.color_r = StrToInteger(campos->Strings[++nc]);
         sdi.color_g = StrToInteger(campos->Strings[++nc]);
         sdi.color_b = StrToInteger(campos->Strings[++nc]);
         }
      else
         {//cor defaul: black
         sdi.color_r = 0;
         sdi.color_g = 0;
         sdi.color_b = 0;
         }
      if (campos->Count >= 10)
         {
         sdi.cluster = campos->Strings[++nc];
         }
      else
         {
         sdi.cluster = "";
         }
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere SE em PreMonta
   if (! pre_monta->InsereRede(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorAutomatico(TStrings *campos)
   {
   //variáveis locais
   int           nc;
   sdi_REGULADOR sdi;

	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Regulador Auto");
   //valida número de dados recebidos
   if (campos->Count < 16)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id                 = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar1            = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar2            = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar_ent         = StrToInteger(campos->Strings[++nc]);
      sdi.modelo             = StrToInteger(campos->Strings[++nc]);
      sdi.cod                = campos->Strings[++nc];
      sdi.extern_id          = campos->Strings[++nc];
      sdi.snom_kva           = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.r            = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.x            = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.r            = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.x            = StrToDouble(campos->Strings[++nc]);
      sdi.num_eqpto          = StrToInteger(campos->Strings[++nc]);
      sdi.ligacao            = campos->Strings[++nc];
      sdi.fases              = campos->Strings[++nc];
      sdi.dv_perc            = StrToDouble(campos->Strings[++nc]);
      sdi.num_passo          = StrToInteger(campos->Strings[++nc]);
      //tap ajustável
      sdi.ajuste_auto.id_bar_ref = StrToInteger(campos->Strings[++nc]);
      sdi.ajuste_auto.v_pu       = StrToDouble(campos->Strings[++nc]); 
      //esquematico
      sdi.esq.posbar1            = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.posbar2            = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.posbar3            = 0;
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere SE em PreMonta
   if (! pre_monta->InsereReguladorAutomatico(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorTapFixo(TStrings *campos)
   {
   //variáveis locais
   int           nc;
   sdi_REGULADOR sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Regulador Fixo");
   //valida número de dados recebidos
   if (campos->Count < 16)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id                 = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar1            = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar2            = StrToInteger(campos->Strings[++nc]);
      sdi.id_bar_ent         = StrToInteger(campos->Strings[++nc]);
      sdi.modelo             = StrToInteger(campos->Strings[++nc]);
      sdi.cod                = campos->Strings[++nc];
      sdi.extern_id          = campos->Strings[++nc];
      sdi.snom_kva           = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.r            = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.x            = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.r            = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.x            = StrToDouble(campos->Strings[++nc]);
      sdi.num_eqpto          = StrToInteger(campos->Strings[++nc]);
      sdi.ligacao            = campos->Strings[++nc];
      sdi.fases              = campos->Strings[++nc];
      sdi.dv_perc            = StrToDouble(campos->Strings[++nc]);
      sdi.num_passo          = StrToInteger(campos->Strings[++nc]);
      //tap fixo
      sdi.ajuste_fixo.passo  = StrToInteger(campos->Strings[++nc]);
      //esquematico
      sdi.esq.posbar1        = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar2        = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar3        = 0;
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere SE em PreMonta
   if (! pre_monta->InsereReguladorTapFixo(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereSuprimento(TStrings *campos)
	{
	//variáveis locais
	int            nc;
	sdi_SUPRIMENTO sdi;

	//atualiza  Resumo
   if (plog) plog->DefineValor("Eqpto", "Suprimento");
   //valida número de dados recebidos
   if (campos->Count < 9)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê dados
      sdi.id             = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar         = StrToInteger(campos->Strings[++nc]);
      sdi.cod            = campos->Strings[++nc];
      sdi.extern_id      = campos->Strings[++nc];
      sdi.tipo_sup       = StrToInteger(campos->Strings[++nc]);
      sdi.fases          = campos->Strings[++nc];
      sdi.vnom_kv        = StrToDouble(campos->Strings[++nc]);
      sdi.smax_kva       = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.r        = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.x        = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.r        = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.x        = StrToDouble(campos->Strings[++nc]);
		//esquematico
		sdi.esq.posbar  = StrToDouble(campos->Strings[++nc]);
      sdi.esq.dxbar   = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar   = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere Suprimento
   if (! pre_monta->InsereSuprimento(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafo2E(TStrings *campos)
	{
	//variáveis locais
	int         nc;
	sdi_TRAFO2E sdi;

	//atualiza  Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo2E");
   //valida número de dados recebidos
   if (campos->Count < 24)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê dados
      sdi.id             = StrToInteger(campos->Strings[nc=0]);
      sdi.cod            = campos->Strings[++nc];
      sdi.extern_id      = campos->Strings[++nc];
      sdi.snom_kva       = StrToDouble(campos->Strings[++nc]);
      sdi.sbase_kva      = StrToDouble(campos->Strings[++nc]);
      sdi.perda_fe_w     = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.r        = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.x        = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.r        = StrToDouble(campos->Strings[++nc]);
      sdi.z1_pu.x        = StrToDouble(campos->Strings[++nc]);
      //primário
      sdi.pri.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.pri.ligacao    = campos->Strings[++nc];
      sdi.pri.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.pri.vtap_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.pri.zat_ohm.r  = StrToDouble(campos->Strings[++nc]);
      sdi.pri.zat_ohm.x  = StrToDouble(campos->Strings[++nc]);
      //secundário
      sdi.sec.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.sec.ligacao    = campos->Strings[++nc];
      sdi.sec.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.sec.vtap_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.sec.zat_ohm.r  = StrToDouble(campos->Strings[++nc]);
      sdi.sec.zat_ohm.x  = StrToDouble(campos->Strings[++nc]);
      //ajuste
      sdi.ajuste.tipo    = campos->Strings[++nc];
      sdi.ajuste.v_pu    = StrToDouble(campos->Strings[++nc]);
      //esquematico
      sdi.esq.posbar1    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar2    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar3    = 0;
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere TrafoSE
   if (! pre_monta->InsereTrafo2E(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafo3E(TStrings *campos)
   {
   //variáveis locais
   int       nc;
   sdi_TRAFO3E sdi;

	//atualiza  Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo3E");
   //valida número de dados recebidos
   if (campos->Count < 38)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê dados
      sdi.id             = StrToInteger(campos->Strings[nc=0]);
      sdi.cod            = campos->Strings[++nc];              
      sdi.extern_id      = campos->Strings[++nc];
      //primário
      sdi.pri.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.pri.ligacao    = campos->Strings[++nc];
      sdi.pri.snom_kva   = StrToDouble(campos->Strings[++nc]);
      sdi.pri.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.pri.vtap_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.pri.zat_ohm.r  = StrToDouble(campos->Strings[++nc]);
      sdi.pri.zat_ohm.x  = StrToDouble(campos->Strings[++nc]);
      //secundário
      sdi.sec.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.sec.ligacao    = campos->Strings[++nc];
      sdi.sec.snom_kva   = StrToDouble(campos->Strings[++nc]);
      sdi.sec.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.sec.vtap_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.sec.zat_ohm.r  = StrToDouble(campos->Strings[++nc]);
      sdi.sec.zat_ohm.x  = StrToDouble(campos->Strings[++nc]);
      //terciário
      sdi.ter.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.ter.ligacao    = campos->Strings[++nc];
      sdi.ter.snom_kva   = StrToDouble(campos->Strings[++nc]);
      sdi.ter.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.ter.vtap_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.ter.zat_ohm.r  = StrToDouble(campos->Strings[++nc]);
      sdi.ter.zat_ohm.x  = StrToDouble(campos->Strings[++nc]);
      //perda no ferro
      sdi.perda_fe_w     = StrToDouble(campos->Strings[++nc]);
      //potência de base p/ valores em pu
      sdi.sbase_kva      = StrToDouble(campos->Strings[++nc]);
      //impedância em pu
      sdi.zps0_pu.r      = StrToDouble(campos->Strings[++nc]);
      sdi.zps0_pu.x      = StrToDouble(campos->Strings[++nc]);
      sdi.zps1_pu.r      = StrToDouble(campos->Strings[++nc]);
      sdi.zps1_pu.x      = StrToDouble(campos->Strings[++nc]);
      sdi.zpt0_pu.r      = StrToDouble(campos->Strings[++nc]);
      sdi.zpt0_pu.x      = StrToDouble(campos->Strings[++nc]);
      sdi.zpt1_pu.r      = StrToDouble(campos->Strings[++nc]);
      sdi.zpt1_pu.x      = StrToDouble(campos->Strings[++nc]);
      sdi.zst0_pu.r      = StrToDouble(campos->Strings[++nc]);
      sdi.zst0_pu.x      = StrToDouble(campos->Strings[++nc]);
      sdi.zst1_pu.r      = StrToDouble(campos->Strings[++nc]);
      sdi.zst1_pu.x      = StrToDouble(campos->Strings[++nc]);
      //esquematico
      sdi.esq.posbar1    = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.posbar2    = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.posbar3    = StrToDouble(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere TrafoSE
   if (! pre_monta->InsereTrafo3E(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafoMono(TStrings *campos)
   {
   //variáveis locais
   int            nc;
   sdi_TRAFO_MONO sdi;

	//atualiza  Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo Monofásico");
   //valida número de dados recebidos
   if (campos->Count < 23)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê dados
      sdi.id             = StrToInteger(campos->Strings[nc=0]);
      sdi.cod            = campos->Strings[++nc];
      sdi.extern_id      = campos->Strings[++nc];
      sdi.snom_kva       = StrToDouble(campos->Strings[++nc]);
      sdi.perda_fe_w     = StrToDouble(campos->Strings[++nc]);
      sdi.sbase_kva      = StrToDouble(campos->Strings[++nc]);
      sdi.z_pu.r         = StrToDouble(campos->Strings[++nc]);
      sdi.z_pu.x         = StrToDouble(campos->Strings[++nc]);
      //primário
      sdi.pri.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.pri.fases      = campos->Strings[++nc];
      sdi.pri.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.pri.vtap_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.pri.zat_ohm.r  = StrToDouble(campos->Strings[++nc]);
      sdi.pri.zat_ohm.x  = StrToDouble(campos->Strings[++nc]);
      //secundário
      sdi.sec.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.sec.fases      = campos->Strings[++nc];
      sdi.sec.vnom_kv    = StrToDouble(campos->Strings[++nc]); 
      sdi.sec.vtap_kv    = StrToDouble(campos->Strings[++nc]); 
      sdi.sec.zat_ohm.r  = StrToDouble(campos->Strings[++nc]); 
      sdi.sec.zat_ohm.x  = StrToDouble(campos->Strings[++nc]); 
      //ajuste
      sdi.ajuste.tipo    = campos->Strings[++nc];              
      sdi.ajuste.v_pu    = StrToDouble(campos->Strings[++nc]); 
      //esquematico
      sdi.esq.posbar1    = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.posbar2    = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar3    = 0;
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere TrafoSE
   if (! pre_monta->InsereTrafoMono(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafoZZ(TStrings *campos)
   {
   //variáveis locais
   int         nc;
   sdi_TRAFOZZ sdi;

	//configura Log
   if (plog) plog->DefineValor("Eqpto", "Trafo ZigZag");
   //valida número de dados recebidos
   if (campos->Count < 10)
      {
      if (plog) plog->Add("Número de campos menor do que o esperado");
      return(false);
      }
   try{//lê campos
      sdi.id         = StrToInteger(campos->Strings[nc=0]);
      sdi.id_bar     = StrToInteger(campos->Strings[++nc]);
      sdi.cod        = campos->Strings[++nc];              
      sdi.extern_id  = campos->Strings[++nc];
      sdi.vnom_kv    = StrToDouble(campos->Strings[++nc]);
      sdi.snom_kva   = StrToDouble(campos->Strings[++nc]); 
      sdi.z0_pu.r    = StrToDouble(campos->Strings[++nc]);
      sdi.z0_pu.x    = StrToDouble(campos->Strings[++nc]);
      //esquematico
      sdi.esq.posbar = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.dxbar  = StrToInteger(campos->Strings[++nc]);
      sdi.esq.dybar  = StrToInteger(campos->Strings[++nc]);
      }catch(Exception &e)
         {
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere TrafoZZ
   if (! pre_monta->InsereTrafoZZ(sdi)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrecho(TStrings *campos)
	{
	//variáveis locais
	int        nc;
	sdi_TRECHO sdi;

	//atualiza  Resumo
	if (plog) plog->DefineValor("Eqpto", "Trecho típico");
	//valida número de dados recebidos
	if (campos->Count < 8)
		{
		if (plog) plog->Add("Número de campos menor do que o esperado");
		return(false);
		}
	try{//lê campos
		sdi.id        = StrToInteger(campos->Strings[nc=0]);
		sdi.id_bar1   = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2   = StrToInteger(campos->Strings[++nc]);
		sdi.cod       = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
      sdi.comp_m    = StrToDouble(campos->Strings[++nc]);
      //arranjo típico
      sdi.arranjo_tipico.id_arranjo = StrToInteger(campos->Strings[++nc]);
		//esquematico
      sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
      sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]); 
      sdi.esq.posbar3 = 0;
      }catch(Exception &e)
			{
         if (plog) plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc+1));
         return(false);
         }
   //insere TrechoBT
	if (! pre_monta->InsereTrecho(sdi)) return(false);
   return(true);
   }


*/

//-----------------------------------------------------------------------------
// eof
