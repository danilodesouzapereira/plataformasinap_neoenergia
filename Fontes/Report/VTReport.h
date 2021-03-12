//---------------------------------------------------------------------------
#ifndef VTReportH
#define VTReportH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTReport : public TObject
   {
   public:
                    __fastcall  VTReport(void) {};
      virtual       __fastcall ~VTReport(void) {};
      //arquivo texto
      virtual  bool __fastcall  ArquivoTexto(AnsiString FileName, TList       *lisEQP,   AnsiString titulo="", bool append=false) = 0;
      virtual  bool __fastcall  ArquivoTexto(AnsiString FileName, TDataSet    *DataSet,  AnsiString titulo="", bool append=false) = 0;
      virtual  bool __fastcall  ArquivoTexto(AnsiString FileName, TDBGrid     *DBGrid,   AnsiString titulo="", bool append=false) = 0;
      virtual  bool __fastcall  ArquivoTexto(AnsiString FileName, TListView   *ListView, AnsiString titulo="", bool append=false) = 0;
      virtual  bool __fastcall  ArquivoTexto(AnsiString FileName, TStringGrid *StrGrid,  AnsiString titulo="", bool append=false) = 0;
      //Excel
      virtual  bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TList       *lisEQP)   = 0;
      virtual  bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TDataSet    *DataSet)  = 0;
      virtual  bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TDBGrid     *DBGrid)   = 0;
      virtual  bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TListView   *ListView) = 0;
      virtual  bool __fastcall  PlanilhaExcel(AnsiString FileName, AnsiString Planilha, TStringGrid *StrGrid)  = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto VTExporta
//---------------------------------------------------------------------------
VTReport* __fastcall NewObjReport(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


