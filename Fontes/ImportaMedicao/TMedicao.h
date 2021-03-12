//---------------------------------------------------------------------------
#ifndef TMedicaoH
#define TMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMedicao.h"

//---------------------------------------------------------------------------
class TMedicao : public VTMedicao
   {
   public:
			 	 __fastcall  TMedicao(void);
			 	 __fastcall ~TMedicao(void);
	  bool       __fastcall  CurvaNula(int tipo_medicao);
	  bool       __fastcall  CurvaNulaNegativa(int tipo_medicao);
	  AnsiString __fastcall  DiagnosticoAsStr(void);
	  int    	 __fastcall  FasesMed(int ind_pat);
	  int    	 __fastcall  FasesRede(int ind_pat);
	  bool       __fastcall  FasesValidas(void);
	  double     __fastcall  MaiorErro(void);
	  void       __fastcall  Reinicia(void);
   };

//---------------------------------------------------------------------------
#endif
//eof

