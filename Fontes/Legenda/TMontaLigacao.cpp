//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TMontaLigacao.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTRede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaLigacao::TMontaLigacao(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaLigacao::~TMontaLigacao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::FluxoIndefinido(int ind_bar)
   {
   //variáveis locais
   VTChave *chave;

   //verifica se fluxo indefinido
   if (ligacao->resflow->SentidoP[ind_bar][num_pat] != fluxoINDEF) return(false);
   //verifica se é uma Chave fechada
   if (ligacao->Tipo() == eqptoCHAVE)
      {
      chave = (VTChave*)ligacao;
      if (chave->Fechada) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaLigacao::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   int     ano     = 0;
   int     num_pat = 0;
   int     ind_bar;
   int     vet_ind_bar[NBAR_LIG];
   VTGrupo *grupo1, *grupo2, *grupo3;

   //salva ponteiro p/ Barra
   ligacao = (VTLigacao*)eqpto;
   //monta ind_bar c/ índice das barras da Ligacao
   vet_ind_bar[0] = (ligacao->resflow->SentidoP[0][0] == fluxoPOS) ? 0 : 1;
   vet_ind_bar[1] = 1 - vet_ind_bar[0];
   vet_ind_bar[2] = 2;
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
   //cria Grupo2 c/ dados de fluxo que independem da Barra
   if ((grupo2 = NewObjGrupo(eqpto, "Fluxo Potência")) != NULL)
      {//insere Grupo2 no Grupo1
      grupo1->Add(grupo2);
      //inicia Grupo2 c/ dados do fluxo
      LisVetValFluxo(eqpto, grupo2->LisVetval, lig_CARGTO,  ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, lig_PERDA,   ano);
      LisVetValFluxo(eqpto, grupo2->LisVetval, lig_IND_BAL, ano);
      }
   //cria Grupos c/ dados de fluxo em relação às Barras
   for (int nb = 0; nb < NBAR_LIG; nb++)
      {//determina ind_bar
      ind_bar = vet_ind_bar[nb];
      //verifica se existe Barra da Ligacao
      if (ligacao->Barra(ind_bar) == NULL) continue;
      //cria Grupo3
      if ((grupo3 = NewObjGrupo(eqpto, ligacao->Alias(ind_bar))) == NULL) continue;
      //insere Grupo3 no Grupo2
      grupo2->Add(grupo3);
      //inicia Grupo3 c/ dados do fluxo
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_S,       ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_S_ABC,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_S_MAX,   ind_bar, ano);
    //LisVetValFluxo(eqpto, grupo3->LisVetval, lig_S_MIN,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_P,       ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_P_ABC,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_P_MAX,   ind_bar, ano);
    //LisVetValFluxo(eqpto, grupo3->LisVetval, lig_P_MIN,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_Q,       ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_Q_ABC,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_Q_MAX,   ind_bar, ano);
    //LisVetValFluxo(eqpto, grupo3->LisVetval, lig_Q_MIN,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_I_ABC,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_I_MAX,   ind_bar, ano);
    //LisVetValFluxo(eqpto, grupo3->LisVetval, lig_I_MIN,   ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_FP,      ind_bar, ano);
      LisVetValFluxo(eqpto, grupo3->LisVetval, lig_FP_ABC,  ind_bar, ano);
      }
   return(grupo1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->ligacao  = (VTLigacao*)eqpto;
   //insere atributos de Eqpto
   TMontaEqpto::LisAtributosEqpto(eqpto, lisATR);
   //insere atributos da Ligacao
   lisATR->Add(new TAtributo(ligacao, lig_REDE,  true, false, "Rede"));
   lisATR->Add(new TAtributo(ligacao, lig_BARRA, true, false, "Barras"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->ligacao  = (VTLigacao*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosFluxo(eqpto, lisATR);
   //insere atributos de fluxo
   lisATR->Add(new TAtributo(ligacao, lig_PIE,      false, false, "Cargto.(gráfico)"));
   lisATR->Add(new TAtributo(ligacao, lig_CARGTO,   false, true,  "Carregamento", "(%)"));
   lisATR->Add(new TAtributo(ligacao, lig_S,        false, true,  "Pot.Aparente", "(MVA)"));
   lisATR->Add(new TAtributo(ligacao, lig_S_ABC,    false, true,  "Pot.Aparente por fase", "(MVA)"));
   lisATR->Add(new TAtributo(ligacao, lig_S_MAX,    false, true,  "Pot.Aparente máxima", "(MVA)"));
 //lisATR->Add(new TAtributo(ligacao, lig_S_MIN,    false, true,  "Pot.Aparente mínima", "(MVA)"));
   lisATR->Add(new TAtributo(ligacao, lig_P,        false, true,  "Pot.Ativa", "(MW)"));
   lisATR->Add(new TAtributo(ligacao, lig_P_ABC,    false, true,  "Pot.Ativa por fase", "(MW)"));
   lisATR->Add(new TAtributo(ligacao, lig_P_MAX,    false, true,  "Pot.Ativa máxima", "(MW)"));
 //lisATR->Add(new TAtributo(ligacao, lig_P_MIN,    false, true,  "Pot.Ativa mínima", "(MW)"));
   lisATR->Add(new TAtributo(ligacao, lig_Q,        false, true,  "Pot.Reativa", "(MVAr)"));
   lisATR->Add(new TAtributo(ligacao, lig_Q_ABC,    false, true,  "Pot.Reativa por fase", "(MVAr)"));
   lisATR->Add(new TAtributo(ligacao, lig_Q_MAX,    false, true,  "Pot.Reativa máxima", "(MVAr)"));
 //lisATR->Add(new TAtributo(ligacao, lig_Q_MIN,    false, true,  "Pot.Reativa mínima", "(MVAr)"));
   lisATR->Add(new TAtributo(ligacao, lig_I_ABC,    false, true,  "Corrente por fase", "(A)"));
   lisATR->Add(new TAtributo(ligacao, lig_I_MAX,    false, true,  "Corrente máxima", "(A)"));
 //lisATR->Add(new TAtributo(ligacao, lig_I_MIN,    false, true,  "Corrente mínima", "(A)"));
   lisATR->Add(new TAtributo(ligacao, lig_FP,       false, true,  "Fator Potência", "(pu)"));
   lisATR->Add(new TAtributo(ligacao, lig_FP_ABC,   false, true,  "Fator Potência por fase", "(pu)"));
   lisATR->Add(new TAtributo(ligacao, lig_PERDA,    false, true,  "Perda", "MW"));
   lisATR->Add(new TAtributo(ligacao, lig_IND_BAL,  false, true,  "Índice Balanceamento", "(%)"));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //variáveis locais
   //int ind_bar;

   //salva parâmetros
   this->ligacao  = (VTLigacao*)eqpto;
   //define índice da barra c/ fluxo positivo ou nulo
   //ind_bar = (ligacao->resflow->SentidoP[0][num_pat] != fluxoNEG) ? 0 : 1;
   try{//dados do Eqpto
      TMontaEqpto::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados gerais do ligacao
      if (ligacao->Show[lig_REDE])  ValorCodigoRede();
      if (ligacao->Show[lig_BARRA]) ValorCodigoBarras();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //variáveis locais
   int ind_bar;

   //salva parâmetros
   ligacao = (VTLigacao*)eqpto;
   //define índice da barra c/ fluxo positivo ou nulo
   ind_bar = (ligacao->resflow->SentidoP[0][num_pat] != fluxoNEG) ? 0 : 1;
   try{//dados do Eqpto
      TMontaEqpto::LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
      //dados de Fluxo
      if (ligacao->Show[lig_CARGTO])   ValorCarregamento();
      if (ligacao->Show[lig_S])        ValorPotAparenteTotal(ind_bar);
      if (ligacao->Show[lig_S_ABC])    ValorPotAparentePorFase(ind_bar);
      if (ligacao->Show[lig_S_MAX])    ValorPotAparente_max(ind_bar);
    //if (ligacao->Show[lig_S_MIN])    ValorPotAparente_min(ind_bar);
      if (ligacao->Show[lig_P])        ValorPotAtivaTotal(ind_bar);
      if (ligacao->Show[lig_P_ABC])    ValorPotAtivaPorFase(ind_bar);
      if (ligacao->Show[lig_P_MAX])    ValorPotAtiva_max(ind_bar);
    //if (ligacao->Show[lig_P_MIN])    ValorPotAtiva_min(ind_bar);
      if (ligacao->Show[lig_Q])        ValorPotReativaTotal(ind_bar);
      if (ligacao->Show[lig_Q_ABC])    ValorPotReativaPorFase(ind_bar);
      if (ligacao->Show[lig_Q_MAX])    ValorPotReativa_max(ind_bar);
    //if (ligacao->Show[lig_Q_MIN])    ValorPotReativa_min(ind_bar);
      if (ligacao->Show[lig_I_ABC])    ValorCorrentePorfase(ind_bar);
      if (ligacao->Show[lig_I_MAX])    ValorCorrente_max(ind_bar);
    //if (ligacao->Show[lig_I_MIN])    ValorCorrente_min(ind_bar);
      if (ligacao->Show[lig_FP])       ValorFatorPotenciaTotal(ind_bar);
      if (ligacao->Show[lig_FP_ABC])   ValorFatorPotenciaPorFase(ind_bar);
      if (ligacao->Show[lig_PERDA])    ValorPerda(ind_bar);
      if (ligacao->Show[lig_IND_BAL])  ValorIndiceBal(ind_bar);
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //variáveis locais
   int       ind_bar;
   VTLigacao *ligacao = (VTLigacao*)eqpto;

   //determina índice da Barra em que o sentido do fluxo é positivo
   ind_bar = (ligacao->resflow->SentidoP[0][0] == fluxoPOS) ? 0 : 1;
   //monta vetor de valores de fluxo em relação à barra selecionada
   return(LisVetValFluxo(eqpto, lisEXT, ind_atrib, ind_bar, ano));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaLigacao::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ind_bar, int ano)
   {
   //variáveis locais
   int num_pat = 0;

   //salva Ligacao e índice da barra de referência
   ligacao = (VTLigacao*)eqpto;
   //salva demais dados
   TMontaEqpto::LisVetValFluxo(eqpto, lisEXT, ano, num_pat);
   //monta Vetval p/ o atributo indicado
   switch(ind_atrib)
      {
      case lig_CARGTO:   VetValorCargto(ind_atrib);             break;
      case lig_S:        VetValorS(ind_atrib, ind_bar);         break;
      case lig_S_ABC:    VetValorSf(ind_atrib, ind_bar);        break;
      case lig_S_MAX:    VetValorSf_max(ind_atrib, ind_bar);    break;
    //case lig_S_MIN:    VetValorSf_min(ind_atrib, ind_bar);    break;
      case lig_P:        VetValorP(ind_atrib, ind_bar);         break;
      case lig_P_ABC:    VetValorPf(ind_atrib, ind_bar);        break;
      case lig_P_MAX:    VetValorPf_max(ind_atrib, ind_bar);    break;
    //case lig_P_MIN:    VetValorPf_min(ind_atrib, ind_bar);    break;
      case lig_Q:        VetValorQ(ind_atrib, ind_bar);         break;
      case lig_Q_ABC:    VetValorQf(ind_atrib, ind_bar);        break;
      case lig_Q_MAX:    VetValorQf_max(ind_atrib, ind_bar);    break;
    //case lig_Q_MIN:    VetValorQf_min(ind_atrib, ind_bar);    break;
      case lig_I_ABC:    VetValorIf(ind_atrib, ind_bar);        break;
      case lig_I_MAX:    VetValorIf_max(ind_atrib, ind_bar);    break;
    //case lig_I_MIN:    VetValorIf_min(ind_atrib, ind_bar);    break;
      case lig_FP:       VetValorFP(ind_atrib, ind_bar);        break;
      case lig_FP_ABC:   VetValorFPf(ind_atrib, ind_bar);       break;
      case lig_PERDA:    VetValorPerda(ind_atrib);              break;
      case lig_IND_BAL:  VetValorIndiceBal(ind_atrib, ind_bar); break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCarregamento(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Cargto"));
   //define unidade e valor
   aue.Carregamento_pu(ligacao->resflow->Cargto_pu[num_pat], dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCodigoBarras(void)
   {
   //variáveis locais
   VTBarra *barra;
   VTDado  *dado;

   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {//verifica se existe a Barra
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Barra" + IntToStr(ind_bar+1)));
      //define unidade e valor
      dado->valor   = barra->Codigo;
      dado->unidade = "";
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCodigoRede(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Rede"));
   //define unidade e valor
   dado->valor   = (ligacao->rede == NULL) ? AnsiString("") : ligacao->rede->Codigo;
   dado->unidade = "";
   }
/*
//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCorrentePorfase(int ind_bar)
   {
   //variáveis locais
   complex<double> i;
   VTDado          *dado;
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("If: indef"));
      return;
      }
   //correntes de fase em A
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("I" + fases->AsStrLower[fases->IndexToTag[nf]]));
      //determina corrente na fase da Barra selecionada
      i  = ligacao->resflow->If_amp[ind_bar][num_pat][nf];
      //define unidade e valor
      aue.Corrente_A(i, dado->valor, dado->unidade);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCorrentePorfase(int ind_bar)
   {
   //variáveis locais
   complex<double> i_amp;
   VTBarra         *barra;
   VTDado          *dado;
   int             fases_lig;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Ia"}, {faseB, "Ib"}, {faseC, "Ic"}, {faseN, "In"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("If: indef"));
      return;
      }
   //correntes de fase em A
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina corrente na fase da Barra selecionada
         i_amp  = ligacao->resflow->If_amp[ind_bar][num_pat][nf];
         //define unidade e valor
         aue.Corrente_A(i_amp, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCorrente_max(int ind_bar)
   {
   //variáveis locais
   double    i;
   VTDado     *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Imax: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Imax"));
   //determina corrente máxima em relação à Barra c/ Pot.Ativa positiva
   i  = ligacao->resflow->If_amp_max[ind_bar][num_pat];
   //define unidade e valor
   aue.Corrente_A(i, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorCorrente_min(int ind_bar)
   {
   //variáveis locais
   double   i_amp;
   VTDado   *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Imin: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Imin"));
   //determina corrente mínima em relação à Barra c/ Pot.Ativa positiva
   i_amp  = ligacao->resflow->If_amp_min[ind_bar][num_pat];
   //define unidade e valor
   aue.Corrente_A(i_amp, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorFatorPotenciaPorFase(int ind_bar)
   {
   //variáveis locais
   double   fp_pu;
   VTDado  *dado;
   VTBarra *barra;
   int     fases_lig;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "FPa"}, {faseB, "FPb"}, {faseC, "FPc"}, {faseN, "FPn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("FPf: indef"));
      return;
      }
   //correntes de fase em A
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina fator de potência na fase da Barra selecionada
         fp_pu  = ligacao->resflow->FPf_pu[ind_bar][num_pat][nf];
         //define unidade e valor
         aue.FatorPotencia_pu(fp_pu, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorFatorPotenciaTotal(int ind_bar)
   {
   //variáveis locais
   double fp_pu;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("FP: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("FP"));
   //determina fator de potência na Barra selecionada
   fp_pu   = ligacao->resflow->FP_pu[ind_bar][num_pat];
   //define unidade e valor
   aue.FatorPotencia_pu(fp_pu, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorIndiceBal(int ind_bar)
   {
   //variáveis locais
   double  indice_bal;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Índice Bal."));
   //determina índice de balanceamento em relação à Barra
   indice_bal  = ligacao->resflow->IndiceBal[ind_bar][num_pat];
   aue.ValorPorcentual(indice_bal, dado->valor, dado->unidade, "%4.3f");
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPerda(int ind_bar)
   {
   //variáveis locais
   double  perda_mw;
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Perda"));
   //determina perda
   perda_mw  = ligacao->resflow->Perda_mw[num_pat];
   aue.PerdaAtiva_MW(perda_mw, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAparenteTotal(int ind_bar)
   {
   //variáveis locais
   int             sentido;
   complex<double> s_mva;
   VTDado          *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("S: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("S"));
   //determina Pot.Aparente total na Barra selecionada
   s_mva   = ligacao->resflow->S_mva[ind_bar][num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(s_mva, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAparentePorFase(int ind_bar)
   {
   //variáveis locais
   complex<double> s_mva;
   VTDado          *dado;
   VTBarra         *barra;
   int             fases_lig;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Sa"}, {faseB, "Sb"}, {faseC, "Sc"}, {faseN, "Sn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Sf: indef"));
      return;
      }
   //correntes de fase em A
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina Pot.Aparente na fase da Barra selecionada
         s_mva  = ligacao->resflow->Sf_mva[ind_bar][num_pat][nf];
         //define unidade e valor
         aue.PotAparente_MVA(s_mva, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAparente_max(int ind_bar)
   {
   //variáveis locais
   double s_mva;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Smax: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smax"));
   //determina Pot.Aparente máxima na Barra selecionada
   s_mva   = ligacao->resflow->Sf_mva_max[ind_bar][num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(s_mva, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAparente_min(int ind_bar)
   {
   //variáveis locais
   double s_mva;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Smin: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Smin"));
   //determina Pot.Aparente mínima na Barra selecionada
   s_mva   = ligacao->resflow->Sf_mva_min[ind_bar][num_pat];
   //define unidade e valor
   aue.PotAparente_MVA(s_mva, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAtivaTotal(int ind_bar)
   {
   //variáveis locais
   double     p_mw;
   VTDado     *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("P: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("P"));
   //determina Pot.Ativa total na Barra selecionada
   p_mw    = ligacao->resflow->P_mw[ind_bar][num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_mw, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAtivaPorFase(int ind_bar)
   {
   //variáveis locais
   int     fases_lig;
   double  p_mw;
   VTDado  *dado;
   VTBarra *barra;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Pa"}, {faseB, "Pb"}, {faseC, "Pc"}, {faseN, "Pn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Pf: indef"));
      return;
      }
   //correntes de fase em A
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina Pot.Ativa na fase da Barra selecionada
         p_mw = ligacao->resflow->Pf_mw[ind_bar][num_pat][nf];
         //define unidade e valor
         aue.PotAtiva_MW(p_mw, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAtiva_max(int ind_bar)
   {
   //variáveis locais
   double p_mw;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Pf: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pmax"));
   //determina Pot.Ativa máxima na Barra selecionada
   p_mw   = ligacao->resflow->Pf_mw_max[ind_bar][num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_mw, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotAtiva_min(int ind_bar)
   {
   //variáveis locais
   double p_mw;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Pmin: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Pmin"));
   //determina Pot.Ativa mínima na Barra selecionada
   p_mw    = ligacao->resflow->Pf_mw_min[ind_bar][num_pat];
   //define unidade e valor
   aue.PotAtiva_MW(p_mw, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotReativaTotal(int ind_bar)
   {
   //variáveis locais
   double q_mvar;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Q: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Q"));
   //determina Pot.Reaiva total na Barra selecionada
   q_mvar  = ligacao->resflow->Q_mvar[ind_bar][num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q_mvar, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotReativaPorFase(int ind_bar)
   {
   //variáveis locais
   int    fases_lig;
   double  q_mvar;
   VTDado  *dado;
   VTBarra *barra;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Qa"}, {faseB, "Qb"}, {faseC, "Qc"}, {faseN, "Qn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Qf: indef"));
      return;
      }
   //correntes de fase em A
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Dado e insere em lisEXT
         lisEXT->Add(dado = NewObjDado(fase[nf].codigo));
         //determina Pot.Reativa na fase da Barra selecionada
         q_mvar  = ligacao->resflow->Qf_mvar[ind_bar][num_pat][nf];
         aue.PotReativa_MVAR(q_mvar, dado->valor, dado->unidade);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotReativa_max(int ind_bar)
   {
   //variáveis locais
   double q_mvar;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Qmax: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qmax"));
   //determina Pot.Reativa máxima na Barra selecionada
   q_mvar  = ligacao->resflow->Qf_mvar_max[ind_bar][num_pat];
  //define unidade e valor
   aue.PotReativa_MVAR(q_mvar, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::ValorPotReativa_min(int ind_bar)
   {
   //variáveis locais
   double q_mvar;
   VTDado *dado;

   //verifica se o é o caso de fluxo indefinido
   if (FluxoIndefinido(ind_bar))
      {
      lisEXT->Add(dado = NewObjDado("Qmin: indef"));
      return;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Qmin"));
   //determina Pot.Reativa mínima em relação à Barra selecionada
   q_mvar  = ligacao->resflow->Qf_mvar_max[ind_bar][num_pat];
   //define unidade e valor
   aue.PotReativa_MVAR(q_mvar, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorCargto(int ind_atrib)
   {
   //variáveis locais
   double      val_pu;
   AnsiString  val_str;
   VTVetval    *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Cargto", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      val_pu            = ligacao->resflow->Cargto_pu[np];
      vetval->Valor[np] = val_pu * 100.;  //pu => %
      //preenche valor como string
      aue.Carregamento_pu(val_pu, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorFP(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   double      fp_pu;
   AnsiString  val_str;
   VTVetval    *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "FP", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      fp_pu             = ligacao->resflow->FP_pu[ind_bar][np];
      vetval->Valor[np] = fp_pu;
      //preenche valor como string
      aue.FatorPotencia_pu(fp_pu, val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorFPf(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   double     fp_pu;
   int        fases_lig;
   VTBarra   *barra;
   VTVetval  *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "FPa"}, {faseB, "FPb"}, {faseC, "FPc"}, {faseN, "FPn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //loop p/ todas fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, ligacao->resflow->Max_pat));
         //preenche valores
         for (int np = 0; np < ligacao->resflow->Max_pat; np++)
            {//verifica se o é o caso de fluxo indefinido
            if (FluxoIndefinido(ind_bar))
               {
               fp_pu   = 0;
               val_str = "indef";
               }
            else
               {
               fp_pu = ligacao->resflow->FPf_pu[ind_bar][np][nf];
               aue.FatorPotencia_pu(fp_pu, val_str, vetval->unidade);
               }
            //atualiza vetval
            vetval->Valor[np]      = fp_pu;
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorIf(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> i_amp;
   int             fases_lig;
   VTBarra         *barra;
   VTVetval        *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Ia"}, {faseB, "Ib"}, {faseC, "Ic"}, {faseN, "In"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //loop p/ todas fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, ligacao->resflow->Max_pat));
         //preenche valores
         for (int np = 0; np < ligacao->resflow->Max_pat; np++)
            {//verifica se o é o caso de fluxo indefinido
            if (FluxoIndefinido(ind_bar))
               {
               i_amp   = 0;
               val_str = "indef";
               }
            else
               {
               i_amp = ligacao->resflow->If_amp[ind_bar][np][nf];
               aue.Corrente_A(i_amp, val_str, vetval->unidade);
               }
            //atualiza vetval
            vetval->Valor[np] = Abs(i_amp);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorIf_max(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   double     i_amp;
   AnsiString  val_str;
   VTVetval    *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Imax", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {//verifica se o é o caso de fluxo indefinido
      if (FluxoIndefinido(ind_bar))
         {
         i_amp   = 0;
         val_str = "indef";
         }
      else
         {
         i_amp = ligacao->resflow->If_amp_max[ind_bar][np];
         aue.Corrente_A(i_amp, val_str, vetval->unidade);
         }
      //atualiza vetval
      vetval->Valor[np] = Abs(i_amp);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorIf_min(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   double      i_amp;
   AnsiString  val_str;
   VTVetval    *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Imin", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {//verifica se o é o caso de fluxo indefinido
      if (FluxoIndefinido(ind_bar))
         {
         i_amp   = 0;
         val_str = "indef";
         }
      else
         {
         i_amp = ligacao->resflow->If_amp_min[ind_bar][np];
         aue.Corrente_A(i_amp, val_str, vetval->unidade);
         }
      //preenche valor como string
      vetval->Valor[np]      = i_amp;
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorIndiceBal(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval    *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Ind.Balanceamento", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->IndiceBal[ind_bar][np];
      //preenche valor como string
      aue.ValorPorcentual(vetval->Valor[np], val_str, vetval->unidade, "%4.3f");
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorPerda(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Perda", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Perda_mw[np];
      //preenche valor como string
      aue.PerdaAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorP(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "P", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->P_mw[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorPf(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   int        fases_lig;
   VTBarra    *barra;
   VTVetval   *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Pa"}, {faseB, "Pb"}, {faseC, "Pc"}, {faseN, "Pn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //loop p/ todas fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, ligacao->resflow->Max_pat));
         //preenche valores
         for (int np = 0; np < ligacao->resflow->Max_pat; np++)
            {
            vetval->Valor[np] = ligacao->resflow->Pf_mw[ind_bar][np][nf];
            //preenche valor como string
            vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorPf_max(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Pmax", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Pf_mw_max[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorPf_min(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Pmin", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Pf_mw_min[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorQ(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Q", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Q_mvar[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorQf(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   int        fases_lig;
   VTBarra    *barra;
   VTVetval   *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Qa"}, {faseB, "Qb"}, {faseC, "Qc"}, {faseN, "Qn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //loop p/ todas fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, ligacao->resflow->Max_pat));
         //preenche valores
         for (int np = 0; np < ligacao->resflow->Max_pat; np++)
            {
            vetval->Valor[np] = ligacao->resflow->Qf_mvar[ind_bar][np][nf];
            //sentido           = ligacao->resflow->SentidoP[ind_bar][np];
            //inverte sinal caso o sentido do fluxo seja negativo
            //if (sentido == fluxoNEG) vetval->Valor[np] *= -1.;
            //preenche valor como string
            vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorQf_max(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Qmax", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Qf_mvar_max[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorQf_min(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Qmin", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Qf_mvar_min[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorS(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> s_mva;
   VTVetval        *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "S", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      s_mva             = ligacao->resflow->S_mva[ind_bar][np];
      //preenche valor como string e salva o módulo
      vetval->Valor[np]      = Abs(aue.PotAparente_MVA(s_mva, val_str, vetval->unidade));
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorSf(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString      val_str;
   complex<double> s_mva;
   int             fases_lig;
   VTBarra         *barra;
   VTVetval        *vetval;
   struct{
         int  tag;
         char *codigo;
         } fase[MAX_FASE] = {{faseA, "Sa"}, {faseB, "Sb"}, {faseC, "Sc"}, {faseN, "Sn"}};

   //determina Barra da Ligacao
   if ((barra = ligacao->Barra(ind_bar)) == NULL) return;
   //loop p/ todas fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina fases da Ligacao em relção à Barra
      fases_lig = ligacao->Fases(barra);
      if ((fases_lig & fase[nf].tag) == fase[nf].tag)
         {//cria um novo Vetval por fase e insere em lisEXT
         lisEXT->Add(vetval = NewObjVetval(ind_atrib, fase[nf].codigo, ligacao->resflow->Max_pat));
         //preenche valores
         for (int np = 0; np < ligacao->resflow->Max_pat; np++)
            {
            s_mva             = ligacao->resflow->Sf_mva[ind_bar][np][nf];
            //preenche valor como string
            vetval->Valor[np]      = Abs(aue.PotAparente_MVA(s_mva, val_str, vetval->unidade));
            vetval->ValorAsStr[np] = val_str;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorSf_max(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString val_str;
   VTVetval   *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Smax", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Sf_mva_max[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(Abs(vetval->Valor[np]), val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaLigacao::VetValorSf_min(int ind_atrib, int ind_bar)
   {
   //variáveis locais
   AnsiString      val_str;
   VTVetval        *vetval;

   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Smin", ligacao->resflow->Max_pat));
   //preenche valores
   for (int np = 0; np < ligacao->resflow->Max_pat; np++)
      {
      vetval->Valor[np] = ligacao->resflow->Sf_mva_min[ind_bar][np];
      //preenche valor como string
      vetval->Valor[np]      = aue.PotAparente_MVA(Abs(vetval->Valor[np]), val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }

//---------------------------------------------------------------------------
//eof



