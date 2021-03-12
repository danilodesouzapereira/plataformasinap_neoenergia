// ---------------------------------------------------------------------------
#include <vcl.h>
#include <vcl.h>
#pragma hdrstop
#include <windows.h>
#include <System.SysUtils.hpp>
#include <inifiles.hpp>
#include <PlataformaSinap\Fontes\Diretorio\VTDir.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Log\VTLog.h>
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#include "TBatchCfg.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
__fastcall TBatchCfg::TBatchCfg(void)
{
	// salva ponteiro para objetos
	this->log_       = NULL;
	this->logResumo_ = NULL;
	this->ignorarFalha_ = false;
    this->pid_ = "";
}

// ---------------------------------------------------------------------------
__fastcall TBatchCfg::~TBatchCfg(void)
{

}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::AdicionaPIDnoPath(AnsiString &filedir)
{
	if (!filedir.IsEmpty())
	{
		if (!this->PID.IsEmpty())
		{
            filedir += "\\" + this->PID;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::CopiaParametros(VTBatchCfg *original)
{
	this->log_       = original->Log;
	this->logResumo_ = original->LogResumo;
	this->ignorarFalha_ = original->IgnorarFalha;
	this->pid_ = original->PID;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TBatchCfg::GetLogDir(AnsiString cfgFilePath)
{
	AnsiString extensao = "";
	AnsiString filename = "";
	AnsiString logFilename = "";

	try
	{
		//verifica existencia do arquivo
		if (!FileExists(cfgFilePath))
		{
			LogAdd("Arquivo de configuração não encontrado: " +  cfgFilePath);
			return "";
		}
		//verifica extensão
		filename = ExtractFileName(cfgFilePath);
		extensao = ExtractFileExt(filename);
		//verifica qual é
		if (extensao.AnsiCompare(".ini") == 0)
		{
			logFilename = GetLogINI(cfgFilePath);
		}
		else if (extensao.AnsiCompare(".json") == 0)
		{
//			logFilename = GetLogJSON(cfgFilePath);
			LogAdd("Leitura de arquivos .json não implementada");
			return "";
//			throw Exception("Leitura de arquivos .json não implementada");
		}

	}
	catch (...)
	{
		logFilename = "";
	}

	return logFilename;
}
// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::GetParametros(AnsiString cfgFilePath)
{
	AnsiString extensao = "";
	AnsiString filename = "";
	bool sucesso = true;

	try
	{
		LogAdd("--------------------------------------------------------", false);
		LogAdd("-----------CONFIGURAÇÃO INICIAL DAS TAREFAS-------------", false);
		LogAdd("--------------------------------------------------------", false);
		//verifica se está vazio, se estiver, já está configurado
		if (cfgFilePath.IsEmpty())
		{
			LogAdd("Caminho vazio, tarefa inicializada com as configurações já definidas.");
            return true;
		}
		//verifica existencia do arquivo
		if (!FileExists(cfgFilePath))
		{
			LogAdd("Arquivo de configuração não encontrado: " +  cfgFilePath);
			return false;
		}
		//verifica extensão
		filename = ExtractFileName(cfgFilePath);
		extensao = ExtractFileExt(filename);
		//verifica qual é
		if (extensao.AnsiCompare(".ini") == 0)
		{
			sucesso = GetParametrosINI(cfgFilePath);
		}
		else if (extensao.AnsiCompare(".json") == 0)
		{
//			sucesso = GetParametrosJSON(cfgFilePath);
			LogAdd("Leitura de arquivos .json não implementada");
			return "";
			//throw Exception("Leitura de arquivos json não implementada");
		}

	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TBatchCfg::GetLogINI(AnsiString iniFilePath)
{
	// carrega arquivo .ini
	TIniFile *ini = new TIniFile(iniFilePath);
	AnsiString logFilename = "";
	byte orig_separador;
    // determina separador configurado no sistam
	orig_separador = FormatSettings.DecimalSeparator;

	FormatSettings.DecimalSeparator = ',';
	try
	{
		logFilename = GetLogINI(ini);
	}
	catch (Exception &e)
	{
		LogAdd("Erro ao ler arquivo ini : " + iniFilePath);
		LogAdd("Erro: " + e.Message);
		logFilename = "";
	}
	catch (...)
	{
		LogAdd("Erro ao ler arquivo ini : " + iniFilePath);
		LogAdd("Erro inesperado");
		logFilename = "";
	}
	FormatSettings.DecimalSeparator = orig_separador;
	delete ini;

	return logFilename;
}
//// ---------------------------------------------------------------------------
//AnsiString __fastcall TBatchCfg::GetLogJSON(AnsiString jsonFilePath)
//{
//	TJSONObject *jRoot;
//	TStringList *linhasArq;
//	// carrega arquivo .ini
//	AnsiString logFilename = "";
//
//	try
//	{
//		linhasArq = new TStringList();
//		// leitura do arquivo
//		linhasArq->LoadFromFile(jsonFilePath);
//		//criação objeto JSON
//		jRoot = (TJSONObject*)TJSONObject::ParseJSONValue(linhasArq->Text);
//		//lê os campos do json
//		logFilename = GetLogJSON(jRoot);
//	}
//	catch (Exception &e)
//	{
//		LogAdd("Erro ao ler arquivo json : " + jsonFilePath);
//		LogAdd("Erro: " + e.Message);
//		logFilename = "";
//	}
//	catch (...)
//	{
//		LogAdd("Erro ao ler arquivo json : " + jsonFilePath);
//		LogAdd("Erro inesperado");
//		logFilename = "";
//	}
//	if (linhasArq)
//	{
//		delete linhasArq;
//	}
//	if (jRoot)
//	{
//		delete jRoot;
//	}
//
//	return logFilename;
//}
// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::GetParametrosINI(AnsiString iniFilePath)
{
	// carrega arquivo .ini
	TIniFile *ini = new TIniFile(iniFilePath);
	bool sucesso = true;
	byte orig_separador;
    // determina separador configurado no sistam
	orig_separador = FormatSettings.DecimalSeparator;

	FormatSettings.DecimalSeparator = ',';
	try
	{
		sucesso &= GetParametrosINI(ini);
	}
	catch (Exception &e)
	{
		LogAdd("Erro ao ler arquivo ini : " + iniFilePath);
		LogAdd("Erro: " + e.Message);
		sucesso = false;
	}
	catch (...)
	{
		LogAdd("Erro ao ler arquivo ini : " + iniFilePath);
		LogAdd("Erro inesperado");
		sucesso = false;
	}
	FormatSettings.DecimalSeparator = orig_separador;
	delete ini;

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::LeArquivosPastaOrigem(VTDir * dir, AnsiString filedir,
										TStrings *lisFILE, AnsiString fileExt)
{
	TStrings *lisTEMP;
	AnsiString pastaComPID ="";
	bool sucesso = true;

	//protecao
	if (dir == NULL)
	{
		return false;
	}
	lisTEMP = new TStringList;
	//verifica se deve olhar dentro da pasta indicada, ou dentro de uma pasta PID
	pastaComPID = filedir;
	AdicionaPIDnoPath(pastaComPID);
	if (DirectoryExists(pastaComPID))
	{
		filedir = pastaComPID;
	}
	else
	{
        //faz nada
    }
	if (!dir->Arquivos(filedir, lisTEMP, fileExt))
	{
		if(log_!=NULL)
			{log_->Add(" A pasta não contém arquivos " + fileExt);}
	}
	if (lisTEMP->Count == 0)
	{
		if(log_!=NULL)
			{log_->Add(" A pasta não contém arquivos " + fileExt);}
			sucesso = false;
	}
	for (int ns = 0; ns < lisTEMP->Count; ns++)
	{
		lisFILE->Add(filedir + "\\" + lisTEMP->Strings[ns]);
	}
	delete lisTEMP;

	return sucesso;
}
//// ---------------------------------------------------------------------------
//bool __fastcall TBatchCfg::GetParametrosJSON(AnsiString jsonFilePath)
//{
//	TJSONObject *jRoot;
//	TStringList *linhasArq;
//	// carrega arquivo .ini
//	bool sucesso = true;
//
//	try
//	{
//		linhasArq = new TStringList();
//        // leitura do arquivo
//		linhasArq->LoadFromFile(jsonFilePath);
//		//criação objeto JSON
//		jRoot = (TJSONObject*)TJSONObject::ParseJSONValue(linhasArq->Text);
//		//lê os campos do json
//		sucesso &= GetParametrosJSON(jRoot);
//	}
//	catch (Exception &e)
//	{
//		LogAdd("Erro ao ler arquivo json : " + jsonFilePath);
//		LogAdd("Erro: " + e.Message);
//		sucesso = false;
//	}
//	catch (...)
//	{
//		LogAdd("Erro ao ler arquivo json : " + jsonFilePath);
//		LogAdd("Erro inesperado");
//		sucesso = false;
//	}
//	if (linhasArq)
//	{
//		delete linhasArq;
//	}
//	if (jRoot)
//	{
//		delete jRoot;
//	}
//
//	return sucesso;
//}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::LogAdd(AnsiString txt_log, bool time)
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
	}
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::LogAddResumo(AnsiString txt_log, AnsiString tipo_erro)
{
	if (logResumo_ != NULL)
	{
		logResumo_->DefineValor("Tipo",tipo_erro);
		logResumo_->Add(txt_log);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::ParseKeyValue(TStrings *orig, TStrings *destino)
{
	TStringList *campos = NULL;
	AnsiString function = __FUNC__;
	AnsiString valor;

	try
	{
		if ((destino == NULL) || (orig == NULL))
		{
			return;
		}
		campos = new TStringList;
		destino->Clear();
		//percorre todos strings originais
		for (int ns = 0; ns < orig->Count; ns++)
		{
			campos->Clear();
			ExtraiStrings(orig->Strings[ns],"=",campos);
			valor = campos->Strings[1];
            destino->Add(valor);
		}
		delete campos;
	}
	catch (...)
	{
		LogAdd("Erro em " + function);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::PM_GetIgnorarFalha(void)
{
	return (ignorarFalha_);
}
// ---------------------------------------------------------------------------
VTLog* __fastcall TBatchCfg::PM_GetLog(void)
{
	return (log_);
}
// ---------------------------------------------------------------------------
VTLog* __fastcall TBatchCfg::PM_GetLogResumo(void)
{
	return (logResumo_);
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TBatchCfg::PM_GetPID(void)
{
	return (this->pid_);
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::PM_SetIgnorarFalha(bool ignorar)
{
	ignorarFalha_ = ignorar;
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::PM_SetLog(VTLog *log)
{
	log_ = log;
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::PM_SetLogResumo(VTLog *logResumo)
{
	logResumo_ = logResumo;
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::PM_SetPID(AnsiString pid)
{
	this->pid_ = pid;
}
// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::ValidaOpcaoInteiro(int min_valor, int max_valor, int valor_lido)
{
	bool valido = true;

	if (valor_lido < min_valor)
	{
		return false;
	}
	if (valor_lido > max_valor)
	{
		return false;
	}

	return valido;
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::ValidaOpcaoDouble(double min_valor, double max_valor,
	double valor_lido)
{
	bool valido = true;

	if (valor_lido < min_valor)
	{
		return false;
	}
	if (valor_lido > max_valor)
	{
		return false;
	}

	return valido;
}
// ---------------------------------------------------------------------------
bool __fastcall TBatchCfg::ValidaPasta(AnsiString dir_path, bool force_create)
{
	bool valido = true;

	//verifica vazio
	if (dir_path.IsEmpty())
		{return false;}
	//valida existencia
	if (!DirectoryExists(dir_path))
	{ 	//diretorio não existe, tenta criar
		if (force_create)
		{
            if (!ForceDirectories(dir_path))
			{ //se falhou ai já era
				return false;
			}
		}
		else
		{
            valido = false;
        }

	}

	return valido;
}
// ---------------------------------------------------------------------------
void __fastcall TBatchCfg::ValoresDefault(void)
{
	this->log_       = NULL;
	this->logResumo_ = NULL;
	this->ignorarFalha_ = false;
	this->pid_ = "";
}
// ---------------------------------------------------------------------------
// eof
