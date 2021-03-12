//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TItemAtributo.h"
#include "TItemCatalogo.h"
#include "TFormCatalogoTrf.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormCatalogo"
#pragma resource "*.dfm"
TFormCatalogoTrf *FormCatalogoTrf;
//---------------------------------------------------------------------------
__fastcall TFormCatalogoTrf::TFormCatalogoTrf(TComponent* Owner, VTApl *apl)
   : TFormCatalogo(Owner, apl)
{
}
//---------------------------------------------------------------------------
TItemCatalogo* __fastcall TFormCatalogoTrf::CriaItem(void)
{
   TItemCatalogo *itemCatalogo;
   //TItemAtributo *atributo;

   //cria item do catalogo
   itemCatalogo = new TItemCatalogo();
   itemCatalogo->Codigo  = trfNOVOTRF + IntToStr(listaItemsCatalogo->Count);
   //cria atributos
   itemCatalogo->CriaAtributo(trfATRIBUTO_SNOM_MVA,"0.1125",dtDOUBLE);
   itemCatalogo->CriaAtributo(trfATRIBUTO_VNOMPRI_KV,"13.8",dtDOUBLE);
   itemCatalogo->CriaAtributo(trfATRIBUTO_VNOMSEC_KV,"0.24",dtDOUBLE);
   itemCatalogo->CriaAtributo(trfTRIBUTO_PERDAFE,"0.1",dtDOUBLE);
   //exibe o item no valueListEditor
   ShowItem(itemCatalogo);

   return itemCatalogo;
}
//---------------------------------------------------------------------------
TItemCatalogo* __fastcall TFormCatalogoTrf::CriaItem(DIAG_TRAFO *cfgDiagTrafo)
{
   TItemCatalogo *itemCatalogo;
   //TItemAtributo *atributo;
    //itemCatalogo->Codigo  = diagCapacitor->codigo;
   //itemCatalogo->CriaAtributo(capATRIBUTO_QMVAR,DoubleToStr("%4.3f",diagCapacitor->Q_MVAr),dtDOUBLE);
   //cria item do catalogo
   itemCatalogo = new TItemCatalogo();
   itemCatalogo->Codigo  = cfgDiagTrafo->codigo;
   //cria atributos
   itemCatalogo->CriaAtributo(trfATRIBUTO_SNOM_MVA,DoubleToStr("%4.3f",cfgDiagTrafo->snom_mva),dtDOUBLE);
   itemCatalogo->CriaAtributo(trfATRIBUTO_VNOMPRI_KV,DoubleToStr("%4.3f",cfgDiagTrafo->vnomPri_kv),dtDOUBLE);
   itemCatalogo->CriaAtributo(trfATRIBUTO_VNOMSEC_KV,DoubleToStr("%4.3f",cfgDiagTrafo->vnomSec_kv),dtDOUBLE);
   itemCatalogo->CriaAtributo(trfTRIBUTO_PERDAFE,DoubleToStr("%4.3f",cfgDiagTrafo->perdaFe),dtDOUBLE);
   //exibe o item no valueListEditor
   ShowItem(itemCatalogo);

   return itemCatalogo;

}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogoTrf::IniciaListValue(void)
{  //variaveis locais
   TList          *listaTrafosDoTxt;
   TItemCatalogo  *itemCatalogo;

   //se foi iniciado o form
   if(diagEqpto)
   {
      listaTrafosDoTxt = diagEqpto->ListCFG;
      //se carregou algum cabo, destroi os items por ventura da lista.
      if(listaTrafosDoTxt->Count > 0){LimpaTList(listaItemsCatalogo);}
      for (int i = 0; i < listaTrafosDoTxt->Count; i++)
      {  //cria um item catalogo a partir de um DIAG_CABO
         itemCatalogo = CriaItem((DIAG_TRAFO*) listaTrafosDoTxt->Items[i]);
         //adiciona o item à lista
         listaItemsCatalogo->Add(itemCatalogo);
      }
   }
}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogoTrf::SalvaItemsCatalogo(void)
{  //variaveis locais
   DIAG_TRAFO* cfgDiagTrafo;
   TItemCatalogo  *itemCatalogo;
   TItemAtributo *atributo;

   //destroi todos diag_cabo do diagEqptoElem
   LimpaTList(diagEqpto->ListCFG);
   //preenche ela novamente com os items do catalogo (incluindo possiveis mudanças ou novos)
   for(int i = 0; i < listaItemsCatalogo->Count; i++)
   {  //pega o item
      itemCatalogo = (TItemCatalogo*) listaItemsCatalogo->Items[i];
      //cria o novo DIAG_TRAFO
      cfgDiagTrafo = new DIAG_TRAFO;
      //seta o id de acordo com o contador
      cfgDiagTrafo->id = i;  /* TODO : melhorar id */
      //encontra o atributo codigo
      atributo = itemCatalogo->Get_ItemAtributo(trfATRIBUTO_COD);
      cfgDiagTrafo->codigo = atributo->Valor;
      //encontra o atributo snom_mva
      atributo = itemCatalogo->Get_ItemAtributo(trfATRIBUTO_SNOM_MVA);
      cfgDiagTrafo->snom_mva = StrToDouble(atributo->Valor);
      //encontra o atributo vnomPri_kv
      atributo = itemCatalogo->Get_ItemAtributo(trfATRIBUTO_VNOMPRI_KV);
      cfgDiagTrafo->vnomPri_kv = StrToDouble(atributo->Valor);
      //encontra o atributo vnomSec_kv
      atributo = itemCatalogo->Get_ItemAtributo(trfATRIBUTO_VNOMSEC_KV);
      cfgDiagTrafo->vnomSec_kv = StrToDouble(atributo->Valor);
      //encontra o atributo vnomSec_kv
      atributo = itemCatalogo->Get_ItemAtributo(trfTRIBUTO_PERDAFE);
      cfgDiagTrafo->perdaFe = StrToDouble(atributo->Valor);
      //adiciona o DIAG_TRAFO à lista
      diagEqpto->ListCFG->Add(cfgDiagTrafo);
   }
   if(diagEqpto)
   {  //salva no txt a lista
      diagEqpto->SalvaArquivo();
   }
}

//---------------------------------------------------------------------------

