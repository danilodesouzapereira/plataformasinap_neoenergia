//---------------------------------------------------------------------------
#ifndef SinapAplH
#define SinapAplH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjApl(TForm *FormOwner, VTApl *AplOwner);
EXPORT VTApl* __fastcall DLL_NewObjAplBatch(TForm *FormOwner, VTApl *AplOwner);
EXPORT VTApl* __fastcall DLL_NewObjAplChild(TForm *FormOwner, VTApl *AplOwner, TAction *ActionInsRede);
EXPORT VTApl* __fastcall DLL_NewObjAplMdi(TForm *FormOwner, VTApl *AplOwner);
EXPORT VTApl* __fastcall DLL_NewObjAplRnt(TForm *FormOwner, VTApl *AplOwner);

//---------------------------------------------------------------------------
#endif
//eof


 