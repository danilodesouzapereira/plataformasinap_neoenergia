//---------------------------------------------------------------------------
#include <vcl.h>
#include <complex>
#pragma hdrstop
#include "TMontaZ.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTPtoFixacao.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTTramo.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
using namespace std;

//---------------------------------------------------------------------------
VTMontaZ* __fastcall NewObjMontaZ(VTApl *apl)
   {
   try{//cria objeto
      return(new TMontaZ(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMontaZ::TMontaZ(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaZ::~TMontaZ(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::Executa(TList *lisLIG)
   {
   //vaiáveis locais
   VTEqpto *eqpto;

   //loop p/ todas Ligacoes em lisLIG
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      eqpto = (VTEqpto*)lisLIG->Items[nl];
      switch(eqpto->Tipo())
         {
         case eqptoTRAFO:  Executa((VTTrafo*)eqpto);  break;
         case eqptoTRAMO:  Executa((VTTramo*)eqpto) ; break;
         case eqptoTRECHO: Executa((VTTrecho*)eqpto); break;
         default:          break;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::Executa(VTArranjo *arranjo)
   {
   //variáveis locais
   double c0, c1;
   VTCabo *cabo;

   //define MatZ do Arranjo como não montada
   arranjo->MatZ->Montada = false;
   arranjo->MatC->Montada = false;
   //verifica o tipo de configuração do Arranjo
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE:
         if (! MontaMatZ(arranjo)) return(false);
         c0 = arranjo->MaiorCapacitancia(0);
         c1 = arranjo->MaiorCapacitancia(1);
         if (! MontaMatC(arranjo->MatC, c0, c1)) return(false);
         break;
      case arranjoCABO_Z0Z1:
         cabo = arranjo->caboZ0Z1;
         if ((cabo->TipoCfg != caboZ0Z1)&&(cabo->TipoCfg != caboPRE_REUNIDO)) return(false);
         MontaMatZ(arranjo->MatZ, cabo->z0.r, cabo->z0.x, cabo->z1.r, cabo->z1.x);
         if (! MontaMatC(arranjo->MatC, cabo->c0, cabo->c1)) return(false);
         break;
      case arranjoZ0Z1:
      case arranjoPU_SB100:
         MontaMatZ(arranjo->MatZ, arranjo->z0.r, arranjo->z0.x, arranjo->z1.r, arranjo->z1.x);
         if (! MontaMatC(arranjo->MatC, arranjo->c0, arranjo->c1)) return(false);
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
         break;
      default:
         return(false);
      }
   //define MatZ do Arranjo como montada
   arranjo->MatZ->Montada = true;
   arranjo->MatC->Montada = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::Executa(VTTrafo *trafo)
   {
   //variáveis locais
   VTMatZ *MatZ = trafo->MatZ;

   //proteção: Trafo de redução não possui MatZ
   if (MatZ == NULL) return(true);
   //define MatZ do Trafo como não montada
   MatZ->Montada = false;
   MontaMatZ(MatZ, trafo->z0.r, trafo->z0.x, trafo->z1.r, trafo->z1.x);
   //define MatZ do Trafo como montada
   MatZ->Montada = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::Executa(VTTramo *tramo)
   {
   //variáveis locais
   VTTrecho *trecho;
   VTMatZ   *MatZ   = tramo->MatZ;
   TList    *lisTRE = tramo->LisTrecho();

   //reinicia MatZ do Tramo
   MatZ->Montada = false;
   MatZ->Zera();
   //monta MatZ de Trechos do Tramo
   for (int ntre = 0; ntre < lisTRE->Count; ntre++)
      {
      trecho = (VTTrecho*)lisTRE->Items[ntre];
      //monta MatZ do Trecho
      if (! Executa(trecho)) return(false);
      //atualiza MatZ do Tramo c/ valores em ohm (e não em ohm/km)
		for (int i = 0; i < MatZ->Dimensao; i++)
         {
         for (int j = 0; j < MatZ->Dimensao; j++)
            {
            MatZ->Z[i][j] = MatZ->Z[i][j] + trecho->MatZ_ohm[i][j];
            //Tramo não tem MatC
            }
         }
      }
   //define MatZ do Tramo como montada
   MatZ->Montada = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::Executa(VTTrecho *trecho)
   {
   Executa(trecho->arranjo);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::MontaMatC(VTMatC *matC, double c0, double c1)
   {
   //variáveis locais
   double cdiag, cfora;

   cdiag = (c0 + 2.*c1) / 3.;
   cfora = (c0 -    c1) / 3.;
   for(int i=0; i < (MAX_FASE-1); i++)
      {
      for(int j=0; j < (MAX_FASE-1); j++)
         {
         if(i == j) matC->Cap[i][j] = cdiag;
         else       matC->Cap[i][j] = cfora;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaZ::MontaMatZ(VTArranjo *arranjo)
   {
   // Variáveis locais
   VTCabo          *cabo,        *cabo_default;
   VTPtoFixacao    *pto_fixacao, *pto_default;
   VTMatZ          *MatZ;
   int             ind_lin, ind_col;
   double          xi, xk, yi, yk, xaux, delta_r, delta_x, val_a, val_a2,
                   val_a3, val_a4, somaq, dxylin, dx, dy, dxy, dyklin, fi;
   double          rester;
   double          res_ohm_km[MAX_FASE], gmr_m[MAX_FASE];
   double          const_a = 0.0217656, const_w = 0.15079645;
   double          abcissa[]  = {0.22, 0.92, 1.655, 0.};
   double          ordenada[] = {10. , 10. , 10.,   6.};
   double          carb[] = {0.235702, 0.062500, 0.015713, 0.002604},
                   carc[] = {0.0     , 1.365931, 0.533333, 1.782598},
                   card[] = {0.185120, 0.049087, 0.012341, 0.002045};
   complex<double> z[MAX_FASE][MAX_FASE];
   // complex<double> z0, z1, zpropria, zmutua, zaux[MAX_FASE-1][MAX_FASE-1];

   // Determina Cabo e PtoFixacao a ser utilizado p/ as eventuais fases sem cabo
   pto_default  = NULL;
   cabo_default = NULL;
   for(int nf=0; nf < MAX_FASE; nf++)
      {
      if (arranjo->fase[nf].cabo == NULL) continue;
      //encontrou uma fase c/ Cabo definido
      pto_default  = arranjo->fase[nf].pto_fixacao;
      cabo_default = arranjo->fase[nf].cabo;
      break;
      }
   // Proteção
   if((pto_default == NULL) || (cabo_default == NULL)) return(false);

   // Inicia estrutura dos cabos da cruzeta
   for(int nf=0; nf < MAX_FASE; nf++)
      {
      pto_fixacao = arranjo->fase[nf].pto_fixacao;
      if(pto_fixacao == NULL) pto_fixacao = pto_default;
      abcissa[nf]  = pto_fixacao->coord.x;
      ordenada[nf] = pto_fixacao->coord.y;
      //proteção (JCG 2014.10.10)
      if (ordenada[nf] <= 0.) ordenada[nf] = 1.;
      }

   // Inicia vetores de R e GMR dos cabos
   for(int nf=0; nf < MAX_FASE; nf++)
      {
      cabo = arranjo->fase[nf].cabo;
      if(cabo == NULL) cabo = cabo_default;
      res_ohm_km[nf] = cabo->r;    // Valor em [ohm / km]
      gmr_m[nf]      = cabo->gmr;  // Valor em [m]
      //proteção (JCG 2014.10.10)
      if (gmr_m[nf] <= 0.) gmr_m[nf] = 0.01;
      }

   // Calcula matriz Zrede - RESISTIVIDADE DA TERRA IGUAL A ZERO
   // Loop de linhas
   for(int i=0; i < MAX_FASE; i++)
      {
      xi      = abcissa[i];
      yi      = ordenada[i];
      delta_r = res_ohm_km[i];
      delta_x = 0.5 * const_w * log(2. * yi / gmr_m[i]);
      z[i][i] = complex<double>(delta_r, delta_x);

       // Loop de colunas
       for(int k = i+1; k < MAX_FASE; k++)
         {
         xk     = abcissa[k];
         yk     = ordenada[k];
         dx     = xi - xk;
         dy     = yi - yk;
         dxy    = sqrt(dx*dx + dy*dy);
         dyklin = yi + yk;
         somaq  = dx*dx + dyklin*dyklin;
         dxylin = sqrt(somaq);
         // Proteções
         if(dxy    <= 0.) dxy    = 1.;
         if(dxylin <= 0.) dxylin = 1.;
         delta_x = 0.5 * const_w * log(dxylin / dxy);
         z[i][k] = z[k][i] = complex<double>(0., delta_x);
         }
      }

   // Corrige matriz Zrede se a resistividade da terra não for zero
   rester = arranjo->ResTerra;
   if(! IsDoubleZero(rester))
      {
      const_a /= sqrt(rester);
      // Loop de linhas
      for(int i=0; i < MAX_FASE; i++)
         {
         xi     = abcissa[i];
         yi     = ordenada[i];
         val_a  = const_a * 2. * yi;
         val_a2 = val_a   * val_a;
         val_a3 = val_a2  * val_a;
         val_a4 = val_a3  * val_a;
         // z(i,i) - correção da resistência
         delta_r = const_w * (0.392699
                 - carb[0] * val_a
                 + carb[1] * val_a2 * (carc[1] - log(val_a))
                 + carb[2] * val_a3
                 - card[3] * val_a4);
         // z(i,i) - correção da reatância
         delta_x = const_w * (0.30796575 - (0.5 * log(val_a))
                 + carb[0] * val_a
                 - card[1] * val_a2
                 + carb[2] * val_a3
                 - carb[3] * val_a4 * (carc[3] - log(val_a)));
         // Atualiza z[i][i]
         delta_r += z[i][i].real();
         delta_x += z[i][i].imag();
         z[i][i]  = complex<double>(delta_r, delta_x);

         // Loop de colunas
         for(int k = i+1; k < MAX_FASE; k++)
            {
            xk     = abcissa[k];
            yk     = ordenada[k];
            dx     = xi - xk;
            dyklin = yi + yk;
            somaq  = dx*dx + dyklin*dyklin;
            dxylin = sqrt(somaq);
            // Proteção
            if(dxylin <= 0.) dxylin = 1.;
            fi     = acos(dyklin / dxylin);
            val_a  = const_a * dxylin;
            val_a2 = val_a   * val_a;
            val_a3 = val_a2  * val_a;
            val_a4 = val_a3  * val_a;
            // z(i,k) - correção da resistência
            delta_r = const_w * (0.392699
                    - carb[0] * val_a * cos(fi)
                    + carb[1] * val_a2 * ((carc[1] - log(val_a))* cos(2.*fi) +
                                           fi * sin(2.*fi))
                    + carb[2] * val_a3 * cos(3.*fi)
                    - card[3] * val_a4 * cos(4.*fi));
            // z(i,k) - correção da reatância
            delta_x = const_w * (0.30796575 - (0.5 * log(val_a))
                    + carb[0] * val_a  * cos(fi)
                    - card[1] * val_a2 * cos(2.*fi)
                    + carb[2] * val_a3 * cos(3.*fi)
                    - carb[3] * val_a4 * ((carc[3] - log(val_a)) * cos(4.*fi)
                    + fi * sin (4.*fi)));
            // Atualiza z[i][k] e z[k][i]
            delta_r += z[i][k].real();
            delta_x += z[i][k].imag();
            z[i][k]  = z[k][i] = complex<double>(delta_r, delta_x);
            } // for(k)
         } // for(i)
      } // if()

   /*
   // Alteração HPS 2012.08.14 - INÍCIO
   // Esta parte foi para TArranjo...
   // Elimina o neutro (multi aterrado)
   for(int i=0; i < (MAX_FASE-1); i++)
      {
      zaux[i][i] = z[i][i] - (z[i][3] * z[i][3]) / z[3][3];
      for(int k=(i+1); k < (MAX_FASE-1); k++)
         {
         zaux[i][k] = zaux[k][i] = z[i][k] - (z[k][3] * z[i][3]) / z[3][3];
         }
      }

   // Cálculo das impedâncias de seqüências direta e zero, realizando transposição
   zpropria = (zaux[0][0] + zaux[1][1] + zaux[2][2]) / 3.;
   zmutua   = (zaux[0][1] + zaux[0][2] + zaux[1][2]) / 3.;
   z1       = zpropria - zmutua;
   z0       = zpropria + 2.0 * zmutua;

   // Atualiza impedâncias de seq. do Arranjo
   arranjo->z1.r = z1.real();
   arranjo->z1.x = z1.imag();
   arranjo->z0.r = z0.real();
   arranjo->z0.x = z0.imag();
   // Alteração HPS 2012.08.14 - FIM
   */

   // Atualiza MatZ do Arranjo
   MatZ = arranjo->MatZ;
   ind_lin = 0;
   for(int i=0; i < MAX_FASE; i++)
      {
      if(arranjo->fase[i].cabo == NULL) continue;
      ind_col = 0;
      for(int j=0; j < MAX_FASE; j++)
         {
         if(arranjo->fase[j].cabo == NULL) continue;
         MatZ->Z[ind_lin][ind_col] = z[i][j];
         // Incrementa ind_col
         ind_col++;
         }
      // Incrementa ind_lin
      ind_lin++;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaZ::MontaMatZ(VTMatZ *MatZ, double r0, double x0, double r1, double x1)
   {
   //variáveis locais
   complex<double> z0, z1, zp, zm;

   z0 = complex<double>(r0, x0);
   z1 = complex<double>(r1, x1);
   zp = (z1 + z1 + z0)/3.;
   zm = (z0 - z1)/3.;
   for (int i = 0; i < 3; i++)
      {
      MatZ->Z[i][i] = zp;
		MatZ->Z[i][3] = CZero();
		MatZ->Z[3][i] = CZero();
      for (int j = i+1; j < 3; j++)
         {
         MatZ->Z[i][j] = zm;
         MatZ->Z[j][i] = zm;
         }
      }
	MatZ->Z[3][3] = CZero();
   }

//---------------------------------------------------------------------------
//eof
