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
// fun��o global p/ criar objetos da classe TEdtCola
// ---------------------------------------------------------------------------
VTEdt* NewObjEdtCola(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtCola(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtCola::TEdtCola(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	: TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
{
	// vari�veis locais
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
	// mais nada a fazer: o contrutor de VTEdt faz todas inicia��es
}

// ---------------------------------------------------------------------------
__fastcall TEdtCola::~TEdtCola(void)
{
	// vari�veis locais
	VTDragdrop *dragdrop = (VTDragdrop*)apl->GetObject(__classid(VTDragdrop));

	// retira este objeto no objeto Dragdrop
	if (dragdrop)
		dragdrop->RetEdtCola(this);
	// destr�i lista sem destruir seus objetos
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
	// atualiza gr�fico
	if (!first_move)
		graf->Redraw(redrawEQP_NOVO, this);
	// destr�i objetos em lisEQN
	if (lisEQN)
		LimpaTList(lisEQN);
	// mais nada a fazer: o destrutor de VTEdt faz todas finaliza��es
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::CopiaEqptos(void)
{
	// vari�veis locais
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
	// destr�i objetos em lisEQN
	LimpaTList(lisEQN);
	// N�O permite c�pia de Reducao p/ mesma Redes
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		// verifica se � uma Reducao
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
	// se necess�rio, inclui MRede das Reducoes em lisEQP
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		// verifica se � uma Reducao
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
	// determina conjuntos de Rede e Cluster que devem inclu�dos em lisEQP p/ c�pia
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->Tipo() == eqptoREDE)
		{ // insere Rede em lisREDE
			lisREDE->Add(eqpto);
		}
		// verifica se � uma Ligacao
		else if (eqpto->TipoLigacao())
		{ // determina Rede da Ligacao (que pode n�o estar definida ou pode ser a Rede isolada)
			if ((rede = ((VTLigacao*)eqpto)->rede) == NULL)
				continue;
			if (rede == rede->Redes->RedeIsolada())
				continue;
			// verifica se a Rede existe em Redes (c�pia sendo feita na mesma janela)
			if (ExisteEqpto(rede, redes->LisRede()))
				continue;
			// verifica se a Rede existe em Redes (c�pia sendo feita entre duas janelas)
			if (ExisteRede(rede->Id, rede->Codigo, redes->LisRede()))
				continue;
			// verifica se a Rede j� existe em lisREDE
			if (lisREDE->IndexOf(rede) < 0)
			{ // insere Rede em lisREDE
				lisREDE->Add(rede);
				// verifica se a Rede est� associada a um Cluster
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
	// reinicia lisEQN c/ c�pias de todos os Eqptos e lisBAR c/ c�pias das Barras
	for (int n = lisEQP->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		lisEQN->Add(clone = eqpto->Clone());
		// salva no Eqpto criado o ponteiro p/ Eqpto original
		clone->Obj = eqpto;
		// redefine Status do eqpto
		clone->DefineStatus(sttNOVO, true);
		// verifica se � uma Barra
		if (clone->TipoBarra())
		{ // define Barra como isolada e insere em lisBAR
			clone->DefineStatus(sttISOLADO, true);
			lisBAR->Add(clone);
		}
		else if (clone->Tipo() == eqptoREDE)
		{ // insere c�pia da Rede e em lisREDE
			lisREDE->Add(clone);
		}
		else if (clone->Tipo() == eqptoCLUSTER)
		{ // insere c�pia do Cluster e em lisCLUSTER
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
			case eqptoCAPACITOR: // redefine Curva t�pica
				capacitor = (VTCapacitor*)eqpto;
				if (capacitor->curva->Tipica)
					capacitor->curva = curvas->ExisteCurvaTipicaCapacitor(capacitor->curva->Id);
				break;
			case eqptoCARGA: // redefine Curva t�pica
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
			case eqptoREATOR: // redefine Curva t�pica
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
			// define Rede para a Ligacao nova: se a Rede n�o est� sendo inclu�da, associa Ligacao com rede isolada
			ligacao->rede = (rede != NULL) ? rede : redes->RedeIsolada();
			// verifica se Ligacao � uma Chave
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
			// verifica se Ligacao � um Trecho
			if (ligacao->Tipo() == eqptoTRECHO)
			{
				trecho = (VTTrecho*)ligacao;
				// verifica se o Trecho est� associado a um Arranjo t�pico
				if (trecho->arranjo->Owner != trecho)
				{ // Arranjo t�pico: determina seu equivalente
					arranjo = arranjos->ExisteArranjo(trecho->arranjo->Id);
					if ((arranjo == NULL) ||
						(arranjo->Codigo.AnsiCompareIC(trecho->arranjo->Codigo) != 0))
					{ // cria um novo Arranjo t�pico
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
		{ // verifica se Capacitor possui Curva pr�pria
			curva = ((VTCapacitor*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoCARGA)
		{ // verifica se Carga possui Curva pr�pria
			curva = ((VTCarga*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoGERADOR)
		{ // verifica se Gerador possui Curva pr�pria
			curva = ((VTGerador*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoSUPRIMENTO)
		{ // verifica se Suprimento possui Curva pr�pria
			curva = ((VTSuprimento*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
		else if (eqpto->Tipo() == eqptoREATOR)
		{ // verifica se Reator possui Curva pr�pria
			curva = ((VTReator*)eqpto)->curva;
			if (!curva->Tipica)
				curva->Id = FalsoId();
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EstimaCoordEsquematico(void)
{
	// vari�gveis locais
	double fat_mult;
	VTBarra *barra;
	VTArea *area_utm = redes->AreaUtm_cm();
	VTArea *area_esq = redes->AreaEsquematico();

	// verifica se �rea utm � v�lida
	if ((area_utm->x1 <= 0) || (area_utm->y1 <= 0))
	{ // ainda n�o h� nenhuma Barra definida: nada a fazer
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
	// vari�gveis locais
	double fat_mult;
	VTBarra *barra;
	VTArea *area_utm = redes->AreaUtm_cm();
	VTArea *area_esq = redes->AreaEsquematico();

	// verifica se �rea utm � v�lida
	if ((area_utm->x1 <= 0) || (area_utm->y1 <= 0))
	{ // ainda n�o h� nenhuma Barra definida: n�o d� p/ fazer nada
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
	// cancela sele��o de Eqptos
	if (EdtCopia)
		EdtCopia->EvtEscape();
	// opera��o cancelada: desabilita este editor e executa a��o externa (se houver)
	ResetGrafFocus();
	Done();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EvtMouseDown(void)
{
	// vari�veis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica se n�o h� eqptos p/ copiar
	if (lisEQN->Count == 0)
	{
		Aviso("N�o h� equipamentos selecionados para copiar");
		return;
	}
	// transfere Eqptos de lisEQN p/ lisEQP
	lisEQP->Clear();
	CopiaTList(lisEQN, lisEQP);
	lisEQN->Clear();
	// redefine coordenadas da Barra
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // estima coordenadas esquem�tico
		EstimaCoordEsquematico();
	}
	else
	{ // estima coordenadas utm
		EstimaCoordUtm();
	}
	// insere c�pias dos Eqptos na Rede
	edita->InsereLisEqpto(lisEQP);
	// reinicia lisEQP sem destruir objetos porque foram inseridos na rede
	lisEQP->Clear();
	// limpa demais listas
	lisBAR->Clear();
	lisREDE->Clear();
	lisCLUSTER->Clear();
	// atualiza gr�fico
	// graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	// graf->Redraw();
	// desabilita este editor
	ResetGrafFocus();
	// equipamentos inseridos: executa a��o externa (se houver)
	Done();
	/*
	 //cria novas c�pias dos Eqptos p/ nova colagem
	 CopiaEqptos();
	 //redefine posi��es das barras
	 MoveAtualiza();
	 //atualiza gr�fico: apaga Eqpto exibidos na posi��o anterior
	 graf->Redraw(redrawEQP_NOVO, this);
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::EvtMouseMove(void)
{
	// vari�veis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	if (first_move)
	{
		first_move = false;
	}
	else
	{ // atualiza gr�fico: apaga Eqpto exibidos na posi��o anterior
		graf->Redraw(redrawEQP_NOVO, this);
	}
	// redefine posi��es das barras
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // estima coordenadas utm
		MoveAtualizaCoordUtm();
	}
	else
	{ // estima coordenadas esquem�tico
		MoveAtualizaCoordEsquematico();
	}
	// atualiza gr�fico: exibe Eqptos na nova posi��o
	graf->Redraw(redrawEQP_NOVO, this);
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TEdtCola::ExisteEqpto(int eqpto_id, TList *lisEQP)
{
	// vari�veis locais
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
	// vari�veis locais
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
	// vari�veis locais
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
 //vari�veis locais
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
 //redefine coordenadas da �rea definida pelas Barras sendo copiadas
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
	// vari�veis locais
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
	// redefine coordenadas da �rea definida pelas Barras sendo copiadas
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
	// vari�veis locais
	int dx, dy;
	VTBarra *barra;

	// altera coordenadas de cada Barra
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		barra->utm.x = evt.x + (barra->utm.x - coord.x1);
		barra->utm.y = evt.y + (barra->utm.y - coord.y1);
	}
	// redefine coordenadas da �rea definida pelas Barras sendo copiadas
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
	// verifica se pode haver indica��o anterior de Eqpto
	if (!first_move)
	{ // atualiza gr�fico: apaga Eqpto exibidos na posi��o anterior
		graf->Redraw(redrawEQP_NOVO, this);
		first_move = true;
	}
	// salva EdtCopia
	PD.edt_copia = edt;
	// monta lista lisEQP c/ Eqptos a serem copiados
	IniciaLisEqptoOriginal();
	// salva �rea definida pelas Barras originais sendo copiadas
	SalvaArea();
	// cria c�pias dos Eqptos
	CopiaEqptos();
	// redefine coordenadas dos Eqptos
	RedefineCoordenadas();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::RedefineCoordenadas(void)
{
	// vari�veis locais
	double width, height, fatx, faty, fator_b;
	int x1, y1, x2, y2;
	VTBarra *barra;
	VTEqbar *eqbar;
	VTEqpto *eqpto;
	VTArea *area = redes->AreaEsquematico();

	// prote��o
	if (lisBAR->Count == 0)
		return;
	// if ((redes->LisBarra())->Count == 0) return;
	// verifica se a �rea da Rede compreende a �rea das barras
	if (area->ContemPonto(coord.x1, coord.y1) && area->ContemPonto(coord.x2, coord.y2))
		return;
	// determina a rela��o entre Barras da rede e barras sendo copiadas
	if ((fator_b = ((redes->LisBarra())->Count) / lisBAR->Count) < 1.0)
		fator_b = 1.;
	// redefine coordenadas das barras de acordo c/ a �rea total da rede
	width = coord.x2 - coord.x1 + 1;
	height = coord.y2 - coord.y1 + 1;
	// fatx = (area->Width()) / (1.2 * width);
	// faty = (area->Height())/ (1.2 * height);
	// fatx = (area->Width()) /(4 * width);
	// faty = (area->Height())/(4 * height);
	fatx = (area->Width()) / (fator_b * width);
	faty = (area->Height()) / (fator_b * height);
	// prote��o
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
	// redefine �rea definida pelas barras
	coord.x1 *= fatx;
	coord.y1 *= fatx;
	coord.x2 *= fatx;
	coord.y2 *= fatx;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::SalvaArea(void)
{
	// vari�veis locais
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
	// verifica o modo gr�fico selecionado
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // determina �rea definida pelas coordenadas utm das Barras sendo copiadas
		SalvaAreaCoordUtm();
	}
	else
	{ // determina �rea definida pelas coordenadas esquem�ticas das Barras sendo copiadas
		SalvaAreaCoordEsquematico();
	}
	// reinicia lisBAR
	lisBAR->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtCola::SalvaAreaCoordEsquematico(void)
{
	// vari�veis locais
	int x1, y1, x2, y2;
	VTBarra *barra;

	// prote��o
	if (lisBAR->Count == 0)
		return;
	// determina �rea defiida pelas Barras sendo copiadas
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
	// vari�veis locais
	VTBarra *barra;

	// prote��o
	if (lisBAR->Count == 0)
		return;
	// determina �rea defiida pelas Barras sendo copiadas
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
