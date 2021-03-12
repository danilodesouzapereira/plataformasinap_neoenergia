//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgEstimaEstado.h"
#include "VTEstimaEstado.h"
#include "..\Apl\VTApl.h"
#include "..\Estimador\VTEstimador.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgEstimaEstado::TFormCfgEstimaEstado(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   estima    = (VTEstimaEstado*)apl_owner->GetObject(__classid(VTEstimaEstado));

   //atualiza op��es
   ExibeConfiguracao();
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgEstimaEstado::~TFormCfgEstimaEstado(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgEstimaEstado::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgEstimaEstado::ActionConfirmaExecute(TObject *Sender)
   {
   //l� configura��o
   if (LeConfiguracao())
      {//fecha o Form
      ModalResult = mrOk;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgEstimaEstado::ExibeConfiguracao(void)
   {
   // Patamar
   EditPatamar->Text = AnsiString(estima->Patamar);

   // Relat�rio de fluxo de pot�ncia
   CheckBoxRelFluxoPotencia->Checked = estima->RelFluxoPotencia;

   // Tipo de estima��o
   if (estima->Tipo == estimaEQUILIBRADO) RadioGroupTipo->ItemIndex = 1;
   else                                   RadioGroupTipo->ItemIndex = 0;

   // M�todo de c�lculo
   switch(estima->MetodoEE)
      {
      case meeCONVENCIONAL:   RadioGroupMetodoEE->ItemIndex = 0; break;
      case meeQR_FAST_GIVENS: RadioGroupMetodoEE->ItemIndex = 1; break;
      case meeQR_GIVENS:      RadioGroupMetodoEE->ItemIndex = 2; break;
      case meeQR_HOUSEHOLDER: RadioGroupMetodoEE->ItemIndex = 3; break;
      default:                RadioGroupMetodoEE->ItemIndex = 2; break;
      }

   // Ordena��o de linhas (QR)
   // Prote��o: ordena��o de linhas implementada somente no m�todo QR_GIVENS
   if(estima->MetodoEE != meeQR_GIVENS) estima->OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
   RadioGroupOrdenacaoLinhasQR->ItemIndex = estima->OrdenacaoLinhasQR;

   // Tipo de refer�ncia
   switch(estima->TipoBarraRef)
      {
      case brFIXOS_1THETA_0V: RadioGroupReferencia->ItemIndex = 0; break;
      case brFIXOS_1THETA_1V: RadioGroupReferencia->ItemIndex = 1; break;
      case brFIXOS_nTHETA_0V: RadioGroupReferencia->ItemIndex = 2; break;
      case brFIXOS_nTHETA_nV: RadioGroupReferencia->ItemIndex = 3; break;
      default:                RadioGroupReferencia->ItemIndex = 0; break;
      }

   // VDNM (Very Dishonest Newton Method)
   CheckBoxVDNM->Checked = estima->UtilizaVDNM;
   EdtNumMaxMontH->Text  = AnsiString(estima->NumMaxMontH);

   // Tipo de grupo de redes
   switch(estima->TipoGrupo)
      {
      case grupoUNICO:        RadioGroupAnalise->ItemIndex = 0; break;
      case grupoSUBESTACAO:   RadioGroupAnalise->ItemIndex = 1; break;
      case grupoPRIMARIO:     RadioGroupAnalise->ItemIndex = 2; break;
      case grupoINDEPENDENTE: RadioGroupAnalise->ItemIndex = 3; break;
      default:                RadioGroupAnalise->ItemIndex = 0; break;
      }

   // Inclus�o de perda no ferro
   CheckBoxPerdaFerro->Checked = estima->PerdaFerroEnabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgEstimaEstado::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgEstimaEstado::LeConfiguracao(void)
   {
   // Prote��o: verifica o patamar fornecido
   int pat = EditPatamar->Text.ToInt();
   if((pat < 1) || (pat > 24))
      {
      Aviso("Patamar inv�lido");
      return(false);
      }

   //prote��o:verifica o tipo de estima��o
   if (RadioGroupTipo->ItemIndex < 0)
      {
      Aviso("Selecione o tipo de estima��o");
      return(false);
      }
   //prote��o:verifica o m�todo de estima��o de estado
   if (RadioGroupMetodoEE->ItemIndex < 0)
      {
      Aviso("Selecione o m�todo de estima��o de estado");
      return(false);
      }
   //prote��o:verifica o tipo de grupo das redes
   if (RadioGroupAnalise->ItemIndex < 0)
      {
      Aviso("Selecione o particionamento da an�lise");
      return(false);
      }

   // Salva o patamar
   estima->Patamar = pat;

   // Salva a configura��o do relat�rio de fluxo de pot�ncia
   estima->RelFluxoPotencia = CheckBoxRelFluxoPotencia->Checked;

   //salva o tipo de estima��o
   if (RadioGroupTipo->ItemIndex == 0) estima->Tipo = estimaDESEQUILIBRADO;
   else                                estima->Tipo = estimaEQUILIBRADO;

   //salva o m�todo de estima��o de estado
   if     (RadioGroupMetodoEE->ItemIndex == 0) estima->MetodoEE = meeCONVENCIONAL;
   else if(RadioGroupMetodoEE->ItemIndex == 1) estima->MetodoEE = meeQR_FAST_GIVENS;
   else if(RadioGroupMetodoEE->ItemIndex == 2) estima->MetodoEE = meeQR_GIVENS;
   else if(RadioGroupMetodoEE->ItemIndex == 3) estima->MetodoEE = meeQR_HOUSEHOLDER;
   else                                        estima->MetodoEE = meeQR_GIVENS;

   // Salva a ordena��o de linhas (QR)
   // Prote��o: ordena��o de linhas implementada somente no m�todo QR_GIVENS
   if(estima->MetodoEE != meeQR_GIVENS) RadioGroupOrdenacaoLinhasQR->ItemIndex = qrlSEM_ORDENACAO;
   estima->OrdenacaoLinhasQR = RadioGroupOrdenacaoLinhasQR->ItemIndex;

   // Salva a configura��o do tipo de refer�ncia
   switch(RadioGroupReferencia->ItemIndex)
      {
      case 0:  estima->TipoBarraRef = brFIXOS_1THETA_0V; break;
      case 1:  estima->TipoBarraRef = brFIXOS_1THETA_1V; break;
      case 2:  estima->TipoBarraRef = brFIXOS_nTHETA_0V; break;
      case 3:  estima->TipoBarraRef = brFIXOS_nTHETA_nV; break;
      default: estima->TipoBarraRef = brFIXOS_1THETA_0V; break;
      }

   // Salva os dados de VDNM (Very Dishonest Newton Method)
   estima->UtilizaVDNM = CheckBoxVDNM->Checked;
   estima->NumMaxMontH = StrToInt(EdtNumMaxMontH->Text);

   //salva o tipo de grupo
   switch(RadioGroupAnalise->ItemIndex)
      {
      case 0: estima->TipoGrupo = grupoUNICO;         break;
      case 1: estima->TipoGrupo = grupoSUBESTACAO;    break;
      case 2: estima->TipoGrupo = grupoPRIMARIO;      break;
      case 3: estima->TipoGrupo = grupoINDEPENDENTE;  break;
      default:estima->TipoGrupo = grupoUNICO;         break;
      }
   //salva inclus�o de perda no ferro
   estima->PerdaFerroEnabled = CheckBoxPerdaFerro->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
//eof

