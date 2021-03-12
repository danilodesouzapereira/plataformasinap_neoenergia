//---------------------------------------------------------------------------
#ifndef TParedeH
#define TParedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTParede.h"

//-----------------------------------------------------------------------------
class TParede : public VTParede
   {
   public:
             __fastcall  TParede(void);
             __fastcall ~TParede(void);
      bool   __fastcall  ExisteBarra(VTBarra *barra);
      bool   __fastcall  ExisteLigacao(VTLigacao *ligacao);
      bool   __fastcall  InsereBarra(VTBarra *barra);
      bool   __fastcall  InsereLigacao(VTLigacao *ligacao);
      TList* __fastcall  LisBarra(void);
      int    __fastcall  LisBarra(TList *lisEXT);
      TList* __fastcall  LisLigacao(void);
      int    __fastcall  LisEqbar(TList *lisEXT,   int tipo=-1);
      int    __fastcall  LisEqpto(TList *lisEXT,   int tipo=-1);
      int    __fastcall  LisLigacao(TList *lisEXT, int tipo=-1);
      void   __fastcall  ReiniciaLisEqpto(void);

   private: //métodos

   private: //objetos externos

   private: //dados locais
      TList *lisBARRA;
      TList *lisLIGACAO;
   };

//---------------------------------------------------------------------------
#endif
//eof
