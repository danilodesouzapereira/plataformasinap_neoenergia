// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TCargaBDGD.h>
#include <TBarraBDGD.h>
#include <TClasseSubclasse.h>
#include <TFasesConexao.h>
#include <TTensao.h>
#include <TGrupoTarifario.h>
#include <TAreaLocalizacao.h>

// -------------------------------------------------------------------------------------------------
TCargaBDGD* __fastcall NewObjCargaBDGD(void)
{
	return (new TCargaBDGD());
}

// -------------------------------------------------------------------------------------------------
__fastcall TCargaBDGD::TCargaBDGD(void)
{

	//cria vetores de 1 dimensão: cargto e diag
	dem_ativa_max_kw   = new float[12];
	ene_ativa_kwh   = new float[12];
   // Inicialização de variáveis
	PD.id_carga = 0;
	PD.id_rede = 0;
	PD.barra = NULL;
	PD.classe_subclasse = NULL;
	PD.fases_con = NULL;
	PD.tensao = NULL;
	PD.grupo_tarif = NULL;
	PD.area_loc = NULL;
	PD.codigo = "";
	PD.carga_inst_kW = 0.0;
	PD.descricao = "";
    PD.id_inspecao = -1;
}

// -------------------------------------------------------------------------------------------------
__fastcall TCargaBDGD::~TCargaBDGD(void)
{
	if (dem_ativa_max_kw)   {delete[] dem_ativa_max_kw;   dem_ativa_max_kw   = NULL;}
	if (ene_ativa_kwh)      {delete[] ene_ativa_kwh;   ene_ativa_kwh   = NULL;}
}

//---------------------------------------------------------------------------
double __fastcall TCargaBDGD::PM_GetDem_Max_kW(int periodo)
   {
   //proteção
   if ((periodo-1 < 0)||(periodo > 12)) throw("TResFlowLig::PM_GetDem_Max() - periodo inválido");
   //retorna demanda
   return(dem_ativa_max_kw[periodo-1]);
   }

//---------------------------------------------------------------------------
double __fastcall TCargaBDGD::PM_GetEne_kWh(int periodo)
   {
   //proteção
   if ((periodo-1 < 0)||(periodo > 12)) throw("TResFlowLig::PM_GetEne_kWh() - periodo inválido");
   //retorna demanda
   return(ene_ativa_kwh[periodo-1]);
   }

//---------------------------------------------------------------------------
void __fastcall TCargaBDGD::PM_SetDem_Max_kW(int periodo, double valor)
   {
   //proteção
   if ((periodo < 0)||(periodo >= 12)) throw("TCarga::PM_SetDem_Max() - periodo inválido");
   //atualiza demanda
   dem_ativa_max_kw[periodo] = valor;
   }

//---------------------------------------------------------------------------
void __fastcall TCargaBDGD::PM_SetEne_kWh(int periodo, double valor)
   {
   //proteção
   if ((periodo < 0)||(periodo >= 12)) throw("TCarga::PM_Set_Ene() - periodo inválido");
   //atualiza energia
   ene_ativa_kwh[periodo] = valor;
   }

// --------------------------------------------------------------------------------------------- eof
