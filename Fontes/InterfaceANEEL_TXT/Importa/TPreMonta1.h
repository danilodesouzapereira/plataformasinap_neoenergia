//---------------------------------------------------------------------------
#ifndef TPreMonta1H
#define TPreMonta1H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "TPreMonta.h"

//---------------------------------------------------------------------------
class TPreMonta1 : public TPreMonta
   {
   public:
               __fastcall  TPreMonta1(VTApl *apl);
               __fastcall ~TPreMonta1(void);
		bool __fastcall  InsereCondutor(sdi_CONDUTOR &sdi);
		bool __fastcall  InsereCoordenada(sdi_COORDENADA &sdi);
		bool __fastcall  InsereCargaBT(sdi_CARGA_BT &sdi);
		bool __fastcall  InsereCargaMT(sdi_CARGA_MT &sdi);
		bool __fastcall  InsereChaveBT(sdi_CHAVE_BT &sdi);
		bool __fastcall  InsereChaveMT(sdi_CHAVE_MT &sdi);
		bool __fastcall  InsereCircuito(sdi_CIRCUITO &sdi);
		bool __fastcall  InsereGeradorBT(sdi_GERADOR_BT &sdi);
		bool __fastcall  InsereGeradorMT(sdi_GERADOR_MT &sdi);
		bool __fastcall  InsereRamalBT(sdi_RAMAL_BT &sdi);
		bool __fastcall  InsereReguladorMT(sdi_REGULADOR &sdi);
		bool __fastcall  InsereSegmentoBT(sdi_SEGMENTO_BT &sdi);
		bool __fastcall  InsereSegmentoMT(sdi_SEGMENTO_MT &sdi);
		bool __fastcall  InsereTrafoAT_MT(sdi_TRAFO_AT_MT &sdi);
		bool __fastcall  InsereTrafoMT_BT(sdi_TRAFO_MT_BT &sdi);

   private: //métodos

   private: //dados locais
      struct{
            bool    enabled ;
            VTBarra *barra_mt;
            VTCarga *carga_eqv;
            }et;
   };

#endif
//-----------------------------------------------------------------------------
// eof



