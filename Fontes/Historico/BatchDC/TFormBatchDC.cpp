// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormBatchDC.h"
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\BD.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Help.h>
#include <DLL_Inc\Perdatec.h>
#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Rede\VTBDgeral.h>
#include <Fontes\BD\Hist\VTBDhist.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Radial\VTArvore.h>
#include "VTBatchDC.h"
#include "Arquivos\TFormSelArqMedicao.h"
#include "Arquivos\TFormSelArqRede.h"
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\VTArquivos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormBatchDC::TFormBatchDC(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	// vari�veis locais
	VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));

	// cria objeto Apl local
	apl = NewObjApl(this, apl_owner);
	// cria objetos locais
	apl->Add(arquivos = NewObjArquivos(apl));
	apl->Add(batch_dc = NewObjBatchDC(apl));
	apl->Add(ajusta = DLL_NewObjAjustaTrifasico(apl));
    ajusta->ProgressoEnabled = false;
	// cria Forms para sele��o de arquivos de arquivos de medi��o
	FormSelArqMedicao = new TFormSelArqMedicao(this, apl, TabSheetArqMedicao);
	FormSelArqMedicao->FormPai(this);
	FormSelArqMedicao->Show();
	// cria Forms para sele��o de arquivos de arquivos de medi��o
	FormSelArqRede = new TFormSelArqRede(this, apl, TabSheetArqRede);
	FormSelArqRede->FormPai(this);
	FormSelArqRede->Show();
	// esconde TabSheetProgresso
	TabSheetProgresso->TabVisible = false;
	// inicia DateTimePickerIni e DateTimePickerFim com data atual
	DateTimePickerIni->Date = DateOf(Now());
	DateTimePickerFim->Date = DateOf(Now());
	// habilita ajuste de demanda
	CheckBoxAjusteDemanda->Checked = true;
	// exibe base de hist�rico padr�o
	EditBaseHistorico->Text = ExtractFileName(path->ArqHistorico);
	// inicia arquivos
	ReiniciaArquivos();
	// inicia posi��o
	FormIniciaPosicao();
	// seleciona TabSheet
	PageControl->ActivePage = TabSheetProgresso;
	PageControl->ActivePage = TabSheetOpcao;
	//inicia variavel
	executando = false;
}

