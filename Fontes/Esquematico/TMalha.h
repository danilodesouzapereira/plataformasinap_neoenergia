//---------------------------------------------------------------------------
#ifndef TMalha_H
#define TMalha_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMalha.h"

//---------------------------------------------------------------------------
class TMalha : public VTMalha
   {
   public:
             __fastcall TMalha(void);
             __fastcall ~TMalha(void);
      bool   __fastcall ExisteBarra(VTBarra *barra);
      bool   __fastcall ExisteBarra(VTLigacao *ligacao);
      bool   __fastcall ExisteLigacao(TList *lisLIG_EXT);
      void   __fastcall InsereLigacao(VTLigacao *ligacao);
      void   __fastcall InsereLigacao(TList *lisLIG_EXT);
      TList* __fastcall LisBarra(void);
      void   __fastcall LisBarra(TList *lisEXT);
      TList* __fastcall LisLigacao(void);
      void   __fastcall LisLigacao(TList *lisEXT);
      bool   __fastcall Monta(VTBarra *barra, VTLigacao *ligacao, TList *lisLM);
      int    __fastcall NumeroLigacao(void);
   private: //métodos

   private: //dados
      TList *lisBAR;
      TList *lisLIG;
      struct   {
               TList *lisBAR;
               TList *lisLIG;
               TList *lisLM;
               }tmp;
   };

#endif
//-----------------------------------------------------------------------------
// eof
