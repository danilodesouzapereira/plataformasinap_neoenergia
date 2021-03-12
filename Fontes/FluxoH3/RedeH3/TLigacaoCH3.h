//---------------------------------------------------------------------------
#ifndef TLigacaoCH3H
#define TLigacaoCH3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>

class TLigacaoC;
class VTMatriz;

//---------------------------------------------------------------------------
class TLigacaoCH3 : public TObject
{
public:
              __fastcall  TLigacaoCH3(TLigacaoC *ligacao,
                                      int       num_harmonicas);
              __fastcall ~TLigacaoCH3(void);

public:  // Dados locais
   TLigacaoC *ligacao;
   int       num_fases;
   int       offset;
   double    *dht_corrente;    // Distorção harmônica total de corrente (%)
   double    *vef_corrente_A;  // Valor eficaz total de corrente (A)

   VTMatriz  *dh_corrente;    // Distorção harmônica de corrente (%)
   VTMatriz  *corrente_A;     // Corrente (A)
   VTMatriz  *perda_kva;      // Perda (kVA)
};

#endif
//---------------------------------------------------------------------------
//eof
