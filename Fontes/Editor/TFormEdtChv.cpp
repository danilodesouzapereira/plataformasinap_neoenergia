// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtChv.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtChv::TFormEdtChv(TComponent* Owner, VTApl *apl, VTChave *chave, bool op_retira)
	: TForm(Owner)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	this->chave = chave;
	// habilita/desabilita opção de retirar a Chave
	ActionRetira->Visible = op_retira;
	// se Chave nova, inicia seus dados c/ dados da última Chave
	IniciaDadosChaveNova();
	// inicia CBox c/ as Redes
	CBoxRedeInicia();
	// inicia CBox c/ tipos das chaves
	CBoxTipoChaveInicia();
	// inicia CBoxTecnoCom
	CBoxTecnoComInicia();
	// exibe dados da Chave
	ExibeDadosChave();
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtChv::~TFormEdtChv(void)
{ // variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::ActionCancelaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::ActionConfirmaExecute(TObject *Sender)
{
	// lê dados da Chave
	if (!LeDadosChave())
		return;
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::ActionRetiraExecute(TObject *Sender)
{
	ModalResult = mrNo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::CBoxRedeInicia(void)
{
	// variáveis locais
	int index;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// reinicia CBox c/ as Redes
	CBoxRede->Clear();
	if (lisREDE->Count == 0)
		return;
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		CBoxRede->Items->AddObject(rede->Codigo, rede);
	}
	// define ordem alfabética
	CBoxRede->Sorted = true;
	// pré-seleciona Rede que conecta as Barras da Chave
	index = 0;
	if ((rede = SelecionaRede(lisREDE)) != NULL)
		index = CBoxRede->Items->IndexOfObject(rede);
	CBoxRede->ItemIndex = index;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::CBoxTecnoComChange(TObject *Sender)
{
	// verifica se foi selecionada opção "não disponível"
	if (CBoxTecnoCom->ItemIndex == 0)
	{ // esconde CheckBoxTelecomandada
		CheckBoxTelecomandada->Visible = false;
		CheckBoxTelecomandada->Checked = false;
	}
	else
	{ // exibe CheckBoxTelecomandada
		CheckBoxTelecomandada->Visible = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::CBoxTecnoComInicia(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::CBoxTipoChaveChange(TObject *Sender)
{
	// variáveis locais
	int index;
	VTTipoChave *tipochave = NULL;

	// determina TipoChave selecionado
	if ((index = CBoxTipoChave->ItemIndex) >= 0)
	{
		tipochave = (VTTipoChave*)CBoxTipoChave->Items->Objects[index];
	}
	// exibe/esconde PanelElo
	PanelElo->Visible = ((tipochave != NULL) && (tipochave->Tipo == chaveBF));
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::CBoxTipoChaveInicia(void)
{
	// variáveis locais
	int index;
	VTTipoChave *tipochave;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTIPO = tipos->LisTipoChave();

	// reinicia CBoxTipoChave
	CBoxTipoChave->Clear();
	// loop p/ todos TipoChave
	for (int n = 0; n < lisTIPO->Count; n++)
	{
		tipochave = (VTTipoChave*)lisTIPO->Items[n];
		CBoxTipoChave->Items->AddObject(tipochave->Codigo, tipochave);
	}
	// determina tipo da chave
	if ((index = CBoxTipoChave->Items->IndexOfObject(chave->TipoChave)) < 0)
		index = 0;
	CBoxTipoChave->ItemIndex = index;
}

// ---------------------------------------------------------------------------
void _fastcall TFormEdtChv::ExibeDadosChave(void)
{
	// exibe dados da Chave
	EditCodigo->Text = chave->Codigo;
	edtChvInom->Text = DoubleToStr("%2.1f", chave->Inom_amp);
	CBoxEstado->ItemIndex = chave->Aberta ? 0 : 1;
	// EditElo->Text         = chave->Extern_id;  //DVK 2016.10.07
	EditElo->Text = chave->EloFusivel;
	// DVK 2014.10.28
	CheckBoxBloqueada->Checked = chave->Bloqueada;
	edtChvTipoGIS->Text = chave->TipoGIS;
	// define tecnologia de comunicação
	switch (chave->TecnoComunicacao)
	{
	case comNAO_DISPONIVEL:
		CBoxTecnoCom->ItemIndex = 0;
		break;
	case comFIBRA_OTICA:
		CBoxTecnoCom->ItemIndex = 1;
		break;
	case comCELULAR:
		CBoxTecnoCom->ItemIndex = 2;
		break;
	case comRADIO:
		CBoxTecnoCom->ItemIndex = 3;
		break;
	case comMICOROONDAS:
		CBoxTecnoCom->ItemIndex = 4;
		break;
	case comSATELITE:
		CBoxTecnoCom->ItemIndex = 5;
		break;
	case comDESCONHECIDO:
		CBoxTecnoCom->ItemIndex = 6;
		break;
	default:
		CBoxTecnoCom->ItemIndex = 0;
		break;
	}
	// define se opção de chave telecomandada
	CheckBoxTelecomandada->Checked = chave->Telecomandada;
	// simula alteração de TipoChave para exibir/esconder informação do elo
	CBoxTipoChaveChange(NULL);
	// simula alteração de opção de tecnologia para garantir consistência com opção de telecomandada
	CBoxTecnoComChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::FormShow(TObject *Sender)
{
	// posiciona o Cursor em EditCodigo
	EditCodigo->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtChv::IniciaDadosChaveNova(void)
{
	/*
	 //variáveis locais
	 VTChave *ult_chv;

	 //verifica se a chave já está definida
	 if (chave->corn > 0.) return;
	 //seleciona Chave de referência p/ copiar os dados
	 if (lisCHV->Count > 0)
	 {
	 if ((ult_chv = (VTChave*)lisCHV->Last()) == NULL)
	 {//assume valores default
	 chave->corn = 400.;
	 }
	 else
	 {//copia dados da chave de referência
	 chave->corn = ult_chv->corn;
	 }
	 }
	 */
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtChv::LeDadosChave(void)
{
	// proteção: verifica se o tipo da Chave está definido
	if (CBoxTipoChave->ItemIndex < 0)
	{
		Aviso("Defina o tipo da Chave");
		return (true);
	}
	// proteção: verifica se a Rede da Chave está definida
	if (CBoxRede->ItemIndex < 0)
	{
		Aviso("Defina a Rede da Chave");
		return (true);
	}
	// proteção: verifica se tecnologia de comunicação está definida
	if (CBoxTecnoCom->ItemIndex < 0)
	{
		Aviso("Defina tecnologia de comunicação da chave");
		return (true);
	}
	try
	{ // lê dados da Chave
		chave->TipoChave = (VTTipoChave*)CBoxTipoChave->Items->Objects[CBoxTipoChave->ItemIndex];
		chave->Codigo = EditCodigo->Text;
		chave->Inom_amp = StrToDouble(edtChvInom->Text);
		(CBoxEstado->ItemIndex == 0) ? chave->Abre() : chave->Fecha();
		// DVK 2014.10.28
		chave->Bloqueada = CheckBoxBloqueada->Checked;
		// verifica se o texto tem 21 caracteres
		if (edtChvTipoGIS->Text.Length() > 21)
		{
			Aviso("O Tipo de Chave do GIS só pode ter até 21 caracteres");
			return (false);
		}
		chave->TipoGIS = edtChvTipoGIS->Text;
		// determina Rede da Chave
		chave->rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
		// lê opção tecnologia de comunicação
		switch (CBoxTecnoCom->ItemIndex)
		{
		case 0:
			chave->TecnoComunicacao = comNAO_DISPONIVEL;
			break;
		case 1:
			chave->TecnoComunicacao = comFIBRA_OTICA;
			break;
		case 2:
			chave->TecnoComunicacao = comCELULAR;
			break;
		case 3:
			chave->TecnoComunicacao = comRADIO;
			break;
		case 4:
			chave->TecnoComunicacao = comMICOROONDAS;
			break;
		case 5:
			chave->TecnoComunicacao = comSATELITE;
			break;
		case 6:
			chave->TecnoComunicacao = comDESCONHECIDO;
			break;
		}
		// lê opção de chave telecomandada
		chave->Telecomandada = CheckBoxTelecomandada->Checked;
		// lê tipo do Elo //DVK 2016.10.07 utiliza property correta
		// chave->Extern_id = "";
		chave->EloFusivel = "";
		if (chave->TipoChave->Tipo == chaveBF)
		{ // verifica se o texto tem 3 caracteres
			if (EditElo->Text.Length() > 3)
			{
				Aviso("O Elo Fusível só pode ter 3 caracteres");
				return (false);
			}
			// verifica se o tipo do elo
			// chave->Extern_id = EditElo->Text;
			chave->EloFusivel = EditElo->Text;
			// if (chave->Extern_id.IsEmpty())
			if (chave->EloFusivel.IsEmpty())
			{ // pede para o usuário confirmar
				if (Confirma("O tipo do elo não está definido.", "Deseja definir o elo ?") != IDNO)
					return (false);
			}
		}
	}
	catch (Exception &e)
	{
		Erro("Dados Inválidos da Chave");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtChv::SelecionaRede(TList *lisREDE)
{
	// variáveis locais
	VTRede *rede;

	// verifica se a Chave já possui uma Rede definida
	if (chave->rede)
		return (chave->rede);
	// procura uma Rede que conecta as Barras da Chave
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if ((rede->ExisteBarra(chave->pbarra1)) || (rede->ExisteBarra(chave->pbarra2)))
			return (rede);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
// eof

