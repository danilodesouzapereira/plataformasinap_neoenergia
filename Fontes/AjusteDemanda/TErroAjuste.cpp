//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TErroAjuste.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TErroAjuste::TErroAjuste(void)
   {
   //define valores default para erro único e erro médio
   DefineErroDefault(0.02);
   //inicia valores dos erros
   qtde_erro         = 0;
   somatoria_erro    = 0;
   erro_unico_maximo = 0;
   }

//---------------------------------------------------------------------------
__fastcall TErroAjuste::~TErroAjuste(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TErroAjuste::Aceitavel(void)
   {
   //variáveis locais
   //double erro_medio;

   //proteção: verifica número de erros definidos
   if (qtde_erro == 0) return(true);
   //verifica se o maior erro individual está abaixo do limite
   if (erro_unico_maximo < limite.erro_unico) return(true);
   //verifica se o erro médio está abaixo do limite
   if (qtde_erro > 1) 
      {
      if ((somatoria_erro / qtde_erro) < limite.erro_medio) return(true);
      }

   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TErroAjuste::DefineErroDefault(double erro)
   {
   //inicia erro default
   erro_unico_default = erro;
   erro_medio_default = erro + erro;
   //inicia valores limites admissíveis
   limite.erro_unico = erro_unico_default;
   limite.erro_medio = erro_medio_default;
   }

//---------------------------------------------------------------------------
void __fastcall TErroAjuste::InsereErro(double erro)
   {
   //atualiza somatoria e qtde de erros
   qtde_erro++;
   somatoria_erro += erro;
   //determina maior erro individual
   if (erro > erro_unico_maximo) erro_unico_maximo = erro;
   }

//---------------------------------------------------------------------------
void __fastcall TErroAjuste::Reinicia(int iteracao, int qtde_iteracao)
   {
   //variáveis locais
   double fat_mult;

   //reinicia somatoria, qtde de erros e maior erro individual
   qtde_erro         = 0;
   somatoria_erro    = 0;
   erro_unico_maximo = 0;
   //proteção
   if (qtde_iteracao == 0) return;
   //determina fator para correção dos limites
   if (iteracao > 0) iteracao++;
   fat_mult = 1.0 + (double(iteracao)/double(qtde_iteracao));
   //define limite mais folgado para erro individual a cada iteração
   limite.erro_unico = fat_mult * erro_unico_default;
   //define limite mais folgado para erro médio apenas na último iteração
   if (iteracao >= qtde_iteracao)
      {
      limite.erro_medio = fat_mult * erro_medio_default;
      }
   }

//---------------------------------------------------------------------------
//eof
