// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCopia.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função para criar objeto VTCopia
VTCopia* __fastcall NewObjCopia(VTApl *apl)
{
	return (new TCopia(apl));
}

// ---------------------------------------------------------------------------
__fastcall TCopia::TCopia(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria lista
	lisEQP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TCopia::~TCopia(void)
{
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaBarras(TList *lisBARRA)
{
	// variáveis locais
	VTBarra *barra, *barra_clone;

	// loop p/ todas Barras
	for (int nb = 0; nb < lisBARRA->Count; nb++)
	{
		barra = (VTBarra*)lisBARRA->Items[nb];
		// duplica Barra
		if ((barra_clone = barra->Clone()) == NULL)
			return (false);
		// salva ponteiro da Barra clonada na Barra original
		barra->Obj = barra_clone;
		// insere Barra clonada na nova Redes
		redes_clone->InsereBarra(barra_clone);
	}
	try
	{
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaClusters(TList *lisCLUSTER)
{
	// variáveis locais
	TList *lisREDE;
	VTCluster *cluster, *cluster_clone;
	VTRede *rede, *rede_clone;

	try
	{ // loop p/ todas Barras
		for (int nc = 0; nc < lisCLUSTER->Count; nc++)
		{
			cluster = (VTCluster*)lisCLUSTER->Items[nc];
			// duplica Cluster
			if ((cluster_clone = cluster->Clone()) == NULL)
				return (false);
			// IMPORTANTE: limpa lista de Redes do clone do Cluster
			cluster_clone->Reinicia();
			// salva ponteiro do Cluster clonado na Cluster original
			cluster->Obj = cluster_clone;
			// obtém lista de Redes do Cluster original
			lisREDE = cluster->LisRede();
			// loop p/ todas Redes do Cluster original
			for (int nr = 0; nr < lisREDE->Count; nr++)
			{
				rede = (VTRede*)lisREDE->Items[nr];
				// determina clone da Rede original
				if ((rede_clone = (VTRede*)(rede->Obj)) == NULL)
					return (false);
				// insere Rede clonada no Cluster clonado
				cluster_clone->InsereRede(rede_clone);
			}
			// insere Cluster clonado no novo objeto Redes
			redes_clone->InsereCluster(cluster_clone);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaDominios(TList *lisDOMINIO)
{
	// variáveis locais
	VTDominio *dominio, *dominio_clone;

	try
	{ // loop p/ todos Dominios
		for (int n = 0; n < lisDOMINIO->Count; n++)
		{
			dominio = (VTDominio*)lisDOMINIO->Items[n];
			dominio_clone = dominio->Clone();
			// salva ponteiro do Dominio clonado no Dominio original
			dominio->Obj = dominio_clone;
			// insere Dominio clonado no novo objeto Redes
			redes_clone->InsereDominio(dominio_clone);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaEqbar(TList *lisBARRA)
{
	// variáveis locais
	TList *lisEQB;
	TList *lisCANAL;
	VTBarra *barra, *barra_clone;
	VTEqbar *eqbar, *eqbar_clone;
	VTMedidor *medidor_clone;
	VTCanal *canal_clone;

	try
	{ // loop p/ todas Barras
		for (int nb = 0; nb < lisBARRA->Count; nb++)
		{
			barra = (VTBarra*)lisBARRA->Items[nb];
			// determina Barra clonada
			if ((barra_clone = (VTBarra*)barra->Obj) == NULL)
				return (false);
			// obtém lista de Eqbar da Barra original
			lisEQB = barra->LisEqbar();
			// loop para todos Eqbar da Barra original
			for (int n = 0; n < lisEQB->Count; n++)
			{
				eqbar = (VTEqbar*)lisEQB->Items[n];
				// duplica Eqbar
				if ((eqbar_clone = eqbar->Clone()) == NULL)
					return (false);
				// salva ponteiro do Eqbar clonado no Eqbar original
				eqbar->Obj = eqbar_clone;
				// insere Eqbar clonado na Barra clonada
				barra_clone->InsereEqbar(eqbar_clone);
				// verifica se Eqbar é um Medidor
				if (eqbar->Tipo() == eqptoMEDIDOR)
				{ // associa Canais do Medidor clonado com Eqptos clonados
					medidor_clone = (VTMedidor*)eqbar_clone;
					// loop p/ todos Canais do Medidor clonado
					lisCANAL = medidor_clone->LisCanal();
					for (int nc = lisCANAL->Count - 1; nc >= 0; nc--)
					{
						canal_clone = (VTCanal*)lisCANAL->Items[nc];
						// IMPORTANTE: pode ser que o Eqpto do Canal seja de outra Rede e não tenha sido clonado
						if (canal_clone->Eqpto != NULL)
						{
							canal_clone->Eqpto = (VTEqpto*)(canal_clone->Eqpto->Obj);
						}
						if (canal_clone->Eqpto == NULL)
						{ // elimina o Canal
							medidor_clone->RemoveCanal(canal_clone);
							DLL_DeleteEqpto(canal_clone);
						}
					}
				}
			}
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaLigacoes(TList *lisREDE)
{
	// variáveis locais
	VTRede *rede, *rede_clone;

	try
	{ // loop p/ todas Redes
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			// determina Rede clonada
			if ((rede_clone = (VTRede*)(rede->Obj)) == NULL)
				return (false);
			// copia Ligacoes da Rede original para a Rede clonada
			if (!CopiaLigacoes(rede, rede_clone))
				return (false);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaLigacoes(VTRede *rede_orig, VTRede *rede_dest)
{
	// variáveis locais
	TList *lisLIG;
	VTLigacao *ligacao, *ligacao_clone;

	try
	{ // obtém lista de Ligacoes da Rede original
		lisLIG = rede_orig->LisLigacao();
		// loop para todas Ligacoes da Rede original
		for (int nl = 0; nl < lisLIG->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			// duplica Ligacao
			if ((ligacao_clone = ligacao->Clone()) == NULL)
				return (false);
			// salva ponteiro da Ligacao clonada na Ligacao original
			ligacao->Obj = ligacao_clone;
			// redefine Barras da Ligacao clonada
			if (!RedefineBarrasLigacao(ligacao, ligacao_clone))
				return (false);
			// insere Ligacao clonada na Rede destino
			rede_dest->InsereLigacao(ligacao_clone);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaMutuas(TList *lisREDE)
{
	// variáveis locais
	TList *lisMUTUA;
	VTBarra *barra, *barra_clone;
	VTTrecho *trecho, *trecho_clone;
	VTMutua *mutua, *mutua_clone;
	VTRede *rede, *rede_clone;

	try
	{ // loop p/ todas Redes
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			// determina Rede clonada
			if ((rede_clone = (VTRede*)(rede->Obj)) == NULL)
				return (false);
			// obtém lista de Mutuas da Rede original
			lisMUTUA = rede->LisMutua();
			// loop para todas Ligacoes da Rede original
			for (int n = 0; n < lisMUTUA->Count; n++)
			{
				mutua = (VTMutua*)lisMUTUA->Items[n];
				// duplica Mutua
				if ((mutua_clone = mutua->Clone()) == NULL)
					return (false);
				// salva ponteiro da Mutua clonada na Mjutua original
				mutua->Obj = mutua_clone;
				// redefine primeiro Trecho da Mutua
				if ((trecho = mutua->ptre1) == NULL)
					return (false);
				if ((trecho_clone = (VTTrecho*)(trecho->Obj)) == NULL)
					return (false);
				mutua_clone->ptre1 = trecho_clone;
				// redefine Barra de referência do primeiro Trecho da Mutua
				if ((barra = mutua->pbar_tre1) == NULL)
					return (false);
				if ((barra_clone = (VTBarra*)(barra->Obj)) == NULL)
					return (false);
				mutua_clone->pbar_tre1 = barra_clone;
				// redefine segundo Trecho da Mutua
				if ((trecho = mutua->ptre2) == NULL)
					return (false);
				if ((trecho_clone = (VTTrecho*)(trecho->Obj)) == NULL)
					return (false);
				mutua_clone->ptre2 = trecho_clone;
				// redefine Barra de referência do segundo Trecho da Mutua
				if ((barra = mutua->pbar_tre2) == NULL)
					return (false);
				if ((barra_clone = (VTBarra*)(barra->Obj)) == NULL)
					return (false);
				mutua_clone->pbar_tre2 = barra_clone;
			}
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TCopia::CopiaLisRedeParaRedes(TList *lisREDE, VTRedes *redes)
 {
 //variáveis locais
 bool        sucesso = true;
 VTRede      *rede;
 VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

 try{//reinicia objeto redes2
 redes->Clear();
 //atualiza ponteiro redes_clone (escopo da classe)
 redes_clone = redes;
 //cancela ponteiro Obj de todas Ligacoes
 ReiniciaPonteiroObj(lisREDE);
 //cria cópias dos Dominios
 //if (sucesso) sucesso = CopiaDominios(redes1->LisDominio());
 //cria cópias das Barras
 for (int n = 0; n < lisREDE->Count; n++)
 {
 rede = (VTRede*)lisREDE->Items[n];
 if (sucesso) sucesso = CopiaBarras(rede->LisBarra());
 }
 //cria cópias das Redes
 if (sucesso) sucesso = CopiaRedes(lisREDE);
 //cria cópias das Ligacoes das Redes
 if (sucesso) sucesso = CopiaLigacoes(lisREDE);
 //cria cópias das Mutuas das Redes
 if (sucesso) sucesso = CopiaMutuas(lisREDE);
 //cria cópias dos Clusters
 //if (sucesso) sucesso = CopiaClusters(lisREDE);
 //cria cópias dos Eqbar: precisa ser feito por último por causa dos Medidores
 for (int n = 0; n < lisREDE->Count; n++)
 {
 rede = (VTRede*)lisREDE->Items[n];
 if (sucesso) sucesso = CopiaEqbar(rede->LisBarra());
 }
 //reconfigura Redes
 if (sucesso) sucesso = topologia->Reconfigura(redes_clone);
 }catch(Exception &e)
 {
 sucesso = false;
 }
 return(sucesso);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaLisRedeParaRedes(TList *lisREDE, VTRedes *redes)
{
	// variáveis locais
	bool sucesso = true;
	VTRedes *redes1;
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	// importante: lisREDE contém objetos VTRedes (e não objetos VTRede)
	try
	{ // reinicia objeto redes
		redes->Clear();
		// cancela ponteiro Obj de todas Ligacoes
		for (int n = 0; n < lisREDE->Count; n++)
		{
			redes1 = (VTRedes*)lisREDE->Items[n];
			// cancela ponteiro Obj de todos Eqptos
			ReiniciaPonteiroObj(redes1->LisRede());
		}
		// atualiza ponteiro redes_clone (escopo da classe)
		redes_clone = redes;
		// loop p/ todos objetos Redes em lisREDE
		for (int n = 0; sucesso && (n < lisREDE->Count); n++)
		{
			redes1 = (VTRedes*)lisREDE->Items[n];
			// cria cópias dos Dominios
			if (sucesso)
				sucesso = CopiaDominios(redes1->LisDominio());
			// cria cópias das Barras
			if (sucesso)
				sucesso = CopiaBarras(redes1->LisBarra());
			// cria cópias das Redes
			if (sucesso)
				sucesso = CopiaRedes(redes1->LisRede());
			// cria cópias das Ligacoes das Redes
			if (sucesso)
				sucesso = CopiaLigacoes(redes1->LisRede());
			// cria cópias das Ligaões isoladas
			if (sucesso)
				sucesso = CopiaLigacoes(redes1->RedeIsolada(), redes_clone->RedeIsolada());
			// cria cópias das Mutuas das Redes
			if (sucesso)
				sucesso = CopiaMutuas(redes1->LisRede());
			// cria cópias dos Clusters
			if (sucesso)
				sucesso = CopiaClusters(redes1->LisCluster());
			// cria cópias dos Eqbar: precisa ser feito por último por causa dos Medidores
			if (sucesso)
				sucesso = CopiaEqbar(redes1->LisBarra());
		}
		// reconfigura Redes
		if (sucesso)
			sucesso = topologia->Reconfigura(redes_clone);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2)
{
	// variáveis locais
	bool sucesso = true;
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	try
	{ // reinicia objeto redes2
		redes2->Clear();
		// atualiza ponteiro redes_clone (escopo da classe)
		redes_clone = redes2;
		// cancela ponteiro Obj de todas Ligacoes
		ReiniciaPonteiroObj(redes1->LisRede());
		// cria cópias dos Dominios
		if (sucesso)
			sucesso = CopiaDominios(redes1->LisDominio());
		// cria cópias das Barras
		if (sucesso)
			sucesso = CopiaBarras(redes1->LisBarra());
		// cria cópias das Redes
		if (sucesso)
			sucesso = CopiaRedes(redes1->LisRede());
		// cria cópias das Ligacoes das Redes
		if (sucesso)
			sucesso = CopiaLigacoes(redes1->LisRede());
		// cria cópias das Ligaões isoladas
		if (sucesso)
			sucesso = CopiaLigacoes(redes1->RedeIsolada(), redes_clone->RedeIsolada());
		// cria cópias das Mutuas das Redes
		if (sucesso)
			sucesso = CopiaMutuas(redes1->LisRede());
		// cria cópias dos Clusters
		if (sucesso)
			sucesso = CopiaClusters(redes1->LisCluster());
		// cria cópias dos Eqbar: precisa ser feito por último por causa dos Medidores
		if (sucesso)
			sucesso = CopiaEqbar(redes1->LisBarra());
		// reconfigura Redes
		if (sucesso)
			sucesso = topologia->Reconfigura(redes_clone);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::DuplicaCargas(TList *lisCARGA)
{
	// variáveis locais
	VTCarga *carga, *carga_clone;

	try
	{ // cria clones das Cargas
		for (int nc = 0; nc < lisCARGA->Count; nc++)
		{
			carga = (VTCarga*)lisCARGA->Items[nc];
			carga_clone = carga->Clone();
			// associa Carga com sua Carga clonada e vice-versa
			carga->Obj = carga_clone;
			carga_clone->Obj = carga;
			// substitui a Carga original pelo seu Clone em lisCARGA
			lisCARGA->Items[nc] = carga_clone;
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::DuplicaCargas(VTRedes *redes, TList *lisCARGA)
{
	// variáveis locais
	VTBarra *barra;
	VTCarga *carga, *carga_clone;
	TList *lisBAR = redes->LisBarra();

	try
	{ // inicia lisCARGA  com Cargas de todas as Barras
		lisCARGA->Clear();
		// loop p/ todas Barras
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			// obtém lista de Cargas da Barra
			barra->LisEqbar(lisCARGA, eqptoCARGA);
		}
		// cria clones das Cargas
		for (int nc = 0; nc < lisCARGA->Count; nc++)
		{
			carga = (VTCarga*)lisCARGA->Items[nc];
			carga_clone = carga->Clone();
			// associa Carga com sua Carga clonada e vice-versa
			carga->Obj = carga_clone;
			carga_clone->Obj = carga;
		}
		// substitui as Cargas pelos Clones em lisCARGA
		for (int nc = 0; nc < lisCARGA->Count; nc++)
		{
			carga = (VTCarga*)lisCARGA->Items[nc];
			lisCARGA->Items[nc] = carga->Obj;
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::CopiaRedes(TList *lisREDE)
{
	// variáveis locais
	VTBarra *barra, *barra_clone;
	VTRede *rede, *rede_clone;

	try
	{ // loop p/ todas Redes
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			// duplica Rede
			rede_clone = rede->Clone();
			// cancela eventual Cluster
			rede_clone->Cluster = NULL;
			// salva ponteiro da Rede clonada na Rede original
			rede->Obj = rede_clone;
			// redefine Barra inicial da rede
			if ((barra = rede->BarraInicial()) != NULL)
			{
				barra_clone = (VTBarra*)(barra->Obj);
				rede_clone->DefineBarraInicial(barra_clone);
			}
			// insere Rede clonada na nova Redes
			redes_clone->InsereRede(rede_clone);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTRedes* __fastcall TCopia::DuplicaRedes(VTRedes *redes)
{
	// variáveis locais
	bool sucesso;
	VTRedes *redes2;

	try
	{ // cria cópia do objeto Redes
		if ((redes2 = DLL_NewObjRedes(apl)) == NULL)
			return (NULL);
		sucesso = CopiaRedes1ParaRedes2(redes, redes2);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	try
	{ // verifica se não houve erro
		if (!sucesso)
		{ // destrói objeto redes2
			if (redes2)
			{
				delete redes2;
				redes2 = NULL;
			}
		}
	}
	catch (Exception &e)
	{
		redes2 = NULL;
	}
	return (redes2);
}

// ---------------------------------------------------------------------------
VTRedes* __fastcall TCopia::DuplicaUmaRede(VTRedes *redes, VTRede *rede)
{
	// variáveis locais
	bool sucesso;
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	try
	{ // proteção não pode pertencer a um Cluster
		// if (rede->Cluster) return(NULL);
		// cancela ponteiro Obj de todas Ligacoes
		ReiniciaPonteiroObj(redes->LisRede());
		// cria cópia do objeto Redes
		sucesso = ((redes_clone = DLL_NewObjRedes(apl)) != NULL);
		// cria cópias apenas das Barras da Rede
		if (sucesso)
			sucesso = CopiaBarras(rede->LisBarra());
		// cria cópia da Rede
		lisEQP->Clear();
		lisEQP->Add(rede);
		if (sucesso)
			sucesso = CopiaRedes(lisEQP);
		// cria cópias das Ligacoes da Rede
		if (sucesso)
			sucesso = CopiaLigacoes(lisEQP);
		// cria cópias das Mutuas da Rede
		if (sucesso)
			sucesso = CopiaMutuas(lisEQP);
		// cria cópias dos Clusters
		// if (sucesso) sucesso = CopiaClusters(redes->LisCluster());
		// cria cópias dos Eqbar: precisa ser feito por último por causa dos Medidores
		if (sucesso)
			sucesso = CopiaEqbar(rede->LisBarra());
		// reconfigura Redes
		if (sucesso)
			sucesso = topologia->Reconfigura(redes_clone);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	try
	{ // verifica se não houve erro
		if (!sucesso)
		{ // destrói objeto clone.redes
			if (redes_clone)
			{
				delete redes_clone;
				redes_clone = NULL;
			}
		}
	}
	catch (Exception &e)
	{
		redes_clone = NULL;
	}
	return (redes_clone);
}

// ---------------------------------------------------------------------------
bool __fastcall TCopia::RedefineBarrasLigacao(VTLigacao *ligacao, VTLigacao *ligacao_clone)
{
	// variáveis locais
	VTBarra *barra, *barra_clone, *vet_barra_clone[NBAR_LIG];
	VTRegulador *regulador, *regulador_clone;
	VTTrafo *trafo, *trafo_clone;

	// determina Barras clonadas da Ligacao
	for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	{
		barra_clone = NULL;
		if ((barra = ligacao->Barra(ind_bar)) != NULL)
		{ // determina Barra clonada
			if ((barra_clone = (VTBarra*)(barra->Obj)) == NULL)
				return (false);
		}
		vet_barra_clone[ind_bar] = barra_clone;
	}
	// redefine Barras da Ligacao clonada
	ligacao_clone->DefineObjBarra(vet_barra_clone[0], vet_barra_clone[1], vet_barra_clone[2]);
	// verifica se Ligacao é um Trafo
	if (ligacao->Tipo() == eqptoTRAFO)
	{
		trafo = (VTTrafo*)ligacao;
		trafo_clone = (VTTrafo*)ligacao_clone;
		if (trafo->ltc.ajuste_auto.bar_ref != NULL)
		{ // redefine Barra associada ao LTC
			if ((barra_clone = (VTBarra*)(trafo->ltc.ajuste_auto.bar_ref->Obj)) == NULL)
				return (false);
			trafo_clone->ltc.ajuste_auto.bar_ref = barra_clone;
		}
	}
	// verifica se Ligacao é um Trafo
	else if (ligacao->Tipo() == eqptoREGULADOR)
	{
		regulador = (VTRegulador*)ligacao;
		regulador_clone = (VTRegulador*)ligacao_clone;
		if (regulador->sentido.bar_ref != NULL)
		{ // redefine Barra associada ao LTC
			if ((barra_clone = (VTBarra*)(regulador->sentido.bar_ref->Obj)) == NULL)
				return (false);
			regulador_clone->sentido.bar_ref = barra_clone;
		}

		if (regulador->cfg_auto.bar_ref != NULL)
		{ // redefine Barra de referência sentido direto
			if ((barra_clone = (VTBarra*)(regulador->cfg_auto.bar_ref->Obj)) == NULL)
				return (false);
			regulador_clone->cfg_auto.bar_ref = barra_clone;
		}

		if (regulador->cfg_auto.bar_ref_inv != NULL)
		{ // redefine Barra de referência sentido inverso
			if ((barra_clone = (VTBarra*)(regulador->cfg_auto.bar_ref_inv->Obj)) == NULL)
				return (false);
			regulador_clone->cfg_auto.bar_ref_inv = barra_clone;
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TCopia::ReiniciaPonteiroObj(TList *lisREDE)
{
	// variáveis locais
	TList *lisBAR;
	TList *lisEQB;
	TList *lisLIG;
	TList *lisCANAL;
	VTBarra *barra;
	VTEqbar *eqbar;
	VTLigacao *ligacao;
	VTCanal *canal;
	VTMedidor *medidor;
	VTRede *rede;

	// loop p/ todas Redes
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		if (!rede->Carregada)
			continue;
		// loop p/ todas Ligacoes da Rede
		lisLIG = rede->LisLigacao();
		for (int nl = 0; nl < lisLIG->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			ligacao->Obj = NULL;
		}
		// loop p/ todas Barras da Rede
		lisBAR = rede->LisBarra();
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			barra->Obj = NULL;
		}
		// loop p/ todos Eqbar da Rede
		for (int nb = 0; nb < lisBAR->Count; nb++)
		{
			barra = (VTBarra*)lisBAR->Items[nb];
			lisEQB = barra->LisEqbar();
			for (int ne = 0; ne < lisEQB->Count; ne++)
			{
				eqbar = (VTEqbar*)lisEQB->Items[ne];
				eqbar->Obj = NULL;
				// verifica se é um Medidor
				if (eqbar->Tipo() == eqptoMEDIDOR)
				{
					medidor = (VTMedidor*)eqbar;
					// obtém lista de Canais do Medidor
					lisCANAL = medidor->LisCanal();
					for (int nc = 0; nc < lisCANAL->Count; nc++)
					{
						canal = (VTCanal*)lisCANAL->Items[nc];
						if (canal->Eqpto != NULL)
							canal->Eqpto->Obj = NULL;
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// eof
