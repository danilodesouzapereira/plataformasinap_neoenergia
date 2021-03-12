//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <ComCtrls.hpp>
#include "TFormEsquematico.h"
#include "Esquematico.h"
#include "VTEsquematico.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEsquematico::TFormEsquematico(TForm *Owner, VTApl *apl, TWinControl *parent)
                            : TFormEdt(Owner, parent)
   {
   //vari�veis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   esquematico = NewObjEsquematico(apl);
   lisEQP      = new TList();
   //inicia coordenadas esquem�ticos
   esquematico->IniciaEsquematico(redes);
   }

//---------------------------------------------------------------------------
__fastcall TFormEsquematico::~TFormEsquematico(void)
   {
   //vari�veis locais
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //se necess�rio, salva coordenadas do diagrama
   SalvaCoordenadas();
   //reapresenta esquem�tico geral
   ExibeEsquematicoGeral();
   //altera modo gr�fico
   if (redegraf->ModoGrafico == modoESQ_FUNCIONAL) redegraf->ModoGrafico = modoESQUEMATICO;
   //destr�i objetos
   if (esquematico) {delete esquematico; esquematico = NULL;}
   //destr�i lista sem destrui seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq1Execute(TObject *Sender)
   {
   //se ncess�rio, salva coordenadas do diagrama atual
   SalvaCoordenadas();
   //atualiza apresenta��o do gr�fico
   ExibeGrafico();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq2Execute(TObject *Sender)
   {
   //se necess�rio, salva coordenadas do diagrama atual
   SalvaCoordenadas();
   //atualiza apresenta��o do gr�fico
   ExibeGrafico();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq2AlignHLRExecute(TObject *Sender)
   {
   //vari�veis locais
   VTRede    *rede;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o: verifica se est� exibindo o esquem�tico parcial de uma Rede
   if ((rede = TViewRedeSelecionada()) == NULL) return;
   //ajusta esquem�tico da Rede
   esquematico->Align(rede, HoriLeftRight);
   //atualiza gr�fico
   grafico->Redraw();
   //salva informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = rede;
   diag_ant.but_modo = ButEsq2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq2AlignHRLExecute(TObject *Sender)
   {
   //vari�veis locais
   VTRede    *rede;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o: verifica se est� exibindo o esquem�tico parcial de uma Rede
   if ((rede = TViewRedeSelecionada()) == NULL) return;
   //ajusta esquem�tico da Rede
   esquematico->Align(rede, HoriRightLeft);
   //atualiza gr�fico
   grafico->Redraw();
   //salva informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = rede;
   diag_ant.but_modo = ButEsq2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq2AlignVBTExecute(TObject *Sender)
   {
   //vari�veis locais
   VTRede    *rede;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o: verifica se est� exibindo o esquem�tico parcial de uma Rede
   if ((rede = TViewRedeSelecionada()) == NULL) return;
   //ajusta esquem�tico da Rede
   esquematico->Align(rede, VertBottomTop);
   //atualiza gr�fico
   grafico->Redraw();
   //salva informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = rede;
   diag_ant.but_modo = ButEsq2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq2AlignVTBExecute(TObject *Sender)
   {
   //vari�veis locais
   VTRede    *rede;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o: verifica se est� exibindo o esquem�tico parcial de uma Rede
   if ((rede = TViewRedeSelecionada()) == NULL) return;
   //ajusta esquem�tico da Rede
   esquematico->Align(rede, VertTopBottom);
   //atualiza gr�fico
   grafico->Redraw();
   //salva informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = rede;
   diag_ant.but_modo = ButEsq2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionEsq2AlignNoneExecute(TObject *Sender)
   {
   //vari�veis locais
   VTRede    *rede;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o: verifica se est� exibindo o esquem�tico parcial de uma Rede
   if ((rede = TViewRedeSelecionada()) == NULL) return;
   //ajusta esquem�tico da Rede
   esquematico->Align(rede, esqNORMAL);
   //atualiza gr�fico
   grafico->Redraw();
   //salva informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = rede;
   diag_ant.but_modo = ButEsq2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ActionGeoExecute(TObject *Sender)
   {
   //se ncess�rio, salva coordenadas do diagrama atual
   SalvaCoordenadas();
   //atualiza apresenta��o do gr�fico
   ExibeGrafico();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::CLBoxReducaoClickCheck(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto   *eqpto;
   //VTRede    *rede;
   VTReducao *reducao;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresenta��o dos esquem�ticos
   for (int n = 0; n < CLBoxReducao->Items->Count; n++)
      {
      eqpto = (VTEqpto*)CLBoxReducao->Items->Objects[n];
      if (eqpto->Tipo() == eqptoREDE)
         {//FALTA IMPLEMENTAR
         //rede = (VTRede*)eqpto;
         }
      else if (eqpto->Tipo() == eqptoREDUCAO)
         {
         reducao = (VTReducao*)eqpto;
         reducao->Show[eqp_VISIBLE] = CLBoxReducao->Checked[n];
         }
      }
   //atualiza gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::CLBoxReducaoInicia(void)
   {
   //vari�veis locais
   int        index;
   VTMRede    *mrede;
   VTRede     *rede;
   VTReducao  *reducao;
   TList      *lisMR, *lisREDE, *lisREDU;
   //VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia CLBoxReducao
   CLBoxReducao->Items->Clear();
   CLBoxReducao->Sorted = false;
   //determina Rede selecionada
   if ((rede = TViewRedeSelecionada()) == NULL) return;
/* BLOQUEADO AT� COLOCAR OP��O DE EXIBIR/ESCONDER ESQUEM�TICO DE REDE
   //insere em CLBoxReducao as Redes carregadas c/ esquematico vis�vel
   lisREDE = redes->LisRede();
   //habilita visualiza��o dos esquem�ticos das Redes vizinhas carregadas
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      if (rede->Visible()) continue;
      if (rede->BarraFicticia() == NULL) continue;
      //insere Rede  em CLBoxReducao
      index = CLBoxReducao->Items->AddObject(rede->Codigo, rede);
      CLBoxReducao->Checked[index] = true;
      }
*/
   //insere em CLBoxReducao as Reducoes vizinhas
   /*
   lisMR = redes->LisMRede();
   for (int nr = 0; nr < lisMR->Count; nr++)
      {
      mrede = (VTMRede*)lisMR->Items[nr];
      if (mrede->Carregada) continue;
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //verifica se Reducao vis�vel
         if (! reducao->Show[eqp_VISIBLE]) continue;
         //insere Reducao em CLBoxReducao
         index = CLBoxReducao->Items->AddObject(reducao->Codigo, reducao);
         CLBoxReducao->Checked[index] = true;
         }
      }
   */
   //insere em CLBoxReducao as Reducoes vizinhas
   lisEQP->Clear();
   rede->MRede->LisMRedeVizinha(lisEQP);
   for (int nm = 0; nm < lisEQP->Count; nm++)
      {
      mrede = (VTMRede*)lisEQP->Items[nm];
      if (mrede->Carregada) continue;
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //verifica se Reducao vis�vel
         //if (! reducao->Show[eqp_VISIBLE]) continue;
         //insere Reducao em CLBoxReducao
         index = CLBoxReducao->Items->AddObject(reducao->Codigo, reducao);
         CLBoxReducao->Checked[index] = reducao->Visible();
         }
      }
   //ordena alfabeticamente
   CLBoxReducao->Sorted = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::DefineVisualizacaoRedes(bool visible)
   {
   //vari�veis locais
   VTRede  *rede;
   TList   *lisREDE;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //habilita/desabilita visualiza��o de todas Redes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //habilita visualiza��o
      rede->Show[eqp_VISIBLE] = visible;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::DefineVisualizacaoReducoes(bool visible)
   {
   //vari�veis locais
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisMR, *lisREDU;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //habilita/desabilita visualiza��o de todas Reducoes
   lisMR = redes->LisMRede();
   for (int nr = 0; nr < lisMR->Count; nr++)
      {
      mrede = (VTMRede*)lisMR->Items[nr];
      if (mrede->Carregada) continue;
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //habilita visualiza��o
         reducao->Show[eqp_VISIBLE] = visible;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::DefineVisualizacaoReducoesVizinhas(VTRede *rede, bool visible)
   {
   //vari�veis locais
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisREDU;
   //VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //habilita/desabilita visualiza��o das Reducoes vizinhas
   lisEQP->Clear();
   rede->MRede->LisMRedeVizinha(lisEQP);
   for (int nm = 0; nm < lisEQP->Count; nm++)
      {
      mrede = (VTMRede*)lisEQP->Items[nm];
      if (mrede->Carregada) continue;
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //habilita visualiza��o
         reducao->Show[eqp_VISIBLE] = visible;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ExibeEsquematicoGeral(void)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //habilita visualiza��o de todas Redes e Reducoes
   DefineVisualizacaoRedes(true);
   DefineVisualizacaoReducoes(true);
   //atualiza coordenadas p/ esquem�tico geral de todas as Barras
   esquematico->MontaEsquematicoGeral(redes);
   //atualiza gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::EscondeGrafico(void)
   {
   //desabilita visualiza��o de todas Redes
   DefineVisualizacaoRedes(false);
   DefineVisualizacaoReducoes(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ExibeGrafico(void)
   {
   //vari�veis locais
   VTRede  *rede;

   //verifica Eqpto selecionado em TViewRede
   if ((rede = TViewRedeSelecionada()) == NULL)
      {//esconde o gr�fico
      EscondeGrafico();
      }
   else
      {//exibe gr�fico da Rede
      ExibeGraficoRede(rede);
      }
   //salva informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = rede;
   if      (ButGeo->Down)  diag_ant.but_modo = ButGeo;
   else if (ButEsq1->Down) diag_ant.but_modo = ButEsq1;
   else if (ButEsq2->Down) diag_ant.but_modo = ButEsq2;
   }


//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::ExibeGraficoRede(VTRede *rede)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //habilita/desabilita op��o p/ selecionar modo geo-referenciado
   ActionGeo->Enabled = (! esquematico->ExisteBarraSemCoordUtm(rede));
   //prote��o
   if ((ButGeo->Down)&&(! ActionGeo->Enabled))
      {//Rede n�o tem coord.utm e est� c/ op��o de diagrama geo-referenciado
      ButGeo->Down  = false;
      ButEsq1->Down = true;
      }
   //habilita op��o de esquem�tico funcional
   ActionEsq2->Enabled = true;
   ButEsq2->PopupMenu  = PopupEsqAlign;
   //verifica se est� sendo mostrada uma Rede diferente da anterior
   if (rede != diag_ant.eqpto)
      {//habilita visualiza��o apenas da Rede
      DefineVisualizacaoRedes(false);
      rede->Show[eqp_VISIBLE] = true;
      //habilita visualiza��o apenas das Reducoes vizinhas da Rede
      DefineVisualizacaoReducoes(false);
      DefineVisualizacaoReducoesVizinhas(rede, true);
      }
   //exibe o diagrama selecionado
   if (ButGeo->Down)
      {//exibe diagrama georefenciado
      redegraf->ModoGrafico = modoGEOREFERENCIADO;
      //atualiza gr�fico
      grafico->ModoGraficoAlterado();
      //grafico->Redraw();
      }
   else if (ButEsq1->Down)
      {//atualiza coordenadas p/ esquem�tico geral de todas as Barras
      esquematico->MontaEsquematicoGeral(rede);
      //exibe diagrama esquem�tico
      redegraf->ModoGrafico = modoESQ_FUNCIONAL;
      //atualiza gr�fico
      grafico->ModoGraficoAlterado();
      //grafico->Redraw();
      }
   else if (ButEsq2->Down)
      {//atualiza coordenadas p/ esquem�tico parcial das Barras da Rede
      esquematico->MontaEsquematicoParcial(rede);
      //exibe diagrama esquem�tico
      redegraf->ModoGrafico = modoESQ_FUNCIONAL;
      //atualiza gr�fico
      grafico->ModoGraficoAlterado();
      //grafico->Redraw();
      }
   else
      {//esconde o gr�fico
      grafico->Hide();
      }
   }

//---------------------------------------------------------------------------
int __fastcall TFormEsquematico::ExisteRedeSemCluster(TList *lisEXT, int tipo_rede)
   {
   //vari�veis locais
   VTRede  *rede;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Rede do TipoRede
   lisEXT->Clear();
   redes->LisRede(lisEXT, tipo_rede);
   //descarta Redes egadas ou n�o carragrupadas em Cluster
   for (int nr = lisEXT->Count-1; nr >= 0; nr--)
      {
      rede = (VTRede*)lisEXT->Items[nr];
      if      (rede->Cluster)     lisEXT->Delete(nr);
      else if (! rede->Carregada) lisEXT->Delete(nr);
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::FormShow(TObject *Sender)
   {
   //reposiciona o Form
   Left = 0;
   Top  = 0;
   //inicia informa��es do diagrama gr�fico selecionado
   diag_ant.eqpto    = TViewRedeSelecionada();
   diag_ant.but_modo = ButEsq2;
   //pr�-seleciona exibi��o de esquem�tico funcional
   ButEsq2->Down = true;
   //inicia TreeViewRede
   TViewRedeInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::SalvaCoordenadas(void)
   {
   //vari�veis locais
   VTEqpto    *eqpto;
   VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica Eqpto anteriormente selecionado
   eqpto = diag_ant.eqpto;
   if (eqpto == NULL)
      {//exibindo todas Redes: verifica diagrama em exibi��o
      if (diag_ant.but_modo == ButGeo)  //if (ButGeo->Down)
         {//diagrama georeferenciado: nada a fazer
         return;
         }
      if (diag_ant.but_modo == ButEsq1)  //if (ButEsq1->Down)
         {//diagrama esquem�tico geral: salva coordenadas esquem�tico geral
         esquematico->SalvaEsquematicoGeral(redes);
         return;
         }
      }
   else if (eqpto->Tipo() == eqptoCLUSTER)
      {//exibindo Cluster: verifica diagrama em exibi��o
      if (diag_ant.but_modo == ButGeo)  //if (ButGeo->Down)
         {//diagrama georeferenciado: nada a fazer
         return;
         }
      if (diag_ant.but_modo == ButEsq1)  //if (ButEsq1->Down)
         {//diagrama esquem�tico geral: salva coordenadas esquem�tico geral
         esquematico->SalvaEsquematicoGeral((VTCluster*)eqpto);
         return;
         }
      if (diag_ant.but_modo == ButEsq2)  //if (ButEsq2->Down)
         {//salva coordenadas do esquem�tico parcial das Redes do Cluster
         esquematico->SalvaEsquematicoParcial((VTCluster*)eqpto);
         //restaura coordenadas do esquem�tico geral
         esquematico->MontaEsquematicoGeral(redes);
         }
      }
   else if (eqpto->Tipo() == eqptoREDE)
      {//exibindo Rede: verifica diagrama em exibi��o
      if (diag_ant.but_modo == ButGeo)  //if (ButGeo->Down)
         {//diagrama georeferenciado: nada a fazer
         return;
         }
      if (diag_ant.but_modo == ButEsq1)  //if (ButEsq1->Down)
         {//diagrama esquem�tico geral: salva coordenadas esquem�tico geral
         esquematico->SalvaEsquematicoGeral((VTRede*)eqpto);
         return;
         }
      if (diag_ant.but_modo == ButEsq2)  //if (ButEsq2->Down)
         {//salva coordenadas do esquem�tico parcial da Rede
         esquematico->SalvaEsquematicoParcial((VTRede*)eqpto);
         //restaura coordenadas do esquem�tico geral
         esquematico->MontaEsquematicoGeral(redes);
         return;
         }
      }
/*


   //verifica gr�fico em exibi��o
   if (ButGeo->Down)
      {//diagrama georeferenciado: nada a fazer
      return;
      }
   if (ButEsq1->Down)
      {//diagrama esquem�tico geral: salva coordenadas esquem�tico geral
      esquematico->SalvaEsquematicoGeral(redes);
      return;
      }
   //diagrama esquem�tico parcial: verifica Eqpto selecionado em TViewRede
   if ((eqpto = TViewRedeSelecionada()) == NULL)
      {//Redes: salva coordenadas do esquem�tico geral
      //esquematico->SalvaEsquematicoGeral(redes);
      }
   else if (eqpto->Tipo() == eqptoCLUSTER)
      {//salva coordenadas do esquem�tico parcial das Redes do Cluster
      esquematico->SalvaEsquematicoParcial((VTCluster*)eqpto);
      //restaura coordenadas do esquem�tico geral
      esquematico->MontaEsquematicoGeral(redes);
      }
   else if (eqpto->Tipo() == eqptoREDE)
      {//salva coordenadas do esquem�tico parcial da Rede
      esquematico->SalvaEsquematicoParcial((VTRede*)eqpto);
      //restaura coordenadas do esquem�tico geral
      esquematico->MontaEsquematicoGeral(redes);
      }
*/
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::TViewRedeChange(TObject *Sender, TTreeNode *Node)
   {
   //atualiza apresenta��o do gr�fico
   ExibeGrafico();
   //inicia CListBoxReducao (IMPORTANTE: ap�s a chamada de ExibeGrafico())
   CLBoxReducaoInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::TViewRedeChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange)
   {
   //habilita a altera��o
   AllowChange = true;
   //se necess�rio, salva coordenadas do diagrama
   SalvaCoordenadas();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEsquematico::TViewRedeInicia(void)
   {
   //vari�veis locais
   TTreeNode  *node_cluster, *node_tipo, *node_rede;
   TList      *lisCLUSTER;
   TList      *lisREDE;
   TList      *lisTIPO;
   VTCluster  *cluster;
   VTRede     *rede;
   VTTipoRede *tiporede;
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos    *tipos    = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //desabilita eventos associados ao TreeView
   TViewRede->OnChange = NULL;
   //reinicia TView
   TViewRede->Items->Clear();
   //insere Nodes de n�vel 0 p/ Clusters
   lisCLUSTER = redes->LisCluster();
   for (int nc = 0; nc < lisCLUSTER->Count; nc++)
      {
      cluster = (VTCluster*)lisCLUSTER->Items[nc];
      if (! cluster->Carregado) continue;
      //insere Node p/ o Cluster
      //node_cluster = TViewRede->Items->AddChildObject(node_raiz, cluster->Codigo, NULL);
      node_cluster = TViewRede->Items->Add(NULL, cluster->Codigo);
      //insere Node de n�vel 1: Redes do Cluster
      lisREDE = cluster->LisRede();
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //insere Node c/ objeto VTRede
         node_rede = TViewRede->Items->AddChildObject(node_cluster, rede->Codigo, rede);
         if (TViewRede->Selected == NULL) TViewRede->Selected = node_rede;
         }
      }
   //loop p/ todo TipoRede
   lisTIPO = tipos->LisTipoRede();
   for (int nt = 0; nt < lisTIPO->Count; nt++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[nt];
      //verifica se h� Rede do TipoRede
      lisEQP->Clear();
      if (ExisteRedeSemCluster(lisEQP, tiporede->Id) == 0) continue;
      //insere Node de n�vel 0: TipoRede
      node_tipo = TViewRede->Items->Add(NULL, tiporede->Codigo);
      //loop p/ todas Redes do TipoRede
      for (int nr = 0; nr < lisEQP->Count; nr++)
         {
         rede = (VTRede*)lisEQP->Items[nr];
         //insere Node de n�vel 1: Rede do tipo
         node_rede = TViewRede->Items->AddChildObject(node_tipo, rede->Codigo, rede);
         if (TViewRede->Selected == NULL) TViewRede->Selected = node_rede;
         }
      //ordena TreeNode das Redes dentro do Node de Tipo
      node_tipo->AlphaSort();
      }
   //expande todo o TreeView
	//tviewRede->FullExpand();
   //expande Node pai do Node selecionado
	if ((TViewRede->Selected)&&(TViewRede->Selected->Parent))
      {
      TViewRede->Selected->Parent->Expand(false);
      }
   //reabilita eventos associados ao TreeView
   TViewRede->OnChange = TViewRedeChange;
   //simula ocorr�ncia do evento
   TViewRedeChange(TViewRede, TViewRede->Selected);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEsquematico::TViewRedeSelecionada(void)
   {
   //verifica se h� um TreeNode selecionado
   if (TViewRede->Selected == NULL) return(NULL);
   return((VTRede*)(TViewRede->Selected->Data));
   }

//---------------------------------------------------------------------------
//eof


