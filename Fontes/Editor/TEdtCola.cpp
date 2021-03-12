// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TEdtCola.h"
#include "VTDragdrop.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Busca\VTBusca.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrecho.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objetos da classe TEdtCola
// ---------------------------------------------------------------------------
VTEdt* NewObjEdtCola(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtCola(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtCola::TEdtCola(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	: TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
{
	// variáveis locais
	VTDragdrop *dragdrop = (VTDragdrop*)apl->GetObject(__classid(VTDragdrop));

	// inicia dados locais
	first_move = true;
	// cria lisBAR, lisEQP e lisREDE
	lisBAR = new TList();
	lisEQP = new TList();
	lisREDE = new TList();
	lisCLUSTER = new TList();
	// insere este objeto no objeto Dragdrop
	dragdrop->InsEdtCola(this);
	// mais nada a fazer: o contrutor de VTEdt faz todas iniciações
}

// ---------------------------------------------------------------------------
__fastcall TEdtCola::~TEdtCola(void)
{
	// variáveis locais
	VTDragdrop *dragdrop = (VTDragdrop*)apl->GetObject(__classid(VTDragdrop));

	// retira este objeto no objeto Dragdrop
	if (dragdrop)
		dragdrop->RetEdtCola(this);
	// destrói lista sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisREDE)
	{
		delete lisREDE;
		lisREDE = NULL;
	}
	if (lisCLUSTER)
	{
		delete lisCLUSTER;
		lisCLUSTER = NULL;
	}
	// atualiza gráfico
	if (!first_move)
		graf->Redraw(redrawEQP_NOVO, this);
	// destrói objetos em lisEQN
	if (lisEQN)
		LimpaTList(lisEQN);
	// mais nada a fazer: o destrutor de VTEdt faz todas finalizações
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::CopiaEqptos(void)
{
	// variáveis locais
	VTBarra *barra;
	VTCapacitor *capacitor;
	VTCarga *carga;
	VTChave *chave;
	VTCluster *cluster;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTEqpto *eqpto, *clone;
	VTLigacao *ligacao;
	VTMRede *mrede;
	VTReator *reator;
	VTRegulador *regulador;
	VTRede *rede;
	VTTrecho *trecho;
	VTArranjo *arranjo;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));

	// limpa lista lisBAR, lisCLUSTER, lisREDE
	lisBAR->Clear();
	lisREDE->Clear();
	lisCLUSTER->Clear();
	// destrói objetos em lisEQN
	LimpaTList(lisEQN);
	// NÃO permite cópia de Reducao p/ mesma Redes
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		// verifica se é uma Reducao
		if (eqpto->Tipo() == eqptoREDUCAO)
		{ // determina MRede da Reducao
			mrede = ((VTReducao*)eqpto)->MRede;
			// verifica se a MRede existe em Redes
			// if (redes->ExisteMRede(mrede->Id) != NULL)
			if (ExisteEqpto(mrede, redes->LisMRede()))
			{ // retira Reducao da lista de Eqptos
				lisEQP->Delete(n);
			}
		}
	}
	// se necessário, inclui MRede das Reducoes em lisEQP
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		// verifica se é uma Reducao
		if (eqpto->Tipo() == eqptoREDUCAO)
		{ // determina Rede da Reducao
			mrede = ((VTReducao*)eqpto)->MRede;
			// verifica se a MRede existe em Redes
			// if (redes->ExisteMRede(mrede->Id) != NULL) continue;
			if (ExisteEqpto(mrede, redes->LisMRede()))
				continue;
			// insere MRede em lisEQP p/ ser copiada
			if (lisEQP->IndexOf(mrede) < 0)
				lisEQP->Add(mrede);
		}
	}
	// determina conjuntos de Rede e Cluster que devem incluídos em lisEQP p/ cópia
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->Tipo() == eqptoREDE)
		{ // insere Rede em lisREDE
			lisREDE->Add(eqpto);
		}
		// verifica se é uma Ligacao
		else if (eqpto->TipoLigacao())
		{ // determina Rede da Ligacao (que pode não estar definida ou pode ser a Rede isolada)
			if ((rede = ((VTLigacao*)eqpto)->rede) == NULL)
				continue;
			if (rede == rede->Redes->RedeIsolada())
				continue;
			// verifica se a Rede existe em Redes (cópia sendo feita na mesma janela)
			if (ExisteEqpto(rede, redes->LisRede()))
				continue;
			// verifica se a Rede existe em Redes (cópia sendo feita entre duas janelas)
			if (ExisteRede(rede->Id, rede->Codigo, redes->LisRede()))
				continue;
			// verifica se a Rede já existe em lisREDE
			if (lisREDE->IndexOf(rede) < 0)
			{ // insere Rede em lisREDE
				lisREDE->Add(rede);
				// verifica se a Rede está associada a um Cluster
				if ((cluster = rede->Cluster) == NULL)
					continue;
				// insere Cluster em lisCLUSTER
				if (lisCLUSTER->IndexOf(cluster) < 0)
					lisCLUSTER->Add(cluster);
			}
		}
	}
	// inclui conjunto de Rede de lisREDE no final de lisEQP
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		lisEQP->Remove(rede);
		lisEQP->Add(rede);
	}
	// inclui conjunto de Cluster no final de lisEQP
	for (int n = 0; n < lisCLUSTER->Count; n++)
	{
		cluster = (VTCluster*)lisCLUSTER->Items[n];
		lisEQP->Remove(cluster);
		lisEQP->Add(cluster);
	}
	// limpa lista lisBAR, lisCLUSTER, lisREDE
	lisBAR->Clear();
	lisREDE->Clear();
	lisCLUSTER->Clear();
	// reinicia lisEQN c/ cópias de todos os Eqptos e lisBAR c/ cópias das Barras
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		lisEQN->Add(clone = eqpto->Clone());
		// salva no Eqpto criado o ponteiro p/ Eqpto original
		clone->Obj = eqpto;
		// redefine Status do eqpto
		clone->DefineStatus(sttNOVO, true);
		// verifica se é uma Barra
		if (clone->TipoBarra())
		{ // define Barra como isolada e insere em lisBAR
			clone->DefineStatus(sttISOLADO, true);
			lisBAR->Add(clone);
		}
		else if (clone->Tipo() == eqptoREDE)
		{ // insere cópia da Rede e em lisREDE
			lisREDE->Add(clone);
		}
		else if (clone->Tipo() == eqptoCLUSTER)
		{ // insere cópia do Cluster e em lisCLUSTER
			lisCLUSTER->Add(clone);
		}
	}
	// atualiza todos Eqptos em lisEQN
	for (int n = 0; n < lisEQN->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQN->Items[n];
		if (eqpto->Tipo() == eqptoREDE)
		{ // redefine Redes da Rede
			rede = (VTRede*)eqpto;
			rede->Redes = redes;
			// redefine Tipo da Rede
			if (rede->TipoRede)
				rede->TipoRede = tipos->ExisteTipoRede(rede->TipoRede->Id);
			// redefine Cluster da Rede
			if (rede->Cluster != NULL)
				rede->Cluster = (VTCluster*)ExisteEqpto(rede->Cluster->Id, lisCLUSTER);
			// redefine Barra inicial
			if ((barra = rede->BarraInicial()) != NULL)
			{ // determina Barra em lisBAR
				barra = (VTBarra*)ExisteEqpto(barra->Id, lisBAR);
				rede->DefineBarraInicial(barra);
			}
		}
		else if (eqpto->TipoBarra())
		{ // nada a fazer
			barra = (VTBarra*)eqpto;
		}
		else if (eqpto->TipoEqbar())
		{ // redefine Barra do Eqbar
			eqbar = (VTEqbar*)eqpto;
			eqbar->pbarra = (VTBarra*)ExisteEqpto(eqbar->pbarra->Id, lisBAR);
			switch (eqpto->Tipo())
			{
			case eqptoCAPACITOR: // redefine Curva típica
				capacitor = (VTCapacitor*)eqpto;
				if (capacitor->curva->Tipica)
					capacitor->curva = curvas->ExisteCurvaTipicaCapacitor(capacitor->curva->Id);
				break;
			case eqptoCARGA: // redefine Curva típica
				carga = (VTCarga*)eqpto;
				// determina ponteiro para Classe
				carga->classe = classes->ExisteClasseTag(carga->classe->Tag);
				if (carga->curva->Tipica)
				{ // verifica se Carga de IP
					if (carga->IP)
						carga->curva = curvas->ExisteCurvaTipicaIP(carga->curva->Id);
					else
						carga->curva = curvas->ExisteCurvaTipicaCarga(carga->curva->Id);
				}
				break;
			case eqptoREATOR: // redefine Curva típica
				reator = (VTReator*)eqpto;
				if (reator->curva->Tipica)
					reator->curva = curvas->ExisteCurvaTipicaReator(reator->curva->Id);
				break;
			}
		}
		else if (eqpto->TipoLigacao())
		{
			ligacao = (VTLigacao*)eqpto;
			// redefine Rede da Ligacao
			// ligacao->DefineObjRede(NULL);
			// redefine Barras da Ligacao
			if (ligacao->pbarra1)
				ligacao->pbarra1 = (VTBarra*)ExisteEqpto(ligacao->pbarra1->Id, lisBAR);
			if (ligacao->pbarra2)
				ligacao->pbarra2 = (VTBarra*)ExisteEqpto(ligacao->pbarra2->Id, lisBAR);
			if (ligacao->pbarra3)
				ligacao->pbarra3 = (VTBarra*)ExisteEqpto(ligacao->pbarra3->Id, lisBAR);
			// determina Rede da Ligacao original
			rede = ((VTLigacao*)(ligacao->Obj))->rede;
			// identifica Rede da Ligacao original na lista de Redes sendo coladas
			if (rede != NULL)
			{
				rede = (VTRede*)ExisteEqpto(rede->Id, lisREDE);
			}
			// define Rede para a Ligacao nova: se a Rede não está sendo incluída, associa Ligacao com rede isolada
			ligacao->rede = (rede != NULL) ? rede : redes->RedeIsolada();
			// verifica se Ligacao é uma Chave
			if (ligacao->Tipo() == eqptoCHAVE)
			{
				chave = (VTChave*)ligacao;
				// redefine Tipo da Chave
				if (chave->TipoChave != NULL)
					chave->TipoChave = tipos->ExisteTipoChave(chave->TipoChave->Id);
				// if (chave->TipoChave == NULL) chave->TipoChave = tipos->ExisteTipoChave("", chaveINDEFINIDA);
				if (chave->TipoChave == NULL)
					chave->TipoChave = tipos->ExisteTipoChave("Indefinida", chaveINDEFINIDA);
			}
			// verifica se Ligacao é um Trecho
			if (ligacao->Tipo() == eqptoTRECHO)
			{
				trecho = (VTTrecho*)ligacao;
				// verifica se o Trecho está associado a um Arranjo típico
				if (trecho->arranjo->Owner != trecho)
				{ // Arranjo típico: determina seu equivalente
					arranjo = arranjos->ExisteArranjo(trecho->arranjo->Id);
					if ((arranjo == NULL) ||
						(arranjo->Codigo.AnsiCompareIC(trecho->arranjo->Codigo) != 0))
					{ // cria um novo Arranjo típico
						arranjo = trecho->arranjo->Clone();
						arranjos->InsereArranjo(arranjo);
					}
					// redefine Arranjo do Trecho
					trecho->DefineObjArranjo(arranjo);
				}
			}
			if (ligacao->Tipo() == eqptoREGULADOR)
			{
				regulador = (VTRegulador*)ligacao;
				if (regulador->sentido.bar_ref)
					regulador->sentido.bar_ref =
						(VTBarra*)ExisteEqpto(regulador->sentido.bar_ref->Id, lisBAR);
				if (regulador->cfg_auto.bar_ref)
					regulador->cfg_auto.bar_ref =
						(VTBarra*)ExisteEqpto(regulador->cfg_auto.bar_ref->Id, lisBAR);
				if (regulador->cfg_auto.bar_ref_inv)
					regulador->cfg_auto.bar_ref_inv =
						(VTBarra*)ExisteEqpto(regulador->cfg_auto.bar_ref_inv->Id, lisBAR);
			}
		}
	}
	// redefine IDs dos Eqptos (p/ evitar IDs repetidos)
	for (int n = 0; n < lisEQN->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQN->Items[n];
		// redefine Id do eqpto
		eqpto->Id = FalsoId();
		// verifica Eqptos que tem Curva associada
		if (eqpto->Tipo() == eqptoCAPACITOR)
		{ // verifica se Capacitor possui Curva própria
			curva = ((VTCapacitor*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoCARGA)
		{ // verifica se Carga possui Curva própria
			curva = ((VTCarga*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoGERADOR)
		{ // verifica se Gerador possui Curva própria
			curva = ((VTGerador*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoSUPRIMENTO)
		{ // verifica se Suprimento possui Curva própria
			curva = ((VTSuprimento*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoREATOR)
		{ // verifica se Reator possui Curva própria
			curva = ((VTReator*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EstimaCoordEsquematico(void)
{
	// variágveis locais
	double fat_mult;
	VTBarra *barra;
	VTArea *area_utm = redes->AreaUtm_cm();
	VTArea *area_esq = redes->AreaEsquematico();

	// verifica se área utm á válida
	if ((area_utm->x1 <= 0) || (area_utm->y1 <= 0))
	{ // ainda não há nenhuma Barra definida: nada a fazer
		return;
	}
	// loop p/ todas Barras
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		// calcula coordenada x
		fat_mult = double(barra->utm.x - area_utm->x1) / area_utm->Width();
		barra->esq.x1 = barra->esq.x2 = area_esq->x1 + (fat_mult * area_esq->Width());
		// calcula coordenada y
		fat_mult = double(barra->utm.y - area_utm->y1) / area_utm->Height();
		barra->esq.y1 = barra->esq.y2 = area_esq->y1 + (fat_mult * area_esq->Height());
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EstimaCoordUtm(void)
{
	// variágveis locais
	double fat_mult;
	VTBarra *barra;
	VTArea *area_utm = redes->AreaUtm_cm();
	VTArea *area_esq = redes->AreaEsquematico();

	// verifica se área utm á válida
	if ((area_utm->x1 <= 0) || (area_utm->y1 <= 0))
	{ // ainda não há nenhuma Barra definida: não dá p/ fazer nada
		return;
	}
	// loop p/ todas Barras
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		// calcula coordenada x
		fat_mult = double(barra->esq.x1 - area_esq->x1) / area_esq->Width();
		barra->utm.x = area_utm->x1 + (fat_mult * area_utm->Width());
		// calcula coordenada y
		fat_mult = double(barra->esq.y1 - area_esq->y1) / area_esq->Height();
		barra->utm.y = area_utm->y1 + (fat_mult * area_utm->Height());
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EvtEscape(void)
{
	// cancela seleção de Eqptos
	if (EdtCopia)
		EdtCopia->EvtEscape();
	// operação cancelada: desabilita este editor e executa ação externa (se houver)
	ResetGrafFocus();
	Done();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EvtMouseDown(void)
{
	// variáveis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica se não há eqptos p/ copiar
	if (lisEQN->Count == 0)
	{
		Aviso("Não há equipamentos selecionados para copiar");
		return;
	}
	// transfere Eqptos de lisEQN p/ lisEQP
	lisEQP->Clear();
	CopiaTList(lisEQN, lisEQP);
	lisEQN->Clear();
	// redefine coordenadas da Barra
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // estima coordenadas esquemático
		EstimaCoordEsquematico();
	}
	else
	{ // estima coordenadas utm
		EstimaCoordUtm();
	}
	// insere cópias dos Eqptos na Rede
	edita->InsereLisEqpto(lisEQP);
	// reinicia lisEQP sem destruir objetos porque foram inseridos na rede
	lisEQP->Clear();
	// limpa demais listas
	lisBAR->Clear();
	lisREDE->Clear();
	lisCLUSTER->Clear();
	// atualiza gráfico
	// graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	// graf->Redraw();
	// desabilita este editor
	ResetGrafFocus();
	// equipamentos inseridos: executa ação externa (se houver)
	Done();
	/*
	 //cria novas cópias dos Eqptos p/ nova colagem
	 CopiaEqptos();
	 //redefine posições das barras
	 MoveAtualiza();
	 //atualiza gráfico: apaga Eqpto exibidos na posição anterior
	 graf->Redraw(redrawEQP_NOVO, this);
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EvtMouseMove(void)
{
	// variáveis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	if (first_move)
	{
		first_move = false;
	}
	else
	{ // atualiza gráfico: apaga Eqpto exibidos na posição anterior
		graf->Redraw(redrawEQP_NOVO, this);
	}
	// redefine posições das barras
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // estima coordenadas utm
		MoveAtualizaCoordUtm();
	}
	else
	{ // estima coordenadas esquemático
		MoveAtualizaCoordEsquematico();
	}
	// atualiza gráfico: exibe Eqptos na nova posição
	graf->Redraw(redrawEQP_NOVO, this);
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TEdtCola::ExisteEqpto(int eqpto_id, TList *lisEQP)
{
	// variáveis locais
	VTEqpto *eqpto;

	for (int nb = 0; nb < lisEQP->Count; nb++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[nb];
		if (eqpto->Id == eqpto_id)
			return (eqpto);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtCola::ExisteEqpto(VTEqpto *eqpto, TList *lisEQP)
{
	return (lisEQP->IndexOf(eqpto) >= 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtCola::ExisteRede(int id, AnsiString codigo, TList *lisEQP)
{
	// variáveis locais
	VTEqpto *eqpto;

	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if ((eqpto->Id == id) && (eqpto->Codigo.AnsiCompare(codigo) == 0))
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::IniciaLisEqptoOriginal(void)
{
	// variáveis locais
	VTBarra *pbar;
	VTEqpto *eqpto;
	VTEqbar *eqbar;
	VTLigacao *ligacao;
	TList *lisEXT;

	// reinicia lisEQP c/ eqptos originais a serem copiados
	lisEQP->Clear();
	// determina lista de Eqptos a serem copiados
	if (EdtCopia == NULL)
		return;
	lisEXT = EdtCopia->LisEqptoSel();
	// insere em lisEQP os Eqptos selecionados em EdtCopia
	CopiaTList(lisEXT, lisEQP);
	// inclui em lisEQP as Barras dos Eqbar e das Ligacoes
	for (int n = 0; n < lisEXT->Count; n++)
	{
		eqpto = (VTEqpto*)lisEXT->Items[n];
		if (eqpto->TipoEqbar())
		{
			eqbar = (VTEqbar*)eqpto;
			if (lisEQP->IndexOf(eqbar->pbarra) < 0)
				lisEQP->Add(eqbar->pbarra);
		}
		else if (eqpto->TipoLigacao())
		{
			ligacao = (VTLigacao*)eqpto;
			ligacao->LisBarra(lisEQP);
		}
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TEdtCola::MoveAtualizaCoordEsquematico(void)
 {
 //variáveis locais
 int      dx, dy;
 VTBarra  *barra;
 VTEqpto  *eqpto;
 VTTrecho *trecho;

 //altera coordenadas de cada Barra
 for (int n = 0; n < lisBAR->Count; n++)
 {
 barra = (VTBarra*)lisBAR->Items[n];
 barra->esq.x1 = evt.x + (barra->esq.x1 - coord.x1);
 barra->esq.y1 = evt.y + (barra->esq.y1 - coord.y1);
 barra->esq.x2 = evt.x + (barra->esq.x2 - coord.x1);
 barra->esq.y2 = evt.y + (barra->esq.y2 - coord.y1);
 }
 //redefine coordenadas da área definida pelas Barras sendo copiadas
 dx = coord.x2 - coord.x1;
 dy = coord.y2 - coord.y1;
 coord.x1 = evt.x;
 coord.y1 = evt.y;
 coord.x2 = evt.x + dx;
 coord.y2 = evt.y + dy;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TEdtCola::MoveAtualizaCoordEsquematico(void)
{
	// variáveis locais
	int dx, dy;
	VTBarra *barra;
	VTEqpto *eqpto;
	VTTrecho *trecho;

	// altera coordenadas de cada Barra
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		barra->esq.x1 = evt.x + (barra->esq.x1 - coord.x1);
		barra->esq.y1 = evt.y + (barra->esq.y1 - coord.y1);
		barra->esq.x2 = evt.x + (barra->esq.x2 - coord.x1);
		barra->esq.y2 = evt.y + (barra->esq.y2 - coord.y1);
	}
	// redefine coordenadas da área definida pelas Barras sendo copiadas
	dx = coord.x2 - coord.x1;
	dy = coord.y2 - coord.y1;
	coord.x1 = evt.x;
	coord.y1 = evt.y;
	coord.x2 = evt.x + dx;
	coord.y2 = evt.y + dy;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::MoveAtualizaCoordUtm(void)
{
	// variáveis locais
	int dx, dy;
	VTBarra *barra;

	// altera coordenadas de cada Barra
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		barra->utm.x = evt.x + (barra->utm.x - coord.x1);
		barra->utm.y = evt.y + (barra->utm.y - coord.y1);
	}
	// redefine coordenadas da área definida pelas Barras sendo copiadas
	dx = coord.x2 - coord.x1;
	dy = coord.y2 - coord.y1;
	coord.x1 = evt.x;
	coord.y1 = evt.y;
	coord.x2 = evt.x + dx;
	coord.y2 = evt.y + dy;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::PM_SetEdtCopia(VTEdt *edt)
{
	// verifica se pode haver indicação anterior de Eqpto
	if (!first_move)
	{ // atualiza gráfico: apaga Eqpto exibidos na posição anterior
		graf->Redraw(redrawEQP_NOVO, this);
		first_move = true;
	}
	// salva EdtCopia
	PD.edt_copia = edt;
	// monta lista lisEQP c/ Eqptos a serem copiados
	IniciaLisEqptoOriginal();
	// salva área definida pelas Barras originais sendo copiadas
	SalvaArea();
	// cria cópias dos Eqptos
	CopiaEqptos();
	// redefine coordenadas dos Eqptos
	RedefineCoordenadas();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::RedefineCoordenadas(void)
{
	// variáveis locais
	double width, height, fatx, faty, fator_b;
	int x1, y1, x2, y2;
	VTBarra *barra;
	VTEqbar *eqbar;
	VTEqpto *eqpto;
	VTArea *area = redes->AreaEsquematico();

	// proteção
	if (lisBAR->Count == 0)
		return;
	// if ((redes->LisBarra())->Count == 0) return;
	// verifica se a área da Rede compreende a área das barras
	if (area->ContemPonto(coord.x1, coord.y1) && area->ContemPonto(coord.x2, coord.y2))
		return;
	// determina a relação entre Barras da rede e barras sendo copiadas
	if ((fator_b = ((redes->LisBarra())->Count) / lisBAR->Count) < 1.0)
		fator_b = 1.;
	// redefine coordenadas das barras de acordo c/ a área total da rede
	width = coord.x2 - coord.x1 + 1;
	height = coord.y2 - coord.y1 + 1;
	// fatx = (area->Width()) / (1.2 * width);
	// faty = (area->Height())/ (1.2 * height);
	// fatx = (area->Width()) /(4 * width);
	// faty = (area->Height())/(4 * height);
	fatx = (area->Width()) / (fator_b * width);
	faty = (area->Height()) / (fator_b * height);
	// proteção
	// if (IsDoubleZero(fatx)) fatx = 1.0;
	// if (IsDoubleZero(faty)) faty = 1.0;
	// assume a menor escala para x e y
	if (fatx < faty)
		faty = fatx;
	if (faty < fatx)
		fatx = faty;
	// limita fat-x
	if (fatx < 0.1)
		fatx = 0.1;
	else if (fatx > 1.0)
		fatx = 1.0;
	// redefine coordenadas de todas as Barras e dos Eqbar
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		barra->esq.x1 *= fatx;
		barra->esq.y1 *= fatx;
		barra->esq.x2 *= fatx;
		barra->esq.y2 *= fatx;
	}
	// redefine posicionamento de todos Eqbar
	for (int n = 0; n < lisEQN->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQN->Items[n];
		if (eqpto->TipoEqbar())
		{
			eqbar = (VTEqbar*)eqpto;
			eqbar->esq.dxbar *= fatx;
			eqbar->esq.dybar *= fatx;
		}
	}
	// redefine área definida pelas barras
	coord.x1 *= fatx;
	coord.y1 *= fatx;
	coord.x2 *= fatx;
	coord.y2 *= fatx;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::SalvaArea(void)
{
	// variáveis locais
	VTEqpto *eqpto;
	VTEqbar *eqbar;
	VTLigacao *ligacao;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// reinicia lisBAR com as Barras originais
	lisBAR->Clear();
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->TipoBarra())
		{
			if (lisBAR->IndexOf(eqpto) < 0)
				lisBAR->Add(eqpto);
		}
		else if (eqpto->TipoEqbar())
		{
			eqbar = (VTEqbar*)eqpto;
			if (lisBAR->IndexOf(eqbar->pbarra) < 0)
				lisBAR->Add(eqbar->pbarra);
		}
		else if (eqpto->TipoLigacao())
		{
			ligacao = (VTLigacao*)eqpto;
			ligacao->LisBarra(lisBAR);
		}
	}
	// verifica o modo gráfico selecionado
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // determina área definida pelas coordenadas utm das Barras sendo copiadas
		SalvaAreaCoordUtm();
	}
	else
	{ // determina área definida pelas coordenadas esquemáticas das Barras sendo copiadas
		SalvaAreaCoordEsquematico();
	}
	// reinicia lisBAR
	lisBAR->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::SalvaAreaCoordEsquematico(void)
{
	// variáveis locais
	int x1, y1, x2, y2;
	VTBarra *barra;

	// proteção
	if (lisBAR->Count == 0)
		return;
	// determina área defiida pelas Barras sendo copiadas
	barra = (VTBarra*)lisBAR->First();
	barra->AreaEsquematico(coord.x1, coord.y1, coord.x2, coord.y2);
	for (int nb = 1; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		barra->AreaEsquematico(x1, y1, x2, y2);
		if (x1 < coord.x1)
			coord.x1 = x1;
		if (y1 < coord.y1)
			coord.y1 = y1;
		if (x2 > coord.x2)
			coord.x2 = x2;
		if (y2 > coord.y2)
			coord.y2 = y2;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::SalvaAreaCoordUtm(void)
{
	// variáveis locais
	VTBarra *barra;

	// proteção
	if (lisBAR->Count == 0)
		return;
	// determina área defiida pelas Barras sendo copiadas
	barra = (VTBarra*)lisBAR->First();
	coord.x1 = coord.x2 = barra->utm.x;
	coord.y1 = coord.y2 = barra->utm.y;
	for (int nb = 1; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if (barra->utm.x < coord.x1)
			coord.x1 = barra->utm.x;
		else if (barra->utm.x > coord.x2)
			coord.x2 = barra->utm.x;
		if (barra->utm.y < coord.y1)
			coord.y1 = barra->utm.y;
		else if (barra->utm.y > coord.y2)
			coord.y2 = barra->utm.y;
	}
}

// ---------------------------------------------------------------------------
// eof
