//---------------------------------------------------------------------------
#ifndef FiguraH
#define FiguraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTFiguraComentario;
class VTFiguras;
class VTLink;
class VTRetangulo;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
//EXPORT VTFiguraComentario* 	__fastcall DLL_NewObjFiguraComentario(VTApl *apl_owner);
EXPORT VTFiguras*    		__fastcall DLL_NewObjFiguras(VTApl *apl_owner);
//EXPORT VTLink*       		__fastcall DLL_NewObjLink(VTApl *apl_owner);
//EXPORT VTRetangulo*  		__fastcall DLL_NewObjRetangulo(VTApl *apl_owner);
EXPORT TForm* 				__fastcall DLL_NewFormFigurasIO(TComponent *Owner, VTApl *apl_owner);
//---------------------------------------------------------------------------
#endif
//eof

 