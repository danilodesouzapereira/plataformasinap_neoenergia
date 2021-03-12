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
   TList    *lisTRECHO;   // Lista dos trechos que comp�em o grupo de m�tuas
   TList    *lisMUTUA;    // Lista das m�tuas que comp�em o grupo de m�tuas
   VTSisLin *Yserie_pu;   // Matriz Yserie do grupo (pu)
   VTSisLin *Zserie_pu;   // Matriz Zserie do grupo (pu)
   int      *vet_offset;  // Offset da primeira linha de cada trecho em Yelem

public:  // M�todos
              __fastcall  TGrupoMutua(void);
              __fastcall ~TGrupoMutua(void);
         void __fastcall  Calcula(int      np,
                                  VTGeralC *geralC);
         bool __fastcall  InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod,
                                       bool     flag_inclui_mutuas);
         bool __fastcall  MontaMatYserie(int ordemh);
         bool __fastcall  MontaMatZserie(VTGeralC *geralC);

private:  // Dados
         complex<double> cmeio;
};

#endif
//---------------------------------------------------------------------------

