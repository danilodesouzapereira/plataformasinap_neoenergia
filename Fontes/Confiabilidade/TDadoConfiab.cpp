//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDadoConfiab.h"                                                                  
#include "VTDataEqpto.h"
#include "TDataRede.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Bloco\VTBloco.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\GeraRede\TLigaSerie.h"
#include "..\..\DLL_inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTDadoConfiab* __fastcall NewObjDadoConfiab(void)
	{
	return(new TDadoConfiab());
	}
//---------------------------------------------------------------------------
__fastcall TDadoConfiab::TDadoConfiab(void)
	{   //hk201711mutua: criar lista de strMutua
	lisDataEqpto = new TList();
	lisDataRede  = new TList();
	lisDataBloco  = new TList();
	lisDataTipo  = new TList();
	lisDataTipoTrecho = new TList();
	lisDataTipoChave  = new TList();
	lisDataTipoGerador= new TList();
	lisDataTipoTrafo  = new TList();
	lisDataFalhaRede  = new TList();
	lisTipoChv        = new TList();
   lisEqptoMutua     = new TList();

	dataGeralEqpto    = NULL;
	dataGeralTrecho   = NULL;
	//Inicia flag cálculo por blocos
	flag_BLOCO = false;
	flag_BARRA = false; //hkN
	//bloco com parametros especificos
	bloco = NULL;
	}
