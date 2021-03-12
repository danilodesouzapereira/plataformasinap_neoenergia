// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAbre.h"
#include "TFormEstudo.h"
#include "TFormRede.h"
#include "VTAbre.h"
#include "Esquematico\VTEsquematico.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Editor\VTEdt.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Mercado\VTMercado.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormAbre::TFormAbre(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	// vari�veis locais
	VTGrafico *grafico;
	bool rede_aberta = false;

	// cria objeto Apl local
	apl = NewObjApl(this, apl_owner);
	// cria objeto Abre
	apl->Add(abre = NewObjAbre(this, apl));
	// configura op��es de tela em fun��o de base Access/Oracle
	ConfiguraTela();
	// cria Forms com TreeView de redes e Estudos
	FormEstudo = new TFormEstudo(this, apl, GBoxEstudo);
	FormRede = new TFormRede(this, apl, GBoxRede);
	// cria listas
	lisEQP = new TList();
	lisREDE_PRE = new TList();
	// Propriedades dos bot�es
	butSelecionaRede->Down = true;
	// insere Grafico em PanelGrafico
	grafico = (VTGrafico*)abre->apl->GetObject(__classid(VTGrafico));
	grafico->DefineParent(PanelGrafico);
	grafico->Show(grafTOPO);
	// cria objeto TEdtBusca
	edt = DLL_NewObjEdtBusca(abre->apl, NULL, NULL, ActionSelRedeGrafico);
	// habilita/desabilita bot�es em fun��o das redes abertas (preenchendo lisREDE_PRE)
	rede_aberta = (!SelecaoInicial());
	ActionSelecionaBase->Enabled = (!rede_aberta);
	ActionCriaPlanejamento->Enabled = (!rede_aberta);
	ActionCriaReclamacaoBT->Enabled = (!rede_aberta);
}

