// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormImportaCurvaLote.h"
#include "TAplCfg.h"
#include "..\Apl\VTApl.h"
#include "..\BD\Rede\VTBDGeral.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\DirArq.h"
#include "..\Diretorio\VTPath.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------

#include <ImportacaoCurvaTipica\TImportaArquivoCurvaTipica.h>

// ---------------------------------------------------------------------------
__fastcall TFormImportaCurvaLote::TFormImportaCurvaLote(TComponent* Owner, TAplCfg *aplcfg)
	: TForm(Owner)
{
	// salva ponteiro p/ objeto
	this->aplcfg = aplcfg;
	this->apl = (VTApl*) aplcfg;
	curvas = (VTCurvas*)aplcfg->GetObject(__classid(VTCurvas));
	// cria lista das redes inseridas
	slisREDESINC = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TFormImportaCurvaLote::~TFormImportaCurvaLote(void)
{
	// destr�i lista
	if (slisREDESINC)
	{
		delete slisREDESINC;
		slisREDESINC = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ActionAjudaExecute(TObject *Sender)
{
	// Vari�veis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Configura��o_Curvas_Tipicas");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ActionConfirmaExecute(TObject *Sender)
{
	// Vari�veis locais
	VTProgresso* progresso = DLL_NewObjProgresso(this, NULL);
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString filename, curvas_fileName, ult_arq_rede;
	VTCurva *curva;
	bool checked, sucesso, boolcurva, boolrede;

	checked = false;
	curvas_fileName = DiretorioCurvas->Text;
	// apl->Add((VTProgresso*)DLL_NewObjProgresso(NULL, NULL));
	// No caso de marcar para "redes futuras" ele vai mudar o rede empresa
	checked = ChRedeFut->Checked;
	IncluiArqRedeEmpresa(checked);
	// Prote��o
	boolcurva = boolrede = true;
	if (curvas_fileName.IsEmpty())
	{
		boolcurva = false;
	}
	if (slisREDESINC->Count == 0)
	{
		boolrede = false;
	}
	// valida
	if (!boolcurva)
	{
		Aviso("Deve ser selecionado o arquivo de curvas a ser importado");
		return;
	}
	if (!boolrede)
	{
		Aviso("Deve ser selecionado pelo menos um arquivo de rede a ser alterado");
		return;
	}

	// pede confirma��o do usu�rio
	int resp =
		Confirma(
		"Todas curvas t�picas existentes nas redes selecionadas ser�o exclu�das e substitu�das pelas novas.",
		"Confirma a importa��o em lote?", MB_YESNO);

	if (resp != IDYES)
	{
		Show();
		return;
	}

	// Esconde este form
	Hide();

	// inicia progresso
	if (progresso)
		progresso->Start(progTEXTO);
	// salva ultimo arquivo de rede usado
	ult_arq_rede = path->ArqRede;
	// CRIA��O DO OBJETO PARA IMPORTA��O DE CURVAS
	TImportaArquivoCurvaTipica *importaArquivo = new TImportaArquivoCurvaTipica(this->aplcfg);
	if (progresso)
		progresso->Add("Importa��o das curvas t�picas nas redes selecionadas");
	for (int n = 0; n < slisREDESINC->Count; n++)
	{
		filename = slisREDESINC->Strings[n];
		path->ArqRede = filename;
		if (progresso)
		{
			if (filename.Pos(path->ArqRedeEmpresa()) <= 0)
				progresso->Add("Leitura da rede: " + ExtractFileName(filename));
			else
				progresso->Add("Leitura da base para as redes futuras");
		}
		aplcfg->LeBaseCurvaTipica();
		if (progresso)
			progresso->Add("Importa��o das curvas na rede");
		int countCurvasSalvas = 0;
		try
		{
			countCurvasSalvas = importaArquivo->Importar(curvas_fileName);
			sucesso = (countCurvasSalvas > 0);
		}
		catch (const runtime_error& e)
		{
			Erro((AnsiString)e.what());
			// recupera ultimo arquivo de rede usado
			path->ArqRede = ult_arq_rede;
			Show();
			return;
		}
		if (progresso)
		{
			if (countCurvasSalvas > 0)
				progresso->Add(IntToStr(countCurvasSalvas) + " curvas foram importadas.");
			else
				progresso->Add("Nenhuma curva foi adicionada !");
		}
	}
	// Apaga o objeto importa arquivo
	// delete importaArquivo;
	// importaArquivo = NULL;
	// excluir o arq rede empresa - no caso de ter sido selecionado
	ExcluiArqRedeEmpresa(checked);
	// recupera ultimo arquivo de rede usado
	path->ArqRede = ult_arq_rede;
	if (progresso)
		progresso->Stop();
	if (sucesso)
	{
		Aviso("Curvas em lote importadas com sucesso !");
	}
	else
	{
		Aviso("Erro ao importar curvas em lote");
	}
	// fecha form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ActionExcluirRedeExecute(TObject *Sender)
{
	// Vari�veis loca�s
	AnsiString filename, nome_arquivo;

	// determina Rede selecionada
	if (LBoxRedes->ItemIndex < 0)
		return;

	nome_arquivo = LBoxRedes->Items->Strings[LBoxRedes->ItemIndex];
	for (int n = 0; n < slisREDESINC->Count; n++)
	{
		filename = slisREDESINC->Strings[n];
		if (filename.Pos(nome_arquivo) > 0)
		{
			slisREDESINC->Delete(slisREDESINC->IndexOf(filename));
			break;
		}
	}
	AtualizaListaRedes();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ActionFechaExecute(TObject *Sender)
{
	// fecha
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ActionInsereRedeExecute(TObject *Sender)
{
	// Vari�veis locais
	AnsiString filename, arq_rede;

	// solicita o arquivo a ser importado
	OpenDialog->Filter = "Base de dados do SINAP (*.mdb)|*.mdb|";
	OpenDialog->Options << ofAllowMultiSelect << ofFileMustExist;
	if (OpenDialog->Execute())
	{ // verifica se o usuario selecionou o arquivo
		if (OpenDialog->Files->Count == 0)
		{
			Aviso("Selecione o arquivo a ser importado!");
			Show();
			return;
		}
		for (int n = 0; n < OpenDialog->Files->Count; n++)
		{
			filename = OpenDialog->Files->Strings[n];
			// verifica se o usuario selecionou o arquivo
			if (OpenDialog->FileName.Length() == 0)
			{
				Aviso("Selecione o arquivo a ser importado!");
				Show();
				return;
			}
			for (int n = 0; n < slisREDESINC->Count; n++)
			{
				arq_rede = slisREDESINC->Strings[n];
				if (arq_rede.Pos(filename) > 0)
				{
					Aviso("Esse arquivo de rede j� foi selecionado!");
					Show();
					return;
				}
			}
			slisREDESINC->Add(filename);
		}
		AtualizaListaRedes();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ActionSelecionaExecute(TObject *Sender)
{
	// Vari�veis locais
	AnsiString fileName;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// Oculta o form para o Confirma ser exibido(evita o conflito entre 2 modal)
	Hide();
	// solicita o arquivo a ser importado
	SaveDialog->Filter = "Comma-separated values (*.csv)|*.csv|Todos Arquivos (*.*)|*.*";
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirImporta();
	if (SaveDialog->Execute())
	{ // verifica se o usuario selecionou o arquivo
		fileName = SaveDialog->FileName;
		if (fileName.IsEmpty())
		{
			Aviso("Selecione o arquivo a ser importado!");
			Show();
			return;
		}
		DiretorioCurvas->Text = fileName;
	}
	// exibe o form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::AtualizaCurvas(void)
{
	// capacitor
	SalvaCurvas(curvas->ListaCurvaTipicaCapacitor());
	// carga
	SalvaCurvas(curvas->ListaCurvaTipicaCarga());
	// gerador
	SalvaCurvas(curvas->ListaCurvaTipicaGeradorFotovoltaico());
	// ip
	SalvaCurvas(curvas->ListaCurvaTipicaIP());
	// reator
	SalvaCurvas(curvas->ListaCurvaTipicaReator());
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::AtualizaListaRedes(void)
{
	// vari�vies locais
	AnsiString nome_arquivo;

	// reinicia LView
	LBoxRedes->Clear();
	// preenche LView
	for (int n = 0; n < slisREDESINC->Count; n++)
	{
		nome_arquivo = ExtractFileName(slisREDESINC->Strings[n]);
		LBoxRedes->Items->Add(nome_arquivo);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::ExcluiArqRedeEmpresa(bool checked)
{
	// Vari�veis locais
	AnsiString filenames;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// A a��o de exclus�o do Arq rede � feita se foi incluida no come�o (checked)
	if (!checked)
		return;

	for (int n = 0; n < slisREDESINC->Count; n++)
	{
		filenames = slisREDESINC->Strings[n];
		if (filenames.Pos(path->ArqRedeEmpresa()) > 0)
		{
			slisREDESINC->Delete(slisREDESINC->IndexOf(path->ArqRedeEmpresa()));
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::IncluiArqRedeEmpresa(bool checked)
{
	// Vari�veis locais
	AnsiString filenames;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// A a��o de inclus�o do Arq rede � feita se foi incluida no come�o (checked)
	if (!checked)
		return;

	for (int n = 0; n < slisREDESINC->Count; n++)
	{
		filenames = slisREDESINC->Strings[n];
		if (filenames.Pos(path->ArqRedeEmpresa()) > 0)
		{
			Aviso("Esse arquivo de rede j� foi selecionado !");
			return;
		}
	}
	slisREDESINC->Add(path->ArqRedeEmpresa());
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportaCurvaLote::SalvaCurvas(TList *lisCURVA)
{
	// vari�veis locais
	VTCurva *curva;

	for (int n = 0; n < lisCURVA->Count; n++)
	{
		curva = (VTCurva*)lisCURVA->Items[n];
		if (curva->Status[sttNOVO] != true)
			continue;
		aplcfg->SalvaBaseCurvaTipica(curva);
	}
}

// ---------------------------------------------------------------------------
