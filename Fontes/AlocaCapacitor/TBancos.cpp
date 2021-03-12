//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBancos.h"
#include "VTBanco.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTBancos* __fastcall NewObjBancos(VTApl *apl)
   {
   return(new TBancos(apl));
   }

//---------------------------------------------------------------------------
__fastcall TBancos::TBancos(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria lista
   lisBANCO  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBancos::~TBancos(void)
   {
   //destrói listas e seus objetos
   if (lisBANCO)  {LimpaTList(lisBANCO); delete lisBANCO;  lisBANCO  = NULL;}
	}

//---------------------------------------------------------------------------
VTBanco*  __fastcall TBancos::ExisteBanco(VTBarra *barra)
   {
   //variáveis locais
   VTBanco *banco;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      if (banco->Barra == barra) return(banco);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTBanco*  __fastcall TBancos::ExisteBanco(VTCapacitor *capacitor)
   {
   //variáveis locais
   VTBanco     *banco;
   VTCapacitor *cap;
   TList       *lisCAP;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      lisCAP = banco->LisCapacitor();
      for (int nc = 0; nc < lisCAP->Count; nc++)
         {
         cap = (VTCapacitor*) lisCAP->Items[nc];
         if (cap == capacitor) {return banco;}
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTBanco*  __fastcall TBancos::InsereBanco(VTBarra *barra)
   {
   //variáveis locais
   VTBanco *banco;

   //proteção
   if (barra == NULL) return(NULL);
   //verifica se já exisnte um Banco associado à Barra
   if ((banco = ExisteBanco(barra)) != NULL) return(banco);
   //cria um novo Banco
   if ((banco = NewObjBanco(apl)) != NULL)
      {
      banco->Barra = barra;
      lisBANCO->Add(banco);
      }
   return(banco);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBancos::LisBanco(void)
   {
   return(lisBANCO);
   }

//---------------------------------------------------------------------------
int __fastcall TBancos:: LisCapacitor(TList *lisEXT)
   {
   //variáveis locais
   VTBanco *banco;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      //copia lista de Capacitores do Banco
      CopiaTList(banco->LisCapacitor(), lisEXT);
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TBancos:: LisCloneCapacitor(TList *lisEXT)
   {
   //variáveis locais
   TList       *lisCAP;
   VTBanco     *banco;
   VTCapacitor *capacitor;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      lisCAP = banco->LisCapacitor();
      for (int nc = 0; nc < lisCAP->Count; nc++)
         {
         capacitor = (VTCapacitor*)lisCAP->Items[nc];
         lisEXT->Add(capacitor->Clone());
         }
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TBancos::NumBanco(void)
   {
   return(lisBANCO->Count);
   }

//---------------------------------------------------------------------------
double __fastcall TBancos::Qtotal_mvar(void)
   {
   //variáveis locais
   double  q_mvar = 0.;
   VTBanco *banco;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco   = (VTBanco*)lisBANCO->Items[n];
      q_mvar += banco->Qtotal_mvar();
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
void __fastcall TBancos::Reinicia(void)
   {
   //destrói todos objetos Banco em lisBANCO
   LimpaTList(lisBANCO);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall  TBancos::RetiraCapacitor(VTCapacitor *capacitor)
	{//Variaveis locais
	VTBanco     *banco;
	VTCapacitor *cap;
	TList      *lisCAP;

	if ((banco = ExisteBanco(capacitor)) != NULL)
		{
			lisCAP = banco->LisCapacitor();
			for(int nc = 0; nc < lisCAP->Count ; nc++)
			{
				cap = (VTCapacitor*) lisCAP->Items[nc];
				if(capacitor == cap)
				{
					lisCAP->Delete(nc);
					return true;
				}
			}
		}
	else{return false;}
	}
*/ //DVK 2015.09.25 reescrito
//---------------------------------------------------------------------------
bool __fastcall  TBancos::RetiraCapacitor(VTCapacitor *capacitor)
	{//Variaveis locais
	VTBanco     *banco;
	VTCapacitor *cap;
	TList       *lisCAP;
	bool sucesso = false;


	//localiza banco que contenha o capacitor em sua lista
	if ((banco = ExisteBanco(capacitor)) != NULL)
		{
		lisCAP = banco->LisCapacitor();
		for(int nc = lisCAP->Count - 1; nc >= 0; nc--)
			{
			cap = (VTCapacitor*) lisCAP->Items[nc];
			if(capacitor == cap)
				{//remove capacitor da lista
				lisCAP->Delete(nc);
//				return true;
				sucesso = true;
				break;
				}
			}
		}
	else
	{
//		return false;
		sucesso = false;
	}
    return sucesso;
	}

//---------------------------------------------------------------------------
//eof
