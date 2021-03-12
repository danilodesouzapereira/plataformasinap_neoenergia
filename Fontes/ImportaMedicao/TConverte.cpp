// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TConverte.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConverte::TConverte(VTApl *apl)
{
    // salva ponteiro p/ objeto
    this->apl = apl;
    // cria lista
    lisEQP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TConverte::~TConverte(void)
{
    // destrói lista sem destruir seus objetos
    if (lisEQP)
    {
        delete lisEQP;
        lisEQP = NULL;
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CalculaDemandaFluxoPorFase(void)
{
    // proteção: verifica se está associado a uma chave (que tem o resflowlig)
    if (!medicao)
        return (false);
    if (!medicao->chave_med)
        return (false);
    // guarda ligacao inicial (que tem o resflow)
    liga_inicial = (VTLigacao*)medicao->chave_med;
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CalculaDemandaMedicao1Fase(int ind_pat)
{
    // variáveis locais
    double i_amp[MAX_FASE];
    strHM hm_ini, hm_fim;

    // limpa vetor
    for (int n = 0; n < MAX_FASE; n++)
        i_amp[n] = 0.;
    // determina ponto da Curva de medição relativo ao patamar indicado
    // ind_pat = 0;
    if (!curva_med_original->GetPonto(ind_pat, hm_ini, hm_fim, i_amp, MAX_FASE))
        return (false);
    // calcula demanda medicao, usando corrente medicao e tensão de fase
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
    {
        s_med_mva[ind_fase] = 0.;
        if (IsDoubleZero(i_amp[ind_fase]))
            continue;
        s_med_mva[ind_fase] = (vfn_kv[ind_fase] * i_amp[ind_fase]) / 1000.; // kVA => MVA
    }
    // define demanda nula no neutro
    s_med_mva[indNEUTRO] = 0.;

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CalculaDemandaMedicaoPorFase(VTPatamar *patamar, TList *lisI)
{ /*
     //variáveis locais
     double      i_amp[MAX_FASE];
     VTCanal     *canal;

     //limpa vetor
     for(int n = 0; n < MAX_FASE; n++) i_amp[n] = 0.;
     //determina ponto da Curva de medição relativo ao patamar indicado
     for(int nc = 0; nc < lisI->Count; nc++)
     {//determina valores no patamar
     canal = (VTCanal*)lisI->Items[nc];
     switch(canal->Fases)
     {
     case faseA: i_amp[indFASE_A] = canal->ValorMedido[patamar]; break;
     case faseB: i_amp[indFASE_B] = canal->ValorMedido[patamar]; break;
     case faseC: i_amp[indFASE_C] = canal->ValorMedido[patamar]; break;
     default: break;
     }
     }
     //calcula demanda medicao, usando corrente medicao e tensão de fase
     for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
     {
     s_med_mva[ind_fase] = (vfn_kv[ind_fase] * i_amp[ind_fase]) / 1000.; //kVA => MVA
     }
     //define demanda nula no neutro
     s_med_mva[indNEUTRO] = 0.;
     */
    return (true);
}

// ---------------------------------------------------------------------------
double __fastcall TConverte::CalculaEnergiaDiariaFluxo(void)
{
    // variáveis locais
    double e_dia_mwh = 0.0;
    int nb = 0;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    { // determina sentido
        if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
            nb = 1;
        // soma potencia ativa em todos os patamares
        e_dia_mwh += liga_inicial->resflow->P_mw[nb][ind_pat];
    }

    return (e_dia_mwh);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CalculaFatorCorrecaoIfase(double fatcor[MAX_FASE], VTPatamar *patamar,
    TList *lisI)
{
    // variáveis locais
    VTCanal *canal_i;
    double i_flow_a, i_med_a;
    int nb, ind_pat;

    // proteção
    if (lisI->Count == 0)
        return (false);
    if (patamar == NULL)
        return (false);
    ind_pat = patamar->Index;
    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // loop em todos canais
    for (int nc = 0; nc < lisI->Count; nc++)
    {
        canal_i = (VTCanal*)lisI->Items[nc];
        // determina valor medido
        i_med_a = canal_i->ValorMedido[patamar];
        // determina valores do flow e fatcor
        switch (canal_i->Fases)
        {
        case faseA:
            i_flow_a = abs(liga_inicial->resflow->Ia_amp[nb][ind_pat]);
            if (IsDoubleZero(i_flow_a))
                fatcor[indFASE_A] = 1;
            else
                fatcor[indFASE_A] = i_med_a / i_flow_a;
            break;
        case faseB:
            i_flow_a = abs(liga_inicial->resflow->Ib_amp[nb][ind_pat]);
            if (IsDoubleZero(i_flow_a))
                fatcor[indFASE_B] = 1;
            else
                fatcor[indFASE_B] = i_med_a / i_flow_a;
            break;
        case faseC:
            i_flow_a = abs(liga_inicial->resflow->Ic_amp[nb][ind_pat]);
            if (IsDoubleZero(i_flow_a))
                fatcor[indFASE_C] = 1;
            else
                fatcor[indFASE_C] = i_med_a / i_flow_a;
            break;
        default:
            break;
        }
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TConverte::CalculaFatorCorrecaoP3f(int ind_pat, double p_mw)
{
    // variáveis locais
    double fat_cor = 0.0;
    double p_flow_mw = 0;
    int nb;

    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina valor do fluxo
    p_flow_mw = liga_inicial->resflow->P_mw[nb][ind_pat];
    // calcula fator
    if (IsDoubleZero(p_flow_mw))
        fat_cor = 0.0;
    else
        fat_cor = p_mw / p_flow_mw;

    return (fat_cor);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TConverte::CalculaFatorCorrecaoPfase(int ind_fase, int ind_pat, double p_mw)
{
    // variáveis locais
    double fat_cor = 0.0;
    double p_flow_mw = 0;
    int nb;

    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina valor do fluxo
    switch (ind_fase)
    {
    case indFASE_A:
        p_flow_mw = liga_inicial->resflow->Pa_mw[nb][ind_pat];
        break;
    case indFASE_B:
        p_flow_mw = liga_inicial->resflow->Pb_mw[nb][ind_pat];
        break;
    case indFASE_C:
        p_flow_mw = liga_inicial->resflow->Pc_mw[nb][ind_pat];
        break;
    default:
        break;
    }
    // calcula fator
    if (IsDoubleZero(p_flow_mw))
        fat_cor = 0.0;
    else
        fat_cor = p_mw / p_flow_mw;

    return (fat_cor);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TConverte::CalculaFatorCorrecaoQ3f(int ind_pat, double q_mvar)
{
    // variáveis locais
    double fat_cor = 0.0;
    double q_flow_mvar = 0;
    int nb;

    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina valor do fluxo
    q_flow_mvar = liga_inicial->resflow->Q_mvar[nb][ind_pat];
    // calcula fator
    if (IsDoubleZero(q_flow_mvar))
        fat_cor = 0.0;
    else
        fat_cor = q_mvar / q_flow_mvar;

    return (fat_cor);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TConverte::CalculaFatorCorrecaoQfase(int ind_fase, int ind_pat, double q_mvar)
{
    // variáveis locais
    double fat_cor = 0.0;
    double q_flow_mvar = 0;
    int nb;

    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina valor do fluxo
    switch (ind_fase)
    {
    case indFASE_A:
        q_flow_mvar = liga_inicial->resflow->Qa_mvar[nb][ind_pat];
        break;
    case indFASE_B:
        q_flow_mvar = liga_inicial->resflow->Qb_mvar[nb][ind_pat];
        break;
    case indFASE_C:
        q_flow_mvar = liga_inicial->resflow->Qc_mvar[nb][ind_pat];
        break;
    default:
        break;
    }
    // calcula fator
    if (IsDoubleZero(q_flow_mvar))
        fat_cor = 0.0;
    else
        fat_cor = q_mvar / q_flow_mvar;

    return (fat_cor);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TConverte::CalculaFatorCorrecaoS3f(int ind_pat, double s_mva)
{
    // variáveis locais
    double fat_cor = 0.0;
    double s_flow_mva = 0;
    int nb;

    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina valor do fluxo, em módulo
    s_flow_mva = Abs(liga_inicial->resflow->S_mva[nb][ind_pat]);
    // calcula fator
    if (IsDoubleZero(s_flow_mva))
        fat_cor = 0.0;
    else
        fat_cor = s_mva / s_flow_mva;

    return (fat_cor);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CalculaFatorCorrecaoSfase(double fatcor_s[MAX_FASE], int ind_pat)
{
    // variáveis locais
    double fat_nao_nulo = 0.;
    double s_fase_mva[MAX_FASE];
    int nb;

    // limpa vetor
    for (int n = 0; n < MAX_FASE; n++)
        s_fase_mva[n] = 0.;
    // determina sentido
    nb = 0;
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
    { // determina valor do fluxo por fase
        switch (ind_fase)
        {
        case indFASE_A:
            s_fase_mva[ind_fase] = Abs(liga_inicial->resflow->Sa_mva[nb][ind_pat]);
            break;
        case indFASE_B:
            s_fase_mva[ind_fase] = Abs(liga_inicial->resflow->Sb_mva[nb][ind_pat]);
            break;
        case indFASE_C:
            s_fase_mva[ind_fase] = Abs(liga_inicial->resflow->Sc_mva[nb][ind_pat]);
            break;
        default:
            break;
        }
        // calcula fator
        if (IsDoubleZero(s_fase_mva[ind_fase]))
            fatcor_s[ind_fase] = 0.0;
        else
            fatcor_s[ind_fase] = s_med_mva[ind_fase] / s_fase_mva[ind_fase];
        // guarda um valor não nulo
        if (!IsDoubleZero(fatcor_s[ind_fase], 1e-2))
            fat_nao_nulo = fatcor_s[ind_fase];
    }
    // verifica as fases da medição, para adequar às fases da rede
    if (medicao->FasesMed(ind_pat) != medicao->FasesRede(ind_pat))
    { // assume um fator de correção não nulo de uma das fases
        if (IsDoubleZero(fat_nao_nulo))
            return (false);
        for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
        {
            if (IsDoubleZero(fatcor_s[ind_fase]))
                fatcor_s[ind_fase] = fat_nao_nulo;
        }
    }
    return (true);
}

/*
 //---------------------------------------------------------------------------
 double __fastcall  TConverte::CalculaFatorCorrecaoSfase(VTCanal *canal_i, int ind_pat)
 {
 //variaveis locais
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTPatamar   *patamar;
 VTLigacao   *ligacao;
 double fat_cor, s_med_mva, s_fase_mva, i_amp, vfn_kv;
 int    nb;

 //determina patamar
 patamar = patamares->Patamar[ind_pat];
 //inicia variaveis
 fat_cor = s_med_mva = s_fase_mva = i_amp = vfn_kv = 0.;
 //determina ligacao
 ligacao = (VTLigacao*)canal_i->Eqpto;
 //determina ponto da Curva de medição relativo ao patamar indicado
 i_amp = canal_i->ValorMedido[patamar];
 //determina sentido
 nb = 0;
 if(ligacao->resflow->SentidoP[nb][ind_pat] == fluxoNEG) nb = 1;
 //determina potencia aparente por fase e tensão do flow
 switch(canal_i->Fases)
 {
 case faseA:
 vfn_kv     = abs(canal_i->Medidor->Barra->resflow->Van_kv[ind_pat]);
 s_fase_mva = abs(ligacao->resflow->Sa_mva[nb][ind_pat]);
 break;
 case faseB:
 vfn_kv     = abs(canal_i->Medidor->Barra->resflow->Vbn_kv[ind_pat]);
 s_fase_mva = abs(ligacao->resflow->Sb_mva[nb][ind_pat]);
 break;
 case faseC:
 vfn_kv     = abs(canal_i->Medidor->Barra->resflow->Vcn_kv[ind_pat]);
 s_fase_mva = abs(ligacao->resflow->Sc_mva[nb][ind_pat]);
 break;
 default: break;
 }
 //calcula demanda medicao, usando corrente medicao e tensão de fase
 s_med_mva = (vfn_kv * i_amp) / 1000.; //kVA => MVA
 //calcula fator
 if(IsDoubleZero(s_fase_mva)) fat_cor = 0.0;
 else fat_cor = s_med_mva / s_fase_mva;

 return(fat_cor);
 }
 */
// ---------------------------------------------------------------------------
double __fastcall TConverte::CalculaFatorCorrecaoSfase(VTCanal *canal_i, int ind_pat)
{
    // variaveis locais
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
    VTPatamar *patamar;
    VTLigacao *ligacao;
    double fat_cor, i_med_a, i_flow_a;
    int nb;

    // determina patamar
    patamar = patamares->Patamar[ind_pat];
    // inicia variaveis
    fat_cor = i_med_a = i_flow_a = 0.;
    // determina ligacao
    ligacao = (VTLigacao*)canal_i->Eqpto;
    // determina ponto da Curva de medição relativo ao patamar indicado
    i_med_a = canal_i->ValorMedido[patamar];
    // determina sentido
    nb = 0;
    if (ligacao->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina valores do flow
    switch (canal_i->Fases)
    {
    case faseA:
        i_flow_a = abs(ligacao->resflow->Ia_amp[nb][ind_pat]);
        break;
    case faseB:
        i_flow_a = abs(ligacao->resflow->Ib_amp[nb][ind_pat]);
        break;
    case faseC:
        i_flow_a = abs(ligacao->resflow->Ic_amp[nb][ind_pat]);
        break;
    default:
        break;
    }
    // calcula fator
    if (IsDoubleZero(i_flow_a))
        fat_cor = 0.0;
    else
        fat_cor = i_med_a / i_flow_a;

    return (fat_cor);
}

// ---------------------------------------------------------------------------
double __fastcall TConverte::CalculaFluxoP(int ind_fase, int ind_pat)
{
    // variáveis locais
    int nb = 0;

    // determina sentido
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina fase
    switch (ind_fase)
    {
    case indFASE_A:
        return (liga_inicial->resflow->Pa_mw[nb][ind_pat]);
    case indFASE_B:
        return (liga_inicial->resflow->Pb_mw[nb][ind_pat]);
    case indFASE_C:
        return (liga_inicial->resflow->Pc_mw[nb][ind_pat]);
    default:
        break;
    }
    return (0.0);
}

// ---------------------------------------------------------------------------
double __fastcall TConverte::CalculaFluxoQ(int ind_fase, int ind_pat)
{
    // variáveis locais
    int nb = 0;

    // determina sentido
    if (liga_inicial->resflow->SentidoP[nb][ind_pat] == fluxoNEG)
        nb = 1;
    // determina fase
    switch (ind_fase)
    {
    case indFASE_A:
        return (liga_inicial->resflow->Qa_mvar[nb][ind_pat]);
    case indFASE_B:
        return (liga_inicial->resflow->Qb_mvar[nb][ind_pat]);
    case indFASE_C:
        return (liga_inicial->resflow->Qc_mvar[nb][ind_pat]);
    default:
        break;
    }
    return (0.0);
}

/*
 //---------------------------------------------------------------------------
 double __fastcall  TConverte::CalculaFluxoP(VTCanal *canal_i, int ind_pat)
 {
 //variáveis locais
 int         nb = 0;
 VTLigacao   *ligacao;

 //determina ligacao
 ligacao = (VTLigacao*)canal_i->Eqpto;
 //determina sentido
 if(ligacao->resflow->SentidoP[nb][ind_pat] == fluxoNEG) nb = 1;
 //determina fase
 switch(canal_i->Fases)
 {
 case faseA:  return(ligacao->resflow->Pa_mw[nb][ind_pat]);
 case faseB:  return(ligacao->resflow->Pb_mw[nb][ind_pat]);
 case faseC:  return(ligacao->resflow->Pc_mw[nb][ind_pat]);
 default: break;
 }
 return(0.0);
 }
 */
// ---------------------------------------------------------------------------
double __fastcall TConverte::CalculaFluxoP(VTCanal *canal_i, int ind_pat)
{
    // variáveis locais
    int nb = 0;
    VTLigacao *ligacao;
    VTBarra *bar_ref;

    // determina ligacao
    ligacao = (VTLigacao*)canal_i->Eqpto;
    // determina Barra de referência: Barra do Medidor
    bar_ref = canal_i->Medidor->Barra;
    // determina índice da Barra de refência na Ligacao
    nb = ligacao->IndiceBarra(bar_ref);
    // determina sentido
    // if(ligacao->resflow->SentidoP[nb][ind_pat] == fluxoNEG) nb = 1;
    // determina fase
    switch (canal_i->Fases)
    {
    case faseA:
        return (ligacao->resflow->Pa_mw[nb][ind_pat]);
    case faseB:
        return (ligacao->resflow->Pb_mw[nb][ind_pat]);
    case faseC:
        return (ligacao->resflow->Pc_mw[nb][ind_pat]);
    default:
        break;
    }
    return (0.0);
}

/*
 //---------------------------------------------------------------------------
 double __fastcall  TConverte::CalculaFluxoQ(VTCanal *canal_i, int ind_pat)
 {
 //variáveis locais
 int         nb = 0;
 VTLigacao   *ligacao;

 //determina ligacao
 ligacao = (VTLigacao*)canal_i->Eqpto;
 //determina sentido
 if(ligacao->resflow->SentidoP[nb][ind_pat] == fluxoNEG) nb = 1;
 //determina fase
 switch(canal_i->Fases)
 {
 case faseA:  return(ligacao->resflow->Qa_mvar[nb][ind_pat]);
 case faseB:  return(ligacao->resflow->Qb_mvar[nb][ind_pat]);
 case faseC:  return(ligacao->resflow->Qc_mvar[nb][ind_pat]);
 default: break;
 }
 return(0.0);
 }
 */
// ---------------------------------------------------------------------------
double __fastcall TConverte::CalculaFluxoQ(VTCanal *canal_i, int ind_pat)
{
    // variáveis locais
    int nb = 0;
    VTLigacao *ligacao;
    VTBarra *bar_ref;

    // determina ligacao
    ligacao = (VTLigacao*)canal_i->Eqpto;
    // determina Barra de referência: Barra do Medidor
    bar_ref = canal_i->Medidor->Barra;
    // determina índice da Barra de refência na Ligacao
    nb = ligacao->IndiceBarra(bar_ref);
    // determina sentido
    // if(ligacao->resflow->SentidoP[nb][ind_pat] == fluxoNEG) nb = 1;
    // determina fase
    switch (canal_i->Fases)
    {
    case faseA:
        return (ligacao->resflow->Qa_mvar[nb][ind_pat]);
    case faseB:
        return (ligacao->resflow->Qb_mvar[nb][ind_pat]);
    case faseC:
        return (ligacao->resflow->Qc_mvar[nb][ind_pat]);
    default:
        break;
    }
    return (0.0);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanaisIparaCanaisPQfase(VTMedidor *medidor)
{
    // variaveis locais
    VTCanal *canal, *canal_p, *canal_q;
    TList *lisCANAL;

    // proteção
    if (!medidor)
        return (false);
    lisCANAL = medidor->LisCanal();
    for (int nc = 0; nc < lisCANAL->Count; nc++)
    {
        canal = (VTCanal*)lisCANAL->Items[nc];
        if (canal->TipoCanal != canalI)
            continue;
        // verifica se já existe canal P do mesmo eqpto e na mesma fase
        canal_p = medidor->ExisteCanal(canal->Eqpto, canalP, canal->Fases);
        if (!canal_p)
        {
            canal_p = medidor->InsereCanal(canal->Eqpto, canalP, canal->Fases);
            canal_p->Tolerancia_perc = canal->Tolerancia_perc;
        }
        // converte curva I para P
        CorrenteToP(canal, canal_p);
        // verifica se já existe canal Q do mesmo eqpto e na mesma fase
        canal_q = medidor->ExisteCanal(canal->Eqpto, canalQ, canal->Fases);
        if (!canal_q)
        {
            canal_q = medidor->InsereCanal(canal->Eqpto, canalQ, canal->Fases);
            canal_q->Tolerancia_perc = canal->Tolerancia_perc;
        }
        // converte curva I para Q
        CorrenteToQ(canal, canal_q);
        // habilita/desabilita os Canais criados
        canal_p->Enabled = canal->Enabled;
        canal_q->Enabled = canal->Enabled;
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanaisIparaCanalPQtrifasico(VTMedidor *medidor)
{
    // variaveis locais
    TList *lisCANAL;
    VTCanal *canal;
    VTCanal *canal_p3f, *canal_q3f;
    VTCanal *canal_pa, *canal_pb, *canal_pc;
    VTCanal *canal_qa, *canal_qb, *canal_qc;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // proteção
    if (!medidor)
        return (false);
    // cria canais P/Q por fase
    if (!CanaisIparaCanaisPQfase(medidor))
        return (false);
    // reinicia lisEQP
    lisEQP->Clear();
    // substitui os Canais P/Q por fase por canais P/Q trifásicos
    lisCANAL = medidor->LisCanal();
    for (int nc = 0; nc < lisCANAL->Count; nc++)
    {
        canal = (VTCanal*)lisCANAL->Items[nc];
        if ((canal->TipoCanal == canalP) && (canal->Fases == faseA))
        { // obtém os canais P das fases B e C
            canal_pa = canal;
            if ((canal_pb = medidor->ExisteCanal(canal->Eqpto, canalP, faseB)) == NULL)
                continue;
            if ((canal_pc = medidor->ExisteCanal(canal->Eqpto, canalP, faseC)) == NULL)
                continue;
            // obtém os canais Q das fases A, B e C
            if ((canal_qa = medidor->ExisteCanal(canal->Eqpto, canalQ, faseA)) == NULL)
                continue;
            if ((canal_qb = medidor->ExisteCanal(canal->Eqpto, canalQ, faseB)) == NULL)
                continue;
            if ((canal_qc = medidor->ExisteCanal(canal->Eqpto, canalQ, faseC)) == NULL)
                continue;
            // verifica se já existe canal P do mesmo eqpto para as fases ABC
            if ((canal_p3f = medidor->ExisteCanal(canal->Eqpto, canalP, faseABC)) == NULL)
            { // cria canal P para as 3 fases e cria sua Curva
                canal_p3f = medidor->InsereCanal(canal->Eqpto, canalP, faseABC);
                canal_p3f->Tolerancia_perc = canal->Tolerancia_perc;
                canal_p3f->Curva = canal->Curva->Clone();
            }
            // verifica se já existe canal Q do mesmo eqpto para as fases ABC
            if ((canal_q3f = medidor->ExisteCanal(canal->Eqpto, canalQ, faseABC)) == NULL)
            { // cria canal P para as 3 fases
                canal_q3f = medidor->InsereCanal(canal->Eqpto, canalQ, faseABC);
                canal_q3f->Tolerancia_perc = canal->Tolerancia_perc;
                canal_q3f->Curva = canal->Curva->Clone();
            }
            // atualiza curva de valores medidos dos canais P e Q trifásicos
            for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
            {
                patamar = patamares->Patamar[ind_pat];
                canal_p3f->ValorMedido[patamar] = canal_pa->ValorMedido[patamar] +
                    canal_pb->ValorMedido[patamar] + canal_pc->ValorMedido[patamar];
                canal_q3f->ValorMedido[patamar] = canal_qa->ValorMedido[patamar] +
                    canal_qb->ValorMedido[patamar] + canal_qc->ValorMedido[patamar];
                // habilita/desabilita os Canais criados
                canal_p3f->Enabled = canal->Enabled;
                canal_q3f->Enabled = canal->Enabled;
            }
            // insere os Canais p/Q por fase em lisEQP para serem retirados do Medidor
            lisEQP->Add(canal_pa);
            lisEQP->Add(canal_pb);
            lisEQP->Add(canal_pc);
            lisEQP->Add(canal_qa);
            lisEQP->Add(canal_qb);
            lisEQP->Add(canal_qc);
        }
    }
    // remove todos os Canais P/Q por fase de lisEQP do Medidor
    for (int nc = lisEQP->Count - 1; nc >= 0; nc--)
    {
        canal = (VTCanal*)lisEQP->Items[nc];
        medidor->RemoveCanal(canal);
    }
    // destrói os Canais removidos
    LimpaTList(lisEQP);

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanalP3FparaCanaisPfase(VTMedidor *medidor, VTCanal *canal_p3f)
{
    // variaveis locais
    int ind_bar;
    double p_mw;
    double fatmult_a, fatmult_b, fatmult_c;
    VTLigacao *ligacao;
    VTCanal *canal_pa, *canal_pb, *canal_pc;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // proteção
    if ((medidor == NULL) || (canal_p3f == NULL))
        return (false);
    // verifica se o Eqpto associado ao Canal é uma Ligacao
    if (!canal_p3f->Eqpto->TipoLigacao())
        return (false);
    ligacao = (VTLigacao*)canal_p3f->Eqpto;
    // determina índice da Barra de referência da medição
    ind_bar = ligacao->IndiceBarra(medidor->pbarra);
    // cria canais P por fase
    canal_pa = medidor->InsereCanal(canal_p3f->Eqpto, canalP, faseA);
    canal_pb = medidor->InsereCanal(canal_p3f->Eqpto, canalP, faseB);
    canal_pc = medidor->InsereCanal(canal_p3f->Eqpto, canalP, faseC);
    // define tolerância dos canais
    canal_pa->Tolerancia_perc = canal_p3f->Tolerancia_perc;
    canal_pb->Tolerancia_perc = canal_p3f->Tolerancia_perc;
    canal_pc->Tolerancia_perc = canal_p3f->Tolerancia_perc;
    // cria curva dos canais P clonando a curva do canal P3f
    canal_pa->Curva = canal_p3f->Curva->Clone();
    canal_pb->Curva = canal_p3f->Curva->Clone();
    canal_pc->Curva = canal_p3f->Curva->Clone();
    // atualiza curva de valores medidos dos canais P e Q trifásicos
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        // calcula fator de multiplicação para dividir a P3f em Pfase
        p_mw = ligacao->resflow->P_mw[ind_bar][ind_pat];
        if (IsDoubleZero(p_mw))
        {
            fatmult_a = fatmult_b = fatmult_c = 1. / 3.;
        }
        else
        {
            fatmult_a = ligacao->resflow->Pa_mw[ind_bar][ind_pat] / p_mw;
            fatmult_b = ligacao->resflow->Pb_mw[ind_bar][ind_pat] / p_mw;
            fatmult_c = ligacao->resflow->Pc_mw[ind_bar][ind_pat] / p_mw;
        }
        canal_pa->ValorMedido[patamar] = canal_p3f->ValorMedido[patamar] * fatmult_a;
        canal_pb->ValorMedido[patamar] = canal_p3f->ValorMedido[patamar] * fatmult_b;
        canal_pc->ValorMedido[patamar] = canal_p3f->ValorMedido[patamar] * fatmult_c;
    }
    // remove canal_p3f do Medidor
    medidor->RemoveCanal(canal_p3f);
    // destrói objeto
    lisEQP->Clear();
    lisEQP->Add(canal_p3f);
    LimpaTList(lisEQP);

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanalQ3FparaCanaisQfase(VTMedidor *medidor, VTCanal *canal_q3f)
{
    // variaveis locais
    int ind_bar;
    double q_mvar;
    double fatmult_a, fatmult_b, fatmult_c;
    VTLigacao *ligacao;
    VTCanal *canal_qa, *canal_qb, *canal_qc;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // proteção
    if ((medidor == NULL) || (canal_q3f == NULL))
        return (false);
    // verifica se o Eqpto associado ao Canal é uma Ligacao
    if (!canal_q3f->Eqpto->TipoLigacao())
        return (false);
    ligacao = (VTLigacao*)canal_q3f->Eqpto;
    // determina índice da Barra de referência da medição
    ind_bar = ligacao->IndiceBarra(medidor->pbarra);
    // cria canais Q por fase
    canal_qa = medidor->InsereCanal(canal_q3f->Eqpto, canalQ, faseA);
    canal_qb = medidor->InsereCanal(canal_q3f->Eqpto, canalQ, faseB);
    canal_qc = medidor->InsereCanal(canal_q3f->Eqpto, canalQ, faseC);
    // define tolerância dos canais
    canal_qa->Tolerancia_perc = canal_q3f->Tolerancia_perc;
    canal_qb->Tolerancia_perc = canal_q3f->Tolerancia_perc;
    canal_qc->Tolerancia_perc = canal_q3f->Tolerancia_perc;
    // cria curva dos canais Q clonando a curva do canal Q3f
    canal_qa->Curva = canal_q3f->Curva->Clone();
    canal_qb->Curva = canal_q3f->Curva->Clone();
    canal_qc->Curva = canal_q3f->Curva->Clone();
    // atualiza curva de valores medidos dos canais P e Q trifásicos
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        // calcula fator de multiplicação para dividir a Q3f em Qfase
        q_mvar = ligacao->resflow->Q_mvar[ind_bar][ind_pat];
        if (IsDoubleZero(q_mvar))
        {
            fatmult_a = fatmult_b = fatmult_c = 1. / 3.;
        }
        else
        {
            fatmult_a = ligacao->resflow->Qa_mvar[ind_bar][ind_pat] / q_mvar;
            fatmult_b = ligacao->resflow->Qb_mvar[ind_bar][ind_pat] / q_mvar;
            fatmult_c = ligacao->resflow->Qc_mvar[ind_bar][ind_pat] / q_mvar;
        }
        canal_qa->ValorMedido[patamar] = canal_q3f->ValorMedido[patamar] * fatmult_a;
        canal_qb->ValorMedido[patamar] = canal_q3f->ValorMedido[patamar] * fatmult_b;
        canal_qc->ValorMedido[patamar] = canal_q3f->ValorMedido[patamar] * fatmult_c;
    }
    // remove canal_q3f do Medidor
    medidor->RemoveCanal(canal_q3f);
    // destrói objeto
    lisEQP->Clear();
    lisEQP->Add(canal_q3f);
    LimpaTList(lisEQP);

    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanaisI(TList *lisI, VTMedicao *medicao)
{
    // proteção
    if (!lisI)
        return (false);
    if (!medicao)
        return (false);
    if (lisI->Count == 0)
        return (false);
    // salva medicao
    this->medicao = medicao;
    // converte correntes dos canais em curva PQ por fase
    return (I3fase(lisI));
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanaisP(TList *lisPQ, VTMedicao *medicao)
{
    // proteção
    if (!lisPQ)
        return (false);
    if (!medicao)
        return (false);
    if (lisPQ->Count == 0)
        return (false);
    // salva medicao
    this->medicao = medicao;
    // converte canais P em curva PQ por fase
    return (Pfase(lisPQ));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::CanaisQ(TList *lisPQ, VTMedicao *medicao)
{
    // proteção
    if (!lisPQ)
        return (false);
    if (!medicao)
        return (false);
    if (lisPQ->Count == 0)
        return (false);
    // salva medicao
    this->medicao = medicao;
    // converte canais Q em curva PQ por fase
    return (Qfase(lisPQ));
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanaisPQ(TList *lisPQ, VTMedicao *medicao)
{
    // proteção
    if (!lisPQ)
        return (false);
    if (!medicao)
        return (false);
    if (lisPQ->Count == 0)
        return (false);
    // salva medicao
    this->medicao = medicao;
    // converte canais em curva PQ por fase
    return (PQfase(lisPQ));
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CanaisS(TList *lisS, VTMedicao *medicao)
{
    // proteção
    if (!lisS)
        return (false);
    if (!medicao)
        return (false);
    if (lisS->Count == 0)
        return (false);
    // salva medicao
    this->medicao = medicao;
    // converte canais em curva PQ por fase
    return (Strifasico(lisS));
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::Energia(void)
 {
 //variáveis locais
 int          index = 0;
 double       e_med_dia_mwh[1];
 double       e_cal_dia_mwh = 0.;
 double       fat_cor;
 double       pq_mw_mvar[MAX_PONTO];
 double       p_mw[MAX_FASE], q_mvar[MAX_FASE];
 strHM        hm_ini, hm_fim;
 VTPatamar   *patamar;
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //determina primeiro ponto da Curva de medição
 if (! curva_med_original->GetPonto(index, hm_ini, hm_fim, e_med_dia_mwh, 1)) return(false);
 //calcula demandas fluxo
 if (! CalculaDemandaFluxoPorFase()) return(false);
 //calcula energia diaria
 e_cal_dia_mwh = CalculaEnergiaDiariaFluxo();
 //recalcula curva de medição
 medicao->curva_pq->NumeroValores = MAX_PONTO;
 medicao->curva_pq->Reconfigura(patamares->Escala);
 //calcula fator de correção
 if(IsDoubleZero(e_cal_dia_mwh)) fat_cor = 1.;
 else   fat_cor = e_med_dia_mwh[index] / e_cal_dia_mwh;
 for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
 {
 patamar = patamares->Patamar[ind_pat];
 hm_ini.hora   = patamar->HoraIni;
 hm_ini.minuto = patamar->MinutoIni;
 hm_fim.hora   = patamar->HoraFim;
 hm_fim.minuto = patamar->MinutoFim;
 //determina P e Q do fluxo
 for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
 {
 p_mw[ind_fase]   = CalculaFluxoP(ind_fase, ind_pat);
 q_mvar[ind_fase] = CalculaFluxoQ(ind_fase, ind_pat);
 }
 //calcula potência ativa e reativa de cada fase no patamar a partir do fluxo
 for (int ind = 0, ind_fase = indFASE_A; ind < MAX_PONTO; ind+=2, ind_fase++)
 {
 pq_mw_mvar[ind]   = p_mw[ind_fase]     * (fat_cor);  //P
 pq_mw_mvar[ind+1] = q_mvar[ind_fase];                //Q
 }
 //atualiza Curva de medição
 medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TConverte::CorrenteToP(VTCanal *canal_i, VTCanal *canal_p)
{
    // variáveis locais
    double p_flow_mw, p_med_mw, fatcor;
    VTBarra *barra;
    VTLigacao *ligacao;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // proteção
    if ((!canal_i) || (!canal_p))
        return (false);
    // verifica se o eqpto é ligacao
    if (!canal_i->Eqpto->TipoLigacao())
        return (false);
    // verifica se existe curva canal P
    if (!canal_p->Curva)
    {
        canal_p->Curva = canal_i->Curva->Clone();
        canal_p->Curva->Id = FalsoId();
    }
    // recalcula curva de medição de corrente
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        // zera valores
        p_med_mw = fatcor = 0.;
        // calcula fator de multiplicação p/ corrigir demandas
        fatcor = CalculaFatorCorrecaoSfase(canal_i, ind_pat);
        // verifica se fatcor é nulo
        if (IsDoubleZero(fatcor))
        { // calcula pot.medida em função da corrente medida, assumindo um fator potência fixo
            barra = canal_i->Medidor->Barra;
            p_med_mw = ((barra->vnom / sqrt(3.)) * canal_i->ValorMedido[patamar] * 0.001) * FP_092;
        }
        else
        { // calcula potência ativa no patamar
            p_flow_mw = CalculaFluxoP(canal_i, ind_pat);
            p_med_mw = fatcor * p_flow_mw;
        }
        // atualiza Curva de medição
        canal_p->ValorMedido[patamar] = p_med_mw;
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::CorrenteToQ(VTCanal *canal_i, VTCanal *canal_q)
{
    // variáveis locais
    double q_flow_mvar, q_med_mvar, fatcor;
    VTBarra *barra;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // proteção
    if ((!canal_i) || (!canal_q))
        return (false);
    // verifica se o eqpto é ligacao
    if (!canal_i->Eqpto->TipoLigacao())
        return (false);
    // verifica se existe curva canal Q
    if (!canal_q->Curva)
    {
        canal_q->Curva = canal_i->Curva->Clone();
        canal_q->Curva->Id = FalsoId();
    }
    // recalcula curva de medição de corrente
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        // zera valores
        q_med_mvar = fatcor = 0.;
        // calcula fator de multiplicação p/ corrigir demandas
        fatcor = CalculaFatorCorrecaoSfase(canal_i, ind_pat);
        // verifica se fatcor é nulo
        if (IsDoubleZero(fatcor))
        { // calcula pot.reativa medida em função da corrente medida, assumindo um fator potência fixo
            barra = canal_i->Medidor->Barra;
            q_med_mvar = ((barra->vnom / sqrt(3.)) * canal_i->ValorMedido[patamar] * 0.001) * Sin
                (ArcCos(FP_092));
        }
        else
        { // calcula potência reativa no patamar
            q_flow_mvar = CalculaFluxoQ(canal_i, ind_pat);
            q_med_mvar = fatcor * q_flow_mvar;
        }
        // atualiza Curva de medição
        canal_q->ValorMedido[patamar] = q_med_mvar;
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::DeterminaVfn_kv(int ind_pat)
{
    // define valor default
    vfn_kv[indFASE_A] = 13.8 / sqrt(3.);
    vfn_kv[indFASE_B] = 13.8 / sqrt(3.);
    vfn_kv[indFASE_C] = 13.8 / sqrt(3.);
    // proteção
    if (!medicao)
        return (false);
    if (!medicao->chave_med)
        return (false);
    // define valor da tensão em uma das barras da chave  //DVK 2015.08.25 do flow
    // vfn_kv = medicao->chave_med->pbarra1->vnom / sqrt(3.);
    vfn_kv[indFASE_A] = abs(medicao->chave_med->pbarra1->resflow->Van_kv[ind_pat]);
    vfn_kv[indFASE_B] = abs(medicao->chave_med->pbarra1->resflow->Vbn_kv[ind_pat]);
    vfn_kv[indFASE_C] = abs(medicao->chave_med->pbarra1->resflow->Vcn_kv[ind_pat]);
    return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::Executa(VTMedicao *medicao, double fator_unico)
 {
 //variáveis locais
 bool sucesso = true;

 //salva ponteiro p/ objeto
 this->medicao = medicao;
 //guarda curva original com medição
 if ((curva_med_original = DLL_NewObjCurva()) == NULL) return(false);
 curva_med_original->CopiaAtributosDe(*medicao->curva_orig);
 //converte a medição em PQ trifásico, conforme tipo de curva
 switch(medicao->curva_orig->Tipo)
 {
 case medicaoIMAXxFASE:
 sucesso = ImaxFase();
 break;
 case medicaoCURVA_IxFASE:
 sucesso = Ifase();
 break;
 case medicaoCURVA_P:
 sucesso = Ptrifasico();
 break;
 case medicaoCURVA_PQ:
 if(fator_unico > 0.) sucesso = FatorUnico(fator_unico);
 else                   sucesso = PQtrifasico();
 break;
 case medicaoCURVA_PQxFASE:
 sucesso = PQfase();
 break;
 case medicaoENERGIA_MWH:
 sucesso = Energia();
 break;
 default:
 return(false);
 }
 medicao->curva_pq->Tipo = medicaoCURVA_PQxFASE;
 return(sucesso);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TConverte::Executa(VTMedicao *medicao)
{ /*
     //variáveis locais
     bool sucesso = true;

     //proteção
     if(! medicao) return(false);
     //salva ponteiro p/ objeto
     this->medicao = medicao;
     liga_inicial  = medicao->chave_med;
     //guarda curva original com medição
     curva_med_original = medicao->curva_orig;
     //cria curva PQ: cuidado, a curva já pode ter sido criada em uma execução anterior
     if (medicao->curva_pq != NULL) delete medicao->curva_pq;
     medicao->curva_pq = DLL_NewObjCurva();
     medicao->curva_pq->Tipo = medicaoCURVA_PQxFASE;
     //verifica se é curva de fator único
     //   if(fator_unico > 0.) return(FatorUnico(fator_unico));
     //converte a medição em PQ trifásico, conforme tipo de curva
     switch(medicao->curva_orig->Tipo)
     {
     case medicaoIMAXxFASE:
     sucesso = ImaxFase();
     break;
     case medicaoCURVA_Ix1FASE:
     sucesso = I1fase();
     break;
     case medicaoCURVA_Ix3FASE:
     sucesso = I3fase();
     break;
     case medicaoCURVA_P:
     sucesso = Ptrifasico();
     break;
     case medicaoCURVA_PQ:
     sucesso = PQtrifasico();
     break;
     case medicaoCURVA_PQxFASE:
     sucesso = PQfase();
     break;
     case medicaoENERGIA_MWH:
     sucesso = Energia();
     break;
     default:
     return(false);
     }
     return(sucesso); */
    return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::FatorUnico(double fator_unico)
 {
 //variáveis locais
 double       pq_mw_mvar[MAX_PONTO];
 double       p_mw[MAX_FASE], q_mvar[MAX_FASE];
 strHM        hm_ini, hm_fim;
 VTPatamar   *patamar;
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //calcula demandas fluxo
 if (! CalculaDemandaFluxoPorFase()) return(false);
 //recalcula curva de medição
 medicao->curva_pq->NumeroValores = MAX_PONTO;
 medicao->curva_pq->Reconfigura(patamares->Escala);
 medicao->curva_orig->NumeroValores = MAX_PONTO;
 medicao->curva_orig->Reconfigura(patamares->Escala);
 for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
 {
 patamar = patamares->Patamar[ind_pat];
 hm_ini.hora   = patamar->HoraIni;
 hm_ini.minuto = patamar->MinutoIni;
 hm_fim.hora   = patamar->HoraFim;
 hm_fim.minuto = patamar->MinutoFim;
 //determina P e Q do fluxo
 for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
 {
 p_mw[ind_fase]   = CalculaFluxoP(ind_fase, ind_pat);
 q_mvar[ind_fase] = CalculaFluxoQ(ind_fase, ind_pat);
 }
 //calcula potência ativa e reativa de cada fase no patamar a partir do fluxo
 for (int ind = 0, ind_fase = indFASE_A; ind < MAX_PONTO; ind+=2, ind_fase++)
 {
 //         pq_mw_mvar[ind]   = p_mw[ind_fase]     * (fator_unico + 0.005);  //P
 //         pq_mw_mvar[ind+1] = q_mvar[ind_fase] * (fator_unico + 0.005);  //Q
 pq_mw_mvar[ind]   = p_mw[ind_fase]     * (fator_unico);  //P
 pq_mw_mvar[ind+1] = q_mvar[ind_fase] * (fator_unico);  //Q
 }
 //atualiza Curva de medição
 medicao->curva_orig->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
 medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
 }
 return(true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::I1fase(void)
 {
 //variáveis locais
 double       p_flow_mw, q_flow_mvar, fatcor_s[MAX_FASE];
 double       pq_mw_mvar[MAX_PONTO], fat_cor_valido;
 strHM        hm;
 VTPatamar   *patamar;
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //calcula demandas fluxo (S por fase)
 if (! CalculaDemandaFluxoPorFase()) return(false);
 //recalcula curva de medição de corrente
 medicao->curva_pq->NumeroValores = MAX_PONTO;
 medicao->curva_pq->Reconfigura(patamares->Escala);
 curva_med_original->Reconfigura(patamares->Escala);
 for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
 {
 patamar = patamares->Patamar[ind_pat];
 //determina tensão nominal de fase p/ cálculo de demanda por fase
 if (! DeterminaVfn_kv(ind_pat)) return(false);
 //calcula demandas medicaos (S 1 fase) em função das correntes medidas
 if (! CalculaDemandaMedicao1Fase(ind_pat)) return(false);
 //calcula fator de multiplicação p/ corrigir demandas por fase
 fat_cor_valido = 0.;
 for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
 {
 fatcor_s[ind_fase] = CalculaFatorCorrecaoSfase(ind_fase, ind_pat);
 if(! IsDoubleZero(fatcor_s[ind_fase])) fat_cor_valido = fatcor_s[ind_fase];
 }
 fatcor_s[indNEUTRO] = 0.;
 //calcula potência ativa e reativa de cada fase no patamar
 for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C); ind+=2)
 {
 p_flow_mw   = CalculaFluxoP(ind_fase, ind_pat);
 q_flow_mvar = CalculaFluxoQ(ind_fase, ind_pat);
 //usa mesmo fator de correção
 pq_mw_mvar[ind]   = fat_cor_valido * p_flow_mw;
 pq_mw_mvar[ind+1] = fat_cor_valido * q_flow_mvar;
 ind_fase++;
 }
 //atualiza Curva de medição
 hm.hora   = patamar->HoraIni;
 hm.minuto = patamar->MinutoIni;
 medicao->curva_pq->SetPonto(hm, pq_mw_mvar, MAX_PONTO);
 }
 return(true);
 }
 */
/*
 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::I3fase(TList *lisI)
 {
 //variáveis locais
 double       p_flow_mw, q_flow_mvar, fatcor_s[MAX_FASE];
 double       pq_mw_mvar[MAX_PONTO];
 strHM        hm;
 VTPatamar   *patamar;
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //limpa vetor
 for(int n = 0; n < MAX_PONTO; n++) pq_mw_mvar[n] = 0.;
 for(int n = 0; n < MAX_FASE;  n++) fatcor_s[n]   = 0.;
 //calcula demandas fluxo (S por fase)
 if (! CalculaDemandaFluxoPorFase()) return(false);
 //recalcula curva de medição de corrente
 medicao->curva_pq->NumeroValores = MAX_PONTO;
 medicao->curva_pq->Reconfigura(patamares->Escala);
 for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
 {
 patamar = patamares->Patamar[ind_pat];
 //determina tensão nominal de fase p/ cálculo de demanda por fase
 if (! DeterminaVfn_kv(ind_pat)) return(false);
 //calcula demandas medicaos (S por fase) em função das correntes medidas
 if (! CalculaDemandaMedicaoPorFase(patamar, lisI)) return(false);
 //calcula fator de multiplicação p/ corrigir demandas por fase
 CalculaFatorCorrecaoSfase(fatcor_s, ind_pat);
 //calcula potência ativa e reativa de cada fase no patamar
 for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C); ind+=2)
 {
 p_flow_mw   = CalculaFluxoP(ind_fase, ind_pat);
 q_flow_mvar = CalculaFluxoQ(ind_fase, ind_pat);
 pq_mw_mvar[ind]   = fatcor_s[ind_fase] * p_flow_mw;
 pq_mw_mvar[ind+1] = fatcor_s[ind_fase] * q_flow_mvar;
 ind_fase++;
 }
 //atualiza Curva de medição
 hm.hora   = patamar->HoraIni;
 hm.minuto = patamar->MinutoIni;
 medicao->curva_pq->SetPonto(hm, pq_mw_mvar, MAX_PONTO);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TConverte::I3fase(TList *lisI)
{
    // variáveis locais
    double p_flow_mw, q_flow_mvar, fatcor[MAX_FASE];
    double pq_mw_mvar[MAX_PONTO];
    strHM hm;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    for (int n = 0; n < MAX_FASE; n++)
        fatcor[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição de corrente
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        // calcula fator de multiplicação p/ corrigir demandas por fase
        if (!CalculaFatorCorrecaoIfase(fatcor, patamar, lisI))
            return (false);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            p_flow_mw = CalculaFluxoP(ind_fase, ind_pat);
            q_flow_mvar = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind] = fatcor[ind_fase] * p_flow_mw;
            pq_mw_mvar[ind + 1] = fatcor[ind_fase] * q_flow_mvar;
            ind_fase++;
        }
        // atualiza Curva de medição
        hm.hora = patamar->HoraIni;
        hm.minuto = patamar->MinutoIni;
        medicao->curva_pq->SetPonto(hm, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::ImaxFase(void)
 {
 //variáveis locais
 int          ind_pat;
 double       p_flow_mw, q_flow_mvar, fatcor_s[MAX_FASE];
 double       pq_mw_mvar[MAX_PONTO];
 strHM        hm;
 VTPatamar   *patamar;
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //determina patamar de demanda máxima da Medicao
 ind_pat = medicao->ind_pat_dem_max;
 //determina tensão nominal de fase p/ cálculo de demanda por fase
 if (! DeterminaVfn_kv(ind_pat)) return(false);
 //calcula demandas medicaos (S por fase) em função das correntes máximas de medicao
 if (! CalculaDemandaMedicaoPorFase(0)) return(false);
 //calcula demandas fluxo (S por fase) do patamar de demanda máxima
 if (! CalculaDemandaFluxoPorFase()) return(false);
 //calcula fator de multiplicação p/ corrigir demandas por fase (no pat.máximo)
 for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
 {
 fatcor_s[ind_fase] = CalculaFatorCorrecaoSfase(ind_fase, ind_pat);
 }
 fatcor_s[indNEUTRO] = 0.;
 //recalcula curva de medição de corrente
 medicao->curva_pq->NumeroValores = MAX_PONTO;
 medicao->curva_pq->Reconfigura(patamares->Escala);
 curva_med_original->Reconfigura(patamares->Escala);
 for (ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
 {
 patamar = patamares->Patamar[ind_pat];
 //calcula potência ativa e reativa de cada fase no patamar
 for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C); ind+=2)
 {
 p_flow_mw   = CalculaFluxoP(ind_fase, ind_pat);
 q_flow_mvar = CalculaFluxoQ(ind_fase, ind_pat);
 pq_mw_mvar[ind]   = fatcor_s[ind_fase] * p_flow_mw;
 pq_mw_mvar[ind+1] = fatcor_s[ind_fase] * q_flow_mvar;
 ind_fase++;
 }
 //atualiza Curva de medição
 hm.hora   = patamar->HoraIni;
 hm.minuto = patamar->MinutoIni;
 medicao->curva_pq->SetPonto(hm, pq_mw_mvar, MAX_PONTO);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TConverte::PQfase(TList *lisPQ)
{
    // variaveis locais
    double p_mw[MAX_FASE], q_mvar[MAX_FASE];
    double pq_mva[MAX_FASE + MAX_FASE];
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
    TList *lisPAT = patamares->LisPatamar();

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_FASE; n++)
    {
        p_mw[n] = 0.;
        q_mvar[n] = 0.;
    }
    // proteção
    if (!lisPQ)
        return (false);
    if (!medicao)
        return (false);
    if (lisPQ->Count == 0)
        return (false);
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    // verifica se ha algum canal com medição trifásica
    canal = (VTCanal*)lisPQ->First();
    if (canal->Fases == faseABC)
        return (PQtrifasico(lisPQ));
    for (int np = 0; np < lisPAT->Count; np++)
    {
        patamar = (VTPatamar*)lisPAT->Items[np];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraIni + 1;
        hm_fim.minuto = patamar->MinutoIni;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisPQ, patamar, pq_mva);
        // concatena curva por fase em uma única curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mva, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::PQtrifasico(TList *lisPQ)
{
    // variáveis locais
    double pq_mw_mvar[MAX_PONTO];
    double pq_mva[MAX_FASE + MAX_FASE];
    double fat_cor_p, fat_cor_q;
    double p_flow, q_flow;
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraFim;
        hm_fim.minuto = patamar->MinutoFim;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisPQ, patamar, pq_mva);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            fat_cor_p = CalculaFatorCorrecaoP3f(ind_pat, pq_mva[0]);
            fat_cor_q = CalculaFatorCorrecaoQ3f(ind_pat, pq_mva[1]);
            p_flow = CalculaFluxoP(ind_fase, ind_pat);
            q_flow = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind] = fat_cor_p * p_flow; // P
            pq_mw_mvar[ind + 1] = fat_cor_q * q_flow; // Q
            ind_fase++;
        }
        // atualiza Curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::Pfase(TList *lisPQ)
{
    // variáveis locais
    int num_ponto = 1;
    double pq_mw_mvar[MAX_PONTO];
    double pq_mva[MAX_FASE + MAX_FASE];
    double p_flow, q_flow, fat_cor_p;
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição de corrente
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    // verifica se ha algum canal com medição trifásica
    canal = (VTCanal*)lisPQ->First();
    if (canal->Fases == faseABC)
        return (Ptrifasico(lisPQ));
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraFim;
        hm_fim.minuto = patamar->MinutoFim;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisPQ, patamar, pq_mva);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            fat_cor_p = CalculaFatorCorrecaoPfase(ind_fase, ind_pat, pq_mva[ind]);
            // calcula Q proporcional à relação entre p_medido e p_fluxo
            p_flow = CalculaFluxoP(ind_fase, ind_pat);
            q_flow = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind] = fat_cor_p * p_flow; // P
            // se nao tiver p_flow, mantém q_flow
            if (IsDoubleZero(p_flow))
                pq_mw_mvar[ind + 1] = q_flow;
            else
                pq_mw_mvar[ind + 1] = q_flow * (pq_mw_mvar[ind] / p_flow); // Q
            ind_fase++;
        }
        // atualiza Curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::Qfase(TList *lisPQ)
{
    // variáveis locais
    int num_ponto = 1;
    double pq_mw_mvar[MAX_PONTO];
    double pq_mva[MAX_FASE + MAX_FASE];
    double p_flow, q_flow, fat_cor_q;
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição de corrente
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    // verifica se ha algum canal com medição trifásica
    canal = (VTCanal*)lisPQ->First();
    if (canal->Fases == faseABC)
        return (Qtrifasico(lisPQ));
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraFim;
        hm_fim.minuto = patamar->MinutoFim;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisPQ, patamar, pq_mva);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            fat_cor_q = CalculaFatorCorrecaoQfase(ind_fase, ind_pat, pq_mva[ind + 1]);
            // calcula P proporcional à relação entre q_medido e q_fluxo
            p_flow = CalculaFluxoP(ind_fase, ind_pat);
            q_flow = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind + 1] = fat_cor_q * q_flow; // Q
            // se nao tiver q_flow, mantém p_flow
            if (IsDoubleZero(q_flow))
                pq_mw_mvar[ind] = p_flow;
            else
                pq_mw_mvar[ind] = p_flow * fat_cor_q; // P
            ind_fase++;
        }
        // atualiza Curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::Ptrifasico(TList *lisPQ)
{
    // variáveis locais
    int num_ponto = 1;
    double pq_mw_mvar[MAX_PONTO];
    double pq_mva[MAX_FASE + MAX_FASE];
    double p_flow, q_flow, fat_cor_p;
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição de corrente
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraFim;
        hm_fim.minuto = patamar->MinutoFim;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisPQ, patamar, pq_mva);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            fat_cor_p = CalculaFatorCorrecaoP3f(ind_pat, pq_mva[indP]);
            // calcula Q proporcional à relação entre p_medido e p_fluxo
            p_flow = CalculaFluxoP(ind_fase, ind_pat);
            q_flow = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind] = fat_cor_p * p_flow; // P
            // se nao tiver p_flow, mantém q_flow
            if (IsDoubleZero(p_flow))
                pq_mw_mvar[ind + 1] = q_flow;
            else
                pq_mw_mvar[ind + 1] = fat_cor_p * q_flow; // * (pq_mw_mvar[ind] / p_flow); //Q
            ind_fase++;
        }
        // atualiza Curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::Qtrifasico(TList *lisPQ)
{
    // variáveis locais
    int num_ponto = 1;
    double pq_mw_mvar[MAX_PONTO];
    double pq_mva[MAX_FASE + MAX_FASE];
    double p_flow, q_flow, fat_cor_q;
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição de corrente
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraFim;
        hm_fim.minuto = patamar->MinutoFim;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisPQ, patamar, pq_mva);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            fat_cor_q = CalculaFatorCorrecaoQ3f(ind_pat, pq_mva[indQ]);
            // calcula P proporcional à relação entre q_medido e q_fluxo
            p_flow = CalculaFluxoP(ind_fase, ind_pat);
            q_flow = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind + 1] = fat_cor_q * q_flow; // Q
            // se nao tiver q_flow, mantém p_flow
            if (IsDoubleZero(q_flow))
                pq_mw_mvar[ind] = p_flow;
            else
                pq_mw_mvar[ind] = fat_cor_q * p_flow; // * (pq_mw_mvar[ind] / p_flow); //P
            ind_fase++;
        }
        // atualiza Curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
bool __fastcall TConverte::Strifasico(TList *lisS)
{
    // variáveis locais
    int num_ponto = 1;
    double pq_mw_mvar[MAX_PONTO];
    double pq_mva[MAX_FASE + MAX_FASE];
    double p_flow, q_flow, fat_cor_s;
    strHM hm_ini, hm_fim;
    VTCanal *canal;
    VTPatamar *patamar;
    VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    // limpa vetor
    for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
        pq_mva[n] = 0.;
    for (int n = 0; n < MAX_PONTO; n++)
        pq_mw_mvar[n] = 0.;
    // calcula demandas fluxo (S por fase)
    if (!CalculaDemandaFluxoPorFase())
        return (false);
    // recalcula curva de medição de corrente
    medicao->curva_pq->NumeroValores = MAX_PONTO;
    medicao->curva_pq->Reconfigura(patamares->Escala);
    for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
    {
        patamar = patamares->Patamar[ind_pat];
        hm_ini.hora = patamar->HoraIni;
        hm_ini.minuto = patamar->MinutoIni;
        hm_fim.hora = patamar->HoraFim;
        hm_fim.minuto = patamar->MinutoFim;
        // limpa vetor
        for (int n = 0; n < MAX_FASE + MAX_FASE; n++)
            pq_mva[n] = 0.;
        // determina valores no patamar
        ValorCanal(lisS, patamar, pq_mva);
        // calcula potência ativa e reativa de cada fase no patamar
        for (int ind = 0, ind_fase = indFASE_A; (ind < MAX_PONTO) && (ind_fase <= indFASE_C);
        ind += 2)
        {
            fat_cor_s = CalculaFatorCorrecaoS3f(ind_pat, pq_mva[indP]);
            // calcula PQ proporcional à relação entre s_medido e s_fluxo
            p_flow = CalculaFluxoP(ind_fase, ind_pat);
            q_flow = CalculaFluxoQ(ind_fase, ind_pat);
            pq_mw_mvar[ind] = fat_cor_s * p_flow; // P
            pq_mw_mvar[ind + 1] = fat_cor_s * q_flow; // Q
            ind_fase++;
        }
        // atualiza Curva de medição
        medicao->curva_pq->SetPonto(hm_ini, pq_mw_mvar, MAX_PONTO);
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TConverte::ValorCanal(TList *lisPQ, VTPatamar *patamar,
    double pq_mva[MAX_FASE + MAX_FASE])
{
    // variaveis locais
    VTCanal *canal;

    // proteção
    if (!patamar)
        return (false);
    for (int nc = 0; nc < lisPQ->Count; nc++)
    {
        canal = (VTCanal*)lisPQ->Items[nc];
        if (canal->TipoCanal == canalP)
        { // potência ativa MW
            if (canal->Fases == faseA)
                pq_mva[0] = canal->ValorMedido[patamar];
            if (canal->Fases == faseB)
                pq_mva[2] = canal->ValorMedido[patamar];
            if (canal->Fases == faseC)
                pq_mva[4] = canal->ValorMedido[patamar];
            if (canal->Fases == faseABC)
                pq_mva[0] = canal->ValorMedido[patamar];
        }
        else
        { // potência reativa Mvar
            if (canal->Fases == faseA)
                pq_mva[1] = canal->ValorMedido[patamar];
            if (canal->Fases == faseB)
                pq_mva[3] = canal->ValorMedido[patamar];
            if (canal->Fases == faseC)
                pq_mva[5] = canal->ValorMedido[patamar];
            if (canal->Fases == faseABC)
                pq_mva[1] = canal->ValorMedido[patamar];
        }
    }
    return (true);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall  TConverte::ValorMedidoNulo(VTCanal *canal)
 {
 //variaveis locais
 bool        nulo = false;
 int         num_pat;
 VTPatamar   *patamar;
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //verifica se a curva existe
 if(! canal->Curva) return(true);
 //determina patamares
 num_pat = patamares->NumPatamar();
 //reconfigura curva
 canal->Curva->Reconfigura(patamares->Escala);
 for(int np = 0; np < num_pat; np++)
 {//verifica se há valores nulos medidos
 patamar = patamares->Patamar[np];
 if(IsDoubleZero(canal->ValorMedido[patamar])) nulo = true;
 }
 return(nulo);
 }
 */

// ---------------------------------------------------------------------------
// eof
