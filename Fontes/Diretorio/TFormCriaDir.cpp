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
   //define diret�rio inicial
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
   //vari�veis locais
   AnsiString dir;

   //determina nome do novo diret�rio
   dir = edtDir->Text;
   if (dir.IsEmpty())
      {
      Erro("Defina o nome da nova rede");
      return;
      }
   //posiciona o diret�rio destino
   SetCurrentDir(dlbDir->Directory);
   //verifica se j� existe o diret�rio
   if (DirectoryExists(dir))
      {
      if (Confirma("J� existe a rede " + dir + " no diret�rio " + dlbDir->Directory,
                   "Deseja substitu�-la ?") != IDYES) return;
      }
   //tenta criar o diret�rio
   else if (! CreateDir(dlbDir->Directory + "\\" + dir))
      {
      Erro("N�o foi poss�vel criar a rede" + dir);
      return;
      }
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TformCriaDir::dlbDirClick(TObject *Sender)
   {
   /*
   //vari�veis locais
   AnsiString diretorio;

   //verifica se h� um diret�rio selecionado
   if (dlbDir->ItemIndex < 0) return;
   //altera Directory
   //dlbDir->Directory = dlbDir->Items->Strings[dlbDir->ItemIndex];
   //usu�rio selecionou um diret�rio existente p/ salvar a rede
   edtDir->Text = dlbDir->Items->Strings[dlbDir->ItemIndex];
   //redefine diret�rio selecionado
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

