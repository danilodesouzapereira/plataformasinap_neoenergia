//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <objbase.h>
#include<DateUtils.hpp>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\AlocaCapacitor.h>
#include <DLL_Inc\AlocaRegulador.h>
#include <DLL_Inc\Arranjo.h>
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\Cfg.h>
#include <DLL_Inc\Editor.h>
#include <DLL_Inc\Estudo.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaRedeGIS.h>
#include <DLL_Inc\Reclamacao.h>
#include <DLL_Inc\Redutor.h>
#include <DLL_Inc\SinapApl.h>
#include <Fontes\Alerta\VTAlerta.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Consulta\VTConsulta.h>
#include <Fontes\Constante\Cursor.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Forms\TFormEdtTxt.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Obra\VTLixeira.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Produto\VTProduto.h>
#include <Fontes\Protesw\VTProtesw.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Redutor\VTRedutor.h>
#include <Fontes\Sinap\VTSinapChild.h>
#include <Fontes\Topologia\VTTopologia.h>
#include "TFormMain.h"
#include "TFormLogin.h"
#include "TFormLicenca.h"
#include "TFormSobreSINAPrnt.h"
#include "..\Sinapsis\TFormSobreSinapsis.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
	{
	//vari�vel local
	VTGrafico    *grafico;
	VTSinapChild *sinap_child;

	//initializes the Component Object Model(COM) library
	CoInitialize(NULL);
	//inicia dados
	FormReclamacao = NULL;
	//salva diret�rio de onde o programa foi iniciado
	start_path = GetCurrentDir();
	//cria objetos
	apl = DLL_NewObjAplRnt(this, NULL);
	apl->Add(sinap_child = NewObjSinapChild(apl));
	//inicia SinapChild c/ objetos que ser�o usados
	sinap_child->ActionFlow           = ActionFlow;
	sinap_child->ActionTopo           = ActionTopologia;
	sinap_child->ActionMercadoChanged = ActionMercadoChanged;
	sinap_child->PanelToolBarGrafico  = PanelToolBarGrafico;
	//DVK 2015.04.13 default: 144 patamares (10 minutos)
	sinap_child->DefinePatamares(pat10MIN);
	//valida diret�rios lidos por Dir
	AtualizaMudancaDir();
	//habilita exibi��o de mensagem p/ Licenca demo
	show_msg_licenca = true;
	//seta op��o p/ N�O requisitar login local
	login_local_ok = false;
	//carrega formatos p/ cursor do mouse
	IniciaCursorMouse();
	//define Application::Tag com ActionAlerta
	Application->Tag = int(ActionAlerta);
	ActionExibeAlertaExecute(NULL);
	ExibeNomeRede(); //DVK 2015.03.30
	//exibe o Logo
	LogoShow();
	//configura Grafico
	grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->DefineParent(PanelGrafico);
	grafico->DefineParentToolBar(PanelToolBarGrafico);
	//bloqueia atualiza��o do gr�fico
	grafico->Disable();
	}

