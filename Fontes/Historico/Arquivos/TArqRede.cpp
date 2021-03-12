//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TArqRede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TArqRede::TArqRede(TDateTime dt_ini, TDateTime dt_fim)
   {
   //salva mes/ano da data inicial
   data_ini.mes = MonthOf(dt_ini);
   data_ini.ano = YearOf(dt_ini);
   //salva mes/ano da data final
   data_fim.mes = MonthOf(dt_fim);
   data_fim.ano = YearOf(dt_fim);
   //cria TStringList
   files = new TStringList();
   //cria lista
   lisSub = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TArqRede::~TArqRede(void)
   {
   //destr�i objetos
   if (files)  {delete files; files  = NULL;}
   if (lisSub)  {delete lisSub; lisSub  = NULL;}
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TArqRede::DataAsString(void)
   {
   //vari�veis locais
   AnsiString txt;

   //verifica se data inicial e final s�o iguais
   if ((data_ini.mes == data_fim.mes)&&(data_ini.ano == data_fim.ano))
      {
      //txt.sprintf("Redes: %02d/%4d", data_ini.mes, data_ini.ano);
      txt.sprintf("%02d/%4d", data_ini.mes, data_ini.ano);
      }
   else
      {
      //txt.sprintf("Redes: %02d/%4d a %02d/%4d",
      //            data_ini.mes, data_ini.ano, data_fim.mes, data_fim.ano);
      txt.sprintf("%02d/%4d a %02d/%4d",
                  data_ini.mes, data_ini.ano, data_fim.mes, data_fim.ano);
      }
   return(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TArqRede::InsereArquivo(AnsiString filename)
   {
   //verifica se o arquivo j� existe
   if (files->IndexOf(filename) < 0)
      {
      files->Add(filename);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TArqRede::InsereArquivo(TStrings *lines)
   {
   //insere todos arquivos
   for (int n = 0; n < lines->Count; n++)
      {
      InsereArquivo(lines->Strings[n]);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TArqRede::PM_GetEnabled(void)
   {
   return(files->Count > 0);
   }

//---------------------------------------------------------------------------
//eof

