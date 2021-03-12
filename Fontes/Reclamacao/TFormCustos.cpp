//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCustos.h"
#include "..\..\Dll_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCustos::TFormCustos(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   campos = new TStringList();
   lines  = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TFormCustos::~TFormCustos(void)
   {
   //destrói objetos
   if (campos) {delete campos; campos = NULL;}
   if (lines)  {delete lines;  lines  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustos::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustos::ActionConfirmaExecute(TObject *Sender)
   {
   //proteção
   if (ListView->Selected == NULL)
      {
      Aviso("Selecione o tipo da obra");
      return;
      }
   ModalResult = true;
   }

//---------------------------------------------------------------------------
int __fastcall TFormCustos::ExtraiCampos(AnsiString linha)
   {
   //variáveis locais
   TSysCharSet separator;
   TSysCharSet whitespace;

   //reinicia campos
   campos->Clear();
   //verifica se linha de comentário
   if (linha.Length() < 2) return(0);
   if ((linha[1] == '/')&&(linha[2] == '/')) return(0);
   //define conjunto de caracteres separadores de campos
   separator << ';';
   //define conjunto de caracteres de espaçamento
   whitespace  << ' ';
   //xtrai os campos do string
//   ExtractStrings(separator, whitespace, linha.c_str(), campos);
   ExtraiStrings(linha.Trim(), ";", campos);
   return(campos->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCustos::LeArquivoObrasCustos(AnsiString arq_txt)
   {
   //variáveis locais
   TListItem *Item;

   //reinicia TListView
   ListView->Items->Clear();
   //proteção: verifica se o arquivo existe
   if (! FileExists(arq_txt)) return;
   //lê o arquivo no Strings
   lines->Clear();
   lines->LoadFromFile(arq_txt);
   //loop p/ todas linhas
   for (int nl = 0; nl < lines->Count; nl++)
      {//extrai campos da linha
      if (ExtraiCampos(lines->Strings[nl]) < 2) continue;
      //insere nova linha em ListView
      Item = ListView->Items->Add();
      Item->Caption = campos->Strings[0];
      Item->SubItems->Add(campos->Strings[1]);
      //campo de descrição é opcional
      if (campos->Count <= 2) continue;
      Item->SubItems->Add(campos->Strings[2]);
      }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCustos::ObraCodigo(void)
   {
   return(ListView->Selected->Caption);
   }

//---------------------------------------------------------------------------
double __fastcall TFormCustos::ObraCusto(void)
   {
   try{
      return(StrToDouble(ListView->Selected->SubItems->Strings[0]));
      }catch(Exception &e)
         {
         return(0.);
         }
   }

//---------------------------------------------------------------------------
//eof


