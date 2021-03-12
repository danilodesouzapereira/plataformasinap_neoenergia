//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCriaDir.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCriaDir::TformCriaDir(TComponent* Owner, AnsiString dir, AnsiString subdir)
   : TForm(Owner)
   {
   //define diretório inicial
   dlbDir->Directory = dir;
   //define filtro
   fcbFilter->ItemIndex = 0;
   //define nome da nova rede
   edtDir->Text = subdir;
   }

//---------------------------------------------------------------------------
__fastcall TformCriaDir::~TformCriaDir(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TformCriaDir::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TformCriaDir::ActionConfirmaExecute(TObject *Sender)
   {
   //variáveis locais
   AnsiString dir;

   //determina nome do novo diretório
   dir = edtDir->Text;
   if (dir.IsEmpty())
      {
      Erro("Defina o nome da nova rede");
      return;
      }
   //posiciona o diretório destino
   SetCurrentDir(dlbDir->Directory);
   //verifica se já existe o diretório
   if (DirectoryExists(dir))
      {
      if (Confirma("Já existe a rede " + dir + " no diretório " + dlbDir->Directory,
                   "Deseja substituí-la ?") != IDYES) return;
      }
   //tenta criar o diretório
   else if (! CreateDir(dlbDir->Directory + "\\" + dir))
      {
      Erro("Não foi possível criar a rede" + dir);
      return;
      }
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TformCriaDir::dlbDirClick(TObject *Sender)
   {
   /*
   //variáveis locais
   AnsiString diretorio;

   //verifica se há um diretório selecionado
   if (dlbDir->ItemIndex < 0) return;
   //altera Directory
   //dlbDir->Directory = dlbDir->Items->Strings[dlbDir->ItemIndex];
   //usuário selecionou um diretório existente p/ salvar a rede
   edtDir->Text = dlbDir->Items->Strings[dlbDir->ItemIndex];
   //redefine diretório selecionado
   */
   }
/*
//---------------------------------------------------------------------------
void __fastcall TformCriaDir::dcbDriveChange(TObject *Sender)
   {
   dlbDir->Drive       = dcbDrive->Drive;
   flbFiles->Drive     = dcbDrive->Drive;
   flbFiles->Directory = dlbDir->Directory;
   }
*/
//---------------------------------------------------------------------------
AnsiString __fastcall TformCriaDir::Dir(void)
   {
   return (dlbDir->Directory + "\\" + edtDir->Text);
   }

//---------------------------------------------------------------------------
void __fastcall TformCriaDir::edtDirKeyPress(TObject *Sender, char &Key)
   {
   //verifica se foi digitada a tecla Enter
   if (Key == VK_RETURN) ActionConfirma->Execute();
   }

//---------------------------------------------------------------------------
//eof

