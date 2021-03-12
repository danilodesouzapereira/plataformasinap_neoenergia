//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaMutua.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaMutua(VTApl *apl)
   {
   try{
      return (new TMontaMutua(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaMutua::TMontaMutua(VTApl *apl)
                        :TMontaEqpto()
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaMutua::~TMontaMutua(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaMutua::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrupo *grupo1, *grupo2;
   TList   *lisGRUPO;

   //salva parâmetros
   mutua = (VTMutua*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados do Eqpto
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMutua::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   mutua = (VTMutua*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMutua::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   mutua = (VTMutua*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Mutua
   lisATR->Add(new TAtributo(mutua, mut_TRECHO, true, false, "Trechos"));
   lisATR->Add(new TAtributo(mutua, mut_Z0,     true, true,  "Zseq.0"));
   lisATR->Add(new TAtributo(mutua, mut_Z1,     true, true,  "Zseq.1"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMutua::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->mutua = (VTMutua*)eqpto;
   //atributos de fluxo de Mutua

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMutua::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMutua::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->mutua     = (VTMutua*)eqpto;
   try{//dados do Eqpto
      TMontaEqpto::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados gerais do mutua
      if (mutua->Show[mut_TRECHO]) ValorCodigoTrechos();
      if (mutua->Show[mut_Z0])     ValorImpedZ0();
      if (mutua->Show[mut_Z1])     ValorImpedZ1();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaMutua::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   return(TMontaEqpto::ValorAtributo(eqpto, ind_atrib));
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMutua::ValorCodigoTrechos(void)
   {
   //variáveis locais
   VTDado *dado;
   VTTrecho *trecho[2] = {mutua->ptre1, mutua->ptre2};

   for (int n = 0; n < sizeof(trecho)/sizeof(VTTrecho*); n++)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Trecho" + IntToStr(n+1)));
      //define unidade e valor
      dado->valor   = trecho[n]->Codigo;
      dado->unidade = "";
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMutua::ValorImpedZ0(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_OHM_KM(mutua->z0, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMutua::ValorImpedZ1(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_OHM_KM(mutua->z1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
//eof



