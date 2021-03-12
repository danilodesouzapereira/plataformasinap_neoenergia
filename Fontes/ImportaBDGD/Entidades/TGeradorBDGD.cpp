// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TGeradorBDGD.h>
#include <TBarraBDGD.h>
#include <TFasesConexao.h>
#include <TTensao.h>

// ------------------------------------------------------------------------------------------- class

TGeradorBDGD* __fastcall NewObjGeradorBDGD(void)
   {
   return(new TGeradorBDGD());
   }

//---------------------------------------------------------------------------
__fastcall TGeradorBDGD::TGeradorBDGD(void)
   {
	    // Inicialização de variáveis
		PD.id_gerador = 0;
		PD.id_rede = 0;
		PD.barra = NULL;
		PD.fases_con = NULL;
		PD.tensao = NULL;
		PD.codigo = "";
		PD.pot_instalada = 0.0;
		PD.pot_contratada_max = 0.0;
		PD.r0 = 0.0;
		PD.x0 = 0.0;
		PD.r1 = 0.0;
		PD.x1 = 0.0;
		PD.descricao = "";
	}

//---------------------------------------------------------------------------
__fastcall TGeradorBDGD::~TGeradorBDGD(void)
   {
   	// nada a fazer
   }

	
// --------------------------------------------------------------------------------------------- eof