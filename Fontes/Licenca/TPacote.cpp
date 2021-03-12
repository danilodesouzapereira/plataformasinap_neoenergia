//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPacote.h"
#include "VTCampo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTPacote* __fastcall NewObjPacote(void)
   {
   return(new TPacote());
   }

//---------------------------------------------------------------------------
__fastcall TPacote::TPacote(void)
   {
   try{//cria listas p/ objetos VTCampo
      lisCAMPO = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TPacote::~TPacote(void)
   {
   //destrói lista e seus objetos
   if (lisCAMPO) {LimpaTList(lisCAMPO); delete lisCAMPO; lisCAMPO = NULL;}
   }

//---------------------------------------------------------------------------
VTCampo* __fastcall TPacote::Campo(AnsiString nome)
   {
   //variáveis locais
   VTCampo *campo;
   
   //verifica se existe o Campo indicado
   if ((campo = ExisteCampo(nome)) ==  NULL)
      {//cria um novo Campo
      lisCAMPO->Add(campo = NewObjCampo(nome));
      }
   return(campo);
   }

//---------------------------------------------------------------------------
void __fastcall TPacote::Clear(void)
   {
   //limpa lista lisCAMPO
   LimpaTList(lisCAMPO);
   }

//---------------------------------------------------------------------------
VTCampo* __fastcall TPacote::ExisteCampo(AnsiString nome)
   {
   //variáveis locais
   VTCampo *campo;

   //procura Campo c/ o nome indicado
   for (int n = 0; n < lisCAMPO->Count; n++)
      {
      campo = (VTCampo*)lisCAMPO->Items[n];
      if (campo->Nome.AnsiCompareIC(nome) == 0) return(campo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TPacote::PM_GetCamposAsStr(void)
   {
   //variáveis locais
   char       separador = ';';
   AnsiString txt;
   VTCampo   *campo;

   //loop p/ todos objetos Campo
   for (int n = 0; n < lisCAMPO->Count; n++)
      {
      campo = (VTCampo*)lisCAMPO->Items[n];
      txt  += campo->Nome + ":" + campo->AsString + separador;
      }
   return(txt);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TPacote::PM_SetCamposAsStr(AnsiString txt)
	{
	//variáveis locais
	TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	TStringList *StrList;

	//cria objeto TStringList
	StrList = new TStringList();
	//define caracteres de separacao de campos
	Separators << ';' << ':';
	//define caracteres a serem ignorados no início do string
	WhiteSpace << ' ';
	//destrói objetos Campo existentes
	Clear();
	//extrai campos de txt
	ExtractStrings(Separators, WhiteSpace, WideString(txt).c_bstr(), StrList);
	//cria Campo e inicia seu valor
	for (int n = 0; n < StrList->Count; n+=2)
	  {//cria Campo e inicia seu valor
	  Campo(StrList->Strings[n])->AsString = StrList->Strings[n+1];
		}
	//destrói objeto TStringList
	delete StrList;
	}
*/
//---------------------------------------------------------------------------
void __fastcall TPacote::PM_SetCamposAsStr(AnsiString txt)
	{
	//variáveis locais
	TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	TStringList *StrList;
	AnsiString  separadores = ":;";

	//cria objeto TStringList
	StrList = new TStringList();
	//define caracteres de separacao de campos
	Separators << ';' << ':';
	//define caracteres a serem ignorados no início do string
	WhiteSpace << ' ';
	//destrói objetos Campo existentes
	Clear();
	//extrai campos de txt
	ExtraiStrings(txt, separadores, StrList);
	//cria Campo e inicia seu valor
	for (int n = 0; n < StrList->Count; n+=2)
	  {//cria Campo e inicia seu valor
	  Campo(StrList->Strings[n])->AsString = StrList->Strings[n+1];
		}
	//destrói objeto TStringList
	delete StrList;
	}

//-----------------------------------------------------------------------------
//eof



