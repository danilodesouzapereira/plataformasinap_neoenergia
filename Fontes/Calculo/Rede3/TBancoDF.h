//---------------------------------------------------------------------------
#ifndef TBancoDFH
#define TBancoDFH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class TFase;
class TTrafo12;
class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TBancoDF : public TObject
{
public:  // Dados locais
   double          perda_energia_diaria_kwh; // Perda di�ria em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda di�ria em energia (parcela de perdas no ferro) (kWh)
   complex<double> vneutro_pu;               // Tens�o no neutro do prim�rio (n� eliminado) (pu)
   complex<double> *vet_corr_a;              // Corrente nas pernas do trafo (A)
   complex<double> *vet_s_kva;               // Pot�ncia nas pernas do trafo (kVA)
   complex<double> *vet_perda_kva;           // Perda complexa por fase no trafo (kVA)
   TList           *lisTRAFO;                // Lista de trafos (3 trafos TTrafo12, sempre)
   TBarra          *bar_AT;                  // Barra do lado de Alta Tens�o (Estrela)
   TBarra          *bar_BT;                  // Barra do lado de Baixa Tens�o (Delta)
   TFase           *fases_AT;                // Fases do lado de Alta Tens�o
   TFase           *fases_BT;                // Fases do lado de Baixa Tens�o
   VTSisLin        *Ynodal_pu;               // Matriz Ynodal local do banco DF (pu)

public:  // M�todos
			    __fastcall  TBancoDF(TTrafo12 *trafo12);
			    __fastcall ~TBancoDF(void);
        bool __fastcall  AddTrafo(TTrafo12 *trafo12);
        void __fastcall  Calcula(int      np,
                                 VTGeralC *geralC);
        bool __fastcall  InsereYnodal(int      ordemh,
                                      VTGeralC *geralC,
                                      VTSisLin *Ynod,
                                      bool      flag_zpequena=false);
        void __fastcall  MontaVetorLocal(TTrafo12 *trafo12,
                                         int      *num_linhas_trafo,
                                         int      *vet_LC_local);
        bool __fastcall  OK(void);
        void __fastcall  ZeraPerdaEnergia(void);

private:  // Dados
   int num_tot_fases;  // N�mero total de fases (prom�rio + secund�rio)
};

#endif
//---------------------------------------------------------------------------

