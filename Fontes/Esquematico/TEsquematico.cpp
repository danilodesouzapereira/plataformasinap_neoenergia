//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TEsquematico.h"
#include "Esquematico.h"
#include "VTEsqMalha.h"
#include "VTEsqRadial.h"
#include "VTEsqViz.h"
#include "VTRotaciona.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsquematico
//---------------------------------------------------------------------------
VTEsquematico* __fastcall NewObjEsquematico(VTApl *apl)
   {
   return(new TEsquematico(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEsquematico::TEsquematico(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl  = apl;
   //cria objetos
   esq_malha  = NewObjEsqMalha(apl);
   esq_radial = NewObjEsqRadial(apl);
   esq_viz    = NewObjEsqViz(apl);
   rotaciona  = NewObjRotaciona(apl);
   //cria listas
   lisORD = new TList();
   lisTMP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEsquematico::~TEsquematico(void)
   {
   //destrói objetos
   if (esq_malha)  {delete esq_malha;  esq_malha  = NULL;}
   if (esq_radial) {delete esq_radial; esq_radial = NULL;}
   if (esq_viz)    {delete esq_viz;    esq_viz    = NULL;}
   if (rotaciona)  {delete rotaciona;  rotaciona  = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisORD) {delete lisORD; lisORD = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
	}
	
//---------------------------------------------------------------------------
bool __fastcall TEsquematico::AjustaCoordBarraSemCoordUtm(VTRedes *redes)
	{
	//variáveis locais
	int     num_bar = 0;
	struct {int x1, y1, x2, y2;} area_utm, area_esq;
	double  fat_x, fat_y;
	VTBarra *barra;
	TList   *lisBAR   = redes->LisBarra();

	//determina área utm da rede, considerando apenas Barras c/ coordenadas válidas
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		//verifica se a Barra possui coordenadas utm
		if ((barra->utm.x == 0)&&(barra->utm.y == 0)) continue;
		if (num_bar == 0)
			{//primeira Barra c/ coordenada utm válida
			area_utm.x1 = area_utm.x2 = barra->utm.x ;
			area_utm.y1 = area_utm.y2 = barra->utm.y;
			//
			area_esq.x1 = barra->esq.x1;
			area_esq.y1 = barra->esq.y1;
			area_esq.x2 = barra->esq.x2;
			area_esq.y2 = barra->esq.y2;
			}
		else
			{
			area_utm.x1 = Min(area_utm.x1, barra->utm.x);
			area_utm.y1 = Min(area_utm.y1, barra->utm.y);
			area_utm.x2 = Max(area_utm.x2, barra->utm.x);
			area_utm.y2 = Max(area_utm.y2, barra->utm.y);
			//
			area_esq.x1 = Min(area_esq.x1, barra->esq.x1);
			area_esq.y1 = Min(area_esq.y1, barra->esq.y1);
			area_esq.x2 = Max(area_esq.x2, barra->esq.x2);
			area_esq.y2 = Max(area_esq.y2, barra->esq.y2);
			}
		num_bar++;
		}
	//proteção: verifica se conseguiu calcular a área utm da rede
	if ((num_bar <= 1)||((area_esq.x1 == area_esq.x2)&&(area_esq.y1 == area_esq.y2))) return(false);
	//calcula fator de escala entre áreas
	fat_x = double(area_utm.x2 - area_utm.x1) / double(area_esq.x2 - area_esq.x1);
	fat_y = double(area_utm.y2 - area_utm.y1) / double(area_esq.y2 - area_esq.y1);
	//assume um fator de escala único
	//if (fat_x < fat_y) fat_x = fat_y;
	//if (fat_y < fat_x) fat_y = fat_x;
	//proteção
	if (fat_x == 0) fat_x = 1;
	if (fat_y == 0) fat_y = 1;
	//calcula coordenadas utm p/ as Barras em que as coordenadas são nulas
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->utm.x == 0)&&(barra->utm.y == 0))
			{
			barra->utm.x = area_utm.x1 + ((((barra->esq.x1 + barra->esq.x2) / 2) - area_esq.x1) * fat_x);
			barra->utm.y = area_utm.y1 + ((((barra->esq.y1 + barra->esq.y2) / 2) - area_esq.y1) * fat_y);
			}
		}
	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TEsquematico::Align(VTRede *rede, int align)
	{
	//redefine coordenadas esquemático da Rede
   rotaciona->Executa(rede, align);
   //monta esquemático da Rede
   MontaEsquematicoParcial(rede);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEsquematico::BarraComum(VTRede *rede, VTCorte *corte)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR;

   //procura Barra do Corte que pertence a Rede
   lisBAR = corte->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (rede->ExisteBarra(barra)) return(barra);
      }
   return(NULL);
   }
   
//---------------------------------------------------------------------------
void __fastcall TEsquematico::CriaEsqRedesVizinhas(VTRede *rede)
   {
   esq_viz->Executa(rede);
   }

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteBarraSemCoordEsq(VTRede *rede)
	{
	//variáveis locais
	VTBarra *barra;
	TList   *lisBAR  = rede->LisBarra();

	//verifica se toda Barra possui coord.utm definida
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->esq.x1 == 0)&&(barra->esq.y1 == 0)&&
          (barra->esq.x2 == 0)&&(barra->esq.y2 == 0)) return(true);
		}
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteBarraSemCoordUtm(VTRede *rede)
	{
	//variáveis locais
	VTBarra *barra;
	TList   *lisBAR  = rede->LisBarra();

	//verifica se toda Barra possui coord.utm definida
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->utm.x == 0)&&(barra->utm.y == 0)) return(true);
		}
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteBarraSemCoordEsq(VTRedes *redes)
	{
	//variáveis locais
	VTBarra *barra;
	TList   *lisBAR  = redes->LisBarra();

	//verifica se toda Barra possui coord.esq definida
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->esq.x1 == 0)&&(barra->esq.y1 == 0)&&
          (barra->esq.x2 == 0)&&(barra->esq.y2 == 0)) return(true);
		}
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteBarraSemCoordUtm(VTRedes *redes)
	{
	//variáveis locais
	VTBarra *barra;
	TList   *lisBAR  = redes->LisBarra();

	//verifica se toda Barra possui coord.utm definida
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->utm.x == 0)&&(barra->utm.y == 0)) return(true);
		}
	return(false);
	}

