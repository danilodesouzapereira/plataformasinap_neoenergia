//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaGerador.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaGerador(VTApl *apl)
   {
   try{
      return (new TMontaGerador(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaGerador::TMontaGerador(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaGerador::~TMontaGerador(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaGerador::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int      ano     = 0;
   int      num_pat = 0;
   unsigned show;
   VTGrupo  *grupo1, *grupo2;

   //salva ponteiro p/ Gerador
   gerador = (VTGerador*)eqpto;
   //cria Grupo básico
   grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
   if (grupo1 == NULL) return(NULL);
   //cria Grupo2 c/ dados do Gerador
   if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do Eqpto
      LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
      //insere dados da curva de  fornecimento
      LisVetValEqpto(eqpto, grupo2->LisVetval, ger_PQVF);
      }
   /*
   //cria Grupo2 c/ dados de fornecimento
   if ((grupo2 = NewObjGrupo(eqpto, "Curva de potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados da curva
      LisVetValEqpto(eqpto, grupo2->LisVetval, ger_PQVF);
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
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_S,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_S_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_S_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_S_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_P,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_P_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_P_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_P_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_Q,     ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_Q_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_Q_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_Q_MIN, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_I_ABC, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_I_MAX, ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, ger_I_MIN, ano);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->gerador = (VTGerador*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->gerador = (VTGerador*)eqpto;
   //atributos de Eqbar
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Gerador
   lisATR->Add(new TAtributo(gerador, ger_TIPO,     true, false, "Tipo suprimento"));
   lisATR->Add(new TAtributo(gerador, ger_SNOM,     true, false, "Pot.Nominal", "(MVA)"));
   lisATR->Add(new TAtributo(gerador, ger_LIGACAO,  true, false, "Tipo de Ligação"));
   lisATR->Add(new TAtributo(gerador, ger_Z_SEQ,    true, false, "Imp.Seq.0/Seq.1", "(pu)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->gerador = (VTGerador*)eqpto;
   //atributos de fluxo de Eqbar
   TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo do Suprimento
   if (gerador->tiposup == supPQ)
      {lisATR->Add(new TAtributo(gerador, ger_PQVF, false, true,  "PQ:Pot.Ativa/Pot.Reativa"));}
   else if (gerador->tiposup == supPV)
      {lisATR->Add(new TAtributo(gerador, ger_PQVF, false, true,  "PV:Mod.Tensão/Pot.Ativa"));}
   else if (gerador->tiposup == supVF)
      {lisATR->Add(new TAtributo(gerador, ger_PQVF, false, true,  "VF:Mod.Tensão/Âng.Tensão"));}
   lisATR->Add(new TAtributo(gerador, ger_PIE,      false, false, "Cargto.(gráfico)"));
   lisATR->Add(new TAtributo(gerador, ger_CARGTO,   false, true,  "Carregamento", "(%)"));
   lisATR->Add(new TAtributo(gerador, ger_S,        false, true,  "Pot.Aparente", "(MVA)"));
   lisATR->Add(new TAtributo(gerador, ger_FP,       false, true,  "Fat.Potência", "(pu)"));
   lisATR->Add(new TAtributo(gerador, ger_S_ABC,    false, true,  "Pot.Aparente por fase", "(MVA)"));
   lisATR->Add(new TAtributo(gerador, ger_FP_ABC,   false, true,  "Fat.Potência por fase", "(pu)"));
   lisATR->Add(new TAtributo(gerador, ger_S_MAX,    false, true,  "Pot.Aparente máxima", "(MVA)"));
   lisATR->Add(new TAtributo(gerador, ger_S_MIN,    false, true,  "Pot.Aparente mínima", "(MVA)"));
   lisATR->Add(new TAtributo(gerador, ger_P,        false, true,  "Pot.Ativa", "(MW)"));
   lisATR->Add(new TAtributo(gerador, ger_P_ABC,    false, true,  "Pot.Ativa por fase", "(MW)"));
   lisATR->Add(new TAtributo(gerador, ger_P_MAX,    false, true,  "Pot.Ativa máxima", "(MW)"));
   lisATR->Add(new TAtributo(gerador, ger_P_MIN,    false, true,  "Pot.Ativa mínima", "(MW)"));
   lisATR->Add(new TAtributo(gerador, ger_Q,        false, true,  "Pot.Reativa", "(MVAr)"));
   lisATR->Add(new TAtributo(gerador, ger_Q_ABC,    false, true,  "Pot.Reativa por fase", "(MVAr)"));
   lisATR->Add(new TAtributo(gerador, ger_Q_MAX,    false, true,  "Pot.Reativa máxima", "(MVAr)"));
   lisATR->Add(new TAtributo(gerador, ger_Q_MIN,    false, true,  "Pot.Reativa mínima", "(MVAr)"));
   lisATR->Add(new TAtributo(gerador, ger_I_ABC,    false, true,  "Corrente por fase", "(A)"));
   lisATR->Add(new TAtributo(gerador, ger_I_MAX,    false, true,  "Corrente máxima", "(A)"));
   lisATR->Add(new TAtributo(gerador, ger_I_MIN,    false, true,  "Corrente mínima", "(A)"));

   return(true);                                    
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //variáveis locais
   bool exibe_tipo;

   //salva parâmetros
   this->gerador = (VTGerador*)eqpto;
   //salva opção de mostrar o tipo de suprimento
   exibe_tipo = eqpto->Show[ger_TIPO];
   //bloqueia opção  de mostrar o tipo de suprimento
   eqpto->Show[ger_TIPO] = false;
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores da Curva de potência
   LisValoresCurva(eqpto, lisEXT, ano, num_pat);
   //restaura opção de mostrar o tipo de suprimento
   eqpto->Show[ger_TIPO] = exibe_tipo;
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisValoresCurva(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat)
   {
   //salva parâmetros
   this->gerador  = (VTGerador*)eqpto;
   try{//dados de Curva de potência
      if (gerador->Show[ger_PQVF]) ValorCurvaPQ_PV_VF();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->gerador  = (VTGerador*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do Gerador
      if (gerador->Show[ger_SNOM])    ValorPotNominal();
      if (gerador->Show[ger_LIGACAO]) ValorTipoLigacao();
      if (gerador->Show[ger_Z_SEQ])   ValorImpSequencia();
      if (gerador->Show[ger_TIPO])    ValorTipoSuprimento();
      //if (gerador->Show[ger_PQVF])    ValorPQ_PV_VF();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->gerador  = (VTGerador*)eqpto;
   this->lisEXT   = lisEXT;
   try{//dados de fluxo
      if (gerador->Show[ger_TIPO])    ValorTipoSuprimentoNoFluxo();
      if (gerador->Show[ger_PIE])     ValorCargtoGrafico();
      if (gerador->Show[ger_CARGTO])  ValorCarregamento();
      if (gerador->Show[ger_S])       ValorPotAparenteTotal();
      if (gerador->Show[ger_S_ABC])   ValorPotAparentePorFase();
      if (gerador->Show[ger_S_MAX])   ValorPotAparente_max();
      if (gerador->Show[ger_S_MIN])   ValorPotAparente_min();
      if (gerador->Show[ger_P])       ValorPotAtivaTotal();
      if (gerador->Show[ger_P_ABC])   ValorPotAtivaPorFase();
      if (gerador->Show[ger_P_MAX])   ValorPotAtiva_max();
      if (gerador->Show[ger_P_MIN])   ValorPotAtiva_min();
      if (gerador->Show[ger_Q])       ValorPotReativaTotal();
      if (gerador->Show[ger_Q_ABC])   ValorPotReativaPorFase();
      if (gerador->Show[ger_Q_MAX])   ValorPotReativa_max();
      if (gerador->Show[ger_Q_MIN])   ValorPotReativa_min();
      if (gerador->Show[ger_I_ABC])   ValorCorrentePorfase();
      if (gerador->Show[ger_I_MAX])   ValorCorrente_max();
      if (gerador->Show[ger_I_MIN])   ValorCorrente_min();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->gerador = (VTGerador*)eqpto;
   this->lisEXT  = lisEXT;
   this->ano     = 0;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case ger_PQVF: VetValorEqptoPQVF(ind_atrib); break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaGerador::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->gerador = (VTGerador*)eqpto;
   this->lisEXT  = lisEXT;
   this->ano     = ano;
   //verifica atributo selecionado
   switch(ind_atrib)
      {
      case ger_CARGTO: VetValorFluxoCargto(ind_atrib);        break;
      case ger_PQVF:   VetValorFluxoPQ_PV_VF(ind_atrib);      break;
      case ger_S:      VetValorFluxoS(ind_atrib);             break;
      case ger_S_ABC:  VetValorFluxoSf(ind_atrib);            break;
      case ger_S_MAX:  VetValorFluxoSf_max(ind_atrib);        break;
      case ger_S_MIN:  VetValorFluxoSf_min(ind_atrib);        break;
      case ger_P:      VetValorFluxoP(ind_atrib);             break;
      case ger_P_ABC:  VetValorFluxoPf(ind_atrib);            break;
      case ger_P_MAX:  VetValorFluxoPf_max(ind_atrib);        break;
      case ger_P_MIN:  VetValorFluxoPf_min(ind_atrib);        break;
      case ger_Q:      VetValorFluxoQ(ind_atrib);             break;
      case ger_Q_ABC:  VetValorFluxoQf(ind_atrib);            break;
      case ger_Q_MAX:  VetValorFluxoQf_max(ind_atrib);        break;
      case ger_Q_MIN:  VetValorFluxoQf_min(ind_atrib);        break;
      case ger_I_ABC:  VetValorFluxoIf(ind_atrib);            break;
      case ger_I_MAX:  VetValorFluxoIf_min(ind_atrib);        break;
      case ger_I_MIN:  VetValorFluxoIf_max(ind_atrib);        break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaGerador::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor, unidade;
   VTGerador  *gerador = (VTGerador*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                           break;
      case eqp_CODIGO: valor = eqpto->Codigo;                                 break;
      case ger_TIPO:   aue.TipoSuprimento(gerador->tiposup, valor, unidade);  break;
      case ger_SNOM:   valor = DoubleToStr("%2.1f", gerador->snom);           break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorCarregamento(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Cargto"));
   //define unidade e valor
   aue.Carregamento_pu(gerador->resflow->Cargto_pu[num_pat], dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorCargtoGrafico(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorCorrentePorfase(void)
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
      i  = gerador->resflow->If_amp[num_pat][nf];
      //define unidade e valor
      aue.Corrente_A(i, dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("In"));
   //determina corrente no neutro da Barra selecionada
   i  = gerador->resflow->Sn_mva[num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorCorrente_max(void)
   {
   //variáveis locais
   double i;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Imax"));
   //determina corrente máxima em relação à Barra c/ Pot.Ativa positiva
   i  = gerador->resflow->If_amp_max[num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorCorrente_min(void)
   {
   //variáveis locais
   double i;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Imin"));
   //determina corrente mínima em relação à Barra c/ Pot.Ativa positiva
   i  = gerador->resflow->If_amp_min[num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorCurvaPQ_PV_VF(void)
   {
   //variáveis locais
   double         p, q, v, t;
   VTDado         *dado;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   if (gerador->tiposup == supPV)
      {
      p = fornecimento->P_mw[gerador][num_pat];
      v = fornecimento->V_pu[gerador][num_pat];
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("P"));
      aue.PotAtiva_MW(p, dado->valor, dado->unidade);
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("V"));
      aue.Tensao_KV(v, dado->valor, dado->unidade);
      }
   else if (gerador->tiposup == supVF)
      {
      v = fornecimento->V_pu[gerador][num_pat];
      t = fornecimento->T_rad[gerador][num_pat];
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("VF"));
      aue.Tensao_KV(complex<double>(v,t), dado->valor, dado->unidade);
      }
   else if (gerador->tiposup == supPQ)
      {
      p = fornecimento->P_mw[gerador][num_pat];
      q = fornecimento->Q_mvar[gerador][num_pat];
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("PQ"));
      aue.PotAparente_MVA(complex<double>(p,q), dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorFatPotenciaTotal(void)
   {
   int PENDENCIA_ValorFatPotenciaTotal = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorFatPotenciaPorFase(void)
   {
   int PENDENCIA_ValorFatPotenciaPorFase = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorImpSequencia(void)
   {
   //variáveis locais
   VTDado *dado;
   complex<double> z0(gerador->z0.r, gerador->z0.x);
   complex<double> z1(gerador->z1.r, gerador->z1.x);

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_PU(z0, dado->valor, dado->unidade);
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_PU(z1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotNominal(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Snom"));
   //define unidade e valor
   aue.PotAparente_MVA(gerador->snom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAparenteTotal(void)
   {
   //variáveis locais
   complex<double> s;
   VTDado          *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("S"));
   //determina Pot.Aparente total na Barra selecionada
   s  = gerador->resflow->S_mva[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(Abs(s), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAparentePorFase(void)
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
      sf  = gerador->resflow->Sf_mva[num_pat][nf];
      //define unidade e valor
      aue.PotAparente_MVA(Abs(sf), dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Sn"));
   //determina Pot.Aparente no neutro da Barra selecionada
   sf  = gerador->resflow->Sn_mva[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(Abs(sf), dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAparente_max(void)
   {
   //variáveis locais
   double s_max;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smax"));
   //determina Pot.Aparente máxima na Barra selecionada
   s_max  = gerador->resflow->Sf_mva_max[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(s_max, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAparente_min(void)
   {
   //variáveis locais
   double s_min;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smin"));
   //determina Pot.Aparente mínima na Barra selecionada
   s_min  = gerador->resflow->Sf_mva_min[num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(s_min, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAtivaTotal(void)
   {
   //variáveis locais
   double p;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("P"));
   //determina Pot.Ativa total na Barra selecionada
   p = gerador->resflow->P_mw[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAtivaPorFase(void)
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
      p = gerador->resflow->Pf_mw[num_pat][nf];
      //define unidade e valor
      aue.PotAtiva_MW(p, dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pn"));
   //determina Pot.Ativa no neutro da Barra selecionada
   p = gerador->resflow->Pn_mw[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAtiva_max(void)
   {
   //variáveis locais
   double p_max;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pmax"));
   //determina Pot.Ativa máxima na Barra selecionada
   p_max  = gerador->resflow->Pf_mw_max[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_max, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotAtiva_min(void)
   {
   //variáveis locais
   double p_min;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pmin"));
   //determina Pot.Ativa mínima na Barra selecionada
   p_min  = gerador->resflow->Pf_mw_min[num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_min, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotReativaTotal(void)
   {
   //variáveis locais
   double q;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //determina Pot.Reaiva total na Barra selecionada
   q = gerador->resflow->Q_mvar[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q, dado->valor, dado->unidade);
   }


//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotReativaPorFase(void)
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
      q = gerador->resflow->Qf_mvar[num_pat][nf];
      //define unidade e valor
      aue.PotReativa_MVAR(q, dado->valor, dado->unidade);
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qn"));
   //determina Pot.Reativa no neutro da Barra selecionada
   q = gerador->resflow->Qn_mvar[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotReativa_max(void)
   {
   //variáveis locais
   double q_max;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qmax"));
   //determina Pot.Reativa máxima na Barra selecionada
   q_max  = gerador->resflow->Qf_mvar_max[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q_max, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorPotReativa_min(void)
   {
   //variáveis locais
   double q_min;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qmin"));
   //determina Pot.Reativa mínima na Barra selecionada
   q_min  = gerador->resflow->Qf_mvar_max[num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q_min, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorTipoLigacao(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Ligação"));
   //define unidade e valor
   aue.TipoLigacao(gerador->ligacao, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorTipoSuprimento(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   //define unidade e valor
   aue.TipoSuprimento(gerador->tiposup, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::ValorTipoSuprimentoNoFluxo(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   //define unidade e valor
   aue.TipoSuprimento(gerador->resflow->TipoSup, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorEqptoPQVF(int ind_atrib)
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
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(P)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      p_mw  = fornecimento->P_mw[gerador][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //cria Vetval p/ pot.reativa
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(Q)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      q_mvar = fornecimento->Q_mvar[gerador][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(q_mvar, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //cria Vetval p/ módulo de tensão
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(V)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      v_pu  = fornecimento->V_pu[gerador][np];
      vetval->Valor[np] = v_pu;
      //preenche valor como string
      aue.Tensao_PU(v_pu, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //cria Vetval p/ ãngulo de tensão
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(teta)", max_pat));
   for (int np = 0; np < max_pat; np++)
      {
      t_grau = fornecimento->T_grau[gerador][np];
      vetval->Valor[np] = t_grau;
      //preenche valor como string
      aue.Angulo_grau(t_grau, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoCargto(int ind_atrib)
   {
   //variáveis locais
   VTVetval *vetval;
   double   cargto;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Cargto(%)", gerador->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < gerador->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = Abs(gerador->resflow->Cargto[np]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoFP(int ind_atrib)
   {
   int PENDENCIA_VetValorFluxoFP = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoFPf(int ind_atrib)
   {
   int PENDENCIA_VetValorFluxoFPf = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoIf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   double          i_amp;
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
            i_amp = Abs(gerador->resflow->If_amp[np][nf]);
            //preenche valor como string
            vetval->Valor[np]      = aue.Corrente_A(i_amp, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoIf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       i_amp;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Imax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      i_amp = gerador->resflow->If_amp_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.Corrente_A(i_amp, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoIf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       i_amp;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Imin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      i_amp = gerador->resflow->If_amp_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.Corrente_A(i_amp, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoP(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       p_mw;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "P", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      p_mw = gerador->resflow->P_mw[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoPf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   double          p_mw;
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
            p_mw = gerador->resflow->Pf_mw[np][nf];
            //preenche valor como string
            vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoPf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       p_mw;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Pmax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      p_mw = gerador->resflow->Pf_mw_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoPf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       p_mw;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Pmin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      p_mw = gerador->resflow->Pf_mw_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }


//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoPQ_PV_VF(int ind_atrib)
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
   //verifica se gerador do tipo supPQ
   if (gerador->tiposup == supVF)
      {//cria Vetval p/ tensão (módulo e fase)
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(V/teta)", max_pat));
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         v_pu  = fornecimento->V_pu[gerador][np];
         t_rad = fornecimento->T_rad[gerador][np];
         vetval->Valor[np] = v_pu;
         //preenche valor como string
         aue.Tensao_PU(polar(v_pu, t_rad), val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_PU();
      }
   //verifica se gerador do tipo supPV
   if (gerador->tiposup == supPV)
      {//cria um  Vetval p/ P e outro Vetval p/ V
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(P)", max_pat));
      //vetval->unidade = "MW";
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         p_mw  = fornecimento->P_mw[gerador][np];
         //preenche valor como string
         vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_MW();
      //cria Vetval p/ V
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(V)", max_pat));
      //vetval->unidade = "pu";
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         v_pu  = fornecimento->V_pu[gerador][np];
         vetval->Valor[np] = v_pu;
         //preenche valor como string
         aue.Tensao_PU(v_pu, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_PU();
      }
   //verifica se gerador do tipo supPQ
   if (gerador->tiposup == supPQ)
      {//cria um Vetval p/ P e um outro Vetval p/ Q
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(P)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         p_mw  = fornecimento->P_mw[gerador][np];
         //preenche valor como string
         vetval->Valor[np]      = aue.PotAtiva_MW(p_mw, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //cria um Vetval  p/ Q
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Gerador(Q)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         q_mvar = fornecimento->Q_mvar[gerador][np];
         //preenche valor como string
         vetval->Valor[np]      = aue.PotReativa_MVAR(q_mvar, val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoQ(int ind_atrib)
   {
   //variáveis locais
   AnsiString     val_str;
   int            max_pat;
   double         q_mvar;
   VTVetval       *vetval;
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Q", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      q_mvar = gerador->resflow->Q_mvar[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(q_mvar, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoQf(int ind_atrib)
   {
   //variáveis locais
   AnsiString      val_str;
   int             max_pat;
   double          q_mvar;
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
            q_mvar = gerador->resflow->Qf_mvar[np][nf];
            //preenche valor como string
            vetval->Valor[np]      = aue.PotReativa_MVAR(q_mvar, val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoQf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       q_max;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Qmax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      q_max = gerador->resflow->Qf_mvar_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(q_max, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoQf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       q_min;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Qmin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      q_min = gerador->resflow->Qf_mvar_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(q_min, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoS(int ind_atrib)
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
      s_mva = gerador->resflow->S_mva[np];
      //vetval->Valor[np] = Abs(s_mva);
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(Abs(s_mva), val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoSf(int ind_atrib)
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
            s_mva = gerador->resflow->Sf_mva[np][nf];
            //vetval->Valor[np] = Abs(s_mva);
            //preenche valor como string
            vetval->Valor[np]      = aue.PotAparente_MVA(Abs(s_mva), val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoSf_max(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       s_max;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Smax", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      s_max = gerador->resflow->Sf_mva_max[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(s_max, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaGerador::VetValorFluxoSf_min(int ind_atrib)
   {
   //variáveis locais
   AnsiString   val_str;
   int          max_pat;
   double       s_min;
   VTVetval     *vetval;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Smin", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      s_min = gerador->resflow->Sf_mva_min[np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(s_min, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof


