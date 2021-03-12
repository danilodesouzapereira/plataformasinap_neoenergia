//---------------------------------------------------------------------------
#ifndef TCfgFluxoH3H
#define TCfgFluxoH3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTCfgFluxoH3.h"

//---------------------------------------------------------------------------
class TCfgFluxoH3 : public VTCfgFluxoH3
   {
   public:
                     __fastcall  TCfgFluxoH3(VTApl *apl);
                     __fastcall ~TCfgFluxoH3(void);
      bool           __fastcall  InsereOrdemHarmonica(int ordem);
      int            __fastcall  OrdemHarmonica(int indice);
      void           __fastcall  Reinicia(void);

   protected:  //métodos acessados via property
      bool __fastcall PM_GetGravaYnodal(void);
      int  __fastcall PM_GetNumeroHarmonicas(void);

      void __fastcall PM_SetGravaYnodal(bool grava_ynodal);

   private: //objetos externos
      VTApl *apl;

   private: //objetos locais
      struct
         {
         bool grava_ynodal;
         int  NumMaxHarmonicas;  // Incrementado automaticamente se necessário
         int  num_harmonicas;
         int  *vet_ordens_harmonicas;
         } dados;
   };

#endif
//---------------------------------------------------------------------------
//eof
