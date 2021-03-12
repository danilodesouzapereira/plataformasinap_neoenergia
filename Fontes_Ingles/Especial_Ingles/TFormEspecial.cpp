//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEspecial.h"
#include "TFormPrintPreview.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\MultiMonitor\VTMultiPanel.h"
//#include "..\Processo\VTProcesso.h"
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
#include "..\..\DLL_Inc\Estabilidade.h"
#include "..\..\DLL_Inc\EstimaDemanda.h"
#include "..\..\DLL_Inc\EstimaEstado.h"
#include "..\..\DLL_Inc\FluxoH3.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\IhmCustoNodal.h"
#include "..\..\DLL_Inc\InterfaceNIX.h"
#include "..\..\DLL_Inc\InterfaceODSS.h"
//#include "..\..\DLL_Inc\InterfaceSpin.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\PartidaMotor.h"
#include "..\..\DLL_Inc\Perdatec.h"
#include "..\..\DLL_Inc\Protecao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Relatorio.h"
#include "..\..\DLL_Inc\Simulador.h"
#include "..\..\DLL_Inc\Stk.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEspecial::TFormEspecial(TComponent *Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //variáveis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));
	VTLicencas   *licencas    = (VTLicencas*)apl_owner->GetObject(__classid(VTLicencas));
	VTLicenca    *licenca     = licencas->ExisteLicenca();


   //salva ponteiro
   apl = apl_owner;
   //verifica módulos desabilitados
   VerificaModulos();
   //insere ActionList
	sinap_child->FormStkAdd("SINAP: Special Components", ActionListModulo);
	sinap_child->FormStkAdd("SINAP: Reports",            ActionListRelatorio);
   //inlcui Menu para usuários Master
   if (licenca->Tipo == tipoMASTER)
      {
		sinap_child->FormStkAdd("SINAP: Development",       ActionListMaster, true);
      }
   }

