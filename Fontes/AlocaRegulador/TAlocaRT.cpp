// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <algorithm>
#include "TAlocaRT.h"
#include "TEstoque.h"
#include "VTBanco.h"
#include "VTBancos.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Distancia\VTCaminho.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTTronco.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Distancia.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"
#pragma package(smart_init)
using namespace std;

// ---------------------------------------------------------------------------
VTAlocaRT* __fastcall NewObjAlocaRT(VTApl *apl)
{
	return (new TAlocaRT(apl));
}

// ---------------------------------------------------------------------------
__fastcall TAlocaRT::TAlocaRT(VTApl *apl)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisPri = new TList();
	lisBar = new TList();
	lisLig = new TList();
	lisEQP = new TList();
	lisEQP_ALT = new TList();
	lisEQP_INS = new TList();
	lisRegClone = new TList();
	lisTmp = new TList();
	// cria objetos locais
	bancos = NewObjBancos(apl);
	radial = DLL_NewObjRadial(apl);
	tronco = DLL_NewObjTronco(apl);
	prog = DLL_NewObjProgresso(NULL, NULL);
	// inicia os valores
	PD.v_min = 0.95;
	PD.v_alvo = 1.02;
	PD.max_reg = 2; // DVK 2014.04.03
	PD.op_tronco = true;
	PD.dist_m = 500.;
}

