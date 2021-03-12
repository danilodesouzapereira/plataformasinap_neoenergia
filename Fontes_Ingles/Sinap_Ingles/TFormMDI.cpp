//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <objbase.h>
#include <stdlib.h>
#include "TFormMDI.h"
#include "TFormChild.h"
#include "TFormLogin.h"
#include "TFormLicenca.h"
#include "VTSinapMdi.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Constante\Cursor.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTDir.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Mercado\VTMercado.h"
#include "..\MultiMonitor\TFormMultiMonitor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Produto\VTProduto.h"
#include "..\Protesw\VTProtesw.h"
#include "..\Rede\VTTipos.h"
//#include "..\Registro\VTRegistro.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Cfg.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\DensidadeCarga.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Licenca.h"
#include "..\..\DLL_Inc\Login.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "..\..\DLL_Inc\Produto.h"
#include "..\..\DLL_Inc\Perdatec.h"
//#include "..\..\DLL_Inc\Registro.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\SinapToolKit.h"
#include "..\..\DLL_Inc\SinapApl.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TFormMDI *FormMDI;

//---------------------------------------------------------------------------
__fastcall TFormMDI::TFormMDI(TComponent *Owner)
   : TForm(Owner)
   {
	//vari�veis locais
   TForm *Form;
	VTSinapMdi *sinap_mdi;

   //initializes the Component Object Model(COM) library
   CoInitialize(NULL);
   //define Application::Tag com ActionAlerta
   Application->Tag = int(ActionAlerta);
   //seta propriedade DoubleBuffered p/ evitar flicker
   PanelLogo->DoubleBuffered = true;
   //cria FormLogo e insere em PanelLogo
   DLL_NewFormLogo(this, PanelLogo);
   //salva diret�rio de onde o programa foi iniciado
   start_path = GetCurrentDir();
   //inicia Tag p/ numera��o sequencial dos FormChild
   Tag = 1;
   //cria objetos
	apl = DLL_NewObjAplMdi(this, NULL);
	//cria objeto VTSinapMdi e insere em Apl
	if ((sinap_mdi = NewObjSinapMdi(apl)) != NULL)
		{
		sinap_mdi->ActionNewChild   = ActionNewChild;
		sinap_mdi->MainMenu         = MainMenu;
		sinap_mdi->PopupMenuImporta = PopupMenuImporta;
		apl->Add(sinap_mdi);
		}
   //inicia controle de MultiMonitor
   IniciaMultiMonitor();
   //valida diret�rios lidos por Dir
   AtualizaMudancaDir();
   //maximiza form
   WindowState = wsMaximized;
   //carrega formatos p/ cursor do mouse
   IniciaCursorMouse();
   //habilita Actions p/ organizar FormChild
   ActionWinCascade->Enabled        = true;
   ActionWinTileHorizontal->Enabled = true;
   ActionWinTileVertical->Enabled   = true;
   //habilita exibi��o de mensagem p/ Licenca demo
   show_msg_licenca = true;
   //seta op��o p/ N�O requisitar login local
   login_local_ok = false;
   //verifica bases de dados dispon�veis
   VerificaBaseDados();
	//inicia SinapToolKit
	SinapToolKitInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormMDI::~TFormMDI(void)
   {
   //vari�veis locais
   VTGeral        *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva informa��o da base de dado em uso
   geral->Infoset->Clear();
   geral->Infoset->AddInfo("base_dado", geral->BaseDado);
   geral->Infoset->SaveToFile(ClassName());
   //finaliza FormHelp
   DLL_CloseFormHelp(this);
   //finaliza SinapToolKit
	DLL_FormChildStop(this);
	//destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionAbreAllExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //abre FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionAbreAll->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionAbreNoneExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //abre FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionAbreNone->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionAbreSelExecute(TObject *Sender)
   {
	//vari�veis locais
   TFormChild *FormChild;

   //abre FormChild e exibe janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionAbreSel->OnExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionAlertaExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString class_name;
   TAction    *ActionSalva;

   //determina FormChild ativo
   if (ActiveMDIChild == NULL) return;
   //verifica se � TFormChild
   class_name = ActiveMDIChild->ClassName();
   if (class_name.AnsiCompareIC("TFormChild") == 0)
      {//insere mensagem de alerta no FormChild
      ((TFormChild*)ActiveMDIChild)->InsereMsgAlerta(ActionAlerta->Caption);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionBaseAccessExecute(TObject *Sender)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase())
      {//marca que n�o h� op��o de banco de dados
      //geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
      geral->BaseDado = SNPDBTYPE_INDEFINIDO;
      return;
      }
   //for�a bot�o Down
   ButBaseAccess->Down = true;
   //altera cor de PanelBaseDado
   PanelBaseDado->Color = clMoneyGreen;
   //habilita Actions espec�ficas da base Access
   ActionCriaRede->Visible  = true;
   ActionSalvaComo->Visible = true;
   //salva op��o de base de dado
   geral->BaseDado = SNPDBTYPE_ACCESS;
   //reinicia objetos b�sicos a partir da base de dados selecionada
   IniciaObjetosBasicos();
   //atualiza Captions
   AtualizaCaptions();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionBaseOracleExecute(TObject *Sender)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //for�a bot�o Down
   ButBaseOracle->Down = true;
   //altera cor de PanelBaseDado
   PanelBaseDado->Color = clSkyBlue;
   //desabilita Actions espec�ficas da base Access
   ActionCriaRede->Visible  = false;
   ActionSalvaComo->Visible = false;
   //salva op��o de base de dado
   geral->BaseDado = SNPDBTYPE_ORACLE;
   //reinicia objetos b�sicos a partir da base de dados selecionada
   IniciaObjetosBasicos();
   //atualiza Captions
   AtualizaCaptions();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionBatchDensidadeCargaExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->Reinicia();
      FormChild->ActionBatchDensidadeCarga->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionBatchImportaGisExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //pede para usu�rio confirmar base de dados
   if (! ConfirmaBaseDado()) return;
   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->Reinicia();
      FormChild->ActionBatchImportaGis->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionBatchPerdaregExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->Reinicia();
      FormChild->ActionBatchPerdareg->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionBatchPerdatecExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->Reinicia();
      FormChild->ActionBatchPerdatec->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgArranjoExecute(TObject *Sender)
   {
	// vari�veis locais
	AnsiString class_name;
	TFormChild *FormChild;
	VTArranjos *arranjos;
	VTApl *apl_child = NULL;

	// verifica se os diret�rios est�o definidos
	if (!ValidaDirBase())
		return;
	// verifica licen�a
	if (!ValidaLicenca())
		return;
	// verifica se h� redes abertas
	if (MDIChildCount > 0) // 1)
	{ // determina FormChild ativo
		FormChild = (TFormChild*)ActiveMDIChild;
		if (FormChild != NULL)
		{ // determina apl child
			apl_child = FormChild->AplChild();
		}
	}
	// esconde logotipo
	LogoHide();
	// inclui objeto Arranjos em Apl
	apl->Add(arranjos = DLL_NewObjArranjos());
	// edita configura��o de Arranjos
	if ((DLL_ConfiguraArranjos(this, apl, apl_child)) && (MDIChildCount > 0))
	{ // verifica se o usu�rio deseja atualizar redes abertas
		if (Confirma("Configura��o de arranjos atualizada na base de dados.",
			"Deseja alterar configura��o das redes em estudo ?") == IDYES)
		{ // atualiza objeto Geral dos demais FormChild
			for (int n = 0; n < MDIChildCount; n++)
			{
				class_name = MDIChildren[n]->ClassName();
				if (class_name.AnsiCompareIC("TFormChild") != 0)
					continue;
				FormChild = (TFormChild*)MDIChildren[n];
				FormChild->ConfiguraArranjos(apl);
			}
		}
	}
	// retira objeto Arranjos inserido em Apl
	apl->Remove(arranjos);
	// destr�i objeto
	delete arranjos;
	// exibe logotipo
	LogoShow();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgCurvaDemandaExecute(TObject *Sender)
   {
   // vari�veis locais
	AnsiString class_name;
	TFormChild *FormChild;
	VTTipos *tipos;
	VTApl *apl_child = NULL;

	// verifica se os diret�rios est�o definidos
	if (!ValidaDirBase())
		return;
	// verifica licen�a
	if (!ValidaLicenca())
		return;
	// verifica se todas as redes abertas s�o da mesma base
	if (MDIChildCount > 0) // 1)
	{ // determina FormChild ativo
		FormChild = (TFormChild*)ActiveMDIChild;
		if (FormChild != NULL)
		{ // determina apl child
			apl_child = FormChild->AplChild();
		}
	}
	// esconde logotipo
	LogoHide();
	// inclui objeto Tipos em Apl //DVK 2015.10.02
	apl->Add(tipos = DLL_NewObjTipos());
	// edita configura��o de Curvas t�picas
	if ((DLL_ConfiguraCurvaDemanda(this, apl, apl_child)) && (MDIChildCount > 0))
	{ // verifica se o usu�rio deseja atualizar as redes abertas
		if (Confirma("Foram feitas altera��es nas curvas t�picas de demanda.",
			"Deseja alterar configura��o das redes em estudo ?") == IDYES)
		{ // atualiza objeto Geral de todos FormChild
			for (int n = 0; n < MDIChildCount; n++)
			{
				class_name = MDIChildren[n]->ClassName();
				if (class_name.AnsiCompareIC("TFormChild") != 0)
					continue;
				FormChild = (TFormChild*)MDIChildren[n];
				FormChild->ConfiguraCurvasTipicas(apl);
			}
		}
	}
	// retira objeto Tipos inserido em Apl //DVK 2015.10.02
	apl->Remove(tipos);
	// destr�i objeto
	delete tipos;
	// exibe logotipo
	LogoShow();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgFlowExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString  class_name;
   TFormChild  *FormChild;

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //edita configura��o de limites p/ diagn�stico de fluxo
	if ((DLL_ConfiguraFlow(this, apl))&&(MDIChildCount > 0))
		{//verifica se o usu�rio deseja atualizar as redes abertas
		if (Confirma("Configura��o de limites de diagn�stico alterada na base de dados.",
						 "Deseja alterar configura��o das redes em estudo ?") == IDYES)
			{//atualiza objeto Geral de todos FormChild
			for (int n = 0; n < MDIChildCount; n++)
				{
				class_name = MDIChildren[n]->ClassName();
				if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
				FormChild = (TFormChild*)MDIChildren[n];
				FormChild->ConfiguraGeral(apl);
				}
			}
		}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgTipoChaveExecute(TObject *Sender)
	{
	//verifica se os diret�rios est�o definidos
	if (! ValidaDirBase()) return;
	//verifica licen�a
	if (! ValidaLicenca()) return;
	//esconde logotipo
	LogoHide();
	//edita configura��o de Curvas t�picas
	DLL_ConfiguraTipoChave(this, apl);
	//exibe logotipo
	LogoShow();
	}
*/ //DVK 2015.10.01 reescrito, para atualizar cor das chaves no Grafico
//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgTipoChaveExecute(TObject *Sender)
	{
	//vari�veis locais
   AnsiString  class_name;
	TFormChild  *FormChild;
	VTTipos 		*tipos;

	//verifica se os diret�rios est�o definidos
	if (! ValidaDirBase()) return;
	//verifica licen�a
	if (! ValidaLicenca()) return;
	//esconde logotipo
	LogoHide();
	//inclui objeto Tipos em Apl
	apl->Add(tipos = DLL_NewObjTipos());
	//edita configura��o de Tipos de chave
	if ((DLL_ConfiguraTipoChave(this, apl))&&(MDIChildCount > 0))
		{//verifica se o usu�rio deseja atualizar as redes abertas
		if (Confirma("Configura��o de tipos de chave alterada na base de dados.",
						 "Deseja alterar configura��o das redes em estudo ?") == IDYES)
			{//atualiza Grafico de todos FormChild
			for (int n = 0; n < MDIChildCount; n++)
				{
				class_name = MDIChildren[n]->ClassName();
				if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
				FormChild = (TFormChild*)MDIChildren[n];
				FormChild->ConfiguraGrafico(apl);
				}
			}
		}
	//retira objeto Tipos inserido em Apl
	apl->Remove(tipos);
	//destr�i objeto
	delete tipos;
	//exibe logotipo
	LogoShow();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgTipoRedeExecute(TObject *Sender)
	{
	//vari�veis locais
	VTTipos 		*tipos;

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //esconde logotipo
	LogoHide();
	//inclui objeto Tipos em Apl //DVK 2015.10.02
	apl->Add(tipos = DLL_NewObjTipos());
   //edita configura��o de Tipo Rede
	DLL_ConfiguraTipoRede(this, apl);
	//retira objeto Tipos inserido em Apl  //DVK 2015.10.02
	apl->Remove(tipos);
	//destr�i objeto  //DVK 2015.10.02
	delete tipos;
   //exibe logotipo
   LogoShow();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCfgPatamaresExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString  class_name;
   TFormChild  *FormChild;

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //edita configura��o de Patamares
	if ((DLL_ConfiguraPatamares(apl))&&(MDIChildCount > 0))
		{//verifica se o usu�rio deseja atualizar as redes abertas
      if (Confirma("Configura��o de patamares alterada na base de dados.",
                   "Deseja alterar configura��o das redes em estudo ?") == IDYES)
         {//atualiza objeto Patamares de todos FormChild
         for (int n = 0; n < MDIChildCount; n++)
            {
            class_name = MDIChildren[n]->ClassName();
            if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
            FormChild = (TFormChild*)MDIChildren[n];
            FormChild->ConfiguraPatamares(apl);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionCriaRedeExecute(TObject *Sender)
   {
   //cria nova base de dados
   if (! CriaNovaBaseDado()) return;
   //cria FormChild sem exibir janela de sele��o de rede
   ActionAbreNone->OnExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionChildClosedExecute(TObject *Sender)
   {
   //verifica se h� algum FormChild
   if (MDIChildCount <= 1)
      {//exibe PanelLogo
      PanelLogo->Visible = true;
      //habilita op��o de trocar a base de dados
      ActionBaseAccess->Visible = base_oracle_enabled;
      ActionBaseOracle->Visible = base_oracle_enabled;
      //altera LabelBaseDado
      AtualizaCaptions();
      }
   else
      {//esconde PanelLogo
      PanelLogo->Visible = false;
      //desabilita op��o de trocar a base de dados
      ActionBaseAccess->Visible = false;
      ActionBaseOracle->Visible = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionConverteBaseExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionConverteBaseExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionExportaXmlExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString  class_name;
   TFormChild  *FormChild;

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //verifica se existe um FormChild aberto
   for (int n = 0; n < MDIChildCount; n++)
      {
      class_name = MDIChildren[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
      FormChild = (TFormChild*)MDIChildren[n];
      FormChild->ActionExportaXml->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionExportaTxtExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString  class_name;
   TFormChild  *FormChild;

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return;
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //verifica se existe um FormChild aberto
   for (int n = 0; n < MDIChildCount; n++)
      {
      class_name = MDIChildren[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormChild") != 0) continue;
      FormChild = (TFormChild*)MDIChildren[n];
      FormChild->ActionExportaTxt->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionFechaRedeExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString class_name;

   //determina FormChild ativo
   if (ActiveMDIChild == NULL)
      {
      Aviso("N�o h� nenhuma rede aberta.");
      return;
      }
   //verifica se � TFormChild
   class_name = ActiveMDIChild->ClassName();
   if (class_name.AnsiCompareIC("TFormChild") == 0)
      {//salva rede
      ((TFormChild*)ActiveMDIChild)->Close();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionHelpExecute(TObject *Sender)
	{
   //vari�veis locais
   TForm *form;

	try{//cria e exibe FormHelp
	   if ((form = DLL_NewFormHelp(this, apl, "Apresenta��o")) != NULL)
         {
         form->Show();
         }
      }catch(Exception &e)
         {//nada afazer
         }
	}

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionImportaAnafasExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionImportaAnafasExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionImportaAnaredeExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //pede para usu�rio confirmar base de dados
   if (! ConfirmaBaseDado()) return;
   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionImportaAnaredeExecute(NULL);
      }
   }


//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionImportaGisExecute(TObject *Sender)
   {
   //vari�veis locais
   int stk_option = 1;

   //pede para usu�rio confirmar base de dados
   if (! ConfirmaBaseDado()) return;
   //cria FormChild sem mostrar janela de sele��o de rede
   NewFormChild(stk_option);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionImportaTxtExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //pede para usu�rio confirmar base de dados
   if (! ConfirmaBaseDado()) return;
   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionImportaTxtExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionImportaXmlExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormChild *FormChild;

   //pede para usu�rio confirmar base de dados
   if (! ConfirmaBaseDado()) return;
   //cria FormChild sem mostrar janela de sele��o de rede
   if ((FormChild = NewFormChild()) != NULL)
      {
      FormChild->ActionImportaXmlExecute(NULL);
      }
   }
   
/*
//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionLicencaAlocaDemoExecute(TObject *Sender)
   {
   //vari�veis locais
   bool       remota = true;
   bool       demo   = true;
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
   //habilita/desabilita m�dulos de acordo c/ a Licenca alocada
   VerificaModulos();
   }
*/

/*
//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionLicencaAlocaLocalExecute(TObject *Sender)
   {
   //vari�veis locais
   bool       remota = false;
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
   //habilita/desabilita m�dulos de acordo c/ a Licenca alocada
   VerificaModulos();
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionLicencaAlocaRemotaExecute(TObject *Sender)
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
   //habilita/desabilita m�dulos de acordo c/ a Licenca alocada
   VerificaModulos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionLicencaExibeExecute(TObject *Sender)
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
void __fastcall TFormMDI::ActionLicencaLiberaExecute(TObject *Sender)
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
   //habilita/desabilita m�dulos de acordo c/ a Licenca alocada
   VerificaModulos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionLoginOffExecute(TObject *Sender)
   {
   //vari�veis locais
   TForm      *form;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //usu�rio est� habilitando Login local
   if (Confirma("Voc� est� DESABILITANDO a prote��o local de uso do programa.\n"
                "O uso do programa ficar� liberado a todos usu�rios deste micro.",
                "Confirma esta op��o ?") == IDYES)
      {//solicita identifica��o do usu�rio
      form = DLL_NewFormLogin(this, apl);
      if (form->ShowModal() == mrOk)
         {//login inativo
         licencas->LoginLocal = false;
         //configura MenuItem para habilitar login
         MenuLicencaLoginLocal->Action = ActionLoginOn;
         }
      }
   //destr�i TFormLogin
   delete form;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionLoginOnExecute(TObject *Sender)
   {
   //vari�veis locais
   TForm      *form;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //usu�rio est� habilitando Login local
   if (Confirma("Voc� est� HABILITANDO a prote��o local de uso do programa.\n"
                "O uso do programa ficar� restrito ao usu�rio da Licen�a de Uso.",
                "Confirma esta op��o ?") == IDYES)
      {//solicita identifica��o do usu�rio
      form = DLL_NewFormLogin(this, apl);
      if (form->ShowModal() == mrOk)
         {//login habilitado
         licencas->LoginLocal = true;
         //configura MenuItem para desabilitar login
         MenuLicencaLoginLocal->Action = ActionLoginOff;
         }
      //destr�i TFormLogin
      delete form;
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionMonitorExchangeExecute(TObject *Sender)
   {
   //vari�veis locais
   int        index, index_mdi, index_aux;
   TFormMultiMonitor *FormMultiMonitor = (TFormMultiMonitor*)apl->GetObject("TFormMultiMonitor");

   //prote��o: verifica exist�ncia de MultiMonitor
   if (Screen->MonitorCount == 1) return;
   if (FormMultiMonitor == NULL)         return;
   //determina Monitor em uso pelo programa
   index = (Monitor == Screen->Monitors[0]) ? 0 : 1;
   //define �ndices p/ monitor da tela principal e da tela auxiliar
   index_mdi = 1 - index;
   index_aux = 1 - index_mdi;
   //esconde os Forms
   Hide();
   FormMultiMonitor->Hide();
   //minimiza este Form e FormMultiMonitor
   WindowState                   = wsMinimized;
   FormMultiMonitor->WindowState = wsMinimized;
   //exibe este Form e FormMultiMonitor
   Show();
   FormMultiMonitor->Show();
   //transfere este Form p/ seu novo monitor
   Left             = Screen->Monitors[index_mdi]->Left;
   //transfere FormMultiMonitor p/ seu novo monitor
   FormMultiMonitor->Left  = Screen->Monitors[index_aux]->Left;
   //maximiza este Form e FormMultiMonitor
   WindowState                   = wsMaximized; //wsNormal;
   FormMultiMonitor->WindowState = wsMaximized;  //wsNormal;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionMonitorExchangeExecute(TObject *Sender)
   {
   //vari�veis locais
   int               index, index_mdi, index_aux, height, width;
   TRect             rect;
   TFormMultiMonitor *FormMultiMonitor = (TFormMultiMonitor*)apl->GetObject("TFormMultiMonitor");

   //prote��o: verifica exist�ncia de MultiMonitor
   if (Screen->MonitorCount == 1) return;
   if (FormMultiMonitor == NULL)  return;
   //determina Monitor em uso pelo programa
   index = (Monitor == Screen->Monitors[0]) ? 0 : 1;
   //define �ndices p/ monitor da tela principal e da tela auxiliar
   index_mdi = 1 - index;
   index_aux = 1 - index_mdi;
   //transfere este Form p/ seu novo monitor
   rect             = Screen->Monitors[index_mdi]->WorkareaRect;
   height           = rect.bottom - rect.top  + 1;
   width            = rect.right  - rect.left + 1;
   Top              = rect.top;
   Left             = rect.left;
   Height           = height;   
   Width            = width;
   WindowState      = wsMaximized;
   //transfere FormMultiMonitor p/ seu novo monitor
   rect                          = Screen->Monitors[index_aux]->WorkareaRect;
   height                        = rect.bottom - rect.top  + 1;
   width                         = rect.right  - rect.left + 1;
   FormMultiMonitor->Top         = rect.top;
   FormMultiMonitor->Left        = rect.left;
   FormMultiMonitor->Height      = height;   
   FormMultiMonitor->Width       = width;    
   FormMultiMonitor->WindowState = wsMaximized;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionMultiMonitorExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormMultiMonitor *FormMultiMonitor = (TFormMultiMonitor*)apl->GetObject("TFormMultiMonitor");

   //prote��o: verifica exist�ncia de MultiMonitor
   if (Screen->MonitorCount == 1)
      {
      if (MenuMultiMonitor->Checked)
         {
         Aviso("O segundo monitor n�o est� dispon�vel");
         MenuMultiMonitor->Checked = false;
         }
      return;
      }
   //verifica se est� habilitando o segundo monitor
   if (MenuMultiMonitor->Checked)
      {//exibe e posiciona FormMultiMonitor no segundo monitor
      FormMultiMonitor->Show();
      FormMultiMonitor->Left        = Screen->Monitors[1]->Left;
      FormMultiMonitor->WindowState = wsMaximized;
      }
   else
      {//desabilitando o segundo monitor: verifica se os Monitores est�o invertidos
      if (Monitor != Screen->Monitors[0])
         {//for�a troca do Monitor
         ActionMonitorExchange->Execute();
         }
      //esconde FormMultiMonitor
      FormMultiMonitor->Hide();
      }
   //habilita/desabilita uso do FormMultiMonitor
   FormMultiMonitor->Ativo        = MenuMultiMonitor->Checked;
   ActionMonitorExchange->Enabled = MenuMultiMonitor->Checked;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionNewChildExecute(TObject *Sender)
	{
	//vari�veis locais
	int stk_option = int(Sender);

	//abre FormChild indicando op��o do SinapToolKit
	NewFormChild(stk_option);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionRelatorioDensidadeCargaExecute(TObject *Sender)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //verifica se h� um base de dados selecionada
   if (geral->BaseDado == SNPDBTYPE_INDEFINIDO)
      {//tenta configurar base Access
      ActionBaseAccess->Execute();
      if (geral->BaseDado == SNPDBTYPE_INDEFINIDO) return;
      }
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //verifica uso de Licenca "free": demo ou did�tica
   VerificaLicencaFree();
   //verifica uso de Login local
   if (! VerificaLoginLocal()) return;
   //esconde logotipo
   LogoHide();
   try{//cria FormDensidadeCarga
      DLL_NewFormDensidadeCarga(Application, apl, ActionChildClosed);
      //desabilita op��o de trocar a base de dados
      ActionBaseAccess->Visible = false;
      ActionBaseOracle->Visible = false;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionRelatorioPerdaregExecute(TObject *Sender)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //verifica se h� um base de dados selecionada
   if (geral->BaseDado == SNPDBTYPE_INDEFINIDO)
      {//tenta configurar base Access
      ActionBaseAccess->Execute();
      if (geral->BaseDado == SNPDBTYPE_INDEFINIDO) return;
      }
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //verifica uso de Licenca "free": demo ou did�tica
   VerificaLicencaFree();
   //verifica uso de Login local
   if (! VerificaLoginLocal()) return;
   //esconde logotipo
   LogoHide();
   try{//cria um novo FormHistoricoPerdareg (que � do tipo FormChild)
      DLL_NewFormHistoricoPerdareg(Application, apl, ActionChildClosed);
       }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionRelatorioPerdatecExecute(TObject *Sender)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //verifica se h� um base de dados selecionada
   if (geral->BaseDado == SNPDBTYPE_INDEFINIDO)
      {//tenta configurar base Access
      ActionBaseAccess->Execute();
      if (geral->BaseDado == SNPDBTYPE_INDEFINIDO) return;
      }
   //verifica licen�a
   if (! ValidaLicenca()) return;
   //verifica uso de Licenca "free": demo ou did�tica
   VerificaLicencaFree();
   //verifica uso de Login local
   if (! VerificaLoginLocal()) return;
   //esconde logotipo
   LogoHide();
   try{//cria um novo FormHistoricoPerdatec (que � do tipo FormChild)
      DLL_NewFormHistoricoPerdatec(Application, apl, ActionChildClosed);
       }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionSairExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionSalvaExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString class_name;
   TAction    *ActionSalva;

   //determina FormChild ativo
   if (ActiveMDIChild == NULL)
      {
      Aviso("N�o h� nenhuma rede aberta.");
      return;
      }
   //verifica se � TFormChild
   class_name = ActiveMDIChild->ClassName();
   if (class_name.AnsiCompareIC("TFormChild") == 0)
      {//salva rede
      ActionSalva = ((TFormChild*)ActiveMDIChild)->ActionSalva;
      //verifica se Action foi bloqueada por outro m�dulo, vis Stk
      if (! ActionSalva->Enabled)
         {//verifica se foi definida uma msg p/ o usu�rio
         if (ActionSalva->Hint.IsEmpty()) Aviso("A op��o de salvar n�o est� dispon�vel.\nUtilize o comando 'Salvar como...' ");
         else                             Aviso(ActionSalva->Hint);
         return;
         }
      ((TFormChild*)ActiveMDIChild)->ActionSalva->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionSalvaComoExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString class_name;
   TFormChild *form_child;
   //VTApl      *apl_child;
   //VTPath     *path_child;
   //VTDir      *dir  = (VTDir*)apl->GetObject(__classid(VTDir));
   //VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //determina FormChild ativo
   if (ActiveMDIChild == NULL)
      {
      Aviso("N�o h� nenhuma rede aberta");
      return;
      }
   //verifica se � TFormChild
   class_name = ActiveMDIChild->ClassName();
   if (class_name.AnsiCompareIC("TFormChild") == 0)
      {//verifica licen�a
      if (! ValidaLicenca()) return;
      //salva rede
      form_child = (TFormChild*)ActiveMDIChild;
      form_child->ActionSalvaComo->Execute();
      //N�O � ncess�rio atualizar objetos VTDir e VTPath, pois eles j� apontam p/ nova base de dados
      //apl_child    = form_child->AplChild();
      //path_child   = (VTPath*)apl_child->GetObject(__classid(VTPath));
      //dir->DirRede = path_child->DirRedeBase();
      //path->DefineDirRede(path_child->DirRedeBase());
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionSobreSinapExecute(TObject *Sender)
   {
   //vari�veis locais
   TForm *form;

   //esconde Logo
   LogoHide();
   try{//cria e exibe FormSobreSinap
	if ((form = DLL_NewFormSobre(this, apl)) != NULL)
      {
      form->ShowModal();
      delete form;
      }
      }catch(Exception &e)
         {
         }
   //exibe Log
   LogoShow();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionSobreSinapsisExecute(TObject *Sender)
   {
   //vari�veis locais
   TForm *form;

   //esconde Logo
   LogoHide();
   try{//cria e exibe FormSobreSinap
	   if ((form = DLL_NewFormSobreSinapsis(this, apl)) != NULL)
         {
         form->ShowModal();
         delete form;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   //exibe Log
   LogoShow();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionUsuarioExecute(TObject *Sender)
   {
   //vari�veis locais
   VTDir *dir  = (VTDir*)apl->GetObject(__classid(VTDir));

   //fecha FormChild abertos
   for (int n = MDIChildCount-1; n >= 0; n--) MDIChildren[n]->Close();
   //seleciona dir_base
   if (dir->SelDirBase())
      {//trata mudan�a do diret�rio base do usu�rio
      AtualizaMudancaDir();
      //altera LabelBaseDado
      AtualizaCaptions();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionWinCascadeExecute(TObject *Sender)
   {
   Cascade();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionWinTileHorizontalExecute(TObject *Sender)
   {
   TileMode = tbHorizontal;
   Tile();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ActionWinTileVerticalExecute(TObject *Sender)
   {
   TileMode = tbVertical;
   Tile();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::AlocaLicenca(bool remota, bool demo)
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
         else
            {//aloca Licen�a local
            sucesso = licencas->AlocaLicencaLocal(FormLogin->GetLogin(), FormLogin->GetPassword(),
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
void __fastcall TFormMDI::AtualizaCaptions(void)
   {
   //vari�veis locais
   VTDir     *dir     = (VTDir*)apl->GetObject(__classid(VTDir));
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //atualiza Caption do Form
   Caption = produto->Codigo + ": " + produto->Descricao;
   //atualiza MainMenu
   ActionHelp->Caption       = produto->Codigo + " help";
   ActionSobreSinap->Caption = "About "  + produto->Codigo + "...";
   //atualiza Caption do LabelBaseDado
   if (geral->BaseDado == SNPDBTYPE_ORACLE)
      {
      LabelBaseDado->Caption = ActionBaseOracle->Caption;
      }
   else if (dir) LabelBaseDado->Caption = ActionBaseAccess->Caption + ": " + ExtractFileName(dir->ArqRede);
   else          LabelBaseDado->Caption = ActionBaseAccess->Caption;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::ApplicationEventsMinimize(TObject *Sender)
   {
   WindowState = wsMinimized;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::AtualizaMudancaDir(void)
   {
   //vari�veis locais
   VTDir  *dir       = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath *path      = (VTPath*)apl->GetObject(__classid(VTPath));

   //atualiza TPath
   path->DirBase = dir->DirBase;
   //path->ArqRede = dir->ArqRede;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::ConfirmaBaseDado(void)
   {
   //vari�veis locais
   AnsiString txt1, txt2;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   txt1 = "Base de dados atualmente selecionada:\n" +
          AnsiQuotedStr(path->ArqRede, '\'');
   txt2 = "Deseja criar uma nova base de dados para as redes que ser�o importadas ?";
   switch(Confirma(txt1, txt2))
      {
      case IDCANCEL: return(false);
      case IDNO:     return(true);
      case IDYES:    return(CriaNovaBaseDado());
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::CriaNovaBaseDado(AnsiString arq_dest)
   {
   //vari�veis locais
   VTDir  *dir    = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath *path   = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se os diret�rios est�o definidos
   if (! ValidaDirBase()) return(false);
   //cria uma nova base de dados vazia
   if (! dir->CriaCopiaRedeEmpresa(arq_dest)) return(false);
   //atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
   path->ArqRede = arq_dest;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::FormResize(TObject *Sender)
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
void __fastcall TFormMDI::FormShow(TObject *Sender)
   {
   //vari�veis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //executa FormResize p/ ajustar logo
   FormResize(NULL);
   //configura MultiMonitor
   ActionMultiMonitor->Execute();
   //inicia controle de uso de acordo c/ tipo da Licenca
   IniciaControleUso();
   //habilita/desabilita m�dulos de acordo c/ a Licenca de uso
   VerificaModulos();
   //gera aviso sobre per�odo de validade da licen�a de uso
   if (licencas->ValidadeLimitada) {ValidaLicenca(true);}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::IniciaControleUso(void)
   {
   //vari�veis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //configura Actions para exigir ou n�o login local
   if (licencas->LoginLocal)
      {//login ativo: configura MenuItem para desabilitar login
      MenuLicencaLoginLocal->Action = ActionLoginOff;
      }
   else
      {//login inativo: configura MenuItem para habilitar login
      MenuLicencaLoginLocal->Action = ActionLoginOn;
      }
   //verifica se deve solicitar Login de controle local
   if (licencas->LoginLocal)
      {//solicita identifica��o do usu�rio
      if (! VerificaLoginLocal())
         {//finaliza o programa
         Close();
         return;
         }
      }
   //verifica se � uma Licenca demo
   if (licencas->LicencaTipoDemo())
      {//habilita TimerDemo usado p/ controle de uso de Licenca demo por 2 horas e 45 minutos
      TimerDemo->Tag      = 4;
      TimerDemo->Interval = (((2 * 60) + 45) * 60) * 1000;
//para teste
//TimerDemo->Interval = 15 * 1000;
      TimerDemo->Enabled = true;
      }
   //verifica se � uma Licenca did�tica
   else if (licencas->LicencaTipoDidatica())
      {//habilita TimerDemo usado p/ controle de uso de Licenca demo por 7 horas e 45 minutos
      TimerDemo->Tag      = 4;
      TimerDemo->Interval = (((7 * 60) + 45) * 60) * 1000;
//para teste
//TimerDemo->Interval = 15 * 1000;
      TimerDemo->Enabled = true;
      }
   else
      {//n�o se trata de Licenca tipoDEMO: desabilita TimerDemo
      TimerDemo->Enabled = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::IniciaCursorMouse(void)
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
                                          {curTrafoZZ   , "TrafoZZ.cur"},
                                          {curMotor     , "Motor.cur"}  };

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
//    if (! FileExists(arquivo))
//       {
//       Aviso("A instala��o n�o inclui cursor do mouse:\n" + arquivo);
//       }
      Screen->Cursors[index] = LoadCursorFromFile(arquivo.c_bstr());
      }
   /*
   Screen->Cursors[curCapacitor]  = LoadCursor(HInstance, WideString("CAPACITOR").c_bstr());
   Screen->Cursors[curCarga]      = LoadCursor(HInstance, WideString("CARGA").c_bstr());
   Screen->Cursors[curChave]      = LoadCursor(HInstance, WideString("CHAVE").c_bstr());
   Screen->Cursors[curGerador]    = LoadCursor(HInstance, WideString("GERADOR").c_bstr());
   Screen->Cursors[curSuprimento] = LoadCursor(HInstance, WideString("SUPRIMENTO").c_bstr());
   Screen->Cursors[curTrafo]      = LoadCursor(HInstance, WideString("TRAFO").c_bstr());
   Screen->Cursors[curTrafo3E]    = LoadCursor(HInstance, WideString("TRAFO3E").c_bstr());
   Screen->Cursors[curTrecho]     = LoadCursor(HInstance, WideString("TRECHO").c_bstr());
   Screen->Cursors[curZoom]       = LoadCursor(HInstance, WideString("ZOOM").c_bstr());
   Screen->Cursors[curGrupo]      = LoadCursor(HInstance, WideString("GRUPO").c_bstr());
   Screen->Cursors[curManobra]    = LoadCursor(HInstance, WideString("MANOBRA").c_bstr());
   Screen->Cursors[curAbreChv]    = LoadCursor(HInstance, WideString("ABRE_CHV").c_bstr());
   Screen->Cursors[curFechaChv]   = LoadCursor(HInstance, WideString("FECHA_CHV").c_bstr());
   Screen->Cursors[curRede]       = LoadCursor(HInstance, WideString("REDE").c_bstr());
   Screen->Cursors[curReducao]    = LoadCursor(HInstance, WideString("REDUCAO").c_bstr());
   Screen->Cursors[curMedidor]    = LoadCursor(HInstance, WideString("MEDIDOR").c_bstr());
   Screen->Cursors[curBarSwing]   = LoadCursor(HInstance, WideString("BAR_SWING").c_bstr());
   Screen->Cursors[curSwing]      = LoadCursor(HInstance, WideString("SWING").c_bstr());
   Screen->Cursors[curSpy]        = LoadCursor(HInstance, WideString("SPY").c_bstr());
   Screen->Cursors[curYref]       = LoadCursor(HInstance, WideString("ZREF").c_bstr());
   Screen->Cursors[curCapserie]   = LoadCursor(HInstance, WideString("CAPSERIE").c_bstr());
   Screen->Cursors[curFiltro]     = LoadCursor(HInstance, WideString("FILTRO").c_bstr());
   Screen->Cursors[curMutua]      = LoadCursor(HInstance, WideString("MUTUA").c_bstr());
   Screen->Cursors[curRegulador]  = LoadCursor(HInstance, WideString("REGULADOR").c_bstr());
   Screen->Cursors[curReator]     = LoadCursor(HInstance, WideString("REATOR").c_bstr());
   Screen->Cursors[curTrafoZZ]    = LoadCursor(HInstance, WideString("TRAFOZZ").c_bstr());
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::IniciaMultiMonitor(void)
   {
   //vari�veis locais
   TFormMultiMonitor *FormMultiMonitor;
   
   //cria FormMultiMonitor
   FormMultiMonitor = new TFormMultiMonitor(this);
   FormMultiMonitor->DefaultMonitor = dmDesktop;
   FormMultiMonitor->WindowState    = wsMinimized;
   FormMultiMonitor->Hide();
   //insere FormMultiMonitor em Apl
   apl->Add(FormMultiMonitor);
   //habilita Action p/ habilitar/desabilitar 2 monitore
   ActionMultiMonitor->Enabled = true;
   //seleciona op��o p/ usar apenas um Monitor
   MenuMultiMonitor->Checked   = false;
   //desabilita Action p/ troca das telas nos moniores
   ActionMonitorExchange->Enabled = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::IniciaObjetosBasicos(void)
   {
   //vari�veis locais
	VTBD    *bd;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //reinicia objetos VTGeral e VTMercado
   geral->ReiniciaValoresDefault();
   try
      {//verifica a  base de dados selecionada
		bd = DLL_NewObjBD(apl);
      bd->Conecta();
      bd->Geral->LeGeral(geral);
      delete bd;
      }
   catch(Exception &e)
      {
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormMDI::LogoHide(void)
   {
   PanelLogo->Visible = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::LogoShow(void)
   {
	PanelLogo->Visible = (MDIChildCount == 0);
   }

//---------------------------------------------------------------------------
TFormChild* __fastcall TFormMDI::NewFormChild(int stk_option)
	{
   //vari�veis locais
   TFormChild *FormChild;
   VTGeral    *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //verifica se os diret�rios est�o definidos
   //if (! ValidaDirBase()) return(NULL);
   //verifica se h� um base de dados selecionada
   if (geral->BaseDado == SNPDBTYPE_INDEFINIDO)
      {//tenta configurar base Access
      ActionBaseAccess->Execute();
      if (geral->BaseDado == SNPDBTYPE_INDEFINIDO) return(NULL);
      }
   //verifica licen�a
   if (! ValidaLicenca()) return(NULL);
   //verifica uso de Licenca "free": demo ou did�tica
	VerificaLicencaFree();
   //verifica uso de Login local
   if (! VerificaLoginLocal()) return(NULL);
   //esconde logotipo
   LogoHide();
   try{//cria TFormChild
      FormChild = new TFormChild(Application, apl, ActionChildClosed, Tag++, stk_option);
		FormChild->ConfiguraPatamares(apl);
		//esconde PanelAlerta  - colocado  p/ evitar efeito feio na tela se colocado em FormChild
		FormChild->PanelAlerta->Visible = false;
		//desabilita op��o de trocar a base de dados
      ActionBaseAccess->Visible = false;
      ActionBaseOracle->Visible = false;
      //atualiza a tela
      Refresh();
      Application->ProcessMessages();
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(FormChild);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::SinapToolKitInicia(void)
   {
   //inicia SinapToolKit
	DLL_FormMdiStart(this, apl);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMDI::TimerDemoTimer(TObject *Sender)
   {
   //vari�veis locais
   AnsiString txt;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //redefine intervalo de tempo de controle p/ 5 minutos
   TimerDemo->Interval = 5 * 60 * 1000;
//para teste
//TimerDemo->Interval = 10 * 1000;
   //verifica se � uma Licenca de Uso demo
   if (licencas->LicencaTipoDemo())
      {//define texto da mensagem de aviso
      txt = (licencas->ExisteLicenca()) ? "Licen�a de Uso DEMO." : "Programa sem Licen�a de Uso.";
      //decrementa Timer::Tag usado p/ controle de t�rmino do programa
      TimerDemo->Tag--;
      if (TimerDemo->Tag <= 0)
         {
         Aviso(txt + "\n\nTempo de uso esgotado. O programa ser� finalizado!");
         exit(EXIT_FAILURE);
         }
      Aviso(txt + "\n\nTempo de uso restante: " + IntToStr((TimerDemo->Tag)*5) + " minutos.");
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::ValidaDirBase(void)
   {
   //vari�veis locais
   VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));

   //valida diret�rios
   return(dir->ValidaDirBase());
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::ValidaDirRede(void)
   {
   //vari�veis locais
   VTDir *dir     = (VTDir*)apl->GetObject(__classid(VTDir));

   //valida diret�rios
   return(dir->ValidaArqRede());
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::ValidaLicenca(bool show_diag)
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
bool __fastcall TFormMDI::ValidaProtecaoSw(void)
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
void __fastcall TFormMDI::VerificaBaseDados(void)
   {
   //vari�veis locais
   int         base_dado;
   AnsiString  oracle_status;
   VTBD       *bd_oracle;
   //VTRegistro *registro;
   VTGeral    *geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //VTProduto  *produto   = (VTProduto*)apl->GetObject(__classid(VTProduto));

   try{//determina �ltima base de dados utilizada
      geral->Infoset->Clear();
      geral->Infoset->LoadFromFile(ClassName());
      if (! geral->Infoset->GetInfo("base_dado", base_dado)) base_dado = SNPDBTYPE_ACCESS;
      //assume base Oracle inativa
      base_oracle_enabled = false;
      //tenta criar conex�o c/ Oracle
      bd_oracle = DLL_NewObjBD(apl);
      base_oracle_enabled = bd_oracle->ConectaServidorRedeRemoto();
      //fecha conex�o c/ Oracle e destr�i objeto
      bd_oracle->DesconectaRede();
      delete bd_oracle;
      //habilita/desabilita op��o de trocar a base de dados
      ActionBaseAccess->Visible = base_oracle_enabled;
      ActionBaseOracle->Visible = base_oracle_enabled;
      MenuItemBaseDado->Visible = base_oracle_enabled;
      //define base de dados a ser utilizada
      switch (base_dado)
         {
         case SNPDBTYPE_ORACLE:
            if (base_oracle_enabled) ActionBaseOracle->Execute();
            else                     ActionBaseAccess->Execute();
            break;
         case SNPDBTYPE_ACCESS:
            ActionBaseAccess->Execute();
            break;
         default:
            ActionBaseAccess->Execute();
            break;
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::VerificaLicencaFree(void)
   {
   //vari�veis locais
   TForm      *form;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));


   //verifica se � uma Licenca tipo demo
   if (licencas->LicencaTipoDemo())
      {//verifica se deve exibir mensagem p/ Licenca demo
      if (! show_msg_licenca) return;
      //cria form p/ exibir termo de aceita��o de uso de Licenca demo
      form = DLL_NewFormTermoUso(this, apl, tipoDEMO);
      form->ShowModal();
      delete form;
      //desabilita exibi��o de mensagem p/ Licenca demo
      show_msg_licenca = false;
      }
   //verifica se � uma Licenca tipo did�tica
   else if (licencas->LicencaTipoDidatica())
      {//verifica se deve exibir mensagem p/ Licenca demo
      if (! show_msg_licenca) return;
      //cria form p/ exibir termo de aceita��o de uso de Licenca demo
      form = DLL_NewFormTermoUso(this, apl, tipoDIDATICA);
      form->ShowModal();
      delete form;
      //desabilita exibi��o de mensagem p/ Licenca demo
      show_msg_licenca = false;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMDI::VerificaLoginLocal(void)
   {
   //vari�veis locais
   TForm      *form;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   //verifica se a Licenca indica necessidade de Login local
   if (! licencas->LoginLocal) return(true);
   //verifica se j� foi feito Login local
   if (! login_local_ok)
      {//cria TFormLogin
      form = DLL_NewFormLogin(this, apl);
      if (form->ShowModal() == mrOk) login_local_ok = true;
      //destr�i TFormLogin
      delete form;
      }
   //disponibiliza op��o de cancelar Login de uso local somente ap�s usu�rio ter feito Login
   return(login_local_ok);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMDI::VerificaModulos(void)
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
   ActionAbreSel->Enabled            = (licenca_master || licenca_user || licenca_demo);
   ActionAbreNone->Enabled           = (licenca_master || licenca_user || licenca_demo);
   ActionCfgArranjo->Enabled         = (licenca_master || licenca_user || licenca_demo);
   ActionCfgCurvaDemanda->Enabled    = (licenca_master || licenca_user || licenca_demo);
   ActionCfgPatamares->Enabled       = (licenca_master || licenca_user || licenca_demo);
   ActionConverteBase->Enabled       = true;
   ActionCriaRede->Enabled           = (licenca_master || licenca_user || licenca_demo);
   ActionCronometraBaseDado->Visible = licenca_master;
   ActionExportaXml->Enabled         = (licenca_master || licenca_user || licenca_demo || licenca_diag);
   ActionFechaRede->Enabled          = (licenca_master || licenca_user || licenca_demo || licenca_diag);
//   ActionImportaAnafas->Enabled      = (licenca_master || licenca_user || licenca_demo);
   ActionImportaAnarede->Enabled     = (licenca_master || licenca_user || licenca_demo);
   //FKM 2015.09.15 - Esconde op��o de usu�rios na privilegiados
   ActionImportaAnafas->Visible      = (licenca_master);
   ActionImportaAnarede->Visible     = (licenca_master || licenca_user || licenca_demo);
//   ActionImportaAnarede->Visible     = (licenca_master);
   //FKM 2015.09.15
   ActionImportaXml->Enabled         = (licenca_master || licenca_user || licenca_demo || licenca_diag);
   ActionLicencaAlocaRemota->Enabled = true;
   ActionLicencaLibera->Enabled      = true;
   //ActionLicencaLoginLocal->Enabled  = (licenca_master || licenca_user || licenca_demo || licenca_diag);
   ActionSair->Enabled               = true;
   ActionSalva->Enabled              = (licenca_master || licenca_user || licenca_demo);
   ActionSalvaComo->Enabled          = (licenca_master || licenca_user || licenca_demo);
   ActionUsuario->Visible            = (licenca_master || ((licenca != NULL)&&(licenca->Empresa == CEPEL)));

	}

//---------------------------------------------------------------------------
//eof
//0x60000000
//0x6FFFFFFF
//0x40000000
//0xC0000000
//0x78000000


