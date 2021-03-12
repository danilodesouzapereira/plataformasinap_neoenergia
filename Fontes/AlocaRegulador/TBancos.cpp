//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBancos.h"
#include "VTBanco.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRegulador.h"
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
   lisBancoRede = new TList();   //DVK 2014.04.03
   }

//---------------------------------------------------------------------------
__fastcall TBancos::~TBancos(void)
   {
   //destrói listas e seus objetos
   if (lisBancoRede)  {delete lisBancoRede;  lisBancoRede  = NULL;}
   //DVK 2014.04.03
   if (lisBANCO)  {LimpaTList(lisBANCO); delete lisBANCO;  lisBANCO  = NULL;}
	}

//---------------------------------------------------------------------------
VTBanco*  __fastcall TBancos:: ExisteBanco(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBanco *banco;

   //proteção
   if(ligacao == NULL) return(NULL);
   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      if (banco->LigacaoPai == ligacao) return(banco);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTBanco*  __fastcall TBancos::InsereBanco(void)
   {
   //variáveis locais
   VTBanco *banco;

   //verifica se já exisnte um Banco associado à Barra
   //if ((banco = ExisteBanco(barra)) != NULL) return(banco);
   //cria um novo Banco
   if ((banco = NewObjBanco(apl)) != NULL)
      {
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
TList* __fastcall TBancos::LisBanco(VTRede *rede)
   {//DVK 2014.04.03
   VTBanco  *banco;

   lisBancoRede->Clear();
   for(int nb = 0; nb < lisBANCO->Count; nb++)
      {
      banco = (VTBanco*)lisBANCO->Items[nb];
      if(banco->Regulador->rede == rede) lisBancoRede->Add(banco);
      }
   return(lisBancoRede);
   }

//---------------------------------------------------------------------------
int __fastcall TBancos:: LisCloneRegulador(TList *lisEXT)
   {
   //variáveis locais
   VTBanco     *banco;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      lisEXT->Add(banco->Regulador->Clone());
      }
   return(lisEXT->Count);
   }
   
//---------------------------------------------------------------------------
int __fastcall TBancos:: LisRegulador(TList *lisEXT)
   {
   //variáveis locais
   VTBanco *banco;

   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      //insere Regulador do Banco
      if (lisEXT->IndexOf(banco->Regulador) < 0)
         {
         lisEXT->Add(banco->Regulador);
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
void __fastcall TBancos::Reinicia(void)
   {
   //destrói todos objetos Banco em lisBANCO
   LimpaTList(lisBANCO);
   }

//---------------------------------------------------------------------------
//eof
