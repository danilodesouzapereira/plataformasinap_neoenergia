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
   TList    *lisTRECHO;  // Lista dos trechos que comp�em o grupo de m�tuas
   TList    *lisMUTUA;   // Lista das m�tuas que comp�em o grupo de m�tuas
   VTSisLin *Yelem0_pu;  // Matriz Yelem do grupo - sequ�ncia zero
   VTSisLin *Yelem1_pu;  // Matriz Yelem do grupo - sequ�ncia direta

public:  // M�todos
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

private:  // M�todos
         void __fastcall DeletaLista(TList **lista);

private:  // Dados
         complex<double> cmeio;
};

#endif
//---------------------------------------------------------------------------

