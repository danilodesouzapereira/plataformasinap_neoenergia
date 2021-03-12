// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TInsereTrafoZZ.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafoZZ.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Rede.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TInsereTrafoZZ::TInsereTrafoZZ(VTApl *apl)
{
    try
    { // salva ponteiro
        this->apl = apl;
    }
    catch (Exception &e)
    {
    }
}

// ---------------------------------------------------------------------------
__fastcall TInsereTrafoZZ::~TInsereTrafoZZ(void)
{
    // nada
}

// -----------------------------------------------------------------------------
bool __fastcall TInsereTrafoZZ::Executa(TList *lisTRAFO_SED)
{
    // variáveis locais
    VTBarra *barra;
    VTTrafo *trafo;

    try
    {//percorre todos os trafos de subestação
        for (int n = 0; n < lisTRAFO_SED->Count; n++)
        {
            trafo = (VTTrafo*)lisTRAFO_SED->Items[n];
            //verifica se possui ligação delta no secundário
            if (trafo->sec.ligacao != lgTRIANG)
                continue;
            barra = trafo->BarraSecundario();
            //se nao tiver trafo zz, insere
            if (!ExisteTrafoZZ(barra))
                InsereZZ(barra);
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

// -----------------------------------------------------------------------------
bool __fastcall TInsereTrafoZZ::ExisteTrafoZZ(VTBarra *barra)
{
    // variáves locais
    VTEqbar *eqbar;
    TList *lisEQBAR = barra->LisEqbar();

    // verifica se tem trafo zz na barra
    for (int n = 0; n < lisEQBAR->Count; n++)
    {
        eqbar = (VTEqbar*)lisEQBAR->Items[n];
        if(eqbar->Tipo() == eqptoTRAFOZZ)
            return (true);
    }
    return (false);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18687127
void __fastcall TInsereTrafoZZ::InsereZZ(VTBarra *barra)
{
    //variaveis locais
    VTTrafoZZ *trafozz;

    try
    {//cria objeto
        trafozz = DLL_NewObjTrafoZZ();
        trafozz->Id = FalsoId();
        trafozz->pbarra = barra;
        trafozz->Codigo = "FICTICIO";
        trafozz->Extern_id = "";
        trafozz->vnom = barra->vnom;
        trafozz->snom = 50; // MVA
        trafozz->z0.r = 0.;
        trafozz->z0.x = 0.05;
        trafozz->Show[eqp_VISIBLE] = true;
        trafozz->Show[eqp_FIXO] = false;
        // define parâmetros para esquemático
        trafozz->esq.dxbar = 200; // sdi.esq;
        trafozz->esq.dybar = 200;
        barra->InsereEqbar(trafozz);
    }
    catch(...)
    {
        return;
    }
}

// -----------------------------------------------------------------------------
// eof
