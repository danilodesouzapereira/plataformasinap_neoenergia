//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TAis.h"
#include "TPtoCarga.h"
#include "VTEnvoltoria.h"
#include "VTPoligono.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
//#include "..\..\Consulta\VTConsulta.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSubestacao.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Rede\VTArea.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTReducao.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Redegraf\VTCelula.h"
#include "..\..\Redegraf\VTCelulas.h"
#include "..\..\Redegraf\VTRedegraf.h"
#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAis* __fastcall NewObjAis(VTApl *apl, VTSubestacao *subestacao)
   {
   try{
      return (new TAis(apl, subestacao));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TAis::TAis(VTApl *apl, VTSubestacao *subestacao)
   {
   //salva ponteiro p/ objetos
   this->apl        = apl;
   PD.subestacao = subestacao;
   //cria objetos
   lisEQP  = new TList();
   lisCEL  = new TList();
   lisPTC  = new TList();
   //assume mesma cor da Rede
   Color   = Subestacao->Rede->Color;
   Visible = true;
   }

//---------------------------------------------------------------------------
__fastcall TAis::~TAis(void)
   {
   //destr�i lista sem destrui seus seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisCEL) {delete lisCEL; lisCEL = NULL;}
   //destr�i lista e seus objetos
   if (lisPTC) {LimpaTList(lisPTC); delete lisPTC; lisPTC = NULL;}
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TAis::BarraRefSubestacao(void)
   {
   //vari�veis locais
   int     x, y;
   VTBarra *barra;
   TList   *lisBAR = Subestacao->Rede->LisBarra();

   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->CoordenadasUtm_cm(x, y)) return(barra);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TAis::ComparaDistanciaPtoCarga(void *Item1, void *Item2)
   {
   return(CompareValue(((TPtoCarga*)Item1)->Distancia_cm, ((TPtoCarga*)Item2)->Distancia_cm));
   }

//---------------------------------------------------------------------------
double __fastcall TAis::DemandaAtendida_mva(void)
   {
   return(PD.s_mva);
   }

//---------------------------------------------------------------------------
double __fastcall TAis::DemandaLimite_mva(void)
   {
   return(PD.s_max_mva);
   }

//---------------------------------------------------------------------------
void __fastcall TAis::IniciaLisCarga(TList *lisCAR, bool carga_propria)
   {
   //vari�veis locais
   TList        *lisSUB;
   VTSubestacao *subestacao;
   VTRadial     *radial;

   //reinicia lisCAR
   lisCAR->Clear();
   //insere Cargas pr�prias (Prim�rios e Secundaros da Subestacao)
   InsereCarga(lisCAR, Subestacao);
   //verifica se deve carregar apenas as Cargas pr�prias (Prim�rios e Secundaros da Subestacao)
   if (carga_propria) return;
   //insere Cargas das demais Subestacoes
   radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
   lisSUB = radial->LisSubestacao();
   for (int n = 0; n < lisSUB->Count; n++)
      {
      subestacao = (VTSubestacao*)lisSUB->Items[n];
      if (subestacao != Subestacao) InsereCarga(lisCAR, subestacao);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TAis::IniciaLisPtoCarga(TList *lisCAR, int ind_pat)
   {
   //vari�veis locais
   int        x1, y1, x2, y2;
   //TList      *lisCAR;
   TPtoCarga  *ptocarga;
   VTBarra    *barra;
   VTCarga    *carga;
   //VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   VTDemanda  *demanda  = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   //VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisPTC
   LimpaTList(lisPTC);
   //determina Barra da Subestac�o que tenha coordenada utm
   if ((barra = BarraRefSubestacao()) == NULL) return(false);
   //determina coordenadas da Subestacao
   barra->CoordenadasUtm_cm(x1, y1);
   //obt�m lista de Cargas
   //lisCAR = consulta->LisEqptoRedeReducao(redes, eqptoCARGA);
   //cria um PtoCarga p/ cada Carga
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //obt�m coordenadas da Barra da Carga
      if (! carga->pbarra->CoordenadasUtm_cm(x2, y2)) continue;
      //cria um novo objetoPtoCarga
      lisPTC->Add(ptocarga = new TPtoCarga(carga));
      //salva coordendas do PtoCarga
      ptocarga->X_cm = x2;
      ptocarga->Y_cm = y2;
      //determina dist�ncia da Carga � Subestacao
      ptocarga->Distancia_cm = Distancia_cm(x1, y1, x2, y2);
      //determina demanda da Carga no patamar indicado
      ptocarga->S_mva = Abs(demanda->S_mva[carga][ind_pat]);
      }
   return(lisPTC->Count > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TAis::InsereCarga(TList *lisCAR, VTSubestacao *subestacao)
   {
   //vari�veis locais
   TList        *lisPRI;
   TList        *lisSEC;
   VTReducao    *reducao;
   VTPrimario   *primario;
   VTSecundario *secundario;

   //insere em lisCAR as Cargas dos Primarios da Subestacao
   lisPRI = subestacao->LisPrimario();
   for (int np = 0; np < lisPRI->Count; np++)
      {
      primario = (VTPrimario*)lisPRI->Items[np];
      //verifica se a Rede est� carregada
      if (primario->Rede->Carregada)
         {//obt�m lista de Cargas do Primario
         primario->Rede->LisEqbar(lisCAR, eqptoCARGA);
         }
/*
      else
         {//determina Cargas da Reducao da Rede
         reducao = (VTReducao*)((primario->Rede->MRede->LisReducao())->First());
         reducao->LisEqbar(lisCAR, eqptoCARGA);
         }
*/
      }
   //insere em lisCAR as Cargas dos Secundarios
   for (int np = 0; np < lisPRI->Count; np++)
      {
      primario = (VTPrimario*)lisPRI->Items[np];
      //verifica se a Rede est� carregada
      if (! primario->Rede->Carregada) continue;
      //obt�m lista de Secundarios
      lisSEC = primario->LisSecundario();
      for (int ns = 0; ns < lisSEC->Count; ns++)
         {
         secundario = (VTSecundario*)lisSEC->Items[ns];
         //verifica se a Rede est� carregada
         if (secundario->Rede->Carregada)
            {//obt�m lista de Cargas do Secundario
            secundario->Rede->LisEqbar(lisCAR, eqptoCARGA);
            }
         else
            {//determina Cargas da Reducao da Rede
            reducao = (VTReducao*)((secundario->Rede->MRede->LisReducao())->First());
            reducao->LisEqbar(lisCAR, eqptoCARGA);
            }
         }
      }
   }

//---------------------------------------------------------------------------
TList * __fastcall TAis::LisCelula(void)
   {
   return(lisCEL);
   }

//---------------------------------------------------------------------------
TList * __fastcall TAis::LisPtoCarga(void)
   {
   return(lisPTC);
   }

//---------------------------------------------------------------------------
bool __fastcall TAis::MontaAreaInfluencia(strOPCAO_AIS &opcao)
   {
   //vari�veis locais
   double     s_mva;
   TPtoCarga  *ptocarga;

   //reinicia lista de c�lulas do pol�gono
   lisCEL->Clear();
   //define demanda m�xima que a Subesta��o pode atender
   if (opcao.sfirme) PD.s_max_mva = Subestacao->Sfirme_mva * opcao.cargto_max_pu;
   else              PD.s_max_mva = Subestacao->Snom_mva   * opcao.cargto_max_pu;
   //determina lista de Cargas que a Subesta��o pode alimentar
   IniciaLisCarga(lisEQP, opcao.carga_propria);
   //prote��o
   if (lisEQP->Count == 0) return(true);
   //cria os PtoCarga da Subestacao
   if (! IniciaLisPtoCarga(lisEQP, opcao.ind_pat)) return(false);
   //ordena lista de PtoCarga em ordem crescente das dist�ncia
   lisPTC->Sort(ComparaDistanciaPtoCarga);
   //identifica subconjunto dos PtoCarga mais pr�ximso da Subestacao que
   //podem ser atendidos pela Subestacao
   //reinicia demanda total atendida
   PD.s_mva = 0.;
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptocarga = (TPtoCarga*)lisPTC->Items[n];
      if (! opcao.carga_propria)
         {//verifica se a demanda pode ser atendidida
         if ((PD.s_mva + ptocarga->S_mva) > PD.s_max_mva) break;
         }
      //atualiza demanda atendida
      PD.s_mva += ptocarga->S_mva;
      //marca que o PtoCarga faz parte da �rea de influ�ncia
      ptocarga->Incluido = true;
      }
   //monta pol�gono que cont�m todas os PtoCarga inclu�dos
   if (MontaPoligono())
      {//identifica celulas contidas no pol�gono da �rea de influ�ncia
      SelecionaCelulas();
      }
   return(true);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TAis::MontaPoligono(void)
   {
   //vari�veis locais
   TList        *lisPTC_ENV;
   TPtoCarga    *ptocarga;
   VTEnvoltoria *envoltoria;

   //destr�i eventual Poligono criado anteriormente
   if (poligono) {delete poligono; poligono = NULL;}
   //elimina PtoCarga n�o inclu�dos na �rea de influ�ncia
   for (int n = lisPTC->Count - 1; n >= 0; n--)
      {
      ptocarga = (TPtoCarga*)lisPTC->Items[n];
      if (ptocarga->Incluido) break;
      //retira PtoCarga da lista e destr�i objeto
      lisPTC->Delete(n);
      delete ptocarga;
      }
   //determina conjunto de PtoCarga que define a envolt�ria o pol�gono da �rea de influencia
   envoltoria = NewObjEnvoltoria();
   if ((lisPTC_ENV = envoltoria->Executa(lisPTC)) != NULL)
      {//cria objeto Poligono
      poligono = NewObjPoligono(lisPTC_ENV->Count);
      //insere coordenadas dos PtoCarga ordenados sequencialmente que definem a envolt�ria do Poligono
      for (int n = 0; n < lisPTC_ENV->Count; n++)
         {
         ptocarga = (TPtoCarga*)lisPTC_ENV->Items[n];
         poligono->InserePonto(ptocarga->X_cm, ptocarga->Y_cm);
         }
      }
   //destr�i objeto envoltoria
   delete envoltoria;

   return(poligono != NULL);
   }


//---------------------------------------------------------------------------
void __fastcall TAis::PintaCelulas(TColor color, bool merge)
   {
   //vari�veis locais
   VTCelula   *celula;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //define cor das Celulas contidas na  �rea de influ�ncia
   for (int i = 0; i < lisCEL->Count; i++)
      {
      celula = (VTCelula*)lisCEL->Items[i];
      //celula->Color = (merge) ? TColor(celula->Color | color) : color ;
      if ((merge)&&(celula->Color != redegraf->CorFundo))
         {celula->Color = TColor(celula->Color | color);}
      else
         {celula->Color = color;}
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TAis::SelecionaCelulas(void)
   {
   //vari�veis locais
   TPtoCarga *ptocarga;
   VTArea    *area_cm;
   VTCelula  *celula;
   VTCelulas *celulas    = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA  = celulas->LisCelula();

   //reinicia lista de Celulas da �rea de influ�ncia
   lisCEL->Clear();
   //prote��o: verifica se existe o poligono
   if (poligono == NULL) return;
   //insere Celulas de todos os PtoCarga
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptocarga = (TPtoCarga*)lisPTC->Items[n];
      celula = celulas->ExisteCelula(ptocarga->X_cm, ptocarga->Y_cm);
      if ((celula != NULL)&&(lisCEL->IndexOf(celula) < 0))
         {//insere Celula em lisCEL
         lisCEL->Add(celula);
         }
      }
   //identifica Celulas que est�o dentro do pol�gono da �rea de influ�ncia
   for (int i = 0; i < lisCELULA->Count; i++)
      {
      celula = (VTCelula*)lisCELULA->Items[i];
      //determina �rea utm da Celula
      area_cm = celula->Area;
      if (poligono->PontoInterno((area_cm->x1 + area_cm->x2)/2, (area_cm->y1 + area_cm->y2)/2))
         {//insere Celula em lisCEL
         if (lisCEL->IndexOf(celula) < 0) lisCEL->Add(celula);
         }
      }
  }
*/
//---------------------------------------------------------------------------
void __fastcall TAis::SelecionaCelulas(void)
   {
   //vari�veis locais
   TPtoCarga *ptocarga;
   VTArea    *area_cm;
   VTCelula  *celula;
   VTCelulas *celulas    = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA  = celulas->LisCelula();

   //prote��o
   if (poligono == NULL) return;
   //reinicia lista de Celulas do pol�gono
   lisCEL->Clear();
   //insere Celulas de todos os PtoCarga
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptocarga = (TPtoCarga*)lisPTC->Items[n];
      celula = celulas->ExisteCelula(ptocarga->X_cm, ptocarga->Y_cm);
      if ((celula != NULL)&&(lisCEL->IndexOf(celula) < 0))
         {//insere Celula em lisCEL
         lisCEL->Add(celula);
         }
      }
   //identifica Celulas que est�o dentro do pol�gono
   for (int i = 0; i < lisCELULA->Count; i++)
      {
      celula = (VTCelula*)lisCELULA->Items[i];
      //determina �rea utm da Celula
      area_cm = celula->Area;
      //verifica se Celula � externa � �rea retangular do pol�gono
      if (! poligono->AreaInternaAreaRetangular(area_cm->x1, area_cm->y1, area_cm->x2, area_cm->y2)) continue;
      //verifica se alguns dos pontos da Celula pertence a� �rea do pol�gono
      if ((poligono->PontoInterno(area_cm->x1, area_cm->y1)) ||
          (poligono->PontoInterno(area_cm->x2, area_cm->y2)) ||
          (poligono->PontoInterno(area_cm->x1, area_cm->y2)) ||
          (poligono->PontoInterno(area_cm->x2, area_cm->y1)))
         {//insere Celula em lisCEL
         if (lisCEL->IndexOf(celula) < 0) lisCEL->Add(celula);
         }
      }
   }

//---------------------------------------------------------------------------
//eof

