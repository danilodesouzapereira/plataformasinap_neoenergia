//---------------------------------------------------------------------------
#ifndef VTEsqRadialH
#define VTEsqRadialH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;
class VTSecao;

//---------------------------------------------------------------------------
class VTEsqRadial : public TObject
   {
   public:
                       __fastcall VTEsqRadial(void) {};
      virtual          __fastcall ~VTEsqRadial(void) {};
      virtual bool     __fastcall Executa(VTRede *rede, bool ordena=true) = 0;
      virtual VTSecao* __fastcall ExisteSecao(VTBarra *barra) = 0;
      virtual TList*   __fastcall LisBarra(void) = 0;
      virtual TList*   __fastcall LisLigacao(void) = 0;
      virtual TList*   __fastcall LisSecao(void) = 0;
      virtual VTSecao* __fastcall SecaoFinal(void) = 0;
      virtual VTSecao* __fastcall SecaoInicial(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsqRadial
//---------------------------------------------------------------------------
VTEsqRadial* __fastcall NewObjEsqRadial(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 