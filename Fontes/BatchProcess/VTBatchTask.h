// ---------------------------------------------------------------------------
#ifndef VTBatchTaskH
#define VTBatchTaskH

// ---------------------------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTLog;
class VTBatchCfg;
// ---------------------------------------------------------------------------
class VTBatchTask : public TObject
{
	public:
	 __property AnsiString DirLog = {read = PM_GetLogDir, write = PM_SetLogDir};

	public :
					__fastcall VTBatchTask(void){};
	 virtual		__fastcall ~VTBatchTask(void){};
     virtual bool 	__fastcall ConfiguraTarefa(VTBatchCfg *cfg) = 0;
	 //descrição:  ConfiguraTarefa(VTBatchCfg *cfg) - Transfere as configuracoes do
	 //             objeto VTBatchCfg para os devidos lugares na tarefa.
	 virtual bool 	__fastcall Executa(VTBatchCfg *cfg) = 0;
	 //descrição: Executa(VTBatchCfg *cfg)- Executa a tarefa "fechada",
	 //           isto é, deve supor como inicio um sinap vazio, e no fim da
	 //           tarefa retornar a esse estado. (abre sinap, faz algo, fecha sinap)
	 virtual bool 	__fastcall ExecuteInMemory(VTBatchCfg *cfg) = 0;
	 //descrição: ExecuteInMemory(VTBatchCfg *cfg) - Executa a tarefa "aberta",
	 //           isto é, deve supor como inicio um sinap já iniciado. Usado para
	 //           sequências de tarefas na memoria, por exemplo: ajustar demanda,
	 //           que supõe uma rede aberta e no fim deixa essa rede ajustada para
	 //           um novo processo seguir.
//	 virtual bool 	__fastcall GetParametros(VTBatchCfg *cfg, AnsiString iniFilePath) = 0;
	 //descrição: GetParametros(VTBatchCfg *cfg, AnsiString iniFilePath) - Inicializa
	 //           as configurações da tarefa à partir de um arquivo INI
	 virtual void 	__fastcall LogAdd(AnsiString txt_log, bool time = true) = 0;
	 //descrição: LogAdd(AnsiString txt_log, bool time = true) - Metodo para
	 //           adicionar registros ao log, com opção de timestamp (com/sem)
     virtual void 	__fastcall LogDetalheAdd(AnsiString txt_log) = 0;
	 //descrição: LogDetalheAdd(AnsiString txt_log) - Metodo para
	 //           adicionar registros ao log detalhado

	protected: //métodos acessados via property
	 //getters
	 virtual AnsiString __fastcall PM_GetLogDir(void) = 0;
	 //setters
	 virtual void       __fastcall PM_SetLogDir(AnsiString log_dir) = 0;

	protected:
	 virtual bool __fastcall ExecutaTarefaBase(VTBatchCfg *cfg) = 0;
	 //descrição: ExecutaProcessoBase(VTBatchCfg *cfg) - Metodo base do processo,
	 //           tanto o Executa(VTBatchCfg *cfg) e o ExecuteInMemory(VTBatchCfg *cfg)
	 //           devem utilizá-lo, uniformizado o comportamento basico do módulo

	public:
	 VTLog *log_detalhado_;
};
// ---------------------------------------------------------------------------
VTBatchTask* __fastcall NewObjBatchAbreRedesTask(VTApl *apl_owner, VTLog *log);
VTBatchTask* __fastcall NewObjBatchAlteraTopologiaTask(VTApl *apl_owner, VTLog *log);
VTBatchTask* __fastcall NewObjBatchAjusteTask(VTApl *apl_owner, VTLog *log);
VTBatchTask* __fastcall NewObjBatchImportaRedesTask(VTApl *apl_owner, VTLog *log);
VTBatchTask* __fastcall NewObjBatchImportaTxtTask(VTApl *apl_owner, VTLog *log);
VTBatchTask* __fastcall NewObjBatchMultiTaskTask(VTApl *apl_owner, VTLog *log);
VTBatchTask* __fastcall NewObjBatchPerdatecTask(VTApl *apl_owner, VTLog *log);

// ---------------------------------------------------------------------------
#endif
// eof