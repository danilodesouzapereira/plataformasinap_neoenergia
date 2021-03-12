//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormDebug.h"
#include "TAplDebug.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Explorer\VTAtributo.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Zona\VTZona.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormDebug::TFormDebug(TForm* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //insere Form no parent (se for definido)
   if (parent) Parent = parent;
   //cria objetos necessários ao Explorer
   apl      = new TAplDebug(this, apl_owner);
   //obtém objetos armazenados em TApl
   atributo = (VTAtributo*)apl->GetObject(__classid(VTAtributo));
   consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   //cria Areas
   area_esq = DLL_NewObjArea();
   area_utm = DLL_NewObjArea();
   //cria lista de Eqptos selecionados
   lisEQP   = new TList();
   lisREDU  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFormDebug::~TFormDebug(void)
   {
   //apaga molduras
   GraficoMoldura();
   //cancela destaque de Eqptos
   DestacaLigacao();
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisREDU) {delete lisREDU; lisREDU = NULL;}
   //destrói objetos
   if (area_esq) {delete area_esq; area_esq = NULL;}
   if (area_utm) {delete area_utm; area_utm = NULL;}
   if (apl)      {delete apl;      apl      = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionBlocosExecute(TObject *Sender)
   {
   //variáveis locais
   VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //força ButBlocos p/ baixo
   ButBlocos->Down = true;
   //reinicia Blocos
   blocos->Executa(redes);
   //reinicia TreeView
   TreeViewOrdenaBloco();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionEliminaBlocosNaoEnergizadosExecute(TObject *Sender)
   {
   //variáveis locais
   VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

   blocos->EliminaBlocosNaoEnergizados();
   //reinicia TView
   TreeViewOrdenaBloco();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionEliminaElosBlocosIguaisExecute(TObject *Sender)
   {
   //variáveis locais
   VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

   blocos->EliminaElosComBlocosIguais();
   //reinicia TView
   TreeViewOrdenaBloco();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionEliminaElosComChaveAbertaExecute(TObject *Sender)
   {
   //variáveis locais
   VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

   blocos->EliminaElosComChaveAberta();
   //reinicia TView
   TreeViewOrdenaBloco();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionOrdenaPorCorteExecute(TObject *Sender)
   {
   //força butOrdenaCorte p/ baixo
   butOrdenaPorCorte->Down = true;
   //reinicia TreeView
   TreeViewOrdenaPorCorte();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionOrdenaPorMRedeExecute(TObject *Sender)
   {
   //força butOrdenaCorte p/ baixo
   butOrdenaPorMRede->Down = true;
   //reinicia TreeView
   TreeViewOrdenaPorMRede();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionOrdenaPorRedeExecute(TObject *Sender)
   {
   //força butOrdenaCorte p/ baixo
   butOrdenaPorRede->Down = true;
   //reinicia TreeView
   TreeViewOrdenaPorRede();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::ActionZonasExecute(TObject *Sender)
   {
   //variáveis locais
   VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));

   //força butOrdenaCorte p/ baixo
   ButZona->Down = true;
   //reinicia Zonas
   zonas->ChaveAbertaEnabled = CheckBoxChaveAberta->Checked;
   zonas->AvisoEnabled       = true;
   zonas->Executa();
   //reinicia TreeView
   TreeViewOrdenaZona();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::CalculaAreaEsq(TList *lisBAR)
   {
   //variáveis locais
   int      x1, y1, x2, y2;
   int      xa, ya, xb, yb;
   VTBarra *barra;

   //inicia coordenadas c/ valor 0
   x1 = x2 = y1 = y2 =  0;
   //verifica se rede vazia
   if (lisBAR->Count > 0)
      {//calcula área definida pelas Barras
      barra = (VTBarra*)lisBAR->First();
      barra->AreaEsquematico(x1, y1, x2, y2);
      for (int n = 1; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         barra->AreaEsquematico(xa, ya, xb, yb);
         x1 = Min(x1, xa);
         y1 = Min(y1, ya);
         x2 = Max(x2, xb);
         y2 = Max(y2, yb);
         }
      }
   //define coordenadas da Area esquematica
   area_esq->x1 = x1;
   area_esq->x2 = x2;
   area_esq->y1 = y1;
   area_esq->y2 = y2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::CalculaAreaUtm(TList *lisBAR)
   {
   //variáveis locais
   bool     first = true;
   int      x1, y1, x2, y2;
   int      x, y;
   VTBarra *barra;

   //inicia coordenadas c/ valor 0
   x1 = x2 = y1 = y2 = -1;
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //obtém coordenadas da Barra em cm
      if (! barra->CoordenadasUtm_cm(x, y)) continue;
      if (first)
         {//inicia área c/ as coordenadas da Barra
         x1 = x2 = x;
         y1 = y2 = y;
         first   = false;
         }
      else
         {//calcula área
         x1 = Min(x1, x);
         y1 = Min(y1, y);
         x2 = Max(x2, x);
         y2 = Max(y2, y);
         }
      }
   //define coordenadas da Area utm
   area_utm->x1 = x1;
   area_utm->x2 = x2;
   area_utm->y1 = y1;
   area_utm->y2 = y2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::DestacaLigacao(TList *lisLIG)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->DestacaEqpto(lisLIG, clLime, 2);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::GraficoMoldura(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se foi definido um Eqpto
   if (eqpto == NULL)
      {//apaga moldura
      grafico->Moldura();
      return;
      }
   //verifica o tipo do Eqpto
   if      (eqpto->TipoBarra())   grafico->Moldura(eqpto);
   else if (eqpto->TipoEqbar())   grafico->Moldura(eqpto);
   else if (eqpto->TipoLigacao()) grafico->Moldura(eqpto);
   else switch(eqpto->Tipo())
      {
      case eqptoCORTE:
         grafico->Moldura(((VTCorte*)eqpto)->LisBarra());
         break;
      default: //apga moldura
         grafico->Moldura();
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::GraficoMoldura(TList *lisEQP)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->Moldura(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::GraficoZoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::GraficoZoom(TList *lisBAR)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //calcula Area definida pelas Barras
   CalculaAreaEsq(lisBAR);
   CalculaAreaUtm(lisBAR);
   grafico->SelecionaZoom(area_utm, area_esq);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói este Forme
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::SGridAtualiza(VTEqpto *eqpto)
   {
   //variáveis locais
   int        row;
   int        num_atr;
   AnsiString nome, valor;

   //proteção
   if (eqpto == NULL)
      {//esconde o Sgrid
      gboxEqpto->Visible = false;
      return;
      }
   //exibe GBoxEqpto
   gboxEqpto->Visible = true;
   //atualiza Caption do GBoxEqpto
   gboxEqpto->Caption = eqpto->TipoAsString() + ": " + eqpto->Codigo;
   //obtém os atributos do Eqpto
   atributo->Executa(eqpto);
   //for (int nc = 0; nc < sgrid->ColCount; nc++) sgrid->Cols[nc]->Clear();
   //verifica se há atributos a serem mostrados no SGrid
   if ((num_atr = atributo->Count()) == 0)
      {//esconde o Sgrid
      gboxEqpto->Visible = false;
      return;
      }
   //exibe SGrid
   gboxEqpto->Visible = true;
   //preenche os títulos das duas colunas
   sgrid->Cells[0][0] = "Atributo";
   sgrid->Cells[1][0] = "Valor";
   //preenche o SGrid c/ atributos do Eqpto
   row = 1;
   for (int n = 0; n < num_atr; n++)
      {
      atributo->NomeValor(n, nome, valor);
      //verifica se valores em branco
      if (nome.IsEmpty() && valor.IsEmpty()) continue;
      sgrid->Cells[0][row]   = nome;
      sgrid->Cells[1][row++] = valor;
      }
   //redefine número de linhas do SGrid
   sgrid->RowCount = row;
   //seleciona primeira celula
   sgrid->Row = 1; sgrid->Col = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::SGridAtualizaBloco(VTBloco *bloco)
   {
   //variáveis locais
   TList     *lisBAR, *lisLIG;
   VTBlocos  *blocos   = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

   //exibe GBoxEqpto
   gboxEqpto->Visible = true;
   //atualiza Caption do GBoxEqpto
   gboxEqpto->Caption = "Bloco";
   //preenche os títulos das duas colunas
   sgrid->Cells[0][0] = "Eqpto";
   sgrid->Cells[1][0] = "Qtde";
   //preenche os títulos das duas colunas
   sgrid->Cells[0][1] = "Rede";
   if (bloco->Rede == NULL) sgrid->Cells[1][1] = "-";
   else                     sgrid->Cells[1][1] = bloco->Rede->Codigo;
   //número de Barras
   lisBAR = bloco->LisBarra();
   sgrid->Cells[0][2] = "Barra";
   sgrid->Cells[1][2] = IntToStr(lisBAR->Count);
   //número de Ligacoes
   lisLIG = bloco->LisLigacao();
   sgrid->Cells[0][3] = "Ligação";
   sgrid->Cells[1][3] = IntToStr(lisLIG->Count);
   //número de Chaves
   lisEQP->Clear();
   bloco->LisLigacao(lisEQP, eqptoCHAVE);
   sgrid->Cells[0][4] = "Chave";
   sgrid->Cells[1][4] = IntToStr(lisEQP->Count);
   //número de Cargas
   lisEQP->Clear();
   bloco->LisEqbar(lisEQP, eqptoCARGA);
   sgrid->Cells[0][5] = "Carga";
   sgrid->Cells[1][5] = IntToStr(lisEQP->Count);
   //número de Blocos vizinhos
   lisEQP->Clear();
   blocos->LisBlocoVizinho(bloco, lisEQP);
   sgrid->Cells[0][6] = "Bloco viz.";
   sgrid->Cells[1][6] = IntToStr(lisEQP->Count);
   //número de Elos internos
   lisEQP->Clear();
   blocos->LisEloInterno(bloco, lisEQP);
   sgrid->Cells[0][7] = "Elos int.";
   sgrid->Cells[1][7] = IntToStr(lisEQP->Count);
   //redefine número de linhas do SGrid
   sgrid->RowCount = 8;
   //seleciona primeira celula
   sgrid->Row = 1; sgrid->Col = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::SGridAtualizaZona(VTZona *zona)
   {
   //variáveis locais
   AnsiString txt;
   int        row;
   double     v_kv, ang_grau;
   VTFases    *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //proteção
   if (zona == NULL)
      {//esconde o Sgrid
      gboxEqpto->Visible = false;
      return;
      }
   //exibe GBoxEqpto
   gboxEqpto->Visible = true;
   //atualiza Caption do GBoxEqpto
   gboxEqpto->Caption = "Zona";
   //preenche os títulos das duas colunas
   sgrid->Cells[0][0] = "Atributo";
   sgrid->Cells[1][0] = "Valor";
   //tipo
   row = 1;
   switch (zona->Tipo)
      {
      case zonaTRIFASICA:     txt = "Trifásica";     break;
      case zonaMONOFASICA:    txt = "Monofásica";    break;
      case zonaDELTA_ABERTO:  txt = "Delta Aberto";  break;
      case zonaDELTA_FECHADO: txt = "Delta Fechado"; break;
      default:                txt = "Indefinida";    break;
      }
   sgrid->Cells[0][row]   = "Tipo";
   sgrid->Cells[1][row++] = txt;
   if (zona->Tipo != zonaINDEFINIDA)
      {//defasagem
      sgrid->Cells[0][row]   = "Def.(grau)";
      sgrid->Cells[1][row++] = DoubleToStr("%2.1f", zona->Defasagem_grau);
      //fases
      sgrid->Cells[0][row]   = "Fases";
      sgrid->Cells[1][row++] = (fases->TagToFase[zona->Fases])->codigo;
      //tensão de linha
      sgrid->Cells[0][row]   = "Vff (kV)";
      sgrid->Cells[1][row++] = DoubleToStr("%4.3f", zona->Vff_kv);
      //tensão da fase A
      if (zona->Fases & faseA)
         {
         sgrid->Cells[0][row]   = "Van (kV)";
         sgrid->Cells[1][row++] = txt.sprintf("%4.3f [%4.0f]", v_kv = zona->Vfn_kv[indFASE_A] , ang_grau = zona->T_grau[indFASE_A]);
         }
      //tensão da fase B
      if (zona->Fases & faseB)
         {
         sgrid->Cells[0][row]   = "Vbn (kV)";
         sgrid->Cells[1][row++] = txt.sprintf("%4.3f [%4.0f]", v_kv = zona->Vfn_kv[indFASE_B], ang_grau = zona->T_grau[indFASE_B]);
         }
      //tensão da fase C
      if (zona->Fases & faseC)
         {
         sgrid->Cells[0][row]   = "Vcn (kV)";
         sgrid->Cells[1][row++] = txt.sprintf("%4.3f [%4.0f]", v_kv = zona->Vfn_kv[indFASE_C], ang_grau = zona->T_grau[indFASE_C]);
         }
      }
   //número de Barras
   sgrid->Cells[0][row]   = "Barras";
   sgrid->Cells[1][row++] = IntToStr((zona->LisBarra())->Count);
   //define número de linhas do SGrid
   sgrid->RowCount = row;
   //seleciona primeira celula
   sgrid->Row = 1; sgrid->Col = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   TList   *lisLIG;
   VTBloco *bloco;
   VTZona  *zona;

   //verifica se está exibindo Zona
   if (ButZona->Down)
      {//verifica TreeNode selecionado
      switch(TView->Selected->Level)
         {
         case 0: //apaga conteúdo do SGrid
            SGridAtualiza(NULL);
            break;
         case 1:  //Zona
            zona  = (VTZona*)TView->Selected->Data;
            SGridAtualizaZona(zona);
            //destaca todas as Barras
            GraficoMoldura(zona->LisBarra());
            break;
         case 2:  //Barra da Zona:
            SGridAtualiza((VTEqpto*)Node->Data);
            //exibe Moldura
            GraficoMoldura((VTEqpto*)Node->Data);
            break;
         }
      }
   else if (ButBlocos->Down)
      {//verifica TreeNode selecionado
      switch(TView->Selected->Level)
         {
         case 0: //apaga conteúdo do SGrid
            SGridAtualiza(NULL);
            break;
         case 1:  //Bloco
            bloco  = (VTBloco*)TView->Selected->Data;
            SGridAtualizaBloco(bloco);
            //verifica se o Bloco possui Ligacoes
            lisLIG = bloco->LisLigacao();
            if (lisLIG->Count == 0)
               {//cancela destaque de Ligacoes do Bloco
               DestacaLigacao();
               //destaca Barra única do Bloco
               GraficoMoldura(bloco->LisBarra());
               }
            else
               {//cancela destaque de Barras do Bloco
               GraficoMoldura();
               //destaca Ligacoes do Bloco
               DestacaLigacao(lisLIG);
               }
            break;
         case 2:  //Ligacao do Bloco:
            SGridAtualiza((VTEqpto*)Node->Data);
            //exibe Moldura
            GraficoMoldura((VTEqpto*)Node->Data);
            break;
         }
      }
   else
      {//exibe dados do Eqpto do TreeNode
      SGridAtualiza((VTEqpto*)Node->Data);
      //exibe Moldura
      GraficoMoldura((VTEqpto*)Node->Data);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TViewDblClick(TObject *Sender)
   {
   //variáveis locais
   TList     *lisLIG;
   VTEqpto   *eqpto;
   VTBloco   *bloco;
   VTZona    *zona;
   TTreeNode *Node = TView->Selected;

   //proteção
   if (Node == NULL) return;
   //verifica se está exibindo Zona
   if (ButZona->Down)
      {//verifica TreeNode selecionado
      switch(Node->Level)
         {
         case 0: //nada a fazer
            break;
         case 1:  //Zona
            zona  = (VTZona*)Node->Data;
            GraficoZoom(zona->LisBarra());
            break;
         case 2:  //Barra da Zona:
            eqpto = (VTEqpto*)Node->Data;
            //exibe Moldura
            GraficoZoom(eqpto);
            break;
         }
      }
   else if (ButBlocos->Down)
      {//verifica TreeNode selecionado
      switch(Node->Level)
         {
         case 0: //nada a fazer
            break;
         case 1:  //Bloco
            bloco  = (VTBloco*)Node->Data;
            GraficoZoom(bloco->LisBarra());
            break;
         case 2:  //Ligacao do Bloco:
            eqpto = (VTEqpto*)Node->Data;
            //exibe Moldura
            GraficoZoom(eqpto);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereBarraDoCorte(VTCorte *corte, TTreeNode *node_corte)
   {
   //variáveis locais
   TTreeNode *node_barra;
   VTBarra   *pbarra;
   TList     *lisBAR = corte->LisBarra();

   //insere TreeNode c/ título "Barras" do Corte
   node_barra = TView->Items->AddChild(node_corte, "Barras");
   //insere um TreeNode p/ cada Barra do Corte
   for (int n = 0; n < lisBAR->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR->Items[n];
      TView->Items->AddChildObject(node_barra, pbarra->Codigo, pbarra);
      }
   //ordena alfabeticamente Nodes de Barra dentro do TreeNode do Corte
   node_barra->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereCorte(TTreeNode *node_pai)
   {
   //variáveis locais
   int        num_bar, num_rede;
   AnsiString txt;
   VTCorte   *corte;
   TTreeNode *node_corte;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisCORTE = redes->LisCorte();

   //insere TreeNodes p/ Cortes
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      num_bar  = (corte->LisBarra())->Count;
      num_rede = (corte->LisRede())->Count;
      txt.sprintf("%s(b=%d)(r=%d)", corte->Codigo.c_str(), num_bar, num_rede);
      //insere TreeNode do Corte
      node_corte = TView->Items->AddChildObject(node_pai, txt, corte);
      //insere TreeNodes p/ as Barras do Corte
      TreeViewInsereBarraDoCorte(corte, node_corte);
      //insere TreeNodes p/ as Redes do Corte
      TreeViewInsereRedeDoCorte(corte, node_corte);
      }
   //ordena alfabeticamente Nodes de Cortes
   node_pai->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereCorteDaMRede(VTMRede *mrede, TTreeNode *node_rede)
   {
   //variáveis locais
   TTreeNode *node_corte, *node_reducao;
   VTCorte   *corte;
   TList     *lisCORTE = mrede->LisCorte();

   //insere um TreeNode p/ cada Corte conectado pela Rede
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      node_corte = TView->Items->AddChildObject(node_rede, corte->Codigo, corte);
      //insere TreeNodes c/ Barras do Corte
      TreeViewInsereBarraDoCorte(corte, node_corte);
      /*
      //insere TreeNode c/ a palavra "Eqptos.Redução"
      node_reducao = TView->Items->AddChild(node_corte, "Eqptos.Redução");
      //insere TreeNode c/ Eqptos da Reducao
      TreeViewInsereEqptoDaReducao(reducao, node_reducao);
      */
      }
   //ordena alfabeticamente Nodes de Cortes dentro do TreeNode de Rede
   node_rede->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereCorteDaRede(VTRede *rede, TTreeNode *node_rede)
   {
   //variáveis locais
   TTreeNode *node_corte, *node_reducao;
   VTCorte   *corte;
   TList     *lisCORTE = rede->LisCorte();

   //insere um TreeNode p/ cada Corte conectado pela Rede
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      node_corte = TView->Items->AddChildObject(node_rede, corte->Codigo, corte);
      //insere TreeNodes c/ Barras do Corte
      TreeViewInsereBarraDoCorte(corte, node_corte);
      /*
      //insere TreeNode c/ a palavra "Eqptos.Redução"
      node_reducao = TView->Items->AddChild(node_corte, "Eqptos.Redução");
      //insere TreeNode c/ Eqptos da Reducao
      TreeViewInsereEqptoDaReducao(reducao, node_reducao);
      */
      }
   //ordena alfabeticamente Nodes de Cortes dentro do TreeNode de Rede
   node_rede->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereEqptoDaReducao(VTReducao *reducao, TTreeNode *node_reducao)
   {
   //variáveis locais
   VTEqpto   *eqpto;
   TTreeNode *node_tipo;
   struct   strEQP{
            int   tipo;
            char *nome;
            } eqp[] = {{eqptoCARGA,      "Carga"},
                       {eqptoSUPRIMENTO, "Suprimento"},
                       {eqptoTRAFO,      "Transformador"},
                       {eqptoTRECHO,     "Trecho"}};
   //loop p/ tipos de Eqptos
   for (int nt = 0; nt < sizeof(eqp)/sizeof(strEQP); nt++)
      {//obtém lista de Epqtos do tipo
      lisEQP->Clear();
      reducao->LisEqpto(lisEQP, eqp[nt].tipo);
      if (lisEQP->Count == 0) continue;
      //cria TreeNode p/ tipo do Eqpto
      node_tipo = TView->Items->AddChild(node_reducao, eqp[nt].nome);
      //insere um TreeNode p/ cada Eqpto
      for (int n = 0; n < lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         TView->Items->AddChildObject(node_tipo, eqpto->Codigo, eqpto);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereMRede(TTreeNode *node_pai)
   {
   //variáveis locais
   int        num_rede, num_corte;
   AnsiString txt;
   VTMRede   *mrede;
   TTreeNode *node_redes, *node_cortes;
   TTreeNode *node_mrede;
   VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisMREDE = redes->LisMRede();

   //insere TreeNodes p/ Cortes
   for (int n = 0; n < lisMREDE->Count; n++)
      {
      mrede = (VTMRede*)lisMREDE->Items[n];
      //insere TreeNode da MRede
      node_mrede = TView->Items->AddChildObject(node_pai, mrede->Codigo, mrede);
      //insere TreeNode p/ Redes da MRede
      num_rede = (mrede->LisRede())->Count;
      txt.sprintf( "Redes(%d)",num_rede);
      node_redes  = TView->Items->AddChild(node_mrede, txt);
      //insere TreeNode p/ Cortes da MRede
      num_corte = (mrede->LisCorte())->Count;
      txt.sprintf( "Cortes(%d)",num_corte);
      node_cortes = TView->Items->AddChild(node_mrede, txt);
      //insere TreeNodes p/ as Redes da MRede
      TreeViewInsereRedeDaMRede(mrede, node_redes);
      //insere TreeNodes p/ os Cortes da MRede
      TreeViewInsereCorteDaMRede(mrede, node_cortes);
      }
   //ordena alfabeticamente Nodes de Cortes
   node_pai->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereRede(TTreeNode *node_pai)
   {
   //variáveis locais
   VTBarra   *barra;
   VTRede    *rede;
   TTreeNode *node_rede, *node_bar;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //insere TreeNodes p/ Cortes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //insere TreeNode da Rede
      node_rede = TView->Items->AddChildObject(node_pai, rede->Codigo, rede);
      //insere TreeNodes p/ Barra inicial
      if (rede->Radial)
         {
         if ((barra = rede->BarraInicial()) != NULL)
            {
            node_bar = TView->Items->AddChildObject(node_rede, "Barra Inicial", barra);
            TView->Items->AddChildObject(node_bar, barra->Codigo, barra);
            }
         }
      TreeViewInsereCorteDaRede(rede, node_rede);
      //insere TreeNodes p/ os Cortes da Rede
      TreeViewInsereCorteDaRede(rede, node_rede);
      }
   //ordena alfabeticamente Nodes de Cortes
   node_pai->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereRedeDaMRede(VTMRede *mrede, TTreeNode *node_pai)
   {
   //variáveis locais
   TTreeNode *node_rede;
   VTRede    *rede;
   TList     *lisREDE = mrede->LisRede();

   //insere TreeNodes p/ Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //insere TreeNode da Rede
      node_rede = TView->Items->AddChildObject(node_pai, rede->Codigo, rede);
      //insere TreeNodes p/ os Cortes da Rede
      TreeViewInsereCorteDaRede(rede, node_rede);
      }
   //ordena alfabeticamente Nodes de Cortes
   node_pai->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereRedeDoCorte(VTCorte *corte, TTreeNode *node_corte)
   {
   //variáveis locais
   TTreeNode *node_rede;
   VTRede *rede;
   TList  *lisREDE = corte->LisRede();

   //insere TreeNode c/ título "Redes" do Corte
   node_rede = TView->Items->AddChild(node_corte, "Redes");
   //insere TreeNodes p/ Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //insere TreeNode da Rede
       TView->Items->AddChildObject(node_rede, rede->Codigo, rede);
      }
   //ordena alfabeticamente os Nodes das redes
   node_rede->AlphaSort(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewInsereReducaoDoCorte(VTCorte *corte, TTreeNode *node_corte)
   {
   //variáveis locais
   TTreeNode *node_reducao, *node_base;
   VTRede    *rede;
   VTReducao *reducao;

   //insere TreeNode c/ título "Reduções Redes Carregadas" no Corte
   node_base = TView->Items->AddChild(node_corte, "Redes");
   //insere um TreeNode p/ cada Rede c/ Redução no Corte
   lisREDU->Clear();
   corte->LisReducao(lisREDU);
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      node_reducao = TView->Items->AddChildObject(node_base, reducao->Codigo, reducao);
      //insere TreeNodes c/ Rede das Reducoes
      //TreeViewInsereRedeDaReducao(reducao, node_reducao);
      //insere TreeNodes c/ Eqptos das Reducoes
      TreeViewInsereEqptoDaReducao(reducao, node_reducao);
      }
   //ordena alfabeticamente Nodes de Reducores dentro do TreeNode de Reducoes de Redes NÃO carregadas
   node_base->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewOrdenaBloco(void)
   {
   //variáveis locais
   AnsiString txt;
   int        bloco_id;
   TList     *lisLIG;
   TTreeNode *node_raiz, *node_bloco;
   VTLigacao *ligacao;
   VTBloco   *bloco;
   VTBlocos  *blocos   = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   TList     *lisBLOCO = blocos->LisBloco();

   //reinicia TView
   TView->Items->Clear();
   //insere TreeNode raiz
   txt.sprintf("Blocos (%d)", lisBLOCO->Count);
   node_raiz = TView->Items->Add(NULL, txt);
   //insere TreeNodes p/ Blocos
   for (int n = 0; n < lisBLOCO->Count; n++)
      {
      bloco   = (VTBloco*)lisBLOCO->Items[n];
      lisLIG = bloco->LisLigacao();
      bloco_id = n+1;
      //insere TreeNode p/ Bloco
      txt.sprintf("Bloco%d (nl=%d)", bloco_id, lisLIG->Count);
      node_bloco = TView->Items->AddChildObject(node_raiz, txt, bloco);
      //insere TreeNodes p/ Ligacoes das Zonas
      for (int nb = 0; nb < lisLIG->Count; nb++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nb];
         //insere TreeNode p/ Ligacao
         txt = ligacao->TipoAsString() + ": " + ligacao->Codigo;
         TView->Items->AddChildObject(node_bloco, txt, ligacao);
         }
      }
   //seleciona Node raiz
   node_raiz->Expand(false);
   TView->Selected = node_raiz;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewOrdenaPorCorte(void)
   {
   //variáveis locais
   TTreeNode *node_raiz;

   //reinicia TView
   TView->Items->Clear();
   //insere TreeNode raiz
   node_raiz = TView->Items->Add(NULL, "Cortes");
   //insere TreeNodes p/ Cortes
   TreeViewInsereCorte(node_raiz);
   //seleciona Node raiz
   node_raiz->Expand(false);
   TView->Selected = node_raiz;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewOrdenaPorRede(void)
   {
   //variáveis locais
   TTreeNode *node_raiz;

   //reinicia TView
   TView->Items->Clear();
   //insere TreeNode raiz
   node_raiz = TView->Items->Add(NULL, "Redes");
   //insere TreeNodes p/ Redes
   TreeViewInsereRede(node_raiz);
   //seleciona Node raiz
   node_raiz->Expand(false);
   TView->Selected = node_raiz;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewOrdenaPorMRede(void)
   {
   //variáveis locais
   TTreeNode *node_raiz;

   //reinicia TView
   TView->Items->Clear();
   //insere TreeNode raiz
   node_raiz = TView->Items->Add(NULL, "MetaRedes");
   //insere TreeNodes p/ Redes
   TreeViewInsereMRede(node_raiz);
   //seleciona Node raiz
   node_raiz->Expand(false);
   TView->Selected = node_raiz;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDebug::TreeViewOrdenaZona(void)
   {
   //variáveis locais
   int        num_bar, zona_id;
   AnsiString txt;
   TTreeNode *node_raiz, *node_zona;
   TList     *lisBAR;
   VTBarra   *barra;
   VTZona    *zona;
   VTZonas   *zonas   = (VTZonas*)apl->GetObject(__classid(VTZonas));
   TList     *lisZONA = zonas->LisZona();

   //reinicia TView
   TView->Items->Clear();
   //insere TreeNode raiz
   node_raiz = TView->Items->Add(NULL, "Zonas");
   //insere TreeNodes p/ Zonas
   for (int n = 0; n < lisZONA->Count; n++)
      {
      zona   = (VTZona*)lisZONA->Items[n];
      lisBAR = zona->LisBarra();
      zona_id = n+1;
      //insere TreeNode p/ Zona
      num_bar = lisBAR->Count;
      txt.sprintf("Zona%d (nb=%d)", zona_id, num_bar);
      node_zona = TView->Items->AddChildObject(node_raiz, txt, zona);
      //insere TReeNodes p/ Barras das Zonas
      for (int nb = 0; nb < lisBAR->Count; nb++)
         {
         barra = (VTBarra*)lisBAR->Items[nb];
         //insere TreeNode p/ barra
         TView->Items->AddChildObject(node_zona, barra->Codigo, barra);
         }
      }
   //seleciona Node raiz
   node_raiz->Expand(false);
   TView->Selected = node_raiz;
   }

//---------------------------------------------------------------------------
//eof

