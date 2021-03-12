//---------------------------------------------------------------------------
#pragma hdrstop

#include "EImportaUgrid.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall EImportaUgrid::EImportaUgrid(AnsiString msg) : Exception(msg)
{

}
//---------------------------------------------------------------------------
__fastcall EImportaUgridDAO::EImportaUgridDAO(AnsiString msg) : EImportaUgrid(msg)
{

}
//---------------------------------------------------------------------------

