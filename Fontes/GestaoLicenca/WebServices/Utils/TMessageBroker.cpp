// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#include <System.DateUtils.hpp>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include "TMessageBroker.h"
#include "..\Entities\TRetornoAlocacaoLicenca.h"
#include "..\Entities\TRetornoLiberacaoLicenca.h"
#include "..\Entities\TLicencaAlocada.h"
#include "..\Entities\TLicencaRequest.h"
#include <Fontes\Util\TExtraiCampos.h>
#include <DLL_Inc\Funcao.h>

// ------------------------------------------------------------------------------------------- class

// ------------------------------------------------------------------------------------- constructor
__fastcall TMessageBroker::TMessageBroker(void)
{
	//nada
}

// ------------------------------------------------------------------------------------------------
__fastcall TMessageBroker::~TMessageBroker(void)
{
	//destroi objetos
	DestroiTObject(retornoAloca);
	DestroiTObject(retornoLibera);
}

// ----------------------------------------------------------------------------------------- methods
TDate __fastcall TMessageBroker::ConverteStrToDate(AnsiString data)
{
	TExtraiCampos *ExtraiCampos;
	TStrings *campos;
	TDate date;
	int dia,mes,ano;
//	AnsiString ansiData;

	//default
	date = Now();

	ExtraiCampos = new TExtraiCampos();
    // configura opções de ExtraiCampos
	ExtraiCampos->SeparadorInsere('/');
	ExtraiCampos->SetOpcaoTrimCampos(true);
//	ansiData = (AnsiString) data;
	campos = ExtraiCampos->Executa(data);
	//protecao
	if (campos->Count == 3)
	{
       try
		{
			dia = StrToInt(campos->Strings[0]);
			mes = StrToInt(campos->Strings[1]);
			ano = StrToInt(campos->Strings[2]);
			date = EncodeDate((Word)ano, (Word)mes, (Word)dia);
		}
		catch(...)
		{
			date = Now();
		}
	}

	if (ExtraiCampos)
	{
		delete ExtraiCampos;
		ExtraiCampos = NULL;
	}

	return date;
}
// -----------------------------------------------------------------------------------------
TRetornoAlocacaoLicenca* TMessageBroker::ToRetornoAlocacaoLicenca(TJSONValue *jValue)
{
	//variaveis locais
	TJSONObject *jRoot, *jObjectLicenca;//, *jObjectOperacoes;
	TJSONValue *jValueLicenca, *jValueOperacoes;
	TJSONArray *jArray;
	TLicencaAlocada *licencaAlocada;
	DynamicArray<int> operacoes;

	//cria objeto
	retornoAloca = new TRetornoAlocacaoLicenca();
	// insere dados
	jRoot = dynamic_cast<TJSONObject*>(jValue);
	retornoAloca->Sucesso = StrToBool(jRoot->Values["sucesso"]->Value());
	retornoAloca->Mensagem = jRoot->Values["mensagem"]->Value();

	if (retornoAloca->Sucesso)
	{ // preenche dados da licença
		licencaAlocada = retornoAloca->LicencaAlocada;
		jValueLicenca = jRoot->Values["licencaAlocada"];
		jObjectLicenca = dynamic_cast<TJSONObject*>(jValueLicenca);
		licencaAlocada->DataAlocada = ConverteStrToDate(jObjectLicenca->Values["dataAlocada"]->Value());
		licencaAlocada->DataTermino = ConverteStrToDate(jObjectLicenca->Values["dataTermino"]->Value());
		licencaAlocada->Tipo = StrToInt(jObjectLicenca->Values["tipo"]->Value());
		licencaAlocada->Validade = StrToInt(jObjectLicenca->Values["validade"]->Value());

		// preenche dados das operacoes
		jValueOperacoes = jObjectLicenca->Values["operacoes"];
		jArray = dynamic_cast<TJSONArray*>(jValueOperacoes);
		operacoes.Length = jArray->Count;
		for (int i = 0; i < jArray->Count; i++)
		{
			operacoes[i] = jArray->Items[i]->Value().ToInt();
		}
        licencaAlocada->Operacoes = operacoes;
	}

	return retornoAloca;
}

// ------------------------------------------------------------------------------------------------
TRetornoLiberacaoLicenca* TMessageBroker::ToRetornoLiberacaoLicenca(TJSONValue *jValue)
{
	//variaveis locais
	TJSONObject *jRoot;//, *jObjectLicenca, *jObjectOperacoes;
//	TJSONValue *jValueLicenca;
//	TJSONArray *jArray;

	//cria objeto
	retornoLibera = new TRetornoLiberacaoLicenca();
	// insere dados
	jRoot = dynamic_cast<TJSONObject*>(jValue);
	retornoLibera->Sucesso = StrToBool(jRoot->Values["sucesso"]->Value());
	retornoLibera->Mensagem = jRoot->Values["mensagem"]->Value();

	return retornoLibera;
}

// --------------------------------------------------------------------------------------------- eof
