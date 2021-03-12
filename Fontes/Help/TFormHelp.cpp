//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <StrUtils.hpp>
#include "TFormHelp.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormHelp::TFormHelp(TComponent *Owner, VTApl *apl_owner, AnsiString topico)
   : TForm(Owner)
   {
   //vari�veis locais
   AnsiString dirname;

   //N�O salva ponteiro para objeto Apl
   //this->apl = apl_owner;
   //salva ponteiro para objetos
   geral = (VTGeral*)apl_owner->GetObject(__classid(VTGeral));
   path  = (VTPath*)apl_owner->GetObject(__classid(VTPath));
   //posiciona o Form
   FormIniciaPosicao();
   //verifica se existe o diret�rio com Help do Sinap
   dirname = path->DirHelp() + "\\Sinap";
   if (DirectoryExists(dirname))
      {//inicia FileListBoxSinap
      FileListBoxSinap->Directory = dirname;
      //preenche LBoxSinap com nomes dos arquivos em FileListBoxSinap
      LBoxInicia(LBoxSinap, FileListBoxSinap, topico);
      }
   else
      {//desabilita TabSheetSinap
      TabSheetSinap->TabVisible = false;
      }
   //verifica se existe o diret�rio com Help do STK
   dirname = path->DirHelp() + "\\Stk";
   if (DirectoryExists(dirname))
      {//inicia FileListBoxStk
      FileListBoxStk->Directory = dirname;
      //preenche FileListBoxStk com nomes dos arquivos em FileListBoxStk
      LBoxInicia(LBoxStk, FileListBoxStk, topico);
      }
   else
      {//desabilita TabSheetStk
      TabSheetStk->TabVisible = false;
      }
   //seleciona TabSheet
   //PageControl->ActivePage = TabSheetSinap;
   //exibe t�pico indicado
   if      (LBoxSinap->ItemIndex >= 0) ExibeArquivo(LBoxSinap, FileListBoxSinap);
   else if (LBoxStk->ItemIndex   >= 0) ExibeArquivo(LBoxStk,   FileListBoxStk  );
   }

//---------------------------------------------------------------------------
__fastcall TFormHelp::~TFormHelp(void)
   {
   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::ExibeArquivo(TListBox *LBox, TFileListBox *FileLBox)
   {
   //vari�veis locais
   int        index;
   AnsiString filename;

   //prote��o: verifica se h� um Item selecionado
   if ((index = LBox->ItemIndex) < 0)
      {//esconde GBoxHelp
      GBoxHelp->Visible = false;
      return;
      }
   //atualiza e exibe GBoxHelp
   GBoxHelp->Caption = LBox->Items->Strings[index];
   GBoxHelp->Visible = true;
   //determina index do item em FileListBox
   index = (int)(LBox->Items->Objects[index]);
   //define path completo do arquivo
   filename = FileLBox->Directory + "\\" + FileLBox->Items->Strings[index];
   if (FileExists(filename))
      {//inicia WebBrowser
      WebBrowser->Navigate(filename);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::ActionCloseExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::FormIniciaPosicao(void)
   {
   //vari�veis locais
   int     height;

   //define posi��o default da janela flutuante
   janela.top    = 0;
   janela.left   = 0;
   janela.width  = Width;
   janela.height = Height;
   //l� par�metros do Form
   geral->Infoset->LoadFromFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::LBoxSinapClick(TObject *Sender)
   {
   //exibe cont�udo do arquivo selecionado
   ExibeArquivo(LBoxSinap, FileListBoxSinap);
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::LBoxStkClick(TObject *Sender)
   {
   //exibe cont�udo do arquivo selecionado
   ExibeArquivo(LBoxStk, FileListBoxStk);
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::LBoxInicia(TListBox *LBox, TFileListBox *FileLBox, AnsiString topico)
   {
   //vari�veis locais
   AnsiString text;
   int        index;
   int        index_topico = -1;

   //reinicia LBoxFile
   LBox->Clear();
   LBox->Sorted = false;
   //loop p/ todos os arquivos de FileLBox
   for (int n = 0; n < FileLBox->Items->Count; n++)
      {//monta nome a ser apresentado
      text = MontaNomeModulo(FileLBox->Items->Strings[n]);
      //verifica se � o arquivo de "Apresenta��o"
      if (text.AnsiPos("Apresenta��o") > 0)
         {//insere como primeiro item do LBox
         index = 0;
         LBox->Items->InsertObject(0, text, (TObject*)n);
         //redefine o index do t�pico a ser apresentado inicialmente
         if (index_topico >= 0) index_topico++;
         }
      else
         {//insere como �ltimo item do LBox
         index = LBox->Items->AddObject(text, (TObject*)n);
         }
      //verifica se o arquivo corresponde ao t�pico indicado
      text = FileLBox->Items->Strings[n];
      if (text.AnsiPos(topico) > 0) index_topico = index;
      }
   //pr�-seleciona o t�pico indicado, se poss�vel
   index = -1;
   if (LBox->Items->Count >= 0) index = 0;
   if (index_topico       >= 0) index = index_topico;
   LBox->ItemIndex = index;
   //ordena LBoxFile
   //LBox->Sorted = true;
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormHelp::LBoxFileInicia(AnsiString topico)
   {
   //vari�veis locais
   AnsiString text;
   int        index = -1;

   //reinicia LBoxFile
   LBoxSinap->Clear();
   LBoxSinap->Sorted = false;
   //obt�m os arquivos de Help do Sinap
   for (int n = 0; n < FileListBoxSinap->Items->Count; n++)
      {//verifica se o arquivo corresponde ao t�pico indicado
      text = FileListBoxSinap->Items->Strings[n];
      if (text.AnsiPos(topico) > 0) index = n;
      //monta nome a ser apresentado
      text = MontaNomeModulo(FileListBoxSinap->Items->Strings[n]);
      //verifica se
      LBoxSinap->Items->AddObject(text, (TObject*)n);
      }
   //pr�-seleciona o arquivo indicado
   LBoxSinap->ItemIndex = index;
   //ordena LBoxFile
   LBoxSinap->Sorted = true;
   }
*/
//---------------------------------------------------------------------------
AnsiString __fastcall TFormHelp::MontaNomeModulo(AnsiString filename)
   {
   filename = ChangeFileExt(ExtractFileName(filename), "");
   filename = AnsiReplaceStr(filename, "__", ": ");
   return(AnsiReplaceStr(filename, "_", " "));
   }

//---------------------------------------------------------------------------
void __fastcall TFormHelp::PageControlChange(TObject *Sender)
   {
   //verifica TabShhet selecionado
   if (PageControl->ActivePage == TabSheetSinap)
      {
      if ((LBoxSinap->Items->Count > 0)&&(LBoxSinap->ItemIndex < 0))
         {//seleciona primeiro Item do LBox
         LBoxSinap->ItemIndex = 0;
         }
      //exibe arquivo selecionado
      ExibeArquivo(LBoxSinap, FileListBoxSinap);
      }
   else if (PageControl->ActivePage == TabSheetStk)
      {
      if ((LBoxStk->Items->Count > 0)&&(LBoxStk->ItemIndex < 0))
         {//seleciona primeiro Item do LBox
         LBoxStk->ItemIndex = 0;
         }
      //exibe arquivo selecionado
      ExibeArquivo(LBoxStk, FileListBoxStk);
      }
   }

//---------------------------------------------------------------------------
//eof



