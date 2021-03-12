// arquivos incluídos ---------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <inifiles.hpp>
#include <System.IOUtils.hpp>

#include "TBatchProcess.h"
#include "VTBatchTask.h"
#include "VTBatchCfg.h"

#include <DateUtils.hpp>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTDir.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Geral\VTInfoset.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Produto\VTProduto.h>
#include <PlataformaSinap\Fontes\Registro\VTRegistro.h>
// ---------------------------------------------------------------------------
#include <PlataformaSinap\DLL_Inc\Diretorio.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Log.h>
#include <PlataformaSinap\DLL_Inc\Registro.h>
#include <PlataformaSinap\DLL_Inc\SinapApl.h>
// ---------------------------------------------------------------------------
__fastcall TBatchProcess::TBatchProcess(VTApl *apl_owner)
{
	// initializes the Component Object Model(COM) library
	CoInitialize(NULL);
	// Inicializa variável PID
	pid_batch_ = "";
    aplIsExterno = false;
	// cria objeto Apl local
	apl_ = CreateApl(apl_owner);
//	log_ = (VTLog*)apl_->GetObject(__classid(VTLog));
//	log_privado = false;
//	if (log_ == NULL)
//	{   // cria objeto log
//		apl_->Add(log_ = DLL_NewObjLog(apl_));
//		log_privado = true;
//	}
	log_ = DLL_NewObjLog(apl_);
	log_detalhado_ = DLL_NewObjLog(apl_);
	// cria objeto batch
	batch_task_ = CreateBatchTask();
	batch_cfg_   = CreateBatchCfg();
	// inicia pasta temporaria para batch
	DefineDiretorioInstalacao();
	IniciaTmp();
}

