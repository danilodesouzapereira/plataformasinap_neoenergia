// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormPlanilha.h"
// #include "TFormQReport.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTEqpto.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormPlanilha::TFormPlanilha(TComponent* Owner, TWinControl *parent, VTApl *apl,
	TADOConnection *Connection) : TForm(Owner)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// insere Form em seu Parent
	if (parent)
	{
		Parent = parent;
		Align = alClient;
	}
	// associa objeto TADOquery c/ TADOConnection
	query->Close();
	query->SQL->Clear();
	query->Filtered = false;
	query->Filter = "";
	query->Connection = Connection;
	// desabilita configuração de atributos
	butAtributo->Down = false;
	ActionConfigura->Execute();
	// DVK 2014.12.03
	DBNavigator->ParentBackground = false;
}

// ---------------------------------------------------------------------------
__fastcall TFormPlanilha::~TFormPlanilha(void)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cancela moldura no gráfico
	grafico->Moldura();
	// fecha consulta à base de dados
	query->Close();
	// cancela associação com TADOConnection
	query->Connection = NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionAtributoMoveDownExecute(TObject *Sender)
{
	// variáveis locais
	TColumn *column;

	// proteção: verifica se existe um item selecionado
	if (CheckListBoxField->ItemIndex < 0)
		return;
	// proteção: verifica se o item pode ser movido p/ baixo
	if (CheckListBoxField->ItemIndex == CheckListBoxField->Items->Count - 1)
		return;
	// determina TColumn do DBGrid associado ao item do CLBox
	column = (TColumn*)CheckListBoxField->Items->Objects[CheckListBoxField->ItemIndex];
	if (column == NULL)
		return;
	// move TColumn uma posição à direita
	column->Index++;
	// reinicia ComboBoxField e CheckLisBoxField
	IniciaComboBoxField(CheckListBoxField->Items->Strings[CheckListBoxField->ItemIndex]);
	IniciaCheckListBoxField(CheckListBoxField->Items->Strings[CheckListBoxField->ItemIndex]);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionAtributoMoveUpExecute(TObject *Sender)
{
	// variáveis locais
	TColumn *column;

	// proteção: verifica se existe um item selecionado
	if (CheckListBoxField->ItemIndex < 0)
		return;
	// proteção: verifica se o item pode ser movido p/ cima
	if (CheckListBoxField->ItemIndex == 0)
		return;
	// determina TColumn do DBGrid associado ao item do CLBox
	column = (TColumn*)CheckListBoxField->Items->Objects[CheckListBoxField->ItemIndex];
	if (column == NULL)
		return;
	// move TColumn uma posição à direita
	column->Index--;
	// reinicia ComboBoxField e CheckLisBoxField
	IniciaComboBoxField(CheckListBoxField->Items->Strings[CheckListBoxField->ItemIndex]);
	IniciaCheckListBoxField(CheckListBoxField->Items->Strings[CheckListBoxField->ItemIndex]);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionAtributoSelAllExecute(TObject *Sender)
{
	// variáveis locais
	TColumn *column;

	// seleciona todos os Items do CLBox e os TColumn  do DBGrid
	for (int n = 0; n < CheckListBoxField->Items->Count; n++)
	{
		// atualiza Item do CLBox
		CheckListBoxField->Checked[n] = true;
		// determina TColumn do DBGrid associado ao item do CLBox
		column = (TColumn*)CheckListBoxField->Items->Objects[n];
		// atualiza o DBGrid
		if (column == NULL)
			return;
		// habilita/desabilita TColumn de acordo c/ o Checked do item
		column->Visible = CheckListBoxField->Checked[n];
		// define Field visible ou não
		column->Field->Visible = column->Visible;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionAtributoSelNoneExecute(TObject *Sender)
{
	// variáveis locais
	TColumn *column;

	// seleciona todos os Items do CLBox e os TColumn  do DBGrid
	for (int n = 0; n < CheckListBoxField->Items->Count; n++)
	{
		// atualiza Item do CLBox
		CheckListBoxField->Checked[n] = false;
		// determina TColumn do DBGrid associado ao item do CLBox
		column = (TColumn*)CheckListBoxField->Items->Objects[n];
		// atualiza o DBGrid
		if (column == NULL)
			return;
		// habilita/desabilita TColumn de acordo c/ o Checked do item
		column->Visible = CheckListBoxField->Checked[n];
		// define Field visible ou não
		column->Field->Visible = column->Visible;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionConfiguraExecute(TObject *Sender)
{
	// exibe/esconde GBoxField p/ habilitar/desabilitar atributos
	PageControl->Visible = butAtributo->Down;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormPlanilha::ActionZoomExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	AnsiString obj;

	try
	{ // determina Eqpto
		obj = query->FieldByName("objeto")->AsString;
		#ifdef _WIN64
		__int64 obj_int = StrToInt64(obj);
		#endif

		#ifdef __WIN32__
		int obj_int = StrToInt(obj);
		#endif

		eqpto = (VTEqpto*)obj_int;
		// seleciona área de zoom p/ destacar o Eqpto
		grafico->SelecionaZoom(eqpto);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionLocalizaExecute(TObject *Sender)
{
	// variáveis locais
	TLocateOptions options;

	// proteção: verifica se há um item selecionado no ComboBoxField
	if (cboxField->ItemIndex < 0)
		return;
	// verifica o string a ser localizado está preenchido
	if (redLocaliza->Lines->Strings[0].IsEmpty())
	{
		Aviso("Preencha o valor do atributo a ser localizado");
		return;
	}
	// procura o valor no DataSet
	query->Locate(cboxField->Text, redLocaliza->Lines->Strings[0], options);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionExportaRelatorioExecute(TObject *Sender)
{
	// variáveis locais
	bool sucesso = false;
	VTReport *report;
	AnsiString arq_ext;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		/*
		 if (arq_ext.AnsiCompareIC(".xls") == 0)
		 {
		 report->PlanilhaExcel(SaveDialog->FileName, tab_name, query);
		 }
		 else
		 {
		 report->ArquivoTexto(SaveDialog->FileName, query);
		 }
		 */
		// salva apenas arquivo texto
		sucesso = report->ArquivoTexto(SaveDialog->FileName, query);
		// destrói VTReport
		DLL_DeleteObjReport(report);
		if (sucesso)
			Aviso("O relatório foi exportado corretamente.");
		else
			Aviso("Erro ao exportar relatório.");
	}
	catch (Exception &e)
	{
		Aviso("Erro ao exportar relatório.");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionRedeSelAllExecute(TObject *Sender)
{
	// desabilita evento CheckListBoxRede::OnClickCheck
	CheckListBoxRede->OnClickCheck = NULL;
	// seleciona todos os Items do CLBox e os TColumn  do DBGrid
	for (int n = 0; n < CheckListBoxRede->Items->Count; n++)
	{
		// atualiza Item do CLBox
		CheckListBoxRede->Checked[n] = true;
	}
	// reabilita evento CheckListBoxRede::OnClickCheck
	CheckListBoxRede->OnClickCheck = CheckListBoxRedeClickCheck;
	// força execução daAction associada ao evento CheckListBoxRede::OnClickCheck
	CheckListBoxRede->OnClickCheck(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ActionRedeSelNoneExecute(TObject *Sender)
{
	// desabilita evento CheckListBoxRede::OnClickCheck
	CheckListBoxRede->OnClickCheck = NULL;
	// seleciona todos os Items do CLBox e os TColumn  do DBGrid
	for (int n = 0; n < CheckListBoxRede->Items->Count; n++)
	{
		// atualiza Item do CLBox
		CheckListBoxRede->Checked[n] = false;
	}
	// reabilita evento CheckListBoxRede::OnClickCheck
	CheckListBoxRede->OnClickCheck = CheckListBoxRedeClickCheck;
	// força execução daAction associada ao evento CheckListBoxRede::OnClickCheck
	CheckListBoxRede->OnClickCheck(NULL);
}

/*
 /---------------------------------------------------------------------------
 void __fastcall TFormRelEqpto::Atualiza(void)
 {
 IniciaDBGrid();
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::CheckListBoxFieldClickCheck(TObject *Sender)
{
	// variáveis locais
	TColumn *column;

	// proteção: verifica se há um item selecionado
	if (CheckListBoxField->ItemIndex < 0)
		return;
	// determina TColumn do DBGrid associado ao item do CLBox
	column = (TColumn*)CheckListBoxField->Items->Objects[CheckListBoxField->ItemIndex];
	if (column == NULL)
		return;
	// habilita/desabilita TColumn de acordo c/ o Checked do item
	column->Visible = CheckListBoxField->Checked[CheckListBoxField->ItemIndex];
	// define Field visible ou não
	column->Field->Visible = column->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::CheckListBoxRedeClickCheck(TObject *Sender)
{
	// variáveis locais
	int count;
	AnsiString filtro, clausula_or;

	// verifica se todos os itens estão selecionados
	count = NumeroDeCheckedItems(CheckListBoxRede);
	if (count == 0)
	{ // nenhum item selecionado: cria filtro p/ resultar dataset vazio
		query->Filtered = false;
		query->Filter = "rede=*";
		query->Filtered = true;
	}
	else if (count == CheckListBoxRede->Count)
	{ // todos itens selecionados: desabilita filtro do dataset
		query->Filtered = false;
		query->Filter = "";
	}
	else
	{ // cria filtro p/ o dataset c/ os itens selecionados
		for (int n = 0; n < CheckListBoxRede->Count; n++)
		{
			if (!CheckListBoxRede->Checked[n])
				continue;
			filtro = filtro + clausula_or + "(rede=" + AnsiQuotedStr
				(CheckListBoxRede->Items->Strings[n], '\'') + ")";
			clausula_or = " OR ";
		}
		query->Filtered = false;
		query->Filter = filtro;
		query->Filtered = true;
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormPlanilha::dbgridCellClick(TColumn *Column)
{
	// variáveis locais
	VTEqpto *eqpto;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	AnsiString obj;

	try
	{ // determina Eqpto
		obj = query->FieldByName("objeto")->AsString;
		#ifdef _WIN64
		__int64 obj_int = StrToInt64(obj);
		eqpto = (VTEqpto*)obj_int;
		#endif

		#ifdef __WIN32__
		int obj_int = StrToInt(obj);
		eqpto = (VTEqpto*)obj_int;
		#endif

		// exibe moldura no gráfico
		grafico->Moldura(eqpto);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::dbgridDrawColumnCell(TObject *Sender, const TRect &Rect, int DataCol,
	TColumn *Column, TGridDrawState State)
{
	// variáveis locais
	int x, y;
	AnsiString texto;

	switch (Column->Field->DataType)
	{
	case ftFloat:
		texto = DoubleToStr("%4.3f", Column->Field->AsFloat);
		switch (Column->Alignment)
		{
		case taRightJustify:
			x = Rect.Left + Rect.Width() - dbgrid->Canvas->TextWidth(texto) - 3;
			break;
		case taLeftJustify:
			x = Rect.Left + 3;
			break;
		default: // taCenter:
			x = Rect.Left + ((Rect.Width() - dbgrid->Canvas->TextWidth(texto)) / 2);
			break;
		}
		// proteção p/ que x fique dentro de TRect
		if (x < Rect.Left)
			x = Rect.Left + 3;
		y = Rect.Top + 2;
		dbgrid->Canvas->TextRect(Rect, x, y, texto);
		break;
	default:
		dbgrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::dbgridTitleClick(TColumn *Column)
{
	// variáveis locais
	AnsiString cmd_sort;
	// verifica se houve alteração da coluna p/ ordenação
	if (Column != ordenacao.column)
	{ // redefine Column da ordenção
		ordenacao.column = Column;
	}
	else
	{ // inverte modo de ordenação
		ordenacao.ascendente = !ordenacao.ascendente;
	}
	// redefine ordenação do dataset
	cmd_sort = "[" + Column->FieldName + "]" + ((ordenacao.ascendente) ? " ASC" : " DESC");
	query->Sort = cmd_sort;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::ExibeTabela(AnsiString tab_name)
{
	// salva tab_name
	this->tab_name = tab_name;
	// seleciona todos os registros da tabela indicada
	query->Close();
	query->SQL->Clear();
	query->SQL->Add("SELECT * FROM " + tab_name);
	query->Open();
	// inicia DBGrid, ComboBoxField e CheckListBoxField
	IniciaDBGrid();
	IniciaComboBoxField();
	IniciaCheckListBoxField();
	IniciaCheckListBoxRede();
	// inicia controle de ordenação
	ordenacao.column = NULL;
	ordenacao.ascendente = true;
	// executa ActionAtributoSelAll p/ atualizar DBGrid
	// ActionAtributoSelAll->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::FormClose(TObject *Sender, TCloseAction &Action)
{
	// apenas esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::FormShow(TObject *Sender)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::IniciaComboBoxField(AnsiString sel_txt)
{
	// variáveis locais
	int index;

	// reinicia ComboBoxField
	cboxField->Clear();
	// inicia ComboBoxField c/ TColumns presentes no DBGrid
	for (int n = 0; n < dbgrid->Columns->Count; n++)
	{
		cboxField->Items->AddObject(dbgrid->Columns->Items[n]->FieldName,
			dbgrid->Columns->Items[n]);
	}
	// verifica se há items no ComboBoxField
	if (cboxField->Items->Count == 0)
		return;
	// pré-seleciona item definido
	index = cboxField->Items->IndexOf(sel_txt);
	if (index < 0)
		index = 0;
	cboxField->ItemIndex = index;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::IniciaCheckListBoxField(AnsiString sel_txt)
{
	// variáveis locais
	int index;
	AnsiString field_name;

	// reinicia CheckListBoxField
	CheckListBoxField->Clear();
	// inicia CheckListBoxField c/ TColumns presentes no DBGrid
	for (int n = 0; n < dbgrid->Columns->Count; n++)
	{ // não inclui item p/ o campo "objeto"
		field_name = dbgrid->Columns->Items[n]->FieldName;
		if (field_name.AnsiCompareIC("objeto") == 0)
			continue;
		index = CheckListBoxField->Items->AddObject(dbgrid->Columns->Items[n]->FieldName,
			dbgrid->Columns->Items[n]);
		// marca item como checked se o campo estiver visível
		CheckListBoxField->Checked[index] = dbgrid->Columns->Items[n]->Visible;
	}
	// verifica se há items no CheckListBoxField
	if (CheckListBoxField->Items->Count == 0)
		return;
	// pré-seleciona item definido
	index = CheckListBoxField->Items->IndexOf(sel_txt);
	if (index < 0)
		index = 0;
	CheckListBoxField->ItemIndex = index;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::IniciaCheckListBoxRede(void)
{
	// variáveis locais
	int index;
	AnsiString codigo;

	// verifica se CheckListBoxRede já está preenchido
	if (CheckListBoxRede->Items->Count > 0)
		return;
	// reinicia CheckListBoxRede
	CheckListBoxRede->Clear();
	// preenche CheckListBoxRede c/ os códigos das redes
	for (query->First(); !query->Eof; query->Next())
	{
		codigo = query->FieldByName("rede")->AsString;
		if (CheckListBoxRede->Items->IndexOf(codigo) >= 0)
			continue;
		// insere código da rede em CheckListBoxRede
		index = CheckListBoxRede->Items->Add(codigo);
		CheckListBoxRede->Checked[index] = true;
	}
	// verifica se existem redes
	if (CheckListBoxRede->Items->Count == 0)
	{ // esconde TabSheetRede
		TabSheetRede->TabVisible = false;
	}
	else
	{
		// exibe TabSheetRede
		TabSheetRede->TabVisible = true;
		// ordena
		CheckListBoxRede->Sorted = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlanilha::IniciaDBGrid(void)
{
	// variáveis locais
	int str_width = 80;
	int num_width = 60;
	AnsiString caption;
	TColumn *column;

	// reinicia DBGrid c/ as colunas
	dbgrid->Columns->RebuildColumns();
	// esconde campo Objeto e os campos de ID
	for (int n = 0; n < dbgrid->Columns->Count; n++)
	{
		column = dbgrid->Columns->Items[n];
		caption = column->Field->DisplayName;
		// verifica se FieldName é "objeto"
		if (caption.AnsiCompareIC("objeto") == 0)
		{
			column->Field->Visible = false;
		}
		// verifica se FieldName contem as letras ID
		else if ((caption.AnsiPos("_id") > 0) || (caption.AnsiPos("_ID") > 0))
		{
			column->Field->Visible = false;
		}
		// define column com visible ou não
		column->Visible = column->Field->Visible;
	}
	// acerta alinhamento e largura
	for (int n = 0; n < dbgrid->Columns->Count; n++)
	{
		column = dbgrid->Columns->Items[n];
		switch (column->Field->DataType)
		{
		case ftInteger:
		case ftFloat:
			column->Title->Alignment = taCenter;
			column->Alignment = taRightJustify;
			column->Width = num_width;
			break;
		case ftString:
		case ftWideString:
			column->Width = str_width;
			break;
        default: break;
		}
	}
}

// ---------------------------------------------------------------------------
int __fastcall TFormPlanilha::NumeroDeCheckedItems(TCheckListBox *clbox)
{
	// variáveis locais
	int count = 0;

	// verifica se todos itens do CheckListBox estão checked
	for (int n = 0; n < clbox->Count; n++)
	{
		if (clbox->Checked[n])
			count++;
	}
	return (count);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPlanilha::SelecionaArquivo(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// verifica se o diretório SaveDialog já foi definido
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	return (SaveDialog->Execute());
}

// ---------------------------------------------------------------------------
// eof
