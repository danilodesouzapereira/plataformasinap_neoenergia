//---------------------------------------------------------------------------
#ifndef TMsgRegistroCapacitorH
#define TMsgRegistroCapacitorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgRegistroCapacitor : public VTMsg
   {
   public:
                 __fastcall  TMsgRegistroCapacitor(void);
                 __fastcall ~TMsgRegistroCapacitor(void);
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
      struct strPCTE_CAPACITOR
            {
            char  funcao;
            short num_bytes;
            char  hora;
            char  minuto;
            char  segundo;
            char  fluxo_curto;
            char  eqpto_tipo;
            int   eqpto_id;
				char  estado;
				float q_mvar;
            }pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

