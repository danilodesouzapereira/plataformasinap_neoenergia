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
public: //métodos públicos
                  __fastcall VTDiagFlow(void) {};
   virtual        __fastcall ~VTDiagFlow(void) {};
   virtual TList* __fastcall Executa(void)= 0;
};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe DiagFlow
//---------------------------------------------------------------------------
VTDiagFlow* __fastcall NewObjDiagFlow(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
