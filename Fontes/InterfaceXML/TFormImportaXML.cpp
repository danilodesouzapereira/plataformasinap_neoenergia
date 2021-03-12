//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <windows.h>
#include "TFormImportaXML.h"
#include "VTCtrlPanel.h"
#include "VTImportaXML.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormImportaXML::TFormImportaXML(TComponent *Owner, VTApl *apl_owner, AnsiString arq_xml)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   apl = apl_owner;
   //cria objetos
   ctrlpanel  = NewObjCtrlPanel();
   ImportaXML = NewObjImportaXML(apl);
   //verifica se o arquivo foi definido
   if ((arq_xml.IsEmpty())||(! FileExists(arq_xml)))
      {//seleciona um arquivo
      SelecionaArquivo();
      }
   else
      {//configura OpenDialog
      OpenDialog->InitialDir = ExtractFilePath(arq_xml);
      OpenDialog->FileName   = arq_xml;
      //exibe dados do arquivo
      ExibeArquivo();
      }
   }

//---------------------------------------------------------------------------
__fastcall TFormImportaXML::~TFormImportaXML(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos
   if (ImportaXML) {delete ImportaXML; ImportaXML = NULL;}
   if (ctrlpanel)  {delete ctrlpanel;  ctrlpanel  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::ActionAvaliaExecute(TObject *Sender)
   {
   //vari�veis locais
   bool         sucesso;
   AnsiString   arq_xml;

   //verifica se o RichEdit est� vazio
   if (redArq->Lines->Count == 0)
      {
      Aviso("Selecione arquivo XML");
      return;
      }
   //limpa redArq
   redArq->Lines->Clear();
   //define nome do arquivo XML
   arq_xml = OpenDialog->FileName;
   //executa avaliacao da estrutura do arquivos XML
   sucesso = ImportaXML->ExecutaAvaliacao(arq_xml);
   //mostra relat�rio de Log
   redArq->Lines->Clear();
   ImportaXML->RelatorioLog(redArq->Lines);
   //verifica se houve erro
   if (sucesso)
      {//gera aviso p/ o usu�rio
      Aviso("A estrutura do arquivo XML est� correta.");
      }
   else
      {//gera aviso p/ o usu�rio
      Aviso("A estrutura do arquivo XML n�o est� correta.\n"
            "Verifique os problemas no relat�rio.");
      }
   }


//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::ActionCancelaExecute(TObject *Sender)
   {
   //cancela importa��o
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::ActionConfirmaExecute(TObject *Sender)
   {
   //importa��o feita c/ sucesso
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::ActionImportaExecute(TObject *Sender)
   {
   //vari�veis locais
   bool         sucesso;
   AnsiString   arq_xml;
   VTRedes     *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o RichEdit est� vazio
   if (redArq->Lines->Count == 0)
      {
      Aviso("Selecione arquivo XML");
      return;
      }
   //redefine separador decimal do Windows
   if (! ctrlpanel->DefineSeperadorDecimal('.'))
      {
      Aviso("O Windows deve estar configurado para utilizar 'ponto' como separador decimal");
      return;
      }
   //limpa redArq
   redArq->Lines->Clear();
   //define nome do arquivo XML
   arq_xml = OpenDialog->FileName;
   //executa importa��o dos dados
   sucesso = ImportaXML->ExecutaImportacao(arq_xml);
   //mostra relat�rio de Log
   redArq->Lines->Clear();
   ImportaXML->RelatorioLog(redArq->Lines);
   //verifica se houve erro
   if (sucesso)
      {//associa ButFecha c/ Action Confirma
      ButFecha->Action = ActionConfirma;
      //gera aviso p/ o usu�rio
      Aviso("A importa��o do arquivo foi conclu�da com sucesso.\n"
            "Foram criadas " + IntToStr((redes->LisRede())->Count) + " redes.");
      }
   else
      {//verifica se foram criadas redes
      if ((redes->LisRede())->Count == 0)
         {//associa ButFecha c/ ActionCancela
         ButFecha->Action = ActionCancela;
         //gera aviso p/ o usu�rio
         Aviso("Exist�ncia de erros na importa��o do arquivo.\n"
               "Verifique os problemas no relat�rio.");
         }
      else
         {//associa ButFecha c/ ActionConfirma
         ButFecha->Action = ActionConfirma;
         //gera aviso p/ o usu�rio
         Aviso("Exist�ncia de erros na importa��o do arquivo.\n"
               "Foram criadas " + IntToStr((redes->LisRede())->Count) + " redes."
               "Verifique os problemas no relat�rio.");
         }
      }
   //restaura separador decimal originalmente configurado no Windows
   ctrlpanel->RestauraSeperadorDecimal();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::ActionSelArqExecute(TObject *Sender)
   {
   //seleciona arquivo XML para importa��o
   SelecionaArquivo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::AlteraSeparadorDecimal(AnsiString arq_xml)
   {
   //vari�veis locais
   AnsiString txt;
   byte       dec_sep, sep_txt;

   //esconde RichEdit
   redArq->Hide();
   dec_sep = FormatSettings.DecimalSeparator;
   if      (dec_sep == '.') {sep_txt = ',';}
   else if (dec_sep == ',') {sep_txt = '.';}
   else                     {return;}
   for (int nl = 0; nl < redArq->Lines->Count; nl++)
      {
      txt = redArq->Lines->Strings[nl];
      for (int i = 1; i <= txt.Length(); i++)
         {
         if (txt[i] == sep_txt) txt[i] = dec_sep;
         }
      redArq->Lines->Strings[nl] = txt;
      }
   //atualiza arquivo
   redArq->Lines->SaveToFile(arq_xml);
   //exibe RichEdit
   redArq->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::ExibeArquivo(void)
   {
   StatusBar->SimpleText = OpenDialog->FileName;
   //exibe conte�do do arquivo no RichEdit
   redArq->Lines->LoadFromFile(OpenDialog->FileName);
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaXML::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormImportaXML::SelecionaArquivo(void)
   {
   //vari�veis locais
   bool    sucesso = false;
   bool    form_visible = Visible;
   VTPath *path         = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   //seleciona diret�rio inicial
   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirImporta();
   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirBase;
   //seleciona arquivo TXT
   if (OpenDialog->Execute())
      {//usu�rio selecionou um arquivo: exibe nome do arquivo no Status bar
      ExibeArquivo();
      sucesso = true;
      }
   //reapresenta este Form
   if (form_visible) Visible = true;
   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof

