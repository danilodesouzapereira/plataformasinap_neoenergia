//---------------------------------------------------------------------------
#ifndef TImportaMDBH
#define TImportaMDBH

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImporta.h"

//---------------------------------------------------------------------------
class VTBDaneel;

//---------------------------------------------------------------------------
class TImportaMDB : public VTImporta
	{
   public:
           __fastcall  TImportaMDB(VTApl *apl_owner);
			  __fastcall ~TImportaMDB(void);
		bool __fastcall  ExecutaImportacao(AnsiString filename);

	private: //m�todos

	private: //objetos exgternos
		VTApl   *apl;

	private: //dados locais
		VTBDaneel *BDaneel;
	};

#endif
//-----------------------------------------------------------------------------
// eof


