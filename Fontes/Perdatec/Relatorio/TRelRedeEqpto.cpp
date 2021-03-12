// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TRelRedeEqpto.h"
#include "..\VTResumo.h"
#include "..\Agregada\VTAgregada.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
__fastcall TRelRedeEqpto::TRelRedeEqpto(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// inicia valor
	fat_mult = 1.;
	// cria listas
	linhas = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TRelRedeEqpto::~TRelRedeEqpto(void)
{
	// destrói lista sem destruir seus objetos
	if (linhas)
	{
		delete linhas;
		linhas = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRelRedeEqpto::DefineFatMult(double fat_mult)
{
	this->fat_mult = fat_mult;
}

// ---------------------------------------------------------------------------
bool __fastcall TRelRedeEqpto::Executa(AnsiString arq_rede, AnsiString title)
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
AnsiString __fastcall TRelRedeEqpto::FormataEnergia(double energia_mwh)
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
AnsiString __fastcall TRelRedeEqpto::FormataPorcentagem(double dividendo, double divisor)
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
bool __fastcall TRelRedeEqpto::InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada)
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
	txt = "Tipo;Código;Rede;Trafo;Cabo(km);Regulador;Capacitor;Cons.A4;Cons.Res.;Cons.Com.;Cons.Ind.;Cons.Rur;Cons.Out.;Cons.Nulo;Energia;";
    linhas->Add(txt);
	// loop em cada resumo, que será uma linha
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		// Resumo de uma rede
		txt = resumo->rede.TipoRede + ";";
		txt += resumo->rede.Codigo + ";";
		txt += IntToStr(resumo->rede.QtdeRede) + ";";
		txt += IntToStr(resumo->rede.QtdeTrafo) + ";";
		txt += DoubleToStr("%4.3f", resumo->rede.QtdeCabo / 1000., ',') + ";";
		txt += IntToStr(resumo->rede.QtdeRegulador) + ";";
		txt += IntToStr(resumo->rede.QtdeCapacitor) + ";";
		txt += IntToStr(resumo->rede.QtdeConsA4) + ";";
		txt += IntToStr(resumo->rede.QtdeConsRes) + ";";
		txt += IntToStr(resumo->rede.QtdeConsCom) + ";";
		txt += IntToStr(resumo->rede.QtdeConsInd) + ";";
		txt += IntToStr(resumo->rede.QtdeConsRur) + ";";
		txt += IntToStr(resumo->rede.QtdeConsOut) + ";";
        txt += IntToStr(resumo->rede.QtdeConsNulo) + ";";
		txt += FormataEnergia(resumo->rede.EnergiaMercado) + ";";

		// insere em linhas
		linhas->Add(txt);
	}

	return true;
}
// ---------------------------------------------------------------------------
