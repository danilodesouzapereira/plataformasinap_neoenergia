//---------------------------------------------------------------------------
#ifndef TImpressaoH
#define TImpressaoH

//---------------------------------------------------------------------------
#include ".\VTImpressao.h"

class VTGeralC;

//---------------------------------------------------------------------------
class TImpressao : public VTImpressao
   {
   public:  // Métodos
                 __fastcall  TImpressao(VTGeralC *geralC);
                 __fastcall ~TImpressao(void);
      bool       __fastcall  ImprimeResumoProc(int arq_alt=-1);
      bool       __fastcall  ImprimeVetorComplex(AnsiString      nome,
                                                 int             count,
                                                 complex<double> *vetor);
      bool       __fastcall  ImprimeVetorDouble(AnsiString nome,
                                                int        count,
                                                double     *vetor);
      bool       __fastcall  ImprimeVetorInt(AnsiString nome,
                                             int        count,
                                             int        *vetor);
      AnsiString __fastcall  MontaString(int num1,
                                         int num_max1,
                                         int num2,
                                         int num_max2);

   protected:  // Dados
      double          radgrau, raiz3, raiz3_inverso;
      complex<double> cum, czero;
      AnsiString      codigo_barra_ref[4], codigo_metodo_EE[4],
                      codigo_proc_linhas_colunas[4],
                      codigo_proc_linhas_qr[3];
      VTGeralC        *geralC;
      TList           *lisBAR_CODIGO;
      TList           *lisTRECHO_ORD;

   protected:  // Métodos
      void __fastcall ImprimeBrancos(int  num_brancos,
                                     FILE *arq);
      bool __fastcall ImprimeSeparador(int tipo_arquivo,
                                       int num_linhas_branco,
                                       int num_caracteres);

   private:  // Métodos
      int  __fastcall NumDigitos(int num1);
   };

#endif
//---------------------------------------------------------------------------

