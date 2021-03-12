//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <math.h>
#include "TLigacao.h"
#include "VTBarra.h"
#include "VTResFlowLig.h"
#include "..\Constante\Fases.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
__fastcall TLigacao::TLigacao(void)
   {
   //inicia dados
   esq.posbar1 = 50.;
   esq.posbar2 = 50.;
   esq.posbar3 = 50.;
   resflow     = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TLigacao::~TLigacao(void)
   {
   //destrói objetos
   if (resflow) {delete resflow; resflow = NULL;}
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLigacao::Alias(int ind_bar)
   {
   //variáveis locais
   VTBarra    *barra;
   AnsiString txt;

   //monta alias
   txt.sprintf("B%d:", ind_bar+1);
   //determina barra
   if ((barra = Barra(ind_bar)) != NULL) txt += "(" + barra->Codigo + ")";
   return(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::AlocaResFlowLig(int num_bar, int num_pat)
   {
   //destrói eventual VTResFlowLig existente
   if (resflow) delete resflow;
   resflow = DLL_NewObjResFlowLig(this, num_bar, num_pat);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TLigacao::Barra(int ind_bar)
   {
   switch(ind_bar)
      {
      case 0: return(pbarra1);
      case 1: return(pbarra2);
      case 2: return(pbarra3);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TLigacao::BarraComum(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

	//proteção
	if (ligacao == NULL) return(NULL);
   //determina Barra que conecta esta Ligacao com a Ligacao indicada
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = Barra(ind_bar)) == NULL) continue;
      if (ligacao->ConectaBarra(barra)) return(barra);
      }
	return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TLigacao::ComprimentoEsq(void)
   {
   //variáveis locais
   int    x1, y1, x2, y2;

   //obtém coordenadas utm
   CoordenadasEsquematico(x1, y1, x2, y2);
   //calcula comprimento
   return(Distancia_cm(x1, y1, x2, y2));
   }


//---------------------------------------------------------------------------
int __fastcall TLigacao::ComprimentoUtm_cm(void)
   {
   //variáveis locais
   int    x1, y1, x2, y2;

   //obtém coordenadas utm
   CoordenadasUtm_cm(x1, y1, x2, y2);
   //calcula comprimento
   return(Distancia_cm(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::ConectaBarra(VTBarra *pbarra)
   {
   //protecao:
   if (pbarra == NULL)    return(false);
   if (pbarra == pbarra1) return(true);
   if (pbarra == pbarra2) return(true);
   if (pbarra == pbarra3) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
   {
   if (pbarra1) pbarra1->PontoConexao(esq.posbar1, x1, y1);
   if (pbarra2) pbarra2->PontoConexao(esq.posbar2, x2, y2);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
                                                 int &x2, int &y2, int &x3, int &y3)
   {
   if (pbarra1) pbarra1->PontoConexao(esq.posbar1, x1, y1);
   if (pbarra2) pbarra2->PontoConexao(esq.posbar2, x2, y2);
   if (pbarra3) pbarra3->PontoConexao(esq.posbar3, x3, y3);
   //verifica se existe a terceira Barra
   if (pbarra3 == NULL)
      {//existem somente duas Barras
      xm = (x1 / 2) + (x2 / 2);
      ym = (y1 / 2) + (y2 / 2);
      }
   else
      {//existem três Barras
      xm = (min(min(x1, x2), x3) / 2) + (max(max(x1, x2),x3) / 2);
      ym = (min(min(y1, y2), y3) / 2) + (max(max(y1, y2),y3) / 2);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::CoordenadasUtm_cm(int &x, int &y)
   {
   if      (pbarra1) return(pbarra1->CoordenadasUtm_cm(x, y));
   else if (pbarra2) return(pbarra2->CoordenadasUtm_cm(x, y));
   else if (pbarra3) return(pbarra3->CoordenadasUtm_cm(x, y));
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
   {
   //proteção
   if ((pbarra1 == NULL)||(pbarra2 == NULL))  return(false);
   if (! pbarra1->CoordenadasUtm_cm(x1, y1)) return(false);
   if (! pbarra2->CoordenadasUtm_cm(x2, y2)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
                                            int &x2, int &y2, int &x3, int &y3)
   {
   //obtém coordenadas das barrras 1 e 2
   if (! CoordenadasUtm_cm(x1, y1, x2, y2)) return(false);
   //verifica se existe a terceira barra
   if (pbarra3 == NULL)
      {//existem somente duas Barras
      xm = (x1 / 2) + (x2 / 2);
      ym = (y1 / 2) + (y2 / 2);
      //define coordenadas da Barra3 iguais a xm, ym
      x3 = xm;
      y3 = ym;
      }
   else
      {//determina coord. na terceira barra
      if (! pbarra3->CoordenadasUtm_cm(x3, y3)) return(false);
      xm = (min(min(x1, x2), x3) / 2) + (max(max(x1, x2),x3) / 2);
      ym = (min(min(y1, y2), y3) / 2) + (max(max(y1, y2),y3) / 2);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::CoordenadasUtm_m(int &x, int &y)
   {
   //determina coord. em cm
   if (! CoordenadasUtm_cm(x, y)) return(false);
   //converte cm p/ metro
   x /= 100;
   y /= 100;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2)
   {
   //determina coordenadas em cm
   if (! CoordenadasUtm_cm(x1, y1, x2, y2)) return(false);
   //converte cm p/ metro
   x1 /= 100;
   y1 /= 100;
   x2 /= 100;
   y2 /= 100;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
                                           int &x2, int &y2, int &x3, int &y3)
   {
   //determina coordenadas em cm
   if (! CoordenadasUtm_cm(xm, ym, x1, y1, x2, y2, x3, y3)) return(false);
   //converte cm p/ metro
   xm /= 100;  ym /= 100;
   x1 /= 100;  y1 /= 100;
   x2 /= 100;  y2 /= 100;
   x3 /= 100;  y3 /= 100;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   VTLigacao &ligacao = (VTLigacao&)ref;

   //proteção
   if (! ref.TipoLigacao()) return;
   //copia atributos de VTEqpto
   VTEqpto::CopiaAtributosDe(ligacao);
   //copia atributos de VTLigacao
   //2011.09.05 - NÃO copia ponteiro da rede p/ evitar erro nas operacões de
   //UndoCompleto e RedoCompleto
   //rede    = ligacao.rede;
   //reintroduzido em 2011.09.25
   rede    = ligacao.rede;
   pbarra1 = ligacao.pbarra1;
   pbarra2 = ligacao.pbarra2;
   pbarra3 = ligacao.pbarra3;
   ligaPai = ligacao.ligaPai;
   esq     = ligacao.esq;
   //copia atributos de VTResFlowLig
   resflow->CopiaAtributosDe(*ligacao.resflow);
   }

//---------------------------------------------------------------------------
int __fastcall TLigacao::Declividade(void)
   {
   //variáveis locais
   int x1, y1, x2, y2;

   //determina coordenadas
   CoordenadasEsquematico(x1, y1, x2, y2);
   //verifica se a barra é horizontal
   if (y1 == y2) return(ligaHORIZONTAL);
   //verifica se a barra é vertical
   if (x1 == x2) return(ligaVERTICAL);
   //verifica se é diagonal ascendente
   if ((x1 < x2)&&(y1 < y2)) return(ligaDIAG_ASCENDENTE);
   //diagonal descendente
   return(ligaDIAG_DESCENDENTE);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TLigacao::DefineObjBarra(VTBarra *pbar1, VTBarra *pbar2, VTBarra *pbar3)
   {
   //retira Ligacao das Barras anteriores
   if (pbarra1 != NULL) pbarra1->RemoveLigacao(this);
   if (pbarra2 != NULL) pbarra2->RemoveLigacao(this);
   if (pbarra3 != NULL) pbarra3->RemoveLigacao(this);
   //salva ponteiro p/ objetos
   pbarra1 = pbar1;
   pbarra2 = pbar2;
   pbarra3 = pbar3;
   //insere Ligacao das Barras anteriores
   if (pbarra1 != NULL) pbarra1->InsereLigacao(this);
   if (pbarra2 != NULL) pbarra2->InsereLigacao(this);
   if (pbarra3 != NULL) pbarra3->InsereLigacao(this);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::DefineObjRede(VTRede *rede)
   {
   //verifica se há mudança de Rede
   if ((this->rede != NULL)&&(this->rede != rede))
      {//atualiza status indicando que houve alteração do Eqpto
      DefineStatus(sttALTERADO, true);
      }
   //salva ponteiro p/ objetos
   this->rede = rede;
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::DefinePosBar(VTBarra *pbar, double posbar)
   {
   if      (pbar == pbarra1) esq.posbar1 = posbar;
   else if (pbar == pbarra2) esq.posbar2 = posbar;
   else if (pbar == pbarra3) esq.posbar3 = posbar;
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::DefinePosBar(VTBarra *pbar, int x, int y)
   {
   //variáveis locais
   double posbar;
   int    dx, dy;

   //determina dx, dy
   dx = pbar->esq.x2 - pbar->esq.x1;
   dy = pbar->esq.y2 - pbar->esq.y1;
   //verifica se Barra vertical ou horizontal
   if (pbar->Vertical())
      {//calcula posição na Barra (0 a 100%)
      if (dy == 0)
         {posbar = 50;}
      else
         {posbar = ((y - pbar->esq.y1)*100.)/dy;}
      }
   else
      {//barra mais horizontal
      if (dx == 0)
         {posbar = 50;}
      else
         {posbar = ((x - pbar->esq.x1)*100.)/dx;}
      }
   //proteção
   if (posbar > 100.) posbar = 100.;
   if (posbar < 0.)   posbar = 0.;
   //atualiza posição
   if      (pbar == pbarra1) esq.posbar1 = posbar;
   else if (pbar == pbarra2) esq.posbar2 = posbar;
   else if (pbar == pbarra3) esq.posbar3 = posbar;
   }

//---------------------------------------------------------------------------
int __fastcall TLigacao::Fases(VTBarra *barra)
   {
   return(faseINV);
   }

//---------------------------------------------------------------------------
int __fastcall TLigacao::IndiceBarra(int barra_id)
   {
   if      ((pbarra1 != NULL)&&(pbarra1->Id == barra_id)) return(0);
   else if ((pbarra2 != NULL)&&(pbarra2->Id == barra_id)) return(1);
   else if ((pbarra3 != NULL)&&(pbarra3->Id == barra_id)) return(2);
   return(-1);
   }

//---------------------------------------------------------------------------
int __fastcall TLigacao::IndiceBarra(VTBarra *pbar)
   {
   if      (pbar == NULL)    return(-1);
   if      (pbar == pbarra1) return(0);
   else if (pbar == pbarra2) return(1);
   else if (pbar == pbarra3) return(2);
   return(-1);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::LisBarra(TList *lisEQP)
   {
   //insere Barras da Ligacao em lisEQP
   if ((pbarra1)&&(lisEQP->IndexOf(pbarra1) < 0)) lisEQP->Add(pbarra1);
   if ((pbarra2)&&(lisEQP->IndexOf(pbarra2) < 0)) lisEQP->Add(pbarra2);
   if ((pbarra3)&&(lisEQP->IndexOf(pbarra3) < 0)) lisEQP->Add(pbarra3);
   }

//---------------------------------------------------------------------------
int __fastcall TLigacao::NumeroBarras(void)
   {
   return(2);
   }

//---------------------------------------------------------------------------
void __fastcall TLigacao::OrdenaBarras(VTBarra *bar_ref)
   {
   //variáveis locais
   double posbar1, posbar2, posbar3;

   //verifica se a Barra de referência já é a Barra1
   if (pbarra1 == bar_ref) return;
   //verifica a Barra de referência é a Barra2
   if (pbarra2 == bar_ref)
      {//salva posbar1 e posbar2
      posbar1 = esq.posbar1;
      posbar2 = esq.posbar2;
      //inverte barra1 e barra2
      pbarra2 = pbarra1;
      pbarra1 = bar_ref;
      //inverte posbar1 e posbar2
      esq.posbar1 = posbar2;
      esq.posbar2 = posbar1;
      return;
      }
   //verifica a Barra de referência é a Barra3
   if (pbarra3 == bar_ref)
      {//salva posbar1 e posbar3
      posbar1 = esq.posbar1;
      posbar3 = esq.posbar3;
      //inverte barra1 e barra3
      pbarra3 = pbarra1;
      pbarra1 = bar_ref;
      //inverte posbar1 e posbar3
      esq.posbar1 = posbar3;
      esq.posbar3 = posbar1;
      return;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::TipoLigacao(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLigacao::Vertical(void)
   {
   //variáveis locais
   int x1, y1, x2, y2;
   int dx, dy;

   //determina coordenadas
   CoordenadasEsquematico(x1, y1, x2, y2);
   //verifica se a barra é 'vertical' ou 'horizontal'
   dx = abs(x2 - x1);
   dy = abs(y2 - y1);
   return((dx == 0)||(dx < dy));
   }

//---------------------------------------------------------------------------
//eof

