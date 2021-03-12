//---------------------------------------------------------------------------
#ifndef TCPBarraH
#define TCPBarraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
//class VTScg;
//---------------------------------------------------------------------------
class TCPBarra : public TCPEqpto
   {
   public:
				 __fastcall  TCPBarra(VTApl *apl, VTBarra *barra);
				 __fastcall ~TCPBarra(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof