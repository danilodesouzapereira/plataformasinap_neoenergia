// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormPerdareg.h"
#include "TFormCfgPerdaTec.h"
#include "TFormMedicao.h"
#include "TFormPerda.h"
#include "TFormSalva.h"
#include "TAplPerdareg.h"
#include "VTPerdareg.h"
#include "AjustaEnergia\VTAjustaEnergia.h"
#include "Medicao\TFormMedicao.h"
#include "Medicao\TMedicao.h"
#include "Medicao\VTMedicoes.h"
#include "PerdaComercial\TFormPerdaComercial.h"
#include "EnergiaPassante\TFormEnergiaPassante.h"
#include "RedePT\VTMontaRedePT.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BDihm\VTSalva.h"
#include "..\Copia\VTCopia.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\BDihm.h"
#include "..\..\DLL_Inc\Copia.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14187786
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
__fastcall TFormPerdareg::TFormPerdareg(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl_owner->GetObject(__classid(VTGeral));
	VTProgresso *prog = (VTProgresso*)apl_owner->GetObject(__classid(VTProgresso));
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));

	this->mApl_owner = apl_owner;
	if (prog == NULL)
	{
		apl_owner->Add(prog = DLL_NewObjProgresso(NULL, NULL));
	}
	if (prog)
	{
		prog->Start(progDEFAULT);
	}
	// desabilita editor
//	editor_enabled = true;
//	if (sinap_child)
//	{
//		editor_enabled = sinap_child->ActionEditor->Enabled;
//		sinap_child->ActionEditor->Enabled = false;
//	}
	if (sinap_child)
	{
		sinap_child->BeginEditorBlock(this);
	}
	// configura o Form
	BorderStyle = bsNone;
	Height = Panel->Height;
	// insere Form em seu Parent
	if (parent)
	{
		Parent = parent;
		Align = alTop;
	}
	// cria objeto AplPertec
	apl = new TAplPerdareg(this, apl_owner);
	// inclui objetos VTBD e VTBSalva
	apl->Add(DLL_NewObjBD(apl));
	apl->Add(DLL_NewObjSalva(this, apl));
	// cria TFormPerda e TFormPerdaComercial sem exibi-los
	FormPerda = new TFormPerda(this, apl, parent);
	FormPerdaComercial = new TFormPerdaComercial(this, apl, parent);
    FormEnergiaPassante = new TFormEnergiaPassante(this, apl, parent);
	// cria novo cfgperdatec para compara��o
	// CfgPerdaTec_anterior = NewObjCfgPerdaTec(apl);
	strCfgPerdaTec_anterior.primeira_valida = false;
	// redefine t�tulo do relat�rio de perda
	FormPerda->Caption = "Perda T�cnica Regulat�ria";
	// cria objeto VTRedes local
	redes_local = DLL_NewObjRedes(apl);
	CopiaRedes1ParaRedes2((VTRedes*)apl->GetObject(__classid(VTRedes)), redes_local);
	// marca a rede como original
	redeAlterada = false;
	// indica que os par�metros para c�lculo de perdas foram alterados
	geral->CfgPerdaTec->Alterada = true;
	// inicia data de refer�ncia com a data da rede
	DateTimePickerInicia();
	if (prog)
	{
		prog->Stop();
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14187786
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
__fastcall TFormPerdareg::~TFormPerdareg(void)
{
	// vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRedes *redes_graf;
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	VTFlow *flowOwner;

	if (redeAlterada)
	{ // restaura objeto VTRedes
		CopiaRedes1ParaRedes2(redes_local, (VTRedes*)apl->GetObject(__classid(VTRedes)));
		redes_graf = (VTRedes*)grafico->Apl->GetObject(__classid(VTRedes));
		grafico->Apl->Remove(redes_graf);
		grafico->Apl->Add(redes);
		grafico->AreaRedeDefine();
		grafico->Refresh();
		// atualiza flow //DVK 2016.07.19
		flow->UpToDate = false;
		if (mApl_owner)
		{
			flowOwner = (VTFlow*)mApl_owner->GetObject(__classid(VTFlow));
			if (flowOwner)
			{
				flowOwner->UpToDate = false;
			}
		}
	}
	// muda apresenta��o p/ topologia da Rede
	if (sinap_child)
		sinap_child->ActionTopo->Execute();
	// restaura editor
//	if (sinap_child)
//		sinap_child->ActionEditor->Enabled = editor_enabled;
    if (sinap_child)
	{
		sinap_child->EndEditorBlock(this);
	}
	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destr�i TFormPerda e TFormPerdaComercial
	if (FormPerda)
	{
		delete FormPerda;
		FormPerda = NULL;
	}
	if (FormPerdaComercial)
	{
		delete FormPerdaComercial;
		FormPerdaComercial = NULL;
	}
	if (FormEnergiaPassante)
	{
		delete FormEnergiaPassante;
		FormEnergiaPassante = NULL;
	}
	// destr�i objetos
	if (redes_local)
	{
		delete redes_local;
		redes_local = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// if (CfgPerdaTec_anterior) {delete CfgPerdaTec_anterior; CfgPerdaTec_anterior = NULL;}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionCfgMedicaoExecute(TObject *Sender)
{
	// verifica se a data est� selecionada
	if (!DataSelecionada())
		return;
	// exibe tela para configura par�metros
	ConfiguraMedicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionCfgParametroExecute(TObject *Sender)
{
	// exibe tela para configura par�metros
	ConfiguraParametros();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionFecharExecute(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionAlteraUnidadeExecute(TObject *Sender)
{
	/*
	 //vari�veis locais
	 AnsiString unidade;
	 double     fat_mult;
	 int        num_dias = DaysInMonth(DateTimePicker->Date);

	 if (MenuItem_kwh->Checked)
	 {
	 unidade  = MenuItem_kwh->Caption;
	 fat_mult = 1000.;
	 }
	 else if (MenuItem_kwh_30dias->Checked)
	 {
	 unidade  = MenuItem_kwh_30dias->Caption;
	 fat_mult = 30 * 1000.;
	 }
	 else if (MenuItem_kwh_diasmes->Checked)
	 {
	 unidade  = MenuItem_kwh_diasmes->Caption;
	 fat_mult = num_dias * 1000.;
	 }
	 else if (MenuItem_mwh->Checked)
	 {
	 unidade  = MenuItem_mwh->Caption;
	 fat_mult = 1.;
	 }
	 else if (MenuItem_mwh_30dias->Checked)
	 {
	 unidade  = MenuItem_mwh_30dias->Caption;
	 fat_mult = 30.;
	 }
	 else if (MenuItem_mwh_diasmes->Checked)
	 {
	 unidade  = MenuItem_mwh_diasmes->Caption;
	 fat_mult = num_dias;
	 }
	 else
	 {
	 Aviso("Defina a unidade para as perdas");
	 return;
	 }
	 //atualiza FormPerda
	 FormPerda->DefineUnidade(unidade, fat_mult);
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionCalculaPerdasExecute(TObject *Sender)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTPerdareg *perdareg = (VTPerdareg*)apl->GetObject(__classid(VTPerdareg));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se a data foi realmente selecionada
	if (!DataSelecionada())
		return;
	// verifica se h� medi��o com perda negativa
	if (!ValidaMedicao())
		return;
	// verifica se nada foi alterado
	if ((!geral->CfgPerdaTec->Alterada) && (DateOf(DateTimePicker->Date) == last_date) &&
		(!medicoes->Alterado))
	{ // n�o � necess�rio recalcular: exibe relat�rio de perda t�cnica
		ActionRelPerdaTecnica->Execute();
		return;
	}
	// salva data atualmente selecionada
	// last_date = DateOf(DateTimePicker->Date);
	// verifica se os par�metros foram alterados
	// if ((geral->CfgPerdaTec->Alterada) || (medicoes->Alterado))
	if (VerificaCfgPerdasAlterado() || (medicoes->Alterado) ||
		(DateOf(DateTimePicker->Date) != last_date))
	{ // reinicia o perdas reg
		ReiniciaFormPerdas();
		// //reinicia objeto Perdareg
		// perdareg->ClearMontaRede();
		// //atualiza o objeto VTRedes
		// CopiaRedes1ParaRedes2(redes_local, redes);
		// //atualiza o grafico
		// grafico->AreaRedeDefine();
		// grafico->Refresh();
		// //reinicia zonas
		// zonas->Executa(redes);
		// //reinicia medi��es
		// ReiniciaMedicao();
	}
	// salva data atualmente selecionada
	last_date = DateOf(DateTimePicker->Date);
	// calcula perdas
	if (CalculaPerdasRegulatorias())
	{ // habilita bot�es
		FormPerdasCalculado(true);
		// exibe relat�rio de perda t�cnica
		ActionRelPerdaTecnica->Execute();
	}
	else
	{ // desabilita bot�es
		FormPerdasCalculado(false);
		FormPerda->Hide();
	}
	// a rede j� n�o � mais a original
	redeAlterada = true;
	// atualiza o grafico
	grafico->AreaRedeDefine();
	grafico->Refresh();
	// sinaliza que n�o houve altera��o nas medi��es //DVK 2016.07.28
	medicoes->Alterado = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Perda_Regulat�ria");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionRelPerdaComercialExecute(TObject *Sender)
{
	// exibe FormPerdaComercial
	FormPerdaComercial->Inicia();
	FormPerdaComercial->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionRelPerdaTecnicaExecute(TObject *Sender)
{
	// exibe FormPerda
	FormPerda->Inicia(DateTimePicker->Date);
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
void __fastcall TFormPerdareg::ActionRelEnergiaPassanteExecute(TObject *Sender)
{
    // exibe FormPerdaComercial
	FormEnergiaPassante->MontaRelatorioEnergiaPassante(DateTimePicker->Date);
	FormEnergiaPassante->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ActionSalvaBaseExecute(TObject *Sender)
{
	// vari�veis locais
	TFormSalva *FormSalva;

	try
	{ // cria TFormSalva e exibe como janela modal
		FormSalva = new TFormSalva(this, apl);
		FormSalva->Caption = FormSalva->Caption + " Regulat�ria";
		FormSalva->ShowModal();
		delete FormSalva;
	}
	catch (Exception &e)
	{ // nada a fazer
		Aviso("N�o foi poss�vel atualizar a base de perdas");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::AtualizaCaptions(void)
{
	/*
	 //vari�veis locais
	 AnsiString dias_mes;
	 int        num_dias = DaysInMonth(DateTimePicker->Date);

	 dias_mes.sprintf("/m�s(%d dias)", num_dias);
	 MenuItem_kwh_diasmes->Caption = dias_mes.sprintf("kWh/m�s(%d dias)", num_dias);
	 MenuItem_mwh_diasmes->Caption = dias_mes.sprintf("MWh/m�s(%d dias)", num_dias);
	 */
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::CalculaPerdasRegulatorias(void)
{
	// vari�veis locais
	bool sucesso;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTPerdareg *perdareg = (VTPerdareg*)apl->GetObject(__classid(VTPerdareg));

	// prote��o: verifica se h� Medicoes definidas
	if (medicoes->LisMedicao()->Count == 0)
	{
		if (Confirma("N�o h� medi��es definidas", "Deseja continuar ?") != IDYES)
		{
			return (false);
		}
	}
	// calcula perdas
	sucesso = perdareg->Executa(DateTimePicker->Date);
	if (!sucesso)
	{
		Aviso("Erro ao calcular perdas regulat�rias");
	}
	// atualiza e exibe FormPerda
	/*
	 //calcula valores agregados de todos Resumos
	 FormPerda->CalculaAgregadaTotal();
	 FormPerda->Show();
	 //inicia TView (ap�s montar a Arvore com Resumos das Redes)
	 FormPerda->TViewInicia();
	 //simula altera��o da unidade
	 ActionAlteraUnidade->Execute();
	 */
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ConfiguraMedicao(void)
{
	// vari�veis locais
	TFormMedicao *FormMedicao;

	try
	{
		// cria TFormCfgMedicao p/ configurar par�metros de c�lculo de perdas t�cnicas
		FormMedicao = new TFormMedicao(this, apl);
		FormMedicao->Date = DateTimePicker->Date;
		FormMedicao->ShowModal();
		delete FormMedicao;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::ConfiguraParametros(void)
{
	// vari�veis locais
	bool sucesso = false;
	TFormCfgPerdatec *FormCfgPerdatec;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	try
	{ // cria FormCfgPerdatec p/ configurar par�metros de c�lculo de perdas t�cnicas
		FormCfgPerdatec = new TFormCfgPerdatec(this, apl);
		// copia configuracoes atuais
		CopiaCfgPerdas();
		if (FormCfgPerdatec->ShowModal() == mrOk)
		{ // desabilita bot�es
			FormPerdasCalculado(false);
			// esconde relat�rios
			FormPerda->Hide();
			// atualiza o gr�fico pois a rede pode ter sido alterada
			grafico->AreaRedeDefine();
			sucesso = true;
			// FKM - 2017.04.04
			// caso se seja a 1a vez que altera a config, o cfgperdas_anterior sera
			// o n�o valido, ent�o, para n�o copiar novamente a rede sem uma altera��o
			// real nas configuracoes, copio a 1a modificao e a considero como o anterior
			// Isso � necess�rio devido a nova verifica��o no ActionCalculaPerdasExecute
			// if(! CfgPerdaTec_anterior->Alterada)
			if (!strCfgPerdaTec_anterior.primeira_valida)
			{
				CopiaCfgPerdas();
				strCfgPerdaTec_anterior.primeira_valida = true;
			}
		}
		delete FormCfgPerdatec;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::CopiaCfgPerdas(void)
{ // vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTCfgPerdaTec *CfgPerdaTec = geral->CfgPerdaTec;

	// //protecao
	// if(CfgPerdaTec_anterior)
	// {
	// CfgPerdaTec_anterior->Alterada = CfgPerdaTec->Alterada;
	// CfgPerdaTec_anterior->RedeComNeutro = CfgPerdaTec->RedeComNeutro;
	// CfgPerdaTec_anterior->RedeComCapacitor = CfgPerdaTec->RedeComCapacitor;
	// CfgPerdaTec_anterior->Zat_mrt = CfgPerdaTec->Zat_mrt;
	// CfgPerdaTec_anterior->Zat_ohm = CfgPerdaTec->Zat_ohm;
	// CfgPerdaTec_anterior->TipoFluxo = CfgPerdaTec->TipoFluxo;
	// CfgPerdaTec_anterior->ModeloCarga = CfgPerdaTec->ModeloCarga;
	// CfgPerdaTec_anterior->FerroPorFora = CfgPerdaTec->FerroPorFora;
	// CfgPerdaTec_anterior->MedidorPorFora = CfgPerdaTec->MedidorPorFora;
	// CfgPerdaTec_anterior->RamalPorFora = CfgPerdaTec->RamalPorFora;
	// CfgPerdaTec_anterior->OpcaoPerdaMedidor = CfgPerdaTec->OpcaoPerdaMedidor;
	// CfgPerdaTec_anterior->PerdaMedidorUnico = CfgPerdaTec->PerdaMedidorUnico;
	// CfgPerdaTec_anterior->OpcaoPerdaRamal = CfgPerdaTec->OpcaoPerdaRamal;
	// CfgPerdaTec_anterior->PerdaRamalUnico = CfgPerdaTec->PerdaRamalUnico;
	// CfgPerdaTec_anterior->OpcaoPerdaTrafo = CfgPerdaTec->OpcaoPerdaTrafo;
	// CfgPerdaTec_anterior->PerdaTrafoUnico = CfgPerdaTec->PerdaTrafoUnico;
	// CfgPerdaTec_anterior->OpcaoPerdaTrafoSED = CfgPerdaTec->OpcaoPerdaTrafoSED;
	// CfgPerdaTec_anterior->PerdaAdicional_perc = CfgPerdaTec->PerdaAdicional_perc;
	// }
	// strCfgPerdaTec_anterior.alterada = CfgPerdaTec->Alterada;
	strCfgPerdaTec_anterior.fluxo.com_neutro = CfgPerdaTec->RedeComNeutro;
	strCfgPerdaTec_anterior.fluxo.com_capacitor = CfgPerdaTec->RedeComCapacitor;
	strCfgPerdaTec_anterior.fluxo.zat_mrt = CfgPerdaTec->Zat_mrt;
	strCfgPerdaTec_anterior.fluxo.zat_ohm = CfgPerdaTec->Zat_ohm;
	strCfgPerdaTec_anterior.fluxo.tipo = CfgPerdaTec->TipoFluxo;
	strCfgPerdaTec_anterior.fluxo.modelo_carga = CfgPerdaTec->ModeloCarga;
	strCfgPerdaTec_anterior.PorFora.ferro = CfgPerdaTec->FerroPorFora;
	strCfgPerdaTec_anterior.PorFora.medidor = CfgPerdaTec->MedidorPorFora;
	strCfgPerdaTec_anterior.PorFora.ramal = CfgPerdaTec->RamalPorFora;
	strCfgPerdaTec_anterior.opcao.perda_medidor = CfgPerdaTec->OpcaoPerdaMedidor;
	strCfgPerdaTec_anterior.unico.perda_medidor = CfgPerdaTec->PerdaMedidorUnico;
	strCfgPerdaTec_anterior.opcao.perda_ramal = CfgPerdaTec->OpcaoPerdaRamal;
	strCfgPerdaTec_anterior.unico.perda_ramal = CfgPerdaTec->PerdaRamalUnico;
	strCfgPerdaTec_anterior.opcao.perda_trafo = CfgPerdaTec->OpcaoPerdaTrafo;
	strCfgPerdaTec_anterior.unico.perda_trafo = CfgPerdaTec->PerdaTrafoUnico;
	strCfgPerdaTec_anterior.opcao.perda_trafo_sed = CfgPerdaTec->OpcaoPerdaTrafoSED;
	strCfgPerdaTec_anterior.perda.adicional_perc = CfgPerdaTec->PerdaAdicional_perc;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2)
{
	// vari�veis locais
	bool sucesso;
	VTCopia *copia;

	try
	{ // prote��o
		copia = DLL_NewObjCopia(apl);
		sucesso = copia->CopiaRedes1ParaRedes2(redes1, redes2);
		delete copia;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::DataSelecionada(void)
{
	// verifica se usu�rio j� efetuou a sele��o de uma data
	if (DateTimePicker->Checked)
		return (true);

	Aviso("� necess�rio confirmar a data selecionada");
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::DateTimePickerChange(TObject *Sender)
{
	// teste
	if (DateTimePicker->Checked)
	{ // caso j� tenha sido calculado perdas, reinicia o perdas
		if (redeAlterada)
		{
			ReiniciaFormPerdas();
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::DateTimePickerCloseUp(TObject *Sender)
{
	// vari�veis locais
	VTAjustaEnergia *ajusta = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));

	// verifica se houve altera��o da data selecionada
	if (DateOf(DateTimePicker->Date) != date)
	{ // inicia dados das Medicoes que dependem da data (dias do m�s) e da rede
		ajusta->IniciaAllMedicao(DaysInMonth(DateTimePicker->Date));
		// esconde FormPerda
		FormPerda->Hide();
		// desabilita bot�o de relat�rio
		ButRelatorio->Enabled = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::DateTimePickerDropDown(TObject *Sender)
{
	// salva data atualmente selecionada
	date = DateOf(DateTimePicker->Date);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::DateTimePickerInicia(void)
{
	// vari�veis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	DateTimePicker->DateTime = redes->DataReferencia;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destr�i o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::FormShow(TObject *Sender)
{
	// aviso
	if (VerificaRedesCarregadas())
	{
		AnsiString msgErro;

		msgErro = "Existem redes secund�rias n�o carregadas, o c�lculo n�o ";
		msgErro += "poder� ser feito corretamente.\nO m�dulo ser� fechado.";

		Aviso(msgErro);
		this->Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::FormPerdasCalculado(bool calculado)
{
	if (calculado)
	{ // habilita bot�es
		ButRelatorio->Enabled = true;
		ButSalvaBase->Enabled = true;
		// deseleciona a data, sendo necessario que o usuario comece o processo novamente
		// caso queira calcular perdas denovo.
		DateTimePicker->Checked = false;
		ButCalcula->Enabled = false;
		ButParametros->Enabled = false;
		ButMedicoes->Enabled = false;
	}
	else
	{ // desabilita bot�es
		ButRelatorio->Enabled = false;
		ButSalvaBase->Enabled = false;
		// habilita
		ButCalcula->Enabled = true;
		ButParametros->Enabled = true;
		ButMedicoes->Enabled = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ReiniciaFormPerdas(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPerdareg *perdareg = (VTPerdareg*)apl->GetObject(__classid(VTPerdareg));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));

	// reinicia os enabled dos botoes
	FormPerdasCalculado(false);
	// reinicia objeto Perdareg
	perdareg->ClearMontaRede();
	// atualiza o objeto VTRedes
	CopiaRedes1ParaRedes2(redes_local, redes);
	// atualiza o grafico
	grafico->AreaRedeDefine();
	grafico->Refresh();
	// reinicia zonas
	zonas->Executa(redes);
	// reinicia medi��es
	ReiniciaMedicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdareg::ReiniciaMedicao(void)
{
	// vari�veis locais
	VTAjustaEnergia *ajusta = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TMedicao *medicao;
	TList *lisMEDICOES = medicoes->LisMedicao();

	for (int nm = 0; nm < lisMEDICOES->Count; nm++)
	{ // limpa as listas de cargas e ponteiro para rede
		medicao = (TMedicao*)lisMEDICOES->Items[nm];
		medicao->Clear();
		// atualiza ponteiro da rede
		medicao->rede = redes->ExisteRede(medicao->cod_rede);
	}
	// inicia dados das Medicoes que dependem da data (dias do m�s) e da rede
	ajusta->IniciaAllMedicao(DaysInMonth(DateTimePicker->Date));
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::ValidaMedicao(void)
{
	// vari�veis locais
	TMedicao *medicao;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	// verifica se h� Medicao com perda negativa
	for (int n = 0; n < lisMEDICAO->Count; n++)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		if (IsDoubleZero(medicao->energia.medida_mwhmes))
			continue;
		if (medicao->PerdaTotal_mwhmes() <= 0)
		{
			Aviso(medicao->rede->Codigo + ": energia medida incompat�vel com a energia das cargas");
			return (false);
		}
	}
	return (true);
}

////---------------------------------------------------------------------------
// bool __fastcall TFormPerdareg::VerificaCfgPerdasAlterado(void)
// {//variaveis locais
// bool alterado = false;
// VTGeral  *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
// VTCfgPerdaTec *CfgPerdaTec = geral->CfgPerdaTec;
//
// //op��o de fluxo: rede com ou sem neutro
// if(CfgPerdaTec->RedeComNeutro != CfgPerdaTec_anterior->RedeComNeutro)
// {return true;}
// //op��o de fluxo: rede com capacitor
// if(CfgPerdaTec->RedeComCapacitor != CfgPerdaTec_anterior->RedeComCapacitor)
// {return true;}
// //op��o de fluxo: Zat de redes MRT
// if(CfgPerdaTec->Zat_mrt != CfgPerdaTec_anterior->Zat_mrt)
// {return true;}
// if (CfgPerdaTec->Zat_mrt)
// {
// if (! IsDoubleZero(CfgPerdaTec->Zat_ohm - CfgPerdaTec_anterior->Zat_ohm))
// {return true;}
// }
// //op��o de fluxo: rede equilibrada/desequilibrada
// if(CfgPerdaTec->TipoFluxo != CfgPerdaTec_anterior->TipoFluxo)
// {return true;}
// //op��o de fluxo: modelo de carga
// if(CfgPerdaTec->ModeloCarga != CfgPerdaTec_anterior->ModeloCarga)
// {return true;}
// //perda por fora
// if(CfgPerdaTec->FerroPorFora != CfgPerdaTec_anterior->FerroPorFora)
// {return true;}
// if(CfgPerdaTec->MedidorPorFora != CfgPerdaTec_anterior->MedidorPorFora)
// {return true;}
// if(CfgPerdaTec->RamalPorFora != CfgPerdaTec_anterior->RamalPorFora)
// {return true;}
// //perdas em medidor
// if(CfgPerdaTec->OpcaoPerdaMedidor != CfgPerdaTec_anterior->OpcaoPerdaMedidor)
// {return true;}
// else
// {//caso sejam iguais..
// if(CfgPerdaTec->OpcaoPerdaMedidor == opMEDIDOR_UNICO)
// {
// if((CfgPerdaTec->PerdaMedidorUnico != NULL) &&
// (CfgPerdaTec_anterior->PerdaMedidorUnico != NULL))
// {//verifca se s�o iguais
// PERDA_MEDIDOR *medidor1, *medidor2;
//
// medidor1 = CfgPerdaTec->PerdaMedidorUnico;
// medidor2 = CfgPerdaTec_anterior->PerdaMedidorUnico;
// //verifica se tem valores iguais
// if(! IsDoubleZero(medidor1->perda_w - medidor2->perda_w))
// {return true;}
// //compara nomes
// if(medidor1->codigo.AnsiCompare(medidor2->codigo) != 0)
// {return true;}
// }
// else
// {
// return true;
// }
// }
// }
// //perdas em ramal de liga��o
// if(CfgPerdaTec->OpcaoPerdaRamal != CfgPerdaTec_anterior->OpcaoPerdaRamal)
// {return true;}
// else
// {
// //caso sejam iguais..
// if((CfgPerdaTec->PerdaRamalUnico != NULL) &&
// (CfgPerdaTec_anterior->PerdaRamalUnico != NULL))
// {//verifca se s�o iguais
// PERDA_RAMAL *ramal1, *ramal2;
//
// ramal1 = CfgPerdaTec->PerdaRamalUnico;
// ramal2 = CfgPerdaTec_anterior->PerdaRamalUnico;
//
// if(ramal1->num_fases != ramal2->num_fases)
// {return true;}
// if(ramal1->cabo != ramal2->cabo)
// {return true;}
// //verifica se tem valores iguais
// if(! IsDoubleZero(ramal1->vmin_kv - ramal2->vmin_kv))
// {return true;}
// if(! IsDoubleZero(ramal1->vmax_kv - ramal2->vmax_kv))
// {return true;}
// if(! IsDoubleZero(ramal1->comp_m - ramal2->comp_m))
// {return true;}
// if(! IsDoubleZero(ramal1->r_ohm_km - ramal2->r_ohm_km))
// {return true;}
// }
// else
// {return true;}
// }
// //perdas em transformador SDBT
// if(CfgPerdaTec->OpcaoPerdaTrafo != CfgPerdaTec_anterior->OpcaoPerdaTrafo)
// {return true;}
// else
// {
// //caso sejam iguais..
// if((CfgPerdaTec->PerdaTrafoUnico != NULL) &&
// (CfgPerdaTec_anterior->PerdaTrafoUnico != NULL))
// {//verifca se s�o iguais
// PERDA_TRAFO *trafo1, *trafo2;
//
// trafo1 = CfgPerdaTec->PerdaTrafoUnico;
// trafo2 = CfgPerdaTec_anterior->PerdaTrafoUnico;
//
// if(trafo1->tipo != trafo2->tipo)
// {return true;}
// //verifica se tem valores iguais
// if(! IsDoubleZero(trafo1->vmin_kv - trafo2->vmin_kv))
// {return true;}
// if(! IsDoubleZero(trafo1->vmax_kv - trafo2->vmax_kv))
// {return true;}
// if(! IsDoubleZero(trafo1->snom_kva - trafo2->snom_kva))
// {return true;}
// if(! IsDoubleZero(trafo1->perda_vazio_w - trafo2->perda_vazio_w))
// {return true;}
// if(! IsDoubleZero(trafo1->perda_total_w - trafo2->perda_total_w))
// {return true;}
// }
// else
// {return true;}
// }
// //perdas em transformador SED
// if(CfgPerdaTec->OpcaoPerdaTrafoSED != CfgPerdaTec_anterior->OpcaoPerdaTrafoSED)
// {return true;}
// if(! IsDoubleZero(CfgPerdaTec->PerdaAdicional_perc - CfgPerdaTec_anterior->PerdaAdicional_perc))
// {return true;}
//
// return alterado;
// }
// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::VerificaCfgPerdasAlterado(void)
{ // variaveis locais
	bool alterado = false;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTCfgPerdaTec *CfgPerdaTec = geral->CfgPerdaTec;

	// op��o de fluxo: rede com ou sem neutro
	if (CfgPerdaTec->RedeComNeutro != strCfgPerdaTec_anterior.fluxo.com_neutro)
	{
		return true;
	}
	// op��o de fluxo: rede com capacitor
	if (CfgPerdaTec->RedeComCapacitor != strCfgPerdaTec_anterior.fluxo.com_capacitor)
	{
		return true;
	}
	// op��o de fluxo: Zat de redes MRT
	if (CfgPerdaTec->Zat_mrt != strCfgPerdaTec_anterior.fluxo.zat_mrt)
	{
		return true;
	}
	if (CfgPerdaTec->Zat_mrt)
	{
		if (!IsDoubleZero(CfgPerdaTec->Zat_ohm - strCfgPerdaTec_anterior.fluxo.zat_ohm))
		{
			return true;
		}
	}
	// op��o de fluxo: rede equilibrada/desequilibrada
	if (CfgPerdaTec->TipoFluxo != strCfgPerdaTec_anterior.fluxo.tipo)
	{
		return true;
	}
	// op��o de fluxo: modelo de carga
	if (CfgPerdaTec->ModeloCarga != strCfgPerdaTec_anterior.fluxo.modelo_carga)
	{
		return true;
	}
	// perda por fora
	if (CfgPerdaTec->FerroPorFora != strCfgPerdaTec_anterior.PorFora.ferro)
	{
		return true;
	}
	if (CfgPerdaTec->MedidorPorFora != strCfgPerdaTec_anterior.PorFora.medidor)
	{
		return true;
	}
	if (CfgPerdaTec->RamalPorFora != strCfgPerdaTec_anterior.PorFora.ramal)
	{
		return true;
	}
	// perdas em medidor
	if (CfgPerdaTec->OpcaoPerdaMedidor != strCfgPerdaTec_anterior.opcao.perda_medidor)
	{
		return true;
	}
	else
	{ // caso sejam iguais..
		if (CfgPerdaTec->OpcaoPerdaMedidor == opMEDIDOR_UNICO)
		{
			if ((CfgPerdaTec->PerdaMedidorUnico != NULL) &&
				(strCfgPerdaTec_anterior.unico.perda_medidor != NULL))
			{ // verifca se s�o iguais
				PERDA_MEDIDOR *medidor1, *medidor2;

				medidor1 = CfgPerdaTec->PerdaMedidorUnico;
				medidor2 = strCfgPerdaTec_anterior.unico.perda_medidor;
				// verifica se tem valores iguais
				if (!IsDoubleZero(medidor1->perda_w - medidor2->perda_w))
				{
					return true;
				}
				// compara nomes
				if (medidor1->codigo.AnsiCompare(medidor2->codigo) != 0)
				{
					return true;
				}
			}
			else if ((CfgPerdaTec->PerdaMedidorUnico != NULL) &&
				(strCfgPerdaTec_anterior.unico.perda_medidor != NULL))
			{
				// nada a fazer
			}
			else
			{
				return true;
			}
		}
	}
	// perdas em ramal de liga��o
	if (CfgPerdaTec->OpcaoPerdaRamal != strCfgPerdaTec_anterior.opcao.perda_ramal)
	{
		return true;
	}
	else
	{
		// caso sejam iguais..
		if ((CfgPerdaTec->PerdaRamalUnico != NULL) &&
			(strCfgPerdaTec_anterior.unico.perda_ramal != NULL))
		{ // verifca se s�o iguais
			PERDA_RAMAL *ramal1, *ramal2;

			ramal1 = CfgPerdaTec->PerdaRamalUnico;
			ramal2 = strCfgPerdaTec_anterior.unico.perda_ramal;

			if (ramal1->num_fases != ramal2->num_fases)
			{
				return true;
			}
			if (ramal1->cabo != ramal2->cabo)
			{
				return true;
			}
			// verifica se tem valores iguais
			if (!IsDoubleZero(ramal1->vmin_kv - ramal2->vmin_kv))
			{
				return true;
			}
			if (!IsDoubleZero(ramal1->vmax_kv - ramal2->vmax_kv))
			{
				return true;
			}
			if (!IsDoubleZero(ramal1->comp_m - ramal2->comp_m))
			{
				return true;
			}
			if (!IsDoubleZero(ramal1->r_ohm_km - ramal2->r_ohm_km))
			{
				return true;
			}
		}
		else if ((CfgPerdaTec->PerdaRamalUnico == NULL) &&
			(strCfgPerdaTec_anterior.unico.perda_ramal == NULL))
		{
			// nada a fazer
		}
		else
		{
			return true;
		}
	}
	// perdas em transformador SDBT
	if (CfgPerdaTec->OpcaoPerdaTrafo != strCfgPerdaTec_anterior.opcao.perda_trafo)
	{
		return true;
	}
	else
	{
		// caso sejam iguais..
		if ((CfgPerdaTec->PerdaTrafoUnico != NULL) &&
			(strCfgPerdaTec_anterior.unico.perda_trafo != NULL))
		{ // verifca se s�o iguais
			PERDA_TRAFO *trafo1, *trafo2;

			trafo1 = CfgPerdaTec->PerdaTrafoUnico;
			trafo2 = strCfgPerdaTec_anterior.unico.perda_trafo;

			if (trafo1->tipo != trafo2->tipo)
			{
				return true;
			}
			// verifica se tem valores iguais
			if (!IsDoubleZero(trafo1->vmin_kv - trafo2->vmin_kv))
			{
				return true;
			}
			if (!IsDoubleZero(trafo1->vmax_kv - trafo2->vmax_kv))
			{
				return true;
			}
			if (!IsDoubleZero(trafo1->snom_kva - trafo2->snom_kva))
			{
				return true;
			}
			if (!IsDoubleZero(trafo1->perda_vazio_w - trafo2->perda_vazio_w))
			{
				return true;
			}
			if (!IsDoubleZero(trafo1->perda_total_w - trafo2->perda_total_w))
			{
				return true;
			}
		}
		else if ((CfgPerdaTec->PerdaTrafoUnico == NULL) &&
			(strCfgPerdaTec_anterior.unico.perda_trafo == NULL))
		{
			// nada a fazer
		}
		else
		{
			return true;
		}
	}
	// perdas em transformador SED
	if (CfgPerdaTec->OpcaoPerdaTrafoSED != strCfgPerdaTec_anterior.opcao.perda_trafo_sed)
	{
		return true;
	}
	if (!IsDoubleZero(CfgPerdaTec->PerdaAdicional_perc -
		strCfgPerdaTec_anterior.perda.adicional_perc))
	{
		return true;
	}

	return alterado;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdareg::VerificaRedesCarregadas(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRedes;
	bool existeBTnCarregada = false;

	lisRedes = redes->LisRede();
	// percorre as redes em busca de alguma bt nao carregada
	for (int nr = 0; nr < lisRedes->Count; nr++)
	{
		rede = (VTRede*) lisRedes->Items[nr];

		if (!rede->Carregada)
		{
			if (rede->TipoRede->Segmento == redeSEC)
			{
				existeBTnCarregada = true;
				break;
			}
		}
	}

	return existeBTnCarregada;
}
// ---------------------------------------------------------------------------
// eof


