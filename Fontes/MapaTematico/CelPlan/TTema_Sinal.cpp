//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTema_Sinal.h"
#include "TFaixa_Sinal.h"
#include "..\Apl\VTApl.h"
#include "..\Cartografia\VTScg.h"
#include "..\Complexo\Complexo.h"
//#include "..\Potencia\VTDemanda.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
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
#include "..\..\DLL_Inc\Cartografia.h"
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
VTTema_Sinal* __fastcall NewObjTema_Sinal(VTApl *apl)
   {
   try{
   return(new TTema_Sinal(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TTema_Sinal::TTema_Sinal(VTApl *apl)
   {
   VTLicencas *licencas =(VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca *licenca = licencas->ExisteLicenca();

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
   //cria um VTScg
//   scg = DLL_NewObjScg(apl, ELETROBRAS_ALAGOAS, "ELETROBRAS - Alagoas");
 	//cria um VTScg
   scg = DLL_NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
   }

//---------------------------------------------------------------------------
__fastcall TTema_Sinal::~TTema_Sinal(void)
   {
   //variáveis locais
   TFaixa_Sinal *faixa_sinal;

   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisFAIXA_ENABLED) {delete lisFAIXA_ENABLED; lisFAIXA_ENABLED = NULL;}
   //destrói lista e seus objetos
   if (lisFAIXA) {LimpaTList(lisFAIXA); delete lisFAIXA; lisFAIXA = NULL;}
   //destrói objetos
   if (scg)    {delete scg;    scg    = NULL;}
   }

//---------------------------------------------------------------------------
//void __fastcall TTema_Sinal::CalculaValorTotalCelula(void)
//   {
//   //variáveis locais
//   //int       num_celula;
//   int     dist;
//   double  fatmult;
//   struct   {
//            int       linha, coluna;
//            VTCelula  *celula;
//            }ref, viz;
//   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//   TList     *lisCEL  = celulas->LisCelula();
//
//   //obtém dimensão do grid de celulas
//   //num_celula = celulas->NumCelula;
//   //zera valor total de todas celulas
//   for (int i = 0; i < lisCEL->Count; i++)
//      {
//      ref.celula = (VTCelula*)lisCEL->Items[i];
//      ref.celula->ValorTotal = 0;
//      ref.celula->NumParcela = 1;
//      }
//   //loop p/ todas celulas
//   for (int i = 0; i < lisCEL->Count; i++)
//      {
//      ref.celula = (VTCelula*)lisCEL->Items[i];
//      //verifica se a célula de referência tem valor próprio
//      if (IsDoubleZero(ref.celula->ValorProprio)) continue;
//      //determina posicionamento da célula de referência no grid
//      if (!  celulas->IndicesDaCelula(ref.celula, ref.linha, ref.coluna)) continue;
//      //distribui seu valor na sua área de influência
//      for (int j = 0; j < lisCEL->Count; j++)
//         {
//         viz.celula = (VTCelula*)lisCEL->Items[j];
//         //determina posicionamento da célula vizinha no grid
//         if (!  celulas->IndicesDaCelula(viz.celula, viz.linha, viz.coluna)) continue;
//         //determina distância entre célula de referência e célula vizinha
//         dist    = Max(abs(ref.linha - viz.linha), abs(ref.coluna - viz.coluna));
//         //fatmult = (dist == 0) ? 1. : (1. / (dist * 8));
////Testes
//fatmult = (dist == 0) ? 1. : (1. / (dist + 1));
//         //totaliza contribuição da célula de referência na célula vizinha
//         viz.celula->ValorTotal = viz.celula->ValorTotal + (ref.celula->ValorProprio * fatmult);
//         }
//      }
//   }

//---------------------------------------------------------------------------
void  __fastcall  TTema_Sinal::ConverteLatLonGMStoSINAP(TList *lisSTR)
{
	VTLicencas *licencas =(VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();

    switch (licenca->Empresa)
	{
		case COPEL:
		case CELPE:
		case COELBA:
		case COSERN:
			ConverteLatLonGMStoLatLonSNP(lisSTR);
			break;
		default:
			ConverteLatLonGMStoUTM(lisSTR);
		;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTema_Sinal::ConverteLatLonGMStoUTM(TList *lisSTR)
   {//variaveis locais
   double lat, lon;
   strImpCelPlanner *impCelPlanner;
   double x, y;

   scg->ZonaUTM = "25L";
   //percorre a lsita com as estruturas do importa cel planner
   for (int n = 0; n < lisSTR->Count; n++)
	  {
	  impCelPlanner = (strImpCelPlanner*) lisSTR->Items[n];
	  //gera a latitude
	  lat = 0.0;
      lat += impCelPlanner->lat.grau;
	  lat += ((double)impCelPlanner->lat.minuto)*0.01666666666666666666666666666667;
	  lat += impCelPlanner->lat.segundo*0.000277777777777777777777777777778;
      //verifica a letra
	  if(impCelPlanner->lat.hemisferio.AnsiCompare("S")==0)
		 {lat = lat*(-1);}
	  //gera a longitude
      lon = 0.0;
	  lon += impCelPlanner->lon.grau;
	  lon += ((double)impCelPlanner->lon.minuto)*0.01666666666666666666666666666667;
      lon += impCelPlanner->lon.segundo*0.000277777777777777777777777777778;
	  //verifica a letra
	  if(impCelPlanner->lon.hemisferio.AnsiCompare("W")==0)
		 {lon = lon*(-1);}
	  //converte
	  scg->LatLon_to_NorthEast(lat,lon,y,x);
	  impCelPlanner->utm.y = y;
	  impCelPlanner->utm.x = x;
	  }
   }
//---------------------------------------------------------------------------
void   __fastcall  TTema_Sinal::ConverteLatLonGMStoLatLonSNP(TList *lisSTR)
{//variaveis locais
	double lat, lon;
	strImpCelPlanner *impCelPlanner;
	double x, y;

	scg->ZonaUTM = "25L";
	//percorre a lsita com as estruturas do importa cel planner
	for (int n = 0; n < lisSTR->Count; n++)
	{
		impCelPlanner = (strImpCelPlanner*) lisSTR->Items[n];
		//gera a latitude
		lat = 0.0;
		lat += impCelPlanner->lat.grau;
		lat += ((double)impCelPlanner->lat.minuto)*0.01666666666666666666666666666667;
		lat += impCelPlanner->lat.segundo*0.000277777777777777777777777777778;
		//verifica a letra
		if(impCelPlanner->lat.hemisferio.AnsiCompare("S")==0)
			{lat = lat*(-1);}
		//gera a longitude
		lon = 0.0;
		lon += impCelPlanner->lon.grau;
		lon += ((double)impCelPlanner->lon.minuto)*0.01666666666666666666666666666667;
		lon += impCelPlanner->lon.segundo*0.000277777777777777777777777777778;
		//verifica a letra
		if(impCelPlanner->lon.hemisferio.AnsiCompare("W")==0)
			{lon = lon*(-1);}
		//converte
 		scg->LatLon_to_FakeLatLon(y,x,lat,lon);
		impCelPlanner->utm.y = y;
		impCelPlanner->utm.x = x;
	}
}
//---------------------------------------------------------------------------
TColor __fastcall TTema_Sinal::Degrade(TColor color, double valor)
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
TColor __fastcall TTema_Sinal::Degrade(TColor beginColor, TColor endColor, double valor)
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
TColor __fastcall TTema_Sinal::DegradeCinza(double valor)
   {
   //variáveis locais
   unsigned valor_hexa = 0xff * valor;

   //retorna tom de cinza
   return(TColor(valor_hexa + (valor_hexa << 8) + (valor_hexa << 16)));
   }

//---------------------------------------------------------------------------
void __fastcall  TTema_Sinal::Executa(TList *lisImpCelPlan)
   {
   IniciaLisCelula(lisImpCelPlan);
   //NÃO prepara os dados p/ o degrade p/ melhorar performance
   degrade_ok = false;
   //monta faixas de densidade
   MontaFaixaSinal();
   }

//---------------------------------------------------------------------------
//void __fastcall  TTema_Sinal::Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat)
//   {
//   IniciaLisCelula(lisREDE,lisCLASSE, ind_pat);
//   //NÃO prepara os dados p/ o degrade p/ melhorar performance
//   degrade_ok = false;
//   //inicia com o "default"
//   //FaixaValoresUniforme();
//   //monta faixas de densidade
//   MontaFaixaDemanda();
//   }
////---------------------------------------------------------------------------
//void __fastcall  TTema_Sinal::Executa(TList *lisREDE, TList *lisCLASSE)
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
//void __fastcall  TTema_Sinal::Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat)
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
void __fastcall  TTema_Sinal::FaixaValoresCelPlanner(void)
   {
   //variaveis locais
   TFaixa_Sinal *faixa_sinal;

   //determina valores default
   double limite[15] = {-150, -104, -94, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110};
//    double limite[5] = {-150, -110, -70, -30, 0};

   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {//inicia atributos
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      faixa_sinal->sinal_min_db  = limite[nf];
      faixa_sinal->sinal_max_db  = limite[nf+1];
      if(nf >= 4)
         {
         faixa_sinal->enabled = false;
         }
      if(nf == 3)
         {
         faixa_sinal->visible = false;
         }
      }
   }
//---------------------------------------------------------------------------
void __fastcall  TTema_Sinal::FaixaValoresDefault(void)
   {
   //variaveis locais
   TFaixa_Sinal *faixa_sinal;

   //determina valores default
   double limite[15] = {-150, -140, -130, -120, -110, -100, -90, -80, -70, -60,
                                                       -50, -40, -30, -20, 0};
//    double limite[5] = {-150, -110, -70, -30, 0};

   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {//inicia atributos
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      faixa_sinal->sinal_min_db  = limite[nf];
      faixa_sinal->sinal_max_db  = limite[nf+1];
      }
   }
//---------------------------------------------------------------------------

/*
//---------------------------------------------------------------------------
void __fastcall  TTema_Sinal::FaixaValoresDemanda(void)
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
//void __fastcall  TTema_Sinal::FaixaValoresSinal(void)
//   {
//   //variáveis locais
//   int       index;
//   double    valor_ini, valor_fim, media, total;
//   TFaixa_Sinal    *faixa_sinal;
//   VTCelula  *celula;
//   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//   TList     *lisCELULA = celulas->LisCelula();
//   TList     *lisORDENADA;
//   //TList     *lisENABLED;
//
//   //proteção
//   if(lisFAIXA->Count == 0) return;
//   //inicia valores
//   valor_ini = valor_fim = media = total = 0.;
//   index = 0;
//   //cria lista
//   lisORDENADA = new TList();
//
//   IniciaLisFaixaEnabled();
//   //copia itens da lista
//   CopiaTList(lisCELULA, lisORDENADA);
//   //remove celulas com valor nulo
//   RemoveCelulaNula(lisORDENADA);
//   //ordena celulas
//   if (lisORDENADA->Count > 1) lisORDENADA->Sort(ComparaValor);
//   //verifica valor total das celulas, comparado à quantidade de faixas
////   for (int nc = 0; nc < lisORDENADA->Count; nc++)
////      {
////      celula = (VTCelula*)lisORDENADA->Items[nc];
////      //total += celula->ValorProprio;
////      }
//   //media = ((total) / (lisFAIXA->Count + 1));
//   media = ((total) / (lisFAIXA_ENABLED->Count + 1));
//   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
//   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
//      {//salva ultimo valor
//      valor_ini = valor_fim;
//      //reinicia total
//      total = 0.;
//      while((index < (lisORDENADA->Count)) && (total < media))
//         {//acumula valores das celulas que compoem a faixa
//         celula = (VTCelula*)lisORDENADA->Items[index];
//         total += celula->ValorProprio;
//         index++;
//         }
//      if(celula) valor_fim = celula->ValorProprio;
//      //insere atributos
//      //faixa = (TFaixa*)lisFAIXA->Items[nf];
//      faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
//      faixa_sinal->sinal_min_db  = valor_ini;
//      faixa_sinal->sinal_max_db  = valor_fim;
//      }
//   //destroi lista
//   delete lisORDENADA;
//   }

//---------------------------------------------------------------------------
//void __fastcall  TTema_Sinal::FaixaValoresQtdCelula(void)
//   {
//   //variáveis locais
//   int       media, index;
//   double    valor_ini, valor_fim;
//   TFaixa_Sinal    *faixa_sinal;
//   VTCelula  *celula;
//   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//   TList     *lisCELULA = celulas->LisCelula();
//   TList     *lisORDENADA;
//
//
//   //proteção
//   if(lisFAIXA->Count == 0) return;
//   //inicia valores
//   valor_ini = valor_fim = 0.;
//   index = 0;
//   //cria lista
//   lisORDENADA = new TList();
//   //lisENABLED = new TList();
//   //preenche lista com as faixas habilitadas
////   for (int nl = 0; nl < lisFAIXA->Count; nl++)
////      {
////      faixa = (TFaixa*)lisFAIXA->Items[nl];
////      if(faixa->enabled)
////         {lisENABLED->Add(faixa);}
////      }
//   IniciaLisFaixaEnabled();
//   //copia itens da lista
//   CopiaTList(lisCELULA, lisORDENADA);
//   //remove celulas com valor nulo
//   RemoveCelulaNula(lisORDENADA);
//   //ordena celulas
//   if (lisORDENADA->Count > 1) lisORDENADA->Sort(ComparaValor);
//   //verifica quantidade de celulas, comparado à quantidade de faixas
//   //(a media sempre será, no mínimo, 1)
//   //media = int((lisORDENADA->Count) / (lisFAIXA->Count)) + 1;
//   media = int((lisORDENADA->Count) / (lisFAIXA_ENABLED->Count)) + 1;
//   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
//   for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
//      {//salva ultimo valor
//      valor_ini = valor_fim;
//      while((index < lisORDENADA->Count) && (index <((nf+1)*media)))
//         {//acumula valores das celulas que compoem a faixa
//         celula = (VTCelula*)lisORDENADA->Items[index];
//         valor_fim = celula->ValorProprio;
//         index++;
//         }
//      //insere atributos
//      //faixa = (TFaixa_Sinal*)lisFAIXA->Items[nf];
//      faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
//      faixa_sinal->sinal_min_db  = valor_ini;
//      faixa_sinal->sinal_max_db  = valor_fim;
//      }
//   //destroi lista
//   delete lisORDENADA;
//   }

//---------------------------------------------------------------------------
void   __fastcall  TTema_Sinal::FaixaValoresUniforme(void)
 {
   //variáveis locais
   int       index;
   double    valor_ini, valor_fim;//, media, total;
   TFaixa_Sinal    *faixa_sinal;
   VTCelula  *celula;
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   TList     *lisORDENADA;

   double menorValor = 0;
   double maiorValor = 0;
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
   //cria um vetor double

   //verifica valor total das celulas, comparado à quantidade de faixas
   for (int nc = 0; nc < lisCELULA->Count; nc++)
      {
      celula = (VTCelula*)lisCELULA->Items[nc];
      //guarda o menor valor
      if(!IsDoubleZero(celula->ValorProprio))
         {
          if(menorValor > celula->ValorProprio)
            {menorValor = celula->ValorProprio;}
         }
      }
   //iguala o menor ao maior valor
   maiorValor = menorValor;
   for (int nc = 0; nc < lisCELULA->Count; nc++)
      {
      celula = (VTCelula*)lisCELULA->Items[nc];
      //guarda o menor valor
      if(!IsDoubleZero(celula->ValorProprio))
         {
          if(maiorValor < celula->ValorProprio)
         {maiorValor = celula->ValorProprio;}
         }
      }
   //intervaloDem = ((total) / (lisFAIXA->Count + 1));
   intervaloDem = ((maiorValor - menorValor) / (lisFAIXA_ENABLED->Count - 1));
   //for (int nf = 0; nf < lisFAIXA->Count; nf++)
   for (int nf = (lisFAIXA_ENABLED->Count - 1); nf >= 0; nf--)
      {//salva ultimo valor
      valor_ini = valor_fim;
      //reinicia total
      valor_fim = valor_ini + intervaloDem*(-1);
      //insere atributos
      //faixa = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
      if(nf != (lisFAIXA_ENABLED->Count - 1))
         {
         faixa_sinal->sinal_max_db  = valor_ini;
         faixa_sinal->sinal_min_db  = valor_fim;
         }
      else
         {
         faixa_sinal->sinal_max_db  = 10;
         faixa_sinal->sinal_min_db  = 0;
         faixa_sinal->visible = false;
         valor_fim = maiorValor;
         }
      }
   //destroi lista
   delete lisORDENADA;
   //delete lisENABLED;
   }

//---------------------------------------------------------------------------
//void __fastcall  TTema_Sinal::IniciaLisCarga(VTRede *rede, TList *lisCARGA)
//   {
//   //variáveis locais
//   TList     *lisREDU;
//   VTReducao *reducao;
//
//   //reinicia lisCARGA
//   lisCARGA->Clear();
//   //verifica se a Rede está carregada
//   if (rede->Carregada)
//      {
//      rede->LisEqbar(lisCARGA, eqptoCARGA);
//      }
//   else
//      {//determina Reducao da Rede
//      lisREDU = rede->MRede->LisReducao();
//      if (lisREDU->Count == 1)
//         {
//         reducao = (VTReducao*)lisREDU->First();
//         reducao->LisEqpto(lisCARGA, eqptoCARGA);
//         }
//      }
//   }
//---------------------------------------------------------------------------
void   __fastcall TTema_Sinal::IniciaLisCelula(TList *lisImpCelPlan)
   {//variáveis locais
   int       x_cm, y_cm;
   VTCelula  *celula;
   VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();
   strImpCelPlanner *impCelPlanner;

   try
      {//reinicia valores das Celulas
      for (int nc = 0; nc < lisCELULA->Count; nc++)
         {
         celula = (VTCelula*)lisCELULA->Items[nc];
         celula->ValorProprio = 0.001;
         celula->ValorTotal   = 0;
         }
      //loop p/ todas estruturas importadas
      for (int nr = 0; nr < lisImpCelPlan->Count; nr++)
         {
         impCelPlanner = (strImpCelPlanner*)lisImpCelPlan->Items[nr];
         x_cm = impCelPlanner->utm.x*100;
         y_cm = impCelPlanner->utm.y*100;
         //determina Celula da Carga
         if ((celula = celulas->ExisteCelula(x_cm, y_cm)) == NULL) continue;

         //atualiza valor médio da Celula c/ a demanda máxima da Carga
         celula->ValorProprio = ((celula->ValorProprio*celula->ValorTotal)
                                 + impCelPlanner->valor)/(celula->ValorTotal + 1);
         //atualiza valorTotal, usado como contador de ocorrencias dentro da celula
         celula->ValorTotal += 1.0;
         }
      //mostra que foi inicializado
      lisCelula_iniciado = true;
      }
   catch (Exception &e)
      {//mostra que foi  Não inicializado
      lisCelula_iniciado = false;
      }
   }
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------

void __fastcall  TTema_Sinal::IniciaLisFaixa(void)
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
void __fastcall  TTema_Sinal::IniciaLisFaixa(void)
   {
   //variáveis locais
   TFaixa_Sinal *faixa_sinal;
   int    num_faixa = 15;
   TColor color[15] = {clWebSalmon, clWebCornFlowerBlue, clLime, clGreen, clYellow, clBlue,
                      clMoneyGreen, clSkyBlue, clTeal, clOlive, clGray,  clCream,
                      clFuchsia, clNavy, clPurple};

//   int    num_faixa = 5;
//   TColor color[5] = {clTeal, clSkyBlue, clGray,  clCream, clFuchsia};

   for (int nf = 0; nf < num_faixa; nf++)
      {//cria objeto Faixa
      lisFAIXA->Add(faixa_sinal = new TFaixa_Sinal());
      //inicia atributos
      faixa_sinal->enabled      = true;
      faixa_sinal->visible      = true;
      faixa_sinal->color        = color[nf];
      }

   //insere valores default dos limites
   //FaixaValoresDefault();
   FaixaValoresCelPlanner();
   //FaixaValoresUniforme(); //FKM 2016.07.14
   }

//---------------------------------------------------------------------------
void   __fastcall TTema_Sinal::IniciaLisFaixaEnabled(void)
   {//variáveis locais
   TFaixa_Sinal    *faixa_sinal;

   lisFAIXA_ENABLED->Clear();
   for (int nf = 0; nf< lisFAIXA->Count; nf++)
      {
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      if(faixa_sinal->enabled)
         {lisFAIXA_ENABLED->Add(faixa_sinal);}
      }
   }
//---------------------------------------------------------------------------
TList* __fastcall TTema_Sinal::LisFaixa(void)
   {
   return(lisFAIXA);
   }

//---------------------------------------------------------------------------
void __fastcall TTema_Sinal::MontaFaixaSinal(void)
   {//variáveis locais
   TFaixa_Sinal    *faixa_sinal;
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCELULA = celulas->LisCelula();


   //reinicia atributos das Faixas
   for (int nf = 0; nf < lisFAIXA->Count; nf++)
      {
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      //reinicia lista de Celulas
      faixa_sinal->lisCELULA->Clear();
      }
   //inicializa faixas habilitadas
   IniciaLisFaixaEnabled();
   //determina Celulas de cada faixa
   for (int n = 0; n < lisCELULA->Count; n++)
      {
      celula = (VTCelula*)lisCELULA->Items[n];
      //for (int nf = 0; nf < lisFAIXA->Count; nf++)
      for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
         {
         //faixa = (TFaixa*)lisFAIXA->Items[nf];
         faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
         if (! faixa_sinal->enabled) continue;
         //verifica se o valor próprio da celula está dentro dos limites da faixa ou se é a última faixa
         if ((celula->ValorProprio <= faixa_sinal->sinal_max_db)||(nf == (lisFAIXA->Count-1)))
            {//insere Celula na lista da faixa
            faixa_sinal->lisCELULA->Add(celula);
            //faixa_sinal->dem_tot_mva += celula->ValorProprio;
            //interrompe o loop
            break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
//void __fastcall  TTema_Sinal::MontaGraficoSinalMax(void)
//   {
//   MontaGraficoPadrao();
//   }
// //FKM 2015.10.01 Monta degradÊ somente pras faixas habilitadas
// //---------------------------------------------------------------------------
 bool __fastcall  TTema_Sinal::MontaGraficoDegradeFaixa(TColor color1, TColor color2, bool linearDem)
   {
   //variáveis locais
   double     valor;
   TFaixa_Sinal     *faixa_sinal;
   double     max, min;
   double     intervalo;

   //TList      *lisFAIXA_ENABLED = new TList;
   VTCelula   *celula;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTCelulas  *celulas  = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList      *lisCEL   = celulas->LisCelula();

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
         faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
         valor = double(nf+1)/lisFAIXA_ENABLED->Count;
         faixa_sinal->color = Degrade(color1, color2, valor);
         }
      }
   else
      {//faz o degrade de acordo com a demanda
      //verifica o intervalo de demanda das faixas
      for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
         {
         faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
         //inicializa max e min
         if(nf == 0)
            {
            max = faixa_sinal->sinal_max_db;
            min = faixa_sinal->sinal_min_db;
            }
         //guarda os maiores e menores
         if(max < faixa_sinal->sinal_max_db) max = faixa_sinal->sinal_max_db;
         if(min > faixa_sinal->sinal_min_db) min = faixa_sinal->sinal_min_db;
         }
      //protecao
      if (IsDoubleZero(min))
         {min = 1.0;}
      //define cor das Celulas
      for (int nf = 0; nf < lisFAIXA_ENABLED->Count; nf++)
         {
         faixa_sinal = (TFaixa_Sinal*)lisFAIXA_ENABLED->Items[nf];
         //valor = double(nf+1)/lisFAIXA_ENABLED->Count;
         valor = faixa_sinal->sinal_max_db / min;
         if(valor >= 1.0) valor = 1.0;
         faixa_sinal->color = Degrade(color1, color2, valor);
         }
      }
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
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      if (faixa_sinal->visible)
         {//define cor das Celuas da Faixa
         for (int n = 0; n < faixa_sinal->lisCELULA->Count; n++)
            {
            celula = (VTCelula*)faixa_sinal->lisCELULA->Items[n];
            //define cor da célula em função do seu valor
            celula->Color = faixa_sinal->color;
            }
         }
      }
   return(true);
   }
//---------------------------------------------------------------------------
void __fastcall  TTema_Sinal::MontaGraficoPadrao(void)
   {//variáveis locais
   TFaixa_Sinal     *faixa_sinal;
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
      faixa_sinal = (TFaixa_Sinal*)lisFAIXA->Items[nf];
      if (faixa_sinal->visible)
         {//define cor das Celuas da Faixa
         for (int n = 0; n < faixa_sinal->lisCELULA->Count; n++)
            {
            celula = (VTCelula*)faixa_sinal->lisCELULA->Items[n];
            //define cor da célula em função do seu valor
            celula->Color = faixa_sinal->color;
            }
         }
      }
   }

//---------------------------------------------------------------------------
//void __fastcall  TTema_Sinal::NormalizaValorTotalCelula(void)
//   {
//   //variáveis locais
//   bool      first = true;
//   double    valor_max, valor_min, dividendo;
//   VTCelula  *celula;
//   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//   TList     *lisCEL  = celulas->LisCelula();
//
//   //loop p/ todas celulas
//   for (int n = 0; n < lisCEL->Count; n++)
//      {
//      celula = (VTCelula*)lisCEL->Items[n];
//      if (celula->NumParcela == 0) continue;
//      if (first) {valor_min = valor_max = celula->ValorTotal; first = false;}
//      valor_max = Max(valor_max, fabs(celula->ValorTotal));
//      valor_min = Min(valor_min, fabs(celula->ValorTotal));
//      }
//   //determina dividendo
//   dividendo = valor_max - valor_min;
//   //proteção
//   if (IsDoubleZero(dividendo)) return;
//   //calcula valor normalizado das celulas
//   for (int n = 0; n < lisCEL->Count; n++)
//      {
//      celula = (VTCelula*)lisCEL->Items[n];
//      celula->ValorTotal = (celula->ValorTotal - valor_min) / dividendo;
//      }
//   }

//---------------------------------------------------------------------------
void __fastcall  TTema_Sinal::RemoveCelulaNula(TList *lisORDENADA)
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
int __fastcall  TTema_Sinal::TotalCelula(void)
   {
   return(total.qtde_cel);
   }

//---------------------------------------------------------------------------
//double __fastcall  TTema_Sinal::TotalDemanda_mva(void)
//   {
//   return(total.s_mva);
//   }

//---------------------------------------------------------------------------
//eof

