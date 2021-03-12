//---------------------------------------------------------------------------
#ifndef TMsgRegistroTrechoH
#define TMsgRegistroTrechoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgRegistroTrecho : public VTMsg
   {
   public:
                 __fastcall  TMsgRegistroTrecho(void);
                 __fastcall ~TMsgRegistroTrecho(void);
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
      struct strPCTE_TRECHO
            {
            char  funcao;
            short num_bytes;
            char  hora;
            char  minuto;
            char  segundo;
            char  fluxo_curto;
            char  eqpto_tipo;
            int   eqpto_id;
            struct{
                  float mod_amp;
                  float ang_grau;
                  }tre_if[MAX_FASE];
				}pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

