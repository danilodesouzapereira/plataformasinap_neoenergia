//---------------------------------------------------------------------------
#ifndef TMsgAckH
#define TMsgAckH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgAck : public VTMsg
   {
   public:
                 __fastcall  TMsgAck(void);
                 __fastcall ~TMsgAck(void);
      bool       __fastcall  ConverteBufToMsg(strBUF *buf, int index);
      strBUF*    __fastcall  ConverteMsgToBuf(void);
      AnsiString __fastcall  MsgAsString(void);

   protected:  //métodos acessados via property
      int   __fastcall PM_GetEqptoId(void);
      char  __fastcall PM_GetEqptoTipo(void);
      char  __fastcall PM_GetFuncao(void);
      short __fastcall PM_GetNumBytes(void);
      //set
      void  __fastcall PM_SetEqptoId(int eqpto_id);
      void  __fastcall PM_SetEqptoTipo(char eqpto_tipo);

   private:  //métodos
      short __fastcall Length(void);

   public:
      struct strPCTE_ACK
            {
            char  funcao;
            short num_bytes;
            char  funcao_acked;
            char  sucesso;
            }pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

