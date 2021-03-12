// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TMontaEstudo.h"
#include "VTCria.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Obra\VTAcao.h"
#include "..\..\Obra\VTObra.h"
#include "..\..\Obra\ItemObra\VTItemObra.h"
#include "..\..\Obra\VTObras.h"
#include "..\..\Planejamento\VTAlternativa.h"
#include "..\..\Planejamento\VTEstudo.h"
#include "..\..\Planejamento\VTIpe.h"
#include "..\..\Planejamento\VTPlanejamento.h"
#include "..\..\Rede\VTEqpto.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
VTMonta* NewObjMontaEstudo(VTApl *apl)
{
    return (new TMontaEstudo(apl));
}

// ---------------------------------------------------------------------------
__fastcall TMontaEstudo::TMontaEstudo(VTApl *apl)
{
    // salva ponteiros p/ objetos
    this->apl = apl;
    fases = (VTFases*)apl->GetObject(__classid(VTFases));
    plog = (VTLog*)apl->GetObject(__classid(VTLog));
    // cria objetos
    cria = NewObjCria(apl);
    lisEQP = new TList();
    // passa lista de Eqptos de obras p/ VTCria
    cria->DefLisEqptoObra(lisEQP);
}

// ---------------------------------------------------------------------------
__fastcall TMontaEstudo::~TMontaEstudo(void)
{
    // destrói lista sem destruir seus objetos
    if (lisEQP)
    {
        delete lisEQP;
        lisEQP = NULL;
    }
    // destrói objetos
    if (cria)
    {
        delete cria;
        cria = NULL;
    }
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaEstudo::DefineBarrraInicial(AnsiString rede_cod, int barra_id)
{
    return (true);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaEstudo::ExisteBarra(int barra_id)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaEstudo::ExisteBarra(AnsiString codigo)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaEstudo::ExisteBarraCoordUtm(int utm_x, int utm_y)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaEstudo::ExisteBarraCoordUtm(int utm_x, int utm_y, double vnom_kv)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaEstudo::ExisteBarraCoordEsq(int esq_x1, int esq_y1, int esq_x2,
    int esq_y2)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::ExisteEqpto(int eqpto_tipo, AnsiString codigo)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::ExisteEqptoDaObra(int tipo_eqpto, int eqpto_id, bool remove)
{
    // variáveis locais
    VTEqpto *eqpto;

    // verifica se o Eqpto existe na lista temporária de Eqptos do estudo
    for (int n = 0; n < lisEQP->Count; n++)
    {
        eqpto = (VTEqpto*)lisEQP->Items[n];
        // if ((eqpto->Tipo() == eqpto_id)&&(eqpto->Id == eqpto_id)) return(eqpto);
        if (eqpto->Id == eqpto_id)
        {
            if (eqpto->Tipo() == tipo_eqpto)
            { // verifica se deve remover o Eqpto da lista
                if (remove)
                    lisEQP->Delete(n);
                // retorna o Eqpto
                return (eqpto);
            }
        }
    }
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::ExisteEqptoDaRede(int tipo_eqpto, int eqpto_id)
{
    // variáveis locais
    VTEqpto *eqpto;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    // verifica se o Eqpto existe na rede original
    return (redes->ExisteEqpto(tipo_eqpto, eqpto_id));
}

// -----------------------------------------------------------------------------
int __fastcall TMontaEstudo::ExisteFase(AnsiString codigo)
{
    return (fases->ExisteFase(codigo));
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::ExisteRede(AnsiString codigo)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::ExisteTrecho(int utm_x1, int utm_y1, int utm_x2, int utm_y2)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaEstudo::InsereArranjo(strARRANJO2 &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereBarraEqpto(strBARRA &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjBarra(str)) == NULL)
    {
        return (NULL);
    }
    // altera atributo p/ evitar que uma Barra que foi apenas movida apareça como isolada
    eqpto->Status[sttISOLADO] = false;
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
void __fastcall TMontaEstudo::InsereBarraStart(void)
{
    // nada a fazer
}

// -----------------------------------------------------------------------------
void __fastcall TMontaEstudo::InsereBarraStop(void)
{
    // nada a fazer
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereBateria(strBATERIA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCabo(strCABO &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCapacitor(strCAPACITOR &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjCapacitor(str)) == NULL)
    {
        return (NULL);
    }
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCapserie(strCAPSERIE &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjCapserie(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCarga(strCARGA &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjCarga(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCargaNL(strCARGANL &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjCargaNL(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP. Na inserção de uma harmônica é retornado o mesmo
    // ponteiro da fundamental, logo não deve inserir novamente.
    if (!lisEQP->IndexOf(eqpto) < 0)
    {
        lisEQP->Add(eqpto);
    }
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereChave(strCHAVE &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjChave(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCluster(strCLUSTER &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjCluster(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereCorte(int corte_id, int mrede_id)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaEstudo::InsereCurvaDemanda(VTCarga *carga, strCURVA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaEstudo::InsereCurvaTipicaCapacitor(strCURVA_TIPICA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaEstudo::InsereCurvaTipicaCarga(strCURVA_TIPICA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaEstudo::InsereCurvaTipicaGerador(strCURVA_TIPICA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaEstudo::InsereCurvaTipicaIP(strCURVA_TIPICA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaEstudo::InsereCurvaTipicaReator(strCURVA_TIPICA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTDominio* __fastcall TMontaEstudo::InsereDominio(strDOMINIO &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16968316
bool __fastcall TMontaEstudo::InsereEstudo(strPLAN_ACAO &str)
{
    // variáveis locais
    bool remove;
    VTEqpto *original, *alterado, *copia_original;
    VTEstudo *estudo;
    VTAlternativa *alternativa;
    VTObra *obra;
    VTItemObra *itemObra = NULL;
    VTAcao *acao = NULL;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
    AnsiString func = "[TMontaEstudo::InsereEstudo(strPLAN_ACAO &str)] id=" + IntToStr(str.acao_id);

    // verifica se já existe Estudo, Alternativa, Obra
    if ((estudo = planejamento->ExisteEstudo(str.estudo_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe estudo " + IntToStr(str.estudo_id));
        return (false);
    }
    if ((alternativa = estudo->ExisteAlternativa(str.alternativa_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe alternativa " + IntToStr(str.alternativa_id));
        return (false);
    }
    if ((obra = alternativa->Obras->ExisteObra(str.obra_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe obra " + IntToStr(str.obra_id));
        return (true);
    }
    try
    { // cria uma nova Acao de acordo com seu tipo
        switch (str.tipo_acao)
        {
        case acaoALTERA:
            // determina Eqpto original e alterado
            if ((original = ExisteEqptoDaRede(str.tipo_eqpto, str.eqpto_id)) == NULL)
            {
                if ((original = ExisteEqptoDaObra(str.tipo_eqpto, str.eqpto_id, remove = false))
                    == NULL)
                {
                    // Erro(" MontaEstudo / InsereAcaoAltera / Ligacao : não existe eqpto_original ");
                    if (plog)
                        plog->Add(func + " Não existe eqpto_original (altera)");
                    return (false);
                }
            }
            // determina eqpto antes da alteração
            if ((copia_original = ExisteEqptoDaObra(str.tipo_eqpto, str.eqpto_antes_id, remove =
                true)) == NULL)
            {
                // Erro(" MontaEstudo / InsereAcaoAltera / Ligacao : não existe copia_original ");
                if (plog)
                    plog->Add(func + " Não existe copia_original (altera)");
                return (false);
            }
            // determina eqpto depois da alteração
            if ((alterado = ExisteEqptoDaObra(str.tipo_eqpto, str.eqpto_depos_id, remove = true))
                == NULL)
            {
                // Erro(" MontaEstudo / InsereAcaoAltera / Ligacao : não existe copia_alterado ");
                if (plog)
                    plog->Add(func + " Não existe copia_alterado (altera)");
                return (false);
            }
            // insere Acao - a sequência de parâmetros está correta
            if ((acao = obra->AddAcaoAltera(original, alterado, copia_original)) == NULL)
            {
                if (plog)
                    plog->Add(func + " Não foi possível criar ação altera");
                return (false);
            }
            // IMPORTANTE: todos os eqptos devem ter o mesmo ID do Eqpto original pois o
            // eqpto original não pode ter seu ID alterado na execução da AcaoAltera
            copia_original->Id = original->Id;
            alterado->Id = original->Id;
            /*
             #ifdef _DEBUG
             //teste em modo debug =====================
             if (original->TipoLigacao())
             {
             VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
             VTRede    *rede_isolada       = redes->RedeIsolada();
             VTLigacao *lig_original       = (VTLigacao*)original;
             VTLigacao *lig_copia_original = (VTLigacao*)copia_original;
             VTLigacao *lig_alterado       = (VTLigacao*)alterado;

             if (lig_original->rede != lig_copia_original->rede)
             {
             Aviso(" MontaEstudo / InsereAcaoAltera / Ligacao : Rede da cópia original inválida ");
             if      (lig_original->rede       == rede_isolada) lig_original->rede       = lig_copia_original->rede;
             else if (lig_copia_original->rede == rede_isolada) lig_copia_original->rede = lig_original->rede;
             else                                               lig_original->rede       = lig_copia_original->rede;
             }
             if (lig_alterado->rede == rede_isolada)
             {
             Aviso(" MontaEstudo / InsereAcaoAltera / Ligacao : Rede da cópia alterada inválida ");
             lig_alterado->rede = lig_original->rede;
             }
             }
             //fim teste em modo debug =================
             #endif
             */
            break;
        case acaoINSERE:
            // determina Eqpto original
            if ((original = ExisteEqptoDaObra(str.tipo_eqpto, str.eqpto_id, remove = false))
                == NULL)
            {
                if (plog)
                    plog->Add(func + " Não existe eqpto_original (insere)");
                return (false);
            }
            // insere Acao
            if ((acao = obra->AddAcaoInsere(original)) == NULL)
            {
                if (plog)
                    plog->Add(func + " Não foi possível criar ação insere");
                return (false);
            }
            break;
        case acaoRETIRA:
            // determina Eqpto original
            if ((original = ExisteEqptoDaRede(str.tipo_eqpto, str.eqpto_id)) == NULL)
            {
                if ((original = ExisteEqptoDaObra(str.tipo_eqpto, str.eqpto_id, remove = false))
                    == NULL)
                {
                    if (plog)
                        plog->Add(func + " Não existe eqpto_original (retira)");
                    return (false);
                }
            }
            // insere Acao
            if ((acao = obra->AddAcaoRetira(original)) == NULL)
            {
                if (plog)
                    plog->Add(func + IntToStr(str.acao_id) + " Não foi possível criar ação retira");
                return (false);
            }
            break;
        default:
            if (plog)
                plog->Add(func + " Ação de tipo inválido");
            return (false);
        }
        // apos inserir a acao na obra, insere o ponteiro no itemobra correspondente
        if (acao != NULL)
        {
            if ((itemObra = obra->ExisteItemObra(str.item_id)) != NULL)
            {
                itemObra->InsereAcao(acao);
            }
        }
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(func + " Erro desconhecido ao inserir ação");
        return (false);
    }
    return (true);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422298
// https://sinapsisenergia.teamwork.com/#tasks/16968316
bool __fastcall TMontaEstudo::InsereEstudo(strPLAN_ALTERNATIVA &str)
{
    // variáveis locais
    VTAlternativa *alternativa;
    VTEstudo *estudo;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
    AnsiString func = "[TMontaEstudo::InsereEstudo(strPLAN_ALTERNATIVA &str)] id=" + IntToStr(str.alternativa_id);

    // verifica se já existe Estudo e Alternativa
    if ((estudo = planejamento->ExisteEstudo(str.estudo_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe estudo " + IntToStr(str.estudo_id));
        return (false);
    }
    if ((alternativa = estudo->ExisteAlternativa(str.alternativa_id)) != NULL)
    {
        return (true);
    }
    try
    { // cria uma nova Alternativa
        alternativa = estudo->InsereAlternativa(str.codigo, false);
        // inicia dados
        alternativa->Id = str.alternativa_id;
        alternativa->Codigo = str.codigo;
        alternativa->Justificativa = str.justificativa;
        alternativa->Escolhida = false;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(func + " Erro desconhecido ao inserir alternativa");
        return (false);
    }
    return (true);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16968316
VTEstudo* __fastcall TMontaEstudo::InsereEstudo(strPLAN_ESTUDO &str)
{
    // variáveis locais
    VTEstudo *estudo;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
    AnsiString func = "[TMontaEstudo::InsereEstudo(strPLAN_ESTUDO &str)] id=" + IntToStr(str.estudo_id);

    // verifica se já existe o Estudo
    if ((estudo = planejamento->ExisteEstudo(str.estudo_id)) != NULL)
        return (estudo);
    try
    { // cria um novo Estudo
        estudo = planejamento->InsereEstudo(str.codigo, str.tipo, str.estudo_id);
        // inicia dados
        // estudo->Codigo  = codigo;
        // estudo->Id      = str.estudo_id;
        // estudo->Tipo    = str.tipo;
        estudo->Codigo = str.codigo;
        estudo->Autor = str.autor;
        estudo->Data = str.data;
        estudo->Justificativa = str.justificativa;
    }
    catch (Exception &e)
    {
        if (plog)
            plog->Add(func + " Erro desconhecido ao inserir estudo");
        return (NULL);
    }
    return (estudo);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16968316
bool __fastcall TMontaEstudo::InsereEstudo(strPLAN_IPE &str)
{
    // variáveis locais
    VTAlternativa *alternativa;
    VTEstudo *estudo;
    VTObra *obra;
    VTItemObra *itemObra;
    VTIpeAtributo *ipe_atributo;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
    AnsiString func = "[TMontaEstudo::InsereEstudo(strPLAN_IPE &str)] id=" + IntToStr(str.atributo_id);

    // verifica se já existe Estudo, Alternativa e Obra
    if ((estudo = planejamento->ExisteEstudo(str.estudo_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe estudo " + IntToStr(str.estudo_id));
        return (false);
    }
    if ((alternativa = estudo->ExisteAlternativa(str.alternativa_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe alternativa " + IntToStr(str.alternativa_id));
        return (false);
    }
    if ((obra = alternativa->Obras->ExisteObra(str.obra_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe obra " + IntToStr(str.obra_id));
        return (false);
    }
    // verifica o item obra
    if (str.item_id == 0)
    {
        try
        { // verifica se o Planejamento possui Ipe associado
            if (planejamento->Ipe)
            { // verifica se a Obra possui IpeAtributo associado
                ipe_atributo = planejamento->Ipe->ExisteIpeAtributo(str.atributo_tipo,
                    str.atributo_id, str.item_id, str.descricao);
                if (ipe_atributo != NULL)
                    obra->LisObject->Add(ipe_atributo);
            }
        }
        catch (Exception &e)
        {
            if (plog)
                plog->Add(func + " Erro na leitura do atributo " + IntToStr(str.atributo_id));
            return (false);
        }
    }
    else
    {
        if ((itemObra = obra->ExisteItemObra(str.item_id)) == NULL)
        {
            if (plog)
                plog->Add(func + " Não existe item obra " + IntToStr(str.item_id));
            return (false);
        }
        else
        {
            try
            { // verifica se o Planejamento possui Ipe associado
                if (planejamento->Ipe)
                {
                    // verifica se a Obra possui IpeAtributo associado
                    ipe_atributo = planejamento->Ipe->ExisteIpeAtributo(str.atributo_tipo,
                        str.atributo_id, str.item_id, str.descricao);
                    if (ipe_atributo != NULL)
                        itemObra->InsereIpeAtributo(ipe_atributo);
                }
            }
            catch (Exception &e)
            {
                if (plog)
                    plog->Add(func + " Erro na leitura do item obra " + IntToStr(str.item_id));
                return (false);
            }
        }
    }

    return (true);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16968316
bool __fastcall TMontaEstudo::InsereEstudo(strPLAN_OBRA &str)
{
    // variáveis locais
    VTAlternativa *alternativa;
    VTEstudo *estudo;
    VTObra *obra;
    VTItemObra *itemObra;
    VTIpeAtributo *ipe_atributo;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
    AnsiString func = "[TMontaEstudo::InsereEstudo(strPLAN_OBRA &str)] id=" + IntToStr(str.obra_id);

    // verifica se já existe Estudo e Alternativa
    if ((estudo = planejamento->ExisteEstudo(str.estudo_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe estudo " + IntToStr(str.estudo_id));
        return (false);
    }
    if ((alternativa = estudo->ExisteAlternativa(str.alternativa_id)) == NULL)
    {
        if (plog)
            plog->Add(func + " Não existe alternativa " + IntToStr(str.alternativa_id));
        return (false);
    }
    // verifica se existe a Obra na Alternativa
    if ((obra = alternativa->Obras->ExisteObra(str.obra_id)) != NULL)
    {
        // caso já exista a obra, verifica o item
        if (str.item_id != 0)
        {
            try
            {
                if ((itemObra = obra->ExisteItemObra(str.item_id)) == NULL)
                { // cria o item
                    itemObra = obra->CriaItemObra();
                    itemObra->Id = str.item_id;
                    itemObra->Obra = obra;
                    // associa o item à obra
                    obra->AddItemObra(itemObra);
                }
            }
            catch (Exception &e)
            {
                if (plog)
                    plog->Add(func + " Erro na leitura do item obra em obra existente " +
                    IntToStr(str.item_id));
                return (false);
            }
        }
    }
    else
    { // não existe
        try
        { // cria uma nova Obra
            obra = alternativa->Obras->InsereObra();
            // inicia dados
            obra->Id = str.obra_id;
            obra->Nome = str.codigo;
            obra->Tipo = str.tipo;
            obra->DataInicio = str.data_inicio;
            obra->DataAtivacao = str.data_ativacao;
            obra->Responsavel = str.responsavel;
            obra->ObEspecial = str.ob_especial;
            obra->Custo = str.custo;
            // JCG 2014.02.25 - início
            int ESTE_CODIGO_DEVERA_SER_ELIMINADO_APOS_CONVERSAO_DAS_BASES = 1;
            // verifica se a Obra possui IpeAtributo associado
            if (planejamento->Ipe)
            {
                ipe_atributo = planejamento->Ipe->ExisteIpeAtributo(-1, str.ipe_atributo_id, 0);
                if (ipe_atributo != NULL)
                    obra->LisObject->Add(ipe_atributo);
            }
            // JCG 2014.02.25 - fim
            // JCG 2016.07.29 - dados adicionais
            obra->AlteraTaxaFalha = str.altera_taxa_falha;
            obra->TaxaFalhaInsercao = str.taxa_falha_ins;
            obra->TaxaFalhaSubstituicao = str.taxa_falha_sub;
            // JCG 2016.07.29 - fim
            obra->Finalizada = str.finalizada;
            obra->Bloqueada = str.bloqueada;
            // caso o item seja diferente de 0, cria o item tb
            // caso já exista a obra, verifica o item
            if (str.item_id != 0)
            {
                try
                {
                    if ((itemObra = obra->ExisteItemObra(str.item_id)) == NULL)
                    { // cria o item
                        itemObra = obra->CriaItemObra();
                        itemObra->Id = str.item_id;
                        itemObra->Obra = obra;
                        // associa o item à obra
                        obra->AddItemObra(itemObra);
                    }
                }
                catch (Exception &e)
                {
                    if (plog)
                        plog->Add(func + " Erro na leitura do item obra em obra nova " + IntToStr(str.item_id));
                    return (false);
                }
            }

        }
        catch (Exception &e)
        {
            if (plog)
                plog->Add(func + " Erro desconhecido durante leitura da obra ");
            return (false);
        }
    }
    return (true);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereFiltro(strFILTRO &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjFiltro(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereGerador(strGERADOR &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjGerador(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereMedidor(strMEDIDOR &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjMedidor(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereMedidorCanal(strCANAL &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereMutua(strMUTUA &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjMutua(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereReator(strREATOR &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjReator(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereRede(strREDE &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjRede(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereReducaoCarga(int mrede_id, strCARGA &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereReducaoSuprimento(int mrede_id, strSUPRIMENTO &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereReducaoTrafo(int mrede_id, strTRAFO &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereReducaoYref(int mrede_id, strYREF &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereRegulador(strREGULADOR &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjRegulador(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereSuporte(strSUPORTE &str)
{
    return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereSuprimento(strSUPRIMENTO &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjSuprimento(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TMontaEstudo::InsereTipoChave(strTIPO_EQPTO &str)
 {
 return(false);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TMontaEstudo::InsereTipoChave(strTIPO_CHAVE &str)
{
    return (false);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TMontaEstudo::InsereTipoRede(strTIPO_EQPTO &str)
 {
 return(false);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TMontaEstudo::InsereTipoRede(strTIPO_REDE &str)
{
    return (false);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrafo(strTRAFO &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrafo(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //cria um novo Eqpto
   if ((eqpto = cria->NewObjTrafo(str, barra1, barra2))== NULL) return(NULL);
   //insere Eqpto em lisEQP
   lisEQP->Add(eqpto);
   return(eqpto);
   }

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrafo3E(strTRAFO3E &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrafo3E(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrafoZZ(strTRAFOZZ &str)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrafoZZ(str)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrecho(strTRECHO &str, strARRANJO1 &str_arr)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrecho(str, str_arr)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrecho(strTRECHO &str, strARRANJO2 &str_arr)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrecho(str, str_arr)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrecho(strTRECHO &str, VTArranjo *arranjo)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrecho(str, arranjo)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaEstudo::InsereTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2,
    VTArranjo *arranjo)
{
    // variáveis locais
    VTEqpto *eqpto;

    // cria um novo Eqpto
    if ((eqpto = cria->NewObjTrecho(str, barra1, barra2, arranjo)) == NULL)
        return (NULL);
    // insere Eqpto em lisEQP
    lisEQP->Add(eqpto);
    return (eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaEstudo::RedefineId(void)
{
    // nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TMontaEstudo::RedefineVnomBarra(void)
{
    // nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TMontaEstudo::RetiraBarraIsolada(void)
{
    // nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TMontaEstudo::RetiraRede(VTRede *rede)
{
    // nada a fazer
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TMontaEstudo::TransformaIlhasEmRedes(void)
 {
 return(true);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
bool __fastcall TMontaEstudo::TrataFimDeDados(bool define_vnom)
{
    // variáveis locais
    TList *lisESTUDO;
    TList *lisALTERNATIVA;
    TList *lisOBRA;
    VTEstudo *estudo;
    VTAlternativa *alternativa;
    VTObra *obra;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

    // IMPORTANTE:
    // 1) as Acoes inseridas nas Obras são armazenadas em lisUNDO
    // 2) as Acoes não foram efetivadas quando da sua inserção
    // 3) as Acoes precisam ser transeridas de lisUNDO p/ lisREDO

    // loop p/ todos Estudos
    lisESTUDO = planejamento->LisEstudo();
    for (int ne = 0; ne < lisESTUDO->Count; ne++)
    {
        estudo = (VTEstudo*)lisESTUDO->Items[ne];
        // loop p/ todas Alternativas
        lisALTERNATIVA = estudo->LisAlternativa();
        for (int na = 0; na < lisALTERNATIVA->Count; na++)
        {
            alternativa = (VTAlternativa*)lisALTERNATIVA->Items[na];
            // loop p/ todas as Obras
            lisOBRA = alternativa->Obras->LisObra();
            for (int no = 0; no < lisOBRA->Count; no++)
            {
                obra = (VTObra*)lisOBRA->Items[no];
                // transfere todas Acoes de lisUNDO p/ lisREDO
                while (obra->Undo() != NULL);
                // redefine Obra como não incorporada
                obra->Incorporada = false;
            }
        }
        estudo->MontaRedesObrasLidasDaBase();
    }
    // limpa lista de equipamentos
    lisEQP->Clear();
    return (true);
}

// -----------------------------------------------------------------------------
// eof
