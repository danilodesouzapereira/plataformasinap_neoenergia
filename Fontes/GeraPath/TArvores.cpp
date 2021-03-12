//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TArvores.h"
#include "VTArvore.h"
#include "TEqptoPath.h"
#include "TFontePath.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTArvores* __fastcall NewObjArvores(VTEqpto* eqpto)
	{
	try{
		return(new TArvores(eqpto));
		}catch(Exception &e)
			{
			return(NULL);
			}
		}

//---------------------------------------------------------------------------
__fastcall TArvores::TArvores(VTEqpto* eqpto)
   {
   this->eqpto  = eqpto;
   flag_calc    = false;
   arvs_ori     = NULL;

	//lisArvs      = new TList();

	arv_raiz     = NewObjArvore();

	lisArvProt   = new TList();//lista de arvore de protecao
	lisPath      = new TList();//lista de caminhos - árvores extremas
	lisPathNF    = new TList();//lista de caminhos - árvores extremas
	lisEqpto     = new TList();//lista de equipamentos
	lisEqptoPath = new TList();//lista de caminhos por equipamento
	lisFontes    = new TList();//lista de fontes (eqptos de nós finais)
	lisFontesPath= new TList();//lista de caminhos por fonte (eqptos de nós finais)
	//lisCortes1   = new TList();//lista com cortes totais de 1a ordem
	//lisCortes2   = new TList();//lista com cortes totais de 2a ordem
	//lisCortes3   = new TList();//lista com cortes totais de 3a ordem
	lisCortesP   = new TList();//lista com cortes parciais
	}
//---------------------------------------------------------------------------
__fastcall TArvores::~TArvores(void)
   {
   //if(arv_raiz) {delete arv_raiz; arv_raiz = NULL;}
	if(lisArvProt) {delete lisArvProt; lisArvProt = NULL;}
	if(lisPath) {delete lisPath; lisPath = NULL;}
	if(lisPathNF) {delete lisPathNF; lisPathNF = NULL;}
	if(lisEqpto) {delete lisEqpto; lisEqpto = NULL;}
	//if(lisArvs) {LimpaTList(lisArvs); delete lisArvs; lisArvs = NULL;}
	//if(lisCortes1) {LimpaTList(lisCortes1); delete lisCortes1; lisCortes1 = NULL;}
	//if(lisCortes2) {LimpaTList(lisCortes2); delete lisCortes2; lisCortes2 = NULL;}
	//if(lisCortes3) {LimpaTList(lisCortes3); delete lisCortes3; lisCortes3 = NULL;}
	if(lisCortesP) {LimpaTList(lisCortesP); delete lisCortesP; lisCortesP = NULL;}
	if(lisEqptoPath) {LimpaTList(lisEqptoPath); delete lisEqptoPath; lisEqptoPath = NULL;}
	if(lisFontes) {delete lisFontes; lisFontes = NULL;}
	if(lisFontesPath) {LimpaTList(lisFontesPath); delete lisFontesPath; lisFontesPath = NULL;}



	}
//---------------------------------------------------------------------------
bool __fastcall TArvores::AddCaminho(VTArvore* arv)
	{
	int iaux;
	TFontePath *fpath;

	if(lisPath->IndexOf(arv) != -1) return(false);
	//Verifica se o ponto da arvore tem ramificação
	if((arv->ListaArvores())->Count > 0) return(false);
	//Adiciona a lista de caminhos gerais
	lisPath->Add(arv);
	//Verifica se tem chave NA
	if(!arv->NA()) lisPathNF->Add(arv);

	//atribui caminho a fonte
	if((iaux = lisFontes->IndexOf(arv->Eqpto)) == -1)
		{
		lisFontes->Add(arv->Eqpto);
		lisFontesPath->Add(fpath = new TFontePath(arv->Eqpto));
		}
	else
		{
		fpath = (TFontePath*)lisFontesPath->Items[iaux];
		}

	fpath->AddCaminho(arv);
	return(true);
	}