// ---------------------------------------------------------------------------
__fastcall TFormAbre::~TFormAbre(void)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)abre->apl->GetObject(__classid(VTGrafico));

	// IMPORTANTE: fecha o gr�fico
	if (grafico)
		grafico->Close();
	// destr�i Forms
	if (FormEstudo)
	{
		FormEstudo->Desabilita();
		delete FormEstudo;
		FormEstudo = NULL;
	}
	if (FormRede)
	{
		FormRede->Desabilita();
		delete FormRede;
		FormRede = NULL;
	}
	// destr�i objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisREDE_PRE)
	{
		delete lisREDE_PRE;
		lisREDE_PRE = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionAbrirEstudoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modPLANEJAMENTO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	if (FormEstudo->AbreEstudo())
	{ // fecha este form
		ModalResult = mrOk;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionCancelaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionCancelaSelExecute(TObject *Sender)
{
	// exibe redes da base
	FormRede->CancelaSelecao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionConfirmaAllExecute(TObject *Sender)
{
	try
	{
		Hide();
		// carrega todas as Redes do Estudo 0
		if (FormRede->CarregaRedeCompleta())
		{ // fecha este form
			// � um tipo de prote��o (DVC)
			ModalResult = mrOk;
		}
	}
	catch(...)
	{
		Show();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionCriaPlanejamentoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modPLANEJAMENTO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	if (FormRede->CriaEstudoPlanejamento())
	{ // fecha este form
		ModalResult = mrOk;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionCriaReclamacaoBTExecute(TObject *Sender)
{
	if (FormRede->CriaEstudoReclamacaoBT())
	{ // fecha este form
		ModalResult = mrOk;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionExcluirEstudoExecute(TObject *Sender)
{
	FormEstudo->ExcluiEstudo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionConfirmaSelExecute(TObject *Sender)
{
	try
	{
		Hide();
		// verifica se tem rede aberta
		if (lisREDE_PRE->Count > 0)
		{
			if (FormRede->CarregaRedeAdicional(lisREDE_PRE))
			{ // fecha este Form
				ModalResult = mrOk;
			}
		}
		else
		{
			if(FormRede->CarregaRedeSelecionada())
			{ // fecha este Form
				ModalResult = mrOk;
			}
			else
			{
			    Show();
            }
		}
	}
	catch(...)
	{
	   Show();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionSelecionaBaseExecute(TObject *Sender)
{
	// seleciona a base da Rede
	if (SelecionaBaseRede())
	{
		ExibeDadosBaseSelecionada();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionSelecionaEstudoExecute(TObject *Sender)
{
	// habilita os bot�es espec�ficos no ToolBar principal
	butSelecionaEstudo->Down = true;
	ActionCancelaSel->Visible = false;
	ActionCriaPlanejamento->Visible = false;
	ActionCriaReclamacaoBT->Visible = false;
	butAbrirRede->Visible = false;
	butRedesSel->Visible = false;
	butAbrirEstudo->Visible = true;
	butExcluirEstudo->Visible = true;
	// desabilita FormRede
	GBoxRede->Visible = false;
	FormRede->Desabilita();
	// habilita FormEstudo
	GBoxEstudo->Align = alClient;
	GBoxEstudo->Visible = true;
	// exibe estudos da base
	FormEstudo->Habilita();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionSelecionaRedeExecute(TObject *Sender)
{
	// habilita os bot�es espec�ficos no ToolBar principal
	butSelecionaRede->Down = true;
	butAbrirRede->Visible = true;
	butRedesSel->Visible = true;
	ActionCancelaSel->Visible = true;
	butAbrirEstudo->Visible = false;
	ActionCriaPlanejamento->Visible = true;
	ActionCriaReclamacaoBT->Visible = true;
	butExcluirEstudo->Visible = false;
	// desabilita FormEstudo
	GBoxEstudo->Visible = false;
	FormEstudo->Desabilita();
	// habilita FormRede
	GBoxRede->Align = alClient;
	GBoxRede->Visible = true;
	// exibe redes da base
	FormRede->Habilita();
//	// habilita/desabilita comandos em fun��o do usu�rio
//	VerificaModulos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ActionSelRedeGraficoExecute(TObject *Sender)
{
	// vari�veis locais
	VTRede *rede;
	TList *lisEQP = (TList*)Sender;

	// prote��o
	if (lisEQP == NULL)
		return;
	// verifica se est� fazendo sele��o de rede
	if (butSelecionaRede->Down)
	{
		rede = (VTRede*)lisEQP->First();
		FormRede->RedeSelecionadaPeloGrafico(rede);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ConfiguraTela(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)abre->apl->GetObject(__classid(VTGeral));
	bool base_acess = (geral->BaseDado == SNPDBTYPE_ACCESS);

	// exibe/esconde componentes relacionados apenas � base Access
	ActionSelecionaBase->Visible = base_acess;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ExibeDadosBaseSelecionada(void)
{
	// vari�veis locais
	VTEsquematico *esquematico = (VTEsquematico*)abre->apl->GetObject(__classid(VTEsquematico));
	VTGeral *geral = (VTGeral*)abre->apl->GetObject(__classid(VTGeral));
	VTGrafico *grafico = (VTGrafico*)abre->apl->GetObject(__classid(VTGrafico));
	VTPath *path = (VTPath*)abre->apl->GetObject(__classid(VTPath));

	// verifica a base atual
	if (geral->BaseDado == SNPDBTYPE_ORACLE)
	{ // atualiza StatusBar c/ path da base de dados
		StatusBar->SimpleText = "Base Oracle";
		// inicia conex�o com a base selecionada
		if (abre->IniciaConexao())
		{ // exibe dados da base selecionada
			ExibeRedesOuEstudos();
			return;
		}
	}
	else if (geral->BaseDado == SNPDBTYPE_ACCESS)
	{ // atualiza StatusBar c/ path da base de dados
		StatusBar->SimpleText = path->ArqRede;
		// inicia conex�o com a base selecionada considerando redes abertas
		if (abre->IniciaConexao(lisREDE_PRE))
		{ // verifica se o arquivo selecionado � de uma base v�lida ou se precisa converter para vers�o atual
			if (abre->ValidaBaseRede())
			{ // atualiza novamente StatusBar c/ path da base de dados (se converteu, mudou)
				StatusBar->SimpleText = path->ArqRede;
				// exibe dados da base selecionada
				ExibeRedesOuEstudos();
				return;
			}
		}
		// apaga esquem�tico e atualiza o grafico
		esquematico->Clear();
		grafico->AreaRedeDefine();
		// desabilita apresenta��o de Redes/Estudos da base selecionada
		FormRede->Desabilita();
		FormEstudo->Desabilita();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ExibeRedesOuEstudos(void)
{
	// simula sele��o de rede/estudos pelo usu�rio
	if (butSelecionaRede->Down)
		ActionSelecionaRede->Execute();
	else
		ActionSelecionaEstudo->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::FormShow(TObject *Sender)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)abre->apl->GetObject(__classid(VTGeral));
	VTPath *path = (VTPath*)abre->apl->GetObject(__classid(VTPath));

	// verifica a base atual
	if (geral->BaseDado == SNPDBTYPE_ORACLE)
	{ // base Oracle: for�a tratamento de sele��o de diret�rio da base de dados
		ExibeDadosBaseSelecionada();
	}
	else
	{ // verifica se h� uma base de rede pr�-definida
		if (!FileExists(path->ArqRede))
		{ // solicita que ousu�rio seleciona a base de uma rede
			if (!SelecionaBaseRede())
				Close();
		}
		ExibeDadosBaseSelecionada();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::ReiniciaRegioes(void)
{ // variaveis locais
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

	regioes->Clear();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAbre::SelecionaBaseRede(void)
{
	// vari�veis locais
	VTPath *path = (VTPath*)abre->apl->GetObject(__classid(VTPath));

	// verifica se o diret�rio inicial est� definido
	if (OpenDialog->InitialDir.IsEmpty())
	{ // define diret�rio das redes
		if (path->ArqRede.IsEmpty())
			OpenDialog->InitialDir = path->DirRedes();
		else
			OpenDialog->InitialDir = ExtractFileDir(path->ArqRede);
	}
	// exibe OpenDialog
	if (OpenDialog->Execute())
	{ // verifica se existe a base selecionada
		if (!FileExists(OpenDialog->FileName))
		{
			Aviso("Selecione uma base existente");
			return (false);
		}
		// salva path da base selecionada
		path->ArqRede = OpenDialog->FileName;
		return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAbre::SelecaoInicial(void)
{
	// vari�veis locais
	VTRedes *redes_owner = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
	TList *lisREDE = redes_owner->LisRede();

	// copia lista de redes j� abertas
	lisREDE_PRE->Clear();
	CopiaTList(lisREDE, lisREDE_PRE);
	return (lisREDE->Count == 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAbre::ValidaBaseRede(AnsiString sdb_name)
{
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAbre::VerificaModulos(void)
{
	// verifica se usu�rio master
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();

	// habilita/desabilita Actions
	ActionCriaReclamacaoBT->Visible = (licenca->Tipo == tipoMASTER) ||
		(licenca->Empresa == ELETROPAULO) || (licenca->Empresa == RGESUL);
}

// ---------------------------------------------------------------------------
// eof
