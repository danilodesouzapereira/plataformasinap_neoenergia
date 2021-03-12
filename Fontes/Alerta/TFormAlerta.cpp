//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAlerta.h"
#include "..\Apl\VTApl.h"
#include "VTAlerta.h"
#include "..\Diretorio\VTPath.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAlerta::TFormAlerta(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   }

//---------------------------------------------------------------------------
__fastcall TFormAlerta::~TFormAlerta(void)
   {
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18599986
void __fastcall TFormAlerta::ActionClearExecute(TObject *Sender)
{
    // variáveis locais
    VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

    //descarta todas as mensagens de alerta
    RichEdit->Lines->Clear();
    if(alerta)
        alerta->Clear(true);
    //fecha o form pois não há mais nada a fazer
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormAlerta::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlerta::ActionSalvaExecute(TObject *Sender)
   {
   //variáveis locais
   AnsiString filename;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //configura SaveDialog
   if (SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = path->DirExporta();
   if (SaveDialog->FileName.IsEmpty())   SaveDialog->FileName   = "Alertas.txt";
   //FKM - 2015.10.08 Esconde este form
   this->Hide();
   //exibe janela p/ usuário selecionar o arquivo
   if (SaveDialog->Execute())
      {
		RichEdit->Lines->SaveToFile(SaveDialog->FileName);
      if (FileExists(SaveDialog->FileName))
         {
         filename = ExtractFileName( SaveDialog->FileName);
         Aviso("O relatório foi gravado corretamente no arquivo " + AnsiQuotedStr(filename, '\''));
         }
      else
         {
         Erro("Erro na gravação do relatório");
         }
      }
   //FKM - 2015.10.08 Mostra este form
   this->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlerta::ExibeTexto(TStrings *Lines)
   {
   RichEdit->Lines->Clear();
   RichEdit->Lines->AddStrings(Lines);
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAlerta::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
//eof

