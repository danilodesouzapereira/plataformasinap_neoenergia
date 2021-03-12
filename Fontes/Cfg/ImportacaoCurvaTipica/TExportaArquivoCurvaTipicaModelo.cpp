// --------------------------------------------------------------------------------------------- bof
#pragma hdrstop
#pragma package(smart_init)

// ----------------------------------------------------------------------------------------- include
#include <vcl.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include <..\Apl\VTApl.h>
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\..\DLL_Inc\Funcao.h"

#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>

#include "TAplCfg.h"
#include "TExportaArquivoCurvaTipicaModelo.h"

// --------------------------------------------------------------------------------------- namespace

//using namespace sinapsis::sinap::configuracao;

// ------------------------------------------------------------------------------------------- class
// -------------------------------------------------------------------------------------------------
__fastcall TExportaArquivoCurvaTipicaModelo::TExportaArquivoCurvaTipicaModelo(TAplCfg *apl)
{
	this->apl = apl;
}

// -------------------------------------------------------------------------------------------------
__fastcall TExportaArquivoCurvaTipicaModelo::~TExportaArquivoCurvaTipicaModelo(void)
{
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TExportaArquivoCurvaTipicaModelo::Exportar(AnsiString fileName) throw(runtime_error)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTBD * bd = (VTBD*)apl->GetObject(__classid(VTBD));
	VTGeral * geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	// guarda o arquivo de rede(.mdb) corrente
	AnsiString redeUsuario = path->ArqRede;

	//
	// 1. desconecta da base atual
	//
	bd->Desconecta();

	//
	// 2. limpa as curvas
	//
	curvas->Clear();

	//
	// 3. coloca o RedeEmpresa.mdb como o arquivo de rede atual
	//
	path->ArqRede = path->ArqRedeEmpresa();

	// define uso da base Access
	geral->BaseDado = SNPDBTYPE_ACCESS;
	bd->Conecta();
	apl->LeBaseCurvaTipica();

	//
	// 4. Monta a lista de curvas(demanda de consumidor + Iluminacao publica)
	//
	TList *curvaConsumidorList = curvas->ListaCurvaTipicaCarga();
	TList *curvaIluminacaoPublicaList = curvas->ListaCurvaTipicaIP();

	//
	// 5. grava as informacoes no arquivo de saida
	//

	// cria o arquivo de saida
	ofstream arquivoSaida(fileName.c_str(), ios::out);
	if (!arquivoSaida.is_open())
	{
		/** TODO: lançar exception. */
		Erro("Não foi possível escrever no arquivo! - Verifique o caminho e nome do arquivo.");
		return false;
	}

	// secao: [Curvas Tipica de Demanda dos Consumidores]

	// texto fixo
	arquivoSaida << GetTextoFixoCurvaConsumidor().c_str();
	// csv
	arquivoSaida << GetCurvaConsumidor(curvaConsumidorList).c_str();

	// secao: [Curvas de Iluminacao Publica]

	// texto fixo
	arquivoSaida << GetTextoFixoCurvaIluminacaoPublica().c_str();
	// csv
	arquivoSaida << GetCurvaIluminacaoPublica(curvaIluminacaoPublicaList).c_str();

	//
	// 6. fecha o arquivo
	//
	arquivoSaida.close(); // TODO exception

	//
	// 5. volta a base corrente para a base(rede) que o usuario estava utilizando
	//
	bd->Desconecta();
	path->ArqRede = redeUsuario;
	bd->Conecta();
	apl->LeBaseCurvaTipica();

	// delete arquivoSaida;

	return true;

}

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TExportaArquivoCurvaTipicaModelo::GetCurvaConsumidor
	(TList *curvaConsumidorList)
{
	/*
	 INICIO;FIM;POTENCIA_ATIVA;FATOR_POTENCIA
	 00:00;01:00;129.452;0.920
	 ...
	 23:00;24:00;114.364;0.920
	 */

	ostringstream textoArquivoSaida;

	AnsiString txt = "";
	strHM hm_ini, hm_fim;

	double valor[nvCURVA_PFP_MDP];

	VTCurva* curva;
	for (int i = 0; i < curvaConsumidorList->Count; i++)
	{
		curva = (VTCurva*)curvaConsumidorList->Items[i];

		// atributos para definicao da curva
		int tipoCurva = curvaPFP_MDP;
		string nomeCurva = curva->Codigo.c_str();
		int classe = curva->Classe->Tag;
		int medicao = curva->DiaMedicao;
		int numeroPontos = curva->NumeroPontos();
		AnsiString limiteInferior = DoubleToStr("%4.3f", curva->LimiteInferior);
		AnsiString limiteSuperior = DoubleToStr("%4.3f", curva->LimiteSuperior);

		// definicao da curva - JSON
		// {"TIPO_CURVA": 5, "NOME":"Residencial", "CLASSE":1, "MEDICAO":1, "PONTOS":24,
		// "FAIXA_CONSUMO_INFERIOR":0.0, "FAIXA_CONSUMO_SUPERIOR":0.0}
		textoArquivoSaida << "{\"TIPO_CURVA\": " << tipoCurva << ", " << "\"NOME\":\"" <<
			nomeCurva << "\", " << "\"CLASSE\":" << classe << ", " << "\"MEDICAO\":" << medicao <<
			", " << "\"PONTOS\":" << numeroPontos << ", " << "\"FAIXA_CONSUMO_INFERIOR\":" <<
			limiteInferior.c_str() << ", " << "\"FAIXA_CONSUMO_SUPERIOR\":" << limiteSuperior.c_str
			() << "}" << endl;

		// header do CSV - CSV
		// INICIO;FIM;POTENCIA_ATIVA;FATOR_POTENCIA
		textoArquivoSaida << "INICIO;FIM;POTENCIA_ATIVA;FATOR_POTENCIA" << endl;

		// valores(pontos) da curva - CSV
		for (int np = 0, nr = 1; np < numeroPontos; np++, nr++)
		{ // obtém ponto da curva
			curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));

			// preenche hora inicial
			txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
			textoArquivoSaida << txt.c_str() << ";";

			// preenche hora final
			txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
			textoArquivoSaida << txt.c_str() << ";";

			// preenche valores de P e Q
			textoArquivoSaida << DoubleToStr("%4.3f", valor[0]).c_str() << ";";
			textoArquivoSaida << DoubleToStr("%4.3f", valor[1]).c_str() << endl;

		}

		textoArquivoSaida << endl;

	}

	return textoArquivoSaida.str().c_str();
}

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TExportaArquivoCurvaTipicaModelo::GetCurvaIluminacaoPublica
	(TList *curvaIluminacaoPublicaList)
{
	/*
	 INICIO;FIM;LIGADO
	 00:00;01:00;1.000
	 ...
	 23:00;24:00;1.000
	 */

	ostringstream textoArquivoSaida;

	AnsiString txt = "";
	strHM hm_ini, hm_fim;

	double valor[nvCURVA_LD];

	VTCurva* curva;
	for (int i = 0; i < curvaIluminacaoPublicaList->Count; i++)
	{
		curva = (VTCurva*)curvaIluminacaoPublicaList->Items[i];

		// atributos para definicao da curva
		int tipoCurva = curvaLD;
		string nomeCurva = curva->Codigo.c_str();
		int numeroPontos = curva->NumeroPontos();

		// definicao da curva - JSON
		// {"TIPO_CURVA": 6, "NOME":"IP", "PONTOS":24}
		textoArquivoSaida << "{\"TIPO_CURVA\": " << tipoCurva << ", " << "\"NOME\":\"" <<
			nomeCurva << "\", " << "\"PONTOS\":" << numeroPontos << "}" << endl;

		// header do CSV - CSV
		// INICIO;FIM;LIGADO
		textoArquivoSaida << "INICIO;FIM;LIGADO" << endl;

		// valores(pontos) da curva - CSV
		for (int np = 0, nr = 1; np < numeroPontos; np++, nr++)
		{ // obtém ponto da curva
			curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));

			// preenche hora inicial
			txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
			textoArquivoSaida << txt.c_str() << ";";

			// preenche hora final
			txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
			textoArquivoSaida << txt.c_str() << ";";

			// preenche valor
			textoArquivoSaida << DoubleToStr("%4.3f", valor[0]).c_str() << endl;

		}

	}

	return textoArquivoSaida.str().c_str();

}

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TExportaArquivoCurvaTipicaModelo::GetTextoFixoCurvaConsumidor()
{
	/*
	 //     *****   Instrucoes para preenchimento das Curvas Tipica de Demanda dos Consumidores   *****
	 //TIPO_CURVA: Numero '5', valor fixo
	 //NOME: Nome ou descricao da curva
	 //CLASSE: Numero entre[1,2,3,4,5,7,9], onde: 0-Residencial, 1-Comercial, 2-Industrial, 3-Rural, 4-Outros, 6-A4, 8-A2
	 //MEDICAO: Numero entre[1,7,8,9], onde: 1-Domingo, 7-Sabado, 8-Dia util, 9-Semana(media)
	 //PONTOS: Quantidade de pontos medidos
	 //FAIXA_CONSUMO_INFERIOR: Numero real[ex. 0.0] ou -1 [indica que nao ha limite inferior para a faixa (Ex.: se o limite superior for 100kWh/mes, indica consumo menor que 100kWh/mes)]
	 //FAIXA_CONSUMO_SUPERIOR: Numero real[ex. 100.0] ou -1 [indica que nao ha limite superior para a faixa (Ex.: se o limite inferior for 100kWh/mes, indica consumo maior que 100kWh/mes)]
	 //CSV: INICIO(hora, HH:MM),FIM(hora, HH:MM),POTENCIA_ATIVA(numero real, 9.999),FATOR_POTENCIA(numero real, 9.999)


	 [Curvas Tipica de Demanda dos Consumidores]
	 */

	ostringstream textoArquivoSaida;

	textoArquivoSaida <<
		"//     *****   Instrucoes para preenchimento das Curvas Tipicas de Demanda dos " <<
		"Consumidores   *****" << endl << "//TIPO_CURVA: Numero '" << curvaPFP_MDP <<
		"', valor fixo" << endl << "//NOME: Nome ou descricao da curva" << endl <<
		"//CLASSE: Numero entre[1,2,3,4,5,7,9], onde: 0-Residencial, 1-Comercial, 2-Industrial, " <<
		"3-Rural, 4-Outros, 6-A4, 8-A2 " << endl <<
		"//MEDICAO: Numero entre[1,7,8,9], onde: 1-Domingo, 7-Sabado, 8-Dia util, 9-Semana(media)" <<
		endl << "//PONTOS: Numero inteiro, quantidade de pontos medidos" << endl <<
		"//FAIXA_CONSUMO_INFERIOR: Numero real[ex. 0.0] ou -1 [indica que nao ha limite " <<
		"inferior para a " <<
		"faixa (Ex.: se o limite superior for 100kWh/mes, indica consumo menor que 100kWh/mes)]" <<
		endl << "//FAIXA_CONSUMO_SUPERIOR: Numero real[ex. 100.0] ou -1 [indica que nao ha " <<
		"limite superior para a " <<
		"faixa (Ex.: se o limite inferior for 100kWh/mes, indica consumo maior que 100kWh/mes)]" <<
		endl << "//CSV: INICIO(hora, HH:MM),FIM(hora, HH:MM),POTENCIA_ATIVA(numero real, 9.999)," <<
		"FATOR_POTENCIA" << "(numero real, 9.999)" << endl;

	textoArquivoSaida << endl << endl;

	textoArquivoSaida << "[Curvas Tipicas de Demanda dos Consumidores]" << endl;

	return textoArquivoSaida.str().c_str();

}

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TExportaArquivoCurvaTipicaModelo::GetTextoFixoCurvaIluminacaoPublica()
{
	/*
	 //     *****   Instrucoes para preenchimento das Curvas de Iluminacao Publica   *****
	 //TIPO_CURVA: Numero '6', valor fixo
	 //NOME: Nome ou descricao da curva
	 //PONTOS: Quantidade de pontos medidos
	 //CSV: INICIO(hora, HH:MM), FIM(hora, HH:MM), LIGADO(numero real, entre 0.000 e 1.000)

	 [Curvas Tipicas de Iluminacao Publica]

	 */

	ostringstream textoArquivoSaida;

	textoArquivoSaida << endl <<
		"//     *****   Instrucoes para preenchimento das Curvas de Iluminacao Publica   *****" <<
		endl << "//TIPO_CURVA: Numero '" << curvaLD << "', valor fixo" << endl <<
		"//NOME: Nome ou descricao da curva" << endl <<
		"//PONTOS: Numero inteiro, quantidade de pontos medidos" << endl <<
		"//CSV: INICIO(hora, HH:MM), FIM(hora, HH:MM), LIGADO(numero real, entre 0.000 e 1.000)" <<
		endl;

	textoArquivoSaida << endl << endl;

	textoArquivoSaida << "[Curvas Tipicas de Iluminacao Publica]" << endl;

	return textoArquivoSaida.str().c_str();

}
// --------------------------------------------------------------------------------------------- eof
