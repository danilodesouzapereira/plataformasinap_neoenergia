//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "VTDiagEqpto.h"
#include "TFormCatalogo.h"
#include "TItemAtributo.h"
#include "TItemCatalogo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCatalogo *FormCatalogo;
//---------------------------------------------------------------------------
   __fastcall TFormCatalogo::TFormCatalogo(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
{
   this->apl = apl;
   //cria lista para guardar os itens do catalogo
   listaItemsCatalogo = new TList();

}
//---------------------------------------------------------------------------
   __fastcall TFormCatalogo::~TFormCatalogo(void)
{
   //destroi lista e seu conteúdo
   if(listaItemsCatalogo){ LimpaTList(listaItemsCatalogo); delete listaItemsCatalogo; listaItemsCatalogo = NULL;}
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::ActionInsereTipoExecute(TObject *Sender)
{  //VAriaveis locais
   TItemCatalogo *novoItem;

   //Limpa o valueListEditor
   vlAtributos->Strings->Clear();
   //cria um novo item de catalogo
   novoItem = CriaItem();
   //insere item na lista de items
   listaItemsCatalogo->Add(novoItem);
   //insere item no listBox
   lbTipos->Items->AddObject(novoItem->Codigo, novoItem);
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::ActionRemoveTipoExecute(TObject *Sender)
{  //VAriaveis locais
   TItemCatalogo *itemSelecionado;
   int           index;
   //proteção
   if((index = lbTipos->ItemIndex) < 0){return;}
   //Limpa o valueListEditor
   vlAtributos->Strings->Clear();
   //guarda o ponteiro do item deletado
   itemSelecionado = (TItemCatalogo*)lbTipos->Items->Objects[index];
   //remove o item do listBox
   lbTipos->DeleteSelected();
   //remove o item da lista de items
   listaItemsCatalogo->RemoveItem(itemSelecionado,FromEnd);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCatalogo::BooleanoParaString(bool boolean)
{  //variaveis internas
   AnsiString booleano_ptbr;

   if (boolean == true) {booleano_ptbr = "Verdadeiro";}
   else {booleano_ptbr = "Falso";}

   return booleano_ptbr;
}
//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::butCancelaClick(TObject *Sender)
{  //variaveis locais
   TItemCatalogo *itemCatalogo;
   int           index;

   //proteção
   if((index = lbTipos->ItemIndex) < 0){return;}
   //se houve alteração, gera aviso
   if(alterado)
   {
      if((itemCatalogo = (TItemCatalogo*)lbTipos->Items->Objects[index]) == NULL){return;}
      if(Confirma("Houveram alterações que serão perdidas,","Confirma a operação?") == idYes)
      {
         ShowItem(itemCatalogo);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::butConfirmaClick(TObject *Sender)
{  //variaveis locais
   TItemCatalogo *itemCatalogo;
   TItemAtributo *itemAtributo;
   int           index;

   //proteção
   if((index = lbTipos->ItemIndex) < 0){return;}
   //obtem o objeto catalogo a ser editado
   if((itemCatalogo = (TItemCatalogo*)lbTipos->Items->Objects[index]) == NULL){return;}
   //recupera o ponteiro pro atributo com o código do item
   itemAtributo = itemCatalogo->codigo;
   //Altera o nome do item
   itemCatalogo->Codigo = vlAtributos->Values[itemAtributo->Nome];
   //altera o nome na lista do listbox
   lbTipos->Items->Strings[index] = itemCatalogo->Codigo;
   //varre todos atributos do item
   for(int na = 1; na < (vlAtributos->RowCount - 1); na++)  //menos 1 por causa do titulo e do codigo
   {  //recupera o ponteiro do atributo a ser editado
      itemAtributo = (TItemAtributo*)vlAtributos->Strings->Objects[na];
      //testa a existencia e a validade do dado
      if((itemAtributo)&&(VerificaValor(vlAtributos->Values[itemAtributo->Nome],itemAtributo->ValorDataType)))
      {  //atribui o valor correspondente ao "key"
         itemAtributo->Valor = vlAtributos->Values[itemAtributo->Nome];
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::FormClose(TObject *Sender, TCloseAction &Action)
{
   SalvaItemsCatalogo();
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::FormShow(TObject *Sender)
{
   IniciaListValue();
   IniciaListBox();
}

//---------------------------------------------------------------------------
//TItemCatalogo* __fastcall TFormCatalogo::CriaItem(void)
//{
//   //definido nas classes derivadas
//}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogo::Inicia(VTDiagEqpto *diagEqpto)
{
   // salva o ponteiro do diagEqptoElem, para salvar, ler txt...
   this->diagEqpto = diagEqpto;
   //carrega todos os itens salvos no txt
   IniciaListValue();
   IniciaListBox();
}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogo::IniciaListBox(void)
{  //variaveis locais
   TItemCatalogo *itemCatalogo;

   //limpa o listbox
   lbTipos->Clear();
   for(int ni = 0; ni < listaItemsCatalogo->Count; ni++)
   {
      itemCatalogo = (TItemCatalogo*)listaItemsCatalogo->Items[ni];
      //adiciona os items ao listbox
      lbTipos->AddItem(itemCatalogo->Codigo, itemCatalogo);
   }
}
//---------------------------------------------------------------------------
void  __fastcall TFormCatalogo::InsereAtributo(TItemAtributo *itemAtributo)
{  //variaveis locais
   AnsiString nome_valor;
   TStringList *listValores;
   int         index;

   if(itemAtributo != NULL)
   {
      //só insere se for visible
      if(!itemAtributo->Visible)return;
      nome_valor = itemAtributo->Nome + "=" +itemAtributo->Valor;
      //se o atributo NÃO for um grupo...
      if(!itemAtributo->Group)
      {
         index = vlAtributos->Strings->AddObject(nome_valor,itemAtributo);
         //caso necessite de um combobox
         if(itemAtributo->NeedCombo)
         {
            if(itemAtributo->ListaValoresCombo){ listValores = itemAtributo->ListaValoresCombo;}
            vlAtributos->ItemProps[index]->ReadOnly = true;
            vlAtributos->ItemProps[index]->PickList = listValores;
         }
      }
      else
      {//se o atributo for um grupo...
         nome_valor = " " + nome_valor;
         index = vlAtributos->Strings->AddObject(nome_valor, itemAtributo);
         vlAtributos->ItemProps[index]->EditStyle = esEllipsis;
         vlAtributos->ItemProps[index]->ReadOnly = true;
         if(itemAtributo->ListaAtributosFilhos)
         {
//            for(int naf = 0; naf < itemAtributo->ListaAtributosFilhos->Count; naf++)
//            {  //variaveis locais
//               TItemAtributo *atributo;
//
//               atributo = (TItemAtributo*) itemAtributo->ListaAtributosFilhos->Items[naf];
//               //recursivo!
//               InsereAtributo(atributo);
//            }
         }
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::lbTiposClick(TObject *Sender)
{  //variaveis locais
   TItemCatalogo *itemSelecionado;
   int            index;

   //determina o item selecionado
   index = lbTipos->ItemIndex;
   //recupera o ponteiro do item
   itemSelecionado = (TItemCatalogo*)lbTipos->Items->Objects[index];
   //exibe o item
   if(itemSelecionado){ShowItem(itemSelecionado);}
}

//---------------------------------------------------------------------------
void  __fastcall TFormCatalogo::ShowItem(TItemCatalogo *itemCatalogo)
{  //variaveis locais
   TItemAtributo *atributo;

   //inicializa o valueListEditor
   vlAtributos->Enabled = true;
   // Limpa o valuelisteditor
   vlAtributos->Strings->Clear();
   //Preenche o valuelisteditor
   for(int na = 0; na < itemCatalogo->listAtributos->Count; na++)
   {
      atributo = (TItemAtributo*) itemCatalogo->listAtributos->Items[na];
      InsereAtributo(atributo);
   }
   //inicializa o toolbar de edição
   alterado = false;
   toolBarEdicao->Enabled = true;
   butConfirma->Enabled = false;
   butCancela->Enabled = false;
   butDefault->Enabled = true;
}

//---------------------------------------------------------------------------
bool __fastcall TFormCatalogo::StringParaBool(AnsiString boolean_ptbr)
{
   //variaveis locais
   bool valor;
   if (boolean_ptbr.AnsiCompare("Verdadeiro") == 0)
      {
      valor = true;
      }
   else if (boolean_ptbr.AnsiCompare("Falso") == 0)
      {
      valor = false;
      }

   return valor;
}
//---------------------------------------------------------------------------
//void __fastcall TFormCatalogo::lvAtributosClick(TObject *Sender)
//{  //variaveis locais
//   TItemAtributo *atributo;
//   TListItem     *item;
//   AnsiString    valor;
//
//   item = lvAtributos->Selected;
//   //guarda o valor anterior do item selecionado
//   valor = item->SubItems->Strings[0];
//   atributo = (TItemAtributo*) item->Data;
//   if(atributo)
//   {  //verifica se é um atributo que possui combobox
//      if (atributo->NeedCombo)
//      {
//         cbValores->Visible = true;
//         IniciaComboBox(atributo->ListaValoresCombo,valor);
//         PosicionaComboBox(item);
//      }
//   }
//
//}

//---------------------------------------------------------------------------
bool  __fastcall TFormCatalogo::VerificaValor(AnsiString valor, int dataTYPE)
{
   //variavel local
   bool sucesso = true;

   switch (dataTYPE)
   {
      case dtINT:
         try
         {
            int integer;

            integer = StrToInt(valor);
         }
         catch (Exception &e)
         {
            Aviso("O valor editado deve ser um número inteiro.");
            sucesso = false;
         }

      case dtDOUBLE:
         try
         {
            double doubleValue;

            doubleValue = StrToDouble(valor);
         }
         catch (Exception &e)
         {
            Aviso("O valor editado deve ser um número.");
         }

      case dtANSISTRING:
         //check desnecessário
         break;

      case dtBOOL:
         try
         {
            bool  boolean;

            //boolean = StrToBool(valor) ;
            boolean = StringParaBool(valor) ;
         }
         catch (Exception &e)
         {
            Aviso("O valor editado deve ser ""true"" ou ""false"".");
            sucesso = false;
         }
      default:
         break;
      ;
   }
   return sucesso;
}

//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::vlAtributosEditButtonClick(TObject *Sender)
{
   //variaveis locais
   TItemCatalogo *itemSelecionado;
   int            index;
   TItemAtributo *itemAtributo;
   TItemAtributo *itemAtributoFilho;
   int ARow;

   //determina o item selecionado
   index = lbTipos->ItemIndex;
   //recupera o ponteiro do item
   itemSelecionado = (TItemCatalogo*)lbTipos->Items->Objects[index];
   //guarda qual atributo
   ARow = vlAtributos->Row;
   //proteção
   if(vlAtributos->Strings->Count < ARow){return;}
   //obtem o objeto itemAtributo da lista de objetos
   if((itemAtributo = (TItemAtributo*) vlAtributos->Strings->Objects[ARow - 1]) == NULL){return;}
   //verifica se tem filhos
   if(itemAtributo->ListaAtributosFilhos)
      {
      //troca a visualizacao dos filhos
      itemAtributo->FilhosVisible = !itemAtributo->FilhosVisible;
      for(int na = 0; na < itemAtributo->ListaAtributosFilhos->Count; na++)
         {
         itemAtributoFilho = (TItemAtributo*)itemAtributo->ListaAtributosFilhos->Items[na];
         //deixa os filhos visible ou nao
         itemAtributoFilho->Visible = itemAtributo->FilhosVisible;
         }
      }
   //exibe novamente o item
   if(itemSelecionado){ShowItem(itemSelecionado);}

}
//---------------------------------------------------------------------------
void __fastcall TFormCatalogo::vlAtributosGetEditText(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value)
{  //variaveis locais
   TItemAtributo *itemAtributo;

   //proteção
   if(vlAtributos->Strings->Count < ARow){return;}
   //obtem o objeto itemAtributo da lista de objetos
   if((itemAtributo = (TItemAtributo*) vlAtributos->Strings->Objects[ARow - 1]) == NULL){return;}
   //verifica se o valor condiz com o tipo esperado
   VerificaValor(Value, itemAtributo->ValorDataType);
   //verifica se houve mudança
   if(itemAtributo->Valor.AnsiCompareIC(Value) != 0){alterado = true;}
   //se foi alterado...
   //inicializa o toolbar de edição
   if(alterado)
   {
      toolBarEdicao->Enabled = true;
      butConfirma->Enabled = true;
      butCancela->Enabled = true;
      butDefault->Enabled = true;
   }


}

//---------------------------------------------------------------------------














