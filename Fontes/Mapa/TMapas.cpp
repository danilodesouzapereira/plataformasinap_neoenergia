//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMapas.h"
#include "VTMapa.h"
#include "..\Apl\VTApl.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMapas* __fastcall NewObjMapas(VTApl *apl)
   {
   return(new TMapas(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMapas::TMapas(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria lista
   lisMAPA = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMapas::~TMapas(void)
   {
   //destr�i listas e seus objetos
   if (lisMAPA) {LimpaTList(lisMAPA); delete lisMAPA; lisMAPA = NULL;}
   }

//-----------------------------------------------------------------------------
VTMapa* __fastcall TMapas::CriaMapa(void)
   {
   //vari�veis locais
   VTMapa *mapa;

   //cria nova Mapa e insere na lista
   if ((mapa = NewObjMapa(apl)) != NULL)
      {//define c�digo
      mapa->Codigo = "Regi�o Nova";
      InsereMapa(mapa);
      }
   return(mapa);
   }

//-----------------------------------------------------------------------------
VTMapa* __fastcall TMapas::ExisteMapa(int mapa_id)
   {
   //vari�veis locais
   VTMapa *mapa;

   for (int n = 0; n < lisMAPA->Count; n++)
      {
      mapa = (VTMapa*)lisMAPA->Items[n];
      if (mapa->Id == mapa_id) return(mapa);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTMapa* __fastcall TMapas::ExisteMapa(AnsiString codigo)
   {
   //vari�veis locais
   VTMapa *mapa;

   for (int n = 0; n < lisMAPA->Count; n++)
      {
      mapa = (VTMapa*)lisMAPA->Items[n];
      if (mapa->Codigo.AnsiCompareIC(codigo) == 0) return(mapa);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
void __fastcall TMapas::InsereMapa(VTMapa *mapa)
   {
   //prote��o
   if (mapa == NULL) return;
   if (lisMAPA->IndexOf(mapa) < 0)
      {
      lisMAPA->Add(mapa);
      }
   }

//-----------------------------------------------------------------------------
TList* __fastcall TMapas::LisMapa(void)
   {
   return(lisMAPA);
   }

//-----------------------------------------------------------------------------
void __fastcall TMapas::RetiraMapa(VTMapa *mapa)
   {
   //prote��o
   if (mapa == NULL) return;
   //retira Mapa de lisMAPA
   lisMAPA->Remove(mapa);
   //destr�i objeto
   delete mapa;
   }

//---------------------------------------------------------------------------
//eof

