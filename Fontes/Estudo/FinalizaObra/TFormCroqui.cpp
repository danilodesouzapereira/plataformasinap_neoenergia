// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Geral\VTInfoset.h>
#include <PlataformaSinap\Dll_Inc\Funcao.h>
#include "TFormCroqui.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCroqui::TFormCroqui(TComponent* Owner, VTApl *apl) : TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	FormIniciaPosicao();
	//cria objeto
	jpg = new TJPEGImage();
}

// ---------------------------------------------------------------------------
__fastcall TFormCroqui::~TFormCroqui(void)
{
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	//destroi objeto
	if(jpg)
	{
		delete jpg;
		jpg = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormCroqui::FormClose(TObject *Sender, TCloseAction &Action)
{
	// ModalResult = mrOk;
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCroqui::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCroqui::FormShow(TObject *Sender)
{
	// nada
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCroqui::LeArquivo(AnsiString filepath)
{
	try
	{ // define o arquivo imagem
		ImageCroqui->Picture->LoadFromFile(filepath);
	}
	catch (Exception &e)
	{
		Aviso("Erro ao abrir arquivo: " + e.Message);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCroqui::SalvaArquivo(AnsiString filepath)
{
	try
	{ // define o arquivo imagem
		ImageCroqui->Picture->SaveToFile(filepath);
		jpg->Assign(ImageCroqui->Picture->Bitmap);
		jpg->SaveToFile(filepath);
	}
	catch (Exception &e)
	{
		Aviso("Erro ao salvar arquivo: " + e.Message);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
