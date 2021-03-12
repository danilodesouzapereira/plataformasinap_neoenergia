//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormZdef.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormZdef::TFormZdef(TComponent* Owner)
   : TForm(Owner)
   {
   //inicia impedâncias de defeito com valores nulos
   Zdef.Zdef3F_ff.r  = Zdef.Zdef3F_ff.x  = 0;
   Zdef.Zdef2F_ff.r  = Zdef.Zdef2F_ff.x  = 0;
   Zdef.Zdef2FT_ff.r = Zdef.Zdef2FT_ff.x = 0;
   Zdef.Zdef2FT_ft.r = Zdef.Zdef2FT_ft.x = 0;
   Zdef.Zdef1FT_ft.r = Zdef.Zdef1FT_ft.x = 0;
   //exibe dados de impedância
   SGridInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormZdef::~TFormZdef(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormZdef::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZdef::ActionConfirmaExecute(TObject *Sender)
   {
   //lê e valida a impedância de defeito
   if (! LeDados()) return;
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZdef::DefineZdef(STR_ZDef &Zdef)
   {
   //copia valores da estrutura
   this->Zdef = Zdef;
   //exibe dados de impedância
   SGridInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormZdef::LeZdef(STR_ZDef &Zdef)
   {
   //copia valores da estrutura
   Zdef = this->Zdef;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormZdef::LeDados(void)
   {
   //variáveis locais
   strIMP *z_def[] = {&Zdef.Zdef3F_ff, &Zdef.Zdef2F_ff, &Zdef.Zdef2FT_ff,
                      &Zdef.Zdef2FT_ft, &Zdef.Zdef1FT_ft};

   try{//lê valores das impedâncias dos defeitos
      for (int n = 0, nr = 1; n < 5; n++, nr++)
         {
         z_def[n]->r = StrToDouble(SGridZdef->Cells[2][nr]);
         z_def[n]->x = StrToDouble(SGridZdef->Cells[3][nr]);
         }
      }catch(Exception &e)
         {
         Erro("Valor de impedância de defeito inválido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZdef::SGridInicia(void)
   {
   //variáveis locais
   char   *col_tit[]  = {"Defeito", "Impedância", "R (ohm)", "X (ohm)"};
   char   *tipo_def[] = {"3F", "2F", "2FT", "2FT", "1FT"};
   char   *tipo_imp[] = {"fase/fase", "fase/fase", "fase/fase", "fase/terra", "fase/terra"};
   strIMP *z_def[] = {&Zdef.Zdef3F_ff, &Zdef.Zdef2F_ff, &Zdef.Zdef2FT_ff,
                      &Zdef.Zdef2FT_ft, &Zdef.Zdef1FT_ft};

   //redimensiona SGrid
   SGridZdef->RowCount = 7;
   SGridZdef->ColCount = 4;
   //preenche títulos das colunas
   for (int nc = 0; nc < SGridZdef->ColCount; nc++) SGridZdef->Cells[nc][0] = col_tit[nc];
   //preenche valores das impedâncias dos defeitos
   for (int n = 0, nr = 1; n < 5; n++, nr++)
      {
      SGridZdef->Cells[0][nr] = tipo_def[n];
      SGridZdef->Cells[1][nr] = tipo_imp[n];
      SGridZdef->Cells[2][nr] = DoubleToStr("%2.1f", z_def[n]->r);
      SGridZdef->Cells[3][nr] = DoubleToStr("%2.1f", z_def[n]->x);
      }
   }

//---------------------------------------------------------------------------
//eof

