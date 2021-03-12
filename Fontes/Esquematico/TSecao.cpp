//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TSecao.h"
#include "Esquematico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSecao
//---------------------------------------------------------------------------
VTSecao* __fastcall NewObjSecao(int tipo)
   {
   return(new TSecao(tipo));
   }

//---------------------------------------------------------------------------
__fastcall TSecao::TSecao(int tipo)
   {
   //salva tipo da Secao
   PD.tipo = tipo;
   //cria objetos
   lisBAR = new TList();
   lisLIG = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSecao::~TSecao(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TSecao::BarraFinal(void)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //proteção: pode não haver Ligacao
   if (lisLIG->Count == 0) return(NULL);
   ligacao = (VTLigacao*) lisLIG->Last();
   return(ligacao->pbarra2);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TSecao::BarraInicial(void)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //proteção: pode não haver Ligacao
   if (lisLIG->Count == 0) return(NULL);
   ligacao = (VTLigacao*) lisLIG->First();
   return(ligacao->pbarra1);
   }

//---------------------------------------------------------------------------
int __fastcall TSecao::ComprimentoUtm_cm(void)
   {
   //variáveis locais
   int       comp = 0;
   VTLigacao *ligacao;

   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      comp   += ligacao->ComprimentoUtm_cm();
      }
   return(comp);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TSecao::DefineBarraFinalFixa(bool fixa)
   {
   barra_final_fixa = fixa;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TSecao::DefineSentido(void)
   {
   //variáveis locais
   int       dx, dy;
   int       xa1, ya1, xa2, ya2;
   int       xb1, yb1, xb2, yb2;
   VTBarra   *barra1, *barra2;

   //determina Barras inicial e final
   barra1 = BarraInicial();
   barra2 = BarraFinal();
//if (barra1->Id == 164)
//{int a=0;}
   //determina coordenas esquemático das Barras
   barra1->CoordenadasEsquematico(xa1, ya1, xa2, ya2);
   barra2->CoordenadasEsquematico(xb1, yb1, xb2, yb2);
   dx = xb1 - xa1;
   dy = yb1 - ya1;
   //verifica se a Secao possui uma Secao pai
   if (SecaoPai == NULL)
      {
      if (abs(dx) > abs(dy))
         {//horizontal
         PD.sentido = (dx > 0) ? HoriLeftRight : HoriRightLeft;
         }
      else
         {//vertical
         PD.sentido = (dy > 0) ? VertTopBottom : VertBottomTop;
         }
      }
   else
      {//analisa sentido da Secao pai
      switch(SecaoPai->Sentido)
         {
         case HoriLeftRight:
         case HoriRightLeft:
            //Secao deve ser vertical
            PD.sentido = (dy > 0) ? VertTopBottom : VertBottomTop;
            break;
         case VertBottomTop:
         case VertTopBottom:
            //Secao deve ser horizontal
            PD.sentido = (dx > 0) ? HoriLeftRight: HoriRightLeft;
            break;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TSecao::ExisteBarra(VTBarra *barra)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (ligacao->IndiceBarra(barra) >= 0) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TSecao::ExisteLigacao(VTLigacao *ligacao)
   {
   return(lisLIG->IndexOf(ligacao) >= 0);
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::InsereLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return;
   //insere Ligacao no início da lista
   if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Insert(0, ligacao);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TSecao::LigacaoFinal(void)
   {
   if (lisLIG->Count == 0) return(NULL);
   return((VTLigacao*)lisLIG->Last());
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TSecao::LigacaoInicial(void)
   {
   if (lisLIG->Count == 0) return(NULL);
   return((VTLigacao*)lisLIG->First());
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::LisBarra(TList *lisBAR)
   {
   //variáveis locais
   VTLigacao *ligacao;
   
   //limpa lisBAR
   lisBAR->Clear();
   //loop p/ todas as Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //insere Barra inicial da primeira Ligacao
      if (nl == 0) lisBAR->Add(ligacao->pbarra1);
      //insere Barra final da Ligacao
      lisBAR->Add(ligacao->pbarra2);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TSecao::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::Ordena(bool bar_final_fixa)
   {
   //verifica se Secao em malha
   if (Tipo == secaoMALHA)
      {
      OrdenaMalha();
      return;
      }
   //verifica se última Barra é fixa
   if (bar_final_fixa)
      {//alinha Ligacoes por equação de reta definida pelas Barras inicial e final
      OrdenaRetaEquacao();
      return;
      }
   //alinha Ligacoes de acordo c/ sentido da Secao
   switch(Sentido)
      {
      case HoriLeftRight: OrdenaRetaHoriLeftRight();  break;
      case HoriRightLeft: OrdenaRetaHoriRightLeft();  break;
      case VertTopBottom: OrdenaRetaVertTopBottom();  break;
      case VertBottomTop: OrdenaRetaVertBottomTop();  break;
      }
   //ordena sempre por equação de reta definida pelas Barras inicial e final
   //OrdenaRetaEquacao();
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::OrdenaMalha(void)
   {
   //variáveis locais
   int     x, y;
   VTBarra *barra;

   //CÓDIGO TEMPORÁRIO: define todas as Barras c/ mesma coordenadas

   //determina lista de Barras
   lisBAR->Clear();
   LisBarra(lisBAR);
   //determina valor médio das coordenadas das Barras
   x = y = 0;
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra  = (VTBarra*)lisBAR->Items[nb];
      x     += barra->esq.x1;
      y     += barra->esq.y1;
      }
   x /= lisBAR->Count;
   y /= lisBAR->Count;
   //define coordenadas de todas as Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra  = (VTBarra*)lisBAR->Items[nb];
      barra->esq.x1 = x;
      barra->esq.y1 = y;
      barra->esq.x2 = x;
      barra->esq.y2 = y;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::OrdenaRetaEquacao(void)
   {
   //variáveis locais
   int        dx, dy, xa1, ya1, xa2, ya2, xb1, yb1, xb2, yb2;
   double     comp, comp_total;
   double     cat1, cat2, hipot, sen_alfa, cos_alfa;
   double     fat_esc_x, fat_esc_y;
   VTBarra   *barra2;
   VTLigacao *ligacao;

   //determina coordenadas da Ligacoes inicial e final
   ligacao = (VTLigacao*)lisLIG->First();
   ligacao->CoordenadasEsquematico(xa1, ya1, xa2, ya2);
   ligacao = (VTLigacao*)lisLIG->Last();
   ligacao->CoordenadasEsquematico(xb1, yb1, xb2, yb2);
   dx     = xb2 - xa1;
   dy     = yb2 - ya1;
   if ((dx == 0)&&(dy == 0)) return;
   //calcula seno e cosseno da reta definida pelas Barras inicial e final da Secao
   cat1     = double(dx)/100;
   cat2     = double(dy)/100;
   hipot    = sqrt((cat1*cat1)+(cat2*cat2)) * 100;
   cos_alfa = dx / hipot;
   sen_alfa = dy / hipot;
   //determina comprimento total das Ligacoes
   comp_total = 0;
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      //NÃO utliza função ligacao->ComprimentoEsq() porque as coord. das Barras estão sendo alteradas
      comp_total += ligacao->esq.comprimento;
      }
   if (comp_total == 0) return;
   //calcula fatores de escala p/ os eixos x e y
   fat_esc_x = (hipot / comp_total) * cos_alfa;
   fat_esc_y = (hipot / comp_total) * sen_alfa;
   //redefine coordenadas das Barras das Ligacoes da Secao
   for (int n = 0; n < lisLIG->Count - 1; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      barra2  = ligacao->pbarra2;
      comp    = ligacao->esq.comprimento;
      //determina cooodenadas da Ligacao
      ligacao->CoordenadasEsquematico(xa1, ya1, xa2, ya2);
      //calcula dimensão da Barra2
      dx = barra2->esq.x2 - barra2->esq.x1;
      dy = barra2->esq.y2 - barra2->esq.y1;
      //recalcula coordenadas (x1,y1) da Barra2
      barra2->esq.x1 = xa1 + (comp * fat_esc_x) + dx/2;
      barra2->esq.y1 = ya1 + (comp * fat_esc_y) - dy/2;
      //define coord.(x2,y2) da Barra2
      barra2->esq.x2 = xa1 + (comp * fat_esc_x) - dx/2;
      barra2->esq.y2 = ya1 + (comp * fat_esc_y) + dy/2;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::OrdenaRetaHoriRightLeft(void)
   {
   //variáveis locais
   int       dx, dy, x1, y1, x2, y2;
   double    comp;
   VTBarra   *barra1, *barra2;
   VTLigacao *ligacao;

   //loop p/ todas Ligacoes da Secao
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      comp    = ligacao->esq.comprimento;
      barra2  = ligacao->pbarra2;
      //determina cooodenadas da Ligacao
      ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
//se primeira Ligacao, ajusta coordenadas da Barra 1
if (n == 0)
   {//ajusta coordenadas da Barra inicial
   barra1  = ligacao->pbarra1;
   barra1->esq.x1 = barra1->esq.x2 = x1;
   barra1->esq.y1 = barra1->esq.y2 = y1;
   }
      //calcula dimensão da Barra2
      //dx = barra2->esq.x2 - barra2->esq.x1;
      //dy = barra2->esq.y2 - barra2->esq.y1;
dx = 0;
dy = 0;
      //redefine coord.(x1,y1) da Barra2
      barra2->esq.x1 = x1 - comp - dx/2;
      barra2->esq.y1 = y1 - dy/2;
      //redefine coord.(x2,y2) da Barra2
      barra2->esq.x2 = x1 - comp + dx/2;
      barra2->esq.y2 = y1 + dy/2;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::OrdenaRetaHoriLeftRight(void)
   {
   //variáveis locais
   int       dx, dy, x1, y1, x2, y2;
   double    comp;
   VTBarra   *barra1, *barra2;
   VTLigacao *ligacao;

   //loop p/ todas Ligacoes da Secao
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      comp    = ligacao->esq.comprimento;
      barra2  = ligacao->pbarra2;
      //determina cooodenadas da Ligacao
      ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
//se primeira Ligacao, ajusta coordenadas da Barra 1
if (n == 0)
   {//ajusta coordenadas da Barra inicial
   barra1  = ligacao->pbarra1;
   barra1->esq.x1 = barra1->esq.x2 = x1;
   barra1->esq.y1 = barra1->esq.y2 = y1;
   }
      //calcula dimensão da Barra2
      //dx = barra2->esq.x2 - barra2->esq.x1;
      //dy = barra2->esq.y2 - barra2->esq.y1;
dx = 0;
dy = 0;
      //redefine coord.(x1,y1) da Barra2
      barra2->esq.x1 = x1 + comp - dx/2;
      barra2->esq.y1 = y1 - dy/2;
      //redefine coord.(x2,y2) da Barra2
      barra2->esq.x2 = x1 + comp + dx/2;
      barra2->esq.y2 = y1 + dy/2;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::OrdenaRetaVertBottomTop(void)
   {
   //variáveis locais
   int       dx, dy, x1, y1, x2, y2;
   double    comp;
   VTBarra   *barra1, *barra2;
   VTLigacao *ligacao;

   //loop p/ todas Ligacoes da Secao
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      comp    = ligacao->esq.comprimento;
      barra2  = ligacao->pbarra2;
      //determina cooodenadas da Ligacao
      ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
//se primeira Ligacao, ajusta coordenadas da Barra 1
if (n == 0)
   {//ajusta coordenadas da Barra inicial
   barra1  = ligacao->pbarra1;
   barra1->esq.x1 = barra1->esq.x2 = x1;
   barra1->esq.y1 = barra1->esq.y2 = y1;
   }
      //calcula dimensão da Barra2
      //dx = barra2->esq.x2 - barra2->esq.x1;
      //dy = barra2->esq.y2 - barra2->esq.y1;
dx = 0;
dy = 0;
      //redefine coord.(x1,y1) da Barra2
      barra2->esq.x1 = x1 - dx/2;
      barra2->esq.y1 = y1 - comp + dy/2;
      //redefine coord.(x2,y2) da Barra2
      barra2->esq.x2 = x1 + dx/2;
      barra2->esq.y2 = y1 - comp - dy/2;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::OrdenaRetaVertTopBottom(void)
   {
   //variáveis locais
   int       dx, dy, x1, y1, x2, y2;
   double    comp;
   VTBarra   *barra1, *barra2;
   VTLigacao *ligacao;

   //loop p/ todas Ligacoes da Secao
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      comp    = ligacao->esq.comprimento;
      barra2  = ligacao->pbarra2;
      //determina cooodenadas da Ligacao
      ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
//se primeira Ligacao, ajusta coordenadas da Barra 1
if (n == 0)
   {//ajusta coordenadas da Barra inicial
   barra1  = ligacao->pbarra1;
   barra1->esq.x1 = barra1->esq.x2 = x1;
   barra1->esq.y1 = barra1->esq.y2 = y1;
   }
      //calcula dimensão da Barra2
      //dx = barra2->esq.x2 - barra2->esq.x1;
      //dy = barra2->esq.y2 - barra2->esq.y1;
dx = 0;
dy = 0;
      //redefine coord.(x1,y1) da Barra2
      barra2->esq.x1 = x1 - dx/2;
      barra2->esq.y1 = y1 + comp - dy/2;
      //redefine coord.(x2,y2) da Barra2
      barra2->esq.x2 = x1 + dx/2;
      barra2->esq.y2 = y1 + comp + dy/2;
      }
   }

//---------------------------------------------------------------------------
VTSecao* __fastcall TSecao::PM_GetSecaoPai(void)
   {
   return(PD.pai);
   }
//---------------------------------------------------------------------------
int __fastcall TSecao::PM_GetSentido(void)
   {
   return(PD.sentido);
   }

//---------------------------------------------------------------------------
int __fastcall TSecao::PM_GetTipo(void)
   {
   return(PD.tipo);
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::PM_SetSecaoPai(VTSecao *pai)
   {
   //salva Secao pai
   PD.pai = pai;
   }

//---------------------------------------------------------------------------
void __fastcall TSecao::PM_SetTipo(int tipo)
   {
   PD.tipo = tipo;
   }

//---------------------------------------------------------------------------
//eof

