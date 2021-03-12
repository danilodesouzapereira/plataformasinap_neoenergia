#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DLL_Inc\Figura.h>
#include <Fontes\Figura\IO\TFormFigurasIO.h>
#include <Fontes\Figura\VTFiguraComentario.h>
#include <Fontes\Figura\VTFiguras.h>
#include <Fontes\Figura\VTFigura.h>
#include <Fontes\Figura\VTLink.h>
#include <Fontes\Figura\VTRetangulo.h>
#pragma argsused

#pragma comment(lib, "BD.a")
#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
//EXPORT VTFiguraComentario* __fastcall DLL_NewObjFiguraComentario(VTApl *apl_owner)
//   {
//   return(NewObjFiguraComentario(apl_owner));
//   }

//-----------------------------------------------------------------------------
EXPORT VTFiguras* __fastcall DLL_NewObjFiguras(VTApl *apl_owner)
   {
   return(NewObjFiguras(apl_owner));
   }
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
////-----------------------------------------------------------------------------
//EXPORT VTLink* __fastcall DLL_NewObjLink(VTApl *apl_owner)
//   {
//   return(NewObjLink(apl_owner));
//   }
//
////-----------------------------------------------------------------------------
//EXPORT VTRetangulo* __fastcall DLL_NewObjRetangulo(VTApl *apl_owner)
//   {
//   return(NewObjRetangulo(apl_owner));
//   }
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormFigurasIO(TComponent *Owner, VTApl *apl_owner)
   {
   return(new TFormFigurasIO(Owner, apl_owner));
   }
//---------------------------------------------------------------------------
//eof

