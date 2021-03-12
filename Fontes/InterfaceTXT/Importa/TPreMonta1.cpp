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
   //inicia controle indicando que n�o est� importando uma rede secund�ria
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
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereBarra(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCapacitor(sdi_CAPACITOR &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereCapacitor(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCapacitorSerie(sdi_CAPSERIE &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereCapacitorSerie(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta1::InsereCargaComMedicao(sdi_CARGA &sdi)
   {
   //vari�veis locais
   VTCarga *carga;

   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereCargaComMedicao(sdi));
   //prote��o
	if (et.barra_mt == NULL)
		{//DVK 2015.11.04 insere aviso no log
		if(plog) plog->Add("N�o existe barra MT da rede secund�ria");
		return(NULL);
		}
   //cria Carga tempor�ria associada � Barra mt da rede secund�ria
   sdi.id_barra = -et.barra_mt->Id;
   if ((carga = TPreMonta::InsereCargaComMedicao(sdi)) != NULL)
      {//atualiza demanda da Carga equivalente
      AtualizaCargaEquivalente(et.carga_eqv, carga);
      //remove Carga tempor�ria e destr�i objeto
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   return(et.carga_eqv);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta1::InsereCargaIP(sdi_CARGA_IP &sdi)
   {
   //vari�veis locais
   VTCarga *carga;

   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereCargaIP(sdi));
   //prote��o
	if (et.barra_mt == NULL)
		{//DVK 2015.11.04 insere aviso no log
		if(plog) plog->Add("N�o existe barra MT da rede secund�ria");
		return(NULL);
		}
   //cria Carga tempor�ria associada � Barra mt da rede secund�ria
   sdi.id_barra = -et.barra_mt->Id;
   if ((carga = TPreMonta::InsereCargaIP(sdi)) != NULL)
      {//atualiza demanda da Carga equivalente
      AtualizaCargaEquivalente(et.carga_eqv, carga);
      //remove Carga tempor�ria e destr�i objeto
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   return(et.carga_eqv);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta1::InsereCargaTipica(sdi_CARGA &sdi)
   {
   //vari�veis locais
   VTCarga *carga;

   //verifica se est� montando uma rede secund�ria
	if (! et.enabled) return(TPreMonta::InsereCargaTipica(sdi));
   //prote��o
	if (et.barra_mt == NULL)
		{//DVK 2015.10.22 insere aviso no log
		if(plog) plog->Add("N�o existe barra MT da rede secund�ria");
		return(NULL);
		}
   //cria Carga tempor�ria associada � Barra mt da rede secund�ria
   sdi.id_barra = -et.barra_mt->Id;
   if ((carga = TPreMonta::InsereCargaTipica(sdi)) != NULL)
      {//atualiza demanda da Carga equivalente
      AtualizaCargaEquivalente(et.carga_eqv, carga);
      //remove Carga tempor�ria e destr�i objeto
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   return(et.carga_eqv);
	}

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereChave(sdi_CHAVE &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereChave(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereFiltro(sdi_FILTRO &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereFiltro(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereGerador(sdi_GERADOR &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereGerador(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereMutua(sdi_MUTUA &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereMutua(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReator(sdi_REATOR &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereReator(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereRede(sdi_REDE &sdi)
	{
   //verifica se � uma rede secund�ria
   et.enabled = (sdi.tiporede.seg == redeSEC);
   //verifica se est� montando uma rede secund�ria
	if (! et.enabled) return(TPreMonta::InsereRede(sdi));
	//reinicia estrutura de controle p/ inser��o de carga equivalente
	et.codigo    = sdi.cod; //DVK 2015.10.23
   et.barra_mt  = NULL;
	et.carga_eqv = NULL;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReguladorAutomatico(sdi_REGULADOR &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereReguladorAutomatico(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReguladorTapFixo(sdi_REGULADOR &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereReguladorTapFixo(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSuprimento(sdi_SUPRIMENTO &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereSuprimento(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafo2E(sdi_TRAFO2E &sdi)
   {
   //veri�veis locais
	int fases_tag, liga;

   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereTrafo2E(sdi));
   //salva ponteiro p/ Barra MT
   if ((et.barra_mt = monta->ExisteBarra(-sdi.pri.id_bar)) != NULL)
      {//cria Carga equivalente //DVK 2015.11.09 traduz liga��o
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
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereTrafo3E(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafoMono(sdi_TRAFO_MONO &sdi)
   {
   //veri�veis locais
   int fases_tag;

   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereTrafoMono(sdi));
   //salva ponteiro p/ Barra MT
   if ((et.barra_mt = monta->ExisteBarra(-sdi.pri.id_bar)) != NULL)
		{//cria Carga equivalente
		fases_tag = fases->ExisteFase(sdi.pri.fases);
		//verifica se a Carga equivalente j� foi criada (devido a um outro trafo monof�sico)
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
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereTrafoZZ(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrechoComArranjo(sdi_TRECHO &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereTrechoComArranjo(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrechoZ0Z1(sdi_TRECHO &sdi)
   {
   //verifica se est� montando uma rede secund�ria
   if (! et.enabled) return(TPreMonta::InsereTrechoZ0Z1(sdi));

   return(true);
   }

//-----------------------------------------------------------------------------
// eof



