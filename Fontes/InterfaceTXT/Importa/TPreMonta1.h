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
      bool     __fastcall  InsereBarra(sdi_BARRA &sdi);
      bool     __fastcall  InsereCapacitor(sdi_CAPACITOR &sdi);
      bool     __fastcall  InsereCapacitorSerie(sdi_CAPSERIE &sdi);
      VTCarga* __fastcall  InsereCargaComMedicao(sdi_CARGA &sdi);
      VTCarga* __fastcall  InsereCargaIP(sdi_CARGA_IP &sdi);
      VTCarga* __fastcall  InsereCargaTipica(sdi_CARGA &sdi);
      bool     __fastcall  InsereChave(sdi_CHAVE &sdi);
      bool     __fastcall  InsereFiltro(sdi_FILTRO &sdi);
      bool     __fastcall  InsereGerador(sdi_GERADOR &sdi);
      bool     __fastcall  InsereMutua(sdi_MUTUA &sdi);
      bool     __fastcall  InsereReator(sdi_REATOR &sdi);
      bool     __fastcall  InsereRede(sdi_REDE &sdi);
      bool     __fastcall  InsereReguladorAutomatico(sdi_REGULADOR &sdi);
      bool     __fastcall  InsereReguladorTapFixo(sdi_REGULADOR &sdi);
      bool     __fastcall  InsereSuprimento(sdi_SUPRIMENTO &sdi);
      bool     __fastcall  InsereTrafo2E(sdi_TRAFO2E &sdi);
      bool     __fastcall  InsereTrafo3E(sdi_TRAFO3E &sdi);
      bool     __fastcall  InsereTrafoMono(sdi_TRAFO_MONO &sdi);
      bool     __fastcall  InsereTrafoZZ(sdi_TRAFOZZ &sdi);
      bool     __fastcall  InsereTrechoComArranjo(sdi_TRECHO &sdi);
      bool     __fastcall  InsereTrechoZ0Z1(sdi_TRECHO &sdi);

   private: //métodos

   private: //dados locais
		struct{
				AnsiString 	codigo;     //DVK 2015.10.23
				bool    		enabled;
				VTBarra 		*barra_mt;
				VTCarga 		*carga_eqv;
            }et;
   };

#endif
//-----------------------------------------------------------------------------
// eof



