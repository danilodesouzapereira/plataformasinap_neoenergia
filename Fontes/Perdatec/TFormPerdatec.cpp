// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormPerdatec.h"
#include "TFormPerda.h"
#include "TFormCfgPerdaTec.h"
#include "TFormSalva.h"
#include "TAplPerdatec.h"
#include "VTPerdatec.h"
#include "RedePT\VTMontaRedePT.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BDihm\VTSalva.h"
#include "..\Copia\VTCopia.h"
#include "..\Curvas\VTCurva.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTRedes.h"
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
#include <Math.hpp>
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Diretorio\VTPath.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrecho.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14187786
__fastcall TFormPerdatec::TFormPerdatec(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
{
	// variáveis locais
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
	apl = new TAplPerdatec(this, apl_owner);
	// inclui objetos VTBD e VTBSalva
	apl->Add(DLL_NewObjBD(apl));
	apl->Add(DLL_NewObjSalva(this, apl));
	// cria TFormPerda dentro do PanelPerda
	FormPerda = new TFormPerda(this, apl, parent);
	// 2016.03.31: cria objeto VTRedes local
	redes_local = DLL_NewObjRedes(apl);
	CopiaRedes1ParaRedes2((VTRedes*)apl->GetObject(__classid(VTRedes)), redes_local);
	// indica que os parâmetros para cálculo de perdas foram alterados
	geral->CfgPerdaTec->Alterada = true;
	// inicia data de referência com a data da rede
	DateTimePickerInicia();
	if (prog)
	{
		prog->Stop();
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14187786
__fastcall TFormPerdatec::~TFormPerdatec(void)
{
	// variáveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRedes *redes_graf;
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	VTFlow *flowOwner;

	// restaura objeto VTRedes
	CopiaRedes1ParaRedes2(redes_local, (VTRedes*)apl->GetObject(__classid(VTRedes)));
	redes_graf = (VTRedes*)grafico->Apl->GetObject(__classid(VTRedes));
	grafico->Apl->Remove(redes_graf);
	grafico->Apl->Add(redes);
	grafico->AreaRedeDefine();
	grafico->Refresh();

	// muda apresentação p/ topologia da Rede
	if (sinap_child)
		sinap_child->ActionTopo->Execute();
	// restaura editor
//	if (sinap_child)
//		sinap_child->ActionEditor->Enabled = editor_enabled;
    if (sinap_child)
	{
		sinap_child->EndEditorBlock(this);
	}
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

	// destrói TFormPerda
	if (FormPerda)
	{
		delete FormPerda;
		FormPerda = NULL;
	}
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// retira Eqptos incluídos para cálculo de perdas
	// montaRedePT->RetiraEqptosRedePT();
	// destrói objetos
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
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::ActionCfgParametroExecute(TObject *Sender)
{
	// exibe tela para configura parâmetros
	ConfiguraParametros();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::ActionFecharExecute(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::ActionCalculaPerdasExecute(TObject *Sender)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPerdatec *perdatec = (VTPerdatec*)apl->GetObject(__classid(VTPerdatec));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se a data foi realmente selecionada
	if (!DataSelecionada())
		return;
	// verifica se nada foi alterado
	if ((!geral->CfgPerdaTec->Alterada) && (DateOf(DateTimePicker->Date) == last_date))
	{ // não é necessário recalcular: exibe relatório de perda técnica
		ActionRelPerdaTecnica->Execute();
		return;
	}
	// salva data atualmente selecionada
	last_date = DateOf(DateTimePicker->Date);
	// verifica se os parâmetros foram alterados
	if (geral->CfgPerdaTec->Alterada)
	{ // reinicia objeto Perdatec
		perdatec->Clear();
		// atualiza o objeto VTRedes
		// CopiaRedes1ParaRedes2(redes_local, redes);
		// atualiza o grafico
		grafico->AreaRedeDefine();
		grafico->Refresh();
		// reinicia zonas
		zonas->Executa(redes);
	}
	// calcula perdas
	if (perdatec->Executa(DateTimePicker->Date, SEMANA_COMPLETA))
	{
		// habilita botões
		ButRelatorio->Enabled = true;
		ButSalvaBase->Enabled = true;
		// exibe relatório de perda técnica
		ActionRelPerdaTecnica->Execute();
	}
	else
	{ // desabilita botões
		ButRelatorio->Enabled = false;
		ButSalvaBase->Enabled = false;
		// esconde relatórios
		FormPerda->Hide();
	}
	// atualiza o grafico
	grafico->AreaRedeDefine();
	grafico->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Perda_Técnica");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::ActionRelPerdaTecnicaExecute(TObject *Sender)
{
	// exibe FormRelPerda
	FormPerda->Inicia(DateTimePicker->Date);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::ActionSalvaBaseExecute(TObject *Sender)
{
	// variáveis locais
	TFormSalva *FormSalva;

	try
	{ // cria TFormSalva e exibe como janela modal
		FormSalva = new TFormSalva(this, apl);
		FormSalva->Caption = FormSalva->Caption + " Técnica";
		FormSalva->ShowModal();
		delete FormSalva;
	}
	catch (Exception &e)
	{ // nada a fazer
		Aviso("Não foi possível atualizar a base de perda");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::CalculaPerdasTecnicas(void)
{
	// variáveis locais
	VTPerdatec *perdatec = (VTPerdatec*)apl->GetObject(__classid(VTPerdatec));

	// calcula perdas
	perdatec->Executa(DateTimePicker->Date, SEMANA_COMPLETA);
	// calcula valores agregados de todos Resumos
	FormPerda->CalculaAgregadaTotal();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormPerdatec::ConfiguraParametros(void)
{
	// variáveis locais
	bool sucesso = false;
	TFormCfgPerdatec *FormCfgPerdatec;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	try
	{ // cria FormCfgPerdatec p/ configurar parâmetros de cálculo de perdas técnicas
		FormCfgPerdatec = new TFormCfgPerdatec(this, apl);
		if (FormCfgPerdatec->ShowModal() == mrOk)
		{ // desabilita botões
			ButRelatorio->Enabled = false;
			ButSalvaBase->Enabled = false;
			// esconde relatórios
			FormPerda->Hide();
			// atualiza o gráfico pois a rede pode ter sido alterada
			grafico->AreaRedeDefine();
			sucesso = true;
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
bool __fastcall TFormPerdatec::CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2)
{
	// variáveis locais
	bool sucesso = true;
	VTCopia *copia;

	try
	{ // proteção
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
bool __fastcall TFormPerdatec::DataSelecionada(void)
{
	// verifica se usuário já efetuou a seleção de uma data
	if (DateTimePicker->Checked)
		return (true);

	Aviso("É necessário confirmar a data selecionada");
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::DateTimePickerCloseUp(TObject *Sender)
{
	// verifica se houve alteração da data selecionada
	if (DateOf(DateTimePicker->Date) != date)
	{ // esconde FormPerda
		FormPerda->Hide();
		// desabilita botão de relatório
		ButRelatorio->Enabled = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::DateTimePickerDropDown(TObject *Sender)
{
	// salva data atualmente selecionada
	date = DateOf(DateTimePicker->Date);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::DateTimePickerInicia(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	DateTimePicker->DateTime = redes->DataReferencia;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerdatec::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
// eof
/*
 void __fastcall TFormPerdatec::ToolButton2Click(TObject *Sender)
 {
 //função teste: exporta todas cargas
 //var locais
 VTDemanda   *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
 VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTBarra     *barra;
 VTCarga     *carga;
 VTEqbar     *eqb;
 TList       *lisBAR = redes->LisBarra();
 TList       *lisEQB;
 TStringList *str_carga = new TStringList();
 complex<double> sf_mva[MAX_FASE];
 int             fase_tag;
 UnicodeString   txt, caminho;

 str_carga->Clear();
 txt = "codigo;hora;Pa;Qa;Pb;Qb;Pc;Qc;";
 str_carga->Add(txt);
 for(int nb = 0; nb < lisBAR->Count; nb++)
 {
 barra = (VTBarra*)lisBAR->Items[nb];
 lisEQB = barra->LisEqbar();
 for(int nc = 0; nc < lisEQB->Count; nc++)
 {
 eqb = (VTEqbar*)lisEQB->Items[nc];
 if(eqb->Tipo() != eqptoCARGA) continue;
 carga = (VTCarga*)eqb;
 for(int np = 0; np < patamares->NumPatamar(); np++)
 {
 if(! demanda->Get_S3f_mva(carga, np, sf_mva)) continue;
 //escreve
 txt = carga->Codigo + ";";
 txt += IntToStr(np) + ";";
 txt += DoubleToStr("%16.15f", sf_mva[indFASE_A].real()) + ";";
 txt += DoubleToStr("%16.15f", sf_mva[indFASE_A].imag()) + ";";
 txt += DoubleToStr("%16.15f", sf_mva[indFASE_B].real()) + ";";
 txt += DoubleToStr("%16.15f", sf_mva[indFASE_B].imag()) + ";";
 txt += DoubleToStr("%16.15f", sf_mva[indFASE_C].real()) + ";";
 txt += DoubleToStr("%16.15f", sf_mva[indFASE_C].imag()) + ";";
 str_carga->Add(txt);
 }
 }
 }
 caminho = path->DirTmp() + "\\CurvaCargas.csv";
 str_carga->SaveToFile(caminho);
 delete str_carga;
 }
 */
// ---------------------------------------------------------------------------

void __fastcall TFormPerdatec::ToolButton2Click(TObject *Sender)
{
	// função teste: exporta impedância dos cabos
	// var locais
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTrecho *trecho;
	VTRede *rede;
	VTLigacao *ligacao;
	TStringList *str_carga = new TStringList();
	UnicodeString txt, caminho;
	TList *lisREDE = redes->LisRede();
	TList *lisLIG;
	double r1, x1, y1;

	str_carga->Clear();
	txt = "codigo;fase;r1;x1;y1;";
	str_carga->Add(txt);

	for (int nb = 0; nb < lisREDE->Count; nb++)
	{
		rede = (VTRede*)lisREDE->Items[nb];
		lisLIG = rede->LisLigacao();
		for (int nc = 0; nc < lisLIG->Count; nc++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[nc];
			if (ligacao->Tipo() != eqptoTRECHO)
				continue;
			trecho = (VTTrecho*)ligacao;
			txt = trecho->Codigo + ";";
			txt += fases->AsStrABCN[trecho->arranjo->Fases] + ";";
			// verifica se o Trecho é trifásico
			if ((trecho->arranjo->TipoCfg == arranjoZ0Z1) ||
				((trecho->arranjo->Fases & faseABC) == faseABC))
			{ // obtém impedância em ohm
				trecho->Z1_ohm(r1, x1, y1);
				txt += DoubleToStr("%16.15f", r1) + ";";
				txt += DoubleToStr("%16.15f", x1) + ";";
				txt += DoubleToStr("%16.15f", y1) + ";";
			}
			else
			{ // Trecho não trifasico
				txt += "não aplicável";
			}
			// escreve
			str_carga->Add(txt);
		}
	}
	caminho = path->DirTmp() + "\\ImpTrechos.csv";
	str_carga->SaveToFile(caminho);
	delete str_carga;
}
// ---------------------------------------------------------------------------
