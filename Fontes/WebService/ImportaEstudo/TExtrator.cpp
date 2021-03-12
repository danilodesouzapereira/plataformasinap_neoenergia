// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "jpeg.hpp"
#include "TExtrator.h"
#include "VTMonta.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <System.NetEncoding.hpp>

// ---------------------------------------------------------------------------
VTExtrator* NewObjExtrator(VTApl *apl_owner)
{
	return (new TExtrator(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TExtrator::TExtrator(VTApl *apl_owner)
{
	// salva ponteiros p/ objeto
	this->apl = apl_owner;
	// cria
	croqui = new TByteDynArray();
	mem = new TMemoryStream();
	out = new TMemoryStream();
}

// ---------------------------------------------------------------------------
__fastcall TExtrator::~TExtrator(void)
{
	// destroi
	if (croqui)
	{
		delete croqui;
		croqui = NULL;
	}
	if (mem)
	{
		delete mem;
		mem = NULL;
	}
	if (out)
	{
		delete out;
		out = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TExtrator::CriaCroqui(AnsiString endereco_croqui)
{
	// variáveis locais
	int inteiro, tam;
	UnicodeString str, arq_txt;
	TBase64Encoding *encoder;
	TStringList *lis;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// reinicia croqui
	croqui->set_length(0);
	mem->Clear();
	out->Clear();

	try
	{ // verifica se o arquivo existe
		if (!FileExists(endereco_croqui))
			return;
		// cria objetos
		encoder = new TBase64Encoding();
		lis = new TStringList();
		// carrega arquivo com imagem do croqui
		mem->LoadFromFile(endereco_croqui);
		inteiro = encoder->Encode(mem, out);
		// determina caminho intermediario
		arq_txt = path->DirTmp() + "\\croqui.txt";
		out->SaveToFile(arq_txt);
		lis->LoadFromFile(arq_txt);
		// decodifica
		str = lis->Text;//Strings[0];
		*croqui = encoder->DecodeStringToBytes(str);
		// destroi objetos
		delete lis;
		delete encoder;

	}
	catch (Exception &e)
	{
		Erro("TExtrator::CriaCroqui(): erro ao criar croqui");
		croqui->set_length(0);
		if (lis)
			delete lis;
		if (encoder)
			delete encoder;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereAlternativa(TStrings * campos)
{
	// variáveis locais
	strALTERNATIVA str;

	// <ALTERNATIVA>; Descricao; RedeOriginal; justificativa
	try
	{ // valida número de dados recebidos
		if (campos->Count < 4)
		{
			Erro("TExtrator::InsereAlternativa(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.descricao = campos->Strings[1];
		str.rede_original = bool(StrToInteger(campos->Strings[2]));
		str.justificativa = campos->Strings[3];
		// insere Alternativa
		return (monta->InsereAlternativa(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereAlternativa(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereDemandaPotencia(TStrings * campos)
{
	// variáveis locais
	strDEMANDA_POTENCIA str;

	// <DemandaPotencia>; Ano; Valor;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 3)
		{
			Erro("TExtrator::InsereDemandaPotencia(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.ano = StrToInteger(campos->Strings[1]);
		str.s_mva = (StrToDouble(campos->Strings[2]));
		// insere DemandaPotencia
		return (monta->InsereDemandaPotencia(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereAlternativa(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereElementoPotencia(TStrings * campos)
{ // DVK 2015.04.28
	// variáveis locais
	strELEMENTO_POTENCIA str;

	// <ELEMENTO_POTENCIA>; ID; Codigo;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 3)
		{
			Erro("TExtrator::InsereElementoPotencia(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.id = StrToInteger(campos->Strings[1]);
		str.codigo = campos->Strings[2];
		// insere Equipamento
		return (monta->InsereElementoPotencia(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereElementoPotencia(): Exception");
		return (false);
	}
}
/*
// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereEquipamento(TStrings * campos)
{
	// variáveis locais
	strEQUIPAMENTO str;

	// <EQUIPAMENTO>; id_padrao; rede_cod; rede_tipo; tipo_acao; descricao; quantidade
	try
	{ // valida número de dados recebidos
		if (campos->Count < 7)
		{
			Erro("TExtrator::InsereEquipamento(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.padrao_construtivo_id = StrToInteger(campos->Strings[1]);
		str.rede.codigo = campos->Strings[2];
		str.rede.tipo_sistema = campos->Strings[3];
		str.tipo_acao = campos->Strings[4];
		str.descricao = campos->Strings[5];
		str.quantidade = StrToDouble(campos->Strings[6]);
		// insere Equipamento
		return (monta->InsereEquipamento(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereEquipamento(): Exception");
		return (false);
	}
}
*/
// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereEquipamento(TStrings * campos)
{
	// variáveis locais
	strEQUIPAMENTO str;

	// <EQUIPAMENTO>; id_padrao; tipo_acao; descricao; quantidade;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 5)
		{
			Erro("TExtrator::InsereEquipamento(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.padrao_construtivo_id = StrToInteger(campos->Strings[1]);
		str.tipo_acao = campos->Strings[2];
		str.descricao = campos->Strings[3];
		str.quantidade = StrToDouble(campos->Strings[4]);
		// insere Equipamento
		return (monta->InsereEquipamento(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereEquipamento(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
bool __fastcall TExtrator::InsereEstudo(TStrings * campos)
{
    // variáveis locais
    strESTUDO str;

    // <ESTUDO>; Usuario; Descricao; NumeroAlterntaivas; Justificativa
    try
    {   // valida número de dados recebidos
        if (campos->Count < 7)
        {
            Erro("TExtrator::InsereEstudo(): número de campos inválido");
            return (false);
        }
        // lê campos
        str.dominio_id      = StrToInteger(campos->Strings[1]);
        str.grupo_id        = StrToInteger(campos->Strings[2]);
        str.usuario         = campos->Strings[3];
        str.descricao       = campos->Strings[4];
        str.num_alternativa = StrToInteger(campos->Strings[5]);
        str.justificativa   = campos->Strings[6];
        // insere Estudo
        return (monta->InsereEstudo(str));
    }
    catch (Exception &e)
    {
        Erro("TExtrator::InsereEstudo(): Exception");
        return (false);
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereInformacaoTecnica(TStrings * campos)
{
	// variáveis locais
	strINFORMACAO_TECNICA str;

	// <informacaoTecnica>; Tipo;;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 2)
		{
			Erro("TExtrator::InsereInformacaoTecnica(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.tipo = campos->Strings[1];
		// insere Equipamento
		return (monta->InsereInformacaoTecnica(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereEquipamento(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereMunicipio(TStrings * campos)
{
	// variáveis locais
	strMUNICIPIO str;

	// <MUNICIPIO>; id_municipio; principal;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 3)
		{
			Erro("TExtrator::InsereMunicipio(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.idMunicipio = StrToInt(campos->Strings[1]);
		str.principal = (campos->Strings[2].CompareIC("1") == 0); //0=nao, 1=sim
		// insere Municipio
		return (monta->InsereMunicipio(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereMunicipio(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16216629
//https://sinapsisenergia.teamwork.com/#/tasks/16287949
bool __fastcall TExtrator::InsereObra(TStrings * campos)
{
	// variáveis locais
	strOBRA str;
	AnsiString caminho;

	// <OBRA>; TipoObra; Descricao; ObrigacaoEspecial; DataInicio; DataAtivacao; ParticipacaoOTR; croqui; codigo ; programa; grupo
	try
	{ // valida número de dados recebidos
		if (campos->Count < 12)
		{
			Erro("TExtrator::InsereObra(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.tipo_obra = campos->Strings[1];
		str.descricao = campos->Strings[2];
		str.obrigacao_especial = StrToDouble(campos->Strings[3]);
		str.data_inicio = StrToDate(campos->Strings[4]);
		str.data_ativacao = StrToDate(campos->Strings[5]);
		str.participacao_otr = StrToDouble(campos->Strings[6]);
		caminho = campos->Strings[7];
		CriaCroqui(caminho);
		str.croqui.set_length(croqui->Length);
		str.croqui = croqui->Copy();
		// str.croqui = *croqui;
		str.codigo = campos->Strings[8];
		str.programa = campos->Strings[9];
		try
		{
			str.grupo = StrToInt(campos->Strings[10]);
		}
		catch(...)
		{
			str.grupo = 0;
		}
        try
		{
			str.conjunto_obra = StrToInt(campos->Strings[11]);
		}
		catch(...)
		{
			str.conjunto_obra = 0;
		}
		// insere obra
		return (monta->InsereObra(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereObra(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InserePotenciaInstalada(TStrings * campos)
{
	// variáveis locais
	strPOTENCIA_INSTALADA str;

	// <PotenciaInstalada>; Ano; Valor;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 3)
		{
			Erro("TExtrator::InserePotenciaInstalada(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.ano = StrToInteger(campos->Strings[1]);
		str.s_mva = (StrToDouble(campos->Strings[2]));
		// insere DemandaPotencia
		return (monta->InserePotenciaInstalada(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InserePotenciaInstalada(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereRede(TStrings * campos)
{
	// variáveis locais
	strREDE str;

	//<REDE_EQP>;  rede; sist; antes_manobra ; redePai
	try
	{ // valida número de dados recebidos
		if (campos->Count < 5)
		{
			Erro("TExtrator::InsereRede(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.codigo = campos->Strings[1];
		str.tipo_sistema = campos->Strings[2];
		str.antes_manobra = StrToInt(campos->Strings[3]);
		str.codigo_rede_pai = (campos->Strings[4]);
		// insere Equipamento
		return (monta->InsereRede(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereRede(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereRedeAlternativa(TStrings * campos)
{
	// variáveis locais
	strREDE_ALTERNATIVA str;

	// <Rede>; Codigo; descricao; TipoSistema; TaxaCrescimentoAnual;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 5)
		{
			Erro("TExtrator::InsereRedeAlternativa(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.codigoGIS = campos->Strings[1];
		str.descricao = campos->Strings[2];
		str.tipoSistema = campos->Strings[3];
		str.taxa_crescimento_anual = (StrToDouble(campos->Strings[4]));
		// insere DemandaPotencia
		return (monta->InsereRedeAlternativa(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereRedeAlternativa(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereSolicitacao(TStrings * campos)
{
	// variáveis locais
	strSOLICITACAO str;

	// <SOLICITACAO>; id; usuario; descricao; subestacao; alimentador; data; responsavel; codigo; cod_ref; cod_doc
	try
	{ // valida número de dados recebidos
		if (campos->Count < 10)
		{
			Erro("TExtrator::InsereSolicitacao(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.id = StrToInteger(campos->Strings[1]);
		str.usuario = campos->Strings[2];
		str.descricao = campos->Strings[3];
		str.subestacao = campos->Strings[4];
		str.alimentador = campos->Strings[5];
		str.data = StrToDate(campos->Strings[6]);
		str.responsavel = campos->Strings[7];
		str.cod_ref = campos->Strings[8];
		str.cod_doc = campos->Strings[9];
		// insere Solicitacao
		return (monta->InsereSolicitacao(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereSolicitacao(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereValorInformacaoTecnica(TStrings * campos)
{
	// variáveis locais
	strVALOR_INFORMACAO_TECNICA str;

	// <Rede>; Codigo; TaxaCrescimentoAnual;
	try
	{ // valida número de dados recebidos
		if (campos->Count < 3)
		{
			Erro("TExtrator::InsereValorInformacaoTecnica(): número de campos inválido");
			return (false);
		}
		// lê campos
		str.ano = StrToInteger(campos->Strings[1]);
		str.valor = (StrToDouble(campos->Strings[2]));
		// insere DemandaPotencia
		return (monta->InsereValorInformacaoTecnica(str));
	}
	catch (Exception &e)
	{
		Erro("TExtrator::InsereValorInformacaoTecnica(): Exception");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::TrataInicio(void)
{
	// determina objetos
	monta = (VTMonta*)apl->GetObject(__classid(VTMonta));

	return (monta != NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::TrataTermino(void)
{
	// nada a fazer
	return (true);
}

// -----------------------------------------------------------------------------
// eof
