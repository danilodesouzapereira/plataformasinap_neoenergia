//---------------------------------------------------------------------------
#ifndef TPatamarH
#define TPatamarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPatamar.h"

//---------------------------------------------------------------------------
class TPatamar : public VTPatamar
   {
   public:  //funções públicas
           __fastcall TPatamar(int index, int hora_ini, int minuto_ini, int hora_fim, int minuto_fim);
			  __fastcall ~TPatamar(void);

	protected:
		double     __fastcall PM_GetDuracao(void);
		bool       __fastcall PM_GetEnabled(void);
		bool       __fastcall PM_GetFluxoCorreto(void);
		int        __fastcall PM_GetHora_cal(void);
		int        __fastcall PM_GetHora_fim(void);
		int        __fastcall PM_GetHora_ini(void);
		int        __fastcall PM_GetIndex(void);
		int        __fastcall PM_GetMinuto_cal(void);
		int        __fastcall PM_GetMinuto_fim(void);
		int        __fastcall PM_GetMinuto_ini(void);
		AnsiString __fastcall PM_GetNome(void);
		AnsiString __fastcall PM_GetPeriodo(void);
		//set
		void       __fastcall PM_SetEnabled(bool enabled);
		void       __fastcall PM_SetFluxoCorreto(bool fluxo_correto);
		void       __fastcall PM_SetHora_cal(int hora_cal);
		void       __fastcall PM_SetHora_fim(int hora_fim);
		void       __fastcall PM_SetHora_ini(int hora_ini);
		void       __fastcall PM_SetMinuto_cal(int minuto_cal);
		void       __fastcall PM_SetMinuto_fim(int minuto_fim);
		void       __fastcall PM_SetMinuto_ini(int minuto_ini);
		void       __fastcall PM_SetNome(AnsiString nome);

	private: //dados acessados via property
		struct   {
               int        index;
               AnsiString nome;
               bool       enabled;
               int        hora_ini, minuto_ini;
               int        hora_fim, minuto_fim;
               int        hora_cal, minuto_cal;
               bool       fluxo_correto;
               } PD;

	};

//---------------------------------------------------------------------------
#endif
//eof
