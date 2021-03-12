//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
#include "TRemoveTrecho.h"
//---------------------------------------------------------------------------
__fastcall TRemoveTrecho::TRemoveTrecho(VTApl *apl_owner)
   {
   //salva ponteiro do apl
   apl = apl_owner;
   //cria lista
   lisLIGA_DUP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRemoveTrecho::~TRemoveTrecho(void)
   {
   //destroi lista
   if(lisLIGA_DUP) {delete lisLIGA_DUP; lisLIGA_DUP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveTrecho::CopiaComum(TList* lisEQP1, TList* lisEQP2)
   {
   //variaveis locais
   VTLigacao *ligacao;

   for(int nl = 0; nl < lisEQP1->Count; nl++)
      {
      ligacao = (VTLigacao*)lisEQP1->Items[nl];
      if(lisEQP2->IndexOf(ligacao) < 0)  continue;
      //insere ligação comum na lista
      if(lisLIGA_DUP->IndexOf(ligacao) < 0) lisLIGA_DUP->Add(ligacao);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveTrecho::Executa(void)
   {
   //variaveis locais
   VTGrafico *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRede    *rede;
   VTRedes   *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE   = redes->LisRede();
   TList     *lisTRECHO = new TList();

   try{//loop em todas as redes
      for(int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         if(rede->TipoRede->Segmento != redePRI) continue;
         rede->LisEqpto(lisTRECHO, eqptoTRECHO);
         RemoveTrechosDuplicados(lisTRECHO);
         }
      //destroi lista
      delete lisTRECHO;
      //atualiza grafico
      grafico->AreaRedeDefine();
   }catch(Exception &e)
      {//destroi lista
      if(lisTRECHO) delete lisTRECHO;
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveTrecho::RemoveEqptos(void)
   {
   //variaveis locais
   VTLigacao *ligacao;

   for(int nt = lisLIGA_DUP->Count - 1; nt >= 0; nt--)
      {
      ligacao = (VTLigacao*)lisLIGA_DUP->Items[nt];
      if(ligacao->Tipo() != eqptoTRECHO) continue;
      //destroi
      ligacao->rede->RemoveLigacao(ligacao);
      DLL_DeleteEqpto(ligacao);
      }
   //limpa lista
   lisLIGA_DUP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveTrecho::RemoveTrechosDuplicados(TList *lisTRECHO)
   {
   //variaveis locais
   VTTrecho *trecho;
   TList    *lisEQP1  = new TList();
   TList    *lisEQP2  = new TList();
   TList    *lisCOMUM = new TList();
   VTRedes  *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   for(int nt = lisTRECHO->Count - 1; nt >= 0; nt--)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[nt];
      //limpa listas
      lisEQP1->Clear();
      lisEQP2->Clear();
      lisCOMUM->Clear();
      //lista trechos
      redes->LisLigacao(lisEQP1, trecho->pbarra1);
      redes->LisLigacao(lisEQP2, trecho->pbarra2);
      //retira das listas o trecho
      lisEQP1->Remove(trecho);
      lisEQP2->Remove(trecho);
      //verifica se ha itens em comum
//      CopiaComum(lisEQP1, lisEQP2);
      lisCOMUM->Assign(lisEQP1, laAnd, lisEQP2);
      for(int nl = lisCOMUM->Count - 1; nl >= 0; nl--)
         {
         trecho = (VTTrecho*)lisCOMUM->Items[nl];
         lisTRECHO->Remove(trecho);
         }
      CopiaTList(lisCOMUM, lisLIGA_DUP);
      }
   //destroi cada trecho duplicado
   RemoveEqptos();
   //destroi listas
   delete lisEQP1;
   delete lisEQP2;
   delete lisCOMUM;
   }

//---------------------------------------------------------------------------

