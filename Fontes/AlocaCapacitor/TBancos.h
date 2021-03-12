//---------------------------------------------------------------------------
#ifndef TBancosH
#define TBancosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBancos.h"

//---------------------------------------------------------------------------
class TBancos : public VTBancos
   {
   public:
               __fastcall  TBancos(VTApl *apl);
               __fastcall ~TBancos(void);
		VTBanco* __fastcall  ExisteBanco(VTBarra *barra);
      VTBanco* __fastcall  ExisteBanco(VTCapacitor *capacitor); //FKM 2015.09.24
		VTBanco* __fastcall  InsereBanco(VTBarra *barra);
		TList*   __fastcall  LisBanco(void);
		int      __fastcall  LisCapacitor(TList *lisEXT);
		int      __fastcall  LisCloneCapacitor(TList *lisEXT);
      int      __fastcall  NumBanco(void);
      double   __fastcall  Qtotal_mvar(void);
      void     __fastcall  Reinicia(void);
      bool     __fastcall  RetiraCapacitor(VTCapacitor *capacitor); //FKM 2015.09.24
   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList *lisBANCO;
   };

#endif
//---------------------------------------------------------------------------
//eof

