//---------------------------------------------------------------------------
#ifndef TCalculo1H
#define TCalculo1H

//---------------------------------------------------------------------------
#include <complex>
#include ".\VTCalculo1.h"
using namespace std;

class TBarra;
class VTGeralC;
class VTMonta1;
class VTRede1;

//---------------------------------------------------------------------------
class TCalculo1 : public VTCalculo1
{
private:  // Definição de dados

public:  // Métodos
                   __fastcall  TCalculo1(VTRede1  *rede1,
                                         VTMonta1 *monta1,
                                         VTGeralC *geralC);
                   __fastcall ~TCalculo1(void);
   bool            __fastcall  AtualizaTensoes(complex<double> *vet_tensoes);
   complex<double> __fastcall  CalculaCorrenteBarra(TBarra          *bar1,
                                                    complex<double> v1);
   complex<double> __fastcall  CalculaCorrenteBarra_Icte(TBarra          *bar1,
                                                         complex<double> v1);
   bool            __fastcall  CalculaCorrentesPerdas(int             np,
                                                      complex<double> *vet_tensoes,
                                                      VTSisLin        *Ynod);
   bool            __fastcall  CalculaGeradores(TList           *lisBAR_GER,
                                                complex<double> *vet_tensoes);
   bool            __fastcall  CopiaTensao_PQV(complex<double> *vet_tensoes);
   bool            __fastcall  CopiaTensao_Sup(complex<double> *vet_tensoes);
   bool            __fastcall  InicializaTensao_PQ_EXT(complex<double> *vet_tensoes);
   bool            __fastcall  InicializaTensao_PQ_EXT_PQV(complex<double> *vet_tensoes);
   void            __fastcall  ZeraPerdaEnergia(void);

private:  // Objetos externos
   VTGeralC *geralC;
   VTRede1  *rede1;
   VTMonta1 *monta1;

private:  // Dados locais
   double          grau_rad, raiz3, raiz3_inverso;
   complex<double> cum, czero;

private:  // Métodos
   complex<double> __fastcall CalculaCorrenteBarra_Radial(TBarra          *bar1,
                                                          complex<double> *vet_tensoes);
   void            __fastcall CalculaPerdaAdmitReduc(complex<double> *vet_tensoes);
   void            __fastcall CopiaCarga(complex<double> *vet_origem,
                                         complex<double> *vet_destino);
   void            __fastcall DivideCarga(complex<double> *vet_carga,
                                          double          fator);
};

#endif
//---------------------------------------------------------------------------

