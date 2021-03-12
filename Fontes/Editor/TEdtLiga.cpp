// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TEdtLiga.h"
#include "..\Utils\TVerificaMalhaEdicao.h"
#include "..\Apl\VTApl.h"
#include "..\Alerta\VTAlerta.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTLigacao.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TEdtLiga::TEdtLiga(VTApl *apl, TCursor cursor, TAction *ResetFocus, TAction *SetFocus,
	TAction *OnDone) : TEdt(apl, cursor, ResetFocus, SetFocus, OnDone)
{
	// define estado inicial do editor
	estado = SelecaoBarraInicial;
	// obtém objeto VTRedegraf
	redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
}

// ---------------------------------------------------------------------------
__fastcall TEdtLiga::~TEdtLiga(void)
{
	// destrói objetos
	if (bartemp)
	{
		delete bartemp;
		bartemp = NULL;
	}
	// mais nada a fazer: o destrutor de VTEdt faz todas finalizações
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtLiga::BarraInicial(VTBarra *barra)
{
	// variáveis locais
	TList *lisLIG;
	VTLigacao *ligacao;

	//proteção
	if(barra == NULL) return false;
	lisLIG = barra->LisLigacao();
	for (int n = 0; n < lisLIG->Count; n++)
	{ // verifica se a outra barra era barra inicial de uma rede
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->rede == NULL)
			continue;
		if (ligacao->rede->BarraInicial() == barra)
		{
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TEdtLiga::BuscaBarra(void)
{
	// variáveis locais
	VTBarra *pbar;
	TList *lisBAR;

	// define a area como o ponto do evento para fazer a busca
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	// faz a busca de barra
	lisBAR = busca->Barra(area);
	// verifica se encontrou uma Barra
	pbar = NULL;
	if (lisBAR->Count > 0)
	{ // seleciona primeira Barra
		pbar = (VTBarra*)lisBAR->First();
	}
	return (pbar);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422287
VTLigacao* __fastcall TEdtLiga::BuscaLigacao(void)
{
	// variáveis locais
	VTLigacao *pligacao;
	TList *lisLIG;

	// define a area como o ponto do evento para fazer a busca
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	// faz a busca de ligacao
	lisLIG = busca->Ligacao(area);
	// verifica se encontrou uma Ligacao
	pligacao = NULL;
	if (lisLIG->Count > 0)
	{ // seleciona primeira Ligacao
		pligacao = (VTLigacao*)lisLIG->First();
	}
	return (pligacao);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422287
VTBarra* __fastcall TEdtLiga::ClonaBarra1(void)
{
	// proteção
	if (pbarra1 == NULL)
		return NULL;
	try
	{ // clona barra 1 na barra 2
		pbarra2 = pbarra1->Clone();
		pbarra2->Id = FalsoId();
		pbarra2->Codigo = pbarra1->Codigo + "_1";
		pbarra2->Status[sttNOVO] = true;
		// considera coordenada do evento
		pbarra2->esq.x1 = evt.x;
		pbarra2->esq.y1 = evt.y;
		pbarra2->esq.x2 = pbarra2->esq.x1;
		pbarra2->esq.y2 = pbarra2->esq.y1;
		// insere barra nova quando inserir a ligação
	}
	catch (Exception &e)
	{
		pbarra2 = NULL;
		return NULL;
	}
	return pbarra2;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtLiga::EvtMouseDown(void)
{
	// proteção
	if (estado != SelecaoBarraInicial)
		return;
	// verifica se a barra inicial foi selecionada
	if (pbarra1 != NULL)
	{ // atualiza o grafico: restaura Barra inicial selecionada
		graf->Redraw(redrawEQP_NOVO, this);
		// cria objeto TLigacao e insere na lista de objetos novos
		CriaLigacao();
		pliga->Id = FalsoId();
		lisEQN->Add(pliga);
		// cria uma barra temporária para associar à Ligacao
		bartemp = DLL_NewObjBarra(patamares->NumPatamar());
		bartemp->esq.x1 = bartemp->esq.x2 = evt.x;
		bartemp->esq.y1 = bartemp->esq.y2 = evt.y;
		// associa as barras ao trecho
		pliga->pbarra1 = pbarra1;
		pliga->pbarra2 = bartemp;
		// define posição da Ligação na Barra1
		if (redegraf->ModoGrafico == modoESQUEMATICO)
		{
			pliga->DefinePosBar(pbarra1, evt.x, evt.y);
		}
		else
		{
			pliga->esq.posbar1 = 50;
		}
		// reapresenta o grafico
		graf->Redraw(redrawEQP_NOVO, this);
		estado = SelecaoBarraFinal;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtLiga::EvtMouseDownMove(void)
{
	// variáveis locais
	int x1, y1, dx, dy;

	// proteção
	if (estado != SelecaoBarraFinal)
		return;
	// reapresenta o grafico (apagar objetos)
	graf->Redraw(redrawEQP_NOVO, this);
	// muda coordenadas da barra temporária
	bartemp->esq.x1 = bartemp->esq.x2 = evt.x;
	bartemp->esq.y1 = bartemp->esq.y2 = evt.y;
	// verifica se a tecla Shift está apertada
	if (evt.Shift.Contains(ssShift))
	{ // determina coordenadas da Ligação na barra1
		pliga->pbarra1->PontoConexao(pliga->esq.posbar1, x1, y1);
		// ligação vertical ou horizontal
		dx = abs(x1 - evt.x);
		dy = abs(y1 - evt.y);
		if (dx > dy)
		{
			bartemp->esq.y1 = bartemp->esq.y2 = y1;
		}
		else
		{
			bartemp->esq.x1 = bartemp->esq.x2 = x1;
		}
	}
	// atualiza grafico
	graf->Redraw(redrawEQP_NOVO, this);
	// procura barra final na coordenada
	if ((pbarra2 = BuscaBarra()) != NULL)
	{
		if (lisEQN->IndexOf(pbarra2) >= 0)
			return;
		// atualiza gráfico: apaga Barras selecionadas
		graf->Redraw(redrawEQP_NOVO, this);
		// reinicia lista de barras selecionadas
		lisEQN->Clear();
		lisEQN->Add(pbarra1);
		lisEQN->Add(pbarra2);
		lisEQN->Add(pliga);
		// atualiza gráfico: exibe Barras selecionadas
		graf->Redraw(redrawEQP_NOVO, this);
		return;
	}
	// atualiza gráfico: apaga Barras selecionadas
	graf->Redraw(redrawEQP_NOVO, this);
	// reinicia lista de Barras selecionadas
	lisEQN->Clear();
	lisEQN->Add(pbarra1);
	lisEQN->Add(pliga);
	graf->Redraw(redrawEQP_NOVO, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtLiga::EvtMouseMove(void)
{
	// variáveis locais
	VTBarra *pbar_nova;

	// proteção
	if (estado != SelecaoBarraInicial)
		return;
	// verifica se tem barra na area
	if ((pbar_nova = BuscaBarra()) == NULL)
	{ // se a lista de eqpto novos estiver vazia, nada a fazer
		if (lisEQN->Count == 0)
			return;
		// atualiza o grafico e limpa lisEQN
		graf->Redraw(redrawEQP_NOVO, this);
		lisEQN->Clear();
		// reinicia barra inicial
		pbarra1 = NULL;
		return;
	}
	// verifica se a barra selecionada é a própria barra inicial
	if (pbar_nova == pbarra1)
		return;
	// redefine barra inicial
	pbarra1 = pbar_nova;
	// caso tenha algum item na lista lisEQN exibe-o na sua cor original
	if (lisEQN->Count)
		graf->Redraw(redrawEQP_NOVO, this);
	// limpa lista de eqptos selecionados
	lisEQN->Clear();
	// insere a barra na lista de eqptos selecionados
	lisEQN->Add(pbarra1);
	// atualiza grafico, lista de selecionados com cor especial
	graf->Redraw(redrawEQP_NOVO, this);
}

////---------------------------------------------------------------------------
// void __fastcall TEdtLiga::EvtMouseUp(void)
// {
// //variáveis locais
// VTBarra *barrasel;
//
// //proteção
// if (estado != SelecaoBarraFinal)
// return;
// //verifica se selecionou Barra final
// if ((barrasel = BuscaBarra()) == NULL)
// {
// RetiraLigacao();
// return;
// }
// //verifica se Barra selecionada é válida
// if (barrasel == pbarra1)
// {
// RetiraLigacao();
// return;
// }
// //reapresenta o grafico (apagar objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// //associa Ligação c/ Barra selecionada
// pliga->pbarra2  = barrasel;
// //define posição na barra
// if (redegraf->ModoGrafico == modoESQUEMATICO)
// {pliga->DefinePosBar(pbarra2, bartemp->esq.x2, bartemp->esq.y2);}
// else
// {pliga->esq.posbar2 = 50;}
// //ordena Barras: barra1 c/ maior tensão
// OrdenaBarras();
// // confere se as tensões das barras estão coerentes com o tipo
// // de ligacao selecionada
// switch (pliga->Tipo())
// {
// case eqptoCAPSERIE:
// if (VnomDiferente(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
// {//redesenha gráfico (exibir objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// Erro("As barras do capacitor série devem ter a mesma tensão nominal!");
// //retira a ligacao criada
// RetiraLigacao();
// return;
// }
// break;
// case eqptoCHAVE:
// if (VnomDiferente(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
// {//redesenha gráfico (exibir objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// Erro("As barras da chave devem ter a mesma tensão nominal!");
// //retira a ligacao criada
// RetiraLigacao();
// return;
// }
// break;
// case eqptoTRECHO :
// if (VnomDiferente(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
// {//redesenha gráfico (exibir objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// Erro ("As barras do trecho devem ter a mesma tensão nominal!");
// //retira a ligacao criada
// RetiraLigacao();
// return;
// }
// break;
// /*
// case eqptoTRAFO:
// if (VnomIgual(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
// {//redesenha gráfico (exibir objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// Erro ("As barras entre trafo devem ter tensões nominais diferentes!");
// //retira a ligacao criada
// RetiraLigacao();
// return;
// }
// break;
// case eqptoTRAFO3E:
// if (VnomIgual(pliga->pbarra1->vnom, pliga->pbarra2->vnom)&&
// VnomIgual(pliga->pbarra1->vnom, pliga->pbarra3->vnom))
// {//redesenha gráfico (exibir objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// Erro ("As barras entre trafo devem ter tensões nominais diferentes!");
// //retira a ligacao criada
// RetiraLigacao();
// return;
// }
// break;
// */
// }
// //reapresenta o grafico (exibir objetos)
// graf->Redraw(redrawEQP_NOVO, this);
// //pede p/ usuário editar dados da ligação e confirmar inserção
// if (ConfirmaLigacao())
// {//usuário confirmou inserção
// InsereLigacao();
// //sinaliza término de operação
// Done();
// }
// else
// {//usuário cancelou inserção
// RetiraLigacao();
// }
// }

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422287
// https://sinapsisenergia.teamwork.com/#tasks/14422540
void __fastcall TEdtLiga::EvtMouseUp(void)
{
	// variáveis locais
	VTBarra *barrasel;
	bool barra_clone = false;
	TVerificaMalhaEdicao *utilMalha = new TVerificaMalhaEdicao(apl);
	VTRede *redeAlterada = NULL;
	bool existeMalhaPrevia = false;
	bool existeMalhaAtual = false;
	VTLigacao *ligTemp;

	// proteção
	if (estado != SelecaoBarraFinal)
		return;
	// verifica se selecionou Barra final ou se é a barra 1
	barrasel = BuscaBarra();
	if ((barrasel == NULL) || (barrasel == pbarra1))
	{ // temporariamente bloqueia clone de barra que seja barra inicial de rede
		if (BarraInicial(barrasel))
		{
			Aviso("Não é possível inserir a ligação a partir de uma barra inicial sem uma segunda barra"
				);
			RetiraLigacao();
			return;
		}
		// clona barra 1 para servir como barra 2
		if ((barrasel = ClonaBarra1()) == NULL)
		{
			RetiraLigacao();
			return;
		}
		// altera flag
		barra_clone = true;
	}

	// reapresenta o grafico (apagar objetos)
	graf->Redraw(redrawEQP_NOVO, this);
	// associa Ligação c/ Barra selecionada
	pliga->pbarra2 = barrasel;
	// define posição na barra
	if (redegraf->ModoGrafico == modoESQUEMATICO)
	{
		pliga->DefinePosBar(pbarra2, bartemp->esq.x2, bartemp->esq.y2);
	}
	else
	{
		pliga->esq.posbar2 = 50;
	}
	// ordena Barras: barra1 c/ maior tensão
	OrdenaBarras();
	// confere se as tensões das barras estão coerentes com o tipo
	// de ligacao selecionada
	switch (pliga->Tipo())
	{
	case eqptoCAPSERIE:
		if (VnomDiferente(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
		{ // redesenha gráfico (exibir objetos)
			graf->Redraw(redrawEQP_NOVO, this);
			Erro("As barras do capacitor série devem ter a mesma tensão nominal!");
			// retira a ligacao criada
			RetiraLigacao();
			return;
		}
		break;
	case eqptoCHAVE:
		if (VnomDiferente(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
		{ // redesenha gráfico (exibir objetos)
			graf->Redraw(redrawEQP_NOVO, this);
			Erro("As barras da chave devem ter a mesma tensão nominal!");
			// retira a ligacao criada
			RetiraLigacao();
			return;
		}
		break;
	case eqptoTRECHO:
		if (VnomDiferente(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
		{ // redesenha gráfico (exibir objetos)
			graf->Redraw(redrawEQP_NOVO, this);
			Erro("As barras do trecho devem ter a mesma tensão nominal!");
			// retira a ligacao criada
			RetiraLigacao();
			return;
		}
		break;
		/*
		 case eqptoTRAFO:
		 if (VnomIgual(pliga->pbarra1->vnom, pliga->pbarra2->vnom))
		 {//redesenha gráfico (exibir objetos)
		 graf->Redraw(redrawEQP_NOVO, this);
		 Erro ("As barras entre trafo devem ter tensões nominais diferentes!");
		 //retira a ligacao criada
		 RetiraLigacao();
		 return;
		 }
		 break;
		 case eqptoTRAFO3E:
		 if (VnomIgual(pliga->pbarra1->vnom, pliga->pbarra2->vnom)&&
		 VnomIgual(pliga->pbarra1->vnom, pliga->pbarra3->vnom))
		 {//redesenha gráfico (exibir objetos)
		 graf->Redraw(redrawEQP_NOVO, this);
		 Erro ("As barras entre trafo devem ter tensões nominais diferentes!");
		 //retira a ligacao criada
		 RetiraLigacao();
		 return;
		 }
		 break;
		 */
	}
	// reapresenta o grafico (exibir objetos)
	graf->Redraw(redrawEQP_NOVO, this);
	// pede p/ usuário editar dados da ligação e confirmar inserção
	if (ConfirmaLigacao())
	{ // guarda ponteiro da ligacao
		ligTemp = pliga;
		// verifica se ja existia malha previa
		if (pliga != NULL)
		{
			redeAlterada = pliga->rede;
			existeMalhaPrevia = utilMalha->ExisteAlertaMalha(redeAlterada);
		}
		// usuário confirmou inserção
		InsereLigacao(barra_clone);
		// sinaliza término de operação
		Done();
		// verifica se fechou malha interna
		existeMalhaAtual = utilMalha->ExisteAlertaMalha(redeAlterada);
		// verifica se possui malha interna OU malha externa
		if ((existeMalhaAtual && !existeMalhaPrevia) ||
			(utilMalha->VerificaSeFechaMalhaExterna(ligTemp)))
		{
			// caso seja malha interna
			if (existeMalhaAtual)
			{
				if (Confirma("Esta operação fechou malha interna.",
					"Deseja confirmar a inserção da ligação?") != IDYES)
				{
					RemoveLigacaoRecemInserida(ligTemp);
				}
			}
			else
			{
				if (Confirma("Esta operação fechou malha externa.",
					"Deseja confirmar a inserção da ligação?") != IDYES)
				{
					RemoveLigacaoRecemInserida(ligTemp);
				}
			}
		}
	}
	else
	{ // usuário cancelou inserção
		RetiraLigacao();
	}
	// destro utilitario
	if (utilMalha)
	{
		delete utilMalha;
		utilMalha = NULL;
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TEdtLiga::InsereLigacao(bool barra_clone)
{
	//variaveis locais
	VTLigacao *ligasel;

	// destrói objetos
	if (bartemp)
	{
		delete bartemp;
		bartemp = NULL;
	}
	if (barra_clone)
	{ ligasel = BuscaLigacao();
		// insere a Ligacao e a barra clone
		lisEQN->Clear();
		lisEQN->Add(pbarra2);
		lisEQN->Add(pliga);
		edita->InsereLisEqpto(lisEQN);
		// verifica se foi selecionada outra ligação também
		if (ligasel != NULL)
		{ // move ligação selecionada para barra clonada
			MoveLigacao(ligasel, pbarra2);
		}
	}
	else
	{ // insere a Ligacao
		edita->InsereLigacao(pliga, pliga->rede);
	}
	// reinicia objetos SEM destruí-los
	pliga = NULL;
	pbarra1 = NULL;
	// limpa listas
	lisEQS->Clear();
	lisEQN->Clear();
	// atualiza gráfico
	graf->Redraw();
	// reinicia estado
	estado = SelecaoBarraInicial;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422287
void __fastcall TEdtLiga::MoveLigacao(VTLigacao *ligacao, VTBarra *barra_destino)
{
	// variáveis locais
	VTLigacao *clone;
	VTBarra *barra;
	int index_bar = 0;

	// proteção
	if (ligacao == NULL)
		return;
	// clona ligacao
	clone = ligacao->Clone();
	ligacao->Obj = clone;
	// se a ligacao estiver ligada à pbarra1, determina indice desta barra da ligacao
	index_bar = ligacao->IndiceBarra(pbarra1);
	if (index_bar < 0)
	{ // verifica se está na pbarra2
		index_bar = ligacao->IndiceBarra(pbarra2);
		if (index_bar < 0)
			return;
	}
	// determina outra barra
	barra = clone->Barra(1 - index_bar);
	// conecta ligação na barra e na barra_destino
	clone->DefineObjBarra(barra, barra_destino);
	// altera ligacao
	edita->Altera(ligacao, clone);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtLiga::OrdenaBarras(void)
{
	// variáveis locais
	VTBarra *pbar;
	int posbar;

	if (pliga->pbarra1->vnom < pliga->pbarra2->vnom)
	{ // salva informações da Barra1
		pbar = pliga->pbarra1;
		posbar = pliga->esq.posbar1;
		// redefine Barra1 c/ dados da Barra2
		pliga->pbarra1 = pliga->pbarra2;
		pliga->esq.posbar1 = pliga->esq.posbar2;
		// redefine Barra2 c/ dados da Barra1
		pliga->pbarra2 = pbar;
		pliga->esq.posbar2 = posbar;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtLiga::RemoveLigacaoRecemInserida(VTLigacao *lig)
{
	//variaveis locais
	VTLigacao *ligasel;

	// Remove a Ligacao
	// edita->Retira(lig);
	edita->ActionUndo->Execute();
	// verifica se foi selecionada outra ligação também
	if ((ligasel = BuscaLigacao()) != NULL)
	{//precisa de 2 undo para voltar a ligação selecionada para barra original e tirar os eqtos inseridos
		edita->ActionUndo->Execute();
    }
	// reinicia objetos SEM destruí-los
	pliga = NULL;
	pbarra1 = NULL;
	// limpa listas
	lisEQN->Clear();
	lisEQN->Clear();
	// atualiza gráfico
	// graf->Redraw();
	// reinicia estado
	estado = SelecaoBarraInicial;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtLiga::RetiraLigacao(void)
{
	// atualiza gráfico: apagar Eqptos novos
	graf->Redraw(redrawEQP_NOVO, this);
	// destrói objetos
	if (bartemp)
	{
		delete bartemp;
		bartemp = NULL;
	}
	if (pliga)
	{
		delete pliga;
		pliga = NULL;
	}
	// limpa listas
	lisEQS->Clear();
	lisEQN->Clear();
	// reinicia objetos SEM destruí-los
	pliga = NULL;
	pbarra1 = NULL;
	// reinicia estado
	estado = SelecaoBarraInicial;
}
////---------------------------------------------------------------------------
// bool __fastcall TEdtLiga::VerificaSeFechaMalhaExterna(VTLigacao *lig)
// {
// VTBarra *barra;
// VTRede *rede;
// VTLigacao *ligacao;
// VTRede *outraRede;
// VTChave *chave;
// bool fechouMalha = false;
//
// if ((lig == NULL)|| (lig->rede == NULL))
// {
// return false;
// }
// rede = lig->rede;
// //se for uma chave aberta faz nada
// if (lig->Tipo() == eqptoCHAVE)
// {
// chave = (VTChave*)lig;
// if (chave->Estado == chvABERTA)
// {
// return false;
// }
// }
// //verifica todas barras
// for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
// {
// if ((barra = lig->Barra(ind_bar)) == NULL)
// {continue;}
// else
// {//verifica se a barra possui ligacoes q sejam de outra rede
// for(int nl = 0; nl < barra->LisLigacao()->Count; nl++)
// {
// ligacao = (VTLigacao*)barra->LisLigacao()->Items[nl];
// //verifica se é a mesma
// if (lig == ligacao)
// {continue;}
// //verifica se a ligacao é de outra rede
// outraRede = ligacao->rede;
// if(outraRede == rede)
// {continue;}
// //se a outra ligacao for uma chave aberta, ignora
// if (ligacao->Tipo() == eqptoCHAVE)
// {
// chave = (VTChave*)ligacao;
// if (chave->Estado == chvABERTA)
// {continue;}
// }
// //verifica se uma das redes é radial
// if (outraRede->Radial || rede->Radial)
// {
// //verifica se a barra analisada é inicial de uma das 2
// if (barra == outraRede->BarraInicial())
// {
// fechouMalha = false;
// continue;
// }
// else if (barra == rede->BarraInicial())
// {
// fechouMalha = false;
// continue;
// }
// else
// {
// fechouMalha = true;
// }
// }
// }
// }
// }
// return fechouMalha;
// }

// ---------------------------------------------------------------------------
bool __fastcall TEdtLiga::VnomDiferente(double vnom1, double vnom2)
{
	return (!VnomIgual(vnom1, vnom2));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtLiga::VnomIgual(double vnom1, double vnom2)
{
	return (RoundTo(vnom1, -5) == RoundTo(vnom2, -5));
}

// ---------------------------------------------------------------------------
// eof
