//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasores.h"
#include "VTFasor.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasores* __fastcall NewObjFasores(void)
   {
   try{//cria objeto TFasores
      return(new TFasores());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
//---------------------------------------------------------------------------
__fastcall TFasores::TFasores(void)
   {
   try{//cria lista de objetos TFasor
      lisFASOR = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasores::~TFasores(void)
   {
   //destrói lista e seus objetos
   if (lisFASOR) {LimpaTList(lisFASOR); delete lisFASOR; lisFASOR = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::DefinePatamar(int ind_pat)
   {
   //variáveis locais
   VTFasor *fasor;

   //define patamar p/ todos TFasor
   for (int n = 0; n < lisFASOR->Count; n++)
      {
      fasor = (VTFasor*)lisFASOR->Items[n];
      fasor->DefinePatamar(ind_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::IniciaVetorCores(TColor *vet_color, int dim_vet)
   {
   //protecção
   if (dim_vet > 6) dim_vet = 6;
   switch(dim_vet)
      {
      case 6: vet_color[5] = clBlack;
      case 5: vet_color[4] = clOlive;
      case 4: vet_color[3] = clSkyBlue;
      case 3: vet_color[2] = clGreen;
      case 2: vet_color[1] = clRed;
      case 1: vet_color[0] = clNavy;
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFasores::Insere(VTEqpto *eqpto)
   {
   //variáveis locais
   bool      visible = true;
   bool      hide    = false;
   VTFasor   *fasor;
   VTBarra   *barra;
   VTLigacao *ligacao;

   //verifica tipo do Eqpto
   switch(eqpto->Tipo())
      {
      case eqptoBARRA:  //fasor p/ Vfase e Vlinha
         lisFASOR->Add(NewObjFasor(eqpto, clBlue, bar_VFN_KV));
         lisFASOR->Add(NewObjFasor(eqpto, clRed,  bar_VFF_KV));
         break;
      case eqptoCHAVE:  //fasor p/ Ifase
         lisFASOR->Add(NewObjFasor(eqpto, clGreen, lig_I_ABC, 1));
         break;
      case eqptoTRECHO:  //fasor p/ Ifase
         lisFASOR->Add(NewObjFasor(eqpto, clGreen,   lig_I_ABC, 0, visible));
         lisFASOR->Add(NewObjFasor(eqpto, clOlive,   lig_I_ABC, 1, hide));
         lisFASOR->Add(NewObjFasor(eqpto, clNavy,    lig_S,     0, visible));
         lisFASOR->Add(NewObjFasor(eqpto, clSkyBlue, lig_S,     1, hide));
         break;
      case eqptoTRAFO:  //fasor p/ Ifase
         lisFASOR->Add(NewObjFasor(eqpto, clGreen,   lig_I_ABC, 0, visible));
         lisFASOR->Add(NewObjFasor(eqpto, clOlive,   lig_I_ABC, 1, visible));
         lisFASOR->Add(NewObjFasor(eqpto, clNavy,    lig_S,     0, hide));
         lisFASOR->Add(NewObjFasor(eqpto, clSkyBlue, lig_S,     1, hide));
         //isere fasores p/ tensão das barras
         ligacao = (VTLigacao*)eqpto;
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            lisFASOR->Add(NewObjFasor(barra, clBlue, bar_VFN_KV, ind_bar, hide));
            lisFASOR->Add(NewObjFasor(barra, clRed,  bar_VFF_KV, ind_bar, hide));
            }
         break;
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFasores::Insere(VTBarra *barra)
   {
   lisFASOR->Add(NewObjFasor(barra, clBlue, bar_VFN_KV));
   lisFASOR->Add(NewObjFasor(barra, clRed,  bar_VFF_KV));
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::Insere(VTBarCC *barCC)
   {
   //variáveis locais
   bool   visible;
   TColor color[NUM_MAX_DEF];

   //inicia vetor de cores
   IniciaVetorCores(color, sizeof(color)/sizeof(TColor));
   //insere um conjunto de valores p/ cada tipo de defeito
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      visible = (ind_def == defeitoFT);
      lisFASOR->Add(NewObjFasor(barCC, color[ind_def], ind_def, 0, visible));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::Insere(VTLigacao *ligacao)
   {
   //variáveis locais
   bool    visible = true;
   bool    hide    = false;
   TColor  color[4];
   VTBarra *barra;

   //inicia vetor de cores
   IniciaVetorCores(color, sizeof(color)/sizeof(TColor));
   switch(ligacao->Tipo())
      {
      case eqptoCHAVE:  //fasor p/ Ifase
         lisFASOR->Add(NewObjFasor(ligacao, clGreen, lig_I_ABC, 1));
         break;
      case eqptoTRECHO:  //fasor p/ Ifase
         lisFASOR->Add(NewObjFasor(ligacao, color[0], lig_I_ABC, 0, visible));
         lisFASOR->Add(NewObjFasor(ligacao, color[1], lig_I_ABC, 1, hide   ));
         lisFASOR->Add(NewObjFasor(ligacao, color[2], lig_S,     0, visible));
         lisFASOR->Add(NewObjFasor(ligacao, color[3], lig_S,     1, hide   ));
         break;
      case eqptoTRAFO:  //fasor p/ Ifase
         lisFASOR->Add(NewObjFasor(ligacao, color[0], lig_I_ABC, 0, visible));
         lisFASOR->Add(NewObjFasor(ligacao, color[1], lig_I_ABC, 1, hide   ));
         lisFASOR->Add(NewObjFasor(ligacao, color[2], lig_S,     0, visible));
         lisFASOR->Add(NewObjFasor(ligacao, color[3], lig_S,     1, hide   ));
         /*
         //insere fasores p/ tensão das barras
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            lisFASOR->Add(NewObjFasor(barra, clBlue, bar_VFN_KV, ind_bar, hide));
            lisFASOR->Add(NewObjFasor(barra, clRed,  bar_VFF_KV, ind_bar, hide));
            }
         */
         break;
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFasores::Insere(VTLigCC *ligCC)
   {
   //variáveis locais
   bool visible;
   TColor color[NUM_MAX_DEF];

   //inicia vetor de cores
   IniciaVetorCores(color, sizeof(color)/sizeof(TColor));

   switch(ligCC->Ligacao->Tipo())
      {
      case eqptoCHAVE:   //fasor p/ Ifase
      case eqptoTRECHO:  //fasor p/ Ifase
         //insere um Fasor p/ cada tipo de defeito
         for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
            {
            visible = (ind_def == defeitoFT);
            lisFASOR->Add(NewObjFasor(ligCC, color[ind_def], ind_def, 0, visible));
            }
         break;
      case eqptoTRAFO:  //fasor p/ Ifase
         //insere um conjunto de Fasores p/ cada fase
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {//proteção
            if (ligCC->Ligacao->Barra(ind_bar) == NULL) continue;
            for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
               {
               visible = (ind_def == defeitoFT);
               lisFASOR->Add(NewObjFasor(ligCC, color[ind_def], ind_def, ind_bar, visible));
               }
            }
         break;
      }
   }
//---------------------------------------------------------------------------
TList* __fastcall TFasores::LisFasor(void)
   {
   return(lisFASOR);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::PM_GetTipoCurto(void)
   {
   //proteção
   if (lisFASOR->Count == 0) return(false);
   return(((VTFasor*)lisFASOR->First())->TipoCurto);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasores::PM_GetTipoFluxo(void)
   {
   //proteção
   if (lisFASOR->Count == 0) return(false);
   return(((VTFasor*)lisFASOR->First())->TipoFluxo);
   }

//---------------------------------------------------------------------------
void __fastcall TFasores::Reinicia(void)
   {
   //destrói todos Fasor existentes
   LimpaTList(lisFASOR);
   }

//---------------------------------------------------------------------------
//eof
