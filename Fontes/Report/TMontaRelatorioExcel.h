//---------------------------------------------------------------------------
#ifndef TMontaRelatorioExcelH
#define TMontaRelatorioExcelH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaRelatorio.h"

//---------------------------------------------------------------------------
class TMontaRelatorioExcel : public VTMontaRelatorio
   {
   public:  // Métodos
           __fastcall  TMontaRelatorioExcel(void);
           __fastcall ~TMontaRelatorioExcel(void);
		bool __fastcall  Executa(AnsiString FileName, VTDados *Dados, AnsiString titulo, bool append);
		bool __fastcall  Executa(AnsiString FileName, AnsiString PlanilhaName, VTDados *Dados);

	private:  // Métodos
		void    		__fastcall AjustarTextoPlanilha(Variant vplanilha);
		void    		__fastcall FormataPlanilha(Variant planilha);
		Variant 		__fastcall InserePlanilha(AnsiString FileName, AnsiString  PlanilhaName);
		bool    		__fastcall InsereTexto(AnsiString texto, Variant v1, TColor color, int Fila,
													  int Coluna, int PosHor, int PosVer, double tamanho,
													  bool Sombreado, bool tabelado, TColor colorInter);
		bool    		__fastcall LimpaPlaninhas(Variant Worksheets);
		bool    		__fastcall PreenchePlanilha(Variant planilha, VTDados *dados);
		AnsiString  __fastcall RetornaNomePlanilha(AnsiString PlanilhaName, Variant Worksheets);
		bool    		__fastcall SalvaArquivo(AnsiString  FileName);

	private:  // Dados locais
		Variant   Excel;
   };

#endif
//---------------------------------------------------------------------------
//eof


