// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormDemanda.h"
#include "TFormCfgDemanda.h"
#include "TFormConfirmaMedicao.h"
#include "TFormInsereMedicao.h"
#include "TFormLogAjustaDemanda.h"
#include "..\TBackup.h"
#include "..\VTAjusteFatores.h"
#include "..\VTAjusteFator.h"
#include "..\VTAjusta.h"
#include "..\Apl\VTApl.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Bloco\VTElo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEditor.h"
#include "..\Flow\VTFlow.h"
#include "..\Forms\TFormEdt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\ImportaMedicao\VTImportaMedicao.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\Mercado\VTMercado.h"
#include "..\Obra\VTObra.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Radial\VTRadial.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResflowlig.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrafo.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\ImportaMedicao.h"
#include "..\..\DLL_Inc\Medidor.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormDemanda::TFormDemanda(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
	: TFormEdt(Owner, parent)
{
	VTProgresso *progresso;

	try
	{
		// insere Form em seu Parent
		if (parent)
		{
			Parent = parent;
		}
		// cria StringList p/ arquivos de medição
		files = new TStringList();
		// cria objeto Apl próprio
		apl = NewObjApl(this, apl_owner);
		//verifica se possui progresso
		progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso == NULL)
		{
			apl->Add(DLL_NewObjProgresso(this, NULL));
		}
		// cria lista local
		lisEQP = new TList();
		// seleciona tipo de ajuste (por fase ou trifásico) em função da topologia da
		// rede e cria objeto ajusta_demanda
		CriaObjAjusta();
		// indica que haverá barra de progresso
		ajusta_demanda->ProgressoEnabled = true;
		if (!ajusta_demanda->IniciaDados(this))
		{
			Close();
			return;
		}
		// insere primarios no CBoxPrimario e medidores no LView
		CBoxPrimarioInicia();
		LViewMedidoresInicia();
		// torna todos os Medidores visíveis
		ExibeMedidores(true);
		// le as configs do tipo de ajuste
		CriaFatores();
		fatorAtivo = NULL;
		// CBoxRedesFatorInicia();
		LeFatores();
		// posiciona o Form
		FormIniciaPosicao();
		// configura o ajusta de acordo com o tipo de ajuste
		AlteraTipoAjuste();
		ConfiguraForm();
		Show();
	}
	catch (Exception &e)
	{ // para progresso
		// if(prog != NULL) prog->Stop();
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormDemanda::~TFormDemanda(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTChave *chave = NULL;
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// exibe gráfico de topologia
	sinap_child->ActionTopo->Execute();
	// destroi form medidor (se tiver)
	DLL_CloseFormMedidor(this);
	// torna todos os Medidores invisíveis
	ExibeMedidores(false);
	// limpa moldura
	Moldura(chave);
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói objetos
	if (ajusta_demanda)
	{
		delete ajusta_demanda;
		ajusta_demanda = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	if (files)
	{
		delete files;
		files = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18381986
void __fastcall TFormDemanda::ActionAjustaDemandaExecute(TObject *Sender)
{
	// variáveis locais
	bool med_neg_enabled;
	bool ajustou = false;
	// int val_medicao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	Hide();
	try
	{ // verifica, pela topologia da rede, se podem haver medições negativas,
		// isto é, se existe gerador na rede
		med_neg_enabled = ExisteGeracao();
		// salva os fatores
		SalvaFatores();
		// verifica se ajuste com fator único de correção
		if (ajusta_demanda->AjusteJaExecutado)
		{ // pergunta se o ajuste deve ser feito a partir da rede original
			switch (Confirma("A rede já foi ajustada.",
				"O novo ajuste deve ser feito a partir da rede original ?"))
			{
			case IDNO: // nada a fazer
				break;
			case IDYES: // restaura cargas originais
				ajusta_demanda->RestauraCargasOriginais();
				// recalcula fluxo
				ajusta_demanda->CalculaFluxo(true);
				// reinicia medidores (curva estimada)
				medicoes->AtualizaMedidores();
				break;
			case IDCANCEL:
                Show();
				return;
			}
		}
		// caso nao use medicao
		if (ajusta_demanda->PorMedicao == true)
		{
			// inicia processo, validando medições e dados
			if (!medicoes->Valida(ajusta_demanda->LimitaAjuste, ajusta_demanda->AjusteMaximo))
			{
				Aviso("Não há medições válidas.");
				Show();
				return;
			}
            // inicia processo, validando medições e dados
			if (medicoes->LisMedicaoValida(med_neg_enabled)->Count == 0)
			{
				Aviso("Não há medições.");
				Show();
				return;
			}
		}
		// atualiza variável de controle de ajuste executado
		ajusta_demanda->AjusteJaExecutado = true;
		// executa ajuste da demanda
		ajustou = ajusta_demanda->Executa(med_neg_enabled);
		// gera mensagem p/ usuário
		if (ajustou)
		{ // teste
			LViewMedidoresInicia();
			// exibe resultados
			ExibeResultadoAjuste();
			// remove as medicoes ficticias
			ajusta_demanda->RemoveMedicoesFicticias();
			// atualiza valores estimados dos medidores
			medicoes->AtualizaMedidores();
			LViewMedidoresInicia();
		}
		else
		{ // exibe diagnóstico, se existir
			if (!ajusta_demanda->Diagnostico.IsEmpty())
				Aviso(ajusta_demanda->Diagnostico);
			// atualiza variável de controle de ajuste executado
			ajusta_demanda->AjusteJaExecutado = false;
		}
		// retira chaves ficticias
		// medicoes->RemoveChavesFic();
		// FKM- 2017.12.05 não é necessário pois no destrutor do tajusta ele da um undocompleto
		// atualiza gráfico
		grafico->Redraw();
	}
	catch (Exception &e)
	{ // mostra erro
		Erro("Erro no ajuste de demanda");
	}
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionCfgAjusteExecute(TObject *Sender)
{
	// variaveis locais
	TFormCfgDemanda *form;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	// configura parâmetros para ajuste de demanda
	Hide();
	form = new TFormCfgDemanda(NULL, ajusta_demanda);
	if (form->ShowModal() == mrOk)
	{ // atualiza medições, se a opção de fluxo foi alterada
		if (form->opFluxoAlterado)
		{
			if (flow)
				flow->UpToDate = false;
			ajusta_demanda->CalculaFluxo(true);
			medicoes->AtualizaMedidores();
			//LViewMedidoresInicia();
		}
		LViewMedidoresInicia();
	}
	Show();
	delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionEdtExecute(TObject *Sender)
{
	// variaveis locais
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// editor de alterações
	// editor->EditaAltera(NULL, NULL, ActionRedeChanged);
	editor->EditaAltera(ActionMouseOff, ActionMouseOn, ActionRedeChanged);
	AtualizaIndicacaoEditorOn(butEdt);
	ButEdtMedicao->Down = (!butEdt->Down);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionEdtMedicaoExecute(TObject *Sender)
{
	// variaveis locais
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// editor de Medições
	// editor->EditaMedicao(NULL, NULL, ActionRedeChanged);
	editor->EditaMedicao(ActionMouseOff, ActionMouseOn, ActionRedeChanged);
	AtualizaIndicacaoEditorOn(ButEdtMedicao);
	butEdt->Down = (!ButEdtMedicao->Down);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionFechaExecute(TObject *Sender)
{
	// fecha este Form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Ajuste_de_Demanda");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionImportaMedicaoExecute(TObject *Sender)
{
	// seleciona arquivo de medição
	if (!SelecionaArquivo())
		return;
	// importa arquivo de medição
	if (ImportaMedicao())
	{ // atualiza medições
		LViewMedidoresInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionInserirMedicaoExecute(TObject *Sender)
{
	// variáveis locais
	TForm *form_insere;

	// esconde form
	Hide();
	try
	{ // cria form
		form_insere = new TFormInsereMedicao(NULL, apl, ajusta_demanda);
		if (form_insere->ShowModal() == mrOk)
		{ // atualiza medidores e lview
			ActionRedeChangedExecute(NULL);
		}
		//destroi form
		delete form_insere;
	}
	catch (Exception &e)
	{
		if (form_insere)
			delete form_insere;
	}
	// reapresenta este form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionLogAjustaExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString arquivo;
	TFormLogAjustaDemanda *form;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// verifica se há algum arquivo com o último log de ajuste
	arquivo = path->DirTmp() + "\\LogAjuste.txt";
	if (!FileExists(arquivo))
	{
		Aviso("Nenhum log de ajuste de demanda foi encontrado.");
		return;
	}
	// cria form
	if ((form = new TFormLogAjustaDemanda(this, apl)) != NULL)
	{ // mostra se as redes foram ajustadas e o que houve com as que não foram
		Hide();
		form->Importa(arquivo);
		if (form->ShowModal() == mrOk)
			delete form;
		Show();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ActionRedeChangedExecute(TObject *Sender)
{
	// variáveis locais
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	// inicia medidores (curva estimada)
	medicoes->AtualizaMedidores();
	// atualiza lista de medidores
	LViewMedidoresInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::AlteraTipoAjuste(void)
{
	// age de acordo com o radiobutton
	if (radioMedicao->Checked == true)
	{
		ajusta_demanda->PorMedicao = true;
	}
	else
	{
		ajusta_demanda->PorMedicao = false;
	}
	// configura o form
	// ConfiguraForm();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::CBoxPrimarioChange(TObject *Sender)
{
	// variáveis locais
	VTRede *rede;

	// proteção
	if (CBoxPrimario->ItemIndex < 0)
		return;
	// verifica se estão selecionados todos os Primario
	if (CBoxPrimario->ItemIndex == 0)
	{ // atualiza medições
		LViewMedidoresInicia();
	}
	else
	{ // seleciona medidores do Primario selecionado
		rede = (VTRede*)CBoxPrimario->Items->Objects[CBoxPrimario->ItemIndex];
		LViewMedidores->Clear();
		// atualiza medições
		LViewMedidoresInicia(rede);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::CBoxPrimarioInicia(void)
{
	// variáveis locais
	// VTLigacao *ligacao;
	VTRede *rede;
	TList *lisREDE_VAL = ajusta_demanda->LisRedeVal();

	// limpa cbox
	CBoxPrimario->Clear();
	// cancela ordenação
	CBoxPrimario->Sorted = false;
	// insere opção de todos os Primários
	CBoxPrimario->Items->AddObject(" Todas", NULL);
	// insere um Primario p/ cada item do CBoxPrimario
	for (int n = 0; n < lisREDE_VAL->Count; n++)
	{
		rede = (VTRede*)lisREDE_VAL->Items[n];
		CBoxPrimario->Items->AddObject(rede->Codigo, rede);
	}
	// habilita ordenação
	CBoxPrimario->Sorted = true;
	// pré-seleciona primeiro item
	if (CBoxPrimario->Items->Count > 0)
		CBoxPrimario->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::CBoxRedesFatorInicia(void)
{
	TList *lisFATORES = new TList;
	VTAjusteFator *fator;
	VTRede *rede;

	// pega uma copia da lista de fatores
	ajusta_demanda->Fatores->LisFatores(lisFATORES);
	cboxRede->Items->BeginUpdate();
	cboxRede->Clear();
	// percorre toda lista
	for (int nf = 0; nf < lisFATORES->Count; nf++)
	{
		fator = (VTAjusteFator*)lisFATORES->Items[nf];
		rede = fator->Rede;
		cboxRede->AddItem(rede->Codigo, fator);
	}
	cboxRede->Items->EndUpdate();
	// seleciona o 1o
	if (cboxRede->Items->Count > 0)
	{
		cboxRede->ItemIndex = 0;
		fatorAtivo = (VTAjusteFator*)cboxRede->Items->Objects[cboxRede->ItemIndex];
	}
	DestroiTObject(lisFATORES);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::cboxRedeChange(TObject *Sender)
{
	LeFatores();
	fatorAtivo = (VTAjusteFator*)cboxRede->Items->Objects[cboxRede->ItemIndex];
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::cboxRedeDropDown(TObject *Sender)
{ // valida valores multiplicadores
	if (ValidaFator())
	{
		// ajusta_demanda->PorMedicao = false;
		SalvaFator();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::cboxRedeClick(TObject *Sender)
{
	// valida valores multiplicadores
	if (ValidaFator())
	{
		// ajusta_demanda->PorMedicao = false;
		SalvaFator();
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::ConfirmaSelecaoArquivoMedicao(void)
{
	// variáveis locais
	bool confirma;
	TFormConfirmaMedicao *Form;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	// verifica se o arquivo importado é do tipo definido pelo usuário
	Form = new TFormConfirmaMedicao(this, medicoes);
	Form->DefineArqMedicao(files);
	confirma = (Form->ShowModal() == mrOk);
	delete Form;

	return (confirma);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ConfiguraForm(void)
{
	bool medicao;

	// age de acordo com a selecao do radio group
	if (radioFator->Checked == true)
	{
		medicao = false;
	}
	else if (radioMedicao->Checked == true)
	{
		medicao = true;
	}

	// esconde ou deixa visible
	GBoxMedicao->Visible = medicao;
	GBoxMultiplicadores->Visible = (!medicao);
	// acerta o ajusta para o tipo correto
	AlteraTipoAjuste();
	// esconde ou mostra o botão de importar medição
	ButSelArqMedicao->Visible = medicao;
	// preenche o cbox
	if (!medicao)
	{
		CBoxRedesFatorInicia();
		LeFatores();
	}
	// acerta o alinhamento
	if (medicao)
	{
		GBoxMedicao->Align = alClient;
	}
	else
	{
		GBoxMultiplicadores->Align = alClient;
	}
	// arruma a altura do form
	if (!medicao)
	{
		this->Height = 255;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ConfiguraGBoxFatores(void)
{
	// verifica se mudou
	if (tipoFator != radiogroupTipo->ItemIndex)
	{
		FatorDefault();
	}
	switch (radiogroupTipo->ItemIndex)
	{
	case adtfFATOR_UNICO: // Fator Unico
		pnlK1->Visible = true;
		pnlK2->Visible = false;
		GBoxFatores->Caption = "Fator Único";
		lblK1->Caption = "Multiplicador:";
		lblK2->Caption = "";
		tipoFator = adtfFATOR_UNICO;
		break;
	case adtfFATOR_PQ: // Fator P e Q
		pnlK1->Visible = true;
		pnlK2->Visible = true;
		GBoxFatores->Caption = "Ativa e Reativa";
		lblK1->Caption = "Multiplicador P:";
		lblK2->Caption = "Multiplicador Q:";
		tipoFator = adtfFATOR_PQ;
		break;
	case adtfFATOR_PFP: // Fator P e FP
		pnlK1->Visible = true;
		pnlK2->Visible = true;
		GBoxFatores->Caption = "Ativa e Fator de Potência";
		lblK1->Caption = "Multiplicador P:";
		lblK2->Caption = "Fator de Potência:";
		tipoFator = adtfFATOR_PFP;
		break;
	default: ;
	}
}
// ---------------------------------------------------------------------------
/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormDemanda::ContinuaMesmoComPrimariosNaoAjustaveis(TList *lisPRI_VAL)
 {
 //variáveis locais
 AnsiString txt, aviso;
 int        count = 1;
 VTLigacao  *ligacao;
 VTPrimario *primario;
 TList      *lisPRIMARIO = radial->LisPrimario();

 //reinicia lisPRI_VAL
 lisPRI_VAL->Clear();
 //identifica Primarios que podem ser ajustados
 for (int n = 0; n < lisPRIMARIO->Count; n++)
 {
 primario = (VTPrimario*)lisPRIMARIO->Items[n];
 //nao insere redes equivalentadas
 if (! primario->Rede->Carregada)
 {
 aviso += txt.sprintf("%02d. %s: rede não carregada\n", count++, primario->Rede->Codigo.c_str());
 continue;
 }
 //não insere redes sem chave inicial
 //      ligacao = primario->LigacaoInicial;
 //      if ((ligacao == NULL) || (ligacao->Tipo() != eqptoCHAVE))
 //         {
 //         aviso += txt.sprintf("%02d. %s: não possui chave inicial\n", count++, primario->Rede->Codigo.c_str());
 //         continue;
 //         }
 //não insere rede com redes supridas equivalentadas
 if (ExisteVizinhaEquivalentada(primario))
 {
 aviso += txt.sprintf("%02d. %s: alimenta redes não carregadas\n", count++, primario->Rede->Codigo.c_str());
 continue;
 }
 //Primario válido: insere em lisPRI_VAL
 lisPRI_VAL->Add(primario);
 }
 if (aviso.IsEmpty()) return(true);
 //exibe informação ao usuário e pede para ele confirmar a continuação
 txt = "Há redes primárias que não podem ser ajustadas:\n" + aviso;
 return(Confirma(txt, "Deseja continuar ?") == IDYES);
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::CriaObjAjusta(void)
{
	// variáveis locais
	VTTrafo *trafo;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTTipoRede *tiporede_SE;

	// inicia tipo de ajuste por fase
	ajuste_por_fase = true;
	try
	{ // verifica se ja foi criado o obj
		if (ajusta_demanda)
			delete ajusta_demanda;
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoTRAFO);
		tiporede_SE = tipos->ExisteTipoRede(redeETD);
		for (int n = 0; n < lisEQP->Count; n++)
		{
			trafo = (VTTrafo*)lisEQP->Items[n];
			if (trafo->rede->TipoRede != tiporede_SE)
			{ // permite apenas ajuste trifásico
				ajusta_demanda = NewObjAjustaTrifasico(apl);
				ajuste_por_fase = false;
				break;
			}
		}
		if (!ajusta_demanda)
		{ // ajuste de demanda por fase
			ajusta_demanda = NewObjAjustaPorFase(apl);
			ajuste_por_fase = true;
		}
	}
	catch (Exception &e)
	{ // nada
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::CriaFatores(void)
{
	ajusta_demanda->Fatores->CriaFatoresRedes(redePRI);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::editK1Click(TObject *Sender)
{
	editK1->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TFormDemanda::editK2Click(TObject *Sender)
{
	editK2->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ExibeMedidores(bool visible)
{
	// variáveis locais
	VTMedidor *medidor;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obtém lista de Medidores da rede
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoMEDIDOR);
	// loop p/ todos Medidores
	for (int i = 0; i < lisEQP->Count; i++)
	{
		medidor = (VTMedidor*)lisEQP->Items[i];
		medidor->Show[eqp_VISIBLE] = visible;
	}
	// atualiza o gráfico
	if (lisEQP->Count > 0)
		grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ExibeResultadoAjuste(void)
{
	// variáveis locais
	TFormLogAjustaDemanda *form;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	// proteção
	if (!medicoes)
		return;
	if (!medicoes->LisMedicao())
		return;
	// verifica se há pelo menos uma Medicao selecionada
	if (ExisteMedidor() == 0)
		return;
	// if(ExisteMedicaoSelecionada() == 0) return;
	if ((form = new TFormLogAjustaDemanda(this, apl)) != NULL)
	{ // mostra se as redes foram ajustadas e o que houve com as que não foram
		Hide();
		form->Executa(lisMEDICAO, ajuste_por_fase, ajusta_demanda);
		if (form->ShowModal() == mrOk)
			delete form;
		Show();
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::ExisteGeracao(void)
{
	// variáveis locais
	VTGerador *gerador;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool existe_gerador = false;
	TList *lisGER = new TList();

	// verifica se há geradores PV ou PQ na rede
	redes->LisEqpto(lisGER, eqptoGERADOR);
	for (int ng = 0; ng < lisGER->Count; ng++)
	{
		gerador = (VTGerador*)lisGER->Items[ng];
		if (gerador->tiposup == supPV)
			existe_gerador = true;
		if (gerador->tiposup == supPQ)
			existe_gerador = true;
	}
	// destroi lista
	delete lisGER;

	return (existe_gerador);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TFormDemanda::ExisteLigacao(VTMedidor *medidor)
{
	// variaveis locais
	VTCanal *canal;
	TList *lisCANAL;

	// proteção
	if (!medidor)
		return (NULL);
	lisCANAL = medidor->LisCanal();
	for (int nc = 0; nc < lisCANAL->Count; nc++)
	{ // verifica se ha algum canal associado a ligacao
		canal = (VTCanal*)lisCANAL->Items[nc];
		if (canal->Eqpto->TipoLigacao())
			return ((VTLigacao*)canal->Eqpto);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
int __fastcall TFormDemanda::ExisteMedidor(void)
{
	// verifica se LView está vazio
	return (LViewMedidores->Items->Count);

}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::FatorDefault(void)
{
	editK1->Text = "1.0";
	editK2->Text = "1.0";
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::FormCloseQuery(TObject *Sender, bool &CanClose)
{ // DVK 2014.09.16
	// verifica se o ajuste deve ser mantido ou não
	if (!ajusta_demanda->AjusteJaExecutado)
	{
		ajusta_demanda->RestauraCargasOriginais();
		RestauraFlowOriginal();
		CanClose = true;
		return;
	}
	// confirmar e sair
	switch (Confirma("Deseja efetivar o ajuste de demanda na rede ?", "", MB_YESNOCANCEL))
	{
	case ID_YES:
		ajusta_demanda->RestauraCargasRedesNaoAjustadas();
		RestauraFlowOriginal();
		CanClose = true;
		break;
	case ID_NO: // retorna cargas originais
		ajusta_demanda->RestauraCargasOriginais();
		RestauraFlowOriginal();
		CanClose = true;
		break;
	case ID_CANCEL:
	default:
		CanClose = false;
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::FormIniciaPosicao(void)
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
	Left = 0;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::ImportaMedicao(void)
{
	// variáveis locais
	bool leitura = false;
	VTImportaMedicao *importa_medicao =
		(VTImportaMedicao*)apl->GetObject(__classid(VTImportaMedicao));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	// proteção
	if (files->Count == 0)
	{
		Aviso("Selecione arquivos de medição");
		return (false);
	}
	// verifica se existem todos os arquivos selecionados
	for (int n = 0; n < files->Count; n++)
	{
		if (!FileExists(files->Strings[n]))
		{
			Aviso("Não existe o arquivo de medição:\n" + files->Strings[n]);
			return (false);
		}
	}
	// esconde este form
	Hide();
	try
	{ // mostra progresso
		prog->Start(progDEFAULT);
		// importa lista de medições no arquivo
		leitura = importa_medicao->LeArquivoCurvaPadrao(files);
		if (leitura)
			medicoes->AtualizaMedidores();
		// if(leitura) leitura = medicoes->CriaMedicoesMedidores();
	}
	catch (Exception &)
	{
		Aviso("Erro na leitura do arquivo de medição");
		// para progresso
		prog->Stop();
		// reapresenta este form
		Show();
		return (false);
	}
	// para progresso
	prog->Stop();
	// reapresenta este form
	Show();
	return (leitura);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::InsereMedidoresAjustados(void)
{
	// variaveis locais
	TList *lisMEDIDOR;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	// desfaz todas ações do edita
	lisMEDIDOR = medicoes->LisMedidorAjustado();
	edita->UndoCompleto();
	edita->Clear();
	// restaura obra original do edita
	edita->Obra = obra_original;
	// insere somente medidores ajustados
	edita->InsereLisEqpto(lisMEDIDOR);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::InsereValor(TListItem* item, VTMedidor* medidor, int tipo_canal,
	int fase_tag)
{
	// variaveis locais
	AnsiString maior_erro;
	VTCanal *canal;
	// VTEqpto *eqpto;
	VTLigacao *ligacao;
	TList *lisEQP;

	// proteção
	if (!item)
		return;
	if (!medidor)
		return;
	if ((ligacao = ExisteLigacao(medidor)) == NULL)
		return;
	// cria lista
	lisEQP = new TList();
	// localiza canal
	canal = medidor->ExisteCanal(ligacao, tipo_canal, fase_tag);
	// verifica o maior valor do erro entre medido e estimado
	maior_erro = MaiorErro(canal);
	item->SubItems->Add(maior_erro);
	// destroi lista
	delete lisEQP;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::LeFatores(void)
{
	bool sucesso = false;
	VTAjusteFator *fator;

	try
	{
		// acerta o gboxTipoAjuste
		if (ajusta_demanda->PorMedicao == true)
		{
			radioMedicao->Checked = true;
			radiogroupTipo->ItemIndex = 0;
			FatorDefault();
		}
		else // acerta o gboxMultiplicadores
		{
			radioFator->Checked = true;
			///ConfiguraForm();
			fator = (VTAjusteFator*)cboxRede->Items->Objects[cboxRede->ItemIndex];
			switch (fator->TipoFatorCorrec)
			{
			case adtfFATOR_UNICO:
				radiogroupTipo->ItemIndex = 0;
				editK1->Text = DoubleToStr("%4.1f%", fator->FatorCorrecP);
				break;
			case adtfFATOR_PQ:
				radiogroupTipo->ItemIndex = 1;
				editK1->Text = DoubleToStr("%4.1f%", fator->FatorCorrecP);
				editK2->Text = DoubleToStr("%4.1f%", fator->FatorCorrecQ);
				break;
			case adtfFATOR_PFP:
				radiogroupTipo->ItemIndex = 2;
				editK1->Text = DoubleToStr("%4.1f%", fator->FatorCorrecP);
				editK2->Text = DoubleToStr("%4.1f%", fator->FatorCorrecFP);
				break;
			default: ;
			}
		}
		ConfiguraGBoxFatores();
		sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}
////---------------------------------------------------------------------------
// bool __fastcall TFormDemanda::LeFatores(void)
// {
// bool sucesso = false;
//
// try
// {
// //acerta o gboxTipoAjuste
// if (ajusta_demanda->PorMedicao == true)
// {
// radioMedicao->Checked = true;
// radiogroupTipo->ItemIndex = 0;
// editK1->Text = "1.0";
// editK2->Text = "1.0";
// }
// else//acerta o gboxMultiplicadores
// {
// radioFator->Checked = true;
// ConfiguraForm();
// switch (ajusta_demanda->TipoFatorCorrec)
// {
// case adtfFATOR_UNICO:
// radiogroupTipo->ItemIndex = 0;
// editK1->Text = DoubleToStr("%4.1f%%",ajusta_demanda->FatorCorrecP);
// break;
// case adtfFATOR_PQ:
// radiogroupTipo->ItemIndex = 1;
// editK1->Text = DoubleToStr("%4.1f%%",ajusta_demanda->FatorCorrecP);
// editK2->Text = DoubleToStr("%4.1f%%",ajusta_demanda->FatorCorrecQ);
// break;
// case adtfFATOR_PFP:
// radiogroupTipo->ItemIndex = 2;
// editK1->Text = DoubleToStr("%4.1f%%",ajusta_demanda->FatorCorrecP);
// editK2->Text = DoubleToStr("%4.1f%%",ajusta_demanda->FatorCorrecFP);
// break;
// default:
// ;
// }
// }
// ConfiguraGBoxFatores();
// sucesso = true;
// }
// catch (Exception &e)
// {
// sucesso = false;
// }
//
// return sucesso;
// }

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::LViewMedidoresClick(TObject *Sender)
{
	// variáveis locais
	TListItem *item;
	VTMedidor *medidor;
	TList *lisEQP = new TList();

	// determina medidor selecionado
	item = LViewMedidores->Selected;
	// desenha moldura
	if (item && item->Data)
	{ // 2016.06.14 moldura no eqpto do medidor (se tiver)
		medidor = (VTMedidor*)item->Data;
		medidor->LisEqptoDosCanais(lisEQP);
		if (lisEQP->Count > 0)
			Moldura((VTEqpto*)lisEQP->First());
		else
			Moldura(medidor);
	}
	delete lisEQP;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::LViewMedidoresColumnClick(TObject *Sender, TListColumn *Column)
{ // DVK 2016.05.17
	// verifica se selecionou a mesma coluna
	if (LViewSort.Column == Column)
	{ // inverte ordem
		LViewSort.OrdemCrescente = !LViewSort.OrdemCrescente;
	}
	else
	{ // salva coluna usada p/ ordenação
		LViewSort.Column = Column;
		LViewSort.OrdemCrescente = true;
	}
	// força reordenação
	((TListView*)Sender)->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::LViewMedidoresCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{ // DVK 2016.05.17
	// variáveis locais
	int index;
	double valor1, valor2;

	// proteção
	if (LViewSort.Column == NULL)
		return;
	// determina index p/ substrings
	index = LViewSort.Column->Index - 1;
	// verifica se coluna de Caption
	if (LViewSort.Column->Index == 0)
	{ // coluna de Caption
		Compare = CompareText(Item1->Caption, Item2->Caption);
	}
	else if (LViewSort.Column->Alignment == taLeftJustify)
	{ // coluna com texto
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
	}
	else
	{ // coluna com valor numérico
		valor1 = StrToDouble(Item1->SubItems->Strings[index]);
		valor2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (valor1 < valor2)
			Compare = -1;
		else if (valor1 > valor2)
			Compare = 1;
		else
			Compare = 0;
	}
	// verifica se ordem crescente/decrescente
	if (!LViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::LViewMedidoresDblClick(TObject *Sender)
{
	// variáveis locais
	TListItem *item;

	// determina medidor selecionado
	item = LViewMedidores->Selected;
	if (item && item->Data)
	{ // cria form e mostra dados do medidor
		DLL_SelEqpto(this, apl, this->Parent, (VTEqpto*)item->Data);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::LViewMedidoresInicia(void)
{
	// variaveis locais
	VTCanal *canal;
	// VTLigacao *ligacao;
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisMEDIDOR, *lisCANAL; // ,*lisSEC;
	TListItem *item;
	bool grandezaI, grandezaP, grandezaQ;

	// limpa lview
	LViewMedidores->Clear();
	// cria lista
	lisMEDIDOR = new TList();
	redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		// reinicia tipo de grandeza
		grandezaI = grandezaP = grandezaQ = false;
		lisCANAL = medidor->LisCanal();
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // insere medidores
			canal = (VTCanal*)lisCANAL->Items[nc];
			// insere dados de canais com grandezas não repetidas por medidor
			if ((grandezaI) && (canal->TipoCanal == canalI))
				continue;
			if ((grandezaP) && (canal->TipoCanal == canalP))
				continue;
			if ((grandezaQ) && (canal->TipoCanal == canalQ))
				continue;
			if (canal->TipoCanal == canalI)
				grandezaI = true;
			if (canal->TipoCanal == canalP)
				grandezaP = true;
			if (canal->TipoCanal == canalQ)
				grandezaQ = true;
			item = LViewMedidores->Items->Add();
			item->Data = medidor;
			// medidor
			item->Caption = medidor->Codigo;
			// grandeza
			item->SubItems->Add(canal->TipoCanalAsString);
			// fase A
			InsereValor(item, medidor, canal->TipoCanal, faseA);
			// fase B
			InsereValor(item, medidor, canal->TipoCanal, faseB);
			// fase C
			InsereValor(item, medidor, canal->TipoCanal, faseC);
			// fase ABC
			InsereValor(item, medidor, canal->TipoCanal, faseABC);
		}
	}
	// destroi lista
	delete lisMEDIDOR;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::LViewMedidoresInicia(VTRede *rede)
{
	// variaveis locais
	VTCanal *canal;
	VTLigacao *ligacao;
	VTMedidor *medidor;
	TList *lisMEDIDOR, *lisCANAL;
	TListItem *item;
	bool grandezaI, grandezaP, grandezaQ;

	// proteção
	if (!rede)
		return;
	// cria lista
	lisMEDIDOR = new TList();
	rede->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		// reinicia tipo de grandeza
		grandezaI = grandezaP = grandezaQ = false;
		lisCANAL = medidor->LisCanal();
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // insere medidores de eqptos da mesma rede
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto->TipoLigacao())
			{ // DVK 2016.03.14
				ligacao = (VTLigacao*)canal->Eqpto;
				if (ligacao->rede != rede)
					continue;
			}
			// insere dados de canais com grandezas não repetidas por medidor
			if ((grandezaI) && (canal->TipoCanal == canalI))
				continue;
			if ((grandezaP) && (canal->TipoCanal == canalP))
				continue;
			if ((grandezaQ) && (canal->TipoCanal == canalQ))
				continue;
			if (canal->TipoCanal == canalI)
				grandezaI = true;
			if (canal->TipoCanal == canalP)
				grandezaP = true;
			if (canal->TipoCanal == canalQ)
				grandezaQ = true;
			item = LViewMedidores->Items->Add();
			item->Data = medidor;
			// medidor
			item->Caption = medidor->Codigo;
			// grandeza
			item->SubItems->Add(canal->TipoCanalAsString);
			// fase A
			InsereValor(item, medidor, canal->TipoCanal, faseA);
			// fase B
			InsereValor(item, medidor, canal->TipoCanal, faseB);
			// fase C
			InsereValor(item, medidor, canal->TipoCanal, faseC);
			// fase ABC
			InsereValor(item, medidor, canal->TipoCanal, faseABC);
		}
	}
	// destroi lista
	delete lisMEDIDOR;
}
/*
// ---------------------------------------------------------------------------
AnsiString __fastcall TFormDemanda::MaiorErro(VTCanal *canal)
{
	// variaveis locais
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	AnsiString str_valor;
	double maior;

	// retorna valor percentual
	maior = medicoes->MaiorErro(canal) * 100.;
	str_valor = DoubleToStr("%2.1f", fabs(maior), ',') + "%";
	return (str_valor);
}
 */
// ---------------------------------------------------------------------------
AnsiString __fastcall TFormDemanda::MaiorErro(VTCanal *canal)
{
	// variaveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();
	double med, est;
	double erro = 0.;
	double maior = 0.;
	AnsiString str_valor;

	// proteção
	if (!canal)
		return ("0.0%");
	for (int np = 0; np < lisPAT->Count; np++)
	{ // verifica a maior diferença entre todos os patamares
		patamar = (VTPatamar*)lisPAT->Items[np];
		//verifica se patamar está habilitado
		if(! ajusta_demanda->Patamares[np])  continue;
		med = canal->ValorMedido[patamar];
		est = canal->ValorEstimado[patamar];
		if (IsDoubleZero(med))
			continue;
		if (IsDoubleZero(est))
			continue;
		erro = (fabs(med - est) / med);
		// considera modulo do erro, pois ele pode ser negativo
		erro = fabs(erro);
		maior = Max(maior, erro);
	}
	// retorna valor
	maior = fabs(maior) * 100.;
    str_valor = DoubleToStr("%2.1f", fabs(maior), ',') + "%";
	return (str_valor);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::Moldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	if (grafico)
		grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::Moldura(TList *lisEqp)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	if (grafico)
		grafico->Moldura(lisEqp);
}
// ---------------------------------------------------------------------------

void __fastcall TFormDemanda::radioFatorClick(TObject *Sender)
{
	ConfiguraForm();
}
// ---------------------------------------------------------------------------

void __fastcall TFormDemanda::radioMedicaoClick(TObject *Sender)
{
	ConfiguraForm();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::radiogroupTipoClick(TObject *Sender)
{
	ConfiguraGBoxFatores();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::ReiniciaDiagnostico(void)
{
	// variáveis locais
	VTMedicao *medicao;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	for (int nm = 0; nm < lisMEDICAO->Count; nm++)
	{ // assume todos ok
		medicao = (VTMedicao*)lisMEDICAO->Items[nm];
		medicao->diagnostico = diagMED_OK;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDemanda::RestauraFlowOriginal(void)
{ // DVK 2015.09.24
	// variáveis locais
	VTApl *apl_owner = apl->Owner;
	VTFlow *flow = (VTFlow*)apl_owner->GetObject(__classid(VTFlow));

	// cancela atualização do flow
	if (flow)
		flow->UpToDate = false;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::SalvaFator(void)
{
	VTAjusteFator *fator;
	bool sucesso = false;

	try
	{
		fator = fatorAtivo;
		// valida valores multiplicadores
		if (ValidaFator())
		{
			ajusta_demanda->PorMedicao = false;
			switch (tipoFator)
			{
			case 0: // fator unico
				fator->TipoFatorCorrec = adtfFATOR_UNICO;
				fator->FatorCorrecP = StrToDouble(editK1->Text);
				fator->FatorCorrecQ = StrToDouble(editK1->Text);
				fator->FatorCorrecFP = 1.0;
				break;
			case 1: // Fator P e Q
				fator->TipoFatorCorrec = adtfFATOR_PQ;
				fator->FatorCorrecP = StrToDouble(editK1->Text);
				fator->FatorCorrecQ = StrToDouble(editK2->Text);
				fator->FatorCorrecFP = 1.0;
				break;
			case 2: // fator P e FP
				fator->TipoFatorCorrec = adtfFATOR_PFP;
				fator->FatorCorrecP = StrToDouble(editK1->Text);
				fator->FatorCorrecQ = 1.0;
				fator->FatorCorrecFP = StrToDouble(editK2->Text);
				break;
			default: ;
			}
			// verifica mudança se tiver algum valor diferente de 1.0
			if (!IsDoubleZero(fator->FatorCorrecP - 1.0))
			{
				fator->Ativo = true;
			}
			else if (!IsDoubleZero(fator->FatorCorrecQ - 1.0))
			{
				fator->Ativo = true;
			}
			else if (!IsDoubleZero(fator->FatorCorrecFP - 1.0))
			{
				fator->Ativo = true;
			}
			else
			{
				fator->Ativo = false;
			}
			sucesso = true;
		}
		else
		{
			Aviso("Multiplicadores inválidos.");
		}

	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::SalvaFatores(void)
{
	// VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	bool sucesso = true;

	// acerta o gboxTipoAjuste
	if (radioMedicao->Checked == true)
	{
		ajusta_demanda->PorMedicao = true;
	}
	else
	{

		// valida valores multiplicadores
		if (ValidaFator())
		{
			ajusta_demanda->PorMedicao = false;
			SalvaFator();
		}
		else
		{
			Aviso("Multiplicadores inválidos.");
			sucesso = false;
		}
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::SelecionaArquivo(void)
{
	// variáveis locais
	bool sucesso = false;

	// esconde este Form
	Visible = false;
	// DVK 2015.09.24 try/catch para impedir que um arquivo de medição aberto
	// tente ser importado
	try
	{ // reinicia StringList de arquivos de medição
		files->Clear();
		if (OpenTextFileDialog->Execute())
		{ // lê lista de arquivos de Medição selecionados
			files->AddStrings(OpenTextFileDialog->Files);
			// exibe o conteúdo do arquivo e pede para o usuário confirmar a seleção
			sucesso = ConfirmaSelecaoArquivoMedicao();
		}
	}
	catch (Exception &e)
	{ // avisa o problema e reexibe este Form
		Aviso("O arquivo selecionado deve estar aberto. "
			"Feche o arquivo para importar a medição.");
		Visible = true;
		return (false);
	}
	// exibe este Form
	Visible = true;

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::ValidaFator(void)
{
	bool sucesso = false;
	double k2; // ,k1;

	switch (tipoFator)
	{
	case adtfFATOR_UNICO: // Fator Unico
		if (ValidaFatorEdit(editK1->Text))
		{

			// k1 = StrToDouble(editK1->Text);
			k2 = StrToDouble(editK1->Text);
			sucesso = true;
		}
		break;

	case adtfFATOR_PQ: // Fator P e Q
		if (ValidaFatorEdit(editK1->Text) && ValidaFatorEdit(editK2->Text))
		{
			// k1 = StrToDouble(editK1->Text);
			k2 = StrToDouble(editK2->Text);
			sucesso = true;
		}
		break;
	case adtfFATOR_PFP: // Fator P e FP
		if (ValidaFatorEdit(editK1->Text) && ValidaFatorEdit(editK2->Text))
		{
			// k1 = StrToDouble(editK1->Text);
			k2 = StrToDouble(editK2->Text);
			// verifica se FP maior que 1
			if (k2 <= 1.0)
			{
				sucesso = true;
			}
		}
		break;
	default: ;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDemanda::ValidaFatorEdit(AnsiString fator)
{
	double valor;
	bool sucesso = false;

	try
	{
		StrToDouble(fator, valor);
		if (valor > 0)
		{
			sucesso = true;
		}
	}
	catch (EConvertError &e)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
// eof
