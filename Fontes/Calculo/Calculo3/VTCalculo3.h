//---------------------------------------------------------------------------
#ifndef VTCalculo3H
#define VTCalculo3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class  TBarra;
class  TCurtoAux3;
class  VTGeralC;
class  VTMonta3;
class  VTRede3;
class  VTSisLin;

//---------------------------------------------------------------------------
class VTCalculo3 : public TObject
   {
   public:  // Métodos
                              __fastcall  VTCalculo3() {};
      virtual                 __fastcall ~VTCalculo3(void) {};
      virtual bool            __fastcall  AtualizaTensoes(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  CalculaCorrentesBarra(TBarra          *bar1,
                                                                complex<double> *vet_tensoes,
                                                                complex<double> *vet_corr_carga_pu) = 0;
      virtual bool            __fastcall  CalculaCorrentesPerdas(int             np,
                                                                 complex<double> *vet_tensoes,
                                                                 VTSisLin        *Ynod=NULL) = 0;
      virtual bool            __fastcall  CalculaLigacoes(int np) = 0;
      // virtual void            __fastcall  ConverteTensoes(int             sentido,
      //                                                     complex<double> *vet_origem,
      //                                                     complex<double> *vet_destino) = 0;
      virtual bool            __fastcall  CopiaTensao_PQV(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  CopiaTensao_Sup(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  InicializaTensao_PQ_EXT(complex<double> *vet_tensoes) = 0;
      virtual bool            __fastcall  InicializaTensao_PQ_EXT_PQV(complex<double> *vet_tensoes) = 0;
      virtual void            __fastcall  ZeraPerdaEnergia(void) = 0;
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTCalculo3
//---------------------------------------------------------------------------
VTCalculo3 * __fastcall NewObjCalculo3(VTRede3    *rede3,
                                       VTMonta3   *monta3,
                                       VTGeralC   *geralC,
                                       TCurtoAux3 *curto_aux3=NULL);

#endif
//---------------------------------------------------------------------------

