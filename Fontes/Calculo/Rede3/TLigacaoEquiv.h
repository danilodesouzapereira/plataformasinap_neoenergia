//---------------------------------------------------------------------------
#ifndef TLigacaoEquivH
#define TLigacaoEquivH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class TFase;
class VTMatriz;

//---------------------------------------------------------------------------
class TLigacaoEquiv : public TObject
{
public:  // Properties
   __property complex<double>  MatYnodal_ij_pu[int i][int j] = {read  = PM_GetMatYnodal_ij_pu,
                                                                write = PM_SetMatYnodal_ij_pu};
   __property complex<double>  MatYnodal_ji_pu[int i][int j] = {read  = PM_GetMatYnodal_ji_pu,
                                                                write = PM_SetMatYnodal_ji_pu};

public:  // Dados
   int             id;          // ID interno da ligacao
   AnsiString      codigo;      // Codigo do trecho
   TBarra          *bar1;       // Ponteiro para a barra "de" na lista de barras
   TBarra          *bar2;       // Ponteiro para a barra "para" na lista de barras
   TFase           *fases_pri;  // Objeto que armazena as fases da barra inicial
   TFase           *fases_sec;  // Objeto que armazena as fases da barra final

public:  // Métodos
                                __fastcall  TLigacaoEquiv(TBarra   *bar1,
                                                          TBarra   *bar2,
                                                          int      id,
                                                          int      fases_int_pri,
                                                          int      fases_int_sec,
                                                          VTMatriz *mat_ij,
                                                          VTMatriz *mat_ji);
                                __fastcall ~TLigacaoEquiv(void);
                           void __fastcall  ImprimeMatriz(AnsiString nome_arq,
                                                          int        ordem);

private:  // Dados
   VTMatriz *mat_Ynodal_ij_pu;  // Submatriz nodal ij da ligação (pu)
   VTMatriz *mat_Ynodal_ji_pu;  // Submatriz nodal ji da ligação (pu)

private:  // Métodos
   complex<double> __fastcall PM_GetMatYnodal_ij_pu(int i, int j);
   complex<double> __fastcall PM_GetMatYnodal_ji_pu(int i, int j);
   void            __fastcall PM_SetMatYnodal_ij_pu(int i, int j, complex<double> valor);
   void            __fastcall PM_SetMatYnodal_ji_pu(int i, int j, complex<double> valor);
};

//---------------------------------------------------------------------------
// Função global para criar objeto da classe TLigacaoEquiv
//---------------------------------------------------------------------------
TLigacaoEquiv * __fastcall NewObjLigacaoEquiv(TBarra   *bar1,
                                              TBarra   *bar2,
                                              int      id,
                                              int      fases_int_pri,
                                              int      fases_int_sec,
                                              VTMatriz *mat_ij,
                                              VTMatriz *mat_ji);

#endif
//---------------------------------------------------------------------------

