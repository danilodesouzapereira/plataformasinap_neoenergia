//---------------------------------------------------------------------------
#ifndef TAtualiza1H
#define TAtualiza1H

//---------------------------------------------------------------------------
#include <complex>
#include ".\VTAtualiza1.h"
using namespace std;

class TBarra;
class VTData;
class VTGeralC;

//---------------------------------------------------------------------------
class TAtualiza1 : public VTAtualiza1
   {
   public:  // Métodos
           __fastcall  TAtualiza1(VTRede1  *rede1,
                                  VTGeralC *geralC);
           __fastcall ~TAtualiza1(void);
      bool __fastcall  AtualizaBateria(int np);
      bool __fastcall  AtualizaCapacitorReator(int seq,
                                               int np);
      bool __fastcall  AtualizaCarga(int np);
      bool __fastcall  AtualizaCurto(int np);
      bool __fastcall  AtualizaEstimador1(int np);
      bool __fastcall  AtualizaFluxoRadial1(int np);
      bool __fastcall  AtualizaGauss1(int np);
      bool __fastcall  AtualizaEstabilidade(int np);
		bool __fastcall  AtualizaNR1(int np);
      bool __fastcall  AtualizaPartidaMotor(int np);
      bool __fastcall  AtualizaSuprimentos(int np);

   private:  // Objetos externos
      VTData   *data;
      VTGeralC *geralC;
      VTRede1  *rede1;

   private:  // Dados
      double          grau_rad, raiz3, raiz3_inverso;
      complex<double> cum, czero;

   private:  // Métodos
      bool __fastcall AtualizaCNLsFundamental(void);
      bool __fastcall AtualizaMedidoresReais(int np);
      // bool __fastcall AtualizaMedidoresPotencia(int  np,
      //                                           bool *flag_med_zero);
      // bool __fastcall AtualizaMedidoresTensaoCorrente(int  np,
		// 																bool *flag_med_zero);
   };

#endif
//---------------------------------------------------------------------------

