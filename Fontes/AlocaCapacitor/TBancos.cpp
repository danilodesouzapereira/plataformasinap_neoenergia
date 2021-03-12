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
//fun��o global p/ criar objeto da classe
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
   //destr�i listas e seus objetos
   if (lisBANCO)  {LimpaTList(lisBANCO); delete lisBANCO;  lisBANCO  = NULL;}
	}

//---------------------------------------------------------------------------
VTBanco*  __fastcall TBancos::ExisteBanco(VTBarra *barra)
   {
   //vari�veis locais
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
   //vari�veis locais
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
   //vari�veis locais
   VTBanco *banco;

   //prote��o
   if (barra == NULL) return(NULL);
   //verifica se j� exisnte um Banco associado � Barra
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
   //vari�veis locais
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
   //vari�veis locais
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
   //vari�veis locais
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
   //destr�i todos objetos Banco em lisBANCO
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
