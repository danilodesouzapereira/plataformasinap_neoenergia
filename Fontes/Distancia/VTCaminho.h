//---------------------------------------------------------------------------
#ifndef VTCaminhoH
#define VTCaminhoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class VTCaminho : public TObject
   {
   public:
                   __fastcall  VTCaminho(void) {};
      virtual      __fastcall ~VTCaminho(void) {};
      virtual void __fastcall  Executa(VTEqpto *eqpto1, VTEqpto *eqpto2, TList *lisLIG_CAM) = 0;
      virtual void __fastcall  Executa(TList *lisEQP_INI, TList *lisEQP_FIM, TList *lisLIG_ALL, TList *lisLIG_CAM) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCaminho
//---------------------------------------------------------------------------
VTCaminho* __fastcall NewObjCaminho(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 