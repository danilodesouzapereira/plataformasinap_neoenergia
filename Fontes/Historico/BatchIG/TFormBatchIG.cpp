// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\BD.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Diretorio.h>
#include <DLL_Inc\Help.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include <DLL_Inc\Perdatec.h>
#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Rede\VTBDgeral.h>
#include <Fontes\BD\Hist\VTBDhist.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Perdatec\VTPerdatec.h>
#include <Fontes\Radial\VTArvore.h>
#include "TFormBatchIG.h"
#include "TFormSelArqMedicao.h"
#include "TFormSelArqRede.h"
#include "VTBatchIG.h"
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\VTArquivos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormBatchIG::TFormBatchIG(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	// cria objeto Apl local
	apl = NewObjApl(this, apl_owner);
	// cria objetos locais
	apl->Add(arquivos = NewObjArquivos(apl));
	apl->Add(batch_ig = NewObjBatchIG(apl)); // voltar depois...
	// apl->Add(batch_ig = NewObjBatchCurva(apl));
	apl->Add(ajusta = DLL_NewObjAjustaTrifasico(apl));
    ajusta->ProgressoEnabled = false;
	// cria Forms para seleção de arquivos de arquivos de medição
	FormSelArqMedicao = new TFormSelArqMedicao(this, apl, TabSheetArqMedicao);
	FormSelArqMedicao->FormPai(this);
	FormSelArqMedicao->Show();
	// cria Forms para seleção de arquivos de arquivos de rede
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
	// inicia arquivos
	ReiniciaArquivos();
	// inicia opcao default
	opcao.date = DateOf(Now());
	opcao.sub.topologia = opSUB_GIS;
	opcao.pri.consA4 = opENERGIA;
	opcao.fator_carga = 0.6;
	opcao.sec.topologia = opSEC_CARGA;
	opcao.valida_fase = true;
	opcao.corrige_fase = true;
	opcao.corrige_perda_fe = true;
	opcao.valida_eqpto = true;
	opcao.gerarTXT = false;
	opcao.ihm.log = false;
	opcao.ihm.progresso = true;
	// inicia posição
	FormIniciaPosicao();
	// seleciona TabSheet
	PageControl->ActivePage = TabSheetOpcao;
	PageControlChange(NULL);
	//inicia variavel
	executando = false;
}

