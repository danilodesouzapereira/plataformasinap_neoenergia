//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtMutua.h"
#include "TFormEdtMutua.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtMutua(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtMutua(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtMutua::TEdtMutua(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                     :TEdt(apl, TCursor(curMutua), ResetFocus, SetFocus, OnDone)
   {
   //cria Barra, Trecho e Mutua tempor�rios
   tmp.pbar  = DLL_NewObjBarra(patamares->NumPatamar());
   tmp.ptre  = DLL_NewObjTrecho(patamares->NumPatamar());
   tmp.mutua = DLL_NewObjMutua();
   //associa Trecho tempor�rio conectando a mesma Barra tempor�ria
   tmp.ptre->pbarra1 = tmp.pbar;
   tmp.ptre->pbarra2 = tmp.pbar;
   //associa Mutua c/ o Trecho
   tmp.mutua->ptre2   = tmp.ptre;
   tmp.mutua->postre2 = 50;
   }

//---------------------------------------------------------------------------
__fastcall TEdtMutua::~TEdtMutua (void)
   {
   //destr�i Barra e Trecho tempor�rios
   if (tmp.mutua) {delete tmp.mutua; tmp.mutua = NULL;}
   if (tmp.ptre)  {delete tmp.ptre;  tmp.ptre  = NULL;}
   if (tmp.pbar)  {delete tmp.pbar;  tmp.pbar  = NULL;}
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TEdtMutua::BuscaTrecho(void)
   {
   //vari�veis locais
   VTTrecho *ptre;
   TList    *lisEQP;

   //define a area como o ponto do evento para fazer a busca
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   //faz a busca de Trecho
   lisEQP = busca->Trecho(area);
   //verifica se encontrou umTrecho
   ptre = NULL;
   if (lisEQP->Count > 0)
      {//seleciona primeiro Trecho
      ptre = (VTTrecho*)lisEQP->First();
      }
   return(ptre);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMutua::CancelaSelecao(void)
   {
   //verifica se h� Eqpto em lisEQN
   if (lisEQN->Count)
      {//atualiza o gr�fico (apaga Eqpto novo)
      graf->Redraw(redrawEQP_NOVO, this);
      //limpa lisEQN
      lisEQN->Clear();
      }
   //verifica se h� Eqpto selecionado
   if (lisEQS->Count)
      {//atualiza o grafico com cor original
      graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
      //limpa lisEQS
      lisEQS->Clear();
      }
   //reinicia Trechos selecionado
   ptre1 = ptre2 = NULL;
   //cancela Trechos da Mutua tempor�ria
   tmp.mutua->ptre1 = NULL;
   tmp.mutua->ptre2 = tmp.ptre;
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtMutua::ConfirmaMutua(void)
   {
   //vari�veis locais
   bool          resp;
   TFormEdtMutua *form;

   //cria form p/ edi��o
   form = new TFormEdtMutua(NULL, apl, tmp.mutua);
   resp =  (form->ShowModal() == mrOk);
   //destr�i o form
   delete(form);
   return(resp);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMutua::EvtMouseDown(void)
   {
   //prote��o: verifica se existe um Trecho selecionado
   if (ptre1 == NULL) return;
   //associa Mutua c/ Trecho selecionado
   tmp.mutua->ptre1  = ptre1;
   tmp.mutua->DefinePosTre(ptre1, evt.x, evt.y);
   //redefine coordenadas do Trecho tempor�rio
   RedefineCoordenadasMutua();
   //insere Mutua na lista de eqptos novos
   lisEQN->Add(tmp.mutua);
   //atulaiza o grafico
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMutua::EvtMouseDownMove(void)
   {
   //vari�veis locais
   VTTrecho *ptre_novo;

   //prote��o: verifica se h� um Trecho selecionado
   if (ptre1 == NULL) return;
   //atualiza o grafico (apaga objetos novos)
   graf->Redraw(redrawEQP_NOVO, this);
   //muda coordenadas da Mutua
   RedefineCoordenadasMutua();
   //atualiza grafico
   graf->Redraw(redrawEQP_NOVO, this);
   //procura Trecho nas coordenadas do mouse
   if ((ptre_novo = BuscaTrecho()) != NULL)
      {//verifica se o Trecho encontrado j� est� selecionado
      if ((ptre_novo == ptre1)||(ptre_novo == ptre2)) return;
      //redefine ptre2
      ptre2 = ptre_novo;
      //atualiza gr�fico: apaga Trechos selecionadas
      graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
      //reinicia lista de Eqptos selecionados
      lisEQS->Clear();
      lisEQS->Add(ptre1);
      lisEQS->Add(ptre2);
      //atualiza gr�fico: exibe Trechos selecionados
      graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
      return;
      }
   //n�o encontrou Trecho final
   if (ptre2 == NULL) return;
   //cancela Trecho final
   ptre2 = NULL;
   //atualiza gr�fico: apaga Trechos selecionadas
   graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
   //reinicia lista de Eqptos selecionadas
   lisEQS->Clear();
   lisEQS->Add(ptre1);
   graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMutua::EvtMouseMove(void)
   {
   //vari�veis locais
   VTTrecho *ptre_novo;

   //verifica se existe Trecho na area
   if ((ptre_novo = BuscaTrecho()) == NULL)
      {//cancela eventual Trecho selecionado
      CancelaSelecao();
      return;
      }
   //verifica se o novo Trecho selecionadoj� estava selecionado
   if (ptre_novo == ptre1) return;
   //redefine Trecho selecionado
   CancelaSelecao();
   ptre1 = ptre_novo;
   //insere Trecho na lista de eqptos selecionados
   lisEQS->Add(ptre1);
   //atualiza grafico, lista de selecionados com cor especial
   graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TEdtMutua::EvtMouseUp(void)
   {
   //vari�veis locais
   VTMutua *mutua;

   //prote��o: verifica se h� dois Trechos selecionados
   if ((ptre1 == NULL)||(ptre2 == NULL))
      {
      CancelaSelecao();
      return;
      }
   //verifica se os dois Trechos t�m Vnom iguais
   if (ptre1->pbarra1->vnom != ptre2->pbarra1->vnom)
      {
      Aviso(AnsiString("Trechos com m�tua devem ter tens�es nominais iguais.\n") +
            "Trecho " + ptre1->Codigo + ": " + DoubleToStr("%4.3f kV", ptre1->pbarra1->vnom) + "\n" +
            "Trecho " + ptre2->Codigo + ": " + DoubleToStr("%4.3f kV", ptre2->pbarra1->vnom));
      CancelaSelecao();
      return;
      }
   //verifica se os dois Trechos pertencem � mesma Rede
   if (ptre1->rede != ptre2->rede)
      {
      Aviso(AnsiString("Trechos da m�tua devem pertencer a uma �nica rede.\n") +
            "Trecho " + ptre1->Codigo + ": " + ptre1->rede->Codigo + "\n" +
            "Trecho " + ptre2->Codigo + ": " + ptre2->rede->Codigo);
      CancelaSelecao();
      return;
      }
   //verifica se a Rede dos Trechos da Mutua � est�tica
   if (! ptre1->rede->Estatica)
      {
      Aviso("Trechos da m�tua devem pertencer a uma rede est�tica.");
      CancelaSelecao();
      return;
      }
   //define Trechos da Mutua tempor�ria
   tmp.mutua->DefineObjTrecho(ptre1, ptre2);
   //define posi��o de liga��o no Trecho2
   tmp.mutua->DefinePosTre(ptre2, evt.x, evt.y);
   //edita dados da Mutua
   if (ConfirmaMutua())
      {//cria uma c�pia da Mutua e insere na Rede
      mutua = tmp.mutua->Clone();
      edita->InsereMutua(mutua);
      }
   //atualiza o gr�fico
   CancelaSelecao();
   //graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMutua::RedefineCoordenadasMutua(void)
   {
   //vari�veis locais
   int x1, y1, x2, y2, dx, dy;

   //define (x2,y2) assumindo que a tecla Shift n�o est� apertada
   x2 = evt.x;
   y2 = evt.y;
   //verifica se a tecla Shift est� apertada
   if (evt.Shift.Contains(ssShift))
      {//determina coordenadas da Mutua
      tmp.mutua->CoordenadasEsquematico(x1, y1, x2, y2);
      //verifica se a reta (x1,y1) (evt.x,evt.y) � vertical ou horizontal
      dx = abs(x1 - evt.x);
      dy = abs(y1 - evt.y);
      if (dx > dy)
         {//hortizontal
         y2 = y1;
         }
      else
         {//vertical
         x2 = x1;
         }
      }
   //redefine coordenadas da Barra tempor�ria
   tmp.pbar->esq.x1 = x2;
   tmp.pbar->esq.y1 = y2;
   tmp.pbar->esq.x2 = x2;
   tmp.pbar->esq.y2 = y2;
   }

//---------------------------------------------------------------------------
//eof

