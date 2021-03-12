//---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#pragma hdrstop
#include "TArq.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TArq::TArq(void)
   {
   //cria TStringList
   strList = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TArq::~TArq(void)
   {
   //destrói TStringList
   if (strList) {delete strList; strList = NULL;}
   }
   
//---------------------------------------------------------------------------
bool __fastcall TArq::Abre(AnsiString arq)
   {
   //verifica se o arquivo existe
   if (! FileExists(arq)) return(false);
   //abre arquivo p/ leitura
   fp = fopen(arq.c_str(), "rt");
   return(fp != NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TArq::Fecha(void)
   {
   //fecha o arquivo
   if (fp) fclose(fp);
   }

//---------------------------------------------------------------------------
TStringList* __fastcall TArq::LeLinha(char chr)
   {
   //variáveis locais
   #define EOS '\0'
   char    *p_ini, *p_fim;

   //lê uma linha do arquivo
   if (fgets(buf, sizeof(buf), fp) == NULL) return(NULL);
   //extrai os campos delimitados por chr em buf
   LeLinha(buf, chr);
   return(strList);
   }

//---------------------------------------------------------------------------
TStringList* __fastcall TArq::LeLinha(char *p_ini, char chr)
   {
   //variáveis locais
   #define EOS '\0'
	char       *p_fim;

   //limpa StringList
	strList->Clear();
   //extrai os campos delimitados por chr em p_ini
   while ((p_fim = strchr(p_ini, chr)) != NULL)
      {//insere string na lista
      *p_fim = EOS; //insere delimitador de string
		strList->Add(AnsiString(p_ini).Trim());
      //copia em pstr o substring p_ini a p_fim-1
      *p_fim = chr;     //restaura chr no string original
      p_ini = p_fim + 1;
		}
	//insere ultimo string sem delimitador no fim
	if(strList->Count > 0) strList->Add(AnsiString(p_ini).Trim());

	return(strList);
   }

//---------------------------------------------------------------------------
//eof
