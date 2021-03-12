// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TParse.h"
#include "VTExtrator.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTParse* NewObjParse(VTApl *apl)
{
    return (new TParse(apl));
}

// -----------------------------------------------------------------------------
__fastcall TParse::TParse(VTApl *apl)
{
    // salva ponteiro p/ objetos
    this->apl = apl;
    try
    { // cria objetos TStringList
        campos = new TStringList();
        lines = new TStringList();
    }
    catch (Exception &e)
    {
    }
}

// -----------------------------------------------------------------------------
__fastcall TParse::~TParse(void)
{
    // destrói objetos criados
    if (lines)
    {
        delete lines;
        lines = NULL;
    }
    if (campos)
    {
        delete campos;
        campos = NULL;
    }
}

// -----------------------------------------------------------------------------
bool __fastcall TParse::Executa(AnsiString arq_txt)
{
    // variáveis locais
    bool sucesso = true;
    int teste = 0;

    // obtém objetos
    extrator = (VTExtrator*)apl->GetObject(__classid(VTExtrator));
    // inicia objeto Extrator
    if (!extrator->TrataInicio())
        return (false);
    // lê arquivo
    lines->LoadFromFile(arq_txt);
    for (int nl = 0; (sucesso) && (nl < lines->Count); nl++)
    {
        teste = nl;
        // extrai campos da linha de texto
        if (ExtraiCampos(lines->Strings[nl], campos) == 0)
            continue;
        // trata campos com caracteres especiais
        TrataCampos();
        // identifica tipo de dado
        switch (PalavraChave())
        {
        case tipoDUMMY: // nada a fazer
            break;
        case tipoINVALIDO: // erro: termina importação
            sucesso = false;
            break;
        case tipoESTUDO:
            sucesso = extrator->InsereEstudo(campos);
            break;
        case tipoALTERNATIVA:
            sucesso = extrator->InsereAlternativa(campos);
            break;
        case tipoOBRA:
            sucesso = extrator->InsereObra(campos);
            break;
        case tipoREDE_ALTERNATIVA:
            sucesso = extrator->InsereRedeAlternativa(campos);
            break;
        case tipoDEMANDA_POTENCIA:
            sucesso = extrator->InsereDemandaPotencia(campos);
            break;
        case tipoPOTENCIA_INSTALADA:
            sucesso = extrator->InserePotenciaInstalada(campos);
            break;
        case tipoELEMENTO_POTENCIA: // DVK 2015.04.28
            sucesso = extrator->InsereElementoPotencia(campos);
            break;
        case tipoINFORMACAO_TECNICA:
            sucesso = extrator->InsereInformacaoTecnica(campos);
            break;
        case tipoVALOR:
            sucesso = extrator->InsereValorInformacaoTecnica(campos);
            break;
        case tipoEQUIPAMENTO:
            sucesso = extrator->InsereEquipamento(campos);
            break;
        case tipoREDE:
            sucesso = extrator->InsereRede(campos);
            break;
        case tipoSOLICITACAO:
            sucesso = extrator->InsereSolicitacao(campos);
            break;
        case tipoMUNICIPIO:
            sucesso = extrator->InsereMunicipio(campos);
            break;
        default: // erro: termina importação
            sucesso = false;
            break;
        }
    }
    // finaliza objeto Extrator
    extrator->TrataTermino();
    return (sucesso);
}

// ---------------------------------------------------------------------------
int __fastcall TParse::ExtraiCampos(AnsiString txt, TStrings *campos)
{
    // variáveis locais
#define EOS '\0'
    char *p_ini, *p_fim;
    char separador = ';';
    int len = txt.Length();

    // reinicia StrinList campos
    campos->Clear();
    // filtra linha com menos de 2 caracteres
    if (len < 2)
        return (0);
    // filtra linha de comentário
    if ((txt[1] == '/') && (txt[2] == '/'))
        return (0);
    // se necessário, inclui separador no final de txt
    if (txt[len] != separador)
        txt += separador;
    // extrai os campos delimitados por separador em txt
    p_ini = txt.c_str();
    while ((p_fim = strchr(p_ini, separador)) != NULL)
    { // insere string na lista
        *p_fim = EOS; // insere delimitador de string
        campos->Add(AnsiString(p_ini).Trim());
        // copia em pstr o substring p_ini a p_fim-1
        *p_fim = separador; // restaura separador no string original
        p_ini = p_fim + 1;
    }
    return (campos->Count);
}

// -----------------------------------------------------------------------------
int __fastcall TParse::PalavraChave(void)
{
    // variáveis locais
    AnsiString str;

    // proteção
    if (campos->Count == 0)
        return (tipoDUMMY);
    // determina primeiro string
    str = campos->Strings[0];
    // versão
    if (str.AnsiCompareIC("<estudo>") == 0)
        return (tipoESTUDO);
    if (str.AnsiCompareIC("<alternativa>") == 0)
        return (tipoALTERNATIVA);
    if (str.AnsiCompareIC("<obra>") == 0)
        return (tipoOBRA);
    if (str.AnsiCompareIC("<rede>") == 0)
        return (tipoREDE_ALTERNATIVA);
    if (str.AnsiCompareIC("<demandaPotencia>") == 0)
        return (tipoDEMANDA_POTENCIA);
    if (str.AnsiCompareIC("<potenciaInstalada>") == 0)
        return (tipoPOTENCIA_INSTALADA);
    if (str.AnsiCompareIC("<elementoPotencia>") == 0)
        return (tipoELEMENTO_POTENCIA); // DVK 2015.04.28
    if (str.AnsiCompareIC("<informacaoTecnica>") == 0)
        return (tipoINFORMACAO_TECNICA);
    if (str.AnsiCompareIC("<valor>") == 0)
        return (tipoVALOR);
    if (str.AnsiCompareIC("<equipamento>") == 0)
        return (tipoEQUIPAMENTO);
    if (str.AnsiCompareIC("<rede_eqp>") == 0)
        return (tipoREDE);
    if (str.AnsiCompareIC("<solicitacao>") == 0)
        return (tipoSOLICITACAO);
    if (str.AnsiCompareIC("<municipio>") == 0)
        return (tipoMUNICIPIO);
    // não encontrou palavra chave
    return (tipoINVALIDO);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16512523
void __fastcall TParse::TrataCampos(void)
{
    // variaveis locais
    AnsiString cr = "<cr>";
    AnsiString sc = "<sc>";
    AnsiString valor;
    int pos, length;

    for (int n = 0; n < campos->Count; n++)
    {
        valor = campos->Strings[n];
        // trata quebra de linha
        while ((pos = valor.AnsiPos(cr)) > 0)
        {
            length = valor.Length() - 2;
            valor = valor.SubString(0, pos - 1) + "\r\n" + valor.SubString(pos + 4, length);
            campos->Strings[n] = valor;
        }
        // trata ponto e virgula
        while ((pos = valor.AnsiPos(sc)) > 0)
        {
            length = valor.Length() - 3;
            valor = valor.SubString(0, pos - 1) + ";" + valor.SubString(pos + 4, length);
            campos->Strings[n] = valor;
        }
    }

}
// -----------------------------------------------------------------------------
// eof
