// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCalcIndCont.h"
//hk201808: nao utilizado #include "TFormChvAuto.h"
#include "TFormMostraDefeito.h"
#include "TFormEditaParamProtJus.h"
#include "TFormAlternativasChaves.h"
#include "TFormEliminaFusivel.h"
#include "TFormInsereConjunto.h"
// #include  <ProjetosGerais\Fontes\Testes\TFormParamAux.h>

#include "..\VTMostraGrafIndic.h"
#include "..\VTCalcIndCont.h"
#include "..\VTBuscaProt.h"
#include "..\VTBarraRad.h"
#include "..\VTVisRede.h"
#include "..\VTVisRedeSoc.h"
#include "..\VTDefeito.h"
#include "..\TCargaBloco.h"
#include "..\VTBlocoRad.h"
#include "..\VTEstratSoc.h"
#include "..\VTResCont.h"
#include "..\VTLigaCrit.h"

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Bloco\VTElo.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>  //hk201612: alteracao JCG
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Classes\VTClasse.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Report\VTReport.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapChild.h>
#include <PlataformaSinap\Fontes\Stk\VTStkEqv.h>
#include <PlataformaSinap\Fontes\Editor\VTEdt.h>
#include "..\..\..\DLL_Inc\CalcIndCont.h"
#include "..\..\..\DLL_Inc\BdadoParam.h"
#include <PlataformaSinap\DLL_Inc\Editor.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Progresso.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Report.h>
#include <PlataformaSinap\DLL_Inc\Stk.h>

// #include <ProjetoEDP\Fontes\ExtratorDados\Bdado_Externo\VTBDado_Externo.h>
// #include <ProjetoEDP\DLL_Inc\EDP_ExtratorDados.h>
// #include <ProjetoEDP\Fontes\ExtratorDados\Dados\VTOcorr.h>
// #include <ProjetoEDP\Fontes\ExtratorDados\Dados\VTEqptoOc.h>
// #include <ProjetosGerais\Fontes\BdadoParam\Modela\VTGeraLisParamEqpto.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma link "TFormEdt"
#pragma resource "*.dfm"
// ---------------------------------------------------------------------------
// declaração
ORDENACAO listViewSort;

// ---------------------------------------------------------------------------
__fastcall TFormCalcIndCont::TFormCalcIndCont(TComponent* Owner, VTApl* apl_owner,
	TWinControl *parent) : TFormEdt(Owner, parent)
{
	// hk4.0 VTStk *stk;
	// this->apl = apl_owner;

	// insere Form em seu Parent
	if (parent)
		Parent = parent;

	// Inicia em tab de parametros gerais
	PageControl2->ActivePageIndex = 0;
	PageControl3->ActivePageIndex = 0;
	PageControl7->ActivePageIndex = 0;

	this->Show();
	this->Refresh();

	// Inicia objetos
	// stk  = (VTStk*)apl_owner->GetObject(__classid(VTStk));
	// this->apl = stk->NewObjApl(this, apl_owner);
	// this->apl = stk->NewObjApl(NULL, aplOwner);
	this->apl = NewObjApl(NULL, apl_owner); // hk4.0

	// JCG 2016.11.07 - verifica existência de redes BT equivalentes
   //	if (TrocaRedeEqvBTporCargaEqv(apl_owner) == true)
	//{
		// Cria lista de chaves para fusao de blocos
		lisChavesFunde = new TList();

		// !!Pendencia!! alterar para funcao em calcIndCont para
		// identificar chaves que ligam carga a rede, hj baseado em código FU
		// lisChavesFunde: chaves na rede final com eliminacao de chaves e fusao de blocos
		// se há fusao lisChavesRede->Count > lisChavesFunde->Count
		IniciaLisChavesFunde(lisChavesFunde);

		int currCursor = Screen->Cursor;
		StatusBarCount->SimpleText = "Iniciando módulo de cálculo de indicadores..";
		Screen->Cursor = crHourGlass;
		apl->Add(calcIndCont = DLL_NewCalcIndCont(apl, lisChavesFunde));
		// NULL));//flagVis = true -->executa fluxPot
		// VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
		// grafico->Moldura(lisChavesFunde);

		apl_owner->Add(calcIndCont->Blocos); // graf

		// Le arquivos e prepara dados
		// gera lisParamEqpto->paramEqpto(lambda, trep)
		// gera lisParamBloco->paramBloco->(lambda, trep)
		// hktmp calcIndCont->LeArquivos("");
		StatusBarCount->SimpleText = "Lendo arquivo de parâmetros..";
		calcIndCont->LeArquivosMdb("");
		lblPathParam->Caption = calcIndCont->ParamConf->PathParam; //hk201808: reformula: calcIndCont->PathParam;
		StatusBarCount->SimpleText = "Leitura de parâmetros finalizada.";
		// hkv201608a: retira if(calcIndCont->LisVerifCjto->Count == 0) calcIndCont->IniciaVerifDefault();
		// Verifica se há parametros históricos
		if (calcIndCont->ParamConf->LisParamEqpto->Count > 0)
			butReiniciaParHist->Enabled = true;
		else
			butReiniciaParHist->Enabled = false;

		lisRedeOrdenada = new TList();
		IniciaLisRedOrdenada();

		// TVChavesInicia(calcIndCont->ParamConf->ParamGerais->flagRecurso);//hk1408
		TVChavesInicia(); // hk1408
		// Atualiza interface de dados gerais
		AtualizaInterfaceDadosGerais();
		// Atualiza listView
		LvParamRedeInicia();
		LvParamBlocoInicia();
		// Inicia grid de ajuste de valores
		SgridAjusteInicia();

		lisBarraOc = new TList();
		lisParamEqpto = new TList();
		lisParamRede = new TList();
		// lisVerifCjto = new TList();

		// Seta parametro de reinicialização de parâmetros
		// para valores históricos dos blocos
		chkBoxReiniciaParam->Checked[0] = true;
		Screen->Cursor = (TCursor)currCursor;

		// TODO - Comentado para o SETUP da Coelba
		TabSheet18->TabVisible = true;


//	}
//	else
//	{
//		this->Close();
//	}
}

// ---------------------------------------------------------------------------
__fastcall TFormCalcIndCont::~TFormCalcIndCont(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->DestacaEqpto(NULL, clBlue, 10);
	grafico->Moldura();
	grafico->Show(grafTOPO);

	if (edt_busca != NULL)
	{
		delete edt_busca;
		edt_busca = NULL;
	}

	if (calcIndCont)
	{
		// Grava arquivo com diretório para o próximo acesso
		calcIndCont->ParamConf->GravaDirConf(apl); //hk201808: reformula: calcIndCont->GravaDirConf();
		// Reinicia chaves com status inicial
		calcIndCont->ReiniciaChavesAuto();
		// Deleta apl do form
		if (apl)
			apl->Owner->Remove(calcIndCont->Blocos); // graf
	}

	if (apl)
	{
		delete apl;
		apl = NULL;
	}

	if (lisBarraOc)
	{
		LimpaTList(lisBarraOc);
		delete lisBarraOc;
		lisBarraOc = NULL;
	}
	if (lisParamEqpto)
	{
		LimpaTList(lisParamEqpto);
		delete lisParamEqpto;
		lisParamEqpto = NULL;
	}
	if (lisParamRede)
	{
		LimpaTList(lisParamRede);
		delete lisParamRede;
		lisParamRede = NULL;
	}
	// if(lisVerifCjto)   {LimpaTList(lisVerifCjto); delete lisVerifCjto; lisVerifCjto = NULL;}
	if (lisRedeOrdenada)
	{
		delete lisRedeOrdenada;
		lisRedeOrdenada = NULL;
	}
	if (lisChavesFunde)
	{
		delete lisChavesFunde;
		lisChavesFunde = NULL;
	}
}

/*
// ---------------------------------------------------------------------------
bool __fastcall TFormCalcIndCont::ExisteRedeEqvBT(void)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		// if (rede->Carregada) continue;  //DVK 2016.11.28 comentada, pois qualquer
		// rede secundária será trocada por carga
		if (rede->TipoRede->Segmento == redeSEC)
			return (true);
	}
	return (false);
}
*/


// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionNAParalelasExecute(TObject *Sender)
{
       IdentificaChavesNASerieParalela();
}
//---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::IdentificaChavesNASerieParalela(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTChave *chave1, *chave2;
	TList *lisChaves, *lisChavesProblema;

	lisChaves = new TList();

    redes->LisEqpto(lisChaves, eqptoCHAVE);
	lisChavesProblema = new TList();
	for(int n = lisChaves->Count - 1; n >=0; n--)
	   {
	   if(((VTChave*)lisChaves->Items[n])->Aberta) continue;
	   lisChaves->Delete(n);
	   }

	VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	for(int n1 = 0; n1 < lisChaves->Count; n1++)
	   {
	   chave1 = (VTChave*)lisChaves->Items[n1];
	   for(int n2 = n1 + 1; n2 < lisChaves->Count; n2++)
		  {
		  chave2 = (VTChave*)lisChaves->Items[n2];
		  if((chave1->pbarra1 != chave2->pbarra1)&&(chave1->pbarra1 != chave2->pbarra2)&&
		     (chave1->pbarra2 != chave2->pbarra1)&&(chave1->pbarra2 != chave2->pbarra2)) continue;

		  if(lisChavesProblema->IndexOf(chave1) == -1)
			 lisChavesProblema->Add(chave1);
		  //if(lisChavesProblema->IndexOf(chave2) == -1)
		  //	 lisChavesProblema->Add(chave2);
		  }
	   }

    grafico->Moldura(lisChavesProblema);
	delete lisChaves;
	delete lisChavesProblema;
}
// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::IniciaLisChavesFunde(TList *lisChavesFunde)
{
	// Funcao que preenche lista de chaves para fusao de blocos
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTChave *chave;
	AnsiString codChave = "FU";

	lisChavesFunde->Clear();
	redes->LisEqpto(lisChavesFunde, eqptoCHAVE);
	for (int nc = lisChavesFunde->Count - 1; nc >= 0; nc--)
	{
		chave = (VTChave*)lisChavesFunde->Items[nc];
		if (chave->Codigo.AnsiCompare(codChave) != 0)
			continue;
		lisChavesFunde->Delete(nc);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::IniciaLisRedOrdenada(void)
{
	// Funcao que preenche lista lisRedeOrdenada com redes ordenadas alfabeticamente
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRadial *radial;
	VTRede *rede, *redeMax;
	TList *lisRedesAux;
	int Compare;

	lisRedeOrdenada->Clear();
	lisRedesAux = new TList();
	radial = DLL_NewObjRadial(apl);
	radial->Inicia(redes);
	lisRedesAux->Assign(redes->LisRede(), laOr);
	// Retira redes não carregadaas
	for (int n = lisRedesAux->Count - 1; n >= 0; n--)
	{
		rede = (VTRede*)lisRedesAux->Items[n];
		// hkv201608a IniciaLisRedOrdenada: Verifica se a rede corresponde a circuito primário, MT : if((rede->Carregada)&&(radial->ExistePrimario(rede))) continue;
		// if((rede->Carregada)&&(radial->ExistePrimario(rede))) continue;

		// hk201701 - inclusão de todas as redes carregadas não só alimentadores primários
		if (rede->Carregada)
			continue;

		// Exclui rede
		lisRedesAux->Delete(n);
	}

	while (lisRedesAux->Count > 0)
	{
		redeMax = (VTRede*)lisRedesAux->Items[0];
		for (int n = 1; n < lisRedesAux->Count; n++)
		{
			rede = (VTRede*)lisRedesAux->Items[n];
			if ((Compare = CompareText(rede->Codigo, redeMax->Codigo)) < 0)
			{
				redeMax = rede;
			}
		}
		lisRedeOrdenada->Add(redeMax);
		lisRedesAux->Remove(redeMax);
	}

	delete lisRedesAux;
	delete radial;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LvParamBlocoInicia(void)
{
	// Atualiza grid de parametros por bloco
	strDadoParamBloco *paramBloco;
	AnsiString codRede;
	TListItem *item;
	VTRede *rede;
	VTBloco *bloco;

	lvParamBloco->Clear();
	// hk201702: ordenação comum de blocos as redes calcIndCont->ListaOrdenadaBlocos(lisOrdBlocos, NULL);
	TList *lisOrdBlocos;
	lisOrdBlocos = new TList();

	for (int nr = 0; nr < lisRedeOrdenada->Count; nr++)
	{ // hk201702: Ordenação de apresentação por rede
		rede = (VTRede*)lisRedeOrdenada->Items[nr];
		if (!rede->Carregada)
			continue;
		lisOrdBlocos->Clear();
      calcIndCont->BuscaProt->ListaOrdenadaBlocos(lisOrdBlocos, rede); ////hk201808: sai: funcao de buscaProt: calcIndCont->ListaOrdenadaBlocos(lisOrdBlocos, rede);
		// hk201702: ordenação comum de blocos: for(int np = 0; np < calcIndCont->LisParamBloco->Count; np++)
		for (int np = 0; np < lisOrdBlocos->Count; np++)
		{
			bloco = (VTBloco*)lisOrdBlocos->Items[np];
			if ((paramBloco = calcIndCont->ParamConf->ParamBloco[bloco]) == NULL)
				continue;
			// hk201702: ordenação comum de blocos: paramBloco = (strDadoParamBloco*)calcIndCont->LisParamBloco->Items[np];
			// hk201702: ordenação comum de blocos:if(paramBloco->bloco == NULL) continue; //hlk201608 -bug chave, bloco = NULL
			// hk201702: ordenação comum de blocos:if(paramBloco->bloco->Rede != rede) continue; //hk201702: Ordenação de apresentação por rede
			item = lvParamBloco->Items->Add();
			item->Data = paramBloco;
			item->Caption = IntToStr(np + 1);
			item->SubItems->Add(paramBloco->eqpto->Codigo); // hk201702: ordenação comum de blocos:
			codRede = "";
			if (paramBloco->bloco->Rede != NULL)
				codRede = paramBloco->bloco->Rede->Codigo;

			item->SubItems->Add(codRede);
			item->SubItems->Add(DoubleToStr("%6.2f", paramBloco->lambda));
			item->SubItems->Add(DoubleToStr("%6.2f", paramBloco->trep));
			// hk201702: LvParamBlocoInicia retirada: item->SubItems->Add(DoubleToStr("%6.2f",paramBloco->nFalhas));
			item->SubItems->Add(DoubleToStr("%6.2f", paramBloco->compBloco));
			// hk201702: LvParamBlocoInicia retirada: hk201702: item->SubItems->Add(DoubleToStr("%6.2f",paramBloco->duraTot));
		}
	}
	delete lisOrdBlocos;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LvParamRedeInicia(void)
{
	strDadoParamRede* paramRede;
	AnsiString dIni, dFim, aux;
	double tmaBT;
	VTRede *rede;
	// hk201701: Apresentação ordenada por lisRedeOrdenada
	// Atualiza grid de parametros por rede
	TListItem *item;
	double taux;
	lvParamRede->Clear();

	for (int nr = 0; nr < lisRedeOrdenada->Count; nr++)
	{
		rede = (VTRede*)lisRedeOrdenada->Items[nr];
		if (!rede->Carregada)
			continue;
		paramRede = NULL;
		//for (int np = 0; np < calcIndCont->LisParamRede->Count; np++)//hk201808: reformula
		for (int np = 0; np < calcIndCont->ParamConf->LisParamRede->Count; np++)
		{
			paramRede = (strDadoParamRede*)calcIndCont->ParamConf->LisParamRede->Items[np];
			if (paramRede->rede == rede)
				break;
			paramRede = NULL;
		}
		if (paramRede == NULL)
			continue;

		item = lvParamRede->Items->Add();
		item->Data = paramRede;
		item->Caption = paramRede->rede->Codigo;
		item->SubItems->Add(DoubleToStr("%6.2f", paramRede->lambda));
		if (paramRede->trep < 0.0)
			taux = 0.0;
		else
			taux = paramRede->trep;

		item->SubItems->Add(DoubleToStr("%6.2f", taux));
		item->SubItems->Add(DoubleToStr("%6.2f", paramRede->lambda * paramRede->compRede));
		item->SubItems->Add(DoubleToStr("%6.2f", paramRede->compRede));
		item->SubItems->Add(DoubleToStr("%6.2f", taux * paramRede->lambda * paramRede->compRede));
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::AtualizaInterfaceDadosGerais(void)
{
	// cbRecAuto->Checked  = calcIndCont->FlagAutoSoc;//flagAutoSoc;
	cbRecurso->Checked = calcIndCont->ParamConf->ParamGerais->flagRecurso;//hk201808: reformula: calcIndCont->FlagRecurso; // flagRecurso;
	cbRetorno->Checked = calcIndCont->ParamConf->ParamGerais->flagRetorno;//hk201808: reformula: calcIndCont->FlagRetorno;


	// limpa TRichs
	edtTLoc->Clear();
	edtTMan->Clear();
	edtTManAuto->Clear();
	edtTRep->Clear();
	edtLambda->Clear();
	edtPuLim->Clear();
	edtPercDefTrans->Clear();
	edtRedTprepIF->Clear();

	// inicia
	edtTLoc->Text = DoubleToStr("%.0f", calcIndCont->ParamConf->ParamGerais->tlocaliza);//hk201808: reformula:DoubleToStr("%.0f", calcIndCont->TLocaliza); // tlocaliza);
	edtTMan->Text = DoubleToStr("%.0f", calcIndCont->ParamConf->ParamGerais->tman_manual); //hk201808: reformula:DoubleToStr("%.0f", calcIndCont->TManManual); // tman_manual);

	edtTManAuto->Text = DoubleToStr("%.0f", calcIndCont->ParamConf->ParamGerais->tman_auto); //hk201808: reformula:DoubleToStr("%.0f", calcIndCont->TManAuto); // tman_auto);
	edtTRep->Text = DoubleToStr("%.0f", calcIndCont->ParamConf->ParamGerais->trep); //hk201808: reformula:DoubleToStr("%.0f", calcIndCont->TReparo); // trep);
	edtLambda->Text = DoubleToStr("%.2f", calcIndCont->ParamConf->ParamGerais->lambda); // lambda);//hk201808: reformula:
	edtPuLim->Text = DoubleToStr("%.2f", calcIndCont->ParamConf->ParamGerais->pu_lim);//hk201808: reformula: DoubleToStr("%.2f", calcIndCont->PuLim); // pu_lim);
	edtPercDefTrans->Text = DoubleToStr("%.2f", calcIndCont->ParamConf->ParamGerais->percDefTrans * 100.);//hk201808: reformula: DoubleToStr("%.2f", calcIndCont->PercDefTrans * 100.);
	//gokh201806: acréscimo de fator de redução de tempo de preparo, devido à instalação de IF em fusível
	edtRedTprepIF->Text = DoubleToStr("%.2f", calcIndCont->ParamConf->ParamGerais->red_tprep_IF * 100.);//hk201808: reformula: DoubleToStr("%.2f", calcIndCont->RedTprepIF * 100.);
   edtIndispPadrao->Text = DoubleToStr("%.2f", calcIndCont->ParamConf->EqptoIndispPadrao->vindisp);

	rgZonaReliga->ItemIndex = calcIndCont->IProfReliga;

	// flagAutoSoc = false; //true: aceita socorros com operações telecom (isolamto e socorr); false: caso contrário
	// Inicia % falhas por patamar
	SgridFalhasPatInicia();
	ChkLstTipoProtInicia();
	SgridParamRedeBlocoInicia();
	// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia

	// Inicia list box de chaves automaticas e chaves Novas
	LbChavesInicia();
	LbChavesIFInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LbChavesIFInicia(void) // hk201710IF
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisChaves;
	// hk1113 TList   *lisRede = redes->LisRede();
	TList *lisRede = lisRedeOrdenada;
	VTChave *pchave;
	TListItem *item;
	AnsiString aux;
	VTTipoChave *tipoChave;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTipoChaves;
	int nconsProt;
	double cargaProt, distProt;

	// Reinicia as listboxes
	lbChavesSemIF->Clear();
	lbChavesComIF->Clear();

	lisTipoChaves = tipos->LisTipoChave();

	lisChaves = new TList();
   calcIndCont->BuscaProt->LisChaveOrd(lisChaves);//hk201808: reformula sai: calcIndCont->LisChavesOrd(lisChaves);
	// Ordena lista de chaves pelo códido
	OrdenaLisChaves(lisChaves);
	// CHAVES NF, FECHADAS
	for (int n = lisTipoChaves->Count - 1; n >= 0; n--)
	{
		tipoChave = (VTTipoChave*)lisTipoChaves->Items[n];
		if ((tipoChave->Codigo.AnsiCompare("Seccionadora") != 0) &&
			(tipoChave->Codigo.AnsiCompare("Base Fusível") != 0))
			continue;
		// if(tipoChave->Codigo.AnsiCompare("Seccionadora") != 0) continue;

		for (int nr = 0; nr < lisRede->Count; nr++)
		{
			rede = (VTRede*)lisRede->Items[nr];
			for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
			{
				pchave = (VTChave*)lisChaves->Items[nc];
				if (calcIndCont->LisChavesIF->IndexOf(pchave) != -1)
				//	int kkk = 0;
				if (pchave->Aberta)
					continue; // if(pchave->Estado == chvABERTA) continue;  //hkRevAloca
				if (pchave->TipoChave != tipoChave)
					continue;
				if (pchave->rede != rede)
					continue;
				lisChaves->Delete(nc);
				aux.sprintf("(%s)", tipoChave->Codigo.c_str());
				aux.cat_sprintf("(%s)", rede->Codigo.c_str());
				aux.cat_sprintf("(%s)", (pchave->Aberta) ? "NA: " : "NF: ");
				// aux.cat_sprintf("(%s)", (pchave->Estado == chvABERTA) ? "NA: " : "NF: "); //hkRevAloca

				if (calcIndCont->LisChavesIF->IndexOf(pchave) == -1)
					lbChavesSemIF->Items->AddObject(aux + pchave->Codigo, pchave);
				else
				{
					aux.cat_sprintf("%s", pchave->Codigo.c_str());
					lbChavesComIF->Items->AddObject(aux, pchave);
				}
			}
		}
	}
	delete lisChaves;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LbChavesInicia(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisChaves;
	// hk1113 TList   *lisRede = redes->LisRede();
	TList *lisRede = lisRedeOrdenada;
	VTChave *pchave;
	TListItem *item;
	AnsiString aux;
	VTTipoChave *tipoChave;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTipoChaves;
	int nconsProt;
	double cargaProt, distProt;

	// Reinicia as listboxes
	lbChaves->Clear();
	lbChavesAuto->Clear();

	lisTipoChaves = tipos->LisTipoChave();

	lisChaves = new TList();
   calcIndCont->BuscaProt->LisChaveOrd(lisChaves);//hk201808: reformula sai: calcIndCont->LisChavesOrd(lisChaves);
	// Adiciona chaves de vis carregadas  buscaProt->LisVisRede
	for (int nv = 0; nv < calcIndCont->LisVisRede->Count; nv++)
	{
		pchave = ((VTVisRede*)calcIndCont->LisVisRede->Items[nv])->pchave;
		if (lisChaves->IndexOf(pchave) != -1)
			continue;
		lisChaves->Add(pchave);
	}
	// Ordena lista de chaves pelo códido
	OrdenaLisChaves(lisChaves);
	// CHAVES NF, FECHADAS
	for (int n = 0; n < lisTipoChaves->Count; n++)
	{
		tipoChave = (VTTipoChave*)lisTipoChaves->Items[n];
		for (int nr = 0; nr < lisRede->Count; nr++)
		{
			rede = (VTRede*)lisRede->Items[nr];
			for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
			{
				pchave = (VTChave*)lisChaves->Items[nc];
				if (pchave->Aberta)
					continue; // if(pchave->Estado == chvABERTA) continue;  //hkRevAloca
				if (pchave->TipoChave != tipoChave)
					continue;
				if (pchave->rede != rede)
					continue;
				lisChaves->Delete(nc);
				aux.sprintf("(%s)", tipoChave->Codigo.c_str());
				aux.cat_sprintf("(%s)", rede->Codigo.c_str());
				aux.cat_sprintf("(%s)", (pchave->Aberta) ? "NA: " : "NF: ");
				// aux.cat_sprintf("(%s)", (pchave->Estado == chvABERTA) ? "NA: " : "NF: "); //hkRevAloca

				if (calcIndCont->LisChavesAuto->IndexOf(pchave) == -1)
					lbChaves->Items->AddObject(aux + pchave->Codigo, pchave);
				else
				{
					aux.cat_sprintf("%s", pchave->Codigo.c_str());
					lbChavesAuto->Items->AddObject(aux, pchave);
				}
			}
		}
	}
	// CHAVES NA, NORMALMENTE ABERTAS
	for (int n = 0; n < lisTipoChaves->Count; n++)
	{
		tipoChave = (VTTipoChave*)lisTipoChaves->Items[n];
		for (int nr = 0; nr < lisRede->Count; nr++)
		{
			rede = (VTRede*)lisRede->Items[nr];

			for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
			{
				pchave = (VTChave*)lisChaves->Items[nc];
				//hk201808: reformula: sai: if (calcIndCont->IdentificaVisRede(pchave) == NULL) continue;
            if (calcIndCont->BuscaProt->IdentificaVisRede(pchave) == NULL) continue;
				if (pchave->Fechada)
					continue; // if(pchave->Estado == chvFECHADA) continue;   //hkRevAloca
				if (pchave->TipoChave != tipoChave)
					continue;
				if (pchave->rede != rede)
					continue;
				lisChaves->Delete(nc);
				aux.sprintf("(%s)", tipoChave->Codigo.c_str());
				aux.cat_sprintf("(%s)", rede->Codigo.c_str());
				aux.cat_sprintf("(%s)", (pchave->Aberta) ? "NA: " : "NF: ");
				// aux.cat_sprintf("(%s)", (pchave->Estado == chvABERTA) ? "NA: " : "NF: ");//hkRevAloca

				if (calcIndCont->LisChavesAuto->IndexOf(pchave) == -1)
					lbChaves->Items->AddObject(aux + pchave->Codigo, pchave);
				else
				{
					aux.cat_sprintf("%s", pchave->Codigo.c_str());
					lbChavesAuto->Items->AddObject(aux, pchave);
				}
			}
		}
	}
	delete lisChaves;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::OrdenaLisChaves(TList *lisChaves)
{
	// Funcao que preenche lista lisRedeOrdenada com redes ordenadas alfabeticamente
	VTChave *chave, *chaveMax;
	TList *lisChavesAux;
	int Compare;

	lisChavesAux = new TList();
	lisChavesAux->Assign(lisChaves, laOr);
	lisChaves->Clear();

	while (lisChavesAux->Count > 0)
	{
		chaveMax = (VTChave*)lisChavesAux->Items[0];
		for (int n = 1; n < lisChavesAux->Count; n++)
		{
			chave = (VTChave*)lisChavesAux->Items[n];
			if ((Compare = CompareText(chave->Codigo, chaveMax->Codigo)) < 0)
			{
				chaveMax = chave;
			}
		}
		lisChaves->Add(chaveMax);
		lisChavesAux->Remove(chaveMax);
	}

	delete lisChavesAux;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCalcIndCont::GravaArqParam(void)  //hk201808: reformula: sai
{
	/*bool resposta = true;

	// Grava em parâmetros no diretório da última leitura
	if (!calcIndCont->GravaArqParam(""))
		resposta = false;

	return (resposta);
   */
   return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::sgridParamBlocoClick(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTBloco *bloco;
	int index;

	if ((index = sgridParamBloco->Row) < 1)
		return;
	if (sgridParamBloco->Objects[0][index] == NULL)
		return;

	bloco = (VTBloco*)sgridParamBloco->Objects[0][index];
	if (bloco->Rede == NULL)
		return;

	TColor color = bloco->Rede->Color;
	grafico->DestacaEqpto(bloco->LisLigacao(), color, 10);

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::SgridParamRedeBlocoInicia(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede, *lisCargas, *lisTMP;
	strDadoParamRede* paramRede;
	TStringGrid *SGrid = sgridParamRede;
	AnsiString tit[] =
	{"Rede", "Taxa(f/km/ano)", "Reparo(min)"};
	int width[] =
	{76, 90, 90, 90, 90};
	int col = sizeof(tit) / sizeof(tit[0]);

	// Limpa grid
	for (int n = 0; n < SGrid->RowCount; n++)
		SGrid->Rows[n]->Clear();

	// define redes com cargas
	lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	lisCargas = new TList();
	lisTMP = new TList();
	for (int n = 0; n < lisRede->Count; n++)
	{
		rede = (VTRede*)lisRede->Items[n];
		if (!rede->Carregada)
			continue; // hk0515
		lisCargas->Clear();
		rede->LisEqbar(lisCargas, eqptoCARGA);
		// hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
		// if(lisCargas->Count == 0) continue;
		lisTMP->Add(rede);
	}

	// Define número de linhas e colunas
	SGrid->ColCount = col;
	if (lisTMP->Count == 0)
		SGrid->RowCount = 2;
	else
		SGrid->RowCount = lisTMP->Count + 1;
	if (SGrid->RowCount > 1)
		SGrid->FixedRows = 1;
	if (SGrid->ColCount > 1)
		SGrid->FixedCols = 1;

	// Preenche título
	for (int n = 0; n < col; n++)
	{
		SGrid->Cells[n][0] = tit[n];
		SGrid->ColWidths[n] = width[n];
	}

	// loop p/ todos VTNivelPR
	for (int n = 0; n < lisTMP->Count; n++)
	{
		rede = (VTRede*)lisTMP->Items[n];
		// preenche parâmetros
		SGrid->Cells[0][n + 1] = rede->Codigo;
		SGrid->Objects[0][n + 1] = rede;
		// paramRede = calcIndCont->ParametroConf(rede);
		paramRede = calcIndCont->ParamConf->ParamRede[rede];
		if (paramRede != NULL)
		{
			SGrid->Cells[1][n + 1] = DoubleToStr("%.2f", paramRede->lambda);
			if (paramRede->trep < 0)
			{
				SGrid->Cells[2][n + 1] = DoubleToStr("%.2f", 0.0);
			}
			else
				SGrid->Cells[2][n + 1] = DoubleToStr("%.2f", paramRede->trep);

		}
	}

	// define redes com cargas
	lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	lisTMP->Clear();
	for (int n = 0; n < lisRede->Count; n++)
	{
		rede = (VTRede*)lisRede->Items[n];
		if (!rede->Carregada)
			continue; // hk0515
		// hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
		// if(lisCargas->Count == 0) continue;
		lisTMP->Add(rede);
	}

	SGrid = sgridParamBloco;
	AnsiString titBloco[] =
	{"Bloco", "Chave", "Rede", "Taxa(f/km/ano)", "Reparo(min)"};
	col = sizeof(titBloco) / sizeof(titBloco[0]);
	// Limpa grid
	for (int n = 0; n < SGrid->RowCount; n++)
		SGrid->Rows[n]->Clear();
	// Define número de linhas e colunas
	SGrid->ColCount = col;
	if (calcIndCont->ParamConf->LisParamBloco->Count == 0)//hk201808: reformula: if (calcIndCont->LisParamBloco->Count == 0)
		SGrid->RowCount = 2;
	// else SGrid->RowCount = calcIndCont->LisParamBloco->Count + 1;
	SGrid->RowCount = 2;
	// if(SGrid->RowCount > 1) SGrid->FixedRows = 1;
	// if(SGrid->ColCount > 2) SGrid->FixedCols = 3;

	// hk201703: edição de parametros por blocos, strParamBloco: larguras especificas para sgridParamBloco, int      width[] = {76, 90, 90, 90, 90};
	width[0] = 40;
	width[1] = 70;
	width[2] = 70;
	// Preenche título
	for (int n = 0; n < col; n++)
	{
		SGrid->Cells[n][0] = titBloco[n];
		SGrid->ColWidths[n] = width[n];
	}
	// loop p/ todos VTNivelPR
	// Atualiza grid de parametros por bloco
	strDadoParamBloco *paramBloco;
	AnsiString codRede;
	VTChave *chave;
	int iconta;
	TList *lisOrdBlocos = new TList();
	VTBloco *bloco;

	iconta = 0;

	for (int nr = 0; nr < lisTMP->Count; nr++)
	{
		rede = (VTRede*)lisTMP->Items[nr];
		lisOrdBlocos->Clear();
      calcIndCont->BuscaProt->ListaOrdenadaBlocos(lisOrdBlocos, rede); ////hk201808: sai: funcao de buscaProt: calcIndCont->ListaOrdenadaBlocos(lisOrdBlocos, rede);
		// hk201702: ordenação comum de blocos: for(int np = 0; np < calcIndCont->LisParamBloco->Count; np++)
		for (int nb = 0; nb < lisOrdBlocos->Count; nb++)
		{
			// hk201702: ordenação comum de blocos: paramBloco = (strDadoParamBloco*)calcIndCont->LisParamBloco->Items[np];
			// hk201702: ordenação comum de blocos: bloco = paramBloco->bloco;//bloco = (VTBloco*)lisOrdBlocos->Items[np];
			bloco = (VTBloco*)lisOrdBlocos->Items[nb];

			if (bloco->Rede != rede)
				continue;
			paramBloco = calcIndCont->ParamConf->ParamBloco[bloco];
			if (paramBloco == NULL)
			{
				/* Edição de paramBloco novo
				 iconta++;
				 SGrid->RowCount = iconta + 1;
				 codRede = rede->Codigo;
				 SGrid->Objects[0][iconta] =  bloco;
				 chave = calcIndCont->ChaveBloco(bloco);
				 SGrid->Cells[0][iconta] = IntToStr(nb+1);
				 if(chave != NULL)
				 SGrid->Cells[1][iconta] = chave->Codigo;
				 else
				 SGrid->Cells[1][iconta] = "";
				 SGrid->Cells[2][iconta] = codRede;
				 SGrid->Cells[3][iconta] = DoubleToStr("%.2f", 0.0);
				 SGrid->Cells[4][iconta] = DoubleToStr("%.2f", 0.0);
				 */
			}
			else
			{
				iconta++;
				SGrid->RowCount = iconta + 1;
				codRede = "";
				if (paramBloco->bloco->Rede != NULL)
					codRede = bloco->Rede->Codigo;
				SGrid->Objects[0][iconta] = bloco;
				SGrid->Cells[0][iconta] = IntToStr(nb + 1);
				SGrid->Cells[1][iconta] = paramBloco->codChave;
				SGrid->Cells[2][iconta] = codRede;
				SGrid->Cells[3][iconta] = DoubleToStr("%.2f", paramBloco->lambda);
				SGrid->Cells[4][iconta] = DoubleToStr("%.2f", paramBloco->trep);
			}
		}
	}
	if (SGrid->RowCount > 1)
		SGrid->FixedRows = 1;
	// delete lisOrdBlocos;

	delete lisCargas;
	delete lisTMP;
	/*
	 for(int np = 0; np < calcIndCont->LisParamBloco->Count; np++)
	 {
	 paramBloco = (strDadoParamBloco*)calcIndCont->LisParamBloco->Items[np];
	 if(paramBloco->bloco == NULL) continue; //hlk201608 -bug chave, bloco = NULL
	 if(paramBloco->bloco->Rede != rede) continue;
	 iconta++;
	 SGrid->RowCount = iconta;
	 codRede = "";
	 if(paramBloco->bloco->Rede != NULL) codRede = paramBloco->bloco->Rede->Codigo;
	 SGrid->Objects[0][iconta] =  paramBloco;
	 SGrid->Cells[0][iconta] = paramBloco->codChave;
	 SGrid->Cells[1][iconta] = codRede;
	 SGrid->Cells[2][iconta] = DoubleToStr("%.2f", paramBloco->lambda);
	 SGrid->Cells[3][iconta] = DoubleToStr("%.2f", paramBloco->trep);
	 }
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ChkLstTipoProtInicia(void)
{
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTTipoChave *tipoChave;
	TList *lisTipoChave;
	TList *lisTipoEqProt = calcIndCont->LisTipoEqProt;

	chkLstTipoProt->Clear();
	lisTipoChave = tipos->LisTipoChave();

	for (int nc = 0; nc < lisTipoChave->Count; nc++)
	{
		tipoChave = (VTTipoChave*)lisTipoChave->Items[nc];
		chkLstTipoProt->Items->AddObject(tipoChave->Codigo, tipoChave);
		if (lisTipoEqProt->IndexOf(tipoChave) == -1)
			chkLstTipoProt->Checked[nc] = false;
		else
			chkLstTipoProt->Checked[nc] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::SgridFalhasPatInicia(void)
{
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	int numPat = patamares->NumPatamar();

	sgridFalhasPat->RowCount = numPat + 1;
	sgridFalhasPat->ColCount = 2;

	for (int nr = 0; nr < sgridFalhasPat->RowCount; nr++)
		for (int nc = 0; nc < sgridFalhasPat->ColCount; nc++)
			sgridFalhasPat->Cells[nc][nr] = "";

	sgridFalhasPat->Cells[0][0] = "Patamar";
	sgridFalhasPat->ColWidths[0] = 90;
	sgridFalhasPat->Cells[1][0] = "%Falhas";
	sgridFalhasPat->ColWidths[1] = 55;

	for (int np = 0; np < numPat; np++)
	{
		patamar = patamares->Patamar[np];
		sgridFalhasPat->Cells[0][np + 1] = patamar->Nome;
		sgridFalhasPat->Cells[1][np + 1] = DoubleToStr("%.2f", 100.0 * calcIndCont->ParamConf->DistFalhasPat[np]);
			//hk201808: reformula: DoubleToStr("%.2f", 100.0 * calcIndCont->DistFalhasPat[np]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionArqParamExecute(TObject *Sender) //hk201808: reformula: sai
{  /*
	// variáveis locais
	// VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	AnsiString arqParam, dirArq;

	// configura OpenDialog
	OpenDialog->Filter = "Arquivos TXT (*.txt)|*.txt";
	OpenDialog->FileName = "";
	// assume diretório default de importação
	OpenDialog->InitialDir = calcIndCont->PathParam;
	// exibe OpenDialog
	if (OpenDialog->Execute())
	{ // identifica caminho do arquivo
		arqParam = OpenDialog->FileName;
		dirArq = ExtractFileDir(arqParam) + "\\";
		// executa leitura
		try
		{ // leitura dos parâmetros
			calcIndCont->LeArquivos(dirArq);
			lblPathParam->Caption = calcIndCont->PathParam;
			// atualiza tela
			AtualizaInterfaceDadosGerais();
			LvParamRedeInicia();
			LvParamBlocoInicia();
			SgridParamRedeBlocoInicia();
			// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia
		}
		catch (Exception &e)
		{
			return;
		}
	}
   */
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionEditaParamJusProtExecute(TObject *Sender)
{
	TFormEditaParamProtJus *formEditaParamProtJus;
	/////////////////////////////////////////////////////////////
	// Mostra form de edicao de parametros por chave de protecao//
	/////////////////////////////////////////////////////////////
	formEditaParamProtJus = new TFormEditaParamProtJus(NULL, apl, this);
	formEditaParamProtJus->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionMostraParamAuxExecute(TObject *Sender)
{
	// TFormParamAux *formParamAux;

	// formParamAux = new TFormParamAux(NULL, apl, this);
	// formParamAux->Show();

	// TFormAlternativasChaves *formAlt;
	// formAlt = new TFormAlternativasChaves(NULL, apl, this);
	// formAlt->Show();

	TFormEliminaFusivel *formEliminaFusivel;
	formEliminaFusivel = new TFormEliminaFusivel(NULL, apl, this);
	formEliminaFusivel->ShowModal();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionViewDECExecute(TObject *Sender)
{
	VTMostraGrafIndic *mostraGraf;

	mostraGraf = NewObjMostraGrafIndic(apl);
	mostraGraf->ResCont = calcIndCont->ResCont;

	mostraGraf->MostraContribBloco(0, "DEC (h)", 1);
	delete mostraGraf;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionViewCLIExecute(TObject *Sender)
{
	VTMostraGrafIndic *mostraGraf;

	mostraGraf = NewObjMostraGrafIndic(apl);
	mostraGraf->ResCont = calcIndCont->ResCont;

	mostraGraf->MostraContribBloco(3, "Contribuição do bloco Clih (deslig.h/ano)", 1);
	delete mostraGraf;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionGrafGanhosNAExecute(TObject *Sender)
{
	VTMostraGrafIndic *mostraGraf;
	mostraGraf = NewObjMostraGrafIndic(apl);
	mostraGraf->MostraContribTransf(calcIndCont->LisDefeitos);
	delete mostraGraf;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionMostraLbChavesExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisAux;
	TListBox *lb;

	lb = (TListBox*)Sender;

	grafico->Moldura();
	lisAux = new TList();
	for (int n = 0; n < lb->Items->Count; n++)
	{
		if (!lb->Selected[n])
			continue;
		lisAux->Add(lb->Items->Objects[n]);
	}

	grafico->Moldura(lisAux);
	delete lisAux;

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionMostraVIPExecute(TObject *Sender)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisCargas = new TList();
	VTCarga *carga;

	redes->LisEqbar(lisCargas, eqptoCARGA);

	for (int n = lisCargas->Count - 1; n >= 0; n--)
	{
		carga = (VTCarga*)lisCargas->Items[n];
		if (carga->VeryImportant)
			continue;
		lisCargas->Delete(n);
	}
	grafico->Moldura(lisCargas);
	delete lisCargas;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionGravaParamMdbExecute(TObject *Sender)
{
	// VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	// AnsiString arq;

	// arq = path->DirDat() + "\\EDP\\parConfTeste.mdb";
	// calcIndCont->GravaArqMdb(arq);

	// variáveis locais
	AnsiString arq = "";

	// reinicia
	OpenDialog->FileName = arq;
	// mostra a janela de salvar acima das outras
	Application->ModalPopupMode = pmAuto;
	// proteção:
	if (calcIndCont == NULL)
		return;
	// configura OpenFileDialog
	OpenDialog->Title = "Selecionar base de dados";
	OpenDialog->Filter = "Arquivos MDB (*.mdb)|*.mdb";
	// exibe OpenTextFileDialog
	if (OpenDialog->Execute())
	{ // valida seleção
		if (OpenDialog->Files->Count == 0)
			return;
		// identifica caminho do arquivo
		arq = OpenDialog->FileName;
		// executa gravação
		try
		{
         //hk201808: reformula
			//calcIndCont->GravaArqMdb(arq);
         calcIndCont->ParamConf->GravaArqMdb(apl, arq);

			lblPathParam->Caption = calcIndCont->ParamConf->PathParam;//hk201808: reformula: calcIndCont->PathParam;
		}
		catch (Exception &e)
		{ // nada a fazer
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionLeTxtIndObservExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString arq = "";

	// reinicia
	OpenDialog->FileName = arq;
	// mostra a janela de salvar acima das outras
	Application->ModalPopupMode = pmAuto;
	// configura OpenFileDialog
	OpenDialog->Title = "Selecionar arquivo com DEC/FEC verificados";
	OpenDialog->Filter = "Arquivos (valorVerif.dat)|*.dat";
	// exibe OpenTextFileDialog
	if (OpenDialog->Execute())
	{ // valida seleção
		if (OpenDialog->Files->Count == 0)
			return;
		// identifica caminho do arquivo
		arq = OpenDialog->FileName;
		// executa leitura
		try
		{
			// calcIndCont->ReiniciaValVerifTxt(arq);
			// Inicia grid de ajuste de valores
			SgridAjusteInicia();
			// Atualiza paramaetros editaveis por circuito
			SgridParamRedeBlocoInicia();
			// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia
		}
		catch (Exception &e)
		{ // nada a fazer
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionLeArqMdbExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString arq = "";

	// reinicia
	OpenDialog->FileName = arq;
	// mostra a janela de salvar acima das outras
	Application->ModalPopupMode = pmAuto;
	// proteção:
	if (calcIndCont == NULL)
		return;
	// configura OpenFileDialog
	OpenDialog->Title = "Selecionar base de dados";
	OpenDialog->Filter = "Arquivos MDB (paramConf.mdb)|paramConf.mdb";
	// exibe OpenTextFileDialog
	if (OpenDialog->Execute())
	{ // valida seleção
		if (OpenDialog->Files->Count == 0)
			return;
		// identifica caminho do arquivo
		arq = OpenDialog->FileName;
		// executa leitura
		try
		{
			LeArquivoParametros(arq); // hk201702: alterações gerais
		}
		catch (Exception &e)
		{ // nada a fazer
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LeArquivoParametros(AnsiString arq) // hk201702: alterações gerais
{
	StatusBarCount->SimpleText = "Lendo arquivo de parâmetros.."; // hk201612
	calcIndCont->LeArquivosMdb(arq);//hk201808: reformulação: LeArqMdb(arq);
	StatusBarCount->SimpleText = "Leitura de parâmetros finalizada."; // hk201612
	lblPathParam->Caption = calcIndCont->ParamConf->PathParam;//hk201808: reformula: calcIndCont->PathParam
	// Verifica se há parametros históricos
	if (calcIndCont->ParamConf->LisParamEqpto->Count > 0)
		butReiniciaParHist->Enabled = true;
	else
		butReiniciaParHist->Enabled = false;
	// TVChavesInicia(calcIndCont->ParamConf->ParamGerais->flagRecurso);//hk1408
	TVChavesInicia(); // hk1408
	// Atualiza interface de dados gerais
	AtualizaInterfaceDadosGerais();
	// Atualiza listView
	LvParamRedeInicia();
	LvParamBlocoInicia();
	// hkv201608 - funcao realizada em calcIndCont->LeArqMdb(arq)
	// se não houverem dados no banco carregada uma linha com parametro verificado
	// com todos os circuitos carregados
	// if(calcIndCont->LisVerifCjto->Count == 0)
	// {
	// calcIndCont->IniciaVerifDefault();
	// }

	// Inicia grid de ajuste de valores
	SgridAjusteInicia();
	// Atualiza paramaetros editaveis por circuito
	SgridParamRedeBlocoInicia();
	// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionAjusteParamExecute(TObject *Sender)
{
	int currCursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	StatusBarCount->SimpleText = "Ajuste de parâmetros em execução..";
	// Verifica se volta a parâmetros em função de histprico
	if (chkReiniciaParam->Checked)
		ActionVoltaParamOriginaisExecute(NULL);
	// ----------------------------------------------------------------
	// Reinicia taxas e tempos de reparo
	// lisParamEqpto->paramEqpto->lambda  , trep
	// paramEqpto->lamb[nm], tr[nm]
	// Funcao de:
	// paramEqpto->durTotal
	// paramEqpto->nFalhasEquip
	// paramEqpto->ndiasObs
	// GeraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);
	// e
	// Reinicia lisParamBloco->paramBloco->(lambda, trep)
	// Funcao de:
	// paramEqpto->lambda
	// paramEqpto->trep
	// GeraLisParamEqpto->RedistribuiFalhasCirc(paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);
	// (flagGeral,flagCircuito,flagEqpto,flagDistHora)
	// calcIndCont->RetornaParametrosHistorico(false, true, true, false);

	// ----------------------------------------------------------------

	// calcIndCont->ExecutaAjusteLisParamVerif();    //calcIndCont->ExecutaAjusteParametrosLisRede
	// calcIndCont->RetornaParametrosHistorico(false, false, true, false);
	// VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
	// AnsiString arq;
	// arq = path->DirTmp() + "\\parsimulaMDB.txt";
	// calcIndCont->ParamConf->ImprimeParametrosSimula(arq);

	// calcIndCont->Executa();
	// hktemp calcIndCont->ExecutaAjusteLisParamVerif();    //calcIndCont->ExecutaAjusteParametrosLisRede
	// ExecutaAjusteSE();
	ExecutaAjusteCjto();
	calcIndCont->ExecutaCompensacoes();
	// TVChavesInicia();//hk201612: calculo de fluxo de potencia

	// double daux = calcIndCont->ResGeral->tdeslN /60.;
	// arq = path->DirTmp() + "\\defeitosMDB.txt";
	// calcIndCont->ImprimeParametrosDefeito(arq);

	// calcIndCont->ExecutaAjuste();
	// Reinicia grid de ajuste de parâmetros
	SgridAjusteInicia();
	// Reinicia grid com parâmetros
	SgridParamRedeBlocoInicia();
	// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia
	// Lista os resultados calculados
	LVResultInicia();
	LvCompInicia();
	// Reinicia lista de valores por bloco e circuito
	// Atualiza listView
	LvParamRedeInicia();
	LvParamBlocoInicia();

	Screen->Cursor = (TCursor)currCursor;
	StatusBarCount->SimpleText = "Parâmetros ajustados.";
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::SgridAjusteInicia(void)
{
	// VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// VTRede      *rede;
	// strDadoParamRede *paramRede;
	// TList       *lisRede;
	TList *lisVerifCjto;
	// hk201706
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRadial *radial;
	VTSubestacao *se;
	TList *lisSE;
	TList *lisAux;
	VTVerifSe *verifSe;
	radial = DLL_NewObjRadial(apl);
	radial->Inicia(redes);
	lisSE = radial->LisSubestacao();
	sgridAjuste->EditorMode = true;

	sgridAjuste->RowCount = 1;
	sgridAjuste->ColCount = 5;
	sgridAjuste->Cells[0][0] = "Rede";
	sgridAjuste->Cells[1][0] = "DEC_VERIF";
	sgridAjuste->Cells[2][0] = "FEC_VERIF";
	sgridAjuste->Cells[3][0] = "DEC_AJUST";
	sgridAjuste->Cells[4][0] = "FEC_AJUST";

	lisVerifCjto = calcIndCont->ParamConf->LisVerifCjto;  //hk201808: reformula: calcIndCont->LisVerifCjto;
	VTVerifCjto *verifCjto;

	lisAux = new TList();
	lisAux->Assign(lisVerifCjto, laOr);
	for (int nv = lisAux->Count - 1; nv >= 0; nv--)
	{
		verifCjto = (VTVerifCjto*)lisAux->Items[nv];
		// Verifica se as SEs do conjunto estão carregadas
		if (verifCjto->LisVerifSe->Count == 0)
		{
			lisAux->Delete(nv);
			continue;
		}

		for (int nse = 0; nse < verifCjto->LisVerifSe->Count; nse++)
		{
			verifSe = (VTVerifSe*)verifCjto->LisVerifSe->Items[nse];
			// Verifica se já houve ajuste
			// if((decVerif > 0.0)&&(fecVerif > 0.0)) continue;
			se = NULL;
			for (int nse = 0; nse < lisSE->Count; nse++)
			{
				se = (VTSubestacao*)lisSE->Items[nse];
				if (se->Rede->Codigo.AnsiCompare(verifSe->Codigo) == 0)
					break;
				se = NULL;
			}
			if (se == NULL)
			{
				lisAux->Delete(nv);
				break;
			}
		}
	}

	// lisRede = lisRedeOrdenada;//hk1113redes->LisRede();
	for (int nv = 0; nv < lisAux->Count; nv++)
	{
		verifCjto = (VTVerifCjto*)lisAux->Items[nv];
		sgridAjuste->Cells[0][sgridAjuste->RowCount] = verifCjto->Codigo;
		// rede = NULL;
		// for(int n = 0; n < lisRede->Count; n++)
		// {
		// rede = (VTRede*)lisRede->Items[n];
		// if(verifCjto->Codigo.AnsiCompare(rede->Codigo) == 0) break;
		// rede = NULL;
		// }
		sgridAjuste->Objects[0][sgridAjuste->RowCount] = verifCjto;
		sgridAjuste->Cells[1][sgridAjuste->RowCount] = DoubleToStr("%6.2f", verifCjto->DecVerif);
		sgridAjuste->Cells[2][sgridAjuste->RowCount] = DoubleToStr("%6.2f", verifCjto->FecVerif);
		sgridAjuste->Cells[3][sgridAjuste->RowCount] = "";
		sgridAjuste->Cells[4][sgridAjuste->RowCount] = "";
		if ((verifCjto->DecAjust > 0.0) && (verifCjto->FecAjust > 0.0))
		{
			sgridAjuste->Cells[3][sgridAjuste->RowCount] =
				DoubleToStr("%6.2f", verifCjto->DecAjust);
			sgridAjuste->Cells[4][sgridAjuste->RowCount] =
				DoubleToStr("%6.2f", verifCjto->FecAjust);
		}
		sgridAjuste->RowCount++;
	}
	delete lisAux;
	delete radial;
	if (sgridAjuste->RowCount > 1)
		sgridAjuste->FixedRows = 1;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ExecutaAjusteCjto(void)
{
	// Executa ajustes por conjunto elétrico
	VTVerifCjto *verifCjto;
	// Atualiza objetos calcIndCont->LisVerifCjto->verifCjto
	if (sgridAjuste->RowCount == 1)
		return;
	for (int n = 1; n < sgridAjuste->RowCount; n++)
	{
		if (sgridAjuste->Objects[0][n] == NULL)
			continue;

		verifCjto = (VTVerifCjto*)sgridAjuste->Objects[0][n];
		verifCjto->DecVerif = StrToDouble(sgridAjuste->Cells[1][n]);
		verifCjto->FecVerif = StrToDouble(sgridAjuste->Cells[2][n]);
		verifCjto->DecAjust = -1.0;
		verifCjto->FecAjust = -1.0;
	}
	calcIndCont->ExecutaAjusteLisParamVerif();
	// Caso existam subestacoes sem valores verificados realiza o cálculo geral
	calcIndCont->Executa();
	// TVChavesInicia();//hk201612: calculo de fluxo de potencia
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionRejeitaParamGeraisExecute(TObject *Sender)
{
	AtualizaInterfaceDadosGerais();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionConfirmaParamGeraisExecute(TObject *Sender)
{
	try
	{ // Leitura dos parâmetros
		ConfirmaParametros();
	}
	catch (Exception &e)
	{
		return;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionCalcIndDerivExecute(TObject *Sender)
{
	int currCursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	StatusBarCount->SimpleText = "Executando o cálculo de indicadores..";
	// Executa cálculo de indicadores de continuidade e compensações
	// para todas as redes
	calcIndCont->Executa();
	calcIndCont->ExecutaCompensacoes();
   //ImprimeResCalcIndCont();    //hktemp
	// parada: mostrar os strIF

	// TVChavesInicia();//hk201612: calculo de fluxo de potencia

	// calcIndCont->ExecutaCompensacoesMensal();
	// Reinicia listView de compensações
	LvCompInicia();
	LVResultInicia();

	// Habilita botão para verificar tratamento de defeito
	butEdt->Enabled = true;
	// Retorna a página com resultados
	PageControl2->ActivePageIndex = 2;
	PageControl1->ActivePageIndex = 0;

	Screen->Cursor = (TCursor)currCursor;
	StatusBarCount->SimpleText = "Indicadores calculados.";
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LvCompInicia(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	strResRede *resRede;
	TListItem *item;
	TList *lisRede;
	double totalComp;

	lvComp->Clear();
	totalComp = 0.0;
	lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];

		if (!rede->Carregada)
			continue;

		resRede = calcIndCont->ResCont->ResRede[rede];//hk201808: reformula: resRede = calcIndCont->ResRede[rede];
		if (resRede->ncons == 0)
			continue;
		item = lvComp->Items->Add();
		item->Caption = rede->Codigo;
		item->SubItems->Add(DoubleToStr("%6.2f", resRede->compensacao));
		item->SubItems->Add(DoubleToStr("%6.2f", resRede->dicVIP));
		item->SubItems->Add(DoubleToStr("%6.2f", resRede->ficVIP));
		totalComp += resRede->compensacao;
	}
	// Adiciona linha com total de Compensacao
	item = lvComp->Items->Add();
	item->Caption = "Total";
	item->SubItems->Add(DoubleToStr("%6.2f", totalComp));
	item->SubItems->Add("-");
	item->SubItems->Add("-");
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionDefListasExecute(TObject *Sender)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTBloco *blocoDef;
	TList *lisBloco, *lisRede, *lisBlocosTot;
	VTRede *rede;
	VTDefeito *pdef;
	strResBloco *resBloco;
	TCargaBloco *cBloco;
	VTBlocos *blocos = calcIndCont->Blocos;

	// VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
	// AnsiString arq;
	// arq = path->DirTmp() + "\\ParamSimula_Executa.txt";
	// calcIndCont->ParamConf->ImprimeParametrosSimula(arq);

	// int iMes;
	// iMes       = StrToInt(edtMes->Text);
	// if(iMes < 0)
	// {
	// calcIndCont->Executa();
	// }
	// else
	// {
	// calcIndCont->ExecutaCalculoMensal(iMes);
	// }

	// Executa: definição de estratégias e cálculo de indicadores
	calcIndCont->Executa();
	// TVChavesInicia();//hk201612: calculo de fluxo de potencia

	// Lista os resultados calculados
	LVResultInicia();

	// Habilita botão para verificar tratamento de defeito
	butEdt->Enabled = true;
	// Retorna a página com resultados
	PageControl2->ActivePageIndex = 2;
	PageControl1->ActivePageIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExecutaMesExecute(TObject *Sender)
{
	// Gera por mês pu mensal dos indicadores
	calcIndCont->ExecutaDicFicPuMensal();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lbChavesAutoClick(TObject *Sender)
{
	ActionMostraLbChavesExecute(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lbChavesClick(TObject *Sender)
{
	ActionMostraLbChavesExecute(Sender);
}

////---------------------------------------------------------------------------
// void __fastcall TFormCalcIndCont::LVResultInicia(void)
// {
// VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
// VTBloco *bloco;
// strResBloco *resBloco;
// strResRede  *resRede, *resGeral;
// strDadoParamBloco *paramBloco;
// TList      *lisBloco, *lisRede, *lisAuxDefeito;
// VTRede     *rede;
// TListItem  *item;
// int nconsTot, nconsRede;
// double dec, fec, end, decRede, fecRede, endRede, ndesl, tdesl, daux;
// VTDefeito  *pdef;
// //hk2007 VTBlocos	   *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
// VTBlocos *blocos = calcIndCont->Blocos;
//
//
// //Reinicia lisView
// lvResult->Items->Clear();
// lvResultRede->Items->Clear();
// lvResultDefeito->Items->Clear();
// lvIndicIndiv->Items->Clear();
//
//
// lisBloco = new TList();
//
//
// end = 0.0; dec = 0.0; fec = 0.0;
// nconsTot = 0;
// lisRede = lisRedeOrdenada;//hk1113 redes->LisRede();
// for(int nr = 0; nr < lisRede->Count; nr++)
// {
// rede = (VTRede*)lisRede->Items[nr];
// //Verifica se a rede está carregada
// if(!rede->Carregada) continue;
// //Busca resultados
// resRede = calcIndCont->ResRede[rede];
// //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
// //if(resRede->ncons == 0) continue;
//
// decRede = 0.0; fecRede = 0.0; endRede = 0.0;
// nconsRede = 0;
// //Preenche lista com blocos ordenados da montante a jusante
// lisBloco->Clear();
// calcIndCont->ListaOrdenadaBlocos(lisBloco, rede);
//
//
// for(int nb = 0; nb < lisBloco->Count; nb++)
// {
// bloco = (VTBloco*)lisBloco->Items[nb];
// resBloco = calcIndCont->ResBloco[bloco];
//
// item = lvResult->Items->Add();
// item->Caption = rede->Codigo;
// item->Data    = resBloco;
// item->SubItems->Add(IntToStr(nb+1));
//
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->tdesl * resBloco->ncons /60.0));//hk0630
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->ndesl * resBloco->ncons));//hk0630
// item->SubItems->Add(IntToStr(resBloco->ncons));
// if(resBloco->ncons != 0)
// {
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->tdesl / 60.)); //em horas
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->ndesl));
// }
// else
// {
// item->SubItems->Add(DoubleToStr("%6.2f", 0.0)); //em horas
// item->SubItems->Add(DoubleToStr("%6.2f", 0.0));
// }
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->end));
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->nconsVIP));
// item->SubItems->Add(DoubleToStr("%6.2f", resBloco->npiscadas));
//
//
// //calculo global e por rede
// nconsTot += resBloco->ncons;
// dec      += (resBloco->tdesl * resBloco->ncons / 60.);//horas
// fec      += (resBloco->ndesl * resBloco->ncons);
// end      += (resBloco->end);
// nconsRede+= resBloco->ncons;
// decRede  += (resBloco->tdesl * resBloco->ncons / 60.);//horas
// fecRede  += (resBloco->ndesl * resBloco->ncons);
// endRede  += (resBloco->end);
// }
// //if(nconsRede == 0) continue;
//
// item = lvResultRede->Items->Add();
// item->Caption = rede->Codigo;
// item->Data    = rede;
// item->SubItems->Add("-");
// //Indicadores falhas MT e BT
// decRede   = resRede->tdeslN/60.0;
// fecRede   = resRede->ndeslN;
// nconsRede = resRede->ncons;
// endRede   = resRede->end;
// item->SubItems->Add(DoubleToStr("%6.2f", decRede));
// item->SubItems->Add(DoubleToStr("%6.2f", fecRede));
// if(nconsRede > 0)
// {
// decRede /= nconsRede;
// fecRede /= nconsRede;
// }
// else {decRede = fecRede = 0.0;}
// item->SubItems->Add(nconsRede);
// item->SubItems->Add(DoubleToStr("%6.2f", decRede));
// item->SubItems->Add(DoubleToStr("%6.2f", fecRede));
// item->SubItems->Add(DoubleToStr("%6.2f", endRede));
// //Indicadores falhas BT
// decRede   = resRede->tdeslBTN/60.0;
// fecRede   = resRede->ndeslBTN;
// endRede   = resRede->endBT;
// if(nconsRede > 0)
// {
// decRede /= nconsRede;
// fecRede /= nconsRede;
// }
// else 	{decRede = fecRede = 0.0;}
//
// item->SubItems->Add(DoubleToStr("%6.2f", decRede));
// item->SubItems->Add(DoubleToStr("%6.2f", fecRede));
// item->SubItems->Add(DoubleToStr("%6.2f", endRede));
//
// if(resRede->ncons > 0)
// {
// fecRede = resRede->ndeslPisca / resRede->ncons;
// }
// else 	{fecRede = 0.0;}
// item->SubItems->Add(DoubleToStr("%6.2f", fecRede));
//
// item->SubItems->Add(DoubleToStr("%6.0f", resRede->nconsVIP));
//
// if(resRede->nconsVIP > 0)
// {
// fecRede = resRede->ndeslPiscaVIP / resRede->nconsVIP;
// }
// else 	{fecRede = 0.0;}
// item->SubItems->Add(DoubleToStr("%6.2f", fecRede));
//
// }
//
// item = lvResultRede->Items->Add();
// //hk2015
// item->Caption = "Global";
// item->Data    = NULL;
// item->SubItems->Add("-");
//
// resGeral = calcIndCont->ResGeral;
// dec      = resGeral->tdeslN /60.;
// fec      = resGeral->ndeslN;
// nconsTot = resGeral->ncons;
// end      = resGeral->end;
// item->SubItems->Add(DoubleToStr("%6.2f", dec));
// item->SubItems->Add(DoubleToStr("%6.2f", fec));
// if(nconsTot > 0)
// {
// dec /= nconsTot;
// fec /= nconsTot;
// } else {dec = fec = 0.0;}
//
// item->SubItems->Add(nconsTot);
// item->SubItems->Add(DoubleToStr("%6.2f", dec));
// item->SubItems->Add(DoubleToStr("%6.2f", fec));
// item->SubItems->Add(DoubleToStr("%6.2f", end));
// //Resultados totais BT
// dec = resGeral->tdeslBTN /60.;
// fec = resGeral->ndeslBTN;
// end = resGeral->endBT;
//
// if(nconsTot > 0)
// {
// dec /= nconsTot;
// fec /= nconsTot;
// } else {dec = fec = 0.0;}
// item->SubItems->Add(DoubleToStr("%6.2f", dec));
// item->SubItems->Add(DoubleToStr("%6.2f", fec));
// item->SubItems->Add(DoubleToStr("%6.2f", end));
//
// if(resGeral->ncons > 0) fec = resGeral->ndeslPisca / resGeral->ncons;
// else fec = 0.0;
// item->SubItems->Add(DoubleToStr("%6.2f", fec));
//
// if(resGeral->nconsVIP > 0) fec = resGeral->ndeslPiscaVIP / resGeral->nconsVIP;
// else fec = 0.0;
// item->SubItems->Add(DoubleToStr("%6.2f", fec));
//
//
// //Preenche lvResultDefeito
// lisRede = lisRedeOrdenada;//hk1113 redes->LisRede();
// lisAuxDefeito = new TList();
// lisAuxDefeito->Assign(calcIndCont->LisDefeitos, laOr);
// for(int nr = 0; nr < lisRede->Count; nr++)
// {
// rede = (VTRede*)lisRede->Items[nr];
// if(!rede->Carregada) continue;  //hk201702: alteração geral
// lisBloco->Clear();
// calcIndCont->ListaOrdenadaBlocos(lisBloco, rede);
// for(int nb = 0; nb < lisBloco->Count; nb++)
// {
// bloco = (VTBloco*)lisBloco->Items[nb];
// for(int n = lisAuxDefeito->Count - 1; n >= 0; n--)
// {
// pdef = (VTDefeito*)lisAuxDefeito->Items[n];
// if(pdef->Rede != rede) continue;
// if(pdef->Bloco == bloco)
// {
// lisAuxDefeito->Remove(pdef);
// item = lvResultDefeito->Items->Add();
// item->Data    = pdef;
// item->Caption = rede->Codigo;
// if(pdef->ChvProt) item->SubItems->Add(pdef->ChvProt->Codigo);
// else item->SubItems->Add("");
// item->SubItems->Add(IntToStr(nb+1));
// item->SubItems->Add(DoubleToStr("%6.2f", pdef->ComprimentoBloco));
// item->SubItems->Add(DoubleToStr("%6.2f", 1000* pdef->NFalhas));
// item->SubItems->Add(DoubleToStr("%6.2f", pdef->DCons));// / nconsTot
// item->SubItems->Add(DoubleToStr("%6.2f", pdef->TCons/60.0));// / nconsTot
// item->SubItems->Add(DoubleToStr("%6.2f", pdef->NmanAuto)); //hkedp int->double
// item->SubItems->Add(DoubleToStr("%6.2f", pdef->NmanMan));  //hkedp int->double
//
// break;
// }
// }
// }
// }
//
// //hkindiv
// /*
// strResCarga *resCarga;
// double dicMT, ficMT;
// double dicBT, ficBT, compAux;
// AnsiString aux;
// int ncons;
//
// for(int nc= 0; nc < calcIndCont->ResCont->LisResCarga->Count; nc++)
// {
// resCarga = (strResCarga*)calcIndCont->ResCont->LisResCarga->Items[nc];
// item = lvIndicIndiv->Items->Add();
// item->Caption = resCarga->rede->Codigo;
// item->SubItems->Add(resCarga->carga->Codigo);
//
//
// //VIP
// aux = (resCarga->carga->VeryImportant) ? "S" : "N";
// item->SubItems->Add(aux);
// //MT
// aux = (resCarga->carga->NumConsA4 > 0) ? "S" : "N";
// item->SubItems->Add(aux);
// //RURAL
// aux = (resCarga->carga->NumConsRural > 0) ? "S" : "N";
// item->SubItems->Add(aux);
// item->SubItems->Add(DoubleToStr("%6.2f", resCarga->dic));
// item->SubItems->Add(DoubleToStr("%6.2f", resCarga->fic));
// item->SubItems->Add(DoubleToStr("%6.2f", resCarga->dicBT));
// item->SubItems->Add(DoubleToStr("%6.2f", resCarga->ficBT));
//
// ncons = resCarga->ncons;
//
// //ncons  = resCarga->carga->NumConsResidencial;
// //ncons += resCarga->carga->NumConsComercial;
// //ncons += resCarga->carga->NumConsIndustrial;
// //ncons += resCarga->carga->NumConsRural;
// //ncons += resCarga->carga->NumConsOutros;
// //ncons += resCarga->carga->NumConsA4;
// item->SubItems->Add(IntToStr(ncons));
//
// //compAux = 0.0;
// //if(ncons > 0) compAux = resCarga->compensacao / ncons;
// //item->SubItems->Add(DoubleToStr("%6.2f", compAux));
// item->SubItems->Add(DoubleToStr("%6.2f", resCarga->compensacao));
// for(int nm = 0; nm < 12; nm++)
// {item->SubItems->Add(DoubleToStr("%6.2f", resCarga->dic_pu[nm]));
// }
// for(int nm = 0; nm < 12; nm++)
// {item->SubItems->Add(DoubleToStr("%6.2f", resCarga->fic_pu[nm]));
// }
// }
// //hkindiv
// */
//
// delete lisAuxDefeito;
// delete lisBloco;
//
// }
// FKM
// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LVResultInicia(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTBloco *bloco;
	strResBloco *resBloco;
	strResRede *resRede, *resGeral;
	strDadoParamBloco *paramBloco;
	TList *lisBloco, *lisRede, *lisAuxDefeito;
	VTRede *rede;
	TListItem *item;
	int nconsTot, nconsRede;
	double dec, fec, end, decRede, fecRede, endRede, ndesl, tdesl, daux;
	VTDefeito *pdef;
	// hk2007 VTBlocos	   *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBlocos *blocos = calcIndCont->Blocos;
	double valores[lvcolCIC_Count]; // para guardar os valores do LVResult //FKM
	double global[lvcolCIC_Count]; // para guardar os valores do LVResult //FKM
	double nconsu_antigo; // FKM
	double nconsuVIP_antigo; // FKM
	AnsiString aux;
	// Reinicia lisView
	lvResult->Items->Clear();
	lvResultRede->Items->Clear();
	lvResultDefeito->Items->Clear();
	lvIndicIndiv->Items->Clear();

	lisBloco = new TList();

	end = 0.0;
	dec = 0.0;
	fec = 0.0;
	nconsTot = 0;
	lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	for (int n = 0; n < lvcolCIC_Count; n++)
	{
		global[n] = 0.0;
	}
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];
		// Verifica se a rede está carregada
		if (!rede->Carregada)
			continue;
		// Busca resultados
      resRede = calcIndCont->ResCont->ResRede[rede];//hk201808: reformula: resRede = calcIndCont->ResRede[rede];
		// hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
		// if(resRede->ncons == 0) continue;

		decRede = 0.0;
		fecRede = 0.0;
		endRede = 0.0;
		nconsRede = 0;
		// Preenche lista com blocos ordenados da montante a jusante
		lisBloco->Clear();
      calcIndCont->BuscaProt->ListaOrdenadaBlocos(lisBloco, rede); ////hk201808: sai: funcao de buscaProt: calcIndCont->ListaOrdenadaBlocos(lisBloco, rede);

		for (int nb = 0; nb < lisBloco->Count; nb++)
		{
			bloco = (VTBloco*)lisBloco->Items[nb];
			resBloco = calcIndCont->ResCont->ResBloco[bloco][false];//hk201808: reformula: calcIndCont->ResBloco[bloco];

			item = lvResult->Items->Add();
			item->Caption = rede->Codigo;
			item->Data = resBloco;
			item->SubItems->Add(IntToStr(nb + 1));

			item->SubItems->Add(DoubleToStr("%6.2f", resBloco->tdesl * resBloco->ncons / 60.0));
			// hk0630
			item->SubItems->Add(DoubleToStr("%6.2f", resBloco->ndesl * resBloco->ncons)); // hk0630
			item->SubItems->Add(IntToStr(resBloco->ncons));
			if (resBloco->ncons != 0)
			{
				item->SubItems->Add(DoubleToStr("%6.2f", resBloco->tdesl / 60.)); // em horas
				item->SubItems->Add(DoubleToStr("%6.2f", resBloco->ndesl));
			}
			else
			{
				item->SubItems->Add(DoubleToStr("%6.2f", 0.0)); // em horas
				item->SubItems->Add(DoubleToStr("%6.2f", 0.0));
			}
			item->SubItems->Add(DoubleToStr("%6.2f", resBloco->end));
			item->SubItems->Add(DoubleToStr("%6.2f", resBloco->nconsVIP));
			item->SubItems->Add(DoubleToStr("%6.2f", resBloco->npiscadas));

			// calculo global e por rede
			nconsTot += resBloco->ncons;
			dec += (resBloco->tdesl * resBloco->ncons / 60.); // horas
			fec += (resBloco->ndesl * resBloco->ncons);
			end += (resBloco->end);
			nconsRede += resBloco->ncons;
			decRede += (resBloco->tdesl * resBloco->ncons / 60.); // horas
			fecRede += (resBloco->ndesl * resBloco->ncons);
			endRede += (resBloco->end);
		}
		// zera todos valores
		for (int n = 0; n < lvcolCIC_Count; n++)
		{
			valores[n] = 0.0;
		}
		// agrega info dos blocos para formar a info rede
		for (int nb = 0; nb < lisBloco->Count; nb++)
		{
			bloco = (VTBloco*)lisBloco->Items[nb];

			resBloco = calcIndCont->ResCont->ResBloco[bloco][false];//hk201808: reformula: calcIndCont->ResBloco[bloco];
			// age caso se a mesma rede
			if (((bloco->LisRede())->IndexOf(rede)) >= 0)
			{
				valores[lvcolCIC_CLIh] += resBloco->tdesl * resBloco->ncons / 60.0;
				valores[lvcolCIC_CLI] += resBloco->ndesl * resBloco->ncons;
				nconsu_antigo = global[lvcolCIC_Consu];
				valores[lvcolCIC_Consu] += resBloco->ncons;
				if (IsDoubleZero(valores[lvcolCIC_Consu]) == false)
				{
					valores[lvcolCIC_DEC] = valores[lvcolCIC_CLIh] / valores[lvcolCIC_Consu];

					valores[lvcolCIC_FEC] = valores[lvcolCIC_CLI] / valores[lvcolCIC_Consu];
				}
				else
				{
					valores[lvcolCIC_DEC] += 0.0;
					valores[lvcolCIC_FEC] += 0.0;
				}
				valores[lvcolCIC_END] += resBloco->end;
				valores[lvcolCIC_ConsuVIP] += resBloco->nconsVIP;
				valores[lvcolCIC_FECPISCA] += resBloco->npiscadas;
			}
		}
		decRede = resRede->tdeslBTN / 60.0;
		fecRede = resRede->ndeslBTN;
		valores[lvcolCIC_ENDBT] = endRede = resRede->endBT;
		// GH201708: agrega valores de clih e cli interrompidos BT no clih e cli total
		valores[lvcolCIC_CLIh] = valores[lvcolCIC_CLIh] + decRede;
		// decRede está em clientes hora interrompidos
		valores[lvcolCIC_CLI] = valores[lvcolCIC_CLI] + fecRede;
		// fecRede está em clientes interrompidos
		valores[lvcolCIC_END] = valores[lvcolCIC_END] + valores[lvcolCIC_ENDBT];
		if (nconsRede > 0)
		{
			// GH201708: agrega valores de DEC e FEC BT no DEC e FEC total
			valores[lvcolCIC_DEC] = valores[lvcolCIC_CLIh] / valores[lvcolCIC_Consu];
			valores[lvcolCIC_FEC] = valores[lvcolCIC_CLI] / valores[lvcolCIC_Consu];
			valores[lvcolCIC_DECBT] = decRede /= nconsRede;
			valores[lvcolCIC_FECBT] = fecRede /= nconsRede;
		}
		else
		{
			valores[lvcolCIC_DECBT] = valores[lvcolCIC_FECBT] = decRede = fecRede = 0.0;
		}
		nconsuVIP_antigo = valores[lvcolCIC_ConsuVIP];
		valores[lvcolCIC_ConsuVIP] = resRede->nconsVIP;
		if (resRede->nconsVIP > 0)
		{
			valores[lvcolCIC_FECPISCAVIP] = fecRede = resRede->ndeslPiscaVIP / resRede->nconsVIP;
		}
		else
		{
			valores[lvcolCIC_FECPISCAVIP] = fecRede = 0.0;
		}
		// Preenche lvResultRede
		item = lvResultRede->Items->Add();
		item->Caption = rede->Codigo;
		item->Data = rede;
		item->SubItems->Add("-");
		for (int ni = 2; ni < lvcolCIC_Count; ni++)
		{
			item->SubItems->Add(DoubleToStr("%6.2f", valores[ni]));
		}
		// soma os valores globais
		global[lvcolCIC_CLIh] += valores[lvcolCIC_CLIh];
		global[lvcolCIC_CLI] += valores[lvcolCIC_CLI];
		global[lvcolCIC_Consu] += valores[lvcolCIC_Consu];
		global[lvcolCIC_END] += valores[lvcolCIC_END];
		global[lvcolCIC_ConsuVIP] += valores[lvcolCIC_ConsuVIP];
		global[lvcolCIC_ENDBT] += valores[lvcolCIC_ENDBT];
		if (IsDoubleZero(global[lvcolCIC_Consu]) == false)
		{
			global[lvcolCIC_DECBT] =
				((global[lvcolCIC_DECBT] * nconsu_antigo) +
				(valores[lvcolCIC_DECBT] * valores[lvcolCIC_Consu])) / global[lvcolCIC_Consu];
			global[lvcolCIC_FECBT] =
				((global[lvcolCIC_FECBT] * nconsu_antigo) +
				(valores[lvcolCIC_FECBT] * valores[lvcolCIC_Consu])) / global[lvcolCIC_Consu];
			global[lvcolCIC_FECPISCA] =
				((global[lvcolCIC_FECPISCA] * nconsu_antigo) +
				(valores[lvcolCIC_FECPISCA] * valores[lvcolCIC_Consu])) / global[lvcolCIC_Consu];
		}
		if (IsDoubleZero(global[lvcolCIC_ConsuVIP]) == false)
		{
			global[lvcolCIC_FECPISCAVIP] =
				((global[lvcolCIC_FECPISCAVIP] * nconsuVIP_antigo) +
				(valores[lvcolCIC_FECPISCAVIP] * valores[lvcolCIC_ConsuVIP]))
				/ global[lvcolCIC_ConsuVIP];
		}
	}
	// preenche valor global
	item = lvResultRede->Items->Add();
	item->Caption = "Global";
	item->Data = NULL;
	item->SubItems->Add("-");
	if (global[lvcolCIC_Consu] != 0)
	{
		global[lvcolCIC_DEC] = global[lvcolCIC_CLIh] / global[lvcolCIC_Consu];
		global[lvcolCIC_FEC] = global[lvcolCIC_CLI] / global[lvcolCIC_Consu];
	}
	else
	{
		global[lvcolCIC_DEC] += 0.0;
		global[lvcolCIC_FEC] += 0.0;
	}
	for (int ni = 2; ni < lvcolCIC_Count; ni++)
	{
		item->SubItems->Add(DoubleToStr("%6.2f", global[ni]));
		// a = item->SubItems->Strings[ni-2];
	}
	// Preenche lvResultDefeito
	lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	lisAuxDefeito = new TList();
	lisAuxDefeito->Assign(calcIndCont->LisDefeitos, laOr);
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];
		if (!rede->Carregada)
			continue; // hk201702: alteração geral
		lisBloco->Clear();
      calcIndCont->BuscaProt->ListaOrdenadaBlocos(lisBloco, rede); ////hk201808: sai: funcao de buscaProt: calcIndCont->ListaOrdenadaBlocos(lisBloco, rede);
		for (int nb = 0; nb < lisBloco->Count; nb++)
		{
			bloco = (VTBloco*)lisBloco->Items[nb];
			for (int n = lisAuxDefeito->Count - 1; n >= 0; n--)
			{
				pdef = (VTDefeito*)lisAuxDefeito->Items[n];
				if (pdef->Rede != rede)
					continue;
				if (pdef->Bloco == bloco)
				{
					lisAuxDefeito->Remove(pdef);
					item = lvResultDefeito->Items->Add();
					item->Data = pdef;
					item->Caption = rede->Codigo;
					if (pdef->ChvProt)
					{
						aux.sprintf("(%s)%s", pdef->ChvProt->TipoChave->Codigo.c_str(),
							pdef->ChvProt->Codigo.c_str());
						item->SubItems->Add(aux);
					}
					else
						item->SubItems->Add("");
					item->SubItems->Add(IntToStr(nb + 1));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->ComprimentoBloco));
					item->SubItems->Add(DoubleToStr("%6.2f", 1000* pdef->NFalhas));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->DCons)); // / nconsTot
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->TCons / 60.0)); // / nconsTot
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->NmanAuto)); // hkedp int->double
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->NmanMan)); // hkedp int->double
					// hk201710IF
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->TReparo));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->Ptpre));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->Ptdlo));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->Ptexe));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->FatIFDlo));
					item->SubItems->Add(DoubleToStr("%6.2f", pdef->FatIFPre));
					break;
				}
			}
		}
	}
	delete lisAuxDefeito;
	delete lisBloco;

}

