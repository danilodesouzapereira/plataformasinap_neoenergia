//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCorEnergia.h"
#include "VTCfgCor.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCorEnergia::TFormCorEnergia(TComponent *Owner, VTApl *apl_owner, TAction *Action)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   apl            = apl_owner;
   cfg_cor        = (VTCfgCor*)apl->GetObject(__classid(VTCfgCor));
   ActionAtualiza = Action;
   //inicia dados
   arq_infoset   = "MapaTematicoEnerga";
   }

//---------------------------------------------------------------------------
__fastcall TFormCorEnergia::~TFormCorEnergia(void)
   {
   //salva dados do form
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva cor utilizada
   geral->Infoset->Clear();
   geral->Infoset->AddInfo("cor_energia", int(cfg_cor->ENERGIA.corPadrao));
   geral->Infoset->SaveToFile(arq_infoset);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorEnergia::ExibeDados(void)
   {
   //inicia cor do Panel
   PanCor->Color = cfg_cor->ENERGIA.corPadrao;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorEnergia::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorEnergia::FormShow(TObject *Sender)
   {
   //inicia cor
   IniciaCor();
   //apresenta dados na tela
   ExibeDados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorEnergia::IniciaCor(void)
   {
   int     cor_energia;
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //assume padrão de cores default
   cor_energia = int(clRed);
   //verifica se há configuração de cores salva em infoset
   if (geral->Infoset->LoadFromFile(arq_infoset))
      {//lê configuração de cores do infoset
      geral->Infoset->GetInfo("cor_energia", cor_energia);
      }
   //inicia cor do Panel
   cfg_cor->ENERGIA.corPadrao = TColor(cor_energia);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorEnergia::PanCorClick(TObject *Sender)
   {
   if (ColorDialog->Execute())
      {
      cfg_cor->ENERGIA.corPadrao = ColorDialog->Color;
      ExibeDados();
      ActionAtualiza->Execute();
      }
   }
//---------------------------------------------------------------------------

