//---------------------------------------------------------------------------
#ifndef TBDeqptoAccess425H
#define TBDeqptoAccess425H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include "TBDeqptoAccess424.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TBDeqptoAccess425 : public TBDeqptoAccess424
   {
   public:
		   __fastcall  TBDeqptoAccess425(VTApl *apl_owner, VTSNPDBConn *bd_conn);
		   __fastcall ~TBDeqptoAccess425(void);

	private:  //objetos externos

   };

#endif
//---------------------------------------------------------------------------
//eof