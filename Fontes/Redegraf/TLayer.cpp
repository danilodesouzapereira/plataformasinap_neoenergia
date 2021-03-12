//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLayer.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
//#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTLayer* NewObjLayer(int tipo)
   {
   try{
      return(new TLayer(tipo));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TLayer::TLayer(int tipo)
   {
   try{//salva tipo do Layer
      PD.tipo = tipo;
      //cria objetos
      lisEQP = new TList();
      } catch (Exception &e)
         {//nada fazer
         }
   }

//---------------------------------------------------------------------------
// TLayer::~TLayer() - destrutor
//---------------------------------------------------------------------------
__fastcall TLayer::~TLayer(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }
   
//---------------------------------------------------------------------------
void __fastcall TLayer::InsereEqpto(VTEqpto *eqpto)
   {
   lisEQP->Add(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TLayer::LisBarra(TList *lisEXT)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é uma Barra
      if (eqpto->TipoBarra())
         {//insere Eqpto em lisEXT
         //lisEXT->Add(eqpto);
         if (lisEXT->IndexOf(eqpto) < 0) lisEXT->Add(eqpto);
         //atualiza Status do Eqpto: indica que deve ser desenhado
         //eqpto->Status[sttREDRAW] = true;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLayer::LisEqbar(TList *lisEXT, int tipo)
   {
   //variáveis locais
   VTBarra *barra;
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é um Eqbar
      if (! eqpto->TipoEqbar()) continue;
      //verifica se o Eqpto é um Eqbar do tipo indicado
      if ((tipo == eqptoINDEF)||(tipo == eqptoEQBAR)||(eqpto->Tipo() == tipo))
         {//insere Eqpto em lisEXT
         //lisEXT->Add(eqpto);
         if (lisEXT->IndexOf(eqpto) < 0) lisEXT->Add(eqpto);
         //atualiza Status do Eqpto: indica que deve ser desenhado
         //eqpto->Status[sttREDRAW] = true;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLayer::LisEqpto(TList *lisEXT, int tipo)
   {
   //verifica se foi definido um tipo
   if (tipo == eqptoINDEF)
      {//obtém todos os Eqptos: importante chamar as 3 funções
      LisBarra(lisEXT);
      LisEqbar(lisEXT, tipo);
      LisLigacao(lisEXT, tipo);
      return;
      }
   //verifica se tipo indica Barra, Eqbar ou Ligacao
   switch(VTEqpto::TipoEqpto_BarraEqbarLigacao(tipo))
      {
      case eqptoBARRA:    LisBarra(lisEXT);         break;
      case eqptoEQBAR:    LisEqbar(lisEXT, tipo);   break;
      case eqptoLIGACAO:  LisLigacao(lisEXT, tipo); break;
      case eqptoMUTUA:    LisMutua(lisEXT);         break;
      default:                                      break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLayer::LisLigacao(TList *lisEXT, int tipo)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é uma Ligacao
      if (! eqpto->TipoLigacao()) continue;
      //verifica se o Eqpto é uma Ligacao do tipo indicado
      if ((tipo == eqptoINDEF)||(tipo == eqptoLIGACAO)||(eqpto->Tipo() == tipo))
         {//insere Eqpto em lisEXT
         //lisEXT->Add(eqpto);
         if (lisEXT->IndexOf(eqpto) < 0) lisEXT->Add(eqpto);
         //atualiza Status do Eqpto: indica que deve ser desenhado
         //eqpto->Status[sttREDRAW] = true;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLayer::LisMutua(TList *lisEXT)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é uma Barra
      if (eqpto->Tipo() == eqptoMUTUA)
         {//insere Eqpto em lisEXT
         //lisEXT->Add(eqpto);
         if (lisEXT->IndexOf(eqpto) < 0) lisEXT->Add(eqpto);
         //atualiza Status do Eqpto: indica que deve ser desenhado
         //eqpto->Status[sttREDRAW] = true;
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TLayer::QtdeBarra(void)
   {
   //variáveis locais
   int     count = 0;
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é uma Barra
      if (eqpto->TipoBarra())  count++;
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TLayer::QtdeEqbar(int tipo)
   {
   //variáveis locais
   int     count = 0;
   VTBarra *barra;
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é um Eqbar
      if (! eqpto->TipoEqbar()) continue;
      //verifica se o Eqpto é um Eqbar do tipo indicado
      if ((tipo == eqptoINDEF)||(tipo == eqptoEQBAR)||(eqpto->Tipo() == tipo)) count++;
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TLayer::QtdeEqpto(int tipo)
   {
   //verifica se foi definido um tipo
   if (tipo == eqptoINDEF)
      {//retorna número total de Eqptos
      return(lisEQP->Count);
      }
   //verifica se tipo indica Barra, Eqbar ou Ligacao
   switch(VTEqpto::TipoEqpto_BarraEqbarLigacao(tipo))
      {
      case eqptoBARRA:    return(QtdeBarra());
      case eqptoEQBAR:    return(QtdeEqbar(tipo));
      case eqptoLIGACAO:  return(QtdeLigacao(tipo));
      case eqptoMUTUA:    return(QtdeMutua());
      default:            return(0);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TLayer::QtdeLigacao(int tipo)
   {
   //variáveis locais
   int     count = 0;
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é uma Ligacao
      if (! eqpto->TipoLigacao()) continue;
      //verifica se o Eqpto é uma Ligacao do tipo indicado
      if ((tipo == eqptoINDEF)||(tipo == eqptoLIGACAO)||(eqpto->Tipo() == tipo)) count++;
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TLayer::QtdeMutua(void)
   {
   //variáveis locais
   int     count = 0;
   VTEqpto *eqpto;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto é uma Barra
      if (eqpto->Tipo() == eqptoMUTUA) count++;
      }
   return(count);
   }

//---------------------------------------------------------------------------
//eof


