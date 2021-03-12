// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "VTElemVisual.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall VTElemVisual::VTElemVisual(void)
{
	// inicia atributos do status
	Status[sttEV_NOVO] = true;
}

// ---------------------------------------------------------------------------
__fastcall VTElemVisual::~VTElemVisual(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall VTElemVisual::DefineStatus(unsigned mask_bit, bool enabled)
{
	// define status indicado
	Status[mask_bit] = enabled;
}

// ---------------------------------------------------------------------------
bool __fastcall VTElemVisual::PM_GetStatus(unsigned bit)
{
	return ((status & bit) == bit);
}

// ---------------------------------------------------------------------------
unsigned __fastcall VTElemVisual::PM_GetStatusAsUnsigned(void)
{
	return (status);
}

// ---------------------------------------------------------------------------
void __fastcall VTElemVisual::PM_SetStatus(unsigned mask_bit, bool enabled)
{
	if (enabled)
	{
		status |= mask_bit;
	}
	else
	{
		status &= ~mask_bit;
	}
}

// ---------------------------------------------------------------------------
void __fastcall VTElemVisual::PM_SetStatusAsUnsigned(unsigned status)
{
	status = status;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall VTElemVisual::StatusAlterado(void)
{
	return ((status & sttEV_ALTERADO) == sttEV_ALTERADO);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall VTElemVisual::StatusExcluido(void)
{
	return ((status & sttEV_EXCLUIDO) == sttEV_EXCLUIDO);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall VTElemVisual::StatusNovo(void)
{
	return ((status & sttEV_NOVO) == sttEV_NOVO);
}

// ---------------------------------------------------------------------------
// eof