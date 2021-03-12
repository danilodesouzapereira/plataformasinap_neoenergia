//---------------------------------------------------------------------------
#ifndef VTMalhasH
#define VTMalhasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class VTMalhas : public TObject
   {
   public:
                       __fastcall VTMalhas(void) {};
      virtual          __fastcall ~VTMalhas(void) {};
      virtual bool     __fastcall Executa(VTRede *rede) = 0;
      virtual TList*   __fastcall LisBarraMalha(void) = 0;
      virtual TList*   __fastcall LisLigacaoMalha(void) = 0;
      virtual TList*   __fastcall LisMalha(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMalhas
//---------------------------------------------------------------------------
VTMalhas* __fastcall NewObjMalhas(void);

//---------------------------------------------------------------------------
#endif
//eof



 