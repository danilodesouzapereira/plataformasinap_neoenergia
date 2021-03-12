// ---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "TBalanceamento.h"
#include "TCarregamento.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Zona\VTZona.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// variável static da classe
VTDemanda* TBalanceamento::demanda = NULL;
int TBalanceamento::ind_pat_pico = 0;

// ---------------------------------------------------------------------------
VTBalanceamento* __fastcall NewObjBalanceamento(VTApl *apl_owner)
{
    try
    { // cria objeto TBalanceamento
        return (new TBalanceamento(apl_owner));
    }
    catch (Exception &e)
    {
        return (NULL);
    }
}

// ---------------------------------------------------------------------------
__fastcall TBalanceamento::TBalanceamento(VTApl *apl_owner)
{
    // variaveis locais
    VTZonas *zonas;

    // salva ponteiros p/ objetos
    this->apl = apl_owner;
    zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));
    demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
    // cria lista auxiliar
    lisEQP = new TList();
    lisFase = new TList();
    lisCARGA = new TList();
    lisCARGA_ALT = new TList();
    lisCOPIA_ALT = new TList();
    lisCOPIA_ORI = new TList();
    // cria relatorio
    // relat = new TStringList();
    // cria as fases
    TPotFase *fase1 = new TPotFase(faseA, "A");
    lisFase->Add(fase1);
    TPotFase *fase2 = new TPotFase(faseB, "B");
    lisFase->Add(fase2);
    TPotFase *fase3 = new TPotFase(faseC, "C");
    lisFase->Add(fase3);
    // separa rede por zonas
    if (!zonas)
        return;
    if (!zonas->Executa())
        return;
}

