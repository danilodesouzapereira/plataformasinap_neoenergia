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
   AnsiString      codigo;             // C�digo do suprimento
   TBarra          *bar_ext;           // Ponteiro para a barra externa do gerador
   TBarra          *bar_int;           // Ponteiro para a barra interna do gerador
   TFase           *fases;             // Fases do suprimento
   int             num_fases_sem_NT;   // N�mero de fases descartando N e T
   double          defasagem_rad;      // Defasagem entre duas fases quaisquer [rad]
   double          snom_mva;           // Pot�ncia nominal (MVA)
   double          rt_pu;              // Resist�ncia de aterramento (pu na base Sbase)
   double          xm_pu;              // Reat�ncia m�tua entre fases (pu na base Sbase)
   double          fp_min;             // Valor m�nimo para o fator de pot�ncia (pu) - PV
   complex<double> zp_pu;              // Imped�ncia pr�pria de cada fase (pu na base Sbase)
   complex<double> pot_curto1_pu;      // Pot. de curto-circuito fase-terra (pu na base Sbase)
   complex<double> pot_curto3_pu;      // Pot. de curto-circuito trif�sico (pu na base Sbase)
   complex<double> vneutro_pu;         // Tens�o do centro-estrela (pu)
   complex<double> *vet_tensao_pu;     // Tens�o de fase especificada (ref. � refer�ncia global ; pu)
   complex<double> *vet_sesp_mva;      // Pot�ncia complexa especificada (MVA)

   // Implementa��o do aterramento do suprimento
   bool            flag_barra_interna; // Flag para incluir (true) barra interna ou n�o (false)
   int             tipo_ater;          // Tipo do aterramento (atIsolado, atZ ou atSolido)
   complex<double> yater_pu;           // Admit�ncia de aterramento do neutro (pu)

public:  // M�todos
          __fastcall  TSup(TBarra *bar_ext,
                           int    tipo,
                           int    fases_int);
          __fastcall ~TSup(void);
   bool   __fastcall  InsereYnodalH(int      ordemh,
                                    VTSisLin *Ynod);
   void   __fastcall  ZeraPotencia(void);

private:  // M�todos
   double __fastcall DefasagemRad(double ang1_rad,
                                  double ang2_rad);
};

#endif
//---------------------------------------------------------------------------

