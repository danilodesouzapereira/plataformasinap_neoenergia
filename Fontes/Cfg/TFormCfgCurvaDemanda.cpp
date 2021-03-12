// ---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
#include <Math.hpp>
#include "TFormCfgCurvaDemanda.h"
#include "TFormCurva.h"
#include "TFormImportaCurvaLote.h"
#include "TAplCfg.h"
// #include "..\BD\VTBD.h"
// #include "..\BD\Rede\VTBDGeral.h"
// #include "..\BD\VTSNPDBConn.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\InterfaceTXT\Exporta\VTExportaTXT.h"
#include "..\InterfaceTXT\Importa\VTImportaTXT.h"
#include "..\InterfaceXML\VTExportaXML.h"
#include "..\InterfaceXML\VTImportaXML.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTEqpto.h"
// #include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipos.h"
// #include "..\..\DLL_Inc\BD.h"
// #include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\InterfaceTXT.h"
#include "..\..\DLL_Inc\InterfaceXML.h"
#include "..\..\DLL_Inc\Progresso.h"
// #include "..\..\DLL_Inc\Rede.h"
#include<stdexcept>
// ---------------------------------------------------------------------------

#include <ImportacaoCurvaTipica\TExportaArquivoCurvaTipicaModelo.h>
#include <ImportacaoCurvaTipica\TImportaArquivoCurvaTipica.h>

#pragma package(smart_init)
#pragma resource "*.dfm"

// ------------------------------------------------------------------- namespace

using namespace std;
//using namespace sinapsis::sinap::configuracao;

// ----------------------------------------------------------------------- class

