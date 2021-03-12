// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Zona\VTZonas.h"
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Zona.h"
// ---------------------------------------------------------------------------
#include "TFormBalanceamento.h"
#include "TFormResultadoBalanc.h"
#include "TCarregamento.h"
#include "VTBalanceamento.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormBalanceamento::TFormBalanceamento(TComponent* form_owner, VTApl *apl_owner,
    TWinControl *parent) : TFormEdt(form_owner, parent)
{
    // salva ponteiro p/ objeto
    apl = NewObjApl(this, apl_owner);
    // cria objetos
    apl->Add(arvore = DLL_NewObjArvore(apl));
    apl->Add(ordena = DLL_NewObjOrdena(apl));
    apl->Add(zonas = DLL_NewObjZonas(apl));
    apl->Add(NewObjBalanceamento(apl));
    // cria lista
    lisRedeBalanc = new TList();
    // verifica planejamento
    // if(! VerificaPlanejamento()) Close();
    // salva Obra originalmente associado a Edita
    // edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));
    // obra_original = edita->Obra;
    // cria objeto Obra e associa com objeto Edita
    // obra        = DLL_NewObjObra(apl);
    // edita->Obra = obra;
    // força seleção de mouse
    ActionEdt->Execute();
    // alinhamento
    ActionAlignLeft->Execute();
    // inicia
    Inicia();
    // posiciona o Form
    FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18129916
__fastcall TFormBalanceamento::~TFormBalanceamento(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
    VTGrafico *graf = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
    VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

    // atualiza flow
    flow->UpToDate = false;
    // apaga molduras
    graf->Moldura();
    // restaura Obra original ao objeto Edita
    // edita->Obra = obra_original;
    // salva parâmetros do Form
    if (geral)
        geral->Infoset->SaveToFile(this);
    // destroi lista sem destruir objs
    if (lisRedeBalanc)
    {
        delete lisRedeBalanc;
        lisRedeBalanc = NULL;
    }
    // libera a memoria alocada
    // if (obra)    {delete obra;     obra = NULL;}

    if (apl)
    {
        delete apl;
        apl = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::ActionBalancearExecute(TObject *Sender)
{
    // variaveis locais
    VTRede *rede;
    TFormResultadoBalanc *formResBalanc;

    // proteção
    if (TViewRedes->Items->Count < 0)
        return;
    if (!VerificaPlanejamento())
        return;
    // verifica se existe uma Rede associada ao TreeNode
    if ((rede = TViewRedeSelecionada()) == NULL)
    {
        Aviso("Selecione uma rede");
        return;
    }
    /*
    // DVK: comentado, pois nao consigo resolver o problema das redes que ja foram balanceadas:
    // se uma segunda rede nao efetiva balanceamento, a primeira que ja foi volta a ser desbalanceada.
    // entao é melhor o usuario repetir balanceamento...
    //
    // verifica se a rede ja foi balanceada
    if (ExisteRedeBalanceada(rede))
    {
        Aviso("A rede " + rede->Codigo + " já foi balanceada");
        return;
    }
    */
    // esconde este Form  p/ não esconder janela de resultados
    Visible = false;
    // cria formResBalanc
    formResBalanc = new TFormResultadoBalanc(NULL, apl, rede);
    // exibe se houve balanceamento //DVK 2015.08.05
    if (formResBalanc->sucesso)
        formResBalanc->ShowModal();
    if (formResBalanc->ModalResult == mrOk)
    { // insere rede na lisRedeBalanc
        if(lisRedeBalanc->IndexOf(rede) < 0)
            lisRedeBalanc->Add(rede);
    }
    // destroi formResBalanc
    delete formResBalanc;
    // reapresenta este Form
    Visible = true;
    AtualizaDados(rede);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::ActionHelpExecute(TObject *Sender)
{
    // variaveis locais
    TForm *formHelp;

    formHelp = DLL_NewFormHelp(this, apl, "Balanceamento_de_Carga");
    if (formHelp)
        formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::ActionSairExecute(TObject *Sender)
{
    // confirmar e sair
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::ActionEdtExecute(TObject *Sender)
{
    /*
     //
     variáveis locais
     VTStk *stk = (VTStk*)apl->GetObject(__classid(VTStk));

     try{//se necessário, cria um objeto TEdtBusca
     if (edt_busca != NULL) delete edt_busca;
     //cria EdtBusca p/ controle do mouse
     edt_busca = (VTEdt*)stk->NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
     //atualiza indicação de mouse ativo e força botão Down
     AtualizaIndicacaoEditorOn(butEdt);
     }catch (Exception &e)
     {
     //nada a fazer
     } */
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::ActionSelEqptoExecute(TObject *Sender)
{ /*
     //variáveis locais
     TList   *lisEQP = (TList*)Sender;
     VTEqpto *eqpto;

     //proteção
     if (lisEQP == NULL) return;
     //insere Medidor p/ todas as Barras selecionadas em lisEQP
     for (int n = 0; n < lisEQP->Count; n++)
     {
     eqpto = (VTEqpto*)lisEQP->Items[n];
     //atualiza o combobox
     //      if(AtualizaComboBox(eqpto)) break;
     } */
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::AnalisaCarregamento(VTRede *rede)
{
    // var locais
    TCarregamento carreg(apl);

    // proteção
    if (!rede)
        return;
    // limpa variaveis
    sa = CZero();
    sb = CZero();
    sc = CZero();
    s3 = CZero();
    // analisa carregamento máximo
    carreg.AnalisaRede(rede);
    carreg.GetCarregamentoABC_kva(&sa, &sb, &sc);
    carreg.GetCarregamento3F_kva(&s3);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::AtualizaDados(VTRede *rede)
{
    // analisa a rede
    AnalisaCarregamento(rede);
    // preenche LView
    PreencheLViewOriginal(rede);
    // preenche charts
    PreencheChartsOriginal();
    // grafico potencia aparente
    PreencheGraficoPotAparente(rede);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TFormBalanceamento::ExisteRede(AnsiString cod_rede, TList *lisREDE)
{ // DVK 2015.09.03
    // variaveis locais
    VTRede *rede;

    for (int nr = 0; nr < lisREDE->Count; nr++)
    { // verifica se existe uma rede com o cod_rede
        rede = (VTRede*)lisREDE->Items[nr];
        if (rede->Codigo.AnsiCompareIC(cod_rede) == 0)
            return (rede);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBalanceamento::ExisteRedeBalanceada(VTRede* rede)
{
    // variaveis locais
    VTRede *rede_bal;

    for (int nr = 0; nr < lisRedeBalanc->Count; nr++)
    { // procura rede na lista
        rede_bal = (VTRede*)lisRedeBalanc->Items[nr];
        if (rede == rede_bal)
            return (true);
    }
    return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::FormClose(TObject *Sender, TCloseAction &Action)
{
    // destroi este form
    Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::FormIniciaPosicao(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    if (geral)
    { // lê parâmetros do Form
        if (geral->Infoset->LoadFromFile(this))
            return;
    }
    // posiciona o Form sem alinhamento
    ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::FormResize(TObject *Sender)
{
    // redefine largura dos charts
    chartPantes->Width = PanelPQ->Width / 2;
    chartQantes->Width = PanelPQ->Width / 2;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::FormShow(TObject *Sender)
{
    // nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::Inicia(void)
{
    // limpa lisRedeBalanc
    lisRedeBalanc->Clear();
    // inicia TView
    TViewRedesInicia();
    // simula click no primeiro node
    TViewRedes->Selected = TViewRedes->TopItem->GetNext();
    TViewRedesClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::PreencheChartsOriginal(void)
{
    // proteção
    if (IsComplexZero(sa.real(), sa.imag()))
        sa = CZero();
    if (IsComplexZero(sb.real(), sb.imag()))
        sb = CZero();
    if (IsComplexZero(sc.real(), sc.imag()))
        sc = CZero();

    // potência ativa
    SeriesCarregP->Clear();
    SeriesCarregP->Add(sa.real(), "Fase A");
    SeriesCarregP->Add(sb.real(), "Fase B");
    SeriesCarregP->Add(sc.real(), "Fase C");

    // potência reativa
    SeriesCarregQ->Clear();
    SeriesCarregQ->Add(sa.imag(), "Fase A");
    SeriesCarregQ->Add(sb.imag(), "Fase B");
    SeriesCarregQ->Add(sc.imag(), "Fase C");
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::PreencheLViewOriginal(VTRede *rede)
{
    // var local
    AnsiString str_zona;
    TListItem *item;
    double media, deseq;
    double carr_sa, carr_sb, carr_sc;
    double delta_sa, delta_sb, delta_sc;
    VTBalanceamento *balanceamento = (VTBalanceamento*)apl->GetObject(__classid(VTBalanceamento));

    // calcula o carregamento e o desvio máximo em relação à media
    if (Abs(s3) > 0.)
    {
        carr_sa = Abs(sa) / Abs(s3);
        carr_sb = Abs(sb) / Abs(s3);
        carr_sc = Abs(sc) / Abs(s3);
        media = (carr_sa + carr_sb + carr_sc) / 3.;
        delta_sa = Abs(carr_sa - media) / media;
        delta_sb = Abs(carr_sb - media) / media;
        delta_sc = Abs(carr_sc - media) / media;
    }
    else
    {
        carr_sa = carr_sb = carr_sc = media = 0.;
        delta_sa = delta_sb = delta_sc = 0.;
    }
    deseq = delta_sa;
    deseq = ((deseq > delta_sb) ? deseq : delta_sb);
    deseq = ((deseq > delta_sc) ? deseq : delta_sc);

    // determina tipo da zona
    str_zona = balanceamento->TipoZona(rede);
    // limpa list view
    listViewPQantes->Items->Clear();
    // insere dados
    item = listViewPQantes->Items->Add();
    item->Caption = rede->Codigo;
    item->SubItems->Add(str_zona);
    item->SubItems->Add(ComplexToStr("%3.2f", sa));
    item->SubItems->Add(ComplexToStr("%3.2f", sb));
    item->SubItems->Add(ComplexToStr("%3.2f", sc));
    item->SubItems->Add(DoubleToStr("%3.2f", deseq*100));
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::PreencheGraficoPotAparente(VTRede *rede)
{
    // var locais
    AnsiString txt;
    double s[4], ymax = 0.;
    TCarregamento carreg(apl);
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*) apl->GetObject(__classid(VTPatamares));

    // limpa series
    SeriesAa->Clear();
    SeriesBa->Clear();
    SeriesCa->Clear();
    SeriesABCa->Clear();

    carreg.AnalisaRede(rede);
    for (int np = 0; np < patamares->NumPatamar(); np++)
    {
        patamar = patamares->Patamar[np];
        // determina as potencias por fase e trifasica
        carreg.GetCarregamentoABC_kva(&sa, &sb, &sc, np);
        carreg.GetCarregamento3F_kva(&s3, np);
        // por fase
        s[0] = Abs(sa);
        s[1] = Abs(sb);
        s[2] = Abs(sc);
        s[3] = Abs(s3);
        // define texto da legenda
        (patamar->MinutoIni == 0) ? txt = IntToStr(patamar->HoraIni) : txt = "";
        SeriesAa->AddXY(np, s[0], txt);
        if (s[0] > ymax)
            ymax = s[0];
        SeriesBa->AddXY(np, s[1], txt);
        if (s[1] > ymax)
            ymax = s[1];
        SeriesCa->AddXY(np, s[2], txt);
        if (s[2] > ymax)
            ymax = s[2];
        SeriesABCa->AddXY(np, s[3], txt);
        if (s[3] > ymax)
            ymax = s[3];
        // redefine texto da legenda
        (patamar->MinutoFim == 0) ? txt = IntToStr(patamar->HoraFim) : txt = "";
        SeriesAa->AddXY(np + 1, s[0], txt);
        SeriesBa->AddXY(np + 1, s[1], txt);
        SeriesCa->AddXY(np + 1, s[2], txt);
        SeriesABCa->AddXY(np + 1, s[3], txt);
    }
    // ajusta valores máximos do gráfico
    chartS3Fantes->LeftAxis->Minimum = 0.;
    chartS3Fantes->LeftAxis->Maximum = 1.1 * ymax;
    if (IsDoubleZero(ymax))
        chartS3Fantes->LeftAxis->Maximum = 1.;
}

// ---------------------------------------------------------------------------
void __fastcall TFormBalanceamento::TViewRedesInicia(void)
{
    // variáveis locais
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    // reinicia TView
    TViewRedes->Items->Clear();
    // inicia Arvore e o TreeView
    arvore->Inicia(redes);
    arvore->TreeViewInicia(TViewRedes);
    // expande o TView
    // TViewRedes->FullExpand();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18129916
void __fastcall TFormBalanceamento::TViewRedesClick(TObject *Sender)
{
    // variáveis locais
    VTRede *rede;

    // verifica se existe uma Rede associada ao TreeNode
    if ((rede = TViewRedeSelecionada()) == NULL)
    {
        Aviso("Selecione uma rede");
        return;
    }
    //verifica se a rede está carregada
    if(!rede->Carregada)
    {
        Aviso("Selecione uma rede que esteja carregada");
        return;
    }
    // atualiza dados
    AtualizaDados(rede);
}

/*
 //---------------------------------------------------------------------------
 VTRede* __fastcall TFormBalanceamento::TViewRedeSelecionada(void)
 {
 //variáveis locais
 AnsiString 	class_name;
 VTNode      *node;
 VTRede 		*rede;
 TList       *lisREDE;

 //verifica se há um TreeNode selecionado
 if ((TViewRedes->Selected == NULL)||(TViewRedes->Selected->Data == NULL)) return(NULL);
 //verifica se objeto associado ao TreeNode é um VTNode
 class_name = ((TObject*)(TViewRedes->Selected->Data))->ClassName();
 if (class_name.AnsiCompareIC("TNode") == 0)
 {//determina Rede associada ao TreeNode
 node = (VTNode*)TViewRedes->Selected->Data;
 lisREDE = node->LisObjectNode();
 if (lisREDE->Count == 1)
 {
 return((VTRede*)lisREDE->First());
 }
 }
 return(NULL);
 }
 */ // DVK 2015.09.03 reescrito
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18129916
VTRede* __fastcall TFormBalanceamento::TViewRedeSelecionada(void)
{
    // variáveis locais
    AnsiString class_name;
    VTNode *node;
    VTRede *rede;
    TList *lisREDE;

    // verifica se há um TreeNode selecionado
    if ((TViewRedes->Selected == NULL) || (TViewRedes->Selected->Data == NULL))
        return (NULL);
    // verifica se objeto associado ao TreeNode é um VTNode
    class_name = ((TObject*)(TViewRedes->Selected->Data))->ClassName();
    if (class_name.AnsiCompareIC("TNode") == 0)
    { // determina Rede associada ao TreeNode
        node = (VTNode*)TViewRedes->Selected->Data;
        lisREDE = node->LisObjectNode();
        if (lisREDE->Count == 1)
        {
            return ((VTRede*)lisREDE->First());
        }
        if (lisREDE->Count > 1)
        { // localiza a rede selecionada na lista de redes do node
            rede = ExisteRede(TViewRedes->Selected->Text, lisREDE);
            return (rede);
        }
    }
    else if (class_name.AnsiCompareIC("TRede") == 0)
    { // determina Rede associada ao TreeNode
        return ((VTRede*)TViewRedes->Selected->Data);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormBalanceamento::VerificaPlanejamento(void)
{
    // variáveis locais
    VTPlanejamento *planejamento = (VTPlanejamento*) apl->GetObject(__classid(VTPlanejamento));
    VTEstudo *estudo;

    // proteção: rede original
    estudo = planejamento->ExisteEstudo();
    if ((estudo->Tipo > 0) && (estudo->AlternativaAtiva == estudo->AlternativaPadrao))
    {
        Aviso("Não é possível balancear cargas na rede original");
        return (false);
    }
    else if ((estudo->Tipo > 0) && (estudo->AlternativaAtiva->Obras->LisObra()->Count == 0))
    {
        Aviso("Crie primeiro uma obra para esta alternativa");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// eof
