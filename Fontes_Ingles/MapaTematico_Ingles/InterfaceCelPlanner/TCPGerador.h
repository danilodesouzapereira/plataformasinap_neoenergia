//---------------------------------------------------------------------------
#ifndef TCPGeradorH
#define TCPGeradorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTGerador;
//class VTScg;
//---------------------------------------------------------------------------
class TCPGerador : public TCPEqpto
   {
   public:
                 __fastcall  TCPGerador(VTApl *apl, VTGerador *gerador);
                 __fastcall ~TCPGerador(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof
