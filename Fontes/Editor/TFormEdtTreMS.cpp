//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#pragma resource "*.dfm"
#include "TFormEdtTreMS.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFormEdtTreMS::TFormEdtTreMS(TComponent* Owner, VTApl *apl, VTTrecho *trecho, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->trecho = trecho;
   //configura opção de retirar
   ActionRetira->Visible = op_retira;
   //cria objetos
   lisTRECHO = new TList();
   //inicia lista com todas Trechos da rede
   IniciaLisTrecho();
   //inicia CBoxRede
   CBoxRedeInicia();
   //se necessário, define valores default p/ Trecho
   DefValoresDefaultTrecho();
   //exibe dados da Trecho
   ExibeDadosTrecho();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTreMS::~TFormEdtTreMS(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisTRECHO) {delete lisTRECHO; lisTRECHO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::ActionCancelaExecute(TObject *Sender)
   {
   //cancela inserção da Trecho
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados da Trecho
   if (! LeDadosTrecho()) return;
   //confirma inserção da Trecho
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::CBoxRedeInicia(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia CBoxRede
   CBoxRede->Clear();
   CBoxRede->Sorted = false;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      CBoxRede->Items->AddObject(rede->Codigo, rede);
      }
   //seleciona Rede do Trecho
   CBoxRede->Sorted = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::DefValoresDefaultTrecho(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString codigo;
   VTArranjo  *arranjo;

   //verifica se a Trecho ainda não definida
   if (trecho->Codigo.IsEmpty())
      {//define código da Trecho
      while(ExisteTrecho(codigo.sprintf("Trecho %d", count++)) != NULL);
      trecho->Codigo = codigo;
      }
   //verifica se Trecho associado a Arranjo tipico
   if ((trecho->arranjo == NULL) || (trecho->ArranjoTipico))
      {//cria Arranjo próprio
      arranjo = DLL_NewObjArranjo();
      arranjo->Codigo   = "próprio";
      arranjo->TipoCfg  = arranjoZ0Z1;
      arranjo->Iadm_amp = 500;
      arranjo->z0.r     = 0.01;
      arranjo->z0.x     = 0.05;
      arranjo->z1       = arranjo->z0;
      trecho->DefineObjArranjo(arranjo);
      }
   //verifica se a rede do Trecho está definida
   if (trecho->rede == NULL)
      {
      trecho->rede = SelecionaRede();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::ExibeDadosTrecho(void)
   {
   //atualiza tela: dados da Trecho
   EditCodigo->Text    = trecho->Codigo;
   EditImax->Text      = DoubleToStr("%2.1f", trecho->arranjo->Iadm_amp);
   EditRseq1->Text      = DoubleToStr("%4.3f", trecho->arranjo->z1.r);
   EditXseq1->Text      = DoubleToStr("%4.3f", trecho->arranjo->z1.x);
   CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(trecho->rede);
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TFormEdtTreMS::ExisteTrecho(AnsiString codigo)
   {
   //variáveis locais
   VTTrecho *trecho;

   for (int n = 0; n < lisTRECHO->Count; n++)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[n];
      if (trecho->Codigo.AnsiCompareIC(codigo) == 0) return(trecho);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //libera a memoria ocupada pelo form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTreMS::IniciaLisTrecho(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   lisTRECHO->Clear();
   redes->LisLigacao(lisTRECHO, eqptoTRECHO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTreMS::LeDadosTrecho(void)
   {
   //variáveis locais
   double     i_amp, r1, x1;
   AnsiString codigo;
   VTRede     *rede;

   try{//verifica se foi definida a Rede
      if (CBoxRede->ItemIndex < 0)
         {
         Aviso("Defina a rede do trecho");
         return(false);
         }
      rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
      //lê dados da Trecho
      codigo = EditCodigo->Text;
      if (codigo.IsEmpty())
         {
         Aviso("Defina o código do trecho");
         return(false);
         }
      //Rseq1
      if (! StrToDouble(EditRseq1->Text, r1))
         {
         Aviso("Resistência com valor inválido");
         return(false);
         }
      if (r1 < 0.)
         {
         Aviso("Resistência com valor incorreto");
         return(false);
         }
      //Xse1
      if (! StrToDouble(EditXseq1->Text, x1))
         {
         Aviso("Reatância com valor inválido");
         return(false);
         }
      //Imax
      if (! StrToDouble(EditImax->Text, i_amp))
         {
         Aviso("Corrente máxima com valor inválido");
         return(false);
         }
      if (i_amp < 0.)
         {
         Aviso("Corrente máxima com valor incorreto");
         return(false);
         }
      //atualiza Trecho
      trecho->Codigo            = codigo;
      trecho->arranjo->Iadm_amp = i_amp;
      trecho->Comprimento_km    = 1.0;
      trecho->arranjo->z1.r     = r1;
      trecho->arranjo->z1.x     = x1;
      trecho->arranjo->z0       = trecho->arranjo->z1;
      //insere Trecho na Rede
      if (trecho->rede != rede)
         {//remove Trecho da sua rede anterior
         if (trecho->rede != NULL) trecho->rede->RemoveLigacao(trecho);
         //insere Trecho na Rede selecionada
         rede->InsereLigacao(trecho);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtTreMS::SelecionaRede(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se o Trafo já possui uma Rede definida
   if (trecho->rede) return(trecho->rede);
   //procura uma Rede que conecta as Barras da Chave
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((rede->ExisteBarra(trecho->pbarra1))||
          (rede->ExisteBarra(trecho->pbarra2))) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


