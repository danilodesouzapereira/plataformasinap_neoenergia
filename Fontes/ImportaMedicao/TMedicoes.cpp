// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Bloco\VTBloco.h>
#include <Fontes\Bloco\VTBlocos.h>
#include <Fontes\Bloco\VTElo.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTNode.h>
#include <Fontes\Radial\VTPrimario.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTSecundario.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCanal.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTMedidor.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTResFlowLig.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Bloco.h>
#include <DLL_Inc\Curvas.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Rede.h>
#include "TMedicoes.h"
#include "TConverte.h"
#include "TPontosMed.h"
#include "VTMedicao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// fun��o p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTMedicoes* __fastcall NewObjMedicoes(VTApl *apl_owner)
{
	try
	{
		return (new TMedicoes(apl_owner));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TMedicoes::TMedicoes(VTApl *apl_owner)
{
	// variaveis locais
	VTRedes *redes;

	// guarda ponteiro
	apl = apl_owner;
	// cria objetos
	stringListArq = new TStringList();
	campos = new TStringList();
	lisMED_ALL = new TList();
	lisMED_SEL = new TList();
	lisMEDIDOR = new TList();
	lisPRI_VAL = new TList();
	lisREDE_VAL = new TList();
	lisMED_DJ = new TList();
	pontos_med = new TPontosMed(apl);
	arvore = DLL_NewObjArvore(apl);
	arvore_bloco = DLL_NewObjArvore(apl);
	// inicia arvore de redes
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes)); // ???
	arvore->TipoArvore = arvoreREDE;
}

