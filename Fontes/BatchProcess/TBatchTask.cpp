// ---------------------------------------------------------------------------
#include <vcl.h>
#include <vcl.h>
#pragma hdrstop
#include <windows.h>
#include <System.SysUtils.hpp>
#include <inifiles.hpp>
// ---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BatchProcess\VTBatchCfg.h>
#include <Fontes\Log\VTLog.h>
// ---------------------------------------------------------------------------
#include "TBatchTask.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
__fastcall TBatchTask::TBatchTask(void)
{
    log_detalhado_ = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TBatchTask::~TBatchTask(void)
{

}
// ---------------------------------------------------------------------------
void __fastcall TBatchTask::LogAdd(AnsiString txt_log, bool time)
{
	// Variáveis locais
	AnsiString txt = "";

	if (log_ != NULL)
	{
		if (time == true)
		{
			txt = Date().FormatString("yyyy/mm/dd") + "_" + Time().FormatString("hh:mm:ss") + " - ";
		}
		log_->Add(txt + txt_log);
		LogDetalheAdd(txt_log);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TBatchTask::LogDetalheAdd(AnsiString txt_log)
{
	// Variáveis locais
	AnsiString txt = "";

	if (log_detalhado_ != NULL)
	{
		txt = Date().FormatString("yyyy/mm/dd") + "_" + Time().FormatString("hh:mm:ss") + " - ";
		log_detalhado_->Add(txt + txt_log);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TBatchTask::LogaTempoExecucao(AnsiString tarefa, clock_t tIni, clock_t tFim)
{
	// variaveis locais
	clock_t tDecorrido;
	clock_t tMinsInMili = 60 * CLOCKS_PER_SEC;
	clock_t tHoursInMili = 60 * tMinsInMili;
	clock_t tDaysinMili = 24 * tHoursInMili;
	AnsiString dias = " dias, ";
	AnsiString horas = " horas, ";
	AnsiString minutos = " minutos, ";
	AnsiString segundos = " segundos, ";
	AnsiString milisegundos = " milisegundos";
	AnsiString tempo = "";

	AnsiString txt;
	if (tarefa.IsEmpty())
	{
		txt = "                      O tempo total : ";
	}
	else
	{
		txt = "                      O tempo total [" + tarefa + "] : ";
	}
	// Calcula o tempo en milisegundos
	tDecorrido = (tFim - tIni); // Tempo em milisegundos

	// Calculo do tempo em d, h, m, s
	// Cálculo dos dias
	long elapsedDays = tDecorrido / tDaysinMili;
	tDecorrido = tDecorrido % tDaysinMili;
	// verifica se as palavras vao no sigular ou plural
	if (elapsedDays >= 1)
	{
		if (elapsedDays == 1)
		{
			dias = " dia, ";
		}
		tempo = FloatToStr(elapsedDays) + dias;
	}
	// Cálculo das horas
	long elapsedHours = tDecorrido / tHoursInMili;
	tDecorrido = tDecorrido % tHoursInMili;
	// verifica se as palavras vao no sigular ou plural
	if (elapsedHours >= 1)
	{
		if (elapsedHours == 1)
		{
			horas = " hora, ";
		}
		tempo = tempo + FloatToStr(elapsedHours) + horas;
	}
	// Cálculo dos minutos
	long elapsedMinutes = tDecorrido / tMinsInMili;
	tDecorrido = tDecorrido % tMinsInMili;
	if (elapsedMinutes >= 1)
	{
		if (elapsedMinutes == 1)
		{
			minutos = " minuto, ";
		}
		tempo = tempo + FloatToStr(elapsedMinutes) + minutos;
	}
	// Cálculo dos segundos
	long elapsedSeconds = tDecorrido / CLOCKS_PER_SEC;
	if (elapsedSeconds >= 1)
	{
		if (elapsedSeconds == 1)
		{
			segundos = " segundo, ";
		}
		tempo = tempo + FloatToStr(elapsedSeconds) + segundos;
	}
	// Cálculo dos milisegundos
	long elapsedMiliSeconds = tDecorrido - (elapsedSeconds * CLOCKS_PER_SEC);
	if (elapsedMiliSeconds >= 1)
	{
		if (elapsedMiliSeconds == 1)
		{
			milisegundos = " milisegundo";
		}
		tempo = tempo + FloatToStr(elapsedMiliSeconds) + milisegundos;
	}
	if (tempo.IsEmpty())
	{
        tempo = "menos de 1 milisegundo.";
	}
	LogAdd(txt + "foi de " + tempo + ".", false);
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TBatchTask::PM_GetLogDir(void)
{
	return logDir_;
}
// ---------------------------------------------------------------------------
void __fastcall TBatchTask::PM_SetLogDir(AnsiString log_dir)
{
	logDir_ = log_dir;
}
// ---------------------------------------------------------------------------
// eof
