//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaDados.h"
#include "VTDados.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMontaDados* __fastcall NewObjMontaDados(VTApl *apl)
   {
   return(new TMontaDados(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMontaDados::TMontaDados(VTApl *apl)
   {
   //salva ponteiro
   this->apl = apl;
   //cria objetos locais
   dados = NewObjDados();
   }

//---------------------------------------------------------------------------
__fastcall TMontaDados::~TMontaDados(void)
   {
   //destró objetos locais
   if (dados) {delete dados; dados = NULL;}
   }

//---------------------------------------------------------------------------
VTDados* __fastcall TMontaDados::Executa(TList *lisEQP)
   {
   //variáveis locais
   int         eqpto_tipo;
   unsigned    show_default, show_eqpto;
   TList       *lisDADO;
   TStringList *StrList;
   VTEqpto     *eqpto;
   VTLegenda   *legenda;
   VTDado      *dado;

   try{//reinicia Dados
      dados->Clear();
      //verifica se a lista possui pelo menos um Eqpto
      if ((lisEQP == NULL)||(lisEQP->Count == 0)) return(NULL);
      //cria objetos
      lisDADO = new TList();
      legenda = DLL_NewObjLegenda(apl);
      //determina tipo do primeiro Eqpto da lista e atributos a serem exibidos
      eqpto        = (VTEqpto*)lisEQP->First();
      eqpto_tipo   = eqpto->Tipo();
      show_default = eqpto->ShowAsUnsigned;
      //monta dados p/ todos Eqpto do mesmo tipo
      for (int n = 0; n < lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         if (eqpto->Tipo() != eqpto_tipo) continue;
         //salva e redefine configuração de atributos do Eqpto
         show_eqpto            = eqpto->ShowAsUnsigned;
         eqpto->ShowAsUnsigned = show_default;
         //obtém conjunto de Dado do Eqpto
         LimpaTList(lisDADO);
         legenda->LisValoresEqpto(eqpto, lisDADO);
         //verifica se é o primeiro Eqpto
         if (n == 0)
            {//insere StringList c/ nomes dos atributos
            StrList = dados->InsereStringList();
            for (int i = 0; i < lisDADO->Count; i++)
               {
               dado = (VTDado*)lisDADO->Items[i];
               StrList->Add(dado->DescUnid());
               }
            }
         //insere um novo StringList em dados
         StrList = dados->InsereStringList();
         //loop p/ todos os Dados de lisDADO
         for (int i = 0; i < lisDADO->Count; i++)
            {
            dado = (VTDado*)lisDADO->Items[i];
            StrList->Add(dado->valor);
            }
         //restaura configuração de atributos do Eqpto
         eqpto->ShowAsUnsigned = show_eqpto;
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   //destrói objetos
   LimpaTList(lisDADO);
   delete lisDADO;
   delete legenda;
   
   return(dados);
   }

//---------------------------------------------------------------------------
VTDados* __fastcall TMontaDados::Executa(TDataSet *DataSet)
   {
   //variáveias locais
   AnsiString  valor;
   TStringList *StrList;

   try{//reinicia Dados
      dados->Clear();
      //insere StringList c/ nomes dos campos
      StrList = dados->InsereStringList();
      for (int n = 0; n < DataSet->Fields->Count; n++)
         {//verifica se o campo está visível
         if (! DataSet->Fields->Fields[n]->Visible) continue;
         StrList->Add(DataSet->Fields->Fields[n]->FieldName);
         }
      //posiciona no início do DataSet
      DataSet->First();
      for (DataSet->First(); ! DataSet->Eof; DataSet->Next())
         {//insere um novo StringList em dados
         StrList = dados->InsereStringList();
         //loop p/ todos os campos
         for (int n = 0; n < DataSet->Fields->Count; n++)
            {//verifica se o campo está visível
            if (! DataSet->Fields->Fields[n]->Visible) continue;
            //lê o valor do campo e insere em StrList
            switch(DataSet->Fields->Fields[n]->DataType)
               {
               case ftString:    //Character or string field
               case ftInteger:   //32-bit integer field
               case ftSmallint:  //16-bit integer field
               case ftWord:      //16-bit unsigned integer field
                  valor = DataSet->Fields->Fields[n]->AsString;
                  break;
               case ftBoolean:   //Boolean field
                  valor = (DataSet->Fields->Fields[n]->AsBoolean) ? "sim" : "não";
                  break;
               case ftFloat:     //Floating-point numeric field
                  valor = DoubleToStr("%4.3e", DataSet->Fields->Fields[n]->AsFloat);
                  break;
               case ftDate:      //Date field
                  valor =  DateToStr(DataSet->Fields->Fields[n]->AsDateTime);
                  break;
               case ftTime:      //Time field
                  valor =  TimeToStr(DataSet->Fields->Fields[n]->AsDateTime);
                  break;
               case ftDateTime:  //Date and time field
                  valor =  DateTimeToStr(DataSet->Fields->Fields[n]->AsDateTime);
                  break;
               default:
                  valor = DataSet->Fields->Fields[n]->AsString;
                  break;
               }
            StrList->Add(valor);
            }
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(dados);
   }

//---------------------------------------------------------------------------
VTDados* __fastcall TMontaDados::Executa(TListView *ListView)
   {
   //variáveias locais
   TListItem   *Item;
   TStringList *StrList;

   try{//reinicia Dados
      dados->Clear();
      //insere StringList c/ nomes dos campos
      StrList = dados->InsereStringList();
      for (int n = 0; n < ListView->Columns->Count; n++)
         {
         StrList->Add(ListView->Columns->Items[n]->Caption);
         }
      for (int n = 0; n < ListView->Items->Count; n++)
         {
         Item = ListView->Items->Item[n];
         //insere um novo StringList em dados
         StrList = dados->InsereStringList();
         //insere Caption
          StrList->Add(Item->Caption);
          //insere SubItems
         for (int i = 0; i < Item->SubItems->Count; i++)
            {//insere celula do StringGrid no  StringList
            StrList->Add(Item->SubItems->Strings[i]);
            }
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(dados);
   }

//---------------------------------------------------------------------------
VTDados* __fastcall TMontaDados::Executa(TStringGrid *StrGrid)
   {
   //variáveias locais
   TStringList *StrList;

   //reinicia Dados
   dados->Clear();
   for (int row = 0; row < StrGrid->RowCount; row++)
      {//insere um novo StringList em dados
      StrList = dados->InsereStringList();
      for (int col = 0; col < StrGrid->ColCount; col++)
         {//insere celula do StringGrid no  StringList
         StrList->Add(StrGrid->Cells[col][row]);
         }
      }
   return(dados);
   }


//---------------------------------------------------------------------------
//eof


