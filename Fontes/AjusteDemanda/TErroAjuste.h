//---------------------------------------------------------------------------
#ifndef TErroAjusteH
#define TErroAjusteH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TErroAjuste
   {
   public:
           __fastcall  TErroAjuste(void);
           __fastcall ~TErroAjuste(void);
      bool __fastcall  Aceitavel(void);
      void __fastcall  DefineErroDefault(double erro);
      void __fastcall  InsereErro(double erro);
      void __fastcall  Reinicia(int iteracao, int qtde_iteracao);

   private: //dados
      struct{
            double erro_unico, erro_medio;
            }limite;
      double erro_unico_default, erro_medio_default;
      double erro_unico_maximo;
      double somatoria_erro;
      int    qtde_erro;
   };

#endif
//---------------------------------------------------------------------------
//eof

