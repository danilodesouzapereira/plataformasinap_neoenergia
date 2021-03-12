//---------------------------------------------------------------------------
#ifndef TMsgRegistroTrafoH
#define TMsgRegistroTrafoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgRegistroTrafo : public VTMsg
   {
   public:
                 __fastcall  TMsgRegistroTrafo(void);
                 __fastcall ~TMsgRegistroTrafo(void);
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
		struct strPCTE_TRAFO
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
                  }bar1_vf[MAX_FASE], bar2_vf[MAX_FASE];
            struct{
                  float mod_amp;
                  float ang_grau;
						}pri_if[MAX_FASE], sec_if[MAX_FASE];
				char  tap;
				}pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

