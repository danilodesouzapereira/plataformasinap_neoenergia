//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCPRegulador.h"
#include "TCPCapacitor.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TCPCarga.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRede
//---------------------------------------------------------------------------
//VTCPEqpto* __fastcall NewObjCPEqpto(VTApl *apl, VTEqpto *eqpto)
//   {
//   return(new TCPCarga(apl,eqpto));
//   }
//---------------------------------------------------------------------------
__fastcall TCPRegulador::TCPRegulador(VTApl *apl, VTRegulador *regulador): TCPEqpto(apl)
   {//variaveis locais
   VTBarra *barra1;
   VTBarra *barra2;
   double x = 0.0;
   double y = 0.0;

   this->apl = apl;
   barra1 = regulador->pbarra1;
   barra2 = regulador->pbarra2;
   //inicia dados
   strEQPTO_CP.subscriber = "Sub" + IntToStr(regulador->Id);
   strEQPTO_CP.numero = regulador->Id;
   strEQPTO_CP.userData1 = regulador->Codigo;
   strEQPTO_CP.userData2 = regulador->TipoAsString();
   strEQPTO_CP.userData3 = DoubleToStr("%2.1f", regulador->Spas_mva) +" MVA";
   strEQPTO_CP.userData4 = "";
   strEQPTO_CP.userData5 = "";
   strEQPTO_CP.userData6 = "";
   strEQPTO_CP.userData7 = "";
   strEQPTO_CP.userData8 = "";
   strEQPTO_CP.act = 1;
   strEQPTO_CP.fase = 1;
   strEQPTO_CP.area = 1;
   strEQPTO_CP.flag1 = 0; //consumidor?
   strEQPTO_CP.flag2 = 0; //chave?
   strEQPTO_CP.flag3 = 0; //gerador?
   strEQPTO_CP.flag4 = 1; //regulador
   strEQPTO_CP.flag5 = 0; //capacitor
   strEQPTO_CP.flag6 = 0; //subestacao
   strEQPTO_CP.flag7 = 0; //trafo
   strEQPTO_CP.flag8 = 0; //
   strEQPTO_CP.col = 8;
   //calcula uma coordenada media
   if((barra1 != NULL) && (barra2 != NULL))
      {
      x = (double)(barra1->utm.x + barra2->utm.x)/200;
      y = (double)(barra1->utm.y + barra2->utm.y)/200;
      }
   strEQPTO_CP.coordUTM_x =x;
   strEQPTO_CP.coordUTM_y =y;
   }

//---------------------------------------------------------------------------
__fastcall TCPRegulador::~TCPRegulador(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//AnsiString  __fastcall TCPRegulador::Classe(VTCarga *carga)
//   {
//   }
//---------------------------------------------------------------------------
//eof
