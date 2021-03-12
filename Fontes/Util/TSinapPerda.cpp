//---------------------------------------------------------------------------
#pragma hdrstop

#include <Fontes\Apl\VTApl.h>
#include <Fontes\Consulta\VTConsulta.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTEqbar.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTResFlowLig.h>
#include <Fontes\Rede\VTResFlowSup.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTTrecho.h>
#include <Fontes\Rede\VTTipoRede.h>
//#include "..\..\Rede\VTRedes.h"
#include "TSinapPerda.h"
//#include "VTSinapPerda.h"
//#include "EstruturaPerdas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

 //---------------------------------------------------------------------------
VTSinapPerda* __fastcall NewObjSinapPerda(VTApl *apl_owner)
	{
	try{
		return(new TSinapPerda(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//---------------------------------------------------------------------------
__fastcall  TSinapPerda::TSinapPerda(VTApl *apl_owner)
	{
	//salva ponteiro p/ objetos
	this->apl = apl_owner;
	//cria objetos
  }

//---------------------------------------------------------------------------
__fastcall TSinapPerda::~TSinapPerda(void)
	{
	//destrói objetos
	//if (consulta)  {delete consulta;  consulta = NULL;}

	//destrói lista
	//if (lisPerdas)  {delete lisPerdas;  lisPerdas  = NULL;}
	//destrói lista e conteúdo
	//if (lisPerdas)  {DestroiLista(lisPerdas);}
	//if (lisBalanco)  {DestroiLista(lisBalanco);}
	}
//---------------------------------------------------------------------------
double __fastcall TSinapPerda::CalcEEntregueDia(void)
{	//variáveis locais
   double        entregue_diaria_mwh;
	double        entregue_mwh;
	TList         *lisCargas;
	VTCarga       *carga;
	//VTEqbar		  *eqBar;
	VTPatamares   *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	//VTRedes       *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTDemanda     *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   double        p_pat_mw, v_pu;

   //inicia energia de entrada diaria total
   entregue_diaria_mwh = 0;
	//obtém lista de Suprimentos
	//lisCargas = consulta->LisEqptoRedeReducao(redes, eqptoCARGA);
	lisCargas = new TList;
	redes->LisEqbar(lisCargas,eqptoCARGA); //ARRUMAR: caso somente REDES
	//rede->LisEqbar(lisCargas,eqptoCARGA);
   //loop p/ todos Suprimentos
	for (int ns = 0; ns < lisCargas->Count; ns++)
	{//calcula demanda da Carga
		carga = (VTCarga*)lisCargas->Items[ns];
		entregue_mwh = 0.;
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{//verifica se o fluxo foi calculado no patamar
			if (carga->pbarra->resflow->Diagnostico[ind_pat] == diagISOLADO) continue;
			//determina tensão mínima da Barra
			v_pu     = carga->pbarra->resflow->Vff_pu_min[ind_pat];//carga->pbarra->resflow->Vff_pu_min[ind_pat];
			p_pat_mw = demanda->P_mw[carga][ind_pat];
			switch(carga->ModeloCarga)
			{
				case cargaICTE: p_pat_mw *= v_pu;         break;
				case cargaZCTE: p_pat_mw *= (v_pu *v_pu); break;
				case cargaSCTE: break;
				default:        break;
			}
			entregue_mwh += p_pat_mw * patamares->Patamar[ind_pat]->Duracao;
		}
		entregue_diaria_mwh += entregue_mwh;
	}
	//salva perda total, hwh/mes
	//resumo->Entregue_mwhm[ano] = entregue_diaria_mwh * DIAS_MES;
	//balanco.eEntregue_mwh = entregue_diaria_mwh;
	if(lisCargas){ delete lisCargas; lisCargas = NULL;}
	return entregue_diaria_mwh;
}
//---------------------------------------------------------------------------
double __fastcall TSinapPerda::CalcEEntradaDia(void)
{
	//variáveis locais
	double        entrada_diaria_mwh;
	double        entrada_mwh;
	TList         *lisSup;
	//VTEqbar		  *eqBar;
	VTSuprimento  *suprimento;
	VTPatamares   *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	//VTRedes       *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//inicia energia de entrada diaria total
	entrada_diaria_mwh = 0;
	//obtém lista de Suprimentos
	//lisSUP = consulta->LisEqptoRedeReducao(redes, eqptoSUPRIMENTO);
	lisSup = new TList;
	redes->LisEqbar(lisSup,eqptoSUPRIMENTO); //ARRUMAR: caso somente REDES
	//rede->LisEqbar(lisSup, eqptoSUPRIMENTO);
	//loop p/ todos Suprimentos
	for (int ns = 0; ns < lisSup->Count; ns++)
	{//calcula energia de entrada diaria do Suprimento
		suprimento = (VTSuprimento*)lisSup->Items[ns];
		entrada_mwh = 0.;
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			entrada_mwh += suprimento->resflow->P_mw[ind_pat] * patamares->Patamar[ind_pat]->Duracao;
		}
		entrada_diaria_mwh += entrada_mwh;
	}
	//balanco.eEntrada_mwh = entrada_diaria_mwh;
	if(lisSup){ delete lisSup; lisSup = NULL;}
	return entrada_diaria_mwh;
}
//---------------------------------------------------------------------------
double __fastcall TSinapPerda::CalcEGeradaDia(void)
{
 	//variáveis locais
	double        gerada_dia_mwh;
   double        gerada_mwh;
	TList			  *lisEQP;
	VTGerador     *gerador;
   VTPatamares   *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	//VTRedes       *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia energia gerada no dia
   gerada_dia_mwh = 0;
	//obtém lista de Geradores
	lisEQP = new TList;
	redes->LisEqbar(lisEQP, eqptoGERADOR);
	//rede->LisEqbar(lisEQP,eqptoGERADOR);
	for (int ng = 0; ng < lisEQP->Count; ng++)
	{
		gerador = (VTGerador*)lisEQP->Items[ng];
		//calcula energia gerada pelo Gerador
		gerada_mwh = 0.;
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			gerada_mwh += gerador->resflow->P_mw[ind_pat] * patamares->Patamar[ind_pat]->Duracao;
		}
		gerada_dia_mwh += gerada_mwh;
	}
	///balanco.eGerada_mwh = gerada_dia_mwh;
	if(lisEQP){ delete lisEQP; lisEQP = NULL;}
	return gerada_dia_mwh;
}
//---------------------------------------------------------------------------
void __fastcall TSinapPerda::CalcPerdasDia(void)
{
	TList 	  *lisLigacao;
	VTLigacao  *ligacao;
	double	  trafo_mwh;
	//double	  trafoFerro_mwh;
	double	  cabo_mwh;
	double	  capacitor_mwh;
	double	  reator_mwh;
	double	  regulador_mwh;
	//double	  medidor_mwh;
	//double	  ramal_mwh;
	double	  outros_mwh;
	double	  total_mwh;
	double     *perdas;
	int			tipo;
	//strPERDAS  perdasAux;

	trafo_mwh =0;
	//trafoFerro_mwh=0;
	cabo_mwh=0;
	capacitor_mwh=0;
	reator_mwh=0;
	regulador_mwh=0;
	//medidor_mwh=0;
	//ramal_mwh=0;
	outros_mwh=0;
	total_mwh=0;
	//zera strPERDAS perdas



	lisLigacao = new TList;
	//redes->LisLigacao(lisLigacao);
	rede->LisLigacao(lisLigacao);
	for (int nl = 0; nl<lisLigacao->Count; nl++)
	{
		ligacao = (VTLigacao *) lisLigacao->Items[nl];
		tipo = ligacao->Tipo();
		if (ligacao->Tipo() == eqptoCHAVE) continue;
		switch (tipo)
		{
			case eqptoTRECHO:
				perdas = &cabo_mwh;
				break;
			case eqptoCAPACITOR:
				perdas = &capacitor_mwh;
				break;
			case eqptoREATOR:
				perdas = &reator_mwh;
				break;
			case eqptoREGULADOR:
				perdas = &regulador_mwh;
				break;
			case eqptoTRAFO:
				perdas = &trafo_mwh;
				break;
			case eqptoTRAFO3E:
				perdas = &trafo_mwh;
				break;
			case eqptoTRAFOZZ:
				perdas = &trafo_mwh;
				break;
			default:
				perdas = &outros_mwh;
				break;
		}

		*perdas += ligacao->resflow->PerdaDiaria_mwh;
		total_mwh += ligacao->resflow->PerdaDiaria_mwh;
	}
	perdasAux.trafo_mwh = trafo_mwh;
	perdasAux.cabo_mwh = cabo_mwh;
	perdasAux.capacitor_mwh = capacitor_mwh;
	perdasAux.reator_mwh =reator_mwh;
	perdasAux.regulador_mwh =regulador_mwh;
	perdasAux.outros_mwh = outros_mwh;
	perdasAux.total_mwh =total_mwh;

	if(lisLigacao){delete lisLigacao; lisLigacao = NULL;}
	//return perdasAux;
}
//---------------------------------------------------------------------------
void __fastcall TSinapPerda::DestroiLista(TList *lista)
{
	TObject *objeto;

	for (int i = lista->Count-1; i>=0; i--)
	{
		objeto = (TObject*) lista->Items[i];
		delete objeto;
		lista->Delete(i);
	}
	delete lista;
	lista = NULL;
}
//---------------------------------------------------------------------------
bool __fastcall  TSinapPerda::Executa(VTRedes *redes)
{
	bool sucesso;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	sucesso = true;

	if(! flow->UpToDate){sucesso=ExecutaFluxoPotencia();}
	this->redes = redes;
	if(sucesso)
	{
		balanco.eEntrada_mwh = CalcEEntradaDia();
		balanco.eEntregue_mwh = CalcEEntregueDia();
		balanco.eGerada_mwh = CalcEGeradaDia();
		InicializaPerdas();
		for(int nr = 0; nr<(redes->LisRede())->Count; nr++)
		{
			rede = (VTRede*) (redes->LisRede())->Items[nr];
			CalcPerdasDia();
			SomaPerdas(&perdasAux, &perdas);
		}
	}
	return sucesso;    //arrumar os returns
}
//---------------------------------------------------------------------------
bool __fastcall  TSinapPerda::Executa(TList *lisRedes)
{
	bool sucesso;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	sucesso = true;

	if(! flow->UpToDate){sucesso=ExecutaFluxoPotencia();}
	if(sucesso)
	{
		balanco.eEntrada_mwh = -1;
		balanco.eEntregue_mwh = -1;
		balanco.eGerada_mwh = -1;
		InicializaPerdas();
		for(int nr = 0; nr<lisRedes->Count; nr++)
		{
			rede = (VTRede*) lisRedes->Items[nr];
			CalcPerdasDia();
			SomaPerdas(&perdasAux, &perdas);
		}
	}
	return sucesso;    //arrumar os returns
}
//---------------------------------------------------------------------------
bool __fastcall  TSinapPerda::Executa(VTRede *rede)
{
	bool sucesso;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	sucesso = true;

	if(! flow->UpToDate){sucesso=ExecutaFluxoPotencia();}
	this->rede = rede;
	if(sucesso)
	{
		balanco.eEntrada_mwh = -1;
		balanco.eEntregue_mwh = -1;
		balanco.eGerada_mwh = -1;
		InicializaPerdas();
		CalcPerdasDia();
		SomaPerdas(&perdasAux, &perdas);
	}
	return sucesso;    //arrumar os returns
}
//---------------------------------------------------------------------------
bool __fastcall  TSinapPerda::ExecutaFluxoPotencia(void)
{
	//variáveis locais
	VTFlow    *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));
	//VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//atualiza ano no Mercado
	//mercado->AnoAtual = ano;
	//executa fluxo de potência equilibrado
	flow->HabilitaAviso(false);
	flow->HabilitaOtimizacao(true);
	flow->TipoRelatorio = relNENHUM;
	flow->TipoFluxo     = fluxoEQUILIBRADO;
	flow->UpToDate      = false;
	//executa cálculo de fluxo
	if (! flow->CalculaPorRede(redes)) return(false);//ano)) return(false);
	return(true);
}
//---------------------------------------------------------------------------
void __fastcall TSinapPerda::InicializaPerdas(void)
{
	perdas.trafo_mwh =0;
	perdas.cabo_mwh =0;
	perdas.capacitor_mwh =0;
	perdas.reator_mwh =0;
	perdas.regulador_mwh =0;
	perdas.outros_mwh =0;
	perdas.total_mwh =0;
}
//---------------------------------------------------------------------------
void		__fastcall TSinapPerda::SomaPerdas(strPERDAS *perda, strPERDAS *perdaAgregada)
{
	perdaAgregada->trafo_mwh 		+= perda->trafo_mwh;
	perdaAgregada->cabo_mwh 		+= perda->cabo_mwh;
	perdaAgregada->capacitor_mwh 	+= perda->capacitor_mwh;
	perdaAgregada->reator_mwh 		+= perda->reator_mwh;
	perdaAgregada->regulador_mwh 	+= perda->regulador_mwh;
	perdaAgregada->outros_mwh 		+= perda->outros_mwh;
	perdaAgregada->total_mwh 		+= perda->total_mwh;
}
//---------------------------------------------------------------------------
double  __fastcall TSinapPerda::PM_GetEnergiaEntrada_mwh(void)
{
	return balanco.eEntrada_mwh;
}
//---------------------------------------------------------------------------
double  __fastcall TSinapPerda::PM_GetEnergiaEntregue_mwh(void)
{
	return balanco.eEntregue_mwh;
}
//---------------------------------------------------------------------------
double  __fastcall TSinapPerda::PM_GetEnergiaGerada_mwh(void)
{
	return balanco.eGerada_mwh;
}
//---------------------------------------------------------------------------
bool 	  	__fastcall TSinapPerda::PM_GetIsAgregada(void)
{
	return isAgregado;
}
//---------------------------------------------------------------------------
double  	__fastcall TSinapPerda::PM_GetPerdaCabo_mwh(void)
{
	return perdas.cabo_mwh;
}
//---------------------------------------------------------------------------
double  	__fastcall TSinapPerda::PM_GetPerdaCapacitor_mwh(void)
{
	return perdas.capacitor_mwh;
}
//---------------------------------------------------------------------------
//double  __fastcall TSinapPerda::PM_GetPerdaMedidor_mwh(void)
//---------------------------------------------------------------------------
double  	__fastcall TSinapPerda::PM_GetPerdaOutros_mwh(void)
{
	return perdas.outros_mwh;
}
//---------------------------------------------------------------------------
//double  __fastcall TSinapPerda::PM_GetPerdaRamal_mwh(void)
//---------------------------------------------------------------------------
double  	__fastcall TSinapPerda::PM_GetPerdaReator_mwh(void)
{
	return perdas.reator_mwh;
}
//---------------------------------------------------------------------------
double  	__fastcall TSinapPerda::PM_GetPerdaRegulador_mwh(void)
{
	return perdas.regulador_mwh;
}
//---------------------------------------------------------------------------
double  	__fastcall TSinapPerda::PM_GetPerdaTotal_mwh(void)
{
	return perdas.total_mwh;
}
//---------------------------------------------------------------------------
double	__fastcall TSinapPerda::PM_GetPerdaTrafo_mwh(void)
{
	return perdas.trafo_mwh;
}
//---------------------------------------------------------------------------
//double  __fastcall TSinapPerda::PM_GetPerdaTrafoFerro_mwh(void)
//---------------------------------------------------------------------------
VTRede*	__fastcall TSinapPerda::PM_GetRede(void)
{
	return rede;
}
//---------------------------------------------------------------------------
int		__fastcall TSinapPerda::PM_GetSegmento(void)
{
	return idSeg;
}
//---------------------------------------------------------------------------









