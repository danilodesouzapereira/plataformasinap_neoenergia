// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "ConstantesPerda.h"
#include "TEnergiaPassantes.h"
#include "VTEnergiaPassante.h"
#include "VTFluxo.h"
#include "VTPerda.h"
#include "VTPerdas.h"
#include "VTBalanco.h"
#include "VTBalancos.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEnergiaPassantes* __fastcall NewObjEnergiaPassantes(VTApl *apl)
{
	try
	{
		return (new TEnergiaPassantes(apl));
	}
	catch (Exception &e)
	{
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TEnergiaPassantes::TEnergiaPassantes(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria listas
	fluxo = NewObjFluxo(apl);
	lisENE_PAS = new TList();
	lisTMP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TEnergiaPassantes::~TEnergiaPassantes(void)
{
	// destrói objetos
	if (fluxo)
	{
		delete fluxo;
		fluxo = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	// destrói lista e seus objetos
	if (lisENE_PAS)
	{
		LimpaTList(lisENE_PAS);
		delete lisENE_PAS;
		lisENE_PAS = NULL;
	}
	// if (lisBAL_EQV) {LimpaTList(lisBAL_EQV); delete lisBAL_EQV; lisBAL_EQV = NULL;}
	// if (lisHR) {LimpaTList(lisHR); delete lisHR; lisHR = NULL;}
}

// ---------------------------------------------------------------------------
int __fastcall TEnergiaPassantes::CategoriaTrafo(VTTrafo *trafo)
{
	VTBarra *barraPri, *barraSec;
	int categoriaPerdas = catptIND;
	int categoriaTrafo = 0;

	barraPri = trafo->BarraPrimario();
	barraSec = trafo->BarraSecundario();

	if ((barraPri == NULL) || (barraSec == NULL))
	{
		return catptIND;
	}
	//verifica o sentido
	if (barraPri->vnom > barraSec->vnom)
	{
		categoriaTrafo |= cattSENTIDODIRETO;
	}
	//verifica os niveis de tensão do primario e do secundario
	categoriaTrafo |= NivelTensao(barraPri);
	categoriaTrafo |= NivelTensao(barraSec);

	return categoriaTrafo;
}
// ---------------------------------------------------------------------------
int __fastcall TEnergiaPassantes::DeterminaCategoriaPT(int catTRAFO)
{
	int categoriaPT = catptIND;
	bool a1, a2, a3, a3a, a4, b, sentidoDireto;

	//inicializa
	a1 = a2 = a3 = a3a = a4 = b = sentidoDireto = false;
	//verifica a existencia do nivel de tensão
	if ((catTRAFO & cattSENTIDODIRETO) == cattSENTIDODIRETO)
		{sentidoDireto = true;}
	if ((catTRAFO & cattA1) == cattA1)
		{a1 = true;}
	if ((catTRAFO & cattA2) == cattA2)
		{a2 = true;}
	if ((catTRAFO & cattA3) == cattA3)
		{a3 = true;}
	if ((catTRAFO & cattA3a) == cattA3a)
		{a3a = true;}
	if ((catTRAFO & cattA4) == cattA4)
		{a4 = true;}
	if ((catTRAFO & cattB) == cattB)
		{b = true;}
	//classifica a categoria de acordo com os bool
	// A1 para A2
	if ((a1) && (a2) && (sentidoDireto))
		{categoriaPT = catptA1_A2;}
	// A1 para A3
	else if ((a1) && (a3) && (sentidoDireto))
		{categoriaPT = catptA1_A3;}
	// A1 para A3a
	else if ((a1) && (a3a) && (sentidoDireto))
		{categoriaPT = catptA1_A3A;}
	// A1 para A4
	else if ((a1) && (a4) && (sentidoDireto))
		{categoriaPT = catptA1_A4;}
	// A2 para A3
	else if ((a2) && (a3) && (sentidoDireto))
		{categoriaPT = catptA2_A3;}
	// A2 para A3a
	else if ((a2) && (a3a) && (sentidoDireto))
		{categoriaPT = catptA2_A3A;}
	// A2 para A4
	else if ((a2) && (a4) && (sentidoDireto))
		{categoriaPT = catptA2_A4;}
	// A3 voltando para A2
	else if ((a3) && (a2) && (!sentidoDireto))
		{categoriaPT = catptA3_A2;}
	// A3 para A3a
	else if ((a3) && (a3a) && (sentidoDireto))
		{categoriaPT = catptA3_A3A;}
	// A3 para A4
	else if ((a3) && (a4) && (sentidoDireto))
		{categoriaPT = catptA3_A4;}
	// A3a voltando para A2
	else if ((a3a) && (a2) && (!sentidoDireto))
		{categoriaPT = catptA3A_A2;}
	// A3a para A4
	else if ((a3a) && (a4) && (sentidoDireto))
		{categoriaPT = catptA3A_A4;}
	// A3a para B
	else if ((a3a) && (a4) && (sentidoDireto))
		{categoriaPT = catptA3A_B;}
	// A4 voltando para A2
	else if ((a4) && (a2) && (!sentidoDireto))
		{categoriaPT = catptA4_A2;}
	// A4 voltando para A3
	else if ((a4) && (a3) && (!sentidoDireto))
		{categoriaPT = catptA4_A3;}
	// A4 voltando para A3a
	else if ((a4) && (a3a) && (!sentidoDireto))
		{categoriaPT = catptA4_A3A;}
//	// A4 para A4 ????
//	else if ((a4) && (???) && (!sentidoDireto))
//		{categoriaPT = catptA4_A4;}
	// A4 para B
	else if ((a4) && (b) && (sentidoDireto))
		{categoriaPT = catptA4_B;}
	// B voltando para A3a
	else if ((b) && (a3a) && (!sentidoDireto))
		{categoriaPT = catptB_A3A;}
	// B voltando para A4
	else if ((b) && (a4) && (!sentidoDireto))
		{categoriaPT = catptB_A4;}
	else
		{categoriaPT = catptIND;}

	return categoriaPT;
}
// ---------------------------------------------------------------------------
bool __fastcall TEnergiaPassantes::Executa(void)
{
	// obtém ponteiros dos objetos
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	// reinicia lista de EnergiaPassantes
	Reinicia();
	// monta energiaPassante por Node da Arvore hierárquica das Redes
	if (!MontaEnergiaPassanteRede())
		return (false);
//	// redistribui erro de energia causado pelo fluxo de potência por rede
//	RedistribuiErroFluxoPorRede();
//	// RedistribuiErroFluxoPorRede();
//	// redistribui as perdas calculadas por fora do fluxo
//	RedistribuiPerdasPorFora();
	// execução sem erro
	return (true);
}

// ---------------------------------------------------------------------------
VTEnergiaPassante* __fastcall TEnergiaPassantes::ExisteEnergiaPassante(VTRede *rede)
{
	// variáveis locais
	VTEnergiaPassante *energiaPassante;
	VTNode *node;

	for (int n = 0; n < lisENE_PAS->Count; n++)
	{
		energiaPassante = (VTEnergiaPassante*)lisENE_PAS->Items[n];
		if (energiaPassante->Object == rede)
			return (energiaPassante);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TEnergiaPassantes::IniciaEnergiaPassanteRede(void)
{
	// variáveis locais
	VTEnergiaPassante *energiaPassante;
	VTRede *rede;
	TList *lisREDE = redes->LisRede();

	// loop p/ todas as Redes
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		// if (! rede->Carregada) continue;
		// cria objeto EnergiaPassante e insere em EnergiaPassantes
		if ((energiaPassante = NewObjEnergiaPassante()) == 0)
			return (false);
		energiaPassante->Object = rede;
		energiaPassante->Descricao = rede->Codigo;
		InsereEnergiaPassante(lisENE_PAS, energiaPassante);
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::IniciaEnergiaPassanteReducao(void)
 {
 //variáveis locais
 VTEnergiaPassante *energiaPassante;
 VTMRede   *mrede;
 TList     *lisMREDE = redes->LisMRede();

 //loop p/ todas as MRedes
 for (int n = 0; n < lisMREDE->Count; n++)
 {
 mrede = (VTMRede*)lisMREDE->Items[n];
 if (mrede->Carregada) continue;
 //cria objeto EnergiaPassante e insere em lisBAL_EQV
 if ((energiaPassante = NewObjEnergiaPassante(patamares->NumPatamar())) == 0) return(false);
 energiaPassante->Object    = (VTReducao*)((mrede->LisReducao())->First());
 energiaPassante->Descricao = mrede->Codigo;
 InsereEnergiaPassante(lisBAL_EQV, energiaPassante);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TEnergiaPassantes::InsereEnergiaPassante(TList *lisENEP,
	VTEnergiaPassante *energiaPassante)
{
	// verifica se a EnergiaPassante já existe na lista
	if (lisENEP->IndexOf(energiaPassante) < 0)
		lisENEP->Add(energiaPassante);
}
// ---------------------------------------------------------------------------
int __fastcall TEnergiaPassantes::InverteCategoriaPT(int enumTCATPT)
{
	int retorno = catptIND;

	switch (enumTCATPT)
	{
		// A2 - A3
		case catptA2_A3:
			retorno = catptA3_A2;
			break;
		case catptA3_A2:
			retorno = catptA2_A3;
			break;
		// A2 - A3a
		case catptA2_A3A:
			retorno = catptA3A_A2;
			break;
		case catptA3A_A2:
			retorno = catptA2_A3A;
			break;
		// A2 - A4
		case catptA2_A4:
			retorno = catptA4_A2;
			break;
		case catptA4_A2:
			retorno = catptA2_A4;
			break;
		// A3 - A4
		case catptA3_A4:
			retorno = catptA4_A3;
			break;
		case catptA4_A3:
			retorno = catptA3_A4;
			break;
		// A3a - B
		case catptA3A_B:
			retorno = catptB_A3A;
			break;
		case catptB_A3A:
			retorno = catptA3A_B;
			break;
		// A3a - A4
		case catptA3A_A4:
			retorno = catptA4_A3A;
			break;
		case catptA4_A3A:
			retorno = catptA3A_A4;
			break;
		// A4 - B
		case catptA4_B:
			retorno = catptB_A4;
			break;
		case catptB_A4:
			retorno = catptA4_B;
			break;
		//o else é q nao tem inverso
		default:
			retorno = catptIND;
			break;
	}

	return catptIND;
}
// ---------------------------------------------------------------------------
TList* __fastcall TEnergiaPassantes::LisEnergiaPassante(void)
{
	return (lisENE_PAS);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::MontaEnergiaPassanteCluster(void)
 {
 //variáveis locais
 VTEnergiaPassante  *energiaPassante;
 VTCluster  *cluster;
 VTRede     *rede;
 TList      *lisREDE;
 TList      *lisCLUSTER = redes->LisCluster();

 //proteçao
 if (lisCLUSTER->Count == 0) return(true);
 //loop p/ todos Cluster
 for (int nc = 0; nc < lisCLUSTER->Count; nc++)
 {
 cluster = (VTCluster*)lisCLUSTER->Items[nc];
 //verifica se o Cluster está carregado
 if (! cluster->Carregado) continue;
 //cria objeto EnergiaPassante e insere em EnergiaPassantes
 if ((energiaPassante = NewObjEnergiaPassante(patamares->NumPatamar())) == NULL) return(false);
 InsereEnergiaPassante(lisBALANCO, energiaPassante);
 energiaPassante->Object    = cluster;
 energiaPassante->Descricao = cluster->Codigo;
 //agrega EnergiaPassante de todas as Redes do Cluster
 lisREDE = cluster->LisRede();
 for (int nr = 0; nr < lisREDE->Count; nr++)
 {
 rede = (VTRede*)lisREDE->Items[nr];
 energiaPassante->Agrega(ExisteEnergiaPassante(rede));
 }
 }
 return(true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::MontaEnergiaPassanteCorte(void)
 {
 //variáveis locais
 VTEnergiaPassante *energiaPassante_corte;

 //cria objeto EnergiaPassante e insere em EnergiaPassantes
 if ((energiaPassante_corte = NewObjEnergiaPassante(patamares->NumPatamar())) == 0) return(false);
 InsereEnergiaPassante(lisBALANCO, energiaPassante_corte);
 energiaPassante_corte->Object    = redes->LisCorte();
 energiaPassante_corte->Descricao = "Fronteiras";
 //determina Energia nos Cortes
 fluxo->Executa(redes->LisCorte());
 energiaPassante_corte->EnergiaGerador_mwh    = fluxo->EnergiaGerador_mwh;
 energiaPassante_corte->EnergiaCarga_mwh      = fluxo->EnergiaCarga_mwh;
 energiaPassante_corte->EnergiaRecebida_mwh   = 0.;
 energiaPassante_corte->EnergiaEntregue_mwh   = 0.;
 energiaPassante_corte->EnergiaSuprimento_mwh = fluxo->EnergiaSuprimento_mwh;
 energiaPassante_corte->EnergiaPerdaFluxo_mwh = 0.;
 energiaPassante_corte->EnergiaPerdaTotal_mwh = 0.;
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TEnergiaPassantes::MontaEnergiaPassanteRede(void)
{
	// cria um EnergiaPassante para cada Node
	if (!IniciaEnergiaPassanteRede())
		return (false);
	// monta balanço de energia de cada EnergiaPassante de Node
	MontaFluxoEnergiaEnergiaPassanteRede();
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::MontaEnergiaPassanteReducao(void)
 {
 //cria um EnergiaPassante para cada Rede
 if (! IniciaEnergiaPassanteReducao()) return(false);
 //monta balanço de energia de cada EnergiaPassante de Reducao
 MontaFluxoEnergiaEnergiaPassanteReducao();
 return(true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::MontaEnergiaPassanteTipoRede(void)
 {
 //variáveis locais
 VTEnergiaPassante  *energiaPassante;
 VTTipoRede *tipo_rede;
 VTRede     *rede;
 TList      *lisREDE = redes->LisRede();

 //loop p/ todas as Redes
 for (int nr = 0; nr < lisREDE->Count; nr++)
 {
 rede = (VTRede*)lisREDE->Items[nr];
 if (! rede->Carregada) continue;
 //verifica se já existe EnergiaPassante p/ o TipoRede da Rede
 tipo_rede = rede->TipoRede;
 if ((energiaPassante = ExisteEnergiaPassante(tipo_rede)) == NULL)
 {//cria objeto EnergiaPassante e insere em EnergiaPassantes
 if ((energiaPassante = NewObjEnergiaPassante(patamares->NumPatamar())) == 0) return(false);
 InsereEnergiaPassante(lisBALANCO, energiaPassante);
 energiaPassante->Object    = tipo_rede;
 energiaPassante->Descricao = tipo_rede->Codigo;
 }
 //soma EnergiaPassante da Rede à EnergiaPassante do TipoRede
 energiaPassante->Agrega(ExisteEnergiaPassante(rede));
 //zera EnergiaRecebida_mwh, EnergiaEntregue_mwh e EnergiaTrocada_mwh
 //energiaPassante->EnergiaRecebida_mwh = 0.;
 //energiaPassante->EnergiaEntregue_mwh = 0.;
 //energiaPassante->EnergiaTrocada_mwh  = 0.;
 }
 return(true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::MontaEnergiaPassanteTotal(void)
 {
 //variáveis locais
 VTEnergiaPassante *energiaPassante, *energiaPassante_total;
 VTRede    *rede;
 TList     *lisREDE = redes->LisRede();

 //cria objeto EnergiaPassante e insere em EnergiaPassantes
 if ((energiaPassante_total = NewObjEnergiaPassante(patamares->NumPatamar())) == 0) return(false);
 InsereEnergiaPassante(lisBALANCO, energiaPassante_total);
 energiaPassante_total->Object    = redes;
 energiaPassante_total->Descricao = "Todas Redes";
 //loop p/ todos EnergiaPassantes de Rede
 for (int nr = 0; nr < lisREDE->Count; nr++)
 {
 rede = (VTRede*)lisREDE->Items[nr];
 if (! rede->Carregada) continue;
 energiaPassante_total->Agrega(ExisteEnergiaPassante(rede));
 }
 //zera EnergiaRecebida_mwh e EnergiaEntregue_mwh
 energiaPassante_total->EnergiaRecebida_mwh = 0.;
 energiaPassante_total->EnergiaEntregue_mwh = 0.;
 //loop p/ todos EnergiaPassantes de Reducao
 for (int n  = 0; n < lisBAL_EQV->Count; n++)
 {
 energiaPassante = (VTEnergiaPassante*)lisBAL_EQV->Items[n];
 //acumula energia recebida e entregue
 energiaPassante_total->EnergiaRecebida_mwh -= energiaPassante->EnergiaEntregue_mwh;
 energiaPassante_total->EnergiaEntregue_mwh -= energiaPassante->EnergiaRecebida_mwh;
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TEnergiaPassantes::MontaFluxoEnergiaEnergiaPassanteRede(void)
{
	// variáveis locais
	VTEnergiaPassante *energiaPassante;
	VTRede *rede;
	VTReducao *reducao;
	VTPerda *perda;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPerdas *perdas = (VTPerdas*)apl->GetObject(__classid(VTPerdas));
	TList *lisREDE = redes->LisRede();
	TList *lisTRAFO = NULL;
	VTTrafo *trafo = NULL;
	int catTrafo = 0;
	int catPT = 0;

	// cria a lista para guardar os trafos de cada rede
	/* TODO : tratar trafo 3e ? */
	lisTRAFO = new TList;
	// loop p/ todas as Redes
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		// verifica se o fluxo foi calculado para a rede
		if (!rede->FluxoOK)
			continue;
		// determina objeto Perda da Rede
		if ((perda = perdas->ExistePerda(rede)) == NULL)
			return (false);
		// determina objeto EnergiaPassante
		if ((energiaPassante = ExisteEnergiaPassante(rede)) == 0)
			return (false);
		// verifica se a Rede está carregada
		if (rede->Carregada)
		{
			// limpa a lista de trafos
			lisTRAFO->Clear();
			// obtem os trafos de 2E da lista
			rede->LisEqpto(lisTRAFO, eqptoTRAFO);
			// para cada trafo executa o fluxo
			for (int nt = 0; nt < lisTRAFO->Count; nt++)
			{
				trafo = (VTTrafo*)lisTRAFO->Items[nt];
				// determina fluxo de Energia no trafo
				fluxo->Executa(trafo);
				// determina em qual categoria o trafo se encaixa
				catTrafo = CategoriaTrafo(trafo);
				// determina a categoria PT de acordo com a categoria do trafo
				catPT = DeterminaCategoriaPT(catTrafo);
				//verifica se o sentido direto está correto
				if(fluxo->EnergiaRecebidaExt_mwh < 0)
					{catPT = InverteCategoriaPT(catPT);}
				//protecao
				if (catPT > 0)
				{
					energiaPassante->EnergiaRecebidaExt_mwh[catPT] += fabs(fluxo->EnergiaRecebidaExt_mwh);
				}
				///* TODO : Pensar se deve e como fazer com as energias que são calculadaspor fora do fluxo de potencia */
			}

		}
	}

	if (lisTRAFO)
	{
		delete lisTRAFO;
		lisTRAFO = NULL;
	}
	return (true);
}
 //---------------------------------------------------------------------------
int __fastcall TEnergiaPassantes::NivelTensao(VTBarra *barra)
{
	int nivel = 0;

	if (barra == NULL)
		{return 0;}
	//olha a tensão
	//nivel A1 V >= 230
	if (barra->vnom > 230)
		{nivel = cattA1;}
	else if (barra->vnom > 138)
	{
		//verifica o 230
		if (IsDoubleZero(barra->vnom - 230))
			{nivel = cattA1;}
		else if (IsDoubleZero(barra->vnom - 138))
			{nivel = cattA2;}
		else
			{nivel = 0;}
	}
	//nivel A2  88 > V >= 138
	else if (barra->vnom > 88)
	{
		nivel = cattA2;
	}
	else if (barra->vnom > 69)
	{
        //verifica o 69
		if (IsDoubleZero(barra->vnom - 88))
			{nivel = cattA2;}
		//verifica o 69
		else if (IsDoubleZero(barra->vnom - 69))
			{nivel = cattA3;}
		else
			{nivel = 0;}
	}//verifica o A3 V = 69
	else if (barra->vnom > 44)
	{	//verifica o 69
		if (IsDoubleZero(barra->vnom - 69))
			{nivel = cattA3;}
		else if (IsDoubleZero(barra->vnom - 44))
			{nivel = cattA3a;}
		else
			{nivel = 0;}
	}
	//verifica o A3a 30 < V < = 44
	else if (barra->vnom > 30)
	{
		nivel = cattA3a;
	}
	else if (barra->vnom > 25)
	{
		if (IsDoubleZero(barra->vnom - 30))
			{nivel = cattA3a;}
		else if (IsDoubleZero(barra->vnom - 25))
			{nivel = cattA4;}
		else
			{nivel = 0;}
	}
	//verifica o A4 2,3 < V < = 25
	else if (barra->vnom > 2.3)
	{
		nivel = cattA4;
	}
	//verifica o B V < 2.3
	else if (barra->vnom > 0)
	{
		 if (IsDoubleZero(barra->vnom - 2.3))
			{nivel = cattA4;}
		 else
			{nivel = cattB;}
	}
	else
		{nivel = 0;}


	return nivel;
}
 //---------------------------------------------------------------------------
/*
 //---------------------------------------------------------------------------
 bool __fastcall TEnergiaPassantes::MontaFluxoEnergiaEnergiaPassanteReducao(void)
 {
 //variáveis locais
 VTEnergiaPassante *energiaPassante;
 VTReducao *reducao;

 //loop p/ todos EnergiaPassantes de Reducao
 for (int n = 0; n < lisBAL_EQV->Count; n++)
 {
 energiaPassante = (VTEnergiaPassante*)lisBAL_EQV->Items[n];
 reducao = (VTReducao*)energiaPassante->Object;
 //determina Energia gerada na Reducao
 fluxo->Executa(reducao);
 energiaPassante->EnergiaGerador_mwh    = 0;
 energiaPassante->EnergiaCarga_mwh      = 0;
 energiaPassante->EnergiaRecebida_mwh   = fluxo->EnergiaRecebida_mwh;
 energiaPassante->EnergiaEntregue_mwh   = fluxo->EnergiaEntregue_mwh;
 energiaPassante->EnergiaSuprimento_mwh = 0;
 energiaPassante->EnergiaPerdaFluxo_mwh = 0;
 energiaPassante->EnergiaPerdaTotal_mwh = 0;
 }
 return(true);
 }
 */

//// ---------------------------------------------------------------------------
//void __fastcall TEnergiaPassantes::RedistribuiErroFluxoPorRede(void)
//{
//	// variáveis locais
//	TList *lisREDE;
//	VTNode *node;
//	VTEnergiaPassante *energiaPassante;
//	VTRede *rede;
//	double erro_energia, erro_energia_total, energia_entrada;
//
//	// obtém lista ordenada de todos Nodes da Arvore
//	lisTMP->Clear();
//	arvore->LisAllNode(lisTMP);
//	// loop p/ todos Nodes na ordem direta
//	// for (int n = 0; n < lisTMP->Count; n++)
//	for (int n = lisTMP->Count - 1; n >= 0; n--)
//	{
//		node = (VTNode*)lisTMP->Items[n];
//		lisREDE = node->LisObjectNode();
//		// calcula total de erro_energia das Redes do Node
//		erro_energia_total = 0;
//		for (int nr = 0; nr < lisREDE->Count; nr++)
//		{
//			rede = (VTRede*)lisREDE->Items[nr];
//			// verifica se o fluxo foi calculado para a rede
//			if (!rede->FluxoOK)
//				continue;
//			// obtém Balanço da Rede
//			energiaPassante = ExisteEnergiaPassante(rede);
//			// calcula erro no balanço devido ao erro do fluxo calculado por rede
//			erro_energia =
//				-(
//				energiaPassante->EnergiaRecebidaExt_mwh +
//				energiaPassante->EnergiaEntregueExt_mwh +
//				);
//			// 2016.04.27 - proteção para evitar a entrada de casas decimais extras
//			erro_energia = RoundTo(erro_energia, ROUND_PRECISION);
//			if (IsDoubleZero(erro_energia, DOUBLE_ZERO_VALUE))
//				continue;
//			// inclui erro_energia da rede como energia entrada do balanço da própria rede
//			SomaEnergiaNaEntrada(rede, energiaPassante, erro_energia);
//			// acumula erro_energia total
//			erro_energia_total += erro_energia;
//		}
//		// 2016.04.27 - proteção para evitar a entrada de casas decimais extras
//		erro_energia_total = RoundTo(erro_energia_total, ROUND_PRECISION);
//		if (IsDoubleZero(erro_energia_total, DOUBLE_ZERO_VALUE))
//			continue;
//		// inclui erro_energia total no Balanço de todas Redes do Node pai como energia entregue e entregue
//		if ((node = node->Pai) != NULL)
//		{
//			lisREDE = node->LisObjectNode();
//			if (lisREDE->Count == 0)
//				continue;
//			// verifica se há apenas uma Rede
//			if (lisREDE->Count == 1)
//			{
//				rede = (VTRede*)lisREDE->First();
//				// verifica se o fluxo foi calculado para a rede
//				if (!rede->FluxoOK)
//					continue;
//				energiaPassante = ExisteEnergiaPassante(rede);
//				// soma erro_energia_total como energia entregue no Balanço da rede
//				SomaEnergiaNaSaida(energiaPassante, erro_energia_total);
//			}
//			else
//			{ // calcula total de energia entrada em todas as Redes do Node
//				energia_entrada = RoundTo(TotalizaEnergiaDaEntrada(lisREDE), ROUND_PRECISION);
//				if (IsDoubleZero(energia_entrada))
//					energia_entrada = lisREDE->Count;
//				// rateia perdas_outras entre as Redes, proporcional à Energia de entrada
//				for (int nr = 0; nr < lisREDE->Count; nr++)
//				{
//					rede = (VTRede*)lisREDE->Items[nr];
//					// verifica se o fluxo foi calculado para a rede
//					if (!rede->FluxoOK)
//						continue;
//					energiaPassante = ExisteEnergiaPassante(rede);
//					// soma erro_energia_total como energia entregue no Balanço da rede
//					SomaEnergiaNaSaida(energiaPassante,
//						erro_energia_total *
//						(energiaPassante->EnergiaEntrada_mwh / energia_entrada));
//				}
//			}
//		}
//	}
//}

//// ---------------------------------------------------------------------------
//void __fastcall TEnergiaPassantes::RedistribuiPerdasPorFora(void)
//{
//	// variáveis locais
//	TList *lisREDE;
//	VTNode *node;
//	VTEnergiaPassante *energiaPassante;
//	VTRede *rede;
//	double perdas_fora_total, perdas_fora_rede, energia_entrada;
//
//	// obtém lista ordenada de todos Nodes da Arvore
//	lisTMP->Clear();
//	arvore->LisAllNode(lisTMP);
//	// loop p/ todos Nodes na ordem inversa
//	for (int n = lisTMP->Count - 1; n >= 0; n--)
//	{
//		node = (VTNode*)lisTMP->Items[n];
//		lisREDE = node->LisObjectNode();
//		// calcula total de perdas_fora das Redes do Node
//		perdas_fora_total = 0;
//		for (int nr = 0; nr < lisREDE->Count; nr++)
//		{
//			rede = (VTRede*)lisREDE->Items[nr];
//			// verifica se o fluxo foi calculado para a rede
//			if (!rede->FluxoOK)
//				continue;
//			// obtém Balanço da Rede
//			energiaPassante = ExisteEnergiaPassante(rede);
//			// calcula perdas_fora da rede
//			// perdas_fora_rede = -(energiaPassante->EnergiaPerdaTotal_mwh - energiaPassante->EnergiaPerdaFluxo_mwh);
//			perdas_fora_rede = -energiaPassante->EnergiaPerdaFora_mwh;
//			if (IsDoubleZero(perdas_fora_rede, DOUBLE_ZERO_VALUE))
//				continue;
//			// inclui perdas_fora da rede como energia entrada do balanço da própria rede
//			SomaEnergiaNaEntrada(rede, energiaPassante, perdas_fora_rede);
//			// acumula perdas_fora total
//			perdas_fora_total += perdas_fora_rede;
//		}
//		perdas_fora_total = RoundTo(perdas_fora_total, ROUND_PRECISION);
//		if (IsDoubleZero(perdas_fora_total, DOUBLE_ZERO_VALUE))
//			continue;
//		// inclui as perdas_fora_total no Balanço de todas Redes de todos Nodes pai como energia recebida e entregue
//		while ((node = node->Pai) != NULL)
//		{
//			lisREDE = node->LisObjectNode();
//			// verifica se há apenas uma Rede
//			if (lisREDE->Count == 1)
//			{
//				rede = (VTRede*)lisREDE->First();
//				// verifica se o fluxo foi calculado para a rede
//				if (!rede->FluxoOK)
//					continue;
//				energiaPassante = ExisteEnergiaPassante(rede);
//				// soma perdas_fora como energia entregue no Balanço da rede
//				SomaEnergiaNaSaida(energiaPassante, perdas_fora_total);
//				// soma perdas_fora como energia entrada no Balanço da rede
//				SomaEnergiaNaEntrada(rede, energiaPassante, perdas_fora_total);
//			}
//			else
//			{ // soma energia entrada em todas as Redes do Node
//				energia_entrada = TotalizaEnergiaDaEntrada(lisREDE);
//				if (IsDoubleZero(energia_entrada))
//					energia_entrada = lisREDE->Count;
//				// rateia perdas_fora total entre as Redes, proporcional à Energia de entrada
//				for (int nr = 0; nr < lisREDE->Count; nr++)
//				{
//					rede = (VTRede*)lisREDE->Items[nr];
//					// verifica se o fluxo foi calculado para a rede
//					if (!rede->FluxoOK)
//						continue;
//					energiaPassante = ExisteEnergiaPassante(rede);
//					// soma perdas_fora como energia entregue no Balanço da rede
//					SomaEnergiaNaSaida(energiaPassante,
//						perdas_fora_total *
//						(energiaPassante->EnergiaEntrada_mwh / energia_entrada));
//					// soma perdas_fora como energia entrada no Balanço da rede
//					SomaEnergiaNaEntrada(rede, energiaPassante,
//						perdas_fora_total *
//						(energiaPassante->EnergiaEntrada_mwh / energia_entrada));
//				}
//			}
//		}
//	}
//}
////---------------------------------------------------------------------------
//void __fastcall TEnergiaPassantes::RedistribuiErroFluxoPorRede(void)
//   {
//   //variáveis locais
//   VTBalancos *balancos = (VTBalancos*)apl->GetObject(__classid(VTBalancos));;
//   TList     *lisREDE;
//   VTNode    *node;
//   VTBalanco *balanco;
//   VTEnergiaPassante *energiaPassante;
//   VTRede    *rede;
//   double    erro_energia, erro_energia_total, energia_entrada;
//
//   //obtém lista ordenada de todos Nodes da Arvore
//   lisTMP->Clear();
//   arvore->LisAllNode(lisTMP);
//   //loop p/ todos Nodes na ordem direta
////   for (int n = 0; n < lisTMP->Count; n++)
//   for (int n = lisTMP->Count-1; n >= 0; n--)
//	  {
//	  node    = (VTNode*)lisTMP->Items[n];
//      lisREDE = node->LisObjectNode();
//      //calcula total de erro_energia das Redes do Node
//      erro_energia_total = 0;
//      for (int nr = 0; nr < lisREDE->Count; nr++)
//         {
//		 rede = (VTRede*)lisREDE->Items[nr];
//         //verifica se o fluxo foi calculado para a rede
//         if (! rede->FluxoOK) continue;
//         //obtém Balanço da Rede
//		 balanco = balancos->ExisteBalanco(rede);
//		 energiaPassante = ExisteEnergiaPassante(rede);
//         //calcula erro no balanço devido ao erro do fluxo calculado por rede
//         erro_energia =  -(balanco->EnergiaSuprimento_mwh  + balanco->EnergiaGerador_mwh     +
//                           balanco->EnergiaRecebidaExt_mwh + balanco->EnergiaRecebidaInt_mwh +
//						   balanco->EnergiaEntregueExt_mwh + balanco->EnergiaEntregueInt_mwh +
//                           balanco->EnergiaCarga_mwh       + balanco->EnergiaPerdaFluxo_mwh);
//         //2016.04.27 - proteção para evitar a entrada de casas decimais extras
//         erro_energia = RoundTo(erro_energia, ROUND_PRECISION);
//         if (IsDoubleZero(erro_energia, DOUBLE_ZERO_VALUE)) continue;
//         //inclui erro_energia da rede como energia entrada do balanço da própria rede
//         SomaEnergiaNaEntrada(rede, energiaPassante, erro_energia);
//         //acumula erro_energia total
//         erro_energia_total += erro_energia;
//		 }
//      //2016.04.27 - proteção para evitar a entrada de casas decimais extras
//      erro_energia_total = RoundTo(erro_energia_total, ROUND_PRECISION);
//      if (IsDoubleZero(erro_energia_total, DOUBLE_ZERO_VALUE)) continue;
//      //inclui erro_energia total no Balanço de todas Redes do Node pai como energia entregue e entregue
//      if ((node = node->Pai) != NULL)
//         {
//         lisREDE = node->LisObjectNode();
//         if (lisREDE->Count == 0) continue;
//		 //verifica se há apenas uma Rede
//         if (lisREDE->Count == 1)
//            {
//            rede    = (VTRede*)lisREDE->First();
//            //verifica se o fluxo foi calculado para a rede
//            if (! rede->FluxoOK) continue;
//            balanco = balancos->ExisteBalanco(rede);
//			energiaPassante = ExisteEnergiaPassante(rede);
//			//soma erro_energia_total como energia entregue no Balanço da rede
//			SomaEnergiaNaSaida(energiaPassante, erro_energia_total);
//            }
//         else
//            {//calcula total de energia entrada em todas as Redes do Node
//            energia_entrada = RoundTo(TotalizaEnergiaDaEntrada(lisREDE), ROUND_PRECISION);
//            if (IsDoubleZero(energia_entrada)) energia_entrada = lisREDE->Count;
//			//rateia perdas_outras entre as Redes, proporcional à Energia de entrada
//            for (int nr = 0; nr < lisREDE->Count; nr++)
//               {
//			   rede    = (VTRede*)lisREDE->Items[nr];
//			   //verifica se o fluxo foi calculado para a rede
//			   if (! rede->FluxoOK) continue;
//               balanco = balancos->ExisteBalanco(rede);
//			   energiaPassante = ExisteEnergiaPassante(rede);
//			   //soma erro_energia_total como energia entregue no Balanço da rede
//			   SomaEnergiaNaSaida(energiaPassante, erro_energia_total * (balanco->EnergiaEntrada_mwh / energia_entrada));
//			   }
//            }
//		 }
//      }
//   }

// ---------------------------------------------------------------------------
void __fastcall TEnergiaPassantes::Reinicia(void)
{
	LimpaTList(lisENE_PAS);
	// LimpaTList(lisBAL_EQV);
}

//// ---------------------------------------------------------------------------
//void __fastcall TEnergiaPassantes::SomaEnergiaNaEntrada(VTRede *rede,
//	VTEnergiaPassante *energiaPassante, double perdas_outras)
//{
//	// inclui perdas_outras como energia entrada do próprio balanço
//	if (!IsDoubleZero(energiaPassante->EnergiaRecebidaExt_mwh, DOUBLE_ZERO_VALUE))
//	{
//		energiaPassante->EnergiaRecebidaExt_mwh += perdas_outras;
//	}
//}
//
//// ---------------------------------------------------------------------------
//void __fastcall TEnergiaPassantes::SomaEnergiaNaSaida(VTEnergiaPassante *energiaPassante,
//	double perdas_outras)
//{
//	// inclui perdas_outras como energia entregue do próprio balanço
//	if (!IsDoubleZero(energiaPassante->EnergiaEntregueExt_mwh, DOUBLE_ZERO_VALUE))
//	{
//		energiaPassante->EnergiaEntregueExt_mwh -= perdas_outras;
//	}
//	else if (!IsDoubleZero(energiaPassante->EnergiaEntregueInt_mwh, DOUBLE_ZERO_VALUE))
//	{
//		energiaPassante->EnergiaEntregueInt_mwh -= perdas_outras;
//	}
//	else
//	{
//		energiaPassante->EnergiaEntregueExt_mwh -= perdas_outras;
//	}
//}


//// ---------------------------------------------------------------------------
//double __fastcall TEnergiaPassantes::TotalizaEnergiaDaEntrada(TList *lisREDE)
//{
//	// variáveis locais
//	VTEnergiaPassante *energiaPassante;
//	VTRede *rede;
//	double energia_entrada = 0.;
//
//	for (int nr = 0; nr < lisREDE->Count; nr++)
//	{
//		rede = (VTRede*)lisREDE->Items[nr];
//		energiaPassante = ExisteEnergiaPassante(rede);
//		energia_entrada += energiaPassante->EnergiaEntrada_mwh;
//	}
//	return (energia_entrada);
//}

// ---------------------------------------------------------------------------
// eof
