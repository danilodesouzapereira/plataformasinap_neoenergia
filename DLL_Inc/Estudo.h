//---------------------------------------------------------------------------
#ifndef EstudoH
#define EstudoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTAnaTec;
class VTBaseAtivo;

//-----------------------------------------------------------------------------
EXPORT void      __fastcall DLL_AtualizaFormEstudo(TComponent *Owner, bool salvando);
EXPORT void      __fastcall DLL_CloseFormEstudo(TComponent *Owner);
EXPORT TForm*    __fastcall DLL_NewFormEstudo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTAnaTec* __fastcall DLL_NewObjAnaTec(VTApl *apl_owner);
EXPORT VTBaseAtivo* __fastcall DLL_NewObjBaseAtivo(void);

//---------------------------------------------------------------------------
#endif
//eof


