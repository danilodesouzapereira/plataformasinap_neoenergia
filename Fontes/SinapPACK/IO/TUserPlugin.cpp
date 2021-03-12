// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TUserPlugin.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\DirArq.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Licenca\VTEmpresa.h>
#include <Fontes\Produto\VTProduto.h>
#include <Fontes\Registro\VTRegistro.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

// ---------------------------------------------------------------------------
__fastcall TUserPlugin::TUserPlugin(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria stringList
	pastas = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TUserPlugin::~TUserPlugin(void)
{
	// destrói objetos
	if (pastas != NULL)
	{
		delete pastas;
		pastas = NULL;
	}
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TUserPlugin::DeletaPastas(void)
 {
 // variaveis locais
 VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
 VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
 UnicodeString nomePasta, filename;

 // localiza pasta Plugins
 nomePasta = path->DirBase + "\\Plugins";
 if (DirectoryExists(nomePasta))
 { // apaga pasta
 dir->DeleteDirectory(nomePasta);
 }
 // verifica se existem os arquivos SinapToolKit.dll e ImportaRedeGis.dll na pasta Bin
 filename = path->DirBin + "\\SinapToolKit.dll";
 if (FileExists(filename))
 {
 DeleteFile(filename);
 }
 filename = path->DirBin + "\\ImportaRedeGis.dll";
 if (FileExists(filename))
 {
 DeleteFile(filename);
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TUserPlugin::DeletaPastas(UnicodeString packEmpresa)
{
	// variaveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	UnicodeString nomePasta, filename;
	TZipFile *zip;

	try
	{ // cria zip
		zip = new TZipFile();
		zip->UTF8Support = true;
		// abre o arquivo
		zip->Open(packEmpresa, zmRead);
		for (int n = 0; n < zip->FileCount; n++)
		{ // localiza arquivo do pack na pasta Bin
			filename = path->DirBin + "\\" + zip->FileName[n];
			if (FileExists(filename))
			{//apaga
				DeleteFile(filename);
			}
		}
		// fecha o arquivo
		zip->Close();
		// destroi objeto
		delete zip;
	}
	catch (const Exception & e)
	{
		// destroi objeto
		if (zip != NULL)
		{
			delete zip;
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TUserPlugin::DescompactaPack(UnicodeString packEmpresa)
{
	// variaveis locais
	TZipFile *zip;
	UnicodeString dirBase;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// define dirBin com sendo base
	dirBase = path->DirBin; // path->DirBase;
	try
	{ // cria zip
		zip = new TZipFile();
		zip->UTF8Support = true;
		// abre o arquivo
		zip->Open(packEmpresa, zmRead);
		// extrai o arquivo (para dirBase)
		zip->ExtractAll(dirBase);
		// fecha o arquivo
		zip->Close();
		// destroi objeto
		delete zip;
	}
	catch (const Exception & e)
	{
		// Erro("Erro ao descompactar o arquivo! (" + e.Message + ")");
		// destroi objeto
		if (zip != NULL)
		{
			delete zip;
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15701165
// https://sinapsisenergia.teamwork.com/#/tasks/17646659
// https://sinapsisenergia.teamwork.com/#/tasks/18194486
int __fastcall TUserPlugin::DeterminaConjuntoEmpresa(int empresa_id)
{
	// variaveis locais
	int conjEmpresa;

	switch (empresa_id)
	{ // cooperativas MIG
	case FECOERGS:
	case CERILUZ:
	case COOPERLUZ:
	case CRERAL:
	case CEJAMA:
	case CERGAPA:
	case CERBRANORTE:
	case CERGRAL:
	case COOPERMILA:
	case CERPALO:
	case COOPERALIANCA:
	case CEPRAG:
	case CERSUL:
	case CERMOFUL:
	case COOPERCOCAL:
	case CEREJ:
	case COORSEL:
	case CERAL_ANITAPOLIS:
	case CERACA:
	case FECOERUSC:
	case CEGERO:
	case CERSAD:
	case CEESAM:
	case COOPERZEM:
	case CERTHIL:
	case CERTREL:
	case COOPERNORTE:
	case CERTAJA: //alterado para MIG
    case CERMISSOES: //alterado para MIG
// nova empresa
    case CERFOX:
		conjEmpresa = COOPERALIANCA; // 51
		break;

		// cooperativas SGD
	//case CERMISSOES:
	//case CERTAJA:
	case CERTEL:
	case COPREL:
	case CRELUZ:
	case CERGAL:
		conjEmpresa = CERTAJA; // 20
		break;

		// edp
	case EDP_BANDEIRANTES:
	case EDP_ESCELSA:
	case EDP:
		conjEmpresa = EDP; // 33
		break;

		// eletrobras SOL
   //	case CEAL:
	case CEPEL:
	case ELETROBRAS_HOLDING:
	case ELETROBRAS_ALAGOAS:
	case ELETROBRAS_PIAUI:
		conjEmpresa = ELETROBRAS_ALAGOAS; // 35
		break;

		// eletrobras SGD
	case ELETROBRAS_ACRE:
	case ELETROBRAS_AMAZONAS:
	case ELETROBRAS_RONDONIA:
	case ELETROBRAS_RORAIMA:
		conjEmpresa = ELETROBRAS_RONDONIA; // 38
		break;

		// neoenergia
	case COELBA:
	case CELPE:
	case COSERN:
		conjEmpresa = COELBA; // 71
		break;

		// outras empresas de plugin unico
	case COPEL:
	case CEMIG:
	case CEB:
	case ELETROPAULO:
	case RGESUL:
	case CPFL:
	case LIGHT:
		conjEmpresa = empresa_id;
		break;

		// demo/outras empresas sem plugin
	case GRUPOREDE:
	case CEEE:
	case SINAPSIS:
	case ENERQ:
	case MACKENZIE:
	case UFABC:
	case UFBA:
	case UNISINOS:
	case URUGUAI:
	case USP:
	case UNIJORGE:
	case REINO_UNIDO:
	case COOPER:
	case IFES:
	case UFJF:
	case IFSP:
	case UFES:
	case UNIVATES:
	case UNIFEI:
	case USF:
	default:
		conjEmpresa = 1;
		break;

	}
	return conjEmpresa;
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TUserPlugin::Executa(UnicodeString &diagnostico)
 {
 // variaveis locais
 VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
 VTEmpresa *empresa = (VTEmpresa*)apl->GetObject(__classid(VTEmpresa));
 VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
 VTRegistro *registro = (VTRegistro*)apl->GetObject(__classid(VTRegistro));
 bool sucesso = false;
 UnicodeString packEmpresa, packBasico;
 AnsiString numEmpresa;
 int conjEmpresa, empresaId;

 // determina empresa pelo registro
 if (!registro->ExisteInfo(produto->CodRev, "empresa_id", numEmpresa))
 return false;

 // localiza endereço do pack basico (modulos opcionais)
 packBasico = path->DirDat() + "\\Packages\\Plugins\\000.pp";
 if (!FileExists(packBasico))
 {
 diagnostico = "Não existe conjunto de módulos opcionais do programa";
 return false;
 }
 // localiza endereço do pack da empresa
 empresaId = StrToInt(numEmpresa);
 conjEmpresa = DeterminaConjuntoEmpresa(empresaId);
 // preenche com zeros se for menor que 100
 numEmpresa = NumEmpresaStr(conjEmpresa);
 packEmpresa = path->DirDat() + "\\Packages\\Plugins\\" + numEmpresa + ".pp";
 if (!FileExists(packEmpresa))
 {
 diagnostico = "Não existe conjunto de módulos da empresa " + empresa->EmpresaAsStr
 (empresaId);
 return false;
 }

 // deleta somente Plugins (e 2 DLLs se existirem no Bin)
 DeletaPastas();
 // descompacta pacote basico
 sucesso = DescompactaPack(packBasico);
 if (!sucesso)
 {
 diagnostico = "Erro ao descompactar arquivos básicos do programa";
 return false;
 }
 // descompacta pacote da empresa associada à licença
 sucesso = DescompactaPack(packEmpresa);
 if (!sucesso)
 {
 diagnostico = "Erro ao descompactar arquivos da empresa " + empresa->EmpresaAsStr
 (empresaId);
 return false;
 }
 return (sucesso);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TUserPlugin::Executa(UnicodeString &diagnostico)
{
	// variaveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTEmpresa *empresa = (VTEmpresa*)apl->GetObject(__classid(VTEmpresa));
	VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
	VTRegistro *registro = (VTRegistro*)apl->GetObject(__classid(VTRegistro));
	bool sucesso = false;
	UnicodeString packEmpresa;
	AnsiString numEmpresaNew, numEmpresaOld;
	int conjEmpresa, empresaId;

	// determina empresa nova pelo registro
	if (!registro->ExisteInfo(produto->CodRev, "empresa_new_id", numEmpresaNew))
		return false;
	// determina empresa antiga pelo registro
	if (!registro->ExisteInfo(produto->CodRev, "empresa_old_id", numEmpresaOld))
		numEmpresaOld = "";

	// localiza endereço do pack da empresa antiga
	if (!numEmpresaOld.IsEmpty())
	{
		empresaId = StrToInt(numEmpresaOld);
		conjEmpresa = DeterminaConjuntoEmpresa(empresaId);
		// preenche com zeros se for menor que 100
		numEmpresaOld = NumEmpresaStr(conjEmpresa);
		packEmpresa = path->DirDat() + "\\Packages\\Plugins\\" + numEmpresaOld + ".pp";
		if (!FileExists(packEmpresa))
		{
			diagnostico = "Não existe conjunto de módulos da empresa anterior: " +
				empresa->EmpresaAsStr(empresaId);
			return false;
		}
		// deleta arquivos da empresa antiga
		DeletaPastas(packEmpresa);
	}
	// deleta somente Plugins (e 2 DLLs se existirem no Bin)
	// DeletaPastas();
	// localiza endereço do pack da empresa nova
	empresaId = StrToInt(numEmpresaNew);
	conjEmpresa = DeterminaConjuntoEmpresa(empresaId);
	// preenche com zeros se for menor que 100
	numEmpresaNew = NumEmpresaStr(conjEmpresa);
	packEmpresa = path->DirDat() + "\\Packages\\Plugins\\" + numEmpresaNew + ".pp";
	if (!FileExists(packEmpresa))
	{
		diagnostico = "Não existe conjunto de módulos da empresa nova: " + empresa->EmpresaAsStr
			(empresaId);
		return false;
	}

	// descompacta pacote da empresa associada à licença
	sucesso = DescompactaPack(packEmpresa);
	if (!sucesso)
	{
		diagnostico = "Erro ao descompactar arquivos da empresa " + empresa->EmpresaAsStr
			(empresaId);
		return false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
UnicodeString __fastcall TUserPlugin::NumEmpresaStr(int num_empresa)
{
	// variaveis locais
	UnicodeString num_empresa_str;

	// verifica se é menor que 10
	if (num_empresa < 10)
	{
		num_empresa_str = "00";
	}
	else if (num_empresa < 100)
	{
		num_empresa_str = "0";
	}
	// converte para texto com os 0 (se tiver)
	num_empresa_str += IntToStr(num_empresa);
	return num_empresa_str;
}

// -----------------------------------------------------------------------------
// eof
