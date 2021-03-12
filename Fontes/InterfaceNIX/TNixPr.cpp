// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixPr.h"
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
VTNixPr* __fastcall NewObjNixPrDefault(VTChave *chave, int index, bool pr_com_corrente)
{
	return (new TNixPr(chave, index, pr_com_corrente));
}

// ---------------------------------------------------------------------------
__fastcall TNixPr::TNixPr(VTChave *chave, int index, bool pr_com_corrente)
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
	strPR.codigo = TraduzCodigo(chave, index);
	strPR.no_inicio = chave->pbarra1->Id;
	strPR.no_fim = chave->pbarra2->Id;
	strPR.i_mod = Abs(i_max_amp);
	strPR.i_ang = Arg(i_max_amp);
	strPR.cons = 0;
	strPR.tipo = 0;
	// strPR.elo       = (chave->Aberta) ? "NA" : "";
	if (chave->Aberta)
	{
		strPR.elo = "NA";
	}
	else
	{ // se a chave for base fusivel...
		if (chave->TipoChave->Tipo == chaveBF)
		{ // salva o elo de acordo com a existencia do ExternId
			if (!(chave->EloFusivel.IsEmpty()))
			{
				strPR.elo = chave->EloFusivel;
				strPR.tipo = 3;
			}
		}
		else
		{
			strPR.elo = "";
		}
	}
}

