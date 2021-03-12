// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormConfig.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormConfig::TFormConfig(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
    // salva objetos
    apl = apl_owner;
    geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
    planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
    estudo = planejamento->ExisteEstudo();
    // inicia datas do per�odo de an�lise
    ExibeDataAnalise();
}

// ---------------------------------------------------------------------------
__fastcall TFormConfig::~TFormConfig(void)
{
    // vari�veis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    // salva par�metros do Form
    if (geral)
        geral->Infoset->SaveToFile(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ActionCancelaExecute(TObject *Sender)
{
    // fecha o Form
    ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ActionConfirmaExecute(TObject *Sender)
{
    // l� e valida per�odo de an�lise
    if (!LeDataAnalise())
        return;
    // fecha o Form
    ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ExibeDataAnalise(void)
{
    EditAnoIni->Text = IntToStr(estudo->AnoInicial);
    EditAnoFim->Text = IntToStr(estudo->AnoFinal);
    // atualliza Label c/ data da �ltima obra
    LabelDataUltObra->Caption = "Ano da �ltima obra: " + IntToStr(estudo->AnoObraFinal);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17577418
void __fastcall TFormConfig::FormIniciaPosicao(void)
{
    /*
     //vari�veis locais
     VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

     if (geral)
     {//l� par�metros do Form
     geral->Infoset->LoadFromFile(this);
     }
     */
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::FormShow(TObject *Sender)
{
    // inicia par�metros do Form
    FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormConfig::LeDataAnalise(void)
{
    // vari�veis locais
    int ano_ini, ano_fim;

    try
    { // l� ano inicial
        ano_ini = StrToInteger(EditAnoIni->Text);
    }
    catch (Exception &e)
    {
        Aviso("Ano inicial com valor inv�lido");
        return (false);
    }
    try
    { // l� ano final
        ano_fim = StrToInteger(EditAnoFim->Text);
    }
    catch (Exception &e)
    {
        Aviso("Ano final com valor inv�lido");
        return (false);
    }
    // verifica se ano inicial � maior que ano final
    if (ano_fim < ano_ini)
    {
        Aviso("Ano final menor que ano inicial");
        return (false);
    }
    // anos v�lidos
    estudo->AnoInicial = ano_ini;
    estudo->AnoFinal = ano_fim;
    return (true);
}

// ---------------------------------------------------------------------------
// eof
