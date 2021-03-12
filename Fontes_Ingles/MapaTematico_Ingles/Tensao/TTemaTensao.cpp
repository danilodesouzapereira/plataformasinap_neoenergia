//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTemaTensao.h"
#include "VTCfgCor.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTemaTensao::TTemaTensao(VTApl *apl) : TTema(apl)
   {
   //define c�digo do tema
   Codigo = "Tens�o M�nima";
   //cria objetos
   lisEQP = new TList();
   //inicia estruturas de dados com valores limites
   //IniciaEstruturas();
   }

//---------------------------------------------------------------------------
__fastcall TTemaTensao::~TTemaTensao(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TTemaTensao::CalculaValorProprioCelula(void)
   {
   //vari�veis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //define tens�o m�nima em cada celula
   for (int i = 0; i < lisCEL->Count; i++)
      {
      celula = (VTCelula*)lisCEL->Items[i];
      celula->ValorProprio = TensaoMinima(celula);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTemaTensao::CalculaValorTotalCelula(void)
   {
   //vari�veis locais
   int     dist;
   int     dist_max = 5;
   double  fatpond;
   struct   {
            VTCelula  *celula;
            }ref, viz;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //zera valor total e n�mero de parcelas de todas as c�lulas
   for (int i = 0; i < lisCEL->Count; i++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[i];
      ref.celula->ValorTotal = 0;
      ref.celula->NumParcela = 0;
      }
   //verifica quais c�lulas possuem valor pr�prio
   //valor total = valor pr�prio
   for (int nc = 0; nc < lisCEL->Count; nc++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[nc];
      if (IsDoubleZero(ref.celula->ValorProprio)) continue;
      ref.celula->ValorTotal = ref.celula->ValorProprio;
      ref.celula->NumParcela = 1;
      }
   //loop p/ todas as c�lulas
   for (int nc = 0; nc < lisCEL->Count; nc++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[nc];
      //verifica se a c�lula de refer�ncia tem valor pr�prio
      if (IsDoubleZero(ref.celula->ValorProprio)) continue;
      //determina lista de Celulas vizinhas
      lisEQP->Clear();
      celulas->LisCelulaVizinha(lisEQP, ref.celula, dist_max);
      for (int i = 0; i < lisEQP->Count; i++)
         {
         viz.celula = (VTCelula*)lisEQP->Items[i];
         //verifica se a c�lula vizinha n�o tem valor pr�prio
         if (! IsDoubleZero(viz.celula->ValorProprio)) continue;
         //determinha dist�ncia entre c�lula de refer�ncia e c�lula vizinha
         dist = celulas->Distancia(ref.celula, viz.celula);
         //determina fator de pondera��o
         if ((fatpond = dist_max - dist) <= 0) continue;;
         //totaliza contribui��o da c�lula de refer�ncia na c�lula vizinha
         viz.celula->ValorTotal = viz.celula->ValorTotal + (fatpond * ref.celula->ValorProprio);
         //totaliza n�mero de parcelas
         viz.celula->NumParcela = viz.celula->NumParcela + fatpond;
         }
      }
   //determina o valor total de cada c�lula como uma m�dia ponderada
   for (int i = 0; i < lisCEL->Count; i++)
      {
      ref.celula = (VTCelula*)lisCEL->Items[i];
      //atualiza valor total
      if (ref.celula->NumParcela > 0)
         {
         ref.celula->ValorTotal = ref.celula->ValorTotal / ref.celula->NumParcela;
         }
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TTemaTensao::CorClara(TColor color)
   {
   return(Degrade(color, clWhite, 0.75));
   }

//---------------------------------------------------------------------------
TColor __fastcall TTemaTensao::CorEscura(TColor color)
   {
   return(Degrade(color, clBlack, 0.25));
   }

//---------------------------------------------------------------------------
void __fastcall TTemaTensao::DefineCorCelula(void)
   {
   //vari�veis locais
   double     percent;
   VTCelula   *celula;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTCelulas  *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList      *lisCEL  = celulas->LisCelula();

   //inicia faixas de tens�o/cor
   IniciaEstruturas();
   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      //verifica se a celula possui um valor definido
      if (celula->NumParcela == 0)
         {
         celula->Color = redegraf->CorFundo;
         continue;
         }
      //identifica faixa que cont�m o valor total da Celula
      for (int nf = 0; nf < NUM_FAIXA; nf++)
         {
         if (celula->ValorTotal < faixa[nf].valor_superior)
            {
            percent = Porcentagem(faixa[nf].valor_inferior, faixa[nf].valor_superior, celula->ValorTotal);
            celula->Color = Degrade(faixa[nf].cor_inferior, faixa[nf].cor_superior, percent);
            break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TTemaTensao::Degrade(TColor beginColor, TColor endColor, double percent)
   {
   //decomp�e beginColor
   unsigned r1 = ((beginColor & 0xff));
   unsigned g1 = ((beginColor & 0xff00) >> 8);
   unsigned b1 = ((beginColor & 0xff0000) >> 16);
   //decomp�e endColor
   unsigned r2 = ((endColor & 0xff));
   unsigned g2 = ((endColor & 0xff00) >> 8);
   unsigned b2 = ((endColor & 0xff0000) >> 16);
   //mistura componentes de beginColor com endColor
   unsigned r = ((1 - percent) * r1) + (percent * r2);
   unsigned g = ((1 - percent) * g1) + (percent * g2);
   unsigned b = ((1 - percent) * b1) + (percent * b2);
   //retorna cor
   return(TColor(r + (g << 8) + (b << 16)));
   }

//---------------------------------------------------------------------------
void __fastcall TTemaTensao::IniciaEstruturas(void)
   {
   //vari�veis locais
   double    limite[NUM_FAIXA+1];
   TColor    color[NUM_FAIXA+1];
   VTGeral  *geral    = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTCfgCor *cfg_cor  = (VTCfgCor*)apl->GetObject(__classid(VTCfgCor));

//   //valores limites das faixas
//   limite[0] = 0;
//   limite[1] = 0.8 * geral->FLOW.V[0];
//   limite[2] = geral->FLOW.V[0];
//   limite[3] = geral->FLOW.V[1];
//   limite[4] = (geral->FLOW.V[0] + geral->FLOW.V[1]) / 2.;
//   limite[5] = geral->FLOW.V[2];
//   limite[6] = geral->FLOW.V[3];
//   limite[7] = 1.2 * geral->FLOW.V[3];
//   limite[8] = 5;
//	//cores das faixas             aqueles
//	color[0] = clWhite;
//	color[1] = cfg_cor->TENSAO.corV1;
//	color[2] = cfg_cor->TENSAO.corV1;
//	color[3] = cfg_cor->TENSAO.corV2;
//	color[4] = cfg_cor->TENSAO.corV3;
//	color[5] = cfg_cor->TENSAO.corV4;
//	color[6] = cfg_cor->TENSAO.corV5;
//	color[7] = cfg_cor->TENSAO.corV5;
//	color[8] = clWhite;

//FKM - 2015.09.14 as cores n�o saiam direito, saiam degrade entre limites que deviam ter mesma cor
   //valores limites das faixas
   limite[0] = 0;
   limite[1] = 0.8 * geral->FLOW.V[0];
   limite[2] = geral->FLOW.V[0];
   limite[3] = (geral->FLOW.V[0] + geral->FLOW.V[1]) / 2.;  //inverti com o proximo
   limite[4] = geral->FLOW.V[1];
   limite[5] = geral->FLOW.V[2];
   limite[6] = geral->FLOW.V[3];
   limite[7] = 1.2 * geral->FLOW.V[3];
   limite[8] = 5;
	//cores das faixas             aqueles
	color[0] = clWhite;
	color[1] = cfg_cor->TENSAO.corV1;
	color[2] = cfg_cor->TENSAO.corV1;
	color[3] = cfg_cor->TENSAO.corV2;
	color[4] = cfg_cor->TENSAO.corV2;
	color[5] = cfg_cor->TENSAO.corV3;
	color[6] = cfg_cor->TENSAO.corV4;
	color[7] = cfg_cor->TENSAO.corV5;
	color[8] = clWhite;
//FKM - 2015.09.14
   //define faixas
   for (int n = 0; n < NUM_FAIXA; n++)
      {
      faixa[n].valor_inferior = limite[n];
      faixa[n].valor_superior = limite[n+1];
      faixa[n].cor_inferior   = color[n];
      faixa[n].cor_superior   = color[n+1];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTemaTensao::NormalizaValorTotalCelula(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
float  __fastcall TTemaTensao::Porcentagem(double valor_inferior, double valor_superior, double valor)
   {
   //vari�veis locais
   double percent;

   //determina posi��o do valor dentro da faixa
   if (valor < 0)
      {
      percent = 0;
      return (percent);
      }
   else if (valor > 2)
      {
      percent = 1;
      return (percent);
      }
   else if (IsDoubleZero(valor_inferior))
      percent = valor / valor_superior;
   else if (IsDoubleZero(valor_superior))
      percent = valor / valor_inferior;
   else
      percent = ((valor - valor_inferior) / (valor_superior - valor_inferior));
    //FKM 2015.09.14 - As cores precisam ser discretas nesse modulo
    //if(percent != 0.){percent = 1.;}
    //FKM 2015.09.14
   return (percent);
   }

//---------------------------------------------------------------------------
double __fastcall TTemaTensao::TensaoMinima(VTCelula *celula)
   {
   //vari�veis locais
   double tensao_min;
   double vfn_pu_min;
   VTBarra *barra;

   //obt�m lista de Cargas da celula
   lisEQP->Clear();
   celula->LisEqpto(lisEQP, eqptoBARRA);
   //se a Celula n�o possui barras, assume tens�o de 0 pu
   if (lisEQP->Count == 0) return(0);
   //determina menor tens�o das barras
   for (int n = 0; n < lisEQP->Count; n++)
      {
      barra = (VTBarra*)lisEQP->Items[n];
      if (ind_pat < 0)
         {//menor tens�o entre todos patamares
         vfn_pu_min = barra->resflow->Vfn_pu_min_allpat();
         }
      else
         {//menor tens�o no patamar
         vfn_pu_min = barra->resflow->Vfn_pu_min[ind_pat];
         }
      //verifica se primeira Barra
      if      (n == 0)                  {tensao_min = vfn_pu_min;}
      else if (vfn_pu_min < tensao_min) {tensao_min = vfn_pu_min;}
      }
   return(tensao_min);
   }

//---------------------------------------------------------------------------
//eof

