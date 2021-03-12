//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormProg.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormProg::TFormProg(TComponent* Owner)
   : TForm(Owner)
   {
	cancelado = false;
   }
//---------------------------------------------------------------------------
__fastcall TFormProg::~TFormProg(void)
   {
   //
	}
//---------------------------------------------------------------------------
bool __fastcall TFormProg::Cancelado(void)
   {
   //retorna variável par ainformar cancelamento
   return cancelado;
   }
//---------------------------------------------------------------------------
void __fastcall TFormProg::FormClose(TObject *Sender,
      TCloseAction &Action)
   {
   cancelado = false;
   }
//---------------------------------------------------------------------------
void __fastcall TFormProg::Hide(void)
   {
	TForm::Hide();
   }
//-----------------------------------------------------------------------------
void __fastcall TFormProg::Progresso(void)
   {
	Progresso(pbar->Position + 1);
   }
//-----------------------------------------------------------------------------
void __fastcall TFormProg::Progresso(int posicao)
   {
   //variáveis locais
   int porcentual;
   int delta = pbar->Max - pbar->Min;
   //proteção
   if(delta == 0) delta = 1;
   //atualiza Barra de progresso
	pbar->Position = posicao;
   //atualiza Label de porcentagem
   porcentual = 100 * (posicao - pbar->Min)/(delta);
   lblPorc->Caption = IntToStr(porcentual) + " %";
   lblPorc->Refresh();
	}
//-----------------------------------------------------------------------------
void __fastcall TFormProg::Progresso(AnsiString txt)
   {
   lblTitulo2->Caption = txt;
   lblTitulo2->Refresh();
   }
//-----------------------------------------------------------------------------
void __fastcall TFormProg::Show(AnsiString titulo, int min, int max)
   {
	TForm::Show();

   //atualiza tela
   if (! titulo.IsEmpty())
      {//atualiza o nome da operação
      lblTitulo1->Caption = titulo;
      lblTitulo1->Refresh();
      }
   //define valores mínimo e máximo da Barra de progresso
   pbar->Min  = min;
   pbar->Max  = max;
   //inicia Barra de progresso
   Progresso(pbar->Min);
	}
//---------------------------------------------------------------------------
void __fastcall TFormProg::butCancelClick(TObject *Sender)
   {
   butCancel->Enabled = false;
   //verifica os clicks repetidos
	if (cancelado == true)
      {
      butCancel->Enabled = true;
      return;
      }     

   //confirmação de cancelamento
	if(Confirma("Socilitação de cancelamento.\n"
               "Deseja realmente cancelar o processo ?","") == ID_YES)
      {
      //cancela processo
      cancelado = true;
      }
   butCancel->Enabled = true;
   }
//---------------------------------------------------------------------------


