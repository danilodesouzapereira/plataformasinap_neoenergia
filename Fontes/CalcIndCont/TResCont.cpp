// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TResCont.h"
#include "VTBuscaProt.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento : Acréscimo de variável resBloco->rede
// ClonaResCont(void)
// IniciaListas(VTRede *rede)
// IniciaResultados(VTRede *rede)
// ImprimeResBloco(AnsiString arq)
// ---------------------------------------------------------------------------
VTResCont* __fastcall NewObjResCont(VTApl *apl)
{
	try
	{ // cria objeto
		return (new TResCont(apl));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TResCont::TResCont(VTApl *apl)
{
	this->apl = apl;
	lisResBloco = new TList();
	lisResRede = new TList();
	lisResCarga = new TList();
	resGeral = new strResRede;

	lisTemp = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TResCont::~TResCont(void)
{
	// destroi listas e objetos
	if (resGeral)
	{
		delete resGeral;
		resGeral = NULL;
	}
	if (lisResBloco)
	{
		LimpaTList(lisResBloco);
		delete lisResBloco;
		lisResBloco = NULL;
	}
	if (lisResRede)
	{
		LimpaTList(lisResRede);
		delete lisResRede;
		lisResRede = NULL;
	}
	if (lisResCarga)
	{
		LimpaTList(lisResCarga);
		delete lisResCarga;
		lisResCarga = NULL;
	}
	if (lisTemp)
	{
		delete lisTemp;
		lisTemp = NULL;
	}
}

// ---------------------------------------------------------------------------
VTResCont* __fastcall TResCont::ClonaResCont(void)
{
	VTResCont *resContClone;
	strResBloco *resBloco, *resBlocoClone;
	strResRede *resRede, *resRedeClone;

	resContClone = NewObjResCont(apl);

	for (int n = 0; n < lisResBloco->Count; n++)
	{
		resBloco = (strResBloco*)lisResBloco->Items[n];
		resContClone->LisResBloco->Add(resBlocoClone = new strResBloco);
		resBlocoClone->bloco = resBloco->bloco;
		resBlocoClone->rede = resBloco->rede;
		// hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento : Acréscimo de variável resBloco->rede
		resBlocoClone->blocoRad = resBloco->blocoRad;
		resBlocoClone->tdesl = resBloco->tdesl;
		resBlocoClone->ndesl = resBloco->ndesl;
		resBlocoClone->end = resBloco->end;
		resBlocoClone->npiscadas = resBloco->npiscadas;
		resBlocoClone->tdeslEND = resBloco->tdeslEND;
		resBlocoClone->ndeslEND = resBloco->ndeslEND;
		// tempos e desligamentos totais sem limitantes
		resBlocoClone->ncons = resBloco->ncons;
		resBlocoClone->nconsVIP = resBloco->nconsVIP;
		// Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
		resBlocoClone->dcons = resBloco->dcons; // desliga x cons
		resBlocoClone->tcons = resBloco->tcons; // tdesligax cons
		resBlocoClone->dconsPisca = resBloco->dconsPisca; // piscadas x nons
		resBlocoClone->dconsPiscaVIP = resBloco->dconsPiscaVIP; // piscadas x consVIP
		// Tratamento do defeito
		resBlocoClone->nmanAuto = resBloco->nmanAuto; // #manobras automáticas
		resBlocoClone->nmanMan = resBloco->nmanMan; // #manobras manuais
	}

	for (int n = 0; n < lisResRede->Count; n++)
	{
		resRede = (strResRede*)lisResRede->Items[n];
		resContClone->LisResRede->Add(resRedeClone = new strResRede);
		resRedeClone->rede = resRede->rede;
		resRedeClone->tdeslN = resRede->tdeslN;
		resRedeClone->ndeslN = resRede->ndeslN; // tempo.consumidores, deslig.consumdidores
		resRedeClone->end = resRede->end;
		resRedeClone->tdeslENDN = resRede->tdeslENDN;
		resRedeClone->ndeslENDN = resRede->ndeslENDN;
		// tempos.consum e deslig.consum totais sem limitantes
		resRedeClone->ncons = resRede->ncons;
		resRedeClone->tdeslBTN = resRede->tdeslBTN;
		resRedeClone->ndeslBTN = resRede->ndeslBTN;
		resRedeClone->endBT = resRede->endBT;
		resRedeClone->ndeslPisca = resRede->ndeslPisca;
		resRedeClone->ndeslPiscaVIP = resRede->ndeslPiscaVIP;
		resRedeClone->nconsVIP = resRede->nconsVIP; // Número de consumidores VIP
		// Indicadores individuais
		resRedeClone->compensacao = resRede->compensacao;
		// compensação regulatória por transgressao de indic indiv
		resRedeClone->ncargaVIP = resRede->ncargaVIP;
		// número de consumidores e redes BT com consumidores VIP
		resRedeClone->dicVIP = resRede->dicVIP;
		// índice de mério em função de atendimento a consumidores VIP
		resRedeClone->ficVIP = resRede->ficVIP;
		// índice de mério em função de atendimento a consumidores VIP
	}

	resContClone->ResGeral->rede = resGeral->rede;
	resContClone->ResGeral->tdeslN = resGeral->tdeslN;
	resContClone->ResGeral->ndeslN = resGeral->ndeslN; // tempo.consumidores, deslig.consumdidores
	resContClone->ResGeral->end = resGeral->end;
	resContClone->ResGeral->tdeslENDN = resGeral->tdeslENDN;
	resContClone->ResGeral->ndeslENDN = resGeral->ndeslENDN;
	// tempos.consum e deslig.consum totais sem limitantes
	resContClone->ResGeral->ncons = resGeral->ncons;
	resContClone->ResGeral->tdeslBTN = resGeral->tdeslBTN;
	resContClone->ResGeral->ndeslBTN = resGeral->ndeslBTN;
	resContClone->ResGeral->endBT = resGeral->endBT;
	resContClone->ResGeral->ndeslPisca = resGeral->ndeslPisca;
	resContClone->ResGeral->ndeslPiscaVIP = resGeral->ndeslPiscaVIP;
	resContClone->ResGeral->nconsVIP = resGeral->nconsVIP; // Número de consumidores VIP
	// Indicadores individuais
	resContClone->ResGeral->compensacao = resGeral->compensacao;
	// compensação regulatória por transgressao de indic indiv
	resContClone->ResGeral->ncargaVIP = resGeral->ncargaVIP;
	// número de consumidores e redes BT com consumidores VIP
	resContClone->ResGeral->dicVIP = resGeral->dicVIP;
	// índice de mério em função de atendimento a consumidores VIP
	resContClone->ResGeral->ficVIP = resGeral->ficVIP;
	// índice de mério em função de atendimento a consumidores VIP
	return (resContClone);
}

// ---------------------------------------------------------------------------
TList* __fastcall TResCont::PM_GetLisResBlocoRede(VTRede *rede)
{
	strResRede *resRede;
	strResBloco *resBloco;
	int index;

	lisTemp->Clear();
	if ((resRede = PM_GetResRede(rede)) == NULL)
		return (lisTemp);
	index = resRede->indexResBloco;

	for (int n = index; n < lisResBloco->Count; n++)
	{
		if ((resBloco = (strResBloco*)lisResBloco->Items[n])->bloco->Rede != rede)
			break;
		lisTemp->Add(resBloco);
	}
	return (lisTemp);
}

// ---------------------------------------------------------------------------
TList* __fastcall TResCont::PM_GetLisResCargaRede(VTRede *rede)
{
	strResRede *resRede;
	strResCarga *resCarga;
	int index;

	lisTemp->Clear();
	if ((resRede = PM_GetResRede(rede)) == NULL)
		return (lisTemp);
	if ((index = resRede->indexResCarga) < 0)
		return (lisTemp);

	for (int n = index; n < lisResCarga->Count; n++)
	{
		if ((resCarga = (strResCarga*)lisResCarga->Items[n])->rede != rede)
			break;
		lisTemp->Add(resCarga);
	}
	return (lisTemp);
}

// ---------------------------------------------------------------------------
TList* __fastcall TResCont::PM_GetLisResCargaBloco(VTBloco *bloco)
{
	strResBloco *resBloco;
	strResCarga *resCarga;
	int index;

	lisTemp->Clear();
	if ((resBloco = PM_GetResBloco(bloco, false)) == NULL)
		return (lisTemp);
	index = resBloco->indexResCarga;

	for (int n = index; n < lisResCarga->Count; n++)
	{
		if ((resCarga = (strResCarga*)lisResCarga->Items[n])->bloco != bloco)
			break;
		lisTemp->Add(resCarga);
	}
	return (lisTemp);
}

// ---------------------------------------------------------------------------
strResBloco* __fastcall TResCont::PM_GetResBloco(VTBloco *bloco, bool flagCria)
{
	strResBloco* resBloco = NULL;
	TList *lisAux;
	strResRede *resRede;
	resRede = NULL;
	for (int nr = 0; nr < lisResRede->Count; nr++)
	{
		resRede = (strResRede*)lisResRede->Items[nr];
		if (resRede->rede == bloco->Rede)
			break;
		resRede = NULL;
	}
	if (resRede == NULL)
		return (NULL);

	int index;
	if ((index = resRede->indexResBloco) < 0)
		return (NULL);
	// Busca estratégia de socorro para o patamar
	// for(int n = 0; n < lisResBloco->Count; n++)
	for (int n = index; n < lisResBloco->Count; n++)
	{
		resBloco = (strResBloco*)lisResBloco->Items[n];
		if (resBloco->bloco == bloco)
			break;
		resBloco = NULL;
	}
	if ((resBloco == NULL) && (flagCria))
	{
		lisResBloco->Add(resBloco = new strResBloco);
		bloco->Obj = (TObject*)resBloco;//hk201902temp
		// resBloco->cBloco   = NULL;
		resBloco->bloco = bloco;
		resBloco->rede = bloco->Rede;
		resBloco->blocoRad = NULL;
		resBloco->tdesl = 0.0;
		resBloco->tdeslEND = 0.0;
		resBloco->ndesl = 0.0;
		resBloco->ndeslEND = 0.0;
		resBloco->end = 0.0;
		resBloco->npiscadas = 0.0; // número total de piscadas no bloco
		lisAux = new TList();
		((VTBloco*)bloco)->LisEqbar(lisAux, eqptoCARGA);

		resBloco->ncons = NCons(lisAux); // lisAux->Count;
		resBloco->nconsVIP = NConsVIP(lisAux); // lisAux->Count;
		delete lisAux;
		resBloco->dcons = 0.0; // desliga x cons
		resBloco->tcons = 0.0; // tdesligax cons
		resBloco->dconsPisca = 0.0; // piscadas x nons
		resBloco->dconsPiscaVIP = 0.0; // piscadas x consVIP
		resBloco->nmanAuto = 0; // #manobras automáticas
		resBloco->nmanMan = 0; // #manobras manuais
	}

	// Tratamento do defeito
	return (resBloco);
}

// ---------------------------------------------------------------------------
strResCarga* __fastcall TResCont::PM_GetResCarga(VTCarga *carga, TList *lisRes)
{
	TList *lisResAux;
	strResCarga *resCarga;
	// Busca resCarga em lisResAux
	if (lisRes != NULL)
		lisResAux = lisRes;
	else
		lisResAux = lisResCarga;

	for (int n = 0; n < lisResAux->Count; n++)
	{
		resCarga = (strResCarga*)lisResAux->Items[n];
		if (resCarga->carga == carga)
			return (resCarga);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
strResRede* __fastcall TResCont::PM_GetResGeral(void)
{
	return (resGeral);
}

// ---------------------------------------------------------------------------
strResRede* __fastcall TResCont::PM_GetResRede(VTRede *rede)
{
	strResRede* resRede = NULL;
	// TList        *lisAux;
	// Busca estratégia de socorro para o patamar
	for (int n = 0; n < lisResRede->Count; n++)
	{
		resRede = (strResRede*)lisResRede->Items[n];
		if (resRede->rede == rede)
			break;
		resRede = NULL;
	}
	/*
	 if(resRede == NULL) {
	 lisResRede->Add(resRede = new strResRede);
	 //resBloco->cBloco   = NULL;
	 resRede->rede     = rede;
	 resRede->tdeslN   = 0.0;
	 resRede->tdeslENDN= 0.0;
	 resRede->ndeslN   = 0.0;
	 resRede->ndeslENDN= 0.0;
	 resRede->end      = 0.0;
	 resRede->ncons    = 0;
	 //								 lisAux = new TList();
	 //rede->LisEqbar(lisAux, eqptoCARGA);
	 //								 resRede->ncons = lisAux->Count;
	 //								 delete lisAux;
	 }
	 */
	return (resRede);
}

// ---------------------------------------------------------------------------
void __fastcall TResCont::IniciaListas(VTRede *rede)
{
	VTRedes *redes;
	VTRede *redeRes;
	TList *lisRedes;
	strResCarga* resCarga = NULL;
	strResRede* resRede = NULL;
	strResBloco* resBloco = NULL;

	if (rede == NULL)
	{
		// Limpa listas
		LimpaTList(lisResBloco);
		LimpaTList(lisResRede);
		LimpaTList(lisResCarga);

		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		lisRedes = redes->LisRede();
		for (int nr = 0; nr < lisRedes->Count; nr++)
		{
			redeRes = (VTRede*)lisRedes->Items[nr];
			if (!redeRes->Carregada)
				continue;
			IniciaListaRede(redeRes);
		}
		// Reinicia resGeral
		resGeral->ncons = 0;
		resGeral->rede = rede;
		resGeral->tdeslN = 0.0;
		resGeral->tdeslENDN = 0.0;
		resGeral->ndeslN = 0.0;
		resGeral->ndeslENDN = 0.0;
		resGeral->end = 0.0;
		resGeral->ncons = 0;
		resGeral->tdeslBTN = 0.0;
		resGeral->ndeslBTN = 0.0;
		resGeral->endBT = 0.0;
		resGeral->ndeslPisca = 0.0; ;
		resGeral->ndeslPiscaVIP = 0.0; ;
		resGeral->nconsVIP = 0.0; ; // Número de consumidores VIP
		resGeral->compensacao = 0.0;
		resGeral->dicVIP = 0.0;
		resGeral->ficVIP = 0.0;
		resGeral->ncargaVIP = 0;
		// Atualiza número de consumidores
		for (int nr = 0; nr < lisResRede->Count; nr++)
		{
			resRede = (strResRede*)lisResRede->Items[nr];
			resGeral->ncons += resRede->ncons;
			resGeral->nconsVIP += resRede->nconsVIP;
		}

	}
	else
	{
		// Retira resRede->Rede == rede existente
		if ((resRede = this->ResRede[rede]) != NULL)
		{
			lisResRede->Remove(resRede);
			delete resRede;
		}
		// retira resBloco->bloco->rede = rede existente
		for (int nb = lisResBloco->Count - 1; nb >= 0; nb--)
		{
			resBloco = (strResBloco*)lisResBloco->Items[nb];
			// hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento
			// if(resBloco->bloco->Rede != rede) continue;
			if (resBloco->rede != rede)
				continue;
			lisResBloco->Delete(nb);
			delete resBloco;
		}
		// retira resCarga->rede = rede existente
		for (int nc = lisResCarga->Count - 1; nc >= 0; nc--)
		{
			resCarga = (strResCarga*)lisResCarga->Items[nc];
			if (resCarga->rede != rede)
				continue;
			lisResCarga->Delete(nc);
			delete resCarga;
		}
		IniciaListaRede(rede);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TResCont::IniciaListaRede(VTRede *rede)
{
	VTBuscaProt *buscaProt = (VTBuscaProt*)apl->GetObject(__classid(VTBuscaProt));
	// VTBlocos	   *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBloco *bloco;
	VTCarga *carga;
	TList *lisBlocos, *lisAux;
	strResRede* resRede = NULL;
	strResBloco* resBloco = NULL;
	strResCarga* resCarga = NULL;

	// gera objeto resultado para rede
	lisResRede->Add(resRede = new strResRede);
	resRede->rede = rede;
	resRede->tdeslN = 0.0;
	resRede->tdeslENDN = 0.0;
	resRede->ndeslN = 0.0;
	resRede->ndeslENDN = 0.0;
	resRede->end = 0.0;
	resRede->ncons = 0;
	resRede->tdeslBTN = 0.0;
	resRede->ndeslBTN = 0.0;
	resRede->endBT = 0.0;
	resRede->ndeslPisca = 0.0;
	resRede->ndeslPiscaVIP = 0.0;
	resRede->nconsVIP = 0.0; // Número de consumidores VIP
	resRede->compensacao = 0.0;
	resRede->dicVIP = 0.0;
	resRede->ficVIP = 0.0;
	resRede->ncargaVIP = 0;
	resRede->indexResBloco = -1;
	resRede->indexResCarga = -1;

	// Preenche lista com blocos ordenados da montante a jusante
	lisBlocos = new TList();
	buscaProt->ListaOrdenadaBlocos(lisBlocos, rede); // hkedp
	// Gerado objeto resultado por bloco
	// lisBlocos = blocos->LisBloco();
	lisAux = new TList();
	for (int nb = 0; nb < lisBlocos->Count; nb++)
	{
		bloco = (VTBloco*)lisBlocos->Items[nb];
		if (rede != NULL)
		{
			if (bloco->Rede != rede)
				continue;
		}
		lisResBloco->Add(resBloco = new strResBloco);
		bloco->Obj = (TObject*)resBloco; //(bloco->LisRede())->Add(resBloco);//hk201902temp
		// Seta índice do primeiro bloco da rede
		if (nb == 0)
			resRede->indexResBloco = lisResBloco->Count - 1;

		resBloco->bloco = bloco;
		resBloco->rede = bloco->Rede;
		resBloco->blocoRad = NULL;
		resBloco->tdesl = 0.0;
		resBloco->tdeslEND = 0.0;
		resBloco->ndesl = 0.0;
		resBloco->ndeslEND = 0.0;
		resBloco->end = 0.0;
		resBloco->npiscadas = 0.0; // número total de piscadas no bloco
		resBloco->dcons = 0.0; // desliga x cons
		resBloco->tcons = 0.0; // tdesligax cons
		resBloco->dconsPisca = 0.0; // piscadas x nons
		resBloco->dconsPiscaVIP = 0.0; // piscadas x consVIP
		resBloco->nmanAuto = 0; // #manobras automáticas
		resBloco->nmanMan = 0; // #manobras manuais

		lisAux->Clear();
		bloco->LisEqbar(lisAux, eqptoCARGA);
		resBloco->ncons = NCons(lisAux); // lisAux->Count;
		resBloco->nconsVIP = NConsVIP(lisAux); // número de consumidores vip do bloco
		resRede->ncons += resBloco->ncons;
		resRede->nconsVIP += resBloco->nconsVIP;
		// Guarda indice resCarga
		resBloco->indexResCarga = lisResCarga->Count;
		// Reinicia resCargas
		for (int nc = 0; nc < lisAux->Count; nc++)
		{
			carga = (VTCarga*)lisAux->Items[nc];
			lisResCarga->Add(resCarga = new strResCarga);
			if (resRede->indexResCarga < 0)
				resRede->indexResCarga = lisResCarga->Count - 1;
			resCarga->carga = carga;
			resCarga->bloco = bloco;
			resCarga->rede = rede;
			resCarga->ncons = NCons(carga);
			resCarga->dic = 0.0;
			resCarga->fic = 0.0;
			resCarga->dicBT = 0.0;
			resCarga->ficBT = 0.0;
			resCarga->compensacao = 0.0;
			resCarga->dmic = 0.0;
		}
	}
	delete lisAux;
	delete lisBlocos;
}

// ---------------------------------------------------------------------------
void __fastcall TResCont::IniciaDicFicPu(bool flagZera) // default flagZera = true
{
	strResCarga *resCarga;
	if (flagZera == true)
	{
		for (int nr = 0; nr < lisResCarga->Count; nr++)
		{
			resCarga = (strResCarga*)lisResCarga->Items[nr];
			for (int nm = 0; nm < 12; nm++)
			{
				resCarga->dic_pu[nm] = 0.0;
				resCarga->fic_pu[nm] = 0.0;
			}
		}
	}
	else
	{
		double dicSoma, ficSoma;
		for (int nr = 0; nr < lisResCarga->Count; nr++)
		{
			resCarga = (strResCarga*)lisResCarga->Items[nr];
			dicSoma = 0.0;
			ficSoma = 0.0;
			for (int nm = 0; nm < 12; nm++)
			{
				dicSoma += resCarga->dic_pu[nm];
				ficSoma += resCarga->fic_pu[nm];
			}
			if (dicSoma > 0)
				for (int nm = 0; nm < 12; nm++)
					resCarga->dic_pu[nm] /= dicSoma;
			else
				for (int nm = 0; nm < 12; nm++)
					resCarga->dic_pu[nm] = 1 / 12;

			if (ficSoma > 0)
				for (int nm = 0; nm < 12; nm++)
					resCarga->fic_pu[nm] /= ficSoma;
			else
				for (int nm = 0; nm < 12; nm++)
					resCarga->fic_pu[nm] = 1 / 12;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TResCont::IniciaResultados(VTRede *rede)
{
	strResRede *resRede;
	strResBloco* resBloco = NULL;
	strResCarga* resCarga;

	if (rede == NULL)
	{
		resGeral->tdeslN = 0.0;
		resGeral->tdeslENDN = 0.0;
		resGeral->ndeslN = 0.0;
		resGeral->ndeslENDN = 0.0;
		resGeral->end = 0.0;
		resGeral->tdeslBTN = 0.0;
		resGeral->ndeslBTN = 0.0;
		resGeral->endBT = 0.0;
		resGeral->ndeslPisca = 0.0;
		resGeral->ndeslPiscaVIP = 0.0;
		resGeral->compensacao = 0.0;
		resGeral->dicVIP = 0.0;
		resGeral->ficVIP = 0.0;
		resGeral->ncargaVIP = 0;
		// resGeral->ncons    = 0;
	}

	// Reinicia resultados
	for (int nr = 0; nr < lisResRede->Count; nr++)
	{
		resRede = (strResRede*)lisResRede->Items[nr];
		if (rede != NULL)
		{
			if (resRede->rede != rede)
				continue;
		}
		resRede->tdeslN = 0.0;
		resRede->tdeslENDN = 0.0;
		resRede->ndeslN = 0.0;
		resRede->ndeslENDN = 0.0;
		resRede->end = 0.0;
		resRede->tdeslBTN = 0.0;
		resRede->ndeslBTN = 0.0;
		resRede->endBT = 0.0;
		resRede->ndeslPisca = 0.0; ;
		resRede->ndeslPiscaVIP = 0.0; ;
		resRede->compensacao = 0.0;
		resRede->dicVIP = 0.0;
		resRede->ficVIP = 0.0;
		resRede->ncargaVIP = 0;
		// resRede->ncons    = 0;
	}
	for (int nr = 0; nr < lisResBloco->Count; nr++)
	{
		resBloco = (strResBloco*)lisResBloco->Items[nr];

		if (rede != NULL)
		{
			// hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento
			// if(((VTBloco*)resBloco->bloco)->Rede == NULL) continue;
			// if(((VTBloco*)resBloco->bloco)->Rede != rede) continue; //hk2004
			if (resBloco->rede != rede)
				continue;
		}
		// resBloco->cBloco   = NULL;
		resBloco->tdesl = 0.0; // tempos(min)  (sem x ncons) para desligamentos > 3min
		resBloco->tdeslEND = 0.0; // tempos(min)  (sem x ncons) totais sem limitantes
		resBloco->ndesl = 0.0;
		// desligamentos(número/periodo) (sem x ncons) para desligamentos > 3min
		resBloco->ndeslEND = 0.0;
		// desligamentos(número/período) (sem x ncons) totais sem limitantes
		resBloco->end = 0.0; // end e energia não distribuida total
		resBloco->dcons = 0.0; // desliga x cons
		resBloco->tcons = 0.0; // tdesligax cons
		resBloco->nmanAuto = 0; // #manobras automáticas
		resBloco->nmanMan = 0; // #manobras manuais
		resBloco->npiscadas = 0; // número total de piscadas no bloco
		resBloco->dconsPisca = 0; // piscadas x nons
		resBloco->dconsPiscaVIP = 0; // piscadas x consVIP
	}

	for (int nr = 0; nr < lisResCarga->Count; nr++)
	{
		resCarga = (strResCarga*)lisResCarga->Items[nr];

		if (rede != NULL)
		{
			if (resCarga->rede != rede)
				continue; // hk2004
		}

		resCarga->dic = 0;
		resCarga->fic = 0;
		resCarga->dicBT = 0;
		resCarga->ficBT = 0;
		resCarga->compensacao = 0;
		// double  dic, fic, dicBT, ficBT;
		// double  dic_pu[12], fic_pu[12];
		// double  compensacao;
	}

}
// ---------------------------------------------------------------------------
// void __fastcall TResCont::LisResCargaRede(TList *lisExt, VTRede *rede)
// {
// Popula lista lisExt com resultados por carga BT da rede
// strResRede *resRede;
// strResCarga *resCarga;
// int indexResCarga;
// lisExt->Clear();
// if((resRede = PM_GetResRede(rede)) == NULL) return;
// indexResCarga = resRede->indexResCarga;

// for(int n = indexResCarga; n < lisResCarga->Count; n++)
// {
// if((resCarga = (strResCarga*)lisResCarga->Items[n])->rede != rede) break;
// lisExt->Add(resCarga);
// }
// }
// ---------------------------------------------------------------------------
int __fastcall TResCont::NCons(VTCarga *carga)
{
	int naux;

	naux = 0;
	naux += carga->NumConsResidencial;
	naux += carga->NumConsComercial;
	naux += carga->NumConsIndustrial;
	naux += carga->NumConsRural;
	naux += carga->NumConsOutros; // MISTURADO COM IP!!!! hk2015
	naux += carga->NumConsA4;
	// Valor mínimo de consumidores
	if (naux == 0)
		naux = 1;

	return (naux);
}

// ---------------------------------------------------------------------------
int __fastcall TResCont::NCons(TList *lisCargas)
{
	VTCarga *carga;
	int ncargas;
	int naux;

	ncargas = 0;
	for (int n = 0; n < lisCargas->Count; n++)
	{
		carga = (VTCarga*)lisCargas->Items[n];
		naux = NCons(carga);
		// naux  = 0;
		// naux += carga->NumConsResidencial;
		// naux += carga->NumConsComercial;
		// naux += carga->NumConsIndustrial;
		// naux += carga->NumConsRural;
		// naux += carga->NumConsOutros; //MISTURADO COM IP!!!! hk2015
		// naux += carga->NumConsA4;
		ncargas += naux;
	}
	if (ncargas == 0)
		ncargas = lisCargas->Count;

	return (ncargas);
}

// ---------------------------------------------------------------------------
int __fastcall TResCont::NConsVIP(TList *lisCargas)
{
	VTCarga *carga;
	int ncargas;
	int naux;

	ncargas = 0;
	for (int n = 0; n < lisCargas->Count; n++)
	{
		carga = (VTCarga*)lisCargas->Items[n];
		if (!carga->VeryImportant)
			continue;
		naux = 0;
		naux += carga->NumConsResidencial;
		naux += carga->NumConsComercial;
		naux += carga->NumConsIndustrial;
		naux += carga->NumConsRural;
		naux += carga->NumConsOutros; // MISTURADO COM IP!!!! hk2015
		naux += carga->NumConsA4;

		ncargas += naux;
	}
	// if(ncargas == 0) ncargas = lisCargas->Count;

	return (ncargas);
}

// ---------------------------------------------------------------------------
double __fastcall TResCont::Indic(strResBloco *resBloco, int ind)
{
	double valor;
	// 0:tdesl 1:ndesl 2:end 3:tcons 4:dcons
	switch (ind)
	{
	case 0:
		valor = resBloco->tdesl / 60.0;
		break;
	case 1:
		valor = resBloco->ndesl;
		break;
	case 2:
		valor = resBloco->end;
		break;
	case 3:
		valor = resBloco->tcons / 60.0;
		break;
	case 4:
		valor = resBloco->dcons;
		break;
	default:
		valor = 0.0;
		break;
	}
	return (valor);
}

// ---------------------------------------------------------------------------
void __fastcall TResCont::ImprimeResGeralRede(AnsiString arq)
{
	FILE *fout;
	strResRede *resRede;
	fout = fopen(arq.c_str(), "wt");

	fprintf(fout, "Resultados Gerais\n");
	fprintf(fout, "Rede\t Consum\t Clih\t Cli\t DEC\t FEC\t END\t ");
	fprintf(fout, "ClihBT\t CliBT\t DECBT\t FECBT\t ENDBT\n");

	fprintf(fout, "Geral\t %i\t", resGeral->ncons);
	fprintf(fout, "%6.2f\t %6.2f\t", resGeral->tdeslN, resGeral->ndeslN);
	if (resGeral->ncons > 0)
		fprintf(fout, "%6.2f\t %6.2f\t", resGeral->tdeslN / 60. / (double)resGeral->ncons,
		resGeral->ndeslN / (double)resGeral->ncons);
	else
		fprintf(fout, "-\t -\t");

	fprintf(fout, "%6.2f\t", resGeral->end);
	fprintf(fout, "%6.2f\t %6.2f\t", resGeral->tdeslBTN, resGeral->ndeslBTN);
	if (resGeral->ncons > 0)
		fprintf(fout, "%6.2f\t %6.2f\t", resGeral->tdeslBTN / 60. / (double)resGeral->ncons,
		resGeral->ndeslBTN / (double)resGeral->ncons);
	else
		fprintf(fout, "-\t -\t");

	fprintf(fout, "%6.2f\n", resGeral->endBT);

	for (int n = 0; n < lisResRede->Count; n++)
	{
		resRede = (strResRede*)lisResRede->Items[n];
		fprintf(fout, "%s\t %i\t", resRede->rede->Codigo.c_str(), resRede->ncons);
		fprintf(fout, "%6.2f\t %6.2f\t", resRede->tdeslN, resRede->ndeslN);
		if (resRede->ncons > 0)
			fprintf(fout, "%6.2f\t %6.2f\t", resRede->tdeslN / 60. / (double)resRede->ncons,
			resRede->ndeslN / (double)resRede->ncons);
		else
			fprintf(fout, "-\t -\t");

		fprintf(fout, "%6.2f\t", resRede->end);
		fprintf(fout, "%6.2f\t %6.2f\t", resRede->tdeslBTN, resRede->ndeslBTN);
		if (resGeral->ncons > 0)
			fprintf(fout, "%6.2f\t %6.2f\t", resRede->tdeslBTN / 60. / (double)resRede->ncons,
			resRede->ndeslBTN / (double)resRede->ncons);
		else
			fprintf(fout, "-\t -\t");

		fprintf(fout, "%6.2f\n", resRede->endBT);
	}

	fclose(fout);
}

// ---------------------------------------------------------------------------
void __fastcall TResCont::ImprimeResBloco(AnsiString arq)
{
	FILE *fout;
	strResBloco* resBloco;

	fout = fopen(arq.c_str(), "wt");

	fprintf(fout, "n\t Rede\t tdeslBTN\t ndeslBTN\t");
	fprintf(fout, "dcons\t tcons\t");
	fprintf(fout, "nmanAuto\t nmanMan\n");
	for (int n = 0; n < lisResBloco->Count; n++)
	{
		resBloco = (strResBloco*)lisResBloco->Items[n];
		// hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento
		// fprintf(fout, "%i\t %s\t", n, resBloco->bloco->Rede->Codigo);
		if (resBloco->rede != NULL)
			fprintf(fout, "%i\t %s\t", n, resBloco->rede->Codigo.c_str());
		else
			fprintf(fout, "%i\t %s\t", n, "ND");

		fprintf(fout, "%6.2f\t %6.2f\t", resBloco->tdeslBTN, resBloco->ndeslBTN);
		fprintf(fout, "%6.2f\t %6.2f\t", resBloco->dcons, resBloco->tcons);
		fprintf(fout, "%6.2f\t %6.2f\n", resBloco->nmanAuto, resBloco->nmanMan);
	}
	fclose(fout);
}

// ---------------------------------------------------------------------------
void __fastcall TResCont::ImprimeResCarga(AnsiString arq)
{
	FILE *fout;
	strResCarga* resCarga;

	fout = fopen(arq.c_str(), "wt");
	fprintf(fout, "rede->Codigo\t ");
	fprintf(fout, "carga->Codigo\t ");
	fprintf(fout, "ncons\t ");
	fprintf(fout, "dic\t fic\t ");
	fprintf(fout, "dicBT\t ficBT\t  endBT\t ");
	fprintf(fout, "compensacao\t ");
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "dic_pu[%i]\t ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "fic_pu[%i]\t ", nm);
	fprintf(fout, "\n");

	for (int n = 0; n < lisResCarga->Count; n++)
	{
		resCarga = (strResCarga*)lisResCarga->Items[n];
		fprintf(fout, "%s\t ", resCarga->rede->Codigo.c_str());
		fprintf(fout, "%s\t ", resCarga->carga->Codigo.c_str());
		fprintf(fout, "%6.0f\t ", resCarga->ncons);
		fprintf(fout, "%6.2f\t %6.2f\t ", resCarga->dic, resCarga->fic);
		fprintf(fout, "%6.2f\t %6.2f\t  %6.2f\t ", resCarga->dicBT, resCarga->ficBT,
			resCarga->endBT);
		fprintf(fout, "%6.2f\t ", resCarga->compensacao);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%6.2f\t ", resCarga->dic_pu[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%6.2f\t ", resCarga->fic_pu[nm]);

		fprintf(fout, "\n");
	}
	fclose(fout);
	/*
	 VTCarga *carga;
	 VTBloco *bloco;
	 VTRede  *rede;
	 double  dic, fic, dicBT, ficBT;
	 double  endBT;
	 double  dic_pu[12], fic_pu[12];
	 double  compensacao;
	 double  ncons;
	 */

}
// ---------------------------------------------------------------------------
