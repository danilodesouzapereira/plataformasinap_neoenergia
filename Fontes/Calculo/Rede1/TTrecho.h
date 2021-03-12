//---------------------------------------------------------------------------
#ifndef TTrechoH
#define TTrechoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG1.h"
#include <complex>
using namespace std;

class TBarra;
class VTEqpto;
class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TTrecho : public VTLigacaoG1
{
public:  // Properties
   __property complex<double> Y0cap_pu = {read=PM_GetY0cap_pu, write=PM_SetY0cap_pu};
   __property complex<double> Y1cap_pu = {read=PM_GetY1cap_pu, write=PM_SetY1cap_pu};
   __property complex<double> Y0ser_pu = {read=PM_GetY0ser_pu, write=PM_SetY0ser_pu};
   __property complex<double> Y1ser_pu = {read=PM_GetY1ser_pu, write=PM_SetY1ser_pu};
   __property complex<double> Z0ser_pu = {read=PM_GetZ0ser_pu, write=PM_SetZ0ser_pu};
   __property complex<double> Z1ser_pu = {read=PM_GetZ1ser_pu, write=PM_SetZ1ser_pu};

public:  // Métodos
                   __fastcall  TTrecho(TBarra *bar1,
                                       TBarra *bar2);
                   __fastcall ~TTrecho(void);
   int             __fastcall  Barras(TBarra **bar1,
                                      TBarra **bar2,
                                      TBarra **bar3);
   void            __fastcall  Calcula(int      np,
                                       VTGeralC *geralC);
   complex<double> __fastcall  CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                       complex<double> *vet_tensoes);
   void            __fastcall  CalculaPiEquivalente(void);
   complex<double> __fastcall  CalculaPotenciaInjetada(TBarra          *bar_ref,
                                                       complex<double> *vet_tensoes);
   int             __fastcall  IndiceGlobal(int indice_local);
   int             __fastcall  IndiceLocal(TBarra *bar_ref);
   bool            __fastcall  InsereYnodal0(VTGeralC *geralC,
                                             VTSisLin *Ynod);
   bool            __fastcall  InsereYnodal1(VTGeralC *geralC,
                                             VTSisLin *Ynod);
   bool            __fastcall  MontaYnodal1_pu(VTGeralC *geralC);
   int             __fastcall  NumBarras(void);
   int             __fastcall  TipoLigacao(void);
   void            __fastcall  TrocaBarras(void);
   complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                        TBarra   *bar1);
   complex<double> __fastcall  Z1ser2_pu(void);
   void            __fastcall  ZeraPerdaEnergia(void);

public:  // Dados locais
   int             id;                 // ID interno do trecho (gerado pelo programa)
   int             id_bar1_orig;       // ID da barra "de" no SINAP (sentido do fluxo)
   int             grupo_mutua;        // grupo_mutua >= 0: trecho pertence a grupo de mútua
   bool            flag_chave_fechada; // Flag que indica chave fechada (true) ou aberta (false);
                                       //   usado somente no módulo de Estabilidade
   VTEqpto         *eqpto;             // Ponteiro para equipamento SINAP
   AnsiString      codigo;             // Codigo do trecho
   TBarra          *bar1;              // Barra inicial (ponteiro para struct Barra)
   TBarra          *bar2;              // Barra final   (ponteiro para struct Barra)
   double          compr_km;           // Comprimento do trecho (km)
   double          iadm_A;             // Corrente admissível (A)
   complex<double> Ynodal0_ji_pu;      // Elemento ji de Ynodal0, usado somente em Eqv1/Eqv3 (pu)
   complex<double> Ynodal1_ji_pu;      // Elemento ji de Ynodal1, usado somente em Eqv1/Eqv3 (pu)
   complex<double> vet_corr_a[2];      // Corrente no trecho (entrada & saída) (A)
   complex<double> vet_s_kva[2];       // Potência que entra nas barras inicial e final (kVA)
   complex<double> perda_kva;          // Perda em potência no trecho (kVA)
   complex<double> *ynodalD_pu;        // Matriz Ynodal do trafo - seq. DIRETA (pu na base 'Sbase')
   double          perda_energia_diaria_kwh;  // Perda diária em energia (kWh)

private:  // Métodos acessados via property
   complex<double> __fastcall PM_GetY0cap_pu(void);
   complex<double> __fastcall PM_GetY0ser_pu(void);
   complex<double> __fastcall PM_GetY1cap_pu(void);
   complex<double> __fastcall PM_GetY1ser_pu(void);
   complex<double> __fastcall PM_GetZ0ser_pu(void);
   complex<double> __fastcall PM_GetZ1ser_pu(void);

   void            __fastcall PM_SetY0cap_pu(complex<double> valor);
   void            __fastcall PM_SetY0ser_pu(complex<double> valor);
   void            __fastcall PM_SetY1cap_pu(complex<double> valor);
   void            __fastcall PM_SetY1ser_pu(complex<double> valor);
   void            __fastcall PM_SetZ0ser_pu(complex<double> valor);
   void            __fastcall PM_SetZ1ser_pu(complex<double> valor);

private:  // Dados
   complex<double> z0ser_pu; // Impedância série de seqüência zero        (TOTAL DA LINHA) (pu)
   complex<double> z1ser_pu; // Impedância série de seqüência direta      (TOTAL DA LINHA) (pu)
   complex<double> y0cap_pu; // Susceptância paralelo de seqüência zero   (TOTAL DA LINHA) (pu)
   complex<double> y1cap_pu; // Susceptância paralelo de seqüência direta (TOTAL DA LINHA) (pu)
   complex<double> y0ser_pu; // Admitância série de seqüência zero        (TOTAL DA LINHA) (pu)
   complex<double> y1ser_pu; // Admitância série de seqüência direta      (TOTAL DA LINHA) (pu)
};

#endif
//---------------------------------------------------------------------------

