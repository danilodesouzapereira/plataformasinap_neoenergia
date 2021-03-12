// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TAjustaEnergia.h"
#include "VTResumo.h"
#include "VTResumos.h"
#include "..\Medicao\TMedicao.h"
#include "..\Medicao\VTMedicoes.h"
#include "..\RedePT\VTRedePT.h"
#include "..\RedePT\VTMontaRedePT.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Curvas\VTCurva.h"
// #include "..\..\Flow\VTFlow.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTResFlowSup.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Radial.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTAjustaEnergia* __fastcall NewObjAjustaEnergia(VTApl *apl_owner)
{
	try
	{ // cria objeto TAjustaEnergia
		return (new TAjustaEnergia(apl_owner));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAjustaEnergia::TAjustaEnergia(VTApl *apl_owner)
{
	// salva ponteiro p/ objetos externos
	this->apl = apl_owner;
	medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	// cria listas
	lisEQP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TAjustaEnergia::~TAjustaEnergia(void)
{
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

/*
 //---------------------------------------------------------------------------
 double __fastcall TAjustaEnergia::EnergiaCarga_mwhmes(TList *lisCARGA)
 {
 //variáveis locais
 double    energia_mwhmes = 0;
 VTCarga   *carga;
 VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

 //loop para todas Cargas da lista
 for (int n = 0; n < lisCARGA->Count; n++)
 {
 carga = (VTCarga*)lisCARGA->Items[n];
 //verifica se Carga de IP
 if (carga->classe->Tag == consIP)
 {
 energia_mwhmes += demanda->Edia_mwh[carga] * num_dias;
 }
 else
 {
 energia_mwhmes += carga->Energia_mwhmes;
 }
 }
 return(energia_mwhmes);
 }
 */
// ---------------------------------------------------------------------------
double __fastcall TAjustaEnergia::EnergiaCarga_mwhmes(TList *lisCARGA)
{
	// variáveis locais
	double energia_mwhmes = 0;
	VTCarga *carga;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	// loop para todas Cargas da lista
	for (int n = 0; n < lisCARGA->Count; n++)
	{
		carga = (VTCarga*)lisCARGA->Items[n];
		energia_mwhmes += demanda->Edia_mwh[carga] * num_dias;
	}
	return (energia_mwhmes);
}
// ---------------------------------------------------------------------------
// double __fastcall TAjustaEnergia::EnergiaCarga_mwhmes(TList *lisCARGA)
// {	//variáveis locais
// VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
// complex<double>s_mva_total;
// int num_pat = patamares->NumPatamar();
// double    energia_mwhmes = 0;
// VTCarga   *carga;
// VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
//
// // zera valor total
// s_mva_total = CZero();
// //loop para todas Cargas da lista
// for (int n = 0; n < lisCARGA->Count; n++)
// {
// carga = (VTCarga*)lisCARGA->Items[n];
// for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
// {
// s_mva_total += demanda->S_mva[carga][ind_pat];
// }
// }
// energia_mwhmes = s_mva_total.real() * num_dias;
// return(energia_mwhmes);
// }

// ---------------------------------------------------------------------------
double __fastcall TAjustaEnergia::EnergiaReducoesFilhas_mwhmes(TList *lisRedes)
{
	return 0.;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16456899
// https://sinapsisenergia.teamwork.com/#tasks/16547266
double __fastcall TAjustaEnergia::ExecutaAjusteComPerda(int dias_mes)
{
	// variáveis locais
	struct
	{
		double ajus_mwhmes, dif_mwhmes;
	} energia_bt, energia_mt;
	struct
	{
		double med_mwhmes, inj_mwhmes, dif_mwhmes;
	} energia_tot;
	double fatmult_bt, fatmult_mt; // , fatmult_max = 1.0;
	double fatmult_max = 1.0;
	TList *lisMEDICAO;
	TMedicao *medicao;
	VTResumo *resumo;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	// salva número de dias do mês
	this->num_dias = dias_mes;
	// obtém lista ordenada de Medições
	lisMEDICAO = medicoes->OrdenaLisMedicao();
	// loop para todas Medicoes, em ordem inversa para que as Cargas das Medicoes
	// a jusante sejam corrigidas antes das Cargas das Medicoes a montante
	for (int n = lisMEDICAO->Count - 1; n >= 0; n--)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		// JCG 2016.02.20 reinicia lista de Cargas fixas a ajustáveis verificando se
		// o fluxo foi executado para cada uma das redes
		IniciaLisCargaAjustaveisFixas(medicao, true);
		// JCG 2016.02.20  verifica se há energia ajustável na sdbt/sdmt e, se necessário,
		// rededine distribuição das perdas comerciais
		ValidaDistribuicaoPerdaComercial(medicao);
		// determina Balanco da Rede da Medicao
		if ((resumo = resumos->ExisteResumo(medicao->rede)) != NULL)
		{ // determina energia mensal medida, calculada e sua diferença
			energia_tot.med_mwhmes = medicao->energia.medida_mwhmes;
			energia_tot.inj_mwhmes =
				(resumo->balanco.EnergiaSuprimento + resumo->balanco.EnergiaRecebidaExt +
				resumo->balanco.EnergiaRecebidaInt) * num_dias;
			energia_tot.dif_mwhmes = energia_tot.med_mwhmes - energia_tot.inj_mwhmes;
			// determina parcela da diferença de energia que deve ser acrescida na MT e na BT
			energia_bt.dif_mwhmes =
				energia_tot.dif_mwhmes * medicao->alocacao_perda_comercial.sdbt_pu;
			energia_mt.dif_mwhmes =
				energia_tot.dif_mwhmes * medicao->alocacao_perda_comercial.sdmt_pu;
			// determina energia mensal das Cargas BT ajustáveis
			energia_bt.ajus_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS);
			// correção das Cargas BT
			if ((!IsDoubleZero(energia_bt.ajus_mwhmes)) && (!IsDoubleZero(energia_bt.dif_mwhmes)))
			{ // determina fator de correção das Cargas BT ajustáveis
				fatmult_bt = 1.0 + (energia_bt.dif_mwhmes / energia_bt.ajus_mwhmes);
				// guarda o fator
				medicao->fator.bt = fatmult_bt;
				// corrige energia das Cargas BT_ajustáveis
				RedefineEnergiaCarga(medicao->lisCAR_BT_AJUS, fatmult_bt);

				// verifica se fator é menor que 0.9
				if (fatmult_bt < 0.9)
				{ // alterando o fator para ser maior que 1, o processo repete até que esteja entre 0.9 e 1
					fatmult_bt = 1.0 + (1.0 - fatmult_bt);
				}

				// salva maior fator de correção
				if (fatmult_bt > fatmult_max)
					fatmult_max = fatmult_bt;
			}
			// determina energia mensal das Cargas MT ajustáveis
			energia_mt.ajus_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_MT_AJUS);
			// correção das Cargas MT
			if ((!IsDoubleZero(energia_mt.ajus_mwhmes)) && (!IsDoubleZero(energia_mt.dif_mwhmes)))
			{ // determina fator de correção das Cargas MT ajustáveis
				fatmult_mt = 1.0 + (energia_mt.dif_mwhmes / energia_mt.ajus_mwhmes);
				// guarda o fator
				medicao->fator.mt = fatmult_mt;
				// corrige energia das Cargas MT_ajustáveis
				RedefineEnergiaCarga(medicao->lisCAR_MT_AJUS, fatmult_mt);

				// verifica se fator é menor que 0.9
				if (fatmult_mt < 0.9)
				{ // alterando o fator para ser maior que 1, o processo repete até que esteja entre 0.9 e 1
					fatmult_mt = 1.0 + (1.0 - fatmult_mt);
				}

				// salva maior fator de correção
				if (fatmult_mt > fatmult_max)
					fatmult_max = fatmult_mt;
			}
		}
	}
	return (fabs(fatmult_max - 1.0));
}

// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::ExecutaAjusteSemPerda(int dias_mes)
{
	// variáveis locais
	struct
	{
		double ajus_mwhmes, fixa_mwhmes, dif_mwhmes;
	} energia_bt, energia_mt;
	struct
	{
		double med_mwhmes, car_mwhmes, dif_mwhmes;
	} energia_tot;

	double fatmult_bt, fatmult_mt;
	TMedicao *medicao;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	// salva número de dias do mês
	this->num_dias = dias_mes;
	// loop para todas Medicao
	for (int n = 0; n < lisMEDICAO->Count; n++)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		// determina energia mensal das Cargas BT fixas, ajustáveis
		energia_bt.ajus_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS);
		energia_bt.fixa_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_FIXA);
		// determina energia mensal das Cargas MT fixas, ajustáveis
		energia_mt.ajus_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_MT_AJUS);
		energia_mt.fixa_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_MT_FIXA);
		// determina energia mensal medida, das Cargas e sua diferença
		energia_tot.med_mwhmes = medicao->energia.medida_mwhmes;
		energia_tot.car_mwhmes = energia_bt.ajus_mwhmes + energia_bt.fixa_mwhmes +
			energia_mt.ajus_mwhmes + energia_mt.fixa_mwhmes;
		energia_tot.dif_mwhmes = energia_tot.med_mwhmes - energia_tot.car_mwhmes;
		// determina parcela da diferença de energia que deve ser acrescida na MT e na BT
		energia_bt.dif_mwhmes = energia_tot.dif_mwhmes * medicao->alocacao_perda_comercial.sdbt_pu;
		energia_mt.dif_mwhmes = energia_tot.dif_mwhmes * medicao->alocacao_perda_comercial.sdmt_pu;
		// correção das Cargas BT
		if (!IsDoubleZero(energia_bt.ajus_mwhmes))
		{ // determina fator de correção das Cargas BT ajustáveis
			fatmult_bt = 1.0 + (energia_bt.dif_mwhmes / energia_bt.ajus_mwhmes);
			// acrescimo para considerar perdas
			fatmult_bt += 0.02; // 0.06;
			// corrige energia das Cargas BT_ajustáveis
			RedefineEnergiaCarga(medicao->lisCAR_BT_AJUS, fatmult_bt);
		}
		// correção das Cargas MT
		if (!IsDoubleZero(energia_mt.ajus_mwhmes))
		{ // determina fator de correção das Cargas MT ajustáveis
			fatmult_mt = 1.0 + (energia_mt.dif_mwhmes / energia_mt.ajus_mwhmes);
			// acrescimo para considerar perdas
			fatmult_mt += 0.02; // 0.06;
			// corrige energia das Cargas MT_ajustáveis
			RedefineEnergiaCarga(medicao->lisCAR_MT_AJUS, fatmult_mt);
		}
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TAjustaEnergia::IniciaAllMedicao(int dias_mes, bool divide_perda_comercial)
 {
 //variáveis locais
 TMedicao   *medicao;
 VTArvore   *arvore     = (VTArvore*)apl->GetObject(__classid(VTArvore));
 VTRedes    *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTMedicoes *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
 TList      *lisMEDICAO = medicoes->LisMedicao();

 //salva número de dias do mês
 this->num_dias = dias_mes;
 //reinicia Arvore
 arvore->Inicia(redes);
 //loop para todas Medicao
 for (int n = 0; n < lisMEDICAO->Count; n++)
 {
 medicao = (TMedicao*)lisMEDICAO->Items[n];
 //inicia Redes e Cargas da Medicao
 IniciaUmaMedicao(num_dias, medicao);
 //verifica se deve iniciar divisão da perda comercial
 if (divide_perda_comercial)
 {
 IniciaDivisaoPerdaComercial(medicao);
 }
 }
 return(true);
 }
 */ // DVK 2016.04.18 se a medição nao for válida, descarta
// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::IniciaAllMedicao(int dias_mes, bool divide_perda_comercial)
{
	// variáveis locais
	TMedicao *medicao;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	// salva número de dias do mês
	this->num_dias = dias_mes;
	// reinicia Arvore
	arvore->Inicia(redes);
	// loop para todas Medicao
	for (int n = lisMEDICAO->Count - 1; n >= 0; n--)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		// inicia Redes e Cargas da Medicao
		IniciaUmaMedicao(num_dias, medicao);
		// valida medição
		if (!ValidaMedicao(medicao))
		{
			// {//retira da lista e destroi medicao
			// lisMEDICAO->Delete(n);
			// delete medicao;
			medicao->valida = false;
			continue;
		}
		// verifica se deve iniciar divisão da perda comercial
		if (divide_perda_comercial)
		{
			IniciaDivisaoPerdaComercial(medicao);
		}
		else
		{
			if (!medicao->alocacao_perda_comercial.valida)
			{
				IniciaDivisaoPerdaComercial(medicao);
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16547266
void __fastcall TAjustaEnergia::IniciaLisCargaAjustaveisFixas(TMedicao *medicao, bool pos_fluxo)
{
	// variáveis locais
	VTCarga *carga;
	VTRede *rede;
	VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));
	VTRedePT *redePT;

	// reinicia lista de Cargas da Medicao
	medicao->lisCAR_BT_AJUS->Clear();
	medicao->lisCAR_BT_FIXA->Clear();
	medicao->lisCAR_MT_AJUS->Clear();
	medicao->lisCAR_MT_FIXA->Clear();
	// salva Cargas das Redes fixas
	for (int nr = 0; nr < medicao->lisREDE_FIXA->Count; nr++)
	{
		rede = (VTRede*)medicao->lisREDE_FIXA->Items[nr];
		// verifica se deve verificar se o fluxo foi executado para a rede
		if (pos_fluxo && (!rede->FluxoOK))
			continue;
		// verifica se rede primária ou secundária
		if (rede->TipoRede->Segmento == redeSEC)
		{ // Rede BT: insere Cargas fixas da rede secundária lisCAR_BT_FIXA
			rede->LisEqbar(medicao->lisCAR_BT_FIXA, eqptoCARGA);
		}
		else
		{ // Rede MT: insere Cargas fixas da rede primária lisCAR_MT_FIXA
			rede->LisEqbar(medicao->lisCAR_MT_FIXA, eqptoCARGA);
		}
	}
	// salva Cargas das Redes ajustáveis (que podem ter Cargas fixas)
	for (int nr = 0; nr < medicao->lisREDE_AJUS->Count; nr++)
	{
		rede = (VTRede*)medicao->lisREDE_AJUS->Items[nr];
		// verifica se deve verificar se o fluxo foi executado para a rede
		if (pos_fluxo && (!rede->FluxoOK))
		{
			continue;
		}
		// determina lista de Cargas da Rede
		lisEQP->Clear();
		// verifica se a rede esta carregada
		// if (!rede->Carregada)
		// {
		// int a = 0; // teste
		// }
		rede->LisEqbar(lisEQP, eqptoCARGA);
		// determina redePT
		redePT = montaRedePT->ExisteRedePT(rede);
		if (rede->TipoRede->Segmento == redeSEC)
		{ // Rede BT: separa as Cargas ajustáveis das fixas
			for (int nc = 0; nc < lisEQP->Count; nc++)
			{
				carga = (VTCarga*)lisEQP->Items[nc];
				if (carga->classe->Tag == consIP)
				{ // insere Carga na lista  de Cargas fixas
					medicao->lisCAR_BT_FIXA->Add(carga);
				}
				else
				{ // insere Carga na lista  de Cargas ajustáveis
					medicao->lisCAR_BT_AJUS->Add(carga);
				}
			}
			if (redePT != NULL)
			{ // remove medidores da lista de cargas
				RetiraLis1DeLis2(redePT->LisCargaMedidor(), medicao->lisCAR_BT_AJUS);
			}
		}
		else
		{ // Rede MT: insere Cargas ajustáveis da rede primária lisCAR_MT_JUS
			rede->LisEqbar(medicao->lisCAR_MT_AJUS, eqptoCARGA);
			if (redePT != NULL)
			{ // remove medidores da lista de cargas
				RetiraLis1DeLis2(redePT->LisCargaMedidor(), medicao->lisCAR_MT_AJUS);
			}
		}

	}
	// separa as cargas MT por nivel de tensão
	IniciaLisCargaNivelTensao(medicao);
	// remove todas as cargas que estiverem na barra inicial da rede,
	// essas cargas serão contabilizadas pela rede mãe

}
// ---------------------------------------------------------------------------
void   __fastcall TAjustaEnergia::IniciaLisCargaNivelTensao(TMedicao *medicao)
{
	VTCarga *carga;
	TList *lisCAR_MT;
	VTBarra *barra;

	if (medicao == NULL)
		{return;}

	medicao->A3a.lisCAR->Clear();
	medicao->A4.lisCAR->Clear();
	medicao->outros.lisCAR->Clear();
	medicao->B.lisCAR->Clear();
	//cria lista auxiliar
	lisCAR_MT = new TList;
	lisCAR_MT->Assign(medicao->lisCAR_MT_AJUS,laOr);
//	lisCAR_MT->Assign(medicao->lisCAR_MT_FIXA,laOr);
	//percorre toda lista de cargas mt
	for (int nc = 0; nc < lisCAR_MT->Count; nc++)
	{
		carga = (VTCarga*) lisCAR_MT->Items[nc];
		barra = carga->pbarra;
		if (barra == NULL)
			{continue;}
		//classifica de acordo com a tensão
		if (IsDoubleZero(barra->vnom - 44))
		{
			medicao->A3a.lisCAR->Add(carga);
		}
		else if (IsDoubleZero(barra->vnom - 30))
		{
			medicao->A3a.lisCAR->Add(carga);
		}
		else if ((barra->vnom < 44) && (barra->vnom > 30))
		{
			medicao->A3a.lisCAR->Add(carga);
		}
		else if (IsDoubleZero(barra->vnom - 25))
		{
			medicao->A3a.lisCAR->Add(carga);
		}
		else if (IsDoubleZero(barra->vnom - 2.3))
		{
			medicao->A4.lisCAR->Add(carga);
		}
		else if ((barra->vnom < 25) && (barra->vnom > 2.3))
		{
			medicao->A4.lisCAR->Add(carga);
		}
		else if ((barra->vnom < 2.3) && (barra->vnom > 1))
		{
			medicao->B.lisCAR->Add(carga);
		}
		else if (IsDoubleZero(barra->vnom - 1))
		{
			medicao->B.lisCAR->Add(carga);
		}
		else
		{
			medicao->outros.lisCAR->Add(carga);
        }
	}
    //guarda as cargas B
	medicao->B.lisCAR->Assign(medicao->lisCAR_BT_AJUS,laOr);
//	medicao->B.lisCAR->Assign(medicao->lisCAR_BT_FIXA,laOr);
	if (lisCAR_MT)
	{
		delete lisCAR_MT;
        lisCAR_MT = NULL;
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::IniciaDivisaoPerdaComercial(TMedicao *medicao)
{
	// variáveis locais
	struct
	{
		double sdbt_mwhmes, sdmt_mwhmes, total_mwhmes;
	} energia_ajus;

	// proteção: verifica se há energia ajustável na BT
	energia_ajus.sdbt_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS);
	energia_ajus.sdmt_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_MT_AJUS);
	energia_ajus.total_mwhmes = energia_ajus.sdbt_mwhmes + energia_ajus.sdmt_mwhmes;
	if (IsDoubleZero(energia_ajus.total_mwhmes))
	{
		medicao->alocacao_perda_comercial.sdbt_pu = 0.;
		medicao->alocacao_perda_comercial.sdmt_pu = 0.;
	}
	else
	{
		medicao->alocacao_perda_comercial.sdbt_pu =
			energia_ajus.sdbt_mwhmes / energia_ajus.total_mwhmes;
		medicao->alocacao_perda_comercial.sdmt_pu =
			energia_ajus.sdmt_mwhmes / energia_ajus.total_mwhmes;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::IniciaLisRedeAjustavelLisRedeFixa(TMedicao *medicao,
	VTNode *node_pai)
{
	// variáveis locais
	VTNode *node_filho;
	TList *lisNODE_FILHO = node_pai->LisNode();

	// insere a Rede do Node na lista de Redes ajustáveis da Medicao
	node_pai->LisObjectNode(medicao->lisREDE_AJUS);
	// loop para todos Node filho
	for (int n = 0; n < lisNODE_FILHO->Count; n++)
	{
		node_filho = (VTNode*)lisNODE_FILHO->Items[n];
		// verifica se a Rede do Node filho possui uma Medicao
		if (medicoes->ExisteMedicao((VTRede*)node_filho->Object) == NULL)
		{ // Rede do Node filho não possui Medicao: chamada recursiva desta função
			IniciaLisRedeAjustavelLisRedeFixa(medicao, node_filho);
		}
		else
		{ // Rede do Node filho possui Medicao: todas as Redes a jusante devem ser
			// vistas como fixas para a Medicao do Node pai
			// insere a Rede do Node filho na lista de Redes fixas da Medicao
			node_filho->LisObjectNode(medicao->lisREDE_FIXA);
			// insere todas as Redes a jusante na lista de Redes
			node_filho->LisObjectJusante(medicao->lisREDE_FIXA);
			// IMPORTANTE: não executa chamada recursiva desta função
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::IniciaUmaMedicao(int dias_mes, TMedicao *medicao)
{
	// variáveis locais
	int div_mt_bt;
	VTNode *node;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// salva número de dias do mês
	this->num_dias = dias_mes;
	// verifica se seve reiniciar a Arvore
	if (arvore->LisNodeRaiz()->Count == 0)
	{ // reinicia Arvore
		arvore->Inicia(redes);
	}
	// determina Node associado da mesma Rede da Medicao
	if ((node = arvore->ExisteNode(medicao->rede)) == NULL)
		return (false);
	// define Medicao com mesmo Level de VTNode
	medicao->level = node->Level;
	// monta listas de Redes ajustáveis e fixas da Medicao
	IniciaLisRedeAjustavelLisRedeFixa(medicao, node);
	// inicia lista de Cargas e ajustáveis e fixas da Rede da Medicao
	IniciaLisCargaAjustaveisFixas(medicao, false);
	// remove as cargas que ficam nas barras iniciais
	RemoveCargasBarraInicial(medicao);
	// determina energia inicial das Cargas para cálculo da perda total
	medicao->energia.carga_ini_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS) +
		EnergiaCarga_mwhmes(medicao->lisCAR_BT_FIXA) + EnergiaCarga_mwhmes(medicao->lisCAR_MT_AJUS)
		+ EnergiaCarga_mwhmes(medicao->lisCAR_MT_FIXA);
	//guarda a energia inicial das cargas por nivel de tensao
	medicao->A3a.energia.carga_ini_mwhmes = EnergiaCarga_mwhmes(medicao->A3a.lisCAR);
	medicao->A4.energia.carga_ini_mwhmes = EnergiaCarga_mwhmes(medicao->A4.lisCAR);
	medicao->outros.energia.carga_ini_mwhmes = EnergiaCarga_mwhmes(medicao->outros.lisCAR);
	medicao->B.energia.carga_ini_mwhmes = EnergiaCarga_mwhmes(medicao->B.lisCAR);
	// proteção: verifica se há energia ajustável na sdbt/sdmt e, se necessário, rededine
	// distribuição das perdas comerciais
	ValidaDistribuicaoPerdaComercial(medicao);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::RedefineEnergiaCarga(TList *lisCARGA, double fatmult)
{
	// variáveis locais
	double p_mw, q_mvar;
	complex<double>sf_mva[MAX_FASE];
	VTCarga *carga;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// loop p/ todas Cargas
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*)lisCARGA->Items[nc];
		// verifica se é uma Carga típica
		if (carga->curva->Tipica)
		{ // corrige a enrgia da Carga
			carga->Energia_mwhmes = carga->Energia_mwhmes * fatmult;
		}
		// carga com curva própria: verifica se curva de demanda por fase
		else if (carga->curva->NumeroValores == nvCURVA_RRCAR)
		{ // corrige o valor de cada fase em todos patamares
			for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
			{
				if (!demanda->Get_S3f_mva(carga, ind_pat, sf_mva))
					continue;
				for (int nf = 0; nf < MAX_FASE; nf++)
				{
					p_mw = sf_mva[nf].real() * fatmult;
					q_mvar = sf_mva[nf].imag() * fatmult;
					sf_mva[nf] = complex<double>(p_mw, q_mvar);
				}
				// Atualiza demanda por fase
				if (!demanda->Set_S3f_mva(carga, ind_pat, sf_mva))
					return (false);
			}
		}
		// carga com curva própria: verifica se curva de demanda trifásica
		else if (carga->curva->NumeroValores == nvCURVA_CAR)
		{ // corrige o valor da demanda rifásica em todos patamares
			for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
			{ // determina demanda total
				p_mw = demanda->P_mw[carga][ind_pat];
				q_mvar = demanda->Q_mvar[carga][ind_pat];
				// atualza demanda total
				demanda->P_mw[carga][ind_pat] = p_mw * fatmult;
				demanda->Q_mvar[carga][ind_pat] = q_mvar * fatmult;
			}
		}
		else
		{
//			int a = 0;
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16792780
void __fastcall TAjustaEnergia::RemoveCargasBarraInicial(TMedicao *medicao)
{ // variáveis locais
	VTCarga *carga;
	VTRede *rede;
	VTBarra *barraIni;
	TList *lisAUX;
	// remove todas as cargas que estiverem na barra inicial da rede,
	// essas cargas serão contabilizadas pela rede mãe

	// percorre todas cargas da medicao e inicializa com 0
	for (int nc = 0; nc < medicao->lisCAR_BT_AJUS->Count; nc++)
	{
		carga = (VTCarga*)medicao->lisCAR_BT_AJUS->Items[nc];
		carga->Tag = 0;
	}
	for (int nc = 0; nc < medicao->lisCAR_MT_AJUS->Count; nc++)
	{
		carga = (VTCarga*)medicao->lisCAR_MT_AJUS->Items[nc];
		carga->Tag = 0;
	}
	for (int nc = 0; nc < medicao->lisCAR_BT_FIXA->Count; nc++)
	{
		carga = (VTCarga*)medicao->lisCAR_BT_FIXA->Items[nc];
		carga->Tag = 0;
	}
	for (int nc = 0; nc < medicao->lisCAR_MT_FIXA->Count; nc++)
	{
		carga = (VTCarga*)medicao->lisCAR_MT_FIXA->Items[nc];
		carga->Tag = 0;
	}

	lisAUX = new TList;
	// verifica todas redes fixas
	for (int nr = 0; nr < medicao->lisREDE_FIXA->Count; nr++)
	{
		lisAUX->Clear();
		rede = (VTRede*)medicao->lisREDE_FIXA->Items[nr];
		barraIni = rede->BarraInicial();
		if (barraIni != NULL)
		{
			barraIni->LisEqbar(lisAUX, eqptoCARGA);
			for (int nc = 0; nc < lisAUX->Count; nc++)
			{
				carga = (VTCarga*) lisAUX->Items[nc];
				carga->Tag = 1;
			}
		}
	}
	// verifica todas redes ajustaveis
	for (int nr = 0; nr < medicao->lisREDE_AJUS->Count; nr++)
	{
		lisAUX->Clear();
		rede = (VTRede*)medicao->lisREDE_AJUS->Items[nr];
		barraIni = rede->BarraInicial();
		if (barraIni != NULL)
		{
			barraIni->LisEqbar(lisAUX, eqptoCARGA);
			for (int nc = 0; nc < lisAUX->Count; nc++)
			{
				carga = (VTCarga*) lisAUX->Items[nc];
				carga->Tag = 1;
			}
		}
	}
	// agora sim remove as cargas
	for (int nc = (medicao->lisCAR_BT_AJUS->Count - 1); nc >= 0; nc--)
	{
		carga = (VTCarga*)medicao->lisCAR_BT_AJUS->Items[nc];
		if (carga->Tag == 1)
		{
			medicao->lisCAR_BT_AJUS->Delete(nc);
            //atualiza tag para não ser removido de outra lista
            carga->Tag = 0;
		}
	}
	for (int nc = (medicao->lisCAR_MT_AJUS->Count - 1); nc >= 0; nc--)
	{
		carga = (VTCarga*)medicao->lisCAR_MT_AJUS->Items[nc];
		if (carga->Tag == 1)
		{
			medicao->lisCAR_MT_AJUS->Delete(nc);
            //atualiza tag para não ser removido de outra lista
            carga->Tag = 0;
		}
	}
	for (int nc = (medicao->lisCAR_BT_FIXA->Count - 1); nc >= 0; nc--)
	{
		carga = (VTCarga*)medicao->lisCAR_BT_FIXA->Items[nc];
		if (carga->Tag == 1)
		{
			medicao->lisCAR_BT_FIXA->Delete(nc);
            //atualiza tag para não ser removido de outra lista
            carga->Tag = 0;
		}
	}
	for (int nc = (medicao->lisCAR_MT_FIXA->Count - 1); nc >= 0; nc--)
	{
		carga = (VTCarga*)medicao->lisCAR_MT_FIXA->Items[nc];
		if (carga->Tag == 1)
		{
			medicao->lisCAR_MT_FIXA->Delete(nc);
            //atualiza tag para não ser removido de outra lista (apesar que aqui não tem outra lista...)
            carga->Tag = 0;
		}
	}

	if (lisAUX)
	{
		delete lisAUX;
		lisAUX = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAjustaEnergia::RemoveMedicaoInvalida(void)
{
	// variáveis locais
	TMedicao *medicao;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	// loop para todas Medicao
	for (int n = lisMEDICAO->Count - 1; n >= 0; n--)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		// valida medição
		if (!medicao->valida)
		{ // retira da lista e destroi medicao
			lisMEDICAO->Delete(n);
			delete medicao;
			continue;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAjustaEnergia::SalvaCargaFinal(void)
{
	// variáveis locais
	struct
	{
		double ajus_mwhmes;
	} energia;
	TMedicao *medicao;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	// loop para todas Medicao
	for (int n = 0; n < lisMEDICAO->Count; n++)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		// determina energia mensal das Cargas ajustáveis
		medicao->energia.carga_fim_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS) +
			EnergiaCarga_mwhmes(medicao->lisCAR_BT_FIXA) + EnergiaCarga_mwhmes
			(medicao->lisCAR_MT_AJUS) + EnergiaCarga_mwhmes(medicao->lisCAR_MT_FIXA);
        //guarda a energia inicial das cargas por nivel de tensao
		medicao->A3a.energia.carga_fim_mwhmes = EnergiaCarga_mwhmes(medicao->A3a.lisCAR);
		medicao->A4.energia.carga_fim_mwhmes = EnergiaCarga_mwhmes(medicao->A4.lisCAR);
		medicao->outros.energia.carga_fim_mwhmes = EnergiaCarga_mwhmes(medicao->outros.lisCAR);
		medicao->B.energia.carga_fim_mwhmes = EnergiaCarga_mwhmes(medicao->B.lisCAR);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAjustaEnergia::ValidaDistribuicaoPerdaComercial(TMedicao *medicao)
{
	// variáveis locais
	int div_mt_bt = 0;

	if (!IsDoubleZero(EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS)))
		div_mt_bt = 1;
	if (!IsDoubleZero(EnergiaCarga_mwhmes(medicao->lisCAR_MT_AJUS)))
		div_mt_bt += 2;
	switch (div_mt_bt)
	{
	case 0: // não há energia ajustável
		medicao->alocacao_perda_comercial.sdbt_pu = 0.;
		medicao->alocacao_perda_comercial.sdmt_pu = 0.;
		break;
	case 1: // há somente energia ajustavel sdbt
		medicao->alocacao_perda_comercial.sdbt_pu = 1.;
		medicao->alocacao_perda_comercial.sdmt_pu = 0.;
		break;
	case 2: // há somente energia ajustavel sdmt
		medicao->alocacao_perda_comercial.sdbt_pu = 0.;
		medicao->alocacao_perda_comercial.sdmt_pu = 1.;
		break;
	case 3: // há energia ajustável sdbt e e sdmt: nada a fazer
		break;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::ValidaMedicao(TMedicao *medicao)
{
	// verifica se a energia medida é nula
	if (IsDoubleZero(medicao->energia.medida_mwhmes))
		return (false);
	// verifica se a perda total é negativa
	if (medicao->PerdaTotal_mwhmes() <= 0)
		return (false);
	return (true);
}

// DVK 2016.01.22 considera possibilidade de redes em grupo
// ---------------------------------------------------------------------------
bool __fastcall TAjustaEnergia::TESTE_CriaMedicoes(int num_dias)
{
	// variáveis locais
	bool divide_perda_comercial = true;
	TList *lisNODE;
	TList *lisREDE, *lisMEDICAO;
	TMedicao *medicao;
	VTRede *rede;
	VTNode *node;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia Medicoes
	medicoes->Clear();
	// inicia Arvore
	// verifica se seve reiniciar a Arvore
	if (arvore->LisNodeRaiz()->Count == 0)
	{ // reinicia Arvore
		arvore->Inicia(redes);
	}
	// cria lista temporária
	lisNODE = new TList();
	// cria uma Medicao para cada Rede primária presente na Arvore
	arvore->LisAllNode(lisNODE);
	for (int n = 0; n < lisNODE->Count; n++)
	{
		node = (VTNode*)lisNODE->Items[n];
		// obtém lista de Redes do Node
		lisREDE = node->LisObjectNode();
		// proteção
		if (lisREDE->Count < 1)
			continue;
		// determina Redes do Node
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			if (!rede->Carregada)
				continue;
			if (rede->TipoRede->Segmento != redePRI)
				continue;
			// cria uma nova Medicao
			medicao = medicoes->InsereMedicao(rede);
		}
	}
	// destrói lista criada
	delete lisNODE;
	// inicia dados das Medicoes
	if (!IniciaAllMedicao(num_dias, divide_perda_comercial))
		return (false);
	// define valor de cada medição igual a 120% da energia mensal das Carga
	lisMEDICAO = medicoes->LisMedicao();
	for (int n = 0; n < lisMEDICAO->Count; n++)
	{
		medicao = (TMedicao*)lisMEDICAO->Items[n];
		// define energia medida igual a 120% da energia inicial
		medicao->energia.medida_mwhmes = medicao->energia.carga_ini_mwhmes / 0.8;
	}
	return (true);
}
// ---------------------------------------------------------------------------
// eof
