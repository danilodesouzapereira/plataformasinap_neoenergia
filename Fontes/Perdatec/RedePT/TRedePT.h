//---------------------------------------------------------------------------
#ifndef TRedePTH
#define TRedePTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRedePT.h"

//---------------------------------------------------------------------------
//class ;

//---------------------------------------------------------------------------
class TRedePT : public VTRedePT
   {
   public:
               __fastcall  TRedePT(void);
               __fastcall ~TRedePT(void);
      void     __fastcall  InsereBarra(VTBarra *barra);
      void     __fastcall  InsereCargaMedidor(VTCarga *carga);
      void     __fastcall  InsereLigacaoRamal(VTLigacao *ligacao);
      TList*   __fastcall  LisBarra(void);
      TList*   __fastcall  LisCargaMedidor(void);
      TList*   __fastcall  LisLigacaoRamal(void);

   private: //métodos

   private: //dados locais
      TList *lisBAR;
      TList *lisCAR_MED;
      TList *lisLIG_RL;
   };

#endif
//---------------------------------------------------------------------------
//eof

