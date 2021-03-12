//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormViewArranjo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRedes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormViewArranjo::TFormViewArranjo(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   apl = apl_owner;
   //rerdefine Parent
   Parent = parent;
   //cria objetos
   lisEQP = new TList();
   lisTMP = new TList();
   //posiciona o Form
   FormIniciaPosicao();
   //inicia vetor de cores
   ind_color = 0;
   vet_color[0] = clBlue;
   vet_color[1] = clRed;
   vet_color[2] = clGreen;
   vet_color[3] = clYellow;
   vet_color[4] = clSkyBlue;
   vet_color[5] = clFuchsia;
   vet_color[6] = clOlive;
   vet_color[7] = clPurple;
   vet_color[8] = clNavy;
   vet_color[9] = clLime;
   }

//---------------------------------------------------------------------------
__fastcall TFormViewArranjo::~TFormViewArranjo(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus equipamentos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::ActionSelAllExecute(TObject *Sender)
   {//DVK 2013.08.16
   //variáveis locais
   VTArranjo *arranjo;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   for (int n = 0; n < CLBox->Items->Count; n++)
      {//marca tudo
      CLBox->Checked[n] = true;
      //determina Arranjo selecionado
      if ((arranjo = (VTArranjo*)CLBox->Items->Objects[n]) == NULL) return;
      //marca se o Arranjo deve ser exibido ou não
      arranjo->Show[eqp_VISIBLE] = CLBox->Checked[n];
      }
   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::ActionSelNoneExecute(TObject *Sender)
   {//DVK 2013.08.16
   //variáveis locais
   VTArranjo *arranjo;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   for (int n = 0; n < CLBox->Items->Count; n++)
      {//desmarca tudo
      CLBox->Checked[n] = false;
      //determina Arranjo selecionado
      if ((arranjo = (VTArranjo*)CLBox->Items->Objects[n]) == NULL) return;
      //marca se o Arranjo deve ser exibido ou não
      arranjo->Show[eqp_VISIBLE] = CLBox->Checked[n];
      }
   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::CheckBoxClick(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::CLBoxClickCheck(TObject *Sender)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há um Item selecionado
   if (CLBox->ItemIndex < 0) return;
   //determina Arranjo selecionado
   if ((arranjo = (VTArranjo*)CLBox->Items->Objects[CLBox->ItemIndex]) == NULL) return;
   //marca se o Arranjo deve ser exibido ou não
   arranjo->Show[eqp_VISIBLE] = CLBox->Checked[CLBox->ItemIndex];
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::CLBoxDblClick(TObject *Sender)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há um Item selecionado
   if (CLBox->ItemIndex < 0) return;
   //determina Arranjo selecionado
   if ((arranjo = (VTArranjo*)CLBox->Items->Objects[CLBox->ItemIndex]) == NULL) return;
   //apresenta ColorDialog com a cor do Arranjo
   ColorDialog->Color = arranjo->Color;
   if (ColorDialog->Execute())
      {//redefine a cor do Arranjo
      arranjo->Color = ColorDialog->Color;
      //atualiza o grafico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::CLBoxDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TColor      color_ori;
   TRect       r;
   TBrushStyle style_ori;
   TCanvas    *canvas = CLBox->Canvas;
   VTArranjo  *arranjo;

   //determina Arranjo
   if ((arranjo = (VTArranjo*)CLBox->Items->Objects[Index]) == NULL) return;
   //salva cor/style original
   color_ori = canvas->Brush->Color;
   style_ori = canvas->Brush->Style;
   //limpa a área do item
   canvas->FillRect(Rect);
   //define área para apresentação da cor, antes do nome
   r.Top     = Rect.Top    + 2;
   r.Bottom  = Rect.Bottom - 2;
   r.Left    = Rect.Left   + 1;
   r.Right   = r.Left      + (r.Bottom - r.Top);
   canvas->Brush->Color = arranjo->Color;
   canvas->Brush->Style = bsSolid;
   //desenha o retângulo da cor
   canvas->FillRect(r);
   //restaura cor/style original
   canvas->Brush->Color = color_ori;
   canvas->Brush->Style = style_ori;
   //escreve código do Arranjo
   Rect.Left = r.Right + 2;
   canvas->TextOut(Rect.Left, Rect.Top, arranjo->Codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::CLBoxInicia(void)
   {
   //variáveis locais
   int       index;
   VTArranjo *arranjo;

   //atualiza Caption
   Caption = "Arranjos";
   //monta lisEQP c/ Arranjos existentes na Rede
   IniciaLisArranjo(lisEQP);
   //reinicia LBox com os eqptos em lisEQP
   CLBox->Clear();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      index   = CLBox->Items->AddObject(arranjo->Codigo, arranjo);
      CLBox->Checked[index] = arranjo->Show[eqp_VISIBLE];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::ExibeArranjos(void)
   {
   //inicia opções de atributos
   CheckBoxCodigo->Checked   = false;
   CheckBoxSempre->Checked   = false;
   CheckBoxZseq_pu->Checked  = false;
   CheckBoxZseq_ohm->Checked = true;
   //inicia lista de arranjos
   IniciaLisArranjo(lisEQP);
   //atualiza atributos dos Arranjos
   IniciaAtributoArranjo();
   //inicia CLBox
   CLBoxInicia();
   //redefine cor dos arranjos
   RedefineCorArranjos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::IniciaAtributoArranjo(void)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //loop p/ todos Arranjos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      //atualiza atributos a serem exibidos
      arranjo->Show[eqp_FIXO]     = CheckBoxSempre->Checked;
      arranjo->Show[tre_ARRANJO]  = CheckBoxCodigo->Checked;
      arranjo->Show[tre_ZSEQ_PU]  = CheckBoxZseq_pu->Checked;
      arranjo->Show[tre_ZSEQ_OHM] = CheckBoxZseq_ohm->Checked;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::IniciaLisArranjo(TList *lisARR)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTTrecho  *trecho;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisARR c/ Arranjos existentes na rede
   lisARR->Clear();
   //inicia lisTMP com Trechos existentes na Rede
   lisTMP->Clear();
   redes->LisLigacao(lisTMP, eqptoTRECHO);
   //loop p/ todos os Trechos
   for (int n = 0; n < lisTMP->Count; n++)
      {
      trecho = (VTTrecho*)lisTMP->Items[n];
      arranjo = trecho->arranjo;
      if (lisARR->IndexOf(arranjo) < 0)
		 {//insere Arranho em lisARR
         arranjo->Show[eqp_VISIBLE] = true;
		 lisARR->Add(arranjo);
		 //verifica se o Arranjo utiliza cor preta
//		 if (arranjo->Color == clBlack)
//			{//redefine cor do Arranjo
//			arranjo->Color = SelecionaColor();
//			}
		 }
	  }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewArranjo::RedefineCorArranjos(void)
   {
   //variaveis locais
   VTArranjo *arranjo;

   for (int n = 0; n < CLBox->Items->Count; n++)
	  {
	  arranjo = (VTArranjo*)CLBox->Items->Objects[n];
	  //redefine cor do Arranjo
	  arranjo->Color = SelecionaColor();
	  }
   }

//---------------------------------------------------------------------------
TColor __fastcall TFormViewArranjo::SelecionaColor(void)
   {
   if (++ind_color >= (sizeof(vet_color)/sizeof(TColor))) ind_color = 0;
   return(vet_color[ind_color]);
   }

//---------------------------------------------------------------------------
//eof



