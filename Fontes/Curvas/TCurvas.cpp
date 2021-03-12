//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TCurvas.h"
#include "VTCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurvas* __fastcall NewObjCurvas(VTApl *apl)
   {
   return(new TCurvas(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCurvas::TCurvas(VTApl *apl)
   {
   try{//salva ponteiro p/ objetos
      this->apl = apl;
      //cria listas de Curvas
      lisCUR_TIP_CAP = new TList();
      lisCUR_TIP_CAR = new TList();
      lisCUR_TIP_IP  = new TList();
      lisCUR_TIP_REA = new TList();
      lisCUR_TIP_GFV = new TList();
      //cria listas p/ Curvas duplicados
      dup.lisCUR_TIP_CAP = new TList();
      dup.lisCUR_TIP_CAR = new TList();
      dup.lisCUR_TIP_IP  = new TList();
      dup.lisCUR_TIP_REA = new TList();
      dup.lisCUR_TIP_GFV = new TList();
      //define data de referência
      DataReferencia = Now();
      //define dia de medição das curvas típicas
      DiaMedicao = SEMANA_COMPLETA;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TCurvas::~TCurvas(void)
   {
   //destrói Curvas
   Clear();
   //destrói listas de curvas
   if (lisCUR_TIP_CAP) {delete lisCUR_TIP_CAP; lisCUR_TIP_CAP = NULL;}
   if (lisCUR_TIP_CAR) {delete lisCUR_TIP_CAR; lisCUR_TIP_CAR = NULL;}
   if (lisCUR_TIP_IP)  {delete lisCUR_TIP_IP;  lisCUR_TIP_IP  = NULL;}
   if (lisCUR_TIP_REA) {delete lisCUR_TIP_REA; lisCUR_TIP_REA = NULL;}
   if (lisCUR_TIP_GFV) {delete lisCUR_TIP_GFV; lisCUR_TIP_GFV = NULL;}
   //destrói listas de curvas duplicadas
   if (dup.lisCUR_TIP_CAP) {delete dup.lisCUR_TIP_CAP; dup.lisCUR_TIP_CAP = NULL;}
   if (dup.lisCUR_TIP_CAR) {delete dup.lisCUR_TIP_CAR; dup.lisCUR_TIP_CAR = NULL;}
   if (dup.lisCUR_TIP_IP)  {delete dup.lisCUR_TIP_IP;  dup.lisCUR_TIP_IP  = NULL;}
   if (dup.lisCUR_TIP_REA) {delete dup.lisCUR_TIP_REA; dup.lisCUR_TIP_REA = NULL;}
   if (dup.lisCUR_TIP_GFV) {delete dup.lisCUR_TIP_GFV; dup.lisCUR_TIP_GFV = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TCurvas::Clear(void)
   {
   LimpaTList(lisCUR_TIP_CAP);
   LimpaTList(lisCUR_TIP_CAR);
   LimpaTList(lisCUR_TIP_IP);
   LimpaTList(lisCUR_TIP_REA);
   LimpaTList(lisCUR_TIP_GFV);
   //curva duplicadas
   LimpaTList(dup.lisCUR_TIP_CAP);
   LimpaTList(dup.lisCUR_TIP_CAR);
   LimpaTList(dup.lisCUR_TIP_IP);
   LimpaTList(dup.lisCUR_TIP_REA);
   LimpaTList(dup.lisCUR_TIP_GFV);
   }

//---------------------------------------------------------------------------
void __fastcall TCurvas::DeterminaDiasMes(TDateTime data)
   {
   //variáveis locais
   unsigned short mes = MonthOf(data);
   unsigned short ano = YearOf(data);

   //inicia estrutura dias_mes
   num_dias.dom = 0;
   num_dias.sab = 0;
   num_dias.dut = 0;
   num_dias.mes = DaysInMonth(data);
   //determina número de dias úteis, sábados e domingos
   for (unsigned short dia = 1; dia <= num_dias.mes; dia++)
      {
      data = TDateTime(ano, mes, dia);
      switch(DayOfWeek(data))
         {
         case DaySunday:   num_dias.dom++; break;
         case DaySaturday: num_dias.sab++; break;
         default:          num_dias.dut++; break;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvas::ExisteAlteracao(void)
   {
   //variáveis locais
   VTCurva *curva;
   TList   *lisCUR;
   TList   *vetLIST[] = {lisCUR_TIP_CAP, lisCUR_TIP_CAR, lisCUR_TIP_IP, lisCUR_TIP_REA, lisCUR_TIP_GFV};

   //loop p/ todas Curvas
   for (int index = 0; index < sizeof(vetLIST)/sizeof(TList*); index++)
      {
      lisCUR = vetLIST[index];
      for (int n = 0; n < lisCUR->Count; n++)
         {
         curva = (VTCurva*)lisCUR->Items[n];
         if (curva->Status[sttNOVO])     return(true);
         if (curva->Status[sttALTERADO]) return(true);
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaEqpto(TList *lisCURVA, int curva_id)
   {
   //variáveis locais
   VTCurva *curva;

   //procura Curva de consumidor típico
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      //verifica se a é Curva inidicada
      if (curva->Id == curva_id) return(curva);
      }
   //não encontrou Curva do
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaEqpto(TList *lisCURVA, AnsiString codigo)
   {
   //variáveis locais
   VTCurva *curva;

   //procura Curva de consumidor típico
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      //verifica se a é Curva inidicada
      if (curva->Codigo.AnsiCompareIC(codigo) == 0) return(curva);
      }
   //não encontrou Curva do
   return(NULL);
   }              

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipica(TList *lisCURVA, TList *lisDUP, int curva_id)
   {
   //variáveis locais
   VTCurva *curva;

   //procura Curva na lista
   for (int n = 0; n < lisCURVA->Count; n++)
      {
	  curva = (VTCurva*)lisCURVA->Items[n];
      if (curva->Id == curva_id) return(curva);
	  }
   //procura Curva na lista de Curvas duplicadas
   for (int n = 0; n < lisDUP->Count; n++)
      {
      curva = (VTCurva*)lisDUP->Items[n];
      if (curva->Id == curva_id)
         {//existe Curva duplicada: determina Curva original pelo Codigo
         if ((curva = ExisteCurvaEqpto(lisCURVA, curva->Codigo)) != NULL) return(curva);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaCapacitor(int curva_id)
   {
   //variáveis locais
   VTCurva *curva = NULL;

   //verifica se não foi definido o Id da Curva
   if (curva_id < 1)
      {//retorna primeira Curva não excluída da lista
      for (int n = 0; n < lisCUR_TIP_CAP->Count; n++)
         {
         curva = (VTCurva*)lisCUR_TIP_CAP->Items[n];
         if (curva->Status[sttEXCLUIDO]) continue;
         return(curva);
         }
      return(NULL);
	  }
   return(ExisteCurvaTipica(lisCUR_TIP_CAP, dup.lisCUR_TIP_CAP, curva_id));
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaCarga(int curva_id)
	{
	return(ExisteCurvaTipica(lisCUR_TIP_CAR, dup.lisCUR_TIP_CAR, curva_id));
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaCarga(AnsiString codigo)
	{
	return(ExisteCurvaEqpto(lisCUR_TIP_CAR, codigo));
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaCarga(VTClasse *classe, double energia_kwmes, int dia_medicao)
   {
   //variáveis locais
   VTCurva *curva;

   //proteção contra dia inválido
   if ((dia_medicao < DOMINGO) || (dia_medicao > SEMANA_COMPLETA)) {dia_medicao = DiaMedicao;}
   if ((dia_medicao < DOMINGO) || (dia_medicao > SEMANA_COMPLETA)) {dia_medicao = SEMANA_COMPLETA;}
   //procura Curva de consumidor típico
   for (int n = 0; n < lisCUR_TIP_CAR->Count; n++)
      {
      curva = (VTCurva*)lisCUR_TIP_CAR->Items[n];
      //verifica se a Curva está excluída
      if (curva->Status[sttEXCLUIDO]) continue;
      //proteção: verifica se a Curva possui uma Classe associada
      if (curva->Classe == NULL) continue;
      //verifica se a Classe atende classe e consumo
      if (curva->Classe !=  classe) continue;
      //verifica dia de medição da curva
      if ((dia_medicao < 0) || (curva->DiaMedicao == dia_medicao))
         {//verifica faixa de consumo
         if ((energia_kwmes >= curva->LimiteInferior)&&
             ((energia_kwmes < curva->LimiteSuperior)||(curva->LimiteSuperior < 0))) return(curva);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaCarga(VTClasse *classe, double lim_inf, double lim_sup, int dia_medicao)
   {
   //variáveis locais
   VTCurva *curva;

   //proteção contra dia inválido
   if ((dia_medicao < DOMINGO) || (dia_medicao > SEMANA_COMPLETA)) {dia_medicao = DiaMedicao;}
   if ((dia_medicao < DOMINGO) || (dia_medicao > SEMANA_COMPLETA)) {dia_medicao = SEMANA_COMPLETA;}
   //procura Curva de consumidor típico
   for (int n = 0; n < lisCUR_TIP_CAR->Count; n++)
      {
      curva = (VTCurva*)lisCUR_TIP_CAR->Items[n];
      //verifica se a Curva está excluída
      if (curva->Status[sttEXCLUIDO]) continue;
      //proteção: verifica se a Curva possui uma Classe associada
      if (curva->Classe == NULL) continue;
      //verifica se a Classe atende classe e consumo
      if (curva->Classe !=  classe) continue;
      //verifica dia de medição da curva
      if (curva->DiaMedicao != dia_medicao) continue;
      //verifica faixa de consumo
      if (! IsDoubleZero(curva->LimiteInferior - lim_inf)) continue;
      if (! IsDoubleZero(curva->LimiteSuperior - lim_sup)) continue;
      //encontrou a Curva
      return(curva);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaCarga(TList *lisCURVA, AnsiString codigo, int dia_medicao)
   { //DVK 2014.02.21
   //variáveis locais
   VTCurva *curva;

   //procura Curva de consumidor típico
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      //verifica se é do mesmo dia da semana
      if(curva->DiaMedicao != dia_medicao) continue;
      //verifica se a é Curva inidicada
      if (curva->Codigo.AnsiCompareIC(codigo) == 0) return(curva);
      }
   //não encontrou Curva do
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaGeradorFotovoltaicoId(int curva_id)
   {
   return(ExisteCurvaTipica(lisCUR_TIP_GFV, dup.lisCUR_TIP_GFV, curva_id));
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaGeradorFotovoltaicoMes(int mes)
   {
   //variáveis locais
   AnsiString   codigo;

   //procura Curva típica de Gerador pelo código
   codigo.sprintf("GerFV_%02d", mes);
   return(ExisteCurvaEqpto(lisCUR_TIP_GFV, codigo));
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaIP(int curva_id)
	{
	//variáveis locais
	VTCurva *curva = NULL;

	//verifica se não foi definido o Id da Curva
	if (curva_id < 1)
		{//retorna primeira Curva não excluída da lista
		for (int n = 0; n < lisCUR_TIP_IP->Count; n++)
			{
			curva = (VTCurva*)lisCUR_TIP_IP->Items[n];
			if (curva->Status[sttEXCLUIDO]) continue;
			return(curva);
			}
		return(NULL);
		}
	return(ExisteCurvaTipica(lisCUR_TIP_IP, dup.lisCUR_TIP_IP, curva_id));
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaIP(AnsiString codigo)
	{
	return(ExisteCurvaEqpto(lisCUR_TIP_IP, codigo));
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurvas::ExisteCurvaTipicaReator(int curva_id)
   {
   //variáveis locais
   VTCurva *curva = NULL;

   //verifica se não foi definido o Id da Curva
   if (curva_id < 1)
      {
      for (int n = 0; n < lisCUR_TIP_REA->Count; n++)
         {
         curva = (VTCurva*)lisCUR_TIP_REA->Items[n];
         if (curva->Status[sttEXCLUIDO]) continue;
         return(curva);
         }
      return(NULL);
      }
   return(ExisteCurvaTipica(lisCUR_TIP_REA, dup.lisCUR_TIP_REA, curva_id));
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvas::InsereCurvaTipicaCapacitor(VTCurva *curva)
   {
   //proteção: verifica se a Curva já existe
   if (lisCUR_TIP_CAP->IndexOf(curva) < 0)
      {//verifica se existe Curva com mesmo código
		if (ExisteCurvaEqpto(lisCUR_TIP_CAP, curva->Codigo))
         {//insere Curva em dup.lisCUR_TIP_CAP
         dup.lisCUR_TIP_CAP->Add(curva);
         }
      else
         {//insere Curva em lisCUR_TIP_CAP
         lisCUR_TIP_CAP->Add(curva);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvas::InsereCurvaTipicaCarga(VTCurva *curva)
   {
   //proteção: verifica se a Curva já existe
   if (lisCUR_TIP_CAR->IndexOf(curva) < 0)
      {//verifica se existe Curva com mesmo código
//      if (ExisteCurvaEqpto(lisCUR_TIP_CAR, curva->Codigo)) //DVK 2014.02.21
	  if (ExisteCurvaTipicaCarga(lisCUR_TIP_CAR, curva->Codigo, curva->DiaMedicao))
         {//insere Curva em dup.lisCUR_TIP_CAR
		 dup.lisCUR_TIP_CAR->Add(curva);
		 }
	  else
		 {//insere Curva em lisCUR_TIP_CAR
         lisCUR_TIP_CAR->Add(curva);
         //verifica se é uma curva definida em pu da demanda máxima
         if (curva->Unidade == unidPU_MAX)
            {//cria curva siamesa, com valores em pu da demanda média
            MontaCurvaSiamesaPUmaxima_PUmedia(curva);
            }
         //verifica se é uma curva definida em MVA
         else if (curva->Unidade == unidMVA)
            {//cria curva siamesa, com valores em pu da demanda média
            MontaCurvaSiamesaMVA_PUmedia(curva);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvas::InsereCurvaTipicaGeradorFotovoltaico(VTCurva *curva)
   {
   //proteção: verifica se a Curva já existe
   if (lisCUR_TIP_GFV->IndexOf(curva) < 0)
      {//verifica se existe Curva com mesmo código
      if (ExisteCurvaEqpto(lisCUR_TIP_GFV, curva->Codigo))
         {//insere Curva em dup.lisCUR_TIP_GFV
         dup.lisCUR_TIP_GFV->Add(curva);
         }
      else
         {//insere Curva em dup.lisCUR_TIP_GFV
         lisCUR_TIP_GFV->Add(curva);
         }
      }
   return(true);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TCurvas::InsereCurvaTipicaIP(VTCurva *curva)
   {
   if (lisCUR_TIP_IP->IndexOf(curva) < 0)
      {//verifica se existe Curva com mesmo código
      if (ExisteCurvaEqpto(lisCUR_TIP_IP, curva->Codigo))
         {//insere Curva em dup.lisCUR_TIP_IP
         dup.lisCUR_TIP_IP->Add(curva);
         }
      else
         {//insere Curva em lisCUR_TIP_IP
         lisCUR_TIP_IP->Add(curva);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvas::InsereCurvaTipicaReator(VTCurva *curva)
   {
   if (lisCUR_TIP_REA->IndexOf(curva) < 0)
      {//verifica se existe Curva com mesmo código
      if (ExisteCurvaEqpto(lisCUR_TIP_REA, curva->Codigo))
         {//insere Curva em dup.lisCUR_TIP_REA
         dup.lisCUR_TIP_REA->Add(curva);
         }
      else
         {//insere Curva em lisCUR_TIP_REA
         lisCUR_TIP_REA->Add(curva);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCurvas::ListaCurvaTipicaCapacitor(void)
   {
   return(lisCUR_TIP_CAP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCurvas::ListaCurvaTipicaCarga(void)
   {
   return(lisCUR_TIP_CAR);
   }

//---------------------------------------------------------------------------
int __fastcall TCurvas::LisCurvaTipicaCarga(VTClasse *classe, int dia_medicao, TList *lisEXT)
   {
   //variáveis locais
   int     count = 0;
   VTCurva *curva;

   //insere em lisEXT c/ as Curvas típicas da classe indicada
   for (int n = 0; n < lisCUR_TIP_CAR->Count; n++)
      {
      curva = (VTCurva*)lisCUR_TIP_CAR->Items[n];
      if ( ((classe == NULL)  || (curva->Classe == classe)) &&
           ((dia_medicao < 0) || (curva->DiaMedicao == dia_medicao)) )
         {//insere Curva em lisEXT
         lisEXT->Add(curva);
         count++;
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCurvas::ListaCurvaTipicaGeradorFotovoltaico(void)
   {
   return(lisCUR_TIP_GFV);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCurvas::ListaCurvaTipicaIP(void)
   {
   return(lisCUR_TIP_IP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCurvas::ListaCurvaTipicaReator(void)
   {
   return(lisCUR_TIP_REA);
   }

//---------------------------------------------------------------------------
void __fastcall TCurvas::MontaCurvaSiamesaMVA_PUmedia(VTCurva *curva)
   {
   //variáveis locais
   strHM    hm_ini, hm_fim;
   double   valor[nvCURVA_PQ_MDP];
   double   p_mw, q_mvar, fatpot, p_medio_mw;
   VTCurva *siamesa;

   //determina valor médio da curva em MVA
   switch(curva->Tipo)
      {
      case curvaPQ:
      case curvaPQ_MDP:
      case curvaPFP:
      case curvaPFP_MDP:
         p_medio_mw = curva->Media(indP);
         break;
      default:
         p_medio_mw = 0.;
      }
   //cria Curva siamesa, como clone da Curva de referência
   siamesa = curva->Clone();
   //redefine unidade e tipo da Curva
   siamesa->Tipo    = curvaPQ_MDP;
   siamesa->Unidade = unidPU_MED;
   //calcula os valores de p e q da curva siamesa
   for (int np = 0; np < curva->NumeroPontos(); np++)
      {
      curva->GetPonto(np, hm_ini, hm_fim, valor, nvCURVA_PQ_MDP);
      //verifica o tipo da Curva
      switch(curva->Tipo)
         {
         case curvaPQ:
         case curvaPQ_MDP:
            p_mw   = valor[0];
            q_mvar = valor[1];
            break;
         case curvaPFP:
         case curvaPFP_MDP:
            p_mw   = valor[0];
            fatpot = valor[1];
            q_mvar = p_mw * Tan(ArcCos(fatpot));
            break;
         default:
            p_mw = q_mvar = 0.;
         }
      //recalcula o valor de p e q, em pu da média
      if (IsDoubleZero(p_medio_mw)) p_medio_mw = 1.0;
      valor[0] = p_mw   / p_medio_mw;
      valor[1] = q_mvar / p_medio_mw;
      valor[2] = 0;
      valor[3] = 0;
      //atualiza ponto da Curva siamesa
      siamesa->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQ_MDP);
      }
   //associa Curva siamesa com sua Curva original
   curva->CurvaSiamesaPUmedia = siamesa;
   }

//---------------------------------------------------------------------------
void __fastcall TCurvas::MontaCurvaSiamesaPUmaxima_PUmedia(VTCurva *curva)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   int       num_ptos;
   double    valor[nvCURVA_PQ_MDP];
   double    p, q, fatpot, p_total, p_medio;
   VTCurva *siamesa;

   //determina número de pontos da Curva
   if ((num_ptos = curva->NumeroPontos()) == 0) return;
   //determina valor médio da curva de demanda em pu da máxima
   p_total = 0;
   //loop p/ todos pontos os pontos da Curva
   for (int np = 0; np < num_ptos; np++)
      {
      curva->GetPonto(np, hm_ini, hm_fim, valor, nvCURVA_PQ_MDP);
      //verifica o tipo da Curva
      switch(curva->Tipo)
         {
         case curvaPQ:
         case curvaPQ_MDP:
            p = valor[0];
            //q = valor[1];
            break;
         case curvaPFP:
         case curvaPFP_MDP:
            p      = valor[0];
            //fatpot = valor[1];
            //q      = p * Tan(ArcCos(fatpot));
            break;
         default:
            p = 0.;
         }
      //totaliza p e q
      p_total += p;
      }
   //calcula potência média
   p_medio = p_total / num_ptos;
   //cria Curva siamesa, como clone da Curva de referência
   siamesa = curva->Clone();
   //redefine unidade e tipo da Curva
   siamesa->Tipo    = curvaPQ_MDP;
   siamesa->Unidade = unidPU_MED;
   //calcula os valores de p e q da curva siamesa
   for (int np = 0; np < curva->NumeroPontos(); np++)
      {
      curva->GetPonto(np, hm_ini, hm_fim, valor, nvCURVA_PQ_MDP);
      //verifica o tipo da Curva
      switch(curva->Tipo)
         {
         case curvaPQ:
         case curvaPQ_MDP:
            p = valor[0];
            q = valor[1];
            break;
         case curvaPFP:
         case curvaPFP_MDP:
            p      = valor[0];
            fatpot = valor[1];
            q      = p * Tan(ArcCos(fatpot));
            break;
         default:
            p = q = 0.;
         }
      //recalcula o valor de p e q, em pu da média
      if (IsDoubleZero(p_medio)) p_medio = 1.0;
      valor[0] = p / p_medio;
      valor[1] = q / p_medio;
      valor[2] = 0;
      valor[3] = 0;
      //atualiza ponto da Curva siamesa
      siamesa->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQ_MDP);
      }
   //associa Curva siamesa com sua Curva original
   curva->CurvaSiamesaPUmedia = siamesa;
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TCurvas::PM_GetDataReferencia(void)
   {
   return(PD.data_referencia);
   }

//---------------------------------------------------------------------------
int __fastcall TCurvas::PM_GetDiaMedicao(void)
   {
   return(PD.dia_medicao);
   }

//---------------------------------------------------------------------------
void __fastcall TCurvas::PM_SetDataReferencia(TDateTime data)
   {
   //variáveis locais

   //verifica se a data de referência é de um mês diferente
   if (MonthOf(data) != MonthOf(DataReferencia))
      {//salva nova data de referência
      PD.data_referencia = data;
      //recalcula fator de correção de energia da curvas
      RecalculaFatorCorrecaoEnergiaDasCurvas(data);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurvas::PM_SetDiaMedicao(int dia_medicao)
   {
   PD.dia_medicao = dia_medicao;
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvas::RecalculaFatorCorrecaoEnergiaDasCurvas(TDateTime data)
   {
   //variáveis locais
   double    energia_media;
   TList     *lisTMP;
   VTCurva   *curva_dut, *curva_sab, *curva_dom, *siamesa;
   VTClasse  *classe;
   VTClasses *classes     = (VTClasses*)apl->GetObject(__classid(VTClasses));
   int       vet_classe[] = {consRESIDENCIAL, consCOMERCIAL, consINDUSTRIAL,
                             consRURAL, consOUTROS, consA4, consA2};

   try{//cria lista temporária
      lisTMP = new TList();
      //determina número de dias úteis, sábados e domingos
      DeterminaDiasMes(data);
      //loop para todas as classes de consumidor
      for (int n = 0; n < sizeof(vet_classe)/sizeof(int); n++)
         {//determina objeto VTClasse
         classe = classes->ExisteClasseTag(vet_classe[n]);
         //inicia lista com Curvas de Carga do dia útil
         lisTMP->Clear();
         LisCurvaTipicaCarga(classe, DIA_UTIL, lisTMP);
         //loop para Curvas de dia útil em lisTMP
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {//curva_dutobtém Curva do dia útil
            curva_dut = (VTCurva*)lisTMP->Items[nc];
            //descarta Curva cuja unidade não seja kVA ou MVA
            if (! ((curva_dut->Unidade == unidKVA)||(curva_dut->Unidade == unidMVA))) continue;
            //determina Curva do sábado e domingo
            curva_sab = ExisteCurvaTipicaCarga(classe, curva_dut->LimiteInferior, curva_dut->LimiteSuperior, SABADO);
            curva_dom = ExisteCurvaTipicaCarga(classe, curva_dut->LimiteInferior, curva_dut->LimiteSuperior, DOMINGO);
            //proteção
            if ((curva_sab == NULL)||(curva_dom == NULL)) continue;
            //calcula energia media diária como média ponderada das energias das 3 curvas
            energia_media = (curva_dut->Integral(indP) * num_dias.dut +
                             curva_sab->Integral(indP) * num_dias.sab +
                             curva_dom->Integral(indP) * num_dias.dom) / num_dias.mes;
            //proteção
            if (IsDoubleZero(energia_media)) continue;
            //calcula fator de correção de energia de cada Curva
            curva_dut->FatorCorrecaoEnergia = curva_dut->Integral(indP) / energia_media;
            curva_sab->FatorCorrecaoEnergia = curva_sab->Integral(indP) / energia_media;
            curva_dom->FatorCorrecaoEnergia = curva_dom->Integral(indP) / energia_media;
            //atualiza fator de correção de energia das Curvas siamesas
            if ((siamesa = curva_dut->CurvaSiamesaPUmedia) != NULL) siamesa->FatorCorrecaoEnergia = curva_dut->FatorCorrecaoEnergia;
            if ((siamesa = curva_sab->CurvaSiamesaPUmedia) != NULL) siamesa->FatorCorrecaoEnergia = curva_sab->FatorCorrecaoEnergia;
            if ((siamesa = curva_dom->CurvaSiamesaPUmedia) != NULL) siamesa->FatorCorrecaoEnergia = curva_dom->FatorCorrecaoEnergia;
            }
         }
      //destrói lista sem destruir seus objetos
      delete lisTMP;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof



