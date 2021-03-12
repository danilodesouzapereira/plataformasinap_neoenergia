//---------------------------------------------------------------------------

#pragma hdrstop
#include "TInfoRJChave.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ----------------------------------------------------------------------------
TInfoRJChave* operator+=(TInfoRJChave &infoResult, const TInfoRJChave &infoAdd)
{
	infoResult.demandaAtivaAcum_kw += infoAdd.demandaAtivaAcum_kw;
	infoResult.demandaReativaAcum_kvar += infoAdd.demandaReativaAcum_kvar;
	infoResult.potenciaNomAcum_kva += infoAdd.potenciaNomAcum_kva;
	infoResult.qntdeConsuBT.resid += infoAdd.qntdeConsuBT.resid;
	infoResult.qntdeConsuBT.comerc += infoAdd.qntdeConsuBT.comerc;
	infoResult.qntdeConsuBT.indust += infoAdd.qntdeConsuBT.indust;
	infoResult.qntdeConsuMT.resid += infoAdd.qntdeConsuMT.resid;
	infoResult.qntdeConsuMT.comerc += infoAdd.qntdeConsuMT.comerc;
	infoResult.qntdeConsuMT.indust += infoAdd.qntdeConsuMT.indust;
	infoResult.consumo.resid_kwh += infoAdd.consumo.resid_kwh;
	infoResult.consumo.comerc_kwh += infoResult.consumo.comerc_kwh;
	infoResult.consumo.indust_kwh += infoResult.consumo.indust_kwh;
	//soma as listas de cargas e trafos
	infoResult.lisCargas->Assign(infoAdd.lisCargas, laOr);
	infoResult.lisTrafos->Assign(infoAdd.lisTrafos, laOr);

	return &infoResult;
}
// ----------------------------------------------------------------------------
__fastcall TInfoRJChave::TInfoRJChave(void)
{
	lisCargas = new TList;
	lisTrafos = new TList;
	this->Clear();
}
// ----------------------------------------------------------------------------
__fastcall TInfoRJChave::~TInfoRJChave(void)
{
	if(lisCargas){
		delete lisCargas;
		lisCargas = NULL;
	}
	if(lisTrafos){
		delete lisTrafos;
		lisTrafos = NULL;
	}
}
// ----------------------------------------------------------------------------
TInfoRJChave* __fastcall TInfoRJChave::Add(const  TInfoRJChave *infoAdd)
{
	this->demandaAtivaAcum_kw += infoAdd->demandaAtivaAcum_kw;
	this->demandaReativaAcum_kvar += infoAdd->demandaReativaAcum_kvar;
	this->potenciaNomAcum_kva += infoAdd->potenciaNomAcum_kva;
	this->qntdeConsuBT.resid += infoAdd->qntdeConsuBT.resid;
	this->qntdeConsuBT.comerc += infoAdd->qntdeConsuBT.comerc;
	this->qntdeConsuBT.indust += infoAdd->qntdeConsuBT.indust;
	this->qntdeConsuMT.resid += infoAdd->qntdeConsuMT.resid;
	this->qntdeConsuMT.comerc += infoAdd->qntdeConsuMT.comerc;
	this->qntdeConsuMT.indust += infoAdd->qntdeConsuMT.indust;
	this->consumo.resid_kwh += infoAdd->consumo.resid_kwh;
	this->consumo.comerc_kwh += infoAdd->consumo.comerc_kwh;
	this->consumo.indust_kwh += infoAdd->consumo.indust_kwh;
	//soma as listas de cargas e trafos
	this->lisCargas->Assign(infoAdd->lisCargas, laOr);
	this->lisTrafos->Assign(infoAdd->lisTrafos, laOr);
	return this;
}
// ----------------------------------------------------------------------------
void __fastcall TInfoRJChave::Clear(void)
{
	this->demandaAtivaAcum_kw = 0;
	this->demandaReativaAcum_kvar = 0;
	this->potenciaNomAcum_kva = 0;
	this->qntdeConsuBT.resid = 0;
	this->qntdeConsuBT.comerc = 0;
	this->qntdeConsuBT.indust = 0;
	this->qntdeConsuMT.resid = 0;
	this->qntdeConsuMT.comerc = 0;
	this->qntdeConsuMT.indust = 0;
	this->consumo.resid_kwh = 0;
	this->consumo.comerc_kwh = 0;
	this->consumo.indust_kwh = 0;
	//soma as listas de cargas e trafos
	this->lisCargas->Clear();
	this->lisTrafos->Clear();
}
// ----------------------------------------------------------------------------
//TInfoRJChave* operator+(TInfoRJChave *infoResult, const TInfoRJChave *infoAdd)
//{
//	infoResult->demandaAtivaAcum_kw += infoAdd->demandaAtivaAcum_kw;
//	infoResult->demandaReativaAcum_kvar += infoAdd->demandaReativaAcum_kvar;
//	infoResult->potenciaNomAcum_kva += infoAdd->potenciaNomAcum_kva;
//	infoResult->qntdeConsuBT.resid += infoAdd->qntdeConsuBT.resid;
//	infoResult->qntdeConsuBT.comerc += infoAdd->qntdeConsuBT.comerc;
//	infoResult->qntdeConsuBT.indust += infoAdd->qntdeConsuBT.indust;
//	infoResult->qntdeConsuMT.resid += infoAdd->qntdeConsuMT.resid;
//	infoResult->qntdeConsuMT.comerc += infoAdd->qntdeConsuMT.comerc;
//	infoResult->qntdeConsuMT.indust += infoAdd->qntdeConsuMT.indust;
//	infoResult->consumo.resid_kwh += infoAdd->consumo.resid_kwh;
//	infoResult->consumo.comerc_kwh += infoResult->consumo.comerc_kwh;
//	infoResult->consumo.indust_kwh += infoResult->consumo.indust_kwh;
//	//soma as listas de cargas e trafos
//	infoResult->lisCargas->Assign(infoAdd->lisCargas, laOr);
//	infoResult->lisTrafos->Assign(infoAdd->lisTrafos, laOr);
//}
// ----------------------------------------------------------------------------