// ---------------------------------------------------------------------------
__fastcall TBalanceamento::~TBalanceamento(void)
{
    // destrói objetos
    // if (relat) {delete relat; relat = NULL;}
    // destrói listas sem destruir seus objetos
    if (lisEQP)
    {
        delete lisEQP;
        lisEQP = NULL;
    }
    if (lisCARGA)
    {
        delete lisCARGA;
        lisCARGA = NULL;
    }
    if (lisCARGA_ALT)
    {
        delete lisCARGA_ALT;
        lisCARGA_ALT = NULL;
    }
    // destrói listas e seus objetos
    if (lisFase)
    {
        LimpaLista(lisFase);
        delete lisFase;
        lisFase = NULL;
    }
    if (lisCOPIA_ALT)
    {
        LimpaTList(lisCOPIA_ALT);
        delete lisCOPIA_ALT;
        lisCOPIA_ALT = NULL;
    }
    if (lisCOPIA_ORI)
    {
        LimpaTList(lisCOPIA_ORI);
        delete lisCOPIA_ORI;
        lisCOPIA_ORI = NULL;
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::AlteraRede(TList *lisCAR)
{
    // variáveis locais
    VTCarga *carga_alt, *carga_ori;
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

    try
    { // verifica se há Cargas alteradas
        if (lisCAR->Count == 0)
            return (true);
        // lisCAR contém as Cargas alteradas (associadas com as respectivas Carga originais)
        // monta lisEQP com as Carga originais associadas com suas respectivas Cargas alteradas
        lisEQP->Clear();
        for (int n = 0; n < lisCAR->Count; n++)
        {
            carga_alt = (VTCarga*)lisCAR->Items[n];
            carga_ori = (VTCarga*)carga_alt->Obj;
            // associa Carga original com a Carga alterada (necesssário p/ TEdita)
            carga_ori->Obj = carga_alt;
            // cancela associação da Carga alterada c/ sua Carga original
            carga_alt->Obj = NULL;
            // insere Carga original em lisEQP
            lisEQP->Add(carga_ori);
        }
        // limpa lisCAR sem destruir seus objetos
        lisCAR->Clear();
        // altera a rede
        edita->AlteraLisEqpto(lisEQP);
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TBalanceamento::AnalisaFase(VTCarga *carga, int fase_nova)
 {
 //variáveis locais
 //VTNo     *no    = carga->pbarra->No[net];
 VTZona   *zona  = carga->pbarra->zona;
 VTFases  *fases = (VTFases*) apl->GetObject(__classid(VTFases));

 //verifica se a zona da Barra comporta a nova fase
 if (zona != NULL)
 {
 if (! fases->Fase1ContemFase2(zona->Fases, fase_nova)) return(false);
 }
 //verifica se o No da Barra comporta a nova fase
 //   if (no != NULL)
 //      {
 //      return(fases->Fase1ContemFase2(no->Fases, fase_nova));
 //      }
 //verifica se a Barra possui as novas fases
 return(fases->Fase1ContemFase2(carga->pbarra->resflow->Fases, fase_nova));
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::AnalisaFase(VTCarga *carga, int fase_nova)
{
    // variáveis locais
    VTNo *no = carga->pbarra->No[NULL];
    VTZona *zona = carga->pbarra->zona;
    VTFases *fases = (VTFases*) apl->GetObject(__classid(VTFases));

    // verifica se a zona da Barra comporta a nova fase
    if (zona != NULL)
    {
        if (!fases->Fase1ContemFase2(zona->Fases, fase_nova))
            return (false);
    }
    // verifica se o No da Barra comporta a nova fase
    if (no != NULL)
    {
        return (fases->Fase1ContemFase2(no->Fases, fase_nova));
    }
    return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::CriaCargaAlterada(VTCarga *carga, int fase_nova)
{
    // variáveis locais
    VTCarga *carga_alt, *copia_alt;

    try
    { // cria uma cópia da Carga original, altera suas fases, associa com Carga original
        // e salva em  lisCARGA_ALT
        carga_alt = carga->Clone();
        // verifica se a Carga possui curva própria
        if (carga_alt->DefinidaPorFase)
        { // altera a curva da Craga
            RedefineCurvaCargaDefinidaPorFase(carga_alt, fase_nova);
        }
        // redefine fases da Carga
        carga_alt->Fases = fase_nova;
        carga_alt->Obj = carga;
        lisCARGA_ALT->Add(carga_alt);
        // cria uma cópia da Carga alterada, associa com Carga original e salva em  lisCOPIA_ALT
        copia_alt = carga_alt->Clone();
        copia_alt->Obj = carga;
        lisCOPIA_ALT->Add(copia_alt);
        return (true);
    }
    catch (Exception &e)
    {
        return (false);
    }
}

/*
 //---------------------------------------------------------------------------
 int  __fastcall TBalanceamento::ComparaCarregamento(void *carga1, void *carga2)
 {
 //variáveis locais
 double   s1, s2;

 //calcular o carregamentos das cargas
 TCarregamento carreg(TBalanceamento::apl);
 s1 = carreg.GetCarregamento((VTEqpto*) carga1);
 s2 = carreg.GetCarregamento((VTEqpto*) carga2);

 if(s1 > s2) return (1);
 if(s1 < s2) return(-1);
 return (0);
 }
 */
// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::ComparaCarregamento(void *carga1, void *carga2)
{
    // variáveis locais
    double s1, s2;

    // compara demanda das Cargas no patamar de pico
    s1 = Abs(demanda->S_mva[(VTCarga*)carga1][ind_pat_pico]);
    s2 = Abs(demanda->S_mva[(VTCarga*)carga2][ind_pat_pico]);

    if (s1 > s2)
        return (1);
    if (s1 < s2)
        return (-1);
    return (0);
}

// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::ComparaCarregamentoPorFase(void *fase1, void *fase2)
{
    // variáveis locais
    double s1, s2;
    TPotFase *f1 = (TPotFase*)fase1;
    TPotFase *f2 = (TPotFase*)fase2;

    // determina o carregamento da fase correspondente
    s1 = sqrt((f1->p * f1->p) + (f1->q * f1->q));
    s2 = sqrt((f2->p * f2->p) + (f2->q * f2->q));
    // faz a comparação
    if (s1 > s2)
        return (1);
    if (s1 < s2)
        return (-1);
    return (0);
}

// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::DeterminaFasesCarga(VTCarga *carga)
{
    // variáveis locais
    int fase1, fase2;
    int fase_nova = carga->Fases;

    // verifica se carga monofásica, bifásica ou trifásica
    switch (carga->MBTfasica)
    {
    case cargaMONOFASICA:
        // tenta transferir Carga para fase menos carregada
        fase_nova = PrimeiraFase();
        // verifica se a Carga pode ser transferida p/ a fase (a fase precisa ser disponível na Barra)
        if (!AnalisaFase(carga, fase_nova))
        {
            fase_nova = SegundaFase();
            if (!AnalisaFase(carga, fase_nova))
            {
                fase_nova = TerceiraFase();
            }
        }
        break;
    case cargaBIFASICA:
        // tenta transferir Carga para as duas fases menos carregadas
        fase1 = PrimeiraFase();
        fase2 = SegundaFase();
        // verifica se a Carga pode ser transferida p/ a fase1 (a fase1 precisa ser disponível na Barra)
        if (!AnalisaFase(carga, fase1))
            fase1 = TerceiraFase();
        // verifica se a Carga pode ser transferida p/ a fase2 (a fase2 precisa ser disponível na Barra)
        if (!AnalisaFase(carga, fase2))
            fase2 = TerceiraFase();
        // compõe as fases
        fase_nova = fase1 | fase2;
        break;
    case cargaTRIFASICA:
        // não há o que fazer: mantém as mesmas fases
        fase_nova = carga->Fases;
        break;
    default: // mantém as mesmas fases
        fase_nova = carga->Fases;
        break;
    }
    // inclui neutro e terra caso a fase original também tenha
    if (carga->Fases & faseN)
        fase_nova |= faseN;
    if (carga->Fases & faseT)
        fase_nova |= faseT;

    return (fase_nova);
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::DistribuiCargaMonofasica(VTCarga *carga, int fases_carga)
{
    // variaveis locais:
    TPotFase *pot;
    double p = demanda->P_mw[carga][ind_pat_pico];
    double q = demanda->Q_mvar[carga][ind_pat_pico];

    // if (carga->Codigo.AnsiCompareIC("16648") == 0)
    // {int a=0;}

    // acumula as demandas por fase (desconsidera neutro e terra)
    switch (fases_carga & faseABC)
    {
    case faseA: // fase A
        pot = GetFaseA();
        pot->p += p;
        pot->q += q;
        break;

    case faseB: // fase B
        pot = GetFaseB();
        pot->p += p;
        pot->q += q;
        break;

    case faseC: // fase C
        pot = GetFaseC();
        pot->p += p;
        pot->q += q;
        break;

    default: // nada a fazer
        return (false);
    }
    return true;
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TBalanceamento::DistribuiCargaBifasica(VTCarga *carga, int fases_carga)
 {
 //variaveis locais
 TPotFase *pot;
 double    p = demanda->P_mw  [carga][ind_pat_pico];
 double    q = demanda->Q_mvar[carga][ind_pat_pico];

 //acumula as demandas bifasicas (desconsidera terra)
 switch (fases_carga & faseABCN)
 {
 case faseAB:
 //fase A
 pot     = GetFaseA();
 pot->p += p;
 pot->q += q;
 //fase B
 pot     = GetFaseB();
 pot->p += p;
 pot->q += q;
 break;
 case faseBC:
 //fase B
 pot     = GetFaseB();
 pot->p += p;
 pot->q += q;
 //fase C
 pot     = GetFaseC();
 pot->p += p;
 pot->q += q;
 break;
 case faseCA:
 //fase C
 pot     = GetFaseC();
 pot->p += p;
 pot->q += q;
 //fase A
 pot     = GetFaseA();
 pot->p += p;
 pot->q += q;
 break;
 case faseABN:
 //fase A
 pot     = GetFaseA();
 pot->p += p/2.;
 pot->q += q/2.;
 //fase B
 pot     = GetFaseB();
 pot->p += p/2.;
 pot->q += q/2.;
 break;
 case faseBCN:
 //fase B
 pot     = GetFaseB();
 pot->p += p/2.;
 pot->q += q/2.;
 //fase C
 pot     = GetFaseC();
 pot->p += p/2.;
 pot->q += q/2.;
 break;
 case faseCAN:
 //fase C
 pot     = GetFaseC();
 pot->p += p/2.;
 pot->q += q/2.;
 //fase A
 pot     = GetFaseA();
 pot->p += p/2.;
 pot->q += q/2.;
 break;
 default: //nada a fazer
 return(false);
 }
 return true;
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::DistribuiCargaBifasica(VTCarga *carga, int fases_carga)
{
    // variaveis locais
    TPotFase *pot;
    complex<double>sa, sb, sc;
    complex<double>fat1 = polar(1.0 / sqrt(3.0), -M_PI / 6);
    complex<double>fat2 = polar(1.0 / sqrt(3.0), M_PI / 6);
    complex<double>s = demanda->S_mva[carga][ind_pat_pico];

    // acumula as demandas bifasicas (desconsidera terra)
    switch (fases_carga & faseABCN)
    {
    case faseAB:
        // calcula demandas p/ fases A e B
        sa = s * fat1;
        sb = s * fat2;
        // fase A
        pot = GetFaseA();
        pot->p += sa.real();
        pot->q += sa.imag();
        // fase B
        pot = GetFaseB();
        pot->p += sb.real();
        pot->q += sb.imag();
        break;
    case faseBC:
        // calcula demandas p/ fases B e C
        sb = s * fat1;
        sc = s * fat2;
        // fase B
        pot = GetFaseB();
        pot->p += sb.real();
        pot->q += sb.imag();
        // fase C
        pot = GetFaseC();
        pot->p += sc.real();
        pot->q += sc.imag();
        break;
    case faseCA:
        // calcula demandas p/ fases C e A
        sc = s * fat1;
        sa = s * fat2;
        // fase C
        pot = GetFaseC();
        pot->p += sc.real();
        pot->q += sc.imag();
        // fase A
        pot = GetFaseA();
        pot->p += sa.real();
        pot->q += sa.imag();
        break;
    case faseABN:
        // calcula demandas p/ fases A e B
        sa = sb = s * 0.5;
        // fase A
        pot = GetFaseA();
        pot->p += sa.real();
        pot->q += sa.imag();
        // fase B
        pot = GetFaseB();
        pot->p += sb.real();
        pot->q += sb.imag();
        break;
    case faseBCN:
        // calcula demandas p/ fases B e C
        sb = sc = s * 0.5;
        // fase B
        pot = GetFaseB();
        pot->p += sb.real();
        pot->q += sb.imag();
        // fase C
        pot = GetFaseC();
        pot->p += sc.real();
        pot->q += sc.imag();
        break;
    case faseCAN:
        // calcula demandas p/ fases C e A
        sc = sa = s * 0.5;
        // fase C
        pot = GetFaseC();
        pot->p += sc.real();
        pot->q += sc.imag();
        // fase A
        pot = GetFaseA();
        pot->p += sa.real();
        pot->q += sa.imag();
        break;
    default: // nada a fazer
        return (false);
    }
    return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::DistribuiCargaTrifasica(VTCarga *carga, int fases_carga)
{
    // variaveis locais:
    TPotFase *pot;
    double p = demanda->P_mw[carga][ind_pat_pico]; // obtem a demanda total da carga
    double q = demanda->Q_mvar[carga][ind_pat_pico];

    // acumula a demanda do instante de pico nas 3 fases
    // fase A
    pot = GetFaseA();
    pot->p += p / 3.;
    pot->q += q / 3.;
    // fase B
    pot = GetFaseB();
    pot->p += p / 3.;
    pot->q += q / 3.;
    // fase C
    pot = GetFaseC();
    pot->p += p / 3.;
    pot->q += q / 3.;

    return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::DistribuiDemandaPorFase(VTCarga *carga, int fases_carga)
{
    // variáveis locais
    bool resp;

    // verifica o tipo de ligação
    switch (carga->MBTfasica)
    {
    case cargaMONOFASICA:
        resp = DistribuiCargaMonofasica(carga, fases_carga);
        break;
    case cargaBIFASICA:
        resp = DistribuiCargaBifasica(carga, fases_carga);
        break;
    case cargaTRIFASICA:
        resp = DistribuiCargaTrifasica(carga, fases_carga);
        break;
    default:
        resp = false;
        break;
    }
    return (resp);
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::Executa(VTRede *rede)
{
    // varivaeis locais
    // AnsiString    linha;//, ordenacao;
    int fase_nova;
    VTCarga *carga, *copia_ori;
    VTNet *net;
    VTProgresso *prog;
    VTOrdena *ordena = (VTOrdena*) apl->GetObject(__classid(VTOrdena));

    // VTFases       *fases  = (VTFases*) apl->GetObject(__classid(VTFases));
    TCarregamento carreg(apl);

    //limpa listas
    LimpaTList(lisCARGA_ALT);
    LimpaTList(lisCOPIA_ALT);
    LimpaTList(lisCOPIA_ORI);

    // obtem lista de cargas da rede
    lisCARGA->Clear();
    rede->LisEqbar(lisCARGA, eqptoCARGA);
    if (lisCARGA->Count == 0)
        return false;
    /*
     //TESTES
     //elimina cargas bifásicas
     for (int n = lisCARGA->Count-1; n >= 0; n--)
     {
     carga = (VTCarga*)lisCARGA->Items[n];
     if (carga->MBTfasica == cargaBIFASICA)
     {
     lisCARGA->Delete(n);
     carga->pbarra->RemoveEqbar(carga);
     //delete carga;
     }
     }
     */

    // verifica se a rede possui delta aberto/fechado //DVK 2015.08.05
    if (!RedeValida(rede))
        return false;
    // ordena rede
    // net = ordena->Executa(rede->Redes, false);
    // ordena->ChaveEnabled   = false;
    // ordena->IsoladoEnabled = false;
    // ordena->TramoEnabled   = false;
    net = ordena->Executa(rede->Redes);
    // obtem patamar de maior carregamento
    carreg.AnalisaRede(rede);
    ind_pat_pico = carreg.PatamarMaiorCarregamento();
    // cria progresso
    prog = DLL_NewObjProgresso(NULL, NULL);
    if (prog)
        prog->Start(progTEXTO);
    if (prog)
        prog->Add("Balanceamento de cargas");
    // ordena lista de cargas em ordem decrescente
    if (prog)
        prog->Add("Ordenação de cargas");
    OrdenaCargas(lisCARGA);
    // zera os contatores e potencias
    Inicia();
    // loop para todas cargas: distribuir a demanda por fase no instante de ind_pat_pico
    if (prog)
        prog->Add("Distribuição de cargas por demanda");
    for (int n = lisCARGA->Count - 1; n >= 0; n--)
    {
        carga = (VTCarga*)lisCARGA->Items[n];
        // cria uma cópia da Carga original e salva em lisCOPIA_ORI
        copia_ori = carga->Clone();
        lisCOPIA_ORI->Add(copia_ori);
        // monta informações iniciais da carga
        // linha = MontaRelCarga(carga);
        // determina carregamento das fases
        OrdenaFases(lisFase);
        // determina novas fases da Carga, em função dos carregamentos das fases da rede
        fase_nova = DeterminaFasesCarga(carga);
        if (fase_nova != carga->Fases)
        { // cria um cópia da Carga com as novas fases e insere lisCARGA_ALT para posterior
            CriaCargaAlterada(carga, fase_nova);
        }
        // inclui demanda da Carga no carregamento das fases da rede
        DistribuiDemandaPorFase(carga, fase_nova);
        // monta informações fase da carga
        // ordenacao  = fases->AsStrABCN[(GetPotFase(0))->tag];
        // ordenacao += fases->AsStrABCN[(GetPotFase(1))->tag];
        // ordenacao += fases->AsStrABCN[(GetPotFase(2))->tag];
        // relat->Add(linha + ";" + fases->AsStrABCN[fase_nova]);

        // código temporário: altera fases da Carga
        // int temporario=0;
        // carga->Fases = fase_nova;

    }
    if (prog)
        prog->Stop();

    // destrói objetos
    if (net)
    {
        DLL_DeleteEqpto(net);
    }
    if (prog)
    {
        delete prog;
    }
    // altera Cargas cujas fases foram modificadas
    AlteraRede(lisCARGA_ALT);
    return true;
}

// ---------------------------------------------------------------------------
double __fastcall TBalanceamento::GetCarregamento(int tag)
{
    TPotFase *fase = GetFase(tag);
    if (fase != NULL)
    {
        return sqrt(fase->p*fase->p + fase->q*fase->q);
    }
    return (0.);
}

// ---------------------------------------------------------------------------
TPotFase* __fastcall TBalanceamento::GetFase(int tag)
{
    // var local
    TPotFase *fase = NULL;

    for (int n = 0; n < lisFase->Count; n++)
    {
        fase = (TPotFase*) lisFase->Items[n];
        if (fase->tag == tag)
            break;
    }
    return (fase);
}

// ---------------------------------------------------------------------------
TPotFase* __fastcall TBalanceamento::GetFaseA(void)
{
    return (GetFase(faseA)); // A = 1
}

// ---------------------------------------------------------------------------
TPotFase* __fastcall TBalanceamento::GetFaseB(void)
{
    return (GetFase(faseB)); // B = 2

}

// ---------------------------------------------------------------------------
TPotFase* __fastcall TBalanceamento::GetFaseC(void)
{
    return (GetFase(faseC)); // C = 4
}

// ---------------------------------------------------------------------------
TPotFase* __fastcall TBalanceamento::GetPotFase(int index)
{
    if (index >= 0 && index < lisFase->Count)
    {
        return ((TPotFase*)lisFase->Items[index]);
    }
    return NULL;
}

// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::IndicePatamarPico(void)
{
    return (ind_pat_pico);
}

// ---------------------------------------------------------------------------
void __fastcall TBalanceamento::Inicia(void)
{
    // var local
    TPotFase *pot;

    pot = GetFaseA();
    pot->Inicia();
    pot = GetFaseB();
    pot->Inicia();
    pot = GetFaseC();
    pot->Inicia();

    // relat->Clear();

}

// ---------------------------------------------------------------------------
TList* __fastcall TBalanceamento::LisCarga(void)
{
    return (lisCARGA);
}

// ---------------------------------------------------------------------------
TList* __fastcall TBalanceamento::LisCloneCargaAlterada(void)
{
    return (lisCOPIA_ALT);
}

// ---------------------------------------------------------------------------
TList* __fastcall TBalanceamento::LisCloneCargaOriginal(void)
{
    return (lisCOPIA_ORI);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TBalanceamento::MontaRelFinal(VTRede *rede)
{
    // var local
    AnsiString linha = NULL;
    TPotFase *faseA = GetFaseA();
    TPotFase *faseB = GetFaseB();
    TPotFase *faseC = GetFaseC();
    double p3 = faseA->p + faseB->p + faseC->p;
    double q3 = faseA->q + faseB->q + faseC->q;

    // id cód  PA  PB  PC  QA  QB  QC  SA  SB  SC St Fase SA  SB  SC  Fase NOVA
    linha.sprintf("%d; %s; %f; %f; %f; %f; %f; %f; %f; %f; %f; %f; ; %f; %f; %f", rede->Id,
        rede->Codigo.c_str(), faseA->p, faseB->p, faseC->p, faseA->q, faseB->q, faseC->q,
        GetCarregamento(1), GetCarregamento(2), GetCarregamento(4), sqrt(p3*p3 + q3*q3),
        GetCarregamento(1), GetCarregamento(2), GetCarregamento(4));
    return (linha);
}

// ---------------------------------------------------------------------------
void __fastcall TBalanceamento::LimpaLista(TList *lista)
{
    TPotFase *item;
    for (int n = lista->Count - 1; n >= 0; n--)
    {
        item = (TPotFase*) lista->Items[n];
        if (item)
        {
            delete item;
            item = NULL;
        }
    }
}

// ---------------------------------------------------------------------------
void __fastcall TBalanceamento::OrdenaCargas(TList *lisCARGA)
{
    // QuickSort
    lisCARGA->Sort(ComparaCarregamento);

}

// ---------------------------------------------------------------------------
void __fastcall TBalanceamento::OrdenaFases(TList *listFase)
{
    // QuickSort
    listFase->Sort(ComparaCarregamentoPorFase);

}

// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::PrimeiraFase(void)
{
    // var local
    TPotFase *fase;

    if (lisFase->Count == 0)
        return -1;
    fase = (TPotFase*) lisFase->Items[0];
    return (fase->tag);
}

// ---------------------------------------------------------------------------
void __fastcall TBalanceamento::RedefineCurvaCargaDefinidaPorFase(VTCarga *carga, int fase_nova)
{
    // variáveis locais
    int fase_ori, fase_alt, fase_comum;
    int ind_ori, ind_alt;
    complex<double>s_mva[MAX_FASE];
    VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
    VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

    // verifica se a Carga é trifásica
    if (carga->MBTfasica == cargaTRIFASICA)
        return;
    // verifica se a Carga é monofásica
    if (carga->MBTfasica == cargaMONOFASICA)
    { // determina qual fase foi alterada
        fase_ori = carga->Fases & faseABC;
        fase_alt = fase_nova & faseABC;
    }
    // verifica se a Carga é monofásica
    else if (carga->MBTfasica == cargaBIFASICA)
    { // determina fase comum entre as fases originais e as fases alteradas
        fase_comum = (carga->Fases & fase_nova) & faseABC;
        // determina qual fase foi alterada
        fase_ori = (carga->Fases & (~fase_comum)) & faseABC;
        fase_alt = (fase_nova & (~fase_comum)) & faseABC;
    }
    // altera a Curva da Carga, movendo os valores de fase_ori para fase_alt
    ind_ori = fases->TagToIndex[fase_ori];
    ind_alt = fases->TagToIndex[fase_alt];
    // loop p/ todos patamares
    for (int np = 0; np < carga->curva->NumeroPontos(); np++)
    {
        demanda->Get_S3f_mva(carga, np, s_mva);
        // copia os valor de ind_ori para ind_alt
        s_mva[ind_alt] = s_mva[ind_ori];
        // zera o valor de ind_ori
        s_mva[ind_ori] = CZero();
        // atualiza demanda da Carga
        demanda->Set_S3f_mva(carga, np, s_mva);
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TBalanceamento::RedeValida(VTRede *rede)
{ // DVK 2015.08.05
    // variaveis locais
    AnsiString tipo_zona;

    // determina o tipo da zona
    tipo_zona = TipoZona(rede);
    if (tipo_zona.AnsiCompareIC("DELTA_A") == 0)
    { // verifica se há delta aberto ou fechado
        Aviso("A rede " + rede->Codigo + " possui transformadores ligados em delta aberto");
        return (false);
    }
    if (tipo_zona.AnsiCompareIC("DELTA_F") == 0)
    {
        Aviso("A rede " + rede->Codigo + " possui transformadores ligados em delta fechado");
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::SegundaFase(void)
{
    // var local
    TPotFase *fase;

    if (lisFase->Count == 0)
        return -1;
    fase = (TPotFase*) lisFase->Items[1];
    return (fase->tag);

}

// ---------------------------------------------------------------------------
int __fastcall TBalanceamento::TerceiraFase(void)
{
    // var local
    TPotFase *fase;

    if (lisFase->Count == 0)
        return -1;
    fase = (TPotFase*) lisFase->Items[2];
    return (fase->tag);

}

// ---------------------------------------------------------------------------
AnsiString __fastcall TBalanceamento::TipoZona(VTRede *rede)
{
    // variaveis locais
    int tipo_zona, ind_bar;
    VTBarra *barra, *barra_ini;
    VTLigacao *ligacao;

    // proteção
    if (!rede)
        return ("INV");
    // localiza barra e ligacao inicial da rede
    barra_ini = rede->BarraInicial();
    ligacao = rede->ExisteLigacao(barra_ini);
    if (!ligacao)
        return ("INV");
    // localiza outra barra da ligacao
    ind_bar = ligacao->IndiceBarra(barra_ini);
    barra = ligacao->Barra(1 - ind_bar);
    if (!barra)
        return ("INV");
    if (!barra->zona)
        return ("INV");
    // determina tipo da zona
    tipo_zona = barra->zona->Tipo;
    switch (tipo_zona)
    { // traduz tipo da zona em string
    case zonaTRIFASICA:
        return ("TRIF");
    case zonaMONOFASICA:
        return ("MONOF");
    case zonaDELTA_ABERTO:
        return ("DELTA_A");
    case zonaDELTA_FECHADO:
        return ("DELTA_F");
    default:
        break;
    }
    // qualquer outra zona é inválida
    return ("INV");
}

// ---------------------------------------------------------------------------
