//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaRelatorioExcel.h"
#include "VTDados.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMontaRelatorio* __fastcall NewObjMontaRelatorioExcel(void)
   {
   return(new TMontaRelatorioExcel());
   }

//---------------------------------------------------------------------------
__fastcall TMontaRelatorioExcel::TMontaRelatorioExcel(void)
   {
   //variáveis locais
   Variant v0, v1, v2;

	try{
      Excel  = Variant::CreateObject("Excel.Application");
	   } catch(Exception &e)
	      {
		   Aviso("Não foi possível criar o relatorio: verifique se o Office está instalado");
		   return;
         }
	//desabilita a visualização da planila
	Excel.OlePropertySet("Visible", false);
   // cria as planilas;
   v0 = Excel.OlePropertyGet("Workbooks");
   v0.OleProcedure("Add");
   v1 = v0.OlePropertyGet("Item",1);
   v2 = v1.OlePropertyGet("Worksheets") ;
   }
//---------------------------------------------------------------------------
__fastcall TMontaRelatorioExcel::~TMontaRelatorioExcel(void)
	{
	//nada a fazer

  }
//---------------------------------------------------------------------------
void  __fastcall TMontaRelatorioExcel::AjustarTextoPlanilha(Variant vplanilha)
{
		// ajusta os elementos da planilha
		Variant UsedRange, ExcelColumns;
		int   ColumnsCount;
		UsedRange=vplanilha.OlePropertyGet("UsedRange");
		ExcelColumns=UsedRange.OlePropertyGet("Columns");
		ColumnsCount=ExcelColumns.OlePropertyGet("Count");

		Variant Column;
		Variant EntireColumn;
		for(int i=0; i < ColumnsCount ; i++)
		{

			Column=ExcelColumns.OlePropertyGet("Item", i+1);
			EntireColumn=Column.OlePropertyGet("EntireColumn");
			EntireColumn.OleFunction("Autofit");
		}
}
//---------------------------------------------------------------------------
bool __fastcall TMontaRelatorioExcel::Executa(AnsiString FileName, VTDados *Dados, AnsiString titulo, bool append)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRelatorioExcel::Executa(AnsiString FileName, AnsiString PlanilhaName, VTDados *Dados)
   {
   //variáveis locais
	Variant planilha;
	//cria planilha
	try{
	planilha = InserePlanilha(FileName, PlanilhaName);
	//salva dados na planilha
	PreenchePlanilha(planilha, Dados);
	//grava arquivo
	SalvaArquivo(FileName);
	}
	catch(...)
	{
		//quite the excel application
		Excel.OleProcedure("Quit");
		//unassign variable
		Excel = Unassigned;
		return false;
	}
   return true;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaRelatorioExcel::FormataPlanilha(Variant planilha)
   {
   //variáveis locais
	int     ColumnsCount;
	Variant UsedRange, ExcelColumns;
	Variant Column;
	Variant EntireColumn;

	//ajusta os elementos da planilha
	UsedRange    = planilha.OlePropertyGet("UsedRange");
	ExcelColumns = UsedRange.OlePropertyGet("Columns");
	ColumnsCount = ExcelColumns.OlePropertyGet("Count");
   //loop p/ todas colunas
	for(int n = 0; n < ColumnsCount; n++)
	   {
      Column       = ExcelColumns.OlePropertyGet("Item", n+1);
      EntireColumn = Column.OlePropertyGet("EntireColumn");
      EntireColumn.OleFunction("Autofit");
      }
   }

//---------------------------------------------------------------------------
Variant __fastcall TMontaRelatorioExcel::InserePlanilha(AnsiString FileName, AnsiString  PlanilhaName)
   {
   //variáveis locais
	Variant   Worksheets, xWorksheet, v0, v1, v2, v3, Vplanilha;
	int NPlanilhas;
	int xS_Count;
	AnsiString xName;

	v0 = Excel.OlePropertyGet("Workbooks");
	if(FileExists(FileName.Trim()))
	{
	 v2 = v0.OleFunction ("Open", FileName.c_str());
	 Worksheets = v2.OlePropertyGet("Worksheets");

	 PlanilhaName = RetornaNomePlanilha(PlanilhaName, Worksheets);

	 Worksheets.OleProcedure("Add");
	 Worksheets.OlePropertyGet("Item",1).OlePropertySet("Name",PlanilhaName.c_str());
	 v3 = Worksheets.OlePropertyGet("Item",1);
	}
	else
	{
		v1 = v0.OlePropertyGet("Item",1);
		Worksheets = v1.OlePropertyGet("Worksheets");
		// limpa as planinhas existentes
		LimpaPlaninhas(Worksheets);
		//xWorksheet.OleFunction      ("Delete");

		//
		Worksheets.OleProcedure("Add");
		Worksheets.OlePropertyGet("Item",1).OlePropertySet("Name",PlanilhaName.c_str());
		v3 = Worksheets.OlePropertyGet("Item",1);
   }




	return v3;
   }

//---------------------------------------------------------------------------
bool  __fastcall  TMontaRelatorioExcel::InsereTexto(AnsiString texto, Variant v1, TColor color, int Fila,
                                                    int Coluna, int PosHor, int PosVer, double tamanho,
                                                    bool Sombreado, bool tabelado, TColor colorInter)
   {
   //variáveis locais
   Variant v2;

   try{
   v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("Value",texto.c_str());
   v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("HorizontalAlignment",PosHor);
   v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("VerticalAlignment", PosVer);  // direita, centro, esquerda
   if(tabelado)
   v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertyGet("Borders").OlePropertySet("LineStyle",1);
   // muda as propiedades
   v2=v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila,Coluna);
   v2.OlePropertyGet("Font").OlePropertySet("Color",color);
   v2.OlePropertyGet("Font").OlePropertySet("Bold",Sombreado);
   v2.OlePropertyGet("Font").OlePropertySet("Size",tamanho);
   v2.OlePropertyGet("Font").OlePropertySet("Name","Arial");

   //v2.OlePropertyGet("Interior").OlePropertySet("ColorIndex",colorInter);
   v2.OlePropertyGet("Interior").OlePropertySet("Color",colorInter);

   // sombrea a celda;
   //v2.OlePropertyGet("Interior").OlePropertySet("ColorIndex",9-3*1);
   // permite o alinhamento central horizontal o vertical
   //Cell.OlePropertySet("HorizontalAlignment",xlCenter)
   //desenha as linhas
   //borders = cell.OlePropertyGet("Borders");
   //borders.OlePropertySet("LineStyle",1);
   }catch(Exception &e)
      {
      return(false);
      }
   return true;
   }

