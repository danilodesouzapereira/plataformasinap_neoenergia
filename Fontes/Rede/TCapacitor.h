//---------------------------------------------------------------------------
#ifndef TCapacitorH
#define TCapacitorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCapacitor.h"

//---------------------------------------------------------------------------
class TCapacitor : public VTCapacitor
   {
   public:
                   __fastcall TCapacitor(int num_pat);
                   __fastcall ~TCapacitor(void);
      VTCapacitor* __fastcall Clone(void);
		void         __fastcall CopiaAtributosDe(VTEqpto &ref);
		void         __fastcall DefineCurva(VTCurva *curva);
		VTCurva*     __fastcall ExisteCurva(void);
		int          __fastcall Tipo(void);
      AnsiString   __fastcall TipoAsString(void);

   private: //métodos acessados via property
      AnsiString  __fastcall PM_GetPadrao(void);
      double      __fastcall PM_GetQ_mvar(void);
      void        __fastcall PM_SetPadrao(AnsiString padrao);

   private:
      AnsiString padrao;
   };

//---------------------------------------------------------------------------
#endif
//eof

 