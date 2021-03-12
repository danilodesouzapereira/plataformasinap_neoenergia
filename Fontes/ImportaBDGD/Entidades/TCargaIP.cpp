// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TCargaIP.h>
#include <TBarraBDGD.h>
#include <TClasseSubclasse.h>
#include <TFasesConexao.h>
#include <TTensao.h>
#include <TGrupoTarifario.h>
#include <TAreaLocalizacao.h>

// -------------------------------------------------------------------------------------------------
TCargaIP* __fastcall NewObjCargaIP(void)
{
	return (new TCargaIP());
}

// -------------------------------------------------------------------------------------------------
__fastcall TCargaIP::TCargaIP(void)
{
	ene_ativa_kwh   = new float[12];
	// Inicialização de variáveis
	PD.id_cargaIP = 0;
	PD.id_rede_MT = 0;
	PD.id_rede_BT = 0;
	PD.barra = NULL;
	PD.classe_subclasse = NULL;
	PD.fases_con = NULL;
	PD.tensao = NULL;
	PD.grupo_tarif = NULL;
	PD.area_loc = NULL;
	PD.codigo = "";
	PD.carga_inst_kW = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TCargaIP::~TCargaIP(void)
{
	if (ene_ativa_kwh)      {delete[] ene_ativa_kwh;   ene_ativa_kwh   = NULL;}
}

//---------------------------------------------------------------------------
double __fastcall TCargaIP::PM_GetEne_kWh(int periodo)
   {
   //proteção
   if ((periodo-1 < 0)||(periodo > 12)) throw("TResFlowLig::PM_GetEne_kWh() - periodo inválido");
   //retorna demanda
   return(ene_ativa_kwh[periodo-1]);
   }

//---------------------------------------------------------------------------
void __fastcall TCargaIP::PM_SetEne_kWh(int periodo, double valor)
   {
   //proteção
   if ((periodo < 0)||(periodo >= 12)) throw("TCarga::PM_Set_Ene() - periodo inválido");
   //atualiza energia
   ene_ativa_kwh[periodo] = valor;
   }

// --------------------------------------------------------------------------------------------- eof
