//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <Fontes\Rede\VTBarra.h>
#include "TDefeitos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDefeitos* __fastcall NewObjDefeitos(void)
   {
   try{
      return(new TDefeitos());
      }catch(Exception &e)
         {
         }
   //erro na cria��o do objeto
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TDefeitos::TDefeitos(void)
   {
   //cria lista
   lisDEF = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDefeitos::~TDefeitos(void)
   {
   //destr�i lista e seus objetos
   if (lisDEF) {LimpaTList(lisDEF); delete lisDEF; lisDEF = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TDefeitos::Clear(void)
   {
   LimpaTList(lisDEF);
   }

//---------------------------------------------------------------------------
TDefeito* __fastcall TDefeitos::ExisteDefeito(VTBarra *barra)
   {
   //vari�veis locais
   TDefeito *defeito;

   for (int n = 0; n < lisDEF->Count; n++)
      {
      defeito = (TDefeito*)lisDEF->Items[n];
      if (defeito->barra == barra) return(defeito);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TDefeito* __fastcall TDefeitos::ExisteDefeito(VTBarra *barra, int tipodefeito, int fases)
   {
   //vari�veis locais
   TDefeito *defeito;

   for (int n = 0; n < lisDEF->Count; n++)
      {
      defeito = (TDefeito*)lisDEF->Items[n];
      if ((defeito->barra       == barra)       &&
          (defeito->tipodefeito == tipodefeito) &&
          (defeito->fases       == fases      )) return(defeito);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TDefeito* __fastcall TDefeitos::FirstDefeito(void)
   {
   //vari�veis locais
   TDefeito *defeito = NULL;

   if (lisDEF->Count > 0)
      {
      defeito = (TDefeito*)lisDEF->First();
      }
   return(defeito);
   }

//---------------------------------------------------------------------------
TDefeito* __fastcall TDefeitos::InsereDefeito(VTBarra *barra, int tipodefeito, int fases)
   {
   //vari�veis locais
   TDefeito *defeito;

   //prote��o: verifica se j� existe o defeito
   if ((defeito = ExisteDefeito(barra, tipodefeito, fases)) != NULL) return(defeito);
   //cria novo objeto
   if ((defeito = new TDefeito()) != NULL)
      {
      defeito->barra       = barra;
      defeito->tipodefeito = tipodefeito;
      defeito->fases       = fases;
      //inserer Defeito em lisDEF
      lisDEF->Add(defeito);
      }
   return(defeito);
   }

//---------------------------------------------------------------------------
TList* __fastcall TDefeitos::LisDefeito(void)
   {
   return(lisDEF);
   }

//---------------------------------------------------------------------------
int __fastcall TDefeitos::NumeroDeDefeitos(void)
   {
   return(lisDEF->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall  TDefeitos::RetiraDefeito(TDefeito *defeito)
   {
   //prote��o
   if (defeito == NULL) return(false);
   //retira Defeito da lista e destr�i objeto
   lisDEF->Remove(defeito);
   delete defeito;

   return(true);
   }

//---------------------------------------------------------------------------
//eof