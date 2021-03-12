//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TEdtEqbar.h"
#include "..\Busca\VTBusca.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TEdtEqbar::TEdtEqbar(VTApl *apl, TCursor cursor, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                     :TEdt(apl, cursor, ResetFocus, SetFocus, OnDone)
   {
   pbarra = NULL;
   estado = SelecaoBarra;
   }

//---------------------------------------------------------------------------
__fastcall TEdtEqbar::~TEdtEqbar(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::CancelaSelecao(void)
   {
   //verifica se há Eqptos selecionados
	if(lisEQN->Count == 0) return;
	//apaga eqptos em edição
	graf->Redraw(redrawEQP_NOVO, this);
   //limpa lista de eqptos selecionados
   lisEQN->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::EvtMouseDown(void)
   {
   //proteção: verifica estado
   if (estado != SelecaoBarra) return;
   //procura barra na posicao
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lisBAR = busca->Barra(area);
   if(lisBAR->Count == 0) return;
   //seleciona primeira Barra da lista
   pbarra = (VTBarra*)lisBAR->First();
   //cria Eqbar e associa c/ sua Barra
   CriaEqbar();
   eqbar->Id = FalsoId();
   eqbar->DefineObjBarra(pbarra);
   //calcula posição/distância do Eqbar na Barra
	eqbar->DefinePosicao(evt.x, evt.y);
	//atualiza gráfico
	CancelaSelecao();
	//acrescenta eqptos à lista lisEQN
	lisEQN->Add(pbarra);
	lisEQN->Add(eqbar);
	//atualiza gráfico
	graf->Redraw(redrawEQP_NOVO, this);
   //atualiza estado do Editor
   estado = PosicaoRelativa;
	}
   
//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::EvtMouseDownMove(void)
   {
   if(estado == PosicaoRelativa)
      {//apaga desenho do eqpto novo
      graf->Redraw(redrawEQP_NOVO, this);
      //define nova posição do eqpto novo
      eqbar->DefinePosicao(evt.x, evt.y);
      //desenha eqpto novo
      graf->Redraw(redrawEQP_NOVO, this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::EvtMouseMove(void)
   {
   //verifica o estado do Editor
   if(estado != SelecaoBarra) return;
   //procura barra na posicao (x,y)
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lisBAR = busca->Barra(area);
   if (lisBAR->Count == 0)
      {//cancela Eqptos selecionados
      CancelaSelecao();
      return;
      }
   //seleciona primeira Barra da lista
   pbarra = (VTBarra*)lisBAR->First();
   //verifica se a Barra já está na lista de eqptos selecionados
   if(lisEQN->IndexOf(pbarra) >= 0) return;
   //cancela Eqptos selecionados
   CancelaSelecao();
   //insere a barra na lisEQN
   lisEQN->Add(pbarra);
   //exibe barra em cor especial
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::EvtMouseUp(void)
   {
   if (estado == PosicaoRelativa)
      {//pede p/ usuário editar dados do Eqbar e confirmar inserção
      if (ConfirmaEqbar())
         {//usuário confirmou inserção: insere Eqbar
         InsereEqbar();
         //sinaliza término de operação
         Done();
         }
      else
         {//usuário cancelou inserção
         NaoInsereEqbar();
         }
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TEdtEqbar::InsereEqbar(void)
   {
   //insere Eqbar na sua Barra
   edita->InsereEqbar(eqbar, pbarra);
   //reinicia estado do Editor
   ReiniciaEditor();
   //atualiza gráfico p/ exibir eqpto novo incluído   (a linha estava comentada por causa da tarefa 14422280, mas trouxe efeito colateral)
   graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::NaoInsereEqbar(void)
	{
	//atualiza gráfico: apagar Eqptos novos e Eqptos selecionados
	graf->Redraw(redrawEQP_NOVO, this);
	//destrói objetos
	if (eqbar)  {delete eqbar;  eqbar  = NULL;}
	//reinicia estado do Editor
	ReiniciaEditor();
	}

//---------------------------------------------------------------------------
void __fastcall TEdtEqbar::ReiniciaEditor(void)
   {
   //limpa listas
	lisEQN->Clear();
	lisEQS->Clear();
	//reinicia ponteiros
   pbarra = NULL;
   eqbar  = NULL;
   estado = SelecaoBarra;
   }

//---------------------------------------------------------------------------

 
