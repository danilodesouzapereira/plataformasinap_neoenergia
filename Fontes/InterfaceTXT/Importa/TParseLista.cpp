//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Log\VTLog.h>
#include "TParseLista.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// fun��o global
//-----------------------------------------------------------------------------
VTParse* NewObjParseLista(VTApl *apl)
   {
   try{
		return(new TParseLista(apl));
      }catch(Exception &e)
         {//nada a fazer
			}
   //errno na cria��o do objeto
   return(NULL);
   }

//-----------------------------------------------------------------------------
__fastcall TParseLista::TParseLista(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//-----------------------------------------------------------------------------
__fastcall TParseLista::~TParseLista(void)
   {
   //nada a fazer
	}

//-----------------------------------------------------------------------------
bool __fastcall TParseLista::Executa(AnsiString arq_txt)
   {
   //n�o implementado nesta classe
   return(false);
   }

//-----------------------------------------------------------------------------
bool __fastcall TParseLista::Executa(AnsiString arq_lista, TStrings *lines)
   {
   //vari�veis locais
   AnsiString dir_name, filename;

   //determina diret�rio do arquivo
   dir_name = ExtractFileDir(arq_lista);
   //carrega lista de arquivos
   lines->LoadFromFile(arq_lista);
   //inclui diret�rio no nome de cada arquivo
   for (int n = 0; n < lines->Count; n++)
      {
      filename = dir_name + "\\" + lines->Strings[n].Trim();
      lines->Strings[n] = filename;
      }
   return(true);
   }


//-----------------------------------------------------------------------------
//eof
