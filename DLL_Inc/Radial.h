//---------------------------------------------------------------------------
#ifndef RadialH
#define RadialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTMalhas;
class VTNode;
class VTRadial;
class VTTronco;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void           __fastcall DLL_CloseFormMalha(TComponent *Owner);
EXPORT TForm*         __fastcall DLL_NewFormMalha(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTArvore*      __fastcall DLL_NewObjArvore(VTApl *apl);
EXPORT VTMalhas*      __fastcall DLL_NewObjMalhas(VTApl *apl);
EXPORT VTNode*        __fastcall DLL_NewObjNode(void);
EXPORT VTRadial*      __fastcall DLL_NewObjRadial(VTApl *apl);
EXPORT VTTronco*      __fastcall DLL_NewObjTronco(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 