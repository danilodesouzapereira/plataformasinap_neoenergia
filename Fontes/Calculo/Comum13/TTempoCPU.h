//---------------------------------------------------------------------------
#ifndef TTempoCPUH
#define TTempoCPUH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <time.h>
#include ".\VTTempoCPU.h"

//---------------------------------------------------------------------------
class TTempoCPU : public VTTempoCPU
   {
   public:  // Métodos
                      __fastcall  TTempoCPU(void);
                      __fastcall ~TTempoCPU(void);
           void       __fastcall  Acumula(int     indice,
                                          clock_t inicio,
                                          clock_t fim);
           void       __fastcall  AcumulaC(int                    indice,
                                           unsigned long long int num_vezes);
           int        __fastcall  NumCategorias(void);
           void       __fastcall  Reinicia(void);
           AnsiString __fastcall  Valores(int                    indice,
                                          bool                   *flag_tempo,
                                          unsigned long long int *num_chamadas,
                                          double                 *tempo_total_seg,
                                          double                 *tempo_medio_seg);

   private:  // Dados
      struct strTempoCPU
         {
         bool                   flag_tempo;   // Indica se o tempo de processamento foi acumulado ou não
         unsigned long long int num_chamadas; // Na Decomposição QR, este número pode exceder (2^31 - 1) !!
         clock_t                ticks;
         AnsiString             nome_funcao;
         };
      int         num_categorias;
      strTempoCPU *vetor;
   };

#endif
//---------------------------------------------------------------------------

