// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
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
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Perdatec\VTPerdatec.h>
#include <Fontes\Radial\VTArvore.h>
#include "TFormBatchPT.h"
#include "VTBatchPT.h"
#include "Arquivos\TFormSelArqMedicao.h"
#include "Arquivos\TFormSelArqRede.h"
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\VTArquivos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormBatchPT::TFormBatchPT(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));

	// cria objeto Apl local
	apl = NewObjApl(this, apl_owner);
	// cria objetos locais
	apl->Add(arquivos = NewObjArquivos(apl));
	apl->Add(batch_pt = NewObjBatchPT(apl));
	apl->Add(ajusta = DLL_NewObjAjustaTrifasico(apl));
    ajusta->ProgressoEnabled = false;
	// cria Forms para seleção de arquivos de arquivos de medição
	FormSelArqMedicao = new TFormSelArqMedicao(this, apl, TabSheetArqMedicao);
	FormSelArqMedicao->FormPai(this);
	FormSelArqMedicao->Show();
	// cria Forms para seleção de arquivos de arquivos de medição
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
	// exibe base de histórico padrão
	EditBaseHistorico->Text = ExtractFileName(path->ArqHistorico);
	// inicia arquivos
	ReiniciaArquivos();
	// inicia posição
	FormIniciaPosicao();
	// seleciona TabSheet
	PageControl->ActivePage = TabSheetProgresso;
	PageControl->ActivePage = TabSheetOpcao;
	//inicia variavel
	executando = false;
}

