// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SysUtils.hpp>
#include "TSalva.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Figura\VTBDfigura.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\Hist\VTBDhist.h"
#include "..\Diretorio\DirArq.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTSalva* __fastcall NewObjSalva(TComponent *Owner, VTApl *apl_owner)
{
    return (new TSalva(Owner, apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TSalva::TSalva(TComponent *Owner, VTApl *apl_owner)
{
    // cria objeto Apl local
    apl = NewObjApl(NULL, apl_owner);
    // cria objetos
    apl->Add(bd = DLL_NewObjBD(apl));
    // cria lista de Redes selecionadas
    lisEQP = new TList();
    // habilita progresso
    Progresso = true;
   // Inicia vari�veis
   reservado = false;
}

// ---------------------------------------------------------------------------
__fastcall TSalva::~TSalva(void)
{
    // cancela conex�o com a base
    bd->Desconecta();
    // destr�i objeto Apl
    if (apl)
    {
        delete apl;
        apl = NULL;
    }
    // destr�i listas sem destruir seus objetos
    if (lisEQP)
    {
        delete lisEQP;
        lisEQP = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TSalva::AvisoBaseVersaoInvalida(void)
{
    Aviso("A base de dados � de vers�o anterior e n�o pode ser gravada");
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TSalva::SalvaEstudo(VTEstudo *estudo)
 {
 try{//inicia conex�o com a base
 if (! bd->Conecta()) return(false);
 //prepara Estudo p/ salvar na base de dados
 estudo->PreparaParaSalvarNaBase();
 //verifica se Estudo � novo
 if (estudo->Novo)
 {//Estudo novo: salva c�pia das Redes do Estudo

 }
 else
 {//Estudo existente: exclui da base Alternativas, Obras, Eqptos das Obras

 }
 //define Alternativas como novas

 //define Obras como novas

 //define Eqptos das obras como novos

 }catch(Exception &e)
 {
 return(false);
 }
 return(true);
 }
 */

// ---------------------------------------------------------------------------
VTDominio* __fastcall TSalva::ExisteDominioRede(VTRede *rede)
{
    // vari�veis locais
    VTDominio *dominio;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisDOMINIO = redes->LisDominio();

    for (int n = 0; n < lisDOMINIO->Count; n++)
    {
        dominio = (VTDominio*)lisDOMINIO->Items[n];
        if (dominio->ExisteRede(rede))
            return (dominio);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
VTDominio* __fastcall TSalva::ExisteDominioRedeVizinha(VTRede *rede)
{
    // vari�veis locais
    VTDominio *dominio;
    VTRede *rede_viz;

    // determina Redes vizinhas da Rede indicada
    lisEQP->Clear();
    rede->LisRedeVizinha(lisEQP);
    // verifica se alguma rede vizinha possui Dominio
    for (int n = 0; n < lisEQP->Count; n++)
    {
        rede_viz = (VTRede*)lisEQP->Items[n];
        if ((dominio = ExisteDominioRede(rede_viz)) != NULL)
            return (dominio);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalva::IniciaConexao(void)
{
    // vari�veis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

    try
    { // cancela conex�o atual
        bd->Desconecta();
        // verifica a base de dados selecionada
        switch (geral->BaseDado)
        {
        case SNPDBTYPE_ORACLE:
            bd->ConectaServidorRedeRemoto();
            break;
        case SNPDBTYPE_ACCESS:
            bd->ConectaServidorRedeLocal();
            break;
        default:
            return (false);
        }
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (bd->Conectado);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalva::InsereDominio(void)
{
    // vari�veis locais
    VTDominio *dominio;
    VTRede *rede;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisREDE = redes->LisRede();
    TList *lisDOMINIO = redes->LisDominio();

    try
    { // verifica se h� um Dominio definido
        if ((lisDOMINIO->Count == 0) && (lisREDE->Count > 0))
        { // cria um novo Dominio
            dominio = DLL_NewObjDominio();
            dominio->Codigo = "";
            dominio->DataCriacao = Now();
            // insere Dominio em Redes
            redes->InsereDominio(dominio);
            // insere todas as Redes no Dominio
            for (int n = 0; n < lisREDE->Count; n++)
            {
                rede = (VTRede*)lisREDE->Items[n];
                dominio->InsereRede(rede);
            }
        }
        // define Dominio para Rede sem Dominio
        for (int n = 0; n < lisREDE->Count; n++)
        {
            rede = (VTRede*)lisREDE->Items[n];
            if (!rede->Carregada)
                continue;
            if ((dominio = ExisteDominioRede(rede)) == NULL)
            { // associa Rede com um Dominio
                if (lisDOMINIO->Count == 1)
                {
                    dominio = (VTDominio*)lisDOMINIO->First();
                    dominio->InsereRede(rede);
                }
                else
                { // verifica se a Rede possui uma Rede vizinha com Dominio definido
                    if ((dominio = ExisteDominioRedeVizinha(rede)) == NULL)
                    { // assume �ltimo Dominio da lista
                        dominio = (VTDominio*)lisDOMINIO->Last();
                    }
                    dominio->InsereRede(rede);
                }
            }
        }
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalva::PM_GetProgresso(void)
{
    return (progresso != NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TSalva::PM_SetProgresso(bool enabled)
{
    if (enabled)
    {
        if (progresso == NULL)
        {
            apl->Add(progresso = DLL_NewObjProgresso(NULL, NULL));
        }
    }
    else
    { // destr�i objeto Progresso
        if (progresso != NULL)
        {
            apl->Remove(progresso);
            delete progresso;
            progresso = NULL;
        }
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TSalva::RetiraDominio(void)
{
    // vari�veis locais
    VTDominio *dominio;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList *lisDOMINIO = redes->LisDominio();

    try
    { // verifica se h� um Dominio definido
        while (lisDOMINIO->Count > 0)
        { // remove um Dominio e destr�i objeto
            dominio = (VTDominio*)lisDOMINIO->Last();
            redes->RemoveDominio(dominio);
            DLL_DeleteEqpto(dominio);
        }
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TSalva::BackupBaseDado(bool salva)
{
    // vari�veis locais
    bool FailIfExists = false;
    WideString arq_orig, arq_backup;
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

    // determina nome do arquivo da base atual
    arq_orig = path->ArqRede;
    // define nome do arquivo de Backup
    arq_backup = path->DirTmp() + "\\Backup_" + ExtractFileName(arq_orig);
    // verifica se deve salvar ou restaurar
    if (salva)
    { // cria c�pia de backup
        if (FileExists(arq_orig))
            CopyFile(arq_orig.c_bstr(), arq_backup.c_bstr(), FailIfExists);
    }
    else
    { // restaura c�pia de backup
        if (FileExists(arq_backup))
            CopyFile(arq_backup.c_bstr(), arq_orig.c_bstr(), FailIfExists);
    }
}
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TSalva::LicencaSinapViewer(void)
{
    // vari�veis locais
    VTLicenca *licenca;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

    // prote��o
    licenca = licencas->ExisteLicenca();
    if (licenca == NULL)
        return false;
    // retorna se a licen�a possui opera��o opVIEW
    for(int n = 0; n < licenca->Operacoes.Length; n++)
    {
        if(licenca->Operacoes[n] == opVIEW)
            return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TSalva::SalvaEstudo(VTEstudo *estudo)
{
    // vari�veis locais
    bool sucesso = false;

    if(LicencaSinapViewer())
    {//verifica se a licen�a � SINAPviewer
        try
        {//avisa
            Aviso("Somente figuras e coment�rios ser�o salvos na base de dados");
            if (estudo->Tipo == estudoREDE_ORIGINAL)
                return (false);
            // cria c�pia de Backup
            BackupBaseDado(true);
            // inicia conex�o com a base
            if (!bd->Conecta())
                return (false);
            // verifica se a base � da vers�o correta
            if (!bd->Eqpto->ValidaVersaoBaseParaGravacao())
            {
                AvisoBaseVersaoInvalida();
                // desconecta da base
                bd->Desconecta();
                return (false);
            }
            // prepara Estudo p/ salvar na base de dados
            estudo->PreparaParaSalvarNaBase();
            // salva Figuras
            sucesso = bd->Figura->SalvaFiguras(estudo);
            // desconecta da base
            bd->Desconecta();
            // verifica se teve sucesso na grava��o
            if (sucesso)
            { // atualiza estado do Estudo
                estudo->Novo = false;
                estudo->Alterado = false;
            }
            else
            { // restaura c�pia de Backup
                BackupBaseDado(false);
                // Erro("Erro na grava��o das redes do estudo");
            }
        }
        catch (...)
        { // desconecta da base
            bd->Desconecta();
            // restaura c�pia de Backup
            BackupBaseDado(false);
            sucesso = false;
        }
        return (sucesso);
    }
    try
    { // verifica se Estudo n�o � a Rede original
        reservado = false;
        if (progresso)
        {
            progresso->Start(progTEXTO);
            progresso->NoAutoInc();
            reservado = progresso->MaxStep(MAX_STEP_SALVA_ESTUDO);
            progresso->Add("Salvando Estudo");
        }

        if (estudo->Tipo == estudoREDE_ORIGINAL)
            return (false);
        // cria c�pia de Backup
        BackupBaseDado(true);
        // inicia conex�o com a base
        if (!bd->Conecta())
            return (false);
        // verifica se a base � da vers�o correta
        if (!bd->Eqpto->ValidaVersaoBaseParaGravacao())
        {
            AvisoBaseVersaoInvalida();
            return (false);
        }
        // prepara Estudo p/ salvar na base de dados
        estudo->PreparaParaSalvarNaBase();
        // IMPORTANTE:
        // salva tipos, arranjos e curvas t�picas que podem ter sido inclu�dos via bloco construtivo
        sucesso = bd->Geral->SalvaTipoChave();
        if (sucesso)
            sucesso = bd->Geral->SalvaTipoRede();
        // salva Arranjos novos ou alterados
        if (sucesso)
            sucesso = bd->Geral->SalvaArranjo();
        // salva Curvas t�picas novas ou alteradas
        if (sucesso)
            sucesso = bd->Geral->SalvaCurvaTipica();
        // verifica se Estudo � novo
        if (progresso)
            progresso->Step();

        if (sucesso && estudo->Novo)
        { // retira Dom�nio de Redes
            RetiraDominio();
            // Estudo novo: salva c�pia das Redes do Estudo
            sucesso = bd->Eqpto->SalvaRedeEstudo(estudo);
        }
        // grava Alternativas do Estudo na base de dados
        if (sucesso)
            sucesso = bd->Eqpto->SalvaAlternativaEstudo(estudo);
        // salva Figuras
        if (sucesso)
            sucesso = bd->Figura->SalvaFiguras(estudo);
        // desconecta da base
        bd->Desconecta();
        // verifica se teve sucesso na grava��o
        if (sucesso)
        { // atualiza estado do Estudo
            estudo->Novo = false;
            estudo->Alterado = false;
        }
        else
        { // restaura c�pia de Backup
            BackupBaseDado(false);
            // Erro("Erro na grava��o das redes do estudo");
        }
    }
    catch (Exception &e)
    { // desconecta da base
        bd->Desconecta();
        // restaura c�pia de Backup
        BackupBaseDado(false);
        sucesso = false;
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TSalva::SalvaEstudoComo(VTEstudo *estudo)
{
    if(LicencaSinapViewer())
    {//verifica se a licen�a � SINAPviewer
        Aviso("N�o � permitido utilizar o comando Salvar Como no SINAPviewer.");
        return false;
    }
    try
    { // define Estudo como novo
        estudo->Novo = true;
        SalvaEstudo(estudo);
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalva::SalvaHistoricoDensidadeCarga(VTArvore *arvore)
{
    // vari�veis locais
    bool sucesso;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    try
    { // inicia conex�o com base de dados
        if (!bd->ConectaBaseHist())
        {
            Aviso("A base de dados n�o est� acess�vel");
            return (false);
        }
        // grava Arvore, cojunto de redes e dados de perdas t�cnicas
        sucesso = bd->Hist->InsereArvore(arvore);
        if (sucesso)
            sucesso = bd->Hist->InsereRede(redes->LisRede());
        if (sucesso)
            sucesso = bd->Hist->InsereDensidadeCarga(arvore);
        // fecha conex�o com a base
        bd->Desconecta();
    }
    catch (Exception &e)
    {
        sucesso = false;
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TSalva::SalvaHistoricoPerdaTecnica(VTArvore *arvore)
{
    // vari�veis locais
    bool sucesso;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    try
    { // inicia conex�o com base de dados
        if (!bd->ConectaBaseHist())
        {
            Aviso("A base de dados n�o est� acess�vel");
            return (false);
        }
        // grava Arvore, cojunto de redes e dados de perdas t�cnicas
        sucesso = bd->Hist->InsereArvore(arvore);
        if (sucesso)
            sucesso = bd->Hist->InsereRede(redes->LisRede());
        if (sucesso)
            sucesso = bd->Hist->InserePerdaTecnica(arvore);
        // fecha conex�o com a base
        bd->Desconecta();
    }
    catch (Exception &e)
    {
        sucesso = false;
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TSalva::SalvaRede(void)
{
    // vari�veis locais
    bool sucesso;
    VTEstudo *estudo;
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

//	 if(LicencaSinapViewer())
//	 {//verifica se a licen�a � SINAPviewer
//		  try
//		  {//avisa
//            Aviso("Somente figuras e coment�rios ser�o salvos na base de dados");
//            // determina Estudo
//            if ((estudo = planejamento->ExisteEstudo()) == NULL)
//                return (false);
//            // verifica se o Estudo � da rede original
//            if (estudo->Tipo != estudoREDE_ORIGINAL)
//                return (false);
//            // cria c�pia de Backup
//            BackupBaseDado(true);
//            // inicia conex�o com base de dados
//            if (!bd->Conecta())
//            {
//                Aviso("A base de dados n�o est� acess�vel");
//                return (false);
//            }
//            // verifica se a base � da vers�o correta
//            if (!bd->Eqpto->ValidaVersaoBaseParaGravacao())
//            {
//                AvisoBaseVersaoInvalida();
//                return (false);
//            }
//            // salva Figuras
//            sucesso = bd->Figura->SalvaFiguras(estudo);
//            // desconecta da base
//            bd->Desconecta();
//            // define que o Estudo n�o � novo
//            if (sucesso)
//            {
//                estudo->Novo = false;
//            }
//            else
//            { // restaura c�pia de Backup
//                BackupBaseDado(false);
//            }
//        }
//        catch (...)
//        {
//            sucesso = false;
//            // desconecta da base
//            bd->Desconecta();
//            // restaura c�pia de Backup
//            BackupBaseDado(false);
//        }
//        return (sucesso);
//    }

    try
    { // determina Estudo
        if ((estudo = planejamento->ExisteEstudo()) == NULL)
            return (false);
        // verifica se o Estudo � da rede original
        if (estudo->Tipo != estudoREDE_ORIGINAL)
            return (false);
        // cria c�pia de Backup
        BackupBaseDado(true);
        // se necess�rio, cria um Dominio
        InsereDominio();
        // inicia conex�o com base de dados
        if (!bd->Conecta())
        {
            Aviso("A base de dados n�o est� acess�vel");
            return (false);
        }
        // verifica se a base � da vers�o correta
        if (!bd->Eqpto->ValidaVersaoBaseParaGravacao())
        {
            AvisoBaseVersaoInvalida();
            return (false);
        }
        // salva TipoConsumidor, TipoChave e TipoRede
        sucesso = bd->Geral->SalvaTipoConsumidor();
        if (sucesso)
            sucesso = bd->Geral->SalvaTipoChave();
        if (sucesso)
            sucesso = bd->Geral->SalvaTipoRede();
        // salva Arranjos novos ou alterados
        if (sucesso)
            sucesso = bd->Geral->SalvaArranjo();
        // salva Curvas t�picas novas ou alteradas
        if (sucesso)
            sucesso = bd->Geral->SalvaCurvaTipica();
        // salva Redes e demais Eqptos
        if (sucesso)
            sucesso = bd->Eqpto->SalvaRedeEstudo(estudo);
        // salva Figuras
        if (sucesso)
            sucesso = bd->Figura->SalvaFiguras(estudo);
        // desconecta da base
        bd->Desconecta();
        // define que o Estudo n�o � novo
        if (sucesso)
        {
            estudo->Novo = false;
        }
        else
        { // restaura c�pia de Backup
            BackupBaseDado(false);
        }
    }
    catch (Exception &e)
    {
        sucesso = false;
        // desconecta da base
        bd->Desconecta();
        // restaura c�pia de Backup
        BackupBaseDado(false);
    }
    return (sucesso);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16131085
bool __fastcall TSalva::SalvaRedeComo(void)
{
    // vari�veis locais
    bool sucesso;
    bool reinicia_status = true;
    int estudo_tipo, estudo_id;
    VTEstudo *estudo;
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
    VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

//    if(LicencaSinapViewer())
//    {//verifica se a licen�a � SINAPviewer
//        Aviso("N�o � permitido utilizar o comando Salvar Como no SINAPviewer.");
//        return false;
//    }
    try
    { // determina Estudo
        if ((estudo = planejamento->ExisteEstudo()) == NULL)
            return (false);
        // inicia conex�o com a nova base de dados
        if (!bd->Conecta())
        {
            Aviso("A base de dados n�o est� acess�vel");
            return (false);
        }
        // verifica se a base � da vers�o correta
        if (!bd->Eqpto->ValidaVersaoBaseParaGravacao())
        {
            AvisoBaseVersaoInvalida();
            return (false);
        }
        // salva o tipo do estudo e redefine como Rede original
        estudo_tipo = estudo->Tipo;
        estudo->Tipo = estudoREDE_ORIGINAL;
        // salva o ID do estudo e redefine estudoID_ZERO
        estudo_id = estudo->Id;
        estudo->Id = estudoID_ZERO;
        // define que o Estudo � novo
        estudo->Novo = true;
        // se necess�rio, cria um Dominio
        InsereDominio();
        // limpa Lixeira p/ evitar exclus�o da base de eqptos retirados
        edita->Clear();
        // salva TipoConsumidor, TipoChave e TipoRede
        sucesso = bd->Geral->SalvaTipoConsumidor(reinicia_status);
        if (sucesso)
            sucesso = bd->Geral->SalvaTipoChave(reinicia_status);
        if (sucesso)
            sucesso = bd->Geral->SalvaTipoRede(reinicia_status);
        // reinicia tabelas de Cabos, Suporte e Arranjos
        // if (sucesso) sucesso = bd->Geral->RetiraCaboSuporteArranjo();
        // salva Arranjos novos ou alterados
        if (sucesso)
            sucesso = bd->Geral->SalvaArranjo(reinicia_status);
        // salva Curvas t�picas novas ou alteradas
        if (sucesso)
            sucesso = bd->Geral->SalvaCurvaTipica(reinicia_status);
        // salva dados de Mercado
        if (sucesso)
            sucesso = bd->Geral->SalvaMercado();
        // salva Redes e demais Eqptos
        if (sucesso)
            sucesso = bd->Eqpto->SalvaRedeEstudoComo(estudo);
        // salva Figuras
        if (sucesso)
            sucesso = bd->Figura->SalvaFiguras(estudo);
        // define que o Estudo n�o � novo
        if (sucesso)
            estudo->Novo = false;
        // restaura tipo e ID do estudo
        estudo->Tipo = estudo_tipo;
        estudo->Id = estudo_id;
    }
    catch (Exception &e)
    {
        sucesso = false;
    }
    // desconecta da base
    bd->Desconecta();
    return (sucesso);
}

// ---------------------------------------------------------------------------
// eof
