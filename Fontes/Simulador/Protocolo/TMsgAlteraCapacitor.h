//---------------------------------------------------------------------------
#ifndef TMsgAlteraCapacitorH
#define TMsgAlteraCapacitorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgAlteraCapacitor : public VTMsg
   {
   public:
                 __fastcall  TMsgAlteraCapacitor(void);
					  __fastcall ~TMsgAlteraCapacitor(void);
      bool       __fastcall  ConverteBufToMsg(strBUF *buf, int index);
      strBUF*    __fastcall  ConverteMsgToBuf(void);
      int        __fastcall  EqptoId(void);
      char       __fastcall  EqptoTipo(void);
      bool       __fastcall  HoraMinutoSegundo(strHMS &hms);
      AnsiString __fastcall  MsgAsString(void);

   protected:  //m?todos acessados via property
      int   __fastcall PM_GetEqptoId(void);
      char  __fastcall PM_GetEqptoTipo(void);
      char  __fastcall PM_GetFuncao(void);
      short __fastcall PM_GetNumBytes(void);
      //set
      void  __fastcall PM_SetEqptoId(int eqpto_id);
      void  __fastcall PM_SetEqptoTipo(char eqpto_tipo);

   private:  //m?todos
      short __fastcall Length(void);

   public:  //dados
		struct strPCTE_CAPACITOR
            {
            char  funcao;
            short num_bytes;
            char  hora;
            char  minuto;
            char  segundo;
            char  eqpto_tipo;
            int   eqpto_id;
				char  ligado;
				}pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