//---------------------------------------------------------------------------
__fastcall TDadoConfiab::~TDadoConfiab(void)
	{
	if(lisDataEqpto)     {LimpaTList(lisDataEqpto); delete lisDataEqpto; lisDataEqpto = NULL;}
	if(lisDataRede)      {LimpaTList(lisDataRede); delete lisDataRede; lisDataRede = NULL;}
	if(lisDataBloco)   {LimpaTList(lisDataBloco); delete lisDataBloco; lisDataBloco = NULL;}
	if(lisDataTipo)       {LimpaTList(lisDataTipo); delete lisDataTipo; lisDataTipo = NULL;}
	if(lisDataTipoGerador){LimpaTList(lisDataTipoGerador); delete lisDataTipoGerador; lisDataTipoGerador = NULL;}
	if(lisDataTipoTrecho) {LimpaTList(lisDataTipoTrecho); delete lisDataTipoTrecho; lisDataTipoTrecho = NULL;}
	if(lisDataTipoChave)  {LimpaTList(lisDataTipoChave); delete lisDataTipoChave; lisDataTipoChave = NULL;}
	if(lisDataTipoTrafo)  {LimpaTList(lisDataTipoTrafo); delete lisDataTipoTrafo; lisDataTipoTrafo = NULL;}
	if(lisDataFalhaRede)  {LimpaTList(lisDataFalhaRede); delete lisDataFalhaRede; lisDataFalhaRede = NULL;}
	if(lisEqptoMutua)     {LimpaTList(lisEqptoMutua); delete lisEqptoMutua; lisEqptoMutua = NULL;}
	if(lisTipoChv)        {delete lisTipoChv; lisTipoChv = NULL;}
	if(dataGeralTrecho)   {delete dataGeralTrecho; dataGeralTrecho = NULL;}
	if(dataGeralEqpto)    {delete dataGeralEqpto; dataGeralEqpto = NULL;}
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::CopiaDados(VTDadoConfiab* dataExt)
	{
	VTDadoConfiab *dataExtAux, *dataAux;
	VTDataEqpto *data, *dataN;
	TDataRede  *dataRede;

	Inicia();
   //hk201711mutua: copiar lista de strMutua
	//Copia dados específicos de blocos
	for(int n = 0; n < dataExt->LisDataBloco->Count; n++)
		{
		dataExtAux = (VTDadoConfiab*)dataExt->LisDataBloco->Items[n];
		lisDataBloco->Add(dataAux = NewObjDadoConfiab());
		dataAux->CopiaDados(dataExtAux);
		}

	this->descricao = dataExt->descricao;
	this->icont   = dataExt->icont;//ordem das contingencias considerado
	this->flag_NA = dataExt->flag_NA;//consideração de socorro entre circuitos
	this->flag_BLOCO = dataExt->flag_BLOCO;
	this->flag_BARRA = dataExt->flag_BARRA; //hkN
	//bloco com dados especificos
	//this->bloco   = bloco;

	dataGeralTrecho->CopiaDados(dataExt->DataGeralTrecho);
	dataGeralEqpto->CopiaDados(dataExt->DataGeralEqpto);
	for(int n = 0; n < dataExt->LisDataEqpto->Count; n++)
		{data = (VTDataEqpto*)dataExt->LisDataEqpto->Items[n];
		 lisDataEqpto->Add(dataN = NewObjDataEqpto(NULL));
		 dataN->CopiaDados(data);
		}
	for(int n = 0; n < dataExt->LisDataTipo->Count; n++)
		{data = (VTDataEqpto*)dataExt->LisDataTipo->Items[n];
		 lisDataTipo->Add(dataN = NewObjDataEqpto(NULL));
		 dataN->CopiaDados(data);
		}
	for(int n = 0; n < dataExt->LisDataTipoTrecho->Count; n++)
		{data = (VTDataEqpto*)dataExt->LisDataTipoTrecho->Items[n];
		 lisDataTipoTrecho->Add(dataN = NewObjDataEqpto(NULL));
		 dataN->CopiaDados(data);
		}
	for(int n = 0; n < dataExt->LisDataTipoChave->Count; n++)
		{data = (VTDataEqpto*)dataExt->LisDataTipoChave->Items[n];
		 lisDataTipoChave->Add(dataN = NewObjDataEqpto(NULL));
		 dataN->CopiaDados(data);
		}
	for(int n = 0; n < dataExt->LisDataTipoTrafo->Count; n++)
		{data = (VTDataEqpto*)dataExt->LisDataTipoTrafo->Items[n];
		 lisDataTipoTrafo->Add(dataN = NewObjDataEqpto(NULL));
		 dataN->CopiaDados(data);
		}
	for(int n = 0; n < dataExt->LisDataTipoGerador->Count; n++)
		{data = (VTDataEqpto*)dataExt->LisDataTipoGerador->Items[n];
		 lisDataTipoGerador->Add(dataN = NewObjDataEqpto(NULL));
		 dataN->CopiaDados(data);
		}
	for(int n = 0; n < dataExt->LisDataAjusteRede->Count; n++)
		{dataRede = (TDataRede*)dataExt->LisDataAjusteRede->Items[n];
		 lisDataRede->Add(new TDataRede(dataRede->rede, dataRede->pres));
		}
	lisTipoChv->Assign(dataExt->lisTipoChv, laOr);

	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamEqpto(PAR_CONF pconf, VTEqpto* eqpto)
	{
	VTDataEqpto* dataEqpto = NULL;
	VTDataEqpto* dataAux;

	if(eqpto == NULL) return;
	//Busca dataEqpto do equipamento itipo
	for(int n = 0; n < lisDataEqpto->Count; n++)
		 {
		 dataAux = (VTDataEqpto*)lisDataEqpto->Items[n];
		 if(dataAux->eqpto == eqpto) {dataEqpto = dataAux;
												break;
											  }
		 }
	if(!dataEqpto) lisDataEqpto->Add(dataEqpto = NewObjDataEqpto(eqpto));

	dataEqpto->pconf = pconf;
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamGeralEqpto(PAR_CONF pconf)
	{
	if(!dataGeralEqpto) dataGeralEqpto  = NewObjDataEqpto(NULL);
	dataGeralEqpto->pconf = pconf;
	//Define para valor específico para eqptos trecho
	DefParamGeralTipo(pconf, eqptoGERADOR);
	DefParamGeralTipo(pconf, eqptoTRAFO);
	DefParamGeralTipo(pconf, eqptoCHAVE);
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamGeralTrecho(PAR_CONF pconf)
	{
	if(!dataGeralTrecho) dataGeralTrecho = NewObjDataEqpto(NULL);
	dataGeralTrecho->pconf = pconf;
	//Define para valor específico para eqptos trecho
	DefParamGeralTipo(pconf, eqptoTRECHO);
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamFalhaRede(PAR_CONF pconf, TObject* rede)
	{
	VTDataEqpto* dataFalhaRede = NULL;
	VTDataEqpto* dataFalhaRedeAux;

	//Busca dataEqpto do equipamento itipo
	for(int n = 0; n < lisDataFalhaRede->Count; n++)
		 {
		 dataFalhaRedeAux = (VTDataEqpto*)lisDataFalhaRede->Items[n];
		 if(dataFalhaRedeAux->eqpto_aux == rede) {dataFalhaRede = dataFalhaRedeAux;
												break;
											  }
		 }
	if(!dataFalhaRede) lisDataEqpto->Add(dataFalhaRede = NewObjDataEqpto(NULL));

	dataFalhaRede->eqpto_aux = rede;
	dataFalhaRede->pconf     = pconf;
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamRede(RES_CONF pres, VTRede* rede)
	{
	TDataRede* dataRede;

	dataRede = NULL;
	for(int n = 0; n < lisDataRede->Count; n++)
		{
		dataRede = (TDataRede*)lisDataRede->Items[n];
		if(dataRede->rede == rede) break;
		dataRede = NULL;
		}

	if(dataRede == NULL)
		lisDataRede->Add(dataRede = new TDataRede(rede, pres));
	else
		dataRede->pres = pres;

	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamGeralTipoChave(PAR_CONF pconf, TObject* tipochave)
	{
	VTDataEqpto *dataTipoChave = NULL;

	if(tipochave == NULL) return;
	//verifica se o dado para o tipo de equipamento já existe
	for(int n = 0; n < lisDataTipoChave->Count; n++)
		{
		dataTipoChave = (VTDataEqpto*)lisDataTipoChave->Items[n];
		if(dataTipoChave->eqpto_aux == tipochave) break;
		dataTipoChave = NULL;
		}
	if(dataTipoChave == NULL)
	  {dataTipoChave = NewObjDataEqpto(NULL);
		lisDataTipoChave->Add(dataTipoChave);
		dataTipoChave->eqpto_aux = tipochave;
	  }


	//Define parametros pelo tipo de chave

	dataTipoChave->pconf = pconf;
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamGeralTipoGerador(PAR_CONF pconf, double smin, double smax)
	{
	VTDataEqpto *dataTipoGerador = NULL;

	//verifica se o dado para o tipo de equipamento já existe
	for(int n = 0; n < lisDataTipoGerador->Count; n++)
		{
		dataTipoGerador = (VTDataEqpto*) lisDataTipoGerador->Items[n];
		if((dataTipoGerador->vmin == smin)&&(dataTipoGerador->vmax == smax)) break;
		dataTipoGerador = NULL;
		}
	if(dataTipoGerador == NULL)
	  {dataTipoGerador = NewObjDataEqpto(NULL);
		lisDataTipoGerador->Add(dataTipoGerador);
		dataTipoGerador->vmin = smin;
		dataTipoGerador->vmax = smax;
	  }

	dataTipoGerador->pconf = pconf;
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamGeralTipoTrafo(PAR_CONF pconf, double smin, double smax)
	{
	VTDataEqpto *dataTipoTrafo = NULL;

	//verifica se o dado para o tipo de equipamento já existe
	for(int n = 0; n < lisDataTipoTrafo->Count; n++)
		{
		dataTipoTrafo = (VTDataEqpto*)lisDataTipoTrafo->Items[n];
		if((dataTipoTrafo->vmin == smin)&&(dataTipoTrafo->vmax == smax)) break;
		dataTipoTrafo = NULL;
		}
	if(dataTipoTrafo == NULL)
	  {dataTipoTrafo = NewObjDataEqpto(NULL);
		lisDataTipoTrafo->Add(dataTipoTrafo);
		dataTipoTrafo->vmin = smin;
		dataTipoTrafo->vmax = smax;
	  }


	//Define parametros pelo tipo de cabo do trecho

	dataTipoTrafo->pconf = pconf;
	}
//---------------------------------------------------------------------------
VTDataEqpto* __fastcall TDadoConfiab::DefParamGeralTipoTrecho(PAR_CONF pconf, VTEqpto* cabo)
	{
	VTDataEqpto *dataTipoTrecho = NULL;

   if(cabo == NULL) return(NULL);
	//verifica se o dado para o tipo de equipamento já existe
	for(int n = 0; n < lisDataTipoTrecho->Count; n++)
		{
		dataTipoTrecho = (VTDataEqpto*)lisDataTipoTrecho->Items[n];
		if(dataTipoTrecho->eqpto_aux == (TObject*)cabo) break;
		dataTipoTrecho = NULL;
		}
	if(dataTipoTrecho == NULL)
	  {dataTipoTrecho = NewObjDataEqpto(NULL);
		lisDataTipoTrecho->Add(dataTipoTrecho);
		dataTipoTrecho->eqpto_aux = (TObject*)cabo;
	  }


	//Define parametros pelo tipo de cabo do trecho

	dataTipoTrecho->pconf = pconf;
	return(dataTipoTrecho);
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::DefParamGeralTipo(PAR_CONF pconf, int itipo)
	{
	VTDataEqpto* dataTipo = NULL;
	VTDataEqpto* dataAux;
	//Busca dataEqpto do equipamento itipo
	for(int n = 0; n < lisDataTipo->Count; n++)
		 {
		 dataAux = (VTDataEqpto*)lisDataTipo->Items[n];
		 if(dataAux->itipo == itipo) {dataTipo = dataAux;
												break;
											  }
		 }
	if(!dataTipo) {lisDataTipo->Add(dataTipo = NewObjDataEqpto(NULL));
					  dataTipo->itipo = itipo;
					  }

	dataTipo->pconf = pconf;
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::Inicia(void)
	{
	PAR_CONF    pconf;
	pconf.tx_falha = 0.10;
	pconf.tx_interr= 0.05;
	pconf.treparo  = 1.50;
	pconf.tman     = 0.66;
	pconf.fat_isola= 1.00;
	pconf.comp     = 1.00;

	DefParamGeralTrecho(pconf);
	DefParamGeralEqpto(pconf);
   //hk201711mutua: limpar lista de strMutua
	LimpaTList(lisDataBloco);
	LimpaTList(lisDataEqpto);
	LimpaTList(lisDataTipo);
	LimpaTList(lisDataTipoGerador);
	LimpaTList(lisDataTipoTrecho);
	LimpaTList(lisDataTipoChave);
	LimpaTList(lisDataTipoTrafo);
	LimpaTList(lisDataFalhaRede);
	LimpaTList(lisDataRede);
	lisTipoChv->Clear();

	icont = 0;
	flag_BLOCO = false;
	flag_BARRA = false; //hkN
	flag_NA    = true;
	bloco = NULL;
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::IniciaFatRede(void)
	{
	TDataRede* dataRede;
	for(int n = 0; n < lisDataRede->Count; n++)
		{
		dataRede = (TDataRede*)lisDataRede->Items[n];
		dataRede->pres.fat_dec = 1.;
		dataRede->pres.fat_fec = 1.;
		}

	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::Interrupcoes(VTEqpto* eqpto, double &fano, double &mano)
	{
	PAR_CONF pconf;

	if(!ParamEqpto(pconf, eqpto)) return(false);

	if(eqpto->Tipo() == eqptoTRECHO)
	  {fano = pconf.tx_falha * ((VTTrecho*)eqpto)->Comprimento_km;
		mano = pconf.tx_interr  * ((VTTrecho*)eqpto)->Comprimento_km;
	  }
	else
	  {fano = pconf.tx_falha;
		mano = pconf.tx_interr;
	  }

	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TDadoConfiab::ImprimeDado(FILE *fout)
	{
	PAR_CONF pconf;
	AnsiString codigo;
	VTDataEqpto* dEqpto;

	if(fout == NULL) return;

	fprintf(fout, "-------------------------------------------------\n");

	fprintf(fout, "-------------------------------------------------\n");
	fprintf(fout, "Descrição: \t%s\n", this->descricao.c_str());
	fprintf(fout, "-------------------------------------------------\n");
	fprintf(fout, "-->Geral trecho\t"); ParamGeralTrecho(pconf);
	fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
	fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
	fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
	fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
	fprintf(fout, "-->Geral equipamento\t"); ParamGeralEqpto(pconf);
	fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
	fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
	fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
	fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
	fprintf(fout, "-------------------------------------------------\n");

	fprintf(fout, "-->Tipo trecho\t"); ParamGeralTipo(pconf, eqptoTRECHO);
	fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
	fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
	fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
	fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
	fprintf(fout, "-->Tipo chave\t"); ParamGeralTipo(pconf, eqptoCHAVE);
	fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
	fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
	fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
	fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
	fprintf(fout, "-->Tipo Gerador\t"); ParamGeralTipo(pconf, eqptoGERADOR);
	fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
	fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
	fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
	fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
	fprintf(fout, "-->Tipo Trafo\t"); ParamGeralTipo(pconf, eqptoTRAFO);
	fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
	fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
	fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
	fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
	fprintf(fout, "-TipoTrecho------------------------------------------------\n");
	for(int n = 0; n < lisDataTipoTrecho->Count; n++)
		{dEqpto = (VTDataEqpto*)lisDataTipoTrecho->Items[n];
		 pconf =  dEqpto->pconf;
		 codigo = ((VTCabo*)dEqpto->eqpto_aux)->Codigo;
		 fprintf(fout, "Arranjo:\t%s\t", codigo.c_str());
		 fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		 fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
		 fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		 fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
		}
	fprintf(fout, "-TipoChave------------------------------------------------\n");
	for(int n = 0; n < lisDataTipoChave->Count; n++)
		{dEqpto = (VTDataEqpto*)lisDataTipoChave->Items[n];
		 pconf =  dEqpto->pconf;
		 codigo = ((VTTipoChave*)dEqpto->eqpto_aux)->Codigo;
		 fprintf(fout, "Tipo:\t%s\t", codigo.c_str());
		 fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		 fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
		 fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		 fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
		}
	fprintf(fout, "-TipoTrafo------------------------------------------------\n");
	for(int n = 0; n < lisDataTipoTrafo->Count; n++)
		{dEqpto = (VTDataEqpto*)lisDataTipoTrafo->Items[n];
		 pconf =  dEqpto->pconf;
		 codigo.sprintf("Trafo %6.2f-%6.2f\t", dEqpto->vmin, dEqpto->vmax);
		 fprintf(fout, "Faixa:\t%s\t", codigo.c_str());
		 fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		 fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
		 fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		 fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
		}
	fprintf(fout, "-TipoGerador------------------------------------------------\n");
	for(int n = 0; n < lisDataTipoGerador->Count; n++)
		{dEqpto = (VTDataEqpto*)lisDataTipoGerador->Items[n];
		 pconf =  dEqpto->pconf;
		 codigo.sprintf("Gerador %6.2f-%6.2f\t", dEqpto->vmin, dEqpto->vmax);
		 fprintf(fout, "Faixa:\t%s\t", codigo.c_str());
		 fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		 fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
		 fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		 fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
		}
	fprintf(fout, "-------------------------------------------------------\n");
	fprintf(fout, "-Rede------------------------------------------------\n");

	for(int n = 0; n < lisDataFalhaRede->Count; n++)
		{dEqpto = (VTDataEqpto*)lisDataFalhaRede->Items[n];
		 pconf =  dEqpto->pconf;
		 codigo.sprintf("Rede: %s\t", ((VTRede*)(dEqpto->eqpto_aux))->Codigo.c_str());
		 fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		 fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
		 fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		 fprintf(fout, "tinterr:\t%6.3f\n", pconf.tman);
		}
	fprintf(fout, "-------------------------------------------------------\n");
	fprintf(fout, "-TiposChaveProtecao------------------------------------------------\n");
	for(int n = 0; n < lisTipoChv->Count; n++)
		{
		 codigo = ((VTTipoChave*)lisTipoChv->Items[n])->Codigo;
		 fprintf(fout, "%i: \t %s\n", n, codigo.c_str());
		}
	fprintf(fout, "-------------------------------------------------------\n");
	fprintf(fout, "-Parametros gerais------------------------------------------------\n");
	fprintf(fout, "icont: %i\n", this->icont);
	codigo = (this->flag_BLOCO) ? "S" : "N";
	fprintf(fout, "flag_BLOCO: %s\n", codigo.c_str());
	codigo = (this->flag_BARRA) ? "S" : "N";
	fprintf(fout, "flag_BARRA: %s\n", codigo.c_str());
	codigo = (this->flag_NA) ? "S" : "N";
	fprintf(fout, "flag_NA: %s\n", codigo.c_str());
	fprintf(fout, "-------------------------------------------------------\n");

	}
//---------------------------------------------------------------------------
int __fastcall TDadoConfiab::TipoEqpto(VTEqpto* eqpto)
	{
	int itipo;
	switch(eqpto->Tipo())
		  {
		  case eqptoTRECHO:   itipo = eqptoTRECHO;   break;
		  case eqptoGERADOR:  itipo = eqptoGERADOR;  break;
		  case eqptoTRAFO:    itipo = eqptoTRAFO;    break;
		  case eqptoTRAFO3E:  itipo = eqptoTRAFO;    break;
		  case eqptoTRAFOZZ:  itipo = eqptoTRAFO;    break;
		  case eqptoCHAVE:    itipo = eqptoCHAVE;    break;
		  case eqptoBARRA:    itipo = eqptoBARRA;    break;//inclui barras
		  default:            itipo = -1;            break;
		  }
	return(itipo);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamEqpto(PAR_CONF &pconf, VTEqpto* eqpto)
	{
	TMetaClass *obj_class;
	TLigaSerie *ligaS;
	TList      *lisEqpto;
	VTEqpto    *eqpto_aux;
	AnsiString obj_name;
	bool       resposta, flag_trecho;
	double     nf, nm, nftot, nmtot, tftot, tmtot, dtot, daux, naux;


	if(eqpto == NULL) return(false);
	//Busca dataEqpto do equipamento itipo


	//Verfica se o equipamento é uma agregação de trechos e equipamentos
	obj_class = eqpto->ClassType();
	obj_name  = obj_class->ClassName();
	if(obj_name.AnsiCompareIC("TLigaSerie") == 0)
	  {
	  ligaS = (TLigaSerie*)eqpto;
	  lisEqpto = ligaS->LisEqpto();
	  //flag_trecho = false;
	  nftot = 0.; nmtot = 0.; dtot = 0.;
	  tftot = 0.; tmtot = 0.;
	  resposta = false;
	  for(int n = 0; n < lisEqpto->Count; n++)
		  {
		  eqpto_aux = (VTEqpto*)lisEqpto->Items[n];
		  if(!Param1Eqpto(pconf, eqpto_aux)) continue;
		  else
			 resposta = true;
		  //se houver um trecho no conjunto registro do número de falhas por km
		  if(eqpto_aux->Tipo() == eqptoTRECHO)
			 {
			  daux  = ((VTTrecho*)eqpto_aux)->Comprimento_km;
			  dtot += daux;

			  naux  = (pconf.tx_falha * daux);
			  tftot+= (naux * pconf.treparo);
			  nftot+= naux;

			  naux  = (pconf.tx_interr * daux);
			  tmtot+= (naux * pconf.tman);
			  nmtot+= naux;
			 }
		  else
			 {nftot+= pconf.tx_falha;
			  tftot+= (pconf.tx_falha * pconf.treparo);
			  nmtot+= pconf.tx_interr;
			  tmtot+= (pconf.tx_interr * pconf.tman);
			 }

		  }

	  pconf.comp = dtot;//ligaS->Comprimento_km = dtot; ALTERA
	  if(resposta)
		 {//Calcula taxas(falhas/ ano) do equipamento
		  pconf.tx_falha = (dtot > 0.) ? nftot / dtot : nftot; //nftot;
		  pconf.tx_interr= (dtot > 0.) ? nmtot / dtot : nmtot; //nmtot;
		  pconf.treparo  = (IsDoubleZero(nftot)) ? 0 : tftot / nftot;
		  pconf.tman     = (IsDoubleZero(nmtot)) ? 0 : tmtot / nmtot;
		 }
	  else
		 {pconf.tx_falha = pconf.tx_interr = 0.;
		  pconf.treparo  = pconf.tman      = 0.;
		 }
	  }
	else
	  {
	  resposta = Param1Eqpto(pconf, eqpto);
	  //if(resposta = Param1Eqpto(pconf, eqpto))
	  //	 {
	  //	 if(eqpto->Tipo() == eqptoTRECHO)
	  //		{
	  //		pconf.tx_falha *= ((VTTrecho*)eqpto)->Comprimento_km;
	  //    pconf.tx_interr *= ((VTTrecho*)eqpto)->Comprimento_km;
	  //    }
	  //  }
	  }



	return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::Param1Eqpto(PAR_CONF &pconf, VTEqpto* eqpto)
	{
	VTDataEqpto* dataEqpto = NULL;
	VTDataEqpto* dataAux;
	int         itipo;
	VTDadoConfiab *dataConfAux;
	VTBloco       *bloco;
	TList         *lisAux;


	//Busca em parâmetros específicos por equipamento
	for(int n = 0; n < lisDataEqpto->Count; n++)
		 {
		 dataAux = (VTDataEqpto*)lisDataEqpto->Items[n];
		 if(dataAux->eqpto == eqpto) {dataEqpto = dataAux;
												break;
											  }
		 }

	if(dataEqpto) {pconf = dataEqpto->pconf; return(true);}


	//Busca paramatros específicos de blocos)
	for(int n = 0; n< lisDataBloco->Count; n++)
		{
		dataConfAux = (VTDadoConfiab*)lisDataBloco->Items[n];


		if(eqpto->Tipo() == eqptoBARRA) lisAux = bloco->LisBarra();  //quem define esse bloco?
		else  lisAux = dataConfAux->Bloco->LisLigacao();

		if(lisAux->IndexOf(eqpto) < 0) continue;

		return(dataConfAux->ParamEqpto(pconf, eqpto));
		}

	//Busca parâmetros específicos de redes
	for(int n = 0; n < lisDataFalhaRede->Count; n++)
		{
		dataAux = (VTDataEqpto*)lisDataFalhaRede->Items[n];
		lisAux = ((VTRede*)dataAux->eqpto_aux)->LisLigacao();
		lisAux->Assign(((VTRede*)dataAux->eqpto_aux)->LisBarra(), laOr);
		if(lisAux->IndexOf(eqpto) != -1) {dataEqpto = dataAux;
												break;
											  }
		 }
	if(dataEqpto) {pconf = dataEqpto->pconf; return(true);}

	//Busca em parâmetros gerais
	itipo = TipoEqpto(eqpto);

	if(itipo < 0)
	  return(false);
	else
	  {
	  if(ParamTipoEqpto(pconf, eqpto)) return(true);
	  return(ParamGeralTipo(pconf, itipo));
	  }
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamTipoTrecho(PAR_CONF &pconf, VTCabo* cabo)
	{
	VTDataEqpto* dataEqpto;
	for(int n = 0; n < lisDataTipoTrecho->Count; n++)
		  {
		  dataEqpto = (VTDataEqpto*)lisDataTipoTrecho->Items[n];
		  if(dataEqpto->eqpto_aux == cabo)
			  {
			  pconf = dataEqpto->pconf;
			  return(true);
			  }
		  }
	return(false);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamTipoChave(PAR_CONF &pconf, VTTipoChave* ptipo)
	{
	VTDataEqpto* dataEqpto;
	for(int n = 0; n < lisDataTipoChave->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataTipoChave->Items[n];
		if(dataEqpto->eqpto_aux == (TObject*)ptipo)
		  {
		  pconf = dataEqpto->pconf;
		  return(true);
		  }
		}
	return(false);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamTipoGerador(PAR_CONF &pconf, double s)
	{
	VTDataEqpto* dataEqpto;
	for(int n = 0; n < lisDataTipoGerador->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataTipoGerador->Items[n];
		if((dataEqpto->vmin <= s)&&(dataEqpto->vmax > s))
		  {
		  pconf = dataEqpto->pconf;
		  return(true);
		  }
		}
	return(false);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamTipoTrafo(PAR_CONF &pconf, double s)
	{
	VTDataEqpto* dataEqpto;
	for(int n = 0; n < lisDataTipoTrafo->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataTipoTrafo->Items[n];
		if((dataEqpto->vmin <= s)&&(dataEqpto->vmax > s))
		  {
		  pconf = dataEqpto->pconf;
		  return(true);
		  }
		}
	return(false);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamTipoEqpto(PAR_CONF &pconf, VTEqpto* eqpto)
	{
	VTTrecho* ptrecho;
	VTTrafo*   trf;
	VTTrafo3E* trf3E;
	VTTrafoZZ* trfZZ;
	VTChave*  pchave;
	VTCabo* cabo;
	double  s;
	bool    resposta = false;

	if(eqpto->Tipo() == eqptoTRECHO)
	  {
	  ptrecho = (VTTrecho*)eqpto;
	  cabo = ptrecho->arranjo->fase[0].cabo;
	  if(cabo == NULL) return(false);
	  resposta = ParamTipoTrecho(pconf, cabo);
	  }

	if(eqpto->Tipo() == eqptoCHAVE)
	  {
	  pchave = (VTChave*)eqpto;
	  resposta = ParamTipoChave(pconf, pchave->TipoChave);
	  }

	if(eqpto->Tipo() == eqptoTRAFO)
	  {
	  trf = (VTTrafo*)eqpto;
	  resposta = ParamTipoTrafo(pconf, trf->snom);
	  }


	return(resposta);
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamGeralTipo(PAR_CONF &pconf, int itipo)
	{
	VTDataEqpto* dataTipo = NULL;
	VTDataEqpto* dataAux;

   if(itipo < 0) return(false);
   //Busca dataEqpto do equipamento itipo
	for(int n = 0; n < lisDataTipo->Count; n++)
		 {
		 dataAux = (VTDataEqpto*)lisDataTipo->Items[n];
		 if(dataAux->itipo == itipo) {dataTipo = dataAux;
                                    break;
                                   }
       }
	if(dataTipo) {pconf = dataTipo->pconf; return(true);}

	if(itipo == eqptoTRECHO)
	  return(ParamGeralTrecho(pconf));
   else
     return(ParamGeralEqpto(pconf));
	}
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamGeralEqpto(PAR_CONF &pconf)
   {
   if(!dataGeralEqpto) return(false);
	pconf = dataGeralEqpto->pconf;
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TDadoConfiab::ParamGeralTrecho(PAR_CONF &pconf)
	{
   if(!dataGeralTrecho) return(false);
   pconf = dataGeralTrecho->pconf;
   return(true);
   }
//---------------------------------------------------------------------------

