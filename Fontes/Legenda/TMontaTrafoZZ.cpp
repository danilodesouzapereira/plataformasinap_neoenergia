// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaTrafoZZ.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18686973
VTMonta* __fastcall NewObjMontaTrafoZZ(VTApl *apl)
{
    try
    {
        return (new TMontaTrafoZZ(apl));
    }
    catch (Exception &e)
    { // nada a fazer
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TMontaTrafoZZ::TMontaTrafoZZ(VTApl *apl)
{
    // salva ponteiro p/ objeto
    this->apl = apl;
}

// ---------------------------------------------------------------------------
__fastcall TMontaTrafoZZ::~TMontaTrafoZZ(void)
{
    // nada a fazer
}

// ---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaTrafoZZ::IniciaGrupo(VTEqpto *eqpto)
{
    // variáveis locais
    int ano = 0;
    int num_pat = 0;
    VTGrupo *grupo1, *grupo2;

    // salva ponteiro p/ trafoZZ
    trafoZZ = (VTTrafoZZ*)eqpto;
    // cria Grupo básico
    grupo1 = NewObjGrupo(eqpto, eqpto->TipoAsString() + ": " + eqpto->Codigo);
    if (grupo1 == NULL)
        return (NULL);
    // cria Grupo2 c/ dados do trafoZZ
    if ((grupo2 = NewObjGrupo(eqpto, "Cadastro")) != NULL)
    { // insere Grupo2 no Grupo1
        grupo1->Add(grupo2);
        // inicia Grupo2 c/ dados do Eqpto
        LisValoresEqpto(eqpto, grupo2->LisDado, ano, num_pat);
    }
    // cria Grupo2 c/ dados de Fluxo de Potência
//    if ((grupo2 = NewObjGrupo(eqpto, "Fluxo de Potência")) != NULL)
//    {//insere Grupo2 no Grupo1
//        grupo1->Add(grupo2);
//        //inicia Grupo2 c/ dados da curva
//        LisVetValFluxo(eqpto, grupo2->LisVetval, trzz_CARGTO, ano);
//    }
    return (grupo1);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisAtributos(VTEqpto *eqpto, TList *lisATR)
{
    // salva ponteiro p/ trafoZZ
    trafoZZ = (VTTrafoZZ*)eqpto;
    // atributos de Eqpto
    TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
    LisAtributosEqpto(eqpto, lisATR);
    // atributos de fluxo
    LisAtributosFluxo(eqpto, lisATR);

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
{
    // salva ponteiro p/ trafoZZ
    trafoZZ = (VTTrafoZZ*)eqpto;
    // atributos de Eqbar
    TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
    // atributos de trafoZZ
    lisATR->Add(new TAtributo(trafoZZ, trzz_S_MVA, true, true, "Pot.Nominal", "(MVA)"));
    lisATR->Add(new TAtributo(trafoZZ, trzz_V_NOM, true, true, "Tensão Nominal", "(kV)"));

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
{
    // salva ponteiro p/ trafoZZ
//    trafoZZ = (VTTrafoZZ*)eqpto;
//    // atributos de fluxo de Eqbar
//    TMontaEqbar::LisAtributosFluxo(eqpto, lisATR);
//    // atributos de fluxo de trafoZZ
//    lisATR->Add(new TAtributo(trafoZZ, trzz_CARGTO,   false, true,  "Carregamento", "(%)"));

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
{
    // monta lista de valores do Eqpto
    LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
    // monta lista de valores da Curva
    // LisValoresCurva(eqpto, lisEXT, ano, num_pat);
    // monta lista de valores de Fluxo
    LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
    return (lisEXT->Count > 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
{
    // salva parâmetros
    // this->capacitor = (VTCapacitor*)eqpto;
    // try{//dados da Curva
    // if (capacitor->Show[cap_Q]) ValorPotenciaReativa();
    // }catch(Exception &e)
    // {
    // }
    // return(lisEXT->Count > 0);
    return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
{
    // salva parâmetros
    this->trafoZZ = (VTTrafoZZ*)eqpto;
    try
    { // dados do Eqpto
        TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
        // dados do trafoZZ
        if (trafoZZ->Show[trzz_S_MVA])   ValorPotenciaNominal();
        if (trafoZZ->Show[trzz_V_NOM])   ValorTensaoNominal();
    }
    catch (Exception &e)
    {
    }
    return (lisEXT->Count > 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
{
    // não há dados de fluxo
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
{
    // salva parâmetros
//     this->trafoZZ = (VTTrafoZZ*)eqpto;
//     this->lisEXT    = lisEXT;
//     this->ano       = ano;
//     //verifica atributo selecionado
//     switch(ind_atrib)
//     {
//        case trzz_S_MVA: VetValorPotenciaNominal(ind_atrib);  break;
//        case trzz_V_NOM: VetValorTensaoNominal(ind_atrib);  break;
//     }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaTrafoZZ::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
{
    // salva parâmetros
//     this->trafoZZ = (VTTrafoZZ*)eqpto;
//     this->lisEXT    = lisEXT;
//     this->ano       = ano;
//     //verifica atributo selecionado
//     switch(ind_atrib)
//     {
//        case trzz_CARGTO: VetValorFluxoCargto(ind_atrib);  break;
//     }
    return (true);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TMontaTrafoZZ::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
{
    // variáveis locais
    AnsiString valor, unidade;
    VTTrafoZZ *trafoZZ = (VTTrafoZZ*)eqpto;

    // determina valor de atributo
    switch (ind_atrib)
    {
    case eqp_ID:
        valor = IntToStr(eqpto->Id);
        break;
    case eqp_CODIGO:
        valor = eqpto->Codigo;
        break;
    case eqb_BARRA:
        valor = trafoZZ->pbarra->Codigo;
        break;
    case trzz_S_MVA:
        valor = DoubleToStr("%5.4f", trafoZZ->snom);
        break;
    case trzz_V_NOM:
        valor = DoubleToStr("%5.4f", trafoZZ->vnom);
        break;
    }
    return (valor);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaTrafoZZ::ValorPotenciaNominal(void)
{
    // variáveis locais
     VTDado *dado;

     //cria um novo Dado e insere em lisEXT
     lisEXT->Add(dado = NewObjDado("Snom"));
     //define unidade e valor
     aue.PotAparente_MVA(trafoZZ->snom, dado->valor, dado->unidade);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaTrafoZZ::ValorTensaoNominal(void)
{
    // variáveis locais
     VTDado *dado;

     //cria um novo Dado e insere em lisEXT
     lisEXT->Add(dado = NewObjDado("Vnom"));
     //define unidade e valor
     aue.Tensao_KV(trafoZZ->vnom, dado->valor, dado->unidade);
}

//---------------------------------------------------------------------------
void __fastcall TMontaTrafoZZ::VetValorFluxoCargto(int ind_atrib)
   {
   //variáveis locais
//   VTVetval *vetval;
//   double   cargto;
//
//   //cria um novo Vetval e insere em lisEXT
//   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Cargto(%)", trafoZZ->resflow->Max_pat));
//   //preenche valores
//   for (int np = 0; np < trafoZZ->resflow->Max_pat; np++)
//      {
//      vetval->Valor[np] = Abs(trafoZZ->resflow->Cargto[np]);
//      }
   }

// ---------------------------------------------------------------------------
// eof
