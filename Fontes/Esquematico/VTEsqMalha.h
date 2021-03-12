//---------------------------------------------------------------------------
#ifndef VTEsqMalhaH
#define VTEsqMalhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;
class VTSecao;

//---------------------------------------------------------------------------
class VTEsqMalha : public TObject
   {
   public:
                       __fastcall VTEsqMalha(void) {};
      virtual          __fastcall ~VTEsqMalha(void) {};
      virtual bool     __fastcall Executa(VTRede *rede) = 0;
      virtual VTSecao* __fastcall ExisteSecao(VTBarra *barra) = 0;
      virtual TList*   __fastcall LisBarra(void) = 0;
      virtual TList*   __fastcall LisLigacao(void) = 0;
      virtual TList*   __fastcall LisSecao(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsqMalha
//---------------------------------------------------------------------------
VTEsqMalha* __fastcall NewObjEsqMalha(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 