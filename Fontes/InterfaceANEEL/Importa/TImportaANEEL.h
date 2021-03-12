//---------------------------------------------------------------------------
#ifndef TImportaANEELH
#define TImportaANEELH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaANEEL.h"

//---------------------------------------------------------------------------
class TImportaANEEL : public VTImportaANEEL
   {
   public:
           __fastcall  TImportaANEEL(VTApl *apl_owner);
			  __fastcall ~TImportaANEEL(void);
		bool __fastcall  ExecutaImportacao(AnsiString filename);

   private: //métodos
      void  __fastcall IniciaLog(void);
      void  __fastcall IniciaObjAplLocal(void);

   private: //objetos exgternos
      VTApl   *apl_owner;

   private: //dados locais
      int      dxbar, dybar;
      TList    *lisEQP;
      VTApl   *apl;
	};

#endif
//-----------------------------------------------------------------------------
// eof


