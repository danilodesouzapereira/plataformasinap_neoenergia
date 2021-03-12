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
   TList           *lisLIG;          // Lista de ligações da barra (classe TLigacaoCH3)
   double          *dht_corrente;    // Distorção harmônica total de corrente (%)
   double          *dht_tensao;      // Distorção harmônica total de tensão (%)
   double          *vef_corrente_A;  // Valor eficaz total de corrente (A)
   double          *vef_tensao_pu;    // Valor eficaz total de tensão (pu)

   double          **dh_corrente;    // Distorção harmônica de corrente (%)
   double          **dh_tensao;      // Distorção harmônica de tensão (%)
   complex<double> **tensao_pu;      // Tensão de fase na barra (ref. ao neutro local ; pu)
   complex<double> **corrente_A;     // Corrente de carga/injetada (A)
};

#endif
//---------------------------------------------------------------------------
//eof
