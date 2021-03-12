// -----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <locale>
#include "TIntegraEtd.h"
#include "..\RetiraETD\VTRetiraETD.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Edita\VTEdita.h"
#include "..\..\Importa\Monta\VTMonta.h"
#include "..\..\InterfaceTXT\Importa\VTImportaTXT.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Radial\VTSubestacao.h"
#include "..\..\Rede\VTArea.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTDominio.h"
#include "..\..\Rede\VTEqbar.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\InterfaceTXT.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\..\DLL_Inc\Rede.h"

// -----------------------------------------------------------------------------
VTIntegraEtd* NewObjIntegraEtd(VTApl *apl)
{
	try
	{
		return (new TIntegraEtd(apl));
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// erro na criação do objeto
	return (NULL);
}

// -----------------------------------------------------------------------------
__fastcall TIntegraEtd::TIntegraEtd(VTApl *apl)
{
	// salva ponteiros
	this->apl = apl;
	// cria objetos
	lisBAR = new TList();
	lisBAR_INI = new TList();
	lisCAR = new TList();
	lisETD = new TList();
	lisLIG = new TList();
	lisPRI = new TList();
	lisSEC = new TList();
	lisEQP = new TList();
	lisDEL = new TList();
	lisALT = new TList();
}

// -----------------------------------------------------------------------------
__fastcall TIntegraEtd::~TIntegraEtd(void)
{
	// destrói lista sem destruir objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisBAR_INI)
	{
		delete lisBAR_INI;
		lisBAR_INI = NULL;
	}
	if (lisCAR)
	{
		delete lisCAR;
		lisCAR = NULL;
	}
	if (lisETD)
	{
		delete lisETD;
		lisETD = NULL;
	}
	if (lisLIG)
	{
		delete lisLIG;
		lisLIG = NULL;
	}
	if (lisPRI)
	{
		delete lisPRI;
		lisPRI = NULL;
	}
	if (lisSEC)
	{
		delete lisSEC;
		lisSEC = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisDEL)
	{
		delete lisDEL;
		lisDEL = NULL;
	}
	if (lisALT)
	{
		delete lisALT;
		lisALT = NULL;
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::CalculaAreaEsq(TList *lisBAR, VTArea *area)
{
	// variáveis locais
	VTBarra *barra;

	// proteção
	if (lisBAR->Count == 0)
		return (false);
	// calcula área definida pelas Barras
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if (nb == 0)
		{
			area->x1 = Min(barra->esq.x1, barra->esq.x2);
			area->x2 = Max(barra->esq.x1, barra->esq.x2);
			area->y1 = Min(barra->esq.y1, barra->esq.y2);
			area->y2 = Max(barra->esq.y1, barra->esq.y2);
		}
		else
		{
			area->x1 = Min(int(area->x1), Min(barra->esq.x1, barra->esq.x2));
			area->x2 = Max(int(area->x2), Max(barra->esq.x1, barra->esq.x2));
			area->y1 = Min(int(area->y1), Min(barra->esq.y1, barra->esq.y2));
			area->y2 = Max(int(area->y2), Max(barra->esq.y1, barra->esq.y2));
		}
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::CalculaAreaUtm(TList *lisBAR, VTArea *area)
{
	// variáveis locais
	VTBarra *barra;

	// proteção
	if (lisBAR->Count == 0)
		return (false);
	// calcula área definida pelas Barras
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if (nb == 0)
		{
			area->x1 = area->x2 = barra->utm.x;
			area->y1 = area->y2 = barra->utm.y;
		}
		else
		{
			area->x1 = Min(int(area->x1), barra->utm.x);
			area->x2 = Max(int(area->x2), barra->utm.x);
			area->y1 = Min(int(area->y1), barra->utm.y);
			area->y2 = Max(int(area->y2), barra->utm.y);
		}
	}
	return ((area->x1 > 0) && (area->y1 > 0));
}

// -----------------------------------------------------------------------------
VTRede* __fastcall TIntegraEtd::CarregaRedeDaBase(AnsiString codigo)
{
	// variáveis locais
	AnsiString arq_txt;
	VTImportaTXT *importaTXT;
	VTRede *rede = NULL;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{ // define path do arquivo TXT da rede
		arq_txt = path->DirImporta() + "\\ArquivosTXT_ModeloSinap\\" + codigo + ".txt";
		if (!FileExists(arq_txt))
			return (NULL);
		// cria objeto p/ importar arquivo TXT
		importaTXT = DLL_NewObjImportaTXT(apl);
		// importa arquivo TXT em Redes do Apl local
		if (importaTXT->ImportaRedes(arq_txt))
		{ // obtém rede lida
			rede = redes->ExisteRede(codigo);
		}
		// destrói objeto criado
		delete importaTXT;
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (rede);
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::ClonaEqptos(TList *lisORIG, TList *lisCLONE)
{
	// variáveis locais
	VTEqpto *eqpto, *clone;

	for (int nt = 0; nt < lisORIG->Count; nt++)
	{ // clona equipamento e guarda na lista de clones
		eqpto = (VTEqpto*)lisORIG->Items[nt];
		clone = eqpto->Clone();
		eqpto->Obj = clone;
		clone->Obj = eqpto;
		lisCLONE->Add(clone);
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::Determina1Suprimento(VTSubestacao *subestacao, VTBarra *barra)
{
	// variaveis locais
	VTRede *rede_etd;
	VTSuprimento *suprimento, *clone;

	// proteção
	if (subestacao == NULL)
		return (false);
	// determina rede da subestação
	rede_etd = subestacao->Rede;
	// obtem suprimentos da subestação
	lisEQP->Clear();
	rede_etd->LisEqbar(lisEQP, eqptoSUPRIMENTO);
	if (lisEQP->Count == 0)
		return (false);
	// considera o primeiro da lista
	suprimento = (VTSuprimento*)lisEQP->First();
	if (suprimento->pbarra != barra)
	{ // clona suprimento
		clone = suprimento->Clone();
		suprimento->Obj = clone;
		clone->Obj = suprimento;
		clone->pbarra->RemoveEqbar(clone);
		barra->InsereEqbar(clone);
		clone->DefineObjBarra(barra);
		// insere suprimento original na lista de eqptos alterados
		lisALT->Add(suprimento);
	}
	// remove demais suprimentos da subestação
	lisEQP->Remove(suprimento);
	CopiaTList(lisEQP, lisDEL);

	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::DeterminaBarraATMT(VTSubestacao *subestacao, VTBarra **barra_at,
	VTBarra **barra_mt)
{
	// variaveis locais
	VTRede *rede_etd;
	TList *lisBAR_ETD;
	VTBarra *bar_at, *bar_mt;

	// proteção
	if (subestacao == NULL)
		return (false);
	// determina rede
	rede_etd = subestacao->Rede;
	// verifica se há barras na subestação
	lisBAR_ETD = rede_etd->LisBarra();
	if (lisBAR_ETD->Count < 2)
		return (false);
	// determina uma barra AT e uma MT
	bar_at = (VTBarra*)lisBAR_ETD->First();
	bar_mt = (VTBarra*)lisBAR_ETD->Last();
	*barra_at = bar_at;
	*barra_mt = bar_mt;
	// proteção
	if (barra_at == barra_mt)
		return (false);

	return (true);
}

// -----------------------------------------------------------------------------
VTCarga* __fastcall TIntegraEtd::ExisteCarga(TList *lisCAR, AnsiString codigo)
{
	// variáveis locais
	AnsiString cod_carga;
	VTCarga *carga;

	// retira caracteres indesejados do código
	codigo = StringTrim(codigo);
	codigo = codigo.SubString(1, 3) + codigo.SubString(codigo.Length() - 2, 3);
	for (int n = 0; n < lisCAR->Count; n++)
	{
		carga = (VTCarga*)lisCAR->Items[n];
		// trata código da Carga, eliminado caracteres indesejados
		cod_carga = StringTrim(carga->Codigo);
		// verifica se o código da Carga é iniciado por "CAR"
		if (cod_carga.AnsiPos("CAR") == 1)
		{ // elimina os caracteres "CAR"
			cod_carga = cod_carga.SubString(4, cod_carga.Length() - 3);
		}
		cod_carga = cod_carga.SubString(1, 3) + cod_carga.SubString(cod_carga.Length() - 2, 3);
		if (cod_carga.AnsiCompareIC(codigo) == 0)
			return (carga);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTLigacao* __fastcall TIntegraEtd::ExisteChave(TList *lisEXT)
{
	// variáveis locais
	VTLigacao *ligacao;

	for (int n = 0; n < lisEXT->Count; n++)
	{
		ligacao = (VTLigacao*)lisEXT->Items[n];
		if (ligacao->Tipo() == eqptoCHAVE)
			return (ligacao);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::InsereETDcompleta(void)
{
	// variáveis locais
	AnsiString codigo;
	int count = 0;
	VTBarra *barra;
	VTCarga *carga;
	VTRede *rede_etd1, *rede_etd2, *rede_pri;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// inicia lisCAR
	lisCAR->Clear();
	// inicia lista de Eqptos a serem excluídos
	lisDEL->Clear();
	// obtém lista de Redes do tipo ETD
	lisETD->Clear();
	redes->LisRede(lisETD, redeETD);
	// obtém lista de Redes do tipo REDE_PRI
	lisPRI->Clear();
	redes->LisRede(lisPRI, redePRI);
	// obtém lista de Redes do tipo REDE_SEC
	lisSEC->Clear();
	redes->LisRede(lisSEC, redeSEC);
	// obtém lista de Barras iniciais das redes primárias
	lisBAR_INI->Clear();
	for (int n = 0; n < lisPRI->Count; n++)
	{
		rede_pri = (VTRede*)lisPRI->Items[n];
		if ((barra = rede_pri->BarraInicial()) == NULL)
			continue;
		if (lisBAR_INI->IndexOf(barra) < 0)
			lisBAR_INI->Add(barra);
	}
	// loop p/ todas redes ETD
	for (int nr = 0; nr < lisETD->Count; nr++)
	{
		rede_etd1 = (VTRede*)lisETD->Items[nr];
		// salva e redefine código da Rede
		codigo = rede_etd1->Codigo;
		rede_etd1->Codigo = rede_etd1->Codigo + "_";
		// carrega ETD da base de dados
		if ((rede_etd2 = CarregaRedeDaBase(codigo)) == NULL)
			continue;
		// retira ETD antiga de Redes
		// redes->RemoveRede(rede_etd1);
		// ETD e suas Barras deverão ser eliminadas
		InsereToDel(rede_etd1);
		InsereToDel(rede_etd1->LisBarra());
		// redefine Dominio da Rede2
		RedefineDominioRede(rede_etd2, rede_etd1);
		// redefine todos Eqptos da Rede2 c/ Status novo
		RedefineEqptoRedeStatusNovo(rede_etd2);
		// redefine coordenadas utm da Rede lida da base
		RedefineCoordenadasUtmETD(rede_etd2, rede_etd1);
		// redefine impedâncias de aterramento dos Trafos da ETD
		RedefineTrafoZat(rede_etd2);
		// salva associacao entre as Redes
		rede_etd1->Obj = rede_etd2;
		// salva Cargas da Rede2 em lisCAR
		lisCAR->Clear();
		rede_etd2->LisEqbar(lisCAR, eqptoCARGA);
		// loop p/ todas redes primárias
		for (int nr = 0; nr < lisPRI->Count; nr++)
		{
			rede_pri = (VTRede*)lisPRI->Items[nr];
			// determina Carga em lisCAR que tem o mesmo nome da rede primária
			if ((carga = ExisteCarga(lisCAR, rede_pri->Codigo)) == NULL)
				continue;
			// Carga deverá ser excluída da rede
			lisCAR->Remove(carga);
			InsereToDel(carga);
			// redefine Barra da Carga como Barra inicial Rede primária
			if (!RedefineBarraInicial(rede_pri, carga->pbarra))
				continue;
			// incrementa número de primários integrados com sua ETD real
			count++;
		}
	}
	// se necessário, redefine coordenadas esquemático das ETDs lidas da base
	RedefineCoordenadasEsqETD();
	// recalcula área das Redes
	redes->CalculaArea();
	// se necessário, define coordenadas UTM das redes primárias e secundárias
	RedefineCoordenadasUtmPRI_SEC();
	// inclui Barras isoladas na lista de eqptos a serem eliminados
	CopiaTList(redes->LisBarraIsolada(), lisDEL);
	// altera todos eqptos em lisALT
	if (lisALT->Count > 0)
		edita->AlteraLisEqpto(lisALT);
	// elimina todos os eqptos em lisDEL
	if (lisDEL->Count > 0)
		edita->RetiraLisEqpto(lisDEL);
	// recalcula área das Redes
	redes->CalculaArea();

	return (count > 0);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TIntegraEtd::InsereETDsimplificada(void)
 {//DVK 2017.05.10
 //variáveis locais
 AnsiString codigo;
 int     count = 0;
 TList   *lisBAR_ETD, *lisCLONE, *lisALT;
 VTBarra *barra_at, *barra_mt, *barra;
 VTCarga *carga;
 VTRede  *rede_etd, *rede_pri;
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
 VTSuprimento *suprimento;

 //cria lista de trafos e eqptos alterados
 lisCLONE = new TList();
 lisALT   = new TList();
 //inicia lista de Eqptos a serem excluídos
 lisDEL->Clear();
 //obtém lista de Redes do tipo ETD
 lisETD->Clear();
 redes->LisRede(lisETD, redeETD);
 //obtém lista de Redes do tipo REDE_PRI
 lisPRI->Clear();
 redes->LisRede(lisPRI, redePRI);
 //obtém lista de Redes do tipo REDE_SEC
 lisSEC->Clear();
 //   redes->LisRede(lisSEC, redeSEC);
 //loop p/ todas redes ETD
 for (int nr = 0; nr < lisETD->Count; nr++)
 {
 rede_etd = (VTRede*)lisETD->Items[nr];
 //verifica se há barras na subestação
 lisBAR_ETD = rede_etd->LisBarra();
 if(lisBAR_ETD->Count < 2) continue;
 //determina uma barra AT e uma MT
 barra_at = (VTBarra*)lisBAR_ETD->First();
 barra_mt = (VTBarra*)lisBAR_ETD->Last();
 //proteção
 if(barra_at == barra_mt) continue;
 //altera coordenadas das barras
 RedefineCoordenadasATMT(barra_at, barra_mt);
 //obtem suprimentos da subestação
 lisEQP->Clear();
 rede_etd->LisEqbar(lisEQP, eqptoSUPRIMENTO);
 //clona lista de suprimentos
 lisCLONE->Clear();
 ClonaEqptos(lisEQP, lisCLONE);
 if(lisCLONE->Count > 0)
 {//associa 1 suprimento à barra at
 suprimento = (VTSuprimento*)lisCLONE->First();
 suprimento->pbarra->RemoveEqbar(suprimento);
 barra_at->InsereEqbar(suprimento);
 suprimento->DefineObjBarra(barra_at);
 //insere suprimento original na lista de eqptos alterados
 lisALT->Add(suprimento->Obj);
 }
 //obtem lista de trafos da subestação
 lisEQP->Clear();
 rede_etd->LisEqpto(lisEQP, eqptoTRAFO);
 //clona lista de trafos
 lisCLONE->Clear();
 ClonaEqptos(lisEQP, lisCLONE);
 //associa todos os trafos clone às barras at e mt
 RedefineTrafos(lisCLONE, barra_at, barra_mt);
 //insere todos os trafos originais na lista de eqptos alterados
 CopiaTList(lisEQP, lisALT);
 //loop p/ todas redes primárias
 for (int nr = 0; nr < lisPRI->Count; nr++)
 {
 rede_pri = (VTRede*)lisPRI->Items[nr];
 //verifica se a rede estava conectada na subestação
 barra = rede_pri->BarraInicial();
 if(lisBAR_ETD->IndexOf(barra) < 0) continue;
 //redefine Barra inicial da Rede primária
 if (! RedefineBarraInicial(rede_pri, barra_mt)) continue;
 //incrementa número de primários integrados com sua ETD real
 count++;
 }
 //remove demais barras e outros eqptos da subestação
 lisBAR_ETD->Remove(barra_at);
 lisBAR_ETD->Remove(barra_mt);
 InsereToDel(lisBAR_ETD);
 }
 //recalcula área das Redes
 redes->CalculaArea();
 //inclui Barras isoladas na lista de eqptos a serem eliminados
 CopiaTList(redes->LisBarraIsolada(), lisDEL);
 //altera todos eqptos em lisALT
 if (lisALT->Count > 0) edita->AlteraLisEqpto(lisALT);
 //elimina todos os eqptos em lisDEL
 if (lisDEL->Count > 0) edita->RetiraLisEqpto(lisDEL);
 //recalcula área das Redes
 redes->CalculaArea();

 //destroi lista
 delete lisCLONE;
 delete lisALT;
 return(count > 0);
 }
 */
/*
 // -----------------------------------------------------------------------------
 bool __fastcall TIntegraEtd::InsereETDsimplificada(void)
 { // DVK 2017.07.12
 // variáveis locais
 AnsiString codigo;
 int count = 0;
 TList *lisBAR_ETD, *lisCLONE, *lisALT;
 VTBarra *barra_at, *barra_mt, *barra;
 VTCarga *carga;
 VTRede *rede_etd, *rede_pri;
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
 VTSuprimento *suprimento, *clone;

 // cria lista de trafos e eqptos alterados
 lisCLONE = new TList();
 lisALT = new TList();
 // inicia lista de Eqptos a serem excluídos
 lisDEL->Clear();
 // obtém lista de Redes do tipo ETD
 lisETD->Clear();
 redes->LisRede(lisETD, redeETD);
 // obtém lista de Redes do tipo REDE_PRI
 lisPRI->Clear();
 redes->LisRede(lisPRI, redePRI);
 // obtém lista de Redes do tipo REDE_SEC
 lisSEC->Clear();
 // redes->LisRede(lisSEC, redeSEC);
 // loop p/ todas redes ETD
 for (int nr = 0; nr < lisETD->Count; nr++)
 {
 rede_etd = (VTRede*)lisETD->Items[nr];
 // verifica se há barras na subestação
 lisBAR_ETD = rede_etd->LisBarra();
 if (lisBAR_ETD->Count < 2)
 continue;
 // determina uma barra AT e uma MT
 barra_at = (VTBarra*)lisBAR_ETD->First();
 barra_mt = (VTBarra*)lisBAR_ETD->Last();
 // proteção
 if (barra_at == barra_mt)
 continue;
 // altera coordenadas das barras
 RedefineCoordenadasATMT(barra_at, barra_mt);
 // obtem suprimentos da subestação
 lisEQP->Clear();
 rede_etd->LisEqbar(lisEQP, eqptoSUPRIMENTO);
 if (lisEQP->Count > 0)
 { // associa 1 suprimento à barra at
 suprimento = (VTSuprimento*)lisEQP->First();
 if (suprimento->pbarra != barra_at)
 { // clona suprimento
 clone = suprimento->Clone();
 suprimento->Obj = clone;
 clone->Obj = suprimento;
 clone->pbarra->RemoveEqbar(clone);
 barra_at->InsereEqbar(clone);
 clone->DefineObjBarra(barra_at);
 // insere suprimento original na lista de eqptos alterados
 lisALT->Add(suprimento);
 }
 // remove demais suprimentos da subestação
 lisEQP->Remove(suprimento);
 CopiaTList(lisEQP, lisDEL);
 }
 // obtem lista de trafos da subestação
 lisEQP->Clear();
 rede_etd->LisEqpto(lisEQP, eqptoTRAFO);
 // clona lista de trafos
 lisCLONE->Clear();
 ClonaEqptos(lisEQP, lisCLONE);
 // associa todos os trafos clone às barras at e mt
 RedefineTrafos(lisCLONE, barra_at, barra_mt);
 // insere todos os trafos originais na lista de eqptos alterados
 CopiaTList(lisEQP, lisALT);
 // loop p/ todas redes primárias
 for (int nr = 0; nr < lisPRI->Count; nr++)
 {
 rede_pri = (VTRede*)lisPRI->Items[nr];
 // verifica se a rede estava conectada na subestação
 //			barra = rede_pri->BarraInicial();
 //			if (lisBAR_ETD->IndexOf(barra) < 0)
 //				continue;
 // redefine Barra inicial da Rede primária
 if (!RedefineBarraInicial(rede_pri, barra_mt, lisALT))
 continue;
 // incrementa número de primários integrados com sua ETD real
 count++;
 }
 // remove demais barras e outros eqptos da subestação
 lisBAR_ETD->Remove(barra_at);
 lisBAR_ETD->Remove(barra_mt);
 InsereToDel(lisBAR_ETD);
 }
 // recalcula área das Redes e barras isoladas
 redes->CalculaArea();
 redes->ReiniciaLisBarraIsolada();
 // inclui Barras isoladas na lista de eqptos a serem eliminados
 CopiaTList(redes->LisBarraIsolada(), lisDEL);
 // altera todos eqptos em lisALT
 if (lisALT->Count > 0)
 edita->AlteraLisEqpto(lisALT);
 // elimina todos os eqptos em lisDEL
 if (lisDEL->Count > 0)
 edita->RetiraLisEqpto(lisDEL);
 // recalcula área das Redes
 redes->CalculaArea();

 // destroi lista
 delete lisCLONE;
 delete lisALT;
 return (count > 0);
 }
 */

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::InsereETDsimplificada(void)
{ // DVK 2017.07.13
	// variáveis locais
	int count = 0;
	TList *lisSUBESTACAO, *lisPRIMARIO, *lisBAR_ETD;
	VTBarra *barra_at, *barra_mt;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	VTSubestacao *subestacao;
	VTPrimario *primario;

	// inicia lista de Eqptos a serem alterados/excluídos
	lisALT->Clear();
	lisDEL->Clear();
	// cria obj Radial se nao tiver no Apl
	if (radial == NULL)
	{
		radial = DLL_NewObjRadial(apl);
		apl->Add(radial);
	}
	// executa radial
	radial->Inicia(redes);
	// obtém lista de Subestações
	lisSUBESTACAO = radial->LisSubestacao();
	// loop p/ todas subestações
	for (int nr = 0; nr < lisSUBESTACAO->Count; nr++)
	{
		subestacao = (VTSubestacao*)lisSUBESTACAO->Items[nr];
		// determina as 2 unicas barras que farao parte da subestacao
		barra_at = barra_mt = NULL;
		if (!DeterminaBarraATMT(subestacao, &barra_at, &barra_mt))
			continue;
		// associa 1 suprimento à barra at
		if (!Determina1Suprimento(subestacao, barra_at))
			continue;
		// altera coordenadas das barras
		RedefineCoordenadasATMT(barra_at, barra_mt);
		// associa todos os trafos da subestacao às barras at e mt
		if (!RedefineTrafos(subestacao, barra_at, barra_mt))
			continue;
		// loop p/ todas redes primárias
		lisPRIMARIO = subestacao->LisPrimario();
		for (int nr = 0; nr < lisPRIMARIO->Count; nr++)
		{
			primario = (VTPrimario*)lisPRIMARIO->Items[nr];
			// redefine Barra inicial da Rede primária e move ligacoes para essa barra
			if (!RedefineBarraInicial(primario->Rede, barra_mt))
				continue;
			// incrementa número de primários integrados com sua ETD real
			count++;
		}
		// remove demais barras e outros eqptos da subestação
		lisBAR_ETD = subestacao->Rede->LisBarra();
		lisBAR_ETD->Remove(barra_at);
		lisBAR_ETD->Remove(barra_mt);
		InsereToDel(lisBAR_ETD);
	}
	// recalcula área das Redes e barras isoladas
	redes->CalculaArea();
	redes->ReiniciaLisBarraIsolada();
	// inclui Barras isoladas na lista de eqptos a serem eliminados
//	CopiaTList(redes->LisBarraIsolada(), lisDEL); //o certo seria remover, mas demora muito
	// altera todos eqptos em lisALT
	if (lisALT->Count > 0)
		edita->AlteraLisEqpto(lisALT);
	// elimina todos os eqptos em lisDEL
	if (lisDEL->Count > 0)
		edita->RetiraLisEqpto(lisDEL);
	// recalcula área das Redes
	redes->CalculaArea();

	return (count > 0);
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::InsereSoSuprimentos(void)
{ // DVK 2017.05.10
	// variáveis locais
	VTRetiraEtd *retira;
	bool sucesso = false;

	// cria objeto
	retira = NewObjRetiraEtd(apl);
	// executa retirada de subestações e inserção de suprimentos no lugar
	if (retira != NULL)
	{
		sucesso = retira->Executa();
		delete retira;
	}
	return (sucesso);
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::InsereToDel(VTEqpto *eqpto)
{
	// verifica se Eqpto  já existe em lisDEL
	if (lisDEL->IndexOf(eqpto) < 0)
		lisDEL->Add(eqpto);
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::InsereToDel(TList *lisEQP)
{
	for (int n = 0; n < lisEQP->Count; n++)
	{
		InsereToDel((VTEqpto*)lisEQP->Items[n]);
	}
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TIntegraEtd::RedefineBarraInicial(VTRede *rede, VTBarra *bar_ini)
 {
 //variáveis locais
 int       ind_bar;
 VTBarra   *barra;
 VTLigacao *ligacao;

 //determina Barra inicial da Rede primária
 barra = rede->BarraInicial();
 //redefine coordenadas esquemático das Barras da Rede
 //determina disjuntor da Rede primária
 lisLIG->Clear();
 rede->LisLigacao(lisLIG, barra);
 //if (lisLIG->Count != 1) return(false);
 //determina Ligacao que é Chave
 if ((ligacao = ExisteChave(lisLIG)) == NULL)
 {//não existe chave: utiliza primeira Ligacao da lista
 ligacao = (VTLigacao*)lisLIG->First();
 }
 //determina Barra da Ligacao diferente da Barra inicial da Rede
 ind_bar =ligacao->IndiceBarra(barra);
 barra   = ligacao->Barra(1 - ind_bar);
 //esta barra deverá ser excluída da rede
 lisDEL->Add(barra);
 //determina todas Ligacoes que se conectam à Barra
 lisLIG->Clear();
 rede->LisLigacao(lisLIG, barra);
 //retira disjuntor da lista de Ligacoes e elimina da rede
 lisLIG->Remove(ligacao);
 rede->RemoveLigacao(ligacao);
 DLL_DeleteEqpto(ligacao);
 //redefine Barra de todas Ligaçoes
 for (int n = 0; n < lisLIG->Count; n++)
 {
 ligacao = (VTLigacao*)lisLIG->Items[n];
 if      (ligacao->pbarra1 == barra) ligacao->pbarra1 = bar_ini;
 else if (ligacao->pbarra2 == barra) ligacao->pbarra2 = bar_ini;
 }
 //redefine Barra inicial da rede primária
 rede->DefineBarraInicial(bar_ini);
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::RedefineBarraInicial(VTRede *rede, VTBarra *bar_ini)
{
	// variáveis locais
	int ind_bar;
	VTBarra *barra;
	VTLigacao *ligacao, *clone;

	// determina Barra inicial da Rede primária
	barra = rede->BarraInicial();
	// determina todas Ligacoes que se conectam à Barra
	lisLIG->Clear();
	rede->LisLigacao(lisLIG, barra);
	for (int n = 0; n < lisLIG->Count; n++)
	{ // redefine Barra da chave da rede conectada à barra inicial
		// ou a única ligacao conectada à essa barra
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if ((lisLIG->Count > 1) && (ligacao->Tipo() != eqptoCHAVE))
			continue;
		clone = ligacao->Clone();
		clone->Obj = ligacao;
		ligacao->Obj = clone;
		if (clone->pbarra1 == barra)
		{
			clone->DefineObjBarra(bar_ini, clone->pbarra2);
			clone->pbarra1 = bar_ini;
		}
		else if (clone->pbarra2 == barra)
		{
			clone->DefineObjBarra(clone->pbarra1, bar_ini);
			clone->pbarra2 = bar_ini;
		}
		// insere ligacao original na lista de eqptos alterados
		lisALT->Add(ligacao);
		break;
	}
	// redefine Barra inicial da rede primária
	rede->DefineBarraInicial(bar_ini);
	// atualiza lista de Barras da Rede
	(rede->LisBarra())->Remove(barra);
	(rede->LisBarra())->Add(bar_ini);

	return (true);
}

/*
 //-----------------------------------------------------------------------------
 void __fastcall TIntegraEtd::RedefineCoordenadas(VTRede *rede, VTBarra *bar_ref)
 {
 //variáveis locais
 int        x_ref, y_ref, x_ini, y_ini;
 VTBarra   *barra, *bar_ini;
 VTLigacao *ligacao;

 //salva coordenadas da Barra inicial
 bar_ini = rede->BarraInicial();
 x_ini = bar_ini->esq.x1;
 y_ini = bar_ini->esq.y1;
 //determina coordenadas de referência
 x_ref = (bar_ref->esq.x1 + bar_ref->esq.x2) / 2;
 y_ref = (bar_ref->esq.y1 + bar_ref->esq.y2) / 2;
 //monta lista c/ todas as Barras da Rede (incluindo Barras conectadas por Chave aberta)
 lisBAR->Clear();
 rede->LisBarra(lisBAR, true);
 lisLIG->Clear();
 rede->LisLigacao(lisLIG, eqptoCHAVE);
 for (int n = 0; n <lisLIG->Count; n++)
 {
 ligacao = (VTLigacao*)lisLIG->Items[n];
 if (lisBAR->IndexOf(ligacao->pbarra1) < 0) lisBAR->Add(ligacao->pbarra1);
 if (lisBAR->IndexOf(ligacao->pbarra2) < 0) lisBAR->Add(ligacao->pbarra2);
 }
 //loop p/ todas as Barras da Rede
 for (int nb = 0; nb < lisBAR->Count; nb++)
 {
 barra = (VTBarra*)lisBAR->Items[nb];
 barra->esq.x1 = x_ref + ((barra->esq.x1 - x_ini) * 10);  // multiplica por 10 p/ tornara a rede maior
 barra->esq.y1 = y_ref + ((barra->esq.y1 - y_ini) * 10);
 barra->esq.x2 =  barra->esq.x1;
 barra->esq.y2 =  barra->esq.y1;
 }
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineCoordenadasATMT(VTBarra *barra_at, VTBarra *barra_mt)
{
	// variáveis locais
	int utm_x, utm_y;

	// considera coordenada da barra AT como referência
	utm_x = barra_at->utm.x;
	utm_y = barra_at->utm.y;
	if (IsDoubleZero(utm_x) || IsDoubleZero(utm_y))
		return;
	// redefine utm da outra barra, deslocada
	barra_mt->utm.x = utm_x;
	barra_mt->utm.y = utm_y - 1000.;
	// redefine coordenadas esquemáticas
	barra_at->esq.x1 = utm_x;
	barra_at->esq.x2 = utm_x + 1000.;
	barra_at->esq.y1 = utm_y;
	barra_at->esq.y2 = utm_y;
	barra_mt->esq.x1 = utm_x;
	barra_mt->esq.x2 = utm_x + 1000.;
	barra_mt->esq.y1 = utm_y - 1000.;
	barra_mt->esq.y2 = utm_y - 1000.;
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineCoordenadasEsqETD(void)
{
	// variáveis locais
	int utm_x, utm_y, esq_x, esq_y;
	double fat_escala;
	TList *lisBAR;
	VTBarra *barra, *bar_ini;
	VTArea *area_utm, *area_esq;
	VTEqbar *eqbar;
	VTRede *rede_etd;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obtém lista de Redes do tipo ETD
	lisETD->Clear();
	redes->LisRede(lisETD, redeETD);
	// loop p/ todas ETDs
	for (int nr = 0; nr < lisETD->Count; nr++)
	{
		rede_etd = (VTRede*)lisETD->Items[nr];
		// obtém área utm da ETD
		area_utm = rede_etd->AreaUtm_cm();
		// proteção: verifica se a Rede possui coordenadas utm definidas
		if (area_utm->x1 < 0)
			continue;
		// determina ponto médio da área utm
		utm_x = (area_utm->x1 + area_utm->x2) / 2;
		utm_y = (area_utm->y1 + area_utm->y2) / 2;
		// obtém área esquemático da ETD
		area_esq = rede_etd->AreaEsquematico();
		// calcula fator de escala em função da área utm total de todas as redes
		fat_escala = 0.02 * (redes->AreaUtm_cm())->Width() / area_esq->Width();
		// if (fat_escala < 100) fat_escala = 100;
		// define coordenadas de referência para o esquemático (ponto superior esquerdo)
		esq_x = utm_x - ((area_esq->Width() * fat_escala) / 2);
		esq_y = utm_y - ((area_esq->Height() * fat_escala) / 2);
		// redefine coordenadas esquemático de todas Barras da Rede
		lisBAR = rede_etd->LisBarra();
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			// salva coordenadas originais
			barra->esq_orig = barra->esq;
			// redefine coordenadas esq
			barra->esq.x1 = esq_x + ((barra->esq.x1 - area_esq->x1) * fat_escala);
			barra->esq.x2 = esq_x + ((barra->esq.x2 - area_esq->x1) * fat_escala);
			barra->esq.y1 = esq_y + ((barra->esq.y1 - area_esq->y1) * fat_escala);
			barra->esq.y2 = esq_y + ((barra->esq.y2 - area_esq->y1) * fat_escala);
		}
		// redefine coordenadas de todos Eqbar
		lisEQP->Clear();
		rede_etd->LisEqbar(lisEQP);
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqbar = (VTEqbar*)lisEQP->Items[n];
			eqbar->esq.dxbar *= fat_escala;
			eqbar->esq.dybar *= fat_escala;
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineCoordenadasUtmETD(VTRede *rede_etd, VTRede *rede_etd_ref)
{
	// variáveis locais
	int utm_x_ref, utm_y_ref;
	VTBarra *barra;
	VTArea *area_utm_ref;

	// obtém área utm da Rede de referência
	area_utm_ref = rede_etd_ref->AreaUtm_cm();
	// proteção: verifica se a Rede de referência tem coordenadas utm definidas
	if (area_utm_ref->x1 <= 0)
		return;
	// determina coordenadas de referência
	utm_x_ref = (area_utm_ref->x1 + area_utm_ref->x2) / 2;
	utm_y_ref = (area_utm_ref->y1 + area_utm_ref->y2) / 2;
	// monta lista c/ todas as Barras da Rede (incluindo Barras conectadas por Chave aberta)
	lisBAR->Clear();
	rede_etd->LisBarra(lisBAR, true);
	// redefine todas as Barras com a mesmas coordenadas utm
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		barra->utm.x = utm_x_ref;
		barra->utm.y = utm_y_ref;
	}
	// recalcula área da Rede
	rede_etd->CalculaArea();
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineCoordenadasUtmPRI_SEC(void)
{
	// variáveis locais
	double fat_x, fat_y;
	VTBarra *barra;
	VTRede *rede;
	VTArea *area_utm, *area_esq;

	// cria objetos locais
	area_esq = DLL_NewObjArea();
	area_utm = DLL_NewObjArea();
	// proteção: para redefinir coord.utm é necessário que exitam pelo menos 2 barras
	if (lisBAR_INI->Count < 2)
	{
		delete area_esq;
		delete area_utm;
		return;
	}
	// verifica se as Barras iniciais originais das Redes primárias já tem coord.utm definidas
	if (CalculaAreaUtm(lisBAR_INI, area_utm))
	{
		delete area_esq;
		delete area_utm;
		return;
	}
	// calcula área esquemático das Barras iniciais originais
	if (!CalculaAreaEsq(lisBAR_INI, area_esq))
	{
		delete area_esq;
		delete area_utm;
		return;
	}
	// reinicia lisBAR_INI com as Barras inciais atuais das Redes Primárias
	lisBAR_INI->Clear();
	for (int nr = 0; nr < lisPRI->Count; nr++)
	{
		rede = (VTRede*)lisPRI->Items[nr];
		if ((barra = rede->BarraInicial()) == NULL)
			continue;
		if (lisBAR_INI->IndexOf(barra) < 0)
			lisBAR_INI->Add(barra);
	}
	// calcula área utm das Barras iniciais atuais
	if (!CalculaAreaUtm(lisBAR_INI, area_utm))
		return;
	// calcula fator de escala entre as duas áreas
	fat_x = area_utm->Width() / area_esq->Width();
	fat_y = area_utm->Height() / area_esq->Height();
	// redefine coordendas UTM das Barras de todas redes primárias
	for (int nr = 0; nr < lisPRI->Count; nr++)
	{
		rede = (VTRede*)lisPRI->Items[nr];
		lisBAR->Clear();
		rede->LisBarra(lisBAR, true);
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			// verifica se as coordeandas utm da Barra já foram definidas
			if (barra->utm.x > 0)
				continue;
			// redefine coordenadas utm da Barra
			barra->utm.x = area_utm->x1 + (barra->esq.x1 - area_esq->x1) * fat_x;
			barra->utm.y = area_utm->y1 + (barra->esq.y1 - area_esq->y1) * fat_y;
			// redefine coordenadas esquemático da Barra
			barra->esq.x1 = barra->esq.x2 = barra->utm.x;
			barra->esq.y1 = barra->esq.y2 = barra->utm.y;
		}
	}
	// redefine coordendas UTM das Barras de todas redes secundárias
	for (int nr = 0; nr < lisSEC->Count; nr++)
	{
		rede = (VTRede*)lisSEC->Items[nr];
		lisBAR->Clear();
		rede->LisBarra(lisBAR, true);
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			// verifica se as coordeandas utm da Barra já foram definidas
			if (barra->utm.x > 0)
				continue;
			// redefine coordenadas utm da Barra
			barra->utm.x = area_utm->x1 + (barra->esq.x1 - area_esq->x1) * fat_x;
			barra->utm.y = area_utm->y1 + (barra->esq.y1 - area_esq->y1) * fat_y;
			// redefine coordenadas esquemático da Barra
			barra->esq.x1 = barra->esq.x2 = barra->utm.x;
			barra->esq.y1 = barra->esq.y2 = barra->utm.y;
		}
	}
	// recalcula área das Redes
	// redes->CalculaArea();
	// destrói objetos locais
	delete area_esq;
	delete area_utm;
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineDominioRede(VTRede *rede, VTRede *rede_ref)
{
	// variáveis locais
	VTDominio *dominio = NULL;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisDOMINIO = redes->LisDominio();

	// determina Dominio da rede_ref
	lisEQP->Clear();
	redes->LisDominio(lisEQP, rede_ref);
	if (lisEQP->Count > 0)
	{
		dominio = (VTDominio*)lisEQP->First();
	}
	else
	{ // assume primeiro Domínio disponível (na importação, existe um único Domínio)
		if (lisDOMINIO->Count > 0)
			dominio = (VTDominio*)lisDOMINIO->First();
	}
	if (dominio != NULL)
		dominio->InsereRede(rede);
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineEqptoRedeStatusNovo(VTRede *rede)
{
	// variáveis locais
	VTEqpto *eqpto;
	TList *lisEQP;

	try
	{ // cria lista temporária e inclui todos Eqptos da Rede
		lisEQP = new TList();
		rede->LisBarra(lisEQP, true);
		rede->LisEqbar(lisEQP);
		rede->LisLigacao(lisEQP);
		lisEQP->Add(rede);
		// redefine todos Eqptos da Rede como novos
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[n];
			eqpto->Id = FalsoId();
			eqpto->Status[sttNOVO] = true;
		}
		// destrói lista temporária
		delete lisEQP;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TIntegraEtd::RedefineTrafos(VTSubestacao *subestacao, VTBarra* barra_at,
	VTBarra* barra_mt)
{
	// variáveis locais
	TList *lisCLONE;
	VTRede *rede_etd;
	VTTrafo *trafo;
	double posbar;

	// proteção
	if (subestacao == NULL)
		return (false);
	if (barra_at == NULL)
		return (false);
	if (barra_mt == NULL)
		return (false);
	// determina rede da subestação
	rede_etd = subestacao->Rede;
	// obtem lista de trafos da subestação
	lisEQP->Clear();
	rede_etd->LisEqpto(lisEQP, eqptoTRAFO);
	// verifica se existem trafos na subestação
	if (lisEQP->Count == 0)
		return (false);
	// cria lista
	lisCLONE = new TList();
	// clona lista de trafos
	lisCLONE->Clear();
	ClonaEqptos(lisEQP, lisCLONE);
	for (int nt = 0; nt < lisCLONE->Count; nt++)
	{ // redefine barras do trafo
		trafo = (VTTrafo*)lisCLONE->Items[nt];
		trafo->DefineObjBarra(barra_at, barra_mt);
		trafo->RedefineBarraDoPrimario(barra_at);
		//define posição na barra
		posbar = double(nt / (lisCLONE->Count-1.)) * 100.;
		trafo->DefinePosBar(barra_at, posbar);
		trafo->DefinePosBar(barra_mt, posbar);
		// redefine tensões do trafo (para não dar conflito, já que estarão nas mesmas barras)
		trafo->pri.vnom = trafo->pri.tap = barra_at->vnom;
		trafo->sec.vnom = trafo->sec.tap = barra_mt->vnom;
	}
	// insere todos os trafos originais na lista de eqptos alterados
	CopiaTList(lisEQP, lisALT);
	// destroi lista
	delete lisCLONE;

	return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TIntegraEtd::RedefineTrafoZat(VTRede *rede)
{
	// variáveis locais
	double r_max = 0.1;
	double x_max = 0.1;
	VTTrafo *trafo;
	VTTrafo3E *trafo3e;
	TList *lisEQP;

	try
	{ // cria lista temporária
		lisEQP = new TList();
		// inicia lisEQP c/ todos os Trafos
		rede->LisLigacao(lisEQP, eqptoTRAFO);
		// redefine Zat de todos Trafos
		for (int n = 0; n < lisEQP->Count; n++)
		{
			trafo = (VTTrafo*)lisEQP->Items[n];
			if (trafo->pri.zterra.r > r_max)
				trafo->pri.zterra.r = r_max;
			if (trafo->pri.zterra.x > x_max)
				trafo->pri.zterra.x = x_max;
			if (trafo->sec.zterra.r > r_max)
				trafo->sec.zterra.r = r_max;
			if (trafo->sec.zterra.x > x_max)
				trafo->sec.zterra.x = x_max;
		}
		// inicia lisEQP c/ todos os Trafo3E
		lisEQP->Clear();
		rede->LisLigacao(lisEQP, eqptoTRAFO3E);
		// redefine Zat de todos Trafo3E
		for (int n = 0; n < lisEQP->Count; n++)
		{
			trafo3e = (VTTrafo3E*)lisEQP->Items[n];
			if (trafo3e->pri.zterra.r > r_max)
				trafo3e->pri.zterra.r = r_max;
			if (trafo3e->pri.zterra.x > x_max)
				trafo3e->pri.zterra.x = x_max;
			if (trafo3e->sec.zterra.r > r_max)
				trafo3e->sec.zterra.r = r_max;
			if (trafo3e->sec.zterra.x > x_max)
				trafo3e->sec.zterra.x = x_max;
			if (trafo3e->ter.zterra.r > r_max)
				trafo3e->ter.zterra.r = r_max;
			if (trafo3e->ter.zterra.x > x_max)
				trafo3e->ter.zterra.x = x_max;
		}
		// destrói lista temporária
		delete lisEQP;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TIntegraEtd::StringTrim(AnsiString txt)
{
	// variáveis locais
	AnsiString txt_alt;
	std::locale loc;
	int len = txt.Length();

	for (int n = 1; n <= len; n++)
	{
		if (isalnum(txt[n], loc))
			txt_alt += txt[n];
	}
	return (txt_alt);
}

// ---------------------------------------------------------------------------
// eof
