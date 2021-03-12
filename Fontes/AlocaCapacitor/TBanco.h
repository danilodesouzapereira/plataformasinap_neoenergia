//---------------------------------------------------------------------------
#ifndef TBancoH
#define TBancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBanco.h"

//---------------------------------------------------------------------------
class TBanco : public VTBanco
   {
   public:
               __fastcall  TBanco(VTApl *apl);
               __fastcall ~TBanco(void);
		bool     __fastcall  InsereCapacitor(VTCapacitor *capacitor);
		TList*   __fastcall  LisCapacitor(void);
      int      __fastcall  NumCapacitor(void);
		double   __fastcall  Q_mvar(int ind_pat, bool nominal=true);
		double   __fastcall  Qtotal_mvar(void);

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList *lisCAP;
   };

#endif
//---------------------------------------------------------------------------
//eof

