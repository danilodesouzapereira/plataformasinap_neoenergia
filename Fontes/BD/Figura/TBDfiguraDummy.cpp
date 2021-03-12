// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBDfiguraDummy.h"
#include "..\Apl\VTApl.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTBDfigura* NewObjBDfiguraDummy(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDfiguraDummy(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDfiguraDummy::TBDfiguraDummy(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	
}

// ---------------------------------------------------------------------------
__fastcall TBDfiguraDummy::~TBDfiguraDummy(void)
{

}
// ---------------------------------------------------------------------------
bool __fastcall  TBDfiguraDummy::LeAuxiliaresFiguras(void)
{
	return false;
}
// ---------------------------------------------------------------------------
bool __fastcall  TBDfiguraDummy::LeFiguras(VTEstudo *estudo)
{
	return true;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall  TBDfiguraDummy::LeFiguras(VTEstudo *estudo, bool estudo_novo, TList *lisREDE_SELE)
{
	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall  TBDfiguraDummy::RemoveEstudo(VTEstudo *estudo)
{
	return false;
}
// ---------------------------------------------------------------------------
bool __fastcall  TBDfiguraDummy::SalvaFiguras(VTEstudo *estudo)
{
	return true;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// eof
