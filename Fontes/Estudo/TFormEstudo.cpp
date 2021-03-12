//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormEstudo.h"
#include "TFormFinalizaObra.h"
#include "TFormAvalia.h"
#include "TFormEditaEstudo.h"
#include "TFormObra.h"
#include "TFormConfig.h"
#include "TFormVisualAlt.h"
#include "TSalvaAlternativa.h"
#include "Relatorio\TFormAtivo.h"
#include "AnaTec\TFormSelResumoTec.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Figura\VTFiguras.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTMercado.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Stk\VTStkEqv.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Stk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEstudo::TFormEstudo(TComponent* Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //inicia objetos Planejamento e Estudo
   if (! ExistePlanejamentoEstudo()) Close();
   //insere este Form em seu parent
   if (parent)
      {//redefine Parent
      Parent = parent;
      }
   //atualiza LabelEstudo
   LabelEstudo->Caption = estudo->Codigo;
   //habilita/desabilita Análise do estudo
   ButAnalise->Visible = planejamento->AnaliseEnabled;
   //salva posição inicial da janela flutuante
   janela.top    = 0;
   janela.left   = 0;
   janela.width  = Width;
   janela.height = Height;
   //cria objetos
   lisTMP = new TList();
   //marca que o usuário ainda não configurou opções p/ análise
   usuario_definiu_opcao_analise = false;
   //habilita exibição das Acoes das Obras
   HabilitaApresentacaoDasAcoes(true);
   //configura PopupMenuAnalise e PopupMenuRelatorio
   PopupMenuInicia(PopupMenuAnalise,   planejamento->LisActionAnalise());
   PopupMenuInicia(PopupMenuRelatorio, planejamento->LisActionRelatorio());
   //gera aviso indicando limite de número de barras para análise de confiabilidade
   //AvisoLimiteDeBarras();
   //desabilita botão para salvar alternativa como rede original
   ButSalvaAlt->Visible = false;
   if(estudo->Tipo == estudoPLANEJAMENTO)
      {//substitui eventuais redes BT equivalentes por suas cargas correpondentes
      TrocaRedeEqvBTporCarga();
      //habilita botão para salvar alternativa como rede original
      ButSalvaAlt->Visible = true;
      }
   //verifica se o Estudo tem a Alternativa padrão
   if (estudo->AlternativaPadrao == NULL)
      {//cria Alternativa padrão (sem obras)
      InsereAlternativa("[Rede Original]", true);
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
__fastcall TFormEstudo::~TFormEstudo(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //restaura rede original
   //if (estudo) estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
   //retira ActionObraAlterada de VTEdita
   HabilitaApresentacaoDasAcoes(false);
   //apaga moldura
   GraficoMoldura();
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
	//fecha FormEditor  //DVK 2015.03.10 descomentado
	DLL_CloseFormEditor(NULL);
   //destrói lista sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlignDownExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada em cima
   if (Align == alTop) Align = alNone;
   //alinha janela em baixo
   Align = alBottom;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlignNoneExecute(TObject *Sender)
   {
   //cancela alinhamento da janela (flutuante)
   Align  = alNone;
   Top    = janela.top;
   Left   = janela.left;
   Width  = janela.width;
   Height = janela.height;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlignLeftExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada à direita
   if (Align == alRight) Align = alNone;
   //alinha janela à esquerda
   Align = alLeft;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlignRightExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada à esquerda
   if (Align == alLeft) Align = alNone;
   //alinha janela à direita
   Align = alRight;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlignUpExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada em baixo
   if (Align == alBottom) Align = alNone;
   //alinha janela em cima
   Align = alTop;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlternativaCopiaExecute(TObject *Sender)
   {
   //variáveis locais
   TFormEdtTxt    *FormEdtTxt;
   VTAlternativa *alternativa, *alternativa_nova;

   //verifica se há uma Alternativa selecionada
   if ((alternativa = TreeViewAlternativaSelecionada()) == NULL) return;
   //verifica se Alternativa padrão
   if (alternativa == estudo->AlternativaPadrao)
      {
      Aviso("A rede original não pode ser copiada");
      return;
      }
   try{//solicita nome da nova Alternativa
      FormEdtTxt = new TFormEdtTxt(this, "Identifique a Alternativa");
      FormEdtTxt->Texto = NomeProximaAlternativa();
      if (FormEdtTxt->ShowModal() == mrOk)
         {//cria uma nova Alternativa como cópia da Alternativa selecionada
         alternativa_nova = estudo->CopiaAlternativa(alternativa);
         alternativa_nova->Codigo = FormEdtTxt->Texto;
         //reinicia TView
         TreeViewInicia();
         }
      //destrói FormEdtTxt
      delete FormEdtTxt;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlternativaEditaExecute(TObject *Sender)
   {
   //variáveis locais
   bool          repita = true;
   TTreeNode     *node;
   VTAlternativa *alternativa;
   TFormEdtTxt   *FormEdtTxt;

   //verifica se há uma Alternativa selecionada
   if ((alternativa = TreeViewAlternativaSelecionada()) == NULL) return;
   //verifica se é a Alternativa da rede original
   if (alternativa == estudo->AlternativaPadrao)
      {
      Aviso("Não é permitido renomear a rede original");
      return;
      }
   //exibe FormEdtTxt p/ usuário editar a identificação da Alternativa
   FormEdtTxt        = new TFormEdtTxt(this, "Identifique a Alternativa");
   FormEdtTxt->Texto = alternativa->Codigo;
   while (repita)
      {
      if (FormEdtTxt->ShowModal() == mrCancel) repita = false;
      else
         {//verifica se existe Alternativa com mesmo nome
         if (estudo->ExisteAlternativa(FormEdtTxt->Texto) != NULL)
            {
            Aviso("Já existe outra alternativa " + AnsiQuotedStr(FormEdtTxt->Texto, '\''));
            }
         else
            {//atualiza código da Alternativa
            alternativa->Codigo = FormEdtTxt->Texto;
            //marca que Obras foi alterada
            alternativa->Obras->Modificada = true;
            repita = false;
            }
         }
      }
   //destrói FormEdtTxt
   delete FormEdtTxt;
   //atualiza TView
   if ((node = TreeViewExisteTreeNode(alternativa)) != NULL)
      {
      node->Text = alternativa->Codigo;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlternativaInsereExecute(TObject *Sender)
   {
   //variáveis locais
   bool           repita = true;
   TFormEdtTxt    *FormEdtTxt;

   try{//solicita nome da nova Alternativa
      FormEdtTxt        = new TFormEdtTxt(this, "Identifique a Alternativa");
      FormEdtTxt->Texto = NomeProximaAlternativa();
      while (repita)
         {
         if (FormEdtTxt->ShowModal() == mrCancel) repita = false;
         else
            {//verifica se existe Alternativa com mesmo nome
            if (estudo->ExisteAlternativa(FormEdtTxt->Texto))
               {
               Aviso("Já existe outra alternativa " + AnsiQuotedStr(FormEdtTxt->Texto, '\''));
               }
            else
               {//cria Alternativa
               InsereAlternativa(FormEdtTxt->Texto);
               //reinicia TView
               TreeViewInicia();
               repita = false;
               }
            }
         }
      //destrói FormEdtTxt
      delete FormEdtTxt;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAlternativaRetiraExecute(TObject *Sender)
   {
   //variáveis locais
   VTAlternativa *alternativa;

   //determina Alternativa selecionada
   if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
      {
      Aviso("Selecione a alternativa a ser excluída");
      return;
      }
   //retira a Alternativa do Estudo
   RetiraAlternativa(alternativa);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAnaliseCfgConfiabilidadeExecute(TObject *Sender)
   {
   Aviso("Falta implementar");
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAnaliseCfgPeriodoExecute(TObject *Sender)
   {
   //esconde este Form
   Hide();
   ConfiguraOpcaoAnalise();
   //torna a exibir este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionAnaliseDefaultExecute(TObject *Sender)
   {
   //variáveis locais
   TFormAvalia    *FormAvalia;

   //valida datas de todas Obras
   if (! ValidaDataTodasObras()) return;
   //funde as Acoes das Alternativa
   FundeAcoesDasAlternativas();
   //torna ativa a Alternativa padrão
   SelecionaAlternativaPadrao();
   //esconde este Form
   Hide();
   //verifica se o período de análise está definido
   if (! usuario_definiu_opcao_analise)
      {//exibe tela p/ usuário configurar opções da análise das alternativas
      ConfiguraOpcaoAnalise();
      }
   if (usuario_definiu_opcao_analise)
      {//cria FormAvalia
      FormAvalia = new TFormAvalia(this, apl);
      FormAvalia->ShowModal();
      //destrói FormAvalia
      delete FormAvalia;
      }
   //torna a exibir este Form
   Show();
  }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionEditorExecute(TObject *Sender)
   {
   //variáveis locais
   bool planejamento = true;

   //verifica se deve exibir ou esconder o Editor
   if (ButEditor->Down)
      {//exibe o Editor
		//DLL_NewFormEditor(this, apl, Parent);
		if((estudo->Tipo == estudoRECLAMACAO_BT)||(estudo->Tipo == estudoRECLAMACAO_MT))
			{//DVK 2015.03.03 insere FormEditor no avô do Parent
			DLL_NewFormEditor(apl->Form, apl, Parent->Parent->Parent, planejamento);
			}
		else DLL_NewFormEditor(apl->Form, apl, Parent, planejamento);
      }
   else
      {//esconde o Editor
      DLL_CloseFormEditor(NULL);
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionEstudoEditaExecute(TObject *Sender)
   {
   //variáveis locais
   TFormEdtTxt    *FormEdtTxt;

   try{//solicita nome do estudo
      FormEdtTxt        = new TFormEdtTxt(this, "Nome do Estudo");
      FormEdtTxt->Edit->MaxLength = 50;
      FormEdtTxt->Texto = estudo->Codigo;
      if (FormEdtTxt->ShowModal() == mrOk)
         {//redefine código do estudo
         estudo->Codigo = FormEdtTxt->Texto;
         //atualiza LabelEstudo
         LabelEstudo->Caption = estudo->Codigo;
         //marca que o Estudo foi alterado
         estudo->Alterado = true;
         }
      //destrói FormEdtTxt
      delete FormEdtTxt;
      }catch(Exception &e)
         {
         }
   }
*/
// ---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionEstudoEditaExecute(TObject *Sender)
{ //DVK 2018.02.23 insere justificativa para estudo
	// variáveis locais
	TFormEditaEstudo *formEditaEstudo;

	try
	{ // cria form para editar nome e justificativa do estudo
		formEditaEstudo = new TFormEditaEstudo(this, apl, estudo);
		formEditaEstudo->ShowModal();
		//atualiza LabelEstudo
        LabelEstudo->Caption = estudo->Codigo;
		// destrói form
		delete formEditaEstudo;
	}
	catch (Exception &e)
	{
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionObraAlteradaExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode *node;
   VTObra    *obra = (VTObra*)Sender;

   //proteção
   if (obra == NULL) return;
   //determina TreeNode da Obra
   if ((node = TreeViewExisteTreeNode(obra)) == NULL) return;
   //verifica se o TreeNode da Obra e o TrreNode selecionado
   if (node != TreeView->Selected)
   {
		if (TreeView->Selected->Parent != node)
		{
			return;
		}
   }
   //atualiza Acoes da Obra
   TreeViewInsereTreeNodeAcao(node, obra);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionObraColorEnabledExecute(TObject *Sender)
	{
	//variáveis locais
	VTAlternativa *alternativa;
	VTObra        *obra;
	VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//determina Alternativa selecionada
	if ((alternativa = TreeViewAlternativaSelecionada()) == NULL) return;
	//configura todas Obras da Alternativa
	alternativa->Obras->ColorEnabled = MenuItemObraColorEnabled->Checked;
	//atualiza o gráfico
	grafico->Redraw();
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionObraEditaExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode     *node;
   VTObra        *obra;

	//determina Alternativa selecionada
	if (TreeViewAlternativaSelecionada() == NULL) return;
	//determina Obra selecionada
   if ((obra = TreeViewObraSelecionada()) == NULL) return;
   //edita atributos da Obra conforme o tipo de planejamento
   if (EditaObra(obra))
      {//marca que Obras foi alterada
      obra->Obras->Modificada = true;
      //atualiza TreeView
      if ((node = TreeViewExisteTreeNode(obra)) != NULL)
         {
         node->Text = TreeViewNomeDaObra(obra);
         }
      //força seleção do TreeNode da Obra editada
      TreeView->Selected = TreeViewExisteTreeNode(obra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionObraInsereExecute(TObject *Sender)
   {
   //variáveis locais
   VTAlternativa *alternativa;
   VTObra        *obra;

   //determina Alternativa selecionada
   if ((alternativa = TreeViewAlternativaSelecionada()) == NULL) return;
   //verifica se é a Alternativa da rede original
   if (alternativa == estudo->AlternativaPadrao)
      {
      Aviso("Não é permitido criar obra na rede original");
      return;
      }
   try{//cria uma nova Obra
      obra = alternativa->Obras->InsereObra(alternativa->Obras->DataFinal);
      //edita atributos da Obra conforme o tipo de planejamento
      if (! EditaObra(obra))
         {//destrói Obra criada
         alternativa->Obras->RetiraObra(obra);
         return;
         }
      //atualiza TreeView
      TreeViewInsereTreeNodeObra(TreeViewExisteTreeNode(alternativa), obra);
      //expande TreeNode da Alternativa
      //TreeView->Selected->Expand(true);
      //força seleção do TreeNode da nova Obra
      TreeView->Selected = TreeViewExisteTreeNode(obra);
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionObraRetiraExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode     *node = TreeView->Selected;
   VTAlternativa *alternativa;
   VTObra        *obra, *ult_obra;

   //proteção: determina Node da Obra
   if (node == NULL) return;
   while (node->Level > 2) node = node->Parent;
   if (node->Level != 2) return;
   //detemina Alternativa e Obra selecionada
   alternativa  = TreeViewAlternativaSelecionada();
   obra         = TreeViewObraSelecionada();
   if ((alternativa == NULL)||(obra == NULL))
	  {
	  Aviso("Selecione a obra a ser excluída");
      return;
	  }
   //verifica se é a última Obra da Alternativa ou uma Obra sem nenhuma Acao
   ult_obra = alternativa->Obras->ObraFinal();
   if ((obra != ult_obra)&&(obra->LisAcao->Count > 0))
	  {
      Aviso(" A obra " + AnsiQuotedStr(obra->Nome, '\'') + " não pode ser excluída");
	  return;
	  }
   //pede confirmação do usuário
   if (Confirma("Obra " + AnsiQuotedStr(obra->Nome, '\''),
				"Confirma a exclusão desta obra ?") != IDYES) return;
   //retira Obra selecionada
   alternativa->Obras->RetiraObra(obra);
   //seleciona TreeNode pai
   TreeView->Selected = node->Parent;
   //retira TreeNode da Obra
   node->Delete();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionObraExibeAcaoExecute(TObject *Sender)
	{
	//habilita/desabilita apresentação das Acoes das Obras
	HabilitaApresentacaoDasAcoes(MenuItemObraExibeAcao->Checked);
	//reinicia TreeView
	TreeViewInicia();
	}

////---------------------------------------------------------------------------
//void __fastcall TFormEstudo::ActionRelatorioExecute(TObject *Sender)
//   {
//   //variáveis locais
//   TFormAtivo *form;
//
//   //esconde este Form
//   Visible = false;
//   try{//cria objeto para exibir relatório do planejamento
//      if ((form = new TFormAtivo(this, estudo, apl)) != NULL)
//         {
//         form->ShowModal();
//         delete form;
//         }
//      }catch(Exception &e)
//         {
//         Erro("O relatório de planejamento não pode ser criado");
//         }
//   //reapresenta este Form
//   Visible = true;
//   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
void __fastcall TFormEstudo::ActionRelatorioExecute(TObject *Sender)
   {
   //variáveis locais
   TFormAtivo *form;

   //esconde este Form
   Visible = false;
   //
   RelatorioObra(estudo);

   //reapresenta este Form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionSalvaAltExecute(TObject *Sender)
   {//DVK 2017.01.11
   //variáveis locais
   TSalvaAlternativa *salva_alt;
   AnsiString        cod_original, autor_original;

   //verifica se a alternativa ativa nao é a rede original
   if(estudo->AlternativaAtiva == estudo->AlternativaPadrao)
      {
      Aviso("Selecione uma alternativa diferente da rede original");
      return;
      }
   //executa todas as obras da alternativa
   estudo->AlternativaAtiva->ReconfiguraRedeTodaObra();
   //renomeia estudo
   cod_original   = estudo->Codigo;
   autor_original = estudo->Autor;
   estudo->Codigo = "Estudo correspondente à rede principal";
   estudo->Autor  = "Sinapsis";
   //cria objeto
   salva_alt = new TSalvaAlternativa(apl);
   //salva alternativa ativa como rede original em uma nova base
   salva_alt->Executa();
   //destroi objeto
   delete salva_alt;
   //retorna nome original
   estudo->Codigo = cod_original;
   estudo->Autor  = autor_original;
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::ActionVisualAltAnoExecute(TObject *Sender)
   { //DVK 2014.04.09
   //variáveis locais
   TFormVisualAlt *form;

   //esconde este Form
   Hide();
   try{//cria objeto para exibir visualização de alternativa
      if ((form = new TFormVisualAlt(this, apl, estudo)) != NULL)
         {
         form->Show();
         }
      }catch(Exception &e)
         {
         Erro("A janela de visualização de alternativa não pode ser criada");
         }
   //reapresenta este Form ao destruir TFormVisualAlt
//   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::AtualizaEstado(bool gravando_base)
   {
   //exibe/esconde PanelAviso
   PanelAviso->Visible = gravando_base;
   //verifica se a base de dados está sendo gravada
   if (gravando_base)
      {//garante que a rede original esteja selecionada (e não uma alternativa)
      SelecionaAlternativaPadrao();
      //proteção: verifica se a Alternativa padrão foi selecionada
      if (estudo->AlternativaAtiva != estudo->AlternativaPadrao)
         {
		   estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
		   DLL_CloseFormEditor(NULL);
         }
      //desabilita este Form para evitar que o usuário altere o estudo
      Enabled = false;
      }
   else
      {//reabilita este Form
      Enabled = true;
      }
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormEstudo::AvisoLimiteDeBarras(void)
   {
   //variáveis locais
   AnsiString txt;
   int        limite_barra = DLL_Confiabilidade_LimiteBarras();
   VTRedes    *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList      *lisBARRA    = redes->LisBarra();

   //verifica número de Barras
   if (lisBARRA->Count > (0.6 * limite_barra))
      {
      txt.sprintf("A rede original possui %d barras.\n"
                  "Na análise das alternativa, cálculo de DEC/FEC tem limitação de %d barras.",
                  lisBARRA->Count, limite_barra);
      Aviso(txt);
      }
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::ConfiguraOpcaoAnalise(void)
   {
   //variáveis locais
   bool        sucesso = false;
   TFormConfig *Form;

   try{//cria TFormConfig e exibe como janela modal
      Form    = new TFormConfig(this, apl);
      sucesso = (Form->ShowModal() == mrOk);
      delete Form;
      //marca que o usuário configurou opção somente se ele confirmar
      if (sucesso) usuario_definiu_opcao_analise = true;
      }catch(Exception &e)
         {
         Aviso("Não foi possível exibir o formulário de configuração");
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::EditaObra(VTObra *obra)
   {
   //variáveis locais
   bool       sucesso;
   TFormObra  *FormObra;

   //verifica se o Planejamento foi configurado para edição externa da Obra
   if (planejamento->ActionEdtObra)
	  {//executa Action  associada ao Planejamento
	  planejamento->ActionEdtObra->OnExecute(obra);
	  //verifica se edição foi confirmada ou cancelada
	  sucesso = (obra->ModalResult == mrOk);
	  }
   else
	  {//planejamento básico do Sinap
	  FormObra = new TFormObra(this, apl, obra);
	  sucesso  =  (FormObra->ShowModal() == mrOk);
	  //destrói TFormObra
	  delete FormObra;
	  }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::ExistePlanejamentoEstudo(void)
   {
   //variáveis locais
   TList *lisESTUDO;

   //assume valores seguros
   planejamento = NULL;
   estudo       = NULL;
   //determina objeto Planejamento
   planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   if (planejamento != NULL)
	  {
	  lisESTUDO = planejamento->LisEstudo();
	  if (lisESTUDO->Count == 1) estudo = (VTEstudo*)lisESTUDO->First();
	  }
   return(estudo != NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::ExisteRedeEqvBT(void)
{
	//variáveis locais
	VTRede  *rede;
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList   *lisREDE;
	bool     possui = false;
	TList *lisRedeCluster;

	lisREDE = new TList;
	redes->LisRede(lisREDE);
	TList   *lisCLUSTER = redes->LisCluster();
	VTCluster *cluster;

	//remove as redes dos clusters da lista de redes a serem verificadas
	for (int nc = 0; nc < lisCLUSTER->Count ; nc++)
	{
		cluster = (VTCluster*)lisCLUSTER->Items[nc];
		lisRedeCluster = cluster->LisRede();
		for (int nr = 0; nr < lisRedeCluster->Count; nr++)
		{
			rede = (VTRede*)lisRedeCluster->Items[nr];
			try
			{
				if((rede->TipoRede != NULL)&&(rede->TipoRede->Segmento == redeSEC))
				{
                    rede->Show[eqp_VISIBLE] = false;
                }
				lisREDE->Remove(rede);
			}
			catch(...)
			{
                //teste
            }
		}
	}

	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		//      if (rede->Carregada) continue;  //DVK 2016.11.28 comentada, pois qualquer
		//rede secundária será trocada por carga
		if (rede->TipoRede->Segmento == redeSEC)
		{
			possui = true;
			break;
		}
	}


	if (lisREDE)
	{
		delete lisREDE;
		lisREDE = NULL;
	}
	return(possui);
}

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //redefine a rede com sua configuração original
   estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::FormCloseQuery(TObject *Sender, bool &CanClose)
   {
   //variáveis locais
   AnsiString meta_class_name;
   TMetaClass *owner_meta_class  = Owner->ClassType();
   TMetaClass *form_meta_class = (__classid(TForm));

   ///não permite o fechamento deste Form
   CanClose = false;
   //verifica se o Owner é um Form
   while(owner_meta_class)
      {
      if (owner_meta_class == form_meta_class)
         {//fecha o Owner
         ((TForm*)Owner)->Close();
         break;
         }
      else
         {
         owner_meta_class = owner_meta_class->ClassParent();
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //define posição default da janela flutuante
   janela.top    = 0;
   janela.left   = 0;
   janela.width  = Width;
   janela.height = Height;
   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this))
         {
         switch(Align)
            {
            case alLeft:   ActionAlignLeft->Execute();   break;
            case alRight:  ActionAlignRight->Execute();  break;
            case alTop:    ActionAlignUp->Execute();     break;
            case alBottom: ActionAlignDown->Execute();   break;
			case alNone:   //salva última
            default:
               janela.top    = Top;
               janela.left   = Left;
               janela.width  = Width;
               janela.height = Height;
               break;
            }
         return;
         }
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //apresenta as Alternativas
   Inicia();
   //gera aviso indicando limite de número de barras para análise de confiabilidade
//   AvisoLimiteDeBarras();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::FundeAcoesDasAlternativas(void)
   {
   //variáveis locais
   VTAlternativa *alternativa;
   TList         *lisALTERNATIVA = estudo->LisAlternativa();

   //loop p/ todas Alternativas
   for (int n = 0; n < lisALTERNATIVA->Count; n++)
      {
      alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
      if (alternativa == estudo->AlternativaPadrao) continue;
      alternativa->Obras->FundeObras();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::GraficoMoldura(VTEqpto *eqpto)
   {
	//variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//apaga moldura
   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::GraficoZoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //apaga moldura
   if (eqpto != NULL) grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::HabilitaApresentacaoDasAcoes(bool enabled)
   {
   //variáveis locais
   VTEdita *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //proteção
   if (edita == NULL) return;
   //verifica se deve exibir ou não as Acoes das Obras no TrreView
   if (enabled)
      {//insere ActionObraAlterada em VTEdita p/ atualizar as Acoes das Obras
      edita->ActionOnChangeRedeInsere(ActionObraAlterada);
      }
   else
      {//retira ActionObraAlterada de VTEdita p/ não atualizar as Acoes das Obras
      edita->ActionOnChangeRedeRetira(ActionObraAlterada);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::Inicia(void)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->Disable();
//   if(estudo->Tipo == estudoPLANEJAMENTO)
//      {//substitui eventuais redes BT equivalentes por suas cargas correpondentes
//      TrocaRedeEqvBTporCarga();
//      //habilita botão para salvar alternativa como rede original
//      ButSalvaAlt->Visible = true;
//      }
//   //verifica se o Estudo tem a Alternativa padrão
//   if (estudo->AlternativaPadrao == NULL)
//      {//cria Alternativa padrão (sem obras)
//      InsereAlternativa("[Rede Original]", true);
//      }
   //atualiza TreeView
   TreeViewInicia();
    }

//---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudo::InsereAlternativa(AnsiString codigo, bool padrao)
   {
   //variáveis locais
   VTAlternativa *alternativa;

   //cria uma nova Alternativa
   if ((alternativa = estudo->InsereAlternativa(codigo, padrao)) != NULL)
      {//define nova Alternativa como ativa
      estudo->DefineAlternativaAtiva(alternativa, NULL);
      }
   return(alternativa);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEstudo::NomeProximaAlternativa(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString codigo;

   do {//define nome
      codigo.sprintf("Alternativa %02d", count++);
      }while (estudo->ExisteAlternativa(codigo) != NULL);

   return(codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::PopupMenuInicia(TPopupMenu *PopupMenu, TList *lisACTION)
   {
   //variáveis locais
   TAction        *ActionExterna;
   TMenuItem      *MenuItem;

   //inclui MenuItems p/ Actions externas cadastradas em VTPlanejamento
   for (int n = 0; n < lisACTION->Count; n++)
      {
      ActionExterna        = (TAction*)(lisACTION->Items[n]);
      //insere novo MenuItem
      MenuItem             = new TMenuItem(PopupMenu);
      //associa MenuItem com Action externa
      MenuItem->Action     = ActionExterna;
      //salva Action em MenuItem::Tag
      MenuItem->Tag        = int(ActionExterna);
      MenuItem->Hint       = ActionExterna->Hint;
      MenuItem->Caption    = ActionExterna->Caption;
      MenuItem->AutoCheck  = false;
      MenuItem->Checked    = false;
      PopupMenu->Items->Add(MenuItem);
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
bool __fastcall TFormEstudo::RelatorioObra(VTEstudo *estudo)
   {
   //variáveis locais
   bool       sucesso;
   TFormAtivo  *FormAtivo;

   //verifica se o Planejamento foi configurado para edição externa da Obra
   if (planejamento->ActionRelObra)
	  {//executa Action  associada ao Planejamento
	  planejamento->ActionRelObra->OnExecute(estudo);
	  //verifica se edição foi confirmada ou cancelada
	  sucesso = (estudo->ModalResult == mrOk);
	  }
   else
	  {
	  //planejamento básico do Sinap
	  FormAtivo = new TFormAtivo(this, estudo, apl);
	  sucesso  =  (FormAtivo->ShowModal() == mrOk);
	  //destrói TFormAtivo
	  delete FormAtivo;
	  }
   return(sucesso);
   }
//---------------------------------------------------------------------------
void __fastcall TFormEstudo::RetiraAlternativa(VTAlternativa *alternativa)
   {
   //não permite exlusão da Alternativa padrão
   if (alternativa == estudo->AlternativaPadrao)
      {
      Aviso("A alternativa com a rede original não pode ser excluída");
      return;
      }
   if (Confirma("Alternativa selecionada: " + AnsiQuotedStr(alternativa->Codigo, '\''),
                "Deseja excluir esta alternativa ?") != IDYES) return;
   //elimina a Alternativa do Estudo
   estudo->RetiraAlternativa(alternativa);
   //reinicia TreeView
   TreeViewInicia();
   }

//---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudo::SelecionaAlternativaPadrao(void)
   {
   //variáveis locais
   TTreeNode     *node;
   VTAlternativa *alternativa;

   //determina TreeNode c/ a Alternativa padrão
   for (int n = 0; n < TreeView->Items->Count; n++)
      {
      node = TreeView->Items->Item[n];
      if (node->Level != 1) continue;
      alternativa = (VTAlternativa*)node->Data;
      if (alternativa == estudo->AlternativaPadrao)
         {
         if (TreeView->Selected != node) TreeView->Selected = node;
         return(alternativa);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudo::TreeViewAlternativaSelecionada(void)
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
VTAlternativa* __fastcall TFormEstudo::TreeViewAlternativaSelecionada(TTreeNode *node)
   {//DVK 2014.06.04
   //proteção
   if (node == NULL) return(NULL);
   //determina TTreeNode de Level 0
   while (node->Level > 1) node = node->Parent;
   //retorna Alternativa associada ao TreeNode
   if (node->Level == 1) return((VTAlternativa*)node->Data);
   //não uma Alternativa selecionada
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   int           ano_rede;
   VTAlternativa *alternativa;
   VTObra        *obra;
   TTreeNode     *node = TreeView->Selected;
   VTMercado     *mercado     = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTSinapChild  *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   if(progresso == NULL)
	{//cria progresso e insere no apl //DVK 2016.11.28
		progresso = DLL_NewObjProgresso(this->Owner, Parent);
		apl->Add(progresso);
	}
   //inicia progresso
	if(progresso) progresso->Start(progDEFAULT);
   //detemina Alternativa selecionada
   if ((alternativa  = TreeViewAlternativaSelecionada()) == NULL) return;
   //detemina Obra selecionada
   obra = TreeViewObraSelecionada();
	DLL_CloseFormEditor(NULL);        //DVK 2014.04.02
   //redefine Alternativa ativa igual a Alternativa selecionada
   estudo->DefineAlternativaAtiva(alternativa, obra);
   //habilita/desabilita inserção de Obras
   ButObra->Enabled = ((alternativa != NULL)&&(alternativa != estudo->AlternativaPadrao));
   //habilita/desabilita apresentação do editor
   ActionEditor->Enabled = (obra != NULL);
   if (ActionEditor->Enabled)
      {//exibe/esconde Editor
      ActionEditor->Execute();
      }
   else
      {//esconde FormEditor
      DLL_CloseFormEditor(NULL);
      }
   //configura ano da rede no Mercado e  reinicia anos disponíveis p/ cálculo de fluxo de potência
   if (obra == NULL) obra = alternativa->Obras->ObraFinal();
   if (obra != NULL)
      {//determina ano de ataivação da obra
      ano_rede = obra->AnoAtivacao;
      }
   else
      {//assume ano inicial do Mercado
      ano_rede = mercado->AnoInicial;
      }
   //verifica se deve configurar o mercado
   if ((mercado->AnoAtual != ano_rede)||(mercado->AnoRede != ano_rede))
      {//altera ano atual do Mercado
      mercado->AnoRede  = ano_rede;
      mercado->AnoAtual = ano_rede;
      //configura Stk p/ reiniciar os anos disponíveis para cálculo de fluxo
		sinap_child->ActionMercadoChanged->Execute();
		}
   //verifica se TreeNode associado a uma Acao (nível 3)
   if (node->Level == 3)
      {//exibe moldura
      GraficoMoldura((VTEqpto*)node->Data);
      }
   else
      {//apaga moldura
      GraficoMoldura();
      }
   //associa Obra com ActionObraAlterada
   //if (obra) obra->ActionOnChange = ActionObraAlterada;

////CÓDIGO TEMPORÁRIO para habilitar ediçaõ da alterantiva RedeOriginal
////habilita editor da rede para RedeOriginal
//VTEdita  *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
//if (alternativa == estudo->AlternativaPadrao)
//   {
//   stk->ActionEditor->Enabled = true;
//   edita->Obra = NULL;
//   }
//else
//   {
//   stk->ActionEditor->Enabled = false;
//   }

    if(progresso) progresso->Stop();
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   int           ano_rede;
   VTAlternativa *alternativa;
   VTObra        *obra;
   TTreeNode     *node = TreeView->Selected;
   VTGrafico     *grafico     = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTMercado     *mercado     = (VTMercado*)apl->GetObject(__classid(VTMercado));
   VTSinapChild  *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
   VTFiguras     *figuras     = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   //OBS: versão da função para o caso de TCelulas não filtrar as redes visíveis

   //determina Alternativa selecionada
   if ((alternativa  = TreeViewAlternativaSelecionada()) == NULL) return;
   //detemina Obra selecionada
   obra = TreeViewObraSelecionada();
   //verifica se Alternativa/Obra selecionada é igual a atual
   if ((alternativa == estudo->AlternativaAtiva) && (obra == estudo->ObraAtiva)) return;
   //DVK 2014.04.02 - fecha a tela do editor
   DLL_CloseFormEditor(NULL);
   //desabilita atualização do Grafico para evitar redesenhar a rede na troca de Alternativa/Obra
   grafico->Disable();
   // Esconde/Mostra os comentários da obra selecionada
   figuras->ObraAtiva(obra);
   //verifica se Alternativa selecionada é diferente da atual
   if (alternativa == estudo->AlternativaAtiva)
      {//redefine Alternativa/Obra
      estudo->DefineAlternativaAtiva(alternativa, obra);
      }
   else
      {//redefine Alternativa/Obra
      estudo->DefineAlternativaAtiva(alternativa, obra);
      //TCelulas precisa ser reiniciado com a rede da nova Alternativa
      grafico->AreaRedeDefine();
	  }
   //exibe o gráfico de topologia (que vai mostrar apenas as redes da Obra selecionada)
   grafico->Show(grafTOPO);
   //habilita/desabilita inserção de Obras
   ButObra->Enabled = ((alternativa != NULL)&&(alternativa != estudo->AlternativaPadrao));
   //habilita/desabilita apresentação do editor
   ActionEditor->Enabled = (obra != NULL);
   if (ActionEditor->Enabled)
      {//exibe/esconde Editor
      ActionEditor->Execute();
      }
   else
      {//esconde FormEditor
      DLL_CloseFormEditor(NULL);
      }
   //configura ano da rede no Mercado e  reinicia anos disponíveis p/ cálculo de fluxo de potência
   if (obra == NULL) obra = alternativa->Obras->ObraFinal();
   if (obra != NULL)
      {//determina ano de ativação da obra
      ano_rede = obra->AnoAtivacao;
      }
   else
      {//assume ano inicial do Mercado
      ano_rede = mercado->AnoInicial;
      }
   //verifica se deve configurar o mercado
   if ((mercado->AnoAtual != ano_rede)||(mercado->AnoRede != ano_rede))
      {//altera ano atual do Mercado
      mercado->AnoRede  = ano_rede;
      mercado->AnoAtual = ano_rede;
      //configura Stk p/ reiniciar os anos disponíveis para cálculo de fluxo
      sinap_child->ActionMercadoChanged->Execute();
      }
   //verifica se TreeNode associado a uma Acao (nível 3)
   if (node->Level == 3)
      {//exibe moldura
      GraficoMoldura((VTEqpto*)node->Data);
      }
   else
      {//apaga moldura
      GraficoMoldura();
      }
   //associa Obra com ActionObraAlterada
   //if (obra) obra->ActionOnChange = ActionObraAlterada;
/*
//CÓDIGO TEMPORÁRIO para habilitar ediçaõ da alterantiva RedeOriginal
//habilita editor da rede para RedeOriginal
VTEdita  *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
if (alternativa == estudo->AlternativaPadrao)
   {
   stk->ActionEditor->Enabled = true;
   edita->Obra = NULL;
   }
else
   {
   stk->ActionEditor->Enabled = false;
   }
*/

   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewChanging(TObject *Sender, TTreeNode *Node,
      bool &AllowChange)
   {//DVK 2014.06.04
   //variaveis locais
   AnsiString     class_name;
   TForm          *form;

   //verifica se a rede original foi selecionada
   if (TreeViewAlternativaSelecionada(Node) != estudo->AlternativaPadrao)
      {
      AllowChange = true;
      return;
      }
   //verifica se existem Forms AlocaBC, AlocaRT ou Balanceamento abertos
   for (int n = 0; n < Screen->FormCount; n++)
      {
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormAlocaCapacitor") == 0)
         {//verifica se o Form n é um FormAlocaBC
         AllowChange = false;
         Aviso("O módulo de Alocação de Capacitor não pode ser usado na Rede Original.");
         return;
         }
      if (class_name.AnsiCompareIC("TFormAlocaRegulador") == 0)
         {//verifica se o Form n é um FormAlocaRT
         AllowChange = false;
         Aviso("O módulo de Alocação de Regulador não pode ser usado na Rede Original.");
         return;
         }
      if (class_name.AnsiCompareIC("TFormBalanceamento") == 0)
         {//verifica se o Form n é um FormAlocaBC
         AllowChange = false;
         Aviso("O módulo de Balanceamento de Carga não pode ser usado na Rede Original.");
         return;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewDblClick(TObject *Sender)
   {
   //variáveis locais
   TTreeNode *node = TreeView->Selected;

   //proteção
   if (node == NULL) return;
   //verifica se TreeNode de nível 3 ou 4
   if ((node->Level == 3)||(node->Level == 3))
      {//executa Zoom
      GraficoZoom((VTEqpto*)node->Data);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewDescricaoAcao(VTAcao *acao, AnsiString &text)
   {
   //variáveis locais
   VTEqpto *eqpto = acao->Eqpto();

   text.sprintf("%03d %s: %s ID=%05d %s",
                acao->Index + 1,
                acao->TipoAsString().c_str(),
                eqpto->TipoAsString().c_str(),
                eqpto->Id,
                AnsiQuotedStr(eqpto->Codigo,'\'').c_str());
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormEstudo::TreeViewExisteTreeNode(void *data)
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
void __fastcall TFormEstudo::TreeViewInicia(void)
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
      //monta TreeNode com Obras da alternativa
      TreeViewInsereTreeNodeObra(node_alter, alternativa);
      }
   //pré-seleciona Alternativa ativa
   TreeView->Selected = TreeViewExisteTreeNode(estudo->AlternativaPadrao);
   //ordena Alternativas
   //node_estudo->AlphaSort();
   //libera tratamento do evento OnChange durante a montagem do TreeView
   TreeView->OnChange = TreeViewChange;
   //força chamada de TReeViewChange p/atualizar o gráfico
   TreeViewChange(TreeView, TreeView->Selected);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewInsereTreeNodeAcao(TTreeNode *node_obra,  VTAcao *acao)
   {
   //variáveis locais
   AnsiString text;
   TList      *lisACAO;
   TTreeNode  *node_acao;
   VTEqpto    *eqpto;

   //verifica se Acao composta
   if (acao->Tipo() == acaoCOMPOSTA)
      {//verifica se a AcaoComposta está vazia
      lisACAO = acao->LisAcao();
      if (lisACAO->Count == 0) return;
      //insere TreeNode p/ acao composta
      node_acao = TreeView->Items->AddChildObject(node_obra, acao->TipoAsString(), NULL);
      //insere TreeNode p/ Acoes da Acao composta
      for (int i = 0; i < lisACAO->Count; i++)
         {
         acao = (VTAcao*)lisACAO->Items[i];
         eqpto = acao->Eqpto();
         //monta string com descrição da Acao p/ TreeNode
         TreeViewDescricaoAcao(acao, text);
         TreeView->Items->AddChildObject(node_acao, text, eqpto);
         }
      }
   else
      {//insere TreeNode p/ acao
      eqpto = acao->Eqpto();
      //monta string com descrição da Acao p/ TreeNode
      TreeViewDescricaoAcao(acao, text);
      TreeView->Items->AddChildObject(node_obra, text, eqpto);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewInsereTreeNodeAcao(TTreeNode *node_obra, VTObra *obra)
   {
   //variáveis locais
   bool   expanded = node_obra->Expanded;
   VTAcao *acao;

   //reinicia TreeNode indicado
   node_obra->DeleteChildren();
   //verifica se Obra está incorporada
   if (obra->Incorporada)
      {//Obra está incorporada: obtém lista lisUNDO
      for (int i = 0; i < obra->LisAcao->Count; i++)
         {
         acao = (VTAcao*)obra->LisAcao->Items[i];
         TreeViewInsereTreeNodeAcao(node_obra, acao);
         }
      }
   else
      {//Obra não está incorporada: obtém lista lisREDO
      for (int i = obra->LisAcao->Count - 1; i >= 0; i--)
         {
         acao = (VTAcao*)obra->LisAcao->Items[i];
         TreeViewInsereTreeNodeAcao(node_obra, acao);
         }
      }
   //expande o TreeNode
   if (expanded) node_obra->Expand(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTAlternativa *alternativa)
   {
   //vatiáveis locais
   TList      *lisOBRA;
   TTreeNode  *node_obra;
   VTObra     *obra;

   //reinicia TreeNode indicado
   node_alter->DeleteChildren();
   //loop p/ todas Obras da Alternativa
   lisOBRA = alternativa->Obras->LisObra();
   for (int i = 0; i < lisOBRA->Count; i++)
      {
      obra      = (VTObra*)lisOBRA->Items[i];
      TreeViewInsereTreeNodeObra(node_alter, obra);
      }
   //expande o Node da Alternativa para exibir as Obras
   node_alter->Expand(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTObra *obra)
   {
   //vatiáveis locais
   bool      expanded = node_alter->Expanded;
   TTreeNode *node_obra;

   //insere Node para a Obra
   node_obra = TreeView->Items->AddChildObject(node_alter, TreeViewNomeDaObra(obra), obra);
   //insere TreeNode das Acoes da Obra
   if (MenuItemObraExibeAcao->Checked)
      {
      TreeViewInsereTreeNodeAcao(node_obra, obra);
      }
   //expande o TreeNode
   if (expanded) node_obra->Expand(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TreeViewMouseDown(TObject *Sender, TMouseButton Button,
															  TShiftState Shift, int X, int Y)
	{
	//variáveis locais
	TPoint        Point;
	TTreeNode     *node;
	VTAlternativa *alternativa;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	//verifica se foi apertado o botão direito
	if (Button != mbRight) return;
	//identifica o TreeNode na posição X,Y
	if ((node = TreeView->GetNodeAt(X, Y)) == NULL) return;
	//verifica se o Node é de uma Alternativa
   if (node->Level != 1) return;
   if(progresso == NULL)
	{//cria progresso e insere no apl //DVK 2016.11.28
		progresso = DLL_NewObjProgresso(this->Owner, Parent);
		apl->Add(progresso);
	}
	//inicia progresso
	if(progresso) progresso->Start(progDEFAULT);
   //determina Alternivativa do Node selecionado
   alternativa = ((VTAlternativa*)node->Data);
   RetiraAlternativa(alternativa);
   if(progresso) progresso->Stop();
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEstudo::TreeViewNomeDaObra(VTObra *obra)
   {
   //vatiáveis locais
   AnsiString text;

//   return(text.sprintf("%4d/%02d/%02d: %s", YearOf(obra->DataInicio),
//                                            MonthOf(obra->DataInicio),
//                                            DayOf(obra->DataInicio),
//                                            obra->Nome));
   //DVK 2015.01.07 considera data de ativação
   return(text.sprintf("%4d/%02d/%02d: %s", YearOf(obra->DataAtivacao),
                                            MonthOf(obra->DataAtivacao),
                                            DayOf(obra->DataAtivacao),
                                            obra->Nome.c_str()));
   }

//---------------------------------------------------------------------------
VTObra* __fastcall TFormEstudo::TreeViewObraSelecionada(void)
   {
   //variáveis locais
   TTreeNode *node = TreeView->Selected;

   //proteção
   if (node == NULL) return(NULL);
   //verifica se o TTreeNode selecionada é de Level 2
   while (node->Level > 2) node = node->Parent;
   if (node->Level == 2) return((VTObra*)node->Data);
   //não uma Obra selecionada
   return(NULL);
   }
//---------------------------------------------------------------------------
void __fastcall TFormEstudo::TrocaRedeEqvBTporCarga(void)
   {
   //variáveis locais
   VTStkEqv    *stk_eqv;
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   try{//verifica se há redes BT equivalentes
      if (ExisteRedeEqvBT())
         {
         Aviso("As redes BT (equivalentes ou não) serão substituídas \n"
               "por cargas equivalentes");
         if(progresso == NULL)
            {//cria progresso e insere no apl //DVK 2016.11.28
            progresso = DLL_NewObjProgresso(this->Owner, Parent);
            apl->Add(progresso);
            }
         //inicia progresso
         if(progresso) progresso->Start(progDEFAULT);
         //cria objeto VTStkEqv
         if ((stk_eqv = DLL_NewObjStkEqv(apl)) != NULL)
            {
            stk_eqv->Executa();
            //destrói objeto VTStkEqv
            delete stk_eqv;
            }
         //finaliza progresso
         if(progresso) progresso->Stop();
         }
      }catch(Exception &e)
         {
		 if(progresso) progresso->Stop();
         Aviso("Não foi possível substituir as redes BT equivalentes por cargas");
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEstudo::ValidaDataTodasObras(void)
   {
   //variáveis locais
   TList         *lisOBRA;
   VTObra        *obra1, *obra2;
   VTAlternativa *alternativa;
   TList         *lisALTERNATIVA = estudo->LisAlternativa();

   //loop p/ todas Alternativas
   for (int n = 0; n < lisALTERNATIVA->Count; n++)
      {
      alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
      if (alternativa == estudo->AlternativaPadrao) continue;
      //verifica se a Alternativa possui pelo menos duas Obras
      lisOBRA = alternativa->Obras->LisObra();
      if (lisOBRA->Count <= 1) continue;
      //compara datas de todas Obras da Alternativa
      obra1 = (VTObra*)lisOBRA->Items[0];
      for (int n = 1; n < lisOBRA->Count; n++)
         {
         obra2 = (VTObra*)lisOBRA->Items[n];
//         if (obra2->DataInicio < obra1->DataInicio)
         if (obra2->DataAtivacao < obra1->DataAtivacao)
            {
            Aviso("Obra " + AnsiQuotedStr(obra2->Nome, '\'') + " com data anterior à obra " + AnsiQuotedStr(obra1->Nome, '\''));
            return(false);
            }
         //redefine obra1
         obra1 = obra2;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


//TFormSelResumoTec *formSel = new TFormSelResumoTec(NULL, apl);
//	formSel->ShowModal();
//	delete formSel;

void __fastcall TFormEstudo::ToolButton1Click(TObject *Sender)
{
	//variáveis locais
   TTreeNode     *node = TreeView->Selected;
   VTAlternativa *alternativa;
   VTObra        *obra, *ult_obra;
   TFormFinalizaObra *form;

   //proteção: determina Node da Obra
   if (node == NULL) return;
   while (node->Level > 2) node = node->Parent;
   if (node->Level != 2) return;
   //detemina Alternativa e Obra selecionada
   alternativa  = TreeViewAlternativaSelecionada();
   obra         = TreeViewObraSelecionada();
   if ((alternativa == NULL)||(obra == NULL))
	  {
	  Aviso("Selecione a obra a ser analisada");
      return;
	  }
   form = new TFormFinalizaObra(this, apl, obra);
   if (form)
   {
        form->ShowModal();
   }

}
//---------------------------------------------------------------------------

