// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRelatorio.h"
#include "VTBalanceamento.h"
#include "..\Apl\VTApl.h"
// #include "..\Complexo\Complexo.h.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRelatorio::TFormRelatorio(TComponent* Owner, VTApl *apl) : TForm(Owner)
{
    // salva objetos
    this->apl = apl;
    // cria objetos
    lisEQP = new TList();
    // inicia CBoxPatamar
    CBoxPatamarInicia();
    // monta o relatório
    LViewInicia();
    // define ordenação do ListView pelo código da Carga
    ListViewSort.ColumnIndex = 0;
    ListViewSort.OrdemCrescente = true;
}

// ---------------------------------------------------------------------------
__fastcall TFormRelatorio::~TFormRelatorio(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    // salva parâmetros do Form
    if (geral)
        geral->Infoset->SaveToFile(this);
    // apaga eventual moldura do gráfico
    Moldura();
    // destrói lista sem destruir seus objetos
    if (lisEQP)
    {
        delete lisEQP;
        lisEQP = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::ActionSairExecute(TObject *Sender)
{
    ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::ActionSalvarExecute(TObject *Sender)
{
    // variáveis locais
    AnsiString arq_ext;
    VTReport *report;
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

    // esconde este Form
    Visible = false;
    try
    { // abre SaveDialog p/ usuário selecionar arquivo
        if (SaveDialog->InitialDir.IsEmpty())
            SaveDialog->InitialDir = path->DirExporta();
        if (SaveDialog->Execute())
        { // cria objeto VTReport
            report = DLL_NewObjReport(apl);
            // verifica extensão do arquivo destino
            arq_ext = ExtractFileExt(SaveDialog->FileName);
            if (arq_ext.AnsiCompareIC(".xls") == 0)
            { // exporta planilha Excel
                report->PlanilhaExcel(SaveDialog->FileName, "Balanceamento", LView);
            }
            else
            { // exporta arquivo texto
                report->ArquivoTexto(SaveDialog->FileName, LView, "Balanceamento");
            }
            // destrói VTReport
            DLL_DeleteObjReport(report);
        }
    }
    catch (Exception &e)
    { // nada a fazer
    }
    // exibe este Form
    Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::CBoxPatamarChange(TObject *Sender)
{
    // reinicia LView
    LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::CBoxPatamarInicia(void)
{
    // variáveis locais
    int ind_pat;
    VTPatamar *patamar;
    VTBalanceamento *balanceamento = (VTBalanceamento*)apl->GetObject(__classid(VTBalanceamento));
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
void __fastcall TFormRelatorio::CheckBoxCargaAlteradaClick(TObject *Sender)
{
    // reinicia ListView
    LViewInicia();
}

// ---------------------------------------------------------------------------
VTCarga* __fastcall TFormRelatorio::ExisteCarga(TList *lisCAR, int carga_id)
{
    // variáveis locais
    VTCarga *carga;

    for (int n = 0; n < lisCAR->Count; n++)
    {
        carga = (VTCarga*)lisCAR->Items[n];
        if (carga->Id == carga_id)
            return (carga);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::FormClose(TObject *Sender, TCloseAction &Action)
{
    // esconde este form
    Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::FormIniciaPosicao(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    if (geral)
    { // lê parâmetros do Form
        if (geral->Infoset->LoadFromFile(this))
            return;
    }
    // posiciona o Form no canto superior esquerdo
    Left = 0;
    Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::FormShow(TObject *Sender)
{
    // posiciona o Form
    FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::LViewClick(TObject *Sender)
{
    // monta lista com Cargas selecionadas
    lisEQP->Clear();
    for (int n = 0; n < LView->Items->Count; n++)
    {
        if (LView->Items->Item[n]->Selected)
        {
            lisEQP->Add(LView->Items->Item[n]->Data);
        }
    }
    // exibe moldura nas Cargas selecionadas
    Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::LViewColumnClick(TObject *Sender, TListColumn *Column)
{
    // verifica se selecionou a mesma coluna
    if (ListViewSort.ColumnIndex == Column->Index)
    { // inverte ordem
        ListViewSort.OrdemCrescente = !ListViewSort.OrdemCrescente;
    }
    else
    { // redefine índice da coluna usada p/ ordenação
        ListViewSort.ColumnIndex = Column->Index;
    }
    // força reordenação
    LView->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
    int Data, int &Compare)
{
    // variáveis locais
    int index = ListViewSort.ColumnIndex - 1;
    double valor1, valor2;

    switch (ListViewSort.ColumnIndex)
    {
    case 0: // código do Carga
        Compare = CompareText(Item1->Caption, Item2->Caption);
        break;
    case 1: // fase original
        Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
        break;
    case 2: // fase atual
        Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
        break;
    case 3: // demanda
        StrToAbsComplex(Item1->SubItems->Strings[index], valor1);
        StrToAbsComplex(Item2->SubItems->Strings[index], valor2);
        if (valor1 < valor2)
            Compare = -1;
        else if (valor1 > valor2)
            Compare = 1;
        else
            Compare = 0;
        break;
    default: // compara Caption
        Compare = CompareText(Item1->Caption, Item2->Caption);
    }
    // verifica se ordem crescente/decrescente
    if (!ListViewSort.OrdemCrescente)
        Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::LViewInicia(void)
{
    // variáveis locais
    int ind_pat;
    complex<double>s_kva;
    TListItem *item;
    VTCarga *carga, *carga_ori, *carga_alt;
    VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
    VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
    VTBalanceamento *balanceamento = (VTBalanceamento*)apl->GetObject(__classid(VTBalanceamento));
    TList *lisCAR = balanceamento->LisCarga();
    TList *lisCAR_ALT = balanceamento->LisCloneCargaAlterada();
    TList *lisCAR_ORI = balanceamento->LisCloneCargaOriginal();

    // reinicia LView
    LView->Clear();
    // determina patamar selecionado
    if ((ind_pat = CBoxPatamar->ItemIndex) < 0)
        return;
    // verifica o tipo de relatório
    if (CheckBoxCargaAlterada->Checked)
    { // relatório com apenas as Cargas alteradas
        for (int n = 0; n < lisCAR_ORI->Count; n++)
        {
            carga_ori = (VTCarga*)lisCAR_ORI->Items[n];
            // determina cópia da Carga alterada
            if ((carga_alt = ExisteCarga(lisCAR_ALT, carga_ori->Id)) == NULL)
                continue;
            // insere um novo ListItem, associando com a Carga da Rede
            item = LView->Items->Add();
            // item->Data = carga_ori->Obj;
            item->Data = carga_ori;
            // código
            item->Caption = carga_ori->Codigo;
            // fase original
            item->SubItems->Add(fases->AsStrUpper[carga_ori->Fases]);
            // fase atual
            item->SubItems->Add(fases->AsStrUpper[carga_alt->Fases]);
            // demanda total no patamar selecionado
            s_kva = 1000. * demanda->S_mva[carga_ori][ind_pat];
            item->SubItems->Add(ComplexToStr("%3.2f", s_kva.real(), s_kva.imag()));
        }
    }
    else
    { // relatório com todas as Cargas
        for (int n = 0; n < lisCAR->Count; n++)
        {
            carga = (VTCarga*)lisCAR->Items[n];
            // determina cópia da Carga original
            if ((carga_ori = ExisteCarga(lisCAR_ORI, carga->Id)) != NULL)
            { // determina cópia da Carga alterada
                carga_alt = ExisteCarga(lisCAR_ALT, carga->Id);
            }
            // proteção
            if (carga_ori == NULL)
                carga_ori = carga;
            if (carga_alt == NULL)
                carga_alt = carga;
            // insere um novo ListItem
            item = LView->Items->Add();
            // item->Data = carga;
            item->Data = carga_ori;
            // código
            // item->Caption = carga->Codigo;
            item->Caption = carga_ori->Codigo;
            // fase original
            item->SubItems->Add(fases->AsStrUpper[carga_ori->Fases]);
            // fase atual
            item->SubItems->Add(fases->AsStrUpper[carga_alt->Fases]);
            // demanda total no patamar selecionado
            // s_kva = 1000. * demanda->S_mva[carga][ind_pat];
            s_kva = 1000. * demanda->S_mva[carga_ori][ind_pat];
            item->SubItems->Add(ComplexToStr("%3.2f", s_kva.real(), s_kva.imag()));
        }
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorio::Moldura(TList *lisEQP)
{
    // variáveis locais
    VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

    grafico->Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRelatorio::StrToAbsComplex(AnsiString text, double &valor)
{
    // variáveis locais
    int index, len;
    double real, imag;

    // verifica se existe o caractere j
    if ((index = text.AnsiPos("j")) < 0)
        return (false);
    // determina parte real e imaginária
    len = text.Length();
    real = StrToDouble(text.SubString(1, index - 2));
    imag = StrToDouble(text.SubString(index + 1, len - index));
    valor = Sqrt((real * real) + (imag * imag));
    return (true);
}

// ---------------------------------------------------------------------------
// eof
