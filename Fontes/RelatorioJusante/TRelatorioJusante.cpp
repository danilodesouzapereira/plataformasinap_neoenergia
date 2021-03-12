// ----------------------------------------------------------------------------
#pragma hdrstop

#include <Fontes\Apl\VTApl.h>
#include "..\Arranjo\VTArranjo.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Bloco\VTElo.h"
#include "..\Distancia\VTCaminho.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h" //teste
#include "..\Importa\Monta\VTMonta.h" //teste
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\TRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
//#include "..\Rede\VTResflowBar.h"
//#include "..\Rede\VTResflowLig.h"
//#include "..\Rede\VTResflowSup.h"
#include "Complexo.h"
#include "Dados\TArranjoTrecho.h"

#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Distancia.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Potencia.h"
#include "..\..\DLL_Inc\Radial.h"

#include "TRelatorioJusante.h"
// ----------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTRelatorioJusante* NewObjRelatorioJusante(VTApl *apl)
{
	return (new TRelatorioJusante(apl));
}

// ----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17283172
__fastcall TRelatorioJusante::TRelatorioJusante(VTApl *apl_owner)
{
	this->apl = NewObjApl(NULL,apl_owner);
	VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

	blocos_executado = false;
	if(blocos == NULL)
	{
		apl->Add(DLL_NewObjBlocos());
    }
	// cria listas
	lisBarra = new TList();
	lisLigacao = new TList();
	lisPrimario = new TList();
	lisLigacaoJusante = new TList();
	lisDestaqueJusante = new TList();
	lisLigacaoMontante = new TList();
	lisDestaqueMontante = new TList();
	//cria arvores
	arvore_lig = DLL_NewObjArvore(apl);
	arvore_bar = DLL_NewObjArvore(apl);

	caminho = DLL_NewObjCaminho(apl);

	colorDestaque = clAqua;
    //não inicia arvore agora
	//IniciaArvoreBlocos();
}

// ----------------------------------------------------------------------------
__fastcall TRelatorioJusante::~TRelatorioJusante(void)
{

    if (caminho)   {delete caminho;   caminho   = NULL;}
	DestroiLista(lisLigacaoMontante);
	DestroiLista(lisLigacaoJusante);
	DestroiLista(lisBarra);
	DestroiLista(lisLigacao);
	DestroiLista(lisDestaqueJusante);
	DestroiLista(lisDestaqueMontante);
	DestroiTObject(apl);
	if(arvore_lig)
	{
		delete arvore_lig;
		arvore_lig = NULL;
	}
	if(arvore_bar)
	{
		delete arvore_bar;
		arvore_bar = NULL;
	}

}


// ----------------------------------------------------------------------------
//void __fastcall TRelatorioJusante::AdicionaListaLigacoesJusanteSecundario(VTSecundario *secundario)
//{
//
//	// adquire a lista ordenadas de ligações do secundário.
//	secundario->Ordena(lisBarra, lisLigacao);
//
//	// Procura a ligação em comum entre as ligações vindas do primário e do secundário
//	for (int i = 0; i < lisLigacao->Count; i++)
//	{
//		VTLigacao *ligacaoSecundario = (VTLigacao*)lisLigacao->Items[i];
//		for (int j = 0; j < lisLigacaoJusante->Count; j++)
//		{
//			VTLigacao *ligacaoOficial = (VTLigacao*)lisLigacaoJusante->Items[j];
//			VTBarra *barra = ligacaoOficial->BarraComum(ligacaoSecundario);
//			if (barra)
//			{
//				// Coloca a ligação do secundário na lista de ligações a jusante
//				lisLigacaoJusante->Add(ligacaoSecundario);
//				break;
//			}
//		}
//	}
//
//	// percorrer lisLigacoes em Loop reverso, para ver as ligacões filhas dessa ligação base.
//	for (int j = 0; j < lisLigacaoJusante->Count; j++)
//	{
//		VTLigacao *ligacaoReferencia = (VTLigacao*)lisLigacaoJusante->Items[j];
//		for (int i = (lisLigacao->Count - 1); i > 0; i--)
//		{
//			VTLigacao *ligacaoOficial = (VTLigacao*)lisLigacao->Items[i];
//			if (ligacaoOficial->ligaPai != NULL && ligacaoOficial->ligaPai == ligacaoReferencia)
//			{
//				lisLigacaoJusante->Add(ligacaoOficial);
//			}
//		}
//	}
//
//}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::AgregaCarga(VTEqpto *eqptoRef, strpi_CARGA &str)
{
    //variáveis locais
	//VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	TList *lisCARGA;
	VTCarga *carga;
	VTBarra *barra;

	//protecao
	if(patamar == NULL)
	{
		throw new Exception("Patamar não selecionado.");
		return;
	}
	lisCARGA = new TList;

	str.at.qtde = 0;
	str.at.P_mw = 0;
	str.at.Q_mvar = 0;
	str.at.S_mva = 0;
	str.mt.qtde = 0;
	str.mt.P_mw = 0;
	str.mt.Q_mvar = 0;
	str.mt.S_mva = 0;
	str.bt.qtde = 0;
	str.bt.P_mw = 0;
	str.bt.Q_mvar = 0;
	str.bt.S_mva = 0;
	str.total.qtde = 0;
	str.total.P_mw = 0;
	str.total.Q_mvar = 0;
	str.total.S_mva = 0;

	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisCARGA, eqptoRef, eqptoCARGA);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisCARGA, eqptoRef, eqptoCARGA);}
	//percorre a lista de cargas e agrega os valores
	for(int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*) lisCARGA->Items[nc];
		barra = carga->pbarra;

		//classifica de acordo com a tensao
		if((barra->vnom > 69.0) || (IsDoubleZero(barra->vnom - 69.0)))
		{
			str.at.qtde++;
			str.at.P_mw += demanda->P_mw[carga][patamar->Index];
			str.at.Q_mvar += demanda->Q_mvar[carga][patamar->Index];
			str.at.S_mva += demanda->S_mva[carga][patamar->Index];
		}
		else if((barra->vnom > 1.0) || (IsDoubleZero(barra->vnom - 1.0)))
		{
			str.mt.qtde++;
			str.mt.P_mw += demanda->P_mw[carga][patamar->Index];
			str.mt.Q_mvar += demanda->Q_mvar[carga][patamar->Index];
			str.mt.S_mva += demanda->S_mva[carga][patamar->Index];
		}
		else
		{
			str.bt.qtde++;
			str.bt.P_mw += demanda->P_mw[carga][patamar->Index];
			str.bt.Q_mvar += demanda->Q_mvar[carga][patamar->Index];
			str.bt.S_mva += demanda->S_mva[carga][patamar->Index];
		}
		str.total.qtde++;
		str.total.P_mw += demanda->P_mw[carga][patamar->Index];
		str.total.Q_mvar += demanda->Q_mvar[carga][patamar->Index];
		str.total.S_mva += demanda->S_mva[carga][patamar->Index];
	}

	DestroiLista(lisCARGA);
}
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaCargaValores(TStrings *key_value,
															  VTEqpto *eqptoRef)
{
	strpi_CARGA str;
	double modulo;

	//deveria ser por fase??
	try
	{
		AgregaCarga(eqptoRef, str);
	}
	catch (Exception &e)
	{
		str.at.qtde = -1;
		str.at.P_mw = -1;
		str.at.Q_mvar = -1;
		str.at.S_mva = -1;
		str.mt.qtde = -1;
		str.mt.P_mw = -1;
		str.mt.Q_mvar = -1;
		str.mt.S_mva = -1;
		str.bt.qtde = -1;
		str.bt.P_mw = -1;
		str.bt.Q_mvar = -1;
		str.bt.S_mva = -1;
		str.total.qtde = -1;
		str.total.P_mw = -1;
		str.total.Q_mvar = -1;
		str.total.S_mva = -1;
	}
	//limpa a lista
	key_value->Clear();
	modulo = Abs(str.total.S_mva);
	key_value->Add("Número de cargas=" + IntToStr(str.total.qtde));
	key_value->Add("Potência Ativa [MW]=" + DoubleToStr("%4.3f",str.total.P_mw ));
	key_value->Add("Potência Reativa [MVAr]=" + DoubleToStr("%4.3f",str.total.Q_mvar ));
	key_value->Add("Módulo da Potência Aparente [MVA]=" + DoubleToStr("%4.3f",modulo ));
	if(str.at.qtde > 0)
	{
		modulo = Abs(str.at.S_mva);
		key_value->Add("Número de cargas AT=" + IntToStr(str.at.qtde));
		key_value->Add("Potência Ativa AT [MW]=" + DoubleToStr("%4.3f",str.at.P_mw ));
		key_value->Add("Potência Reativa AT [MVAr]=" + DoubleToStr("%4.3f",str.at.Q_mvar ));
		key_value->Add("Módulo da Potência Aparente AT [MVA]=" + DoubleToStr("%4.3f",modulo ));
	}
	if(str.mt.qtde > 0)
	{
		modulo = Abs(str.mt.S_mva);
		key_value->Add("Número de cargas MT=" + IntToStr(str.mt.qtde));
		key_value->Add("Potência Ativa MT [MW]=" + DoubleToStr("%4.3f",str.mt.P_mw ));
		key_value->Add("Potência Reativa MT [MVAr]=" + DoubleToStr("%4.3f",str.mt.Q_mvar ));
		key_value->Add("Módulo da Potência Aparente MT [MVA]=" + DoubleToStr("%4.3f",modulo ));
	}
	if(str.bt.qtde > 0)
	{
		modulo = Abs(str.bt.S_mva);
		key_value->Add("Número de cargas BT=" + IntToStr(str.bt.qtde));
		key_value->Add("Potência Ativa BT [MW]=" + DoubleToStr("%4.3f",str.bt.P_mw ));
		key_value->Add("Potência Reativa BT [MVAr]=" + DoubleToStr("%4.3f",str.bt.Q_mvar ));
		key_value->Add("Módulo da Potência Aparente BT [MVA]=" + DoubleToStr("%4.3f",modulo ));
	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::AgregaConsumidores(VTEqpto *eqptoRef, strpi_CONSUMIDOR &str)
{
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	TList *lisCARGA;
	VTCarga *carga;
	VTBarra *barra;
	int total_carga = 0;
	lisCARGA = new TList;

	str.energia_mwhmes = 0;
	str.qtde.residencial = 0;
	str.qtde.comercial = 0;
	str.qtde.industrial = 0;
	str.qtde.rural = 0;
	str.qtde.outros = 0;
	str.qtde.bt = 0;
	str.qtde.mt = 0;
	str.qtde.at = 0;
	str.qtde.total = 0;
	// Tipo de relatorio
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisCARGA, eqptoRef, eqptoCARGA);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisCARGA, eqptoRef, eqptoCARGA);}
	//percorre a lista de cargas e agrega os valores
	for(int nc = 0; nc < lisCARGA->Count; nc++)
	{
		total_carga = 0;
		carga = (VTCarga*) lisCARGA->Items[nc];
		barra = carga->pbarra;
		total_carga = carga->NumConsResidencial + carga->NumConsComercial +
			carga->NumConsIndustrial + carga->NumConsRural + carga->NumConsOutros
			 +  carga->NumConsA4;
		str.qtde.total += total_carga;
		//str.energia_mwhmes += carga->Energia_mwhmes;
		str.energia_mwhmes += demanda->Emes_mwh[carga];
		str.qtde.residencial += carga->NumConsResidencial;
		str.qtde.comercial += carga->NumConsComercial;
		str.qtde.industrial += carga->NumConsIndustrial;
		str.qtde.rural += carga->NumConsRural;
		str.qtde.outros += carga->NumConsOutros + carga->NumConsA4;

		//classifica de acordo com a tensao
		if((barra->vnom > 69.0) || (IsDoubleZero(barra->vnom - 69.0)))
			{str.qtde.at += total_carga;}
		else if((barra->vnom > 1.0) || (IsDoubleZero(barra->vnom - 1.0)))
			{str.qtde.mt += total_carga;}
		else
			{str.qtde.bt += total_carga;}
	}

	DestroiLista(lisCARGA);
}
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaConsumidoresValores(TStrings *key_value,
															  VTEqpto *eqptoRef)
{
	strpi_CONSUMIDOR str;

	AgregaConsumidores(eqptoRef, str);
	//limpa a lista
	key_value->Clear();
	key_value->Add("Consumo mensal [MWh/mês]=" + DoubleToStr("%4.3f",str.energia_mwhmes));
	key_value->Add("Consumidores totais=" + IntToStr(str.qtde.total));
	key_value->Add("Consumidores AT=" + IntToStr(str.qtde.at));
	key_value->Add("Consumidores MT=" + IntToStr(str.qtde.mt));
	key_value->Add("Consumidores BT=" + IntToStr(str.qtde.bt));
	key_value->Add("Consumidores residenciais=" + IntToStr(str.qtde.residencial));
	key_value->Add("Consumidores comerciais=" + IntToStr(str.qtde.comercial));
	key_value->Add("Consumidores industriais=" + IntToStr(str.qtde.industrial));
	key_value->Add("Consumidores rurais=" + IntToStr(str.qtde.rural));
	key_value->Add("Outros consumidores=" + IntToStr(str.qtde.outros));
}

