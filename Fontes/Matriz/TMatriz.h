
//---------------------------------------------------------------------------
#ifndef TMatrizH
#define TMatrizH

//---------------------------------------------------------------------------
#include ".\VTMatriz.h"

//---------------------------------------------------------------------------
class TMatriz : public VTMatriz
   {
   public:
                 __fastcall  TMatriz(int        num1,
                                     int        num2,
                                     int        num3,
                                     VTTempoCPU *tCPU,
                                     AnsiString nome_matriz);
                 __fastcall ~TMatriz(void);

   protected:  // Dados
      bool       matriz_fatorada;
      int        num_dimensoes;    // 1, 2 ou 3
      int        num1, num2, num3; // N�mero de elementos em cada dimens�o
      int        num_elems;        // N�mero total de elementos
      int        tipo;             // Cf. enum_TIPO_MATRIZ em VTMatriz.h
      int        *vet_ordem;       // Vetor que cont�m a ordem de elimina��o das linhas
      AnsiString Nome;             // Nome da matriz
      VTTempoCPU *tCPU;

   protected:  // M�todos
      bool __fastcall EstruturaIdentica(VTMatriz *matB);
      bool __fastcall MatrizQuadrada(void);
      bool __fastcall ReiniciaOrdemLinhas(void);

   private:  // M�todos
      bool __fastcall PM_GetJaFatorada(void);
      int  __fastcall PM_GetNumColunas(void);
      int  __fastcall PM_GetNumDimensoes(void);
      int  __fastcall PM_GetNumElementos(void);
      int  __fastcall PM_GetNumLinhas(void);
      int  __fastcall PM_GetNumPlanos(void);
      int  __fastcall PM_GetTipo(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