// ---------------------------------------------------------------------------
__fastcall TAlocaRT::~TAlocaRT(void)
{
	// destroi objetos criados
	if (bancos)
	{
		delete bancos;
		bancos = NULL;
	}
	if (prog)
	{
		delete prog;
		prog = NULL;
	}
	if (tronco)
	{
		delete tronco;
		tronco = NULL;
	}
	if (radial)
	{
		delete radial;
		radial = NULL;
	}
	// destroi listas sem destruir seus objetos
	if (lisBar)
	{
		delete lisBar;
		lisBar = NULL;
	}
	if (lisLig)
	{
		delete lisLig;
		lisLig = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisEQP_ALT)
	{
		delete lisEQP_ALT;
		lisEQP_ALT = NULL;
	}
	if (lisEQP_INS)
	{
		delete lisEQP_INS;
		lisEQP_INS = NULL;
	}
	if (lisRegClone)
	{
		delete lisRegClone;
		lisRegClone = NULL;
	}
	if (lisTmp)
	{
		delete lisTmp;
		lisTmp = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::AlocaRT(VTPrimario *primario)
{
	// var local
	bool resp = false;

	// reinicia as listas
	lisBar->Clear();
	lisLig->Clear();
	try
	{ // atualiza progresso
		prog->Add(primario->Rede->Codigo + ": análise do tronco");
		// proteção
		if (!primario->Ordena(lisBar, lisLig))
			return false;
		// determina o pico de carregamento
		this->indPatMax = DeterminaPico();
		// inicia objeto
		tronco->DefinePrimario(primario, indPatMax);
		// aloca Regulador apenas no Tronco
		resp = AlocaRTPorLigacao(primario, LisTronco(), false);
		// aloca Regulador em Ramal (ou no Tronco mais próximo do Ramal)
		prog->Add(primario->Rede->Codigo + ": análise dos ramais");
		resp = AlocaRTPorLigacao(primario, LisRamal(), PD.op_tronco);
	}
	catch (Exception &e)
	{
		return false;
	}
	return (resp);
}

// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::AlocaRTPorLigacao(VTPrimario *primario, TList *lisLigacao, bool op_tronco)
{
	// variáveis locais
	double v_pu, dv_pu;
	VTLigacao *ligacao, *lig_com_ramal;
	VTBarra *barra_out; // *barra_in;
	VTBanco *banco;

	try
	{ // determina um delta de tensão igual a um passo de um RT disponível
		dv_pu = DvPerTap();
		// percorre a lista de ligações do inicio ao fim
		for (int index = 0; index < lisLigacao->Count; index++)
		{
			ligacao = (VTLigacao*) lisLigacao->Items[index];
			// verifica limite de bancos
			if (bancos->LisBanco(ligacao->rede)->Count >= PD.max_reg)
				return true; // DVK 2014.04.02
			// não considera Chave: aloca regulador após chave
			if (ligacao->Tipo() == eqptoCHAVE)
				continue; // DVK 2013.07.23 descomentado
			// verifica há fluxo passante
			if (!ExisteFluxo(ligacao))
				continue;
			// determina as barra do sentido do fluxo
			// if ((barra_in  = BarraIn(ligacao))  == NULL) continue;
			if ((barra_out = BarraOut(ligacao)) == NULL)
				continue;
			// DVK 2013.08.01 aloca somente em redes trifásicas
			if (ligacao->Fases(barra_out) < faseABC)
				continue;
			// determina tensão minima da Barra de saída da Ligacao
			v_pu = barra_out->resflow->Vfn_pu_min[indPatMax];
			// verifica se tensão menor que o limite inferior
			if ((v_pu - dv_pu) <= PD.v_min)
			{ // verifica se a alocação deve ser apenas apenas no tronco
				if (op_tronco)
				{ // determina primeira Ligacao de tronco a montante
					ligacao = DeterminaLigTronco(ligacao);
				}
				// verifica se há ramificação da rede até N trechos a montante da Ligacao
				else if ((lig_com_ramal = ExisteLigacaoMontanteComRamal(ligacao)) != NULL)
				{
					ligacao = lig_com_ramal;
				}
				// verifica se pode alocar, dependendo da distancia minima
				if (!VerificaDistanciaMinima(ligacao))
					continue;
				// insere um novo Banco antes da Ligacao
				if ((banco = MontaNovoBanco(ligacao)) != NULL)
				{ // ajuste o mesmo fator de regulação para todas barras a jusante da ligação
					SetGanhoAjusante(banco);
				}
			}
		}
	}
	catch (Exception &e)
	{
		return false;
	}
	return true;
}

/*
 //---------------------------------------------------------------------------
 VTBarra* __fastcall TAlocaRT::BarraIn(VTLigacao *ligacao)
 {
 int index1, index2;
 //fluxo P vai de 1 - 2
 index1 = ligacao->resflow->SentidoP[0][indPatMax];  if(index1 == 1) return (ligacao->pbarra1);
 index2 = ligacao->resflow->SentidoP[1][indPatMax];  if(index2 == 1) return (ligacao->pbarra2);

 //não há fluxo
 index1 = lisBar->IndexOf(ligacao->pbarra1);
 index2 = lisBar->IndexOf(ligacao->pbarra2);

 if(index1 < index2) return(ligacao->pbarra1);
 else                return(ligacao->pbarra2);
 }

 //---------------------------------------------------------------------------
 VTBarra* __fastcall TAlocaRT::BarraOut(VTLigacao *ligacao)
 {
 int index1, index2;

 //fluxo P vai de 1 - 2
 index1 = ligacao->resflow->SentidoP[0][indPatMax];  if(index1 == 2) return (ligacao->pbarra1);
 index2 = ligacao->resflow->SentidoP[1][indPatMax];  if(index2 == 2) return (ligacao->pbarra2);

 //não há fluxo
 index1 = lisBar->IndexOf(ligacao->pbarra1);
 index2 = lisBar->IndexOf(ligacao->pbarra2);

 if(index1 > index2) return(ligacao->pbarra1);
 else                return(ligacao->pbarra2);
 }
 */
// ---------------------------------------------------------------------------
VTBarra* __fastcall TAlocaRT::BarraIn(VTLigacao *ligacao)
{
	// variáveis locais
	int index1, index2;

	// barra de entrada possui pot.ativa positiva
	if (ligacao->resflow->SentidoP[0][indPatMax] == fluxoPOS)
		return (ligacao->pbarra1);
	if (ligacao->resflow->SentidoP[1][indPatMax] == fluxoPOS)
		return (ligacao->pbarra2);

	// não há fluxo
	index1 = lisBar->IndexOf(ligacao->pbarra1);
	index2 = lisBar->IndexOf(ligacao->pbarra2);

	if (index1 < index2)
		return (ligacao->pbarra1);
	else
		return (ligacao->pbarra2);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TAlocaRT::BarraOut(VTLigacao *ligacao)
{
	int index1, index2;

	// barra de saída possui pot.ativa negativa
	if (ligacao->resflow->SentidoP[0][indPatMax] == fluxoNEG)
		return (ligacao->pbarra1);
	if (ligacao->resflow->SentidoP[1][indPatMax] == fluxoNEG)
		return (ligacao->pbarra2);

	// não há fluxo
	index1 = lisBar->IndexOf(ligacao->pbarra1);
	index2 = lisBar->IndexOf(ligacao->pbarra2);

	if (index1 > index2)
		return (ligacao->pbarra1);
	else
		return (ligacao->pbarra2);
}

// ---------------------------------------------------------------------------
void __fastcall TAlocaRT::BuscaLigacaoAjusante(VTLigacao *ligPai, TList *lisLigJus)
{
	// var local
	VTLigacao *lig;

	for (int n = 0; n < lisLig->Count; n++)
	{
		lig = (VTLigacao*) lisLig->Items[n];
		if (lig->ligaPai == ligPai)
			lisLigJus->Add(lig);
	}

}

/*
 //---------------------------------------------------------------------------
 int __fastcall TAlocaRT::ComparaSnom(void *item1, void *item2)
 {
 VTRegulador *reg1 = (VTRegulador*) item1;
 VTRegulador *reg2 = (VTRegulador*) item2;

 if(reg1->snom < reg2->snom) return ( 1);
 if(reg1->snom > reg2->snom) return (-1);
 return (0);
 }
 */
// ---------------------------------------------------------------------------
int __fastcall TAlocaRT::ComparaSnom(void *item1, void *item2)
{ // DVK 2015.07.21
	// variaveis locais
	VTRegulador *reg1, *reg2;
	TEstoque *est1 = (TEstoque*) item1;
	TEstoque *est2 = (TEstoque*) item2;

	reg1 = est1->Regulador;
	reg2 = est2->Regulador;
	if (reg1->snom < reg2->snom)
		return (1);
	if (reg1->snom > reg2->snom)
		return (-1);
	return (0);
}

// ---------------------------------------------------------------------------
void __fastcall TAlocaRT::DeleteObject(TObject *obj)
{
	if (obj)
	{
		delete obj;
		obj = NULL;
	}
}

// ---------------------------------------------------------------------------
int __fastcall TAlocaRT::DeterminaPico(void)
{
	// var local
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int num_pat = patamares->NumPatamar();
	int indPatMax = 0;
	int index;
	double smax = 0.;
	double spat;
	VTLigacao *lig_ini;

	// loop p/ todos Patamares
	for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
	{
		spat = 0.;
		// calcula valores de fluxo do patamar
		if (lisLig->Count == 0)
			return 0.;
		// verifica o fluxo na saída do alimentador
		lig_ini = (VTLigacao*) lisLig->First();
		index = IndexFluxoPositivo(lig_ini); // 0 ou 1
		// potencia aparente total
		spat = Abs(lig_ini->resflow->S_mva[index][ind_pat]);
		// salva o patamar de maior fluxo
		if (spat > smax)
		{
			indPatMax = ind_pat;
			smax = spat;
		}
	}
	return (indPatMax);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TAlocaRT::DeterminaLigTronco(VTLigacao* ligacao)
{
	// var local
	TList *lisTronco = tronco->LisLigacaoTronco();
	VTRegulador *reg;

	// proteção
	if (lisTronco->Count == 0)
		return NULL;

	// encontra a ligação pai na lista de troncos
	if (lisTronco->IndexOf(ligacao) >= 0)
		return ligacao;

	// loop p/ caminho no sentido das ligações pais
	while (ligacao->ligaPai)
	{
		// localiza a ligacao-pai
		ligacao = ligacao->ligaPai;
		if (ligacao->Tipo() == eqptoREGULADOR)
		{
			reg = (VTRegulador*)ligacao;
			if (!reg->by_pass)
				return NULL; // DVK 2013.07.23
		}
		if (ligacao->Tipo() == eqptoCHAVE)
			continue; // DVK 2013.07.23
		if (lisTronco->IndexOf(ligacao) >= 0)
			return ligacao;
	}
	// nada encontrado
	return NULL;
}

// ---------------------------------------------------------------------------
double __fastcall TAlocaRT::Distancia(TList *lisCAMINHO)
{
	// variaveis locais
	VTEqpto *eqpto;
	double dist_m = 0;

	// proteção
	if (!lisCAMINHO)
		return (0.);
	for (int ne = 0; ne < lisCAMINHO->Count; ne++)
	{ // soma distancia dos trechos
		eqpto = (VTEqpto*)lisCAMINHO->Items[ne];
		if (eqpto->Tipo() != eqptoTRECHO)
			continue;
		dist_m += ((VTTrecho*)eqpto)->Comprimento_m;
	}
	return (dist_m);
}

// ---------------------------------------------------------------------------
double __fastcall TAlocaRT::DvPerTap(void)
{
	// var local
	TEstoque *est;
	VTRegulador *reg = NULL, *tipo;
	double dv_pu = 0.005;

	// percorre a lista de modelos
	for (int n = lisTabReg->Count - 1; n >= 0; n--)
	{ // DVK 2015.07.21
		est = (TEstoque*)lisTabReg->Items[n];
		tipo = est->Regulador;
		// determina um reg. automático
		if(tipo->modelo == reguFIXO)
			continue;
		reg = tipo;
		break;
	}
	// se não tiver automático: usa o ultimo
	if (reg == NULL)
		reg = tipo;
	if (reg != NULL)
	{ // ganho por tap em pu
		if (reg->num_passo > 0)
			dv_pu = (reg->var_tensao * 0.01) / reg->num_passo;
	}
	return (dv_pu);
}

// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::Executa(TList *lisTabReg)
{
	// var local
	bool resp;
	bool flow_prog, flow_aviso;
	VTPrimario *pri;
	VTFlow *flow = (VTFlow*) apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));

	try
	{ // salva tabela de reguladores definidos
		this->lisTabReg = lisTabReg;
		// proteção
		if (!radial)
			return false;
		lisPri->Clear();
		// reinicia Bancos alocados
		bancos->Reinicia();
		// executa a montagem da rede em radiais
		if (!radial->Inicia(redes))
			return false;
		if (lisTabReg->Count > 1)
			lisTabReg->Sort(TAlocaRT::ComparaSnom);
		// ordena a lista
		lisPri = radial->LisPrimario();
		// mostra barra de progresso
		prog->Start(progTEXTO);
		prog->Add("Alocação de Reguladores");
		prog->Add("Diagnóstico da Rede");
		// salva opções do cáculo do flow
		flow_aviso = flow->AvisoEnabled;
		flow_prog = flow->ProgressoEnabled;
		// executa flow
		flow->AvisoEnabled = false;
		flow->ProgressoEnabled = false;
		// flow->TipoFluxo = fluxoAUTO;
		flow->UpToDate = false;
		if (!flow->CalculaPorRede(redes, 0))
		{
			prog->Stop();
			Aviso("Não foi possível calcular o fluxo de potência da rede");
			return false;
		}
		// restaura opções do cáculo do flow
		flow->AvisoEnabled = flow_aviso;
		flow->ProgressoEnabled = flow_prog;
		// loop p/ todos primarios
		for (int n = 0; n < lisPri->Count; n++)
		{
			pri = (VTPrimario*) lisPri->Items[n];
			// verifica se a Rede está carregada
			if (!pri->Rede->Carregada)
				continue;
			//verifica se a rede está visível (rede da obra do planejamento)
			if (!pri->Rede->Visible())
				continue;
			// aloca regulador no alimentador
			AlocaRT(pri);
		}
		// insere Reguladores alocados na Rede
		InsereBancosAlocadosNaRede();
	}
	catch (Exception &e)
	{
		prog->Stop();
		return false;
	}
	prog->Stop();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::ExisteFluxo(VTLigacao *ligacao)
{
	int index1, index2;
	// fluxo P vai de 1 - 2
	index1 = ligacao->resflow->SentidoP[0][indPatMax];
	index2 = ligacao->resflow->SentidoP[1][indPatMax];

	return (!(index1 == index2));
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TAlocaRT::ExisteLigacaoMontanteComRamal(VTLigacao *lig_ref)
{
	// var local
	VTLigacao *ligacao;
	VTLigacao *lig_pai;
	VTLigacao *lig_com_ramal = NULL;

	// determina Ligacao pai da Ligacao de referência
	lig_pai = lig_ref->ligaPai;
	// loop p/ até 3 Ligacoes a montante
	for (int n = 1; (n <= 3) && (lig_pai != NULL); n++)
	{ // verifica se há alguma Ligacao com o mesmo pai
		for (int nl = 0; nl < lisLig->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLig->Items[nl];
			// desconsidera Ligacao de referência e a Ligacao pai
			if (ligacao == lig_ref)
				continue;
			if (ligacao == lig_pai)
				continue;
			// verifica se a Ligacao tem a mesma Ligacao pai
			if (ligacao->ligaPai == lig_pai)
			{ // lig_pai possui ramal
				lig_com_ramal = lig_pai;
				break;
			}
		}
		// define próxima Ligacao de referência e sua Ligacao pai
		lig_ref = lig_pai;
		lig_pai = lig_ref->ligaPai;
	}
	return (lig_com_ramal);
}
/*
 //---------------------------------------------------------------------------
 double __fastcall TAlocaRT::Gambi_GetQinj_mvar(VTBarra *barra, int pat)
 {
 //retorna potencia reativa armazenada em ResFlowbar
 return(barra->resflow->Ia_amp[pat].imag());
 }

 //---------------------------------------------------------------------------
 void __fastcall TAlocaRT::Gambi_SetQinj_mvar(VTBarra* barra, int pat, double q_mvar)
 {
 barra->resflow->Ia_amp[pat] = complex<double>(0., q_mvar);
 }
 */

// ---------------------------------------------------------------------------
int __fastcall TAlocaRT::IndexFluxoPositivo(VTLigacao *ligacao)
{
	VTBarra *barra = BarraIn(ligacao);

	// barra 1
	if (barra == ligacao->pbarra1)
		return 0;
	// barra 2
	return (1);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TAlocaRT::InsereBancosAlocadosNaRede(void)
 {
 //variáveis locais
 VTBanco     *banco;
 VTBarra     *barra;
 VTLigacao   *ligacao, *ligacao_alt;
 VTRegulador *regulador;
 VTEdita     *edita    = (VTEdita*)apl->GetObject(__classid(VTEdita));
 TList       *lisBANCO = bancos->LisBanco();

 //limpa listas clone   //DVK 2013.07.23
 lisBarClone->Clear();
 lisRegClone->Clear();
 lisLigOrig->Clear();
 //montas lista de Barras novas, reguladores alocados e Ligacoes pai
 for (int n = 0; n < lisBANCO->Count; n++)
 {
 banco     = (VTBanco*)lisBANCO->Items[n];
 //cria clone do Regulador
 regulador                  = banco->Regulador->Clone();   //DVK 2013.07.23
 lisRegClone->Add(regulador);                              //DVK 2013.07.23
 //regulador->Status[sttNOVO] = true;
 //cria clone da Barra
 barra                  = banco->BarraNova->Clone();   //DVK 2013.07.23
 barra->Codigo          = barra->Codigo+"_1";          //DVK 2013.07.23
 lisBarClone->Add(barra);                              //DVK 2013.07.23
 //barra->Status[sttNOVO] = true;
 //configura Regulador
 regulador->Codigo          = "Reg_"+IntToStr(n+1);   //DVK 2014.04.03
 regulador->rede            = banco->LigacaoPai->rede;
 regulador->pbarra1         = barra;
 regulador->pbarra2         = banco->BarraRede;
 regulador->sentido.bar_ref = barra;
 //verifica se o Regulador deve ser configurado em modo automático
 if (regulador->ajuste_auto)
 {
 regulador->cfg_auto.v_pu    = PD.v_alvo;
 regulador->cfg_auto.bar_ref = banco->BarraRede;
 }
 //insere Barra nova na rede
 edita->InsereBarra(barra);
 //insere Regulador na Rede
 edita->InsereLigacao(regulador, regulador->rede);
 //altera Ligacao pai
 ligacao     = banco->LigacaoPai;
 ligacao_alt = ligacao->Clone();
 lisLigOrig->Add(ligacao);     //DVK 2013.07.23
 if (ligacao_alt->pbarra1 == banco->BarraRede) ligacao_alt->pbarra1 = barra;
 else                                          ligacao_alt->pbarra2 = barra;
 edita->Altera(ligacao, ligacao_alt);
 }
 return(true);
 }
 */

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18127942
bool __fastcall TAlocaRT::InsereBancosAlocadosNaRede(void)
{
	// variáveis locais
	VTBanco *banco;
	VTBarra *barra;
	VTLigacao *ligacao, *ligacao_alt;
	VTRegulador *regulador, *reg_pai;
	VTTrafo *trafo_pai;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	TList *lisBANCO = bancos->LisBanco();

	// limpa listas sem destruir objetos
	lisEQP_ALT->Clear();
	lisEQP_INS->Clear();
	lisRegClone->Clear();
	// montas lista de Eqptos novos (Barras e reguladores) e lista de Eqptos alterados (Ligacoes pai)
	for (int n = 0; n < lisBANCO->Count; n++)
	{
		banco = (VTBanco*)lisBANCO->Items[n];
		// cria clone do Regulador
		regulador = banco->Regulador->Clone(); // DVK 2013.07.23
		regulador->Id = FalsoId(); // DVK 2015.09.16
		lisRegClone->Add(regulador); // DVK 2013.07.23
		// regulador->Status[sttNOVO] = true;
		// cria clone da Barra
		barra = banco->BarraNova->Clone(); // DVK 2013.07.23
        barra->LisLigacao()->Clear();
		barra->Id = FalsoId(); // DVK 2015.09.16
		barra->Codigo = barra->Codigo + "_1"; // DVK 2013.07.23
		// barra->Status[sttNOVO] = true;
		// configura Regulador
		regulador->Codigo = "Reg_" + IntToStr(n + 1); // DVK 2014.04.03
		regulador->rede = banco->LigacaoPai->rede;
		regulador->DefineObjBarra(barra, banco->BarraRede);
//		regulador->pbarra1 = barra;
//		regulador->pbarra2 = banco->BarraRede;
		regulador->sentido.bar_ref = barra;
		// verifica se o Regulador deve ser configurado em modo automático
		if (PD.tipo_ajuste == 2)  //barra fixa
		{
			regulador->modelo      = reguAUTO_FIXA;
//			regulador->cfg_auto.v_pu = PD.v_alvo;
//			regulador->cfg_auto.vinv_pu = PD.v_alvo;
//			regulador->cfg_auto.bar_ref = banco->BarraRede;
//			regulador->cfg_auto.bar_ref_inv = banco->BarraRede;
//			regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
//			regulador->cfg_auto.larg_bandamorta_pu = 0;
		}
		if (PD.tipo_ajuste == 3)  //unidirecional
		{
			regulador->modelo      = reguAUTO_UNI;
//			regulador->cfg_auto.v_pu = PD.v_alvo;
//			regulador->cfg_auto.vinv_pu = PD.v_alvo;
//			regulador->cfg_auto.bar_ref = banco->BarraRede;
//			regulador->cfg_auto.bar_ref_inv = banco->BarraRede;
//			regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
//			regulador->cfg_auto.larg_bandamorta_pu = 0;
		}
		regulador->cfg_auto.v_pu = PD.v_alvo;
		regulador->cfg_auto.vinv_pu = PD.v_alvo;
		regulador->cfg_auto.bar_ref = banco->BarraRede;
		regulador->cfg_auto.bar_ref_inv = banco->BarraRede;
		regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
		regulador->cfg_auto.larg_bandamorta_pu = 0;
		// altera Ligacao pai
		ligacao = banco->LigacaoPai;
		ligacao_alt = ligacao->Clone();
		if (ligacao_alt->pbarra1 == banco->BarraRede)
		{
//			ligacao_alt->pbarra1 = barra;
			if (ligacao_alt->pbarra2 != NULL)
				{ligacao_alt->pbarra2->LisLigacao()->Clear();}
			if (ligacao_alt->pbarra3 != NULL)
				{ligacao_alt->pbarra3->LisLigacao()->Clear();}
			ligacao_alt->DefineObjBarra(barra, ligacao_alt->pbarra2, ligacao_alt->pbarra3);
		}
		else
		{
//			ligacao_alt->pbarra2 = barra;
			if (ligacao_alt->pbarra1 != NULL)
                {ligacao_alt->pbarra1->LisLigacao()->Clear();}
			if (ligacao_alt->pbarra3 != NULL)
				{ligacao_alt->pbarra3->LisLigacao()->Clear();}
			ligacao_alt->DefineObjBarra(ligacao_alt->pbarra1, barra, ligacao_alt->pbarra3);
		}
		// verifica se a Ligacao é um Regulador
		if (ligacao_alt->Tipo() == eqptoREGULADOR)
		{
			reg_pai = (VTRegulador*)ligacao_alt;
			// verifica se a Barra original também é de referência para ajuste automático
			if (reg_pai->cfg_auto.bar_ref == banco->BarraRede)
			{ // redefine Barra de referência
				reg_pai->cfg_auto.bar_ref = barra;
			}
			// verifica se a Barra original também é de referência para o sentido do fluxo
			if (reg_pai->sentido.bar_ref == banco->BarraRede)
			{ // redefine Barra de referência
				reg_pai->sentido.bar_ref = barra;
			}
		}
		// verifica se a Ligacao é um Trafo
		else if (ligacao_alt->Tipo() == eqptoTRAFO)
		{ // verifica se a Barra desligada era de referência
			trafo_pai = (VTTrafo*)ligacao_alt;
			if (trafo_pai->ltc.ajuste_auto.bar_ref == banco->BarraRede)
			{ // redefine Barra de referência
				trafo_pai->ltc.ajuste_auto.bar_ref = barra;
			}
		}
		// insere Barra e Regulador em lisEQP_NOVO
		lisEQP_INS->Add(barra);
		lisEQP_INS->Add(regulador);
		// insere Ligacao que será alterada em lisEQP_ALT
		ligacao->Obj = ligacao_alt;
		lisEQP_ALT->Add(ligacao);
	}
	// executa Edita para incluir e alterar Eqptos
	edita->InsereAlteraRetiraLisEqpto(lisEQP_INS, lisEQP_ALT, NULL);
	return (true);
}

// ---------------------------------------------------------------------------
TList* __fastcall TAlocaRT::LisRamal(void)
{
	// variaveis locais
	TList *lisTronco;
	VTLigacao *lig;

	// reinicia lista
	lisTmp->Clear();
	// copia a lista ordenada
	CopiaTList(lisLig, lisTmp);
	// remove os troncos
	lisTronco = tronco->LisLigacaoTronco();
	for (int i = 0; i < lisTronco->Count; i++)
	{
		lig = (VTLigacao*) lisTronco->Items[i];
		lisTmp->Remove(lig);
	}
	return (lisTmp);
}

// ---------------------------------------------------------------------------
int __fastcall TAlocaRT::ListaReguladorAlocado(TList *lisEXT)
{
	// copia Reguladores dos Bancos
	// return(bancos->LisRegulador(lisEXT));
	CopiaTList(lisRegClone, lisEXT); // DVK 2013.07.23
	return (lisEXT->Count); // DVK 2013.07.23
}

// ---------------------------------------------------------------------------
TList* __fastcall TAlocaRT::LisTronco(void)
{
	// variaveis locais
	TList *lisRamal;
	VTLigacao *lig;
	// reinicia lista
	lisTmp->Clear();
	// copia a lista ordenada
	CopiaTList(lisLig, lisTmp);

	lisRamal = tronco->LisLigacaoRamal();
	for (int i = 0; i < lisRamal->Count; i++)
	{
		lig = (VTLigacao*) lisRamal->Items[i];
		lisTmp->Remove(lig); // remove os ramais
	}
	return (lisTmp);
	// return (tronco->LisLigacaoTronco());
}

// ---------------------------------------------------------------------------
VTBanco* __fastcall TAlocaRT::MontaNovoBanco(VTLigacao *ligacao)
{
	// variaveis locais
	int ind_bar;
	double s_mva;
	double v_pu;
	VTBanco *banco;
	VTRegulador *tipo;
	VTBarra *barra_in, *barra_out;

	// verifica se já existe um Banco na mesma Ligacao
	if (bancos->ExisteBanco(ligacao) != NULL)
		return (NULL);
	// determina a barra de entrada do fluxo
	barra_in = BarraIn(ligacao);
	barra_out = BarraOut(ligacao);
	// determina a potencia aparente passante na ligação
	ind_bar = ligacao->IndiceBarra(barra_in);
	s_mva = Abs(ligacao->resflow->S_mva[ind_bar][indPatMax]);
	// determina tensão minima da Barra de saída da Ligacao
	v_pu = barra_out->resflow->Vfn_pu_min[indPatMax];
	// determina modelo de Regulador, pela pot.nominal
	if ((tipo = SelecionaModeloRegulador(s_mva)) == NULL)
		return (NULL);
	// cria um novo Banco
	banco = bancos->InsereBanco();
	// cria um novo regulador
	banco->Regulador = tipo->Clone();
	banco->Regulador->rede = ligacao->rede; // DVK 2014.04.03
	banco->LigacaoPai = ligacao;
	banco->Regulador->ligaPai = ligacao;
	banco->BarraRede = barra_out;
	// cria uma nova Barra
	banco->BarraNova = barra_out->Clone();
	banco->BarraNova->Id = FalsoId();
	banco->DefineCoordenadasBarraNova();
	// define passo adequado p/ ajuste fixo e determina fator de ganho
	banco->FatorAjuste = SetTapAdequado(banco->Regulador, v_pu);
	return (banco);
}

// ---------------------------------------------------------------------------
void __fastcall TAlocaRT::MoveLigacoesB1B2(TList *lisLig, VTBarra *b_orig, VTBarra *b_dest)
{ // DVK 2014.02.11
	// variaveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTLigacao *ligacao;

	for (int nl = 0; nl < lisLig->Count; nl++)
	{ // troca todas as ligacoes para a barra b_dest
		ligacao = (VTLigacao*)lisLig->Items[nl];
		edita->TrocaBarraLigacao(ligacao, b_orig, b_dest);
	}
}

/*
 //---------------------------------------------------------------------------
 int __fastcall TAlocaRT::PM_GetMAX_REG(void)
 {//DVK 2014.04.02
 return this->max_reg;
 }

 //---------------------------------------------------------------------------
 bool __fastcall TAlocaRT::PM_GetOP_TRONCO(void)
 {
 return this->op_tronco;
 }

 //---------------------------------------------------------------------------
 double __fastcall TAlocaRT::PM_GetV_ALVO(void)
 {
 return this->v_alvo;
 }

 //---------------------------------------------------------------------------
 double  __fastcall TAlocaRT::PM_GetV_MIN(void)
 {
 return this->v_min;
 }

 //---------------------------------------------------------------------------
 void  __fastcall TAlocaRT::PM_SetMAX_REG(int value)
 {//DVK 2014.04.02
 this->max_reg = value;
 }

 //---------------------------------------------------------------------------
 void  __fastcall TAlocaRT::PM_SetOP_TRONCO(bool value)
 {
 this->op_tronco = value;
 }

 //---------------------------------------------------------------------------
 void  __fastcall TAlocaRT::PM_SetV_ALVO(double value)
 {
 this->v_alvo = value;
 }

 //---------------------------------------------------------------------------
 void  __fastcall TAlocaRT::PM_SetV_MIN(double value)
 {
 this->v_min = value;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TAlocaRT::Reinicia(void)
{
	// limpa listas clone
	lisEQP_ALT->Clear();
	lisEQP_INS->Clear();
	lisRegClone->Clear(); // DVK 2015.09.16
	// reinicia Bancos
	bancos->Reinicia();
}

// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::RetiraBancosAlocadosDaRede(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// cancela todas alterações na Rede
	edita->UndoCompleto();
	edita->Clear();

	return (true);
}
 /*
// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::RetiraBancosAlocadosDaRede(TList *lisReg)
{ // DVK 2014.02.11
	// variáveis locais
	VTBanco *banco;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRegulador *reg;
	TList *lisLig = new TList();

	for (int nr = 0; nr < lisReg->Count; nr++)
	{ // transfere ligaçoes a jusante de cada regulador para montante
		reg = (VTRegulador*)lisReg->Items[nr];
		lisLig->Clear();
		redes->LisLigacao(lisLig, reg->pbarra1);
		lisLig->Remove(reg);
		MoveLigacoesB1B2(lisLig, reg->pbarra1, reg->pbarra2);
		// retira da lista
		lisRegClone->Remove(reg);
		// retira banco de reguladores
		banco = bancos->ExisteBanco(reg->ligaPai);
		if (banco != NULL)
			bancos->LisBanco()->Remove(banco);
	}
	// apaga reguladores e barras isoladas
	edita->RetiraLisEqpto(lisReg);
	edita->RetiraLisEqpto(redes->LisBarraIsolada());
	// destroi lista
	if (lisLig)
		delete lisLig;
	return (true);
}
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
bool __fastcall TAlocaRT::RetiraBancosAlocadosDaRede(TList *lisReg, bool remove_banco)
{ // DVK 2014.02.11
    // variáveis locais
    VTBanco *banco;
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTRegulador *reg;
    TList *lisLig;

    for (int nr = 0; nr < lisReg->Count; nr++)
    { // transfere ligaçoes a jusante de cada regulador para montante
        reg = (VTRegulador*)lisReg->Items[nr];
        lisLig = reg->pbarra2->LisLigacao();
        lisLig->Remove(reg);
        lisLig = reg->pbarra1->LisLigacao();
        lisLig->Remove(reg);
        MoveLigacoesB1B2(lisLig, reg->pbarra1, reg->pbarra2);
        // retira da lista
        lisRegClone->Remove(reg);
        if(remove_banco)
        {// retira banco de reguladores
            banco = bancos->ExisteBanco(reg->ligaPai);
            if (banco != NULL)
                bancos->LisBanco()->Remove(banco);
        }
    }
    // apaga reguladores e barras isoladas
    edita->RetiraLisEqpto(lisReg);
    edita->RetiraLisEqpto(redes->LisBarraIsolada());
    return (true);
}

// ---------------------------------------------------------------------------
VTRegulador* __fastcall TAlocaRT::SelecionaModeloRegulador(double s_mva)
{
	// variaveis locais
	TEstoque *est;
	VTRegulador *tipo;

	// percorre a lista de trás pra frente
	for (int n = lisTabReg->Count - 1; n >= 0; n--)
	{ // DVK 2015.07.21
		est = (TEstoque*)lisTabReg->Items[n];
		tipo = est->Regulador;
		if ((tipo->Spas_mva >= s_mva) && (est->Unidades > 0))
		{ // verifica se ainda tem no estoque e decrementa //DVK 2015.07.21
			est->Unidades--;
			return (tipo);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TAlocaRT::SetGanhoAjusante(VTBanco *banco)
{
	// variáveis locais
	VTBarra *barra;
	VTLigacao *lig;
	TList *lisLigJus, *lisBarJus;

	try
	{ // cria listas locais
		lisLigJus = new TList();
		lisBarJus = new TList();
		// inicia lista de Ligacoes a jusante a partir da ligação indicada (sem inclui-la)
		BuscaLigacaoAjusante(banco->LigacaoPai, lisLigJus);
		// busca demais Ligacoes a jusante
		for (int n = 0; n < lisLigJus->Count; n++)
		{
			lig = (VTLigacao*) lisLigJus->Items[n];
			BuscaLigacaoAjusante(lig, lisLigJus);
		}
		// monta de lista de Barras cujas tensões serão ajustadas
		// lisBarJus->Add(banco->BarraRede);
		for (int n = 0; n < lisLigJus->Count; n++)
		{
			lig = (VTLigacao*) lisLigJus->Items[n];
			if (!ExisteFluxo(lig))
				continue;
			// inclui Barra de saída da Ligacao
			if ((barra = BarraOut(lig)) == NULL)
				continue;
			// teste
			// if (barra == banco->BarraRede)
			// {
			// int a=0;
			// continue;
			// }
			lisBarJus->Add(barra);
		}
		// multiplica a tensão de todas barras a jusante pelo mesmo ganho
		for (int n = 0; n < lisBarJus->Count; n++)
		{
			barra = (VTBarra*) lisBarJus->Items[n];
			// seta o ganho de tensão na barra
			barra->resflow->Van_pu[indPatMax] =
				banco->FatorAjuste * barra->resflow->Van_pu[indPatMax];
			barra->resflow->Vbn_pu[indPatMax] =
				banco->FatorAjuste * barra->resflow->Vbn_pu[indPatMax];
			barra->resflow->Vcn_pu[indPatMax] =
				banco->FatorAjuste * barra->resflow->Vcn_pu[indPatMax];
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói listas locais
	if (lisBarJus)
	{
		delete lisBarJus;
		lisBarJus = NULL;
	}
	if (lisLigJus)
	{
		delete lisLigJus;
		lisLigJus = NULL;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TAlocaRT::SetTapAdequado(VTRegulador *reg, double vpu)
{
	// var local
	double dv_pu = reg->var_tensao / reg->num_passo / 100.; // ganho/tap
	double ganho = 1.;
	double erro, erro_min = 1.;
	int tap;

	// verifica se a opção é por tap fixo pré-definido
	if (PD.tipo_ajuste == 0)
	{ // assume tap pré-definido
		tap = PD.tap_fixo;
	}
	else
	{ // define valor do tap fixo mais adequado
		for (int index = 0; index < reg->num_passo; index++)
		{
			// calcula a nova tensão com elevação do tap
			ganho += dv_pu;
			// erro
			erro = fabs(ganho * vpu - PD.v_alvo);
			// salva menor erro encontrado
			if (erro < erro_min)
			{
				erro_min = erro;
				tap = index + 1;
			}
		}
	}
	// salva ajuste de tap
	reg->cfg_fixo.passo = tap;

	return (1. + tap * dv_pu); // valor absoluto
}

// ---------------------------------------------------------------------------
bool __fastcall TAlocaRT::VerificaDistanciaMinima(VTLigacao *ligacao)
{
	// variaveis locais
	double dist_m;
	TList *lisBANCO, *lisCAMINHO;
	VTBanco *banco;
	VTCaminho *caminho;
	VTLigacao *liga_banco;

	// proteção
	if (!ligacao)
		return (true);
	// verifica se opção de distância mínima está habilitada
	if (!PD.dist_definido)
		return (true);
	// cria obj caminho e lista
	caminho = DLL_NewObjCaminho(apl);
	lisCAMINHO = new TList();
	// loop p/ todos Bancos alocados
	lisBANCO = bancos->LisBanco(ligacao->rede);
	for (int n = 0; n < lisBANCO->Count; n++)
	{
		banco = (VTBanco*)lisBANCO->Items[n];
		// reinicia distancia entre o banco alocado e a ligacao
		dist_m = 0;
		liga_banco = banco->LigacaoPai;
		lisCAMINHO->Clear();
		// verifica distancia
		caminho->Executa(ligacao, liga_banco, lisCAMINHO);
		dist_m = Distancia(lisCAMINHO);
		// if(IsDoubleZero(dist_m)) continue;
		if (dist_m < PD.dist_m)
		{ // nao pode alocar: destroi obj e lista
			delete caminho;
			delete lisCAMINHO;
			return (false);
		}
	}
	// pode alocar
	delete caminho;
	delete lisCAMINHO;
	return (true);
}

// ---------------------------------------------------------------------------
// eof
