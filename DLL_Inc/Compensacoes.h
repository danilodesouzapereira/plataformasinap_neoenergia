//---------------------------------------------------------------------------
#ifndef CompensacoesH
#define CompensacoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//#define EXPORT extern "C" __declspec(dllexport)
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCompensacoes;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void            __fastcall DLL_CloseFormCompensacoes(TComponent *Owner);
EXPORT TForm*          __fastcall DLL_NewFormCompensacoes(TComponent *Owner, VTApl *apl_owner, TWinControl *parent, AnsiString empresa = "");
EXPORT VTCompensacoes* __fastcall DLL_NewObjCompensacoes(TComponent *Owner, VTApl *apl_owner, AnsiString empresa = "");

//---------------------------------------------------------------------------
#endif
//eof
