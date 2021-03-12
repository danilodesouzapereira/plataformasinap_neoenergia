//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelArranjo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelArranjo::TFormSelArranjo(TComponent* Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl            = apl;
   //inser Form em seu Parent
   if (parent)
      {
      Parent      = parent;
      Align       = alClient;
      BorderStyle = bsNone;
      }
   //cria listas
   lisSEL   = new TList();
   //inicia o CLBoxArranjo
   CLBoxArranjoInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormSelArranjo::~TFormSelArranjo(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisSEL) {delete lisSEL; lisSEL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::ActionSelAllExecute(TObject *Sender)
   {
   //marca todos os items do CLBox
   for (int n = 0; n < CLBoxArranjo->Items->Count; n++)
      {
      CLBoxArranjo->Checked[n] = true;
      }
   //monta novo conjunto de Arranjos selecionadas
   SelecionaArranjos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::ActionSelNoneExecute(TObject *Sender)
   {
   //desmarca todos os items do CLBox
   for (int n = 0; n < CLBoxArranjo->Items->Count; n++)
      {
      CLBoxArranjo->Checked[n] = false;
      }
   //monta novo conjunto de Arranjos selecionadas
   SelecionaArranjos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::CLBoxArranjoClickCheck(TObject *Sender)
   {
   //monta novo conjunto de Arranjos selecionadas
   SelecionaArranjos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::CLBoxArranjoInicia(void)
   {
   //variáveis locais
   int        index;
   VTArranjo  *arranjo;

   //determina conjunto de Arranjos em uso na rede
   IniciaLisArranjodaRede(lisSEL);
   //reinicia CLBoxArranjo
   CLBoxArranjo->Clear();
   for (int n = 0; n < lisSEL->Count; n++)
      {
      arranjo = (VTArranjo*)lisSEL->Items[n];
      index = CLBoxArranjo->Items->AddObject(arranjo->Codigo, arranjo);
      CLBoxArranjo->Checked[index] = true;
      }
   //reinicia lisSEL
   lisSEL->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::DefineActionOnSelArranjo(TAction *ActionExterna)
   {
   //salva ponteiro de ActionExterna
   ActionOnSelArranjo = ActionExterna;
   //monta conjunto de Arranjos selecionadas
   SelecionaArranjos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::IniciaLisArranjodaRede(TList *lisARRANJO)
   {
   //variáveis locais
   TList    *lisTRECHO;
   VTTrecho *trecho;
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisARRANJO
   lisARRANJO->Clear();
   //cria lista local
   lisTRECHO = new TList();
   //obtém lista de Trechos das Redes
   redes->LisEqpto(lisTRECHO, eqptoTRECHO);
   //mantém em lisTRECHO somente os Trechos cujos Arranjos foram selecionadas
   for (int n = lisTRECHO->Count-1; n >= 0; n--)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[n];
      if (lisARRANJO->IndexOf(trecho->arranjo) < 0)
         {
         lisARRANJO->Add(trecho->arranjo);
         }
      }
   //destrói lista local
   delete lisTRECHO;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArranjo::SelecionaArranjos(void)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //reinicia lista de Arranjos selecionadas
   lisSEL->Clear();
   //inclui na lista as Arranjos selecionadas no CLBoxArranjo
   for (int n = 0; n < CLBoxArranjo->Items->Count; n++)
      {
      if (CLBoxArranjo->Checked[n])
         {
         arranjo = (VTArranjo*)CLBoxArranjo->Items->Objects[n];
         lisSEL->Add(arranjo);
         }
      }
   //executa Action externa que trata as arranjos selecionadas
   if (ActionOnSelArranjo) ActionOnSelArranjo->OnExecute(lisSEL);
   }

//---------------------------------------------------------------------------
//eof



