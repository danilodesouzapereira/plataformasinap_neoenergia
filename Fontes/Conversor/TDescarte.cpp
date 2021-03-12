//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDescarte.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTrecho.h>
 //---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TDescarte::TDescarte(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria objetos
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDescarte::~TDescarte(void)
   {
   //destrói lista sem destrui seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TDescarte::Arranjos(int num_arranjos)
   {
   //variáveis locais
   TList      *lisLIG;
   VTArranjo  *arranjo;
   VTLigacao  *ligacao;
   VTTrecho   *trecho;
   VTRede     *rede;
   VTArranjos *arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTRedes    *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList      *lisARR    = arranjos->LisArranjo();
   TList      *lisREDE   = redes->LisRede();

   //redefine Arranjos importados como novos
   for (int n = num_arranjos; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      arranjo->Status[sttNOVO] = true;
      }
   //inicia lisEQP com os Arranjos típicos usados nos Trechos das Redes
   lisEQP->Clear();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //obtém lista de Ligacoes da Rede
      lisLIG = rede->LisLigacao();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         if (ligacao->Tipo() != eqptoTRECHO) continue;
         trecho = (VTTrecho*)ligacao;
         //verifica se o Trecho utiliza um Arranjo típico
         if (trecho->ArranjoTipico)
            {//inseere Arranjo em lisEQP
            if (lisEQP->IndexOf(trecho->arranjo) < 0) lisEQP->Add(trecho->arranjo);
            }
         }
      }
   //elimina todos Arranjos novos que não estão presentes em lisEQP
   for (int n = lisARR->Count-1; n >= 0; n--)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Arranjo é novo
      if (arranjo->Status[sttNOVO])
         {//verifica se o Arranjo não está na lista lisEQP de Arranjos usados nas redes
         if (lisEQP->IndexOf(arranjo) < 0)
            {//elimina o Arranjo
            lisARR->Delete(n);
            delete arranjo;
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof

