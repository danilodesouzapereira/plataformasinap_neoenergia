// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
// ---------------------------------------------------------------------------
#include "TFormEstudoNovo.h"
#include "TFormCroqui.h"
#include "TFormCfgEstudo.h"
#include "VTFabricaFormFinaliza.h"
#include "TFormAvalia.h"
#include "TFormEditaEstudo.h"
#include "TFormEditaAlternativa.h"
#include "TFormEditaObra.h"
// #include "TFormObra.h"
#include "TFormConfig.h"
#include "TFormVisualAlt.h"
#include "TSalvaAlternativa.h"
#include "Relatorio\TFormAtivo.h"
#include "Relatorio\TFormRelObraCOPEL.h"
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\MultiObra\VTMultiObraBO.h>
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Diretorio\VTPath.h"
#include "..\Figura\VTFiguras.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTSegmento.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Stk\VTStkEqv.h"
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Stk.h"
#include <DLL_Inc\MultiObra.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/17577418
__fastcall TFormEstudoNovo::TFormEstudoNovo(TComponent* Owner, VTApl *apl, TWinControl *parent)
    : TForm(Owner)
{

    try
    {
        // salva ponteiro p/ objetos
        this->apl = apl;
        sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
        MenuItemObraExibeAcao->Checked = false;
        // inicia objetos Planejamento e Estudo
        if (!ExistePlanejamentoEstudo())
            Close();
        // insere este Form em seu parent
        if (parent)
        { // redefine Parent
            Parent = parent;
        }
        apl->Add(DLL_NewObjMultiObraBO(apl));
        // atualiza LabelEstudo
        LabelEstudo->Caption = estudo->Codigo;
        // habilita/desabilita Análise do estudo
        ButAnalise->Visible = planejamento->AnaliseEnabled;
        // salva posição inicial da janela flutuante
        janela.top = 0;
        janela.left = 0;
        janela.width = Width;
        janela.height = Height;
        // cria objetos
        lisEQP = new TList();
        lisTMP = new TList();
        lisNomesRedes = new TList();
        // marca que o usuário ainda não configurou opções p/ análise
        usuario_definiu_opcao_analise = false;
        // habilita exibição das Acoes das Obras
        HabilitaApresentacaoDasAcoes(true);
        // configura PopupMenuAnalise e PopupMenuRelatorio
        PopupMenuInicia(PopupMenuAnalise, planejamento->LisActionAnalise());
        PopupMenuInicia(PopupMenuRelatorio, planejamento->LisActionRelatorio());
        // gera aviso indicando limite de número de barras para análise de confiabilidade
        // AvisoLimiteDeBarras();
        // desabilita botão para salvar alternativa como rede original
        ButSalvaAlt->Visible = false;
        if (estudo->Tipo == estudoPLANEJAMENTO)
        { // substitui eventuais redes BT equivalentes por suas cargas correpondentes
            TrocaRedeEqvBTporCarga();
            // habilita botão para salvar alternativa como rede original
            ButSalvaAlt->Visible = true;
        }
        // verifica se o Estudo tem a Alternativa padrão
        if (estudo->AlternativaPadrao == NULL)
        { // cria Alternativa padrão (sem obras)
            InsereAlternativa(AnsiString("[Rede Original]"), AnsiString(""), true);
        }
        // //desabilita o editor
        // // habilita/desabilita apresentação do editor
        // ActionEditor->Enabled = false;
        // ActionEditor->Execute();
        if (sinap_child != NULL)
        {
            // insere nosso botao do editor como botao de´editor
            sinap_child->InsereBotaoEditor(ButEditor);
        }
        // habilita botoes em desenvolvimento
        HabilitaButtonsDev();
        // le periodo de análise
        LeArqPeriodoAnalise();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
// https://sinapsisenergia.teamwork.com/#tasks/17577418
__fastcall TFormEstudoNovo::~TFormEstudoNovo(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    try
    { // salva período de análise
        // SalvaArqPeriodoAnalise();
        // restaura rede original
        // if (estudo) estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
        // retira ActionObraAlterada de VTEdita
        HabilitaApresentacaoDasAcoes(false);
        // apaga moldura
        GraficoMoldura();
        // salva parâmetros do Form
        if (geral)
            geral->Infoset->SaveToFile(this);
        // fecha FormEditor  //DVK 2015.03.10 descomentado
        // DLL_CloseFormEditor(NULL);
        if (sinap_child != NULL)
        {
            sinap_child->HabilitaEditor(false);
            // remove nosso botao do editor como botao de´editor
            sinap_child->RemoveBotaoEditor(ButEditor);
        }
        // destrói lista sem destruir seus objetos
        if (lisTMP)
        {
            delete lisTMP;
            lisTMP = NULL;
        }
        if (lisEQP)
        {
            // LimpaTList(lisEQP);
            delete lisEQP;
            lisEQP = NULL;
        }
        if (lisNomesRedes)
        {
            // LimpaTList(lisNomesRedes);
            delete lisNomesRedes;
            lisNomesRedes = NULL;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAlignDownExecute(TObject *Sender)
{
    // verifica se a janela estava alinhada em cima
    if (Align == alTop)
        Align = alNone;
    // alinha janela em baixo
    Align = alBottom;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAlignNoneExecute(TObject *Sender)
{
    // cancela alinhamento da janela (flutuante)
    Align = alNone;
    Top = janela.top;
    Left = janela.left;
    Width = janela.width;
    Height = janela.height;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAlignLeftExecute(TObject *Sender)
{
    // verifica se a janela estava alinhada à direita
    if (Align == alRight)
        Align = alNone;
    // alinha janela à esquerda
    Align = alLeft;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAlignRightExecute(TObject *Sender)
{
    // verifica se a janela estava alinhada à esquerda
    if (Align == alLeft)
        Align = alNone;
    // alinha janela à direita
    Align = alRight;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAlignUpExecute(TObject *Sender)
{
    // verifica se a janela estava alinhada em baixo
    if (Align == alBottom)
        Align = alNone;
    // alinha janela em cima
    Align = alTop;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422298
void __fastcall TFormEstudoNovo::ActionAlternativaCopiaExecute(TObject *Sender)
{
    // variáveis locais
    TFormEditaAlternativa *FormEdtTxt;
    VTAlternativa *alternativa, *alternativa_nova;

    try
    {
        // verifica se há uma Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
            return;
        // verifica se Alternativa padrão
        if (alternativa == estudo->AlternativaPadrao)
        {
            Aviso("A rede original não pode ser copiada");
            return;
        }
        // solicita nome da nova Alternativa
        alternativa_nova = estudo->CopiaAlternativa(alternativa);
        alternativa_nova->Codigo = NomeProximaAlternativa();
        FormEdtTxt = new TFormEditaAlternativa(this, apl, alternativa_nova);
        if (FormEdtTxt->ShowModal() == mrOk)
        { // marca que Obras foi alterada
            alternativa->Obras->Modificada = true;
            // reinicia TView
            TreeViewInicia();
        }
        else
        {
            estudo->RetiraAlternativa(alternativa_nova);
        }
        // destrói FormEdtTxt
        delete FormEdtTxt;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
void __fastcall TFormEstudoNovo::ActionAlternativaEditaExecute(TObject *Sender)
{
    // variáveis locais
    TTreeNode *node;
    VTAlternativa *alternativa = NULL;
    TFormEditaAlternativa *formEditaAlternativa;

    try
    {
        // verifica se há uma Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
            return;
        // verifica se é a Alternativa da rede original
        if (alternativa == estudo->AlternativaPadrao)
        {
            Aviso("Não é permitido renomear a rede original");
            return;
        }
        // exibe FormEdtTxt p/ usuário editar a identificação da Alternativa
        // cria form para editar nome e justificativa do estudo
        formEditaAlternativa = new TFormEditaAlternativa(this, apl, alternativa);
        if (formEditaAlternativa->ShowModal() == mrOk)
        {
            // marca que Obras foi alterada
            alternativa->Obras->Modificada = true;
        }
        // destrói FormEdtTxt
        delete formEditaAlternativa;
        // atualiza TView
        if ((node = TreeViewExisteTreeNode(alternativa)) != NULL)
        {
            node->Text = alternativa->Codigo;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
void __fastcall TFormEstudoNovo::ActionAlternativaInsereExecute(TObject *Sender)
{
    // variáveis locais
    bool repita = true;
    // TFormEdtTxt *FormEdtTxt;
    VTAlternativa *alternativa;
    TFormEditaAlternativa *formEditaAlternativa;

    try
    { // solicita nome da nova Alternativa
        formEditaAlternativa = new TFormEditaAlternativa(this, apl, NULL);
        formEditaAlternativa->Nome = NomeProximaAlternativa();
        while (repita)
        {
            if (formEditaAlternativa->ShowModal() == mrCancel)
            {
                repita = false;
            }
            else
            { // verifica se existe Alternativa com mesmo nome
                // cria Alternativa
                InsereAlternativa(formEditaAlternativa->Nome, formEditaAlternativa->Justificativa);
                // reinicia TView
                TreeViewInicia();
                repita = false;
            }
        }
        // destrói TFormEditaAlternativa
        delete formEditaAlternativa;
    }
    catch (Exception &e)
    {
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAlternativaRetiraExecute(TObject *Sender)
{
    // variáveis locais
    VTAlternativa *alternativa;

    try
    {
        // determina Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
        {
            Aviso("Selecione a alternativa a ser excluída");
            return;
        }
        // retira a Alternativa do Estudo
        RetiraAlternativa(alternativa);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAnaliseCfgConfiabilidadeExecute(TObject *Sender)
{
    Aviso("Falta implementar");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAnaliseCfgPeriodoExecute(TObject *Sender)
{
    try
    {
        // esconde este Form
        Hide();
        ConfiguraOpcaoAnalise();
        // torna a exibir este Form
        Show();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionAnaliseDefaultExecute(TObject *Sender)
{
    // variáveis locais
    TFormAvalia *FormAvalia;

    try
    {
        // valida datas de todas Obras
        if (!ValidaDataTodasObras())
            return;
        // funde as Acoes das Alternativa
        FundeAcoesDasAlternativas();
        // torna ativa a Alternativa padrão
        SelecionaAlternativaPadrao();
        // esconde este Form
        Hide();
        // verifica se o período de análise está definido
        // if (!usuario_definiu_opcao_analise)
        // { // exibe tela p/ usuário configurar opções da análise das alternativas
        // ConfiguraOpcaoAnalise();
        // }
        // if (usuario_definiu_opcao_analise)
        // { // cria FormAvalia
        FormAvalia = new TFormAvalia(this, apl);
        FormAvalia->ShowModal();
        // destrói FormAvalia
        delete FormAvalia;
        // }
        // torna a exibir este Form
        Show();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionCfgEstudoExecute(TObject *Sender)
{
    TFormCfgEstudo *form = NULL;

    form = new TFormCfgEstudo(NULL, apl);
    // sem real necessidade de ser modal...
    form->ShowModal();

    delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionCriarCroquiExecute(TObject *Sender)
{
    // variaveis locais
    AnsiString arq_croqui;
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

    try
    {
        // define onde salvar a imagem
        Hide();
        SaveDialog->InitialDir = path->DirExporta();
        // escolhe o nome

        SaveDialog->FileName = NomeCroqui();
        SaveDialog->DefaultExt = "jpg";
        if (SaveDialog->Execute())
        { // verifica se o arquivo ja existe
            arq_croqui = SaveDialog->FileName;
            if (FileExists(arq_croqui))
            {
                if (Confirma("Já existe um arquivo com mesmo nome.", "Deseja substituir ?")
                    != idYes)
                {
                    ActionCriarCroquiExecute(NULL);
                    return;
                }
            }
            // força extensão do arquivo para jpg
            ChangeFileExt(arq_croqui, SaveDialog->DefaultExt);
            CriaCroqui(arq_croqui);
            MostraCroqui(arq_croqui);
        }
        Show();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// https://sinapsisenergia.teamwork.com/#/tasks/16335731
// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionDesbloqueiaObraExecute(TObject *Sender)
{
    // variáveis locais
    VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
    TTreeNode *node;
    VTAlternativa *alternativa;
    VTObra *obra;
    VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));

    try
    {
        // determina Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
            return;
        // determina Obra selecionada
        if ((obra = TreeViewObraSelecionada()) == NULL)
            return;
        // verifica se a obra já esta finalizada
        if ((obra->Bloqueada) || (obra->Finalizada))
        {
            // verifica se o usuario quer mesmo fazer isso
            if (Confirma("Liberar a edição da obra irá desfazer a edição dos items",
                "Deseja continuar?") == idYes)
            {
                // desbloqueia Obra e reinicializa o mult
                if (multiObraBO != NULL)
                {
                    multiObraBO->ReiniciaMultiObra(obra);
                }
                EstadoFormObraSelecionada(obra);
                if (sinap_child != NULL)
                {
                    sinap_child->EndEditorBlock(alternativa);
                    sinap_child->EndEditorBlock(obra);
                }
                AtualizaTreeView(obra);
            }
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionEditorExecute(TObject *Sender)
{
    // variáveis locais
    bool planejamento = true;

    try
    {
        // verifica se deve exibir ou esconder o Editor
        if (ButEditor->Down)
        { // exibe o Editor
            // DLL_NewFormEditor(this, apl, Parent);
            if ((estudo->Tipo == estudoRECLAMACAO_BT) || (estudo->Tipo == estudoRECLAMACAO_MT))
            { // DVK 2015.03.03 insere FormEditor no avô do Parent
                DLL_NewFormEditor(apl->Form, apl, Parent->Parent->Parent, planejamento);
            }
            else
                DLL_NewFormEditor(apl->Form, apl, Parent, planejamento);
        }
        else
        { // esconde o Editor
            DLL_CloseFormEditor(NULL);
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::ActionEstudoEditaExecute(TObject *Sender)
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
void __fastcall TFormEstudoNovo::ActionEstudoEditaExecute(TObject *Sender)
{ // DVK 2018.02.23 insere justificativa para estudo
    // variáveis locais
    TFormEditaEstudo *formEditaEstudo;
    TTreeNode *node_estudo;

    try
    { // cria form para editar nome e justificativa do estudo
        formEditaEstudo = new TFormEditaEstudo(this, apl, estudo);
        formEditaEstudo->ShowModal();
        // atualiza LabelEstudo
        LabelEstudo->Caption = estudo->Codigo;
        // atualiza node estudo
        node_estudo = TreeViewExisteTreeNode(estudo);
        if (node_estudo != NULL)
        {
            node_estudo->Text = estudo->Codigo;
        }
        // destrói form
        delete formEditaEstudo;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16770548
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormEstudoNovo::ActionFinalizaObraExecute(TObject *Sender)
{
    // variáveis locais
    TTreeNode *node;
    VTObra *obra;
    VTFabricaFormFinaliza *fabrica;
    TForm *formFinaliza, *formMercado;
    // TList *lisRede;
    // VTRede *rede, *rede_sem_mercado;
    // VTCarga *carga;
    AnsiString redes_sem_taxa;
    bool nao_taxa = true;
    VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));

    try
    { // determina Alternativa selecionada
        if (TreeViewAlternativaSelecionada() == NULL)
            return;
        // determina Obra selecionada
        if ((obra = TreeViewObraSelecionada()) == NULL)
            return;
        // Verifica se existe alguma rede sem taxa
        if (ExisteRedeSemTaxa(redes_sem_taxa, obra))
        {
            if (Confirma("As seguintes redes não possuem taxa de crescimento: " + redes_sem_taxa,
                "Deseja inserir os valores ?") == idYes)
            { // exibe tela do Mercado
                formMercado = DLL_NewFormMercado(this, apl, SelecionaParentParaJanela());
                formMercado->Show();
                return;
            }
        }
        // verifica se há eqptos novos e isolados
        if (ExisteEqptoNovoIsolado(obra))
        {
            Aviso("Há equipamentos que foram inseridos e estão isolados. "
                "Conecte-os a uma rede para que sejam devidamente contabilizados.");
            return;
        }
        // verifica se a obra já esta finalizada
        if (obra->Finalizada)
        {
            Aviso("A obra já foi finalizada");
            return;
        }
        try
        {
            // fecha o editor
            DLL_CloseFormEditor(NULL);
            // cria a fabrica de forms
            fabrica = NewObjFabricaFormFinaliza(apl);
            // cria o form finaliza obra
            formFinaliza = fabrica->NewFormFinalizaObra(this, apl, obra);
            // mostra modal
            if (formFinaliza->ShowModal() == mrOk)
            { // em caso positivo, marca que Obras foram alteradas
                obra->Obras->Modificada = true;
                // se a obra foi finalizada, bloqueia
                // if (obra->Finalizada)
                // {
                // obra->Bloqueada = true;
                // }
                // sempre bloqueia obra, pois o atributo Finalizada serve para validar itens da obra
                obra->Bloqueada = true;
                // atualiza
                EstadoFormObraSelecionada(obra);
                AtualizaTreeView(obra);
            }
            // atualiza
            obra->Finalizada = multiObraBO->IsObraFinalizada(obra);
            AtualizaTreeView(obra);
            // marca que Obras foram alteradas
            obra->Obras->Modificada = true;
            delete formFinaliza;
            // pensar se devo reaproveitar os forms...
            delete fabrica;
        }
        catch (Exception &e)
        {
            Erro(e.Message);
        }
        catch (...)
        {
            Erro("Erro ao criar o form.");
        }

    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionObraAlteradaExecute(TObject *Sender)
{
    // variáveis locais
    TTreeNode *node;
    VTObra *obra = (VTObra*)Sender;

    try
    {
        // proteção
        if (obra == NULL)
            return;
        // determina TreeNode da Obra
        if ((node = TreeViewExisteTreeNode(obra)) == NULL)
            return;
        // verifica se o TreeNode da Obra e o TrreNode selecionado
        if (node != TreeView->Selected)
        {
            if (TreeView->Selected->Parent != node)
            {
                return;
            }
        }
        // atualiza Acoes da Obra
        TreeViewInsereTreeNodeAcao(node, obra);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionObraColorEnabledExecute(TObject *Sender)
{
    // variáveis locais
    VTAlternativa *alternativa;
    VTObra *obra;
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

    try
    {
        // determina Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
            return;
        // configura todas Obras da Alternativa
        alternativa->Obras->ColorEnabled = MenuItemObraColorEnabled->Checked;
        // atualiza o gráfico
        grafico->Redraw();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionObraEditaExecute(TObject *Sender)
{
    // variáveis locais
    TTreeNode *node;
    VTObra *obra;

    try
    {
        // determina Alternativa selecionada
        if (TreeViewAlternativaSelecionada() == NULL)
            return;
        // determina Obra selecionada
        if ((obra = TreeViewObraSelecionada()) == NULL)
            return;
        // edita atributos da Obra conforme o tipo de planejamento
        if (EditaObra(obra))
        { // marca que Obras foi alterada
            obra->Obras->Modificada = true;
            // atualiza TreeView
            if ((node = TreeViewExisteTreeNode(obra)) != NULL)
            {
                node->Text = TreeViewNomeDaObra(obra);
            }
            // força seleção do TreeNode da Obra editada
            TreeView->Selected = TreeViewExisteTreeNode(obra);
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionObraExibeAcaoExecute(TObject *Sender)
{
    try
    {
        // habilita/desabilita apresentação das Acoes das Obras
        HabilitaApresentacaoDasAcoes(MenuItemObraExibeAcao->Checked);
        // reinicia TreeView
        TreeViewInicia();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionObraInsereExecute(TObject *Sender)
{
    // variáveis locais
    VTAlternativa *alternativa;
    VTObra *obra;

    try
    {
        // determina Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
            return;
        // verifica se é a Alternativa da rede original
        if (alternativa == estudo->AlternativaPadrao)
        {
            Aviso("Não é permitido criar obra na rede original");
            return;
        }
        // cria uma nova Obra
        obra = alternativa->Obras->InsereObra(alternativa->Obras->DataFinal);
        // edita atributos da Obra conforme o tipo de planejamento
        if (!EditaObra(obra))
        { // destrói Obra criada
            alternativa->Obras->RetiraObra(obra);
            return;
        }
        // atualiza TreeView
        TreeViewInsereTreeNodeObra(TreeViewExisteTreeNode(alternativa), obra);
        // expande TreeNode da Alternativa
        // TreeView->Selected->Expand(true);
        // força seleção do TreeNode da nova Obra
        TreeView->Selected = TreeViewExisteTreeNode(obra);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// --------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15599530-
// https://sinapsisenergia.teamwork.com/#tasks/15938942
void __fastcall TFormEstudoNovo::ActionObraRetiraExecute(TObject *Sender)
{
    // variáveis locais
    VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
    VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
    TTreeNode *node = TreeView->Selected;
    VTAlternativa *alternativa;
    VTObra *obra, *ult_obra;
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

    try
    {
        // proteção: determina Node da Obra
        if (node == NULL)
            return;
        while (node->Level > 2)
            node = node->Parent;
        if (node->Level != 2)
            return;
        // detemina Alternativa e Obra selecionada
        alternativa = TreeViewAlternativaSelecionada();
        obra = TreeViewObraSelecionada();
        if ((alternativa == NULL) || (obra == NULL))
        {
            Aviso("Selecione a obra a ser excluída");
            return;
        }
        // verifica se é a última Obra da Alternativa ou uma Obra sem nenhuma Acao
        ult_obra = alternativa->Obras->ObraFinal();
        if ((obra != ult_obra) && (obra->LisAcao->Count > 0))
        {
            Aviso(" A obra " + AnsiQuotedStr(obra->Nome, '\'') + " não pode ser excluída");
            return;
        }
        // pede confirmação do usuário
        if (Confirma("Obra " + AnsiQuotedStr(obra->Nome, '\''), "Confirma a exclusão desta obra ?")
            != IDYES)
            return;
        // retira Obra selecionada
        if (sinap_child != NULL)
        {
            sinap_child->EndEditorBlock(obra);
        }
        alternativa->Obras->RetiraObra(obra);
        // desbloqueia Obra e reinicializa o mult
        // movido para o TObras
        // if (multiObraBO != NULL)
        // {
        // multiObraBO->ReiniciaMultiObra(obra);
        // }
        // seleciona TreeNode pai
        TreeView->Selected = node->Parent;
        // retira TreeNode da Obra
        node->Delete();
        // redesenha a rede
        if (grafico != NULL)
        {
            // grafico->Redraw();
            grafico->AreaRedeDefine();
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionRelatorioExecute(TObject *Sender)
{
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca *licenca = licencas->ExisteLicenca();

    // variáveis locais
    TForm *form;

    // esconde este Form
    Visible = false;
    try
    {
        // verifica se é Copel
        if ((licenca != NULL) && (licenca->Empresa == COPEL))
        {
            form = new TFormRelObraCOPEL(this, estudo, apl);
        }
        else
        {
            // cria objeto para exibir relatório do planejamento
            form = new TFormAtivo(this, estudo, apl);
        }
        if (form != NULL)
        {
            form->ShowModal();
            delete form;
        }

    }
    catch (Exception &e)
    {
        Erro("O relatório de planejamento não pode ser criado");
    }
    // reapresenta este Form
    Visible = true;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18604874
void __fastcall TFormEstudoNovo::ActionSalvaAltExecute(TObject *Sender)
{ // DVK 2017.01.11
    // variáveis locais
    TSalvaAlternativa *salva_alt;
    AnsiString cod_original, autor_original;
    bool novo;

    try
    {
        // verifica se a alternativa ativa nao é a rede original
        if (estudo->AlternativaAtiva == estudo->AlternativaPadrao)
        {
            Aviso("Selecione uma alternativa diferente da rede original");
            return;
        }
        // executa todas as obras da alternativa
        estudo->AlternativaAtiva->ReconfiguraRedeTodaObra();
        // renomeia estudo e salva estado do estudo
        cod_original = estudo->Codigo;
        autor_original = estudo->Autor;
        novo = estudo->Novo;
        estudo->Codigo = "Estudo correspondente à rede principal";
        estudo->Autor = "Sinapsis";
        // cria objeto
        salva_alt = new TSalvaAlternativa(apl);
        // salva alternativa ativa como rede original em uma nova base
        salva_alt->Executa();
        // destroi objeto
        delete salva_alt;
        // retorna nome original e estado do estudo
        estudo->Codigo = cod_original;
        estudo->Autor = autor_original;
        estudo->Novo = novo;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ActionVisualAltAnoExecute(TObject *Sender)
{ // DVK 2014.04.09
    // variáveis locais
    TFormVisualAlt *form;

    // esconde este Form
    Hide();
    try
    { // cria objeto para exibir visualização de alternativa
        if ((form = new TFormVisualAlt(this, apl, estudo)) != NULL)
        {
            form->Show();
        }
    }
    catch (Exception &e)
    {
        Erro("A janela de visualização de alternativa não pode ser criada");
    }
    // reapresenta este Form ao destruir TFormVisualAlt
    // Visible = true;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16176123
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormEstudoNovo::ActionVisualizaObraExecute(TObject *Sender)
{
    // variáveis locais
    VTObra *obra;
    VTFabricaFormFinaliza *fabrica;
    TForm *formFinaliza, *formMercado;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));

    try
    {
        // determina Alternativa selecionada
        if (TreeViewAlternativaSelecionada() == NULL)
            return;
        // determina Obra selecionada
        if ((obra = TreeViewObraSelecionada()) == NULL)
            return;

        // verifica se a obra já esta finalizada
        if (!obra->Finalizada)
        {
            Aviso("Finalize a obra para visualizar");
            return;
        }
        try
        {
            // fecha o editor
            DLL_CloseFormEditor(NULL);
            // cria a fabrica de forms
            fabrica = NewObjFabricaFormFinaliza(apl);
            // cria o form finaliza obra
            formFinaliza = fabrica->NewFormFinalizaObra(this, apl, obra);
            // mostra modal
            if (formFinaliza->ShowModal() == mrOk)
            {
                // em caso positivo
                // marca que Obras foi alterada
                obra->Obras->Modificada = true;
                // se a obra foi finalizada, bloqueia
                // if (obra->Finalizada)
                // {
                // obra->Bloqueada = true;
                // }
                // sempre mantem a obra bloqueada, pois o atributo Finalizada serve para validar itens da obra
                obra->Bloqueada = true;
                // atualiza
                EstadoFormObraSelecionada(obra);
                AtualizaTreeView(obra);
            }
            // atualiza
            obra->Finalizada = multiObraBO->IsObraFinalizada(obra);
            AtualizaTreeView(obra);
            // marca que Obras foi alterada
            obra->Obras->Modificada = true;
            delete formFinaliza;
            // pensar se devo reaproveitar os forms...
            delete fabrica;
        }
        catch (Exception &e)
        {
            Erro(e.Message);
        }
        catch (...)
        {
            Erro("Erro ao criar o form.");
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudoNovo::AlternativaSelecionada(void)
{ // variáveis locais
    VTAlternativa *alt = NULL;

    try
    {
        TTreeNode *node = TreeView->Selected;

        alt = AlternativaSelecionada(node);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }

    return alt;
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudoNovo::AlternativaSelecionada(TTreeNode *node)
{
    // VTAlternativa *alt = NULL;

    try
    {
        // proteção
        if (node == NULL)
            return (NULL);
        // determina TTreeNode de Level 0
        while (node->Level > 1)
            node = node->Parent;
        // retorna Alternativa associada ao TreeNode
        if (node->Level == 1)
            return ((VTAlternativa*)node->Data);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    // não uma Alternativa selecionada
    return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::AtualizaEstado(bool gravando_base)
{
    try
    {
        // exibe/esconde PanelAviso
        PanelAviso->Visible = gravando_base;
        // verifica se a base de dados está sendo gravada
        if (gravando_base)
        { // garante que a rede original esteja selecionada (e não uma alternativa)
            SelecionaAlternativaPadrao();
            // proteção: verifica se a Alternativa padrão foi selecionada
            if (estudo->AlternativaAtiva != estudo->AlternativaPadrao)
            {
                estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
                DLL_CloseFormEditor(NULL);
            }
            // desabilita este Form para evitar que o usuário altere o estudo
            Enabled = false;
        }
        else
        { // reabilita este Form
            Enabled = true;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormEstudoNovo::AtualizaTreeView(VTObra *obra)
{
    TTreeNode *node;

    try
    {
        TreeView->Items->BeginUpdate();
        // atualiza TreeView
        if ((node = TreeViewExisteTreeNode(obra)) != NULL)
        {
            node->Text = TreeViewNomeDaObra(obra);

            // se a obra está bloqueada
            if (obra->Bloqueada)
            {
                // se a obra foi finalizada (fechou)
                if (obra->Finalizada)
                { // muda a figura do node
                    node->ImageIndex = 4;
                    node->SelectedIndex = 4; // 2;

                }
                else
                { // muda a figura do node (pendente)
                    node->ImageIndex = 3;
                    node->SelectedIndex = 3; // 2;
                }
            }
        }
        // força seleção do TreeNode da Obra editada
        TreeView->Selected = TreeViewExisteTreeNode(obra);
        TreeView->Items->EndUpdate();
        TreeView->Refresh();
        // TreeView->Repaint();
        // TreeViewInicia();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ButEditorMouseEnter(TObject *Sender)
{
    // VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
    // VTObra *obra = NULL;
    //
    // try
    // {
    // // determina Obra selecionada
    // obra = TreeViewObraSelecionada();
    // //atualiza o estado do botao devido a alguma restricao
    // EstadoFormObraSelecionada(obra);
    // }
    // catch(Exception &e)
    // {
    // AnsiString funcao = __PRETTY_FUNCTION__;
    // Erro(e.Message + " em " + funcao);
    // }
    // catch(...)
    // {
    // AnsiString funcao = __PRETTY_FUNCTION__;
    // Erro(funcao);
    // }
}

// ---------------------------------------------------------------------------
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::AvisoLimiteDeBarras(void)
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

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/17577418
bool __fastcall TFormEstudoNovo::ConfiguraOpcaoAnalise(void)
{
    // variáveis locais
    bool sucesso = false;
    TFormConfig *Form;

    try
    { // cria TFormConfig e exibe como janela modal
        Form = new TFormConfig(NULL, apl);
        sucesso = (Form->ShowModal() == mrOk);
        delete Form;
        // marca que o usuário configurou opção somente se ele confirmar
        if (sucesso)
        {
            ExibeDataAnalise();
            // usuario_definiu_opcao_analise = true;
        }
    }
    catch (Exception &e)
    {
        Aviso("Não foi possível exibir o formulário de configuração");
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstudoNovo::CriaCroqui(AnsiString filepath)
{
    // variaveis locais
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
    TFormCroqui *formCroqui;
    TRect rect;

    try
    {
        // cria form croqui
        formCroqui = new TFormCroqui(NULL, apl);
        // exporta imagem atual para o formCroqui
        rect.init(formCroqui->ImageCroqui->Left, formCroqui->ImageCroqui->Top,
            formCroqui->ImageCroqui->Left + formCroqui->ImageCroqui->Width,
            formCroqui->ImageCroqui->Top + formCroqui->ImageCroqui->Height);
        grafico->Exporta(formCroqui->ImageCroqui->Canvas, rect);
        // salva imagem
        formCroqui->SalvaArquivo(filepath);
        // destroi form
        delete formCroqui;

        return (FileExists(filepath));
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return false;
}
//// ---------------------------------------------------------------------------
// bool __fastcall TFormEstudoNovo::EditaObra(VTObra *obra)
// {
// // variáveis locais
// bool sucesso;
// TFormObra *FormObra;
//
// // verifica se o Planejamento foi configurado para edição externa da Obra
// if (planejamento->ActionEdtObra)
// { // executa Action  associada ao Planejamento
// planejamento->ActionEdtObra->OnExecute(obra);
// // verifica se edição foi confirmada ou cancelada
// sucesso = (obra->ModalResult == mrOk);
// }
// else
// { // planejamento básico do Sinap
// FormObra = new TFormObra(this, apl, obra);
// sucesso = (FormObra->ShowModal() == mrOk);
// // destrói TFormObra
// delete FormObra;
// }
// return (sucesso);
// }

// ---------------------------------------------------------------------------
bool __fastcall TFormEstudoNovo::EditaObra(VTObra *obra)
{
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
    // variáveis locais
    bool sucesso = false;
    TFormEditaObra *FormObra;

    try
    {
        // planejamento básico do Sinap
        FormObra = new TFormEditaObra(this, apl, obra);
        sucesso = (FormObra->ShowModal() == mrOk);
        // se houve sucesso, precisa atualizar o label de ultima obra
        if (sucesso)
        {
            ExibeDataAnalise();
            // atualiza o gráfico
            grafico->Redraw();
        }

        // destrói TFormObra
        delete FormObra;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }

    return (sucesso);
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::EditAnoFimClick(TObject *Sender)
 {
 EditAnoFim->SetFocus();
 }
 //---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::EditAnoFimExit(TObject *Sender)
 {
 LeDataAnalise();
 Application->ProcessMessages();
 }
 //---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::EditAnoIniExit(TObject *Sender)
 {
 LeDataAnalise();
 Application->ProcessMessages();
 }
 //---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::EditAnoIniClick(TObject *Sender)
 {
 EditAnoIni->SetFocus();
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::EstadoFormAlternativaSelecionada(VTAlternativa *alternativa)
{
    try
    {
        if (alternativa != NULL)
        {
            ButInsAlt->Enabled = true;
            ButExcluirAlt->Enabled = true;
            ButEdtAlt->Enabled = true;
            ButDupliAlt->Enabled = true;
            ButSalvaAlt->Enabled = true;
            ButObra->Enabled = true;
        }
        else
        {
            ButInsAlt->Enabled = true;
            ButExcluirAlt->Enabled = false;
            ButEdtAlt->Enabled = false;
            ButDupliAlt->Enabled = false;
            ButSalvaAlt->Enabled = false;
            ButObra->Enabled = false;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16176123
void __fastcall TFormEstudoNovo::EstadoFormObraSelecionada(VTObra *obra)
{
    VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

    try
    {
        if (obra != NULL)
        {
            ButExcObra->Enabled = true;
            ButEdtObra->Enabled = true;
            // if (obra->Bloqueada)
            // {
            // ActionEditor->Enabled = false;
            // ActionEditor->Execute();
            // }
            // else
            // {
            // ActionEditor->Enabled = true;
            // }
            if (sinap_child != NULL)
            {
                sinap_child->HabilitaEditor(!obra->Bloqueada);
            }
            ButFinalizaObra->Enabled = !obra->Finalizada;
            ButDesbloqObra->Enabled = obra->Bloqueada;
            ButVisualizaObra->Enabled = obra->Finalizada;
        }
        else
        {
            ButExcObra->Enabled = false;
            ButEdtObra->Enabled = false;
            // ButEditor->Enabled = false;
            // ActionEditor->Enabled = false;
            // ActionEditor->Execute();
            if (sinap_child != NULL)
            {
                sinap_child->HabilitaEditor(false);
            }
            ButFinalizaObra->Enabled = false;
            ButDesbloqObra->Enabled = false;
            ButVisualizaObra->Enabled = false;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16770548
bool __fastcall TFormEstudoNovo::ExisteEqptoNovoIsolado(VTObra *obra)
{
    // variáveis locais
    bool existe = false;
    TList *lisACAO = obra->LisAcao;
    TList *lisBAR_ISO;
    VTEqbar *eqbar;
    VTEqpto *eqpto;
    VTAcao *acao;
    VTLigacao *ligacao;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    lisBAR_ISO = redes->LisBarraIsolada();

    for (int n = 0; n < lisACAO->Count; n++)
    {
        acao = (VTAcao*)lisACAO->Items[n];
        if (acao->Tipo() != acaoINSERE)
            continue;
        eqpto = acao->Eqpto();
        if (!eqpto->StatusNovo())
            continue;
        if (eqpto->TipoLigacao())
        {
            ligacao = (VTLigacao*)eqpto;
            if (ligacao->rede == redes->RedeIsolada())
            {
                existe = true;
                break;
            }
        }
        if (eqpto->TipoEqbar())
        {
            eqbar = (VTEqbar*)eqpto;
            if (lisBAR_ISO->IndexOf(eqbar->pbarra) >= 0)
            {
                existe = true;
                break;
            }
        }
    }

    return existe;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstudoNovo::ExistePlanejamentoEstudo(void)
{
    // variáveis locais
    TList *lisESTUDO;

    try
    {
        // assume valores seguros
        planejamento = NULL;
        estudo = NULL;
        // determina objeto Planejamento
        planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
        if (planejamento != NULL)
        {
            lisESTUDO = planejamento->LisEstudo();
            if (lisESTUDO->Count == 1)
                estudo = (VTEstudo*)lisESTUDO->First();
        }
        return (estudo != NULL);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstudoNovo::ExisteRedeEqvBT(void)
{
    // variáveis locais
    VTRede *rede;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisREDE;
    bool possui = false;
    TList *lisRedeCluster;

    try
    {
        lisREDE = new TList;
        redes->LisRede(lisREDE);
        TList *lisCLUSTER = redes->LisCluster();
        VTCluster *cluster;

        // remove as redes dos clusters da lista de redes a serem verificadas
        for (int nc = 0; nc < lisCLUSTER->Count; nc++)
        {
            cluster = (VTCluster*)lisCLUSTER->Items[nc];
            lisRedeCluster = cluster->LisRede();
            for (int nr = 0; nr < lisRedeCluster->Count; nr++)
            {
                rede = (VTRede*)lisRedeCluster->Items[nr];
                try
                {
                    if ((rede->TipoRede != NULL) && (rede->TipoRede->Segmento == redeSEC))
                    {
                        rede->Show[eqp_VISIBLE] = false;
                    }
                    lisREDE->Remove(rede);
                }
                catch (...)
                {
                    // teste
                }
            }
        }

        for (int n = 0; n < lisREDE->Count; n++)
        {
            rede = (VTRede*)lisREDE->Items[n];
            // if (rede->Carregada) continue;  //DVK 2016.11.28 comentada, pois qualquer
            // rede secundária será trocada por carga
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
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }

    return (possui);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16770548
bool __fastcall TFormEstudoNovo::ExisteRedeSemTaxa(AnsiString &redes_sem_taxa, VTObra *obra)
{
    // variaveis locais
    VTRede *rede, *rede_sem_mercado;
    VTCarga *carga;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
    TList *lisRede;
    bool nao_taxa = false;

    lisEQP->Clear();
    lisNomesRedes->Clear();
    lisRede = obra->LisRede();
    mercado->IniciaRedes(redes);
    for (int nr = 0; nr < lisRede->Count; nr++)
    {
        rede = (VTRede*)lisRede->Items[nr];
        rede->LisEqpto(lisEQP, eqptoCARGA);
        for (int n = 0; n < lisEQP->Count; n++)
        {
            carga = (VTCarga*)lisEQP->Items[n];
            if ((carga->crescimento == NULL) || (carga->crescimento->Nulo() == true))
            {
                if (lisNomesRedes->IndexOf(rede) < 0)
                {
                    nao_taxa = true;
                    lisNomesRedes->Add(rede);
                    break;
                }
            }
        }
    }
    if (lisNomesRedes->Count > 0)
    {
        for (int nr = 0; nr < lisNomesRedes->Count; nr++)
        {
            rede_sem_mercado = (VTRede*)lisNomesRedes->Items[nr];
            if (nao_taxa)
            { // primeira rede
                redes_sem_taxa = rede_sem_mercado->Codigo;
                nao_taxa = false;
            }
            else
            {
                redes_sem_taxa += ", " + rede_sem_mercado->Codigo;
            }
        }
    }
    return (redes_sem_taxa.Length() > 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::ExibeDataAnalise(void)
{
    try
    {
        EditAnoIni->Text = IntToStr(estudo->AnoInicial);
        EditAnoFim->Text = IntToStr(estudo->AnoFinal);
        // atualliza Label c/ data da última obra
        LabelDataUltObra->Caption = "Ano da última obra: " + IntToStr(estudo->AnoObraFinal);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::FormClose(TObject *Sender, TCloseAction &Action)
{
    try
    {
        // redefine a rede com sua configuração original
        estudo->DefineAlternativaAtiva(estudo->AlternativaPadrao, NULL);
        // destrói o Form
        Action = caFree;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    // variáveis locais
    AnsiString meta_class_name;
    TMetaClass *owner_meta_class = Owner->ClassType();
    TMetaClass *form_meta_class = (__classid(TForm));

    try
    {
        ///não permite o fechamento deste Form
        CanClose = false;
        // verifica se o Owner é um Form
        while (owner_meta_class)
        {
            if (owner_meta_class == form_meta_class)
            { // fecha o Owner
                ((TForm*)Owner)->Close();
                break;
            }
            else
            {
                owner_meta_class = owner_meta_class->ClassParent();
            }
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::FormIniciaPosicao(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    try
    {
        // define posição default da janela flutuante
        janela.top = 0;
        janela.left = 0;
        janela.width = Width;
        janela.height = Height;
        if (geral)
        { // lê parâmetros do Form
            if (geral->Infoset->LoadFromFile(this))
            {
                switch (Align)
                {
                case alLeft:
                    ActionAlignLeft->Execute();
                    break;
                case alRight:
                    ActionAlignRight->Execute();
                    break;
                case alTop:
                    ActionAlignUp->Execute();
                    break;
                case alBottom:
                    ActionAlignDown->Execute();
                    break;
                case alNone: // salva última
                default:
                    janela.top = Top;
                    janela.left = Left;
                    janela.width = Width;
                    janela.height = Height;
                    break;
                }
                return;
            }
        }
        // posiciona o Form sem alinhamento
        ActionAlignNoneExecute(NULL);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::FormShow(TObject *Sender)
{
    try
    {
        // posiciona o Form
        FormIniciaPosicao();
        // apresenta as Alternativas
        Inicia();
        // gera aviso indicando limite de número de barras para análise de confiabilidade
        // AvisoLimiteDeBarras();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::FundeAcoesDasAlternativas(void)
{
    // variáveis locais
    VTAlternativa *alternativa;
    try
    {
        TList *lisALTERNATIVA = estudo->LisAlternativa();

        // loop p/ todas Alternativas
        for (int n = 0; n < lisALTERNATIVA->Count; n++)
        {
            alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
            if (alternativa == estudo->AlternativaPadrao)
                continue;
            alternativa->Obras->FundeObras();
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::GraficoMoldura(VTEqpto *eqpto)
{
    // variáveis locais
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

    try
    {
        // apaga moldura
        grafico->Moldura(eqpto);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::GraficoZoom(VTEqpto *eqpto)
{
    // variáveis locais
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

    try
    {
        // apaga moldura
        if (eqpto != NULL)
            grafico->SelecionaZoom(eqpto);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::HabilitaApresentacaoDasAcoes(bool enabled)
{
    // variáveis locais
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

    try
    {
        // proteção
        if (edita == NULL)
            return;
        // verifica se deve exibir ou não as Acoes das Obras no TrreView
        if (enabled)
        { // insere ActionObraAlterada em VTEdita p/ atualizar as Acoes das Obras
            edita->ActionOnChangeRedeInsere(ActionObraAlterada);
        }
        else
        { // retira ActionObraAlterada de VTEdita p/ não atualizar as Acoes das Obras
            edita->ActionOnChangeRedeRetira(ActionObraAlterada);
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::HabilitaButtonsDev(void)
{
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca *licenca = licencas->ExisteLicenca();

    // butCfgExtras->Visible =  (licenca->Tipo == tipoMASTER);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::Inicia(void)
{
    // variáveis locais
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

    try
    {
        grafico->Disable();
        // if(estudo->Tipo == estudoPLANEJAMENTO)
        // {//substitui eventuais redes BT equivalentes por suas cargas correpondentes
        // TrocaRedeEqvBTporCarga();
        // //habilita botão para salvar alternativa como rede original
        // ButSalvaAlt->Visible = true;
        // }
        // //verifica se o Estudo tem a Alternativa padrão
        // if (estudo->AlternativaPadrao == NULL)
        // {//cria Alternativa padrão (sem obras)
        // InsereAlternativa("[Rede Original]", true);
        // }
        // verifica se as obras estão realmente finalizadas
        IniciaObras();
        // atualiza a data de analise
        ExibeDataAnalise();
        // atualiza TreeView
        TreeViewInicia();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::IniciaObras(void)
{
    VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
    TList *lisALTERNATIVA = estudo->LisAlternativa();
    TList *lisOBRA;
    VTObra *obra;
    VTAlternativa *alternativa;

    try
    {
        for (int n = 0; n < lisALTERNATIVA->Count; n++)
        {
            alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
            lisOBRA = alternativa->Obras->LisObra();
            for (int i = 0; i < lisOBRA->Count; i++)
            {
                obra = (VTObra*)lisOBRA->Items[i];
                obra->Finalizada = multiObraBO->IsObraFinalizada(obra);
            }
        }
    }
    catch (...)
    {

    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
VTAlternativa* __fastcall TFormEstudoNovo::InsereAlternativa(AnsiString codigo,
    AnsiString justificativa, bool padrao)
{
    // variáveis locais
    VTAlternativa *alternativa = NULL;

    try
    {
        // cria uma nova Alternativa
        if ((alternativa = estudo->InsereAlternativa(codigo, padrao)) != NULL)
        { // define nova Alternativa como ativa
            estudo->DefineAlternativaAtiva(alternativa, NULL);
            // define justificativa
            alternativa->Justificativa = justificativa;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return (alternativa);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/17577418
void __fastcall TFormEstudoNovo::LeArqPeriodoAnalise(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
    AnsiString ano_ini, ano_fim;

    try
    { // determina valores default
        // EditAnoIni->Text = IntToStr(estudo->AnoInicial);
        // EditAnoFim->Text = IntToStr(estudo->AnoFinal);
        if (geral)
        { // lê parâmetros do arquivo
            if (geral->Infoset->LoadFromFile("PeriodoAnalise"))
            {
                if (geral->Infoset->GetInfo("AnoIni", ano_ini))
                    estudo->AnoInicial = StrToInt(ano_ini);
                if (geral->Infoset->GetInfo("AnoFim", ano_fim))
                    estudo->AnoFinal = StrToInt(ano_fim);
            }
        }
        // LeDataAnalise();
    }
    catch (...)
    {
        // nada
    }
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TFormEstudoNovo::LeDataAnalise(void)
 {
 // variáveis locais
 int ano_ini, ano_fim;

 try
 { // lê ano inicial
 ano_ini = StrToInteger(EditAnoIni->Text);
 }
 catch (Exception &e)
 {
 Aviso("Ano inicial com valor inválido");
 return (false);
 }
 try
 { // lê ano final
 ano_fim = StrToInteger(EditAnoFim->Text);
 }
 catch (Exception &e)
 {
 Aviso("Ano final com valor inválido");
 return (false);
 }
 // varifica se ano inicial é maior que ano final
 if (ano_fim < ano_ini)
 {
 Aviso("Ano final menor que ano inicial");
 return (false);
 }
 // anos válidos
 estudo->AnoInicial = ano_ini;
 estudo->AnoFinal = ano_fim;
 return (true);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::MostraCroqui(AnsiString filepath)
{
    // variaveis locais
    TFormCroqui *formCroqui;

    try
    {
        // esconde form
        Hide();
        // cria form para apresentar o croqui
        formCroqui = new TFormCroqui(NULL, apl);
        // le arquivo formCroqui
        formCroqui->LeArquivo(filepath);
        formCroqui->ShowModal();
        delete formCroqui;
        Show();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEstudoNovo::NomeCroqui(void)
{
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    VTAlternativa *altAux;
    VTObra *obraAux;
    int num = 2;
    AnsiString nome = "";
    AnsiString filename = "";
    AnsiString filenameAux = "";

    try
    {
        // verifica do menos especifico para o mais
        obraAux = ObraSelecionada();
        altAux = AlternativaSelecionada();
        if (obraAux != NULL)
        {
            nome = RemoveCaracterInvalidoFilename(obraAux->Nome);

        }
        else
        {
            if (altAux != NULL)
            {
                nome = RemoveCaracterInvalidoFilename(altAux->Codigo);
            }
            else
            {
                if (estudo != NULL)
                {
                    nome = RemoveCaracterInvalidoFilename(estudo->Codigo);
                }
            }
        }
        // protecao final
        if (nome.IsEmpty())
        {
            nome = "croqui";
        }
        // verifica se ja existe
        filenameAux = path->DirExporta() + "\\";
        filename = filenameAux + nome + ".jpg";

        while (FileExists(filename))
        {
            nome += "_" + IntToStr(num);
            num++;
            filename = filenameAux + nome + ".jpg";
        }

    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return nome;

}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEstudoNovo::NomeProximaAlternativa(void)
{
    // variáveis locais
    int count = 1;
    AnsiString codigo = "";

    try
    {
        do
        { // define nome
            codigo.sprintf("Alternativa %02d", count++);
        }
        while (estudo->ExisteAlternativa(codigo) != NULL);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return (codigo);
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TFormEstudoNovo::ObraSelecionada(void)
{
    try
    {
        // variáveis locais
        TTreeNode *node = TreeView->Selected;

        // proteção
        if (node == NULL)
            return (NULL);
        // verifica se o TTreeNode selecionada é de Level 2
        while (node->Level > 2)
            node = node->Parent;
        if (node->Level == 2)
            return ((VTObra*)node->Data);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    // não uma Obra selecionada
    return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::PopupMenuInicia(TPopupMenu *PopupMenu, TList *lisACTION)
{
    // variáveis locais
    TAction *ActionExterna;
    TMenuItem *MenuItem;

    try
    {
        // inclui MenuItems p/ Actions externas cadastradas em VTPlanejamento
        for (int n = 0; n < lisACTION->Count; n++)
        {
            ActionExterna = (TAction*)(lisACTION->Items[n]);
            // insere novo MenuItem
            MenuItem = new TMenuItem(PopupMenu);
            // associa MenuItem com Action externa
            MenuItem->Action = ActionExterna;
            // salva Action em MenuItem::Tag
            MenuItem->Tag = int(ActionExterna);
            MenuItem->Hint = ActionExterna->Hint;
            MenuItem->Caption = ActionExterna->Caption;
            MenuItem->AutoCheck = false;
            MenuItem->Checked = false;
            PopupMenu->Items->Add(MenuItem);
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::PopupMenuRelatorioPopup(TObject *Sender)
{
    // LeDataAnalise();
    Application->ProcessMessages();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::RetiraAlternativa(VTAlternativa *alternativa)
{
    try
    {
        // não permite exlusão da Alternativa padrão
        if (alternativa == estudo->AlternativaPadrao)
        {
            Aviso("A alternativa com a rede original não pode ser excluída");
            return;
        }
        if (Confirma("Alternativa selecionada: " + AnsiQuotedStr(alternativa->Codigo, '\''),
            "Deseja excluir esta alternativa ?") != IDYES)
            return;
        // elimina a Alternativa do Estudo
        estudo->RetiraAlternativa(alternativa);
        // reinicia TreeView
        TreeViewInicia();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

TWinControl* __fastcall TFormEstudoNovo::SelecionaParentParaJanela(void)
{
    // variáveis locais
    VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

    return (MultiPanel->PanelAtivo);
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudoNovo::SelecionaAlternativaPadrao(void)
{
    // variáveis locais
    TTreeNode *node;
    VTAlternativa *alternativa;

    try
    {
        // determina TreeNode c/ a Alternativa padrão
        for (int n = 0; n < TreeView->Items->Count; n++)
        {
            node = TreeView->Items->Item[n];
            if (node->Level != 1)
                continue;
            alternativa = (VTAlternativa*)node->Data;
            if (alternativa == estudo->AlternativaPadrao)
            {
                if (TreeView->Selected != node)
                    TreeView->Selected = node;
                return (alternativa);
            }
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TFormEstudoNovo::TreeViewAlternativaSelecionada(void)
{
    try
    {
        // variáveis locais
        TTreeNode *node = TreeView->Selected;
        VTAlternativa *alt = NULL;

        alt = AlternativaSelecionada(node);

        EstadoFormAlternativaSelecionada(alt);

        return alt;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17887287
VTAlternativa* __fastcall TFormEstudoNovo::TreeViewAlternativaSelecionada(TTreeNode *node)
{
    // variáveis locais
    VTAlternativa *alt = NULL;
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
    try
    {
        alt = AlternativaSelecionada(node);
        EstadoFormAlternativaSelecionada(alt);
        // seleciona gráfico de topologia
        if (!sinap_child)
            sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
        if (sinap_child)
            sinap_child->ActionTopo->Execute();
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }

    return alt;
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormEstudoNovo::TreeViewChange(TObject *Sender, TTreeNode *Node)
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

// https://sinapsisenergia.teamwork.com/#/tasks/15933980
// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
    // variáveis locais
    int ano_rede;
    VTAlternativa *alternativa;
    VTObra *obra;
    TTreeNode *node = TreeView->Selected;
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
    VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
    VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
    VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
    VTProgresso *progresso;

    // OBS: versão da função para o caso de TCelulas não filtrar as redes visíveis

    try
    {
        // determina Alternativa selecionada
        if ((alternativa = TreeViewAlternativaSelecionada()) == NULL)
        {
            if (sinap_child != NULL)
            {
                sinap_child->HabilitaEditor(false);
            }
            return;
        }
        // detemina Obra selecionada
        obra = TreeViewObraSelecionada();
        // altera o grafico de acordo com a obra selecionada
        EstadoFormObraSelecionada(obra);
        if (sinap_child != NULL)
        {
            sinap_child->EndEditorBlock(estudo->AlternativaAtiva);
            sinap_child->EndEditorBlock(estudo->ObraAtiva);
        }
        // verifica se Alternativa/Obra selecionada é igual a atual
        if (alternativa == estudo->AlternativaAtiva)
        {
            if (sinap_child != NULL)
            {
                sinap_child->BeginEditorBlock(estudo->AlternativaAtiva);
            }
            if (obra == estudo->ObraAtiva)
            {
                if (sinap_child != NULL)
                {
                    sinap_child->BeginEditorBlock(estudo->ObraAtiva);
                }
                return;
            }
        }
        progresso = DLL_NewObjProgresso(this, NULL);
        if (progresso)
        {
            progresso->Start(progDEFAULT);
        }
        // desabilita atualização do Grafico para evitar redesenhar a rede na troca de Alternativa/Obra
        grafico->Disable();
        // Esconde/Mostra os comentários da obra selecionada
        figuras->ObraAtiva(obra);
        // verifica se Alternativa selecionada é diferente da atual
        if (alternativa == estudo->AlternativaAtiva)
        { // redefine Alternativa/Obra
            estudo->DefineAlternativaAtiva(alternativa, obra);
        }
        else
        { // redefine Alternativa/Obra
            estudo->DefineAlternativaAtiva(alternativa, obra);
            // TCelulas precisa ser reiniciado com a rede da nova Alternativa
            grafico->AreaRedeDefine();
        }
        // exibe o gráfico de topologia (que vai mostrar apenas as redes da Obra selecionada)
        grafico->Show(grafTOPO);
        // Se for alternativa padrão, bloqueia o editor
        if (alternativa != NULL)
        {
            sinap_child->BeginEditorBlock(alternativa);
        }
        if (obra != NULL)
        {
            if (obra->Bloqueada)
            {
                sinap_child->BeginEditorBlock(obra);
            }
            else
            {
                sinap_child->EndEditorBlock(obra);
                sinap_child->EndEditorBlock(alternativa);
            }
        }
        // habilita/desabilita inserção de Obras
        ButObra->Enabled = ((alternativa != NULL) && (alternativa != estudo->AlternativaPadrao));
        // configura ano da rede no Mercado e  reinicia anos disponíveis p/ cálculo de fluxo de potência
        if (obra == NULL)
            obra = alternativa->Obras->ObraFinal();
        if (obra != NULL)
        { // determina ano de ativação da obra
            ano_rede = obra->AnoAtivacao;
        }
        else
        { // assume ano inicial do Mercado
            ano_rede = mercado->AnoInicial;
        }
        // verifica se deve configurar o mercado
        if ((mercado->AnoAtual != ano_rede) || (mercado->AnoRede != ano_rede))
        { // altera ano atual do Mercado
            mercado->AnoRede = ano_rede;
            mercado->AnoAtual = ano_rede;
            // configura Stk p/ reiniciar os anos disponíveis para cálculo de fluxo
            sinap_child->ActionMercadoChanged->Execute();
        }
        // verifica se TreeNode associado a uma Acao (nível 3)
        if (node->Level == 3)
        { // exibe moldura
            GraficoMoldura((VTEqpto*)node->Data);
        }
        else
        { // apaga moldura
            GraficoMoldura();
        }
        if (progresso)
        {
            progresso->Stop();
            delete progresso;
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewChanging(TObject *Sender, TTreeNode *Node,
    bool &AllowChange)
{ // DVK 2014.06.04
    // variaveis locais
    AnsiString class_name;
    TForm *form;

    try
    {
        // verifica se a rede original foi selecionada
        if (TreeViewAlternativaSelecionada(Node) != estudo->AlternativaPadrao)
        {
            AllowChange = true;
            return;
        }
        // verifica se existem Forms AlocaBC, AlocaRT ou Balanceamento abertos
        for (int n = 0; n < Screen->FormCount; n++)
        {
            class_name = Screen->Forms[n]->ClassName();
            if (class_name.AnsiCompareIC("TFormAlocaCapacitor") == 0)
            { // verifica se o Form n é um FormAlocaBC
                AllowChange = false;
                Aviso("O módulo de Alocação de Capacitor não pode ser usado na Rede Original.");
                return;
            }
            if (class_name.AnsiCompareIC("TFormAlocaRegulador") == 0)
            { // verifica se o Form n é um FormAlocaRT
                AllowChange = false;
                Aviso("O módulo de Alocação de Regulador não pode ser usado na Rede Original.");
                return;
            }
            if (class_name.AnsiCompareIC("TFormBalanceamento") == 0)
            { // verifica se o Form n é um FormAlocaBC
                AllowChange = false;
                Aviso("O módulo de Balanceamento de Carga não pode ser usado na Rede Original.");
                return;
            }
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewDblClick(TObject *Sender)
{
    // variáveis locais
    TTreeNode *node = TreeView->Selected;

    try
    {
        // proteção
        if (node == NULL)
            return;
        // verifica se TreeNode de nível 3 ou 4
        if ((node->Level == 3) || (node->Level == 3))
        { // executa Zoom
            GraficoZoom((VTEqpto*)node->Data);
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewDescricaoAcao(VTAcao *acao, AnsiString &text)
{
    try
    {
        // variáveis locais
        VTEqpto *eqpto = acao->Eqpto();

        text.sprintf("%03d %s: %s ID=%05d %s", acao->Index + 1, acao->TipoAsString().c_str(),
            eqpto->TipoAsString().c_str(), eqpto->Id, AnsiQuotedStr(eqpto->Codigo, '\'').c_str());
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormEstudoNovo::TreeViewExisteTreeNode(void *data)
{
    // variáveis locais
    TTreeNode *node;

    try
    {
        for (int n = 0; n < TreeView->Items->Count; n++)
        {
            node = TreeView->Items->Item[n];
            if (node->Data == data)
                return (node);
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewInicia(void)
{
    // variáveis locais
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
    TTreeNode *node_estudo, *node_alter;
    VTAlternativa *alternativa;
    TList *lisALTERNATIVA = estudo->LisAlternativa();

    try
    {
        // bloqueia tratamento do evento OnChange durante a montagem do TreeView
        TreeView->OnChange = NULL;
        // reinicia TreeView
        TreeView->Items->Clear();
        // insere TreeNode do Estudo
        node_estudo = TreeView->Items->Insert(NULL, estudo->Codigo);
        node_estudo->Data = estudo;
        node_estudo->ImageIndex = -1;
        node_estudo->SelectedIndex = -1;
        // cria um TreeNode p/ cada Alternativa
        for (int n = 0; n < lisALTERNATIVA->Count; n++)
        {
            alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
            // insere TreeNode inicial da Alternativa
            node_alter = TreeView->Items->AddChildObject(node_estudo, alternativa->Codigo,
                alternativa);
            node_alter->ImageIndex = -1;
            node_alter->SelectedIndex = -1;
            // monta TreeNode com Obras da alternativa
            TreeViewInsereTreeNodeObra(node_alter, alternativa);
        }
        // pré-seleciona Alternativa ativa
        TreeView->Selected = TreeViewExisteTreeNode(estudo->AlternativaPadrao);
        // ordena Alternativas
        // node_estudo->AlphaSort();
        // libera tratamento do evento OnChange durante a montagem do TreeView
        TreeView->OnChange = TreeViewChange;
        // força chamada de TReeViewChange p/atualizar o gráfico
        TreeViewChange(TreeView, TreeView->Selected);
        if (grafico)
        {
            grafico->Redraw();
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewInsereTreeNodeAcao(TTreeNode *node_obra, VTAcao *acao)
{
    // variáveis locais
    AnsiString text;
    TList *lisACAO;
    TTreeNode *node_acao;
    VTEqpto *eqpto;

    try
    {
        if (MenuItemObraExibeAcao->Checked)
        {
            // verifica se Acao composta
            if (acao->Tipo() == acaoCOMPOSTA)
            { // verifica se a AcaoComposta está vazia
                lisACAO = acao->LisAcao();
                if (lisACAO->Count == 0)
                    return;
                // insere TreeNode p/ acao composta
                node_acao = TreeView->Items->AddChildObject(node_obra, acao->TipoAsString(), NULL);
                // insere TreeNode p/ Acoes da Acao composta
                for (int i = 0; i < lisACAO->Count; i++)
                {
                    acao = (VTAcao*)lisACAO->Items[i];
                    eqpto = acao->Eqpto();
                    // monta string com descrição da Acao p/ TreeNode
                    TreeViewDescricaoAcao(acao, text);
                    TreeView->Items->AddChildObject(node_acao, text, eqpto);
                }
            }
            else
            { // insere TreeNode p/ acao
                eqpto = acao->Eqpto();
                // monta string com descrição da Acao p/ TreeNode
                TreeViewDescricaoAcao(acao, text);
                TreeView->Items->AddChildObject(node_obra, text, eqpto);
            }
        }
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewInsereTreeNodeAcao(TTreeNode *node_obra, VTObra *obra)
{
    // variáveis locais
    bool expanded = node_obra->Expanded;
    VTAcao *acao;

    try
    {
        // reinicia TreeNode indicado
        node_obra->DeleteChildren();
        // verifica se Obra está incorporada
        if (obra->Incorporada)
        { // Obra está incorporada: obtém lista lisUNDO
            for (int i = 0; i < obra->LisAcao->Count; i++)
            {
                acao = (VTAcao*)obra->LisAcao->Items[i];
                TreeViewInsereTreeNodeAcao(node_obra, acao);
            }
        }
        else
        { // Obra não está incorporada: obtém lista lisREDO
            for (int i = obra->LisAcao->Count - 1; i >= 0; i--)
            {
                acao = (VTAcao*)obra->LisAcao->Items[i];
                TreeViewInsereTreeNodeAcao(node_obra, acao);
            }
        }
        // expande o TreeNode
        if (expanded)
            node_obra->Expand(false);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewInsereTreeNodeObra(TTreeNode *node_alter,
    VTAlternativa *alternativa)
{
    // vatiáveis locais
    TList *lisOBRA;
    TTreeNode *node_obra;
    VTObra *obra;

    try
    {
        // reinicia TreeNode indicado
        node_alter->DeleteChildren();
        // loop p/ todas Obras da Alternativa
        lisOBRA = alternativa->Obras->LisObra();
        for (int i = 0; i < lisOBRA->Count; i++)
        {
            obra = (VTObra*)lisOBRA->Items[i];
            TreeViewInsereTreeNodeObra(node_alter, obra);
        }
        // expande o Node da Alternativa para exibir as Obras
        node_alter->Expand(false);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormEstudoNovo::TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTObra *obra)
{
    // vatiáveis locais
    bool expanded = node_alter->Expanded;
    TTreeNode *node_obra;

    try
    {
        // insere Node para a Obra
        node_obra = TreeView->Items->AddChildObject(node_alter, TreeViewNomeDaObra(obra), obra);

        // se a obra está bloqueada
        if (obra->Bloqueada)
        {
            // se a obra foi finalizada (fechou)
            if (obra->Finalizada)
            { // muda a figura do node
                node_obra->ImageIndex = 4;
                node_obra->SelectedIndex = 4; // 2;

            }
            else
            { // muda a figura do node (pendente)
                node_obra->ImageIndex = 3;
                node_obra->SelectedIndex = 3; // 2;
            }
        }
        else
        {
            // muda a figura do node
            node_obra->ImageIndex = -1;
            node_obra->SelectedIndex = -1;
        }
        // insere TreeNode das Acoes da Obra
        if (MenuItemObraExibeAcao->Checked)
        {
            TreeViewInsereTreeNodeAcao(node_obra, obra);
        }
        // expande o TreeNode
        if (expanded)
            node_obra->Expand(false);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TreeViewMouseDown(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y)
{
    // variáveis locais
    TPoint Point;
    TTreeNode *node;
    VTAlternativa *alternativa;
    VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

    try
    {
        // verifica se foi apertado o botão direito
        if (Button != mbRight)
            return;
        // identifica o TreeNode na posição X,Y
        if ((node = TreeView->GetNodeAt(X, Y)) == NULL)
            return;
        // verifica se o Node é de uma Alternativa
        if (node->Level != 1)
            return;
        if (progresso == NULL)
        { // cria progresso e insere no apl //DVK 2016.11.28
            progresso = DLL_NewObjProgresso(this->Owner, Parent);
            apl->Add(progresso);
        }
        // inicia progresso
        if (progresso)
            progresso->Start(progDEFAULT);
        // determina Alternivativa do Node selecionado
        alternativa = ((VTAlternativa*)node->Data);
        RetiraAlternativa(alternativa);
        if (progresso)
            progresso->Stop();
    }
    catch (Exception &e)
    {
        if (progresso)
            progresso->Stop();
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        if (progresso)
            progresso->Stop();
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEstudoNovo::TreeViewNomeDaObra(VTObra *obra)
{
    // vatiáveis locais
    AnsiString text;

    // return(text.sprintf("%4d/%02d/%02d: %s", YearOf(obra->DataInicio),
    // MonthOf(obra->DataInicio),
    // DayOf(obra->DataInicio),
    // obra->Nome));
    // DVK 2015.01.07 considera data de ativação
    return (text.sprintf("%4d/%02d/%02d: %s", YearOf(obra->DataAtivacao),
        MonthOf(obra->DataAtivacao), DayOf(obra->DataAtivacao), obra->Nome.c_str()));
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TFormEstudoNovo::TreeViewObraSelecionada(void)
{
    VTObra *obra = NULL;

    try
    {
        obra = ObraSelecionada();

        EstadoFormObraSelecionada(obra);
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }

    return obra;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstudoNovo::TrocaRedeEqvBTporCarga(void)
{
    // variáveis locais
    VTStkEqv *stk_eqv;
    VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

    try
    { // verifica se há redes BT equivalentes
        if (ExisteRedeEqvBT())
        {
            Aviso("As redes BT (equivalentes ou não) serão substituídas \n"
                "por cargas equivalentes");
            if (progresso == NULL)
            { // cria progresso e insere no apl //DVK 2016.11.28
                progresso = DLL_NewObjProgresso(this->Owner, Parent);
                apl->Add(progresso);
            }
            // inicia progresso
            if (progresso)
                progresso->Start(progDEFAULT);
            // cria objeto VTStkEqv
            if ((stk_eqv = DLL_NewObjStkEqv(apl)) != NULL)
            {
                stk_eqv->Executa();
                // destrói objeto VTStkEqv
                delete stk_eqv;
            }
            // finaliza progresso
            if (progresso)
                progresso->Stop();
        }
    }
    catch (Exception &e)
    {
        if (progresso)
            progresso->Stop();
        AnsiString funcao = __PRETTY_FUNCTION__;
        Aviso("Não foi possível substituir as redes BT equivalentes por cargas");
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        if (progresso)
            progresso->Stop();
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstudoNovo::ValidaDataTodasObras(void)
{
    // variáveis locais
    TList *lisOBRA;
    VTObra *obra1, *obra2;
    VTAlternativa *alternativa;
    TList *lisALTERNATIVA = estudo->LisAlternativa();
    bool sucesso = false;
    try
    {
        // loop p/ todas Alternativas
        for (int n = 0; n < lisALTERNATIVA->Count; n++)
        {
            alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
            if (alternativa == estudo->AlternativaPadrao)
                continue;
            // verifica se a Alternativa possui pelo menos duas Obras
            lisOBRA = alternativa->Obras->LisObra();
            if (lisOBRA->Count <= 1)
                continue;
            // compara datas de todas Obras da Alternativa
            obra1 = (VTObra*)lisOBRA->Items[0];
            for (int n = 1; n < lisOBRA->Count; n++)
            {
                obra2 = (VTObra*)lisOBRA->Items[n];
                // if (obra2->DataInicio < obra1->DataInicio)
                if (obra2->DataAtivacao < obra1->DataAtivacao)
                {
                    Aviso("Obra " + AnsiQuotedStr(obra2->Nome, '\'') +
                        " com data anterior à obra " + AnsiQuotedStr(obra1->Nome, '\''));
                    return (false);
                }
                // redefine obra1
                obra1 = obra2;
            }
        }
        sucesso = true;
    }
    catch (Exception &e)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(e.Message + " em " + funcao);
    }
    catch (...)
    {
        AnsiString funcao = __PRETTY_FUNCTION__;
        Erro(funcao);
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
// eof
