//---------------------------------------------------------------------------
#ifndef GraficoH
#define GraficoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTGrafico;
class VTQuad;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void       __fastcall DLL_CloseFormPadrao(TForm *form_owner);
EXPORT void       __fastcall DLL_CloseFormZoom(TForm *form_owner);
EXPORT void       __fastcall DLL_NewFormPadrao(TForm *form_owner, VTApl *apl, TWinControl *parent);
EXPORT void       __fastcall DLL_NewFormZoom(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTGrafico* __fastcall DLL_NewObjGrafico(VTApl *apl);
EXPORT VTGrafico* __fastcall DLL_NewObjGraficoAbre(VTApl *apl);
EXPORT VTQuad*    __fastcall DLL_NewObjQuad(void);

//---------------------------------------------------------------------------
#endif
//eof


 