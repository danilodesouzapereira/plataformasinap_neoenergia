// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------------------- includes
#include <algorithm>
#include <DateUtils.hpp>
#include <Classes.hpp>
#include <TypInfo.hpp>
#include <map>

// Plataforma include
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Cartografia\VTCartografia.h>
#include <Fontes\Cartografia\VTScg.h>
#include <Fontes\Cronometro\VTCronometro.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\Cronometro\VTCronometro.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Sinap\VTSinapChild.h>
#include <DLL_Inc\Cartografia.h>
#include <DLL_Inc\Cronometro.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Geral.h>
#include <DLL_Inc\Help.h>
#include <DLL_Inc\Progresso.h>

// Project include
#include <Ugrid\VTConversor.h>
#include <Ugrid\VTImportador.h>
#include <Exceptions\EImportaUgrid.h>
#include <Commons\VTCommons.h>
#include <Commons\Entidades\TArranjoBDGD.h>
#include <Commons\Entidades\TBarraBDGD.h>
#include <Commons\Entidades\TBarramento.h>
#include <Commons\Entidades\TRedeMT.h>
#include <Commons\Entidades\TSubestacao.h>
#include <Commons\Entidades\TTrafoSubestacao.h>

#include <TFormImportadorUgrid.h>

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18496139
__fastcall TFormImportadorUgrid::TFormImportadorUgrid(TComponent *Owner, VTApl *apl_owner,
                                                      TWinControl *parent, VTImportador *importador)
                                :TForm(Owner)
{
    // variáveis locais
    VTSinapChild    *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));
    VTPath          *path;
    //TImportaBDGD já criou um VTDadosEmpresa
    conversor = (VTConversor*)apl_owner->GetObject(__classid(VTConversor));
    commons = (VTCommons*)apl_owner->GetObject(__classid(VTCommons));
    arqEgrid =  importador->ArqEgrid;
    // cria objeto Apl local
    apl = NewObjApl(this, apl_owner);
    this->importadorArquivo = importador;
    // // cria objeto Progresso
    progresso = (VTProgresso*)apl_owner->GetObject(__classid(VTProgresso));
    if (progresso != NULL)
    {
        apl->Add(progresso = DLL_NewObjProgresso(this, NULL));
    }
    // cria objeto Empresa
    if(conversor == NULL)
    {
        apl->Add(conversor = NewObjConversor(apl));
    }
    conversor->Importador = importador;
    if(commons == NULL)
    {
        apl->Add(commons = NewObjCommons(apl, NULL));
    }
    lisTMP = new TList();
    // inicia DateTimePicker c/ data atual
    DateTimePicker->Date = DateOf(Now());
    // exibe o gráfico de topologia
    if (sinap_child)
        sinap_child->ActionTopo->Execute();
    // Modifica nomes
    if (arqEgrid)
    {
        this->Caption = "Importador de arquivo .Egrid";
        btnAbrir->Caption = "Abrir arquivo .Egrid";
    }
    // posiciona Form
    FormIniciaPosicao();
    FormLeOpcoes();
    // pasta para exportar as novas BDs
    path = (VTPath*)apl->GetObject(__classid(VTPath));
    EditPastaPublica->Text = ExtractFilePath(path->ArqRede);
    // cria objeto VTCronometo
    cronometro = DLL_NewObjCronometro(apl);
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormImportadorUgrid::~TFormImportadorUgrid(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva informações da tela
	if (geral)
	{ // salva parâmetros do Form
		geral->Infoset->SaveToFile(this);
		FormSalvaOpcoes();
	}

	// destroi listas sem destruir objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}

	// remove objeto Cronometro do Apl
	if (apl)
		apl->Remove(cronometro);

	// destrói objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	if (cronometro)
	{
		delete cronometro;
		cronometro = NULL;
	}

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionAbrirUgridExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString fileName;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// oculta o form para exibir a caixa de dialogo de abrir arquivo
	Hide();
	if(arqEgrid)
	{
		dlgOpen->Filter = "extracted grid (*.Egrid)|*.Egrid|";
	}
	else
	{
		dlgOpen->Filter = "universal grid (*.Ugrid)|*.Ugrid|";
	}
	if (dlgOpen->InitialDir.IsEmpty())
	{
		dlgOpen->InitialDir = path->DirImporta();
	}
	if (dlgOpen->Execute())
	{
		fileName = dlgOpen->FileName;
	}
	// verifica se o usuario selecionou o arquivo
	if (fileName.IsEmpty())
	{
		Aviso("Selecione o arquivo a ser importado!");
		Show();
		return;
	}
	try
	{
		commons->Clear();
		conversor->Importador = importadorArquivo;
		conversor->ImportaCommons(fileName);
		// inicia CheckListBox de Subestação
		CLBoxSubestacaoInicia();
		// inicia LBoxPrimarioAEreo e Subterraneo
		CLBoxPrimarioInicia();
		// inicia CBox dos condutores
		CBoxCondutorInicia(CBoxCondutorBT, commons->GetmapArranjo(), 0);
		CBoxCondutorInicia(CBoxCondutorMT, commons->GetmapArranjo(), 0);
		CBoxCondutorInicia(CBoxCondutorIP, commons->GetmapArranjo(), 0);
		CBoxCondutorInicia(CBoxCondutorRL, commons->GetmapArranjo(), 0);
		// verifica se há alguma opção de "Não utilizar" um Condutor
		if ((CBoxCondutorMT->ItemIndex == 0) || (CBoxCondutorBT->ItemIndex == 0))
		{
			Aviso("Está selecionada opção de não utilizar condutor default\n"
				"em caso de trecho com condutor não definido");
		}
		// habilita o botao para importar as subestacoes
		ActionIniciarImportacao->Enabled = true;
		ActionSubestacaoSelAll->Enabled = true;
		ActionSubestacaoSelNone->Enabled = true;
		ActionSubestacaoSelAll->Visible = true;
	}
	catch (EImportaUgrid &e)
	{
		Erro("Erro ao importar dados da empresa, revise o (E-U)grid utilizado.");
		Show();
		if (progresso)
		{
			progresso->Stop();
		}
		return;
	}
//	catch (const runtime_error &e)
//	{
//		Erro((AnsiString)e.what());
//		Show();
//		if (progresso)
//		{
//			progresso->Stop();
//		}
//		return;
//	}
	// exibe o form
	Show();

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionFechaExecute(TObject *Sender)
{
	Close();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionHelpExecute(TObject *Sender)
{
	// variáveis locais
	TForm *FormHelp;

	// exibe arquivo de ajuda do módulo
	if ((FormHelp = DLL_NewFormHelp(this, apl, "Importador_de_redes_Ugrid")) != NULL)
	{
		FormHelp->Show();
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionIniciarImportacaoExecute(TObject *Sender)
{
	// variáveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	int count_rede = 0;

	lisTMP->Clear();
	// preenche lisTMP com as subestações selecionadas
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;

	// valida opções selecionadas
	if (!ValidaOpcoesDeImportacao())
		return;
	// btnImportar->Enabled = false;

	if (ImportaSubestacao())
	{ // guarda quantidade de redes importadas
		count_rede = redes->LisRede()->Count;
		// verifica se a opção de importação é só pra salvar as redes
		if (RadioGroupDestino->ItemIndex > 0)
		{ // limpa redes da memória
			Aviso("Importação concluída com sucesso");
			redes->Clear();
			count_rede = 0;
		}
		// exibe o gráfico de topologia
		grafico->Show(grafTOPO);
		// atualiza o gráfico
		grafico->AreaRedeDefine();
		// marca que o fluxo não está atualziado
		flow->UpToDate = false;
		// exibe resultado do Cronometro
		if (CheckBoxCronometro->Checked)
			cronometro->Show();
		// fecha o Form, indicando se há redes carregadas ou não
		if (count_rede > 0)
			ModalResult = mrOk;
		else
			ModalResult = mrCancel;
	}
}

// --------------------------------------------------------------------------------------------- eof
void __fastcall TFormImportadorUgrid::ActionPrimarioSelAllExecute(TObject *Sender)
{
	// variáveis locais
	TRedeMT* primario;

	// seleciona todos os Primarios
	for (int n = 0; n < CLBoxPrimario->Items->Count; n++)
	{
		CLBoxPrimario->Checked[n] = true;
		primario = (TRedeMT*)CLBoxPrimario->Items->Objects[n];
		primario->Selecionado = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionPrimarioSelNoneExecute(TObject *Sender)
{
	// variáveis locais
	TRedeMT* primario;

	// cancela seleção de todos os Primarios
	for (int n = 0; n < CLBoxPrimario->Items->Count; n++)
	{
		CLBoxPrimario->Checked[n] = false;
		primario = (TRedeMT*)CLBoxPrimario->Items->Objects[n];
		primario->Selecionado = false;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionRedeClearExecute(TObject *Sender)
{
	// var local
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// solicita aviso p/ apagar redes
	if (!ReiniciaRede(true))
		return;
	// atualiza o gráfico
	grafico->AreaRedeDefine();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionSelPastaPublicaExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString dirname;
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Visible = false;
	// verifica se existe uma pasta selecionada
	dirname = EditPastaPublica->Text;
	if (dirname.IsEmpty())
	{ // assume pasta default: Redes
		dirname = path->DirRedes();
	}
	// verifica se é uma pasta válida
	if (!DirectoryExists(dirname))
		dirname = "";
	// exibe janela para selecionar a pasta
	if (dir->SelDir(dirname))
		EditPastaPublica->Text = dirname;
	else
		EditPastaPublica->Text = "";
	// torna a exibir este Form
	Visible = true;
}

// --------------------------------------------------------------------------------------------- eof
// https://sinapsisenergia.teamwork.com/#/tasks/17945075
void __fastcall TFormImportadorUgrid::ActionSubestacaoSelAllExecute(TObject *Sender)
{
    // seleciona todas as Subestacoes
    CheckAll(CLBoxSubestacao, true);
    // reinicia CLBoxPrimario
    // LBoxPrimarioInicia();
    CLBoxPrimarioInicia();
    // aciona todos os primários de todas as SEs
    ActionPrimarioSelAllExecute(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::ActionSubestacaoSelNoneExecute(TObject *Sender)
{
	// cancela seleção de todas as Subestacoes
	CheckAll(CLBoxSubestacao, false);
	// reinicia CLBoxPrimario
	// LBoxPrimarioInicia();
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::CBoxCondutorInicia(TComboBox *CBox, std::map<int,TArranjoBDGD*>mapArranjo,
	int arranjo_id)
{
	// variáveis locais
	TArranjoBDGD *arranjo;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	std::map<int,TArranjoBDGD*>::iterator it;
	AnsiString nome_arq;
	int index, valor;

	// reinicia CBox
	CBox->Clear();
	// insere opção de não assumir um Condutor default
	CBox->Items->AddObject("Não utilizar", NULL);
	for (it = mapArranjo.begin(); it != mapArranjo.end(); it++)
	{
		arranjo = (TArranjoBDGD*)it->second;
		index = CBox->Items->AddObject(arranjo->Nome(), arranjo);
		// seleciona Condutor indicado
		if (arranjo->Id == arranjo_id)
			CBox->ItemIndex = index;
		continue;
	}
	// verifica se foi selecionado um Condutor
	if (CBox->ItemIndex < 0)
	{ // verifica se condutor_id é nulo, o que indica opção "Não utilizar"
		if (arranjo_id == 0)
			CBox->ItemIndex = 0;
	}
	try
	{ // define nome do arquivo
		nome_arq = "OpcoesImportadorEgrid";
		if (geral)
		{ // lê parâmetros do Form
			if (!geral->Infoset->LoadFromFile(nome_arq))
				return;
			geral->Infoset->GetInfo(CBox->Name, valor);
			// verifica se o index existe
			if ((valor < CBox->Items->Count) && (valor >= 0))
				CBox->ItemIndex = valor;
		}
	}
	catch (Exception &e)
	{
		Aviso("Erro ao carregar opções de condutores");
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::CheckAll(TCheckListBox *CLBox, bool checked)
{
	for (int n = 0; n < CLBox->Items->Count; n++)
	{
		CLBox->Checked[n] = checked;
	}

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::CLBoxPrimarioClickCheck(TObject *Sender)
{
	// variáveis locais
	TRedeMT *primario;
	TSubestacao *subestacao;
	TCheckListBox *CLBox = CLBoxPrimario;
	bool checked = false;
	int item_check;

	//verifica selecao
	item_check = CLBoxPrimario->ItemIndex;
	if (item_check < 0) {
		return;
	}
	// marca/desmarca seleção
	if (CLBox->Checked[item_check] == false) 
	{
		checked = true;
	}
	else
	{
		checked = false;
	}
	CLBox->Checked[item_check] = checked;

	// determina primario
	primario = (TRedeMT*)CLBoxPrimario->Items->Objects[item_check];
	// marca/desmarca seleção
	primario->Selecionado = CLBoxPrimario->Checked[item_check];
}

// -------------------------------------------------------------------------------------------------

void __fastcall TFormImportadorUgrid::CLBoxPrimarioInicia(void)
{
	// variáveis locais
	TRedeMT *primario;
	std::map<int,TRedeMT*>mapPrimario = commons->GetmapRedeMT();
	std::map<int,TRedeMT*>::iterator it;
	int index;

	// reinicia CLBoxPrimario
	CLBoxPrimario->Clear();
	// monta lista com as Subestações selecionadas
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;
	// insere em CLBoxPrimario os Primarios associados a Subestaçoes selecionadas
	for (it = mapPrimario.begin(); it != mapPrimario.end(); it++)
	{
		primario = it->second;
		// verifica se a Subestacao do Primario existe no mapSub
		if((primario->Id_Sub !=NULL) && (lisTMP->IndexOf(commons->GetSubestacao(primario->Id_Sub)) >= 0))
		{
			// insere Primario em CLBoxPrimario
			index = CLBoxPrimario->Items->AddObject(primario->Nome, primario);
			// Verifica se estava com check
			CLBoxPrimario->Checked[index] = primario->Selecionado;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::CLBoxSubestacaoClickCheck(TObject *Sender)
{
	TCheckListBox *CLBox = CLBoxSubestacao;
	bool checked = false;
	int item_check;

	//verifica selecao
	item_check = CLBoxSubestacao->ItemIndex;
	if (item_check < 0) {
		return;
	}
	// marca/desmarca seleção
	if (CLBox->Checked[item_check] == false) {
		checked = true;
	}
	else
	{
		checked = false;
	}
	CLBox->Checked[item_check] = checked;
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::CLBoxSubestacaoInicia(void)
{
	// variáveis locais
	TSubestacao *subestacao;
	std::map<int,TSubestacao*>mapSub = commons->GetmapSubestacao();
	std::map<int,TSubestacao*>::iterator it;

	// reinicia CLBoxSubestacao
	CLBoxSubestacao->Clear();
	// insere em CLBoxSubestacao as Subestaçoes do Ugrid
	for (it = mapSub.begin(); it != mapSub.end(); it++)
	{
		subestacao = it->second;
		CLBoxSubestacao->Items->AddObject(subestacao->Nome, subestacao);
	}
	CLBoxSubestacao->Sorted = true;
	CLBoxSubestacao->Visible = true;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::FormLeOpcoes(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString nome_arq;
	int valor;
	AnsiString texto;

	try
	{ // define nome do arquivo
		nome_arq = "OpcoesImportadorEgrid";
		if (geral)
		{ // lê parâmetros do Form
			if (!geral->Infoset->LoadFromFile(nome_arq))
				return;
			geral->Infoset->GetInfo("RadioGroupDestino", valor);
			RadioGroupDestino->ItemIndex = valor;

			geral->Infoset->GetInfo("EditPastaPublica", texto);
			EditPastaPublica->Text = texto;

			geral->Infoset->GetInfo("RadioGroupConsPrimario", valor);
			RadioGroupConsPrimario->ItemIndex = valor;

			geral->Infoset->GetInfo("CheckBoxLog", valor);
			CheckBoxLog->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxCronometro", valor);
			CheckBoxCronometro->Checked = (valor == 1);

			geral->Infoset->GetInfo("RadioGroupRedeSec", valor);
			RadioGroupRedeSec->ItemIndex = valor;

			geral->Infoset->GetInfo("CheckBoxValidaFase", valor);
			CheckBoxValidaFase->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxCorrigeFase", valor);
			CheckBoxCorrigeFase->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxCorrigePerdaFerro", valor);
			CheckBoxCorrigePerdaFerro->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxAbrirChaveExt", valor);
			CheckBoxAbrirChaveExt->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxAbrirChaveInt", valor);
			CheckBoxAbrirChaveInt->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxDeslocarTrechoExt", valor);
			CheckBoxDeslocarTrechoExt->Checked = (valor == 1);

			geral->Infoset->GetInfo("CheckBoxDeslocarTrechoInt", valor);
			CheckBoxDeslocarTrechoInt->Checked = (valor == 1);
		}
	}
	catch (Exception &e)
	{
		Aviso("Erro ao ler opções de importação");
	}

}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormImportadorUgrid::FormSalvaOpcoes(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString nome_arq;

	try
	{ // define nome do arquivo
		nome_arq = "OpcoesImportadorEgrid";
		if (geral)
		{ // salva parâmetros do Form
			geral->Infoset->AddInfo("RadioGroupDestino", RadioGroupDestino->ItemIndex);
			geral->Infoset->AddInfo("EditPastaPublica", EditPastaPublica->Text);
			geral->Infoset->AddInfo("RadioGroupConsPrimario", RadioGroupConsPrimario->ItemIndex);
			geral->Infoset->AddInfo("CheckBoxLog", (CheckBoxLog->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxCronometro", (CheckBoxCronometro->Checked ? 1 : 0));
			geral->Infoset->AddInfo("RadioGroupRedeSec", RadioGroupRedeSec->ItemIndex);
			geral->Infoset->AddInfo("CheckBoxValidaFase", (CheckBoxValidaFase->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxCorrigeFase", (CheckBoxCorrigeFase->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxCorrigePerdaFerro",
				(CheckBoxCorrigePerdaFerro->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxAbrirChaveExt", (CheckBoxAbrirChaveExt->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxAbrirChaveInt", (CheckBoxAbrirChaveInt->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxDeslocarTrechoExt", (CheckBoxDeslocarTrechoExt->Checked ? 1 : 0));
			geral->Infoset->AddInfo("CheckBoxDeslocarTrechoInt", (CheckBoxDeslocarTrechoInt->Checked ? 1 : 0));
			// condutores
			geral->Infoset->AddInfo("CBoxCondutorMT", CBoxCondutorMT->ItemIndex);
			geral->Infoset->AddInfo("CBoxCondutorBT", CBoxCondutorBT->ItemIndex);
            // Salva opções
			geral->Infoset->SaveToFile(nome_arq);
		}
	}
	catch (Exception &e)
	{
		Aviso("Erro ao salvar opções de importação");
	}
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TFormImportadorUgrid::ImportaSubestacao(void)
{
	// variáveis locais
	bool sucesso = false;

	// define pasta para o caso de salvar as redes
	conversor->Pasta = EditPastaPublica->Text;
	// esconde este Form
	Visible = false;
	// faz a leitura da base Oracle e montagem das redes
	sucesso = conversor->ImportaRedes(lisTMP, &opcao, RadioGroupDestino->ItemIndex);
	// exibe este Form
	Visible = true;

	return (sucesso);
}

// ---------------------------------------------------------------------------
// void __fastcall TFormImportadorUgrid::LBoxPrimarioInicia(void)
// {
// // variáveis locais
// VTPrimario *primario;
// TList *lisEXT = empresa->LisPrimario();
//
// // reinicia LBoxPrimario
// LBoxPrimario->Clear();
// LBoxPrimario->Sorted = false;
// // lisTMP->Clear();
// // monta lista com as Subestações selecionadas
// if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
// return;
// // insere em LBoxPrimario os Primarios associados a Subestaçoes selecionadas
// for (int n = 0; n < lisEXT->Count; n++)
// {
// primario = (VTPrimario*)lisEXT->Items[n];
// // verifica se a Subestacao do Primario está na lista lisTMP
// if ((primario->Subestacao != NULL) && (lisTMP->IndexOf(primario->Subestacao) >= 0))
// { // insere Primario em LBoxPrimario
// LBoxPrimario->Items->AddObject(primario->Nome, primario);
// }
// }
// LBoxPrimario->Sorted = true;
// }

// -------------------------------------------------------------------------------------------------
int __fastcall TFormImportadorUgrid::LisObjectChecked(TCheckListBox *CLBox, TList *lisOBJ)
{
	// variáveis locais
	TObject *object;

	lisOBJ->Clear();
	// insere em lisOBJ os objetos associados aos Items checked
	for (int n = 0; n < CLBox->Items->Count; n++)
	{
		if (CLBox->Checked[n])
		{
			if ((object = CLBox->Items->Objects[n]) == NULL)
				continue;
			if (lisOBJ->IndexOf(object) < 0)
				lisOBJ->Add(object);
		}
	}

	return (lisOBJ->Count);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TFormImportadorUgrid::ReiniciaRede(bool aviso)
{
	// var local
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisRedes;

	// proteção
	if (!redes)
		return false;
	lisRedes = redes->LisRede();
	if (lisRedes->Count == 0)
		return true;

	// aviso
	if (aviso)
		if (Confirma("As redes serão apagadas da memória.\n" "Deseja realmente continuar ?",
			"") != ID_YES)
		{
			return false;
		}

	// remove redes da memoria
	if (redes)
		redes->Clear();

	return true;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TFormImportadorUgrid::ValidaOpcoesDeImportacao(void)
{
	// variáveis locais
	AnsiString dirname;

	// verifica destino das redes importadas
	if (RadioGroupDestino->ItemIndex < 0)
	{
		Aviso("Selecione opção de como serão tratadas as redes importadas");
		return (false);
	}
	lisTMP->Clear();
	// inicia lista com Subestações selecionadas
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
	{
		Aviso("Selecione as subestações a serem importadas");
		return (false);
	}

	// verifica seleção de Condutores default
	if ((CBoxCondutorMT->ItemIndex < 0) || (CBoxCondutorBT->ItemIndex < 0) ||
		(CBoxCondutorRL->ItemIndex < 0) || (CBoxCondutorIP->ItemIndex < 0))
	{
		Aviso("Selecione opções de condutor default");
		return (false);
	}
	conversor->CondutorDefaultMT =
		(TArranjoBDGD*)CBoxCondutorMT->Items->Objects[CBoxCondutorMT->ItemIndex];
	conversor->CondutorDefaultBT =
		(TArranjoBDGD*)CBoxCondutorBT->Items->Objects[CBoxCondutorBT->ItemIndex];
	conversor->CondutorDefaultRL =
		(TArranjoBDGD*)CBoxCondutorRL->Items->Objects[CBoxCondutorRL->ItemIndex];
	conversor->CondutorDefaultIP =
		(TArranjoBDGD*)CBoxCondutorIP->Items->Objects[CBoxCondutorIP->ItemIndex];

	// verifica se foi definida uma pasta publica válida
	dirname = EditPastaPublica->Text.Trim();
	if ((RadioGroupDestino->ItemIndex > 0) && (!dirname.IsEmpty()))
	{ // verifica se é uma pasta válida
		if (!DirectoryExists(dirname))
		{
			Aviso("Não existe a pasta pública para cópia das redes");
			return (false);
		}
	}
	// define as demais opções selecionadas
	opcao.date = DateTimePicker->Date;
	opcao.sub.topologia = opSUB_GIS;
	opcao.pri.consA4 = RadioGroupConsPrimario->ItemIndex; // == 0;
	opcao.sec.topologia = RadioGroupRedeSec->ItemIndex;
	opcao.valida_fase = CheckBoxValidaFase->Checked; // true;
	opcao.corrige_fase = CheckBoxCorrigeFase->Checked; // true;
	opcao.corrige_perda_fe = CheckBoxCorrigePerdaFerro->Checked; // true;
	opcao.gerarTXT = false;
	// malhas
	opcao.malha.abre_chave_ext = CheckBoxAbrirChaveExt->Checked;
	opcao.malha.abre_chave_int = CheckBoxAbrirChaveInt->Checked;
	opcao.malha.desloca_trecho_ext = CheckBoxDeslocarTrechoExt->Checked;
	opcao.malha.desloca_trecho_int = CheckBoxDeslocarTrechoInt->Checked;
	// opções de ihm
	opcao.ihm.log = CheckBoxLog->Checked;
	opcao.ihm.progresso = true;

	// verifica se deve cronometrar tempos
	apl->Remove(cronometro);
	if (CheckBoxCronometro->Checked)
	{
		apl->Add(cronometro);
	}
	return (true);
}

// --------------------eof-----------------------------------------------------
