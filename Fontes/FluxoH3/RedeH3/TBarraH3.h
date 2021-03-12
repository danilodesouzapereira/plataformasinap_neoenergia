//---------------------------------------------------------------------------
#ifndef TBarraH3H
#define TBarraH3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>

class TBarra;

//---------------------------------------------------------------------------
class TBarraH3 : public TObject
{
public:
              __fastcall  TBarraH3(TBarra *barra,
                                   int    num_harmonicas);
              __fastcall ~TBarraH3(void);

public:  // Dados locais
   TBarra          *barra;           // TBarra do objeto TBarraH3
   TList           *lisLIG;          // Lista de liga��es da barra (classe TLigacaoCH3)
   double          *dht_corrente;    // Distor��o harm�nica total de corrente (%)
   double          *dht_tensao;      // Distor��o harm�nica total de tens�o (%)
   double          *vef_corrente_A;  // Valor eficaz total de corrente (A)
   double          *vef_tensao_pu;    // Valor eficaz total de tens�o (pu)

   double          **dh_corrente;    // Distor��o harm�nica de corrente (%)
   double          **dh_tensao;      // Distor��o harm�nica de tens�o (%)
   complex<double> **tensao_pu;      // Tens�o de fase na barra (ref. ao neutro local ; pu)
   complex<double> **corrente_A;     // Corrente de carga/injetada (A)
};

#endif
//---------------------------------------------------------------------------
//eof
