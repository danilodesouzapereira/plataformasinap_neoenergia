//---------------------------------------------------------------------------
#pragma hdrstop
#include "TReduz.h"
#include "TReduzRamal.h"
#include "TReduzSecao.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
//#include "..\Rede\VTRedes.h"
//#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTReduz * __fastcall NewObjReduz(VTApl *apl_owner)
   {
   return(new TReduz(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TReduz::TReduz(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   }

//---------------------------------------------------------------------------
__fastcall TReduz::~TReduz(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TReduz::Executa(void)
   {
   //variáveis locais
   VTGrafico *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //faz a redução dos ramais
   if (! ReduzRamal()) return(false);
   //faz a redução das seções
   if (! ReduzSecao()) return(false);
   //reinicia Grafico
   grafico->AreaRedeDefine();

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduz::ReduzRamal(void)
   {
   //variáveis locais
   bool        sucesso;
   TReduzRamal *reduz_ramal;

   try{//cria objeto TReduzRamal
      if ((reduz_ramal = NewObjReduzRamal(apl)) == NULL) return(false);
      //executa redução
      sucesso = reduz_ramal->Executa();
      //destrói objeto
      delete reduz_ramal;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduz::ReduzSecao(void)
   {
   //variáveis locais
   bool        sucesso;
   TReduzSecao *reduz_secao;

   try{//cria objeto TReduzRamal
      if ((reduz_secao = NewObjReduzSecao(apl)) == NULL) return(false);
      //executa redução
      sucesso = reduz_secao->Executa();
      //destrói objeto
      delete reduz_secao;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof

