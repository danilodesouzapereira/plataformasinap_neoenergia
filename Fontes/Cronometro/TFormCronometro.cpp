//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCronometro.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCronometro::TFormCronometro(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TFormCronometro::~TFormCronometro(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormCronometro::Insere(AnsiString codigo, AnsiString descricao,
                                        int acionamentos, int mseg, double perc)
   {
   //variáveis locais
   TListItem  *item;
   AnsiString txt;

   //cria um novo ListItem
   item = lview->Items->Add();
   item->Caption = codigo;
   item->SubItems->Add(IntToStr(acionamentos));
   item->SubItems->Add(IntToStr(mseg));
   txt.sprintf("%3.2f", 100. * perc);
   item->SubItems->Add(txt);
   item->SubItems->Add(descricao);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCronometro::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCronometro::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCronometro::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof

