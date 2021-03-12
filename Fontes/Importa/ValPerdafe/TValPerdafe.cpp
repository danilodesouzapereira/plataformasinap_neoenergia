//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Filectrl.hpp>
#include "TValPerdafe.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
VTValPerdafe* __fastcall NewObjValPerdafe(VTApl *apl)
   {
   return(new TValPerdafe(apl));
   }

//---------------------------------------------------------------------------
__fastcall TValPerdafe::TValPerdafe(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   //cria objetos
   lisEQP = new TList();
   lisTRF = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TValPerdafe::~TValPerdafe(void)
   {
   //destró lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP;  lisEQP = NULL;}
   if (lisTRF)  {delete lisTRF;  lisTRF = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TValPerdafe::Executa(void)
   {
   //variáveis locais
   VTTrafo     *trafo;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   //exibe progresso
   if (progresso) progresso->Add("Verificando perda no ferro dos transformadores");
   //reinicia lisEQP
	lisTRF->Clear();
	//obtém lista de Trafos da rede
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoTRAFO);
   //loop p/ todos Trafos
	for (int n = 0; n < lisEQP->Count; n++)
      {
		trafo = (VTTrafo*)lisEQP->Items[n];
		if (IsDoubleZero(trafo->perda_ferro))
			{//insere Trafo em lisTRF;
			lisTRF->Add(trafo);
			//redefine perda no ferro
			trafo->perda_ferro = 0.1;
         //atualiza Status
         trafo->Status[sttERROGIS] = true;
			}
		}
   return(true);
   }

//---------------------------------------------------------------------------
int  __fastcall TValPerdafe::ExisteTrafoComPerdaFeInvalida(void)
   {
	return(lisTRF->Count);
   }

//---------------------------------------------------------------------------
TList* __fastcall TValPerdafe::LisTrafoComPerdaFeInvalida(void)
   {
   return(lisTRF);
   }

//-----------------------------------------------------------------------------
// eof
