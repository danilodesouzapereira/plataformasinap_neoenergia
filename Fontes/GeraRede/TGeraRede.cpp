//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGeraRede.h"
#include "TEqptoLiga.h"
#include "TLigaSerie.h"
#include "..\Apl\VTApl.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Bloco\VTElo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Const.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Diretorio\DirArq.h"
#include "..\Diretorio\VTPath.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\TLigacao.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTGeraRede* __fastcall NewObjGeraRede(VTApl *apl)
   {
   return(new TGeraRede(apl));
   }
//---------------------------------------------------------------------------
__fastcall TGeraRede::TGeraRede(VTApl* apl)
   {
   this->apl = apl;
   lisEqLiga= new TList();
   lisLigaS  = new TList();
   lisAux    = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TGeraRede::~TGeraRede(void)
	{
	if(lisAux) {delete lisAux; lisAux = NULL;}
	if(lisEqLiga) {LimpaTList(lisEqLiga); delete lisEqLiga; lisEqLiga = NULL;}
	if(lisLigaS) {LimpaTList(lisLigaS); delete lisLigaS; lisLigaS = NULL;}
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraRede::Executa(TList* lisLiga, TList *lisFora)
	{
	//variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTReducao *reducao;
	VTLigacao*  plig;
	VTLigacao*  plig0;
	VTLigacao*  plig1;
	TLigaSerie* pligS;
	//TLigaS*     pligg;
	VTEqpto*    eqpto;
	TEqptoLiga* eqliga;
	VTConsulta* consulta;
	int iaux;
	TList* lisBarra;
	TList* lisFontes;
	TList* lisCargas;
	TList* lisBarRedu;
	TList* lisRedu;
	TList* lisTMP;
	TList* lisEqpto;
	VTBarra *pbarra0;
	VTBarra* pbarra1;
	AnsiString aux;
	bool     flag_filtro;

	//Inicia lista
	//lisBarra  = new TList();
	lisFontes = new TList();
	lisCargas = new TList();
	lisRedu   = new TList();
	lisBarRedu= new TList();

	//Inicia Listas
	LimpaTList(lisLigaS);
	lisLiga->Clear();

	int parou;// mudar atributo aberto ou fechado
	//Cria objeto consulta
	consulta = DLL_NewObjConsulta(apl);
	//Inicia lista de suprimentos e geradores
	lisTMP   = consulta->LisEqptoRede(redes, eqptoGERADOR);
	for(int n = 0; n < lisTMP->Count; n++) {lisFontes->Add(((VTEqbar*)lisTMP->Items[n])->pbarra);}
	lisTMP   = consulta->LisEqptoRede(redes, eqptoSUPRIMENTO);
	for(int n = 0; n < lisTMP->Count; n++) {lisFontes->Add(((VTEqbar*)lisTMP->Items[n])->pbarra);}
	//PENDENCIA REDES REDUZIDAS COM SUPRIMENTO

	lisTMP = consulta->LisEqptoRede(redes, eqptoCARGA);
	for(int n = 0; n < lisTMP->Count; n++) {lisCargas->Add(((VTCarga*)lisTMP->Items[n])->pbarra);}

	//Cria lista de barras do corte de reducoes
	redes->LisReducao(lisRedu);
	for(int nr = 0; nr < lisRedu->Count; nr++)
      {
		reducao = (VTReducao*)lisRedu->Items[nr];
		lisBarRedu->Assign(reducao->Corte->LisBarra(), laOr);
      }

	//lisAux->Clear();
	//lisAux->Assign(consulta->LisEqptoRede(redes, eqptoGERADOR), laOr);
	//lisAux->Assign(consulta->LisEqptoRede(redes, eqptoSUPRIMENTO), laOr);
	//for(int n = 0; n < lisAux->Count; n++)
	//	lisFontes->Add(((VTEqbar*)lisAux->Items[n])->pbarra);
	lisBarra = redes->LisBarra();
	for(int n = 0; n < lisBarra->Count; n++) {((VTEqpto*)lisBarra->Items[n])->Obj = NULL;}
	//Cria lista de ligações série inicial
	redes->LisLigacao(lisLiga);
	//Cria lista de barras e ligacoes/equipamento
	for(int nl = 0; nl < lisLiga->Count; nl++)
		{
		plig = (VTLigacao*)lisLiga->Items[nl];
		lisAux->Clear();
		//Percorre as barras da ligacao
		plig->LisBarra(lisAux);
		for(int nb = 0; nb < lisAux->Count; nb++)
			{
			eqpto = (VTEqpto*)lisAux->Items[nb];
			//Verifica se a barra liga fonte ou carga
			if(lisCargas->IndexOf(eqpto) != -1) continue;
			if(lisFontes->IndexOf(eqpto) != -1) continue;
			if(lisBarRedu->IndexOf(eqpto) != -1)continue;

			//if((iaux = lisBarra->IndexOf(eqpto)) == -1)
			if(eqpto->Obj == NULL)
			  {
			  eqliga = new TEqptoLiga(eqpto);
			  lisEqLiga->Add(eqliga);
			  eqpto->Obj = eqliga;
			  //lisBarra->Add(eqpto);
			  }
			else
			  {
			  eqliga = (TEqptoLiga*)eqpto->Obj;
			  //eqliga = (TEqptoLiga*)lisEqLiga->Items[iaux];
			  }
			eqliga->AddLiga(plig, lisAux->Count - 1);
			}
		}

	//Elimina ligacoes/equipamentos ligacoes com 3 enrolamentos e ligacoes de lisFora
	for(int n = lisEqLiga->Count - 1; n >= 0 ; n--)
		{
		eqliga = (TEqptoLiga*)lisEqLiga->Items[n];
		for(int nl = 0; nl < eqliga->lisLiga->Count; nl++)
			{
			plig = (VTLigacao*)eqliga->lisLiga->Items[nl];//hk12 plig = (VTLigacao*)lisLiga->Items[nl];
			if(lisFora->IndexOf(plig) != -1) {lisEqLiga->Remove(eqliga);
														 delete eqliga;
														 break;
														}
			lisAux->Clear();
			plig->LisBarra(lisAux);
			if(lisAux->Count > 2) {lisEqLiga->Remove(eqliga);
										  delete eqliga;
										  break;
										 }
			}
		}

	//Elimina ligações em paralelo
	for(int n = lisEqLiga->Count - 1; n >= 0 ; n--)
		{
		eqliga = (TEqptoLiga*)lisEqLiga->Items[n];
		//if(FiltroEq(eqliga);//, lisFontes, lisCargas))//hk12, lisLiga))
		if(eqliga->lisLiga->Count != 2)
		  {
		  //Remove eqliga
		  lisEqLiga->Remove(eqliga);
		  delete eqliga;
		  continue;
		  }

		plig0 = (VTLigacao*)eqliga->lisLiga->Items[0];
		plig1 = (VTLigacao*)eqliga->lisLiga->Items[1];
		lisAux->Clear();
		plig0->LisBarra(lisAux);
		if((lisAux->IndexOf(plig1->pbarra1) != -1)&&(lisAux->IndexOf(plig1->pbarra2) != -1))
			 {
			 lisEqLiga->Remove(eqliga);
			 delete eqliga;
			 continue;
			 }
		}

	//Faz a fusão de ligacoes em série de duplas
	lisLigaS->Clear();
	while(lisEqLiga->Count > 0)
		{
		for(int n = lisEqLiga->Count - 1; n >=0; n--)
			{
			eqliga = (TEqptoLiga*)lisEqLiga->Items[n];
			//if(FiltroEq(eqliga);//, lisFontes, lisCargas))//hk12 , lisLiga))
			if(eqliga->lisLiga->Count != 2)
				{//Remove eqliga com mais de 2 ligacoes
				lisEqLiga->Remove(eqliga);
				delete eqliga;
				continue;
				}
			//Cria nova ligacao em serie
			pligS = new TLigaSerie();
			//Define as barras extremas
			plig0 = (VTLigacao*)eqliga->lisLiga->Items[0];
			plig1 = (VTLigacao*)eqliga->lisLiga->Items[1];

			lisAux->Clear();
			plig0->LisBarra(lisAux);
			plig1->LisBarra(lisAux);
			lisAux->Remove(eqliga->eqpto);
			pbarra0 = (VTBarra*)lisAux->Items[0];
			pbarra1 = (VTBarra*)lisAux->Items[1];
			pligS->DefineObjBarra(pbarra0, pbarra1);
			aux.sprintf("[%s %s]", pbarra0->Codigo.c_str(), pbarra1->Codigo.c_str());
			pligS->Codigo = aux;

			if(lisLigaS->IndexOf(plig0) == -1)
				pligS->AddEqpto(plig0);
			else
				{
				lisEqpto = ((TLigaSerie*)plig0)->LisEqpto();
				for(int ns = 0; ns < lisEqpto->Count; ns++)
					 pligS->AddEqpto((VTEqpto*)lisEqpto->Items[ns]);
				}

			if(lisLigaS->IndexOf(plig1) == -1)
				pligS->AddEqpto(plig1);
			else
				{
				lisEqpto = ((TLigaSerie*)plig1)->LisEqpto();
				for(int ns = 0; ns < lisEqpto->Count; ns++)
					pligS->AddEqpto((VTEqpto*)lisEqpto->Items[ns]);
				}

			pligS->AddEqpto(eqliga->eqpto);

			//Remove eqliga
			lisEqLiga->Remove(eqliga);
			delete eqliga;
			//Remove as ligacoes da lista de ligacoes e adiciona a nova ligaSerie
			lisLiga->Remove(plig0);
			lisLiga->Remove(plig1);
			lisLiga->Add(pligS);
			lisLigaS->Add(pligS);

			//Atualiza ligações em lisEqLiga
			for(int neq = 0; neq < lisEqLiga->Count; neq++)
				{
				eqliga = (TEqptoLiga*)lisEqLiga->Items[neq];
				if(eqliga->lisLiga->IndexOf(plig0) != -1)
					{
					eqliga->lisLiga->Remove(plig0);
					eqliga->lisLiga->Add(pligS);
					}
				if(eqliga->lisLiga->IndexOf(plig1) != -1)
					{
					eqliga->lisLiga->Remove(plig1);
					eqliga->lisLiga->Add(pligS);
					}
				}
			}
		}

	//destrói objetos
	delete consulta;
	//delete lisBarra;
	delete lisFontes;
	delete lisCargas;
	delete lisBarRedu;
	delete lisRedu;

	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraRede::ExecutaBlocos(TList* lisLiga, VTBlocos* blocos)
	{
	//VTBlocos *blocos = this->Blocos;
	VTBloco  *bloco;
	VTBarra  *pbarBloco;
	VTEqbar  *eqptoBar;
	TList    *lisBlocos;
	TList    *lisEqBar, *lisElos, *lisBarBloco, *lisLigaBloco, *lisEqBarBloco;
	VTLigacao *plig;
	TLigaSerie *pligS;
	VTElo      *elo;
	AnsiString aux;


	//Inicia Listas
	LimpaTList(lisLigaS);
	lisLiga->Clear();

	lisElos = new TList();
	lisEqBarBloco = new TList();
	lisLigaBloco  = new TList();
	lisBlocos = blocos->LisBloco();
	//Gera uma ligacao serie por bloco
	for(int nb = 0; nb < lisBlocos->Count; nb++)
		{
		bloco = (VTBloco*)lisBlocos->Items[nb];
		//Atualiza lista de ligacoes/bloco
		lisLigaBloco->Clear();
		//Adiciona ligacoes do bloco a pligS
		bloco->LisLigacao(lisLigaBloco);

		if(lisLigaBloco->Count == 0) continue;


		//lisLigaBloco->Clear();
		//bloco->LisLigacao(lisLigaBloco);
		lisElos->Clear();
		blocos->LisElo(bloco, lisElos);
		if(lisElos->Count == 0) continue;
		lisBarBloco = bloco->LisBarra();
		//Cria nova ligacao em serie
		lisLigaS->Add(pligS = new TLigaSerie());
		aux = "";
		lisLiga->Add(pligS);
		plig = NULL;
		for(int nl = 0; nl < lisLigaBloco->Count; nl++)
			{
			plig = (VTLigacao*)lisLigaBloco->Items[nl];
			pligS->AddEqpto(plig);
			}
		if(plig) pligS->rede = plig->rede;

		for(int nl = 0; nl < lisElos->Count; nl++)
			{
			elo = (VTElo*)lisElos->Items[nl];
			if(lisLiga->IndexOf(elo->Chave) == -1) lisLiga->Add(elo->Chave);
			pbarBloco = (lisBarBloco->IndexOf(elo->Chave->pbarra1) == -1) ? elo->Chave->pbarra2 : elo->Chave->pbarra1;
			pligS->DefineObjBarraSerie(pbarBloco);
			aux.cat_sprintf("%s_", pbarBloco->Codigo.c_str());
			}
		pligS->Codigo = aux;
		//Adiciona barras de carga e suprimento
		lisEqBarBloco->Clear();
		bloco->LisEqbar(lisEqBarBloco);
		for(int ncarga = 0; ncarga <lisEqBarBloco->Count; ncarga++)
			{
			eqptoBar = (VTEqbar*)lisEqBarBloco->Items[ncarga];
			pligS->DefineObjBarraSerie(eqptoBar->pbarra);
			}
		}
	delete lisElos;
	delete lisEqBarBloco;
	delete lisLigaBloco;
	return(true);
	}
//---------------------------------------------------------------------------
/*
bool __fastcall TGeraRede::FiltroEq(TEqptoLiga* eqliga)//, TList* lisFontes, TList* lisCargas)//hk12, TList* lisLiga)
	{
	VTLigacao* plig;

	//if(lisFontes->IndexOf(eqliga->eqpto) != -1)
	//	return(true);
	//if(lisCargas->IndexOf(eqliga->eqpto) != -1)
	//	 return(true);


	//Elimina ligacoes/equipamentos com número de ligações diferente de duas
	if(eqliga->lisLiga->Count != 2) return(true);

	return(false);
	}
//--------------------------------------------------------------------------
*/
