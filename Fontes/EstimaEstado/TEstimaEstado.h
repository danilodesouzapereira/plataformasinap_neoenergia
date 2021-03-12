//---------------------------------------------------------------------------
#ifndef TEstimaEstadoH
#define TEstimaEstadoH

//---------------------------------------------------------------------------
#include <complex>
#include "VTEstimaEstado.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TEstimaEstado : public VTEstimaEstado
   {
   public:  //métodos
                 __fastcall  TEstimaEstado(VTApl *apl);
                 __fastcall ~TEstimaEstado(void);
      bool       __fastcall  Executa(void);

   private:  //métodos
      bool __fastcall ExecutaEstimaEstadoRedeUnica(void);
      void __fastcall HabilitaPatamarUnico(int ind_pat);
      bool __fastcall ValidaMedidores(void);

   private:  //ojetos externos
      VTApl  *apl;

   private: //dados locais
      TList *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
