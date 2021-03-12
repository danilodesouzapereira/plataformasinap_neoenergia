//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEvolucao.h"
#include "VTResumo.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Hist\VTBDHist.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEvolucao::TFormEvolucao(TComponent* Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   lisRESUMO = new TList();
   //insere Form no parent definido
   if (parent)
      {
      Parent = parent;
      Align = alClient;
      }
   }

//---------------------------------------------------------------------------
__fastcall TFormEvolucao::~TFormEvolucao(void)
   {
   //destrói lista e seus objeto
   if (lisRESUMO) {LimpaTList(lisRESUMO); delete lisRESUMO; lisRESUMO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEvolucao::IniciaEvolucaoEmpresa(void)
   {

   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEvolucao::IniciaEvolucaoRede(AnsiString codigo_rede)
   {
   //variáveis locais
   VTBD* bd = (VTBD*)apl->GetObject(__classid(VTBD));

   if (!bd) return;

   //reinicia lisRESUMO
   LimpaTList(lisRESUMO);
   //obtém todos Resumo da rede disponíveis na base de dados
   bd->Hist->LeResumoPerdaUmaRede(codigo_rede, lisRESUMO);
   //atualiza LView
   LViewPerdaInicia();
   LViewBalancoInicia();
   LViewEqptoInicia();
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEvolucao::IniciaEvolucaoRede(AnsiString codigo_rede)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormEvolucao::LViewBalancoInicia(void)
   {
   //variáveis locais
   VTResumo  *resumo;
   TListItem *Item;
   TListView *LView = LViewBalanco;

   //reinicia LView
   LView->Items->Clear();
   //loop p/ todos os resumos
   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      resumo = (VTResumo*)lisRESUMO->Items[n];
      //insere novo ListItem
      Item = LView->Items->Add();
      //data do Resumo
      Item->Caption = resumo->rede.Data;
      Item->SubItems->Add(resumo->rede.Codigo);
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaSuprimento));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaGerador));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaEntregueExt + resumo->balanco.EnergiaEntregueInt));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaCarga));
      //Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaPerdaFluxo));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaPerdaTotal));
      /*
      //perda porcentual em relação ao total de energia entrada na rede
      if (IsDoubleZero(resumo->balanco.EnergiaPerdaTotalPerc))
         Item->SubItems->Add("-");
      else
         Item->SubItems->Add(DoubleToStr("%3.2f", resumo->balanco.EnergiaPerdaTotalPerc));
      */
       }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEvolucao::LViewEqptoInicia(void)
   {
   //variáveis locais
   VTResumo  *resumo;
   TListItem *Item;
   TListView *LView = LViewEqpto;

   //reinicia LView
   LView->Items->Clear();
   //loop p/ todos os resumos
   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      resumo = (VTResumo*)lisRESUMO->Items[n];
      //insere novo ListItem
      Item = LView->Items->Add();
      //data do Resumo
      Item->Caption = resumo->rede.Data;
      Item->SubItems->Add(resumo->rede.Codigo);
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeRede));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeTrafo));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->rede.QtdeCabo / 1000.));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeRegulador));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeCapacitor));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeReator));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsA4));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsRes));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsCom));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsInd));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsRur));
      Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsOut));
       }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEvolucao::LViewPerdaInicia(void)
   {
   //variáveis locais
   VTResumo  *resumo;
   TListItem *Item;
   TListView *LView = LViewPerda;

   //reinicia LView
   LView->Items->Clear();
   //loop p/ todos os resumos
   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      resumo = (VTResumo*)lisRESUMO->Items[n];
      //insere novo ListItem
      Item = LView->Items->Add();
      //data do Resumo
      Item->Caption = resumo->rede.Data;
      Item->SubItems->Add(resumo->rede.Codigo);
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaPerdaTotal));
      //perdas calculadas pelo fluxo
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaTrafoCobre));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaCabo));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaPerdaFluxo));
      //perdas não calculadas pelo fluxo
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaTrafoFerro));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaRegulador));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaCapacitor));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaReator));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaRamal));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaMedidor));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->perda.PerdaOutros));
      Item->SubItems->Add(DoubleToStr("%4.3f", resumo->balanco.EnergiaPerdaTotal - resumo->balanco.EnergiaPerdaFluxo));
      }
   }

//---------------------------------------------------------------------------
//eof