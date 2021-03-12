// ----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <map>
#include <iostream>
#include <cassert>
#include <list>

#include <Fontes\Apl\VTApl.h>
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEdt.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Flow\VTFlow.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "VTRelatorioJusante.h"
#include "VTRelJusChaves.h"
#include "TRelJusChaves.h"
#include "TFormRelatorioJusante.h"

// ----------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
TFormRelatorioJusante *FormRelatorioJusante;

// https://sinapsisenergia.teamwork.com/#/tasks/16586897
// ----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17283852
__fastcall TFormRelatorioJusante::TFormRelatorioJusante(TForm *form_owner, VTApl *apl_owner,
	TWinControl *parent) : TFormEdt(form_owner, parent)
{
	this->apl = apl_owner;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));

	if (sinap_child)
	{
		sinap_child->BeginEditorBlock(this);
	}
	// Cria objetosw
	grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	// progresso
	if (progresso == NULL)
	{
		apl->Add(progresso = DLL_NewObjProgresso(NULL, NULL));
		progresso->Start(progDEFAULT);
	}
	if (progresso != NULL)
	{
		progresso->Start(progDEFAULT);
	}
	relatorioJusante = NewObjRelatorioJusante(apl);
	relatorioJusante->PreparaExecucao();
	if (progresso != NULL)
	{
		progresso->Stop();
	}

	// esconde o negocio do flow
	checkFlow->Visible = false;
	if (checkFlow->Visible == false)
	{
		gboxOpcoes->Height = 69;
	}
	else
	{
		gboxOpcoes->Height = 90;
	}
	//Se não existir redes BT, ele avisa ao usuário que o relatório não inclui
	//informações da BT

	//Verifica se NÃO existe redes BT carregadas
		if(!ExisteRedeEqvBT())
	{
		 //Se não possui BT - o Usuário é avisado antes de ser aberta o rel_jusante
		 Aviso("O sistema não possui redes BT ");
	}
	// Inicia tipo de relatorio
	tipo_relatorio  = 0 ;
	CBoxRelatorio->ItemIndex = tipo_relatorio;
	// Inicia patamar
	CBoxPatamarInicia();
	//
	ActionEdt->Execute();
}

// ----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17283852
__fastcall TFormRelatorioJusante::~TFormRelatorioJusante(void)
{
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

    if (sinap_child)
	{
		sinap_child->EndEditorBlock(this);
	}
	// exibe moldura no Eqpto
	if (grafico)
	{
		grafico->Moldura();
		TList *listaNULL = new TList;
		grafico->DestacaEqpto(listaNULL, clRed);
		delete listaNULL;
		listaNULL = NULL;
	}

	// destrói objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
}

// ----------------------------------------------------------------------------
// void __fastcall TFormRelatorioJusante::CalculaPotenciaTransformadoresJusante(void)
// {
// somaTrafosJusante = relatorioJusante->CalculaTrafosJusante(eqpto);
// txtRelatorioJusante->Text = DoubleToStr("%4.3f", somaTrafosJusante);
// }

// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::ActionEdtExecute(TObject *Sender)
{

	try
	{ // se necessário, cria um objeto VTEdt
		if (edt != NULL)
		{
			delete edt;
		}
		// cria VTEdt p/ controle do mouse
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelecionaLigacao);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
		// Form está ativo: atualiza  moldura
		SelecionaEquipamento();
	}
	catch (Exception &e)
	{
		// nada a fazer
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::ActionExportaExecute(TObject *Sender)
{
	// variáveis locais
	TStringList *relatorio = new TStringList();
	AnsiString caminho;
	AnsiString arq;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		//verifica em qual aba foi requisitada para se exportar o relatório //DSF
        switch (tabControl->TabIndex)
		{
		case 0: // consumidores
		case 1: // cargas
		case 2: // trafos
			PreparaRelatorio(vleAtributos->Strings, relatorio);
			break;
		case 3:
			PreparaRelatorioTrechos(vleAtributos->Strings, relatorio); //o relatório de
			break;                                   //trechos é diferenciado porque
		default:                                     //deve-se discriminar o nivel de tensao
			break;
		}
		//determina o caminho onde será salvo o arquivo
		arq = SaveDialog->FileName;
		//salva o arquivo
		relatorio->SaveToFile(arq);
		//avisa o usuário que tudo deu certo
		Aviso("Relatório de trechos a jusante/montante exportado com sucesso!");


	}
	catch (Exception &e)
	{
		Aviso("Erro ao exportar relatório.");
	}

	delete relatorio;
}

// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::ActionSelecionaLigacaoExecute(TObject *Sender)
{
	// variáveis locais
	// VTEqpto *eqpto;
	TList *lisEXT = (TList*)Sender;

	// proteção
	if (lisEXT == NULL)
		return;
	if (lisEXT->Count == 0)
		return;
	// determina primeiro Eqpto da lista
	if ((eqpto = (VTEqpto*)lisEXT->First()) == NULL)
		return;

	// atualiza  moldura
	SelecionaEquipamento();
	tabControlChange(NULL);

}

// ---------------------------------------------------------------------------

void __fastcall TFormRelatorioJusante::CBoxPatamarInicia(void)
{
	// variáveis locais
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisPAT = patamares->LisPatamar();

	// reinicia CBoxPatamar c/ Patamares
	CBoxPatamar->Clear();
	for (int np = 0; np < lisPAT->Count; np++)
	{
		patamar = (VTPatamar*)lisPAT->Items[np];
		if (patamar->Enabled)
		{
			CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
		}
	}
	// pré-seleciona primeiro patamar
	if (CBoxPatamar->Items->Count > 0)
	{
		CBoxPatamar->ItemIndex = 0;
		patamar = (VTPatamar*) CBoxPatamar->Items->Objects[0];
		// relatorioJusante->SelecionaPatamar(patamar);
		patamar_sel = patamar;
	}
}

// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::CBoxPatamarChange(TObject *Sender)
{
	VTPatamar *patamar;
	int index = CBoxPatamar->ItemIndex;

	patamar = (VTPatamar*) CBoxPatamar->Items->Objects[index];
	// relatorioJusante->SelecionaPatamar(patamar);
	patamar_sel = patamar;
	tabControlChange(NULL);
}

// https://sinapsisenergia.teamwork.com/#/tasks/16586897
// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::CBoxRelatorioChange(TObject *Sender)
{
	// Tipo de Relatorio
	// 0 - Relatorio Jusante
    // 1 - Relatorio Montante
	tipo_relatorio = CBoxRelatorio->ItemIndex;
	tabControlChange(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::CheckBoxFlowInicia(void)
{
	// variáveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	// mostra se o fluxo esta atualizado
	checkFlow->Checked = flow->UpToDate;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::checkFlowClick(TObject *Sender)
{
	// variáveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	if (checkFlow->Checked == true)
	{
		if (flow->UpToDate != true)
		{
			if (Confirma("Fluxo não calculado,", "Deseja calcular?") == IDYES)
			{
				flow->CalculaPorRede(redes);
			}
			else
			{
				checkFlow->Checked = false;
			}
		}
	}
}

// ----------------------------------------------------------------------------
bool __fastcall TFormRelatorioJusante::ExisteRedeEqvBT(void)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (! rede->Carregada) continue;
		if (rede->TipoRede->Segmento == redeSEC)
			return (true);
	}
	return (false);
}

// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o form
	Action = caFree;
}

