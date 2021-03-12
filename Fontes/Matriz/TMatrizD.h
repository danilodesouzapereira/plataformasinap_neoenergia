
//---------------------------------------------------------------------------
#ifndef TMatrizDH
#define TMatrizDH

//---------------------------------------------------------------------------
#include ".\TMatriz.h"

//---------------------------------------------------------------------------
class TMatrizD : public TMatriz
   {
   public:
      // Métodos comuns às implementações complex<double>, double e int
                 __fastcall  TMatrizD(int        num1,
                                      int        num2,
                                      int        num3,
                                      VTTempoCPU *tCPU,
                                      AnsiString nome_matriz);
                 __fastcall ~TMatrizD(void);
      bool       __fastcall  Copia(VTMatriz *mat_dest);
      void       __fastcall  CopiaTriangSupTriangInf(void);
      VTMatriz * __fastcall  Identidade(int        num1_aux,
                                        AnsiString nome_matriz,
                                        VTTempoCPU *tCPU2);
      bool       __fastcall  Imprime(AnsiString nome_arq,
                                     int        chave);
      VTMatriz * __fastcall  Inverte(AnsiString nome_inversa,
                                     VTTempoCPU *tCPU2);
      VTMatriz * __fastcall  NovaCopia(int        tipo_aux,
                                       AnsiString nome_copia,
                                       VTTempoCPU *tCPU2);
      VTMatriz * __fastcall  ProdutoMM(VTMatriz   *MB,
                                       AnsiString nome_produto,
                                       VTTempoCPU *tCPU2);
      bool       __fastcall  ProdutoMV(VTMatriz *vet_ent,
                                       VTMatriz *vet_sai);
      VTMatriz * __fastcall  Transposta(AnsiString nome_transp,
                                        VTTempoCPU *tCPU2);
      bool       __fastcall  Triang(void);
      void       __fastcall  Zera(void);

      // Métodos com argumentos complex<double> (implementação nula)
      bool            __fastcall Back(complex<double> *vet_TC) {return(true);};
      void            __fastcall DefineValor(complex<double> valor) {};
      void            __fastcall Insere(int             linha,
                                        int             coluna,
                                        complex<double> valor) {};
      void            __fastcall InsereDiagonal(int    linha,
                                                complex<double> valor) {};
      void            __fastcall InsereQuatro(int             linha,
                                              int             coluna,
                                              complex<double> valor) {};
      void            __fastcall InsereSimetrico(int             linha,
                                                 int             coluna,
                                                 complex<double> valor) {};
      bool            __fastcall ProdutoMV(complex<double> *vet_ent,
                                           complex<double> *vet_sai) {return(true);};
      complex<double> __fastcall ProdutoVV(complex<double> *vet_ent) {return(complex<double>(0., 0.));};
      complex<double> __fastcall ProdutoVVC(VTMatriz *vet_ent) {return(complex<double>(0., 0.));};

      // Métodos com argumentos double
      bool            __fastcall Back(double *vet_TC);
      void            __fastcall DefineValor(double valor);
      void            __fastcall Insere(int    linha,
                                        int    coluna,
                                        double valor);
      void            __fastcall InsereDiagonal(int    linha,
                                                double valor);
      void            __fastcall InsereQuatro(int    linha,
                                              int    coluna,
                                              double valor);
      void            __fastcall InsereSimetrico(int    linha,
                                                 int    coluna,
                                                 double valor);
      bool            __fastcall ProdutoMV(double *vet_ent,
                                           double *vet_sai);
      double          __fastcall ProdutoVV(double *vet_ent);
      double          __fastcall ProdutoVVD(VTMatriz *vet_ent);

      // Métodos com argumentos int (implementação nula)
      bool            __fastcall Back(int *vet_TC) {return(true);};
      void            __fastcall DefineValor(int valor) {};
      void            __fastcall Insere(int linha,
                                        int coluna,
                                        int valor) {};
      void            __fastcall InsereDiagonal(int linha,
                                                int valor) {};
      void            __fastcall InsereQuatro(int linha,
                                              int coluna,
                                              int valor) {};
      void            __fastcall InsereSimetrico(int linha,
                                                 int coluna,
                                                 int valor) {};
      bool            __fastcall ProdutoMV(int *vet_ent,
                                           int *vet_sai) {return(true);};
      int             __fastcall ProdutoVV(int *vet_ent) {return(0);};
      int             __fastcall ProdutoVVI(VTMatriz *vet_ent) {return(0);};

   private: // Dados
      double *vet_dados;

   private:  // Métodos com dados complex<double> (implementação nula)
      complex<double> __fastcall PM_GetC1(int i) {return(complex<double>(0., 0.));};
      complex<double> __fastcall PM_GetC2(int i, int j) {return(complex<double>(0., 0.));};
      complex<double> __fastcall PM_GetC3(int i, int j, int k) {return(complex<double>(0., 0.));};
      void            __fastcall PM_SetAddC1(int i, complex<double> valor) {};
      void            __fastcall PM_SetAddC2(int i, int j, complex<double> valor) {};
      void            __fastcall PM_SetAddC3(int i, int j, int k, complex<double> valor) {};
      void            __fastcall PM_SetC1(int i, complex<double> valor) {};
      void            __fastcall PM_SetC2(int i, int j, complex<double> valor) {};
      void            __fastcall PM_SetC3(int i, int j, int k, complex<double> valor) {};

   private:  // Métodos com dados double
      double          __fastcall PM_GetD1(int i);
      double          __fastcall PM_GetD2(int i, int j);
      double          __fastcall PM_GetD3(int i, int j, int k);
      void            __fastcall PM_SetAddD1(int i, double valor);
      void            __fastcall PM_SetAddD2(int i, int j, double valor);
      void            __fastcall PM_SetAddD3(int i, int j, int k, double valor);
      void            __fastcall PM_SetD1(int i, double valor);
      void            __fastcall PM_SetD2(int i, int j, double valor);
      void            __fastcall PM_SetD3(int i, int j, int k, double valor);

   private:  // Métodos com dados int (implementação nula)
      int             __fastcall PM_GetI1(int i) {return(0);};
      int             __fastcall PM_GetI2(int i, int j) {return(0);};
      int             __fastcall PM_GetI3(int i, int j, int k) {return(0);};
      void            __fastcall PM_SetAddI1(int i, int valor) {};
      void            __fastcall PM_SetAddI2(int i, int j, int valor) {};
      void            __fastcall PM_SetAddI3(int i, int j, int k, int valor) {};
      void            __fastcall PM_SetI1(int i, int valor) {};
      void            __fastcall PM_SetI2(int i, int j, int valor) {};
      void            __fastcall PM_SetI3(int i, int j, int k, int valor) {};

   private:  // Métodos locais
      bool            __fastcall CopiaElementosC(VTMatriz *mat_dest);
      bool            __fastcall Imprime1(AnsiString nome_arq,
                                          int        chave);
      bool            __fastcall Imprime2(AnsiString nome_arq,
                                          int        chave);
      bool            __fastcall Imprime3(AnsiString nome_arq,
                                          int        chave);
   };

#endif
//---------------------------------------------------------------------------
//eof

