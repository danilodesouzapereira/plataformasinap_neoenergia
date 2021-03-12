//---------------------------------------------------------------------------
#ifndef TGrupoMutuaH
#define TGrupoMutuaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TGrupoMutua : public TObject
{
public:  // Dados locais
   TList    *lisTRECHO;  // Lista dos trechos que compõem o grupo de mútuas
   TList    *lisMUTUA;   // Lista das mútuas que compõem o grupo de mútuas
   VTSisLin *Yelem0_pu;  // Matriz Yelem do grupo - sequência zero
   VTSisLin *Yelem1_pu;  // Matriz Yelem do grupo - sequência direta

public:  // Métodos
              __fastcall  TGrupoMutua(void);
              __fastcall ~TGrupoMutua(void);
         void __fastcall  Calcula(int      np,
                                  VTGeralC *geralC);
         bool __fastcall  InsereYnodal(int      seq,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod,
                                       bool     flag_inclui_mutuas=true);
         bool __fastcall  MontaMatrizYelem(int    seq,
                                           double Sbase);

private:  // Métodos
         void __fastcall DeletaLista(TList **lista);

private:  // Dados
         complex<double> cmeio;
};

#endif
//---------------------------------------------------------------------------

