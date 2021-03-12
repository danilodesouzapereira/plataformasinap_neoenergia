// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TBuscaProt.h"
#include "TLigaCrit.h"
#include "VTLisBlocoChave.h"
#include "VTBarraRad.h"
#include "VTVisRede.h"
#include "VTBlocoRad.h"
#include "VTDefeito.h"

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Bloco\VTElo.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTReducao.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\DLL_Inc\Bloco.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// COMENTÁRIOS
// hk201608  OrdenaChaves - criacao de lisBlocoRad: Verifica se há múltiplos suprimentos, se sim elimina segundo ponto de geração (rede equiv, gerador, suprimento)
// hk201608  OrdenaLisBlocosGeracao: Método para ordenacao dos blocos de geracao em blocos ligados a
// redes equivalentes com suprimento, blocos com gerador e blocos com suprimento vteta
// hk201609: restricao em chave  : IA_Crit(VTVisRede *visRede, int indPat), if(ligaCrit == NULL) return(1000.0)
// hk201609: Restr.cap.Chaves: Inclusão de.. : Liga_Crit(VTChave *pchvVis, int indPat)
// hk201610: definição de rede de bloco: em blocoRad->Rede no caso de bloco formado por 1 única barra: inclusao de apl//new TBlocoRad(blocoSup));
// hk201610 - temporário para estudo EDRO: limitação de área de religamento baseado em código específico de chave da EDRO

// hk201611
// hk201611: delecao de fontes secundarias em paralelo
// hk201611: criacao de ponteiro de blocoRad para plisBlocoChave a montante
// hk201611: caso de lisDJPri->Count > 0: eliminação de suprimentos acima de disjuntor
// ---------------------------------------------------------------------------
VTBuscaProt* __fastcall NewObjBuscaProt(VTApl* apl, bool flagCalc)
{
	return (new TBuscaProt(apl, flagCalc));
}

// ---------------------------------------------------------------------------
__fastcall TBuscaProt::TBuscaProt(VTApl *apl, bool flagCalc)
{
	// variáveis locais
	// salva ponteiro dos objetos
	this->apl = apl;

	lisBlocoRad = new TList();
	lisBlocoChv = new TList();

	lisVisRede = new TList();
	lisChavesVis = new TList();

	lisChavesMed = new TList();

	// lista de disjuntores inicio dos alimentadores primários
	lisDjPri = new TList();
	// Lista de chaves automáticas
	lisChvAuto = new TList();
	// lista com tipos de equipamentos de protecao
	lisTipoEqProt = new TList();
   //lista de objetos strIF, para cálculo de ganhos em duração de localização:hk201710IF
   lisEQIF      = new TList();
   //lista de chaves com IF instalado:hk201710IF
   lisChavesIF  = new TList();
	// Inicia lista com tipos de chave de proteção
	// hk18 IniciaTipoProt();

	this->flagCalc = flagCalc;

	if (flagCalc)
	{
		// Busca objeto da classe blocos criado anteriormente por calcIndCont
		blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	}
	else
	{
		// Cria blocos
		apl->Add(blocos = DLL_NewObjBlocos()); // hk2005 acrescenta em apl
	}

	// Seta flag de inicialização falso
	flagInicia = false;

	// Inicia objeto para gerar lista de barras ordenadas
	barraRad = NULL;

	// Inicia tipos de chave religadora e disjuntor
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	tipoReligador = tipos->ExisteTipoChave(chaveRA);
	tipoDisjuntor = tipos->ExisteTipoChave(chaveDJ);

	// Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto)
	// hkedp1 lisChavesAtivas =  NULL;
	// Inicia lista de chaves ativas, lisChavesAtivas
	lisChavesAtivas = new TList();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	redes->LisEqpto(lisChavesAtivas, eqptoCHAVE);

	iProfReliga = 1;
	// sensibilidade de religamento: =0, somente o bloco imediatamente a jusante: =1, até o próximo religador
    iProfDescoord  = 0; //hk201711: taxa de descoordenação de proteção: 0, protecao ajustada, n>0: sensibilização do n além da proteção imediatamente a montante do defeito
}