// ---------------------------------------------------------------------------
__fastcall TFormBatchPT::~TFormBatchPT(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói Forms criados
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
	// destrói objetos locais
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionCfgAjusteDemandaExecute(TObject *Sender)
{
	// variáveis locais
	TForm *Form;

	// esconde este Form  p/ não esconde janela de seleção do arquivo
	Visible = false;
	try
	{ // cria FormCfgDemanda e exibe como janela modal
		if ((Form = DLL_NewFormCfgDemanda(this, ajusta)) != NULL)
		{
			Form->ShowModal();
			// destrói o Form
			delete Form;
		}
	}
	catch (Exception &e)
	{
		Aviso("Não foi possível configurar os parâmetros para ajuste de demanda");
	}
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionCfgPerdaExecute(TObject *Sender)
{
	// variáveis locais
	TForm *Form;

	// esconde este Form  p/ não esconde janela de seleção do arquivo
	Visible = false;
	try
	{ // cria FormCfgPertec como janela modal
		Form = DLL_NewFormCfgPerdatec(this, apl);
		Form->ShowModal();
		// destrói Form
		delete Form;
		// atualiza controle p/ indicar que usuário configurou opções de perdas
		ActionCfgPerda->Tag = 1;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionCriaBaseHistoricoExecute(TObject *Sender)
{
	// esconde este Form  p/ não esconde janela de seleção do arquivo
	Visible = false;
	// cria base
	CriaBaseAccessHistorico();
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionExecutarExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString msg;

    //verifica se está executando
	if(executando)
		return;
	// verifica versão Demo
	if (!ValidaLicenca())
		return;
	// verifica se foi feita configuração das opções de perdas
	if (!UsuarioConfigurouPerdas())
		return;
	// verifica as opções selecionadas
	if (!VerificaOpcoes())
		return;
	// verifica se todas as redes foram selecionadas
	if (!ValidaArquivosSelecionados())
		return;
	// verifica se a data selecionada já existe nas bases selecionadas
	if (!ValidaDatasSelecionadas())
		return;
	//altera variavel para true
	executando = true;
	// exibe e seleciona TabSheetProgresso
	LBoxLogPasso->Clear();
	LBoxLogPasso->Clear();
	TabSheetProgresso->TabVisible = true;
	PageControl->ActivePage = TabSheetProgresso;
	// executa o cálculo de perdas
	batch_pt->AjusteDemandaEnabled = CheckBoxAjusteDemanda->Checked;
	batch_pt->DataIni = DateOf(DateTimePickerIni->Date);
	batch_pt->DataFim = DateOf(DateTimePickerFim->Date);
	batch_pt->LBoxLogRede = LBoxLogRede;
	batch_pt->LBoxLogPasso = LBoxLogPasso;
	if (batch_pt->Executa())
		msg = "Cálculo de perdas concluído com sucesso";
	else
		msg = "Erro no cálculo de perdas";
	// seleciona TabSheetOpcao
	// PageControl->ActivePage = TabSheetOpcao;
	// esconde TabSheetProgresso
	// TabSheetProgresso->TabVisible = false;
	// atualiza ListBox
	LBoxLogPasso->Clear();
	LBoxLogPasso->Items->Add(msg);
	// mostra avio ao usuário
	Aviso(msg);
    //altera variavel para false
	executando = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionFechaExecute(TObject *Sender)
{
	// fecha o Form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Operação_Em_Lote");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ActionSelBaseHistoricoExecute(TObject *Sender)
{
	// esconde este Form  p/ não esconder janela de seleção do arquivo
	Visible = false;
	// solicita seleção de arquivos
	SelecionaBaseHistorico();
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::CheckBoxAjusteDemandaClick(TObject *Sender)
{
	// habilita/desabilita seleção de arquivo de medição
	TabSheetArqMedicao->Visible = CheckBoxAjusteDemanda->Checked;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormBatchPT::CriaBaseAccessHistorico(void)
 {
 //variaveis locais
 WideString  hist_model, hist_novo;
 VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));

 //inicia dialog
 SaveDialog->Title      = "Nova Base de Histórico";
 SaveDialog->InitialDir = path->DirHistorico();
 SaveDialog->DefaultExt = ".mdb";
 SaveDialog->Filter     = "BaseHistorico (*.mdb)|*.mdb|Todos(*.*)|*.*";
 if (SaveDialog->Execute())
 {
 hist_novo = SaveDialog->FileName;
 //copia base de dado modelo (vazio) para o Historico Novo
 hist_model = path->DirHistorico() + "\\HistoricoVazio.mdb";
 CopyFile(hist_model.c_bstr(), hist_novo.c_bstr(), false);
 }
 //atualiza BasePerdaTecnica
 path->ArqHistorico = hist_novo;
 //atualiza EditBaseHist
 EditBaseHistorico->Text = ExtractFileName(path->ArqHistorico);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::CriaBaseAccessHistorico(void)
{
	// variaveis locais
	WideString hist_model, hist_novo;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// inicia dialog
	SaveDialog->Title = "Nova Base de Histórico";
	SaveDialog->InitialDir = path->DirHistorico();
	SaveDialog->DefaultExt = ".mdb";
	SaveDialog->Filter = "BaseHistorico (*.mdb)|*.mdb|Todos(*.*)|*.*";
	if (SaveDialog->Execute())
	{ // verifica se uma base com mesmo nome já existe  //DVK 2016.07.27
		if (FileExists(SaveDialog->FileName))
		{
			switch (Confirma("Já existe uma base com mesmo nome.", "Deseja substituir o arquivo ?"))
			{
			case IDNO:
			case IDCANCEL:
				return;
			case IDYES:
				break;
			}
		}
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
void __fastcall TFormBatchPT::DateTimePickerFimChange(TObject *Sender)
{
	// verifica consistência com DateTimePickerIni
	if (DateTimePickerIni->Date > DateTimePickerFim->Date)
	{ // altera DateTimePickerIni
		DateTimePickerIni->Date = DateTimePickerFim->Date;
	}
	// descarta arquivos fora do período
	ReiniciaArquivos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::DateTimePickerIniChange(TObject *Sender)
{
	// verifica consistência com DateTimePickerFim
	if (DateTimePickerFim->Date < DateTimePickerIni->Date)
	{ // altera DateTimePickerIni
		DateTimePickerFim->Date = DateTimePickerIni->Date;
	}
	// descarta arquivos fora do período
	ReiniciaArquivos();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchPT::ExisteArvoreNaBase(int tipo, TDateTime data_ini, TDateTime data_fim)
{
	// varia´veis locais
	bool existe = false;
	int num_meses;
	TList *lisARVORE;
	VTArvore *arvore;
	VTBD *bd;

	// cria objetos locais
	bd = DLL_NewObjBD(apl);
	lisARVORE = new TList();
	// proteção
	if ((bd != NULL) && (lisARVORE != NULL))
	{ // obtém lista de Árvores da base dedados
		if (bd->ConectaBaseHist())
		{
			bd->Hist->LeListaArvore(lisARVORE);
			// loop para todas Arvores
			for (int n = 0; n < lisARVORE->Count; n++)
			{
				arvore = (VTArvore*)lisARVORE->Items[n];
				// verifica o tipo da árvore
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
	// destrói lista e seus objetos
	if (lisARVORE)
	{
		LimpaTList(lisARVORE);
		delete lisARVORE;
	}
	// destrói objeto BD
	if (bd)
	{
		bd->Desconecta();
		delete bd;
	}

	return (existe);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form canto superior esquerdo
	Top = 0;
	Left = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::IniciaArquivos(void)
{
	// variáveis locais
	int num_meses;
	AnsiString txt;
	struct
	{
		int mes, ano;
	} data_ini, data_fim;

	// inicia conjunto de ArqMedicao para o período: um por mês
	arquivos->IniciaArqMedicao(DateTimePickerIni->Date, DateTimePickerFim->Date);
	// verifica se está usando rede de um único mês ou de cada mês
	if (RadioGroupTopologia->ItemIndex == 0)
	{ // inicia um único ArqRede para o período
		arquivos->IniciaArqRedeUnico(DateTimePickerIni->Date, DateTimePickerFim->Date);
	}
	else
	{ // inicia ArqRede para o período: um por mês
		arquivos->IniciaArqRedePorMes(DateTimePickerIni->Date, DateTimePickerFim->Date);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::PageControlChange(TObject *Sender)
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
void __fastcall TFormBatchPT::PageControlChanging(TObject *Sender, bool &AllowChange)
{
	// assume que alteração é permitida
	AllowChange = true;
	// verifica TabSheet atual
	if (PageControl->ActivePage == TabSheetOpcao)
	{ // só permite alteração se as opções estiverem definidas
		AllowChange = VerificaOpcoes();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::RadioGroupTopologiaClick(TObject *Sender)
{
	// elimina arquivos fora do período selecionado
	ReiniciaArquivos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::ReiniciaArquivos(void)
{
	// remonta conjunto de ArqMedicao para o período selecionado
	arquivos->IniciaArqMedicao(DateTimePickerIni->Date, DateTimePickerFim->Date);
	// remonta conjunto de ArqRede para o período selecionado
	if (RadioGroupTopologia->ItemIndex == 0)
		arquivos->IniciaArqRedeUnico(DateTimePickerIni->Date, DateTimePickerFim->Date);
	else
		arquivos->IniciaArqRedePorMes(DateTimePickerIni->Date, DateTimePickerFim->Date);
}

/*
 //---------------------------------------------------------------------------
 void  __fastcall TFormBatchPT::SelecionaBaseHistorico(void)
 {
 //variáveis locais
 VTPath *path  = (VTPath*)apl->GetObject(__classid(VTPath));

 OpenDialog->Title      = "Seleção de Base de Histórico";
 OpenDialog->InitialDir = path->DirHistorico();
 //define tipo de arquivo a ser lido
 OpenDialog->Filter = "Base Historico (*.mdb)|*.mdb|Todos(*.*)|*.*";
 //exibe OpenTextFileDialog
 if (OpenDialog->Execute())
 {//salva base selecionada
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
 */
// ---------------------------------------------------------------------------
void __fastcall TFormBatchPT::SelecionaBaseHistorico(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq;

	OpenDialog->Title = "Seleção de Base de Histórico";
	OpenDialog->InitialDir = path->DirHistorico();
	// define tipo de arquivo a ser lido
	OpenDialog->Filter = "Base Historico (*.mdb)|*.mdb|Todos(*.*)|*.*";
	// exibe OpenTextFileDialog
	if (OpenDialog->Execute())
	{ // salva base selecionada
		if (OpenDialog->Files->Count < 1)
		{
			Aviso("Selecione uma base de dados Historico");
			return;
		}
		arq = OpenDialog->Files->Strings[0];
		// verifica se o arquivo existe //DVK 2016.07.27
		if (!FileExists(arq))
		{
			Aviso("A base selecionada não existe");
			return;
		}
		path->ArqHistorico = arq;
		EditBaseHistorico->Text = ExtractFileName(arq);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchPT::UsuarioConfigurouPerdas(void)
{
	// inicia controle: usuário precisa configurar de perdas
	if (ActionCfgPerda->Tag == 0)
	{
		Aviso("Selecione opções do cálculo de perdas");
	}
	return (ActionCfgPerda->Tag > 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchPT::ValidaArquivosSelecionados(void)
{
	// variáveis locais
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
				Aviso("Selecione as subestações e primários do egrid: " + arq_rede->DataAsString());
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
				Aviso("Selecione o arquivo de medição: " + arq_medicao->DataAsString());
				return (false);
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchPT::ValidaDatasSelecionadas(void)
{
	// verifica se a data selecionada existe na base de perdas técnicas
	if (ExisteArvoreNaBase(arvorePERDATEC, DateTimePickerIni->Date, DateTimePickerFim->Date))
	{ // pede confirmação do usuário
		if (Confirma("Já existe base de perdas técnicas para a data indicada.",
			"Deseja incluir as perdas das redes selecionadas na mesma data ?") != IDYES)
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchPT::ValidaLicenca(void)
{
	// variáveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	// verifica se é uma Licenca demo
	if (licencas->LicencaTipoDemo())
	{
		Aviso("As bases de dados não podem ser geradas pela Licença de Uso DEMO");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBatchPT::VerificaOpcoes(void)
{
	// verifica se a base de histórico está selecionada
	if (EditBaseHistorico->Text.IsEmpty())
	{
		Aviso("Selecione a base de perdas a ser atualizada");
		return (false);
	}
	// verifica se data inicial e final estão definidas corretamente
	if (DateTimePickerIni->Date > DateTimePickerFim->Date)
	{
		Aviso("Período de análise incorreto");
		return (false);
	}
	// verifica se opção de topologia está definida
	if (RadioGroupTopologia->ItemIndex < 0)
	{
		Aviso("Selecione opção de topologia das redes");
		return (false);
	}
	/*
	 //verifica se opção de ajuste de demanda está selecionada e o tipo de medição
	 if ((CheckBoxAjusteDemanda->Checked)&&(RadioGroupTipoMedicao->ItemIndex < 0))
	 {
	 Aviso("Selecione tipo de arquivo com dados de medição");
	 return(false);
	 }
	 //salva opções do usuário
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
