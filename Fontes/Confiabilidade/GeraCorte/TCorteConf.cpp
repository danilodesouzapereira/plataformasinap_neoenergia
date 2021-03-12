//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCorteConf.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCorteConf::TCorteConf(void)
	{
	lisEqpto  = new TList();
	lisChvSoc = new TList();
	lisFontes = new TList();
	lisFontesP = new TList();
	pcorte0    = NULL;


	lisCortes = NULL;
	eqpto     = NULL;
	}
//---------------------------------------------------------------------------
__fastcall TCorteConf::TCorteConf(VTEqpto* eqpto, TCorteConf* pcorte0)
	{
	this->eqpto = eqpto;
	lisCortes = new TList();
   this->pcorte0  = pcorte0;

	lisEqpto  = new TList();
	lisChvSoc = new TList();
	lisFontes = new TList();
	lisFontesP = new TList();

	}
//---------------------------------------------------------------------------
__fastcall TCorteConf::~TCorteConf(void)
	{
	if(lisEqpto) {delete lisEqpto; lisEqpto = NULL;}
	if(lisChvSoc) {delete lisChvSoc; lisChvSoc = NULL;}
	if(lisFontes) {delete lisFontes; lisFontes = NULL;}
	if(lisFontesP) {delete lisFontesP; lisFontesP = NULL;}
	}
//---------------------------------------------------------------------------
AnsiString __fastcall TCorteConf::Codigo(void)
   {
   VTEqpto* eqpto;
   AnsiString aux;

   aux = "";
   for(int n = 0; n < lisEqpto->Count; n++)
      {
      eqpto = (VTEqpto*)lisEqpto->Items[n];
      aux.cat_sprintf("%s ", eqpto->Codigo.c_str());
      }
   return(aux);
   }
//---------------------------------------------------------------------------
