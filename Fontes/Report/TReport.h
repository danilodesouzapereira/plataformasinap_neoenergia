//---------------------------------------------------------------------------
#ifndef TReportH
#define TReportH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReport.h"

//---------------------------------------------------------------------------
class TReport : public VTReport
   {
   public:  // Métodos
           __fastcall  TReport(VTApl *apl);
           __fastcall ~TReport(void);
      //arquivo texto
		bool __fastcall  ArquivoTexto(AnsiString FileName, TList       *lisEQP,   AnsiString titulo, bool append);
		bool __fastcall  ArquivoTexto(AnsiString FileName, TDataSet    *DataSet,  AnsiString titulo, bool append);
      bool __fastcall  ArquivoTexto(AnsiString FileName, TDBGrid     *DBGrid,   AnsiString titulo, bool append);
		bool __fastcall  ArquivoTexto(AnsiString FileName, TListView   *ListView, AnsiString titulo, bool append);
		bool __fastcall  ArquivoTexto(AnsiString FileName, TStringGrid *StrGrid,  AnsiString titulo, bool append);
      //Excel
		bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TList       *lisEQP);
		bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TDataSet    *DataSet);
      bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TDBGrid     *DBGrid);
		bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TListView   *ListView);
		bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TStringGrid *StrGrid);

   private:  // Métodos

   private:  // Objetos externos
      VTApl         *apl;

   private:  // Dados locais
   };

#endif
//---------------------------------------------------------------------------
//eof


