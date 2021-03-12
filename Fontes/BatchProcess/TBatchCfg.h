// ---------------------------------------------------------------------------
#ifndef TBatchCfgH
#define TBatchCfgH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <PlataformaSinap\Fontes\BatchProcess\VTBatchCfg.h>
#include "VTBatchCfg.h"
// ---------------------------------------------------------------------------
class VTLog;
class VTDir;
// ---------------------------------------------------------------------------
class TBatchCfg : public VTBatchCfg
{
	public:
					__fastcall TBatchCfg(void);
					__fastcall ~TBatchCfg(void);
    		 void 	__fastcall AdicionaPIDnoPath(AnsiString &filedir);
	 virtual void   __fastcall CopiaParametros(VTBatchCfg *original) = 0;
	 AnsiString		__fastcall GetLogDir(AnsiString cfgFilePath);
	 bool 			__fastcall GetParametros(AnsiString cfgFilePath); //caminho do arquivo de config, precisa ter extensão correta
	 virtual void   __fastcall ValoresDefault(void) = 0;
	 virtual bool   __fastcall ValidaParametros(void) = 0;
			 void 	__fastcall LogAddResumo(AnsiString txt_log, AnsiString tipo_erro);

	protected:

			 AnsiString	__fastcall GetLogINI(AnsiString iniFilePath);
//			 AnsiString	__fastcall GetLogJSON(AnsiString jsonFilePath);
			 bool 	   	__fastcall GetParametrosINI(AnsiString iniFilePath);
			 bool		__fastcall GetParametrosJSON(AnsiString jsonFilePath);
	 // deve lançar exceções do tipo Exception caso falhe em algo
	 virtual AnsiString	__fastcall GetLogINI(TIniFile *ini)= 0;
	 virtual bool   	__fastcall GetParametrosINI(TIniFile *ini)= 0;
	 // deve lançar exceções do tipo Exception caso falhe em algo
//	 virtual AnsiString	__fastcall GetLogJSON(TJSONObject *json) throw(Exception) = 0;
//	 virtual bool   	__fastcall GetParametrosJSON(TJSONObject *json) throw(Exception) = 0;
			 bool   	__fastcall LeArquivosPastaOrigem(VTDir * dir, AnsiString filedir, TStrings *lisFILE, AnsiString fileExt);
			 void 		__fastcall LogAdd(AnsiString txt_log, bool time = true);
			 void 		__fastcall ParseKeyValue(TStrings *orig, TStrings *destino);

	 //validacoes padrão
     bool __fastcall ValidaOpcaoInteiro(int min_valor, int max_valor, int valor_lido);
	 bool __fastcall ValidaOpcaoDouble(double min_valor, double max_valor, double valor_lido);
	 bool __fastcall ValidaPasta(AnsiString dir_path, bool force_create);


	protected: //métodos acessados via property
			  bool		 __fastcall PM_GetIgnorarFalha(void);
	  virtual AnsiString __fastcall PM_GetLogFilepath(void) = 0;
			  VTLog*	 __fastcall PM_GetLog(void);
			  VTLog*	 __fastcall PM_GetLogResumo(void);
          	  AnsiString __fastcall PM_GetPID(void);
			  void       __fastcall PM_SetIgnorarFalha(bool ignorar);
	  virtual void       __fastcall PM_SetLogFilepath(AnsiString filepath) = 0;
	  		  void       __fastcall PM_SetLog(VTLog *log);
			  void       __fastcall PM_SetLogResumo(VTLog *logResumo);
              void       __fastcall PM_SetPID(AnsiString pid);
	  virtual void       __fastcall PM_SetDefaults(VTBatchCfg *defaults) = 0;

	protected:
	 // objetos externos
	 VTLog *log_;
	 VTLog *logResumo_;
	 bool  ignorarFalha_;
     AnsiString pid_;
};

// ---------------------------------------------------------------------------
#endif
// eof
