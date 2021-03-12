//---------------------------------------------------------------------------
#ifndef MapaH
#define MapaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTMapa;
class VTMapas;
class VTPoligono;
class VTRegiao;
class VTRegioes;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*      __fastcall DLL_NewFormRegioes(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
//EXPORT TForm*     __fastcall DLL_NewFormEdtRegioes(TComponent* Owner, VTApl *apl, TList* lisRegiao, TWinControl *parent);
EXPORT TForm*      __fastcall DLL_NewFormEdtRegioes(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *OnDone, TList* lisRegiao);
EXPORT VTMapa*     __fastcall DLL_NewObjMapa(VTApl *apl);
EXPORT VTMapas*    __fastcall DLL_NewObjMapas(VTApl *apl);
EXPORT VTPoligono* __fastcall DLL_NewObjPoligono(void);
EXPORT VTRegiao*   __fastcall DLL_NewObjRegiao(VTApl *apl);
EXPORT VTRegioes*  __fastcall DLL_NewObjRegioes(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


