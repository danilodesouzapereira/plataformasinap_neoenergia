//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <math.h>
//#include <math.hpp>
#include "VTCustos.h"
#include "VTCustoEqpto.h"
#include "VTCustoNodal.h"
#include "VTParcela.h"
#include "TCoefNodal.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\FluxoCC\TBar.h"
#include "..\FluxoCC\TLig.h"
#include "..\FluxoCC\VTData.h"
#include "..\FluxoCC\VTMontaData.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;
/*
//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjCoefNodal(VTApl *apl_owner)
   {
   return(new TCoefNodal(apl_owner));
   }
*/
//---------------------------------------------------------------------------
__fastcall TCoefNodal::TCoefNodal(VTApl *apl_owner)
   {
   //salva objetos
   this->apl = apl_owner;
   //cria objetos
   data   = NewObjData(apl);
   monta  = NewObjMontaData(apl);
   }

//---------------------------------------------------------------------------
__fastcall TCoefNodal::~TCoefNodal(void)
   {
   //destrói objetos
   if (data)    {delete data;   data   = NULL;}
   if (monta)   {delete monta;  monta  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::CalculaCoeficientes(VTCustoNodal *custo_nodal, double *vetorX, int dim_x)
   {
   //variáveis locais
   int       i, j, k;
   double    Cij_k, Bij, Xik, Xjk;
   TLig      *lig;
   VTParcela *parcela;
   TList     *lisPARCELA = custo_nodal->LisParcela();

   //loop p/ todas as Ligacoes de CustoNodal
   for (int k = 0; k < lisPARCELA->Count; k++)
      {
      parcela = (VTParcela*)lisPARCELA->Items[k];
      lig     = ExisteLig(data->LisLigacao(), parcela->custo_eqpto->eqpto);
      //determina índices das Bar da Lig em Bnodal
      if ((i = lig->bar1->index) >= dim_x) return(false);
      if ((j = lig->bar2->index) >= dim_x) return(false);
      //determina valores
      Xik = vetorX[i];
      Xjk = vetorX[j];
/*
      bnodal2->Elemento_LC(i,j, &Bij);
      Cij_k =  -Bij * (Xik - Xjk);  // HPS 24.05.2007
*/
      Cij_k =  lig->bser * (Xik - Xjk);
      //salva Cij_k como coeficiente da Parcela
      parcela->coef = Cij_k;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCoefNodal::CfgAvisoErro(bool enabled)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::Executa(VTNet *net, double Sbase,
                                    AnsiString arqRelFlow, AnsiString arqRelRede)

   {
   //variáveis locais
   bool sucesso;
   int  ind_pat = 0;
   int  num_bar;

   //reinicia objeto Data p/ o cálculo
   if (! monta->Inicia(net, data, ind_pat)) return(false);
   //insere Eqptos que interessam p/ cálculo de coeficiente nodal
   if (! monta->InsereBarra())      return(false);
   if (! monta->InsereLigacao())    return(false);
   if (! monta->InsereGerador())    return(false);
   if (! monta->InsereSuprimento()) return(false);
   //define uma única Bar swing no final da lista de Bar
   PreparaBarSwing();
   //define índices de Bar que serão usados na montagem da matriz bnodal
   PreparaIndiceBar();
   //determina número de Barras da  rede
   num_bar = data->NumBar;
   try{//cria objetos usados no cálculo
      col_matZ = new double[num_bar];
      bnodal   = DLL_NewObjSisLinD((num_bar-1), num_bar);
      if (bnodal == NULL) return(false);
      //executa cálculo dos coeficientes
      sucesso = ExecutaCalculo();
      //destrói objetos
      delete[] col_matZ;
      delete bnodal;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::ExecutaCalculo(void)
   {
   //variáveis locais
   TBar         *bar;
   VTEqbar      *eqbar;
   VTCustoNodal *custo_nodal;
   VTParcela    *parcela;
   VTCustos     *custos = (VTCustos*)apl->GetObject(__classid(VTCustos));
   TList        *lisCUSTO_NOD = custos->ListaCustoNodal();

   //monta e triangulariza bnodal
   if (! MontaTriangularizaBnodal()) return(false);
   //calcula coeficientes de cada CustoNodal
   for (int n = 0; n < lisCUSTO_NOD->Count; n++)
      {
      custo_nodal = (VTCustoNodal*)lisCUSTO_NOD->Items[n];
      eqbar       = (VTEqbar*)custo_nodal->eqpto;
      if ((bar    = ExisteBar(data->LisBarra(), eqbar->pbarra)) == NULL) return(false);
      //zera vetor que irá receber os valores da coluna da matriz Z
      for (int i = 0; i < data->NumBar; i++) col_matZ[i] = 0.;
      //não calcula coluna da matriz Z p/ a barra swing
      if (bar->tipo != barSWING)
         {//calcula coluna da Z
         if (! bnodal->ColunaInversa(bar->index, col_matZ)) return(false);
         }
      //calcula coeficientes do custo nodal
      if (! CalculaCoeficientes(custo_nodal, col_matZ, data->NumBar)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
TBar* __fastcall TCoefNodal::ExisteBar(TList *lisBAR, void *eqpto)
   {
   //variáveis locais
   TBar *bar;

   //procura Bar em lisBAR c/ o Id indicado
   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar*)lisBAR->Items[n];
      if (bar->eqpto == eqpto) return(bar);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TBar* __fastcall TCoefNodal::ExisteBarDoTipo(TList *lisBAR, int tipo)
   {
   //variáveis locais
   TBar  *bar;

   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar*)lisBAR->Items[n];
      if (bar->tipo == tipo) return(bar);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TLig* __fastcall TCoefNodal::ExisteLig(TList *lisLIG, void *eqpto)
   {
   //variáveis locais
   TLig *lig;

   //procura Lig em lisLIG c/ as mesmas barras
   for (int n = 0; n < lisLIG->Count; n++)
      {
      lig = (TLig*)lisLIG->Items[n];
      if (lig->eqpto == eqpto) return(lig);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::MontaTriangularizaBnodal(void)
   {
   //variáveis locais
   TLig   *lig;
   TList  *lisLIG = data->LisLigacao();
   VTPath *path = (VTPath *) apl->GetObject(__classid(VTPath));
   AnsiString arqtemp = path->DirTmp() + "\\Bnodal.txt";

   //insere Lig em bnodal
   for (int n = 0; n < lisLIG->Count; n++)
      {
      lig = (TLig*)lisLIG->Items[n];
      bnodal->InsereQuatro(lig->bar1->index, lig->bar2->index, lig->bser);
      }
   if (bnodal->Erro != slSemErro) return(false);
   // bnodal->Imprime(arqtemp);
   //executa triangularização
   bnodal->Triang();
   if (bnodal->Erro != slSemErro) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::PM_GetEquilibrado(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCoefNodal::PreparaBarSwing(void)
   {
   //variáveis locais
   TBar  *bar;
   TList *lisBAR = data->LisBarra();

   //verifica se existe uma Bar swing
   if ((bar = ExisteBarDoTipo(lisBAR, barSWING)) != NULL)
      {//transfere Bar p/ o fim de lisBAR
      if (bar != lisBAR->Last())
         {
         lisBAR->Remove(bar);
         lisBAR->Add(bar);
         }
      }
   else
      {//assume última Bar como swing
      bar = (TBar*)lisBAR->Last();
      bar->tipo = barSWING;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCoefNodal::PreparaIndiceBar(void)
   {
   //variáveis locais
   TBar  *bar;
   TList *lisBAR = data->LisBarra();

   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar*)lisBAR->Items[n];
      bar->index = n;
      }
   }
//---------------------------------------------------------------------------
AnsiString __fastcall TCoefNodal::Modelo(void)
   {
   return("Corrente contínuca");
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::UtilizaBarraPV(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TCoefNodal::ValidaAderencia(VTNet *net, VTLog *plog)
	{
	return(true);
	}
	
//---------------------------------------------------------------------------
//eof
