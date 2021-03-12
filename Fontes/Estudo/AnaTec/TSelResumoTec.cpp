// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TSelResumoTec.h"
#include "AnaTec\VTResumoTec.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Planejamento\VTAlternativa.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TSelResumoTec::TSelResumoTec(VTApl *apl_owner)
{
	// salva ponteiro
	this->apl = apl_owner;
	// cria lista
	lisALT_SEL = new TList();
	// inicia array
	ano.Length = 0;
}

// ---------------------------------------------------------------------------
__fastcall TSelResumoTec::~TSelResumoTec(void)
{
	// destroi lista sem destruir objetos
	if (lisALT_SEL)
	{
		delete lisALT_SEL;
		lisALT_SEL = NULL;
	}
	// libera dynamic array
	ano.Length = 0;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::AlimentadorSelecionado(AnsiString cod_alim, AnsiString cod_sub)
{
	// compõe nome alimentador_subestação
	AnsiString sub_alim = cod_alim + "_" + cod_sub;
	// localiza no map alimentador da subestação
	if (alimentador.find(sub_alim) != alimentador.end())
		return true;

	return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::AlternativaSelecionada(VTAlternativa *alternativa)
{
	// verifica se alimentador está na lista
	if (lisALT_SEL->IndexOf(alternativa) >= 0)
		return true;

	return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::AnoSelecionado(int ano_sel)
{
	// verifica se o ano está no array
	for (int a = 0; a < ano.Length; a++)
	{
		if (ano[a] == ano_sel)
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::Exporta(AnsiString nome_arq)
{
	// variáveis locais
	AnsiString txt;
	TStringList *linhas;
	VTAlternativa *alternativa;
	VTResumoTec *resumo, *resumo_alim;
	TList *lisRES_ALIM;
	double perda_orig = 1.;
	double perda, perc;

	try
	{ // cria string list
		linhas = new TStringList();
		// insere cabeçalho
		txt = "Alternativa;Ano;Nome SED;Código SED;Nome Alimentador;Código Alimentador;Barras críticas[%];Barras precárias[%];";
		txt += "Barras adequadas[%];Trafos críticos[%];Trafos precários[%];Trafos adequados[%];Trechos críticos[km];Trechos precários[km];";
		txt += "Trechos adequados[km];Energia recebida[MWh/mês];Energia gerada[MWh/mês];Energia consumida[MWh/mês];Perda[MWh/mês];";
		txt += "Delta perda[%];DEC[h/mês];FEC[falha/mês];END[MWh/mês];Compensação[R$/mês]";
		linhas->Add(txt);
		// loop p/ todas Alternativas do Estudo
		for (int n = 0; n < lisALT_SEL->Count; n++)
		{
			alternativa = (VTAlternativa*)lisALT_SEL->Items[n];
			resumo = (VTResumoTec*)alternativa->Apl->GetObject(__classid(VTResumoTec));
			for (int ano_sel = resumo->AnoInicial; ano_sel <= resumo->AnoFinal; ano_sel++)
			{ // verifica se o ano foi selecionado
				if (!AnoSelecionado(ano_sel))
					continue;
				// loop em todos resumos de alimentadores
				lisRES_ALIM = resumo->LisResumoTecRede();
				for (int a = 0; a < lisRES_ALIM->Count; a++)
				{
					resumo_alim = (VTResumoTec*)lisRES_ALIM->Items[a];
					if (!AlimentadorSelecionado(resumo_alim->Alimentador, resumo_alim->Subestacao))
						continue;
					// insere linha
					txt = alternativa->Codigo + ";";
					txt += IntToStr(ano_sel) + ";";
					txt += resumo_alim->Subestacao + ";"; // alterar nome sed
					txt += resumo_alim->CodSubestacao +";"; // alterar cod sed
					txt += resumo_alim->Alimentador + ";";
					txt += resumo_alim->CodAlimentador + ";";
					// barra
					txt += DoubleToStr("%3.2f",
						resumo_alim->PercDiagBarra[ano_sel][diagRUIM]) + ";";
					txt += DoubleToStr("%3.2f",
						resumo_alim->PercDiagBarra[ano_sel][diagREGULAR]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->PercDiagBarra[ano_sel][diagBOM]) + ";";
					// trafo
					txt += DoubleToStr("%3.2f",
						resumo_alim->PercDiagTrafo[ano_sel][diagRUIM]) + ";";
					txt += DoubleToStr("%3.2f",
						resumo_alim->PercDiagTrafo[ano_sel][diagREGULAR]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->PercDiagTrafo[ano_sel][diagBOM]) + ";";
					// trecho
					txt += DoubleToStr("%3.2f", resumo_alim->DiagTrechoKm[ano_sel][diagRUIM]) + ";";
					txt += DoubleToStr("%3.2f",
						resumo_alim->DiagTrechoKm[ano_sel][diagREGULAR]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->DiagTrechoKm[ano_sel][diagBOM]) + ";";
					// balanço
					txt += DoubleToStr("%3.2f", resumo_alim->Entrada_mwhm[ano_sel]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->Gerada_mwhm[ano_sel]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->Entregue_mwhm[ano_sel]) + ";";
					// perda
					perda = resumo_alim->Perda_mwhm[ano_sel];
					txt += DoubleToStr("%3.2f", perda) + ";";
					perc = 0.;
					// verifica se alternativa é rede original
					if (alternativa->Id == 0)
					{ // salva valor original
						SalvaPerdaOriginal(resumo_alim->Alimentador, resumo_alim->Subestacao, ano_sel, perda);
						txt += DoubleToStr("%3.2f", perc) + ";";
					}
					else
					{ // recupera valor original para determinar delta
						perda_orig = PerdaOriginal(resumo_alim->Alimentador, resumo_alim->Subestacao, ano_sel);
						if (!IsDoubleZero(perda_orig))
							perc = ((perda - perda_orig) / perda_orig) * 100.;
						txt += DoubleToStr("%3.2f", perc) + ";";
					}
					// continuidade
					txt += DoubleToStr("%3.2f", resumo_alim->DEC[ano_sel]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->FEC[ano_sel]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->END[ano_sel]) + ";";
					txt += DoubleToStr("%3.2f", resumo_alim->Compensacao[ano_sel]) + ";";
					// insere
					linhas->Add(txt);
				}
			}
		}
		// exporta
		linhas->SaveToFile(nome_arq);
		// destroi linhas
		delete linhas;
	}
	catch (Exception &e)
	{
		if (linhas)
			delete linhas;
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::InsereAlimentador(AnsiString cod_alim, AnsiString cod_sub)
{
	// insere no map composição alimentador_subestacao
	AnsiString sub_alim = cod_alim + "_" + cod_sub;
	if (alimentador.find(sub_alim) != alimentador.end())
		return false;
	alimentador.insert(sub_alim);
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::InsereAlternativa(VTAlternativa *alternativa)
{
	// insere na lista de alternativas
	if (lisALT_SEL->IndexOf(alternativa) >= 0)
		return false;

	lisALT_SEL->Add(alternativa);
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TSelResumoTec::InsereAno(int ano_sel)
{
	for (int a = 0; a < ano.Length; a++)
	{
		if (ano[a] == ano_sel)
			return false;
	}
	// incrementa array e insere ano
	ano.Length++;
	ano[ano.Length - 1] = ano_sel;
	return true;
}

// ---------------------------------------------------------------------------
double __fastcall TSelResumoTec::PerdaOriginal(AnsiString nome_alimentador, AnsiString nome_subestacao, int ano_sel)
{
	// variaveis locais
	AnsiString nome;
	double valor;

	// compoe string
	nome = nome_alimentador + "_" + nome_subestacao + "_" + IntToStr(ano_sel);
	// localiza no map
	if (perda_orig.find(nome) != perda_orig.end())
	{
		valor = perda_orig.find(nome)->second;
		return valor;
	}
	return 0.;
}

// ---------------------------------------------------------------------------
void __fastcall TSelResumoTec::SalvaPerdaOriginal(AnsiString nome_alimentador, AnsiString nome_subestacao, int ano_sel, double perda)
{
	// variaveis locais
	AnsiString nome;

	// compoe string
	nome = nome_alimentador + "_" + nome_subestacao + "_" + IntToStr(ano_sel);
	// insere no map
	perda_orig[nome] = perda;
}

// ---------------------------------------------------------------------------
