//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPadrao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// fun��o global
//-----------------------------------------------------------------------------
TPadrao* NewObjPadrao(void)
   {
   return(new TPadrao());
   }

//---------------------------------------------------------------------------
__fastcall TPadrao::TPadrao(void)
   {
   //inicia dados relativo ao fluxo de pot�ncia
   flow.ano_carga = 0;
   flow.num_pat   = 0;
   //inicia padr�o default
   Default();
   }

//---------------------------------------------------------------------------
__fastcall TPadrao::~TPadrao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TPadrao::Default(void)
   {
   //cor de fundo
   //fundo.cor   = clBlack;
   //cor de eqpto isolado
   isolado.cor = clGray; //clDkGray;
   //cor e dimens�o de linha p/ Barras
   barra.cor       = clYellow;
   barra.cor_eqbar = clLime;  //clAqua;
   barra.dim       = 4;
   //dimens�o de linhas das Ligacoes
   ligacao.dim = 1;
   //dimens�o do ret�ngulo p/ Chaves
   chave.dim    = 12;
   chave.op_cor = 1;  //cor da rede
   //dimens�o m�nima do ret�ngulo p/ Trafos //DVK 2014.12.09
   trafo.dim = 40;
   //dimens�o m�nima do ret�ngulo p/ Eqbar
   eqbar.dim = 40;
   //cor da legenda
   legenda.cor = clGray;
   legenda.cor_rede = true;
   }

//---------------------------------------------------------------------------
//eof
