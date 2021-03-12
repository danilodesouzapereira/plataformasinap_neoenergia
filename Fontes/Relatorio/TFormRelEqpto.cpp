// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormRelEqpto.h"
#include "TFormPlanilha.h"
#include "VTResFlowRede.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRelEqpto::TFormRelEqpto(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
{
	// salva ponteiro p/ objetos
	apl = apl_owner;
	// redefine parent
	if (parent)
		Parent = parent;
	// cria objeto lisFORM
	lisEQP = new TList();
	FormPlanilha = new TFormPlanilha(this, panelRel, apl, Connection);
	// inicia PopupMenuRelatorio
	PopupMenuRelatorioInicia();
	// inicia o progresso
	progresso = DLL_NewObjProgresso(this, NULL);
	progresso->Start(progDEFAULT);
	// inicia base de dados p/ relatório
	ConectaBaseDadoRelatorio();
	// para o progresso
	progresso->Stop();
	// FKM 2016.04.05 inicializa booleans que guardam se a tabela foi atualizada
	for (int i = 0; i < tipoRelCOUNT; i++)
	{
		preencheu[i] = false;
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormRelEqpto::~TFormRelEqpto(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// finaliza base de dados p/ relatório
	DeconectaBaseDadoRelatorio();
	// esconde eventual FormRel<> que esteja sendo exibido
	DesabilitaRelatorio();
	// destrói TFormRelEqpto
	if (FormPlanilha)
	{
		delete FormPlanilha;
		FormPlanilha = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	// destrói objetos
	if (progresso)
	{
		delete progresso;
		progresso = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Relatórios_Relatórios_dos_Equipamentos");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelBarraExecute(TObject *Sender)
{
	// monta tabela c/ dados da Barra
	// PreencheRelBarra();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelBarra])
	{
		PreencheRelBarra();
		preencheu[tipoRelBarra] = true;
	}
	// 'abaixa' butRelBarra
	butRelBarra->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Barra");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelCapacitorExecute(TObject *Sender)
{
	// PreencheRelCapacitor();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelCapacitor])
	{
		PreencheRelCapacitor();
		preencheu[tipoRelCapacitor] = true;
	}
	// 'abaixa' butRelBarra
	butRelCapacitor->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Capacitor");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelCargaExecute(TObject *Sender)
{
	// PreencheRelCarga();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelCarga])
	{
		PreencheRelCarga();
		preencheu[tipoRelCarga] = true;
	}
	// 'abaixa' butRelCarga
	butRelCarga->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Carga");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelChaveExecute(TObject *Sender)
{
	// PreencheRelChave();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelChave])
	{
		PreencheRelChave();
		preencheu[tipoRelChave] = true;
	}
	// 'abaixa' butRelChave
	butRelChave->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Chave");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelEqptoExecute(TObject *Sender)
{
	// cancela Patamar selecionado p/ relatório de fluxo
	rel_fluxo.patamar = NULL;
	// redefine Caption de ButRelatorio
	ButRelatorio->Caption = "Equipamento (atributos)";
	// associa botões do ToolBar c/ Actions de relatório de eqptos
	ToolBarRedefineAction(true);
	// verifica se há algum botão "down"
	for (int n = 0; n < ToolBar->ButtonCount; n++)
	{
		if (ToolBar->Buttons[n]->Down)
		{ // executa Action associado ao botão
			ToolBar->Buttons[n]->Action->Execute();
			return;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelFluxoExecute(TObject *Sender)
{
	// determina MenuItem selecionado
	for (int i = 0; i < PopupMenuRelatorio->Items->Count; i++)
	{
		for (int j = 0; j < PopupMenuRelatorio->Items->Items[i]->Count; j++)
		{
			if (PopupMenuRelatorio->Items->Items[i]->Items[j]->Checked)
			{ // determina Patamar selecionado p/ o relatório de fluxo
				rel_fluxo.patamar = (VTPatamar*)PopupMenuRelatorio->Items->Items[i]->Items[j]->Tag;
				if (rel_fluxo.patamar == NULL)
					return;
				// redefine Caption de ButRelatorio
				ButRelatorio->Caption = "Fluxo de Potência: " + rel_fluxo.patamar->Nome;
				// associa botões do ToolBar c/ Actions de relatório de fluxo
				ToolBarRedefineAction(false);
				// verifica se há algum botão "down"
				for (int n = 0; n < ToolBar->ButtonCount; n++)
				{
					if (ToolBar->Buttons[n]->Down)
					{ // executa Action associado ao botão
						if (ToolBar->Buttons[n]->Action)
							ToolBar->Buttons[n]->Action->Execute();
						return;
					}
				}
				return;
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelFluxoBarraExecute(TObject *Sender)
{
	// monta tabela c/ dados da Barra
	// PreencheRelFluxoBarra();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelFluxoBarra])
	{
		PreencheRelFluxoBarra();
		preencheu[tipoRelFluxoBarra] = true;
	}
	// 'abaixa' butRelBarra
	butRelBarra->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("FluxoBarra");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelFluxoRedeExecute(TObject *Sender)
{
	// monta tabela c/ dados da Barra
	// PreencheRelFluxoRede();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelFluxoRede])
	{
		PreencheRelFluxoRede();
		preencheu[tipoRelFluxoRede] = true;
	}
	// 'abaixa' butRelBarra
	butRelRede->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("FluxoRede");
	FormPlanilha->Show();
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRelEqpto::ActionRelFluxoReguladorExecute(TObject *Sender)
 {
 PreencheRelFluxoRegulador();
 //'abaixa' butRelFluxo de Potência
 butRelRegulador->Down = true;
 //esconde FormRelEqpto
 DesabilitaRelatorio();
 //exibe FormRelEqpto
 FormPlanilha->ExibeTabela("FluxoRegulador");
 FormPlanilha->Show();
 } */
// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelFluxoTrechoExecute(TObject *Sender)
{
	// PreencheRelFluxoTrecho();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelFluxoTrecho])
	{
		PreencheRelFluxoTrecho();
		preencheu[tipoRelFluxoTrecho] = true;
	}
	// 'abaixa' butRelFluxo de Potência
	butRelTrecho->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("FluxoTrecho");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelFluxoTrafoExecute(TObject *Sender)
{
	// PreencheRelFluxoTrafo();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelFluxoTrafo])
	{
		PreencheRelFluxoTrafo();
		preencheu[tipoRelFluxoTrafo] = true;
	}
	// 'abaixa' butRelFluxo de Potência
	butRelTrafo->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("FluxoTrafo");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelGeradorExecute(TObject *Sender)
{
	// PreencheRelGerador();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelGerador])
	{
		PreencheRelGerador();
		preencheu[tipoRelGerador] = true;
	}
	// 'abaixa' butRelGerador
	butRelGerador->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Gerador");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelPerfilCargaExecute(TObject *Sender)
{
	// PreencheRelPerfilCarga();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelPerfilCarga])
	{
		PreencheRelPerfilCarga();
		preencheu[tipoRelPerfilCarga] = true;
	}
	// 'abaixa' butRelPerfilCarga
	butRelPerfilCarga->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("PerfilCarga");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelReatorExecute(TObject *Sender)
{
	// PreencheRelReator();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelReator])
	{
		PreencheRelReator();
		preencheu[tipoRelReator] = true;
	}
	// 'abaixa' butRelReator
	butRelReator->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Reator");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelRedeExecute(TObject *Sender)
{
	// PreencheRelRede();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelRede])
	{
		PreencheRelRede();
		preencheu[tipoRelRede] = true;
	}
	// 'abaixa' butRelRede
	butRelRede->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Rede");
	FormPlanilha->Show();
}

/* DVK 2012.10.05 INICIO */
// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelReguladorExecute(TObject *Sender)
{
	// PreencheRelRegulador();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelRegulador])
	{
		PreencheRelRegulador();
		preencheu[tipoRelRegulador] = true;
	}
	// 'abaixa' butRelRegulador
	butRelRegulador->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Regulador");
	FormPlanilha->Show();
}

/* DVK 2012.10.05 FIM */
// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelSuprimentoExecute(TObject *Sender)
{
	// PreencheRelSuprimento();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelSuprimento])
	{
		PreencheRelSuprimento();
		preencheu[tipoRelSuprimento] = true;
	}
	// 'abaixa' butRelSuprimento
	butRelSuprimento->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Suprimento");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelTrafoExecute(TObject *Sender)
{
	// PreencheRelTrafo();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelTrafo])
	{
		PreencheRelTrafo();
		preencheu[tipoRelTrafo] = true;
	}
	// 'abaixa' butRelTrafo
	butRelTrafo->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Trafo");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelTrafo3EExecute(TObject *Sender)
{
	// PreencheRelTrafo3E();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelTrafo3E])
	{
		PreencheRelTrafo3E();
		preencheu[tipoRelTrafo3E] = true;
	}
	// 'abaixa' butRelTrafo3E
	butRelTrafo3E->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Trafo3E");
	FormPlanilha->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ActionRelTrechoExecute(TObject *Sender)
{
	// PreencheRelTrecho();
	// FKM 2016.04.05 se a tabela foi atualizada
	if (!preencheu[tipoRelTrecho])
	{
		PreencheRelTrecho();
		preencheu[tipoRelTrecho] = true;
	}
	// 'abaixa' butRelTrecho
	butRelTrecho->Down = true;
	// esconde FormRelEqpto
	DesabilitaRelatorio();
	// exibe FormRelEqpto
	FormPlanilha->ExibeTabela("Trecho");
	FormPlanilha->Show();
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRelatorio::ActionRelLigCCExecute(TObject *Sender)
 {
 PreencheRelLigCC();
 //'abaixa' butRelBarra
 butRelLigCC->Down = true;
 //esconde eventual FormRel<> que esteja sendo exibido
 DesabilitaRelatorio();
 //verifica se ainda não existe TFormRelBarra
 if (FormRelLigCC != NULL)
 {
 delete FormRelLigCC;
 lisFORM->Remove(FormRelLigCC);
 FormRelLigCC = NULL;
 }
 if (FormRelLigCC == NULL)
 {//cria TFormRelBarra
 FormRelLigCC = new TFormRelEqpto(this, panelRel, Connection, "LigCC");
 lisFORM->Add(FormRelLigCC);
 }
 //exibe FormRelBarra
 FormRelLigCC->Show();
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormRelatorio::ActionRelBarCCExecute(TObject *Sender)
 {
 PreencheRelBarCC();
 //'abaixa' butRelBarra
 butRelBarCC->Down = true;
 //esconde eventual FormRel<> que esteja sendo exibido
 DesabilitaRelatorio();
 //verifica se ainda não existe TFormRelBarra
 if (FormRelBarCC != NULL)
 {
 delete FormRelBarCC;
 lisFORM->Remove(FormRelBarCC);
 FormRelBarCC = NULL;
 }
 if (FormRelBarCC == NULL)
 {//cria TFormRelBarra
 FormRelBarCC = new TFormRelEqpto(this, panelRel, Connection, "BarCC");
 lisFORM->Add(FormRelBarCC);
 }
 //exibe FormRelBarra
 FormRelBarCC->Show();
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::Atualiza(void)
{
	// reinicia PopupMenuRelatorio
	PopupMenuRelatorioInicia();
}

// ---------------------------------------------------------------------------
double __fastcall TFormRelEqpto::CapacidadeTrechoMVA(VTTrecho *trecho)
{
	// variáveis locais
	double cap_kva = 0;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// verifica se Arranjo tem as 3 fases
	if ((trecho->arranjo->Fases & faseABC) == faseABC)
	{
		cap_kva = sqrt(3.) * trecho->pbarra1->vnom * trecho->arranjo->Iadm_amp;
	}
	else
	{
		for (int ind_fase = 0; ind_fase <= MAX_FASE - 1; ind_fase++)
		{
			if (fases->Fase1ContemFase2(faseABC, fases->IndexToTag[ind_fase]))
			{
				cap_kva += trecho->pbarra1->zona->Vfn_kv[ind_fase] * trecho->arranjo->Ifase_amp
					[ind_fase];
			}
		}
	}
	// retorn capacidade em MVA
	return (0.001 * cap_kva);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::Carga_Curva(int tipica)
{
	switch (tipica)
	{
	case 0:
		return ("Não");
	case 1:
		return ("Sim");
	}
	return ("Inválida");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::Carga_Especial(int vip)
{ // DVK 2014.01.27
	switch (vip)
	{
	case 0:
		return ("Não");
	case 1:
		return ("Sim");
	}
	return ("Inválida");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::Carga_Modelo(int modelo)
{
	switch (modelo)
	{
	case 1:
		return ("Icte");
	case 2:
		return ("Scte");
	case 3:
		return ("Zcte");
	}
	return ("Inválida");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::Chave_Estado(VTChave *chave)
{
	if (chave->Aberta)
		return ("Aberta");
	return ("Fechada");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::ConnectionString(AnsiString arq_db)
{
	// variáveis locais
	AnsiString conn_str;

	// monta ConnectionString
//	conn_str = "Provider=Microsoft.Jet.OLEDB.4.0;";
	conn_str = "Provider=Microsoft.ACE.OLEDB.12.0;";
	conn_str += "Data Source=" + arq_db + ";";
	conn_str += "Jet OLEDB:Database Password=sinap2006_2013";
	return (conn_str);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ConectaBaseDadoRelatorio(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	WideString report_model = path->DirDat() + "\\Relatorio\\SinapReportModel.mdb";
	WideString report = path->DirDat() + "\\Relatorio\\SinapReport.mdb";

	// copia base de dado modelo (vazio) para o Relatorio
	CopyFile(report_model.c_bstr(), report.c_bstr(), false);
	// proteção: desconecta base de dados de relatórios
	DeconectaBaseDadoRelatorio();
	// monta string p/ conexão c/ a base de dados
	Connection->ConnectionString = ConnectionString(report);
	// inicia conexão
	Connection->Connected = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::DeconectaBaseDadoRelatorio(void)
{
	if (Connection->Connected)
		Connection->Connected = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::DesabilitaRelatorio(void)
{
	FormPlanilha->Hide();
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TFormRelEqpto::ExisteRede(VTBarra *pbarra)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// determina uma Barra que contenha a Barra indicada
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->ExisteBarra(pbarra))
			return (rede);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::FormIniciaPosicao(void)
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
	Left = 0; ;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
	// exibe relatório de Redes
	butRelRede->Down = true;
	ActionRelEqpto->Execute();
}

// ---------------------------------------------------------------------------
int __fastcall TFormRelEqpto::NumeroDeCarga(TList *lisCARGA, VTClasse *classe, TFase *fase)
{
	// variáveis locais
	int count = 0;
	VTCarga *carga;

	// loop p/ todas  Cargas em lisCARGA
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*)lisCARGA->Items[nc];
		// verifica se a Carga atende à Classe e Fase indicada
		if ((carga->classe == classe) && (carga->Fases == fase->tag))
			count++;
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TFormRelEqpto::NumeroDePatamarComFluxoCalculado(void)
{
	// variáveis locais
	int count = 0;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// determina número de patamares com fluxo calculado
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		patamar = patamares->Patamar[ind_pat];
		// verifica se foi calculado fluxo no Patamar
		if (!patamar->Enabled)
			continue;
		if (!patamar->FluxoCorreto)
			continue;
		count++;
	}
	return (count);
}

// ---------------------------------------------------------------------------
TMenuItem* __fastcall TFormRelEqpto::PopupMenuIniciaMenuItem(void)
{
	// variáveis locais
	TMenuItem *MenuItem;

	// cria um MenuIem
	MenuItem = new TMenuItem(Owner);
	MenuItem->Action = NULL;
	MenuItem->Caption = Caption;
	// MenuItem->GroupIndex = GroupIndex;
	MenuItem->Tag = 0;
	MenuItem->RadioItem = false;
	MenuItem->AutoCheck = false;
	return (MenuItem);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::PopupMenuRelatorioInicia(void)
{
	// variáveis locais
	TMenuItem *MenuItem, *MenuItemPai;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// reinicia PopupMenu
	PopupMenuRelatorio->Items->Clear();
	// insere MenuItem p/ relatório de Eqptos
	MenuItemPai = PopupMenuIniciaMenuItem();
	MenuItemPai->Action = ActionRelEqpto;
	PopupMenuRelatorio->Items->Add(MenuItemPai);
	// verifica se há patamar c/ fluxo calculado
	if (NumeroDePatamarComFluxoCalculado() == 0)
		return;
	// insere MenuItem p/ relatório de Fluxo
	MenuItemPai = PopupMenuIniciaMenuItem();
	MenuItemPai->Caption = "Fluxo de Potência";
	PopupMenuRelatorio->Items->Add(MenuItemPai);
	// insere MenuItem p/ patamares
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		patamar = patamares->Patamar[ind_pat];
		// verifica se foi calculado fluxo no Patamar
		if (!patamar->Enabled)
			continue;
		if (!patamar->FluxoCorreto)
			continue;
		// insere MenuItem p/ Patamar
		MenuItem = PopupMenuIniciaMenuItem();
		MenuItem->Action = ActionRelFluxo;
		MenuItem->Caption = patamar->Nome;
		MenuItem->Tag = (int)patamar;
		MenuItem->RadioItem = true;
		MenuItem->AutoCheck = true;
		MenuItemPai->Add(MenuItem);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRelatorio::PreencheRelBarCC(void)
 {
 //variáveis locais
 int ind_def;
 VTBarCC *pbarra;
 TList   *lisBAR;
 VTRedeCC *redeCC  = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
 //protecao: caso nao esteja selecionado ponto de defeito, return;
 if (redeCC == NULL) return;
 if (redeCC->BarraDefeito() == NULL && redeCC->TrechoDefeito() == NULL) return;
 //seleciona qual o tipo de defeito
 switch (cbDefeito->ItemIndex)
 {
 case 0: ind_def = defeito3F; break;
 case 1: ind_def = defeito2F; break;
 case 2: ind_def = defeitoFT; break;
 case 3: ind_def = defeitoFTZ; break;
 case 4: ind_def = defeito2FT; break;
 default: return;
 }
 //proteção: reinicia o objeto TTable
 table->Close();
 table->Filtered = false;
 table->Filter   = "";
 table->TableName = "BarCC";
 table->Open();
 //atualiza tabela Barra
 lisBAR = redeCC->LisBarraCC();
 //grava todos Eqptos
 for (int n = 0; n < lisBAR->Count; n++)
 {
 pbarra = (VTBarCC*)lisBAR->Items[n];
 //verifica se a Barra precisa ser gravada
 table->Filter   = "barra_id=" + IntToStr(pbarra->Barra->Id);
 table->Filtered = true;
 if (table->RecordCount == 0)
 {
 table->Filtered = false;
 table->Append();
 }
 //habilita edição
 table->Edit();
 //atualiza campos
 table->FieldByName("barra_id")->AsInteger = pbarra->Barra->Id;
 table->FieldByName("codigo")->AsString    = pbarra->Barra->Codigo;
 table->FieldByName("vnom")->AsFloat       = pbarra->Barra->vnom;
 table->FieldByName("Ia")->AsString = ComplexPolarToStr("%4.3f", pbarra->Ifas_amp[ind_def][0]);
 table->FieldByName("Ib")->AsString = ComplexPolarToStr("%4.3f", pbarra->Ifas_amp[ind_def][1]);
 table->FieldByName("Ic")->AsString = ComplexPolarToStr("%4.3f", pbarra->Ifas_amp[ind_def][2]);
 table->FieldByName("I0")->AsString = ComplexPolarToStr("%4.3f", pbarra->Iseq_amp[ind_def][0]);
 table->FieldByName("I1")->AsString = ComplexPolarToStr("%4.3f", pbarra->Iseq_amp[ind_def][1]);
 table->FieldByName("I2")->AsString = ComplexPolarToStr("%4.3f", pbarra->Iseq_amp[ind_def][2]);
 table->FieldByName("Va")->AsString = ComplexPolarToStr("%4.3f", pbarra->Vfas_pu[ind_def][0]);
 table->FieldByName("Vb")->AsString = ComplexPolarToStr("%4.3f", pbarra->Vfas_pu[ind_def][1]);
 table->FieldByName("Vc")->AsString = ComplexPolarToStr("%4.3f", pbarra->Vfas_pu[ind_def][2]);
 table->FieldByName("V0")->AsString = ComplexPolarToStr("%4.3f", pbarra->Vseq_pu[ind_def][0]);
 table->FieldByName("V1")->AsString = ComplexPolarToStr("%4.3f", pbarra->Vseq_pu[ind_def][1]);
 table->FieldByName("V2")->AsString = ComplexPolarToStr("%4.3f", pbarra->Vseq_pu[ind_def][2]);
 table->FieldByName("S")->AsString = ComplexPolarToStr("%4.3f", pbarra->S_mva[ind_def]);
 //força gravaçao do registro
 table->Post();
 }
 table->Close();
 RelBarCC = false;
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormRelatorio::PreencheRelLigCC(void)
 {
 //variáveis locais
 int      ind_def, ind_bar;
 VTLigCC *pligCC;
 TList   *lisLIG;
 VTRedeCC *redeCC  = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));

 //protecao: caso nao esteja selecionado ponto de defeito, return;
 if (redeCC == NULL) return;
 if (redeCC->BarraDefeito() == NULL && redeCC->TrechoDefeito() == NULL) return;
 //seleciona corrente em relação à barra 0
 ind_bar = 0;
 //seleciona qual o tipo de defeito
 switch (cbDefeito->ItemIndex)
 {
 case 0: ind_def = defeito3F; break;
 case 1: ind_def = defeito2F; break;
 case 2: ind_def = defeitoFT; break;
 case 3: ind_def = defeitoFTZ; break;
 case 4: ind_def = defeito2FT; break;
 default: return;
 }
 //proteção: reinicia o objeto TTable
 table->Close();
 table->Filtered = false;
 table->Filter   = "";
 table->TableName = "LigCC";
 table->Open();
 //atualiza tabela Barra
 lisLIG = redeCC->LisLigacaoCC();
 //grava todos Eqptos
 for (int n = 0; n < lisLIG->Count; n++)
 {
 pligCC = (VTLigCC*)lisLIG->Items[n];
 if (pligCC->Ligacao->rede == NULL) continue;

 //verifica se a Ligacao precisa ser gravada
 table->Filter   = "ligacao_id=" + IntToStr(pligCC->Ligacao->Id);
 table->Filtered = true;
 if (table->RecordCount == 0)
 {
 table->Filtered = false;
 table->Append();
 }

 //habilita edição
 table->Edit();
 //atualiza campos
 table->FieldByName("ligacao_id")->AsInteger = pligCC->Ligacao->Id;
 table->FieldByName("tipo")->AsString        = pligCC->Ligacao->TipoAsString();
 table->FieldByName("codigo")->AsString      = pligCC->Ligacao->Codigo;
 table->FieldByName("rede")->AsString        = pligCC->Ligacao->rede->Codigo;
 table->FieldByName("Ia")->AsString          = ComplexPolarToStr("%4.3f", pligCC->Ifas_amp[ind_def][ind_bar][0]);
 table->FieldByName("Ib")->AsString          = ComplexPolarToStr("%4.3f", pligCC->Ifas_amp[ind_def][ind_bar][1]);
 table->FieldByName("Ic")->AsString          = ComplexPolarToStr("%4.3f", pligCC->Ifas_amp[ind_def][ind_bar][2]);
 table->FieldByName("I0")->AsString          = ComplexPolarToStr("%4.3f", pligCC->Iseq_amp[ind_def][ind_bar][0]);
 table->FieldByName("I1")->AsString          = ComplexPolarToStr("%4.3f", pligCC->Iseq_amp[ind_def][ind_bar][1]);
 table->FieldByName("I2")->AsString          = ComplexPolarToStr("%4.3f", pligCC->Iseq_amp[ind_def][ind_bar][2]);
 table->FieldByName("S")->AsString           = ComplexPolarToStr("%4.3f", pligCC->S_mva[ind_def][0]);
 //força gravaçao do
 table->Post();
 }
 table->Close();
 RelLigCC = false;
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelBarra(void)
{
	// variáveis locais
	VTBarra *pbarra;
	VTRede *rede;
	TList *lisBAR;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelBarra
	butRelBarra->Down = true;
	try
	{ // reinicia tabela Barra
		ReiniciaTabela("Barra");
		// reinicia o objeto TADOTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Barra";
		// abre a tabela
		table->Open();
		// atualiza tabela Barra
		lisBAR = redes->LisBarra();
		// grava todos Eqptos
		for (int n = 0; n < lisBAR->Count; n++)
		{
			pbarra = (VTBarra*)lisBAR->Items[n];
			// detemina uma Rede da Barra
			rede = ExisteRede(pbarra);
			// DVK 2014.05.05 exclui barras de redes reduzidas
			if ((rede != NULL) && (!rede->Carregada))
				continue;
			// insere novo registro e habilita edição
			table->Append();
			table->Edit();
			// atualiza campos
			table->FieldByName("barra_id")->AsInteger = pbarra->Id;
			table->FieldByName("barra")->AsString = pbarra->Codigo;
			table->FieldByName("vnom_kv")->AsFloat = pbarra->vnom;
			table->FieldByName("utm_x")->AsFloat = pbarra->utm.x;
			table->FieldByName("utm_y")->AsFloat = pbarra->utm.y;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(pbarra);
			#endif

			#ifdef __WIN32__
			int obj = int(pbarra);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravação do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelCapacitor(void)
{
	// variáveis locais
	VTCapacitor *capacitor;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelCapacitor
	butRelCapacitor->Down = true;
	try
	{ // reinicia tabela Capacitor
		ReiniciaTabela("Capacitor");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Capacitor";
		table->Open();
		// atualiza tabela Capacitor
		lisEQP->Clear();
		redes->LisEqbar(lisEQP, eqptoCAPACITOR);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			capacitor = (VTCapacitor*)lisEQP->Items[nc];
			// detemina uma Rede da Barra
			rede = ExisteRede(capacitor->pbarra);
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("capacitor_id")->AsInteger = capacitor->Id;
			table->FieldByName("barra_id")->AsInteger = capacitor->pbarra->Id;
			table->FieldByName("curva_id")->AsInteger = capacitor->curva->Id;
			table->FieldByName("barra")->AsString = capacitor->pbarra->Codigo;
			table->FieldByName("curva")->AsString = capacitor->curva->Codigo;
			table->FieldByName("capacitor")->AsString = capacitor->Codigo;
			table->FieldByName("ligacao")->AsString = TipoDeLigacao(capacitor->ligacao);
			table->FieldByName("vnom")->AsFloat = capacitor->vnom;
			table->FieldByName("q_kvar")->AsFloat = capacitor->q * 1000.;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(capacitor);
			#endif

			#ifdef __WIN32__
			int obj = int(capacitor);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelCarga(void)
{
	// variáveis locais
	AnsiString campo;
	VTCarga *pcarga;
	VTRede *rede;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelCarga
	butRelCarga->Down = true;
	try
	{ // reinicia tabela Carga
		ReiniciaTabela("Carga");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Carga";
		table->Open();
		// atualiza tabela Carga
		lisEQP->Clear();
		redes->LisEqbar(lisEQP, eqptoCARGA);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			pcarga = (VTCarga*)lisEQP->Items[nc];
			rede = ExisteRede(pcarga->pbarra);
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("carga_id")->AsInteger = pcarga->Id;
			table->FieldByName("carga")->AsString = pcarga->Codigo;
			table->FieldByName("barra_id")->AsInteger = pcarga->pbarra->Id;
			table->FieldByName("barra")->AsString = pcarga->pbarra->Codigo;
			table->FieldByName("classe_id")->AsInteger = pcarga->classe->Id;
			table->FieldByName("classe")->AsString = pcarga->classe->Codigo;
			table->FieldByName("especial")->AsString =
				Carga_Especial(int(pcarga->VeryImportant)); // DVK 2014.01.27
			// table->FieldByName("qtde cons")->AsInteger    = pcarga->NumConsTotal;
			// DVK 2014.12.03 alterado para "qtde_cons" aqui e no BD
			// (espaço causava erro na ordenação)
			table->FieldByName("qtde_cons")->AsInteger = pcarga->NumConsTotal;
			table->FieldByName("curva_id")->AsInteger = pcarga->curva->Id;
			table->FieldByName("tipica")->AsString = Carga_Curva(int(pcarga->curva->Tipica));
			table->FieldByName("fases")->AsString = fases->AsStrUpper[pcarga->Fases];
			table->FieldByName("E(MWh/mes)")->AsFloat = demanda->Emes_mwh[pcarga];
			table->FieldByName("modelo")->AsString = Carga_Modelo(pcarga->ModeloCarga);
			if (int(pcarga->curva->Tipica) == 0)
				table->FieldByName("curva")->AsString = "própria";
			else
				table->FieldByName("curva")->AsString = pcarga->curva->Codigo;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(pcarga);
			#endif

			#ifdef __WIN32__
			int obj = int(pcarga);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelChave(void)
{
	// variáveis locais
	AnsiString txt;
	VTChave *pchave;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipoChave *tipoChave;

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelChave
	butRelChave->Down = true;
	try
	{ // reinicia tabela Chave
		ReiniciaTabela("Chave");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Chave";
		table->Open();
		// atualiza tabela Chave
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoCHAVE);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			pchave = (VTChave*)lisEQP->Items[nc];
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("chave_id")->AsInteger = pchave->Id;
			table->FieldByName("chave")->AsString = pchave->Codigo;
			table->FieldByName("barra1_id")->AsInteger = pchave->pbarra1->Id;
			table->FieldByName("barra1")->AsString = pchave->pbarra1->Codigo;
			table->FieldByName("barra2_id")->AsInteger = pchave->pbarra2->Id;
			table->FieldByName("barra2")->AsString = pchave->pbarra2->Codigo;
			table->FieldByName("estado")->AsString = Chave_Estado(pchave);
			// table->FieldByName("tipo")->AsString         = pchave->TipoChave->Codigo;
			// teste
			tipoChave = pchave->TipoChave;
			if (tipoChave)
			{
				txt = pchave->TipoChave->Codigo;
				// verifica se base fusível
				if (pchave->TipoChave->Tipo == chaveBF)
				{ // verifica se foi definido o elo
					if (!pchave->EloFusivel.IsEmpty())
					{ // inclui informação do elo
						txt = txt + " " + pchave->EloFusivel;
					}
				}
				table->FieldByName("tipo")->AsString = txt;
			}
			else
			{
				table->FieldByName("tipo")->AsString = "Indefinida";
			}
			table->FieldByName("iadm_a")->AsFloat = pchave->Inom_amp;
			// preenche dados da Rede
			if (pchave->rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = pchave->rede->Id;
				table->FieldByName("rede")->AsString = pchave->rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(pchave);
			#endif

			#ifdef __WIN32__
			int obj = int(pchave);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro na base de dados
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelFluxoBarra(void)
{
	// variáveis locais
	TList *lisBAR;
	VTBarra *pbarra;
	VTRede *rede;
	VTResFlowBar *resflowbar;
	int ind_pat = rel_fluxo.patamar->Index;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTResFlowRede *resflow;

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelBarra
	butRelBarra->Down = true;
	try
	{ // reinicia tabela FluxoBarra
		ReiniciaTabela("FluxoBarra");
		// cria objeto VTResFlowRede
		resflow = NewObjResFlowRede(apl);
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "FluxoBarra";
		table->Open();
		// atualiza tabela Barra
		lisBAR = redes->LisBarra();
		// grava todos Eqptos
		for (int n = 0; n < lisBAR->Count; n++)
		{
			pbarra = (VTBarra*)lisBAR->Items[n];
			// detemina uma Rede da Barra
			rede = ExisteRede(pbarra);
			// DVK 2014.05.05 exclui barras de redes reduzidas
			if (!rede->Carregada)
				continue;
			resflowbar = pbarra->resflow;
			// insere um registro na tabela
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("barra_id")->AsInteger = pbarra->Id;
			table->FieldByName("barra")->AsString = pbarra->Codigo;
			table->FieldByName("status")->AsString = (pbarra->Status[sttISOLADO]) ? "desligado" :
				"ligado";
			table->FieldByName("vnom_kv")->AsFloat = pbarra->vnom;
			// módulo de tensão
			table->FieldByName("Va_pu")->AsFloat = Abs(resflowbar->Van_pu[ind_pat]);
			table->FieldByName("Vb_pu")->AsFloat = Abs(resflowbar->Vbn_pu[ind_pat]);
			table->FieldByName("Vc_pu")->AsFloat = Abs(resflowbar->Vcn_pu[ind_pat]);
			// ângulo da tensão
			table->FieldByName("Va_grau")->AsFloat = RadToDeg(Arg(resflowbar->Van_pu[ind_pat]));
			table->FieldByName("Vb_grau")->AsFloat = RadToDeg(Arg(resflowbar->Vbn_pu[ind_pat]));
			table->FieldByName("Vc_grau")->AsFloat = RadToDeg(Arg(resflowbar->Vcn_pu[ind_pat]));
			// potência injetada
			table->FieldByName("Pger_kW")->AsFloat =
				(resflow->Pgerador_mw(pbarra, ind_pat) + resflow->Psuprimento_mw(pbarra,
				ind_pat)) * 1000;
			table->FieldByName("Qger_kvar")->AsFloat =
				(resflow->Qgerador_mvar(pbarra, ind_pat) + resflow->Qsuprimento_mvar(pbarra,
				ind_pat)) * 1000;
			// potência de carga
			table->FieldByName("Pcar_kW")->AsFloat = resflow->Pcarga_mw(pbarra, ind_pat) * 1000;
			table->FieldByName("Qcar_kvar")->AsFloat = resflow->Qcarga_mvar(pbarra, ind_pat) * 1000;
			// potênciareativa injetada
			table->FieldByName("Qshunt_kvar")->AsFloat =
				(resflow->Qcapacitor_mvar(pbarra, ind_pat) + resflow->Qreator_mvar(pbarra,
				ind_pat)) * 1000;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(pbarra);
			#endif

			#ifdef __WIN32__
			int obj = int(pbarra);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRelEqpto::PreencheRelFluxoRegulador(void)
 {
 //variáveis locais
 bool           isolado;
 int            ind_bar_pri, ind_bar_sec;
 VTBarra        *bar_pri, *bar_sec;
 VTRegulador    *pregulador;
 VTResFlowLig   *resflowlig;
 int            ind_pat = rel_fluxo.patamar->Index;
 VTRedes        *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

 //'abaixa' butRelTrafo
 butRelRegulador->Down = true;
 try{//reinicia tabela FluxoRegulador
 ReiniciaTabela("FluxoRegulador");
 //proteção: reinicia o objeto TTable
 table->Close();
 table->Filtered = false;
 table->Filter   = "";
 table->TableName = "FluxoRegulador";
 table->Open();
 //atualiza tabela Regulador
 lisEQP->Clear();
 redes->LisLigacao(lisEQP, eqptoREGULADOR);
 //grava todos Eqptos
 for (int n = 0; n < lisEQP->Count; n++)
 {
 pregulador     = (VTRegulador*)lisEQP->Items[n];
 resflowlig = pregulador->resflow;
 //determina Barras do primário e do secundário
 bar_pri = pregulador->pbarra1;
 bar_sec = pregulador->pbarra2;
 ind_bar_pri = pregulador->IndiceBarra(bar_pri);
 ind_bar_sec = pregulador->IndiceBarra(bar_sec);
 //verifica se Eqpto isolado
 isolado = (bar_pri->Status[sttISOLADO] && bar_sec->Status[sttISOLADO]);
 //insere um registro na tabela
 table->Append();
 //habilita edição
 table->Edit();
 //atualiza campos
 table->FieldByName("regulador_id")->AsInteger   = pregulador->Id;
 table->FieldByName("regulador")->AsString       = pregulador->Codigo;
 table->FieldByName("status")->AsString          = (isolado) ? "desligado" : "ligado";
 table->FieldByName("snom_kva")->AsString        = pregulador->snom * 1000.;
 table->FieldByName("cargto_pu")->AsFloat        = resflowlig->Cargto[ind_pat];
 //dados do primário
 table->FieldByName("pri_barra")->AsString       = bar_pri->Codigo;
 table->FieldByName("pri_va_pu")->AsFloat        = Abs(bar_pri->resflow->Van_pu[ind_pat]);
 table->FieldByName("pri_s_kva")->AsFloat        = Abs(resflowlig->S_mva[ind_bar_pri][ind_pat]) * 1000.;
 table->FieldByName("pri_p_kw")->AsFloat         = resflowlig->P_mw[ind_bar_pri][ind_pat] * 1000.;
 table->FieldByName("pri_q_kvar")->AsFloat       = resflowlig->Q_mvar[ind_bar_pri][ind_pat] * 1000.;
 //dados do secundário
 table->FieldByName("sec_barra")->AsString       = bar_sec->Codigo;
 table->FieldByName("sec_va_pu")->AsFloat        = Abs(bar_sec->resflow->Van_pu[ind_pat]);
 table->FieldByName("sec_s_kva")->AsFloat        = Abs(resflowlig->S_mva[ind_bar_sec][ind_pat]) * 1000.;
 table->FieldByName("sec_p_kw")->AsFloat         = resflowlig->P_mw[ind_bar_sec][ind_pat] * 1000.;
 table->FieldByName("sec_q_kvar")->AsFloat       = resflowlig->Q_mvar[ind_bar_sec][ind_pat] * 1000.;
 //
 table->FieldByName("queda_tensao_pu")->AsFloat  = resflowlig->QuedaVfn_pu_max[ind_pat];
 table->FieldByName("Perda_kw")->AsFloat         = resflowlig->Perda_mw[ind_pat] * 1000;
 //preenche dados da Rede
 if (pregulador->rede != NULL)
 {
 table->FieldByName("rede_id")->AsInteger    = pregulador->rede->Id;
 table->FieldByName("rede")->AsString        = pregulador->rede->Codigo;
 }
 table->FieldByName("objeto")->AsInteger   = int(pregulador);
 //força gravaçao do registro
 table->Post();
 }
 }catch(Exception &e)
 {
 }
 //fecha a tabela
 if (table->Active) table->Close();
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelFluxoRede(void)
{
	// variáveis locais
	TList *lisREDE;
	VTRede *rede;
	int ind_pat = rel_fluxo.patamar->Index;
	VTResFlowRede *resflow;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelRede
	butRelRede->Down = true;
	try
	{ // reinicia tabela FluxoRede
		ReiniciaTabela("FluxoRede");
		// cria objeto VTResFlowRede
		resflow = NewObjResFlowRede(apl);
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "FluxoRede";
		table->Open();
		// atualiza tabela Barra
		lisREDE = redes->LisRede();
		// grava todos Eqptos
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			// define Rede p/ VTResFlowRede
			resflow->Rede = rede;
			// insere um registro na tabela
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("rede_id")->AsInteger = rede->Id;
			table->FieldByName("rede")->AsString = rede->Codigo;
			table->FieldByName("Psup_kW")->AsFloat = resflow->Psup_mw[ind_pat] * 1000;
			table->FieldByName("Qsup_kvar")->AsFloat = resflow->Qsup_mvar[ind_pat] * 1000;
			table->FieldByName("Pger_kW")->AsFloat = resflow->Pger_mw[ind_pat] * 1000;
			table->FieldByName("Qger_kvar")->AsFloat = resflow->Qger_mvar[ind_pat] * 1000;
			table->FieldByName("Pcar_kW")->AsFloat = resflow->Pcar_mw[ind_pat] * 1000;
			table->FieldByName("Qcar_kvar")->AsFloat = resflow->Qcar_mvar[ind_pat] * 1000;
			table->FieldByName("Qshunt_kvar")->AsFloat =
				(resflow->Qcap_mvar[ind_pat] + resflow->Qrea_mvar[ind_pat]) * 1000;
			table->FieldByName("Perda_kW")->AsFloat = resflow->Perda_kw[ind_pat] * 1000;
			// table->FieldByName("Perda_kvar")->AsFloat     = resflow->Perda_kvar[ind_pat] * 1000;
			table->FieldByName("Barras")->AsInteger = rede->NumeroDeBarras();
			table->FieldByName("Trechos")->AsInteger = rede->NumeroDeLigacoes(eqptoTRECHO);
			table->FieldByName("Suprimentos")->AsInteger = rede->NumeroDeEqbar(eqptoSUPRIMENTO);
			table->FieldByName("Cargas")->AsInteger = rede->NumeroDeEqbar(eqptoCARGA);
			// table->FieldByName("Capacitores")->AsInteger  = 0;
			// table->FieldByName("Reatores")->AsInteger     = 0;
            #ifdef _WIN64
			__int64 obj = __int64(rede);
			#endif

			#ifdef __WIN32__
			int obj = int(rede);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
		// destrói objeto VTResFlowRede
		delete resflow;
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelFluxoTrafo(void)
{
	// variáveis locais
	bool isolado;
	int ind_bar_pri, ind_bar_sec;
	VTBarra *bar_pri, *bar_sec;
	VTTrafo *ptrafo;
	VTResFlowLig *resflowlig;
	int ind_pat = rel_fluxo.patamar->Index;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelTrafo
	butRelTrafo->Down = true;
	try
	{ // reinicia tabela FluxoTrafo
		ReiniciaTabela("FluxoTrafo");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "FluxoTrafo";
		table->Open();
		// atualiza tabela Trechos
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoTRAFO);
		// grava todos Eqptos
		for (int n = 0; n < lisEQP->Count; n++)
		{
			ptrafo = (VTTrafo*)lisEQP->Items[n];
			resflowlig = ptrafo->resflow;
			// determina Barras do primário e do secundário
			bar_pri = ptrafo->BarraPrimario();
			bar_sec = ptrafo->BarraSecundario();
			ind_bar_pri = ptrafo->IndiceBarra(bar_pri);
			ind_bar_sec = ptrafo->IndiceBarra(bar_sec);
			// verifica se Eqpto isolado
			isolado = (bar_pri->Status[sttISOLADO] && bar_sec->Status[sttISOLADO]);
			// insere um registro na tabela
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("trafo_id")->AsInteger = ptrafo->Id;
			table->FieldByName("trafo")->AsString = ptrafo->Codigo;
			table->FieldByName("status")->AsString = (isolado) ? "desligado" : "ligado";
			table->FieldByName("snom_kva")->AsString = ptrafo->snom * 1000.;
			table->FieldByName("cargto_pu")->AsFloat = resflowlig->Cargto[ind_pat];
			// dados do primário
			table->FieldByName("pri_barra")->AsString = bar_pri->Codigo;
			table->FieldByName("pri_va_pu")->AsFloat = Abs(bar_pri->resflow->Van_pu[ind_pat]);
			table->FieldByName("pri_s_kva")->AsFloat =
				Abs(resflowlig->S_mva[ind_bar_pri][ind_pat]) * 1000.;
			table->FieldByName("pri_p_kw")->AsFloat =
				resflowlig->P_mw[ind_bar_pri][ind_pat] * 1000.;
			table->FieldByName("pri_q_kvar")->AsFloat =
				resflowlig->Q_mvar[ind_bar_pri][ind_pat] * 1000.;
			// dados do secundário
			table->FieldByName("sec_barra")->AsString = bar_sec->Codigo;
			table->FieldByName("sec_va_pu")->AsFloat = Abs(bar_sec->resflow->Van_pu[ind_pat]);
			table->FieldByName("sec_s_kva")->AsFloat =
				Abs(resflowlig->S_mva[ind_bar_sec][ind_pat]) * 1000.;
			table->FieldByName("sec_p_kw")->AsFloat =
				resflowlig->P_mw[ind_bar_sec][ind_pat] * 1000.;
			table->FieldByName("sec_q_kvar")->AsFloat =
				resflowlig->Q_mvar[ind_bar_sec][ind_pat] * 1000.;
			//
			table->FieldByName("queda_tensao_pu")->AsFloat = resflowlig->QuedaVfn_pu_max[ind_pat];
			table->FieldByName("Perda_kw")->AsFloat = resflowlig->Perda_mw[ind_pat] * 1000;
			// preenche dados da Rede
			if (ptrafo->rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = ptrafo->rede->Id;
				table->FieldByName("rede")->AsString = ptrafo->rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(ptrafo);
			#endif

			#ifdef __WIN32__
			int obj = int(ptrafo);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelFluxoTrecho(void)
{
	// variáveis locais
	bool isolado;
	int ind_bar1, ind_bar2;
	int compr_cod_arranjo = 58;
	double capacidade_mva;
	AnsiString cod_arranjo;
	VTBarra *bar1, *bar2;
	VTTrecho *trecho;
	VTResFlowLig *resflowlig;
	int ind_pat = rel_fluxo.patamar->Index;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelTrecho
	butRelTrecho->Down = true;
	try
	{ // reinicia tabela FluxoTrecho
		ReiniciaTabela("FluxoTrecho");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "FluxoTrecho";
		table->Open();
		// atualiza tabela Trechos
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoTRECHO);
		// grava todos Eqptos
		for (int n = 0; n < lisEQP->Count; n++)
		{
			trecho = (VTTrecho*)lisEQP->Items[n];
			resflowlig = trecho->resflow;
			// define Barra de referência como Barra de entrada do fluxo
			ind_bar1 = 0;
			if (resflowlig->SentidoP[ind_bar1][ind_pat] != fluxoPOS)
				ind_bar1 = 1;
			ind_bar2 = 1 - ind_bar1;
			bar1 = trecho->Barra(ind_bar1);
			bar2 = trecho->Barra(ind_bar2);
			// verifica se Eqpto isolado
			isolado = (trecho->pbarra1->Status[sttISOLADO] && trecho->pbarra2->Status[sttISOLADO]);
			// determina capacidade do trecho em MVA
			// capacidade_mva = trecho->arranjo->Capacidade(trecho->pbarra1->zona->Vfn_kv);
			capacidade_mva = CapacidadeTrechoMVA(trecho);
			// insere um registro na tabela
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campo de ID somente p/ Eqpto novo
			table->FieldByName("trecho_id")->AsInteger = trecho->Id;
			// atualiza campos
			table->FieldByName("trecho")->AsString = trecho->Codigo;
			table->FieldByName("status")->AsString = (isolado) ? "desligado" : "ligado";
			table->FieldByName("comprimento_m")->AsInteger = trecho->Comprimento_m;
			if (trecho->arranjo->Owner == trecho)
			{
				table->FieldByName("arranjo")->AsString = trecho->arranjo->TipoDeArranjo();
			}
			else
				// {table->FieldByName("arranjo")->AsString      = trecho->arranjo->Codigo;}
			{ // DVK 2016.06.16 verifica comprimento do nome do arranjo
				cod_arranjo = trecho->arranjo->Codigo;
				if (cod_arranjo.Length() > compr_cod_arranjo)
				{ // trunca código do arranjo
					cod_arranjo = cod_arranjo.SubString(0, compr_cod_arranjo - 1);
				}
				table->FieldByName("arranjo")->AsString =
					cod_arranjo + ": " + fases->AsStrABCN[trecho->arranjo->Fases];
			}
			table->FieldByName("fases")->AsString = fases->AsStrUpper[trecho->arranjo->Fases];
			table->FieldByName("capacidade_mva")->AsFloat = capacidade_mva;
			table->FieldByName("cargto_pu")->AsFloat = resflowlig->Cargto[ind_pat];
			// dados da Barra1
			table->FieldByName("bar1")->AsString = bar1->Codigo;
			table->FieldByName("bar1_va_pu")->AsFloat = Abs(bar1->resflow->Van_pu[ind_pat]);
			table->FieldByName("bar1_s_kva")->AsFloat =
				Abs(resflowlig->S_mva[ind_bar1][ind_pat]) * 1000.;
			table->FieldByName("bar1_p_kw")->AsFloat = resflowlig->P_mw[ind_bar1][ind_pat] * 1000.;
			table->FieldByName("bar1_q_kvar")->AsFloat =
				resflowlig->Q_mvar[ind_bar1][ind_pat] * 1000.;
			// dados da Barra2
			table->FieldByName("bar2")->AsString = bar2->Codigo;
			table->FieldByName("bar2_va_pu")->AsFloat = Abs(bar2->resflow->Van_pu[ind_pat]);
			table->FieldByName("bar2_s_kva")->AsFloat =
				Abs(resflowlig->S_mva[ind_bar2][ind_pat]) * 1000.;
			table->FieldByName("bar2_p_kw")->AsFloat = resflowlig->P_mw[ind_bar2][ind_pat] * 1000.;
			table->FieldByName("bar2_q_kvar")->AsFloat =
				resflowlig->Q_mvar[ind_bar2][ind_pat] * 1000.;
			// perda e queda de tensão
			table->FieldByName("Perda_kw")->AsFloat = resflowlig->Perda_mw[ind_pat] * 1000;
			table->FieldByName("queda_tensao_pu")->AsFloat = resflowlig->QuedaVfn_pu_max[ind_pat];
			// preenche dados da Rede
			if (trecho->rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = trecho->rede->Id;
				table->FieldByName("rede")->AsString = trecho->rede->Codigo;
			}
            #ifdef _WIN64
			__int64 obj = __int64(trecho);
			#endif

			#ifdef __WIN32__
			int obj = int(trecho);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelGerador(void)
{
	// variáveis locais
	AnsiString campo;
	VTGerador *pger;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelGerador
	butRelGerador->Down = true;
	try
	{ // reinicia tabela Gerador
		ReiniciaTabela("Gerador");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Gerador";
		table->Open();
		// atualiza tabela Gerador
		lisEQP->Clear();
		redes->LisEqbar(lisEQP, eqptoGERADOR);
		for (int ng = 0; ng < lisEQP->Count; ng++)
		{
			pger = (VTGerador*)lisEQP->Items[ng];
			rede = ExisteRede(pger->pbarra);
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("gerador_id")->AsInteger = pger->Id;
			table->FieldByName("gerador")->AsString = pger->Codigo;
			table->FieldByName("barra_id")->AsInteger = pger->pbarra->Id;
			table->FieldByName("barra")->AsString = pger->pbarra->Codigo;
			// table->FieldByName("curva_id")->AsInteger    = pger->curva->Id;
			table->FieldByName("TIPO")->AsString = TipoDeSuprimento(pger->tiposup);
			table->FieldByName("LIGACAO")->AsString = TipoDeLigacao(pger->ligacao);
			table->FieldByName("SNOM_kva")->AsFloat = pger->snom * 1000;
			table->FieldByName("VNOM_kv")->AsFloat = pger->vnom;
			table->FieldByName("fatpot")->AsFloat = pger->fplim;
			table->FieldByName("R0_pu")->AsFloat = pger->z0.r;
			table->FieldByName("X0_pu")->AsFloat = pger->z0.x;
			table->FieldByName("R1_pu")->AsFloat = pger->z1.r;
			table->FieldByName("X1_pu")->AsFloat = pger->z1.x;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(pger);
			#endif

			#ifdef __WIN32__
			int obj = int(pger);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelReator(void)
{
	// variáveis locais
	VTReator *reator;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelReator
	butRelReator->Down = true;
	try
	{ // reinicia tabela Reator
		ReiniciaTabela("Reator");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Reator";
		table->Open();
		// atualiza tabela Reator
		lisEQP->Clear();
		redes->LisEqbar(lisEQP, eqptoREATOR);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			reator = (VTReator*)lisEQP->Items[nc];
			rede = ExisteRede(reator->pbarra);
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("reator_id")->AsInteger = reator->Id;
			table->FieldByName("reator")->AsString = reator->Codigo;
			table->FieldByName("barra_id")->AsInteger = reator->pbarra->Id;
			table->FieldByName("barra")->AsString = reator->pbarra->Codigo;
			table->FieldByName("curva_id")->AsInteger = reator->curva->Id;
			table->FieldByName("curva")->AsString = reator->curva->Codigo;
			table->FieldByName("ligacao")->AsString = TipoDeLigacao(reator->ligacao);
			table->FieldByName("vnom_kv")->AsFloat = reator->vnom;
			table->FieldByName("q_kvar")->AsFloat = reator->q * 1000.;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(reator);
			#endif

			#ifdef __WIN32__
			int obj = int(reator);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelRegulador(void) /* DVK 2012.10.05 INICIO */
{
	// variáveis locais
	AnsiString txt = "Regulador";
	VTRegulador *regulador;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelRegulador
	butRelRegulador->Down = true;
	try
	{ // reinicia tabela Regulador
		ReiniciaTabela("Regulador");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Regulador";
		table->Open();
		// atualiza tabela Regulador
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoREGULADOR);
		for (int nc = 0; nc < lisEQP->Count; nc++)
		{
			regulador = (VTRegulador*)lisEQP->Items[nc];
			rede = ExisteRede(regulador->pbarra1);
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("regulador_id")->AsInteger = regulador->Id;
			table->FieldByName("regulador")->AsString = regulador->Codigo;
			table->FieldByName("modelo")->AsString = TraduzModeloRegulador(regulador->modelo);
			// table->FieldByName("ligacao")->AsInteger 		 =	regulador->ligacao;
			txt = TipoDeLigacao(regulador->ligacao) + ":" + fases->AsStrABCN[regulador->fases];
			table->FieldByName("ligacao")->AsString = txt;
			table->FieldByName("snom_kva")->AsFloat = regulador->snom * 1000.; // kVA
			table->FieldByName("by_pass")->AsString = regulador->by_pass ? "sim" : "não";
			table->FieldByName("ajuste_auto")->AsString = "";
			// regulador->ajuste_auto?"automático":"fixo"; /* TODO : reg */
			table->FieldByName("v_pu")->AsFloat = regulador->cfg_auto.v_pu;
			table->FieldByName("dv_pu")->AsFloat = regulador->cfg_auto.dv_pu;
			table->FieldByName("passo")->AsInteger = regulador->cfg_fixo.passo;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
            #ifdef _WIN64
			__int64 obj = __int64(regulador);
			#endif

			#ifdef __WIN32__
			int obj = int(regulador);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}
/* DVK 2012.10.05 FIM */

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelPerfilCarga(void)
{
	// variáveis locais
	int num_carga;
	TFase *fase;
	VTRede *rede;
	VTClasse *classe;
	TList *lisFASE;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCLASSE = classes->LisClasse();
	TList *lisREDE = redes->LisRede();

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelPerfilCarga
	butRelPerfilCarga->Down = true;
	try
	{ // reinicia tabela
		ReiniciaTabela("PerfilCarga");
		// reinicia o objeto TADOTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "PerfilCarga";
		table->Open();
		// loop p/ todas Redes carregadas
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			if (!rede->Carregada)
				continue;
			// obtém lista de Cargas da Rede
			lisEQP->Clear();
			rede->LisEqbar(lisEQP, eqptoCARGA);
			if (lisEQP->Count == 0)
				continue;
			// loop p/ toda Classe de consumidor
			for (int nc = 0; nc < lisCLASSE->Count; nc++)
			{
				classe = (VTClasse*)lisCLASSE->Items[nc];
				// loop p/ todos tipos de Carga: monofásica, bifásica, trifásica
				for (int carMBT = cargaMONOFASICA; carMBT <= cargaTRIFASICA; carMBT++)
				{ // obtém lista de Fase p/ tipo de carga
					lisFASE = fases->LisFasesCarga(carMBT);
					// loop p/ todas Fases de carga
					for (int nf = 0; nf < lisFASE->Count; nf++)
					{
						fase = (TFase*)lisFASE->Items[nf];
						if ((num_carga = NumeroDeCarga(lisEQP, classe, fase)) == 0)
							continue;
						// insere novo registro e habilita edição
						table->Append();
						table->Edit();
						// atualiza campos
						table->FieldByName("rede_id")->AsInteger = rede->Id;
						table->FieldByName("rede")->AsString = rede->Codigo;
						table->FieldByName("classe")->AsString = classe->Codigo;
						table->FieldByName("fases")->AsString = fase->codigo;
						table->FieldByName("consumidores")->AsInteger = num_carga;
						// força gravaçao do registro
                        #ifdef _WIN64
						__int64 obj = __int64(rede);
						#endif

						#ifdef __WIN32__
						int obj = int(rede);
						#endif
						table->FieldByName("objeto")->AsString = IntToStr(obj);
						table->Post();
					}
				}
			}
		}
	}
	catch (Exception &e)
	{
	}
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelRede(void)
{
	// variaveis locais
	struct
	{
		int rede, barra, trecho, trafo, chave, regulador, carga, capacitor, gerador, suprimento;
	} parcial, total;
	TList *lisREDE;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelRede
	butRelRede->Down = true;
	try
	{ // inicia totais dos eqptos
		total.rede = 0;
		total.barra = 0;
		total.trecho = 0;
		total.trafo = 0;
		total.chave = 0;
		total.regulador = 0;
		total.carga = 0;
		total.capacitor = 0;
		total.gerador = 0;
		total.suprimento = 0;
		// reinicia tabela Rede
		ReiniciaTabela("Rede");
		// reinicia o objeto TADOTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Rede";
		table->Open();
		// atualiza Redes
		lisREDE = redes->LisRede();
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			// insere novo registro e habilita edição
			table->Append();
			table->Edit();
			// atualiza campos
			if (rede->Carregada)
			{
				table->FieldByName("REDE")->AsString = rede->Codigo;
			}
			else
			{
				table->FieldByName("REDE")->AsString = rede->Codigo + "(*)";
			}
			table->FieldByName("TIPO_REDE")->AsString = rede->TipoRede->Codigo;
			if (rede->Cluster)
			{
				table->FieldByName("AGRUPADA")->AsString = rede->Cluster->Codigo;
			}
			table->FieldByName("RADIAL")->AsString = (rede->Radial) ? "Sim" : "Não";
			table->FieldByName("ESTATICA")->AsString = (rede->Estatica) ? "Sim" : "Não";
			if (rede->Carregada)
			{ // número de Eqptos
				table->FieldByName("BARRA")->AsInteger = parcial.barra = rede->NumeroDeBarras();
				table->FieldByName("TRECHO")->AsInteger = parcial.trecho =
					rede->NumeroDeLigacoes(eqptoTRECHO);
				table->FieldByName("TRAFO")->AsInteger = parcial.trafo =
					rede->NumeroDeLigacoes(eqptoTRAFO);
				table->FieldByName("CHAVE")->AsInteger = parcial.chave =
					rede->NumeroDeLigacoes(eqptoCHAVE);
				table->FieldByName("REGULADOR")->AsInteger = parcial.regulador =
					rede->NumeroDeLigacoes(eqptoREGULADOR);
				table->FieldByName("CARGA")->AsInteger = parcial.carga =
					rede->NumeroDeEqbar(eqptoCARGA);
				table->FieldByName("CAPACITOR")->AsInteger = parcial.capacitor =
					rede->NumeroDeEqbar(eqptoCAPACITOR);
				table->FieldByName("GERADOR")->AsInteger = parcial.gerador =
					rede->NumeroDeEqbar(eqptoGERADOR);
				table->FieldByName("SUPRIMENTO")->AsInteger = parcial.suprimento =
					rede->NumeroDeEqbar(eqptoSUPRIMENTO);
				// totaliza Eqptos
				total.rede += 1;
				total.barra += parcial.barra;
				total.trecho += parcial.trecho;
				total.trafo += parcial.trafo;
				total.chave += parcial.chave;
				total.regulador += parcial.regulador;
				total.carga += parcial.carga;
				total.capacitor += parcial.capacitor;
				total.gerador += parcial.gerador;
				total.suprimento += parcial.suprimento;
			}
			// IDs
			table->FieldByName("REDE_ID")->AsInteger = rede->Id;
			table->FieldByName("TIPO_REDE_ID")->AsInteger = rede->TipoRede->Id;
			// ponteiro p/ objeto
            #ifdef _WIN64
			__int64 obj = __int64(rede);
			#endif

			#ifdef __WIN32__
			int obj = int(rede);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
		// inclui registro com total de Eqptos
		table->Append();
		table->Edit();
		// atualiza campos
		table->FieldByName("REDE")->AsString = "Total: " + IntToStr(total.rede);
		table->FieldByName("TIPO_REDE")->AsString = "";
		table->FieldByName("AGRUPADA")->AsString = "";
		table->FieldByName("RADIAL")->AsString = "";
		table->FieldByName("ESTATICA")->AsString = "";
		table->FieldByName("BARRA")->AsInteger = total.barra;
		table->FieldByName("TRECHO")->AsInteger = total.trecho;
		table->FieldByName("TRAFO")->AsInteger = total.trafo;
		table->FieldByName("CHAVE")->AsInteger = total.chave;
		table->FieldByName("REGULADOR")->AsInteger = total.regulador;
		table->FieldByName("CARGA")->AsInteger = total.carga;
		table->FieldByName("CAPACITOR")->AsInteger = total.capacitor;
		table->FieldByName("GERADOR")->AsInteger = total.gerador;
		table->FieldByName("SUPRIMENTO")->AsInteger = total.suprimento;
		// IDs
		table->FieldByName("REDE_ID")->AsInteger = 0;
		table->FieldByName("TIPO_REDE_ID")->AsInteger = 0;
		// ponteiro p/ objeto
		table->FieldByName("objeto")->AsString = "0";
		// força gravaçao do registro
		table->Post();
	}
	catch (Exception &e)
	{
	}
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelSuprimento(void)
{
	// variáveis locais
	AnsiString campo;
	VTSuprimento *psup;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelSuprimento
	butRelSuprimento->Down = true;
	try
	{ // reinicia tabela Suprimento
		ReiniciaTabela("Suprimento");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Suprimento";
		table->Open();
		// atualiza tabela Suprimento
		lisEQP->Clear();
		redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
		for (int ns = 0; ns < lisEQP->Count; ns++)
		{
			psup = (VTSuprimento*)lisEQP->Items[ns];
			rede = ExisteRede(psup->pbarra);
			// insere um registro na tabela
			table->Filtered = false;
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("suprimento_id")->AsInteger = psup->Id;
			table->FieldByName("suprimento")->AsString = psup->Codigo;
			table->FieldByName("barra_id")->AsInteger = psup->pbarra->Id;
			table->FieldByName("barra")->AsString = psup->pbarra->Codigo;
			// table->FieldByName("curva_id")->AsInteger       = psup->curva->Id;
			table->FieldByName("TIPO")->AsString = TipoDeSuprimento(psup->tiposup);
			table->FieldByName("Smax_mva")->AsFloat = psup->smax;
			table->FieldByName("PCC_FT_mw")->AsFloat = psup->pcc_ft.p;
			table->FieldByName("QCC_FT_mvar")->AsFloat = psup->pcc_ft.q;
			table->FieldByName("PCC_3F_mw")->AsFloat = psup->pcc_3f.p;
			table->FieldByName("QCC_3F_mvar")->AsFloat = psup->pcc_3f.q;
			// table->FieldByName("R0_EQV_pu")->AsFloat        = psup->zeq0.r;
			// table->FieldByName("X0_EQV_pu")->AsFloat        = psup->zeq0.x;
			// table->FieldByName("R1_EQV_pu")->AsFloat        = psup->zeq1.r;
			// table->FieldByName("X1_EQV_pu")->AsFloat        = psup->zeq1.x;
			// preenche dados da Rede
			if (rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = rede->Id;
				table->FieldByName("rede")->AsString = rede->Codigo;
			}
            #ifdef _WIN64
			__int64 obj = __int64(psup);
			#endif

			#ifdef __WIN32__
			int obj = int(psup);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelTrafo(void)
{
	// variáveis locais
	AnsiString txt = "Trafo";
	AnsiString tabela = "Trafo";
	double defasagem_grau;
	VTTrafo *ptrafo;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelTrafo
	butRelTrafo->Down = true;
	try
	{ // reinicia tabela Trafo
		ReiniciaTabela(tabela);
		// reinicia o objeto TADOTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = tabela;
		table->Open();
		// atualiza tabela Trafo
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoTRAFO);
		for (int n = 0; n < lisEQP->Count; n++)
		{
			ptrafo = (VTTrafo*)lisEQP->Items[n];
			defasagem_grau = ptrafo->DefasagemPriSec();
			// insere novo registro e habilita edição
			table->Append();
			table->Edit();
			// atualiza campos
			table->FieldByName("trafo_id")->AsInteger = ptrafo->Id;
			table->FieldByName("trafo")->AsString = ptrafo->Codigo;
			table->FieldByName("barra1_id")->AsInteger = ptrafo->pbarra1->Id;
			table->FieldByName("barra1")->AsString = ptrafo->pbarra1->Codigo;
			table->FieldByName("barra2_id")->AsInteger = ptrafo->pbarra2->Id;
			table->FieldByName("barra2")->AsString = ptrafo->pbarra2->Codigo;
			table->FieldByName("snom_kva")->AsFloat = ptrafo->snom * 1000.; // kVA
			if (ptrafo->Monofasico)
			{
				txt = TipoDeLigacao(ptrafo->pri.ligacao) + ":" +
					fases->AsStrABCN[ptrafo->pri.fase] + "/" + fases->AsStrABCN[ptrafo->sec.fase];
				table->FieldByName("ligacao")->AsString = txt;
				table->FieldByName("rotacao_fase")->AsFloat = 0.;
			}
			else
			{
				txt = TipoDeLigacao(ptrafo->pri.ligacao) + TipoDeLigacao(ptrafo->sec.ligacao);
				table->FieldByName("ligacao")->AsString = txt;
				table->FieldByName("rotacao_fase")->AsFloat = defasagem_grau;
			}
			table->FieldByName("perda_ferro")->AsFloat = ptrafo->perda_ferro;
			// primário
			table->FieldByName("pri_vnom_kv")->AsFloat = ptrafo->pri.vnom;
			table->FieldByName("pri_tap_kv")->AsFloat = ptrafo->pri.tap;
			table->FieldByName("pri_ater_r")->AsFloat = ptrafo->pri.zterra.r;
			table->FieldByName("pri_ater_x")->AsFloat = ptrafo->pri.zterra.x;
			// secundário
			table->FieldByName("sec_vnom_kv")->AsFloat = ptrafo->sec.vnom;
			table->FieldByName("sec_tap_kv")->AsFloat = ptrafo->sec.tap;
			table->FieldByName("sec_ater_r")->AsFloat = ptrafo->sec.zterra.r;
			table->FieldByName("sec_ater_x")->AsFloat = ptrafo->sec.zterra.x;
			// impedâncias de seq
			table->FieldByName("r0_pu")->AsFloat = ptrafo->z0.r;
			table->FieldByName("x0_pu")->AsFloat = ptrafo->z0.x;
			table->FieldByName("r1_pu")->AsFloat = ptrafo->z1.r;
			table->FieldByName("x1_pu")->AsFloat = ptrafo->z1.x;
			// ltc
			if (ptrafo->ltc.ajuste_auto.bar_ref == NULL)
			{
				table->FieldByName("ltc_ajuste")->AsString = "tap fixo";
				table->FieldByName("ltc_barra")->AsString = "";
				// table->FieldByName("ltc_vpu")->AsFloat    = 0;
			}
			else
			{
				table->FieldByName("ltc_ajuste")->AsString = (ptrafo->ltc.tipo == ltcPRIMARIO) ?
					"primário" : "secundário";
				table->FieldByName("ltc_barra")->AsString = ptrafo->ltc.ajuste_auto.bar_ref->Codigo;
				table->FieldByName("ltc_v_pu")->AsFloat = ptrafo->ltc.ajuste_auto.v_pu;
			}
			// preenche dados da Rede
			if (ptrafo->rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = ptrafo->rede->Id;
				table->FieldByName("rede")->AsString = ptrafo->rede->Codigo;
			}
			#ifdef _WIN64
			__int64 obj = __int64(ptrafo);
			#endif

			#ifdef __WIN32__
			int obj = int(ptrafo);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha tabelas
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelTrafo3E(void)
{
	// variáveis locais
	VTTrafo3E *trafo3E;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelTrafo3E
	butRelTrafo3E->Down = true;
	try
	{ // reinicia tabela Trafo3E
		ReiniciaTabela("Trafo3E");
		// proteção: reinicia o objeto TTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = "Trafo3E";
		table->Open();
		// atualiza tabela Trafo3E
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoTRAFO3E);
		for (int n = 0; n < lisEQP->Count; n++)
		{
			trafo3E = (VTTrafo3E*)lisEQP->Items[n];
			// insere um registro na tabela
			table->Append();
			// habilita edição
			table->Edit();
			// atualiza campos
			table->FieldByName("trafo3e_id")->AsInteger = trafo3E->Id;
			table->FieldByName("rede_id")->AsInteger = trafo3E->rede->Id;
			table->FieldByName("barra1_id")->AsInteger = trafo3E->pbarra1->Id;
			table->FieldByName("barra1")->AsString = trafo3E->pbarra1->Codigo;
			table->FieldByName("barra2_id")->AsInteger = trafo3E->pbarra2->Id;
			table->FieldByName("barra2")->AsString = trafo3E->pbarra2->Codigo;
			table->FieldByName("barra3_id")->AsInteger = trafo3E->pbarra3->Id;
			table->FieldByName("barra3")->AsString = trafo3E->pbarra3->Codigo;
			table->FieldByName("trafo3e")->AsString = trafo3E->Codigo;
			table->FieldByName("perda_ferro")->AsFloat = trafo3E->perda_ferro;
			// primário
			table->FieldByName("pri_ligacao")->AsString = TipoDeLigacao(trafo3E->pri.ligacao);
			table->FieldByName("pri_rotfase")->AsFloat = trafo3E->pri.defasagem;
			table->FieldByName("pri_snom_kva")->AsFloat = trafo3E->pri.snom * 1000.;
			table->FieldByName("pri_vnom_kv")->AsFloat = trafo3E->pri.vnom;
			table->FieldByName("pri_tap_kv")->AsFloat = trafo3E->pri.tap;
			table->FieldByName("pri_ater_r")->AsFloat = trafo3E->pri.zterra.r;
			table->FieldByName("pri_ater_x")->AsFloat = trafo3E->pri.zterra.x;
			// secundário
			table->FieldByName("sec_ligacao")->AsString = TipoDeLigacao(trafo3E->sec.ligacao);
			table->FieldByName("sec_rotfase")->AsFloat = trafo3E->sec.defasagem;
			table->FieldByName("sec_snom_kva")->AsFloat = trafo3E->sec.snom * 1000.;
			table->FieldByName("sec_vnom_kv")->AsFloat = trafo3E->sec.vnom;
			table->FieldByName("sec_tap_kv")->AsFloat = trafo3E->sec.tap;
			table->FieldByName("sec_ater_r")->AsFloat = trafo3E->sec.zterra.r;
			table->FieldByName("sec_ater_x")->AsFloat = trafo3E->sec.zterra.x;
			// tercário
			table->FieldByName("ter_ligacao")->AsString = TipoDeLigacao(trafo3E->ter.ligacao);
			table->FieldByName("ter_rotfase")->AsFloat = trafo3E->ter.defasagem;
			table->FieldByName("ter_snom_kva")->AsFloat = trafo3E->ter.snom * 1000.;
			table->FieldByName("ter_vnom_kv")->AsFloat = trafo3E->ter.vnom;
			table->FieldByName("ter_tap_kv")->AsFloat = trafo3E->ter.tap;
			table->FieldByName("ter_ater_r")->AsFloat = trafo3E->ter.zterra.r;
			table->FieldByName("ter_ater_x")->AsFloat = trafo3E->ter.zterra.x;
			// impedâncias de seq
			table->FieldByName("r0_ps_pu")->AsFloat = trafo3E->zps0.r;
			table->FieldByName("x0_ps_pu")->AsFloat = trafo3E->zps0.x;
			table->FieldByName("r1_ps_pu")->AsFloat = trafo3E->zps1.r;
			table->FieldByName("x1_ps_pu")->AsFloat = trafo3E->zps1.x;
			table->FieldByName("r0_pt_pu")->AsFloat = trafo3E->zpt0.r;
			table->FieldByName("x0_pt_pu")->AsFloat = trafo3E->zpt0.x;
			table->FieldByName("r1_pt_pu")->AsFloat = trafo3E->zpt1.r;
			table->FieldByName("x1_pt_pu")->AsFloat = trafo3E->zpt1.x;
			table->FieldByName("r0_st_pu")->AsFloat = trafo3E->zst0.r;
			table->FieldByName("x0_st_pu")->AsFloat = trafo3E->zst0.x;
			table->FieldByName("r1_st_pu")->AsFloat = trafo3E->zst1.r;
			table->FieldByName("x1_st_pu")->AsFloat = trafo3E->zst1.x;
			// preenche dados da Rede
			if (trafo3E->rede != NULL)
			{
				table->FieldByName("rede")->AsString = trafo3E->rede->Codigo;
				table->FieldByName("rede_id")->AsInteger = trafo3E->rede->Id;
			}
            #ifdef _WIN64
			__int64 obj = __int64(trafo3E);
			#endif

			#ifdef __WIN32__
			int obj = int(trafo3E);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha a tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15971807
void __fastcall TFormRelEqpto::PreencheRelTrecho(void)
{
	// variáveis locais
	double r0, x0, b0;
	double r1, x1, b1;
	int compr_cod_arranjo = 58;
	AnsiString tabela = "Trecho";
	AnsiString campo, cod_arranjo;
	VTTrecho *trecho;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// FKM 2016.04.05 - progresso start
	progresso->Start(progDEFAULT);
	// 'abaixa' butRelTrecho
	butRelTrecho->Down = true;
	try
	{ // reinicia tabela Trafo
		ReiniciaTabela(tabela);
		// reinicia o objeto TADOTable
		table->Close();
		table->Filtered = false;
		table->Filter = "";
		table->TableName = tabela;
		table->Open();
		// atualiza tabela Trecho
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoTRECHO);
		for (int n = 0; n < lisEQP->Count; n++)
		{
			trecho = (VTTrecho*)lisEQP->Items[n];
			// obtém valores de impedância em ohm
			trecho->Z0_ohm(r0, x0, b0);
			trecho->Z1_ohm(r1, x1, b1);
			// insere novo registro e habilita edição
			table->Append();
			table->Edit();
			// atualiza campos
			table->FieldByName("trecho_id")->AsInteger = trecho->Id;
			table->FieldByName("trecho")->AsString = trecho->Codigo;
			table->FieldByName("barra1_id")->AsInteger = trecho->pbarra1->Id;
			table->FieldByName("barra1")->AsString = trecho->pbarra1->Codigo;
			table->FieldByName("barra2_id")->AsInteger = trecho->pbarra2->Id;
			table->FieldByName("barra2")->AsString = trecho->pbarra2->Codigo;
			table->FieldByName("comprimento_m")->AsFloat = trecho->Comprimento_m;
			if (trecho->ArranjoTipico)
			{
				table->FieldByName("arranjo_id")->AsInteger = trecho->arranjo->Id;
				// DVK 2016.06.16 verifica comprimento do nome do arranjo
				cod_arranjo = trecho->arranjo->Codigo;
				if (cod_arranjo.Length() > compr_cod_arranjo)
				{ // trunca código do arranjo
					cod_arranjo = cod_arranjo.SubString(0, compr_cod_arranjo - 1);
				}
				table->FieldByName("arranjo")->AsString =
					cod_arranjo + ": " + fases->AsStrABCN[trecho->arranjo->Fases];
			}
			else
			{
				table->FieldByName("arranjo")->AsString =
					"próprio: " + fases->AsStrABCN[trecho->arranjo->Fases];
			}
			// inclui impedâncias do Arranjo
			table->FieldByName("r0_ohm")->AsFloat = r0;
			table->FieldByName("x0_ohm")->AsFloat = x0;
			table->FieldByName("b0_s")->AsFloat = b0;
			table->FieldByName("r1_ohm")->AsFloat = r1;
			table->FieldByName("x1_ohm")->AsFloat = x1;
			table->FieldByName("b1_s")->AsFloat = b1;
			// preenche dados da Rede
			if (trecho->rede != NULL)
			{
				table->FieldByName("rede_id")->AsInteger = trecho->rede->Id;
				table->FieldByName("rede")->AsString = trecho->rede->Codigo;
			}
            #ifdef _WIN64
			__int64 obj = __int64(trecho);
			#endif

			#ifdef __WIN32__
			int obj = int(trecho);
			#endif
			table->FieldByName("objeto")->AsString = IntToStr(obj);
			// força gravaçao do registro
			table->Post();
		}
	}
	catch (Exception &e)
	{
	}
	// fecha tabela
	if (table->Active)
		table->Close();
	// FKM 2016.04.05 - progresso stop
	progresso->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ReiniciaTabela(AnsiString tab_name)
{
	// reinicia ADOQuery
	query->Close();
	query->SQL->Clear();
	query->SQL->Add("DELETE * FROM " + tab_name);
	query->ExecSQL();
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::TipoDeLigacao(int tipo_lig)
{
	switch (tipo_lig)
	{
	case lgEST_AT:
		return ("Yat");
	case lgTRIANG:
		return ("D");
	case lgEST_ISO:
		return ("Y");
	case lgMONOFASICO:
		return ("Mono");
	case lgDELTA_ABERTO:
		return ("Delta Aberto");
	case lgDELTA_FECHADO:
		return ("Delta Fechado");
	}
	return ("inválida");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::TipoDeSuprimento(int suprimento)
{
	switch (suprimento)
	{
	case supPV:
		return ("PV");
	case supPQ:
		return ("PQ");
	case supVF:
		return ("VF");
	}
	return ("Inválida");
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelEqpto::ToolBarRedefineAction(bool rel_eqpto)
{
	// variáveis locais
	TToolButton *but_down;

	// salva botão "down"
	for (int n = 0; n < ToolBar->ButtonCount; n++)
	{
		if (ToolBar->Buttons[n]->Down)
		{
			but_down = ToolBar->Buttons[n];
			break;
		}
	}
	if (rel_eqpto)
	{
		butRelBarra->Action = ActionRelBarra;
		butRelCapacitor->Action = ActionRelCapacitor;
		butRelCarga->Action = ActionRelCarga;
		butRelChave->Action = ActionRelChave;
		butRelGerador->Action = ActionRelGerador;
		butRelPerfilCarga->Action = ActionRelPerfilCarga;
		butRelReator->Action = ActionRelReator;
		butRelRegulador->Action = ActionRelRegulador;
		butRelRede->Action = ActionRelRede;
		butRelSuprimento->Action = ActionRelSuprimento;
		butRelTrafo->Action = ActionRelTrafo;
		butRelTrafo3E->Action = ActionRelTrafo3E;
		butRelTrecho->Action = ActionRelTrecho;
	}
	else
	{
		butRelBarra->Action = ActionRelFluxoBarra;
		butRelCapacitor->Action = ActionRelFluxoCapacitor;
		butRelCarga->Action = ActionRelFluxoCarga;
		butRelChave->Action = ActionRelFluxoChave;
		butRelGerador->Action = ActionRelFluxoGerador;
		butRelPerfilCarga->Action = ActionRelFluxoPerfilCarga;
		butRelReator->Action = ActionRelFluxoReator;
		butRelRegulador->Action = ActionRelRegulador; // ActionRelFluxoRegulador;
		butRelRede->Action = ActionRelFluxoRede;
		butRelSuprimento->Action = ActionRelFluxoSuprimento;
		butRelTrafo->Action = ActionRelFluxoTrafo;
		butRelTrafo3E->Action = ActionRelFluxoTrafo3E;
		butRelTrecho->Action = ActionRelFluxoTrecho;
	}
	// redefine botão como "down"
	but_down->Down = true;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelEqpto::TraduzModeloRegulador(int tipo)
{
	switch (tipo)
	{
	case reguFIXO:
		return ("Tap fixo");
	case reguAUTO_UNI:
		return ("Tap automático unidirecional");
	case reguAUTO_BI:
		return ("Tap automático bidirecional");
	case reguAUTO_FIXA:
		return ("Tap automático barra fixa");
	case reguENERQCT:
		return ("Regulador BT");
	default:
		return ("Indefinido");
	}
}
// ---------------------------------------------------------------------------
// eof
