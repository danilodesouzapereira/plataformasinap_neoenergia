//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TCcs.h"
#include "..\AreaInfluencia\TPtoCarga.h"
#include "..\AreaInfluencia\VTEnvoltoria.h"
#include "..\AreaInfluencia\VTPoligono.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
//#include "..\..\Consulta\VTConsulta.h"
#include "..\..\Potencia\VTDemanda.h"
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
VTCcs* __fastcall NewObjCcs(VTApl *apl, VTSubestacao *subestacao)
   {
   try{
      return (new TCcs(apl, subestacao));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TCcs::TCcs(VTApl *apl, VTSubestacao *subestacao)
   {
   //salva ponteiro p/ objetos
   this->apl        = apl;
   PD.subestacao = subestacao;
   //cria objetos
   //lisEQP  = new TList();
   lisCEL  = new TList();
   lisEQP  = new TList();
   lisPTC  = new TList();
   //assume mesma cor da Rede
   Color   = Subestacao->Rede->Color;
   Visible = true;
   }

//---------------------------------------------------------------------------
__fastcall TCcs::~TCcs(void)
   {
   //destrói lista sem destrui seus seus objetos
   if (lisCEL) {delete lisCEL; lisCEL = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destrói lista e seus objetos
   if (lisPTC) {LimpaTList(lisPTC); delete lisPTC; lisPTC = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TCcs::CoordenadaSubestacao(void)
   {
   //variáveis locais
   int     num_bar = 0;
   double  x_cm    = 0; //importante usar double p/ evitar overflow
   double  y_cm    = 0;
   strUTM  utm;
   VTBarra *barra;
   TList   *lisBAR = Subestacao->Rede->LisBarra();

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (barra->CoordenadasUtm_cm(utm.x, utm.y))
         {
         x_cm += utm.x;
         y_cm += utm.y;
         num_bar++;
         }
      }
   //calcula coordenadas média
   if (num_bar > 0)
      {
      x_cm /= num_bar;
      y_cm /= num_bar;
      }
   //salva coordenadas
   centro_se.x = x_cm;
   centro_se.y = y_cm;

   return(num_bar > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TCcs::CalculaCoordenadasCentroCarga(void)
   {
   //variáveis locais
   TPtoCarga       *ptocarga;
   //importante usar double p/ evitar overflow
   double           dist_x_cm;
   double           dist_y_cm;
   double           s_mva       = 0.;
   complex<double>  dist_X_smva = CZero();

   //loop p/ todosmPtoCarga
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptocarga     = (TPtoCarga*)lisPTC->Items[n];
      dist_x_cm    = ptocarga->X_cm - centro_se.x;
      dist_y_cm    = ptocarga->Y_cm - centro_se.y;
      dist_X_smva += complex<double>(dist_x_cm, dist_y_cm) * ptocarga->S_mva;
      s_mva       += ptocarga->S_mva;
      }
   //inicia coordeadas do centro de carga com as coordenadas do centro da Subestacao
   centro_carga = centro_se;
   if (s_mva > 0)
      {
      dist_X_smva     = dist_X_smva / s_mva;
      centro_carga.x += dist_X_smva.real();
      centro_carga.y += dist_X_smva.imag();
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCcs::ComparaDistanciaPtoCarga(void *Item1, void *Item2)
   {
   return(CompareValue(((TPtoCarga*)Item1)->Distancia_cm, ((TPtoCarga*)Item2)->Distancia_cm));
   }

//---------------------------------------------------------------------------
bool __fastcall TCcs::CoordenadasCentroCarga(int &x_cm, int &y_cm)
   {
   //copia coordenadas do centro de carga
   x_cm = centro_carga.x;
   y_cm = centro_carga.y;

   return((x_cm != 0)&&(y_cm != 0));
   }
/*
//---------------------------------------------------------------------------
TColor __fastcall TCcs::Degrade(TColor color, double valor_pu)
   {
   //variáveis locais
   unsigned r = ((color & 0xff))           * valor_pu;
   unsigned g = ((color & 0xff00) >> 8)    * valor_pu;
   unsigned b = ((color & 0xff0000) >> 16) * valor_pu;

   return(TColor(r + (g << 8) + (b << 16)));
   }
*/
//---------------------------------------------------------------------------
TColor __fastcall TCcs::Degrade(TColor beginColor, TColor endColor, double valor_pu)
   {
   //variáveis locais
   unsigned r;
   unsigned g;
   unsigned b;
   unsigned r1 = ((beginColor & 0xff));
   unsigned g1 = ((beginColor & 0xff00) >> 8);
   unsigned b1 = ((beginColor & 0xff0000) >> 16);
   unsigned r2 = ((endColor & 0xff));
   unsigned g2 = ((endColor & 0xff00) >> 8);
   unsigned b2 = ((endColor & 0xff0000) >> 16);

   r = ((1 - valor_pu) * r1) + (valor_pu * r2);
   g = ((1 - valor_pu) * g1) + (valor_pu * g2);
   b = ((1 - valor_pu) * b1) + (valor_pu * b2);

   //retorna cor
   return(TColor(r + (g << 8) + (b << 16)));
   }

//---------------------------------------------------------------------------
void __fastcall TCcs::IniciaLisCarga(TList *lisCAR)
   {
   //variáveis locais
   TList        *lisPRI;
   TList        *lisSEC;
   VTReducao    *reducao;
   VTPrimario   *primario;
   VTSecundario *secundario;

   //reinicia lisCAR
   lisCAR->Clear();
   //insere em lisCAR as Cargas dos Primarios
   lisPRI = Subestacao->LisPrimario();
   for (int np = 0; np < lisPRI->Count; np++)
      {
      primario = (VTPrimario*)lisPRI->Items[np];
      //verifica se a Rede está carregada
      if (primario->Rede->Carregada)
         {//obtém lista de Cargas do Primario
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
      //verifica se a Rede está carregada
      if (! primario->Rede->Carregada) continue;
      //obtém lista de Secundarios
      lisSEC = primario->LisSecundario();
      for (int ns = 0; ns < lisSEC->Count; ns++)
         {
         secundario = (VTSecundario*)lisSEC->Items[ns];
         //verifica se a Rede está carregada
         if (secundario->Rede->Carregada)
            {//obtém lista de Cargas do Secundario
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
bool __fastcall TCcs::IniciaLisPtoCarga(int ind_pat)
   {
   //variáveis locais
   int        x, y;
   TPtoCarga  *ptocarga;
   VTBarra    *barra;
   VTCarga    *carga;
   VTDemanda  *demanda  = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //reinicia lisPTC
   LimpaTList(lisPTC);
   //determina lista de Cargas
   IniciaLisCarga(lisEQP);
   //cria um PtoCarga p/ cada Carga
   for (int nc = 0; nc < lisEQP->Count; nc++)
      {
      carga = (VTCarga*)lisEQP->Items[nc];
      //obtém coordenadas da Barra da Carga
      if (! carga->pbarra->CoordenadasUtm_cm(x, y)) continue;
      //cria um novo objetoPtoCarga
      lisPTC->Add(ptocarga = new TPtoCarga(carga));
      //salva coordendas do PtoCarga
      ptocarga->X_cm = x;
      ptocarga->Y_cm = y;
      //determina distância da Carga à Subestacao
      ptocarga->Distancia_cm = Distancia_cm(centro_se.x, centro_se.y, x, y);
      //determina demanda da Carga no patamar indicado
      ptocarga->S_mva = Abs(demanda->S_mva[carga][ind_pat]);
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList * __fastcall TCcs::LisCelula(void)
   {
   return(lisCEL);
   }

//---------------------------------------------------------------------------
TList * __fastcall TCcs::LisPtoCarga(void)
   {
   return(lisPTC);
   }

//---------------------------------------------------------------------------
bool __fastcall TCcs::MontaCentroCarga(int ind_pat)
   {
   //variáveis locais
   TPtoCarga  *ptocarga;

   //determina coordenadas do centro da Subestacao
   if (! CoordenadaSubestacao()) return(false);
   //cria os PtoCarga p/ as Cargas da da Subestacao
   if (! IniciaLisPtoCarga(ind_pat)) return(false);
   //determina coordenadas do centro de carga
   CalculaCoordenadasCentroCarga();
   //ordena lista de PtoCarga em ordem crescente das distância
   lisPTC->Sort(ComparaDistanciaPtoCarga);
   //monta polígono que contêm todas os PtoCarga incluídos
   MontaPoligono();
   //identifica celulas contidas no polígono da área de influência
   SelecionaCelulas();
   //define cor das Celulas
   return(true);
   }
   
//---------------------------------------------------------------------------
void __fastcall TCcs::MontaPoligono(void)
   {
   //variáveis locais
   TList        *lisPTC_ENV;
   TPtoCarga    *ptocarga;
   VTEnvoltoria *envoltoria;

   //destrói eventual Pologono criado anteriormente
   if (poligono) {delete poligono;}
   //determina conjunto de PtoCarga que define a envoltória o polígono da área de influencia
   envoltoria = NewObjEnvoltoria();
   if ((lisPTC_ENV = envoltoria->Executa(lisPTC)) != NULL)
      {//cria objeto Poligono
      poligono = NewObjPoligono(lisPTC_ENV->Count);
      //insere coordenadas dos PtoCarga ordenados sequencialmente que definem a envoltória do Poligono
      for (int n = 0; n < lisPTC_ENV->Count; n++)
         {
         ptocarga = (TPtoCarga*)lisPTC_ENV->Items[n];
         poligono->InserePonto(ptocarga->X_cm, ptocarga->Y_cm);
         }
      }
   //destrói objeto envoltoria
   delete envoltoria;
   }


//---------------------------------------------------------------------------
void __fastcall TCcs::PintaCelulas(TColor color, bool merge)
   {
   //variáveis locais
   double     dist, dist_max;
   TColor     color_cel;
   VTCelula   *celula, *celula_ref;
   VTCelulas  *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //determina Celula do centro de carga
   if ((celula_ref = celulas->ExisteCelula(centro_carga.x, centro_carga.y)) == NULL) return;
   //pinta Celula do centro de carga com cor preta
   celula_ref->Color = clBlack;
   //determina distância máxima das celulas
   dist_max = 1;
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula   = (VTCelula*)lisCEL->Items[n];
      dist     = celulas->Distancia(celula_ref, celula);
      dist_max = Max(dist, dist_max);
      }
   //define cor das Celulas contidas no polígono
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      if (celula == celula_ref) continue;
      //define cor, considerando efeito degrade
      dist      = celulas->Distancia(celula_ref, celula);
      color_cel = Degrade(color, clWhite, dist/dist_max);
      if ((merge)&&(celula->Color != redegraf->CorFundo))
         {celula->Color = TColor(celula->Color | color_cel);}
      else
         {celula->Color = color_cel;}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCcs::SelecionaCelulas(void)
   {
   //variáveis locais
   TPtoCarga *ptocarga;
   VTArea    *area_cm;
   VTCelula  *celula;
   VTCelulas *celulas    = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA  = celulas->LisCelula();

   //reinicia lista de Celulas do polígono
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
   //identifica Celulas que estão dentro do polígono
   for (int i = 0; i < lisCELULA->Count; i++)
      {
      celula = (VTCelula*)lisCELULA->Items[i];
      //determina área utm da Celula
      area_cm = celula->Area;
      //verifica se Celula é externa à àrea retangular do polígono
      if (! poligono->AreaInternaAreaRetangular(area_cm->x1, area_cm->y1, area_cm->x2, area_cm->y2)) continue;
      //verifica se alguns dos pontos da Celula pertence aà área do polígono
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

