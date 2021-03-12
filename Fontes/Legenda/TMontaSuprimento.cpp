//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaSuprimento.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaSuprimento(VTApl *apl)
   {
   try{
      return (new TMontaSuprimento(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaSuprimento::TMontaSuprimento(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaSuprimento::~TMontaSuprimento(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaSuprimento::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int      ano     = 0;
   int      num_pat = 0;
   unsigned show;
   VTGrupo  *grupo1, *grupo2;

   //salva ponteiro p/ Suprimento
   suprimento = (VTSuprimento*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados do Suprimento
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      //insere dados da curva de  fornecimento
      LisVetValEqpto(eqpto, grupo2->LisVetval, sup_PQVF);
      }
   /*
   //cria Grupo2 c/ dados de fornecimento
   if ((grupo2 = NewObjGrupo(eqpto, "Curva de potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados da curva
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_PQVF, ano);
      }
   */
   //cria Grupo2 c/ dados de fluxo de potência
   if ((grupo2 = NewObjGrupo(eqpto, "Fluxo de Potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //insere tipo do suprimento (usado no fluxo)
      show = eqpto->ShowAsUnsigned;
      eqpto->ShowAsUnsigned = sup_TIPO;
      LisValoresFluxo(eqpto, grupo2->LisDado, ano, num_pat);
      eqpto->ShowAsUnsigned = show;
      //inicia Grupo2 c/ dados da curva
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_S,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_S_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_S_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_S_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_P,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_P_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_P_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_P_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_Q,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_Q_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_Q_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_Q_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_I_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_I_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, sup_I_MIN, ano);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro para Suprimento
   suprimento = (VTSuprimento*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro para Suprimento
   suprimento = (VTSuprimento*)eqpto;
   //atributos de Eqpto
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos do Suprimento
   lisATR->Add(new TAtributo(suprimento, sup_TIPO,     true,  false, "Tipo de suprimento"));
   lisATR->Add(new TAtributo(suprimento, sup_SMAX,     true,  false, "Cap.máxima", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_Z_EQV,    true,  false, "Imp.Equivalente", "(pu)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro para Suprimento
   suprimento = (VTSuprimento*)eqpto;
   //atributos de fluxo de Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo do Suprimento
   if (suprimento->tiposup == supPQ)
      {lisATR->Add(new TAtributo(suprimento, sup_PQVF, false, true,  "PQ:Pot.Ativa/Pot.Reativa"));}
   else if (suprimento->tiposup == supPV)
      {lisATR->Add(new TAtributo(suprimento, sup_PQVF, false, true,  "PV:Mod.Tensão/Pot.Ativa"));}
   else if (suprimento->tiposup == supVF)
      {lisATR->Add(new TAtributo(suprimento, sup_PQVF, false, true,  "VF:Mod.Tensão/Âng.Tensão"));}
   //demais atributos
   lisATR->Add(new TAtributo(suprimento, sup_PIE,      false, false, "Cargto.(gráfico)"));
   lisATR->Add(new TAtributo(suprimento, sup_CARGTO,   false, true,  "Carregamento", "(%)"));
   lisATR->Add(new TAtributo(suprimento, sup_S,        false, true,  "Pot.Aparente", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_FP,       false, true,  "Fat.Potência", "(pu)"));
   lisATR->Add(new TAtributo(suprimento, sup_S_ABC,    false, true,  "Pot.Aparente por fase", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_FP_ABC,   false, true,  "Fat.Potência por fase", "(pu)"));
   lisATR->Add(new TAtributo(suprimento, sup_S_MAX,    false, true,  "Pot.Aparente máxima", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_S_MIN,    false, true,  "Pot.Aparente mínima", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_P,        false, true,  "Pot.Ativa", "(MW)"));
   lisATR->Add(new TAtributo(suprimento, sup_P_ABC,    false, true,  "Pot.Ativa por fase", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_P_MAX,    false, true,  "Pot.Ativa máxima", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_P_MIN,    false, true,  "Pot.Ativa mínima", "(MVA)"));
   lisATR->Add(new TAtributo(suprimento, sup_Q,        false, true,  "Pot.Reativa", "(MVAr)"));
   lisATR->Add(new TAtributo(suprimento, sup_Q_ABC,    false, true,  "Pot.Reativa por fase", "(MVAr)"));
   lisATR->Add(new TAtributo(suprimento, sup_Q_MAX,    false, true,  "Pot.Reativa máxima", "(MVAr)"));
   lisATR->Add(new TAtributo(suprimento, sup_Q_MIN,    false, true,  "Pot.Reativa mínima", "(MVAr)"));
   lisATR->Add(new TAtributo(suprimento, sup_I_ABC,    false, true,  "Corrente por fase", "(A)"));
   lisATR->Add(new TAtributo(suprimento, sup_I_MAX,    false, true,  "Corrente máxima", "(A)"));
   lisATR->Add(new TAtributo(suprimento, sup_I_MIN,    false, true,  "Corrente mínima", "(A)"));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //variáveis locais
   bool exibe_tipo;

   //salva parâmetros
   this->suprimento = (VTSuprimento*)eqpto;
   //salva opção de mostrar o tipo de suprimento
   exibe_tipo = eqpto->Show[sup_TIPO];
   //bloqueia opção  de mostrar o tipo de suprimento
   eqpto->Show[sup_TIPO] = false;
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores da Curva de potência
   LisValoresCurva(eqpto, lisEXT, ano, num_pat);
   //restaura opção de mostrar o tipo de suprimento
   eqpto->Show[sup_TIPO] = exibe_tipo;
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisValoresCurva(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat)
   {
   //salva parâmetros
   this->suprimento  = (VTSuprimento*)eqpto;
   try{//dados de Curva de potência
      if (suprimento->Show[sup_PQVF]) ValorCurvaPQ_PV_VF();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->suprimento = (VTSuprimento*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do Suprimento
      //if (suprimento->Show[sup_PQVF])    ValorPQ_PV_VF();
      if (suprimento->Show[sup_SMAX])    ValorCapacidade();
      if (suprimento->Show[sup_Z_EQV])   ValorZeqv();
      if (suprimento->Show[sup_TIPO])    ValorTipoSuprimento();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->suprimento  = (VTSuprimento*)eqpto;
   this->lisEXT      = lisEXT;
   try{//dados de fluxo
      if (suprimento->Show[sup_TIPO])    ValorTipoSuprimentoNoFluxo();
      if (suprimento->Show[sup_PIE])     ValorCargtoGrafico();
      if (suprimento->Show[sup_CARGTO])  ValorCarregamento();
      if (suprimento->Show[sup_S])       ValorPotAparenteTotal();
      if (suprimento->Show[sup_FP])      ValorFatPotenciaTotal();
      if (suprimento->Show[sup_S_ABC])   ValorPotAparentePorFase();
      if (suprimento->Show[sup_FP_ABC])  ValorFatPotenciaPorFase();
      if (suprimento->Show[sup_S_MAX])   ValorPotAparente_max();
      if (suprimento->Show[sup_S_MIN])   ValorPotAparente_min();
      if (suprimento->Show[sup_P])       ValorPotAtivaTotal();
      if (suprimento->Show[sup_P_ABC])   ValorPotAtivaPorFase();
      if (suprimento->Show[sup_P_MAX])   ValorPotAtiva_max();
      if (suprimento->Show[sup_P_MIN])   ValorPotAtiva_min();
      if (suprimento->Show[sup_Q])       ValorPotReativaTotal();
      if (suprimento->Show[sup_Q_ABC])   ValorPotReativaPorFase();
      if (suprimento->Show[sup_Q_MAX])   ValorPotReativa_max();
      if (suprimento->Show[sup_Q_MIN])   ValorPotReativa_min();
      if (suprimento->Show[sup_I_ABC])   ValorCorrentePorfase();
      if (suprimento->Show[sup_I_MAX])   ValorCorrente_max();
      if (suprimento->Show[sup_I_MIN])   ValorCorrente_min();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->suprimento = (VTSuprimento*)eqpto;
   this->lisEXT     = lisEXT;
   this->ano        = 0;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case sup_PQVF: VetValorEqptoPQVF(ind_atrib); break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaSuprimento::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->suprimento = (VTSuprimento*)eqpto;
   this->lisEXT  = lisEXT;
   this->ano     = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case sup_CARGTO: VetValorFluxoCargto(ind_atrib);        break;
      case sup_PQVF:   VetValorFluxoCurvaPQ_PV_VF(ind_atrib); break;
      case sup_S:      VetValorFluxoS(ind_atrib);             break;
      case sup_FP:     VetValorFluxoFP(ind_atrib);            break;
      case sup_S_ABC:  VetValorFluxoSf(ind_atrib);            break;
      case sup_FP_ABC: VetValorFluxoFPf(ind_atrib);            break;
      case sup_S_MAX:  VetValorFluxoSf_max(ind_atrib);        break;
      case sup_S_MIN:  VetValorFluxoSf_min(ind_atrib);        break;
      case sup_P:      VetValorFluxoP(ind_atrib);             break;
      case sup_P_ABC:  VetValorFluxoPf(ind_atrib);            break;
      case sup_P_MAX:  VetValorFluxoPf_max(ind_atrib);        break;
      case sup_P_MIN:  VetValorFluxoPf_min(ind_atrib);        break;
      case sup_Q:      VetValorFluxoQ(ind_atrib);             break;
      case sup_Q_ABC:  VetValorFluxoQf(ind_atrib);            break;
      case sup_Q_MAX:  VetValorFluxoQf_max(ind_atrib);        break;
      case sup_Q_MIN:  VetValorFluxoQf_min(ind_atrib);        break;
      case sup_I_ABC:  VetValorFluxoIf(ind_atrib);            break;
      case sup_I_MAX:  VetValorFluxoIf_min(ind_atrib);        break;
      case sup_I_MIN:  VetValorFluxoIf_max(ind_atrib);        break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaSuprimento::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString    valor, unidade;
   VTSuprimento  *suprimento = (VTSuprimento*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                              break;
      case eqp_CODIGO: valor = eqpto->Codigo;                                    break;
      case sup_TIPO:   aue.TipoSuprimento(suprimento->tiposup, valor, unidade);  break;
      case sup_SMAX:   valor = DoubleToStr("%2.1f", suprimento->smax);           break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCapacidade(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smax"));
   //define unidade e valor
   aue.PotAparente_MVA(suprimento->smax, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCarregamento(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Cargto"));
   //define unidade e valor
   aue.Carregamento_pu(suprimento->resflow->Cargto_pu[num_pat], dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCargtoGrafico(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCorrentePorfase(void)
   {
   //variáveis locais
   complex<double> i;
   VTDado          *dado;
   char           *fase[] = {"Ia", "Ib", "Ic"};

   //correntes de fase em A
   for (int nf = 0; nf < sizeof(fase)/sizeof(char*); nf++)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado(fase[nf]));
      //determina corrente na fase da Barra selecionada
      i  = suprimento->resflow->If_amp[num_pat][nf];
      //define unidade e valor
      aue.Corrente_A(i, dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("In"));
   //determina corrente no neutro da Barra selecionada
   i  = suprimento->resflow->Sn_mva[num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCorrente_max(void)
   {
   //variáveis locais
   double i;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Imax"));
   //determina corrente máxima em relação à Barra c/ Pot.Ativa positiva
   i  = suprimento->resflow->If_amp_max[num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCorrente_min(void)
   {
   //variáveis locais
   double i;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Imin"));
   //determina corrente mínima em relação à Barra c/ Pot.Ativa positiva
   i  = suprimento->resflow->If_amp_min[num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorCurvaPQ_PV_VF(void)
   {
   //variáveis locais
   double         p, q, v, t;
   VTDado         *dado;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   if (suprimento->tiposup == supPV)
      {
      p = fornecimento->P_mw[suprimento][num_pat];
      v = fornecimento->V_pu[suprimento][num_pat];
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("P"));
      aue.PotAtiva_MW(p, dado->valor, dado->unidade);
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("V"));
      aue.Tensao_PU(v, dado->valor, dado->unidade);
      }
   else if (suprimento->tiposup == supVF)
      {
      v = fornecimento->V_pu[suprimento][num_pat];
      t = fornecimento->T_rad[suprimento][num_pat];
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("VF"));
      aue.Tensao_PU(complex<double>(v,t), dado->valor, dado->unidade);
      }
   else if (suprimento->tiposup == supPQ)
      {
      p = fornecimento->P_mw[suprimento][num_pat];
      q = fornecimento->Q_mvar[suprimento][num_pat];
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("PQ"));
      aue.PotAparente_MVA(complex<double>(p,q), dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorFatPotenciaTotal(void)
   {
   int PENDENCIA_ValorFatPotenciaTotal = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorFatPotenciaPorFase(void)
   {
   int PENDENCIA_ValorFatPotenciaPorFase = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAparenteTotal(void)
   {
   //variáveis locais
   complex<double> s;
   VTDado          *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("S"));
   //determina Pot.Aparente total na Barra selecionada
   s  = suprimento->resflow->S_mva[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(Abs(s), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAparentePorFase(void)
   {
   //variáveis locais
   complex<double> sf;
   VTDado          *dado;
   char           *fase[] = {"Sa", "Sb", "Sc"};

   //Pot.Aparente por fase
   for (int nf = 0; nf < sizeof(fase)/sizeof(char*); nf++)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado(fase[nf]));
      //determina Pot.Aparente na fase da Barra selecionada
      sf  = suprimento->resflow->Sf_mva[num_pat][nf];
      //define unidade e valor
      aue.PotAparente_MVA(Abs(sf), dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Sn"));
   //determina Pot.Aparente no neutro da Barra selecionada
   sf  = suprimento->resflow->Sn_mva[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(Abs(sf), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAparente_max(void)
   {
   //variáveis locais
   double s_max;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smax"));
   //determina Pot.Aparente máxima na Barra selecionada
   s_max  = suprimento->resflow->Sf_mva_max[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(Abs(s_max), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAparente_min(void)
   {
   //variáveis locais
   double s_min;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smin"));
   //determina Pot.Aparente mínima na Barra selecionada
   s_min  = suprimento->resflow->Sf_mva_min[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(Abs(s_min), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAtivaTotal(void)
   {
   //variáveis locais
   double p;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("P"));
   //determina Pot.Ativa total na Barra selecionada
   p = suprimento->resflow->P_mw[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAtivaPorFase(void)
   {
   //variáveis locais
   double p;
   VTDado  *dado;
   char   *fase[] = {"Pa", "Pb", "Pc"};

   //Pot.Ativa por fase
   for (int nf = 0; nf < sizeof(fase)/sizeof(char*); nf++)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado(fase[nf]));
      //determina Pot.Ativa na fase da Barra selecionada
      p = suprimento->resflow->Pf_mw[num_pat][nf];
      //define unidade e valor
      aue.PotAtiva_MW(p, dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pn"));
   //determina Pot.Ativa no neutro da Barra selecionada
   p = suprimento->resflow->Pn_mw[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAtiva_max(void)
   {
   //variáveis locais
   double p_max;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pmax"));
   //determina Pot.Ativa máxima na Barra selecionada
   p_max  = suprimento->resflow->Pf_mw_max[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_max, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotAtiva_min(void)
   {
   //variáveis locais
   double p_min;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pmin"));
   //determina Pot.Ativa mínima na Barra selecionada
   p_min  = suprimento->resflow->Pf_mw_min[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_min, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotReativaTotal(void)
   {
   //variáveis locais
   double q;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //determina Pot.Reaiva total na Barra selecionada
   q = suprimento->resflow->Q_mvar[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q, dado->valor, dado->unidade);
   }


//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotReativaPorFase(void)
   {
   //variáveis locais
   double q;
   VTDado  *dado;
   char   *fase[] = {"Qa", "Qb", "Qc"};

   //Pot.Reativa por fase
   for (int nf = 0; nf < sizeof(fase)/sizeof(char*); nf++)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado(fase[nf]));
      //determina Pot.Reativa na fase da Barra selecionada
      q = suprimento->resflow->Qf_mvar[num_pat][nf];
      //define unidade e valor
      aue.PotReativa_MVAR(q, dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qn"));
   //determina Pot.Reativa no neutro da Barra selecionada
   q = suprimento->resflow->Qn_mvar[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotReativa_max(void)
   {
   //variáveis locais
   double q_max;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qmax"));
   //determina Pot.Reativa máxima na Barra selecionada
   q_max  = suprimento->resflow->Qf_mvar_max[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q_max, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorPotReativa_min(void)
   {
   //variáveis locais
   double q_min;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qmin"));
   //determina Pot.Reativa mínima na Barra selecionada
   q_min  = suprimento->resflow->Qf_mvar_max[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q_min, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorTipoSuprimento(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   //define unidade e valor
   aue.TipoSuprimento(suprimento->tiposup, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorTipoSuprimentoNoFluxo(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   //define unidade e valor
   aue.TipoSuprimento(suprimento->resflow->TipoSup, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::ValorZeqv(void)
   {
   //variáveis locais
   complex<double> z_pu, s_mva;
   VTDado         *dado;

   if ((suprimento->zeq0.r > 0.)||(suprimento->zeq0.x > 0.))
      {//cria um novo Dado p/ zeq0 e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Z0eqv"));
      //define unidade e valor
      z_pu = complex<double>(suprimento->zeq0.r, suprimento->zeq0.x);
      aue.Impedancia_PU100(z_pu, dado->valor, dado->unidade);
      //cria um novo Dado p/ zeq1 e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Z1eqv"));
      //define unidade e valor
      z_pu = complex<double>(suprimento->zeq1.r, suprimento->zeq1.x);
      aue.Impedancia_PU100(z_pu, dado->valor, dado->unidade);
      }
   else
      {//cria um novo Dado p/ pcc_3f e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("pcc_3f"));
      //define unidade e valor
      s_mva = complex<double>(suprimento->pcc_3f.p, suprimento->pcc_3f.q);
      aue.PotAparente_MVA(Abs(s_mva), dado->valor, dado->unidade);
      //cria um novo Dado p/ pcc_ft e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("pcc_ft"));
      //define unidade e valor
      s_mva = complex<double>(suprimento->pcc_ft.p, suprimento->pcc_ft.q);
      aue.PotAparente_MVA(Abs(s_mva), dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorEqptoPQVF(int ind_atrib)
   {
   //variáveis locais
   AnsiString     val_str;
   int            max_pat;
   double         v_pu, t_grau, p_mw, q_mvar;
   VTVetval       *vetval;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria Vetval p/ pot.ativa
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(P)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      p_mw  = fornecimento->P_mw[suprimento][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //cria Vetval p/ pot.reativa
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(Q)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      q_mvar = fornecimento->Q_mvar[suprimento][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(q_mvar, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //cria Vetval p/ módulo de tensão
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(V)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      v_pu  = fornecimento->V_pu[suprimento][np];
      vetval->Valor[np] = v_pu;
      //preenche valor como string
      aue.Tensao_PU(v_pu, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //cria Vetval p/ ãngulo de tensão
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(teta)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      t_grau = fornecimento->T_grau[suprimento][np];
      vetval->Valor[np] = t_grau;
      //preenche valor como string
      aue.Angulo_grau(t_grau, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoCargto(int ind_atrib)
   {
   //variáveis locais
   VTVetval *vetval;
   double   cargto;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Cargto(%)", suprimento->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < suprimento->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = Abs(suprimento->resflow->Cargto[np]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoCurvaPQ_PV_VF(int ind_atrib)
   {
   //variáveis locais
   AnsiString     val_str;
   int            max_pat;
   double         v_pu, t_rad, p_mw, q_mvar;
   VTVetval       *vetval;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //verifica se suprimento do tipo supPQ
   if (suprimento->tiposup == supVF)
      {//cria Vetval p/ tensão (módulo e fase)
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(V/teta)", max_pat));
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         v_pu  = fornecimento->V_pu[suprimento][np];
         t_rad = fornecimento->T_rad[suprimento][np];
         vetval->Valor[np] = v_pu;
         //preenche valor como string
         aue.Tensao_PU(polar(v_pu, t_rad), val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   //verifica se suprimento do tipo supPV
   if (suprimento->tiposup == supPV)
      {//cria um  Vetval p/ P e outro Vetval p/ V
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(P)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         p_mw  = fornecimento->P_mw[suprimento][np];
         vetval->Valor[np] = p_mw;
         //preenche valor como string
         vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //cria Vetval p/ V
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(V)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         v_pu  = fornecimento->V_pu[suprimento][np];
         vetval->Valor[np] = v_pu;
         //preenche valor como string
         aue.Tensao_PU(v_pu, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   //verifica se suprimento do tipo supPQ
   if (suprimento->tiposup == supPQ)
      {//cria um  Vetval p/ P e outro Vetval p/ Q
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(P)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         p_mw  = fornecimento->P_mw[suprimento][np];
         vetval->Valor[np] = p_mw;
         //preenche valor como string
         vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //cria Vetval p/ Q
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Suprimento(Q)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         q_mvar  = fornecimento->Q_mvar[suprimento][np];
         vetval->Valor[np] = q_mvar;
         //preenche valor como string
         vetval->Valor[np] = aue.PotReativa_MVAR(q_mvar, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoFP(int ind_atrib)
   {
   int PENDENCIA_VetValorFluxoFP = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoFPf(int ind_atrib)
   {
   int PENDENCIA_VetValorFluxoFPf = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoIf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   complex<double> i_amp;
   VTVetval        *vetval;
   VTFases         *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int             fase_id[]  = {faseA, faseB, faseC, faseN};

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < sizeof(fase_id)/sizeof(int); nf++)
      {
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "I" + fases->AsStrLower[fase_id[nf]], max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {//preenche valores
         for (int np = 0; np < max_pat; np++)
            {
            i_amp = suprimento->resflow->If_amp[np][nf];
            vetval->Valor[np] = Abs(i_amp);
            //preenche valor como string
            aue.Corrente_A(i_amp, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoIf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Imax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->If_amp_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.Corrente_A(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoIf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Imin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->If_amp_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.Corrente_A(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoP(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "P", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->P_mw[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoPf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   VTVetval        *vetval;
   VTFases         *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int             fase_id[]  = {faseA, faseB, faseC, faseN};

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < sizeof(fase_id)/sizeof(int); nf++)
      {
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "P" + fases->AsStrLower[fase_id[nf]], max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {//preenche valores
         for (int np = 0; np < max_pat; np++)
            {
            vetval->Valor[np] = suprimento->resflow->Pf_mw[np][nf];
            //preenche valor como string
            vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoPf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Pmax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Pf_mw_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoPf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Pmin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Pf_mw_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoQ(int ind_atrib)
   {
   //variáveis locais
   AnsiString     val_str;
   int            max_pat;
   VTVetval       *vetval;
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Q", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Q_mvar[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoQf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   VTVetval        *vetval;
   VTFases         *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int             fase_id[]  = {faseA, faseB, faseC, faseN};

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < sizeof(fase_id)/sizeof(int); nf++)
      {
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Q" + fases->AsStrLower[fase_id[nf]], max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {//preenche valores
         for (int np = 0; np < max_pat; np++)
            {
            vetval->Valor[np] = suprimento->resflow->Qf_mvar[np][nf];
            //preenche valor como string
            vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoQf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Qmax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Qf_mvar_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoQf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Qmin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Qf_mvar_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoS(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   complex<double> s_mva;
   VTVetval        *vetval;
   VTPatamares     *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "S", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      s_mva = suprimento->resflow->S_mva[np];
      //preenche valor como string
      vetval->Valor[np]      = Abs(aue.PotAparente_MVA(s_mva, val_str, vetval->unidade));
      vetval->ValorAsStr[np] = val_str;
      }
   //ajusta valores de acordo c/ a UE mais conveniente
   //vetval->AjustaUE_MVA();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoSf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   complex<double> s_mva;
   VTVetval        *vetval;
   VTFases         *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int             fase_id[]  = {faseA, faseB, faseC, faseN};

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval por fase e insere em lisEXT
   for (int nf = 0; nf < sizeof(fase_id)/sizeof(int); nf++)
      {
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "S" + fases->AsStrLower[fase_id[nf]], max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {//preenche valores
         for (int np = 0; np < max_pat; np++)
            {
            s_mva = suprimento->resflow->Sf_mva[np][nf];
            //preenche valor como string
            vetval->Valor[np]      = Abs(aue.PotAparente_MVA(s_mva, val_str, vetval->unidade));
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoSf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Smax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Sf_mva_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(Abs(vetval->Valor[np]), val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaSuprimento::VetValorFluxoSf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Smin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = suprimento->resflow->Sf_mva_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof


