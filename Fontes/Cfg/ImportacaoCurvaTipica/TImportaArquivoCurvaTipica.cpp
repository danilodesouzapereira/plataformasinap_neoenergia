// --------------------------------------------------------------------------------------------- bof
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <vcl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <StrUtils.hpp>
#include <System.JSON.hpp>
#include <System.RegularExpressions.hpp>

#include <..\Apl\VTApl.h>
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Rede\VTEqpto.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Constante\Const.h"

#include "TAplCfg.h"
#include "TImportaArquivoCurvaTipica.h"
#include "TValidaArqCurva.h"

// --------------------------------------------------------------------------------------- namespace
using namespace std;
//using namespace sinapsis::sinap::configuracao;

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
__fastcall TImportaArquivoCurvaTipica::TImportaArquivoCurvaTipica(TAplCfg *apl)
{
	this->apl = apl;
	curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
}

// -------------------------------------------------------------------------------------------------
__fastcall TImportaArquivoCurvaTipica::~TImportaArquivoCurvaTipica(void)
{
	//nada
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoCurvaTipica::AdicionarPonto(tipoCURVA tipoCurva, VTCurva* curva,
	AnsiString linha)
{

	// variáveis locais
	strHM hm_ini, hm_fim;

	// separa os campos do .csv (split na string)
	TStringList *campoList = new TStringList();
	campoList->Delimiter = L';';
	campoList->DelimitedText = linha;

	// separa os campos do INICIO
	TStringList *inicioList = new TStringList();
	inicioList->Delimiter = L':';
	inicioList->DelimitedText = campoList->Strings[0];

	// separa os campos do FIM
	TStringList *fimList = new TStringList();
	fimList->Delimiter = L':';
	fimList->DelimitedText = campoList->Strings[1];

	// define o inicio
	hm_ini.hora = StrToInt(inicioList->Strings[0]);
	hm_ini.minuto = StrToInt(inicioList->Strings[1]);

	// define o fim
	hm_fim.hora = StrToInt(fimList->Strings[0]);
	hm_fim.minuto = StrToInt(fimList->Strings[1]);

	switch (tipoCurva)
	{
	case curvaPFP_MDP:
		{
			double valor[nvCURVA_PFP_MDP];

			// define o valor: POTENCIA_ATIVA
			valor[0] = StrToDouble(campoList->Strings[2]);

			// define o valor: FATOR_POTENCIA
			valor[1] = StrToDouble(campoList->Strings[3]);

			// dp(P)
			valor[2] = 0.0;

			// dp(FP)
			valor[3] = 0.0;

			curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PFP_MDP);
			break;
		}

	case curvaLD:
		{
			double valor[nvCURVA_LD];

			// define o valor: LIGADO
			valor[0] = StrToDouble(campoList->Strings[2]);

			curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_LD);
			break;
		}
    default: break;
	}
	//destroi string lists
	delete campoList;
	delete inicioList;
	delete fimList;
}

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TImportaArquivoCurvaTipica::CorrigeLinha(AnsiString linha)
{

	// 1) remove os 3 ';' do final da linha(quando estes estiverem presentes)
	if (linha.Pos(";;;") > 0)
	{
		linha = AnsiReplaceStr(linha, ";;;", "");
	}

	// 2) remove o ';' do final da linha(quando presente)
	if (linha.SubString(linha.Length(), linha.Length()) == ";")
	{
		linha = linha.SubString(0, linha.Length() - 1);
	}

	// 3) substituí "" por ", comum nas linhas com JSON
	if (linha.Pos("\"\"") > 0)
	{
		linha = AnsiReplaceStr(linha, "\"\"", "\"");
	}

	// 4) remove '"' do final e começo das linhas com JSON
	if (linha.Pos("\"{") > 0)
	{
		linha = AnsiReplaceStr(linha, "\"{", "{");
	}
	if (linha.Pos("}\"") > 0)
	{
		linha = AnsiReplaceStr(linha, "}\"", "}");
	}

	return linha;
}

