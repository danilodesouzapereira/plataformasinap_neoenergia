//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagReg.h"
//#include "TFormDiagEqptoReg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//define do nome do arquivo
//#define FILE_CFG_REG "CfgDiagEqptoREG"

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
__fastcall TDiagReg::TDiagReg(VTApl *apl)
{
    this->apl = apl;

    //Mostra que ainda não foi executado
    executado = false;
    //define se tem informações adicionais
    infoExtra = false;
    //cria a lista de configuracoes
    listCfg = new TList;
    //inicializa as opcoes do diagnostico
    dgReg.barref = true;
    //cria o log_reg
    log_reg = new TStringList;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
__fastcall  TDiagReg::~TDiagReg(void)
{
    //destroi lista e conteudo
    if(listCfg)
    {
        LimpaTList(listCfg);
        delete listCfg;
        listCfg = NULL;
    }
    //destroi log
    if(log_reg)
    {
        delete log_reg;
        log_reg = NULL;
    }
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TDiagReg::AddLinhaLog(VTRegulador *reg, int opDIAG_REG)
{
    //incrementa o contador de erros
    //infoReg.n_ocorrencias++;
    temErro = true;
    //vetor de strings seguindo o enum col_LOG_REG
    AnsiString  linhaPreenchida = "";
    AnsiString  linha[colReg_COUNT];
    AnsiString  Separador  = ";";

    VTBarra    *barra1,         *barra2,          *barra3,
               *sentido,        *barraref,        *barrarefinv;
    AnsiString barraCod1  = "", barraCod2   = "", barraCod3      = "",
               sentidoCod = "", barrarefCod = "", barrarefinvCod = "";

    if (reg)
    {
        //verifica se as barras de referencia do regulador são as mesmas da sua ligação
        barra1        = reg->pbarra1;
        barra2        = reg->pbarra2;
//        barra3        = reg->pbarra3;
//        sentido       = reg->sentido.bar_ref;
        barraref      = reg->cfg_auto.bar_ref;
        barrarefinv   = reg->cfg_auto.bar_ref_inv;

        if(barra1)
            barraCod1      = reg->pbarra1->Codigo;
        if(barra2)
            barraCod2      = reg->pbarra2->Codigo;
//        if(barra3)
//            barraCod3      = reg->pbarra3->Codigo;
//        if(sentido)
//            sentidoCod     = reg->sentido.bar_ref->Codigo;
        if(barraref)
            barrarefCod    = reg->cfg_auto.bar_ref->Codigo;
        if(barrarefinv)
            barrarefinvCod = reg->cfg_auto.bar_ref_inv->Codigo;

        //inicializa com os valores vindos do regulador
        linha[colReg_SEGMENTO]         = reg->rede->TipoRede->Codigo + Separador;
        linha[colReg_REDE]             = reg->rede->Codigo           + Separador;
        linha[colReg_EQPTO]            = "Regulador"                 + Separador;
        linha[colReg_CODIGO]           = reg->Codigo                 + Separador;
        switch (opDIAG_REG)
        {
        case opRegBARREF:
            linha[colReg_TIPOERRO]     = str_TIPOERRO_AVISO          + Separador;

            linha[colReg_CODBARRA1]    = barraCod1                   + Separador;
            linha[colReg_CODBARRA2]    = barraCod2                   + Separador;
//            linha[colReg_CODBARRA3]    = barraCod3                   + Separador;
//            linha[colReg_CODSENTIDO]   = sentidoCod                  + Separador;
            linha[colReg_CODBARREF]    = barrarefCod                 + Separador;
            linha[colReg_CODBARREFINV] = barrarefinvCod              + Separador;

            linha[colReg_DESCRICAO]    = "Barra de referência não está conectada ao regulador.";
            break;

        default:
            ;
        }
        //concatena os campos em uma linha
        for (int i = 0; i < colReg_COUNT; i++)
            linhaPreenchida += linha[i];
        log_reg->AddObject(linhaPreenchida, reg);
    }
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
bool     __fastcall  TDiagReg::Executa(void)
{
    //variaveis locais
    VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
    VTRegulador *reg;
    TList       *lisReg = new TList();

    //inicialza infos
    infoReg.n_total = 0;
    infoReg.n_verif = 0;
    infoReg.n_ocorrencias = 0;
    if(log_reg)
       IniciaLog(log_reg);
    //if(log_reg)
    //    {log_reg->Clear();}
    try
    {
        //percorre todos os reguladores
        redes->LisLigacao(lisReg, eqptoREGULADOR);
        infoReg.n_total = lisReg->Count;
        for(int nr = 0; nr < infoReg.n_total; nr++)
        {
            reg = (VTRegulador*) lisReg->Items[nr];
            temErro = false;
            //Verifica regulador
            VerificaReg(reg);
            if(temErro)
                infoReg.n_ocorrencias++;
        }
        //altera o boolean mostrando que foi executado
        executado = true;
        // deleta a lista
        if (lisReg)
        {
            delete lisReg;
            lisReg = NULL;
        }
        return true;
    }
    catch(Exception &e)
    {return false;}
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TDiagReg::Inicia(void)
{
   //
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TDiagReg::IniciaCheckList(TCheckListBox *checkListBox)
{
    checkListBox->Clear();
    //checkListBox->Items->Add("Coordenadas nulas");
    checkListBox->Items->Add(op_REG_TEXT[opRegBARREF]);
    checkListBox->Checked[opRegBARREF] = this->OpcaoDiagBarRef;
}
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void  __fastcall  TDiagReg::IniciaLog(TStringList *log)
{
    //variaveis locais
    AnsiString  Separador = ";";
    AnsiString  linha[colReg_COUNT];
    AnsiString  linhaPreenchida = "";
    if(log)
    {
        //limpa o log
        log->Clear();

        linha[colReg_SEGMENTO]     = "Segmento"      + Separador;
        linha[colReg_REDE]         = "Rede"          + Separador;
        linha[colReg_EQPTO]        = "Eqpto"         + Separador;
        linha[colReg_CODIGO]       = "Código"        + Separador;
        linha[colReg_TIPOERRO]     = "Tipo Erro"     + Separador;
        linha[colReg_CODBARRA1]    = "Barra1"        + Separador;
        linha[colReg_CODBARRA2]    = "Barra2"        + Separador;
//        linha[colReg_CODBARRA3]    = "Barra3"        + Separador;
//        linha[colReg_CODSENTIDO]   = "Sentido"       + Separador;
        linha[colReg_CODBARREF]    = "Barra Ref"     + Separador;
        linha[colReg_CODBARREFINV] = "Barra Ref Inv" + Separador;
        linha[colReg_DESCRICAO]    = "Descrição";
        //concatena os campos em uma linha
        for (int i = 0; i < colReg_COUNT; i++)
            linhaPreenchida += linha[i];
        log->Add(linhaPreenchida);
    }
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
long __fastcall TDiagReg::PM_GetItensVerificados(void)
{
    if(executado)
        return infoReg.n_verif;
    return -1;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
TStringList* __fastcall TDiagReg::PM_GetLog(void)
{
    if(executado)
        return log_reg;
    return NULL;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
long __fastcall TDiagReg::PM_GetOcorrencias(void)
{
    if(executado)
        return infoReg.n_ocorrencias;
    return -1;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void  __fastcall  TDiagReg::PreencheListView(TListView *listView)
{
    //variaveis locais
    TListItem   *listItem;
    TListColumn *listColumn;
    TStringList *campos = new TStringList();

    //tentar melhorar performance do listview
    listView->Items->BeginUpdate();
    //limpa o listView
    listView->Clear();
    listView->Columns->Clear();

    campos->Clear();
    //Extrai campos da linha
    ExtraiStrings(log_reg->Strings[0], ";", campos);

    //adiciona as colunas
    listColumn = listView->Columns->Add();
    listColumn->Width = 62;
    listColumn->Caption = campos->Strings[colReg_SEGMENTO];
    //listColumn->Caption = "Segmento";
    //listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 50;
    listColumn->Caption = campos->Strings[colReg_REDE];
    //listColumn->Caption = "Rede";
    //listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 50;
    listColumn->Caption = campos->Strings[colReg_EQPTO];
    //listColumn->Caption = "Eqpto";
    //listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 50;
    listColumn->Caption = campos->Strings[colReg_CODIGO];
    //listColumn->Caption = "Código";
    //listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 60;
    listColumn->Caption = campos->Strings[colReg_TIPOERRO];
    //listColumn->Caption = "Tipo Erro";
    //listColumn->Width = ColumnHeaderWidth;

    listColumn = listView->Columns->Add();
    listColumn->Width = 80;
    listColumn->Caption = campos->Strings[colReg_CODBARRA1];
    // listColumn->Caption = "Barra 1";
    // listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 80;
    listColumn->Caption = campos->Strings[colReg_CODBARRA2];
    // listColumn->Caption = "Barra 2";
    // listColumn->Width = ColumnHeaderWidth;
//    listColumn = listView->Columns->Add();
//    listColumn->Width = 80;
//    listColumn->Caption = campos->Strings[colReg_CODBARRA3];
//    // listColumn->Caption = "Barra 3";
//    // listColumn->Width = ColumnHeaderWidth;
//    listColumn = listView->Columns->Add();
//    listColumn->Width = 80;
//    listColumn->Caption = campos->Strings[colReg_CODSENTIDO];
//    // listColumn->Caption = "Sentido";
//    // listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 80;
    listColumn->Caption = campos->Strings[colReg_CODBARREF];
    // listColumn->Caption = "Barra Ref";
    // listColumn->Width = ColumnHeaderWidth;
    listColumn = listView->Columns->Add();
    listColumn->Width = 80;
    listColumn->Caption = campos->Strings[colReg_CODBARREFINV];
    // listColumn->Caption = "Barra Inv";
    // listColumn->Width = ColumnHeaderWidth;

    listColumn = listView->Columns->Add();
    listColumn->Width = 150;
    listColumn->Caption = campos->Strings[colReg_DESCRICAO];
    //listColumn->Caption = "Descrição";
    //listColumn->Width = ColumnHeaderWidth;

    //preenche linhas e colunas
    for(int nl = 1; nl < log_reg->Count; nl++)
    {
        campos->Clear();
        //Extrai campos da linha
        ExtraiStrings(log_reg->Strings[nl], ";", campos);
        listItem = listView->Items->Add();
        listItem->Data = log_reg->Objects[nl];
        listItem->Caption = campos->Strings[0] ;
        for(int nc = 1; nc < campos->Count; nc++)
            listItem->SubItems->Add(campos->Strings[nc]);
    }
    //destroi o TStrings
    if(campos)
    {
        delete campos; 
        campos = NULL;
    }
    //tentar melhorar performance do listview
    listView->Items->EndUpdate();
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
bool __fastcall TDiagReg::LeArquivo(void)
{
    return false;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TDiagReg::LeCheckList(TCheckListBox *checkListBox)
{
    this->OpcaoDiagBarRef = checkListBox->Checked[opRegBARREF];
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
bool  __fastcall  TDiagReg::SalvaArquivo(void)
{
    return false;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TDiagReg::ShowFormCfg(void)
{
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
int __fastcall TDiagReg::ShowFormCfgModal(void)
{
    return -1;
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
int __fastcall TDiagReg::Tipo(void)
{
    return(dgeREG);
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
AnsiString __fastcall TDiagReg::TipoAsString(void)
{
    return "Reguladores";
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TDiagReg::VerificaReg(VTRegulador *reg)
{
    //DIAG_REG *diagReg;
    VTBarra *barra1,       *barra2,        *barra3,
            *sentido,      *barraref,      *barrarefinv;
    int     barraId1  = 0, barraId2   = 0, barraId3      = 0,
            sentidoId = 0, barrarefId = 0, barrarefinvId = 0;

    //verifica se as barras de referencia do regulador são as mesmas da sua ligação
    barra1        = reg->pbarra1;
    barra2        = reg->pbarra2;
//    barra3        = reg->pbarra3;
//    sentido       = reg->sentido.bar_ref;
    barraref      = reg->cfg_auto.bar_ref;
    barrarefinv   = reg->cfg_auto.bar_ref_inv;

    if(barra1)
        barraId1      = reg->pbarra1->Id;
    if(barra2)
        barraId2      = reg->pbarra2->Id;
//    if(barra3)
//        barraId3      = reg->pbarra3->Id;
//    if(sentido)
//        sentidoId     = reg->sentido.bar_ref->Id;
    if(barraref)
        barrarefId    = reg->cfg_auto.bar_ref->Id;
    if(barrarefinv)
        barrarefinvId = reg->cfg_auto.bar_ref_inv->Id;

    if (dgReg.barref)
    {
        if(
//           (
//            (sentido     != reg->pbarra1) &&
//            (sentido     != reg->pbarra2)
//           )
//             ||
           (
            (barraref    != reg->pbarra1) &&
            (barraref    != reg->pbarra2)
           )
             ||
           (
            (barrarefinv != reg->pbarra1) &&
            (barrarefinv != reg->pbarra2)
           )
          )
            AddLinhaLog(reg, opRegBARREF);
    }
    //incrementa o contador de verificações
    infoReg.n_verif++;
}

//---------------------------------------------------------------------------
