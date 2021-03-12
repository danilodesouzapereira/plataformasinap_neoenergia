//---------------------------------------------------------------------------
#ifndef TNR3H
#define TNR3H

//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <Classes.hpp>
#include <complex>
#include ".\VTNR3.h"
#include "..\SisLin\VTSisLin.h"
using namespace std;

class TBarra;
class TBarraJ;
class TGrupoLTC12;
class TGrupoLTC32;
class TImpressao3;
class TReg12;
class TReg32;
class TTrafo12;
class TTrafo32;
class TTrafo33;
class TTrafoEquiv;
class TTrafoZZ;
class TYnodalJ;
class VTAtualiza3;
class VTCalculo3;
class VTGeralC;
class VTMatriz;
class VTMonta3;
class VTRede3;
class VTResultados3;

//---------------------------------------------------------------------------
class TNR3 : public VTNR3
{
private:  // Definição de dados

public:  // Métodos
        __fastcall  TNR3(VTApl *apl);
        __fastcall ~TNR3(void);
   bool __fastcall  Executa(VTData *data);
   int  __fastcall  NumPatamaresOK(void);

private:  // Objetos externos
   TImpressao3   *impr3;
   VTApl         *apl;
   VTData        *data;
   VTAtualiza3   *atual3;
   VTCalculo3    *calculo3;
   VTGeralC      *geralC;
   VTMonta3      *monta3;
   VTRede3       *rede3;
   VTResultados3 *result3;

private:  // Dados locais
   bool            flag_existe_regul_Jacob, flag_montar_Y, flag_ordenar_barras,
                   flag_verifica_NAN;
   int             num_ext, num_fatora_J, num_max_monta_J, num_monta_J,
                   num_pqv, num_pv, tipo_monta_J;
   int             int_retorno; // Para interceptar return(false) - rede do Acre com bug não sistemático
   double          raiz3, raiz3_inv;
   double          *vet_p3f_calc, *vet_p3f_esp, *vet_TC;
   complex<double> cum, czero;
   complex<double> *vet_corr_calc, *vet_corr_esp, *vet_scalc, *vet_tensoes;
   AnsiString      nome_caso;
   TList           *lisBAR_J;    // Lista de barras com seus objetos TYnodalJ (classe TBarraJ)
   TList           *lisYJ;       // Lista de elementos de Ynodal com seus endereços no Jacobiano (classe TYnodalJ)
   TList           *lisPV_FORA;  // Lista de barras PV com limites de Q excedidos (classe TPV_Fora)
   VTSisLin        *Jacob;
   clock_t         fim, fim_main, inicio, inicio_main;

private:  // Métodos
   bool                __fastcall AjustaTapReguladoresNAO_PADRAO(int iterCONT);
   bool                __fastcall AlocaMemoria(void);
   bool                __fastcall AtualizaEstado(void);
	bool                __fastcall AtualizaEstadoTReg12(TGrupoLTC12 *grupoLTC12);
	bool                __fastcall AtualizaEstadoTReg32(TGrupoLTC32 *grupoLTC32);
	bool                __fastcall AtualizaTensaoSuprimentos(void);
   bool                __fastcall CalculaCorrentesPotencias(void);
   bool                __fastcall CalculaRede(int np);
   bool                __fastcall CalculaTC(void);
   bool                __fastcall Carga_THETA_TAP_THETA_TAP(int             linY1,
                                                            int             linY2,
                                                            int             nmo,
                                                            complex<double> sfase_pu);
   bool                __fastcall Carga_VR_VM_THETA_TAP(int             linY1,
                                                        int             linY2,
                                                        int             nmo,
                                                        complex<double> sfase_pu);
   bool                __fastcall Carga_VR_VM_VR_VM(int             linY1,
                                                    int             linY2,
                                                    int             nmo,
                                                    complex<double> sfase_pu);
   void                __fastcall CfgAvisoErro(bool enabled);
   int                 __fastcall ColunaDoTap(TBarra *bar1,
                                              int    fase_int);
   void                __fastcall DefineMontagemJ(void);
   bool                __fastcall ExecutaNR3(void);
   bool                __fastcall ExisteBarraPV(void);
   bool                __fastcall ExisteNAN(void);
   bool                __fastcall Finaliza(void);
   bool                __fastcall FlagQmaxPV(void);
   void                __fastcall GravaArquivoSaida(bool sucesso);
   // bool                __fastcall GravaSistemaEquacoes(void);
   // bool                __fastcall GravaSistemaEquacoes_FluxoCC(void);
   // void                __fastcall ImprimeTaps(int np,
   //                                            int iterCONT,
   //                                            int iterNR_acm);
   bool                __fastcall InicializaJacob(void);
	bool                __fastcall InsereContribuicaoAUTO4_AUTO61T(TGrupoLTC12 *grupoLTC12);
	bool                __fastcall InsereContribuicaoAUTO62T(TGrupoLTC12 *grupoLTC12);
	bool                __fastcall InsereContribuicaoAUTODF(TGrupoLTC32 *grupoLTC32);
	bool                __fastcall InsereContribuicaoAUTOY(TGrupoLTC32 *grupoLTC32);
   bool                __fastcall InsereContribuicaoBarrasPQ_EXT_PQV(void);
   bool                __fastcall InsereContribuicaoBarrasPQV_Taps(void);
   bool                __fastcall InsereContribuicaoBarrasPV_P3f(void);
   bool                __fastcall InsereContribuicaoBarrasPV_SW_INT_NosCarga(void);
   bool                __fastcall InsereContribuicaoCargas(void);
	bool                __fastcall InsereContribuicaoDD(TGrupoLTC32 *grupoLTC32);
	bool                __fastcall InsereContribuicaoDY1(TGrupoLTC32 *grupoLTC32);
	bool                __fastcall InsereContribuicaoDY11(TGrupoLTC32 *grupoLTC32);
   void                __fastcall InsereContribuicaoNoDeCargaOuGeracao(TYnodalJ        *pYJ1,
                                                                       TYnodalJ        *pYJ2,
                                                                       complex<double> Y1);
   bool                __fastcall InsereContribuicaoUmaFaseRegulador(int             colJ_alfa,
                                                                     int             linY_j,
                                                                     int             linY_k,
                                                                     int             linY_m,
                                                                     double          alfa_tap,
                                                                     complex<double> ytrafo_pu);
	bool                __fastcall InsereContribuicaoYD1(TGrupoLTC32 *grupoLTC32);
	bool                __fastcall InsereContribuicaoYD11(TGrupoLTC32 *grupoLTC32);
	bool                __fastcall InsereContribuicaoYY(TGrupoLTC32 *grupoLTC32);
   void                __fastcall InsereDD_Primario(int             linJ_r,
                                                    int             linJ_m,
                                                    int             colJ,
                                                    double          alfa_pu,
                                                    complex<double> ytrafo_pu,
                                                    complex<double> dv1,
                                                    complex<double> dv2);
   void                __fastcall InsereDD_Secundario(int             linJ_r,
                                                      int             linJ_m,
                                                      int             colJ,
                                                      complex<double> ytrafo_pu,
                                                      complex<double> dv1);
   bool                __fastcall InsereDerivadasUmaFaseCarga(int             linY1,
                                                              int             linY2,
                                                              int             nmo,
                                                              complex<double> sfase_pu);
   void                __fastcall InsereDY_Primario(int             linJ_r,
                                                    int             linJ_m,
                                                    int             colJ,
                                                    double          alfa_pu,
                                                    complex<double> ytrafo_pu,
                                                    complex<double> dv1,
                                                    complex<double> dv2);
   void                __fastcall InsereDY_Secundario(int             linJ1_r,
                                                      int             linJ1_m,
                                                      int             linJ2_r,
                                                      int             linJ2_m,
                                                      int             colJ,
                                                      complex<double> ytrafo_pu,
                                                      complex<double> dv1);
   void                __fastcall InsereYD_Primario(int             linJ1_r,
                                                    int             linJ1_m,
                                                    int             linJ2_r,
                                                    int             linJ2_m,
                                                    int             colJ,
                                                    double          alfa_pu,
                                                    complex<double> ytrafo_pu,
                                                    complex<double> dv1,
                                                    complex<double> dv2);
   void               __fastcall InsereYD_Secundario(int             linJ_r,
                                                     int             linJ_m,
                                                     int             colJ1,
                                                     int             colJ2,
                                                     complex<double> ytrafo_pu,
                                                     complex<double> dv1,
                                                     complex<double> dv2);
   void                __fastcall InsereYY_Primario(int             linJ1_r,
                                                    int             linJ1_m,
                                                    int             linJ2_r,
                                                    int             linJ2_m,
                                                    int             colJ,
                                                    double          alfa_pu,
                                                    complex<double> ytrafo_pu,
                                                    complex<double> dv1,
                                                    complex<double> dv2);
   void               __fastcall InsereYY_Secundario(int             linJ1_r,
                                                     int             linJ1_m,
                                                     int             linJ2_r,
                                                     int             linJ2_m,
                                                     int             colJ,
                                                     complex<double> ytrafo_pu,
                                                     complex<double> dv1);
   void                __fastcall LiberaTaps(void);
   void                __fastcall LinhasEmJ(TBarra *bar1,
                                            int    fase_int,
                                            int    *linha1,
                                            int    *linha2);
   bool                __fastcall MontaJacob(bool flag_modif_diag=false);
   bool                __fastcall RemontaRede(void);
   bool                __fastcall RetornaBarrasPV(void);
   bool                __fastcall VerificaBarrasPV(int iterPV);
};

#endif
//---------------------------------------------------------------------------

