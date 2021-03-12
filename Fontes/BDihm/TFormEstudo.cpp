//---------------------------------------------------------------------------
#include <vcl.h>
#include "TFormEstudo.h"
#pragma hdrstop
#include "VTAbre.h"
#include "Esquematico\VTEsquematico.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\BD\Figura\VTBDfigura.h"
//#include "..\Diretorio\VTPath.h"
#include "..\Esquematico\VTEsquematico.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
//#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEstudo::TFormEstudo(TComponent* Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos externos
   abre = (VTAbre*)apl->GetObject(__classid(VTAbre));
   //cria listas
   lisEQP = new TList();
   //define Parent
   if (parent)
      {
      Parent = parent;
      Align  = alClient;
      }
   }

//---------------------------------------------------------------------------
__fastcall TFormEstudo::~TFormEstudo(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::AbreEstudo(void)
   {
   //variáveis locais
   VTEstudo  *estudo;
   TTreeNode *node  = TView->Selected;

   //verifica se há um Estudo selecionado
   if ((node == NULL)||(node->Level == LevelAutor))
      {
      Aviso("Selecione um estudo");
      return(false);
      }
   //determina TreeNode do Estudo
   while (node->Level > LevelEstudo) node = node->Parent;
   //determina estudo
   estudo = (VTEstudo*)node->Data;
   //reinicia Planejamento, mantendo apenas este Estudo
   planejamento->Reinicia(estudo);
   //reinicia Redes
   redes->Clear();
   //carrega estudo da base de dados
   bd->Eqpto->CarregaEstudo(estudo);
   //marca que o Estudo não é novo e não possui alterações
   estudo->Novo     = false;
   estudo->Alterado = false;
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::AbreEstudo(void)
   {
   //variáveis locais
   VTEstudo  *estudo;
   TTreeNode *node  = TView->Selected;

   //verifica se há um Estudo selecionado
   if ((node == NULL)||(node->Level == LevelAutor))
      {
      Aviso("Selecione um estudo");
      return(false);
      }
   //determina TreeNode do Estudo
   while (node->Level > LevelEstudo) node = node->Parent;
   //determina estudo
   estudo = (VTEstudo*)node->Data;
   //carrega estudo da base de dados
   return(abre->CarregaEstudoExistente(estudo));
   }


//---------------------------------------------------------------------------
void __fastcall TFormEstudo::Desabilita(void)
   {
   //limpa TView
   TView->OnChange    = NULL;
   TView->OnExpanding = NULL;
   TView->Items->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ExcluiEstudo(void)
   {
   //variáveis locais
   VTEstudo       *estudo;
   TTreeNode      *node         = TView->Selected;
   VTBD           *bd           = (VTBD*)abre->apl->GetObject(__classid(VTBD));
   VTPlanejamento *planejamento = (VTPlanejamento*)abre->apl->GetObject(__classid(VTPlanejamento));

   //verifica se há um Estudo selecionado
   if ((bd == NULL)||(node == NULL)||(node->Level == LevelAutor))
      {
      Aviso("Selecione um estudo");
      return;
      }
   //determina TreeNode do Estudo
   while (node->Level > LevelEstudo) {node = node->Parent;}
   //determina estudo
   estudo = (VTEstudo*)node->Data;
   //solicita confirmação do usuário
   if (Confirma("O estudo " + AnsiQuotedStr(estudo->Codigo, '\'') + " será excluído da base de dados.",
                "Confirma exclusão do estudo ?") != IDYES) return;
   //exclui as figuras
   bd->Figura->RemoveEstudo(estudo);
   //exclui estudo da base de dados
   bd->Eqpto->RemoveEstudo(estudo);
   //elimina TreeNode
   TView->Selected->Delete();
   //retira Estudo do Planejamento
   planejamento->RetiraEstudo(estudo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::Habilita(void)
   {
   //reinicia TView
   TViewInicia();
   //torna visível
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::IniciaLisRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ)
   {
   //variáveis locais
   VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

   lisVIZ->Clear();
   bd->Eqpto->LeListaRedeVizinha(estudo, rede, lisVIZ);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ReiniciaEsquematico(VTEstudo *estudo, TList* lisREDE)
   {
   //Variáveis locais
   VTRede        *rede;
   VTBD          *bd          = (VTBD*)abre->apl->GetObject(__classid(VTBD));
   VTEsquematico *esquematico = (VTEsquematico*)abre->apl->GetObject(__classid(VTEsquematico));

   esquematico->Clear();
   //mantém em lisREDE somente as redes selecionadas
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //obtém lista de Redes vizinhas
      lisEQP->Clear();
      bd->Eqpto->LeListaRedeVizinha(estudo, rede, lisEQP);
      //atualiza Esquematico (sem apagar anterior)
      esquematico->Inicia(rede, lisEQP, false);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   VTEstudo      *estudo;
   VTRede        *rede;
   VTEsquematico *esquematico = (VTEsquematico*)abre->apl->GetObject(__classid(VTEsquematico));
   VTGrafico     *grafico     = (VTGrafico*)abre->apl->GetObject(__classid(VTGrafico));

   //trata seleção de acordo c/ nível do TreeNode
   switch(TView->Selected->Level)
      {
      case LevelAutor: //autor
         esquematico->Clear();
         break;
      case LevelEstudo:  //estudo
         //elimina todos os TreeNodes filhos dos TreeNodes de estudo
         TViewEliminaTreeNodeRede();
         //se necessário, expande TreeNode c/ as Redes do Estudo
         TViewInsereRedeDoEstudo(Node);
         //obtém lista de Redes do estudo
         TViewLisRedes(Node, lisEQP);
         //reinicia Esquematico
         esquematico->Inicia(NULL, lisEQP, true);
         break;
      case LevelRede:  //rede
         //obtém lista de Redes do estudo
         TViewLisRedes(Node->Parent, lisEQP);
         //reinicia Esquematico
         esquematico->Inicia(NULL, lisEQP, true);
         break;
      }
   //atualiza o grafico
   grafico->AreaRedeDefine();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TViewEliminaTreeNodeRede(void)
   {
   //variáveis locais
   TTreeNode *node;

   //loop p/ todos os TreeNodes
   for (int n = 0; n < TView->Items->Count; n++)
      {
      node = TView->Items->Item[n];
      if (node->Level == LevelEstudo) node->DeleteChildren();
      }
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormEstudo::TViewExisteTreeNode(int level, AnsiString text)
   {
   //variáveis locais
   TTreeNode *node;

   for (int n = 0; n < TView->Items->Count; n++)
      {
      node = TView->Items->Item[n];
      if ((node->Level == level)&&(node->Text.CompareIC(text) == 0)) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TViewInicia(void)
   {
   //variáveis locais
   TTreeNode      *node_autor  = NULL;
   TTreeNode      *node_estudo = NULL;
   VTEstudo       *estudo;
   VTBD           *bd          = (VTBD*)abre->apl->GetObject(__classid(VTBD));
   VTPlanejamento *planejamento = (VTPlanejamento*)abre->apl->GetObject(__classid(VTPlanejamento));
   TList          *lisESTUDO = planejamento->LisEstudo();

   //reinicia TView
   TView->Items->Clear();
   //verifica se existe a base de dados
   if (bd == NULL) return;
   //obtém lista de Estudos existentes na base de dados
   bd->Eqpto->LeListaEstudo();
   //insere TreeNodes p/ autor/estudo
   for (int n = 0; n < lisESTUDO->Count; n++)
      {
      estudo = (VTEstudo*)lisESTUDO->Items[n];
      //verifica se é o estudo da rede original
      if (estudo->Id == estudoID_ZERO) continue;
      //verifica se existe TreeNode do autor
      if ((node_autor = TViewExisteTreeNode(LevelAutor, estudo->Autor)) == NULL)
         {//insere TreeNode p/ autor
         node_autor = TView->Items->AddChildObject(NULL, estudo->Autor, estudo);
         }
      //insere TreeNode p/ Estudo
      node_estudo = TView->Items->AddChildObject(node_autor, estudo->Codigo, estudo);
      //insere TreeNode fictício, apenas p/ exibir ícone [+]
      //TView->Items->AddChild(node_estudo, "trocar ao expandir");
      }
   //expande Node do último autor e pré-seleciona seu último estudo
   if ((node_autor != NULL)&&(node_estudo != NULL))
      {
      node_autor->Expand(false);
      TView->Selected = node_estudo;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TViewInsereRedeDoEstudo(TTreeNode *node_estudo)
   {
   //variáveis locais
   VTEstudo *estudo;
   VTRede   *rede;
   VTBD     *bd      = (VTBD*)abre->apl->GetObject(__classid(VTBD));
   VTRedes  *redes   = (VTRedes*)abre->apl->GetObject(__classid(VTRedes));
   TList    *lisREDE = redes->LisRede();

   //proteção
   if (node_estudo->Level != LevelEstudo) return;
   //determina Estudo
   estudo = (VTEstudo*)node_estudo->Data;
   //descarta TreeNodes filhos
   node_estudo->DeleteChildren();
   //reinicia Redes com as rede do Estudo
   redes->Clear();
   bd->Eqpto->LeListaRede(estudo, (VTTipoRede*)NULL);
   //insere TreeNode para rede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //insere TreeNode
      TView->Items->AddChildObject(node_estudo, rede->Codigo, rede);
      }
   //ordena Redes do estudo
   node_estudo->AlphaSort(false);
   //expande o TreeNode
   node_estudo->Expand(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TViewLisRedes(TTreeNode *node_ref, TList *lisEXT)
   {
   //variáveis locais
   TTreeNode *node_rede;
   VTRede    *rede;

   //reinicia lisVIZ
   lisEXT->Clear();
   //loop nos filhos do nó
   for (int i = 0; i < node_ref->Count; i++)
      {//determina nó da Rede vizinha
      node_rede = node_ref->Item[i];
      //determina Rede do Estudo
      if ((rede = (VTRede*)node_rede->Data) == NULL) continue;
      //insere Rede vizinha em lisEXT
      if (lisEXT->IndexOf(rede) < 0) lisEXT->Add(rede);
      }
   }
//---------------------------------------------------------------------------
//eof