//---------------------------------------------------------------------------
__fastcall TFormEspecial::~TFormEspecial(void)
   {
	DLL_CloseFormAlocaCapacitor(this);
	//DVK 2014.12.03
	DLL_CloseFormMalha(this);
   DLL_CloseFormAlocaRegulador(this);
   DLL_CloseFormConfiabilidade(this);
   DLL_CloseFormDebug(this);
   DLL_CloseFormDemanda(this);
   DLL_CloseFormDiagEqpto(this);
   DLL_CloseFormDiagFlow(this);
//   DLL_CloseFormEquivalente(this);
   DLL_CloseFormEstabilidade(this);
   DLL_CloseFormEstimaDemanda(this);
   DLL_CloseFormEstimaEstado(this);
   DLL_CloseFormFluxoH3(this);
   DLL_CloseFormGoogle(this);
   DLL_CloseFormMercado(this);
   DLL_CloseFormPartidaMotor(this);
   DLL_CloseFormPerdareg(this);
   DLL_CloseFormPerdatec(this);
   DLL_CloseFormProtecao(this);
   DLL_CloseRelatorio(this);
   DLL_CloseFormZoom(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionAjusteDemandaExecute(TObject *Sender)
   {
   //verifica se há módulos abertos que podem gerar conflitos
   if (ExisteModuloAtivoConflitante()) return;
   //cria Form do módulo de demanda
   DLL_NewFormDemanda(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionAlocaCapacitorExecute(TObject *Sender)
   {
   //verifica se há módulos abertos que podem gerar conflitos
   if (ExisteModuloAtivoConflitante()) return;
   //cria Form do módulo de alocação de capacitor
   DLL_NewFormAlocaCapacitor(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionAlocaReguladorExecute(TObject *Sender)
	{
	//verifica se há módulos abertos que podem gerar conflitos
	if (ExisteModuloAtivoConflitante()) return;
	//cria Form do módulo de alocação de regulador
	DLL_NewFormAlocaRegulador(this, apl, SelecionaParentParaJanela());
	}

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionAnaliseMalhasExecute(TObject *Sender)
	{//DVK 2014.12.03
	//cria Form do módulo de análise de malhas
	DLL_NewFormMalha(this, apl, SelecionaParentParaJanela());
	}

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionBalanceamentoExecute(TObject *Sender)
   {
   //verifica se há módulos abertos que podem gerar conflitos
   if (ExisteModuloAtivoConflitante()) return;
   //cria Form do módulo de balanceamento
   DLL_NewFormBalanceamento(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionCustoNodalExecute(TObject *Sender)
   {
   //exibe tela do Custo Nodal
   DLL_NewFormCustoNodal(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionDiagEqptoExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *Form;

   //exibe tela do diagnóstico da reder
   if ((Form = DLL_NewFormDiagEqpto(this, apl, SelecionaParentParaJanela())) != NULL)
      {
      Form->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionDiagFlowExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *Form;

   //exibe tela do editor
   if ((Form = DLL_NewFormDiagFlow(this, apl, SelecionaParentParaJanela())) != NULL)
      {
      Form->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionDistanciaEletricaExecute(TObject *Sender)
   {
   //exibe tela de distância elétrica
   DLL_NewFormDistancia(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionEstabilidadeExecute(TObject *Sender)
   {
   //exibe tela do módulo de Estabilidade
   DLL_NewFormEstabilidade(this, apl, SelecionaParentParaJanela());
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionExportaNIXExecute(TObject *Sender)
   {
   //variáveis locais
   VTInterfaceNIX *interface_nix;

   try{//cria objeto VTProtecao
      interface_nix = DLL_NewObjInterfaceNIX(apl);
      interface_nix->ExportaArqNix();
      delete interface_nix;
      }catch(Exception &e)
         {//nada a fazer
         }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionExportaNIXExecute(TObject *Sender)
   {
   //veriáveis locais
   TForm *form;

   //exibe TFormNIX
   form = DLL_NewFormNIX(this, apl, SelecionaParentParaJanela());
   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionFluxoHarmonicoExecute(TObject *Sender)
   {
   //exibe tela do editor
   DLL_NewFormFluxoH3(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionGoogleMapsExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;
   //exibe tela do editor
   //DLL_NewFormGoogle(this, apl, SelecionaParentParaJanela());
   //FKM 2015.09.14 Verifica se já existe o Form
   if((form = (TForm*) ExisteModuloAtivo("FormGoogle")) != NULL){form->Show();}
   else{DLL_NewFormGoogle(this, apl, SelecionaParentParaJanela());}
   //FKM 2015.09.14
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionMercadoExecute(TObject *Sender)
   {
   //exibe tela do Mercado
   DLL_NewFormMercado(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPartidaMotorExecute(TObject *Sender)
   {
   //exibe tela do módulo de PartidaMotor
   DLL_NewFormPartidaMotor(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPerdaRegulatoriaExecute(TObject *Sender)
   {
   //verifica se há módulos abertos que podem gerar conflitos
   if (ExisteModuloAtivoConflitante()) return;
   //exibe tela de Pertec
   DLL_NewFormPerdareg(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPerdaTecnicaExecute(TObject *Sender)
   {
   //verifica se há módulos abertos que podem gerar conflitos
   if (ExisteModuloAtivoConflitante()) return;
   //exibe tela de Pertec
   DLL_NewFormPerdatec(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPrintPreviewExecute(TObject *Sender)
	{
	/*
   //cria TFormPrintPreview como janela modal
   TFormPrintPreview* pp = new TFormPrintPreview(Application, apl);
   pp->PrintPreview->Refresh();
   pp->ShowModal();
   //destrói o Form
	delete pp;
	*/
	Aviso("Esta função não está disponível nesta versão");
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_ConfiabilidadeExecute(TObject *Sender)
   {
   //variáveis locais
   //int       ind_pat = 0;
   //int       ano     = 0;
   //VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   /*
   //proteção para execução do módulo de Confiabilidade
   if (! ValidaExecucaoConfiabilidade())
      {//seleciona gráfico de topologia
      ActionTopo->Execute();
      return;
      }
   */
   //fecha janelas
   //FechaJanelas();
   //esconde CBoxPatamar
   //CBoxPatamar->Visible = false;
   //abaixa Button
   //ButConfiabilidade->Down = true;
   //exibe tela de confiabilidade
   DLL_NewFormConfiabilidade(this, apl, SelecionaParentParaJanela());
   //exibe gráfico de Confiabilidade DEPOIS de ter aberto tela de Confiabilidade (que cria objeto RedeCFB)
   //grafico->Show(grafCONFIAB, ano, ind_pat);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_DebugExecute(TObject *Sender)
   {
   //exibe tela do Debug
   DLL_NewFormDebug(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_EquivalenteExecute(TObject *Sender)
   {
//   //veriáveis locais
//   TForm *form;
//
//   //exibe tela do módulo de Equivalentes
//   form = DLL_NewFormEquivalente(this, apl, SelecionaParentParaJanela());
//   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_EstimaEstadoExecute(TObject *Sender)
   {
   //veriáveis locais
   TForm *form;

   //exibe tela do Estimador
   form = DLL_NewFormEstimaEstado(this, apl, SelecionaParentParaJanela());
   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_EstimaDemandaExecute(TObject *Sender)
   {
   //veriáveis locais
   TForm *form;

   //exibe tela do Estimador
   form = DLL_NewFormEstimaDemanda(this, apl, SelecionaParentParaJanela());
   if (form) form->Show();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_ProcessoExecute(TObject *Sender)
   {
   //variáveis locais
   VTProcesso *processo;

   processo = DLL_NewObjProcesso(apl);
   processo->ActionOnTerminate = NULL;
   processo->Wait              = true;
   processo->Executa(NULL);
   delete processo;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_InterfaceODSSExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;
   //exibe tela do Exportador OpenDSS
   form = DLL_NewFormExportaODSS(this, apl);
   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_ProtecaoExecute(TObject *Sender)
   {
   //exibe tela de Proteção
   DLL_NewFormProtecao(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_SinapClientExecute(TObject *Sender)
   {
   //veriáveis locais
   TForm *form;
   //exibe tela do Servidor

   form = DLL_NewFormClient(this, apl, SelecionaParentParaJanela());
   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_SinapServerExecute(TObject *Sender)
   {
   //veriáveis locais
   TForm *form;
   //exibe tela do Servidor

   form = DLL_NewFormServer(this, apl, SelecionaParentParaJanela());
   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_SpinClientExecute(TObject *Sender)
   {
//   //veriáveis locais
//   TForm *form;
//   //exibe tela do Servidor
//
//   form = DLL_NewFormSpinClient(this, apl, SelecionaParentParaJanela());
//   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_SpinServerExecute(TObject *Sender)
   {
//   //veriáveis locais
//   TForm *form;
//   //exibe tela do Servidor
//
//   form = DLL_NewFormSpinServer(this, apl, SelecionaParentParaJanela());
//   if (form) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionPriv_TrocaEqvBTExecute(TObject *Sender)
   {
   //variáveis locais
   VTStkEqv *stk_eqv;

   try{//cria objeto VTStkEqv
      if ((stk_eqv = DLL_NewObjStkEqv(apl)) != NULL)
         {
         stk_eqv->Executa();
         //destrói objeto VTStkEqv
         delete stk_eqv;
         }
      }catch(Exception &e)
         {
         Aviso("Não foi possível substituir as redes BT equivalentes por cargas");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionRelCargaExecute(TObject *Sender)
   {
   //exibe tela de Relatórios
   DLL_NewFormRelCarga(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionRelEqptoExecute(TObject *Sender)
   {
   //exibe tela de Relatórios
   DLL_NewFormRelEqpto(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionRelFluxoExecute(TObject *Sender)
   {
   //exibe tela de Relatórios
   DLL_NewFormRelFluxo(this, apl,  SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::ActionZoomExecute(TObject *Sender)
   {
   //exibe tela do editor
   DLL_NewFormZoom(this, apl, SelecionaParentParaJanela());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::Atualiza(void)
   {
   //DLL_AtualizaFormDiagFlow(this);
   //DLL_AtualizaFormDiagEqpto(this);
   DLL_AtualizaFormProtecao(this);
   DLL_AtualizaRelatorio(this);
   //DLL_AtualizaFormPertec(this, num_pat);
   }

//---------------------------------------------------------------------------
//static TForm* __fastcall TFormEspecial::ExisteRelatorio(TForm *Owner, AnsiString form_name)
//   {
//   //variáveis locais
//   AnsiString class_name;
//   TForm      *form;
//
//   //verifica se existe um FormRelatorio aberto
//   for (int n = 0; n < Screen->FormCount; n++)
//      {//verifica se o Form n é um TFormRelatorio
//      class_name = Screen->Forms[n]->ClassName();
//      if (class_name.AnsiCompareIC(form_name) == 0)
//         {
//         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
//         }
//      }
//   return(NULL);
//   }
//---------------------------------------------------------------------------
// FKM 2015.09.14 - Verifica se existe um modulo ativo usando o nomed o TComponent
TComponent*  __fastcall TFormEspecial::ExisteModuloAtivo(AnsiString nomeComponent)
{
   TComponent  *owned;
   TForm       *form;
   AnsiString  nome;

   //Verifica a lista dos TComponents "owned" por esse
   for (int i = 0; i < this->ComponentCount; i++)
      {
      owned = this->Components[i];
      nome = owned->Name;
      if (nome.AnsiCompareIC(nomeComponent) == 0 ) {return owned;}
      }
   return NULL;
}

//---------------------------------------------------------------------------
bool __fastcall TFormEspecial::ExisteModuloAtivoConflitante(void)
   {
   // variáveis locais
   TForm *form;

   //procura Form de módulos ativoa que podem ter conflitos
   form                   = DLL_ExisteFormAlocaCapacitor(this);
   if (form == NULL) form = DLL_ExisteFormAlocaRegulador(this);
   if (form == NULL) form = DLL_ExisteFormBalanceamento(this);
   if (form == NULL) form = DLL_ExisteFormDemanda(this);
   if (form == NULL) form = ExisteForm("TFormPerdatec", this);
   if (form == NULL) form = ExisteForm("TFormPerdareg", this);
   //verifica se foi encontrado um form
   if (form != NULL)
      {
      Aviso("Feche o módulo " + AnsiQuotedStr(form->Caption, '\''));
      return(true);
      }
	return(false);
   }

//---------------------------------------------------------------------------
TWinControl* __fastcall TFormEspecial::SelecionaParentParaJanela(void)
   {
   //variáveis locais
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

   return(MultiPanel->PanelAtivo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEspecial::VerificaModulos(void)
   {
   //verifica se usuário master
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //habilita/desabilita Actions de módulos especiais
   ActionAjusteDemanda->Enabled     = DLL_AjusteDemanda_Enabled();
   ActionAlocaCapacitor->Visible    = true;
   ActionAlocaRegulador->Visible    = true;
	ActionAnaliseMalhas->Visible     = true;
   ActionBalanceamento->Visible     = true;
   ActionDiagEqpto->Visible         = true;
   ActionDiagFlow->Visible          = true;
   ActionDistanciaEletrica->Enabled = true;
   ActionEstabilidade->Enabled      = DLL_Estabilidade_Enabled();
   ActionExportaNIX->Visible        = true;
   ActionMercado->Visible           = true;
   ActionPartidaMotor->Enabled      = false; //DLL_PartidaMotor_Enabled();
	//habilita/desabilita Actions em função da Licença
   ActionGoogleMaps->Enabled        = licenca->Google;
   //Actions exclusivas de usuários Master
   ActionPrintPreview->Visible       = (licenca->Tipo == tipoMASTER);
   ActionCustoNodal->Visible         = (licenca->Tipo == tipoMASTER);
   ActionPartidaMotor->Visible       = (licenca->Tipo == tipoMASTER);
   ActionEstabilidade->Visible       = (licenca->Tipo == tipoMASTER);
   ActionFluxoHarmonico->Visible     = (licenca->Tipo == tipoMASTER);
   ActionPriv_Debug->Visible         = (licenca->Tipo == tipoMASTER);
   ActionPriv_Equivalente->Visible   = (licenca->Tipo == tipoMASTER);
   ActionPriv_EstimaDemanda->Visible = (licenca->Tipo == tipoMASTER);
   ActionPriv_EstimaEstado->Visible  = (licenca->Tipo == tipoMASTER);
   ActionPriv_Protecao->Visible      = (licenca->Tipo == tipoMASTER);
   ActionPriv_SinapClient->Visible   = (licenca->Tipo == tipoMASTER);
   ActionPriv_SinapServer->Visible   = (licenca->Tipo == tipoMASTER);
   ActionPriv_InterfaceODSS->Visible = (licenca->Tipo == tipoMASTER);
   //ActionPriv_Processo->Visible      = (licenca->Tipo == tipoMASTER);
   }

//---------------------------------------------------------------------------
//eof


