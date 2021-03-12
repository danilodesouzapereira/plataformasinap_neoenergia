//---------------------------------------------------------------------------
#ifndef TCPCargaH
#define TCPCargaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
//class VTScg;
//---------------------------------------------------------------------------
class TCPCarga : public TCPEqpto
   {
   public:
                 __fastcall  TCPCarga(VTApl *apl, VTCarga *carga);
                 __fastcall ~TCPCarga(void);

   private: //métodos
      AnsiString  __fastcall Classe(VTCarga *carga);

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof
