//---------------------------------------------------------------------------
#pragma hdrstop
#include "TEqptoPath.h"
#include "VTArvore.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TEqptoPath::TEqptoPath(VTEqpto* eqpto)
	{
	this->eqpto = eqpto;
	lisFontes  = new TList();
	lisPath   = new TList();
	lisPathP  = new TList();
	lisArv    = new TList();
	lisPathNF = new TList();
	lisEqProt = new TList();
	flag_NA   = true;
	}
//---------------------------------------------------------------------------
__fastcall TEqptoPath::~TEqptoPath(void)
	{
	if(lisArv)    {delete lisArv;    lisArv = NULL;}
	if(lisFontes) {delete lisFontes; lisFontes = NULL;}
	if(lisPath)   {delete lisPath;   lisPath = NULL;}
	if(lisPathP)  {delete lisPathP;  lisPathP = NULL;}
	if(lisPathNF) {delete lisPathNF; lisPathNF = NULL;}
	if(lisEqProt) {delete lisEqProt; lisEqProt = NULL;}
	}
//---------------------------------------------------------------------------
bool __fastcall TEqptoPath::AddCaminho(VTArvore* arv)
	{
	int iaux;

	//Verifica se a arvore já foi incluída
	if(lisPath->IndexOf(arv) != -1) return(false); //se houver ciclo no caminho!
	//Verifica se é ramo sem bifurcação
	if((arv->ListaArvores())->Count > 0) return(false);
	lisPath->Add(arv);
	//Verifica se existe um caminho ao menos com conexão
	if(!arv->NA())
	  {lisPathNF->Add(arv);
		flag_NA = false;
	  }

	if(lisFontes->IndexOf(arv->Eqpto) == -1) lisFontes->Add(arv->Eqpto);

	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TEqptoPath::AddCaminhoProt(VTArvore* arv)
	{
	int iaux;

	//Verifica se a arvore já foi incluída
	if(lisPathP->IndexOf(arv) != -1) return(false); //se houver ciclo no caminho!
	//Verifica se é ramo sem bifurcação
	if((arv->ListaArvores())->Count > 0) return(false);
	lisPathP->Add(arv);
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TEqptoPath::AddArvore(VTArvore* arv)
	{
	//Verifica se a arvore já foi incluída
	if(lisArv->IndexOf(arv) != -1) return(false); //se houver ciclo no caminho!
	lisArv->Add(arv);
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TEqptoPath::AddEqProt(VTArvore* arv)
	{
	VTLigacao* pliga;

	if(arv == NULL) return(false);
	if((pliga = arv->Ligacao) == NULL) return(false);//--
	if(lisEqProt->IndexOf(pliga) != -1) return(false);

	lisEqProt->Add(pliga);
	return(true);
	}
//---------------------------------------------------------------------------
VTEqpto* __fastcall TEqptoPath::Eqpto(void)
	{
	return(eqpto);
	}
//---------------------------------------------------------------------------
TList* __fastcall TEqptoPath::LisArv(void)
	{
	return(lisArv);
	}
//---------------------------------------------------------------------------
TList* __fastcall TEqptoPath::LisEqProt(void)
	{
	return(lisEqProt);
	}
//---------------------------------------------------------------------------
TList* __fastcall TEqptoPath::LisPath(bool flagNF)
	{
	TList* lisAux;
	lisAux = (flagNF) ? lisPathNF : lisPath;
	return(lisAux);
	}
//---------------------------------------------------------------------------
TList* __fastcall TEqptoPath::LisPathProt(void)
	{
	return(lisPathP);
	}
//---------------------------------------------------------------------------
bool __fastcall TEqptoPath::NA(void)
	{
	return(flag_NA);
	}
//---------------------------------------------------------------------------
