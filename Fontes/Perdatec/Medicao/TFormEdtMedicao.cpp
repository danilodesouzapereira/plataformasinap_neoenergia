//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <DLL_Inc\Funcao.h>
#include "TFormEdtMedicao.h"
#include "TMedicao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtMedicao::TFormEdtMedicao(TComponent* Owner, VTApl *apl_owner, TMedicao *medicao)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl     = apl_owner;
   this->medicao = medicao;
   //inicia CBoxRede
   CBoxTipoRedeInicia();
   //inicia CBoxRede
   CBoxRedeInicia();
   //insere valor de energia //DVK 2016.01.19
   EditEnergia->Text = DoubleToStr("%2.1f", medicao->energia.medida_mwhmes,',');
   //inicia TrackBar
   TrackBar->Position = int((1.0 - medicao->alocacao_perda_comercial.sdbt_pu) * TrackBar->Max);
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtMedicao::~TFormEdtMedicao(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::ActionConfirmaExecute(TObject *Sender)
   {
   //lê dados da Medicao

   if (!LeDadosMedicao()) return;
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::CBoxRedeInicia(void)
   {
   //variáveis locais
   int        index;
   VTRede     *rede;
   VTTipoRede *tiporede;
   VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList      *lisREDE = redes->LisRede();

   //reinicia CBoxRede c/ as Redes
   CBoxRede->Clear();
   //determina Tiporede selecionada
   if ((index = CBoxTipoRede->ItemIndex) < 0) return;
   tiporede = (VTTipoRede*)CBoxTipoRede->Items->Objects[index];
   if (lisREDE->Count == 0) return;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      if (rede->TipoRede != tiporede) continue;
      CBoxRede->Items->AddObject(rede->Codigo, rede);
      }
   //define ordem alfabética
   CBoxRede->Sorted = true;
   //verifica se existe rede da medição //DVK 2016.01.18
   if(medicao->rede == NULL) return;
   //pré-seleciona Rede da Medicao
   if ((index = CBoxRede->Items->IndexOfObject(medicao->rede)) < 0) index = 0;
   CBoxRede->ItemIndex = index;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::CBoxTipoRedeChange(TObject *Sender)
   {
   //reinicia CBoxRede
   CBoxRedeInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::CBoxTipoRedeInicia(void)
   {
   //variáveis locais
   int         index;
   VTTipoRede *tiporede;
   VTTipos    *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList      *lisTIPO = tipos->LisTipoRede();

   //desabilita evento OnChange
   CBoxTipoRede->OnChange = NULL;
   //reinicia CBoxTipoRede c/ as Redes
   CBoxTipoRede->Clear();
   if (lisTIPO->Count == 0) return;
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      CBoxTipoRede->Items->AddObject(tiporede->Codigo, tiporede);
      }
   //define ordem alfabética
   CBoxTipoRede->Sorted = true;
   //verifica se existe rede da medição //DVK 2016.01.18
   if(medicao->rede == NULL)
      {//reabilita evento OnChange
      CBoxTipoRede->OnChange = CBoxTipoRedeChange;
      return;
      }
   //pré-seleciona Rede da Medicao
   if ((index = CBoxTipoRede->Items->IndexOfObject(medicao->rede->TipoRede)) < 0) index = 0;
   CBoxTipoRede->ItemIndex = index;
   //reabilita evento OnChange
   CBoxTipoRede->OnChange = CBoxTipoRedeChange;
   }
 //---------------------------------------------------------------------------
double __fastcall TFormEdtMedicao::EnergiaCarga_mwhmes(TList *lisCARGA)
   {
   //variáveis locais
   double    energia_mwhmes = 0;
   VTCarga   *carga;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop para todas Cargas da lista
   for (int n = 0; n < lisCARGA->Count; n++)
	  {
	  carga = (VTCarga*)lisCARGA->Items[n];
	  energia_mwhmes += demanda->Edia_mwh[carga] * 30;
	  }
   return(energia_mwhmes);
   }
//---------------------------------------------------------------------------
bool __fastcall TFormEdtMedicao::LeDadosMedicao(void)
   {
   //variáveis locais
   int index;
   //verifica se foi selecionada uma Rede
   if ((index = CBoxRede->ItemIndex) < 0)
      {
      Aviso("Seleciona a rede da medição");
      return(false);
      }
   medicao->rede = (VTRede*)CBoxRede->Items->Objects[index];
   medicao->cod_rede = medicao->rede->Codigo; //DVK 2016.05.25
   try{//lê valor de energia
      medicao->energia.medida_mwhmes = StrToDouble(EditEnergia->Text);
      if ( medicao->energia.medida_mwhmes <= 0)
         {
         Erro("Valor de energia não pode ser nulo ou negativo");
         return(false);
         }
      }catch(Exception &e)
         {
         Erro("Valor de energia inválido");
         return(false);
		 }
   //valida a distribuicao
   ValidaDistribuicaoPerdas();
   //lê distribuição de perdas comerciais
   medicao->alocacao_perda_comercial.sdmt_pu = double(TrackBar->Position)/double(TrackBar->Max);
   //medicao->alocacao_perda_comercial.sdbt_pu = double(TrackBar->Max - TrackBar->Position)/double(TrackBar->Max);
   medicao->alocacao_perda_comercial.sdbt_pu = (1.0 - medicao->alocacao_perda_comercial.sdmt_pu);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::TrackBarChange(TObject *Sender)
   {

   //atualiza LabelPercSDMT
   LabelPercSDMT->Caption = IntToStr(TrackBar->Position);
   //atualiza LabelPercSDMT
   LabelPercSDBT->Caption = IntToStr(TrackBar->Max - TrackBar->Position);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtMedicao::ValidaDistribuicaoPerdas(void)
{
    //variáveis locais
   double eBT_mwhmes, eMT_mwhmes;
   AnsiString msg = "";
   bool existeBT, existeMT;

   existeBT = existeMT = true;
   eBT_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_BT_AJUS);
   eMT_mwhmes = EnergiaCarga_mwhmes(medicao->lisCAR_MT_AJUS);
   msg = "Valor de distribuição inválido, não existem cargas ajustaveis ";
   if (IsDoubleZero(eBT_mwhmes))
   {
		existeBT = false;
   }
   else if (IsDoubleZero(eMT_mwhmes))
   {
		existeMT = false;
   }
   //monta msg de erro
   if ((!existeBT) && (!existeMT))
   {
		msg += "tanto na MT quanto na BT.";
   }
   else if (!existeBT)
   {
		msg += "na BT.";
   }
   else if (!existeMT)
   {
		msg += "na MT.";
   }
   else
   {
	   msg = "";
   }

   if (!msg.IsEmpty())
   {
		Aviso(msg);
		//trata divisao
		if (!existeBT)
		{
			TrackBar->Position = TrackBar->Max;
			this->Refresh();
		}
		else if (!existeMT)
		{
			TrackBar->Position = 0;
			this->Refresh();
		}
   }
}
//---------------------------------------------------------------------------
//eof