/*//---------------------------------------------------------------------------
bool __fastcall TArvores::AtribCaminho(VTArvore* arv, VTEqpto* eqpto)
	{
	TEqptoPath* eqpath;
	int iaux;
	bool resposta;

	if(!eqpto) return(false);

	if((iaux = lisEqpto->IndexOf(eqpto)) != -1)
	  {eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
		if(eqpath->Eqpto() != eqpto) return(false);
	  }
	else
	  {lisEqptoPath->Add(eqpath = new TEqptoPath(eqpto));
		lisEqpto->Add(eqpto);
	  }
	//Adiciona arv a eqpath->lisPath, lisPathNF
	return(eqpath->AddCaminho(arv));
	}
*/
//---------------------------------------------------------------------------
bool __fastcall TArvores::AtribCaminhoBarra(VTArvore* arv, VTEqpto* eqpto)
	{
	TEqptoPath  *eqpath;

	int iaux;
	bool resposta;

	//if(!(eqpto = (VTEqpto*)arvEqpto->Ligacao)) return(false);//--
	//Inclui a ligacao
	if((iaux = lisEqpto->IndexOf(eqpto)) != -1)
	  {eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
		if(eqpath->Eqpto() != eqpto) return(false);
	  }
	else
	  {lisEqptoPath->Add(eqpath = new TEqptoPath(eqpto));
		lisEqpto->Add(eqpto);
	  }

	//eqpath->AddEqProt(arvEqpto->ArvProt);//--

	//eqpath->DefEqProt();
	//Adiciona arv a eqpath->lisPath, lisPathNF
	return(eqpath->AddCaminho(arv));
	}
//---------------------------------------------------------------------------
bool __fastcall TArvores::AtribCaminho(VTArvore* arv, VTArvore* arvEqpto)
	{
	VTEqpto     *eqpto;
	TEqptoPath  *eqpath;

	int iaux;
	bool resposta;

	if(!(eqpto = (VTEqpto*)arvEqpto->Ligacao)) return(false);//--
	//Inclui a ligacao
	if((iaux = lisEqpto->IndexOf(eqpto)) != -1)
	  {eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
		if(eqpath->Eqpto() != eqpto) return(false);
	  }
	else
	  {lisEqptoPath->Add(eqpath = new TEqptoPath(eqpto));
		lisEqpto->Add(eqpto);
	  }
	eqpath->AddEqProt(arvEqpto->ArvProt);//--

	//eqpath->DefEqProt();
	//Adiciona arv a eqpath->lisPath, lisPathNF
	return(eqpath->AddCaminho(arv));
	}
//---------------------------------------------------------------------------
bool __fastcall TArvores::AtribCaminhoProt(VTArvore* arv, VTArvore* arvEqpto)
	{
	VTEqpto     *eqpto;
	TEqptoPath  *eqpath;
	int iaux;
	bool resposta;

	if(!(eqpto = (VTEqpto*)arvEqpto->Ligacao)) return(false);//--



	if((iaux = lisEqpto->IndexOf(eqpto)) != -1)
	  {eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
		if(eqpath->Eqpto() != eqpto) return(false);

	  }
	else
	  {lisEqptoPath->Add(eqpath = new TEqptoPath(eqpto));
		lisEqpto->Add(eqpto);
	  }

	eqpath->AddEqProt(arvEqpto->ArvProt);//--
	//Adiciona arv a eqpath->lisPath, lisPathNF
	return(eqpath->AddCaminhoProt(arv));
	}
