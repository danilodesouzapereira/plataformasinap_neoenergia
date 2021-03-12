//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaCapacitor.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaCapacitor(VTApl *apl)
   {
   try{
      return (new TMontaCapacitor(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaCapacitor::TMontaCapacitor(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaCapacitor::~TMontaCapacitor(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaCapacitor::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTGrupo *grupo1, *grupo2;

   //salva ponteiro p/ Capacitor
   capacitor = (VTCapacitor*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados do Capacitor
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
      LisVetValFluxo(eqpto, grupo2->LisVetval, cap_Q, ano);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Capacitor
   capacitor = (VTCapacitor*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Capacitor
   capacitor = (VTCapacitor*)eqpto;
   //atributos de Eqbar
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Capacitor
   lisATR->Add(new TAtributo(capacitor, cap_Q,    true, true,  "Pot.Reativa", "(MVAr)"));
   lisATR->Add(new TAtributo(capacitor, cap_LIGA, true, false, "Ligacao"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Capacitor
   capacitor = (VTCapacitor*)eqpto;
   //atributos de fluxo de Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Capacitor

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores da Curva
   //LisValoresCurva(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->capacitor = (VTCapacitor*)eqpto;
   try{//dados da Curva
      if (capacitor->Show[cap_Q]) ValorPotenciaReativa();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->capacitor = (VTCapacitor*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do Capacitor
      if (capacitor->Show[cap_Q])      ValorPotenciaReativa();
      if (capacitor->Show[cap_LIGA])   ValorTipoLigacao();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //não há dados de fluxo
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->capacitor = (VTCapacitor*)eqpto;
   this->lisEXT    = lisEXT;
   this->ano       = ano;
   //verifica atributo selecionado
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case cap_Q: VetValorPotReativa(ind_atrib);  break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapacitor::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->capacitor = (VTCapacitor*)eqpto;
   this->lisEXT    = lisEXT;
   this->ano       = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case cap_Q: VetValorPotReativa(ind_atrib);  break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaCapacitor::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor, unidade;
   VTCapacitor *capacitor = (VTCapacitor*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                         break;
      case eqp_CODIGO: valor = eqpto->Codigo;                               break;
      case eqb_BARRA:  valor = capacitor->pbarra->Codigo;                   break;
      case cap_Q:      valor = DoubleToStr("%5.4f", capacitor->Q_mvar);     break;
      case cap_LIGA:   aue.TipoLigacao(capacitor->ligacao, valor, unidade); break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCapacitor::ValorTipoLigacao(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Ligação"));
   //define unidade e valor
   aue.TipoLigacao(capacitor->ligacao, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCapacitor::ValorPotenciaReativa(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //define unidade e valor
   aue.PotReativa_MVAR(capacitor->q, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCapacitor::VetValorPotReativa(int ind_atrib)
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
      vetval->Valor[np] = demanda->Q_mvar[capacitor][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof


