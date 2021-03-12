//---------------------------------------------------------------------------
#ifndef TFaseH
#define TFaseH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TFase : public TObject
{
public:  // Métodos
                     __fastcall  TFase(int  fases_int,
                                       bool flag_ordena_fases=true,
                                       bool flag_linhas=false);
                     __fastcall ~TFase(void);
          bool       __fastcall  AddFase(int  fase,
                                         bool flag_ordena_fases=true);
          bool       __fastcall  Copia(TFase *fases2);
          bool       __fastcall  ExisteFase(int fase_int);
          bool       __fastcall  ExisteFaseNumero(int numero);
          bool       __fastcall  ExisteN(void);
          int        __fastcall  Fase(int indice);
          char       __fastcall  FaseAsChar(int fase_int);
          char       __fastcall  FaseAsCharIndice(int indice);
          int        __fastcall  FaseNumero(int numero);
          int        __fastcall  FasesAsInt(void);
          AnsiString __fastcall  FasesAsString(void);
          bool       __fastcall  FasesIguais(TFase *fases2);
          void       __fastcall  Impoe4Fases(void);
          int        __fastcall  Indice(int fase_int);
          int        __fastcall  IndiceA(void);
          int        __fastcall  IndiceABCN(int indice);
          int        __fastcall  IndiceABCN_Fase(int fase_int);
          int        __fastcall  IndiceB(void);
          int        __fastcall  IndiceC(void);
          int        __fastcall  IndiceLinear(int i,
                                              int j);
          int        __fastcall  IndiceN(void);
          bool       __fastcall  IsFaseABC(int fase_int);
          int        __fastcall  LinhaFase(int fase_int);
          int        __fastcall  LinhaIndice(int indice);
          int        __fastcall  NumFases(void);
          int        __fastcall  NumFasesSemNT(void);
          int        __fastcall  NumFasesQ(void);
          void       __fastcall  Reinicia(void);
          void       __fastcall  ReiniciaLinhas(void);
          bool       __fastcall  RemoveFase(int fase_int);
          bool       __fastcall  SetFase(int indice,
                                         int fase_int);
          bool       __fastcall  SetLinhaIndice(int indice,
                                                int linha);
          void       __fastcall  SetNumFases(int num_fases);
          int        __fastcall  Uniao(TFase *fases2,
                                       int   *vet_fases);
          bool       __fastcall  VerificaFases(int num_fases,
                                               int *vet_fases);

private:  // Métodos
          bool    __fastcall AddFaseVetor(int  fase,
                                          int  *num_fases_aux,
                                          int  *vet_fases_aux,
                                          bool flag_ordena_fases=true);
          void    __fastcall MontaVetorFases(int  fases_int,
                                             bool flag_ordena_fases);

private:  // Dados locais
   int num_fases;    // Número de fases (0 a MAX_FASES_G3)
   int *vet_fases;   // Vetor das fases (faseA, faseB, faseC ou faseN)
   int *vet_linhas;  // Linha de cada fase em Ynodal
};

#endif
//---------------------------------------------------------------------------

