//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "TItemAtributo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
      __fastcall TItemAtributo::TItemAtributo(void)
{
   visible = true;
   filhosVisible = false;
}
//---------------------------------------------------------------------------
      __fastcall TItemAtributo::~TItemAtributo(void)
{   //se as listas ainda existirem
    if(listAtributos){delete listAtributos; listAtributos = NULL;}
    if(listValoresCombo){delete listValoresCombo; listValoresCombo = NULL;}
}
//---------------------------------------------------------------------------
bool  __fastcall TItemAtributo::InsereAtributoFilho(TItemAtributo *atributo)
{  //variaveis locais
   TItemAtributo *atributo_temp;
   bool existe = false;

   //se existir uma lista de atributos
   if(listAtributos)
   {
      for(int na = 0; na < listAtributos->Count ; na++)
      {
         atributo_temp = (TItemAtributo*)listAtributos->Items[na];
         //verifica a existencia
         if (atributo_temp == atributo)
         {
            existe = true;
            //se existe não insere novamente
            return false;
         }
      }
      //se não existe, insere o atributo
      if(!existe)
      {
         atributo->AtributoPai = this;
         atributo->Nome = atributo->Nome;
         atributo->Visible = this->FilhosVisible;
         listAtributos->Add(atributo);
         return true;
      }
   }
   //não é grupo então não tem lista de atributos
   return false;

}
//---------------------------------------------------------------------------
TItemAtributo*  __fastcall TItemAtributo::InsereAtributoFilho(AnsiString nome,AnsiString valor,int dataType,
                                                   bool isGroup, TList *listaAtributosFilhos,
                                              bool needCombo, TStringList *listaValoresCombo,
                                                                          TItemAtributo  *atributoPai)
{
   try
   {  //esse atributo será destruido qnd o item for destruído
      TItemAtributo *atributo = new TItemAtributo() ;
      //preenche os atributos com os parametros
      atributo->Nome = " " + nome;
      atributo->Valor = valor;
      atributo->ValorDataType = dataType;
      atributo->Group = isGroup;
      atributo->ListaAtributosFilhos =  listaAtributosFilhos;
      atributo->NeedCombo = needCombo;
      atributo->ListaValoresCombo = listaValoresCombo;
      atributo->AtributoPai = atributoPai;
      //adiciona o atributo a lista
      //if (listAtributos) { listAtributos->Add(atributo);}
      //tenta inserir o atributo
      if(InsereAtributoFilho(atributo)){return atributo;}
      else{return NULL;}

   } catch (Exception &e)
   {
      return NULL;
   }
}

//---------------------------------------------------------------------------
bool  __fastcall TItemAtributo::InsereValorLista(AnsiString valor)
{  //variaveis locais
   bool existe = false;

   //se existir uma lista de valores
   if(listValoresCombo)
   {
      for(int nv = 0; nv < listValoresCombo->Count ; nv++)
      {
         //verifica a existencia
         if (valor.AnsiCompareIC(listValoresCombo->Strings[nv]) == 0)
         {
            existe = true;
            //se existe não insere novamente
            return false;
         }
      }
      //se não existe, insere o atributo
      if(!existe){listValoresCombo->Add(valor); return true;}
   }
   //não é grupo então não tem lista de atributos
   return false;
}

//---------------------------------------------------------------------------
bool  __fastcall TItemAtributo::RetiraAtributoFilho(TItemAtributo *atributo)
{
   if(listAtributos)
   {
      if(listAtributos->Remove(atributo))
      {
         return true;
      }
   }
   return false;
}
//---------------------------------------------------------------------------
bool  __fastcall TItemAtributo::RetiraValorLista(AnsiString valor)
{
   if(listValoresCombo)
   {
      for(int ns = listValoresCombo->Count - 1; ns > 0 ; ns--)
      {
         if(valor.AnsiCompareIC(listValoresCombo->Strings[ns]) == 0)
         {
            listValoresCombo->Delete(ns);
            return true;
         }
      }
   }
   return false;
}
//---------------------------------------------------------------------------
//Getters
bool __fastcall  TItemAtributo::PM_GetGroup(void)
{
   return this->group;
}
//---------------------------------------------------------------------------
bool __fastcall  TItemAtributo::PM_GetNeedCombo(void)
{
   return this->needCombo;
}
//---------------------------------------------------------------------------
//Setters
void __fastcall  TItemAtributo::PM_SetGroup(bool isGroup)
{
   if(isGroup)
   {  //cria a lista
      if(!listAtributos){listAtributos = new TList();}
      this->group = true;
      this->Valor = "------";
   }
   else
   {  //destroi a lista
      if(listAtributos){delete listAtributos; listAtributos = NULL;}
      this->group = false;
   }
}
//---------------------------------------------------------------------------
void __fastcall   TItemAtributo::PM_SetListaAtributos(TList *listaAtributosNovos)
{  //destroi a lista
   if(listAtributos){delete listAtributos; listAtributos = NULL;}
   //seta o novo ponteiro
   listAtributos = listaAtributosNovos;
}
//---------------------------------------------------------------------------
void __fastcall  TItemAtributo::PM_SetNeedCombo(bool needCombo)
{
   if(needCombo)
   {  //cria a lista
      if(!listValoresCombo){listValoresCombo = new TStringList();}
      this->needCombo = true;
   }
   else
   {  //destroi a lista
      if(listValoresCombo){delete listValoresCombo; listValoresCombo = NULL;}
      this->needCombo = false;
   }
}
//---------------------------------------------------------------------------
