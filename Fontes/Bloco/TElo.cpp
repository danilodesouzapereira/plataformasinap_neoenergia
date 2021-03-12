//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TElo.h"
#include "VTBloco.h"
//#include "..\DLL_Stub\Funcao\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTElo* __fastcall NewObjElo(void)
   {
   return(new TElo());
   }

//---------------------------------------------------------------------------
__fastcall TElo::TElo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TElo::~TElo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TElo::ConectaBloco(VTBloco *bloco)
   {
   return((bloco == Bloco1)||(bloco == Bloco2));
   }

//---------------------------------------------------------------------------
bool __fastcall TElo::ConectaRede(VTRede *rede)
   {
   //verifica um dos Blocos conecta a Rede
   if (Bloco1->ExisteLigacaoDaRede(rede)) return(true);
   if (Bloco2->ExisteLigacaoDaRede(rede)) return(true);
   //não conecta a Rede
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TElo::ConectaRede1Rede2(VTRede *rede1, VTRede *rede2)
   {
   //verifica se o primeiro Bloco conecta a primeira Rede e o segundo Bloco a segunda Rede
   if (Bloco1->ExisteLigacaoDaRede(rede1)&&Bloco2->ExisteLigacaoDaRede(rede2)) return(true);
   //verifica se o primeiro Bloco conecta a segunda Rede e o segundo Bloco a primeira Rede
   if (Bloco1->ExisteLigacaoDaRede(rede2)&&Bloco2->ExisteLigacaoDaRede(rede1)) return(true);
   //não conecta as Redes
   return(false);
   }

//---------------------------------------------------------------------------
VTBloco* __fastcall TElo::PM_GetBloco1(void)
   {
   return(PD.bloco1);
   }

//---------------------------------------------------------------------------
VTBloco* __fastcall TElo::PM_GetBloco2(void)
   {
   return(PD.bloco2);
   }

//---------------------------------------------------------------------------
VTChave* __fastcall TElo::PM_GetChave(void)
   {
   return(PD.chave);
   }

//---------------------------------------------------------------------------
void __fastcall TElo::PM_SetBloco1(VTBloco *bloco)
   {
   PD.bloco1 = bloco;
   }

//---------------------------------------------------------------------------
void __fastcall TElo::PM_SetBloco2(VTBloco *bloco)
   {
   PD.bloco2 = bloco;
   }

//---------------------------------------------------------------------------
void __fastcall TElo::PM_SetChave(VTChave *chave)
   {
   PD.chave = chave;
   }

//---------------------------------------------------------------------------
//eof
