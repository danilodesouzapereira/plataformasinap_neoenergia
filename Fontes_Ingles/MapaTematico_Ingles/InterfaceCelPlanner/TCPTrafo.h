//---------------------------------------------------------------------------

#ifndef TCPTrafoH
#define TCPTrafoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTTrafo;
//class VTScg;
//---------------------------------------------------------------------------
class TCPTrafo : public TCPEqpto
   {
   public:
                 __fastcall  TCPTrafo(VTApl *apl, VTTrafo *trafo);
                 __fastcall ~TCPTrafo(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof

