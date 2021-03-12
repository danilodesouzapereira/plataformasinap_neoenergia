//---------------------------------------------------------------------------
#ifndef TCalculo3H
#define TCalculo3H

//---------------------------------------------------------------------------
#include <complex>
#include ".\VTCalculo3.h"
using namespace std;

class TBarra;
class TCurtoAux3;

//---------------------------------------------------------------------------
class TCalculo3 : public VTCalculo3
{
public:  // Métodos
                   __fastcall  TCalculo3(VTRede3    *rede3,
                                         VTMonta3   *monta3,
                                         VTGeralC   *geralC,
                                         TCurtoAux3 *curto_aux3=NULL);
                   __fastcall ~TCalculo3(void);
   bool            __fastcall  AtualizaTensoes(complex<double> *vet_tensoes);
   bool            __fastcall  CalculaCorrentesBarra(TBarra          *bar1,
                                                     complex<double> *vet_tensoes,
                                                     complex<double> *vet_corr_pu);
   bool            __fastcall  CalculaCorrentesPerdas(int             np,
                                                      complex<double> *vet_tensoes,
                                                      VTSisLin        *Ynod);
   bool            __fastcall  CalculaLigacoes(int np);
   // void            __fastcall  ConverteTensoes(int             sentido,
   //                                             complex<double> *vet_origem,
   //                                             complex<double> *vet_destino);
   bool            __fastcall  CopiaTensao_PQV(complex<double> *vet_tensoes);
   bool            __fastcall  CopiaTensao_Sup(complex<double> *vet_tensoes);
   bool            __fastcall  InicializaTensao_PQ_EXT(complex<double> *vet_tensoes);
   bool            __fastcall  InicializaTensao_PQ_EXT_PQV(complex<double> *vet_tensoes);
   void            __fastcall  ZeraPerdaEnergia(void);

private:  // Objetos externos
   VTMonta3   *monta3;
   VTRede3    *rede3;
   VTGeralC   *geralC;
   TCurtoAux3 *curto_aux3;

private:  // Dados locais
   double          grau_rad, raiz3, raiz3_inverso;
   complex<double> cum, czero;

private:  // Métodos
   bool            __fastcall CalculaCorrenteAterramentos(void);
   complex<double> __fastcall CalculaCorrenteNo(TBarra *bar1,
                                                int    fase_int);
   bool            __fastcall CalculaCorrentesBarra_ESTIM3(TBarra          *bar1,
                                                           complex<double> *vet_tensoes,
                                                           complex<double> *vet_corr_pu);
   void            __fastcall CalculaPerdaAdmitReduc(void);
   void            __fastcall DefineTensoes(TBarra          *bar1,
                                            double          mod_vsup_pu,
                                            complex<double> *vet_tensoes);
   void            __fastcall DefineTensoesPV(TBarra          *bar1,
                                              double          mod_vsup_pu,
                                              complex<double> *vet_tensoes);
   double          __fastcall TensaoPrimeiraBarraSwing(void);
};

#endif
//---------------------------------------------------------------------------

