//---------------------------------------------------------------------------
#ifndef TGauss1H
#define TGauss1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include ".\VTGauss1.h"

class  TBarra;
class  TImpressao1;
class  TTrafo2;
class  TTrafo3;
class  TTrafoEquiv;
class  VTAtualiza1;
class  VTCalculo1;
class  VTGeralC;
class  VTMonta1;
class  VTRede1;
class  VTResultados1;
class  VTSisLin;
struct Est1_G1_NR1;

//---------------------------------------------------------------------------
class TGauss1 : public VTGauss1
{
public:  // Métodos
        __fastcall  TGauss1(VTApl *apl);
        __fastcall ~TGauss1(void);
   bool __fastcall  CalculaRede_EST1_PM1(int      np,
                                         VTSisLin **Ynod2,
                                         TList    *lisBAR_GER=NULL);
   bool __fastcall  Executa(VTData *data);
   bool __fastcall  InicializaGauss1_EST1_PM1(Est1_G1_NR1 *str);
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
   bool            flag_alocou_externos, flag_ordenar_barras;
   // bool            flag_gera_arq_rede;
   double          grau_rad, raiz3, raiz3_inverso;
   complex<double> cum, czero, zerov;
   complex<double> *vet_corr_ger, *vet_tensoes, *vet_TC;
   VTSisLin        *Ynod2;

private:  // Métodos
   void            __fastcall AjustaTapReguladores(void);
   bool            __fastcall AlocaMemoria2(void);
   bool            __fastcall AlocaMemoria3(void);
   bool            __fastcall AnalisaSuprimentosPatamar(int np);
   void            __fastcall AtualizaTensaoSupPQ(void);
   bool            __fastcall CalculaCorrenteGeradores(void);
   bool            __fastcall CalculaRede(int np);
   bool            __fastcall CalculaTC(void);
   bool            __fastcall DeterminaMontagemYnodal(bool flag_novo_patamar,
                                                      bool flag_tap_alterado);
   bool            __fastcall ExecutaGauss1(void);
   bool            __fastcall Finaliza(void);
   // bool            __fastcall GeraArquivoRede(void);
   // bool            __fastcall GravaSistemaEquacoes(void);
   void            __fastcall LiberaTaps(void);
   bool            __fastcall MontaFatoraYnod2(VTSisLin **Ynod2);
   bool            __fastcall VerificaConvergencia(int  iter_Gauss,
                                                   bool *flag_tap_alterado);
};

#endif
//---------------------------------------------------------------------------

