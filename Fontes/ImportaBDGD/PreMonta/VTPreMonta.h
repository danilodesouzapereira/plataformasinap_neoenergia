//-----------------------------------------------------------------------------
#ifndef VTPreMonta_H
#define VTPreMonta_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Rede\VTEqpto.h>
#include "..\Commons\VTCommons.h"
#include "..\Commons\Dominio\TClasseTensao.h"
#include "..\Commons\Dominio\TTensao.h"
#include "..\Commons\Entidades\TArranjoBDGD.h"
#include "..\Commons\Entidades\TBarraBDGD.h"
#include "..\Commons\Entidades\TBarramento.h"
#include "..\Commons\Entidades\TRedeMT.h"
#include "..\Commons\Entidades\TSubestacao.h"
#include "..\Commons\Entidades\TTrafoSubestacao.h"
//
#include "..\Entidades\TCapacitorBDGD.h"
#include "..\Entidades\TCargaBDGD.h"
#include "..\Entidades\TCargaIP.h"
#include "..\Entidades\TChaveBDGD.h"
#include "..\Entidades\TGeradorBDGD.h"
#include "..\Entidades\TRamalLigacao.h"
#include "..\Entidades\TRedeBT.h"
#include "..\Entidades\TReguladorMT.h"
#include "..\Entidades\TTrafoDistribuicao.h"
#include "..\Entidades\TTrechoBDGD.h"


#define sttTRECHOCURTO 0x00200000

//-----------------------------------------------------------------------------
class VTApl;


//-----------------------------------------------------------------------------
class VTPreMonta : public TObject
   {
   public:  //property
      __property TDateTime  DataDominio         = {read=PD.date          , write=PD.date          };
      __property int        ConsA4              = {read=PD.consA4        , write=PD.consA4};
      __property AnsiString ArranjoRedePRI      = {read=PD.arranjoPRI    , write=PD.arranjoPRI    };
      __property AnsiString ArranjoRedeSEC      = {read=PD.arranjoSEC    , write=PD.arranjoSEC    };

   public:
						 __fastcall  VTPreMonta(void) {};
		virtual      __fastcall ~VTPreMonta(void) {};

		//controle de barras
		virtual void __fastcall InsereBarraStart(void) = 0;
		virtual void __fastcall InsereBarraStop(void) = 0;
		//
		virtual bool __fastcall  InsereCfg_Arranjo(TArranjoBDGD *arranjoBDGD)=0;
		virtual bool __fastcall InsereCfg_TipoChave(TTipoUnidade* tipo_ch)= 0;
		//etd
		virtual bool __fastcall  InsereEtd_Barra(TBarramento *barramento)=0;
		virtual bool __fastcall  InsereEtd_Rede(TSubestacao *subestacao)=0;
		virtual bool __fastcall  InsereEtd_Trafo(TTrafoSubestacao *trafo_sub)=0;
		//pri
		virtual bool __fastcall  InserePri_Barra(TBarraBDGD *barraBDGD)=0;
		virtual bool __fastcall  InserePri_Capacitor(TCapacitorBDGD *cap)=0;
		virtual bool __fastcall  InserePri_Chave(TChaveBDGD *chaveBDGD)=0;
		virtual bool __fastcall  InserePri_Consumidor(TCargaBDGD *cargaMT)=0;
		virtual bool __fastcall  InserePri_Gerador(TGeradorBDGD *geradorMT)=0;
		virtual bool __fastcall  InserePri_Rede(TRedeMT *primario)=0;
		virtual bool __fastcall  InserePri_Regulador(TReguladorMT *reg)=0;
	   //	virtual bool __fastcall  InserePri_Trafo(sdi_TRAFO_LINHA &sdi)=0;
		virtual bool __fastcall  InserePri_Trecho(TTrechoBDGD *trechoBDGD)=0;
		//sec
		virtual bool __fastcall  InsereSec_Barra(TBarraBDGD *barraBT)=0;
		virtual bool __fastcall  InsereSec_Capacitor(TCapacitorBDGD *capBT)=0;
		virtual bool __fastcall  InsereSec_Chave(TChaveBDGD *chaveBDGD) = 0;
		virtual bool __fastcall  InsereSec_Consumidor(TCargaBDGD *cargaBT)=0;
		virtual bool __fastcall  InsereSec_ConsIP(TCargaIP *cargaIP)=0;
		virtual bool __fastcall  InsereSec_Gerador(TGeradorBDGD *geradorBT)=0;
		virtual bool __fastcall  InsereSec_Ramal(TRamalLigacao *ramal) = 0;
		virtual bool __fastcall  InsereSec_Rede(TRedeBT *redeBT)=0;
		virtual bool __fastcall  InsereSec_Trecho(TTrechoBDGD *trechoBT)=0;
		virtual bool __fastcall  InsereSec_TrafoMTBT(TTrafoDistribuicao *trafoMTBT)=0;
		virtual bool __fastcall  Sucesso(void)=0;
		virtual bool __fastcall  TrataInicioDeDados(void)=0;
		virtual bool __fastcall  TrataFimDeDados(void)=0;

   protected:
      struct   {
               TDateTime  date;
               int        consA4;
               AnsiString arranjoPRI;
               AnsiString arranjoSEC;
               }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta1(VTApl *apl);
VTPreMonta* NewObjPreMonta2(VTApl *apl);
VTPreMonta* NewObjPreMonta3(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
