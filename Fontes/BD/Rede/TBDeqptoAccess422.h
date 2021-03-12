//---------------------------------------------------------------------------
#ifndef TBDeqptoAccess422H
#define TBDeqptoAccess422H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include "TBDeqptoAccess.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------
class VTMonta;
class VTLog;

//---------------------------------------------------------------------------
class TBDeqptoAccess422 : public TBDeqptoAccess
   {
   public:
		   __fastcall  TBDeqptoAccess422(VTApl *apl_owner, VTSNPDBConn *bd_conn);
		   __fastcall ~TBDeqptoAccess422(void);
			bool __fastcall SPSNP_rdBateria(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra);
			bool __fastcall SPSNP_rdEstudo(void);
			bool __fastcall SPSNP_rdEstudoAlternativa(VTEstudo* estudo);
			bool __fastcall SPSNP_rdEstudoObraAtribExt(VTEstudo* estudo);
			bool __fastcall SPSNP_rdMunicipio(void);
			bool __fastcall SPSNP_rdRegulador(VTEstudo* estudo, bool bolUseRedeTmp,bool bolObra);
			
	private:  //objetos externos

   };

#endif
//---------------------------------------------------------------------------
//eof