// ---------------------------------------------------------------------------
__fastcall TFormCfgCurvaDemanda::TFormCfgCurvaDemanda(TComponent* Owner, VTApl *apl_owner,
	VTApl *apl_child) : TForm(Owner)
{
	// cria objeto AplCfg
	apl = new TAplCfg(this, apl_owner);
	// salva ponteiro
	this->apl_child = apl_child;
	// salva ponteiro p/ objeto VTCurvas
	curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	// cria lisCURVA
	lisCURVA = new TList();
	// seleciona TabSheet
	PageControl->ActivePage = TabSheetGrafico;
	RGroupSerie->ItemIndex = 0;
	// habilita/desabilita ActionSalvaOraclecomandos de usu�rio privilegiado
	// HabilitaComandosUsuarioPrivilegiado();
	// l� base de dados
	curvas->Clear();
	apl->LeBaseCurvaTipica();
	// inicia TreeView
	TreeViewInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgCurvaDemanda::~TFormCfgCurvaDemanda(void)
{
	// destr�i objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// destr�i lista sem destruir objetos
	if (lisCURVA)
	{
		delete lisCURVA;
		lisCURVA = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionEditaExecute(TObject *Sender)
{
	// vari�veis locais
	VTCurva *curva;

	// verifica se n�o tem estudo do planejamento aberto
	if (ExistePlanejamento())
		return;
	// determina Curva selecionada
	if ((curva = TreeViewCurvaSelecionada()) == NULL)
	{
		Aviso("Seleciona a curva t�pica para edi��o");
		return;
	}
	// edita a Curva
	EditaCurva(curva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionFechaExecute(TObject *Sender)
{
	// fecha este Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionExportarArquivoCurvasExecute(TObject *Sender)
{
	// variaveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	TExportaArquivoCurvaTipicaModelo *exportaArquivoModelo;
	AnsiString fileName;

	// oculta o form para o TSaveDialog ser exibido(evita o conflito entre 2 modal)
	Hide();

	// solicita o local para salvar o arquivo
	if (SaveDialog->InitialDir.IsEmpty())
	{
		SaveDialog->InitialDir = path->DirExporta();
	}
	SaveDialog->FileName = "CurvasTipicasDemanda.csv";
	SaveDialog->Filter = "Comma-separated values (*.csv)|*.csv|";
	if (SaveDialog->Execute())
	{
		fileName = SaveDialog->FileName;
		// cria o objeto responsavel por expotar as curvas para um arquivo
		exportaArquivoModelo = new TExportaArquivoCurvaTipicaModelo(this->apl);

		try
		{
			if (exportaArquivoModelo->Exportar(fileName))
				Aviso("O arquivo foi gerado com sucesso!");
		}
		catch (const runtime_error& e)
		{
			Erro("Erro ao gerar o arquivo: " + (AnsiString)e.what());
		}
		// remove objetos
		delete exportaArquivoModelo;
	}
	// exibe o form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionExportaTXTExecute(TObject *Sender)
{
	// vari�veis locais
	VTExportaTXT *exportaTXT;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Hide();
	// inicia diret�rio de OpenDialog
	SaveDialog->Filter = "Arquivos Texto (*.txt)|*.txt|Todos Arquivos (*.*)|*.*";
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	// exibe OpenDialog
	if (SaveDialog->Execute())
	{ // executa importa��o dos arranjos a partir do arquivo
		exportaTXT = DLL_NewObjExportaTXT(apl);
		exportaTXT->ExportaCurvasTipicas(SaveDialog->FileName);
		delete exportaTXT;
	}
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionExportaXMLExecute(TObject *Sender)
{
	// vari�veis locais
	VTExportaXML *exportaXML;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Hide();
	// inicia diret�rio de OpenDialog
	SaveDialog->Filter = "Arquivos XML (*.xml)|*.xml|Todos Arquivos (*.*)|*.*";
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	// exibe OpenDialog
	if (SaveDialog->Execute())
	{ // executa importa��o dos arranjos a partir do arquivo
		exportaXML = DLL_NewObjExportaXML(apl);
		exportaXML->ExportaArranjo = false;
		exportaXML->ExportaTipoChave = false;
		exportaXML->ExportaTipoRede = false;
		exportaXML->ExportaRede = false;
		exportaXML->ExportaCurvaTipica = true;
		exportaXML->ExportaReducao = false;
		exportaXML->ExportaCurvasTipicas(SaveDialog->FileName);
		delete exportaXML;
	}
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionImportarArquivoCurvasExecute(TObject *Sender)
{
	// se existe uma janela com rede aberta, nao deixa carregar as curvas(pois isto exigiria
	// atualiza��o das curvas nas janelas abertas...)
	if (ExistePlanejamento())
		return;

	// oculta o form para o Confirma ser exibido(evita o conflito entre 2 modal)
	Hide();

	// pede confirma��o do usu�rio
	int resp =
		Confirma("Todas curvas t�picas existentes ser�o exclu�das e substitu�das pelas novas.",
		"Confirma a importa��o ?", MB_YESNO);

	if ((resp == IDNO) || (resp == IDCANCEL))
	{
		Show();
		return;
	}

	// solicita o arquivo a ser importado
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	// inicia diret�rio de OpenDialog
	if (OpenDialog->InitialDir.IsEmpty())
		OpenDialog->InitialDir = path->DirImporta();
	OpenDialog->Filter = "Comma-separated values (*.csv)|*.csv|";
	if (OpenDialog->Execute())
	{
		AnsiString fileName = OpenDialog->FileName;
		// verifica se o usuario selecionou o arquivo
		if (fileName.IsEmpty())
		{
			Aviso("Selecione o arquivo a ser importado!");
			Show();
			return;
		}
		VTProgresso* progresso = DLL_NewObjProgresso(this, NULL);
		// inicia progresso
		if (progresso)
			progresso->Start(progDEFAULT);
		//
		// cria o objeto responsavel por importar as curvas para um arquivo
		//
		TImportaArquivoCurvaTipica *importaArquivo = new TImportaArquivoCurvaTipica(this->apl);
		int countCurvasSalvas = 0;

		try
		{
			countCurvasSalvas = importaArquivo->Importar(fileName);
		}
		catch (const runtime_error& e)
		{
			Erro((AnsiString)e.what());
			if (progresso)
				progresso->Stop();
			Show();
			return;
		}

		//
		// remove objetos
		delete importaArquivo;
		importaArquivo = NULL;

		//
		// Atualiza a interface gr�fica
		//
		curvas->Clear();
		apl->LeBaseCurvaTipica();

		// inicia TreeView
		TreeViewInicia();

		if (progresso)
			progresso->Stop();

		if (countCurvasSalvas > 0)
			Aviso(IntToStr(countCurvasSalvas) + " curvas foram importadas.");
		else
			Aviso("Nenhuma curva foi adicionada!");

	}
	// exibe o form
	Show();

}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionImportarArquivoCurvasLoteExecute(TObject *Sender)
{
	// D.V.C. 2018
	// Vari�veis locais
	bool confirma;
	TFormImportaCurvaLote *FormCurva;

	// esconde este Form
	Hide();
	try
	{ // cria um Clone da Curva atual;
		curvas->Clear();
		apl->LeBaseCurvaTipica();
		TreeViewInicia();
		// cria TFormCurva e exibe como janela modal
		FormCurva = new TFormImportaCurvaLote(NULL, apl);
		if (FormCurva->ShowModal() == mrOk)
		{ //atualiza
			curvas->Clear();
			apl->LeBaseCurvaTipica();
			// inicia TreeView
			TreeViewInicia();
		}
		delete FormCurva;
		// reapresenta este Form
		Show();
		// ModalResult = mrOk;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionImportaTXTExecute(TObject *Sender)
{
	// vari�veis locais
	VTImportaTXT *importaTXT;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Hide();

	// verifica se o usu�rio quer reiniciar base de dados
	// ReiniciabaseDados();
	// inicia diret�rio de OpenDialog
	if (OpenDialog->InitialDir.IsEmpty())
		OpenDialog->InitialDir = path->DirImporta();
	// configura filtro
	OpenDialog->Filter = "Arquivos Texto (*.txt)|*.txt|Todos Arquivos (*.*)|*.*";
	// exibe OpenDialog
	if (OpenDialog->Execute())
	{ // executa importa��o dos arranjos a partir do arquivo
		importaTXT = DLL_NewObjImportaTXT(apl);
		importaTXT->ImportaCurvasTipicas(OpenDialog->FileName);
		delete importaTXT;
		// atualiza base de dados
		AtualizaCurvas();
		// atualiza TreeView
		TreeViewInicia();
	}
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionImportaXMLExecute(TObject *Sender)
{
	// vari�veis locais
	VTImportaXML *importaXML;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// esconde este Form
	Hide();
	// verifica se o usu�rio quer reiniciar base de dados
	// ReiniciabaseDados();
	// inicia diret�rio de OpenDialog
	if (OpenDialog->InitialDir.IsEmpty())
		OpenDialog->InitialDir = path->DirImporta();
	// configura filtro
	OpenDialog->Filter = "Arquivos XML (*.xml)|*.xml|Todos Arquivos (*.*)|*.*";
	// exibe OpenDialog
	if (SaveDialog->Execute())
	{ // executa importa��o dos arranjos a partir do arquivo
		importaXML = DLL_NewObjImportaXML(apl);
		importaXML->ImportaCurvasTipicas(SaveDialog->FileName);
		delete importaXML;
		// atualiza base de dados
		AtualizaCurvas();
		// atualiza TreeView
		TreeViewInicia();
	}
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionInsereExecute(TObject *Sender)
{
	// vari�veis locais
	VTClasse *classe;
	TTreeNode *node = TreeView->Selected;

	// verifica se n�o tem estudo do planejamento aberto
	if (ExistePlanejamento())
		return;
	// prote��o: verifica classse de curva selecionada no TreeView
	if (node == NULL)
	{
		Aviso("Selecione o tipo de curva a ser criada");
		return;
	}
	// detemina Classe selecionada
	if ((classe = TreeViewClasseSelecionada()) != NULL)
	{
		switch (classe->Tag)
		{
		case consRESIDENCIAL:
			InsereCurvaConsumidor(classe);
			break;
		case consCOMERCIAL:
			InsereCurvaConsumidor(classe);
			break;
		case consINDUSTRIAL:
			InsereCurvaConsumidor(classe);
			break;
		case consRURAL:
			InsereCurvaConsumidor(classe);
			break;
		case consOUTROS:
			InsereCurvaConsumidor(classe);
			break;
		case consIP:
			InsereCurvaIP();
			break;
			// case consIP:           InsereCurvaConsumidor(classe); break;
		case consA4:
			InsereCurvaConsumidor(classe);
			break;
		case consA2:
			InsereCurvaConsumidor(classe);
			break;
		}
	}
	// verifica se foi slecionada curva de capacitor, reator ou fotovoltaico
	// else if (node->Text.AnsiCompareIC("Capacitores") == 0)
	else if (node->Text.Pos("Capacitor") != 0) /* DVK2012.12.18 */
	{
		InsereCurvaCapacitor();
	}
	// verifica se foi slecionada curva de capacitor ou reator
	// else if (node->Text.AnsiCompareIC("Reatores") == 0)
	else if (node->Text.Pos("Reator") != 0) /* DVK2012.12.18 */
	{
		InsereCurvaReator();
	}
	// else if (node->Text.CompareIC("Gerador Fotovoltaico") == 0)
	else if (node->Text.CompareIC("Gerador") == 0)
	{
		InsereCurvaGeradorFotovoltaico();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionRetiraExecute(TObject *Sender)
{
	// vari�veis locais
	VTCurva *curva;

	// verifica se n�o tem estudo do planejamento aberto
	if (ExistePlanejamento())
		return;
	// determina Curva selecionada
	if ((curva = TreeViewCurvaSelecionada()) == NULL)
	{
		Aviso("Seleciona a curva t�pica a ser exclu�da");
		return;
	}
	// pede confirma��o do usu�rio
	if (Confirma("A curva " + AnsiQuotedStr(curva->Codigo, '\'') + " ser� exclu�da.",
		"Confirma a opera��o ?") == IDYES)
	{ // marca Curva como exclu�da e atualiza base de dados
		curva->Status[sttEXCLUIDO] = true;
		apl->SalvaBaseCurvaTipica(curva);
		// atualiza TreeView: elimina TreeNode selecionado
		TreeViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ActionSalvaOracleExecute(TObject *Sender)
{
	/*
	 //vari�veis locais
	 VTBD *bd = (VTBD*)apl->GetObject(__classid(VTBD));

	 //define Status novo p/ todas as Curvas t�picas
	 DefineIdInvalido(curvas->ListaCurvaTipicaCapacitor());
	 DefineIdInvalido(curvas->ListaCurvaTipicaCarga());
	 DefineIdInvalido(curvas->ListaCurvaTipicaIP());
	 DefineIdInvalido(curvas->ListaCurvaTipicaReator());
	 DefineIdInvalido(curvas->ListaCurvaTipicaGeradorFotovoltaico());
	 //salva todas as Curvas na base oracle
	 if (bd->ConectaServidorRedeRemoto())
	 {
	 if (bd->Geral->SalvaCurvaTipica())
	 {
	 Aviso("Curvas salvas com sucesso.");
	 }
	 else
	 {
	 Aviso("Erro ao salvar as curvas.");
	 }
	 }
	 else
	 {
	 Aviso("N�o foi poss�vel conectar com a base Oracle");
	 }
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::AtualizaCurvas(void)
{
	// vari�veis locais
	VTCurva *curva;

	for (int n = 0; n < curvas->ListaCurvaTipicaCapacitor()->Count; n++)
	{
		curva = (VTCurva*)curvas->ListaCurvaTipicaCapacitor()->Items[n];
		if (curva->Status[sttNOVO] != true)
			continue;
		apl->SalvaBaseCurvaTipica(curva);
	}

	for (int n = 0; n < curvas->ListaCurvaTipicaCarga()->Count; n++)
	{
		curva = (VTCurva*)curvas->ListaCurvaTipicaCarga()->Items[n];
		if (curva->Status[sttNOVO] != true)
			continue;
		apl->SalvaBaseCurvaTipica(curva);
	}

	for (int n = 0; n < curvas->ListaCurvaTipicaGeradorFotovoltaico()->Count; n++)
	{
		curva = (VTCurva*)curvas->ListaCurvaTipicaGeradorFotovoltaico()->Items[n];
		if (curva->Status[sttNOVO] != true)
			continue;
		apl->SalvaBaseCurvaTipica(curva);
	}

	for (int n = 0; n < curvas->ListaCurvaTipicaIP()->Count; n++)
	{
		curva = (VTCurva*)curvas->ListaCurvaTipicaIP()->Items[n];
		if (curva->Status[sttNOVO] != true)
			continue;
		apl->SalvaBaseCurvaTipica(curva);
	}

	for (int n = 0; n < curvas->ListaCurvaTipicaReator()->Count; n++)
	{
		curva = (VTCurva*)curvas->ListaCurvaTipicaReator()->Items[n];
		if (curva->Status[sttNOVO] != true)
			continue;
		apl->SalvaBaseCurvaTipica(curva);
	}
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartClick(TObject *Sender)
{
	// vari�veis locais
	double val_min, val_max, delta;
	TLineSeries *Serie;

	// prote��o
	if (RGroupSerie->ItemIndex < 0)
		return;
	chart->LeftAxis->Minimum = 0.;
	chart->LeftAxis->Maximum = 0.;

	if(RGroupSerie->ItemIndex == 0)
	{
		Serie = Series1;
	}
	else
	{
		Serie = Series2;
	}
	// redimensiona o eixo Y
	// determina valores m�ximo e m�nimo do eixo y
	val_max = Serie->MaxYValue();
	val_min = Serie->MinYValue();
	// inclui uma margem adicional nos valores m�ximo e m�nimo
	delta = fabs((fabs(val_max) - fabs(val_min))) * 0.1;
	val_max += delta;
	val_min -= delta;
	// define incremento
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	if (val_max > 0)
	{
		chart->LeftAxis->Maximum = val_max;
		chart->LeftAxis->Minimum = val_min;
	}
	else
	{
		chart->LeftAxis->Minimum = val_min;
		chart->LeftAxis->Maximum = val_max;
	}
	chart->LeftAxis->Increment = delta;
	chart->Refresh();
	chart->Visible = true;
	// exibe a Serie
	Serie->Visible = true;
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartIniciaCurvaPQ(VTCurva* curva)
{
	// vari�veis locais
	AnsiString txt;
	int np;
	strHM hm;
	double valor_max, valor_min, increment;
	double valor[nvCURVA_PQ];

	// mostra legenda //DVK 2015.09.08
	chart->Legend->Visible = false; //true;
	// esconde o t�tulo
	chart->Title->Visible = true;
	chart->Title->Text->Clear();
	chart->Title->Text->Add(curva->Codigo);
	// reinicia eixos do Chart
	chart->BottomAxis->LabelStyle = talText;
	// reinicia Serie1
	Series1->Clear();
	Series1->Visible = false; //true;
	Series1->Color = clBlue;
	Series1->Title = "Pot�ncia Ativa";
	// reinicia Serie2
	Series2->Clear();
	Series2->Color = clRed;
	Series2->Visible = false; //true;
	Series2->Title = "Pot�ncia Reativa";
	// exibe valores da Curva
	np = 0;
	for (hm.hora = 0; hm.hora <= 23; hm.hora++)
	{
		txt = IntToStr(hm.hora);
		for (hm.minuto = 0; hm.minuto < 60; hm.minuto += 15)
		{ // obt�m valor da curva
			curva->GetValor(hm, valor, sizeof(valor) / sizeof(double));
			if (hm.minuto != 0)
				txt = "";
			// insere P na Serie2
			Series1->AddXY(np, valor[0], txt, clBlue);
			Series1->AddXY(np + 1, valor[0], "", clBlue);
			// insere Q na Serie2
			Series2->AddXY(np, valor[1], txt, clRed);
			Series2->AddXY(np + 1, valor[1], "", clRed);
			// incrementa pr�ximo valor do eixo x
			np++;
		}
	}
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	RGroupSerie->Items->Add(Series1->Title);
	RGroupSerie->Items->Add(Series2->Title);
	RGroupSerie->ItemIndex = 0;
	ChartClick(NULL);
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartIniciaCurvaPQVT(VTCurva* curva)
{
	// vari�veis locais
	AnsiString txt;
	int np;
	strHM hm;
	double valor_max, valor_min, increment;
	double valor[nvCURVA_PQVT];

	// mostra legenda //DVK 2015.09.08
	chart->Legend->Visible = false; //true;
	// esconde o t�tulo
	chart->Title->Visible = true;
	chart->Title->Text->Clear();
	chart->Title->Text->Add(curva->Codigo);
	// reinicia eixos do Chart
	chart->BottomAxis->LabelStyle = talText;
	// reinicia Serie1
	Series1->Clear();
	Series1->Visible = false; //true;
	Series1->Color = clBlue;
	Series1->Title = "Pot�ncia Ativa";
	// reinicia Serie2
	Series2->Clear();
	Series2->Color = clRed;
	Series2->Visible = false; //true;
	Series2->Title = "Pot�ncia Reativa";
	// exibe valores da Curva
	np = 0;
	for (hm.hora = 0; hm.hora <= 23; hm.hora++)
	{
		txt = IntToStr(hm.hora);
		for (hm.minuto = 0; hm.minuto < 60; hm.minuto += 15)
		{ // obt�m valor da curva
			curva->GetValor(hm, valor, sizeof(valor) / sizeof(double));
			if (hm.minuto != 0)
				txt = "";
			// insere P na Serie1
			Series1->AddXY(np, valor[0], txt, clBlue);
			Series1->AddXY(np + 1, valor[0], "", clBlue);
			// insere FP na Serie2
			Series2->AddXY(np, valor[1], txt, clRed);
			Series2->AddXY(np + 1, valor[1], "", clRed);
			// incrementa pr�ximo valor do eixo x
			np++;
		}
	}
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	RGroupSerie->Items->Add(Series1->Title);
	RGroupSerie->Items->Add(Series2->Title);
	RGroupSerie->ItemIndex = 0;
	ChartClick(NULL);
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartIniciaCurvaPQ_MDP(VTCurva* curva)
{
	// vari�veis locais
	AnsiString txt;
	int np;
	strHM hm;
	double valor_max, valor_min, increment;
	double valor[nvCURVA_PQ_MDP];

	// mostra legenda //DVK 2015.09.08
	chart->Legend->Visible = false; //true;
	// esconde o t�tulo
	chart->Title->Visible = true;
	chart->Title->Text->Clear();
	chart->Title->Text->Add(curva->Codigo);
	// reinicia eixos do Chart
	chart->BottomAxis->LabelStyle = talText;
	// reinicia Serie1
	Series1->Clear();
	Series1->Visible = false; //true;
	Series1->Color = clBlue;
	Series1->Title = "Pot�ncia Ativa";
	// reinicia Serie2
	Series2->Clear();
	Series2->Color = clRed;
	Series2->Visible = false; //true;
	Series2->Title = "Pot�ncia Reativa";
	// exibe valores da Curva
	np = 0;
	for (hm.hora = 0; hm.hora <= 23; hm.hora++)
	{
		txt = IntToStr(hm.hora);
		for (hm.minuto = 0; hm.minuto < 60; hm.minuto += 15)
		{ // obt�m valor da curva
			curva->GetValor(hm, valor, sizeof(valor) / sizeof(double));
			if (hm.minuto != 0)
				txt = "";
			// insere P na Serie1
			Series1->AddXY(np, valor[0], txt, clBlue);
			Series1->AddXY(np + 1, valor[0], "", clBlue);
			// insere Q na Serie2
			Series2->AddXY(np, valor[1], txt, clRed);
			Series2->AddXY(np + 1, valor[1], "", clRed);
			// incrementa pr�ximo valor do eixo x
			np++;
		}
	}
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	RGroupSerie->Items->Add(Series1->Title);
	RGroupSerie->Items->Add(Series2->Title);
	RGroupSerie->ItemIndex = 0;
	ChartClick(NULL);
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartIniciaCurvaPFP(VTCurva* curva)
{
	// vari�veis locais
	AnsiString txt;
	int np;
	strHM hm;
	double valor_max, valor_min, increment;
	double valor[nvCURVA_PFP];

	// mostra legenda //DVK 2015.09.08
	chart->Legend->Visible = false; //true;
	// esconde o t�tulo
	chart->Title->Visible = true;
	chart->Title->Text->Clear();
	chart->Title->Text->Add(curva->Codigo);
	// reinicia eixos do Chart
	chart->BottomAxis->LabelStyle = talText;
	// reinicia Serie1
	Series1->Clear();
	Series1->Visible = false; //true;
	Series1->Color = clBlue;
	Series1->Title = "Pot�ncia Ativa";
	// reinicia Serie1
	Series2->Clear();
	Series2->Color = clRed;
	Series2->Visible = false; //true;
	Series2->Title = "Fat.Pot�ncia";
	// exibe valores da Curva
	np = 0;
	for (hm.hora = 0; hm.hora <= 23; hm.hora++)
	{
		txt = IntToStr(hm.hora);
		for (hm.minuto = 0; hm.minuto < 60; hm.minuto += 15)
		{ // obt�m valor da curva
			curva->GetValor(hm, valor, sizeof(valor) / sizeof(double));
			if (hm.minuto != 0)
				txt = "";
			// insere P na Serie2
			Series1->AddXY(np, valor[0], txt, clBlue);
			Series1->AddXY(np + 1, valor[0], "", clBlue);
			// insere FP na Serie2
			Series2->AddXY(np, valor[1], txt, clRed);
			Series2->AddXY(np + 1, valor[1], "", clRed);
			// incrementa pr�ximo valor do eixo x
			np++;
		}
	}
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	RGroupSerie->Items->Add(Series1->Title);
	RGroupSerie->Items->Add(Series2->Title);
	RGroupSerie->ItemIndex = 0;
	ChartClick(NULL);
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartIniciaCurvaPFP_MDP(VTCurva* curva)
{
	// vari�veis locais
	AnsiString txt;
	int np;
	strHM hm;
	double valor_max, valor_min, increment;
	double valor[nvCURVA_PFP_MDP];

	// mostra legenda //DVK 2015.09.08
	chart->Legend->Visible = false; //true;
	// esconde o t�tulo
	chart->Title->Visible = true;
	chart->Title->Text->Clear();
	chart->Title->Text->Add(curva->Codigo);
	// reinicia eixos do Chart
	chart->BottomAxis->LabelStyle = talText;
	// reinicia Serie1
	Series1->Clear();
	Series1->Visible = false; //true;
	Series1->Color = clBlue;
	Series1->Title = "Pot�ncia Ativa";
	// reinicia Serie1
	Series2->Clear();
	Series2->Color = clRed;
	Series2->Visible = false; //true;
	Series2->Title = "Fat.Pot�ncia";
	// exibe valores da Curva
	np = 0;
	for (hm.hora = 0; hm.hora <= 23; hm.hora++)
	{
		txt = IntToStr(hm.hora);
		for (hm.minuto = 0; hm.minuto < 60; hm.minuto += 15)
		{ // obt�m valor da curva
			curva->GetValor(hm, valor, sizeof(valor) / sizeof(double));
			if (hm.minuto != 0)
				txt = "";
			// insere P na Serie2
			Series1->AddXY(np, valor[0], txt, clBlue);
			Series1->AddXY(np + 1, valor[0], "", clBlue);
			// insere FP na Serie2
			Series2->AddXY(np, valor[1], txt, clRed);
			Series2->AddXY(np + 1, valor[1], "", clRed);
			// incrementa pr�ximo valor do eixo x
			np++;
		}
	}
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	RGroupSerie->Items->Add(Series1->Title);
	RGroupSerie->Items->Add(Series2->Title);
	RGroupSerie->ItemIndex = 0;
	ChartClick(NULL);
}

// https://sinapsisenergia.teamwork.com/#/projects/180883/tasks/12226502
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ChartIniciaCurvaLD(VTCurva* curva)
{
	// vari�veis locais
	AnsiString txt;
	int np;
	strHM hm;
	double valor[nvCURVA_LD];

	// esconde legenda //DVK 2015.09.08
	chart->Legend->Visible = false; //true;
	// esconde o t�tulo
	chart->Title->Visible = true;
	chart->Title->Text->Clear();
	chart->Title->Text->Add(curva->Codigo);
	// reinicia eixos do Chart
	chart->BottomAxis->LabelStyle = talText;
	// reinicia Serie1
	Series1->Clear();
	Series1->Visible = false; //true;v
	Series1->Color = clBlue;
	Series1->Title = "0=Desligado, 1=Ligado";
	// esconde Serie2
	Series2->Clear();
	Series2->Visible = false; //true;
	// exibe valores da Curva
	np = 0;
	for (hm.hora = 0; hm.hora <= 23; hm.hora++)
	{
		txt = IntToStr(hm.hora);
		for (hm.minuto = 0; hm.minuto < 60; hm.minuto += 15)
		{ // obt�m valor da curva
			curva->GetValor(hm, valor, sizeof(valor) / sizeof(double));
			if (hm.minuto != 0)
				txt = "";
			// insere Ligado/Desligado na Serie1
			Series1->AddXY(np, valor[0], txt, clBlue);
			Series1->AddXY(np + 1, valor[0], "", clBlue);
			// incrementa pr�ximo valor do eixo x
			np++;
		}
	}
	// redefine valores m�nimo e m�ximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	RGroupSerie->Items->Add(Series1->Title);
	RGroupSerie->Items->Add(Series2->Title);
	RGroupSerie->ItemIndex = 0;
	ChartClick(NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCfgCurvaDemanda::DefineCodigoCurva(AnsiString &codigo)
{
	// vari�veis locais
	bool sucesso;
	TFormEdtTxt *FormEdtTxt;

	// cria TFormEdtTxt e exibe como janela modal
	FormEdtTxt = new TFormEdtTxt(this, "C�digo da Curva");
	FormEdtTxt->Texto = codigo;
	FormEdtTxt->ShowModal();
	sucesso = (FormEdtTxt->ModalResult == mrOk); /* DVK2012.12.18 */
	if (sucesso)
		codigo = FormEdtTxt->Texto;
	delete FormEdtTxt;
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::DefineIdInvalido(TList *lisCURVA)
{
	// vari�veis locais
	VTCurva *curva;

	for (int n = 0; n < lisCURVA->Count; n++)
	{
		curva = (VTCurva*)lisCURVA->Items[n];
		if (curva->Codigo.Trim().IsEmpty())
		{
			curva->Codigo = "Indefinida";
		}
		curva->Id = -1;
		curva->Status[sttNOVO] = true;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCfgCurvaDemanda::EditaCurva(VTCurva *curva)
{
	// vari�veis locais
	bool confirma;
	VTCurva *clone;
	TFormCurva *FormCurva;

	// esconde este Form
	Hide();
	try
	{ // cria um Clone da Curva atual;
		clone = curva->Clone();
		// cria TFormCurva e exibe como janela modal
		FormCurva = new TFormCurva(this, apl);
		// Atualiza a janela de edi��o da curva com os dados atuais
		FormCurva->EditaCurva(clone);
        // Click em "Confirma" para atualizar a curva
		confirma = (FormCurva->ShowModal() == mrOk);
		if (confirma)
		{ // atualiza Curva com dados do Clone alterado
			curva->CopiaAtributosDe(*clone);
			// reinicia TreeView
			TreeViewInicia();
			// atualiza Status da Curva
			curva->Status[sttALTERADO] = true;
			// grava Curva na base de dados
			apl->SalvaBaseCurvaTipica(curva);
		}
		// destr�i clone da Curva e o Form
		delete clone;
		delete FormCurva;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// reapresenta este Form
	Show();
	return (confirma);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ExcluiCurvasExistentes(void)
{
	// vari�veis locais
	VTCurva *curva;

	// exclui curvas t�picas de carga
	if (curvas->ListaCurvaTipicaCarga()->Count > 0)
	{
		if (Confirma("H� curvas t�picas para Consumidores.", "Deseja excluir curvas existentes ?")
			== IDYES)
		{
			for (int n = 0; n < curvas->ListaCurvaTipicaCarga()->Count; n++)
			{
				curva = (VTCurva*)curvas->ListaCurvaTipicaCarga()->Items[n];
				// marca Curva como exclu�da e atualiza base de dados
				curva->Status[sttEXCLUIDO] = true;
				apl->SalvaBaseCurvaTipica(curva);
			}
		}
	}
	// exclui curvas t�picas de IP
	if (curvas->ListaCurvaTipicaIP()->Count > 0)
	{
		if (Confirma("H� curvas t�picas para Ilumina��o P�blica.",
			"Deseja excluir curvas existentes ?") == IDYES)
		{
			for (int n = 0; n < curvas->ListaCurvaTipicaIP()->Count; n++)
			{
				curva = (VTCurva*)curvas->ListaCurvaTipicaIP()->Items[n];
				// marca Curva como exclu�da e atualiza base de dados
				curva->Status[sttEXCLUIDO] = true;
				apl->SalvaBaseCurvaTipica(curva);
			}
		}
	}
	// exclui curvas t�picas de capacitor
	if (curvas->ListaCurvaTipicaCapacitor()->Count > 0)
	{
		if (Confirma("H� curvas t�picas para Capacitores.", "Deseja excluir curvas existentes ?")
			== IDYES)
		{
			for (int n = 0; n < curvas->ListaCurvaTipicaCapacitor()->Count; n++)
			{
				curva = (VTCurva*)curvas->ListaCurvaTipicaCapacitor()->Items[n];
				// marca Curva como exclu�da e atualiza base de dados
				curva->Status[sttEXCLUIDO] = true;
				apl->SalvaBaseCurvaTipica(curva);
			}
		}
	}
	// exclui curvas t�picas de gerador fotovoltaico
	if (curvas->ListaCurvaTipicaGeradorFotovoltaico()->Count > 0)
	{
		// if (Confirma("H� curvas t�picas para Geradores Fotovoltaico.",
		// "Deseja excluir curvas existentes ?") == IDYES)
		if (Confirma("H� curvas t�picas para Geradores.", "Deseja excluir curvas existentes ?")
			== IDYES)
		{
			for (int n = 0; n < curvas->ListaCurvaTipicaGeradorFotovoltaico()->Count; n++)
			{
				curva = (VTCurva*)curvas->ListaCurvaTipicaGeradorFotovoltaico()->Items[n];
				// marca Curva como exclu�da e atualiza base de dados
				curva->Status[sttEXCLUIDO] = true;
				apl->SalvaBaseCurvaTipica(curva);
			}
		}
	}
	// exclui curvas t�picas de reator
	if (curvas->ListaCurvaTipicaReator()->Count > 0)
	{
		if (Confirma("H� curvas t�picas para Reatores.", "Deseja excluir curvas existentes ?")
			== IDYES)
			for (int n = 0; n < curvas->ListaCurvaTipicaReator()->Count; n++)
			{
				curva = (VTCurva*)curvas->ListaCurvaTipicaReator()->Items[n];
				// marca Curva como exclu�da e atualiza base de dados
				curva->Status[sttEXCLUIDO] = true;
				apl->SalvaBaseCurvaTipica(curva);
			}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCfgCurvaDemanda::ExistePlanejamento(void)
{
	// vari�veis locais
	VTPlanejamento *planejamento;
	VTEstudo *estudo;

	// prote��o
	if (apl_child == NULL)
		return (false);
	planejamento = (VTPlanejamento*)apl_child->GetObject(__classid(VTPlanejamento));
	if (planejamento == NULL)
		return (false);
	if ((estudo = planejamento->ExisteEstudo()) != NULL)
	{
		Aviso("Fun��o bloqueada enquanto houver uma rede aberta!");
		return (true);
	}
	return (false);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormCfgCurvaDemanda::ExisteCurva(TList *lisCURVA, VTCurva *curva)
 {
 return(lisCURVA->IndexOf(curva) >= 0);
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormCfgCurvaDemanda::GravaCurvaTipica(VTCurva *curva)
 {
 //vari�veis locais
 VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

 //verifica base de dados em uso
 switch(geral->BaseDado)
 {
 case SNPDBTYPE_ORACLE: GravaCurvaTipicaOracle(curva); break;
 case SNPDBTYPE_ACCESS: GravaCurvaTipicaAccess(curva); break;
 default: return;
 }
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormCfgCurvaDemanda::GravaCurvaTipica(VTCurva *curva)
 {
 //vari�veis locais
 VTBD *bd = (VTBD*)apl->GetObject(__classid(VTBD));

 //prote��o
 if (bd == NULL) return;
 //verifica se � uma curva de carga
 if (ExisteCurva(curvas->ListaCurvaTipicaCarga(), curva))
 {
 // OOPS: Modificado em 20120611
 // bd_curva->GravaCurvaTipicaCarga(curva);
 bd->Geral->SalvaCurvaTipica(eqptoCARGA, curva);
 }
 //verifica se � uma curva de IP
 else if (ExisteCurva(curvas->ListaCurvaTipicaIP(), curva))
 {
 // OOPS: Modificado em 20120611
 // bd_curva->GravaCurvaTipicaIP(curva);
 bd->Geral->SalvaCurvaTipica(eqptoCARGA, curva);
 }
 //verifica se � uma curva de capacitor
 else if (ExisteCurva(curvas->ListaCurvaTipicaCapacitor(), curva))
 {
 // OOPS: Modificado em 20120611
 // bd_curva->GravaCurvaTipicaCapacitor(curva);
 bd->Geral->SalvaCurvaTipica(eqptoCAPACITOR, curva);
 }
 //verifica se � uma curva de reator
 else if (ExisteCurva(curvas->ListaCurvaTipicaReator(), curva))
 {
 // OOPS: Modificado em 20120611
 // bd_curva->GravaCurvaTipicaReator(curva);
 bd->Geral->SalvaCurvaTipica(eqptoREATOR, curva);
 }
 else
 {
 Erro("Curva indefinida: a base de dados n�o pode ser atualizada");
 }
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormCfgCurvaDemanda::GravaCurvaTipicaOracle(VTCurva *curva)
 {
 //vari�veis locais
 VTBD *bd = (VTBD*)apl->GetObject(__classid(VTBD));

 //prote��o
 if (bd == NULL) return;
 //salva todas as Curvas na base oracle
 if (bd->ConectaServidorRedeRemoto())
 {
 bd->Geral->SalvaCurvaTipica();
 }
 else
 {
 Aviso("N�o foi poss�vel conectar com a base Oracle");
 }
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormCfgCurvaDemanda::HabilitaComandosUsuarioPrivilegiado(void)
 {
 //vari�veis locais
 VTGeral    *geral    = (VTGeral*)apl->GetObject(__classid(VTGeral));
 VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
 VTLicenca  *licenca  = licencas->ExisteLicenca();

 ActionSalvaOracle->Visible = ((geral->BaseDado == SNPDBTYPE_ACCESS)&&(licenca->Tipo == tipoMASTER));
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::InsereCurvaConsumidor(VTClasse *classe)
{
	// vari�veis locais
	AnsiString codigo = "Nova curva";
	VTCurva *curva;

	// solicita c�digo
	if (!DefineCodigoCurva(codigo))
		return;
	// cria uma nova Curva e insere em Curvas
	curva = DLL_NewObjCurva();
	curvas->InsereCurvaTipicaCarga(curva);
	curva->Codigo = codigo;
	curva->Id = FalsoId();
	curva->Tipo = curvaPFP_MDP;
	curva->Unidade = unidPU_MED;
	curva->Tipica = true;
	curva->Escala = pat01HORA;
	curva->NumeroValores = nvCURVA_PFP_MDP;
	curva->Eqpto = NULL;
	curva->Classe = classe;
	curva->LimiteInferior = 0.;
	curva->LimiteSuperior = 0.;
	// reconfigura a Curva (muda n�mero de pontos, salvando seus valores originais)
	curva->Reconfigura(pat01HORA);
	// atualiza Status da Curva
	curva->Status[sttNOVO] = true;
	// edita a Curva
	if (!EditaCurva(curva))
	{ // destr�i Curva
		(curvas->ListaCurvaTipicaCarga())->Remove(curva);
		delete curva;
	}
	else
	{ // atualiza TreeView
		TreeViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::InsereCurvaCapacitor(void)
{
	// vari�veis locais
	AnsiString codigo = "Nova curva";
	VTCurva *curva;

	// solicita c�digo
	if (!DefineCodigoCurva(codigo))
		return;
	// cria uma nova Curva e insere em Curvas
	curva = DLL_NewObjCurva();
	curvas->InsereCurvaTipicaCapacitor(curva);
	curva->Codigo = codigo;
	curva->Id = FalsoId();
	curva->Tipo = curvaLD;
	curva->Unidade = unidON_OFF;
	curva->Tipica = true;
	curva->Escala = pat01HORA;
	curva->NumeroValores = nvCURVA_LD;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0.;
	curva->LimiteSuperior = 0.;
	// reconfigura a Curva (muda n�mero de pontos, salvando seus valores originais)
	curva->Reconfigura(pat01HORA);
	// atualiza Status da Curva
	curva->Status[sttNOVO] = true;
	// edita a Curva
	if (!EditaCurva(curva))
	{ // destr�i Curva
		(curvas->ListaCurvaTipicaCapacitor())->Remove(curva);
		delete curva;
	}
	else
	{
		// atualiza TreeView
		TreeViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::InsereCurvaGeradorFotovoltaico(void)
{
	// vari�veis locais
	AnsiString codigo = "Nova curva";
	VTCurva *curva;

	// solicita c�digo
	if (!DefineCodigoCurva(codigo))
		return;
	// cria uma nova Curva e insere em Curvas
	curva = DLL_NewObjCurva();
	curvas->InsereCurvaTipicaGeradorFotovoltaico(curva);
	curva->Codigo = codigo;
	curva->Id = FalsoId();
	curva->Tipo = curvaPQVT;
	curva->Unidade = unidPU_MED;
	curva->Tipica = true;
	curva->Escala = pat01HORA;
	curva->NumeroValores = nvCURVA_PQVT;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0.;
	curva->LimiteSuperior = 0.;
	// reconfigura a Curva (muda n�mero de pontos, salvando seus valores originais)
	curva->Reconfigura(pat01HORA);
	// atualiza Status da Curva
	curva->Status[sttNOVO] = true;
	// edita a Curva
	if (!EditaCurva(curva))
	{ // destr�i Curva
		(curvas->ListaCurvaTipicaGeradorFotovoltaico())->Remove(curva);
		delete curva;
	}
	else
	{ // atualiza TreeView
		TreeViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::InsereCurvaIP(void)
{
	// vari�veis locais
	AnsiString codigo = "Nova curva";
	VTCurva *curva;

	// solicita c�digo
	if (!DefineCodigoCurva(codigo))
		return;
	// cria uma nova Curva e insere em Curvas
	curva = DLL_NewObjCurva();
	curvas->InsereCurvaTipicaIP(curva);
	curva->Codigo = codigo;
	curva->Id = FalsoId();
	curva->Tipo = curvaLD;
	curva->Unidade = unidON_OFF;
	curva->Tipica = true;
	curva->Escala = pat01HORA;
	curva->NumeroValores = nvCURVA_LD;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0.;
	curva->LimiteSuperior = 0.;
	// reconfigura a Curva (muda n�mero de pontos, salvando seus valores originais)
	curva->Reconfigura(pat01HORA);
	// atualiza Status da Curva
	curva->Status[sttNOVO] = true;
	// edita a Curva
	if (!EditaCurva(curva))
	{ // destr�i Curva
		(curvas->ListaCurvaTipicaIP())->Remove(curva);
		delete curva;
	}
	else
	{
		// atualiza TreeView
		TreeViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::InsereCurvaReator(void)
{
	// vari�veis locais
	AnsiString codigo = "Nova curva";
	VTCurva *curva;

	// solicita c�digo
	if (!DefineCodigoCurva(codigo))
		return;
	// cria uma nova Curva e insere em Curvas
	curva = DLL_NewObjCurva();
	curvas->InsereCurvaTipicaReator(curva);
	curva->Codigo = codigo;
	curva->Id = FalsoId();
	curva->Tipo = curvaLD;
	curva->Unidade = unidON_OFF;
	curva->Tipica = true;
	curva->Escala = pat01HORA;
	curva->NumeroValores = nvCURVA_LD;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0.;
	curva->LimiteSuperior = 0.;
	// reconfigura a Curva (muda n�mero de pontos, salvando seus valores originais)
	curva->Reconfigura(pat01HORA);
	// atualiza Status da Curva
	curva->Status[sttNOVO] = true;
	// edita a Curva
	if (!EditaCurva(curva))
	{ // destr�i Curva
		(curvas->ListaCurvaTipicaReator())->Remove(curva);
		delete curva;
	}
	else
	{
		// atualiza TreeView
		TreeViewInicia();
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TFormCfgCurvaDemanda::LisCurvaTipicaCarga(VTClasse *classe, int dia_medicao)
{
	// monta lisCURVA c/ as Curvas t�picas da classe indicada
	lisCURVA->Clear();
	curvas->LisCurvaTipicaCarga(classe, dia_medicao, lisCURVA);
	return (lisCURVA);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::ReiniciaBaseDados(void)
{
	// verifica se h� Curvas
	if ((curvas->ListaCurvaTipicaCapacitor()->Count + curvas->ListaCurvaTipicaCarga()->Count +
		curvas->ListaCurvaTipicaGeradorFotovoltaico()->Count + curvas->ListaCurvaTipicaIP()->Count +
		curvas->ListaCurvaTipicaReator()->Count) > 0)
	{ // verifica se o usu�rio deseja excluir as curvas existentes
		if (Confirma("H� curvas t�picas gravadas na base de dados",
			"Deseja excluir curvas existentes ?") == IDYES)
		{
			ExcluiCurvasExistentes();
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::RichEditIniciaCurvaPQ(VTCurva* curva)
{
	// reinicia RichEdit
	RichEdit->Lines->Clear();
	RichEdit->Lines->Add("Curva:\t\t" + curva->Codigo);
	if ((curva->Classe != NULL) && (curva->Classe->Tag != consIP))
	{
		RichEdit->Lines->Add("Medi��o:\t\t" + curva->DiaMedicaoAsStr);
	}
	RichEdit->Lines->Add("Dados:\t\t" + curva->TipoAsStr);
	RichEdit->Lines->Add("Pontos:\t\t" + IntToStr(curva->NumeroPontos()));
	// verifica se curva de carga (associada a uma classe)
	if (curva->Classe != NULL)
	{
		RichEdit->Lines->Add("Classe:\t\t" + curva->Classe->Codigo);
		// verifica se Curva tipica
		if (curva->Tipica)
		{
			if (curva->LimiteInferior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("abaixo de %3.f kWh/m�s",
					curva->LimiteSuperior));
			}
			else if (curva->LimiteSuperior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("acima de %3.f kWh/m�s",
					curva->LimiteInferior));
			}
			else
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("entre %3.f e ",
					curva->LimiteInferior) + DoubleToStr("%3.f kWh/m�s", curva->LimiteSuperior));
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::RichEditIniciaCurvaPQVT(VTCurva* curva)
{
	// reinicia RichEdit
	RichEdit->Lines->Clear();
	RichEdit->Lines->Add("Curva:\t\t" + curva->Codigo);
	if ((curva->Classe != NULL) && (curva->Classe->Tag != consIP))
	{
		RichEdit->Lines->Add("Medi��o:\t\t" + curva->DiaMedicaoAsStr);
	}
	RichEdit->Lines->Add("Dados:\t\t" + curva->TipoAsStr);
	RichEdit->Lines->Add("Pontos:\t\t" + IntToStr(curva->NumeroPontos()));
	RichEdit->Lines->Add("Unidade:\t\t" + UnidadeCurva(curva->Unidade));
	// verifica se curva de carga (associada a uma classe)
	if (curva->Classe != NULL)
	{
		RichEdit->Lines->Add("Classe:\t\t" + curva->Classe->Codigo);
		// verifica se Curva tipica
		if (curva->Tipica)
		{
			if (curva->LimiteInferior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("abaixo de %3.f kWh/m�s",
					curva->LimiteSuperior));
			}
			else if (curva->LimiteSuperior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("acima de %3.f kWh/m�s",
					curva->LimiteInferior));
			}
			else
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("entre %3.f e ",
					curva->LimiteInferior) + DoubleToStr("%3.f kWh/m�s", curva->LimiteSuperior));
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::RichEditIniciaCurvaPQ_MDP(VTCurva* curva)
{
	// reinicia RichEdit
	RichEdit->Lines->Clear();
	RichEdit->Lines->Add("Curva:\t\t" + curva->Codigo);
	if ((curva->Classe != NULL) && (curva->Classe->Tag != consIP))
	{
		RichEdit->Lines->Add("Medi��o:\t\t" + curva->DiaMedicaoAsStr);
	}
	RichEdit->Lines->Add("Dados:\t\t" + curva->TipoAsStr);
	RichEdit->Lines->Add("Pontos:\t\t" + IntToStr(curva->NumeroPontos()));
	RichEdit->Lines->Add("Unidade:\t\t" + UnidadeCurva(curva->Unidade));
	// verifica se curva de carga (associada a uma classe)
	if (curva->Classe != NULL)
	{
		RichEdit->Lines->Add("Classe:\t\t" + curva->Classe->Codigo);
		// verifica se Curva tipica
		if (curva->Tipica)
		{
			if (curva->LimiteInferior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("abaixo de %3.f kWh/m�s",
					curva->LimiteSuperior));
			}
			else if (curva->LimiteSuperior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("acima de %3.f kWh/m�s",
					curva->LimiteInferior));
			}
			else
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("entre %3.f e ",
					curva->LimiteInferior) + DoubleToStr("%3.f kWh/m�s", curva->LimiteSuperior));
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::RichEditIniciaCurvaPFP(VTCurva* curva)
{
	// reinicia RichEdit
	RichEdit->Lines->Clear();
	RichEdit->Lines->Add("Curva:\t\t" + curva->Codigo);
	if ((curva->Classe != NULL) && (curva->Classe->Tag != consIP))
	{
		RichEdit->Lines->Add("Medi��o:\t\t" + curva->DiaMedicaoAsStr);
	}
	RichEdit->Lines->Add("Dados:\t\t" + curva->TipoAsStr);
	RichEdit->Lines->Add("Pontos:\t\t" + IntToStr(curva->NumeroPontos()));
	// verifica se curva de carga (associada a uma classe)
	if (curva->Classe != NULL)
	{
		RichEdit->Lines->Add("Classe:\t\t" + curva->Classe->Codigo);
		// verifica se Curva tipica
		if (curva->Tipica)
		{
			if (curva->LimiteInferior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("abaixo de %3.f kWh/m�s",
					curva->LimiteSuperior));
			}
			else if (curva->LimiteSuperior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("acima de %3.f kWh/m�s",
					curva->LimiteInferior));
			}
			else
			{
				RichEdit->Lines->Add("Consumo:\t " + DoubleToStr("entre %3.f e ",
					curva->LimiteInferior) + DoubleToStr("%3.f kWh/m�s", curva->LimiteSuperior));
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::RichEditIniciaCurvaPFP_MDP(VTCurva* curva)
{
	// reinicia RichEdit
	RichEdit->Lines->Clear();
	RichEdit->Lines->Add("Curva:\t\t" + curva->Codigo);
	if ((curva->Classe != NULL) && (curva->Classe->Tag != consIP))
	{
		RichEdit->Lines->Add("Medi��o:\t\t" + curva->DiaMedicaoAsStr);
	}
	RichEdit->Lines->Add("Dados:\t\t" + curva->TipoAsStr);
	RichEdit->Lines->Add("Pontos:\t\t" + IntToStr(curva->NumeroPontos()));
	RichEdit->Lines->Add("Unidade:\t\t" + UnidadeCurva(curva->Unidade));
	// verifica se curva de carga (associada a uma classe)
	if (curva->Classe != NULL)
	{
		RichEdit->Lines->Add("Classe:\t\t" + curva->Classe->Codigo);
		// verifica se Curva tipica    /*DVK2012.12.18 demanda*/
		if ((curva->Tipica) && ((curva->Classe->Tag == consA2) || (curva->Classe->Tag == consA4)))
		{
			if (curva->LimiteInferior < 0)
			{
				RichEdit->Lines->Add("Demanda::\t" + DoubleToStr("abaixo de %3.f kW/m�s",
					curva->LimiteSuperior));
			}
			else if (curva->LimiteSuperior < 0)
			{
				RichEdit->Lines->Add("Demanda::\t" + DoubleToStr("acima de %3.f kW/m�s",
					curva->LimiteInferior));
			}
			else
			{
				RichEdit->Lines->Add("Demanda::\t" + DoubleToStr("entre %3.f e ",
					curva->LimiteInferior) + DoubleToStr("%3.f kW/m�s", curva->LimiteSuperior));
			}
		}
		if ((curva->Tipica) && ((curva->Classe->Tag != consA2) && (curva->Classe->Tag != consA4)))
		{
			if (curva->LimiteInferior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("abaixo de %3.f kWh/m�s",
					curva->LimiteSuperior));
			}
			else if (curva->LimiteSuperior < 0)
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("acima de %3.f kWh/m�s",
					curva->LimiteInferior));
			}
			else
			{
				RichEdit->Lines->Add("Consumo:\t" + DoubleToStr("entre %3.f e ",
					curva->LimiteInferior) + DoubleToStr("%3.f kWh/m�s", curva->LimiteSuperior));
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::RichEditIniciaCurvaLD(VTCurva* curva)
{
	// reinicia RichEdit
	RichEdit->Lines->Clear();
	RichEdit->Lines->Add("Curva:\t\t" + curva->Codigo);
	if ((curva->Classe != NULL) && (curva->Classe->Tag != consIP))
	{
		RichEdit->Lines->Add("Medi��o:\t\t" + curva->DiaMedicaoAsStr);
	}
	RichEdit->Lines->Add("Dados:\t\t" + curva->TipoAsStr);
	RichEdit->Lines->Add("Pontos:\t\t" + IntToStr(curva->NumeroPontos()));
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::SGridIniciaCurvaPQ(VTCurva *curva)
{
	// vari�veis locais
	AnsiString txt;
	char *tit_col[] =
	{"In�cio", "Fim", "P", "Q"};
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PQ];
	int num_pto = curva->NumeroPontos();

	// reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++)
	{
		SGrid->Rows[nr]->Clear();
	}
	// redimensiona SGrid
	SGrid->ColCount = sizeof(tit_col) / sizeof(char*);
	SGrid->RowCount = num_pto + 1;
	// preenche t�tulos das colunas
	for (int nc = 0; nc < SGrid->ColCount; nc++)
	{
		SGrid->Cells[nc][0] = tit_col[nc];
	}
	// preenche valores da Curva
	for (int np = 0, nr = 1; np < num_pto; np++, nr++)
	{ // obt�m ponto da curva
		curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));
		// preenche hora inicial
		txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
		SGrid->Cells[0][nr] = txt;
		// preenche hora final
		txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
		SGrid->Cells[1][nr] = txt;
		// preenche valores de P e Q
		SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
		SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::SGridIniciaCurvaPQVT(VTCurva *curva)
{
	// vari�veis locais
	AnsiString txt;
	char *tit_col[] =
	{"In�cio", "Fim", "P", "Q", "V", "F"};
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PQVT];
	int num_pto = curva->NumeroPontos();

	// reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++)
	{
		SGrid->Rows[nr]->Clear();
	}
	// redimensiona SGrid
	SGrid->ColCount = sizeof(tit_col) / sizeof(char*);
	SGrid->RowCount = num_pto + 1;
	// preenche t�tulos das colunas
	for (int nc = 0; nc < SGrid->ColCount; nc++)
	{
		SGrid->Cells[nc][0] = tit_col[nc];
	}
	// preenche valores da Curva
	for (int np = 0, nr = 1; np < num_pto; np++, nr++)
	{ // obt�m ponto da curva
		curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));
		// preenche hora inicial
		txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
		SGrid->Cells[0][nr] = txt;
		// preenche hora final
		txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
		SGrid->Cells[1][nr] = txt;
		// preenche valores de P e Q
		SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
		SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
		SGrid->Cells[4][nr] = DoubleToStr("%4.3f", valor[2]);
		SGrid->Cells[5][nr] = DoubleToStr("%4.3f", valor[3]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::SGridIniciaCurvaPQ_MDP(VTCurva *curva)
{
	// vari�veis locais
	AnsiString txt;
	char *tit_col[] =
	{"In�cio", "Fim", "P", "Q", "dp(P)", "dp(Q)"};
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PQ_MDP];
	int num_pto = curva->NumeroPontos();

	// reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++)
	{
		SGrid->Rows[nr]->Clear();
	}
	// redimensiona SGrid
	SGrid->ColCount = sizeof(tit_col) / sizeof(char*);
	SGrid->RowCount = num_pto + 1;
	// preenche t�tulos das colunas
	for (int nc = 0; nc < SGrid->ColCount; nc++)
	{
		SGrid->Cells[nc][0] = tit_col[nc];
	}
	// preenche valores da Curva
	for (int np = 0, nr = 1; np < num_pto; np++, nr++)
	{ // obt�m ponto da curva
		curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));
		// preenche hora inicial
		txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
		SGrid->Cells[0][nr] = txt;
		// preenche hora final
		txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
		SGrid->Cells[1][nr] = txt;
		// preenche valores de P, dp[P], Q, dp(Q)
		SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
		SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
		SGrid->Cells[4][nr] = DoubleToStr("%4.3f", valor[2]);
		SGrid->Cells[5][nr] = DoubleToStr("%4.3f", valor[3]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::SGridIniciaCurvaPFP(VTCurva *curva)
{
	// vari�veis locais
	AnsiString txt;
	char *tit_col[] =
	{"In�cio", "Fim", "P", "FP"};
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PFP];
	int num_pto = curva->NumeroPontos();

	// reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++)
	{
		SGrid->Rows[nr]->Clear();
	}
	// redimensiona SGrid
	SGrid->ColCount = sizeof(tit_col) / sizeof(char*);
	SGrid->RowCount = num_pto + 1;
	// preenche t�tulos das colunas
	for (int nc = 0; nc < SGrid->ColCount; nc++)
	{
		SGrid->Cells[nc][0] = tit_col[nc];
	}
	// preenche valores da Curva
	for (int np = 0, nr = 1; np < num_pto; np++, nr++)
	{ // obt�m ponto da curva
		curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));
		// preenche hora inicial
		txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
		SGrid->Cells[0][nr] = txt;
		// preenche hora final
		txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
		SGrid->Cells[1][nr] = txt;
		// preenche valores de P, dp[P], Q, dp(Q)
		SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
		SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::SGridIniciaCurvaPFP_MDP(VTCurva *curva)
{
	// vari�veis locais
	AnsiString txt;
	char *tit_col[] =
	{"In�cio", "Fim", "P", "FP", "dp(P)", "dp(FP)"};
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PFP_MDP];
	int num_pto = curva->NumeroPontos();

	// reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++)
	{
		SGrid->Rows[nr]->Clear();
	}
	// redimensiona SGrid
	SGrid->ColCount = sizeof(tit_col) / sizeof(char*);
	SGrid->RowCount = num_pto + 1;
	// preenche t�tulos das colunas
	for (int nc = 0; nc < SGrid->ColCount; nc++)
	{
		SGrid->Cells[nc][0] = tit_col[nc];
	}
	// preenche valores da Curva
	for (int np = 0, nr = 1; np < num_pto; np++, nr++)
	{ // obt�m ponto da curva
		curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));
		// preenche hora inicial
		txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
		SGrid->Cells[0][nr] = txt;
		// preenche hora final
		txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
		SGrid->Cells[1][nr] = txt;
		// preenche valores de P, dp[P], Q, dp(Q)
		SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
		SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
		SGrid->Cells[4][nr] = DoubleToStr("%4.3f", valor[2]);
		SGrid->Cells[5][nr] = DoubleToStr("%4.3f", valor[3]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::SGridIniciaCurvaLD(VTCurva *curva)
{
	// vari�veis locais
	AnsiString txt;
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_LD];
	char *tit_col[] =
	{"In�cio", "Fim", "Ligado"};
	int num_pto = curva->NumeroPontos();

	// reinicia SGrid
	for (int nr = 0; nr < SGrid->RowCount; nr++)
	{
		SGrid->Rows[nr]->Clear();
	}
	// redimensiona SGrid
	SGrid->ColCount = sizeof(tit_col) / sizeof(char*);
	SGrid->RowCount = num_pto + 1;
	// preenche t�tulos das colunas
	for (int nc = 0; nc < SGrid->ColCount; nc++)
	{
		SGrid->Cells[nc][0] = tit_col[nc];
	}
	// preenche valores da Curva
	for (int np = 0, nr = 1; np < num_pto; np++, nr++)
	{ // obt�m ponto da curva
		curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor) / sizeof(double));
		// preenche hora inicial
		txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
		SGrid->Cells[0][nr] = txt;
		// preenche hora final
		txt.sprintf("%02d:%02d", hm_fim.hora, hm_fim.minuto);
		SGrid->Cells[1][nr] = txt;
		// preenche valor de Ligado
		SGrid->Cells[2][nr] = IntToStr(int(valor[0]));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
	// vari�veis locais
	VTCurva *curva;

	RGroupSerie->Items->Clear();
	// prote��o
	if (Node != TreeView->Selected)
		return;
	// determina Curva selecionada
	if ((curva = TreeViewCurvaSelecionada()) == NULL)
	{ // esconde GroupBoxCurva
		GroupBoxCurva->Visible = false;
		return;
	}
	// exibe GroupBoxCurva
	GroupBoxCurva->Visible = true;
	GroupBoxCurva->Caption = curva->Codigo;
	// verifica o tipo de Curva
	switch (curva->Tipo)
	{
	case curvaPQ:
		RichEditIniciaCurvaPQ(curva);
		SGridIniciaCurvaPQ(curva);
		ChartIniciaCurvaPQ(curva);
		break;
	case curvaPQVT:
		RichEditIniciaCurvaPQVT(curva);
		SGridIniciaCurvaPQVT(curva);
		ChartIniciaCurvaPQVT(curva);
		break;
	case curvaPQ_MDP:
		RichEditIniciaCurvaPQ_MDP(curva);
		SGridIniciaCurvaPQ_MDP(curva);
		ChartIniciaCurvaPQ_MDP(curva);
		break;
	case curvaPFP:
		RichEditIniciaCurvaPFP(curva);
		SGridIniciaCurvaPFP(curva);
		ChartIniciaCurvaPFP(curva);
		break;
	case curvaPFP_MDP:
		RichEditIniciaCurvaPFP_MDP(curva);
		SGridIniciaCurvaPFP_MDP(curva);
		ChartIniciaCurvaPFP_MDP(curva);
		break;
	case curvaLD:
		RichEditIniciaCurvaLD(curva);
		SGridIniciaCurvaLD(curva);
		ChartIniciaCurvaLD(curva);
		break;
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
VTClasse* __fastcall TFormCfgCurvaDemanda::TreeViewClasseSelecionada(void)
{
	// vari�veis locais
	AnsiString meta_class_name, obj_class_name;
	TMetaClass *meta_class;
	TObject *object;

	// prote��o
	if (TreeView->Selected == NULL)
		return (NULL);
	if (TreeView->Selected->Data == NULL)
		return (NULL);
	// determina class name da classe VTClasse
	meta_class = __classid(VTClasse);
	meta_class_name = meta_class->ClassName();
	// verifica se o objeto associado ao TreeNode � uma Classe
	object = (TObject*)TreeView->Selected->Data;
	meta_class = object->ClassType();
	while (meta_class)
	{
		obj_class_name = meta_class->ClassName();
		if (obj_class_name.AnsiCompareIC(meta_class_name) == 0)
			return ((VTClasse*)object);
		meta_class = meta_class->ClassParent();
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormCfgCurvaDemanda::TreeViewCurvaSelecionada(void)
{
	// vari�veis locais
	AnsiString meta_class_name, obj_class_name;
	TMetaClass *meta_class;
	TObject *object;

	// prote��o
	if (TreeView->Selected == NULL)
		return (NULL);
	if (TreeView->Selected->Data == NULL)
		return (NULL);
	// determina class name da classe VTCurva
	meta_class = __classid(VTCurva);
	meta_class_name = meta_class->ClassName();
	// verifica se o objeto associado ao TreeNode � uma Curva
	object = (TObject*)TreeView->Selected->Data;
	meta_class = object->ClassType();
	while (meta_class)
	{
		obj_class_name = meta_class->ClassName();
		if (obj_class_name.AnsiCompareIC(meta_class_name) == 0)
			return ((VTCurva*)object);
		meta_class = meta_class->ClassParent();
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::TreeViewDblClick(TObject *Sender)
{
	// vari�veis locais
	VTCurva *curva;

	// determina Curva selecionada
	if ((curva = TreeViewCurvaSelecionada()) == NULL)
		return;
	// edita Curva
	EditaCurva(curva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::TreeViewInicia(void)
{
	// vari�veis locais
	int vet_dia[] =
	{DOMINGO, SABADO, DIA_UTIL, SEMANA_COMPLETA};
	TTreeNode *node_raiz, *node_dia, *node_classe;
	VTClasse *classe;
	VTCurva *curva;
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	TList *lisCLASSE = classes->LisClasse();

	// bloqueia tratamento do evento OnChange
	TreeView->OnChange = NULL;
	// reinicia TreeView
	TreeView->Items->Clear();
	// cria uma curva tempor�ria apenas para obter string DiaMedicao
	curva = DLL_NewObjCurva();
	// insere node_raiz para curvas t�picas de demanda
	node_raiz = TreeView->Items->Add(NULL, "Curvas de Demanda de consumidores");
	// loop p/ todos os conjuntos de curvas t�picas
	for (int n = 0; n < (sizeof(vet_dia) / sizeof(int)); n++)
	{ // atualiza DiaMedicao da Curva tempor�ria
		curva->DiaMedicao = vet_dia[n];
		// insere TreeNode com dia das Curvas t�picas
		node_dia = TreeView->Items->AddChild(node_raiz, curva->DiaMedicaoAsStr);
		// loop p/ todas classes de consumidor
		for (int nc = 0; nc < lisCLASSE->Count; nc++)
		{
			classe = (VTClasse*)lisCLASSE->Items[nc];
			if (classe->Tag == consIP)
				continue;
			if (classe->Tag == consEQUIVALENTE)
				continue;
			// insere TreeNodes c/ curvas t�picas da Classe do consumidor
			node_classe = TreeView->Items->AddChildObject(node_dia, "Consumidor " + classe->Codigo,
				classe);
			TreeViewInsereLisCurva(node_classe, LisCurvaTipicaCarga(classe, vet_dia[n]));
		}
	}
	// insere TreeNodes c/ curvas t�picas de IP
	classe = classes->ExisteClasseTag(consIP);
	node_raiz = TreeView->Items->AddObject(NULL, "Curvas de Ilumina��o P�blica", classe);
	TreeViewInsereLisCurva(node_raiz, curvas->ListaCurvaTipicaIP());
	// insere TreeNodes c/ curvas t�picas de Capacitor
	node_raiz = TreeView->Items->AddObject(NULL, "Curvas de Capacitor", NULL);
	TreeViewInsereLisCurva(node_raiz, curvas->ListaCurvaTipicaCapacitor());
	// insere TreeNodes c/ curvas t�picas de Reator
	node_raiz = TreeView->Items->AddObject(NULL, "Curvas de Reator", NULL);
	TreeViewInsereLisCurva(node_raiz, curvas->ListaCurvaTipicaReator());
	// insere TreeNodes c/ curvas t�picas de GeradorFotovoltaico
	// node_raiz = TreeView->Items->AddObject(NULL, "Curvas de Gerador Fotovoltaico", NULL);
	node_raiz = TreeView->Items->AddObject(NULL, "Curvas de Gerador", NULL);
	TreeViewInsereLisCurva(node_raiz, curvas->ListaCurvaTipicaGeradorFotovoltaico());
	// reabilita tratamento do evento OnChange
	TreeView->OnChange = TreeViewChange;
	// seleciona TreeNode
	TreeView->Selected = TreeView->Items->Item[0];
	// destr�i objeto Curva tempor�rio
	delete curva;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgCurvaDemanda::TreeViewInsereLisCurva(TTreeNode *node_pai, TList *lisCURVA)
{
	// vari�veis locais
	VTClasse *classe;
	VTCurva *curva;

	// loop p/ todas Curvas
	for (int n = 0; n < lisCURVA->Count; n++)
	{
		curva = (VTCurva*)lisCURVA->Items[n];
		// verifica se � uma Curva excluida
		if (curva->Status[sttEXCLUIDO])
			continue;
		// insere TreeNodes c/ curvas t�picas de IP
		TreeView->Items->AddChildObject(node_pai, curva->Codigo, curva);
		// node = TreeView->Items->AddChildObject(node_pai, curva->Codigo, curva);
		// insere TreeNodes c/ dados da Curva
		// TreeView->Items->AddChildObject(node, "Tipo:      " + curva->TipoAsStr, curva);
		// TreeView->Items->AddChildObject(node, "Patamares: " + IntToStr(curva->NumeroPontos()), curva);
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormCfgCurvaDemanda::UnidadeCurva(int unidade)
{
	switch (unidade)
	{
	case unidKV:
		return ("kV");
	case unidKVA:
		return ("kVA");
		// case unidMVA: 		return("MVA"); //DVK 2015.09.08 redefinido p/fotovoltaico
	case unidMVA:
		return ("Pot�ncia (MVA)");
	case unidPU:
		return ("pu");
	case unidPU_MED:
		return ("pu m�dio");
	case unidPU_MAX:
		return ("pu m�ximo");
	case unidA:
		return ("A");
	case unidON_OFF:
		return ("Ligado/Desligado");
	default:
		return (" ");
	}
}
//---------------------------------------------------------------------------