// ---------------------------------------------------------------------------
__fastcall TFormBatchIG::~TFormBatchIG(void)
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
void __fastcall TFormBatchIG::ActionCfgAjusteDemandaExecute(TObject *Sender)
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
void __fastcall TFormBatchIG::ActionCfgImportaExecute(TObject *Sender)
{
	// variáveis locais
	TForm *Form;

	// esconde este Form  p/ não esconde janela de seleção do arquivo
	Visible = false;
	try
	{ // cria FormCfgImporta e exibe como janela modal
		if ((Form = DLL_NewFormCfgImporta(this, &opcao)) != NULL)
		{
			Form->ShowModal();
			// destrói o Form
			delete Form;
		}
	}
	catch (Exception &e)
	{
		Aviso("Não foi possível configurar os parâmetros para importação de rede");
	}
	// reapresenta este Form
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::ActionCriaDiretorioExecute(TObject *Sender)
{
	/*
	 //esconde este Form  p/ não esconde janela de seleção do arquivo
	 Visible = false;
	 //cria base
	 CriaDirRede();
	 //reapresenta este Form
	 Visible = true;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::ActionExecutarExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString dir_rede, msg;

	//verifica se está executando
	if(executando)
		return;
	// verifica versão Demo
	if (!ValidaLicenca())
		return;
	// verifica as opções selecionadas
	if (!VerificaOpcoes())
		return;
	// verifica se todas as redes foram selecionadas
	if (!ValidaArquivosSelecionados())
		return;
	//altera variavel para true
	executando = true;
	// define um diretório onde as redes serão salvas
	// DVK 2016.11.23 definido em TBatchIG::SalvaRedeNaBase
	// if (! DefineDiretorioRede(dir_rede)) return;
	// exibe e seleciona TabSheetProgresso
	TabSheetProgresso->TabVisible = true;
	PageControl->ActivePage = TabSheetProgresso;
	// executa a importação
	batch_ig->AjusteDemandaEnabled = CheckBoxAjusteDemanda->Checked;
	batch_ig->BaseUnica = (RadioGroupBaseDado->ItemIndex == 0);
	batch_ig->DirRede = dir_rede;
	batch_ig->DataIni = DateOf(DateTimePickerIni->Date);
	batch_ig->DataFim = DateOf(DateTimePickerFim->Date);
	batch_ig->Opcao = opcao;
	if (batch_ig->Executa(ListBox))
		msg = "Importação concluída com sucesso";
	else
		msg = "Erro na importação das redes";
	// seleciona TabSheetOpcao
	// PageControl->ActivePage = TabSheetOpcao;
	// esconde TabSheetProgresso
	// TabSheetProgresso->TabVisible = false;
	// atualiza ListBox
	ListBox->Items->Add(msg);
	// mostra aviso ao usuário
	Aviso(msg);
	//altera variavel para false
    executando = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::ActionFechaExecute(TObject *Sender)
{
	// fecha o Form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Operação_Em_Lote");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::ActionSelDiretorioExecute(TObject *Sender)
{
	/*
	 //esconde este Form  p/ não esconder janela de seleção do arquivo
	 Visible = false;
	 //seleciona diretório para armazenar as bases de dados
	 SelecionaDirRede();
	 //reapresenta este Form
	 Visible = true;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::CheckBoxAjusteDemandaClick(TObject *Sender)
{
	// habilita/desabilita seleção de arquivo de medição
	TabSheetArqMedicao->Visible = CheckBoxAjusteDemanda->Checked;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormBatchIG::CriaDirRede(void)
 {
 //variáveis locais
 AnsiString dirname;
 VTDir      *dir  = (VTDir*)apl->GetObject(__classid(VTDir));
 VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

 try{//cria FormCriaDir e exibe como janela modal
 if (dir->CriaDiretorio(path->DirBase, dirname))
 {//salva nome do diretório
 dir_rede = dirname;
 //exibe pasta selecionada
 EditDirRede->Text = ExtractFileName(dir_rede);
 }
 }catch (Exception &e)
 {
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::DateTimePickerFimChange(TObject *Sender)
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
void __fastcall TFormBatchIG::DateTimePickerIniChange(TObject *Sender)
{
	// verifica consistência com DateTimePickerFim
	if (DateTimePickerFim->Date < DateTimePickerIni->Date)
	{ // altera DateTimePickerIni
		DateTimePickerFim->Date = DateTimePickerIni->Date;
	}
	// descarta arquivos fora do período
	ReiniciaArquivos();
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormBatchIG::DefineDiretorioRede(AnsiString &dir_rede)
 {
 //variáveis locais
 int     count = 1;
 VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));

 //loop p/ definir o nome de um diretório ainda não existente
 do {
 dir_rede = path->DirBase + "\\Redes_" + IntToStr(count++);
 }while (DirectoryExists(dir_rede));
 //cria diretório
 CreateDir(dir_rede);

 return(true);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::FormIniciaPosicao(void)
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
void __fastcall TFormBatchIG::IniciaArquivos(void)
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
	// inicia conjunto de ArqRede para o período: um por mês
	arquivos->IniciaArqRedePorMes(DateTimePickerIni->Date, DateTimePickerFim->Date);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchIG::PageControlChange(TObject *Sender)
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
void __fastcall TFormBatchIG::PageControlChanging(TObject *Sender, bool &AllowChange)
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
void __fastcall TFormBatchIG::ReiniciaArquivos(void)
{
	// remonta conjunto de ArqMedicao para o período selecionado
	arquivos->IniciaArqMedicao(DateTimePickerIni->Date, DateTimePickerFim->Date);
	// remonta conjunto de ArqRede para o período selecionado
	arquivos->IniciaArqRedePorMes(DateTimePickerIni->Date, DateTimePickerFim->Date);
}

/*
 //---------------------------------------------------------------------------
 void  __fastcall TFormBatchIG::SelecionaDirRede(void)
 {
 //variáveis locais
 AnsiString dirname;
 VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));
 VTDir      *dir  = (VTDir*)apl->GetObject(__classid(VTDir));

 //seleciona disrtório
 if (dir->SelDir(dirname))
 {//verifica se o diretório existe
 if (DirectoryExists(dirname))
 {//salva nome do diretório
 dir_rede = dirname;
 //exibe pasta selecionada
 EditDirRede->Text = ExtractFileName(dir_rede);
 }
 else
 {
 Aviso("A pasta selecionada não existe");
 return;
 }
 }
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormBatchIG::ValidaArquivosSelecionados(void)
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
		{// valida egrid
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
bool __fastcall TFormBatchIG::ValidaLicenca(void)
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
bool __fastcall TFormBatchIG::VerificaOpcoes(void)
{
	// verifica se data inicial e final estão definidas corretamente
	if (DateTimePickerIni->Date > DateTimePickerFim->Date)
	{
		Aviso("Período de análise incorreto");
		return (false);
	}
	// verifica se opção de topologia está definida
	if (RadioGroupBaseDado->ItemIndex < 0)
	{
		Aviso("Selecione opção de base de dados do Sinap");
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
