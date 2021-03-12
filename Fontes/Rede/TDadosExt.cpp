// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDadosExt.h"
#include "..\Curvas\VTCurva.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTDadosExt
// ---------------------------------------------------------------------------
VTDadosExt* __fastcall NewObjDadosExt(void)
{
    return (new TDadosExt());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
__fastcall TDadosExt::TDadosExt(void)
{
    try
    {// inicia variaveis
        curvaDemanda = NULL;
        PD.cod_chave = "";
        PD.sigla = "";
        PD.regional = "";
		PD.id_alim_pai = -1;
		PD.mae_filha_conectada = false;
    }
    catch (Exception &e)
    {
    }
}

// ---------------------------------------------------------------------------
__fastcall TDadosExt::~TDadosExt(void)
{
    // destrói curva
    if (curvaDemanda != NULL)
    {
        delete curvaDemanda;
        curvaDemanda = NULL;
    }
}

// ---------------------------------------------------------------------------
VTDadosExt* __fastcall TDadosExt::Clone(void)
{
    // variáveis locais
    VTDadosExt *dados_ext;

    // cria um novo objeto
    if ((dados_ext = NewObjDadosExt()) != NULL)
    { // copia dados deste objeto para o novo
        dados_ext->CopiaAtributosDe(*this);
    }
    return (dados_ext);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16968316
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
void __fastcall TDadosExt::CopiaAtributosDe(VTDadosExt &ref)
{
    // variáveis locais
    TDadosExt &dados_ext = (TDadosExt&)ref;

    // copia
    this->IdAlimPai = ref.IdAlimPai;
    this->CodChave = ref.CodChave;
    this->Sigla = ref.Sigla;
    this->Regional = ref.Regional;
    this->MaeFilhaConectada = ref.MaeFilhaConectada;
    // destrói Curva própria
    if (this->curvaDemanda)
        delete this->curvaDemanda;
    // cria cópia de Curva própria
    if(ref.CurvaDemanda != NULL)
        this->curvaDemanda = ref.CurvaDemanda->Clone();
}

// ---------------------------------------------------------------------------
void __fastcall TDadosExt::DefineCurva(VTCurva *curva)
{
    // salva curva
    curvaDemanda = curva;
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TDadosExt::PM_GetCurva(void)
{
    return (curvaDemanda);
}

// ---------------------------------------------------------------------------
