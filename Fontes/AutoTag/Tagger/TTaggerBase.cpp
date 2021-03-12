//---------------------------------------------------------------------------

#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
//---------------------------------------------------------------------------
#include "TTaggerBase.h"
#include "VTAutoTag.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TTaggerBase::TTaggerBase(VTApl *apl, VTAutoTag *autoTag)
{
	this->apl_ = apl;
	autoTag_ = autoTag;
}
//---------------------------------------------------------------------------
__fastcall TTaggerBase::~TTaggerBase(void)
{
	#ifdef _DEBUG
//	Application->MessageBoxW(L" TTaggerBase destructor", L"Aviso", MB_OKCANCEL);
	#endif
}
//---------------------------------------------------------------------------
void __fastcall  TTaggerBase::InicializaComentariosAutomaticos(void)
{
	//faz nada
}
//---------------------------------------------------------------------------
bool  __fastcall TTaggerBase::InsereTags(VTAcao *acao, bool undo)
{
	//faz nada
	return true;
}

//---------------------------------------------------------------------------
