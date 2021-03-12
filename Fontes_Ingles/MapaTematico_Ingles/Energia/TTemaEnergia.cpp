//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTemaEnergia.h"
#include "VTCfgCor.h"
#include "..\Apl\VTApl.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTCarga.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTemaEnergia::TTemaEnergia(VTApl *apl) : TTema(apl)
   {
   //define c�digo do tema
   Codigo = "Consumo de Energia";
   //cria objetos
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TTemaEnergia::~TTemaEnergia(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TTemaEnergia::CalculaValorProprioCelula(void)
   {
   //vari�veis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //define consumo de energia em cada celula
   for (int i = 0; i < lisCEL->Count; i++)
      {
      celula = (VTCelula*)lisCEL->Items[i];
      celula->ValorProprio = ConsumoEnergia(celula);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTemaEnergia::CalculaValorTotalCelula(void)
   {
   //vari�veis locais
   //int       num_celula;
   int     dist;
   double  fatmult;
   struct   {
            int       linha, coluna;
            VTCelula  *celula;
            }ref, viz;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //obt�m dimens�o do grid de celulas
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
      //verifica se a c�lula de refer�ncia tem valor pr�prio
      if (IsDoubleZero(ref.celula->ValorProprio)) continue;
      //determina posicionamento da c�lula de refer�ncia no grid
      if (!  celulas->IndicesDaCelula(ref.celula, ref.linha, ref.coluna)) continue;
      //distribui seu valor na sua �rea de influ�ncia
      for (int j = 0; j < lisCEL->Count; j++)
         {
         viz.celula = (VTCelula*)lisCEL->Items[j];
         //determina posicionamento da c�lula vizinha no grid
         if (!  celulas->IndicesDaCelula(viz.celula, viz.linha, viz.coluna)) continue;
         //determina dist�ncia entre c�lula de refer�ncia e c�lula vizinha
         dist    = Max(abs(ref.linha - viz.linha), abs(ref.coluna - viz.coluna));
         //fatmult = (dist == 0) ? 1. : (1. / (dist * 8));
//Testes
fatmult = (dist == 0) ? 1. : (1. / (dist + 1));
         //totaliza contribui��o da c�lula de refer�ncia na c�lula vizinha
         viz.celula->ValorTotal = viz.celula->ValorTotal + (ref.celula->ValorProprio * fatmult);
         }
      }
   }

//---------------------------------------------------------------------------
double __fastcall TTemaEnergia::ConsumoEnergia(VTCelula *celula)
   {
   //vari�veis locais
   double    energia = 0;
   VTCarga   *carga;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //obt�m lista de Cargas da celula
   lisEQP->Clear();
   celula->LisEqpto(lisEQP, eqptoCARGA);
   //totaliza energia das Cargas
   for (int n = 0; n < lisEQP->Count; n++)
      {
      carga    = (VTCarga*)lisEQP->Items[n];
      energia += demanda->Edia_mwh[carga];
      }
   return(energia);
   }

//---------------------------------------------------------------------------
void __fastcall TTemaEnergia::DefineCorCelula(void)
   {
   //vari�veis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      //define cor da c�lula em fun��o do seu valor
      celula->Color = Degrade(celula->ValorTotal);
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TTemaEnergia::Degrade(double valor)
   {
   //vari�veis locais
   VTCfgCor *cor = (VTCfgCor*)apl->GetObject(__classid(VTCfgCor));
   TColor color = cor->ENERGIA.corPadrao;

   unsigned r = ((color & 0xff)) * valor;
   unsigned g = ((color & 0xff00) >> 8) * valor;
   unsigned b = ((color & 0xff0000) >> 16) * valor;
   unsigned temp;

   /*
   //testa cor para determinar qual o tom dominante
   temp = r;
   if (r < g)
      temp = g;
   if (r < b)
      temp = b;
   if (g < b)
      temp = b;

   if (temp == r)
      {
      //r = 0xff * valor;
      g = 0xff * valor;
      b = 0xff * valor;
      }
   else if (temp == g)
      {
      r = 0xff * valor;
      //g = 0xff * valor;
      b = 0xff * valor;
      }
   else
      {
      r = 0xff * valor;
      g = 0xff * valor;
      //b = 0xff * valor;
      }

   /*
   unsigned r = 0xff;
   unsigned g = 0xff * valor;
   unsigned b = 0x00;
   */

   //retorna tom de cor gerado
   return(TColor(r + (g << 8) + (b << 16)));
   }

//---------------------------------------------------------------------------
TColor __fastcall TTemaEnergia::DegradeCinza(double valor)
   {
   //vari�veis locais
   unsigned valor_hexa = 0xff * valor;

   //retorna tom de cinza
   return(TColor(valor_hexa + (valor_hexa << 8) + (valor_hexa << 16)));
   }

//---------------------------------------------------------------------------
void __fastcall  TTemaEnergia::NormalizaValorTotalCelula(void)
   {
   //vari�veis locais
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
   //prote��o
   if (IsDoubleZero(dividendo)) return;
   //calcula valor normalizado das celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      celula->ValorTotal = (celula->ValorTotal - valor_min) / dividendo;
      }
   }

//---------------------------------------------------------------------------
//eof

