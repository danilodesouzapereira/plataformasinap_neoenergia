//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMultiPanel.h"
#include "..\Apl\VTApl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMultiPanel* __fastcall NewObjMultiPanel(void)
   {
   try{
      return (new TMultiPanel());
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMultiPanel::TMultiPanel(void)
   {
   //inicia dados
   PD.panel2_enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TMultiPanel::~TMultiPanel(void)
   {
   //destrói Panel2
   if (PD.panel2)
      {
      //PD.panel2->Owner  = NULL;
      PD.panel2->Parent = NULL;
      delete PD.panel2;
      PD.panel2 = NULL;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMultiPanel::ObjectTipoForm(TObject *obj)
   {
   //variáveis locais
   TMetaClass *obj_class;
   AnsiString obj_class_name;

   //verifica se o Object é um TForm diferente de TFormGraf
   obj_class = obj->ClassType();
   while(obj_class)
      {
      obj_class_name = obj_class->ClassName();
      if (obj_class_name.AnsiCompareIC("TFormGraf") == 0) return(false);
      if (obj_class_name.AnsiCompareIC("TForm") == 0) return(true);
      obj_class = obj_class->ClassParent();
      }
   //Object não é um  Form
   return(false);
   }

//---------------------------------------------------------------------------
TPanel* __fastcall TMultiPanel::PM_GetPanelAtivo(void)
   {
   return((PD.panel2_enabled) ? PD.panel2 : PD.panel1);
   }

//---------------------------------------------------------------------------
TPanel* __fastcall TMultiPanel::PM_GetPanel1(void)
   {
   return(PD.panel1);
   }

//---------------------------------------------------------------------------
TPanel* __fastcall TMultiPanel::PM_GetPanel2(void)
   {
   return(PD.panel2);
   }

//---------------------------------------------------------------------------
void __fastcall TMultiPanel::PM_SetPanel1(TPanel *panel1)
   {
   PD.panel1 = panel1;
   }

//---------------------------------------------------------------------------
void __fastcall TMultiPanel::PM_SetPanel2(TPanel *panel2)
   {
   PD.panel2 = panel2;
   }

//---------------------------------------------------------------------------
void __fastcall TMultiPanel::PM_SetPanel2_Enabled(bool enabled)
   {
   //verifica se há alteração de estado
   if (PD.panel2_enabled == enabled) return;
   //atualiza estado do Panel2
   PD.panel2_enabled = enabled;
   //verifica se Panel2 está habilitado
   if (PD.panel2_enabled)
      {//transfere Forms de Panel1 p/ Panel2
      TransfereForms(Panel1, Panel2);
      }
   else
      {//transfere Forms de Panel2 p/ Panel1
      TransfereForms(Panel2, Panel1);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMultiPanel::TransfereForms(TPanel *panel_orig, TPanel *panel_dest)
   {
   //variáveis locais
   int   top, left;
   TForm *form;
   
   //redefine Parent de todos Forms contidos bo Panel origem
   for (int n = panel_orig->ControlCount-1; n >= 0; n--)
      {
      if (ObjectTipoForm(panel_orig->Controls[n]))
         {
         form = (TForm*)panel_orig->Controls[n];
         //salva posição no Panel de origem
         top  = form->Top;
         left = form->Left;
         form->Parent = panel_dest;
         //define mesma posição no Panel destino
         form->Top  = top;
         form->Left = left;
         }
      }
   }

//---------------------------------------------------------------------------
//eof
