//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdt::TFormEdt(TComponent* Owner, TWinControl *parent)
   : TForm(Owner)
   {
   try{//insere o Form no WinControl definido como parent
      DefParent(parent);
      //salva posi��o inicial da janela flutuante
      janela.top    = 0;
      janela.left   = 0;
      janela.width  = Width;
      janela.height = Height;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormEdt::~TFormEdt(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionAlignDownExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada em cima
   if (Align == alTop) Align = alNone;
   //alinha janela em baixo
   Align = alBottom;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionAlignLeftExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada � direita
   if (Align == alRight) Align = alNone;
   //alinha janela � esquerda
   Align = alLeft;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionAlignNoneExecute(TObject *Sender)
   {
   //cancela alinhamento da janela (flutuante)
   Align  = alNone;
   Top    = janela.top;
   Left   = janela.left;
   Width  = janela.width;
   Height = janela.height;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionAlignRightExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada � esquerda
   if (Align == alLeft) Align = alNone;
   //alinha janela � direita
   Align = alRight;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionAlignUpExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada em baixo
   if (Align == alBottom) Align = alNone;
   //alinha janela em cima
   Align = alTop;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionEdtExecute(TObject *Sender)
   {
   //OBS: m�todo a ser redefinido nas classes derivadas

   //sinaliza mouse ativo
   AtualizaIndicacaoEditorOn(butEdt);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionMouseOffExecute(TObject *Sender)
   {
   //liga indica��o de mouse ativo e for�a bot�o Down
   AtualizaIndicacaoEditorOff();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::ActionMouseOnExecute(TObject *Sender)
   {
   //liga indica��o de mouse ativo e for�a bot�o Down
   AtualizaIndicacaoEditorOn(button_down);
   //executa m�todo Ativo
   Ativo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::Ativo(void)
   {
   //m�todo a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::Atualiza(void)
   {
   //m�todo a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::AtualizaIndicacaoEditorOff(void)
   {
   //indica mouse inativo
   ImageMouseOff->Visible = true;
   ImageMouseOn->Visible  = false;
   //verifica se h� um Button "abaixado"
   if (button_down)
      {//"levanta" Button que est� abaixado
      button_down->Down = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::AtualizaIndicacaoEditorOn(TToolButton *button)
   {
   //salva ponteiro do Button
   button_down = button;
   if (button)
      {//"abaixa" o Button
      button->Down = true;
      //indica mouse ativo
      ImageMouseOn->Visible  = true;
      ImageMouseOff->Visible = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::Centraliza(void)
   {
   Left = (Parent->Width  - Width) / 2;
   Top  = (Parent->Height - Height) / 2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdt::DefParent(TWinControl *parent)
   {
   if (parent) Parent = parent;
   }

//---------------------------------------------------------------------------
//eof



