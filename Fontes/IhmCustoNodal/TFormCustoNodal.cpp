//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <math.h>
#include "TFormCustoNodal.h"
#include "TAplCustoNodal.h"
#include "..\CustoNodal\VTCustos.h"
#include "..\CustoNodal\VTCustoNodal.h"
#include "..\CustoNodal\VTMainCustoNodal.h"
#include "..\Edita\VTEdita.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormCustoNodal::TFormCustoNodal(TForm* Owner, VTApl *apl_owner, TWinControl *parent)
                           :TFormEdt(Owner, parent)
   {
   //variáveis locais
   VTEdita *edita  = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

   try{//cria objetos
      apl = new TAplCustoNodal(this, apl_owner);
      //insere ActionRedeChanged em VTEdita
      if (edita) edita->ActionOnChangeRedeInsere(ActionRedeChanged);
      }catch(Exception &e)
         {
         }
   //pré-seleciona TabSheetGrafico
   PageControl->ActivePage = TabSheetGrafico;
   }

//---------------------------------------------------------------------------
__fastcall TFormCustoNodal::~TFormCustoNodal(void)
   {
   //destrói objetos
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::ActionRedeChangedExecute(TObject *Sender)
   {
   //atualiza Chart e CLBoxSerie
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::Atualiza(void)
   {
   //variáveis locais
   VTMainCustoNodal *main = (VTMainCustoNodal*)apl->GetObject(__classid(VTMainCustoNodal));

   //calcula custos nodais
   main->Executa();
   //inicia Chart e CLBoxSerie: uma curva p/ cada CustoNodal
   ChartInicia();
   //inicia ListView
   LViewInicia();
   //simula click em CLBoxSerie p/ exibir apenas as Series selecionadas
   CLBoxSerieClickCheck(NULL);
   }

//---------------------------------------------------------------------------
TColor __fastcall TFormCustoNodal::ChartCorSerie(int n)
   {
   TColor color[] = {clBlue, clRed, clLime, clYellow, clFuchsia};

   //proteção
   n = (n % (sizeof(color)/sizeof(TColor)));
   return(color[n]);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::ChartInicia(void)
   {
   //variáveis locais
   double       val_min, val_max, increment;
   VTCustoNodal *custo_nodal;
   VTCustos     *custos = (VTCustos*)apl->GetObject(__classid(VTCustos));
   TList        *lisCUSTO_NODAL = custos->ListaCustoNodal();

   //esconde todas as Series
   for (int n = 0; n < chart->SeriesCount(); n++) {chart->Series[n]->Active = false;}
   //reinicia TChart
   chart->Title->Text->Clear();
   chart->Title->Text->Add("Custo Nodal");
   chart->LeftAxis->Minimum  = -10000.;
   chart->LeftAxis->Maximum  = +10000.;
   //reinicia CLBoxSerie: exibe uma Serie p/ cada CustoNodal
   CLBoxSerie->Clear();
   for (int n = 0; (n < lisCUSTO_NODAL->Count)&&(n < chart->SeriesCount()); n++)
      {
      custo_nodal = (VTCustoNodal*)lisCUSTO_NODAL->Items[n];
      ChartInsereCurva(custo_nodal, chart->Series[n], ChartCorSerie(n), (n == 0));
      CLBoxSerie->Items->AddObject(custo_nodal->eqpto->Codigo, chart->Series[n]);
      CLBoxSerie->Checked[n] = true;
      }
   //completa configuração do TChart
   //inclui uma margem adicional nos valores máximo e mínimo
   val_max   = chart->LeftAxis->Maximum;
   val_min   = chart->LeftAxis->Minimum;
   increment = (val_max - val_min) * 0.1;
   val_max  += increment;
   val_min  -= increment;
   //define incremento
   increment = (val_max - val_min) / 10.;
   if      (increment > 1.)   increment = 1.;
   else if (increment > 0.1)  increment = 0.1;
   else if (increment > 0.01) increment = 0.01;
   else                       increment = 0.001;
   //redefine valores mínimo e máximo do eixo y
   chart->LeftAxis->Visible   = true;
   chart->LeftAxis->Automatic = false;
   chart->LeftAxis->Maximum   = val_max;
   chart->LeftAxis->Minimum   = val_min;
   chart->LeftAxis->Increment = increment;
   chart->Visible      = true;
   CLBoxSerie->Visible = true;
   chart->Refresh();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::ChartInsereCurva(VTCustoNodal *custo_nodal, TChartSeries *serie, TColor cor, bool first)
   {
   //variáveis locais
   AnsiString  titulo;
   double      val, val_max, val_min;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //reinicia série
   serie->Clear();
   serie->Active      = true;
   serie->Title       = custo_nodal->eqpto->Codigo;
   serie->SeriesColor = cor;
   //preenche a Série c/ os valores em todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina valor no patamar
      val = custo_nodal->Valor[np];
      //insere ponto na Serie
      //serie->AddXY(np+1, val, IntToStr(np+1), cor);
      serie->AddXY(np, val, IntToStr(np+1), cor);
      //inclui ponto adicional p/ representar o fim do patamar(histograma)
      serie->AddXY(np+1, val, IntToStr(np+1), cor);
      //determina valores máximo e mínimo
      if (np == 0) {val_max = val_min = val;}
      val_max = max(val_max, val);
      val_min = min(val_min, val);
      }
   //define valores mínimo e máximo do TChart
   if (first)
      {
      chart->LeftAxis->Maximum = val_max;
      chart->LeftAxis->Minimum = val_min;
      }
   else
      {
      chart->LeftAxis->Maximum = max(chart->LeftAxis->Maximum, val_max);
      chart->LeftAxis->Minimum = min(chart->LeftAxis->Minimum, val_min);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::CLBoxSerieClickCheck(TObject *Sender)
   {
   //variáveis locais
   TChartSeries *serie;

   //exibe/esconde Serie
   for (int n = 0; n < CLBoxSerie->Count; n++)
      {
      serie = (TChartSeries*)CLBoxSerie->Items->Objects[n];
      serie->Active = CLBoxSerie->Checked[n];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::CLBoxSerieDrawItem(TWinControl *Control, int Index,
                                               TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TCanvas      *Canvas = CLBoxSerie->Canvas;
   TChartSeries *series;
   TColor       color_ori;
   TBrushStyle  style_ori;

   //limpa a área do item
   Canvas->FillRect(Rect);
   //determina objeto TChartSeries
   if ((series = (TChartSeries*)CLBoxSerie->Items->Objects[Index]) == NULL) return;
   //salva cor/style original
   color_ori = Canvas->Brush->Color;
   style_ori = Canvas->Brush->Style;
   //define área do retângulo p/ a cor da série
   TRect r = Rect;
   r.Top += 2;  r.Bottom -=2;
   r.Right = r.Left + (r.Bottom - r.Top) + 2;
   //escreve nome da Serie
   Canvas->TextOut(r.Right+1, Rect.Top, series->Title);
   //desenha retângulo c/ a cor
   Canvas->Brush->Color = series->SeriesColor;
   Canvas->Brush->Style = bsSolid;
   Canvas->FillRect(r);  //desenha o retângulo da cor
   //restaura cor/style original
   Canvas->Brush->Color = color_ori;
   Canvas->Brush->Style = style_ori;
   /*
   //define área para apresentação da cor, após o nome
   TRect r = Rect;
   r.Top++;  r.Bottom--; r.Right--;
   //r.Left = r.Right -(2*(r.Bottom - r.Top));
   r.Left = r.Right -(r.Bottom - r.Top);
   Canvas->Brush->Color = series->SeriesColor;
   Canvas->Brush->Style = bsSolid;
   Canvas->FillRect(r);  //desenha o retângulo da cor
   //restaura cor/style original
   Canvas->Brush->Color = color_ori;
   Canvas->Brush->Style = style_ori;
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::DefinePatamar(int ind_pat)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::FormShow(TObject *Sender)
   {
   //atualiza Chart e CLBoxSerie
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustoNodal::LViewInicia(void)
   {
   //variáveis locais
   int          count;
   TListItem    *Item;
   VTCustoNodal *custo_nodal;
   VTPatamar    *patamar;
   VTCustos     *custos    = (VTCustos*)apl->GetObject(__classid(VTCustos));
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList        *lisCUSTO_NODAL = custos->ListaCustoNodal();

   //reinicia LView
   LView->Clear();
   //define número de colunas do LView
   count = lisCUSTO_NODAL->Count + 1;
   while (LView->Columns->Count > count) LView->Columns->Delete(LView->Columns->Count-1);
   //define Caption da Coluna
   for (int n = 0; (n < lisCUSTO_NODAL->Count)&&(n < LView->Columns->Count-1); n++)
      {//define Caption da Coluna
      custo_nodal = (VTCustoNodal*)lisCUSTO_NODAL->Items[n];
      LView->Column[n+1]->Caption = custo_nodal->eqpto->Codigo;
      }
   //loop p/ todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {
      patamar = patamares->Patamar[np];
      //insere TListItem
      Item = LView->Items->Add();
      //insere Hora do patamar
      Item->Caption = patamar->Nome;
      //loop p/ todos CustoNodal
      for (int n = 0; (n < lisCUSTO_NODAL->Count)&&(n < LView->Columns->Count); n++)
         {
         //define Caption da Coluna
         custo_nodal = (VTCustoNodal*)lisCUSTO_NODAL->Items[n];
         //insere SubItem
         Item->SubItems->Add(DoubleToStr("%2.1f", custo_nodal->Valor[np]));
         }
      }
   }

//---------------------------------------------------------------------------
//eof



