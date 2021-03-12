//---------------------------------------------------------------------------
#ifndef VTMontaRelatorioH
#define VTMontaRelatorioH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTDados;

//---------------------------------------------------------------------------
class VTMontaRelatorio
   {
   public:
						  __fastcall  VTMontaRelatorio(void) {};
      virtual       __fastcall ~VTMontaRelatorio(void) {};
		virtual  bool __fastcall  Executa(AnsiString FileName, VTDados *Dados, AnsiString titulo, bool append) = 0;
		virtual  bool __fastcall  Executa(AnsiString FileName, AnsiString PlanilhaName, VTDados *Dados) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto VTExporta
//---------------------------------------------------------------------------
VTMontaRelatorio* __fastcall NewObjMontaRelatorioExcel(void);
VTMontaRelatorio* __fastcall NewObjMontaRelatorioTexto(void);

//---------------------------------------------------------------------------
#endif
//eof


