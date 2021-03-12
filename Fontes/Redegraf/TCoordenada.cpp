//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCoordenada.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCoordenada* __fastcall NewObjCoordenada(void)
   {
   try{
      return(new TCoordenada());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
__fastcall TCoordenada::TCoordenada(void)
   {
   }

//-----------------------------------------------------------------------------
__fastcall TCoordenada::~TCoordenada(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TCoordenada::PM_SetArea(TArea *area)
	{
	PD.area     = area;
	PD.point    = NULL;
	PD.line     = NULL;
	PD.polyline = NULL;
	}

//---------------------------------------------------------------------------
void __fastcall TCoordenada::PM_SetPoint(TPoint *point)
	{
	PD.area     = NULL;
	PD.point    = point;
	PD.line     = NULL;
	PD.polyline = NULL;
	}

//---------------------------------------------------------------------------
void __fastcall TCoordenada::PM_SetLine(TLine *line)
   {
	PD.area     = NULL;
	PD.point    = NULL;
   PD.line     = line;
   PD.polyline = NULL;
   }
   
//---------------------------------------------------------------------------
void __fastcall TCoordenada::PM_SetPolyline(TPolyline *polyline)
   {
	PD.area     = NULL;
   PD.point    = NULL;
   PD.line     = NULL;
   PD.polyline = polyline;
   }

//---------------------------------------------------------------------------
//eof