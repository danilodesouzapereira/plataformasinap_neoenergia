//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Constante\Const.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Sinap\VTSinapChild.h"
#include <DLL_Inc\Funcao.h>
#include "TMedicao.h"
#include "VTTensao.h"
#include "..\ImportaMedicao\EstruturaMed.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMedicao*  __fastcall NewObjMedicao(VTApl *apl)
   {
   try{
      return(new TMedicao(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMedicao::TMedicao(VTApl *apl)
   {
   //saalva ponteiro p/ objetos
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria listas locais
   lisEQP    = new TList();
   lisTENSAO = new TList();
   //inicia dados
   ask_user = true;
   //inicia todas fases não disponíveis
   FaseEnabled[indFASE_A] = false;
   FaseEnabled[indFASE_B] = false;
   FaseEnabled[indFASE_C] = false;
   FaseEnabled[indNEUTRO] = false;
   }

//---------------------------------------------------------------------------
__fastcall TMedicao::~TMedicao(void)
   {
   //destrói lista e seus objetos
   if (lisTENSAO) {LimpaTList(lisTENSAO); delete lisTENSAO; lisTENSAO = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMedicao::AlteraMedicao(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE])
   {
   //variáveis locais
   VTTensao *tensao;

   //verifica se existe Tensao p/ o dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   //verifica se Tensao do dia possui a medição do patamar indicado
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   if (! tensao->Enabled[ind_pat]) return(false);
   //copia os valores de tensão
   tensao->InsereTensao(ind_pat, vfn_kv);
   //habilita medições não nulas
   PD.fase_enabled[indFASE_A] = (! IsDoubleZero(vfn_kv[indFASE_A]));
   PD.fase_enabled[indFASE_B] = (! IsDoubleZero(vfn_kv[indFASE_B]));
   PD.fase_enabled[indFASE_C] = (! IsDoubleZero(vfn_kv[indFASE_C]));

   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TMedicao::DeterminaNumeroMedicaoPorDia(strREGISTRO &reg1, strREGISTRO &reg2)
   {
   //variáveis locais
   int dt_min;

   //proteção contra virada hora cheia
   if (reg2.minuto >= reg1.minuto) {dt_min =      reg2.minuto - reg1.minuto;}
   else                            {dt_min = 60 + reg2.minuto - reg1.minuto;}
   //verifica se o número de minutos é válido
   if (dt_min <= 0) return(false);
   //determina número de medições por dia
   PD.num_medicao_dia = 24 * (60 / dt_min);
   //altera número de patamares
   RedefinePatamares();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMedicao::ExisteMedicao(TDateTime data, int ind_pat)
   {
   //variáveis locais
   VTTensao *tensao;

   //verifica se existe Tensao p/ a data
   if ((tensao = ExisteTensao(data)) == NULL) return(false);
   //verifica se existe medição no patamar
   return(tensao->Enabled[ind_pat]);
   }

//---------------------------------------------------------------------------
bool __fastcall TMedicao::ExisteMedicao(TDateTime data, int ind_pat, double vfn_kv[MAX_FASE])
   {
   //variáveia locais
   VTTensao *tensao;

   //verifica se existe Tensao p/ o dia indicado
   if ((tensao = ExisteTensao(data)) == NULL) return(false);
   //verifica se Tensao do dia possui a medição do patamar indicado
   if (! tensao->Enabled[ind_pat]) return(false);
   //copia os valores de tensão
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vfn_kv[ind_fase] = tensao->ModVfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMedicao::ExisteMedicao(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE])
   {
   //variáveis locais
   VTTensao *tensao;

   //verifica se existe Tensao p/ o dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   //verifica se Tensao do dia possui a medição do patamar indicado
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];
   if (! tensao->Enabled[ind_pat]) return(false);
   //copia os valores de tensão
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vfn_kv[ind_fase] = tensao->ModVfn_kv[ind_pat][ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTTensao* __fastcall TMedicao::ExisteTensao(TDateTime data)
   {
   return(ExisteTensao(DayOf(data), MonthOf(data), YearOf(data)));
   }

//---------------------------------------------------------------------------
VTTensao* __fastcall TMedicao::ExisteTensao(int dia, int mes, int ano)
   {
   //variáveis locais
   VTTensao *tensao;

   for (int n = 0; n < lisTENSAO->Count; n++)
      {
      tensao = (VTTensao*)lisTENSAO->Items[n];
      if ((tensao->Ano == ano)&&(tensao->Mes == mes)&&(tensao->Dia == dia)) return(tensao);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
int  __fastcall TMedicao::InsereMedicao(strREGISTRO &reg)
   {
   //verifica se é o primeiro registro de medição
   if (reg.num_seq == 1)
      {//apenas salva o primeiro registro à espera do segundo p/ calcular a taxa de amostragem
      reg_ini = reg;
      return(diagOK);
      }
   //verifica se os registros são consecutivos
   if (reg.num_seq == (reg_ini.num_seq + 1))
      {//registros consecutivos: calcula taxa de amostragem
      if (DeterminaNumeroMedicaoPorDia(reg_ini, reg))
         {//salva os dois registros de medição
         if (! SalvaMedicao(reg))     return(diagCONTINUA);
         if (  SalvaMedicao(reg_ini)) return(diagCONTINUA);
         }
      else
         {//não foi possível calcular a taxa de amostragem: aguarda o próximo registro
         reg_ini = reg;
         }
      }
   else
      {//salva o registro de medição
      if (! SalvaMedicao(reg)) return(diagCONTINUA);
      }
   return(diagOK);
   }
*/
//---------------------------------------------------------------------------
int  __fastcall TMedicao::InsereMedicao(strREGISTRO &reg)
   {
   //variaveis locais
   int resp_reg, resp_ini;

   //verifica se é o primeiro registro de medição
   if (reg.num_seq == 1)
      {//apenas salva o primeiro registro à espera do segundo p/ calcular a taxa de amostragem
      reg_ini = reg;
      return(diagOK);
      }
   //verifica se os registros são consecutivos
   if (reg.num_seq == (reg_ini.num_seq + 1))
      {//registros consecutivos: calcula taxa de amostragem
      if (DeterminaNumeroMedicaoPorDia(reg_ini, reg))
         {//salva os dois registros de medição
         resp_reg = SalvaMedicao(reg);
         resp_ini = SalvaMedicao(reg_ini);
         if(resp_reg == diagINTERROMPE) return(diagINTERROMPE);
         if (resp_reg != diagOK)        return(diagCONTINUA);
         if (resp_ini == diagOK)        return(diagCONTINUA);
         }
      else
         {//não foi possível calcular a taxa de amostragem: aguarda o próximo registro
         reg_ini = reg;
         }
      }
   else
      {//salva o registro de medição
      resp_reg = SalvaMedicao(reg);
      if (resp_reg == diagINTERROMPE) return(diagINTERROMPE);
      if (resp_reg != diagOK)         return(diagCONTINUA);
      }
   return(diagOK);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TMedicao::PM_GetCarga(void)
   {
   return(PD.carga);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TMedicao::PM_GetData(int ind_dia)
   {
   //variáveis locais
   VTTensao *tensao;

   //verifica se existe Tensao p/ o dia indicado
   if ((ind_dia < 0)||(ind_dia >= lisTENSAO->Count)) return(false);
   //determina Tensao do dia indicado
   tensao = (VTTensao*)lisTENSAO->Items[ind_dia];

   return(tensao->Data);
   }


//---------------------------------------------------------------------------
bool  __fastcall TMedicao::PM_GetFaseEnabled(int ind_fase)
   {
   return(PD.fase_enabled[ind_fase]);
   }

//---------------------------------------------------------------------------
int  __fastcall TMedicao::PM_GetFasesRede(void)
   {
   return(PD.fases_rede);
   }

//---------------------------------------------------------------------------
int __fastcall TMedicao::PM_GetNumeroDia(void)
   {
   return(lisTENSAO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TMedicao::PM_GetNumeroMedicaoPorDia(void)
   {
   return(PD.num_medicao_dia);
   }

//---------------------------------------------------------------------------
int __fastcall TMedicao::PM_GetNumeroMedicaoTotal(void)
   {
   return(NumeroDia * NumeroMedicaoPorDia);
   }

//---------------------------------------------------------------------------
void  __fastcall TMedicao::PM_SetCarga(VTCarga* carga)
   {
   PD.carga = carga;
   }

//---------------------------------------------------------------------------
void  __fastcall TMedicao::PM_SetFaseEnabled(int ind_fase, bool enabled)
    {
   PD.fase_enabled[ind_fase] = enabled;
   }

//---------------------------------------------------------------------------
void  __fastcall TMedicao::PM_SetFasesRede(int fases_rede)
    {
   PD.fases_rede = fases_rede;
   }

//---------------------------------------------------------------------------
void __fastcall TMedicao::RedefinePatamares(void)
   {
   //variáveis locais
   int          escala;
   VTPatamares  *patamares   = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   switch(NumeroMedicaoPorDia)
      {
      case  24: escala = pat01HORA; break;
      case  96: escala = pat15MIN;  break;
      case 144: escala = pat10MIN;  break;
      case 288: escala = pat05MIN;  break;
      default:  escala = pat10MIN;  break;
      }
   //verifica se é necessário alterar número de patamares
   if (patamares->Escala != escala)
      {//redefine número de patamares
      sinap_child->DefinePatamares(escala);
      }
   }

//---------------------------------------------------------------------------
void  __fastcall TMedicao::Reinicia(void)
   {
   LimpaTList(lisTENSAO);
   }

//---------------------------------------------------------------------------
int  __fastcall TMedicao::SalvaMedicao(strREGISTRO &reg)
   {
   //variáveis locais
   VTFases         *fases = (VTFases*)apl->GetObject(__classid(VTFases));
   VTPatamar       *patamar;
   VTTensao        *tensao;
   complex<double> vfn_kv[MAX_FASE], valor_aux;
   int             resp;

   //determina o patamar do registro
   if ((patamar = patamares->ExistePatamar(reg.hora, reg.minuto)) == NULL) return(diagCONTINUA);
   //proteção contra valores nulos
   if (IsDoubleZero(reg.v1) && IsDoubleZero(reg.v2) && IsDoubleZero(reg.v3))
      {//copia último registro não nulo
      reg.v1_cmplx = ult_reg.v1_cmplx;
      reg.v2_cmplx = ult_reg.v2_cmplx;
      reg.v3_cmplx = ult_reg.v3_cmplx;
      //pergunta 1 única vez se o usuário quer continuar a importação
      if (ask_user)
         {//marca flag p/ não pedir nova confirmação do usuário
         ask_user = false;
         resp = Confirma ("O arquivo de medição contém valores nulos de tensão.",
                          "Deseja importar assim mesmo?");
         if(resp != IDYES) return(diagINTERROMPE);
         }
      }
   else
      {//salva último registro não nulo
      ult_reg.v1_cmplx = reg.v1_cmplx;
      ult_reg.v2_cmplx = reg.v2_cmplx;
      ult_reg.v3_cmplx = reg.v3_cmplx;
      }
   //verifica se a rede é monofásica e se a medição é válida para a fase da rede
   if(fases->NumeroDeFases(PD.fases_rede) == 1)
      {
      if(! IsDoubleZero(reg.v1)) valor_aux = reg.v1_cmplx;
      if(! IsDoubleZero(reg.v2)) valor_aux = reg.v2_cmplx;
      if(! IsDoubleZero(reg.v3)) valor_aux = reg.v3_cmplx;
      //assume valor não nulo para fase
      if(PD.fases_rede == faseA) {reg.v1_cmplx = valor_aux; FaseEnabled[indFASE_A] = true;}
      if(PD.fases_rede == faseB) {reg.v2_cmplx = valor_aux; FaseEnabled[indFASE_B] = true;}
      if(PD.fases_rede == faseC) {reg.v3_cmplx = valor_aux; FaseEnabled[indFASE_C] = true;}
      }

   //verifica se existe objeto Tensao p/ a data indicada
   if ((tensao = ExisteTensao(reg.dia, reg.mes, reg.ano)) == NULL)
      {//cria e inicia um novo objeto Tensao
      tensao = NewObjTensao(NumeroMedicaoPorDia);
      tensao->Dia = reg.dia;
      tensao->Mes = reg.mes;
      tensao->Ano = reg.ano;
      lisTENSAO->Add(tensao);
      }
   //proteção
   if (tensao == NULL) return(diagCONTINUA);
   //insere os valores das tensões de fase em Tensao
   vfn_kv[indFASE_A] = reg.v1_cmplx / 1000.;
   vfn_kv[indFASE_B] = reg.v2_cmplx / 1000.;
   vfn_kv[indFASE_C] = reg.v3_cmplx / 1000.;
   vfn_kv[indNEUTRO] = CZero();

   if(! tensao->InsereTensao(patamar->Index, vfn_kv)) return(diagCONTINUA);
   return(diagOK);
   }

//---------------------------------------------------------------------------
//eof

