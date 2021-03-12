//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormViewFase.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRedes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormViewFase::TFormViewFase(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
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
   IniciaVetorFases();
   //inicia lista de arranjos
   IniciaLisArranjo(lisEQP);
   //salva cores dos arranjos em InfoSet
   SalvaCoresDosArranjos();
   }

//---------------------------------------------------------------------------
__fastcall TFormViewFase::~TFormViewFase(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //restaura cores dos Arranjos
   RestauraCoresDosArranjos();
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus equipamentos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::ActionSelAllExecute(TObject *Sender)
   {//DVK 2013.08.16
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   for (int n = 0; n < CLBox->Items->Count; n++)
      {//marca tudo
      CLBox->Checked[n] = true;
      //marca se a fase deve ser exibida ou não
      vet_fase[n].visible = CLBox->Checked[n];
      }
   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::ActionSelNoneExecute(TObject *Sender)
   {//DVK 2013.08.16
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   for (int n = 0; n < CLBox->Items->Count; n++)
      {//desmarca tudo
      CLBox->Checked[n] = false;
      //marca se a fase deve ser exibida ou não
      vet_fase[n].visible = CLBox->Checked[n];
      }
   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::CheckBoxClick(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::CLBoxClickCheck(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há um Item selecionado
   if (CLBox->ItemIndex < 0) return;
   //marca se a fase deve ser exibida ou não
   vet_fase[CLBox->ItemIndex].visible = CLBox->Checked[CLBox->ItemIndex];
   //reinicia atributos dos Arranjos
   IniciaAtributoArranjo();
   //atualiza o grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::CLBoxDblClick(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há um Item selecionado
   if (CLBox->ItemIndex < 0) return;
   //apresenta ColorDialog com a cor da fase
   ColorDialog->Color = vet_fase[CLBox->ItemIndex].cor;
   if (ColorDialog->Execute())
      {//redefine a cor do Arranjo
      vet_fase[CLBox->ItemIndex].cor = ColorDialog->Color;
      //reinicia atributos dos Arranjos
      IniciaAtributoArranjo();
      //atualiza o grafico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::CLBoxDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TColor      color_ori;
   TRect       r;
   TBrushStyle style_ori;
   TCanvas    *canvas = CLBox->Canvas;

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
   canvas->Brush->Color = vet_fase[Index].cor;
   canvas->Brush->Style = bsSolid;
   //desenha o retângulo da cor
   canvas->FillRect(r);
   //restaura cor/style original
   canvas->Brush->Color = color_ori;
   canvas->Brush->Style = style_ori;
   //escreve código da fase
   Rect.Left = r.Right + 2;
   canvas->TextOut(Rect.Left, Rect.Top, vet_fase[Index].codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::CLBoxInicia(void)
   {
   //variáveis locais
   int index;

   //reinicia LBox com as fases disponíveis
   CLBox->Clear();
   for (int n = 0; n < DIM_VET_FASE-1; n++)
      {
      index   = CLBox->Items->Add(vet_fase[n].codigo);
      CLBox->Checked[index] = vet_fase[n].visible;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::ExibeFases(void)
   {
   //inicia opções de atributos
   CheckBoxFase->Checked   = true;
   CheckBoxSempre->Checked = false;
   CheckBoxCodigo->Checked = false;
   //inicia CLBox
   CLBoxInicia();
   //atualiza atributos dos Arranjos
   IniciaAtributoArranjo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::IniciaAtributoArranjo(void)
   {
   //variáveis locais
   int       index;
   VTArranjo *arranjo;

   //loop p/ todos Arranjos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      //determina index que mapeia as fases do Arranjo em vet_fase
      index = MapeiaFaseNoVetFase(arranjo->Fases);
      //atualiza atributos a serem exibidos
      arranjo->Show[eqp_FIXO]    = CheckBoxSempre->Checked;
      arranjo->Show[tre_ARRANJO] = CheckBoxCodigo->Checked;
      arranjo->Show[tre_FASES]   = CheckBoxFase->Checked;
      arranjo->Show[eqp_VISIBLE] = vet_fase[index].visible;
      //redefine cor do arranjo em função das suas fases
      arranjo->Color             = vet_fase[index].cor;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::IniciaLisArranjo(TList *lisARR)
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
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::IniciaVetorFases(void)
   {
   //variáveis locais
   int     tag[DIM_VET_FASE] = {faseA,   faseB, faseC,
                                faseAB,  faseBC, faseCA,
                                faseABC, -1};
   TColor  cor[DIM_VET_FASE] = {clRed,     clGreen,   clYellow,
                                clSkyBlue, clFuchsia, clOlive,
                                clLime,    clGray};
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //inicia valores da fase e da cor de todos os elementos
   for (int n = 0; n < DIM_VET_FASE; n++)
      {
      vet_fase[n].tag     = tag[n];
      vet_fase[n].cor     = cor[n];
      vet_fase[n].visible = true;
      }
   //inicia código das fases válidas
   for (int n = 0; n < DIM_VET_FASE-1; n++)
      {
      vet_fase[n].codigo = fases->AsStrABCN[vet_fase[n].tag];
      }
   }

//---------------------------------------------------------------------------
int __fastcall TFormViewFase::MapeiaFaseNoVetFase(int fases)
   {
   //verifica a fase do arranjo (excluindo neutro e terra)
   switch(fases & faseABC)
      {
      case faseA:    return(0);
      case faseB:    return(1);
      case faseC:    return(2);
      case faseAB:   return(3);
      case faseBC:   return(4);
      case faseCA:   return(5);
      case faseABC:  return(6);
      default:       return(DIM_VET_FASE - 1);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::RestauraCoresDosArranjos(void)
   {
   //variáveis locais
   int       color;
   VTArranjo *arranjo;
   VTGeral   *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //loop p/ todos Arranjos
   geral->Infoset->LoadFromFile(ARQ_INFOSET);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      geral->Infoset->GetInfo(arranjo->Codigo, color);
      arranjo->Color = TColor(color);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormViewFase::SalvaCoresDosArranjos(void)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTGeral   *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //loop p/ todos Arranjos
   geral->Infoset->Clear();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      geral->Infoset->AddInfo(arranjo->Codigo, int(arranjo->Color));
      }
   geral->Infoset->SaveToFile(ARQ_INFOSET);
   }

//---------------------------------------------------------------------------
//eof


