// ---------------------------------------------------------------------------
#ifndef TBatchTaskH
#define TBatchTaskH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.hpp>
#include <Classes.hpp>
#include <time.h>
#include "VTBatchTask.h"

// ---------------------------------------------------------------------------
class TArqRede;
class VTLog;
// ---------------------------------------------------------------------------
// teste
//#ifndef _DEBUG
//#define DEBUG_MESSAGE //
//#endif
//#ifdef _DEBUG
//#define DEBUG_MESSAGE(msg) OutputDebugString("msg")
//#endif
// ---------------------------------------------------------------------------
class TBatchTask : public VTBatchTask
{
	public:
					__fastcall TBatchTask(void);
	 virtual		__fastcall ~TBatchTask(void);
     virtual bool 	__fastcall ConfiguraTarefa(VTBatchCfg *cfg) = 0;
	 virtual bool 	__fastcall Executa(VTBatchCfg *cfg) = 0;
	 virtual bool	__fastcall ExecuteInMemory(VTBatchCfg *cfg) = 0;
//	 virtual bool 	__fastcall GetParametros(VTTaskCfg *cfg, AnsiString iniFilePath) = 0;
			 void 	__fastcall LogAdd(AnsiString txt_log, bool time = true);
			 void 	__fastcall LogDetalheAdd(AnsiString txt_log);

	protected: //métodos acessados via property
	 //getters
	 AnsiString __fastcall PM_GetLogDir(void);
	 //setters
	 void       __fastcall PM_SetLogDir(AnsiString log_dir);

	protected: // métodos
	 virtual bool __fastcall ExecutaTarefaBase(VTBatchCfg *cfg) = 0;
			 void __fastcall LogaTempoExecucao(AnsiString tarefa, clock_t tIni, clock_t tFim);
//			 void __fastcall ParseKeyValue(TStrings *orig, TStrings *destino);

    protected:
	 // objetos externos
	 VTApl *apl_;
	 VTLog *log_;

	 // dados
	 AnsiString logDir_;

};

// ---------------------------------------------------------------------------
#endif
// eof
