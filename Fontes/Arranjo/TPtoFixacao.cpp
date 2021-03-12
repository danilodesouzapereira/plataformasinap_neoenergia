//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPtoFixacao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPtoFixacao
//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall NewObjPtoFixacao(void)
   {
   return(new TPtoFixacao());
   }
   
//---------------------------------------------------------------------------
__fastcall TPtoFixacao::TPtoFixacao(void)
   {
   id      = -1;
   codigo  = "";
   coord.x = 0;
   coord.y = 0;
   }

//---------------------------------------------------------------------------
__fastcall TPtoFixacao::~TPtoFixacao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall TPtoFixacao::Clone(void)
   {
   //variáveis locais
   VTPtoFixacao *pto_fixacao;

   //cria um novo PtoFixacao
   if ((pto_fixacao = NewObjPtoFixacao()) != NULL)
      {//copia dados deste PtoFixacao p/ o novo PtoFixacao
		pto_fixacao->CopiaAtributosDe(*this);
      }
   return(pto_fixacao);
   }

//---------------------------------------------------------------------------
void __fastcall TPtoFixacao::CopiaAtributosDe(VTPtoFixacao &ref)
	{
   //copia atributos do PtoFixacao
	id     = ref.id;
	codigo = ref.codigo;
	coord  = ref.coord;
	}
   
//---------------------------------------------------------------------------
//eof

 