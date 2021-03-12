//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormExplorer.h"
#include "TFormCapacitorPorPotencia.h"
#include "TFormCargaPorTipo.h"
#include "TFormChavePorTipo.h"
#include "TFormRedePorHierarquia.h"
#include "TFormRedePorTipo.h"
#include "TFormRegPorTipoOp.h"
#include "TFormTrechoPorCargto.h"
#include "TAplExplorer.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEdt.h"
#include "..\Flow\VTFlow.h"
#include "..\Forms\TFormEqpto.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormExplorer::TFormExplorer(TForm* Owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(Owner, parent)
   {
   //variáveis locais
	VTEdita   *edita   = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

   try{//cria objetos necessários ao Explorer
      //apl     = new TAplExplorer(this, apl_owner);
      //salva ponteiro p/ objeto
      apl = apl_owner;
      //cria objetos
      lisFORM = new TList();
      //inicia ponteiro p/ Form atualmente ativo
      form_atual = NULL;
      //insere ActionRedeChanged em VTEdita
      if (edita) edita->ActionOnChangeRedeInsere(ActionRedeChanged);
      //força apresentação das redes ordenadas por tipo
      //ActionTViewTipoRede->Execute();
      //posiciona o Form
      //FormIniciaPosicao();
      //FKM 2015.11.27 como o actionEdtExecute faz nada...
      butEdt->Visible     = false;
      panelMouse->Visible = false;
      PanelLeft->Width    = butAlign->Width + 5;
      //FKM 2015.11.27
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormExplorer::~TFormExplorer(void)
   {
   //variáveis locais
   VTEdita   *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTGeral   *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral) geral->Infoset->SaveToFile(this);
   //retira ActionRedeChanged de VTEdita
   if (edita) edita->ActionOnChangeRedeRetira(ActionRedeChanged);
   //destrói Forms
   //if (FormRedePorTipo)       {delete FormRedePorTipo;       FormRedePorTipo       = NULL;}
   //if (FormRedePorHierarquia) {delete FormRedePorHierarquia; FormRedePorHierarquia = NULL;}
   if (lisFORM) {LimpaTList(lisFORM); delete lisFORM; lisFORM = NULL;}
   //destrói objetos
   if (edt)    {delete edt;    edt    = NULL;}
   //if (apl)    {delete apl;    apl    = NULL;}
   }


//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionEdtExecute(TObject *Sender)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------

void __fastcall TFormExplorer::ActionExibeCapPorPotenciaExecute(TObject *Sender)
   {
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormCapacitorPorPotencia
   form_atual = ExisteForm("TFormCapacitorPorPotencia");
   if (form_atual == NULL)
	  {//cria TFormCapacitorPorPotencia
	  form_atual = new TFormCapacitorPorPotencia(this, this, apl);
	  }
   //exibe Form
   if (form_atual != NULL)
	  {//atualiza Caption
	  Caption = form_atual->Caption;
	  form_atual->Show();
	  }
   }

//---------------------------------------------------------------------------

void __fastcall TFormExplorer::ActionExibeCargaPorTipoExecute(TObject *Sender)
   {
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormCargaPorTipo
   form_atual = ExisteForm("TFormCargaPorTipo");
   if (form_atual == NULL)
      {//cria TFormChavePorTipo
      form_atual = new TFormCargaPorTipo(this, this, apl);
      }
   //exibe Form
   if (form_atual != NULL)
      {//atualiza Caption
      Caption = form_atual->Caption;
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionExibeChavePorTipoExecute(TObject *Sender)
   {
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormChavePorTipo
   form_atual = ExisteForm("TFormChavePorTipo");
   if (form_atual == NULL)
      {//cria TFormChavePorTipo
      form_atual = new TFormChavePorTipo(this, this, apl);
      }
   //exibe Form
   if (form_atual != NULL)
      {//atualiza Caption
      Caption = form_atual->Caption;
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionExibeRedePorHierarquiaExecute(TObject *Sender)
   {
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormRedePorHierarquia
   form_atual = ExisteForm("TFormRedePorHierarquia");
   if (form_atual == NULL)
      {//cria TFormChavePorTipo
      form_atual = new TFormRedePorHierarquia(this, this, apl);
      }
   //exibe Form
   if (form_atual != NULL)
      {//atualiza Caption
      Caption = form_atual->Caption;
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionExibeRedePorTipoExecute(TObject *Sender)
   {
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormRedePorTipo
   form_atual = ExisteForm("TFormRedePorTipo");
   if (form_atual == NULL)
      {//cria TFormChavePorTipo
      form_atual = new TFormRedePorTipo(this, this, apl);
      }
   //exibe Form
   if (form_atual != NULL)
      {//atualiza Caption
      Caption = form_atual->Caption;
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionExibeReguladorPorTipoOpExecute(TObject *Sender)
{
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormRegPorTipoOp
   form_atual = ExisteForm("TFormRegPorTipoOp");
   if (form_atual == NULL)
	  {//cria TFormRegPorTipoOp
	  form_atual = new TFormRegPorTipoOp(this, this, apl);
	  }
   //exibe Form
   if (form_atual != NULL)
	  {//atualiza Caption
	  Caption = form_atual->Caption;
	  form_atual->Show();
	  }
}

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionExibeTrechoPorCargtoExecute(TObject *Sender)
   {
   //variáveis locais
   VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

   //verifica se o fluxo de potência foi calculado
   if (! flow->UpToDate)
      {
      Aviso("Execute o cálculo de fluxo de potência para obter o carregamento dos trechos");
      return;
      }
   //esconde Form atualmente ativo
   if (form_atual != NULL) form_atual->Hide();
   //verifica se já foi criado TFormTrechoPorCargto
   form_atual = ExisteForm("TFormTrechoPorCargto");
   if (form_atual == NULL)
      {//cria TFormTrechoPorCargto
      form_atual = new TFormTrechoPorCargto(this, this, apl);
      }
   //exibe Form
   if (form_atual != NULL)
      {//atualiza Caption
      Caption = form_atual->Caption;
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Explorer");
	if(formHelp) formHelp->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionRedeChangedExecute(TObject *Sender)
   {
   //OBS: Action executado remotamente sempre que ocorre uma mudança na Rede
   if (form_atual != NULL)
      {//força atualização do Form em exibição
      form_atual->Hide();
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::ActionSelEqptoExecute(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::Ativo(void)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::Atualiza(void)
   {
   //atualiza Form visível
   if (form_atual != NULL)
      {
      form_atual->Hide();
      form_atual->Show();
      }
   }

//---------------------------------------------------------------------------
TForm* __fastcall TFormExplorer::ExisteForm(AnsiString form_name)
   {
   //variáveis locais
   AnsiString obj_name;
   TObject    *obj;

   //retorna objeto da classe indicada
   for (int n = 0; n < lisFORM->Count; n++)
      {
      obj      = (TObject*)lisFORM->Items[n];
      obj_name = obj->ClassName();
      if (obj_name.AnsiCompareIC(form_name) == 0) return((TForm*)obj);
      }
   //não existe o Form
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TFormExplorer::FormShow(TObject *Sender)
	{
   //inicia objeto EdtExplorer
   ActionEdtExecute(NULL);
   //força apresentação das redes ordenadas por tipo
   ActionExibeRedePorTipo->Execute();
   //posiciona o Form
   FormIniciaPosicao();
   }


//---------------------------------------------------------------------------
//eof




