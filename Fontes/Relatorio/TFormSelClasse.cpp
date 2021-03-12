//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelClasse.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelClasse::TFormSelClasse(TComponent* Owner, VTApl *apl, TWinControl *parent)
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
   //inicia o CLBoxClasse
   CLBoxClasseInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormSelClasse::~TFormSelClasse(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisSEL) {delete lisSEL; lisSEL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::ActionSelAllExecute(TObject *Sender)
   {
   //marca todos os items do CLBox
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      CLBoxClasse->Checked[n] = true;
      }
   //monta novo conjunto de Classes selecionadas
   SelecionaClasses();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::ActionSelNoneExecute(TObject *Sender)
   {
   //desmarca todos os items do CLBox
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      CLBoxClasse->Checked[n] = false;
      }
   //monta novo conjunto de Classes selecionadas
   SelecionaClasses();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::CLBoxClasseClickCheck(TObject *Sender)
   {
   //monta novo conjunto de Classes selecionadas
   SelecionaClasses();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::CLBoxClasseInicia(void)
   {
   //variáveis locais
   int        index;
   VTClasse  *classe;
   VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
   TList     *lisCLASSE = classes->LisClasse();

   //reinicia CLBoxClasse
   CLBoxClasse->Clear();
   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      index = CLBoxClasse->Items->AddObject(classe->Codigo, classe);
      CLBoxClasse->Checked[index] = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::DefineActionOnSelClasse(TAction *ActionExterna)
   {
   //salva ponteiro de ActionExterna
   ActionOnSelClasse = ActionExterna;
   //monta conjunto de Classes selecionadas
   SelecionaClasses();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelClasse::SelecionaClasses(void)
   {
   //variáveis locais
   VTClasse *classe;

   //reinicia lista de Classes selecionadas
   lisSEL->Clear();
   //inclui na lista as Classes selecionadas no CLBoxClasse
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      if (CLBoxClasse->Checked[n])
         {
         classe = (VTClasse*)CLBoxClasse->Items->Objects[n];
         lisSEL->Add(classe);
         }
      }
   //executa Action externa que trata as classes selecionadas
   if (ActionOnSelClasse) ActionOnSelClasse->OnExecute(lisSEL);
   }

//---------------------------------------------------------------------------
//eof



