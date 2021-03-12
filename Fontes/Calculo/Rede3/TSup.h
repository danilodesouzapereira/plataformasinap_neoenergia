//---------------------------------------------------------------------------
#ifndef TSupH
#define TSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class TFase;
class VTSisLin;

//---------------------------------------------------------------------------
class TSup : public TObject
{
public:  // Dados locais
   int             id;
   AnsiString      codigo;             // Código do suprimento
   TBarra          *bar_ext;           // Ponteiro para a barra externa do gerador
   TBarra          *bar_int;           // Ponteiro para a barra interna do gerador
   TFase           *fases;             // Fases do suprimento
   int             num_fases_sem_NT;   // Número de fases descartando N e T
   double          defasagem_rad;      // Defasagem entre duas fases quaisquer [rad]
   double          snom_mva;           // Potência nominal (MVA)
   double          rt_pu;              // Resistência de aterramento (pu na base Sbase)
   double          xm_pu;              // Reatância mútua entre fases (pu na base Sbase)
   double          fp_min;             // Valor mínimo para o fator de potência (pu) - PV
   complex<double> zp_pu;              // Impedância própria de cada fase (pu na base Sbase)
   complex<double> pot_curto1_pu;      // Pot. de curto-circuito fase-terra (pu na base Sbase)
   complex<double> pot_curto3_pu;      // Pot. de curto-circuito trifásico (pu na base Sbase)
   complex<double> vneutro_pu;         // Tensão do centro-estrela (pu)
   complex<double> *vet_tensao_pu;     // Tensão de fase especificada (ref. à referência global ; pu)
   complex<double> *vet_sesp_mva;      // Potência complexa especificada (MVA)

   // Implementação do aterramento do suprimento
   bool            flag_barra_interna; // Flag para incluir (true) barra interna ou não (false)
   int             tipo_ater;          // Tipo do aterramento (atIsolado, atZ ou atSolido)
   complex<double> yater_pu;           // Admitância de aterramento do neutro (pu)

public:  // Métodos
          __fastcall  TSup(TBarra *bar_ext,
                           int    tipo,
                           int    fases_int);
          __fastcall ~TSup(void);
   bool   __fastcall  InsereYnodalH(int      ordemh,
                                    VTSisLin *Ynod);
   void   __fastcall  ZeraPotencia(void);

private:  // Métodos
   double __fastcall DefasagemRad(double ang1_rad,
                                  double ang2_rad);
};

#endif
//---------------------------------------------------------------------------

