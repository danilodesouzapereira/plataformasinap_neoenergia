//---------------------------------------------------------------------------
#ifndef TNR1H
#define TNR1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include ".\VTNR1.h"

class  TImpressao1;
class  VTAtualiza1;
class  VTCalculo1;
class  VTGeralC;
class  VTMonta1;
class  VTRede1;
class  VTResultados1;
class  TBarra;
class  TGrupoLTC;
class  TTrafo2;
class  TTrafo3;
class  TTrafoEquiv;
struct Est1_G1_NR1;

//---------------------------------------------------------------------------
class TNR1 : public VTNR1
{
private:  // Definição de dados

public:  // Métodos
        __fastcall  TNR1(VTApl *apl);
        __fastcall ~TNR1(void);
   bool __fastcall  CalculaRede_EST1_PM1(int   np,
                                         bool  flag_inicializa_Jacob,
                                         TList *lisBAR_GER=NULL);
   void __fastcall  CfgAvisoErro(bool enabled);
   bool __fastcall  Executa(VTData *data);
   bool __fastcall  InicializaNR1_EST1_PM1(Est1_G1_NR1 *str);
   int  __fastcall  NumPatamaresOK(void);

private:  // Objetos externos
   TImpressao1   *impr1;
   VTApl         *apl;
   VTData        *data;
   VTAtualiza1   *atual1;
   VTCalculo1    *calculo1;
   VTGeralC      *geralC;
   VTMonta1      *monta1;
   VTRede1       *rede1;
   VTResultados1 *result1;

private:  // Dados locais
   bool            flag_alocou_externos, flag_existe_regul_Jacob,
                   flag_montar_Y, flag_ordenar_barras;
   int             num_ext, num_fatora_J, num_max_monta_J, num_monta_J,
                   num_pqv, num_pv, tipo_monta_J;
   double          grau_rad, raiz3;
   double          *vet_p3f_calc, *vet_p3f_esp, *vet_TC;
   complex<double> cum, czero;
   complex<double> *vet_corr_calc, *vet_corr_esp, *vet_scalc, *vet_tensoes;
   AnsiString      nome_caso;
   TList           *lisBAR_J;   // Lista de barras com seus objetos TYnodalJ (classe TBarraJ)
   TList           *lisYJ;      // Lista de elementos de Ynodal com seus endereços no Jacobiano (classe TYnodalJ)
   TList           *lisPV_FORA; // Lista de barras PV com limites de Q excedidos (classe TPV_Fora)
   VTSisLin        *Jacob;
   clock_t         fim, fim_main, inicio, inicio_main;

private:  // Métodos
   bool            __fastcall AjustaTapReguladoresNAO_PADRAO(int iterCONT);
   bool            __fastcall AlocaMemoria1(void);
   bool            __fastcall AlocaMemoria2(void);
   bool            __fastcall AtualizaEstado(void);
   bool            __fastcall AtualizaEstadoReguladores(TGrupoLTC *grupoLTC);
   bool            __fastcall AtualizaTensaoSuprimentos(void);
   bool            __fastcall AtualizaYnodalReguladores(int opcao);
   bool            __fastcall CalculaCorrentesPotencias(void);
   bool            __fastcall CalculaRede(int np);
   bool            __fastcall CalculaTC(void);
   int             __fastcall ColunaDoTap(TBarra *bar1);
   void            __fastcall DefineMontagemJ(void);
   // bool            __fastcall EP_PEA3301_2018(void);
   bool            __fastcall ExecutaNR1(void);
   bool            __fastcall ExisteBarraPV(void);
   bool            __fastcall Finaliza(void);
   bool            __fastcall FlagQmaxPV(void);
   // bool            __fastcall GravaSistemaEquacoes(void);
   // bool            __fastcall GravaSistemaEquacoes_FluxoCC(void);
   bool            __fastcall InicializaJacob(void);
	bool            __fastcall InsereContribuicaoAUTODF(TGrupoLTC *grupoLTC);
   bool            __fastcall InsereContribuicaoBarrasPQ_EXT_PQV(void);
   bool            __fastcall InsereContribuicaoBarrasPQV_Taps(void);
   bool            __fastcall InsereContribuicaoBarrasPV_P3f(void);
   bool            __fastcall InsereContribuicaoCargas(void);
	bool            __fastcall InsereContribuicaoDemaisTiposT2(TGrupoLTC *grupoLTC);
   bool            __fastcall InsereDerivadasCarga_PQ_EXT(TBarra          *bar1,
                                                          int             modelo,
                                                          complex<double> s_pu);
   bool            __fastcall InsereDerivadasCarga_PQV(TBarra          *bar1,
                                                       int             modelo,
                                                       complex<double> s_pu);
   void            __fastcall LiberaTaps(void);
   void            __fastcall LinhasEmJ(TBarra *bar1,
                                        int    *linha1,
                                        int    *linha2);
   bool            __fastcall MontaJacob(bool flag_modif_diag=false);
   bool            __fastcall RemontaRede(void);
   bool            __fastcall RetornaBarrasPV(void);
   bool            __fastcall VerificaBarrasPV(int iterPV);
};

#endif
//---------------------------------------------------------------------------

