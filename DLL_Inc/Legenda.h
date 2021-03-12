//---------------------------------------------------------------------------
#ifndef LegendaH
#define LegendaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTLegenda;
class VTVetval;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTLegenda* __fastcall DLL_NewObjLegenda(VTApl *apl);
EXPORT VTVetval*  __fastcall DLL_NewObjVetval(int ind_atrib, AnsiString descricao, int num_pat);

//---------------------------------------------------------------------------
#endif
//eof


 