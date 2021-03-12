//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TReport.h"
#include "TMontaDados.h"
#include "TMontaRelatorioExcel.h"
#include "TMontaRelatorioTexto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTReport* __fastcall NewObjReport(VTApl *apl)
   {
   return(new TReport(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReport::TReport(VTApl *apl)
   {
   //salva ponteiro para Apl
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TReport::~TReport(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::ArquivoTexto(AnsiString FileName, TList *lisEQP, AnsiString titulo, bool append)
   {
   TMontaDados          MontaDados(apl);
   TMontaRelatorioTexto MontaRelatorioTexto;

   return(MontaRelatorioTexto.Executa(FileName, MontaDados.Executa(lisEQP), titulo, append));
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::ArquivoTexto(AnsiString FileName, TDataSet *DataSet, AnsiString titulo, bool append)
   {
   //variáveis locais
   TMontaDados          MontaDados(apl);
   TMontaRelatorioTexto MontaRelatorioTexto;

   return(MontaRelatorioTexto.Executa(FileName, MontaDados.Executa(DataSet), titulo, append));
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::ArquivoTexto(AnsiString FileName, TDBGrid *DBGrid, AnsiString titulo, bool append)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::ArquivoTexto(AnsiString FileName, TListView *ListView, AnsiString titulo, bool append)
   {
   //variáveis locais
   TMontaDados          MontaDados(apl);
   TMontaRelatorioTexto MontaRelatorioTexto;

   return(MontaRelatorioTexto.Executa(FileName, MontaDados.Executa(ListView), titulo, append));
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::ArquivoTexto(AnsiString FileName, TStringGrid *StrGrid, AnsiString titulo, bool append)
   {
   TMontaDados          MontaDados(apl);
   TMontaRelatorioTexto MontaRelatorioTexto;

   return(MontaRelatorioTexto.Executa(FileName, MontaDados.Executa(StrGrid), titulo, append));
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TList *lisEQP)
   {
   //variáveis locais
   TMontaDados          MontaDados(apl);
   TMontaRelatorioExcel MontaRelatorioExcel;

   return(MontaRelatorioExcel.Executa(FileName, Planilha, MontaDados.Executa(lisEQP)));
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TDataSet *DataSet)
   {
   //variáveis locais
   TMontaDados          MontaDados(apl);
   TMontaRelatorioExcel MontaRelatorioExcel;

   return(MontaRelatorioExcel.Executa(FileName, Planilha, MontaDados.Executa(DataSet)));
   }
//---------------------------------------------------------------------------
bool __fastcall TReport::PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TDBGrid *DBGrid)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TListView *ListView)
   {
   //variáveis locais
   TMontaDados          MontaDados(apl);
   TMontaRelatorioExcel MontaRelatorioExcel;

   return(MontaRelatorioExcel.Executa(FileName, Planilha, MontaDados.Executa(ListView)));
   }

//---------------------------------------------------------------------------
bool __fastcall TReport::PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TStringGrid *StrGrid)
   {
   //variáveis locais
   TMontaDados          MontaDados(apl);
   TMontaRelatorioExcel MontaRelatorioExcel;

   return(MontaRelatorioExcel.Executa(FileName, Planilha, MontaDados.Executa(StrGrid)));
   }

//---------------------------------------------------------------------------
//eof


