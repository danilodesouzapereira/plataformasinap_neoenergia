// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixPrNeoEnergia.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTNixPr* __fastcall NewObjNixPrNeoEnergia(VTChave *chave, int index, bool pr_com_corrente)
{
	return (new TNixPrNeoEnergia(chave, index, pr_com_corrente));
}

// ---------------------------------------------------------------------------
__fastcall TNixPrNeoEnergia::TNixPrNeoEnergia(VTChave *chave, int index, bool pr_com_corrente)
	: TNixPr()
{
	// variáveis locais
	complex<double>i_max_amp;

	// determina corrente máxima na chave
	if (pr_com_corrente)
		i_max_amp = CorrenteMaxima(chave);
	else
		i_max_amp = CZero();
	// inicia dados
	strPR.pr = "PR";
	strPR.tipo = 0;
	strPR.codigo = TraduzCodigo(chave, index);
	strPR.no_inicio = chave->pbarra1->Id;
	strPR.no_fim = chave->pbarra2->Id;
	strPR.i_mod = Abs(i_max_amp);
	strPR.i_ang = Arg(i_max_amp);
	strPR.cons = 0;
	// strPR.elo       = (chave->Aberta) ? "NA" : "";
	if (chave->Aberta)
	{
		strPR.elo = "NA";
	}
	else
	{ // se a chave for base fusivel...
		if (chave->TipoChave->Tipo == chaveBF)
		{ // salva o elo de acordo com a existencia do ExternId
			strPR.elo = chave->EloFusivel;
			// depende do tipo de elo
			if (strPR.elo.Pos("K") > 0)
			{
				strPR.tipo = 0;
			}
			else if (strPR.elo.Pos("H") > 0)
			{
				strPR.tipo = 3;
			}
		}
		else
		{
			strPR.elo = "#";
		}
	}
}

// ---------------------------------------------------------------------------
__fastcall TNixPrNeoEnergia::~TNixPrNeoEnergia(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TNixPrNeoEnergia::TraduzCodigo(VTChave *chave, int index)
{
	// variáveis locais
	AnsiString codigo;

	// inicia código c/ a parte referente ao index
	// codigo = IntToStr(index);
	// para facilitar a localização da chave e a unicidade do codigo
	codigo = chave->Codigo + "(id:" + IntToStr(chave->Id) + ")";
	// Todos os tipos serão 0
	strPR.tipo = 0;
	switch (chave->TipoChave->Tipo)
	{
	case chaveDJ:
		codigo = "DJ" + codigo;
		break;
	case chaveRA:
		codigo = "RG" + codigo;
		break;
	case chaveBF:
		codigo = "BF" + codigo;
		break;
	case chaveSEC:
		codigo = "CS" + codigo;
		break;
	case chaveFACA:
		codigo = "CF" + codigo;
		break;
		// default:        codigo = "BF" + codigo; break;
	default:
		codigo = SupoeTipoChave(chave->TipoChave) + codigo;
	}
	// reduz o tamanho do codigo caso tenha ficado grande demais
	if (codigo.Length() > 20)
	{
		codigo = codigo.SetLength(20);
	}
	return (codigo);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TNixPrNeoEnergia::SupoeTipoChave(VTTipoChave *tipo)
{
	// variaveis locais
	AnsiString codigoTipo;
	AnsiString retorno = "IND";

	// protecao
	if (!tipo)
		retorno = "IND";

	// deixa tudo minusculo
	codigoTipo = LowerCase(tipo->Codigo);

	//FR - Chave Fusível Religadora;
	if(codigoTipo.Pos("religador") && (codigoTipo.Pos("fusível") > 0 || codigoTipo.Pos("fusivel") > 0))
	{
		retorno = "FR";
	}

	//SE - Chave Seccionalizadora;
	if(codigoTipo.Pos("seccionalizador") > 0)
	{
		retorno = "SE";
	}

	//RL - Religador Automático;
	if(codigoTipo.Pos("religador") && (codigoTipo.Pos("automático") > 0 || codigoTipo.Pos("automatico") > 0))
	{
		retorno = "RL";
	}

	//CO - Chave a Óleo;
	if(codigoTipo.Pos("óleo") > 0 || codigoTipo.Pos("oleo") > 0)
	{
		retorno = "CO";
	}

	//CL - Chave Lâmina;
	if(codigoTipo.Pos("lâmina") > 0 || codigoTipo.Pos("lamina") > 0)
	{
    	retorno = "CL";
	}

	return retorno;
}

// ---------------------------------------------------------------------------
