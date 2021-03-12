//---------------------------------------------------------------------------

#ifndef TCPReguladorH
#define TCPReguladorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTRegulador;
//class VTScg;
//---------------------------------------------------------------------------
class TCPRegulador : public TCPEqpto
   {
   public:
                 __fastcall  TCPRegulador(VTApl *apl, VTRegulador *regulador);
                 __fastcall ~TCPRegulador(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof

