//---------------------------------------------------------------------------
#ifndef ImportaXML_H
#define ImportaXML_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExportaXML;
class VTImportaXML;


//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT bool          __fastcall DLL_ExportaXML_Enabled(void);
EXPORT bool          __fastcall DLL_ImportaXML_Enabled(void);
EXPORT TForm*        __fastcall DLL_NewFormExportaXML(TComponent *Owner, VTApl *apl_owner, AnsiString arq_xml="");
EXPORT TForm*        __fastcall DLL_NewFormImportaXML(TComponent *Owner, VTApl *apl_owner, AnsiString arq_xml="");
EXPORT VTExportaXML* __fastcall DLL_NewObjExportaXML(VTApl *apl_owner);
EXPORT VTImportaXML* __fastcall DLL_NewObjImportaXML(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 