// ---------------------------------------------------------------------------
__fastcall TBuscaProt::~TBuscaProt(void)
{
	if (lisTipoEqProt)
	{
		delete lisTipoEqProt;
		lisTipoEqProt = NULL;
   //hk201710IF
   for(int n = 0; n < lisEQIF->Count; n++)
      {
      delete ((strIF*)lisEQIF->Items[n])->lisBlJus;
      delete ((strIF*)lisEQIF->Items[n])->lisBlProt;
	}
	if(lisEQIF) {LimpaTList(lisEQIF); delete lisEQIF; lisEQIF = NULL;}

	if(lisChavesIF) {delete lisChavesIF; lisChavesIF = NULL;}
	}
if (lisChavesMed)
	{
		delete lisChavesMed;
		lisChavesMed = NULL;
	}
	if (lisChavesVis)
	{
		delete lisChavesVis;
		lisChavesVis = NULL;
	}
	if (lisChavesAtivas)
	{
		delete lisChavesAtivas;
		lisChavesAtivas = NULL;
	} // hkedp1
	if (lisDjPri)
	{
		delete lisDjPri;
		lisDjPri = NULL;
	}
	if (lisChvAuto)
	{
		delete lisChvAuto;
		lisChvAuto = NULL;
	}
	if (lisBlocoChv)
	{
		LimpaTList(lisBlocoChv);
		delete lisBlocoChv;
		lisBlocoChv = NULL;
	}
	if (lisBlocoRad)
	{
		LimpaTList(lisBlocoRad);
		delete lisBlocoRad;
		lisBlocoRad = NULL;
	}
	if (lisVisRede)
	{
		LimpaTList(lisVisRede);
		delete lisVisRede;
		lisVisRede = NULL;
	}
	if (barraRad)
	{
		delete barraRad;
		barraRad = NULL;
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ChavesSerieMax(VTRede *rede, TList *lisTipoChaves, int ncLim,
	AnsiString &chaveSerie)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTBlocoRad *blocoRad;
	TList *lisChaves;
	VTLisBlocoChave *lisBlocoChave;
	VTChave *chave;
	int ichv;
	bool resposta = true;
	// -----------------------------------------------------------------------------------------------
	// Verifica se na rede existem casos de chaves de tipos específicos em série acima do limite ncLim
	// Retorna falso se existem casos acima de ncLim
	// e true caso contrário
	// -----------------------------------------------------------------------------------------------
	lisChaves = new TList();
	rede->LisEqpto(lisChaves, eqptoCHAVE);
	// Exclui chaves com tipos nao existentes em lisTipoChaves
	for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
	{
		chave = (VTChave*)lisChaves->Items[nc];
		if (chave->Aberta) // if(chave->Estado == chvABERTA)//hkRevAloca
		{
			lisChaves->Delete(nc);
			continue;
		}

		if (lisChavesAtivas->IndexOf(chave) == -1)
		{
			lisChaves->Delete(nc);
			continue;
		}

		if (lisTipoChaves != NULL)
		{
			if (lisTipoChaves->IndexOf(chave->TipoChave) != -1)
				continue;
		}
		else
		{
			if ((chave->TipoChave == tipoReligador) || (chave->TipoChave == tipoDisjuntor))
				continue;
		}

		lisChaves->Delete(nc);
	}
	// Para cada chave busca numero de chaves em serie a montante
	for (int nc = 0; nc < lisChaves->Count; nc++)
	{
		chave = (VTChave*)lisChaves->Items[nc];
		lisBlocoChave = IdentificaBlocoChave(chave);
		if (lisBlocoChave == NULL)
		{
			continue;
		}

		ichv = 1;
		chaveSerie.sprintf("%s ", chave->Codigo.c_str());
		blocoRad = lisBlocoChave->blMon;
		while (blocoRad->BlocoRadMon != NULL)
		{
			if (lisChaves->IndexOf(blocoRad->Chave) != -1)
			{
				ichv++;
				chaveSerie.cat_sprintf("%s ", blocoRad->Chave->Codigo.c_str());
			}
			blocoRad = blocoRad->BlocoRadMon;
		}
		if (ichv > ncLim)
		{
			resposta = false;
			break;
		}
	}

	delete lisChaves;
	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::CalculaCargasBlocoRad(int iPat)
{
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	TList *lisBlocos, *lisCargas, *lisBarra;
	VTBloco *bloco;
	VTCarga *carga;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int numPat = patamares->NumPatamar();
	VTBlocoRad *blocoRad, *blocoRadAux;
	TList *lisBlRadTerm, *lisBlocoRadTot;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	lisCargas = new TList();
	// Inicia cargas por blocos radiais, blocoRad
	lisBlocoRadTot = new TList();
	ListaBlocoRadTotal(lisBlocoRadTot);
	for (int nb = 0; nb < lisBlocoRadTot->Count; nb++)
	{
		blocoRad = (VTBlocoRad*)lisBlocoRadTot->Items[nb];
		lisCargas->Clear();
		blocoRad->Bloco->LisEqbar(lisCargas, eqptoCARGA);
		blocoRad->IniciaCarga(numPat);
		blocoRad->NCons = 0;//hk201902
		for (int nc = 0; nc < lisCargas->Count; nc++)
		{
			carga = (VTCarga*)lisCargas->Items[nc];
			blocoRad->NCons += NCons(carga);
			for (int np = 0; np < numPat; np++)
				blocoRad->CargaBloco[np] += demanda->P_mw[carga][np];
		}
	}

	// Inicia cargas por blocos radiais
	// lisBlocoRadTot = new TList();
	// buscaProt->ListaBlocoRadTotal(lisBlocoRadTot);
	for (int nb = 0; nb < lisBlocoRadTot->Count; nb++)
	{
		blocoRad = (VTBlocoRad*)lisBlocoRadTot->Items[nb];
		for (int np = 0; np < numPat; np++)
			blocoRad->CargaBlRad[np] += blocoRad->CargaBloco[np];
		blocoRad->NConsJus = blocoRad->NCons; //hk201902
		for (int nj = 0; nj < blocoRad->LisBlocoJus->Count; nj++) //PEND: MUDAR PARA blocoRad->LisBlocoRadJus
		{
			bloco = (VTBloco*)blocoRad->LisBlocoJus->Items[nj];
			blocoRadAux = IdentificaBlocoRad(bloco);
			if (blocoRadAux == NULL)
			{
				int ii = 0;
				continue;
			}
			blocoRad->NConsJus += blocoRadAux->NCons;   //hk201902
			for (int np = 0; np < numPat; np++)
				blocoRad->CargaBlRad[np] += blocoRadAux->CargaBloco[np];
		}
	}
	delete lisCargas;
	delete lisBlocoRadTot;
	return (true);
}

//---------------------------------------------------------------------------
int __fastcall TBuscaProt::NCons(VTCarga *carga)
   { //hk201902
   int naux  = 0;

   naux += carga->NumConsResidencial;
   naux += carga->NumConsComercial;
   naux += carga->NumConsIndustrial;
   naux += carga->NumConsRural;
   naux += carga->NumConsOutros; //MISTURADO COM IP!!!!
   naux += carga->NumConsA4;
   //Valor mínimo de consumidores
	if (naux == 0)
		naux = 1;

   return(naux);
   }

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaZonaProt(VTChave *pchv, TList *lisBlocoExt)
{
	VTBlocoRad *blocoRad, *blocoRadProt;
	TList *lisAux;
	bool resposta;
	// Funcao que preenche lista de blocos da chave até a ilha de protecao a montante
	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}
	// Inicia
	lisBlocoExt->Clear();

	// Identifica blocoRad a jusante da chave
	if ((blocoRad = IdentificaBlocoRad(pchv)) == NULL)
		return (false);
	// Identifica a primeira chave de protecao a montante
	blocoRad = blocoRad->BlocoRadMon;
	while (blocoRad != NULL)
	{
		if (ChaveProtecao(blocoRad->Chave))
			break;
		// hkind if(lisTipoEqProt->IndexOf(blocoRad->Chave->TipoChave) != -1) break;
		blocoRad = blocoRad->BlocoRadMon;
	}
	if (blocoRad == NULL)
		return (false);

	// preenche lista com blocos a justante da chave de protecao
	// até as chaves de protecao seguintes
	resposta = ListaZonaProtJus(blocoRad, lisBlocoExt, NULL, false);//hk201802a

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaZonaProtJus(VTBlocoRad *blocoRad, TList *lisBlocoExt, TList *lisChavesExt, bool flagIF, bool flagBloco) //hk201710IF
{
	TList *lisAux;

	///////////////////////////////////////////////////////////////////////
	//funcao que preenche lista de blocos entre chave e blocos a jusante até as proximas chaves com protecao///
   //se flagIF = true: entao até a chave contendo IF : hk201710IF
	///////////////////////////////////////////////////////////////////////
	// if(!flagInicia)
	// {if(!Inicia()) return(false);}

	if(flagBloco) lisBlocoExt->Add(blocoRad->Bloco);
   else lisBlocoExt->Add(blocoRad);

	lisAux = new TList();
	lisAux->Assign(blocoRad->LisBlocoRad, laOr);
	while (lisAux->Count > 0)
	{
		blocoRad = (VTBlocoRad*)lisAux->Items[0];
		lisAux->Delete(0);
		// hkind if(lisTipoEqProt->IndexOf(blocoRad->Chave->TipoChave) != -1)
		// Verifica se chave de protecao e se a chave esta no conjunto de ativas, lisChavesAtivas
		if (ChaveProtecao(blocoRad->Chave))
		{
			if (lisChavesExt != NULL)
				lisChavesExt->Add(blocoRad->Chave);
			continue;
		}
        if(flagIF)
		    {if(lisChavesIF->IndexOf(blocoRad->Chave) != -1)
			    {
			    if(lisChavesExt != NULL) lisChavesExt->Add(blocoRad->Chave);
			    continue;
			    }
          }
		if(flagBloco) lisBlocoExt->Add(blocoRad->Bloco);
      else lisBlocoExt->Add(blocoRad);
		lisAux->Assign(blocoRad->LisBlocoRad, laOr);
	}
	delete lisAux;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaZonaReligaJus(VTBlocoRad *blocoRad, TList *lisBlocoExt,
	TList *lisChavesExt) // , TList *lisBarrasExt)
{
	TList *lisAux;
	VTChave *chave;

	// if(!flagInicia)
	// {if(!Inicia()) return(false);}

	lisBlocoExt->Add(blocoRad->Bloco);
	// se profundidade de religamento corresponde somente ao primeiro bloco a jusante do religador
	if (iProfReliga == 0)
		return (true);

	lisAux = new TList();
	lisAux->Assign(blocoRad->LisBlocoRad, laOr);
	while (lisAux->Count > 0)
	{
		blocoRad = (VTBlocoRad*)lisAux->Items[0];
		chave = blocoRad->Chave;
		lisAux->Delete(0);
		// if(lisTipoEqProt->IndexOf(blocoRad->Chave->TipoChave) != -1)
		// hkInd if((chave->TipoChave->Tipo == chaveRELIGADORA)||(chave->TipoChave->Tipo == chaveDISJUNTOR))
		if (ChaveReligadora(chave))
		{
			if (lisChavesExt != NULL)
				lisChavesExt->Add(chave);
			continue;
		}
		lisBlocoExt->Add(blocoRad->Bloco);

		lisAux->Assign(blocoRad->LisBlocoRad, laOr);
	}
	delete lisAux;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBarChaveZonaProt(VTChave *pchave, TList *lisBarra)
{
	TList *lisBlocos, *lisChavesZonaProt;
	VTBloco *bloco;
	VTChave *chaveAux;
	VTBlocoRad *blocoRad;

	lisBarra->Clear();

	lisBlocos = new TList();
	lisChavesZonaProt = new TList();

	ListaZonaProtJus(pchave, lisBlocos, lisChavesZonaProt);

	// Monta lista de barras, lisBarra com barras de chaves de protecao
	for (int nc = 0; nc < lisChavesZonaProt->Count; nc++)
	{
		chaveAux = (VTChave*)lisChavesZonaProt->Items[nc];
		// verifica qual a barra de chave entra em lisBarra
		for (int nb = 0; nb < lisBlocos->Count; nb++)
		{
			bloco = (VTBloco*)lisBlocos->Items[nb];
			if (bloco->LisBarra()->IndexOf(chaveAux->pbarra1) != -1)
			{
				lisBarra->Add(chaveAux->pbarra1);
				break;
			}
			if (bloco->LisBarra()->IndexOf(chaveAux->pbarra2) != -1)
			{
				lisBarra->Add(chaveAux->pbarra2);
				break;
			}
		}
	}

	// Preenche lisBarra com barras extremas com todos os blocos da zona de protecao
	// incluindo barras de chaves faca ao longo da zona de protecao
	for (int nb = 0; nb < lisBlocos->Count; nb++)
	{
		bloco = (VTBloco*)lisBlocos->Items[nb];
		blocoRad = IdentificaBlocoRad(bloco);
		LisBarChave(blocoRad, lisBarra);
	}
	// Retira barras de chaves faca intermediarias da zona de protecao, excluindo
	// as definidas em lisBarra (para o caso esquisito de chaves em série)
	for (int nb = 0; nb < lisBlocos->Count; nb++)
	{
		bloco = (VTBloco*)lisBlocos->Items[nb];
		blocoRad = IdentificaBlocoRad(bloco);
		// Verifica se a chave de suprimento é do tipo chave faca
		// não pertencie a lisTipoEqProt
		// hkind if(lisTipoEqProt->IndexOf(blocoRad->Chave->TipoChave) == -1)
		if (ChaveProtecao(blocoRad->Chave))
		{
			lisBarra->Remove(blocoRad->Chave->pbarra1);
			lisBarra->Remove(blocoRad->Chave->pbarra2);
		}
	}

	delete lisBlocos;
	delete lisChavesZonaProt;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaBlocoRadZonaProtJus(VTChave *pchv, TList *lisBlocoRadExt)//hk201802a
{ // parou acrescentar lista de chaves ativas
	VTBlocoRad *blocoRad;
	bool resposta;

	///////////////////////////////////////////////////////////////////////
	// funcao que preenche lista de blocosRad entre chave e blocos a jusante até as proximas chaves com protecao///
	///////////////////////////////////////////////////////////////////////
	// Inicia
	lisBlocoRadExt->Clear();

	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	// Identifica blocoRad a jusante da chave
	if ((blocoRad = IdentificaBlocoRad(pchv)) == NULL)
		return (false);
   //hk201802a: popula lisBlocoRadExt com blocoRad´s
//ListaZonaProtJus(VTBlocoRad *blocoRad, TList *lisBlocoExt, TList *lisChavesExt, bool flagIF, bool flagBloco) //hk201710IF

	resposta = ListaZonaProtJus(blocoRad, lisBlocoRadExt, NULL, false, false); //hk201802a
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaZonaProtJus(VTChave *pchv, TList *lisBlocoExt,
	TList *lisChavesExt) // , TList *lisBarrasExt)
{ // parou acrescentar lista de chaves ativas
	VTBlocoRad *blocoRad;
	bool resposta;

	///////////////////////////////////////////////////////////////////////
	// funcao que preenche lista de blocos entre chave e blocos a jusante até as proximas chaves com protecao///
	///////////////////////////////////////////////////////////////////////
	// Inicia
	lisBlocoExt->Clear();

	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	// Identifica blocoRad a jusante da chave
	if ((blocoRad = IdentificaBlocoRad(pchv)) == NULL)
		return (false);
	resposta = ListaZonaProtJus(blocoRad, lisBlocoExt, lisChavesExt, false); //hk201802a
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaZonaProtIFJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt)//, TList *lisBarrasExt)
	{ // parou acrescentar lista de chaves ativas
	VTBlocoRad  *blocoRad;
	bool resposta;

	///////////////////////////////////////////////////////////////////////
	//funcao que preenche lista de blocos entre chave e blocos a jusante até
   //as proximas chaves com protecao ou chaves com identificador de falta, IF instalado
	///////////////////////////////////////////////////////////////////////
	//Inicia
	lisBlocoExt->Clear();

	if(!flagInicia)
	  {if(!Inicia()) return(false);}

	//Identifica blocoRad a jusante da chave
	if((blocoRad = IdentificaBlocoRad(pchv)) == NULL) return(false);
	resposta = ListaZonaProtJus(blocoRad, lisBlocoExt, lisChavesExt, true);//true: limitado por IFs
	return(true);
	}
//---------------------------------------------------------------------------
TList* __fastcall TBuscaProt::ListaZonaReliga(VTChave *pchv)
{
	VTLisBlocoChave *lisBlocoChave;

	// Busca zona de religamento iniciada através de ReiniciaZonaReliga
	if (pchv == NULL)
		return (NULL);
	if ((lisBlocoChave = IdentificaBlocoChave(pchv)) == NULL)
		return (NULL);

	return (lisBlocoChave->lisBlReliga);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ListaZonaReligaJus(VTChave *pchv, TList *lisBlocoExt,
	TList *lisChavesExt) // , TList *lisBarrasExt)
{
	VTBlocoRad *blocoRad;

	bool resposta;

	///////////////////////////////////////////////////////////////////////
	// funcao que preenche lista de blocos entre chave e blocos a jusante ate a proxima chave com religamento///
	///////////////////////////////////////////////////////////////////////
	// se iProfReliga = 0 retorna o bloco imediato a jusante da chave
	///////////////////////////////////////////////////////////////////////

	// Inicia
	lisBlocoExt->Clear();

	// Identifica blocoRad a jusante da chave
	if ((blocoRad = IdentificaBlocoRad(pchv)) == NULL)
		return (false);
	resposta = ListaZonaReligaJus(blocoRad, lisBlocoExt, lisChavesExt); // , lisBarrasExt);
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::IdentificaChvMont(VTBloco *bloco, TList *lisExt, bool flagProt)
{
	IdentificaChvMontante((TObject*)bloco, lisExt, flagProt);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::IdentificaChvMont(VTEqpto *eqpto, TList *lisExt, bool flagProt)
{
	IdentificaChvMontante((TObject*)eqpto, lisExt, flagProt);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::IdentificaChvMontante(TObject *pobj, TList *lisExt, bool flagProt)
{
	VTBlocoRad *blocoRad;
	AnsiString obj_name;
	TMetaClass* meta_class;
	/////////////////////////////////////////////////////////////////////////////
	// Funcao que identifica lista de chaves a montante                         //
	// se flagProt = true, somente chaves de lisTipoEqProt, de protecao         //
	/////////////////////////////////////////////////////////////////////////////
	if (lisExt != NULL)
		{
		lisExt->Clear();
		}

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	blocoRad = NULL;
	meta_class = pobj->ClassType();
	obj_name = (meta_class->ClassParent())->ClassName(); // pobj->ClassName();
	if((obj_name.AnsiCompare("VTEqpto") == 0)||(obj_name.AnsiCompare("VTChave") == 0))
	{
		if ((blocoRad = IdentificaBlocoRad((VTEqpto*)pobj)) == NULL)
			return;
	}
	else
	{
		if (obj_name.AnsiCompare("VTBloco") == 0)
		{
			if ((blocoRad = IdentificaBlocoRad((VTBloco*)pobj)) == NULL)
				return;
		}
		else
			return;
	}

	while (blocoRad != NULL)
	{
		if (lisChavesAtivas != NULL) // Inclusao de lischavesAtivas hk03LisBlocosAgrega
		{
			if (lisChavesAtivas->IndexOf(blocoRad->Chave) == -1)
			{
				blocoRad = blocoRad->BlocoRadMon;
				continue;
			}
		} // Inclusao de lischavesAtivas hk03LisBlocosAgrega
		if (blocoRad->Chave != NULL)
		{
			if (flagProt)
			{ // hkind if(lisTipoEqProt->IndexOf(blocoRad->Chave->TipoChave) != -1)
				if (ChaveProtecao(blocoRad->Chave))
					lisExt->Add(blocoRad->Chave);
			}
			else
				lisExt->Add(blocoRad->Chave);
		}
		blocoRad = blocoRad->BlocoRadMon;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::Inicia(bool flagExeBlocos) // (bool flagExeBlocos = true)
{
	bool resposta = true;
	/////////////////////////////////////////////////////////////////
	// Função que gera:                                           ////
	// se flagExeBlocos = true --> blocos->Executa(redes)       ////
	// lisBlocoRad->blocoRad de suprimento                      ////
	// lisBlocoChv->TLisBlocoChave : ordenação de chaves        ////
	// Cria lista de blocosRad a jusante dos blocos             ////
	// Monta lista de chaves VIS entre circuitos                ////
	/////////////////////////////////////////////////////////////////
	if (!OrdenaChaves(flagExeBlocos))
		return (false); // flagExeBlocos-->blocos->Executa(), resposta = false;

	// Seta flag de inicialização
	if (resposta)
		flagInicia = true;

	// Reinicia zonas de religamento por chave religadora
	// Preenche lisBlocoChave->lisBlReliga para lisBlocoChave->pchv com funcao de religamento
	// ChaveReligadora(lisBlocoChave->pchv)) :
	// ListaZonaReligaJus(lisBlocoChave->pchv, lisBlocoChave->lisBlReliga, NULL);
	ReiniciaZonaReliga();

	// Função que monta:
	// lisVisRede->TVisRede->pchave, (rede1, rede2), (bloco1, bloco2)
	// if(!MontaLisChavesVis()) resposta = false;

	// Funcao que gera para lisVisRede->TVisRede:
	// lista de ligacoes radiais até o suprimento: TVisRede->lisLiga(rede1), lisLiga(rede2)
	// if(!MontaLisLigaVis()) resposta = false;

	return (resposta);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::ReiniciaZonaReliga(void)
{
	VTLisBlocoChave *lisBlocoChave;

	for (int nl = 0; nl < lisBlocoChv->Count; nl++)
	{
		lisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nl];
		lisBlocoChave->lisBlReliga->Clear();
		if (!ChaveReligadora(lisBlocoChave->pchv))
			continue;
		ListaZonaReligaJus(lisBlocoChave->pchv, lisBlocoChave->lisBlReliga, NULL);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisBlocosVis(VTRede *rede, TList *lisBlocoExt)
{
	VTVisRede *visRede;
	VTBloco *blocoRede;
	VTBlocoRad *blocoRad;
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche lista de blocos de suprimento a montante das chaves de vis ligadas     //
	// a rede                                                                                     //
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Limpa lista externa
	lisBlocoExt->Clear();

	for (int nv = 0; nv < lisVisRede->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRede->Items[nv];
		if ((visRede->rede1 != rede) && (visRede->rede2 != rede))
			continue;
		// define blocoRede
		if (visRede->rede1 == rede)
			blocoRede = visRede->bloco1;
		else
			blocoRede = visRede->bloco2;

		if ((blocoRad = IdentificaBlocoRad(blocoRede)) == NULL)
			continue;

		do
		{
			if (lisBlocoExt->IndexOf(blocoRad->Bloco) == -1)
				lisBlocoExt->Add(blocoRad->Bloco);
			blocoRad = blocoRad->BlocoRadMon;
		}
		while (blocoRad != NULL);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisChvJus(VTBloco *bloco, VTChave *chvMont, TList *lisChvJus)
{
	TList *lisEloDef;
	VTElo *elo;
	////////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche lisChvJus com as chaves ligadas ao bloco excetuando a chave //
	// de suprimento                                                                   //
	////////////////////////////////////////////////////////////////////////////////////

	lisEloDef = new TList();
	blocos->LisElo(bloco, lisEloDef);
	lisChvJus->Clear();
	for (int ne = 0; ne < lisEloDef->Count; ne++)
	{
		elo = (VTElo*)lisEloDef->Items[ne];
		if (elo->Chave == chvMont)
			continue;
		lisChvJus->Add(elo->Chave);
	}
	delete lisEloDef; // novo
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisChavesVis(VTRede *rede, TList *lisChaveExt)
{
	VTVisRede *visRede;
	VTBloco *blocoRede;
	VTBlocoRad *blocoRad;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	///////////////////////////////////////////////////////////////////////
	// Funcao que preenche lista de chaves das chaves NA, e NA e inclusive,/
	// a rede até o suprimento da rede                                    //
	///////////////////////////////////////////////////////////////////////
	// Limpa lista externa
	lisChaveExt->Clear();
	for (int nv = 0; nv < lisVisRede->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRede->Items[nv];
		if (visRede->rede1 == visRede->rede2)
			continue;
		// Busca visRede conectada a rede
		if ((visRede->rede1 != rede) && (visRede->rede2 != rede))
			continue;
		// define blocoRede
		if (visRede->rede1 == rede)
			blocoRede = visRede->bloco1;
		else
			blocoRede = visRede->bloco2;
		if ((blocoRad = IdentificaBlocoRad(blocoRede)) == NULL)
			continue;
		// Adiciona chave NA à lista
		lisChaveExt->Add(visRede->pchave);
		do
		{
			if ((blocoRad->Chave != NULL) && (lisChaveExt->IndexOf(blocoRad->Chave) == -1))
			{
				lisChaveExt->Add(blocoRad->Chave);
			}
			blocoRad = blocoRad->BlocoRadMon;
		}
		while (blocoRad != NULL);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisChavesProt(VTRede *rede, TList *lisChaveExt)
{
	VTLisBlocoChave *pblChave;
	VTBlocoRad *blocoRad;
	/////////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche lista de chaves de todas as Chaves NF de protecao,           //
	// da rede até o suprimento                                                         //
	// Elimina chaves de protecao BF em série (2 em série, elimina a mais a jusante)    //
	/////////////////////////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	// Versao que preenche lista de cahves com chaves
	// de qualquer chave fechada, não protecao, até o suprimento
	/* lisChaveExt->Clear();
	 for(int n = 0; n < lisBlocoChv->Count; n++)
	 {
	 pblChave = (VTLisBlocoChave*)lisBlocoChv->Items[n];
	 if(pblChave->pchv->Aberta) continue;  //chave fechada
	 if(pblChave->pchv->rede != rede) continue;  //da rede
	 //hkind if(lisTipoEqProt->IndexOf(pblChave->pchv->TipoChave) == -1) continue;
	 if(ChaveProtecao(pblChave->pchv)) continue; //chave sem funcao protecao
	 if(lisChaveExt->IndexOf(pblChave->pchv) != -1) continue;
	 blocoRad = pblChave->blMon;
	 do
	 {
	 if((blocoRad->Chave != NULL)&&(lisChaveExt->IndexOf(blocoRad->Chave) == -1))
	 lisChaveExt->Add(blocoRad->Chave);
	 blocoRad = blocoRad->BlocoRadMon;
	 }
	 while(blocoRad != NULL);
	 }
	 */
	// Versao que preenche lista de cahves com chaves
	// de qualquer chave protecao fechada
	lisChaveExt->Clear();
	for (int n = 0; n < lisBlocoChv->Count; n++)
	{
		pblChave = (VTLisBlocoChave*)lisBlocoChv->Items[n];
		if (pblChave->pchv->Aberta)
			continue; // chave fechada
		if (pblChave->pchv->rede != rede)
			continue; // da rede

		// hkind if(lisTipoEqProt->IndexOf(pblChave->pchv->TipoChave) == -1) continue;
		if (!ChaveProtecao(pblChave->pchv))
			continue; // chave sem funcao protecao

		if (lisChaveExt->IndexOf(pblChave->pchv) != -1)
			continue;
		// Inclui chave de protecao inicial
		// lisChaveExt->Add(pblChave->pchv);//hkrev0626
		blocoRad = pblChave->blMon;
		do
		{
			if ((blocoRad->Chave != NULL) && (lisChaveExt->IndexOf(blocoRad->Chave) == -1))
				lisChaveExt->Add(blocoRad->Chave);
			blocoRad = blocoRad->BlocoRadMon;
		}
		while (blocoRad != NULL);
	}

	// Elimina chaves de protecao BF em série (2 em série, elimina a mais a jusante)
	TList *lisAux;
	lisAux = new TList();
	for (int n = 0; n < lisBlocoChv->Count; n++)
	{
		pblChave = (VTLisBlocoChave*)lisBlocoChv->Items[n];
		if (pblChave->pchv->Aberta)
			continue; // chave fechada
		if (pblChave->pchv->rede != rede)
			continue; // da rede
		if (pblChave->blJus == NULL)
			lisAux->Add(pblChave->blJus);
		else
		{
			if (pblChave->blJus->LisBlocoRad->Count == 0)
				lisAux->Add(pblChave->blJus);
		}
	}
	TList *lisFusiveis;
	lisFusiveis = new TList();
	for (int n = 0; n < lisAux->Count; n++)
	{
		blocoRad = (VTBlocoRad*)lisAux->Items[n];
		do
		{ // tipoChave->Tipo ==chaveBF
			if (blocoRad->Chave != NULL)
			{
				if (blocoRad->Chave->TipoChave->Tipo != chaveBF)
					lisFusiveis->Clear();
				else
					lisFusiveis->Add(blocoRad->Chave);

				if (lisFusiveis->Count == 2) // hkver0626 de 2 para 3
				{
					lisChaveExt->Remove(lisFusiveis->First());
					lisFusiveis->Delete(0);
				}
			}
			blocoRad = blocoRad->BlocoRadMon;
		}
		while (blocoRad != NULL);
	}
	delete lisAux;
	delete lisFusiveis;
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TBuscaProt::BuscaChaveMon(VTBloco *bloco)
{ // hk000
	VTBlocoRad *blocoRad;
	VTChave *pchave = NULL;
	////////////////////////////////////////////////////////
	// Funcao que retorna chave de suprimento do bloco
	//////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}

	if ((blocoRad = IdentificaBlocoRad(bloco)) != NULL)
		pchave = blocoRad->Chave;

	return (pchave);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::ListaOrdenadaBlocos(TList *lisBlocoExt, VTRede *rede)
{
	TList *lisBloco;
	VTBlocoRad *blocoRad;
	VTBloco *bloco;
	/////////////////////////////////////////////////////////////////////
	// Funcao que preenche lista de blocos ordenadas da montante a jusante
	/////////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	lisBlocoExt->Clear();
	for (int n = 0; n < lisBlocoRad->Count; n++)
	{
		blocoRad = (VTBlocoRad*)lisBlocoRad->Items[n];
		lisBlocoExt->Add(blocoRad->Bloco);
		lisBlocoExt->Assign(blocoRad->LisBlocoJus, laOr);
		if (rede != NULL)
		{
			for (int nb = lisBlocoExt->Count - 1; nb >= 0; nb--)
			{
				bloco = (VTBloco*)lisBlocoExt->Items[nb];
				if (bloco->Rede == rede)
					continue;
				lisBlocoExt->Delete(nb);
			}
		}
	}

	/*
	 TList *lisAux;
	 lisAux = new TList();
	 //Para cada blocoRad de suprimento faz a busca
	 lisBlocoExt->Clear();
	 for(int n = 0; n < lisBlocoRad->Count; n++)
	 {
	 blocoRad = (VTBlocoRad*)lisBlocoRad->Items[n];
	 lisAux->Clear();

	 lisAux->Add(blocoRad);
	 while(lisAux->Count > 0)
	 {
	 blocoRad = (VTBlocoRad*)lisAux->First();
	 if((rede == NULL)||(blocoRad->Bloco->Rede == rede)) lisBlocoExt->Add(blocoRad->Bloco);
	 lisAux->Delete(0);
	 lisAux->Assign(blocoRad->LisBlocoRad, laOr);
	 }
	 }
	 delete lisAux;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::IdentificaLisBlocosRadJusante(TList *lisBlocoDef, TList *lisEXT)
{
	VTBloco *bloco;
	VTBlocoRad *blocoRad;
	TList *lisAux;
	////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche lisEXT com blocosRad a jusante dos blocos de lisBlocoDef
	////////////////////////////////////////////////////////////////////////////////
	lisAux = new TList();
	for (int nb = 0; nb < lisBlocoDef->Count; nb++)
	{
		bloco = (VTBloco*)lisBlocoDef->Items[nb];
		blocoRad = IdentificaBlocoRad(bloco);
		lisAux->Add(blocoRad);
		lisEXT->Assign(blocoRad->LisBlocoRad, laOr);
	}
	for (int nb = 0; nb < lisAux->Count; nb++)
		lisEXT->Remove(lisAux->Items[nb]);
	delete lisAux;
}

// ---------------------------------------------------------------------------
VTBlocoRad* __fastcall TBuscaProt::IdentificaBlocoRad(VTBloco *bloco)
{
	TList *lisBloco;
	VTBlocoRad *blocoRad = NULL;
	///////////////////////////////////////////////////
	// Funcao que identifica blocoRad->bloco = bloco  //
	///////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(NULL);}

	TList *lisAux;
	lisAux = new TList();
	// Para cada blocoRad de suprimento faz a busca
	for (int n = 0; n < lisBlocoRad->Count; n++)
	{
		blocoRad = (VTBlocoRad*)lisBlocoRad->Items[n];
		lisAux->Clear();
		lisAux->Add(blocoRad);
		while (lisAux->Count > 0)
		{
			blocoRad = (VTBlocoRad*)lisAux->First();
			if (blocoRad->Bloco == bloco)
				break;
			lisAux->Delete(0);
			lisAux->Assign(blocoRad->LisBlocoRad, laOr);
			blocoRad = NULL;
		}
		if (blocoRad != NULL)
			break;
		blocoRad = NULL;
	}
	delete lisAux;

	return (blocoRad);

}

// ---------------------------------------------------------------------------
VTBlocoRad* __fastcall TBuscaProt::IdentificaBlocoRad(VTEqpto *eqpto)
{
	VTBloco *bloco;
	int indChave;
	VTLisBlocoChave *lisBlChave;
	VTBlocoRad *blocoRad = NULL;
	VTBlocoRad *blSup;
	TList *lisAux;

	////////////////////////////////////////////////////////
	// Funcao que identifica blocoRad->bloco contém eqpto  //
	////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(NULL);}

	if (eqpto->Tipo() == eqptoCHAVE)
	{
		// PEND: e se for chave NA??

		// Identifica TLisBlocoChave
		lisBlChave = IdentificaBlocoChave((VTChave*)eqpto);
		if (lisBlChave == NULL)
			return (NULL);
		// if((indChave = IndiceChave((VTChave*)eqpto)) < 0) return(NULL);
		// lisBlChave = (VTLisBlocoChave*)lisBlocoChv->Items[indChave];
		blocoRad = lisBlChave->blJus;
	}
	else
	{
		bloco = IdentificaBlocoEqNaoChave(eqpto);
		blocoRad = IdentificaBlocoRad(bloco);
	}
	return (blocoRad);
}

// ---------------------------------------------------------------------------
double __fastcall TBuscaProt::IA_Crit(VTChave *pchvVis, int indPat)
{
	VTVisRede *visRede;
	/////////////////////////////////////////////////////////////////
	// Funcao que busca visRede
	// Retorna a capacidade nominal de corrente da ligação crítica
	// referente a rede de vis de visRede->blocoDesl
	/////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (-1);
	}
	if ((visRede = IdentificaVisRede(pchvVis)) == NULL)
		return (1000.0);

	return (IA_Crit(visRede, indPat));
}

// ---------------------------------------------------------------------------
double __fastcall TBuscaProt::IA_Crit(VTVisRede *visRede, int indPat)
{
	VTLigaCrit *ligaCrit = NULL;
	strDelta *delta;
	VTLigacao *pliga;
	double ia_crit;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (-1);
	}

	if (visRede->bloco1 == visRede->blocoDesl)
		ligaCrit = visRede->ligaCrit2;

	if (visRede->bloco2 == visRede->blocoDesl)
		ligaCrit = visRede->ligaCrit1;

	// hkv201609: restricao em chave  : IA_Crit(VTVisRede *visRede, int indPat), if(ligaCrit == NULL) return(1000.0)
	if (ligaCrit == NULL)
		return (1000.0);
	// Busca a folga do patamar selecionado
	delta = NULL;
	for (int n = 0; n < ligaCrit->lisDelta->Count; n++)
	{
		delta = (strDelta*)ligaCrit->lisDelta->Items[n];
		if (delta->ipat == indPat)
			break;
		delta = NULL;
	}

	ia_crit = 1000.0;
	if (delta != NULL)
		ia_crit = delta->ia_nom;

	return (ia_crit);
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TBuscaProt::Liga_Crit(VTChave *pchvVis, int indPat)
{
	VTVisRede *visRede;
	/////////////////////////////////////////////////////////////////
	// Funcao que busca visRede
	// Retorna a ligação crítica
	// referente a rede de vis de visRede->blocoDesl
	/////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}
	if ((visRede = IdentificaVisRede(pchvVis)) == NULL)
		return (NULL);

	return (Liga_Crit(visRede, indPat));
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TBuscaProt::Liga_Crit(VTVisRede *visRede, int indPat)
{
	VTLigaCrit *ligaCrit = NULL;
	strDelta *delta;
	VTEqpto *pliga;
	double ia_crit;
	/////////////////////////////////////////////////////////////////
	// Funcao que busca visRede
	// Retorna a ligação crítica
	// referente a rede de vis de visRede->blocoDesl
	/////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}

	if (visRede->bloco1 == visRede->blocoDesl)
		ligaCrit = visRede->ligaCrit2;

	if (visRede->bloco2 == visRede->blocoDesl)
		ligaCrit = visRede->ligaCrit1;

	if (ligaCrit == NULL)
		return (NULL);
	// Busca a folga do patamar selecionado
	delta = NULL;
	for (int n = 0; n < ligaCrit->lisDelta->Count; n++)
	{
		delta = (strDelta*)ligaCrit->lisDelta->Items[n];
		if (delta->ipat == indPat)
			break;
		delta = NULL;
	}

	pliga = NULL;

	if (delta != NULL)
		pliga = delta->pliga;

	return (pliga);
}

// ---------------------------------------------------------------------------
double __fastcall TBuscaProt::FolgaIA(VTChave *pchvVis, int indPat)
{
	VTVisRede *visRede;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (-1);
	}

	if ((visRede = IdentificaVisRede(pchvVis)) == NULL)
		return (-99999.0);

	return (FolgaIA(visRede, indPat));
}

// ---------------------------------------------------------------------------
double __fastcall TBuscaProt::FolgaIA(VTVisRede *visRede, int indPat)
{
	VTBloco *blocoVis;
	VTLigaCrit *ligaCrit;
	strDelta *delta;
	double folga = -999999.0;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (-1);
	}

	if (visRede->bloco1 == visRede->blocoDesl)
	{
		blocoVis = visRede->bloco2;
		ligaCrit = visRede->ligaCrit2;
	}

	if (visRede->bloco2 == visRede->blocoDesl)
	{
		blocoVis = visRede->bloco1;
		ligaCrit = visRede->ligaCrit1;
	}

	// Busca a folga do patamar selecionado
	delta = NULL;
	for (int n = 0; n < ligaCrit->lisDelta->Count; n++)
	{
		delta = (strDelta*)ligaCrit->lisDelta->Items[n];
		if (delta->ipat == indPat)
			break;
		delta = NULL;
	}

	if (delta != NULL)
		folga = delta->dMax;

	return (folga);
}

// ---------------------------------------------------------------------------
VTVisRede* __fastcall TBuscaProt::IdentificaVisRede(VTChave *pchv, TList *lisBlocosDesl)//hk201805: rede pai-filha: não transferencia para blocos desligados
{
	VTVisRede* visRede = NULL;

	// Verifica se é chave NF
	if (pchv->Fechada)
		return (NULL);

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}

	for (int nv = 0; nv < lisVisRede->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRede->Items[nv];
		if (visRede->pchave == pchv)
			break;
		visRede = NULL;
	}
   //hk201805: rede pai-filha: não transferencia para blocos desligados
   if((visRede)&&(lisBlocosDesl))
	 {
	 if(!((lisBlocosDesl->IndexOf(visRede->bloco1) == -1)||(lisBlocosDesl->IndexOf(visRede->bloco2) == -1)))
		visRede = NULL;
	 }
	return (visRede);
}

// ---------------------------------------------------------------------------
VTBloco* __fastcall TBuscaProt::IdentificaBlocoEqNaoChave(VTEqpto *eqpto)
{
	VTBloco *bloco;
	TList *lisBloco;

	if (eqpto->Tipo() == eqptoCHAVE)
		return (NULL);
	// Identifica o bloco do equipamento
	// Obs caso de chave a funcao bloco = NULL
	lisBloco = blocos->LisBloco();
	bloco = NULL;
	for (int n = 0; n < lisBloco->Count; n++)
	{
		bloco = (VTBloco*)lisBloco->Items[n];
		if (eqpto->TipoLigacao())
			if (bloco->ExisteLigacao((VTLigacao*)eqpto))
				break;

		if (eqpto->TipoBarra())
			if (bloco->ExisteBarra((VTBarra*)eqpto))
				break;

		bloco = NULL;
	}
	return (bloco);
}

// ---------------------------------------------------------------------------
VTBloco* __fastcall TBuscaProt::IdentificaBloco(VTEqpto *eqpto)
{
	VTBlocoRad *blocoRad;
	VTBloco *bloco;
	///////////////////////////////////////////////////////////////////
	// Funcao que retorna bloco do equipamento, barra ou ligacao////////
	///////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}

	if (eqpto->Tipo() == eqptoCHAVE) // problemaaa
	{
		if ((blocoRad = IdentificaBlocoRad(eqpto)) == NULL)  //Identifica o blocoRad a jusante da chave
			bloco = NULL;
		else
			bloco = blocoRad->Bloco;
	}
	else
	{
		bloco = IdentificaBlocoEqNaoChave(eqpto);
	}

	return (bloco);

}

// ---------------------------------------------------------------------------
TList* __fastcall TBuscaProt::LisBlJusChave(VTChave *pchv)
{
	VTLisBlocoChave *pblChave;
	int index;
	// Funcao que retorna lista de blocos a jusante de chave

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}
	// Funcao que retorna índice de lisBlocoChv associado a pchv//
	if ((index = IndiceChave(pchv)) < 0)
		return (NULL);

	pblChave = (VTLisBlocoChave*)lisBlocoChv->Items[index];
	// if(!pblChave->flagBlocos)
	// {
	// if(MontaLisBlocos(pblChave->pchv))
	// {pblChave->lisBlJ->Assign(lisBlocoJus, laOr);
	// pblChave->flagBlocos = true;
	// }
	// Preenche lista de blocos a montante e lista de blocos a jusante
	// preenche blocos a jusante da chave pblChave->lisBlJ
	// if(MontaLisBlocos(pblChave)) pblChave->flagBlocos = true; //hkedp
	// }

	return (pblChave->lisBlJ);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBlJus(VTChave *pchv, TList *lisEXTBloco)
{
	VTBlocoRad *blocoRad;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	lisEXTBloco->Clear();
	// protecao
	if (pchv == NULL)
		return (false);

	// Identifica blocoRad a jusante da chave
	if ((blocoRad = IdentificaBlocoRad(pchv)) == NULL)
		return (false);
	// Adiciona blocos a jusante da chave
	lisEXTBloco->Assign(blocoRad->LisBlocoJus, laCopy);//hk201902: laOr);
	// Adiciona o bloco a justante da chave
	lisEXTBloco->Add(blocoRad->Bloco);
	return (true);
	/*
	 //Identifica blocos a jusante de blocoRad
	 if(!LisBlJus(blocoRad, lisEXTBloco)) return(false);
	 //Adiciona o bloco a justante da chave
	 lisEXTBloco->Add(blocoRad->Bloco);
	 */
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBlJus(VTBloco *bloco, TList *lisEXTBloco)
{
	VTBlocoRad *blocoRad;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	lisEXTBloco->Clear();
	if ((blocoRad = IdentificaBlocoRad(bloco)) == NULL)
		return (false);
	lisEXTBloco->Assign(blocoRad->LisBlocoJus);
	// LisBlJus(blocoRad, lisEXTBloco);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBlJus(VTBlocoRad *blocoRad, TList *lisEXTBloco, bool flagProt)
	// flagProt = false);
{
	VTBlocoRad *blRadAux;
	TList *lisAux, *lisAux1;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	// Inicia lista externa
	// caso flagProt = true os primeiros elementos da lista serão da classe VTChaves
	lisEXTBloco->Clear();
	// Funcao identifica blocos a jusante de blocoRad->bloco
	// se flagProt = true -> até primeira chave de protecao
	lisAux = new TList();
	lisAux1 = new TList();
	for (int n = 0; n < blocoRad->LisBlocoRad->Count; n++)
	{
		blRadAux = (VTBlocoRad*)blocoRad->LisBlocoRad->Items[n];
		if (flagProt)
		{
			// hkind if(lisTipoEqProt->IndexOf(blRadAux->Chave->TipoChave) != -1)
			if (ChaveProtecao(blRadAux->Chave))
			{
				// lisEXTChave->Add(blRadAux->Chave);
				continue;
			}
		}
		lisAux->Add(blRadAux);
		lisAux1->Add(blRadAux);
	}

	while (lisAux->Count > 0)
	{
		blRadAux = (VTBlocoRad*)lisAux->First();
		lisAux->Delete(0);
		if (flagProt)
		{ // if(lisTipoEqProt->IndexOf(blRadAux->Chave->TipoChave) != -1) continue;
			if (ChaveProtecao(blRadAux->Chave))
				continue;
		}
		lisAux->Assign(blRadAux->LisBlocoRad, laOr);
		lisAux1->Assign(blRadAux->LisBlocoRad, laOr);
	}

	for (int nb = 0; nb < lisAux1->Count; nb++)
		lisEXTBloco->Add(((VTBlocoRad*)lisAux1->Items[nb])->Bloco);

	delete lisAux;
	delete lisAux1;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBlProt(VTChave *pchv, TList *lisEXTBloco)
{
	AnsiString obj_name;
	TObject *pobj;
	VTBlocoRad *blocoRad;
	bool flagProt;

	// Preenche lisExtBloco com o blocos de pchv até chaves de protecao a jusante
	// e lisExtChave com chaves limitrofes  //hk2004

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	lisEXTBloco->Clear();

	// Identifica blocoRad a jusante da chave
	if ((blocoRad = IdentificaBlocoRad(pchv)) == NULL)
		return (false);
	// Identifica blocos a jusante de blocoRad até as chaves de protecao
	ListaZonaProtJus(blocoRad, lisEXTBloco, NULL, false);//hk201802a

	// lisEXTBloco->Assign(blocoRad->LisBlocoJusProt);
	// if(!LisBlJus(blocoRad, lisEXTBloco, flagProt = true)) return(false);

	/*
	 //Adiciona pchv a lisEXTChave
	 if(lisEXTChave != NULL) lisEXTChave->Add(pchv);
	 for(int n = lisEXTBloco->Count - 1; n >= 0; n--)
	 {
	 pobj = (TObject*)lisEXTBloco->Items[n];
	 obj_name = pobj->ClassName();
	 if(obj_name.AnsiCompare("TChave") != 0) continue;
	 if(lisEXTChave != NULL) lisEXTChave->Add(pobj);
	 lisEXTBloco->Delete(n);
	 }
	 */
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::BuscaBlocoAgreg(VTBloco *bloco, TList *lisEXT)
{
	VTElo *elo;
	VTBloco *blocoDef, *blocoDef1;
	TList *lisElosBloco, *lisBlocosFunde;
	bool flagFunde;
	/////////////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche lista com conjunto de blocos vizinhos a bloco ligados /////////////
	// por chaves nao ativas//////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Inicia lista com blocos fundidos
	lisEXT->Clear();
	lisEXT->Add(bloco);

	lisElosBloco = new TList();
	lisBlocosFunde = new TList();
	// Monta lista de chaves do bloco de defeito

	flagFunde = true;
	lisBlocosFunde->Add(bloco);
	while (lisBlocosFunde->Count > 0)
	{
		blocoDef = (VTBloco*)lisBlocosFunde->First();
		lisBlocosFunde->Delete(0);
		blocos->LisElo(blocoDef, lisElosBloco);
		for (int ne = 0; ne < lisElosBloco->Count; ne++)
		{
			elo = (VTElo*)lisElosBloco->Items[ne];
			if (lisChavesAtivas->IndexOf(elo->Chave) != -1)
				continue;
			if (elo->Chave->Aberta)
				continue; // if(elo->Chave->Estado == chvABERTA) continue;  //hkRevAloca
			blocoDef1 = (elo->Bloco1 == blocoDef) ? elo->Bloco2 : elo->Bloco1;

			if (lisEXT->IndexOf(blocoDef1) == -1)
			{
				lisEXT->Add(blocoDef1);
				lisBlocosFunde->Add(blocoDef1);
			}
		}
	}

	delete lisBlocosFunde;
	delete lisElosBloco;
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBlManLis(VTDefeito *pdef)
{ // lista de blocos manobraveis
	bool resposta;
	VTBloco *bloco;
	VTElo *elo;
	VTVisRede *visRede;
	VTBlocoRad *blocoRad, *blocoRadVis, *blocoRadAlivio;
	VTRede *redeDef, *redeVis;
	VTChave *chave;
	TList *lisAux;
	TList *lisChvs;
	TList *lisElos;
	TList *lisChvsBlocoDef;
	TList *lisBlocoMon;
	// TList *lisEXTBloco, *lisBlocoDef, *lisEXTBlocoVis, *lisEXTChv;
	TList *lisVisRedeDef;
	TList *lisChvAlivioVis;
	TList *lisChvBlocoVis;

	// lisBlocoDef    = pdef->LisBlocosAgrega;
	// lisEXTBloco    = pdef->LisBlocosMan;
	// lisEXTBlocoVis = pdef->LisBlocosVis;
	// lisEXTChv      = pdef->LisChvVisDef;
	lisVisRedeDef = pdef->LisVisRedeDef;
	lisChvAlivioVis = pdef->LisChvAlivioVis; // n14
	lisChvBlocoVis = pdef->LisChvBlocoVis;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche a partir de lisBlocoDef contíguos (bloco de carga com o defeito), as listas abaixo:
	// pdef->LisBlocosMan(lisEXTBloco)        : lista de blocos a jusante do defeito, com possibilidade de transferencia
	// pdef->LisChaveBlocosMan = f(pdef->LisBlocoRadJusDef) : lista de chaves NF de lisBlocosMan, inclusive chaves de fronteira com bloco de defeito
	// pdef->LisBlocosVis(lisEXTBlocoVis)     : conjunto de blocos troncais nos circuitos de socorro da chave vis ao suprimento
	// pdef->LisChvVisDef(lisEXTChv)          : conjunto de chaves NA de vis que podem transferir blocos
	// pdef->LisVisRedeDef(lisVisRedeDef)     : conjunto de visRede corresposndente a pdef->LisChvVisDef
	// pdef->LisChvAlivioVis     : chaves de alívio nos circuitos socorrentes
	// pdef->LisChvBlocoVis      : chaves no caminho elétrico no circuito de socorro para todas as possibilidades de transferencia
	// e seta para as visRede do defeito:
	// visRede->BlocoDesl    = visRede->bloco1 ou visRede->bloco2
	// visRede->blocoRadSup  : blocoRad de suprimento em circuito são
	// visRede->lisBlocosVis : blocos no caminho elétrico no circuito de socorro
	// visRede->lisChvBlocoVis: chaves no caminho elétrico no circuito de socorro
	// visRede->BlocoRadDesl = visRede->blocoRad1 ou visRede->bloco2
	// visRede->lisBlocoRadIsolaJus: lista de blocoRad até da chave vis ao defeito
	// visRede->chvJus : chave imediatamente a jusante do defeito e tem ligacao a visRede
	// visRede->blRadJusDef: blocoRad imediatamente a jusante do defeito e com ligacao a visRede
	// visRede->lisBlocosDefVis: lista de blocos com possibilidade de suprimento por visRede
	// visRede->lisChvAlivioVis: chaves de alívio nos circuitos socorrentes
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //hk201808: inserir após a preparação dos blocos

	// protecao
	if (pdef->LisBlocosAgrega->Count == 0)
		return (false); // if(lisBlocoDef->Count == 0) return(false);
	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	pdef->LisBlocosMan->Clear(); // lisEXTBloco->Clear();
	pdef->LisBlocosVis->Clear(); // lisEXTBlocoVis->Clear();
	pdef->LisChvVisDef->Clear(); // lisEXTChv->Clear();
	lisVisRedeDef->Clear();
	pdef->LisChvJus->Clear();

	// Define rede com bloco em falha             //Verif201701
	redeDef = ((VTBloco*)pdef->LisBlocosAgrega->First())->Rede;
	// hk03 lisBlocosAgrega //redeDef = ((VTBloco*)lisBlocoDef->First())->Rede;//hk03 lisBlocosAgrega

	// Cria listas temporárias
	lisChvsBlocoDef = new TList();
	lisAux = new TList();
	lisElos = new TList();

   //hk201808: inserir após a preparação dos blocos: Preparacao de pdef->lisVisRedeDef
	// Monta lista de chaves do bloco de defeito
	for (int nb = 0; nb < pdef->LisBlocosAgrega->Count;
	nb++) // for(int nb = 0; nb < lisBlocoDef->Count; nb++)
	{
		bloco = (VTBloco*)pdef->LisBlocosAgrega->Items[nb];
		// bloco = (VTBloco*)lisBlocoDef->Items[nb];
		blocos->LisElo(bloco, lisChvsBlocoDef); // hk03 lisBlocosAgrega
	} // hk03 lisBlocosAgrega

	// Identifica as chaves vis a jusante do bloco de defeito lisVisRedeDef
	// lisAux->Assign(blocoRad->LisBlocoRad, laOr);//hk03 lisBlocosAgrega

	// Preenche pdef->LisVisRedeDef, LisChvVisDef
	// visRede->blocoRadDesl,       blocoDesl
	// Identifica lista de blocosRad a justante da lista de blocos de defeito
	IdentificaLisBlocosRadJusante(pdef->LisBlocosAgrega, lisAux);
	// hk03 lisBlocosAgrega //IdentificaLisBlocosRadJusante(lisBlocoDef, lisAux);

	while (lisAux->Count > 0)
	{
		blocoRad = (VTBlocoRad*)lisAux->First();

		lisAux->Delete(0);
		lisElos->Clear();
		blocos->LisElo(blocoRad->Bloco, lisElos);
		for (int ne = 0; ne < lisElos->Count; ne++)
		{
			elo = (VTElo*)lisElos->Items[ne];
			// Verifica se elo está ligado ao bloco de defeito
			if (lisChvsBlocoDef->IndexOf(elo) != -1)
				continue;
			// Verifica se elo->chave pertence a lisVisRede
		 //hk201805: rede pai-filha: não transferencia para blocos desligados
			visRede = IdentificaVisRede(elo->Chave, pdef->LisBlocosJusProt);
			if (visRede != NULL)
			{

				visRede->IniciaDadosDefeito();
				// blocoDesl = NULL;             //bloco ligado a chave de vis do lado do circuito em falha
				// blocoRadDesl = NULL;          //blocoRadDesl->bloco = blocoDesl
				// lisBlocosDefVis->Clear();     //Lista de blocos a justante do defeito até chave vis (nao inclui o bloco de defeito)
				// lisBlocoRadIsolaJus->Clear(); //lista de blocosRad que apontam para chaves de isolamento da montante, de chvJus, para a jusante
				// blRadJusDef = NULL;           //blocoRad imediatamente a justante do defeito, 1o elemento de lisBlocoRadIsolaJus
				// chvJus = NULL;                //chave imediatamente a jusante do blocos de defeito que liga a chave de vis
				lisVisRedeDef->Add(visRede);
				pdef->LisChvVisDef->Add(visRede->pchave); // lisEXTChv->Add(visRede->pchave);
				visRede->blocoDesl = blocoRad->Bloco; // seta o bloco desligado da chave vis
				visRede->blocoRadDesl = blocoRad; // seta o blocoRad desligado da chave vis
				// Define lista de blocos
				blocoRadVis = (visRede->blocoRadDesl == visRede->blocoRad2) ? visRede->blocoRad1 :
					visRede->blocoRad2;

			}
		}
		lisAux->Assign(blocoRad->LisBlocoRad, laOr);
	}
   //hk201808: inserir após a preparação dos blocos: Preparacao de pdef->lisVisRedeDef

	lisChvAlivioVis->Clear(); // n14
	lisChvBlocoVis->Clear();

	// Determina lisEXTBlocoVis = pdef->LisBlocosVis
	for (int n = 0; n < lisVisRedeDef->Count; n++)
	{ // lisEXTChv->Add((visRede = (VTVisRede*)lisVisRedeDef->Items[n])->pchave);
		// if((visRede->rede1 != redeDef)&&(visRede->rede2 != redeDef)) continue; //problema
		visRede = (VTVisRede*)lisVisRedeDef->Items[n];
		visRede->lisChvBlocoVis->Clear();
		visRede->lisChvAlivioVis->Clear();
		visRede->lisBlocosVis->Clear();
		// Cria lista de blocos troncais nos circuitos de socorro- lisExtBlocoVis
		// lista de chaves troncais
		// lista de chaves de alívio nos circuitos de socorro
		blocoRad = visRede->blocoRad1;
		if (visRede->blocoRad1 == visRede->blocoRadDesl)
			blocoRad = visRede->blocoRad2;
		visRede->blocoRadSup = NULL;
		// Percorre blocos no circuito de socorro até suprimento
		while ((blocoRad != NULL) && (blocoRad->FlagSup == false))
		{
			visRede->lisBlocosVis->Add(blocoRad->Bloco);
			if (blocoRad->Chave != NULL)
				visRede->lisChvBlocoVis->Add(blocoRad->Chave); // cn
			blocoRad = blocoRad->BlocoRadMon;
		}

		if (blocoRad->FlagSup)
		{
			visRede->blocoRadSup = blocoRad;
			for (int nb = 0; nb < blocoRad->LisBlocoRadJus->Count; nb++)
			{
				blocoRadAlivio = (VTBlocoRad*)blocoRad->LisBlocoRadJus->Items[nb];
				if ((chave = blocoRadAlivio->Chave) == NULL)
					continue;
				if (visRede->lisChvAlivioVis->IndexOf(chave) == -1) // cn
						visRede->lisChvAlivioVis->Add(chave);
			}
		}

		/*
		 while((blocoRad != NULL)&&(blocoRad->FlagSup == false))
		 {
		 visRede->lisBlocosVis->Add(blocoRad->Bloco);
		 //lisEXTBlocoVis->Add(blocoRad->Bloco);
		 if(blocoRad->Chave != NULL)
		 {
		 visRede->lisChvBlocoVis->Add(blocoRad->Chave);
		 //if(lisChvBlocoVis->IndexOf(blocoRad->Chave) == -1)
		 //		  lisChvBlocoVis->Add(blocoRad->Chave);
		 }
		 //Adiciona chaves a jusante de blocoRad a
		 //			for(int nb = 0; nb < blocoRad->LisBlocoRad->Count; nb++)
		 //			  {
		 //			  blocoRadAlivio = (VTBlocoRad*)blocoRad->LisBlocoRad->Items[nb];
		 //			  chave = blocoRadAlivio->Chave;
		 //			  visRede->lisChvAlivioVis->Add(chave);
		 //			  }
		 for(int nb = 0; nb < blocoRad->LisBlocoRadJus->Count; nb++)
		 {
		 blocoRadAlivio = (VTBlocoRad*)blocoRad->LisBlocoRadJus->Items[nb];
		 if((chave = blocoRadAlivio->Chave) == NULL) continue;
		 if(visRede->lisChvAlivioVis->IndexOf(chave) == -1)
		 visRede->lisChvAlivioVis->Add(chave);
		 }
		 blocoRad = blocoRad->BlocoRadMon;
		 }
		 if(blocoRad->FlagSup == true) visRede->blocoRadSup = blocoRad;
		 */

		for (int nc = 0; nc < visRede->lisChvBlocoVis->Count; nc++)
		{
			visRede->lisChvAlivioVis->Remove(visRede->lisChvBlocoVis->Items[nc]);
		}

		// for(int nc = 0; nc < lisChvBlocoVis->Count; nc++)
		// {lisChvAlivioVis->Remove(lisChvBlocoVis->Items[nc]);
		// }
		lisChvAlivioVis->Assign(visRede->lisChvAlivioVis, laOr);
		lisChvBlocoVis->Assign(visRede->lisChvBlocoVis, laOr);
		pdef->LisBlocosVis->Assign(visRede->lisBlocosVis, laOr);
		// lisEXTBlocoVis->Assign(visRede->lisBlocosVis, laOr);
	}

	// n14for(int n = 0; n < lisVisRedeDef->Count; n++)
	// {//lisEXTChv->Add((visRede = (VTVisRede*)lisVisRedeDef->Items[n])->pchave);
	// if((visRede->rede1 != redeDef)&&(visRede->rede2 != redeDef)) continue; //problema
	// visRede = (VTVisRede*)lisVisRedeDef->Items[n];
	// redeVis = (visRede->rede1 == visRede->blocoDesl->Rede) ? visRede->rede2 : visRede->rede1;
	// blocos->LisBlocoRede(redeVis, lisEXTBlocoVis);
	// n14  }

	// Para cada chave VIS identifica blocos a montante: visRede->lisBlocosDefVis
	// e chave de isolamento ligada ao bloco de defeito: visRede->chvJus
	lisBlocoMon = new TList();
	pdef->LisBlocoRadJusDef->Clear();
	for (int nv = 0; nv < lisVisRedeDef->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRedeDef->Items[nv];
		// Verifica se a chave vis liga ao bloco de defeito
		if (pdef->LisBlocosAgrega->IndexOf((blocoRad = visRede->blocoRadDesl)->Bloco) != -1)
			continue;
		// hk03 lisBlocosAgrega //if(lisBlocoDef->IndexOf((blocoRad = visRede->blocoRadDesl)->Bloco) != -1) continue;
		// if(((blocoRad = visRede->blocoRadDesl)->Bloco) == blocoDef) continue; //hk03 lisBlocosAgrega

		visRede->lisBlocoRadIsolaJus->Add(blocoRad);
		// while(blocoRad->BlocoRadMon->Bloco != blocoDef)     //hk03 lisBlocosAgrega
			// hk03 lisBlocosAgrega //while(lisBlocoDef->IndexOf(blocoRad->BlocoRadMon->Bloco) == -1)
	  //while (pdef->LisBlocosAgrega->IndexOf(blocoRad->BlocoRadMon->Bloco) == -1)
	  //hk201805: rede pai-filha: transferência de blocos da rede da desligada, ligada a chave vis, blocoRad->Rede == visRede->blocoRadDesl->Rede
		while ((pdef->LisBlocosAgrega->IndexOf(blocoRad->BlocoRadMon->Bloco) == -1)&&(blocoRad->Rede == visRede->blocoRadDesl->Rede))
		{
			blocoRad = blocoRad->BlocoRadMon;
			visRede->lisBlocoRadIsolaJus->Add(blocoRad);
		}

		// Guarda chave de isolamento a montante -- HK0713 -- guardar lista de chaves e eleger
		// a chave automática sem reenergizar todos os blocos??
		visRede->chvJus = blocoRad->Chave;
		visRede->blRadJusDef = blocoRad;

		// Atribui lusta de aos blocos que podem ser supridos a jusante por visRede para pdef
		visRede->lisBlocosDefVis->Add(blocoRad->Bloco);
		visRede->lisBlocosDefVis->Assign(blocoRad->LisBlocoJus, laOr);

		// Adiciona a lista de blocos manobráveis
		pdef->LisBlocosMan->Assign(visRede->lisBlocosDefVis, laOr);
		// lisEXTBloco->Assign(visRede->lisBlocosDefVis, laOr);
		// Adiciona blocoRad e chave a jusante do defeito com possibilidade de transferencia
		// a pdef->LisBlocoRadJusDef e pdef->LisChvJus
		if (pdef->LisBlocoRadJusDef->IndexOf(visRede->blRadJusDef) == -1)
		{
			pdef->LisBlocoRadJusDef->Add(visRede->blRadJusDef);
			pdef->LisChvJus->Add(visRede->blRadJusDef->Chave);
		}
	}

	// Preenche lista de chaves de pdef->LisChaveBlocosMan = f(pdef->LisBlocoRadJusDef)
	pdef->LisChavesBlocosMan->Clear();
	for (int nb = 0; nb < pdef->LisBlocoRadJusDef->Count; nb++)
	{
		blocoRad = (VTBlocoRad*)pdef->LisBlocoRadJusDef->Items[nb];
		lisAux->Clear();
		lisAux->Add(blocoRad);
		while (lisAux->Count > 0)
		{
			blocoRad = (VTBlocoRad*)lisAux->First();
			lisAux->Delete(0);
			pdef->LisChavesBlocosMan->Add(blocoRad->Chave);
			lisAux->Assign(blocoRad->LisBlocoRad, laOr);
		}

	}

	delete lisAux;
	delete lisElos;
	delete lisChvsBlocoDef;
	delete lisBlocoMon;

	return (true);
}

// ---------------------------------------------------------------------------
double __fastcall TBuscaProt::CargaLisBlocosMan(VTDefeito *pdef, int iPat)
{
	double carga = 0.0;
	VTBlocoRad *blocoRad;

	for (int n = 0; n < pdef->LisBlocoRadJusDef->Count; n++)
	{
		blocoRad = (VTBlocoRad*)pdef->LisBlocoRadJusDef->Items[n];
		carga += blocoRad->CargaBlRad[iPat];
	}

	return (carga);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBlVisMan(VTBloco *blocoDef, TList *lisEXTBloco)
{
	VTChave *pchave;
	VTRede *redeDef, *redeVis;
	VTVisRede *visRede;
	VTBloco *blocoVis;
	VTBlocoRad *blocoRad;
	TList *lisRedeVisDef;
	TList *lisChvVis;
	/////////////////////////////////////////////////////////////////////////
	// Funcao que gera blocos nas redes de socorro ou de vis os blocos fora //
	// dos caminhos elétricos entre as chaves de vis e o suprimento         //
	/////////////////////////////////////////////////////////////////////////
	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	// Busca o conjunto de blocos manobraveis em circuitos VIS:Conjunto com
	// blocos que podem ser desligados,
	// a partir do bloco de defeito e de lista de chaves VIS do circuito do bloco de defeito
	lisEXTBloco->Clear();
	// Rede de defeito
	redeDef = blocoDef->Rede; // Verif201701
	// Seleciona todos os blocos das redes Vis
	// e Inicia lisEXTBloco com todos os blocos das redes Vis
	lisRedeVisDef = new TList();
	lisChvVis = new TList();
	for (int nv = 0; nv < lisVisRede->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRede->Items[nv];
		redeVis = NULL;
		if (visRede->rede1 == redeDef)
			redeVis = visRede->rede2;
		if (visRede->rede2 == redeDef)
			redeVis = visRede->rede1;
		if (redeVis == NULL)
			continue;
		lisChvVis->Add(visRede);
		if (lisRedeVisDef->IndexOf(redeVis) == -1)
		{
			lisRedeVisDef->Add(redeVis);
			blocos->LisBlocoRede(redeVis, lisEXTBloco);
		}
	}

	// Elimina blocos nos caminhos de suprimento das
	// redes vis
	for (int nc = 0; nc < lisChvVis->Count; nc++)
	{
		visRede = (VTVisRede*)lisChvVis->Items[nc]; // Verif201701
		blocoVis = (visRede->bloco1->Rede == redeDef) ? visRede->bloco2 : visRede->bloco1;
		blocoRad = IdentificaBlocoRad(blocoVis);
		while (blocoRad != NULL)
		{
			lisEXTBloco->Remove(blocoRad->Bloco);
			blocoRad = blocoRad->BlocoRadMon;
		}
	}

	delete lisRedeVisDef;
	delete lisChvVis;

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisChaveOrd(TList *lisExt)
{
	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	for (int n = 0; n < lisBlocoChv->Count; n++)
		lisExt->Add(((VTLisBlocoChave*)lisBlocoChv->Items[n])->pchv);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisChvBloco(VTBloco *bloco, TList *lisExt)
{ // REVER
	VTBlocoRad *blocoRad, *blocoRadJus;
	TList *lisElo;
	VTElo *elo;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	// Retorna chave a montante e lista de chaves a jusante
	if ((blocoRad = IdentificaBlocoRad(bloco)) == NULL)
		return (false);
	lisExt->Clear();
	if (blocoRad->Chave != NULL)
		lisExt->Add(blocoRad->Chave);
	lisElo = new TList();
	blocos->LisElo(bloco, lisElo);
	for (int n = lisElo->Count - 1; n >= 0; n--)
	{
		elo = (VTElo*)lisElo->Items[n];
		if (elo->Chave == blocoRad->Chave)
			continue;
		lisExt->Add(elo->Chave);
	}
	delete lisElo;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisLigaVis(VTVisRede *visRede)
{
	// LisLigaVis(visRede->pchave, visRede->pbarra1, visRede->lisLiga1, visRede->lisBlocosVis1);
	VTBlocoRad *blocoRad;
	VTChave *pchave;

	visRede->lisLiga1->Clear();
	visRede->lisLiga2->Clear();
	visRede->lisBlocosVis1->Clear();
	visRede->lisBlocosVis2->Clear();

	// percorre os blocos até o suprimento 1
	blocoRad = visRede->blocoRad1;
	pchave = visRede->pchave;
	while (blocoRad != NULL)
	{
		// Define listas internas de ligações
		if (pchave != blocoRad->Chave)
			LisLigaBloco(blocoRad->Chave, pchave, visRede->lisLiga1);
		pchave = blocoRad->Chave;
		if (blocoRad->BlocoRadMon != NULL)
		{
			if (blocoRad->BlocoRadMon->FlagSup)
				visRede->lisBlocosVis1->Assign(blocoRad->LisBlocoJus, laOr);
			visRede->lisBlocosVis1->Add(blocoRad->BlocoRadMon->Bloco);
		}
		blocoRad = blocoRad->BlocoRadMon;
	}

	// percorre os blocos até o suprimento 2
	blocoRad = visRede->blocoRad2;
	pchave = visRede->pchave;
	while (blocoRad != NULL)
	{
		// Define listas internas de ligações
		if (pchave != blocoRad->Chave)
			LisLigaBloco(blocoRad->Chave, pchave, visRede->lisLiga2);
		pchave = blocoRad->Chave;
		if (blocoRad->BlocoRadMon != NULL)
		{
			if (blocoRad->BlocoRadMon->FlagSup)
				visRede->lisBlocosVis2->Assign(blocoRad->LisBlocoJus, laOr);
			visRede->lisBlocosVis2->Add(blocoRad->BlocoRadMon->Bloco);
		}
		blocoRad = blocoRad->BlocoRadMon;
	}

	return (true);

}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisLigaVis(VTChave *chv, VTBarra *pbarra, TList *lisExt,
	TList *lisBlocosExt)
{
	VTBarra *pbarra1, *pbarra2;
	VTBlocoRad *blocoRad, *blocoRadJus;
	VTChave *pchave;

	// Retorna lista com ligações da chv selecionada até o suprimento
	// pbarra1 = chv->pbarra1;
	// pbarra2 = chv->pbarra2;

	lisExt->Clear();
	if (lisBlocosExt)
		lisBlocosExt->Clear();
	// define o bloco da barra
	if ((blocoRad = IdentificaBlocoRad(pbarra)) == NULL)
		return (false);
	// percorre os blocos até o suprimento
	pchave = chv;
	while (blocoRad != NULL)
	{
		// Define listas internas de ligações
		if (pchave != blocoRad->Chave)
			LisLigaBloco(blocoRad->Chave, pchave, lisExt);
		pchave = blocoRad->Chave;

		if (lisBlocosExt != NULL)
		{
			if (blocoRad->BlocoRadMon != NULL)
			{
				if (blocoRad->BlocoRadMon->FlagSup)
					lisBlocosExt->Assign(blocoRad->LisBlocoJus, laOr);
				lisBlocosExt->Add(blocoRad->BlocoRadMon->Bloco);
			}
		}

		blocoRad = blocoRad->BlocoRadMon;
	}
	// blocoRadJus: bloco de suprimento
	// if(lisBlocosExt) lisBlocosExt->Assign(blocoRadJus->LisBlocoJus, laOr);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisLigaVis(VTBarra *pbarra1, TList *lisExt)
{
	VTBlocoRad *blocoRad;
	VTChave *pchave;

	// Retorna lista com ligações da chv selecionada até o suprimento
	// pbarra1 = chv->pbarra1;

	lisExt->Clear();
	if ((blocoRad = IdentificaBlocoRad(pbarra1)) == NULL)
		return (false);
	pchave = NULL;
	while (blocoRad != NULL)
	{
		if (pchave != blocoRad->Chave)
			LisLigaBloco(blocoRad->Chave, pchave, lisExt);
		pchave = blocoRad->Chave;
		blocoRad = blocoRad->BlocoRadMon;
	}

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisLigaJusChave(VTChave *chave, TList *lisLigaJus)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTBlocoRad *blocoRad;
	VTBarra *barra;

	if (lisLigaJus == NULL)
		return;
	lisLigaJus->Clear();

	if ((blocoRad = IdentificaBlocoRad(chave)) == NULL)
		return;

	barra = chave->pbarra2;
	if (blocoRad->Bloco->LisBarra()->IndexOf(chave->pbarra1) != -1)
		barra = chave->pbarra1;

	redes->LisLigacao(lisLigaJus, barra);
	lisLigaJus->Remove(chave);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::ListaLigaVisTotal(TList *lisChvVis, TList *lisLigaVis,
	TList *lisLigaVisDef)
{
	VTVisRede *visRede;

	for (int nv = 0; nv < lisVisRede->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRede->Items[nv];
		// hkJunho
		if (lisChvVis->IndexOf(visRede->pchave) == -1)
			continue;

		if (visRede->bloco1 == visRede->blocoDesl)
		{
			lisLigaVis->Assign(visRede->lisLiga2, laOr);
			lisLigaVisDef->Assign(visRede->lisLiga1, laOr);
		}
		else
		{
			lisLigaVis->Assign(visRede->lisLiga1, laOr);
			lisLigaVisDef->Assign(visRede->lisLiga2, laOr);
		}

		lisLigaVis->Remove(visRede->pchave); // hkJunho
		// lisLigaVisDef->Remove(visRede->pchave); //hkJunho
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisBlRadTerm(VTBloco *bloco, TList *lisEXTBlocoRad)
{
	VTBlocoRad *blRad;
	TList *lisAux;
	// Preenche lista de TBlocoRad "terminais"

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	if ((blRad = IdentificaBlocoRad(bloco)) == NULL)
		return;
	lisAux = new TList();
	lisAux->Add(blRad);
	while (lisAux->Count > 0)
	{
		blRad = (VTBlocoRad*)lisAux->First();
		lisAux->Delete(0);
		if (blRad->LisBlocoRad->Count == 0)
		{
			lisEXTBlocoRad->Add(blRad);
			continue;
		}
		lisAux->Assign(blRad->LisBlocoRad, laOr);
	}
	delete lisAux;
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::ListaBlocoRadTotal(TList *lisEXT)
{
	VTBlocoRad *blRad;
	TList *lisAux;

	/////////////////////////////////////////////////
	// Preenche lista com todos os blocosRad criados//
	/////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return;}

	lisAux = new TList();
	for (int nb = 0; nb < lisBlocoRad->Count; nb++)
	{
		blRad = (VTBlocoRad*)lisBlocoRad->Items[nb];
		lisAux->Add(blRad);
		while (lisAux->Count > 0)
		{
			blRad = (VTBlocoRad*)lisAux->First();
			lisAux->Delete(0);
			lisAux->Assign(blRad->LisBlocoRad, laOr);
			lisEXT->Add(blRad);
		}
	}
	delete lisAux;
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisChavesDmax(double dMax, TList *lisChavesDmax)
{
	//////////////////////////////////////////////////////////////////////////
	// Lista chaves a partir do bloco(s) de suprimento com distância < dMax  //
	//////////////////////////////////////////////////////////////////////////
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisAux;
	VTEqbar *psup;

	lisChavesDmax->Clear();
	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (false);
	}

	///////////////////////////////////////////////////////////////////////////////
	// Busca bloco de suprimento percorre em lista de barras do bloco se existe geracao
	lisAux = new TList();
	lisAux->Clear();
	redes->LisEqbar(lisAux, eqptoSUPRIMENTO);
	lisChavesDmax->Clear();
	for (int n = 0; n < lisAux->Count; n++)
	{
		psup = (VTEqbar*)lisAux->Items[n];

		// Busca chaves até dMax
		LisLigaDMax(psup->pbarra, eqptoCHAVE, lisChavesDmax, dMax);
	}

	// Adiciona blocos ligados a redes equivalentes de suprimento
	// REDUÇÕES COM SUPRIMENTO
	delete lisAux;

	/* VTBlocoRad *blocoRad0, *blocoRad;
	 VTBarra    *pbarra;
	 VTChave    *chaveRef;

	 lisChavesDmax->Clear();
	 //Verifica se a classe já foi inicializada
	 if(!flagInicia)
	 {if(!Inicia()) return(false);}

	 for(int ns = 0; ns < lisBlocoRad->Count; ns++)
	 {
	 blocoRad0 = (VTBlocoRad*)lisBlocoRad->Items[ns];
	 for(int ns1 = 0; ns1 < blocoRad0->LisBlocoRad->Count; ns1++)
	 {
	 blocoRad = (VTBlocoRad*)blocoRad0->LisBlocoRad->Items[ns1];
	 //Define a barra Inicial
	 pbarra = blocoRad->Chave->pbarra1;
	 if(blocoRad->Bloco->LisBarra()->IndexOf(pbarra) == -1) pbarra = blocoRad->Chave->pbarra2;
	 //Busca chaves até dMax
	 LisLigaDMax(pbarra, eqptoCHAVE, lisChavesDmax, dMax);
	 }
	 }
	 return(true);
	 */
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBarraRadChave(VTBlocoRad *blocoRad, TList *lisBarraRad)
{
	VTBarraRad *barraRadAux;
	VTBarra *pbarra;

	// Preenche lisBarraRad, lista de barrasRad extremas do bloco, radialmente falando
	lisBarraRad->Clear();

	pbarra = blocoRad->Chave->pbarra1;
	if (blocoRad->Bloco->LisBarra()->IndexOf(pbarra) == -1)
		pbarra = blocoRad->Chave->pbarra2;

	if (barraRad != NULL)
		delete barraRad;
	barraRad = NULL;
	barraRad = LisBarraBloco(blocoRad->Bloco, pbarra);

	TList *lisAux = new TList();
	lisAux->Add(barraRad);

	while (lisAux->Count > 0)
	{
		barraRadAux = (VTBarraRad*)lisAux->First();
		lisAux->Delete(0);
		if (barraRadAux->lisBarRad->Count == 0)
		{
			if (lisBarraRad->IndexOf(barraRadAux) == -1)
				lisBarraRad->Add(barraRadAux);
		}
		else
			lisAux->Assign(barraRadAux->lisBarRad, laOr);
	}
	delete lisAux;

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBarChave(VTBlocoRad *blocoRad, TList *lisBarra)
{
	TList *lisBarraRad;
	VTBarraRad *barraRad;
	// Preenche lisBarra, lista de barrasRad extremas do bloco, radialmente falando
	lisBarraRad = new TList();
	LisBarraRadChave(blocoRad, lisBarraRad);
	for (int n = 0; n < lisBarraRad->Count; n++)
	{
		barraRad = (VTBarraRad*)lisBarraRad->Items[n];
		if (lisBarra->IndexOf(barraRad->pbarra) != -1)
			continue;
		lisBarra->Add(barraRad->pbarra);
	}
	delete lisBarraRad;
	return (true);

	/*
	 VTBarraRad *barraRadAux;
	 VTBarra    *pbarra;
	 //Preenche lisBarra, lista de barras extremas do bloco, radialmente falando
	 pbarra = blocoRad->Chave->pbarra1;
	 if(blocoRad->Bloco->LisBarra()->IndexOf(pbarra) == -1) pbarra = blocoRad->Chave->pbarra2;

	 if(barraRad != NULL) delete barraRad;
	 barraRad = NULL;
	 barraRad = LisBarraBloco(blocoRad->Bloco, pbarra);

	 TList *lisAux = new TList();
	 lisAux->Add(barraRad);

	 while(lisAux->Count > 0)
	 {
	 barraRadAux = (VTBarraRad*)lisAux->First();
	 lisAux->Delete(0);
	 if(barraRadAux->lisBarRad->Count == 0)
	 {
	 if(lisBarra->IndexOf(barraRadAux->pbarra) == -1) lisBarra->Add(barraRadAux->pbarra);
	 }
	 else
	 lisAux->Assign(barraRadAux->lisBarRad, laOr);
	 }
	 delete lisAux;

	 return(barraRad);
	 */
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisBarChave(VTChave *pchave, TList *lisBarra)
{
	VTBlocoRad *blocoRad;
	bool resposta;

	// n14 lisBarra->Clear();
	if ((blocoRad = IdentificaBlocoRad(pchave)) == NULL)
		return (false);

	resposta = LisBarChave(blocoRad, lisBarra);
	return (resposta);

}

// ---------------------------------------------------------------------------
VTBarraRad* __fastcall TBuscaProt::LisBarProt(VTBloco *bloco)
{
	VTBlocoRad *blocoRad;
	TList *lisBlocos;
	VTBarra *pbarra;
	////////////////////////////////////////////////////////////////////////////////////////
	// Cria lista de estruturas de barra pai -> barras filhas a partir da
	// chave de suprimento do bloco
	// com chave de suprimento blocoRad->pchave; blocoRad->bloco = bloco
	////////////////////////////////////////////////////////////////////////////////////////

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (NULL);
	}

	// Identifica blocoRad de bloco
	if ((blocoRad = IdentificaBlocoRad(bloco)) == NULL)
		return (NULL);

	// Identifica a barra de suprimento pertencente ao bloco
	pbarra = blocoRad->Chave->pbarra1;
	if (bloco->LisBarra()->IndexOf(pbarra) == -1)
		pbarra = blocoRad->Chave->pbarra2;

	if (barraRad != NULL)
		delete barraRad;
	barraRad = NULL;
	barraRad = LisBarraBloco(bloco, pbarra);

	return (barraRad);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisLigaDMax(VTBarra *pbarra, int tipoLigacao, TList *lisLigaDMax,
	double distMax)
{
	TList *lisLigaBarra, *lisBarra, *lisLiga; // , *lisExt;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *pliga;
	VTBarraRad *barRad, *barRadAux, *barRadIni;
	VTBarra *barraAux;
	double daux;
	TList *lisAux;

	lisLiga = new TList();
	lisLigaBarra = new TList();
	lisBarra = new TList();
	lisAux = new TList();

	lisBarra->Add(barRadIni = NewObjBarraRad(pbarra));
	barRadIni->dcompMon = 0.0;

	while (lisBarra->Count > 0)
	{
		barRad = (VTBarraRad*)lisBarra->First();
		lisBarra->Delete(0);

		lisLigaBarra->Clear();
		redes->LisLigacao(lisLigaBarra, barRad->pbarra);
		// Elimina ligacaoes que não estão em lisLiga
		for (int n = lisLigaBarra->Count - 1; n >= 0; n--)
		{
			pliga = (VTLigacao*)lisLigaBarra->Items[n];

			if (lisLiga->IndexOf(pliga) == -1)
			{
				daux = barRad->dcompMon;
				if (pliga->Tipo() == eqptoTRECHO)
					daux += (pliga->ComprimentoUtm_cm() / 100.0);

				if (daux > distMax)
					continue;

				pliga->Obj = pbarra->Obj;
				lisLiga->Add(pliga);
				if ((pliga->pbarra1 != NULL) && (pliga->pbarra1 != barRad->pbarra))
				{
					lisBarra->Add(barRadAux = NewObjBarraRad(pliga->pbarra1));
					barRadAux->dcompMon = daux;
					if (pliga->Tipo() == tipoLigacao)
					{ // if(lisChavesAtivas->IndexOf(pliga) != -1)
						lisLigaDMax->Add(pliga);
						if (((VTChave*)pliga)->Aberta)
							// if(((VTChave*)pliga)->Estado == chvABERTA)//hkRevAloca
								lisAux->Add(pliga);
					}
					barRad->lisBarRad->Add(barRadAux);
					barRadAux->pligaPai = pliga;
					barRadAux->pbarRadPai = barRad;
					barRadAux->pbarPai = barRad->pbarra;
				}
				if ((pliga->pbarra2 != NULL) && (pliga->pbarra2 != barRad->pbarra))
				{
					lisBarra->Add(barRadAux = NewObjBarraRad(pliga->pbarra2));
					barRadAux->dcompMon = daux;
					if (pliga->Tipo() == tipoLigacao)
					{ // if(lisChavesAtivas->IndexOf(pliga) != -1)
						lisLigaDMax->Add(pliga);
						if (((VTChave*)pliga)->Aberta)
							// if(((VTChave*)pliga)->Estado == chvABERTA)//hkRevAloca
								lisAux->Add(pliga);
					}

					barRad->lisBarRad->Add(barRadAux);
					barRadAux->pligaPai = pliga;
					barRadAux->pbarRadPai = barRad;
					barRadAux->pbarPai = barRad->pbarra;
				}
				if ((pliga->pbarra3 != NULL) && (pliga->pbarra3 != barRad->pbarra))
				{
					lisBarra->Add(barRadAux = NewObjBarraRad(pliga->pbarra3));
					barRadAux->dcompMon = daux;
					if (pliga->Tipo() == tipoLigacao)
					{ // if(lisChavesAtivas->IndexOf(pliga) != -1)
						lisLigaDMax->Add(pliga);
						if (((VTChave*)pliga)->Aberta)
							// if(((VTChave*)pliga)->Estado == chvABERTA)//hkRevAloca
								lisAux->Add(pliga);
					}

					barRad->lisBarRad->Add(barRadAux);
					barRadAux->pligaPai = pliga;
					barRadAux->pbarRadPai = barRad;
					barRadAux->pbarPai = barRad->pbarra;
				}
			}
		}
	}

	delete barRadIni;
	delete lisLiga; // hk2004
	delete lisLigaBarra; // hk2004
	delete lisBarra; // hk2004
	delete lisAux;

}

// ---------------------------------------------------------------------------
VTBarraRad* __fastcall TBuscaProt::LisBarraBloco(VTBloco *bloco, VTBarra *pbarra)
{
	TList *lisLigaBarra, *lisBarra, *lisLiga; // , *lisExt;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *pliga;
	VTBarraRad *barRad, *barRadAux, *barRadIni;
	VTBarra *barraAux;
	// Cria estrutura de objetos em hierarquia de barras pai e filha
	// a partir de barra de suprimento do bloco (pbarra)
	lisLiga = new TList();
	lisLigaBarra = new TList();
	lisBarra = new TList();

	lisLiga->Assign(bloco->LisLigacao(), laOr);

	lisBarra->Add(barRadIni = NewObjBarraRad(pbarra));

	while (lisBarra->Count > 0)
	{
		barRad = (VTBarraRad*)lisBarra->First();
		lisBarra->Delete(0);

		lisLigaBarra->Clear();
		redes->LisLigacao(lisLigaBarra, barRad->pbarra);
		// Elimina ligacaoes que não estão em lisLiga
		for (int n = lisLigaBarra->Count - 1; n >= 0; n--)
		{
			pliga = (VTLigacao*)lisLigaBarra->Items[n];
			if (lisLiga->IndexOf(pliga) != -1)
			{
				pliga->Obj = pbarra->Obj;
				lisLiga->Remove(pliga);
				if ((pliga->pbarra1 != NULL) && (pliga->pbarra1 != barRad->pbarra))
				{
					lisBarra->Add(barRadAux = NewObjBarraRad(pliga->pbarra1));
					barRad->lisBarRad->Add(barRadAux);
					barRadAux->pligaPai = pliga;
					barRadAux->pbarRadPai = barRad;
					barRadAux->pbarPai = barRad->pbarra;
				}
				if ((pliga->pbarra2 != NULL) && (pliga->pbarra2 != barRad->pbarra))
				{
					lisBarra->Add(barRadAux = NewObjBarraRad(pliga->pbarra2));
					barRad->lisBarRad->Add(barRadAux);
					barRadAux->pligaPai = pliga;
					barRadAux->pbarRadPai = barRad;
					barRadAux->pbarPai = barRad->pbarra;
				}
				if ((pliga->pbarra3 != NULL) && (pliga->pbarra3 != barRad->pbarra))
				{
					lisBarra->Add(barRadAux = NewObjBarraRad(pliga->pbarra3));
					barRad->lisBarRad->Add(barRadAux);
					barRadAux->pligaPai = pliga;
					barRadAux->pbarRadPai = barRad;
					barRadAux->pbarPai = barRad->pbarra;
				}
			}
		}
	}

	delete lisLiga; // hk2004
	delete lisLigaBarra; // hk2004
	delete lisBarra; // hk2004

	return (barRadIni);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::LisLigaBloco(VTChave *chv1, VTChave *chv2, TList *lisExt)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisLigaBarra, *lisBarra, *lisLiga;
	VTBarra *pbarra;
	VTBloco *bloco;
	VTLigacao *pliga;
	VTEqpto *eqpto;
	VTBlocoRad *blocoRad;

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(false);}

	// Identifica lista de ligacoes de chv1(Montante) a chv2(Jusante)
	if ((chv1 == NULL) || (chv2 == NULL))
		return (false);

	// iDENTIFICA BLOCORAD DE CHV1
	if ((blocoRad = IdentificaBlocoRad((VTEqpto*)chv1)) == NULL)
		return (false);
	bloco = blocoRad->Bloco;

	pbarra = chv1->pbarra2;
	if ((bloco->LisBarra())->IndexOf(chv1->pbarra1) != -1)
		pbarra = chv1->pbarra1;

	// LisLigaBloco(pbarra, chv2, lisExt);

	lisLiga = new TList();
	lisLigaBarra = new TList();
	lisBarra = new TList();

	lisLiga->Assign(bloco->LisLigacao(), laOr);
	for (int n = 0; n < lisLiga->Count; n++)
	{
		pliga = (VTLigacao*)lisLiga->Items[n];
		pliga->Obj = NULL;
		pliga->pbarra1->Obj = NULL;
		pliga->pbarra2->Obj = NULL;
		if (pliga->pbarra3)
			pliga->pbarra3->Obj = NULL;
	}
	lisLiga->Add(chv2);

	lisBarra->Add(pbarra);
	pbarra->Obj = chv1;
	chv1->Obj = NULL;
	while (lisBarra->Count > 0)
	{
		pbarra = (VTBarra*)lisBarra->First();
		lisBarra->Delete(0);
		lisLigaBarra->Clear();
		redes->LisLigacao(lisLigaBarra, pbarra);
		// Elimina ligacaoes que não estão em lisLiga
		for (int n = lisLigaBarra->Count - 1; n >= 0; n--)
		{
			pliga = (VTLigacao*)lisLigaBarra->Items[n];
			if (lisLiga->IndexOf(pliga) != -1)
			{
				pliga->Obj = pbarra->Obj;
				if (pliga == chv2)
				{
					lisBarra->Clear();
					break;
				}
				lisLiga->Remove(pliga);
				if ((pliga->pbarra1 != NULL) && (pliga->pbarra1 != pbarra))
				{
					lisBarra->Add(pliga->pbarra1);
					pliga->pbarra1->Obj = pliga;
				}
				if ((pliga->pbarra2 != NULL) && (pliga->pbarra2 != pbarra))
				{
					lisBarra->Add(pliga->pbarra2);
					pliga->pbarra2->Obj = pliga;
				}
				if ((pliga->pbarra3 != NULL) && (pliga->pbarra3 != pbarra))
				{
					lisBarra->Add(pliga->pbarra3);
					pliga->pbarra3->Obj = pliga;
				}
			}
		}
	}

	if (pliga == chv2)
	{
		lisExt->Add(pliga);
		while (pliga->Obj != NULL)
		{
			lisExt->Add(pliga->Obj);
			pliga = (VTLigacao*)(pliga->Obj);
		}
	}

	delete lisLigaBarra;
	delete lisLiga;
	delete lisBarra;
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisBlocosMed(VTChave *pchave, TList *lisExt, TList *lisExtFixo)
{
	int indBl, indBlAux;
	TList *lisBlocosAux;
	VTBloco *pbloco;
	bool flagFixo;
	VTBlocoRad *blSup;

	// Retorna lista de blocos a jusante até a próxima chave com medicao seguinte
	// e lista de blocos da proxima chave de medição até o final do circuito

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	if ((indBl = IndiceChave(pchave)) < 0)
		return;
	// Define blocoRad de suprimento
	blSup = ((VTLisBlocoChave*)lisBlocoChv->Items[indBl])->blSup;

	// Adiciona a lisExt os blocos a jusante de pchave
	lisExt->Assign(LisBlJusChave(pchave), laOr);
	for (int n = 0; n < lisChavesMed->Count; n++)
	{
		pchave = (VTChave*)lisChavesMed->Items[n];
		indBlAux = IndiceChave(pchave);
		// Verifica se chave tem mesmo suprimento
		if (((VTLisBlocoChave*)lisBlocoChv->Items[indBlAux])->blSup != blSup)
			continue;

		if (indBlAux <= indBl)
			continue;
		lisBlocosAux = LisBlJusChave(pchave);
		if (lisBlocosAux->Count == 0)
			continue;
		pbloco = (VTBloco*)lisBlocosAux->First();
		flagFixo = (lisExt->IndexOf(pbloco) != -1);
		for (int nb = 0; nb < lisBlocosAux->Count; nb++)
		{
			lisExt->Remove(lisBlocosAux->Items[nb]);
			if (flagFixo && lisExtFixo)
				lisExtFixo->Add(lisBlocosAux->Items[nb]);
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::LisBlocoChave(VTBloco *bloco, TList *lisExt)
{
	VTLisBlocoChave *pblChave;

	if (!bloco)
		return;

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return;
	}

	for (int n = 0; n < lisBlocoChv->Count; n++)
	{
		pblChave = (VTLisBlocoChave*)lisBlocoChv->Items[n];
		if (pblChave->blJus->Bloco == bloco)
			lisExt->Add(pblChave);
	}
}

// ---------------------------------------------------------------------------
VTLisBlocoChave* __fastcall TBuscaProt::IdentificaBlocoChave(VTChave *pchv)
{
	VTLisBlocoChave *plisBlocoChave;
	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(NULL);}

	plisBlocoChave = NULL;
	for (int n = 0; n < lisBlocoChv->Count; n++)
	{
		if ((plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[n])->pchv == pchv)
			break;
		plisBlocoChave = NULL;
	}

	return (plisBlocoChave);
}

// ---------------------------------------------------------------------------
int __fastcall TBuscaProt::IndiceChave(VTChave *pchv)
{
	// -----------------------------------------------------------
	// Funcao que retorna índice de lisBlocoChv associado a pchv//
	// -----------------------------------------------------------

	if (!pchv)
		return (lisBlocoChv->Count);

	// Verifica se a classe já foi inicializada
	if (!flagInicia)
	{
		if (!Inicia())
			return (-1);
	}

	for (int n = 0; n < lisBlocoChv->Count; n++)
	{
		if (((VTLisBlocoChave*)lisBlocoChv->Items[n])->pchv == pchv)
			return (n);
	}
	return (-1);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::MontaLisBlocos(VTLisBlocoChave *plisBlocoChave)
{
	plisBlocoChave->lisBlJ->Clear();
	if (plisBlocoChave->blMon == NULL)
		return (false);
	// hk201611:?
	if (plisBlocoChave->pchv->Aberta)
	{
		int kkk = 0;
		return (false);
	}
	if (plisBlocoChave->blMon == plisBlocoChave->blSup) // hkedp
	{
		if (plisBlocoChave->blJus == NULL)
			return (false);
		plisBlocoChave->lisBlJ->Assign(plisBlocoChave->blJus->LisBlocoJus, laOr);
		plisBlocoChave->lisBlJ->Add(plisBlocoChave->blJus->Bloco);
	}
	else
	{
		plisBlocoChave->lisBlJ->Assign(plisBlocoChave->blMon->LisBlocoJus, laOr);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::OrdenaChaves(bool flagExeBlocos)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTBloco *blocoSup, *blocoJus, *blocoAux;
	VTBlocoRad *blSup, *blSupAux, *blAux, *blJus, *blMon;
	VTVisRede *visRede;
	TList *lisAux, *lisAux1, *lisAux2, *lisBlM, *lisChvAux, *lisRedu, *lisBarra;
	VTChave *pchave;
	VTLisBlocoChave *plisBlocoChave;
	VTReducao* redu;
	VTElo *elo;
	VTBarra *pbarra;
	bool resposta = true;
	///////////////////////////////////////////////////////////////////////////////
	// Função que gera:                                                           //
	// lisBlocoRad->blocoRad de suprimento --> arvore de blocosRad até chaves NA//
	// lisBlocoChv->TLisBlocoChave : ordenação de chaves                        //
	// lisVisRede->visRede-> (bloco1/2, rede1/2, pbarra1/2)
	///////////////////////////////////////////////////////////////////////////////

	// Inicia os blocos com a última versão de rede
	if (flagExeBlocos)
	{
		if (!blocos->Executa(redes))
			resposta = false;
	}

	lisAux = new TList();
	lisAux1 = new TList();
	lisAux2 = new TList();
	lisBlM = new TList();
	lisChvAux = new TList();

	// PEND suprimentos em redes reduzidas

	// Reinicia lisBlocoRad e lisBlocoChv
	LimpaTList(lisBlocoRad);
	LimpaTList(lisBlocoChv);
	///////////////////////////////////////////////////////////////////////////////
	// Popula lisBlocoRad -> lista de blocos que aponta para blocos de suprimento
	// e lisBlocoChv
	///////////////////////////////////////////////////////////////////////////////
	// Busca bloco de suprimento percorre em lista de barras do bloco se existe geracao
	lisAux->Clear();
	blocos->LisBlocoGeracao(lisAux);

	// Adiciona blocos ligados a redes equivalentes de suprimento
	// REDUÇÕES COM SUPRIMENTO
	lisRedu = new TList();
	redes->LisReducao(lisRedu);
	for (int nr = 0; nr < lisRedu->Count; nr++)
	{
		redu = (VTReducao*)lisRedu->Items[nr];
		if (!redu->ExisteGeracao())
			continue;
		// Barras da
		lisBarra = redu->LisBarra();
		for (int nbar = 0; nbar < lisBarra->Count; nbar++)
		{
			pbarra = (VTBarra*)lisBarra->Items[nbar];
			blocoAux = blocos->ExisteBloco(pbarra);
			if (lisAux->IndexOf(blocoAux) == -1)
				lisAux->Add(blocoAux);
		}
	}
	delete lisRedu;

	// hkv201608 Método para ordenacao dos blocos de geracao em blocos ligados a
	// redes equivalentes com suprimento, blocos com gerador e blocos com suprimento vteta
	OrdenaLisBlocosGeracao(lisAux);

	// Verifica se existem blocos de suprimento na rede
	if (lisAux->Count == 0)
	{
		// hk201612 Aviso("Rede sem Suprimento");
		return (false);
	}

	// Popula lisBlocoRad -> lista de blocos que aponta para blocos de suprimento
	for (int n = 0; n < lisAux->Count; n++) // for(int n = lisAux->Count - 1; n >= 0; n--)
	{
		blocoSup = (VTBloco*)lisAux->Items[n];
		lisBlocoRad->Add(blSup = NewObjBlocoRad(blocoSup, apl));
		// hkv201610: definição de rede de bloco: inclusao de apl//new TBlocoRad(blocoSup));
		blSup->FlagSup = true;
	}

	// Gera lista em arvore a partir de objetos em lisBlocoRad
	// e popula lisBlocoChv que aponta para chaves da rede

	lisAux->Clear();
	for (int nsup = lisBlocoRad->Count - 1; nsup >= 0;	nsup--) // for(int nsup = 0; nsup < lisBlocoRad->Count; nsup++)
	{
		blSup = (VTBlocoRad*)lisBlocoRad->Items[nsup];
		if (blSup->FlagSup == false)
		{
			lisBlocoRad->Delete(nsup);
			delete blSup; // hk201611: delecao de fontes secundarias em paralelo
			continue;
		}
		lisAux->Add(blSup);
		lisBlM->Add(blSup->Bloco);
		while (lisAux->Count > 0)
		{
			blAux = (VTBlocoRad*)lisAux->First();
			lisAux->Delete(0);

			// hkv201608 Verifica se há múltiplos suprimentos, se sim elimina segundo ponto de geração (rede equiv, gerador, suprimento)
			for (int nn = 0; nn < lisBlocoRad->Count; nn++)
			{
				blSupAux = (VTBlocoRad*)lisBlocoRad->Items[nn];
				if ((blSupAux->Bloco == blAux->Bloco) && (blSupAux != blSup))
				{
					blSupAux->FlagSup = false;
					break;
				}
			}
			lisChvAux->Clear();
			// Percorre chaves do bloco
			blocos->LisChaveBloco(blAux->Bloco, lisChvAux);
			for (int nc = 0; nc < lisChvAux->Count; nc++)
			{
				pchave = (VTChave*)lisChvAux->Items[nc];
				// Se chave NA
				if (pchave->Aberta)
				{
					// Busca plisBlocoChave em lisBlocoChv
					plisBlocoChave = NULL;
					for (int nc = 0; nc < lisBlocoChv->Count; nc++)
					{
						plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
						if (plisBlocoChave->pchv == pchave)
							break;
						plisBlocoChave = NULL;
					}
					if (plisBlocoChave == NULL)
					{
						lisBlocoChv->Add(plisBlocoChave = NewObjLisBlocoChave(pchave));
						plisBlocoChave->blMon = blAux;
						plisBlocoChave->blSup = blSup;
					}
					else
					{
						plisBlocoChave->blMon1 = blAux;
						plisBlocoChave->blSup1 = blSup;
					}
					continue;
				}
				// Se Chave NA

				// Se Chave NF
				elo = blocos->ExisteElo(pchave);
				blocoJus = elo->Bloco1;
				if (elo->Bloco1 == blAux->Bloco)
					blocoJus = elo->Bloco2;
				if (lisBlM->IndexOf(blocoJus) != -1)
					continue;
				lisBlM->Add(blocoJus);
				lisAux->Add(blJus = NewObjBlocoRad(blocoJus, apl));
				// hkv201610: definição de rede de bloco: inclusao de apl//new TBlocoRad(blocoAux));
				blAux->LisBlocoRad->Add(blJus);
				blJus->Chave = pchave; // chave de suprimento
            blJus->BarraChave = BuscaBarraChave(pchave, blJus->Bloco);
				blJus->BlocoRadMon = blAux;

				// Adiciona blJus a lista de blocos a jusante
				blMon = blJus;
				while (blMon->BlocoRadMon != NULL)
				{
					blMon = blMon->BlocoRadMon;
					blMon->LisBlocoRadJus->Add(blJus);
					blMon->LisBlocoJus->Add(blJus->Bloco);
				}
				// Cria objeto lisBlocoChave
				lisBlocoChv->Add(plisBlocoChave = NewObjLisBlocoChave(pchave));
				blJus->LisBlocoChave = plisBlocoChave;
				// hk201611: criacao de ponteiro de blocoRad para plisBlocoChave a montante
				plisBlocoChave->blSup = blSup;
				plisBlocoChave->blMon = blAux;
				plisBlocoChave->blJus = blJus;
			}
		}
	}

	VTBlocoRad *blocoRad;
	// lisAux1->Clear();
	// Elimina blocos a montante de disjuntor
	if (lisDjPri->Count > 0)
	{
		// Busca blocoRad->Chave pertence a lisDjPri
		lisAux->Clear();
		lisAux1->Clear();
		lisAux->Assign(lisDjPri, laOr);
		while (lisAux->Count > 0)
		{
			pchave = (VTChave*)lisAux->First();
			lisAux->Delete(0);
			if ((blAux = IdentificaBlocoRad(pchave)) == NULL)
				continue;
			blAux->FlagSup = true;
			lisAux1->Add(blAux);
		}

		// hkv201610 flagSup = false para disjuntores a jusante de disjunto em série
		for (int nsup = 0; nsup < lisAux1->Count; nsup++)
		{
			blocoRad = (VTBlocoRad*)lisAux1->Items[nsup];
			lisAux->Clear();
			lisAux->Assign(blocoRad->LisBlocoRad);

			while (lisAux->Count > 0)
			{
				blocoRad = (VTBlocoRad*)lisAux->First();
				lisAux->Delete(0);
				if (blocoRad->FlagSup == true)
				{
					blocoRad->FlagSup = false;
				}
				lisAux->Assign(blocoRad->LisBlocoRad, laOr);
			}
		}
		TList *lisBlocoRadAux;
		// hk201611: caso de lisDJPri->Count > 0: eliminação de suprimentos acima de disjuntor
		TList *lisBlocoChvAux; // hk201611: eliminação de suprimentos acima de disjuntor
		lisBlocoRadAux = new TList(); // hk201611: eliminação de suprimentos acima de disjuntor
		lisBlocoChvAux = new TList(); // hk201611: eliminação de suprimentos acima de disjuntor
		for (int nsup = 0; nsup < lisAux1->Count; nsup++)
		{
			blAux = (VTBlocoRad*)lisAux1->Items[nsup];
			if (blAux->FlagSup == false)
			{
				continue;
			}
			// Corta blocos a montande do disjuntor
			lisBlocoRadAux->Add(blAux);
			// hk201611: eliminação de suprimentos acima de disjuntor: lisBlocoRad->Add(blAux);
			if (blAux->BlocoRadMon != NULL)
			{
				blAux->BlocoRadMon->LisBlocoRad->Remove(blAux);
				blAux->BlocoRadMon = NULL;
			}

		}
		// Refaz lista de blocosRad de suprimento
		LimpaTList(lisBlocoRad); // hk201611: eliminação de suprimentos acima de disjuntor
		lisBlocoRad->Assign(lisBlocoRadAux, laOr);
		// hk201611: eliminacao de blocochaveAux
		lisAux2->Clear();
		ListaBlocoRadTotal(lisAux2);
		for (int n = lisBlocoChv->Count - 1; n >= 0; n--)
		{
			plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[n];
			if (plisBlocoChave->pchv->Aberta)
			{
				if ((lisAux2->IndexOf(plisBlocoChave->blMon) != -1) &&
					(lisAux2->IndexOf(plisBlocoChave->blMon1) != -1))
					continue;
			}
			else
			{
				if (lisAux2->IndexOf(plisBlocoChave->blJus) != -1)
					continue;
			}
			lisBlocoChvAux->Add(plisBlocoChave);
			lisBlocoChv->Remove(plisBlocoChave);
		}
		LimpaTList(lisBlocoChvAux); // hk201611: eliminação de suprimentos acima de disjuntor
		delete lisBlocoRadAux;
		delete lisBlocoChvAux;

	}

	//////////////////////////////////////////////////////////////
	// Preenche blocos a jusante de chave, plisBlocoChave->lisBlJ//
	//////////////////////////////////////////////////////////////
	for (int nc = 0; nc < lisBlocoChv->Count; nc++)
	{
		plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
		MontaLisBlocos(plisBlocoChave);
	}

	////////////////////////////////////////////////////
	// Preenche lista de blocos a montante por blocoRad//
	////////////////////////////////////////////////////

	// Define lista de blocos iniciais de circuitos
	// primários baseado em lista de disjuntores
	TList *lisBlocoRadSupAux;
	int iconta;
	lisBlocoRadSupAux = new TList();
	lisBlocoRadSupAux->Assign(lisBlocoRad, laOr);
	for (int nsup = 0; nsup < lisBlocoRadSupAux->Count; nsup++)
	{
		blSup = (VTBlocoRad*)lisBlocoRadSupAux->Items[nsup];
		blSup->LisBlocoMon->Clear();
		lisAux1->Clear();
		lisAux->Clear();
		// Preenche lisAux1: lista com blRad extremos
		lisAux->Add(blSup);
		while (lisAux->Count > 0)
		{
			blAux = (VTBlocoRad*)lisAux->First();
			lisAux->Delete(0);
			if (blAux->LisBlocoRad->Count == 0)
			{
				if (lisAux1->IndexOf(blAux) == -1)
					lisAux1->Add(blAux);
			}
			else
			{
				lisAux->Assign(blAux->LisBlocoRad, laOr);
			}
		}
		lisAux->Clear();
		for (int nb = 0; nb < lisAux1->Count; nb++)
		{
			blAux = (VTBlocoRad*)lisAux1->Items[nb];
			blAux->LisBlocoMon->Add(blSup->Bloco);
			blAux->LisBlocoMon->Assign(blSup->LisBlocoJus, laOr);
			blAux->LisBlocoMon->Remove(blAux->Bloco);
			// BlRad já percorrido
			lisAux->Add(blAux);
			blJus = blAux;
			blAux = blJus->BlocoRadMon;
			while (blAux != NULL)
			{
				if (lisAux->IndexOf(blAux) == -1)
				{
					blAux->LisBlocoMon->Assign(blJus->LisBlocoMon, laOr);
					blAux->LisBlocoMon->Remove(blAux->Bloco);
					lisAux->Add(blAux);
				}
				blJus = blAux;
				blAux = blJus->BlocoRadMon;
			}
		}
	}
	delete lisBlocoRadSupAux;
	////////////////////////////
	// Monta lisChaves Vis     //
	////////////////////////////
	// Reinicia lisVisRede
	LimpaTList(lisVisRede);
	// Reinicia lista de chaves vis
	lisChavesVis->Clear();
	for (int nc = 0; nc < lisBlocoChv->Count; nc++)
	{
		plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
		// Filtra chaves NF
		if (plisBlocoChave->pchv->Fechada)
			continue; // if(plisBlocoChave->pchv->Estado == chvFECHADA) continue; //hkRevAloca
		// Filtra chaves NA sem ligacao
		if ((plisBlocoChave->blMon == NULL) || (plisBlocoChave->blMon1 == NULL))
			continue;
		// Descarta blocoChave sem rede em uma das ligacoes hkSH_ComSE
		// hkv201610: definição de rede de bloco
		// if((plisBlocoChave->blMon->Bloco->Rede == NULL)||(plisBlocoChave->blMon1->Bloco->Rede == NULL)) continue;
		if ((plisBlocoChave->blMon->Rede == NULL) || (plisBlocoChave->blMon1->Rede == NULL))
			continue;
		// Filtra chaves com o mesmo suprimento (chaves NA internas, bypass)
		// hkv201610: definição de rede de bloco
		// if(plisBlocoChave->blMon->Bloco->Rede == plisBlocoChave->blMon1->Bloco->Rede) continue;
		if (plisBlocoChave->blMon->Rede == plisBlocoChave->blMon1->Rede)
			continue;
		// if(plisBlocoChave->blSup == plisBlocoChave->blSup1) continue;

		// Cria lisVisRede
		visRede = NewObjVisRede(plisBlocoChave->pchv);
		lisVisRede->Add(visRede);
		visRede->plisBlocoChave = plisBlocoChave;

		visRede->blocoRad1 = plisBlocoChave->blMon;
		visRede->bloco1 = plisBlocoChave->blMon->Bloco;
		// hkv201610: definição de rede de bloco
		// visRede->rede1 = plisBlocoChave->blMon->Bloco->Rede;
		visRede->rede1 = plisBlocoChave->blMon->Rede;

		visRede->blocoRad2 = plisBlocoChave->blMon1;
		visRede->bloco2 = plisBlocoChave->blMon1->Bloco;
		// hkv201610: definição de rede de bloco
		// visRede->rede2 = plisBlocoChave->blMon1->Bloco->Rede;
		visRede->rede2 = plisBlocoChave->blMon1->Rede;

		//hk201805a: Correcao if (plisBlocoChave->blMon->Bloco->LisBarra()->IndexOf(pchave->pbarra1) != -1)
		if (plisBlocoChave->blMon->Bloco->LisBarra()->IndexOf(visRede->pchave->pbarra1) != -1)
		{
			visRede->pbarra1 = plisBlocoChave->pchv->pbarra1;
			visRede->pbarra2 = plisBlocoChave->pchv->pbarra2;
		}
		else
		{
			visRede->pbarra1 = plisBlocoChave->pchv->pbarra2;
			visRede->pbarra2 = plisBlocoChave->pchv->pbarra1;
		}
      //if(visRede->bloco1->LisBarra()->IndexOf(visRede->pbarra1) == -1)
      //   int kkk = 0;
 		////////////////////////////////////////////////////////////////////////////////////////
		// Funcao que gera para lisVisRede->TVisRede:                                          //
		// lista de ligacoes       até o suprimento: TVisRede->lisLiga(rede1),  lisLiga(rede2) //
		// lista de blocos         até o suprimento: TVisRede->lisBloco(rede1), lisBloco(rede2)//
		////////////////////////////////////////////////////////////////////////////////////////
		LisLigaVis(visRede);

		// LisLigaVis(visRede->pchave, visRede->pbarra1, visRede->lisLiga1, visRede->lisBlocosVis1);
		// LisLigaVis(visRede->pchave, visRede->pbarra2, visRede->lisLiga2, visRede->lisBlocosVis2);

		// Adiciona chave NA entre circuitos a lisChavesVis
		lisChavesVis->Add(visRede->pchave);
	}

	////////////////////////////////////////////////////////////////////////////////
	// Para chaves de protecao NF gera lista de blocos até o proximo equipamento   //
	////////////////////////////////////////////////////////////////////////////////
	// for(int nc = 0; nc < lisBlocoChv->Count; nc++)
	// {
	// plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
	// Exclui chaves NA
	// if(plisBlocoChave->pchv->Aberta) continue;
	// Verifica se é chave de protecao
	// if(lisTipoEqProt->IndexOf(plisBlocoChave->pchv->TipoChave) == -1) continue;
	// Protecao caso seja uma chave NF ligada a nada
	// if(plisBlocoChave->blJus == NULL) continue;
	// Preenche lista de blocos a jusante até as chaves de proteção, funcao de lisTipoEqProt (já tem que ter definido em leitura!)
	// ListaZonaProtJus(plisBlocoChave->blJus, plisBlocoChave->blJus->LisBlocoJusProt);
	// }

	delete lisAux;
	delete lisAux1;
	delete lisAux2;
	delete lisBlM;
	delete lisChvAux;

	return (true);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TBuscaProt::BuscaBarraChave(VTChave *chave, VTBloco *bloco)
   {
   TList *lisBarBloco;
   VTBarra *barraChave;

   lisBarBloco = bloco->LisBarra();
   barraChave = chave->pbarra1;
   if(lisBarBloco->IndexOf(chave->pbarra2) != -1) barraChave = chave->pbarra2;

   return(barraChave);
   }
// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::OrdenaLisBlocosGeracao(TList *lisBlocoGeracao)
{
	// hkv201608 OrdenaLisBlocosGeracao: Método para ordenacao dos blocos de geracao em blocos ligados a
	// redes equivalentes com suprimento, blocos com gerador e blocos com suprimento vteta
	VTBloco *bloco;
	VTBarra *barra;
	TList *lisAux = new TList();
	TList *lisSuprimento = new TList();

	lisAux->Assign(lisBlocoGeracao, laOr);
	lisBlocoGeracao->Clear();

	for (int n = lisAux->Count - 1; n >= 0; n--)
	{
		bloco = (VTBloco*)lisAux->Items[n];
		lisSuprimento->Clear();
		bloco->LisEqbar(lisSuprimento, eqptoSUPRIMENTO);
		if (lisSuprimento->Count > 0)
		{
			lisAux->Delete(n);
			lisBlocoGeracao->Add(bloco);
		}
	}
	for (int n = lisAux->Count - 1; n >= 0; n--)
	{
		bloco = (VTBloco*)lisAux->Items[n];
		lisSuprimento->Clear();
		bloco->LisEqbar(lisSuprimento, eqptoGERADOR);
		if (lisSuprimento->Count > 0)
		{
			lisAux->Delete(n);
			lisBlocoGeracao->Insert(0, bloco);
		}
	}
	for (int n = 0; n < lisAux->Count; n++)
		lisBlocoGeracao->Insert(0, lisAux->Items[n]);

	delete lisAux;
	delete lisSuprimento;
}

//---------------------------------------------------------------------------
void __fastcall TBuscaProt::PreparaIndicacaoFalta(void)   //hk201710IF  //hk201808: reformulação: sai para buscaProt
   {
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList *lisChavesProt, *lisChavesProtMont;
   VTChave *pchv, *pchvProt;
   TList   *lisBlocosProtJus;
   VTBloco *bloco;
   double  compBloco;
   strIF   *infIF, *infIFProt;
   int     index;

   //Funcao que gera, por chave de protecao e chave com identif. de falta, IF
   //infIF = (chave, compProt, compIF)

   //Limpa a lista
   for(int n = 0; n < lisEQIF->Count; n++)
      {
      delete ((strIF*)lisEQIF->Items[n])->lisBlJus;
      delete ((strIF*)lisEQIF->Items[n])->lisBlProt;
      }
   LimpaTList(lisEQIF);

   lisChavesProt= new TList();
   redes->LisEqpto(lisChavesProt, eqptoCHAVE);
   //Cria lista de chaves de protecao
   for(int n = lisChavesProt->Count - 1; n >= 0; n--)
      {
      pchv = (VTChave*)lisChavesProt->Items[n];
      if(lisTipoEqProt->IndexOf(pchv->TipoChave) != -1) continue;
      lisChavesProt->Delete(n);
      }

   lisBlocosProtJus = new TList();
   for(int n = 0; n < lisChavesProt->Count; n++)
      {
      pchv = (VTChave*)lisChavesProt->Items[n];

      lisEQIF->Add(infIF = new strIF);
      infIF->lisBlJus = new TList();
      infIF->lisBlProt= new TList();

      if(lisChavesIF->IndexOf(pchv) != -1) infIF->flagIF = true;
      else infIF->flagIF = false;

      infIF->chave    = pchv;
      infIF->chaveProt= pchv;
      infIF->compProt = 0.0;
      infIF->compIF   = 0.0;
      //preenche buscaProt->LisEQIF para as chaves de protecao
      //1-Cálculo de compProt, distância total entre equipamentos de protecao
      ListaZonaProtJus(pchv, lisBlocosProtJus, NULL);
      infIF->lisBlProt->Assign(lisBlocosProtJus, laOr);
      for(int nb = 0; nb < lisBlocosProtJus->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocosProtJus->Items[nb];
         infIF->compProt += ComprimentoBloco(bloco);
         }


      //2-Cálculo de compIF, distância total entre equipamentos de protecao e chaves com IF, identif. de falta
      ListaZonaProtIFJus(pchv, lisBlocosProtJus, NULL);
      infIF->lisBlJus->Assign(lisBlocosProtJus, laOr);
      for(int nb = 0; nb < lisBlocosProtJus->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocosProtJus->Items[nb];
         infIF->compIF += ComprimentoBloco(bloco);
         }
      }

   lisChavesProtMont = new TList();
   for(int n = 0; n < lisChavesIF->Count; n++)
      {
		pchv = (VTChave*)lisChavesIF->Items[n];
      if(lisChavesProt->IndexOf(pchv) != -1) continue;

      lisEQIF->Add(infIF = new strIF);
      infIF->lisBlJus = new TList();
      infIF->lisBlProt= new TList();
      infIF->flagIF   = true;

      infIF->chave    = pchv;
      infIF->compProt = 0.0;
      infIF->compIF   = 0.0;
      //preenche buscaProt->LisEQIF para as chaves com IF, identificador de falta
      IdentificaChvMont(pchv, lisChavesProtMont, true);
      //proteca
      if(lisChavesProtMont->Count == 0) continue;
      pchvProt = (VTChave*)lisChavesProtMont->First();
      infIF->chaveProt= pchvProt;

      index = lisChavesProt->IndexOf(pchvProt);
      infIFProt = (strIF*)lisEQIF->Items[index];
      infIF->compProt = infIFProt->compProt;
      infIF->lisBlProt->Assign(infIFProt->lisBlProt, laOr);

      //2-Cálculo de compIF, distância total entre equipamentos de protecao e chaves com IF, identif. de falta
      ListaZonaProtIFJus(pchv, lisBlocosProtJus, NULL);
      infIF->lisBlJus->Assign(lisBlocosProtJus, laOr);

      for(int nb = 0; nb < lisBlocosProtJus->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocosProtJus->Items[nb];
         infIF->compIF += ComprimentoBloco(bloco);
         }
      }

   delete lisChavesProt;
   delete lisChavesProtMont;
   delete lisBlocosProtJus;
   }
//---------------------------------------------------------------------------
double __fastcall TBuscaProt::ComprimentoBloco(VTBloco *bloco) //hk201808: reformula
   {
   TList *lisLiga = bloco->LisLigacao();
   VTEqpto *pliga;
   double    comp_km = 0.0;

   for(int n = 0; n < lisLiga->Count; n++)
      {
      pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
      comp_km += ((VTTrecho*)pliga)->Comprimento_km;
      }
   return(comp_km);
   }

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::PreparaDefeito(VTDefeito *pdef) // hkedp , TList *lisChavesAtivas)
{
	// variáveis locais
	bool flagProt, flagFluxo, flagAuto;
	VTChave *pchave;
	int iconta;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Preenche listas
	// ->VTChave: pdef->LisProt, pdef->LisChvMon
	// define VTChave: pdef->ChvProt, pdef->ChvIsolaMont, pdef->ChvIsolaAutoMont
	// Preenche LISTA DE BLOCOS A JUSANTE DA CHAVE DE PROTECAO
	// ->VTBloco: pdef->LisBlocosJusProt;
	// Preenche LISTA DE BLOCOS A JUSANTE DE CHAVE AUTOMATICA DE ISOLAMENTO A MONTANTE
	// ->VTBloco: pdef->LisBlocosJusAuto
	// Preenche LISTA DE BLOCOS A JUSANTE DO DEFEITO
	// ->VTBloco: pdef->LisBlocosJusDef
	// Preenche LISTA DE BLOCOS ENTRE A CHAVE DE PROTECAO E O BLOCO DE DEFEITO
	// ->VTBloco:pdef->LisBlocosProtDef
	// Preenche LISTA DE BLOCOS ENTRE CHAVE AUTO DE ISOLAM A MONTANTE E BLOCO DE DEFEITO
	// ->VTBloco: pdef->LisBlocosAutoDef
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Função LisBlManLis(pdef):
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Funcao que preenche a partir de lisBlocoDef contíguos (bloco de carga com o defeito), as listas abaixo:
	// pdef->LisBlocosMan(lisEXTBloco)        : lista de blocos a jusante do defeito, com possibilidade de transferencia
	// pdef->LisBlocosVis(lisEXTBlocoVis)     : conjunto de blocos troncais nos circuitos de socorro da chave vis ao suprimento
	// pdef->LisChvVisDef(lisEXTChv)          : conjunto de chaves NA de vis que podem transferir blocos
	// pdef->LisVisRedeDef(lisVisRedeDef)     : conjunto de visRede correspondedente a pdef->LisChvVisDef
	// pdef->LisBlocoRadJusDef                : conjunto de blocosRad imediatamente a jusante do defeito e com possibilidad de transferÊncia
	// e seta para as visRede do defeito:
	// visRede->BlocoDesl    = visRede->bloco1 ou visRede->bloco2
	// visRede->BlocoRadDesl = visRede->blocoRad1 ou visRede->bloco2
	// visRede->lisBlocoRadIsolaJus: lista de blocoRad até da chave vis ao defeito
	// visRede->chvJus : chave imediatamente a jusante do defeito e tem ligacao a visRede
	// visRede->blRadJusDef: blocoRad imediatamente a jusante do defeito e com ligacao a visRede
	// visRede->lisBlocosDefVis: lista de blocos com possibilidade de suprimento por visRede
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// IDENTIFICA CHAVES A MONTANTE DO DEFEITO, DE PROTECAO E TODAS
	IdentificaChvMont(pdef->Bloco, pdef->LisProt, flagProt = true);
	// hk03LisBlocosAgrega inclusao de chaves ativas
	IdentificaChvMont(pdef->Bloco, pdef->LisChvMon, flagProt = false);
	// hk03LisBlocosAgrega inclusao de chaves ativas
	// IDENTIFICA CHAVES A MONTANTE DO DEFEITO, DE PROTECAO E TODAS
	pdef->ChvProt = (pdef->LisProt->Count > 0) ? (VTChave*)pdef->LisProt->Items[0] : NULL;

	// Protecao
	if (pdef->ChvProt == NULL)
		return (false);
   //hk201711: taxa de descoordenação de proteção
   if(iProfDescoord > 0)
     {
     int indexProt;
     if(iProfDescoord < pdef->LisProt->Count)
         pdef->ChvProt = (VTChave*)pdef->LisProt->Items[iProfDescoord];
     }
	// IDENTIFICACAO DE CHAVE DE ISOLAMENTO A MONTANTE
	// Fixa chave para isolar o defeito 1a chave montante
	pdef->ChvIsolaMont = (pdef->LisChvMon->Count > 0) ? (VTChave*)pdef->LisChvMon->Items[0] : NULL;
	// IDENTIFICA CHAVE RELIGADORA QUE RELIGOU EM CASO DE DEFEITO TEMPORÁRIO
	// Admite seletividade com a primeira chave a montante

   //IDENTIFICA CHAVE DE PROTEÇÃO ATUADA OU CHAVE COM IF INSTALADO
   pchave = NULL;
   for(int n = 0; n < pdef->LisChvMon->Count; n++)
      {
      pchave = (VTChave*)pdef->LisChvMon->Items[n];
      if(ChaveProtecao(pchave)) break;
      if(lisChavesIF->IndexOf(pchave) != -1) break;
      pchave = NULL;
      }
   //hk201808: reformula
   strIF *infIF;
   if(pchave != NULL)
     {
     //Associa a defeito o objeto com informações do IF
     infIF= NULL;
     for(int n = 0; n < lisEQIF->Count; n++)
        {
        infIF = (strIF*)lisEQIF->Items[n];
        if(infIF->chave == pchave) break;
        infIF = NULL;
        }
     pdef->InfIF = infIF; //associado a chave de proteção a montante de pdef ou a primeiro IF a montante do bloco de defeito
     }

	// hkrel
	// profundidade : até o primeiro elemento de religamento, RELIGADOR OU DISJUNTOR
	pdef->ChvReliga = NULL; // hkSH
	switch (iProfReliga)
	{
	case 0: // somente o bloco imediatamente a jusante
		if (pdef->ChvIsolaMont != NULL)
		{
			if (ChaveReligadora(pdef->ChvIsolaMont))
				pdef->ChvReliga = pdef->ChvIsolaMont;
		}
		break;
	case 1: // até o próximo religador
		for (int n = 0; n < pdef->LisProt->Count; n++)
		{
			pdef->ChvReliga = (VTChave*)pdef->LisProt->Items[n];
			// if((pdef->ChvReliga->TipoChave->Tipo == chaveRELIGADORA)||(pdef->ChvReliga->TipoChave->Tipo == chaveDISJUNTOR))
			// Alteracao provisoria para representar zona de religamento hk201610
			// hkv201610 - temporário para estudo EDRO
			// if(pdef->ChvReliga->TipoChave->Codigo.AnsiCompare("RELIGADOR DE TENSAO") == 0)
			// {
			// pdef->ChvReliga = NULL;
			// break;
			// }

			if (ChaveReligadora(pdef->ChvReliga))
				break;
			pdef->ChvReliga = NULL;
		}
		break;
	}

	/*
	 //hkrel
	 //profundidade máxima n=0 ou n=1 para tentativas de religamento
	 pdef->ChvReliga = NULL; //hkSH
	 for(int n = 0; (n < 2)&&(n < pdef->LisProt->Count); n++)//pdef->LisProt->Count; n++)
	 {
	 pdef->ChvReliga = (VTChave*)pdef->LisProt->Items[n];
	 if((pdef->ChvReliga->TipoChave->Tipo == chaveRELIGADORA)||(pdef->ChvReliga->TipoChave->Tipo == chaveDISJUNTOR))
	 break;
	 pdef->ChvReliga = NULL;
	 }
	 */
	// Define Chave isolamento a montante automática telecomandada
	// pdef->ChvIsolaMontAuto =...

	// Fixa chave automática para isolar o defeito a montante
	// parou- definicao de chave a montante telecomandada
	// pdef->ChvIsolaMontAuto = BuscaChaveAuto(pdef->LisChvMon);

	// Inicia combo com chaves a montante

	iconta = 0;
	pdef->ChvIsolaAutoMont = NULL;
	flagAuto = false;
	do
	{
		pdef->ChvIsolaAutoMont = (VTChave*)pdef->LisChvMon->Items[iconta];
		iconta++;
	}
	while ((!(flagAuto = ChaveAutomatica(pdef->ChvIsolaAutoMont))) &&
		(pdef->ChvIsolaAutoMont != pdef->ChvProt));

	if ((!flagAuto) || (pdef->ChvProt == pdef->ChvIsolaAutoMont))
		pdef->ChvIsolaAutoMont = NULL;

	// GERA LISTA DE BLOCOS A JUSANTE DA CHAVE DE PROTECAO
	LisBlJus(pdef->ChvProt, pdef->LisBlocosJusProt);

	// GERA LISTA DE BLOCOS A JUSANTE DE CHAVE AUTOMATICA DE ISOLAMENTO A MONTANTE
	if ((pdef->ChvIsolaAutoMont != NULL)) // (pdef->ChvIsolaAutoMont != pdef->ChvIsolaMont)&&
			LisBlJus(pdef->ChvIsolaAutoMont, pdef->LisBlocosJusAuto);

	// GERA LISTA DE BLOCOS A JUSANTE DO DEFEITO
	LisBlJus(pdef->ChvIsolaMont, pdef->LisBlocosJusDef);
	// pdef->LisBlocosJusDef->Remove(pdef->Bloco);//hk03LisBlocosAgrega
	for (int nb = 0; nb < pdef->LisBlocosAgrega->Count; nb++)
		pdef->LisBlocosJusDef->Remove(pdef->LisBlocosAgrega->Items[nb]);

	// GERA LISTA DE BLOCOS ENTRE A CHAVE DE PROTECAO E O BLOCO DE DEFEITO
	pdef->LisBlocosProtDef->Clear();
	pdef->LisBlocosProtDef->Assign(pdef->LisBlocosJusProt, laCopy);//hk201902 laOr);
	// hk03LisBlocosAgrega pdef->LisBlocosProtDef->Remove(pdef->Bloco);
	for (int nb = 0; nb < pdef->LisBlocosAgrega->Count; nb++)
		pdef->LisBlocosProtDef->Remove(pdef->LisBlocosAgrega->Items[nb]); // hk03LisBlocosAgrega

	// E LISTA DE BLOCOS ENTRE CHAVE AUTO DE ISOLAM A MONTANTE E BLOCO DE DEFEITO
	if (pdef->ChvIsolaAutoMont != NULL) // (pdef->ChvIsolaAutoMont != pdef->ChvIsolaMont)&&
	{
		pdef->LisBlocosAutoDef->Clear();
		pdef->LisBlocosAutoDef->Assign(pdef->LisBlocosJusAuto, laCopy); //hk201902: Assign(pdef->LisBlocosJusAuto, laCopy)
		// hk03LisBlocosAgrega pdef->LisBlocosAutoDef->Remove(pdef->Bloco);
		for (int nb = 0; nb < pdef->LisBlocosAgrega->Count; nb++)
			pdef->LisBlocosAutoDef->Remove(pdef->LisBlocosAgrega->Items[nb]); // hk03LisBlocosAgrega
	}

	if (pdef->LisBlocosProtDef->Count > 0)
	{ // Remove blocos a jusante do defeito de lisBlocosProtDef e lisBlocosAutoDef
		for (int n = 0; n < pdef->LisBlocosJusDef->Count; n++)
		{
			pdef->LisBlocosProtDef->Remove((VTBloco*)pdef->LisBlocosJusDef->Items[n]);
			if (pdef->LisBlocosAutoDef->Count == 0)
				continue;
			pdef->LisBlocosAutoDef->Remove((VTBloco*)pdef->LisBlocosJusDef->Items[n]);
		}
		pdef->LisBlocosProtAuto->Assign(pdef->LisBlocosProtDef, laOr);
		if (pdef->LisBlocosAutoDef->Count > 0)
		{
			for (int n = 0; n < pdef->LisBlocosAutoDef->Count; n++)
				pdef->LisBlocosProtAuto->Remove(pdef->LisBlocosAutoDef->Items[n]);
		}
	}

	// GERA LISTAS DE BLOCOS DO DEFEITO
	// Gera lista de blocos manobraveis e com alternativas de socorro a jusante possíveis(desligados)
	// e lista de blocos dos circuitos de vis manobraveis
	// Para cada chave vis definir chaves a jusante para o isolamento visRede->chvJus
	// e lista de blocosRad que apontam para chave de isolamento a jusante até visRede->chvJus
	// parou definir estrategias intermediarias com chaves automaticas
	LisBlManLis(pdef);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ChaveAutomatica(VTChave *pchv)
{
	return (lisChvAuto->IndexOf(pchv) != -1);
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ChaveProtecao(VTChave *pchv)
{
	// Verifica se a chave esta no conjunto de ativas
	if (lisChavesAtivas != NULL)
	{
		if (lisChavesAtivas->IndexOf(pchv) == -1)
			return (false);
	}

	return (lisTipoEqProt->IndexOf(pchv->TipoChave) != -1); // se pertence retorna true
}

// ---------------------------------------------------------------------------
bool __fastcall TBuscaProt::ChaveReligadora(VTChave *pchv)
{ // Problema - pode haver outras chaves com função de religamento
	// comparacao as vezes não funciona
	if (lisChavesAtivas != NULL)
	{
		if (lisChavesAtivas->IndexOf(pchv) == -1)
			return (false);
	}
	VTTipoChave *tipoChave;
	tipoChave = pchv->TipoChave;
	return ((tipoChave->Tipo == chaveRELIGADORA) || (tipoChave->Tipo == chaveDISJUNTOR));
	// hk201612
	// return((pchv->TipoChave->Tipo == chaveRELIGADORA)||(pchv->TipoChave->Tipo == chaveDISJUNTOR));
}

// ---------------------------------------------------------------------------
double __fastcall TBuscaProt::PM_GetCargaBlocoJus(VTChave* chave, int np)
{
	VTBlocoRad* blocoRad;
	if (!flagInicia)
		return (0.0);
	if ((blocoRad = IdentificaBlocoRad(chave)) == NULL)
		return (0.0);

	return (blocoRad->CargaBlRad[np]);
}

// ---------------------------------------------------------------------------
void __fastcall TBuscaProt::PM_SetIProfReliga(int iProfReliga)
{
	if ((this->iProfReliga != iProfReliga) && (flagInicia))
		ReiniciaZonaReliga();
	this->iProfReliga = iProfReliga;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// void __fastcall TBuscaProt::IniciaTipoProt(void)
// {
// VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
// VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
// VTTipoChave *tipoChave;
// VTChave     *chave;
// TList       *lisChave;
//
// if(tipos == NULL)
// {
// lisTipoEqProt->Clear();
// lisChave = new TList();
// redes->LisEqpto(lisChave, eqptoCHAVE);
// for(int n = 0; n < lisChave->Count; n++)
// {
// chave = (VTChave*)lisChave->Items[n];
// tipoChave = chave->TipoChave;
// if(lisTipoEqProt->IndexOf(tipoChave) == -1) lisTipoEqProt->Add(tipoChave);
// }
// delete lisChave;
// }
// else
// {
// lisTipoEqProt->Clear();
// lisTipoEqProt->Assign(tipos->LisTipoChave(), laOr);
// }

/*
 for(int n = lisTipoEqProt->Count - 1; n--; n >= 0)
 {
 tipoChave = (VTTipoChave*)lisTipoEqProt->Items[n];
 if(tipoChave->Tipo ==chaveDJ) continue;
 if(tipoChave->Tipo ==chaveRA) continue;
 if(tipoChave->Tipo ==chaveBF) continue;
 lisTipoEqProt->Remove(tipoChave);
 }
 chaveDJ=1, chaveDISJUNTOR   =chaveDJ,
 chaveRA,   chaveRELIGADORA  =chaveRA,
 chaveBF,   chaveBASE_FUSIVEL=chaveBF,
 chaveSEC,  chaveSECCIONADORA=chaveSEC,
 chaveINDEFINIDA,
 chaveDEFINIDA_USUARIO,
 //chaves Eletropaulo/Subterrâneo
 chaveDESCONECTAVEL=1000, chaveNETWORK_PROTECTOR
 */

// for(int n = lisTipoEqProt->Count - 1; n--; n >= 0)
// {
// tipoChave = (VTTipoChave*)lisTipoEqProt->Items[n];
//
// if(tipoChave->Codigo.AnsiCompare("Disjuntor") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Religador") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Base Fusível") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Chave Fusivel Repetidora MT") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Seccionalizador") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Chave Fusivel") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Chave Interruptora Tripolar Seca") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Chave Fusivel Especial 34,5kV") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Chave de Protecao Subterranea") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Posto Transformador 34,5kV com eqpto de proteçao/manobra") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Posto Transformador 34,5kV sem eqpto de proteçao/manobra") == 0) continue;
// lisTipoEqProt->Remove(tipoChave);
// }

// tipoChave->Codigo.AnsiCompare("Seccionadora") == 0) continue;
// if(tipoChave->Codigo.AnsiCompare("Regulador de Tensao") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Faca") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Indefinida") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Autotransformador") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Chave Faca Basculante") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Banco de Capacitores") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Chave Seccionadora MT SF6") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Chave Faca Unipolar") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Chave Faca Adaptada") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Chave Faca Interligaçao 13,8kV/34,5kV") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Seccionamento Operativo") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Chave de Manobra Subterranea") == 0) continue;
// tipoChave->Codigo.AnsiCompare("Seccionamento de Primario") == 0) continue;
// }
// ---------------------------------------------------------------------------
// hkv201610: correção da eliminação de suprimentos secundários
/* bool __fastcall TBuscaProt::OrdenaChaves(bool flagExeBlocos)
 {
 VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTBloco *blocoSup, *blocoJus, *blocoAux;
 VTBlocoRad *blSup, *blSupAux, *blAux, *blJus, *blMon;
 VTVisRede  *visRede;
 TList   *lisAux, *lisAux1, *lisAux2, *lisBlM, *lisChvAux, *lisRedu, *lisBarra;
 VTChave *pchave;
 VTLisBlocoChave *plisBlocoChave;
 VTReducao*  redu;
 VTElo   *elo;
 VTBarra *pbarra;
 bool resposta = true;
 ///////////////////////////////////////////////////////////////////////////////
 //Função que gera:                                                           //
 //	 lisBlocoRad->blocoRad de suprimento --> arvore de blocosRad até chaves NA//
 //  lisBlocoChv->TLisBlocoChave : ordenação de chaves                        //
 //  lisVisRede->visRede-> (bloco1/2, rede1/2, pbarra1/2)
 ///////////////////////////////////////////////////////////////////////////////

 //Inicia os blocos com a última versão de rede
 if(flagExeBlocos)
 {if(!blocos->Executa(redes)) resposta = false;}

 lisAux = new TList();
 lisBlM = new TList();
 lisChvAux = new TList();

 //PEND suprimentos em redes reduzidas

 //Reinicia lisBlocoRad e lisBlocoChv
 LimpaTList(lisBlocoRad);
 LimpaTList(lisBlocoChv);
 ///////////////////////////////////////////////////////////////////////////////
 //Popula lisBlocoRad -> lista de blocos que aponta para blocos de suprimento
 //     e lisBlocoChv
 ///////////////////////////////////////////////////////////////////////////////
 //Busca bloco de suprimento percorre em lista de barras do bloco se existe geracao
 lisAux->Clear();
 blocos->LisBlocoGeracao(lisAux);


 //Adiciona blocos ligados a redes equivalentes de suprimento
 //REDUÇÕES COM SUPRIMENTO
 lisRedu = new TList();
 redes->LisReducao(lisRedu);
 for(int nr = 0; nr < lisRedu->Count; nr++)
 {
 redu = (VTReducao*)lisRedu->Items[nr];
 if(!redu->ExisteGeracao()) continue;
 //Barras da
 lisBarra = redu->LisBarra();
 for(int nbar = 0; nbar < lisBarra->Count; nbar++)
 {
 pbarra   = (VTBarra*)lisBarra->Items[nbar];
 blocoAux = blocos->ExisteBloco(pbarra);
 if(lisAux->IndexOf(blocoAux) == -1) lisAux->Add(blocoAux);
 }
 }
 delete lisRedu;

 //Método para ordenacao dos blocos de geracao em blocos ligados a
 //          redes equivalentes com suprimento, blocos com gerador e blocos com suprimento vteta
 OrdenaLisBlocosGeracao(lisAux);

 //Verifica se existem blocos de suprimento na rede
 if(lisAux->Count == 0) {
 Aviso("Rede sem Suprimento");
 return(false);
 }

 //Popula lisBlocoRad -> lista de blocos que aponta para blocos de suprimento
 for(int n = 0; n < lisAux->Count; n++)//for(int n = lisAux->Count - 1; n >= 0; n--)
 {
 blocoSup = (VTBloco*)lisAux->Items[n];
 lisBlocoRad->Add(blSup = NewObjBlocoRad(blocoSup));//new TBlocoRad(blocoSup));
 blSup->FlagSup = true;
 }

 //Gera lista em arvore a partir de objetos em lisBlocoRad
 //e popula lisBlocoChv que aponta para chaves da rede
 lisAux->Clear();
 for(int nsup = lisBlocoRad->Count - 1; nsup >= 0; nsup--)//for(int nsup = 0; nsup < lisBlocoRad->Count; nsup++)
 {
 blSup = (VTBlocoRad*)lisBlocoRad->Items[nsup];
 lisAux->Add(blSup);
 lisBlM->Add(blSup->Bloco);
 while(lisAux->Count > 0)
 {
 blAux = (VTBlocoRad*)lisAux->First();
 lisAux->Delete(0);

 //Verifica se há múltiplos suprimentos, se sim elimina segundo ponto de geração (rede equiv, gerador, suprimento)
 blSupAux = NULL;
 for(int nn = 0; nn < lisBlocoRad->Count; nn++)
 {
 blSupAux = (VTBlocoRad*)lisBlocoRad->Items[nn];
 if((blSupAux->Bloco == blAux->Bloco)&&(blSupAux != blSup))
 {
 lisBlocoRad->Delete(nn);
 break;
 }
 blSupAux = NULL;
 }
 //if(blSupAux != NULL) break; //hkv201610

 lisChvAux->Clear();
 //Percorre chaves do bloco
 blocos->LisChaveBloco(blAux->Bloco, lisChvAux);
 for(int nc = 0; nc < lisChvAux->Count; nc++)
 {
 pchave = (VTChave*)lisChvAux->Items[nc];
 //Se chave NA
 if(pchave->Aberta)
 {
 //Busca plisBlocoChave em lisBlocoChv
 plisBlocoChave = NULL;
 for(int nc = 0; nc < lisBlocoChv->Count; nc++)
 {
 plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
 if(plisBlocoChave->pchv == pchave) break;
 plisBlocoChave = NULL;
 }
 if(plisBlocoChave == NULL)
 {lisBlocoChv->Add(plisBlocoChave = NewObjLisBlocoChave(pchave));
 plisBlocoChave->blMon = blAux;
 plisBlocoChave->blSup = blSup;
 }
 else
 {plisBlocoChave->blMon1 = blAux;
 plisBlocoChave->blSup1 = blSup;
 }
 continue;
 }
 //Se Chave NA

 //Se Chave NF
 elo = blocos->ExisteElo(pchave);
 blocoJus = elo->Bloco1;
 if(elo->Bloco1 == blAux->Bloco) blocoJus = elo->Bloco2;
 if(lisBlM->IndexOf(blocoJus) != -1) continue;
 lisBlM->Add(blocoJus);
 lisAux->Add(blJus = NewObjBlocoRad(blocoJus));//new TBlocoRad(blocoAux));
 blAux->LisBlocoRad->Add(blJus);
 blJus->Chave = pchave; //chave de suprimento
 blJus->BlocoRadMon = blAux;

 //Adiciona blJus a lista de blocos a jusante
 blMon = blJus;
 while(blMon->BlocoRadMon != NULL)
 {
 blMon = blMon->BlocoRadMon;
 blMon->LisBlocoRadJus->Add(blJus);
 blMon->LisBlocoJus->Add(blJus->Bloco);
 }
 //Cria objeto lisBlocoChave
 lisBlocoChv->Add(plisBlocoChave = NewObjLisBlocoChave(pchave));
 plisBlocoChave->blSup = blSup;
 plisBlocoChave->blMon = blAux;
 plisBlocoChave->blJus = blJus;
 }
 }
 }

 VTBlocoRad *blocoRad;
 lisAux1 = new TList();
 //Elimina blocos a montante de disjuntor
 if(lisDjPri->Count > 0)
 {
 //Busca blocoRad->Chave pertence a lisDjPri
 lisAux->Clear();

 lisAux->Assign(lisDjPri, laOr);
 while(lisAux->Count > 0)
 {
 pchave = (VTChave*)lisAux->First();
 lisAux->Delete(0);
 if((blAux = IdentificaBlocoRad(pchave)) == NULL) continue;
 blAux->FlagSup = true;
 //Corta blocos a montande do disjuntor
 //lisBlocoRad->Add(blAux);
 //if(blAux->BlocoRadMon != NULL)
 //	{
 //	lisAux1->Clear();
 //	lisAux1->Assign(blAux->BlocoRadMon->LisBlocoRad, laOr);
 //	blAux->BlocoRadMon->LisBlocoRad->Clear();
 //	for(int nb = 0; nb < lisAux1->Count; nb++)
 //		{
 //		blocoRad = (VTBlocoRad*)lisAux1->Items[nb];
 //		blocoRad->BlocoRadMon = NULL;
 //		}
 //	delete blAux->BlocoRadMon;
 //	}
 //blAux->BlocoRadMon = NULL;
 ////blAux->Chave       = NULL;
 //blAux->FlagSup     = true;
 }

 }

 //////////////////////////////////////////////////////////////
 //Preenche blocos a jusante de chave, plisBlocoChave->lisBlJ//
 //////////////////////////////////////////////////////////////
 for(int nc = 0; nc < lisBlocoChv->Count; nc++)
 {
 plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
 MontaLisBlocos(plisBlocoChave);
 }


 ////////////////////////////////////////////////////
 //Preenche lista de blocos a montante por blocoRad//
 ////////////////////////////////////////////////////

 //Define lista de blocos iniciais de circuitos
 //primários baseado em lista de disjuntores
 TList *lisBlocoRadSupAux;
 int iconta;
 lisBlocoRadSupAux = new TList();
 lisBlocoRadSupAux->Assign(lisBlocoRad, laOr);
 for(int nsup = 0; nsup < lisBlocoRadSupAux->Count; nsup++)
 {
 blSup = (VTBlocoRad*)lisBlocoRadSupAux->Items[nsup];
 blSup->LisBlocoMon->Clear();
 lisAux1->Clear();
 lisAux->Clear();
 //Preenche lisAux1: lista com blRad extremos
 lisAux->Add(blSup);
 while(lisAux->Count > 0)
 {
 blAux = (VTBlocoRad*)lisAux->First();
 lisAux->Delete(0);
 if(blAux->LisBlocoRad->Count == 0)
 {
 if(lisAux1->IndexOf(blAux) == -1) lisAux1->Add(blAux);
 }
 else {lisAux->Assign(blAux->LisBlocoRad, laOr);
 }
 }
 lisAux->Clear();
 for(int nb = 0; nb < lisAux1->Count; nb++)
 {
 blAux = (VTBlocoRad*)lisAux1->Items[nb];
 blAux->LisBlocoMon->Add(blSup->Bloco);
 blAux->LisBlocoMon->Assign(blSup->LisBlocoJus, laOr);
 blAux->LisBlocoMon->Remove(blAux->Bloco);
 //BlRad já percorrido
 lisAux->Add(blAux);
 blJus = blAux;
 blAux = blJus->BlocoRadMon;
 while(blAux != NULL)
 {
 if(lisAux->IndexOf(blAux) == -1)
 {
 blAux->LisBlocoMon->Assign(blJus->LisBlocoMon, laOr);
 blAux->LisBlocoMon->Remove(blAux->Bloco);
 lisAux->Add(blAux);
 }
 blJus = blAux;
 blAux = blJus->BlocoRadMon;
 }
 }
 }
 delete lisBlocoRadSupAux;
 ////////////////////////////
 //Monta lisChaves Vis     //
 ////////////////////////////
 //Reinicia lisVisRede
 LimpaTList(lisVisRede);
 //Reinicia lista de chaves vis
 lisChavesVis->Clear();
 for(int nc = 0; nc < lisBlocoChv->Count; nc++)
 {
 plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
 //Filtra chaves NF
 if(plisBlocoChave->pchv->Fechada) continue;  //if(plisBlocoChave->pchv->Estado == chvFECHADA) continue; //hkRevAloca
 //Filtra chaves NA sem ligacao
 if((plisBlocoChave->blMon == NULL)||(plisBlocoChave->blMon1== NULL)) continue;
 //Descarta blocoChave sem rede em uma das ligacoes hkSH_ComSE
 if((plisBlocoChave->blMon->Bloco->Rede == NULL)||(plisBlocoChave->blMon1->Bloco->Rede == NULL)) continue;
 //Filtra chaves com o mesmo suprimento (chaves NA internas, bypass)
 if(plisBlocoChave->blMon->Bloco->Rede == plisBlocoChave->blMon1->Bloco->Rede) continue;
 //		if(plisBlocoChave->blSup == plisBlocoChave->blSup1) continue;

 //Cria lisVisRede
 visRede = NewObjVisRede(plisBlocoChave->pchv);
 lisVisRede->Add(visRede);
 visRede->plisBlocoChave = plisBlocoChave;

 visRede->blocoRad1 = plisBlocoChave->blMon;
 visRede->bloco1= plisBlocoChave->blMon->Bloco;
 visRede->rede1 = plisBlocoChave->blMon->Bloco->Rede;

 visRede->blocoRad2 = plisBlocoChave->blMon1;
 visRede->bloco2= plisBlocoChave->blMon1->Bloco;
 visRede->rede2 = plisBlocoChave->blMon1->Bloco->Rede;

 if(plisBlocoChave->blMon->Bloco->LisBarra()->IndexOf(pchave->pbarra1) != -1)
 {visRede->pbarra1 = plisBlocoChave->pchv->pbarra1;
 visRede->pbarra2 = plisBlocoChave->pchv->pbarra2;
 }
 else
 {visRede->pbarra1 = plisBlocoChave->pchv->pbarra2;
 visRede->pbarra2 = plisBlocoChave->pchv->pbarra1;
 }


 ////////////////////////////////////////////////////////////////////////////////////////
 //Funcao que gera para lisVisRede->TVisRede:                                          //
 //lista de ligacoes       até o suprimento: TVisRede->lisLiga(rede1),  lisLiga(rede2) //
 //lista de blocos         até o suprimento: TVisRede->lisBloco(rede1), lisBloco(rede2)//
 ////////////////////////////////////////////////////////////////////////////////////////
 LisLigaVis(visRede);

 //LisLigaVis(visRede->pchave, visRede->pbarra1, visRede->lisLiga1, visRede->lisBlocosVis1);
 //LisLigaVis(visRede->pchave, visRede->pbarra2, visRede->lisLiga2, visRede->lisBlocosVis2);


 //Adiciona chave NA entre circuitos a lisChavesVis
 lisChavesVis->Add(visRede->pchave);
 }

 ////////////////////////////////////////////////////////////////////////////////
 //Para chaves de protecao NF gera lista de blocos até o proximo equipamento   //
 ////////////////////////////////////////////////////////////////////////////////
 //for(int nc = 0; nc < lisBlocoChv->Count; nc++)
 //	{
 //	plisBlocoChave = (VTLisBlocoChave*)lisBlocoChv->Items[nc];
 //Exclui chaves NA
 //	if(plisBlocoChave->pchv->Aberta) continue;
 //Verifica se é chave de protecao
 //if(lisTipoEqProt->IndexOf(plisBlocoChave->pchv->TipoChave) == -1) continue;
 //Protecao caso seja uma chave NF ligada a nada
 //	if(plisBlocoChave->blJus == NULL) continue;
 //Preenche lista de blocos a jusante até as chaves de proteção, funcao de lisTipoEqProt (já tem que ter definido em leitura!)
 //	ListaZonaProtJus(plisBlocoChave->blJus, plisBlocoChave->blJus->LisBlocoJusProt);
 //	}



 delete lisAux;
 delete lisAux1;
 delete lisBlM;
 delete lisChvAux;

 return(true);
 }
 */
// ---------------------------------------------------------------------------
