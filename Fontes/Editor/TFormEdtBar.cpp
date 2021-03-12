//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "TFormEdtBar.h"
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
__fastcall TFormEdtBar::TFormEdtBar(TComponent* Owner, VTApl *apl, VTBarra *pbarra, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->pbarra = pbarra;
   //configura op��o de retirar
   ActionRetira->Visible = op_retira;
   //atualiza StatusBar c/ aviso de barras sem coordenadas utm
   if (ExisteBarraSemCoordUtm())
      {StatusBar->SimpleText = "Rede sem coordenadas georeferenciadas";}
   else
      {StatusBar->SimpleText = "Rede com coordenadas georeferenciadas";}
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtBar::~TFormEdtBar(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::ActionCancelaExecute(TObject *Sender)
   {
   //cancela inser��o da Barra
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados da Barra
   if (! LeDadosBarra()) return;
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(barra)) return;
      }
   */
   //confirma inser��o da Barra
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(barra);
      }
   */
   //n�o permite exclus�o de uma Barra de Corte
   if (ExisteCorte(pbarra))
      {
      Aviso("A Barra n�o pode ser exclu�da.\n"
            "Existe uma rede equivalente conectada a ela.");
      return;
      }
   //fecha o form indicando exclus�o do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::DefValoresDefaultBarra(void)
   {
   //vari�veis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();
   VTBarra *ult_barra;

   //verifica se a Barra ainda n�o definida
   if (pbarra->vnom != 0.) return;
   //assume Vnom default
   pbarra->vnom = 13.8;
   //verifica se h� Barras definidas
   if (lisBAR->Count > 0)
      {//determina �ltima Barra
      ult_barra = (VTBarra*)lisBAR->Last();
      //assume Vnom da �ltima Barra
      pbarra->vnom = ult_barra->vnom;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::ExibeDadosBarra(void)
   {
   //atualiza tela: dados da Barra
   EditCodigo->Text = pbarra->Codigo;
   EditVnom->Text   = DoubleToStr("%4.3f", pbarra->vnom);
   EditCoordX->Text = DoubleToStr("%3.2f", double(pbarra->utm.x)/100.);
   EditCoordY->Text = DoubleToStr("%3.2f", double(pbarra->utm.y)/100.);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBar::ExisteBarraSemCoordUtm(void)
   {
   //vari�veis locais
   VTBarra *barra;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();

   //verifica se a rede ainda n�o tem nenhuma Barra
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
bool __fastcall TFormEdtBar::ExisteCorte(VTBarra *barra)
   {
   //vari�veis locais
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
void __fastcall TFormEdtBar::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //libera a memoria ocupada pelo form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtBar::FormShow(TObject *Sender)
   {
   //se necess�rio, define valores default p/ Barra
   DefValoresDefaultBarra();
   //exibe dados da Barra
   ExibeDadosBarra();
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBar::LeDadosBarra(void)
   {
   try{//l� dados da Barra
      pbarra->Codigo = EditCodigo->Text;
      pbarra->vnom   = StrToDouble(EditVnom->Text);
      pbarra->utm.x  = int(StrToDouble(EditCoordX->Text)) * 100;
      pbarra->utm.y  = int(StrToDouble(EditCoordY->Text)) * 100;
      }catch(Exception &e)
         {
         Erro("Valor num�rico c/ caracteres inv�lidos");
         return(false);
         }
   //valida Vnom
   if (IsDoubleZero(pbarra->vnom))
      {
      Erro("Tens�o nominal nula");
      return(false);
      }
   //verifica coordenadas
   if ((pbarra->utm.x == 0)||(pbarra->utm.y == 0))
      {//coordenadas indefinidas: gera aviso apenas se for a primeira Barra c/ coord.inv�lida
      if (! ExisteBarraSemCoordUtm())
         {
         if (Confirma("As coordenadas utm da barra n�o est�o definidas.\n"
                      "A falta desta informa��o n�o permite a visualiza��o geo-referenciada da rede.",
                      "Deseja finalizar edi��o da barra ?") != IDYES) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


