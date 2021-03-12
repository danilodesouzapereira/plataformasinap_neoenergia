//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#pragma resource "*.dfm"
#include "TFormEdtChvMS.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFormEdtChvMS::TFormEdtChvMS(TComponent* Owner, VTApl *apl, VTChave *chave, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->chave = chave;
   //configura opção de retirar
   ActionRetira->Visible = op_retira;
   //cria objetos
   lisCHAVE = new TList();
   //inicia lista com todas Chaves da rede
   IniciaLisChave();
   //inicia CBoxRede e CBoxTipoChave
   CBoxRedeInicia();
   CBoxTipoChaveInicia();
   //se necessário, define valores default p/ Chave
   DefValoresDefaultChave();
   //exibe dados da Chave
   ExibeDadosChave();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtChvMS::~TFormEdtChvMS(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisCHAVE) {delete lisCHAVE; lisCHAVE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::ActionCancelaExecute(TObject *Sender)
   {
   //cancela inserção da Chave
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados da Chave
   if (! LeDadosChave()) return;
   //confirma inserção da Chave
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::CBoxRedeInicia(void)
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
   //seleciona Rede do Chave
   CBoxRede->Sorted = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::CBoxTipoChaveInicia(void)
   {
   //variáveis locais
   VTTipoChave *tipochave;
   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //reinicia CBoxRede
   CBoxTipoChave->Clear();
   CBoxTipoChave->Sorted = false;
   if ((tipochave = tipos->ExisteTipoChave("DJ", chaveDJ)) != NULL)
      {
      CBoxTipoChave->Items->AddObject(tipochave->Codigo, tipochave);
      }
   if ((tipochave = tipos->ExisteTipoChave("RA", chaveRA)) != NULL)
      {
      CBoxTipoChave->Items->AddObject(tipochave->Codigo, tipochave);
      }
   if ((tipochave = tipos->ExisteTipoChave("indefinida", chaveINDEFINIDA)) != NULL)
      {
      CBoxTipoChave->Items->AddObject("Chave sem telecomando", tipochave);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::CBoxTipoChaveChange(TObject *Sender)
{
	ExibeDadosDisparo();
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::CheckBoxDisparoClick(TObject *Sender)
	{
    Application->ProcessMessages();
	PnlDelay->Visible = CheckBoxDisparo->Checked;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::DefValoresDefaultChave(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString codigo;
   //VTTipos   *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //verifica se a Chave ainda não definida
   if (chave->Codigo.IsEmpty())
      {//define código da Chave
      while(ExisteChave(codigo.sprintf("Chave %d", count++)) != NULL);
      chave->Codigo = codigo;
      }
   //verifica se a Rede da Chave está definida
   if (chave->rede == NULL)
      {
      chave->rede = SelecionaRede();
      }
   /*
   //verifica se o tipo da Chave está definido
   if (chave->TipoChave == NULL)
      {//define TipoChave
      chave->TipoChave = tipos->ExisteTipoChave("DJ", chaveDJ);
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::ExibeDadosChave(void)
   {
   //atualiza tela: dados da Chave
   EditCodigo->Text            = chave->Codigo;
   EditFabricante->Text        = chave->Extern_id;
   EditImax->Text              = DoubleToStr("%2.1f", chave->Inom_amp);
   CBoxRede->ItemIndex         = CBoxRede->Items->IndexOfObject(chave->rede);
   CBoxTipoChave->ItemIndex    = CBoxTipoChave->Items->IndexOfObject(chave->TipoChave);
   RadioGroupEstado->ItemIndex = (chave->Aberta) ? 0 : 1;

   ExibeDadosDisparo();
   //executa função que trata evento de seleção do estado
   RadioGroupEstadoClick(NULL);
   }
//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::ExibeDadosDisparo(void)
{
	if (TipoChaveIsTipoDisparo() == true)
		{
		PanelDisparo->Visible = true;
		this->Height = 228;
		//exibe informacao de disparo
		CheckBoxDisparo->Checked = chave->ProtecaoAtiva;
		if (chave->ProtecaoAtiva)
			{
			PnlDelay->Visible = true;
			EditDelay->Text   = IntToStr(chave->TecnoComunicacao);
			}
		else
			{
			PnlDelay->Visible	   = false;
			}
		}
   else
		{
		this->Height = 205;
		CheckBoxDisparo->Checked = false;
		PanelDisparo->Visible = false;
		}
}
//---------------------------------------------------------------------------
VTChave* __fastcall TFormEdtChvMS::ExisteChave(AnsiString codigo)
   {
   //variáveis locais
   VTChave *chave;

   for (int n = 0; n < lisCHAVE->Count; n++)
      {
      chave = (VTChave*)lisCHAVE->Items[n];
      if (chave->Codigo.AnsiCompareIC(codigo) == 0) return(chave);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //libera a memoria ocupada pelo form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::IniciaLisChave(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   lisCHAVE->Clear();
   redes->LisLigacao(lisCHAVE, eqptoCHAVE);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtChvMS::LeDadosChave(void)
   {
   //variáveis locais
   int         estado;
   double      i_amp;
   int         dtDisparo_s = 0;
   AnsiString  codigo;
   VTRede      *rede;
   VTTipoChave *tipochave;

   try{//lê código da Chave
      codigo = EditCodigo->Text;
      if (codigo.IsEmpty())
         {
         Aviso("Defina o código do chave");
         return(false);
         }
      //verifica se foi definida a Rede
      if (CBoxRede->ItemIndex < 0)
         {
         Aviso("Defina a rede do chave");
         return(false);
         }
      rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
      //verifica se foi definido o tipo da chave
      if(CBoxTipoChave->ItemIndex < 0)
         {
         Aviso("Defina o tipo da chave");
         return(false);
         }
      tipochave = (VTTipoChave*)CBoxTipoChave->Items->Objects[CBoxTipoChave->ItemIndex];
      /*
      if (ExisteChave(codigo) != NULL)
         {
         Aviso("Existe outro chave com o mesmo código");
         return(false);
         }
      */
      //verifica se foi definido estado da Chave
      if (RadioGroupEstado->ItemIndex < 0)
         {
         Aviso("Defina o estado da chave");
         return(false);
         }
      estado = (RadioGroupEstado->ItemIndex == 0) ? chvABERTA : chvFECHADA;
      //lê valor da corrente de ajuste
   if (! StrToDouble(EditImax->Text, i_amp))
       {
       Aviso("Ajuste com valor inválido");
       return(false);
       }
   if (i_amp < 0.)
       {
       Aviso("Ajuste com valor incorreto");
       return(false);
       }
   //dados do disparo
   if (CheckBoxDisparo->Checked)
      {
      if (! StrToInteger(EditDelay->Text, dtDisparo_s))
         {
         Aviso("Delay de disparo com valor inválido");
         return(false);
         }
      if (dtDisparo_s < 0)
         {
         Aviso("Delay de disparo com valor negativo");
         return(false);
         }
      }
      //atualiza Chave
      chave->Codigo    = codigo;
      chave->Inom_amp  = i_amp;
      chave->Extern_id = EditFabricante->Text;
      chave->Estado    = estado;
      //define TipoChave
      chave->TipoChave = tipochave;
      //define disparo
      chave->TecnoComunicacao = dtDisparo_s;
      chave->ProtecaoAtiva    = CheckBoxDisparo->Checked;
      //insere Chave na Rede
      if (chave->rede != rede)
         {//remove Chave da sua rede anterior
         if (chave->rede != NULL) chave->rede->RemoveLigacao(chave);
         //insere Chave na Rede selecionada
         rede->InsereLigacao(chave);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtChvMS::RadioGroupEstadoClick(TObject *Sender)
   {
   //redefine cor
   if (RadioGroupEstado->ItemIndex == 0) RadioGroupEstado->Color = clGreen;
   else                                  RadioGroupEstado->Color = clRed;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtChvMS::SelecionaRede(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se o Trafo já possui uma Rede definida
   if (chave->rede) return(chave->rede);
   //procura uma Rede que conecta as Barras da Chave
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((rede->ExisteBarra(chave->pbarra1))||
          (rede->ExisteBarra(chave->pbarra2))) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtChvMS::TipoChaveIsTipoDisparo(void)
{
   //variáveis locais
   VTTipoChave *tipochave;
   int index = -1;
   bool temDisparo = false;

   if ((index = CBoxTipoChave->ItemIndex) < 0)
      {
      temDisparo = false;
      }
   else
      {
      tipochave = (VTTipoChave*) CBoxTipoChave->Items->Objects[CBoxTipoChave->ItemIndex];
      if (tipochave != NULL)
         {
         if((tipochave->Tipo == chaveDJ) || (tipochave->Tipo == chaveRA))
            {
            temDisparo = true;
            }
         }
      }
   return temDisparo;
}
//---------------------------------------------------------------------------
//eof














