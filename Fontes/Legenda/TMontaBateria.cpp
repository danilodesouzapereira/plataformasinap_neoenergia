//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaBateria.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTResFlowBar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaBateria(VTApl *apl)
   {
   try{
	  return (new TMontaBateria(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaBateria::TMontaBateria(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   }

//---------------------------------------------------------------------------
__fastcall TMontaBateria::~TMontaBateria(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaBateria::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTGrupo *grupo1, *grupo2;

   //salva ponteiro p/ bateria
   bateria = (VTBateria*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados da Carga
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      //iinswere dados da curva de demanda
	  LisVetValEqpto(eqpto, grupo2->LisVetval, car_P);
      LisVetValEqpto(eqpto, grupo2->LisVetval, car_Q);
      }

   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Bateria
   bateria = (VTBateria*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Carga
   bateria = (VTBateria*)eqpto;
   //atributos de Eqpto
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   lisATR->Add(new TAtributo(bateria, bat_P,        false, true,  "Pot.Ativa",    "(MW)"));
   lisATR->Add(new TAtributo(bateria, bat_Q,        false, true,  "Pot.Reativa",  "(MVAr)"));
   //atributo de fluxo
   // preciso descobrir se preciso disso
   //lisATR->Add(new TAtributo(bateria, car_DRP_DRC,  true, false,  "DRP,DRC",      "(%)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Carga
   bateria = (VTBateria*)eqpto;
   //atributos de fluxo do Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->bateria = (VTBateria*)eqpto;
   try{//dados da Curva
	  if (bateria->Show[bat_P])       ValorPotAtiva();
	  if (bateria->Show[bat_Q])       ValorPotReativa();
}catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->bateria   = (VTBateria*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //atributos de Carga
	}catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->bateria  = (VTBateria*)eqpto;
   this->lisEXT = lisEXT;
   this->ano     = ano;
   this->num_pat = num_pat;
   try{//insere dados do Eqpto
	  TMontaEqpto::LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
      //dados de fluxo
	  if (bateria->Show[bat_P])       ValorPotAtiva();
	  if (bateria->Show[bat_Q])       ValorPotReativa();
	}catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->bateria   = (VTBateria*)eqpto;
   this->lisEXT  = lisEXT;
   this->ano     = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
	  {
	  case bat_P: VetValorPotAtiva(ind_atrib);    break;
	  case bat_Q: VetValorPotReativa(ind_atrib);  break;
	  }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaBateria::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaBateria::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor;
   VTBateria    *bateria   = (VTBateria*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:      valor = IntToStr(eqpto->Id);                          break;
      case eqp_CODIGO:  valor = eqpto->Codigo;                                break;

	  }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBateria::ValorDRP_DRC(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("DRP"));
   //define unidade e valor
   aue.ValorPorcentual(bateria->pbarra->resflow->DRP * 100, dado->valor, dado->unidade, "%3.2f");
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("DRC"));
   //define unidade e valor
   aue.ValorPorcentual(bateria->pbarra->resflow->DRC * 100, dado->valor, dado->unidade, "%3.2f");
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBateria::ValorFases(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Fases"));
   //define unidade e valor
   dado->valor    = fases->AsStrUpper[bateria->Fases];
   dado->unidade  = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBateria::ValorModelo(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Modelo"));
   //define unidade e valor
   dado->unidade  = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBateria::ValorPotAtiva(void)
   {
   //variáveis locais
   VTDado *dado;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("P"));
   //define unidade e valor
   aue.PotAtiva_MW(demanda->P_mw[bateria][num_pat], dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBateria::ValorPotReativa(void)
   {
   //variáveis locais
   VTDado    *dado;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //define unidade e valor
   aue.PotReativa_MVAR(demanda->Q_mvar[bateria][num_pat], dado->valor, dado->unidade);
   }

/*//---------------------------------------------------------------------------
void __fastcall TMontaBateria::ValorPotAparente(void)
   {
   //variáveis locais
   VTDado    *dado;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("S"));
   //define unidade e valor
   aue.PotAparente_MVA(Abs(demanda->S_mva[bateria][num_pat]), dado->valor, dado->unidade);
   }
 */
/*//---------------------------------------------------------------------------
void __fastcall TMontaBateria::VetValorPotAparente(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   complex<double> s_mva;
   VTVetval        *vetval;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "S", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
	  {
	  s_mva = demanda->S_mva[bateria][np];
	  //preenche valor como string
	  vetval->Valor[np]      = Abs(aue.PotAparente_MVA(s_mva, val_str, vetval->unidade));
	  vetval->ValorAsStr[np] = val_str;
	  }
   }
  */
//---------------------------------------------------------------------------
void __fastcall TMontaBateria::VetValorPotAtiva(int ind_atrib)
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
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "P", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = demanda->P_mw[bateria][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaBateria::VetValorPotReativa(int ind_atrib)
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
      vetval->Valor[np] = demanda->Q_mvar[bateria][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof

