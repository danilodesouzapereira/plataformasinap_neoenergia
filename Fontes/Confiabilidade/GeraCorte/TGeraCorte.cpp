//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGeraCorte.h"
#include "TCorteConf.h"
#include "..\VTDadoConfiab.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Diretorio\DirArq.h"
#include "..\Flow\VTFlow.h"
#include "..\Patamar\VTPatamar.h"
#include "..\GeraPath\VTArvore.h"
#include "..\GeraPath\VTArvores.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTramo.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Edita\VTEdita.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTGeraCorte* __fastcall NewObjGeraCorte(VTApl *apl)
	{
	return(new TGeraCorte(apl));
	}
//---------------------------------------------------------------------------
__fastcall TGeraCorte::TGeraCorte(VTApl* apl)
	{
	this->apl = apl;
	lisEQS = new TList();
	//lisCorte = new TList();
	lisC1  = new TList();
	lisC2  = new TList();
	lisC3  = new TList();
	lisCortes = new TList();
	lisExcl   = new TList();
	}
//---------------------------------------------------------------------------
__fastcall TGeraCorte::~TGeraCorte(void)
	{
	if(lisEQS) {delete lisEQS; lisEQS = NULL;}
	if(lisC1) {LimpaTList(lisC1); delete lisC1; lisC1 = NULL;}
	if(lisC2) {LimpaTList(lisC2); delete lisC2; lisC2 = NULL;}
	if(lisC3) {LimpaTList(lisC3); delete lisC3; lisC3 = NULL;}
	if(lisCortes) {LimpaTList(lisCortes); delete lisCortes; lisCortes = NULL;}
	if(lisExcl) {delete lisExcl; lisExcl = NULL;}


   }
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::AvaliaCorte1(VTEqpto* eqpto_sai, VTEqpto* eqpto, VTArvores* arvs)
   {
   VTEdita*   edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
   int iaux;
   bool diag_path;
   TList* lisPath;
   VTArvore* arv;
   VTLigacao* pliga;
   //Avalia corte parcial de primeira ordem
   if((!eqpto_sai)||(!eqpto)) return(false);

	lisEQS->Clear();
   //Elimina o equipamentos
   lisEQS->Add(eqpto_sai);
	edita->RetiraLisEqpto(lisEQS);
   //Executa fluxo de potencia
   ExecutaFluxo();
	//Testa os caminhos restantes
   lisPath = (arvs->LisPath(eqpto));
   for(int np = 0; np < lisPath->Count; np++)
      {
      arv = (VTArvore*)lisPath->Items[np];
      diag_path = true;
      while(arv != NULL)
           {
			  pliga = arv->Ligacao; //--
			  //Avalia ligação
			  if(pliga) diag_path = AvaliaLiga(pliga);
			  if(!diag_path) break;
			  arv = arv->ArvOri;   //--
           }
      //Verifica se existe um caminho não sobrecarregado
      if(diag_path) break;
      }
   //Undo do RetiraEqpto
   edita->ActionUndo->Execute();

   return(diag_path);

   }
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::AvaliaLiga(VTLigacao* pliga)
   {
   //variaveis locais
   VTTrafo   *ptrf;
   VTTrafo3E *ptrf3E;
   VTTramo   *tramo;
   VTTrecho  *ptre;
   VTChave   *pchv;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int       max_pat = patamares->NumPatamar();
   bool      resposta = true;

	//Verifica se a ligação em algum patamar tem diagnostico ruim

   //Diagnostico da ligacao
   for(int num_pat = 0; num_pat < max_pat; num_pat++)
      {
      switch (pliga->Tipo())
          {
          case eqptoTRAFO:
               ptrf = (VTTrafo*)pliga;
               if(ptrf->resflow->Diagnostico[num_pat] == diagRUIM) resposta = false;
               break;
          case eqptoTRAFO3E:
               ptrf3E = (VTTrafo3E*)pliga;
               if(ptrf3E->resflow->Diagnostico[num_pat] == diagRUIM) resposta = false;
               break;
          case eqptoTRECHO:
               ptre = (VTTrecho*)pliga;
               if(ptre->resflow->Diagnostico[num_pat] == diagRUIM) resposta = false;
          case eqptoCHAVE:
               pchv = (VTChave*)pliga;
               if(pchv->resflow->Diagnostico[num_pat] == diagRUIM) resposta = false;
          case eqptoGERADOR:
               pchv = (VTChave*)pliga;
               if(pchv->resflow->Diagnostico[num_pat] == diagRUIM) resposta = false;
               break;

          }
      if(!resposta) return(false);
		}
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::VerEqptoCorte(TList* lisCorte, VTEqpto* eqpto1, VTEqpto* eqpto2, VTEqpto* eqpto3)
   {
   TCorteConf* pcorte;
   bool flag1, flag2, flag3;
	for(int n = 0; n < lisCorte->Count; n++)
		{
		pcorte = (TCorteConf*)lisCorte->Items[n];
		flag1  = (pcorte->lisEqpto->IndexOf(eqpto1) != -1);
		if(flag1)
			{
			if(eqpto2)
			  {flag2 = (pcorte->lisEqpto->IndexOf(eqpto2) != -1);
				if(flag2)
				  {if(eqpto3)
					  {flag3 = (pcorte->lisEqpto->IndexOf(eqpto3) != -1);
						if(flag3) return(true);
					  }
					else
					  return(true);
				  }
			  }
			else return(true);
			}
		}

	return(false);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaCorteA(TList* lisArvs, int icont)
	{
	TList    *lisEqpto;
	VTArvores* arvs;
	VTEqpto  *eqpto;

	//Inicia lista de cortes
	LimpaTList(lisCortes);
	//Monta lista com equipamentos
	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		if(arvs->arvs_ori != NULL) continue;
		lisEqpto = arvs->LisEqpto();
		for(int n = 0; n < lisEqpto->Count; n++)
			{
			eqpto = (VTEqpto*)lisEqpto->Items[n];
			eqpto->Obj = NULL;
			}
		}


	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		if(arvs->arvs_ori != NULL) continue;
		ExecutaCorteA(arvs, icont);
		}

    return true;
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaCorteA(VTArvores* arvs, int icont)
	{
	TList* lisEqpto;
	TList* lisPath;
	TList* lisPathEqpto;
	TCorteConf *pcorte1, *pcorte2, *pcorte3;
	VTEqpto* eqpto;
	VTEqpto* eqpto1;
	VTEqpto* eqpto2;
	VTEqpto* eqpto3;
	bool     flag_prot;

	TList *lisPathD, *lisPathP;
	TList *lisAux1, *lisAux1P;
	TList *lisAux2;
	TList *lisAux20;
	TList *lisAux3;
	TList *lisAux30;


	//PENDENCIA INCLUIR PARAMETRO FALG_NF PARA A CONSIDERACAO OU NAO DE CHAVES NA

	lisAux1  = new TList();
	lisAux1P = new TList();
	lisAux20 = new TList();
	lisAux2  = new TList();
	lisAux30 = new TList();
	lisAux3  = new TList();
	lisEqpto = new TList();

	lisPath  = arvs->LisPath(false);//PENDENCIA CAMINHOS NA lisPath = arvs->LisPath(true);

	//Cria lista de equipamento expurgando equipamentos exluidos
	for(int n = 0; n < (arvs->LisEqpto())->Count; n++)
		{
		if(lisExcl->IndexOf((arvs->LisEqpto())->Items[n]) == -1)
			lisEqpto->Add((arvs->LisEqpto())->Items[n]);
		}


	//Cortes de primeira, segunda e terceira ordem
	for(int n1 = 0; n1 < lisEqpto->Count; n1++)
		{
		lisAux1->Clear();
		eqpto1 = (VTEqpto*)lisEqpto->Items[n1];

		//ENTRA
		lisPathD = arvs->LisPath(false, eqpto1);
		lisPathP = arvs->LisPathProt(eqpto1);

		lisAux1->Assign(lisPathP, laOr);
		lisAux1->Assign(lisPathD, laOr);

		if(lisPathD->Count == lisPath->Count)
			{flag_prot = false; //lisAux1->Assign(lisPathD, laOr);
			}
		else
			{if(lisPathP->Count == lisPath->Count)
				{flag_prot = true; //lisAux1->Assign(lisPathP, laOr);
				}
			}
		//ENTRA
		//SAI lisAux1->Assign(arvs->LisPath(false, eqpto1), laOr);
		//Verifica corte de 1a ordem
		if(lisAux1->Count == lisPath->Count)
		  {
		  //Busca o corte
		  if(eqpto1->Obj == NULL)
			 {
			 lisCortes->Add(pcorte1 = new TCorteConf(eqpto1));
			 eqpto1->Obj = (TObject*)pcorte1;
			 }
		  else
			 {pcorte1 = (TCorteConf*)(eqpto1->Obj);}
		  if(flag_prot) pcorte1->lisFontesP->Add(arvs);
		  else pcorte1->lisFontes->Add(arvs);
		  continue;
		  }
		else
		  {
		  if(icont < 1) continue;
		  for(int n2 = n1 + 1; n2 < lisEqpto->Count; n2++)
			  {
			  eqpto2 = (VTEqpto*)lisEqpto->Items[n2];
			  lisAux2->Clear();
			  lisAux20->Clear();
			  lisAux20->Assign(arvs->LisPath(false, eqpto2), laOr);
			  lisAux20->Assign(arvs->LisPathProt(eqpto2), laOr);
			  //Verifica se já é corte de primeira ordem
			  if(lisAux20->Count == lisPath->Count) continue;

			  lisAux2->Assign(lisAux20, laOr);
			  lisAux2->Assign(lisAux1, laOr);
			  if(lisAux2->Count == lisPath->Count)
				 {
				 //Busca o corte
				 if(eqpto1->Obj == NULL)
					{
					lisCortes->Add(pcorte1 = new TCorteConf(eqpto1));
					eqpto1->Obj = (TObject*)pcorte1;
					}
				 else
					{pcorte1 = (TCorteConf*)(eqpto1->Obj);}
				 pcorte2 = NULL;
				 for(int np2 = 0; np2 < pcorte1->lisCortes->Count; np2++)
					 {
					 pcorte2 = (TCorteConf*)pcorte1->lisCortes->Items[np2];
					 if(pcorte2->eqpto == eqpto2)
						  {pcorte2->lisFontes->Add(arvs);
							break;
						  }
					 pcorte2 = NULL;
					 }
				 if(pcorte2 == NULL)
					 {pcorte1->lisCortes->Add(pcorte2 = new TCorteConf(eqpto2, pcorte1));
					  pcorte2->lisFontes->Add(arvs);
					 }
				 continue;
				 }
			  else
				 {
				 if(icont < 2) continue;
				 for(int n3 = n2 + 1; n3 < lisEqpto->Count; n3++)
					 {
					 eqpto3 = (VTEqpto*)lisEqpto->Items[n3];
					 lisAux30->Clear();
					 lisAux30->Assign(arvs->LisPath(false, eqpto3), laOr);
					 lisAux30->Assign(arvs->LisPathProt(eqpto3), laOr);
					 //Verifica se já é corte de primeira ordem
					 if(lisAux30->Count == lisPath->Count) continue;

					 //Verifica segunda ordem (2 + 1) já verificado
					 //Verifica segunda ordem (3 + 1)
					 lisAux3->Clear();
					 lisAux3->Assign(lisAux30, laOr);
					 lisAux3->Assign(lisAux1, laOr);
					 if(lisAux3->Count == lisPath->Count) continue;

					 //Verifica segunda ordem (3 + 2)
					 lisAux3->Clear();
					 lisAux3->Assign(lisAux30, laOr);
					 lisAux3->Assign(lisAux20, laOr);
					 if(lisAux3->Count == lisPath->Count) continue;

					 lisAux3->Assign(lisAux1, laOr);
					 if(lisAux3->Count == lisPath->Count)
						{
						//Busca o corte
						if(eqpto1->Obj == NULL)
						  {
						  lisCortes->Add(pcorte1 = new TCorteConf(eqpto1));
						  eqpto1->Obj = (TObject*)pcorte1;
						  }
						else
						  {pcorte1 = (TCorteConf*)(eqpto1->Obj);}
						pcorte2 = NULL;
						for(int np2 = 0; np2 < pcorte1->lisCortes->Count; np2++)
							{
							pcorte2 = (TCorteConf*)pcorte1->lisCortes->Items[np2];
							if(pcorte2->eqpto == eqpto2) break;

							pcorte2 = NULL;
							}
						if(pcorte2 == NULL) pcorte1->lisCortes->Add(pcorte2 = new TCorteConf(eqpto2, pcorte1));
						pcorte3 = NULL;
						for(int np3 = 0; np3 < pcorte2->lisCortes->Count; np3++)
							{
							pcorte3 = (TCorteConf*)pcorte2->lisCortes->Items[np3];
							if(pcorte3->eqpto == eqpto3)
								{pcorte3->lisFontes->Add(arvs);
								 break;
								}
							pcorte3 = NULL;
							}
						if(pcorte3 == NULL)
							{pcorte2->lisCortes->Add(pcorte3 = new TCorteConf(eqpto3, pcorte2));
							pcorte3->lisFontes->Add(arvs);
							}

						continue;
						}
					 }
				 }

			  }
		  }
		}


	delete lisAux1;
	delete lisAux1P;
	delete lisAux2;
	delete lisAux20;
	delete lisAux3;
	delete lisAux30;
	delete lisEqpto;
	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TGeraCorte::InsereLigaExcl(TList* lis)
	{
	lisExcl->Assign(lis, laOr);
	}
//---------------------------------------------------------------------------
TList* __fastcall TGeraCorte::LisExcl(void)
	{
	return(lisExcl);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::RealocaCortes(TList* lis1, TList* lis2,
														TList* lis3, VTArvores* arvs)
	{
	VTEqpto* eqpto;
   VTEqpto* eqpto1;
   VTEqpto* eqpto2;
   VTEqpto* eqpto3;
   TCorteConf*  pcorte;
	TCorteConf*  pcorte_aux;
   bool     flag1, flag2, flag3;
   TList*   lisPath;
   TList*   lisNA;
   TList*   lis;
   TList*   lisAux;
   TList*   lisAux1;
   VTArvore* arv;


	lisAux = new TList();
   lisAux1= new TList();

   //Retira cortes simples em caminhos não conectados
   for(int n = lis1->Count - 1; n >= 0 ; n--)
      {
      pcorte = (TCorteConf*)lis1->Items[n];
		eqpto = (VTEqpto*)(pcorte->lisEqpto)->First();
      //Verifica se o equipamento pertence somente a caminho
      //ligado por chaves NA
		if(!arvs->NA(eqpto)) continue;

      //Retira o corte de primeira ordem
      lis1->Remove(pcorte);
      delete pcorte;
      }

   //Busca socorros em cortes de segunda ordem falsos
   lisAux->Clear();
   for(int n = lis2->Count - 1; n >= 0 ; n--)
      {
		pcorte = (TCorteConf*)lis2->Items[n];
		eqpto1 = (VTEqpto*)pcorte->lisEqpto->Items[0];
      eqpto2 = (VTEqpto*)pcorte->lisEqpto->Items[1];
      flag1  = arvs->NA(eqpto1);//VerChvSoc(eqpto1);
      flag2  = arvs->NA(eqpto2);//VerChvSoc(eqpto2);

      if((!flag1)&&(!flag2)) continue;


      lis2->Remove(pcorte);
      //Carga não conectada por eqpto1 e eqpto2
      if(flag1 && flag2)
        {delete pcorte;
			continue;
        }
      //Seta chave de socorro
      eqpto = (flag1) ? eqpto1 : eqpto2;

      //Busca chaves de socorro
      lisPath = arvs->LisPath(false, eqpto);
      lisNA   = ((VTArvore*)lisPath->First())->ListaNA();

      //Cortes de segunda ordem que se tornaram cortes
      //de primeira ordem
      pcorte->lisEqpto->Remove(eqpto);
      pcorte->lisChvSoc->Add(lisNA);
      for(int naux = 0; naux < lisAux->Count; naux++)
         {
         pcorte_aux = (TCorteConf*)lisAux->Items[naux];
         //Compara as listas dos equipamentos restante
         if(VerCompLis(pcorte->lisEqpto, pcorte_aux->lisEqpto))
           {
           delete pcorte;
           pcorte = NULL;
           pcorte_aux->lisChvSoc->Assign(pcorte->lisChvSoc, laOr);
           break;
			  }
         }
      if(pcorte) lisAux->Add(pcorte);
      }
	//Adiciona lista de contingencias simples derivadas de lis2
   lis1->Assign(lisAux, laOr);

   //Busca socorros em cortes de terceira ordem falsos
   //Retira cortes simples em chaves NA
   lisAux->Clear();
   for(int n = lis3->Count - 1; n >= 0 ; n--)
      {
      pcorte = (TCorteConf*)lis3->Items[n];
      eqpto1 = (VTEqpto*)pcorte->lisEqpto->Items[0];
      eqpto2 = (VTEqpto*)pcorte->lisEqpto->Items[1];
      eqpto3 = (VTEqpto*)pcorte->lisEqpto->Items[2];
      lisAux1->Clear();
	  if((flag1  = arvs->NA(eqpto1)) == true) lisAux1->Add(eqpto1);//if(flag1  = VerChvSoc(eqpto1)) lisAux1->Add(eqpto1);
	  if((flag2  = arvs->NA(eqpto2)) == true) lisAux1->Add(eqpto2);
	  if((flag3  = arvs->NA(eqpto3)) == true) lisAux1->Add(eqpto3);
      if(!(flag1 || flag2 || flag3)) continue;
      lis3->Remove(pcorte);
      if(flag1 && flag2 && flag3)
        {delete pcorte;
         continue;
        }

      for(int naux = 0; naux < lisAux1->Count; naux++)
           {
            //Busca chaves de socorro
            eqpto   = (VTEqpto*)lisAux1->Items[naux];
				lisPath = arvs->LisPath(false, eqpto);
            lisNA   = ((VTArvore*)lisPath->First())->ListaNA();

            pcorte->lisEqpto->Remove(eqpto);
            pcorte->lisChvSoc->Add(lisNA);
			  }
      for(int naux = 0; naux < lisAux->Count; naux++)
         {
         pcorte_aux = (TCorteConf*)lisAux->Items[naux];
         if(VerCompLis(pcorte->lisEqpto, pcorte_aux->lisEqpto))
           {
           delete pcorte;
           pcorte = NULL;
           pcorte_aux->lisChvSoc->Assign(pcorte->lisChvSoc, laOr);
           break;
           }
         }
      if(pcorte) lisAux->Add(pcorte);
      }

   //Adiciona lista de contingencias simples derivadas de lis2
   for(int n = 0; n < lisAux->Count; n++)
      {
      pcorte = (TCorteConf*)lisAux->Items[n];
      lis    = (pcorte->lisEqpto->Count == 1) ? lis1 : lis2;
      lis->Add(pcorte);
      }


   delete lisAux;
   delete lisAux1;
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::VerLisSoc(TList* lisSoc, VTArvores* arvs)
   {
   //variaveis locais
   VTEqpto* eqpto1;
   VTEqpto* eqpto2;
   TList*   lisPath1;
   TList*   lisPath2;
	//Verifica os caminhos supridos pelos socorros
   for(int n1 = lisSoc->Count - 1; n1 >= 0; n1--)
      {
      eqpto1 = (VTEqpto*)lisSoc->Items[n1];
      lisPath1 = arvs->LisPath(false, eqpto1);
      for(int n2 = lisSoc->Count - 1; n2 >= 0; n2--)
         {
         eqpto2 = (VTEqpto*)lisSoc->Items[n2];
         lisPath2 = arvs->LisPath(false, eqpto2);
         //Verifica se as listas tem os mesmos elementos
         if(!VerCompLis(lisPath1, lisPath2)) continue;
         lisSoc->Remove(eqpto1);
         lisSoc->Remove(eqpto2);
         break;
         }
      }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::VerCompLis(TList* lis1, TList* lis2)
   {
   //Verifica se as listas tem os mesmos objetos
   for(int n = 0; n < lis1->Count; n++)
		if(lis2->IndexOf(lis1->Items[n]) == -1) return(false);
   for(int n = 0; n < lis2->Count; n++)
		if(lis1->IndexOf(lis2->Items[n]) == -1) return(false);
   return(true);

   }
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::VerChvSoc(VTEqpto* eqpto)
   {
   VTChave* pchave;
   //Verifica se o equipamento é chave NA
   //1-Verifica se é chave
   if(eqpto->Tipo() != eqptoCHAVE) return(false);
	pchave = (VTChave*)eqpto;
   //2-Verifica se é chave NF
   if(pchave->Aberta) return(true);
   return(true);
	}
//---------------------------------------------------------------------------
/*bool __fastcall TGeraCorte::ExecutaCorteParcial(VTArvores* arvs)
   {
	TList* lisCortes;
   TCorteConf* pcorte;
   VTEqpto* eqpto1;
   VTEqpto* eqpto2;
   TList*   lisEqpto;
   int      iconta;

   lisEqpto = new TList();
   iconta = 0;
   //Executa cortes fracos de primeira eliminando contingências dos cortes de 2a ordem
   lisCortes = arvs->LisCortes(2);
   for(int n = 0; n < lisCortes->Count; n++)
      {
      pcorte = (TCorteConf*)lisCortes->Items[n];
      eqpto1 = (VTEqpto*)pcorte->lisEqpto->Items[0];
      eqpto2 = (VTEqpto*)pcorte->lisEqpto->Items[1];
      //testa saida de eqpto1 e permanencia de eqpto2
      if(lisEqpto->IndexOf(eqpto1) == -1)
        {
        iconta++;
        lisEqpto->Add(eqpto1);
        if(!AvaliaCorte1(eqpto1, eqpto2, arvs))
          {
          pcorte = new TCorteConf();
          pcorte->lisEqpto->Add(eqpto1);
          (arvs->LisCortesP())->Add(pcorte);
          }
        }
		//testa saida de eqpto2 e permanencia de eqpto1
      if(lisEqpto->IndexOf(eqpto2) == -1)
        {
        iconta++;
        lisEqpto->Add(eqpto2);
        if(!AvaliaCorte1(eqpto2, eqpto1, arvs))
          {
          pcorte = new TCorteConf();
          pcorte->lisEqpto->Add(eqpto2);
          (arvs->LisCortesP())->Add(pcorte);
          }
        }
      }
   delete lisEqpto;
   return(true);
   } */
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaFluxo(void)
   {
   bool      resposta = true;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTFlow    *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));
   int       ano      = 0;
   //Executa fluxo e diagnóstico da rede
   if(!flow->CalculaPorRede(redes, ano)) resposta = false;

   return(resposta);
   }
//---------------------------------------------------------------------------
TList* __fastcall TGeraCorte::LisCortes(void)
	{
   return(lisCortes);
   }
/*//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaCorte1(VTArvores* arvs, bool flagNF)
	{
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	VTEqpto* eqpto1;
	TList*   lisEqpto;
	TList*   lisPath;
	TList*   lisPathEqpto;
	TList*   lisCorte1;
	TCorteConf*  pcorte;
	PAR_CONF     pconf;


	//Rotina para a identificacao de cortes totais de 1a ordem
	lisPath  = arvs->LisPath(flagNF);
	lisEqpto = arvs->LisEqpto();
	lisCorte1 = arvs->LisCortes(1);
	//Limpa a lista de cortes de 1a ordem
	LimpaTList(lisCorte1);
	//Cortes de primeira, segunda e terceira ordem
	for(int n1 = 0; n1 < lisEqpto->Count; n1++)
		{
		eqpto1 = (VTEqpto*)lisEqpto->Items[n1];
		if(!dataConfiab->ParamEqpto(pconf, eqpto1)) continue;

		//Verifica se o corte é existente
		if(VerEqptoCorte(lisCorte1, eqpto1, NULL, NULL)) continue;
		//Vrifica corte de 1a ordem
		if((arvs->LisPath(flagNF, eqpto1))->Count == lisPath->Count)
		  {
		  lisCorte1->Add(pcorte = new TCorteConf());
		  pcorte->lisEqpto->Add(eqpto1);
		  continue;
		  }
		}
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaCorte2(VTArvores* arvs, bool flagNF)
	{
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
   VTEqpto* eqpto1;
   VTEqpto* eqpto2;
   TList* lisEqpto;
   TList* lisPath;
   TList* lisPathEqpto;
   TList* lisCorte1;
   TList* lisCorte2;
   TCorteConf* pcorte;
   TList*   lisAux;
   PAR_CONF pconf;
   //Rotina para a identificacao de cortes totais de 2a ordem
   //dado o cálculo prévio de cortes de primeira ordem


   lisPath = arvs->LisPath(flagNF);
   lisEqpto = arvs->LisEqpto();
   lisCorte1 = arvs->LisCortes(1);
   lisCorte2 = arvs->LisCortes(2);
   //Inicia lista de cortes de 2a ordem
   LimpaTList(lisCorte2);
   lisAux = new TList();
   //Cortes de primeira, segunda e terceira ordem
   for(int n1 = 0; n1 < lisEqpto->Count; n1++)
      {
      eqpto1 = (VTEqpto*)lisEqpto->Items[n1];
      if(!dataConfiab->ParamEqpto(pconf, eqpto1)) continue;
      if(VerEqptoCorte(lisCorte1, eqpto1, NULL, NULL)) continue;
      for(int n2 = n1 + 1; n2 < lisEqpto->Count; n2++)
         {
         eqpto2 = (VTEqpto*)lisEqpto->Items[n2];
         if(!dataConfiab->ParamEqpto(pconf, eqpto2)) continue;
         if(VerEqptoCorte(lisCorte1, eqpto2, NULL, NULL)) continue;
         if(VerEqptoCorte(lisCorte2, eqpto1, eqpto2, NULL)) continue;
         lisAux->Clear();
			lisAux->Assign(arvs->LisPath(flagNF, eqpto1), laOr);
         lisAux->Assign(arvs->LisPath(flagNF, eqpto2), laOr);
         if(lisAux->Count == lisPath->Count)
           {
           lisCorte2->Add(pcorte = new TCorteConf());
           pcorte->lisEqpto->Add(eqpto1);
           pcorte->lisEqpto->Add(eqpto2);
           continue;
           }
         }
      }
   delete lisAux;
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaCorte3(VTArvores* arvs, bool flagNF)
   {
   VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
   VTEqpto* eqpto1;
   VTEqpto* eqpto2;
   VTEqpto* eqpto3;
   TList* lisEqpto;
   TList* lisPath;
   TList* lisPathEqpto;
   TList* lisCorte1;
   TList* lisCorte2;
   TList* lisCorte3;
   TCorteConf* pcorte;
   TList*   lisAux;
   PAR_CONF pconf;
   //Rotina para a identificacao de cortes totais de 3a ordem
   //dados um conjunto de cortes de 1a e 2a ordem

   lisPath = arvs->LisPath(flagNF);
   lisEqpto  = arvs->LisEqpto();
   lisCorte1 = arvs->LisCortes(1);
	lisCorte2 = arvs->LisCortes(2);
   lisCorte3 = arvs->LisCortes(3);
   //Inicia cortes de 3a ordem
   LimpaTList(lisCorte3);
   lisAux = new TList();
   //Cortes de primeira, segunda e terceira ordem
   for(int n1 = 0; n1 < lisEqpto->Count; n1++)
      {
      eqpto1 = (VTEqpto*)lisEqpto->Items[n1];
      if(!dataConfiab->ParamEqpto(pconf, eqpto1)) continue;
      if(VerEqptoCorte(lisCorte1, eqpto1, NULL, NULL)) continue;
      for(int n2 = n1 + 1; n2 < lisEqpto->Count; n2++)
         {
         eqpto2 = (VTEqpto*)lisEqpto->Items[n2];
         if(!dataConfiab->ParamEqpto(pconf, eqpto2)) continue;
         if(VerEqptoCorte(lisCorte1, eqpto2, NULL, NULL)) continue;
         if(VerEqptoCorte(lisCorte2, eqpto1, eqpto2, NULL)) continue;
         for(int n3 = n2 + 1; n3 < lisEqpto->Count; n3++)
            {
            eqpto3 = (VTEqpto*)lisEqpto->Items[n3];
            if(!dataConfiab->ParamEqpto(pconf, eqpto3)) continue;
            if(VerEqptoCorte(lisCorte1, eqpto3, NULL, NULL)) continue;
            if(VerEqptoCorte(lisCorte2, eqpto1, eqpto3, NULL)) continue;
            if(VerEqptoCorte(lisCorte2, eqpto2, eqpto3, NULL)) continue;
            if(VerEqptoCorte(lisCorte3, eqpto1, eqpto2, eqpto3)) continue;
            lisAux->Clear();
            lisAux->Assign(arvs->LisPath(flagNF, eqpto1), laOr);
            lisAux->Assign(arvs->LisPath(flagNF, eqpto2), laOr);
            lisAux->Assign(arvs->LisPath(flagNF, eqpto3), laOr);
            if(lisAux->Count == lisPath->Count)
              {
              lisCorte3->Add(pcorte = new TCorteConf());
              pcorte->lisEqpto->Add(eqpto1);
              pcorte->lisEqpto->Add(eqpto2);
              pcorte->lisEqpto->Add(eqpto3);
              continue;
				  }
            }
         }
      }
   delete lisAux;
   return(true);
	}
*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*bool __fastcall TGeraCorte::ExecutaCorteArvores(TList* lisArvs, VTEqpto* eqpto, int icont)
	{
	VTArvores* arvs;
	VTArvore*  arv;
	VTEqpto    *eqpto_aux;
	int        iaux;
	TList *lisFontes, *lisP;
	TList *lisEqptoT, *lisEqpto, *lisLis;

	
	lisEqptoT = new TList();
	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		lisEqptoT->Assign(arvs->LisEqpto(), laOr);
		}

	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs      = (VTArvores*)lisArvs->Items[n];
		lisEqpto  = arvs->LisEqpto();

		//Busca lista de caminhos da fonte = eqpto
		lisFontes = arvs->LisFontes();
		iaux      = lisFontes->IndexOf(eqpto);
		lisLis    = arvs->LisLisEqptoP();
		lisEqptoPath = (TList*)lisLis->Items[iaux];

		for(int neq = 0; neq < lisEqptoT->Count; neq++)
			{
			eqpto_aux = (VTEqpto*)lisEqptoT->Items[neq];
			iaux = lisEqpto->IndexOf(eqpto_aux);

			}

		}

	lisP = new TList();

	lisPath = arvs->LisPath(false);
	for(int np = 0; np <lisPath->Count; np++)
		{

		}


	delete lisP;
	
	return(true);
	}  */
/*//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::AgregaCortes(TList* lisArvs)
	{
	VTArvores *arvs;
	TList* lisCorte1;
	TList* lisCorte2;
	TList* lisCorte3;
	TList* lisAux;
	TCorteConf *pcorte, *pcorte_aux;
	bool   flag_agr;


	LimpaTList(lisC1);
	LimpaTList(lisC2);
	LimpaTList(lisC3);
	lisAux = new TList();
	for(int na = 0; na < lisArvs->Count; na++)
		{
		arvs = (VTArvores*)lisArvs->Items[na];
		lisCorte1 = arvs->LisCortes(1);
		lisCorte2 = arvs->LisCortes(2);
		lisCorte3 = arvs->LisCortes(3);
		for(int n1 = 0; n1 < lisCorte1->Count; n1++) lisC1->Add(lisCorte1->Items[n1]);
		for(int n1 = 0; n1 < lisCorte2->Count; n1++) lisC2->Add(lisCorte2->Items[n1]);
		for(int n1 = 0; n1 < lisCorte3->Count; n1++) lisC3->Add(lisCorte3->Items[n1]);
		}

	lisAux->Clear();
	for(int n1 = 0; n1 < lisC1->Count; n1++)
			  {
			  pcorte = (TCorteConf*)lisC1->Items[n1];
			  if(lisAux->IndexOf(pcorte) != -1) continue;
			  for(int n2 = lisC1->Count - 1; n2 >= (n1 + 1); n2--)
				  {
				  pcorte_aux = (TCorteConf*)lisC1->Items[n2];
				  if(lisAux->IndexOf(pcorte_aux) != -1) continue;
				  if(pcorte->lisEqpto->Items[0] == pcorte_aux->lisEqpto->Items[0])
					 {
					 pcorte->lisFontes->Add(pcorte_aux->lisFontes->First());
					 lisAux->Add(pcorte_aux);
					 }
				  }
			  }
	for(int n = 0; n < lisAux->Count; n++) lisC1->Remove(lisAux->Items[n]);

	lisAux->Clear();
	for(int n1 = 0; n1 < lisC2->Count; n1++)
			  {
			  pcorte = (TCorteConf*)lisC2->Items[n1];
			  if(lisAux->IndexOf(pcorte) != -1) continue;
			  for(int n2 = lisC2->Count - 1; n2 >= (n1 + 1); n2--)
				  {
				  pcorte_aux = (TCorteConf*)lisC2->Items[n2];
				  if(lisAux->IndexOf(pcorte_aux) != -1) continue;
				  if(pcorte->lisEqpto->IndexOf(pcorte_aux->lisEqpto->Items[0]) == -1) continue;
				  if(pcorte->lisEqpto->IndexOf(pcorte_aux->lisEqpto->Items[1]) == -1) continue;

				  pcorte->lisFontes->Add(pcorte_aux->lisFontes->First());
				  lisAux->Add(pcorte_aux);
				  }
			  }
	for(int n = 0; n < lisAux->Count; n++) lisC2->Remove(lisAux->Items[n]);
	delete lisAux;
	}       */
/*//---------------------------------------------------------------------------
bool __fastcall TGeraCorte::ExecutaCorte(VTArvores* arvs, int icont)
	{
	TList* lisEqpto;
	TList* lisPath;
	TList* lisPathEqpto;
	TList* lisCorte1;
	TList* lisCorte2;
	TList* lisCorte3;
	TCorteConf* pcorte;
	VTEqpto* eqpto;
	VTEqpto* eqpto1;
	VTEqpto* eqpto2;
	VTEqpto* eqpto3;

	TList* lisAux1;
	TList* lisAux2;
	TList* lisAux20;
	TList* lisAux3;
	TList* lisAux30;

	//PENDENCIA INCLUIR PARAMETRO FALG_NF PARA A CONSIDERACAO OU NAO DE CHAVES NA

	lisAux1  = new TList();
	lisAux20 = new TList();
	lisAux2  = new TList();
	lisAux30 = new TList();
	lisAux3  = new TList();

	lisPath   = arvs->LisPath(false);//PENDENCIA CAMINHOS NA lisPath = arvs->LisPath(true);
	lisEqpto  = arvs->LisEqpto();
	lisCorte1 = arvs->LisCortes(1);
	lisCorte2 = arvs->LisCortes(2);
	lisCorte3 = arvs->LisCortes(3);

	lisCorte1->Clear();
	lisCorte2->Clear();
	lisCorte3->Clear();
	//Cortes de primeira, segunda e terceira ordem
	for(int n1 = 0; n1 < lisEqpto->Count; n1++)
		{
		lisAux1->Clear();
		eqpto1 = (VTEqpto*)lisEqpto->Items[n1];
		lisAux1->Assign(arvs->LisPath(false, eqpto1), laOr);
		//Vrifica corte de 1a ordem
		if(lisAux1->Count == lisPath->Count)
		  {
		  lisCorte1->Add(pcorte = new TCorteConf());
		  pcorte->lisEqpto->Add(eqpto1);  pcorte->lisFontes->Add(arvs->EqRaiz());
		  continue;
		  }
		else
		  {
		  if(icont < 1) continue;
		  for(int n2 = n1 + 1; n2 < lisEqpto->Count; n2++)
			  {
			  eqpto2 = (VTEqpto*)lisEqpto->Items[n2];
			  lisAux2->Clear();
			  lisAux20->Clear();
			  lisAux20->Assign(arvs->LisPath(false, eqpto2), laOr);
			  //Verifica se já é corte de primeira ordem
			  if(lisAux20->Count == lisPath->Count) continue;

			  lisAux2->Assign(lisAux20, laOr);
			  lisAux2->Assign(lisAux1, laOr);
			  if(lisAux2->Count == lisPath->Count)
				 {
				 lisCorte2->Add(pcorte = new TCorteConf());
				 pcorte->lisEqpto->Add(eqpto1);
				 pcorte->lisEqpto->Add(eqpto2); pcorte->lisFontes->Add(arvs->EqRaiz());
				 continue;
				 }
			  else
				 {
				 if(icont < 2) continue;
				 for(int n3 = n2 + 1; n3 < lisEqpto->Count; n3++)
					 {
					 eqpto3 = (VTEqpto*)lisEqpto->Items[n3];
					 lisAux30->Clear();
					 lisAux30->Assign(arvs->LisPath(false, eqpto3), laOr);

					 //Verifica se já é corte de primeira ordem
					 if(lisAux30->Count == lisPath->Count) continue;

					 //Verifica segunda ordem (2 + 1) já verificado
					 //Verifica segunda ordem (3 + 1)
					 lisAux3->Clear();
					 lisAux3->Assign(lisAux30, laOr);
					 lisAux3->Assign(lisAux1, laOr);
					 if(lisAux3->Count == lisPath->Count) continue;

					 //Verifica segunda ordem (3 + 2)
					 lisAux3->Clear();
					 lisAux3->Assign(lisAux30, laOr);
					 lisAux3->Assign(lisAux20, laOr);
					 if(lisAux3->Count == lisPath->Count) continue;

					 lisAux3->Assign(lisAux1, laOr);
					 if(lisAux3->Count == lisPath->Count)
						{
						lisCorte3->Add(pcorte = new TCorteConf());
						pcorte->lisEqpto->Add(eqpto1);
						pcorte->lisEqpto->Add(eqpto2);
						pcorte->lisEqpto->Add(eqpto3); pcorte->lisFontes->Add(arvs->EqRaiz());
						}
					 }
				 }

			  }
		  }
		}

	//Realoca caminhos com chaves de socorro
	//RealocaCortes(lisCorte1, lisCorte2, lisCorte3, arvs);



	delete lisAux1;
	delete lisAux2;
	delete lisAux20;
	delete lisAux3;
	delete lisAux30;
	return(true);
	} */

