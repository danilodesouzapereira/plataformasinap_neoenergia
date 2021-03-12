//---------------------------------------------------------------------------
#ifndef TDadosH
#define TDadosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDados.h"

//---------------------------------------------------------------------------
class TDados : public VTDados
   {
   public:  // Métodos
                   __fastcall  TDados(void);
                   __fastcall ~TDados(void);
		void         __fastcall  Clear(void);
		TStringList* __fastcall  InsereStringList(void);
      TList*       __fastcall  LisStringList(void);

   protected:
      AnsiString __fastcall PM_GetCelula(int ind_reg, int ind_col);
      void       __fastcall PM_SetCelula(int ind_reg, int ind_col, AnsiString valor);

   private:  // Métodos

   private:  // Dados locais
      TList *LisStrList;
   };

#endif
//---------------------------------------------------------------------------
//eof


