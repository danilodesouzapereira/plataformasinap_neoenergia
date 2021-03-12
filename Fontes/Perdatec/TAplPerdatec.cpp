//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplPerdatec.h"
#include "VTBalancos.h"
#include "VTPerdas.h"
#include "VTPerdatec.h"
#include "VTResumos.h"
#include "RedePT\VTMontaRedePT.h"
//#include "RedePT\VTRedePT.h"
#include ".\EnergiaPassante\VTEnergiaPassantes.h"
#include "..\Flow\VTFlow.h"
#include "..\Radial\VTArvore.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplPerdatec::TAplPerdatec(TForm *form_owner, VTApl *apl_owner)
                        :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   VTArvore *arvore;

   //cria objetos
   Add(arvore = DLL_NewObjArvore(this));
   Add(DLL_NewObjFlow(this));
   Add(NewObjResumos(this));
   Add(NewObjBalancos(this));
   Add(NewObjEnergiaPassantes(this));
   Add(NewObjPerdas(this));
   Add(NewObjPerdatec(this));
   //2016.01.26: IMPORTANTE
   //Quando Perdatec é usado por Perdareg, Perdareg já criou o objeto MontaRedePT
   if (apl_owner->GetObject(__classid(VTMontaRedePT)) == NULL)
      {//cria objeto MontaRedePT
      Add(NewObjMontaRedePT(this));
      }
   //inicia  dados da Arvore
//   arvore->TipoArvore = arvorePERDATEC;
   arvore->TipoArvore = arvoreINDEFINIDA;
   arvore->Data       = Now();
   }

//---------------------------------------------------------------------------
__fastcall TAplPerdatec::~TAplPerdatec(void)
   {
   //destrói objetos VTresumo armazenados na Arvore
   VTArvore  *arvore = (VTArvore*)GetObject(__classid(VTArvore));

   if (arvore && (arvore->TipoArvore == arvorePERDATEC)) {arvore->ClearObjects();}
   //mais nada a fazer: a classe base TApl destrói todos os objetos criados
   }

//---------------------------------------------------------------------------
//eof
