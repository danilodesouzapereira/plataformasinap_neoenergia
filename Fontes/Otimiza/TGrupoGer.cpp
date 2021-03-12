//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTGerador.h>
#include <PlataformaSinap\Fontes\Rede\VTSuprimento.h>
#include "TGrupoGer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoGer::TGrupoGer(VTEqpto* eqpto)
   {
   lisGer = new TList();
   lisBar = new TList();
   lisChv = new TList();
   pgrupo_aux = NULL;
   flag_conec  = false;
   flag_carga = (eqpto->Tipo() == eqptoCARGA);
   itipo = 0;
   itipoVar = -1;
	AddEqpto(eqpto);
   }
//---------------------------------------------------------------------------
__fastcall TGrupoGer::~TGrupoGer(void)
   {
   if(lisGer) {delete lisGer; lisGer = NULL;}
   if(lisBar) {delete lisBar; lisBar = NULL;}
   if(lisChv) {delete lisChv; lisChv = NULL;}
   }
//---------------------------------------------------------------------------
void __fastcall TGrupoGer::AddEqpto(VTEqpto* eqpto)
   {
   if(flag_carga)
     if(eqpto->Tipo() != eqptoCARGA) return;

   if(lisGer->IndexOf(eqpto) < 0) lisGer->Add(eqpto);
   }
//---------------------------------------------------------------------------
AnsiString __fastcall TGrupoGer::Codigo(void)
   {
   VTEqbar* eqbar;
   AnsiString aux;
   return(nome);
   }
//---------------------------------------------------------------------------
VTEqpto* __fastcall TGrupoGer::Eqpto(int neq)
   {
   //protecao
   if(!(neq < lisGer->Count)) return(NULL);

   return((VTEqbar*)lisGer->Items[neq]);
   }
//---------------------------------------------------------------------------
TList* __fastcall TGrupoGer::LisChv(void)
   {
   return(lisChv);
   }
//---------------------------------------------------------------------------
TList* __fastcall TGrupoGer::LisEqpto(void)
   {
   return(lisGer);
   }
//---------------------------------------------------------------------------
int __fastcall TGrupoGer::NEq(void)
   {
   return(lisGer->Count);
   }
//---------------------------------------------------------------------------
void __fastcall TGrupoGer::RemEqpto(VTEqpto* eqpto)
   {
   if(!(lisGer->IndexOf(eqpto) < 0)) lisGer->Remove(eqpto);
   }
//---------------------------------------------------------------------------
bool __fastcall TGrupoGer::VerEqpto(VTEqpto* eqpto)
   {
   if(lisGer->IndexOf(eqpto) < 0) return(false);
   else
     return(true);
   }
//---------------------------------------------------------------------------
double __fastcall TGrupoGer::Snom(int iger)
   {
   VTEqbar* eqbar;
   double snom = 0;
   int itipo;

   //Retorna a potencia nominal de conjunto de geradores ou suprimentos
   itipo = Tipo();
   if(iger < 0)//potencia nominal total
     {
     for(int ng = 0; ng < lisGer->Count; ng++)
        {
        eqbar  = (VTEqbar*)lisGer->Items[ng];
        snom += (itipo == eqptoGERADOR) ? ((VTGerador*)eqbar)->snom : ((VTSuprimento*)eqbar)->smax;
        }
     }
   else
     {
     //Protecao
     if(!(iger < lisGer->Count)) return(-1.);
     eqbar  = (VTEqbar*)lisGer->Items[iger];
     snom   = (itipo == eqptoGERADOR) ? ((VTGerador*)eqbar)->snom : ((VTSuprimento*)eqbar)->smax;
     }
   return(snom);
   }
//---------------------------------------------------------------------------
int __fastcall TGrupoGer::Tipo(void)
   {
   VTEqbar* eqbar;
   if(lisGer->Count == 0) return(-1);
   eqbar = (VTEqbar*)lisGer->First();
   return(eqbar->Tipo());
   }
//---------------------------------------------------------------------------
int __fastcall TGrupoGer::TipoSup(void)
   {
   int      itipo, isup;
   VTEqbar* eqbar;
   if(lisGer->Count == 0) return(-1);
   eqbar = (VTEqbar*)lisGer->First();
   itipo = eqbar->Tipo();
	if(itipo == eqptoCARGA) return(0);
	if((itipo != eqptoGERADOR)&&(itipo != eqptoSUPRIMENTO)) return(-1);
	//tipo do suprimento{supPV, supPQ, supVF}
   isup = (itipo == eqptoGERADOR) ? ((VTGerador*)eqbar)->tiposup : ((VTSuprimento*)eqbar)->tiposup;
   return(isup);
   }
//---------------------------------------------------------------------------
bool __fastcall TGrupoGer::VerBarra(VTBarra* pbarra)
   {
   for(int n = 0; n < lisGer->Count; n++)
      {
      if(((VTEqbar*)(Eqpto(n)))->pbarra == pbarra) return(true);
      }
   return(false);
   }
//---------------------------------------------------------------------------
