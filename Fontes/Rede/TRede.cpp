// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include "TRede.h"
#include "VTArea.h"
#include "VTBarra.h"
#include "VTCarga.h"
#include "VTChave.h"
#include "VTCorte.h"
#include "VTDadosExt.h"
#include "VTLigacao.h"
#include "VTMedidor.h"
#include "VTMutua.h"
#include "VTMRede.h"
#include "VTTrecho.h"
#include "VTRedes.h"
#include "VTTipoRede.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTRede
// ---------------------------------------------------------------------------
VTRede* __fastcall NewObjRede(int num_pat)
{
	return (new TRede(num_pat));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16511871
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
__fastcall TRede::TRede(int num_pat)
{
	// inicia dados
	FluxoOK = false;
	Valida = true;
	Estatica = true;
	Radial = false;
	// Redutivel   = true;
	Carregada = false;
	bd.barra_id = -1;
	// Valida      = false;
	Color = clDefault;
	// inicia DynamicArray
	diag.flow.Length = num_pat;
	// cria áreas
	area.esq = NewObjArea();
	area.utm = NewObjArea();
	// cria listas
	lisBAR = new TList();
	lisLIG = new TList();
	lisMUT = new TList();
	lisCOR = new TList();
	lisBAR_NaoIso = new TList();
	// cria objetos p/ esquemático
	esq.pbarra = NewObjBarra(num_pat);
	esq.lisLIG = new TList();
    // cria objeto VTDadosExt
    dados_ext = NewObjDadosExt();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16511871
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
__fastcall TRede::~TRede(void)
{
	// destrói objetos
    if (dados_ext)
    {
        delete dados_ext;
        dados_ext = NULL;
    }

	if (area.esq)
	{
		delete area.esq;
		area.esq = NULL;
	}
	if (area.utm)
	{
		delete area.utm;
		area.utm = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisBAR_NaoIso)
	{
		delete lisBAR_NaoIso;
		lisBAR_NaoIso = NULL;
	}
	if (lisCOR)
	{
		delete lisCOR;
		lisCOR = NULL;
	}
	// destrói lista e seus objetos
	if (lisLIG)
	{
		LimpaTList(lisLIG);
		delete lisLIG;
		lisLIG = NULL;
	}
	if (lisMUT)
	{
		LimpaTList(lisMUT);
		delete lisMUT;
		lisMUT = NULL;
	}
	// linera DynamicArray
	diag.flow.Length = 0;
	// destrói objetos usados p/ esquemático
	if (esq.pbarra)
	{
		delete esq.pbarra;
		esq.pbarra = NULL;
	}
	if (esq.lisLIG)
	{
		LimpaTList(esq.lisLIG);
		delete esq.lisLIG;
		esq.lisLIG = NULL;
	}
}

// -----------------------------------------------------------------------------
VTArea* __fastcall TRede::AreaEsquematico(void)
{
	return (area.esq);
}

// -----------------------------------------------------------------------------
VTArea* __fastcall TRede::AreaUtm_cm(void)
{
	return (area.utm);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TRede::BarraFicticia(void)
{
	return (esq.pbarra);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TRede::BarraInicial(void)
{
	return (pbar_ini);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::CalculaArea(void)
{
	// verifica se está carregada
	if (!Carregada)
		return;
	// calcula área da rede utm
	CalculaAreaUtm();
	// calcula área do diag.esquemático
	CalculaAreaEsquematico();
}

// ---------------------------------------------------------------------------
void __fastcall TRede::CalculaAreaEsquematico(void)
{
	// variáveis locais
	struct
	{
		int x1, y1, x2, y2;
	} area_b, area_t;
	VTBarra *pbarra;

	// inicia coordenadas c/ valor 0
	area_t.x1 = area_t.x2 = 0;
	area_t.y1 = area_t.y2 = 0;
	// verifica se rede vazia
	if (lisBAR->Count > 0)
	{ // calcula área definida pelas Barras
		pbarra = (VTBarra*)lisBAR->First();
		pbarra->AreaEsquematico(area_t.x1, area_t.y1, area_t.x2, area_t.y2);
		for (int n = 1; n < lisBAR->Count; n++)
		{ // obtém área da Barra
			pbarra = (VTBarra*)lisBAR->Items[n];
			pbarra->AreaEsquematico(area_b.x1, area_b.y1, area_b.x2, area_b.y2);
			area_t.x1 = min(area_t.x1, area_b.x1);
			area_t.y1 = min(area_t.y1, area_b.y1);
			area_t.x2 = max(area_t.x2, area_b.x2);
			area_t.y2 = max(area_t.y2, area_b.y2);
		}
	}
	// atualiza Area da Rede
	area.esq->x1 = area_t.x1;
	area.esq->y1 = area_t.y1;
	area.esq->x2 = area_t.x2;
	area.esq->y2 = area_t.y2;
}

// -----------------------------------------------------------------------------
void __fastcall TRede::CalculaAreaUtm(void)
{
	// variáveis locais
	bool first = true;
	int x, y;
	VTBarra *barra;

	// assume valores inválido
	area.utm->x1 = area.utm->y1 = area.utm->x2 = area.utm->y2 = -1;
	if (lisBAR->Count == 0)
		return;
	// calcula área em cm
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		// obtém coordenadas da Barra em cm
		if (!barra->CoordenadasUtm_cm(x, y))
			continue;
		if (first)
		{ // inicia área c/ as coordenadas da Barra
			area.utm->x1 = area.utm->x2 = x;
			area.utm->y1 = area.utm->y2 = y;
			first = false;
		}
		else
		{ // calcula área
			area.utm->x1 = Min(int(area.utm->x1), x);
			area.utm->y1 = Min(int(area.utm->y1), y);
			area.utm->x2 = Max(int(area.utm->x2), x);
			area.utm->y2 = Max(int(area.utm->y2), y);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRede::Clear(void)
{
	// limpa listas de Cortes sem destruir os objetos
	lisCOR->Clear();
	// limpa listas de barras sem destruir os objetos
	lisBAR->Clear();
	// destrói Mútuas
	LimpaTList(lisMUT);
	// destrói Ligacoes
	LimpaTList(lisLIG);
	// reinicia ponteiros
	pbar_ini = NULL;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
VTRede* __fastcall TRede::Clone(void)
{
	// variáveis locais
	VTRede *rede;

	// cria uma nova Rede
	if ((rede = NewObjRede(diag.flow.Length)) != NULL)
	{ // copia dados desta Rede p/ a nova Rede
		rede->CopiaAtributosDe(*this);
        // copia dados_ext
        rede->dados_ext->CopiaAtributosDe(*dados_ext);
	}
	return (rede);
}

// ---------------------------------------------------------------------------
double __fastcall TRede::Comprimento_km(VTArranjo *arranjo)
{
	// variáveis locais
	double comp_km = 0;
	VTLigacao *ligacao;
	VTTrecho *trecho;

	// loop p/ todas as Ligacoes
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() != eqptoTRECHO)
			continue;
		trecho = (VTTrecho*)ligacao;
		if ((arranjo == NULL) || (trecho->arranjo == arranjo))
		{ // soma comprimento do Trecho
			comp_km += trecho->Comprimento_km;
		}
	}
	return (comp_km);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::CopiaAtributosDe(VTEqpto &ref)
{
	// variáveis locais
	TRede &rede = (TRede&)ref;
	VTLigacao *ligacao;
	TList *lisEXT;

	// proteção
	if (ref.Tipo() != eqptoREDE)
		return;
	// copia atributos de VTEqpto
	VTEqpto::CopiaAtributosDe(rede);
	// copia atributos de VTRede
	TipoRede = rede.TipoRede;
	Redes = rede.Redes;
	// define Barra inicial
	// pbar_ini    = rede.pbar_ini;
	// bd.barra_id = rede.bd.barra_id;
	DefineBarraInicial(rede.pbar_ini);
	// copia atributos acessados via property
	PD = rede.PD;
	// JCG 2013.12.11 - não copia ponteiro p/ MRede pois MRede é um objeto recriado
	// a cada execução do objeto Topologia
	// mrede      = rede.mrede;
	area.esq->CopiaAtributosDe(*rede.area.esq);
	area.utm->CopiaAtributosDe(*rede.area.utm);
	diag = rede.diag;
	Cluster = rede.Cluster;
	// copia objetos usados p/ esquemático
	if (rede.esq.pbarra)
	{ // destrói objetos usados p/ esquemático
		if (esq.pbarra)
		{
			delete esq.pbarra;
			esq.pbarra = NULL;
		}
		LimpaTList(esq.lisLIG);
		// copia dados
		esq.height = rede.esq.height;
		esq.width = rede.esq.width;
		esq.pbarra = rede.esq.pbarra->Clone();
		lisEXT = rede.esq.lisLIG;
		for (int n = 0; n < lisEXT->Count; n++)
		{
			ligacao = (VTLigacao*)lisEXT->Items[n];
			esq.lisLIG->Add(ligacao->Clone());
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRede::DefineBarraInicial(VTBarra *pbarra)
{
	// salva ponteiro p/ Barra inicial
	pbar_ini = pbarra;
	// salva ID da barra inicial
	bd.barra_id = (pbar_ini != NULL) ? pbar_ini->Id : -1;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::DefineObjRedes(VTRedes *redes)
{
	// salva ponteiro p/ objeto
	this->redes = redes;
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::ExisteAlteracao(void)
{
	// variáveis locais
	VTEqpto *eqpto;

	// verifica se a própria Rede é nova ou alterada
	if ((Status[sttNOVO]) || (Status[sttALTERADO]) || (Status[sttDESLOCADO]))
		return (true);
	// verifica se há alguma Ligacao nova ou alterada
	for (int n = 0; n < lisLIG->Count; n++)
	{
		eqpto = (VTEqpto*)lisLIG->Items[n];
		// if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
		if ((eqpto->Status[sttNOVO]) || (eqpto->Status[sttALTERADO]) ||
			(eqpto->Status[sttDESLOCADO]))
			return (true);
	}
	// verifica se há alguma Mutua nova ou alterada
	for (int n = 0; n < lisMUT->Count; n++)
	{
		eqpto = (VTEqpto*)lisMUT->Items[n];
		// if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
		if ((eqpto->Status[sttNOVO]) || (eqpto->Status[sttALTERADO]) ||
			(eqpto->Status[sttDESLOCADO]))
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::ExisteBarra(VTBarra *pbarra)
{
	return (lisBAR->IndexOf(pbarra) >= 0);
}

// ---------------------------------------------------------------------------
VTCorte* __fastcall TRede::ExisteCorte(VTRede *rede_viz)
{
	// variáveis locais
	VTCorte *corte;
	TList *lisEXT;

	for (int n = 0; n < lisCOR->Count; n++)
	{
		corte = (VTCorte*)lisCOR->Items[n];
		lisEXT = corte->LisRede();
		// if ((lisEXT->Count == 2)&&(lisEXT->IndexOf(rede_viz) >= 0)) return(corte);
		if (lisEXT->IndexOf(rede_viz) >= 0)
			return (corte);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16511871
bool __fastcall TRede::ExisteGeracao(void)
{
	// variáveis locais
	bool existe = false;
    // bool barra_isolada = false;
    // TList *lisEQP = new TList();
	VTBarra *barra;

	// verifica se há geração somente nas barras não isoladas
    // LisBarra(lisEQP, barra_isolada);
	// loop p/ todos objetos Barra em lisEQP
	//for (int n = 0; n < lisBAR->Count; n++)
	for (int n = 0; n < lisBAR_NaoIso->Count; n++)
	{
		barra = (VTBarra*)lisBAR_NaoIso->Items[n];
		if (barra->ExisteGeracao())
		{
			return true;
            // existe = true;
            // break;
		}
	}
    // delete lisEQP;
	return (existe);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::ExisteLigacao(VTLigacao *pliga)
{
	return (lisLIG->IndexOf(pliga) >= 0);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TRede::ExisteLigacao(int tipo_eqpto, AnsiString codigo)
{
	// variáveis locais
	VTLigacao *pliga;

	// procura Ligacao que conecta a Barra
	for (int n = 0; n < lisLIG->Count; n++)
	{
		pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != tipo_eqpto)
			continue;
		if (pliga->Codigo.AnsiCompareIC(codigo) == 0)
			return (pliga);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TRede::ExisteLigacao(int tipo_eqpto, int eqpto_id)
{
	// variáveis locais
	VTLigacao *pliga;

	// procura Ligacao que conecta a Barra
	for (int n = 0; n < lisLIG->Count; n++)
	{
		pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != tipo_eqpto)
			continue;
		if (pliga->Id == eqpto_id)
			return (pliga);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TRede::ExisteLigacao(VTBarra *barra)
{
	// variáveis locais
	VTLigacao *ligacao;

	// procura Ligacao que conecta a Barra
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->ConectaBarra(barra))
			return (ligacao);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TRede::ExisteLigacao(VTBarra *barra1, VTBarra *barra2, VTBarra *barra3)
{
	// variáveis locais
	VTLigacao *ligacao;

	// proteção
	if (barra1 == barra2)
		return (NULL);
	if (barra1 == barra3)
		return (NULL);
	if (barra2 == barra3)
		return (NULL);
	// procura Ligacao que conecta a Barra
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (!ligacao->ConectaBarra(barra1))
			continue;
		if (!ligacao->ConectaBarra(barra2))
			continue;
		if ((barra3 == NULL) || (ligacao->ConectaBarra(barra3)))
			return (ligacao);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::ExisteMutua(VTTrecho *trecho)
{
	// variáveis locais
	VTMutua *mutua;

	// verifica se existe Mutua p/ o Trecho indicado
	for (int n = 0; n < lisMUT->Count; n++)
	{
		mutua = (VTMutua*)lisMUT->Items[n];
		if ((mutua->ptre1 == trecho) || (mutua->ptre2 == trecho))
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16511871
void __fastcall TRede::IniciaLisBarra(void)
{
	// variáveis locais
	VTBarra *pbarra;
	VTLigacao *pliga;
	VTChave *chave;

	// reinicia lista c/ a Barra inicial
	lisBAR->Clear();
	if (pbar_ini)
	{
		lisBAR->Add(pbar_ini);
		// proteção: redeinde bd.barra_id
		bd.barra_id = pbar_ini->Id;
	}
	// reinicia lista de Barras a partir dos objetos VTLigacao
	for (int n = 0; n < lisLIG->Count; n++)
	{
		pliga = (VTLigacao*)lisLIG->Items[n];
		pliga->LisBarra(lisBAR);
	}
	// determina Barra inicial de Rede radial
	if ((Radial) && (pbar_ini == NULL))
	{
		for (int n = 0; n < lisBAR->Count; n++)
		{
			pbarra = (VTBarra*)lisBAR->Items[n];
			if (pbarra->Id == bd.barra_id)
			{ // encontrou Barra inicial
				pbar_ini = pbarra;
				break;
			}
		}
	}
    // com a lista de barras totais, preenche a lista com as barras não isoladas
	lisBAR_NaoIso->Clear();
	// loop p/ todos objetos Barra em lisBAR
	for (int n = 0; n < lisBAR->Count; n++)
	{
		pbarra = (VTBarra*)lisBAR->Items[n];
		// inicia tag com 0
		pbarra->Tag = 0;
	}
	// loop p/ todas as Ligacoes
	for (int n = 0; n < lisLIG->Count; n++)
	{
		pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() == eqptoCHAVE)
		{ // não inclui Barras de Chave aberta
			chave = (VTChave*)pliga;
			if (chave->Aberta)
				continue;
		}
		// atualiza tag das barras da ligação com 1
		if (pliga->pbarra1)
			pliga->pbarra1->Tag = 1;
		if (pliga->pbarra2)
			pliga->pbarra2->Tag = 1;
		if (pliga->pbarra3)
			pliga->pbarra3->Tag = 1;
	}
	// loop p/ todos objetos Barra em lisBAR
	for (int n = 0; n < lisBAR->Count; n++)
	{
		pbarra = (VTBarra*)lisBAR->Items[n];
		// verifica se tag é 1
		if (pbarra->Tag == 1)
		{
			// lisEQP->Add(barra);
			if (lisBAR_NaoIso->IndexOf(pbarra) < 0)
				lisBAR_NaoIso->Add(pbarra);
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TRede::IniciaLisLigacao(TList *lisEQP)
{
	// reinicia lista de Ligacoes
	lisLIG->Clear();
	// verifica se foi passada uma lista de Ligacao
	if (lisEQP != NULL)
	{ // insere Ligacoes na rede
		for (int nl = 0; nl < lisEQP->Count; nl++)
		{
			InsereLigacao((VTLigacao*)lisEQP->Items[nl]);
		}
	}
	// reinicia lista de Barras
	IniciaLisBarra();
}

// ---------------------------------------------------------------------------
void __fastcall TRede::InsereCorte(VTCorte *corte)
{
	// proteção
	if (corte == NULL)
		return;
	// insere objeto Corte na lista lisCOR
	if (lisCOR->IndexOf(corte) < 0)
	{
		lisCOR->Add(corte);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::InsereLigacao(VTLigacao *ligacao)
 {
 //variáveis locais
 VTBarra *barra;

 //proteção
 if (ligacao == NULL) return;
 //insere objeto VTLigacao na lista lisLIG
 if (lisLIG->IndexOf(ligacao) < 0)
 {
 lisLIG->Add(ligacao);
 //associa Ligacao c/ esta Rede
 ligacao->DefineObjRede(this);
 //define para a Ligacao o mesmo Status isolado da Rede
 ligacao->Status[sttISOLADO] = Status[sttISOLADO];
 }
 //se é uma Rede Radial, verifica se Barra inicial ainda não foi definida
 if ((Radial)&&(pbar_ini == NULL))
 {//verifica se o ID da barra inicial é nulo
 if (bd.barra_id == 0)
 {//define a Barra inicial da Rede
 DefineBarraInicial(ligacao->pbarra1);
 }
 else
 {//verifica se uma das Barras é a inicial
 for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
 {
 if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
 if (barra->Id == bd.barra_id)
 {//barra inicial
 DefineBarraInicial(barra);
 break;
 }

 }
 }
 }
 //insere Barras da Ligacao na Rede
 ligacao->LisBarra(lisBAR);
 //remove Ligacao da Lixeira
 //redes->RemoveEqptoDaLixeira(ligacao);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TRede::InsereLigacao(VTLigacao *ligacao)
{
	// variáveis locais
	VTBarra *barra;

	// proteção
	if (ligacao == NULL)
		return;
	// insere objeto VTLigacao na lista lisLIG
	if (lisLIG->IndexOf(ligacao) < 0)
	{
		lisLIG->Add(ligacao);
		// associa Ligacao c/ esta Rede
		ligacao->DefineObjRede(this);
		// define para a Ligacao o mesmo Status isolado da Rede
		ligacao->Status[sttISOLADO] = Status[sttISOLADO];
		// insere Ligacao nas suas Barras
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao->Barra(ind_bar)) == NULL)
				continue;
			barra->InsereLigacao(ligacao);
		}
	}
	// se é uma Rede Radial, verifica se Barra inicial ainda não foi definida
	if ((Radial) && (pbar_ini == NULL))
	{ // verifica se o ID da barra inicial é nulo
		if (bd.barra_id == 0)
		{ // define a Barra inicial da Rede
			DefineBarraInicial(ligacao->pbarra1);
		}
		else
		{ // verifica se uma das Barras é a inicial
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
				if ((barra = ligacao->Barra(ind_bar)) == NULL)
					continue;
				if (barra->Id == bd.barra_id)
				{ // barra inicial
					DefineBarraInicial(barra);
					break;
				}

			}
		}
	}
	// insere Barras da Ligacao na Rede
	ligacao->LisBarra(lisBAR);
	// remove Ligacao da Lixeira
	// redes->RemoveEqptoDaLixeira(ligacao);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::InsereMutua(VTMutua *mutua)
{
	// proteção
	if (mutua == NULL)
		return;
	// não permite inserção de Mutua se a Rede não for estatica
	if (!Estatica)
		return;
	// insere objeto VTMutua na lista lisMUT
	if (lisMUT->IndexOf(mutua) < 0)
	{
		lisMUT->Add(mutua);
	}
	// remove Mutua da Lixeira
	// redes->RemoveEqptoDaLixeira(mutua);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisArranjo(TList *lisEQP)
{
	// variáveis locais
	VTTrecho *trecho;
	VTLigacao *ligacao;

	// loop p/ todos objetos VTLigacao
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoTRECHO)
		{
			trecho = (VTTrecho*)ligacao;
			if (lisEQP->IndexOf(trecho->arranjo) < 0)
				lisEQP->Add(trecho->arranjo);
		}
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TRede::LisBarra(void)
{
	return (lisBAR);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisBarra(TList *lisEQP, bool barra_isolada)
{
	// variáveis locais
	VTBarra *barra;
	VTChave *chave;
	VTLigacao *ligacao;

	// verifica se deve incluir Barras isoladas
	if (barra_isolada)
	{ // copia barras
		CopiaTList(lisBAR, lisEQP);
	}
	else
	{ // loop p/ todos objetos Barra em lisBAR
		for (int n = 0; n < lisBAR->Count; n++)
		{
			barra = (VTBarra*)lisBAR->Items[n];
			// inicia tag com 0
			barra->Tag = 0;
		}
		// loop p/ todas as Ligacoes
		for (int n = 0; n < lisLIG->Count; n++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[n];
			if (ligacao->Tipo() == eqptoCHAVE)
			{ // não inclui Barras de Chave aberta
				chave = (VTChave*)ligacao;
				if (chave->Aberta)
					continue;
			}
			// atualiza tag das barras da ligação com 1
			if (ligacao->pbarra1)
				ligacao->pbarra1->Tag = 1;
			if (ligacao->pbarra2)
				ligacao->pbarra2->Tag = 1;
			if (ligacao->pbarra3)
				ligacao->pbarra3->Tag = 1;
		}
		// loop p/ todos objetos Barra em lisBAR
		for (int n = 0; n < lisBAR->Count; n++)
		{
			barra = (VTBarra*)lisBAR->Items[n];
			// verifica se tag é 1
			if (barra->Tag == 1)
			{
				// lisEQP->Add(barra);
				if (lisEQP->IndexOf(barra) < 0)
					lisEQP->Add(barra);
			}
		}
	}
	// insere Barra inicial de rede radial
	if (Radial)
	{
		if ((barra = BarraInicial()) != NULL)
		{
			if (lisEQP->IndexOf(barra) < 0)
				lisEQP->Add(barra);
		}
	}
}

/*
 2012.01.11 função transferida p/ VTRedes porque não existem Cortes associados
 a barras ligadas por chave aberta
 //---------------------------------------------------------------------------
 void __fastcall TRede::LisBarraExclusiva(TList *lisEQP)
 {
 //variáveis locais
 TList   *lisBAR_COR;
 VTBarra *barra;
 VTCorte *corte;

 //inclui todas as Barras da Rede em lisEQP
 LisBarra(lisEQP);
 //retira todas as Barras de Cortes da Rede
 for (int nc = 0; nc < lisCOR->Count; nc++)
 {
 corte = (VTCorte*)lisCOR->Items[nc];
 //obtém lista de Barras do Corte
 lisBAR_COR = corte->LisBarra();
 for (int nb = 0; nb < lisBAR_COR->Count; nb++)
 {
 lisEQP->Remove((VTBarra*)lisBAR_COR->Items[nb]);
 }
 }
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TRede::LisCanal(TList *lisEQP)
{
	// variáveis locais
	VTMedidor *medidor;
	TList *lisTMP;

	try
	{ // cria lista ltemporária
		lisTMP = new TList();
		// obtém lista de Medidores
		LisEqbar(lisTMP, eqptoMEDIDOR);
		// loop p/ todos Medidores
		for (int n = 0; n < lisTMP->Count; n++)
		{
			medidor = (VTMedidor*)lisTMP->Items[n];
			CopiaTList(medidor->LisCanal(), lisEQP);
		}
		// destrói lista temporária
		delete lisTMP;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisChave(TList *lisEQP, int estado)
{
	// variáveis locais
	VTChave *chave;
	VTLigacao *ligacao;

	// loop p/ todos objetos VTLigacao
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoCHAVE)
		{
			chave = (VTChave*)ligacao;
			if ((estado < 0) || (chave->Estado == estado))
			{ // insere Chave em lisEQP
				if (lisEQP->IndexOf(chave) < 0)
					lisEQP->Add(chave);
			}
		}
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TRede::LisCorte(void)
{
	return (lisCOR);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisCorte(TList *lisEQP)
{
	// variáveis locais
	VTCorte *corte;

	// loop p/ todos Cortes
	for (int nc = 0; nc < lisCOR->Count; nc++)
	{
		corte = (VTCorte*)lisCOR->Items[nc];
		if (lisEQP->IndexOf(corte) < 0)
			lisEQP->Add(corte);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::LisEqbar(TList *lisEQP, bool barra_isolada)
 {
 //variáveis locais
 VTBarra *barra;

 //determina objetos VTEqbar dos objetos VTBarra
 for (int nb = 0; nb < lisBAR->Count; nb++)
 {
 barra = (VTBarra*)lisBAR->Items[nb];
 barra->LisEqbar(lisEQP);
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TRede::LisEqbar(TList *lisEQP, int tipo, bool barra_isolada)
{
	// variáveis locais
	VTBarra *barra;
	TList *lisTMP;

	// verifica se deve incluir os Eqbar das Barras isoladas
	if (barra_isolada)
	{ // determina objetos VTEqbar de todos objetos VTBarra, inclusive isolada
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			barra->LisEqbar(lisEQP, tipo);
		}
	}
	else
	{ // monta lista temporária com as Barras não isoladas
		lisTMP = new TList();
		LisBarra(lisTMP, false);
		// determina objetos VTEqbar somente das Barras não isoladas
		for (int nb = 0; nb < lisTMP->Count; nb++)
		{
			barra = (VTBarra*)lisTMP->Items[nb];
			barra->LisEqbar(lisEQP, tipo);
		}
		// destrói lista temporária
		delete lisTMP;
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::LisEqpto(TList *lisEQP, bool barra_isolada)
 {
 //insere em lisEQP todos os objetos VTEqpto
 LisBarra(lisEQP);
 LisEqbar(lisEQP);
 LisLigacao(lisEQP);
 LisMutua(lisEQP);
 }

 //---------------------------------------------------------------------------
 void __fastcall TRede::LisEqpto(TList *lisEQP, int tipo, bool barra_isolada)
 {
 //verifica o tipo de eqpto solicitado
 if      (tipo <  0)            LisEqpto(lisEQP, barra_isolada);
 else if (tipo == eqptoBARRA)   LisBarra(lisEQP, barra_isolada);
 else if (tipo == eqptoEQBAR)   LisEqbar(lisEQP, barra_isolada);
 else if (tipo == eqptoLIGACAO) LisLigacao(lisEQP);
 else if (tipo == eqptoMUTUA)   LisMutua(lisEQP);
 else if (tipo == eqptoCANAL)   LisCanal(lisEQP);
 else
 {
 LisEqbar(lisEQP, tipo, barra_isolada);
 LisLigacao(lisEQP, tipo);
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TRede::LisEqpto(TList *lisEQP, int tipo, bool barra_isolada)
{
	// verifica tipo de eqpto solicitado
	if (tipo == eqptoBARRA)
		LisBarra(lisEQP, barra_isolada);
	else if (tipo == eqptoEQBAR)
		LisEqbar(lisEQP, barra_isolada);
	else if (tipo == eqptoLIGACAO)
		LisLigacao(lisEQP);
	else if (tipo == eqptoMUTUA)
		LisMutua(lisEQP);
	else if (tipo == eqptoCANAL)
		LisCanal(lisEQP);
	else if (tipo == -1)
	{ // todos os equipamentos
		LisBarra(lisEQP, barra_isolada);
		LisEqbar(lisEQP, tipo, barra_isolada);
		LisLigacao(lisEQP, tipo);
		LisMutua(lisEQP);
	}
	else
	{ // um tipo de equipamento específico (Eqbar ou Ligacao)
		LisEqbar(lisEQP, tipo, barra_isolada);
		LisLigacao(lisEQP, tipo);
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TRede::LisLigacao(void)
{
	return (lisLIG);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::LisLigacao(TList *lisEQP)
 {
 //copia lista lisLIG em lisEQP
 CopiaTList(lisLIG, lisEQP);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TRede::LisLigacao(TList *lisEQP, int tipo)
{
	// variáveis locais
	VTEqpto *eqpto;

	// verifica o tipo de objeto VTLigacao solicitado
	if ((tipo < 0) || (tipo == eqptoLIGACAO))
	{ // todos os objetos VTLigacao
		CopiaTList(lisLIG, lisEQP);
		return;
	}
	// copia somente os objetos VTLigacao do tipo solicitado
	for (int n = 0; n < lisLIG->Count; n++)
	{
		eqpto = (VTEqpto*)lisLIG->Items[n];
		if (eqpto->Tipo() == tipo)
		{ // insere objeto VTEqpto em lisEQP
			lisEQP->Add(eqpto);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisLigacao(TList *lisEQP, VTBarra *barra)
{
	// variáveis locais
	VTLigacao *ligacao;

	// loop p/ todos objetos VTLigacao
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->ConectaBarra(barra))
		{
			if (lisEQP->IndexOf(ligacao) < 0)
				lisEQP->Add(ligacao);
		}
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TRede::LisLigacaoEsquematico(void)
{
	return (esq.lisLIG);
}

// ---------------------------------------------------------------------------
TList* __fastcall TRede::LisMutua(void)
{
	return (lisMUT);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisMutua(TList *lisEQP)
{
	// copia lista lisMUT em lisEQP
	CopiaTList(lisMUT, lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisMutua(TList *lisEQP, VTTrecho *trecho)
{
	// variáveis locais
	VTMutua *mutua;

	// proteção
	if (trecho->rede != this)
		return;
	// determina Mutuas do Trecho indicado
	for (int n = 0; n < lisMUT->Count; n++)
	{
		mutua = (VTMutua*)lisMUT->Items[n];
		if ((mutua->ptre1 == trecho) || (mutua->ptre1 == trecho))
		{
			if (lisEQP->IndexOf(mutua) < 0)
				lisEQP->Add(mutua);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRede::LisRedeVizinha(TList *lisEQP)
{
	// variáveis locais
	VTCorte *corte;

	// loop p/ todos Cortes
	for (int nc = 0; nc < lisCOR->Count; nc++)
	{
		corte = (VTCorte*)lisCOR->Items[nc];
		// determina demais Redes associadas ao Corte
		corte->LisRedeVizinha(this, lisEQP);
	}
}

// ---------------------------------------------------------------------------
int __fastcall TRede::NumeroDeBarras(void)
{
	return (lisBAR->Count);
}

// ---------------------------------------------------------------------------
int __fastcall TRede::NumeroDeConsumidor(int classe_tag)
{
	// variáveis locais
	VTBarra *barra;
	VTCarga *carga;
	VTEqbar *eqbar;
	TList *lisEQB;
	int count = 0;

	// verifica se classe_tag é consNULO
	if (classe_tag == consNULO)
	{ // determina número de Cargas com consumo nulo, independente da classe
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			lisEQB = barra->LisEqbar();
			for (int n = 0; n < lisEQB->Count; n++)
			{
				eqbar = (VTEqbar*)lisEQB->Items[n];
				if (eqbar->Tipo() != eqptoCARGA)
					continue;
				carga = (VTCarga*)eqbar;
				// verifica se IP
				if (carga->IP)
				{ // verifica pot.ativa de IP
					if ((carga->IP_NumLamp == 0) || IsDoubleZero(carga->IP_Pot_kw))
						count++;
				}
				// verifica se é uma Carga típica
				else if (carga->curva->Tipica)
				{ // verifica se energia é nula
					if (IsDoubleZero(carga->Energia_kwhmes))
						count++;
				}
				else
				{ // curva própria: verifica se a integral da curva é nula
					if (IsDoubleZero(carga->curva->Integral(indP), 1e-8))
						count++;
				}
			}
		}
	}
	else
	{ // determina número de Cargas da classe indicada
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			lisEQB = barra->LisEqbar();
			for (int n = 0; n < lisEQB->Count; n++)
			{
				eqbar = (VTEqbar*)lisEQB->Items[n];
				if (eqbar->Tipo() != eqptoCARGA)
					continue;
				carga = (VTCarga*)eqbar;
				if (carga->classe->Tag == classe_tag)
				{
					count++;
				}
			}
		}
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TRede::NumeroDeCortes(void)
{
	return (lisCOR->Count);
}

// ---------------------------------------------------------------------------
int __fastcall TRede::NumeroDeEqbar(int tipo)
{
	// variáveis locais
	VTBarra *barra;
	int count = 0;

	// determina objetos VTEqbar dos objetos VTBarra
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		count += barra->NumeroDeEqbar(tipo);
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TRede::NumeroDeLigacoes(int tipo)
{
	// variáveis locais
	VTEqpto *eqpto;
	int count = 0;

	// verifica se deve retornar o número total de Ligacoes
	if (tipo < 0)
		return (lisLIG->Count);
	// identifica as Ligacoes do tipo indicado
	for (int n = 0; n < lisLIG->Count; n++)
	{
		eqpto = (VTEqpto*)lisLIG->Items[n];
		if (eqpto->Tipo() == tipo)
			count++;
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TRede::PM_GetBarraInicialId(void)
{
	return (bd.barra_id);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::PM_GetCarregada(void)
{
	return (PD.carregada);
}

// ---------------------------------------------------------------------------
VTCluster* __fastcall TRede::PM_GetCluster(void)
{
	return (cluster);
}

// ---------------------------------------------------------------------------
TColor __fastcall TRede::PM_GetColor(void)
{
	// if (PD.color != clBlack) return(PD.color);
	// return(tipo_rede->Color);
	return (PD.color);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::PM_GetEstatica(void)
{
	return (PD.estatica);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::PM_GetFluxoOK(void)
{
	return (PD.fluxoOK);
}

// ---------------------------------------------------------------------------
VTMRede* __fastcall TRede::PM_GetMRede(void)
{
	return (mrede);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRede::PM_GetPadrao(void)
{
	return (PD.padrao);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::PM_GetRadial(void)
{
	return (PD.radial);
}

// ---------------------------------------------------------------------------
VTRedes* __fastcall TRede::PM_GetRedes(void)
{
	return (redes);
}
/*
 //---------------------------------------------------------------------------
 VTRegiao* __fastcall TRede::PM_GetRegiao(void)
 {
 return(PD.regiao);
 }
 */

// ---------------------------------------------------------------------------
TDate __fastcall TRede::PM_GetRevisao(void)
{
	return (PD.revisao);
}

// ---------------------------------------------------------------------------
VTTipoRede* __fastcall TRede::PM_GetTipoRede(void)
{
	return (tipo_rede);
}

// ---------------------------------------------------------------------------
bool __fastcall TRede::PM_GetValida(void)
{
	return (PD.valida);
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetBarraInicialId(int barra_id)
{
	// salva ID da barra inicial
	bd.barra_id = barra_id;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetCarregada(bool carregada)
{
	// marca que a Rede está carrega
	PD.carregada = carregada;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetCluster(VTCluster *cluster)
{
	this->cluster = cluster;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetColor(TColor color)
{
	PD.color = color;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetEstatica(bool estatica)
{
	PD.estatica = estatica;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetFluxoOK(bool fluxoOK)
{
	PD.fluxoOK = fluxoOK;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetMRede(VTMRede *mrede)
{
	this->mrede = mrede;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetPadrao(AnsiString padrao)
{
	PD.padrao = padrao;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetRedes(VTRedes *redes)
{
	this->redes = redes;
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::PM_SetRegiao(VTRegiao *regiao)
 {
 PD.regiao = regiao;
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetRevisao(TDate revisao)
{
	PD.revisao = revisao;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetRadial(bool radial)
{
	PD.radial = radial;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetValida(bool valida)
{
	PD.valida = valida;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::PM_SetTipoRede(VTTipoRede *tipo_rede)
{
	this->tipo_rede = tipo_rede;
	// redefine cor da Rede igual à do seu TipoRede
	if ((tipo_rede) && (Color == clDefault))
		Color = tipo_rede->Color;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::ReiniciaEsquematico(VTBarra *bar_fic, TList *lisLIG_FIC)
{
	// destrói objetos usados p/ esquemático
	if (esq.pbarra)
	{
		delete esq.pbarra;
		esq.pbarra = NULL;
	}
	if (esq.lisLIG)
	{
		LimpaTList(esq.lisLIG);
	}
	// proteção
	if (bar_fic == NULL)
		return;
	// salva Barra
	esq.pbarra = bar_fic;
	// salva Ligacoes
	if (lisLIG_FIC != NULL)
		CopiaTList(lisLIG_FIC, esq.lisLIG);
	// define dimensões do retângulo p/ nome
	esq.height = 50;
	esq.width = 150;
	// redefine área esquemático da rede
	area.esq->x1 = bar_fic->esq.x1 - esq.width;
	area.esq->y1 = bar_fic->esq.y1 - esq.height;
	area.esq->x2 = bar_fic->esq.x2 + esq.width;
	area.esq->y2 = bar_fic->esq.y2 + esq.height;
	// redefine área utm da rede
	area.utm->x1 = area.utm->x2 = bar_fic->utm.x;
	area.utm->y1 = area.utm->y2 = bar_fic->utm.y;
}

// ---------------------------------------------------------------------------
void __fastcall TRede::ReiniciaLisCorte(void)
{
	lisCOR->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TRede::RemoveBarra(VTBarra *pbarra)
{
	// proteção
	if (pbarra == NULL)
		return;
	// retira objeto VTBarra da lista lisBAR
	lisBAR->Remove(pbarra);
	// verifica se é a Barra inicial
	if (pbarra == pbar_ini)
	{ // cancela Barra inicial
		pbar_ini = NULL;
	}
	// NÃO destrói objeto VTBarra
}

// ---------------------------------------------------------------------------
void __fastcall TRede::RemoveCorte(VTCorte *corte)
{
	// proteção
	if (corte == NULL)
		return;
	// retira objeto VTCorte da lista lisCOR
	lisCOR->Remove(corte);
	// NÃO destrói objeto VTCorte
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::RemoveLigacao(VTLigacao *ligacao)
 {
 //proteção
 if (ligacao == NULL) return;
 //retira objeto VTLigacao da lista lisLIG
 lisLIG->Remove(ligacao);
 //NÃO destrói objeto VTLigacao
 }
 */

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TRede::RemoveLigacao(VTLigacao *ligacao)
{
	// variáveis locais
	VTBarra *barra;

	// proteção
	if (ligacao == NULL)
		return;
	// retira objeto VTLigacao da lista lisLIG
	lisLIG->Remove(ligacao);
	// retira Ligacao nas suas Barras
	for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	{
		if ((barra = ligacao->Barra(ind_bar)) == NULL)
			continue;
		barra->RemoveLigacao(ligacao);
	}
	// NÃO destrói objeto VTLigacao
}

// ---------------------------------------------------------------------------
void __fastcall TRede::RemoveMutua(VTMutua *mutua)
{
	// proteção
	if (mutua == NULL)
		return;
	// retira objeto VTMutua da lista lisMUT
	lisMUT->Remove(mutua);
	// NÃO destrói objeto VTMutua
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TRede::RetiraLigacao(VTLigacao *ligacao)
 {
 //proteção
 if (ligacao == NULL) return;
 //retira objeto VTLigacao da lista lisLIG
 if (lisLIG->Remove(ligacao) < 0) return;
 //NÃO destrói objeto VTLigacao
 //delete ligacao;
 //salva Eqpto na Lixeira
 redes->SalvaEqptoExcluidoNaLixeira(ligacao);
 }

 //---------------------------------------------------------------------------
 void __fastcall TRede::RetiraMutua(VTMutua *mutua)
 {
 //proteção
 if (mutua == NULL) return;
 //retira objeto VTMutua da lista lisMUT
 if (lisMUT->Remove(mutua) < 0) return;
 //NÃO destrói objeto VTMutua
 //delete mutua;
 //salva Eqpto na Lixeira
 redes->SalvaEqptoExcluidoNaLixeira(mutua);
 }
 */

// ---------------------------------------------------------------------------
int __fastcall TRede::Tipo(void)
{
	return (eqptoREDE);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRede::TipoAsString(void)
{
	return ("Rede");
}

// ---------------------------------------------------------------------------
// eof
