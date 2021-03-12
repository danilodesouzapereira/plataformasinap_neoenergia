//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtZref.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTYref.h"
//#include "..\Rede\VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtZref::TFormEdtZref(TComponent* Owner, VTApl *apl, VTYref* yref)
   : TForm(Owner)
   {
   //salva ponteiros p/ objetos
   this->yref = yref;
   this->apl = apl;
   /*
   //verifica se deve habilitar edição de dados de planejamento
   if ((yref->planeja.ano > 0)&&(yref->planeja.ano >= rede->planeja.ano))
      {//cria Form p/ edição de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, yref);
      //habilita TableSheet de planejamento
      tsPlaneja->TabVisible = true;
      }
   else
      {//desabilita TableSheet de planejamento
      tsPlaneja->TabVisible = false;
      }
   */
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtZref::~TFormEdtZref(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói objetos criados
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::butCancelaClick(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::butConfirmaClick(TObject *Sender)
   {
   //lê dados do Eqpto
   if (! LeDadosZref()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(yref)) return;
      }
   */
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::butRetiraClick(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(yref);
      }
   */
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void _fastcall TFormEdtZref::ExibeDadosZref(void)
   {
   //variáveis locais
	strIMP z_pu;

   //obtém impedância em ohm
	yref->Z1_pu(z_pu);
   //exibe dados do Zref
   EditCodigo->Text        = yref->Codigo;
	edtR1->Text            = DoubleToStr("%4.3f", z_pu.r);
   edtX1->Text            = DoubleToStr("%4.3f", z_pu.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::FormShow(TObject *Sender)
   {
   //se necessário, define valores default p/ Barra
   DefValoresDefaultZref();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //exibe dados do Zref
   ExibeDadosZref();
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::DefValoresDefaultZref(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::IniciaParteFixaTela(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtZref::LeDadosZref(void)
   {
   //proteção: verifica se o Zref está incluído
   if (yref == NULL) return(true);
   try{//lê dados d0 Zref
      yref->Codigo = EditCodigo->Text;
      //yref->
      }catch(Exception &e)
         {
         Erro("Dados Inválidos do Zref");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtZref::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
//eof

