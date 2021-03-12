//---------------------------------------------------------------------------

#ifndef TCPCapacitorH
#define TCPCapacitorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTCapacitor;
//class VTScg;
//---------------------------------------------------------------------------
class TCPCapacitor : public TCPEqpto
   {
   public:
                 __fastcall  TCPCapacitor(VTApl *apl, VTCapacitor *capacitor);
                 __fastcall ~TCPCapacitor(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof

