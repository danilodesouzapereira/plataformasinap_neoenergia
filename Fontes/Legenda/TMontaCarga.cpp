//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaCarga.h"
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
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTResFlowBar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaCarga(VTApl *apl)
   {
   try{
      return (new TMontaCarga(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaCarga::TMontaCarga(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   }

//---------------------------------------------------------------------------
__fastcall TMontaCarga::~TMontaCarga(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaCarga::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   VTGrupo *grupo1, *grupo2;

   //salva ponteiro p/ Capacitor
   carga = (VTCarga*)eqpto;
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
      LisVetValEqpto(eqpto, grupo2->LisVetval, car_S);
      LisVetValEqpto(eqpto, grupo2->LisVetval, car_P);
      LisVetValEqpto(eqpto, grupo2->LisVetval, car_Q);
      }
   /*
   //cria Grupo2 c/ dados de curva típica
   if ((grupo2 = NewObjGrupo(eqpto, "Curva de potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados da curva
      LisVetValFluxo(eqpto, grupo2->LisVetval, car_S, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, car_P, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, car_Q, ano);
      }
   */
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Carga
   carga = (VTCarga*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Carga
   carga = (VTCarga*)eqpto;
   //atributos de Eqpto
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   lisATR->Add(new TAtributo(carga, car_CLASSE,   true, false,  "Classe"));
   lisATR->Add(new TAtributo(carga, car_NUMCONS,  true, false,  "Consumidores"));
   lisATR->Add(new TAtributo(carga, car_ENERGIA,  true, false,  "Energia",      "(kWh/mês)"));
   lisATR->Add(new TAtributo(carga, car_FASES,    true, false,  "Fases"));
   lisATR->Add(new TAtributo(carga, car_MODELO,   true, false,  "Modelo"));
   lisATR->Add(new TAtributo(carga, car_P,        false, true,  "Pot.Ativa",    "(MW)"));
   lisATR->Add(new TAtributo(carga, car_Q,        false, true,  "Pot.Reativa",  "(MVAr)"));
   lisATR->Add(new TAtributo(carga, car_S,        false, true,  "Pot.Aparente", "(MVA)"));
   //atributo de fluxo
   lisATR->Add(new TAtributo(carga, car_DRP_DRC,  true, false,  "DRP,DRC",      "(%)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Carga
   carga = (VTCarga*)eqpto;
   //atributos de fluxo do Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
   /*
   //atributos de fluxo de Carga
   lisATR->Add(new TAtributo(carga, car_P, false, true,   "Pot.Ativa",    "(MW)"));
   lisATR->Add(new TAtributo(carga, car_Q, false, true,   "Pot.Reativa",  "(MVAr)"));
   lisATR->Add(new TAtributo(carga, car_S, false, true,   "Pot.Aparente", "(MVA)"));
   */
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
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
bool __fastcall TMontaCarga::LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->carga = (VTCarga*)eqpto;
   try{//dados da Curva
      if (carga->Show[car_P])       ValorPotAtiva();
      if (carga->Show[car_Q])       ValorPotReativa();
      if (carga->Show[car_S])       ValorPotAparente();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->carga   = (VTCarga*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //atributos de Carga
      if (carga->Show[car_CLASSE])  ValorClasse();
      if (carga->Show[car_ENERGIA]) ValorEnergia();
      if (carga->Show[car_NUMCONS]) ValorNumConsumidores();
      if (carga->Show[car_FASES])   ValorFases();
      if (carga->Show[car_MODELO])  ValorModelo();
      if (carga->Show[car_DRP_DRC]) ValorDRP_DRC();
      //if (carga->Show[car_P])       ValorPotAtiva();
      //if (carga->Show[car_Q])       ValorPotReativa();
      //if (carga->Show[car_S])       ValorPotAparente();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->carga  = (VTCarga*)eqpto;
   this->lisEXT = lisEXT;
   this->ano     = ano;
   this->num_pat = num_pat;
   try{//insere dados do Eqpto
      TMontaEqpto::LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
      //dados de fluxo
      if (carga->Show[car_P])       ValorPotAtiva();
      if (carga->Show[car_Q])       ValorPotReativa();
      if (carga->Show[car_S])       ValorPotAparente();
      if (carga->Show[car_DRP_DRC]) ValorDRP_DRC();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->carga   = (VTCarga*)eqpto;
   this->lisEXT  = lisEXT;
   this->ano     = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case car_P: VetValorPotAtiva(ind_atrib);    break;
      case car_Q: VetValorPotReativa(ind_atrib);  break;
      case car_S: VetValorPotAparente(ind_atrib); break;
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->carga   = (VTCarga*)eqpto;
   this->lisEXT  = lisEXT;
   this->ano     = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case car_P: VetValorPotAtiva(ind_atrib);    break;
      case car_Q: VetValorPotReativa(ind_atrib);  break;
      case car_S: VetValorPotAparente(ind_atrib); break;
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaCarga::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaCarga::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   double      energia;
   AnsiString  valor, unidade;
   VTCarga    *carga   = (VTCarga*)eqpto;
   VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:      valor = IntToStr(eqpto->Id);                          break;
      case eqp_CODIGO:  valor = eqpto->Codigo;                                break;
      case car_P:       aue.PotAtiva_MW(demanda->P_mw[carga][num_pat], valor, unidade);           break;
      case car_Q:       aue.PotReativa_MVAR(demanda->Q_mvar[carga][num_pat], valor, unidade);     break;
      case car_S:       aue.PotAparente_MVA(Abs(demanda->S_mva[carga][num_pat]), valor, unidade); break;;
      case car_ENERGIA:
         if (carga->curva->Tipica) energia = carga->Energia_mwhmes;
         else                      energia = demanda->Emes_mwh[carga];
         aue.Energia_MWH(energia, valor, unidade);
         break;
      case car_NUMCONS: valor = IntToStr(carga->NumConsTotal);                break;
      case car_FASES:   valor = fases->AsStrUpper[carga->Fases];              break;
      case car_MODELO:  aue.ModeloCarga(carga->ModeloCarga, valor, unidade);  break;
      case car_CLASSE:  valor   = carga->classe->Codigo;                      break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorClasse(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Classe"));
   //define unidade e valor
   dado->valor   = carga->classe->Codigo;
   dado->unidade = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorDRP_DRC(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("DRP"));
   //define unidade e valor
   aue.ValorPorcentual(carga->pbarra->resflow->DRP * 100, dado->valor, dado->unidade, "%3.2f");
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("DRC"));
   //define unidade e valor
   aue.ValorPorcentual(carga->pbarra->resflow->DRC * 100, dado->valor, dado->unidade, "%3.2f");
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16972670
void __fastcall TMontaCarga::ValorEnergia(void)
   {
   //variáveis locais
   double     energia;
   VTDado     *dado;
   VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //determina energia mensal da Carga
   if ((carga->curva->Tipica) && (! carga->IP))
      energia = carga->Energia_mwhmes;
   else
      energia = demanda->Emes_mwh[carga];
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Energia"));
   //define unidade e valor
   aue.Energia_MWH(energia, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorFases(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Fases"));
   //define unidade e valor
   dado->valor    = fases->AsStrUpper[carga->Fases];
   dado->unidade  = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorModelo(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Modelo"));
   //define unidade e valor
   aue.ModeloCarga(carga->ModeloCarga, dado->valor, dado->unidade);
   dado->unidade  = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorNumConsumidores(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Consumidores"));
   //define unidade e valor
   dado->valor    = carga->NumConsTotal;
   dado->unidade  = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorPotAtiva(void)
   {
   //variáveis locais
   VTDado *dado;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("P"));
   //define unidade e valor
   aue.PotAtiva_MW(demanda->P_mw[carga][num_pat], dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorPotReativa(void)
   {
   //variáveis locais
   VTDado    *dado;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //define unidade e valor
   aue.PotReativa_MVAR(demanda->Q_mvar[carga][num_pat], dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::ValorPotAparente(void)
   {
   //variáveis locais
   VTDado    *dado;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("S"));
   //define unidade e valor
   aue.PotAparente_MVA(Abs(demanda->S_mva[carga][num_pat]), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::VetValorPotAparente(int ind_atrib)
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
      s_mva = demanda->S_mva[carga][np];
      //preenche valor como string
      vetval->Valor[np]      = Abs(aue.PotAparente_MVA(s_mva, val_str, vetval->unidade));
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::VetValorPotAtiva(int ind_atrib)
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
      vetval->Valor[np] = demanda->P_mw[carga][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCarga::VetValorPotReativa(int ind_atrib)
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
      vetval->Valor[np] = demanda->Q_mvar[carga][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof

