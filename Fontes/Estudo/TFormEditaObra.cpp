// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormEditaObra.h"
// #include "VTCustos.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Planejamento\VTAlternativa.h"

#include "..\..\Dll_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEditaObra::TFormEditaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra)
	: TForm(Owner)
{
	// salva objetos
	this->apl = apl_owner;
	this->obra_ref = obra;

	if (obra_ref == NULL)
	{
        throw;
	}
	// preenche dados da Obra
	if (obra_ref->Nome.IsEmpty())
		EditNome->Text = NomeProximaObra();
	else
		EditNome->Text = obra_ref->Nome;
	DateTimePickerInicio->Date = obra_ref->DataInicio;
	DateTimePickerAtivacao->Date = obra_ref->DataAtivacao;
	// cria objetos
	lisTMP = new TList();
    // inicia CBoxTipo
	CBoxTipoInicia();
	// seleciona tipo da Obra em CBoxTipo
	if (obra_ref != NULL)
	{
		if(obra_ref->Tipo < CBoxTipo->Items->Count)
		{
			CBoxTipo->ItemIndex = obra_ref->Tipo;
        }
	}
	// inicia CLBoxRede
	CLBoxRedeInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaObra::~TFormEditaObra(void)
{
	// destrói objetos
	// if (apl != NULL) {delete apl; apl = NULL;}
	// destrói lista sem destruir seus objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionCancelaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionConfirmaExecute(TObject *Sender)
{
	// variáveis locais
	double custo, ob_esp;
	int number_obra;

	// valida nome da obra
	if (EditNome->Text.IsEmpty())
	{
		Aviso("Defina o nome da obra");
		return;
	}
	//arruma nome
    EditNome->Text = ASCIIFilterWithAccents(EditNome->Text);
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
	// Verifica se o nome da obra já existe - Evita ter nomes de obras repetidos na alternativa
	if(NomeObraExist(obra_ref->Obras->LisObra(), GetNome()))
	{
		Aviso("O nome da obra já existe. Defina outro nome.");
		return;
	}
	// atualiza Obra
	obra_ref->Nome = GetNome();
	obra_ref->DataInicio = GetDataInicio();
	obra_ref->DataAtivacao = GetDataAtivacao();
	obra_ref->Responsavel = "";
	obra_ref->ObEspecial = 0.;
    // define redes da Obra
	obra_ref->IniciaLisRede(lisTMP);
	obra_ref->Obras->DefineRedesVisiveis(obra_ref);
	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionRedeSelAllExecute(TObject *Sender)
{
	// cancela seleção todas as redes
	for (int n = 0; n < CLBoxRede->Count; n++)
	{
		CLBoxRede->Checked[n] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionRedeSelNoneExecute(TObject *Sender)
{
	// cancela seleção todas as redes
	for (int n = 0; n < CLBoxRede->Count; n++)
	{
		CLBoxRede->Checked[n] = false;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::CBoxTipoInicia(void)
{
    // variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();

	// reinicia CBoxTipo
	CBoxTipo->Clear();
	// Tipo da Obra (tipoObra)
	// EXP - Expansão
	// REN - Renovação
	// MEL - Melhoria
	// OBR - Obra com Participação Financeira
	// LUT - Luz Para Todos
	// Programas Copel
	CBoxTipo->Items->Add("Expansão");
	CBoxTipo->Items->Add("Renovação");
	CBoxTipo->Items->Add("Melhoria");
	CBoxTipo->Items->Add("Obra com Participação Financeira");
	CBoxTipo->Items->Add("Luz Para Todos");
	// verifica se é Copel
	if ((licenca != NULL) && (licenca->Empresa == COPEL))
	{
		CBoxTipo->Items->Add("Programas Copel");
	}

	// seleciona o primeiro
	CBoxTipo->ItemIndex = 0;
}
// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::CLBoxRedeInicia(void)
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
bool __fastcall TFormEditaObra::ExisteObra(TList *lisOBRA, AnsiString nome)
{
	// variáveis locais
	VTObra *obra;

	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (nome.AnsiCompareIC(obra->Nome) == 0)
		{
			return (true);
		}

	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaObra::FormResize(TObject *Sender)
{
	//redimensiona gboxIni e gboxAtiv
	int width = this->Width;
	GroupBoxIni->Width = int((width - 28) / 2);
    GroupBoxAtiv->Width = int((width - 28) / 2);
}

//---------------------------------------------------------------------------
void __fastcall TFormEditaObra::FormShow(TObject *Sender)
{
	// define campo c/ o focus
	EditNome->SetFocus();
}
// ---------------------------------------------------------------------------
int __fastcall TFormEditaObra::GetTipo(void)
{
	return (CBoxTipo->ItemIndex);
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEditaObra::NomeProximaObra(void)
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

// https://sinapsisenergia.teamwork.com/#/tasks/16218782
// ---------------------------------------------------------------------------
bool __fastcall TFormEditaObra::NomeObraExist(TList *lisOBRA, AnsiString nome)
{
	// variáveis locais
	VTObra *obra;

	// Verifica se o nome da obra já existe
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
        // Verifica que a obra com o nome existente seja diferente da obra selecionada
		if ((nome.AnsiCompareIC(obra->Nome) == 0) && (obra != obra_ref))
		{
			return (true);
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TFormEditaObra::GetDataAtivacao(void)
{
	return (DateOf(DateTimePickerAtivacao->Date));
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TFormEditaObra::GetDataInicio(void)
{
	return (DateOf(DateTimePickerInicio->Date));
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEditaObra::GetNome(void)
{
	return (EditNome->Text.Trim());
}

// ---------------------------------------------------------------------------
double __fastcall TFormEditaObra::GetValorDouble(TEdit *Edit, AnsiString descricao)
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
// ---------------------------------------------------------------------------
bool __fastcall TFormEditaObra::ValidaDataInicio(void)
{
	// variáveis locais
	int ind_obra;
	VTObra *obra_ant, *obra_pos;
	TList *lisOBRA = obra_ref->Obras->LisObra();
	TDateTime data = DateTimePickerInicio->Date;
	TDateTime data_atual = DateOf(Now());

	// verifica se data atual é posterior à data inicial da Alternativa
	if (data_atual > obra_ref->Obras->DataInicial)
	{ // assume data_atual igual à data inicial da Alternativa
		data_atual = obra_ref->Obras->DataInicial;
	}
	// compara data com data_atual
	if (data < data_atual)
	{
		Aviso("Data de início da obra não pode ser anterior a " + DateToStr(data_atual));
		return (false);
	}
	// verifica se a Obra é nova
	if (obra_ref == NULL)
	{ // Obra nova: compara com data final da Alternativa
		if (data < obra_ref->Obras->DataFinal)
		{
			Aviso("Data de início da obra não pode ser anterior a " +
				DateToStr(obra_ref->Obras->DataFinal));
			return (false);
		}
		return (true);
	}
	// Obra existente: determina índice da Obra na Alternativa
	if ((ind_obra = lisOBRA->IndexOf(obra_ref)) < 0)
		return (false);
	// verifica datas das Obras anteriores
	for (int n = 0; n < ind_obra; n++)
	{
		obra_ant = (VTObra*)lisOBRA->Items[n];
		if (data < obra_ant->DataInicio)
		{
			Aviso("Data de início anterior à obra " + AnsiQuotedStr(obra_ant->Nome, '\''));
			return (false);
		}
	}
	// verifica datas das Obras posteriore
	for (int n = ind_obra + 1; n < lisOBRA->Count; n++)
	{
		obra_pos = (VTObra*)lisOBRA->Items[n];
		if (data > obra_pos->DataInicio)
		{
			Aviso("Data de início posterior à obra " + AnsiQuotedStr(obra_pos->Nome, '\''));
			return (false);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof


