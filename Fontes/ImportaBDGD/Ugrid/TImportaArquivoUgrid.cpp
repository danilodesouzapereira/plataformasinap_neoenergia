// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <algorithm>
#include <inifiles.hpp>
// DLL Include
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Progresso.h>
// Fontes Include
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\Rede\BD_Inc.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>
#include <Fontes\LocalDB\SQLite\ESQLite.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\ImportaRedesBatch\VTBatchImportaRedesTask.h>
//
#include "..\Exceptions\EImportaUgrid.h"
#include "..\Entidades\TCapacitorBDGD.h"
#include "..\Entidades\TCargaBDGD.h"
#include "..\Entidades\TCargaIP.h"
#include "..\Entidades\TChaveBDGD.h"
#include "..\Entidades\TGeradorBDGD.h"
#include "..\Entidades\TRamalLigacao.h"
#include "..\Entidades\TRedeBT.h"
#include "..\Entidades\TReguladorMT.h"
#include "..\Entidades\TTrafoDistribuicao.h"
#include "..\Entidades\TTrechoBDGD.h"
#include "..\Commons\Entidades\TSubestacao.h"
#include "..\Commons\Entidades\TBarramento.h"
#include "..\Commons\Entidades\TRedeMT.h"
#include "..\Commons\VTCommons.h"
#include "..\PreMonta\VTPreMonta.h"
#include "..\Ugrid\VTImportador.h"
#include "..\Ugrid\BDGDDAO.h"

#include "TImportaArquivoUgrid.h"
// ---------------------------------------------------------------------------
using namespace std;

// ---------------------------------------------------------------------------
VTImportador* NewObjImportadorUgrid(VTApl *apl, UnicodeString filename)
{
	return (new TImportaArquivoUgrid(apl, filename));
}

// -----------------------------------------------------------------------------
__fastcall TImportaArquivoUgrid::TImportaArquivoUgrid(VTApl *apl, UnicodeString uGridPath)
//throw (runtime_error)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// define o diretorio temporario
	DefinirDiretorioTemporario();
	// cria progresso
	progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	// guarda o path do arquivo .egrid
	this->PD.gridFilePath = uGridPath;
	// cria objeto zip e extrai arquivos
//	if(gridPath.Length() > 0)
//	{
//		IniciaZip(gridPath);
//	}
	// inicia o parametro batch
	PD.batch = false;
	PD.arqEgrid = false;
}

// -----------------------------------------------------------------------------
__fastcall TImportaArquivoUgrid::~TImportaArquivoUgrid(void)
{
	if (slisNomes != NULL)
	{
		delete slisNomes;
		slisNomes = NULL;
	}
	this->Close();
}

