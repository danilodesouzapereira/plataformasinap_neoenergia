//---------------------------------------------------------------------------
#ifndef VTStkReguladorH
#define VTStkReguladorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRegulador;

//---------------------------------------------------------------------------
class VTStkRegulador : public TObject
	{
   public :
			          __fastcall  VTStkRegulador(void) {};
		virtual      __fastcall ~VTStkRegulador(void) {};
      virtual bool __fastcall  AlteraPasso(VTRegulador *regulador, int passo) = 0;
	};

//---------------------------------------------------------------------------
VTStkRegulador* __fastcall NewObjStkRegulador(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
