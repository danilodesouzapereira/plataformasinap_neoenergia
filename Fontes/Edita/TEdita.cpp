// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdita.h"
#include "TDivide.h"
#include "VTModifica.h"
#include "..\Apl\VTApl.h"
#include "..\AutoTag\VTAutoTag.h"
#include "..\Busca\VTBusca.h"
#include "..\Esquematico\VTEsquematico.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Manobra\VTSincroniza.h"
#include "..\MultiObra\VTMultiObra.h"
#include "..\MultiObra\VTMultiObraBO.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTMotor.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\AutoTag.h"
#include "..\..\DLL_Inc\Esquematico.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Manobra.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEdita* __fastcall NewObjEdita(VTApl *apl)
{
	return (new TEdita(apl));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16498932
__fastcall TEdita::TEdita(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// cria ActionUndo e associa c/ método de tratamento
	ActionUndo = new TAction(NULL);
	ActionUndo->OnExecute = ActionUndoExecute;
	ActionUndo->Hint = "Cancelar última alteração";
	ActionUndo->Enabled = false;
	// cria ActionRedo e associa c/ método de tratamento
	ActionRedo = new TAction(NULL);
	ActionRedo->OnExecute = ActionRedoExecute;
	ActionRedo->Hint = "Refazer última alteração cancelada";
	ActionRedo->Enabled = false;
	// cria ActionUndoSemRedo e associa c/ método de tratamento
	ActionUndoSemRedo = new TAction(NULL);
	ActionUndoSemRedo->OnExecute = ActionUndoSemRedoExecute;
	ActionUndoSemRedo->Hint = "";
	ActionUndoSemRedo->Enabled = true;
	// cria listas
	lisACTION = new TList();
	lisRET = new TList();
	lisTMP = new TList();
	// listas para redo/undo com múltiplas ações
	ru.lisREDE = new TList();
	ru.lisACAO = new TList();
	// cria objeto VTObra local p/ controle de Undo/Redo
	PD.obra_local = NewObjObra(apl);
	PD.obra_local->Nome = "Rede original";
	PD.obra_local->ColorEnabled = false;
	// inicia objeto Obra externa
	PD.obra_externa = NULL;
	// inicia Action do objeto Obra
	Obra->DefineActions(ActionUndo, ActionRedo);
	// cria objeto Sincroniza p/ alteração de Chaves sincronizadas
	sincroniza = DLL_NewObjSincroniza(apl);
	// cria objeto Esquematico p/ montagem de diagrams esquematicos das redes
	esquematico = DLL_NewObjEsquematico(apl);
	// cria objeto VTModifica p/ efetivar as alterações das Redes
	modifica = NewObjModifica(apl);
	modifica->DefineActionExterna(lisACTION);
	//inicializa ponteiro para o AutoTag
    autoTag = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TEdita::~TEdita(void)
{
	// destrói objetos
	if (ActionRedo)
	{
		delete ActionRedo;
		ActionRedo = NULL;
	}
	if (ActionUndo)
	{
		delete ActionUndo;
		ActionUndo = NULL;
	}
	if (PD.obra_local)
	{
		delete PD.obra_local;
		PD.obra_local = NULL;
	}
	if (modifica)
	{
		delete modifica;
		modifica = NULL;
	}
	if (sincroniza)
	{
		delete sincroniza;
		sincroniza = NULL;
	}
	if (esquematico)
	{
		delete esquematico;
		esquematico = NULL;
	}
	//destroi auto tag
	if (autoTag != NULL)
	{
		delete autoTag;
		autoTag = NULL;
	}
	// destrói listas sem destruir seus objetos
	if (lisACTION)
	{
		delete lisACTION;
		lisACTION = NULL;
	}
	if (lisRET)
	{
		delete lisRET;
		lisRET = NULL;
	}
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	// destrói listas sem destruir seus objetos
	if (ru.lisREDE)
	{
		delete ru.lisREDE;
		ru.lisREDE = NULL;
	}
	if (ru.lisACAO)
	{
		delete ru.lisACAO;
		ru.lisACAO = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
void __fastcall TEdita::ActionRedoExecute(TObject *Sender)
{
	// variáveis locais
	VTAcao *acao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// determina Acao p/ Redo
	if ((acao = Obra->Redo(true)) == NULL)
		return;
	// executa Acao
	modifica->Redo(Obra, acao);
	// atualiza o gráfico
	if (grafico)
		grafico->Redraw();
	//atualiza o multiObra
	AtualizaMultiObra();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
void __fastcall TEdita::ActionUndoExecute(TObject *Sender)
{
	// variáveis locais
	VTAcao *acao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// determina Acao p/ Undo
	if ((acao = Obra->Undo(true)) == NULL)
		return;
	// executa Acao
	modifica->Undo(Obra, acao);
	// atualiza o gráfico
	if (grafico)
		grafico->Redraw();
	//atualiza o multiObra
	AtualizaMultiObra();
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::ActionUndoSemRedoExecute(TObject *Sender)
{
	// variáveis locais
	VTAcao *acao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// determina Acao p/ Undo
	if ((acao = Obra->Undo()) == NULL)
		return;
	// undo Acao
	modifica->Undo(Obra, acao);
	// elimina Acao p/ evitar opção Redo
	Obra->DeleteRedo(acao);
	// atualiza o gráfico
	if (grafico)
		grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::ActionOnChangeRedeInsere(TAction *OnChangeRede)
{
	// insere Action na lista
	if (lisACTION->IndexOf(OnChangeRede) < 0)
		lisACTION->Add(OnChangeRede);
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::ActionOnChangeRedeRetira(TAction *OnChangeRede)
{
	// retira Action da lista
	lisACTION->Remove(OnChangeRede);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::Altera(VTEqpto *antes, VTEqpto *depois)
{
	// variáveis locais
	VTAcao *acao;

	// cria Acao p/ Undo/Redo
	if ((acao = Obra->AddAcaoAltera(antes, depois)) == NULL)
		return (false);
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDiagEsqArea(VTRedes *redes, double fator)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// altera Redes
	redes->DiagEsquematicoResize(fator);
	// atualiza gráfico
	grafico->AreaRedeDefine();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDiagEsqArea(VTRedes *redes, TList *lisEQP, double fator)
{
	// variáveis locais
	VTAcao *acao, *acao_composta;
	VTEqpto *eqpto, *copia_alterado;
	VTLigacao *ligacao;
	VTBarra *pbarra, *pbar_alterada;
	VTTrecho *ptrecho, *ptre_alterado;
	TList *lisACAO;

	// monta lisTMP com Barras dos Eqptos selecionados
	lisTMP->Clear();
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->TipoBarra())
		{
			if (lisTMP->IndexOf(eqpto) < 0)
				lisTMP->Add(eqpto);
		}
		else if (eqpto->TipoEqbar())
		{
			eqpto = ((VTEqbar*)eqpto)->pbarra;
			if (lisTMP->IndexOf(eqpto) < 0)
				lisTMP->Add(eqpto);
		}
		else if (eqpto->TipoLigacao())
		{
			ligacao = (VTLigacao*)eqpto;
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
				if ((eqpto = ligacao->Barra(ind_bar)) == NULL)
					continue;
				if (lisTMP->IndexOf(eqpto) < 0)
					lisTMP->Add(eqpto);
			}
		}
	}
	// verifica se há Barras selecionadas
	if (lisTMP->Count == 0)
		return (false);
	// cria Acao composta
	if ((acao_composta = Obra->AddAcaoComposta()) == NULL)
		return (false);
	// cria uma Acao p/ alteração da Barras
	for (int n = 0; n < lisTMP->Count; n++)
	{
		pbarra = (VTBarra*)lisTMP->Items[n];
		// cria uma cópia da Barra
		if ((pbar_alterada = pbarra->Clone()) == NULL)
			return (false);
		// insere operação p/ Undo/Redo
		if (Obra->AddAcaoAltera(pbarra, pbar_alterada, acao_composta) == NULL)
			return (false);
	}
	// redefine coordenadas de todas as Barras
	redes->DiagEsquematicoResize(lisTMP, fator);
	// atualiza Eqpto alterado e Eqpto original de todas Acoes da Acao composta
	lisACAO = acao_composta->LisAcao();
	for (int n = 0; n < lisACAO->Count; n++)
	{
		acao = (VTAcao*)lisACAO->Items[n];
		// atualiza Eqpto alterado c/ Eqpto original
		// *(acao->CopiaDoEqptoAlterado()) << *(acao->Eqpto());
		eqpto = acao->Eqpto();
		copia_alterado = acao->CopiaDoEqptoAlterado();
		copia_alterado->CopiaAtributosDe(*eqpto);
		// atualiza estado do eqpto
		// (acao->CopiaDoEqptoAlterado())->Status[sttALTERADO] = true;
		copia_alterado->Status[sttALTERADO] = true;
	}
	// return(modifica->Do(acao));
	return (modifica->Do(Obra, acao_composta));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDiagEsqIgualDiagGeo(VTRedes *redes)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// verifica se é possível fazer a montagem
	if (esquematico->ExisteBarraSemCoordUtm(redes))
	{ // tenta calcular coordenadas UTM das Barras em função das demais Barras
		if (!esquematico->AjustaCoordBarraSemCoordUtm(redes))
		{
			Aviso("Existem barras sem coordenadas utm.\n"
				"Não é possível montar o diag.esquemático a partir do diag.geo-referenciado");
			return (false);
		}
	}
	// cancela Undo/Redo
	Clear();
	// monta esquemático igual c/ as mesmas coordendas do geo-referenciado
	esquematico->IniciaEsquematicoIgualGeo(redes);
	// atualiza gráfico
	grafico->AreaRedeDefine();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDiagEsqInverteX(VTRedes *redes)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cancela Undo/Redo
	Clear();
	// monta esquemático igual c/ coordendas Y invertidas
	esquematico->InverteCoordX(redes);
	// atualiza gráfico
	grafico->AreaRedeAltera();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDiagEsqInverteY(VTRedes *redes)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cancela Undo/Redo
	Clear();
	// monta esquemático igual c/ coordendas Y invertidas
	esquematico->InverteCoordY(redes);
	// atualiza gráfico
	grafico->AreaRedeAltera();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDiagGeoArea(VTRedes *redes, double fator)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// altera Redes
	redes->DiagGeorefenciadoResize(fator);
	// atualiza gráfico
	grafico->AreaRedeDefine();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraDivideTrecho(VTTrecho *tre_ori, VTTrecho *tre_alt, VTBarra *bar_ref,
	double dist_m)
{
	// variáveis locais
	bool sucesso;
	VTAcao *acao, *acao_composta;
	VTBarra *barra;
	TDivide divide(tre_ori, tre_alt, bar_ref, dist_m);

	try
	{ // cria Acao composta
		if ((acao_composta = Obra->AddAcaoComposta()) == NULL)
			return (false);
		// cria Acao para inserir Barra nova
		barra = divide.BarraNova();
		Obra->AddAcaoInsere(barra, acao_composta);
		// cria Acao p/ alterar Trecho original
		tre_ori = divide.TrechoOriginal();
		tre_alt = divide.TrechoAlterado();
		Obra->AddAcaoAltera(tre_ori, tre_alt, acao_composta);
		// cria Acao para inserir Trecho novo
		tre_ori = divide.TrechoNovo();
		Obra->AddAcaoInsere(tre_ori, acao_composta);
		// efetiva alteração
		sucesso = modifica->Do(Obra, acao_composta);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
// IMPORTANTE: cada Eqpto original aponta p/ Eqpto alterado através de VTEqpto::Obj
// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraLisEqpto(TList *lisEQP)
{
	// variáveis locais
	VTAcao *acao;
	VTAcao *acao_composta;
	VTEqpto *eqpto_ori, *eqpto_alt;

	// verifica se lisEQP possui um único Eqpto
	if (lisEQP->Count == 1)
	{ // cria uma única Acao
		eqpto_ori = (VTEqpto*)lisEQP->First();
		eqpto_alt = (VTEqpto*)eqpto_ori->Obj;
		eqpto_ori->Obj = NULL;
		eqpto_alt->Obj = NULL;
		return (Altera(eqpto_ori, eqpto_alt));
	}
	// cria Acao composta
	if ((acao_composta = Obra->AddAcaoComposta()) == NULL)
		return (false);
	// cria uma Acao p/ cada Eqpto da lista
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto_ori = (VTEqpto*)lisEQP->Items[n];
		eqpto_alt = (VTEqpto*)eqpto_ori->Obj;
		eqpto_ori->Obj = NULL;
		eqpto_alt->Obj = NULL;
		Obra->AddAcaoAltera(eqpto_ori, eqpto_alt, acao_composta);
	}
	return (modifica->Do(Obra, acao_composta));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraSincronismo(bool enabled)
{
	// variáveis locais
	VTAcao *acao;
	VTChave *pchave, *pchave_alt;
	TList *lisCHV;

	// verifica se está desabilitando
	if (!enabled)
	{ // desabilita sincronismo
		sincroniza->Disable();
		return (true);
	}
	// habilita sincronismo e verifica se houve alteração de Chave
	lisCHV = sincroniza->Enable();
	// se nenhuma Chave foi alterada, não há mais nada a fazer
	if (lisCHV->Count == 0)
		return (true);
	// se houver uma única Chave em lisCHV, cria Acao simples
	if (lisCHV->Count == 1)
	{
		pchave = (VTChave*)lisCHV->First();
		// cria clone da Chave antes de mudar seu estado
		if ((pchave_alt = pchave->Clone()) == NULL)
			return (false);
		(pchave_alt->Aberta) ? pchave_alt->Fecha() : pchave_alt->Abre();
		// insere operação p/ Undo/Redo
		if ((acao = Obra->AddAcaoAltera(pchave, pchave_alt)) == NULL)
			return (false);
	}
	else
	{ // cria Acao composta
		if ((acao = Obra->AddAcaoComposta()) == NULL)
			return (false);
		// cria uma Acao p/ alteração de cada Chace
		for (int n = 0; n < lisCHV->Count; n++)
		{
			pchave = (VTChave*)lisCHV->Items[n];
			// cria clone da Chave antes de mudar seu estado
			if ((pchave_alt = pchave->Clone()) == NULL)
				return (false);
			(pchave_alt->Aberta) ? pchave_alt->Fecha() : pchave_alt->Abre();
			// insere operação p/ Undo/Redo
			if (Obra->AddAcaoAltera(pchave, pchave_alt, acao) == NULL)
				return (false);
		}
	}
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::AlteraTapTrafo(VTTrafo *trafo, int variacao_tap)
{
	// variáveis locais
	VTTrafo *clone;

	try
	{ // cria uma cópia do Trafo e redefine seu tap
		clone = trafo->Clone();
		clone->Tap = trafo->Tap + variacao_tap;
		// proteção garante que Tap está dentro da faixa de variação
		if (abs(clone->Tap) > clone->ltc.num_passo)
		{
			if (clone->Tap < 0)
				clone->Tap = -clone->ltc.num_passo;
			else
				clone->Tap = clone->ltc.num_passo;
		}
		// efetiva alteração
		Altera(trafo, clone);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
void __fastcall TEdita::AtualizaMultiObra(void)
{
	VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
	VTMultiObra *multiObra;

	if (multiObraBO != NULL)
	{
		multiObra = multiObraBO->ExisteMultiObra(Obra);
		if (multiObra != NULL)
		{
			multiObra->UpToDate = false;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TEdita::Clear(void)
{
	// variáveis locais
	// VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cancela Undo/Redo
	Obra->Clear();
	// limpa lixeira, retirando APENAS os eqptos novos
	// redes->Lixeira->ClearEqptoNovo();
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::BarraInicialDeRede(VTBarra *barra)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// verifica se existe alguma Barra de Corte na lista lisEQP
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->BarraInicial() == barra)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::CopiaLisOrigLisDest(TList *lisORIG, TList *lisDEST)
{
	// variáveis locais
	TObject *obj;

	for (int n = 0; n < lisORIG->Count; n++)
	{
		obj = (TObject*)lisORIG->Items[n];
		if (lisDEST->IndexOf(obj) < 0)
			lisDEST->Add(obj);
	}
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
void __fastcall  TEdita::CopiaTListRedes(TList *origem, TList* destino)
	{
	VTEstudo *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;

	//copia toda a lista
	CopiaTList(origem, destino);
	if (planejamento != NULL)
		{
        // obtém objeto Estudo
		estudo = planejamento->ExisteEstudo();
		if((estudo != NULL) && (estudo->Tipo != estudoREDE_ORIGINAL))
			{//remove as redes novas q foram criadas pelas obras
			for (int nr = (destino->Count - 1); nr >= 0; nr--)
				{
				rede = (VTRede*)destino->Items[nr];
//				if (rede->StatusNovo())
//					{
//					destino->Remove(rede);
//					}
				if(redes->LisRede()->IndexOf(rede) < 0)
                    {
					destino->Remove(rede);
					}
				}
            }
		}

	}
// ---------------------------------------------------------------------------
TForm* __fastcall TEdita::EditorAtivo(void)
{
	// variáveis locais
	AnsiString class_name;
	TForm *form;

	// verifica se existe um FormPlanejamento aberto
	for (int n = 0; n < Screen->FormCount; n++)
	{ // verifica se o Form n é um TFormEditor
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC("TFormEditor") == 0)
		{
			if (Screen->Forms[n]->Owner == apl->Form)
				return (Screen->Forms[n]);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::EqptoEditavel(VTEqpto *eqpto)
{
	// verifica se Eqpto está associado a alguma Obra do planejamento
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::EqptoEditavel(TList *lisEXT)
{
	// verifica se algum Eqpto da lista está associado a alguma Obra do planejamento
	return (true);
}

// ---------------------------------------------------------------------------
VTAcao* __fastcall TEdita::ExisteMoveEmAndamento(VTEqpto *eqpto)
{
	// variáveis locais
	VTAcao *acao;

	// obtém última Acao inserida
	if ((acao = Obra->LastAcao()) == NULL)
		return (NULL);
	// verifica se a última Acao é de alteração
	if (acao->Tipo() != acaoALTERA)
		return (NULL);
	// verifica se o Eqpto original da Acao é o Eqpto indicado
	if (acao->Eqpto() != eqpto)
		return (NULL);
	// verifica se a Acao de alteração é de deslocamento
	if (!acao->Moving)
		return (NULL);
	return (acao);
}

// ---------------------------------------------------------------------------
VTAcao* __fastcall TEdita::ExisteMoveEmAndamento(TList *lisBAR)
{
	// variáveis locais
	VTBarra *barra;
	VTAcao *acao, *uma_acao;
	TList *lisACAO;

	// obtém última Acao inserida
	if ((acao = Obra->LastAcao()) == NULL)
		return (NULL);
	// verifica se a última Acao é composta
	if (acao->Tipo() != acaoCOMPOSTA)
		return (NULL);
	// verifica se o número Acoes é menor que o número de Barras
	lisACAO = acao->LisAcao();
	if (lisACAO->Count < lisBAR->Count)
		return (NULL);
	// avalia cada Barra
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		uma_acao = (VTAcao*)lisACAO->Items[n];
		// verifica se a  Acao é de alteração da Barra
		if (uma_acao->Tipo() != acaoALTERA)
			return (NULL);
		// verifica o Eqpto original da Acao é a Barra
		if (uma_acao->Eqpto() != barra)
			return (NULL);
	}
	return (acao);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereAlteraRetiraLisEqpto(TList *lisEQP_INS, TList *lisEQP_ALT,
	TList *lisEQP_RET)
{
	// variáveis locais
	VTAcao *acao_composta;
	VTEqpto *eqpto, *eqpto_alt;

	// cria Acao composta
	if ((acao_composta = Obra->AddAcaoComposta()) == NULL)
		return (false);
	// verifica se foi definida a lista lisEQP_INS
	if (lisEQP_INS != NULL)
	{ // cria uma Acao p/ cada Eqpto da lista lisEQP_INS
		for (int n = 0; n < lisEQP_INS->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP_INS->Items[n];
			Obra->AddAcaoInsere(eqpto, acao_composta);
		}
	}
	// verifica se foi definida a lista lisEQP_ALT
	if (lisEQP_ALT != NULL)
	{ // cria uma Acao p/ cada Eqpto da lista lisEQP_ALT
		for (int n = 0; n < lisEQP_ALT->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP_ALT->Items[n];
			eqpto_alt = (VTEqpto*)eqpto->Obj;
			eqpto->Obj = NULL;
			eqpto_alt->Obj = NULL;
			Obra->AddAcaoAltera(eqpto, eqpto_alt, acao_composta);
		}
	}
	// verifica se foi definida a lista lisEQP_RET
	if (lisEQP_RET != NULL)
	{ // monta lista com todos Eqptos que serão retirados
		lisRET->Clear();
		for (int n = 0; n < lisEQP_RET->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP_RET->Items[n];
			MontaLisEqptoRetira(eqpto, lisRET);
		}
		// cria uma Acao p/ cada Eqpto da lista lisRET
		for (int n = 0; n < lisRET->Count; n++)
		{
			eqpto = (VTEqpto*)lisRET->Items[n];
			Obra->AddAcaoRetira(eqpto, acao_composta);
		}
	}
	// executa Acao composta
	return (modifica->Do(Obra, acao_composta));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereBarra(VTBarra *pbarra)
{
	// variáveis locais
	VTAcao *acao;

	// cria Acao p/ Undo/Redo
	if ((acao = Obra->AddAcaoInsere(pbarra)) == NULL)
		return (false);
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereEqbar(VTEqbar *eqbar, VTBarra *pbarra)
{
	// variáveis locais
	VTAcao *acao;

	// associa Eqbar c/ sua Barra
	eqbar->DefineObjBarra(pbarra);
	// cria Acao p/ Undo/Redo
	if ((acao = Obra->AddAcaoInsere(eqbar)) == NULL)
		return (false);
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereLigacao(VTLigacao *pliga, VTRede *rede)
{
	// variáveis locais
	VTAcao *acao;

	// associa Ligacao c/ sua Rede
	pliga->DefineObjRede(rede);
	// cria Acao p/ Undo/Redo
	if ((acao = Obra->AddAcaoInsere(pliga)) == NULL)
		return (false);
	// executa Acao
	return (modifica->Do(Obra, acao));
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEdita::InsereLisEqpto(TList *lisEQP)
 {
 //variáveis locais
 VTAcao  *acao;
 VTEqpto *eqpto;

 //verifica se há um único Eqpto em lisEQP
 if (lisEQP->Count == 1)
 {//cria Acao p/ Undo/Redo de retirada de um 1 Eqpto
 eqpto = (VTEqpto*)lisEQP->First();
 if ((acao = Obra->AddAcaoInsere(eqpto)) == NULL) return(false);
 }
 else
 {//cria Acao composta
 if ((acao = Obra->AddAcaoComposta()) == NULL) return(false);
 //cria uma Acao p/ cada Barra da lista
 for (int n = 0; n < lisEQP->Count; n++)
 {
 eqpto = (VTEqpto*)lisEQP->Items[n];
 if (eqpto->TipoBarra())
 {Obra->AddAcaoInsere(eqpto, acao);}
 }
 //cria uma Acao p/ demais Eqptos da lista
 for (int n = 0; n < lisEQP->Count; n++)
 {
 eqpto = (VTEqpto*)lisEQP->Items[n];
 if (! eqpto->TipoBarra())
 {Obra->AddAcaoInsere(eqpto, acao);}
 }
 LisEqptosLibera();
 }
 return (modifica->Do(Obra, acao));
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15599530
bool __fastcall TEdita::InsereLisEqpto(TList *lisEQP)
{
	// variáveis locais
	VTAcao *acao;
	VTEqpto *eqpto;
	VTRede *rede;

	// verifica se há um único Eqpto em lisEQP
	if (lisEQP->Count == 1)
	{ // cria Acao p/ Undo/Redo de retirada de um 1 Eqpto
		eqpto = (VTEqpto*)lisEQP->First();
		//Movido para o TModifica :: InsereRede
//		if(eqpto->Tipo() == eqptoREDE)
//		{
//			rede = (VTRede*) eqpto;
//			Obra->AddRede(rede);
//		}
		if ((acao = Obra->AddAcaoInsere(eqpto)) == NULL)
			return (false);
	}
	else
	{ // ordena Eqptos em listas de Clusters, Redes, Barras e demais eqptos
		LisEqptosAloca(lisEQP);
		// cria Acao composta
		if ((acao = Obra->AddAcaoComposta()) == NULL)
			return (false);
		// cria uma Acao p/ cada Cluster da lista insere.lisCLUSTER
		for (int n = 0; n < insere.lisCLUSTER->Count; n++)
		{
			eqpto = (VTEqpto*)insere.lisCLUSTER->Items[n];
			Obra->AddAcaoInsere(eqpto, acao);
		}
		// cria uma Acao p/ cada Barra da lista insere.lisBARRA
		for (int n = 0; n < insere.lisBARRA->Count; n++)
		{
			eqpto = (VTEqpto*)insere.lisBARRA->Items[n];
			Obra->AddAcaoInsere(eqpto, acao);
		}
		// cria uma Acao p/ cada Rede da lista insere.lisREDE
		for (int n = 0; n < insere.lisREDE->Count; n++)
		{
			eqpto = (VTEqpto*)insere.lisREDE->Items[n];
			rede = (VTRede*) eqpto;
			Obra->AddAcaoInsere(eqpto, acao);
			Obra->AddRede(rede);
		}
		// cria uma Acao p/ demais Eqptos da lista insere.lisEQP
		for (int n = 0; n < insere.lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)insere.lisEQP->Items[n];
			Obra->AddAcaoInsere(eqpto, acao);
		}
		// destrói listas usadas p/ ordenadr Eqptos
		LisEqptosLibera();
	}
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereMutua(VTMutua *mutua)
{
	// variáveis locais
	VTAcao *acao;

	// verifica se os Trechos pertencem à mesma Rede
	if ((mutua->ptre1->rede != mutua->ptre2->rede))
		return (false);
	// verifica se a Rede é estática (não reconfigurável)
	if (!mutua->ptre1->rede->Estatica)
		return (false);
	// cria Acao p/ Undo/Redo
	if ((acao = Obra->AddAcaoInsere(mutua)) == NULL)
		return (false);
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereObra(VTObra *obra)
{
	// variáveis locais
	VTAcao *acao;
	TList *lisACAO = Obra->LisAcao;

	// transfere todas as Acoes de lisACAO p/ Obra
	for (int n = 0; n < lisACAO->Count; n++)
	{
		acao = (VTAcao*)lisACAO->Items[n];
		// insere Acao em Obra
		Obra->AddAcao(acao);
		// executa Acao
		modifica->Do(Obra, acao);
	}
	// limpa lisACAO da Obra indicada
	lisACAO->Clear();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::InsereRede(VTRede *rede)
{
	// variáveis locais
	VTAcao *acao;
	bool sucesso = false;

	// cria Acao p/ Undo/Redo
	if ((acao = Obra->AddAcaoInsere(rede)) == NULL)
		return (false);
	// executa Acao
	sucesso = modifica->Do(Obra, acao);
	if (sucesso)
	{
		Obra->AddRede(rede);
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::LisEqptosAloca(TList *lisEQP)
{
	// variáveis locais
	VTEqpto *eqpto;

	// proteção: destrói listas caso já existam
	LisEqptosLibera();
	// cria listas
	insere.lisCLUSTER = new TList();
	insere.lisREDE = new TList();
	insere.lisBARRA = new TList();
	insere.lisEQP = new TList();
	// separa Clusters, Redes e Barras e demais eqptos recebidos em lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		switch (eqpto->Tipo())
		{
		case eqptoBARRA:
			insere.lisBARRA->Add(eqpto);
			break;
		case eqptoCLUSTER:
			insere.lisCLUSTER->Add(eqpto);
			break;
		case eqptoREDE:
			insere.lisREDE->Add(eqpto);
			break;
		default:
			insere.lisEQP->Add(eqpto);
			break;
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::LisEqptosLibera(void)
{
	// destrói listas sem destruir seus objetos
	if (insere.lisCLUSTER)
	{
		delete insere.lisCLUSTER;
		insere.lisCLUSTER = NULL;
	}
	if (insere.lisREDE)
	{
		delete insere.lisREDE;
		insere.lisREDE = NULL;
	}
	if (insere.lisBARRA)
	{
		delete insere.lisBARRA;
		insere.lisBARRA = NULL;
	}
	if (insere.lisEQP)
	{
		delete insere.lisEQP;
		insere.lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::Manobra(VTChave *pchave)
{
	// proteção
	if (pchave == NULL)
		return (false);
	// reinicia lisTMP c/ Chave
	lisTMP->Clear();
	lisTMP->Add(pchave);
	// executa Manobra p/ lista de Chave
	return (Manobra(lisTMP));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::Manobra(TList *lisCHV)
{
	// variáveis locais
	VTAcao *acao_composta;
	VTChave *pchave, *pchave_alt;

	// determina conjunto de Chaves manobradas em função da existência de sincronismo
	lisCHV = sincroniza->Manobra(lisCHV);
	// cria Acao composta
	if ((acao_composta = Obra->AddAcaoComposta()) == NULL)
		return (false);
	// cria uma Acao p/ alteração de cada Chave
	for (int n = 0; n < lisCHV->Count; n++)
	{
		pchave = (VTChave*)lisCHV->Items[n];
		// cria clone da Chave antes de mudar seu estado
		if ((pchave_alt = pchave->Clone()) == NULL)
			return (false);
		(pchave_alt->Aberta) ? pchave_alt->Fecha() : pchave_alt->Abre();
		// insere operação p/ Undo/Redo
		if (Obra->AddAcaoAltera(pchave, pchave_alt, acao_composta) == NULL)
			return (false);
	}
	// executa Acao
	return (modifica->Do(Obra, acao_composta));
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::MontaLisCanalMedidorRetira(VTEqpto *eqpto, TList *lisEQP)
{
	// variáveis locais
	VTBarra *barra;
	VTLigacao *ligacao;
	VTMedidor *medidor;
	VTCanal *canal;
	TList *lisCANAL;

	// monta lisTMP c/ Medidores que podem ter Canal p/ o Eqpto indicado
	lisTMP->Clear();
	if (eqpto->TipoBarra())
	{
		barra = (VTBarra*)eqpto;
		barra->LisEqbar(lisTMP, eqptoMEDIDOR);
	}
	else if (eqpto->TipoEqbar())
	{
		barra = ((VTEqbar*)eqpto)->pbarra;
		barra->LisEqbar(lisTMP, eqptoMEDIDOR);
	}
	else if (eqpto->TipoLigacao())
	{
		ligacao = (VTLigacao*)eqpto;
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao->Barra(ind_bar)) != NULL)
			{
				barra->LisEqbar(lisTMP, eqptoMEDIDOR);
			}
		}
	}
	// loop p/ todos os Medidores em lisTMP
	for (int n = 0; n < lisTMP->Count; n++)
	{
		medidor = (VTMedidor*)lisTMP->Items[n];
		lisCANAL = medidor->LisCanal();
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto == eqpto)
			{ // insere Canal em lisEQP
				if (lisEQP->IndexOf(canal) < 0)
					lisEQP->Add(canal);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::MontaLisEqptoRetira(VTEqpto *eqpto, TList *lisEQP)
{
	// variáveis locais
	bool visible = false;
	TList *lisLIG;
	VTBarra *barra;
	VTRede *rede;
	VTTrecho *trecho;
	VTBusca *busca = (VTBusca*)apl->GetObject(__classid(VTBusca));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// proteção: verifica se o Eqpto já está em lisEQP
	if (lisEQP->IndexOf(eqpto) >= 0)
		return;
	// verifica o tipo de Eqpto
	if (eqpto->Tipo() == eqptoREDE)
	{ // retira todas Mutuas, Ligacoes, Eqbar e Barras exclusivas da Rede
		rede = (VTRede*)eqpto;
		// retira todas as Mutuas
		CopiaTList(rede->LisMutua(), lisEQP);
		// retira todas Ligacoes
		CopiaTList(rede->LisLigacao(), lisEQP);
		// obtém lista de Barras exclusivas da Rede
		lisTMP->Clear();
		redes->LisBarraExclusiva(lisTMP, rede);
		// retira todos Eqbar
		for (int nb = 0; nb < lisTMP->Count; nb++)
		{
			barra = (VTBarra*)lisTMP->Items[nb];
			CopiaLisOrigLisDest(barra->LisEqbar(), lisEQP);
		}
		// retira todas Barras exclusivas
		CopiaTList(lisTMP, lisEQP);
	}
	else if (eqpto->TipoBarra())
	{ // verifica se Barra inicial de Rede
		barra = (VTBarra*)eqpto;
		if (BarraInicialDeRede(barra))
			return;
		// retira Eqbars conectados à Barra
		CopiaLisOrigLisDest(barra->LisEqbar(), lisEQP);
		// retira Ligacoes que se conectam à Barra
		CopiaLisOrigLisDest(busca->Ligacao(barra, visible), lisEQP);
		// retira todas Mutuas entre Trechos que conectam a Barra
		CopiaLisOrigLisDest(busca->Mutua(barra), lisEQP);
	}
	else if (eqpto->Tipo() == eqptoTRECHO)
	{ // retira todas Mutuas do Trecho
		trecho = (VTTrecho*)eqpto;
		trecho->LisMutua(lisEQP);
		// retira Canal de Medidor
		MontaLisCanalMedidorRetira(eqpto, lisEQP);
	}
	else if (eqpto->Tipo() == eqptoMEDIDOR)
	{ // retira Canal de Medidor
		MontaLisCanalMedidorRetira(eqpto, lisEQP);
	}
	// insere o próprio Eqpto em lisEQP
	lisEQP->Add(eqpto);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::MoveBarra(VTBarra *pbarra, int dx, int dy, bool moving)
{
	// variáveis locais
	VTAcao *acao;
	VTBarra *depois;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica se existe uma Acao de Move em andamento p/ a Barra indicada
	if ((acao = ExisteMoveEmAndamento(pbarra)) == NULL)
	{ // cria cópia da Barra
		if ((depois = pbarra->Clone()) == NULL)
			return (false);
		// cria Acao
		if ((acao = Obra->AddAcaoAltera(pbarra, depois)) == NULL)
			return (false);
	}
	acao->Moving = moving;
	// atualiza coordenadas da Barra alterada
	depois = (VTBarra*)(acao->CopiaDoEqptoAlterado());
	// verifica se deve alterar cordenadas esq ou utm
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // atualiza coordenadas utm
		depois->utm.x += dx;
		depois->utm.y += dy;
	}
	else
	{ // atualiza coordenadas esq
		depois->esq.x1 += dx;
		depois->esq.y1 += dy;
		depois->esq.x2 += dx;
		depois->esq.y2 += dy;
	}
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::MoveBarra(VTBarra *pbarra, int x1, int y1, int x2, int y2, bool moving)
{
	// variáveis locais
	VTAcao *acao;
	VTBarra *depois;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica se existe uma Acao de Move em andamento p/ a Barra indicada
	if ((acao = ExisteMoveEmAndamento(pbarra)) == NULL)
	{ // cria cópia da Barra
		if ((depois = pbarra->Clone()) == NULL)
			return (false);
		// cria Acao
		if ((acao = Obra->AddAcaoAltera(pbarra, depois)) == NULL)
			return (false);
	}
	acao->Moving = moving;
	// atualiza coordenadas da Barra alterada
	depois = (VTBarra*)(acao->CopiaDoEqptoAlterado());
	// verifica se deve alterar cordenadas esq ou utm
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // atualiza coordenadas utm
		depois->utm.x += x1;
		depois->utm.y += y1;
	}
	else
	{ // atualiza coordenadas esq
		depois->esq.x1 = x1;
		depois->esq.y1 = y1;
		depois->esq.x2 = x2;
		depois->esq.y2 = y2;
	}
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::MoveBarra(TList *lisBAR, int dx, int dy, bool moving)
{
	// variáveis locais
	VTAcao *acao_composta, *acao;
	VTBarra *pbarra, *pbar_alterada;
	VTEqpto *eqpto;
	TList *lisACAO;
	TList *lisPTO;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica se lisBAR possui uma única Barra
	if (lisBAR->Count == 1)
	{ // executa move de barra única
		return (MoveBarra((VTBarra*)lisBAR->First(), dx, dy, moving));
	}
	// verifica se existe uma Acao de Move em andamento p/ a lista de Barras indicada
	if ((acao_composta = ExisteMoveEmAndamento(lisBAR)) == NULL)
	{ // cria Acao composta
		if ((acao_composta = Obra->AddAcaoComposta()) == NULL)
			return (false);
		// cria uma Acao p/ alteração de cada Barra
		for (int n = 0; n < lisBAR->Count; n++)
		{
			pbarra = (VTBarra*)lisBAR->Items[n];
			// cria uma cópia da Barra e redefine suas coordenadas
			if ((pbar_alterada = pbarra->Clone()) == NULL)
				return (false);
			// cria Acao
			if (Obra->AddAcaoAltera(pbarra, pbar_alterada, acao_composta) == NULL)
				return (false);
		}
	}
	acao_composta->Moving = moving;
	// Move em andamento através de vários passos
	lisACAO = acao_composta->LisAcao();
	for (int n = 0; n < lisACAO->Count; n++)
	{
		acao = (VTAcao*)lisACAO->Items[n];
		// determina se o Eqpto da Acao é uma Barra
		eqpto = (VTEqpto*)(acao->CopiaDoEqptoAlterado());
		if (!eqpto->TipoBarra())
			continue;
		pbar_alterada = (VTBarra*)eqpto;
		// verifica se deve alterar cordenadas esq ou utm
		if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
		{ // atualiza coordenadas utm
			pbar_alterada->utm.x += dx;
			pbar_alterada->utm.y += dy;
		}
		else
		{ // atualiza coordenadas esq
			pbar_alterada->esq.x1 += dx;
			pbar_alterada->esq.y1 += dy;
			pbar_alterada->esq.x2 += dx;
			pbar_alterada->esq.y2 += dy;
		}
	}
	return (modifica->Do(Obra, acao_composta));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::MoveEqbar(VTEqbar *eqbar, int x, int y, bool moving)
{
	// variáveis locais
	VTAcao *acao;
	VTEqbar *depois;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// bloqueado no modo geo-referenciado
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
		return (true);
	// verifica se existe uma Acao de Move em andamento p/ o Eqbar indicada
	if ((acao = ExisteMoveEmAndamento(eqbar)) != NULL)
	{ // Move em andamento através de vários passos
		depois = (VTEqbar*)(acao->CopiaDoEqptoAlterado());
		// redefine coordenadas do Eqbar
		depois->DefinePosicao(x, y);
		acao->Moving = moving;
		// executa Acao
		return (modifica->Do(Obra, acao));
	}
	// cria cópia do Eqbar e redefine suas coordenadas
	if ((depois = eqbar->Clone()) == NULL)
		return (false);
	depois->DefinePosicao(x, y);
	// insere operação p/ Undo/Redo
	if ((acao = Obra->AddAcaoAltera(eqbar, depois)) == NULL)
		return (false);
	acao->Moving = moving;
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::MoveLigacao(VTLigacao *pliga, VTBarra *pbar, int x, int y, bool moving)
{
	// variáveis locais
	VTAcao *acao;
	VTLigacao *depois;

	// verifica se existe uma Acao de Move em andamento p/ a Ligacao indicada
	if ((acao = ExisteMoveEmAndamento(pliga)) != NULL)
	{ // Move em andamento através de vários passos
		depois = (VTLigacao*)(acao->CopiaDoEqptoAlterado());
		// redefine coordenadas da Ligacao
		depois->DefinePosBar(pbar, x, y);
		acao->Moving = moving;
		return (modifica->Do(Obra, acao));
	}
	// cria cópia da Ligacao e redefine suas coordenadas
	if ((depois = pliga->Clone()) == NULL)
		return (false);
	depois->DefinePosBar(pbar, x, y);
	// verifica se houve deslocamento da Ligacao
	if ((moving) && (pliga->esq.posbar1 == depois->esq.posbar1) &&
		(pliga->esq.posbar2 == depois->esq.posbar2) && (pliga->esq.posbar3 == depois->esq.posbar3))
	{ // Ligacao não foi deslocada: destrói objeto depois
		delete depois;
		return (true);
	}
	// insere operação p/ Undo/Redo
	if ((acao = Obra->AddAcaoAltera(pliga, depois)) == NULL)
		return (false);
	acao->Moving = moving;
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
VTAutoTag* __fastcall TEdita::PM_GetAutoTag(void)
{
	return (autoTag);
}
//---------------------------------------------------------------------------
bool __fastcall TEdita::PM_GetAutoTagHabilitado(void)
{
	bool existe = false;

	if(autoTag != NULL)
	{
		existe = true;
    }

	return (existe);
}
//---------------------------------------------------------------------------
void __fastcall TEdita::PM_SetAutoTagHabilitado(bool habilitado)
{
	if (habilitado == true)
	{
		if (autoTag == NULL)
		{
//			apl->Add(autoTag = DLL_NewObjAutoTag(apl));
			autoTag = DLL_NewObjAutoTag(apl);
			autoTag->Habilitado = true;
		}
	}
	else
	{
		if (autoTag != NULL)
		{
//			apl->Remove(autoTag);
			delete autoTag;
			autoTag = NULL;
		}
    }

}
// ---------------------------------------------------------------------------
AnsiString __fastcall TEdita::PM_GetNome(void)
{
	return ("Editor: " + AnsiQuotedStr(Obra->Nome, '\''));
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TEdita::PM_GetObra(void)
{
	// verifica se existe Obra externa
	if (PD.obra_externa != NULL)
		return (PD.obra_externa);
	// retorna Obra local
	return (PD.obra_local);
}

// ---------------------------------------------------------------------------
void __fastcall TEdita::PM_SetObra(VTObra *obra)
{
	// variáveis locais
	TForm *form;
	// VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// IMPORTANTE: proteção contra os programadores - não permite que se defina
	// a obra local como obra externa
	if (obra == PD.obra_local)
	{
		obra = NULL;
	}
	// salva Obra externa
	PD.obra_externa = obra;
	// associa Obra c/ Actions
	Obra->DefineActions(ActionUndo, ActionRedo);
	// atualiza Caption do Editor (se o Form estiver ativo)
	if ((form = EditorAtivo()) != NULL)
		form->Caption = Nome;
	// atualiza o grafico
	// if (grafico) grafico->Redraw();
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEdita::RedoCompleto(void)
 {
 //variáveis locais
 int       count = 0;
 VTAcao    *acao;
 VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

 //reinicia lista de lisUNDO (p/ descartar Acoes que foram refeitas)
 Obra->CancelaUndo();
 //executa todas Acoes de Redo
 while ((acao = Obra->Redo()) != NULL)
 {//executa a Acao que não pode ser tratada em conjunto com as demais
 modifica->Redo(Obra, acao);
 }
 //atualiza o gráfico
 if ((count > 0)&&(grafico)) grafico->Redraw();
 return(true);
 }
 */
/*
 //---------------------------------------------------------------------------
 bool __fastcall TEdita::RedoCompleto(void)
 {
 //variáveis locais
 int       count = 0;
 VTAcao    *acao;
 VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

 //reinicia lista de lisUNDO (p/ descartar Acoes que foram refeitas)
 Obra->CancelaUndo();
 //monta lista com todas Acoes de Redo
 lisTMP->Clear();
 while ((acao = Obra->Redo()) != NULL)
 {//verifica se Acao pode ser executada dentro de Undo/Redo completo
 if (ValidaUndoRedoCompleto(acao))
 {//insere Acao em lisTMP
 lisTMP->Add(acao);
 }
 else
 {//efetiva as Acoes já incluídas em lisTMP
 if (lisTMP->Count > 0)
 {
 modifica->Redo(Obra, lisTMP);
 //reinicia lisTMP
 lisTMP->Clear();
 }
 //executa a Acao que não pode ser tratada em conjunto com as demais
 modifica->Redo(Obra, acao);
 }
 count++;
 }
 //executa Acoes em lisTMP
 if (lisTMP->Count > 0) modifica->Redo(Obra, lisTMP);
 //atualiza o gráfico
 if ((count > 0)&&(grafico)) grafico->Redraw();
 return(true);
 }
 */

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
bool __fastcall TEdita::RedoCompleto(void)
{
	// variáveis locais
	VTAcao *acao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// reinicia lista de lisUNDO (p/ descartar Acoes que foram refeitas)
	Obra->CancelaUndo();
	// monta lista com redes da Obra
	ru.lisREDE->Clear();
	CopiaTList(Obra->LisRede(), ru.lisREDE);
	// monta lista com todas Acoes de Redo
	ru.lisACAO->Clear();
	while ((acao = Obra->Redo()) != NULL)
	{ // proteção: garante que Acao está associada à Obra
		acao->Obra = Obra;
		// insere Acao em ru.lisACAO
		ru.lisACAO->Add(acao);
	}
	// executa Acoes em ru.lisACAO
	if (ru.lisACAO->Count > 0)
		modifica->Redo(ru.lisREDE, ru.lisACAO);
	// atualiza o gráfico
	if ((ru.lisACAO->Count > 0) && (grafico))
		grafico->Redraw();
	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TEdita::RedoLisObra(TList *lisOBRA)
{
	// variáveis locais
	VTAcao *acao;
	// VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// proteção
	if (lisOBRA->Count == 0)
		return (true);
	// monta lista de Redes e de Acoes de Redo de todas as Obras em lisOBRA
	ru.lisREDE->Clear();
	ru.lisACAO->Clear();
	// loop p/ todas Obras em lisOBR
	for (int n = 0; n < lisOBRA->Count; n++)
	{ // define Obra deste objeto Edita
		Obra = (VTObra*)lisOBRA->Items[n];
		// inclui Redes da Obra em ru.lisREDE
//		CopiaTList(Obra->LisRede(), ru.lisREDE);
		CopiaTListRedes(Obra->LisRede(), ru.lisREDE);
		// reinicia lista de lisUNDO (p/ descartar Acoes que foram refeitas)
		Obra->CancelaUndo();

#ifdef _DEBUG //teste em modo debug =====================
		int last_index = -1;
#endif        //fim teste em modo debug =================

		while ((acao = Obra->Redo()) != NULL)
		{ // proteção: garante que Acao está associada à Obra
			acao->Obra = Obra;
			// insere Acao em ru.lisACAO
			ru.lisACAO->Add(acao);

#ifdef _DEBUG //teste em modo debug =====================
			if (last_index == -1)
			{
				last_index = acao->Index;
			}
			else if (acao->Index <= last_index)
			{ // condição de erro
				Erro("TEdita::RedoLisObra()\nErro na sequencia das ações");
			}
			// salva index da da última Acao da Obra
			last_index = acao->Index;
#endif        //fim teste em modo debug =================

		}
	}
	// executa Acoes em ru.lisACAO
	if (ru.lisACAO->Count > 0)
		modifica->Redo(ru.lisREDE, ru.lisACAO);
	// atualiza o gráfico
	// if (grafico) grafico->Redraw();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::Retira(VTEqpto *eqpto)
{
	// monta lista de Eqptos que serão retirados
	lisRET->Clear();
	MontaLisEqptoRetira(eqpto, lisRET);
	// retira de lista de Equipamentos selecionados
	return (RetiraLisEqptoSelecionado(lisRET));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18599859
bool __fastcall TEdita::RetiraEqptosIsolados(bool apaga_tudo)
{
    // variáveis locais
    VTBarra *barra;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisEQP_ISO = new TList();
    TList *lisBARRA = redes->RedeIsolada()->LisBarra();
    bool sucesso = false;

    if(apaga_tudo)
    {// lista ligações da rede isolada
        CopiaTList(redes->RedeIsolada()->LisLigacao(), lisEQP_ISO);
        // lista barras da rede isolada
        CopiaTList(lisBARRA, lisEQP_ISO);
    }
    else
    {// verifica se há Barras isoladas sem eqbar
        for (int n = 0; n < lisBARRA->Count; n++)
        {
            barra = (VTBarra*)lisBARRA->Items[n];
            if((barra->LisEqbar()->Count == 0) && (barra->LisLigacao()->Count == 0))
            {
                lisEQP_ISO->Add(barra);
            }
        }
    }
    // apaga tudo o que estiver isolado e foi selecionado
    sucesso = RetiraLisEqpto(lisEQP_ISO);
    // destroi lista
    delete lisEQP_ISO;
    return sucesso;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18070667
// https://sinapsisenergia.teamwork.com/#tasks/18261839
bool __fastcall TEdita::RetiraLisEqpto(TList *lisEXT)
{
	// variáveis locais
	VTEqpto *eqpto;
    VTRede  *rede;
    bool     ignoraRede = false;

	// monta lista de Eqptos que serão retirados
	lisRET->Clear();
	for (int n = 0; n < lisEXT->Count; n++)
	{
		eqpto = (VTEqpto*)lisEXT->Items[n];
        rede  = (VTRede*)eqpto;
        ignoraRede = false;
        // ignora redes se ela foi reduzida ou não está carregada
        if(eqpto->Tipo() == eqptoREDUCAO)
            ignoraRede = true;
        else if(eqpto->Tipo() == eqptoREDE &&
                !rede->Carregada)
            ignoraRede = true;
        if(ignoraRede)
        {
            Aviso("A rede " + rede->Codigo + " foi reduzida ou não está carregada e não pode ser excluída");
            continue;
        }
		MontaLisEqptoRetira(eqpto, lisRET);
	}
	// retira lista de Equipamentos selecionados
	return (RetiraLisEqptoSelecionado(lisRET));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::RetiraLisEqptoSelecionado(TList *lisEQP)
{
	// variáveis locais
	VTAcao *acao;
	VTEqpto *eqpto;

	// proteção
	if (lisEQP->Count == 0)
		return (true);
	// verifica se há apenas um Eqpto a ser retirado
	if (lisEQP->Count == 1)
	{ // cria Acao p/ Undo/Redo de retirada de um 1 Eqpto
		eqpto = (VTEqpto*)lisEQP->First();
		if ((acao = Obra->AddAcaoRetira(eqpto)) == NULL)
			return (false);
	}
	else
	{ // cria Acao composta p/ Undo/Redo de retirada de N Eqptos
		if ((acao = Obra->AddAcaoComposta()) == NULL)
			return (false);
		// cria uma Acao p/ cada Eqpto da lista
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[n];
			Obra->AddAcaoRetira(eqpto, acao);
		}
	}
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::RetiraLisRede(TList *lisEXT)
{
	// variáveis locais
	bool sucesso;
	VTEqpto *eqpto;
	VTCorte *corte;
	VTRede *rede1, *rede2;
	TList *lisEQP;

	try
	{ // monta lisTMP c/ Barras dos Cortes existentes entre essas redes
		lisTMP->Clear();
		for (int i = 0; i < lisEXT->Count; i++)
		{
			eqpto = (VTEqpto*)lisEXT->Items[i];
			if (eqpto->Tipo() != eqptoREDE)
				continue;
			rede1 = (VTRede*)eqpto;
			for (int j = i + 1; j < lisEXT->Count; j++)
			{
				eqpto = (VTEqpto*)lisEXT->Items[j];
				if (eqpto->Tipo() != eqptoREDE)
					continue;
				rede2 = (VTRede*)eqpto;
				// determina Corte entre Rede1 e Rede2
				if ((corte = rede1->ExisteCorte(rede2)) != NULL)
				{ // verifica se todas Redes do Corte estão em lisEXT
					if (Lista1ContemLista2(lisEXT, corte->LisRede()))
					{
						CopiaLisOrigLisDest(corte->LisBarra(), lisTMP);
					}
				}
			}
		}
		// inicia lisEQP com lista das Redes e das Barras dos Cortes
		lisEQP = new TList();
		CopiaTList(lisEXT, lisEQP);
		CopiaTList(lisTMP, lisEQP);
		// retira Redes e seus Eqpos
		sucesso = RetiraLisEqpto(lisEQP);
		// destrói lista temporária
		if (lisEQP)
			delete lisEQP;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::RotacionaBarras(VTBarra *barra_ref, double ang_grau)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cancela Undo/Redo
	Clear();
	// monta esquemático igual c/ coordendas Y invertidas
	esquematico->RotacionaBarras(barra_ref, ang_grau);
	// atualiza gráfico
	grafico->AreaRedeAltera();
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
bool __fastcall TEdita::TrocaBarraLigacao(VTLigacao *pliga, VTBarra *pbar_orig, VTBarra *pbar_nova)
{
	// variáveis locais
	VTAcao *acao;
	VTLigacao *depois;
	VTRegulador *regulador;
	VTTrafo *trafo;
    TList *lisLIG_BARRA;

	// verifica se existe uma Acao de Move em andamento p/ a Ligacao indicada
	if ((acao = ExisteMoveEmAndamento(pliga)) != NULL)
	{ // Move em andamento através de vários passos
		depois = (VTLigacao*)(acao->CopiaDoEqptoAlterado());
	}
	else
	{ // cria cópia da Ligacao
		if ((depois = pliga->Clone()) == NULL)
			return (false);
	}
    //retira da lista da barra a ligação original
    lisLIG_BARRA = pbar_orig->LisLigacao();
    lisLIG_BARRA->Remove(pliga);
	// redefine Barra da Ligacao
	if (pbar_orig == depois->pbarra1)
    {
		depois->pbarra1 = pbar_nova;
    }
	else if (pbar_orig == depois->pbarra2)
    {
		depois->pbarra2 = pbar_nova;
    }
	else if (pbar_orig == depois->pbarra3)
    {
		depois->pbarra3 = pbar_nova;
    }
	// verifica se a Ligacao é um Regulador
	if (depois->Tipo() == eqptoREGULADOR)
	{
		regulador = (VTRegulador*)depois;
		// verifica se a Barra original também é de referência para ajuste automático
		if (regulador->cfg_auto.bar_ref == pbar_orig)
		{ // redefine Barra de referência
			regulador->cfg_auto.bar_ref = pbar_nova;
		}
		// verifica se a Barra original também é de referência para o sentido do fluxo
		if (regulador->sentido.bar_ref == pbar_orig)
		{ // redefine Barra de referência
			regulador->sentido.bar_ref = pbar_nova;
		}
	}
	// verifica se a Ligacao é um Trafo
	else if (depois->Tipo() == eqptoTRAFO)
	{ // verifica se a Barra desligada era de referência
		trafo = (VTTrafo*)depois;
		if (trafo->ltc.ajuste_auto.bar_ref == pbar_orig)
		{ // redefine Barra de referência
			trafo->ltc.ajuste_auto.bar_ref = pbar_nova;
		}
	}
	// verifica se deve criar uma nova Acao
	if (acao == NULL)
	{ // insere operação p/ Undo/Redo
		if ((acao = Obra->AddAcaoAltera(pliga, depois)) == NULL)
			return (false);
	}
	// executa Acao
	return (modifica->Do(Obra, acao));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::TrocaBarraEqbar(VTEqbar *eqbar, VTBarra *pbar_nova)
{
	// variáveis locais
	VTAcao *acao;
	VTEqbar *depois;

	// cria cópia do Eqbar e redefine sua Barra
	if ((depois = eqbar->Clone()) == NULL)
		return (false);
	// verifica se é um Medidor
	if (depois->Tipo() == eqptoMEDIDOR)
	{ // elimin todos os Canais
		((VTMedidor*)depois)->Clear();
	}
	// redefine Barra do Eqbar
	depois->pbarra = pbar_nova;
	// insere operação p/ Undo/Redo
	if ((acao = Obra->AddAcaoAltera(eqbar, depois)) == NULL)
		return (false);
	// executa Acao
	return (modifica->Do(Obra, acao));
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEdita::UndoCompleto(void)
 {
 //variáveis locais
 int       count = 0;
 VTAcao    *acao;
 VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

 //reinicia lista de lisREDO (p/ descartar Acoes que foram desfeitas)
 Obra->CancelaRedo();
 while ((acao = Obra->Undo()) != NULL)
 {//executa a Acao
 modifica->Undo(Obra, acao);
 count++;
 }
 //atualiza o gráfico
 if ((count > 0)&&(grafico)) grafico->Redraw();
 return(true);
 }
 */
/*
 //---------------------------------------------------------------------------
 bool __fastcall TEdita::UndoCompleto(void)
 {
 //variáveis locais
 int       count = 0;
 VTAcao    *acao;
 VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

 //reinicia lista de lisREDO (p/ descartar Acoes que foram desfeitas)
 Obra->CancelaRedo();
 //monta lista com todas Acoes de Undo
 lisTMP->Clear();
 while ((acao = Obra->Undo()) != NULL)
 {//verifica se Acao pode ser executada dentro de Undo/Redo completo
 if (ValidaUndoRedoCompleto(acao))
 {//insere Acao em lisTMP
 lisTMP->Add(acao);
 }
 else
 {//efetiva as Acoes já incluídas em lisTMP
 if (lisTMP->Count > 0)
 {
 modifica->Undo(Obra, lisTMP);
 //reinicia lisTMP
 lisTMP->Clear();
 }
 //executa a Acao que não pode ser tratada em conjunto com as demais
 modifica->Undo(Obra, acao);
 }
 count++;
 }
 //executa Acoes em lisTMP
 if (lisTMP->Count > 0) modifica->Undo(Obra, lisTMP);
 //atualiza o gráfico
 if ((count > 0)&&(grafico)) grafico->Redraw();
 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TEdita::UndoCompleto(void)
{
	// variáveis locais
	VTAcao *acao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// reinicia lista de lisREDO (p/ descartar Acoes que foram desfeitas)
	Obra->CancelaRedo();
	// monta lista com redes da Obra
	ru.lisREDE->Clear();
	CopiaTList(Obra->LisRede(), ru.lisREDE);
	// monta lista com todas Acoes de Redo
	ru.lisACAO->Clear();
	while ((acao = Obra->Undo()) != NULL)
	{ // proteção: garante que Acao está associada à Obra
		acao->Obra = Obra;
		// insere Acao em lisTMP
		ru.lisACAO->Add(acao);
	}
	// executa Acoes em lisTMP
	if (ru.lisACAO->Count > 0)
		modifica->Undo(ru.lisREDE, ru.lisACAO);
	// atualiza o gráfico
//	if ((lisTMP->Count > 0) && (grafico))
	if ((ru.lisACAO->Count > 0) && (grafico))
//		grafico->Redraw();
		grafico->AreaRedeDefine();
	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TEdita::UndoLisObra(TList *lisOBRA)
{
	// variáveis locais
	VTAcao *acao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// proteção
	if (lisOBRA->Count == 0)
		return (true);
	// monta lista de Redes e de Acoes de Undo de todas as Obras em lisOBRA
	ru.lisREDE->Clear();
	ru.lisACAO->Clear();
	// loop p/ todas Obras em lisOBRA, sendtido inverso
	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{ // define Ora deste objeto Edita
		Obra = (VTObra*)lisOBRA->Items[n];
		// inclui Redes da Obra em ru.lisREDE
//		CopiaTList(Obra->LisRede(), ru.lisREDE);
        CopiaTListRedes(Obra->LisRede(), ru.lisREDE);
		// reinicia lista de lisREDO (p/ descartar Acoes que foram desfeitas)
		Obra->CancelaRedo();

#ifdef _DEBUG //teste em modo debug =====================
		int last_index = -1;

#endif        //fim teste em modo debug =================

		while ((acao = Obra->Undo()) != NULL)
		{ // proteção: garante que Acao está associada à Obra
			acao->Obra = Obra;
			// insere Acao em ru.lisACAO
			ru.lisACAO->Add(acao);

#ifdef _DEBUG //teste em modo debug =====================
			if (last_index == -1)
			{
				last_index = acao->Index;
			}
			else if (acao->Index >= last_index)
			{ // condição de erro
				Erro("TEdita::UndoLisObra()\nErro na sequencia das ações");
			}
			// salva index da da última Acao da Obra
			last_index = acao->Index;
#endif        //fim teste em modo debug =================

		}
	}
	// executa Acoes em ru.lisACAO, avaliando impacto somente após todas Ações
	if (ru.lisACAO->Count > 0)
		modifica->Undo(ru.lisREDE, ru.lisACAO);
	// atualiza o gráfico
	if (grafico)
		grafico->Redraw();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdita::ValidaUndoRedoCompleto(VTAcao *acao)
{
	// variáveis locais
	VTEqpto *eqpto;
	TList *lisACAO;

	// verifica se é uma Acao composta
	if (acao->Tipo() == acaoCOMPOSTA)
	{ // obtém lista de Acoes
		lisACAO = acao->LisAcao();
		for (int n = 0; n < lisACAO->Count; n++)
		{ // chamada recursiva
			acao = (VTAcao*)lisACAO->Items[n];
			if (!ValidaUndoRedoCompleto(acao))
				return (false);
		}
		return (true);
	}
	// proteção: verifica se Acao é inserção/retirada de uma Rede
	eqpto = acao->Eqpto();
	if (((acao->Tipo() == acaoINSERE) || (acao->Tipo() == acaoRETIRA)) &&
		(eqpto->Tipo() == eqptoREDE))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
// eof
