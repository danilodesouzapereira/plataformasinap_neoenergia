//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBloco.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTBloco* __fastcall NewObjBloco(void)
   {
   return(new TBloco());
   }

//---------------------------------------------------------------------------
__fastcall TBloco::TBloco(void)
   {
   //cria lista
   lisBAR  = new TList();
   lisLIG  = new TList();
   lisREDE = new TList();
   //define cor
   PD.color = clWhite;
   PD.obj = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TBloco::~TBloco(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR)  {delete lisBAR;  lisBAR  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
   if (lisREDE) {delete lisREDE; lisREDE = NULL;}
	}
/*
//---------------------------------------------------------------------------
void __fastcall TBloco::DefineRedeParaBlocoSemLigacao(VTRede *rede)
   {
	if (lisLIG->Count > 0)  return;
	//pode um bloco c/ uma única Barra que é conectado por várias redes (exemplo, barramento da SE)
	//if (lisREDE->Count > 0) return;
   //insere Rede em lisREDE
	lisREDE->Add(rede);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBloco::ExisteBarra(VTBarra *barra)
   {
   //verifica se lisBAR contém a Barra
   return(lisBAR->IndexOf(barra) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TBloco::ExisteGeracao(void)
	{
	//variáveis locais
	VTBarra *barra;

	//loop p/ todas as Barras
	for (int n = 0; n < lisBAR->Count; n++)
		{
		barra = (VTBarra*)lisBAR->Items[n];
		if (barra->ExisteGeracao()) return(true);
		}
	return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBloco::ExisteLigacao(VTLigacao *ligacao)
   {
   //verifica se lisLIG contém a Ligacao
   return(lisLIG->IndexOf(ligacao) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TBloco::ExisteLigacaoDaRede(VTRede *rede)
   {
   //verifica se lisREDE contém a Rede
   return(lisREDE->IndexOf(rede) >= 0);
   }

//---------------------------------------------------------------------------
void __fastcall TBloco::InsereBarra(VTBarra *barra)
   {
   //insere Barra em lisBAR
   if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
   }

//---------------------------------------------------------------------------
void  __fastcall TBloco::InsereLigacao(VTLigacao *ligacao)
   {
   //verifica se a Ligacao já existe em lisLIG
   if (lisLIG->IndexOf(ligacao) < 0)
      {//insere Ligacao em lisLIG
      lisLIG->Add(ligacao);
      //verifica se a Rede da Ligacao já existe em lisREDE
      if (lisREDE->IndexOf(ligacao->rede) < 0)
         {//insere Rede em lisREDE
         lisREDE->Add(ligacao->rede);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TBloco::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
void  __fastcall TBloco::LisEqbar(TList *lisEXT, int tipo)
	{
	//variáveis locais
	VTBarra *barra;

	//loop p/ todas as Barras
	for (int n = 0; n < lisBAR->Count; n++)
		{
		barra = (VTBarra*)lisBAR->Items[n];
		barra->LisEqbar(lisEXT, tipo);
		}
	}

//---------------------------------------------------------------------------
TList* __fastcall TBloco::LisLigacao(void)
   {
   return(lisLIG);
   }
   
//---------------------------------------------------------------------------
void  __fastcall TBloco::LisLigacao(TList *lisEXT, int tipo)
	{
	//variáveis locais
	VTEqpto *eqpto;

	//loop p/ todas Ligacoes do Bloco
	for (int n = 0; n < lisLIG->Count; n++)
		{
		eqpto = (VTEqpto*)lisLIG->Items[n];
		if ((tipo < 0)||(eqpto->Tipo() == tipo))
			{
			if (lisEXT->IndexOf(eqpto) < 0) lisEXT->Add(eqpto);
			}
		}
	}

//---------------------------------------------------------------------------
TList* __fastcall TBloco::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
TColor __fastcall TBloco::PM_GetColor(void)
   {
   return(PD.color);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TBloco::PM_GetRede(void)
	{
	//variáveis locais
	VTLigacao *ligacao;

	//verifica se o Bloco possui Ligacoes
	if (lisLIG->Count == 0) return(NULL);
	//retorna Rede da Ligacao
	ligacao = (VTLigacao*)lisLIG->First();
	return(ligacao->rede);
	}

//---------------------------------------------------------------------------
void  __fastcall TBloco::PM_SetColor(TColor color)
   {
   PD.color = color;
   }

//---------------------------------------------------------------------------
//eof
