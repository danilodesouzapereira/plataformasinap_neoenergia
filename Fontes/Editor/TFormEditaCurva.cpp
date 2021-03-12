// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include "TFormEditaCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
// #include "..\Legenda\VTLegenda.h"
// #include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

// ---------------------------------------------------------------------------
__fastcall TFormEditaCurva::TFormEditaCurva(TForm* form_owner, VTApl *apl_owner,
    TWinControl *parent) : TForm(Owner)
{
    // salva ponteiro p/ objetos
    this->apl = apl_owner;
    // insere em seu Parent
    if (parent)
    {
        Parent = parent;
        Align = alClient;
    }
    // cria objeto Patamares e define sua escala
    patamares = DLL_NewObjPatamares(apl);
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaCurva::~TFormEditaCurva(void)
{
    // libera memóra alocada
    coluna.Length = 0;
    // destrói objetos criados
    if (patamares)
    {
        delete patamares;
        patamares = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::CBoxNumPtoInicia(void)
{
    // verifica o número de pontos da Curva definido pela Curva
    switch (curva->Escala)
    {
    case pat24HORA:
        CBoxNumPto->ItemIndex = 0;
        break;
    case pat12HORA:
        CBoxNumPto->ItemIndex = 1;
        break;
    case pat08HORA:
        CBoxNumPto->ItemIndex = 2;
        break;
    case pat06HORA:
        CBoxNumPto->ItemIndex = 3;
        break;
    case pat04HORA:
        CBoxNumPto->ItemIndex = 4;
        break;
    case pat03HORA:
        CBoxNumPto->ItemIndex = 5;
        break;
    case pat02HORA:
        CBoxNumPto->ItemIndex = 6;
        break;
    case pat01HORA:
        CBoxNumPto->ItemIndex = 7;
        break;
    case pat15MIN:
        CBoxNumPto->ItemIndex = 8;
        break;
    case pat10MIN:
        CBoxNumPto->ItemIndex = 9;
        break;
    case pat05MIN:
        CBoxNumPto->ItemIndex = 10;
        break;
    case patEDITAVEL:
        CBoxNumPto->ItemIndex = 11;
        break;
    default: // curva c/ escala indefinida
        curva->Escala = pat01HORA;
        // chamada recursiva c/ Escala definida
        CBoxNumPtoInicia();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::CBoxNumPtoChange(TObject *Sender)
{
    // redefine número de Patamares
    switch (CBoxNumPto->ItemIndex)
    {
    case 0:
        patamares->Escala = pat24HORA;
        break;
    case 1:
        patamares->Escala = pat12HORA;
        break;
    case 2:
        patamares->Escala = pat08HORA;
        break;
    case 3:
        patamares->Escala = pat06HORA;
        break;
    case 4:
        patamares->Escala = pat04HORA;
        break;
    case 5:
        patamares->Escala = pat03HORA;
        break;
    case 6:
        patamares->Escala = pat02HORA;
        break;
    case 7:
        patamares->Escala = pat01HORA;
        break;
    case 8:
        patamares->Escala = pat15MIN;
        break;
    case 9:
        patamares->Escala = pat10MIN;
        break;
    case 10:
        patamares->Escala = pat05MIN;
        break;
    default:
        patamares->Escala = patEDITAVEL;
        break;
    }
    // reconfigura a Curva (muda número de pontos, salvando seus valores originais)
    curva->Reconfigura(patamares->Escala);
    // reapresenta pontos da Curva
    SGridInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::DefineCurva(VTCurva *curva)
{
    // salva ponteiro p/ objeto
    this->curva = curva;
    // atualiza CBoxNumPto p/ exibir número de pontos da Curva
    CBoxNumPtoInicia();
    // redefine número de Patamares
    switch (curva->Escala)
    {
    case pat24HORA:
        patamares->Escala = pat24HORA;
        break;
    case pat12HORA:
        patamares->Escala = pat12HORA;
        break;
    case pat08HORA:
        patamares->Escala = pat08HORA;
        break;
    case pat06HORA:
        patamares->Escala = pat06HORA;
        break;
    case pat04HORA:
        patamares->Escala = pat04HORA;
        break;
    case pat03HORA:
        patamares->Escala = pat03HORA;
        break;
    case pat02HORA:
        patamares->Escala = pat02HORA;
        break;
    case pat01HORA:
        patamares->Escala = pat01HORA;
        break;
    case pat15MIN:
        patamares->Escala = pat15MIN;
        break;
    case pat10MIN:
        patamares->Escala = pat10MIN;
        break;
    case pat05MIN:
        patamares->Escala = pat05MIN;
        break;
    default:
        patamares->Escala = pat01HORA;
        break;
    }
    // reapresenta pontos da Curva
    SGridInicia();
    // desabilita edição p/ Curva típica
    Enabled = !curva->Tipica;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::InsereColuna(int ind_curva, AnsiString caption, double valor_min,
    double valor_max)
{
    // variáveis locais
    int index;

    // salva índice da coluna
    index = coluna.Length;
    // salva dados da nova coluna
    coluna.Length = coluna.Length + 1;
    coluna[index].caption = caption;
    coluna[index].ind_curva = ind_curva;
    coluna[index].valor_min = valor_min;
    coluna[index].valor_max = valor_max;

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva::LePontosCurva(void)
{
    // verifica se Curva c/ patamares de período variável
    if (CBoxNumPto->ItemIndex == 11)
        return (LeDadosPatamarVariavel());
    else
        return (LeDadosPatamarFixo());
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva::LeDadosPatamarFixo(void)
{
    // variáveis locais
    int index;
    strHM hm_ini, hm_fim;
    double valor[nvCURVA_PQVT];
    VTPatamar *patamar;

    try
    { // atualiza pontos da Curva
        for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
        nr++, np++)
        {
            patamar = patamares->Patamar[np];
            hm_ini.hora = patamar->HoraIni;
            hm_ini.minuto = patamar->MinutoIni;
            hm_fim.hora = patamar->HoraFim;
            hm_fim.minuto = patamar->MinutoFim;
            // lê valores do SGrid
            for (int nc = 1, j = 0; (nc < SGrid->ColCount) && (j < coluna.Length); nc++, j++)
            { // lê o valor
                index = coluna[j].ind_curva;
                valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
                // verifica se é o ângulo
                // if (index == indV)
                if (index == indT) // HPS, 20100920
                { // converte valor de grau p/ radiano
                    valor[index] = DegToRad(valor[index]);
                }
                // valida o valor lido
                if (!ValidaValor(np, valor[index], coluna[j]))
                {
                    Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) + " a " +
                        IntToStr(hm_fim.hora));
                    return (false);
                }
            }
            // atualiza Curva
            if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
            {
                Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
                    IntToStr(hm_fim.hora));
                return (false);
            }
        }
    }
    catch (Exception &e)
    {
        Erro("Valor Inválido: dados da curva");
        return (false);
    }
    // verifica se a Curva cobre o período de 24 horas
    if (!curva->ValidaPeriodo24Horas())
    {
        Erro("A curva deve cobrir o período de 24 horas");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva::LeDadosPatamarVariavel(void)
{
    // variáveis locais
    int index;
    strHM hm_ini, hm_fim;
    double valor[nvCURVA_PQVT];

    // reinicia Curva
    curva->Reinicia();
    try
    { // atualiza pontos da Curva
        for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
        nr++, np++)
        { // lê hora inicial/final do patamar
            if (!SGridExistePeriodo(nr))
                break;
            hm_ini.hora = StrToInteger(SGrid->Cells[0][nr]);
            hm_ini.minuto = 0;
            hm_fim.hora = StrToInteger(SGrid->Cells[1][nr]);
            hm_fim.minuto = 0;
            // reinicia vetor de valores
            for (index = 0; index < nvCURVA_PQVT; index++)
            {
                valor[index] = 0.;
            }
            // lê valores do SGrid
            for (int nc = 2, j = 0; (nc < SGrid->ColCount) && (j < coluna.Length); nc++, j++)
            { // lê o valor da coluna
                index = coluna[j].ind_curva;
                valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
                // verifica se é o ângulo
                if (index == indT)
                { // converte valor de grau p/ radiano
                    valor[index] = DegToRad(valor[index]);
                }
                // valida o valor lido
                if (!ValidaValor(np, valor[index], coluna[j]))
                {
                    Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) + " a " +
                        IntToStr(hm_fim.hora));
                    return (false);
                }
            }
            // insere patamar na Curva
            if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
            {
                Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
                    IntToStr(hm_fim.hora));
                return (false);
            }
        }
    }
    catch (Exception &e)
    {
        Erro("Valor Inválido: dados da curva");
        return (false);
    }
    // verifica se a Curva cobre o período de 24 horas
    if (!curva->ValidaPeriodo24Horas())
    {
        Erro("A curva deve cobrir o período de 24 horas");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva::SGridExistePeriodo(int nr)
{
    // verifica se a linha está preenchida
    for (int nc = 0; nc < 2; nc++)
    {
        if (SGrid->Cells[nc][nr].IsEmpty())
            return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::SGridInicia(void)
{
    if (curva->Escala == patEDITAVEL)
        SGridIniciaPatamarVariavel();
    else
        SGridIniciaPatamarFixo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::SGridIniciaPatamarFixo(void)
{
    // variáveis locais
    int index;
    strHM hm_ini, hm_fim;
    double valor[nvCURVA_PQVT];
    VTPatamar *patamar;

    // limpa SGrid
    for (int nr = 1; nr < SGrid->RowCount; nr++)
        SGrid->Rows[nr]->Clear();
    // reinicia dimensão do SGrid
    SGrid->RowCount = patamares->NumPatamar() + 1;
    SGrid->ColCount = 1 + coluna.Length;
    SGrid->FixedCols = 1;
    // preenche títulos das colunas
    SGrid->Cells[0][0] = "Patamar";
    for (int nc = 1, j = 0; (nc < SGrid->ColCount) && (j < coluna.Length); nc++, j++)
    {
        SGrid->Cells[nc][0] = coluna[j].caption;
    }
    // proteção
    // if (curva->NumeroValores > coluna.Length) return;   //DVK 2013.08.06
    // preenche dados dos pontos da Curva
    for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar()); nr++, np++)
    { // preenche hora/minuto do Patamar
        patamar = (VTPatamar*)patamares->Patamar[np];
        SGrid->Cells[0][nr] = patamar->Nome;
        // obtém ponto da Curva p/ o Patamar
        if (!curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores))
            break;
        for (int nc = 1, j = 0; (nc < SGrid->ColCount) && (j < coluna.Length); nc++, j++)
        {
            index = coluna[j].ind_curva;
            // verifica se é o ângulo
            if (index == indT)
            {
                SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", RadToDeg(valor[index]));
            }
            else
            {
                SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", valor[index]);
            }
        }
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva::SGridIniciaPatamarVariavel(void)
{
    // variáveis locais
    int index;
    AnsiString txt;
    strHM hm_ini, hm_fim;
    double valor[nvCURVA_PQVT];

    // proteção
    if (curva->Escala != patEDITAVEL)
        return;

    // limpa SGrid
    for (int nr = 1; nr < SGrid->RowCount; nr++)
        SGrid->Rows[nr]->Clear();
    // reinicia dimensão do SGrid
    SGrid->RowCount = 24 + 1;
    SGrid->ColCount = 2 + coluna.Length;
    SGrid->FixedCols = 0;
    // preenche títulos das colunas
    SGrid->Cells[0][0] = "Hora início";
    SGrid->Cells[1][0] = "Hora fim";
    for (int nc = 2, j = 0; (nc < SGrid->ColCount) && (j < coluna.Length); nc++, j++)
    {
        SGrid->Cells[nc][0] = coluna[j].caption;
    }
    // preenche dados dos pontos da Curva
    for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar()); nr++, np++)
    { // obtém ponto da Curva p/ o Patamar
        if (!curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores))
            break;
        // preenche SGrid
        SGrid->Cells[0][nr] = txt.sprintf("%02d", hm_ini.hora);
        SGrid->Cells[1][nr] = txt.sprintf("%02d", hm_fim.hora);
        for (int nc = 2, j = 0; (nc < SGrid->ColCount) && (j < coluna.Length); nc++, j++)
        {
            index = coluna[j].ind_curva;
            // verifica se é o ângulo
            if (index == indT)
            {
                SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", RadToDeg(valor[index]));
            }
            else
            {
                SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", valor[index]);
            }
        }
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva::ValidaHora(int np, int hora)
{
    // valida hora
    if ((hora < 0) || (hora > 24))
    {
        Erro("Hora inválida no patamar " + IntToStr(np));
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva::ValidaValor(int np, double valor, strCOLUNA &coluna)
{
    // valida valor mínimo
    if ((valor < coluna.valor_min) || (valor > coluna.valor_max))
    {
        Erro(coluna.caption + ": valor inválido no patamar " + IntToStr(np));
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// eof
