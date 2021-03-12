//---------------------------------------------------------------------------
#ifndef VTPatamarH
#define VTPatamarH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPatamar : public TObject
   {
	public:  //property
		__property int        Index        = {read=PM_GetIndex                                   };
		__property AnsiString Nome         = {read=PM_GetNome,         write= PM_SetNome         };
		__property bool       Enabled      = {read=PM_GetEnabled,      write= PM_SetEnabled      };
		__property int        HoraIni      = {read=PM_GetHora_ini,     write= PM_SetHora_ini     };
		__property int        HoraFim      = {read=PM_GetHora_fim,     write= PM_SetHora_fim     };
		__property int        HoraCal      = {read=PM_GetHora_cal,     write= PM_SetHora_cal     };
		__property int        MinutoIni    = {read=PM_GetMinuto_ini,   write= PM_SetMinuto_ini   };
		__property int        MinutoFim    = {read=PM_GetMinuto_fim,   write= PM_SetMinuto_fim   };
		__property int        MinutoCal    = {read=PM_GetMinuto_cal,   write= PM_SetMinuto_cal   };
		__property bool       FluxoCorreto = {read=PM_GetFluxoCorreto, write= PM_SetFluxoCorreto };
		__property AnsiString Periodo      = {read=PM_GetPeriodo                                 };
		__property double     Duracao      = {read=PM_GetDuracao                                 };

	public:  //funções públicas
			__fastcall  VTPatamar(void) {};
			__fastcall ~VTPatamar(void) {};

	protected:
		virtual double     __fastcall PM_GetDuracao(void) = 0;
		virtual bool       __fastcall PM_GetEnabled(void) = 0;
		virtual bool       __fastcall PM_GetFluxoCorreto(void) = 0;
		virtual int        __fastcall PM_GetHora_cal(void) = 0;
		virtual int        __fastcall PM_GetHora_fim(void) = 0;
		virtual int        __fastcall PM_GetHora_ini(void) = 0;
		virtual int        __fastcall PM_GetIndex(void) = 0;
		virtual int        __fastcall PM_GetMinuto_cal(void) = 0;
		virtual int        __fastcall PM_GetMinuto_fim(void) = 0;
		virtual int        __fastcall PM_GetMinuto_ini(void) = 0;
		virtual AnsiString __fastcall PM_GetNome(void) = 0;
		virtual AnsiString __fastcall PM_GetPeriodo(void) = 0;
		//set
		virtual void       __fastcall PM_SetEnabled(bool enabled) = 0;
		virtual void       __fastcall PM_SetFluxoCorreto(bool fluxo_correto) = 0;
		virtual void       __fastcall PM_SetHora_cal(int hora_cal) = 0;
		virtual void       __fastcall PM_SetHora_fim(int hora_fim) = 0;
		virtual void       __fastcall PM_SetHora_ini(int hora_ini) = 0;
		virtual void       __fastcall PM_SetMinuto_cal(int minuto_cal) = 0;
		virtual void       __fastcall PM_SetMinuto_fim(int minuto_fim) = 0;
		virtual void       __fastcall PM_SetMinuto_ini(int minuto_ini) = 0;
		virtual void       __fastcall PM_SetNome(AnsiString nome) = 0;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPatamar
//---------------------------------------------------------------------------
VTPatamar* __fastcall NewObjPatamar(int index, int hora_ini, int minuto_ini, int hora_fim, int minuto_fim);

//---------------------------------------------------------------------------
#endif
//eof
