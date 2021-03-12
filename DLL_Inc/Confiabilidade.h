//---------------------------------------------------------------------------
#ifndef ConfiabilidadeH
#define ConfiabilidadeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTConfiab;
class TFormParam;

//-----------------------------------------------------------------------------
// fun��es da DLL
//-----------------------------------------------------------------------------
EXPORT bool        __fastcall DLL_Confiabilidade_Enabled(void);
EXPORT int         __fastcall DLL_Confiabilidade_LimiteBarras(void);
EXPORT void        __fastcall DLL_AtualizaFormConfiabilidade(TForm *form_owner);
EXPORT void        __fastcall DLL_CloseFormConfiabilidade(TForm *form_owner);
EXPORT TForm*      __fastcall DLL_NewFormConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TFormParam* __fastcall DLL_NewFormParamConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTConfiab*  __fastcall DLL_NewObjConfiab(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 