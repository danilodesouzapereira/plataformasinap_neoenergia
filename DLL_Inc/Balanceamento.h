//---------------------------------------------------------------------------
#ifndef BalanceamentoH
#define BalanceamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void    __fastcall DLL_AtualizaFormBalanceamento(TComponent *owner);
EXPORT void    __fastcall DLL_CloseFormBalanceamento(TComponent *owner);
EXPORT TForm*  __fastcall DLL_ExisteFormBalanceamento(TComponent *owner);
EXPORT TForm*  __fastcall DLL_NewFormBalanceamento(TComponent *owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 