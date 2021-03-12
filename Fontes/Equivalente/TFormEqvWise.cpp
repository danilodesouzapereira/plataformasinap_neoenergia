//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TFormEqvWise.h"
#include "VTReduz.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEqvWise::TFormEqvWise(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //cria lista
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFormEqvWise::~TFormEqvWise(void)
   {
   //destr�i lista sem destruir objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqvWise::ActionCloseExecute(TObject *Sender)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqvWise::ActionEqvExecute(TObject *Sender)
   {
   //vari�veis locais
   VTReduz *reduz;

   //verifica se foram definidas as medi��es
   if (! ExisteMedicao())
      {
      if (Confirma("N�o h� medi��es definidas.",
                   "Deseja gerar a rede Wise sem considerar os pontos de medi��o ?") != IDYES) return;
      }
   //cria objeto
   if ((reduz = NewObjReduz(apl)) != NULL)
      {
      reduz->Executa();
      //destr�i objeto
      delete reduz;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEqvWise::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEqvWise::ExisteMedicao(void)
   {
   //vari�veis locais
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Medidores da rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);

   return(lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
//eof



