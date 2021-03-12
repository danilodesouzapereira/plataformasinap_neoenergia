//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRelTexto.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRelTexto::TFormRelTexto(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //define Parent
   Parent = parent;
   }

//---------------------------------------------------------------------------
__fastcall TFormRelTexto::~TFormRelTexto(void)
   {
   //nada a fazer
   }
//---------------------------------------------------------------------------
void __fastcall TFormRelTexto::ActionCloseExecute(TObject *Sender)
   {
   Close();
   }
//---------------------------------------------------------------------------
void __fastcall TFormRelTexto::ActionExportaExecute(TObject *Sender)
   {
   ExportaRelatorio();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTexto::Atualiza(AnsiString arq_texto)
   {
   //verifica se existe o arquivo
   if (FileExists(arq_texto))
      {//carrega conteúdo do arquivo
      RichEdit->Lines->LoadFromFile(arq_texto);
      }
   else
      {//limpa RichEdit
      RichEdit->Lines->Clear();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTexto::ExportaRelatorio(void)
   {
   //variáveis locais
   AnsiString arq_ext;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //define diretório
   if (SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = path->DirExporta();
   //define nome do arquivo
   SaveDialog->FileName    = "RelCurtoSimultaneo";
   SaveDialog->FilterIndex = 1;
   //seleciona arquivo
   if (SaveDialog->Execute())
      {//inclui informação de barras de defeito

      //salva o RichEdit no arquivo
      RichEdit->Lines->SaveToFile(SaveDialog->FileName);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTexto::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
//eof

