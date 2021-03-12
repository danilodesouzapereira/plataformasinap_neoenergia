//---------------------------------------------------------------------------

#ifndef TPadraoRedeH
#define TPadraoRedeH

#include "VTPadraoRede.h"

//---------------------------------------------------------------------------
class TPadraoRede : public VTPadraoRede
   {
   public:
                    __fastcall  TPadraoRede(void);
                    __fastcall ~TPadraoRede(void);
      VTPadraoRede* __fastcall  Clone(void);
   };
#endif
