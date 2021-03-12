//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaReator.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTReator.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaReator(VTApl *apl)
   {
   try{
      return (new TMontaReator(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaReator::TMontaReator(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaReator::~TMontaReator(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaReator::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTGrupo *grupo1, *grupo2;

   //salva ponteiro p/ Reator
   reator = (VTReator*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados do Reator
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      }
   //cria Grupo2 c/ dados de curva típica
   if ((grupo2 = NewObjGrupo(eqpto, "Curva de potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados da curva
      LisVetValFluxo(eqpto, grupo2->LisVetval, reat_Q, ano);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Reator
   reator = (VTReator*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ objeto
   reator = (VTReator*)eqpto;
   //atributos de Eqbar
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Reator
   lisATR->Add(new TAtributo(reator, reat_Q,    true, true,  "Pot.Reativa", "(MVAr)"));
   lisATR->Add(new TAtributo(reator, reat_LIGA, true, false, "Ligacao"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Reator
   reator = (VTReator*)eqpto;
   //atributos de fluxo de Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Reator

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores da Curva
   LisValoresCurva(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->reator = (VTReator*)eqpto;
   try{//dados da Curva
      if (reator->Show[reat_Q]) ValorPotenciaReativa();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->reator = (VTReator*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do Reator
      //if (reator->Show[reat_Q])      ValorPotenciaReativa();
      if (reator->Show[reat_LIGA])   ValorTipoLigacao();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //não há dados de fluxo
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaReator::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->reator = (VTReator*)eqpto;
   this->lisEXT    = lisEXT;
   this->ano       = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case reat_Q: VetValorPotReativa(ind_atrib);  break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaReator::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor, unidade;
   VTReator    *reator = (VTReator*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                           break;
      case eqp_CODIGO: valor = eqpto->Codigo;                                 break;
      case reat_Q:     valor = DoubleToStr("%2.1f", reator->Q_mvar);          break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaReator::ValorTipoLigacao(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Ligação"));
   //define unidade e valor
   dado->valor   = "Estrela";
   dado->unidade = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaReator::ValorPotenciaReativa(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //define unidade e valor
   aue.PotReativa_MVAR(reator->q, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaReator::VetValorPotReativa(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   int         max_pat;
   VTVetval    *vetval;
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Q", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = demanda->Q_mvar[reator][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof


