//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TFormSelPadrao.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelPadrao::TFormSelPadrao(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   StrList = new TStringList();
   campos  = new TStringList();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormSelPadrao::~TFormSelPadrao(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói objetos
   if (StrList) {delete StrList; StrList = NULL;}
   if (campos ) {delete campos ; campos  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::ActionConfirmaExecute(TObject *Sender)
   {
   //verifica se há um ListItem selecionado
   if (LViewPadrao->Selected == NULL)
      {
      Aviso("Selecione um padrão");
      return;
      }
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::ExtraiCampos(AnsiString txt, TStringList *campos)
   {
   //variáveis locais
   TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	WideString  ws_txt = txt;

   //reinicia StrinList campos
   campos->Clear();
   //verifica linha de comentário
   if ((txt[1] == '/')&&(txt[2] == '/')) return;
   if ((txt[1] == '/')&&(txt[2] == '*')) return;
   //inicia conjunto de caracteres separadores
   Separators << ';';
   //inicia conjunto de caracteres de espaçamento
   WhiteSpace << ' ';
   //separa campos do txt
   ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::Inicia(int tipo_eqpto)
   {
   //atualiza Caption
   Caption = Caption + " de " + VTEqpto::TipoEqpto_AsString(tipo_eqpto);
   //lê conjunto de padrões disponíveis para o tipo de Eqpto
   LePadrao(tipo_eqpto);
   //reinicia LBoxPadrao
   LViewPadraoInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::LePadrao(int tipo_eqpto)
   {
   //variáveis locais
   AnsiString arq_txt;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //reinicia StrList
   StrList->Clear();
   //determina arquivo c/ dados dos Eqptos padrão
   arq_txt = path->DirBase + "\\Dat\\Padrao\\" + VTEqpto::TipoEqpto_AsString(tipo_eqpto) + ".txt";
   //verifica se existe o arquivo
   if (! FileExists(arq_txt)) return;
   //carrega dados do arquivo padrão
   StrList->LoadFromFile(arq_txt);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelPadrao::LViewPadraoInicia(void)
   {
   //variáveis locais
   int       id;
   TListItem *Item;
   
   //reinicia LViewPadrao
   LViewPadrao->Items->Clear();
   //loop p/ todas linhas do arquivo carregadas em StrList
   for (int n = 0; n < StrList->Count; n++)
      {//extrai campos da linha
      ExtraiCampos(StrList->Strings[n], campos);
      if (campos->Count < 3) continue;
      try{//verifica se o primeiro campo é numérico
         id = StrToInt(campos->Strings[0]);
         //insere um novo ListItem
         Item          = LViewPadrao->Items->Add();
         Item->Data    = (void*)id;
         Item->Caption = campos->Strings[1];
         Item->SubItems->Add(campos->Strings[2]);
         }catch(Exception &e)
            {
            continue;
            }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormSelPadrao::PadraoSelecionado(int &id, AnsiString &codigo)
   {
   //proteção
   if (LViewPadrao->Selected == NULL) return(false);
   //retorna id e código
   id     = (int)(LViewPadrao->Selected->Data);
   codigo = LViewPadrao->Selected->Caption;
   return(true);
   }

//---------------------------------------------------------------------------
//eof

