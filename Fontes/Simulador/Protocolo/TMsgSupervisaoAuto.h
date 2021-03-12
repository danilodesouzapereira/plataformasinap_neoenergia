//---------------------------------------------------------------------------
#ifndef TMsgSupervisaoAutoH
#define TMsgSupervisaoAutoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMsg.h"

//---------------------------------------------------------------------------
class TMsgSupervisaoAuto : public VTMsg
   {
	public:	//property
		__property bool Enabled = {read=PM_GetEnabled , write=PM_SetEnabled};

   public:
					  __fastcall  TMsgSupervisaoAuto(void);
					  __fastcall ~TMsgSupervisaoAuto(void);
      bool       __fastcall  ConverteBufToMsg(strBUF *buf, int index);
      strBUF*    __fastcall  ConverteMsgToBuf(void);
      AnsiString __fastcall  MsgAsString(void);

	protected:  //métodos acessados via property
		bool  __fastcall PM_GetEnabled(void);
		int   __fastcall PM_GetEqptoId(void);
      char  __fastcall PM_GetEqptoTipo(void);
      char  __fastcall PM_GetFuncao(void);
      short __fastcall PM_GetNumBytes(void);
		//set
		void  __fastcall PM_SetEnabled(bool enabled);
		void  __fastcall PM_SetEqptoId(int eqpto_id);
		void  __fastcall PM_SetEqptoTipo(char eqpto_tipo);

   private:  //métodos
      short __fastcall Length(void);

	public:
		struct strPCTE_SUPERVISAO_AUTO
            {
            char  funcao;
            short num_bytes;
            char  hora;
            char  minuto;
            char  segundo;
				char  enabled;
				}pcte;
   };

//---------------------------------------------------------------------------
#endif
//eof