// ---------------------------------------------------------------------------
__fastcall TFormBatchDC::~TFormBatchDC(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destr�i Forms criados
	if (FormSelArqMedicao)
	{
		delete FormSelArqMedicao;
		FormSelArqMedicao = NULL;
	}
	if (FormSelArqRede)
	{
		delete FormSelArqRede;
		FormSelArqRede = NULL;
	}
	// destr�i objetos locais
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ActionCfgAjusteDemandaExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;

	// esconde este Form  p/ n�o esconde janela de sele��o do arquivo
	Visible = false;
	try
	{ // cria FormCfgDemanda e exibe como janela modal
		if ((Form = DLL_NewFormCfgDemanda(this, ajusta)) != NULL)
		{
			Form->ShowModal();
			// destr�i o Form
			delete Form;
		}
	}
	catch (Exception &e)
	{
		Aviso("N�o foi poss�vel configurar os par�metros para ajuste de demanda");
	}
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ActionCriaBaseHistoricoExecute(TObject *Sender)
{
	// esconde este Form  p/ n�o esconde janela de sele��o do arquivo
	Visible = false;
	// cria base
	CriaBaseAccessHistorico();
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ActionExecutarExecute(TObject *Sender)
{
	// vari�veis locais
	AnsiString msg;

    //verifica se est� executando
	if(executando)
		return;
	// verifica vers�o Demo
	if (!ValidaLicenca())
		return;
	// verifica as op��es selecionadas
	if (!VerificaOpcoes())
		return;
	// verifica se todas as redes foram selecionadas
	if (!ValidaArquivosSelecionados())
		return;
	// verifica se a data selecionada j� existe nas bases selecionadas
	if (!ValidaDatasSelecionadas())
		return;
	//altera variavel para true
	executando = true;
	// exibe e seleciona TabSheetProgresso
	TabSheetProgresso->TabVisible = true;
	PageControl->ActivePage = TabSheetProgresso;
	// executa o c�lculo de densidade de carga
	batch_dc->AjusteDemandaEnabled = CheckBoxAjusteDemanda->Checked;
	batch_dc->DataIni = DateOf(DateTimePickerIni->Date);
	batch_dc->DataFim = DateOf(DateTimePickerFim->Date);
	if (batch_dc->Executa(ListBox))
		msg = "Mapa de densidade de carga calculado corretamente";
	else
		msg = "Erro no c�lculo do mapa de densidade de carga";
	// seleciona TabSheetOpcao
	// PageControl->ActivePage = TabSheetOpcao;
	// esconde TabSheetProgresso
	// TabSheetProgresso->TabVisible = false;
	// atualiza ListBox
	ListBox->Items->Add(msg);
	// mostra avio ao usu�rio
	Aviso(msg);
	//altera variavel para false
	executando = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ActionFechaExecute(TObject *Sender)
{
	// fecha o Form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Opera��o_Em_Lote");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ActionSelBaseHistoricoExecute(TObject *Sender)
{
	// esconde este Form  p/ n�o esconder janela de sele��o do arquivo
	Visible = false;
	// solicita sele��o de arquivos
	SelecionaBaseHistorico();
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::CheckBoxAjusteDemandaClick(TObject *Sender)
{
	// habilita/desabilita sele��o de arquivo de medi��o
	TabSheetArqMedicao->Visible = CheckBoxAjusteDemanda->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::CriaBaseAccessHistorico(void)
{
	// variaveis locais
	WideString hist_model, hist_novo;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// inicia dialog
	SaveDialog->Title = "Nova Base de Hist�rico";
	SaveDialog->InitialDir = path->DirHistorico();
	SaveDialog->DefaultExt = ".mdb";
	SaveDialog->Filter = "BaseHistorico (*.mdb)|*.mdb|Todos(*.*)|*.*";
	if (SaveDialog->Execute())
	{
		hist_novo = SaveDialog->FileName;
		// copia base de dado modelo (vazio) para o Historico Novo
		hist_model = path->DirHistorico() + "\\HistoricoVazio.mdb";
		CopyFile(hist_model.c_bstr(), hist_novo.c_bstr(), false);
	}
	// atualiza BasePerdaTecnica
	path->ArqHistorico = hist_novo;
	// atualiza EditBaseHist
	EditBaseHistorico->Text = ExtractFileName(path->ArqHistorico);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::DateTimePickerFimChange(TObject *Sender)
{
	// verifica consist�ncia com DateTimePickerIni
	if (DateTimePickerIni->Date > DateTimePickerFim->Date)
	{ // altera DateTimePickerIni
		DateTimePickerIni->Date = DateTimePickerFim->Date;
	}
	// descarta arquivos fora do per�odo
	ReiniciaArquivos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::DateTimePickerIniChange(TObject *Sender)
{
	// verifica consist�ncia com DateTimePickerFim
	if (DateTimePickerFim->Date < DateTimePickerIni->Date)
	{ // altera DateTimePickerIni
		DateTimePickerFim->Date = DateTimePickerIni->Date;
	}
	// descarta arquivos fora do per�odo
	ReiniciaArquivos();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchDC::ExisteArvoreNaBase(int tipo, TDateTime data_ini, TDateTime data_fim)
{
	// vari�veis locais
	bool existe = false;
	int num_meses;
	TList *lisARVORE;
	VTArvore *arvore;
	VTBD *bd;

	// cria objetos locais
	bd = DLL_NewObjBD(apl);
	lisARVORE = new TList();
	// prote��o
	if ((bd != NULL) && (lisARVORE != NULL))
	{ // obt�m lista de �rvores da base dedados
		if (bd->ConectaBaseHist())
		{
			bd->Hist->LeListaArvore(lisARVORE);
			// loop para todas Arvores
			for (int n = 0; n < lisARVORE->Count; n++)
			{
				arvore = (VTArvore*)lisARVORE->Items[n];
				// verifica o tipo da �rvore
				if ((arvore->TipoArvore & tipo) == tipo)
					// verifica data da Arvore
					if ((arvore->Data >= data_ini) && (arvore->Data <= data_fim))
					{
						existe = true;
						break;
					}
			}
		}
	}
	// destr�i lista e seus objetos
	if (lisARVORE)
	{
		LimpaTList(lisARVORE);
		delete lisARVORE;
	}
	// destr�i objeto BD
	if (bd)
	{
		bd->Desconecta();
		delete bd;
	}

	return (existe);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::FormIniciaPosicao(void)
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
	Left = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::IniciaArquivos(void)
{
	// vari�veis locais
	int num_meses;
	AnsiString txt;
	struct
	{
		int mes, ano;
	} data_ini, data_fim;

	// inicia conjunto de ArqMedicao para o per�odo: um por m�s
	arquivos->IniciaArqMedicao(DateTimePickerIni->Date, DateTimePickerFim->Date);
	// verifica se est� usando rede de um �nico m�s ou de cada m�s
	if (RadioGroupTopologia->ItemIndex == 0)
	{ // inicia um �nico ArqRede para o per�odo
		arquivos->IniciaArqRedeUnico(DateTimePickerIni->Date, DateTimePickerFim->Date);
	}
	else
	{ // inicia ArqRede para o per�odo: um por m�s
		arquivos->IniciaArqRedePorMes(DateTimePickerIni->Date, DateTimePickerFim->Date);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::PageControlChange(TObject *Sender)
{
	// verifica TabSheet atual
	if (PageControl->ActivePage == TabSheetOpcao)
	{ // nada a fazer
	}
	if (PageControl->ActivePage == TabSheetArqRede)
	{ // atualiza TView de ArqMedicao
		FormSelArqRede->TViewInicia();
	}
	if (PageControl->ActivePage == TabSheetArqMedicao)
	{ // atualiza TView de ArqRede
		FormSelArqMedicao->TViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::PageControlChanging(TObject *Sender, bool &AllowChange)
{
	// assume que altera��o � permitida
	AllowChange = true;
	// verifica TabSheet atual
	if (PageControl->ActivePage == TabSheetOpcao)
	{ // s� permite altera��o se as op��es estiverem definidas
		AllowChange = VerificaOpcoes();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::RadioGroupTopologiaClick(TObject *Sender)
{
	// elimina arquivos fora do per�odo selecionado
	ReiniciaArquivos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::ReiniciaArquivos(void)
{
	// remonta conjunto de ArqMedicao para o per�odo selecionado
	arquivos->IniciaArqMedicao(DateTimePickerIni->Date, DateTimePickerFim->Date);
	// remonta conjunto de ArqRede para o per�odo selecionado
	if (RadioGroupTopologia->ItemIndex == 0)
		arquivos->IniciaArqRedeUnico(DateTimePickerIni->Date, DateTimePickerFim->Date);
	else
		arquivos->IniciaArqRedePorMes(DateTimePickerIni->Date, DateTimePickerFim->Date);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchDC::SelecionaBaseHistorico(void)
{
	// vari�veis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	OpenDialog->Title = "Sele��o de Base de Hist�rico";
	OpenDialog->InitialDir = path->DirHistorico();
	// define tipo de arquivo a ser lido
	OpenDialog->Filter = "Base Historico (*.mdb)|*.mdb|Todos(*.*)|*.*";
	// exibe OpenTextFileDialog
	if (OpenDialog->Execute())
	{ // salva base selecionada
		if (OpenDialog->Files->Count >= 1)
		{
			path->ArqHistorico = OpenDialog->Files->Strings[0];
			EditBaseHistorico->Text = ExtractFileName(path->ArqHistorico);
		}
		else
		{
			Aviso("Selecione uma base de dados Historico");
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchDC::ValidaArquivosSelecionados(void)
{
	// vari�veis locais
	TArqMedicao *arq_medicao;
	TArqRede *arq_rede;
	TList *lisEXT;

	// verifica se todos ArqRede foram seleciondados
	lisEXT = arquivos->LisArqRede();
	for (int n = 0; n < lisEXT->Count; n++)
	{
		arq_rede = (TArqRede*)lisEXT->Items[n];
		if (!arq_rede->Enabled)
		{
			Aviso("Selecione os arquivos de rede: " + arq_rede->DataAsString());
			return (false);
		}
		if (FormSelArqRede->RadioGroupTipoArqRede->ItemIndex == 1)
		{ // valida egrid
			if (arq_rede->files->Count > 1)
			{
				Aviso("Selecione apenas 1 arquivo egrid: " + arq_rede->DataAsString());
				return (false);
			}
			if (arq_rede->lisSub->Count == 0)
			{
				Aviso("Selecione as subesta��es e prim�rios do egrid: " + arq_rede->DataAsString());
				return (false);
			}
		}
	}
	// verifica se todos ArqMedicao foram seleciondados
	if (CheckBoxAjusteDemanda->Checked)
	{
		lisEXT = arquivos->LisArqMedicao();
		for (int n = 0; n < lisEXT->Count; n++)
		{
			arq_medicao = (TArqMedicao*)lisEXT->Items[n];
			if (!arq_medicao->Enabled)
			{
				Aviso("Selecione o arquivo de medi��o: " + arq_medicao->DataAsString());
				return (false);
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchDC::ValidaDatasSelecionadas(void)
{
	// verifica se a data selecionada existe na base de densidade de carga
	if (ExisteArvoreNaBase(arvoreDENSIDADE, DateTimePickerIni->Date, DateTimePickerFim->Date))
	{ // pede confirma��o do usu�rio
		if (Confirma("J� existe base de densidade de carga para a data indicada.",
			"Deseja incluir a densidade de carga das redes selecionadas na mesma data ?") != IDYES)
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchDC::ValidaLicenca(void)
{
	// vari�veis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	// verifica se � uma Licenca demo
	if (licencas->LicencaTipoDemo())
	{
		Aviso("As bases de dados n�o podem ser geradas pela Licen�a de Uso DEMO");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchDC::VerificaOpcoes(void)
{
	// verifica se a base de hist�rico est� selecionada
	if (EditBaseHistorico->Text.IsEmpty())
	{
		Aviso("Selecione a base de perdas a ser atualizada");
		return (false);
	}
	// verifica se data inicial e final est�o definidas corretamente
	if (DateTimePickerIni->Date > DateTimePickerFim->Date)
	{
		Aviso("Per�odo de an�lise incorreto");
		return (false);
	}
	// verifica se op��o de topologia est� definida
	if (RadioGroupTopologia->ItemIndex < 0)
	{
		Aviso("Selecione op��o de topologia das redes");
		return (false);
	}
	/*
	 //verifica se op��o de ajuste de demanda est� selecionada e o tipo de medi��o
	 if ((CheckBoxAjusteDemanda->Checked)&&(RadioGroupTipoMedicao->ItemIndex < 0))
	 {
	 Aviso("Selecione tipo de arquivo com dados de medi��o");
	 return(false);
	 }
	 //salva op��es do usu�rio
	 opcao.data_ini.mes           = ;
	 opcao.data_ini.ano           = ;
	 opcao.data_fim.mes           = ;
	 opcao.data_fim.ano           = ;
	 opcao.rede_unica             = ;
	 opcao.tipo_arq_rede          = ;
	 opcao.ajuste_demanda_enabled = ;
	 opcao.tipo_arq_medicao       = ;
	 */
	return (true);
}

// ---------------------------------------------------------------------------
// eof
