// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEspecial.h"
#include "TFormConexoesRedes.h"
#include "TFormPrintPreview.h"
#include "TFormTesteBateria.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\MultiMonitor\VTMultiPanel.h"
// #include "..\Processo\VTProcesso.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Stk\VTStkEqv.h"
#include "..\..\DLL_Inc\AjusteDemanda.h"
#include "..\..\DLL_Inc\AlocaCapacitor.h"
#include "..\..\DLL_Inc\AlocaRegulador.h"
#include "..\..\DLL_Inc\Balanceamento.h"
#include "..\..\DLL_Inc\BDihm.h"
#include "..\..\DLL_Inc\Cartografia.h"
#include "..\..\DLL_Inc\Confiabilidade.h"
#include "..\..\DLL_Inc\Debug.h"
#include "..\..\DLL_Inc\Diagnostico.h"
#include "..\..\DLL_Inc\Distancia.h"
#include "..\..\DLL_Inc\Equivalente.h"
#include "..\..\DLL_Inc\EstimaDemanda.h"
#include "..\..\DLL_Inc\EstimaEstado.h"
#include "..\..\DLL_Inc\Figura.h"
#include "..\..\DLL_Inc\FluxoH3.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\IhmCustoNodal.h"
#include "..\..\DLL_Inc\InterfaceNIX.h"
#include "..\..\DLL_Inc\InterfaceODSS.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\Perdatec.h"
#include "..\..\DLL_Inc\RelatorioJusante.h"
#include "..\..\DLL_Inc\RelatorioTensao.h"
#include "..\..\DLL_Inc\Protecao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Relatorio.h"
#include "..\..\DLL_Inc\Simulador.h"
#include "..\..\DLL_Inc\Stk.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEspecial::TFormEspecial(TComponent *Owner, VTApl *apl_owner) : TForm(Owner)
{
	// vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));
//	VTLicencas *licencas = (VTLicencas*)apl_owner->GetObject(__classid(VTLicencas));
//	VTLicenca *licenca = licencas->ExisteLicenca();

	// salva ponteiro
	apl = apl_owner;
	// verifica m�dulos desabilitados
	VerificaModulos();
	// insere ActionList
	sinap_child->FormStkAdd("SINAP: M�dulos Especiais", ActionListModulo, false);
	sinap_child->FormStkAdd("SINAP: Relat�rios", ActionListRelatorio, false);
//	// inlcui Menu para usu�rios Master
//	if (licenca->Tipo == tipoMASTER)
//	{
//		sinap_child->FormStkAdd("SINAP: Development", ActionListMaster, false);
//	}
	sinap_child->FormStkAdd("SINAP: Development", ActionListMaster, false);
}

