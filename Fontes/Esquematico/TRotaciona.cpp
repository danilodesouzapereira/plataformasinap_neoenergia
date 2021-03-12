//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRotaciona.h"
#include "Esquematico.h"
#include "VTEsqRadial.h"
#include "VTSecao.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
//#include "..\Rede\VTCluster.h"
//#include "..\Rede\VTCorte.h"
#include "..\Rede\VTLigacao.h"
//#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
//#include "..\Rede\VTReducao.h"
//#include "..\DLL_Stub\Consulta\Consulta.h"
//#include "..\DLL_Stub\Funcao\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRotaciona
//---------------------------------------------------------------------------
VTRotaciona* __fastcall NewObjRotaciona(VTApl *apl)
   {
   return(new TRotaciona(apl));
   }

//---------------------------------------------------------------------------
__fastcall TRotaciona::TRotaciona(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl  = apl;
   //cria objetos
   esq_radial = NewObjEsqRadial(apl);
   }

//---------------------------------------------------------------------------
__fastcall TRotaciona::~TRotaciona(void)
   {
   //destrói objetos
   if (esq_radial) {delete esq_radial; esq_radial = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TRotaciona::Executa(VTRede *rede, int align)
   {
   switch(align)
      {
      case esqNORMAL:     MontaEsqNormal(rede); break;
      case HoriLeftRight: MontaHoriLeftRight(rede); break;
      case HoriRightLeft: MontaHoriRightLeft(rede); break;
      case VertBottomTop: MontaVertBottomTop(rede); break;
      case VertTopBottom: MontaVertTopBottom(rede); break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRotaciona::MontaEsqNormal(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisLIG;

   //redefine coordenadas de todas Ligacoes iguais às coordenadas esquemático
   lisLIG = rede->LisLigacao();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         ligacao->esq_bar[ind_bar] = barra->esq_orig;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRotaciona::MontaHoriLeftRight(VTRede *rede)
   {
   //variáveis locais
   int          dx, dy;
   strESQ_BARRA esq_bar_orig, esq_bar_novo;
   VTArea       *area;
   VTBarra      *bar_ini;
   VTLigacao    *ligacao;
   VTSecao      *secao;
   TList        *lisLIG;
   VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //monta Secoes da Rede, sem alterar coordenadas esquemático
   esq_radial->Executa(rede, false);
   //determina Secao inicial da Rede
   if ((secao = esq_radial->SecaoInicial()) == NULL) return;
   //determina Ligacao inicial da Rede (última Ligacao da primeira Secao)
   if ((ligacao = secao->LigacaoInicial()) == NULL) return;
   //salva coordenada original do início do esquematico
   esq_bar_orig = ligacao->esq_bar[0];
   //determina a nova coordenada do início do esquematico
   area = redes->AreaEsquematico();
   esq_bar_novo.x1 = esq_bar_novo.x2 = area->x1 + (0.05 * area->Width());
   esq_bar_novo.y1 = esq_bar_novo.y2 = (area->y1 + area->y2)/2 ;
   //redefine coordenadas de todas Ligacoes em função do sentido da Secao inicial
   lisLIG = rede->LisLigacao();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //recalcula coordenadas esquemático da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if (ligacao->Barra(ind_bar) == NULL) continue;
         //determina distância em relação à origem anterior
         dx = ligacao->esq_bar[ind_bar].x1 - esq_bar_orig.x1;
         dy = ligacao->esq_bar[ind_bar].y1 - esq_bar_orig.y1;
         //mantém a mesma distância em relação à nova origem
         switch(secao->Sentido)
            {
            case HoriLeftRight:
            case HoriRightLeft:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + abs(dx);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + dy;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + abs(dx);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + dy;
               break;
            case VertBottomTop:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 - dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 - dx;
               break;
            case VertTopBottom:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + dx;
               break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRotaciona::MontaHoriRightLeft(VTRede *rede)
   {
   //variáveis locais
   int          dx, dy;
   strESQ_BARRA esq_bar_orig, esq_bar_novo;
   VTArea       *area;
   VTBarra      *bar_ini;
   VTLigacao    *ligacao;
   VTSecao      *secao;
   TList        *lisLIG;
   VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //monta Secoes da Rede, sem alterar coordenadas esquemático
   esq_radial->Executa(rede, false);
   //determina Secao inicial da Rede
   if ((secao = esq_radial->SecaoInicial()) == NULL) return;
   //determina Ligacao inicial da Rede (última Ligacao da primeira Secao)
   if ((ligacao = secao->LigacaoInicial()) == NULL) return;
   //salva coordenada original do início do esquematico
   esq_bar_orig = ligacao->esq_bar[0];
   //determina a nova coordenada do início do esquematico
   area = redes->AreaEsquematico();
   esq_bar_novo.x1 = esq_bar_novo.x2 = area->x2 - (0.05 * area->Width());
   esq_bar_novo.y1 = esq_bar_novo.y2 = (area->y1 + area->y2)/2 ;
   //redefine coordenadas de todas Ligacoes em função do sentido da Secao inicial
   lisLIG = rede->LisLigacao();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //recalcula coordenadas esquemático da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if (ligacao->Barra(ind_bar) == NULL) continue;
         //determina distância em relação à origem anterior
         dx = ligacao->esq_bar[ind_bar].x1 - esq_bar_orig.x1;
         dy = ligacao->esq_bar[ind_bar].y1 - esq_bar_orig.y1;
         //mantém a mesma distância em relação à nova origem
         switch(secao->Sentido)
            {
            case HoriLeftRight:
            case HoriRightLeft:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 - abs(dx);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + dy;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 - abs(dx);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + dy;
               break;
            case VertBottomTop:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 - abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 - abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + dx;
               break;
            case VertTopBottom:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 - abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 - abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + dx;
               break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRotaciona::MontaVertBottomTop(VTRede *rede)
   {
   //variáveis locais
   int          dx, dy;
   strESQ_BARRA esq_bar_orig, esq_bar_novo;
   VTArea       *area;
   VTBarra      *bar_ini;
   VTLigacao    *ligacao;
   VTSecao      *secao;
   TList        *lisLIG;
   VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //monta Secoes da Rede, sem alterar coordenadas esquemático
   esq_radial->Executa(rede, false);
   //determina Secao inicial da Rede
   if ((secao = esq_radial->SecaoInicial()) == NULL) return;
   //determina Ligacao inicial da Rede (última Ligacao da primeira Secao)
   if ((ligacao = secao->LigacaoInicial()) == NULL) return;
   //salva coordenada original do início do esquematico
   esq_bar_orig = ligacao->esq_bar[0];
   //determina a nova coordenada do início do esquematico
   area = redes->AreaEsquematico();
   esq_bar_novo.x1 = esq_bar_novo.x2 = (area->x1 + area->x2)/2 ;
   esq_bar_novo.y1 = esq_bar_novo.y2 = area->y2 - (0.05 * area->Height());
   //redefine coordenadas de todas Ligacoes em função do sentido da Secao inicial
   lisLIG = rede->LisLigacao();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //recalcula coordenadas esquemático da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if (ligacao->Barra(ind_bar) == NULL) continue;
         //determina distância em relação à origem anterior
         dx = ligacao->esq_bar[ind_bar].x1 - esq_bar_orig.x1;
         dy = ligacao->esq_bar[ind_bar].y1 - esq_bar_orig.y1;
         //mantém a mesma distância em relação à nova origem
         switch(secao->Sentido)
            {
            case HoriLeftRight:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 - abs(dx);//dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 - abs(dx);//dx;
               break;
            case HoriRightLeft:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 - abs(dx);//dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 - abs(dx);//dx;
               break;
            case VertBottomTop:
            case VertTopBottom:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + dx;
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 - abs(dy);
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + dx;
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 - abs(dy);
               break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRotaciona::MontaVertTopBottom(VTRede *rede)
   {
   //variáveis locais
   int          dx, dy;
   strESQ_BARRA esq_bar_orig, esq_bar_novo;
   VTArea       *area;
   VTBarra      *bar_ini;
   VTLigacao    *ligacao;
   VTSecao      *secao;
   TList        *lisLIG;
   VTRedes      *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //monta Secoes da Rede, sem alterar coordenadas esquemático
   esq_radial->Executa(rede, false);
   //determina Secao inicial da Rede
   if ((secao = esq_radial->SecaoInicial()) == NULL) return;
   //determina Ligacao inicial da Rede (última Ligacao da primeira Secao)
   if ((ligacao = secao->LigacaoInicial()) == NULL) return;
   //salva coordenada original do início do esquematico
   esq_bar_orig = ligacao->esq_bar[0];
   //determina a nova coordenada do início do esquematico
   area = redes->AreaEsquematico();
   esq_bar_novo.x1 = esq_bar_novo.x2 = (area->x1 + area->x2)/2 ;
   esq_bar_novo.y1 = esq_bar_novo.y2 = area->y1 + (0.05 * area->Height());
   //redefine coordenadas de todas Ligacoes em função do sentido da Secao inicial
   lisLIG = rede->LisLigacao();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //recalcula coordenadas esquemático da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if (ligacao->Barra(ind_bar) == NULL) continue;
         //determina distância em relação à origem anterior
         dx = ligacao->esq_bar[ind_bar].x1 - esq_bar_orig.x1;
         dy = ligacao->esq_bar[ind_bar].y1 - esq_bar_orig.y1;
         //mantém a mesma distância em relação à nova origem
         switch(secao->Sentido)
            {
            case HoriLeftRight:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + abs(dx);//dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + abs(dx);//dx;
               break;
            case HoriRightLeft:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + abs(dx);//dx;
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + dy;     //abs(dy);
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + abs(dx);//dx;
               break;
            case VertBottomTop:
            case VertTopBottom:
               ligacao->esq_bar[ind_bar].x1 = esq_bar_novo.x1 + dx;
               ligacao->esq_bar[ind_bar].y1 = esq_bar_novo.y1 + abs(dy);
               ligacao->esq_bar[ind_bar].x2 = esq_bar_novo.x2 + dx;
               ligacao->esq_bar[ind_bar].y2 = esq_bar_novo.y2 + abs(dy);
               break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof

