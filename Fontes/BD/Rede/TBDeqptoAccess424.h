//---------------------------------------------------------------------------
#ifndef TBDeqptoAccess424H
#define TBDeqptoAccess424H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include "TBDeqptoAccess423.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TBDeqptoAccess424 : public TBDeqptoAccess423
   {
   public:
		   __fastcall  TBDeqptoAccess424(VTApl *apl_owner, VTSNPDBConn *bd_conn);
		   __fastcall ~TBDeqptoAccess424(void);

	private:  //objetos externos

   };

#endif
//---------------------------------------------------------------------------
//eof