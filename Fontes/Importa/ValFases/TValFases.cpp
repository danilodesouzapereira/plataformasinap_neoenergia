// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Filectrl.hpp>
#include "TValFases.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\..\Arranjo\VTCabo.h"
#include "..\..\Arranjo\VTPtoFixacao.h"
#include "..\..\Arranjo\VTSuporte.h"
#include "..\..\Log\VTLog.h"
#include "..\..\MontaZ\VTMontaZ.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTEqpto.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\..\DLL_Inc\Arranjo.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Rede.h"

// ---------------------------------------------------------------------------
VTValFases* __fastcall NewObjValFases(VTApl *apl)
{
	return (new TValFases(apl));
}

// ---------------------------------------------------------------------------
__fastcall TValFases::TValFases(VTApl *apl)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	fases = (VTFases*)apl->GetObject(__classid(VTFases));
	// cria objetos
	lisBAR = new TList();
	lisEQP = new TList();
	lisLIG = new TList();
	lisTRF = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TValFases::~TValFases(void)
{
	// destrói listas sem destruir seus objetos
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
	if (lisLIG)
	{
		delete lisLIG;
		lisLIG = NULL;
	}
	if (lisTRF)
	{
		delete lisTRF;
		lisTRF = NULL;
	}
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TValFases::BarraComum(VTLigacao *pai, VTLigacao *filha)
{
	// proteção
	if ((pai == NULL) || (filha == NULL))
		return (NULL);
	// pode haver chaves entre as Ligacoes pai e filha
	while ((filha != NULL) && (filha->ligaPai != pai))
	{
		filha = filha->ligaPai;
	}
	// protecao
	if (filha == NULL)
		return (NULL);
	// retorna Barra comum entre filha e pai
	return (filha->BarraComum(pai));
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesCarga(VTCarga *carga, int fases_ref)
{
	// variáveis locais
	int fases_orig = carga->Fases;

	// verifica se as fases disponíveis atendem à Carga
	if ((fases_ref & carga->Fases) != carga->Fases)
	{ // redefine fases da Carga igual às fases disponíveis (sem faseT)
		carga->Fases = fases_ref & faseABCN;
		if (fases->NumeroDeFases(carga->Fases) == 1)
		{ // DVK 2015.11.09 verifica se não possui neutro (para incluir terra)
			if ((carga->Fases & faseN) != faseN)
				carga->Fases |= faseT;
		}
		// atualiza Status
		carga->Status[sttERROGIS] = true;
		// atualiza log
		if (plog)
			plog->DefineValor("Eqpto", carga->TipoAsString());
		if (plog)
			plog->DefineValor("Código", carga->Codigo);
		if (plog)
			plog->DefineValor("Rede", ""); // DVK 2014.08.07
		if (plog)
			plog->DefineValor("ID", carga->Extern_id);
		if (plog)
			plog->Add("Fases alteradas de " + fases->AsStrABCN[fases_orig] + " para " +
			fases->AsStrABCN[carga->Fases]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesRegulador(VTTrecho *trecho_pai, VTRegulador *regulador)
{
	/*
	 //variáveis locais
	 int      ind_bar = 0;
	 int      fases_pai, fases_filha, fases_nt;
	 VTBarra *barra;

	 //determina Barra comum entre Trecho e Regulador
	 if ((barra = regulador->BarraComum(trecho_pai)) == NULL) return;
	 ind_bar = regulador->IndiceBarra(barra);
	 //verifica se o Trecho pai possui todas as fases do Regulador
	 fases_pai = trecho_pai->arranjo->Fases;
	 fases_filha = regulador->Fases(ind_bar);
	 if ((fases_filha & fases_pai) != fases_filha)
	 {//Regulador com fases incorretas:assume fases do Trecho
	 fases_filha = fases_pai;
	 //redefine tipo de ligação em função das fases
	 }
	 */
}

// ---------------------------------------------------------------------------
// DVK 2015.08.11 reescrito: não muda trafos de trifásico p/ monofásico
void __fastcall TValFases::CorrigeFasesTrafo(VTTrecho *trecho_pai, VTTrafo *trafo)
{
	// variáveis locais
	struct
	{
		int fases_pri, fases_sec;
	} antes, depois;
	int fases_pai, fases_filha, fases_nt;
	VTBarra *bar_ref_trafo, *bar_ref_trecho;

	// atualiza log
	if (plog)
		plog->DefineValor("Tabela", "");
	if (plog)
		plog->DefineValor("Eqpto", trafo->TipoAsString());
	if (plog)
		plog->DefineValor("Código", trafo->Codigo);
	if (plog)
		plog->DefineValor("Rede", trafo->rede->Codigo);
	if (plog)
		plog->DefineValor("ID", trafo->Extern_id);
	// salva fases originais do trafo
	antes.fases_pri = trafo->Fases(trafo->BarraPrimario());
	antes.fases_sec = trafo->Fases(trafo->BarraSecundario());
	// CUIDADO: pode haver outras Ligações entre o Trafo e o Trecho pai
	bar_ref_trafo = trafo->BarraComum(trafo->ligaPai);
	bar_ref_trecho = BarraComum(trecho_pai, trafo);
	// proteção
	if ((bar_ref_trafo == NULL) || (bar_ref_trecho == NULL))
		return;
	// determina fases do trecho pai, excluindo neutro e terra
	fases_pai = trecho_pai->arranjo->Fases & faseABC;
	// determina fases do Trafo (enrolamento conectado pela Barra)
	fases_filha = trafo->Fases(bar_ref_trafo);
	// salva informação de neutro e terra do trafo
	fases_nt = fases_filha & (faseN | faseT);
	// elimina neutro e terra das fases do trafo
	fases_filha &= faseABC;
	// verifica se as fases do Trafo são compatíveis com as fases da Ligacao pai
	// if ((fases_filha & fases_pai) != fases_pai)
	if ((fases_filha & fases_pai) != fases_filha)
		// {//trafo com fases incorretas
		// if (fases->NumeroDeFases(fases_filha) <= fases->NumeroDeFases(fases_pai))
		// {//trafo com apenas 1 fase: assume uma das fases pai
		// if      ((fases_filha == faseB) && (fases_pai & faseC)) fases_filha = faseC;
		// else if ((fases_filha == faseC) && (fases_pai & faseB)) fases_filha = faseB;
		// else if ((fases_pai & faseA) == faseA)  fases_filha = faseA;
		// else if ((fases_pai & faseB) == faseB)  fases_filha = faseB;
		// else if ((fases_pai & faseC) == faseC)  fases_filha = faseC;
		// }
	{ // trafo com fases incorretas
		// DVK 2016.05.23 validação depende do número de fases
		if (fases->NumeroDeFases(fases_filha) == fases->NumeroDeFases(fases_pai))
		{ // assume mesmas fases do pai
			fases_filha = fases_pai;
		}
		if ((fases->NumeroDeFases(fases_filha) == 2) && (fases->NumeroDeFases(fases_pai) == 1))
		{ // assume mesmas fases do pai
			fases_filha = fases_pai;
		}
		if ((fases->NumeroDeFases(fases_filha) == 1) && (fases->NumeroDeFases(fases_pai) == 2))
		{ // trafo com apenas 1 fase: assume uma das fases pai
			if ((fases_filha == faseA) && (fases_pai & faseB))
				fases_filha = faseB;
			else if ((fases_filha == faseB) && (fases_pai & faseC))
				fases_filha = faseC;
			else if ((fases_filha == faseC) && (fases_pai & faseA))
				fases_filha = faseA;
			else if ((fases_pai & faseA) == faseA)
				fases_filha = faseA;
			else if ((fases_pai & faseB) == faseB)
				fases_filha = faseB;
			else if ((fases_pai & faseC) == faseC)
				fases_filha = faseC;
		}
		// atualiza fases do Trafos
		if (bar_ref_trafo == trafo->BarraPrimario())
			trafo->pri.fase = fases_filha | fases_nt;
		else
			trafo->sec.fase = fases_filha | fases_nt;
		// atualiza Status
		trafo->Status[sttERROGIS] = true;
		// salva fases depois da alteração
		depois.fases_pri = trafo->Fases(trafo->BarraPrimario());
		depois.fases_sec = trafo->Fases(trafo->BarraSecundario());
		// DVK 2014.10.14 altera mensagem no log
		if ((antes.fases_pri == depois.fases_pri) && (antes.fases_sec == depois.fases_sec))
		{
			if (plog)
				plog->Add("Fases do trafo " + fases->AsStrABCN[antes.fases_pri] + "/" +
				fases->AsStrABCN[antes.fases_sec] + " incompatíveis com fases da rede " +
				fases->AsStrABCN[fases_pai]);
		}
		else
		{
			if (plog)
				plog->Add("Fases alteradas de " + fases->AsStrABCN[antes.fases_pri] + "/" +
				fases->AsStrABCN[antes.fases_sec] + " para " + fases->AsStrABCN[depois.fases_pri] +
				"/" + fases->AsStrABCN[depois.fases_sec]);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrafoMonoEmDelta(VTTrecho *trecho_pai, TList *lisTRF)
{
	// variáveis locais
	int num_trafo;
	int fases_disp, fases_trf;
	VTBarra *bar_pri, *bar_sec;
	VTTrafo *trafo1, *trafo2;

	// proteção
	if ((num_trafo = lisTRF->Count) <= 1)
		return;
	// ordena Trafos em lisTRF para que o Trafo com neutro no secundário seja colocado no início da lista
	for (int n = 1; n < lisTRF->Count; n++)
	{
		trafo1 = (VTTrafo*)lisTRF->Items[n];
		if ((trafo1->sec.fase & faseN) == faseN)
		{ // transfere Trafo para início da lista
			trafo2 = (VTTrafo*)lisTRF->Items[0];
			lisTRF->Items[0] = trafo1;
			lisTRF->Items[n] = trafo2;
			break;
		}
	}
	// determina fases disponíveis para conectar os Trafos
	fases_disp = trecho_pai->arranjo->Fases & faseABC;
	// loop para identificar Trafos com fase correta
	for (int n = lisTRF->Count - 1; n >= 0; n--)
	{
		trafo1 = (VTTrafo*)lisTRF->Items[n];
		fases_trf = trafo1->pri.fase & faseABC;
		// verifica se a fase do Trafo está disponível
		if ((fases_trf & fases_disp) == fases_trf)
		{ // retira Trafo de lisTRF e não altera sua fase
			lisTRF->Delete(n);
			// elimina a fase do Trafo das fases disponíveis
			fases_disp ^= fases_trf;
		}
	}
	// loop para corrigir Trafos com fase errada
	for (int n = 0; n < lisTRF->Count; n++)
	{
		trafo1 = (VTTrafo*)lisTRF->Items[n];
		// identifica uma fase disponível para conectar o Trafo
		if ((fases_disp & faseA) == faseA)
			fases_trf = faseA;
		else if ((fases_disp & faseB) == faseB)
			fases_trf = faseB;
		else if ((fases_disp & faseC) == faseC)
			fases_trf = faseC;
		else
			fases_trf = faseINV;
		// verifica se encontrou uma fase disponível
		if (fases_trf == faseINV)
		{ // não há fase para ligar o Trafo
			if (plog)
				plog->Add
					("Não há fase da rede primária p/ conectar trafo monofásico em delta: ET " +
				trafo1->rede->Codigo);
			// Não remove Trafo da rede. O fluxo não vai rodar mas a ET permanece em delta
			// trafo1->rede->RemoveLigacao(trafo1);
			// DLL_DeleteEqpto(trafo1);
		}
		else
		{ // elimina a fase selecionada das fases disponíveis
			fases_disp ^= fases_trf;
			// inclui neutro na fase selecionado
			fases_trf |= faseN;
			// inclui faseT somente se Trafo ligado em delta aberto
			if (num_trafo != 3)
				fases_trf |= faseT;
			// altera fases do primário  do Trafo
			trafo1->pri.fase = fases_trf;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrafoMTBT(VTPrimario *primario)
{
	// variáveis locais
	VTLigacao *ligacao;
	VTTrafo *trafo;
	VTTrecho *trecho;
	int fases_trecho, fases_trafo;
	TList *lisTRF = new TList();

    fases_trecho = fases_trafo = 0;
	lisTRF->Clear();
	primario->Rede->LisLigacao(lisTRF, eqptoTRAFO);
	if (lisTRF->Count == 0)
		return;
	for (int nt = 0; nt < lisTRF->Count; nt++)
	{ // determina trafo MT/BT
		trafo = (VTTrafo*)lisTRF->Items[nt];
		ligacao = trafo->ligaPai;
		while ((ligacao != NULL) && (ligacao->Tipo() != eqptoTRECHO))
		{ // determina Ligacao pai
			ligacao = ligacao->ligaPai;
		}
		// proteção
		if ((ligacao == NULL) || (ligacao->Tipo() != eqptoTRECHO))
			return;
		// encontrou Ligacao pai que é Trecho
		trecho = (VTTrecho*)ligacao;
		fases_trafo = trafo->pri.fase & faseABC;
		fases_trecho = trecho->arranjo->Fases & faseABC;
		if ((fases_trecho & fases_trafo) != fases_trafo)
		{
			CorrigeFasesTrafo(trecho, trafo);
		}
	}
	// destroi lista
	if (lisTRF)
		delete lisTRF;
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrafoSecundario(VTSecundario *secundario)
{
	// variáveis locais
	VTBarra *barra;
	VTLigacao *ligacao;
	VTTrafo *trafo;
	VTTrecho *trecho;

	// determina Barra inicial so Secundario
	if ((barra = secundario->BarraInicial) == NULL)
		return;
	// verifica se a Barra inicial tem informação de sua Ligacao pai
	if (barra->Obj == NULL)
		return;
	// determina Ligacao pai da Barra inicial do Secundario que seja Trecho
	ligacao = (VTLigacao*)barra->Obj;
	while ((ligacao != NULL) && (ligacao->Tipo() != eqptoTRECHO))
	{ // determina Ligacao pai
		ligacao = ligacao->ligaPai;
	}
	// proteção
	if ((ligacao == NULL) || (ligacao->Tipo() != eqptoTRECHO))
		return;
	// encontrou Ligacao pai que é Trecho
	trecho = (VTTrecho*)ligacao;
	// inicia lisTRF com os Trafos dao Secundario
	lisTRF->Clear();
	secundario->Rede->LisLigacao(lisTRF, eqptoTRAFO);
	// verifica número de Trafos
	if (lisTRF->Count == 0)
		return;
	if (lisTRF->Count == 1)
	{ // corrige fases do Trafo
		trafo = (VTTrafo*)lisTRF->First();
		CorrigeFasesTrafo(trecho, trafo);
	}
	else
	{ // verifica se todos Trafos são monofásicos conectado nas mesmas Barras
		if (VerificaTrafosMonoEmDelta(lisTRF))
		{ // corrige fases de Trafos ligados em delta
			CorrigeFasesTrafoMonoEmDelta(trecho, lisTRF);
		}
		else
		{ // corrige fases de cada Trafo
			for (int n = 0; n < lisTRF->Count; n++)
			{
				trafo = (VTTrafo*)lisTRF->Items[n];
				CorrigeFasesTrafo(trecho, trafo);
			}
		}
	}
}

// valida fases dos trechos em relação aos reguladores
// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrechoPai(VTTrecho *pai, VTRegulador *regulador)
{
	// variáveis locais
	int fases_pai = pai->arranjo->Fases;
	int fases_filho = regulador->fases & faseABC;
	int fases_orig = pai->arranjo->Fases;

#ifdef _DEBUG
	if (fases_pai == 0)
	{
		int a = 0;
	}
	if (fases_filho == 0)
	{
		int a = 0;
	}
	if (fases_orig == 0)
	{
		int a = 0;
	}
#endif
	// verifica se o Trecho pai não contém todas as fases do filho
	if ((fases_pai & fases_filho) != fases_filho)
	{ // verifica se o filho contêm todas fases do Trecho pai
		pai->Status[sttERROGIS] = true;
		RedefineArranjoPaiFasesFilho(pai, fases_filho);
		if (plog)
			plog->DefineValor("Rede", pai->rede->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", pai->TipoAsString());
		if (plog)
			plog->DefineValor("Código", pai->Codigo);
		if (plog)
			plog->DefineValor("ID", pai->Extern_id);
		if (plog)
			plog->Add("Fases alteradas de " + fases->AsStrABCN[fases_orig] + " para " +
			fases->AsStrABCN[pai->arranjo->Fases]);
	}
}

// DVK 2017.02.24 valida fases dos trechos em relação aos trafos
// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrechoPai(VTTrecho *pai, VTTrafo *trafo)
{
	// variáveis locais
	int fases_pai = pai->arranjo->Fases;
	int fases_filho = trafo->pri.fase & faseABC;
	int fases_orig = pai->arranjo->Fases;

#ifdef _DEBUG
	if (fases_pai == 0)
	{
		int a = 0;
	}
	if (fases_filho == 0)
	{
		int a = 0;
	}
	if (fases_orig == 0)
	{
		int a = 0;
	}
#endif
	// verifica se o Trecho pai não contém todas as fases do filho
	if ((fases_pai & fases_filho) != fases_filho)
	{ // verifica se o filho contêm todas fases do Trecho pai
		pai->Status[sttERROGIS] = true;
		RedefineArranjoPaiFasesFilho(pai, fases_filho);
		if (plog)
			plog->DefineValor("Rede", pai->rede->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", pai->TipoAsString());
		if (plog)
			plog->DefineValor("Código", pai->Codigo);
		if (plog)
			plog->DefineValor("ID", pai->Extern_id);
		if (plog)
			plog->Add("Fases alteradas de " + fases->AsStrABCN[fases_orig] + " para " +
			fases->AsStrABCN[pai->arranjo->Fases]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrechoPai(VTTrecho *pai, VTTrecho *filho)
{
	// variáveis locais
	int fases_pai = pai->arranjo->Fases;
	int fases_filho = filho->arranjo->Fases;
	int fases_orig = pai->arranjo->Fases;
	int fase_sem_neutro;

#ifdef _DEBUG
	if (fases_pai == 0)
	{
		int a = 0;
	}
	if (fases_filho == 0)
	{
		int a = 0;
	}
	if (fases_orig == 0)
	{
		int a = 0;
	}
#endif
	// verifica se o pai possui apenas o neutro
	if (pai->arranjo->Fases == faseN)
	{
		RedefineArranjoPaiIgualArranjoFilho(pai, filho);
	}
	// verifica se o Trecho pai não contém todas as fases do Trecho filho
	if ((fases_pai & fases_filho) != fases_filho)
	{ // verifica se o Trecho filho contêm todas fases do Trecho pai
		// if ((fases_filho & fases_pai) == fases_pai)
		// {
		pai->Status[sttERROGIS] = true;
		// verifica se é só problema de neutro (não sendo, copia arranjo)
		fase_sem_neutro = filho->arranjo->Fases & faseABC;
		if (fases_pai == fase_sem_neutro)
			RedefineArranjoComNeutro(pai);
		// else                              RedefineArranjoPaiIgualArranjoFilho(pai, filho);
		// DVK 2017.02.24 insere fases do filho no arranjo do pai
		else
			RedefineArranjoPaiFasesFilho(pai, fases_filho);
		if (plog)
			plog->DefineValor("Rede", pai->rede->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", pai->TipoAsString());
		if (plog)
			plog->DefineValor("Código", pai->Codigo);
		if (plog)
			plog->DefineValor("ID", pai->Extern_id);
		if (plog)
			plog->Add("Fases alteradas de " + fases->AsStrABCN[fases_orig] + " para " +
			fases->AsStrABCN[pai->arranjo->Fases]);
		// }
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeFasesTrechoFilho(VTTrecho *pai, VTTrecho *filho)
{
	// variáveis locais
	int fases_pai = pai->arranjo->Fases;
	int fases_filho = filho->arranjo->Fases;
	int fases_orig = filho->arranjo->Fases;

#ifdef _DEBUG
	if (fases_pai == 0)
	{
		int a = 0;
	}
	if (fases_filho == 0)
	{
		int a = 0;
	}
	if (fases_orig == 0)
	{
		int a = 0;
	}
#endif
	// verifica se o filho possui apenas o neutro
	if (filho->arranjo->Fases == faseN)
	{
		RedefineArranjoFilhoIgualArranjoPai(pai, filho);
	}
	// verifica se há descontinuidade do neutro
	else if (((fases_filho & faseN) == faseN) && ((fases_pai & faseN) != faseN))
	{
		RedefineArranjoSemNeutro(filho);
	}
	// verifica se as fases do Trecho pai contêm as fases do Trecho filho
	if ((pai->arranjo->Fases & filho->arranjo->Fases) != filho->arranjo->Fases)
	{ // DVK 2017.02.24 insere fases do filho no arranjo do pai
		RedefineArranjoFilhoIgualArranjoPai(pai, filho);
		// RedefineArranjoPaiFasesFilho(pai, fases_filho);
	}
	// atualiza log
	fases_filho = filho->arranjo->Fases;
	if (fases_filho == fases_orig)
		return;
	// atualiza Status
	filho->Status[sttERROGIS] = true;
	// atualiza log
	if (plog)
		plog->DefineValor("Eqpto", filho->TipoAsString());
	if (plog)
		plog->DefineValor("Código", filho->Codigo);
	if (plog)
		plog->DefineValor("Rede", filho->rede->Codigo);
	if (plog)
		plog->DefineValor("ID", filho->Extern_id);
	if (plog)
		plog->Add("Fases alteradas de " + fases->AsStrABCN[fases_orig] + " para " +
		fases->AsStrABCN[filho->arranjo->Fases]);
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeTrechoFaseINV(VTTrecho *trecho)
{
	// variáveis locais
	VTLigacao *pai;
	VTTrecho *trecho_aux;
	int fases_pai;

	// protecao
	if (trecho->arranjo == NULL)
	{
		return;
	}
	if ((trecho->arranjo->Fases == faseN) || (trecho->arranjo->Fases == faseINV))
	{ // corrige se só tiver neutro ou se a fase é invalida
		// determina Ligacao pai
		pai = ExistePaiValido((VTLigacao*)trecho);
		// salva as fases disponiveis do pai
		if (pai != NULL)
		{
			fases_pai = FasesDisponiveis(pai);
			if ((fases_pai != faseINV) && (fases_pai != faseN))
			{
				RedefineArranjo(trecho, fases_pai);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeTrechoMono(VTTrecho *pai, VTTrecho *trecho)
{ // DVK 2014.08.06
	// variáveis locais
	int fases_trecho = trecho->arranjo->Fases;
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTCabo *cabo;
	VTSuporte *suporte;

	// verifica se é arranjo tipico
	if (trecho->ArranjoTipico)
	{ // verifica qual fase tem o trecho
		suporte = trecho->arranjo->suporte;
		switch (fases_trecho & faseABC)
		{
		case faseA:
			cabo = trecho->arranjo->fase[indFASE_A].cabo;
			arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, cabo, NULL, NULL, NULL);
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
			break;
		case faseB:
			cabo = trecho->arranjo->fase[indFASE_B].cabo;
			arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, NULL, cabo, NULL, NULL);
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
			break;
		case faseC:
			cabo = trecho->arranjo->fase[indFASE_C].cabo;
			arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, NULL, NULL, cabo, NULL);
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
			break;
		default:
			break;
		}
		// se não tiver um arranjo sem neutro, copia arranjo do trecho pai
		if (!arranjo)
			RedefineArranjoFilhoIgualArranjoPai(pai, trecho);
	}
	else
	{
		RedefineArranjoFilhoIgualArranjoPai(pai, trecho);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeTrechoBi(VTTrecho *pai, VTTrecho *trecho)
{ // DVK 2014.08.06
	// variáveis locais
	int fases_trecho = trecho->arranjo->Fases;
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTCabo *cabo;
	VTSuporte *suporte;

	// verifica se é arranjo tipico
	if (trecho->ArranjoTipico)
	{ // verifica qual fase tem o trecho
		suporte = trecho->arranjo->suporte;
		switch (fases_trecho & faseABC)
		{
		case faseAB:
			cabo = trecho->arranjo->fase[indFASE_A].cabo;
			arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, cabo, cabo, NULL, NULL);
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
			break;
		case faseBC:
			cabo = trecho->arranjo->fase[indFASE_B].cabo;
			arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, NULL, cabo, cabo, NULL);
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
			break;
		case faseCA:
			cabo = trecho->arranjo->fase[indFASE_C].cabo;
			arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, cabo, NULL, cabo, NULL);
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
			break;
		default:
			break;
		}
		// se não tiver um arranjo sem neutro, copia arranjo do trecho pai
		if (!arranjo)
			RedefineArranjoFilhoIgualArranjoPai(pai, trecho);
	}
	else
		RedefineArranjoFilhoIgualArranjoPai(pai, trecho);
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::CorrigeTrechoTri(VTTrecho *pai, VTTrecho *trecho)
{ // DVK 2014.08.06
	// variáveis locais
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTCabo *cabo;
	VTSuporte *suporte;

	// verifica se é arranjo tipico
	if (trecho->ArranjoTipico)
	{ // se não tiver um arranjo sem neutro, copia arranjo do trecho pai
		suporte = trecho->arranjo->suporte;
		cabo = trecho->arranjo->fase[indFASE_A].cabo;
		arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, cabo, cabo, cabo, NULL);
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		else
			RedefineArranjoFilhoIgualArranjoPai(pai, trecho);
	}
	else
		RedefineArranjoFilhoIgualArranjoPai(pai, trecho);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TValFases::Executa(void)
 {
 //variáveis locais
 TList        *lisSEC, *lisPRI;
 VTPrimario   *primario;
 VTSecundario *secundario;
 VTRadial     *radial    = (VTRadial*)apl->GetObject(__classid(VTRadial));
 VTProgresso  *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

 //obtém objeto VTLog
 plog = (VTLog*)apl->GetObject(__classid(VTLog));
 //cancela informação de arquivo e rede
 if (plog) plog->DefineValor("Arquivo",  "");
 if (plog) plog->DefineValor("Rede",     "");
 //reinicia lisEQP que será preenchida com os eqptos com fases inválidas
 lisEQP->Clear();
 //obtem lista de redes primárias
 lisPRI = radial->LisPrimario();
 //obtém lista de redes secundárias
 lisSEC = radial->LisSecundario();
 //exibe progresso
 if (progresso) progresso->Add("Verificando fases das redes primárias");
 //loop p/todos primarios
 for(int npri = 0; npri < lisPRI->Count; npri++)
 {
 primario = (VTPrimario*) lisPRI->Items[npri];
 //if (progresso) progresso->Add(primario->Rede->Codigo);
 VerificaFases(primario);
 }
 //exibe progresso
 if (progresso) progresso->Add("Verificando fases das redes secundárias");
 //loop p/ todos Secundario
 for (int n = 0; n < lisSEC->Count; n++)
 {
 secundario = (VTSecundario*)lisSEC->Items[n];

 //#ifdef _DEBUG
 //if (secundario->Rede->Codigo.AnsiCompareIC("045ET000804899") == 0)
 //{
 //int a=0;
 //}
 //#endif

 //if (progresso) progresso->Add(secundario->Rede->Codigo);
 VerificaFases(secundario);
 }
 //esconde o progresso
 //if (progresso) progresso->Stop();
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TValFases::Executa(void)
{ // DVK 2017.05.17 sequência invertida: primeiro verifica fases dos secundários,
	// depois dos primários
	// variáveis locais
	TList *lisSEC, *lisPRI;
	VTPrimario *primario;
	VTSecundario *secundario;
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	// obtém objeto VTLog
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	// cancela informação de arquivo e rede
	if (plog)
		plog->DefineValor("Arquivo", "");
	if (plog)
		plog->DefineValor("Rede", "");
	// reinicia lisEQP que será preenchida com os eqptos com fases inválidas
	lisEQP->Clear();
	// obtem lista de redes primárias
	lisPRI = radial->LisPrimario();
	// obtém lista de redes secundárias
	lisSEC = radial->LisSecundario();
	// exibe progresso
	if (progresso)
		progresso->Add("Verificando fases das redes secundárias");
	// FKM 2017.05.26
	// caso nao esteja habilitado a correção de fases, substitui fases
	// invalidas por  fase padrao
	if (this->CorrigeFases == false)
	{
		SubstituiTrechosFasesInvalidas(faseABC);
	}
	// loop p/todos primarios apenas para guardar ligaPai dos trafos MT/BT
	for (int npri = 0; npri < lisPRI->Count; npri++)
	{ // obtém lista ordenada de barras e ligações do primário
		primario = (VTPrimario*) lisPRI->Items[npri];
		primario->Ordena(lisBAR, lisLIG);
	}
	// loop p/ todos Secundario
	for (int n = 0; n < lisSEC->Count; n++)
	{
		secundario = (VTSecundario*)lisSEC->Items[n];
		// if (progresso) progresso->Add(secundario->Rede->Codigo);
		VerificaFases(secundario);
	}
	// exibe progresso
	if (progresso)
		progresso->Add("Verificando fases das redes primárias");
	// loop p/todos primarios
	for (int npri = 0; npri < lisPRI->Count; npri++)
	{
		primario = (VTPrimario*) lisPRI->Items[npri];
		// if (progresso) progresso->Add(primario->Rede->Codigo);
		VerificaFases(primario);
		// obtém lista ordenada de barras e ligações do primário
		primario->Ordena(lisBAR, lisLIG);
	}
	// esconde o progresso
	// if (progresso) progresso->Stop();
	return (true);
}

// ---------------------------------------------------------------------------
int __fastcall TValFases::ExisteEqptoComFaseInvalida(void)
{
	return (lisEQP->Count);
}

// ---------------------------------------------------------------------------
VTTrecho* __fastcall TValFases::ExisteTrechoPai(VTLigacao *filha, bool aceita_faseINV)
{
	// variáveis locais
	VTLigacao *pai = filha->ligaPai;
	VTTrecho *trecho;

	// determina Ligacao pai que seja um Trecho
	while (pai != NULL)
	{ // não considera Ligacao pai que é Chave
		if (pai->Tipo() == eqptoTRECHO)
		{
			trecho = (VTTrecho*)pai;
			// caso tenha fase invalida, segue buscando
			if (!aceita_faseINV)
			{
				if (trecho->arranjo != NULL)
				{
					if (trecho->arranjo->Fases != 0)
					{
						return ((VTTrecho*)pai);
					}
				}
			}
			else
			{
				return ((VTTrecho*)pai);
			}
		}
		// interrompe a busca caso a Ligação pai seja Trafo
		if (pai->Tipo() == eqptoTRAFO)
			return (NULL);
		// if (pai->Tipo() == eqptoREGULADOR) return(NULL); //DVK 2017.02.14 comentado

		// determina próxima Ligacao pai
		pai = pai->ligaPai;
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TValFases::ExistePaiValido(VTLigacao *filha)
{ // variáveis locais
	VTLigacao *pai = filha->ligaPai;
	VTTrecho *trecho;

	// determina Ligacao pai que seja valida
	while (pai != NULL)
	{ // só considera Ligacao pai que é Trecho, Trafo ou Regulador
		if (pai->Tipo() == eqptoTRECHO)
		{
			trecho = (VTTrecho*)pai;
			if (trecho->arranjo != NULL)
			{
				if ((trecho->arranjo->Fases != faseINV) && (trecho->arranjo->Fases != faseN))
				{
					return (pai);
				}
			}
		}
		if (pai->Tipo() == eqptoTRAFO)
			return (pai);
		if (pai->Tipo() == eqptoREGULADOR)
			return (pai);
		// determina próxima Ligacao pai
		pai = pai->ligaPai;
	}

	return (NULL);
}

// ---------------------------------------------------------------------------
int __fastcall TValFases::FasesDisponiveis(VTBarra *barra)
{
	TList *lisSuprimento = new TList;
	int fasesDisp = 0;
	VTSuprimento *sup;

	barra->LisEqbar(lisSuprimento, eqptoSUPRIMENTO);
	for (int ns = 0; ns < lisSuprimento->Count; ns++)
	{
		sup = (VTSuprimento*)lisSuprimento->Items[ns];
		fasesDisp |= sup->Fases;
	}

	DestroiTObject(lisSuprimento);

	return fasesDisp;
}

// ---------------------------------------------------------------------------
int __fastcall TValFases::FasesDisponiveis(VTLigacao *ligacao, VTBarra *barraSecTerc)
{ // variaveis locais
	int fasesDisp = 0;
	int tipoLig;
	VTBarra *barra;
	VTLigacao *ligacaoAux;
	VTRegulador *regulador;
	VTTrafo *trafo;
	VTTrecho *trecho;

	if (ligacao == NULL)
		return (faseINV);
	// verifica qual o tipo de ligacao
	tipoLig = ligacao->Tipo();
	// verifica as fases disponiveis de acordo com o tipo de ligacao
	switch (tipoLig)
	{
	case eqptoCHAVE:
		ligacaoAux = ExistePaiValido(ligacao);
		if (ligacaoAux == NULL)
		{
			fasesDisp |= FasesDisponiveis(ligacao->pbarra1);
			fasesDisp |= FasesDisponiveis(ligacao->pbarra2);
		}
		else
		{
			fasesDisp = FasesDisponiveis(ligacaoAux);
		}
		break;
	case eqptoREGULADOR:
		regulador = (VTRegulador*) ligacao;
		// olha a barra controlada
		/* TODO : (presume não existir regulador reversivel) */
		barra = regulador->cfg_auto.bar_ref;
		// guarda as fases do secundario
		fasesDisp = ligacao->Fases(barra);
		break;
	case eqptoTRAFO:
		trafo = (VTTrafo*) ligacao;
		// ordena prim sec
		trafo->OrdenaBar12PriSec();
		// pega a barra do secundario
		barra = trafo->BarraSecundario();
		// guarda as fases do secundario
		fasesDisp = ligacao->Fases(barra);
		break;
	case eqptoTRAFO3E:
		if (barraSecTerc == NULL)
		{
			fasesDisp = faseINV;
			break;
		}
		else
		{ // guarda as fases do secundario
			fasesDisp = ligacao->Fases(barraSecTerc);
		}
		break;
	case eqptoTRECHO:
		trecho = (VTTrecho*) ligacao;
		fasesDisp = trecho->arranjo->Fases;
		break;
	default:
		fasesDisp = faseINV;
	}
	return fasesDisp;
}

// ---------------------------------------------------------------------------
int __fastcall TValFases::FasesDaLigacao(VTLigacao *ligacao, VTBarra *barra)
{
	// variáveis locais
	int fases_tag;
	VTTrafo *trafo;

	// determina fases da Ligacao
	fases_tag = ligacao->Fases(barra);
	// verifica se a Ligacao é um Trafo
	if (ligacao->Tipo() == eqptoTRAFO)
	{ // obtém lista dos Trafos que conectam a Barra
		LisTrafoConectaBarra(lisLIG, barra, lisTRF);
		// determina fases do conjunto de Trafos
		fases_tag = 0;
		for (int n = 0; n < lisTRF->Count; n++)
		{
			trafo = (VTTrafo*)lisTRF->Items[n];
			fases_tag |= trafo->Fases(barra);
		}
	}
	// verifica se a Ligacao é um trecho
	if (ligacao->Tipo() == eqptoTRECHO)
	{
		fases_tag = ((VTTrecho*)ligacao)->arranjo->Fases;
	}
	return (fases_tag);
}

// ---------------------------------------------------------------------------
TList* __fastcall TValFases::LisEqptoComFaseInvalida(void)
{
	return (lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::LisTrafoConectaBarra(TList *lisLIG, VTBarra *barra, TList *lisEXT)
{
	// variáveis locais
	VTLigacao *ligacao;

	// reinicia lisEXT
	lisEXT->Clear();
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoTRAFO)
		{
			if (ligacao->ConectaBarra(barra))
				lisEXT->Add(ligacao);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::RedefineArranjo(VTTrecho *trecho, int faseNOVA)
{ // variáveis locais
	int fases_trecho = faseNOVA;
	int fase_tag;
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTMontaZ *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
	VTCabo *vet_cabo[MAX_FASE];
	VTCabo *ult_cabo_fase;
	VTPtoFixacao *pto_fix;
	AnsiString cod_novo_arranjo = trecho->arranjo->Codigo + "*" + fases->AsStrABCN[fases_trecho];

	// verifica o tipo do Arranjo
	switch (trecho->arranjo->TipoCfg)
	{
	case arranjoCABOxFASE:
		// determina um cabo de fase não nulo
		for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
		{
			if (trecho->arranjo->fase[ind_fase].cabo == NULL)
				continue;
			ult_cabo_fase = trecho->arranjo->fase[ind_fase].cabo;
			break;
		}
		// proteção
		if (ult_cabo_fase == NULL)
			return;
		// monta vetor de cabos das fases do trecho pai com fases do trecho filho
		for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
		{
			vet_cabo[ind_fase] = trecho->arranjo->fase[ind_fase].cabo;
			fase_tag = fases->IndexToTag[ind_fase];
			if ((vet_cabo[ind_fase] == NULL) && (fases->Fase1ContemFase2(fases_trecho, fase_tag)))
			{ // assume cabo não nulo
				vet_cabo[ind_fase] = ult_cabo_fase;
			}
		}
		// verifica se existe o Arranjo para os cabos
		arranjo = arranjos->ExisteArranjoCABOxFASE(trecho->arranjo->suporte, vet_cabo[indFASE_A],
			vet_cabo[indFASE_B], vet_cabo[indFASE_C], vet_cabo[indNEUTRO]);
		if (arranjo == NULL)
		{ // DVK 2016.03.30 localiza arranjo pelo novo código
			arranjo = arranjos->ExisteArranjo(cod_novo_arranjo);
		}
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = trecho->arranjo->Clone();
			for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
			{
				if (vet_cabo[ind_fase] == NULL)
					continue;
				pto_fix = trecho->arranjo->suporte->ExistePtoFixacaoIndex(ind_fase);
				if (pto_fix == NULL)
					continue;
				// define Cabo p/ neutro do Arranjo
				arranjo->DefPtoFixacaoFase(ind_fase, pto_fix);
				arranjo->DefCaboFase(ind_fase, vet_cabo[ind_fase]);
			}
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
			// int na = arranjos->LisArranjo()->Count;
		}
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		break;
	case arranjoCABO_Z0Z1:
		arranjo = arranjos->ExisteArranjoCABO_Z0Z1(trecho->arranjo->caboZ0Z1, fases_trecho);
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = trecho->arranjo->Clone();
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
		}
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		break;
	case arranjoZ0Z1:
		if (trecho->ArranjoTipico)
		{
			arranjo = arranjos->ExisteArranjoZ0Z1(trecho->arranjo->z0.r, trecho->arranjo->z0.x,
				trecho->arranjo->c0, trecho->arranjo->z1.r, trecho->arranjo->z1.x,
				trecho->arranjo->c1, fases_trecho);
			if (arranjo == NULL)
			{ // cria um novo Arranjo típico
				arranjo = trecho->arranjo->Clone();
				arranjo->Fases = fases_trecho;
				arranjo->Id = FalsoId();
				arranjo->Codigo = cod_novo_arranjo;
				arranjo->Status[sttNOVO] = true;
				// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
				if (!montaZ->Executa(arranjo))
				{ // destrói arranjo
					delete arranjo;
					return;
				}
				arranjos->InsereArranjo(arranjo);
			}
			// redefine o Arranjo do Trecho
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
		}
		else
		{ // redefine fases do arranjo próprio
			trecho->arranjo->Fases = fases_trecho;
		}
		break;
	default: // desisto
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::RedefineArranjoFilhoIgualArranjoPai(VTTrecho *pai, VTTrecho *filho)
{ // DVK 2014.08.07
	// verifica se o Trecho pai possui um Arranjo típico
	if (pai->ArranjoTipico)
	{ // redefine Trecho filho com mesmo Arranjo do Trecho pai
		filho->DefineObjArranjo(pai->arranjo);
	}
	else
	{ // redefine Trecho filho com um clone do Arranjo do Trecho pai
		filho->DefineObjArranjo(pai->arranjo->Clone());
	}
}

// DVK 2017.02.24 altera arranjo do pai, considerando fases do filho
// ---------------------------------------------------------------------------
void __fastcall TValFases::RedefineArranjoPaiFasesFilho(VTTrecho *pai, int fases_filho)
{
	// variáveis locais
	int fases_trecho = fases_filho | pai->arranjo->Fases;
	int fase_tag;
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTMontaZ *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
	VTCabo *vet_cabo[MAX_FASE];
	VTCabo *ult_cabo_fase;
	VTPtoFixacao *pto_fix;
	AnsiString cod_novo_arranjo = pai->arranjo->Codigo + "*" + fases->AsStrABCN[fases_trecho];

	// verifica o tipo do Arranjo
	switch (pai->arranjo->TipoCfg)
	{
	case arranjoCABOxFASE:
		// determina um cabo de fase não nulo
		for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
		{
			if (pai->arranjo->fase[ind_fase].cabo == NULL)
				continue;
			ult_cabo_fase = pai->arranjo->fase[ind_fase].cabo;
			break;
		}
		// proteção
		if (ult_cabo_fase == NULL)
			return;
		// monta vetor de cabos das fases do trecho pai com fases do trecho filho
		for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
		{
			vet_cabo[ind_fase] = pai->arranjo->fase[ind_fase].cabo;
			fase_tag = fases->IndexToTag[ind_fase];
			if ((vet_cabo[ind_fase] == NULL) && (fases->Fase1ContemFase2(fases_trecho, fase_tag)))
			{ // assume cabo não nulo
				vet_cabo[ind_fase] = ult_cabo_fase;
			}
		}
		// verifica se existe o Arranjo para os cabos
		arranjo = arranjos->ExisteArranjoCABOxFASE(pai->arranjo->suporte, vet_cabo[indFASE_A],
			vet_cabo[indFASE_B], vet_cabo[indFASE_C], vet_cabo[indNEUTRO]);
		if (arranjo == NULL)
		{ // DVK 2016.03.30 localiza arranjo pelo novo código
			arranjo = arranjos->ExisteArranjo(cod_novo_arranjo);
		}
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = pai->arranjo->Clone();
			for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
			{
				if (vet_cabo[ind_fase] == NULL)
					continue;
				pto_fix = pai->arranjo->suporte->ExistePtoFixacaoIndex(ind_fase);
				if (pto_fix == NULL)
					continue;
				// define Cabo p/ neutro do Arranjo
				arranjo->DefPtoFixacaoFase(ind_fase, pto_fix);
				arranjo->DefCaboFase(ind_fase, vet_cabo[ind_fase]);
			}
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
			// int na = arranjos->LisArranjo()->Count;
		}
		if (arranjo)
			pai->DefineObjArranjo(arranjo);
		break;
	case arranjoCABO_Z0Z1:
		arranjo = arranjos->ExisteArranjoCABO_Z0Z1(pai->arranjo->caboZ0Z1, fases_trecho);
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = pai->arranjo->Clone();
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
		}
		if (arranjo)
			pai->DefineObjArranjo(arranjo);
		break;
	case arranjoZ0Z1:
		if (pai->ArranjoTipico)
		{
			arranjo = arranjos->ExisteArranjoZ0Z1(pai->arranjo->z0.r, pai->arranjo->z0.x,
				pai->arranjo->c0, pai->arranjo->z1.r, pai->arranjo->z1.x, pai->arranjo->c1,
				fases_trecho);
			if (arranjo == NULL)
			{ // cria um novo Arranjo típico
				arranjo = pai->arranjo->Clone();
				arranjo->Fases = fases_trecho;
				arranjo->Id = FalsoId();
				arranjo->Codigo = cod_novo_arranjo;
				arranjo->Status[sttNOVO] = true;
				// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
				if (!montaZ->Executa(arranjo))
				{ // destrói arranjo
					delete arranjo;
					return;
				}
				arranjos->InsereArranjo(arranjo);
			}
			// redefine o Arranjo do Trecho
			if (arranjo)
				pai->DefineObjArranjo(arranjo);
		}
		else
		{ // redefine fases do arranjo próprio
			pai->arranjo->Fases = fases_trecho;
		}
		break;
	default: // desisto
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::RedefineArranjoPaiIgualArranjoFilho(VTTrecho *pai, VTTrecho *filho)
{
	// verifica se o Trecho filho possui um Arranjo típico
	if (filho->ArranjoTipico)
	{ // redefine Trecho pai com mesmo Arranjo do Trecho filho
		pai->DefineObjArranjo(filho->arranjo);
	}
	else
	{ // redefine Trecho pai com um clone do Arranjo do Trecho filho
		pai->DefineObjArranjo(filho->arranjo->Clone());
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::RedefineArranjoComNeutro(VTTrecho *trecho)
{
	// variáveis locais
	int fases_trecho = trecho->arranjo->Fases | faseN;
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTMontaZ *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
	VTCabo *vet_cabo[MAX_FASE];
	VTPtoFixacao *pto_fix;
	AnsiString cod_novo_arranjo = trecho->arranjo->Codigo + "*" + fases->AsStrABCN[fases_trecho];

	// verifica o tipo do Arranjo
	switch (trecho->arranjo->TipoCfg)
	{
	case arranjoCABOxFASE:
		// monta vetor de cabos das fases, com o neutro
		for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
			vet_cabo[ind_fase] = trecho->arranjo->fase[ind_fase].cabo;
			if (vet_cabo[ind_fase] != NULL)
				vet_cabo[indNEUTRO] = vet_cabo[ind_fase];
		}
		// verifica se existe o Arranjo para os cabos
		arranjo = arranjos->ExisteArranjoCABOxFASE(trecho->arranjo->suporte, vet_cabo[indFASE_A],
			vet_cabo[indFASE_B], vet_cabo[indFASE_C], vet_cabo[indNEUTRO]);
		if (arranjo == NULL)
		{ // DVK 2016.03.30 localiza arranjo pelo código + *
			arranjo = arranjos->ExisteArranjo(cod_novo_arranjo);
		}
		if (arranjo == NULL)
		{ // define PtoFixacao p/ neutro do Arranjo
			pto_fix = trecho->arranjo->suporte->ExistePtoFixacaoIndex(3);
			if (pto_fix == NULL)
				return;
			// cria um novo Arranjo típico
			arranjo = trecho->arranjo->Clone();
			// define Cabo p/ neutro do Arranjo
			arranjo->DefPtoFixacaoFase(indNEUTRO, pto_fix);
			arranjo->DefCaboFase(indNEUTRO, vet_cabo[indNEUTRO]);
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
			// int na = arranjos->LisArranjo()->Count;
		}
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		break;
	case arranjoCABO_Z0Z1:
		arranjo = arranjos->ExisteArranjoCABO_Z0Z1(trecho->arranjo->caboZ0Z1, fases_trecho);
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = trecho->arranjo->Clone();
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
		}
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		break;
	case arranjoZ0Z1:
		if (trecho->ArranjoTipico)
		{
			arranjo = arranjos->ExisteArranjoZ0Z1(trecho->arranjo->z0.r, trecho->arranjo->z0.x,
				trecho->arranjo->c0, trecho->arranjo->z1.r, trecho->arranjo->z1.x,
				trecho->arranjo->c1, fases_trecho);
			if (arranjo == NULL)
			{ // cria um novo Arranjo típico
				arranjo = trecho->arranjo->Clone();
				arranjo->Fases = fases_trecho;
				arranjo->Id = FalsoId();
				arranjo->Codigo = cod_novo_arranjo;
				arranjo->Status[sttNOVO] = true;
				// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
				if (!montaZ->Executa(arranjo))
				{ // destrói arranjo
					delete arranjo;
					return;
				}
				arranjos->InsereArranjo(arranjo);
			}
			// redefine o Arranjo do Trecho
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
		}
		else
		{ // redefine fases do arranjo próprio
			trecho->arranjo->Fases = fases_trecho;
		}
		break;
	default: // desisto
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::RedefineArranjoSemNeutro(VTTrecho *trecho)
{
	// variáveis locais
	int fases_trecho = trecho->arranjo->Fases & faseABC;
	VTArranjo *arranjo = NULL;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTMontaZ *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
	VTCabo *vet_cabo[MAX_FASE];
	AnsiString cod_novo_arranjo = trecho->arranjo->Codigo + "*" + fases->AsStrABCN[fases_trecho];

	// verifica o tipo do Arranjo
	switch (trecho->arranjo->TipoCfg)
	{
	case arranjoCABOxFASE:
		// monta vetor de cabos das fases, sem o neutro
		for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
			vet_cabo[ind_fase] = trecho->arranjo->fase[ind_fase].cabo;
		}
		vet_cabo[indNEUTRO] = NULL;
		// verifica se existe o Arranjo para os cabos
		arranjo = arranjos->ExisteArranjoCABOxFASE(trecho->arranjo->suporte, vet_cabo[indFASE_A],
			vet_cabo[indFASE_B], vet_cabo[indFASE_C], vet_cabo[indNEUTRO]);
		if (arranjo == NULL)
		{ // DVK 2016.03.30 localiza arranjo pelo código + *
			arranjo = arranjos->ExisteArranjo(cod_novo_arranjo);
		}
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = trecho->arranjo->Clone();
			// define PtoFixacao e Cabo p/ neutro do Arranjo
			arranjo->DefPtoFixacaoFase(indNEUTRO, NULL);
			arranjo->DefCaboFase(indNEUTRO, vet_cabo[indNEUTRO]);
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
			// int na = arranjos->LisArranjo()->Count;
		}
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		break;
	case arranjoCABO_Z0Z1:
		arranjo = arranjos->ExisteArranjoCABO_Z0Z1(trecho->arranjo->caboZ0Z1, fases_trecho);
		if (arranjo == NULL)
		{ // cria um novo Arranjo típico
			arranjo = trecho->arranjo->Clone();
			arranjo->Fases = fases_trecho;
			arranjo->Id = FalsoId();
			arranjo->Codigo = cod_novo_arranjo;
			arranjo->Status[sttNOVO] = true;
			// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
			if (!montaZ->Executa(arranjo))
			{ // destrói arranjo
				delete arranjo;
				return;
			}
			arranjos->InsereArranjo(arranjo);
		}
		if (arranjo)
			trecho->DefineObjArranjo(arranjo);
		break;
	case arranjoZ0Z1:
		if (trecho->ArranjoTipico)
		{
			arranjo = arranjos->ExisteArranjoZ0Z1(trecho->arranjo->z0.r, trecho->arranjo->z0.x,
				trecho->arranjo->c0, trecho->arranjo->z1.r, trecho->arranjo->z1.x,
				trecho->arranjo->c1, fases_trecho);
			if (arranjo == NULL)
			{ // cria um novo Arranjo típico
				arranjo = trecho->arranjo->Clone();
				arranjo->Fases = fases_trecho;
				arranjo->Id = FalsoId();
				arranjo->Codigo = cod_novo_arranjo;
				arranjo->Status[sttNOVO] = true;
				// DVK 2016.03.30 calcula Matriz de impedâncias do Arranjo
				if (!montaZ->Executa(arranjo))
				{ // destrói arranjo
					delete arranjo;
					return;
				}
				arranjos->InsereArranjo(arranjo);
			}
			// redefine o Arranjo do Trecho
			if (arranjo)
				trecho->DefineObjArranjo(arranjo);
		}
		else
		{ // redefine fases do arranjo próprio
			trecho->arranjo->Fases = fases_trecho;
		}
		break;
	default: // desisto
		break;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16017631
bool __fastcall TValFases::SubstituiTrechosFasesInvalidas(int faseNOVA)
{
	// Variaveis locais
	bool sucesso = false;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisTRECHOS_INV;
	VTTrecho *trecho;

	// protecao
	if (redes == NULL)
	{
		return false;
	}
	// cria lista
	lisTRECHOS_INV = new TList;
	// lista todos trechos
	redes->LisLigacao(lisTRECHOS_INV, eqptoTRECHO);
	// percorre toda lista de trechos
	SubstituiTrechosFasesInvalidas(lisTRECHOS_INV,faseNOVA);
//	for (int nt = 0; nt < lisTRECHOS_INV->Count; nt++)
//	{
//		trecho = (VTTrecho*) lisTRECHOS_INV->Items[nt];
//		// caso tenha arranjo com fase invalida
//		if (trecho->arranjo != NULL)
//		{
//			if ((trecho->arranjo->Fases == faseINV) || (trecho->arranjo->Fases == faseN))
//			{
//				RedefineArranjo(trecho, faseNOVA);
////				CorrigeTrechoFaseINV(trecho);
//			}
//		}
//	}
	// deleta lista
	if (lisTRECHOS_INV != NULL)
	{
		delete lisTRECHOS_INV;
		lisTRECHOS_INV = NULL;
	}
	sucesso = true;
	return (sucesso);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16017631
bool __fastcall TValFases::SubstituiTrechosFasesInvalidas(TList *lisLIG, int faseNOVA)
{
	// Variaveis locais
	bool sucesso = false;
	VTTrecho *trecho;
	VTLigacao *ligacao;

	try
	{
		// percorre toda lista de trechos
		for (int nt = 0; nt < lisLIG->Count; nt++)
		{
			ligacao = (VTLigacao*) lisLIG->Items[nt];
			if (ligacao->Tipo() != eqptoTRECHO)
			{
				continue;
			}
			trecho = (VTTrecho*) ligacao;
			// caso tenha arranjo com fase invalida
			if (trecho->arranjo != NULL)
			{
				if (trecho->arranjo->Fases == faseINV)
				{
					if(faseNOVA > 0)
					{
						RedefineArranjo(trecho, faseNOVA);
					}
					else
					{
						CorrigeTrechoFaseINV(trecho);
					}
				}
				else if (trecho->arranjo->Fases == faseN)
				{
					if(faseNOVA > 0)
					{
						//faz nada
					}
					else
					{
						CorrigeTrechoFaseINV(trecho);
					}
				}
			}
		}
		sucesso = true;
	}
	catch(...)
	{
		int a = 0;
        sucesso = false;
    }

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16017631
void __fastcall TValFases::VerificaFases(VTPrimario * primario)
{
	// obtém lista ordenada de barras e ligações do primário
	primario->Ordena(lisBAR, lisLIG);
	//acerta os trechos com faseN
	SubstituiTrechosFasesInvalidas(lisLIG, -1);
	// se necessário, corrige fases dos Trafos
	CorrigeFasesTrafoMTBT(primario);
	// verifica fases de Cargas e Trechos
	VerificaFases();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16017631
void __fastcall TValFases::VerificaFases(VTSecundario * secundario)
{
	// obtém lista ordenada de barras e ligações do primário
	secundario->Ordena(lisBAR, lisLIG);
	//acerta os trechos com faseN
	SubstituiTrechosFasesInvalidas(lisLIG, -1);
	// se necessário, corrige fases dos Trafos
	CorrigeFasesTrafoSecundario(secundario);
	// verifica fases de Cargas e Trechos
	VerificaFases();
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TValFases::VerificaFases(void)
 {
 // variáveis locais
 int fases_filha, fases_pai;
 VTEqpto *eqpto;
 VTLigacao *filha, *pai, *chave;
 VTBarra *bar_filha, *bar_pai, *bar_carga;
 VTTrecho *trecho_pai;

 // verifica se deve corrigir fases dos Trechos
 if (CorrigeFases)
 { // loop p/ acertar barra inicial de Regulador de tensão
 for (int nl = 0; nl < lisLIG->Count; nl++)
 { // determina Ligacao que seja Regulador
 filha = (VTLigacao*)lisLIG->Items[nl];
 if (filha->Tipo() != eqptoREGULADOR)
 continue;
 // determina Ligacao pai
 if ((pai = filha->ligaPai) == NULL)
 continue;
 // determina Barra comum entre Regulador e Ligacao pai
 if ((bar_filha = filha->BarraComum(pai)) == NULL)
 continue;
 // se necessário, redefine barras do regulador
 ((VTRegulador*)filha)->DefineBarraEntrada(bar_filha);
 }
 // DVK 2017.05.04 loop quebrado em 2: primeiro loop trafo, segundo loop trecho
 // loop p/ todas as Ligacoes, do fim p/ o começo da lista para corrigir fases
 // do trecho pai
 for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
 { // determina Ligacao que seja Trafo
 filha = (VTLigacao*)lisLIG->Items[nl];

 // teste
 // if(filha->Codigo.AnsiCompareIC("C55638") == 0)
 // {
 // int a = 0;
 // }

 // corrige trechos em função das fases do trafo
 if (filha->Tipo() == eqptoTRAFO)
 { // determina Trecho pai que seja um Trecho
 if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
 { // se necessário, corrige as fases da Ligacao pai
 CorrigeFasesTrechoPai(trecho_pai, (VTTrafo*)filha);
 }
 }
 // corrige trechos em função das fases do regulador
 if (filha->Tipo() == eqptoREGULADOR)
 { // determina Trecho pai que seja um Trecho
 if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
 { // se necessário, corrige as fases da Ligacao pai
 CorrigeFasesTrechoPai(trecho_pai, (VTRegulador*)filha);
 }
 }
 }
 for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
 { // determina Ligacao que seja Trecho
 filha = (VTLigacao*)lisLIG->Items[nl];
 if (filha->Tipo() == eqptoTRECHO)
 {
 CorrigeTrechoFaseINV((VTTrecho*)filha);
 // determina Trecho pai que seja um Trecho
 if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
 { // se necessário, corrige as fases da Ligacao pai
 CorrigeFasesTrechoPai(trecho_pai, (VTTrecho*)filha);
 }
 }
 }
 // //repete loop
 // for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
 // {//determina Ligacao que seja Trecho ou Trafo
 // filha = (VTLigacao*)lisLIG->Items[nl];
 // if (filha->Tipo() == eqptoTRECHO)
 // {//determina Trecho pai que seja um Trecho
 // if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
 // {//se necessário, corrige as fases da Ligacao pai
 // CorrigeFasesTrechoPai(trecho_pai, (VTTrecho*)filha);
 // }
 // }
 // //DVK 2017.02.24 corrige trechos em função das fases do trafo
 // if (filha->Tipo() == eqptoTRAFO)
 // {//determina Trecho pai que seja um Trecho
 // if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
 // {//se necessário, corrige as fases da Ligacao pai
 // CorrigeFasesTrechoPai(trecho_pai, (VTTrafo*)filha);
 // }
 // }
 // }
 // //loop p/ todas as Ligacoes, do começo p/ o fim da lista
 // for (int nl = 0; nl < lisLIG->Count; nl++)
 // {//determina Ligacao que seja Trecho ou Regulador
 // filha = (VTLigacao*)lisLIG->Items[nl];
 // //if ((filha->Tipo() == eqptoTRECHO)||(filha->Tipo() == eqptoTRAFO)||(filha->Tipo() == eqptoREGULADOR))
 // if ((filha->Tipo() == eqptoTRECHO) || (filha->Tipo() == eqptoREGULADOR))
 // {//determina Trecho pai que seja um Trecho
 // if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
 // {//se necessário, corrige as fases da Ligacao filha
 // if      (filha->Tipo() == eqptoTRECHO)    CorrigeFasesTrechoFilho(trecho_pai, (VTTrecho*)filha);
 // //else if (filha->Tipo() == eqptoTRAFO)     CorrigeFasesTrafo(trecho_pai, (VTTrafo*)filha);
 // else if (filha->Tipo() == eqptoREGULADOR) CorrigeFasesRegulador(trecho_pai, (VTRegulador*)filha);
 // }
 // }
 // }
 }
 // loop p/ todas as Ligacoes, do fim p/ o começo da lista
 for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
 {
 filha = (VTLigacao*)lisLIG->Items[nl];
 // if (filha->Tipo() == eqptoCHAVE) continue;
 // determina Ligacao pai
 if ((pai = filha->ligaPai) == NULL)
 { // verifica fases das Cargas para todas as Barras da Ligacao
 if (filha->Tipo() == eqptoCHAVE)
 continue;
 VerificaFasesCarga(filha->pbarra1, FasesDaLigacao(filha, filha->pbarra1));
 VerificaFasesCarga(filha->pbarra2, FasesDaLigacao(filha, filha->pbarra2));
 }
 else
 { // determina Barra comum entre Ligacao filha e Ligacao pai
 if ((bar_filha = filha->BarraComum(pai)) == NULL)
 continue;
 // determina Barra para analisar fases de cargas
 bar_carga = (filha->pbarra1 == bar_filha) ? filha->pbarra2 : filha->pbarra1;
 // verifica fases das Cargas conectadas à barra da carga
 VerificaFasesCarga(bar_carga, FasesDaLigacao(filha, bar_carga));
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TValFases::VerificaFases(void)
{
	// variáveis locais
	int fases_filha, fases_pai;
	VTEqpto *eqpto;
	VTLigacao *filha, *pai, *chave;
	VTBarra *bar_filha, *bar_pai, *bar_carga;
	VTTrecho *trecho_pai;

	// verifica se deve corrigir fases dos Trechos
	if (CorrigeFases)
	{ // loop p/ acertar barra inicial de Regulador de tensão
		for (int nl = 0; nl < lisLIG->Count; nl++)
		{ // determina Ligacao que seja Regulador
			filha = (VTLigacao*)lisLIG->Items[nl];
			if (filha->Tipo() != eqptoREGULADOR)
				continue;
			// determina Ligacao pai
			if ((pai = filha->ligaPai) == NULL)
				continue;
			// determina Barra comum entre Regulador e Ligacao pai
			if ((bar_filha = filha->BarraComum(pai)) == NULL)
				continue;
			// se necessário, redefine barras do regulador
			((VTRegulador*)filha)->DefineBarraEntrada(bar_filha);
		}
		// loop quebrado em 2: primeiro loop regulador, segundo loop trecho
		// loop p/ todas as Ligacoes, do fim p/ o começo da lista para corrigir fases
		// do trecho pai
		// Obs.: não considera mais as fases dos trafos MTBT, pois as fases dos trechos primários não devem
		// ser alterados por causa deles... coisa da Copel =/
		for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
		{ // determina Ligacao que seja Trafo
			filha = (VTLigacao*)lisLIG->Items[nl];
			// corrige trechos em função das fases do regulador
			if (filha->Tipo() == eqptoREGULADOR)
			{ // determina Trecho pai que seja um Trecho
				if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
				{ // se necessário, corrige as fases da Ligacao pai
					CorrigeFasesTrechoPai(trecho_pai, (VTRegulador*)filha);
				}
			}
		}
		for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
		{ // determina Ligacao que seja Trecho
			filha = (VTLigacao*)lisLIG->Items[nl];
			if (filha->Tipo() == eqptoTRECHO)
			{
				CorrigeTrechoFaseINV((VTTrecho*)filha);
				// determina Trecho pai que seja um Trecho
				if ((trecho_pai = ExisteTrechoPai(filha)) != NULL)
				{ // se necessário, corrige as fases da Ligacao pai
					CorrigeFasesTrechoPai(trecho_pai, (VTTrecho*)filha);
				}
			}
		}
	}
	// loop p/ todas as Ligacoes, do fim p/ o começo da lista
	for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
	{
		filha = (VTLigacao*)lisLIG->Items[nl];
		// if (filha->Tipo() == eqptoCHAVE) continue;
		// determina Ligacao pai
		if ((pai = filha->ligaPai) == NULL)
		{ // verifica fases das Cargas para todas as Barras da Ligacao
			if (filha->Tipo() == eqptoCHAVE)
				continue;
			VerificaFasesCarga(filha->pbarra1, FasesDaLigacao(filha, filha->pbarra1));
			VerificaFasesCarga(filha->pbarra2, FasesDaLigacao(filha, filha->pbarra2));
		}
		else
		{ // determina Barra comum entre Ligacao filha e Ligacao pai
			if ((bar_filha = filha->BarraComum(pai)) == NULL)
				continue;
			// determina Barra para analisar fases de cargas
			bar_carga = (filha->pbarra1 == bar_filha) ? filha->pbarra2 : filha->pbarra1;
			// verifica fases das Cargas conectadas à barra da carga
			VerificaFasesCarga(bar_carga, FasesDaLigacao(filha, bar_carga));
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValFases::VerificaFasesCarga(VTBarra *barra, int fases_tag)
{
	// variáveis locais
	int fases_carga;
	VTCarga *carga;
	VTEqpto *eqpto;
	TList *lisEQB;

	// obtém lista de Eqbar da Barra
	lisEQB = barra->LisEqbar();
	for (int n = 0; n < lisEQB->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQB->Items[n];
		if (eqpto->Tipo() != eqptoCARGA)
			continue;
		carga = (VTCarga*)eqpto;
		fases_carga = carga->Fases;
		// verifica se as fases disponíveis contém as fases da Carga
		if (!fases->Fase1ContemFase2(fases_tag, fases_carga))
		{ // verifica se deve corrigir as fases
			if (CorrigeFases)
			{ // corrige fases da Carga
				CorrigeFasesCarga(carga, fases_tag);
			}
			else
			{ // Carga com fases inconsistentes
				lisEQP->Add(carga);
			}
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TValFases::VerificaTrafosMonoEmDelta(TList *lisTRF)
{
	// variáveis locais
	VTBarra *bar_pri, *bar_sec;
	VTTrafo *trafo;

	// proteção: verifica se há apenas um Trafo
	if (lisTRF->Count <= 1)
		return (false);
	// verifica se todos Trafos são monofásicos conectados nas mesmas Barras
	bar_pri = NULL;
	bar_sec = NULL;
	for (int n = 0; n < lisTRF->Count; n++)
	{
		trafo = (VTTrafo*)lisTRF->Items[n];
		// verifica se é um trafo monofásico com uma única fase
		if (!trafo->Monofasico)
			return (false);
		if (fases->NumeroDeFases(trafo->pri.fase) != 1)
			return (false);
		// verifica se o Trafo conecta as mesmas barras
		if (bar_pri == NULL)
			bar_pri = trafo->BarraPrimario();
		if (bar_sec == NULL)
			bar_sec = trafo->BarraSecundario();
		if (trafo->BarraPrimario() != bar_pri)
			return (false);
		if (trafo->BarraSecundario() != bar_sec)
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
// eof
