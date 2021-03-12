// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormDadosLicenca.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Produto\VTProduto.h>
#include <Fontes\RBAC\VTRBACManager.h>
#include <DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormDadosLicenca::TFormDadosLicenca(TComponent* Owner, VTApl *apl)
: TForm(Owner)
{
	// salva ponteiro para Apl
	this->apl = apl;
	// exibe dados da Licença
	DadosDaLicenca();
	// exibe operacoes
	DadosOperacoes();
	// mostra dados da licenca
	PageControl->ActivePage = TabSheetLicenca;
	// força ajuste da largura da coluna do form
	FormResize(NULL);
}

// ---------------------------------------------------------------------------
__fastcall TFormDadosLicenca::~TFormDadosLicenca(void)
{
	//
}

// ---------------------------------------------------------------------------
void __fastcall TFormDadosLicenca::ActionFechaExecute(TObject *Sender)
{
	ModalResult = mrOk;
}

// -----------------------------------------------------------------------------
void __fastcall TFormDadosLicenca::DadosDaLicenca(void)
{
	// variáveis locais
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));
	AnsiString validade, google_maps;

	// verifica se existe uma Licenca p/ o produto
	if ((licenca = licencas->ExisteLicenca()) == NULL)
	{
		// Aviso("Não existe Licença de Uso alocada");
		return;
	}
	// define validade
	validade = DefineValidade(licenca->Validade);
	// define google_maps
	google_maps = (licenca->Google) ? "habilitado" : "desabilitado";
	// insere dados da Licenca
	ListViewLicenca->Items->Item[0]->SubItems->Add(produto->Codigo);
	ListViewLicenca->Items->Item[1]->SubItems->Add(licenca->EmpresaStr);
	ListViewLicenca->Items->Item[2]->SubItems->Add(licenca->Login);
	ListViewLicenca->Items->Item[3]->SubItems->Add(AnsiString::StringOfChar('*',
		licenca->Password.Length()));
	ListViewLicenca->Items->Item[4]->SubItems->Add(DateToStr(licenca->DH_alocada));
	ListViewLicenca->Items->Item[5]->SubItems->Add(validade);
	ListViewLicenca->Items->Item[6]->SubItems->Add(google_maps);
}

// -----------------------------------------------------------------------------
void __fastcall TFormDadosLicenca::DadosOperacoes(void)
{
	// variáveis locais
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
	TListItem* item;

	// verifica se existe uma Licenca p/ o produto
	if ((licenca = licencas->ExisteLicenca()) == NULL)
	{
		// Aviso("Não existe Licença de Uso alocada");
		return;
	}
	// preenche operacoes
	for (int n = 0; n < licenca->Operacoes.Length; n++)
	{
		item = ListViewOperacao->Items->Add();
		item->Caption = IntToStr(licenca->Operacoes[n]);
		item->SubItems->Add(rbac->GetDescricaoOperacao(licenca->Operacoes[n]));
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormDadosLicenca::DefineValidade(int validade)
{
	AnsiString texto;

	// define validade
	if (validade < 0)
	{
		texto = "não limitada";
	}
	else if (validade > 1)
	{
		texto = IntToStr(validade) + " dias";
	}
	else
	{
		texto = IntToStr(validade) + " dia";
	}
	return(texto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDadosLicenca::FormResize(TObject *Sender)
{
	ListViewLicenca->Column[1]->Width = this->Width - 130;
	ListViewOperacao->Column[1]->Width = this->Width - 130;
}

// ---------------------------------------------------------------------------
// eof
