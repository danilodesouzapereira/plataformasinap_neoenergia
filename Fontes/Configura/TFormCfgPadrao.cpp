//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgPadrao.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\TPad.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Grafico\VTGrid.h"
#include "..\Geral\VTGeral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgPadrao::TformCfgPadrao(TForm* Owner, VTApl *apl, VTGrafico *graf)
   : TForm(Owner)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva ponteiros
   my_owner   = Owner;
   this->graf = graf;
   grid       = geral->grid;
   //esconde o form
   Visible = false;
   //redefine Parent
   if (Owner) Parent = Owner;
   //inicia ponteiros
   pad  = NULL;
   gbox = NULL;
   //redimensiona form p/ suas dimensões mínimas
   Width  = Constraints->MinWidth;
   Height = Constraints->MinHeight;
   //inicia CBoxEqpto c/ os GroupBox
   IniciaCBoxEqpto();
   //inicia GBox c/ configuração do Grid
   IniciaGBoxGrid();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionAlteraExibeExecute(TObject *Sender)
   {
   //lê opções de exibir pizzas
   pad->exibe.pie_chv = checkPieChvNF->Checked;
   pad->exibe.pie_ger = checkPieGer->Checked;
   pad->exibe.pie_sup = checkPieSup->Checked;
   pad->exibe.pie_tre = checkPieTre->Checked;
   pad->exibe.pie_trf = checkPieTrf->Checked;
   pad->exibe.pie_trf3e = checkPieTrf3e->Checked;
   //atualiza o gráfico
   if (graf) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionAlteraCorExecute(TObject *Sender)
   {
   //determina origem do evento
   TPanel *panel = (TPanel*)Sender;

   //passa o foco p/ o Form
   my_owner->SetFocus();
   //exibe dialog de seleção de cor
   ColorDialog->Color = panel->Color;
   if (ColorDialog->Execute())
      {//determina nova cor
      panel->Color = ColorDialog->Color;
      //atualiza o padrão e o gráfico
      ActionAlteraPadExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionAlteraPadExecute(TObject *Sender)
   {
   //lê nova configuração
   LeConfiguracao();
   //atualiza o gráfico
   if (graf) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionSelecionaEqptoExecute(TObject *Sender)
   {
   //esconde GBox atual
   if (gbox) gbox->Visible = false;
   //determina o GBox selecionado
   if (cboxEqpto->ItemIndex < 0) return;
   gbox = (TGroupBox*)cboxEqpto->Items->Objects[cboxEqpto->ItemIndex];
   //reposiciona o GBox
   gbox->Left = 4;
   gbox->Top  = toolBar->Top + toolBar->Height + 4;
   gbox->Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionSelecionaPadExecute(TObject *Sender)
   {
   //variáveis locais
   TPad *pad_sel;

   //proteção
   if (cboxPad->ItemIndex < 0) return;
   //novo padrão selecionado
   pad_sel = (TPad*)cboxPad->Items->Objects[cboxPad->ItemIndex];
   //atualiza o padrão atual
   *pad = *pad_sel;
   //atualiza este form
   IniciaTela();
   //atualiza o gráfico
   if (graf) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionSelGridAlignExecute(TObject *Sender)
   {
   //salva alinhamento do grid
   grid->align = cboxGridAlign->ItemIndex;
   //atualiza gráfico
   if (graf) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionSelGridCorExecute(TObject *Sender)
   {
   //exibe ColorDialog
   ColorDialog->Color = panelGridCor->Color;
   if (ColorDialog->Execute())
      {//seleciona nova cor de fundo
      grid->cor           = ColorDialog->Color;
      panelGridCor->Color = grid->cor;
      //atualiza gráfico
      if (graf) graf->Redraw();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionSelGridDimExecute(TObject *Sender)
   {
   //proteção
   if (cboxGridDim->ItemIndex < 0) return;
   //salva dimensão do grid
   grid->dim = cboxGridDim->Text.ToInt();
   //atualiza gráfico
   if (graf) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::ActionSelGridVisibleExecute(TObject *Sender)
   {
   //salva opção de exibir/esconder grid
   grid->visible = checkGridExibir->Checked;
   //atualiza o gráfico
   if (graf) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::DefObjPadrao(TPad *pad)
   {
   //salva ponteiro p/ objetos
   this->pad = pad;
   //atualiza CBox c/ padrão atual
   cboxPad->ItemIndex = cboxPad->Items->IndexOf(pad->nome);
   if (cboxPad->ItemIndex < 0) cboxPad->ItemIndex = 0;
   //atualiza tela
   IniciaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::IniciaCBoxEqpto(void)
   {
   //variáveis locais
   TGroupBox *gb[] = {gboxReducao, gboxBar,    gboxCap,   gboxCar,  gboxChvNA,
                      gboxChvNF,  gboxGer,   gboxSup,   gboxTrf,    gboxTrf3e,
                      gboxTre,     gboxBorda, gboxPad,  gboxGrid};
   //reinicia CBoxEqpto c/ os TGroupBox
   cboxEqpto->Clear();
   for (int n = 0; n < sizeof(gb)/sizeof(void*); n++)
      {
      cboxEqpto->Items->AddObject(gb[n]->Caption, gb[n]);
      }
   //seleciona o primeiro
   cboxEqpto->ItemIndex = 0;
   ActionSelecionaEqptoExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::FormClose(TObject *Sender, TCloseAction &Action)   {
   //apenas esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::FormShow(TObject *Sender)
   {
   //inicia tela c/ a configuração definida
   IniciaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::IniciaCBox(TComboBox *cbox, AnsiString opcao)
   {
   //procura item do CBox igual à opção indicada
   for  (int n = 0; n < cbox->Items->Count; n++)
      {
      if (cbox->Items->Strings[n] == opcao)
         {
         cbox->ItemIndex = n;
         return;
         }
      }
   //não encontrou opção: seleciona a primeira
   cbox->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::IniciaGBoxGrid(void)
   {
   //inicia GBox c/ configuração do Grid
   checkGridExibir->Checked = grid->Visible();
   cboxGridDim->ItemIndex   = cboxGridDim->Items->IndexOf(IntToStr(grid->dim));
   if (cboxGridDim->ItemIndex < 0) cboxGridDim->ItemIndex = 0;
   cboxGridAlign->ItemIndex = grid->align;
   if (cboxGridAlign->ItemIndex < 0) cboxGridAlign->ItemIndex = 0;
   panelGridCor->Color = grid->cor;
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::IniciaTela(void)
   {
   //desabilita Actions
   ActionAlteraExibe->Enabled = false;
   ActionAlteraPad->Enabled   = false;
   //inicia cores de borda/fundo
   panelBordaCor->Color     = pad->cor_borda;
   panelFundoCor->Color     = pad->cor_fundo;
   //inicia opções de exibir pizzas
   checkPieChvNF->Checked = pad->exibe.pie_chv;
   checkPieGer->Checked   = pad->exibe.pie_ger;
   checkPieSup->Checked   = pad->exibe.pie_sup;
   checkPieTre->Checked   = pad->exibe.pie_tre;
   checkPieTrf->Checked   = pad->exibe.pie_trf;
   checkPieTrf3e->Checked = pad->exibe.pie_trf3e;
   //inicia configurações de barra
   cboxBarExibe->ItemIndex = pad->bar.fmt;
   panelBarCor->Color      = pad->bar.cor;
   IniciaCBox(cboxBarDim, IntToStr(pad->bar.dim));
   //inicia configurações de reduções
   cboxReducaoExibe->ItemIndex = pad->reducao.fmt;
   panelReducaoCor->Color      = pad->reducao.cor;
   IniciaCBox(cboxReducaoDim, IntToStr(pad->reducao.dim));
   //inicia configurações de barra swing
   cboxSwingExibe->ItemIndex = pad->swing.fmt;
   panelSwingCor->Color      = pad->swing.cor;
   IniciaCBox(cboxSwingDim, IntToStr(pad->swing.dim));
   //inicia configurações de capacitores
   cboxCapExibe->ItemIndex = pad->cap.fmt;
   panelCapCor->Color      = pad->cap.cor;
   IniciaCBox(cboxCapDim, IntToStr(pad->cap.dim));
   //inicia configurações de cargas
   cboxCarExibe->ItemIndex = pad->car.fmt;
   panelCarCor->Color      = pad->car.cor;
   IniciaCBox(cboxCarDim, IntToStr(pad->car.dim));
   //inicia configurações de chaveNA
   cboxChvNAExibe->ItemIndex = pad->chv_na.fmt;
   panelChvNACor->Color      = pad->chv_na.cor;
   IniciaCBox(cboxChvNADim, IntToStr(pad->chv_na.dim));
   //inicia configurações de chaveNF
   cboxChvNFExibe->ItemIndex = pad->chv_nf.fmt;
   panelChvNFCor->Color      = pad->chv_nf.cor;
   IniciaCBox(cboxChvNFDim, IntToStr(pad->chv_nf.dim));
   //inicia configurações de gerador
   cboxGerExibe->ItemIndex = pad->ger.fmt;
   panelGerCor->Color      = pad->ger.cor;
   IniciaCBox(cboxGerDim, IntToStr(pad->ger.dim));
   //inicia configurações de suprimento
   cboxSupExibe->ItemIndex = pad->sup.fmt;
   panelSupCor->Color      = pad->sup.cor;
   IniciaCBox(cboxSupDim, IntToStr(pad->sup.dim));
   //inicia configurações de trechos
   cboxTreExibe->ItemIndex = pad->tre.fmt;
   panelTreCor->Color      = pad->tre.cor;
   IniciaCBox(cboxTreDim, IntToStr(pad->tre.dim));
   //inicia configurações de trafos
   cboxTrfExibe->ItemIndex = pad->trf.fmt;
   panelTrfCor->Color      = pad->trf.cor;
   IniciaCBox(cboxTrfDim, IntToStr(pad->trf.dim));
   //inicia configurações de trafos3E
   cboxTrf3eExibe->ItemIndex = pad->trf_3e.fmt;
   panelTrf3eCor->Color      = pad->trf_3e.cor;
   IniciaCBox(cboxTrf3eDim, IntToStr(pad->trf_3e.dim));
   //reabilita Actions
   ActionAlteraExibe->Enabled = true;
   ActionAlteraPad->Enabled   = true;
   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPadrao::LeConfiguracao(void)
   {
   //lê configuração de cor de borda e fundo
   pad->cor_borda = panelBordaCor->Color;
   pad->cor_fundo = panelFundoCor->Color;
   //lê configurações de barra
   pad->bar.fmt = cboxBarExibe->ItemIndex;
   pad->bar.cor = panelBarCor->Color;
   pad->bar.dim = cboxBarDim->Text.ToInt();
   //lê configurações de barra fictícia
   pad->reducao.fmt = cboxReducaoExibe->ItemIndex;
   pad->reducao.cor = panelReducaoCor->Color;
   pad->reducao.dim = cboxReducaoDim->Text.ToInt();
   //lê configurações de barra swing
   pad->swing.fmt = cboxSwingExibe->ItemIndex;
   pad->swing.cor = panelSwingCor->Color;
   pad->swing.dim = cboxSwingDim->Text.ToInt();
   //lê configurações de capacitor
   pad->cap.fmt = cboxCapExibe->ItemIndex;
   pad->cap.cor = panelCapCor->Color;
   pad->cap.dim = cboxCapDim->Text.ToInt();
   //lê configurações de carga
   pad->car.fmt = cboxCarExibe->ItemIndex;
   pad->car.cor = panelCarCor->Color;
   pad->car.dim = cboxCarDim->Text.ToInt();
   //lê configurações de chaveNA
   pad->chv_na.fmt = cboxChvNAExibe->ItemIndex;
   pad->chv_na.cor = panelChvNACor->Color;
   pad->chv_na.dim = cboxChvNADim->Text.ToInt();
   //lê configurações de chaveNF
   pad->chv_nf.fmt    = cboxChvNFExibe->ItemIndex;
   pad->chv_nf.cor    = panelChvNFCor->Color;
   pad->chv_nf.dim    = cboxChvNFDim->Text.ToInt();
   //lê configurações de gerador
   pad->ger.fmt = cboxGerExibe->ItemIndex;
   pad->ger.cor = panelGerCor->Color;
   pad->ger.dim = cboxGerDim->Text.ToInt();
   //lê configurações de suprimento
   pad->sup.fmt = cboxSupExibe->ItemIndex;
   pad->sup.cor = panelSupCor->Color;
   pad->sup.dim = cboxSupDim->Text.ToInt();
   //lê configurações de trafos
   pad->trf.fmt = cboxTrfExibe->ItemIndex;
   pad->trf.cor = panelTrfCor->Color;
   pad->trf.dim = cboxTrfDim->Text.ToInt();
   //lê configurações de trafos3E
   pad->trf_3e.fmt = cboxTrf3eExibe->ItemIndex;
   pad->trf_3e.cor = panelTrf3eCor->Color;
   pad->trf_3e.dim = cboxTrf3eDim->Text.ToInt();
   //lê configurações de trecho
   pad->tre.fmt = cboxTreExibe->ItemIndex;
   pad->tre.cor = panelTreCor->Color;
   pad->tre.dim = cboxTreDim->Text.ToInt();
   //define filtros p/ exibição
   pad->exibe.carga      = (pad->car.fmt > 0);
   pad->exibe.capacitor  = (pad->cap.fmt > 0);
   pad->exibe.gerador    = (pad->ger.fmt > 0);
   pad->exibe.suprimento = (pad->sup.fmt > 0);
   }

//---------------------------------------------------------------------------
//eof

