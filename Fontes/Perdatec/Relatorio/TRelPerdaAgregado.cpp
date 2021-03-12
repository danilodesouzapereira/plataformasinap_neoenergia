// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TRelPerdaAgregado.h"
#include "..\VTResumo.h"
#include "..\VTResumos.h"
#include "..\Agregada\VTAgregada.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
__fastcall TRelPerdaAgregado::TRelPerdaAgregado(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// inicia valor
	fat_mult = 1.;
	// cria listas
	linhas = new TStringList();
	lisTMP = new TList();
	// cria objeto Resumo para fazer agregação
	resumo_agregado = NewObjResumo();
	resumo_agregado->tipo_resumo = resumoAGREGADO;
}

// ---------------------------------------------------------------------------
__fastcall TRelPerdaAgregado::~TRelPerdaAgregado(void)
{
	// destrói lista sem destruir seus objetos
	if (linhas)
	{
		delete linhas;
		linhas = NULL;
	}
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	DestroiTObject(resumo_agregado);
}

// ---------------------------------------------------------------------------
void __fastcall TRelPerdaAgregado::AgregaResumo(VTResumo *resumo)
{
	// variaveis locais
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	// monta lista com Resumo e seus Resumos a jusante
	lisTMP->Clear();
	lisTMP->Add(resumo);
	resumos->LisResumoJusante((VTNode*)(resumo->object), lisTMP);
	// monta Agregada com o Resumo e resumos a jusante
	resumo_agregado->ZeraValores();
	resumo_agregado->Agrega(lisTMP);
}

// ---------------------------------------------------------------------------
void __fastcall TRelPerdaAgregado::DefineFatMult(double fat_mult)
{
	this->fat_mult = fat_mult;
}

// ---------------------------------------------------------------------------
bool __fastcall TRelPerdaAgregado::Executa(AnsiString arq_rede, AnsiString title)
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
AnsiString __fastcall TRelPerdaAgregado::FormataEnergia(double energia_mwh)
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
AnsiString __fastcall TRelPerdaAgregado::FormataPorcentagem(double dividendo, double divisor)
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
bool __fastcall TRelPerdaAgregado::InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada)
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
	txt = "Tipo;Código;Energia Injetada;Perda Própria;Cabo;RT+BC;Trafo(cu);Trafo(fe);Ramal Ligação;Medidor;Outros;Total;% (PT*);IPTS (%);PPT (%);";
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
        //propria
		txt += FormataEnergia(resumo->balanco.EnergiaPerdaTotal) + ";";

		//agrega
		AgregaResumo(resumo);
		resumo = resumo_agregado;
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
