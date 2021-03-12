//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Zona\VTZona.h>
#include <DLL_INC\Funcao.h>
#include "TPerfilAlt.h"
#include "VTPerfilBarra.h"
#include "VTPerfilRegulador.h"
#include "VTMedicao.h"
#include "VTRes505.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerfilAlt*  __fastcall NewObjPerfilAlt(VTApl *apl)
   {
   try{
      return(new TPerfilAlt(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPerfilAlt::TPerfilAlt(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   lisEQP        = new TList();
   lisPERFIL_BAR = new TList();
   lisPERFIL_REG = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPerfilAlt::~TPerfilAlt(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destrói lista e seus objetos
   if (lisPERFIL_BAR) {LimpaTList(lisPERFIL_BAR); delete lisPERFIL_BAR; lisPERFIL_BAR = NULL;}
   if (lisPERFIL_REG) {LimpaTList(lisPERFIL_REG); delete lisPERFIL_REG; lisPERFIL_REG = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TPerfilAlt::CalculaDRA_DRP_DRC(int limite_amostra)
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

//---------------------------------------------------------------------------
void __fastcall TPerfilAlt::CalculaIndicesQualidadeUmDia(int ind_dia)
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

//---------------------------------------------------------------------------
VTPerfilBarra* __fastcall  TPerfilAlt::ExistePerfilBarra(VTBarra *barra)
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

//---------------------------------------------------------------------------
VTPerfilRegulador* __fastcall TPerfilAlt::ExistePerfilReg(VTRegulador *reg)
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

//---------------------------------------------------------------------------
TList* __fastcall  TPerfilAlt::LisPerfilBarra(void)
   {
   return(lisPERFIL_BAR);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilAlt::PM_GetEnabled(void)
   {
   return(lisPERFIL_BAR->Count > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TPerfilAlt::Reinicia(void)
   {
   LimpaTList(lisPERFIL_BAR);
//   LimpaTList(lisPERFIL_CAR);
   LimpaTList(lisPERFIL_REG);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilAlt::SalvaPerfilBarra(TDateTime data)
   {
   //variáveis locais
   bool            padrao;
   VTBarra        *barra;
   VTEstudo         *estudo;
   VTPerfilBarra  *perfil_barra;
   VTMedicao      *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTRedes        *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList          *lisBAR = redes->LisBarra();

   //verifica se a alternativa ativa é a rede original //DVK 2015.04.01
   estudo = planejamento->ExisteEstudo();
   padrao = (estudo->AlternativaAtiva == estudo->AlternativaPadrao);
   //loop p/ todas Barras da rede
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
      perfil_barra->SalvaTensao(data, medicao, padrao);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerfilAlt::SalvaPerfilRegulador(TDateTime data)
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
   //loop p/ todos Reguladores da rede secundária
   for (int nr = 0; nr < lisEQP->Count; nr++)
      {
      reg = (VTRegulador*)lisEQP->Items[nr];
      //verifica se existe PerfilRegulador do Regulador
      if ((perfil_reg = ExistePerfilReg(reg)) == NULL)
         {//cria PerfilRegulador p/ o Regulador
         perfil_reg = NewObjPerfilRegulador(reg);
         lisPERFIL_REG->Add(perfil_reg);
         }
      //salva carregamento do regulador
      perfil_reg->SalvaCarreg(data, medicao);
      }
   return(true);
   }

//---------------------------------------------------------------------------