// ---------------------------------------------------------------------------
__fastcall TMedicoes::~TMedicoes(void)
{
	// destr�i objetos
	if (stringListArq)
	{
		delete stringListArq;
		stringListArq = NULL;
	}
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	if (pontos_med)
	{
		delete pontos_med;
		pontos_med = NULL;
	}
	if (arvore)
	{
		delete arvore;
		arvore = NULL;
	}
	if (arvore_bloco)
	{
		delete arvore_bloco;
		arvore_bloco = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisMED_SEL)
	{
		delete lisMED_SEL;
		lisMED_SEL = NULL;
	}
	if (lisMEDIDOR)
	{
		delete lisMEDIDOR;
		lisMEDIDOR = NULL;
	}
	if (lisPRI_VAL)
	{
		delete lisPRI_VAL;
		lisPRI_VAL = NULL;
	}
	if (lisREDE_VAL)
	{
		delete lisREDE_VAL;
		lisREDE_VAL = NULL;
	}
	if (lisMED_DJ)
	{
		delete lisMED_DJ;
		lisMED_DJ = NULL;
	}
	// destr�i lista e seus objetos
	if (lisMED_ALL)
	{
		LimpaTList(lisMED_ALL);
		delete lisMED_ALL;
		lisMED_ALL = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::AcertaBarraMedidores(void)
{ // altera medidores para ficarem na barra do bloco mais pr�ximo ao suprimento
	// variaveis locais
	bool reinicia_lista = true;
	// int       ind_bar, index1, index2;
	int ind_bar;
	VTBarra *barra_nova;
	VTBloco *bloco_jus, *bloco_mon;
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTChave *chave;
	VTElo *elo;
	VTMedidor *medidor, *clone;
	VTNode *node1, *node2;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisNODE, *lisEQP, *lisMED_CLONE;

	// prote��o
	if (!redes)
		return;
	if (!blocos)
		return;
	// inicia blocos
	if (!blocos->Executa(redes))
		return;
	// utiliza �rvore de blocos
	if (!arvore_bloco->Inicia(blocos))
		return;
	// cria  listas
	lisNODE = new TList();
	lisEQP = new TList();
	lisMED_CLONE = new TList();
	arvore_bloco->LisAllNode(lisNODE);
	// inicia lista com todos medidores da rede
	PreencheLisMedidor(reinicia_lista);
	lisMED_CLONE->Clear();
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		lisEQP->Clear();
		medidor->LisEqptoDosCanais(lisEQP);
		if (lisEQP->Count == 0)
		{
			continue;
		}
		chave = (VTChave*)lisEQP->First();
		// localiza elo
		elo = blocos->ExisteElo(chave);
		// determina nodes dos blocos ligados ao elo
		node1 = ExisteNode(arvore_bloco, elo->Bloco1);
		node2 = ExisteNode(arvore_bloco, elo->Bloco2);
		if ((node1 == NULL) || (node2 == NULL))
			continue;
		// determina hierarquia dos nodes
		/*
		 //JCG 2017.06.26
		 index1 = lisNODE->IndexOf(node1);
		 index2 = lisNODE->IndexOf(node2);
		 if((index1 < 0) || (index2 < 0)) continue;
		 //verifica qual bloco � montante e qual est� a jusante
		 bloco_mon = elo->Bloco1;
		 bloco_jus = elo->Bloco2;
		 if(index1 > index2)
		 {
		 bloco_mon = elo->Bloco2;
		 bloco_jus = elo->Bloco1;
		 }
		 */
		if (node2->Pai == node1)
		{
			bloco_mon = elo->Bloco1;
			bloco_jus = elo->Bloco2;
		}
		else if (node1->Pai == node2)
		{
			bloco_mon = elo->Bloco2;
			bloco_jus = elo->Bloco1;
		}
		else
		{ // isso n�o pode ocorrer
			continue;
		}
		// determina barra do bloco a montante
		ind_bar = IndBarraMontante(elo, bloco_jus, bloco_mon);
		// verifica se o medidor est� na barra certa
		barra_nova = elo->Chave->Barra(ind_bar);
		if (medidor->Barra != barra_nova)
		{
			clone = medidor->Clone();
			medidor->Obj = clone;
			clone->DefineObjBarra(barra_nova);
			lisMED_CLONE->Add(medidor);
		}
	}
	// move todos os medidores errados para barra certa
	pontos_med->MoveMedidores(lisMED_CLONE);
	// destroi lista
	delete lisNODE;
	delete lisMED_CLONE;
	delete lisEQP;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::AlteraEqptosMedidor(VTMedidor *medidor)
{
	// variaveis locais
	VTCanal *canal, *novo_canal;
	VTChave *chave;
	VTEqpto *eqpto;
	VTLigacao *ligacao;
	VTPrimario *primario;
	TList *lisCANAL, *lisEQPTO;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cria lista
	lisEQPTO = new TList();
	lisCANAL = medidor->LisCanal();
	for (int nc = lisCANAL->Count - 1; nc >= 0; nc--)
	{
		canal = (VTCanal*)lisCANAL->Items[nc];
		if (canal->Eqpto->Tipo() != eqptoCHAVE)
			continue;
		// localiza chave ficticia
		chave = (VTChave*)canal->Eqpto;
		lisEQPTO->Clear();
		// procura liga��es vizinhas � chave
		redes->LisLigacao(lisEQPTO, chave->pbarra1);
		redes->LisLigacao(lisEQPTO, chave->pbarra2);
		for (int ne = 0; ne < lisEQPTO->Count; ne++)
		{ // localiza outra liga��o da mesma rede
			eqpto = (VTEqpto*)lisEQPTO->Items[ne];
			if (eqpto == chave)
				continue;
			if (!eqpto->TipoLigacao())
				continue;
			ligacao = (VTLigacao*)eqpto;
			if (ligacao->rede != chave->rede)
				continue;
			// copia curva do canal antigo para o novo
			novo_canal = medidor->ExisteCanal(ligacao, canal->Tipo(), canal->Fases);
			if (!novo_canal)
				novo_canal = medidor->InsereCanal(ligacao, canal->Tipo(), canal->Fases);
			novo_canal->Curva = canal->Curva->Clone();
			novo_canal->Curva->Id = FalsoId();
			break;
		}
	}
	delete lisEQPTO;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::AtualizaMedidores(void)
{
	// variaveis locais
	VTMedidor *medidor;
	VTCanal *canal;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCANAL;
	bool reinicia_lista = true;

	// inicia lista com todos medidores da rede
	PreencheLisMedidor(reinicia_lista);
	// lisMEDIDOR->Clear();
	// redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		lisCANAL = medidor->LisCanal();
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // insere resultado do fluxo na curva estimada
			canal = (VTCanal*)lisCANAL->Items[nc];
			DefineCurvaEstimada(canal);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::Clear(void)
{
	// reinicia lista de Medicao selecionadas
	lisMED_SEL->Clear();
	lisMEDIDOR->Clear();
	// destr�i todos objetos Medicao
	LimpaTList(lisMED_ALL);
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TMedicoes::ConverteMedidorPtrifasicoToPQfase(void)
 {
 // vari�veis locais
 TList *lisCANAL;
 TConverte *converte;
 VTCanal *canal;
 VTMedidor *medidor;
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

 // cria objeto Converte
 if ((converte = new TConverte(apl)) == NULL)
 return;
 // limpa lista de Medidores
 lisMEDIDOR->Clear();
 redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
 for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
 {
 medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
 // verifica se o Medidor possui um �nico Canal de pot�ncia trif�sica
 lisCANAL = medidor->LisCanal();
 if (lisCANAL->Count == 1)
 {
 canal = (VTCanal*)lisCANAL->First();
 if ((canal->TipoCanal == canalP) && ((canal->Fases & faseABC) == faseABC))
 { // converte o canal de pot�ncia trif�sica em 3 canais P e 3 canais Q
 converte->CanalP3FparaCanaisPfase(medidor, canal);
 }
 }
 }
 delete converte;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ConverteMedidorPtrifasicoToPQfase(void)
{
	// vari�veis locais
	TList *lisCANAL;
	TConverte *converte;
	VTCanal *canal;
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// limpa lista de Medidores
	lisMEDIDOR->Clear();
	redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		// verifica Canal de pot�ncia ativa e reativa trif�sica
		lisCANAL = medidor->LisCanal();
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if ((canal->TipoCanal == canalP) && ((canal->Fases & faseABC) == faseABC))
			{ // converte o canal de pot�ncia trif�sica em 3 canais P
				converte->CanalP3FparaCanaisPfase(medidor, canal);
				nc--;
			}
			else if ((canal->TipoCanal == canalQ) && ((canal->Fases & faseABC) == faseABC))
			{ // converte o canal de pot�ncia trif�sica em 3 canais Q
				converte->CanalQ3FparaCanaisQfase(medidor, canal);
				nc--;
			}
		}
	}
	delete converte;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ConverteMedI(TList *lisI, VTMedicao *medicao)
{
	// vari�veis locais
	TConverte *converte;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// converte os canais de corrente em uma curva PQ por fase
	converte->CanaisI(lisI, medicao);
	delete converte;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ConverteMedPQ(TList *lisPQ, VTMedicao *medicao)
{
	// vari�veis locais
	TConverte *converte;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// converte os canais em uma curva PQ por fase
	converte->CanaisPQ(lisPQ, medicao);
	delete converte;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ConverteMedP(TList *lisPQ, VTMedicao *medicao)
{
	// vari�veis locais
	TConverte *converte;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// converte os canais de pot.ativa em uma curva PQ por fase
	converte->CanaisP(lisPQ, medicao);
	delete converte;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
void __fastcall TMedicoes::ConverteMedQ(TList *lisPQ, VTMedicao *medicao)
{
	// vari�veis locais
	TConverte *converte;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// converte os canais de pot.reativa em uma curva PQ por fase
	converte->CanaisQ(lisPQ, medicao);
	delete converte;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ConverteMedS(TList *lisS, VTMedicao *medicao)
{
	// vari�veis locais
	TConverte *converte;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// converte os canais de pot.aparente em uma curva PQ por fase
	converte->CanaisS(lisS, medicao);
	delete converte;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::CorrenteToPQfase(void)
{
	// vari�veis locais
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TConverte *converte;
	TList *lisCANAL;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// limpa lista de Medidores
	lisMEDIDOR->Clear();
	redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		// converte os canais de corrente em uma curva PQ por fase
		converte->CanaisIparaCanaisPQfase(medidor);
	}
	delete converte;
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::CorrenteToPQtrifasico(void)
{
	// vari�veis locais
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TConverte *converte;
	TList *lisCANAL;

	// cria objeto Converte
	if ((converte = new TConverte(apl)) == NULL)
		return;
	// limpa lista de Medidores
	lisMEDIDOR->Clear();
	redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		// converte os canais de corrente em uma curva PQ trifasica
		converte->CanaisIparaCanalPQtrifasico(medidor);
	}
	delete converte;
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TMedicoes::CriaChaveFicticia(VTCanal *canal)
{
	// variaveis locais
	VTChave *chave;

	// prote��o
	if (!pontos_med)
		return (NULL);
	if (!canal)
		return (NULL);
	// verifica se a chave ja existe
	if ((chave = ExisteChave(canal)) != NULL)
		return (chave);
	// n�o existe: cria chave
	chave = pontos_med->CriaChaveFicticia(canal);
	return (chave);
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TMedicoes::CriaCurva(int num_ponto, int num_valores)
{
	// vari�veis locais
	VTCurva *curva;

	try
	{ // cria e inicia Curva
		curva = DLL_NewObjCurva();
		curva->Id = FalsoId();
		curva->Eqpto = NULL;
		curva->Classe = NULL;
		curva->Escala = num_ponto;
		curva->NumeroValores = num_valores;
		curva->Tipo = Tipo;
		curva->Unidade = -1;
		curva->Reconfigura(num_ponto);
	}
	catch (Exception &e)
	{
		curva = NULL;
	}
	return (curva);
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicoes::CriaMedicoesMedidores(void)
{
	// variaveis locais
	VTMedidor *medidor;
	VTMedicao *medicao;

	try
	{
		for (int nm = 0; nm < lisMED_SEL->Count; nm++)
		{ // cria medicao com curva PQ por fase
			medidor = (VTMedidor*)lisMED_SEL->Items[nm];
			medicao = CriaMedicaoMedidor(medidor);
			lisMED_ALL->Add(medicao);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
VTMedicao* __fastcall TMedicoes::CriaMedicaoMedidor(VTMedidor *medidor)
{
	// vari�veis locais
	bool existe_p = false;
	bool existe_q = false;
	VTCanal *canal;
	VTChave *chave_fic = NULL;
	VTChave *chave;
	VTMedicao *medicao;
	TList *lisCANAL, *lisI, *lisPQ, *lisS;
	int num_pto = pat01HORA;
	int num_valores = MAX_FASE + MAX_FASE;

	// prote��o
	if (!medidor)
		return (NULL);
	// cria listas
	lisI = new TList();
	lisPQ = new TList();
	lisS = new TList();
	// cria um novo objeto Medicao e suas curvas
	medicao = NewObjMedicao();
	medicao->curva_pq = CriaCurva(num_pto, num_valores);
	medicao->enabled = true;
	lisCANAL = medidor->LisCanal();
	for (int nc = 0; nc < lisCANAL->Count; nc++)
	{ // verifica se h� algum canal com medi��o
		canal = (VTCanal*)lisCANAL->Items[nc];
		// verifica se o canal est� enabled
		if (!canal->Enabled)
			continue;
		// verifica se � ligacao
		if (!canal->Eqpto->TipoLigacao())
			continue;
		// verifica se � chave
		if (canal->Eqpto->Tipo() != eqptoCHAVE)
		{ // verifica se h� alguma chave pr�xima ao medidor //DVK 2016.01.12
			if ((chave = ExisteChave(medidor)) != NULL)
				medicao->chave_med = chave;
			else
			{ // cria chave ficticia e associa � medi��o
				if (!chave_fic)
					chave_fic = CriaChaveFicticia(canal);
				if (!medicao->chave_med)
					medicao->chave_med = chave_fic;
			}
		}
		else
		{ // associa diretamente a chave do canal � medi��o
			if (!medicao->chave_med)
				medicao->chave_med = (VTChave*)canal->Eqpto;
		}
		// separa canais do mesmo tipo
		switch (canal->TipoCanal)
		{
		case canalI:
			if (lisI->IndexOf(canal) < 0)
				lisI->Add(canal);
			break;
		case canalP:
			if (lisPQ->IndexOf(canal) < 0)
				lisPQ->Add(canal);
			existe_p = true;
			break;
		case canalQ:
			if (lisPQ->IndexOf(canal) < 0)
				lisPQ->Add(canal);
			existe_q = true;
			break;
		case canalS:
			if (lisS->IndexOf(canal) < 0)
				lisS->Add(canal);
			break;
		default:
			break;
		}
	}
	// preenche curva de medi��o, dependendo do tipo do canal
	if (lisPQ->Count > 0)
	{
		if (existe_p && existe_q)
			ConverteMedPQ(lisPQ, medicao);
		else if(existe_p)
			ConverteMedP(lisPQ, medicao);
		else if(existe_q)
			ConverteMedQ(lisPQ, medicao);
	}
	else if (lisI->Count > 0)
		ConverteMedI(lisI, medicao);
	else if (lisS->Count > 0)
		ConverteMedS(lisPQ, medicao);
	else
	{ // n�o considera medidor: destroi medicao
		delete medicao;
		medicao = NULL;
	}
	// calcula maior erro  //DVK 2016.05.31
	medicao->erro = medicao->MaiorErro();
	// destroi listas
	delete lisI;
	delete lisPQ;
	delete lisS;
	return (medicao);
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::DefineCurvaEstimada(VTCanal *canal)
{
	// variaveis locais
	VTLigacao *ligacao;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();
	double valor;
	int nb = 1;

	// prote��o
	if (!canal)
		return;
	if (!canal->Eqpto)
		return;
	if (!canal->Eqpto->TipoLigacao())
		return;
	ligacao = (VTLigacao*)canal->Eqpto;
	// determina �ndice da barra onde est� o medidor
	nb = ligacao->IndiceBarra(canal->Medidor->Barra);
	if (nb < 0)
		return;
	canal->Curva->Reconfigura(patamares->Escala);
	for (int np = 0; np < lisPAT->Count; np++)
	{ // verifica o valor do fluxo em cada patamar
		patamar = (VTPatamar*)lisPAT->Items[np];
		// verifica o tipo do canal para atualizar a curva estimada
		switch (canal->TipoCanal)
		{
		case canalI:
			if (canal->Fases == faseA)
				valor = Abs(ligacao->resflow->Ia_amp[nb][np]);
			if (canal->Fases == faseB)
				valor = Abs(ligacao->resflow->Ib_amp[nb][np]);
			if (canal->Fases == faseC)
				valor = Abs(ligacao->resflow->Ic_amp[nb][np]);
			break;
		case canalV:
			break;
		case canalP:
			if (canal->Fases == faseA)
				valor = (ligacao->resflow->Pa_mw[nb][np]);
			if (canal->Fases == faseB)
				valor = (ligacao->resflow->Pb_mw[nb][np]);
			if (canal->Fases == faseC)
				valor = (ligacao->resflow->Pc_mw[nb][np]);
			if (canal->Fases == faseABC)
				valor = (ligacao->resflow->P_mw[nb][np]);
			break;
		case canalQ:
			if (canal->Fases == faseA)
				valor = (ligacao->resflow->Qa_mvar[nb][np]);
			if (canal->Fases == faseB)
				valor = (ligacao->resflow->Qb_mvar[nb][np]);
			if (canal->Fases == faseC)
				valor = (ligacao->resflow->Qc_mvar[nb][np]);
			if (canal->Fases == faseABC)
				valor = (ligacao->resflow->Q_mvar[nb][np]);
			break;
		case canalS:
			if (canal->Fases == faseA)
				valor = Abs(ligacao->resflow->Sa_mva[nb][np]);
			if (canal->Fases == faseB)
				valor = Abs(ligacao->resflow->Sb_mva[nb][np]);
			if (canal->Fases == faseC)
				valor = Abs(ligacao->resflow->Sc_mva[nb][np]);
			if (canal->Fases == faseABC)
				valor = Abs(ligacao->resflow->S_mva[nb][np]);
			break;
		default:
			break;
		}
		canal->ValorEstimado[patamar] = valor;
	}
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TMedicoes::ExisteChave(VTCanal *canal)
{
	// variaveis locais
	VTChave *chave;
	VTMedicao *medicao;

	// prote��o
	if (!canal)
		return (NULL);
	// verifica se h� alguma medi��o associada a uma chave com mesmo c�digo do medidor
	medicao = ExisteMedicao(canal->Medidor->Codigo);
	if (medicao)
		return (medicao->chave_med);
	return (NULL);
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TMedicoes::ExisteChave(VTMedidor *medidor)
{ // DVK 2016.01.12
	// variaveis locais
	VTChave *chave = NULL;
	VTLigacao *ligacao;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisEQP;

	// prote��o
	if (!medidor)
		return (NULL);
	lisEQP = new TList();
	redes->LisLigacao(lisEQP, medidor->Barra);
	for (int ne = 0; ne < lisEQP->Count; ne++)
	{ // verifica se h� alguma chave na mesma barra do medidor
		ligacao = (VTLigacao*)lisEQP->Items[ne];
		if (ligacao->Tipo() != eqptoCHAVE)
			continue;
		chave = (VTChave*)ligacao;
		break;
	}
	// destroi lista
	delete lisEQP;
	return (chave);
}

// ---------------------------------------------------------------------------
VTMedicao* __fastcall TMedicoes::ExisteMedicao(AnsiString cod_chave)
{
	// vari�veis locais
	VTMedicao *medicao;

	// procura Medicao em lisMED_ALL
	for (int n = 0; n < lisMED_ALL->Count; n++)
	{
		medicao = (VTMedicao*)lisMED_ALL->Items[n];
		if (medicao->chave_med == NULL)
			continue;
		if (medicao->chave_med->Codigo.AnsiCompareIC(cod_chave) == 0)
			return (medicao);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTMedidor* __fastcall TMedicoes::ExisteMedidor(VTChave *chave)
{
	// vari�veis locais
	VTMedidor *medidor;
	VTCanal *canal;
	TList *lisCANAL;

	// procura Medidor em lisMEDIDOR
	for (int n = 0; n < lisMEDIDOR->Count; n++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[n];
		lisCANAL = medidor->LisCanal();
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // procura canal associado a chave
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto == chave)
				return (medidor);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTNode* __fastcall TMedicoes::ExisteNode(VTArvore *arvore_bloco, VTBloco *bloco)
{
	// vari�veis locais
	VTNode *node;
	TList *lisNODE;

	// prote��o
	if (arvore_bloco == NULL)
		return (NULL);
	if (bloco == NULL)
		return (NULL);
	// cria lista
	lisNODE = new TList();
	arvore_bloco->LisAllNode(lisNODE);
	// loop p/ todos Nodes da Arvore
	for (int n = 0; n < lisNODE->Count; n++)
	{ // localiza node associado ao bloco
		node = (VTNode*)lisNODE->Items[n];
		if (node->Object == bloco)
			break;
		else
			node = NULL;
	}
	// destroi lista
	delete lisNODE;
	return (node);
}

// ---------------------------------------------------------------------------
VTNode* __fastcall TMedicoes::ExisteNode(VTRede *rede)
{
	// vari�veis locais
	VTNode *node;
	TList *lisNODE, *lisREDE;

	// prote��o
	if (rede == NULL)
		return (NULL);
	// cria lista
	lisNODE = new TList();
	arvore->LisAllNode(lisNODE);
	// loop p/ todos Nodes da Arvore
	for (int n = 0; n < lisNODE->Count; n++)
	{
		node = (VTNode*)lisNODE->Items[n];
		lisREDE = node->LisObjectNode();
		if (lisREDE->IndexOf(rede) >= 0)
			break;
		else
			node = NULL;
	}
	// destroi lista
	delete lisNODE;
	return (node);
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicoes::ExisteVizinhaEquivalentada(VTPrimario *primario)
{
	// variaveis locais
	VTSecundario *secundario;
	TList *lisSEC = primario->LisSecundario();

	// verifica se o Primario alimenta algum secund�rio equivalentado
	for (int nr = 0; nr < lisSEC->Count; nr++)
	{ // verifica se a rede est� equivalentada
		secundario = (VTSecundario*)lisSEC->Items[nr];
		if (!secundario->Rede->Carregada)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicoes::ExisteVizinhaEquivalentada(VTRede *rede)
{
	// variaveis locais
	bool existe = false;
	TList *lisJUS;
	VTNode *node_rede;
	VTRede *rede_jus;

	// prote��o
	if (rede == NULL)
		return (true);
	// localiza node da rede
	node_rede = ExisteNode(rede);
	// cria lista
	lisJUS = new TList();
	node_rede->LisObjectJusante(lisJUS);
	// verifica se a rede alimenta alguma rede equivalentada
	for (int nr = 0; nr < lisJUS->Count; nr++)
	{ // verifica se h� alguma rede equivalentada
		rede_jus = (VTRede*)lisJUS->Items[nr];
		if (rede_jus->Carregada)
			continue;
		existe = true;
		break;
	}
	// destroi lista
	delete lisJUS;
	return (existe);
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicoes::FluxoZerado(VTRede *rede)
{
	// vari�veis locais
	int valores_zerados = 0;
	int nb = 0;
	double valor;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	TList *lisPAT = patamares->LisPatamar();
	VTPrimario *primario;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	//verifica se rodou fluxo
	if(flow == NULL)
		return false;
	if(! flow->UpToDate)
		return false;
	//prote��o
	primario = radial->ExistePrimario(rede);
	if(primario == NULL)
		return true;
	if(primario->LigacaoInicial == NULL)
		return true;
	if(primario->BarraInicial == NULL)
		return true;
	//determina indice da barra
	nb = primario->LigacaoInicial->IndiceBarra(primario->BarraInicial);
	//verifica se todos os patamares o valor de P � zerado
	for(int np = 0; np < lisPAT->Count; np++)
	{
		patamar = patamares->Patamar[np];
		valor = primario->LigacaoInicial->resflow->P_mw[nb][np];
		if(IsDoubleZero(valor))
			valores_zerados++;
    }
	return (lisPAT->Count == valores_zerados);
}

/*
 //---------------------------------------------------------------------------
 int __fastcall TMedicoes::IndBarraMontante(VTBloco *bloco_jus, VTBloco *bloco_mon)
 {
 //vari�veis locais
 VTElo       *elo;
 VTChave     *chave;
 VTBarra     *barra;
 VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

 //prote��o
 if(bloco_jus == NULL) return(0);
 if(bloco_mon == NULL) return(0);
 //localiza elo
 elo = blocos->ExisteElo(bloco_jus, bloco_mon);
 chave = elo->Chave;
 //testa barra1
 barra = chave->pbarra1;
 if(! bloco_mon->ExisteBarra(barra) && (bloco_jus->ExisteBarra(barra)))
 {//a barra montante � a barra2
 barra = chave->pbarra2;
 }
 return(chave->IndiceBarra(barra));
 }
 */
// ---------------------------------------------------------------------------
int __fastcall TMedicoes::IndBarraMontante(VTElo *elo, VTBloco *bloco_jus, VTBloco *bloco_mon)
{
	// vari�veis locais
	VTChave *chave;
	VTBarra *barra;

	// prote��o
	if (elo == NULL)
		return (0);
	if (bloco_jus == NULL)
		return (0);
	if (bloco_mon == NULL)
		return (0);
	// localiza elo
	chave = elo->Chave;
	// testa barra1
	barra = chave->pbarra1;
	if (!bloco_mon->ExisteBarra(barra) && (bloco_jus->ExisteBarra(barra)))
	{ // a barra montante � a barra2
		barra = chave->pbarra2;
	}
	return (chave->IndiceBarra(barra));
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::InsereDisjuntoresFicticios(bool clear_lisRedeVal)
{
	// vari�veis locais
	VTChave *chave;
	VTLigacao *ligacao;
	VTRede *redeMT;
	TList *lisLIGACOES;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool possui_chave = false;

	try
	{
		lisLIGACOES = new TList;
		// preenche lista de redes MT validas
		if (clear_lisRedeVal)
		{
			PreencheLisRedeVal();
		}
		else if (lisREDE_VAL->Count == 0)
		{
			PreencheLisRedeVal();
		}
		// identifica as redes que n�o tem chave como liga��o inicial
		for (int n = 0; n < lisREDE_VAL->Count; n++)
		{
			// reseta o possui chaves
			possui_chave = false;
			// somente para redes mt v�lidos
			redeMT = (VTRede*)lisREDE_VAL->Items[n];
			// caso a rede n�o possua barra inicial...
			if (redeMT->BarraInicial() == NULL)
			{
				continue;
			}
			// Limpa uma lista de ligacoes
			lisLIGACOES->Clear();
			// obtem a lista de ligacoes na barra inicial dessa rede
			redes->LisLigacao(lisLIGACOES, redeMT->BarraInicial());
			// varre a lista procurando chaves dessa rede
			for (int nl = 0; nl < lisLIGACOES->Count; nl++)
			{
				ligacao = (VTLigacao*)lisLIGACOES->Items[nl];
				// verifica se a liga��o � uma chave FECHADA e se pertence a mesma rede
				if ((ligacao->rede == redeMT) && (ligacao->Tipo() == eqptoCHAVE))
				{
					if (((VTChave*)ligacao)->Fechada)
					{
						possui_chave = true;
						break;
					}
				}
			}
			// caso n�o possua chave, insere uma
			if (!possui_chave)
			{
				chave = pontos_med->CriaChaveFicticia(redeMT);
				// reseta
				possui_chave = false;
			}
		}
		delete lisLIGACOES;
	}
	catch (Exception &e)
	{
		if (lisLIGACOES)
		{
			delete lisLIGACOES;
			lisLIGACOES = NULL;
		}
	}
}

////---------------------------------------------------------------------------
// void __fastcall TMedicoes::InsereDisjuntoresFicticios(void)
// {
// //vari�veis locais
// VTChave    *chave;
// VTLigacao  *ligacao;
// VTPrimario *primario;
//
// //preenche lista de primarios validos
// PreencheLisPriVal();
// //identifica Primarios que n�o tem chave como liga��o inicial
// for (int n = 0; n < lisPRI_VAL->Count; n++)
// {//somente para prim�rios v�lidos
// primario = (VTPrimario*)lisPRI_VAL->Items[n];
// ligacao = primario->LigacaoInicial;
// if ((ligacao == NULL) || (ligacao->Tipo() != eqptoCHAVE))
// {//cria chave ficticia
// chave = pontos_med->CriaChaveFicticia(primario);
// }
// }
// }
/*
 //---------------------------------------------------------------------------
 VTMedidor* __fastcall TMedicoes::InsereMedidor(VTChave *chave)
 {
 //variaveis locais
 int       ind_bar;
 VTBloco   *bloco_jus, *bloco_mon;
 VTBlocos  *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
 VTElo     *elo;
 VTMedidor *medidor = NULL;
 VTNode    *node;
 VTRedes   *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
 TList     *lisNODE;

 //prote��o
 if(! chave)  return(NULL);
 if(! redes)  return(NULL);
 //localiza medidor associado � chave
 if((medidor = ExisteMedidor(chave)) != NULL) return(medidor);
 //inicia blocos
 if(! blocos) apl->Add(blocos = DLL_NewObjBlocos());
 if(! blocos->Executa(redes)) return(NULL);
 //n�o existe: cria medidor na barra do bloco mais pr�ximo ao suprimento
 //utiliza �rvore de blocos
 if(! arvore_bloco->Inicia(blocos)) return(NULL);
 lisNODE = new TList();
 arvore_bloco->LisAllNode(lisNODE);
 for(int n = lisNODE->Count - 1; n >= 0; n--)
 {//come�a das folhas pra raiz
 node      = (VTNode*)lisNODE->Items[n];
 bloco_jus = (VTBloco*)node->Object;
 if(! node->Pai) continue;
 bloco_mon = (VTBloco*)node->Pai->Object;
 elo = blocos->ExisteElo(bloco_jus, bloco_mon);
 //verifica se � a chave que une os blocos
 if(elo->Chave == chave)
 {//determina barra do bloco a montante
 ind_bar = IndBarraMontante(bloco_jus, bloco_mon);
 //insere medidor nessa barra do bloco a montante
 medidor = pontos_med->CriaMedidor(chave, ind_bar);
 break;
 }
 }
 //destroi lista
 delete lisNODE;
 return(medidor);
 }
 */
/*
 //---------------------------------------------------------------------------
 VTMedidor* __fastcall TMedicoes::InsereMedidor(VTChave *chave)
 {//DVK 2016.12.09 outra implementa��o
 //variaveis locais
 int       ind_bar, index1, index2;
 VTBloco   *bloco_jus, *bloco_mon;
 VTBlocos  *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
 VTElo     *elo;
 VTMedidor *medidor   = NULL;
 VTNode    *node1, *node2;
 VTRedes   *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
 TList     *lisNODE;

 //prote��o
 if(! chave)  return(NULL);
 if(! redes)  return(NULL);
 if(! blocos)  return(NULL);
 //localiza medidor associado � chave
 if((medidor = ExisteMedidor(chave)) != NULL) return(medidor);
 //n�o existe: cria medidor na barra do bloco mais pr�ximo ao suprimento
 //inicia blocos
 if(! blocos->Executa(redes))                 return(NULL);
 //utiliza �rvore de blocos
 if(! arvore_bloco->Inicia(blocos))           return(NULL);
 lisNODE = new TList();
 arvore_bloco->LisAllNode(lisNODE);
 //localiza elo
 elo = blocos->ExisteElo(chave);
 //determina nodes dos blocos ligados ao elo
 node1 = ExisteNode(arvore_bloco, elo->Bloco1);
 node2 = ExisteNode(arvore_bloco, elo->Bloco2);
 if((node1 == NULL) || (node2 == NULL))
 {
 delete lisNODE;
 return(NULL);
 }
 //determina hierarquia dos nodes
 index1 = lisNODE->IndexOf(node1);
 index2 = lisNODE->IndexOf(node2);
 if((index1 < 0) || (index2 < 0))
 {
 delete lisNODE;
 return(NULL);
 }
 //verifica qual bloco � montante e qual est� a jusante
 bloco_mon = elo->Bloco1;
 bloco_jus = elo->Bloco2;
 if(index1 > index2)
 {
 bloco_mon = elo->Bloco2;
 bloco_jus = elo->Bloco1;
 }
 //determina barra do bloco a montante
 ind_bar = IndBarraMontante(bloco_jus, bloco_mon);
 //insere medidor nessa barra do bloco a montante
 medidor = pontos_med->CriaMedidor(chave, ind_bar);
 //destroi lista
 delete lisNODE;
 return(medidor);
 }
 */
/*
 //---------------------------------------------------------------------------
 VTMedidor* __fastcall TMedicoes::InsereMedidor(VTChave *chave)
 {//DVK 2017.04.28 outra implementa��o sem ordenar blocos
 //variaveis locais
 int       ind_bar;
 VTMedidor *medidor   = NULL;

 //prote��o
 if(! chave)  return(NULL);
 //localiza medidor associado � chave
 if((medidor = ExisteMedidor(chave)) != NULL) return(medidor);
 //JCG - 2017.06.26 - isso n�o est� funcionando direito
 //n�o existe: cria medidor em qualquer barra da chave (depois ser� acertado)
 ind_bar = chave->IndiceBarra(chave->pbarra1);
 //insere medidor nessa barra do bloco a montante
 medidor = pontos_med->CriaMedidor(chave, ind_bar);
 return(medidor);
 }
 */
// ---------------------------------------------------------------------------
VTMedidor* __fastcall TMedicoes::InsereMedidor(VTChave *chave)
{ // DVK 2017.04.28 outra implementa��o sem ordenar blocos
	// variaveis locais
	int ind_bar;
	VTMedidor *medidor = NULL;

	// prote��o
	if (!chave)
		return (NULL);
	// localiza medidor associado � chave
	if ((medidor = ExisteMedidor(chave)) != NULL)
		return (medidor);
	// determina Barra para o qual o fluxo � positivo
	ind_bar = 0;
	if (chave->resflow->SentidoP[ind_bar][0] == fluxoNEG)
		ind_bar = 1;
	// insere medidor na barra selecionada
	medidor = pontos_med->CriaMedidor(chave, ind_bar);
	return (medidor);
}

// ---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedicao(void)
{
	return (lisMED_ALL);
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::LisMedicaoComErroNoAjuste(TList *lisMED_ERRO)
{
	// vari�veis locais
	VTMedicao *medicao;

	// verifica se alguma Medicao apresentou erro
	for (int i = 0; i < lisMED_ALL->Count; i++)
	{
		medicao = (VTMedicao*)lisMED_ALL->Items[i];
		if ((medicao->diagnostico == diagMED_ERRO_INICIAL) ||
			(medicao->diagnostico == diagMED_ERRO_FINAL) ||
			(medicao->diagnostico == diagMED_ERRO_FLUXO))
			lisMED_ERRO->Add(medicao);
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedidor(void)
{
	return (lisMEDIDOR);
}

// ---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedidorAjustado(void)
{
	// vari�veis locais
	VTMedicao *medicao;
	VTMedidor *medidor, *clone;

	// limpa lisMED_SEL
	lisMED_SEL->Clear();
	for (int nm = 0; nm < lisMED_ALL->Count; nm++)
	{
		medicao = (VTMedicao*)lisMED_ALL->Items[nm];
		// verifica o diagnostico da medi��o
		if (medicao->diagnostico != diagMED_OK)
			continue;
		// localiza medidores de cada medi��o
		medidor = ExisteMedidor(medicao->chave_med);
		if (!medidor)
			continue;
		// clona medidor
		clone = medidor->Clone();
		clone->Id = FalsoId();
		// verifica se a chave da medi��o � fict�cia
		if (pontos_med->ChaveFicticia(medicao->chave_med))
		{ // altera eqpto do medidor
			AlteraEqptosMedidor(clone);
		}
		lisMED_SEL->Add(clone);
	}
	return (lisMED_SEL);
}

// ---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedicao(VTRede *rede)
{
	// vari�veis locais
	VTMedicao *medicao;

	// limpa lisMED_SEL
	lisMED_SEL->Clear();
	for (int nm = 0; nm < lisMED_ALL->Count; nm++)
	{
		medicao = (VTMedicao*)lisMED_ALL->Items[nm];
		if (medicao->chave_med->rede == rede)
			lisMED_SEL->Add(medicao);
	}
	return (lisMED_SEL);
}

// ---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedicaoDJ(void)
{
	// variaveis locais
	TList *lisLIGACOES = new TList();
	VTRede *redeMT;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *ligacao;
	VTMedidor *medidor;
	VTChave *chave;
	VTBarra *bar_ini;

	// limpa lista
	lisMED_DJ->Clear();
	// identifica as redes que n�o tem chave como liga��o inicial
	for (int n = 0; n < lisREDE_VAL->Count; n++)
	{
		// somente para redes mt v�lidos
		redeMT = (VTRede*)lisREDE_VAL->Items[n];
		// caso a rede n�o possua barra inicial...
		bar_ini = redeMT->BarraInicial();
		if (bar_ini == NULL)
		{
			continue;
		}
		// Limpa uma lista de ligacoes
		lisLIGACOES->Clear();
		// obtem a lista de ligacoes na barra inicial dessa rede
		redes->LisLigacao(lisLIGACOES, bar_ini);
		// varre a lista procurando chaves dessa rede
		for (int nl = 0; nl < lisLIGACOES->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIGACOES->Items[nl];
			// verifica se a liga��o � uma chave FECHADA e se pertence a mesma rede
			if ((ligacao->rede == redeMT) && (ligacao->Tipo() == eqptoCHAVE))
			{
				chave = (VTChave*)ligacao;
				if (chave->Aberta)
					continue;
				// verifica se existe medidor
				if ((medidor = ExisteMedidor(chave)) == NULL)
				{
					medidor = InsereMedidor(chave);
					if (medidor->Barra != bar_ini)
					{ // passa medidor para barra inicial
						medidor->Barra->RemoveEqbar(medidor);
						bar_ini->InsereEqbar(medidor);
					}
				}
				// insere medidor na lista
				lisMED_DJ->Add(medidor);
			}
		}
	}
	delete lisLIGACOES;

	return lisMED_DJ;
}

// ---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedicaoValida(bool med_neg_enabled)
{
	// vari�veis locais
	VTMedicao *medicao;

	// limpa lisMED_SEL
	lisMED_SEL->Clear();
	for (int nm = 0; nm < lisMED_ALL->Count; nm++)
	{ // insere na lista as medi��es v�lidas
		medicao = (VTMedicao*)lisMED_ALL->Items[nm];
		// verifica se h� medi��es em todas as fases dispon�veis do ponto medido
		if (!medicao->FasesValidas())
			continue;
		// prote��o: verifica se a Medi��o possui algum valor nulo ou negativo
		if (!medicao->enabled)
			continue;
		if (!med_neg_enabled)
		{
			if (medicao->CurvaNulaNegativa(medicaoCURVA_PQxFASE))
				medicao->enabled = false;
			else
				medicao->enabled = true;
		}
		if (!medicao->enabled)
			continue;
		if (!medicao->chave_med)
			continue;
		lisMED_SEL->Add(medicao);
	}
	return (lisMED_SEL);
}

/*
 // ---------------------------------------------------------------------------
 double __fastcall TMedicoes::MaiorErro(VTCanal *canal)
 {
 // variaveis locais
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTPatamar *patamar;
 TList *lisPAT = patamares->LisPatamar();
 double med, est;
 double erro = 0.;
 double maior = 0.;

 // prote��o
 if (!canal)
 return (0.);
 for (int np = 0; np < lisPAT->Count; np++)
 { // verifica a maior diferen�a entre todos os patamares
 patamar = (VTPatamar*)lisPAT->Items[np];
 med = canal->ValorMedido[patamar];
 est = canal->ValorEstimado[patamar];
 if (IsDoubleZero(med))
 continue;
 if (IsDoubleZero(est))
 continue;
 erro = (fabs(med - est) / med);
 // verifica se � negativo
 if (erro < 0)
 maior = Min(maior, erro);
 else
 maior = Max(maior, erro);
 }
 // retorna valor
 return (fabs(maior));
 }
 */
// ---------------------------------------------------------------------------
double __fastcall TMedicoes::MaiorErro(VTCanal *canal)
{
	// variaveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();
	double med, est;
	double erro = 0.;
	double maior = 0.;

	// prote��o
	if (!canal)
		return (0.);
	for (int np = 0; np < lisPAT->Count; np++)
	{ // verifica a maior diferen�a entre todos os patamares
		patamar = (VTPatamar*)lisPAT->Items[np];
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
	return (fabs(maior));
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TMedicoes::PreencheLisMedidor(void)
 {
 //variaveis locais
 VTPrimario     *primario;
 VTSecundario   *secundario;
 TList          *lisSEC;

 //preenche lista de primarios validos
 PreencheLisPriVal();
 //limpa lista
 lisMEDIDOR->Clear();
 //insere todos medidores dos prim�rios v�lidos na lista
 for(int np = 0; np < lisPRI_VAL->Count; np++)
 {
 primario = (VTPrimario*)lisPRI_VAL->Items[np];
 primario->Rede->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
 lisSEC = primario->LisSecundario();
 for(int ns = 0; ns < lisSEC->Count; ns++)
 {
 secundario = (VTSecundario*)lisSEC->Items[ns];
 secundario->Rede->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TMedicoes::PreencheLisMedidor(bool clear)
{
	// variaveis locais
	VTRede *rede;

	// preenche lista de primarios validos
	if (clear)
	{
		PreencheLisRedeVal();
	}
	else
	{
		if (lisREDE_VAL->Count == 0)
		{
			PreencheLisRedeVal();
		}
	}
	// limpa lista
	lisMEDIDOR->Clear();
	// insere todos medidores das redes v�lidas na lista
	for (int np = 0; np < lisREDE_VAL->Count; np++)
	{
		rede = (VTRede*)lisREDE_VAL->Items[np];
		rede->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::PreencheLisPriVal(void)
{
	// variaveis locais
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	VTPrimario *primario;
	VTSecundario *secundario;
	TList *lisSEC;
	TList *lisPRIMARIO = radial->LisPrimario();

	// reinicia lisPRI_VAL
	lisPRI_VAL->Clear();
	// identifica Primarios que podem ser ajustados
	for (int n = 0; n < lisPRIMARIO->Count; n++)
	{
		primario = (VTPrimario*)lisPRIMARIO->Items[n];
		// nao insere redes equivalentadas
		if (!primario->Rede->Carregada)
			continue;
		// n�o insere rede com redes supridas equivalentadas
		if (ExisteVizinhaEquivalentada(primario))
			continue;
		lisPRI_VAL->Add(primario);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TMedicoes::PreencheLisRedeVal(void)
 {
 //variaveis locais
 bool           sub_valida = true;
 VTRadial       *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
 VTPrimario     *primario;
 VTSubestacao   *subestacao;
 TList          *lisSUB      = radial->LisSubestacao();
 TList          *lisPRIMARIO;

 //reinicia lisREDE_VAL
 lisREDE_VAL->Clear();
 //identifica Redes que podem ser ajustadas
 for (int n = 0; n < lisSUB->Count; n++)
 {
 subestacao = (VTSubestacao*)lisSUB->Items[n];
 sub_valida = true;
 //nao insere redes equivalentadas
 if (! subestacao->Rede->Carregada)            continue;
 lisPRIMARIO = subestacao->LisPrimario();
 for(int np = 0; np < lisPRIMARIO->Count; np++)
 {//n�o insere rede com redes supridas equivalentadas
 primario = (VTPrimario*)lisPRIMARIO->Items[np];
 if ((! primario->Rede->Carregada) || (ExisteVizinhaEquivalentada(primario)))
 {
 sub_valida = false;
 continue;
 }
 //insere rede do primario como v�lida
 lisREDE_VAL->Add(primario->Rede);
 }
 if(sub_valida)
 {//insere rede da subesta��o como v�lida
 lisREDE_VAL->Add(subestacao->Rede);
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TMedicoes::PreencheLisRedeVal(void)
{ // DVK 2016.06.30 alterado para n�o utilizar radial: se n�o houver SED, mas
	// houver redes SDMT, as redes SDMT podem entrar na lista
	// variaveis locais
	VTRede *redeMT;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTTipoRede *tipo_rede_MT = tipos->ExisteTipoRede(redePRI);
	TList *lisREDE = redes->LisRede();

	// reinicia lisREDE_VAL
	lisREDE_VAL->Clear();
	// inicia arvore de redes
	if (!arvore->Inicia(redes))
		return;
	// identifica Redes que podem ser ajustadas
	for (int n = 0; n < lisREDE->Count; n++)
	{
		redeMT = (VTRede*)lisREDE->Items[n];
		if (redeMT->TipoRede != tipo_rede_MT)
			continue;
		// nao insere redes equivalentadas
		if (!redeMT->Carregada)
			continue;
		// n�o insere rede com redes supridas equivalentadas
		if (ExisteVizinhaEquivalentada(redeMT))
			continue;
		// n�o insere rede com fluxo zerado em todos patamares
		if (FluxoZerado(redeMT))
			continue;
		// insere rede MT como v�lida
		lisREDE_VAL->Add(redeMT);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::PreencheLisRedeVal(TList *lisREDE)
{
	PreencheLisRedeVal();
	CopiaTList(lisREDE_VAL, lisREDE);
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::RemoveChavesFic(void)
{
	// prote��o
	if (!pontos_med)
		return;
	// remove todas as chaves ficticias, exceto disjuntores
	pontos_med->RemoveChaveFicticia();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17882048
bool __fastcall TMedicoes::Valida(bool limite_ajuste, double ajuste_max)
{
    // lista medidores da rede
    Clear();
    // prote��o: verifica se h� medi��es  //DVK 2016.03.14 comentado
    // if (lisMED_ALL->Count == 0) return(true);
    //
    PreencheLisMedidor(); // lisMEDIDOR contem VTMedidor
    // verifica se os valores s�o nulos entre medi��o e fluxo
    ValidaMedidores(); // lisMED_SEL contem VTMedidor
    // cria medicao a partir de cada medidor
    CriaMedicoesMedidores(); // lisMED_ALL contem VTMedicao
    // valida diferen�a entre medi��o e fluxo

    // se n�o h� limite para o ajuste => n�o h� que validar as medi��es
    if (!limite_ajuste)
        return true;

    ValidaMedicoes(ajuste_max);
    return (lisMED_ALL->Count > 0);
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ValidaMedicoes(double ajuste_max)
{ // DVK 2016.05.31
	// vari�veis locais
	VTCanal *canal;
	VTMedicao *medicao;
	VTMedidor *medidor;
	TList *lisCANAL;

	for (int nm = 0; nm < lisMED_ALL->Count; nm++)
	{
		medicao = (VTMedicao*)lisMED_ALL->Items[nm];
		// verifica se � maior que o limite
		if (medicao->erro > ajuste_max)
		{ // insere diagn�stico de medi��o com erro inicial inv�lido
			medicao->diagnostico = diagMED_ERRO_INICIAL;
			medicao->enabled = false;
		}
		else
			medicao->enabled = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMedicoes::ValidaMedidores(void)
{
	// vari�veis locais
	VTCanal *canal;
	VTMedicao *medicao;
	VTMedidor *medidor;
	TList *lisCANAL;
	double maior_erro;

	// limpa lista
	lisMED_SEL->Clear();
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		lisCANAL = medidor->LisCanal();
		maior_erro = 0.;
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // verifica o maior valor do erro entre medido e estimado
			canal = (VTCanal*)lisCANAL->Items[nc];
			maior_erro = Max(maior_erro, MaiorErro(canal));
		}
		// verifica se o erro � nulo
		if (IsDoubleZero(maior_erro))
			continue;
		lisMED_SEL->Add(medidor);
	}
}

// ---------------------------------------------------------------------------
// eof
