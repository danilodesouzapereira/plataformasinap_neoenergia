//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TRedes.h"
#include "TArea.h"
#include "VTBarra.h"
#include "VTCarga.h"
#include "VTChave.h"
#include "VTCluster.h"
#include "VTComentario.h"
#include "VTCorte.h"
#include "VTDominio.h"
#include "VTEqbar.h"
#include "VTListeqp.h"
#include "VTMedidor.h"
#include "VTMRede.h"
#include "VTTrecho.h"
#include "VTRede.h"
#include "VTReducao.h"
#include "VTTipoRede.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Patamar\VTPatamares.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedes
//---------------------------------------------------------------------------
VTRedes* __fastcall NewObjRedes(VTApl *apl)
   {
   return(new TRedes(apl));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
__fastcall TRedes::TRedes(VTApl *apl)
   {
   //variáveis locais
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    =  patamares->NumPatamar();

   //salva ponteiro p/ objetos
   this->apl = apl;
   //inicia dados
   area.esq       = new TArea();
   area.utm       = new TArea();
   //cria Rede isolada p/ armazenar Barras e Ligacoes isoladas
   isolada.rede           = NewObjRede(num_pat);
   isolada.rede->Codigo   = "RedeIsolada";
   isolada.rede->Estatica = false;
   isolada.rede->Status[sttISOLADO] = true;
   //associa Rede isolada com Redes
   isolada.rede->Redes = this;
   //cria listas VTListeqp
   lisBAR         = NewObjListeqp();
   //cria listas
   lisCLUSTER     = new TList();
   lisREDE        = new TList();
   lisMR          = new TList();
   lisMNET        = new TList();
   lisCOR         = new TList();
   lisDOMINIO     = new TList();
   lisCOMENTARIO  = new TList();
   lisTMP         = new TList();
   //assume área default
   DiagEsquematicoAreaDefault();
   //permite a troca dos ids
   PD.allowRedefineIds = true;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16903333
__fastcall TRedes::~TRedes(void)
   {
   //destrói objetos
   if (area.esq)     {delete area.esq; area.esq = NULL;}
   if (area.utm)     {delete area.utm; area.utm = NULL;}
   //if (area.utm_m)   {delete area.utm_m;   area.utm_m   = NULL;}
   if (isolada.rede) {delete isolada.rede; isolada.rede = NULL;}
//   if (lixeira)      {delete lixeira;      lixeira      = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisTMP)  {delete lisTMP; lisTMP = NULL;}
   //destrói listas e seus objetos
   if (lisCLUSTER)    {LimpaTList(lisCLUSTER);    delete lisCLUSTER;    lisCLUSTER   = NULL;}
   if (lisREDE)       {LimpaTList(lisREDE);       delete lisREDE;       lisREDE      = NULL;}
   if (lisMR)         {LimpaTList(lisMR);         delete lisMR;         lisMR        = NULL;}
   if (lisMNET)       {LimpaTList(lisMNET);       delete lisMNET;       lisMNET      = NULL;}
   if (lisCOR)        {LimpaTList(lisCOR);        delete lisCOR;        lisCOR       = NULL;}
   if (lisDOMINIO)    {LimpaTList(lisDOMINIO);    delete lisDOMINIO;    lisDOMINIO   = NULL;}
   if (lisCOMENTARIO) {LimpaTList(lisCOMENTARIO); delete lisCOMENTARIO; lisCOMENTARIO= NULL;}
   //destroi por último as barras, para evitar problemas ao remover ligações das barras
   if (lisBAR)        {LimpaTList(lisBAR);        delete lisBAR;        lisBAR       = NULL;}
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TRedes::AreaEsquematico(void)
   {
   return(area.esq);
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TRedes::AreaUtm_cm(void)
   {
   return(area.utm);
   }
/*
//-----------------------------------------------------------------------------
VTArea* __fastcall TRedes::AreaUtm_m(void)
   {
   return(area.utm_m);
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TRedes::CalculaArea(void)
   {
   //variáveis locais
   VTRede  *rede;

   //calcula área de cada rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->CalculaArea();
      }
   //calcula área da rede utm
   CalculaAreaUtm();
   //calcula área do diag.esquemático e faz ajuste se necessário
   CalculaAreaEsq();
   //DiagEsquematicoAjusta();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TRedes::CalculaAreaEsq(void)
   {
   //variáveis locais
   int       delta = 5;
   double    fator = 0.;
   struct {
          int x1, y1, x2, y2;
          int dx, dy;
          } t;
   //calcula área do diag.esquemático definida pelas Redes
   if (! CalculaAreaEsq(t.x1, t.y1, t.x2, t.y2))
      {//define área mínima default
      DiagEsquematicoAreaDefault();
      return;
      }
   //proteção: verifica se área tem dimensão menor que (2*delta)
   if ((t.dx = t.x2 - t.x1) < (delta+delta))
      {//acrescenta delta na coord.x
      //t.x1 -= delta;
      //t.x2 += delta;
      //proteção contra valor x1 muito pequeno
      if (t.x1 > delta) t.x1 -= delta;
      //proteção contra valor x2 muito alto (virando negativo)
      if ((t.x2 + delta) > 0) t.x2 += delta;
      t.dx = t.x2 - t.x1;
      }
   if ((t.dy = t.y2 - t.y1) < (delta+delta))
      {//acrescenta delta na coord.y
      //t.y1 -= delta;
      //t.y2 += delta;
      //proteção contra valor y1 muito pequeno
      if (t.y1 > delta) t.y1 -= delta;
      //proteção contra valor y2 muito alto (virando negativo)
      if ((t.y2 + delta) > 0) t.y2 += delta;
      t.dy = t.y2 - t.y1;
      }
   //define limites mínimos p/ área do esquemático
   area.esq_width_min  = t.dx;
   area.esq_height_min = t.dy;
   //define área esquemático com 1.1 vezes o tamanho da área calculada
   t.dx *= 0.05;
   t.dy *= 0.05;
   //if ((t.x1 -= t.dx) < 0) t.x1 = 0;
   //if ((t.y1 -= t.dy) < 0) t.y1 = 0;
   //proteção para valores x1,y1 menor que t.dx
   if (t.x1 > t.dx)  t.x1 -= t.dx;
   if (t.y1 > t.dy)  t.y1 -= t.dy;
   //proteção contra valores x2,y2 muito altos (virando negativo)
   if ((t.x2 + t.dx) > 0) t.x2 += t.dx;
   if ((t.y2 + t.dy) > 0) t.y2 += t.dy;
   //atualiza área
   area.esq->x1 = t.x1;
   area.esq->x2 = t.x2;
   area.esq->y1 = t.y1;
   area.esq->y2 = t.y2;
   //ajuste dimensão da área em função do fator de escala anterior
   DiagEsquematicoResize(fator);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TRedes::CalculaAreaEsq(void)
   {
   //variáveis locais
   int       width, height, dx, dy;
   double    fator = 0.;
   struct {
          int x1, y1, x2, y2;
          int dx, dy;
          } t;
   //define esquemático com área default
   DiagEsquematicoAreaDefault();
   //calcula área do diag.esquemático definida pelas Redes
   if (! CalculaAreaEsq(t.x1, t.y1, t.x2, t.y2)) return;
   //verifica se a área está contida na área default
   if ((t.x1 > area.esq->x1)&&(t.x2 < area.esq->x2)&&
       (t.y1 > area.esq->y1)&&(t.y2 < area.esq->y2)) return;
   //verifica a largura da área é menor que a largura default
   if ((t.dx = t.x2 - t.x1) < (width = area.esq->Width()))
      {//aumenta a largura da área
      dx    = (width - t.dx) * 0.5;
      t.x1 -= dx;
      t.x2 += dx;
      }
   //verifica a altura da área é menor que a altura default
   if ((t.dy = t.y2 - t.y1) < (height = area.esq->Height()))
      {//aumenta a altura da área
      dy    = (height - t.dy) * 0.5;
      t.y1 -= dy;
      t.y2 += dy;
      }
   //define limites mínimos p/ área do esquemático
   area.esq_width_min  = t.dx;
   area.esq_height_min = t.dy;
   //define área esquemático com 1.1 vezes o tamanho da área calculada
   t.dx = (t.x2 - t.x1) * 0.05;
   t.dy = (t.y2 - t.y1) * 0.05;
   //proteção para valores x1,y1 menor que t.dx
   if (t.x1 > t.dx)  t.x1 -= t.dx;
   if (t.y1 > t.dy)  t.y1 -= t.dy;
   //proteção contra valores x2,y2 muito altos (virando negativo)
   if ((t.x2 + t.dx) > 0) t.x2 += t.dx;
   if ((t.y2 + t.dy) > 0) t.y2 += t.dy;
   //atualiza área
   area.esq->x1 = t.x1;
   area.esq->x2 = t.x2;
   area.esq->y1 = t.y1;
   area.esq->y2 = t.y2;
   //ajuste dimensão da área em função do fator de escala anterior
   DiagEsquematicoResize(fator);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedes::CalculaAreaEsq(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   bool      first = true;
   int       b_x1, b_y1, b_x2, b_y2;
   TList     *lisBAR_ISO;
   VTBarra   *barra;
   VTRede    *rede;
   VTArea    *area_rede;

   //assume valores nulos
   x1 = y1 = x2 = y2 = 0;
   //verifica se há Rede
   //if (lisREDE->Count == 0) return(false);
   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede      = (VTRede*)lisREDE->Items[n];
      //JCG 2016.09.15 - código incluído em razão de edição em modo geo-referenciado
      if (! rede->Carregada) continue;
      area_rede = rede->AreaEsquematico();
      //proteção
      if ((area_rede->x1 == 0)&&(area_rede->y1 == 0)&&
          (area_rede->x2 == 0)&&(area_rede->y2 == 0)) continue;
      //verifica se área da primeira Rede
      if (first)
         {
         first = false;
         x1 = int(area_rede->x1);
         y1 = int(area_rede->y1);
         x2 = int(area_rede->x2);
         y2 = int(area_rede->y2);
         }
      else
         {
         x1 = Min(x1, int(area_rede->x1));
         y1 = Min(y1, int(area_rede->y1));
         x2 = Max(x2, int(area_rede->x2));
         y2 = Max(y2, int(area_rede->y2));
         }
      }
   //trata Barras isoladas
   lisBAR_ISO = isolada.rede->LisBarra();
   for (int nb = 0; nb < lisBAR_ISO->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_ISO->Items[nb];
      barra->CoordenadasEsquematico(b_x1, b_y1, b_x2, b_y2);
      if (first)
         {
         first = false;
         x1 = int(b_x1);
         y1 = int(b_y1);
         x2 = int(b_x2);
         y2 = int(b_y2);
         }
      else
         {
         x1 = Min(x1, b_x1);
         y1 = Min(y1, b_y1);
         x2 = Max(x2, b_x2);
         y2 = Max(y2, b_y2);
         }
      }
   //proteção
   if ((x1 == 0)&&(y1 == 0)&&(x2 == 0)&&(y2 == 0)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedes::CalculaAreaEsq(int &x1, int &y1, int &x2, int &y2, TList *lisBARRA, bool inclui_reducao)
   {
   //variáveis locais
   struct {
          int x1, y1, x2, y2;
          } b;
   VTBarra   *barra;
   VTReducao *reducao;

   //verifica se rede vazia
   if (lisBARRA->Count == 0) return(false);
   //determina área definida pela primeira Barra
   barra = (VTBarra*)lisBARRA->First();
   barra->AreaEsquematico(x1, y1, x2, y2);
   //loop p/ demais Barras (OBS: lisBAR inclui as Barras isoladas)
   for (int n = 1; n < lisBARRA->Count; n++)
      {//obtém área da Barra
      barra = (VTBarra*)lisBARRA->Items[n];
      barra->AreaEsquematico(b.x1, b.y1, b.x2, b.y2);
      x1 = Min(x1, b.x1);
      y1 = Min(y1, b.y1);
      x2 = Max(x2, b.x2);
      y2 = Max(y2, b.y2);
      }
   //verifica se deve incluir áreas das Reduçoes
   if (inclui_reducao)
      {//inclui Areas das Reduções
      lisTMP->Clear();
      LisReducaoCarregada(lisTMP);
      for (int nr = 0; nr < lisTMP->Count; nr++)
         {
         reducao = (VTReducao*)lisTMP->Items[nr];
         //obtém coordenadas da Barra fictícia da Reducao
         barra = reducao->BarraFicticia();
         barra->AreaEsquematico(b.x1, b.y1, b.x2, b.y2);
         x1 = Min(x1, b.x1);
         y1 = Min(y1, b.y1);
         x2 = Max(x2, b.x2);
         y2 = Max(y2, b.y2);
         }
      }
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedes::CalculaAreaUtm(void)
   {
   //variáveis locais
   bool      first = true;
   int       x, y, dx, dy;
   double    fator = 0.;
   TList     *lisBAR_ISO;
   VTBarra   *barra;
   VTRede    *rede;
   VTArea    *area_rede;

   //assume valores inválido
   area.utm->x1 = area.utm->y1 = area.utm->x2 = area.utm->y2 = -1;
   //verifica se há Barras
   if (lisBAR->Count == 0) return;
   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede      = (VTRede*)lisREDE->Items[n];
      area_rede = rede->AreaUtm_cm();
      //proteção
      if ((area_rede->x1 <= 0)||(area_rede->y1 <= 0)) continue;
      //verifica se área da primeira Rede
      if (first)
         {
         first = false;
         area.utm->CopiaAtributosDe(*(area_rede));
         }
      else
         {
         area.utm->x1 = Min(area.utm->x1, area_rede->x1);
         area.utm->y1 = Min(area.utm->y1, area_rede->y1);
         area.utm->x2 = Max(area.utm->x2, area_rede->x2);
         area.utm->y2 = Max(area.utm->y2, area_rede->y2);
         }
      }
   //trata Barras isoladas
   lisBAR_ISO = isolada.rede->LisBarra();
   for (int nb = 0; nb < lisBAR_ISO->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_ISO->Items[nb];
      if (! barra->CoordenadasUtm_cm(x, y)) continue;
      if (first)
         {
         first = false;
         area.utm->x1 = area.utm->x2 = x;
         area.utm->y1 = area.utm->y2 = y;
         }
      else
         {
         area.utm->x1 = Min(area.utm->x1, double(x));
         area.utm->y1 = Min(area.utm->y1, double(y));
         area.utm->x2 = Max(area.utm->x2, double(x));
         area.utm->y2 = Max(area.utm->y2, double(y));
         }
      }
   //zera limites mínimos p/ área utm
   area.utm_width_min  = 0;
   area.utm_height_min = 0;
   //proteção: verifica se existe área utm
   if ((area.utm->x1 > 0)&&(area.utm->x2 > 0))
      {//verifica se área tem dimensão muito pequena
      if (area.utm->Width() < 1000 )
         {//crescenta 10 m na área calculada
         area.utm->x1 -= 500;
         area.utm->x2 += 500;
         }
      if (area.utm->Height() < 1000)
         {//crescenta 10 m na área calculada
         area.utm->y1 -= 500;
         area.utm->y2 += 500;
         }
      //define limites mínimos p/ área utm
      area.utm_width_min  = area.utm->Width();
      area.utm_height_min = area.utm->Height();
      //define área utm com 1.1 vezes o tamanho da área calculada
      dx = area.utm->Width()  * 0.05;
      dy = area.utm->Height() * 0.05;
      area.utm->x1 -= dx;
      area.utm->y1 -= dy;
      area.utm->x2 += dx;
      area.utm->y2 += dy;
      DiagGeorefenciadoResize(fator);
      }
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15653253
void __fastcall TRedes::Clear(bool inclusive_dominio, bool mantem_barras)
   {
   //variáveis locais
   VTDominio *dominio;

   try{//limpa rede isolada
      isolada.rede->Clear();
      //limpa listas destruindo seus objetos
      if (inclusive_dominio)
         {//destrói todos os Domínios
         LimpaTList(lisDOMINIO);
         }
      else
         {//remove as Redes dos Dominios
         for (int n = 0; n < lisDOMINIO->Count; n++)
            {
            dominio = (VTDominio*)lisDOMINIO->Items[n];
            (dominio->LisRede())->Clear();
            }
         }
      LimpaTList(lisCLUSTER);
	  LimpaTList(lisREDE);
      LimpaTList(lisMR);
      LimpaTList(lisMNET);
      LimpaTList(lisCOR);
	  if(mantem_barras == false)
	  {
         LimpaTList(lisBAR);
	  }
      //recalcula área da rede
      CalculaArea();
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedes::ClearEqptosIsolados(void)
   {
   //variáveis locais
   VTBarra *barra;

   try{//copia lista de Barras isoladas
      lisTMP->Clear();
      CopiaTList(isolada.rede->LisBarra(), lisTMP);
      //limpa rede isolada (destrói suas Ligacoes e reinicai lista de Barras)
      isolada.rede->Clear();
      //retira Barras isoladas de lisBAR
      for (int n = 0; n < lisTMP->Count; n++)
         {
         barra = (VTBarra*)lisTMP->Items[n];
         lisBAR->Remove(barra);
         }
      //destrói Barras isoladas
      LimpaTList(lisTMP);
      //recalcula área da rede
      CalculaArea();
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TRedes::PM_GetDataReferencia(void)
   {
   //variáveis locais
   VTDominio *dominio;

   //verifica se há algum Dominio definido
   if (lisDOMINIO->Count > 0)
      {
      dominio = (VTDominio*)lisDOMINIO->First();
      return(dominio->DataCriacao);
      }
   //retorna data/hora atual
   return(Now());
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TRedes::PM_GetAllowRedefineIds(void)
{
	return(PD.allowRedefineIds);
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TRedes::PM_SetAllowRedefineIds(bool allow)
{
    //colocado como metodos para pode por breakpoint e ver quem tá mexendo
	PD.allowRedefineIds = allow;
}
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
void __fastcall TRedes::DefineStatus(unsigned bitmask, bool status)
   {
   //variáveis locais
   TList      *lisEQP   = NULL;
   TList      *lisEQPRR = NULL;
   VTEqpto    *eqpto;
   VTCurva    *curva;
   VTReducao  *reducao;
   VTTrecho   *trecho;

   __try
      {
      try{//cria listas
         lisEQP   = new TList();
         lisEQPRR = new TList();
         //atualiza status de todos Dominios
         for (int i = 0; i < lisDOMINIO->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisDOMINIO->Items[i]) == NULL) continue;
            eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todos Cluster
         for (int i = 0; i < lisCLUSTER->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisCLUSTER->Items[i]) == NULL) continue;
            eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todas MRedes
         for (int i = 0; i < lisMR->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisMR->Items[i]) == NULL) continue;
            eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todos Cortes
         for (int i = 0; i < lisCOR->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisCOR->Items[i]) == NULL) continue;
            eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todas redes
         for (int i = 0; i < lisREDE->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisREDE->Items[i]) == NULL) continue;
            eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todos Eqptos das redes
         LisEqpto(lisEQP);
         for (int i = 0; i < lisEQP->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisEQP->Items[i]) == NULL) continue;
            eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            //atualiza status da Curva do Eqpto
            if ((curva = eqpto->ExisteCurva()) != NULL)
               {
               if (! curva->Tipica)
                  {
                  curva->StatusAsUnsigned = 0;
                  curva->DefineStatus(bitmask, status);
                  }
               }
            //atualiza sttatus de Arranjo próprio
            if (eqpto->Tipo() == eqptoTRECHO)
               {
               trecho = (VTTrecho*)eqpto;
               if (! trecho->ArranjoTipico)
                  {//atualiza Status do Arranjo próprio do trecho
                  trecho->arranjo->StatusAsUnsigned = 0;
                  trecho->arranjo->DefineStatus(bitmask, status);
                  }
               }
            }
         //atualiza status de todos Eqptos das reduções
         lisEQP->Clear();
         LisReducao(lisEQP);
         for (int i = 0; i < lisEQP->Count; i++)
            {
            if ((reducao = (VTReducao*)lisEQP->Items[i]) == NULL) continue;
            reducao->LisEqpto(lisEQPRR);
            for (int j = 0; j < lisEQPRR->Count; j++)
               {
               if ((eqpto = (VTEqpto*)lisEQPRR->Items[j]) == NULL) continue;
               eqpto->StatusAsUnsigned = 0;
               eqpto->DefineStatus(bitmask, status);
               //atualiza status da Curva do Eqpto
               if ((curva = eqpto->ExisteCurva()) != NULL)
                  {
                  if (! curva->Tipica)
                     {
                     curva->StatusAsUnsigned = 0;
                     curva->DefineStatus(bitmask, status);
                     }
                  }
               }
            }
         }
      catch (Exception& e)
         {
         }
      }
   __finally
      {
      if (lisEQP)   { delete lisEQP; lisEQP = NULL; }
      if (lisEQPRR) { delete lisEQP; lisEQP = NULL; }
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TRedes::DefineStatus(unsigned bitmask, bool status)
   {
   //variáveis locais
   TList      *lisEQP   = NULL;
   TList      *lisEQPRR = NULL;
   VTEqpto    *eqpto;
   VTCurva    *curva;
   VTReducao  *reducao;
   VTTrecho   *trecho;

   //JCG 2016.08.31 - atualiza apenas os bits indicados em bitmasck
   __try
      {
      try{//cria listas
         lisEQP   = new TList();
         lisEQPRR = new TList();
         //atualiza status de todos Dominios
         for (int i = 0; i < lisDOMINIO->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisDOMINIO->Items[i]) == NULL) continue;
            //eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todos Cluster
         for (int i = 0; i < lisCLUSTER->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisCLUSTER->Items[i]) == NULL) continue;
            //eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todas MRedes
         for (int i = 0; i < lisMR->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisMR->Items[i]) == NULL) continue;
            //eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todos Cortes
         for (int i = 0; i < lisCOR->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisCOR->Items[i]) == NULL) continue;
            //eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todas redes
         for (int i = 0; i < lisREDE->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisREDE->Items[i]) == NULL) continue;
            //eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            }
         //atualiza status de todos Eqptos das redes
         LisEqpto(lisEQP);
         for (int i = 0; i < lisEQP->Count; i++)
            {
            if ((eqpto = (VTEqpto*)lisEQP->Items[i]) == NULL) continue;
            //eqpto->StatusAsUnsigned = 0;
            eqpto->DefineStatus(bitmask, status);
            //atualiza status da Curva do Eqpto
            if ((curva = eqpto->ExisteCurva()) != NULL)
               {
               if (! curva->Tipica)
                  {
                  //curva->StatusAsUnsigned = 0;
                  curva->DefineStatus(bitmask, status);
                  }
               }
            //atualiza sttatus de Arranjo próprio
            if (eqpto->Tipo() == eqptoTRECHO)
               {
               trecho = (VTTrecho*)eqpto;
               if (! trecho->ArranjoTipico)
                  {//atualiza Status do Arranjo próprio do trecho
                  //trecho->arranjo->StatusAsUnsigned = 0;
                  trecho->arranjo->DefineStatus(bitmask, status);
                  }
               }
            }
         //atualiza status de todos Eqptos das reduções
         lisEQP->Clear();
         LisReducao(lisEQP);
         for (int i = 0; i < lisEQP->Count; i++)
            {
            if ((reducao = (VTReducao*)lisEQP->Items[i]) == NULL) continue;
            reducao->LisEqpto(lisEQPRR);
            for (int j = 0; j < lisEQPRR->Count; j++)
               {
               if ((eqpto = (VTEqpto*)lisEQPRR->Items[j]) == NULL) continue;
               //eqpto->StatusAsUnsigned = 0;
               eqpto->DefineStatus(bitmask, status);
               //atualiza status da Curva do Eqpto
               if ((curva = eqpto->ExisteCurva()) != NULL)
                  {
                  if (! curva->Tipica)
                     {
                     //curva->StatusAsUnsigned = 0;
                     curva->DefineStatus(bitmask, status);
                     }
                  }
               }
            }
         }
      catch (Exception& e)
         {
         }
      }
   __finally
      {
      if (lisEQP)   { delete lisEQP; lisEQP = NULL; }
      if (lisEQPRR) { delete lisEQPRR; lisEQPRR = NULL; }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::DiagEsquematicoAjusta(void)
   {
   //variáveis locais
   double dist_min_eqbar = 0.002; //dist.mínima percentual em relação à área do diag.esquemático
   double fat_escala;
   struct {
          int x1, y1, x2, y2;
          double dx, dy;
          } t;
   VTBarra   *pbarra;
   VTEqbar   *eqbar;
   VTTrecho  *ptrecho;
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisPTO;
   TList     *lisREDU;

   //define área fixa p/ o diag.esquemático
   //DiagEsquematicoAreaDefault();
   //calcula área do diag.esquemático definida pelas Barras
   if (! CalculaAreaEsq(t.x1, t.y1, t.x2, t.y2)) return;
   //verifica se a área definida pelas Barras está dentro da área default
   if ((t.x1 >= area.esq->x1)&&(t.x2 <= area.esq->x2)&&
       (t.y1 >= area.esq->y1)&&(t.y2 <= area.esq->y2))
      {//área das Barras dentro da área default: nada a fazer
      return;
      }
   //verifica se a área definida pelas Barras possui intersecção c/ área default
   if ((area.esq->ContemPonto(t.x1, t.y1))||
       (area.esq->ContemPonto(t.x1, t.y2))||
       (area.esq->ContemPonto(t.x2, t.y1))||
       (area.esq->ContemPonto(t.x2, t.y2)))
      {//ajusta área definida pelas Barras p/ conversão de coordenadas
      if (t.x1 > area.esq->x1) t.x1 = area.esq->x1;
      if (t.x2 < area.esq->x2) t.x2 = area.esq->x2;
      if (t.y1 > area.esq->y1) t.y1 = area.esq->y1;
      if (t.y2 < area.esq->y2) t.y2 = area.esq->y2;
      }
   //determina dimensões da rede
   t.dx = t.x2 - t.x1 + 1;
   t.dy = t.y2 - t.y1 + 1;
   //determina fator de escala único p/ manter mesma relação nos eixos XY
   fat_escala = Min(double(area.esq->Width()) /double(t.dx),
                    double(area.esq->Height())/double(t.dy));
   //converte coordenadas das Barras (OBS: lisBAR inclui as Barras isoladas)
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra         = (VTBarra*)lisBAR->Items[nb];
      pbarra->esq.x1 = area.esq->x1 + ((pbarra->esq.x1 - t.x1) * fat_escala);
      pbarra->esq.x2 = area.esq->x1 + ((pbarra->esq.x2 - t.x1) * fat_escala);
      pbarra->esq.y1 = area.esq->y1 + ((pbarra->esq.y1 - t.y1) * fat_escala);
      pbarra->esq.y2 = area.esq->y1 + ((pbarra->esq.y2 - t.y1) * fat_escala);
      //atualiza status sttDESLOCADO
      pbarra->Status[sttDESLOCADO] = true;
      }
   //converte coordenadas das Barras dos esquemáticos das Reducoes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da MRede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém Barra fictícia da reducao
         pbarra = reducao->BarraFicticia();
         //converte coordenadas da Barra mantendo relação de escala das coordenadas xy
         pbarra->esq.x1 = area.esq->x1 + ((pbarra->esq.x1 - t.x1) * fat_escala);
         pbarra->esq.x2 = area.esq->x1 + ((pbarra->esq.x2 - t.x1) * fat_escala);
         pbarra->esq.y1 = area.esq->y1 + ((pbarra->esq.y1 - t.y1) * fat_escala);
         pbarra->esq.y2 = area.esq->y1 + ((pbarra->esq.y2 - t.y1) * fat_escala);
         }
      }
   //converte dxbar, dybar dos Eqbar
   lisTMP->Clear();
   LisEqbar(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqbar = (VTEqbar*)lisTMP->Items[n];
      //redefine distância do Eqbar da Barra
      eqbar->esq.dxbar *= fat_escala;
      eqbar->esq.dybar *= fat_escala;
      //verifica se Eqbar não está distanciado do Barra
      if ((eqbar->esq.dxbar == 0)&&(eqbar->esq.dybar == 0))
         {
         eqbar->esq.dxbar = dist_min_eqbar * area.esq->Width();
         eqbar->esq.dybar = dist_min_eqbar * area.esq->Width();
         }
      //atualiza status sttDESLOCADO
      eqbar->Status[sttDESLOCADO] = true;
      }
   lisTMP->Clear();
   //recalcula área de cada rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->CalculaArea();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::DiagEsquematicoAlignClient(void)
   {
   //variáveis locais
   double fat_x, fat_y ;
   double dist_min_eqbar = 0.002; //dist.mínima percentual em relação à área do diag.esquemático
   struct {
          int x1, y1, x2, y2;
          double dx, dy;
          } t;
   VTBarra   *pbarra;
   VTEqbar   *eqbar;
   VTMRede   *mrede;
   VTTrecho  *ptrecho;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisPTO;
   TList     *lisREDU;

   //define área fixa p/ o diag.esquemático
   //DiagEsquematicoAreaDefault();
   //calcula área do diag.esquemático definida pelas Barras
   if (! CalculaAreaEsq(t.x1, t.y1, t.x2, t.y2)) return;
   //determina dimensões da rede
   t.dx = t.x2 - t.x1 + 1;
   t.dy = t.y2 - t.y1 + 1;
   //determina fatores de escala x e y sem manter a proporção nos eixos XY
   fat_x = double(area.esq->Width())/double(t.dx);
   fat_y = double(area.esq->Height())/double(t.dy);
   //converte coordenadas das Barras SEM manter relação de escala XY
   //OBS: lisBAR inclui as Barras isoladas
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra         = (VTBarra*)lisBAR->Items[nb];
      pbarra->esq.x1 = area.esq->x1 + ((pbarra->esq.x1 - t.x1) * fat_x);
      pbarra->esq.x2 = area.esq->x1 + ((pbarra->esq.x2 - t.x1) * fat_x);
      pbarra->esq.y1 = area.esq->y1 + ((pbarra->esq.y1 - t.y1) * fat_y);
      pbarra->esq.y2 = area.esq->y1 + ((pbarra->esq.y2 - t.y1) * fat_y);
      //atualiza status sttDESLOCADO
      pbarra->Status[sttDESLOCADO] = true;
      }
   //converte coordenadas das Barras dos esquemáticos das Reducoes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da MRede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém Barra fictícia da reducao
         pbarra = reducao->BarraFicticia();
         //converte coordenadas da Barra mantendo relação de escala das coordenadas xy
         pbarra->esq.x1 = area.esq->x1 + ((pbarra->esq.x1 - t.x1) * fat_x);
         pbarra->esq.x2 = area.esq->x1 + ((pbarra->esq.x2 - t.x1) * fat_x);
         pbarra->esq.y1 = area.esq->y1 + ((pbarra->esq.y1 - t.y1) * fat_y);
         pbarra->esq.y2 = area.esq->y1 + ((pbarra->esq.y2 - t.y1) * fat_y);
         }
      }
   //converte dxbar, dybar dos Eqbar
   lisTMP->Clear();
   LisEqbar(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqbar = (VTEqbar*)lisTMP->Items[n];
      //redefine distância do Eqbar da Barra
      eqbar->esq.dxbar *= fat_x;
      eqbar->esq.dybar *= fat_y;
      //verifica se Eqbar não está distanciado do Barra
      if ((eqbar->esq.dxbar == 0)&&(eqbar->esq.dybar == 0))
         {
         eqbar->esq.dxbar = dist_min_eqbar * area.esq->Width();
         eqbar->esq.dybar = dist_min_eqbar * area.esq->Width();
         }
      //atualiza status sttDESLOCADO
      eqbar->Status[sttDESLOCADO] = true;
      }
   lisTMP->Clear();
   //recalcula área de cada rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->CalculaArea();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::DiagEsquematicoResize(double fator)
   {
   //variáveis locais
   int  delta_width, delta_height;
   int  new_width  = area.esq->Width()  * (1.0 - fator);
   int  new_height = area.esq->Height() * (1.0 - fator);

   //proteção
   if (new_width  < area.esq_width_min)  return;
   if (new_height < area.esq_height_min) return;
   //redefine delta_width
   delta_width  = (new_width  - area.esq->Width())  / 2;
   delta_height = (new_height - area.esq->Height()) / 2;
   if (delta_width < area.esq->x1)
      {
      area.esq->x1 = int(area.esq->x1 - delta_width);
      area.esq->x2 = int(area.esq->x2 + delta_width);
      }
   if (delta_height < area.esq->y1)
      {
      area.esq->y1 = int(area.esq->y1 - delta_height);
      area.esq->y2 = int(area.esq->y2 + delta_height);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::DiagEsquematicoResize(TList *lisBAR_SEL, double fator)
   {
   //variáveis locais
   double dist_min_eqbar = 0.002; //dist.mínima percentual em relação à área do diag.esquemático
   double fat_escala;
   struct {
          int x, y;
          } offset;
   struct {
          int    x1, y1, x2, y2;
          double dx, dy;
          } t;
   VTBarra   *pbarra;
   VTEqbar   *eqbar;
   VTMRede   *mrede;
   VTTrecho  *ptrecho;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisPTO;
   TList     *lisREDU;

   //define área fixa p/ o diag.esquemático
   //DiagEsquematicoAreaDefault();
   //calcula área do diag.esquemático definida pelas Barras (SEM incluir Reduçoes)
   if (! CalculaAreaEsq(t.x1, t.y1, t.x2, t.y2, lisBAR_SEL, false)) return;
   //determina dimensões da rede
   t.dx = t.x2 - t.x1 + 1;
   t.dy = t.y2 - t.y1 + 1;
   //determina fator de escala atual entre área total e área definida pelas Barras em lisBAR_SEL
   //fat_escala = Min(double(area.esq->Width())/double(t.dx), double(area.esq->Height())/double(t.dy));
   //proteção p/ não ultrapassar limites
   //if (fator < 0) {fator = fat_escala;}
   //if ((fator > area.fator_esq)&&((fator * area.fator_esq) > 1)) return;
   //define fator de escala a ser aplicado
   fat_escala = 1.0 + fator;
   //determina coordenadas de offset (ponto superior esquerdo)
   offset.x = t.x1;
   offset.y = t.y1;
   //converte coordenadas das Barras (OBS: lisBAR inclui as Barras isoladas)
   for (int nb = 0; nb < lisBAR_SEL->Count; nb++)
      {
      pbarra         = (VTBarra*)lisBAR_SEL->Items[nb];
      pbarra->esq.x1 = offset.x + ((pbarra->esq.x1 - t.x1) * fat_escala);
      pbarra->esq.x2 = offset.x + ((pbarra->esq.x2 - t.x1) * fat_escala);
      pbarra->esq.y1 = offset.y + ((pbarra->esq.y1 - t.y1) * fat_escala);
      pbarra->esq.y2 = offset.y + ((pbarra->esq.y2 - t.y1) * fat_escala);
      //atualiza status sttDESLOCADO
      pbarra->Status[sttDESLOCADO] = true;
      }
   //converte coordenadas das Barras dos esquemáticos das Reducoes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém Barra fictícia da reducao
         pbarra = reducao->BarraFicticia();
         //converte coordenadas da Barra mantendo relação de escala das coordenadas xy
         pbarra->esq.x1 = offset.x + ((pbarra->esq.x1 - t.x1) * fat_escala);
         pbarra->esq.x2 = offset.x + ((pbarra->esq.x2 - t.x1) * fat_escala);
         pbarra->esq.y1 = offset.y + ((pbarra->esq.y1 - t.y1) * fat_escala);
         pbarra->esq.y2 = offset.y + ((pbarra->esq.y2 - t.y1) * fat_escala);
         }
      }
   //converte dxbar, dybar dos Eqbar
   lisTMP->Clear();
   for (int nb = 0; nb < lisBAR_SEL->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR_SEL->Items[nb];
      pbarra->LisEqbar(lisTMP);
      }
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqbar = (VTEqbar*)lisTMP->Items[n];
      //redefine distância do Eqbar da Barra
      eqbar->esq.dxbar *= fat_escala;
      eqbar->esq.dybar *= fat_escala;
      //verifica se Eqbar não está distanciado do Barra
      if ((eqbar->esq.dxbar == 0)&&(eqbar->esq.dybar == 0))
         {
         eqbar->esq.dxbar = dist_min_eqbar * area.esq->Width();
         eqbar->esq.dybar = dist_min_eqbar * area.esq->Width();
         }
      //atualiza status sttDESLOCADO
      eqbar->Status[sttDESLOCADO] = true;
      }
   lisTMP->Clear();
   //recalcula área de cada rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->CalculaArea();
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TRedes::DiagEsquematicoAreaDefault(void)
   {
   //define coordenadas fixas p/ o diag.esquemático
   area.esq->x1 = 50000; area.esq->x2 = area.esq->x1 + (Screen->Width  * 10);
   area.esq->y1 = 50000; area.esq->y2 = area.esq->y1 + (Screen->Height * 10);
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TRedes::DiagGeorefenciadoResize(double fator)
   {
   //variáveis locais
   int    width, height;
   double fat_mult;

   //proteção
   if ((fator > 1.)&&((fator * area.fator_utm) > 1.0)) return;
   //calcula fator de multiplicação
   //fat_mult = (1. - fator) / 2;
   if (fator <= 1.0)
      {
      fat_mult = (1. - fator) / 2.;
      }
   else
      {
      fat_mult =  ((1./fator) - 1.) / 2.;
      }
   //width    = area.utm->Width()  * fat_mult;
   //height   = area.utm->Height() * fat_mult;
   width    = (area.utm->Width() * area.fator_utm) * fat_mult;
   height   = (area.utm->Height()* area.fator_utm) * fat_mult;
   //recalcula área utm
   area.utm->x1 = int(area.utm->x1 - width);
   area.utm->x2 = int(area.utm->x2 + width);
   area.utm->y1 = int(area.utm->y1 - height);
   area.utm->y2 = int(area.utm->y2 + height);
   //salva fator acumulado
   area.fator_utm *= fator;
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TRedes::DiagGeorefenciadoResize(double fator)
   {
   //variáveis locais
   int  delta_width, delta_height;
   int  new_width  = area.utm->Width()  * (1.0 - fator);
   int  new_height = area.utm->Height() * (1.0 - fator);

   //proteção
   if (new_width  < area.utm_width_min)  return;
   if (new_height < area.utm_height_min) return;
   //redefine delta_width
   delta_width  = (new_width  - area.utm->Width())  / 2;
   delta_height = (new_height - area.utm->Height()) / 2;
   area.utm->x1 = int(area.utm->x1 - delta_width);
   area.utm->x2 = int(area.utm->x2 + delta_width);
   area.utm->y1 = int(area.utm->y1 - delta_height);
   area.utm->y2 = int(area.utm->y2 + delta_height);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedes::ExisteAlteracao(void)
   {
   //variáveis locais
   VTEqpto *eqpto;
   VTRede  *rede;

   //verifica se há algum Cluster novo, alterado, deslocado
   for (int n = 0; n < lisCLUSTER->Count; n++)
      {
      eqpto = (VTCluster*)lisCLUSTER->Items[n];
      //verifica se o Eqpto é isolado
      if (eqpto->Status[sttISOLADO]) continue;
      //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
      if ((eqpto->Status[sttNOVO])||
          (eqpto->Status[sttALTERADO])||
          (eqpto->Status[sttDESLOCADO])) return(true);
      }
   //verifica se há alguma Barra nova, alterada, deslocada
   for (int n = 0; n < lisBAR->Count; n++)
      {
      eqpto = (VTEqpto*)lisBAR->Items[n];
      //verifica se o Eqpto é isolado
      if (eqpto->Status[sttISOLADO]) continue;
      //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
      if ((eqpto->Status[sttNOVO])||
          (eqpto->Status[sttALTERADO])||
          (eqpto->Status[sttDESLOCADO])) return(true);
      }
   //verifica se há algum Eqbar novo ou alterado
   lisTMP->Clear();
   LisEqbar(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqpto = (VTEqpto*)lisTMP->Items[n];
      //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
      if ((eqpto->Status[sttNOVO])||
          (eqpto->Status[sttALTERADO])||
          (eqpto->Status[sttDESLOCADO])) return(true);
      }
   //verifica se há alguma alteração em cada Rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteAlteracao()) return(true);
      }
   lisTMP->Clear();
   return(false);
   }

/*
//---------------------------------------------------------------------------
VTBarra* __fastcall TRedes::ExisteBarra(int barra_id)
   {
   //variáveis locais
   VTBarra *barra;

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->Id == barra_id) return(barra);
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTBarra* __fastcall TRedes::ExisteBarra(int barra_id)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //procura Barra na lista local lisBAR
   eqpto = lisBAR->ExisteEqpto(barra_id);
   return((VTBarra*)eqpto);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TRedes::ExisteBarra(AnsiString codigo)
   {
   //variáveis locais
   VTBarra *barra;

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->Codigo.AnsiCompareIC(codigo) == 0) return(barra);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TRedes::ExisteBarraCoordUtm(int utm_x, int utm_y)
   {
   //variáveis locais
  VTBarra *pbarra;

   //procura Barra por suas coord.utm
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      if ((pbarra->utm.x == utm_x)&&(pbarra->utm.y == utm_y)) return(pbarra);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCluster* __fastcall TRedes::ExisteCluster(int cluster_id)
   {
   //variáveis locais
   VTCluster *cluster;

   //loop p/ todos VTCluster
   for (int n = 0; n < lisCLUSTER->Count; n++)
      {
      cluster = (VTCluster*)lisCLUSTER->Items[n];
      if (cluster->Id == cluster_id) return(cluster);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCluster* __fastcall TRedes::ExisteCluster(AnsiString codigo)
   {
   //variáveis locais
   VTCluster *cluster;

   //loop p/ todos VTCluster
   for (int n = 0; n < lisCLUSTER->Count; n++)
      {
      cluster = (VTCluster*)lisCLUSTER->Items[n];
      if (cluster->Codigo.AnsiCompareIC(codigo) == 0) return(cluster);
      }
   return(NULL);
   }

// ---------------------------------------------------------------------------
VTComentario* __fastcall TRedes::ExisteComentario(VTEqpto *eqpto)
{
	VTComentario *comentario;

	if (!lisCOMENTARIO)
		return (NULL);
	if (!eqpto)
		return (NULL);
	if (eqpto->Status[sttCOMENTA] == false)
		return (NULL);

	// Como existe o comentário (sttCOMENTA == true), busca o equipamento na lista
	// if ((ndxComentario = lisCOMENTARIO->IndexOf(eqpto)) < 0) return(NULL);
	for (int n = 0; n < lisCOMENTARIO->Count; n++)
	{
		comentario = (VTComentario*)lisCOMENTARIO->Items[n];
		if (comentario->Eqpto == eqpto)
		{
			return (comentario);
		}
	}
	return(NULL);
	// return((VTComentario*)lisCOMENTARIO->Items[ndxComentario]);
}

//---------------------------------------------------------------------------
VTCorte* __fastcall TRedes::ExisteCorte(int corte_id)
   {
   //variáveis locais
   VTCorte *corte;

   //loop p/ todos Cortes
   for (int n = 0; n < lisCOR->Count; n++)
      {
      corte = (VTCorte*)lisCOR->Items[n];
      if (corte->Id == corte_id) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTDominio* __fastcall TRedes::ExisteDominio(int dominio_id)
   {
   //variáveis locais
   VTDominio *dominio;

   //loop p/ todos Dominios
   for (int n = 0; n < lisDOMINIO->Count; n++)
      {
      dominio = (VTDominio*)lisDOMINIO->Items[n];
      if (dominio->Id == dominio_id) return(dominio);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTDominio* __fastcall TRedes::ExisteDominio(AnsiString codigo)
   {
   //variáveis locais
   VTDominio *dominio;

   //loop p/ todos Dominios
   for (int n = 0; n < lisDOMINIO->Count; n++)
      {
      dominio = (VTDominio*)lisDOMINIO->Items[n];
      if (dominio->Codigo.AnsiCompareIC(codigo) == 0) return(dominio);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TRedes::ExisteEqpto(int tipo_eqpto, AnsiString codigo)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se é busca de uma Barra
   if (tipo_eqpto == eqptoBARRA)
      {//executa função ExisteBarra que faz busca binária na lista de barras
      return(ExisteBarra(codigo));
      }
   //obtém lista de Eqptos do tipo indicado
   lisTMP->Clear();
   LisEqpto(lisTMP, tipo_eqpto);
   //determina Eqpto
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqpto = (VTEqpto*)lisTMP->Items[n];
      if (eqpto->Codigo.AnsiCompareIC(codigo) == 0) return(eqpto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TRedes::ExisteEqpto(int tipo_eqpto, int eqpto_id)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se é busca de uma Barra
   if (tipo_eqpto == eqptoBARRA)
      {//executa função ExisteBarra que faz busca binária na lista de barras
      return(ExisteBarra(eqpto_id));
      }
   //obtém lista de Eqptos do tipo indicado
   lisTMP->Clear();
   LisEqpto(lisTMP, tipo_eqpto);
   //determina Eqpto
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqpto = (VTEqpto*)lisTMP->Items[n];
      if (eqpto->Id == eqpto_id) return(eqpto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TRedes::ExisteMRede(int mrede_id)
   {
   //variáveis locais
   VTMRede *mrede;

   //procura MRede em lisMR
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Id == mrede_id) return(mrede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TRedes::ExisteRede(int rede_id)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Id == rede_id) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TRedes::ExisteRede(AnsiString codigo)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Codigo.AnsiCompareIC(codigo) == 0) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereBarra(VTBarra *barra)
   {
   //variáveis locais
   TList *lisBAR_ISO = isolada.rede->LisBarra();

   //proteção
   if (barra == NULL) return;
   //insere objeto VTBarra em lisBAR
   if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
   //associa objeto VTBarra c/ este objeto
   //barra->DefineObjRedes(this);
   //verifica se é uma Barra isolada
   if (barra->Status[sttISOLADO])
      {//insere Barra na lista de Barras isoladas
      if (lisBAR_ISO->IndexOf(barra) < 0)lisBAR_ISO->Add(barra);
      }
   //remove Barra da Lixeira
   //RemoveEqptoDaLixeira(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereCluster(VTCluster *cluster)
   {
   //proteção
   if (cluster == NULL) return;
   //insere objeto VTCorte em lisCOR
   if (lisCLUSTER->IndexOf(cluster) < 0) lisCLUSTER->Add(cluster);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereComentario(VTComentario *comentario)
   {
   if (!comentario)         return;
   if (!comentario->Eqpto)  return;
   if (!lisCOMENTARIO)      return;

   // Marca o flag do equipamento, facilitando a busca
   comentario->Eqpto->Status[sttCOMENTA] = true;
   // Insere na lista de comentários da rede
   lisCOMENTARIO->Add(comentario);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereCorte(VTCorte *corte)
   {
   //proteção
   if (corte == NULL) return;
   //insere objeto VTCorte em lisCOR
   if (lisCOR->IndexOf(corte) < 0) lisCOR->Add(corte);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereDominio(VTDominio *dominio)
   {
   //proteção
   if (dominio == NULL) return;
   //insere objeto Dominio em lisDOMINIO
   if (lisDOMINIO->IndexOf(dominio) < 0) lisDOMINIO->Add(dominio);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereMRede(VTMRede *mrede)
   {
   //proteção
   if (mrede == NULL) return;
   //verifica se o objeto VTMRede já existe em lisMR
   if (lisMR->IndexOf(mrede) < 0)
      {//insere objeto VTMRede na lista lisMR
      lisMR->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::InsereRede(VTRede *rede)
   {
   //proteção
   if (rede == NULL) return;
   //verifica se o objeto VTRede já existe em lisREDE
   if (lisREDE->IndexOf(rede) < 0)
      {//insere objeto VTRede na lista lisREDE
      lisREDE->Add(rede);
      //associa objeto VTRede c/ este objeto
      rede->Redes = this;
      //define Rede com Status não isoaldo
      rede->Status[sttISOLADO] = false;
      }
   //remove Rede da Lixeira
   //RemoveEqptoDaLixeira(rede);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisBarra(void)
   {
   return(lisBAR);
   }

/*
//---------------------------------------------------------------------------
void __fastcall TRedes::LisBarra(TList *lisEQP)
   {
   //variáveis locais
   VTBarra *barra;

   if (inclui_isolado)
      {
      CopiaTList(lisBAR, lisEQP);
      }
   else
      {//copia somente as Barras não isoladas
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         if (! barra->Status[sttISOLADO]) lisEQP->Add(barra);
         }
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TRedes::LisBarra(TList *lisEQP)
   {
   CopiaTList(lisBAR, lisEQP);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TRedes::LisBarraExclusiva(TList *lisEQP, VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra;
   VTCorte   *corte;
   VTLigacao *ligacao;
   VTRede    *rede_viz;
   TList     *lisBAR_COR;
   TList     *lisCORTE;

   //obtém todas as Barras da Rede, exceto as conectadas por Chave aberta
   rede->LisBarra(lisEQP, false);
   //retira todas as Barras de Cortes da Rede
   lisCORTE = rede->LisCorte();
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //obtém lista de Barras do Corte
      lisBAR_COR = corte->LisBarra();
      for (int nb = 0; nb < lisBAR_COR->Count; nb++)
         {
         lisEQP->Remove((VTBarra*)lisBAR_COR->Items[nb]);
         }
      }
   //retira Barras conectadas por Chaves abertas de outras redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede_viz = (VTRede*)lisREDE->Items[nr];
      if (! rede_viz->Carregada) continue;
      if (rede_viz == rede) continue;
      //obtém lista de Chaves da Rede vizinha
      lisTMP->Clear();
      rede_viz->LisChave(lisTMP, chvABERTA);
      for (int n = 0; n < lisTMP->Count; n++)
         {
         ligacao = (VTLigacao*)lisTMP->Items[n];
         for (int ind_bar = 0; ind_bar <NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            //retira Barras da Ligacao da lista de barras exclusivas
            lisEQP->Remove(barra);
            }
         }
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TRedes::LisBarraExclusiva(TList *lisEQP, VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra;
   VTRede    *rede_viz;
   TList     *lisBAR_REDE;

   //marca todas as Barras da Rede
   lisBAR_REDE = rede->LisBarra();
   for (int nb = 0; nb < lisBAR_REDE->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_REDE->Items[nb];
      barra->Tag = rede->Id;
      }
   //loop p/ todas as demais redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede_viz = (VTRede*)lisREDE->Items[nr];
      if (rede_viz == rede) continue;
      //marca todas as Barras da Rede vizinha
      lisBAR_REDE = rede_viz->LisBarra();
      for (int nb = 0; nb < lisBAR_REDE->Count; nb++)
         {
         barra = (VTBarra*)lisBAR_REDE->Items[nb];
         barra->Tag = rede_viz->Id;
         }
      }
   //seleciona Barras que são exclusivas da Rede
   lisBAR_REDE = rede->LisBarra();
   for (int nb = 0; nb < lisBAR_REDE->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_REDE->Items[nb];
      if (barra->Tag == rede->Id)
         {//insere Barra em lisEQP
         if (lisEQP->IndexOf( barra) < 0) lisEQP->Add( barra);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisBarraIsolada(void)
   {
   return(isolada.rede->LisBarra());
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisCanal(TList *lisEQP)
   {
   //variáveis locais
   VTMedidor *medidor;

   //obtém lista de Medidores
   lisTMP->Clear();
   LisEqbar(lisTMP, eqptoMEDIDOR);
   //loop p/ todos Medidores
   for (int n =0; n < lisTMP->Count; n++)
      {
      medidor = (VTMedidor*)lisTMP->Items[n];
      CopiaTList(medidor->LisCanal(), lisEQP);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisCorte(void)
   {
   return(lisCOR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisCluster(void)
   {
   return(lisCLUSTER);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisComentario(void)
   {
   return(lisCOMENTARIO);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisCorte(TList *lisEQP)
   {
    CopiaTList(lisCOR, lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisDominio(void)
   {
   return(lisDOMINIO);
   }
//---------------------------------------------------------------------------
void __fastcall TRedes::LisDominio(TList *lisEXT)
   {
   CopiaTList(lisDOMINIO, lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisDominio(TList *lisEXT, VTRede *rede)
   {
   //variáveis locais
   VTDominio *dominio;

   //loop p/ todos Dominios
   for (int n = 0; n < lisDOMINIO->Count; n++)
      {
      dominio = (VTDominio*)lisDOMINIO->Items[n];
      if (dominio->ExisteRede(rede))
         {//insere Dominio em lisEXT
         if (lisEXT->IndexOf(dominio) < 0) lisEXT->Add(dominio);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqbar(TList *lisEQP)
   {
   //variáveis locais
   VTBarra *barra;

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->LisEqbar(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqbar(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTBarra *barra;

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->LisEqbar(lisEQP, tipo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqpto(TList *lisEQP)
   {
   //insere em lisEQP todos os objetos VTEqpto
   LisBarra(lisEQP);
   LisEqbar(lisEQP);
   LisLigacao(lisEQP);
   LisMutua(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqpto(TList *lisEQP, int tipo)
   {
   //verifica o tipo de eqpto solicitado
   switch(tipo)
      {
      case eqptoBARRA:      LisBarra(lisEQP);         break;
      case eqptoEQBAR:      LisEqbar(lisEQP);         break;
      case eqptoCANAL:      LisCanal(lisEQP);         break;
      case eqptoCAPACITOR:  LisEqbar(lisEQP, tipo);   break;
      case eqptoCAPSERIE:   LisLigacao(lisEQP, tipo); break;
      case eqptoCARGA:      LisEqbar(lisEQP, tipo);   break;
      case eqptoCARGANL:    LisEqbar(lisEQP, tipo);   break;
      case eqptoCORTE:      LisCorte(lisEQP);         break;
      case eqptoCHAVE:      LisLigacao(lisEQP, tipo); break;
      case eqptoFILTRO:     LisEqbar(lisEQP, tipo);   break;
      case eqptoGERADOR:    LisEqbar(lisEQP, tipo);   break;
      case eqptoLIGACAO:    LisLigacao(lisEQP);       break;
      case eqptoMEDIDOR:    LisEqbar(lisEQP, tipo);   break;
      case eqptoMOTOR:      LisEqbar(lisEQP, tipo);   break;
      case eqptoMUTUA:      LisMutua(lisEQP);         break;
      case eqptoREATOR:     LisEqbar(lisEQP, tipo);   break;
      case eqptoREDE:       LisRede(lisEQP);          break;
      case eqptoREGULADOR:  LisLigacao(lisEQP, tipo); break;
      case eqptoSUPRIMENTO: LisEqbar(lisEQP, tipo);   break;
      case eqptoTRAFO:      LisLigacao(lisEQP, tipo); break;
      case eqptoTRAFO3E:    LisLigacao(lisEQP, tipo); break;
	  case eqptoTRAFOZZ:    LisEqbar(lisEQP, tipo);   break;
	  case eqptoTRECHO:     LisLigacao(lisEQP, tipo); break;
	  case eqptoBATERIA:    LisEqbar(lisEQP, tipo);   break;
      default:              break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqptoIsolado(TList *lisEQP)
   {
   //copia lista de Barras
   CopiaTList(isolada.rede->LisBarra(), lisEQP);
   //copia lista de Eqbar
   isolada.rede->LisEqbar(lisEQP);
   //copia lista de Ligacao
   isolada.rede->LisLigacao(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqptoIsolado(TList *lisEQP, int tipo)
   {
   isolada.rede->LisEqpto(lisEQP, tipo);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisEqptoNaoIsolado(TList *lisEQP, int tipo)
   {
   //obtém conjunto de todos Eqptos, não isolado e isolados
   LisEqpto(lisEQP, tipo);
   //obtém conjunto de Eqptos isolados
   lisTMP->Clear();
   LisEqptoIsolado(lisTMP, tipo);
   //exclui de lisEQP os Eqptos em lisTMP
   RetiraLis1DeLis2(lisTMP, lisEQP);
   //limpa lisTMP
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisLigacao(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      rede->LisLigacao(lisEQP);
      }
   //inclui Ligacoes da Rede isolada
   isolada.rede->LisLigacao(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisLigacao(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      rede->LisLigacao(lisEQP, tipo);
      }
   //inclui Ligacoes da Rede isolada
   isolada.rede->LisLigacao(lisEQP, tipo);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisLigacao(TList *lisEQP, VTBarra *barra)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      rede->LisLigacao(lisEQP, barra);
      }
   //inclui Ligacoes da Rede isolada
   isolada.rede->LisLigacao(lisEQP, barra);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisMNetFluxo(void)
   {
   return(lisMNET);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisMRede(void)
   {
   return(lisMR);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisMRede(TList *lisEQP)
   {
   CopiaTList(lisMR, lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisMRedeCarregada(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada)
         {
         if (lisEQP->IndexOf(mrede) < 0) lisEQP->Add(mrede);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisMRedeReduzida(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (! mrede->Carregada)
         {
         if (lisEQP->IndexOf(mrede) < 0) lisEQP->Add(mrede);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisMutua(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      rede->LisMutua(lisEQP);
      }
   //inclui Mutuas da Rede isolada
   isolada.rede->LisMutua(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedes::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisRede(TList *lisEQP)
   {
   CopiaTList(lisREDE, lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisRede(TList *lisEQP, int tipo_rede_id)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (rede->TipoRede->Id == tipo_rede_id) lisEQP->Add(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisRedeCarregada(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (rede->Carregada) lisEQP->Add(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisReducao(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;

   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisMR->Count; nr++)
      {
      mrede = (VTMRede*)lisMR->Items[nr];
      mrede->LisReducao(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::LisReducaoCarregada(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;

   //loop p/ todos objetos VTRede
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (! mrede->Carregada)
         {//insere Reducao (única) de MRedes NÃO carregadas
         mrede->LisReducao(lisEQP);
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TRedes::RedefineCurvaTipicaCarga(int dia)
   {
   //variáveis locais
   VTCarga  *carga;
   VTCurva  *curva;
   VTRede   *rede;
   VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //redefine dia de medicção do objeto Curvas
   curvas->DiaMedicao = dia;
   //loop p/ todos objetos VTRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //obtém lista de Cargas da Rede
      lisTMP->Clear();
      rede->LisEqbar(lisTMP, eqptoCARGA);
      //loop p/ todas Cargas da Rede
      for (int nc = 0; nc < lisTMP->Count; nc++)
         {
         carga = (VTCarga*)lisTMP->Items[nc];
         //proteção
         if ((carga->classe == NULL) || (carga->curva == NULL)) continue;
         //verifica se não é uma Carga típica
         if (! carga->curva->Tipica) continue;
         //dtermina nova Curva típica
         if (carga->IP)
            {//determina Curva de IP
            curva = curvas->ExisteCurvaTipicaIP();
            }
         else
            {//determina Curva típica de demnanda
            curva = curvas->ExisteCurvaTipicaCarga(carga->classe, carga->Energia_kwhmes, dia);
            }
         //redefine Curva da Carga
         if (curva) carga->DefineCurva(curva);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TRedes::RedeIsolada(void)
   {
   return(isolada.rede);
   }
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TRedes::RedefineIds(void)
{
	// variáveis locais
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTRede *rede;
	TList *lisREDE = this->LisRede();
	bool sucesso = true;

	if (!AllowRedefineIds)
	{
		return false;
	}
	try
	{
		// redefine IDs de todos Eqptos
		sucesso &= RedefineIds(arranjos->LisSuporte()); // DVK 2013.05.21
		sucesso &= RedefineIds(arranjos->LisCabo()); // DVK 2013.05.21
		sucesso &= RedefineIds(arranjos->LisArranjo()); // DVK 2013.05.21
		sucesso &= RedefineIds(this->LisDominio());
		sucesso &= RedefineIds(this->LisRede());
		sucesso &= RedefineIds(this->LisCorte());
		sucesso &= RedefineIds(this->LisBarra());
		lisTMP->Clear();
		this->LisEqbar(lisTMP);
		sucesso &= RedefineIds(lisTMP);
		lisTMP->Clear();
		this->LisLigacao(lisTMP);
		sucesso &= RedefineIds(lisTMP);
		lisTMP->Clear();
		this->LisReducao(lisTMP);
		sucesso &= RedefineIds(lisTMP);
		lisTMP->Clear();
		this->LisMutua(lisTMP);
		sucesso &= RedefineIds(lisTMP);
		lisTMP->Clear();
		// reinicia id das Barras inicias das redes
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			rede->DefineBarraInicial(rede->BarraInicial());
		}
		// importante: reordena lista de Barras de VTRedes
		VTListeqp *lisBAR = (VTListeqp*)(this->LisBarra());
		lisBAR->Ordena();
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TRedes::RedefineIds(TList *lisEQP)
{
	// variáveis locais
	VTEqpto *eqpto;
    bool sucesso = true;

	try
	{
		// loop p/ todos Eqptos
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[n];
			// eqpto->Id = FalsoId();
			if (eqpto->StatusNovo())
				eqpto->Id = FalsoId();
		}
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TRedes::ReiniciaLisBarra(TList *lisEQP)
    {
   //variáveis locais
   VTBarra *barra;

   //limpa lista de Barras sem destrui-las
   lisBAR->Clear();
   //copia lista Barras recebida
   CopiaTList(lisEQP, lisBAR);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::ReiniciaLisBarraIsolada(void)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR_ISO = isolada.rede->LisBarra();

   //limpa lista de Barras isoladas
   lisBAR_ISO->Clear();
   //verifica as Barras definidas como isoladas
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->Status[sttISOLADO]) lisBAR_ISO->Add(barra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::ReiniciaLisCorte(TList *lisEQP)
   {
   //variáveis locais
   VTCorte *corte, *corteI, *corteJ;

   //identifica Cortes originais iguais aos Cortes novos
   for (int i = 0; i < lisEQP->Count; i++)
      {
      corteI = (VTCorte*)lisEQP->Items[i];
      for (int j = 0; j < lisCOR->Count; j++)
         {
         corteJ = (VTCorte*)lisCOR->Items[j];
         if (corteJ == corteI)
            {//retira CorteJ da lista lisCOR
            lisCOR->Delete(j);
            break;
            }
         //verifica se o conjunto de Barras é igual nos dois Cortes
         if (Lista1IgualLista2(corteI->LisBarra(), corteJ->LisBarra()))
            {//atualiza ID do CorteI: copia ID do CorteJ
            corteI->Id = corteJ->Id;
            corteI->Status[sttNOVO]     = false;
            corteI->Status[sttALTERADO] = false;
            //retira CorteJ da lista lisCOR
            lisCOR->Delete(j);
            //destrói objeto
            delete corteJ;
            break;
            }
         }
      }
/*
   //elimina objetos Corte que restaram em lisCOR
   for (int nc = 0; nc < lisCOR->Count; nc++)
      {
      corte = (VTCorte*)lisCOR->Items[nc];
      if (corte->Id < 0)
         {//destrói objeto VTCorte
         delete corte;
         }
      else
         {//NÃO destrói objeto VTCorte
         //delete corte;
         //salva Eqpto excluído na Lixeira
         SalvaEqptoExcluidoNaLixeira(corte);
         }
      }
*/
   //elimina objetos Corte que restaram em lisCOR
   for (int nc = 0; nc < lisCOR->Count; nc++)
      {
      corte = (VTCorte*)lisCOR->Items[nc];
      //destrói objeto VTCorte
      delete corte;
      }
   //atualiza lisCOR
   lisCOR->Clear();
   CopiaTList(lisEQP, lisCOR);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::ReiniciaLisMNetFluxo(TList *lisEQP)
   {
   //limpa lisMRC
   LimpaTList(lisMNET);
   //copia lisEQP
   CopiaTList(lisEQP, lisMNET);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::ReiniciaLisMRede(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;

   //destrói VTMRedes que existiam em lisMR e não existem em lisEQP
   for (int n = lisMR->Count-1; n >= 0; n--)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (lisEQP->IndexOf(mrede) < 0)
         {//destrói objeto
         delete mrede;
         }
      }
   //reincia lisMR c/ as VTMRede de lisEQP
   lisMR->Clear();
   CopiaTList(lisEQP, lisMR);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::ReiniciaLisRede(TList *lisEQP)
   {
   //reinicia lista de Redes sem destruir os objetos
   lisREDE->Clear();
   //copia lista de Redes recebida
   CopiaTList(lisEQP, lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::RemoveBarra(VTBarra *pbarra)
   {
   //variáveis locais
   VTRede *rede;
   
   //proteção
   if (pbarra == NULL) return;
   //retira objeto VTBarra da lista lisBAR
   lisBAR->Remove(pbarra);
   //remove Barra de todas Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (rede->Carregada) rede->RemoveBarra(pbarra);
      }
   //remove Barra da lista de Equipamentos isolados
   isolada.rede->RemoveBarra(pbarra);
   //NÃO destrói objeto VTBarra
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::RemoveCluster(VTCluster *cluster)
   {
   //proteção
   if (cluster == NULL) return;
   //retira Cluster de lisCLUSTER e não destrói objeto
   lisCLUSTER->Remove(cluster);
   }
   
//---------------------------------------------------------------------------
void __fastcall TRedes::RemoveDominio(VTDominio *dominio)
   {
   //proteção
   if (dominio == NULL) return;
   //retira objeto VTDominio de lisDOMINIO
   lisDOMINIO->Remove(dominio);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::RemoveRede(VTRede *rede)
   {
   //variáveis locais
   VTDominio *dominio;

   //proteção
   if (rede == NULL) return;
   //retira objeto VTRede de lisREDE
   lisREDE->Remove(rede);
   //retira objeto VTRede do seu Cluster
   if (rede->Cluster)
      {
      rede->Cluster->RemoveRede(rede);
      rede->Cluster = NULL;
      }
   //retira objeto VTRede dos Dominios
   for (int n = 0; n < lisDOMINIO->Count; n++)
      {
      dominio = (VTDominio*)lisDOMINIO->Items[n];
      dominio->RemoveRede(rede);
      }
   //remove Rede da MRede
   if (rede->MRede)
      {
      (rede->MRede->LisRede())->Remove(rede);
      rede->MRede = NULL;
      }
   //NÃO destrói objeto VTRede
   }
   
/*
//---------------------------------------------------------------------------
void __fastcall TRedes::RemoveEqptoDaLixeira(VTEqpto *eqpto)
   {
   //remove Eqpto da Lixeira
   lixeira->RemoveEqpto(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::RetiraBarra(VTBarra *barra)
   {
   //variáveis locais
   VTRede *rede;

   //proteção
   if (barra == NULL) return;
   //remove Barra dos objetos VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->RemoveBarra(barra);
      }
   //remove Barra que pode ser isolada
   isolada.rede->RemoveBarra(barra);
   //retira objeto VTBarra de lisBAR
   lisBAR->Remove(barra);
   //NÃO destrói objeto VTBarra
   //delete barra;
   //salva Eqpto excluído na Lixeira
   SalvaEqptoExcluidoNaLixeira(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::RetiraCluster(VTCluster *cluster)
   {
   //proteção
   if (cluster == NULL) return;
   //retira Cluster de lisCLUSTER e destrói objeto
   lisCLUSTER->Remove(cluster);
   //NÃO destrói objeto VTCluster
   //delete cluster;
   //salva Eqpto excluído na Lixeira
   SalvaEqptoExcluidoNaLixeira(cluster);
   }

//---------------------------------------------------------------------------
void __fastcall TRedes::RetiraRede(VTRede *rede)
   {
   //proteção
   if (rede == NULL) return;
   //retira objeto VTRede de lisREDE
   lisREDE->Remove(rede);
   //NÃO destrói objeto VTRede
   //delete rede;
   //salva Eqpto excluído na Lixeira
   SalvaEqptoExcluidoNaLixeira(rede);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedes::SalvaEqptoExcluidoNaLixeira(VTEqpto *eqpto)
   {
   //armazena Eqpto na lixeira
   lixeira->InsereEpto(eqpto);
   }
*/
/*
//---------------------------------------------------------------------------
bool __fastcall TRedes::ValidaInsercaoGeracao(void)
   {
   //variáveis locais
   AnsiString txt;
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisREDE;
   TList     *lisREDU;

   //reinicia lisTMP
   lisTMP->Clear();
   //verifica se alguma Rede não carregada possui uma Reducao ficticia tipo geração
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //loop p/ todas as Reducoes da MRede não carregada (na verdade, existe 1 única Reducao)
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         if (! reducao->ExisteGeracao()) continue;
         if ((reducao->LisEqbar())->Count == 1) lisTMP->Add(mrede);
         }
      }
   //verifica se não encontrou MRede que impede a inserção de geração
   if (lisTMP->Count == 0) return(true);
   //gera aviso
   txt = "A rede em estudo não permite inserção de Gerador/Suprimento.\n"
         "Inclua as redes:\n";
   for (int n = 0; n < lisTMP->Count; n++)
      {
      mrede = (VTMRede*)lisTMP->Items[n];
      lisREDE = mrede->LisRede();
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         txt += "> " + rede->Codigo;
         }
      }
   Aviso(txt);
   //limpa lisTMP
   lisTMP->Clear();
   return(false);
   }
 */
//---------------------------------------------------------------------------
bool __fastcall TRedes::ValidaInsercaoGeracao(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedes::ViolaLimiteLicencaDemo(void)
   {
   //variáveis locais
   VTRede *rede;

   //verifica limite de barras
   if (lisBAR->Count > 1000) return(true);
   //verifica se há rede reduzida: isto é necessário p/ evitar que o usuário tente
   //editar uma rede por vez com até 1000 barras
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
//eof

