//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelRede.h"
#include "VTSelRede.h"
#include "..\Apl\VTApl.h"
#include "..\BDado\VTBDrede.h"
#include "..\Diretorio\VTPath.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\DLL_Stub\Funcao\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelRede::TFormSelRede(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   bdRede  = (VTBDrede*)apl->GetObject(__classid(VTBDrede));
   redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   path    = (VTPath*)apl->GetObject(__classid(VTPath));

   //cria objetos
   selRede    = NewObjSelRede();
   lisEQP     = new TList();
   lisSEL     = new TList();
   //obtém tipos das redes
   //lê Redes da base de dados
   bdRede->LeListaRede(redes);
   //configura CLBox como MultSelct
   clboxRede->MultiSelect    = true;
   clboxRedeViz->MultiSelect = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormSelRede::~TFormSelRede(void)
   {
   //destrói objetos
   if (selRede) {delete selRede; selRede = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisSEL) {delete lisSEL; lisSEL = NULL;}
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormSelRede::ActionCancelaExecute(TObject *Sender)
   {
   //limpa objeto VTRedes
   redes->Clear();
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::ActionConfirmaExecute(TObject *Sender)
   {
   //variáveis locais
   int nivel_dependencia;

   //proteção: verifica se há pelo menos um objeto VTRede selecionado
   if (lisSEL->Count == 0)
      {
      Aviso("Não há rede selecionada");
      return;
      }
   //define Redes que devem ser carregadas
   nivel_dependencia = cboxDependencia->ItemIndex - 1;
   selRede->Executa(redes, lisSEL, nivel_dependencia);
   //lê Eqptos dos objetos VTRede selecionados
   bdRede->LeEqptoRede(redes);
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::ActionInsereRedeExecute(TObject *Sender)
   {
   //inclui em lisSEL os objetos VTRede 'checked' em CLBoxRede e CLBoxRedeViz
   if (AtualizaListaRedeSelecionada() == 0) return;
   //determina novas redes selecionadas e atualiza TViewSel
   IniciaTViewSel();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::ActionReiniciaSelExecute(TObject *Sender)
   {
   //limpa lista lisSEL
   lisSEL->Clear();
   //determina novas redes selecionadas e atualiza TViewSel
   IniciaTViewSel();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::ActionRetiraRedeExecute(TObject *Sender)
   {
   //falta implementar
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::ActionSelAllExecute(TObject *Sender)
   {
   //reinicia lisSEL c/ todos os objetos VTRede
   lisSEL->Clear(); CopiaTList(redes->LisRede(), lisSEL);
   //atualiza TViewSel
   IniciaTViewSel();
   }

//---------------------------------------------------------------------------
int __fastcall TFormSelRede::AtualizaListaRedeSelecionada(void)
   {
   //variáveis locais
   VTRede  *rede;
   int     num_rede = 0;

   //inclui em lisSEL todos objetos VTRede 'checked' em clboxRede
   for (int nr = 0; nr < clboxRede->Items->Count; nr++)
      {//verifica se o Item está 'checked'
      if (! clboxRede->Checked[nr]) continue;
      //obtém objeto VTRede armazenao no Item
      rede = (VTRede*) clboxRede->Items->Objects[nr];
      //verifica se o objeto VTRede já existe em lisSEL
      if (lisSEL->IndexOf(rede) < 0)
         {//insere objeto VTRede em lisSEL
         lisSEL->Add(rede);
         //incrementa número de objetos VTRede selecionados
         num_rede++;
         }
      }
   //inclui em lisSEL todos objetos VTRede 'checked' em clboxRedeViz
   for (int nr = 0; nr < clboxRedeViz->Items->Count; nr++)
      {//verifica se o Item está 'checked'
      if (! clboxRedeViz->Checked[nr]) continue;
      //obtém objeto VTRede armazenado dmo Item
      rede = (VTRede*) clboxRedeViz->Items->Objects[nr];
      //verifica se o objeto VTRede já existe em lisSEL
      if (lisSEL->IndexOf(rede) < 0)
         {//insere objeto VTRede em lisSEL
         lisSEL->Add(rede);
         //incrementa número de objetos VTRede selecionados
         num_rede++;
         }
      }
   return(num_rede);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::clboxRedeClick(TObject *Sender)
   {
   //variáveis locais
   int       index;
   VTRede    *rede, *rede_viz;

   //proteção
   if (clboxRede->ItemIndex < 0) return;
   //determina objeto VTRede armazenado no Item
   rede = (VTRede*)clboxRede->Items->Objects[clboxRede->ItemIndex];
   //cancela 'selected' de todos Items em CLBoxRedeViz
   for (int n = 0; n < clboxRedeViz->Items->Count; n++) clboxRedeViz->Selected[n] = false;
   //obtém lista de Redes vizinhas da Rede selecionada
   lisEQP->Clear();
   rede->LisRedeVizinha(lisEQP);
   for (int n = 0; n < lisEQP->Count; n++)
      {//determina Rede vizinha
      rede_viz = (VTRede*)lisEQP->Items[n];
      //determina Item do objeto VTRede em CLBoxRedeViz
      if ((index = clboxRedeViz->Items->IndexOfObject(rede_viz)) >= 0)
         {//'select' do Item em CLBoxRedeViz
         clboxRedeViz->Selected[index] = true;
         }
      }
   //limpa lista lisEQP
   lisEQP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::clboxRedeDblClick(TObject *Sender)
   {
   //variáveis locais
   int       index;
   bool      checked;
   VTRede    *rede, *rede_viz;

   //proteção
   if (clboxRede->ItemIndex < 0) return;
   //verifica se setar ou resetar atributo 'checked'
   checked = ! clboxRede->Checked[clboxRede->ItemIndex];
   //redefine atributo 'checked' do Item corrente em CLBoxRede
   clboxRede->Checked[clboxRede->ItemIndex] = checked;
   //determina objeto VTRede armazenado no Item
   rede = (VTRede*)clboxRede->Items->Objects[clboxRede->ItemIndex];
   //obtém lista de Redes vizinhas da Rede selecionada
   lisEQP->Clear();
   rede->LisRedeVizinha(lisEQP);
   for (int n = 0; n < lisEQP->Count; n++)
      {//determina Rede vizinha
      rede_viz = (VTRede*)lisEQP->Items[n];
      //determina Item do objeto VTRede em CLBoxRedeViz
      if ((index = clboxRedeViz->Items->IndexOfObject(rede_viz)) >= 0)
         {//redefine atributo 'checked'  do Item em CLBoxRedeViz
         clboxRedeViz->Checked[index] = checked;
         }
      }
   //limpa lista lisEQP
   lisEQP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::clboxRedeVizClick(TObject *Sender)
   {
   //cancela seleção de CLBoxRede
   clboxRede->ClearSelection();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::FormClose(TObject *Sender, TCloseAction &Action)
   {
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::FormShow(TObject *Sender)
   {
   //inicia TViewRede c/ todas Redes
   IniciaTViewRede();
   //inicia CLBoxRede c/ a Rede selecionada em TViewRede
   IniciaCheckListBoxRede();
   //inicia TViewSel c/ as Redes selecionadas
   IniciaTViewSel();
   //CLBoxSel c/ as Redes interligadas à Rede selecionada em TViewSel
   IniciaCheckListBoxSel();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::IniciaCheckListBoxRede(void)
   {
   //variáveis locais
   VTRede    *rede;
   TTreeNode *node, *node_rede;

   //limpa CListBox
   clboxRede->Clear();
   //determina Node selecionado
   if ((node = tviewRede->Selected) == NULL) return;
   //atualiza CListBoxRede de acordo c/ Node selecionado
   switch(node->Level)
      {
      case 1:  //Node de tipo da rede: exibe todas Redes do tipo
         for (int n = 0; n < node->Count; n++)
            {
            node_rede = node->Item[n];
            rede = (VTRede*)node_rede->Data;
            clboxRede->Items->AddObject(rede->Codigo(), rede);
            }
         break;
      case 2:  //Node de rede: exibe Rede
         rede = (VTRede*)node->Data;
         clboxRede->Items->AddObject(rede->Codigo(), rede);
         break;
      case 0:
      default: break; //nada a fazer
      }
   //pré-seleciona primeiro Item
   if (clboxRede->Items->Count > 0) clboxRede->Selected[0] = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::IniciaCheckListBoxRedeViz(void)
   {
   //variáveis locais
   int       index;
   VTRede    *rede, *rede_viz;
   VTReducao *reducao;

   //esconde CListBox p/ evitar flicker
   clboxRedeViz->Visible = false;
   //limpa CListBox
   clboxRedeViz->Clear();
   //loop p/ todos objetos VTRede em CLBoxRede
   for (int n = 0; n < clboxRede->Items->Count; n++)
      {
      rede = (VTRede*) clboxRede->Items->Objects[n];
      //obtém lista de Redes vizinhas da Rede selecionada
      lisEQP->Clear();
      rede->LisRedeVizinha(lisEQP);
      for (int nrv = 0; nrv < lisEQP->Count; nrv++)
         {//determina Rede vizinha
         rede_viz = (VTRede*)lisEQP->Items[nrv];
         //verifica se o objeto VTRede já existe em CLBoxRedeViz
         if ((index = clboxRedeViz->Items->IndexOfObject(rede_viz)) < 0)
            {//insere objeto VTRede em CLBoxRedeViz
            clboxRedeViz->Items->AddObject(rede_viz->Codigo(), rede_viz);
            index = clboxRedeViz->Items->Count-1;
            }
         //seleciona objeto VTRede em CLBoxRedeViz se o objeto VTRede associado estiver
         //selecionado em CLBoxRede
         if (clboxRede->Selected[n]) clboxRedeViz->Selected[index] = true;
         }
      }
   //exibe CListBox
   clboxRedeViz->Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::IniciaCheckListBoxSel(void)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::IniciaTViewRede(void)
   {
   //variáveis locais
   VTRede     *rede;
   VTTipoRede *tipo_rede;
   TTreeNode  *node_raiz, *node_tipo;
   TList      *lisREDE;
   TList      *lisTIPO;
   AnsiString nome_rede = ExtractFileName(path->DirRedeBase());

   //obtém lista de TipoRede
   lisTIPO = redes->LisTipoRede();
   //obtém lista de Redes
   lisREDE = redes->LisRede();
   //reinicia TViewAll
   tviewRede->Items->Clear();
   //insere Node raiz c/ nome da rede
   node_raiz = tviewRede->Items->Add(NULL, nome_rede);
   //insere Node de nível 1: tipos das redes
   for (int nt = 0; nt < lisTIPO->Count; nt++)
      {
      tipo_rede = (VTTipoRede*)lisTIPO->Items[nt];
      node_tipo = tviewRede->Items->AddChild(node_raiz, tipo_rede->codigo);
      //insere Node de nível 2: Redes do tipo
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         if (rede->TipoRede() == tipo_rede)
            {//insere Node c/ objeto VTRede
            tviewRede->Items->AddChildObject(node_tipo, rede->Codigo(), rede);
            }
         }
      //elimina TreeNode dos tipos de rede que está vazio
      if (node_tipo->Count == 0) tviewRede->Items->Delete(node_tipo);
      }
   //expande todo o TreeView
   tviewRede->FullExpand();
   //pré-seleciona Node raiz
   tviewRede->Selected = node_raiz;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::IniciaTViewSel(void)
   {
   //variáveis locais
   VTRede     *rede;
   VTTipoRede *tipo_rede;
   TTreeNode  *node_raiz, *node_tipo;
   TList      *lisTIPO;
   AnsiString nome_rede = ExtractFileName(path->DirRedeBase());

   //reinicia TViewSel
   tviewSel->Items->Clear();
   //insere Node raiz c/ nome da rede
   node_raiz = tviewSel->Items->Add(NULL, nome_rede);
   //insere Node de nível 1: tipos das redes
   lisTIPO = redes->LisTipoRede();
   for (int nt = 0; nt < lisTIPO->Count; nt++)
      {
      tipo_rede = (VTTipoRede*)lisTIPO->Items[nt];
      node_tipo = tviewSel->Items->AddChild(node_raiz, tipo_rede->codigo);
      //insere Node de nível 2: Redes do tipo
      for (int nr = 0; nr < lisSEL->Count; nr++)
         {
         rede = (VTRede*)lisSEL->Items[nr];
         if (rede->TipoRede() == tipo_rede)
            {//insere Node c/ objeto VTRede
            tviewSel->Items->AddChildObject(node_tipo, rede->Codigo(), rede);
            }
         }
      //elimina TreeNode dos tipos de rede que está vazio
      if (node_tipo->Count == 0) tviewSel->Items->Delete(node_tipo);
      }
   //expande todo o TreeView
   tviewSel->FullExpand();
   //pré-seleciona Node raiz
   tviewSel->Selected = node_raiz;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormSelRede::SelecionaRedesVizinhas(void)
   {
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormSelRede::tviewRedeChange(TObject *Sender, TTreeNode *Node)
   {
   //reinicia CLBoxRede
   IniciaCheckListBoxRede();
   //reinicia CLBoxRedeViz
   IniciaCheckListBoxRedeViz();
   }

//---------------------------------------------------------------------------
//eof


