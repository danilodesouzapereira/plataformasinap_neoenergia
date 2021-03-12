//---------------------------------------------------------------------------
#ifndef VTDiagFlowH
#define VTDiagFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTDiagFlow : public TObject
{
public: //m�todos p�blicos
                  __fastcall VTDiagFlow(void) {};
   virtual        __fastcall ~VTDiagFlow(void) {};
   virtual TList* __fastcall Executa(void)= 0;
};

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe DiagFlow
//---------------------------------------------------------------------------
VTDiagFlow* __fastcall NewObjDiagFlow(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
