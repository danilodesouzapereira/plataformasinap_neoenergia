//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGeraPath.h"
#include "VTArvore.h"
#include "VTArvores.h"
#include "TEqptoPath.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Diretorio\DirArq.h"
#include "..\GeraRede\TLigaSerie.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTLigacao.h"
#include "..\Diretorio\VTPath.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTGeraPath* __fastcall NewObjGeraPath(VTApl *apl)
	{
	return(new TGeraPath(apl));
	}
//---------------------------------------------------------------------------
__fastcall TGeraPath::TGeraPath(VTApl* apl)
	{
	//Objeto construtor de caminhos dados conjunto de eqptos destino (cargas),
	//eqptos origem(suprimentos e geradores)
	this->apl = apl;
	lisFontes   = new TList();
	lisCargas   = new TList();
	lisLiga     = NULL;
	lisL12      = new TList();
	lisL21      = new TList();
	lisBarraLig = new TList();
	lisLigBarra = new TList();
	lisTipoProt = new TList();
	flagBarra   = true;
	}
//---------------------------------------------------------------------------
__fastcall TGeraPath::~TGeraPath(void)
	{
	//if(arv) {delete arv; arv = NULL;}
	if(lisFontes) {delete lisFontes; lisFontes = NULL;}
	if(lisCargas) {delete lisCargas; lisCargas = NULL;}
	if(lisL12)  {delete lisL12; lisL12 = NULL;}
	if(lisL21)  {delete lisL21; lisL21 = NULL;}
	if(lisBarraLig)  {delete lisBarraLig; lisBarraLig = NULL;}
	if(lisLigBarra)  {LimpaTList(lisLigBarra); delete lisLigBarra; lisLigBarra = NULL;}
	if(lisTipoProt)  {delete lisTipoProt; lisTipoProt = NULL;}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::AddCarga(VTEqpto* eqpto)
	{
	if(!eqpto)
	  {lisCargas->Clear();
		return;
	  }
	if(lisCargas->IndexOf(eqpto) != -1) return;
	lisCargas->Add(eqpto);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::AddFonte(VTEqpto* eqpto)
	{
	if(!eqpto)
	  {lisFontes->Clear();
		return;
	  }
	if(lisFontes->IndexOf(eqpto) != -1) return;
	lisFontes->Add(eqpto);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::AddLigaExc(VTLigacao* pliga, bool flag_12)
	{
	VTLigacao  *pliga_serie, *pliga_aux;
	TList      *lisEqpto;


	if(!pliga)
	  {
	  lisL12->Clear();
	  lisL21->Clear();
	  return;
	  }

	if(lisLiga->IndexOf(pliga) < 0)
	  {
	  pliga_serie = NULL;
	  for(int nl = 0; nl < lisLiga->Count; nl++)
		{
		pliga_serie = (VTLigacao*)lisLiga->Items[nl];
		if(pliga_serie->Tipo() < 0)
		  {
		  lisEqpto = ((TLigaSerie*)pliga_serie)->LisEqpto();
		  if(lisEqpto->IndexOf(pliga) != -1) break;
		  }
		pliga_serie = NULL;
		}
	  if(pliga_serie == NULL) return;
	  else
		 {
		 //Descarta pliga_serie em barra1->barra2 e barra2->barra1
		 //informação não guardada em lisEqpto da ligacao serie
		 lisL12->Add(pliga_serie); lisL21->Add(pliga_serie);
		 return;
		 }
	  }

	if(flag_12)
	  lisL12->Add(pliga);
	else
	  lisL21->Add(pliga);
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TGeraPath::BckTrk(VTArvore* parv)
	{
	VTArvore* arv1;
	VTArvore* arv2;

	arv1 = parv;
	while((arv2 = arv1->ProximoRamo()) == NULL)
		  {arv1 = arv1->ArvOri;//--
			if(!arv1) return(NULL);
		  }
	return(arv2);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::DefLiga(TList* lisLiga)
	{
	VTLigacao *plig;
	VTBarra   *pbarra1, *pbarra2;
	VTBarra   *pbarra;
	int       iaux;
	TList     *lisAux, *lisBarra;

	this->lisLiga = lisLiga;

	lisBarra = new TList();
	//Define lista de barras e correspondente lista de lista de ligações às barras
	//Reinicia listas
	lisBarraLig->Clear();
	LimpaTList(lisLigBarra);
	for(int nl = 0; nl < lisLiga->Count; nl++)
		{
		plig = (VTLigacao*)lisLiga->Items[nl];
		lisBarra->Clear();
		plig->LisBarra(lisBarra);
		for(int nb = 0; nb < lisBarra->Count; nb++)
			{
			pbarra = (VTBarra*)lisBarra->Items[nb];
			iaux = lisBarraLig->IndexOf(pbarra);
			if(iaux == -1)
			  {lisBarraLig->Add(pbarra);
				lisLigBarra->Add(lisAux = new TList());
			  }
			else
			  {lisAux = (TList*)lisLigBarra->Items[iaux];
			  }
			lisAux->Add(plig);
			}
		}
		/*
		//Verifica barra1
		iaux = lisBarraLig->IndexOf(plig->pbarra1);
		if(iaux == -1)
		  {lisBarraLig->Add(plig->pbarra1);
			lisLigBarra->Add(lisAux = new TList());
		  }
		else
		  {lisAux = (TList*)lisLigBarra->Items[iaux];
		  }
		lisAux->Add(plig);
		//Verifica barra 2
		iaux = lisBarraLig->IndexOf(plig->pbarra2);
		if(iaux == -1)
		  {lisBarraLig->Add(plig->pbarra2);
			lisLigBarra->Add(lisAux = new TList());
		  }
		else
		  {lisAux = (TList*)lisLigBarra->Items[iaux];
		  }
		lisAux->Add(plig);
		//Verifica barra3 se existir
		if(plig->pbarra3 == NULL) continue;
		iaux = lisBarraLig->IndexOf(plig->pbarra3);
		if(iaux == -1)
		  {lisBarraLig->Add(plig->pbarra3);
			lisLigBarra->Add(lisAux = new TList());
		  }
		else
		  {lisAux = (TList*)lisLigBarra->Items[iaux];
		  }
		lisAux->Add(plig);
		}
		*/
	delete lisBarra;
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraArvore(TList* lisArv, TList* lisSUP, bool flag_inv)
	{
	//Gera lista de arvores, uma por carga a partir de arvores dos suprimentos
	VTEqpto   *eqpto, *eqptoCarga;
	VTArvores *arvs, *arvsGeral;
	VTArvore  *arv;
	TList     *lisAux;
	TList     *lisArvS;//Arvores dos suprimentos

	lisArvS = new TList();//Arvores a partir dos suprimentos
	//Inicia lista de arvores
	LimpaTList(lisArv);

	//Gera conjunto de arvores para cada eqpto de lisFontes


	//Cria todos os caminhos com origem no suprimento
	for(int nsup = 0; nsup < lisSUP->Count; nsup++)
		{
		eqpto = (VTEqpto*)lisSUP->Items[nsup];
		arvs = GeraArvoreIni(eqpto, flag_inv);
		lisArvS->Add(arvs);
		}

	//Gera atuacao da protecao
	GeraProtPath1(lisArvS);    //ok
	//Gera conjunto de arvores por destino (carga)
	GeraPathFonte1(lisArvS, lisArv);//GeraPathFonte2(lisArvS, lisArv);
	//Gera ligacao equipamento->caminhos
	GeraEqPath1(lisArv);

	LimpaTList(lisArvS);
	delete lisArvS;
	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraLisArv(TList* lisArv)
	{
	TList      *lisPathf;
	VTArvores  *arvf;
	VTArvore   *arv;

	for(int nf = 0; nf < lisArv->Count; nf++)
		{
		arvf = (VTArvores*)lisArv->Items[nf];
		lisPathf = arvf->LisPath(false);
		for(int np = lisPathf->Count - 1; np >= 0; np--)
			{
			arv = (VTArvore*)lisPathf->Items[np];
			if(arv->Eqpto == arvf->EqRaiz()) continue;
			lisPathf->Delete(np);
			}
		}

	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraPathFonte1(TList* lisArvS, TList* lisArv)
	{
	VTArvores  *arvs, *arvf;
	VTArvore   *arv;
	VTEqpto    *eqFonte, *eqpto;
	TList      *lisEqpto, *lisEqptoPath;
	TList      *lisEqptof, *lisEqptoPathf;
	TList      *lisPath, *lisPathf;
	TEqptoPath *eqPath, *eqPathf;
	TList      *lisPathEqpto, *lisPathEqptof;
	TList      *lisP;
	int        iaux;
	//ALTERA : CRIAR UNIÃO DE TODOS OS CAMINHOS PREVIAMENTE
	for(int nf = 0; nf < lisFontes->Count; nf++)
		{
		eqFonte  = (VTEqpto*)lisFontes->Items[nf];
		//Cria conjunto de árvores
		lisArv->Add(arvf = NewObjArvores(eqFonte));
		lisPathf = arvf->LisPath(false);
		//lisEqptof  = arvf->LisEqpto();
		//Cria lista de equipamentos (todos os eqptos de lisArvS), lisEqptof
		//Cria lista de caminhos TOTAIS que partem de eqFonte, lisPathf
		for(int n = 0; n < lisArvS->Count; n++)
			{
			arvs = (VTArvores*)lisArvS->Items[n];
			(arvf->LisArvProt())->Assign(arvs->LisArvProt(), laOr);
			//lisEqpto     = arvs->LisEqpto();
			//lisEqptof->Assign(lisEqpto, laOr);
			lisPath = arvs->LisPath(false);
			for(int np = lisPath->Count - 1; np >=0 ; np--)
				{
				arv = (VTArvore*)lisPath->Items[np];
				if(arv->Eqpto != eqFonte) continue;
				lisPathf->Add(arv);
				lisPath->Delete(np);
				}
			}
		}

	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		lisPath = arvs->LisPath(false);
		lisPath->Clear();
		lisPath = arvs->LisPath(true);
		lisPath->Clear();
		}


	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraPathFonte2(TList* lisArvS, TList* lisArv)
	{
	VTArvores  *arvs, *arvf;
	TList      *lisPath, *lisPathf;

	//Cria uma arvore para todas os suprimentos
	lisArv->Add(arvf = NewObjArvores(NULL));
	lisPathf = arvf->LisPath(false);
	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		(arvf->LisArvProt())->Assign(arvs->LisArvProt(), laOr);
		lisPathf->Assign(arvs->LisPath(false), laOr);
		}

	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		lisPath = arvs->LisPath(false);
		lisPath->Clear();
		lisPath = arvs->LisPath(true);
		lisPath->Clear();
		}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraEqPath1(TList* lisArv)
	{
	VTArvores  *arvs;
	VTArvore   *arv1, *arv2, *arv_act;
	VTEqpto    *eqpto;
	TList      *lisPath;
	TList      *lisProt, *lisArvAct;


	//Atribui equipamentos a caminhos das arvores
	lisArvAct = new TList();
	for(int narv = 0; narv < lisArv->Count; narv++)
		{
		arvs = (VTArvores*)lisArv->Items[narv];
		lisProt = arvs->LisArvProt();
		lisPath = arvs->LisPath(false);//todos os caminhos
		for(int n = 0; n < lisPath->Count; n++)
			{
			lisArvAct->Clear();
			arv1 = (VTArvore*)lisPath->Items[n];
			arv2 = arv1;

			if(flagBarra) arvs->AtribCaminhoBarra(arv1, arv1->ArvRaiz->Eqpto);//Inclui barras: barra suprim->pbarra
			while(arv2 != NULL)
			  {
			  eqpto = (VTEqpto*)arv2->Ligacao;//--
			  if(eqpto == NULL) {arv2 = arv2->ArvOri; continue;}//--
			  arvs->AtribCaminho(arv1, arv2); //arvs->AtribCaminho(arv1, eqpto);  //gera novo elemento eqpto/ eqptoPath
			  if(flagBarra) arvs->AtribCaminhoBarra(arv1, arv2->Eqpto);//Inclui barras
			  //Adiciona equipamentos que podem atuar a protecao
			  if(lisProt->IndexOf(arv2) != -1)
			     {lisArvAct->Assign(arv2->ListaArvoresAct(), laOr);}
			  //arvore a montante
			  arv2 = arv2->ArvOri;//--
			  }

			arv2 = arv1;
			while(arv2 != NULL)
			  {
			  lisArvAct->Remove(arv2);
			  //arvore a montante
			  arv2 = arv2->ArvOri;//--
			  }

			//Adiciona equipamentos de protecao
			for(int nact = 0; nact < lisArvAct->Count; nact++)
				{
				arv2 =  (VTArvore*)lisArvAct->Items[nact];
				eqpto = (VTEqpto*)arv2->Ligacao;//--
				if(eqpto == NULL) continue;
				arvs->AtribCaminhoProt(arv1, arv2);//arvs->AtribCaminhoProt(arv1, eqpto);    //llll
				}

			}
		}

	delete lisArvAct;
	}
//---------------------------------------------------------------------------
VTArvores* __fastcall TGeraPath::GeraArvoreIni(VTEqpto* eqpto, bool flag_inv)
	{
	TList*   lisArv;
	TList*   lisArv_aux;
	VTArvores* arvs;
	VTArvore* arv1;
	VTArvore* arv2;
	VTArvore* arv3;
	VTArvore* arv_raiz;
	VTEqpto* eqpto_aux;
	AnsiString obj_name, class_name;

	//Arvore deve partir de uma barra ou equipamento de barra
	if(eqpto->Tipo() == eqptoLIGACAO) return(NULL);

	switch(eqpto->Tipo())
			{
			case eqptoCARGA:
				  eqpto_aux = ((VTEqbar*)eqpto)->pbarra;
				  break;
			case eqptoSUPRIMENTO:
				  eqpto_aux = ((VTEqbar*)eqpto)->pbarra;
				  break;
			case eqptoGERADOR:
				  eqpto_aux = ((VTEqbar*)eqpto)->pbarra;
				  break;
			default:
				  eqpto_aux = eqpto;
				  break;
			}


	//Cria conjunto de árvores
	arvs = NewObjArvores(eqpto); //arvs->eqpto = eqpto
	//Inicia arvore de caminhos - atribui
	arv_raiz = arv1 = arvs->Arv();//= arv_ori
	arv1->IniciaArvore(eqpto_aux);//(((VTEqbar*)eqpto)->pbarra);
	//Gera todas a arvore a partir de eqpto
	int iconta = 0;
	while(arv1 != NULL)
		{
		iconta++;
		GeraRamos1(arv1, true, flag_inv);//flag_NA=true: descarta chaves NA
		arv2 = arv1->ProximoRamo();
		if(arv2 != NULL)
			 {arv1 = arv2;
			  continue;}
		//NOVALTERA
		//if(lisFontes->IndexOf(arv1->Eqpto()) != -1)
		arvs->AddCaminho(arv1); //adiciona a arvs->lisPath e a arvs->lisPathNF
		arv1->ArvRaiz = arv_raiz;//-- arv1->ArvRaizDef(arv_raiz);
		arv1 = BckTrk(arv1);
		}
	return(arvs);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraProtPath1(TList* lisArvS)
	{
	VTArvores *arvs;
	VTArvore  *arv1, *arv2, *arv_aux;
	TList     *lisP, *lisPath, *lisPathEqptoProt;
	VTLigacao *pliga;
	VTChave   *pchave;
	int       iaux;
	TEqptoPath *eqptoPath;

	lisP    = new TList();
	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		//Busca todos(false) os caminhos com extremidade na carga
		//ALTERA pegar apenas caminhos NF
		lisPath = arvs->LisPath(false);
		for(int np = 0; np < lisPath->Count; np++)
			{
			arv1 = (VTArvore*)lisPath->Items[np];
			//Varre a árvore do destino até a origem
			arv2 = arv1;
			lisP->Clear();
			while(arv2 != NULL)
				{
				arv2->ArvProt = NULL;//--	arv2->ArvProtDef(NULL); //arv2->arv_prot = NULL
				//Lista lisP guarda todos TArvore a jusante da protecao
				//SAIlisP->Add(arv2);
				pliga = arv2->Ligacao; //--
				if(!pliga) {arv2 = arv2->ArvOri; continue;}//--

				if(pliga->Tipo() != eqptoCHAVE)
					 {lisP->Add(arv2); //ENTRA
					  arv2 = arv2->ArvOri;//--
					  continue;
					 }

				//Verifica se é eqpto de protecao
				pchave = (VTChave*)pliga;
				if(lisTipoProt->IndexOf(pchave->TipoChave) == -1)
					 {lisP->Add(arv2); //ENTRA
					  arv2 = arv2->ArvOri;//--
					  continue;
					 }


				//Adiciona arv2 a lista de arvore de protecao
				//ALTERA testar se não está somando mais de uma vez
				(arvs->LisArvProt())->Add(arv2);//arvs->lisArvProt->Add(arv2)
				for(int nprot = 0; nprot < lisP->Count; nprot++)
							 {
							 arv_aux = (VTArvore*)lisP->Items[nprot];
							 arv_aux->ArvProt = arv2;//--	 arv_aux->ArvProtDef(arv2);//arv_aux->arv_prot = arv2
							 (arv2->ListaArvoresAct())->Add(arv_aux);
							 }
				lisP->Clear();
				arv2  = arv2->ArvOri; //--
				}
			}
		}
	delete lisP;
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraCaminhos(TList* lisArv, VTArvores* arvs)
	{
	VTArvore* arv1;
	VTArvore* arv2;
	VTArvore* arv_prot;
	VTChave*  pchave;
	VTEqpto*  eqpto_aux;


	for(int n = 0; n < lisArv->Count; n++)
		{
		//Busca extremidade
		arv1 = (VTArvore*)lisArv->Items[n];

		//Inicia eqpto de protecao
		arv_prot = NULL;

		//Preenche lista de chaves abertas em arv1->lisNA
		arv2 = arv1;
		while(arv2 != NULL)
		 {
		 eqpto_aux = arv2->Ligacao;//--
		 if(eqpto_aux)
			{
			if(eqpto_aux->Tipo() == eqptoCHAVE)
			  {
			  pchave = (VTChave*)eqpto_aux;
			  if(pchave->Aberta) arv1->EqptoNA(eqpto_aux);
			  else
				 {if(lisTipoProt->IndexOf(pchave->TipoChave) != -1) arv_prot = arv2;}
			  //if(pchave->TipoBaseFusivel || pchave->TipoDisjuntor || pchave->TipoReligadora)

			  }
			else
			  {//TLigaSerie
			  if(eqpto_aux->Tipo() == -2) arv1->EqptoNA(eqpto_aux);
			  }
			}
		 //Define primeiro ponto a jusante da arvore com equipamento de protecao
		 if(arv_prot)
			 arv2->ArvProt = arv_prot;//-- arv2->ArvProtDef(arv_prot);
		 //Se move em direcao a carga
		 arv2 = arv2->ArvOri;//--
		 }
		//Adiciona caminho a VTArvores:arvs->lisPath e arvs->lisPathNF
		arvs->AddCaminho(arv1);

		//Atribui lista de equipamentos percorridos pelo caminho
		//em arvs->lisEqpto (lista de equipamentos) e
		//em arvs->lisEqptoPath(lista de eqptopath, para cada equipamento
		//lista de caminhos)
		arv2 = arv1;
		while(arv2 != NULL)
		 {
		 //PENDENCIA INCLUSÃO DE BARRAS arvs->AtribCaminho(arv1, arv2->Eqpto());
		 arvs->AtribCaminho(arv1, arv2);//arvs->AtribCaminho(arv1, arv2->Ligacao());
		 arv2 = arv2->ArvOri;//--
		 }
		}
	}
//---------------------------------------------------------------------------
VTArvores* __fastcall TGeraPath::GeraArvore(VTEqpto* eqpto, bool flag_inv)
	{
	VTArvores* arvs;
	VTArvore*  arv1;
	TList*   lisFR;//Listas de fontes alcançadas
	TList*   lisOF;//Listas de outros caminhos
	TList*   lisPath;

	lisFR = new TList();

	arvs = GeraArvoreIni(eqpto, flag_inv);
	lisPath = arvs->LisPath(false);
	lisFR->Assign(lisPath, laOr);
	for(int n = lisFR->Count - 1; n >= 0; n--)
		{
		arv1 = (VTArvore*)lisFR->Items[n];
		if(lisFontes->IndexOf(arv1->Eqpto) == -1) lisFR->Remove(arv1);
		}

	//Gera caminhos e armazena em VTArvores
	GeraCaminhos(lisFR, arvs);
	delete lisFR;
	return(arvs);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraRamos1(VTArvore* parv, bool flag_NA, bool flag_inv)
	{
	VTEqpto* eq_aux;
	VTEqbar* eqbar;
	VTLigacao* plig;
	TList    *lisA, *lisB;
	TList    *lisLigaAux, *lisAux;
	bool     resposta = false;
	double   v0, v1;
	int      iaux;

	//Verifica se o ramo já atingiu lista de fontes
	if(lisFontes->IndexOf(parv->Eqpto) != -1) return(false);
	if(!lisLiga) return(false);

	if(flag_inv)
		{lisA = lisL21; lisB = lisL12;}
	else
		{lisA = lisL12; lisB = lisL21;}


	//lisBarraLig: lista de barras da rede vinculado a lisLigBarra com as ligações com origem na barra
	iaux = lisBarraLig->IndexOf(parv->Eqpto);
	if(iaux < 0) return(false);

	lisLigaAux = (TList*)lisLigBarra->Items[iaux];
	lisAux     = new TList();
	//Preenche lista de ligações do conjunto de redes
	for(int nl = 0; nl < lisLigaAux->Count; nl++)
		{
		plig = (VTLigacao*)lisLigaAux->Items[nl];
		//flag_NA = true: se chave NA não gera ramo
		if(flag_NA)
		  {
		  if(plig->Tipo() == eqptoCHAVE)
			  {if(((VTChave*)plig)->Aberta) continue;
			  }
		  }
		//Verifica se esta em lista de ligações/ sentido excludentes
		if((lisA->IndexOf(plig) != -1)&&(plig->pbarra1 == parv->Eqpto))
			 {int ii = 0; continue;}
		if((lisB->IndexOf(plig) != -1)&&(plig->pbarra2 == parv->Eqpto))
			 {int jj = 0; continue;}

		/*//teste billington
		if(parv->Eqpto->Codigo.AnsiCompare("7")==0)
			{if((plig->pbarra1->Codigo.AnsiCompare("5")==0)||(plig->pbarra2->Codigo.AnsiCompare("5")==0))
				continue;
			}

		if(parv->Eqpto->Codigo.AnsiCompare("17")==0)
			{if((plig->pbarra1->Codigo.AnsiCompare("7")==0)||(plig->pbarra2->Codigo.AnsiCompare("7")==0))
				continue;
			}
		//teste
		*/

		lisAux->Clear();
		plig->LisBarra(lisAux);
		lisAux->Remove(parv->Eqpto);
		for(int nb = 0; nb < lisAux->Count; nb++)
			  {
			  eq_aux = (VTEqpto*)lisAux->Items[nb];
			  //Verifica se o equipamento não foi incluído em descentes e nos ascendentes
			  if(parv->AddEqpto(eq_aux, plig))
			                          resposta = true;
			  }
		}
	//Pesquisa em ligações a fontes hk08a
	for(int nf = 0; nf < lisFontes->Count; nf++)
		{
		eqbar = (VTEqbar*)lisFontes->Items[nf];
		if(eqbar->pbarra != parv->Eqpto) continue;
		if(parv->AddEqpto(eqbar, NULL)) resposta = true;
		}

	delete lisAux; //ALTERA criação de lista - deixar criada
	return(resposta);

	}
//---------------------------------------------------------------------------
VTArvores* __fastcall TGeraPath::GeraCopia(VTArvores* arvs, VTEqpto* eqpto)
	{
	VTArvores* arv_copia;

	if(!arvs) return(NULL);
	arv_copia = NewObjArvores(eqpto);
	arv_copia->arvs_ori  = arvs;
	arv_copia->res_conf  = arvs->res_conf;

	return(arv_copia);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::LisFontesConect(VTEqpto* eqpto, TList* lisConect)
	{
	VTArvores* arvs;
	VTArvore* arv1;
	VTArvore* arv2;
	bool     resposta = false;
	//------------
	//Função que retorna lista de fontes conectadas ao equipamento
	//------------
	//Cria conjunto de árvores
	arvs = NewObjArvores(eqpto);
	//Inicia arvore de caminhos - atribui
	arv1 = arvs->Arv();//= arv_ori
	arv1->IniciaArvore(eqpto);
	//Gera todas a arvore a partir de eqpto
	while(arv1 != NULL)
		{
		//Gera ramos sem retorno e
		if(lisFontes->IndexOf(arv1->Eqpto) != -1)
		  {
		  if(lisConect->IndexOf(arv1->Eqpto) == -1) lisConect->Add(arv1->Eqpto);
		  resposta = true;
		  }
		GeraRamos1(arv1, true);
		arv2 = arv1->ProximoRamo();
      arv1 = (arv2 != NULL) ? arv2 : BckTrk(arv1);
		}
   delete arvs;
   return(resposta);
   }
//---------------------------------------------------------------------------
TList* __fastcall TGeraPath::LisTipoProt(void)
	{
	//Define tipos de chaves que atuam como protecao
	return(lisTipoProt);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::LigaIsolada(VTLigacao* pliga)
	{
	TList* lisB1 = new TList();
	TList* lisB2 = new TList();
	bool   flag_carga1, flag_sup1;
	bool   flag_carga2, flag_sup2;



	lisB1->Clear();
	if(!LisFontesConect(pliga->pbarra1, lisB1))
								  {//individuo->lisChvIso->Add(ldesp);
									delete lisB1;
									delete lisB2;
									return(true);
								  }

	lisB2->Clear();
	if(!LisFontesConect(pliga->pbarra2, lisB2))
								  {//individuo->lisChvIso->Add(ldesp);
									delete lisB1;
									delete lisB2;
									return(true);
								  }

	flag_carga1 = flag_sup1 = false;
	for(int n1 = 0; n1 < lisB1->Count; n1++)
		{
		if(((VTEqpto*)lisB1->Items[n1])->Tipo() == eqptoCARGA)
		  flag_carga1 = true;
		else
		  flag_sup1 = true;

		if(flag_carga1 && flag_sup1) break;
		}

	flag_carga2 = flag_sup2 = false;
	for(int n2 = 0; n2 < lisB2->Count; n2++)
		{
		if(((VTEqpto*)lisB2->Items[n2])->Tipo() == eqptoCARGA)
		  flag_carga2 = true;
		else
		  flag_sup2 = true;

		if(flag_carga2 && flag_sup2) break;
		}
	delete lisB1;
	delete lisB2;
	if(flag_carga1 && flag_sup2) return(false);
	if(flag_carga2 && flag_sup1) return(false);
	return(true);
	//individuo->lisChvIso->Add(ldesp);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::VerConect(VTEqpto* eqpto)
	{
	VTArvores* arvs;
	VTArvore* arv1;
   VTArvore* arv2;
   bool     resposta = false;
   //------------
   //Função que verifica existencia de fonte de suprimento conectada a eqpto
   //------------
   //Cria conjunto de árvores
	arvs = NewObjArvores(eqpto);
   //Inicia arvore de caminhos - atribui
   arv1 = arvs->Arv();//= arv_ori
   arv1->IniciaArvore(eqpto);

   //Gera todas a arvore a partir de eqpto
   while(arv1 != NULL)
      {
		//Gera ramos sem retorno e
		if(lisFontes->IndexOf(arv1->Eqpto) != -1) {resposta = true; break;}
		GeraRamos1(arv1, true);
      arv2 = arv1->ProximoRamo();
      arv1 = (arv2 != NULL) ? arv2 : BckTrk(arv1);
      }
   delete arvs;
   return(resposta);
   }
//---------------------------------------------------------------------------





/*
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::BuscaInsercao(TList* lisP, TList* lisArvs, VTArvore* arv1)
	{
	VTArvores* arvs_aux;
	TList*   lisPath;
	TList*   lisArv;
	VTArvore* arv2;
	VTArvore* arv1_aux;
	VTArvore* arv_aux;
   VTLigacao* plig;
   bool     resposta = false;
   TList*   lisTMP;


   if(!lisArvs) return(resposta);
   //arv1 - eqpto atual
   lisTMP = new TList();
   //Busca construção anterior de arvore a partir de arv1
   for(int narv = 0; narv < lisArvs->Count; narv++)
		{
		arvs_aux = (VTArvores*)lisArvs->Items[narv];
      //Busca lista de caminhos em
		lisPath  = arvs_aux->LisPath(false, arv1->Eqpto());
      if(lisPath == NULL) continue;
		lisTMP->Clear();
      for(int np = 0; np < lisPath->Count; np++)
         {
         //Busca ponto de inserção comum aos caminhos a jusante
			arv2 = (VTArvore*)lisPath->Items[np];
			//Busca ponto de inserção em arv1a
         while((arv2->Eqpto() != arv1->Eqpto())&&(arv2 != NULL))
                    arv2 = arv2->ArvOri();

         if(arv2 == NULL) Aviso("problema!!!");
         if(lisTMP->IndexOf(arv2) != -1) continue;
         lisTMP->Add(arv2);
         //Adiciona ramificações a arvore atual arv1 a partir de arv2
         //arv1->AdicionaRamos(arv2, lisP, lisFontes);
         //continue;
         arv1_aux = arv1;
         plig = arv2->Ligacao();
         arv2 = arv2->ArvOri();
			//if(np == 5)
			//	int jj = 0;
         while(arv2 != NULL)
           {
			  arv_aux = arv1_aux->AddEqpto(arv2->Eqpto(), plig);
           arv1_aux = arv_aux;
			  //if(!arv1_aux)
			  //	 int ii = 0;
           //if(arv_aux) arv1_aux = arv_aux;
           //else
           //  {
           //  break;
           //  }

           //arv1_aux->iarv = (arv1_aux->ListaArvores())->Count;
           //arv1_aux->AdicionaRamos(arv2, lisP, lisFontes);
           plig = arv2->Ligacao();
			  arv2 = arv2->ArvOri();
			  }

             //AdicionaRamos(VTArvore* arv_ini, VTArvore* arv_ret, TList* lisPath, TList* lisFontes)
         //(arv2->ListaArvores())->Assign(arv2->ListaArvores(), laOr);
         //Adiciona caminhos a lista de caminhos
         //lisP->Assign(lisPath, laOr);
         }

		resposta = true;
		//break;
		}
	delete lisTMP;
	return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraArvore1(TList* lisArv, TList* lisSUP, bool flag_inv)
	{
	//Gera lista de arvores, uma por carga a partir de arvores dos suprimentos
	VTEqpto   *eqpto, *eqptoCarga;
	VTArvores *arvs, *arvsGeral;
	VTArvore *arv;
	TList    *lisAux;

	//Inicia lista de arvores
	LimpaTList(lisArv);

	//Gera conjunto de arvores para cada eqpto de lisFontes


	//Cria todos os caminhos com origem no suprimento
	for(int nsup = 0; nsup < lisSUP->Count; nsup++)
		{
		eqpto = (VTEqpto*)lisSUP->Items[nsup];
		arvs = GeraArvoreIni(eqpto, flag_inv);
		lisArv->Add(arvs);//lisArvS->Add(arvs);
		}

	//Gera atuacao da protecao
	GeraProtPath1(lisArv);
	//Gera ligacao equipamento->caminhos
	GeraEqPath1A(lisArv);



	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraProtPath(TList* lisArvS)
	{
	VTArvores *arvs;
	VTArvore  *arv1, *arv2, *arv_aux;
	TList     *lisP, *lisPath;
	VTLigacao *pliga;
	VTChave   *pchave;

	//Elimina caminhos da carga até chaves NA
	//PENDENCIA OPÇÕES DE CAMINHO COM CHAVE ABERTA DESCARTADAS
	lisP    = new TList();
	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		//Busca todos(false) os caminhos com extremidade na carga
		lisPath = arvs->LisPath(false);
		for(int np = 0; np < lisPath->Count; np++)
			{
			arv1 = (VTArvore*)lisPath->Items[np];
			//Varre a árvore do destino até a origem
			arv2 = arv1;
			lisP->Clear();
			while(arv2 != NULL)
				{
				arv2->ArvProtDef(NULL);
				//Lista lisP guarda todos TArvore a jusante da protecao
				lisP->Add(arv2);
				pliga = arv2->Ligacao();
				if(!pliga) {arv2 = arv2->ArvOri(); continue;}
				if(pliga->Tipo() != eqptoCHAVE) {arv2 = arv2->ArvOri(); continue;}
				pchave = (VTChave*)pliga;
				//Verifica se é eqpto de protecao
				if(lisTipoProt->IndexOf(pchave->TipoChave) == -1) {arv2 = arv2->ArvOri(); continue;}

				for(int nprot = 0; nprot < lisP->Count; nprot++)
							 {
							 arv_aux = (VTArvore*)lisP->Items[nprot];
							 arv_aux->ArvProtDef(arv2);
							 }
				lisP->Clear();
				arv2  = arv2->ArvOri();
				}
			}
		}
	delete lisP;
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraPathFonte(TList* lisArvS, TList* lisArv)
	{
	VTArvores  *arvs, *arvf;
	VTArvore   *arv;
	VTEqpto    *eqFonte, *eqpto;
	TList      *lisEqpto, *lisEqptoPath;
	TList      *lisEqptof, *lisEqptoPathf;
	TList      *lisPath, *lisPathf;
	TEqptoPath *eqPath, *eqPathf;
	TList      *lisPathEqpto, *lisPathEqptof;
	TList      *lisP;
	int        iaux;

	for(int nf = 0; nf < lisFontes->Count; nf++)
		{
		eqFonte = (VTEqpto*)lisFontes->Items[nf];
		//Cria conjunto de árvores
		lisArv->Add(arvf = NewObjArvores(eqFonte));
		lisPathf   = arvf->LisPath(false);
		lisEqptof  = arvf->LisEqpto();
		//Cria lista de equipamentos (todos os eqptos de lisArvS), lisEqptof
		//Cria lista de caminhos TOTAIS que partem de eqFonte, lisPathf
		for(int n = 0; n < lisArvS->Count; n++)
			{
			arvs = (VTArvores*)lisArvS->Items[n];
			lisEqpto     = arvs->LisEqpto();
			lisEqptof->Assign(lisEqpto, laOr);
			lisPath = arvs->LisPath(false);
			lisPathf->Assign(lisPath, laOr);
			for(int np = lisPathf->Count - 1; np >=0 ; np--)
				{
				arv = (VTArvore*)lisPathf->Items[np];
				if(arv->Eqpto() == eqFonte) continue;
				lisPathf->Delete(np);
				}
			}

		//Para cada equipamento eqpto de lisEqptof:
		//     cria eqpathf em lisEqptoPathf e popula lista de arvores de incidencia
		//, eqPathf->LisPath(false - todos os caminhos)

		lisEqptoPathf = arvf->LisEqptoPath();
		for(int neq = 0; neq < lisEqptof->Count; neq++)
			{
			eqpto = (VTEqpto*)lisEqptof->Items[neq];
			lisEqptoPathf->Add(eqPathf = new TEqptoPath(eqpto));
			lisPathEqptof = eqPathf->LisPath(false);
			for(int n = 0; n < lisArvS->Count; n++)
				{
				arvs       = (VTArvores*)lisArvS->Items[n];
				lisEqpto   = arvs->LisEqpto();
				iaux       = lisEqpto->IndexOf(eqpto);
				if(iaux < 0) continue;
				lisEqptoPath = arvs->LisEqptoPath();
				eqPath       = (TEqptoPath*)lisEqptoPath->Items[iaux];
				lisPathEqpto = eqPath->LisPath(false);
				lisPathEqptof->Assign(lisPathEqpto, laOr);

				for(int np = lisPathEqptof->Count - 1; np >=0 ; np--)
					{
					arv = (VTArvore*)lisPathEqptof->Items[np];
					if(arv->Eqpto() == eqFonte) continue;
					lisPathEqptof->Delete(np);
					}
				}
			}

		}

	for(int n = 0; n < lisArvS->Count; n++)
			{
			arvs = (VTArvores*)lisArvS->Items[n];
			lisPath = arvs->LisPath(false);
			lisPath->Clear();
			lisPath = arvs->LisPath(true);
			lisPath->Clear();
			}


	for(int n = 0; n < lisArv->Count; n++)
		{
		arvs = (VTArvores*)lisArv->Items[n];
		eqFonte = arvs->EqRaiz();

		//Elimina caminhos que não atingiram fonte em listas por equipamento
		lisPath = arvs->LisPath(false);
		for(int np = lisPath->Count - 1; np >= 0; np--)
			{
			arv  = (VTArvore*)lisPath->Items[np];
			eqpto = arv->Eqpto();
			if(eqpto != eqFonte) lisPath->Delete(np);
			}
		//Elimina caminhos que não atingiram fonte em listas por equipamento
		lisEqpto = arvs->LisEqpto();
		for(int neq = lisEqpto->Count - 1; neq >= 0 ; neq--)
			{
			eqpto = (VTEqpto*)lisEqpto->Items[neq];
			lisPath = arvs->LisPath(false, eqpto);
			for(int np = lisPath->Count - 1; np >= 0; np--)
				{
				arv  = (VTArvore*)lisPath->Items[np];
				eqpto = arv->Eqpto();
				if(eqpto != eqFonte) lisPath->Delete(np);
				}
			}
		//Elimina equipamentos e eqpath com zero caminhos
		for(int neq = lisEqpto->Count - 1; neq >= 0 ; neq--)
			{
			eqpto = (VTEqpto*)lisEqpto->Items[neq];
			lisPath = arvs->LisPath(false, eqpto);
			if(lisPath->Count > 0) continue;
			lisEqpto->Delete(neq);
			lisEqptoPath = arvs->LisEqptoPath();
			eqPath = (TEqptoPath*)lisEqptoPath->Items[neq];
			delete eqPath;
			lisEqptoPath->Delete(neq);
			}

		}


	}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraEqPath(TList* lisArvS)
	{
	VTArvores  *arvs;
	VTArvore   *arv1, *arv2, *arvProt;
	VTEqpto    *eqpto;
	TList      *lisPath;
	TEqptoPath *eqptoPath;
	TList       *lisPathEqpto;
	TList       *lisPathProt;
	TEqptoPath  *eqptoPathProt;
	int         iaux;
	TList       *lisEqpto, *lisEqptoPath;

	//Atribui equipamentos a caminhos das arvores
	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		lisPath = arvs->LisPath(false);//todos os caminhos
		for(int n = 0; n < lisPath->Count; n++)
			{
			arv1 = (VTArvore*)lisPath->Items[n];
			arv2 = arv1;
			while(arv2 != NULL)
			  {
			  eqpto = (VTEqpto*)arv2->Ligacao();
			  if(eqpto == NULL) {arv2 = arv2->ArvOri(); continue;}
			  arvs->AtribCaminho(arv1, eqpto);  //gera novo elemento eqpto/ eqptoPath
			  arv2 = arv2->ArvOri();
			  }
			}
		}

	//Adiciona caminhos eliminados pela atuação da protecao
	for(int n = 0; n < lisArvS->Count; n++)
		{
		arvs = (VTArvores*)lisArvS->Items[n];
		lisEqpto = arvs->LisEqpto();
		lisEqptoPath = arvs->LisEqptoPath();
		lisPath = arvs->LisPath(false);
		for(int np = 0; np < lisPath->Count; np++)
			{
			arv1 = (VTArvore*)lisPath->Items[np];
			arv2 = arv1;
			while(arv2 != NULL)
			  {
			  eqpto = (VTEqpto*)arv2->Ligacao();
			  if(eqpto == NULL) {arv2 = arv2->ArvOri(); continue;}
			  if((arvProt = arv2->ArvProt()) == NULL) {arv2 = arv2->ArvOri(); continue;}

			  iaux      = lisEqpto->IndexOf(eqpto);
			  eqptoPath = (TEqptoPath*)lisEqptoPath->Items[iaux];

			  eqpto        = (VTEqpto*)arvProt->Ligacao();
			  iaux         = lisEqpto->IndexOf(eqpto);
			  eqptoPathProt= (TEqptoPath*)lisEqptoPath->Items[iaux];

			  lisPathEqpto= eqptoPath->LisPath(false);
			  lisPathProt = eqptoPathProt->LisPath(false);
			  //Equipamento de protecao é acionado pelo eqpto a jusante
			  //A Lista referente a eqpto a jusante é unida lista referente ao eqpto de protecao
			  lisPathEqpto->Assign(lisPathProt, laOr);
			  arv2 = arv2->ArvOri();
			  }
			}
		}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraEqPath1A(TList* lisArv)
	{
	VTArvores  *arvs;
	VTArvore   *arv1, *arv2, *arv_act;
	VTEqpto    *eqpto;
	TList      *lisPath;
	TList      *lisProt, *lisArvAct;


	lisProt = new TList();
	for(int n = 0; n < lisArv->Count; n++)
		{
		arvs = (VTArvores*)lisArv->Items[n];
		lisProt->Assign(arvs->LisArvProt(), laOr);
		}


	//Atribui equipamentos a caminhos das arvores
	for(int narv = 0; narv < lisArv->Count; narv++)
		{
		arvs = (VTArvores*)lisArv->Items[narv];
		lisPath = arvs->LisPath(false);//todos os caminhos
		for(int n = 0; n < lisPath->Count; n++)
			{
			arv1 = (VTArvore*)lisPath->Items[n];
			arv2 = arv1;
			while(arv2 != NULL)
			  {
			  eqpto = (VTEqpto*)arv2->Ligacao();
			  if(eqpto == NULL) {arv2 = arv2->ArvOri(); continue;}
			  arvs->AtribCaminho(arv1, eqpto);  //gera novo elemento eqpto/ eqptoPath
			  if(lisProt->IndexOf(arv2) != -1)
				 {
				 lisArvAct = arv2->ListaArvoresAct();
				 for(int nac = 0; nac < lisArvAct->Count; nac++)
					 {
					 arv_act = (VTArvore*)lisArvAct->Items[nac];
					 arvs->AtribCaminho(arv1, arv_act->Ligacao());
					 }
				 }
			  arv2 = arv2->ArvOri();
			  }
			}

		}
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraRamos(VTArvore* parv, bool flag_NA, bool flag_inv)
	{
	VTEqpto* eq_aux;
	VTEqbar* eqbar;
	VTLigacao* plig;
	TList    *lisAux, *lisA, *lisB;
	bool     resposta = false;
	double   v0, v1;


	//Verifica se o ramo já atingiu lista de fontes
	if(lisFontes->IndexOf(parv->Eqpto()) != -1) return(false);
	if(!lisLiga) return(false);
	lisAux = new TList();
	if(!flag_inv)
		{lisA = lisL12; lisB = lisL21;}
	else
		{lisA = lisL21; lisB = lisL12;}
	//Preenche lista de ligações do conjunto de redes
	for(int nl = 0; nl < lisLiga->Count; nl++)
		{
		plig = (VTLigacao*)lisLiga->Items[nl];
		if(flag_NA)
		  {
		  if(plig->Tipo() == eqptoCHAVE)
			  {if(((VTChave*)plig)->Aberta) continue;
			  }
		  }
		//Verifica se eqarv pertence a plig
		lisAux->Clear();
		plig->LisBarra(lisAux);
		if(lisAux->IndexOf(parv->Eqpto()) == -1) continue;
		if((lisA->IndexOf(plig) != -1)&&(plig->pbarra1 == parv->Eqpto()))
			 continue;
		if((lisB->IndexOf(plig) != -1)&&(plig->pbarra2 == parv->Eqpto()))
			 continue;
		lisAux->Remove(parv->Eqpto());
		for(int nb = 0; nb < lisAux->Count; nb++)
			  {
			  eq_aux = (VTEqpto*)lisAux->Items[nb];
			  //Verifica se o equipamento não foi incluído em descentes e nos ascendentes
			  if(parv->AddEqpto(eq_aux, plig)) resposta = true;
			  }
		}
	//Pesquisa em ligações a fontes hk08a
	for(int nf = 0; nf < lisFontes->Count; nf++)
		{
		eqbar = (VTEqbar*)lisFontes->Items[nf];
		if(eqbar->pbarra != parv->Eqpto()) continue;
		if(parv->AddEqpto(eqbar, NULL)) resposta = true;
		}

	//hk08a delete lisLiga;
	delete lisAux;
	return(resposta);

	}
//---------------------------------------------------------------------------
VTGeraPath* __fastcall NewObjGeraPath(VTApl *apl)
	{
	return(new TGeraPath(apl));
	}
//---------------------------------------------------------------------------
__fastcall TGeraPath::TGeraPath(VTApl* apl)
	{
	//Objeto construtor de caminhos dados conjunto de eqptos destino (cargas),
	//eqptos origem(suprimentos e geradores)
	this->apl = apl;
	//arv = new TArvore();
	lisFontes   = new TList();
	lisCargas   = new TList();
	arv_aux     = NewObjArvore();
	lisLiga     = NULL;
	lisL12      = new TList();
	lisL21      = new TList();
	lisBarraLig = new TList();
	lisLigBarra = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TGeraPath::~TGeraPath(void)
	{
	//if(arv) {delete arv; arv = NULL;}
	if(lisFontes) {delete lisFontes; lisFontes = NULL;}
	if(lisCargas) {delete lisCargas; lisCargas = NULL;}
	if(lisL12)  {delete lisL12; lisL12 = NULL;}
	if(lisL21)  {delete lisL21; lisL21 = NULL;}
	if(lisBarraLig)  {delete lisBarraLig; lisBarraLig = NULL;}
	if(lisLigBarra)  {LimpaTList(lisLigBarra); delete lisLigBarra; lisLigBarra = NULL;}
	if(arv_aux) {delete arv_aux; arv_aux = NULL;}
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::AddCarga(VTEqpto* eqpto)
	{
	if(!eqpto)
	  {lisCargas->Clear();
		return;
	  }
	if(lisCargas->IndexOf(eqpto) != -1) return;
	lisCargas->Add(eqpto);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::AddFonte(VTEqpto* eqpto)
	{
	if(!eqpto)
	  {lisFontes->Clear();
		return;
	  }
	if(lisFontes->IndexOf(eqpto) != -1) return;
	lisFontes->Add(eqpto);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::AddLigaExc(VTLigacao* pliga, bool flag_12)
	{
	if(!pliga)
	  {
	  lisL12->Clear();
	  lisL21->Clear();
	  return;
	  }

	if(flag_12)
	  lisL12->Add(pliga);
	else
	  lisL21->Add(pliga);
	}
//---------------------------------------------------------------------------
VTArvore* __fastcall TGeraPath::BckTrk(VTArvore* parv)
	{
	VTArvore* arv1;
	VTArvore* arv2;

	arv1 = parv;
	while((arv2 = arv1->ProximoRamo()) == NULL)
		  {arv1 = arv1->ArvOri();
			if(!arv1) return(NULL);
		  }
	return(arv2);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::BuscaInsercao(TList* lisP, TList* lisArvs, VTArvore* arv1)
	{
	VTArvores* arvs_aux;
	TList*   lisPath;
	TList*   lisArv;
	VTArvore* arv2;
	VTArvore* arv1_aux;
   VTArvore* arv_aux;
	VTLigacao* plig;
   bool     resposta = false;
   TList*   lisTMP;


	if(!lisArvs) return(resposta);
   //arv1 - eqpto atual
   lisTMP = new TList();
   //Busca construção anterior de arvore a partir de arv1
   for(int narv = 0; narv < lisArvs->Count; narv++)
      {
		arvs_aux = (VTArvores*)lisArvs->Items[narv];
      //Busca lista de caminhos em
		lisPath  = arvs_aux->LisPath(false, arv1->Eqpto());
      if(lisPath == NULL) continue;
      lisTMP->Clear();
      for(int np = 0; np < lisPath->Count; np++)
			{
			//Busca ponto de inserção comum aos caminhos a jusante
			arv2 = (VTArvore*)lisPath->Items[np];
			//Busca ponto de inserção em arv1a
         while((arv2->Eqpto() != arv1->Eqpto())&&(arv2 != NULL))
                    arv2 = arv2->ArvOri();

         if(arv2 == NULL) Aviso("problema!!!");
         if(lisTMP->IndexOf(arv2) != -1) continue;
         lisTMP->Add(arv2);
			//Adiciona ramificações a arvore atual arv1 a partir de arv2
         //arv1->AdicionaRamos(arv2, lisP, lisFontes);
         //continue;
         arv1_aux = arv1;
			plig = arv2->Ligacao();
         arv2 = arv2->ArvOri();
			//if(np == 5)
			//	int jj = 0;
         while(arv2 != NULL)
           {
           arv_aux = arv1_aux->AddEqpto(arv2->Eqpto(), plig);
           arv1_aux = arv_aux;
			  //if(!arv1_aux)
			  //	 int ii = 0;
           //if(arv_aux) arv1_aux = arv_aux;
           //else
           //  {
			  //  break;
			  //  }

			  //arv1_aux->iarv = (arv1_aux->ListaArvores())->Count;
           //arv1_aux->AdicionaRamos(arv2, lisP, lisFontes);
           plig = arv2->Ligacao();
			  arv2 = arv2->ArvOri();
           }

             //AdicionaRamos(VTArvore* arv_ini, VTArvore* arv_ret, TList* lisPath, TList* lisFontes)
			//(arv2->ListaArvores())->Assign(arv2->ListaArvores(), laOr);
         //Adiciona caminhos a lista de caminhos
         //lisP->Assign(lisPath, laOr);
         }

      resposta = true;
      //break;
		}
   delete lisTMP;
   return(resposta);
   }
//---------------------------------------------------------------------------
void __fastcall TGeraPath::DefLiga(TList* lisLiga)
	{
	VTLigacao *plig;
	VTBarra   *pbarra1, *pbarra2;
	int       iaux;
	TList     *lisAux;

	this->lisLiga = lisLiga;

	//Define lista de barras e correspondente lista de lista de ligações às barras
	//Reinicia listas
	lisBarraLig->Clear();
	LimpaTList(lisLigBarra);
	for(int nl = 0; nl < lisLiga->Count; nl++)
		{
		plig = (VTLigacao*)lisLiga->Items[nl];
		//Verifica barra1
		iaux = lisBarraLig->IndexOf(plig->pbarra1);
		if(iaux == -1)
		  {lisBarraLig->Add(plig->pbarra1);
			lisLigBarra->Add(lisAux = new TList());
		  }
		else
		  {lisAux = (TList*)lisLigBarra->Items[iaux];
		  }
		lisAux->Add(plig);
		//Verifica barra 2
		iaux = lisBarraLig->IndexOf(plig->pbarra2);
		if(iaux == -1)
		  {lisBarraLig->Add(plig->pbarra2);
			lisLigBarra->Add(lisAux = new TList());
		  }
		else
		  {lisAux = (TList*)lisLigBarra->Items[iaux];
		  }
		lisAux->Add(plig);
		//Verifica barra3 se existir
		if(plig->pbarra3 == NULL) continue;
		iaux = lisBarraLig->IndexOf(plig->pbarra3);
		if(iaux == -1)
		  {lisBarraLig->Add(plig->pbarra3);
			lisLigBarra->Add(lisAux = new TList());
		  }
		else
		  {lisAux = (TList*)lisLigBarra->Items[iaux];
		  }
		lisAux->Add(plig);
		}

   }
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraArvore(TList* lisArvs)
	{

	//Para cada fonte -> gera arvore para os pontos de carga



	return(true);
	}
//---------------------------------------------------------------------------
VTArvores* __fastcall TGeraPath::GeraArvore(VTEqpto* eqpto, TList* lisArvs, bool flag_inv)
	{
	TList*   lisAux;
	TList*   lisArv;
	TList*   lisArv_aux;
	VTArvores* arvs;
	VTArvore* arv1;
	VTArvore* arv2;
	VTArvore* arv3;
	VTEqpto* eqpto_aux;
	AnsiString obj_name, class_name;

	//Arvore deve partir de uma barra ou equipamento de barra
	if(eqpto->Tipo() == eqptoLIGACAO) return(NULL);
	//hkSup obj_name = eqpto->ClassName();
	//class_name = "TCarga";
	//eqpto_aux = (obj_name.AnsiCompareIC(class_name) == 0) ? ((VTEqbar*)eqpto)->pbarra : eqpto;

	switch(eqpto->Tipo())
			{
			case eqptoCARGA:
				  eqpto_aux = ((VTEqbar*)eqpto)->pbarra;
				  break;
			case eqptoSUPRIMENTO:
				  eqpto_aux = ((VTEqbar*)eqpto)->pbarra;
				  break;
			case eqptoGERADOR:
				  eqpto_aux = ((VTEqbar*)eqpto)->pbarra;
				  break;
			default:
				  eqpto_aux = eqpto;
				  break;
			}


	lisAux = new TList();
	//Cria conjunto de árvores
	arvs = NewObjArvores(eqpto);
	//Inicia arvore de caminhos - atribui
	arv1 = arvs->Arv();//= arv_ori
	arv1->IniciaArvore(eqpto_aux);//(((VTEqbar*)eqpto)->pbarra);
	//Gera todas a arvore a partir de eqpto
	int iconta = 0;
	//while((arv1 != NULL)&&(iconta < 1000))
	while(arv1 != NULL)
		{
		iconta++;
		//iconta++;
		//Gera ramos sem retorno e
		//GeraRamos(arv1, true, flag_inv);
		GeraRamos1(arv1, true, flag_inv);
		arv2 = arv1->ProximoRamo();
		if(arv2 != NULL) {arv1 = arv2; continue;}
		if(lisFontes->IndexOf(arv1->Eqpto()) != -1) lisAux->Add(arv1);
		arv1 = BckTrk(arv1);
		//if(iconta> 1000) break;
		}

	//Elimina ramos que não atingiram fontes
	//EliminaRamos(arvs->Arv());
	//Gera caminhos busca extremidades incluindo fontes e geradores
	//BuscaExtrem(arvs->Arv(), lisAux, false);
	//Gera caminhos e armazena em VTArvores
	GeraCaminhos(lisAux, arvs);
	delete lisAux;

	return(arvs);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraPath::GeraCaminhos(TList* lisArv, VTArvores* arvs)
	{
	VTArvore* arv1;
	VTArvore* arv2;
	VTEqpto*  eqpto_aux;

	for(int n = 0; n < lisArv->Count; n++)
      {
      //Busca extremidade
      arv1 = (VTArvore*)lisArv->Items[n];
		//Preenche lista de chaves abertas em arv1->lisNA
      arv2 = arv1;
      while(arv2 != NULL)
       {
       eqpto_aux = arv2->Ligacao();
		 if(eqpto_aux)
			{
         if(eqpto_aux->Tipo() == eqptoCHAVE)
			  {if(((VTChave*)eqpto_aux)->Aberta)
								arv1->EqptoNA(eqpto_aux);
			  }
         else
			  {if(eqpto_aux->Tipo() == -2)  //TLigaSerie
                        arv1->EqptoNA(eqpto_aux);
			  }
         }
       arv2 = arv2->ArvOri();
       }
		//Adiciona caminho a VTArvores:arvs->lisPath e arvs->lisPathNF
		arvs->AddCaminho(arv1);

      //Atribui lista de equipamentos percorridos pelo caminho
      //em arvs->lisEqpto (lista de equipamentos) e
		//em arvs->lisEqptoPath(lista de eqptopath, para cada equipamento
      //lista de caminhos)
      arv2 = arv1;
      while(arv2 != NULL)
       {
		 //PENDENCIA arvs->AtribCaminho(arv1, arv2->Eqpto());
		 arvs->AtribCaminho(arv1, arv2->Ligacao());
		 arv2 = arv2->ArvOri();
		 }
		}

	}
//---------------------------------------------------------------------------
VTArvores* __fastcall TGeraPath::GeraCopia(VTArvores* arvs, VTEqpto* eqpto)
	{
	VTArvores* arv_copia;

	if(!arvs) return(NULL);
	arv_copia = NewObjArvores(eqpto);
	arv_copia->arv_ori = arvs;
	arv_copia->res_conf = arvs->res_conf;

	return(arv_copia);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraRamos1(VTArvore* parv, bool flag_NA, bool flag_inv)
	{
	VTEqpto* eq_aux;
	VTEqbar* eqbar;
	VTLigacao* plig;
	TList    *lisA, *lisB;
	TList    *lisLigaAux, *lisAux;
	bool     resposta = false;
	double   v0, v1;
	int      iaux;


	//Verifica se o ramo já atingiu lista de fontes
	if(lisFontes->IndexOf(parv->Eqpto()) != -1) return(false);
	if(!lisLiga) return(false);



	if(!flag_inv)
		{lisA = lisL12; lisB = lisL21;}
	else
		{lisA = lisL21; lisB = lisL12;}

	iaux = lisBarraLig->IndexOf(parv->Eqpto());
	if(iaux < 0) return(false);

	lisLigaAux = (TList*)lisLigBarra->Items[iaux];
	lisAux     = new TList();
	//hk08a lisLiga = new TList();
	//Preenche lista de ligações do conjunto de redes
	//hk08a redes->LisLigacao(lisLiga);
	for(int nl = 0; nl < lisLigaAux->Count; nl++)
		{
		plig = (VTLigacao*)lisLigaAux->Items[nl];
		if(flag_NA)
		  {
		  if(plig->Tipo() == eqptoCHAVE)
			  {if(((VTChave*)plig)->Aberta) continue;
			  }
		  }

		if((lisA->IndexOf(plig) != -1)&&(plig->pbarra1 == parv->Eqpto()))
			 continue;
		if((lisB->IndexOf(plig) != -1)&&(plig->pbarra2 == parv->Eqpto()))
			 continue;

		lisAux->Clear();
		plig->LisBarra(lisAux);
		lisAux->Remove(parv->Eqpto());
		for(int nb = 0; nb < lisAux->Count; nb++)
			  {
			  eq_aux = (VTEqpto*)lisAux->Items[nb];
			  //Verifica se o equipamento não foi incluído em descentes e nos ascendentes
			  if(parv->AddEqpto(eq_aux, plig)) resposta = true;
			  }
		}
	//Pesquisa em ligações a fontes hk08a
	for(int nf = 0; nf < lisFontes->Count; nf++)
		{
		eqbar = (VTEqbar*)lisFontes->Items[nf];
		if(eqbar->pbarra != parv->Eqpto()) continue;
		if(parv->AddEqpto(eqbar, NULL)) resposta = true;
		}

	//hk08a delete lisLiga;
	delete lisAux;
	return(resposta);

	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::GeraRamos(VTArvore* parv, bool flag_NA, bool flag_inv)
	{
	VTEqpto* eq_aux;
	VTEqbar* eqbar;
	VTLigacao* plig;
	TList    *lisAux, *lisA, *lisB;
	bool     resposta = false;
	double   v0, v1;


	//Verifica se o ramo já atingiu lista de fontes
	if(lisFontes->IndexOf(parv->Eqpto()) != -1) return(false);
	if(!lisLiga) return(false);

	lisAux = new TList();
	//hk08a lisLiga = new TList();


	if(!flag_inv)
		{lisA = lisL12; lisB = lisL21;}
	else
		{lisA = lisL21; lisB = lisL12;}
	//Preenche lista de ligações do conjunto de redes
	//hk08a redes->LisLigacao(lisLiga);
	for(int nl = 0; nl < lisLiga->Count; nl++)
		{
		plig = (VTLigacao*)lisLiga->Items[nl];
		if(flag_NA)
		  {
		  if(plig->Tipo() == eqptoCHAVE)
			  {if(((VTChave*)plig)->Aberta) continue;
			  }
		  }
		//Verifica se eqarv pertence a plig
		lisAux->Clear();
		plig->LisBarra(lisAux);
		if(lisAux->IndexOf(parv->Eqpto()) == -1) continue;


		if((lisA->IndexOf(plig) != -1)&&(plig->pbarra1 == parv->Eqpto()))
			 continue;
		if((lisB->IndexOf(plig) != -1)&&(plig->pbarra2 == parv->Eqpto()))
			 continue;

		//if((lisL12->IndexOf(plig) != -1)&&(plig->pbarra1 == parv->Eqpto()))
		//	 continue;
		//if((lisL21->IndexOf(plig) != -1)&&(plig->pbarra2 == parv->Eqpto()))
		//	 continue;
		lisAux->Remove(parv->Eqpto());
		for(int nb = 0; nb < lisAux->Count; nb++)
			  {
			  eq_aux = (VTEqpto*)lisAux->Items[nb];
			  //Compara as tensões nominais entre barra de origem (justante) e barra de destino (montante)
			  //PENDENCIA
			  //v0 =((VTBarra*)(parv->Eqpto()))->vnom;
			  //v1 =((VTBarra*)eq_aux)->vnom;
			  //hk TEMP if(v1 < v0) continue;

			  //Verifica se o equipamento não foi incluído em descentes e nos ascendentes
			  if(parv->AddEqpto(eq_aux, plig)) resposta = true;
			  }
		}
	//Pesquisa em ligações a fontes hk08a
	for(int nf = 0; nf < lisFontes->Count; nf++)
		{
		eqbar = (VTEqbar*)lisFontes->Items[nf];
		if(eqbar->pbarra != parv->Eqpto()) continue;
		if(parv->AddEqpto(eqbar, NULL)) resposta = true;
		}

	//hk08a delete lisLiga;
	delete lisAux;
	return(resposta);

	}
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::LisFontesConect(VTEqpto* eqpto, TList* lisConect)
	{
	VTArvores* arvs;
	VTArvore* arv1;
	VTArvore* arv2;
	bool     resposta = false;
	//------------
	//Função que retorna lista de fontes conectadas ao equipamento
	//------------
	//Cria conjunto de árvores
	arvs = NewObjArvores(eqpto);
	//Inicia arvore de caminhos - atribui
	arv1 = arvs->Arv();//= arv_ori
   arv1->IniciaArvore(eqpto);
	//Gera todas a arvore a partir de eqpto
	while(arv1 != NULL)
		{
      //Gera ramos sem retorno e
		if(lisFontes->IndexOf(arv1->Eqpto()) != -1)
        {
        if(lisConect->IndexOf(arv1->Eqpto()) == -1) lisConect->Add(arv1->Eqpto());
        resposta = true;
		  }
      GeraRamos(arv1, true);
      arv2 = arv1->ProximoRamo();
      arv1 = (arv2 != NULL) ? arv2 : BckTrk(arv1);
		}
	delete arvs;
   return(resposta);
   }
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::VerConect(VTEqpto* eqpto)
	{
	VTArvores* arvs;
	VTArvore* arv1;
   VTArvore* arv2;
   bool     resposta = false;
   //------------
   //Função que verifica existencia de fonte de suprimento conectada a eqpto
   //------------
   //Cria conjunto de árvores
	arvs = NewObjArvores(eqpto);
   //Inicia arvore de caminhos - atribui
   arv1 = arvs->Arv();//= arv_ori
	arv1->IniciaArvore(eqpto);

   //Gera todas a arvore a partir de eqpto
	while(arv1 != NULL)
      {
      //Gera ramos sem retorno e
		if(lisFontes->IndexOf(arv1->Eqpto()) != -1) {resposta = true; break;}
		GeraRamos(arv1, true);
		arv2 = arv1->ProximoRamo();
		arv1 = (arv2 != NULL) ? arv2 : BckTrk(arv1);
		}
	delete arvs;
	return(resposta);
	}
//---------------------------------------------------------------------------
*/


/*
bool __fastcall TGeraPath::BuscaExtrem(VTArvore* parv, TList* lisRamos, bool flag_fontes)
	{
	//default: flag_fontes = true
	VTArvore* arv_aux;

	lisRamos->Clear();
	arv_aux = parv;
	//Gera conjunto de arvores
	lisRamos->Add(arv_aux);
	for(int n = 0; n < lisRamos->Count; n++)
		{
		arv_aux = (VTArvore*)lisRamos->Items[n];
		lisRamos->Assign(arv_aux->ListaArvores(), laOr);
      }
	//Remove arvores com ramificações
   for(int n = lisRamos->Count - 1; n >= 0; n--)
      {
      arv_aux = (VTArvore*)lisRamos->Items[n];
		//Verifica se o equipamento não está na lista de fontes

      //Verifica se a arvore tem ramificacoes
      if((arv_aux->ListaArvores())->Count == 0)
          {
			 if(!flag_fontes) continue;
          else
              if(lisFontes->IndexOf(arv_aux->Eqpto()) == -1) continue;  //Não inclui fontes nos ramos isolados
          }
      //se flag_fontes = true Remove se for suprimento ou geração
      lisRamos->Remove(arv_aux);
      }
   //Retorna lista de ramos sem ramificação (flag_fontes = false)
   //Retorna lista de ramos não fontes e sem ramificacao(flag_fontes = true)
	return(lisRamos);
   }
//---------------------------------------------------------------------------
bool __fastcall TGeraPath::EliminaRamos(VTArvore* parv)
	{
	TList* lisRamos;
	VTArvore* arv_aux;
	VTArvore* arv_ori;
   TList* lisAux;


   //Gera o primeiro conjunto de extremidades não fontes
   lisRamos = new TList();

	//Busca extremidades
	do
	  {
	  //Retorna em lista de ramos extremos não suprimentos e não geradores
	  BuscaExtrem(parv, lisRamos);
	  //Remove ramos das arvores pai
	  for(int n = 0; n < lisRamos->Count; n++)
		  {
		  arv_aux = (VTArvore*)lisRamos->Items[n];
		  arv_ori = arv_aux->ArvOri();
		  if(arv_ori) (arv_ori->ListaArvores())->Remove(arv_aux);
		  delete arv_aux;
		  }
	  } while(lisRamos->Count > 0);

	delete lisRamos;
	return(true);
	}
//---------------------------------------------------------------------------
*/