// ---------------------------------------------------------------------------
__fastcall TFormEspecial::~TFormEspecial(void)
{
	DLL_CloseFormAlocaCapacitor(this);
	// DVK 2014.12.03
	DLL_CloseFormMalha(this);
	DLL_CloseFormAlocaRegulador(this);
	DLL_CloseFormConfiabilidade(this);
	DLL_CloseFormDebug(this);
	DLL_CloseFormDemanda(this);
	DLL_CloseFormDiagEqpto(this);
	DLL_CloseFormDiagFlow(this);
	DLL_CloseFormEqv(this);
	DLL_CloseFormEqvWise(this);
	DLL_CloseFormEstimaDemanda(this);
	DLL_CloseFormEstimaEstado(this);
	DLL_CloseFormFluxoH3(this);
	DLL_CloseFormGoogle(this);
	DLL_CloseFormMercado(this);
	DLL_CloseFormPerdareg(this);
	DLL_CloseFormPerdatec(this);
	DLL_CloseFormProtecao(this);
	DLL_CloseRelatorio(this);
	DLL_CloseFormZoom(this);
	DLL_CloseFormRelatorioJusante(this);
    DLL_CloseFormRelatorioTensao(this);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionAjusteDemandaExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modAJUSTEDEMANDA))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opCARGAS));
		return;
	}
	// verifica se h� m�dulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante())
		return;
	// cria Form do m�dulo de demanda
	DLL_NewFormDemanda(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionAlocaCapacitorExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modALOCABC))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opREATIVOS));
		return;
	}
	// verifica se h� m�dulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante())
		return;
	// cria Form do m�dulo de aloca��o de capacitor
	DLL_NewFormAlocaCapacitor(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionAlocaReguladorExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modALOCART))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opREGULADOR));
		return;
	}
	// verifica se h� m�dulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante())
		return;
	// cria Form do m�dulo de aloca��o de regulador
	DLL_NewFormAlocaRegulador(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionAnaliseMalhasExecute(TObject *Sender)
{ // DVK 2014.12.03
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modANALISEMALHA))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
		return;
	}
	// cria Form do m�dulo de an�lise de malhas
	DLL_NewFormMalha(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionBalanceamentoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modBALANCEAMENTO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// verifica se h� m�dulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante())
		return;
	// cria Form do m�dulo de balanceamento
	DLL_NewFormBalanceamento(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionCustoNodalExecute(TObject *Sender)
{
	// exibe tela do Custo Nodal
	DLL_NewFormCustoNodal(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionDiagEqptoExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDIAGEQUIPAMENTO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}

	// exibe tela do diagn�stico da reder
	if ((Form = DLL_NewFormDiagEqpto(this, apl, SelecionaParentParaJanela())) != NULL)
	{
		Form->Show();
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionDiagFlowExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDIAGFLUXO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}

	// exibe tela do editor
	if ((Form = DLL_NewFormDiagFlow(this, apl, SelecionaParentParaJanela())) != NULL)
	{
		Form->Show();
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionDistanciaEletricaExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modCAMINHOELETRICO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// exibe tela de dist�ncia el�trica
	DLL_NewFormDistancia(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionExportaNIXExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;

	// exibe TFormNIX
	form = DLL_NewFormNIX(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionFluxoHarmonicoExecute(TObject *Sender)
{
	// exibe tela do editor
	DLL_NewFormFluxoH3(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionGoogleMapsExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;
	// exibe tela do editor
	// DLL_NewFormGoogle(this, apl, SelecionaParentParaJanela());
	// FKM 2015.09.14 Verifica se j� existe o Form
	if ((form = (TForm*) ExisteModuloAtivo("FormGoogle")) != NULL)
	{
		form->Show();
	}
	else
	{
		DLL_NewFormGoogle(this, apl, SelecionaParentParaJanela());
	}
	// FKM 2015.09.14
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionMercadoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modMERCADO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opCARGAS));
		return;
	}
	// exibe tela do Mercado
	DLL_NewFormMercado(this, apl, SelecionaParentParaJanela());
}

// https://sinapsisenergia.teamwork.com/#/tasks/15921291
// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPerdaRegulatoriaExecute(TObject *Sender)
{
	// verifica se h� m�dulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante())
		return;
	if(ExistePlanejamento())
		return;
	// exibe tela de Pertec
	DLL_NewFormPerdareg(this, apl, SelecionaParentParaJanela());
}

// https://sinapsisenergia.teamwork.com/#/tasks/15921291
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPerdaTecnicaExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modPERDATEC))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// verifica se h� m�dulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante())
		return;
	 if(ExistePlanejamento())
		return;
	// exibe tela de Pertec
	DLL_NewFormPerdatec(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPrintPreviewExecute(TObject *Sender)
{
	/*
	 //cria TFormPrintPreview como janela modal
	 TFormPrintPreview* pp = new TFormPrintPreview(Application, apl);
	 pp->PrintPreview->Refresh();
	 pp->ShowModal();
	 //destr�i o Form
	 delete pp;
	 */
	Aviso("Esta fun��o n�o est� dispon�vel nesta vers�o");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_ConexoesRedesExecute(TObject *Sender)
{
	if (conex == NULL)
	{ // cria TFormConexoesRedes
		conex = new TFormConexoesRedes(this, apl);
	}
	conex->Show();
	// destr�i o Form
	// delete conex;

	// este form ira destruir o conex...
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_ConfiabilidadeExecute(TObject *Sender)
{
	// vari�veis locais
	// int       ind_pat = 0;
	// int       ano     = 0;
	// VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	/*
	 //prote��o para execu��o do m�dulo de Confiabilidade
	 if (! ValidaExecucaoConfiabilidade())
	 {//seleciona gr�fico de topologia
	 ActionTopo->Execute();
	 return;
	 }
	 */
	// fecha janelas
	// FechaJanelas();
	// esconde CBoxPatamar
	// CBoxPatamar->Visible = false;
	// abaixa Button
	// ButConfiabilidade->Down = true;
	// exibe tela de confiabilidade
	DLL_NewFormConfiabilidade(this, apl, SelecionaParentParaJanela());
	// exibe gr�fico de Confiabilidade DEPOIS de ter aberto tela de Confiabilidade (que cria objeto RedeCFB)
	// grafico->Show(grafCONFIAB, ano, ind_pat);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_DebugExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Debug
	DLL_NewFormDebug(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_EqvExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do m�dulo de Equivalentes
	form = DLL_NewFormEqv(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_EqvWiseExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do m�dulo de Equivalentes
	form = DLL_NewFormEqvWise(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_EstimaEstadoExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Estimador
	form = DLL_NewFormEstimaEstado(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_EstimaDemandaExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Estimador
	form = DLL_NewFormEstimaDemanda(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_FigurasTXTExecute(TObject *Sender)
{
	TForm *form;
    VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Estimador
	form = DLL_NewFormFigurasIO(this, apl);
	if (form)
		form->Show();
}


/*
 //---------------------------------------------------------------------------
 void __fastcall TFormEspecial::ActionPriv_ProcessoExecute(TObject *Sender)
 {
 //vari�veis locais
 VTProcesso *processo;

 processo = DLL_NewObjProcesso(apl);
 processo->ActionOnTerminate = NULL;
 processo->Wait              = true;
 processo->Executa(NULL);
 delete processo;
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_InterfaceODSSExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Exportador OpenDSS
	form = DLL_NewFormExportaODSS(this, apl);
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_ProtecaoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela de Prote��o
	DLL_NewFormProtecao(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_SinapClientExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Servidor
	form = DLL_NewFormClient(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_SinapServerExecute(TObject *Sender)
{
	// veri�veis locais
	TForm *form;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	// exibe tela do Servidor
	form = DLL_NewFormServer(this, apl, SelecionaParentParaJanela());
	if (form)
		form->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionPriv_TrocaEqvBTExecute(TObject *Sender)
{
	// vari�veis locais
	VTStkEqv *stk_eqv;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modDEVELOPER))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
			(opDEVELOPER));
		return;
	}
	try
	{ // cria objeto VTStkEqv
		if ((stk_eqv = DLL_NewObjStkEqv(apl)) != NULL)
		{
			stk_eqv->Executa();
			// destr�i objeto VTStkEqv
			delete stk_eqv;
		}
	}
	catch (Exception &e)
	{
		Aviso("N�o foi poss�vel substituir as redes BT equivalentes por cargas");
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionRelCargaExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modRELDEMANDA))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// exibe tela de Relat�rios
	DLL_NewFormRelCarga(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionRelEqptoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modRELEQUIPAMENTO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// exibe tela de Relat�rios
	DLL_NewFormRelEqpto(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionRelFluxoExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modRELFLUXO))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// exibe tela de Relat�rios
	DLL_NewFormRelFluxo(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionRelJusanteExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modRELJUSANTE))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		return;
	}
	// cria Form do m�dulo de demanda
	DLL_NewFormRelatorioJusante(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormEspecial::ActionZoomExecute(TObject *Sender)
{
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modZOOM))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
		return;
	}
	// exibe tela do editor
	DLL_NewFormZoom(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::Atualiza(void)
{
	// DLL_AtualizaFormDiagFlow(this);
	// DLL_AtualizaFormDiagEqpto(this);
	DLL_AtualizaFormProtecao(this);
	DLL_AtualizaRelatorio(this);
	// DLL_AtualizaFormPertec(this, num_pat);
}

// ---------------------------------------------------------------------------
// static TForm* __fastcall TFormEspecial::ExisteRelatorio(TForm *Owner, AnsiString form_name)
// {
// //vari�veis locais
// AnsiString class_name;
// TForm      *form;
//
// //verifica se existe um FormRelatorio aberto
// for (int n = 0; n < Screen->FormCount; n++)
// {//verifica se o Form n � um TFormRelatorio
// class_name = Screen->Forms[n]->ClassName();
// if (class_name.AnsiCompareIC(form_name) == 0)
// {
// if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
// }
// }
// return(NULL);
// }
// ---------------------------------------------------------------------------
// FKM 2015.09.14 - Verifica se existe um modulo ativo usando o nomed o TComponent
TComponent* __fastcall TFormEspecial::ExisteModuloAtivo(AnsiString nomeComponent)
{
	TComponent *owned;
	TForm *form;
	AnsiString nome;

	// Verifica a lista dos TComponents "owned" por esse
	for (int i = 0; i < this->ComponentCount; i++)
	{
		owned = this->Components[i];
		nome = owned->Name;
		if (nome.AnsiCompareIC(nomeComponent) == 0)
		{
			return owned;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEspecial::ExisteModuloAtivoConflitante(void)
{
	// vari�veis locais
	TForm *form;

	// procura Form de m�dulos ativoa que podem ter conflitos
	form = DLL_ExisteFormAlocaCapacitor(this);
	if (form == NULL)
		form = DLL_ExisteFormAlocaRegulador(this);
	if (form == NULL)
		form = DLL_ExisteFormBalanceamento(this);
	if (form == NULL)
		form = DLL_ExisteFormDemanda(this);
	if (form == NULL)
		form = ExisteForm("TFormPerdatec", this);
	if (form == NULL)
		form = ExisteForm("TFormPerdareg", this);
	// verifica se foi encontrado um form
	if (form != NULL)
	{
		Aviso("Feche o m�dulo " + AnsiQuotedStr(form->Caption, '\''));
		return (true);
	}
	return (false);
}

// https://sinapsisenergia.teamwork.com/#/tasks/15921291
// ---------------------------------------------------------------------------
bool __fastcall TFormEspecial::ExistePlanejamento(void)
{
	// vari�veis locais
	TForm *form;

	// procura Form do planejamento
	form = ExisteForm("TFormEstudoNovo", this->Owner);
	// verifica se foi encontrado um form
	if (form != NULL)
	{
		Aviso("Feche o m�dulo " + AnsiQuotedStr(form->Caption, '\''));
		return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
TWinControl* __fastcall TFormEspecial::SelecionaParentParaJanela(void)
{
	// vari�veis locais
	VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

	return (MultiPanel->PanelAtivo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEspecial::VerificaModulos(void)
{
//	// verifica se usu�rio master
//	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
//	VTLicenca *licenca = licencas->ExisteLicenca();

	// habilita/desabilita Actions de m�dulos especiais
	ActionAjusteDemanda->Enabled = DLL_AjusteDemanda_Enabled();
	ActionAlocaCapacitor->Visible = true;
	ActionAlocaRegulador->Visible = true;
	ActionAnaliseMalhas->Visible = true;
	ActionBalanceamento->Visible = true;
	ActionDiagEqpto->Visible = true;
	// ActionDiagFlow->Visible = true;
	ActionDiagFlow->Visible = false;
	ActionDistanciaEletrica->Enabled = true;
	ActionExportaNIX->Visible = true;
	ActionMercado->Visible = true;
	// habilita/desabilita Actions em fun��o da Licen�a
//	 ActionGoogleMaps->Enabled = licenca->Google;

	// oculta exportador Google Earth
//	ActionGoogleMaps->Visible = false;

	// especial COPEL
//	ActionPerdaRegulatoria->Visible = (licenca->Empresa != COPEL);
	ActionPerdaRegulatoria->Visible = true;

	// Actions exclusivas de usu�rios Master
//	ActionPrintPreview->Visible = (licenca->Tipo == tipoMASTER);
//	ActionCustoNodal->Visible = (licenca->Tipo == tipoMASTER);
//	ActionFluxoHarmonico->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_Confiabilidade->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_TrocaEqvBT->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_Protecao->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_Debug->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_Eqv->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_EqvWise->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_SinapServer->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_SinapClient->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_EstimaDemanda->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_EstimaEstado->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_InterfaceODSS->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_InsereBateria->Visible = (licenca->Tipo == tipoMASTER);
//	ActionPriv_ConexoesRedes->Visible = (licenca->Tipo == tipoMASTER);

	ActionPrintPreview->Visible = true;
	ActionCustoNodal->Visible = true;
	ActionFluxoHarmonico->Visible = true;
	ActionPriv_Confiabilidade->Visible = true;
	ActionPriv_TrocaEqvBT->Visible = true;
	ActionPriv_Protecao->Visible = true;
	ActionPriv_Debug->Visible = true;
	ActionPriv_Eqv->Visible = true;
	ActionPriv_EqvWise->Visible = true;
	ActionPriv_SinapServer->Visible = true;
	ActionPriv_SinapClient->Visible = true;
	ActionPriv_EstimaDemanda->Visible = true;
	ActionPriv_EstimaEstado->Visible = true;
	ActionPriv_InterfaceODSS->Visible = true;
	ActionPriv_InsereBateria->Visible = true;
	ActionPriv_ConexoesRedes->Visible = true;
}

// ---------------------------------------------------------------------------
// eof
void __fastcall TFormEspecial::ActionPriv_InsereBateriaExecute(TObject *Sender)
{
	TForm *form;
	form = new TFormTesteBateria(this, SelecionaParentParaJanela(), apl);
	form->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormEspecial::ActionRelTensaoExecute(TObject *Sender)
{
//    VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
//	if ((!rbac->ExistePermissao(opCONTINGENCIAS)) && (!rbac->ExistePermissao(opREDE)))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
//		return;
//	}
	// exibe tela de Relat�rio de Tens�o
	DLL_NewFormRelatorioTensao(this, apl, SelecionaParentParaJanela());
}
//---------------------------------------------------------------------------


