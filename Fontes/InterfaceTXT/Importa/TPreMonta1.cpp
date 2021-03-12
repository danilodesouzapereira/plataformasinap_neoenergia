//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
#include "TPreMonta1.h"

//---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta1(VTApl *apl)
   {
   try{//cria objeto TPreMonta1
      return(new TPreMonta1(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta1::TPreMonta1(VTApl *apl)
                      :TPreMonta(apl)
   {
   //inicia controle indicando que não está importando uma rede secundária
   et.enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta1::~TPreMonta1(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereBarra(sdi_BARRA &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereBarra(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCapacitor(sdi_CAPACITOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCapacitor(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCapacitorSerie(sdi_CAPSERIE &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCapacitorSerie(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta1::InsereCargaComMedicao(sdi_CARGA &sdi)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCargaComMedicao(sdi));
   //proteção
	if (et.barra_mt == NULL)
		{//DVK 2015.11.04 insere aviso no log
		if(plog) plog->Add("Não existe barra MT da rede secundária");
		return(NULL);
		}
   //cria Carga temporária associada à Barra mt da rede secundária
   sdi.id_barra = -et.barra_mt->Id;
   if ((carga = TPreMonta::InsereCargaComMedicao(sdi)) != NULL)
      {//atualiza demanda da Carga equivalente
      AtualizaCargaEquivalente(et.carga_eqv, carga);
      //remove Carga temporária e destrói objeto
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   return(et.carga_eqv);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta1::InsereCargaIP(sdi_CARGA_IP &sdi)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCargaIP(sdi));
   //proteção
	if (et.barra_mt == NULL)
		{//DVK 2015.11.04 insere aviso no log
		if(plog) plog->Add("Não existe barra MT da rede secundária");
		return(NULL);
		}
   //cria Carga temporária associada à Barra mt da rede secundária
   sdi.id_barra = -et.barra_mt->Id;
   if ((carga = TPreMonta::InsereCargaIP(sdi)) != NULL)
      {//atualiza demanda da Carga equivalente
      AtualizaCargaEquivalente(et.carga_eqv, carga);
      //remove Carga temporária e destrói objeto
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   return(et.carga_eqv);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta1::InsereCargaTipica(sdi_CARGA &sdi)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se está montando uma rede secundária
	if (! et.enabled) return(TPreMonta::InsereCargaTipica(sdi));
   //proteção
	if (et.barra_mt == NULL)
		{//DVK 2015.10.22 insere aviso no log
		if(plog) plog->Add("Não existe barra MT da rede secundária");
		return(NULL);
		}
   //cria Carga temporária associada à Barra mt da rede secundária
   sdi.id_barra = -et.barra_mt->Id;
   if ((carga = TPreMonta::InsereCargaTipica(sdi)) != NULL)
      {//atualiza demanda da Carga equivalente
      AtualizaCargaEquivalente(et.carga_eqv, carga);
      //remove Carga temporária e destrói objeto
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   return(et.carga_eqv);
	}

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereChave(sdi_CHAVE &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereChave(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereFiltro(sdi_FILTRO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereFiltro(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereGerador(sdi_GERADOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereGerador(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereMutua(sdi_MUTUA &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereMutua(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReator(sdi_REATOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereReator(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereRede(sdi_REDE &sdi)
	{
   //verifica se é uma rede secundária
   et.enabled = (sdi.tiporede.seg == redeSEC);
   //verifica se está montando uma rede secundária
	if (! et.enabled) return(TPreMonta::InsereRede(sdi));
	//reinicia estrutura de controle p/ inserção de carga equivalente
	et.codigo    = sdi.cod; //DVK 2015.10.23
   et.barra_mt  = NULL;
	et.carga_eqv = NULL;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReguladorAutomatico(sdi_REGULADOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereReguladorAutomatico(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReguladorTapFixo(sdi_REGULADOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereReguladorTapFixo(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSuprimento(sdi_SUPRIMENTO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereSuprimento(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafo2E(sdi_TRAFO2E &sdi)
   {
   //veriáveis locais
	int fases_tag, liga;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafo2E(sdi));
   //salva ponteiro p/ Barra MT
   if ((et.barra_mt = monta->ExisteBarra(-sdi.pri.id_bar)) != NULL)
      {//cria Carga equivalente //DVK 2015.11.09 traduz ligação
//		fases_tag = (sdi.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
		liga = TraduzTipoLigacao(sdi.pri.ligacao);
		fases_tag = (liga == lgEST_AT) ? faseABCN : faseABC;
//		et.carga_eqv = CriaCargaEquivalente(et.barra_mt->Id, sdi.cod, fases_tag);
		et.carga_eqv = CriaCargaEquivalente(et.barra_mt->Id, et.codigo, fases_tag);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafo3E(sdi_TRAFO3E &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafo3E(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafoMono(sdi_TRAFO_MONO &sdi)
   {
   //veriáveis locais
   int fases_tag;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafoMono(sdi));
   //salva ponteiro p/ Barra MT
   if ((et.barra_mt = monta->ExisteBarra(-sdi.pri.id_bar)) != NULL)
		{//cria Carga equivalente
		fases_tag = fases->ExisteFase(sdi.pri.fases);
		//verifica se a Carga equivalente já foi criada (devido a um outro trafo monofásico)
      if (et.carga_eqv == NULL)
         {
//         et.carga_eqv = CriaCargaEquivalente(et.barra_mt->Id, sdi.cod, fases_tag);
         et.carga_eqv = CriaCargaEquivalente(et.barra_mt->Id, et.codigo, fases_tag);
			}
      else
         {//atualiza fases da Carga equivalente
         et.carga_eqv->Fases = et.carga_eqv->Fases | fases_tag;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafoZZ(sdi_TRAFOZZ &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafoZZ(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrechoComArranjo(sdi_TRECHO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrechoComArranjo(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrechoZ0Z1(sdi_TRECHO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrechoZ0Z1(sdi));

   return(true);
   }

//-----------------------------------------------------------------------------
// eof



