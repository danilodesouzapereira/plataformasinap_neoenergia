//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Zona\VTZona.h>
#include <DLL_INC\Funcao.h>
#include "TPerfilRede.h"
#include "VTPerfilCarga.h"
#include "VTPerfilSup.h"
#include "VTMedicao.h"
#include "VTRes505.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerfilRede*  __fastcall NewObjPerfilRede(VTApl *apl)
   {
   try{
      return(new TPerfilRede(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilRede::TPerfilRede(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   lisEQP        = new TList();
//   lisPERFIL_BAR = new TList();
   lisPERFIL_CAR = new TList();
//   lisPERFIL_REG = new TList();
   lisPERFIL_SUP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPerfilRede::~TPerfilRede(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destrói lista e seus objetos
//   if (lisPERFIL_BAR) {LimpaTList(lisPERFIL_BAR); delete lisPERFIL_BAR; lisPERFIL_BAR = NULL;}
   if (lisPERFIL_CAR) {LimpaTList(lisPERFIL_CAR); delete lisPERFIL_CAR; lisPERFIL_CAR = NULL;}
//   if (lisPERFIL_REG) {LimpaTList(lisPERFIL_REG); delete lisPERFIL_REG; lisPERFIL_REG = NULL;}
   if (lisPERFIL_SUP) {LimpaTList(lisPERFIL_SUP); delete lisPERFIL_SUP; lisPERFIL_SUP = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TPerfilRede::CalculaDRA_DRP_DRC(int limite_amostra)
   {
   //variáveis locais
   VTPerfilBarra *perfil_barra;
   VTRes505      *res505  = (VTRes505*)apl->GetObject(__classid(VTRes505));

   for (int n = 0; n < lisPERFIL_BAR->Count; n++)
      {
      perfil_barra = (VTPerfilBarra*)lisPERFIL_BAR->Items[n];
      //calcula índices de  qualidade da Barra
      perfil_barra->CalculaDRA_DRP_DRC(res505, limite_amostra);
      }
   }
*/ /*
//---------------------------------------------------------------------------
void __fastcall TPerfilRede::CalculaIndicesQualidadeUmDia(int ind_dia)
   {
   //variáveis locais
   VTPerfilBarra *perfil_barra;
   VTRes505      *res505  = (VTRes505*)apl->GetObject(__classid(VTRes505));

   for (int n = 0; n < lisPERFIL_BAR->Count; n++)
      {
      perfil_barra = (VTPerfilBarra*)lisPERFIL_BAR->Items[n];
      //calcula índices de  qualidade da Barra
      perfil_barra->CalculaIndicesQualidadeUmDia(res505, ind_dia);
      }
   }
*/ /*
//---------------------------------------------------------------------------
VTPerfilBarra* __fastcall  TPerfilRede::ExistePerfilBarra(VTBarra *barra)
   {
   //variáveis locais
   VTPerfilBarra *perfil_barra;

   for (int n = 0; n < lisPERFIL_BAR->Count; n++)
      {
      perfil_barra = (VTPerfilBarra*)lisPERFIL_BAR->Items[n];
      if (perfil_barra->Barra == barra) return(perfil_barra);
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTPerfilCarga* __fastcall TPerfilRede::ExistePerfilCarga(VTCarga *carga)
   {
   //variáveis locais
   VTPerfilCarga *perfil_carga;

   for (int n = 0; n < lisPERFIL_CAR->Count; n++)
      {
      perfil_carga = (VTPerfilCarga*)lisPERFIL_CAR->Items[n];
      if (perfil_carga->Carga == carga) return(perfil_carga);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTPerfilRegulador* __fastcall TPerfilRede::ExistePerfilReg(VTRegulador *reg)
   {
   //variáveis locais
   VTPerfilRegulador *perfil_reg;

   for (int n = 0; n < lisPERFIL_REG->Count; n++)
      {
      perfil_reg = (VTPerfilRegulador*)lisPERFIL_REG->Items[n];
      if (perfil_reg->Reg == reg) return(perfil_reg);
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTPerfilSup* __fastcall TPerfilRede::ExistePerfilSup(VTSuprimento *sup)
   {
   //variáveis locais
   VTPerfilSup *perfil_sup;

   for (int n = 0; n < lisPERFIL_SUP->Count; n++)
      {
      perfil_sup = (VTPerfilSup*)lisPERFIL_SUP->Items[n];
      if (perfil_sup->Suprimento == sup) return(perfil_sup);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
TList* __fastcall  TPerfilRede::LisPerfilBarra(void)
   {
   return(lisPERFIL_BAR);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilRede::PM_GetEnabled(void)
   {
   return(lisPERFIL_BAR->Count > 0);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TPerfilRede::Reinicia(void)
   {
//   LimpaTList(lisPERFIL_BAR);
   LimpaTList(lisPERFIL_CAR);
//   LimpaTList(lisPERFIL_REG);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilRede::RestauraPerfilCarga(TDateTime data)
   {
   //variáveis locais
   VTCarga         *carga;
   VTPerfilCarga *perfil_carga;
   VTRedes         *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia lisEQP com Cargas
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCARGA);
   //loop p/ todas Cargas
   for (int nc = 0; nc < lisEQP->Count; nc++)
      {
      carga = (VTCarga*)lisEQP->Items[nc];
      //atualiza curva de demanda das Cargas com as demandas salvas em PerfilDemanda
      if ((perfil_carga = ExistePerfilCarga(carga)) == NULL) continue;
      perfil_carga->AtualizaDemandaDaCarga(data);
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TPerfilRede::SalvaPerfilBarra(TDateTime data)
   {
   //variáveis locais
   VTPerfilBarra *perfil_barra;
   VTBarra       *barra;
   VTMedicao     *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTRedes       *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList         *lisBAR = redes->LisBarra();

   //loop p/ todas Barras da rede secundária
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //if (! IsDoubleZero(barra->zona->Vff_kv - res505->Vnom_ff_kv)) continue;
      //verifica se existe PerfilBarra da Barra
      if ((perfil_barra = ExistePerfilBarra(barra)) == NULL)
         {//cria PerfilBarra p/ a Barra
         perfil_barra = NewObjPerfilBarra(barra);
         lisPERFIL_BAR->Add(perfil_barra);
         }
      //salva tensões da barra
      perfil_barra->SalvaTensao(data, medicao);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TPerfilRede::SalvaPerfilCarga(TDateTime data)
   {
   //variáveis locais
   VTCarga           *carga;
   VTPerfilCarga     *perfil_carga;
   VTMedicao           *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTRedes             *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia lisEQP com Reguladores
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCARGA);
   //loop p/ todas Cargas
   for (int nc = 0; nc < lisEQP->Count; nc++)
      {
      carga = (VTCarga*)lisEQP->Items[nc];
      //verifica se existe PerfilCarga para a Carga
      if ((perfil_carga = ExistePerfilCarga(carga)) == NULL)
         {//cria PerfilCarga
         perfil_carga = NewObjPerfilCarga(apl, carga);
         lisPERFIL_CAR->Add(perfil_carga);
         }
      //salva demanda da Carga
      perfil_carga->SalvaDemanda(data, medicao);
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TPerfilRede::SalvaPerfilRegulador(TDateTime data)
   {
   //variáveis locais
   VTMedicao           *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTRedes             *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTPerfilRegulador *perfil_reg;
   VTRegulador         *reg;

   //inicia lisEQP com Reguladores
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoREGULADOR);
   if (lisEQP->Count == 0) {return(false);}
   reg = (VTRegulador*)lisEQP->Items[0];
   //verifica se existe PerfilRegulador do Regulador
   if ((perfil_reg = ExistePerfilReg(reg)) == NULL)
      {//cria PerfilRegulador p/ o Regulador
      perfil_reg = NewObjPerfilRegulador(reg);
      lisPERFIL_REG->Add(perfil_reg);
      }
   //salva carregamento do regulador
   perfil_reg->SalvaCarreg(data, medicao);
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TPerfilRede::SalvaPerfilSuprimento(TDateTime data)
   {
   //variáveis locais
   VTMedicao         *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTRedes             *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTPerfilSup       *perfil_sup;
   VTSuprimento       *suprimento;

   //inicia lisEQP
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoSUPRIMENTO);
   if (lisEQP->Count == 0) {return(false);}
   //loop p/ todos Suprimentos
   for (int ns = 0; ns < lisEQP->Count; ns++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[ns];
      //verifica se existe PerfilSup do Suprimento
      if ((perfil_sup = ExistePerfilSup(suprimento)) == NULL)
         {//cria PerfilSup
         perfil_sup = NewObjPerfilSup(apl, suprimento);
         lisPERFIL_SUP->Add(perfil_sup);
         }
      //salva tensão
      perfil_sup->SalvaTensao(data, medicao);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

