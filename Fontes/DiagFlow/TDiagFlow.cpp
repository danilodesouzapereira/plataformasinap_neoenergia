//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDiagFlow.h"
#include "TErro.h"
//#include "..\Rede\VTBarra.h"
//#include "..\Rede\VTCapacitor.h"
//#include "..\Rede\VTCapserie.h"
//#include "..\Rede\VTCarga.h"
//#include "..\Rede\VTChave.h"
//#include "..\Rede\VTEqpto.h"
//#include "..\Rede\VTFiltro.h"
//#include "..\Rede\VTGerador.h"
//#include "..\Rede\VTLigacao.h"
//#include "..\Rede\VTMutua.h"
//#include "..\Rede\VTReator.h"
//#include "..\Rede\VTRedes.h"
//#include "..\Rede\VTRegulador.h"
//#include "..\Rede\VTSuprimento.h"
//#include "..\Rede\VTTrafo.h"
//#include "..\Rede\VTTrafo3e.h"
//#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTDiagFlow* __fastcall NewObjDiagFlow(VTApl *apl)
   {
   try{
      return(new TDiagFlow(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TDiagFlow::TDiagFlow(VTApl *apl)
   {
   //salva objetos
   this->apl = apl;
   //cria objetos
   lisERRO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDiagFlow::~TDiagFlow(void)
   {
   //destrói lista e seus objetos
   if(lisERRO) {LimpaTList(lisERRO); delete lisERRO; lisERRO = NULL;}
   }

//---------------------------------------------------------------------------
TList* __fastcall TDiagFlow::Executa(void)
   {
   return(lisERRO);
   }

//---------------------------------------------------------------------------
//eof




