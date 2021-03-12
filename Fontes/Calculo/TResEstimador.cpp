//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TResEstimador.h"
#include "..\Apl\VTApl.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCanal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTResEstimador * __fastcall NewObjResEstimador(VTApl *apl)
   {
	return(new TResEstimador(apl));
   }

//---------------------------------------------------------------------------
__fastcall TResEstimador::TResEstimador(VTApl *apl)
   {
	//salva ponteiro para objeto
	this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	}

//---------------------------------------------------------------------------
__fastcall TResEstimador::~TResEstimador(void)
   {
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TResEstimador::ResMedidor(smcRES_MEDIDOR *smc_res_medidor)
   {
   // Transfere resultados do medidor
   VTCanal *canal            = (VTCanal *) smc_res_medidor->canal;
   int     ind_pat           = smc_res_medidor->ind_pat;
   double  val_med           = smc_res_medidor->val_med;
   double  val_est           = smc_res_medidor->val_est;
   double  erro_pu           = smc_res_medidor->erro_pu;
   bool    excede_desvio_max = smc_res_medidor->excede_desvio_max;

   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TResEstimador::ResMedidor(smcRES_MEDIDOR *smc_res_medidor)
   {
   //variáveis locais
   VTCanal   *canal   = (VTCanal *) smc_res_medidor->canal;
   VTPatamar *patamar = patamares->Patamar[smc_res_medidor->ind_pat];

   //verifica se o Patamar está habilitado
   if (patamar->Enabled)
      {//copia apenas valor estimado
      canal->ValorEstimado[patamar] = smc_res_medidor->val_est;
      canal->ValorNominal[patamar]  = smc_res_medidor->val_est_nom;
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof
