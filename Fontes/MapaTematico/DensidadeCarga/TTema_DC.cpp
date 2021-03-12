//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTema_DC.h"
#include "TFaixa.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
static int __fastcall ComparaValor(void *item1, void *item2)
   {//função para ordenar lista
   //variaveis locais
   VTCelula *c1 = (VTCelula*) item1;
   VTCelula *c2 = (VTCelula*) item2;

   if(c1->ValorProprio < c2->ValorProprio) return (-1);
   if(c1->ValorProprio > c2->ValorProprio) return ( 1);
   return (0);
   }

//---------------------------------------------------------------------------
VTTema_DC* __fastcall NewObjTema_DC(VTApl *apl)
   {
   try{
   return(new TTema_DC(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TTema_DC::TTema_DC(VTApl *apl)
   {
   //salva ponteiro p/ objeto externos
   this->apl = apl;
   //cria objetos
   lisEQP   = new TList();
   lisFAIXA = new TList();
   lisFAIXA_ENABLED = new TList();
   //inicia opção de modo degrade habilitado
   degrade_ok     = false;
   //cria objetos Faixa
   IniciaLisFaixa();    //teste
   //inicizaliza mostrando que as celulas ainda nao foram inicializadas
   lisCelula_iniciado = false;
   }

//---------------------------------------------------------------------------
__fastcall TTema_DC::~TTema_DC(void)
   {
   //variáveis locais
   TFaixa *faixa;

   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisFAIXA_ENABLED) {delete lisFAIXA_ENABLED; lisFAIXA_ENABLED = NULL;}
   //destrói lista e seus objetos
   if (lisFAIXA) {LimpaTList(lisFAIXA); delete lisFAIXA; lisFAIXA = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TTema_DC::CalculaValorTotalCelula(void)
   {
   //variáveis locais
   //int       num_celula;
   int     dist;
   double  fatmult;
   struct   {
            int       linha, coluna;
            VTCelula  *celula;
            }ref, viz;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //obtém dimensão do grid de celulas
   //num_celula = celulas->NumCelula;
   //zera valor total de todas celulas
   for (int i = 0; i < lisCEL->Count; i++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[i];
      ref.celula->ValorTotal = 0;
      ref.celula->NumParcela = 1;
      }
   //loop p/ todas celulas
   for (int i = 0; i < lisCEL->Count; i++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[i];
      //verifica se a célula de referência tem valor próprio
      if (IsDoubleZero(ref.celula->ValorProprio)) continue;
      //determina posicionamento da célula de referência no grid
      if (!  celulas->IndicesDaCelula(ref.celula, ref.linha, ref.coluna)) continue;
      //distribui seu valor na sua área de influência
      for (int j = 0; j < lisCEL->Count; j++)
         {
         viz.celula = (VTCelula*)lisCEL->Items[j];
         //determina posicionamento da célula vizinha no grid
         if (!  celulas->IndicesDaCelula(viz.celula, viz.linha, viz.coluna)) continue;
         //determina distância entre célula de referência e célula vizinha
         dist    = Max(abs(ref.linha - viz.linha), abs(ref.coluna - viz.coluna));
         //fatmult = (dist == 0) ? 1. : (1. / (dist * 8));
//Testes
fatmult = (dist == 0) ? 1. : (1. / (dist + 1));
         //totaliza contribuição da célula de referência na célula vizinha
         viz.celula->ValorTotal = viz.celula->ValorTotal + (ref.celula->ValorProprio * fatmult);
         }
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TTema_DC::Degrade(TColor color, double valor)
   {
   //variáveis locais
   unsigned r = ((color & 0xff)) * valor;
   unsigned g = ((color & 0xff00) >> 8) * valor;
   unsigned b = ((color & 0xff0000) >> 16) * valor;
   unsigned temp;

   //retorna tom de cor gerado
   return(TColor(r + (g << 8) + (b << 16)));
   }

//---------------------------------------------------------------------------
TColor __fastcall TTema_DC::Degrade(TColor beginColor, TColor endColor, double valor)
   {
   //decompõe beginColor
   unsigned r1 = ((beginColor & 0xff));
   unsigned g1 = ((beginColor & 0xff00) >> 8);
   unsigned b1 = ((beginColor & 0xff0000) >> 16);
   //decompõe endColor
   unsigned r2 = ((endColor & 0xff));
   unsigned g2 = ((endColor & 0xff00) >> 8);
   unsigned b2 = ((endColor & 0xff0000) >> 16);
   //mistura componentes de beginColor com endColor
   unsigned r = ((1 - valor) * r1) + (valor * r2);
   unsigned g = ((1 - valor) * g1) + (valor * g2);
   unsigned b = ((1 - valor) * b1) + (valor * b2);
   //retorna cor
   return(TColor(r + (g << 8) + (b << 16)));
   }

//---------------------------------------------------------------------------
TColor __fastcall TTema_DC::DegradeCinza(double valor)
   {
   //variáveis locais
   unsigned valor_hexa = 0xff * valor;

   //retorna tom de cinza
   return(TColor(valor_hexa + (valor_hexa << 8) + (valor_hexa << 16)));
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::Executa(TList *lisREDE, TList *lisCLASSE)
   {
   IniciaLisCelula(lisREDE,lisCLASSE);
   //NÃO prepara os dados p/ o degrade p/ melhorar performance
   degrade_ok = false;
   //monta faixas de densidade
   MontaFaixaDemanda();
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat)
   {
   IniciaLisCelula(lisREDE,lisCLASSE, ind_pat);
   //NÃO prepara os dados p/ o degrade p/ melhorar performance
   degrade_ok = false;
   //inicia com o "default"
   //FaixaValoresUniforme();
   //monta faixas de densidade
   MontaFaixaDemanda();
   }
////---------------------------------------------------------------------------
//void __fastcall  TTema_DC::Executa(TList *lisREDE, TList *lisCLASSE)
//   {
//   //variáveis locais
//   double      s_mva;
//   double      demandamax = 0.;
//   int         x_cm, y_cm;
//   VTCarga     *carga;
//   VTRede      *rede;
//   VTCelula    *celula;
//   VTPatamar   *patamar;
//   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
//   VTCelulas   *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
//   TList       *lisCELULA = celulas->LisCelula();
//   TList       *lisPAT  = new TList();
//
//   //guarda patamares habilitados
//   patamares->LisPatamarHabilitado(lisPAT);
//   //reinicia valores das Celulas
//   for (int nc = 0; nc < lisCELULA->Count; nc++)
//      {
//      celula = (VTCelula*)lisCELULA->Items[nc];
//      celula->ValorProprio = 0;
//      celula->ValorTotal   = 0;
//      }
//   //loop p/ todas Redes
//   for (int nr = 0; nr < lisREDE->Count; nr++)
//      {
//      rede = (VTRede*)lisREDE->Items[nr];
//      //obtém lista de Cargas da rede
//      IniciaLisCarga(rede, lisEQP);
//      //loop p/ todas Cargas
//      for (int nc = 0; nc < lisEQP->Count; nc++)
//         {
//         carga = (VTCarga*)lisEQP->Items[nc];
//         //verifica se Carga pertence a uma Classe selecionada
//         if (lisCLASSE->IndexOf(carga->classe) < 0) continue;
//         //determina coordenadas da Carga
//         if (! carga->pbarra->CoordenadasUtm_cm(x_cm, y_cm)) continue;
//         //determina Celula da Carga
//         if ((celula = celulas->ExisteCelula(x_cm, y_cm)) == NULL) continue;
//         //reinicia demandamax
//         demandamax = 0.;
//         //determina demanda máxima da Carga
//         for (int n = 0; n < lisPAT->Count; n++)
//            {//determina o Patamar
//            patamar = (VTPatamar*)lisPAT->Items[n];
//            //determina demanda no patamar
//            s_mva = Abs(demanda->S_mva[carga][patamar->Index]);
//            if (demandamax < s_mva)
//               {
//               demandamax = s_mva;
//               }
//            }
//         //atualiza valor próprio da Celula c/ a demanda máxima da Carga
//         celula->ValorProprio = celula->ValorProprio + demandamax;
//         }
//      }
//   delete lisPAT;
//   //NÃO prepara os dados p/ o degrade p/ melhorar performance
//   degrade_ok = false;
//   //monta faixas de densidade
//   MontaFaixaDemanda();
//   }
//
////---------------------------------------------------------------------------
//void __fastcall  TTema_DC::Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat)
//   {
//   //variáveis locais
//   int       x_cm, y_cm;
//   VTCarga   *carga;
//   VTRede    *rede;
//   VTCelula  *celula;
//   VTDemanda *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
//   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//   TList     *lisCELULA = celulas->LisCelula();
//
//   //reinicia valores das Celulas
//   for (int nc = 0; nc < lisCELULA->Count; nc++)
//      {
//      celula = (VTCelula*)lisCELULA->Items[nc];
//      celula->ValorProprio = 0;
//      celula->ValorTotal   = 0;
//      }
//   //loop p/ todas Redes
//   for (int nr = 0; nr < lisREDE->Count; nr++)
//      {
//      rede = (VTRede*)lisREDE->Items[nr];
//      //obtém lista de Cargas da rede
//      IniciaLisCarga(rede, lisEQP);
//      //loop p/ todas Cargas
//      for (int nc = 0; nc < lisEQP->Count; nc++)
//         {
//         carga = (VTCarga*)lisEQP->Items[nc];
//         //verifica se Carga pertence a uma Classe selecionada
//         if (lisCLASSE->IndexOf(carga->classe) < 0) continue;
//         //determina coordenadas da Carga
//         if (! carga->pbarra->CoordenadasUtm_cm(x_cm, y_cm)) continue;
//         //determina Celula da Carga
//         if ((celula = celulas->ExisteCelula(x_cm, y_cm)) == NULL) continue;
//         //atualiza valor próprio da Celula c/ a demanda da Carga
//         celula->ValorProprio = celula->ValorProprio + Abs(demanda->S_mva[carga][ind_pat]);
//         }
//      }
//   //NÃO prepara os dados p/ o degrade p/ melhorar performance
//   degrade_ok = false;
//   //monta faixas de densidade
//   MontaFaixaDemanda();
//   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::FaixaValoresDefault(void)
   {
   //variaveis locais
   TFaixa *faixa;

   //determina valores default
   double limite[16] = {0.000, 0.015, 0.030, 0.050, 0.075, 0.100, 0.150, 0.200,
                        0.500, 1.0,   1.5,   2.0,   2.5,   5.0,  10.0,   15.0};
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {//inicia atributos
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa->dem_min_mva  = limite[nf];
      faixa->dem_max_mva  = limite[nf+1];
      }
   }
//---------------------------------------------------------------------------

/*
//---------------------------------------------------------------------------
void __fastcall  TTema_DC::FaixaValoresDemanda(void)
   {
   //variáveis locais
   double    media;
   TFaixa    *faixa;
   VTCelula  *celula;
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   TList     *lisORDENADA;

   //proteção
   if(lisFAIXA->Count == 0)  return;
   //inicia valores
   media = 0.;
   //cria lista
   lisORDENADA = new TList();
   //copia itens da lista
   CopiaTList(lisCELULA, lisORDENADA);
   //remove celulas com valor nulo
   RemoveCelulaNula(lisORDENADA);
   //ordena celulas
   if (lisORDENADA->Count > 1) lisORDENADA->Sort(ComparaValor);
   //verifica maior celula, comparado à quantidade de faixas
   if(lisORDENADA->Count > 0)
      {
      celula = (VTCelula*)lisORDENADA->Last();
      media = (celula->ValorProprio / (lisFAIXA->Count));
      }
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {//insere atributos
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa->dem_min_mva  = (media * nf);
      faixa->dem_max_mva  = (media * (nf + 1));
      }
   //destroi lista
   delete lisORDENADA;
   }
*/
//---------------------------------------------------------------------------
void __fastcall  TTema_DC::FaixaValoresDemanda(void)
   {
   //variáveis locais
   int       index;
   double    valor_ini, valor_fim, media, total;
   TFaixa    *faixa;
   VTCelula  *celula;
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   TList     *lisORDENADA;
   //TList     *lisENABLED;

   //proteção
   if(lisFAIXA->Count == 0) return;
   //inicia valores
   valor_ini = valor_fim = media = total = 0.;
   index = 0;
   //cria lista
   lisORDENADA = new TList();
   //preenche lista com as faixas habilitadas
//   for (int nl = 0; nl < lisFAIXA->Count; nl++)
//      {
//      faixa = (TFaixa*)lisFAIXA->Items[nl];
//      if(faixa->enabled)
//         {lisFAIXA_ENABLED->Add(faixa);}
//      }
   IniciaLisFaixaEnabled();
   //copia itens da lista
   CopiaTList(lisCELULA, lisORDENADA);
   //remove celulas com valor nulo
   RemoveCelulaNula(lisORDENADA);
   //ordena celulas
   if (lisORDENADA->Count > 1) lisORDENADA->Sort(ComparaValor);
   //verifica valor total das celulas, comparado à quantidade de faixas
   for (int nc = 0; nc < lisORDENADA->Count; nc++)
      {
      celula = (VTCelula*)lisORDENADA->Items[nc];
      total += celula->ValorProprio;
      }
   //media = ((total) / (lisFAIXA->Count + 1));
   media = ((total) / (lisFAIXA_ENABLED->Count + 1));
   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
      {//salva ultimo valor
      valor_ini = valor_fim;
      //reinicia total
      total = 0.;
      while((index < (lisORDENADA->Count)) && (total < media))
         {//acumula valores das celulas que compoem a faixa
         celula = (VTCelula*)lisORDENADA->Items[index];
         total += celula->ValorProprio;
         index++;
         }
      if(celula) valor_fim = celula->ValorProprio;
      //insere atributos
      //faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
      faixa->dem_min_mva  = valor_ini;
      faixa->dem_max_mva  = valor_fim;
      }
   //destroi lista
   delete lisORDENADA;
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::FaixaValoresQtdCelula(void)
   {
   //variáveis locais
   int       media, index;
   double    valor_ini, valor_fim;
   TFaixa    *faixa;
   VTCelula  *celula;
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   TList     *lisORDENADA;


   //proteção
   if(lisFAIXA->Count == 0) return;
   //inicia valores
   valor_ini = valor_fim = 0.;
   index = 0;
   //cria lista
   lisORDENADA = new TList();
   //lisENABLED = new TList();
   //preenche lista com as faixas habilitadas
//   for (int nl = 0; nl < lisFAIXA->Count; nl++)
//      {
//      faixa = (TFaixa*)lisFAIXA->Items[nl];
//      if(faixa->enabled)
//         {lisENABLED->Add(faixa);}
//      }
   IniciaLisFaixaEnabled();
   //copia itens da lista
   CopiaTList(lisCELULA, lisORDENADA);
   //remove celulas com valor nulo
   RemoveCelulaNula(lisORDENADA);
   //ordena celulas
   if (lisORDENADA->Count > 1) lisORDENADA->Sort(ComparaValor);
   //verifica quantidade de celulas, comparado à quantidade de faixas
   //(a media sempre será, no mínimo, 1)
   //media = int((lisORDENADA->Count) / (lisFAIXA->Count)) + 1;
   media = int((lisORDENADA->Count) / (lisFAIXA_ENABLED->Count)) + 1;
   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
      {//salva ultimo valor
      valor_ini = valor_fim;
      while((index < lisORDENADA->Count) && (index <((nf+1)*media)))
         {//acumula valores das celulas que compoem a faixa
         celula = (VTCelula*)lisORDENADA->Items[index];
         valor_fim = celula->ValorProprio;
         index++;
         }
      //insere atributos
      //faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
      faixa->dem_min_mva  = valor_ini;
      faixa->dem_max_mva  = valor_fim;
      }
   //destroi lista
   delete lisORDENADA;
   }

//---------------------------------------------------------------------------
void   __fastcall  TTema_DC::FaixaValoresUniforme(void)
 {
   //variáveis locais
   int       index;
   double    valor_ini, valor_fim;//, media, total;
   TFaixa    *faixa;
   VTCelula  *celula;
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   TList     *lisORDENADA;

   double maiorTotal = 0;
   double intervaloDem =0;

   //proteção
   if(lisFAIXA->Count == 0) return;
   //inicia valores
   valor_ini = valor_fim = 0.;
   index = 0;
   //cria lista
   lisORDENADA = new TList();
   //preenche lista com as faixas habilitadas
   IniciaLisFaixaEnabled();
   //copia itens da lista
   //CopiaTList(lisCELULA, lisORDENADA);
   //remove celulas com valor nulo
   //RemoveCelulaNula(lisORDENADA);
   //ordena celulas
   //if (lisORDENADA->Count > 1) lisORDENADA->Sort(ComparaValor);
   //verifica valor total das celulas, comparado à quantidade de faixas
   for (int nc = 0; nc < lisCELULA->Count; nc++)
      {
      celula = (VTCelula*)lisCELULA->Items[nc];
      //guarda o maior valor
      if(maiorTotal < celula->ValorProprio) maiorTotal = celula->ValorProprio;
      }
   //intervaloDem = ((total) / (lisFAIXA->Count + 1));
   intervaloDem = ((maiorTotal) / (lisFAIXA_ENABLED->Count + 1));
   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
      {//salva ultimo valor
      valor_ini = valor_fim;
      //reinicia total
      valor_fim = valor_ini + intervaloDem;
      //insere atributos
      //faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
      faixa->dem_min_mva  = valor_ini;
      faixa->dem_max_mva  = valor_fim;
      }
   //destroi lista
   delete lisORDENADA;
   //delete lisENABLED;
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::IniciaLisCarga(VTRede *rede, TList *lisCARGA)
   {
   //variáveis locais
   TList     *lisREDU;
   VTReducao *reducao;

   //reinicia lisCARGA
   lisCARGA->Clear();
   //verifica se a Rede está carregada
   if (rede->Carregada)
      {
      rede->LisEqbar(lisCARGA, eqptoCARGA);
      }
   else
      {//determina Reducao da Rede
      lisREDU = rede->MRede->LisReducao();
      if (lisREDU->Count == 1)
         {
         reducao = (VTReducao*)lisREDU->First();
         reducao->LisEqpto(lisCARGA, eqptoCARGA);
         }
      }
   }
//---------------------------------------------------------------------------
void   __fastcall TTema_DC::IniciaLisCelula(TList *lisREDE, TList *lisCLASSE, int ind_pat)
   {//variáveis locais
   int       x_cm, y_cm;
   VTCarga   *carga;
   VTRede    *rede;
   VTCelula  *celula;
   VTDemanda *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   //caso 2
   double      s_mva;
   double      demandamax = 0.;
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT  = new TList();

   try
      {//reinicia valores das Celulas
      for (int nc = 0; nc < lisCELULA->Count; nc++)
         {
         celula = (VTCelula*)lisCELULA->Items[nc];
         celula->ValorProprio = 0;
         celula->ValorTotal   = 0;
         }
      //loop p/ todas Redes
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //obtém lista de Cargas da rede
         IniciaLisCarga(rede, lisEQP);
         //loop p/ todas Cargas
         for (int nc = 0; nc < lisEQP->Count; nc++)
            {
            carga = (VTCarga*)lisEQP->Items[nc];
            //verifica se Carga pertence a uma Classe selecionada
            if (lisCLASSE->IndexOf(carga->classe) < 0) continue;
            //determina coordenadas da Carga
            if (! carga->pbarra->CoordenadasUtm_cm(x_cm, y_cm)) continue;
            //determina Celula da Carga
            if ((celula = celulas->ExisteCelula(x_cm, y_cm)) == NULL) continue;
            //agora difere de acordo com a config do patamar
            if(ind_pat < 0)
               {
               //guarda patamares habilitados
               patamares->LisPatamarHabilitado(lisPAT);
               //reinicia demandamax
               demandamax = 0.;
               //determina demanda máxima da Carga
               for (int n = 0; n < lisPAT->Count; n++)
                  {//determina o Patamar
                  patamar = (VTPatamar*)lisPAT->Items[n];
                  //determina demanda no patamar
                  s_mva = Abs(demanda->S_mva[carga][patamar->Index]);
                  if (demandamax < s_mva)
                     {
                     demandamax = s_mva;
                     }
                  }
               //atualiza valor próprio da Celula c/ a demanda máxima da Carga
               celula->ValorProprio = celula->ValorProprio + demandamax;
               }
            else
               {//atualiza valor próprio da Celula c/ a demanda da Carga
               celula->ValorProprio = celula->ValorProprio + Abs(demanda->S_mva[carga][ind_pat]);
               }
            }
         }
      //mostra que foi inicializado
      lisCelula_iniciado = true;
      }
   catch (Exception &e)
      {//mostra que foi  Não inicializado
      lisCelula_iniciado = false;
      }
   //deleta a lista criada
   if(lisPAT) delete lisPAT;
   }
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------

void __fastcall  TTema_DC::IniciaLisFaixa(void)
   {
   //variáveis locais
   TFaixa *faixa;
   int    num_faixa = 15;
   TColor color[15] = {clGreen, clYellow,     clRed,     clBlue, clLime,
                       clAqua,  clMoneyGreen, clSkyBlue, clTeal, clOlive,
                       clGray,  clCream,      clFuchsia, clNavy, clPurple};
   double limite[16] = {0.000, 0.015, 0.030, 0.050, 0.075, 0.100, 0.150, 0.200,
                        0.500, 1.0,   1.5,   2.0,   2.5,   5.0,  10.0,   15.0};

   for (int nf = 0; nf < num_faixa; nf++)
      {//cria objeto Faixa
      lisFAIXA->Add(faixa = new TFaixa());
      //inicia atributos
      faixa->enabled      = true;
      faixa->visible      = true;
      faixa->color        = color[nf];
      faixa->dem_min_mva  = limite[nf];
      faixa->dem_max_mva  = limite[nf+1];
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall  TTema_DC::IniciaLisFaixa(void)
   {
   //variáveis locais
   TFaixa *faixa;
   int    num_faixa = 15;
   TColor color[15] = {clGreen, clYellow,     clRed,     clBlue, clLime,
                       clAqua,  clMoneyGreen, clSkyBlue, clTeal, clOlive,
                       clGray,  clCream,      clFuchsia, clNavy, clPurple};

   for (int nf = 0; nf < num_faixa; nf++)
      {//cria objeto Faixa
      lisFAIXA->Add(faixa = new TFaixa());
      //inicia atributos
      faixa->enabled      = true;
      faixa->visible      = true;
      faixa->color        = color[nf];
      }

   //insere valores default dos limites
   FaixaValoresDefault();
   //FaixaValoresUniforme(); //FKM 2016.07.14
   }

//---------------------------------------------------------------------------
void   __fastcall TTema_DC::IniciaLisFaixaEnabled(void)
   {//variáveis locais
   TFaixa    *faixa;

   lisFAIXA_ENABLED->Clear();
   for (int nf = 0; nf< lisFAIXA->Count; nf++)
   {
   faixa = (TFaixa*)lisFAIXA->Items[nf];
   if(faixa->enabled)
      {lisFAIXA_ENABLED->Add(faixa);}
   }
   }
//---------------------------------------------------------------------------
TList* __fastcall TTema_DC::LisFaixa(void)
   {
   return(lisFAIXA);
   }

//---------------------------------------------------------------------------
void __fastcall TTema_DC::MontaFaixaDemanda(void)
   {
   //variáveis locais
   TFaixa    *faixa;
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();

   //IMPORTANTE: monta faixas de demanda fixas, independente do total da rede

   //reinicia atributos das Faixas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      //reinicia lista de Celulas
      faixa->lisCELULA->Clear();
      //define valores da faixa
      //faixa->visible     = true;
      faixa->dem_tot_mva = 0.;
      }
   //inicializa faixas habilitadas
   IniciaLisFaixaEnabled();
   //determina Celulas de cada faixa
   for (int n = 0; n < lisCELULA->Count; n++)
      {
      celula = (VTCelula*)lisCELULA->Items[n];
      if (fabs((long double)(celula->ValorProprio > 0)))
         {//determina a Faixa p/ a Celula
         //for (int nf = 0; nf < lisFAIXA->Count; nf++)
         for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
            {
            //faixa = (TFaixa*)lisFAIXA->Items[nf];
            faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
            if (! faixa->enabled) continue;
            //verifica se o valor próprio da celula está dentro dos limites da faixa ou se é a última faixa
            if ((celula->ValorProprio <= faixa->dem_max_mva)||(nf == (lisFAIXA->Count-1)))
               {//insere Celula na lista da faixa
               faixa->lisCELULA->Add(celula);
               faixa->dem_tot_mva += celula->ValorProprio;
               //interrompe o loop
               break;
               }
            }
         }
      }
   //calcula total de Celulas e de demanda das faixas
   total.qtde_cel = 0;
   total.s_mva    = 0;
   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
      {
      //faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
      total.qtde_cel += faixa->lisCELULA->Count;
      total.s_mva    += faixa->dem_tot_mva;
      }
   //calcula percetual de Celulas e de demanda das faixas
   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
      {
      //faixa = (TFaixa*)lisFAIXA->Items[nf];
      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
      if (total.qtde_cel == 0)       faixa->num_cel_perc = 0;
      else                           faixa->num_cel_perc = 100. *(double(faixa->lisCELULA->Count) / double(total.qtde_cel)) ;
      if (IsDoubleZero(total.s_mva)) faixa->dem_tot_perc = 0.;
      else                           faixa->dem_tot_perc = 100. *(faixa->dem_tot_mva / total.s_mva);
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::MontaGraficoDegrade(TColor color)
   {
   //variáveis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //verifica se os dados p/ o degrade estão calculados
   if (! degrade_ok)
      {//calcula valor total das celulas
      CalculaValorTotalCelula();
      //normaliza valor total das Celulas
      NormalizaValorTotalCelula();
      //marca que os dados p/ o degrade estão calculados
      degrade_ok = true;
      }
   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      //define cor da célula em função do seu valor
      celula->Color = Degrade(color, celula->ValorTotal);
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::MontaGraficoDemandaMax(void)
   {
   MontaGraficoPadrao();
   }

 /*
//---------------------------------------------------------------------------
bool __fastcall  TTema_DC::MontaGraficoDegradeFaixa(TColor color1, TColor color2)
   {
   //variáveis locais
   bool       first = true;
   double     dem_med, dem_min, dem_max;
   TFaixa     *faixa;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //determina valores mínimo e máximo da demanda média das faixas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      //verifica se a Faixa possui Celulas
      if (faixa->lisCELULA->Count == 0) continue;
      dem_med = faixa->dem_tot_mva / faixa->lisCELULA->Count;
      //verifica se é a primeira Faixa com Celulas
      if (first)
         {
         first = false;
         dem_min = dem_max = dem_med;
         }
      dem_min = min(dem_min, dem_med);
      dem_max = max(dem_max, dem_med);
      }
   //proteção
   if (IsDoubleZero(dem_max)) return(false);
   //define cor das Celulas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      //verifica se a Faixa possui Celulas
      if (faixa->lisCELULA->Count == 0)
         {
         faixa->color = redegraf->CorFundo;
         }
      else
         {
         dem_med = faixa->dem_tot_mva / faixa->lisCELULA->Count;
         faixa->color = Degrade(color1, color2, (dem_med - dem_min)/dem_max);
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
/*
bool __fastcall  TTema_DC::MontaGraficoDegradeFaixa(TColor color1, TColor color2)
   {
   //variáveis locais
   double     valor;
   TFaixa     *faixa;

   //proteção
   if (lisFAIXA->Count == 0) return(false);
   //define cor das Celulas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      valor = double(nf+1)/lisFAIXA->Count;
      faixa->color = Degrade(color1, color2, valor);
      }
   return(true);
   }
 */
 //FKM 2015.10.01 Monta degradÊ somente pras faixas habilitadas
 //---------------------------------------------------------------------------
 bool __fastcall  TTema_DC::MontaGraficoDegradeFaixa(TColor color1, TColor color2, bool linearDem)
   {
   //variáveis locais
   double     valor;
   TFaixa     *faixa;
   double     max, min;
   //TList      *lisFAIXA_ENABLED = new TList;

   //proteção
   if (lisFAIXA->Count == 0) return(false);
   //inicializa a lista de faixas habilitadas
   IniciaLisFaixaEnabled();
   //proteção
   if (lisFAIXA_ENABLED->Count == 0) return(false);
   //se não for o tipo de degrade relacioando com a demanda
   if(!linearDem)
      {
      //define cor das Celulas
      for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
         {
         faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
         valor = double(nf+1)/lisFAIXA_ENABLED->Count;
         faixa->color = Degrade(color1, color2, valor);
         }
      }
   else
      {//faz o degrade de acordo com a demanda
      //verifica o intervalo de demanda das faixas
      for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
         {
         faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
         //inicializa max e min
         if(nf == 0)
            {
            max = faixa->dem_max_mva;
            min = faixa->dem_min_mva;
            }
         //guarda os maiores e menores
         if(max < faixa->dem_max_mva) max = faixa->dem_max_mva;
         if(min > faixa->dem_min_mva) min = faixa->dem_min_mva;
         }
      //protecao
      if (IsDoubleZero(max))
         {max = 1.0;}
      //define cor das Celulas
      for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
         {
         faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];

         //valor = double(nf+1)/lisFAIXA_ENABLED->Count;
         valor = faixa->dem_max_mva / max;
         if(valor >= 1.0) valor = 1.0;
         faixa->color = Degrade(color1, color2, valor);
         }
      }
   return(true);
   }
  //---------------------------------------------------------------------------
//bool __fastcall  TTema_DC::MontaGraficoDegradeFaixaLinear(TColor color1, TColor color2)
//   {
//   //variáveis locais
//   double     valor;
//   TFaixa     *faixa;
//   //TList      *lisFAIXA_ENABLED = new TList;
//   //
//   double     max, min;
//
//   //proteção
//   if (lisFAIXA->Count == 0) return(false);
//   //inicializa a lista de faixas habilitadas
//   IniciaLisFaixaEnabled();
//   //proteção
//   if (lisFAIXA_ENABLED->Count == 0) return(false);
//   //verifica o intervalo de demanda das faixas
//   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
//      {
//      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
//      //inicializa max e min
//      if(nf == 0)
//         {
//         max = faixa->dem_max_mva;
//         min = faixa->dem_min_mva;
//         }
//      //guarda os maiores e menores
//      if(max < faixa->dem_max_mva) max = faixa->dem_max_mva;
//      if(min > faixa->dem_min_mva) min = faixa->dem_min_mva;
//      }
//   //protecao
//   if (IsDoubleZero(max))
//      {max = 1.0;}
//   //define cor das Celulas
//   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
//      {
//      faixa = (TFaixa*)lisFAIXA_ENABLED->Items[nf];
//
//      //valor = double(nf+1)/lisFAIXA_ENABLED->Count;
//      valor = faixa->dem_max_mva / max;
//      if(valor >= 1.0) valor = 1.0;
//      faixa->color = Degrade(color1, color2, valor);
//      }
//   return(true);
//   }
//---------------------------------------------------------------------------
void __fastcall  TTema_DC::MontaGraficoPadrao(void)
   {
   //variáveis locais
   TFaixa     *faixa;
   VTCelula   *celula;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTCelulas  *celulas  = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList      *lisCEL   = celulas->LisCelula();

   //reinicia cor de todas as celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      //define cor da célula em função do seu valor
      celula->Color = redegraf->CorFundo;
      }
   //loop p/ todas Faixa
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      if (faixa->visible)
         {//define cor das Celuas da Faixa
         for (int n = 0; n < faixa->lisCELULA->Count; n++)
            {
            celula = (VTCelula*)faixa->lisCELULA->Items[n];
            //define cor da célula em função do seu valor
            celula->Color = faixa->color;
            }
         }
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall  TTema_DC::NormalizaValorTotalCelula(void)
   {
   //variáveis locais
   bool      first = true;
   double    valor_max, valor_min, dividendo;
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      if (celula->NumParcela == 0) continue;
      if (first) {valor_min = valor_max = celula->ValorTotal; first = false;}
      valor_max = Max(valor_max, fabs(celula->ValorTotal));
      valor_min = Min(valor_min, fabs(celula->ValorTotal));
      }
   //determina dividendo
   dividendo = valor_max - valor_min;
   //proteção
   if (IsDoubleZero(dividendo)) return;
   //calcula valor normalizado das celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      celula->ValorTotal = (celula->ValorTotal - valor_min) / dividendo;
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_DC::RemoveCelulaNula(TList *lisORDENADA)
   {
   //variáveis locais
   VTCelula  *celula;

   for(int nc = lisORDENADA->Count - 1; nc >= 0; nc--)
      {//retira da lista as celulas com valor nulo
      celula = (VTCelula*)lisORDENADA->Items[nc];
      if(IsDoubleZero(celula->ValorProprio)) lisORDENADA->Delete(nc);
      }
   }

//---------------------------------------------------------------------------
int __fastcall  TTema_DC::TotalCelula(void)
   {
   return(total.qtde_cel);
   }

//---------------------------------------------------------------------------
double __fastcall  TTema_DC::TotalDemanda_mva(void)
   {
   return(total.s_mva);
   }

//---------------------------------------------------------------------------
//eof

