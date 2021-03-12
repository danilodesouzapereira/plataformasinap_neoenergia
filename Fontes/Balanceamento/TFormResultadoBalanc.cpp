// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFormResultadoBalanc.h"
#include "TFormRelatorio.h"
#include "VTBalanceamento.h"
#include "TCarregamento.h"
// ---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTRede.h"
#include "..\Zona\VTZona.h"
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Zona.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormResultadoBalanc::TFormResultadoBalanc(TComponent* Owner, VTApl* apl_owner,
    VTRede* rede) : TForm(Owner)
{
    // salva ponteiros
    this->apl = apl_owner;
    this->rede = rede;
    balanceamento = (VTBalanceamento*)apl->GetObject(__classid(VTBalanceamento));
    // verifica planejamento
    // if(! VerificaPlanejamento())       return;
    // salva Obra originalmente associado a Edita
    edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));
    obra_original = edita->Obra;
    // cria objeto Obra e associa com objeto Edita
    edita->Obra = obra = DLL_NewObjObra(apl);
    // inicia variavel //DVK 2015.08.05
    sucesso = false;
    // Inicia //DVK 2015.08.05
    sucesso = Inicia();
    // posiciona o Form
    FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormResultadoBalanc::~TFormResultadoBalanc(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    // salva parâmetros do Form
    if (geral)
        geral->Infoset->SaveToFile(this);
    // cancela Obra associada ao objeto Edita
    edita->Obra = obra_original; // NULL;
    // destroi objetos
    if (obra)
    {
        delete obra;
        obra = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::ActionCancelarExecute(TObject *Sender)
{
    // cancela o Balanceamento e restaura fases das Cargas alteradas
    CancelaBalanceamento();
    // fecha
    ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::ActionConfirmarExecute(TObject *Sender)
{
    // atualiza a Rede coma as Cargas alteradas
    ConfirmaBalanceamento();
    // desabilita evento CloseQuery
    OnCloseQuery = NULL;
    // fecha
    ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::ActionRelCargasExecute(TObject *Sender)
{
    // variáveis locais
    TFormRelatorio *formRel;

    // esconde este Form
    Visible = false;
    // relatório de cargas da rede balanceada
    formRel = new TFormRelatorio(NULL, apl);
    formRel->ShowModal();
    delete formRel;
    // exibe este Form
    Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::AnalisaRede(void)
{
    // variaveis locais
    TCarregamento carreg(apl);

    // limpa variaveis
    sa = CZero();
    sb = CZero();
    sc = CZero();
    s3 = CZero();
    // analisa carregamento máximo na rede
    carreg.AnalisaRede(rede);
    carreg.GetCarregamentoABC_kva(&sa, &sb, &sc);
    carreg.GetCarregamento3F_kva(&s3);
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::CancelaBalanceamento(void)
{
    // variáveis locais
    VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

    // restaura Rede original
    RestauraRedeOriginal();
    // restaura Obra original do objeto Edita
    edita->Obra = obra_original;
    // atualiza grafico
    if (graf)
        graf->AreaRedeAltera();
    // reinicia Balanceamento para que a lista de Cargas alteradas fique vazia
    // balanceamento->Reinicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::CBoxPatamarInicia(void)
{
    // variáveis locais
    int ind_pat;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
    TList *lisPAT = patamares->LisPatamar();

    // reinicia CBoxPatamar c/ Patamares
    CBoxPatamar->Clear();
    for (int np = 0; np < lisPAT->Count; np++)
    {
        patamar = (VTPatamar*)lisPAT->Items[np];
        if (patamar->Enabled)
        {
            CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
        }
    }
    // proteção
    if (CBoxPatamar->Items->Count == 0)
        return;
    // pré-seleciona patamar de pico
    ind_pat = balanceamento->IndicePatamarPico();
    if (CBoxPatamar->Items->Count > ind_pat)
        CBoxPatamar->ItemIndex = ind_pat;
    else
        CBoxPatamar->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::ConfirmaBalanceamento(void)
{
    // variáveis locais
    VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

    // restaura Rede original
    RestauraRedeOriginal();
    // restaura Obra original do objeto Edita
    edita->Obra = obra_original;
    // refaz a alteração das Cargas alteradas, agora usando a Obra original de Edita
    balanceamento->AlteraRede(balanceamento->LisCloneCargaAlterada());
    // atualiza grafico
    if (graf)
        graf->AreaRedeAltera();
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    // confirmar e sair
    switch (Confirma("Deseja efetivar o balanceamento na rede ?", AnsiQuotedStr(rede->Codigo, '\''),
        MB_YESNOCANCEL))
    {
    case ID_YES:
        ConfirmaBalanceamento();
        CanClose = true;
        ModalResult = mrOk;
        break;
    case ID_NO: // restaura Cargas com suas fases originais
        CancelaBalanceamento();
        CanClose = true;
        ModalResult = mrCancel;
        break;
    case ID_CANCEL:
    default:
        CanClose = false;
        break;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::FormIniciaPosicao(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    if (geral)
    { // lê parâmetros do Form
        if (geral->Infoset->LoadFromFile(this))
            return;
    }
    // posiciona o Form sem alinhamento
    // ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::FormResize(TObject *Sender)
{
    // redefine largura dos Panels
    PanelAntes->Width = this->Width / 2;
    PanelDepois->Width = this->Width / 2;
    // redefine largura dos charts
    chartPantes->Width = PanelPQantes->Width / 2;
    chartQantes->Width = PanelPQantes->Width / 2;
    chartPdepois->Width = PanelPQdepois->Width / 2;
    chartQdepois->Width = PanelPQdepois->Width / 2;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::FormShow(TObject *Sender)
{
    // nada
}

// ---------------------------------------------------------------------------
bool __fastcall TFormResultadoBalanc::Inicia(void)
{ // DVK 2015.08.05 alterado para retornar bool
    // proteção
    if (rede == NULL)
        return (false);
    if (balanceamento == NULL)
        return (false);
    // analisa rede antes de balancear
    AnalisaRede();
    // preenche charts antes
    PreencheChartsOriginal();
    PreencheLViewOriginal();
    PreencheGraficoPotAparenteOrig();
    // executa balanceamento
    if (balanceamento->Executa(rede))
    { // analisa rede depois de balancear
        AnalisaRede();
        // preenche charts depois
        PreencheChartsBalanceado();
        PreencheLViewBalanceado();
        PreencheGraficoPotAparenteBalanc();
        CBoxPatamarInicia(); // DVK 2015.08.05
        return (true);
    }
    return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::PreencheChartsBalanceado(void)
{
    // proteção
    if (IsComplexZero(sa.real(), sa.imag()))
        sa = CZero();
    if (IsComplexZero(sb.real(), sb.imag()))
        sb = CZero();
    if (IsComplexZero(sc.real(), sc.imag()))
        sc = CZero();

    // potência ativa
    SeriesCarregPdepois->Clear();
    SeriesCarregPdepois->Add(sa.real(), "Fase A");
    SeriesCarregPdepois->Add(sb.real(), "Fase B");
    SeriesCarregPdepois->Add(sc.real(), "Fase C");

    // potência reativa
    SeriesCarregQdepois->Clear();
    SeriesCarregQdepois->Add(sa.imag(), "Fase A");
    SeriesCarregQdepois->Add(sb.imag(), "Fase B");
    SeriesCarregQdepois->Add(sc.imag(), "Fase C");
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::PreencheChartsOriginal(void)
{
    // proteção
    if (IsComplexZero(sa.real(), sa.imag()))
        sa = CZero();
    if (IsComplexZero(sb.real(), sb.imag()))
        sb = CZero();
    if (IsComplexZero(sc.real(), sc.imag()))
        sc = CZero();

    // potência ativa
    SeriesCarregPantes->Clear();
    SeriesCarregPantes->Add(sa.real(), "Fase A");
    SeriesCarregPantes->Add(sb.real(), "Fase B");
    SeriesCarregPantes->Add(sc.real(), "Fase C");

    // potência reativa
    SeriesCarregQantes->Clear();
    SeriesCarregQantes->Add(sa.imag(), "Fase A");
    SeriesCarregQantes->Add(sb.imag(), "Fase B");
    SeriesCarregQantes->Add(sc.imag(), "Fase C");
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::PreencheLViewBalanceado(void)
{
    // var local
    AnsiString str_zona;
    TListItem *item;
    double media, deseq;
    double carr_sa, carr_sb, carr_sc;
    double delta_sa, delta_sb, delta_sc;

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
    LViewPQdepois->Items->Clear();
    // insere dados
    item = LViewPQdepois->Items->Add();
    item->Caption = rede->Codigo;
    item->SubItems->Add(str_zona);
    item->SubItems->Add(ComplexToStr("%3.2f", sa));
    item->SubItems->Add(ComplexToStr("%3.2f", sb));
    item->SubItems->Add(ComplexToStr("%3.2f", sc));
    item->SubItems->Add(DoubleToStr("%3.2f", deseq*100));
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::PreencheLViewOriginal(void)
{
    // var local
    AnsiString str_zona;
    TListItem *item;
    double media, deseq;
    double carr_sa, carr_sb, carr_sc;
    double delta_sa, delta_sb, delta_sc;

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
    LViewPQantes->Items->Clear();
    // insere dados
    item = LViewPQantes->Items->Add();
    item->Caption = rede->Codigo;
    item->SubItems->Add(str_zona);
    item->SubItems->Add(ComplexToStr("%3.2f", sa));
    item->SubItems->Add(ComplexToStr("%3.2f", sb));
    item->SubItems->Add(ComplexToStr("%3.2f", sc));
    item->SubItems->Add(DoubleToStr("%3.2f", deseq*100));
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::PreencheGraficoPotAparenteBalanc(void)
{
    // var locais
    AnsiString txt;
    double s[4], ymax = 0.;
    TCarregamento carreg(apl);
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*) apl->GetObject(__classid(VTPatamares));

    // limpa series
    SeriesAd->Clear();
    SeriesBd->Clear();
    SeriesCd->Clear();
    SeriesABCd->Clear();
    // obtém carregamento
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
        SeriesAd->AddXY(np, s[0], txt);
        if (s[0] > ymax)
            ymax = s[0];
        SeriesBd->AddXY(np, s[1], txt);
        if (s[1] > ymax)
            ymax = s[1];
        SeriesCd->AddXY(np, s[2], txt);
        if (s[2] > ymax)
            ymax = s[2];
        SeriesABCd->AddXY(np, s[3], txt);
        if (s[3] > ymax)
            ymax = s[3];
        // redefine texto da legenda
        (patamar->MinutoFim == 0) ? txt = IntToStr(patamar->HoraFim) : txt = "";
        SeriesAd->AddXY(np + 1, s[0], txt);
        SeriesBd->AddXY(np + 1, s[1], txt);
        SeriesCd->AddXY(np + 1, s[2], txt);
        SeriesABCd->AddXY(np + 1, s[3], txt);
    }
    // ajusta valores máximos do gráfico
    chartS3Fdepois->LeftAxis->Minimum = 0.;
    chartS3Fdepois->LeftAxis->Maximum = 1.1 * ymax;
    if (IsDoubleZero(ymax))
        chartS3Fdepois->LeftAxis->Maximum = 1.;
}

// ---------------------------------------------------------------------------
void __fastcall TFormResultadoBalanc::PreencheGraficoPotAparenteOrig(void)
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
void __fastcall TFormResultadoBalanc::RestauraRedeOriginal(void)
{
    // restaura Obra original
    edita->UndoCompleto();
    edita->Clear();
}

// ---------------------------------------------------------------------------
// eof
