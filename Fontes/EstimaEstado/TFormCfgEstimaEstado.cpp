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

   //atualiza opções
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
   //lê configuração
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

   // Relatório de fluxo de potência
   CheckBoxRelFluxoPotencia->Checked = estima->RelFluxoPotencia;

   // Tipo de estimação
   if (estima->Tipo == estimaEQUILIBRADO) RadioGroupTipo->ItemIndex = 1;
   else                                   RadioGroupTipo->ItemIndex = 0;

   // Método de cálculo
   switch(estima->MetodoEE)
      {
      case meeCONVENCIONAL:   RadioGroupMetodoEE->ItemIndex = 0; break;
      case meeQR_FAST_GIVENS: RadioGroupMetodoEE->ItemIndex = 1; break;
      case meeQR_GIVENS:      RadioGroupMetodoEE->ItemIndex = 2; break;
      case meeQR_HOUSEHOLDER: RadioGroupMetodoEE->ItemIndex = 3; break;
      default:                RadioGroupMetodoEE->ItemIndex = 2; break;
      }

   // Ordenação de linhas (QR)
   // Proteção: ordenação de linhas implementada somente no método QR_GIVENS
   if(estima->MetodoEE != meeQR_GIVENS) estima->OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
   RadioGroupOrdenacaoLinhasQR->ItemIndex = estima->OrdenacaoLinhasQR;

   // Tipo de referência
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

   // Inclusão de perda no ferro
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
   // Proteção: verifica o patamar fornecido
   int pat = EditPatamar->Text.ToInt();
   if((pat < 1) || (pat > 24))
      {
      Aviso("Patamar inválido");
      return(false);
      }

   //proteção:verifica o tipo de estimação
   if (RadioGroupTipo->ItemIndex < 0)
      {
      Aviso("Selecione o tipo de estimação");
      return(false);
      }
   //proteção:verifica o método de estimação de estado
   if (RadioGroupMetodoEE->ItemIndex < 0)
      {
      Aviso("Selecione o método de estimação de estado");
      return(false);
      }
   //proteção:verifica o tipo de grupo das redes
   if (RadioGroupAnalise->ItemIndex < 0)
      {
      Aviso("Selecione o particionamento da análise");
      return(false);
      }

   // Salva o patamar
   estima->Patamar = pat;

   // Salva a configuração do relatório de fluxo de potência
   estima->RelFluxoPotencia = CheckBoxRelFluxoPotencia->Checked;

   //salva o tipo de estimação
   if (RadioGroupTipo->ItemIndex == 0) estima->Tipo = estimaDESEQUILIBRADO;
   else                                estima->Tipo = estimaEQUILIBRADO;

   //salva o método de estimação de estado
   if     (RadioGroupMetodoEE->ItemIndex == 0) estima->MetodoEE = meeCONVENCIONAL;
   else if(RadioGroupMetodoEE->ItemIndex == 1) estima->MetodoEE = meeQR_FAST_GIVENS;
   else if(RadioGroupMetodoEE->ItemIndex == 2) estima->MetodoEE = meeQR_GIVENS;
   else if(RadioGroupMetodoEE->ItemIndex == 3) estima->MetodoEE = meeQR_HOUSEHOLDER;
   else                                        estima->MetodoEE = meeQR_GIVENS;

   // Salva a ordenação de linhas (QR)
   // Proteção: ordenação de linhas implementada somente no método QR_GIVENS
   if(estima->MetodoEE != meeQR_GIVENS) RadioGroupOrdenacaoLinhasQR->ItemIndex = qrlSEM_ORDENACAO;
   estima->OrdenacaoLinhasQR = RadioGroupOrdenacaoLinhasQR->ItemIndex;

   // Salva a configuração do tipo de referência
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
   //salva inclusão de perda no ferro
   estima->PerdaFerroEnabled = CheckBoxPerdaFerro->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
//eof

