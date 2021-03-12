// ---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "TCarregamento.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Fases.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRede.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TCarregamento::TCarregamento(VTApl *apl)
{
    // salva ponteiro
    this->apl = apl;
    // inicia controle de vetores alocados dinamicamente
    num_pat = 0;
    p_faseA = NULL;
    p_faseB = NULL;
    p_faseC = NULL;
    q_faseA = NULL;
    q_faseB = NULL;
    q_faseC = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TCarregamento::~TCarregamento(void)
{
    // destrói vetores alcoados
    LiberaVetoresCargto();
}

// ---------------------------------------------------------------------------
bool __fastcall TCarregamento::AcumulaDemPorFase(VTNet *net, VTEqpto *eqpto, int npat)
{

    return true;
}

// ---------------------------------------------------------------------------
void __fastcall TCarregamento::AlocaVetoresCargto(void)
{
    // variáveis locais
    VTPatamares *patamares = (VTPatamares*) apl->GetObject(__classid(VTPatamares));

    // verifica se os vetores já foram alocados para o número de pataamres
    if (num_pat == patamares->NumPatamar())
        return;
    // libera vetores alocados anteriormente
    if (num_pat != 0)
        LiberaVetoresCargto();
    // redefine valor de num_pat
    num_pat = patamares->NumPatamar();
    // aloca vetores dinâmicos
    p_faseA = new double[num_pat];
    p_faseB = new double[num_pat];
    p_faseC = new double[num_pat];
    q_faseA = new double[num_pat];
    q_faseB = new double[num_pat];
    q_faseC = new double[num_pat];
    // inicia os valores de carregamento
    for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
    {
        p_faseA[ind_pat] = p_faseB[ind_pat] = p_faseC[ind_pat] = 0.;
        q_faseA[ind_pat] = q_faseB[ind_pat] = q_faseC[ind_pat] = 0.;
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TCarregamento::AnalisaCarga(VTCarga *carga)
{
    // proteção
    if (!carga)
        return false;

    // loop para todos os patamares: distribuir a demanda por fase
    for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
    {
        DistribuiDemPorFase(carga, ind_pat);
    }
    return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TCarregamento::AnalisaRede(VTRede *rede)
{
    // var locais
    VTCarga *carga;
    TList *lisCarga;

    // proteção
    if (!rede)
        return false;
    // reseta os valores
    AlocaVetoresCargto();
    // obtem lista de cargas da rede
    lisCarga = new TList();
    rede->LisEqbar(lisCarga, eqptoCARGA);

    if (lisCarga->Count > 0)
    {
        // loop para todos os patamares: distribuir a demanda por fase
        for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
        { // por Carga
            for (int n = 0; n < lisCarga->Count; n++)
            {
                carga = (VTCarga*) lisCarga->Items[n];
                DistribuiDemPorFase(carga, ind_pat);
            }
        }
    }
    delete lisCarga;
    return true;
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TCarregamento::DistribuiDemPorFase(VTCarga *carga, int npat)
 {
 //var locais
 double     p, q;
 VTDemanda  *demanda =  (VTDemanda*) apl->GetObject(__classid(VTDemanda));

 //proteção
 if(! carga) return false;
 //determina potência ativa e reativa do Eqpto
 p = demanda->P_mw  [carga][npat];
 q = demanda->Q_mvar[carga][npat];
 //acumula as potencias de acordo com  a fase da carga (desconsidera terra)
 switch (carga->Fases & faseABCN)
 {
 case faseA:
 case faseAN:
 p_faseA[npat] += p;
 q_faseA[npat] += q;
 break;
 case faseB:
 case faseBN:
 p_faseB[npat] += p;
 q_faseB[npat] += q;
 break;
 case faseC:
 case faseCN:
 p_faseC[npat] += p;
 q_faseC[npat] += q;
 break;

 case faseAB:
 //faseA
 p_faseA[npat] += p/2.;
 q_faseA[npat] += q/2.;
 //faseB
 p_faseB[npat] += p/2.;
 q_faseB[npat] += q/2.;
 break;
 case faseABN:
 //faseA
 p_faseA[npat] += p/2.;
 q_faseA[npat] += q/2.;
 //faseB
 p_faseB[npat] += p/2.;
 q_faseB[npat] += q/2.;
 break;

 case faseCA:
 //faseA
 p_faseA[npat] += p/2.;
 q_faseA[npat] += q/2.;
 //faseC
 p_faseC[npat] += p/2.;
 q_faseC[npat] += q/2.;
 break;
 case faseCAN:
 //faseA
 p_faseA[npat] += p/2.;
 q_faseA[npat] += q/2.;
 //faseC
 p_faseC[npat] += p/2.;
 q_faseC[npat] += q/2.;
 break;

 case faseBC:
 //faseB
 p_faseB[npat] += p/2.;
 q_faseB[npat] += q/2.;
 //faseC
 p_faseC[npat] += p/2.;
 q_faseC[npat] += q/2.;
 break;
 case faseBCN:
 //faseB
 p_faseB[npat] += p/2.;
 q_faseB[npat] += q/2.;
 //faseC
 p_faseC[npat] += p/2.;
 q_faseC[npat] += q/2.;
 break;

 case faseABC:
 case faseABCN:
 //faseA
 p_faseA[npat] += p/3.;
 q_faseA[npat] += q/3.;
 //faseB
 p_faseB[npat] += p/3.;
 q_faseB[npat] += q/3.;
 //faseC
 p_faseC[npat] += p/3.;
 q_faseC[npat] += q/3.;
 break;
 default: break;
 }
 return true;
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TCarregamento::DistribuiDemPorFase(VTCarga *carga, int npat)
{
    // var locais
    double p, q;
    complex<double>s, sa, sb, sc, fat1, fat2;
    VTDemanda *demanda = (VTDemanda*) apl->GetObject(__classid(VTDemanda));

    // proteção
    if (!carga)
        return false;
    // determina potência ativa e reativa do Eqpto
    s = demanda->S_mva[carga][npat];
    p = demanda->P_mw[carga][npat];
    q = demanda->Q_mvar[carga][npat];
    fat1 = polar(1.0 / sqrt(3.0), -M_PI / 6);
    fat2 = polar(1.0 / sqrt(3.0), M_PI / 6);
    // acumula as potencias de acordo com  a fase da carga (desconsidera terra)
    switch (carga->Fases & faseABCN)
    {
    case faseA:
    case faseAN:
        p_faseA[npat] += p;
        q_faseA[npat] += q;
        break;
    case faseB:
    case faseBN:
        p_faseB[npat] += p;
        q_faseB[npat] += q;
        break;
    case faseC:
    case faseCN:
        p_faseC[npat] += p;
        q_faseC[npat] += q;
        break;

    case faseAB:
        // calcula demandas p/ fases A e B
        sa = s * fat1;
        sb = s * fat2;
        // faseA
        p_faseA[npat] += sa.real();
        q_faseA[npat] += sa.imag();
        // faseB
        p_faseB[npat] += sb.real();
        q_faseB[npat] += sb.imag();
        break;
    case faseABN:
        // faseA
        p_faseA[npat] += p / 2.;
        q_faseA[npat] += q / 2.;
        // faseB
        p_faseB[npat] += p / 2.;
        q_faseB[npat] += q / 2.;
        break;

    case faseBC:
        // calcula demandas p/ fases B e C
        sb = s * fat1;
        sc = s * fat2;
        // faseB
        p_faseB[npat] += sb.real();
        q_faseB[npat] += sb.imag();
        // faseC
        p_faseC[npat] += sc.real();
        q_faseC[npat] += sc.imag();
        break;
    case faseBCN:
        // faseB
        p_faseB[npat] += p / 2.;
        q_faseB[npat] += q / 2.;
        // faseC
        p_faseC[npat] += p / 2.;
        q_faseC[npat] += q / 2.;
        break;

    case faseCA:
        // calcula demandas p/ fases C e A
        sc = s * fat1;
        sa = s * fat2;
        // faseA
        p_faseA[npat] += sa.real();
        q_faseA[npat] += sa.imag();
        // faseC
        p_faseC[npat] += sc.real();
        q_faseC[npat] += sc.imag();
        break;
    case faseCAN:
        // faseA
        p_faseA[npat] += p / 2.;
        q_faseA[npat] += q / 2.;
        // faseC
        p_faseC[npat] += p / 2.;
        q_faseC[npat] += q / 2.;
        break;

    case faseABC:
    case faseABCN:
        // faseA
        p_faseA[npat] += p / 3.;
        q_faseA[npat] += q / 3.;
        // faseB
        p_faseB[npat] += p / 3.;
        q_faseB[npat] += q / 3.;
        // faseC
        p_faseC[npat] += p / 3.;
        q_faseC[npat] += q / 3.;
        break;
    default:
        break;
    }
    return true;
}

// ---------------------------------------------------------------------------
double __fastcall TCarregamento::GetCarregamento_kva(VTEqpto *eqpto, int npat)
{
    int pat = npat;
    double p3, q3, s3;

    // Inicia as potencias
    AlocaVetoresCargto();

    // distribui a demanda
    AnalisaCarga((VTCarga*)eqpto);

    // detremina o patamar de maior carregamento caso não foi solicitado
    if (pat < 0)
        pat = PatamarMaiorCarregamento();

    // soma as 3 fases
    p3 = p_faseA[pat] + p_faseB[pat] + p_faseC[pat];
    q3 = q_faseA[pat] + q_faseB[pat] + q_faseC[pat];

    // proteção
    if (IsDoubleZero(p3))
        p3 = 0.;
    if (IsDoubleZero(q3))
        q3 = 0.;

    s3 = sqrt(p3 * p3 + q3 * q3);

    return (s3 * 1000.);
}

// ---------------------------------------------------------------------------
void __fastcall TCarregamento::GetCarregamentoABC_kva(complex<double> *sa, complex<double> *sb,
    complex<double> *sc, int npat)
{
    int pat = npat;
    // detremina o patamar de maior carregamento caso não foi solicitado
    if (pat < 0)
        pat = PatamarMaiorCarregamento();
    // por fase
    *sa = complex<double>(p_faseA[pat], q_faseA[pat]) * 1000.;
    *sb = complex<double>(p_faseB[pat], q_faseB[pat]) * 1000.;
    *sc = complex<double>(p_faseC[pat], q_faseC[pat]) * 1000.;
}

// ---------------------------------------------------------------------------
void __fastcall TCarregamento::GetCarregamento3F_kva(complex<double> *s3, int npat)
{
    int pat = npat;
    double p3, q3;
    // detremina o patamar de maior carregamento caso não foi solicitado
    if (pat < 0)
        pat = PatamarMaiorCarregamento();

    // soma as 3 fases
    p3 = p_faseA[pat] + p_faseB[pat] + p_faseC[pat];
    q3 = q_faseA[pat] + q_faseB[pat] + q_faseC[pat];
    *s3 = complex<double>(p3, q3) * 1000.;
}

// ---------------------------------------------------------------------------
void __fastcall TCarregamento::GetPotAtiva(double *pa, double *pb, double *pc, int npat)
{
    int pat = npat;
    // detremina o patamar de maior carregamento caso não foi solicitado
    *pa = p_faseA[pat];
    *pb = p_faseB[pat];
    *pc = p_faseC[pat];

}

// ---------------------------------------------------------------------------
void __fastcall TCarregamento::GetPotReativa(double *qa, double *qb, double *qc, int npat)
{
    int pat = npat;
    // detremina o patamar de maior carregamento caso não foi solicitado
    *qa = q_faseA[pat];
    *qb = q_faseB[pat];
    *qc = q_faseC[pat];

}

// ---------------------------------------------------------------------------
void __fastcall TCarregamento::LiberaVetoresCargto(void)
{
    // verifica se os vetores foram alocados
    if (num_pat == 0)
        return;
    // destrói vetores
    if (p_faseA)
    {
        delete[]p_faseA;
        p_faseA = NULL;
    }
    if (p_faseB)
    {
        delete[]p_faseB;
        p_faseB = NULL;
    }
    if (p_faseC)
    {
        delete[]p_faseC;
        p_faseC = NULL;
    }
    if (q_faseA)
    {
        delete[]q_faseA;
        q_faseA = NULL;
    }
    if (q_faseB)
    {
        delete[]q_faseB;
        q_faseB = NULL;
    }
    if (q_faseC)
    {
        delete[]q_faseC;
        q_faseC = NULL;
    }
    // reinicia num_pat
    num_pat = 0;
}

// ---------------------------------------------------------------------------
int __fastcall TCarregamento::PatamarMaiorCarregamento(void)
{
    // variavei locais
    int pat_max = 0;
    double smax = 0.;
    double p3, q3, s3;
    VTPatamares *patamar = (VTPatamares*) apl->GetObject(__classid(VTPatamares));

    for (int pat = 0; pat < patamar->NumPatamar(); pat++)
    {
        // soma as 3 fases
        p3 = p_faseA[pat] + p_faseB[pat] + p_faseC[pat];
        q3 = q_faseA[pat] + q_faseB[pat] + q_faseC[pat];
        s3 = sqrt(p3 * p3 + q3 * q3);
        // atualiza patamar de maior carregamento
        if (s3 > smax)
        {
            smax = s3;
            pat_max = pat;
        }
    }
    return (pat_max);
}
// ---------------------------------------------------------------------------
