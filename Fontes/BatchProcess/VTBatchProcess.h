// ---------------------------------------------------------------------------
#ifndef VTBatchProcessH
#define VTBatchProcessH

// ---------------------------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTBatchTask;
class VTBatchCfg;
// ---------------------------------------------------------------------------
class VTBatchProcess : public TObject
{
	public:
	 __property AnsiString LogFilename = {read = PM_GetLogFilename};
	 __property AnsiString ProcessName = {read = PM_GetProcessName};
	 __property AnsiString ProcessDescription = {read = PM_GetProcessDescription};

	public:
				  __fastcall VTBatchProcess(void){};
	 virtual 	  __fastcall ~VTBatchProcess(void){};
	 virtual int  __fastcall Executa(AnsiString cfgFilePath, AnsiString pid) = 0;
	 virtual int  __fastcall ExecuteInMemory(AnsiString cfgFilePath, AnsiString pid) = 0;


	protected:
     virtual VTApl*          __fastcall CreateApl(VTApl *apl_owner) throw(Exception) = 0;
     virtual VTBatchTask* 	 __fastcall CreateBatchTask(void) = 0;
	 virtual VTBatchCfg* 	 __fastcall CreateBatchCfg(void) = 0;
	 virtual bool 			 __fastcall DefineDiretorioInstalacao(void) = 0;
//	 virtual bool 			 __fastcall GetParametros(AnsiString iniFilePath) = 0;
	 virtual void 			 __fastcall IniciaTmp(void) = 0;
	 virtual void 			 __fastcall LimpaTmp(void) = 0;
	 virtual bool 			 __fastcall LogInicia(AnsiString log_path) = 0;
	 virtual void 			 __fastcall LogFecha(void) = 0;
//	 virtual AnsiString      __fastcall ReadLogFilepath(AnsiString iniFilePath) = 0;
//	 virtual bool 			 __fastcall ValidaPasta(AnsiString dir_path) = 0;

    protected: //métodos acessados via property
	 //getters
	 virtual AnsiString __fastcall PM_GetLogFilename(void) = 0;
	 virtual AnsiString __fastcall PM_GetProcessName(void) = 0;
	 virtual AnsiString __fastcall PM_GetProcessDescription(void) = 0;

	protected:
	 VTBatchTask *batch_task_;
	 VTBatchCfg  *batch_cfg_;
};
// ---------------------------------------------------------------------------
VTBatchProcess* __fastcall NewObjBatchAbreRedesProcess(VTApl *apl_owner);
VTBatchProcess* __fastcall NewObjBatchAlteraTopologiaProcess(VTApl *apl_owner);
VTBatchProcess* __fastcall NewObjBatchAjusteProcess(VTApl *apl_owner);
VTBatchProcess* __fastcall NewObjBatchImportaRedesProcess(VTApl *apl_owner);
VTBatchProcess* __fastcall NewObjBatchImportaTxtProcess(VTApl *apl_owner);
VTBatchProcess* __fastcall NewObjBatchMultiTaskProcess(VTApl *apl_owner);
VTBatchProcess* __fastcall NewObjBatchPerdatecProcess(VTApl *apl_owner);
// ---------------------------------------------------------------------------
#endif
// ---------------------------------------------------------------------------
// eof
