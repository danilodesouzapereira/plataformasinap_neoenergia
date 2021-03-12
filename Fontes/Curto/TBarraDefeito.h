//---------------------------------------------------------------------------
#ifndef TBarraDefeitoH
#define TBarraDefeitoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TBarraDefeito : public TObject
{
public:  // Dados locais
   TBarra *barra;
   int    tipo_curto;  // Cf. 'enumCU' em VTCurtoAux.h (cuTRI=0, cu2F, cuFT, cuFTZ, cu2FT)
   int    fases_curto; // Cf. 'enum_FASES_CURTO' em VTGeralC.h (ctAN=0, ctBN, ctCN, ...)
   bool   calculavel;  // Indica se pode (ou não) ser calculada a Zentrada para esta barra

public:  // Métodos
              __fastcall  TBarraDefeito(TBarra *barra,
                                        int    tipo_curto,
                                        int    fases_curto);
              __fastcall ~TBarraDefeito(void);
};

#endif
//---------------------------------------------------------------------------

