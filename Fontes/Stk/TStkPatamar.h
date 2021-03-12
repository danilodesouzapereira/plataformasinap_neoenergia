//---------------------------------------------------------------------------
#ifndef TStkPatamarH
#define TStkPatamarH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTStkPatamar.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;

//---------------------------------------------------------------------------
class TStkPatamar : public VTStkPatamar
	{
   public :
			  __fastcall  TStkPatamar(VTApl *apl_owner);
			  __fastcall ~TStkPatamar(void);
      bool __fastcall  IniciaCBox(TComboBox *cbox, bool opcao_todos);

	private: //objetos externos
      VTApl *apl;

	private: //métodos

	private: //dados locais
	};

//---------------------------------------------------------------------------
#endif
//eof
