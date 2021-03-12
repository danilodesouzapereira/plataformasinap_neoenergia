//---------------------------------------------------------------------------
#ifndef ReclamacaoH
#define ReclamacaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormReclamacao(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormReclamacaoBT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm* __fastcall DLL_NewFormReclamacaoMT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 