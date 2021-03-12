//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtAlign.h"
#include "VTEdicao.h"
#include "Marca\VTMarca.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Rede\VTLigacao.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe TEdtAlign
//---------------------------------------------------------------------------
VTEdt* NewObjEdtAlign(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtAlign(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtAlign::TEdtAlign(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
   {
   //inicia objetos
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //mais nada a fazer: o contrutor de VTEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtAlign::~TEdtAlign(void)
   {
   //nada a fazer: o destrutor de VTEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
void __fastcall TEdtAlign::Align(VTLigacao *ligacao, VTBarra *bar_move)
   {
   //variáveis locais
   int dx, dy;
   int xm, ym, x1, y1, x2, y2, x3, y3;

   //verifica se a Ligação conecta 2 ou 3 barras
   if (ligacao->pbarra3 == NULL)
      {//Ligação conecta 2 barras: deter coordenadas da marca
      ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
      if (bar_move == ligacao->pbarra2)
         {
         dx = -(x2 - x1);
         dy = -(y2 - y1);
         }
      else
         {
         dx = -(x1 - x2);
         dy = -(y1 - y2);
         }
      //verifica qual dimensão é menor
      if (abs(dy) < abs(dx)) dx = 0;
      else                   dy = 0;
      edita->MoveBarra(bar_move, dx, dy, false);
      }
   else
      {//Ligação conecta 3 barras: inicia coordenadas da marca
      //ligacao->CoordenadasEsquematico(xm, ym, x1, y1, x2, y2, x3, y3);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtAlign::AvisoEqptoBloqueado(VTEqpto *eqpto)
   {
   //variáveis locais
   AnsiString txt;
   AnsiString tipo   = eqpto->TipoAsString();
   AnsiString codigo = AnsiQuotedStr(eqpto->Codigo, '\'');

   Aviso(txt.sprintf("%s %s:\nEquipamento associado a obra posterior não pode ser alterado.",
                     tipo.c_str(), codigo.c_str()));
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TEdtAlign::BuscaEqpto(void)
   {
   //variáveis locais
   TList   *lis;
   VTEqpto *eqpto;

   //varifica se há alguma Barra nas coordenadas
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lis = busca->Eqpto(area);
   if (lis->Count == 0)
      {//não encontrou nenhum eqpto: muda formato do mouse
      graf->Cursor(crDefault);
      return(NULL);
      }
   //determina primeiro Eqpto da lista
   eqpto = (VTEqpto*)lis->First();
   //muda formato do mouse
   switch(eqpto->Tipo())
      {//barra
      case eqptoBARRA:      graf->Cursor(TCursor(curBarra));      break;
      //eqbar
      case eqptoCAPACITOR:  graf->Cursor(TCursor(curCapacitor));  break;
      case eqptoCARGA:      graf->Cursor(TCursor(curCarga));      break;
      case eqptoFILTRO:     graf->Cursor(TCursor(curFiltro));     break;
		case eqptoGERADOR:    graf->Cursor(TCursor(curGerador));    break;
		case eqptoMOTOR:    graf->Cursor(TCursor(curMotor));    break;
      case eqptoMEDIDOR:    graf->Cursor(TCursor(curMedidor));    break;
      case eqptoREATOR:     graf->Cursor(TCursor(curReator));     break;
      case eqptoSUPRIMENTO: graf->Cursor(TCursor(curSuprimento)); break;
      case eqptoTRAFOZZ:    graf->Cursor(TCursor(curTrafoZZ));    break;
      case eqptoYREF:       graf->Cursor(TCursor(curYref));       break;
      //ligacao
      case eqptoCHAVE:      graf->Cursor(TCursor(curChave));      break;
      case eqptoCAPSERIE:   graf->Cursor(TCursor(curCapserie));   break;
      case eqptoREGULADOR:  graf->Cursor(TCursor(curRegulador));  break;
      case eqptoTRAFO:      graf->Cursor(TCursor(curTrafo));      break;
      case eqptoTRAFO3E:    graf->Cursor(TCursor(curTrafo3E));    break;
      case eqptoTRECHO:     graf->Cursor(TCursor(curTrecho));     break;
      //mútua
      case eqptoMUTUA:      graf->Cursor(TCursor(curMutua));      break;
      //rede ou redução
      case eqptoREDE:       graf->Cursor(TCursor(curRede));       break;
      case eqptoREDUCAO:    graf->Cursor(TCursor(curRede));       break;
      default: break;
      }
   return(eqpto);
   }

//---------------------------------------------------------------------------
VTMarca* __fastcall TEdtAlign::BuscaMarca(void)
   {
   //variáveis locais
   VTMarca *marca;

   //atualiza area
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   if ((marca = busca->Marca(lisMAR, area)) != NULL)
      {//atualiza cursor
      graf->Cursor(TCursor(crHandPoint));
      }
   return(marca);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtAlign::EvtMouseDown(void)
   {
   //variáveis locais
   VTBarra *barra;

   //verifica se há uma Ligacao selecionada
   if (ligacao == NULL) return;
   //verifica se há uma Marca selecionada
   if (marca != NULL)
      {//determina Barra da Marca
      barra = marca->BarraDaMarca();
      //IMPORTANTE: cancela Marcas ANTES de alinhar a Ligacao
      MarcaCancela();
      //alinha a Ligacao movendo a Barra associada à Marca
      Align(ligacao, barra);
      //cancela a Ligacao
      ligacao = NULL;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtAlign::EvtMouseMove(void)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //busca Marca apontada pelo mouse
   if ((marca = BuscaMarca()) != NULL) return;
   //busca Eqpto apontado pelo mouse
   if ((eqpto = BuscaEqpto()) == NULL) return;
   //verifica se Eqpto selecionado é uma Ligacao
   if (eqpto->TipoLigacao())
      {//verifica se é a mesma ligacao já selecionada
      if (eqpto == ligacao) return;
      //cancela eventuais marcas criadas anteriormente
      MarcaCancela();
      //salva Ligacao selecionada
      ligacao = (VTLigacao*)eqpto;
      //cria Marcas para a Ligacao
      MarcaInicia(ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtAlign::MarcaCancela(void)
   {
   //verifica se há marcas
   if (lisMAR->Count > 0)
      {//atualiza gráfico
      graf->Redraw(redrawMARCA, this);
      //destrói Marcas
      LimpaTList(lisMAR);
      }
   //reinicia ponteiro
   marca = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtAlign::MarcaInicia(VTLigacao *ligacao)
   {
   //variáveis locais
   VTMarca  *marca;

   //elimina eventuais Marcas existentes
   MarcaCancela();
   //proteção: verifica existência de Eqpto bloqueado
   if (VerificaEqptoBloqueado(ligacao)) return;
   //cria Marca p/ Barras da Ligacao
   if ((marca = NewObjMarcaLiga(apl, ligacao, ligacao->pbarra1)) != NULL) lisMAR->Add(marca);
   if ((marca = NewObjMarcaLiga(apl, ligacao, ligacao->pbarra2)) != NULL) lisMAR->Add(marca);
   if (ligacao->pbarra3)
      {
      if ((marca = NewObjMarcaLiga(apl, ligacao, ligacao->pbarra3)) != NULL) lisMAR->Add(marca);
      }
   //atualiza gráfico
   graf->Redraw(redrawMARCA, this);
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtAlign::VerificaEqptoBloqueado(VTEqpto *eqpto)
   {
   //proteção
   if ((edita->Obra == NULL)||(edita->Obra->Obras == NULL)) return(false);
   if (edita->Obra->Obras->VerificaEqptoBloqueado(eqpto))
      {
      AvisoEqptoBloqueado(eqpto);
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
//eof
