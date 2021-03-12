// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormObra.h"
// #include "VTCustos.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\Dll_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormObra::TFormObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra) : TForm(Owner)
{
	// salva objetos
	this->apl = apl_owner;
	this->obra_ref = obra;
	PanelTipo->Visible = false;
	// cria objetos
	lisTMP = new TList();
	// preenche dados da Obra
	if (obra_ref->Nome.IsEmpty())
		EditNome->Text = NomeProximaObra();
	else
		EditNome->Text = obra_ref->Nome;
	DateTimePickerInicio->Date = obra_ref->DataInicio;
	DateTimePickerAtivacao->Date = obra_ref->DataAtivacao;
	///* TODO : preparação futuro */
	// EditPNT->Text = DoubleToStr("%2.1f",obra_ref->ReducaoPNT);
	if (obra_ref->AlteraTaxaFalha)
	{
		CheckBoxQualidade->Checked = true;
		EditTaxaInsercao->Text = DoubleToStr("%2.1f", obra_ref->TaxaFalhaInsercao);
		EditTaxaSubstituicao->Text = DoubleToStr("%2.1f", obra_ref->TaxaFalhaSubstituicao);
	}
	else
	{
		CheckBoxQualidade->Checked = false;
		// EditTaxaInsercao->Text     = "0.0";
		// EditTaxaSubstituicao->Text = "0.0";
	}
	// inicia CLBoxRede
	CLBoxRedeInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormObra::TFormObra(TComponent* Owner) : TForm(Owner)
{
	// cria objetos
	lisTMP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TFormObra::~TFormObra(void)
{
	// destrói lista sem destruir seus objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::ActionCancelaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TFormObra::ActionConfirmaExecute(TObject *Sender)
{
	// variáveis locais
	double custo, ob_esp;

	// valida nome da obra
	if (EditNome->Text.IsEmpty())
	{
		Aviso("Defina o nome da obra");
		return;
	}
	// valida data selecionada
	if (!ValidaDataInicio())
		return;
	// verifica se há redes selecionadas
	lisTMP->Clear();
	for (int n = 0; n < CLBoxRede->Count; n++)
	{
		if (CLBoxRede->Checked[n])
			lisTMP->Add(CLBoxRede->Items->Objects[n]);
	}
	if (lisTMP->Count == 0)
	{
		Aviso("Selecione as redes da obra");
		return;
	}
	// atualiza Obra
	obra_ref->Nome = GetNome();
	obra_ref->DataInicio = GetDataInicio();
	obra_ref->DataAtivacao = GetDataAtivacao();
	obra_ref->Responsavel = "";
	obra_ref->ObEspecial = 0.;
	// taxas de falha
	if (CheckBoxQualidade->Checked)
	{
		obra_ref->AlteraTaxaFalha = true;
		obra_ref->TaxaFalhaInsercao =
			GetValorDouble(EditTaxaInsercao, "Taxa falha de inserção de eqpto");
		obra_ref->TaxaFalhaSubstituicao =
			GetValorDouble(EditTaxaSubstituicao, "Taxa falha de substituição de eqpto");
	}
	else
	{
		obra_ref->AlteraTaxaFalha = false;
		obra_ref->TaxaFalhaInsercao = 0.;
		obra_ref->TaxaFalhaSubstituicao = 0.;
	}
	// define redes da Obra
	obra_ref->IniciaLisRede(lisTMP);
	obra_ref->Obras->DefineRedesVisiveis(obra_ref);
	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::ActionRedeSelAllExecute(TObject *Sender)
{
	// cancela seleção todas as redes
	for (int n = 0; n < CLBoxRede->Count; n++)
	{
		CLBoxRede->Checked[n] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::ActionRedeSelNoneExecute(TObject *Sender)
{
	// cancela seleção todas as redes
	for (int n = 0; n < CLBoxRede->Count; n++)
	{
		CLBoxRede->Checked[n] = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::CheckBoxQualidadeClick(TObject *Sender)
{
	// exibe/esconde GBoxTaxaFalha
	GBoxTaxaFalha->Visible = CheckBoxQualidade->Checked;
	// arruma tamanho do groupbox
	if (GBoxTaxaFalha->Visible)
	{
		GBoxQualidade->Height = 117;
	}
	else
	{
		GBoxQualidade->Height = 44;
	}
	// acerta posição Top do GBoxQualidade com valor salvo em Tag
	// GBoxTaxaFalha->Top = GBoxTaxaFalha->Tag;
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::CLBoxRedeInicia(void)
{
	// variáveis locais
	int index;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		index = CLBoxRede->Items->AddObject(rede->Codigo, rede);
		if (rede->Visible())
		{
			CLBoxRede->Checked[index] = true;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::EditPNTChange(TObject *Sender)
{
	valorPNT = GetValorDouble(EditPNT, "Redução de perdas não técnicas");
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::EditPNTClick(TObject *Sender)
{
	EditPNT->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::EditTaxaInsercaoClick(TObject *Sender)
{
	if (GBoxTaxaFalha->Visible)
		EditTaxaInsercao->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::EditTaxaSubstituicaoClick(TObject *Sender)
{
	if (GBoxTaxaFalha->Visible)
		EditTaxaSubstituicao->SetFocus();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormObra::ExisteObra(TList *lisOBRA, AnsiString nome)
{
	// variáveis locais
	VTObra *obra;

	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (nome.AnsiCompareIC(obra->Nome) == 0)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::FormResize(TObject *Sender)
{ // variaveis locais
	int width;

	width = (this->Width - 28) / 2;
	gboxDataInicio->Width = width;
}

// ---------------------------------------------------------------------------
void __fastcall TFormObra::FormShow(TObject *Sender)
{
	// define campo c/ o focus
	EditNome->SetFocus();
	// acerta o groupbox de qualidade
	CheckBoxQualidadeClick(NULL);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormObra::NomeProximaObra(void)
{
	// variáveis locais
	int count = 1;
	AnsiString nome;

	do
	{ // define nome
		nome.sprintf("Obra %02d", count++);
	}
	while (ExisteObra(obra_ref->Obras->LisObra(), nome));

	return (nome);
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TFormObra::GetDataAtivacao(void)
{
	return (DateOf(DateTimePickerAtivacao->Date));
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TFormObra::GetDataInicio(void)
{
	return (DateOf(DateTimePickerInicio->Date));
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormObra::GetNome(void)
{
	return (EditNome->Text.Trim());
}

// ---------------------------------------------------------------------------
double __fastcall TFormObra::GetReducaoPNT(void)
{
	return (valorPNT);
}

// ---------------------------------------------------------------------------
double __fastcall TFormObra::GetValorDouble(TEdit *Edit, AnsiString descricao)
{
	try
	{
		return (StrToDouble(Edit->Text));
	}
	catch (Exception &e)
	{
		Erro(descricao + " com valor inválido");
		return (0.);
	}
}

////---------------------------------------------------------------------------
// FKM-2017.02.22 - adotado igual ao da eletrobras
////---------------------------------------------------------------------------
// bool __fastcall TFormObra::ValidaDataInicio(void)
// {
// //variáveis locais
// int       ind_obra;
// VTObra    *obra_ant, *obra_pos;
// TList     *lisOBRA    = obra_ref->Obras->LisObra();
// TDateTime  data       = DateTimePickerInicio->Date;
// TDateTime  data_atual = DateOf(Now());
//
// //verifica se data atual é posterior à data inicial da Alternativa
// if (data_atual > obra_ref->Obras->DataInicial)
// {//assume data_atual igual à data inicial da Alternativa
// data_atual = obra_ref->Obras->DataInicial;
// }
// //compara data com data_atual
// if (data < data_atual)
// {
// Aviso("Data de início da obra não pode ser anterior a " + DateToStr(data_atual));
// return(false);
// }
// //verifica se a Obra é nova
// if (obra_ref == NULL)
// {//Obra nova: compara com data final da Alternativa
// if (data < obra_ref->Obras->DataFinal)
// {
// Aviso("Data de início da obra não pode ser anterior a " + DateToStr(obra_ref->Obras->DataFinal));
// return(false);
// }
// return(true);
// }
// //Obra existente: determina índice da Obra na Alternativa
// if ((ind_obra = lisOBRA->IndexOf(obra_ref)) < 0) return(false);
// //verifica datas das Obras anteriores
// for (int n = 0; n < ind_obra; n++)
// {
// obra_ant = (VTObra*)lisOBRA->Items[n];
// if (data < obra_ant->DataInicio)
// {
// Aviso("Data de início anterior à obra " + AnsiQuotedStr(obra_ant->Nome, '\''));
// return(false);
// }
// }
// //verifica datas das Obras posteriore
// for (int n = ind_obra+1; n < lisOBRA->Count; n++)
// {
// obra_pos = (VTObra*)lisOBRA->Items[n];
// if (data > obra_pos->DataInicio)
// {
// Aviso("Data de início posterior à obra " + AnsiQuotedStr(obra_pos->Nome, '\''));
// return(false);
// }
// }
// return(true);
// }
// ---------------------------------------------------------------------------
bool __fastcall TFormObra::ValidaDataInicio(void)
{
	// variáveis locais
	int ind_obra;
	VTObra *obra_ant, *obra_pos;
	TList *lisOBRA;
	TDateTime data_ativ = DateTimePickerAtivacao->Date;
	TDateTime data_ini = DateTimePickerInicio->Date;
	TDateTime data_atual = DateOf(Now());

	// verifica se data de inicio é > data de ativação
	if (data_ini > data_ativ)
	{
		Aviso("Data de ativação não pode ser anterior à data de início.");
		return (false);
	}
	// verifica se a Obra é nova
	if (obra_ref == NULL)
	{ // Obra nova: não precisa validar
		return (true);
	}
	// Obra existente: verifica se data atual é posterior à data inicial de Obras
	if (data_atual > obra_ref->Obras->DataInicial)
	{ // assume data_atual igual à data inicial de Obras
		data_atual = obra_ref->Obras->DataInicial;
	}
	// compara data com data_atual
	if (data_ini < data_atual)
	{
		Aviso("Data de início da obra não pode ser anterior a " + DateToStr(data_atual));
		return (false);
	}
	// determina índice da Obra em Obras
	lisOBRA = obra_ref->Obras->LisObra();
	if ((ind_obra = lisOBRA->IndexOf(obra_ref)) < 0)
		return (false);
	// verifica datas das Obras anteriores
	for (int n = 0; n < ind_obra; n++)
	{
		obra_ant = (VTObra*)lisOBRA->Items[n];
		// considera data de ativação
		if (data_ativ < obra_ant->DataAtivacao)
		{
			Aviso("Data de ativação anterior à obra " + AnsiQuotedStr(obra_ant->Nome, '\''));
			return (false);
		}
	}
	// verifica datas das Obras posteriores
	for (int n = ind_obra + 1; n < lisOBRA->Count; n++)
	{
		obra_pos = (VTObra*)lisOBRA->Items[n];
		// considera data de ativação
		if (data_ativ > obra_pos->DataAtivacao)
		{
			Aviso("Data de ativação posterior à obra " + AnsiQuotedStr(obra_pos->Nome, '\''));
			return (false);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
