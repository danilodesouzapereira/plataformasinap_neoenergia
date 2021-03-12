//---------------------------------------------------------------------------
#ifndef MercadoH
#define MercadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCrescimento;
class VTImportaMercado;
class VTMercado;
class VTSegmento;

//-----------------------------------------------------------------------------
// funções da DLL
//---------------------------------------------------------------------------
EXPORT void           __fastcall DLL_CloseFormMercado(TComponent *Owner);
EXPORT TForm*         __fastcall DLL_NewFormMercado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTCrescimento* __fastcall DLL_NewObjCrescimento(void);
EXPORT VTImportaMercado*     __fastcall DLL_NewObjImportaMercado(VTApl *apl);
EXPORT VTMercado*     __fastcall DLL_NewObjMercado(VTApl *apl);
EXPORT VTSegmento*    __fastcall DLL_NewObjSegmento(void);

//---------------------------------------------------------------------------
#endif
//eof


 