//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMemoria.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMemoria::TMemoria(void)
	{
	//obt�m informa��o da mem�ria
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	}

//---------------------------------------------------------------------------
__fastcall TMemoria::~TMemoria(void)
   {
	//nada a fazer
	}

//-----------------------------------------------------------------------------
double __fastcall TMemoria::EmUso_perc(void)
	{
	return(statex.dwMemoryLoad);
   }

//-----------------------------------------------------------------------------
//eof