//https://sinapsisenergia.teamwork.com/#/tasks/16586943
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaResumo(VTEqpto *eqptoRef, strpi_RESUMO &str)
{
	// Variáveis locais
	TList *lisEXT;
	lisEXT = new TList;
	VTCarga *carga;
	VTTrecho *trecho;
	int total_carga = 0;
	double comprimento_trecho = 0.0;
	str.comp_trecho_km = 0.0;

	// Codigo do equipamento de referencia
	str.codigo_eqptoRef = eqptoRef->Codigo;

	// Qtde Trafos
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoTRAFO);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoTRAFO);}
	str.qtd_trafos = lisEXT->Count;

	// Qtde chaves
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoCHAVE);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoCHAVE);}
	str.qtd_chaves = lisEXT->Count;

	// Qtde Cargas
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoCARGA);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoCARGA);}
	str.qtd_cargas = lisEXT->Count;

	// Qtde Consumidores
	for(int nc = 0; nc < lisEXT->Count; nc++)
	{
		total_carga = 0;
		carga = (VTCarga*) lisEXT->Items[nc];
		total_carga = carga->NumConsResidencial + carga->NumConsComercial +
			carga->NumConsIndustrial + carga->NumConsRural + carga->NumConsOutros
			 +  carga->NumConsA4;
		str.qtd_consumidores += total_carga;
	}

	// Qtde Capacitores shunt
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoCAPACITOR);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoCAPACITOR);}
	str.qtd_capacitores_shunt = lisEXT->Count;

	// Qtde Capacitores serie
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoCAPSERIE);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoCAPSERIE);}
	str.qtd_capacitores_serie = lisEXT->Count;

	// Qtde Reguladores
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoREGULADOR);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoREGULADOR);}
	str.qtd_reguladores = lisEXT->Count;

	// Qtde Trechos
	lisEXT->Clear();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisEXT, eqptoRef, eqptoTRECHO);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisEXT, eqptoRef, eqptoTRECHO);}
	str.qtd_trechos = lisEXT->Count;

	// Comprimento total
	for(int nc = 0; nc < lisEXT->Count; nc++)
	{
		trecho = (VTTrecho*) lisEXT->Items[nc];
		comprimento_trecho = 0.0;
		//proteção caso o trecho selecionado seja nulo
		if (trecho == NULL)
		{
			continue;
		}
		comprimento_trecho = trecho->Comprimento_km;
		str.comp_trecho_km += comprimento_trecho;
	}
	// Destroi a lista
	DestroiLista(lisEXT);
}

//https://sinapsisenergia.teamwork.com/#/tasks/16586943
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaResumoValores(TStrings *key_value, VTEqpto *eqptoRef)
{
	strpi_RESUMO str;

	AgregaResumo(eqptoRef, str);
	//limpa a lista
	key_value->Clear();
	key_value->Add("Equipamento de Origem=" + str.codigo_eqptoRef);
	key_value->Add("Qtde de Trafos=" + IntToStr(str.qtd_trafos));
	key_value->Add("Qtde de Chaves=" + IntToStr(str.qtd_chaves));
	key_value->Add("Qtde de Cargas=" + IntToStr(str.qtd_cargas));
	key_value->Add("Qtde de Consumidores=" + IntToStr(str.qtd_consumidores));
	key_value->Add("Qtde de Capacitores Shunt=" + IntToStr(str.qtd_capacitores_shunt));
	key_value->Add("Qtde de Capacitores Série=" + IntToStr(str.qtd_capacitores_serie));
	key_value->Add("Qtde de Reguladores de Tensão=" + IntToStr(str.qtd_reguladores));
	key_value->Add("Qtde de Trechos Primários=" + IntToStr(str.qtd_trechos));
	key_value->Add("Comprimento total [km]=" + DoubleToStr("%4.3f",str.comp_trecho_km));
}

// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::AgregaTrafo(VTEqpto *eqptoRef, strpi_TRAFO &str)
{
	TList *lisTRAFO;
	VTTrafo *trafo;

	lisTRAFO = new TList;

	str.snom_mva = 0;
	str.qtde = 0;
	str.trifasico.snom_mva = 0;
	str.trifasico.qtde = 0;
	str.monofasico.snom_mva = 0;
	str.monofasico.qtde = 0;
	//
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisTRAFO, eqptoRef, eqptoTRAFO);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisTRAFO, eqptoRef, eqptoTRAFO);}

	//percorre a lista de cargas e agrega os valores
	for(int nc = 0; nc < lisTRAFO->Count; nc++)
	{
		trafo = (VTTrafo*) lisTRAFO->Items[nc];
		str.snom_mva += trafo->snom;
		str.qtde++;
		if(trafo->Monofasico)
		{
			str.monofasico.snom_mva += trafo->snom;
			str.monofasico.qtde++;
		}
		else
		{
			str.trifasico.snom_mva += trafo->snom;
			str.trifasico.qtde++;
		}
	}
	DestroiLista(lisTRAFO);
//	if(lisTRAFO)
//	{
//		delete lisTRAFO;
//		lisTRAFO = NULL;
//	}

}
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaTrafoValores(TStrings *key_value,
															  VTEqpto *eqptoRef)
{
	strpi_TRAFO str;

	AgregaTrafo(eqptoRef, str);
	//limpa a lista
	key_value->Clear();
	key_value->Add("Potência instalada total [MVA]=" + DoubleToStr("%4.3f",str.snom_mva));
	key_value->Add("Quantidade total=" + IntToStr(str.qtde));
	key_value->Add("Potência instalada trifásica [MVA]=" + DoubleToStr("%4.3f",str.trifasico.snom_mva));
	key_value->Add("Quantidade trifásica=" + IntToStr(str.trifasico.qtde));
	key_value->Add("Potência instalada monofásica [MVA]=" + DoubleToStr("%4.3f",str.monofasico.snom_mva));
	key_value->Add("Quantidade monofásica=" + IntToStr(str.monofasico.qtde));
}