// ---------------------------------------------------------------------------
__fastcall TNixPr::~TNixPr(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
complex<double>__fastcall TNixPr::CorrenteMaxima(VTChave *chave)
{
	// variáveis locais
	int ind_pat_max;
	int ind_bar = 0;
	double cor, cor_max = 0;
	double ang_120grau_rad = (2. * M_PI) / 3.;
	complex<double>cor_fase;

	// loop para todos patamares
	for (int ind_pat = 0; ind_pat < chave->resflow->Max_pat; ind_pat++)
	{ // determina corrente máxima
		cor = chave->resflow->If_amp_max[ind_bar][ind_pat];
		if ((ind_pat == 0) || (cor > cor_max))
		{
			ind_pat_max = ind_pat;
			cor_max = cor;
		}
	}
	// determina barra da chave para a qual a pot.ativa é positiva
	if (chave->resflow->SentidoP[ind_bar][ind_pat_max] == fluxoNEG)
		ind_bar = 1 - ind_bar;
	// determina corrente da fase A
	cor_fase = chave->resflow->Ia_amp[ind_bar][ind_pat_max];
	if (!IsDoubleZero(Abs(cor_fase)))
	{ // monta conrente máxima com ângulo da corrente da fase A
		return (polar(cor_max, Arg(cor_fase)));
	}
	// determina corrente da fase B
	cor_fase = chave->resflow->Ib_amp[ind_bar][ind_pat_max];
	if (!IsDoubleZero(Abs(cor_fase)))
	{ // monta conrente máxima com ângulo da corrente da fase A
		return (polar(cor_max, Arg(cor_fase) + ang_120grau_rad));
	}
	// determina corrente da fase C
	cor_fase = chave->resflow->Ic_amp[ind_bar][ind_pat_max];
	if (!IsDoubleZero(Abs(cor_fase)))
	{ // monta conrente máxima com ângulo da corrente da fase A
		return (polar(cor_max, Arg(cor_fase) - ang_120grau_rad));
	}
	return (CZero());
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TNixPr::LinhaArqCsv(void)
{
	// variáveis locais
	AnsiString separador = ";";
	AnsiString txt;

	txt = strPR.pr + separador;
	txt += strPR.codigo + separador;
	txt += IntToStr(strPR.no_inicio) + separador;
	txt += IntToStr(strPR.no_fim) + separador;
	// FKM 2016.05.10- mandar 0.0 zoa o nix
	if (IsDoubleZero(strPR.i_mod))
	{
		txt += "0" + separador;
	}
	else
	{
		txt += DoubleToStr("%2.1f", strPR.i_mod) + separador;
	}
	// txt += DoubleToStr("%2.1f", strPR.i_mod) + separador;
	// FKM 2016.05.10- mandar 0.0 zoa o nix
	if (IsDoubleZero(strPR.i_ang))
	{
		txt += "0" + separador;
	}
	else
	{
		txt += DoubleToStr("%2.1f", strPR.i_ang) + separador;
	}
	// txt += DoubleToStr("%2.1f", strPR.i_ang) + separador;
	txt += strPR.elo + separador;
	txt += IntToStr(strPR.cons) + separador;
	txt += IntToStr(strPR.tipo) + separador;
	return (txt);
}

// ---------------------------------------------------------------------------
int __fastcall TNixPr::SupoeTipo(VTChave *chave)
{ // variaveis locais
	AnsiString codigo_lc;
	int tipoChave = -1;
	VTTipoChave *tipo = chave->TipoChave;

	// protecao
	if (!tipo)
		return tipoChave;
	// deixa tudo minusculo
	codigo_lc = LowerCase(tipo->Codigo);

	// procura por disjuntor
	if (codigo_lc.Pos("disjuntor") > 0)
		tipoChave = chaveDJ;
	// procura por religador
	if (codigo_lc.Pos("religador") > 0)
		tipoChave = chaveRA;
	// procura por SECCIONADORA
	if (codigo_lc.Pos("seccio") > 0)
		tipoChave = chaveSEC;
	// procura por fusivel
	if (codigo_lc.Pos("fusivel") > 0)
		tipoChave = chaveBF;
	if (codigo_lc.Pos("fusível") > 0)
		tipoChave = chaveBF;

	return tipoChave;
}

// ---------------------------------------------------------------------------
// AnsiString __fastcall TNixPr::TraduzCodigo(VTChave *chave, int index)
// {
// //variáveis locais
// AnsiString codigo;
//
// //inicia código c/ a parte referente ao index
// codigo.sprintf("%04d", index);
// switch(chave->TipoChave->Tipo)
// {
// case chaveDJ:   codigo = "DJ" + codigo; break;
// case chaveRA:   codigo = "RA" + codigo; break;
// case chaveBF:   codigo = "BF" + codigo; break;
// case chaveSEC:  codigo = "CS" + codigo; break;
// case chaveFACA: codigo = "CF" + codigo; break;
// default:        codigo = "BF" + codigo; break;
// }
// return(codigo);
// }
// ---------------------------------------------------------------------------
// AnsiString __fastcall TNixPr::TraduzCodigo(VTChave *chave, int index)
// {
// //variáveis locais
// AnsiString codigo;
//
// //inicia código c/ a parte referente ao index
// //codigo = IntToStr(index);
// //para facilitar a localização da chave e a unicidade do codigo
// codigo = chave->Codigo + "(id:" + IntToStr(chave->Id) +")";
// switch(chave->TipoChave->Tipo)
// {
// case chaveDJ:   codigo = "DJ-" + codigo; break;
// case chaveRA:   codigo = "RA-" + codigo; break;
// case chaveBF:   codigo = "BF-" + codigo; break;
// case chaveSEC:  codigo = "CS-" + codigo; break;
// case chaveFACA: codigo = "CF-" + codigo; break;
// //default:        codigo = "BF" + codigo; break;
// default:        codigo = "CF-" + codigo; break;   //FKM 2016.02.15 - melhor ser chave faca ai não entra na análise
// }
// //reduz o tamanho do codigo caso tenha ficado grande demais
// if(codigo.Length() > 20)
// {
// codigo = codigo.SetLength(20);
// }
// return(codigo);
// }
// ---------------------------------------------------------------------------
AnsiString __fastcall TNixPr::TraduzCodigo(VTChave *chave, int index)
{
	// variáveis locais
	AnsiString codigo;
	int tipoSuposto;

	// inicia código c/ a parte referente ao index
	// codigo = IntToStr(index);
	// para facilitar a localização da chave e a unicidade do codigo
	codigo = chave->Codigo + "(id:" + IntToStr(chave->Id) + ")";
	switch (chave->TipoChave->Tipo)
	{
	case chaveDJ:
		codigo = "DJ-" + codigo;
		break;
	case chaveRA:
		codigo = "RA-" + codigo;
		break;
	case chaveBF:
		codigo = "BF-" + codigo;
		break;
	case chaveSEC:
		codigo = "CS-" + codigo;
		break;
	case chaveFACA:
		codigo = "CF-" + codigo;
		break;
		// default:        codigo = "BF" + codigo; break;
	default:
		tipoSuposto = SupoeTipo(chave);
		switch (tipoSuposto)
		{
		case chaveDJ:
			codigo = "DJ-" + codigo;
			break;
		case chaveRA:
			codigo = "RA-" + codigo;
			break;
		case chaveBF:
			codigo = "BF-" + codigo;
			break;
		case chaveSEC:
			codigo = "CS-" + codigo;
			break;
		default:
			codigo = "CF-" + codigo;
			break;
		}
	}
	// reduz o tamanho do codigo caso tenha ficado grande demais
	if (codigo.Length() > 20)
	{
		codigo = codigo.SetLength(20);
	}
	return (codigo);
}
// ---------------------------------------------------------------------------
