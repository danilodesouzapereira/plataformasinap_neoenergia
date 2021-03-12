// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <PlataformaSinap\DLL_Inc\CalcIndCont.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTCalcIndCont.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTResCont.h>
#include "TAnaTec.h"
#include "VTResumoTec.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Consulta\VTConsulta.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Edita\VTEdita.h"
#include "..\..\Flow\VTFlow.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\Obra\VTAcao.h"
#include "..\..\Obra\VTObra.h"
#include "..\..\Obra\ItemObra\VTItemObra.h"
#include "..\..\Obra\VTObras.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Planejamento\VTAlternativa.h"
#include "..\..\Planejamento\VTEstudo.h"
#include "..\..\Planejamento\VTPlanejamento.h"
#include "..\..\Perdatec\ConstantesPerda.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTCorte.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTReducao.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTResFlowSup.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Progresso.h"
#include "..\..\..\DLL_Inc\Radial.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTAnaTec* __fastcall NewObjAnaTec(VTApl *apl_owner)
{
	try
	{
		return (new TAnaTec(apl_owner));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAnaTec::TAnaTec(VTApl *apl_owner)
{
	// salva ponteiro p/ objetos
	this->apl = apl_owner;

	// variaveis locais para impressao
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	slContinuidade = new TStringList();

	// cria objetos
	lisEQP = new TList();
	lisEQP_INS = new TList();
	lisEQP_SUB = new TList();
	lisREDE = new TList();
	lisOBRA = new TList();
	campos = new TStringList();
	// calc_ind_cont = DLL_NewCalcIndCont(apl);
	consulta = DLL_NewObjConsulta(apl);
	progresso = DLL_NewObjProgresso(NULL, NULL);
	arvore = DLL_NewObjArvore(apl);
	// prepara CalcIndCont
	// calc_ind_cont->LeArquivosMdb("");
	// inicialização do diretório para salvar o txt com resultados dos indices de continuidade
	arq_rede = path->DirTmp() + "\\" + ExtractFileName(path->ArqRede);
}

// ---------------------------------------------------------------------------
__fastcall TAnaTec::~TAnaTec(void)
{
	// destrói objetos
	// if (calc_ind_cont) {delete calc_ind_cont; calc_ind_cont = NULL;}

	if (arvore)
	{
		arvore->Clear();
		delete arvore;
		arvore = NULL;
	}

	if (consulta)
	{
		delete consulta;
		consulta = NULL;
	}
	if (progresso)
	{
		delete progresso;
		progresso = NULL;
	}
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	// destrói lista sem destruir seus objetos
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
	if (lisOBRA)
	{
		delete lisOBRA;
		lisOBRA = NULL;
	}
	// destrói lista e seus objetos
	if (lisEQP_INS)
	{
		LimpaTList(lisEQP_INS);
		delete lisEQP_INS;
		lisEQP_INS = NULL;
	}
	if (lisEQP_SUB)
	{
		LimpaTList(lisEQP_SUB);
		delete lisEQP_SUB;
		lisEQP_SUB = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
bool __fastcall TAnaTec::AlimentadorPai(VTRede *rede_sub, VTRede *rede_filha)
{
	// variáveis locais
	AnsiString cod_gis;

	// proteçao
	if ((rede_sub == NULL) || (rede_filha == NULL))
		return false;

	// limpa campos
	campos->Clear();
	ExtraiStrings(rede_sub->Extern_id, "_", campos);
	// somente com 2 campos ou  mais será analisado
	if (campos->Count < 2)
		return false;
	for (int n = 1; n < campos->Count; n++)
	{ // o primeiro campo é o codigo GIS da sub, então compara os demais
		cod_gis = campos->Strings[n];
		if (cod_gis.AnsiCompareIC(rede_filha->Extern_id) == 0)
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::Cabecalho(void)
{
	// variáveis locais
	AnsiString linha;

	linha = "//ano;alimentador;alternativa;numero_consumidores;DEC;FEC;END;";
	slContinuidade->Add(linha);
}

// ---------------------------------------------------------------------------
double __fastcall TAnaTec::CalculaCompensacao(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	strResRede *resRede;
	TList *lisRede;
	double totalComp;

	// acumula compensação de cada rede (R$/ano)
	lisRede = redes->LisRede();
	totalComp = 0.0;
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];
		if (!rede->Carregada)
			continue;
		resRede = calc_ind_cont->ResCont->ResRede[rede]; // hk201902: calc_ind_cont->ResRede[rede];
		if (resRede->ncons == 0)
			continue;
		totalComp += resRede->compensacao;
	}
	return (totalComp);
}

// ---------------------------------------------------------------------------
double __fastcall TAnaTec::CalculaEnergia_mwh(double p_mw, int num_pat)
{
	// variáveis locais
	double periodo_pat = 24. / num_pat;

	// converte mw ==> mwh
	return (RoundTo(p_mw * periodo_pat, ROUND_PRECISION));
}

// ---------------------------------------------------------------------------
double __fastcall TAnaTec::CalculaEnergiaRecebida(VTRede *rede)
{
	// variáveis locais
	strLKC lkc;
	double energia_recebida_mwh;
	VTCorte *corte;
	VTLigacao *ligacao;
	TList *lisCORTE;

	// inicia energia recebida
	energia_recebida_mwh = 0.;
	// loop p/ todos Cortes da Rede
	lisCORTE = rede->LisCorte();
	for (int nc = 0; nc < lisCORTE->Count; nc++)
	{
		corte = (VTCorte*)lisCORTE->Items[nc];
		// calcula energia transferida pela Rede no Corte
		FluxoEnergiaDaRedeNoCorte_mwh(rede, corte, lkc);
		// verifica se a potência transferida no Corte é nula
		if (IsDoubleZero(fabs(lkc.Eext_mwh) + fabs(lkc.Eint_mwh), DOUBLE_ZERO_VALUE))
			continue;
		// verifica se a energia transferida p/ Rede externa no Corte é positiva ou negativa
		if (lkc.Eext_mwh > 0)
		{ // energia recebida (sinal positivo)
			energia_recebida_mwh += lkc.Eext_mwh;
		}

		// verifica se a energia transferida p/ Rede interna no Corte é positiva ou negativa
		if (lkc.Eint_mwh > 0)
		{ // energia recebida (sinal positivo)
			energia_recebida_mwh += lkc.Eint_mwh;
		}
	}
	return energia_recebida_mwh;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
AnsiString __fastcall TAnaTec::CodigoGIS(VTRede *rede)
{
	// variáveis locais
	AnsiString cod_gis = "";

	if (rede != NULL)
	{
		campos->Clear();
		ExtraiStrings(rede->Extern_id, "_", campos);
		if (campos->Count > 0)
		{ // o primeiro campo é o codigo GIS
			cod_gis = campos->Strings[0];
		}
	}
	return cod_gis;
}

// ---------------------------------------------------------------------------
VTResumoTec* __fastcall TAnaTec::DeterminaResumo(VTRede *rede, VTResumoTec *resumo_pai)
{
	// variáveis locais
	VTNode *node;
	VTRede *rede_pai;
	VTResumoTec *resumo_alim;
	AnsiString sub, cod_sub;

	//proteção
	if((resumo_pai == NULL) || (rede == NULL))
		return NULL;
	try
	{ // determina node
		node = arvore->ExisteNode(rede);
		if (node == NULL)
			return NULL;
		// alimentador pode não ter subestação supridora
		sub = "";
		cod_sub = "";
		if ((node->Pai != NULL) && (node->Pai->Object != NULL))
		{ // determina subestação supridora
			rede_pai = DeterminaSubestacao(node->Pai, rede);
			if (rede_pai != NULL)
			{
				sub = rede_pai->Codigo;
				cod_sub = CodigoGIS(rede_pai);
			}
		}
		else
		{ // pode ser que esteja no grupo de redes do mesmo node
			rede_pai = DeterminaSubestacao(node, rede);
			if (rede_pai != NULL)
			{
				sub = rede_pai->Codigo;
				cod_sub = CodigoGIS(rede_pai);
			}
		}
		// determina resumo do alimentador
		resumo_alim = resumo_pai->ExisteResumoRede(rede->Codigo, sub);
		if (resumo_alim == NULL)
		{ // cria resumo para o alimentador
			resumo_alim = NewObjResumoTec(resumo_pai->AnoInicial, resumo_pai->AnoFinal);
			resumo_alim->Alimentador = rede->Codigo;
			resumo_alim->CodAlimentador = CodigoGIS(rede);
			resumo_alim->Subestacao = sub;
			resumo_alim->CodSubestacao = cod_sub;
			resumo_pai->LisResumoTecRede()->Add(resumo_alim);
		}
		return resumo_alim;
	}
	catch (Exception &e)
	{
		return NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
VTRede* __fastcall TAnaTec::DeterminaSubestacao(VTNode *node_pai, VTRede *rede_filha)
{
	// variáveis locais
	UnicodeString className;
	VTRede *rede;
	TObject *obj;
	TList *lisOBJ;
	//VTNode *node;
	int ind;

	try
	{ // proteção
		if (node_pai->Object == NULL)
		{
			return NULL;
		}
		// verifica se é grupo de redes
		ind = node_pai->Codigo.AnsiPos("Grupo");
		if (ind <= 0)
		{ // verifica se object é rede subestação
			className = node_pai->Object->ClassName();
			if (className.CompareIC("TRede") == 0)
			{
				rede = (VTRede*)node_pai->Object;
				if ((rede->Carregada) && (rede->TipoRede->Segmento == redeETD))
				{ // verifica se a rede filha não é um alimentador pai
					if (!AlimentadorPai(rede, rede_filha))
						return rede;
				}
			}
		}
		// verifica objects do node
		lisOBJ = node_pai->LisObjectNode();
		for (int n = 0; n < lisOBJ->Count; n++)
		{
			obj = (TObject*)lisOBJ->Items[n];
			className = obj->ClassName();
			if (className.CompareIC("TRede") == 0)
			{
				rede = (VTRede*)obj;
				if ((rede->Carregada) && (rede->TipoRede->Segmento == redeETD))
				{ // verifica se a rede filha não é um alimentador pai
					if (!AlimentadorPai(rede, rede_filha))
						return rede;
				}
			}
		}

	}
	catch (Exception &e)
	{
		return NULL;
	}
	catch(...)
	{
        return NULL;
    }
	return NULL;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
bool __fastcall TAnaTec::Executa(void)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString msg;
	VTAlternativa *alternativa;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();
	TList *lisALTER = estudo->LisAlternativa();

	// insere Cabecalho para impressão do txt de DEC, FEC e END
	Cabecalho();
	// determina ano inicial e final do Estudo
	ano_ini = estudo->AnoInicial;
	// ano_fim = estudo->AnoFinal;
	ano_fim = estudo->AnoFinal;

	// exibe barra de progresso
	msg.printf("Análise das %d Alternativas: %d a %d", lisALTER->Count, ano_ini, ano_fim);
	progresso->Start(progTEXTO);
	progresso->Add(msg);
	try
	{ // garante que todas as Alternativas tenham o objeto VTResumoTec
		IniciaAlternativas();
		// loop p/ todas Alternativas do Estudo
		for (int n = 0; (sucesso) && (n < lisALTER->Count); n++)
		{
			alternativa = (VTAlternativa*)lisALTER->Items[n];
			// define Alternativa ativa no Estudo
			estudo->DefineAlternativaAtiva(alternativa, NULL);
			// executa análise técnica da Alternativa
			sucesso = Executa(alternativa);
		}
        // imprime os dados de DEC, FEC e END
		slContinuidade->SaveToFile(arq_rede + "_DEC_FEC_END_planejamento.txt");
	}
	catch (Exception &e)
	{
		progresso->Stop();
		sucesso = false;
	}
	catch (...)
	{
		progresso->Stop();
		sucesso = false;
	}

	// esconde barra de progresso
	progresso->Stop();
	return (sucesso);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
bool __fastcall TAnaTec::Executa(VTAlternativa *alternativa, int ano)
{
	// variáveis locais
	bool sucesso = true;
	bool fluxo_ok;
	AnsiString msg;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();
	TList *lisALTER = estudo->LisAlternativa();

	// determina ano inicial e final do Estudo
	ano_ini = estudo->AnoInicial;
	// ano_fim = estudo->AnoFinal;
	ano_fim = estudo->AnoFinal;
	// exibe barra de progresso
	// progresso->Start(progTEXTO);
	// msg.printf("Análise das %d Alternativas: %d a %d", lisALTER->Count, ano_ini, ano_fim);
	// progresso->Add(msg);
	try
	{ // garante que todas as Alternativas tenham o objeto VTResumoTec
		IniciaAlternativas();
		// define Alternativa ativa no Estudo
		estudo->DefineAlternativaAtiva(alternativa, NULL);
		// atualiza progresso
		// progresso->Add(alternativa->Codigo + ": " + IntToStr(ano));
		// reconfigura a rede, retirando todas as Obras
		// alternativa->ReconfiguraRedeSemObra();
		// inclui as Obras até ano na rede
		alternativa->ReconfiguraRede(ano);
		// cria cálculo de confiabilidade
		calc_ind_cont = DLL_NewCalcIndCont(apl);
		// prepara CalcIndCont
		calc_ind_cont->LeArquivosMdb(""); // hk201902: calc_ind_cont->LeArquivosMdb("");
		// executa cálculo do fluxo de potência
		fluxo_ok = ExecutaFluxoPotencia(ano);
		// executa cálculo de continuidade
		ExecutaContinuidade(alternativa, ano);
		// //executa cálculo do fluxo de potência
		// fluxo_ok = ExecutaFluxoPotencia(ano);
		// adiciona os dados de índices de continuidade para uma alternativa e um ano
		ImprimeDEC_FEC_END(alternativa, ano);
		// salva informações técnicas da rede
		IniciaResumoTec(alternativa, ano, fluxo_ok);
		// destroi confiabilidade
		delete calc_ind_cont;
	}
	catch (Exception &e)
	{
		if (calc_ind_cont)
		{
			delete calc_ind_cont;
			calc_ind_cont = NULL;
		}
		sucesso = false;
	}
	// esconde barra de progresso
	// progresso->Stop();
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaTec::Executa(VTAlternativa *alternativa)
{
	// variáveis locais
	bool fluxo_ok;

	try
	{ // reconfigura a rede, retirando todas as Obras
		alternativa->ReconfiguraRedeSemObra();
		// loop p/ todos anos do planejamento
		for (int ano = ano_ini; ano <= ano_fim; ano++)
		{ // atualiza progresso
			progresso->Add(alternativa->Codigo + ": " + IntToStr(ano));
			// inclui as Obras do ano na rede
			alternativa->ReconfiguraRede(ano);
			// executa cálculo do fluxo de potência
			fluxo_ok = ExecutaFluxoPotencia(ano);

			if (!PossuiuReticulado())
			{
				// cria cálculo de confiabilidade
				calc_ind_cont = DLL_NewCalcIndCont(apl);
				// prepara CalcIndCont
				calc_ind_cont->LeArquivosMdb(""); // hk201902: calc_ind_cont->LeArquivosMdb("");
				// executa cálculo de continuidade
				ExecutaContinuidade(alternativa, ano);
				// adiciona os dados de índices de continuidade para uma alternativa e um ano
				ImprimeDEC_FEC_END(alternativa, ano);
			}
			// salva informações técnicas da rede
			IniciaResumoTec(alternativa, ano, fluxo_ok);
			if (calc_ind_cont != NULL)
			{
				// destroi confiabilidade
				delete calc_ind_cont;
			}
		}
	}
	catch (Exception &e)
	{
		if (calc_ind_cont)
		{
			delete calc_ind_cont;
			calc_ind_cont = NULL;
		}
		return (false);
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TAnaTec::ExecutaContinuidade(void)
 {
 //variáveis locais
 VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
 TList     *lisBARRA = redes->LisBarra();

 //verifica limitação de Barras do Confiabilidade
 if (lisBARRA->Count > DLL_Confiabilidade_LimiteBarras())
 {
 return(false);
 }
 confiab->IniciaConfigGerais();
 confiab->ExecutaSC();
 return(true);
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15755250
bool __fastcall TAnaTec::ExecutaContinuidade(VTAlternativa *alternativa, int ano)
{
	// variáveis locais
	bool sucesso = true;
	VTObra *obra;
	VTItemObra *itemObra;

	if (PossuiuReticulado())
		return true;

	try
	{ // determina conjunto de Obras até o ano indicado que afetam a taxa de falha da rede
		lisOBRA->Clear();
		// alternativa->Obras->LisObra(lisOBRA, ano);
		// 20.02.2018 - alteração GH - correção para que sejam analisadas as obras até determinado ano, ao invés das obras no ano.
		alternativa->Obras->LisObraContinuidade(lisOBRA, ano);
		// determina conjunto de Redes inseridas nas Obras
		lisREDE->Clear();
		for (int n = 0; n < lisOBRA->Count; n++)
		{
			obra = (VTObra*)lisOBRA->Items[n];
			for (int ni = 0; ni < obra->LisItemObra->Count; ni++)
			{
				itemObra = (VTItemObra*)obra->LisItemObra->Items[ni];
				//se não for um item contabilizado ou não alterar taxa de falha
				if ((!itemObra->Contabilizar) || (!itemObra->AlteraTaxaFalha))
					{continue;}
                // determina conjunto de Redes inseridas na Obra
				IniciaLisRedeInserida(itemObra->LisAcao);
			}
		}
		// determina conjunto de Eqptos inseridos/substituidos nas Obras que afetam a taxa de falha da rede
		LimpaTList(lisEQP_INS);
		LimpaTList(lisEQP_SUB);
		for (int n = 0; n < lisOBRA->Count; n++)
		{
			obra = (VTObra*)lisOBRA->Items[n];
			for (int ni = 0; ni < obra->LisItemObra->Count; ni++)
			{
				itemObra = (VTItemObra*)obra->LisItemObra->Items[ni];
				//se não for um item contabilizado ou não alterar taxa de falha
				if ((!itemObra->Contabilizar) || (!itemObra->AlteraTaxaFalha))
					{continue;}
                // determina conjunto de Redes inseridas na Obra
				IniciaLisEqptoInseridoSubstituido(itemObra, itemObra->LisAcao);
			}
		}
		// executa cálculo de confiabilidade
		sucesso = calc_ind_cont->Executa(lisEQP_INS, lisEQP_SUB);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	// destrói objetos das listas
	LimpaTList(lisEQP_INS);
	LimpaTList(lisEQP_SUB);

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaTec::ExecutaFluxoPotencia(int ano)
{
	// variáveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// atualiza ano no Mercado
	mercado->AnoAtual = ano;
	// executa fluxo de potência equilibrado
	flow->AvisoEnabled = false;
	flow->TramoEnabled = true;
	flow->TipoRelatorio = relNENHUM;
	flow->TipoFluxo = fluxoEQUILIBRADO;
	flow->UpToDate = false;
	// executa cálculo de fluxo
	if (!flow->CalculaPorRede(redes))
		return (false); // ano)) return(false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaTec::ExisteGD(void)
{
	// variáveis locais
	bool existe = false;
	TList *lisGER = new TList();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se há geradores
	lisGER->Clear();
	redes->LisEqpto(lisGER, eqptoGERADOR);
	if (lisGER->Count > 0)
		existe = true;
	// destroi lista
	delete lisGER;
	return (existe);
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaTec::FluxoEnergiaDaRedeNoCorte_mwh(VTRede *rede, VTCorte *corte, strLKC &lkc)
{
	// variáveis locais
	int ind_bar;
	double p_mw, p_viz_ext_mw, p_viz_int_mw, parcela_mw;
	TList *lisBAR_CORTE;
	TList *lisREDE_CORTE;
	VTBarra *barra;
	VTLigacao *ligacao;
	VTRede *rede_viz;
	VTNode *node;
	TList *lisREDE;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int num_pat = patamares->NumPatamar();

	// determina Node da Rede
	node = arvore->ExisteNode(rede);
	// determina lista de Redes do mesmo Node (redes que fazem parte de um mesmo Grupo)
	lisREDE = node->LisObjectNode();
	// inicia potencia transferida pela Rede no Corte
	p_mw = p_viz_ext_mw = p_viz_int_mw = 0;
	// determina conjunto de Barras do Corte
	lisBAR_CORTE = corte->LisBarra();
	// obtém lista de redes do Corte
	lisREDE_CORTE = corte->LisRede();
	// loop p/ todas redes do Corte
	for (int nr = 0; nr < lisREDE_CORTE->Count; nr++)
	{
		rede_viz = (VTRede*)lisREDE_CORTE->Items[nr];
		// verifica se o fluxo calculado p/ a rede vizinha
		if (!rede_viz->FluxoOK)
			continue;
		// loop p/ todas Barras do Corte
		for (int nb = 0; nb < lisBAR_CORTE->Count; nb++)
		{
			barra = (VTBarra*)lisBAR_CORTE->Items[nb];
			// determina conjunto de Ligacoes da rede vizinha que conectam a Barra do Corte
			lisEQP->Clear();
			if (rede_viz->Carregada)
			{ // obtém Ligacoes da rede
				LisLigacao(rede_viz->LisLigacao(), barra, lisEQP);
			}
			else
			{ // obtém Ligacoes do esquemático (que também tem informação de fluxo)
				VTReducao* reducao = (VTReducao*)(rede_viz->MRede->LisReducao()->First());
				LisLigacao(reducao->LisLigacaoEsquematico(), barra, lisEQP);
			}
			// acumula a potencia de cada Ligacao na Barra em todos patamares
			for (int nl = 0; nl < lisEQP->Count; nl++)
			{
				ligacao = (VTLigacao*)lisEQP->Items[nl];
				// determina indice da Barra conectada pela Ligacao
				ind_bar = ligacao->IndiceBarra(barra);
				// loop p/ todos patamares
				for (int np = 0; np < num_pat; np++)
				{ // verifica se a rede_viz e própria rede
					if (rede_viz == rede)
					{ // acumula a potencia da Ligacao da Rede na Barra no patamar
						p_mw += ligacao->resflow->P_mw[ind_bar][np];
					}
					// verifica se Rede vizinha é interna (pertence à mesma MRede)
					// else if (rede->MRede->MNet == rede_viz->MRede->MNet)
					// else if (rede->MRede == rede_viz->MRede)
					else if (lisREDE->IndexOf(rede_viz) >= 0)
					{ // acumula a potencia da Ligacao da Rede vizinha interna na Barra no patamar
						p_viz_int_mw += ligacao->resflow->P_mw[ind_bar][np];
					}
					else
					{ // acumula a potencia da Ligacao da Rede vizinha externa na Barra no patamar
						p_viz_ext_mw += ligacao->resflow->P_mw[ind_bar][np];
					}
				}
			}
		}
	}
	// filtra valores muito pequenos
	p_mw = RoundTo(p_mw, ROUND_PRECISION);
	p_viz_ext_mw = RoundTo(p_viz_ext_mw, ROUND_PRECISION);
	p_viz_int_mw = RoundTo(p_viz_int_mw, ROUND_PRECISION);
	// zera valores muito pequenos
	if (IsDoubleZero(p_mw, DOUBLE_ZERO_VALUE))
		p_mw = 0.;
	if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
		p_viz_ext_mw = 0.;
	if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
		p_viz_int_mw = 0.;
	// análise dos possíveis casos
	if (IsDoubleZero(p_mw, DOUBLE_ZERO_VALUE))
	{ // caso 1: não há fluxo da Rede no Corte
		lkc.Eext_mwh = lkc.Eint_mwh = 0.;
		return (true);
	}
	// verifica se a Rede é supridora no Corte
	if (p_mw < 0)
	{
		// alterado em 2016.05.20
		if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
		{ // caso 2: fluxo da Rede é igual ao inverso do fluxo interno
			lkc.Eext_mwh = 0.;
			if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
			{
				lkc.Eint_mwh = 0.;
			}
			else if (p_viz_int_mw < 0.)
			{
				lkc.Eint_mwh = 0.;
			}
			else
			{
				lkc.Eint_mwh = -CalculaEnergia_mwh(p_viz_int_mw, num_pat);
			}
			return (true);
		}
		// alterado em 2016.05.20
		if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
		{ // caso 3: fluxo da Rede é igual ao inverso do fluxo externo
			lkc.Eint_mwh = 0.;
			if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
			{
				lkc.Eext_mwh = 0.;
			}
			else if (p_viz_ext_mw < 0.)
			{
				lkc.Eext_mwh = 0.;
			}
			else
			{
				lkc.Eext_mwh = -CalculaEnergia_mwh(p_viz_ext_mw, num_pat);
			}
			return (true);
		}
		if ((p_viz_ext_mw > 0) && (p_viz_int_mw > 0))
		{ // caso 4: fluxo da Rede é igual ao inverso do fluxo externo e interno
			lkc.Eext_mwh = -CalculaEnergia_mwh(p_viz_ext_mw, num_pat);
			lkc.Eint_mwh = -CalculaEnergia_mwh(p_viz_int_mw, num_pat);
			return (true);
		}
		if ((p_viz_ext_mw < 0) && (p_viz_int_mw > 0))
		{ // caso 5: fluxo da Rede é igual ao inverso de uma parcela do fluxo interno
			parcela_mw = fabs(p_mw * (p_viz_int_mw / (p_mw + p_viz_ext_mw)));
			lkc.Eext_mwh = 0;
			lkc.Eint_mwh = -CalculaEnergia_mwh(parcela_mw, num_pat);
			return (true);
		}
		if ((p_viz_ext_mw > 0) && (p_viz_int_mw < 0))
		{ // caso 6: fluxo da Rede é igual ao inverso de uma parcela do fluxo externo
			parcela_mw = fabs(p_mw * (p_viz_ext_mw / (p_mw + p_viz_int_mw)));
			lkc.Eext_mwh = -CalculaEnergia_mwh(parcela_mw, num_pat);
			lkc.Eint_mwh = 0.;
			return (true);
		}
		if ((p_viz_ext_mw < 0) && (p_viz_int_mw < 0))
		{ // caso 7: não há fluxo da Rede no Corte
			lkc.Eext_mwh = 0.;
			lkc.Eint_mwh = 0.;
			return (true);
		}
		Aviso("FluxoEnergiaDaRedeNoCorte_mwh() - caso supridora não previsto");
	}
	// Rede é consumidora no Corte
	else
	{
		// alterado em 2016.05.20
		if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
		{ // caso 8: fluxo da Rede é igual ao seu fluxo
			lkc.Eext_mwh = 0.;
			if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
			{
				lkc.Eint_mwh = 0.;
			}
			else if (p_viz_int_mw > 0)
			{
				lkc.Eint_mwh = 0.;
			}
			else
			{
				lkc.Eint_mwh = CalculaEnergia_mwh(p_mw, num_pat);
			}
			return (true);
		}
		// alterado em 2016.05.20
		if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
		{ // caso 9: fluxo da Rede é igual ao seu fluxo
			lkc.Eint_mwh = 0.;
			if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
			{
				lkc.Eext_mwh = 0.;
			}
			else if (p_viz_ext_mw > 0.)
			{
				lkc.Eext_mwh = 0.;
			}
			else
			{
				lkc.Eext_mwh = CalculaEnergia_mwh(p_mw, num_pat);
			}
			return (true);
		}
		if ((p_viz_ext_mw > 0) && (p_viz_int_mw > 0))
		{ // caso 10: não há fluxo da Rede no Corte (ocorre se houver gerador)
			lkc.Eext_mwh = 0.;
			lkc.Eint_mwh = 0.;
			return (true);
		}
		// alterado em 2016.05.20
		if ((p_viz_ext_mw < 0) && (p_viz_int_mw > 0))
		{ // caso 11: fluxo da Rede é igual ao inverso de uma parcela do fluxo interno
			lkc.Eext_mwh = CalculaEnergia_mwh(p_mw, num_pat);
			lkc.Eint_mwh = 0;
			return (true);
		}
		// alterado em 2016.05.20
		if ((p_viz_ext_mw > 0) && (p_viz_int_mw < 0))
		{ // caso 12: fluxo da Rede é igual ao inverso de uma parcela do fluxo externo
			lkc.Eext_mwh = 0.;
			lkc.Eint_mwh = CalculaEnergia_mwh(p_mw, num_pat);
			return (true);
		}
		// alterado em 2016.05.20
		if ((p_viz_ext_mw < 0) && (p_viz_int_mw < 0))
		{ // caso 13: calcula parcelas
			parcela_mw = fabs(p_mw * (p_viz_ext_mw / (p_viz_ext_mw + p_viz_int_mw)));
			lkc.Eext_mwh = CalculaEnergia_mwh(parcela_mw, num_pat);
			parcela_mw = fabs(p_mw * (p_viz_int_mw / (p_viz_ext_mw + p_viz_int_mw)));
			lkc.Eint_mwh = CalculaEnergia_mwh(parcela_mw, num_pat);
			return (true);
		}
		Aviso("FluxoEnergiaDaRedeNoCorte_mwh() - caso consumidora não previsto");
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaTec::ImprimeDEC_FEC_END(VTAlternativa *alternativa, int ano)
{
	// VTPath *path= (VTPath*)apl->GetObject(__classid(VTPath));
	// FILE *fout;
	// AnsiString arq;
	// arq = path->DirTmp(); //+ "\\DEC_FEC_END_planejamento.txt";
	// fout = fopen(arq.c_str(), "wt");
	double DEC, FEC, END;
	int ncons;
	TList *lisRede;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	strResRede *resRede;
	AnsiString straux, strDEC, strFEC, strEND;

	lisRede = new TList();
	lisRede = redes->LisRede();
	DEC = 0.0;
	FEC = 0.0;
	END = 0.0;
	ncons = 0;

	for (int i = 0; i < lisRede->Count; i++)
	{
		DEC = 0.0;
		FEC = 0.0;
		END = 0.0;
		ncons = 0;
		rede = (VTRede*)lisRede->Items[i];
		if (!rede->Carregada)
			continue;
		resRede = calc_ind_cont->ResCont->ResRede[rede];
		// hk201902: resRede = calc_ind_cont->ResRede[rede];
		ncons = resRede->ncons;
		if (ncons != 0)
		{
			DEC = resRede->tdeslN / ncons / 60 / 12;
			FEC = resRede->ndeslN / ncons / 12;
		}
		END = resRede->end / 12;
		if (resRede != NULL)
		{
			strDEC = DoubleToStr("%3.2f", DEC);
			strFEC = DoubleToStr("%3.2f", FEC);
			strEND = DoubleToStr("%3.2f", END);
			straux.sprintf("%d;%s;%s;%d;%s;%s;%s", ano, rede->Codigo.c_str(),
				alternativa->Codigo.c_str(), ncons, strDEC.c_str(), strFEC.c_str(), strEND.c_str());
		}
		slContinuidade->Add(straux);
	}

	// fclose(fout);
	return (true);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TAnaTec::IniciaAlternativas(void)
 {
 //variáveis locais
 VTAlternativa  *alternativa;
 VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
 VTEstudo       *estudo       = planejamento->ExisteEstudo();
 TList          *lisALTER     = estudo->LisAlternativa();

 //garante que todas as Alternativas tenham o objeto VTResumoTec
 for (int n = 0; n < lisALTER->Count; n++)
 {
 alternativa = (VTAlternativa*)lisALTER->Items[n];
 if (alternativa->Apl->GetObject(__classid(VTResumoTec)) == NULL)
 {//cria objeto VTResumoTec e insere na Alternativa
 alternativa->Apl->Add(NewObjResumoTec(ano_ini, ano_fim));
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TAnaTec::IniciaAlternativas(void)
{
	// variáveis locais
	VTAlternativa *alternativa;
	VTResumoTec *resumotec;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();
	TList *lisALTER = estudo->LisAlternativa();

	// garante que todas as Alternativas tenham o objeto VTResumoTec
	for (int n = 0; n < lisALTER->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALTER->Items[n];
		resumotec = (VTResumoTec*)alternativa->Apl->GetObject(__classid(VTResumoTec));
		if (resumotec != NULL)
		{ // destrói objeto VTResumoTec da Alternativa
			alternativa->Apl->Remove(resumotec);
			delete resumotec;
		}
		// cria um novo objeto ResumoTec p/a Alternativa
		alternativa->Apl->Add(NewObjResumoTec(ano_ini, ano_fim));
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15755250
//void __fastcall TAnaTec::IniciaLisEqptoInseridoSubstituido(TList *lisACAO)
void __fastcall TAnaTec::IniciaLisEqptoInseridoSubstituido(VTItemObra *itemObra, TList *lisACAO)
{
	// variáveis locais
	VTAcao *acao;
	VTEqpto *eqpto;
	VTRede *rede;
	strDadoInsercao *insercao;
	strDadoSubstituicao *substituicao;

	//protecao
	if(itemObra == NULL)
	{
		return;
    }

	// ATENÇÃO 1: esta função consulta lisREDE, que deve ser previamente preenchida
	// ATENÇÃO 2: esta função preenche lisEQP_INS e lisEQP_SUB, que devem ser previamente iniciadas
	// loop p/ todas Acoes em lisACAO
	for (int n = 0; n < lisACAO->Count; n++)
	{
		acao = (VTAcao*)lisACAO->Items[n];
		// verifica se Acao composta
		if (acao->Tipo() == acaoCOMPOSTA)
		{ // chamada recursiva com lista de Acoes da Acao composta
			IniciaLisEqptoInseridoSubstituido(itemObra, acao->LisAcao());
		}
		else
		{ // determina Eqpto da Acao
			eqpto = acao->Eqpto();
			// verifica se Eqpto é Rede ou Trecho
			if ((eqpto->Tipo() == eqptoREDE) || (eqpto->Tipo() == eqptoTRECHO))
			{ // verifica se Eqpto é Trecho
				if (eqpto->Tipo() == eqptoTRECHO)
				{ // desconsidera Trecho que pertence a uma Rede inserida
					rede = ((VTLigacao*)eqpto)->rede;
					// if (lisREDE->IndexOf(rede) >= 0) continue; //DVK 2017.01.23
					if (lisREDE->IndexOf(rede) >= 0)
						continue;
				}
				// verifica o tipo de Ação
				switch (acao->Tipo())
				{
				case acaoALTERA: // cria objeto TEqptoSub e insere em lisEQP_SUB
					lisEQP_SUB->Add(substituicao = new strDadoSubstituicao);
					substituicao->eqpto_inserido = acao->Eqpto();
					substituicao->eqpto_retirado = acao->CopiaDoEqptoOriginal();
					substituicao->variacao_taxa_falha_perc = itemObra->TaxaFalhaSubstituicao;
					break;
				case acaoINSERE: // cria objeto TEqptoIns e insere em lisEQP_INS
					lisEQP_INS->Add(insercao = new strDadoInsercao);
					insercao->eqpto = acao->CopiaDoEqptoOriginal();
					insercao->taxa_falha = itemObra->TaxaFalhaInsercao;
					break;
				default: // nada a fazer
					break;
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::IniciaLisRedeInserida(TList *lisACAO)
{
	// variáveis locais
	VTAcao *acao;
	VTEqpto *eqpto;

	// ATENÇÃO: esta função preenche lisREDE, que deve ser previamente iniciada

	// loop p/ todas Acoes em lisACAO
	for (int n = 0; n < lisACAO->Count; n++)
	{
		acao = (VTAcao*)lisACAO->Items[n];
		// verifica se Acao composta
		if (acao->Tipo() == acaoCOMPOSTA)
		{ // chamada recursiva com lista de Acoes da Acao composta
			IniciaLisRedeInserida(acao->LisAcao());
		}
		else
		{
			eqpto = acao->Eqpto();
			// verifica se Acao é de inserção de Rede
			if ((acao->Tipo() == acaoINSERE) && (eqpto->Tipo() == eqptoREDE))
			{ // insere Rede em lisREDE
				if (lisREDE->IndexOf(eqpto) < 0)
					lisREDE->Add(eqpto);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::IniciaResumoTec(VTAlternativa *alternativa, int ano, bool fluxo_ok)
{
	// variáveis locais
	VTResumoTec *resumo = (VTResumoTec*)alternativa->Apl->GetObject(__classid(VTResumoTec));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool existe_gd = false;

	// remove GDs existentes para montar a arvore radial
	existe_gd = ExisteGD();
	if (existe_gd)
		RemoveGD();
	// reinicia arvore de redes
	arvore->Clear();
	arvore->Inicia(redes);
	// reinsere GDs
	if (existe_gd)
		InsereGD();
	// Salva resultado de confiabilidade
	SalvaContinuidade(resumo, ano);
	// salva diagnóstico do fluxo
	resumo->Calculo_OK[ano] = fluxo_ok;
	// verifica se executou o cálculo de fluxo
	if (!fluxo_ok)
		return;
	// Salva energia consumida
	SalvaEnergiaEntregue(resumo, ano);
	// Salva energia injetada pelos suprimentos
	SalvaEnergiaEntrada(resumo, ano);
	// salva energia injetada por geradores
	SalvaEnergiaGerada(resumo, ano);
	// salva perdas
	SalvaPerdaTecnica(resumo, ano);
	// salva resumo do diagnóstico técnico
	SalvaDiagTecnico(resumo, ano);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::InsereGD(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// desfaz ultima ação, que foi remover todas as GDs
	edita->ActionUndo->Execute();

}

/*
 //---------------------------------------------------------------------------
 void __fastcall TAnaTec::SalvaDiagTecnico(VTResumoTec *resumo, int ano)
 {
 VTRedes     *redes    = (VTRedes*)   apl->GetObject(__classid(VTRedes));
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTLigacao   *pliga;
 VTBarra     *pbarra;
 TList       *lisPat = patamares->LisPatamar();
 TList       *lisEqptos, *lisEqpReg, *lisEqpRuim;
 TList       *lisTrecho;
 VTRede      *rede;
 int         nbar_ruim, nbar_reg, nbar_total = 0;
 int         ntraf_ruim, ntraf_reg, ntra_total = 0;
 double      ntre_ruim, ntre_reg, ntre_total = 0;
 double      perc_bar_ruim, perc_tre_ruim;

 //cria listas
 lisEqpReg = new TList();
 lisEqpRuim= new TList();
 lisEqptos = new TList();
 lisTrecho = new TList();
 //inicia quantidade total de barras e trechos
 nbar_total = redes->LisBarra()->Count;
 //totaliza comprimento total dos trechos
 for(int nr = 0; nr < redes->LisRede()->Count; nr++)
 {
 rede = (VTRede*)redes->LisRede()->Items[nr];
 rede->LisEqpto(lisTrecho, eqptoTRECHO);
 for (int nt = 0; nt < ; i < 10; i++) {

 }
 ntre_total += lisTrecho->Count;
 }
 if(nbar_total <= 0)
 {
 nbar_total = 1;
 }
 if(ntre_total <= 0)
 {
 ntre_total = 1;
 }

 //Avalia trafos
 lisEqptos->Clear();
 lisEqpReg->Clear();
 lisEqpRuim->Clear();
 lisEqptos->Assign(consulta->LisEqptoRede(redes, eqptoTRAFO), laOr);
 lisEqptos->Assign(consulta->LisEqptoRede(redes, eqptoTRAFO3E), laOr);
 lisEqptos->Assign(consulta->LisEqptoRede(redes, eqptoTRAFOZZ), laOr);
 ntraf_ruim = ntraf_reg = 0;
 for(int n = 0; n < lisEqptos->Count; n++)
 {
 pliga = (VTLigacao*)lisEqptos->Items[n];
 for(int np = 0; np < lisPat->Count; np++)
 {
 if(pliga->resflow->Diagnostico[np] != diagRUIM) continue;
 lisEqpRuim->Add(pliga);
 pliga = NULL;
 break;
 }
 if(pliga == NULL) continue;
 for(int np = 0; np < lisPat->Count; np++)
 {
 if(pliga->resflow->Diagnostico[np] != diagREGULAR) continue;
 lisEqpReg->Add(pliga);
 break;
 }
 }
 ntraf_ruim = lisEqpRuim->Count;
 ntraf_reg  = lisEqpReg->Count;

 //Avalia trechos
 lisEqptos->Clear();
 lisEqpReg->Clear();
 lisEqpRuim->Clear();
 lisEqptos->Assign(consulta->LisEqptoRede(redes, eqptoTRECHO), laOr);
 ntre_ruim = ntre_reg = 0;
 for(int n = 0; n < lisEqptos->Count; n++)
 {
 pliga = (VTLigacao*)lisEqptos->Items[n];
 for(int np = 0; np < lisPat->Count; np++)
 {
 if(pliga->resflow->Diagnostico[np] != diagRUIM) continue;
 lisEqpRuim->Add(pliga);
 pliga = NULL;
 break;
 }
 if(pliga == NULL) continue;
 for(int np = 0; np < lisPat->Count; np++)
 {
 if(pliga->resflow->Diagnostico[np] != diagREGULAR) continue;
 lisEqpReg->Add(pliga);
 break;
 }
 }
 ntre_ruim = lisEqpRuim->Count;
 ntre_reg  = lisEqpReg->Count;

 //Avalia barras
 lisEqptos->Clear();
 lisEqpReg->Clear();
 lisEqpRuim->Clear();
 lisEqptos->Assign(consulta->LisEqptoRede(redes, eqptoBARRA), laOr);
 nbar_ruim = nbar_reg = 0;
 for(int n = 0; n < lisEqptos->Count; n++)
 {
 pbarra = (VTBarra*)lisEqptos->Items[n];
 for(int np = 0; np < lisPat->Count; np++)
 {
 if(pbarra->resflow->Diagnostico[np] != diagRUIM) continue;
 lisEqpRuim->Add(pbarra);
 pbarra = NULL;
 break;
 }
 if(pbarra == NULL) continue;
 for(int np = 0; np < lisPat->Count; np++)
 {
 if(pbarra->resflow->Diagnostico[np] != diagREGULAR) continue;
 lisEqpReg->Add(pbarra);
 break;
 }
 }
 nbar_ruim = lisEqpRuim->Count;
 nbar_reg  = lisEqpReg->Count;

 resumo->DiagTrafo[ano][diagRUIM]    = ntraf_ruim;
 resumo->DiagTrafo[ano][diagREGULAR] = ntraf_reg;

 resumo->DiagTrechoKm[ano][diagRUIM]  = ntre_ruim;
 resumo->DiagTrecho[ano][diagREGULAR] = ntre_reg;
 perc_tre_ruim = (double)(ntre_ruim)/(double)(ntre_total);
 resumo->PercDiagTrecho[ano] = perc_tre_ruim;  //DVK 2013.12.20

 resumo->DiagBarra[ano][diagRUIM]    = nbar_ruim;
 resumo->DiagBarra[ano][diagREGULAR] = nbar_reg;
 perc_bar_ruim = (double)(nbar_ruim)/(double)(nbar_total);
 resumo->PercDiagBarra[ano] = perc_bar_ruim;   //DVK 2013.12.20


 if(lisEqpReg)  delete lisEqpReg;
 if(lisEqpRuim) delete lisEqpRuim;
 if(lisEqptos)  delete lisEqptos;
 if(lisTrecho)  delete lisTrecho;

 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TAnaTec::SalvaDiagTecnico(VTResumoTec *resumo, int ano)
 {
 VTBarra     *barra;
 VTLigacao   *ligacao;
 VTTrecho    *trecho;
 VTRede      *rede;
 VTRedes     *redes     = (VTRedes*)   apl->GetObject(__classid(VTRedes));
 int         nbar_ruim, nbar_reg, nbar_total;
 int         ntraf_ruim, ntraf_reg, ntraf_total;
 double      tre_ruim_km, tre_reg_km, tre_total_km;
 double      perc_bar_ruim, perc_tre_ruim;

 //obtém lista de todas Barras
 lisEQP->Clear();
 redes->LisBarra(lisEQP);
 //inicia quantidade total de barras
 nbar_total = lisEQP->Count;
 //totaliza Barras com diagnóstico regular e ruim
 nbar_ruim = nbar_reg = 0;
 for (int n = 0; n < lisEQP->Count; n++)
 {
 barra = (VTBarra*)lisEQP->Items[n];
 if (barra->resflow->ExisteDiagnostico(diagRUIM))
 {//incrementa número de Barras ruins
 nbar_ruim++;
 }
 else if (barra->resflow->ExisteDiagnostico(diagREGULAR))
 {//incrementa número de Barras regulares
 nbar_reg++;
 }
 }
 //obtém lista de todos Trafos
 lisEQP->Clear();
 redes->LisLigacao(lisEQP, eqptoTRAFO);
 redes->LisLigacao(lisEQP, eqptoTRAFO3E);
 //inicia quantidade total de Trafos
 ntraf_total = lisEQP->Count;
 //totaliza Trafos com diagnóstico regular e ruim
 ntraf_ruim = ntraf_reg = 0;
 for (int n = 0; n < lisEQP->Count; n++)
 {
 ligacao = (VTLigacao*)lisEQP->Items[n];
 if (ligacao->resflow->ExisteDiagnostico(diagRUIM))
 {//incrementa número de Trafos ruins
 ntraf_ruim++;
 }
 else if (ligacao->resflow->ExisteDiagnostico(diagREGULAR))
 {//incrementa número de Trafos regulares
 ntraf_reg++;
 }
 }
 //obtém lista de todos Trechos
 lisEQP->Clear();
 redes->LisLigacao(lisEQP, eqptoTRECHO);
 //totaliza Trechos com diagnóstico regular e ruim
 tre_ruim_km = tre_reg_km = tre_total_km = 0;
 for (int n = 0; n < lisEQP->Count; n++)
 {
 trecho = (VTTrecho*)lisEQP->Items[n];
 if (trecho->resflow->ExisteDiagnostico(diagRUIM))
 {//incrementa número de Trechos ruins
 tre_ruim_km += trecho->Comprimento_km;
 }
 else if (trecho->resflow->ExisteDiagnostico(diagREGULAR))
 {//incrementa número de Trechos regulares
 tre_reg_km += trecho->Comprimento_km;
 }
 //soma comprimento total
 tre_total_km += trecho->Comprimento_km;
 }
 //atualiza resumo de Barra
 perc_bar_ruim = (nbar_total == 0) ? 0. : (double(nbar_ruim)/double(nbar_total));
 resumo->DiagBarra[ano][diagRUIM]    = nbar_ruim;
 resumo->DiagBarra[ano][diagREGULAR] = nbar_reg;
 resumo->PercDiagBarra[ano]          = perc_bar_ruim;
 //atualiza resumo de Trafo
 resumo->DiagTrafo[ano][diagRUIM]    = ntraf_ruim;
 resumo->DiagTrafo[ano][diagREGULAR] = ntraf_reg;
 //atualiza resumo de Trecho
 perc_tre_ruim = IsDoubleZero(tre_total_km) ? 0. : (tre_ruim_km/tre_total_km);
 resumo->DiagTrechoKm[ano][diagRUIM]    = tre_ruim_km;
 resumo->DiagTrechoKm[ano][diagREGULAR] = tre_reg_km;
 resumo->PercDiagTrecho[ano]            = perc_tre_ruim;
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::LisLigacao(TList *lisLIG, VTBarra *barra, TList *lisEQP)
{
	// variáveis locais
	VTLigacao *ligacao;

	// determina as Ligacoes em lisLIG que conectam a Barra
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->ConectaBarra(barra))
		{ // insere Ligacao em lisEQP
			if (lisEQP->IndexOf(ligacao) < 0)
				lisEQP->Add(ligacao);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaTec::PossuiuReticulado(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool possui = false;
	TList *lisCLUSTER = redes->LisCluster();

	if (lisCLUSTER->Count > 0)
	{
		possui = true;
	}

	return possui;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::RemoveGD(void)
{
	// variáveis locais
	TList *lisGER = new TList();
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// limpa lista
	lisGER->Clear();
	// determina todos os geradores e remove todos eles
	redes->LisEqpto(lisGER, eqptoGERADOR);
	edita->RetiraLisEqpto(lisGER);

	// destroi lista
	delete lisGER;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaContinuidade(VTResumoTec *resumo, int ano)
{
	/*
	 //salva perda total, hwh/mes   PENDENCIA unidades
	 resumo->END[ano] = res_conf.end /12;// * DIAS_MES;
	 resumo->DEC[ano] = res_conf.dec /12;  //DVK 2015.08.12
	 resumo->FEC[ano] = res_conf.fec /12;  //DVK 2015.08.12
	 */
	// variáveis locais
	double dec, fec, end, comp;
	double denominador = 0.0;

	dec = fec = end = comp = 0.0;
	if (calc_ind_cont != NULL)
	{ // calcula valores
		end = calc_ind_cont->ResCont->ResGeral->end; // hk201902: calc_ind_cont->ResGeral->end;
		denominador = calc_ind_cont->ResCont->ResGeral->ncons;
		// verifica se denominador é zero
		if (!IsDoubleZero(denominador))
		{
			dec = (calc_ind_cont->ResCont->ResGeral->tdeslN / 60.) / denominador;
			// hk201902: ((calc_ind_cont->ResGeral->tdeslN /60.) / calc_ind_cont->ResGeral->ncons);
			fec = (calc_ind_cont->ResCont->ResGeral->ndeslN / denominador);
		}
		// hk201902: (calc_ind_cont->ResGeral->ndeslN / calc_ind_cont->ResGeral->ncons);
		comp = CalculaCompensacao();
	}
	// salva no resumo
	resumo->END[ano] = end / 12; // MWh/ano   => MWh/mês
	resumo->DEC[ano] = dec / 12; // hora/ano  => hora/mês
	resumo->FEC[ano] = fec / 12; // falha/ano => falha/mês
	resumo->Compensacao[ano] = comp / 12; // R$/ano => R$/mês

	// temporário: zera valores
	// resumo->END[ano] = 0;
	// resumo->DEC[ano] = 0;
	// resumo->FEC[ano] = 0;
	// resumo->Compensacao[ano] = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaContinuidade(VTResumoTec * resumo, int ano, VTRede * rede)
{
	/*
	 //salva perda total, hwh/mes   PENDENCIA unidades
	 resumo->END[ano] = res_conf.end /12;// * DIAS_MES;
	 resumo->DEC[ano] = res_conf.dec /12;  //DVK 2015.08.12
	 resumo->FEC[ano] = res_conf.fec /12;  //DVK 2015.08.12
	 */
	// variáveis locais
	double dec, fec, end, comp;
	double denominador = 0.0;

	dec = fec = end = comp = 0.0;
	if ((calc_ind_cont != NULL) && (calc_ind_cont->ResCont->ResRede[rede] != NULL))
	{ // calcula valores
		end = calc_ind_cont->ResCont->ResRede[rede]->end; // hk201902: calc_ind_cont->ResGeral->end;
		denominador = calc_ind_cont->ResCont->ResGeral->ncons;
		// verifica se denominador é zero
		if (!IsDoubleZero(denominador))
		{
			dec = (calc_ind_cont->ResCont->ResRede[rede]->tdeslN / 60.) / denominador;
			// hk201902: ((calc_ind_cont->ResGeral->tdeslN /60.) / calc_ind_cont->ResGeral->ncons);
			fec = (calc_ind_cont->ResCont->ResRede[rede]->ndeslN / denominador);
		}
		else
		{
			dec = fec = 0.;
		}
		// hk201902: (calc_ind_cont->ResGeral->ndeslN / calc_ind_cont->ResGeral->ncons);
		comp = calc_ind_cont->ResCont->ResRede[rede]->compensacao;
	}
	// salva no resumo
	resumo->END[ano] = end / 12; // MWh/ano   => MWh/mês
	resumo->DEC[ano] = dec / 12; // hora/ano  => hora/mês
	resumo->FEC[ano] = fec / 12; // falha/ano => falha/mês
	resumo->Compensacao[ano] = comp / 12; // R$/ano => R$/mês

	// temporário: zera valores
	// resumo->END[ano] = 0;
	// resumo->DEC[ano] = 0;
	// resumo->FEC[ano] = 0;
	// resumo->Compensacao[ano] = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaDiagTecnico(VTResumoTec * resumo, int ano)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	VTResumoTec* resumo_alim;

	// obtém lista de alimentadores
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		if (rede->TipoRede->Segmento == redePRI)
		{
			// determina resumo do alimentador
			resumo_alim = DeterminaResumo(rede, resumo);
			if (resumo_alim == NULL)
				continue;
			// obtém lista de todas Barras
			lisEQP->Clear();
			rede->LisBarra(lisEQP);
			SalvaDiagTecnicoBarra(resumo_alim, ano, lisEQP);
			// inicia lista de cargas
			lisEQP->Clear();
			rede->LisEqpto(lisEQP, eqptoCARGA);
			SalvaDiagTecnicoCarga(resumo_alim, ano, lisEQP);
			// obtém lista de todos Trafos
			lisEQP->Clear();
			rede->LisLigacao(lisEQP, eqptoTRAFO);
			rede->LisLigacao(lisEQP, eqptoTRAFO3E);
			SalvaDiagTecnicoTrafo(resumo_alim, ano, lisEQP);
			// obtém lista de todos Trechos
			lisEQP->Clear();
			rede->LisLigacao(lisEQP, eqptoTRECHO);
			SalvaDiagTecnicoTrecho(resumo_alim, ano, lisEQP);
			// calcula continuidade para a rede
			SalvaContinuidade(resumo_alim, ano, rede);
		}
	}

	// obtém lista de todas Barras
	lisEQP->Clear();
	redes->LisBarra(lisEQP);
	SalvaDiagTecnicoBarra(resumo, ano, lisEQP);

	// inicia lista de cargas
	lisEQP->Clear();
	redes->LisEqpto(lisEQP, eqptoCARGA);
	SalvaDiagTecnicoCarga(resumo, ano, lisEQP);

	// obtém lista de todos Trafos
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoTRAFO);
	redes->LisLigacao(lisEQP, eqptoTRAFO3E);
	SalvaDiagTecnicoTrafo(resumo, ano, lisEQP);

	// obtém lista de todos Trechos
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoTRECHO);
	SalvaDiagTecnicoTrecho(resumo, ano, lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaDiagTecnicoBarra(VTResumoTec * resumo, int ano, TList * lisBarra)
{
	// variaveis locais
	VTBarra *barra;
	int nbar_ruim, nbar_reg, nbar_total;
	double perc_bar_ruim, perc_bar_reg, perc_bar_bom;

	// inicia quantidade total de barras
	nbar_total = lisBarra->Count;
	// totaliza Barras com diagnóstico regular e ruim
	nbar_ruim = nbar_reg = 0;
	for (int n = 0; n < lisBarra->Count; n++)
	{
		barra = (VTBarra*)lisBarra->Items[n];
		if (barra->resflow->ExisteDiagnostico(diagRUIM))
		{ // incrementa número de Barras ruins
			nbar_ruim++;
		}
		else if (barra->resflow->ExisteDiagnostico(diagREGULAR))
		{ // incrementa número de Barras regulares
			nbar_reg++;
		}
	}
	// atualiza resumo de Barra
	perc_bar_ruim = (nbar_total == 0) ? 0. : (double(nbar_ruim) / double(nbar_total)) * 100;
	perc_bar_reg = (nbar_total == 0) ? 0. : (double(nbar_reg) / double(nbar_total)) * 100;
	perc_bar_bom = 100. - perc_bar_ruim - perc_bar_reg;
	resumo->DiagBarra[ano][diagRUIM] = nbar_ruim;
	resumo->DiagBarra[ano][diagREGULAR] = nbar_reg;
	resumo->DiagBarra[ano][diagBOM] = nbar_total - nbar_ruim - nbar_reg;
	resumo->PercDiagBarra[ano][diagRUIM] = perc_bar_ruim;
	resumo->PercDiagBarra[ano][diagREGULAR] = perc_bar_reg;
	resumo->PercDiagBarra[ano][diagBOM] = perc_bar_bom;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaDiagTecnicoCarga(VTResumoTec * resumo, int ano, TList * lisCarga)
{
	// variaveis locais
	VTCarga *carga;
	int ncar_ruim, ncar_reg, ncar_total;
	double perc_car_ruim, perc_car_reg, perc_car_bom;

	// totaliza Cargas com diagnóstico regular e ruim
	ncar_ruim = ncar_reg = ncar_total = 0;
	for (int n = 0; n < lisCarga->Count; n++)
	{
		carga = (VTCarga*)lisCarga->Items[n];
		if (carga->pbarra->resflow->ExisteDiagnostico(diagRUIM))
		{ // incrementa número de Cargas em barra ruins
			ncar_ruim += carga->NumConsTotal;
		}
		else if (carga->pbarra->resflow->ExisteDiagnostico(diagREGULAR))
		{ // incrementa número de Cargas em barra regulares
			ncar_reg += carga->NumConsTotal;
		}
		// totaliza cargas
		ncar_total += carga->NumConsTotal;
	}
	// atualiza resumo de Carga
	perc_car_ruim = (ncar_total == 0) ? 0. : (double(ncar_ruim) / double(ncar_total)) * 100;
	perc_car_reg = (ncar_total == 0) ? 0. : (double(ncar_reg) / double(ncar_total)) * 100;
	perc_car_bom = 100. - perc_car_ruim - perc_car_reg;
	resumo->DiagCarga[ano][diagRUIM] = ncar_ruim;
	resumo->DiagCarga[ano][diagREGULAR] = ncar_reg;
	resumo->DiagCarga[ano][diagBOM] = ncar_total - ncar_ruim - ncar_reg;
	resumo->PercDiagCarga[ano][diagRUIM] = perc_car_ruim;
	resumo->PercDiagCarga[ano][diagREGULAR] = perc_car_reg;
	resumo->PercDiagCarga[ano][diagBOM] = perc_car_bom;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaDiagTecnicoTrafo(VTResumoTec * resumo, int ano, TList * lisTrafo)
{
	// variaveis locais
	VTLigacao *ligacao;
	int ntraf_ruim, ntraf_reg, ntraf_total;
	double perc_tra_ruim, perc_tra_reg, perc_tra_bom;

	// inicia quantidade total de Trafos
	ntraf_total = lisTrafo->Count;
	// totaliza Trafos com diagnóstico regular e ruim
	ntraf_ruim = ntraf_reg = 0;
	for (int n = 0; n < lisTrafo->Count; n++)
	{
		ligacao = (VTLigacao*)lisTrafo->Items[n];
		if (ligacao->resflow->ExisteDiagnostico(diagRUIM))
		{ // incrementa número de Trafos ruins
			ntraf_ruim++;
		}
		else if (ligacao->resflow->ExisteDiagnostico(diagREGULAR))
		{ // incrementa número de Trafos regulares
			ntraf_reg++;
		}
	}
	// atualiza resumo de Trafo
	perc_tra_ruim = (ntraf_total == 0) ? 0. : (double(ntraf_ruim) / double(ntraf_total)) * 100;
	perc_tra_reg = (ntraf_total == 0) ? 0. : (double(ntraf_reg) / double(ntraf_total)) * 100;
	perc_tra_bom = 100. - perc_tra_ruim - perc_tra_reg;
	resumo->DiagTrafo[ano][diagRUIM] = ntraf_ruim;
	resumo->DiagTrafo[ano][diagREGULAR] = ntraf_reg;
	resumo->DiagTrafo[ano][diagBOM] = ntraf_total - ntraf_ruim - ntraf_reg;
	resumo->PercDiagTrafo[ano][diagRUIM] = perc_tra_ruim;
	resumo->PercDiagTrafo[ano][diagREGULAR] = perc_tra_reg;
	resumo->PercDiagTrafo[ano][diagBOM] = perc_tra_bom;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaDiagTecnicoTrecho(VTResumoTec * resumo, int ano, TList * lisTrecho)
{
	// variaveis locais
	VTTrecho *trecho;
	double tre_ruim_km, tre_reg_km, tre_bom_km, tre_total_km;

	// totaliza Trechos com diagnóstico regular e ruim
	tre_ruim_km = tre_reg_km = tre_total_km = tre_bom_km = 0;
	for (int n = 0; n < lisTrecho->Count; n++)
	{
		trecho = (VTTrecho*)lisTrecho->Items[n];
		if (trecho->resflow->ExisteDiagnostico(diagRUIM))
		{ // incrementa número de Trechos ruins
			tre_ruim_km += trecho->Comprimento_km;
		}
		else if (trecho->resflow->ExisteDiagnostico(diagREGULAR))
		{ // incrementa número de Trechos regulares
			tre_reg_km += trecho->Comprimento_km;
		}
		else if (trecho->resflow->ExisteDiagnostico(diagBOM))
		{ // incrementa número de Trechos bons
			tre_bom_km += trecho->Comprimento_km;
		}
		// soma comprimento total
		tre_total_km += trecho->Comprimento_km;
	}
	// atualiza resumo de Trecho
	resumo->DiagTrechoKm[ano][diagRUIM] = tre_ruim_km;
	resumo->DiagTrechoKm[ano][diagREGULAR] = tre_reg_km;
	resumo->DiagTrechoKm[ano][diagBOM] = tre_bom_km;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaEnergiaEntrada(VTResumoTec * resumo, int ano)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	VTResumoTec *resumo_alim;
	TList *lisSuprimentos, *lisAux;
	double entrada_diaria_mwh;

	// cria lista
	lisSuprimentos = new TList();
	// obtém lista de alimentadores
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->TipoRede->Segmento == redePRI)
		{
			resumo_alim = DeterminaResumo(rede, resumo);
			if (resumo_alim == NULL)
				continue;
			// lista suprimentos
			// lisSuprimentos->Clear();
			// rede->LisEqbar(lisSuprimentos, eqptoSUPRIMENTO);
			// // atualiza por rede
			// SalvaEnergiaEntrada(resumo_alim, ano, lisSuprimentos);
			// calcula
			entrada_diaria_mwh = CalculaEnergiaRecebida(rede);
			// salva energia entrada, mwh/mes
			resumo_alim->Entrada_mwhm[ano] = entrada_diaria_mwh * DIAS_MES;
		}
	}
	// destroi lista
	delete lisSuprimentos;
	// atualiza total
	lisAux = consulta->LisEqptoRedeReducao(redes, eqptoSUPRIMENTO);
	SalvaEnergiaEntrada(resumo, ano, lisAux);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaEnergiaEntrada(VTResumoTec * resumo, int ano, TList * lisSuprimentos)
{
	// variáveis locais
	double entrada_diaria_mwh;
	double entrada_mwh;
	VTSuprimento *suprimento;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// inicia energia de entrada diaria total
	entrada_diaria_mwh = 0;

	// loop p/ todos Suprimentos
	for (int ns = 0; ns < lisSuprimentos->Count; ns++)
	{
		suprimento = (VTSuprimento*)lisSuprimentos->Items[ns];
		// calcula energia de entrada diaria do Suprimento
		entrada_mwh = 0.;
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			entrada_mwh += suprimento->resflow->P_mw[ind_pat] * patamares->Patamar[ind_pat]
				->Duracao;
		}
		entrada_diaria_mwh += entrada_mwh;
	}
	// salva entrada total, mwh/mes
	resumo->Entrada_mwhm[ano] = entrada_diaria_mwh * DIAS_MES;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaEnergiaEntregue(VTResumoTec * resumo, int ano)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	VTResumoTec *resumo_alim;
	TList *lisCargas, *lisAux;

	// cria lista
	lisCargas = new TList();
	// obtém lista de alimentadores
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		if (rede->TipoRede->Segmento == redePRI)
		{
			resumo_alim = DeterminaResumo(rede, resumo);
			if (resumo_alim == NULL)
				continue;
			// lista cargas
			lisCargas->Clear();
			rede->LisEqbar(lisCargas, eqptoCARGA);
			// atualiza por rede
			SalvaEnergiaEntregue(resumo_alim, ano, lisCargas);
		}
	}
	// destroi lista
	delete lisCargas;
	// atualiza total
	lisAux = consulta->LisEqptoRedeReducao(redes, eqptoCARGA);
	SalvaEnergiaEntregue(resumo, ano, lisAux);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaEnergiaEntregue(VTResumoTec * resumo, int ano, TList * lisCargas)
{
	// variáveis locais
	double entregue_diaria_mwh;
	double entregue_mwh;
	VTCarga *carga;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	double p_pat_mw, v_pu;

	// inicia energia de entrada diaria total
	entregue_diaria_mwh = 0;
	// loop p/ todos Cargas
	for (int ns = 0; ns < lisCargas->Count; ns++)
	{
		carga = (VTCarga*)lisCargas->Items[ns];
		// calcula demanda da Carga
		entregue_mwh = 0.;
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{ // verifica se o fluxo foi calculado no patamar
			if (carga->pbarra->resflow->Diagnostico[ind_pat] == diagISOLADO)
				continue;
			// determina tensão mínima da Barra
			v_pu = carga->pbarra->resflow->Vff_pu_min[ind_pat];
			// carga->pbarra->resflow->Vff_pu_min[ind_pat];
			p_pat_mw = demanda->P_mw[carga][ind_pat];
			switch (carga->ModeloCarga)
			{
			case cargaICTE:
				p_pat_mw *= v_pu;
				break;
			case cargaZCTE:
				p_pat_mw *= (v_pu * v_pu);
				break;
			case cargaSCTE:
				break;
			default:
				break;
			}
			entregue_mwh += p_pat_mw * patamares->Patamar[ind_pat]->Duracao;
		}
		entregue_diaria_mwh += entregue_mwh;
	}
	// salva entregue total, mwh/mes
	resumo->Entregue_mwhm[ano] = entregue_diaria_mwh * DIAS_MES;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaEnergiaGerada(VTResumoTec * resumo, int ano)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	VTResumoTec *resumo_alim;
	TList *lisGeradores;

	// cria lista
	lisGeradores = new TList();
	// obtém lista de alimentadores
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		if (rede->TipoRede->Segmento == redePRI)
		{
			resumo_alim = DeterminaResumo(rede, resumo);
			if (resumo_alim == NULL)
				continue;
			// lista geradores
			lisGeradores->Clear();
			rede->LisEqbar(lisGeradores, eqptoGERADOR);
			// atualiza por rede
			SalvaEnergiaGerada(resumo_alim, ano, lisGeradores);
		}
	}

	// atualiza total
	lisGeradores->Clear();
	redes->LisEqbar(lisGeradores, eqptoGERADOR);
	SalvaEnergiaGerada(resumo, ano, lisGeradores);
	// destroi lista
	delete lisGeradores;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaEnergiaGerada(VTResumoTec * resumo, int ano, TList * lisGeradores)
{
	// variáveis locais
	double gerada_dia_mwh;
	double gerada_mwh;
	VTGerador *gerador;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// inicia energia gerada no dia
	gerada_dia_mwh = 0;
	for (int ng = 0; ng < lisGeradores->Count; ng++)
	{
		gerador = (VTGerador*)lisGeradores->Items[ng];
		// calcula energia gerada pelo Gerador
		gerada_mwh = 0.;
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			gerada_mwh += gerador->resflow->P_mw[ind_pat] * patamares->Patamar[ind_pat]->Duracao;
		}
		gerada_dia_mwh += gerada_mwh;
	}
	// salva energia gerada no mês, mwh/mes
	resumo->Gerada_mwhm[ano] = gerada_dia_mwh * DIAS_MES;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaPerdaTecnica(VTResumoTec * resumo, int ano)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	VTResumoTec *resumo_alim;
	TList *lisLigacao;

	// cria lista
	lisLigacao = new TList();
	// obtém lista de alimentadores
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		if (rede->TipoRede->Segmento == redePRI)
		{
			resumo_alim = DeterminaResumo(rede, resumo);
			if (resumo_alim == NULL)
				continue;
			// lista geradores
			lisLigacao->Clear();
			rede->LisLigacao(lisLigacao);
			// atualiza por rede
			SalvaPerdaTecnica(resumo_alim, ano, lisLigacao);
		}
	}

	// atualiza total
	lisLigacao->Clear();
	redes->LisLigacao(lisLigacao);
	SalvaPerdaTecnica(resumo, ano, lisLigacao);
	// destroi lista
	delete lisLigacao;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaTec::SalvaPerdaTecnica(VTResumoTec * resumo, int ano, TList * lisLigacao)
{
	// variáveis locais
	double energia_mwhm;
	double perda_ligacao_mwh;
	double perda_diaria_mwh;
	VTLigacao *ligacao;

	// inicia perda diaria total
	perda_diaria_mwh = 0;
	// loop p/ todas Redes
	for (int nl = 0; nl < lisLigacao->Count; nl++)
	{
		ligacao = (VTLigacao*)lisLigacao->Items[nl];
		if (ligacao->Tipo() == eqptoCHAVE)
			continue;
		// calcula perda diaria da Ligacao
		perda_ligacao_mwh = ligacao->resflow->PerdaDiaria_mwh;
		perda_diaria_mwh += perda_ligacao_mwh;
	}
	// salva perda total, mwh/mes
	resumo->Perda_mwhm[ano] = perda_diaria_mwh * DIAS_MES;
	// salva perda percentual
	energia_mwhm = resumo->Entrada_mwhm[ano];
	if (IsDoubleZero(energia_mwhm))
		resumo->Perda_perc[ano] = 0.;
	else
		resumo->Perda_perc[ano] = 100. * (resumo->Perda_mwhm[ano] / energia_mwhm);
}

// ---------------------------------------------------------------------------
// eof
