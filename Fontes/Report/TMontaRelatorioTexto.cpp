//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include "TMontaRelatorioTexto.h"
#include "VTDados.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMontaRelatorio* __fastcall NewObjMontaRelatorioTexto(void)
   {
   return(new TMontaRelatorioTexto());
   }

//---------------------------------------------------------------------------
__fastcall TMontaRelatorioTexto::TMontaRelatorioTexto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaRelatorioTexto::~TMontaRelatorioTexto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRelatorioTexto::Executa(AnsiString FileName, VTDados *Dados, AnsiString titulo, bool append)
   {
   //vari�veis locais
	AnsiString  texto;
   TList       *LisStringList;
   TStringList *StrList;
	FILE        *stream;

   //prote��o
	if (Dados == NULL) return false;
   //abre o arquivo p/ grava��o
   if (append) stream = fopen(FileName.c_str(), "a+");
   else        stream = fopen(FileName.c_str(), "w+");
 	if (stream == NULL) return(false);
   //verifica se est� fazendo append
   if (append)
      {//insere 2 linhas de separa��o
      fprintf(stream, "\n\n");
      }
   //verifica se existe t�tulo
   if (! titulo.IsEmpty())
      {
      fprintf(stream, "%s", titulo.c_str());
		fprintf(stream, "\n");
      }
   //loop p/ todos registros
   LisStringList = Dados->LisStringList();
	for(int nl = 0; nl < LisStringList->Count; nl++)
	   {
      StrList = (TStringList*)LisStringList->Items[nl];
		for(int n = 0; n < StrList->Count; n++)
		   {
			texto = StrList->Strings[n];
//			fprintf(stream, "%s; ", texto.c_str());
         fprintf(stream, "%s;", texto.c_str()); //DVK 2017.02.16 sem espa�o
		   }
		fprintf(stream, "\n");
	   }
   //fecha arquivo
   fclose(stream);
	return true;
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRelatorioTexto::Executa(AnsiString FileName, AnsiString PlanilhaName, VTDados *Dados)
   {
   return(false);
   }
   
//---------------------------------------------------------------------------
//eof


