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
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
#include "TPreMonta2.h"

//---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta2(VTApl *apl)
   {
   try{//cria objeto TPreMonta2
      return(new TPreMonta2(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta2::TPreMonta2(VTApl *apl)
                      :TPreMonta(apl)
   {
   //inicia controle indicando que não está importando uma rede secundária
   et.enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta2::~TPreMonta2(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta2::CriaBarraBT(double vnom_kv)
   {
   //variáveis locais
   strBARRA str_barra;
   VTBarra  *barra;

	try{//preenche estrutura strBARRA
		str_barra.bar_id = FalsoId();
		//str_barra.bar_id = - str_barra.bar_id; //DVK 2015.10.23 (evita que outras barras novas tenham mesmo ID)
		str_barra.codigo = "";
      str_barra.vnom   = vnom_kv;
		str_barra.utm.x  = et.barra_mt->utm.x;
      str_barra.utm.y  = et.barra_mt->utm.y;
      str_barra.esq.x1 = et.barra_mt->esq.x1;
//		str_barra.esq.y1 = et.barra_mt->esq.y1 - 1000;
		str_barra.esq.y1 = et.barra_mt->esq.y1 + 1000;
      str_barra.esq.x2 = et.barra_mt->esq.x1;
//		str_barra.esq.y2 = et.barra_mt->esq.y1 - 1000;
		str_barra.esq.y2 = et.barra_mt->esq.y1 + 1000;
      //insere Rede em Monta
      barra = (VTBarra*)monta->InsereBarraEqpto(str_barra);
      //força inclusão da Barra na rede
      monta->InsereBarraStop();
      }catch(Exception &e)
         {
         barra = NULL;
         }
   return(barra);
   }
//---------------------------------------------------------------------------
VTRede* __fastcall TPreMonta2::ExisteRedeDaBarra(VTBarra *barra)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisREDE = redes->LisRede();

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteBarra(barra)) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereBarra(sdi_BARRA &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereBarra(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereCapacitor(sdi_CAPACITOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCapacitor(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereCapacitorSerie(sdi_CAPSERIE &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCapacitorSerie(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta2::InsereCargaComMedicao(sdi_CARGA &sdi)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCargaComMedicao(sdi));
   //proteção
	if (et.barra_bt == NULL)
		{//DVK 2015.11.04 insere aviso no log
		if(plog) plog->DefineValor("Rede", et.codigo);
		if(plog) plog->Add("Não existe barra BT da rede secundária");
		return(NULL);
		}
   //cria Carga temporária associada à Barra mt da rede secundária
   sdi.id_barra = -et.barra_bt->Id;
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
VTCarga* __fastcall TPreMonta2::InsereCargaIP(sdi_CARGA_IP &sdi)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCargaIP(sdi));
   //proteção
	if (et.barra_bt == NULL)
		{//DVK 2015.11.04 insere aviso no log
		if(plog) plog->DefineValor("Rede", et.codigo);
		if(plog) plog->Add("Não existe barra BT da rede secundária");
		return(NULL);
		}
   //cria Carga temporária associada à Barra mt da rede secundária
   sdi.id_barra = -et.barra_bt->Id;
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
VTCarga* __fastcall TPreMonta2::InsereCargaTipica(sdi_CARGA &sdi)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereCargaTipica(sdi));
	//proteção
//	if (et.barra_bt == NULL) return(NULL);
	if (et.barra_bt == NULL)
		{//DVK 2015.10.22 insere aviso no log
		if(plog) plog->DefineValor("Rede", et.codigo);
		if(plog) plog->Add("Não existe barra BT da rede secundária");
		return(NULL);
		}
	//verifica se a carga é do grupo A
	if(sdi.grupo_tarifa.AnsiCompareIC("A") == 0)
	{//não atualiza
		return (et.carga_eqv);
    }
	// cria Carga temporária associada à Barra mt da rede secundária
	sdi.id_barra = -et.barra_bt->Id;
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
bool __fastcall TPreMonta2::InsereChave(sdi_CHAVE &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereChave(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereFiltro(sdi_FILTRO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereFiltro(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereGerador(sdi_GERADOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereGerador(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereMutua(sdi_MUTUA &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereMutua(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereReator(sdi_REATOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereReator(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereRede(sdi_REDE &sdi)
   {
   //verifica se é uma rede secundária
	et.enabled = (sdi.tiporede.seg == redeSEC);
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereRede(sdi));
	//reinicia estrutura de controle p/ inserção de carga equivalente
	et.codigo  = sdi.cod; //DVK 2015.10.23
   et.barra_mt  = NULL;
   et.barra_bt  = NULL;
   et.carga_eqv = NULL;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereReguladorAutomatico(sdi_REGULADOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereReguladorAutomatico(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereReguladorTapFixo(sdi_REGULADOR &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereReguladorTapFixo(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereSuprimento(sdi_SUPRIMENTO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereSuprimento(sdi));

   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrafo2E(sdi_TRAFO2E &sdi)
	{
	//veriáveis locais
	int     fases_tag;

	//verifica se está montando uma rede secundária
	if (! et.enabled) return(TPreMonta::InsereTrafo2E(sdi));
	//salva ponteiro p/ Barra MT
	if ((et.barra_mt = monta->ExisteBarra(sdi.pri.id_bar)) == NULL) return(false);
	//determina Rede da Barra MT
	if ((rede = ExisteRedeDaBarra(et.barra_mt)) == NULL) return(false);
	//cria Barra BT com mesmas coordenadas
	if ((et.barra_bt = CriaBarraBT(sdi.sec.vnom_kv)) != NULL)
		{//insere Trafo
		sdi.sec.id_bar = et.barra_bt->Id;
		if (TPreMonta::InsereTrafo2E(sdi))
			{//cria Carga equivalente
         fases_tag = (sdi.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
			et.carga_eqv = CriaCargaEquivalente(et.barra_bt->Id, sdi.cod, fases_tag);
			}
		}
	//IMPORTANTE: cancela ponteiro para Rede
	rede = NULL;
	return(true);
	}
*/   //DVK 2015.10.23 reescrito: a barra_mt pode ser nula e isso não deve
	  //interromper a importação, mas apenas não importar a ET
//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrafo2E(sdi_TRAFO2E &sdi)
	{
	//veriáveis locais
	int     fases_tag;

	//verifica se está montando uma rede secundária
	if (! et.enabled) return(TPreMonta::InsereTrafo2E(sdi));
	//salva ponteiro p/ Barra MT (se houver)
	if ((et.barra_mt = monta->ExisteBarra(-sdi.pri.id_bar)) == NULL)
		{//insere aviso no log
		if(plog) plog->DefineValor("Rede", et.codigo);
		if(plog) plog->Add("Não existe barra MT da rede secundária");
		rede = NULL;
		return(true);
      }
	//determina Rede da Barra MT
	rede = ExisteRedeDaBarra(et.barra_mt);
	//cria Barra BT com mesmas coordenadas
	if ((et.barra_bt = CriaBarraBT(sdi.sec.vnom_kv)) != NULL)
		{//insere Trafo
		sdi.sec.id_bar = -et.barra_bt->Id;
		if (TPreMonta::InsereTrafo2E(sdi))
			{//cria Carga equivalente
			fases_tag = (sdi.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
			et.carga_eqv = CriaCargaEquivalente(et.barra_bt->Id, et.codigo, fases_tag);
			}
		}
	//IMPORTANTE: cancela ponteiro para Rede
	rede = NULL;
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrafo3E(sdi_TRAFO3E &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafo3E(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrafoMono(sdi_TRAFO_MONO &sdi)
   {
   //veriáveis locais
   int    fases_tag;

   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafoMono(sdi));
   //salva ponteiro p/ Barra MT
   if ((et.barra_mt = monta->ExisteBarra(-sdi.pri.id_bar)) == NULL) return(false);
   //determina Rede da Barra MT
   if ((rede = ExisteRedeDaBarra(et.barra_mt)) == NULL) return(false);
   //cria Barra BT com mesmas coordenadas
   if ((et.barra_bt = CriaBarraBT(sdi.sec.vnom_kv)) != NULL)
      {//insere Trafo
	  sdi.sec.id_bar = -et.barra_bt->Id;
      if (TPreMonta::InsereTrafoMono(sdi))
         {//cria Carga equivalente
         fases_tag = fases->ExisteFase(sdi.sec.fases);
         //verifica se a Carga equivalente já foi criada (devido a um outro trafo monofásico)
         if (et.carga_eqv == NULL)
            {
//            et.carga_eqv = CriaCargaEquivalente(et.barra_bt->Id, sdi.cod, fases_tag);
            et.carga_eqv = CriaCargaEquivalente(et.barra_bt->Id, et.codigo, fases_tag);
				}
         else
            {//atualiza fases da Carga equivalente
            et.carga_eqv->Fases = et.carga_eqv->Fases | fases_tag;
            }
         }
      }
   //IMPORTANTE: cancela ponteiro para Rede
   rede = NULL;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrafoZZ(sdi_TRAFOZZ &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrafoZZ(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrechoComArranjo(sdi_TRECHO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrechoComArranjo(sdi));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPreMonta2::InsereTrechoZ0Z1(sdi_TRECHO &sdi)
   {
   //verifica se está montando uma rede secundária
   if (! et.enabled) return(TPreMonta::InsereTrechoZ0Z1(sdi));

   return(true);
   }

//-----------------------------------------------------------------------------
// eof



