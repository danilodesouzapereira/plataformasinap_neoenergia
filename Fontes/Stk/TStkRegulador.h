//---------------------------------------------------------------------------
#ifndef TStkReguladorH
#define TStkReguladorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTStkRegulador.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;

//---------------------------------------------------------------------------
class TStkRegulador : public VTStkRegulador
	{
   public :
			  __fastcall  TStkRegulador(VTApl *apl_owner);
			  __fastcall ~TStkRegulador(void);
      bool __fastcall  AlteraPasso(VTRegulador *regulador, int passo);

	private: //objetos externos
      VTApl *apl;

	private: //métodos

	private: //dados locais
	};

//---------------------------------------------------------------------------
#endif
//eof
