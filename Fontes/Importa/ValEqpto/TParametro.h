//---------------------------------------------------------------------------

#ifndef TParametroH
#define TParametroH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTParametro.h"

//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TParametro : public VTParametro
	{
	public:
					  __fastcall  TParametro(VTApl *apl);
					  __fastcall ~TParametro(void);
		AnsiString __fastcall  PM_Tipo(void);

	private: //objetos externos
		VTApl       *apl;

	private: //obj locais

	};
//---------------------------------------------------------------------------
#endif