// -------------------------------------------------------------------------------------------------
VTCurva* __fastcall TImportaArquivoCurvaTipica::CriarCurva(tipoCURVA tipoCurva, AnsiString linha)
{
	AnsiString Lim_superior = "";
	// JSON Parses para pegar as propriedades da curva
	TJSONObject *JSON = (TJSONObject*)TJSONObject::ParseJSONValue((UnicodeString)linha);

	// cria uma nova Curva
	VTCurva *curva = DLL_NewObjCurva();

	// propriedades comuns
	curva->Id = FalsoId();
	curva->Escala = pat01HORA;
	curva->Tipica = true;
	curva->Status[sttNOVO] = true;
	curva->Eqpto = NULL;
	curva->NumeroValores = StrToInt(JSON->Get("PONTOS")->JsonValue->Value());

	if (tipoCurva == curvaPFP_MDP)
	{
		curva->Tipo = curvaPFP_MDP;
		curva->Unidade = unidMVA;
		curva->NumeroValores = nvCURVA_PFP_MDP;
		curva->DiaMedicao = StrToInt(JSON->Get("MEDICAO")->JsonValue->Value());
		curva->LimiteInferior =
			StrToDouble(JSON->Get("FAIXA_CONSUMO_INFERIOR")->JsonValue->Value());
		curva->LimiteSuperior =
			StrToDouble(JSON->Get("FAIXA_CONSUMO_SUPERIOR")->JsonValue->Value());
		Lim_superior = (JSON->Get("FAIXA_CONSUMO_SUPERIOR")->JsonValue->Value());
		if (Lim_superior.AnsiCompareIC("-1") == 0)
		{
			curva->Codigo = (JSON->Get("NOME")->JsonValue->Value()) + " [Acima de " +
				(JSON->Get("FAIXA_CONSUMO_INFERIOR")->JsonValue->Value()) + "]";
		}
		else
		{
			curva->Codigo = (JSON->Get("NOME")->JsonValue->Value()) + " [" +
				(JSON->Get("FAIXA_CONSUMO_INFERIOR")->JsonValue->Value()) + " - " +
				(JSON->Get("FAIXA_CONSUMO_SUPERIOR")->JsonValue->Value()) + "]";
		}
		// classe
		VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
		int tagClasse = StrToInt(JSON->Get("CLASSE")->JsonValue->Value());
		curva->Classe = classes->ExisteClasseTag(tagClasse);

	}
	else if (tipoCurva == curvaLD)
	{
		curva->Codigo = JSON->Get("NOME")->JsonValue->Value();
		curva->Tipo = curvaLD;
		curva->Unidade = unidON_OFF;
		curva->NumeroValores = nvCURVA_LD;
		curva->Classe = NULL;
		curva->LimiteInferior = 0.;
		curva->LimiteSuperior = 0.;
	}
	else
	{
		curva->Codigo = JSON->Get("NOME")->JsonValue->Value();
	}
	curva->Reconfigura(pat01HORA);

	return curva;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaArquivoCurvaTipica::ExcluirCurvasTipicas()
{
	int pos_CONS, length_CONS, pos_IP, length_IP;
	apl->LeBaseCurvaTipica();

	// apaga as curvas de demanda de consumidor
	TList *curvaConsumidorList = curvas->ListaCurvaTipicaCarga();
	VTCurva *curva;
	for (int i = 0; i < curvaConsumidorList->Count; i++)
	{
		curva = (VTCurva*)curvaConsumidorList->Items[i];
		// Verifica se existe ou não uma curva que contém "OLD" no nome
		pos_CONS = curva->Codigo.AnsiPos("_OLD_");
		length_CONS = curva->Codigo.Length();
		if (pos_CONS > 0)
		{
			curva->Codigo = (curva->Codigo.SubString(1, pos_CONS-1) + "_OLD_" +
				DateTimeToStr(Now()));
		}
		else
		{
			curva->Codigo = (curva->Codigo + "_OLD_" + DateTimeToStr(Now()));
		}
		curva->Status[sttEXCLUIDO] = true;
		apl->SalvaBaseCurvaTipica(curva);
	}

	// apaga as curvas de Iluminação pública
	TList *curvaIluminacaoPublicaList = curvas->ListaCurvaTipicaIP();
	for (int i = 0; i < curvaIluminacaoPublicaList->Count; i++)
	{
		curva = (VTCurva*)curvaIluminacaoPublicaList->Items[i];
		// Verifica se existe ou não uma curva que contém "OLD" no nome
		pos_IP = curva->Codigo.AnsiPos("_OLD_");
		length_IP = curva->Codigo.Length();
		if (pos_IP > 0)
		{
//            curva->Codigo = (curva->Codigo.SubString(1, length_IP + 1 - pos_IP) + "_OLD_" +
//				DateTimeToStr(Now()));
			curva->Codigo = (curva->Codigo.SubString(1, pos_IP-1) + "_OLD_" +
				DateTimeToStr(Now()));
		}
		else
		{
			curva->Codigo = (curva->Codigo + "_OLD_" + DateTimeToStr(Now()));
		}
		curva->Status[sttEXCLUIDO] = true;
		apl->SalvaBaseCurvaTipica(curva);
	}

}

// -------------------------------------------------------------------------------------------------
int __fastcall TImportaArquivoCurvaTipica::Executa(AnsiString filePath)
{

	// cria o arquivo de entrada   throw std::runtime_error("error");
	//
	ifstream arquivoEntrada(filePath.c_str(), ios::in);

	string linha1;
	getline(arquivoEntrada, linha1);

	AnsiString linha = CorrigeLinha(linha1.c_str());


	if (linha.AnsiCompareIC("//     *****   Instrucoes para preenchimento das Curvas Tipicas de Demanda dos Consumidores   *****")
		!= 0)
		{
		throw std::runtime_error("Arquivo inválido!.");

		}

	// verifica se o arquivo foi aberto corretamente
	if (!arquivoEntrada)
	{
		// lança uma exceção
		throw std::runtime_error
			("Não foi possível abrir o arquivo! - Verifique o caminho e nome do arquivo.");
	}

	//
	// le as curvas do arquivo
	//
	string line;

	// fluxoProcessa = 0-nao achou curva, 1-processou curva, 2-processou header, 3-processando csv
	int fluxoProcessa = 0;

	VTCurva *curva = NULL;
	int tipoCurva = 0;
	int countLine = 0;

	TList *curvaList = new TList();

	// le a linha do arquivo
	while (getline(arquivoEntrada, line))
	{
		++countLine;

		AnsiString linha = CorrigeLinha(line.c_str());

		// 1. processa a linha que define a curva
		if ((fluxoProcessa != 1) && (linha.UpperCase().Pos("\"TIPO_CURVA\"")) > 0)
		{
			try
			{
				TJSONObject *JSON = (TJSONObject*)TJSONObject::ParseJSONValue((UnicodeString)linha);
				tipoCurva = StrToInt(JSON->Get("TIPO_CURVA")->JsonValue->Value());
			}
			catch (Exception &e)
			{
				// lança uma exceção
				AnsiString msg =
					("Erro ao processar o arquivo! - Verifique a linha " + IntToStr(countLine) +
					" '" + linha + "'");
				throw std::runtime_error(msg.c_str());
			}

			//cria nova
			curva = CriarCurva((tipoCURVA)tipoCurva, linha);
			fluxoProcessa = 1;
			continue;
		}

		// 2. processa a linha do header do csv
		if (fluxoProcessa == 1)
		{
			if (!IsHeaderValido((tipoCURVA)tipoCurva, linha))
			{
				// lança uma exceção
				AnsiString msg =
					("Erro ao processar o arquivo! - Verifique a linha " + IntToStr(countLine) +
					" '" + linha + "'");
				throw std::runtime_error(msg.c_str());
			}

			fluxoProcessa = 2;

			continue;
		}

		// 3. processa csv
		if (((fluxoProcessa == 2) || (fluxoProcessa == 3)) && IsLinhaMedicao(linha))
		{
			AdicionarPonto((tipoCURVA)tipoCurva, curva, linha);
			fluxoProcessa = 3;
			continue;
		}

		// 4. adiciona a curva na lista de curva
		if ((fluxoProcessa == 3) && !IsLinhaMedicao(linha))
		{

			if (curvaList->IndexOf(curva) == -1)
			{
				curvaList->Add(curva);
			}

		}

	}

	// adiciona a ultima curva processada na lista de curva
	if (curvaList->IndexOf(curva) == -1)
	{
		curvaList->Add(curva);
	}

	//
	// limpa a base(apaga todas as curvas de demanda de consumidor e iluminação pública)
	//
	ExcluirCurvasTipicas();

	//
	// insere as curvas novas
	//
	int countCurvasSalvas = 0;

	for (int i = 0; i < curvaList->Count; i++)
	{

		VTCurva *curva = (VTCurva*)curvaList->Items[i];

		if (curva->Tipo == curvaPFP_MDP)
			curvas->InsereCurvaTipicaCarga(curva);
		else if (curva->Tipo == curvaLD)
			curvas->InsereCurvaTipicaIP(curva);

		try
		{
			apl->SalvaBaseCurvaTipica(curva);
			countCurvasSalvas++;
		}
		catch (...)
		{
			// só para contar corretamente as linhas que foram efetivamente gravadas no banco
		}
	}

	// fecha o arquivo
	arquivoEntrada.close();

	delete curvaList;
	curvaList = NULL;

	return countCurvasSalvas;

}
/*
// -------------------------------------------------------------------------------------------------
// int __fastcall TImportaArquivoCurvaTipica::Importar(AnsiString filePath) throw(runtime_error)
int __fastcall TImportaArquivoCurvaTipica::Importar(AnsiString filePath)
{

	// cria o arquivo de entrada   throw std::runtime_error("error");
	//
	ifstream arquivoEntrada(filePath.c_str(), ios::in);

	string linha;
	getline(arquivoEntrada, linha);

	if (linha.compare
		("//     *****   Instrucoes para preenchimento das Curvas Tipicas de Demanda dos Consumidores   *****")
		!= 0)
	{
		throw std::runtime_error("Arquivo inválido!.");
	}

	// verifica se o arquivo foi aberto corretamente
	if (!arquivoEntrada)
	{
		// lança uma exceção
		throw std::runtime_error
			("Não foi possível abrir o arquivo! - Verifique o caminho e nome do arquivo.");
	}

	//
	// le as curvas do arquivo
	//
	string line;

	// fluxoProcessa = 0-nao achou curva, 1-processou curva, 2-processou header, 3-processando csv
	int fluxoProcessa = 0;

	VTCurva *curva = NULL;
	int tipoCurva = 0;
	int countLine = 0;

	TList *curvaList = new TList();

	// le a linha do arquivo
	while (getline(arquivoEntrada, line))
	{
		++countLine;

		AnsiString linha = CorrigeLinha(line.c_str());

		// 1. processa a linha que define a curva
		if ((fluxoProcessa != 1) && (linha.UpperCase().Pos("\"TIPO_CURVA\"")) > 0)
		{
			try
			{
				TJSONObject *JSON = (TJSONObject*)TJSONObject::ParseJSONValue((UnicodeString)linha);
				tipoCurva = StrToInt(JSON->Get("TIPO_CURVA")->JsonValue->Value());
			}
			catch (Exception &e)
			{
				// lança uma exceção
				AnsiString msg =
					("Erro ao processar o arquivo! - Verifique a linha " + IntToStr(countLine) +
					" '" + linha + "'");
				throw std::runtime_error(msg.c_str());
			}

			//cria nova
			curva = CriarCurva(tipoCurva, linha);
			fluxoProcessa = 1;
			continue;
		}

		// 2. processa a linha do header do csv
		if (fluxoProcessa == 1)
		{
			if (!IsHeaderValido(tipoCurva, linha))
			{
				// lança uma exceção
				AnsiString msg =
					("Erro ao processar o arquivo! - Verifique a linha " + IntToStr(countLine) +
					" '" + linha + "'");
				throw std::runtime_error(msg.c_str());
			}

			fluxoProcessa = 2;

			continue;
		}

		// 3. processa csv
		if (((fluxoProcessa == 2) || (fluxoProcessa == 3)) && IsLinhaMedicao(linha))
		{
			AdicionarPonto(tipoCurva, curva, linha);
			fluxoProcessa = 3;
			continue;
		}

		// 4. adiciona a curva na lista de curva
		if ((fluxoProcessa == 3) && !IsLinhaMedicao(linha))
		{

			if (curvaList->IndexOf(curva) == -1)
			{
				curvaList->Add(curva);
			}

		}

	}

	// adiciona a ultima curva processada na lista de curva
	if (curvaList->IndexOf(curva) == -1)
	{
		curvaList->Add(curva);
	}

	//
	// limpa a base(apaga todas as curvas de demanda de consumidor e iluminação pública)
	//
	ExcluirCurvasTipicas();

	//
	// insere as curvas novas
	//
	int countCurvasSalvas = 0;

	for (int i = 0; i < curvaList->Count; i++)
	{

		VTCurva *curva = (VTCurva*)curvaList->Items[i];

		if (curva->Tipo == curvaPFP_MDP)
			curvas->InsereCurvaTipicaCarga(curva);
		else if (curva->Tipo == curvaLD)
			curvas->InsereCurvaTipicaIP(curva);

		try
		{
			apl->SalvaBaseCurvaTipica(curva);
			countCurvasSalvas++;
		}
		catch (...)
		{
			// só para contar corretamente as linhas que foram efetivamente gravadas no banco
		}
	}

	// fecha o arquivo
	arquivoEntrada.close();

	delete curvaList;
	curvaList = NULL;

	return countCurvasSalvas;

}
*/

// -------------------------------------------------------------------------------------------------
int __fastcall TImportaArquivoCurvaTipica::Importar(AnsiString filePath)
{
	//variáveis locais
	AnsiString msg;

	//valida arquivo
	if(! ValidaArquivo(filePath, msg))
	{
		Aviso(msg);
		return 0;
    }
	//importa curvas
	return (Executa(filePath));
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoCurvaTipica::IsHeaderValido(tipoCURVA tipoCurva, AnsiString linha)
{

	if ((tipoCurva == curvaPFP_MDP) && (linha.AnsiCompareIC
		("INICIO;FIM;POTENCIA_ATIVA;FATOR_POTENCIA") == 0))
		return true;

	else if ((tipoCurva == curvaLD) && (linha.AnsiCompareIC("INICIO;FIM;LIGADO") == 0))
		return true;

	return false;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoCurvaTipica::IsLinhaMedicao(AnsiString linha)
{
	// valida se a linha começa com o formato 99:99
	int match = TRegEx::IsMatch(linha, "^\\d{2}:\\d{2}");

	if (match == 1)
		return true;
	else
		return false;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaArquivoCurvaTipica::ValidaArquivo(AnsiString filePath, AnsiString &msg)
{
	//variáveis locais
	TValidaArqCurva *valida = new TValidaArqCurva(apl);

	msg = valida->Executa(filePath);

	delete valida;

	return(msg.IsEmpty());
}

// --------------------------------------------------------------------------------------------- eof
