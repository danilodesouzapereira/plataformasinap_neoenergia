//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TItemAtributo.h"
#include "TItemCatalogo.h"
#include "TFormCatalogoCab.h"
#include "..\Arranjo\VTCabo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormCatalogo"
#pragma resource "*.dfm"
TFormCatalogoCab *FormCatalogoCab;
//---------------------------------------------------------------------------
__fastcall TFormCatalogoCab::TFormCatalogoCab(TComponent* Owner, VTApl *apl)
   : TFormCatalogo(Owner, apl)
{
}
//---------------------------------------------------------------------------
TItemCatalogo* __fastcall TFormCatalogoCab::CriaItem(void)
{
   TItemCatalogo *itemCatalogo;
   TItemAtributo *atributo;
   TStringList   *lisValoresBool= new TStringList();

   lisValoresBool->Add("Verdadeiro");
   lisValoresBool->Add("Falso");

   //cria item do catalogo
   itemCatalogo = new TItemCatalogo();
   itemCatalogo->Codigo  = cabNOVOCAB + IntToStr(listaItemsCatalogo->Count);
   itemCatalogo->CriaAtributo(cabATRIBUTO_IADM,DoubleToStr("%2.1f",100.),dtDOUBLE);
   itemCatalogo->CriaAtributo(cabATRIBUTO_VMAX,DoubleToStr("%2.1f",15.),dtDOUBLE);
   itemCatalogo->CriaAtributo(cabATRIBUTO_VMIN,DoubleToStr("%2.1f",0.),dtDOUBLE);
   //cria grupo Definido por R/RMG
   atributo = itemCatalogo->CriaAtributo(cabATRIBUTO_DEFRMG," ",dtANSISTRING,true);
   itemCatalogo->CriaAtributo(cabATRIBUTO_RESIST," ",dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_RAIO," ",dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_RMG," ",dtDOUBLE, false, NULL, false, NULL, atributo);


   //cria grupo Definido por Z
   atributo = itemCatalogo->CriaAtributo(cabATRIBUTO_DEFZSEQ," ",dtANSISTRING,true);
   //cabo pré-reunido?
   itemCatalogo->CriaAtributo(cabATRIBUTO_PREUN,"False",dtBOOL, false, NULL, true, lisValoresBool, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z0R,DoubleToStr("%4.3f",0.02),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z0X,DoubleToStr("%4.3f",0.02),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z0C,DoubleToStr("%4.3f",0.00),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z1R,DoubleToStr("%4.3f",0.02),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z1X,DoubleToStr("%4.3f",0.02),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z1C,DoubleToStr("%4.3f",0.00),dtDOUBLE, false, NULL, false, NULL, atributo);

   //exibe o item no valueListEditor
   ShowItem(itemCatalogo);

//   diagCabo = new DIAG_CABO;
//   diagCabo->id = listaItemsCatalogo->Count;
//   diagCabo->cfg_cabo = caboZ0Z1;
//   diagCabo->codigo = itemCatalogo->Codigo;
//   diagCabo->iadm_a = 100;
//   diagCabo->vmax_kv = 15;
//   diagCabo->vmin_kv = 0;
//   diagCabo->r_ohm_km = NULL;
//   diagCabo->raio_m = NULL;
//   diagCabo->gmr_m = NULL;
//   diagCabo->z0_r_ohm_km = 0.02;
//   diagCabo->z0_x_ohm_km = 0.02;
//   diagCabo->z1_r_ohm_km = 0.02;
//   diagCabo->z1_x_ohm_km = 0.02;
//   diagCabo->c0_nf_km = 0;
//   diagCabo->c1_nf_km = 0;
//
//   //exibe o item no valueListEditor
//   ShowItem(itemCatalogo);

   return itemCatalogo;

}
//---------------------------------------------------------------------------
TItemCatalogo* __fastcall TFormCatalogoCab::CriaItem(DIAG_CABO *diagCabo)
{
   TItemCatalogo *itemCatalogo;
   TItemAtributo *atributo;
   TStringList   *lisValoresBool= new TStringList();

   lisValoresBool->Add("Verdadeiro");
   lisValoresBool->Add("Falso");
   //cria item do catalogo
   itemCatalogo = new TItemCatalogo();
   itemCatalogo->Codigo  = diagCabo->codigo;
   itemCatalogo->CriaAtributo(cabATRIBUTO_IADM,DoubleToStr("%4.3f",diagCabo->iadm_a),dtDOUBLE);
   itemCatalogo->CriaAtributo(cabATRIBUTO_VMAX,DoubleToStr("%4.3f",diagCabo->vmax_kv),dtDOUBLE);
   itemCatalogo->CriaAtributo(cabATRIBUTO_VMIN,DoubleToStr("%4.3f",diagCabo->vmin_kv),dtDOUBLE);
   //cria grupo Definido por R/RMG
   atributo = itemCatalogo->CriaAtributo(cabATRIBUTO_DEFRMG," ",dtANSISTRING,true);
   itemCatalogo->CriaAtributo(cabATRIBUTO_RESIST,DoubleToStr("%4.3f",diagCabo->r_ohm_km),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_RAIO,DoubleToStr("%4.3f",diagCabo->raio_m),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_RMG,DoubleToStr("%4.3f",diagCabo->gmr_m),dtDOUBLE, false, NULL, false, NULL, atributo);
   //cria grupo Definido por Z
   atributo = itemCatalogo->CriaAtributo(cabATRIBUTO_DEFZSEQ," ",dtANSISTRING,true);
   //traducao de boolean! q droga ahha
   //itemCatalogo->CriaAtributo(cabATRIBUTO_PREUN,BoolToStr(diagCabo->preReunido) ,dtBOOL, false, NULL, true, lisValoresBool, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_PREUN,BooleanoParaString(diagCabo->preReunido) ,dtBOOL, false, NULL, true, lisValoresBool, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z0R,DoubleToStr("%4.3f",diagCabo->z0_r_ohm_km),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z0X,DoubleToStr("%4.3f",diagCabo->z0_x_ohm_km),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z0C,DoubleToStr("%4.3f",diagCabo->c0_nf_km),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z1R,DoubleToStr("%4.3f",diagCabo->z1_r_ohm_km),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z1X,DoubleToStr("%4.3f",diagCabo->z1_x_ohm_km),dtDOUBLE, false, NULL, false, NULL, atributo);
   itemCatalogo->CriaAtributo(cabATRIBUTO_Z1C,DoubleToStr("%4.3f",diagCabo->c1_nf_km),dtDOUBLE, false, NULL, false, NULL, atributo);

   //exibe o item no valueListEditor
   ShowItem(itemCatalogo);

   return itemCatalogo;

}
//---------------------------------------------------------------------------
void  __fastcall TFormCatalogoCab::IniciaListValue(void)
{  //variaveis locais
   TList          *listaCabosDoTxt;
   TItemCatalogo  *itemCatalogo;

   //se foi iniciado o form
   if(diagEqpto)
   {
      listaCabosDoTxt = diagEqpto->ListCFG;
      //se carregou algum cabo, destroi os items por ventura da lista.
      if(listaCabosDoTxt->Count > 0){LimpaTList(listaItemsCatalogo);}
      for (int i = 0; i < listaCabosDoTxt->Count; i++)
      {  //cria um item catalogo a partir de um DIAG_CABO
         itemCatalogo = CriaItem((DIAG_CABO*) listaCabosDoTxt->Items[i]);
         //adiciona o item à lista
         listaItemsCatalogo->Add(itemCatalogo);
      }
   }
}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogoCab::SalvaItemsCatalogo(void)
{  //variaveis locais
   DIAG_CABO* diagCabo;
   TItemCatalogo  *itemCatalogo;
   TItemAtributo *atributo;

   //destroi todos diag_cabo do diagEqptoElem
   LimpaTList(diagEqpto->ListCFG);
   //preenche ela novamente com os items do catalogo (incluindo possiveis mudanças ou novos)
   for(int i = 0; i < listaItemsCatalogo->Count; i++)
   {  //pega o item
      itemCatalogo = (TItemCatalogo*) listaItemsCatalogo->Items[i];
      //cria o novo DIAG_CABO
      diagCabo = new DIAG_CABO;
      //seta o id de acordo com o contador
      diagCabo->id = i;  /* TODO : melhorar id */
      //indefine o tipo de cabo
      diagCabo->cfg_cabo = -1;
      //encontra o atributo codigo
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_COD);
      if(atributo)diagCabo->codigo = atributo->Valor;
      //encontra o atributo iadm
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_IADM);
      if(atributo)diagCabo->iadm_a = StrToDouble(atributo->Valor);
      //encontra o atributo vmax_kv
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_VMAX);
      if(atributo)diagCabo->vmax_kv = StrToDouble(atributo->Valor);
      //encontra o atributo vmin_kv
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_VMIN);
      if(atributo)diagCabo->vmin_kv = StrToDouble(atributo->Valor);

      //verifica se falta algum atributo de cabo definido por r e rmg
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_RESIST);
      if(!IsDoubleZero(StrToDouble(atributo->Valor)))
      {
         if(atributo)diagCabo->r_ohm_km = StrToDouble(atributo->Valor);
         atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_RAIO);
         if(!IsDoubleZero(StrToDouble(atributo->Valor)))
         {
            if(atributo)diagCabo->raio_m = StrToDouble(atributo->Valor);
            atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_RMG);
            if(!IsDoubleZero(StrToDouble(atributo->Valor)))
            {
               if(atributo)diagCabo->gmr_m = StrToDouble(atributo->Valor);
               //não faltou nenhum atributo, o cabo deve ser caboR_GMR
               diagCabo->cfg_cabo = caboR_GMR;  /* TODO : melhorar escolha tipo cabo */
            }
         }
      }
      //encontra o atributo pre-reunido
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_PREUN);
      if(atributo)diagCabo->preReunido = StringParaBool(atributo->Valor);
      if(diagCabo->preReunido) diagCabo->cfg_cabo = caboPRE_REUNIDO;
      //encontra o atributo z0_r_ohm_km
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_Z0R);
      if(atributo)diagCabo->z0_r_ohm_km = StrToDouble(atributo->Valor);
      //encontra o atributo z0_x_ohm_km
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_Z0X);
      if(atributo)diagCabo->z0_x_ohm_km = StrToDouble(atributo->Valor);
      //encontra o atributo z1_r_ohm_km
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_Z1R);
      if(atributo)diagCabo->z1_r_ohm_km = StrToDouble(atributo->Valor);
      //encontra o atributo z1_x_ohm_km
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_Z1X);
      if(atributo)diagCabo->z1_x_ohm_km = StrToDouble(atributo->Valor);
      //encontra o atributo c0_nf_km
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_Z0C);
      if(atributo)diagCabo->c0_nf_km = StrToDouble(atributo->Valor);
      //encontra o atributo c1_nf_km
      atributo = itemCatalogo->Get_ItemAtributo(cabATRIBUTO_Z1C);
      if(atributo)diagCabo->c1_nf_km = StrToDouble(atributo->Valor);
      //se não foi definido o tipo de cabo
      if(diagCabo->cfg_cabo == -1) diagCabo->cfg_cabo = caboZ0Z1;

      //adiciona o DIAG_CABO à lista
      diagEqpto->ListCFG->Add(diagCabo);
   }
   if(diagEqpto)
   {  //salva no txt a lista
      diagEqpto->SalvaArquivo();
   }
}

//---------------------------------------------------------------------------


