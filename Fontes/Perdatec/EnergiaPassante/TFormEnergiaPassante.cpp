// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <System.DateUtils.hpp>
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
#include "VTResumo.h"
#include "Agregada\VTAgregada.h"
#include "TFormEnergiaPassante.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEnergiaPassante *FormEnergiaPassante;

// ---------------------------------------------------------------------------
__fastcall TFormEnergiaPassante::TFormEnergiaPassante(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	// define Parent
	if (parent)
		Parent = parent;
	// salva ponteiro
	this->apl = apl_owner;
	// inicia controle da ordena��o dos LView
	LViewSort.column = NULL;
	LViewSort.ascendente = true;
	// inicia fat_mult
	fat_mult = 1.;

}

// ---------------------------------------------------------------------------
__fastcall TFormEnergiaPassante::~TFormEnergiaPassante(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destr�i objetos
	if (agregada)
	{
		delete agregada;
		agregada = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEnergiaPassante::MontaRelatorioEnergiaPassante(TDateTime date)
{
	// vari�veis locais
	TListItem *Item;
	TListView *LView = LViewEnergiaPassante;
	int dias_mes = 0;
	int mes, ano;
	AnsiString caption ="";

	dias_mes = DaysInMonth(date);
	mes = MonthOf(date);
	ano = YearOf(date);
	//acerta o nome da coluna
	caption.sprintf("Energia Passante [MWh/m�s (%02d/%d)]",mes,ano);
	LViewEnergiaPassante->Column[1]->Caption = caption;
	// cria objeto Agregada para montar Balan�o agregado por segmento
	if (agregada != NULL)
	{
		delete agregada;
		agregada = NULL;
	}
	agregada = NewObjAgregada(apl);
	agregada->Inicia();
	VTResumo *total = agregada->ResumoTotal();

	// monta LViewResumo
	LView->Items->Clear();
	// prepara preenchimento
	LView->Items->BeginUpdate();

	// A1 -> A2
	Item = LView->Items->Add();
	Item->Caption = "A1 -> A2";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A1_A2 * dias_mes));
	// A1 -> A3
	Item = LView->Items->Add();
	Item->Caption = "A1 -> A3";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A1_A3 * dias_mes));
	// A1 -> A3a
	Item = LView->Items->Add();
	Item->Caption = "A1 -> A3a";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A1_A3A * dias_mes));
	// A1 -> A4
	Item = LView->Items->Add();
	Item->Caption = "A1 -> A4";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A1_A4 * dias_mes));
	// A2 -> A3
	Item = LView->Items->Add();
	Item->Caption = "A2 -> A3";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A2_A3 * dias_mes));
	// A2 -> A3a
	Item = LView->Items->Add();
	Item->Caption = "A2 -> A3a";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A2_A3A * dias_mes));
	// A2 -> A4
	Item = LView->Items->Add();
	Item->Caption = "A2 -> A4";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A2_A4 * dias_mes));
	// A3 -> A2
	Item = LView->Items->Add();
	Item->Caption = "A3 -> A2";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A3_A2 * dias_mes));
	// A3 -> A3a
	Item = LView->Items->Add();
	Item->Caption = "A3 -> A3a";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A3_A3A * dias_mes));
	// A3 -> A4
	Item = LView->Items->Add();
	Item->Caption = "A3 -> A4";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A3_A4 * dias_mes));
	// A3a -> A2
	Item = LView->Items->Add();
	Item->Caption = "A3a -> A2";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A3A_A4 * dias_mes));
	// A3a -> A4
	Item = LView->Items->Add();
	Item->Caption = "A3a -> A4";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A3A_A4 * dias_mes));
	// A3a -> B
	Item = LView->Items->Add();
	Item->Caption = "A3a -> B";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A3A_B * dias_mes));
	// A4 -> A2
	Item = LView->Items->Add();
	Item->Caption = "A4 -> A2";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A4_A2 * dias_mes));
	// A4 -> A3
	Item = LView->Items->Add();
	Item->Caption = "A4 -> A3";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A4_A3 * dias_mes));
	// A4 -> A3a
	Item = LView->Items->Add();
	Item->Caption = "A4 -> A3a";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A4_A3A * dias_mes));
	// A4 -> B
	Item = LView->Items->Add();
	Item->Caption = "A4 -> B";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.A4_B * dias_mes));
	// B -> A3A
	Item = LView->Items->Add();
	Item->Caption = "B -> A3a";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.B_A3A * dias_mes));
	// B -> A4
	Item = LView->Items->Add();
	Item->Caption = "B -> A4";
	Item->SubItems->Add(FormataEnergia(total->energiaPassante.B_A4 * dias_mes));

	LView->Items->EndUpdate();

	if (agregada)
	{
		delete agregada;
		agregada = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEnergiaPassante::ActionSalvaRelatorioExecute(TObject *Sender)
{
	// vari�veis locais
	bool append = false;
	VTReport *report;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // verifica se o arquivo j� existe
		// define diret�rio inicial
		if (SaveDialog->InitialDir.IsEmpty())
			SaveDialog->InitialDir = path->DirExporta();
		// exibe OpenTextFileDialog
		if (!SaveDialog->Execute())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// exporta ListView
		report->ArquivoTexto(SaveDialog->FileName, LViewEnergiaPassante, Caption, append);
		// destr�i objeto
		delete report;
	}
	catch (Exception &e)
	{
		Erro("N�o foi poss�vel gravar o arquivo " + ExtractFileName(SaveDialog->FileName));
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormEnergiaPassante::ActionFecharExecute(TObject *Sender)
{
	Close();
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEnergiaPassante::FormataEnergia(double energia_mwh)
{
	// vari�veis locais
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

void __fastcall TFormEnergiaPassante::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
	// desabilita tratamento deste evento novamente
	OnShow = NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEnergiaPassante::FormIniciaPosicao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form canto superior esquerdo
	Top = 0;
	Left = 0; ;
}
// ---------------------------------------------------------------------------

void __fastcall TFormEnergiaPassante::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}
// ---------------------------------------------------------------------------

void __fastcall TFormEnergiaPassante::LViewEnergiaPassanteColumnClick(TObject *Sender,
	TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (LViewSort.column == Column)
	{ // inverte ordem
		LViewSort.ascendente = !LViewSort.ascendente;
	}
	else
	{ // redefine �ndice da coluna usada p/ ordena��o
		LViewSort.column = Column;
		LViewSort.ascendente = true;
	}
	// for�a reordena��o
	LViewEnergiaPassante->AlphaSort();
}
// ---------------------------------------------------------------------------

void __fastcall TFormEnergiaPassante::LViewEnergiaPassanteCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)

{
	// vari�veis locais
	int index = LViewSort.column->Index - 1;
	double valor1, valor2;

	// verifica Coluna sendo ordenada
	if (LViewSort.column->Index == 0)
	{ // compara Caption
		Compare = CompareText(Item1->Caption, Item2->Caption);
	}
	// todas as demais colunas s�o num�ricas
	else
	{ // compara��o de colunas com valor num�rico
		valor1 = StrToDouble(Item1->SubItems->Strings[index]);
		valor2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (valor1 < valor2)
			Compare = -1;
		else if (valor1 > valor2)
			Compare = 1;
		else
			Compare = 0;
	}
	// verifica se ordem ascendente/descendente
	if (!LViewSort.ascendente)
		Compare = -Compare;
}
// ---------------------------------------------------------------------------
