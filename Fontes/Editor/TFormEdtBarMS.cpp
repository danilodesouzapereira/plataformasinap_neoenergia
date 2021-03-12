//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "TFormEdtBarMS.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFormEdtBarMS::TFormEdtBarMS(TComponent* Owner, VTApl *apl, VTBarra *pbarra, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->pbarra = pbarra;
   //configura opção de retirar
   ActionRetira->Visible = op_retira;
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtBarMS::~TFormEdtBarMS(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::ActionCancelaExecute(TObject *Sender)
   {
   //cancela inserção da Barra
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados da Barra
   if (! LeDadosBarra()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(barra)) return;
      }
   */
   //confirma inserção da Barra
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(barra);
      }
   */
   //não permite exclusão de uma Barra de Corte
   if (ExisteCorte(pbarra))
      {
      Aviso("A Barra não pode ser excluída.\n"
            "Existe uma rede equivalente conectada a ela.");
      return;
      }
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::DefValoresDefaultBarra(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();
   VTBarra *ult_barra;

   //verifica se a Barra ainda não definida
   if (pbarra->vnom != 0.) return;
   //assume Vnom default
   pbarra->vnom = 13.8;
   //verifica se há Barras definidas
   if (lisBAR->Count > 0)
      {//determina última Barra
      ult_barra = (VTBarra*)lisBAR->Last();
      //assume Vnom da última Barra
      pbarra->vnom = ult_barra->vnom;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::ExibeDadosBarra(void)
   {
   //atualiza tela: dados da Barra
   EditCodigo->Text = pbarra->Codigo;
   EditVnom->Text   = DoubleToStr("%4.3f", pbarra->vnom);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBarMS::ExisteBarraSemCoordUtm(void)
   {
   //variáveis locais
   VTBarra *barra;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();

   //verifica se a rede ainda não tem nenhuma Barra
   if (lisBAR->Count == 0) return(false);
   //verifica se toda Barra possui coord.utm definida
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if ((barra->utm.x == 0)&&(barra->utm.y == 0)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBarMS::ExisteCorte(VTBarra *barra)
   {
   //variáveis locais
   VTCorte *corte;
   VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisCORTE = redes->LisCorte();

   //verifica se a Barra existe em algum Corte
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      if (corte->ExisteBarra(barra)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //libera a memoria ocupada pelo form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBarMS::FormShow(TObject *Sender)
   {
   //se necessário, define valores default p/ Barra
   DefValoresDefaultBarra();
   //exibe dados da Barra
   ExibeDadosBarra();
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBarMS::LeDadosBarra(void)
   {
   try{//lê dados da Barra
      pbarra->Codigo = EditCodigo->Text;
      pbarra->vnom   = StrToDouble(EditVnom->Text);
      //valida Vnom
      if (IsDoubleZero(pbarra->vnom))
         {
         Aviso("Tensão nominal inválida");
         return(false);
         }
      pbarra->utm.x  = -1;
      pbarra->utm.y  = -1;
      }catch(Exception &e)
         {
         Erro("Valor numérico c/ caracteres inválidos");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