// ----------------------------------------------------------------------------
TList* __fastcall TRelatorioJusante::AgregaTrecho(VTEqpto *eqptoRef)
{
	TList *lisTRECHO;
	TList *lisArranjoTrecho;
	VTTrecho *trecho;
	TArranjoTrecho *arranjoTrecho;
	double comprimentoKm;

	lisTRECHO = new TList();
	lisArranjoTrecho = new TList();
	if(index_tipo_rel ==0)
	{ListaEqptosJusante(lisTRECHO, eqptoRef, eqptoTRECHO);}
	if(index_tipo_rel ==1)
	{ListaEqptosMontante(lisTRECHO, eqptoRef, eqptoTRECHO);}
	// Percorre a lista de trechos
	for(int nc = 0; nc < lisTRECHO->Count; nc++)
	{
		trecho = (VTTrecho*) lisTRECHO->Items[nc];
		//proteção caso o trecho selecionado seja nulo
		if (trecho == NULL)
		{
			continue;
		}
		//passa um trecho e verifica se seu arranjo já está na lista de arranjo-trechos
		arranjoTrecho = ExisteArranjoTrecho(lisArranjoTrecho, trecho);
		// caso seja retornado NULL, então o arranjo não está na lista e será criado um novo arranjo-trecho
		if (arranjoTrecho == NULL)
		{
			//proteções para garantir o sucesso do construtor do arranjoTrecho
			if (trecho->rede == NULL)
			{
				continue;
			}
			if (trecho->rede->TipoRede == NULL)
			{
				continue;
			}
			// criação de um novo arranjo-trecho
			arranjoTrecho = new TArranjoTrecho(apl, trecho);
			// adciona-o para a lista
			lisArranjoTrecho->Add(arranjoTrecho);
		}
		// para esse arranjo-trecho contabiliza-se o comprimento de todos os trechos
		// que são referentes a ele
		comprimentoKm = arranjoTrecho->ComprimentoKm + trecho->Comprimento_km;
		arranjoTrecho->ComprimentoKm = comprimentoKm;
	}

	DestroiLista(lisTRECHO);

	return lisArranjoTrecho;
}

// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaTrechoValores(TStrings *key_value, VTEqpto *eqptoRef)
{
	TArranjoTrecho *arranjoTrecho;
	TList *lisArranjoTrecho;

	// preenche a lista de arranjo-trechos
	lisArranjoTrecho = AgregaTrecho(eqptoRef);

	key_value->Clear();
	for(int i = 0; i < lisArranjoTrecho->Count; i++)
	{
		// pega o i-ésimo arranjo techo
		arranjoTrecho = (TArranjoTrecho*)lisArranjoTrecho->Items[i];
		// adciona o código do arranjo com nível de tensão e seu comprimento ao key_value
		key_value->Add(arranjoTrecho->CodigoArranjo + "= " + DoubleToStr("%4.3f",arranjoTrecho->ComprimentoKm));
	}

	LimpaTList(lisArranjoTrecho);
	DestroiLista(lisArranjoTrecho);
}

//https://sinapsisenergia.teamwork.com/#/tasks/16586943
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::AgregaValores(TStrings *key_value,
								VTEqpto *eqptoRef, int eqptoTIPO, VTPatamar *patamar, int tipo_relatorio)
{
	SelecionaPatamar(patamar);
	index_tipo_rel = tipo_relatorio;
	//age de acordo com o tipo
	switch (eqptoTIPO)
	{
	case 0:
		AgregaResumoValores(key_value, eqptoRef);
		break;

	case eqptoBARRA:

		break;
	case eqptoEQBAR:

		break;
	case eqptoCAPACITOR:

		break;
	case eqptoCAPSERIE:

		break;
	case eqptoCONSUMIDOR:
		AgregaConsumidoresValores(key_value, eqptoRef);
		break;
	case eqptoCARGA:
		AgregaCargaValores(key_value, eqptoRef);
		break;
	case eqptoCHAVE:

		break;
	case eqptoGERADOR:

		break;
	case eqptoFILTRO:

		break;
	case eqptoLIGACAO:

		break;
	case eqptoREATOR:

		break;
	case eqptoREGULADOR:

		break;
	case eqptoSUPRIMENTO:

		break;
	case eqptoTRAFO:
		AgregaTrafoValores(key_value, eqptoRef);
		break;
	case eqptoTRAFO3E:

		break;
	case eqptoTRAFOZZ:

		break;
	case eqptoTRECHO:
		AgregaTrechoValores(key_value, eqptoRef);
		break;
	default:
		;
	}
}

// ----------------------------------------------------------------------------
VTBarra* __fastcall TRelatorioJusante::BarraReferencia(VTEqpto *eqptoRef)
{
	//
	VTBarra *barra = NULL;
	VTEqbar *eqbar = NULL;
	VTLigacao *ligacao = NULL;

	if(eqptoRef->TipoBarra())
	{
		barra = (VTBarra*)eqptoRef;
	}
	else if (eqptoRef->TipoEqbar())
	{
		if((eqptoRef->Tipo() == eqptoSUPRIMENTO) ||
			(eqptoRef->Tipo() == eqptoGERADOR))
		{
        	eqbar = (VTEqbar*)eqptoRef;
			barra = eqbar->pbarra;
		}
		else
		{
        	barra = NULL;
        }

	}
	else if (eqptoRef->TipoLigacao())
	{
		ligacao = (VTLigacao*)eqptoRef;
		//encontra a barra referencia
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			 if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
			 //se a barra ordenada  apontar para a ligacao pai
			 if(barra->Obj == ligacao->ligaPai)
			 {
				break;
			 }
		}
	}

	return barra;
}
// ----------------------------------------------------------------------------
VTChave* __fastcall TRelatorioJusante::ChavePai(VTBloco *bloco)
{	//variáveis locais
	VTBlocos    *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTElo *elo;
	VTNode *nodeFilho, *nodePai = NULL;
	VTBloco *blocoPai;
	VTChave *chave;
	VTChave *chavePai = NULL;
	TList *lisElo = new TList;

	//protecao
	if((blocos == NULL) || (bloco == NULL))
		{return chavePai;}
	//faz se estiver preparada a execução
	if(blocos_executado == false)
		{PreparaExecucao();}
	nodeFilho = ExisteNodeBloco(bloco);
	if (nodeFilho)
	{
		nodePai = nodeFilho->Pai;
	}
	if (nodePai)
	{
		blocoPai = (VTBloco*)nodePai->Object;
		elo = blocos->ExisteElo(bloco,blocoPai);
		if(elo != NULL)
		{
			chave = elo->Chave;
			if(chave != NULL)
			{
				chavePai = chave;
			}
		}
	}
	else
	{
    	elo = blocos->ExisteElo(bloco,NULL);
		if(elo != NULL)
		{
			chave = elo->Chave;
			if(chave != NULL)
			{
				chavePai = chave;
			}
		}
	}

	DestroiTObject(lisElo);
	return  chavePai;
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::DestacaChavesEntreBlocosJusante(VTBloco *bloco)
{	//variáveis locais
	VTBlocos    *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTNode *nodeFilho;
	TList *list;
	VTNode *node;
	VTBloco *bloco2;
	VTElo	*elo;
	VTChave *chave;

	if((blocos == NULL) || (bloco == NULL))
		{return;}
	//teste: destacar as chaves
	node = ExisteNodeBloco(bloco);
	list = node->LisNode();

	//loop p/ todos os Node filho
	for (int n = 0; n < list->Count; n++)
	{
		nodeFilho = (VTNode*)list->Items[n];
		bloco2 = (VTBloco*)nodeFilho->Object;
		elo = blocos->ExisteElo(bloco,bloco2);
		if(elo != NULL)
		{
			chave = elo->Chave;
			if(chave != NULL)
			{
				if(lisDestaqueJusante->IndexOf(chave) < 0)
					{lisDestaqueJusante->Add(chave);}
			}
		}
		//chamada recursiva
		DestacaChavesEntreBlocosJusante(bloco2);
	}

}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::DestacaEquipamentosJusante(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->DestacaEqpto(lisDestaqueJusante, colorDestaque);
	grafico->Refresh();
}

// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::DestacaEquipamentosMontante(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->DestacaEqpto(lisDestaqueMontante, colorDestaque);
	grafico->Refresh();
}

// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::DestroiLista(TList *lista)
{
	if(lista != NULL)
	{
		delete lista;
		lista = NULL;
	}
}
// ----------------------------------------------------------------------------
//double __fastcall TRelatorioJusante::CalculaTrafosJusante(VTEqpto *eqpto)
//{
//	// Redes
//	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
//	VTRadial *radial = DLL_NewObjRadial(apl);
//	radial->Inicia(redes);
//
//	lisLigacaoJusante->Clear();
//	lisPrimario = radial->LisPrimario();
//
//	// Preenche a lista de ligações a jusante de acordo com o tipo de equipamento selecionado.
//	if (eqpto->TipoBarra())
//	{
//		VTBarra *barra = (VTBarra*)eqpto;
//		PreencheListaLigacoesJusanteTipoBarra(barra);
//	}
//	else if (eqpto->TipoEqbar())
//	{
//		VTEqbar *eqBar = (VTEqbar*)eqpto;
//		PreencheListaLigacoesJusanteTipoBarra(eqBar->pbarra);
//	}
//	else if (eqpto->TipoLigacao())
//	{
//		VTLigacao *ligacao = (VTLigacao*)eqpto;
//		PreencheListaLigacoesJusanteTipoLigacao(ligacao);
//	}
//
//	try
//	{
//		return SomaRelatorioJusante();
//	}
//	catch (Exception &e)
//	{
//		Aviso("Não foi possível realizar a soma da Potência Nominal dos Transformadores.");
//		return 0.0;
//	}
//
//}

// ----------------------------------------------------------------------------
TArranjoTrecho* __fastcall TRelatorioJusante::ExisteArranjoTrecho(TList *lisArranjoTrecho, VTTrecho *trecho)
{
	TArranjoTrecho *arranjoTrecho = NULL;

	try
	{
		// percorre a lista de arranjo-trechos
		for(int i = 0; i < lisArranjoTrecho->Count; i++)
		{
			// pega o i-ésimo arranjo-trecho
			arranjoTrecho = (TArranjoTrecho*)lisArranjoTrecho->Items[i];
			// verifica se arranjo do trecho passado ja está na lista de arranjo-trechos
			if (arranjoTrecho->Arranjo == trecho->arranjo)
			{
				// caso já exista, verifica se o nível de tensão é o mesmo
				if(arranjoTrecho->Segmento == trecho->rede->TipoRede->Segmento)
				{
					// se o nível de tensao for o mesmo, retorna o arranjo-trecho
					return arranjoTrecho;
				}
			}
		}
		//caso ainda não esteja na lista, retorna NULL para que seja criado um novo arranjo-trecho
		arranjoTrecho = NULL;
		return arranjoTrecho;
	}
	catch(Exception &e)
	{
		//caso dê algo errado, retorna NULL para que seja criado um novo arranjo-trecho
		arranjoTrecho = NULL;
		return arranjoTrecho;
	}

}

// ----------------------------------------------------------------------------
VTNode* __fastcall TRelatorioJusante::ExisteNodeBloco(VTBloco *bloco)
{
	VTArvore    *arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
	TList       *listAUX;
	VTNode 		*node;

	if((arvore == NULL) || (bloco == NULL))
		{return NULL;}
	listAUX = new TList;
	arvore->LisAllNode(listAUX);
	for (int nn = 0; nn < listAUX->Count; nn++)
	{
		node = (VTNode*) listAUX->Items[nn];
		if(node->Object == bloco)
		{
			break;
		}
		else
		{
			node = NULL;
		}
	}
	//destroi lista
	DestroiLista(listAUX);
//	if(listAUX)
//	{
//		delete listAUX;
//		listAUX = NULL;
//	}

	return node;
}
// ----------------------------------------------------------------------------
VTNode* __fastcall TRelatorioJusante::ExisteNodeEqpto(VTEqpto *eqpto)
{
	VTBlocos    *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBloco		*blocoCHV_filho;
	VTNode 		*node = NULL;
	VTLigacao	*ligacao;
	VTBarra		*barraAUX;
	VTEqbar		*eqbar;
	VTChave     *chave;
	VTElo		*eloAUX;
	VTNode		*nodeCHV1, *nodeCHV2, *nodeTemp;

	//proteção contra ponteiros nulos
	if((eqpto == NULL) || (blocos==NULL))
		{return NULL;}
	//Age de acordo com o tipo do equipamento
	if(eqpto->TipoLigacao())
	{
		if(eqpto->Tipo() == eqptoCHAVE)
		{	//caso seja uma chave, a chave não pertence a nenhum bloco
			//é preciso verificar os Elos
			chave = (VTChave*)eqpto;
			if(chave->Estado == chvFECHADA)
			{//caso a chave esteja fechada é preciso ver os blocos que ela liga
				eloAUX = blocos->ExisteElo(chave);
				nodeCHV1 = ExisteNodeBloco(eloAUX->Bloco1);
				nodeCHV2 = ExisteNodeBloco(eloAUX->Bloco2);
				//verifica qual é o bloco pai
				if(nodeCHV1->Pai == nodeCHV2)
				{
					blocoCHV_filho = eloAUX->Bloco1;
					nodeTemp = nodeCHV1;
				}
				else
				{
					blocoCHV_filho = eloAUX->Bloco2;
					nodeTemp = nodeCHV2;
				}
				//perrcorre as barras da chave
				for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
				{
					barraAUX = chave->Barra(ind_bar);
					//verifica se existe a barra no bloco filho
					if(blocoCHV_filho->LisBarra()->IndexOf(barraAUX)>= 0)
					{   //caso a barra pertença ao bloco filho, o node filho
						//é o node procurado
						node = nodeTemp;
						break;
					}
				}
			}
		}
		else
		{//se nao for chave, ai fica simples.
			ligacao = (VTLigacao*)eqpto;
			node = arvore_lig->ExisteNode(ligacao);
		}
	}
	else if(eqpto->TipoBarra())
	{ //caso seja barra, verifica qual node da arvore de barras é
		barraAUX = (VTBarra*)eqpto;
		node = arvore_bar->ExisteNode(barraAUX);
	}
	else if(eqpto->TipoEqbar())
	{ //caso seja eqbar, verifica qual node da arvore de barras tem a barra dele
		eqbar = (VTEqbar*)eqpto;
		barraAUX = eqbar->pbarra;
		node = arvore_bar->ExisteNode(barraAUX);
	}
	return node;
}
// ----------------------------------------------------------------------------
int __fastcall TRelatorioJusante::IniciaArvoreBlocos(void)
{
	//variáveis locais
   //bool         sucesso = true;
   VTArvore    *arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTBloco     *bloco_jus, *bloco_mon;
   VTElo       *elo,       *elo_pai;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTNode      *node;
   TList       *lisNODE;

   if(blocos_executado){return(1);}
   //inicia arvore
   if(arvore) {apl->Remove(arvore); delete arvore;}
   apl->Add(arvore = DLL_NewObjArvore(apl));
   //proteção
   if((! arvore) || (! blocos))
		{return 2;}
   if(! blocos->Executa(redes))
		{return 3;}
   //elimina Elos cujas Chaves estão abertas
   if (! blocos->EliminaElosComChaveAberta())
		{return 4;}
   //elimina Blocos isolados ou conectados apenas por Chaves abertas
   if (! blocos->EliminaBlocosNaoEnergizados())
		{return 5;}
   //elimina Elos que conectam um mesmo Bloco
   if (! blocos->EliminaElosComBlocosIguais())
		{return 6;}
   if(! arvore->Inicia(blocos))
   		{return 7;}

   blocos_executado = true;
   return(1);

}

// ----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17283172
void __fastcall TRelatorioJusante::InsereGD(void)
{
    // variaveis locais
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

    if(existe_gd)
    {// desfaz ultima acao, que foi remover todas as GDs
        edita->ActionUndo->Execute();
    }
}

// ----------------------------------------------------------------------------
VTLigacao* __fastcall TRelatorioJusante::LigacaoReferencia(VTEqpto *eqptoRef)
{
	VTBarra *barra = NULL;
	VTEqbar *eqbar = NULL;
	VTLigacao *ligacao = NULL;
	VTLigacao *ligPai = NULL;

	if(eqptoRef->TipoBarra())
	{
		barra = (VTBarra*) eqptoRef;
		ligPai = (VTLigacao*) barra->Obj;
	}
	else if (eqptoRef->TipoEqbar())
	{
        if((eqptoRef->Tipo() == eqptoSUPRIMENTO) ||
			(eqptoRef->Tipo() == eqptoGERADOR))
		{
			eqbar = (VTEqbar*)eqptoRef;
			barra = eqbar->pbarra;
			ligPai = (VTLigacao*) barra->Obj;
        }
	}
	else if (eqptoRef->TipoLigacao())
	{
		ligacao = (VTLigacao*)eqptoRef;
		ligPai = ligacao;
	}

	return ligPai;
}

// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaBlocosJusante(VTBloco *bloco, TList *lisEXT)
{
	VTArvore    *arvore;
	TList *listAUX;
	VTNode *nodeAUX;
	VTNode *node;

	//verifica se ja foi executado
	//faz se estiver preparada a execução
	if(blocos_executado == false)
		{PreparaExecucao();}
	arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
	//verifica qual node corresponde ao bloco
	node = arvore->ExisteNode(bloco);
	listAUX = new TList;
	if(node)
	{
		node->LisNodeJusante(listAUX);
		for(int nn = 0; nn < listAUX->Count; nn++)
		{
			nodeAUX = (VTNode*)listAUX->Items[nn];
			lisEXT->Add(nodeAUX->Object);
		}
	}
	DestroiLista(listAUX);
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaBlocosJusante(VTNode *node, TList *lisEXT)
{
	TList *listAUX;
	VTNode *nodeAUX;

	listAUX = new TList;
	if(node)
	{
		node->LisNodeJusante(listAUX);
		for(int nn = 0; nn < listAUX->Count; nn++)
		{
			nodeAUX = (VTNode*)listAUX->Items[nn];
			lisEXT->Add(nodeAUX->Object);
		}
	}
	DestroiLista(listAUX);
//	if(listAUX)
//	{
//		delete listAUX;
//		listAUX = NULL;
//	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaBlocosJusante(TList *lisELOConsiderados,
												TList *lisEXT, VTBloco *bloco_ref)
{
	VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	TList 		*listBLOCOAUX;
	TList 		*listNODEAUX;
	TList		*lisAUX;
	VTNode 		*nodeAUX, *nodePAI;
	VTBloco		*blocoAUX;
	VTElo  		*elo;


	//protecao
	if(bloco_ref == NULL)
		{return;}
	//cria uma lista para os blocos filhos desses elos
	listBLOCOAUX = new TList;
	listNODEAUX = new TList;
	lisAUX = new TList;
	//percorre os elos e lista os outros blocos
	for(int ne = 0; ne < lisELOConsiderados->Count; ne++)
	{
		elo = (VTElo*) lisELOConsiderados->Items[ne];
		if(elo->Bloco1 != bloco_ref)
		{
			listBLOCOAUX->Add(elo->Bloco1);
		}
		else if(elo->Bloco2 != bloco_ref)
		{
			listBLOCOAUX->Add(elo->Bloco2);
		}
	}
	//descobre os nodes dos blocos
	for(int nb = 0; nb < listBLOCOAUX->Count; nb++)
	{
		blocoAUX = (VTBloco*)listBLOCOAUX->Items[nb];
		nodeAUX = ExisteNodeBloco(blocoAUX);
		if(nodeAUX)
		{
			listNODEAUX->Add(nodeAUX);
        }
	}
	//percorre a lista de nodes e lista os blocos

	for(int nn = 0; nn < listNODEAUX->Count; nn++)
	{
		lisAUX->Clear();
		nodePAI = (VTNode*)listNODEAUX->Items[nn];
		//pega os nodes a jusante
		//teste
		int a = nodePAI->LisNode()->Count;
		//nodePAI->LisNodeJusante(lisAUX);
		ListaNodesJusante(nodePAI,lisAUX);
		if(lisEXT->IndexOf(nodePAI->Object) < 0)
			{lisEXT->Add(nodePAI->Object);}
		for(int nna = 0; nna < lisAUX->Count; nna++)
		{
			nodeAUX = (VTNode*)lisAUX->Items[nna];
			//se o node nao apontar pro bloco referencia
			if(nodeAUX->Object != bloco_ref)
			{   //e o bloco já nao estiver na lista
				if(lisEXT->IndexOf(nodeAUX->Object) < 0)
					{lisEXT->Add(nodeAUX->Object);}
			}
		}
	}
	DestroiLista(lisAUX);
	DestroiLista(listBLOCOAUX);
	DestroiLista(listNODEAUX);
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaEqbarExistentes(TList *lisBAR, TList *lisEXT, int eqptoTIPO)
{
	VTEqbar *eqbar;
	VTBarra	*barra;

	//protecao
	if((lisBAR == NULL) || (lisEXT == NULL))
		{return;}
	//percore a lsita de ligacoes e guarda na lista as do tipo especificado
	for(int nb = 0; nb <  lisBAR->Count; nb++)
	{
		barra = (VTBarra*) lisBAR->Items[nb];
		barra->LisEqbar(lisEXT,eqptoTIPO);
	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaEqptosJusante(TList *lisEXT,
						                       VTEqpto *eqptoRef, int eqptoTIPO)
{
	VTArvore    *arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBloco 	*bloco;
	VTBloco 	*blocoAUX;
	VTBloco 	*bloco1, *bloco2;
	VTNode 		*node, *node_aux ,*node_pai;
	VTChave		*chaveAUX;
	VTBarra		*barraAUX;
	VTEqbar		*eqbarAUX;
	VTLigacao	*ligacaoAUX;
	VTElo		*eloAUX;
	int			tipoEqptoRef;
	//int			indexEqpto;
	TList 		*lisORD_BAR;
	TList 		*lisORD_LIG;
	TList		*lisEloFront;
	TList		*lisBLOCOS;
	TList 		*listAUX;
	TList 		*lisEloFilhos;
	//TList *lisDestaque;
	isBarra = isEqbar = isLigacao = false;
	VTNode *nodeCHV1, *nodeCHV2;
	VTBloco *blocoCHV_aux, *blocoCHV_filho;


	//seleciona eqpto
	SelecionaEqpto(eqptoRef);
	//faz se estiver preparada a execução
	if(blocos_executado == false)
		{PreparaExecucao();}
	//marca qual tipo é
	//age de acordo com o tipo
	TipoEquipamento(eqptoTIPO);
	//protecao
	if(eqptoRef == NULL) return;
	if(blocos == NULL) return;
	//cria lista de blocos
	lisBLOCOS = new TList;
	listAUX = new TList;
	lisORD_BAR = new TList;
	lisORD_LIG = new TList;
	lisEloFront = new TList;
	lisEloFilhos = new TList;
	//limpa a lista de destaque
	lisDestaqueJusante->Clear();
	//limpa lista
	lisEXT->Clear();
	//separa de acordo com tipo do vtepqto
	if(eqptoRef->TipoBarra()  == true)
	{
		bloco = blocos->ExisteBloco((VTBarra*)eqptoRef);
	}
	else if (eqptoRef->TipoLigacao() == true)
	{  	//se ligacao for chave fechada...
		if(eqptoRef->Tipo() == eqptoCHAVE)
		{
			chaveAUX = (VTChave*)eqptoRef;
			if(chaveAUX->Estado == chvFECHADA)
			{//caso a chave esteja fechada é preciso ver os blocos que ela liga
				eloAUX = blocos->ExisteElo(chaveAUX);
				nodeCHV1 = ExisteNodeBloco(eloAUX->Bloco1);
				nodeCHV2 = ExisteNodeBloco(eloAUX->Bloco2);
				//verifica qual é o bloco pai
				if(nodeCHV1->Pai == nodeCHV2)
					{blocoCHV_filho = eloAUX->Bloco1;}
				else
					{blocoCHV_filho = eloAUX->Bloco2;}
				//perrcorre as barras da chave
				for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
				{
					barraAUX = chaveAUX->Barra(ind_bar);
					//verifica se existe a barra no bloco filho
					if(blocoCHV_filho->LisBarra()->IndexOf(barraAUX)>= 0)
					{
						bloco = blocoCHV_filho;
						lisDestaqueJusante->Add(chaveAUX);
						break;
					}
				}
			}
		}
		else
		{
			bloco = blocos->ExisteBloco((VTLigacao*)eqptoRef);
		}
	}
	else if (eqptoRef->TipoEqbar() == true)
	{
        if((eqptoRef->Tipo() == eqptoSUPRIMENTO) ||
			(eqptoRef->Tipo() == eqptoGERADOR))
		{
			eqbarAUX = (VTEqbar*) eqptoRef;
			bloco = blocos->ExisteBloco(eqbarAUX->pbarra);
			eqbarAUX = NULL;
		}
		else
		{
			bloco = NULL;
        }
	}
	//agrega todos blocos à jusante
	if(bloco)
	{
		if(arvore == NULL) return;

		//verifica qual node tem o bloco
		node = ExisteNodeBloco(bloco);
		if(node != NULL)
		{
			node_pai = node->Pai;
		}
		//ordena as ligaoces dentro do bloco selecionado
		OrdenaLigBloco(bloco, lisORD_BAR, lisORD_LIG);
		//remove os eqptos das listas ordenadas que não devem ser analisados
		RemoveEqptosNaoJusante(eqptoRef,lisORD_BAR,lisORD_LIG);
		//lista os eqptos dentro do bloco do eqpto atual de acordo com o tipo
		if(isBarra == true)
			{CopiaTList(lisORD_BAR,lisEXT);}
		else if( isEqbar == true)
			{ListaEqbarExistentes(lisORD_BAR,lisEXT,eqptoTIPO);}
		else if (isLigacao == true)
			{ListaLigacaoExistentes(lisORD_LIG,lisEXT,eqptoTIPO);}
		//apos ordenar a lista de ligacoes, verificar quais elos devem
		//ser considerados para ver os blocos filhos
		blocos->LisElo(bloco, lisEloFront);
		//percorre a lista das chaves e verifica se pelo menos uma das barras
		//pertence as barras ordenadas
		for(int nef = 0; nef < lisEloFront->Count; nef++)
		{
			eloAUX = (VTElo*) lisEloFront->Items[nef];
			chaveAUX = eloAUX->Chave;
			barraAUX = chaveAUX->pbarra1;
			//verifica se esta olhando bloco pai
			bloco1 = eloAUX->Bloco1;
            bloco2 = eloAUX->Bloco2;
			//verifica se o bloco selecionado eh o 1 ou o 2 e verifica se outro eh pai
			node_aux = NULL;
			if(bloco == bloco1)
			{
				node_aux = ExisteNodeBloco(bloco2);
				if(node_aux == node_pai)
				{
					continue;
				}
			}
			else if (bloco == bloco2)
			{
				node_aux = ExisteNodeBloco(bloco1);
				if(node_aux == node_pai)
				{
					continue;
				}
			}
			//verifica barra 1
			if(lisORD_BAR->IndexOf(barraAUX) >= 0)
			{	//se a barra foi encontrada nas barras ordenadas
				//osfilhos desse elo estão à jusante
				lisEloFilhos->Add(eloAUX);
				//destaca a chave
				lisDestaqueJusante->Add(chaveAUX);
			}
			else
			{
				barraAUX = chaveAUX->pbarra2;
				if(lisORD_BAR->IndexOf(barraAUX) >= 0)
				{	//se a barra foi encontrada nas barras ordenadas
					//osfilhos desse elo estão à jusante
					lisEloFilhos->Add(eloAUX);
					//destaca a chave
					lisDestaqueJusante->Add(chaveAUX);
				}
			}
		}
		//com os elos cujos filhos devem ser considerados..
		ListaBlocosJusante(lisEloFilhos,lisBLOCOS,bloco);
		//destaca as chaves entre blocos a jusante
		for(int nb = 0; nb < lisBLOCOS->Count; nb++)
		{
			blocoAUX = (VTBloco*) lisBLOCOS->Items[nb];
			DestacaChavesEntreBlocosJusante(blocoAUX);
        }
	}
	//percorre todos blocos jusante e lista os equiptos dependendo do tipo
	for (int nb = 0; nb < lisBLOCOS->Count; nb++)
	{
		bloco = (VTBloco*) lisBLOCOS->Items[nb];
		//destaca as barras e ligacoes
		CopiaTList(bloco->LisBarra(),lisDestaqueJusante);
		CopiaTList(bloco->LisLigacao(),lisDestaqueJusante);
		//age de acordo com o tipo
		if(isBarra == true)
		{
			listAUX = bloco->LisBarra();
			CopiaTList(listAUX, lisEXT);
		}
		else if( isEqbar == true)
		{
			bloco->LisEqbar(lisEXT,eqptoTIPO);
			CopiaTList(lisEXT,lisDestaqueJusante);
		}
		else if (isLigacao == true)
		{
			bloco->LisLigacao(lisEXT,eqptoTIPO);
			CopiaTList(lisEXT,lisDestaqueJusante);
		}
	}

	//Destaca os equiptos selecionados
	CopiaTList(lisORD_BAR,lisDestaqueJusante);
	CopiaTList(lisORD_LIG,lisDestaqueJusante);
	CopiaTList(lisEXT,lisDestaqueJusante);
	//deleta a lista criada
	DestroiLista(lisBLOCOS);
	DestroiLista(listAUX);
	DestroiLista(lisORD_BAR);
	DestroiLista(lisORD_LIG);
	DestroiLista(lisEloFront);
	DestroiLista(lisEloFilhos);
}

// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaEqptosMontante(TList *lisEXT,
											   VTEqpto *eqptoRef, int eqptoTIPO)
{
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList *lisEQPTOinicial = new TList();
   TList *lisLigacoes = new TList();
   TList *lisSuprimentos = new TList();
   TList *lisLIG = new TList();
   lisLigacaoMontante->Clear();


   //seleciona eqpto
   SelecionaEqpto(eqptoRef);

   //preenche as listas criadas para executar o caminho
   lisEQPTOinicial->Add(eqpto);
   redes->LisLigacao(lisLigacoes);
   redes->LisEqpto(lisSuprimentos, eqptoSUPRIMENTO);

   //limpa a lista de ligações e executa o caminho
   lisDestaqueMontante->Clear();
   lisEXT->Clear();
   caminho->Executa(lisEQPTOinicial, lisSuprimentos, lisLigacoes, lisLIG);
   ListaLigacaoExistentes(lisLIG, lisLigacaoMontante, eqptoTIPO);
   CopiaTList(lisLigacaoMontante,lisEXT);
   for (int nl = 0; nl < lisLIG->Count; nl++)
   {
	   VTLigacao *ligacao = (VTLigacao*)lisLIG->Items[nl];
	   if(ligacao== NULL){continue;}
	   if(lisDestaqueMontante->IndexOf(ligacao) < 0)
					{lisDestaqueMontante->Add(ligacao);}
       // Inclui as barras das ligações
	   if((ligacao->pbarra1 != NULL) && (lisDestaqueMontante->IndexOf(ligacao->pbarra1) < 0))
					{lisDestaqueMontante->Add(ligacao->pbarra1);}
	   if((ligacao->pbarra2 != NULL) && (lisDestaqueMontante->IndexOf(ligacao->pbarra2) < 0))
					{lisDestaqueMontante->Add(ligacao->pbarra2);}
	   if((ligacao->pbarra3 != NULL) && (lisDestaqueMontante->IndexOf(ligacao->pbarra3) < 0))
					{lisDestaqueMontante->Add(ligacao->pbarra3);}

   }
   //deleta as listas criadas
   if (lisEQPTOinicial) {delete lisEQPTOinicial; lisEQPTOinicial = NULL;}
   if (lisLigacoes) {delete lisLigacoes; lisLigacoes = NULL;}
   if (lisSuprimentos) {delete lisSuprimentos; lisSuprimentos = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
}

// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaLigacaoExistentes(TList *lisLIG, TList *lisEXT, int eqptoTIPO)
{
	VTLigacao *lig;

	//protecao
	if((lisLIG == NULL) || (lisEXT == NULL))
		{return;}
	//percore a lsita de ligacoes e guarda na lista as do tipo especificado
	for(int nl = 0; nl <  lisLIG->Count; nl++)
	{
		lig = (VTLigacao*) lisLIG->Items[nl];
		if(lig->Tipo() == eqptoTIPO)
		{
        	lisEXT->Add(lig);
        }
    }
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::ListaNodesJusante(VTNode *node, TList *lisEXT)
{	//variáveis locais
	VTNode *nodeFilho;
	TList *list;
	//teste: destacar as chaves


	list = node->LisNode();
	//loop p/ todos os Node filho
	for (int n = 0; n < list->Count; n++)
	{
		nodeFilho = (VTNode*)list->Items[n];
		if(lisEXT->IndexOf(nodeFilho) < 0)
			{lisEXT->Add(nodeFilho);}
		//chamada recursiva
		ListaNodesJusante(nodeFilho, lisEXT);
	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::Ordena(VTBarra *barra_ini,TList *lisLIG,
											TList *lisEXT_BAR, TList *lisEXT_LIG)
{	//variáveis locais
   VTBarra   *barra;
   VTChave   *chave;
   VTLigacao *ligacao_filha, *ligacao_pai;
   TList 	  *lisEQP = lisLIG;
   VTNode	  *node_lig_pai, *node_bar_pai;
   VTNode	  *node_lig_filho, *node_bar_filho;
   //determina Barra inicial
   if ((barra = barra_ini) == NULL)
	  {return;}

   //limpa as arvores
   arvore_lig->Clear();
   arvore_bar->Clear();
   //inicia lisEXT_BAR com a Barra inicial
   lisEXT_BAR->Clear();
   lisEXT_BAR->Add(barra);
   node_bar_pai = arvore_bar->InsereNode(NULL);
   node_bar_pai->InsereObject(barra);
   //define a Barra iniciou sem Ligacao pai que a conectou
   barra->Obj = NULL;
   //inicia lisEXT_LIG com as Ligacoes que conectam a Barra inicial
   lisEXT_LIG->Clear();
   //percorre todas ligacoes
   for (int nl = lisEQP->Count-1; nl >= 0; nl--)
	  {
      ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
      //verifica se é uma Chave aberta
      if (ligacao_filha->Tipo() == eqptoCHAVE)
         {
         chave = (VTChave*)ligacao_filha;
		 if (chave->Aberta)
            {//retira Ligacao de lisEQP
            lisEQP->Delete(nl);
            continue;
            }
         }
      //verifica se a Ligacao conecta a Barra
	  if (ligacao_filha->ConectaBarra(barra))
         {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
         lisEQP->Delete(nl);
		 lisEXT_LIG->Add(ligacao_filha);
         //define Ligacao pai como nula
		 ligacao_filha->ligaPai = NULL;
		 //cria o node da ligacao
		 node_lig_filho = arvore_lig->InsereNode(NULL);
		 node_lig_filho->InsereObject(ligacao_filha);
         }
	  }
   //loop p/ todas Ligacoes pai que estão em lisEXT_LIG
   for (int nlp = 0; nlp < lisEXT_LIG->Count; nlp++)
      {
	  ligacao_pai = (VTLigacao*)lisEXT_LIG->Items[nlp];
	  //insere o node pai
	  node_lig_pai = arvore_lig->ExisteNode(ligacao_pai);
	  //verifica qual barra ja foi analisada
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	  {
		 if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
		 //descarta Barras já analisadas
		 if (lisEXT_BAR->IndexOf(barra) >= 0)
		 {
			node_bar_pai = arvore_bar->ExisteNode(barra);
			continue;
		 }
	  }
	  //loop p/ todas Barras da Ligacao pai
	  for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	  {
		 if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
		 //descarta Barras já analisadas
		 if (lisEXT_BAR->IndexOf(barra) >= 0)
		 	{continue;}
		 AnsiString nome_barra_pai, nome_barra_filha;
		 nome_barra_pai =((VTBarra*)(node_bar_pai->LisObjectNode()->First()))->Codigo;
		 node_bar_filho = arvore_bar->InsereNode(node_bar_pai);
		 node_bar_filho->InsereObject(barra);
		 nome_barra_filha =((VTBarra*)(node_bar_filho->LisObjectNode()->First()))->Codigo;
		 //insere Barra em lisEXT_BAR
		 lisEXT_BAR->Add(barra);
         //salva em Barra a Ligacao pai que a conectou
         barra->Obj = ligacao_pai;
         //determina Ligacoes em lisEQP que se conectam à Barra
		 for (int nl = lisEQP->Count-1; nl >= 0; nl--)
		 {//OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
			ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
            //verifica se a Ligacao conecta a Barra
            if (ligacao_filha->ConectaBarra(barra))
               {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
               lisEQP->Delete(nl);
               lisEXT_LIG->Add(ligacao_filha);
			   //associa Ligacao filha c/ Ligacao Pai
			   ligacao_filha->ligaPai = ligacao_pai;
			   //cria o node da ligacao
			   node_lig_filho = arvore_lig->InsereNode(node_lig_pai);
			   node_lig_filho->InsereObject(ligacao_filha);
               }
            }
         }
      }
   //limpa listas locais
   lisEQP->Clear();
}
// ----------------------------------------------------------------------------
void __fastcall TRelatorioJusante::OrdenaLigBloco(VTBloco *bloco, TList* lisEXT_BAR,
															  TList *lisEXT_LIG)
{
	VTArvore    *arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTNode 		*node, *nodeAUX;
	VTElo 		*elo;
	VTBloco		*bloco_pai;
	VTChave 	*chave;
	VTBarra		*b1, *b2;
	VTBarra  	*barra_ini = NULL;
	VTBarra  	*barra_aux = NULL;
	//VTBarra 	*barra_aux;
	VTLigacao	*ligacao_aux;
	VTLigacao   *ligacao_pai;
	VTEqbar		*eqbar_aux;
	//TList		*lisLig, *lisBar;
	TList		*lisLIGBLOCO;
	TList		*lisLIGBAR;
	TList		*lisEQBAR;
	try
	{
		//lisLig = new TList;
		//lisBar = new TList;
		lisEXT_LIG->Clear();
		lisEXT_BAR->Clear();
		lisLIGBLOCO = new TList;
		//proteção
		if((arvore == NULL) || (blocos == NULL) || (bloco == NULL))
			{throw new Exception("(arvore == NULL) || (blocos == NULL)|| (bloco == NULL)");}
		//descobre o node que esta o bloco
		node = ExisteNodeBloco(bloco);
		//verifica o bloco pai
		if(node)
		{
			nodeAUX = node->Pai;
			if(nodeAUX)
			{
				bloco_pai = (VTBloco*)nodeAUX->Object;
				//caso tenha bloco pai
				if(bloco_pai != NULL)
				{
					//descobre o elo
					elo = blocos->ExisteElo(bloco_pai, bloco);
					if(elo)
					{
						chave = elo->Chave;
						//verifica qual é a barra inicial do bloco
						if(chave)
						{
							b1 = chave->pbarra1;
							b2 = chave->pbarra2;
							if(bloco->ExisteBarra(b1))
								{barra_ini = b1;}
							if(bloco->ExisteBarra(b2))
								{barra_ini = b2;}
						}
						else {throw new Exception("Não existe elo chave no elo encontrado.");}
					}
					else {throw new Exception("Não existe elo entre o bloco e o bloco pai.");}
				}
				else {throw new Exception("Não existe bloco pai apesar de existir node pai.");}
			}
			else
			{	//caso não possua node pai
				bloco_pai = NULL;
				//percorre todas as barras a procura de um suprimento
				lisLIGBAR = bloco->LisBarra();
				lisEQBAR = new TList;
				for(int nb = 0; nb < lisLIGBAR->Count; nb++)
				{
					lisEQBAR->Clear();
					barra_aux = (VTBarra*) lisLIGBAR->Items[nb];
					barra_aux->LisEqbar(lisEQBAR,eqptoSUPRIMENTO);
					if(lisEQBAR->Count > 0)
					{
						eqbar_aux = (VTEqbar*) lisEQBAR->First();
						barra_ini = eqbar_aux->pbarra;
						break;
					}
				}
				DestroiLista(lisEQBAR);
				if(barra_ini == NULL)
				{
					//analisa o equipamento selecionado e escolhe uma barra pra ser inicial
					if(eqpto != NULL)
					{
						if(eqpto->TipoBarra())
							{barra_ini = (VTBarra*) eqpto;}
						else if(eqpto->TipoLigacao())
						{
							ligacao_aux = (VTLigacao*) eqpto;
							//teste
							//
							barra_ini = ligacao_aux->Barra(0);
						}
						else if(eqpto->TipoEqbar())
						{
							eqbar_aux = (VTEqbar*) eqpto;
							//teste
							barra_ini = eqbar_aux->pbarra;
						}
						else {throw new Exception("Eqpto selecionado de tipo indefinido.");}
					}
					else {throw new Exception("Eqpto não selecionado.");}
				}
			}
		}
		else {throw new Exception("Não existe node correspondente ao bloco.");}
		if(barra_ini != NULL)
		{
			CopiaTList(bloco->LisLigacao(),lisLIGBLOCO);
			//ordena as ligacoes à partir da barra inicial defininida
			Ordena(barra_ini,lisLIGBLOCO,lisEXT_BAR,lisEXT_LIG);
		}
	}
	catch (Exception &e)
	{
		//nao sei haha
	}
	DestroiLista(lisLIGBLOCO);
}

// ----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17283172
void __fastcall TRelatorioJusante::PreparaExecucao(void)
{
    //remove GDs
    RemoveGD();
    IniciaArvoreBlocos();
    //insere GDs
    InsereGD();
}
// ----------------------------------------------------------------------------
//void __fastcall TRelatorioJusante::PreencheListaLigacoesJusanteTipoLigacao(VTLigacao *ligacao)
//{
//	// Variáveis locais
//	VTLigacao *ligacaoSelecionada = ligacao;
//	VTPrimario *primario;
//
//	// Encontra o primário referente ao Equipamento, no caso a Ligação
//	for (int i = 0; i < lisPrimario->Count; i++)
//	{
//		VTPrimario *primarioLista = (VTPrimario*)lisPrimario->Items[i];
//		if (primarioLista->Rede == ligacao->rede)
//		{
//			primario = primarioLista;
//			break;
//		}
//	}
//
//	if(!primario)
//	{
//    	throw new Exception("Não foi possível encontrar o primário do equipamento seleiconado");
//	}
//
//	// Percorre a lisLigacao procurando o equipamento selecionado.
//	primario->Ordena(lisBarra, lisLigacao);
//
//	lisLigacaoJusante->Add(ligacaoSelecionada);
//
//	// percorrer lisLigacoes em Loop reverso, para ver as ligacões filhas dessa ligação base.
//	for (int j = 0; j < lisLigacaoJusante->Count; j++)
//	{
//		VTLigacao *ligacaoReferencia = (VTLigacao*)lisLigacaoJusante->Items[j];
//		for (int i = (lisLigacao->Count - 1); i > 0; i--)
//		{
//			VTLigacao *ligacaoOficial = (VTLigacao*)lisLigacao->Items[i];
//			if (ligacaoOficial->ligaPai != NULL && ligacaoOficial->ligaPai == ligacaoReferencia)
//			{
//				lisLigacaoJusante->Add(ligacaoOficial);
//			}
//		}
//	}
//
//	// Continua a pegar as ligações, mas dessa vez de cada um dos secundários do Primario já definido.
//	for (int i = 0; i < primario->LisSecundario()->Count; i++)
//	{
//		VTSecundario *secundario = (VTSecundario*)primario->LisSecundario()->Items[i];
//		AdicionaListaLigacoesJusanteSecundario(secundario);
//	}
//
//}

// ----------------------------------------------------------------------------
//void __fastcall TRelatorioJusante::PreencheListaLigacoesJusanteTipoBarra(VTBarra *barra)
//{
//	// Variáveis locais
//	VTBarra *barraSelecionada = barra;
//	VTPrimario *primario;
//	VTLigacao *ligacaoReferencia;
//
//	// Encontra o primário referente ao Equipamento, no caso a Barra
//	for (int i = 0; i < lisPrimario->Count; i++)
//	{
//		VTPrimario *primarioLista = (VTPrimario*)lisPrimario->Items[i];
//		VTRede *rede = primarioLista->Rede;
//		if (rede->ExisteBarra(barraSelecionada))
//		{
//			primario = primarioLista;
//		}
//	}
//
//	if(!primario)
//	{
//    	throw new Exception("Não foi possível encontrar o primário do equipamento seleiconado");
//	}
//
//	// Percorre a lisLigacao procurando o equipamento selecionado.
//	primario->Ordena(lisBarra, lisLigacao);
//
//	// Encontra a ligacao da barra selecionada
//	for (int i = 0; i < lisLigacao->Count; i++)
//	{
//		VTLigacao *ligacaoOficial = (VTLigacao*)lisLigacao->Items[i];
//		if ((ligacaoOficial->pbarra1 == barraSelecionada) ||
//			(ligacaoOficial->pbarra2 == barraSelecionada) ||
//			(ligacaoOficial->pbarra3 == barraSelecionada))
//		{
//			ligacaoReferencia = ligacaoOficial;
//			break;
//		}
//	}
//
//	if(ligacaoReferencia)
//	{
//		lisLigacaoJusante->Add(ligacaoReferencia);
//	}
//
//	// percorrer lisLigacoes em Loop reverso, para ver as ligacões filhas dessa ligação base.
//	for (int j = 0; j < lisLigacaoJusante->Count; j++)
//	{
//		VTLigacao *ligacaoReferencia = (VTLigacao*)lisLigacaoJusante->Items[j];
//		for (int i = (lisLigacao->Count - 1); i > 0; i--)
//		{
//			VTLigacao *ligacaoOficial = (VTLigacao*)lisLigacao->Items[i];
//			if (ligacaoOficial->ligaPai != NULL && ligacaoOficial->ligaPai == ligacaoReferencia)
//			{
//				lisLigacaoJusante->Add(ligacaoOficial);
//			}
//		}
//	}
//
//	// Continua a pegar as ligações, mas dessa vez de cada um dos secundários do Primario já definido.
//	for (int i = 0; i < primario->LisSecundario()->Count; i++)
//	{
//		VTSecundario *secundario = (VTSecundario*)primario->LisSecundario()->Items[i];
//		AdicionaListaLigacoesJusanteSecundario(secundario);
//	}
//
//}

//// ----------------------------------------------------------------------------
//void __fastcall TRelatorioJusante::RemoveEqptosNaoJusante(VTEqpto *eqptoRef,
//											TList *lisEXT_BAR, TList *lisEXT_LIG)
//{
//	bool isBarra, isEqbar, isLigacao, isChave;
//	VTBarra *barraAUX;
//	VTLigacao *ligacaoAUX;
//	int indexEqpto =-1;
//
//	//Remove as barras que certamentenão estão à jusante
//	barraAUX = BarraReferencia(eqptoRef);
//	indexEqpto = lisEXT_BAR->IndexOfItem(barraAUX,FromBeginning);
//	//caso de ser uma ligacao selecionada
//	if(eqptoRef->TipoLigacao())
//		{indexEqpto++;}
//	if(indexEqpto < 0)
//		{indexEqpto = 0;}
//	//remove todas barras acima da barra selecionada
//	for(int nbo = (indexEqpto - 1); nbo >= 0; nbo--)
//	{
//		lisEXT_BAR->Delete(nbo);
//	}
//	//Remove as ligacoes que certamente não estão à jusante
//	ligacaoAUX = LigacaoReferencia(eqptoRef);
//	indexEqpto = lisEXT_LIG->IndexOfItem(ligacaoAUX,FromBeginning);
//	//caso de ser uma ligacao selecionada
//	if(eqptoRef->TipoLigacao())
//		{indexEqpto++;}
//	if(indexEqpto < 0)
//		{indexEqpto = 0;}
//	//remove todas ligacoes acima da ligacao selecionada
//	for(int nbo = (indexEqpto - 1); nbo >= 0; nbo--)
//	{
//		lisEXT_LIG->Delete(nbo);
//	}
//}// ----------------------------------------------------------------------------void __fastcall TRelatorioJusante::RemoveEqptosNaoJusante(VTEqpto *eqptoRef,
											TList *lisEXT_BAR, TList *lisEXT_LIG)
{
	//deu dó, mas mudei o approach
	VTNode *node_lig, *node_bar;
	VTBarra *barraREF;
	VTBarra *barraAUX2;
	VTLigacao *ligacaoREF;
	VTLigacao *ligacaoAUX2;
	TList    *lisNODE;
	TList    *lisNODEparaAnalisar;

	barraREF = BarraReferencia(eqptoRef);
	ligacaoREF = LigacaoReferencia(eqptoRef);

	//protecao
	if((barraREF == NULL) || (ligacaoREF == NULL))
		{return;}
	//percorre a arvore de ligacoes preenchendo a lista
	node_lig = arvore_lig->ExisteNode(ligacaoREF);
	//node_lig = ExisteNodeEqpto(ligacaoREF);
	if(node_lig == NULL)
	{
		//node_lig = ExisteNodeEqpto(ligacaoREF);
		//eqpamento nao pertence ao bloco, remover nenhum eqpto
		return;
	}

    lisEXT_BAR->Clear();
	lisEXT_LIG->Clear();
	lisNODEparaAnalisar = new TList;
	lisNODEparaAnalisar->Add(node_lig);
	//lisEXT_LIG->Add(ligacaoAUX2 = (VTLigacao*)node_lig->LisObjectNode()->First());
	//percorre todos nodes
	for (int nn = 0 ; nn < lisNODEparaAnalisar->Count; nn++)
	{
		node_lig = (VTNode*)lisNODEparaAnalisar->Items[nn];
		ligacaoAUX2 = (VTLigacao*)node_lig->LisObjectNode()->First();
		if(lisEXT_LIG->IndexOf(ligacaoAUX2) < 0)
		{
			lisEXT_LIG->Add(ligacaoAUX2);
			//adiciona os nodes filhos para analise
			node_lig->LisNode(lisNODEparaAnalisar);
		}
	}
	lisNODEparaAnalisar->Clear();
	//caso seja selecionado uma ligacao é necessario analisar as "barras filhas"
	//e não a barra referencia
	if(eqptoRef->TipoLigacao()== true)
	{
		ligacaoAUX2 = (VTLigacao*)eqptoRef;
		//loop p/ todas Barras da Ligacao
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barraAUX2 = ligacaoREF->Barra(ind_bar)) == NULL) continue;
			//caso a barra seja diferente da ref
			if(barraAUX2 !=  barraREF)
			{
				node_bar = arvore_bar->ExisteNode(barraAUX2);
				if(node_bar != NULL)
					{lisNODEparaAnalisar->Add(node_bar);}
			}
		}
	}
	else
	{
    	//percorre a arvore de barras preenchendo a lista
		//node_bar = arvore_bar->ExisteNode(barraREF);
		node_bar = ExisteNodeEqpto(barraREF);
		if(node_bar == NULL) return;
		lisNODEparaAnalisar->Add(node_bar);
    }
	//lisEXT_BAR->Add(node_bar->LisObjectNode()->First());
	//percorre todos nodes
	for (int nn = 0 ; nn < lisNODEparaAnalisar->Count; nn++)
	{
		node_bar = (VTNode*)lisNODEparaAnalisar->Items[nn];
		barraAUX2 = (VTBarra*)node_bar->LisObjectNode()->First();
		if(lisEXT_BAR->IndexOf(barraAUX2) < 0)
		{
			lisEXT_BAR->Add(barraAUX2);
			//adiciona os nodes filhos para analise
			node_bar->LisNode(lisNODEparaAnalisar);
		}
	}
	//deleta
	DestroiLista(lisNODEparaAnalisar);
	//caso o equipamento de referencia seja uma ligacao
	//necessario excluir a barra de referencia
	if(eqptoRef->TipoLigacao()== true)
		{lisEXT_BAR->Remove(barraREF);}
	//caso o equipamento de referencia seja uma barra
	//necessario excluir a ligacao de referencia
	if(eqptoRef->TipoBarra()== true)
		{lisEXT_LIG->Remove(ligacaoREF);}
}

// ----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17283172
void   __fastcall TRelatorioJusante::RemoveGD(void)
{
    //variaveis locais
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisEQP;

    //cria lista
    lisEQP = new TList();
    // verifica se ha geradores
    lisEQP->Clear();
    redes->LisEqpto(lisEQP, eqptoGERADOR);
    //atualiza flag
    existe_gd = (lisEQP->Count > 0);
    if(existe_gd)
    {//remove
        edita->RetiraLisEqpto(lisEQP);
    }
    //destroi lista
    delete lisEQP;
}

// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::SelecionaEqpto(VTEqpto *eqpto)
{
	this->eqpto = eqpto;
}
// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::SelecionaPatamar(VTPatamar *patamar)
{
	this->patamar = patamar;
}

// ----------------------------------------------------------------------------
void   __fastcall TRelatorioJusante::TipoEquipamento(int eqptoTIPO)
{
	switch (eqptoTIPO)
	{
	case eqptoBARRA:
		isBarra = true;
		break;
	case eqptoEQBAR:
		isEqbar = true;
		break;
	case eqptoCAPACITOR:
		isEqbar = true;
		break;
	case eqptoCAPSERIE:
		isLigacao = true;
		break;
	case eqptoCARGA:
		isEqbar = true;
		break;
	case eqptoCHAVE:
		isLigacao = true;
		//isChave = true;
		break;
	case eqptoCONSUMIDOR:
		isEqbar = true;
		eqptoTIPO = eqptoCARGA;
		break;
	case eqptoGERADOR:
		isEqbar = true;
		break;
	case eqptoFILTRO:
		isEqbar = true;
		break;
	case eqptoLIGACAO:
		isLigacao = true;
		break;
	case eqptoREATOR:
		isEqbar = true;
		break;
	case eqptoREGULADOR:
		isLigacao = true;
		break;
	case eqptoSUPRIMENTO:
		isEqbar = true;
		break;
	case eqptoTRAFO:
		isLigacao = true;
		break;
	case eqptoTRAFO3E:
		isLigacao = true;
		break;
	case eqptoTRAFOZZ:
		isLigacao = true;
		break;
	case eqptoTRECHO:
		isLigacao = true;
		break;
	default:
		;
	}
}
// ----------------------------------------------------------------------------

//double __fastcall TRelatorioJusante::SomaRelatorioJusante(void)
//{
//
//	// Inicia variável
//	double retorno = 0.0;
//
//	for (int i = 0; i < lisLigacaoJusante->Count; i++)
//	{
//		VTLigacao *ligacao = (VTLigacao*)lisLigacaoJusante->Items[i];
//		if (ligacao->Tipo() == eqptoTRAFO)
//		{
//			VTTrafo *trafo = (VTTrafo*)ligacao;
//			// Unidade em MVA
//			retorno += trafo->snom;
//		}
//	}
//
//	return retorno;
//
//}
// ----------------------------------------------------------------------------
