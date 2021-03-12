//---------------------------------------------------------------------------
#ifndef TDataRedeH
#define TDataRedeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTConfiab.h"
//---------------------------------------------------------------------------
class VTRede;
//---------------------------------------------------------------------------
class TDataRede : public TObject
   {
   public:
				 __fastcall TDataRede(VTRede *rede, RES_CONF presRede);
				 __fastcall ~TDataRede(void);

   private: //métodos

   private: //dados externos

   private: //dados locais
   public:
		VTRede   *rede;
		RES_CONF pres;
	};

#endif
//---------------------------------------------------------------------------
//eof
