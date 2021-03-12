//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaArranjo.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Arranjo\VTArranjo.h"
//#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaArranjo(VTApl *apl)
   {
   try{
      return (new TMontaArranjo(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaArranjo::TMontaArranjo(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   fases = (VTFases*)apl->GetObject(__classid(VTFases));
   }

//---------------------------------------------------------------------------
__fastcall TMontaArranjo::~TMontaArranjo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaArranjo::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTGrupo *grupo1, *grupo2;

   //salva ponteiro p/ Arranjo
   arranjo = (VTArranjo*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   /*
   //cria Grupo2 c/ dados do Arranjo
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      }
   */
   //inicia Grupo1 c/ dados do Eqpto
   LisValoresEqpto(eqpto, grupo1->LisDado, ano, num_pat);
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArranjo::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Barra
   this->arranjo = (VTArranjo*)eqpto;
   //insere atributos de Eqpto
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de Fluxo
   LisAtributosFluxo(eqpto, lisATR);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArranjo::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Barra
   this->arranjo = (VTArranjo*)eqpto;
   //insere atributos de Eqpto
   TMontaEqpto::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Arranjo
   lisATR->Add(new TAtributo(arranjo, arr_CFG,  true,  false, "Configuração"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArranjo::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //nada a fazer

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArranjo::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   //LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArranjo::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->arranjo  = (VTArranjo*)eqpto;
   try{//insere dados do Eqpto
      TMontaEqpto::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do Arranjo
      //if (arranjo->Show[arr_CFG])  ValorDadosConfiguracao();
      ValorDadosConfiguracao();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }


//---------------------------------------------------------------------------
void __fastcall TMontaArranjo::ValorDadosConfiguracao(void)
   {
   //variáveis locais
   VTDado     *dado;
   /*
   //código: montado por TMontaEqpto
   lisEXT->Add(dado = NewObjDado("Código"));
   if (arranjo->Owner == arranjos)
      {dado->valor = arranjo->Codigo;}
   else
      {dado->valor = "próprio";}
   */
   //fases
   lisEXT->Add(dado = NewObjDado("Fases"));
   dado->valor = fases->AsStrUpper[arranjo->Fases];
   //tipo de configuração
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE: ValorTipoArranjoCABOxFASE(); break;
      case arranjoCABO_Z0Z1: ValorTipoArranjoCABO_Z0Z1(); break;
      case arranjoZ0Z1:      ValorTipoArranjoZ0Z1() ;     break;
      case arranjoMAT_Z:     ValorTipoArranjoMAT_Z();     break;
      case arranjoPU_SB100:  ValorTipoArranjoPU_SB100();  break;
      }
   //
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArranjo::ValorTipoArranjoCABOxFASE(void)
   {
   //variáveis locais
   VTDado *dado;
   VTCabo *cabo;

   //tipo de configuração
   lisEXT->Add(dado = NewObjDado("Tipo"));
   dado->valor = "Suporte+Cabos";
   //fases
   lisEXT->Add(dado = NewObjDado("Fases"));
   dado->valor = fases->AsStrABCN[arranjo->Fases];
   //Iadm (A)
   lisEXT->Add(dado = NewObjDado("Iadm"));
   aue.Corrente_A(arranjo->Iadm_amp, dado->valor, dado->unidade);
   //suporte
   lisEXT->Add(dado = NewObjDado("Suporte"));
   dado->valor = arranjo->suporte->Codigo;
   //cabos
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      if ((cabo = arranjo->fase[nf].cabo) == NULL) continue;
      lisEXT->Add(dado = NewObjDado("Cabo fase " + fases->AsStrUpper[fases->IndexToTag[nf]]));
      dado->valor = cabo->Codigo;
      switch(cabo->TipoCfg)
         {
         case caboR_GMR:
            //resistencia (ohm/km)
            lisEXT->Add(dado = NewObjDado("R"));
            dado->valor = DoubleToStr("%5.4", cabo->r);
            dado->unidade = ("ohm/km");
            //gmr (mm)
            lisEXT->Add(dado = NewObjDado("GMR"));
            dado->valor = DoubleToStr("%4.3", cabo->gmr);
            dado->unidade = ("mm");
            break;
         case caboZ0Z1:
         case caboPRE_REUNIDO:
            //Z0 em ohm/km
            lisEXT->Add(dado = NewObjDado("Zseq.0"));
            aue.Impedancia_OHM_KM(arranjo->caboZ0Z1->z0, dado->valor, dado->unidade);
            //Z1em ohm/km
            lisEXT->Add(dado = NewObjDado("Zseq.1"));
            aue.Impedancia_OHM_KM(arranjo->caboZ0Z1->z1, dado->valor, dado->unidade);
            //C0 em nf/km
            lisEXT->Add(dado = NewObjDado("Cseq.0"));
            aue.Capacitancia_NF_KM(arranjo->caboZ0Z1->c0, dado->valor, dado->unidade);
            //C1 em nf/km
            lisEXT->Add(dado = NewObjDado("Cseq.1"));
            aue.Capacitancia_NF_KM(arranjo->caboZ0Z1->c1, dado->valor, dado->unidade);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArranjo::ValorTipoArranjoCABO_Z0Z1(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   dado->valor = "Cabo(Z0,Z1,C0,C1)";
   //Iadm (A)
   lisEXT->Add(dado = NewObjDado("Iadm"));
   aue.Corrente_A(arranjo->Iadm_amp, dado->valor, dado->unidade);
   //suporte
   if (arranjo->suporte)
      {
      lisEXT->Add(dado = NewObjDado("Suporte"));
      dado->valor = arranjo->suporte->Codigo;
      }
   //Cabo
   lisEXT->Add(dado = NewObjDado("Cabo"));
   dado->valor = arranjo->caboZ0Z1->Codigo;
   //Z0 em ohm/km
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_OHM_KM(arranjo->caboZ0Z1->z0, dado->valor, dado->unidade);
   //Z1em ohm/km
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_OHM_KM(arranjo->caboZ0Z1->z1, dado->valor, dado->unidade);
   //C0 em nf/km
   lisEXT->Add(dado = NewObjDado("Cseq.0"));
   aue.Capacitancia_NF_KM(arranjo->caboZ0Z1->c0, dado->valor, dado->unidade);
   //C1 em nf/km
   lisEXT->Add(dado = NewObjDado("Cseq.1"));
   aue.Capacitancia_NF_KM(arranjo->caboZ0Z1->c1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArranjo::ValorTipoArranjoMAT_Z(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   dado->valor = "Matriz Z";
   //Iadm (A)
   lisEXT->Add(dado = NewObjDado("Iadm"));
   aue.Corrente_A(arranjo->Iadm_amp, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArranjo::ValorTipoArranjoPU_SB100(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   dado->valor = "Z0,Z1,C0,C1(total)";
   //Iadm (A)
   lisEXT->Add(dado = NewObjDado("Iadm"));
   aue.Corrente_A(arranjo->Iadm_amp, dado->valor, dado->unidade);
   //Z0 em pu Sb=100
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_PU100(arranjo->z0, dado->valor, dado->unidade);
   //Z1 em pu Sb=100
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_PU100(arranjo->z1, dado->valor, dado->unidade);
   //C0 em pu Sb=100
   lisEXT->Add(dado = NewObjDado("Cseq.0"));
   aue.Capacitancia_PU100(arranjo->c0, dado->valor, dado->unidade);
   //C1 em pu Sb=100
   lisEXT->Add(dado = NewObjDado("Cseq.1"));
   aue.Capacitancia_PU100(arranjo->c1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArranjo::ValorTipoArranjoZ0Z1(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   dado->valor = "(Z0,Z1,C0,C1)/km";
   //Iadm (A)
   lisEXT->Add(dado = NewObjDado("Iadm"));
   aue.Corrente_A(arranjo->Iadm_amp, dado->valor, dado->unidade);
   //Z0 em ohm/km
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_OHM_KM(arranjo->z0, dado->valor, dado->unidade);
   //Z1em ohm/km
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_OHM_KM(arranjo->z1, dado->valor, dado->unidade);
   //C0 em nf/km
   lisEXT->Add(dado = NewObjDado("Cseq.0"));
   aue.Capacitancia_NF_KM(arranjo->c0, dado->valor, dado->unidade);
   //C1 em nf/km
   lisEXT->Add(dado = NewObjDado("Cseq.1"));
   aue.Capacitancia_NF_KM(arranjo->c1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
//eof

