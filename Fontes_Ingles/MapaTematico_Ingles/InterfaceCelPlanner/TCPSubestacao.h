//---------------------------------------------------------------------------

#ifndef TCPSubestacaoH
#define TCPSubestacaoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TCPEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTRede;
//class VTScg;
//---------------------------------------------------------------------------
class TCPSubestacao : public TCPEqpto
   {
   public:
                 __fastcall  TCPSubestacao(VTApl *apl, VTRede *rede);
                 __fastcall ~TCPSubestacao(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof
