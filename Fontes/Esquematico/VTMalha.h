//-----------------------------------------------------------------------------
#ifndef VTMalha_H
#define VTMalha_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;

//---------------------------------------------------------------------------
class VTMalha : public TObject
   {
   public:
                     __fastcall VTMalha(void) {};
      virtual        __fastcall ~VTMalha(void) {};
      virtual bool   __fastcall ExisteBarra(VTBarra *barra) = 0;
      virtual bool   __fastcall ExisteBarra(VTLigacao *ligacao) = 0;
      virtual bool   __fastcall ExisteLigacao(TList *lisLIG_EXT) = 0;
      virtual void   __fastcall InsereLigacao(VTLigacao *ligacao) = 0;
      virtual void   __fastcall InsereLigacao(TList *lisLIG_EXT) = 0;
      virtual TList* __fastcall LisBarra(void) = 0;
      virtual void   __fastcall LisBarra(TList *lisEXT) = 0;
      virtual TList* __fastcall LisLigacao(void) = 0;
      virtual void   __fastcall LisLigacao(TList *lisEXT) = 0;
      virtual bool   __fastcall Monta(VTBarra *barra, VTLigacao *ligacao, TList *lisLM) = 0;
      virtual int    __fastcall NumeroLigacao(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMalha
//---------------------------------------------------------------------------
VTMalha* __fastcall NewObjMalha(void);

#endif
//-----------------------------------------------------------------------------
// eof
