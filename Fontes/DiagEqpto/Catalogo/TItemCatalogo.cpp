//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "TItemCatalogo.h"
#include "TItemAtributo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
      __fastcall TItemCatalogo::TItemCatalogo(void)
{
   listAtributos = new TList();
}
//---------------------------------------------------------------------------
      __fastcall TItemCatalogo::~TItemCatalogo(void)
{
   if(listAtributos){ LimpaTList(listAtributos); delete listAtributos; listAtributos = NULL;}
}
//---------------------------------------------------------------------------
//Função cheia de parametros pra criar um atributo dentro desse item de catalogo
TItemAtributo*  __fastcall TItemCatalogo::CriaAtributo(AnsiString nome,AnsiString valor,
                                                      int dataType, bool isGroup,
                                    TList *listaAtributosFilhos, bool needCombo,
                     TStringList *listaValoresCombo,TItemAtributo  *atributoPai)
{
   try
   {  //esse atributo será destruido qnd o item for destruído
      TItemAtributo *atributo = new TItemAtributo() ;
      //preenche os atributos com os parametros
      atributo->Nome = nome;
      atributo->Valor = valor;
      atributo->ValorDataType = dataType;
      atributo->Group = isGroup;
      //se houver uma nova lista sobrescreve a anterior?
      if(listaAtributosFilhos)atributo->ListaAtributosFilhos =  listaAtributosFilhos;
      atributo->NeedCombo = needCombo;
      //se houver uma nova lista sobrescreve a anterior?
      if(listaValoresCombo)atributo->ListaValoresCombo = listaValoresCombo;
      //se tiver atributo pai, insere no pai
      if(atributoPai)
      {
         atributo->AtributoPai = atributoPai;
         atributoPai->InsereAtributoFilho(atributo);
      }
      //adiciona o atributo a lista
      if (listAtributos) { listAtributos->Add(atributo);}
      //retorna o endereço do atributo
      return atributo;

   } catch (Exception &e)
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
TItemAtributo* __fastcall TItemCatalogo::Get_ItemAtributo(AnsiString nomeAtributo)
{  //variaveis locais
   TItemAtributo *atributo;

   for(int na = 0; na < listAtributos->Count; na++)
   {
      atributo = (TItemAtributo*)listAtributos->Items[na];
      if(nomeAtributo.AnsiCompare(TrimLeft(atributo->Nome)) == 0){return atributo;}
   }
   return NULL;
}

//---------------------------------------------------------------------------
int __fastcall TItemCatalogo::Get_ItemAtributoIndex(AnsiString nomeAtributo)
{  //variaveis locais
   TItemAtributo *atributo;

   for(int na = 0; na < listAtributos->Count; na++)
   {
      atributo = (TItemAtributo*)listAtributos->Items[na];
      if(nomeAtributo.AnsiCompare(TrimLeft(atributo->Nome)) == 0){return na;}
   }
   return -1;
}

//---------------------------------------------------------------------------
AnsiString  __fastcall TItemCatalogo::PM_GetCodigo(void)
{  //caso exista um ponteiro pro código do item
   if(codigo)
   {
      return codigo->Valor;
   }
   return " ";
}
//---------------------------------------------------------------------------
void  __fastcall TItemCatalogo::PM_SetCodigo(AnsiString codigo)
{  //se existe o ponteiro, troca o valor
   if(this->codigo)
   {
      this->codigo->Valor = codigo;
   }
   //se não, cria um atributo
   else
   {  //guarda o ponteiro
      this->codigo = CriaAtributo("Código",codigo, dtANSISTRING);
   }
}
//---------------------------------------------------------------------------
