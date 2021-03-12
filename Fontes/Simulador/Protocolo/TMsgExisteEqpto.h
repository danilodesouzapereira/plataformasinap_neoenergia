//---------------------------------------------------------------------------
#ifndef TMsgExisteEqptoH
#define TMsgExisteEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgExisteEqpto : public VTMsg
   {
   public:
                 __fastcall  TMsgExisteEqpto(void);
                 __fastcall ~TMsgExisteEqpto(void);
      bool       __fastcall  ConverteBufToMsg(strBUF *buf, int index);
      strBUF*    __fastcall  ConverteMsgToBuf(void);
      int        __fastcall  EqptoId(void);
      char       __fastcall  EqptoTipo(void);
      bool       __fastcall  HoraMinutoSegundo(strHMS &hms);
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
      struct strPCTE_EQPTO
            {
            char  funcao;
            short num_bytes;
            char  hora;
            char  minuto;
            char  segundo;
            char  eqpto_tipo;
            int   eqpto_id;
            char  codigo[32];
            }pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

