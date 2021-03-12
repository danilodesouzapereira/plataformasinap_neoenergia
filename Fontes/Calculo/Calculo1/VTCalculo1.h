//---------------------------------------------------------------------------
#ifndef VTCalculo1H
#define VTCalculo1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class TTrafo2;
class TTrafo3;
class TTrafoEquiv;
class VTGeralC;
class VTMonta1;
class VTRede1;
class VTSisLin;

//---------------------------------------------------------------------------
class VTCalculo1 : public TObject
   {
   public:  // Métodos
                              __fastcall  VTCalculo1() {};
      virtual                 __fastcall ~VTCalculo1(void) {};
      virtual bool            __fastcall  AtualizaTensoes(complex<double> *vet_tensoes) = 0;
      virtual complex<double> __fastcall  CalculaCorrenteBarra(TBarra          *bar1,
                                                               complex<double> v1) = 0;
      virtual complex<double> __fastcall  CalculaCorrenteBarra_Icte(TBarra          *bar1,
                                                                    complex<double> v1) = 0;
      virtual bool            __fastcall  CalculaCorrentesPerdas(int             np,
                                                                 complex<double> *vet_tensoes,
                                                                 VTSisLin        *Ynod=NULL) = 0;
      virtual bool            __fastcall  CalculaGeradores(TList           *lisBAR_GER,
                                                           complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  CopiaTensao_PQV(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  CopiaTensao_Sup(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  InicializaTensao_PQ_EXT(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  InicializaTensao_PQ_EXT_PQV(complex<double> *vet_tensoes) = 0;
      virtual void            __fastcall  ZeraPerdaEnergia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCalculo1
//---------------------------------------------------------------------------
VTCalculo1 * __fastcall NewObjCalculo1(VTRede1  *rede1,
                                       VTMonta1 *monta1,
                                       VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

