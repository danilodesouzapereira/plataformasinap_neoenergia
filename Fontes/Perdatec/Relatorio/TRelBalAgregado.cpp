// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TRelBalAgregado.h"
#include "..\VTResumo.h"
#include "..\VTResumos.h"
#include "..\Agregada\VTAgregada.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
__fastcall TRelBalAgregado::TRelBalAgregado(VTApl *apl)
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
__fastcall TRelBalAgregado::~TRelBalAgregado(void)
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
bool __fastcall TRelBalAgregado::AgregaResumo(VTResumo *resumo)
{
	// variaveis locais
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTNode *node;

	// verifica se a Rede pertence a um grupo de Redes
	if ((node = arvore->ExisteNode(resumo)) == NULL)
		return false;
	if (node->LisObjectNode()->Count > 1)
		return false;
	// monta lista com Resumo e seus Resumos a jusante
	lisTMP->Clear();
	lisTMP->Add(resumo);
	resumos->LisResumoJusante((VTNode*)(resumo->object), lisTMP);
	// monta Agregada com o Resumo e resumos a jusante
	resumo_agregado->ZeraValores();
	resumo_agregado->Agrega(lisTMP);
	// zera energia recebida e entregue do Resumo agreagado
	resumo_agregado->balanco.EnergiaEntrada = 0.;
	resumo_agregado->balanco.EnergiaEntregueExt = 0.;
	resumo_agregado->balanco.EnergiaEntregueInt = 0.;
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TRelBalAgregado::DefineFatMult(double fat_mult)
{
	this->fat_mult = fat_mult;
}

// ---------------------------------------------------------------------------
bool __fastcall TRelBalAgregado::Executa(AnsiString arq_rede, AnsiString title)
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
AnsiString __fastcall TRelBalAgregado::FormataEnergia(double energia_mwh)
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
AnsiString __fastcall TRelBalAgregado::FormataPorcentagem(double dividendo, double divisor)
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
bool __fastcall TRelBalAgregado::InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada)
{
	// variáveis locais
	double injetada, fornecida, entregue, consumida;
	// TListItem *Item;
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

		//agrega
		if(!AgregaResumo(resumo))
		{
			continue;
		}
		resumo = resumo_agregado;

		txt += FormataEnergia(resumo->balanco.EnergiaPerdaTotal) + ";";
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, injetada) + ";";
		txt += FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, resumo_total->balanco.EnergiaEntrada) + ";";

		consumida = resumo->balanco.EnergiaCarga;
		entregue = 0.;

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
