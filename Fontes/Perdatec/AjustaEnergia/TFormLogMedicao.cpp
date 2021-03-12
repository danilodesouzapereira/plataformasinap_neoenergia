//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormLogMedicao.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormLogMedicao::TFormLogMedicao(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl_owner;
   }

//---------------------------------------------------------------------------
__fastcall TFormLogMedicao::~TFormLogMedicao(void)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogMedicao::ActionFecharExecute(TObject *Sender)
   {
   //fecha o Form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogMedicao::ActionExportarExecute(TObject *Sender)
   {
   //variaveis locais
   AnsiString   nome_arq, titulo;
   VTReport     *report;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //cria objeto report
   report = DLL_NewObjReport(apl);
   //esconde form, para a janela Salvar Como não ficar por baixo
   Hide();
   //define pasta inicial
   SaveDialogExporta->InitialDir = path->DirExporta();
   if(SaveDialogExporta->Execute())
      {//recebe nome do arquivo a ser exportado e exporta listView
      nome_arq = SaveDialogExporta->FileName;
      titulo = "Log Medições para Perda Regulatória";
      if(report->ArquivoTexto(nome_arq, LViewLog, titulo, false))
         Aviso("A tabela foi exportada corretamente.");
      else Erro("Erro ao exportar tabela.");
      }
   //destroi report
   if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reapresenta form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogMedicao::Executa(TStringList* strDIAG_MED)
   {
   //var locais
   AnsiString  linha;
   TListItem   *item;
   TStringList *campos;

   //cria campos
   campos = new TStringList();
   //limpa log
   LViewLog->Clear();
   for(int nm = 0; nm < strDIAG_MED->Count; nm++)
      {
      linha = strDIAG_MED->Strings[nm];
      campos->Clear();
      ExtraiStrings(linha, ";", campos);
      item = LViewLog->Items->Add();
      //rede
      item->Caption = campos->Strings[0];
      //diag
      item->SubItems->Add(campos->Strings[1]);
      }
   //destroi campos
   delete campos;
   //salva log em arquivo
   SalvaLog();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogMedicao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogMedicao::FormShow(TObject *Sender)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogMedicao::SalvaLog(bool append)
   {
   //variaveis locais
   AnsiString   nome_arq;
   VTReport    *report;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //cria objeto report
   report = DLL_NewObjReport(apl);
   //exporta listView na pasta tmp
   nome_arq = path->DirTmp() + "\\LogMedicao.txt";
   report->ArquivoTexto(nome_arq, LViewLog, "", append);
   //destroi report
   if(report) {DLL_DeleteObjReport(report); report = NULL;}
   }

//---------------------------------------------------------------------------
//eof



