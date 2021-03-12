//---------------------------------------------------------------------------
#ifndef TMsgNAckH
#define TMsgNAckH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgNAck : public VTMsg
   {
   public:
                 __fastcall  TMsgNAck(void);
                 __fastcall ~TMsgNAck(void);
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

   public:  //dados
      struct strPCTE_NACK
            {
            char  funcao;
            short num_bytes;
            char  funcao_nacked;
            }pcte;

   };

//---------------------------------------------------------------------------
#endif
//eof

