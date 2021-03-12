//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
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
__fastcall TCPCarga::TCPCarga(VTApl *apl, VTCarga *carga): TCPEqpto(apl)
   {//variaveis locais
   VTBarra *barra;

   barra = carga->pbarra;
   //inicia dados
   strEQPTO_CP.subscriber = "Sub" + IntToStr(carga->Id);
   strEQPTO_CP.numero = carga->Id;
   strEQPTO_CP.userData1 = carga->Codigo;
   strEQPTO_CP.userData2 = carga->TipoAsString();
   strEQPTO_CP.userData3 = Classe(carga);
   strEQPTO_CP.userData4 = "";
   strEQPTO_CP.userData5 = "";
   strEQPTO_CP.userData6 = "";
   strEQPTO_CP.userData7 = "";
   strEQPTO_CP.userData8 = "";
   strEQPTO_CP.act = 1;
   strEQPTO_CP.fase = 1;
   strEQPTO_CP.area = 1;
   strEQPTO_CP.flag1 = 1;  //consumidor?
   strEQPTO_CP.flag2 = 0;  //chave?
   strEQPTO_CP.flag3 = 0;  //gerador?
   strEQPTO_CP.flag4 = 0;  //regulador
   strEQPTO_CP.flag5 = 0;  //capacitor
   strEQPTO_CP.flag6 = 0;  //subestacao
   strEQPTO_CP.flag7 = 0;  //trafo
   strEQPTO_CP.flag8 = 0;  //
   strEQPTO_CP.col = 8;
   strEQPTO_CP.coordUTM_x = barra->utm.x/100;
   strEQPTO_CP.coordUTM_y = barra->utm.y/100;
   }

//---------------------------------------------------------------------------
__fastcall TCPCarga::~TCPCarga(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TCPCarga::Classe(VTCarga *carga)
   {//variaveis locais
   AnsiString classe = "";

   if(carga->NumConsTotal != 1)
      {
      classe = "Diversas";
      }
   else
      {
      classe = carga->classe->Codigo;
//      if (carga->NumConsResidencial >= 0)
//         {
//         classe = "Residencial";
//         }
//      else if (carga->NumConsComercial >= 0)
//         {
//         classe = "Comercial";
//         }
//      else if (carga->NumConsIndustrial >= 0)
//         {
//         classe = "Industrial";
//         }
//      else if (carga->NumConsRural >= 0)
//         {
//         classe = "Rural";
//         }
//      else if (carga->NumConsOutros >= 0)
//         {
//         classe = "Outros";
//         }
//      else if (carga->NumConsA4 >= 0)
//         {
//         classe = "Grupo A";
//         }
      }
   return (classe);
   }
//---------------------------------------------------------------------------
//eof
