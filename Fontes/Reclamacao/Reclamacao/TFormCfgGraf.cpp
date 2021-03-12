//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFormCfgGraf.h"
//---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTCfgGraf.h>
//---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormCfgGraf::TFormCfgGraf(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //define o ponteiro da classe
   this->cfg = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));
   this->apl = apl;

   //inicializa os valores
   RgView->ItemIndex = cfg->View;

   //parametros de tens�o
   EditDRC->Text = DoubleToStr("%2.1f", cfg->DRC);
   EditDRP->Text = DoubleToStr("%2.1f", cfg->DRP);

   //parametros de carregamento
   EditSmed->Text = DoubleToStr("%2.1f", cfg->Smed);
   EditSnom->Text = DoubleToStr("%2.1f", cfg->Snom);
   EditSmedExit(NULL);

   confirma = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgGraf::~TFormCfgGraf(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgGraf::ActionCancelarExecute(TObject *Sender)
   {
   //fecha o form
   confirma    = false;
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgGraf::ActionConfirmarExecute(TObject *Sender)
   {
   //valida, salva e fecha o form
   if(! SalvaCfg())  return;
   confirma    = false;
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgGraf::EditSmedExit(TObject *Sender)
   {
   //copia valor para EditSadeq
   EditSadeq->Text = EditSmed->Text;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgGraf::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgGraf::FormCloseQuery(TObject *Sender, bool &CanClose)
   {
   //vari�vel local
   int resp = IDNO;

   if(! confirma) {CanClose = true; return;}
   //verifica se deve salvar
   resp = Confirma("Deseja salvar os valores definidos ?", "", MB_YESNOCANCEL);
   switch(resp)
      {
      case IDYES:
         //atualiza valores no cfg
         if(! SalvaCfg())  CanClose = false;
         else               CanClose = true;
         return;
      case IDNO:
         //volta valores originais sem salvar
         CanClose = true;
         return;
      case IDCANCEL: //n�o salva a rede
      default:
         CanClose = false;
         return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgGraf::FormShow(TObject *Sender)
   {
   //nada
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgGraf::SalvaCfg(void)
   {
   //salva no infoset
   if(! SalvaDados()) return(false);
   //salva no cfg
   cfg->DRC  = valDRC;
   cfg->DRP  = valDRP;
   cfg->Smed = valSmed;
   cfg->Snom = valSnom;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgGraf::SalvaDados(void)
   {
   //vari�veis locais
   VTGeral        *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //valida antes de salvar
   if(! ValidaDados()) return(false);
   //salva informa��es no infoset
   if(! geral) return(false);
   try{
      geral->Infoset->Clear();
      geral->Infoset->AddInfo("valDRC",  valDRC);
      geral->Infoset->AddInfo("valDRP",  valDRP);
      geral->Infoset->AddInfo("valSmed", valSmed);
      geral->Infoset->AddInfo("valSnom", valSnom);
      geral->Infoset->SaveToFile("CfgGraf");
   }catch(Exception &e)
      {
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgGraf::ValidaDados(void)
   {
   try{//visualiza��o
      cfg->View = RgView->ItemIndex;
      //indices de tens�o
      if (! StrToDouble(EditDRC->Text, valDRC))
         {
         Aviso("Valor inv�lido do limite para DRC");
         return false;
         }
      if (! StrToDouble(EditDRP->Text, valDRP))
         {
         Aviso("Valor inv�lido do limite para DRP");
         return false;
         }
      //pot�ncia passante
      if (! StrToDouble(EditSmed->Text, valSmed))
         {
         Aviso("Valor inv�lido do limite para pot�ncia passante m�dia");
         return false;
         }
      if (! StrToDouble(EditSnom->Text, valSnom))
         {
         Aviso("Valor inv�lido do limite para pot�ncia passante nominal");
         return false;
         }
      //valida valores
      if(valSmed > valSnom)
         {
         Aviso("Valores de pot�ncia inv�lidos");
         return false;
         }
   }catch(Exception &e)
      {
      Aviso("Valor inv�lido !");
      return false;
      }
   return true;
   }

//---------------------------------------------------------------------------


