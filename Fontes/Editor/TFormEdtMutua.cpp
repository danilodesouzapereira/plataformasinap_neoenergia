// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtMutua.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtMutua::TFormEdtMutua(TComponent* Owner, VTApl *apl, VTMutua* mutua)
	: TForm(Owner)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	this->mutua = mutua;
	// cria lista
	lisMUTUA = new TList();
	// verifica se as Barras de referência estão definidas
	if (mutua->pbar_tre1 == NULL)
		mutua->pbar_tre1 = mutua->ptre1->pbarra1;
	if (mutua->pbar_tre2 == NULL)
		mutua->pbar_tre2 = mutua->ptre2->pbarra1;
	// desabilita TableSheet de planejamento
	tsPlaneja->TabVisible = false;
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtMutua::~TFormEdtMutua(void)
{ // variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói lista sem destruir seus objetos
	if (lisMUTUA)
	{
		delete lisMUTUA;
		lisMUTUA = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::ActionCancelaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::ActionConfirmaExecute(TObject *Sender)
{
	if (!LeDadosMutua())
		return;
	// atualiza status do Eqpto: alterado
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::ActionRetiraExecute(TObject *Sender)
{
	ModalResult = mrNo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::DefValoresDefaultMutua(void)
{
	// variáveis locais
	VTMutua *ult_mutua;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se a Mutua já foi iniciado
	if ((mutua->z0.r == 0.) && (mutua->z0.x == 0.))
	{ // inicia Mutua c/ valores default
		mutua->z0.r = 0.;
		mutua->z0.x = 0.05;
		mutua->z1 = mutua->z0;
		// seleciona Mutua de referência p/ copiar dados
		lisMUTUA->Clear();
		redes->LisEqpto(lisMUTUA, eqptoMUTUA);
		if (lisMUTUA->Count == 0)
			return;
		if ((ult_mutua = (VTMutua*)lisMUTUA->Last()) == NULL)
			return;
		// copia dados da chave de referência
		mutua->z0 = ult_mutua->z0;
		mutua->z1 = ult_mutua->z1;
	}
}

// ---------------------------------------------------------------------------
void _fastcall TFormEdtMutua::ExibeDadosMutua(void)
{
	//variaveis locais
	AnsiString codigo;

	// exibe dados do Mutua
	EditCodigo->Text = mutua->Codigo;
	edtX0->Text = DoubleToStr("%5.4f", mutua->z0.x);
	edtX1->Text = DoubleToStr("%5.4f", mutua->z1.x);
	// atualiza Labels c/ códigos dos Trechos
	// LabelTre1->Caption = mutua->ptre1->Codigo;
	// LabelTre2->Caption = mutua->ptre2->Codigo;
	// FKM - 2015.10.07 - Evita nomes em branco \\
	codigo = mutua->ptre1->Codigo;
	if(codigo.IsEmpty())
	{
		LabelTre1->Caption = "(id: " + IntToStr(mutua->ptre1->Id) + ")";
	}
	else
	{
		LabelTre1->Caption = codigo;
	}
	codigo = mutua->ptre2->Codigo;
	if(codigo.IsEmpty())
	{
		LabelTre2->Caption = "(id: " + IntToStr(mutua->ptre2->Id) + ")";
	}
	else
	{
		LabelTre2->Caption = codigo;
	}
	// FKM - 2015.10.07 - //
	// inicia CBox c/ Barras dos Trechos
	IniciaCBoxBar(cboxBarTre1, mutua->ptre1->pbarra1, mutua->ptre1->pbarra2);
	IniciaCBoxBar(cboxBarTre2, mutua->ptre2->pbarra1, mutua->ptre2->pbarra2);
	// seleciona no CBox as Barras de referência p/ a mútua
	cboxBarTre1->ItemIndex = cboxBarTre1->Items->IndexOfObject(mutua->pbar_tre1);
	cboxBarTre2->ItemIndex = cboxBarTre2->Items->IndexOfObject(mutua->pbar_tre2);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::FormShow(TObject *Sender)
{
	// se Mutua nova, inicia seus dados c/ dados da última Mutua
	DefValoresDefaultMutua();
	// inicia Captions,Labels etc da tela
	IniciaParteFixaTela();
	// exibe dados da Mutua
	ExibeDadosMutua();
	// seleciona TableSheet
	pgCtrl->ActivePage = tsMutua;
	// posiciona o Cursor em EditCodigo
	EditCodigo->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::IniciaCBoxBar(TComboBox *cboxBar, VTBarra *pbar1, VTBarra *pbar2)
{
	// limpa CBox
	cboxBar->Clear();
	// insere Barras no CBox
	cboxBar->Items->AddObject(pbar1->Codigo, pbar1);
	cboxBar->Items->AddObject(pbar2->Codigo, pbar2);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::IniciaParteFixaTela(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtMutua::LeDadosMutua(void)
{
	// proteção: verifica se Mutua está definida
	if (mutua == NULL)
		return (true);
	// verifica se as Barras de referência estão definidas
	if ((cboxBarTre1->ItemIndex < 0) || (cboxBarTre2->ItemIndex < 0))
	{
		Erro("Selecione as barras de referência dos trechos da mútua");
		return (false);
	}
	try
	{ // lê dados d0 Mutua
		mutua->Codigo = EditCodigo->Text;
		mutua->z0.r = 0.;
		mutua->z0.x = StrToDouble(edtX0->Text);
		mutua->z1.r = 0.;
		mutua->z1.x = StrToDouble(edtX1->Text);
		// determina Barras de referência
		mutua->pbar_tre1 = (VTBarra*)cboxBarTre1->Items->Objects[cboxBarTre1->ItemIndex];
		mutua->pbar_tre2 = (VTBarra*)cboxBarTre2->Items->Objects[cboxBarTre2->ItemIndex];
	}
	catch (Exception &e)
	{
		Erro("Dados Inválidos da Mutua");
		return (false);
	}
	// valida valores
	if ((mutua->z0.x == 0.) || (mutua->z1.x == 0))
	{
		Erro("Valores inválidos (impedância nula)");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMutua::OpcaoRetira(bool enabled)
{
	butRetira->Visible = enabled;
}

// ---------------------------------------------------------------------------
// eof
