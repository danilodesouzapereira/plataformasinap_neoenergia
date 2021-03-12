//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <SysUtils.hpp>
#include "TImportaEstudo.h"
#include "VTExtrator.h"
#include "VTParse.h"
#include "VTMonta.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTImportaEstudo* NewObjImportaEstudo(VTApl *apl_owner)
   {
   return(new TImportaEstudo(apl_owner));
   }

//-----------------------------------------------------------------------------
__fastcall TImportaEstudo::TImportaEstudo(VTApl *apl_owner)
   {
   try{//cria objeto Apl local
      apl = NewObjApl(NULL, apl_owner);
      //cria objetos usandos na importação do arqivo
      apl->Add(parse    = NewObjParse(apl));
      apl->Add(NewObjExtrator(apl));
      apl->Add(NewObjMonta(apl));
      }catch(Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
__fastcall TImportaEstudo::~TImportaEstudo(void)
   {
   //destrói objetos criados
   if (apl) {delete apl; apl = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TImportaEstudo::Executa(AnsiString arq_txt)
   {
   //variáveis locais
   bool sucesso;

   try{//executa o Parse
      sucesso = parse->Executa(arq_txt);
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }


//-----------------------------------------------------------------------------
//eof
