//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TArvore.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTArvore* __fastcall NewObjArvore(void)
	{
	try{
		return(new TArvore());
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//---------------------------------------------------------------------------
VTArvore* __fastcall NewObjArvore(VTEqpto* eqpto, TArvore* parv)
	{
	try{
		return(new TArvore(eqpto, parv));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//---------------------------------------------------------------------------
__fastcall TArvore::TArvore(void)
	{
	eqpto   = NULL;
	arv_ori = NULL;
	arv_prot= NULL;
	arv_raiz= NULL;
	pliga   = NULL;
	lisArv     = new TList();
	lisArvAct  = new TList();
	lisNA      = new TList();
	iarv       = -1;
	}
//---------------------------------------------------------------------------
__fastcall TArvore::TArvore(VTEqpto* eqpto, VTArvore* parv)
	{
	this->eqpto = eqpto;
	arv_ori = parv;
	arv_prot= NULL;
	arv_raiz= NULL;
	pliga   = NULL;
	lisArv     = new TList();
	lisArvAct  = new TList();
	lisNA      = new TList();
	iarv       = -1;
	}
//---------------------------------------------------------------------------
__fastcall TArvore::~TArvore(void)
	{
	if(lisArv)
		{
		//LimpaTList(lisArv);
		for(int n = 0; n < lisArv->Count; n++)
			{
			if(lisArv->Items[n]) {delete ((TArvore*)lisArv->Items[n]);}
			}
		delete lisArv; lisArv = NULL;}
	if(lisNA) {delete lisNA; lisNA = NULL;}
	if(lisArvAct) {delete lisArvAct; lisArvAct = NULL;}
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::AddEqpto(VTEqpto* eqpto_aux, VTLigacao* pliga)
	{
	TArvore* arv_aux;

	if(eqpto_aux == eqpto) return(NULL);
	//Verifica se o eqpto/liga já foi incluído nos descendentes
	for(int n = 0; n < lisArv->Count; n++)
		{
		arv_aux = (TArvore*)lisArv->Items[n];
		if((arv_aux->Eqpto == eqpto_aux)&&(arv_aux->Ligacao == pliga)) return(NULL);//--
		}
	//Verifica se o eqpto já foi incluído no caminho
	arv_aux = this;
	while(arv_aux->arv_ori != NULL)
	  {
	  if(arv_aux->arv_ori->Eqpto == eqpto_aux) return(NULL);
	  if(arv_aux->Ligacao != NULL) {if(arv_aux->Ligacao == pliga) return(NULL);}//--
	  arv_aux = (TArvore*)arv_aux->arv_ori;
	  }

	arv_aux = new TArvore(eqpto_aux, this);
	arv_aux->Ligacao = pliga;//--	arv_aux->DefLigacao(pliga);

	lisArv->Add(arv_aux);

	return(arv_aux);
	}
//---------------------------------------------------------------------------
//VTEqpto* __fastcall TArvore::Eqpto1(void)
//	{
//   return(eqpto);
//   }
//---------------------------------------------------------------------------
void __fastcall TArvore::IniciaArvore(VTEqpto* eqpto)
   {
   this->eqpto = eqpto;
   arv_ori = NULL;
   LimpaTList(lisArv);
   iarv = -1;

   }
//---------------------------------------------------------------------------
TList* __fastcall TArvore::ListaArvores(void)
	{
	return(lisArv);
	}
//---------------------------------------------------------------------------
TList* __fastcall TArvore::ListaArvoresAct(void)
	{
	return(lisArvAct);
	}
//---------------------------------------------------------------------------
TList* __fastcall TArvore::ListaNA(void)
   {
   return(lisNA);
   }
//---------------------------------------------------------------------------
void __fastcall TArvore::EqptoNA(VTEqpto* eqpto)
   {
   if(lisNA->IndexOf(eqpto) != -1) return;
   lisNA->Add(eqpto);
   }
//---------------------------------------------------------------------------
bool __fastcall TArvore::NA(void)
   {
	return(!(lisNA->Count == 0));
   }
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::ProximoRamo(void)
	{
   if(!((iarv + 1) < lisArv->Count)) return(NULL);
   iarv++;
	return((TArvore*)lisArv->Items[iarv]);
   }
//---------------------------------------------------------------------------
VTEqpto* __fastcall TArvore::PM_GetEqpto(void)//Eqpto(void)
	{
	return(eqpto);
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::PM_GetArvOri(void)//ArvOri(void)
	{
	return(arv_ori);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::PM_SetArvOri(VTArvore* arv_ori)//ArvOriDef(VTArvore* arv_ori)
	{
	this->arv_ori = arv_ori;
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::PM_GetArvProt(void)//ArvProt(void)
	{
	return(arv_prot);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::PM_SetArvProt(VTArvore* arv_prot)//ArvProtDef(VTArvore* arv_prot)
	{
	this->arv_prot = arv_prot;
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::PM_GetArvRaiz(void)
	{
	return(arv_raiz);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::PM_SetArvRaiz(VTArvore* arv_raiz)
	{
	this->arv_raiz = arv_raiz;
	}
//---------------------------------------------------------------------------
VTLigacao* __fastcall TArvore::PM_GetLigacao(void)//Ligacao(void)
	{
	return(pliga);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::PM_SetLigacao(VTLigacao* pliga)//DefLigacao(VTLigacao* pliga)
	{
   this->pliga = pliga;
   }
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::ArvOri(void)
	{
	return(arv_ori);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::ArvOriDef(VTArvore* arv_ori)
	{
	this->arv_ori = arv_ori;
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::ArvRaiz(void)
	{
	return(arv_raiz);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::ArvRaizDef(VTArvore* arv_raiz)
	{
	this->arv_raiz = arv_raiz;
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvore::ArvProt(void)
	{
	return(arv_prot);
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::DefLigacao(VTLigacao* pliga)
	{
	this->pliga = pliga;
	}
//---------------------------------------------------------------------------
void __fastcall TArvore::ArvProtDef(VTArvore* arv_prot)
	{
	this->arv_prot = arv_prot;
	}
//---------------------------------------------------------------------------
VTLigacao* __fastcall TArvore::Ligacao(void)
	{
	return(pliga);
	}
//---------------------------------------------------------------------------

*/
