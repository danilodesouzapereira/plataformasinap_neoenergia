//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TArqMedicao.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TArqMedicao::TArqMedicao(TDateTime dt)
   {
   //salva mes/ano da data
   data.mes = MonthOf(dt);
   data.ano = YearOf(dt);
   //cria TStringList
   files = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TArqMedicao::~TArqMedicao(void)
   {
   //destrói objetos
   if (files)  {delete files; files  = NULL;}
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TArqMedicao::DataAsString(void)
   {
   //variáveis locais
   AnsiString txt;

   //return(txt.sprintf("Medição: %02d/%4d", data.mes, data.ano));
   return(txt.sprintf("%02d/%4d", data.mes, data.ano));
   }

//---------------------------------------------------------------------------
void __fastcall TArqMedicao::InsereArquivo(AnsiString filename)
   {
   //verifica se o arquivo já existe
   if (files->IndexOf(filename) < 0)
      {
      files->Add(filename);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TArqMedicao::InsereArquivo(TStrings *lines)
   {
   //insere todos arquivos
   for (int n = 0; n < lines->Count; n++)
      {
      InsereArquivo(lines->Strings[n]);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TArqMedicao::PM_GetEnabled(void)
   {
   return(files->Count > 0);
   }

//---------------------------------------------------------------------------
//eof

