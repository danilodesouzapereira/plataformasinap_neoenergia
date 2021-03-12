//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCustoNodal.h"
#include "VTCustoEqpto.h"
#include "VTParcela.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCustoNodal* __fastcall NewObjCustoNodal(void)
   {
   try{
      return(new TCustoNodal());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TCustoNodal::TCustoNodal(void)
   {
   try{//cria lista
      lisPARCELA = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TCustoNodal::~TCustoNodal(void)
   {
   //destrói lista e seus objetos lisPONTO
   if (lisPARCELA) {LimpaTList(lisPARCELA); delete lisPARCELA; lisPARCELA = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TCustoNodal::InsereParcela(double coef, VTCustoEqpto *custo_eqpto)
   {
   //variáveis locais
   VTParcela *parcela;

   //cria um novo objeto Custo e insere em lisPARCELA
   if ((parcela = NewObjParcela()) == NULL) return(false);
   lisPARCELA->Add(parcela);
   //inicia dados da Parcela
   parcela->coef        = coef;
   parcela->custo_eqpto = custo_eqpto;
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCustoNodal::LisParcela(void)
   {
   return(lisPARCELA);
   }

//---------------------------------------------------------------------------
double __fastcall TCustoNodal::PM_GetValor(int ind_pat)
   {
   //variáveis locais
   double    cargto;
   double    temp, valor = 0.;
   VTLigacao *ligacao;
   VTParcela *parcela;

   for (int n = 0; n < lisPARCELA->Count; n++)
      {
      parcela  = (VTParcela*)lisPARCELA->Items[n];
      ligacao  = (VTLigacao*)parcela->custo_eqpto->eqpto;
      //determina cargto e define seu sinal em função do sentido do fluxo
      cargto   = ligacao->resflow->Cargto_pu[ind_pat];
      if (ligacao->resflow->SentidoP[0][ind_pat] == fluxoNEG)
         {//define valor negativo p/ o cargto
         cargto = -cargto;
         }
      temp     = parcela->coef * parcela->custo_eqpto->custo * cargto;
      valor   += temp;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
//eof
