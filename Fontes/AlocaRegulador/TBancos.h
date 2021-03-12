//---------------------------------------------------------------------------
#ifndef TBancosH
#define TBancosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBancos.h"
//---------------------------------------------------------------------------
class VTRede;  //DVK 2014.04.03
//---------------------------------------------------------------------------
class TBancos : public VTBancos
   {
   public:
               __fastcall  TBancos(VTApl *apl);
               __fastcall ~TBancos(void);
		VTBanco* __fastcall  ExisteBanco(VTLigacao *ligacao);
		VTBanco* __fastcall  InsereBanco(void);
		TList*   __fastcall  LisBanco(void);
      TList*   __fastcall  LisBanco(VTRede* rede);    //DVK 2014.04.03
		int      __fastcall  LisRegulador(TList *lisEXT);
		int      __fastcall  LisCloneRegulador(TList *lisEXT);
      int      __fastcall  NumBanco(void);
      void     __fastcall  Reinicia(void);
      
   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList *lisBANCO;
      TList *lisBancoRede;    //DVK 2014.04.03
   };

#endif
//---------------------------------------------------------------------------
//eof

