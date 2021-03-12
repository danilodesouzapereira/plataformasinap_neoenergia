// -----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\BD\VTBD.h>
#include <PlataformaSinap\Fontes\BD\Rede\VTBDeqpto.h>
#include <PlataformaSinap\Fontes\Classes\VTClasse.h>
#include <PlataformaSinap\Fontes\Classes\VTClasses.h>
#include <PlataformaSinap\Fontes\Constante\Const.h>
#include <PlataformaSinap\Fontes\Curvas\VTCurva.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Planejamento\VTEstudo.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTSecundario.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTMRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTReducao.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Topologia\VTTopologia.h>
#include <PlataformaSinap\DLL_Inc\BD.h>
#include <PlataformaSinap\DLL_Inc\Curvas.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Planejamento.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Rede.h>
#include "TStkEqv.h"

// -----------------------------------------------------------------------------
VTStkEqv* NewObjStkEqv(VTApl *apl_owner)
{
	try
	{
		return (new TStkEqv(apl_owner));
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// erro na criação do objeto
	return (NULL);
}

// -----------------------------------------------------------------------------
__fastcall TStkEqv::TStkEqv(VTApl *apl_owner)
{
	// cria objeto Apl local
	apl = NewObjApl(NULL, apl_owner); ;
	// cria objetos
	apl->Add(local.redes = DLL_NewObjRedes(apl));
	apl->Add(local.estudo = DLL_NewObjEstudo(apl));
	// atenção: o objeto VTBD precisa ser criado depois do objeto VTRedes
	apl->Add(local.bd = DLL_NewObjBD(apl));
	local.estudo->Id = 0;
	// cria objetos
	lisEQP = new TList();
	StrList = new TStringList();
}

// -----------------------------------------------------------------------------
__fastcall TStkEqv::~TStkEqv(void)
{
	// destrói objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	if (StrList)
	{
		delete StrList;
		StrList = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// -----------------------------------------------------------------------------
VTCarga* __fastcall TStkEqv::CriaCargaEqv(AnsiString codigo)
{
	// variáveis locais
	VTCarga *carga;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// preenche dados básicos da carga
	carga = DLL_NewObjCarga(patamares->NumPatamar());
	carga->Id = FalsoId();
	carga->Status[sttNOVO] = true;
	carga->Show[eqp_VISIBLE] = true;
	carga->Codigo = codigo;
	carga->Fases = 0;
	carga->esq.posbar = 50.;
	carga->esq.dxbar = 4000.;
	carga->esq.dybar = 0.;
	return (carga);
}

// -----------------------------------------------------------------------------
void __fastcall TStkEqv::DefineConsumoCarga(VTCarga *carga_eqv, TList *lisCARGA)
{
	// variáveis locais
	VTCarga *carga;
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int num_pat = patamares->NumPatamar();

	// proteção
	if (carga_eqv == NULL)
		return;
	// cria curva
	carga_eqv->curva = DLL_NewObjCurva();
	carga_eqv->curva->Id = FalsoId();
	carga_eqv->curva->Tipica = false;
	carga_eqv->curva->Tipo = curvaPQ;
	carga_eqv->curva->Unidade = unidMVA;
	carga_eqv->curva->NumeroValores = nvCURVA_PQ;
	carga_eqv->curva->DiaMedicao = SEMANA_COMPLETA;
	carga_eqv->classe = classes->ExisteClasseTag(consEQUIVALENTE);
	carga_eqv->curva->Reconfigura(num_pat);
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*)lisCARGA->Items[nc];
		// carga_eqv->Fases |= carga->Fases;  //tem que arrumar isso
		carga->curva->Reconfigura(num_pat);
		for (int np = 0; np < num_pat; np++)
		{ // atualiza demanda trifásica da carga equivalente
			demanda->P_mw[carga_eqv][np] += demanda->P_mw[carga][np];
			demanda->Q_mvar[carga_eqv][np] += demanda->Q_mvar[carga][np];
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TStkEqv::DefineNConsCarga(VTCarga *carga_eqv, TList *lisCARGA)
{
	// variáveis locais
	VTCarga *carga;

	// proteção
	if (carga == NULL)
		return;
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*)lisCARGA->Items[nc];
		// define quantidade de consumidores da carga
		switch (carga->classe->Tag)
		{
		case consRESIDENCIAL:
			carga_eqv->NumConsResidencial++;
			break;
		case consCOMERCIAL:
			carga_eqv->NumConsComercial++;
			break;
		case consINDUSTRIAL:
			carga_eqv->NumConsIndustrial++;
			break;
		case consRURAL:
			carga_eqv->NumConsRural++;
			break;
		case consOUTROS:
			carga_eqv->NumConsOutros++;
			break;
		case consA4:
			carga_eqv->NumConsA4++;
			break;
		default:
			break;
		}
	}
}

// ---------------------------------------------------------------------------
int __fastcall TStkEqv::DeterminaFasePriTrafos(TList *lisEQP)
{
	// variaveis locais
	int fase_tag = 0;
	VTTrafo *trafo;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// verifica se a lista está vazia e retorna default ABC
	if (lisEQP->Count == 0)
		return (faseABC);
	for (int nt = 0; nt < lisEQP->Count; nt++)
	{ // acumula fases do primário dos trafos
		trafo = (VTTrafo*)lisEQP->Items[nt];
		fase_tag |= trafo->pri.fase;
	}
	// verifica se tem só 1 fase
	if (fases->NumeroDeFases(fase_tag) == 1)
	{ // insere faseT se não tiver neutro
		if ((fase_tag & faseN) != faseN)
			fase_tag |= faseT;
		// elimina faseN se o neutro estiver aterrado
		if ((fase_tag & faseNT) == faseNT)
			fase_tag = fase_tag & faseABCT;
	}
	return (fase_tag);
}

// ---------------------------------------------------------------------------
int __fastcall TStkEqv::DeterminaFaseSecTrafos(TList *lisEQP)
{
	// variaveis locais
	int fase_tag = 0;
	VTTrafo *trafo;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// verifica se a lista está vazia e retorna default ABC
	if (lisEQP->Count == 0)
		return (faseABC);
	for (int nt = 0; nt < lisEQP->Count; nt++)
	{ // acumula fases do secundário dos trafos
		trafo = (VTTrafo*)lisEQP->Items[nt];
		fase_tag |= trafo->sec.fase;
	}
	// verifica se tem só 1 fase
	if (fases->NumeroDeFases(fase_tag) == 1)
	{ // insere faseT se não tiver neutro
		if ((fase_tag & faseN) != faseN)
			fase_tag |= faseT;
		// elimina faseN se o neutro estiver aterrado
		if ((fase_tag & faseNT) == faseNT)
			fase_tag = fase_tag & faseABCT;
	}
	// verifica se tem 3 fases
	if (fases->NumeroDeFases(fase_tag) == 3)
	{ // retira faseT se neutro estiver aterrado
		if ((fase_tag & faseNT) == faseNT)
			fase_tag = faseABCN;
	}
	return (fase_tag);
}

// ---------------------------------------------------------------------------
void __fastcall TStkEqv::EliminaEqptos(VTRede *rede)
{
	// variaveis locais
	VTBarra *barra;
	VTEqpto *eqpto;
	VTEqbar *eqbar;
	VTLigacao *ligacao;

	for (int ne = lisEQP->Count - 1; ne >= 0; ne--)
	{
		eqpto = (VTEqpto*)lisEQP->Items[ne];
		switch (eqpto->Tipo())
		{
		case eqptoTRECHO:
			ligacao = (VTLigacao*)eqpto;
			ligacao->rede->RemoveLigacao(ligacao);
			ligacao->Status[sttEXCLUIDO] = true;
			DLL_DeleteEqpto(eqpto);
			break;
		case eqptoCARGA:
			eqbar = (VTEqbar*)eqpto;
			eqbar->pbarra->RemoveEqbar(eqbar);
			eqbar->Status[sttEXCLUIDO] = true;
			DLL_DeleteEqpto(eqpto);
			break;
		case eqptoBARRA:
			barra = (VTBarra*)eqpto;
			// apenas remove da lista de barras da rede
			if (rede != NULL)
			{
				if (barra == rede->BarraInicial())
					continue;
				rede->RemoveBarra(barra);
			}
			// apaga barra se estiver isolada
			if (!barra->Status[sttISOLADO])
				continue;
			barra->Status[sttEXCLUIDO] = true;
			DLL_DeleteEqpto(eqpto);
			break;
		default:
			break;
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TStkEqv::EliminaRede(VTRede *rede)
{
	// variáveis locais
	VTRedes *redes;

	// IMPORTANTE: obtém objeto VTRedes do Apl recebido e não o objeto VTRedes criado no Apl local
	redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
	// insere na lista cada tipo de equipamento
	lisEQP->Clear();
	rede->LisEqpto(lisEQP, eqptoLIGACAO);
	EliminaEqptos(rede);
	// apaga rede
	redes->RemoveRede(rede);
	rede->Status[sttEXCLUIDO] = true;
	DLL_DeleteEqpto(rede);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
//https://sinapsisenergia.teamwork.com/#/tasks/18678361
bool __fastcall TStkEqv::Executa(bool a4NaBT)
{
	// variáveis locais
	VTBarra *barra;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
	TList *lisBAR_ISO;
	VTRedes *redes;

	// IMPORTANTE: obtém objeto VTRedes do Apl recebido e não o objeto VTRedes criado no Apl local
	redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));

	// cria objeto radial e executa
	radial = DLL_NewObjRadial(apl);
	if (!radial->Inicia(redes))
	{
		delete radial;
		radial = NULL;
		return (false);
	}
	// obtém lista de Redes originais
	LeListaRedesOriginais();
	// monta StringList com as redes originais ordenadas
	IniciaStrListOrdenadaRedesOriginais();
	// elimina Redes equivalentes
	RetiraRedeEquivalente();
	// elimina redes BT abertas //DVK 2016.11.28
	// RetiraRedeBT();
	RetiraRedeBTmenosTrafo(a4NaBT);
	// atualiza Topologia e Grafico
	if (topologia)
	{
		topologia->Reconfigura();
	}
//	if (grafico)
//	{
//		grafico->AreaRedeDefine();
//	}
	// grafico->Redraw();
	// elimina barras isoladas //DVK 2017.04.12
//	redes->ReiniciaLisBarraIsolada();
	lisBAR_ISO = redes->LisBarraIsolada();
	for (int nb = lisBAR_ISO->Count - 1; nb >= 0; nb--)
	{//remove somente barras que não possuam ligação para não dar problema no Topologia
		barra = (VTBarra*)lisBAR_ISO->Items[nb];
        if(barra->LisLigacao()->Count > 0)
            continue;
		redes->RemoveBarra(barra);
		DLL_DeleteEqpto(barra);
	}
	// atualiza de novo Topologia e Grafico
	if (topologia)
	{
		topologia->Reconfigura();
	}
	if (grafico)
	{
		grafico->AreaRedeDefine();
	}
	// destroi radial
	delete radial;
	radial = NULL;
	return (true);
}
/*
 //-----------------------------------------------------------------------------
 VTRede* __fastcall TStkEqv::ExisteRedeOriginal(AnsiString codigo)
 {
 //variáveis locais
 VTRede *rede;

 for (int nr = 0; nr < lisREDE->Count; nr++)
 {
 rede = (VTRede*)lisREDE->Items[nr];
 if (rede->Codigo.AnsiCompareIC(codigo) == 0)
 {return(rede);}
 }
 return(NULL);
 }
 */

// -----------------------------------------------------------------------------
VTRede* __fastcall TStkEqv::ExisteRedeOriginal(AnsiString codigo)
{
	// variáveis locais
	int result;
	VTRede *rede;

	// procura codigo em StrLst ordenado em ordem crescente
	for (int n = StrList->Count - 1; n >= 0; n--)
	{
		if ((result = codigo.AnsiCompareIC(StrList->Strings[n])) == 0)
		{ // determina Rede
			rede = (VTRede*)StrList->Objects[n];
			// retira item do StringList para otimizar próximas buscas
			StrList->Delete(n);
			return (rede);
		}
		else if (result > 0)
		{
			return (NULL);
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
void __fastcall TStkEqv::IniciaStrListOrdenadaRedesOriginais(void)
{
	// variáveis locais
	VTRede *rede;
	TList *lisREDE = local.redes->LisRede();

	// reinicia StrList
	StrList->Clear();
	StrList->Sorted = false;
	// loop p/ todas redes originais
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		StrList->AddObject(rede->Codigo, rede);
	}
	// ordena StrList em ordem crescente
	StrList->Sorted = true;
}

// -----------------------------------------------------------------------------
bool __fastcall TStkEqv::LeListaRedesOriginais(void)
{
	// variáveis locais
	VTTipoRede *tiporede;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));

	// obtém objeto TipoRede
	if ((tiporede = tipos->ExisteTipoRede(redeSEC)) == NULL)
		return (false);
	// conecta com a base da rede
	if (!local.bd->Conecta())
		return (false);
	// lê Redes do TipoRede
	local.bd->Eqpto->LeListaRede(local.estudo, tiporede);

	return (true);
}

// -----------------------------------------------------------------------------
int __fastcall TStkEqv::QtdeConsumidorRede(VTRede *rede)
{
	// variáveis locais
	int count = 0;

	// determina Rede original
	if ((rede = ExisteRedeOriginal(rede->Codigo)) != NULL)
	{ // determina quantidade de Cargas da Rede original
		count = local.bd->Eqpto->LeQtdeCargaRede(rede->Id);
	}
	return (count);
}
/*
 //-----------------------------------------------------------------------------
 int __fastcall TStkEqv::QtdeConsumidorRede(VTRede *rede)
 {
 return(0);
 }
 */

// -----------------------------------------------------------------------------
// aqui as redes BT se transformavam em cargas concentradas no primario do trafo
bool __fastcall TStkEqv::RetiraRedeBT(void)
{
	// variáveis locais
	int fase_tag;
	VTCarga *carga;
	VTRede *rede;
	VTRedes *redes;
	TList *lisREDE, *lisCARGA;

	try
	{ // IMPORTANTE: obtém objeto VTRedes do Apl recebido e não o objeto VTRedes criado no Apl local
		redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
		lisREDE = redes->LisRede();
		// cria lista
		lisCARGA = new TList();
		// loop para todas redes
		for (int nr = lisREDE->Count - 1; nr >= 0; nr--)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			if (!rede->Carregada)
				continue;
			if (rede->TipoRede->Segmento != redeSEC)
				continue;
			// obtém fases do primário dos trafos da rede
			lisEQP->Clear();
			rede->LisEqpto(lisEQP, eqptoTRAFO);
			fase_tag = DeterminaFasePriTrafos(lisEQP);
			// obtém lista de cargas da Rede
			lisCARGA->Clear();
			rede->LisEqpto(lisCARGA, eqptoCARGA);
			if ((lisCARGA->Count > 0) && (rede->BarraInicial() != NULL))
			{ // cria carga equivalente
				carga = CriaCargaEqv(rede->Codigo);
				if (carga == NULL)
					continue;
				// atualiza fases da carga
				carga->Fases = fase_tag;
				// insere Carga na barra inicial da rede
				rede->BarraInicial()->InsereEqbar(carga);
				// define número de consumidores
				DefineNConsCarga(carga, lisCARGA);
				// define consumo da carga equivalente
				DefineConsumoCarga(carga, lisCARGA);
				// elimina cargas
				lisEQP->Clear();
				CopiaTList(lisCARGA, lisEQP);
				EliminaEqptos(rede);
			}
			// elimina a Rede e seus equipamentos
			EliminaRede(rede);
		}
		// destroi lista
		delete lisCARGA;
	}
	catch (Exception &e)
	{
		if (lisCARGA)
			delete lisCARGA;
		if (carga)
			delete carga;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
// aqui as redes BT se transformam em cargas concentradas no secundario do trafo
//https://sinapsisenergia.teamwork.com/#tasks/16580785
bool __fastcall TStkEqv::RetiraRedeBTmenosTrafo(bool a4NaBT)
{
	// variáveis locais
	int fase_tag;
	VTBarra *barra, *barraPri;
	VTCarga *carga;
	VTRede *rede;
	VTRedes *redes;
	VTSecundario *secundario;
	VTTrafo *trafo;
	TList *lisREDE, *lisCARGA;
	TList *lisCARGAA4;

	try
	{ // IMPORTANTE: obtém objeto VTRedes do Apl recebido e não o objeto VTRedes criado no Apl local
		redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
		lisREDE = redes->LisRede();
		// cria lista
		lisCARGA = new TList();
		lisCARGAA4 = new TList();
		// loop para todas redes
		for (int nr = lisREDE->Count - 1; nr >= 0; nr--)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			if (!rede->Carregada)
				continue;
			if (rede->TipoRede->Segmento != redeSEC)
				continue;
			// obtém todos os trafos da rede
			lisEQP->Clear();
			rede->LisEqpto(lisEQP, eqptoTRAFO);
			if (lisEQP->Count == 0)
			{ // elimina a rede inteira, pois nao tem onde colocar a carga eqv.
				EliminaRede(rede);
				continue;
			}
			// obtém fases do secundário dos trafos da rede
			fase_tag = DeterminaFaseSecTrafos(lisEQP);
			// determina barra do secundario de um trafo
			trafo = (VTTrafo*)lisEQP->First();
			barra = trafo->BarraSecundario();
			barraPri = trafo->BarraPrimario();
			// obtém lista de cargas da Rede
			lisCARGA->Clear();
			lisCARGAA4->Clear();
			rede->LisEqpto(lisCARGA, eqptoCARGA);
			// trata cargas a4
			if (!a4NaBT)
			{
				for (int nc = (lisCARGA->Count - 1); nc >= 0; nc--)
				{
					carga = (VTCarga*) lisCARGA->Items[nc];
					if ((carga->pbarra != NULL) && (carga->pbarra->vnom >= 1.0))
					{
						carga->GrupoA = true;
						lisCARGAA4->Add(carga);
						lisCARGA->Remove(carga);
					}
				}
			}
			// move trafos para o primario
			secundario = radial->ExisteSecundario(rede);
			if ((secundario != NULL) && (secundario->Primario != NULL))
			{
				TransfereTrafosParaPrimario(lisEQP, secundario->Primario->Rede);
				// redefine barra inicial da rede
				rede->DefineBarraInicial(barra);
			}
			lisEQP->Clear();
			/* //as cargas A4 não serão equivalentadas
			if ((lisCARGAA4->Count > 0) && (barraPri != NULL))
			{ // cria carga equivalente
				carga = CriaCargaEqv("MT." + rede->Codigo);
				if (carga == NULL)
					continue;
				// atualiza fases da carga sem T
				carga->Fases = (fase_tag & faseABCN);
				// insere neutro (caso nao tenha)
				carga->Fases = (carga->Fases | faseN);
				// insere Carga na barra do primario da rede
				barraPri->InsereEqbar(carga);
				// define número de consumidores
				DefineNConsCarga(carga, lisCARGAA4);
				// define consumo da carga equivalente
				DefineConsumoCarga(carga, lisCARGAA4);
				// limpa lista que contem trafos, pois eles nao serao eliminados
				// elimina cargas
				CopiaTList(lisCARGAA4, lisEQP);
				// elimina trechos
				// rede->LisLigacao(lisEQP, eqptoTRECHO);
				// elimina
				// EliminaEqptos(rede);
			}
			*/
			if ((lisCARGA->Count > 0) && (barra != NULL))
			{ // cria carga equivalente
				carga = CriaCargaEqv(rede->Codigo);
				if (carga == NULL)
					continue;
				// atualiza fases da carga sem T
				carga->Fases = (fase_tag & faseABCN);
				// insere neutro (caso nao tenha)
				carga->Fases = (carga->Fases | faseN);
				// insere Carga na barra do secundario da rede
				barra->InsereEqbar(carga);
				// define número de consumidores
				DefineNConsCarga(carga, lisCARGA);
				// define consumo da carga equivalente
				DefineConsumoCarga(carga, lisCARGA);
				// elimina cargas
				CopiaTList(lisCARGA, lisEQP);
				// elimina trechos
				// rede->LisLigacao(lisEQP, eqptoTRECHO);
				// elimina
				EliminaEqptos(rede);
			}
			// elimina a Rede e seus eqptos
			EliminaRede(rede);
		}
		// destroi lista
		delete lisCARGA;
	}
	catch (Exception &e)
	{
		if (lisCARGA)
			delete lisCARGA;
		if (carga)
			delete carga;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TStkEqv::RetiraRedeEquivalente(void)
{
	// variáveis locais
	int num_cons;
	TList *lisEQBAR;
	TList *lisREDUCAO;
	VTCarga *carga;
	VTReducao *reducao;
	VTRede *rede;
	VTRedes *redes;
	TList *lisREDE;

	// IMPORTANTE: obtém objeto VTRedes do Apl recebido e não o objeto VTRedes criado no Apl local
	redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
	lisREDE = redes->LisRede();
	// loop para todas redes
	for (int nr = lisREDE->Count - 1; nr >= 0; nr--)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		if (rede->Carregada)
			continue;
		if (rede->TipoRede->Segmento != redeSEC)
			continue;
		// obtém Reducao da Rede
		lisREDUCAO = rede->MRede->LisReducao();
		if (lisREDUCAO->Count != 1)
			continue;
		reducao = (VTReducao*)lisREDUCAO->First();
		// obtém lista de Cargas da Reducao
		lisEQP->Clear();
		reducao->LisEqbar(lisEQP, eqptoCARGA);
		if (lisEQP->Count != 1)
			continue;
		// determina número de Cargas da rede
		num_cons = QtdeConsumidorRede(rede);
		// transfere a Carga da Reducao para a Barra
		reducao->LisEqbar()->Clear();
		carga = (VTCarga*)lisEQP->First();
		carga->Codigo = rede->Codigo;
		// insere Carga na sua Barra
		carga->pbarra->InsereEqbar(carga);
		// define número de consumidores
		carga->NumConsResidencial = 0;
		carga->NumConsIndustrial = 0;
		carga->NumConsComercial = 0;
		carga->NumConsRural = 0;
		carga->NumConsA4 = 0;
		carga->NumConsOutros = num_cons;
		// elimina a Rede
		(redes->LisMRede())->Remove(rede->MRede);
		redes->RemoveRede(rede);
		delete rede->MRede;
		delete rede;
	}
	return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TStkEqv::TransfereTrafosParaPrimario(TList *lisTRAFO, VTRede *rede_pri)
{
	// variáveis locais
	VTTrafo *trafo;

	// proteção
	if (rede_pri == NULL)
		return;

	for (int n = 0; n < lisTRAFO->Count; n++)
	{ // altera a rede de cada trafo para pertencer à rede primaria
		trafo = (VTTrafo*)lisTRAFO->Items[n];
		trafo->rede->RemoveLigacao(trafo);
		rede_pri->InsereLigacao(trafo);
	}
}

// ---------------------------------------------------------------------------
// eof