//https://sinapsisenergia.teamwork.com/#/tasks/16586897
// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::PanelColorClick(TObject *Sender)
{
	TColor cor_antiga;
	// apresenta form de seleção de cor
	cor_antiga = PanelColor->Color;
	dlgSelColor->Color = PanelColor->Color;
	if (dlgSelColor->Execute())
	{ // redefine cor do PanelColor
		PanelColor->Color = dlgSelColor->Color;
		relatorioJusante->CorDestaque = dlgSelColor->Color;
		if (cor_antiga != relatorioJusante->CorDestaque)
		{
			if(tipo_relatorio == 0)
			{relatorioJusante->DestacaEquipamentosJusante();}
			if(tipo_relatorio == 1)
			{relatorioJusante->DestacaEquipamentosMontante();}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::PreencheListValueEditor(int tipoeqpto)
{
	TStrings *lisKEY_VALUE;

	// protecao
	if (tipoeqpto < 0)
	{
		return;
	}
	lisKEY_VALUE = vleAtributos->Strings;

	relatorioJusante->AgregaValores(lisKEY_VALUE, eqpto, tipoeqpto, patamar_sel, tipo_relatorio);

}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::PreparaRelatorio(TStrings *lisAtribValor,
	TStringList *relatorio)
{
	TStringList *lisCampos = new TStringList();
	AnsiString cabecalho, atributo, valor, linha;

	// esvazia a lista
	relatorio->Clear();

	//cria cabeçalho do relatório
	cabecalho = "Atributo; Valor";
	relatorio->Add(cabecalho);

	for (int i = 0; i < lisAtribValor->Count; i++)
	{
		//separa o atributo do valor
		ExtraiStrings(lisAtribValor->Strings[i], "=", lisCampos);
		atributo = lisCampos->Strings[0];
		valor = lisCampos->Strings[1];
		// adciona o ponto e vírgula, tornando a linha compatível com um CSV
		linha = atributo + ";" + valor + ";";

		relatorio->Add(linha);

		lisCampos->Clear();
	}

	delete lisCampos;

}
// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::PreparaRelatorioTrechos(TStrings *lisAtribValor,
	TStringList *relatorio)
{
	TStringList *lisCampos = new TStringList();
	AnsiString cabecalho, atributo, valor, nivelTensao, linha;

	//esvazia a lista
	relatorio->Clear();

	//cria cabeçalho do relatório
	cabecalho = "Nivel de Tensao; Arranjo; Comprimento (km)";
	relatorio->Add(cabecalho);

	for (int i = 0; i < lisAtribValor->Count; i++)
	{
		//separa o atributo do valor
		ExtraiStrings(lisAtribValor->Strings[i], "=", lisCampos);
		atributo = lisCampos->Strings[0];
		valor = lisCampos->Strings[1];
		//separa o nivel de tensão
		nivelTensao = atributo.SubString(0, 2);
		atributo = atributo.SubString(4, atributo.Length() - 3);
		// adciona o ponto e vírgula, tornando a linha compatível com um CSV
		linha = nivelTensao + ";" + atributo + ";" + valor + ";";

		relatorio->Add(linha);

		lisCampos->Clear();
	}

	delete lisCampos;

}

// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::SelecionaEquipamento(void)
{
	// exibe moldura no Eqpto
	if (grafico)
	{
//		try
//		{
			grafico->Moldura(eqpto);
//		}
//			catch(...)
//		{
//		}
	}
}

//https://sinapsisenergia.teamwork.com/#/tasks/16586897
// ----------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::tabControlChange(TObject *Sender)
{
	int tipoeqpto = -1;

	DefineTitleCaptionVlwATributos("Atributo", "Valor");

	switch (tabControl->TabIndex)
	{
	case 0: // Resumo
		tipoeqpto = 0;
		CBoxPatamar->Enabled = false;
		break;
	case 1: // Consumidores
		tipoeqpto = eqptoCONSUMIDOR;
		CBoxPatamar->Enabled = false;
		break;
	case 2: // Carga
		tipoeqpto = eqptoCARGA;
		CBoxPatamar->Enabled = true;
		relatorioJusante->SelecionaPatamar
			((VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
		break;
	case 3: // Trafos
		tipoeqpto = eqptoTRAFO;
		CBoxPatamar->Enabled = false;
		break;
	case 4: // Trechos
		tipoeqpto = eqptoTRECHO;
		CBoxPatamar->Enabled = false;
		DefineTitleCaptionVlwATributos("Atributo", "Valor(km)");
		break;
	default:
		tipoeqpto = -1;
	}
	if (eqpto != NULL)
	{
		tipo_relatorio = CBoxRelatorio->ItemIndex;
		PreencheListValueEditor(tipoeqpto);
		if(tipo_relatorio == 0)
		{relatorioJusante->DestacaEquipamentosJusante();}
		else if(tipo_relatorio == 1)
		{relatorioJusante->DestacaEquipamentosMontante();}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioJusante::DefineTitleCaptionVlwATributos(AnsiString primeiraColuna,
	AnsiString segundaColuna)
{
	vleAtributos->TitleCaptions->Clear();
	vleAtributos->TitleCaptions->Add(primeiraColuna);
	vleAtributos->TitleCaptions->Add(segundaColuna);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRelatorioJusante::SelecionaArquivo(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// verifica se o diretório SaveDialog já foi definido
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	return (SaveDialog->Execute());
}

//---------------------------------------------------------------------------
//eof