//---------------------------------------------------------------------------
__fastcall TFormMain::~TFormMain(void)
	{
	//vari�veis locais
   VTGeral        *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva informa��o da base de dado em uso
   geral->Infoset->Clear();
   geral->Infoset->AddInfo("base_dado", geral->BaseDado);
   geral->Infoset->SaveToFile(ClassName());
	//destr�i objetos
	if (apl != NULL)    {delete apl; apl = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::AbreEstudoRNT(int tipo_estudo)
	{
	//vari�veis locais
	bool           sucesso;
	bool           confirma;
	TForm          *FormAbreEstudo;
	VTEstudo       *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	//reinicia Rede
	ReiniciaRede(confirma=false);
	//esconde o logo
	LogoHide();
	//prote��o
	if ((tipo_estudo != estudoRECLAMACAO_BT)&&(tipo_estudo != estudoRECLAMACAO_MT)) return(false);
	//verifica licen�a
	if (! ValidaLicenca()) return(false);
	//exibe Form para sele��o do estudo
	if (tipo_estudo == estudoRECLAMACAO_BT) FormAbreEstudo = DLL_NewFormAbreEstudoRBT(this, apl);
	else                                    FormAbreEstudo = DLL_NewFormAbreEstudoRMT(this, apl);
	sucesso = (FormAbreEstudo->ShowModal() == mrOk);
	//destr�i FormAbreEstudo
	delete FormAbreEstudo;
	//verifica se o Estudo foi aberto
	if (! sucesso) return(false);
	return(AbreReclamacao(tipo_estudo));
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::AbreReclamacao(int tipo_estudo)
	{//DVK 2015.04.15
	//vari�veis locais
	VTEstudo       *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	//verifica se o Estudo foi aberto
	estudo = planejamento->ExisteEstudo();
	estudo->Novo = false;
	//verifica o tipo do Estudo aberto
	if (estudo->Tipo != tipo_estudo) return(false);
	//reinicia este Form c/ a rede aberta
	TrataRedeCarregada();
	//bloqueia o uso do Cria estudo, Editor e Aj.Demanda
	ActionCriaEstudo->Enabled 	= false;
	ActionEditor->Enabled 		= false;
	ActionDemanda->Enabled 		= false;
	//habilita alocaBC e alocaRT
	ActionAlocaBC->Enabled = true;
	ActionAlocaRT->Enabled = true;
	//cria TFormReclamacao adequado
	if (tipo_estudo == estudoRECLAMACAO_BT) FormReclamacao = DLL_NewFormReclamacaoBT(this, apl, SelecionaParentParaJanela());
	else                                    FormReclamacao = DLL_NewFormReclamacaoMT(this, apl, SelecionaParentParaJanela());
	if (FormReclamacao) FormReclamacao->Show();
	else ActionFechaRedeExecute(NULL);      //DVK 2015.03.31

	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAbreEstudoSDBTExecute(TObject *Sender)
	{
	//vari�veis locais
	bool       confirma;
	VTGrafico *grafico      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//verifica se h� um estudo aberto
	if (SalvaEstudo(confirma=true) == ID_CANCEL) return;
	//fecha estudo aberto (se tiver) //DVK 2015.03.30
	ActionFechaRedeExecute(NULL);
	//abre estudo da base de dados
	if (AbreEstudoRNT(estudoRECLAMACAO_BT))
		{//esconde logo e exibe Toolbar
		LogoShow();
		}
	else
		{//reinicia rede
		ReiniciaRede(confirma=false);
		//exibe logo, esconde ToolBar e esconde o gr�fico
		LogoShow();
		grafico->Hide();
		}
	//reinicia Alerta
	ReiniciaAlerta();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAbreEstudoSDMTExecute(TObject *Sender)
	{
	//vari�veis locais
	bool      confirma;
	VTGrafico *grafico      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//verifica se h� um estudo aberto
	if (SalvaEstudo(confirma=true) == ID_CANCEL) return;
	//fecha estudo aberto (se tiver) //DVK 2015.03.30
	ActionFechaRedeExecute(NULL);
	//abre estudo da base de dados
	if (AbreEstudoRNT(estudoRECLAMACAO_MT))
		{//esconde logo e exibe Toolbar
		LogoShow();
		}
	else
		{//reinicia rede
		ReiniciaRede(confirma=false);
		//exibe logo, esconde ToolBar e esconde o gr�fico
		LogoShow();
		grafico->Hide();
		}
	//reinicia Alerta
	ReiniciaAlerta();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAlertaExecute(TObject *Sender)
	{
	//vari�veis locais
	AnsiString txt = ActionAlerta->Caption;
	VTAlerta  *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	try{//prote��o
		if (alerta == NULL) return;
		//insere mensagem em Alerta
		alerta->Add(txt);
		//atualiza n�mero de mensagens
		ButExibeAlerta->Caption = "Alertas: " + IntToStr(alerta->NumeroDeMensagens());
		//atualiza �ltima mensagem
		LabelAlerta->Caption = txt;
		//define cor do PanelAlerta
		PanelAlerta->Visible = true;
	}catch(Exception &e)
		{
		//nada
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAlocaBCExecute(TObject *Sender)
	{
	try{//verifica licen�a
		if (! ValidaLicenca()) return;
		//verifica se h� m�dulos abertos que podem gerar conflitos
		if (ExisteModuloAtivoConflitante()) return;
		//cria Form do m�dulo de aloca��o de capacitor
		DLL_NewFormAlocaCapacitor(this, apl, SelecionaParentParaJanela());
		}catch(Exception &e)
			{
			//nada
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAlocaRTExecute(TObject *Sender)
	{
	try{//verifica licen�a
		if (! ValidaLicenca()) return;
		//verifica se h� m�dulos abertos que podem gerar conflitos
		if (ExisteModuloAtivoConflitante()) return;
		//cria Form do m�dulo de aloca��o de regulador
		DLL_NewFormAlocaRegulador(this, apl, SelecionaParentParaJanela());
		}catch(Exception &e)
			{
			//nada
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAnimacaoExecute(TObject *Sender)
	{
	//vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//altera habilita��o da anima��o
	ActionAnimacao->Checked = (! ActionAnimacao->Checked);
	//configura gr�fico
	grafico->AnimacaoEnabled = ActionAnimacao->Checked;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionCfgArranjoExecute(TObject *Sender)
	{
	//vari�veis locais
   AnsiString class_name;
	//TFormChild *FormChild;
	VTArranjos *arranjos;

	//verifica se os diret�rios est�o definidos
	if (! ValidaDirBase()) return;
   //verifica licen�a
	if (! ValidaLicenca()) return;
	//verifica se h� redes abertas
	if(! ExisteRedeDoEstudo())
		{
		Aviso("Inicie um estudo para editar cabos e arranjos.");
		return;
		}
	//esconde Logo
	LogoHide();
	//inclui objeto Arranjos em Apl
	apl->Add(arranjos = DLL_NewObjArranjos());
	//edita configura��o de Arranjos
	if (DLL_ConfiguraArranjos(this, apl))
      {//verifica se o usu�rio deseja atualizar redes abertas
      if (Confirma("Configura��o de arranjos atualizada na base de dados.",
                   "Deseja alterar configura��o das redes em estudo ?") == IDYES)
         {//atualiza objeto Geral dos demais FormChild
			/*for (int n = 0; n < MDIChildCount; n++)
				{
				class_name = MDIChildren[n]->ClassName();
				if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
				FormChild = (TFormChild*)MDIChildren[n];
				FormChild->ConfiguraArranjos(apl);
				} */
         }
      }
	//retira objeto Arranjos inserido em Apl
   apl->Remove(arranjos);
   //destr�i objeto
   delete arranjos;
   //exibe logotipo
	LogoShow();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionCfgCurvaTipicaExecute(TObject *Sender)
	{
   //vari�veis locais
   AnsiString class_name;
	//TFormChild *FormChild;

	//verifica se os diret�rios est�o definidos
	if (! ValidaDirBase()) return;
	//verifica licen�a
	if (! ValidaLicenca()) return;
	//verifica se h� redes abertas
	if(! ExisteRedeDoEstudo())
		{
		Aviso("Importe uma rede ou abra um estudo para editar curvas t�picas.");
		return;
		}
	//esconde logotipo
	LogoHide();
   //edita configura��o de Curvas t�picas
	if (DLL_ConfiguraCurvaDemanda(this, apl))
      {//verifica se o usu�rio deseja atualizar as redes abertas
      if (Confirma("Foram feitas altera��es nas curvas t�picas de demanda.",
                   "Deseja alterar configura��o das redes em estudo ?") == IDYES)
         {//atualiza objeto Geral de todos FormChild
			/*for (int n = 0; n < MDIChildCount; n++)
            {
				class_name = MDIChildren[n]->ClassName();
            if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
            FormChild = (TFormChild*)MDIChildren[n];
				FormChild->ConfiguraCurvasTipicas(apl);
            } */
         }
      }
   //exibe logotipo
	LogoShow();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionCriaEstudoExecute(TObject *Sender)
	{ //DVk 2015.04.15
	//vari�veis locais
	bool				aviso;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo       *estudo       = planejamento->ExisteEstudo();

	//prote��o: verifica se h� redes abertas
	if (! ExisteRedeDoEstudo())
		{
		Aviso("N�o h� estudo aberto.");
		return;
		}
	//verifica se os diret�rios est�o definidos
	if (! ValidaDirBase()) return;
	//verifica licen�a
	if (! ValidaLicenca()) return;
	//para o fluxo
	ActionTopologiaExecute(NULL);
	//salva estudo
	if(estudo) SalvaEstudo(estudo, aviso=false);
	//abre form Reclama��o
	if(! AbreReclamacao(estudo->Tipo))
		{
		Aviso("Erro ao abrir estudo");
		ActionFechaRedeExecute(NULL);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionDemandaExecute(TObject *Sender)
	{
	try{//verifica licen�a
		if (! ValidaLicenca()) return;
		//verifica se h� m�dulos abertos que podem gerar conflitos
		if (ExisteModuloAtivoConflitante()) return;
		//cria Form do m�dulo de ajuste de demanda
		DLL_NewFormDemanda(this, apl, SelecionaParentParaJanela());
			}catch(Exception &e)
			{
			//nada
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionEditorExecute(TObject *Sender)
	{
	//vari�veis locais
	bool planejamento = false;

	//exibe tela do editor
	DLL_NewFormEditor(this, apl, SelecionaParentParaJanela(), planejamento);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionExibeAlertaExecute(TObject *Sender)
	{
	//vari�veis locais
	VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	//prote��o
	if (alerta == NULL) return;
	//exibe janela com mensagens de alerta
	alerta->ShowModal();
	//exibe/esconde PanelAlerta
	PanelAlerta->Visible = (alerta->NumeroDeMensagens() > 0);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFechaRedeExecute(TObject *Sender)
	{
	//vari�veis locais
	VTEdita        *edita    	  = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico      *grafico  	  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTRedes        *redes   	  = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//verifica altera��es na rede para salvar
	if(ExisteAlteracaoNaRede()) SalvaEstudo(true);
	//retira Action de VTEdita
	if(edita) edita->ActionOnChangeRedeRetira(ActionRedeChanged);
	//fecha editor
	DLL_CloseFormEditor(this);
	//destroi o FormReclamacao
	if(FormReclamacao)
		{
		FormReclamacao->Close();
		delete FormReclamacao;
		FormReclamacao = NULL;
		}
	//desabilita o gr�fico
	if(grafico) grafico->Disable();
	//remove redes da memoria e estudos
	if(redes) redes->Clear();
	if(edita) edita->Clear();
	if(planejamento) planejamento->Reinicia();
	//fecha o gr�fico
	ActionTopologiaExecute(NULL);  //DVK 2015.04.14
	if(grafico) grafico->Hide();
	//exibe o Logo
	LogoShow();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFlowAlteraOpcaoExecute(TObject *Sender)
	{
	//vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	//marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	//verifica se est� habilitado fluxo por rede
	if (MenuFlowPorRede->Checked)
		{//habilita op��o de threads
		MenuFlowThreads->Enabled = true;
		}
	else
		{//desabilita op��o de threads
		MenuFlowThreads->Enabled = false;
		MenuFlowThreads->Checked = false;
		}
	//verifica se est� exibindo gr�fico de fluxo
	if (ButFlow->Down)
		{//recalcula fluxo e atualiza o gr�fico
		ActionFlow->Execute();
		}
	else
		{//atualiza op��es do fluxo, caso ele seja usado em outro m�dulo
		ConfiguraFlow();
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFlowExecute(TObject *Sender)
	{
   //vari�veis locais
	int            ano = 0;
   int            ind_pat;
   VTPatamar      *patamar;
	VTGrafico      *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //determina o patamar selecionado
   if ((ind_pat = CBoxPatamar->ItemIndex) < 0) ind_pat = 0;
   else
      {
      patamar = (VTPatamar*)CBoxPatamar->Items->Objects[ind_pat];
      ind_pat = patamar->Index;
		}
   //executa c�lculo de fluxo
	if (ExecutaCalculoFluxo())
      {//abaixa Button
		ButFlow->Down = true;
      //inicia e exibe CBoxPatamar
		CBoxPatamarInicia();
		CBoxPatamar->Visible = true;
      //seleciona patamar de maior demanda
		SelecionaPatamarMaiorDemanda();
      //atualiza eventuais janelas abertas
		//AtualizaJanelas();
		//atualiza gr�fico de Flow
		if (ButFlow->Down) grafico->Show(grafFLOW, ano, ind_pat);
		}
   else
      {
		ActionTopologia->Execute();
		}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionImportaSDBTExecute(TObject *Sender)
	{
	//vari�veis locais
	bool       confirma;
	VTGrafico *grafico      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//verifica se h� um estudo aberto
	if (SalvaEstudo(confirma=true) == ID_CANCEL) return;
	//fecha estudo aberto (se tiver) //DVK 2015.03.30
	ActionFechaRedeExecute(NULL);
	//importa rede de arquivo TXT
	if (ImportaRede(estudoRECLAMACAO_BT))
		{//esconde logo e exibe Toolbar
		LogoShow();
		}
	else
		{//reinicia rede
		ReiniciaRede(confirma=false);
		//exibe logo, esconde ToolBar e esconde o gr�fico
		LogoShow();
		grafico->Hide();
		}
	//reinicia Alerta
	ReiniciaAlerta();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionImportaSDMTExecute(TObject *Sender)
	{
	//vari�veis locais
	bool       confirma;
	VTGrafico *grafico      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//verifica se h� um estudo aberto
	if (SalvaEstudo(confirma=true) == ID_CANCEL) return;
	//fecha estudo aberto (se tiver) //DVK 2015.03.30
	ActionFechaRedeExecute(NULL);
	//importa rede de arquivo TXT
	if (ImportaRede(estudoRECLAMACAO_MT))
		{//esconde logo e exibe Toolbar
		LogoShow();
		}
	else
		{//reinicia rede
		ReiniciaRede(confirma=false);
		//exibe logo, esconde ToolBar e esconde o gr�fico
		LogoShow();
		grafico->Hide();
		}
	//reinicia Alerta
	ReiniciaAlerta();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionLicencaAlocaExecute(TObject *Sender)
	{
	//vari�veis locais
	bool       remota = true;
	bool       demo   = false;
	AnsiString diagnostico;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	//verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica prote��o de sotware
   if (! ValidaProtecaoSw()) return;
   //verifica se h� uma Licenca de Uso alocada
   if (licencas->ValidaLicenca(diagnostico))
      {//j� existe Licenca de Uso alocada
      Aviso(diagnostico);
      return;
      }
   AlocaLicenca(remota, demo);
   //reinicia controle de uso
	IniciaControleUso();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionLicencaLiberaExecute(TObject *Sender)
	{
	//vari�veis locais
   AnsiString diagnostico;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTProduto  *produto  = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica prote��o de sotware
   if (! ValidaProtecaoSw()) return;
   //verifica se h� licen�a de uso alocada
   if (licencas->ExisteLicenca() == NULL)
      {//prepara texto de diagn�stico
      Aviso("N�o existe Licen�a de Uso para o programa " + produto->Codigo);
      return;
      }
	//pede confirma��o do usu�rio
   if (Confirma("O programa " + produto->Codigo + " n�o poder� ser executado nesta m�quina ap�s a libera��o da Licen�a de Uso.",
                "Confirma libera��o da Licen�a de Uso ?") != IDYES) return;
   //libera Licen�a de Uso
   licencas->LiberaLicenca(diagnostico);
   if (! diagnostico.IsEmpty()) Aviso(diagnostico);
   //reinicia controle de uso
	IniciaControleUso();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionLicencaExibeExecute(TObject *Sender)
	{
	//vari�veis locais
   TFormLicenca *Form;

   //esconde logotipo
   LogoHide();
   try{//cria TFormLicenca como janela modal
      Form = new TFormLicenca(this, apl);
      Form->ShowModal();
      //destr�i TFormLicenca
      delete Form;
      }catch(Exception &e)
         {//nada a fazer
         }
   //exibe logotipo
	LogoShow();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionLicencaLoginLocalExecute(TObject *Sender)
	{
	//vari�veis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	//habilita/desabilita Login local
	ActionLicencaLoginLocal->Checked = ! ActionLicencaLoginLocal->Checked;
	//atualiza confgura��o da Licenca de Uso
	licencas->LoginLocal = ActionLicencaLoginLocal->Checked;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionMercadoChangedExecute(TObject *Sender)
	{
	//vari�veis locais
   TMenuItem *MenuItem;
   VTMercado *mercado     = (VTMercado*)apl->GetObject(__classid(VTMercado));
   int        ano_inicial = mercado->AnoInicial;
   int        ano_final   = mercado->AnoFinal;
   int        ano_atual   = mercado->AnoAtual;
   int        ano_rede    = mercado->AnoRede;

   //reinicia MenuAnoCarga
   MenuFlowAnoCarga->Clear();
   //verifica se ano atual est� dentro da faixa do mercado
   if ((ano_atual < ano_inicial)||(ano_atual > ano_final))
      {//bloqueia c�lculo de fluxo de pot�ncia
      ActionFlow->Enabled = false;
      }
   else
      {//habilita c�lculo de fluxo de pot�ncia
      ActionFlow->Enabled = true;
      //insere anos entre ano_inicial e ano_final
      for (int ano = ano_inicial; ano <= ano_final; ano++)
         {
         MenuItem             = new TMenuItem(MenuFlowAnoCarga);
         MenuItem->Action     = ActionSelAnoMercado;
         MenuItem->Hint       = IntToStr(ano);
         MenuItem->Caption    = IntToStr(ano);
         MenuItem->GroupIndex = MenuFlowAnoCarga->GroupIndex + 1;
         MenuItem->RadioItem  = true;
         MenuItem->AutoCheck  = true;
         MenuItem->Checked    = (ano == ano_atual);
         MenuItem->Enabled    = (ano >= ano_rede);
         MenuFlowAnoCarga->Add(MenuItem);
         }
      }
   //atualiza Caption de ActionFlow
	ActionSelAnoMercado->Execute();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionReadSenhaExecute(TObject *Sender)
	{
	Aviso("Falta implementar");
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionRedeChangedExecute(TObject *Sender)
	{
	//vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	//marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	//houve altera��o na Rede
	if (ButFlow->Down)
		{//recalcula fluxo de pot�ncia
		ActionFlowExecute(Sender);
		}
	//houve altera��o na Rede
	else
		{//atualiza eventuais janelas abertas
		AtualizaJanelas();
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSairExecute(TObject *Sender)
	{
	//fecha o programa
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSalvaExecute(TObject *Sender)
	{
	//vari�veis locais
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo       *estudo       = planejamento->ExisteEstudo();

	//prote��o: verifica se h� redes abertas
	if (! ExisteRedeDoEstudo())
		{
		Aviso("N�o h� estudo aberto.");
		return;
		}
	//verifica se os diret�rios est�o definidos
	if (! ValidaDirBase()) return;
	//verifica licen�a
	if (! ValidaLicenca()) return;
	//para o fluxo
	ActionTopologiaExecute(NULL);
	//salva estudo
	if(estudo) SalvaEstudo(estudo);
	//fecha a rede
	ActionFechaRedeExecute(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSalvaComoExecute(TObject *Sender)
	{      /*
   //vari�veis locais
	bool            edita_nome_estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo       *estudo       = planejamento->ExisteEstudo();

	//verifica se a Licenca permite gravar base de dados
	if (! ActionSalvaComo->Enabled) return;
	//verifica se � a Rede original ou um Estudo de planejamento
	if (estudo->Tipo == estudoREDE_ORIGINAL)
		{//salva rede original
		SalvaRedeComo();
		}
	//verifica se o usu�rio quer duplicar a base com o estudo ou apenas o estudo dentro da mesma base
	else
		{
		switch(Confirma("Para criar uma nova base com o estudo, selecione 'Sim/Yes'",
							 "Para criar um novo estudo na mesma base, selecione 'N�o/No'", MB_YESNOCANCEL))
			{
			case IDYES: //salva a rede em uma nova base e salva o estudo
				//atualiza FormEstudo: inicio de grava��o do estudo na base
				DLL_AtualizaFormEstudo(this, true);
				aviso_base_salva = false;
				if (SalvaRedeComo())
					{//habilita aviso de base salva
					aviso_base_salva = true;
					SalvaEstudoComo(estudo, edita_nome_estudo=false);
					}
				//atualiza FormEstudo: fim de grava��o do estudo na base
				DLL_AtualizaFormEstudo(this, false);
				//habilita aviso de base salva
				aviso_base_salva = true;
				break;
			case IDNO:  //salva um novo estudo na mesma base
				SalvaEstudoComo(estudo, edita_nome_estudo=true);
				break;
         case IDCANCEL:
				break;
         }
		} */
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSelAnoMercadoExecute(TObject *Sender)
	{
	//vari�veis locais
	int       ano_atual = 0;
	VTFlow    *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTMercado *mercado  = (VTMercado*)apl->GetObject(__classid(VTMercado));

	//determina ano selecionado em MenuAnoCarga
	if (! AnoMercado(ano_atual)) return;
	//atualiza Caption de ActionFlow
	ActionFlow->Caption = "Fluxo Pot�ncia " + IntToStr(ano_atual);
	//verifica se houve altera��o no ano de mercado selecionado
	if (mercado->AnoAtual == ano_atual) return;
	//atualiza ano atual selecionado
	mercado->AnoAtual = ano_atual;
	//marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	//se necess�rio, executa fluxo
	if (ButFlow->Down)
		{//recalcula fluxo e atualiza o gr�fico
		ActionFlow->OnExecute(NULL);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSobreSINAPrntExecute(TObject *Sender)
   {
   //vari�veis locais
	TFormSobreSINAPrnt *Form;

   //esconde Logo
   LogoHide();
   try{//exibe TAboutBox como janela modal
		Form = new TFormSobreSINAPrnt(this, apl);
		Form->ShowModal();
		//destr�i Form
      delete Form;
      }catch(Exception &e)
         {
			}
	//exibe Logo
   LogoShow();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSobreSinapsisExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormSobreSinapsis *Form;

   //esconde Logo
   LogoHide();
   try{//exibe TAboutSinap como janela modal
		Form = new TFormSobreSinapsis(this, apl);
      Form->ShowModal();
      //destr�i Form
      delete Form;
      }catch(Exception &e)
         {
         }
   //exibe Logo
	LogoShow();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionTopologiaExecute(TObject *Sender)
   {
	//vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//esconde CBoxPatamar
	CBoxPatamar->Visible = false;
	//abaixa Button e acerta seu Caption
	ButTopologia->Down = true;
   //exibe gr�fico de Topologia
   grafico->Show(grafTOPO);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::AlocaLicenca(bool remota, bool demo)
   {
   //vari�veis locais
   bool       sucesso = false;
   AnsiString diagnostico;
   TFormLogin *FormLogin;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   try{//cria TFormLogin e exibe como janela modal
      FormLogin = new TFormLogin(this);
      if (FormLogin->ShowModal() == mrOk)
         {//tenta alocar Licenca
         if (remota)
            {//aloca Licen�a via Web
            sucesso = licencas->AlocaLicencaRemota(FormLogin->GetLogin(), FormLogin->GetPassword(),
                                                   demo, diagnostico);
            }
			if (! diagnostico.IsEmpty()) Aviso(diagnostico);
         }
      //destr�i TFormLogin
      delete FormLogin;
      }catch(Exception &e)
         {
         return(false);
         }
   return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::AnoMercado(int &ano)
	{
   //determina ano selecionado em MenuAnoCarga
   for (int n = 0; n < MenuFlowAnoCarga->Count; n++)
      {
      if (MenuFlowAnoCarga->Items[n]->Checked)
         {
         ano = StrToInt(MenuFlowAnoCarga->Items[n]->Hint);
         return(true);
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::AtualizaJanelas(void)
   {
	//vari�veis locais
	int            ano = 0;
	int            ind_pat;
   VTPatamar      *patamar;
   VTGrafico      *grafico      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //determina o patamar selecionado
   if ((ind_pat = CBoxPatamar->ItemIndex) < 0) ind_pat = 0;
   else
      {
      patamar = (VTPatamar*)CBoxPatamar->Items->Objects[ind_pat];
      ind_pat = patamar->Index;
      }
   //atualiza gr�fico de Flow
	if (ButFlow->Down) grafico->Show(grafFLOW, ano, ind_pat);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::AtualizaMudancaDir(void)
   {
   //vari�veis locais
   VTDir  *dir       = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path      = (VTPath*)apl->GetObject(__classid(VTPath));

   //atualiza TPath
   path->DefineDirBase(dir->DirBase);
	path->DefineDirRede(dir->DirRede);
	}

//---------------------------------------------------------------------
void __fastcall TFormMain::CBoxPatamarChange(TObject *Sender)
	{
	//atualiza janelas de resultados de flow
	AtualizaJanelas();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::CBoxPatamarInicia(void)
	{
   //vari�veis locais
	VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT    = patamares->LisPatamar();

   //reinicia CBoxPatamar c/ Patamares
   CBoxPatamar->Clear();
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (patamar->Enabled)
         {
         CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
         }
      }
   //pr�-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   }

//---------------------------------------------------------------------
bool __fastcall TFormMain::ConfiguraFlow(void)
	{
	//vari�veis locais
	int    ano;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	//habilita/desabilita uso de threads
	flow->MultiThreadEnabled = MenuFlowThreads->Checked;
	//seleciona algoritmo de c�lculo de fluxo
	if      (MenuFlowAlgoritmoAutomatico->Checked)  flow->TipoFluxo = fluxoAUTO;
	else if (MenuFlowAlgoritmoEquilibrado->Checked) flow->TipoFluxo = fluxoEQUILIBRADO;
	else if (MenuFlowAlgoritmoG1->Checked)          flow->TipoFluxo = fluxoGAUSS1;
	else if (MenuFlowAlgoritmoG3->Checked)          flow->TipoFluxo = fluxoGAUSS3;
	else if (MenuFlowAlgoritmoNR1->Checked)         flow->TipoFluxo = fluxoNR1;
	else if (MenuFlowAlgoritmoNR3->Checked)         flow->TipoFluxo = fluxoNR3;
	else if (MenuFlowAlgoritmoRadial1->Checked)     flow->TipoFluxo = fluxoRADIAL1;
	else if (MenuFlowAlgoritmoRadial3->Checked)     flow->TipoFluxo = fluxoRADIAL3;
	else if (MenuFlowAlgoritmoCC1->Checked)         flow->TipoFluxo = fluxoCC1;
	else
		{
		Aviso("Selecione modelo de c�lculo de fluxo a ser utilizado");
		return(false);
		}
	//seleciona modelo de carga
	if      (MenuFlowCargaAuto->Checked)  flow->ModeloCarga = cargaAUTO;
	else if (MenuFlowCargaIcte->Checked)  flow->ModeloCarga = cargaICTE;
	else if (MenuFlowCargaScte->Checked)  flow->ModeloCarga = cargaSCTE;
	else if (MenuFlowCargaZcte->Checked)  flow->ModeloCarga = cargaZCTE;
	else if (MenuFlowCargaAneel->Checked) flow->ModeloCarga = cargaANEEL;
	else
		{
		Aviso("Selecione modelo de carga a ser utilizado no c�lculo de fluxo");
		return(false);
		}
	//verifica se o ano de mercado est� selecionado
	if (! AnoMercado(ano))
		{
		Aviso("Selecione ano de refer�ncia do mercado");
		return(false);
		}
	//habilita mensagem de erro do flow
	flow->HabilitaAviso(true);
	//habilita/desabilita relatorio do flow
	if      (MenuFlowRelatorioResumido->Checked) flow->TipoRelatorio = relRESUMIDO;
	else if (MenuFlowRelatorioCompleto->Checked) flow->TipoRelatorio = relCOMPLETO;
	else                                         flow->TipoRelatorio = relNENHUM;
	//habilita/desabilita medida de tempo do flow
	flow->HabilitaCronometro(MenuFlowCronometro->Checked);
	//habilita/desabilita otimiza��es (uso de Tramos)
	flow->HabilitaOtimizacao(MenuFlowOtimizacao->Checked);
	//seleciona op��o de c�lculo de perda no ferro
	flow->PerdaFerroEnabled = MenuFlowPerdaFe->Checked;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::CriaNovaBaseDado(AnsiString subdir)
	{
   //vari�veis locais
   VTDir  *dir    = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath *path   = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return(false);
   //cria uma nova base de dados vazia
	if (! dir->CriaRedeEmpresa(subdir)) return(false);
   //atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
   path->DefineDirRede(subdir);
   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::DefineNomeEstudo(VTEstudo *estudo)
   {
   //vari�veis locais
   bool        sucesso = false;
   TFormEdtTxt *FormEdtTxt;

   try{//cria TFormEdtTxt p/ definir nome do Estudo
      FormEdtTxt = new TFormEdtTxt(this, "Estudo: salvar como...");
      FormEdtTxt->Label->Caption = "Novo Estudo:";
      FormEdtTxt->Texto = estudo->Codigo + "_1";
      if (FormEdtTxt->ShowModal() == mrOk)
         {
         estudo->Codigo = FormEdtTxt->Texto ;
         sucesso = true;
         }
      //detr�i Form
      delete FormEdtTxt;
      }catch(Exception &e)
         {
         }
   return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ExecutaCalculoFluxo(void)
	{
	//vari�veis locais
	bool    sucesso;
	VTFlow  *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//configura op��es de c�lculo do fluxo
		if (! ConfiguraFlow()) return(false);
		//verifica se deve executar fluxo por rede
		if (MenuFlowPorRede->Checked)
			{//executa c�lculo de fluxo individual das redes
			sucesso = flow->CalculaPorRede(redes);
			}
		else
			{//executa c�lculo de fluxo �nico das redes
			sucesso = flow->CalculaRedeUnica(redes);
			}
		}catch(Exception &e)
			{
			sucesso = false;
         }
	return(sucesso);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ExibeNomeRede(void)
	{
   //vari�veis locais
	VTPath    *path  = (VTPath*)apl->GetObject(__classid(VTPath));

	//exibe nome da rede carregada
	StatusBar->SimpleText = path->DirRedeBase();
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ExisteAlteracaoNaRede(void)
	{
	//vari�veis locais
	VTEqpto    *eqpto;
	VTArranjos *arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTCurvas   *curvas    = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	VTRedes    *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEdita    *edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
   TList      *lisEQP    = edita->Obra->Lixeira->LisEqpto();

   //verifica se existe algum Eqpto novo ou alterado na rede
   if (redes->ExisteAlteracao()) return(true);
   //verifica se h� algum Eqpto pr�-existente na base que foi retirado
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Id > 0) return(true);
      }
   //verifica se h� suportes, cabos ou arranjos novos ou alterados
   if (arranjos->ExisteAlteracao()) return(true);
   //verifica se h� curvas t�picas novas ou alterdas
   if (curvas->ExisteAlteracao()) return(true);
   return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ExisteModuloAtivoConflitante(void)
   {
   // vari�veis locais
   TForm *form = NULL;

   //procura Form de m�dulos ativos que podem ter conflitos
   form                   = DLL_ExisteFormAlocaCapacitor(this);
   if (form == NULL) form = DLL_ExisteFormAlocaRegulador(this);
   if (form == NULL) form = DLL_ExisteFormDemanda(this);
   //verifica se foi encontrado um form
   if (form != NULL)
      {
      Aviso("Feche o m�dulo " + AnsiQuotedStr(form->Caption, '\''));
      return(true);
      }
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ExisteRedeDoEstudo(void)
	{
	//vari�veis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	return(redes->LisRede()->Count > 0);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
	{
	//fecha redes abertas
	ActionFechaRedeExecute(NULL); //DVK 2015.03.30
	//destr�i o Form
	Action = caFree;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormResize(TObject *Sender)
	{
	///vari�veis locais
	int top, left;

	//reposiciona logo
	top  = ToolBar->Height + (ClientHeight - ToolBar->Height - PanelLogo->Height)/2;
	left = (ClientWidth  - PanelLogo->Width)/2;
	PanelLogo->Top  = (top  > ToolBar->Height) ? top : ToolBar->Height;
	PanelLogo->Left = (left > 0) ? left : 0;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject *Sender)
	{
	//vari�veis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	//executa FormResize p/ ajustar logo
	FormResize(NULL);
	//inicia controle de uso de acordo c/ tipo da Licenca
	IniciaControleUso();
	//habilita/desabilita m�dulos de acordo c/ a Licenca de uso
	VerificaModulos();
	//gera aviso sobre per�odo de validade da licen�a de uso
	if (licencas->ValidadeLimitada) {ValidaLicenca(true);}
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ImportaRede(int tipo_estudo)
	{
	//vari�veis locais
	bool             confirma;
	bool             sucesso;
	VTImportaRedeGIS *ImportaRedeGIS;
	VTPlanejamento   *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo         *estudo       = planejamento->ExisteEstudo();

	//reinicia Rede
	ReiniciaRede(confirma=false);
	//esconde o logo
	LogoHide();
	//prote��o
	if ((tipo_estudo != estudoRECLAMACAO_BT)&&(tipo_estudo != estudoRECLAMACAO_MT)) return(false);
	//verifica licen�a
	if (! ValidaLicenca()) return(false);
	//cria nova base de dados
	if (! CriaNovaBaseDado("")) return(false);
	try{//cria objeto VTImportaRedeGIS
		if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(this, apl)) == NULL) return(false);
		//importa arquivo de acordo com o tipo de estudo
		if (tipo_estudo == estudoRECLAMACAO_BT) sucesso = ImportaRedeGIS->ShowModalFormImportaRedeGIS_SDBT();
		else                                    sucesso = ImportaRedeGIS->ShowModalFormImportaRedeGIS_SDMT();
		//destr�i objeto VTImportaRedeGIS
		delete ImportaRedeGIS;
		//verifica se a rede foi importada
		if (! sucesso) return(false);
		}catch(Exception &e)
			{
			if (ImportaRedeGIS) delete ImportaRedeGIS;
			return(false);
			}
	//prepara a rede o estudo
	TrataRedeCarregada();
	//cria novo estudo  //DVK 2015.04.16
	planejamento->InsereEstudo("Estudo_RNT", tipo_estudo, -1);
	//habilita o uso do Editor e Cria estudo
	ActionEditor->Enabled     = true;
	ActionCriaEstudo->Enabled = true;
	//desabilita alocaBC e alocaRT
	ActionAlocaBC->Enabled = false;
	ActionAlocaRT->Enabled = false;
	//habilita Aj.Demanda somente para redes MT  //DVK 2015.03.30
	ActionDemanda->Enabled = (tipo_estudo != estudoRECLAMACAO_BT);
	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::IniciaControleUso(void)
	{
	//vari�veis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	//verifica se deve solicitar Login de controle local
	if (licencas->LoginLocal)
      {//solicita identifica��o do usu�rio
      if (! VerificaLoginLocal())
         {//finaliza o programa
         Close();
         return;
         }
      }
   //verifica se deve solicitar Login de controle local
   if (licencas->LoginLocal)
      {//Login de controle local obrigat�rio
      ActionLicencaLoginLocal->Checked = true;
      //esconde comando que permite cancelar Login de controle obrigat�rio
      ActionLicencaLoginLocal->Visible = login_local_ok;
      }
   else
		{//Login de controle local n�o � necess�rio
      ActionLicencaLoginLocal->Checked = false;
      //exibe comando que permite cancelar Login de controle obrigat�rio
      ActionLicencaLoginLocal->Visible = true;
		}
	//verifica se � uma Licenca demo
   if (licencas->LicencaTipoDemo())
      {//habilita TimerDemo usado p/ controle de uso de Licenca demo por 2 horas e 45 minutos
      TimerDemo->Tag      = 4;
      TimerDemo->Interval = (((2 * 60) + 45) * 60) * 1000;
		TimerDemo->Enabled = true;
      }
   //verifica se � uma Licenca did�tica
   else if (licencas->LicencaTipoDidatica())
      {//habilita TimerDemo usado p/ controle de uso de Licenca demo por 7 horas e 45 minutos
      TimerDemo->Tag      = 4;
		TimerDemo->Interval = (((7 * 60) + 45) * 60) * 1000;
		TimerDemo->Enabled = true;
      }
   else
      {//n�o se trata de Licenca tipoDEMO: desabilita TimerDemo
      TimerDemo->Enabled = false;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::IniciaCursorMouse(void)
   {
   //vari�veis locais
   int        index;
   WideString arquivo;
   VTPath     *path      = (VTPath*)apl->GetObject(__classid(VTPath));
   WideString dir_cursor = path->DirDat() + ("\\CursorMouse");
   struct strVET_CURSOR{
                        int  index;
                        char *arquivo;
                        }vet_cursor[] = { {curBarra     , "Barra.cur"},
                                          {curCapacitor , "Capacitor.cur"},
                                          {curCarga     , "Carga.cur"},
                                          {curChave     , "Chave.cur"},
                                          {curGerador   , "Gerador.cur"},
                                          {curSuprimento, "Suprimento.cur"},
                                          {curTrafo     , "Trafo.cur"},
                                          {curTrafo3E   , "Trafo3E.cur"},
                                          {curTrecho    , "Trecho.cur"},
                                          {curZoom      , "Zoom.cur"},
                                          {curGrupo     , "Grupo.cur"},
                                          {curManobra   , "Manobra.cur"},
                                          {curAbreChv   , "ChaveAbre.cur"},
                                          {curFechaChv  , "ChaveFecha.cur"},
                                          {curRede      , "Rede.cur"},
                                          {curReducao   , "Reducao.cur"},
                                          {curMedidor   , "Medidor.cur"},
//                                        {curBarSwing  , "BAR_SWING.cur"},
//                                        {curSwing     , "SWING.cur"},
                                          {curSpy       , "Spy.cur"},
                                          {curYref      , "Zref.cur"},
                                          {curCapserie  , "Capserie.cur"},
                                          {curFiltro    , "Filtro.cur"},
                                          {curMutua     , "Mutua.cur"},
                                          {curRegulador , "Regulador.cur"},
                                          {curReator    , "Reator.cur"},
                                          {curTrafoZZ   , "TrafoZZ.cur"} };

   //verifica exist�ncia do diret�rio
   if (! DirectoryExists(dir_cursor))
      {
      Aviso("A instala��o n�o inclui os arquivos de cursor do mouse");
      return;
      }
   //carrega cursor do mouse a partir dos arquivos *.cur
   for (int n = 0; n < (sizeof(vet_cursor) / sizeof(strVET_CURSOR)); n++)
		{
      index   = vet_cursor[n].index;
      arquivo = dir_cursor + "\\" + vet_cursor[n].arquivo;
		Screen->Cursors[index] = LoadCursorFromFile(arquivo.c_bstr());
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::LogoHide(void)
	{
	//esconde panel com logo
	PanelLogo->Visible = false;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::LogoShow(void)
	{
	//mostra logo e esconde toolbar se houver estudo aberto
	ToolBar->Visible   = ExisteRedeDoEstudo();
	PanelLogo->Visible = ! ToolBar->Visible;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::PopupMenuFlowIniciaCurvasTipicas(void)
	{
	//vari�veis locais
	TList    *lisTMP;
	VTCurvas *curvas  = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   struct strVETOR   {
                     int        dia;
                     TMenuItem *MenuItem;
							} vetor[] = {{DOMINGO,        MenuFlowCurvaDomingo},
                                 {SABADO,          MenuFlowCurvaSabado },
                                 {DIA_UTIL,        MenuFlowCurvaDiaUtil},
                                 {SEMANA_COMPLETA, MenuFlowCurvaSemana }
											};

   //cria lista tempor�ria
   lisTMP = new TList();
   //habilita MenuItem somente se houver curvas do dia
   for (int n = 0; n < (sizeof(vetor)/sizeof(strVETOR)); n++)
      {//determina n�mero de curvas de demanda do dia
      lisTMP->Clear();
      vetor[n].MenuItem->Enabled = (curvas->LisCurvaTipicaCarga(NULL, vetor[n].dia, lisTMP) > 0);
      }
   //destr�i lista tempor�ria
   delete lisTMP;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::PopupMenuFlowIniciaMercado(void)
	{
   //vari�veis locais
   TMenuItem *MenuItem;
   VTMercado *mercado     = (VTMercado*)apl->GetObject(__classid(VTMercado));
   int        ano_inicial = mercado->AnoInicial;
   int        ano_final   = mercado->AnoFinal;
   int        ano_atual   = mercado->AnoAtual;
   int        ano_rede    = mercado->AnoRede;

   //reinicia MenuAnoCarga
   MenuFlowAnoCarga->Clear();
   //verifica se ano atual est� dentro da faixa do mercado
   if ((ano_atual < ano_inicial)||(ano_atual > ano_final))
      {//boqueia c�lculo de fluxo de pot�ncia
      ActionFlow->Enabled = false;
      }
   else
      {//habilita c�lculo de fluxo de pot�ncia
      ActionFlow->Enabled = true;
      //insere anos entre ano_inicial e ano_final
      for (int ano = ano_inicial; ano <= ano_final; ano++)
         {
         MenuItem             = new TMenuItem(MenuFlowAnoCarga);
         MenuItem->Action     = ActionSelAnoMercado;
         MenuItem->Hint       = IntToStr(ano);
			MenuItem->Caption    = IntToStr(ano);
         MenuItem->GroupIndex = MenuFlowAnoCarga->GroupIndex + 1;
         MenuItem->RadioItem  = true;
			MenuItem->AutoCheck  = true;
         MenuItem->Checked    = (ano == ano_atual);
         MenuItem->Enabled    = (ano >= ano_rede);
         MenuFlowAnoCarga->Add(MenuItem);
         }
		}
   //atualiza Caption de ActionFlow
	ActionSelAnoMercado->Execute();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ReiniciaAlerta(void)
	{
	//vari�veis locais
	VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	//elimina todas as mensagesn de alerta
	alerta->Clear();
	//exibe/esconde PanelAlerta
	PanelAlerta->Visible = (alerta->NumeroDeMensagens() > 0);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ReiniciaRede(bool confirma)
	{
	//vari�veis locais
	VTEdita   *edita    = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList     *lisRedes = redes->LisRede();

	//prote��o
	if(! redes) return false;
	//verifica se deve pedir confirma��o do usu�rio
	if ((confirma) && (lisRedes->Count > 0))
		{
		if(Confirma("As redes ser�o apagadas da mem�ria.\n"
						"Deseja continuar ?","") != ID_YES)
			{
			return false;
			}
		}
	//remove redes da memoria
	redes->Clear();
	edita->Clear();
	grafico->Show(grafTOPO);
	return true;
	}

//---------------------------------------------------------------------------
int __fastcall TFormMain::Salva(bool confirma)
   {
   //vari�veis locais
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo       *estudo       = planejamento->ExisteEstudo();

   //verifica se a Licenca permite gravar base de dados
   if (! ActionSalva->Enabled) return(IDNO);
   //verifica se � a Rede original ou um Estudo de planejamento
   if (estudo->Tipo == estudoREDE_ORIGINAL)
      {//salva rede original
      return(SalvaRede(confirma));
      }
	else
      {//salva Estudo
		return(SalvaEstudo(confirma));
		}
	}

//---------------------------------------------------------------------------
int __fastcall TFormMain::SalvaEstudo(bool confirma)
	{
	//vari�veis locais
	int             resp         = IDNO;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo       *estudo       = planejamento->ExisteEstudo();

	//verifica se existe Rede do estudo
	if (! ExisteRedeDoEstudo()) return(IDNO);
	//verifica se h� necessidade de gravar o Estudo na base de dados
	if (estudo->Novo || estudo->Alterado)
		{//verifica se deve pedir confirma��o do usu�rio
		if (confirma) resp = Confirma("Deseja salvar o estudo na base de dados ?", "", MB_YESNOCANCEL);
		else          resp = IDYES;
		}
	switch(resp)
		{
		case IDYES: //muda apresenta��o p/ topologia da Rede
			//atualiza base de dados
			if (SalvaEstudo(estudo))
				{//atualiza FormEstudo
				return(IDYES);
				}
			else
				{//atualiza FormEstudo
				return(IDCANCEL);
				}
		case IDNO: //n�o salva a rede
			return(IDNO);
		case IDCANCEL: //n�o salva a rede
		default:
			return(IDCANCEL);
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::SalvaEstudo(VTEstudo *estudo, bool aviso)
	{
	//vari�veis locais
	bool    sucesso;
	VTSalva *salva;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	//salva autor do estudo conforme licen�a
	estudo->Autor = licencas->ExisteLicenca()->Login;
	try{//cria objeto VTSalva
		if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
		//atualiza FormEstudo: inicio de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, true);
		//salva Estudo
		if ((sucesso = salva->SalvaEstudo(estudo)) == true)
			{
			if(aviso) Aviso("A base de dados foi atualizada corretamente");
			}
		else
			{
			Erro("A base de dados n�o foi atualizada.");
			}
		//atualiza FormEstudo: fim de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, false);
		//destr�i objeto
		delete salva;
		}catch(Exception &e)
			{//atualiza FormEstudo: fim de grava��o do estudo na base
			DLL_AtualizaFormEstudo(this, false);
			sucesso = false;
			}
	 return(sucesso);
	 }

//---------------------------------------------------------------------------
bool __fastcall TFormMain::SalvaEstudoComo(VTEstudo *estudo, bool edita_nome)
   {
   //vari�veis locais
   bool    sucesso;
   VTSalva *salva;

   try{//verifica se deve editar o nome do estudo
      if (edita_nome)
         {//solicita novo nome para o Estudo
         if (! DefineNomeEstudo(estudo)) return(false);
         }
      //cria objeto VTSalva
      if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
      //atualiza FormEstudo: inicio de grava��o do estudo na base
      DLL_AtualizaFormEstudo(this, true);
      //salva Estudo
      if ((sucesso = salva->SalvaEstudoComo(estudo)) == true)
         {
         Aviso("A base de dados foi salva corretamente");
         }
      else
         {
         Erro("A base de dados n�o foi salva.");
         }
      //atualiza FormEstudo: fim de grava��o do estudo na base
      DLL_AtualizaFormEstudo(this, false);
      //destr�i objeto
      delete salva;
      }catch(Exception &e)
         {//atualiza FormEstudo: fim de grava��o do estudo na base
         DLL_AtualizaFormEstudo(this, false);
         sucesso = false;
         }
    return(sucesso);
    }

//---------------------------------------------------------------------------
int __fastcall TFormMain::SalvaRede(bool confirma)
	{
   //vari�veis locais
   bool base_nova = false;
   int  resp      = IDYES;

   //verifica se h� necessidade de gravar a Rede
	if (! ExisteAlteracaoNaRede()) return(IDNO);
   //verifica se deve pedir confirma��o do usu�rio
   if (confirma) resp = Confirma("Deseja atualizar a rede na base de dados ?", "", MB_YESNOCANCEL);
   //verifica se o usu�rio deseja atualizar a base de dados
   switch(resp)
      {
      case IDYES: //atualiza base de dados
         resp = (SalvaRedeBase(base_nova)) ?IDYES : IDCANCEL;
         //return(resp);
         break;
      case IDNO: //n�o salva a rede
         //return(IDNO);
         break;
      case IDCANCEL: //n�o fecha o form
      default:
         //return(IDCANCEL);
         break;
      }
   return(resp);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::SalvaRedeBase(bool base_nova)
   {
   //vari�veis locais
	bool        sucesso;
   VTRedutor   *redutor;
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //verifica se h� Eqptos isolados
   if (! ValidaEqptoIsolado()) return(false);
   //muda apresenta��o p/ topologia da Rede
   ActionTopologia->Execute();
   //cria objeto VTRedutor
	if ((redutor =  DLL_NewObjRedutor(apl)) == NULL) return(false);
   //desabilita mensagens do redutor
   redutor->AvisoEnabled = false;
   //monta redes reduzidas
   if (redutor->ExecutaReducaoMNet())
      {//atualiza base de dados
      if (base_nova) sucesso = SalvaRedeBaseNova();
      else           sucesso = SalvaRedeBaseExistente();
      }
   else
      {//avisa usu�rio e pede confirma��o p/ atualizar base de dados
      if (Confirma("N�o foi poss�vel criar as redes equivalentes",
                   "A base de dados deve ser atualizada ?") == IDYES)
         {//atualiza base de dados
         if (base_nova) sucesso = SalvaRedeBaseNova();
         else           sucesso = SalvaRedeBaseExistente();
         }
      }
   //destr�i objeto VTRedutor
   delete redutor;
	//reconfigura Redes
   topologia->ClassificaRedes();
   //trata altera��o de rede
   ActionRedeChangedExecute(NULL);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMain::SalvaRedeBaseExistente(void)
   {
   //vari�veis locais
   bool     sucesso;
   VTSalva *salva;
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   try{//cria objeto VTSalva
      if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
      //salva Rede existente na base de dados
      sucesso = salva->SalvaRede();
      if (sucesso)
         {//cancela Undo/Redo
         edita->Clear();
         if (aviso_base_salva) Aviso("A base de dados foi atualizada corretamente");
         }
      else
         {
			Erro("A base de dados n�o foi atualizada.");
         }
      //destr�i objeto
      delete salva;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMain::SalvaRedeBaseNova(void)
   {
   //vari�veis locais
	bool     sucesso;
   VTSalva *salva;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   try{//cria objeto VTSalva
      if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
      //salva Rede existente na base de dados
      sucesso = salva->SalvaRedeComo();
      if (sucesso)
         {//cancela Undo/Redo
         edita->Clear();
         if (aviso_base_salva) Aviso("A base de dados foi atualizada corretamente");
         }
      else
         {
         Erro("A base de dados n�o foi atualizada.");
         }
      //destr�i objeto
      delete salva;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMain::SalvaRedeComo(void)
   {
   //vari�veis locais
   bool           sucesso;
   bool           base_nova = true;
   AnsiString     subdir;
   TList          *lisREDE;
   VTEqpto        *eqpto;
   VTDir          *dir          = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath         *path         = (VTPath*)apl->GetObject(__classid(VTPath));
   VTRedes        *redes        = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //cria uma nova base de dados vazia
   subdir = ExtractFileName(path->DirRedeBase());
   if (! dir->CriaRedeSinap(subdir)) return(false);
   //atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
   path->DefineDirRede(subdir);
   //marca que todas as Redes s�o novas (informa��o importante p/ o Redutor)
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      eqpto = (VTEqpto*)lisREDE->Items[n];
      eqpto->Status[sttNOVO] = true;
      }
   //salva rede na base de dados
	sucesso = SalvaRedeBase(base_nova);
   return(sucesso);
	}

//---------------------------------------------------------------------------
TWinControl* __fastcall TFormMain::SelecionaParentParaJanela(void)
	{
	//micro c/ apenas 1 monitor
   return(PanelGrafico);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::SelecionaPatamarMaiorDemanda(void)
	{
   //vari�veis locais
   VTPatamar  *patamar;
	VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
	VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

	if ((patamar = consulta->PatamarFluxoMaximo(redes)) != NULL)
		{
		CBoxPatamar->ItemIndex = CBoxPatamar->Items->IndexOfObject(patamar);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TrataRedeCarregada(void)
	{
	//vari�veis locais
	AnsiString   txt;
	VTDir        *dir       = (VTDir*)apl->GetObject(__classid(VTDir));
	VTEdita      *edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTFlow       *flow      = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTPath       *path      = (VTPath*)apl->GetObject(__classid(VTPath));
	VTGrafico    *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTMercado    *mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes      *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//limpa edita e fecha editor (se estiver aberto) //DVk 2015.04.15
	edita->Clear();
	DLL_CloseFormEditor(NULL);
	//exibe nome da rede carregada
	ExibeNomeRede();
	//atualiza diret�rio da rede no objeto Dir
	dir->DirRede = path->DirRedeBase();
	//associa Redes e Cargas c/ Mercado
	mercado->IniciaRedes(redes);
	if (mercado->AnoInicial < YearOf(Now()))
      {
      Aviso(txt.sprintf("O Mercado est� configurado para o per�odo de %d a %d", mercado->AnoInicial, mercado->AnoFinal));
		}
   //marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	//inicia CBoxPatamar
	CBoxPatamarInicia();
	//inicia PopupMenuFlow c/ anos de mercado e curvas t�picas dispon�veis
   PopupMenuFlowIniciaMercado();
   PopupMenuFlowIniciaCurvasTipicas();
	//configura Grafico
	//bloqueia atualiza��o do gr�fico
	grafico->Disable();
	//habilita o gr�fico, ainda sem definir o tipo de gr�fico
	grafico->AreaRedeDefine();
	//exibe gr�fico de topologia
	ActionTopologiaExecute(NULL);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ValidaDirBase(void)
   {
   //vari�veis locais
   VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));

	//valida diret�rios
   return(dir->ValidaDirBase());
	}

//---------------------------------------------------------------------------
bool  __fastcall TFormMain::ValidaEqptoIsolado(void)
   {
   //vari�veis locais
   int     resp = IDYES;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede  *rede  = redes->RedeIsolada();

   //verifica se h� Liga��es isoladas
   if ((rede->LisLigacao())->Count > 0)
      {//pede confirma��o do usu�rio
      resp = Confirma("Existem blocos isolados que n�o ser�o salvos na base de dados",
                      "Confirma a grava��o ?");
      }
   else if ((rede->LisBarra())->Count > 0)
      {//pede confirma��o do usu�rio
      resp = Confirma("Existem barras isoladas que n�o ser�o salvas na base de dados",
                      "Confirma a grava��o ?");

      }
   return(resp == IDYES);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ValidaLicenca(bool show_diag)
	{
   //vari�veis locais
   AnsiString diagnostico;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTProduto  *produto  = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //verifica prote��o de sotware
	if (! ValidaProtecaoSw()) return(false);
   //verifica a exist�ncia de Licenca
   if (licencas->ExisteLicenca() == NULL)
      {//pergunta ao usu�rio se ele deseja obter Licenca de uso
      Aviso("N�o existe Licen�a de Uso para o " + produto->Codigo);
      return(false);
      }
   //valida Licenca de Uso existente
   if (! licencas->ValidaLicenca(diagnostico))
      {//exibe aviso de licen�a inv�lida
      Aviso(diagnostico);
      return(false);
      }
   if (show_diag)
      {//exibe aviso de diagn�stico de licen�a v�lida (p/ mostrar tempo de validade)
      Aviso(diagnostico);
      }
   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::ValidaProtecaoSw(void)
	{
	//vari�veis locais
	VTProtesw *protesw = (VTProtesw*)apl->GetObject(__classid(VTProtesw));

   //verifica integridade da DLL de controle de licenca
	if (! protesw->Valida(start_path))
		{
      Erro("Instala��o corrompida!\nEntre em contato com a assist�ncia t�cnica ou reinstale o programa.");
      return(false);
      }
   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormMain::VerificaLoginLocal(void)
	{
   //vari�veis locais
   TForm      *form;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //verifica se a Licenca indica necessidade de Login local
   if (! licencas->LoginLocal) return(true);
   //verifica se j� foi feito Login local
   if (! login_local_ok)
		{//cria TFormLogin
		form = new TFormLogin(this);
      if (form->ShowModal() == mrOk) login_local_ok = true;
      //destr�i TFormLogin
		delete form;
      }
   //disponibiliza op��o de cancelar Login de uso local somente ap�s usu�rio ter feito Login
   ActionLicencaLoginLocal->Visible = login_local_ok;
   return(login_local_ok);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMain::VerificaModulos(void)
   {
   //verifica se usu�rio master
   bool           licenca_master, licenca_user, licenca_demo, licenca_diag;
   bool            master_user_action_enabled;
   VTLicencas     *licencas     = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca      *licenca      = licencas->ExisteLicenca();

   //verifica se n�o h� uma Licenca alocada
   if (licenca == NULL)
      {//desabilita todas Actions
      licenca_master = false;
      licenca_user   = false;
      licenca_demo   = false;
      licenca_diag   = false;
      }
   else
      {//verifica o tipo da Licenca alocada
      switch(licenca->Tipo)
         {
         case tipoMASTER:
            licenca_master = true;
            licenca_user   = false;
            licenca_demo   = false;
            licenca_diag   = false;
            break;
         case tipoUSER:
            licenca_master = false;
            licenca_user   = true;
            licenca_demo   = false;
            licenca_diag   = false;
            break;
         case tipoDEMO:
            licenca_master = false;
            licenca_user   = false;
            licenca_demo   = true;
            licenca_diag   = false;
            break;
         case tipoDIAGNOSTICO:
            licenca_master = false;
            licenca_user   = false;
            licenca_demo   = false;
            licenca_diag   = true;
            break;
         case tipoELPA:
            licenca_master = false;
            licenca_user   = true;
            licenca_demo   = false;
            licenca_diag   = false;
            break;
            }
         }
   //configura Actions
	ActionAbreEstudoSDBT->Enabled     = (licenca_master || licenca_user || licenca_demo);
	ActionAbreEstudoSDMT->Enabled     = (licenca_master || licenca_user || licenca_demo);
   ActionCfgArranjo->Enabled         = (licenca_master || licenca_user || licenca_demo);
	ActionCfgCurvaTipica->Enabled     = (licenca_master || licenca_user || licenca_demo);
	ActionFechaRede->Enabled          = (licenca_master || licenca_user || licenca_demo || licenca_diag);
	ActionImportaSDBT->Enabled        = (licenca_master || licenca_user || licenca_demo);
	ActionImportaSDMT->Enabled        = (licenca_master || licenca_user || licenca_demo);
	ActionLicencaAloca->Enabled       = true;
	ActionLicencaLibera->Enabled      = true;
   ActionLicencaLoginLocal->Enabled  = (licenca_master || licenca_user || licenca_demo || licenca_diag);
	ActionSair->Enabled               = true;
   ActionSalva->Enabled              = (licenca_master || licenca_user || licenca_demo);
   ActionSalvaComo->Enabled          = (licenca_master || licenca_user || licenca_demo);
	}

//---------------------------------------------------------------------------
//eof






