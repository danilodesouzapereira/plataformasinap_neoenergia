
//---------------------------------------------------------------------------
#ifndef VTMatrizH
#define VTMatrizH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include <complex>

// Tipo das matrizes
enum enum_TIPO_MATRIZ {mtINVALIDO=-1, mtINT=0, mtDOUBLE, mtCOMPLEX};

// Tolerância para pivô nulo (inversão de matriz quadrada de duas dimensões)
#define toler_pivo   1.e-14

//---------------------------------------------------------------------------
class VTTempoCPU;

//---------------------------------------------------------------------------
class VTMatriz : public TObject
   {
   public:
      // Métodos comuns às implementações complex<double>, double e int
                         __fastcall  VTMatriz() {};
      virtual            __fastcall ~VTMatriz(void) {};
      virtual bool       __fastcall  Copia(VTMatriz *mat_dest) = 0;
      virtual void       __fastcall  CopiaTriangSupTriangInf(void) = 0;
      virtual bool       __fastcall  EstruturaIdentica(VTMatriz *matB) = 0;
      virtual VTMatriz * __fastcall  Identidade(int        num1_aux,
                                                AnsiString nome_matriz="Matriz",
                                                VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool       __fastcall  Imprime(AnsiString nome_arq,
                                             int        chave=0) = 0;
      virtual VTMatriz * __fastcall  Inverte(AnsiString nome_inversa="Matriz_Inversa",
                                             VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool       __fastcall  MatrizQuadrada(void) = 0;
      virtual VTMatriz * __fastcall  NovaCopia(int        tipo_aux=mtINVALIDO,
                                               AnsiString nome_copia="Matriz_Copia",
                                               VTTempoCPU *tCPU2=NULL) = 0;
      virtual VTMatriz * __fastcall  ProdutoMM(VTMatriz   *MB,
                                               AnsiString nome_produto="Matriz_Produto",
                                               VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool       __fastcall  ProdutoMV(VTMatriz *vet_ent,
                                               VTMatriz *vet_sai) = 0;
      virtual VTMatriz * __fastcall  Transposta(AnsiString nome_transp="Matriz_Transposta",
                                                VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool       __fastcall  Triang(void) = 0;
      virtual void       __fastcall  Zera(void) = 0;

      // Métodos com argumentos complex<double>
      virtual bool            __fastcall Back(complex<double> *vet_TC) = 0;
      virtual void            __fastcall DefineValor(complex<double> valor) = 0;
      virtual void            __fastcall Insere(int             linha,
                                                int             coluna,
                                                complex<double> valor) = 0;
      virtual void            __fastcall InsereDiagonal(int             linha,
                                                        complex<double> valor) = 0;
      virtual void            __fastcall InsereQuatro(int             linha,
                                                      int             coluna,
                                                      complex<double> valor) = 0;
      virtual void            __fastcall InsereSimetrico(int             linha,
                                                         int             coluna,
                                                         complex<double> valor) = 0;
      virtual bool            __fastcall ProdutoMV(complex<double> *vet_ent,
                                                   complex<double> *vet_sai) = 0;
      virtual complex<double> __fastcall ProdutoVV(complex<double> *vet_ent) = 0;
      virtual complex<double> __fastcall ProdutoVVC(VTMatriz *vet_ent) = 0;

      // Métodos com argumentos double
      virtual bool            __fastcall Back(double *vet_TC) = 0;
      virtual void            __fastcall DefineValor(double valor) = 0;
      virtual void            __fastcall Insere(int    linha,
                                                int    coluna,
                                                double valor) = 0;
      virtual void            __fastcall InsereDiagonal(int    linha,
                                                        double valor) = 0;
      virtual void            __fastcall InsereQuatro(int    linha,
                                                      int    coluna,
                                                      double valor) = 0;
      virtual void            __fastcall InsereSimetrico(int    linha,
                                                         int    coluna,
                                                         double valor) = 0;
      virtual bool            __fastcall ProdutoMV(double *vet_ent,
                                                   double *vet_sai) = 0;
      virtual double          __fastcall ProdutoVV(double *vet_ent) = 0;
      virtual double          __fastcall ProdutoVVD(VTMatriz *vet_ent) = 0;

      // Métodos com argumentos int
      virtual bool            __fastcall Back(int *vet_TC) = 0;
      virtual void            __fastcall DefineValor(int valor) = 0;
      virtual void            __fastcall Insere(int linha,
                                                int coluna,
                                                int valor) = 0;
      virtual void            __fastcall InsereDiagonal(int linha,
                                                        int valor) = 0;
      virtual void            __fastcall InsereQuatro(int linha,
                                                      int coluna,
                                                      int valor) = 0;
      virtual void            __fastcall InsereSimetrico(int linha,
                                                         int coluna,
                                                         int valor) = 0;
      virtual bool            __fastcall ProdutoMV(int *vet_ent,
                                                   int *vet_sai) = 0;
      virtual int             __fastcall ProdutoVV(int *vet_ent) = 0;
      virtual int             __fastcall ProdutoVVI(VTMatriz *vet_ent) = 0;

   public:  // Properties
      __property bool JaFatorada   = {read=PM_GetJaFatorada};
      __property int  Tipo         = {read=PM_GetTipo};
      __property int  NumColunas   = {read=PM_GetNumColunas};
      __property int  NumDimensoes = {read=PM_GetNumDimensoes};
      __property int  NumElementos = {read=PM_GetNumElementos};
      __property int  NumLinhas    = {read=PM_GetNumLinhas};
      __property int  NumPlanos    = {read=PM_GetNumPlanos};

   public:  // Properties para acesso aos elementos da matriz
      __property complex<double> AddC1[int i]               = {write=PM_SetAddC1};
      __property complex<double> AddC2[int i][int j]        = {write=PM_SetAddC2};
      __property complex<double> AddC3[int i][int j][int k] = {write=PM_SetAddC3};
      __property complex<double> C1[int i]                  = {read=PM_GetC1, write=PM_SetC1};
      __property complex<double> C2[int i][int j]           = {read=PM_GetC2, write=PM_SetC2};
      __property complex<double> C3[int i][int j][int k]    = {read=PM_GetC3, write=PM_SetC3};

      __property double          AddD1[int i]               = {write=PM_SetAddD1};
      __property double          AddD2[int i][int j]        = {write=PM_SetAddD2};
      __property double          AddD3[int i][int j][int k] = {write=PM_SetAddD3};
      __property double          D1[int i]                  = {read=PM_GetD1, write=PM_SetD1};
      __property double          D2[int i][int j]           = {read=PM_GetD2, write=PM_SetD2};
      __property double          D3[int i][int j][int k]    = {read=PM_GetD3, write=PM_SetD3};

      __property int             AddI1[int i]               = {write=PM_SetAddI1};
      __property int             AddI2[int i][int j]        = {write=PM_SetAddI2};
      __property int             AddI3[int i][int j][int k] = {write=PM_SetAddI3};
      __property int             I1[int i]                  = {read=PM_GetI1, write=PM_SetI1};
      __property int             I2[int i][int j]           = {read=PM_GetI2, write=PM_SetI2};
      __property int             I3[int i][int j][int k]    = {read=PM_GetI3, write=PM_SetI3};

   protected:  // Métodos acessados via property
      virtual bool __fastcall PM_GetJaFatorada(void)   = 0;
      virtual int  __fastcall PM_GetTipo(void)         = 0;
      virtual int  __fastcall PM_GetNumColunas(void)   = 0;
      virtual int  __fastcall PM_GetNumDimensoes(void) = 0;
      virtual int  __fastcall PM_GetNumElementos(void) = 0;
      virtual int  __fastcall PM_GetNumLinhas(void)    = 0;
      virtual int  __fastcall PM_GetNumPlanos(void)    = 0;

   protected:  // Métodos acessados via property
      virtual complex<double> __fastcall PM_GetC1(int i) = 0;
      virtual complex<double> __fastcall PM_GetC2(int i, int j) = 0;
      virtual complex<double> __fastcall PM_GetC3(int i, int j, int k) = 0;
      virtual double          __fastcall PM_GetD1(int i) = 0;
      virtual double          __fastcall PM_GetD2(int i, int j) = 0;
      virtual double          __fastcall PM_GetD3(int i, int j, int k) = 0;
      virtual int             __fastcall PM_GetI1(int i) = 0;
      virtual int             __fastcall PM_GetI2(int i, int j) = 0;
      virtual int             __fastcall PM_GetI3(int i, int j, int k) = 0;
      //
      virtual void            __fastcall PM_SetAddC1(int i, complex<double> valor) = 0;
      virtual void            __fastcall PM_SetAddC2(int i, int j, complex<double> valor) = 0;
      virtual void            __fastcall PM_SetAddC3(int i, int j, int k, complex<double> valor) = 0;
      virtual void            __fastcall PM_SetAddD1(int i, double valor) = 0;
      virtual void            __fastcall PM_SetAddD2(int i, int j, double valor) = 0;
      virtual void            __fastcall PM_SetAddD3(int i, int j, int k, double valor) = 0;
      virtual void            __fastcall PM_SetAddI1(int i, int valor) = 0;
      virtual void            __fastcall PM_SetAddI2(int i, int j, int valor) = 0;
      virtual void            __fastcall PM_SetAddI3(int i, int j, int k, int valor) = 0;

      virtual void            __fastcall PM_SetC1(int i, complex<double> valor) = 0;
      virtual void            __fastcall PM_SetC2(int i, int j, complex<double> valor) = 0;
      virtual void            __fastcall PM_SetC3(int i, int j, int k, complex<double> valor) = 0;
      virtual void            __fastcall PM_SetD1(int i, double valor) = 0;
      virtual void            __fastcall PM_SetD2(int i, int j, double valor) = 0;
      virtual void            __fastcall PM_SetD3(int i, int j, int k, double valor) = 0;
      virtual void            __fastcall PM_SetI1(int i, int valor) = 0;
      virtual void            __fastcall PM_SetI2(int i, int j, int valor) = 0;
      virtual void            __fastcall PM_SetI3(int i, int j, int k, int valor) = 0;
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe TMatrizC
//---------------------------------------------------------------------------
VTMatriz * __fastcall NewObjMatrizC(int        num1,
                                    int        num2=0,
                                    int        num3=0,
                                    VTTempoCPU *tCPU=NULL,
                                    AnsiString nome_matriz="MatrizC");

//---------------------------------------------------------------------------
// Função global para criar objeto da classe TMatrizD
//---------------------------------------------------------------------------
VTMatriz * __fastcall NewObjMatrizD(int        num1,
                                    int        num2=0,
                                    int        num3=0,
                                    VTTempoCPU *tCPU=NULL,
                                    AnsiString nome_matriz="MatrizD");

//---------------------------------------------------------------------------
// Função global para criar objeto da classe TMatrizI
//---------------------------------------------------------------------------
VTMatriz * __fastcall NewObjMatrizI(int        num1,
                                    int        num2=0,
                                    int        num3=0,
                                    VTTempoCPU *tCPU=NULL,
                                    AnsiString nome_matriz="MatrizI");

#endif
//---------------------------------------------------------------------------
// eof
