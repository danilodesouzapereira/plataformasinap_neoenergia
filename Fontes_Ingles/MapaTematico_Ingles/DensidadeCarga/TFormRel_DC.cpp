//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <Math.hpp>
#include "TFormRel_DC.h"
#include "TFaixa.h"
#include "TFormLimites_DC.h"
#include "VTTema_DC.h"
#include "..\..\Apl\TApl.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "TeCanvas"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRel_DC::TFormRel_DC(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *ActionRemontaMapaPadrao)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   this->ActionRemontaMapaPadrao = ActionRemontaMapaPadrao;
   //insere Form em seu Parent
   if (parent) Parent = parent;
   //exibe dados das Faixas de demanda
   //Atualiza();
   //cores padrão para degrade de faixas de demanda
   ButCorConsMenor->SymbolColor=clBlue;
   ButCorConsMaior->SymbolColor=clRed;
   }

//---------------------------------------------------------------------------
__fastcall TFormRel_DC::~TFormRel_DC(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::Atualiza(bool habilita_checkbox)
   {
   //habilita desabilita CheckBox
   LView->Checkboxes = habilita_checkbox;
   LView->Column[0]->Width = (habilita_checkbox) ? 20 : 0;
   //monta LView c/ as faixas de demanda
//   LViewInicia();
   if(RadioGroupUnidade->ItemIndex == 0) LViewIniciaKVA();
   else                                  LViewIniciaMVA();
   //inicia Chart
   ChartInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::AtualizaDegrade(void)
   {
   //variáveis locais
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));

   //determina opção
   switch(RadioGroupDegrade->ItemIndex)
      {
      case 0:  //linear por demanda
         if(tema_DC->MontaGraficoDegradeFaixa(ButCorConsMenor->SymbolColor,
                                                   ButCorConsMaior->SymbolColor, true))
            {
            ActionRemontaMapaPadrao->Execute();
            //atualiza LView e Chart
            Atualiza(LView->Checkboxes);
            }
         break;

      default: //linear por faixa
         if(tema_DC->MontaGraficoDegradeFaixa(ButCorConsMenor->SymbolColor,
                                                   ButCorConsMaior->SymbolColor))
            {
            ActionRemontaMapaPadrao->Execute();
            //atualiza LView e Chart
            Atualiza(LView->Checkboxes);
            }
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::ButCorConsMaiorClick(TObject *Sender)
   {
   if (DegradePorFaixa->Down)
      {
      AtualizaDegrade();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::ButCorConsMenorClick(TObject *Sender)
   {
   if (DegradePorFaixa->Down)
      {
      AtualizaDegrade();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::ButEditLimitesClick(TObject *Sender)
   {
   //variaveis locais
   VTTema_DC  *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));

   //cria FormLimites_DC
   FormLimites_DC = new TFormLimites_DC(this, apl, ActionRemontaMapaPadrao);
   //exibe
   if (FormLimites_DC->ShowModal() == mrOk)
      {//atualiza mapa temático
      tema_DC->MontaFaixaDemanda();
      tema_DC->MontaGraficoPadrao();
      if (ActionRemontaMapaPadrao) ActionRemontaMapaPadrao->Execute();
      //atualiza LView e Chart
      Atualiza(LView->Checkboxes);
      //FKM-2015.10.01 atualiza cores das faixas de degradê
      AtualizaDegrade();
      }
   //destroi FormLimites_DC
   delete  FormLimites_DC;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::ChartInicia(void)
   {
   //variáveis locais
   AnsiString txt;
   TFaixa    *faixa;
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList     *lisFAIXA = tema_DC->LisFaixa();

   //reinicia PieSeries
   PieSeriesQtde->Clear();
   PieSeriesSmva->Clear();
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      //verifica se faixa possui celulas
      if (faixa->lisCELULA->Count == 0) continue;
      txt.sprintf("Faixa: %2d: ", nf + 1);
      txt += DoubleToStr("%4.3f", faixa->dem_min_mva) + DoubleToStr(" / %4.3f", faixa->dem_max_mva);
      //insere faixa em PieSeriesQtde
      PieSeriesQtde->Add(faixa->lisCELULA->Count, txt, faixa->color);
      //insere faixa em PieSeriesSmva
      PieSeriesSmva->Add(faixa->dem_tot_mva, txt, faixa->color);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::DegradePorFaixaClick(TObject *Sender)
   {
   //variaveis locais
   TFaixa    *faixa;
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList     *lisFAIXA = tema_DC->LisFaixa();

   //botão down: habilita degrade por faixa
   if (DegradePorFaixa->Down)
      {
      //DegradePorFaixa->Caption   = "Cor por Faixa"; //FKM 2015.09.23
      PanelDegrade->Visible      = true; //FKM 2015.09.23
      RadioGroupDegrade->Visible = true; //FKM 2016.07.14
      AtualizaDegrade();
      }
   //botão up: grafico padrão
   else
      {
      //DegradePorFaixa->Caption   = "Degradê por Faixa"; //FKM 2015.09.23
      PanelDegrade->Visible      = false; //FKM 2015.09.23
      RadioGroupDegrade->Visible = false; //FKM 2016.07.14
      TColor color[15] = {clGreen, clYellow,     clRed,     clBlue, clLime,
                        clAqua,  clMoneyGreen, clSkyBlue, clTeal, clOlive,
                        clGray,  clCream,      clFuchsia, clNavy, clPurple};
      for (int nf = 0; nf < lisFAIXA->Count; nf++)
         {//voltam as cores default
         faixa        = (TFaixa*)lisFAIXA->Items[nf] ;
         faixa->color = color[nf];
         }
      tema_DC->MontaGraficoPadrao();
      //atualiza mapa
      ActionRemontaMapaPadrao->Execute();
      //atualiza LView e Chart
      Atualiza(LView->Checkboxes);
      }
     }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona no canto superior esquerdo
   Top  = 0;
   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia o descricao
   RadioGroupEquilibraFaixaClick(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::FormResize(TObject *Sender)
   {
   //redefine tamanho dos TChart
   ChartQtde->Width = PanelChart->ClientWidth / 2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewClick(TObject *Sender)
   {
   //variáveis locais
   int        num_alteracao = 0;
   TListItem  *Item;
   TFaixa     *faixa;
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTTema_DC  *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));

   //verifica se está exibindo Checkboxes
   if (! LView->Checkboxes) return;
   //verifica se foi alterada a opção de exibir/esconder uma Faixa
   for (int n = 0; n < LView->Items->Count; n++)
      {
      Item  = LView->Items->Item[n];
      if ((faixa = (TFaixa*)Item->Data) == NULL) continue;
      if (Item->Checked != faixa->visible)
         {//atualiza estado da Faixa
         faixa->visible = Item->Checked;
         num_alteracao++;
         }
      }
   //se houve alteração, atualiza e reapresenta o mapa temático
   if (num_alteracao > 0)
      {
      tema_DC->MontaGraficoPadrao();
      grafico->MapaTematicoEnabled = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //variáveis locais
   bool checked;

   //verifica se primeira coluna
   if (Column->Index != 0) return;
   //verifica se deve marcar ou desmarcar todas as linhas
   if (Column->Caption[1] == '+')
      {//redefine Caption
      Column->Caption = "-";
      //marca todas as linhas
      checked = true;
      }
   else
      {//redefine Caption
      Column->Caption = "+";
      //desmarca todos as linhas
      checked = false;
      }
   for (int n = 0; n < LView->Items->Count; n++)
      {
      LView->Items->Item[n]->Checked = checked;
      }
   //simula evento de Click p/ atualizar o mapa temático
   LViewClick(Sender);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewCustomDrawSubItem(TCustomListView *Sender,
      TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw)
   {
   //variáveis locais
   TFaixa *faixa = (TFaixa*)Item->Data;

   //proteção
   if (faixa == NULL) return;
   if (SubItem == (Item->SubItems->Count + 1))
      {
      LView->Canvas->Brush->Color = faixa->color;
      LView->Canvas->Brush->Style = bsSolid;
      }
   else
      {
      LView->Canvas->Brush->Color = clWindow;
      LView->Canvas->Brush->Style = bsSolid;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewDblClick(TObject *Sender)
   {
   //variáveis locais
   TFaixa *faixa;

   //verifica se há um ListItem selecionado
   if (LView->Selected == NULL) return;
   //determina Faixa
   if ((faixa = (TFaixa*)LView->Selected->Data) == NULL) return;
   //abre TDialogColor p/ usuário selecionar cor p/ a Faixa
   ColorDialog->Color = faixa->color;
   if (ColorDialog->Execute())
      {//verifica se houve troca de cor
      if (ColorDialog->Color != faixa->color)
         {
         faixa->color = ColorDialog->Color;
         //atualiza mapa temático
         if (ActionRemontaMapaPadrao) ActionRemontaMapaPadrao->Execute();
         //atualiza LView e Chart
         Atualiza(LView->Checkboxes);
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewInicia(void)
   {
   //variáveis locais
   TListItem *Item;
   TFaixa    *faixa;
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList     *lisFAIXA = tema_DC->LisFaixa();

   //reincia lView
   LView->Clear();
   //loop p/ todas Faixas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      if(! faixa->enabled) break;
      //insere ListItem
      Item = LView->Items->Add();
      Item->Data    = faixa;
      Item->Checked = faixa->visible;
      Item->Caption = "";
      //faixa
      Item->SubItems->Add(IntToStr(nf+1));
      //limite inferior e superior
      Item->SubItems->Add(DoubleToStr("%4.3f", faixa->dem_min_mva));
      //verifica se é a última faixa
      if (nf < (lisFAIXA->Count-1))
         {Item->SubItems->Add(DoubleToStr("%4.3f", faixa->dem_max_mva));}
      else
         {Item->SubItems->Add(DoubleToStr("> %4.3f", faixa->dem_min_mva));}
      //número de células (proporcional à área)
      Item->SubItems->Add(IntToStr(faixa->lisCELULA->Count));
      //percentual de células
      Item->SubItems->Add(DoubleToStr("%3.2f", faixa->num_cel_perc));
      //demanda total na faixa
      Item->SubItems->Add(DoubleToStr("%4.3f", faixa->dem_tot_mva));
      //percentual da demanda na faixa
      Item->SubItems->Add(DoubleToStr("%3.2f", faixa->dem_tot_perc));
      }
   //inclui mais uma linha com total de células e de demanda
   Item = LView->Items->Add();
   Item->Data    = NULL;
   Item->Checked = false;
   Item->Caption = "";
   //faixa e limite inferior/superior
   Item->SubItems->Add("Total");
   Item->SubItems->Add("-");
   Item->SubItems->Add("-");
   //número de células (proporcional à área)
   Item->SubItems->Add(IntToStr(tema_DC->TotalCelula()));
   //percentual de células
   if (tema_DC->TotalCelula() == 0)
      {Item->SubItems->Add("0");}
   else
      {Item->SubItems->Add("100");}
   //demanda total na faixa
   Item->SubItems->Add(DoubleToStr("%4.3f", tema_DC->TotalDemanda_mva()));
   //percentual da demanda na faixa
   if (IsDoubleZero(tema_DC->TotalDemanda_mva()))
      {Item->SubItems->Add("0");}
   else
      {Item->SubItems->Add("100");}
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewIniciaKVA(void)
   {
   //variáveis locais
   double    valor;
   TListItem *Item;
   TFaixa    *faixa;
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList     *lisFAIXA = tema_DC->LisFaixa();

   //reincia lView
   LView->Clear();
   //altera caption das colunas com unidade
   LView->Columns->Items[2]->Caption = "Lim.Inf (kVA)";
   LView->Columns->Items[3]->Caption = "Lim.Sup (kVA)";
   LView->Columns->Items[6]->Caption = "S (kVA)";
   //loop p/ todas Faixas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      if(! faixa->enabled) break;
      //insere ListItem
      Item = LView->Items->Add();
      Item->Data    = faixa;
      Item->Checked = faixa->visible;
      Item->Caption = "";
      //faixa
      Item->SubItems->Add(IntToStr(nf+1));
      //limite inferior e superior
      valor = faixa->dem_min_mva * 1000.;
      Item->SubItems->Add(DoubleToStr("%4.3f", valor));
      //verifica se é a última faixa
      if (nf < (lisFAIXA->Count-1))
         {
         valor = faixa->dem_max_mva * 1000.;
         Item->SubItems->Add(DoubleToStr("%4.3f", valor));
         }
      else
         {
         valor = faixa->dem_min_mva * 1000.;
         Item->SubItems->Add(DoubleToStr("> %4.3f", valor));
         }
      //número de células (proporcional à área)
      Item->SubItems->Add(IntToStr(faixa->lisCELULA->Count));
      //percentual de células
      Item->SubItems->Add(DoubleToStr("%3.2f", faixa->num_cel_perc));
      //demanda total na faixa
      valor = faixa->dem_tot_mva * 1000.;
      Item->SubItems->Add(DoubleToStr("%4.3f", valor));
      //percentual da demanda na faixa
      Item->SubItems->Add(DoubleToStr("%3.2f", faixa->dem_tot_perc));
      }
   //inclui mais uma linha com total de células e de demanda
   Item = LView->Items->Add();
   Item->Data    = NULL;
   Item->Checked = false;
   Item->Caption = "";
   //faixa e limite inferior/superior
   Item->SubItems->Add("Total");
   Item->SubItems->Add("-");
   Item->SubItems->Add("-");
   //número de células (proporcional à área)
   Item->SubItems->Add(IntToStr(tema_DC->TotalCelula()));
   //percentual de células
   if (tema_DC->TotalCelula() == 0)
      {Item->SubItems->Add("0");}
   else
      {Item->SubItems->Add("100");}
   //demanda total na faixa
   valor = tema_DC->TotalDemanda_mva() * 1000.;
   Item->SubItems->Add(DoubleToStr("%4.3f", valor));
   //percentual da demanda na faixa
   if (IsDoubleZero(tema_DC->TotalDemanda_mva()))
      {Item->SubItems->Add("0");}
   else
      {Item->SubItems->Add("100");}
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::LViewIniciaMVA(void)
   {
   //variáveis locais
   TListItem *Item;
   TFaixa    *faixa;
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList     *lisFAIXA = tema_DC->LisFaixa();

   //reincia lView
   LView->Clear();
   //altera caption das colunas com unidade
   LView->Columns->Items[2]->Caption = "Lim.Inf (MVA)";
   LView->Columns->Items[3]->Caption = "Lim.Sup (MVA)";
   LView->Columns->Items[6]->Caption = "S (MVA)";
   //loop p/ todas Faixas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      if(! faixa->enabled) break;
      //insere ListItem
      Item = LView->Items->Add();
      Item->Data    = faixa;
      Item->Checked = faixa->visible;
      Item->Caption = "";
      //faixa
      Item->SubItems->Add(IntToStr(nf+1));
      //limite inferior e superior
      Item->SubItems->Add(DoubleToStr("%4.3f", faixa->dem_min_mva));
      //verifica se é a última faixa
      if (nf < (lisFAIXA->Count-1))
         {Item->SubItems->Add(DoubleToStr("%4.3f", faixa->dem_max_mva));}
      else
         {Item->SubItems->Add(DoubleToStr("> %4.3f", faixa->dem_min_mva));}
      //número de células (proporcional à área)
      Item->SubItems->Add(IntToStr(faixa->lisCELULA->Count));
      //percentual de células
      Item->SubItems->Add(DoubleToStr("%3.2f", faixa->num_cel_perc));
      //demanda total na faixa
      Item->SubItems->Add(DoubleToStr("%4.3f", faixa->dem_tot_mva));
      //percentual da demanda na faixa
      Item->SubItems->Add(DoubleToStr("%3.2f", faixa->dem_tot_perc));
      }
   //inclui mais uma linha com total de células e de demanda
   Item = LView->Items->Add();
   Item->Data    = NULL;
   Item->Checked = false;
   Item->Caption = "";
   //faixa e limite inferior/superior
   Item->SubItems->Add("Total");
   Item->SubItems->Add("-");
   Item->SubItems->Add("-");
   //número de células (proporcional à área)
   Item->SubItems->Add(IntToStr(tema_DC->TotalCelula()));
   //percentual de células
   if (tema_DC->TotalCelula() == 0)
      {Item->SubItems->Add("0");}
   else
      {Item->SubItems->Add("100");}
   //demanda total na faixa
   Item->SubItems->Add(DoubleToStr("%4.3f", tema_DC->TotalDemanda_mva()));
   //percentual da demanda na faixa
   if (IsDoubleZero(tema_DC->TotalDemanda_mva()))
      {Item->SubItems->Add("0");}
   else
      {Item->SubItems->Add("100");}
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::RadioGroupDegradeClick(TObject *Sender)
   {
   AtualizaDegrade();
   }
//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::RadioGroupEquilibraFaixaClick(TObject *Sender)
   {
   //variáveis locais
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));

   //determina opção
   switch(RadioGroupEquilibraFaixa->ItemIndex)
      {
      case 0:  //quantidade de células
         tema_DC->FaixaValoresQtdCelula();
         RichEditDescricao->Lines->Clear();
         RichEditDescricao->Lines->Add("Redefine os limites das faixas de "
         "demanda, fazendo com que cada faixa possua a mesma quantidade de células.\n"
         "Isto é, cada faixa de demanda ocupa a mesma área da rede.");
         break;
      case 1:  //demanda  agregada
         tema_DC->FaixaValoresDemanda();
         RichEditDescricao->Lines->Clear();
         RichEditDescricao->Lines->Add("Redefine os limites das faixas de "
         "demanda, fazendo com que cada faixa possua a mesma demanda total.\n"
         "Isto é, cada faixa de demanda (cor) possui a mesma demanda total.");
         break;
      case 2:  //demanda   igual
         tema_DC->FaixaValoresUniforme();
         RichEditDescricao->Lines->Clear();
         RichEditDescricao->Lines->Add("Redefine os limites das faixas de "
         "demanda, fazendo com que cada faixa possua o mesmo intervalo de demanda.\n"
         "Isto é, cada faixa de demanda (cor) possui a mesma demanda total.");
         break;

      default: //default
         tema_DC->FaixaValoresDefault();
         RichEditDescricao->Lines->Clear();
         break;
      }
   //atualiza mapa temático
   tema_DC->MontaFaixaDemanda();
   //atualiiza as cores da faixa
   if (DegradePorFaixa->Down)
      {AtualizaDegrade();}
   tema_DC->MontaGraficoPadrao();
   if (ActionRemontaMapaPadrao) ActionRemontaMapaPadrao->Execute();
   //atualiza LView e Chart
   RadioGroupUnidadeClick(NULL);
   ChartInicia();
//   Atualiza(LView->Checkboxes);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRel_DC::RadioGroupUnidadeClick(TObject *Sender)
   {
   //verifica unidade selecionada
   switch(RadioGroupUnidade->ItemIndex)
      {
      case 0: //kVA
         LViewIniciaKVA();
         ChartSmva->Title->Text->Clear();
         ChartSmva->Title->Text->Add("Demanda (kVA) por Faixa");
         break;

      case 1:
      default: //MVA
         LViewIniciaMVA();
         ChartSmva->Title->Text->Clear();
         ChartSmva->Title->Text->Add("Demanda (MVA) por Faixa");
         break;
      }
   }

//---------------------------------------------------------------------------
//eof




