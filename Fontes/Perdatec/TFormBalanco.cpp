//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormBalanco.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormBalanco::TFormBalanco(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose)
   : TForm(Owner)
   {
   //salva objetos externos
   this->apl           = apl_owner;
   this->ActionOnClose = ActionOnClose;
   //inicia controle da ordenção do DBGrid
   ordenacao.column     = NULL;
   ordenacao.ascendente = true;
   //inicia conexão c/ a base de dados
   ConectaBaseDado();
   }

//---------------------------------------------------------------------------
__fastcall TFormBalanco::~TFormBalanco(void)
   {
   //desconecta da base de dados
   DesconectaBaseDado();
   //executa Action externa
   if (ActionOnClose) ActionOnClose->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormBalanco::ActionExportaExecute(TObject *Sender)
   {
   //
   }
//---------------------------------------------------------------------------
void __fastcall TFormBalanco::ActionFechaExecute(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormBalanco::FormClose(TObject *Sender, TCloseAction &Action)   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
//eof

