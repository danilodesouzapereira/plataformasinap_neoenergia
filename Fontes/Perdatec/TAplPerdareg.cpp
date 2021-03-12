//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplPerdareg.h"
#include "VTBalancos.h"
#include "VTPerdas.h"
#include "VTPerdareg.h"
#include "VTResumos.h"
#include "AjustaEnergia\VTAjustaEnergia.h"
#include "RedePT\VTMontaRedePT.h"
//#include "RedePT\VTRedePT.h"
#include "Medicao\VTMedicoes.h"
#include ".\EnergiaPassante\VTEnergiaPassantes.h"
#include "..\Flow\VTFlow.h"
#include "..\Radial\VTArvore.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
__fastcall TAplPerdareg::TAplPerdareg(TForm *form_owner, VTApl *apl_owner)
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
   Add(NewObjPerdareg(this));
   //Add(NewObjRedePT());
   Add(NewObjMontaRedePT(this));
   Add(NewObjMedicoes(this));
   Add(NewObjAjustaEnergia(this));
   //inicia  dados da Arvore
//   arvore->TipoArvore = arvorePERDATEC;
   arvore->TipoArvore = arvoreINDEFINIDA;
   arvore->Data       = Now();
   }

//---------------------------------------------------------------------------
__fastcall TAplPerdareg::~TAplPerdareg(void)
   {
   //destrói objetos VTresumo armazenados na Arvore
   VTArvore  *arvore = (VTArvore*)GetObject(__classid(VTArvore));

   if (arvore && (arvore->TipoArvore == arvorePERDATEC)) {arvore->ClearObjects();}
   //mais nada a fazer: a classe base TApl destrói todos os objetos criados
   }

//---------------------------------------------------------------------------
//eof
