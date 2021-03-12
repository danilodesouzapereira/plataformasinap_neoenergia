//---------------------------------------------------------------------------
#ifndef VTMontaCCH
#define VTMontaCCH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;
class VTRedeCC;

//---------------------------------------------------------------------------
class VTMontaCC : public TObject
   {
   public:
                     __fastcall VTMontaCC(void) {};
                     __fastcall ~VTMontaCC(void) {};
      virtual VTNet* __fastcall Executa(VTRedeCC *redeCC) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaCC
//---------------------------------------------------------------------------
VTMontaCC* __fastcall NewObjMontaCC(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 