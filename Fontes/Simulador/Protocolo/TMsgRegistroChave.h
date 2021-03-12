//---------------------------------------------------------------------------
#ifndef TMsgRegistroChaveH
#define TMsgRegistroChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgRegistroChave : public VTMsg
   {
   public:
                 __fastcall  TMsgRegistroChave(void);
                 __fastcall ~TMsgRegistroChave(void);
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
      struct strPCTE_CHAVE
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
                  }chv_if[MAX_FASE];
				char  estado;
				char  cbr;
				char  cbtl;
				char  lockout;
				float p3f_mw;
				float q3f_mvar;
				float pa_mw;
				float qa_mvar;
				float pb_mw;
				float qb_mvar;
				float pc_mw;
				float qc_mvar;
				}pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

