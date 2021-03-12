//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMsgBox.h"
#include <DLL_Inc\Funcao.h>
#include <windows.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMsgBox::TFormMsgBox(TComponent* Owner)
   : TForm(Owner)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TFormMsgBox::~TFormMsgBox(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall   TFormMsgBox::AcertaTamanho(int max_width)
   {//variaveis locais
   int n_linhas = RichEdit->Lines->Count;
   int max_size = max_width;
   int total_height = 0;
   int height = 0;
   int width = 0;
   int adicional_x = 0;
   int adicional_y = 0;
   TCanvas *canvas = this->Canvas;

   if(PanelAviso->Visible) adicional_x += PanelAviso->Width;
   if(PanelErro->Visible) adicional_x += PanelErro->Width;
   adicional_x += this->BorderWidth;
   adicional_x += this->Width - this->ClientWidth;
   adicional_x += 20;

   if(PanelBottom->Visible) adicional_y += PanelBottom->Height;
   if(PanelTop->Visible) adicional_y += PanelTop->Height;
   if(PanelSimNao->Visible) adicional_y += PanelSimNao->Height;
   if(PanelOK->Visible) adicional_y += PanelOK->Height;
   adicional_y += this->BorderWidth;
   adicional_y += this->Height - this->ClientHeight;

   //mede a string antes do processamento de quebra de linha do
   //richedit
   //max_size = canvas->TextWidth(txt);
   //percorre todas linhas do richedit medindo
   for (int n = 0; n < n_linhas; n++)
      {
      width = canvas->TextWidth(RichEdit->Lines->Strings[n]);
      height = canvas->TextHeight(RichEdit->Lines->Strings[n]);
      if(width > max_size)
         {
         max_size = width;
         }
      //se nao for a ultima linha e ela estiver vazia
      if(!(n == (n_linhas - 1) && (RichEdit->Lines->Strings[n].IsEmpty())))
         {
         total_height += height;
         }
      }
//
   //acerta a largura  e altura
   this->Width = max_size + adicional_x;
   this->Refresh();
   this->Height = total_height + adicional_y;
   this->Refresh();
   }
//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::Aviso(AnsiString txt)
   {
   Caption             = "Aviso !";
   PanelAviso->Visible = true;
   PanelOK->Visible    = true;
   RichEdit->Lines->Clear();
   RichEdit->Lines->Add(txt);
   //AcertaTamanho(txt);
   AcertaTamanho(LarguraMax(txt));
   }

//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::ButSimClick(TObject *Sender)
   {
   ModalResult = mrYes;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::ButNaoClick(TObject *Sender)
   {
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::ButCancelaClick(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::Confirma(AnsiString txt1, AnsiString txt2, int buttons)
   {
   AnsiString  txt_concatenada;

   Caption              = "Confirmar !";
   PanelAviso->Visible  = true;
   PanelSimNao->Visible = true;
   ButCancela->Visible  = (buttons == -1);
   RichEdit->Lines->Clear();
   RichEdit->Lines->Add(txt1);
   RichEdit->Lines->Add(txt2);

   txt_concatenada = txt1 + "\n" + txt2;
   //AcertaTamanho(txt_concatenada);
   AcertaTamanho(LarguraMax(txt_concatenada));
   }

//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::Erro(AnsiString txt)
   {
   Caption            = "Erro !";
   PanelErro->Visible = true;
   PanelOK->Visible   = true;
   RichEdit->Lines->Clear();
   RichEdit->Lines->Add(txt);
   //AcertaTamanho(txt);
   AcertaTamanho(LarguraMax(txt));
   }

//---------------------------------------------------------------------------
void __fastcall TFormMsgBox::FormCloseQuery(TObject *Sender, bool &CanClose)
   {
   CanClose = true;
   if (ModalResult == mrNone) ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
int  __fastcall   TFormMsgBox::LarguraMax(AnsiString txt)
   {
   TStringList *strings = new TStringList;
   TCanvas *canvas = this->Canvas;
   int max_width = 0;
   int width = 0;

   //proteção
   if (strings == NULL) return(Width);
   ExtraiStrings(txt,"\n",strings);
   //percorre todas strings medindo
   for (int n = 0; n < strings->Count; n++)
      {
      width = canvas->TextWidth(strings->Strings[n]);
      if(width > max_width)
         {
         max_width = width;
         }
      }
   delete strings;
   return max_width;
   }

//---------------------------------------------------------------------------
//eof





