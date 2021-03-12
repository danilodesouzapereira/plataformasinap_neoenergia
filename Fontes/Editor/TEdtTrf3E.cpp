//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TEdtTrf3E.h"
#include "TFormEdtTrf3E.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtTrf3E(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtTrf3E(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrf3E::TEdtTrf3E(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                     :TEdt(apl, TCursor(curTrafo3E), ResetFocus, SetFocus, OnDone)
   {
   //cria objeto TBarra auxiliar
   tmp.pbar  = DLL_NewObjBarra(patamares->NumPatamar());
   //define estado inicial do editor
   estado = estLig1Bar1;
   }

//---------------------------------------------------------------------------                            
__fastcall TEdtTrf3E::~TEdtTrf3E(void)
   {
   //destrói objetos
   if (tmp.pbar) {delete tmp.pbar; tmp.pbar = NULL;}
   if (ptrf)     {delete ptrf;     ptrf     = NULL;}
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEdtTrf3E::BuscaBarra(void)
   {
   //variáveis locais
   VTBarra *pbar;
   TList *lisBAR;

   //define a area como o ponto do evento para fazer a busca
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   //faz a busca de barra
   lisBAR = busca->Barra(area);
   //verifica se encontrou uma Barra
   pbar = NULL;
   if (lisBAR->Count > 0)
      {//seleciona primeira Barra
      pbar = (VTBarra*)lisBAR->First();
      }
   return(pbar);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::CancelaSelecao(void)
   {
   //atualiza o gráfico
   if (lisEQS->Count) graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
   if (lisEQN->Count) graf->Redraw(redrawEQP_NOVO, this);
   //limpa listas
   lisEQS->Clear();
   lisEQN->Clear();
   //reinicia Trafo3E
   if (ptrf != NULL)
      {
      ptrf->pbarra1 = NULL;
      ptrf->pbarra2 = NULL;
      ptrf->pbarra3 = NULL;
      }
   //reinicia ponteiro auxiliar
   pbar1 = NULL;
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtTrf3E::ConfirmaLigacao(void)
   {
   //variáveis locais
   bool resp;
   TFormEdtTrf3E *FormEdtTrf3E;

   FormEdtTrf3E = new TFormEdtTrf3E(NULL, apl, ptrf);
   FormEdtTrf3E->OpcaoRetira(false);
   //exibe o form
   resp = (FormEdtTrf3E->ShowModal() == mrOk);
   delete FormEdtTrf3E;
   return(resp);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::CriaLigacao(void)
   {
   //verifica se existe Trafo3E
   if (ptrf != NULL)return;
   //cria objeto Trafo3E
   if ((ptrf  = DLL_NewObjTrafo3E(patamares->NumPatamar())) == NULL) return;
   //inicia ID
   ptrf->Id = FalsoId();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::EvtMouseDown(void)
   {
   //verifica se há uma barra selecionada
   if (pbar1 == NULL)
      {
      CancelaSelecao();
      return;
      }
   //cria objeto TRafo3E
   CriaLigacao();
   //define Barra selecionada com Barra1 da Ligacao
   ptrf->pbarra1 = pbar1;
   //define Barras termporária como Barra2 da Ligacao
   ptrf->pbarra2 = tmp.pbar;
   ptrf->pbarra3 = NULL;
   //define posição da Ligação na Barra1
   ptrf->DefinePosBar(pbar1, evt.x, evt.y);
   //redefine coordenadas da Barra temporária
   tmp.pbar->esq.x1 = tmp.pbar->esq.x2 = evt.x;
   tmp.pbar->esq.y1 = tmp.pbar->esq.y2 = evt.y;
   //atualiza o grafico
   graf->Redraw(redrawEQP_NOVO, this);
   lisEQN->Clear();
   lisEQN->Add(ptrf);
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16835731
void __fastcall TEdtTrf3E::EvtMouseDownMove(void)
   {
   //variáveis locais
   VTBarra *pbarsel;
   int     x1, y1, dx, dy;

   //proteção
   if(ptrf == NULL) return;
   //proteção: verifica se a Ligacao tem pelo menos 2 barra definidas
   if ((ptrf->pbarra1 == NULL)||(ptrf->pbarra2 == NULL)) return;
   //atualiza o grafico (apaga objetos novos)
   graf->Redraw(redrawEQP_NOVO, this);
   //muda coordenadas da barra temporária
   tmp.pbar->esq.x1 = tmp.pbar->esq.x2 = evt.x;
   tmp.pbar->esq.y1 = tmp.pbar->esq.y2 = evt.y;
   //verifica se a tecla Shift está apertada
   if (evt.Shift.Contains(ssShift))
      {//determina coordenadas da Ligação na barra1
      ptrf->pbarra1->PontoConexao(ptrf->esq.posbar1, x1, y1);
      //ligação vertical ou horizontal
      dx = abs(x1 - evt.x);
      dy = abs(y1 - evt.y);
      if (dx > dy)
         {tmp.pbar->esq.y1 = tmp.pbar->esq.y2 = y1;}
      else
         {tmp.pbar->esq.x1 = tmp.pbar->esq.x2 = x1;}
      }
   //atualiza grafico (exibe objetos novos)
   graf->Redraw(redrawEQP_NOVO, this);
   //procura barra final na coordenada
   pbarsel = BuscaBarra();
   //verifica se é a mesma Barra selecionada anteriormente
   if (pbarsel == pbar1) return;
   //atualiza Barra selecionada
   if ((pbar1 = pbarsel) == NULL)
      {//atualiza gráfico
      graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
      //reinicia lista de Barras selecionadas
      lisEQS->Clear();
      if ((ptrf->pbarra1 != NULL)&&(ptrf->pbarra1 != tmp.pbar)) lisEQS->Add(ptrf->pbarra1);
      if ((ptrf->pbarra2 != NULL)&&(ptrf->pbarra2 != tmp.pbar)) lisEQS->Add(ptrf->pbarra2);
      //if ((ptrf->pbarra3 != NULL)&&(ptrf->pbarra3 != tmp.pbar)) lisEQS->Add(ptrf->pbarra2);
      graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
      return;
      }
   //verifica se está sendo selecionada a segunda Barra da ligacao
   if (ptrf->pbarra2 == tmp.pbar)
      {//verifica se a Barra selecionada é igual a Barra 1 do Trafo
      if (ptrf->pbarra2 == pbarsel) return;
      //atualiza grafico (apaga objetos novos)
      graf->Redraw(redrawEQP_NOVO, this);
      //redefine segunda Barra da Ligacao
      ptrf->pbarra2 = pbarsel;
      //define posição da Ligação na Barra2
      ptrf->DefinePosBar(ptrf->pbarra2, evt.x, evt.y);
      //redefine terceira Barra da Ligacao c/ a Barra temporária
      ptrf->pbarra3     = tmp.pbar;
      ptrf->esq.posbar3 = 50;
      //atualiza grafico (exibe objetos novos)
      graf->Redraw(redrawEQP_NOVO, this);
      }
   else
      {//está sendo selecionada a terceira Barra da ligacao
      //verifica se a Barra selecionada é igual a Barra 1 ou Barra 2 do Trafo
      if ((ptrf->pbarra1 == pbarsel)||(ptrf->pbarra2 == pbarsel)) return;
      //atualiza gráfico: apaga Barras selecionadas
      graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
      //reinicia lista de barras selecionadas
      lisEQS->Clear();
      lisEQS->Add(ptrf->pbarra1);
      lisEQS->Add(ptrf->pbarra2);
      lisEQS->Add(pbarsel);
      //atualiza gráfico: exibe Barras selecionadas
      graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::EvtMouseMove(void)
   {
   //variáveis locais
   VTBarra *pbar_nova;

   //verifica se tem barra na area
   if ((pbar_nova = BuscaBarra()) == NULL)
      {//cancela eventual seleção anterior
      CancelaSelecao();
      return;
      }
   // se a barra selecionada é igual à atual
   if (pbar_nova == pbar1) return;
   //reinicia gráfico
   CancelaSelecao();
   //redefine selecionada
   pbar1 = pbar_nova;
   // insere a barra na lista de eqptos selecionados
   lisEQS->Add(pbar1);
   // atualiza grafico, lista de selecionados com cor especial
   graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::EvtMouseUp(void)
   {
   //variáveis locais
   VTBarra *pbarsel;

   //procura barra final na coordenada
   pbarsel = BuscaBarra();
   //verifica se há uma Barra selecionada
   if (pbarsel == NULL)
      {
      CancelaSelecao();
      return;
      }
   //verifica se as duas primeiras barras da ligacao foram definidas
   if ((ptrf->pbarra1 == NULL)||(ptrf->pbarra2 == NULL))
      {
      CancelaSelecao();
      return;
      }
   //seleção da terceira Barra da Ligacao: verifica se a Barra selecionada coincide c/ a Barra1 ou c/ a Barra2 da Ligacao
   if ((pbarsel == ptrf->pbarra1)||(pbarsel == ptrf->pbarra2))
      {
      CancelaSelecao();
      return;
      }
   //atualiza gráfico
   graf->Redraw(redrawEQP_NOVO, this);
   //redefine Barra3 da Ligacao
   ptrf->pbarra3 = pbarsel;
   //define posição da Ligação na Barra3
   ptrf->DefinePosBar(ptrf->pbarra3, evt.x, evt.y);
   //atualiza gráfico
   graf->Redraw(redrawEQP_NOVO, this);
   /*
   //valida Barras
   if ((ptrf->pbarra1->vnom == ptrf->pbarra2->vnom)&&
       (ptrf->pbarra1->vnom == ptrf->pbarra3->vnom))
      {
      Erro ("Barras c/ tensões nominais iguais!");
      //cancela seleção
      CancelaSelecao();
      return;
      }
   */
   //edita os dados do trafo e confirma inserção
   if (ConfirmaLigacao())
      {//usuário confirmou inserção
      InsereLigacao();
      }
   else
      {//usuário cancelou inserção
      CancelaSelecao();
      if (ptrf) delete ptrf;
      }
   //reinicia ponteiro p/ trafo
   ptrf = NULL;
   //atualiza o gráfico
   graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::InsereLigacao(void)
   {
   //insere a Ligacao
   edita->InsereLigacao(ptrf, ptrf->rede);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrf3E::OrdenaBarras(void)
   {
   //variáveis locais
   VTBarra *pbar;
   int     posbar;

   if (ptrf->pbarra1->vnom < ptrf->pbarra2->vnom)
      {//salva informações da Barra1
      pbar   = ptrf->pbarra1;
      posbar = ptrf->esq.posbar1;
      //redefine Barra1 c/ dados da Barra2
      ptrf->pbarra1     = ptrf->pbarra2;
      ptrf->esq.posbar1 = ptrf->esq.posbar2;
      //redefine Barra2 c/ dados da Barra1
      ptrf->pbarra2     = pbar;
      ptrf->esq.posbar2 = posbar;
      }
   }

//---------------------------------------------------------------------------
//eof

