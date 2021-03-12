//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMT.h"
#include "Basico\TFormMapaTematico.h"
#include "AreaInfluencia\TFormMT_AIS.h"
#include "CentroCarga\TFormMT_CCS.h"
#include "CelPlan\TFormMT_Sinal.h"
#include "DensidadeCarga\TFormMT_DC.h"
#include "InterfaceCelPlanner\TFormCelPlanner.h"
#include "PerfilTensao\TFormMT_PT.h"
#include "PerfilFatpot\TFormMT_PFP.h"
#include "..\Apl\VTApl.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTRedes.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMT::TFormMT(TComponent *Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
	//variáveis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));

	//salva ponteiro
   apl = apl_owner;
   //insere ActionList
	sinap_child->FormStkAdd("SINAP: Theme Maps", ActionListMapaTematico);
   //insere ActionList
	sinap_child->FormStkAdd("SINAP: CelPlanner", ActionListCellPlan);
   }

//---------------------------------------------------------------------------
__fastcall TFormMT::~TFormMT(void)
   {
   //destrói eventual Form aberto
   CloseMapaTematico();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionCellPlannerExportaExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMT_DC aberto
   if ((form = ExisteForm("TFormCelPlanner")) == NULL)
      {
      form = new TFormCelPlanner(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMT_CCS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionCelPlannerExecute(TObject *Sender)
   {
   //variáveis locais
   TForm      *form;

   //verifica se a rede possui coordenadas
   if (! RedeGeoreferenciada())
      {
      Aviso("A rede não é geo-referenciada.\nO mapa de densidade de carga não pode ser gerado.");
      return;
      }
   //verifica se existe um TFormMT_DC aberto
   if ((form = ExisteForm("TFormMT_Sinal")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMT_CCS e exibe como janela normal
      form = new TFormMT_Sinal(this, apl, SelecionaParentParaJanela(), NULL);
      }
   //exibe  TFormMT_CCS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_AISExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se a rede possui coordenadas
   if (! RedeGeoreferenciada())
      {
      Aviso("A rede não é geo-referenciada.\nO mapa de área de influência não pode ser gerado.");
      return;
      }
   //verifica se existe um TFormMT_AIS aberto
   if ((form = ExisteForm("TFormMT_AIS")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMT_AIS e exibe como janela normal
      form = new TFormMT_AIS(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMT_AIS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMP_CCSExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se a rede possui coordenadas
   if (! RedeGeoreferenciada())
      {
      Aviso("A rede não é geo-referenciada.\nO mapa de centro de carga não pode ser gerado.");
      return;
      }
   //verifica se existe um TFormMT_CCS aberto
   if ((form = ExisteForm("TFormMT_CCS")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMT_CCS e exibe como janela normal
      form = new TFormMT_CCS(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMT_CCS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_DensidadeCargaExecute(TObject *Sender)
   {
   //variáveis locais
   TForm      *form;

   //verifica se a rede possui coordenadas
   if (! RedeGeoreferenciada())
      {
      Aviso("A rede não é geo-referenciada.\nO mapa de densidade de carga não pode ser gerado.");
      return;
      }
   //verifica se existe um TFormMT_DC aberto
   if ((form = ExisteForm("TFormMT_DC")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMT_CCS e exibe como janela normal
      form = new TFormMT_DC(this, apl, SelecionaParentParaJanela(), NULL);
      }
   //exibe  TFormMT_CCS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_EnergiaExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMapaTematico aberto
   if ((form = ExisteForm("TFormMapaTematico")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMapaTematico e exibe como janela normal
      form = new TFormMapaTematico(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMapaTematico
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_PerfilFatpotExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMT_PT aberto
   if ((form = ExisteForm("TFormMT_PFP")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMT_PFP e exibe como janela normal
      form = new TFormMT_PFP(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMT_CCS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_PerfilTensaoExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMT_PT aberto
   if ((form = ExisteForm("TFormMT_PT")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMT_CCS e exibe como janela normal
      form = new TFormMT_PT(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMT_CCS
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_TensaoExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMapaTematico aberto
   if ((form = ExisteForm("TFormMapaTematico")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMapaTematico e exibe como janela normal
      form = new TFormMapaTematico(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMapaTematico
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT::ActionMT_TempExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMapaTematico aberto
   if ((form = ExisteForm("TFormMapaTematico")) == NULL)
      {//destrói outros mapas temáticos
      CloseMapaTematico();
      //cria um novo TFormMapaTematico e exibe como janela normal
      form = new TFormMapaTematico(this, apl, SelecionaParentParaJanela());
      }
   //exibe  TFormMapaTematico
   if (form != NULL) form->Show();
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormMT::Atualiza(void)
   {
   //variáveis locais
   TForm *form;

   //atualiza o TForm temático aberto
   if ((form = ExisteForm("TFormMT_AIS")) != NULL)
      {
      ((TFormMT_AIS*)form)->Atualiza();
      }
   else if ((form = ExisteForm("TFormMT_CCS")) != NULL)
      {
      ((TFormMT_CCS*)form)->Atualiza();
      }
   else if ((form = ExisteForm("TFormMT_DC")) != NULL)
      {
      ((TFormMT_DC*)form)->Atualiza();
      }
   else if ((form = ExisteForm("TFormMT_PT")) != NULL)
      {
      ((TFormMT_PT*)form)->Atualiza();
      }
   else if ((form = ExisteForm("TFormMapaTematico")) != NULL)
      {
      ((TFormMapaTematico*)form)->Atualiza();
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TFormMT::CloseMapaTematico(void)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormMapaTematico abertos
   if ((form = ExisteForm("TFormMT_AIS"))       != NULL) delete form;
   if ((form = ExisteForm("TFormMT_CCS"))       != NULL) delete form;
   if ((form = ExisteForm("TFormMT_DC"))       != NULL) delete form;
   if ((form = ExisteForm("TFormMT_PT"))       != NULL) delete form;
   if ((form = ExisteForm("TFormMapaTematico")) != NULL) delete form;
   }

//---------------------------------------------------------------------------
TForm* __fastcall TFormMT::ExisteForm(AnsiString class_name_alvo)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormMapaTematico aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMapaTematico
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC(class_name_alvo) == 0)
         {
         if (Screen->Forms[n]->Owner == this) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMT::RedeGeoreferenciada(void)
   {
   //variáveis locais
   VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTArea  *area  = redes->AreaUtm_cm();

   //verifica se a área utm é válida
   if (IsDoubleZero(area->Width()))  return(false);
   if (IsDoubleZero(area->Height())) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
TWinControl* __fastcall TFormMT::SelecionaParentParaJanela(void)
   {
   //variáveis locais
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

   return(MultiPanel->PanelAtivo);
   }
   
//---------------------------------------------------------------------------
//eof



