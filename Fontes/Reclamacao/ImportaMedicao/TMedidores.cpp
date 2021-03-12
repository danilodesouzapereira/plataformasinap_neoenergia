//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include "..\..\DLL_Inc\Funcao.h"
#include "TMedidores.h"
#include "EstruturaMed.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMedidores::TMedidores(VTApl *apl_owner)
{
   VTPath *path;
   AnsiString caminho;
   bool dirCriado = true;

   //salva ponteiro p/ objeto
   this->apl = apl_owner;
   listMedidores = new TList;
   path = (VTPath*)apl->GetObject(__classid(VTPath));
   caminho = path->DirDat()+"\\Medidores\\";
   if(! DirectoryExists(caminho))
   {
      dirCriado = CreateDir(caminho);
   }
   if(dirCriado){filepath = path->DirDat() +"\\Medidores\\perfilMedidores.txt";}
   if(FileExists(filepath))
   {
      LeArqMedidores(filepath);
   }
}
//---------------------------------------------------------------------------
__fastcall TMedidores::~TMedidores(void)
{
   if(listMedidores)
   {
      for (int i = listMedidores->Count -1; i > -1; i--)
      {
         listMedidores->Delete(i);
      }
   delete listMedidores;
   listMedidores = NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TMedidores::AddMedidor(strMEDIDOR *medidor)
{
   if(Consistente(medidor))
   {
      listMedidores->Add(medidor);
   }
}
//---------------------------------------------------------------------------
void __fastcall TMedidores::AddMedidorTXT(strMEDIDOR *medidor)
{  //salva perfil no arquivo txt
   FILE *arqtxt;

   if(Consistente(medidor))
   {
      arqtxt = fopen(filepath.c_str(), "a");
      if(arqtxt == NULL)
      {
         Aviso("Não foi encontrado o arquivo com as definições de medidores");
         return;
      }
      else
      {
		 fprintf(arqtxt,"\n%s;",medidor->nome.c_str());
         fprintf(arqtxt,"%i;",medidor->numFases);
         fprintf(arqtxt,"%i;",medidor->colData);
         fprintf(arqtxt,"%i;",medidor->colHora);
         fprintf(arqtxt,"%i;",medidor->colV1);
         fprintf(arqtxt,"%i;",medidor->colV2);
         fprintf(arqtxt,"%i",medidor->colV3);
      }
      fclose(arqtxt);
   }
}
//---------------------------------------------------------------------------
strMEDIDOR* __fastcall TMedidores::BuscaPorNome(AnsiString nome)
{
   strMEDIDOR *aux;

   for (int i = listMedidores->Count-1 ; i >= 0; i--)
   {
      aux = (strMEDIDOR*)listMedidores->Items[i];
      if (aux->nome == nome){   return aux;}
   }
   return NULL;
}
//---------------------------------------------------------------------------
bool  __fastcall TMedidores::Consistente(strMEDIDOR *medidor)
{
   bool sucesso =  true;
   if(medidor->colData==0){sucesso =  false;}
   if(medidor->colHora==0){sucesso =  false;}
   if((medidor->colV1==0)&&(medidor->colV2)&&(medidor->colV3)){sucesso =  false;}
   //provisório, desenvolver jeito de ler arquivo sem v1
   if(medidor->colV1==0){sucesso =  false;}
   return sucesso;
}
//---------------------------------------------------------------------------
TList* __fastcall TMedidores::GetListMedidores(void)
{  //atualiza lista de medidores e passa a lista atualizada
   LeArqMedidores(filepath);
   return listMedidores;
}
//---------------------------------------------------------------------------
void __fastcall TMedidores::LeArqMedidores(AnsiString filepath)
{
   TStringList *campos;
   TStringList *linhas;
   strMEDIDOR *medidor;
   AnsiString comeco;
   int numFases;
   bool sucesso =true;

   //proteção //DVK 2015.04.14
   if(! FileExists(filepath)) return;
   //cria stringList
   campos = new TStringList;
   linhas = new TStringList;
   linhas->LoadFromFile(filepath);
   listMedidores->Clear();
   for (int nl=0; nl < linhas->Count ; nl++)
   {
      numFases = 1;
      medidor = new strMEDIDOR; //destruo no destructor da lista de medidores
      linhas->Strings[nl].TrimLeft();
      if(linhas->Strings[nl].IsEmpty()){continue;} //pula linhas em branco
      comeco = linhas->Strings[nl].SubString(0,2);
      if(comeco == "//"){continue;}//pula linhas de comentario
      ExtraiStrings(linhas->Strings[nl], ";\t", campos);
      medidor->nome = campos->Strings[0];
      try{medidor->colData = campos->Strings[2].ToInt();}
      catch (Exception &e){sucesso = false;}
      try{medidor->colHora = campos->Strings[3].ToInt();}
      catch (Exception &e){sucesso = false;}
      try{medidor->colV1 = campos->Strings[4].ToInt();}
      catch (Exception &e){sucesso = false;}
      try
      {
         medidor->colV2 = campos->Strings[5].ToInt();
         if(! medidor->colV2 ==0){numFases++;}
      }
      catch (Exception &e){sucesso = false;}
      try
      {
         medidor->colV3 = campos->Strings[6].ToInt();
         if(! medidor->colV3 ==0){numFases++;}
      }
      catch (Exception &e){sucesso = false;}
      try{medidor->numFases = campos->Strings[1].ToInt();}
      catch (Exception &e){medidor->numFases = 0;}
      if(numFases != medidor->numFases)
      {
         Aviso("Numero de fases imcompatível com a descrição das colunas de tensão.");
         medidor->numFases = numFases;
      }
      if(sucesso){AddMedidor(medidor);}
   }
   delete campos;
   delete linhas;
}
//---------------------------------------------------------------------------
//eof