//---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ImprimeResCalcIndCont(void)
	{
   //VTCalcIndCont *calcIndCont;
	VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede     *rede;
   TList *lisRedes;
	VTResCont         *resCont;
   strResRede        *resRede;
   FILE *fout = NULL;
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq = path->DirTmp() + "\\SaidaIFconf.txt";

   //calcIndCont = avaliaPosIF->CalcIndCont();


   fout = fopen(arq.c_str(), "a");
   fprintf(fout, "------------------------------------------------------------\n");
   lisRedes = redes->LisRede();
   resCont = calcIndCont->ResCont;
   for(int nr = 0; nr < lisRedes->Count; nr++)
      {
      rede = (VTRede*)lisRedes->Items[nr];
      if((resRede = resCont->ResRede[rede]) != NULL)
		 fprintf(fout, "%s\t %6.2f\t %6.2f\t %6.2f\t %i\n", rede->Codigo.c_str(), resRede->tdeslN, resRede->end, resRede->compensacao, resRede->ncons);
      else
		 fprintf(fout, "%s\n", rede->Codigo.c_str());
      }
   fprintf(fout, "------------------------------------------------------------\n");
   fclose(fout);


	// Preenche lvResultDefeito
   arq = path->DirTmp() + "\\SaidaIFconf1.txt";
   fout = fopen(arq.c_str(), "a");
   fprintf(fout, "------------------------------------------------------------\n");
   TList *lisAuxDefeito, *lisBloco;
   VTDefeito *pdef;
   VTBloco   *bloco;
   strResBloco *resBloco;
   TList *lisAuxResBloco;

	//lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	lisAuxDefeito = new TList();
   lisAuxResBloco = new TList();
   lisBloco = new TList();
   lisAuxResBloco->Assign(resCont->LisResBloco, laOr);
	lisAuxDefeito->Assign(calcIndCont->LisDefeitos, laOr);
	for (int nr = 0; nr < lisRedes->Count; nr++)
	{
		rede = (VTRede*)lisRedes->Items[nr];
		if (!rede->Carregada)
			continue; // hk201702: alteração geral
		lisBloco->Clear();
      calcIndCont->BuscaProt->ListaOrdenadaBlocos(lisBloco, rede); ////hk201808: sai: funcao de buscaProt: calcIndCont->ListaOrdenadaBlocos(lisBloco, rede);
		for (int nb = 0; nb < lisBloco->Count; nb++)
		{
			bloco = (VTBloco*)lisBloco->Items[nb];
			for (int n = lisAuxDefeito->Count - 1; n >= 0; n--)
			{
				pdef = (VTDefeito*)lisAuxDefeito->Items[n];
				if (pdef->Rede != rede)
					continue;
				if (pdef->Bloco == bloco)
				{
			   fprintf(fout, "%s\t", rede->Codigo.c_str());
			   if(pdef->ChvIsolaMont) fprintf(fout, "%s\t", pdef->ChvIsolaMont->Codigo.c_str());
               else fprintf(fout, "-\t");
               fprintf(fout, "%10.4f\t", pdef->ComprimentoBloco);
               fprintf(fout, "%10.4f\t", 1000* pdef->NFalhas);
               fprintf(fout, "%10.4f\t", pdef->DCons);
               fprintf(fout, "%10.4f\t", pdef->TCons);

               lisAuxDefeito->Delete(n);

					break;
				}
			}
         bool flagResBloco = false;
			for (int n = lisAuxResBloco->Count - 1; n >= 0; n--)
			{
         resBloco = (strResBloco*)lisAuxResBloco->Items[n];
         if(resBloco->rede != rede) continue;
         if(resBloco->bloco == bloco)
           {
           fprintf(fout, "%10.4f\t", resBloco->tdesl);
           fprintf(fout, "%10.4f\t", resBloco->ndesl);
           fprintf(fout, "%10.4f\t", resBloco->end);
           fprintf(fout, "%i\n", resBloco->ncons);

           flagResBloco = true;
           lisAuxResBloco->Delete(n);
           break;
           }
         }
         if(!flagResBloco) fprintf(fout, "0000000000\n");


		}
	}
   fprintf(fout, "------------------------------------------------------------\n");
   fclose(fout);
	delete lisAuxDefeito;
   delete lisBloco;
   delete lisAuxResBloco;
   }
// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necessário, cria um objeto EdtBusca
		if (edt_busca != NULL)
			delete edt_busca;
		// cria EdtBusca p/ controle do mouse
		edt_busca = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionMostraResBlocoExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;
	VTDefeito *pdef;
	if ((item = lvResultDefeito->Selected) == NULL)
		return;
	pdef = (VTDefeito*)item->Data;

	grafico->DestacaEqpto(pdef->Bloco->LisLigacao(), clRed, 10);

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionImprimeParamSimulaExecute(TObject *Sender)

{ // hkv201608a Impressao de parametros de simulacao : ActionImprimeParamSimulaExecute(TObject *Sender)
	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// imprime o relatório
		calcIndCont->ParamConf->ImprimeParametrosSimula(SaveDialog->FileName);
	}
	catch (Exception &e)
	{
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionInsereConjuntoExecute(TObject *Sender)
{
	// WIP
	// Objetivo: Criar um conjunto aqui, exibindo-o na aba Ajuste de Parâmetros.
	// Criar um novo objeto de VerifSE através de um pop-up(novo form)

	TList *lisSeParametro = new TList();

	PreencheListaParametroSe(lisSeParametro);

	TFormInsereConjunto *form = new TFormInsereConjunto(this, apl, lisSeParametro, calcIndCont);
	form->ShowModal();
	delete form;

	SgridAjusteInicia();

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionDeletaConjuntoExecute(TObject *Sender)
{
	// WIP
	int linhaSelecionada = sgridAjuste->Row;
	VTVerifCjto* cjto = (VTVerifCjto*)sgridAjuste->Objects[0][linhaSelecionada];
   calcIndCont->ParamConf->LisVerifCjto->Remove(cjto); //hk201808: reformula: calcIndCont->LisVerifCjto->Remove(cjto);
	SgridAjusteInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::PreencheListaParametroSe(TList* lisSeParametro)
{
	// Variáveis locais
	TList *lisVerifSe = new TList();
	TList *lisCjto = new TList();
	TList *lisRedes = new TList();
	VTRede *rede;
	VTVerifSe *verifSeRede;
	VTVerifSe *verifSeCjto;
	VTVerifCjto *cjto;

	lisCjto->Assign(calcIndCont->ParamConf->LisVerifCjto, laCopy);//hk201808: reformula: lisCjto->Assign(calcIndCont->LisVerifCjto, laCopy);

	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	lisRedes->Assign(redes->LisRede(), laCopy);

	// Cria a lista de VerifSe a partir das Redes
	for (int i = 0; i < lisRedes->Count; i++)
	{
		rede = (VTRede*)lisRedes->Items[i];
		if (rede->TipoRede->Segmento == redeETD)
		{
			verifSeRede = calcIndCont->ParamConf->CriaVerifSe();//hk201805: NewObjVerifSe();
			verifSeRede->Codigo = rede->Codigo;
			lisSeParametro->Add(verifSeRede);
		}
	}

	// Cria a lista de VerifSe que já estão em algum VerifCjto
	for (int i = 0; i < lisCjto->Count; i++)
	{
		cjto = (VTVerifCjto*)lisCjto->Items[i];
		for (int j = 0; j < cjto->LisVerifSe->Count; j++)
		{
			verifSeCjto = (VTVerifSe*)cjto->LisVerifSe->Items[j];
			lisVerifSe->Add(verifSeCjto);
		}
	}

	// Filtra as SE
	for (int i = 0; i < lisVerifSe->Count; i++)
	{
		verifSeRede = (VTVerifSe*)lisVerifSe->Items[i];
		for (int j = 0; j < lisSeParametro->Count; j++)
		{
			verifSeCjto = (VTVerifSe*)lisSeParametro->Items[j];
			if (verifSeRede->Codigo.AnsiCompare(verifSeCjto->Codigo) == 0)
			{
				// lisSeParametro->Remove(verifSeCjto);
				lisSeParametro->Delete(j);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExportaRelatorioCompensacoesExecute(TObject *Sender)

{
	// variáveis locais
	VTReport *report;
	AnsiString arq_ext;
	AnsiString titulo;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		titulo = "Resultado por equipamento";
		if (arq_ext.AnsiCompareIC(".xls") == 0)
		{
			// report->PlanilhaExcel(SaveDialog->FileName, "", lvResultDefeito);
		}
		else
		{
			report->ArquivoTexto(SaveDialog->FileName, lvComp);
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExportaRelatorioExecute(TObject *Sender)
{
	// variáveis locais
	VTReport *report;
	AnsiString arq_ext;
	AnsiString titulo;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		titulo = "Resultado por equipamento";
		if (arq_ext.AnsiCompareIC(".xls") == 0)
		{
			// report->PlanilhaExcel(SaveDialog->FileName, "", lvResultDefeito);
		}
		else
		{
			report->ArquivoTexto(SaveDialog->FileName, lvResultDefeito);
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
	}
	catch (Exception &e)
	{
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExportaRelatVerifAlimBlocoExecute(TObject *Sender)
{
	// variáveis locais
	VTReport *report;
	AnsiString arq_ext;
	AnsiString titulo1, titulo2;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		titulo1 = "Parâmetros por alimentador";
		titulo2 = "Parâmetros por bloco";
		if (arq_ext.AnsiCompareIC(".xls") == 0)
		{
			// report->PlanilhaExcel(SaveDialog->FileName, titulo1, lvParamRede);
			// report->PlanilhaExcel(SaveDialog->FileName, titulo2, lvParamBloco);
		}
		else
		{
			report->ArquivoTexto(SaveDialog->FileName, lvParamRede, titulo1);
			report->ArquivoTexto(SaveDialog->FileName, lvParamBloco, titulo2, true);
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
	}
	catch (Exception &e)
	{
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionImprimeCircuitosSocorroExecute(TObject *Sender)

{
	// variáveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int numPat = patamares->NumPatamar();
	AnsiString arq_ext;
	AnsiString titulo, aux;
	VTVisRede *visRede;
	double daux, demAux;
	strDelta *delta;
	FILE *fout;
	TTreeNode *nodeRede, *nodeVis;
	VTRede *redeDef, *redeSoc;

	if (!SelecionaArquivo())
		return;
	// imprime o relatório
	arq_ext = SaveDialog->FileName;

	fout = fopen(arq_ext.c_str(), "wt");
	titulo = "Relatório de Circuitos de Socorro";
	fprintf(fout, "---------------------------------------------------------\n");
	fprintf(fout, "-------------   %s\n   ------------------------", titulo.c_str());
	fprintf(fout, "---------------------------------------------------------\n");
	fprintf(fout, "RedeSocorrida; RedeSocorro\n");

	for (int n = 0; n < tvCircuitosSocorro->Items->Count; n++)
	{
		nodeRede = tvCircuitosSocorro->Items->Item[n];
		redeDef = (VTRede*)(nodeRede->Data);

		if (nodeRede->Parent != NULL)
			continue;
		for (int nc = 0; nc < nodeRede->Count; nc++)
		{
			nodeVis = nodeRede->Item[nc];
			redeSoc = (VTRede*)nodeVis->Data;
			aux.sprintf("%s; ", redeDef->Codigo.c_str());
			aux.cat_sprintf("%s\n", redeSoc->Codigo.c_str());
			fprintf(fout, "%s", aux.c_str());
		}
	}

	fclose(fout);

}
//---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionImprimeRelNAExecute(TObject *Sender)
{
	// variáveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int numPat = patamares->NumPatamar();
	AnsiString arq_ext;
	AnsiString titulo, aux;
	VTVisRede *visRede;
	double daux, demAux;
	strDelta *delta;
	FILE *fout;
	TTreeNode *nodeRede, *nodeVis;
	VTRede *redeDef, *redeSoc;

	if (!SelecionaArquivo())
		return;
	// imprime o relatório
	arq_ext = SaveDialog->FileName;

	fout = fopen(arq_ext.c_str(), "wt");
	titulo = "Relatório de Conexões VIS";
	fprintf(fout, "---------------------------------------------------------\n");
	fprintf(fout, "-------------   %s\n   ------------------------", titulo.c_str());
	fprintf(fout, "---------------------------------------------------------\n");
	fprintf(fout, "RedeDef; RedeSoc; ChaveNA\n");

	for (int n = 0; n < tvChavesNA->Items->Count; n++)
	{
		nodeRede = tvChavesNA->Items->Item[n];
		redeDef = (VTRede*)(nodeRede->Data);
		if (nodeRede->Parent != NULL)
			continue;
		for (int nc = 0; nc < nodeRede->Count; nc++)
		{
			nodeVis = nodeRede->Item[nc];
			visRede = (VTVisRede*)(nodeVis->Data);
			if (visRede->rede1 == redeDef)
				redeSoc = visRede->rede2;
			else
				(redeSoc = visRede->rede1);
			aux.sprintf("%s; ", redeDef->Codigo.c_str());
			aux.cat_sprintf("%s; ", redeSoc->Codigo.c_str());
			aux.cat_sprintf("%s\n", visRede->pchave->Codigo.c_str());
			fprintf(fout, "%s", aux.c_str());
		}
	}

	titulo = "Relatório de folgas NA";
	fprintf(fout, "---------------------------------------------------------\n");
	fprintf(fout, "-------------   %s\n   ------------------------", titulo.c_str());
	fprintf(fout, "---------------------------------------------------------\n");
	fprintf(fout, "ChaveNA; RedeTransf; LigaCrit; pat(h); Folga(A); Iadm(A); I(A); DMax(Mw)\n");

	for (int n = 0; n < calcIndCont->LisVisRede->Count; n++)
	{
	  visRede = (VTVisRede*)calcIndCont->LisVisRede->Items[n];
		if (visRede->rede1 != NULL)
		{
			for (int np = 0; np < numPat; np++)
			{
				fprintf(fout, "%s; ", visRede->pchave->Codigo.c_str());
				fprintf(fout, "%s; ", visRede->rede1->Codigo.c_str());
				delta = (strDelta*)visRede->ligaCrit1->lisDelta->Items[np];
				fprintf(fout, "%s; ", delta->pliga->Codigo.c_str());
				daux = delta->dMax - delta->ia_nom * (1 - calcIndCont->ParamConf->ParamGerais->pu_lim);//hk201808: reformula: daux = delta->dMax - delta->ia_nom * (1 - calcIndCont->PuLim);
				demAux = daux * (visRede->pbarra1->vnom * 1.73205) / 1000.;
				aux.sprintf("%i; %6.4f; %6.4f; %6.4f; %6.4f\n", np, daux, delta->ia_nom,
					delta->ifluxo, demAux);
				fprintf(fout, "%s ", aux.c_str());
			}
		}

		if (visRede->rede2 != NULL)
		{
			for (int np = 0; np < numPat; np++)
			{
				fprintf(fout, "%s; ", visRede->pchave->Codigo.c_str());
				fprintf(fout, "%s; ", visRede->rede2->Codigo.c_str());
				delta = (strDelta*)visRede->ligaCrit2->lisDelta->Items[np];
				fprintf(fout, "%s; ", delta->pliga->Codigo.c_str());
			daux = delta->dMax - delta->ia_nom * (1 - calcIndCont->ParamConf->ParamGerais->pu_lim);//hk201808: reformula: daux = delta->dMax - delta->ia_nom * (1 - calcIndCont->PuLim);
				demAux = daux * (visRede->pbarra1->vnom * 1.73205) / 1000.;
				aux.sprintf("%i; %6.4f; %6.4f; %6.4f; %6.4f\n", np, daux, delta->ia_nom,
					delta->ifluxo, demAux);
				fprintf(fout, "%s ", aux.c_str());
			}
		}
	}
	fclose(fout);

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionSelEqptoExecute(TObject *Sender)
{
	TList *lisEqpto;
	VTEqpto *eqpto;
	VTDefeito *pdef;
	TFormMostraDefeito *formMostraDefeito;

	if (Sender == NULL)
		return;
	lisEqpto = (TList*)Sender;
	if (lisEqpto->Count == 0)
		return;
	// seta equipamento
	eqpto = (VTEqpto*)lisEqpto->First();
	// busca defeito
	pdef = calcIndCont->IdentificaDefeito(eqpto);
	if (pdef == NULL)
		return;
	// Seleciona linha em lvResultDefeito
	TListItem *item;
	lvResultDefeito->ItemIndex = 0;
	for (int nl = 0; nl < lvResultDefeito->Items->Count; nl++)
	{
		item = lvResultDefeito->Items->Item[nl];
		if (item->Data == pdef)
		{
			lvResultDefeito->ItemFocused = item;
			lvResultDefeito->ItemIndex = nl;
			break;
		}
	}

	formMostraDefeito = new TFormMostraDefeito(NULL, this->Parent, apl, pdef, calcIndCont->LisChavesAuto);
	formMostraDefeito->Show();

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionReiniciarExecute(TObject *Sender)
{
	// calcIndCont->Executa(NULL, NULL);
	// Reinicia listView de compensações
	// LvCompInicia();
	// LVResultInicia();

	int currCursor = Screen->Cursor;
	StatusBarCount->SimpleText = "Reiniciando módulo de cálculo de indicadores..";
	Screen->Cursor = crHourGlass;

	calcIndCont->IniciaProblema(NULL);
	calcIndCont->IniciaListasResultados(NULL);
	calcIndCont->AvaliaFolgaVis();
	// Reinicia lista de redes ordenada em caso de acréscimo ou deleção de rede
	IniciaLisRedOrdenada();
	// TVChavesInicia(calcIndCont->ParamConf->ParamGerais->flagRecurso);//hk1408
	TVChavesInicia(); // hk1408
	// Atualiza interface de dados gerais
	AtualizaInterfaceDadosGerais();
	// Atualiza listView
	LvParamRedeInicia();
	LvParamBlocoInicia();
	// Reinicia grid com parâmetros
	SgridParamRedeBlocoInicia();
	// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia

	StatusBarCount->SimpleText = "Módulo de cálculo de indicadores reiniciado.";
	Screen->Cursor = (TCursor)currCursor;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionMostraBlocoParamExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;
	TList *lisLiga, *lisEqpto;
	strDadoParamBloco *paramBloco;

	lisLiga = new TList();
	lisEqpto = new TList();
	for (int n = 0; n < lvParamBloco->Items->Count; n++)
	{
		if (!((item = lvParamBloco->Items->Item[n])->Selected))
			continue;
		paramBloco = (strDadoParamBloco*)item->Data;
		lisLiga->Assign((paramBloco->bloco)->LisLigacao(), laOr);
		lisEqpto->Add(paramBloco->eqpto);
	}
	grafico->DestacaEqpto(lisLiga, clYellow, 10);
	grafico->Moldura(lisEqpto);
	delete lisLiga;
	delete lisEqpto;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionSelChvAutoExecute(TObject *Sender)
{
	VTChave *pchave;

	for (int n = 0; n < lbChaves->Items->Count; n++)
	{
		if (!lbChaves->Selected[n])
			continue;
		pchave = (VTChave*)lbChaves->Items->Objects[n];
		// Insere chave em calcIndCont->LisChavesAuto
		// gh201809
		calcIndCont->InsereChaveAuto(pchave, true, false);
		// calcIndCont->LisChavesAuto->Add(pchave);
	}
	LbChavesInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionRemChvAutoExecute(TObject *Sender)
{
	VTChave *pchave;
	for (int n = 0; n < lbChavesAuto->Items->Count; n++)
	{
		if (!lbChavesAuto->Selected[n])
			continue;
		pchave = (VTChave*)lbChavesAuto->Items->Objects[n];
		calcIndCont->RemoveChaveAuto(pchave, true);
		// calcIndCont->LisChavesAuto->Remove(pchave);
	}
	LbChavesInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::tvIFClick(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *nodeIF;
	strIF *infIF;
	TList *lisAux;
	TColor color;
	VTBloco *bloco;
	TList *lisLiga;

	if ((nodeIF = tvIF->Selected) == NULL)
		return;
	if (nodeIF->Data == NULL)
		return;

	infIF = (strIF*)nodeIF->Data;
	grafico->Moldura();
	if (nodeIF->Level == 0)
	{
		grafico->Moldura(infIF->chave);
	}

	if (nodeIF->Level == 1)
	{
		lisAux = new TList();
		if (nodeIF->Text.Pos("lisBlJus") > 0)
		{
			if (infIF->lisBlJus->Count > 0)
			{
				if (IsDoubleZero(infIF->compIF))
					grafico->Moldura(((VTBloco*)infIF->lisBlJus->Items[0])->LisBarra());
				else
				{
					for (int n = 0; n < infIF->lisBlJus->Count; n++)
					{
						lisLiga = ((VTBloco*)infIF->lisBlJus->Items[n])->LisLigacao();
						if (lisLiga->Count > 0)
							color = ((VTBloco*)infIF->lisBlJus->Items[n])->Rede->Color;
						lisAux->Assign(lisLiga, laOr);
					}
					grafico->DestacaEqpto(lisAux, color, 10);
				}
			}
		}

		if (nodeIF->Text.Pos("lisBlProt") > 0)
		{
			if (infIF->lisBlProt->Count > 0)
			{
				if (IsDoubleZero(infIF->compProt))
					grafico->Moldura(((VTBloco*)infIF->lisBlProt->Items[0])->LisBarra());
				else
				{

					for (int n = 0; n < infIF->lisBlProt->Count; n++)
					{
						lisLiga = ((VTBloco*)infIF->lisBlProt->Items[n])->LisLigacao();
						if (lisLiga->Count > 0)
							color = ((VTBloco*)infIF->lisBlProt->Items[n])->Rede->Color;
						lisAux->Assign(lisLiga, laOr);
					}
					grafico->DestacaEqpto(lisAux, color, 10);
				}
			}
		}

		if (nodeIF->Text.Pos("ChaveProt") > 0)
		{
			lisAux->Add(infIF->chave);
			lisAux->Add(infIF->chaveProt);
			grafico->Moldura(lisAux);
		}
		delete lisAux;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionSelChaveIFExecute(TObject *Sender)
{
	VTChave *pchave;

	for (int n = 0; n < lbChavesSemIF->Items->Count; n++)
	{
		if (!lbChavesSemIF->Selected[n])
			continue;
		pchave = (VTChave*)lbChavesSemIF->Items->Objects[n];
		// Insere chave em calcIndCont->LisChavesAuto
		calcIndCont->InsereChaveIF(pchave);
		// calcIndCont->LisChavesAuto->Add(pchave);
	}
	LbChavesIFInicia();
	ActionPreparaIndicacaoFaltaExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionRemChaveIFExecute(TObject *Sender)
{
	VTChave *pchave;

	for (int n = 0; n < lbChavesComIF->Items->Count; n++)
	{
		if (!lbChavesComIF->Selected[n])
			continue;
		pchave = (VTChave*)lbChavesComIF->Items->Objects[n];
		// Insere chave em calcIndCont->LisChavesAuto
		calcIndCont->RemoveChaveIF(pchave);
		// calcIndCont->LisChavesAuto->Add(pchave);
	}
	LbChavesIFInicia();
	ActionPreparaIndicacaoFaltaExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionPreparaIndicacaoFaltaExecute(TObject *Sender)
{
	strIF *infIF;
	TTreeNode *nodeIF;
	AnsiString aux;

   calcIndCont->BuscaProt->PreparaIndicacaoFalta();//hk201808: reformula: mudar para calcIndCont->BuscaProt
	tvIF->Items->Clear();
	for (int n = 0; n < calcIndCont->LisEQIF->Count; n++)
	{
		infIF = (strIF*)calcIndCont->LisEQIF->Items[n];

		if ((IsDoubleZero(infIF->compIF - infIF->compProt)) && (calcIndCont->LisChavesIF->IndexOf(infIF->chave) == -1))
			continue;

		aux.sprintf("Chave: %s(%s)", infIF->chave->Codigo.c_str(), infIF->chave->TipoChave->Codigo.c_str());
		nodeIF = tvIF->Items->AddChildObject(NULL, aux, infIF);

		aux.sprintf("ChaveProt: %s(%s)", infIF->chaveProt->Codigo.c_str(),
			infIF->chaveProt->TipoChave->Codigo.c_str());
		if (infIF->flagIF)
			aux.cat_sprintf("(IF)");
		tvIF->Items->AddChildObject(nodeIF, aux, infIF);
		aux.sprintf("lisBlJus: %6.2f km", infIF->compIF);
		tvIF->Items->AddChildObject(nodeIF, aux, infIF);
		aux.sprintf("lisBlProt: %6.2f km", infIF->compProt);
		tvIF->Items->AddChildObject(nodeIF, aux, infIF);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionSalvarExecute(TObject *Sender)
{
	GravaArqParam();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::FormClose(TObject *Sender, TCloseAction &Action)
{
	// variáveis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	// hkrel GravaArqParam();
	// Reinicia chaves com status inicial
	if (calcIndCont)
	{
		calcIndCont->ReiniciaChavesAuto();
	}
	// esconde e desabilita o Form
	Hide();
	Enabled = false;
	// seleciona gráfico de topologia
	sinap_child->ActionTopo->Execute();
	// destrói o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::TVChavesInicia(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRede *rede, *redeTransf;
	VTPatamar *patamar;
	int numPat = patamares->NumPatamar();
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	TTreeNode *node, *nodeRede, *nodeCircuitoSocorrido;
	TTreeNode *node1, *node1A, *node1B;
	TTreeNode *node2, *node2A, *node2B;
	VTBloco *bloco;
	TCargaBloco *cBloco;
	VTVisRede *visRede;
	VTLigacao *pliga;
	strDelta *delta;
	VTCarga *carga;
	TList *lisCargas, *lisBlocoAux, *lisRede;
	AnsiString aux;
	double demAux;
	TList *lisBlocoRadTot;
	VTBlocoRad *blRad;
	int indBloco;
	double ctot, daux;
	TList *lisChaves;
	VTTipoChave *tipoChave;
	VTChave *pchave;
	// hk2007 VTBlocos	   *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBlocos *blocos = calcIndCont->Blocos;
	TList *lisBlocos = blocos->LisBloco();

	// Reinicia treeview de blocos e chaves vis
	tvChavesNA->Items->Clear();
	tvChavesProt->Items->Clear();
	tvCircuitosSocorro->Items->Clear();//hk201901

	TList *lisCircuitosSocorro = new TList();


	lisCargas = new TList();
	lisBlocoAux = new TList();

	for (int nrede = 0; nrede < lisRedeOrdenada->Count; nrede++)
	{
		rede = (VTRede*)lisRedeOrdenada->Items[nrede];
		nodeRede = tvChavesNA->Items->AddChildObject(NULL, rede->Codigo, rede);

		lisCircuitosSocorro->Clear();
		nodeCircuitoSocorrido = tvCircuitosSocorro->Items->AddChildObject(NULL, rede->Codigo, rede);

		for (int n = 0; n < calcIndCont->LisVisRede->Count; n++)//hk201808: reformula
		{
			visRede = (VTVisRede*)calcIndCont->LisVisRede->Items[n];//hk201808: reformula
			if ((visRede->rede1 != rede) && (visRede->rede2 != rede))
				continue;

			if (visRede->rede1 == rede)
				redeTransf = visRede->rede2;
			else
				redeTransf = visRede->rede1;

			aux.sprintf("%s (%s)", visRede->pchave->Codigo.c_str(), redeTransf->Codigo.c_str());
			node = tvChavesNA->Items->AddChildObject(nodeRede, aux, visRede);

			if(lisCircuitosSocorro->IndexOf(redeTransf) == -1)
			  {
			  lisCircuitosSocorro->Add(redeTransf);
			  tvCircuitosSocorro->Items->AddChildObject(nodeCircuitoSocorrido, redeTransf->Codigo, redeTransf);
			  }

			// node1A= tvChaves->Items->AddChildObject(node1, "LigaCrit1", NULL);
			node1 = tvChavesNA->Items->AddChildObject(node,
				"Bar1: (Folga(A), Iadm(A), I(A), DMax(Mw))" + visRede->pbarra1->Codigo,
				visRede->pbarra1);

			aux = "";
			if (visRede->rede1)
				aux = "Rede de socorro: " + visRede->rede1->Codigo;
		 //if(visRede->bloco1->LisBarra()->IndexOf(visRede->pbarra1) == -1)
		 //   aux+= "problema1";
			tvChavesNA->Items->AddChildObject(node1, aux, visRede->bloco1);
			tvChavesNA->Items->AddChildObject(node1, "Caminho", visRede->lisLiga1);

			node2 = tvChavesNA->Items->AddChildObject(node,
				"Bar2: (Folga(A), Iadm(A), I(A), DMax(Mw))" + visRede->pbarra2->Codigo,
				visRede->pbarra2);
			aux = "";
			if (visRede->rede2)
				aux = "Rede de socorro: " + visRede->rede2->Codigo;
		 //if(visRede->bloco2->LisBarra()->IndexOf(visRede->pbarra2) == -1)
		 //   aux+= "problema2";
			tvChavesNA->Items->AddChildObject(node2, aux, visRede->bloco2);
			tvChavesNA->Items->AddChildObject(node2, "Caminho", visRede->lisLiga2);
			for (int np = 0; np < numPat; np++)
			{
				delta = (strDelta*)visRede->ligaCrit1->lisDelta->Items[np];

				daux = delta->dMax - delta->ia_nom * (1 - calcIndCont->ParamConf->ParamGerais->pu_lim); //hk201808: reformula
				demAux = daux * (visRede->pbarra1->vnom * 1.73205) / 1000.;
				aux.sprintf("%i: %6.4f/ %6.4f/ %6.4f/ %6.4f", np, daux, delta->ia_nom,
					delta->ifluxo, demAux);
				tvChavesNA->Items->AddChildObject(node1, aux, delta);

				delta = (strDelta*)visRede->ligaCrit2->lisDelta->Items[np];
				daux = delta->dMax - delta->ia_nom * (1 - calcIndCont->ParamConf->ParamGerais->pu_lim); //hk201808: reformula
				demAux = daux * (visRede->pbarra1->vnom * 1.73205) / 1000.;
				aux.sprintf("%i: %6.4f/ %6.4f/ %6.4f/ %6.4f", np, daux, delta->ia_nom,
					delta->ifluxo, demAux);
				tvChavesNA->Items->AddChildObject(node2, aux, delta);
			}
		}

	}

	delete lisCircuitosSocorro;
	lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();

	lisChaves = new TList();
	redes->LisEqpto(lisChaves, eqptoCHAVE);

	for (int n = 0; n < calcIndCont->LisTipoEqProt->Count; n++)//hk201808: reformula
	{
		tipoChave = (VTTipoChave*)calcIndCont->LisTipoEqProt->Items[n];//hk201808: reformula
		node = tvChavesProt->Items->AddChildObject(NULL, tipoChave->Codigo, NULL);
		for (int nr = 0; nr < lisRede->Count; nr++)
		{
			rede = (VTRede*)lisRede->Items[nr];
			for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
			{
				pchave = (VTChave*)lisChaves->Items[nc];
				if (lisChavesFunde->IndexOf(pchave) == -1)
					continue;
				if (pchave->rede != rede)
					continue;
				if (pchave->TipoChave != tipoChave)
					continue;
				aux.sprintf("%s(%s)", pchave->Codigo.c_str(), rede->Codigo.c_str());
				if (pchave->Aberta)
					aux.cat_sprintf("-NA");
				tvChavesProt->Items->AddChildObject(node, aux, pchave);
				lisChaves->Delete(nc);
			}
		}
	}

	delete lisChaves;
	delete lisCargas;
	delete lisBlocoAux;

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::tvChavesProtClick(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node, *nodeAux;
	TList *lisChaves;

	if ((node = tvChavesProt->Selected) == NULL)
		return;

	lisChaves = new TList();
	if (node->Level == 0)
	{
		for (int nn = 0; nn < tvChavesProt->Items->Count; nn++)
		{
			nodeAux = tvChavesProt->Items->Item[nn];
			if (nodeAux->Parent != node)
				continue;

			lisChaves->Add(((VTEqpto*)nodeAux->Data));
		}

	}
	else
	{
		lisChaves->Add(node->Data);
	}
	grafico->DestacaEqpto(lisChaves, clBlue, 45);
	grafico->Moldura(lisChaves);
	delete lisChaves;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::tvChavesNAClick(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node;
	VTChave *chave;
	VTBarra *barra;
	VTBloco *bloco;
	TList *lisLiga, *lisAux;
	strDelta *delta;
	AnsiString obj_name;
	VTVisRede *visRede;

	if ((node = tvChavesNA->Selected) == NULL)
		return;
	if (!node->Data)
		return;

	lisAux = new TList();

	obj_name = ((TObject*)node->Data)->ClassName();

	if (obj_name.AnsiCompare("TRede") == 0)
	{
		for (int n = 0; n < node->Count; n++)
		{
			lisAux->Add(((VTVisRede*)node->Item[n]->Data)->pchave);
			// lisAux->Add(node->Item[n]->Data);
		}
		grafico->Moldura(lisAux);
	}

	if (obj_name.AnsiCompare("TBloco") == 0)
	{
		bloco = (VTBloco*)node->Data;
		grafico->DestacaEqpto(bloco->LisLigacao(), clGreen, 10);
	}

	// if(obj_name.AnsiCompare("TChave") == 0)
	if (obj_name.AnsiCompare("TVisRede") == 0)
	{
		lisAux->Clear();
		visRede = (VTVisRede*)(node->Data);

		lisAux->Add(visRede->pchave); // lisAux->Add(node->Data);
		grafico->Moldura(visRede->pchave); // grafico->Moldura((VTEqpto*)node->Data);
		grafico->DestacaEqpto(lisAux, clBlue, 10);
	}

	if (obj_name.AnsiCompare("TBarra") == 0)
	{
		barra = (VTBarra*)node->Data;
		grafico->Moldura(barra);
	}

	if (obj_name.AnsiCompare("TList") == 0)
	{
		lisLiga = (TList*)node->Data;
		grafico->DestacaEqpto(lisLiga, clRed, 10);
	}

	if (obj_name.AnsiCompare("strDelta") == 0)
	{
		delta = (strDelta*)node->Data;
		grafico->Moldura(delta->pliga);
	}
	delete lisAux;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::tvChavesProtMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	return;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTChave *chave;
	TTreeNode *node;
	TList *lisAux;

	lisAux = new TList();
	for (int n = 0; n < tvChavesProt->Items->Count; n++)
	{
		if (!tvChavesProt->Items->Item[n]->Selected)
			continue;
		if (tvChavesProt->Items->Item[n]->Data == NULL)
			continue;
		lisAux->Add(tvChavesProt->Items->Item[n]->Data);
	}
	grafico->Moldura(lisAux);
	delete lisAux;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultClick(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	strResBloco *resBloco;
	TListItem *item;
	if ((item = lvResult->Selected) == NULL)
		return;
	if (item->Data == NULL)
		return;
	resBloco = (strResBloco*)item->Data;
	// hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento
	// grafico->DestacaEqpto(((VTBloco*)resBloco->bloco)->LisLigacao(), ((VTBloco*)resBloco->bloco)->Rede->Color, 10);
	grafico->DestacaEqpto(((VTBloco*)resBloco->bloco)->LisLigacao(), resBloco->rede->Color, 10);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultDefeitoDblClick(TObject *Sender)
{
	TListItem *item;
	VTDefeito *pdef;
	if ((item = lvResultDefeito->Selected) == NULL)
		return;
	pdef = (VTDefeito*)item->Data;
	calcIndCont->ExecutaEstrategia(pdef);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExportaResIndicIndividuaisExecute(TObject *Sender)

{
	// variáveis locais
	VTReport *report;
	AnsiString arq_ext;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		if (arq_ext.AnsiCompareIC(".xls") == 0)
		{
			// report->PlanilhaExcel(SaveDialog->FileName, "", lvResultRede);
		}
		else
		{
			report->ArquivoTexto(SaveDialog->FileName, lvIndicIndiv);
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
	}
	catch (Exception &e)
	{
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionVoltaParamOriginaisExecute(TObject *Sender)
{
	// Reinicia taxas e tempos de reparo
	// lisParamEqpto->paramEqpto->lambda  , trep
	// paramEqpto->lamb[nm], tr[nm]
	// Funcao de:
	// paramEqpto->durTotal
	// paramEqpto->nFalhasEquip
	// paramEqpto->ndiasObs
	// GeraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);
	// e
	// Reinicia lisParamBloco->paramBloco->(lambda, trep);
	// Funcao de:
	// paramEqpto->lambda
	// paramEqpto->trep
	// GeraLisParamEqpto->RedistribuiFalhasCirc(paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);
	// (flagGeral,flagCircuito,flagEqpto,flagDistHora)
	bool flagGeral = chkBoxReiniciaParam->Checked[2];
	bool flagCircuito = chkBoxReiniciaParam->Checked[1];
	bool flagEqpto = chkBoxReiniciaParam->Checked[0];
	bool flagDistHora = chkBoxReiniciaParam->Checked[3];
	////flagGeral: 	Calcula paramGeral->lambda e paramGeral->trep
	////flagCircuito: Calcula paramRede->lambda e paramRede->trep
	////flagEqpto:		Calcula lisParamEqpto->paramEqpto->lambda  , trep
	////					Reinicia lisParamBloco->paramBloco->(lambda, trep)
	////flagDistHora:	Calcula paramConf->DistFalhasPat
   ////Reinicia parâmetros por defeito em função de parâmetros por bloco/alimentador/geral
   ////IniciaParamDefeito();
	calcIndCont->RetornaParametrosHistorico(flagGeral, flagCircuito, flagEqpto, flagDistHora);
	// calcIndCont->RetornaParametrosHistorico(false, true, true, false);

	// IMPRESSAO
	// VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));  //hkprov
	// FILE      *fout;
	// AnsiString arq;
	// arq = path->DirTmp() + "\\ParamSimula_RetornaParametrosHistorico.txt";
	// calcIndCont->ParamConf->ImprimeParametrosSimula(arq);

	// Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
	// lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
	// Atualiza taxas (por blocos-->por circuito-->valor geral
	// pdef->TaxaFalha         = paramBloco->lambda;
	// pdef->TReparo           = paramBloco->trep;
	// --->pdef->TaxaFalha     = paramRede->lambda;
	// --->pdef->TReparo       = paramRede->trep;
	// --->--->pdef->TaxaFalha = paramDefault->lambda;
	// --->--->pdef->TReparo   = paramDefault->trep;
	// hk201802c: Já executado em calcIndCont->RetornaParametrosHistorico: calcIndCont->IniciaParamDefeito();

	// Gera por mês pu mensal dos indicadores
	calcIndCont->ExecutaDicFicPuMensal();

	// Atualiza Interfaces
	AtualizaInterfaceDadosGerais();
	SgridFalhasPatInicia();
	LvParamRedeInicia();
	LvParamBlocoInicia();
	SgridParamRedeBlocoInicia();
	// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionVoltaParamMdbExecute(TObject *Sender)

{
	StatusBarCount->SimpleText = "Reiniciando dados..";
	calcIndCont->LeArquivosMdb("");
	// hk201702: Form de edição de parâmetros, botão voltar: Atualiza tela com os dados gravados em memória
	AtualizaInterfaceDadosGerais();
	LvParamRedeInicia();
	LvParamBlocoInicia();
	// Inicia grid de ajuste de valores hk201706
	SgridAjusteInicia();

	StatusBarCount->SimpleText = "Dados reiniciados.";
	// Reinicia taxas e tempos de reparo lisParamRede->paramRede(lambda, trep)
	// Funcao de:
	// paramEqpto->durTotal
	// paramEqpto->nFalhasEquip
	// paramEqpto->ndiasObs
	// calcIndCont->RetornaParametrosHistorico(false, true, false, false);

	// SgridParamRedeBlocoInicia();//hk201702: SgridParamRedeInicia para SgridParamRedeBlocoInicia
	// LvParamRedeInicia();

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExportaResultadosGeraisResRedeExecute(TObject *Sender)
{
	// variáveis locais
	VTReport *report;
	AnsiString arq_ext;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		if (arq_ext.AnsiCompareIC(".xls") == 0)
		{
			// report->PlanilhaExcel(SaveDialog->FileName, "", lvResultRede);
		}
		else
		{
			report->ArquivoTexto(SaveDialog->FileName, lvResultRede);
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
	}
	catch (Exception &e)
	{
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ActionExportaResultadosGeraisResExecute(TObject *Sender)
{
	// variáveis locais
	VTReport *report;
	AnsiString arq_ext;

	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		if (arq_ext.AnsiCompareIC(".xls") == 0)
		{
			// report->PlanilhaExcel(SaveDialog->FileName, "", lvResult);
		}
		else
		{
			report->ArquivoTexto(SaveDialog->FileName, lvResult);
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
	}
	catch (Exception &e)
	{
	}

}

// ---------------------------------------------------------------------------
bool __fastcall TFormCalcIndCont::SelecionaArquivo(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	int index;

	// mostra a janela de salvar acima das outras
	Application->ModalPopupMode = pmAuto;
	// verifica se o diretório SaveDialog já foi definido
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	// define extensão do arquivo
	index = SaveDialog->FilterIndex;
	if (index == 1)
		SaveDialog->DefaultExt = "txt";
	else if (index == 2)
		SaveDialog->DefaultExt = "csv";
	// else                 SaveDialog->DefaultExt = "xls";
	// define nome inicial do arquivo
	SaveDialog->FileName = "";
	// retorna
	return (SaveDialog->Execute());
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ConfirmaParametros(void)
{
	// variáveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	int numPat = patamares->NumPatamar();
	strDadoParamRede *paramRede;
	AnsiString aux;
	VTRede *rede;

	for (int np = 0; np < numPat; np++)
		calcIndCont->ParamConf->DistFalhasPat[np] = StrToDouble(sgridFalhasPat->Cells[1][np + 1] / 100.); //hk201808: reformula: calcIndCont->DistFalhasPat[np] = StrToDouble(sgridFalhasPat->Cells[1][np + 1] / 100.);

   //hkreformula
	//calcIndCont->RedistribuiFalhasHora(); //hk201808: reformulação (passa a propriedade de paramConf)
   calcIndCont->ParamConf->RedistribuiFalhasHora(numPat);

	// Atualiza flagSocAuto
	// calcIndCont->FlagAutoSoc = cbRecAuto->Checked;
	// Atualiza flagRecurso
	calcIndCont->ParamConf->ParamGerais->flagRecurso = cbRecurso->Checked;//hk201808: reformula
	// Atualiza flagRetorno
	calcIndCont->ParamConf->ParamGerais->flagRetorno = cbRetorno->Checked;//hk201808: reformula

	// Atualiza tempos

	calcIndCont->ParamConf->ParamGerais->tlocaliza = StrToDouble(edtTLoc->Text);      //hk201808: reformula
	calcIndCont->ParamConf->ParamGerais->tman_auto = StrToDouble(edtTManAuto->Text);  //hk201808: reformula
	calcIndCont->ParamConf->ParamGerais->tman_manual = StrToDouble(edtTMan->Text);    //hk201808: reformula
	calcIndCont->ParamConf->ParamGerais->trep = StrToDouble(edtTRep->Text);           //hk201808: reformula
	calcIndCont->ParamConf->ParamGerais->lambda = StrToDouble(edtLambda->Text);       //hk201808: reformula
	calcIndCont->ParamConf->ParamGerais->pu_lim = StrToDouble(edtPuLim->Text);        //hk201808: reformula
	calcIndCont->ParamConf->ParamGerais->percDefTrans = StrToDouble(edtPercDefTrans->Text) / 100.0;   //hk201808: reformula
	calcIndCont->IProfReliga = rgZonaReliga->ItemIndex;//hk201808: reformula: calcIndCont->IProfReliga = rgZonaReliga->ItemIndex; // hkedp
	//gh201806: acréscimo de fator de redução de tempo de preparo, devido à instalação de IF em fusível
	calcIndCont->ParamConf->ParamGerais->red_tprep_IF =  StrToDouble(edtRedTprepIF->Text) / 100.0;    //hk201808: reformula
   calcIndCont->ParamConf->EqptoIndispPadrao->vindisp = StrToDouble(edtIndispPadrao->Text);

	for (int n = 1; n < sgridParamRede->RowCount; n++)
	{
		rede = (VTRede*)sgridParamRede->Objects[0][n];
		if ((paramRede = calcIndCont->ParamConf->ParamRede[rede]) == NULL)
			continue;
		paramRede->lambda = StrToDouble(sgridParamRede->Cells[1][n]);
		paramRede->trep = StrToDouble(sgridParamRede->Cells[2][n]);
	}
	strDadoParamBloco *paramBloco;
	VTBloco *bloco;
	double lambda, trep;
	for (int n = 1; n < sgridParamBloco->RowCount; n++)
	{
		bloco = (VTBloco*)sgridParamBloco->Objects[0][n];
		paramBloco = calcIndCont->ParamConf->ParamBloco[bloco];
		lambda = StrToDouble(sgridParamBloco->Cells[3][n]);
		trep = StrToDouble(sgridParamBloco->Cells[4][n]);
		if (paramBloco != NULL)
		{
			if (!IsDoubleZero((paramBloco->lambda - lambda), 1e-2))
			{
				paramBloco->lambda = lambda;
				// paramBloco->nFalhas = lambda * paramBloco->compBloco;
			}
			if (!IsDoubleZero((paramBloco->trep - trep), 1e-2))
			{
				paramBloco->trep = trep;
				// paramBloco->duraTot = trep * paramBloco->nFalhas;
			}
		}
		else
		{
			if ((IsDoubleZero(lambda), 1e-2) || (IsDoubleZero(trep), 1e-2))
				continue;
			paramBloco = new strDadoParamBloco;
			paramBloco->fatTMA = 1.0;
			// hk201703: adaptações para planTurmas: acrescimo de fatos tma por bloco
			paramBloco->bloco = bloco;
			paramBloco->lambda = lambda;
			paramBloco->trep = trep;
			paramBloco->compBloco = calcIndCont->ParamConf->LisParamBloco->Add(paramBloco);
		}
	}
	// Atualiza listView
	LvParamRedeInicia();
	LvParamBlocoInicia();

	// buscaProt->LisTipoEqProt
	calcIndCont->LisTipoEqProt->Clear();//hk201808: reformula: calcIndCont->LisTipoEqProt->Clear();
	for (int nc = 0; nc < chkLstTipoProt->Count; nc++)
	{
		if (!chkLstTipoProt->Checked[nc])
			continue;
      calcIndCont->LisTipoEqProt->Add(chkLstTipoProt->Items->Objects[nc]);//hk201808: reformula: calcIndCont->LisTipoEqProt->Add(chkLstTipoProt->Items->Objects[nc]);
	}
	////reinicia paramConf->LisTipoEqProt = f(buscaProt->LisTipoEqProt = calcIndCont->LisTipoEqProt)
	calcIndCont->ReiniciaTipoEqProt();

	// Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
	// lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
	// Atualiza taxas (por blocos-->por circuito-->valor geral
	// pdef->TaxaFalha         = paramBloco->lambda;
	// pdef->TReparo           = paramBloco->trep;
	// --->pdef->TaxaFalha     = paramRede->lambda;
	// --->pdef->TReparo       = paramRede->trep;
	// --->--->pdef->TaxaFalha = paramDefault->lambda;
	// --->--->pdef->TReparo   = paramDefault->trep;
	calcIndCont->IniciaParamDefeito();
	// Inicia % falhas por patamar
	SgridFalhasPatInicia();
	SgridParamRedeBlocoInicia();
	// hk201702: Possibilidade de edição de parâmetros por bloco: SgridParamRedeInicia para SgridParamRedeBlocoInicia

	TVChavesInicia(); // hk1408
}

// ---------------------------------------------------------------------------

void __fastcall TFormCalcIndCont::Button3Click(TObject *Sender)
{
	/*

	 bdado = (VTBDado_Externo*)apl->GetObject(__classid(VTBDado_Externo));

	 //-----------------------------------------------------------------
	 ////cria objeto bdado
	 //bdado = NewObjBDado(apl);
	 ////Cria objeto para cálculo de indicadores de continuidade
	 //CriaObjetoCalculoIndicadores();
	 //bdado->LeDadoBanco();
	 ////atualiza flag_rede_carregada
	 //flag_rede_carregada = flag; default = true
	 ////Leitura do banco de dados
	 //if (! PopulaDado()) return(false);
	 //LeOcorrencia();
	 ////lê dados de ocorrência
	 //bdado->LeDadoOcorrencia(opcao);
	 ////monta dados de ocorrência a partir das opções escolhidas
	 //MontaDadoOcorrencia(ConsultaOcorrencia(opcao));
	 //ConsultaOcorrencia(opcao)
	 //MontaDadoOcorrencia(TADOQuery *tdb) SAÍDA-->lisOCORR
	 //IniciaBusca(void): lisOCORR_f->Assign(lisOCORR, laCopy);
	 if(bdado == NULL)
	 apl->Add(bdado = DLL_NewObjBDado_Externo(apl)); //preenche lisOCORR_f com todas as ocorrencias do banco
	 //-----------------------------------------------------------------
	 //---------------------------Geração de parêmetros
	 //calcIndCont->LeArquivos(arqPath);
	 //geraLisParamEqpto->GeraLisParamEqpto(lisOc = lisOCORR_f, lisParamEqpto); -->SAÍDA lisParamEqpto
	 //calcIndCont->ReiniciaLisParamEqptoExterno(lisParamEqpto)
	 //GeraLisParamRede() -->Saída lisParamRede com parametros de rede zerados
	 //calcIndCont->ReiniciaLisParamRedeExterno(lisParamRede);
	 //Reinicia distribuicao de falhas
	 //for(int np = 0; np < 24; np++)
	 //calcIndCont->DistFalhasPat[np] = geraLisParamEqpto->Falhas[np];
	 //calcIndCont->RedistribuiFalhasHora();
	 //---------------------------Ajustes de taxas e tempos de reparo
	 //Inicia resultados de continuidade
	 //resCont->resGeral e resCont->lisResRede->resRede e resCont->lisResBloco->resBloco
	 //calcIndCont->ResCont->IniciaResultados();
	 //calcIndCont->IniciaResultado();
	 //Executa dos parametros por subestacao
	 //calcIndCont->ExecutaAjusteLisParamVerif();
	 //ExecutaCompensacoes();
	 //Atualiza parConfRec.txt
	 //calcIndCont->GravaArqParam("");
	 IniciaParamOcorrencias();
	 }

	 //---------------------------------------------------------------------------
	 void __fastcall TFormCalcIndCont::IniciaParamOcorrencias(void)
	 {
	 TList *lisOc, *lisEqptoOc, *lisCircuitos;
	 VTOcorr   *ocorr, *ocorrAux;
	 VTEqptoOc *eqptoOc;
	 TList     *lisOcorr;
	 double diaMin, diaMax, dia;
	 int    ndiasObs;
	 TDateTime data_Interrupcao;
	 int    icontaNEnc, indBarraOc;
	 double distFalhas[24];
	 unsigned short hour, min, sec, msec;


	 //hktmp	TList *lisAux = new TList();
	 //	sdi_FILTRO_OC sdi;
	 //	bdado->ListaDadoEqptoOc(lisAux, sdi);
	 //	delete lisAux;



	 //	MONTAR FUNCAO EM CALCINDCONT QUE ENTRA COM LISOC E NOME DA EMPRESA
	 //	PARA ACESSAR PACONFREC.TXT E SAI COM:
	 //	LISPARAMEQPTO

	 //	MAICON RODA ESTA FUNCAO E MANDA LISPARAMEQPTO
	 //	MAICON RODA ReiniciaLisParamEqptoExterno E GERA LISPARAMBLOCO

	 //ETAPA 1: PARAMOCORRENCIAS: A PARTIR DE BDADO CRIA LISTA DE OCORRENCIAS
	 lisOc    = new TList();
	 lisEqptoOc = new TList();
	 lisCircuitos = new TList();
	 bdado->ListaEqptoOcorrencia(lisEqptoOc);
	 bdado->ListaOcorrencia(lisOc);
	 bdado->ListaCircuitos(lisCircuitos);

	 for(int i = lisOc->Count; i > 0; i--)
	 {
	 VTOcorr* ocorr = (VTOcorr*)lisOc->Items[i-1];
	 if(ocorr->DH_Interrupcao < DateTimePickerInicio->Date)
	 {
	 lisOc->Remove(ocorr);
	 continue;
	 }
	 if(ocorr->DH_Interrupcao > DateTimePickerTermino->Date)
	 {
	 lisOc->Remove(ocorr);
	 continue;
	 }
	 }

	 VTGeraLisParamEqpto *geraLisParamEqpto;
	 geraLisParamEqpto = DLL_NewObjGeraLisParamEqpto(apl);
	 geraLisParamEqpto->GeraLisParamEqptoBdado(lisOc, lisEqptoOc, lisParamEqpto);
	 geraLisParamEqpto->PreparaLisParamEqptoRede(lisParamEqpto, calcIndCont->DistFalhas);

	 //ETAPA 2: PARAMOCORRENCIAS: INICIA DADOS DE CONFIABILIDADE:
	 //PARAMETROS BÁSICOS E VALORES VERIFICADOS POR CONJUNTO
	 //ImprimeOcorrencias(lisOc);
	 //--	AnsiString arqPath;
	 //--	arqPath = path->DirDat() + "\\EDP\\";
	 //--	calcIndCont->LeArquivos(arqPath);
	 //Le dados de valores verificados de dec e fec por conjunto
	 //LeArqVerifCjto(arqPath); //preenche lisMetaCirc

	 //Reincia parametros gerais
	 calcIndCont->Lambda = 0.0;

	 //Reinicia parametros por equipamento
	 LimpaTList(calcIndCont->LisParamEqpto);
	 //Cria copia em lista de calcIndCont->lisParamEqpto (preenche calcIndCont->lisParamEqpto->paramEpto->paramOrig)
	 calcIndCont->ReiniciaLisParamEqptoExterno(lisParamEqpto);
	 //Reinicia parametros de rede
	 //Gera lisParamRede com valores zerados
	 GeraLisParamRede();
	 LimpaTList(calcIndCont->LisParamRede);
	 calcIndCont->ReiniciaLisParamRedeExterno(lisParamRede);
	 //Reinicia distribuicao de falhas
	 //for(int np = 0; np < 24; np++)
	 //	calcIndCont->DistFalhasPat[np] = geraLisParamEqpto->Falhas[np];
	 calcIndCont->RedistribuiFalhasHora();

	 //Inicia resultados de continuidade
	 //resCont->resGeral e resCont->lisResRede->resRede e resCont->lisResBloco->resBloco
	 calcIndCont->ResCont->IniciaResultados();
	 //calcIndCont->IniciaResultado();

	 //Executa dos parametros por subestacao
	 //VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
	 //AnsiString arq;
	 //arq = path->DirTmp() + "\\parsimulaCarr.txt";
	 //calcIndCont->ParamConf->ImprimeParametrosSimula(arq);
	 calcIndCont->ExecutaAjusteLisParamVerif();    //calcIndCont->ExecutaAjusteParametrosLisRede
	 //calcIndCont->Executa();
	 double daux = calcIndCont->ResGeral->tdeslN /60.;
	 //arq = path->DirTmp() + "\\defeitosCarr.txt";
	 //calcIndCont->ImprimeParametrosDefeito(arq);

	 ExecutaCompensacoes();

	 //Reinicia grid com parâmetros
	 SgridParamRedeInicia();
	 //Lista os resultados calculados
	 LVResultInicia();
	 //Reinicia lista de valores por bloco e circuito
	 //Atualiza listView
	 LvParamRedeInicia();
	 LvParamBlocoInicia();

	 //Atualiza parConfRec.txt
	 calcIndCont->GravaArqParam("");

	 delete lisOc;
	 delete lisEqptoOc;
	 delete geraLisParamEqpto;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::LeArqVerifCjto(AnsiString codPath)
{
	/* hkpc AnsiString   arqParam, codSe, aux;
	 FILE         *fmed;
	 strVerifCjto *verifSe;
	 bool         flagEOF = false;

	 arqParam = codPath + "ArqVerifCjto.dat";

	 LimpaTList(lisVerifCjto);

	 if(!FileExists(arqParam))
	 return;

	 fmed = fopen(arqParam.c_str(), "r");
	 //Le cabeçalho
	 LeDado(fmed, flagEOF);
	 LeDado(fmed, flagEOF);
	 LeDado(fmed, flagEOF);
	 LeDado(fmed, flagEOF);

	 codSe = LeDado(fmed, flagEOF);//cjto/se
	 while(!flagEOF)
	 {
	 lisVerifCjto->Add(verifSe = new strVerifCjto);
	 verifSe->codSe = codSe.Trim();
	 aux = LeDado(fmed, flagEOF);
	 verifSe->nomeSe = aux.Trim();
	 aux = LeDado(fmed, flagEOF);
	 verifSe->decVerif = StrToDouble(aux);
	 aux = LeDado(fmed, flagEOF);
	 verifSe->fecVerif = StrToDouble(aux);
	 codSe = LeDado(fmed, flagEOF);//cjto/se
	 }

	 fclose(fmed);
	 hkpc */
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ExecutaAjusteSE2(void)
{
	/* hkpc VTRedes  *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	 VTRadial *radial = DLL_NewObjRadial(apl);
	 radial->Inicia(redes);
	 TList *lisSE = radial->LisSubestacao();

	 bool flagCalcGeral = false;

	 double decVerif, fecVerif, decMeta, fecMeta;
	 TList *lisAux = new TList;

	 for(int nv = 0; nv < lisVerifCjto->Count; nv++)
	 {
	 strVerifCjto *verifCjto = (strVerifCjto*)lisVerifCjto->Items[nv];
	 VTSubestacao *se = NULL;
	 for(int nse = 0; nse < lisSE->Count; nse++)
	 {
	 se = (VTSubestacao*)lisSE->Items[nse];
	 if(se->Rede->Codigo.AnsiCompare(verifCjto->codSe) == 0)
	 break;
	 se = NULL;
	 }
	 if(se == NULL)
	 {
	 flagCalcGeral = true;
	 continue;
	 }

	 TList* lisPrimario = se->LisPrimario();
	 lisAux->Clear();
	 for(int npr = 0; npr < lisPrimario->Count; npr++)
	 {
	 VTPrimario *primario = (VTPrimario*)lisPrimario->Items[npr];
	 lisAux->Add(primario->Rede);
	 }

	 decVerif = verifCjto->decVerif;
	 fecVerif = verifCjto->fecVerif;

	 calcIndCont->ExecutaAjusteParametrosLisRede(lisAux, decVerif, fecVerif, decMeta, fecMeta);
	 }

	 //Caso existam subestacoes sem valores verificados realiza o cálculo geral
	 if(flagCalcGeral)
	 calcIndCont->Executa();

	 delete radial;
	 delete lisAux;
	 hkpc
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::ExecutaCompensacoes(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede = lisRedeOrdenada; // hk1113 redes->LisRede();
	strResRede *resRede, *resGeral;

	resGeral = calcIndCont->ResCont->ResGeral;//hk201808: reformula: calcIndCont->ResGeral;
	resGeral->compensacao = 0.0;
	resGeral->dicVIP = 0.0;
	resGeral->ficVIP = 0.0;
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];
		if (!rede->Carregada)
			continue;
		calcIndCont->ExecutaCompensacoes(rede); // , "EDP");
		// Busca parametros de rede

		resRede = calcIndCont->ResCont->ResRede[rede]; //hk201808: reformula: calcIndCont->ResRede[rede];
		resGeral->compensacao += resRede->compensacao;
		resGeral->dicVIP += resRede->dicVIP;
		resGeral->ficVIP += resRede->ficVIP;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormCalcIndCont::LeDado(FILE* fp, bool &flagEOF)
{
	AnsiString x;
	char aux;

	flagEOF = false;

	x = "";
	aux = fgetc(fp);
	while ((aux != ';') && (aux != '\n') && (aux != EOF))
	{
		x += AnsiString(aux);
		aux = fgetc(fp);
	}
	if (aux == EOF)
		flagEOF = true;

	return (x);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvParamRedeColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvParamRede->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvParamRedeCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	double d1, d2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // Taxa (f/km/ano)
	case 2: // Reparo(min)
	case 3: // Falhas(f/ano)
	case 4: // Comprimento(km)
	case 5: // Duração(min)
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvParamBlocoColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvParamBloco->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvParamBlocoCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	double d1, d2;
	int i1, i2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Bloco //hk201702: ordenação comum de blocos:
		i1 = StrToInt(Item1->SubItems->Strings[index]);
		i2 = StrToInt(Item2->SubItems->Strings[index]);
		if (i1 < i2)
			Compare = -1;
		else if (i1 > i2)
			Compare = 1;
		else
			Compare = 0;
		break;
	case 1: // Equipamento
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 2: // Rede
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		break;
	case 3: // Taxa (f/km/ano)
	case 4: // Reparo(min)
	case 5: // Falhas(f/ano)
	case 6: // Comprimento(km)
	case 7: // Duração(min)
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultRedeColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvResultRede->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultRedeCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	int i1, i2;
	double d1, d2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // Bloco
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		break;
	case 4: // Consumidores
		i1 = StrToInt(Item1->SubItems->Strings[index]);
		i2 = StrToInt(Item2->SubItems->Strings[index]);
		if (i1 < i2)
			Compare = -1;
		else if (i1 > i2)
			Compare = 1;
		else
			Compare = 0;
		break;
	case 2: // CLIh (deslig.h/ano)
	case 3: // CLI  (deslig/ano)
	case 5: // DEC (deslig.h/ano.cons.)
	case 6: // FEC (deslig/ano.cons.)
	case 7: // END (MWh/ano)
	case 8: // DECBT (deslig.h/ano.cons.)
	case 9: // FECBT (deslh/ano.cons.)
	case 10: // ENDBT (MWh/ano)
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvResult->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	int i1, i2;
	double d1, d2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // Bloco
	case 4: // Consumidores
		i1 = StrToInt(Item1->SubItems->Strings[index]);
		i2 = StrToInt(Item2->SubItems->Strings[index]);
		if (i1 < i2)
			Compare = -1;
		else if (i1 > i2)
			Compare = 1;
		else
			Compare = 0;
		break;
	case 2: // CLIh (deslig.h/ano)
	case 3: // CLI  (deslig/ano)
	case 5: // DEC (deslig.h/ano.cons.)
	case 6: // FEC (deslig/ano.cons.)
	case 7: // END (MWh/ano)
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultDefeitoColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvResultDefeito->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvResultDefeitoCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	int i1, i2;
	double d1, d2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // Chave Prot.
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		break;
	case 2: // Bloco
	case 7: // ManobrasAuto
	case 8: // ManobrasManuais
		// i1 = StrToInt(Item1->SubItems->Strings[index]);
		// i2 = StrToInt(Item2->SubItems->Strings[index]);
		// if(i1 < i2) Compare = -1;
		// else if(i1 > i2) Compare = 1;
		// else Compare = 0;
		// break; hkedp manManuais e manAuto, int->double

	case 3: // Comprimento(km)
	case 4: // Falhas(x1000)
	case 5: // Cli (deslig./ano)
	case 6: // Clih (deslig.h/ano)
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvCompColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvComp->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvCompCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
	int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	double d1, d2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // Compensação(R$/Ano)
	case 2: // Deslig.h/Trafo Vip
	case 3: // Deslig/Trafo Vip
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvIndicIndivColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (listViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		listViewSort.OrdemCrescente = !listViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		listViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	lvIndicIndiv->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::lvIndicIndivCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index = listViewSort.ColumnIndex - 1;
	int i1, i2;
	double d1, d2;

	switch (listViewSort.ColumnIndex)
	{
	case 0: // Rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // Carga
	case 2: // VIP
	case 3: // MT
	case 4: // Rural
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		break;
	case 9: // Consumidores
		i1 = StrToInt(Item1->SubItems->Strings[index]);
		i2 = StrToInt(Item2->SubItems->Strings[index]);
		if (i1 < i2)
			Compare = -1;
		else if (i1 > i2)
			Compare = 1;
		else
			Compare = 0;
		break;
	case 5: // DIC  (deslig.h/ano)
	case 6: // FIC  (deslig/ano)
	case 7: // DICBT  (deslig.h/ano)
	case 8: // FICBT  (deslig/ano)
	case 10: // Compensação
		d1 = StrToDouble(Item1->SubItems->Strings[index]);
		d2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (d1 < d2)
			Compare = -1;
		else if (d1 > d2)
			Compare = 1;
		else
			Compare = 0;
		break;
	default:
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!listViewSort.OrdemCrescente)
		Compare = -Compare;
}

/*
// ---------------------------------------------------------------------------
bool __fastcall TFormCalcIndCont::TrocaRedeEqvBTporCargaEqv(VTApl* apl)
	// hk201612: acréscimo de função JCG
{
	// variáveis locais
	VTStkEqv *stk_eqv;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	int buttons = 1;

	try
	{ // verifica se há redes BT equivalentes
		if (ExisteRedeEqvBT())
		{
			// Aviso("As redes BT (equivalentes ou não) serão substituídas \n"
			// "por cargas equivalentes");
			Hide();
			if (Confirma("As redes de BT serão equivalentadas.", "Deseja continuar?",
				buttons) != idYes)
			{
				return false;
			}
			Show();
			// if (Confirma("As redes BT (equivalentes ou não) serão substituídas por cargas equivalentes",
			// "Confirma esta operação ?") == idNo) {
			// Close();
			// return;
			 //}
			if (progresso == NULL)
			{ // cria progresso e insere no apl //DVK 2016.11.28
				progresso = DLL_NewObjProgresso(this->Owner, Parent);
				apl->Add(progresso);
			}
			// inicia progresso
			if (progresso)
				progresso->Start(progDEFAULT);
			// cria objeto VTStkEqv
			if ((stk_eqv = DLL_NewObjStkEqv(apl)) != NULL)
			{
				stk_eqv->Executa();
				// destrói objeto VTStkEqv
				delete stk_eqv;
			}
			// finaliza progresso
			if (progresso)
				progresso->Stop();
		}
	}
	catch (Exception &e)
	{
		if (progresso)
			progresso->Stop();
		Aviso("Não foi possível substituir as redes BT equivalentes por cargas");
		return false;
	}

	return true;
}
*/

// ---------------------------------------------------------------------------
void __fastcall TFormCalcIndCont::sgridAjusteKeyPress(TObject *Sender, System::WideChar &Key)
{
	// set which columns you want to not be edited by the user but still can select any cell in the column
	// we'l set it to column 4
	if (sgridAjuste->Col == 3 || sgridAjuste->Col == 4)
	{
		Key = 0;
	}

	if (Key == 2 && sgridAjuste->EditorMode)
	{ // key=2 same as VK_RETURN for the OnKeyDown event

		int SetRow = sgridAjuste->Row + 1;

		if (sgridAjuste->RowCount > SetRow)
		{
			sgridAjuste->EditorMode = true;
			sgridAjuste->Row = SetRow;
		}
	}
}

// ---------------------------------------------------------------------------


void __fastcall TFormCalcIndCont::ActionLeIFExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString arq = "";

	// reinicia
	OpenDialog->FileName = arq;
	// mostra a janela de salvar acima das outras
	Application->ModalPopupMode = pmAuto;
	// proteção:
	if (calcIndCont == NULL)
		return;
	// configura OpenFileDialog
	OpenDialog->Title = "Selecionar arquivo de localização de IFs";
	OpenDialog->Filter = "Arquivos CSV (*.csv)|*.csv";
	// exibe OpenTextFileDialog
	if (OpenDialog->Execute())
	{ // valida seleção
		if (OpenDialog->Files->Count == 0)
			return;
		// identifica caminho do arquivo
		arq = OpenDialog->FileName;
		// executa leitura
		try
		{
			calcIndCont->InsereChavesIF(arq);
			LbChavesIFInicia();
			ActionPreparaIndicacaoFaltaExecute(NULL);
		}
		catch (Exception &e)
		{ // nada a fazer
		}
	}
}
//---------------------------------------------------------------------------



