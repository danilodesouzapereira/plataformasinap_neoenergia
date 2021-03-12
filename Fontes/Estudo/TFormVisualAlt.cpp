//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormVisualAlt.h"
#include "..\Apl\VTApl.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTMercado.h"
#include "..\Obra\VTObra.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Editor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//__fastcall TFormVisualAlt::TFormVisualAlt(TComponent* Owner, VTApl *apl, VTEstudo *estudo)
__fastcall TFormVisualAlt::TFormVisualAlt(TForm* Owner, VTApl *apl, VTEstudo *estudo)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   this->estudo = estudo;
   this->Owner = Owner;
   //inicia
   Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormVisualAlt::~TFormVisualAlt(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);

   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::ActionFecharExecute(TObject *Sender)
   {
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::CBoxAnoChange(TObject *Sender)
   {
   TreeViewChange(NULL, NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //redefine a rede com sua configuração original
//   VTAlternativa *alternativa = TreeViewAlternativaSelecionada();
//   if(alternativa != NULL) alternativa->ReconfiguraRedeSemObra();
   //DVK 2015.01.06
   //mostra panel
   PanelAviso->Visible = true;
   this->Refresh();
   //redefine Alternativa ativa como rede original
   estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
   this->Redraw();
   //mostra form Estudo
   if ((Owner != NULL) && (Owner->InheritsFrom(__classid(TForm))))
	   {
	   Owner->Show();
	   }
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //define posição da janela flutuante
   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::Inicia(void)
   {
   //esconde panel
   PanelAviso->Visible = false;
   //inicia treeview com alternativas
   TreeViewInicia();
   for(int ano = estudo->AnoInicial; ano <= estudo->AnoFinal; ano++)
      {//preenche cbox com os anos do estudo
      CBoxAno->Items->Add(AnsiString(ano));
      }
   CBoxAno->ItemIndex = 0;
   }
//--------------------------------------------------------------------------
void __fastcall TFormVisualAlt::Redraw(void)
{
	VTGrafico  *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede;

	//exibe todas as redes
	lisRede = redes->LisRede();
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*) lisRede->Items[nr];
		rede->Show[eqp_VISIBLE] = true;
	}
	if (grafico)
	{
		grafico->Redraw();
	}
}
//---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormVisualAlt::TreeViewAlternativaSelecionada(void)
   {
   //variáveis locais
   TTreeNode *node = TreeView->Selected;

   //proteção
   if (node == NULL) return(NULL);
   //determina TTreeNode de Level 0
   while (node->Level > 1) node = node->Parent;
   //retorna Alternativa associada ao TreeNode
   if (node->Level == 1) return((VTAlternativa*)node->Data);
   //não uma Alternativa selecionada
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::TreeViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   int           ano_rede;
   VTAlternativa *alternativa;
   VTObra        *obra;
	VTFlow        *flow        = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTMercado     *mercado     = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTSinapChild  *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   //mostra panel
   PanelAviso->Visible = true;
   this->Refresh();
   //detemina Alternativa selecionada
   if ((alternativa  = TreeViewAlternativaSelecionada()) == NULL)
   {
        PanelAviso->Visible = false;
		this->Refresh();
		return;
   }
   DLL_CloseFormEditor(NULL);
   //mostra panel
//   PanelReconfig->Visible = true;
   //determina ano
   if(CBoxAno->ItemIndex < 0) ano_rede = estudo->AnoInicial;
   else  ano_rede = CBoxAno->ItemIndex + estudo->AnoInicial;
   //redefine Alternativa ativa no ano selecionado
//   alternativa->ReconfiguraRedeSemObra(); //DVK 2015.01.06
   estudo->DefineAlternativaAtiva(alternativa, NULL);
   alternativa->ReconfiguraRede(ano_rede);
   this->Redraw();
   //verifica se deve configurar o mercado
   if ((mercado->AnoAtual != ano_rede)||(mercado->AnoRede != ano_rede))
      {//altera ano atual do Mercado
      mercado->AnoRede  = ano_rede;
      mercado->AnoAtual = ano_rede;
      //configura Stk p/ reiniciar os anos disponíveis para cálculo de fluxo
		sinap_child->ActionMercadoChanged->Execute();
		flow->UpToDate      = false;
      }
   //esconde panel
   PanelAviso->Visible = false;
   this->Refresh();
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormVisualAlt::TreeViewExisteTreeNode(void *data)
   {
   //variáveis locais
   TTreeNode *node;

   for (int n = 0; n < TreeView->Items->Count; n++)
      {
      node = TreeView->Items->Item[n];
      if (node->Data == data) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormVisualAlt::TreeViewInicia(void)
   {
   //variáveis locais
   TTreeNode     *node_estudo, *node_alter;
   VTAlternativa *alternativa;
   TList         *lisALTERNATIVA = estudo->LisAlternativa();

   //bloqueia tratamento do evento OnChange durante a montagem do TreeView
   TreeView->OnChange = NULL;
   //reinicia TreeView
   TreeView->Items->Clear();
   //insere TreeNode do Estudo
   node_estudo = TreeView->Items->Insert(NULL, estudo->Codigo);
   //cria um TreeNode p/ cada Alternativa
   for (int n = 0; n < lisALTERNATIVA->Count; n++)
      {
      alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
      //insere TreeNode inicial da Alternativa
      node_alter = TreeView->Items->AddChildObject(node_estudo, alternativa->Codigo, alternativa);
      }
   //pré-seleciona Alternativa ativa
   TreeView->Selected = TreeViewExisteTreeNode(estudo->AlternativaAtiva);
   //libera tratamento do evento OnChange durante a montagem do TreeView
   TreeView->OnChange = TreeViewChange;
   //força chamada de TreeViewChange p/atualizar o gráfico
   TreeViewChange(TreeView, TreeView->Selected);
   }

//---------------------------------------------------------------------------



