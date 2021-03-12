//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16526463
#ifndef TBDfiguraDummyH
#define TBDfiguraDummyH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBDfigura.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TBDfiguraDummy : public VTBDfigura
   {
   public:
           __fastcall  TBDfiguraDummy(VTApl *apl_owner, VTSNPDBConn *bd_conn);
		   __fastcall ~TBDfiguraDummy(void);
      bool __fastcall  LeAuxiliaresFiguras(void);
      bool __fastcall  LeFiguras(VTEstudo *estudo);
	  bool __fastcall  LeFiguras(VTEstudo *estudo, bool estudo_novo, TList *lisREDE_SELE);
	  bool __fastcall  RemoveEstudo(VTEstudo *estudo);
	  bool __fastcall  SalvaFiguras(VTEstudo *estudo);

   };

#endif
//---------------------------------------------------------------------------
//eof