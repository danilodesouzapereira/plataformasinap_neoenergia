//---------------------------------------------------------------------------
#ifndef TFluxoNR1H
#define TFluxoNR1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Fluxo\VTFluxo.h"

//---------------------------------------------------------------------------
class VTData;
class VTNR1;

//---------------------------------------------------------------------------
class TFluxoNR1 : public VTFluxo
   {
   public:
                 __fastcall TFluxoNR1(VTApl *apl);
                 __fastcall ~TFluxoNR1(void);
      void       __fastcall CfgAvisoErro(bool enabled);
      bool       __fastcall Executa(VTNet *net, double Sbase,
                                    AnsiString arqRelFlow, AnsiString arqRelRede);
      AnsiString __fastcall Modelo(void);
      bool       __fastcall UtilizaBarraPV(void);
      bool       __fastcall ValidaAderencia(VTNet *net, VTLog *plog);

   protected:  //métodos acessados via property
      bool       __fastcall  PM_GetEquilibrado(void);

   private: //objetos externos
      VTApl     *apl;
      VTNet     *net;

   private: //objetos locais
      bool   aviso_enabled;
      VTData *data;
      VTNR1  *nr;
   };

#endif
//---------------------------------------------------------------------------
//eof
