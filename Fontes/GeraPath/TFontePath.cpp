//---------------------------------------------------------------------------
#pragma hdrstop
#include "TFontePath.h"
#include "VTArvore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TFontePath::TFontePath(VTEqpto* eqpto)
	{
   this->eqpto = eqpto;
	lisPath = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TFontePath::~TFontePath(void)
	{
	if(lisPath) {delete lisPath; lisPath = NULL;}
	}
//---------------------------------------------------------------------------
bool __fastcall TFontePath::AddCaminho(VTArvore* arv)
   {
   //Verifica se a arvore já foi incluída
   if(lisPath->IndexOf(arv) != -1) return(false); //se houver ciclo no caminho!
	//Verifica se é ramo sem bifurcação
	if((arv->ListaArvores())->Count > 0) return(false);
	lisPath->Add(arv);
	return(true);
   }
//---------------------------------------------------------------------------
VTEqpto* __fastcall TFontePath::Eqpto(void)
	{
	return(eqpto);
	}
//---------------------------------------------------------------------------
TList* __fastcall TFontePath::LisPath(void)
	{
	return(lisPath);
   }
//---------------------------------------------------------------------------
