//---------------------------------------------------------------------------
#ifndef TEstimadorH
#define TEstimadorH

//---------------------------------------------------------------------------
#include <complex>
using namespace std;
#include ".\VTEstimador.h"

class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TEstimador : public VTEstimador
   {
   public:  // Métodos
                 __fastcall  TEstimador(VTApl *apl);
                 __fastcall ~TEstimador(void);

   protected:  // Métodos
      void __fastcall AtualizaMedidoresReais(TList *lisMED);
      void __fastcall DeletaLista(TList **lista);
      void __fastcall DeletaMatrizes(void);
      bool __fastcall GravaMatriz(VTSisLin   *mat,
                                  AnsiString nome_matriz);
      bool __fastcall GravaVetor(int        num_elems,
                                 double     *vetor,
                                 AnsiString nome_vetor);
      void __fastcall Log(AnsiString txt, bool data=true);
      bool __fastcall RecuperaOrdemColunas(VTSisLin *mat);
      bool __fastcall SalvaOrdemColunas(VTSisLin *mat);

   protected:  // Objetos externos
      VTApl    *apl;
      VTData   *data;
      VTGeralC *geralC;

   protected:  // Dados
      bool            flag_grava_matriz_vetor, flag_montar_Rm1,
                      grava_arq_ordem_col, le_arq_ordem_col;
      double          raiz3;
      complex<double> cum, czero;
      double          fat_mult_med_desab;
      double          desv_tot, desv_tot_ant;  // Desvio total [J(x)]
      double          *Rm1;
      double          *vet_TC;
      double          *vet_desv;
      double          *vet_desv_iter;
      complex<double> *vet_tensoes;
      TList           *lisBAR_ESTADO; // Contém as barras e suas variáveis de estado (objetos TBarraEstado)
      TList           *lisBAR_REF;    // Contém as barras de referência (objetos TBarra)
      TList           *lisYE;         // Contém informações do par barra/fase e das variáveis de estado (objetos TYnodalE)
      VTSisLin        *A;
      VTSisLin        *H;
      VTSisLin        *HtRm1;

   private:  // Métodos
      bool __fastcall GravaArquivoOrdemColunas(void);
      bool __fastcall LeArquivoOrdemColunas(void);
      void __fastcall LiberaMemoria(void);

   private:  // Dados
      bool gravou_arq_ordem_col, leu_arq_ordem_col;
      int  num_col_arq;
      int  *vet_col_arq;
      int  num_col_ordenadas;
      int  *vet_col_ordenadas; // Para salvar ordem de processamento das colunas

   };

#endif
//---------------------------------------------------------------------------
//eof