// ---------------------------------------------------------------------------
__fastcall TBatchProcess::~TBatchProcess(void)
{
	// limpa tmp
	LimpaTmp();
	// destroi objetos  process_cfg_
	if (batch_cfg_ != NULL)
	{
		delete batch_cfg_;
		batch_cfg_ = NULL;
	}
	if (batch_task_ != NULL)
	{
		apl_->Remove(batch_task_);
		delete batch_task_;
		batch_task_ = NULL;
	}
	if (log_ != NULL)
	{
		delete log_;
		log_ = NULL;
	}
	if (log_detalhado_ != NULL)
	{
		delete log_detalhado_;
		log_detalhado_ = NULL;
	}
	if ((!aplIsExterno) && (apl_ != NULL))
	{
		delete apl_;
		apl_ = NULL;
	}
}
// ---------------------------------------------------------------------------
VTApl* __fastcall TBatchProcess::CreateApl(VTApl *apl_owner)
{
	VTApl *apl = NULL;

	try
	{
		if (apl_owner != NULL)
		{
			aplIsExterno = true;
			apl = apl_owner;
		}
		else
		{
			aplIsExterno = false;
			apl = DLL_NewObjAplBatch(NULL, NULL);
        }

	}
	catch (...)
	{
		throw new Exception("Erro ao criar o Apl no TBatchProcess");
	}

	return apl;
}
// ---------------------------------------------------------------------------
bool __fastcall TBatchProcess::DefineDiretorioInstalacao(void)
{
	// variáveis locais
	VTRegistro *registro;
	VTPath *path = (VTPath*)apl_->GetObject(__classid(VTPath));
	VTProduto *produto = (VTProduto*)apl_->GetObject(__classid(VTProduto));
	AnsiString chave = produto->Sinapsis;
	AnsiString secao = produto->WinRegister;
	AnsiString info = WINREG_DIR_BASE;
	AnsiString arq_rede = "";
	AnsiString dir_base = "";

	try
	{ // verifica se o diretório base foi recebido no comando de execução

		// cria objeto Registro para leitura do registro do Windows
		if ((registro = DLL_NewObjRegistro(chave)) == NULL)
			return (false);
		// lê chave com informação de diretório base de instalação do programa
		if (registro->ExisteInfo(secao, WINREG_DIR_BASE, dir_base))
		{
			path->DirBase = dir_base;
		}
		// lê chave com informação da rede
		if (registro->ExisteInfo(secao, WINREG_ARQ_REDE, arq_rede))
		{
			path->ArqRede = arq_rede;
		}
		// destrói objeto
		delete registro;
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (DirectoryExists(dir_base));
}
// ---------------------------------------------------------------------------
// As duas funções acabaram se uniformizando...
// ---------------------------------------------------------------------------
int __fastcall TBatchProcess::ExecuteInMemory(AnsiString cfgFilePath, AnsiString pid)
{
	int retorno = 99;

	retorno = ExecutaTask(cfgFilePath,pid, true);

	return retorno;
}
// ---------------------------------------------------------------------------
int __fastcall TBatchProcess::Executa(AnsiString cfgFilePath, AnsiString pid)
{
	int retorno = 99;

	retorno = ExecutaTask(cfgFilePath,pid, false);

	return retorno;
}
// ---------------------------------------------------------------------------
int __fastcall TBatchProcess::ExecutaTask(AnsiString cfgFilePath, AnsiString pid, bool na_memoria)
{
    // variaveis locais
	bool sucesso = false;
	int retorno = 99;
	AnsiString logDir = "";

	__try
	{
		try
		{
			pid_batch_ = pid;
			batch_cfg_->PID = pid_batch_;
			//se o caminho for vazio, usa o caminho pré configurado
			if (cfgFilePath.IsEmpty())
			{
				logDir = ExtractFileDir(batch_cfg_->LogFilepath);
			}
			else
			{
				logDir = batch_cfg_->GetLogDir(cfgFilePath);
            }
			// inicia parâmetros do log
			if (!LogInicia(logDir))
			{ // nao existe caminho para inserir log
				retorno = 98;
				return retorno;
//				throw new Exception("Não existe caminho para inserir log.");
			}
			//indica log ao cfg
			if ((batch_cfg_ != NULL) && (batch_cfg_->Log == NULL))
			{
                batch_cfg_->Log = log_;
			}
			// verifica se o diretorio de instalação é valido
			if (!DefineDiretorioInstalacao())
			{ // invalido
                if (batch_task_ != NULL)
				{
					batch_task_->LogAdd("Diretorio de instalação é inválido.", false);
				}
				retorno = 97;
				return retorno;
//				throw new Exception("Diretorio de instalação é inválido.");
			}
			// leitura dos parametros para a tarefa batch
			if (batch_cfg_->GetParametros(cfgFilePath))
			{
				//valida parametros
				if (batch_cfg_->ValidaParametros())
				{
					// cria e executa objeto VTBatchProcess
					if (batch_task_ != NULL)
					{
                        //configura tarefa
						batch_task_->ConfiguraTarefa(batch_cfg_);
						// guarda atributos
						batch_task_->LogAdd("-----------------------------"+ FillHifenProcessName() + "------", false);
						batch_task_->LogAdd("------DESCRIÇÃO DO PROCESSO: "+this->ProcessName.UpperCase() + "------",false);
						batch_task_->LogAdd("-----------------------------"+ FillHifenProcessName() + "------", false);
						batch_task_->LogAdd("Inicio do processo de " +this->ProcessName + ".");
						// executa
						if (na_memoria)
							{sucesso = batch_task_->ExecuteInMemory(batch_cfg_);}
						else
							{sucesso = batch_task_->Executa(batch_cfg_);}
					}
					if (sucesso)
						// salvar na pasta de ajustadas
					{
						batch_task_->LogAdd("Processamento concluído com sucesso");
						retorno = 0;
					}
					else
						// salvar nao ajustadas
					{
						batch_task_->LogAdd("Processamento não concluído");
					}
				}
				else
				{
                    batch_task_->LogAdd("Parametros inválidos");
                }
            }
			LogFecha();
		}
		catch (Exception &e)
		{
			if (log_)
			{
				log_->Add("Erro:" + e.Message);
			}
			LogFecha();
			if (retorno == 0)
			{
				retorno = 99;
			}
		}
		catch (...)
		{
			if (log_)
			{
				log_->Add("Erro inesperado.");
			}
			LogFecha();
			if (retorno == 0)
			{
				retorno = 99;
			}
        }
	}
	__finally
	{
		if (batch_cfg_ != NULL)
		{
			if (retorno != 0)
				{batch_cfg_->LogAddResumo("##### FALHA em "+ this->ProcessName + " #####", "FALHA");}
		}
		LogFecha();
	}
	return retorno;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TBatchProcess::FillHifenProcessName(void)
{
	AnsiString hifens = "";
	int lenght = 0;

	lenght = this->ProcessName.Length();

	for (int n = 0; n < lenght; n++)
	{
		hifens += "-";
	}

	return hifens;
}
// ---------------------------------------------------------------------------
void __fastcall TBatchProcess::IniciaTmp(void)
{
	// cria o diretorio temporario para a tarefa batch
	// Isto permite executar em paralelo, pois cada processo(instancia do sinap) utiliza
	// um diretorio tmp proprio

	// variaveis locais
	// AnsiString tempWindowsDir;
	// TGUID g;
	VTPath* path = (VTPath*)apl_->GetObject(__classid(VTPath));

	// // pega o diretorio temporario do windows(diretorio temporario do usuario)
	// tempWindowsDir = TPath::GetTempPathW();
	//
	// // cria um UID para cada processo batch:
	// CreateGUID(g);
	//
	// // define o diretorio temporario para estah instancia do sinap <tmp do windows>\<UID>
	// path->DirTmpBatch = (tempWindowsDir + "\\" + Sysutils::GUIDToString(g));
	//
	// // cria o diretorio temporario
	// CreateDirectoryA(path->DirTmpBatch.c_str(), NULL);

	// assume diretorio TmpBatch como sendo o Tmp do Sinap (com o devido tratamento da instancia em execução)
	path->DirTmpBatch = path->DirTmp();

}
// -----------------------------------------------------------------------------
void __fastcall TBatchProcess::LimpaTmp(void)
{
	// variaveis locais
	VTDir *dir = (VTDir*)apl_->GetObject(__classid(VTDir));

	// apaga arquivos temporarios antigos
	dir->DeleteTmp();
}

// -----------------------------------------------------------------------------
bool __fastcall TBatchProcess::LogInicia(AnsiString log_path)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl_->GetObject(__classid(VTPath));
	AnsiString caminho_destino, caminho_destino_correcao;
	AnsiString arq_log, arq_log_det, agora;
	bool sucesso = true;

	try
	{
		// recupera o nome da pasta onde estao os logs
		caminho_destino = log_path + "\\" + pid_batch_;

		// verifica se tem barra no final
		if ((caminho_destino.SubString(caminho_destino.Length(), 1)).AnsiCompareIC("\\") == 0)
		{ // corta ultimo caractere
			caminho_destino = caminho_destino.SubString(0, caminho_destino.Length() - 1);
		}
		if (pid_batch_.AnsiCompareIC("") == 0)
		{
			// cria pasta com data e hora e altera caminho destino
			agora = Date().FormatString("yyyymmdd") + Time().FormatString("hhmmsszzz");
			caminho_destino = caminho_destino + "\\" + agora;
		}
		if (!ForceDirectories(caminho_destino))
		{
			return false;
		}
		// determina caminho do arquivo
		arq_log = caminho_destino + "\\" + this->LogFilename;
		arq_log_det = caminho_destino + "\\" + "Detalhado_" + this->LogFilename;
		if (log_)
		{
//			if(log_privado)
//			{
//				log_->Open(arq_log);
//				//acerta a nova config de log
//				batch_cfg_->LogFilepath = arq_log;
//			}
			log_->Open(arq_log, "a");
			//acerta a nova config de log
			batch_cfg_->LogFilepath = arq_log;
			batch_task_->LogAdd("-- " + this->ProcessName + " -- ", false);
			batch_task_->LogAdd(" - Descrição geral - ", false);
		}
		if (log_detalhado_)
		{
//			if(log_privado)
//			{
//				log_->Open(arq_log);
//				//acerta a nova config de log
//				batch_cfg_->LogFilepath = arq_log;
//			}
			log_detalhado_->Open(arq_log_det, "a");
			//acerta a nova config de log
			batch_task_->log_detalhado_ = log_detalhado_;
			batch_task_->LogDetalheAdd("-- " + this->ProcessName + " -- ");
			batch_task_->LogDetalheAdd(" - Descrição geral - ");
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	catch (...)
	{
		sucesso = false;
    }

	return sucesso;
}

// -----------------------------------------------------------------------------
void __fastcall TBatchProcess::LogFecha(void)
{
	// salva no arquivo
	if (log_)
		log_->Close();
}
// -----------------------------------------------------------------------------
// eof
