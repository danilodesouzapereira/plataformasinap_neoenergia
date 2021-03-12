//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TItemAtributo.h"
#include "TItemCatalogo.h"
#include "TFormCatalogoCap.h"
#include "..\Rede\VTCapacitor.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormCatalogo"
#pragma resource "*.dfm"
TFormCatalogoCap *FormCatalogoCap;
//---------------------------------------------------------------------------
__fastcall TFormCatalogoCap::TFormCatalogoCap(TComponent* Owner, VTApl *apl)
   : TFormCatalogo(Owner, apl)
{
}
//---------------------------------------------------------------------------
TItemCatalogo* __fastcall TFormCatalogoCap::CriaItem(void)
{
   TItemCatalogo *itemCatalogo;
   //TItemAtributo *atributo;
   //TStringList   *lisValores = new TStringList();

   //cria item do catalogo
   itemCatalogo = new TItemCatalogo();
   itemCatalogo->Codigo  = capNOVOCAP + IntToStr(listaItemsCatalogo->Count);;
   //cria atributos
   itemCatalogo->CriaAtributo(capATRIBUTO_QMVAR,"0.300",dtDOUBLE);
   //exibe o item no valueListEditor
   ShowItem(itemCatalogo);

   return itemCatalogo;

}
//---------------------------------------------------------------------------
TItemCatalogo* __fastcall TFormCatalogoCap::CriaItem(DIAG_CAPACITOR *diagCapacitor)
{
   TItemCatalogo *itemCatalogo;
   //TItemAtributo *atributo;
   //TStringList   *lisValoresBool= new TStringList();

   //lisValoresBool->Add("True");
   //lisValoresBool->Add("False");
   //cria item do catalogo
   itemCatalogo = new TItemCatalogo();
   itemCatalogo->Codigo  = diagCapacitor->codigo;
   itemCatalogo->CriaAtributo(capATRIBUTO_QMVAR,DoubleToStr("%4.3f",diagCapacitor->Q_MVAr),dtDOUBLE);
   //exibe o item no valueListEditor
   ShowItem(itemCatalogo);

   return itemCatalogo;
}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogoCap::IniciaListValue(void)
{  //variaveis locais
   TList          *listaCapsDoTxt;
   TItemCatalogo  *itemCatalogo;

   //se foi iniciado o form
   if(diagEqpto)
   {
      listaCapsDoTxt = diagEqpto->ListCFG;
      //se carregou algum cabo, destroi os items por ventura da lista.
      if(listaCapsDoTxt->Count > 0){LimpaTList(listaItemsCatalogo);}
      for (int i = 0; i < listaCapsDoTxt->Count; i++)
      {  //cria um item catalogo a partir de um DIAG_CABO
         itemCatalogo = CriaItem((DIAG_CAPACITOR*) listaCapsDoTxt->Items[i]);
         //adiciona o item à lista
         listaItemsCatalogo->Add(itemCatalogo);
      }
   }
}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogoCap::SalvaItemsCatalogo(void)
{  //variaveis locais
   DIAG_CAPACITOR* diagCapacitor;
   TItemCatalogo  *itemCatalogo;
   TItemAtributo *atributo;

   //destroi todos diag_cabo do diagEqptoElem
   LimpaTList(diagEqpto->ListCFG);
   //preenche ela novamente com os items do catalogo (incluindo possiveis mudanças ou novos)
   for(int i = 0; i < listaItemsCatalogo->Count; i++)
   {  //pega o item
      itemCatalogo = (TItemCatalogo*) listaItemsCatalogo->Items[i];
      //cria o novo DIAG_CAPACITOR
      diagCapacitor = new DIAG_CAPACITOR;
      //seta o id/tipo de acordo com o contador
      diagCapacitor->id = i;  /* TODO : melhorar id */
      //encontra o atributo codigo
      atributo = itemCatalogo->Get_ItemAtributo(capATRIBUTO_COD);
      diagCapacitor->codigo = atributo->Valor;
      //encontra o atributo qmvar
      atributo = itemCatalogo->Get_ItemAtributo(capATRIBUTO_QMVAR);
      diagCapacitor->Q_MVAr = StrToDouble(atributo->Valor);

      //adiciona o DIAG_CABO à lista
      diagEqpto->ListCFG->Add(diagCapacitor);
   }
   if(diagEqpto)
   {  //salva no txt a lista
      diagEqpto->SalvaArquivo();
   }
}

//---------------------------------------------------------------------------

