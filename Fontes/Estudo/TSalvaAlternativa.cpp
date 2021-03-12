// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TSalvaAlternativa.h"

#include "..\Apl\VTApl.h"
#include <PlataformaSinap\Fontes\BDihm\VTSalva.h>
#include <PlataformaSinap\Fontes\Copia\VTCopia.h>
#include <PlataformaSinap\Fontes\Diretorio\VTDir.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Edita\VTEdita.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Redutor\VTRedutor.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapChild.h>
#include <PlataformaSinap\Fontes\Topologia\VTTopologia.h>
#include <PlataformaSinap\DLL_Inc\BDihm.h>
#include <PlataformaSinap\DLL_Inc\Copia.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Redutor.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TSalvaAlternativa::TSalvaAlternativa(VTApl *apl_owner)
{
    // salva ponteiro
    this->apl = apl_owner;
}

// ---------------------------------------------------------------------------
__fastcall TSalvaAlternativa::~TSalvaAlternativa(void)
{
    // nada
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18604874
void __fastcall TSalvaAlternativa::Executa(void)
{
    // vari�veis locais
    bool sucesso;
    bool base_nova = true;
    AnsiString arq_rede, full_arq;
    TList *lisREDE;
    VTEqpto *eqpto;
    VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTRedes *redes_copia;
    VTCopia *copia;

    // cria uma nova base de dados vazia
    arq_rede = ExtractFileName(path->ArqRede);
    full_arq = path->ArqRede;
    if (!dir->CriaCopiaRedeSinap(arq_rede))
    {
        Aviso("N�o foi poss�vel criar uma base de dados.");
        return;
    }
    // cria objeto Copia
    copia = DLL_NewObjCopia(apl);
    // cria e usa c�pia do Redes
    redes_copia = copia->DuplicaRedes(redes);
    apl->Remove(redes);
    apl->Add(redes_copia);
    // atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
    path->ArqRede = arq_rede;
    // marca que todas as Redes s�o novas (informa��o importante p/ o Redutor)
    lisREDE = redes_copia->LisRede();
    for (int n = 0; n < lisREDE->Count; n++)
    {
        eqpto = (VTEqpto*)lisREDE->Items[n];
        eqpto->Status[sttNOVO] = true;
    }
    // salva rede na base de dados
    sucesso = SalvaRedeBase(base_nova);
    // retorna Redes original
    apl->Remove(redes_copia);
    apl->Add(redes);
    // destroi objetos
    delete copia;
    delete redes_copia;
    // volta arq_rede original
    path->ArqRede = full_arq;
    // return(sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalvaAlternativa::SalvaRedeBase(bool base_nova)
{
    // vari�veis locais
    bool sucesso;
    VTRedutor *redutor;
    VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
    VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

    // verifica se h� Eqptos isolados
    if (!ValidaEqptoIsolado())
        return (false);
    // muda apresenta��o p/ topologia da Rede
    sinap_child->ActionTopo->Execute();
    // cria objeto VTRedutor
    if ((redutor = DLL_NewObjRedutor(apl)) == NULL)
        return (false);
    // desabilita mensagens do redutor
    redutor->AvisoEnabled = false;
    // monta redes reduzidas
    if (redutor->ExecutaReducaoMNet())
    { // atualiza base de dados
        sucesso = SalvaRedeBaseNova();
    }
    else
    { // avisa usu�rio e pede confirma��o p/ atualizar base de dados
        if (Confirma("N�o foi poss�vel criar as redes equivalentes",
            "A base de dados deve ser atualizada ?") == IDYES)
        { // atualiza base de dados
            sucesso = SalvaRedeBaseNova();
        }
    }
    // destr�i objeto VTRedutor
    delete redutor;
    // reconfigura Redes
    topologia->ClassificaRedes();
    // trata altera��o de rede
    // ActionRedeChangedExecute(NULL);

    return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalvaAlternativa::SalvaRedeBaseNova(void)
{
    // vari�veis locais
    bool sucesso;
    VTSalva *salva;
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

    try
    { // cria objeto VTSalva
        if ((salva = DLL_NewObjSalva(NULL, apl)) == NULL)
            return (false);
        // salva Rede existente na base de dados
        sucesso = salva->SalvaRedeComo();
        if (sucesso)
        { // cancela Undo/Redo
            edita->Clear();
            Aviso("A base de dados foi atualizada corretamente");
        }
        else
        {
            Erro("A base de dados n�o foi atualizada.");
        }
        // destr�i objeto
        delete salva;
    }
    catch (Exception &e)
    {
        sucesso = false;
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalvaAlternativa::ValidaEqptoIsolado(void)
{
    // vari�veis locais
    int resp = IDYES;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTRede *rede = redes->RedeIsolada();

    // verifica se h� Liga��es isoladas
    if ((rede->LisLigacao())->Count > 0)
    { // pede confirma��o do usu�rio
        resp = Confirma("Existem blocos isolados que n�o ser�o salvos na base de dados",
            "Confirma a grava��o ?");
    }
    else if ((rede->LisBarra())->Count > 0)
    { // pede confirma��o do usu�rio
        resp = Confirma("Existem barras isoladas que n�o ser�o salvas na base de dados",
            "Confirma a grava��o ?");
    }
    return (resp == IDYES);
}

// ---------------------------------------------------------------------------
