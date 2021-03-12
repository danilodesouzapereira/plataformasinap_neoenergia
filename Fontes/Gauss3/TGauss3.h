//---------------------------------------------------------------------------
#ifndef TGauss3H
#define TGauss3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include ".\VTGauss3.h"

class  TBarra;
class  TCurtoAux3;
class  TImpressao3;
class  VTAtualiza3;
class  VTCalculo3;
class  VTGeralC;
class  VTMonta3;
class  VTNet;
class  VTRede3;
class  VTRedeCC;
class  VTResultados3;
class  VTSisLin;
struct Curto;
struct FluxoH3_G3;

//---------------------------------------------------------------------------
class TGauss3 : public VTGauss3
{
private:  // Definição de dados

public:  // Métodos
        __fastcall  TGauss3(VTApl *apl);
        __fastcall ~TGauss3(void);
   bool __fastcall  CalculaRedeFluxoH3(int np);
   bool __fastcall  ExecutaCurtoMA(int      ind_pat,
                                   VTData   *data,
                                   VTBarra  *barra_sinap,
                                   VTNet    *net,
                                   VTRedeCC *redeCC,
                                   TList    *lisBAR_DEF_SINAP);
   bool __fastcall  ExecutaFluxo(VTData *data);
   bool __fastcall  InicializaGauss3_FluxoH3(FluxoH3_G3 *str);
   int  __fastcall  NumPatamaresOK(void);

private:  // Objetos externos
   TImpressao3   *impr3;
   TList         *lisBAR_DEF; // Barras para defeito (objetos strSinapCurtoMA)
   VTApl         *apl;
   VTData        *data;
   VTAtualiza3   *atual3;
   VTCalculo3    *calculo3;
   VTGeralC      *geralC;
   VTMonta3      *monta3;
   VTNet         *net;
   VTRede3       *rede3;
   VTRedeCC      *redeCC;
   VTResultados3 *result3;

private:  // Dados locais
   bool            flag_alocou_externos, flag_ordenar_barras, flag_verifica_NAN;
   // bool            flag_IEEE13;  // Rede IEEE 13 barras (Distribuição)
   double          grau_rad, raiz3, raiz3_inverso;
   complex<double> cum, czero, zerov;
   complex<double> *vet_corr_ger, *vet_tensoes, *vet_TC;
   clock_t         fim, fim_main, inicio, inicio_main;
   TCurtoAux3      *curto_aux3;
   TList           *lisBAR_SEM_CONV;
   VTSisLin        *Ynod2;

private:  // Métodos
   // bool            __fastcall AcertaImpedanciasAdmitancias_IEEE13(void);
   // void            __fastcall AdicionaBarraSemConvergencia(TBarra *bar1);
   void            __fastcall AjustaTapReguladores1(void);
   void            __fastcall AjustaTapReguladores3(void);
   bool            __fastcall AlocaMemoria(void);
   // void            __fastcall AnalisaCurtoEPUSP(void);
   void            __fastcall AtualizaTensaoSupPQ(void);
   void            __fastcall CalculaCorrentesAssimetricas(void);
   bool            __fastcall CalculaCorrenteGeradores(void);
   bool            __fastcall CalculaRedeCurtoMA(int np);
   bool            __fastcall CalculaRedeFluxo(int  np,
                                               bool flag_imprime);
   bool            __fastcall CalculaTC(void);
   bool            __fastcall DefineCurto(int tipo);
   bool            __fastcall DeterminaMontagemYnodal(bool flag_novo_patamar,
                                                      bool flag_tap_alterado);
   bool            __fastcall ExecutaCurtoMA_Gauss3(int ind_pat);
   bool            __fastcall ExecutaCurtoMA_Gauss3_Simultaneo(int ind_pat);
   bool            __fastcall ExecutaFluxo_Gauss3(void);
   bool            __fastcall ExisteNAN(void);
   bool            __fastcall Finaliza(void);
   // bool            __fastcall GravaSistemaEquacoes(void);
   bool            __fastcall IsNoGeracao(int linha);
   void            __fastcall LiberaTaps(void);
   bool            __fastcall VerificaConvergencia(int  iter_Gauss,
                                                   bool *flag_tap_alterado);

   // CurtoSimultaneo
   // TBarra          *bar_def2;
   // bool            __fastcall ExecutaCurtoMA_Gauss3_CurtoSimultaneo(int ind_pat);
   bool            __fastcall InsereZdef_CurtoSimultaneo(void);
};

#endif
//---------------------------------------------------------------------------

