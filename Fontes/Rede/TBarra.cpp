// ---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TBarra.h"
#include "TCorte.h"
#include "VTEqbar.h"
#include "VTGerador.h"
#include "VTLigacao.h"
#include "VTNo.h"
#include "VTSuprimento.h"
#include "VTRede.h"
#include "VTRedes.h"
#include "VTReducao.h"
#include "VTTrafo.h"
#include "VTTrafo3E.h"
#include "VTResFlowBar.h"
#include "..\Constante\Fases.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTBarra
// ---------------------------------------------------------------------------
VTBarra* __fastcall NewObjBarra(int num_pat)
{
    return (new TBarra(num_pat));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
__fastcall TBarra::TBarra(int num_pat)
{
    // inicia dados
    vnom = 0.;
    utm.x = utm.y = 0;
    esq.x1 = esq.y1 = esq.x2 = esq.y2 = 0;
    // inicia atributos p/ visualização gráfica
    // Show[eqp_FIXO]     = true;
    Status[sttISOLADO] = true;
    // cria objetos
    resflow = NewObjResFlowBar(this, num_pat);
    lisEQBAR = new TList();
    lisLIG = new TList();
    lisNO = new TList();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
__fastcall TBarra::~TBarra(void)
{
    // destrói objetos
    if (resflow)
    {
        delete resflow;
        resflow = NULL;
    }
    // destrói lista sem destruir seus objetos
    if (lisLIG)
    {
        delete lisLIG;
        lisLIG = NULL;
    }
    if (lisNO)
    {
        delete lisNO;
        lisNO = NULL;
    }
    // destrói lista e seus objetos
    if (lisEQBAR)
    {
        LimpaTList(lisEQBAR);
        delete lisEQBAR;
        lisEQBAR = NULL;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::AreaEsquematico(int &x1, int &y1, int &x2, int &y2)
{
    // variáveis locais
    int xa, ya, xb, yb, dx, dy;
    int dim_icone;
    VTEqbar *eqbar;

    // determina área da Barra definida por suas coordenadas
    x1 = min(esq.x1, esq.x2);
    y1 = min(esq.y1, esq.y2);
    x2 = max(esq.x1, esq.x2);
    y2 = max(esq.y1, esq.y2);
    // ajusta área p/ os Eqbar conectados à Barra
    for (int n = 0; n < lisEQBAR->Count; n++)
    {
        eqbar = (VTEqbar*)lisEQBAR->Items[n];
        eqbar->CoordenadasEsquematico(xa, ya, xb, yb);
        // o ícone assume tamanho igual à metade da maior distância
        dx = abs(x2 - x1) + 1;
        dy = abs(y2 - y1) + 1;
        dim_icone = max(dx, dy) / 2;
        x1 = min(x1, min(xa, xb) - dim_icone);
        y1 = min(y1, min(ya, yb) - dim_icone);
        x2 = max(x2, max(xa, xb) + dim_icone);
        y2 = max(y2, max(ya, yb) + dim_icone);
    }
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TBarra::Clone(void)
{
    // variáveis locais
    TBarra *barra;

    // cria uma nova Barra
    if ((barra = new TBarra(resflow->Max_pat)) != NULL)
    { // copia dados desta Barra p/ a nova Barra
        barra->CopiaAtributosDe(*this);
    }
    return (barra);
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::CopiaAtributosDe(VTEqpto &ref)
{
    // variáveis locais
    TBarra &barra = (TBarra&)ref;

    // copia atributos de VTEqpto
    VTEqpto::CopiaAtributosDe(barra);
    // copia atributos de Barra
    // CopiaTList(barra.lisNO, lisNO);
    zona = barra.zona;
    // swing     = barra.swing;
    vnom = barra.vnom;
    utm = barra.utm;
    esq = barra.esq;
    esq_orig = barra.esq_orig;
    // copia atributos de VTResFlowBar
    resflow->CopiaAtributosDe(*(barra.resflow));
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
{
    x1 = esq.x1;
    y1 = esq.y1;
    x2 = esq.x2;
    y2 = esq.y2;
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
    int &x3, int &y3)
{
    // determina coordenadas x1, y1, x2, y2
    CoordenadasEsquematico(x1, y1, x2, y2);
    // define x3, y3  = (0,0)
    x3 = y3 = 0;
    // define xm, ym
    xm = (x1 + x2) / 2;
    ym = (y1 + y2) / 2;
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::CoordenadasUtm_cm(int &x, int &y)
{
    // proteção: verifica se possui coordenadas válidas
    if ((utm.x <= 0) || (utm.y <= 0))
    {
        x = y = -1;
        return (false);
    }
    x = utm.x;
    y = utm.y;
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
{
    // variáveis locais
    bool coord_valida;

    // determina coordenadas x1, y1
    coord_valida = CoordenadasUtm_cm(x1, y1);
    // define coordenadas (x2,y2) = (x1, y1)
    x2 = x1;
    y2 = y1;
    return (coord_valida);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
    int &x3, int &y3)
{
    // variáveis locais
    bool coord_valida;

    // determina coordenadas x1, y1, x2, y2
    coord_valida = CoordenadasUtm_cm(x1, y1, x2, y2);
    // define (x3, y3) = (0,0)
    x3 = y3 = 0;
    // define xm, ym
    xm = (x1 + x2) / 2;
    ym = (y1 + y2) / 2;
    return (coord_valida);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::CoordenadasUtm_m(int &x, int &y)
{
    // obtém coordenadas em cm
    if (!CoordenadasUtm_cm(x, y))
        return (false);
    // converte cm => m
    x /= 100;
    y /= 100;
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2)
{
    // variáveis locais
    bool coord_valida;

    // determina coordenadas x1, y1
    coord_valida = CoordenadasUtm_m(x1, y1);
    // define coordenadas (x2,y2) = (x1, y1)
    x2 = x1;
    y2 = y1;
    return (coord_valida);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
    int &x3, int &y3)
{
    // variáveis locais
    bool coord_valida;

    // determina coordenadas x1, y1, x2, y2
    coord_valida = CoordenadasUtm_m(x1, y1, x2, y2);
    // define x3, y3  = (0,0)
    x3 = y3 = 0;
    // define xm, ym
    xm = (x1 + x2) / 2;
    ym = (y1 + y2) / 2;
    return (coord_valida);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TBarra::DefineObjNo(VTNo *pno)
 {
 //salva ponteiro p/ objeto
 this->pno = pno;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TBarra::DefineObjZona(VTZona *zona)
{
    // salva ponteiro p/ objeto
    this->zona = zona;
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::ExisteEqbar(VTEqbar *eqbar)
{
    return (lisEQBAR->IndexOf(eqbar) >= 0);
}

// ---------------------------------------------------------------------------
VTEqbar* __fastcall TBarra::ExisteEqbar(int tipo, int id)
{
    // variáveis locais
    VTEqbar *eqbar;

    // loop p/ todos Eqbar em lisEQP
    for (int n = 0; n < lisEQBAR->Count; n++)
    { // verifica se possui o Eqbar indicado
        eqbar = (VTEqbar*)lisEQBAR->Items[n];
        if (eqbar->Tipo() != tipo)
            continue;
        if (eqbar->Id == id)
            return (eqbar);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16996614
// https://sinapsisenergia.teamwork.com/#/tasks/17643259
// https://sinapsisenergia.teamwork.com/#/tasks/17926800
bool __fastcall TBarra::ExisteGeracao(bool incluir_reducao)
{
    // variáveis locais
    VTEqpto *eqpto;
    VTReducao *reducao;
    VTLigacao *ligacao;
    VTRede *rede_lig;
    VTCorte *corte;
    TList *lisEQP_BAR, *lisREDU;

    // loop p/ todos objetos VTEqbar
    for (int n = 0; n < lisEQBAR->Count; n++)
    {
        eqpto = (VTEqpto*)lisEQBAR->Items[n];
        if (eqpto->Tipo() == eqptoGERADOR)
        {
            return (true);
        }
        if (eqpto->Tipo() == eqptoSUPRIMENTO)
        {
            return (true);
        }
    }
	if (incluir_reducao)
	{
		lisEQP_BAR = new TList();
		lisREDU = new TList();
		for (int n = 0; n < lisLIG->Count; n++)
		{
			eqpto = (VTEqpto*)lisLIG->Items[n];
			ligacao = (VTLigacao*)lisLIG->Items[n];
			// verifica se a ligação contém redução
			if (LigacaoReducao(ligacao))
			{
				rede_lig = ligacao->rede;
				rede_lig->LisCorte(lisEQP_BAR);
				for (int nc = 0; nc < lisEQP_BAR->Count; nc++)
				{
					corte = (VTCorte*)lisEQP_BAR->Items[nc];
					corte->LisReducao(lisREDU);
					for (int nr = 0; nr < lisREDU->Count; nr++)
					{
						reducao = (VTReducao*)lisREDU->Items[nr];
						if (reducao->ExisteGeracao())
						{
							delete lisEQP_BAR;
							delete lisREDU;
							return (true);
						}
					}
				}
			}
		}
		delete lisEQP_BAR;
		delete lisREDU;
	}

    return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::ExisteGeracaoPV(void)
{
    // variáveis locais
    VTEqbar *eqbar;
    VTGerador *pger;
    VTSuprimento *psup;

    // loop p/ todos Eqbar em lisEQP
    for (int n = 0; n < lisEQBAR->Count; n++)
    { // verifica possui algum Suprimento ou Gerador PV
        eqbar = (VTEqbar*)lisEQBAR->Items[n];
        if (eqbar->Tipo() == eqptoGERADOR)
        {
            pger = (VTGerador*)eqbar;
            if (pger->tiposup == supPV)
                return (true);
        }
        else if (eqbar->Tipo() == eqptoSUPRIMENTO)
        {
            psup = (VTSuprimento*)eqbar;
            if (psup->tiposup == supPV)
                return (true);
        }
    }
    return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::ExisteGeracaoVF(void)
{
    // variáveis locais
    VTEqbar *eqbar;
    VTGerador *pger;
    VTSuprimento *psup;

    // loop p/ todos Eqbar em lisEQP
    for (int n = 0; n < lisEQBAR->Count; n++)
    { // verifica possui algum Suprimento ou Gerador PV
        eqbar = (VTEqbar*)lisEQBAR->Items[n];
        if (eqbar->Tipo() == eqptoGERADOR)
        {
            pger = (VTGerador*)eqbar;
            if (pger->tiposup == supVF)
                return (true);
        }
        else if (eqbar->Tipo() == eqptoSUPRIMENTO)
        {
            psup = (VTSuprimento*)eqbar;
            if (psup->tiposup == supVF)
                return (true);
        }
    }
    return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::InsereEqbar(VTEqbar *eqbar)
{
    // proteção
    if (eqbar == NULL)
        return;
    // insere objeto VTEqbar em lisEQBAR
    if (lisEQBAR->IndexOf(eqbar) < 0)
        lisEQBAR->Add(eqbar);
    // associa Eqbar
    eqbar->DefineObjBarra(this);
    // remove Eqbar da Lixeira
    // if (redes) redes->RemoveEqptoDaLixeira(eqbar);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TBarra::InsereLigacao(VTLigacao *ligacao)
{
    // proteção
    if (ligacao == NULL)
        return;
    // insere Ligacao em lisLIG
    if (lisLIG->IndexOf(ligacao) < 0)
        lisLIG->Add(ligacao);
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::InsereNo(VTNo *no)
{
    // salva No na lista lisNO
    if (no != NULL)
        lisNO->Add(no);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::IsBarra(void)
{
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17643259
// https://sinapsisenergia.teamwork.com/#/tasks/17958402
bool __fastcall TBarra::LigacaoReducao(VTLigacao *ligacao)
{
    // se não há redes associadas => não há o que checar
    if (!ligacao->rede)
        return false;
    // se a rede não está carregada => é uma redução
    if (!ligacao->rede->Carregada)
        return true;
    // finalmente, verifica se é um trafo de redução
    if ((ligacao->Tipo() == eqptoTRAFO) && (ligacao->Codigo.Pos("reduc") > 0))
        return true;

    // se não ocorrer nenhuma das anteriores => não é uma redução
    return false;
}

// ---------------------------------------------------------------------------
TList* __fastcall TBarra::LisEqbar(void)
{
    return (lisEQBAR);
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::LisEqbar(TList *lisEQP)
{
    // variáveis locais
    VTEqpto *eqpto;

    // copia os objetos VTEqbar da Barra
    for (int n = 0; n < lisEQBAR->Count; n++)
    {
        eqpto = (VTEqpto*)lisEQBAR->Items[n];
        if (lisEQP->IndexOf(eqpto) < 0)
            lisEQP->Add(eqpto);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::LisEqbar(TList *lisEQP, int tipo)
{
    // variáveis locais
    VTEqpto *eqpto;

    // verifica o tipo de objeto VTEqbar solicitado
    if ((tipo < 0) || (tipo == eqptoEQBAR))
    { // todos os objetos VTEqbar
        LisEqbar(lisEQP);
        return;
    }
    // copia somente os objetos VTEqbar do tipo solicitado
    for (int n = 0; n < lisEQBAR->Count; n++)
    {
        eqpto = (VTEqpto*)lisEQBAR->Items[n];
        if (eqpto->Tipo() == tipo)
        { // insere objeto VTEqpto em lisEQP
            if (lisEQP->IndexOf(eqpto) < 0)
                lisEQP->Add(eqpto);
        }
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
TList* __fastcall TBarra::LisLigacao(void)
{
    return (lisLIG);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18417316
void __fastcall TBarra::LisLigacao(TList *lisEQP, int tipo)
{
    // variáveis locais
    VTEqpto *eqpto;

    // verifica o tipo de objeto VTLigacao solicitado
    if ((tipo < 0) || (tipo == eqptoLIGACAO))
    {
        // todos os objetos VTLigacao
        CopiaTList(lisLIG, lisEQP);
        return;
    }
    // copia somente os objetos VTLigacao do tipo solicitado
    for (int n = 0; n < lisLIG->Count; n++)
    {
        eqpto = (VTEqpto*)lisLIG->Items[n];
        if (eqpto->Tipo() == tipo)
        {
            // insere objeto VTEqpto em lisEQP
            // evita duplicacao se a lista não estiver 
            // inicialmente vazia
            if (lisEQP->IndexOf(eqpto) < 0)
                lisEQP->Add(eqpto);
        }
    }
}

// ---------------------------------------------------------------------------
int __fastcall TBarra::NumeroDeEqbar(int tipo)
{
    // variáveis locais
    VTEqpto *eqpto;
    int count = 0;

    // verifica se deve retornar o número total de Eqbar
    if (tipo < 0)
        return (lisEQBAR->Count);
    // identifica Eqbar do tipo indicado
    for (int n = 0; n < lisEQBAR->Count; n++)
    {
        eqpto = (VTEqpto*)lisEQBAR->Items[n];
        if (eqpto->Tipo() == tipo)
            count++;
    }
    return (count);
}

// ---------------------------------------------------------------------------
VTNo* __fastcall TBarra::PM_GetNo(VTNet *net)
{
    // variáveis locais
    VTNo *no;

    // verifica se foi passado o objeto Net
    if ((net == NULL) && (lisNO->Count == 1))
    {
        return ((VTNo*)lisNO->First());
    }
    // procura No associado a Net
    for (int n = 0; n < lisNO->Count; n++)
    {
        no = (VTNo*)lisNO->Items[n];
        if (no->net == net)
            return (no);
    }
    return (NULL);
}

// ---------------------------------------------------------------------------
double __fastcall TBarra::PM_GetVnom_kv(void)
{
    return (PD.vnom_kv);
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::PM_SetVnom_kv(double vnom_kv)
{
    PD.vnom_kv = vnom_kv;
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::PontoConexao(double posbar, int &x, int &y)
{
    // variáveis locais
    int dx, dy;

    // determina pto. de conexão
    dx = ((esq.x2 - esq.x1) * posbar) / 100.;
    dy = ((esq.y2 - esq.y1) * posbar) / 100.;
    x = esq.x1 + dx;
    y = esq.y1 + dy;
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::ReiniciaLisNo(void)
{
    lisNO->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::RemoveEqbar(VTEqbar *eqbar)
{
    // proteção
    if (eqbar == NULL)
        return;
    // retira objeto VTEqbar da lista lisEQBAR
    lisEQBAR->Remove(eqbar);
    // NÃO destrói objeto VTEqbar
    // delete eqbar;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
// https://sinapsisenergia.teamwork.com/#tasks/18261839
void __fastcall TBarra::RemoveLigacao(VTLigacao *ligacao)
{
    // proteção
    if ((ligacao == NULL) || (lisLIG == NULL))
        return;
    // retira objeto VTLigacao da lista lisLIG
    if (lisLIG->IndexOf(ligacao) >= 0)
        lisLIG->Remove(ligacao);
    // NÃO destrói objeto VTLigacao
    // delete ligacao;
}

// ---------------------------------------------------------------------------
void __fastcall TBarra::RemoveNo(VTNo *no)
{
    lisNO->Remove(no);
}

// ---------------------------------------------------------------------------
int __fastcall TBarra::Tipo(void)
{
    return (eqptoBARRA);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TBarra::TipoAsString(void)
{
    return ("Barra");
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::TipoBarra(void)
{
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBarra::Vertical(void)
{
    // variáveis locais
    int dx, dy;

    // verifica se a barra é 'vertical' ou 'horizontal'
    dx = abs(esq.x2 - esq.x1);
    dy = abs(esq.y2 - esq.y1);
    return ((dx == 0) || (dx < dy));
}

// ---------------------------------------------------------------------------
// eof
