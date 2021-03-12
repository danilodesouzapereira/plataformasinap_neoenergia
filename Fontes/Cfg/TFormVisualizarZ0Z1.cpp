// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormVisualizarZ0Z1.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Diretorio\VTPath.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormVisualizarZ0Z1::TFormVisualizarZ0Z1(TComponent* Owner, VTArranjos *arranjos,
    VTApl *apl) : TForm(Owner)
{
    // salva ponteiro c/ arranjos e apl
    this->arranjos = arranjos;
    this->apl = apl;
}

// ---------------------------------------------------------------------------
__fastcall TFormVisualizarZ0Z1::~TFormVisualizarZ0Z1(void)
{
    // nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormVisualizarZ0Z1::ActionExportaExecute(TObject *Sender)
{
    // variaveis locais
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    VTReport *report;

    // configura SaveDialog
    SaveDialog->InitialDir = path->DirExporta();
    if (SaveDialog->Execute())
    { // cria obj report
        report = DLL_NewObjReport(apl);
        // exporta conteúdo do LView para o arquivo
        if (report->ArquivoTexto(SaveDialog->FileName, LViewZ0Z1))
        {
            Aviso("A tabela foi exportada corretamente");
        }
        else
            Aviso("Erro ao exportar tabela");
        // destroi report
        delete report;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormVisualizarZ0Z1::ActionFecharExecute(TObject *Sender)
{
    // fecha o form
    ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormVisualizarZ0Z1::FormClose(TObject *Sender, TCloseAction &Action)
{
    // esconde o form
    Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormVisualizarZ0Z1::FormShow(TObject *Sender)
{
    // preenche LViewZ0Z1 com as impedâncias de cada arranjo
    LViewZ0Z1Inicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormVisualizarZ0Z1::LViewZ0Z1Inicia(void)
{
    // variaveis locais
    TListItem *item;
    VTArranjo *arranjo;
    strIMP z0, z1;
    double yc0, yc1, c0, c1;
    double compr_km = 1.;

    // proteção
    if (!arranjos)
        return;
    // limpa LView
    LViewZ0Z1->Clear();
    for (int na = 0; na < arranjos->LisArranjo()->Count; na++)
    { // insere as impedâncias de sequência de cada arranjo
        arranjo = (VTArranjo*)arranjos->LisArranjo()->Items[na];
        item = LViewZ0Z1->Items->Add();
        item->Caption = arranjo->Codigo;
        if (!arranjo->Z0_ohm(compr_km, z0, yc0))
            continue;
        if (!arranjo->Z1_ohm(compr_km, z1, yc1))
            continue;
        c0 = arranjo->MaiorCapacitancia(0);
        c1 = arranjo->MaiorCapacitancia(1);
        item->SubItems->Add(DoubleToStr("%4.3f", z0.r, ','));
        item->SubItems->Add(DoubleToStr("%4.3f", z0.x, ','));
        item->SubItems->Add(DoubleToStr("%4.3f", c0, ','));
        item->SubItems->Add(DoubleToStr("%4.3f", z1.r, ','));
        item->SubItems->Add(DoubleToStr("%4.3f", z1.x, ','));
        item->SubItems->Add(DoubleToStr("%4.3f", c1, ','));
    }
}

// ---------------------------------------------------------------------------
// eof