/*//---------------------------------------------------------------------------
bool __fastcall TArvores::AtribCaminhoProt(VTArvore* arv, VTEqpto* eqpto)
	{
	TEqptoPath* eqpath;
	int iaux;
	bool resposta;

	if(!eqpto) return(false);



	if((iaux = lisEqpto->IndexOf(eqpto)) != -1)
	  {eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
		if(eqpath->Eqpto() != eqpto) return(false);

	  }
	else
	  {lisEqptoPath->Add(eqpath = new TEqptoPath(eqpto));
		lisEqpto->Add(eqpto);
	  }

	eqpath->AddEqProt(arvEqpto->ArvProt());
	//Adiciona arv a eqpath->lisPath, lisPathNF
	return(eqpath->AddCaminhoProt(arv));
	}
//---------------------------------------------------------------------------
bool __fastcall TArvores::AtribArvore(VTArvore* arv, VTEqpto* eqpto)
	{
	TEqptoPath* eqpath;
	int iaux;
	bool resposta;

	if(!eqpto) return(false);

	if((iaux = lisEqpto->IndexOf(eqpto)) != -1)
	  {eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
		if(eqpath->Eqpto() != eqpto) return(false);
	  }
	else
	  {lisEqptoPath->Add(eqpath = new TEqptoPath(eqpto));
		lisEqpto->Add(eqpto);
	  }
	//Adiciona arv a eqpath->lisPath, lisPathNF
	return(eqpath->AddArvore(arv));
	}
*/
//---------------------------------------------------------------------------
VTArvore* __fastcall TArvores::Arv(void)
	{
	return(arv_raiz);
	}
//---------------------------------------------------------------------------
VTEqpto* __fastcall TArvores::EqRaiz(void)
   {
   return(eqpto);
   }
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisPath(bool flagNF, VTEqpto* eqpto)
   {
   TList* lisAux;
   int iaux;
   TEqptoPath* eqpath;

   if(eqpto == NULL)
     {
     lisAux = (flagNF) ? lisPathNF : lisPath;
     return(lisAux);
	  }



   iaux = lisEqpto->IndexOf(eqpto);
   if(iaux < 0) return(NULL);


   eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
   //protecao
   if(eqpath->Eqpto() != eqpto) return(NULL);
   lisAux = eqpath->LisPath(flagNF);

   return(lisAux);
   }
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisPathProt(VTEqpto* eqpto)
	{
	TList* lisAux;
	int iaux;
	TEqptoPath* eqpath;

	if(eqpto == NULL) return(NULL);
	iaux = lisEqpto->IndexOf(eqpto);
	if(iaux < 0) return(NULL);
	eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
	//protecao
	if(eqpath->Eqpto() != eqpto) return(NULL);
	lisAux = eqpath->LisPathProt();

	return(lisAux);
	}
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisEqProt(VTEqpto* eqpto)
	{
	TList* lisAux;
	int iaux;
	TEqptoPath* eqpath;

	if(eqpto == NULL) return(NULL);
	iaux = lisEqpto->IndexOf(eqpto);
	if(iaux < 0) return(NULL);
	eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
	//protecao
	if(eqpath->Eqpto() != eqpto) return(NULL);
	lisAux = eqpath->LisEqProt();

	return(lisAux);
	}
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisArvProt(void)
   {
	return(lisArvProt);
   }
/*//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisCortes(int icorte)
	{
	TList* lisCortes;
	switch(icorte)
	  {
	  case 1:  lisCortes = lisCortes1; break;
	  case 2:  lisCortes = lisCortes2; break;
	  case 3:  lisCortes = lisCortes3; break;
	  default: lisCortes = NULL; break;
	  }
	return(lisCortes);
	}
*/
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisCortesP(void)
   {
   return(lisCortesP);
   }
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisEqpto(void)
   {
   return(lisEqpto);
   }
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisEqptoPath(void)
	{
	return(lisEqptoPath);
	}
//---------------------------------------------------------------------------
TList* __fastcall TArvores::LisFontesPath(void)
	{
	return(lisFontesPath);
	}
//---------------------------------------------------------------------------
bool __fastcall TArvores::NA(VTEqpto* eqpto)
   {
   int iaux;
   TEqptoPath* eqpath;

   iaux = lisEqpto->IndexOf(eqpto);
   if(iaux < 0) return(true);
   eqpath = (TEqptoPath*)lisEqptoPath->Items[iaux];
   return(eqpath->NA());
   }
//---------------------------------------------------------------------------
