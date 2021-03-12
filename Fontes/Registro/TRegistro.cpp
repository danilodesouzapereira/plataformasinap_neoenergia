//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRegistro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTRegistro* __fastcall NewObjRegistro(AnsiString arquivo)
   {
   try{
      return(new TRegistro(arquivo));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TRegistro::TRegistro(AnsiString arquivo)
   {
   try{//cria objeto
      RegistryIniFile = new TRegistryIniFile(arquivo);
/*
//por default,  RegistryIniFile->RegIniFile->RootKey = HKEY_LOCAL_MACHINE;
//redefine RootKey
RegistryIniFile->RegIniFile->RootKey = HKEY_LOCAL_MACHINE;
//redefine CurrentKey
RegistryIniFile->RegIniFile->OpenKey("SOFTWARE", true);
*/
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
__fastcall TRegistro::~TRegistro(void)
   {
   //destrói objeto
   if (RegistryIniFile) {delete RegistryIniFile; RegistryIniFile=NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::ExisteInfo(AnsiString secao, AnsiString info, AnsiString &valor)
   {
   //verifica se existe a Secao
   if (! RegistryIniFile->SectionExists(secao)) return(false);
   //verifica se existe o campo
   if (! RegistryIniFile->ValueExists(secao, info)) return(false);
   //lê o valor
   valor = RegistryIniFile->ReadString(secao, info, "");
   //verifica erro
   if (valor.IsEmpty()) return(false);
   //leitura correta
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GravaInfo(AnsiString secao, AnsiString info, AnsiString &valor)
   {
   //atualiza o campo
   RegistryIniFile->WriteString(secao, info, valor);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::RetiraSecao(AnsiString secao)
   {
   //atualiza o campo
   RegistryIniFile->EraseSection(secao);
   return(true);
   }

//---------------------------------------------------------------------------
//eof
