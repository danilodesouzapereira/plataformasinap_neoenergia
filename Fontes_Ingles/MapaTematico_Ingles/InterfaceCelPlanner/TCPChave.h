//---------------------------------------------------------------------------
#ifndef TCPChaveH
#define TCPChaveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTChave;
//class VTScg;
//---------------------------------------------------------------------------
class TCPChave : public TCPEqpto
   {
   public:
                 __fastcall  TCPChave(VTApl *apl, VTChave *chave);
                 __fastcall ~TCPChave(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof
