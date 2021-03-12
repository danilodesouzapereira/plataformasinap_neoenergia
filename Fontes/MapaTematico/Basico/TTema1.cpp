//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTema1.h"
#include "..\Apl\VTApl.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTema1::TTema1(VTApl *apl) : TTema(apl)
   {
   //define código do tema
   Codigo = "Degrade: white->preto";
   }

//---------------------------------------------------------------------------
__fastcall TTema1::~TTema1(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TTema1::CalculaValorProprioCelula(void)
   {
   //variáveis locais
   int       index, num_celula;
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //define valor próprio apenas p/ célula central
   num_celula = celulas->NumCelula;
   index      = (num_celula  + 1) * (num_celula / 2);
   for (int i = 0; i < lisCEL->Count; i++)
      {
      celula = (VTCelula*)lisCEL->Items[i];
      celula->ValorProprio = (i == index) ? 100 : 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTema1::CalculaValorTotalCelula(void)
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
void __fastcall TTema1::DefineCorCelula(void)
   {
   //variáveis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      //define cor da célula em função do seu valor
      //celula->Color = Degrade(celula->ValorTotal);
      celula->Color = DegradeCinza(celula->ValorTotal);
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TTema1::Degrade(double valor)
	{
	//variáveis locais
	unsigned r = 0xff;
	unsigned g = 0xff * valor;
	unsigned b = 0x00;

	//retorna tom de cinza
	return(TColor(r + (g << 8) + (b << 16)));
	}

//---------------------------------------------------------------------------
TColor __fastcall TTema1::DegradeCinza(double valor)
	{
	//variáveis locais
	unsigned valor_hexa = 0xff * valor;

	//retorna tom de cinza
	return(TColor(valor_hexa + (valor_hexa << 8) + (valor_hexa << 16)));
	}

//---------------------------------------------------------------------------
void __fastcall  TTema1::NormalizaValorTotalCelula(void)
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
//eof