//---------------------------------------------------------------------------
bool    		__fastcall TMontaRelatorioExcel::LimpaPlaninhas(Variant Worksheets)
{
	int  NPlanilhas;
	int xS_Count;
	AnsiString xName;
	Variant xWorksheet;

	NPlanilhas = Worksheets.OlePropertyGet ("Count");
	// atualiza o nome da planilha.. obs: Não pode existir duas planilhas com o mesmo nome
	for(int i = NPlanilhas ; i > 1  ; i--)
	{
		xS_Count = i;
		xWorksheet  = Worksheets.OlePropertyGet("Item", &xS_Count);
		xWorksheet.OleFunction("Delete");
	}

   return true;
}
//---------------------------------------------------------------------------
bool __fastcall TMontaRelatorioExcel::PreenchePlanilha(Variant planilha, VTDados *Dados)
   {
   //variáveis locais
	int         posx, posy;
	AnsiString  texto;
	TColor      colorInt;
   TList       *LisStringList;
   TStringList *StrList;

   //define cor
	colorInt = clNone;
   //loop p/ todos registros
   //loop p/ todos registros
   LisStringList = Dados->LisStringList();
	for(int nl = 0; nl < LisStringList->Count; nl++)
	   {
      StrList = (TStringList*)LisStringList->Items[nl];
		for(int nc = 0; nc < StrList->Count; nc++)
		   {
			texto = StrList->Strings[nc];
			posx = nl+1;
			posy = nc+1;
			InsereTexto(texto, planilha, clBlack, posx, posy,  3,2,10, true, true, colorInt);
         }
		}
  AjustarTextoPlanilha(planilha);

   return(true);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TMontaRelatorioExcel::RetornaNomePlanilha(AnsiString PlanilhaName, Variant Worksheets)
{
	int  NPlanilhas;
	int xS_Count;
	AnsiString xName;
	Variant xWorksheet;

	NPlanilhas = Worksheets.OlePropertyGet ("Count");
	// atualiza o nome da planilha.. obs: Não pode existir duas planilhas com o mesmo nome
	for(int i = 0 ; i < NPlanilhas ; i++)
	{
		xS_Count = i+1;
		xWorksheet  = Worksheets.OlePropertyGet("Item", &xS_Count);
		xName       = xWorksheet.OlePropertyGet ("Name");
		if(xName.UpperCase().Trim() == PlanilhaName.UpperCase().Trim())
		{
			PlanilhaName = PlanilhaName + "("+IntToStr(NPlanilhas)+")"; break;
		}
	}

	return PlanilhaName;
}
//---------------------------------------------------------------------------
bool __fastcall TMontaRelatorioExcel::SalvaArquivo(AnsiString  FileName)
   {
   //variáveis locais
	Variant  vWorkbook;
	Variant  vsave;

	vWorkbook = Excel.OlePropertyGet("ActiveWorkbook");

	try{
	vWorkbook.OleProcedure("SaveAs", FileName.c_str());
	}
	catch(...)
	{
		ShowMessage("Fechar o arquivo:"+FileName);
   }

   //quite the excel application
	Excel.OleProcedure("Quit");
	//unassign variable
	Excel = Unassigned;

   return(true);
   }

//---------------------------------------------------------------------------
//eof


