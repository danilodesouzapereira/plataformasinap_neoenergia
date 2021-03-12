//---------------------------------------------------------------------------
#ifndef VTTreeH
#define VTTreeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;

//---------------------------------------------------------------------------
class VTTree : public TObject
   {
   public:
                     __fastcall VTTree(void)  {};
      virtual        __fastcall ~VTTree(void) {};
      virtual void   __fastcall Add(VTBarra *barra) = 0;
      virtual void   __fastcall Add(VTBarra *barra, VTBarra *barra_pai, VTLigacao *ligacao) = 0;
      virtual void   __fastcall Filtra(VTBarra *barra) = 0;
      virtual TList* __fastcall LisBarra(void) = 0;
      virtual TList* __fastcall LisLigacao(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTTree* __fastcall NewObjTree(void);

#endif
//---------------------------------------------------------------------------
//eof

