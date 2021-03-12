// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TRelRedeBalanco.h"
#include "..\VTResumo.h"
#include "..\Agregada\VTAgregada.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
__fastcall TRelRedeBalanco::TRelRedeBalanco(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// inicia valor
	fat_mult = 1.;
	// cria listas
	linhas = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TRelRedeBalanco::~TRelRedeBalanco(void)
{
	// destrói lista sem destruir seus objetos
	if (linhas)
	{
		delete linhas;
		linhas = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRelRedeBalanco::DefineFatMult(double fat_mult)
{
	this->fat_mult = fat_mult;
}

// ---------------------------------------------------------------------------
bool __fastcall TRelRedeBalanco::Executa(AnsiString arq_rede, AnsiString title)
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
AnsiString __fastcall TRelRedeBalanco::FormataEnergia(double energia_mwh)
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
AnsiString __fastcall TRelRedeBalanco::FormataPorcentagem(double dividendo, double divisor)
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
bool __fastcall TRelRedeBalanco::InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada)
{
	// variáveis locais
	double injetada, consumida, fornecida, entregue;
	//TListItem *Item;
	VTResumo *resumo;
	VTResumo *resumo_total = agregada->ResumoTotal();
	AnsiString txt;

	// limpa linhas
	linhas->Clear();
	//insere cabeçalho
	txt = "Tipo;Código;Inj.Externa(1);Inj.Montante(2);Inj.GD(3);Injetada(1+2+3);% (EI*);Perda;IPTS (%);PPT (%);For.Consumidor(4);For.Jusante(5);For.Externa(6);Fornecida(4+5+6);% (EI*)";
	linhas->Add(txt);
	// loop em cada resumo, que será uma linha
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		// Resumo de uma rede
		txt = resumo->rede.TipoRede + ";";
		txt += resumo->rede.Codigo + ";";

		injetada = resumo->balanco.EnergiaEntrada;

		txt += FormataEnergia(resumo->balanco.EnergiaSuprimento) + ";";
		txt += FormataEnergia(resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt) + ";";
		txt += FormataEnergia(resumo->balanco.EnergiaGerador) + ";";
		txt += FormataEnergia(injetada) + ";";
		txt += FormataPorcentagem(injetada,	resumo_total->balanco.EnergiaEntrada) + ";";

		txt += FormataEnergia(resumo->balanco.EnergiaPerdaTotal) + ";";
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, injetada) + ";";
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, resumo_total->balanco.EnergiaEntrada) + ";";

		consumida = resumo->balanco.EnergiaCarga;
		entregue = resumo->balanco.EnergiaEntregueExt + resumo->balanco.EnergiaEntregueInt;

		txt += FormataEnergia(consumida) + ";";
		txt += FormataEnergia(entregue) + ";";
        txt += FormataEnergia(0.) + ";";

		fornecida = consumida + entregue;
		txt += FormataEnergia(fornecida) + ";";
		txt += FormataPorcentagem(fornecida, resumo_total->balanco.EnergiaEntrada) + ";";

		// insere em linhas
		linhas->Add(txt);
	}

	return true;
}
// ---------------------------------------------------------------------------
