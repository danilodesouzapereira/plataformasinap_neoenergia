//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include "TGrupoChv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGrupo* NewObjGrupoChv(VTEqpto* eqpto)
	{
	return(new TGrupoChv(eqpto));
	}

//---------------------------------------------------------------------------
__fastcall TGrupoChv::TGrupoChv(VTEqpto* eqpto)
	{
	lisCHV = new TList();
	pgrupo_aux = NULL;
	p1    = NULL;
	p2    = NULL;
	flag_conec  = false;
	flag_vis    = false;
	itipo    =  tipoChv;
	itipoVar = -1;
	enabled  = true;
	ativo    = true;
	if(eqpto) AddEqpto(eqpto);
	}
//---------------------------------------------------------------------------
__fastcall TGrupoChv::~TGrupoChv(void)
	{
	if(lisCHV) {delete lisCHV; lisCHV = NULL;}
	}
//---------------------------------------------------------------------------
void __fastcall TGrupoChv::AddEqpto(VTEqpto* eqpto)
	{
	if(eqpto->Tipo() != eqptoCHAVE) return;
	this->estado = ((VTChave*)eqpto)->Estado;
	if(lisCHV->IndexOf(eqpto) < 0) lisCHV->Add(eqpto);
	}
//---------------------------------------------------------------------------
int __fastcall TGrupoChv::Estado(void)
	{
	VTChave* pchv;
	if(lisCHV->Count == 0) return(-1);
	pchv = (VTChave*)lisCHV->First();
	this->estado = pchv->Estado;
	return(pchv->Estado);
	}
//---------------------------------------------------------------------------
void __fastcall TGrupoChv::Abre(void)
	{
	VTChave* pchv;
	for(int n = 0; n < lisCHV->Count; n++)
		{
		pchv = (VTChave*)lisCHV->Items[n];
		pchv->Abre();
		this->estado = pchv->Estado;
		}

	}
//---------------------------------------------------------------------------
void __fastcall TGrupoChv::Fecha(void)
	{
	VTChave* pchv;
	for(int n = 0; n < lisCHV->Count; n++)
		{
		pchv = (VTChave*)lisCHV->Items[n];
		pchv->Fecha();
		this->estado = pchv->Estado;
		}
	}
//---------------------------------------------------------------------------
AnsiString __fastcall TGrupoChv::Codigo(void)
	{
	VTChave* pchv;
	AnsiString aux;
	aux = "";
	for(int ng = 0; ng < lisCHV->Count; ng++)
		{
		pchv  = (VTChave*)lisCHV->Items[ng];
		aux.cat_sprintf("%s ", pchv->Codigo.c_str());
		}
	return(aux);
	}
//---------------------------------------------------------------------------
VTEqpto* __fastcall TGrupoChv::Eqpto(int neq)
	{
	//protecao
	if(!(neq < lisCHV->Count)) return(NULL);

	return((VTEqpto*)lisCHV->Items[neq]);
	}
//---------------------------------------------------------------------------
TList* __fastcall TGrupoChv::LisEqpto(void)
	{
	return(lisCHV);
	}
//---------------------------------------------------------------------------
int __fastcall TGrupoChv::NEq(void)
	{
	return(lisCHV->Count);
	}
//---------------------------------------------------------------------------
void __fastcall TGrupoChv::RemEqpto(VTEqpto* eqpto)
	{
	if(!(lisCHV->IndexOf(eqpto) < 0)) lisCHV->Remove(eqpto);
	}
//---------------------------------------------------------------------------
bool __fastcall TGrupoChv::VerEqpto(VTEqpto* eqpto)
	{
	if(lisCHV->IndexOf(eqpto) < 0) return(false);
	else
	  return(true);
	}
//---------------------------------------------------------------------------
