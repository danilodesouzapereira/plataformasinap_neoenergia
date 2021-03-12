//---------------------------------------------------------------------------
#ifndef VTStkPatamarH
#define VTStkPatamarH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTStkPatamar : public TObject
	{
   public :
			          __fastcall  VTStkPatamar(void) {};
		virtual      __fastcall ~VTStkPatamar(void) {};
      virtual bool __fastcall  IniciaCBox(TComboBox *cbox, bool opcao_todos) = 0;
	};

//---------------------------------------------------------------------------
VTStkPatamar* __fastcall NewObjStkPatamar(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
