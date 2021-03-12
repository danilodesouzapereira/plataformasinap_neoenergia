//---------------------------------------------------------------------------
#ifndef TMsgRegistroBarraH
#define TMsgRegistroBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgRegistroBarra : public VTMsg
   {
   public:
                 __fastcall  TMsgRegistroBarra(void);
                 __fastcall ~TMsgRegistroBarra(void);
      bool       __fastcall  ConverteBufToMsg(strBUF *buf, int index);
      strBUF*    __fastcall  ConverteMsgToBuf(void);
      AnsiString __fastcall  MsgAsString(void);

   protected:  //m�todos acessados via property
      int   __fastcall PM_GetEqptoId(void);
      char  __fastcall PM_GetEqptoTipo(void);
      char  __fastcall PM_GetFuncao(void);
      short __fastcall PM_GetNumBytes(void);
      //set
      void  __fastcall PM_SetEqptoId(int eqpto_id);
      void  __fastcall PM_SetEqptoTipo(char eqpto_tipo);

   private:  //m�todos
      short __fastcall Length(void);

   public:  //objetos externos
      VTBarra *barra;

   public:  //dados
      struct strPCTE_BARRA
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
                  float mod_volt;
                  float ang_grau;
                  }vf[MAX_FASE];
            }pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

