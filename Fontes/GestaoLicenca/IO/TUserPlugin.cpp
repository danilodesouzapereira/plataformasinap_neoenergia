// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TUserPlugin.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Diretorio\DirArq.h"
#include "..\..\Diretorio\VTDir.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Licenca\VTLicenca.h"
#include "..\..\Licenca\VTLicencas.h"
#include "..\..\..\DLL_Inc\Funcao.h"

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
	DestroiTObject(pastas);
}

// ---------------------------------------------------------------------------
void __fastcall TUserPlugin::DeletaPastas(void)
{
	// variaveis locais
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	UnicodeString nomePasta;

	// localiza pasta Plugins
	nomePasta = path->DirBase + "\\Plugins";
	if(!DirectoryExists(nomePasta))
		return;
	//apaga pasta
	dir->DeleteDirectory(nomePasta);
}

// ---------------------------------------------------------------------------
bool __fastcall TUserPlugin::DescompactaPack(UnicodeString packEmpresa)
{
	// variaveis locais
	TZipFile *zip;
	UnicodeString dirBase;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// define dirBase
	dirBase = path->DirBase;
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
		DestroiTObject(zip);
	}
	catch (const Exception & e)
	{
		Erro("Erro ao descompactar o arquivo! (" + e.Message + ")");
		// destroi objeto
		DestroiTObject(zip);
	}
	return true;
}

// ---------------------------------------------------------------------------
int __fastcall TUserPlugin::DeterminaConjuntoEmpresa(int empresa_id)
{
	// variaveis locais
	int conjEmpresa;

	switch (empresa_id)
	{//cooperativas MIG
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
		conjEmpresa = COOPERALIANCA;  //51
		break;

	//cooperativas SGD
	case CERMISSOES:
	case CERTAJA:
	case CERTEL:
	case COPREL:
	case CRELUZ:
	case CERGAL:
		conjEmpresa = CERTAJA; //20
		break;

	//edp
	case EDP_BANDEIRANTES:
	case EDP_ESCELSA:
	case EDP:
		conjEmpresa = EDP;  //33
		break;

	//eletrobras SOL
	case CEAL:
	case CEPEL:
	case ELETROBRAS_HOLDING:
	case ELETROBRAS_ALAGOAS:
	case ELETROBRAS_PIAUI:
		conjEmpresa = ELETROBRAS_ALAGOAS;  //35
		break;

	//eletrobras SGD
	case ELETROBRAS_ACRE:
	case ELETROBRAS_AMAZONAS:
	case ELETROBRAS_RONDONIA:
	case ELETROBRAS_RORAIMA:
		conjEmpresa = ELETROBRAS_RONDONIA; //38
		break;

	//neoenergia
	case COELBA:
	case CELPE:
	case COSERN:
		conjEmpresa = COELBA; //71
		break;

	//outras empresas de plugin unico
	case COPEL:
    case CEMIG:
	case CEB:
	case ELETROPAULO:
	case RGESUL:
		conjEmpresa = empresa_id;
		break;

	//demo/outras empresas sem plugin
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
	default:
		conjEmpresa = 1;
		break;

	}
	return conjEmpresa;
}

// ---------------------------------------------------------------------------
bool __fastcall TUserPlugin::Executa(UnicodeString &diagnostico)
{
	// variaveis locais
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	bool sucesso = false;
	UnicodeString packEmpresa, packBasico, numEmpresa;
	int conjEmpresa;

	// proteção
	licenca = licencas->ExisteLicenca();
	if (licenca == NULL)
	{
		diagnostico = "Não existe licença de uso";
		return (false);
	}
	// localiza endereço do pack basico (modulos opcionais)
	packBasico = path->DirDat() + "\\Packages\\Plugins\\000.pp";
	if (!FileExists(packBasico))
	{
		diagnostico = "Não existe conjunto de módulos opcionais do programa";
		return false;
	}
	// localiza endereço do pack da empresa
	conjEmpresa = DeterminaConjuntoEmpresa(licenca->Empresa);
	//preenche com zeros se for menor que 100
	numEmpresa = NumEmpresaStr(conjEmpresa);
	packEmpresa = path->DirDat() + "\\Packages\\Plugins\\" + numEmpresa + ".pp";
	if (!FileExists(packEmpresa))
	{
		diagnostico = "Não existe conjunto de módulos da empresa " + licenca->EmpresaStr;
		return false;
	}

	// deleta somente Plugins
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
		diagnostico = "Erro ao descompactar arquivos da empresa " + licenca->EmpresaStr;
		return false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
UnicodeString __fastcall TUserPlugin::NumEmpresaStr(int num_empresa)
{
	// variaveis locais
	UnicodeString num_empresa_str;

	//verifica se é menor que 10
	if(num_empresa < 10)
	{
		num_empresa_str = "00";
	}
	else if(num_empresa < 100)
	{
		num_empresa_str = "0";
    }
	//converte para texto com os 0 (se tiver)
	num_empresa_str += IntToStr(num_empresa);
	return num_empresa_str;
}

// -----------------------------------------------------------------------------
// eof
