//---------------------------------------------------------------------------
#ifndef TMontaRelatorioTextoH
#define TMontaRelatorioTextoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaRelatorio.h"

//---------------------------------------------------------------------------
class TMontaRelatorioTexto : public VTMontaRelatorio
   {
   public:  // Métodos
           __fastcall  TMontaRelatorioTexto(void);
           __fastcall ~TMontaRelatorioTexto(void);
		bool __fastcall  Executa(AnsiString FileName, VTDados *Dados, AnsiString titulo, bool append);
      bool __fastcall  Executa(AnsiString FileName, AnsiString PlanilhaName, VTDados *Dados);

   private:  // Métodos

   private:  // Dados locais
   };

#endif
//---------------------------------------------------------------------------
//eof


