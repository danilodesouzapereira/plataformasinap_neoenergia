//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLigaSerie.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TLigaSerie::TLigaSerie(void)
   {
	lisEqpto   = new TList();
	lisBarra   = new TList();
   //Trecho NF
   i_na = -1;
   }
//---------------------------------------------------------------------------
__fastcall TLigaSerie::~TLigaSerie(void)
   {
	if(lisEqpto) {delete lisEqpto; lisEqpto = NULL;}
	if(lisBarra) {delete lisBarra; lisBarra = NULL;}
	}
//---------------------------------------------------------------------------
bool __fastcall TLigaSerie::AddEqpto(VTEqpto* eqpto)
   {
   //Verifica se a arvore já foi incluída
   if(lisEqpto->IndexOf(eqpto) != -1) return(false);
   //Adiciona eqpto a lista
	lisEqpto->Add(eqpto);
	switch (eqpto->Tipo())
			 {
			 case eqptoCHAVE:
					if(((VTChave*)eqpto)->Aberta) i_na = -2;
					break;
			 case eqptoTRECHO:
					Comprimento_km += ((VTTrecho*)eqpto)->Comprimento_km;
					break;
			 default:
			 break;
			 }

	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TLigaSerie::DefineObjBarraSerie(VTBarra *pbar)
	{
	if(pbar == NULL) return;
	if(lisBarra->IndexOf(pbar) != -1) return;
	lisBarra->Add(pbar);
	}

//---------------------------------------------------------------------------
int __fastcall TLigaSerie::Tipo(void)
	{
	return(i_na);
	}
//---------------------------------------------------------------------------
TList* __fastcall TLigaSerie::LisEqpto(void)
	{
	return(lisEqpto);
	}
//---------------------------------------------------------------------------
void __fastcall TLigaSerie::LisBarra(TList *lisEQP)
	{
	if(lisBarra->Count > 0)
	  {
	  lisEQP->Assign(lisBarra, laOr);
	  }
	else
     {
	  //insere Barras da Ligacao em lisEQP
	  if ((pbarra1)&&(lisEQP->IndexOf(pbarra1) < 0)) lisEQP->Add(pbarra1);
	  if ((pbarra2)&&(lisEQP->IndexOf(pbarra2) < 0)) lisEQP->Add(pbarra2);
	  if ((pbarra3)&&(lisEQP->IndexOf(pbarra3) < 0)) lisEQP->Add(pbarra3);
	  }
	}

//---------------------------------------------------------------------------

