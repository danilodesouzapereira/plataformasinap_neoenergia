//---------------------------------------------------------------------------
#ifndef DiagnosticoH
#define DiagnosticoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTDiagFlow;
class VTDiagTopo;
class VTProgresso;

//-----------------------------------------------------------------------------
// funções da DLL
//---------------------------------------------------------------------------
EXPORT bool         __fastcall DLL_DiagEqptoEnabled(void);
EXPORT bool         __fastcall DLL_DiagFlowEnabled(void);
EXPORT void         __fastcall DLL_CloseFormDiagEqpto(TForm *form_owner);
EXPORT void         __fastcall DLL_CloseFormDiagFlow(TForm *form_owner);
EXPORT TForm*       __fastcall DLL_NewFormDiagEqpto(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*       __fastcall DLL_NewFormDiagFlow(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTDiagTopo*  __fastcall DLL_NewObjDiagTopo(VTApl *apl, VTProgresso *progresso);
EXPORT VTDiagFlow*  __fastcall DLL_NewObjDiagFlow(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

