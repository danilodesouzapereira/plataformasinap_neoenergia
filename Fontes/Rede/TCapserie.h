//---------------------------------------------------------------------------
#ifndef TCapserieH
#define TCapserieH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCapserie.h"

//---------------------------------------------------------------------------
class TCapserie : public VTCapserie
   {
   public:
                  __fastcall TCapserie(int num_pat);
                  __fastcall ~TCapserie(void);
      VTCapserie* __fastcall Clone(void);
		void        __fastcall CopiaAtributosDe(VTEqpto &ref);
      int         __fastcall Tipo(void);
      AnsiString  __fastcall TipoAsString(void);
      void        __fastcall Z0_pu(double Sbase, double Vbase, double &r0, double &x0);
      void        __fastcall Z1_pu(double Sbase, double Vbase, double &r1, double &x1);

	protected: //métodos virtuais redefinidos
		int        __fastcall Fases(VTBarra *barra);

	};

//---------------------------------------------------------------------------
#endif
//eof

 