// -----------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::Close(void)
{
	try
	{
		if (DirectoryExists(diretorioTemporario))
			TDirectory::Delete(diretorioTemporario, true);
	}
	catch (...)
	{
	}
}
// -------------------------------------------------------------------------------------------------
VTCommons* __fastcall TImportaArquivoUgrid::CriaCommons(UnicodeString filepathXGRID)
{
	VTCommons *commons_aux = NULL;
	bool batch_mode_original;

	try
	{
		if (FileExists(filepathXGRID))
		{	//primeiro dezipa o egrid
			IniciaZip(filepathXGRID);
			//cria dao em batch mode
			batch_mode_original = this->BatchMode;
			this->BatchMode = false;
			//cria dao
			CriaDAO("");
			//cria commons
			commons_aux = NewObjCommons(apl, NULL); //deve ser destruid por quem  cria
			//le o comomons
			ReadCommons(commons_aux);
			//destroi o dao
            DestroiDAO();
		}
		else
		{
			throw Exception("Arquivo : " + filepathXGRID +" não existe.");
		}

	}
	catch(Exception &e)
	{
        if (commons_aux)
		{
			delete commons_aux;
			commons_aux = NULL;
		}
		throw;
    }
	catch (...)
	{
		if (commons_aux)
		{
			delete commons_aux;
			commons_aux = NULL;
		}
		throw Exception("Erro inesperado em CriaCommons(" + filepathXGRID +")");
    }

	return commons_aux;
}
// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::CriaDAO(AnsiString nomeArq)
{
	if (dao == NULL)
	{ // verifica se não foi passado o nome do arquivo (caso contrario, sera arquivo de configuração)
		if (nomeArq.IsEmpty())
			dao = new BDGDDAO(arquivoConfiguracaoPath);
		else
			dao = new BDGDDAO(GetPath(nomeArq), arquivoConfiguracaoPath);
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::DestroiDAO(void)
{
	if (dao)
	{
		dao->Close();
		delete dao;
		dao = NULL;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::DefinirDiretorioTemporario()
{
	// pega o diretorio temporario do windows(diretorio temporario do usuario)
	AnsiString tempWindowsDir = TPath::GetTempPathW();

	// cria um UID para cada importacao
	TGUID g;
	CreateGUID(g);

	// define o diretorio temporario
	this->diretorioTemporario = (tempWindowsDir + Sysutils::GUIDToString(g));
	try
	{ // cria o diretorio temporario
		TDirectory::CreateDirectory(diretorioTemporario);
	}
	catch (...)
	{ // se nao foi possivel criar um diretorio temporario no tmp do windows, cria no tmp do sinap
		VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

		// define o diretorio temporario
		this->diretorioTemporario = path->DirTmp() + "\\" + Sysutils::GUIDToString(g);
		// cria o diretorio temporario
		TDirectory::CreateDirectory(diretorioTemporario);
	}
}

// -------------------------------------------------------------------------------------------------
UnicodeString TImportaArquivoUgrid::GetPath(UnicodeString siglaSubestacao)
{
	return (diretorioTemporario + "\\Sub_" + siglaSubestacao + ".es");
}

// -------------------------------------------------------------------------------------------------
TStringList* __fastcall TImportaArquivoUgrid::GetSubestacaoList()
{
	// Variáveis locais
	TStringList *retorno = new TStringList;
	TStringDynArray Files = TDirectory::GetFiles(diretorioTemporario);

	for (int i = 0; i < Files.Length; i++)
	{
		// se for um arquivo .es
		if ((Files[i].Pos("Sub_") > 0) && (Files[i].Pos(".es") > 0))
		{
			UnicodeString siglaSub = Files[i].SubString((Files[i].Length() - 5), 3);
			retorno->Add(siglaSub);
		}
	}
	return retorno;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::InsereArranjo(void)
{
	// variáveis locais
    VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	TArranjoBDGD *arranjo;
	std::map<int,TArranjoBDGD*>mapArranjo = commons->GetmapArranjo();
	std::map<int,TArranjoBDGD*>::iterator it;
	pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));

	// insere Arranjo para Condutor
	// insere em CLBoxPrimario os Primarios associados a Subestaçoes selecionadas
	for (it = mapArranjo.begin(); it != mapArranjo.end(); it++)
	{
		arranjo = it->second;
		// insere Arranjo em PreMonta
		pre_monta->InsereCfg_Arranjo(arranjo);
	}
	return (true);
}


// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::IniciaZip(UnicodeString gridPath)
{
	VTBatchImportaRedesTask *batch = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));
	AnsiString versao;

	// guarda o path do arquivo .egrid
	this->PD.gridFilePath = gridPath;
	// cria zip
	zip = new TZipFile();
	try
	{ // abre o arquivo
		zip->Open(gridPath, zmRead);
	}
	catch (const Exception & e)
	{
		AnsiString msg = ("Arquivo inválido ou corrompido! (" + e.Message + ")");
		throw std::runtime_error(msg.c_str());
	}
	try
	{ // verifica se a versão do Egrid é compativel
		if (!VersaoCompativel(versao))
		{
			AnsiString msg = ("Versão inválida da extração: " + versao);
			throw std::runtime_error(msg.c_str());
		}
		if(slisNomes == NULL)
		{
			slisNomes = new TStringList();
		}
		slisNomes->Clear();
		Nomes(slisNomes);
	}
	catch (const Exception & e)
	{
		AnsiString msg = ("Erro ao abrir o arquivo! (" + e.Message + ")");
		throw std::runtime_error(msg.c_str());
	}
	// define o arquivo .es que serah utilizado para obter as configuracoes da empresa e tabelas de domínio (COMMONS)
	if (zip->IndexOf(FILENAME_COMMONS) >= 0)
	{
		zip->Extract(FILENAME_COMMONS, diretorioTemporario, true);
		arquivoConfiguracaoPath = (diretorioTemporario + "\\" + FILENAME_COMMONS);
	}
	// fecha o arquivo
	try
	{
		zip->Close();
	}
	catch (const Exception & e)
	{
        if (batch != NULL)
		{
			batch->LogDetalheAdd("Erro ao fechar o arquivo! (" + e.Message + ")");
		}
		else
		{
			Erro("Erro ao fechar o arquivo! (" + e.Message + ")");
        }
	}
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ExisteSubZip(AnsiString sigla_sub)
{
	// variaveis locais
	AnsiString nome;

	for (int i = 0; i < slisNomes->Count; i++)
	{
		nome = slisNomes->Strings[i];
		// if (nome.AnsiPos(sigla_sub) > 0)
		if (nome.AnsiCompareIC(sigla_sub) == 0)
		{
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::LeRedes(TSubestacao *subestacao, strOPImporta *opcao)
{
	// variáveis locais
	VTBatchImportaRedesTask *batch = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));
	bool sucesso = true;

	try
	{ // salva opções de importação
		this->opcao = opcao;
		// atualiza progresso
		if (!BatchMode)
		{
			if (progresso == NULL)
				progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso)
			{
				progresso->Add("Iniciando importação");
			}
		}
		// obtém ponteiro para objeto PreMonta
		pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
		// inicia a importação
		pre_monta->TrataInicioDeDados();
		// insere tipos de Chaves
        ReadCfg_TipoChave();
		// faz a leitura da Subestacao e de todas suas redes primárias e secundárias
		if (sucesso)
		{
			nomeSe = subestacao->Codigo;
			sucesso = LeUmaRedeEtd_TodasRedesPriSec(subestacao);
		}
		// atualiza progresso
		if (!BatchMode)
		{
			if (progresso == NULL)
				progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
			if (progresso)
				progresso->Add("Montagem das redes");
		}
		// finaliza a importação
		pre_monta->TrataFimDeDados();
	}
	catch (Exception &e)
	{
		if (!BatchMode)
		{
			Erro(e.Message);
		}
		else
		{
            if (batch != NULL)
			{
				batch->LogDetalheAdd("Erro em LeRedes : " + e.Message + ")");
			}
        }
		sucesso = false;
  }
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::LeUmaRedeEtd_TodasRedesPriSec(TSubestacao* subestacao) throw(EImportaUgrid)
{
	//
	TRedeMT *primario;

	TList *lisPRIMARIO = subestacao->LisPrimario();
	try
	{
		// INSERE ETD
		// lê dados da rede
		if (!ReadEtd_TabSe(subestacao->Id))
			return (false);
		// lê dados dos Barramentos
		//if (!ReadEtd_TabBarramento(subestacao->Id))
			//return (false);
		// lê dados dos transformadores
		//if (!ReadEtd_TabTrafoSe(subestacao->Id))
			//return (false);
		for (int n = 0; n < lisPRIMARIO->Count; n++)
		{ // lê a rede primária e todas suas redes secundárias
			primario = (TRedeMT*)lisPRIMARIO->Items[n];
			nomeCircuito = IntToStr(n + 1) + " de " + lisPRIMARIO->Count;
			// verifica se o primário foi selecionado
			if (!primario->Selecionado)
				continue;
			if (!LeUmaRedePri_TodasRedesSec(primario))
				continue;
		}
	}
	catch (Exception &e)
	{
		throw EImportaUgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::LeUmaRedePri_TodasRedesSec(TRedeMT *primario) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString prog;
	TList *lisREDESBT;
	TRedeBT* secundario;

	try
	{ // lê dados da rede
		if (!ReadPri_TabRede(primario))
			return (false);
        // As barras serão inseridas em quanto se inserem os equipamentos
		if (!ReadPri_TabTrecho(primario->Id))
			return (false);
		if (!ReadPri_TabChave(primario->Id))
			return (false);
		if (!ReadPri_TabRegulador(primario->Id))
			return (false);
		if (!ReadPri_TabCapacitor(primario->Id))
			return (false);
		if (!ReadPri_TabGerador(primario->Id))
			return (false);
		if (!ReadPri_TabCargaMT(primario->Id))
			return (false);
		// Faz a leitura das redes BT
        // lê dados da rede
		if (!ReadSec_TabRede(primario))
			return (false);
        if (!ReadSec_TabTrafoMTBT(primario))
			return (false);
		lisREDESBT = primario->LisSecundario();
        //
		for (int n = 0; n < lisREDESBT->Count; n++)
		{
			secundario = (TRedeBT*)lisREDESBT->Items[n];
			nomeSec = IntToStr(n + 1) + " de " + IntToStr(lisREDESBT->Count);
			// atualiza progresso
			if (!BatchMode)
			{
				if (progresso)
				{
					prog = "SED: " + nomeSe + " SDMT: " + nomeCircuito + " SDBT: " + nomeSec;
					progresso->Add(prog, true);
				}
			}
			// verifica como dever ser lida a rede secundária
			switch (opcao->sec.topologia)
			{
				case opSEC_GIS:
					sucesso = LeUmaRedeSec_Completa(secundario->Id_Rede_BT);
					break;
				case opSEC_UTC:
					sucesso = LeUmaRedeSec_TrafoCarga(secundario->Id_Rede_BT);
					break;
				case opSEC_CARGA:
					sucesso = LeUmaRedeSec_SomenteCarga(secundario->Id_Rede_BT);
					break;
			}
		}
	}
	catch (Exception &e)
	{
		throw EImportaUgrid(e.Message);
		sucesso = false;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::LeUmaRedeSec_Completa(int sec_id)
{

	try
	{
		if (!ReadSec_TabTrechoBT(sec_id))
			return (false);
		if (!ReadSec_TabRamalLig(sec_id))
			return (false);
		if (!ReadSec_TabChave(sec_id))
			return (false);
		if (!ReadSec_TabConsIP(sec_id))
			return (false);
		if (!ReadSec_TabConsBT(sec_id))
			return (false);
		if (!ReadSec_TabCapacitor(sec_id))
			return (false);
		if (!ReadSec_TabGeradorBT(sec_id))
			return (false);
	}
	catch (Exception &e)
	{
		throw EImportaUgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::LeUmaRedeSec_SomenteCarga(int sec_id)
{
	try
	{
		if (!ReadSec_TabConsIP(sec_id))
			return (false);
		if (!ReadSec_TabConsBT(sec_id))
			return (false);
		if (!ReadSec_TabGeradorBT(sec_id))
			return (false);
		if (!ReadSec_TabCapacitor(sec_id))
			return (false);
	}
	catch (Exception &e)
	{
		throw EImportaUgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::LeUmaRedeSec_TrafoCarga(int sec_id)
{
	try
	{
		if (!ReadSec_TabConsIP(sec_id))
			return (false);
		if (!ReadSec_TabConsBT(sec_id))
			return (false);
		if (!ReadSec_TabGeradorBT(sec_id))
			return (false);
		if (!ReadSec_TabCapacitor(sec_id))
			return (false);
	}
	catch (Exception &e)
	{
		throw EImportaUgrid(e.Message); ;
		return (false);
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::Nomes(TStringList *slisNomes)
{
	// variaveis locais
	AnsiString nome;
	AnsiString sub, subini, esfim;
	int pos_ini, pos_fim, length;
	int length_subini, length_esfim;

	subini = "Sub_";
	esfim = ".es";

	for (int i = 0; i < zip->FileCount; i++)
	{
		nome = zip->FileNames[i];
		pos_ini = nome.Pos("Sub_");
		length_subini = subini.Length();
		if (pos_ini <= 0)
		{
			continue;
		}
		pos_fim = nome.Pos(".es");
		if (pos_fim <= 0)
		{
			continue;
		}
		length = nome.Length();
		length_esfim = esfim.Length();
		sub = nome.SubString(pos_ini + length_subini, length - length_subini - length_esfim);
		if (slisNomes->IndexOf(sub) > 0)
		{
			continue;
		}
		slisNomes->Add(sub);
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadCommons(void)
{
	//variáveis locais
	bool sucesso = true;
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));

	sucesso = ReadCommons(commons);

	return (sucesso);
}
// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadCommons(VTCommons *commons)
{
	//variáveis locais
	VTBatchImportaRedesTask *batch = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));
	bool sucesso = true;

	if (this->dao == NULL)
	{
		if (batch != NULL)
			{batch->LogDetalheAdd("Não existe DAO");}
		else
			{Aviso("Não existe DAO");}
		return(false);
	}
	try
	{
		commons->LeDAO(this->dao);
		if (!commons->ReadTabelasDominio())
		{
			if (batch != NULL)
				{batch->LogDetalheAdd("Erro ao ler tabelas de dominio do commons");}
			return (false);
		}
		if (!commons->ReadEntidadesEmpresa())
		{
			if (batch != NULL)
				{batch->LogDetalheAdd("Erro ao ler tabelas de entidades empresa do commons");}
			return (false);
		}
	}
	catch (Exception &e)
	{
        if (batch != NULL)
			{batch->LogAdd("E");}
		throw EImportaUgrid(e.Message);
//		sucesso = false;
	}
	return (sucesso);
}
// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadCfg_TipoChave(void)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	std::map<int,TTipoUnidade*>mapTipoCh;
	std::map<int,TTipoUnidade*>::iterator it;
	bool sucesso = true;
	TTipoUnidade *tipo_unidade;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Tipos de Chaves");
	}

	if(commons == NULL)
	{
		throw EImportaUgrid("Erro em ReadCfg_TipoChave.");
		return(false);
	}
	mapTipoCh = commons->GetmapTipoUnidade();
	try
		{
			for (it = mapTipoCh.begin(); it != mapTipoCh.end(); it++)
			{
				tipo_unidade = it->second;
				pre_monta->InsereCfg_TipoChave(tipo_unidade);
			}
		}
	catch (...)
	{
			throw EImportaUgrid("Erro inesperado em ReadCfg_TipoChave.");
			sucesso = false;
	}
	return (sucesso);
}


// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadEtd_TabBarramento(int etd_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	std::map<int,TBarramento*>mapBarramento;
	std::map<int,TBarramento*>::iterator it;
	bool sucesso = true;
	TBarramento *barramento;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de barramento SED");
	}

	if(commons == NULL)
	{
		throw EImportaUgrid("Erro em ReadEtd_TabBarramento.");
		return(false);
	}
    mapBarramento = commons->GetmapBarramento();
	try
		{
			for (it = mapBarramento.begin(); it != mapBarramento.end(); it++)
			{
				barramento = it->second;
				// Verifica se o barramento faz pate da sub que está sendo lida
				if(barramento->Id_Sub == etd_id)
				{
					pre_monta->InsereEtd_Barra(barramento);
				}
		}
	}
	catch (...)
	{
			throw EImportaUgrid("Erro inesperado em ReadEtd_TabBarramento.");
			sucesso = false;
	}
	return (sucesso);
}


// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadEtd_TabSe(int etd_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	TSubestacao *subestacao;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de rede SED", true);
	}
	if(commons == NULL)
	{
		throw EImportaUgrid("Erro em ReadEtd_TabSe.");
		return(false);
	}
	subestacao = commons->GetSubestacao(etd_id);
	if (subestacao != NULL)
	{
		try
		{
			pre_monta->InsereEtd_Rede(subestacao);
		}
		catch (...)
		{
			throw EImportaUgrid("Erro em ReadEtd_TabSe.");
			sucesso = false;
		}

	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadEtd_TabTrafoSe(int etd_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	std::map<int,TTrafoSubestacao*>mapTrafoSub;
	std::map<int,TTrafoSubestacao*>::iterator it;
	bool sucesso = true;
	TTrafoSubestacao *trafo_sub;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de transformador SED");
	}
	if(commons == NULL)
	{
		throw EImportaUgrid("Erro em ReadEtd_TabTrafoSe.");
		return(false);
	}
	mapTrafoSub = commons->GetmapTrafoSub();
	try
	{
			for (it = mapTrafoSub.begin(); it != mapTrafoSub.end(); it++)
			{
				trafo_sub = it->second;
				// Verifica se o trafo faz pate da sub que está sendo lida
				if(trafo_sub->Id_Sub == etd_id)
				{
					pre_monta->InsereEtd_Trafo(trafo_sub);
				}
			}
	}
	catch (...)
	{ // erro inesperado
		throw EImportaUgrid("Erro inesperado em ReadEtd_TabTrafoSe.");
		sucesso = false;
	}
	return (sucesso);
}

//// -----------------------------------------------------------------------------
//bool __fastcall TImportaArquivoUgrid::ReadPri_TabBarra(int pri_id)
//{
//	// variáveis locais
//	bool sucesso = true;
//	AnsiString cmd_sql, strlog;
//	sdi_BARRA sdi;
//	VTSQLiteTable *table;
//
//	// atualiza progresso
//	if (!BatchMode)
//	{
//		if (progresso == NULL)
//			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
//		if (progresso)
//			progresso->Add("Leitura de barra SDMT");
//	}
//	try
//	{
//		table = dao->ExecuteSqlPriBarra(pri_id);
//	}
//	catch (EImportaUgridDAO &e)
//	{
//		table = NULL;
//		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabBarra.");
//		sucesso = false;
//	}
//	if (sucesso)
//	{
//		try
//		{
//			for (int row = 0; row < table->NumRows(); row++)
//			{
//				table->SetRow(row);
//				sdi.id = table->GetIntField("ID_BARRA");
//				sdi.codigo = table->GetStringField("CODIGO");
//				sdi.x_utm_m = table->GetFloatField("UTMX_M");
//				sdi.y_utm_m = table->GetFloatField("UTMY_M");
//				// insere Barra em PreMonta
//				pre_monta->InserePri_Barra(sdi);
//			}
//		}
//		catch (ESQLite &e)
//		{ // provavelmente uma coluna que nao existe
//			// log_geral->Add("ReadEtd_TabSe");
//			// log_geral->Add(e.Message);
//			throw EImportaUgrid("Coluna inexistente em ReadPri_TabBarra.");
//			sucesso = false;
//		}
//		catch (...)
//		{ // erro inesperado
//			// log_geral->Add("ReadEtd_TabSe");
//			// log_geral->Add(e.Message);
//			throw EImportaUgrid("Erro inesperado em ReadPri_TabBarra.");
//			sucesso = false;
//		}
//	}
//	if (table != NULL)
//	{
//		delete table;
//	}
//	return (sucesso);
//}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabCapacitor(int pri_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCapacitorBDGD *cap;
	VTSQLiteTable *table;
	//
	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de barra SDMT");
	}
	try
	{
		table = dao->ExecuteSqlPriCapacitor(pri_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabCapacitor.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			//cria somente 1 vez o TO e o reaproveita
			cap = NewObjCapacitorBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);

				cap->Id = table->GetIntField("ID_CAPACITOR");
				cap->Id_Rede = table->GetIntField("ID_REDE_MT");
				cap->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				cap->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				cap->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				cap->Tipo_Unidade = commons->GetTipoUnidade(table->GetIntField("ID_TIPO_UNIDADE"));
				cap->Tipo_Q = commons->GetTipoPotReativa(table->GetIntField("ID_TIPO_POTENCIA_REATIVA"));
				cap->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				cap->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				cap->Banco = (table->GetIntField("BANCO") == 1) ? true : false;
				cap->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Capacitor em PreMonta
				pre_monta->InserePri_Capacitor(cap);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (cap)
			{
				delete cap;
			}
			throw EImportaUgrid("Coluna inexistente em ReadPri_TabCapacitor.");
			sucesso = false; //unreachable code, o throw é tipo um return
		}
		catch (...)
		{ 
			if (table != NULL)
				{delete table;}
			//destroi TO
			if (cap)
			{
				delete cap;
			}
			throw EImportaUgrid("Erro inesperado em ReadPri_TabCapacitor.");
			sucesso = false; //unreachable code, o throw é tipo um return
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (cap)
	{
		delete cap;
	}

	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabCargaMT(int pri_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCargaBDGD *cargaMT;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de consumidor SDMT");
	}

	try
	{
		table = dao->ExecuteSqlPriCargaMT(pri_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabCargaMT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
            //cria somente 1 vez o TO e o reaproveita
			cargaMT = NewObjCargaBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				cargaMT->Id = table->GetIntField("ID_CARGA_MT");
				cargaMT->Id_Rede = table->GetIntField("ID_REDE_MT");
				cargaMT->Barra = commons->GetBarra(table->GetIntField("ID_BARRA"));
				cargaMT->Classe_Subclasse = commons->GetClasseSubclasse(table->GetIntField("ID_CLASSE_SUBCLASSE"));
				cargaMT->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				cargaMT->Tensao = commons->GetTensao(table->GetIntField("ID_TENSAO_FORNECIMENTO"));
				cargaMT->Grupo_Tarifario = commons->GetGrupoTarifario(table->GetIntField("ID_GRUPO_TARIFARIO"));
				cargaMT->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				cargaMT->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				cargaMT->Carga_Instal_kW = table->GetFloatField("CARGA_INSTALADA");
				cargaMT->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// Demanda Ativa max kW
				// Para armazenar começa em zero mas para a leitura, o usuario pode passar o periodo normal que vai ser pego
				// o elemento n-1
				cargaMT->Dem_Max_kW[0] = table->GetFloatField("DEM_01");
				cargaMT->Dem_Max_kW[1] = table->GetFloatField("DEM_02");
				cargaMT->Dem_Max_kW[2] = table->GetFloatField("DEM_03");
				cargaMT->Dem_Max_kW[3] = table->GetFloatField("DEM_04");
				cargaMT->Dem_Max_kW[4] = table->GetFloatField("DEM_05");
				cargaMT->Dem_Max_kW[5] = table->GetFloatField("DEM_06");
				cargaMT->Dem_Max_kW[6] = table->GetFloatField("DEM_07");
				cargaMT->Dem_Max_kW[7] = table->GetFloatField("DEM_08");
				cargaMT->Dem_Max_kW[8] = table->GetFloatField("DEM_09");
				cargaMT->Dem_Max_kW[9] = table->GetFloatField("DEM_10");
				cargaMT->Dem_Max_kW[10] = table->GetFloatField("DEM_11");
				cargaMT->Dem_Max_kW[11] = table->GetFloatField("DEM_12");
				// Energia Ativa kWh
				// Para armazenar começa em zero mas para a leitura, o usuario pode passar o periodo normal que vai ser pego
				// o elemento n-1
				cargaMT->Ene_kWh[0] = table->GetFloatField("ENE_01");
				cargaMT->Ene_kWh[1] = table->GetFloatField("ENE_02");
				cargaMT->Ene_kWh[2] = table->GetFloatField("ENE_03");
				cargaMT->Ene_kWh[3] = table->GetFloatField("ENE_04");
				cargaMT->Ene_kWh[4] = table->GetFloatField("ENE_05");
				cargaMT->Ene_kWh[5] = table->GetFloatField("ENE_06");
				cargaMT->Ene_kWh[6] = table->GetFloatField("ENE_07");
				cargaMT->Ene_kWh[7] = table->GetFloatField("ENE_08");
				cargaMT->Ene_kWh[8] = table->GetFloatField("ENE_09");
				cargaMT->Ene_kWh[9] = table->GetFloatField("ENE_10");
				cargaMT->Ene_kWh[10] = table->GetFloatField("ENE_11");
				cargaMT->Ene_kWh[11] = table->GetFloatField("ENE_12");
				// insere Consumidor MT em PreMonta
				pre_monta->InserePri_Consumidor(cargaMT);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (cargaMT)
				{delete cargaMT;}
			throw EImportaUgrid("Coluna inexistente em ReadPri_TabCargaMT.");
			sucesso = false; //unreachable code, o throw é tipo um return
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (cargaMT)
				{delete cargaMT;}
			throw EImportaUgrid("Erro inesperado em ReadPri_TabCargaMT.");
			sucesso = false; //unreachable code, o throw é tipo um return
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	if (cargaMT)
		{delete cargaMT;}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabChave(int pri_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TChaveBDGD *chave;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de chave SDMT");
	}
	try
	{
		table = dao->ExecuteSqlPriChave(pri_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabChave.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{   //cria somente 1 vez o TO e o reaproveita
			chave = NewObjChaveBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				chave->Id = table->GetIntField("ID_CHAVE");
				chave->Id_Rede = table->GetIntField("ID_REDE_MT");
				chave->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				chave->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				chave->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				chave->Tipo_Unidade = commons->GetTipoUnidade(table->GetIntField("ID_TIPO_UNIDADE"));
				chave->Elo_Fusivel = commons->GetCapElo(table->GetIntField("ID_CAPACIDADE_ELO_FUSIVEL"));
				chave->Corrente_Nom = commons->GetCorrenteNom(table->GetIntField("ID_CORRENTE_NOMINAL"));
				chave->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				chave->Telecomandada = (table->GetIntField("TELECOMANDADA") == 1) ? true : false;
				chave->Estado_Normal = table->GetIntField("ESTADO_NORMAL");
				chave->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Chave em PreMonta
				pre_monta->InserePri_Chave(chave);
			}
		}
		catch (ESQLite &e)
		{
			if (table != NULL)
				{delete table;}
			//destroi TO
			if (chave)
				{delete chave;}
			throw EImportaUgrid("Coluna inexistente em ReadPri_TabChave.");
			sucesso = false;
		}
		catch (...)
		{ 
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (chave)
				{delete chave;}
			throw EImportaUgrid("Erro inesperado em ReadPri_TabChave.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (chave)
		{delete chave;}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabGerador(int pri_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TGeradorBDGD *geradorMT;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Gerador SDMT");
	}

	try
	{
		table = dao->ExecuteSqlPriGerador(pri_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabGerador.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{   //cria somente 1 vez o TO e o reaproveita
			geradorMT = NewObjGeradorBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				geradorMT->Id = table->GetIntField("ID_GERADOR_MT");
				geradorMT->Id_Rede = table->GetIntField("ID_REDE_MT");
				geradorMT->Barra = commons->GetBarra(table->GetIntField("ID_BARRA"));
				geradorMT->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				geradorMT->Tensao = commons->GetTensao(table->GetIntField("ID_TENSAO_FORNECIMENTO"));
				geradorMT->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				geradorMT->P_Inst_kW = table->GetFloatField("POTENCIA_INSTALADA");
				geradorMT->P_Contra_Max_kW = table->GetFloatField("POTENCIA_CONTRATADA_MAX");
				geradorMT->R0 = table->GetFloatField("R0");
				geradorMT->X0 = table->GetFloatField("X0");
				geradorMT->R1 = table->GetFloatField("R1");
				geradorMT->X1 = table->GetFloatField("X1");
				geradorMT->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Gerador em PreMonta
				pre_monta->InserePri_Gerador(geradorMT);
			}
		}
		catch (ESQLite &e)
		{ 
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (geradorMT)
				{delete geradorMT;}
			throw EImportaUgrid("Coluna inexistente em ReadPri_TabGerador.");
			sucesso = false;
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (geradorMT)
				{delete geradorMT;}
			throw EImportaUgrid("Erro inesperado em ReadPri_TabGerador.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
    //destroi TO
	if (geradorMT)
		{delete geradorMT;}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabRede(TRedeMT *primario)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString prog;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
		{
			prog = "SED: " + nomeSe + " SDMT: " + nomeCircuito;
			progresso->Add(prog, true);
			progresso->Add("Leitura de rede SDMT");
		}
	}
   sucesso = pre_monta->InserePri_Rede(primario);
   return (sucesso);
}




// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabRegulador(int pri_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TReguladorMT *reg;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura dos reguladores SDMT");
	}

	try
	{
		table = dao->ExecuteSqlPriRegulador(pri_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabRegulador.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			reg = NewObjReguladorMT();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);

				reg->Id = table->GetIntField("ID_REGULADOR_TENSAO_MT");
				reg->Id_Rede = table->GetIntField("ID_REDE_MT");
				reg->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				reg->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				reg->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				reg->Tipo_Unidade = commons->GetTipoUnidade(table->GetIntField("ID_TIPO_UNIDADE"));
				reg->Tipo_Regulador = commons->GetTipoRegulador(table->GetIntField("ID_TIPO_REGULADOR"));
				reg->Tipo_S_Nom = commons->GetTipoPotAparente(table->GetIntField("ID_TIPO_POTENCIA_NOMINAL"));
				reg->Corrente_Nom = commons->GetCorrenteNom(table->GetIntField("ID_CORRENTE_NOMINAL"));
				reg->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				reg->Banco = (table->GetIntField("BANCO") == 1) ? true : false;
				reg->Qtd_Unid_Mono = table->GetIntField("QT_UNIDADES_MONOF");
				reg->Pot_Nom = table->GetFloatField("POTENCIA_NOMINAL");
				reg->Tensao_Regulacao = table->GetFloatField("TENSAO_REGULACAO");
				reg->R = table->GetFloatField("RESISTENCIA");
				reg->X_Pri_Sec = table->GetFloatField("REATANCIA_PRI_SEC");
				reg->Perda_Fe = table->GetFloatField("PERDA_FERRO");
				reg->Perda_Total = table->GetFloatField("PERDA_TOTAL");
				reg->RTP = table->GetIntField("RTP");
				reg->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Regulador em PreMonta
				pre_monta->InserePri_Regulador(reg);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (reg)
				{delete reg;}
			throw EImportaUgrid("Coluna inexistente em ReadPri_TabRegulador.");
			sucesso = false;
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (reg)
				{delete reg;}
			throw EImportaUgrid("Erro inesperado em ReadPri_TabRegulador.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (reg)
		{delete reg;}
	return (sucesso);
}


// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadPri_TabTrecho(int pri_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTrechoBDGD *trecho;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de trecho SDMT");
	}
	try
	{
		table = dao->ExecuteSqlPriTrecho(pri_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadPri_TabTrecho.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			trecho = NewObjTrechoBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				trecho->Id = table->GetIntField("ID_TRECHO_MT");
				trecho->Id_Rede = table->GetIntField("ID_REDE_MT");
				trecho->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				trecho->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				trecho->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				trecho->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				trecho->Arranjo = commons->GetArranjo(table->GetIntField("ID_ARRANJO"));
				trecho->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				trecho->Comprimento_m = table->GetFloatField("COMPRIMENTO");
				trecho->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Trecho em PreMonta
				sucesso = pre_monta->InserePri_Trecho(trecho);
            }
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (trecho)
				{delete trecho;}
			throw EImportaUgrid("Coluna inexistente em ReadPri_TabTrecho.");
			sucesso = false;
		}
		catch (...)
		{ // erro inesperado
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (trecho)
				{delete trecho;}
			throw EImportaUgrid("Erro inesperado em ReadPri_TabTrecho.");
			sucesso = false;
		}
	}
    if (table != NULL)
		{delete table;}
	//destroi TO
	if (trecho)
		{delete trecho;}
	return (sucesso);
}

//// -----------------------------------------------------------------------------
//bool __fastcall TImportaArquivoUgrid::ReadSec_TabBarraBT(int sec_id)
//{
//	// variáveis locais
//	bool sucesso = true;
//	AnsiString cmd_sql, strlog;
//	sdi_BARRA_BT sdi;
//	int count_barra = 0;
//	VTSQLiteTable *table;
//	int numero = 0;
//
//	// atualiza progresso
//	if (!BatchMode)
//	{
//		if (progresso == NULL)
//			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
//		if (progresso)
//			progresso->Add("Leitura de barra SDBT");
//	}
//
//	try
//	{
//		table = dao->ExecuteSqlSecBarra(sec_id);
//	}
//	catch (EImportaUgridDAO &e)
//	{
//		table = NULL;
//		throw EImportaUgrid("Erro ao executar a query em ReadSec_TabTrafoMTBT.");
//		sucesso = false;
//	}
//	if (sucesso)
//	{
//		try
//		{
//			numero = table->NumRows();
//			for (int row = 0; row < table->NumRows(); row++)
//			{
//				table->SetRow(row);
//				sdi.id = table->GetIntField("ID_BARRA");
//				sdi.codigo = table->GetStringField("CODIGO");
//				sdi.x_utm_m = table->GetFloatField("UTMX_M");
//				sdi.y_utm_m = table->GetFloatField("UTMY_M");
//				sdi.redebt_id = table->GetIntField("ID_REDE_BT");
//				// insere BarraBT em PreMonta
//				pre_monta->InsereSec_Barra(sdi);
//				count_barra++;
//			}
//
//			// insere BarraBT em PreMonta (somente para inserir os trafos)
//			if (count_barra == 0)
//			{
//				sdi.id = -1;
//				pre_monta->InsereSec_Barra(sdi);
//			}
//		}
//		catch (ESQLite &e)
//		{ // provavelmente uma coluna que nao existe
//			// log_geral->Add("ReadEtd_TabSe");
//			// log_geral->Add(e.Message);
//			throw EImportaUgrid("Coluna inexistente em ReadSec_TabBarraBT.");
//			sucesso = false;
//		}
//		catch (...)
//		{ // erro inesperado
//			// log_geral->Add("ReadEtd_TabSe");
//			// log_geral->Add(e.Message);
//			throw EImportaUgrid("Erro inesperado em ReadSec_TabBarraBT.");
//			sucesso = false;
//		}
//	}
//	if (table != NULL)
//	{
//		delete table;
//	}
//	return (sucesso);
//}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabCapacitor(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCapacitorBDGD *cap;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura dos capacitores SDBT");
	}
	try
	{
		table = dao->ExecuteSqlSecCapacitor(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadSec_TabCapacitor.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{   //cria somente 1 vez o TO e o reaproveita
			cap = NewObjCapacitorBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				cap->Id = table->GetIntField("ID_CAPACITOR_BT");
				cap->Id_Rede = table->GetIntField("ID_REDE_BT");
				cap->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				cap->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				cap->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				cap->Tipo_Unidade = commons->GetTipoUnidade(table->GetIntField("ID_TIPO_UNIDADE"));
				cap->Tipo_Q = commons->GetTipoPotReativa(table->GetIntField("ID_TIPO_POTENCIA_REATIVA"));
				cap->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				cap->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				cap->Banco = (table->GetIntField("BANCO") == 1) ? true : false;
				cap->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Capacitor em PreMonta
				pre_monta->InsereSec_Capacitor(cap);
			}
		}
		catch (ESQLite &e)
		{
			if (table != NULL)
			{
				delete table;
			}
			//destroi TO
			if (cap)
				{delete cap;}
			throw EImportaUgrid("Coluna inexistente em ReadSec_TabCapacitor.");
			sucesso = false; 	//unreachable code, o throw é tipo um return
		}
		catch (...)
		{
            if (table != NULL)
			{
				delete table;
			}
			//destroi TO
			if (cap)
				{delete cap;}
			throw EImportaUgrid("Erro inesperado em ReadSec_TabCapacitor.");
			sucesso = false;  	//unreachable code, o throw é tipo um return
		}
	}
	if (table != NULL)
	{
		delete table;
	}
    //destroi TO
	if (cap)
	{
		delete cap;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabChave(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TChaveBDGD *chave;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de chave SDBT");
	}
	try
	{
		table = dao->ExecuteSqlSecChave(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadSec_TabChave.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{   //cria somente 1 vez o TO e o reaproveita
			chave = NewObjChaveBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				chave->Id = table->GetIntField("ID_CHAVE_BT");
				chave->Id_Rede = table->GetIntField("ID_REDE_BT");
				chave->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				chave->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				chave->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				chave->Tipo_Unidade = commons->GetTipoUnidade(table->GetIntField("ID_TIPO_UNIDADE"));
				chave->Elo_Fusivel = commons->GetCapElo(table->GetIntField("ID_CAPACIDADE_ELO_FUSIVEL"));
				chave->Corrente_Nom = commons->GetCorrenteNom(table->GetIntField("ID_CORRENTE_NOMINAL"));
				chave->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				chave->Telecomandada = (table->GetIntField("TELECOMANDADA") == 1) ? true : false;
				chave->Estado_Normal = table->GetIntField("ESTADO_NORMAL");
				chave->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Chave em PreMonta
				pre_monta->InsereSec_Chave(chave);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (chave)
				{delete chave;}
			throw EImportaUgrid("Coluna inexistente em ReadSec_TabChave.");
			sucesso = false;
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (chave)
				{delete chave;}
			throw EImportaUgrid("Erro inesperado em ReadSec_TabChave.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (chave)
		{delete chave;}
	return (sucesso);
}


// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabConsBT(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCargaBDGD *cargaBT;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de consumidor SDBT");
	}

	try
	{
		table = dao->ExecuteSqlSecConsBT(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadSec_TabConsBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			cargaBT = NewObjCargaBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				cargaBT->Id = table->GetIntField("ID_CARGA_BT");
				cargaBT->Id_Rede = table->GetIntField("ID_REDE_BT");
				cargaBT->Barra = commons->GetBarra(table->GetIntField("ID_BARRA"));
				cargaBT->Classe_Subclasse = commons->GetClasseSubclasse(table->GetIntField("ID_CLASSE_SUBCLASSE"));
				cargaBT->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				cargaBT->Tensao = commons->GetTensao(table->GetIntField("ID_TENSAO_FORNECIMENTO"));
				cargaBT->Grupo_Tarifario = commons->GetGrupoTarifario(table->GetIntField("ID_GRUPO_TARIFARIO"));
				cargaBT->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				cargaBT->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				cargaBT->Carga_Instal_kW = table->GetFloatField("CARGA_INSTALADA");
				cargaBT->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// Energia Ativa kWh
				// Para armazenar começa em zero mas para a leitura, o usuario pode passar o periodo normal que vai ser pego
				// o elemento n-1
				cargaBT->Ene_kWh[0] = table->GetFloatField("ENE_01");
				cargaBT->Ene_kWh[1] = table->GetFloatField("ENE_02");
				cargaBT->Ene_kWh[2] = table->GetFloatField("ENE_03");
				cargaBT->Ene_kWh[3] = table->GetFloatField("ENE_04");
				cargaBT->Ene_kWh[4] = table->GetFloatField("ENE_05");
				cargaBT->Ene_kWh[5] = table->GetFloatField("ENE_06");
				cargaBT->Ene_kWh[6] = table->GetFloatField("ENE_07");
				cargaBT->Ene_kWh[7] = table->GetFloatField("ENE_08");
				cargaBT->Ene_kWh[8] = table->GetFloatField("ENE_09");
				cargaBT->Ene_kWh[9] = table->GetFloatField("ENE_10");
				cargaBT->Ene_kWh[10] = table->GetFloatField("ENE_11");
				cargaBT->Ene_kWh[11] = table->GetFloatField("ENE_12");
				// insere ConsBT em PreMonta
				pre_monta->InsereSec_Consumidor(cargaBT);
			}
		}
		catch (ESQLite &e)
		{
			if (table != NULL)
				{delete table;}
			//destroi TO
			if (cargaBT)
				{delete cargaBT;}
			throw EImportaUgrid(" Coluna inexistente em ReadSec_TabConsBT.");
			sucesso = false;
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (cargaBT)
				{delete cargaBT;}
			throw EImportaUgrid(" Erro inesperado em ReadSec_TabConsBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	if (cargaBT)
		{delete cargaBT;}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabConsIP(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCargaIP *cargaIP;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de iluminação pública SDBT ");
	}

	try
	{
		table = dao->ExecuteSqlSecConsIP(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid(" Erro ao executar a query em ReadSec_TabConsIP.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{   //cria somente 1 vez o TO e o reaproveita
			cargaIP = NewObjCargaIP();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				cargaIP->Id = table->GetIntField("ID_CARGA_IP");
				cargaIP->Id_Rede_MT = table->GetIntField("ID_REDE_MT");
				cargaIP->Id_Rede_BT = table->GetIntField("ID_REDE_BT");
				cargaIP->Barra = commons->GetBarra(table->GetIntField("ID_BARRA"));
				cargaIP->Classe_Subclasse = commons->GetClasseSubclasse(table->GetIntField("ID_CLASSE_SUBCLASSE"));
				cargaIP->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				cargaIP->Tensao = commons->GetTensao(table->GetIntField("ID_TENSAO_FORNECIMENTO"));
				cargaIP->Grupo_Tarifario = commons->GetGrupoTarifario(table->GetIntField("ID_GRUPO_TARIFARIO"));
				cargaIP->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				cargaIP->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				cargaIP->Carga_Instal_kW = table->GetFloatField("CARGA_INSTALADA");
				cargaIP->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// Energia Ativa kWh
				// Para armazenar começa em zero mas para a leitura, o usuario pode passar o periodo normal que vai ser pego
				// o elemento n-1
				cargaIP->Ene_kWh[0] = table->GetFloatField("ENE_01");
				cargaIP->Ene_kWh[1] = table->GetFloatField("ENE_02");
				cargaIP->Ene_kWh[2] = table->GetFloatField("ENE_03");
				cargaIP->Ene_kWh[3] = table->GetFloatField("ENE_04");
				cargaIP->Ene_kWh[4] = table->GetFloatField("ENE_05");
				cargaIP->Ene_kWh[5] = table->GetFloatField("ENE_06");
				cargaIP->Ene_kWh[6] = table->GetFloatField("ENE_07");
				cargaIP->Ene_kWh[7] = table->GetFloatField("ENE_08");
				cargaIP->Ene_kWh[8] = table->GetFloatField("ENE_09");
				cargaIP->Ene_kWh[9] = table->GetFloatField("ENE_10");
				cargaIP->Ene_kWh[10] = table->GetFloatField("ENE_11");
				cargaIP->Ene_kWh[11] = table->GetFloatField("ENE_12");
				// insere ConsIP em PreMonta
				pre_monta->InsereSec_ConsIP(cargaIP);
			}

		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (cargaIP)
				{delete cargaIP;}
			throw EImportaUgrid(" Coluna inexistente em ReadSec_TabConsIP.");
			sucesso = false;
		}
		catch (...)
		{ // erro inesperado
			// log_geral->Add(" ReadEtd_TabSe ");
			// log_geral->Add(e.Message);
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (cargaIP)
				{delete cargaIP;}
			throw EImportaUgrid(" Erro inesperado em ReadSec_TabConsIP.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (cargaIP)
		{delete cargaIP;}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabGeradorBT(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TGeradorBDGD *geradorBT;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de gerador SDBT");
	}
	try
	{
		table = dao->ExecuteSqlSecGeradorBT(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid("Erro ao executar a query em ReadSec_TabGeradorBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{   //cria somente 1 vez o TO e o reaproveita
			geradorBT = NewObjGeradorBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				geradorBT->Id = table->GetIntField("ID_GERADOR_BT");
				geradorBT->Id_Rede = table->GetIntField("ID_REDE_BT");
				geradorBT->Barra = commons->GetBarra(table->GetIntField("ID_BARRA"));
				geradorBT->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				geradorBT->Tensao = commons->GetTensao(table->GetIntField("ID_TENSAO_FORNECIMENTO"));
				geradorBT->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				geradorBT->P_Inst_kW = table->GetFloatField("POTENCIA_INSTALADA");
				geradorBT->P_Contra_Max_kW = table->GetFloatField("POTENCIA_CONTRATADA_MAX");
				geradorBT->R0 = table->GetFloatField("R0");
				geradorBT->X0 = table->GetFloatField("X0");
				geradorBT->R1 = table->GetFloatField("R1");
				geradorBT->X1 = table->GetFloatField("X1");
				geradorBT->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Gerador em PreMonta
				pre_monta->InsereSec_Gerador(geradorBT);
			}

		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			if (table != NULL)
				{delete table;}
			//destroi TO
			if (geradorBT)
				{delete geradorBT;}
			throw EImportaUgrid("Coluna inexistente em ReadSec_TabGeradorBT.");
			sucesso = false;
		}
		catch (...)
		{ // erro inesperado
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (geradorBT)
				{delete geradorBT;}
			throw EImportaUgrid("Erro inesperado em ReadSec_TabGeradorBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (geradorBT)
		{delete geradorBT;}
	return (sucesso);
}


// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabRamalLig(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TRamalLigacao *ramal;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura dos ramais de ligação da SDBT ");
	}
	try
	{
		table = dao->ExecuteSqlSecRamal(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid(" Erro ao executar a query em ReadSec_TabRamalLig.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			ramal = NewObjRamalLigacao();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				ramal->Id = table->GetIntField("ID_RAMAL_LIGACAO");
				ramal->Id_Rede = table->GetIntField("ID_REDE_BT");
				ramal->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				ramal->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				ramal->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				ramal->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				ramal->Arranjo = commons->GetArranjo(table->GetIntField("ID_ARRANJO"));
				ramal->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				ramal->Comprimento_m = table->GetFloatField("COMPRIMENTO");
				ramal->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Trecho em PreMonta
				sucesso = pre_monta->InsereSec_Ramal(ramal);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (ramal)
				{delete ramal;}
			throw EImportaUgrid(" Coluna inexistente em ReadSec_TabRamalLig.");
			sucesso = false;
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (ramal)
				{delete ramal;}
			throw EImportaUgrid(" Erro inesperado em ReadSec_TabRamalLig.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
    //destroi TO
	if (ramal)
		{delete ramal;}
	return (sucesso);
}



// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabRede(TRedeMT *primario)
{
    // variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso, sucesso_total;
	sucesso = true;
	TRedeBT *redeBT;
	VTSQLiteTable *table;
	int rede_mt_id, rede_bt_id;
	AnsiString codigo_bt;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura das redes SDBT do primario '" + primario->Codigo + "' ");
	}
	try
	{
		table = dao->ExecuteSqlSecRede(primario->Id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid(" Erro ao executar a query em ReadSec_TabRede.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				redeBT = NewObjRedeBT();
				redeBT->Id_Rede_MT = primario->Id;
				redeBT->Rede_MT = primario;
				redeBT->Id_Rede_BT = table->GetIntField("ID_REDE_BT");
				if (progresso)
				  progresso->Add("Leitura da rede SDBT '" + IntToStr(redeBT->Id_Rede_BT) + "' ");
				redeBT->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				redeBT->Tensao_SecMTBT = commons->GetTensao(table->GetIntField("TEN_SEC_MTBT"));
				redeBT->Tensao_TerMTBT = commons->GetTensao(table->GetIntField("TEN_TER_MTBT"));
				if (primario != NULL)
				{
						primario->InsereSecundario(redeBT); //responsavel pela destruicao do redeBT
                        // insere rede em PreMonta
						sucesso = pre_monta->InsereSec_Rede(redeBT);
				}
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
			{
				delete table;
			}
			throw EImportaUgrid(" Coluna inexistente em ReadSec_TabRede.");
			sucesso = false;
		}
		catch (Exception &e)
		{
            if (table != NULL)
			{
				delete table;
			}
			throw EImportaUgrid(" Erro inesperado em ReadSec_TabRede.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabTrafoMTBT(TRedeMT *primario)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTrafoDistribuicao *trafo_MTBT;
	AnsiString prog;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura dos trafos MTBT do primario' " + primario->Codigo + "' ");
	}
	try
	{
		table = dao->ExecuteSqlSecTrafo(primario->Id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid(" Erro ao executar a query em ReadSec_TabTrafoMTBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			trafo_MTBT = NewObjTrafoDistribuicao();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);

				trafo_MTBT->Id = table->GetIntField("ID_TRAFO_DISTRIBUICAO");
				trafo_MTBT->Id_Rede_MT = table->GetIntField("ID_REDE_MT");
				trafo_MTBT->Id_Rede_BT = table->GetIntField("ID_REDE_BT");
				trafo_MTBT->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				trafo_MTBT->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				trafo_MTBT->Barra_3 = commons->GetBarra(table->GetIntField("ID_BARRA_3"));
				trafo_MTBT->Fases_Con_Pri = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO_PRI"));
				trafo_MTBT->Fases_Con_Sec = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO_SEC"));
				trafo_MTBT->Fases_Con_Ter = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO_TER"));
				trafo_MTBT->Tensao_Pri = commons->GetTensao(table->GetIntField("ID_TENSAO_PRI"));
				trafo_MTBT->Tensao_Sec = commons->GetTensao(table->GetIntField("ID_TENSAO_SEC"));
				trafo_MTBT->Tensao_Ter = commons->GetTensao(table->GetIntField("ID_TENSAO_TER"));
				trafo_MTBT->Tipo_Unidade = commons->GetTipoUnidade(table->GetIntField("ID_TIPO_UNIDADE"));
				trafo_MTBT->Posse = commons->GetPosse(table->GetIntField("ID_POSSE"));
				trafo_MTBT->Elo_Fusivel = commons->GetCapElo(table->GetIntField("ID_CAPACIDADE_ELO_FUSIVEL"));
				trafo_MTBT->Corrente_Nom = commons->GetCorrenteNom(table->GetIntField("ID_CAPACIDADE_CHAVE"));
				trafo_MTBT->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				trafo_MTBT->Config_Circuito = commons->GetConfCircuito(table->GetIntField("ID_CONFIGURACAO_CIRCUITO"));
				trafo_MTBT->Tipo_Trafo = commons->GetTipoTrafo(table->GetIntField("ID_TIPO_TRAFO"));
				trafo_MTBT->Ligacao_Trafo = commons->GetLigacaoTrafo(table->GetIntField("ID_LIGACAO_TRAFO"));
				trafo_MTBT->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				trafo_MTBT->Atribuicao_Perda = table->GetIntField("ATRIBUICAO_PERDA");
				trafo_MTBT->Ten_Lin_Sec_kV = table->GetFloatField("TENSAO_LINHA_SECUNDARIO");
				trafo_MTBT->Tap = table->GetFloatField("TAP");
				trafo_MTBT->S_Nom_kVA = table->GetFloatField("POTENCIA_NOMINAL_APARENTE");
				trafo_MTBT->Perda_Fe_W = table->GetFloatField("PERDA_FERRO");
				trafo_MTBT->Perda_Total_W = table->GetFloatField("PERDA_TOTAL");
				trafo_MTBT->Banco = (table->GetIntField("BANCO") == 1) ? true : false;
				trafo_MTBT->Mono_Retorno_Terra = (table->GetIntField("MONOFASICA_RETORNO_TERRA") == 1) ? true : false;
				trafo_MTBT->R = table->GetFloatField("RESISTENCIA");
				trafo_MTBT->X_Pri_Sec = table->GetFloatField("REATANCIA_PRI_SEC");
				trafo_MTBT->X_Pri_Ter = table->GetFloatField("REATANCIA_PRI_TER");
				trafo_MTBT->X_Sec_Ter = table->GetFloatField("REATANCIA_SEC_TER");
				trafo_MTBT->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Trafo_MTBT em PreMonta
				sucesso = pre_monta->InsereSec_TrafoMTBT(trafo_MTBT);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (trafo_MTBT)
				{delete trafo_MTBT;}
			throw EImportaUgrid(" Coluna inexistente em ReadSec_TabTrafoMTBT.");
			sucesso = false;
		}
		catch (Exception &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (trafo_MTBT)
				{delete trafo_MTBT;}
			throw EImportaUgrid(" Erro inesperado em ReadSec_TabTrafoMTBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	//destroi TO
	if (trafo_MTBT)
		{delete trafo_MTBT;}
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::ReadSec_TabTrechoBT(int sec_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTrechoBDGD *trecho;
	VTSQLiteTable *table;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de trecho SDBT ");
	}
	try
	{
		table = dao->ExecuteSqlSecTrecho(sec_id);
	}
	catch (EImportaUgridDAO &e)
	{
		table = NULL;
		throw EImportaUgrid(" Erro ao executar a query em ReadSec_TabTrechoBT.");
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			trecho = NewObjTrechoBDGD();
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);

				trecho->Id = table->GetIntField("ID_TRECHO_BT");
				trecho->Id_Rede = table->GetIntField("ID_REDE_BT");
				trecho->Barra_1 = commons->GetBarra(table->GetIntField("ID_BARRA_1"));
				trecho->Barra_2 = commons->GetBarra(table->GetIntField("ID_BARRA_2"));
				trecho->Area_Loc = commons->GetAreaLoc(table->GetIntField("ID_AREA_LOCALIZACAO"));
				trecho->Fases_Con = commons->GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO"));
				trecho->Arranjo = commons->GetArranjo(table->GetIntField("ID_ARRANJO"));
				trecho->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
				trecho->Comprimento_m = table->GetFloatField("COMPRIMENTO");
				trecho->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
				// insere Trecho em PreMonta
				sucesso = pre_monta->InsereSec_Trecho(trecho);
			}
		}
		catch (ESQLite &e)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (trecho)
				{delete trecho;}
			throw EImportaUgrid(" Coluna inexistente em ReadSec_TabTrechoBT.");
			sucesso = false;
		}
		catch (...)
		{
            if (table != NULL)
				{delete table;}
			//destroi TO
			if (trecho)
				{delete trecho;}
			throw EImportaUgrid(" Erro inesperado em ReadSec_TabTrechoBT.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
    //destroi TO
	if (trecho)
		{delete trecho;}
	return (sucesso);
}

// -----------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoUgrid::Unzip(TList *lisTMP)
{
	VTBatchImportaRedesTask *batch = (VTBatchImportaRedesTask*)apl->GetObject(__classid(VTBatchImportaRedesTask));

	// Inicia progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add(" Carregando subestações : ", true);
	}
	//verifica se o zip ja foi destruido
	if(zip == NULL)
	{
		IniciaZip(PD.gridFilePath);
	}
	// abre o zip
	zip->Open(PD.gridFilePath, zmRead);
	// loop p/ todas Subestacoes selecionadas
	for (int i = 0; i < slisNomes->Count; i++)
	{
		AnsiString nome = (AnsiString)slisNomes->Strings[i];
		for (int j = 0; j < lisTMP->Count; j++)
		{
			TSubestacao* subestacao = (TSubestacao*)lisTMP->Items[j];
			if (nome.AnsiCompareIC(subestacao->Codigo) == 0)
			{
				if (progresso)
					progresso->Add(nome);
				zip->Extract("Sub_" + nome + ".es", diretorioTemporario, true);
			}
		}
	}
	try
	{
		zip->Close();
	}
	catch (const Exception & e)
	{
		if (!BatchMode)
		{
			Erro(" Erro ao fechar o arquivo !(" + e.Message + ")");
		}
		else
		{
            if (batch != NULL)
			{
				batch->LogDetalheAdd(" Erro ao fechar o arquivo !(" + e.Message + ")");
			}
        }
	}
	delete zip;
	zip = NULL;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoUgrid::VersaoCompativel(AnsiString &versao)
{
	// variaveis locais
//	AnsiString nome, arq_ini, key, section;
//	double valor = 1.0; // default
//	TIniFile *ini = NULL;
//	bool compativel = false;
//
//	// inicia dados
//	arq_ini = "meta-inf.ini";
//	section = "META-INF";
//	key = "VERSION";
//	versao = DoubleToStr("%3.1f", valor);
//
//	for (int i = 0; i < zip->FileCount; i++)
//	{
//		nome = zip->FileNames[i];
//		if (nome.AnsiCompareIC(arq_ini) != 0)
//			continue;
//		// lê arquivo
//		zip->Extract(nome, diretorioTemporario, true);
//		ini = new TIniFile(diretorioTemporario + "\\" + nome);
//		versao = ini->ReadString(section, key, versao);
//		valor = StrToDouble(versao);
//		if (valor < VERSAO_UGRID)
//		{
//			break;
//		}
//		else
//		{
//			compativel = true;
//			break;
//		}
//	}
//	if (ini != NULL)
//		delete ini;
//	return compativel;
	return true;
}

// -----------------------------------------------------------------------------
// eof
