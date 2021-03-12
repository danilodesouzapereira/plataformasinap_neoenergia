//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSegmentoJSON.h"
#include "VTCrescimentoJSON.h"
#include "..\Classes\VTClasse.h"
//#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSegmentoJSON* __fastcall NewObjSegmentoJSON(void)
   {
   try{
      return(new TSegmentoJSON());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TSegmentoJSON::TSegmentoJSON(void)
   {
   try{//cria objetos
		lisCRES_JSON = new TList();
		//inicia dados
        ExternId = "";
		Codigo  = "";
		Tipo    = "";
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TSegmentoJSON::~TSegmentoJSON(void)
   {
   //destrói lista e seus objetos
   if (lisCRES_JSON) {LimpaTList(lisCRES_JSON); delete lisCRES_JSON; lisCRES_JSON = NULL;}
   }

////---------------------------------------------------------------------------
//VTCrescimento* __fastcall TSegmentoJSON::ExisteCrescimento(VTClasse *classe)
//   {
//   //variáveis locais
//   VTCrescimento *crescimento;
//
//   //verifica se existe um Crescimento p/ a Classe
//   for (int n = 0; n < lisCRES->Count; n++)
//	  {
//	  crescimento = (VTCrescimento*)lisCRES->Items[n];
//	  if (crescimento->Classe == classe) return(crescimento);
//	  }
//   return(NULL);
//   }

////---------------------------------------------------------------------------
//void __fastcall TSegmentoJSON::IniciaCrescimentoNulo(void)
//   {
//   //variáveis locais
//   VTCrescimento *crescimento;
//
//   //verifica se existe um Crescimento p/ a Classe
//   for (int n = 0; n < lisCRES->Count; n++)
//	  {
//	  crescimento = (VTCrescimento*)lisCRES->Items[n];
//	  //loop p/ todos os anos
//	  for (int ano = crescimento->AnoInicial; ano <= crescimento->AnoFinal; ano++)
//		 {
//		 crescimento->Taxa_pu[ano] = 0.;
//		 }
//	  }
//   }

//---------------------------------------------------------------------------
bool __fastcall TSegmentoJSON::InsereCrescimento(VTCrescimentoJSON *crescimentoJSON)
   {
   //proteção
   if (crescimentoJSON == NULL) return(false);
   //insere Crescimento na lista
   if (lisCRES_JSON->IndexOf(crescimentoJSON) < 0)
	{
		lisCRES_JSON->Add(crescimentoJSON);
	}
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TSegmentoJSON::PM_GetListCrescimento(void)
   {
   return(lisCRES_JSON);
   }


//---------------------------------------------------------------------------
AnsiString __fastcall TSegmentoJSON::PM_GetCodigo(void)
   {
   return(PD.codigo);
   }


//---------------------------------------------------------------------------
AnsiString  __fastcall TSegmentoJSON::PM_GetExternId(void)
   {
   return(PD.extern_id);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TSegmentoJSON::PM_GetTipo(void)
	{
	return(PD.tipo);
	}

//---------------------------------------------------------------------------
void __fastcall TSegmentoJSON::PM_SetExternId(AnsiString extern_id)
	{
	PD.extern_id = extern_id;
	}

//---------------------------------------------------------------------------
void __fastcall TSegmentoJSON::PM_SetCodigo(AnsiString codigo)
	{
	PD.codigo = codigo;
	}

//---------------------------------------------------------------------------
void __fastcall TSegmentoJSON::PM_SetTipo(AnsiString tipo)
	{
	PD.tipo = tipo;
	}


//---------------------------------------------------------------------------
//eof
