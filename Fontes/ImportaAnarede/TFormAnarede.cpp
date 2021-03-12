// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAnarede.h"
#include "TAplAnarede.h"
#include "VTAnarede.h"
#include "VTAnaxy.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Log\VTLog.h"
#include "..\Diretorio\VTPath.h"
#include "..\Progresso\VTProgresso.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormAnarede::TFormAnarede(TComponent *Owner, VTApl *apl_owner) : TForm(Owner)
{
    // cria objeto AplAnarede
    apl = new TAplAnarede(this, apl_owner);
}

// ---------------------------------------------------------------------------
__fastcall TFormAnarede::~TFormAnarede(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    // salva parâmetros do Form
    if (geral)
        geral->Infoset->SaveToFile(this);
    // destrói objetos
    if (apl)
    {
        delete apl;
        apl = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::ActionCancelaExecute(TObject *Sender)
{
    // cancela importação
    ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::ActionConfirmaExecute(TObject *Sender)
{
    // variáveis locais
    bool sucesso;
    int num_area = -1;
    VTAnarede *anarede = (VTAnarede*)apl->GetObject(__classid(VTAnarede));
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
    VTLog *plog = (VTLog*)apl->GetObject(__classid(VTLog));

    // verifica se existe progresso
    if (prog == NULL)
    {
        apl->Add(prog = DLL_NewObjProgresso(NULL, this));
    }
    // verifica se o RichEdit está vazio
    if (redArq->Lines->Count == 0)
    {
        Aviso("Selecione arquivo c/ dados da rede");
        return;
    }
    try
    { // verifica se opção de área está selecionada
        if (CheckBoxArea->Checked)
        { // verifica se foi definida a área
            if (EditArea->Text.IsEmpty())
            {
                Aviso("Defina número da área da rede");
                return;
            }
            // determina número da área selecionada
            num_area = StrToInt(EditArea->Text);
        }
    }
    catch (Exception &e)
    {
        Erro("Número da área da rede inválido");
        return;
    }
    // executa importação dos dados
    plog->Open(path->DirTmp() + "\\Anarede.log");
    prog->Start(progDEFAULT);
    sucesso = anarede->Executa(redArq->Lines, num_area);
    plog->Close();
    if (!sucesso)
    { // erro: mostra arquivo de Log
        redArq->Lines->Clear();
        redArq->Lines->LoadFromFile(plog->ArqLog());
        Aviso("Existência de erros na importação da rede.\n"
            "Verifique os problemas no relatório.");
        prog->Stop();
        return;
    }
    // permite importação de arquivo com coordenadas das barras
    prog->Stop();
    ImportaCoordenadasEsquematico();
    // importação feita c/ sucesso
    ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::ActionSelArqExecute(TObject *Sender)
{
    // variáveis locais
    VTPath *path = (VTPath*)apl->GetObject("TPath");

    // seleciona diretório inicial
    if (OpenDialog->InitialDir.IsEmpty())
        OpenDialog->InitialDir = path->DirImporta();
    if (OpenDialog->InitialDir.IsEmpty())
        OpenDialog->InitialDir = path->DirBase;
    OpenDialog->Filter =
        "Arquivo anarede (*.pwf)|*.pwf|Arquivos texto (*.dat)|*.dat|Arquivos texto (*.txt)|*.txt)|todos (*.*)|*.*";
    // seleciona arquivo TXT
    if (!OpenDialog->Execute())
        return;
    // usuário selecionou um arquivo: exibe nome do arquivo no Status bar
    StatusBar->SimpleText = OpenDialog->FileName;
    // exibe conteúdo do arquivo no RichEdit
    redArq->Lines->LoadFromFile(OpenDialog->FileName);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::CheckBoxAreaClick(TObject *Sender)
{
    // exibe esconde EditArea
    EditArea->Visible = CheckBoxArea->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::ImportaCoordenadasEsquematico(void)
{
    // variáveis locais
    VTAnaxy *anaxy = NULL;
    VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

    try
    {
        OpenDialog->Title = "Importar coordenadas das barras";
        OpenDialog->Filter = "Arquivo LST (*.lst)|*.lst|Todos arquivos(*.*)|*.*";
        if (!OpenDialog->Execute())
            return;
        // inicia progresso
        prog->Start(progDEFAULT);
        // le arquivo no RichEdit
        redArq->Lines->LoadFromFile(OpenDialog->FileName);
        // cria objeto p/ importar coordenadas
        anaxy = NewObjAnaxy(apl);
        if (!anaxy->Executa(redArq->Lines))
        {
            Erro("Erro ao importar arquivo LST.");
        }
        delete anaxy;
        prog->Stop();
    }
    catch (Exception &e)
    { // nada a fazer
        prog->Stop();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::FormClose(TObject *Sender, TCloseAction &Action)
{
    // esconde o Form
    Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::FormIniciaPosicao(void)
{
    // variáveis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    if (geral)
    { // lê parâmetros do Form
        if (geral->Infoset->LoadFromFile(this))
            return;
    }
    // posiciona o Form sem alinhamento
    Left = 0;
    Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnarede::FormShow(TObject *Sender)
{
    // posiciona o Form
    FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
// eof