//---------------------------------------------------------------------------
VTCorte* __fastcall TEsquematico::ExisteCorte(TList *lisCORTE, VTBarra *barra)
   {
   //variáveis locais
   VTCorte   *corte;

   //verifica se a Barra pertence a algum Corte da Rede
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      if (corte->ExisteBarra(barra)) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteEsquematicoGeral(VTRedes *redes)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR  = redes->LisBarra();

   //verifica se todas Barras já tem coordenadas do esquemático definidas
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if ((barra->esq.x1 <= 0)||(barra->esq.y1 <= 0)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteEsquematicoParcial(VTRede *rede)
   {
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisLIG = rede->LisLigacao();

   //verifica se todas Ligacoes já tem coordenadas de esquemático definidas
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //loop p/ todas Barras da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if (ligacao->Barra(ind_bar) == NULL) continue;
         if ((ligacao->esq_bar[ind_bar].x1 <= 0)||(ligacao->esq_bar[ind_bar].y1 <= 0)) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TEsquematico::ExisteLigacao(TList *lisLIG, VTBarra *barra)
   {
	//variáveis locais
	int       x1, y1, x2, y2;
   VTLigacao *ligacao;

   //procura Ligacao da Rede que conecta a Barra
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (ligacao->ConectaBarra(barra)) return(ligacao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::ExisteRedeVizinha(VTRede *rede, VTBarra *barra, TList *lisVIZ)
   {
   //variáveis locais
   VTRede    *rede_viz;
   TList     *lisREDE;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //limpa lisVIZ
   lisVIZ->Clear();
   //verifica se a Barra pertence a algum Corte da Rede
   if (ExisteCorte(rede->LisCorte(), barra) == NULL) return(false);
   //verifica se existe alguma Rede vizinha carregada que conecta o Corte
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede_viz = (VTRede*)lisREDE->Items[n];
      if (rede_viz == rede) continue;
      //verifica se a Rede vizinha está carregada
      if (! rede_viz->Carregada) continue;
      //verifica se a Rede vizinha conecta a Barra
      if (rede_viz->ExisteBarra(barra)) lisVIZ->Add(rede_viz);
      }
   return(lisVIZ->Count > 0);
   }

//---------------------------------------------------------------------------
bool  __fastcall TEsquematico::ExisteReducao(VTRedes *redes)
   {
   //variáveis locais
   VTMRede *mrede;
   TList   *lisMREDE;

   //verifica se existe alguma Reducao
   lisMREDE = redes->LisMRede();
   for (int n = 0; n < lisMREDE->Count; n++)
      {
      mrede = (VTMRede*)lisMREDE->Items[n];
      if (! mrede->Carregada) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool  __fastcall TEsquematico::ExisteReducaoVizinha(VTRede *rede, VTBarra *barra, TList *lisVIZ)
   {
   //variáveis locais
   VTCorte   *corte;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisMR;
   TList     *lisREDU;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //limpa lisVIZ
   lisVIZ->Clear();
   //verifica se a Barra pertence a algum Corte da Rede
   if ((corte = ExisteCorte(rede->LisCorte(), barra)) == NULL) return(false);
   //verifica se existe alguma Reducao p/ o Corte
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede está carregada
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //verifica se  Reducao conecta o Corte da Barra
         if (reducao->Corte == corte) lisVIZ->Add(reducao);
         }
      }
   return(lisVIZ->Count > 0);
   }

//---------------------------------------------------------------------------
int __fastcall TEsquematico::IndexOf(TList *lisREDE, VTRede *rede)
   {
   //variáveis locais
   VTRede *rede_ord;

   //proteção
   if (lisREDE->Count == 0) return(0);
   //determina índice do valor em
   for (int n = lisREDE->Count-1; n >= 0; n--)
      {
      rede_ord = (VTRede*)lisREDE->Items[n];
      if (rede->NumeroDeCortes() <= rede_ord->NumeroDeCortes()) return(n+1);
      }
   return(0);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::IniciaEsquematico(VTRedes *redes)
   {
   //variáveis locais
   TList  *lisREDE;
   VTRede *rede;

   //verifica se as Barras já tem coordenadas p/ esquemático geral
   if (! ExisteEsquematicoGeral(redes))
      {//inicia coordenadas esquemático geral de cada Barra iguais às suas coord.utm
      IniciaEsquematicoGeral(redes);
      }
   //salva coordenadas do esquemático geral
   //SalvaEsquematicoGeral(redes);
   //se necessário, inicia coordenadas p/ esquemático parcial de cada Rede
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se a Rede está carregada
      if (! rede->Carregada) continue;
      if (! ExisteEsquematicoParcial(rede))
         {//monta esquemático parcial da rede
         IniciaEsquematicoParcial(rede);
         //salva coordenadas esquemático parcial
         SalvaEsquematicoParcial(rede);
         //alinha esquemático da direita p/ esquerda
         rotaciona->Executa(rede, HoriLeftRight);
         //restaura coordenadas esquemático geral
         MontaEsquematicoGeral(redes);
         }
      }
	}

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::IniciaEsquematicoIgualGeo(VTRedes *redes)
   {
   //variáveis locais
	VTBarra   *barra;
	VTReducao *reducao;
   TList     *lisBAR;

   //proteção: verifica se há Reduções
   //if (ExisteReducao(redes)) return(false);
	//proteção: verifica se Barra sem coord.utm
   if (ExisteBarraSemCoordUtm(redes)) return(false);
   //atualiza coordenadas esquemático de todas Barras
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
		barra->esq.x1 = barra->esq.x2 = barra->utm.x;
		barra->esq.y1 = barra->esq.y2 = barra->utm.y;
		//salva coord.esq originais
		barra->esq_orig = barra->esq;
		}
	//atualiza coordenadas das Barras fictícias das Reduçoes
	lisTMP->Clear();
	redes->LisReducaoCarregada(lisTMP);
	for (int nr = 0; nr < lisTMP->Count; nr++)
		{
		reducao = (VTReducao*)lisTMP->Items[nr];
		barra   = reducao->BarraFicticia();
		barra->esq.x1 = barra->esq.x2 = barra->utm.x;
		barra->esq.y1 = barra->esq.y2 = barra->utm.y;
		//salva coord.esq originais
		barra->esq_orig = barra->esq;
		}
   //recalcula áreas das redes
   redes->CalculaArea();
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::IniciaEsquematicoGeral(VTRedes *redes)
   {
   //variáveis locais
   strUTM     coord_utm;
   VTBarra   *barra;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisBAR;
   TList     *lisMR;
   TList     *lisREDU;

   //reinicia coordenadas p/ esquemático de cada Barra iguais às suas coord.utm
   lisBAR  = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //obtém coordenadas utm em cm das Barras inicial e final da Secao
      barra->CoordenadasUtm_cm(coord_utm.x, coord_utm.y);
      //define coordenadas p/ diag.esquemático da Barra
      barra->esq.x1 = barra->esq.x2 = coord_utm.x;
      barra->esq.y1 = barra->esq.y2 = coord_utm.y;
      }
   //reinicia coordenadas p/ esquemático das Barras fictícias das redes reduzidas
   lisMR   = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede está carregada
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém Barra fictícia da reducao
         barra = reducao->BarraFicticia();
         //obtém coordenadas utm em cm da Barra ficticia
         barra->CoordenadasUtm_cm(coord_utm.x, coord_utm.y);
         //define coordenadas p/ diag.esquemático da Barra
         barra->esq.x1 = barra->esq.x2 = coord_utm.x;
         barra->esq.y1 = barra->esq.y2 = coord_utm.y;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TEsquematico::IniciaEsquematicoParcial(VTRede *rede)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //salva comprimento das Ligacoes no diag.esquemático
   SalvaComprimentoLigacao(rede);
   //tenta montagem supondo Rede radial
   if (esq_radial->Executa(rede))
      {//recalcula áreas
      redes->CalculaArea();
      return(true);
      }
   //tenta montagem supondo existência de malha
   if (esq_malha->Executa(rede))
      {//recalcula áreas
      redes->CalculaArea();
      return(true);
      }
   //erro
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::InverteCoordX(VTRedes *redes)
   {
   //variáveis locais
   VTArea    *area;
   VTBarra   *barra;
   VTEqbar   *eqbar;
   VTTrecho  *trecho;
   TList     *lisBAR;

   //obtém área de Redes
   area = redes->AreaEsquematico();
   //inverte  coordenada X de todas Barras
   lisBAR = redes->LisBarra();
   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      barra->esq.x1 = area->x1 + (area->x2 - barra->esq.x1);
      barra->esq.x2 = area->x1 + (area->x2 - barra->esq.x2);
      }
   //inverte  deslocamento DX de todos Eqbar
   lisTMP->Clear();
   redes->LisEqbar(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqbar = (VTEqbar*)lisTMP->Items[n];
      eqbar->esq.dxbar = - eqbar->esq.dxbar;
      }
   lisTMP->Clear();
   //recalcula áreas das redes
   redes->CalculaArea();
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::InverteCoordY(VTRedes *redes)
   {
   //variáveis locais
   VTArea    *area;
   VTBarra   *barra;
   VTEqbar   *eqbar;
   VTTrecho  *trecho;
   TList     *lisBAR;

   //obtém área de Redes
   area = redes->AreaEsquematico();
   //inverte coordenada Y de todas Barras
   lisBAR = redes->LisBarra();
   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      barra->esq.y1 = area->y1 + (area->y2 - barra->esq.y1);
      barra->esq.y2 = area->y1 + (area->y2 - barra->esq.y2);
      }
   //inverte  deslocamento DY de todos Eqbar
   lisTMP->Clear();
   redes->LisEqbar(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      eqbar = (VTEqbar*)lisTMP->Items[n];
      eqbar->esq.dybar = - eqbar->esq.dybar;
      }
   lisTMP->Clear();
   //recalcula áreas das redes
   redes->CalculaArea();
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::MontaEsquematicoGeral(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisBAR;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //atualiza coordenadas esquemático de todas Barras da Rede
   lisBAR = rede->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->esq = barra->esq_orig;
      }
   //cria esquemático das Redes vizinhas carregadas
   CriaEsqRedesVizinhas(rede);
   //atualiza áreas
   redes->CalculaArea();
   //atualiza coordenadas esquemático da Barra fictícia de todas Reducoes
   lisBAR = rede->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //verifica se existem Reducoes conectadas à Barra
      if (! ExisteReducaoVizinha(rede, barra, lisTMP)) continue;
      //determina uma Ligacao da Rede que conecta a Barra
      if ((ligacao = ExisteLigacao(rede->LisLigacao(), barra)) == NULL) continue;
      MontaEsquematicoReducao(ligacao, barra, lisTMP);
      }
   //atualiza coordenadas esquemático da Barra fictícia de todas Redes vizinhas carregdas
   lisBAR = rede->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //verifica há Redes vizinhas carregadas conectadas à Barra
      if (! ExisteRedeVizinha(rede, barra, lisTMP)) continue;
      //determina uma Ligacao da Rede que conecta a Barra
      if ((ligacao = ExisteLigacao(rede->LisLigacao(), barra)) == NULL) continue;
      MontaEsquematicoRedeVizinha(ligacao, barra, lisTMP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::MontaEsquematicoGeral(VTRedes *redes)
   {
   //variáveis locais
   VTBarra   *barra;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisBAR;
   TList     *lisMR;
   TList     *lisREDU;

   //atualiza coordenadas esquemático de todas as Barras
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->esq = barra->esq_orig;
      }
   //reinicia coordenadas p/ esquemático das Barras fictícias das redes reduzidas
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede está carregada
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém Barra fictícia da reducao
         barra = reducao->BarraFicticia();
         barra->esq = barra->esq_orig;
         }
      }
   //atualiza áreas
   redes->CalculaArea();
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::MontaEsquematicoParcial(VTCluster *cluster)
   {
   //variáveis locais
   int       dx, dy;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisBAR;
   TList     *lisLIG;
   TList     *lisMR;
   TList     *lisREDE;
   TList     *lisREDU;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //atualiza coordenadas esquemático de todas Barras da Rede a partir
   //das coordenadas salvas nas Ligacoes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      lisLIG = rede->LisLigacao();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //atualiza coordenadas de esquemático das Barras da Ligacao
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            barra->esq = ligacao->esq_bar[ind_bar];
            }
         }
      }
   //atualiza áreas
   redes->CalculaArea();
   //atualiza coordenadas esquemático de todas Barras fictícias das Reducoes
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede está carregada
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da MRede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //calcula somatória do deslocamento das Barras de Corte
         lisBAR = reducao->Corte->LisBarra();
         barra = (VTBarra*)lisBAR->First();
         dx = barra->esq.x1 - barra->esq_orig.x1;
         dy = barra->esq.y1 - barra->esq_orig.y1;
         for (int nb = 1; nb < lisBAR->Count; nb++)
            {
            barra = (VTBarra*)lisBAR->Items[nb];
            dx += barra->esq.x1 - barra->esq_orig.x1;
            dy += barra->esq.y1 - barra->esq_orig.y1;
            }
         //obtém Barra fictícia da reducao
         barra = reducao->BarraFicticia();
         //define coordenadas como ponto médio das mesmo deslocamento na Barra fictícia
         barra->esq.x1 = barra->esq.x2 = barra->esq_orig.x1 + (dx/lisBAR->Count);
         barra->esq.y1 = barra->esq.y2 = barra->esq_orig.y1 + (dy/lisBAR->Count);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::MontaEsquematicoParcial(VTRede *rede)
   {
   //variáveis locais
   int       dx, dy;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisBAR;
   TList     *lisLIG;
   TList     *lisMR;
   TList     *lisREDU;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   
   //atualiza coordenadas esquemático de todas Barras da Rede a partir
   //das coordenadas salvas nas Ligacoes
   lisLIG = rede->LisLigacao();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //atualiza coordenadas de esquemático das Barras da Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         barra->esq = ligacao->esq_bar[ind_bar];
         }
      }
   //cria esquemático das Redes vizinhas carregadas
   CriaEsqRedesVizinhas(rede);
   //atualiza áreas
   redes->CalculaArea();
   //atualiza coordenadas esquemático da Barra fictícia de todas Reducoes
   lisBAR = rede->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //verifica se existem Reducoes conectadas à Barra
      if (! ExisteReducaoVizinha(rede, barra, lisTMP)) continue;
      //determina uma Ligacao da Rede que conecta a Barra
      if ((ligacao = ExisteLigacao(rede->LisLigacao(), barra)) == NULL) continue;
      MontaEsquematicoReducao(ligacao, barra, lisTMP);
      }
   //atualiza coordenadas esquemático da Barra fictícia de todas Redes vizinhas carregdas
   lisBAR = rede->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //verifica há Redes vizinhas carregadas conectadas à Barra
      if (! ExisteRedeVizinha(rede, barra, lisTMP)) continue;
      //determina uma Ligacao da Rede que conecta a Barra
      if ((ligacao = ExisteLigacao(rede->LisLigacao(), barra)) == NULL) continue;
      MontaEsquematicoRedeVizinha(ligacao, barra, lisTMP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::MontaEsquematicoRedeVizinha(VTLigacao *ligacao, VTBarra *barra, TList *lisREDE)
   {
   //variáveis locais
   int       comp;
   VTBarra   *bar_fic;
   VTLigacao *liga_fic;
   VTRede    *rede;

   //loop p/ todas Redes vizinhas em lisREDE
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //obtém Barra fictícia da Rede
      bar_fic = rede->BarraFicticia();
      //obtém primeira Ligacao do esquemático  da Rede que conecta a Barra indicada
      liga_fic = ExisteLigacao(rede->LisLigacaoEsquematico(), barra);
      if (liga_fic == NULL) continue;
      comp     = liga_fic->ComprimentoEsq();
      //redefine coordenada esquemático da Barra fictícia p/ que fique perpendicula à Ligacao indicada
      if (ligacao->Vertical())
         {//esquemático da Rede deve ser horizontal
         if ((nr % 2) == 0)
            {//posiciona à esquerda
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1 - comp;
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1;
            }
         else
            {//posiciona à direita
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1 + comp;
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1;
            }
         }
      else
         {//esquemático da Reducao deve ser vertical
         if ((nr % 2) == 0)
            {//posiciona acima
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1;
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1 - comp;
            }
         else
            {//posiciona abaixo
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1;
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1 + comp;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::MontaEsquematicoReducao(VTLigacao *ligacao, VTBarra *barra, TList *lisREDU)
   {
   //variáveis locais
   int       dx, dy;
   VTBarra   *bar_fic, *barra2;
   VTLigacao *liga_fic;
   VTReducao *reducao;
   TList     *lisLIG;

   //determina deslocamento da Barra indicada
   dx = barra->esq.x1 - barra->esq_orig.x1;
   dy = barra->esq.y1 - barra->esq_orig.y1;
   //loop p/ todas Reducoes em lisREDU
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //obtém Barra fictícia da reducao
      bar_fic = reducao->BarraFicticia();
      //aplica mesmo deslocamento na Barra fictícia
      bar_fic->esq.x1 = bar_fic->esq.x2 = bar_fic->esq_orig.x1 + dx;
      bar_fic->esq.y1 = bar_fic->esq.y2 = bar_fic->esq_orig.y1 + dy;
      //obtém Ligacao fictícia da Reducao que conecta a Barra indicada (e a Barra fictícia)
      liga_fic = ExisteLigacao(reducao->LisLigacaoEsquematico(), barra);
      //redefine coordenada esquemático da Barra fictícia p/ que fique perpendicula à Ligacao indicad
      if (ligacao->Vertical())
         {//esquemático da Reducao deve ser horizontal
         if (reducao->ExisteGeracao())
            {//posiciona à esquerda
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1 - liga_fic->ComprimentoEsq();
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1;
            }
         else
            {//posiciona à esquerda
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1 + liga_fic->ComprimentoEsq();
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1;
            }
         }
      else
         {//esquemático da Reducao deve ser vertical
         if (reducao->ExisteGeracao())
            {//posiciona acima
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1;
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1 - liga_fic->ComprimentoEsq();
            }
         else
            {//posiciona abaixo
            bar_fic->esq.x1 = bar_fic->esq.x2 = barra->esq.x1;
            bar_fic->esq.y1 = bar_fic->esq.y2 = barra->esq.y1 + liga_fic->ComprimentoEsq();
            }
         }
      }
   //ajusta coordenadas esquemático das Barras das Reducoes
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //obtém Barra fictícia da reducao
      bar_fic = reducao->BarraFicticia();
      //determina deslocamento da Barra fictícia
      dx = bar_fic->esq.x1 - bar_fic->esq_orig.x1;
      dy = bar_fic->esq.y1 - bar_fic->esq_orig.y1;
      //ajusta cordenadas esquemático das demais Barras da Reducao
      lisLIG = reducao->LisLigacaoEsquematico();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         barra2 = (ligacao->pbarra1 == bar_fic) ? ligacao->pbarra2 : ligacao->pbarra1;
         if (barra2 == barra) continue;
         //aplica mesmo deslocamento na Barra
         barra2->esq.x1 = barra2->esq.x2 = barra2->esq_orig.x1 + dx;
         barra2->esq.y1 = barra2->esq.y2 = barra2->esq_orig.y1 + dy;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::OrdenaRedes(VTRedes *redes, TList *lisORD)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisREDE = redes->LisRede();

   //reinicia lisORD
   lisORD->Clear();
   //loop p/ toda Rede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se a Rede está carregada
      if (! rede->Carregada) continue;
      //verifica se a Rede é esq_radial
      //if (! rede->Radial()) continue;
      //insere Rede em lisORD ordenada por número decrescente de Cortes
      if (n == 0)
         {lisORD->Add(rede);}
      else
         {lisORD->Insert(IndexOf(lisORD, rede), rede);}
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TEsquematico::RotacionaBarras(VTBarra *barra_ref, double ang_grau)
   {
   //variáveis locais
   int             dx, dy;
   int             dist_x, dist_y;
   complex<double> dist_xy;
   complex<double> fatmult = polar<double>(1.0, DegToRad(ang_grau));
   VTBarra *barra;
   VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBARRA = redes->LisBarra();

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      if (barra == barra_ref) continue;
      //determina dimensão da barra
      dx = barra->esq.x2 - barra->esq.x1;
      dy = barra->esq.y2 - barra->esq.y1;
      //determina vetor entre a Barra e Barra de referência
      dist_x   = barra->esq.x1 - barra_ref->esq.x1;
      dist_y   = barra->esq.y1 - barra_ref->esq.y1;
      //rotaciona o vetor
      dist_xy = complex<double>(dist_x, dist_y) * fatmult;
      //determina as novas distâncias entre a Barra e Barra de referência
      dist_x   = dist_xy.real();
      dist_y   = dist_xy.imag();
      //redefine as coordenadas da Barra
      barra->esq.x1 = barra_ref->esq.x1 + dist_x;
      barra->esq.y1 = barra_ref->esq.y1 + dist_y;
      barra->esq.x2 = barra->esq.x1 + dx;
      barra->esq.y2 = barra->esq.y1 + dy;
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TEsquematico::RotacionaBarras(VTBarra *barra_ref, double ang_grau)
   {
   //variáveis locais
   int             dx1, dy1, dx2, dy2;
   complex<double> dxy1, dxy2;
   complex<double> fatmult = polar<double>(1.0, DegToRad(ang_grau));
   TList   *lisEQB;
   VTBarra *barra;
   VTEqbar *eqbar;
   VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBARRA = redes->LisBarra();

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      //if (barra == barra_ref) continue;
      //determina distâncias entre a Barra e Barra de referência
      dx1  = barra->esq.x1 - barra_ref->esq.x1;
      dy1  = barra->esq.y1 - barra_ref->esq.y1;
      dx2  = barra->esq.x2 - barra_ref->esq.x1;
      dy2  = barra->esq.y2 - barra_ref->esq.y1;
      //monta vetores de distância e rotaciona
      dxy1 = complex<double>(dx1, dy1) * fatmult;
      dxy2 = complex<double>(dx2, dy2) * fatmult;
      //determina as novas distâncias entre a Barra e Barra de referência
      dx1  = dxy1.real();
      dy1  = dxy1.imag();
      dx2  = dxy2.real();
      dy2  = dxy2.imag();
      //redefine as coordenadas da Barra
      barra->esq.x1 = barra_ref->esq.x1 + dx1;
      barra->esq.y1 = barra_ref->esq.y1 + dy1;
      barra->esq.x2 = barra_ref->esq.x1 + dx2;
      barra->esq.y2 = barra_ref->esq.y1 + dy2;
      //loop p/ todos Eqbar da Barra
      lisEQB = barra->LisEqbar();
      for (int n = 0; n < lisEQB->Count; n++)
         {
         eqbar = (VTEqbar*)lisEQB->Items[n];
         dxy1  = complex<double>(eqbar->esq.dxbar, eqbar->esq.dybar) * fatmult;
         eqbar->esq.dxbar = dxy1.real();
         eqbar->esq.dybar = dxy1.imag();
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::SalvaComprimentoLigacao(VTRede *rede)
   {
   /*
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisLIG = rede->LisLigacao();

   //loop p/ todas Ligacoes da Rede
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      //salva valor do comprimento
      //ligacao->esq.comprimento = ligacao->ComprimentoEsq();
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::SalvaEsquematicoGeral(VTCluster *cluster)
   {
   }
   
//---------------------------------------------------------------------------
void __fastcall TEsquematico::SalvaEsquematicoGeral(VTRede *rede)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR;

   //salva coordenadas esquemático de todas as Barras
   lisBAR = rede->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->esq_orig = barra->esq;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::SalvaEsquematicoGeral(VTRedes *redes)
   {
   //variáveis locais
   VTBarra   *barra;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisBAR;
   TList     *lisMR;
   TList     *lisREDU;

   //atualiza coordenadas esquemático de todas as Barras
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->esq_orig = barra->esq;
      }
   //salva coordenadas p/ esquemático das Barras fictícias das redes reduzidas
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede está carregada
      if (mrede->Carregada) continue;
      //obtém lista de Reducoes da rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //obtém Barra fictícia da reducao
         barra = reducao->BarraFicticia();
         barra->esq_orig = barra->esq;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::SalvaEsquematicoParcial(VTCluster *cluster)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::SalvaEsquematicoParcial(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisLIG = rede->LisLigacao();

   //salva coordenadas esquemático de todas as Barras da Rede nas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //salva coordenadas de esquemático das Barras na Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         ligacao->esq_bar[ind_bar] =  barra->esq;
         }
      }
   }

//---------------------------------------------------------------------------
//eof

