//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLog2.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLog2::TFormLog2(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva objeto externo
   this->apl = apl;
   //cria objetos
   lines  = new TStringList();
   campos = new TStringList();
   ListViewSort.ColumnIndex    = 0;
   ListViewSort.OrdemCrescente = true;
   //inicia exibição de GBoxConfigura
   ActionConfigura->Execute();
   }

//---------------------------------------------------------------------------
__fastcall TFormLog2::~TFormLog2(void)
   {
   //destrói objetos
   if (lines ) {delete lines ; lines  = NULL;}
   if (campos) {delete campos; campos = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ActionConfiguraExecute(TObject *Sender)
   {
   //exibe/esconde GBoxField p/ habilitar/desabilitar atributos
   GBoxConfigura->Visible = ButConfigura->Down;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ActionExportaExcelExecute(TObject *Sender)
   {
   //variáveis locais
   VTReport *report;
   VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   try{//seleciona arquivo de saida
      SaveDialog->DefaultExt = "xls";
      SaveDialog->FileName = path->DirExporta() + "\\Log.xls";
      if (SaveDialog->Execute())
         {//cria objeto Report
         report = DLL_NewObjReport(apl);
         report->PlanilhaExcel(SaveDialog->FileName, "Log", LView);
         //destrói objeto
         DLL_DeleteObjReport(report);
         }
      }catch(Exception &e)
         {
         Aviso("Não foi possível exportar o relatóio p/ o arquivo " + SaveDialog->FileName);
         }
   //reapresenta este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ActionExportaTxtExecute(TObject *Sender)
   {
   //variáveis locais
   VTReport *report;
   VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   try{//seleciona arquivo de saida
      SaveDialog->DefaultExt = "txt";
      SaveDialog->FileName = path->DirExporta() + "\\Log.Txt";
      if (SaveDialog->Execute())
         {//cria objeto Report
         report = DLL_NewObjReport(apl);
         report->ArquivoTexto(SaveDialog->FileName, LView);
         //destrói objeto
         DLL_DeleteObjReport(report);
         }
      }catch(Exception &e)
         {
         Aviso("Não foi possível exportar o relatóio p/ o arquivo " + SaveDialog->FileName);
         }
   //reapresenta este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ActionFechaExecute(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ExibeLog(AnsiString arq_log, TStrings *colunas)
   {
   //inicia StrList com o arquivo de log
   lines->LoadFromFile(arq_log);
   //cria colunas do LView
   LViewConfigura(colunas);
   //inicia LView
   LViewInicia(lines);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ExibeLog(TStrings *Lines, TStrings *colunas)
   {
   //cria colunas do LView
   LViewConfigura(colunas);
   //inicia LView
   LViewInicia(Lines);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::ExtraiCampos(AnsiString txt, TStrings *campos)
   {
   //variáveis locais
	//TSysCharSet Separators;
	//TSysCharSet WhiteSpace;
	//WideString  ws_txt = txt;

   //inicia conjunto de caracteres separadores
	//Separators << ';';
   //inicia conjunto de caracteres de espaçamento
   //WhiteSpace << ' ';
   //reinicia StrinList campos
   campos->Clear();
   //separa campos do txt //DVK 2014.12.10
//	ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
	ExtraiStrings(txt, ";", campos);
   //elimina espaços em branco dos strings
   for (int n = 0; n < campos->Count; n++)
      {
      campos->Strings[n] = campos->Strings[n].Trim();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (ListViewSort.ColumnIndex == Column->Index)
      {//inverte ordem
      ListViewSort.OrdemCrescente  = ! ListViewSort.OrdemCrescente;
      }
   else
      {//redefine índice da coluna usada p/ ordenação
      ListViewSort.ColumnIndex = Column->Index;
      }
   //força reordenação
   LView->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::LViewCompare(TObject *Sender, TListItem *Item1,
      TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
   int  index = ListViewSort.ColumnIndex-1;

   switch(ListViewSort.ColumnIndex)
      {
      case 0:  //caption
         Compare = CompareText(Item1->Caption,Item2->Caption);
         break;
      default: //substring
         Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      }
   //verifica se ordem crescente/decrescente
   if (! ListViewSort.OrdemCrescente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::LViewConfigura(TStrings *colunas)
   {
   //variáveis locais
   TListColumn  *ListColumn;

   LView->Columns->Clear();
   for (int n = 0; n < colunas->Count; n++)
      {//insere ListColumn
      ListColumn            = LView->Columns->Add();
      ListColumn->Caption   = colunas->Strings[n];
      ListColumn->Alignment = taLeftJustify;
      ListColumn->Width     = 80;
      ListColumn->Index     = n;
      }
   //define a última ListColumn como autosize
   ListColumn->Width    = 350;
   ListColumn->AutoSize = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog2::LViewInicia(TStrings *lines)
   {
   //variáveis locais
   TListItem *Item;

   //reinicia LView
   LView->Items->Clear();
   //loop p/ todas as linhas do arquivo de log
   for (int n = 0; n < lines->Count; n++)
      {//extrai campos da linha
      ExtraiCampos(lines->Strings[n], campos);
      if (campos->Count == 0) continue;
      if (campos->Count == 1)
         {//insere TListItem
         Item = LView->Items->Add();
         Item->Caption = "";
         for (int i = 1; i < LView->Columns->Count-1; i++)
            {
            Item->SubItems->Add("");
            }
         Item->SubItems->Add(campos->Strings[0]);
         }
      else if (campos->Count == LView->Columns->Count)
         {//insere TListItem
         Item = LView->Items->Add();
         Item->Caption = campos->Strings[0];
         for (int i = 1; i < LView->Columns->Count; i++)
            {
            Item->SubItems->Add(campos->Strings[i]);
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof

