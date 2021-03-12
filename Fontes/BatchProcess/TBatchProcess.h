// ---------------------------------------------------------------------------
#ifndef TBatchProcessH
#define TBatchProcessH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <System.hpp>
#include <System.Classes.hpp>
#include "VTBatchProcess.h"

// ---------------------------------------------------------------------------
class VTLog;
class VTApl;
class VTBatchTask;

// ---------------------------------------------------------------------------
class TBatchProcess : public VTBatchProcess
{
	public:
				 __fastcall TBatchProcess(VTApl *apl_owner);
				 __fastcall ~TBatchProcess(void);
			 int __fastcall Executa(AnsiString cfgFilePath, AnsiString pid);
			 int __fastcall ExecuteInMemory(AnsiString cfgFilePath, AnsiString pid);

	protected: // métodos
	 virtual VTApl*          __fastcall CreateApl(VTApl *apl_owner);
	 virtual VTBatchTask*    __fastcall CreateBatchTask(void) throw(Exception) = 0;
	 virtual VTBatchCfg* 	 __fastcall CreateBatchCfg(void) throw(Exception) = 0;
			 bool 			 __fastcall DefineDiretorioInstalacao(void);
			 int             __fastcall ExecutaTask(AnsiString cfgFilePath, AnsiString pid, bool na_memoria);
			 AnsiString      __fastcall FillHifenProcessName(void);
//			 bool 			 __fastcall GetParametros(AnsiString cfgFilePath);
			 void 			 __fastcall IniciaTmp(void);
			 void 			 __fastcall LimpaTmp(void);
			 bool 			 __fastcall LogInicia(AnsiString log_path);
			 void 			 __fastcall LogFecha(void);
//             AnsiString      __fastcall ReadLogFilepath(AnsiString cfgFilePath);
//			 bool 			 __fastcall ValidaPasta(AnsiString dir_path);

    protected: //métodos acessados via property
	 //getters
	 virtual AnsiString __fastcall PM_GetLogFilename(void) = 0;
	 virtual AnsiString __fastcall PM_GetProcessName(void) = 0;
	 virtual AnsiString __fastcall PM_GetProcessDescription(void) = 0;

	protected:// Objeto Externo
		#define WINREG_DIR_BASE      AnsiString("dir_base")
		#define WINREG_ARQ_REDE      AnsiString("arq_rede")

	protected: // dados
//        bool   log_privado;
		VTLog *log_;
        VTLog  *log_detalhado_;
		VTApl *apl_;
		bool    aplIsExterno;
		AnsiString pid_batch_;
};

#endif

// -----------------------------------------------------------------------------
// eof
