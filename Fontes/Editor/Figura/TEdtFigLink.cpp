//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TEdtFigLink.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"
#include "..\Redegraf\VTRedegraf.h"
//#include "..\..\DLL_Inc\Figura.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtFigLink  (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtFigLink(apl, TCursor(curFiltro), ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtFigLink::TEdtFigLink(VTApl *apl, TCursor cursor, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                       :TEdt(apl, cursor, ResetFocus, SetFocus, OnDone)
   {
   //obtém objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //inicia objetos
   selecao.eqpto      = NULL;
   selecao.comentario = NULL;
   //cria Barras e Trecho temporários, apenas para desenhar durante a movimentação do mouse
   tmp.barra1  = DLL_NewObjBarra(24);
   tmp.barra2  = DLL_NewObjBarra(24);
   tmp.trecho  = DLL_NewObjTrecho(24);
   //associa Trecho com as Barras
   tmp.trecho->pbarra1     = tmp.barra1;
   tmp.trecho->pbarra2     = tmp.barra2;
   tmp.trecho->esq.posbar1 = 50;
   tmp.trecho->esq.posbar2 = 50;
   }

//---------------------------------------------------------------------------                            
__fastcall TEdtFigLink::~TEdtFigLink(void)
   {
   //destrói objeto temporários
   if (tmp.trecho) {delete tmp.trecho; tmp.trecho = NULL;}
   if (tmp.barra1) {delete tmp.barra1; tmp.barra1 = NULL;}
   if (tmp.barra2) {delete tmp.barra2; tmp.barra2 = NULL;}
   //mais nada a fazer: o destrutor de VTEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TEdtFigLink::BuscaComentario(void)
   {
   //variáveis locais
   TList        *lisEQP;
   VTFiguraComentario *comentario = NULL;

   //define a area como o ponto do evento para fazer a busca
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   //faz a busca de barra
   lisEQP = busca->FigComentario(area);
   //verifica se encontrou um Comentario
   if (lisEQP->Count > 0)
	  {//seleciona primeiro Comentario
      comentario = (VTFiguraComentario*)lisEQP->First();
      // muda formato do mouse
      graf->Cursor(TCursor(curFiltro));
      }
   else
      {
      graf->Cursor(crDefault);
      }
   return(comentario);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TEdtFigLink::BuscaEqpto(void)
   {
   //variáveis locais
   VTEqpto *eqpto = NULL;
   TList   *lisEQP;

   //define a area como o ponto do evento para fazer a busca
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   //faz a busca de barra
   lisEQP = busca->Eqpto(area);
   //verifica se encontrou um Eqpto
   if (lisEQP->Count == 0)
      {
      graf->Cursor(crDefault);
      }
   else
      {//seleciona primeiro Eqpto
      eqpto = (VTEqpto*)lisEQP->First();
      // muda formato do mouse
      switch (eqpto->Tipo())
         {
         case eqptoBARRA:
            graf->Cursor(TCursor(curBarra));
            break;
         case eqptoCAPACITOR:
            graf->Cursor(TCursor(curCapacitor));
            break;
         case eqptoCARGA:
            graf->Cursor(TCursor(curCarga));
            break;
         case eqptoFILTRO:
            graf->Cursor(TCursor(curFiltro));
            break;
         case eqptoGERADOR:
            graf->Cursor(TCursor(curGerador));
            break;
         case eqptoMEDIDOR:
            graf->Cursor(TCursor(curMedidor));
            break;
         case eqptoMOTOR:
            graf->Cursor(TCursor(curMotor));
            break;
         case eqptoREATOR:
            graf->Cursor(TCursor(curReator));
            break;
         case eqptoSUPRIMENTO:
            graf->Cursor(TCursor(curSuprimento));
            break;
         case eqptoTRAFOZZ:
            graf->Cursor(TCursor(curTrafoZZ));
            break;
         case eqptoYREF:
            graf->Cursor(TCursor(curYref));
            break;
            // ligacao
         case eqptoCHAVE:
            graf->Cursor(TCursor(curChave));
            break;
         case eqptoCAPSERIE:
            graf->Cursor(TCursor(curCapserie));
            break;
         case eqptoREGULADOR:
            graf->Cursor(TCursor(curRegulador));
            break;
         case eqptoTRAFO:
            graf->Cursor(TCursor(curTrafo));
            break;
         case eqptoTRAFO3E:
            graf->Cursor(TCursor(curTrafo3E));
            break;
         case eqptoTRECHO:
            graf->Cursor(TCursor(curTrecho));
            break;
            // mútua
         case eqptoMUTUA:
            graf->Cursor(TCursor(curMutua));
            break;
            // rede ou redução
         case eqptoREDE:
            graf->Cursor(TCursor(curRede));
            break;
         case eqptoREDUCAO:
            graf->Cursor(TCursor(curRede));
            break;
         default:
            graf->Cursor(crDefault);
            break;
         }
      }
   return(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFigLink::EvtMouseDown(void)
   {
   //verifica se há um Eqpto ou Comentario apontado pelo mouse
   if ((selecao.comentario = BuscaComentario()) == NULL)
      {
      selecao.eqpto = BuscaEqpto();
      }
   //proteção
   if ((selecao.comentario == NULL) && (selecao.eqpto == NULL)) return;
   //define as coordenadas das Barras
   tmp.barra1->esq.x1 = tmp.barra1->esq.x2 = evt.x;
   tmp.barra1->esq.y1 = tmp.barra1->esq.y2 = evt.y;
   tmp.barra2->esq    = tmp.barra1->esq;
   //inserer Trecho em lisEQN e atualiza o gráfico
   lisEQN->Add(tmp.trecho);
   //insere Eqpto ou Comentario selecionado
   if (selecao.eqpto     ) lisEQN->Add(selecao.eqpto);
   //if (selecao.comentario) lisEQN->Add(selecao.comentario);
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TEdtFigLink::EvtMouseDownMove(void)
   {
   //variáveis locais
   int          x1, y1, dx, dy;
   VTEqpto      *eqpto;
   VTFiguraComentario *comentario;

   //reapresenta o grafico (apagar objetos)
   graf->Redraw(redrawEQP_NOVO, this);
   //atualiza coordenadas da Barra final do Trecho
   tmp.barra2->esq.x1 = tmp.barra2->esq.x2 = evt.x;
   tmp.barra2->esq.y1 = tmp.barra2->esq.y2 = evt.y;
   //verifica se a tecla Shift está apertada
   if (evt.Shift.Contains(ssShift))
      {//ligação vertical ou horizontal
      dx = abs(tmp.barra1->esq.x1 - evt.x);
      dy = abs(tmp.barra1->esq.y1 - evt.y);
      if (dx > dy)
         {tmp.barra2->esq.y1 = tmp.barra2->esq.y2 = tmp.barra1->esq.y1;}
      else
         {tmp.barra2->esq.x1 = tmp.barra2->esq.x2 = tmp.barra1->esq.x1;}
      }
   //verifica se foi selecionado um Eqpto
   if (selecao.comentario != NULL) eqpto      = BuscaEqpto();
   else                            comentario = BuscaComentario();
   //atualiza grafico
   lisEQN->Clear();
   lisEQN->Add(tmp.trecho);
   if (selecao.eqpto)
      {
      lisEQN->Add(selecao.eqpto);
      //if (comentario) lisEQN->Add(comentario);
      }
   else if (selecao.comentario)
      {
      //lisEQN->Add(selecao.comentario);
      if (eqpto) lisEQN->Add(eqpto);
      }
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TEdtFigLink::EvtMouseMove(void)
   {
   //variáveis locais
   VTEqpto      *eqpto;
   VTFiguraComentario *comentario;

   //verifica se há um Eqpto apontado pelo mouse
   if ((eqpto = BuscaEqpto()) == NULL)
      {//verifica se há um Comentario apontado pelo mouse
      comentario = BuscaComentario();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFigLink::EvtMouseUp(void)
   {
   //variáveis locais

   //reapresenta o grafico (apagar objetos)
   graf->Redraw(redrawEQP_NOVO, this);
   //verifica se foi selecionado um Eqpto
   if (selecao.comentario != NULL) selecao.eqpto      = BuscaEqpto();
   else                            selecao.comentario = BuscaComentario();
   //proteção
   if ((selecao.comentario != NULL) && (selecao.eqpto != NULL))
      {//insere Link
      InsereFigLink();
      //atualilza o Grafico
      lisEQS->Clear();
      lisEQS->Add(selecao.comentario);
      graf->Redraw(redrawFIG_SEL_COR_ORIG, this);
      }
   //reinicia dados
   selecao.comentario = NULL;
   selecao.eqpto      = NULL;
   lisEQS->Clear();
   lisEQN->Clear();
   //atualilza o Grafico
   //graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFigLink::InsereFigLink(void)
   {
   //variáveis locais
   VTLink *link;
   VTEqpto   *eqpto;
   VTLigacao *ligacao;
   VTEqbar   *eqbar;
   VTRede    *rede;
   TList     *lisLIG;

   //cria VTLink
   if ((link = selecao.comentario->InsereLink()) == NULL) return;
   //associa Link com Obra atual
   link->Obra = edita->Obra;
   //define Eqpto do Link
   link->Eqpto = selecao.eqpto;
   //determina Rede do Eqpto
   if (selecao.eqpto->Tipo() == eqptoREDE)
   {
		rede = (VTRede*)(selecao.eqpto);
   }
   // No caso de ser um eqpto tipo EQBAR  (Conectado em uma barra)
   else if (selecao.eqpto->TipoEqbar())
   {
	  eqbar = (VTEqbar*)(selecao.eqpto);
	  lisLIG = ((VTBarra*)eqbar->pbarra)->LisLigacao();
	  for (int nl = 0; nl < lisLIG->Count; nl++)
	  {
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			rede = ligacao->rede;
	  }
   }
   // No caso de ser um eqto Tipo ligacao (Ligado em duas barras)
   else if (selecao.eqpto->TipoLigacao())
   {
	   rede = ((VTLigacao*)(selecao.eqpto))->rede;
   }
   else if (selecao.eqpto->TipoBarra())
   {
	   lisLIG = ((VTBarra*)(selecao.eqpto))->LisLigacao();
	   for (int nl = 0; nl < lisLIG->Count; nl++)
	   {
		   ligacao = (VTLigacao*)lisLIG->Items[nl];
		   rede = ligacao->rede;
	   }
   }
   //Associa com rede
   selecao.comentario->Rede = rede;
   //define Eqpto do Link
   //link->Eqpto = selecao.eqpto;
   //define pontos de conexão do Link com Eqpto e Comentario
   link->PosCom = posAUTO;
   //define ponto de conexão com Eqpto
   if      (link->Eqpto->TipoBarra())   link->PosEqp = posEQP_XmYm;
   else if (link->Eqpto->TipoEqbar())   link->PosEqp = posEQP_X2Y2;
   else if (link->Eqpto->TipoLigacao()) link->PosEqp = posEQP_XmYm;
   else                                 link->PosEqp = posEQP_XmYm;
   //sinaliza término de operação
   Done();
   }

//---------------------------------------------------------------------------
//eof

