//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<Math.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Planejamento\VTAlternativa.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Potencia\VTFornecimento.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Zona\VTZona.h>
#include <Fontes\Zona\VTZonas.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
#include "TEt.h"
#include "VTAjusta.h"
#include "VTMedicao.h"
#include "VTPerfilAlt.h"
#include "VTPerfilBarra.h"
#include "VTPerfilCarga.h"
#include "VTPerfilRede.h"
///---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEt* __fastcall NewObjEt(VTApl *apl)
   {
   try{
      return(new TEt(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TEt::TEt(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   //lisTRF = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEt::~TEt(void)
   {
   //destrói lista sem destruir  seus objetos
   //if (lisTRF) {delete lisTRF; lisTRF = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TEt::CorrigeDemandaCarga(VTCarga* carga, int ind_pat, double *fatcor)
   {
   //variáveis locais
   complex<double> saux_mva[MAX_FASE];
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //proteção
//   if(carga->IP) return(true);
   if(! demanda->Get_S3f_mva(carga, ind_pat, saux_mva)) return(true);
   for(int nf=0; nf < MAX_FASE; nf++)
      {
      saux_mva[nf] *= fatcor[nf];
      }
   if(! demanda->Set_S3f_mva(carga, ind_pat, saux_mva)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEt::DesequilibraDemandaCarga(VTCarga* carga, int ind_pat, double *deseq)
   {
   //variáveis locais
   double          deseq_total;
   complex<double> saux_mva[MAX_FASE];
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   // Obtém demanda por fase
   if(! demanda->Get_S3f_mva(carga, ind_pat, saux_mva)) return(true);

   //determina total de desequilíbrio, considerando fases da Carga
   deseq_total = 0.;
   for (int ind_fase = 0, fase = faseA; ind_fase < MAX_FASE; ind_fase++, fase++)
      {//verifica se Carga possui a fase
      if ((carga->Fases & fase) == fase) deseq_total += deseq[ind_fase];
      }
   //redefine demanda por fase
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      saux_mva[nf] *= (1.0 - deseq_total + deseq[nf]);
      }

   // Atualiza demanda por fase
   if(! demanda->Set_S3f_mva(carga, ind_pat, saux_mva)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TEt::EstimaMedicao(double vfn_med_kv[MAX_FASE], int ind_pat, int ind_fase)
   {
   //variáveis locais
   double   valor_est = 0.;
   double   fat_cor, valor_med_nao_nulo, valor_cal_nao_nulo, valor_cal;
   int      ind_fase_nao_nula;

   //inicia variável
   valor_med_nao_nulo = 0.;
   for(int ind = indFASE_A; ind <= MAX_FASE; ind++)
      {//guarda valor medido não nulo e fase
      if(! IsDoubleZero(valor_med_nao_nulo)) break;
      valor_med_nao_nulo = vfn_med_kv[ind];
      ind_fase_nao_nula  = ind;
      }
   //determina valor do fluxo na mesma fase do medido não nulo
   valor_cal_nao_nulo = Abs(barra_car->resflow->Vfn_kv[ind_pat][ind_fase_nao_nula]);
   //determina valor do fluxo na fase selecionada
   valor_cal          = Abs(barra_car->resflow->Vfn_kv[ind_pat][ind_fase]);
   //proteção: sem fluxo, assume medido não nulo
   if(IsDoubleZero(valor_cal_nao_nulo)) return(valor_med_nao_nulo);
   //calcula fator de correção
   fat_cor = valor_med_nao_nulo / valor_cal_nao_nulo;
   //calcula valor medido estimado
   valor_est = valor_cal * fat_cor;
   return(valor_est);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TEt::MontaPerfilDemandaFinal(TList *lisCARGA, VTBarra *barra_ini, VTBarra *barra_car)
   {
   //variáveis locais
   double          a, b;
   double          vfn_cal_bar_car_kv[MAX_FASE];
   double          vfn_cal_bar_ini_kv[MAX_FASE];
   double          vfn_med_bar_car_kv[MAX_FASE];
   double          fatcor[MAX_FASE];
   VTCarga         *carga;
   //VTPerfilCarga   *perfil_carga;
   VTPerfilBarra   *perfil_barra_car;
   VTPerfilBarra   *perfil_barra_ini;
   VTAjusta        *ajusta    = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTMedicao       *medicao   = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém PerfilBarra da barra inicial e da barra do consumidor
   perfil_barra_car = ajusta->PerfilRedeInicial->ExistePerfilBarra(barra_car);
   perfil_barra_ini  = ajusta->PerfilRedeInicial->ExistePerfilBarra(barra_ini);
   //loop p/ todos dias de medição
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//atualiza curva de demanda das Cargas com as demandas salvas em PerfilDemanda
      ajusta->PerfilRedeInicial->RestauraPerfilCarga(medicao->Data[ind_dia]);
      //loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//obtém tensão medida por fase
         if (! medicao->ExisteMedicao  (ind_dia, ind_pat, vfn_med_bar_car_kv)) continue;
         if (! perfil_barra_car->Vfn_kv(ind_dia, ind_pat, vfn_cal_bar_car_kv)) continue;
         if (! perfil_barra_ini->Vfn_kv(ind_dia, ind_pat, vfn_cal_bar_ini_kv)) continue;
         //determina fator de correção de demanda por fase
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {
            a = vfn_cal_bar_ini_kv[ind_fase] - vfn_cal_bar_car_kv[ind_fase];
            b = vfn_cal_bar_car_kv[ind_fase] - vfn_med_bar_car_kv[ind_fase];
if (IsDoubleZero(b)) b  = 0;
            if        (IsDoubleZero(b))  fatcor[ind_fase] = 1.;
            else if   (IsDoubleZero(a))  fatcor[ind_fase] = 1.;
            else                         fatcor[ind_fase] = (1. + (b/a));
            //proteção contra fatores muito elevados
            //if (fatcor[ind_fase] > 1) fatcor[ind_fase] = 1. + (fatcor[ind_fase] * 0.4);
            if      (fatcor[ind_fase] > 8)
               {fatcor[ind_fase] = 3.0 + (fatcor[ind_fase] * 0.2);}
            else if (fatcor[ind_fase] > 4)
               {fatcor[ind_fase] = 1.2 + (fatcor[ind_fase] * 0.4);}
            else if (fatcor[ind_fase] > 1)
               {fatcor[ind_fase] = 0.4 + (fatcor[ind_fase] * 0.6);}
            }
         fatcor[indNEUTRO] = 1.;
        //corrige demanda de todas as Cargas, aplicando o fator de correção por fase
         for (int nc = 0; nc < lisCARGA->Count; nc++)
            {
            carga = (VTCarga*)lisCARGA->Items[nc];
            CorrigeDemandaCarga(carga, ind_pat, fatcor);
            }
         }
      //torna a salvar o perfil de demanda das Cargas
      ajusta->PerfilRedeInicial->SalvaPerfilCarga(medicao->Data[ind_dia]);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TEt::MontaPerfilDemandaFinal(TList *lisCARGA, VTBarra *barra_ini, VTBarra *barra_car)
   {
//return;
   //variáveis locais
   double          a, b;
   double          vfn_cal_bar_car_kv[MAX_FASE];
   double          vfn_cal_bar_ini_kv[MAX_FASE];
   double          vfn_med_bar_car_kv[MAX_FASE];
   double          fatcor[MAX_FASE];
   VTCarga         *carga;
   //VTPerfilCarga   *perfil_carga;
   VTPerfilAlt     *perfil_alt_padrao;
   VTPerfilBarra   *perfil_barra_car;
   VTPerfilBarra   *perfil_barra_ini;
   VTAjusta        *ajusta    = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTMedicao       *medicao   = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTPerfilAlt     *perfil_alt;

   //obtém PerfilBarra da barra inicial e da barra do consumidor
   perfil_alt_padrao = (VTPerfilAlt*)ajusta->LisPerfilAlt->Items[0];
   perfil_barra_car = perfil_alt_padrao->ExistePerfilBarra(barra_car);
   perfil_barra_ini = perfil_alt_padrao->ExistePerfilBarra(barra_ini);
   //proteção //DVK 2015.03.23
   if((! perfil_barra_car) || (! perfil_barra_ini)) return;
   //loop p/ todos dias de medição
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//atualiza curva de demanda das Cargas com as demandas salvas em PerfilDemanda
      ajusta->PerfilRedeInicial->RestauraPerfilCarga(medicao->Data[ind_dia]);
      //loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//obtém tensão medida por fase
         if (! medicao->ExisteMedicao  (ind_dia, ind_pat, vfn_med_bar_car_kv)) continue;
         if (! perfil_barra_car->Vfn_kv(ind_dia, ind_pat, vfn_cal_bar_car_kv)) continue;
         if (! perfil_barra_ini->Vfn_kv(ind_dia, ind_pat, vfn_cal_bar_ini_kv)) continue;
         //determina fator de correção de demanda por fase
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {//verifica se existe medição p/ fase
            if (! medicao->FaseEnabled[ind_fase]) continue;
            a = vfn_cal_bar_ini_kv[ind_fase] - vfn_cal_bar_car_kv[ind_fase];
            b = vfn_cal_bar_car_kv[ind_fase] - vfn_med_bar_car_kv[ind_fase];
            if        (IsDoubleZero(b))  fatcor[ind_fase] = 1.;
            else if   (IsDoubleZero(a))  fatcor[ind_fase] = 1.;
            else if   (b < 0)            fatcor[ind_fase] = 1. / (1. + (fabs(b)/a));
            else                         fatcor[ind_fase] = (1. + (b/a));
            //proteção contra fatores muito elevados
            //if (fatcor[ind_fase] > 1) fatcor[ind_fase] = 1. + (fatcor[ind_fase] * 0.4);
            if      (fatcor[ind_fase] > 8)
               {fatcor[ind_fase] = 3.0 + (fatcor[ind_fase] * 0.2);}
            else if (fatcor[ind_fase] > 4)
               {fatcor[ind_fase] = 1.2 + (fatcor[ind_fase] * 0.4);}
            else if (fatcor[ind_fase] > 1)
               {fatcor[ind_fase] = 0.4 + (fatcor[ind_fase] * 0.6);}
            }
         fatcor[indNEUTRO] = 1.;
        //corrige demanda de todas as Cargas, aplicando o fator de correção por fase
         for (int nc = 0; nc < lisCARGA->Count; nc++)
            {
            carga = (VTCarga*)lisCARGA->Items[nc];
            CorrigeDemandaCarga(carga, ind_pat, fatcor);
            }
         }
      //torna a salvar o perfil de demanda das Cargas
      ajusta->PerfilRedeInicial->SalvaPerfilCarga(medicao->Data[ind_dia]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEt::MontaPerfilDemandaInicial(TList *lisCARGA)
   {
   //variáveis locais
   VTAjusta  *ajusta  = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTMedicao *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));

   //inicia PerfilCarga de todas Cargas, com valores iguais p/ todos os dias
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {
      ajusta->PerfilRedeInicial->SalvaPerfilCarga(medicao->Data[ind_dia]);
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TEt::MontaPerfilTensaoSuprimento(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_car)
   {
   //variáveis locais
   double          fatmult = 1.00;
   double          p_mw = 0,  q_mvar = 0;
   //double          ang_vfn_rad[MAX_FASE] = {0, DegToRad(240.), DegToRad(120.), 0};
   double          ang_vfn_rad[MAX_FASE] = {0, 0, 0, 0};
   double          vfn_nom_kv, vfn_cal_kv, vfn_pu, vfn_pu_max, vsup_pu;
   double          vfn_med_kv[MAX_FASE];
   //VTPerfilSup     *perfil_sup;
   VTAjusta        *ajusta       = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTListaSupFase  *listaSF      = fornecimento->ListaSupFase();;
   VTMedicao       *medicao      = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTPatamares     *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //if ((perfil_sup = ajusta->PerfilRedeInicial->ExistePerfilSup(suprimento)) == NULL) return;
   //determina tensão de fase nominal da Barra da Carga
   vfn_nom_kv = barra_car->zona->Vfn_kv[indFASE_A];
   //loop p/ todos dias da medição
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         if (medicao->ExisteMedicao(ind_dia, ind_pat, vfn_med_kv))
            {//determina tensão máxima em pu p/ o suprimento
            vfn_pu_max = 0.;
            for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
               {//verifica se existe medição p/ a fase
               if (! medicao->FaseEnabled[ind_fase]) continue;
               vfn_cal_kv = Abs(barra_car->resflow->Vfn_kv[ind_pat][ind_fase]);
               vfn_pu     = 1.0 + ((vfn_med_kv[ind_fase] - vfn_cal_kv)/vfn_nom_kv);
               if (vfn_pu > vfn_pu_max) vfn_pu_max = vfn_pu;
               }
            //define tensão em pu para o Suprimento (igual p/ todas as fases)
            vsup_pu = fatmult * vfn_pu_max;
            }
         else
            {//não existe medição: assume 1 pu
            vsup_pu = 1.0;
            }
         //configura todas as tensões de fase do Suprimento iguais a maior tensão de fase da medição
         listaSF->Clear();
         //define PQVT por fase do suprimento
         for (int ind_fase = indFASE_A, fase = faseA; ind_fase <= indFASE_C; ind_fase++, fase++)
            {
            listaSF->Add(fase, suprimento->tiposup, p_mw,  q_mvar, vsup_pu, ang_vfn_rad[ind_fase]);
            }
         listaSF->Add(faseN, suprimento->tiposup, 0, 0, 0, 0);
         fornecimento->PQVT[suprimento][ind_pat] = listaSF;
         }
      //atualiza perfil de tensão do suprimento no dia
      //perfil_sup->SalvaTensao(medicao->Data[ind_dia], medicao);
      ajusta->PerfilRedeInicial->SalvaPerfilSuprimento(medicao->Data[ind_dia]);
      }
   }
*/
/*

//---------------------------------------------------------------------------
void __fastcall TEt::MontaPerfilTensaoSuprimento(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_car)
   {
   //variáveis locais
   int             num_fase;
   double          ang_vfn_rad[MAX_FASE] = {0, DegToRad(240.), DegToRad(120.), 0};
   //double          ang_vfn_rad[MAX_FASE] = {0, 0, 0, 0};
   double          vfn_nom_kv, vfn_cal_kv, vfn_pu[MAX_FASE], vfn_pu_med, vsup_pu;
   double          vfn_med_kv[MAX_FASE];
   strSUP          *str_sup, vet_sup[MAX_FASE];
   //VTPerfilSup     *perfil_sup;
   VTAjusta        *ajusta       = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTMedicao       *medicao      = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTPatamares     *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //if ((perfil_sup = ajusta->PerfilRedeInicial->ExistePerfilSup(suprimento)) == NULL) return;
   //determina tensão de fase nominal da Barra da Carga
   vfn_nom_kv = barra_car->zona->Vfn_kv[indFASE_A];
   //loop p/ todos dias da medição
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         if (medicao->ExisteMedicao(ind_dia, ind_pat, vfn_med_kv))
            {//determina tensões de fase em pu p/ o suprimento
            num_fase = 0;
            for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
               {
               vfn_pu[ind_fase] = 0.;
               //verifica se existe medição p/ a fase
               if (! medicao->FaseEnabled[ind_fase]) continue;
					vfn_cal_kv       = Abs(barra_car->resflow->Vfn_kv[ind_pat][ind_fase]);
               vfn_pu[ind_fase] = 1.0 + ((vfn_med_kv[ind_fase] - vfn_cal_kv)/vfn_nom_kv);
               num_fase++;
               }
            //define tensão média em pu para o Suprimento (igual p/ todas as fases)
            vsup_pu = ((vfn_pu[indFASE_A] + vfn_pu[indFASE_B] + vfn_pu[indFASE_C]) / num_fase);
            }
         else
            {//não existe medição: assume 1 pu
            vsup_pu = 1.0;
            }
         //configura todas as tensões de fase do Suprimento iguais a maior tensão de fase da medição
         //define PQVT por fase do suprimento
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {
            str_sup = vet_sup + ind_fase;  // Obtém endereço no vetor
            str_sup->p = 0.;
            str_sup->q = 0.;
            str_sup->v = vsup_pu;
            str_sup->t = ang_vfn_rad[ind_fase];
            }
         str_sup    = vet_sup + indNEUTRO;
         str_sup->p = 0.; str_sup->q = 0.; str_sup->v = 0.; str_sup->t = 0.;
         fornecimento->Set_PQVT_porfase(suprimento, ind_pat, vet_sup);
         }
      //atualiza perfil de tensão do suprimento no dia
      //perfil_sup->SalvaTensao(medicao->Data[ind_dia], medicao);
      ajusta->PerfilRedeInicial->SalvaPerfilSuprimento(medicao->Data[ind_dia]);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TEt::MontaPerfilTensaoSuprimento(VTSuprimento *suprimento,
                                                VTBarra *barra_ini, VTBarra *barra_car)
   {
   //variáveis locais
   int             num_fase, fase_tag;
   double          ang_vfn_rad[MAX_FASE] = {0, DegToRad(240.), DegToRad(120.), 0};
   //double          ang_vfn_rad[MAX_FASE] = {0, 0, 0, 0};
   double          vfn_nom_kv, vfn_cal_kv, vfn_pu[MAX_FASE], vfn_pu_med, vsup_pu;
   double          vfn_med_kv[MAX_FASE];
   strSUP          *str_sup, vet_sup[MAX_FASE];
   //VTPerfilSup     *perfil_sup;
   VTAjusta        *ajusta       = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTFases         *fases        = (VTFases*)apl->GetObject(__classid(VTFases));
   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTMedicao       *medicao      = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   VTPatamares     *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //salva ponteiro
   this->barra_car = barra_car;
   //if ((perfil_sup = ajusta->PerfilRedeInicial->ExistePerfilSup(suprimento)) == NULL) return;
   //determina tensão de fase nominal da Barra da Carga
   vfn_nom_kv = barra_car->zona->Vfn_kv[indFASE_A];
   if(IsDoubleZero(vfn_nom_kv)) vfn_nom_kv = barra_car->zona->Vfn_kv[indFASE_B];
   if(IsDoubleZero(vfn_nom_kv)) vfn_nom_kv = barra_car->zona->Vfn_kv[indFASE_C];
   //loop p/ todos dias da medição
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//determina tensões de fase em pu p/ o suprimento
         num_fase = 0;
         if (medicao->ExisteMedicao(ind_dia, ind_pat, vfn_med_kv))
            {
            for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
               {
               vfn_pu[ind_fase] = 0.;
               //verifica se existe a fase na rede
               fase_tag = fases->IndexToTag[ind_fase];
               if ((fase_tag & medicao->FasesRede) != fase_tag) continue;
               //determina medição p/ a fase (pode ser estimada em função do fluxo)
               vfn_cal_kv       = Abs(barra_car->resflow->Vfn_kv[ind_pat][ind_fase]);
               if(IsDoubleZero(vfn_med_kv[ind_fase]))
                  {//altera medição de forma proporcional ao fluxo
                  vfn_med_kv[ind_fase] = EstimaMedicao(vfn_med_kv, ind_pat, ind_fase);
                  medicao->AlteraMedicao(ind_dia, ind_pat, vfn_med_kv);
                  }
               //calcula tensão por fase em pu
               vfn_pu[ind_fase] = 1.0 + ((vfn_med_kv[ind_fase] - vfn_cal_kv)/vfn_nom_kv);
               num_fase++;
               }
            //define tensão média em pu para o Suprimento (igual p/ todas as fases)
            vsup_pu = ((vfn_pu[indFASE_A] + vfn_pu[indFASE_B] + vfn_pu[indFASE_C]) / num_fase);
            }
         else
            {//não existe medição: assume 1 pu
            vsup_pu = 1.0;
            }
         //configura todas as tensões de fase do Suprimento iguais a maior tensão de fase da medição
         //define PQVT por fase do suprimento
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {
            str_sup = vet_sup + ind_fase;  // Obtém endereço no vetor
            str_sup->p = 0.;
            str_sup->q = 0.;
            str_sup->v = vsup_pu;
            str_sup->t = ang_vfn_rad[ind_fase];
            }
         str_sup    = vet_sup + indNEUTRO;
         str_sup->p = 0.; str_sup->q = 0.; str_sup->v = 0.; str_sup->t = 0.;
         fornecimento->Set_PQVT_porfase(suprimento, ind_pat, vet_sup);
         }
      //atualiza perfil de tensão do suprimento no dia
      //perfil_sup->SalvaTensao(medicao->Data[ind_dia], medicao);
      ajusta->PerfilRedeInicial->SalvaPerfilSuprimento(medicao->Data[ind_dia]);
      }
   }

//---------------------------------------------------------------------------
//eof


