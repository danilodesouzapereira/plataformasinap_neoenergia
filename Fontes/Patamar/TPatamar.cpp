//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPatamar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPatamar* __fastcall NewObjPatamar(int index, int hora_ini, int minuto_ini, int hora_fim, int minuto_fim)
   {
   try{
      return(new TPatamar(index, hora_ini, minuto_ini, hora_fim, minuto_fim));
      }catch(Exception &e)
         { }
	return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TPatamar::TPatamar(int index, int hora_ini, int minuto_ini, int hora_fim, int minuto_fim)
   {
   //variáveis locais
   int  num_hora;

   //salva parâmetros
	PD.index      = index;
	PD.hora_ini   = hora_ini;
	PD.minuto_ini = minuto_ini;
	PD.hora_fim   = hora_fim;
	PD.minuto_fim = minuto_fim;
   //define hora/minuto de cálculo
   if ((num_hora = hora_fim - hora_ini) <= 1)
      {//define hora/min de cálculo igual à hora/min inicial
		PD.hora_cal    = hora_ini;
		PD.minuto_cal  = minuto_ini;
      }
   else
      {//define hora/min de cálculo igual à hora média
		PD.hora_cal    = PD.hora_ini + (num_hora / 2);
		PD.minuto_cal  = 0;
      }
   //inicia nome igual ao período do patamar
	PD.nome = Periodo;
   //habilita o patamar
	PD.enabled = true;
   }

//---------------------------------------------------------------------------
__fastcall TPatamar::~TPatamar(void)
   {
   //nada a fazer
	}

//---------------------------------------------------------------------------
double __fastcall TPatamar::PM_GetDuracao(void)
	{
	//variáveis locais
	double duracao;
	int    min_ini = (HoraIni * 60) + MinutoIni;
	int    min_fim = (HoraFim * 60) + MinutoFim;

	//verifica se o patamar envolve virada do dia
	if (min_fim >= min_ini)
		{//horário inicial menor que horário final
		duracao = double(min_fim - min_ini) / 60.;
		}
   else
		{//horário inicial maior que horário final
		duracao = double(((24 * 60) - min_ini) + min_fim) / 60.;
      }
   return(duracao);
   }

//---------------------------------------------------------------------------
bool __fastcall TPatamar::PM_GetEnabled(void)
	{
	return(PD.enabled);
	}

//---------------------------------------------------------------------------
bool __fastcall TPatamar::PM_GetFluxoCorreto(void)
	{
	return(PD.fluxo_correto);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetHora_cal(void)
	{
	return(PD.hora_cal);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetHora_fim(void)
	{
	return(PD.hora_fim);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetHora_ini(void)
	{
	return(PD.hora_ini);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetIndex(void)
	{
	return(PD.index);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetMinuto_cal(void)
	{
	return(PD.minuto_cal);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetMinuto_fim(void)
	{
	return(PD.minuto_fim);
	}

//---------------------------------------------------------------------------
int __fastcall TPatamar::PM_GetMinuto_ini(void)
	{
	return(PD.minuto_ini);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TPatamar::PM_GetNome(void)
	{
	return(PD.nome);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TPatamar::PM_GetPeriodo(void)
   {
   AnsiString periodo;

   periodo.sprintf("%02d:%02d a %02d:%02d", HoraIni, MinutoIni, HoraFim, MinutoFim);
   return(periodo);
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetEnabled(bool enabled)
	{
	PD.enabled = enabled;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetFluxoCorreto(bool fluxo_correto)
	{
	PD.fluxo_correto = fluxo_correto;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetHora_cal(int hora_cal)
	{
	PD.hora_cal = hora_cal;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetHora_fim(int hora_fim)
	{
	PD.hora_fim = hora_fim;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetHora_ini(int hora_ini)
	{
	PD.hora_ini = hora_ini;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetMinuto_cal(int minuto_cal)
	{
	PD.minuto_cal = minuto_cal;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetMinuto_fim(int minuto_fim)
	{
	PD.minuto_fim = minuto_fim;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetMinuto_ini(int minuto_ini)
	{
	PD.minuto_ini = minuto_ini;
	}

//---------------------------------------------------------------------------
void __fastcall TPatamar::PM_SetNome(AnsiString nome)
	{
	PD.nome = nome;
	}

//---------------------------------------------------------------------------
//eof
