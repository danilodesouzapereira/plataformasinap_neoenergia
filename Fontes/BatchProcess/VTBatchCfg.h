//---------------------------------------------------------------------------
#ifndef VTBatchCfgH
#define VTBatchCfgH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <System.JSON.hpp>
#include <inifiles.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTLog;
//---------------------------------------------------------------------------
// VTBatchCfg: Classe utilizada para transportar parametros de configuração
//---------------------------------------------------------------------------
class VTBatchCfg : public TObject
{
	public:
	 __property VTBatchCfg* Defaults 	= {write=PM_SetDefaults};
	 __property bool	 	IgnorarFalha= {read=PM_GetIgnorarFalha, write=PM_SetIgnorarFalha};
	 __property AnsiString 	LogFilepath	= {read=PM_GetLogFilepath, write=PM_SetLogFilepath};
	 __property VTLog* 		Log			= {read=PM_GetLog, write=PM_SetLog};
	 __property VTLog* 		LogResumo	= {read=PM_GetLogResumo, write=PM_SetLogResumo};
	 __property AnsiString  PID         = {read=PM_GetPID, write=PM_SetPID};


	public:
						__fastcall  VTBatchCfg(void) {};
	 virtual      		__fastcall ~VTBatchCfg(void) {};
     virtual void 		__fastcall AdicionaPIDnoPath(AnsiString &filedir) = 0;
	 virtual void       __fastcall CopiaParametros(VTBatchCfg *original) = 0;
	 virtual AnsiString __fastcall GetLogDir(AnsiString cfgFilePath) throw(Exception)= 0; //caminho do arquivo de config, precisa ter extensão correta
	 virtual bool 		__fastcall GetParametros(AnsiString cfgFilePath) throw(Exception) = 0;//caminho do arquivo de config, precisa ter extensão correta
	 virtual void 		__fastcall LogAddResumo(AnsiString txt_log, AnsiString tipo_erro) = 0;
	 virtual void   	__fastcall ValoresDefault(void) = 0;
	 virtual bool   	__fastcall ValidaParametros(void) = 0;

	protected:
	 virtual AnsiString	__fastcall GetLogINI(AnsiString iniFilePath) = 0;
//	 virtual AnsiString	__fastcall GetLogJSON(AnsiString jsonFilePath) = 0;
	 virtual bool   	__fastcall GetParametrosINI(AnsiString iniFilePath) = 0;
//	 virtual bool   	__fastcall GetParametrosJSON(AnsiString jsonFilePath) = 0;
	 // deve lançar exceções do tipo Exception caso falhe em algo
	 virtual AnsiString	__fastcall GetLogINI(TIniFile *ini) throw(Exception) = 0;
	 virtual bool   	__fastcall GetParametrosINI(TIniFile *ini) throw(Exception) = 0;
	 // deve lançar exceções do tipo Exception caso falhe em algo
//	 virtual AnsiString	__fastcall GetLogJSON(TJSONObject *json) throw(Exception) = 0;
//	 virtual bool   	__fastcall GetParametrosJSON(TJSONObject *json) throw(Exception) = 0;

	 protected: //métodos acessados via property
	  virtual bool		 __fastcall PM_GetIgnorarFalha(void) = 0;
	  virtual AnsiString __fastcall PM_GetLogFilepath(void) = 0;
	  virtual VTLog*	 __fastcall PM_GetLog(void) = 0;
	  virtual VTLog*	 __fastcall PM_GetLogResumo(void) = 0;
	  virtual AnsiString __fastcall PM_GetPID(void) = 0;
	  virtual void       __fastcall PM_SetIgnorarFalha(bool ignorar) = 0;
	  virtual void       __fastcall PM_SetLogFilepath(AnsiString filepath) = 0;
	  virtual void       __fastcall PM_SetLog(VTLog *log) = 0;
	  virtual void       __fastcall PM_SetLogResumo(VTLog *logResumo) = 0;
	  virtual void       __fastcall PM_SetPID(AnsiString pid) = 0;
	  virtual void       __fastcall PM_SetDefaults(VTBatchCfg *defaults) = 0;
};
//---------------------------------------------------------------------------
//VTTaskCfg* __fastcall NewObjMultiTaskCfg(void);
//VTBatchCfg* __fastcall NewObjPerdatecTaskCfg(void);
//VTBatchCfg* __fastcall NewObjAlteraTopologiaTaskCfg(void);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
//eof
