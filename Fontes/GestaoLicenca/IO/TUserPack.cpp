// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TUserPack.h"
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
__fastcall TUserPack::TUserPack(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria stringList
	pastas = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TUserPack::~TUserPack(void)
{
	// destrói objetos
	DestroiTObject(pastas);
}

// ---------------------------------------------------------------------------
void __fastcall TUserPack::DeletaPastas(void)
{
	// variaveis locais
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	AnsiString nome;

	for (int n = pastas->Count - 1; n >= 0; n--)
	{ // apaga todas as pastas
		nome = (AnsiString)pastas->Strings[n];
		dir->DeleteDirectory(nome);
		pastas->Delete(n);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TUserPack::DescompactaPack(UnicodeString packEmpresa)
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
int __fastcall TUserPack::DeterminaConjuntoEmpresa(int empresa_id)
{
	// variaveis locais
	int conjEmpresa;

	switch (empresa_id)
	{//cooperativas (todas)
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
	case CERMISSOES:
    case CERTAJA:
	case CERTEL:
	case COPREL:
	case CRELUZ:
	case CERGAL:
		conjEmpresa = CERTAJA; //20
		break;

	//edp band
	case EDP_BANDEIRANTES:
	case EDP:
		conjEmpresa = EDP_BANDEIRANTES; //28
		break;

	//eletrobras alagoas
	case CEPEL:
	case ELETROBRAS_HOLDING:
    case CEAL:
	case ELETROBRAS_ALAGOAS:
		conjEmpresa = ELETROBRAS_ALAGOAS; //35
		break;

	//outras empresas com seu próprio usuario
	case EDP_ESCELSA:
	case COELBA:
	case CELPE:
	case COSERN:
	case ELETROBRAS_ACRE:
	case ELETROBRAS_AMAZONAS:
	case ELETROBRAS_RONDONIA:
	case ELETROBRAS_RORAIMA:
	case ELETROBRAS_PIAUI:
	case COPEL:
    case CEMIG:
	case CEB:
	case ELETROPAULO:
	case RGESUL:
		conjEmpresa = empresa_id;
		break;

	//demo/outras empresas sem usuario
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
bool __fastcall TUserPack::Executa(UnicodeString &diagnostico)
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
	// localiza endereço do pack basico
	packBasico = path->DirDat() + "\\Packages\\Users\\000.up";
	if (!FileExists(packBasico))
	{
		diagnostico = "Não existe conjunto de arquivos básicos do programa";
		return false;
	}
	// localiza endereço do pack da empresa
	conjEmpresa = DeterminaConjuntoEmpresa(licenca->Empresa);
	//preenche com zeros se for menor que 100
	numEmpresa = NumEmpresaStr(conjEmpresa);
	packEmpresa = path->DirDat() + "\\Packages\\Users\\" + numEmpresa + ".up";
	if (!FileExists(packEmpresa))
	{
		diagnostico = "Não existe conjunto de arquivos da empresa " + licenca->EmpresaStr;
		return false;
	}

	// lista todas as pastas, exceto dat/UserPackages e bin
	ListaPastas();
	// apaga pastas
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
void __fastcall TUserPack::ListaPastas(void)
{
	// variaveis locais
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	int index;
	UnicodeString nomePasta;

	// lista todas as pastas do Dat
	pastas->Clear();
	nomePasta = path->DirDat();
	if (!dir->LisDirectory(nomePasta, pastas))
		return;
	// retira da lista a pasta Packages
	index = pastas->IndexOf(path->DirDat() + "\\Packages");
	if (index >= 0)
		pastas->Delete(index);
	// lista todas as pastas do dir_base
	nomePasta = path->DirBase;
	if (!dir->LisDirectory(nomePasta, pastas))
		return;
	// retira da lista a pasta Dat e Bin
	index = pastas->IndexOf(path->DirDat());
	if (index >= 0)
		pastas->Delete(index);
	index = pastas->IndexOf(path->DirBin);
	if (index >= 0)
		pastas->Delete(index);
}

// ---------------------------------------------------------------------------
UnicodeString __fastcall TUserPack::NumEmpresaStr(int num_empresa)
{
	// variaveis locais
	UnicodeString num_empresa_str;

	//verifica se é menor que 10
	if(num_empresa < 10)
	{
		num_empresa_str = "00";
	}
	//verifica se é menor que 100
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
