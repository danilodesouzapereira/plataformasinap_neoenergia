// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TRelRedePerda.h"
#include "..\VTResumo.h"
#include "..\Agregada\VTAgregada.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/7975908
__fastcall TRelRedePerda::TRelRedePerda(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// inicia valor
	fat_mult = 1.;
	// cria listas
	linhas = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TRelRedePerda::~TRelRedePerda(void)
{
	// destrói lista sem destruir seus objetos
	if (linhas)
	{
		delete linhas;
		linhas = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRelRedePerda::DefineFatMult(double fat_mult)
{
	this->fat_mult = fat_mult;
}

// ---------------------------------------------------------------------------
bool __fastcall TRelRedePerda::Executa(AnsiString arq_rede, AnsiString title)
{
	// variáveis locais
	AnsiString texto;
	TList *LisStringList;
	TStringList *StrList;
	FILE *stream;

	// abre o arquivo p/ gravação
	stream = fopen(arq_rede.c_str(), "a+");
	if (stream == NULL)
		return (false);
	// insere 2 linhas de separação
	fprintf(stream, "\n\n");
	// verifica se existe título
	if (!title.IsEmpty())
	{
		fprintf(stream, "%s", title.c_str());
		fprintf(stream, "\n");
	}
	// loop p/ todos registros
	for (int nl = 0; nl < linhas->Count; nl++)
	{
		texto = linhas->Strings[nl];
		fprintf(stream, "%s", texto.c_str());
		fprintf(stream, "\n");
	}
	// fecha arquivo
	fclose(stream);
	return true;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRedePerda::FormataEnergia(double energia_mwh)
{
	// variáveis locais
	AnsiString txt;
	struct
	{
		char DecimalSeparator;
		char ThousandSeparator;
	} original;

	// salva separadores originais
	original.DecimalSeparator = FormatSettings.DecimalSeparator;
	original.ThousandSeparator = FormatSettings.ThousandSeparator;
	// redefine separador decimal
	FormatSettings.ThousandSeparator = '.';
	FormatSettings.DecimalSeparator = ',';
	txt = AnsiString::FormatFloat("#,##0.000", energia_mwh * fat_mult);
	// restaura separadores originais
	FormatSettings.DecimalSeparator = original.DecimalSeparator;
	FormatSettings.ThousandSeparator = original.ThousandSeparator;

	return (txt);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRedePerda::FormataPorcentagem(double dividendo, double divisor)
{
	// variáveis locais
	double valor;
	AnsiString txt;
	struct
	{
		char DecimalSeparator;
		char ThousandSeparator;
	} original;

	// calcula valor
	if (IsDoubleZero(divisor))
		valor = 0.;
	else
		valor = dividendo / divisor;
	// salva separadores originais
	original.DecimalSeparator = FormatSettings.DecimalSeparator;
	original.ThousandSeparator = FormatSettings.ThousandSeparator;
	// redefine separador decimal
	FormatSettings.ThousandSeparator = '.';
	FormatSettings.DecimalSeparator = ',';
	txt = AnsiString::FormatFloat("#,##0.000", valor * 100.);
	// restaura separadores originais
	FormatSettings.DecimalSeparator = original.DecimalSeparator;
	FormatSettings.ThousandSeparator = original.ThousandSeparator;

	return (txt);
}

// ---------------------------------------------------------------------------
bool __fastcall TRelRedePerda::InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada)
{
	// variáveis locais
	double injetada;
	//TListItem *Item;
	VTResumo *resumo;
	VTResumo *resumo_total = agregada->ResumoTotal();
	AnsiString txt;

	// limpa linhas
	linhas->Clear();
	//insere cabeçalho
	txt = "Tipo;Código;Energia Injetada;Cabo;RT+BC;Trafo(cu);Trafo(fe);Ramal Ligação;Medidor;Outros;Total;% (PT*);IPTS (%);PPT (%);";
    linhas->Add(txt);
	// loop em cada resumo, que será uma linha
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		// Resumo de uma rede
		txt = resumo->rede.TipoRede + ";";
		txt += resumo->rede.Codigo + ";";
		// energia de injetada
		// injetada = resumo->balanco.EnergiaEntrada + resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt;
		injetada = resumo->balanco.EnergiaEntrada;
		// + resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt;
		txt += FormataEnergia(injetada) + ";";
		// perdas calculadas pelo fluxo
		txt += FormataEnergia(resumo->perda.PerdaCabo) + ";";
		txt += FormataEnergia(resumo->perda.PerdaRegulador + resumo->perda.PerdaCapacitor) + ";";
		txt += FormataEnergia(resumo->perda.PerdaTrafoCobre) + ";";
		txt += FormataEnergia(resumo->perda.PerdaTrafoFerro) + ";";
		txt += FormataEnergia(resumo->perda.PerdaRamal) + ";";
		txt += FormataEnergia(resumo->perda.PerdaMedidor) + ";";
		txt += FormataEnergia(resumo->perda.PerdaOutros) + ";";
		txt += FormataEnergia(resumo->balanco.EnergiaPerdaTotal) + ";";
		// (perda da rede)/(perda total)
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal,
			resumo_total->balanco.EnergiaPerdaTotal) + ";";
		// IPTS: (perda da rede) / (energia injetada na rede)
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, injetada) + ";";
		// PPT: (perda total) / (energia injetada total)
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal,
			resumo_total->balanco.EnergiaEntrada) + ";";

		// insere em linhas
		linhas->Add(txt);
	}

	return true;
}
// ---------------------------------------------------------------------------
