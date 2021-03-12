//---------------------------------------------------------------------------
#ifndef TPadraoH
#define TPadraoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TPadrao : public TObject
   {
   public:
           __fastcall TPadrao(void);
           __fastcall ~TPadrao(void);
      void __fastcall Default(void);

   public:
      struct   {//dados relativos ao fluxo
               int ano_carga;
               int num_pat;  
               }flow;

      struct   {//cor de eqpto isolado
               TColor cor;
               } isolado;
      struct   {//cor e dimens�o de linha p/ Barras
               TColor cor;
               TColor cor_eqbar;
               int    dim;
               } barra;
      struct   {//dimens�o de linha p/ Ligacoes
               int    dim;
               } ligacao, trafo, eqbar;
      struct   {//dimens�o de linha e cor a ser selecionada (da rede ou do tipo da chave)
               int    dim;
               int    op_cor;
			   } chave;
	  struct   {//cor da legenda (da rede ou outra)
			   TColor cor;
			   bool cor_rede;
			   } legenda;
   };

//-----------------------------------------------------------------------------
// fun��o global  p/ criar objeto TPadrao
//-----------------------------------------------------------------------------
TPadrao* NewObjPadrao(void);

#endif
//---------------------------------------------------------------------------
//eof
