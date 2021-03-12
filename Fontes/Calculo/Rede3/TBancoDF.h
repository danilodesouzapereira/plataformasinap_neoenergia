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
   double          perda_energia_diaria_kwh; // Perda diária em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda diária em energia (parcela de perdas no ferro) (kWh)
   complex<double> vneutro_pu;               // Tensão no neutro do primário (nó eliminado) (pu)
   complex<double> *vet_corr_a;              // Corrente nas pernas do trafo (A)
   complex<double> *vet_s_kva;               // Potência nas pernas do trafo (kVA)
   complex<double> *vet_perda_kva;           // Perda complexa por fase no trafo (kVA)
   TList           *lisTRAFO;                // Lista de trafos (3 trafos TTrafo12, sempre)
   TBarra          *bar_AT;                  // Barra do lado de Alta Tensão (Estrela)
   TBarra          *bar_BT;                  // Barra do lado de Baixa Tensão (Delta)
   TFase           *fases_AT;                // Fases do lado de Alta Tensão
   TFase           *fases_BT;                // Fases do lado de Baixa Tensão
   VTSisLin        *Ynodal_pu;               // Matriz Ynodal local do banco DF (pu)

public:  // Métodos
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
   int num_tot_fases;  // Número total de fases (promário + secundário)
};

#endif
//---------------------------------------------------------------------------

