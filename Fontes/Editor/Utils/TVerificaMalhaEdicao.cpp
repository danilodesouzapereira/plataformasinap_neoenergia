// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TVerificaMalhaEdicao.h"
// ---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
#include "..\Alerta\VTAlerta.h"
#include "..\Radial\VTMalha.h"
#include "..\Radial\VTMalhas.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTTrafo.h"

#include "..\..\DLL_Inc\Radial.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TVerificaMalhaEdicao::TVerificaMalhaEdicao(VTApl *apl)
{
    this->apl = apl;
    malhas = NULL;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17616225
__fastcall TVerificaMalhaEdicao::~TVerificaMalhaEdicao(void)
{
    //destroi obj anterior
    if(malhas != NULL)
    {
        delete malhas;
        malhas = NULL;
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16233879
// https://sinapsisenergia.teamwork.com/#tasks/16818904
// https://sinapsisenergia.teamwork.com/#/tasks/17616225
bool __fastcall TVerificaMalhaEdicao::VerificaSeFechaMalhaExterna(VTLigacao *lig)
{
    VTBarra *barra;
    VTRede *rede;
    VTLigacao *ligacao;
    VTRede *outraRede;
    VTChave *chave;
    VTTrafo *trafo;
    TList *lisLIG;
    bool fechouMalha = false;

    try
    {
        //destroi obj anterior
        if(malhas != NULL)
        {
            delete malhas;
            malhas = NULL;
        }
        //proteção
        if ((lig == NULL) || (lig->rede == NULL))
        {
            return false;
        }
        rede = lig->rede;
        // se for uma chave aberta faz nada
        if (lig->Tipo() == eqptoCHAVE)
        {
            chave = (VTChave*)lig;
            if (chave->Estado == chvABERTA)
            {
                return false;
            }
        }
        // verifica todas barras
        for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
        {
            if ((barra = lig->Barra(ind_bar)) == NULL)
            {
                continue;
            }
            // verifica se a barra possui ligacoes q sejam de outra rede
            lisLIG = barra->LisLigacao();
            for (int nl = 0; nl < lisLIG->Count; nl++)
            {
                ligacao = (VTLigacao*)lisLIG->Items[nl];
                // verifica se é a mesma
                if (lig == ligacao)
                {
                    continue;
                }
                // verifica se a ligacao é de outra rede
                outraRede = ligacao->rede;
                if (outraRede == rede)
                {
                    continue;
                }
                // se a outra ligacao for uma chave aberta, ignora
                if (ligacao->Tipo() == eqptoCHAVE)
                {
                    chave = (VTChave*)ligacao;
                    if (chave->Estado == chvABERTA)
                    {
                        continue;
                    }
                }

                //executa malha
                if(FechouMalhaExterna(ligacao))
                {
                    fechouMalha = true;
                    return fechouMalha;
                }

/*
                // verifica se a barra analisada é inicial de uma das 2
                if ((rede->Radial) && (barra != rede->BarraInicial()))
                {
                    fechouMalha = true;
                }
                if ((outraRede->Radial) && (barra != outraRede->BarraInicial()))
                {
                    fechouMalha = true;
                }
*/
            }
        }
    }
    catch (...)
    {
        return false;
    }
    return fechouMalha;
}

// ---------------------------------------------------------------------------
bool __fastcall TVerificaMalhaEdicao::ExisteAlertaMalha(VTRede* rede)
{
    VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));
    AnsiString aviso = "";

    aviso = alerta->ExisteAvisoMalha(rede);

    return (!aviso.IsEmpty());
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17616225
bool __fastcall TVerificaMalhaEdicao::FechouMalhaExterna(VTLigacao *lig)
{
    //variaveis locais
    TList *lisMALHA_EXT, *lisLIG;
    VTMalha *malha;

    //cria objeto, se ainda nao existir, e roda
    if(malhas == NULL)
    {
        malhas = DLL_NewObjMalhas(apl);
        malhas->Inicia();
    }
    //verifica se alguma malha contém a ligação
    lisMALHA_EXT = malhas->LisMalhaExterna();
    for(int n = 0; n < lisMALHA_EXT->Count; n++)
    {
        malha = (VTMalha*)lisMALHA_EXT->Items[n];
        lisLIG = malha->LisLigacao();
        if(lisLIG->IndexOf(lig) >= 0)
            return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
