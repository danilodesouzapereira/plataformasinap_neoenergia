//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <algorithm>
#include "TArranjo.h"
#include "VTCabo.h"
#include "VTSuporte.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTMatZ.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTArranjo
//---------------------------------------------------------------------------
VTArranjo* __fastcall NewObjArranjo(VTEqpto *owner)
   {
   return(new TArranjo(owner));
   }

//---------------------------------------------------------------------------
__fastcall TArranjo::TArranjo(VTEqpto *owner)
   {
   //cria objetos
   matC = DLL_NewObjMatC();
   matZ = DLL_NewObjMatZ();
   //inicia dados
   Id        = -1;
   Codigo    = "";
   Extern_id = "";
   Owner     = owner;
	TipoCfg   = arranjoZ0Z1;
	ramal     = false;
   ResTerra = 100.;
   //Data     = TDate(2000, 1,1);
   Data     = DateOf(Now());
   //inicia atributos p/ visualização gráfica
   Show[eqp_VISIBLE] = true;
   Show[eqp_CODIGO]  = true;
   Show[arr_CFG]     = true;
   }

//---------------------------------------------------------------------------
__fastcall TArranjo::~TArranjo(void)
   {
   //destrói objetos
   if (matC) {delete matC; matC = NULL;}
   if (matZ) {delete matZ; matZ = NULL;}
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::Calcula_C0(double c3x3[3][3])
   {
   double cmutua, cpropria, c0x;

   // Considera transposição das fases
   cpropria = c3x3[0][0] + c3x3[1][1] + c3x3[2][2];
   cmutua   = c3x3[0][1] + c3x3[0][2] + c3x3[1][2];
   c0x      = (cpropria + (2. * cmutua)) / 3.;  // Economiza uma divisão...

   return(c0x);
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::Calcula_C1(double c3x3[][MAX_FASE-1])
   {
   double cmutua, cpropria, c1x;

   // Considera transposição das fases
   cpropria = c3x3[0][0] + c3x3[1][1] + c3x3[2][2];
   cmutua   = c3x3[0][1] + c3x3[0][2] + c3x3[1][2];
   c1x      = (cpropria - cmutua) / 3.;  // Economiza uma divisão...

   return(c1x);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TArranjo::Calcula_Z0(complex<double> z3x3[3][3])
   {
   complex<double> zmutua, zpropria, z0x;

   // Considera transposição das fases
   zpropria = z3x3[0][0] + z3x3[1][1] + z3x3[2][2];
   zmutua   = z3x3[0][1] + z3x3[0][2] + z3x3[1][2];
   z0x      = (zpropria + (2. * zmutua)) / 3.;  // Economiza uma divisão...

   return(z0x);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TArranjo::Calcula_Z1(complex<double> z3x3[][MAX_FASE-1])
   {
   complex<double> zmutua, zpropria, z1x;

   // Considera transposição das fases
   zpropria = z3x3[0][0] + z3x3[1][1] + z3x3[2][2];
   zmutua   = z3x3[0][1] + z3x3[0][2] + z3x3[1][2];
   z1x      = (zpropria - zmutua) / 3.;  // Economiza uma divisão...

   return(z1x);
   }
/*
//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjo::Clone(void)
	{
	//variáveis locais
	VTArranjo *arranjo;

	//cria uma nova Barra
	if ((arranjo = NewObjArranjo(Owner)) != NULL)
		{//copia dados deste Arranjo p/ o novo Arranjo
		*arranjo << *this;
		}
	return(arranjo);
	}
*/
//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjo::Clone(void)
	{
	//variáveis locais
	TArranjo *arranjo;

	//cria um novo Arranjo
	if ((arranjo = new TArranjo(Owner)) != NULL)
		{//copia dados deste Arranjo p/ o novo Arranjo
		arranjo->CopiaAtributosDe(*this);
		}
	return(arranjo);
	}

//---------------------------------------------------------------------------
void __fastcall TArranjo::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	TArranjo &arranjo = (TArranjo&)ref;

	//copia atributos de VTEqpto
	VTEqpto::CopiaAtributosDe(arranjo);
	//copia atributos do Arranjo
	PD           = arranjo.PD;
	ramal        = arranjo.ramal;
	suporte      = arranjo.suporte;
	caboZ0Z1     = arranjo.caboZ0Z1;
	z0           = arranjo.z0;
	c0           = arranjo.c0;
	z1           = arranjo.z1;
	c1           = arranjo.c1;
	//copia MatC
	MatC->Dimensao = arranjo.MatC->Dimensao;
	for (int i = 0; i < MatC->Dimensao; i++)
		{
		for (int j = 0; j < MatC->Dimensao; j++)
			{
			MatC->Cap[i][j] = arranjo.MatC->Cap[i][j];
			}
		}
	//copia MatZ
   MatZ->Dimensao = arranjo.MatZ->Dimensao;
	for (int i = 0; i < MatZ->Dimensao; i++)
		{
		for (int j = 0; j < MatZ->Dimensao; j++)
			{
			MatZ->Z[i][j] = arranjo.MatZ->Z[i][j];
         }
		}
   //copia configuração das fases
   for (int nf = 0; nf < MAX_FASE; nf++)
		{
      fase[nf] = arranjo.fase[nf];
      }
	}

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefCaboFase(int ind_fase, VTCabo *cabo, int multiplicidade)
	{
   //proteção
   if ((ind_fase < indFASE_A)||(ind_fase > indNEUTRO)) return;
   //redefine Cabo da fase
   fase[ind_fase].cabo           = cabo;
   fase[ind_fase].multiplicidade = multiplicidade;
   //redefine tipo de configuração do Arranjo
   TipoCfg = arranjoCABOxFASE;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefCaboZ0Z1(VTCabo *cabo)
   {
   //salva ponteiro p/ Cabo pre-reunido
   caboZ0Z1 = cabo;
   //redefine tipo de configuração do Arranjo
   TipoCfg = arranjoCABO_Z0Z1;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefC0C1_nF_km(double c0, double c1)
   {
   //salva valores
   this->c0 = c0;
   this->c1 = c1;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefC0C1_pu(double Sbase, double Vbase, double comp, double yc0_pu, double yc1_pu)
   {
   //variáveis locais
   double yc0_mho, yc1_mho;
   double freq = 60.; //hertz
   double w    =  2. * M_PI * freq;
   double Zbase = (Vbase * Vbase) / Sbase;

   //calcula admitâncias em mho/km
   yc0_mho = (yc0_pu / Zbase) / comp;
   yc1_mho = (yc1_pu / Zbase) / comp;
   //calcula capacitâncias em nf/km
   c0 = (yc0_mho / w) * 1e9;
   c1 = (yc1_mho / w) * 1e9;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefPtoFixacaoFase(int ind_fase, VTPtoFixacao *pto_fixacao)
   {
   //proteção
   if ((ind_fase < indFASE_A)||(ind_fase > indNEUTRO)) return;
   //redefine PtoFixacao da fase
   fase[ind_fase].pto_fixacao = pto_fixacao;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefSuporte(VTSuporte *suporte)
   {
   this->suporte = suporte;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefZ0Z1_ohm_km(strIMP z0, strIMP z1)
   {
   //salva valores
   this->z0 = z0;
   this->z1 = z1;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::DefZ0Z1_pu(double Sbase, double Vbase, double comp, strIMP z0_pu, strIMP z1_pu)
   {
   //variáveis locais
   strIMP z0_ohm, z1_ohm;
   double Zbase = (Vbase * Vbase) / Sbase;

   //calcula Z0 em ohm
   z0_ohm.r = z0_pu.r * Zbase;
   z0_ohm.x = z0_pu.x * Zbase;
   //salva Z0 em ohm/km
   z0.r = z0_ohm.r / comp;
   z0.x = z0_ohm.x / comp;
   //calcula Z1 em ohm
   z1_ohm.r = z1_pu.r * Zbase;
   z1_ohm.x = z1_pu.x * Zbase;
   //salva Z1 em ohm/km
   z1.r = z1_ohm.r / comp;
   z1.x = z1_ohm.x / comp;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TArranjo::EliminaNeutro(complex<double> zaux[][MAX_FASE-1])
   {
   // Variáveis locais
   int             ind_neutro;
   complex<double> cdenom, z_in;

   // Elimina neutro
   ind_neutro = MAX_FASE - 1;
   cdenom     = 1. / MatZ->Z[ind_neutro][ind_neutro];
   for(int i=0; i < ind_neutro; i++)
      {
      z_in       = MatZ->Z[i][ind_neutro];
      zaux[i][i] = MatZ->Z[i][i] - (z_in * z_in) * cdenom;
      for(int k=(i+1); k < ind_neutro; k++)
         {
         zaux[i][k] = zaux[k][i] = MatZ->Z[i][k]
                                 - (z_in * MatZ->Z[k][ind_neutro]) * cdenom;
         }
      }
   }
 */
//---------------------------------------------------------------------------
void __fastcall TArranjo::EliminaNeutro(complex<double> z4x4[4][4], complex<double> z3x3[3][3])
   {
   // Variáveis locais
   int             ind_neutro;
   complex<double> cdenom, z_in;

   // Elimina neutro
   ind_neutro = 3;
   cdenom     = 1. / z4x4[ind_neutro][ind_neutro];
   for(int i = 0; i < ind_neutro; i++)
      {
      z_in       = z4x4[i][ind_neutro];
      z3x3[i][i] = z4x4[i][i] - (z_in * z_in) * cdenom;
      for(int k=(i+1); k < ind_neutro; k++)
         {
         z3x3[i][k] = z3x3[k][i] = z4x4[i][k]
                                 - (z_in * z4x4[k][ind_neutro]) * cdenom;
         }
      }
   }
//---------------------------------------------------------------------------

double __fastcall TArranjo::MaiorCapacitancia(int seq)
   {
   // Variáveis locais
   double cap_cabo, cap_max;

   switch(TipoCfg)
      {
      case arranjoCABOxFASE:
         //Determina maior capacitância entre os cabos, na sequência fornecida
         cap_max = 0.;
         for(int ind_fase=0; ind_fase < MAX_FASE; ind_fase++)
            {
            if (fase[ind_fase].cabo == NULL) continue;
            if (seq == 0) cap_cabo = fase[ind_fase].cabo->c0;
            else          cap_cabo = fase[ind_fase].cabo->c1;
            cap_max = max(cap_max, cap_cabo);
            }
         break;
      case arranjoCABO_Z0Z1:
         if (seq == 0) cap_max = caboZ0Z1->c0;
         else          cap_max = caboZ0Z1->c1;
         break;
      case arranjoZ0Z1:
         if (seq == 0) cap_max = c0;
         else          cap_max = c1;
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
         double c3x3[3][3];
         MontaMatrizC3x3(c3x3);
         if (seq == 0) cap_max = Calcula_C0(c3x3);
         else          cap_max = Calcula_C1(c3x3);
         break;
      case arranjoPU_SB100:
         if (seq == 0) cap_max = c0;
         else          cap_max = c1;
         break;
      default:
         cap_max = 0.;
         break;
      }
   return(cap_max);
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::MontaMatrizC3x3(double c3x3[3][3])
   {
   // Variáveis locais
   int num_fase;
   struct
      {
      double cdiag, cfora;  // Matriz de capacitâncias NODAIS
      } fase;

   // Verifica se o Arranjo tem:
   //    1. as 3 fases sem o neutro
   //    2. as 3 fases e   o neutro
   if(((Fases & faseABCN) == faseABC) ||
      ((Fases & faseABCN) == faseABCN))
      {
      // Copia a matriz MatC
      for(int i = 0; i < 3; i++)
         {
         c3x3[i][i] = MatC->Cap[i][i];
         for(int k = (i+1); k < 3; k++)
            {
            c3x3[i][k] = c3x3[k][i] = MatC->Cap[i][k];
            }
         }
      return;
      }

   // Determina número de fases do Arranjo
   num_fase = 0;
   if (Fases & faseA) num_fase++;
   if (Fases & faseB) num_fase++;
   if (Fases & faseC) num_fase++;

   // Demais casos: determina valores de capacitância para montar c3x3:
   //    1)valor médio da impedância própria das fases,
   //    2)valor médio da impedência mútua entre fases,
   if(num_fase == 1)
      {
      fase.cdiag = MatC->Cap[0][0];
      fase.cfora = 0.;
      }
   else if(num_fase == 2)
      {
      fase.cdiag = (MatC->Cap[0][0] + MatC->Cap[1][1]) / 2.;
      fase.cfora =  MatC->Cap[0][1];
      }
   // Monta matriz c3x3
   for (int i = 0; i < 3; i++)
      {
      c3x3[i][i] = fase.cdiag;
      for(int k = (i+1); k < 3; k++)
         {
         c3x3[i][k] = c3x3[k][i] = fase.cfora;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::MontaMatrizZ3x3(complex<double> z3x3[3][3])
   {
   //variáveis locais
   //int ind_neutro;
   int             num_fase;
   complex<double> z4x4[4][4];
   struct   {
            complex<double> zp, zm;
            }fase, neutro;

   //verifica se o Arranjo tem as 3 fases sem o neutro
   if ((Fases & faseABCN) == faseABC)
      {//copia a submatriz 3x3 de MatZ em z3x3
      for(int i = 0; i < 3; i++)
         {
         z3x3[i][i] = MatZ->Z[i][i];
         for(int k = (i+1); k < 3; k++)
            {
            z3x3[i][k] = z3x3[k][i] = MatZ->Z[i][k];
            }
         }
      return;
      }
   //verifica se o Arranjo tem as 3 fases e o neutro
   if ((Fases & faseABCN) == faseABCN)
      {//copia MatZ em z
      for(int i = 0; i < 4; i++)
         {
         z4x4[i][i] = MatZ->Z[i][i];
         for(int k = (i+1); k < 4; k++)
            {
            z4x4[i][k] = z4x4[k][i] = MatZ->Z[i][k];
            }
         }
      //monta z3x3, eliminando neutro de z4x4
      EliminaNeutro(z4x4, z3x3);
      return;
      }
   //determina número de fases do Arranjo
   num_fase = 0;
   if (Fases & faseA) num_fase++;
   if (Fases & faseB) num_fase++;
   if (Fases & faseC) num_fase++;
   //determina valores de impedância p/ montar z4x4 ou z3x3:
   // 1)valor médio da impedância própria das fases,
   // 2)valor médio da impedência mútua entre fases,
   // 3) valor da impedância de neutro e
   // 4) valor médio da impedância mútua entre neutro e fases
   if (num_fase == 1)
      {
      fase.zp   = MatZ->Z[0][0];
      fase.zm   = CZero();
      if ((Fases & faseN) == faseN)
         {
         neutro.zp = MatZ->Z[num_fase][num_fase];
         neutro.zm = MatZ->Z[0][num_fase];
         }
      else
         {
         neutro.zp = CZero();
         neutro.zm = CZero();
         }
      }
   else if (num_fase == 2)
      {
      fase.zp = (MatZ->Z[0][0] + MatZ->Z[1][1]) / 2.;
      fase.zm =  MatZ->Z[0][1];
      //verifica se o arranjo possui neutro
      if ((Fases & faseN) == faseN)
         {
         neutro.zp = MatZ->Z[num_fase][num_fase];
         neutro.zm = (MatZ->Z[0][num_fase] + MatZ->Z[1][num_fase]) / 2.;
         }
      else
         {
         neutro.zp = CZero();
         neutro.zm = CZero();
         }
      }
   //verifica se o arranjo possui neutro
   if ((Fases & faseN) == faseN)
      {//monta linhas e colunas de z4x4 relativas às 3 fases
      for(int i = 0; i < 3; i++)
         {
         z4x4[i][i] = fase.zp;
         for(int k = (i+1); k < 3; k++)
            {
            z4x4[i][k] = z4x4[k][i] = fase.zm;
            }
         }
      //monta linhas e colunas de z4x4 relativas ao neutro
      z4x4[3][3] = neutro.zp;
      for(int k = 0; k < 3; k++)
         {
         z4x4[3][k] = z4x4[k][3] = neutro.zm;
         }
      //monta z3x3, eliminando neutro de z4x4
      EliminaNeutro(z4x4, z3x3);
      return;
      }
   //monta matriz z3x3
   for (int i = 0; i < 3; i++)
      {
      z3x3[i][i] = fase.zp;
      for(int k = (i+1); k < 3; k++)
         {
         z3x3[i][k] = z3x3[k][i] = fase.zm;
         }
      }
   }

//---------------------------------------------------------------------------
TColor __fastcall TArranjo::PM_GetColor(void)
	{
	return(PD.color);
	}

//---------------------------------------------------------------------------
TDate __fastcall TArranjo::PM_GetData(void)
	{
	return(PD.data);
	}

//---------------------------------------------------------------------------
int __fastcall TArranjo::PM_GetFases(void)
   {
   return(PD.fases);
	}

//---------------------------------------------------------------------------
double __fastcall TArranjo::PM_GetIadm_amp(void)
	{
	//variáveis locais
   bool   first = true;
   double i_amp = 0;
   int    vet_fase[] = {faseA, faseB, faseC};

   //verifica o tipo de configuração
   switch(TipoCfg)
      {
      case arranjoCABOxFASE:
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {//verifica se existe a fase
            if ((PD.fases & vet_fase[ind_fase]) == vet_fase[ind_fase])
               {//verifica se é a primeira fase com valor válido
               if (first)  {i_amp = Ifase_amp[ind_fase];}
               else        {i_amp = min(i_amp, Ifase_amp[ind_fase]);}
               first = false;
               }
            }
         break;
      case arranjoCABO_Z0Z1:
         if (caboZ0Z1) i_amp = caboZ0Z1->Iadm_A;
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
      case arranjoPU_SB100:
      case arranjoZ0Z1:
         i_amp = PD.iadm_amp;
         break;
      }
   return(i_amp);
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::PM_GetIfase_amp(int nf)
   {
   //variáveis locais
   double iadm = PD.iadm_amp;

   //verifica o tipo de configuração
   switch(TipoCfg)
      {
      case arranjoCABOxFASE:
         if (fase[nf].cabo != NULL) iadm = fase[nf].cabo->Iadm_A;
         break;
      case arranjoCABO_Z0Z1:
         if (caboZ0Z1) iadm = caboZ0Z1->Iadm_A;
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
      case arranjoPU_SB100:
      case arranjoZ0Z1:
         iadm = PD.iadm_amp;
         break;
      }
   return(iadm);
   }

//---------------------------------------------------------------------------
VTMatC* __fastcall TArranjo::PM_GetMatC(void)
   {
   return(matC);
   }

//---------------------------------------------------------------------------
VTMatZ* __fastcall TArranjo::PM_GetMatZ(void)
   {
   return(matZ);
   }

//---------------------------------------------------------------------------
int __fastcall TArranjo::PM_GetNumeroCabos(void)
   {
   //variáveis locais
   int count = 0;

   //verifica o tipo de configuração
   switch(TipoCfg)
      {
      case arranjoCABOxFASE:
         if (fase[indFASE_A].cabo != NULL) count++;
         if (fase[indFASE_B].cabo != NULL) count++;
         if (fase[indFASE_C].cabo != NULL) count++;
         if (fase[indNEUTRO].cabo != NULL) count++;
         break;
      case arranjoCABO_Z0Z1:
         count = 1;
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
      case arranjoPU_SB100:
      case arranjoZ0Z1:
         count = 0;
         break;
      default:
         count = 0;
         break;
      }
   return(count);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TArranjo::PM_GetOwner(void)
   {
   return(PD.owner);
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::PM_GetResTerra(void)
   {
   return(PD.res_terra);
   }

//---------------------------------------------------------------------------
int __fastcall TArranjo::PM_GetTipoCfg(void)
   {
   return(PD.tipo_cfg);
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::PM_GetVmax_KV(void)
   {
   //variáveis locais
   double vmax = PD.vmax_kv;

   //verifica o tipo de configuração
   switch(TipoCfg)
      {
      case arranjoCABOxFASE:
         //determina A MENOR entre as tensões máximas do Suporte e dos Cabos
         vmax = suporte->Vmax_KV;
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            if (fase[nf].cabo == NULL)     continue;
            if (fase[nf].cabo->Vmax_KV == 0.) continue;
            if (vmax == 0) vmax = fase[nf].cabo->Vmax_KV;
            if (fase[nf].cabo->Vmax_KV < vmax) vmax = fase[nf].cabo->Vmax_KV;
            }
         break;
      case arranjoCABO_Z0Z1:
         //determina A MENOR entre as tensões máximas do Suporte e do Cabo pré_reunido
         if (vmax == 0) vmax = caboZ0Z1->Vmax_KV;
         if (caboZ0Z1->Vmax_KV < vmax) vmax = caboZ0Z1->Vmax_KV;
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
      case arranjoPU_SB100:
      case arranjoZ0Z1:
         vmax = PD.vmax_kv;
         break;
      }
   return(vmax);
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::PM_GetVmin_KV(void)
   {
   //variáveis locais
   double vmin = PD.vmin_kv;

   //verifica o tipo de configuração
   switch(TipoCfg)
      {
      case arranjoCABOxFASE:
         //determina A MAIOR entre as tensões mínimas do Suporte e dos Cabos
         vmin = suporte->Vmin_KV;
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            if (fase[nf].cabo == NULL)     continue;
            if (fase[nf].cabo->Vmin_KV == 0.) continue;
            if (vmin == 0) vmin = fase[nf].cabo->Vmin_KV;
            if (fase[nf].cabo->Vmin_KV > vmin) vmin = fase[nf].cabo->Vmin_KV;
            }
         break;
      case arranjoCABO_Z0Z1:
         //determina A MAIOR entre as tensões mínimas do Suporte e do Cabo pré_reunido
         if (vmin == 0) vmin = caboZ0Z1->Vmin_KV;
         if (caboZ0Z1->Vmin_KV > vmin) vmin = caboZ0Z1->Vmin_KV;
         break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
      case arranjoPU_SB100:
      case arranjoZ0Z1:
         vmin = PD.vmin_kv;
         break;
      }
   return(vmin);
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::PM_GetYcap_siemens_km(int i, int j)
   {
   //variáveia locais
   return(Ycap_siemens_km(MatC->Cap[i][j]));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TArranjo::PM_GetZ_ohm_km(int i, int j)
   {
   return(MatZ->Z[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetCodigo(AnsiString codigo)
   {
   //proteção contra dimensão do string
   if (codigo.Length() > 128) codigo = codigo.SubString(1, 128);
   VTEqpto::PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetColor(TColor color)
   {
   PD.color = color;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetData(TDate data)
   {
   PD.data = data;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetFases(int fases)
   {
   //variáveis locais
   int dim;

   PD.fases = fases;
   //define dimensões de MatZ e Matc
   switch(fases)
      {
      case faseA:
      case faseB:
      case faseC:
         dim = 1;
         break;
      case faseAN:
      case faseBN:
      case faseCN:
      case faseAB:
      case faseBC:
      case faseCA:
         dim = 2;
         break;
      case faseABN:
      case faseBCN:
      case faseCAN:
         dim = 3;
         break;
      case faseABC:
      case faseABCN:
      default:
         dim = 4;
      }
   MatC->Dimensao = dim;
   MatZ->Dimensao = dim;
   }

//---------------------------------------------------------------------------
void  __fastcall TArranjo::PM_SetIadm_amp(double iadm)
   {
   //a definição de Iadm só é utilizada p/ arranjoZ0_Z1, arranjoMUTUA e arranjoMAT_Z,
   //ou seja, é um valor único que independe da fase
   PD.iadm_amp = iadm;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetResTerra(double res_terra)
   {
   PD.res_terra = res_terra;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetTipoCfg(int tipo_cfg)
   {
   PD.tipo_cfg = tipo_cfg;
   //se necessário, redefine fases
   switch(tipo_cfg)
      {
      case arranjoCABOxFASE: //nada a fazer
      case arranjoCABO_Z0Z1:
      case arranjoZ0Z1:
      case arranjoMAT_Z:
      case arranjoMUTUA:
         break;
      case arranjoPU_SB100:
      default:
         if ((Fases != faseABC)&&(Fases != faseABCN)) Fases = faseABCN;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetVmax_KV(double vmax)
   {
   PD.vmax_kv = vmax;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetVmin_KV(double vmin)
   {
   PD.vmin_kv = vmin;
   }

//---------------------------------------------------------------------------
void __fastcall TArranjo::PM_SetOwner(VTEqpto *owner)
   {
   PD.owner = owner;
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjo::TemCapacitancia(void)
   {
   return(! IsDoubleZero(c0 + c1));
   }

//---------------------------------------------------------------------------
int __fastcall TArranjo::Tipo(void)
   {
   return(eqptoARRANJO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TArranjo::TipoAsString(void)
   {
   return("Arranjo");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TArranjo::TipoDeArranjo(void)
   {
   if (suporte == NULL) return("-");
   return(suporte->TipoDeSuporteAsString());
   }

//---------------------------------------------------------------------------
double __fastcall TArranjo::Ycap_siemens_km(double cap_nF_km)
   {
   //variáveis locais
   double freq = 60.; // Hertz
   double w    =  2. * M_PI * freq;

   //calcula admitância: Ycap(S/km) = w * cap_nF_km * 1e-9)
   return(w * cap_nF_km * 1e-9);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjo::Z0_ohm(double comp, strIMP &z0_ohm, double &yc0_mho)
   {
   // Variáveis locais
   strIMP          z;
   float           ycap, cabo_c0;
   double          c0x, c3x3[3][3];
   complex<double> z0x, z3x3[3][3];

   // Verifica o tipo de configuração do Arranjo
   switch(TipoCfg)
      {
      case arranjoCABO_Z0Z1:
         z    = caboZ0Z1->z0;
         ycap = Ycap_siemens_km(caboZ0Z1->c0);
         break;

      case arranjoZ0Z1:
      case arranjoPU_SB100:
         z    = z0;
         ycap = Ycap_siemens_km(c0);
         break;

      case arranjoMAT_Z:
      case arranjoMUTUA:
         // Impedância: elimina neutro, se existir (o considera multi-aterrado)
         MontaMatrizZ3x3(z3x3);
         // Cálculo da impedância de sequência zero
         z0x = Calcula_Z0(z3x3);
         z.r = z0x.real();
         z.x = z0x.imag();
         // Capacitância
         MontaMatrizC3x3(c3x3);
         // Cálculo da capacitância de sequência zero
         c0x  = Calcula_C0(c3x3);
         ycap = Ycap_siemens_km(c0x);
         break;

      default: // arranjoCABOxFASE
         // Impedância: elimina neutro, se existir (o considera multi-aterrado)
         MontaMatrizZ3x3(z3x3);
         // Cálculo da impedância de sequência zero
         z0x = Calcula_Z0(z3x3);
         z.r = z0x.real();
         z.x = z0x.imag();
         // Capacitância: determina maior capacitância entre os cabos
         cabo_c0 = MaiorCapacitancia(0);
         ycap    = Ycap_siemens_km(cabo_c0);

      } // switch()

   // Calcula Z0 em ohm
   z0_ohm.r = z.r * comp;
   z0_ohm.x = z.x * comp;

   // Calcula y1 em siemens
   yc0_mho = ycap * comp;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjo::Z0_pu(double Sbase, double Vbase, double comp, strIMP &z0_pu, double &yc0_pu)
   {
   //variáveis locais
   strIMP z0_ohm;
   double yc0_mho;
   double Zbase = (Vbase * Vbase) / Sbase;

   //calcula Z0 em ohm
   Z0_ohm(comp, z0_ohm, yc0_mho);

   //calcula Z0 em pu
   z0_pu.r = (z0_ohm.r) / Zbase;
   z0_pu.x = (z0_ohm.x) / Zbase;
   yc0_pu  = yc0_mho * Zbase;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjo::Z1_ohm(double comp, strIMP &z1_ohm, double &yc1_mho)
   {
   // Variáveis locais
   strIMP          z;
   float           ycap, cabo_c1;
   double          c1x, c3x3[3][3];
   complex<double> z1x, z3x3[3][3];

   // Verifica o tipo de configuração do Arranjo
   switch(TipoCfg)
      {
      case arranjoCABO_Z0Z1:
         z = caboZ0Z1->z1;
         // Calcula admitância: Ycap(mhos/km) = w*(cabo->c1*1e-9)
         ycap = Ycap_siemens_km(caboZ0Z1->c1);
         break;

      case arranjoZ0Z1:
      case arranjoPU_SB100:
         z = z1;
         // Calcula admitância: Ycap(mhos/km) = w*(c1*1e-9)
         ycap = Ycap_siemens_km(c1);
         break;

      case arranjoMAT_Z:
      case arranjoMUTUA:
         // Impedância: elimina neutro, se existir (o considera multi-aterrado)
         MontaMatrizZ3x3(z3x3);
         // Cálculo da impedância de sequência direta
         z1x = Calcula_Z1(z3x3);
         z.r = z1x.real();
         z.x = z1x.imag();
         // Capacitância
         MontaMatrizC3x3(c3x3);
         // Cálculo da capacitância de sequência direta
         c1x  = Calcula_C1(c3x3);
         ycap = Ycap_siemens_km(c1x);
         break;

      default: // arranjoCABOxFASE
         // Impedância: elimina neutro, se existir (o considera multi-aterrado)
         MontaMatrizZ3x3(z3x3);
         // Cálculo da impedância de sequência direta
         z1x = Calcula_Z1(z3x3);
         z.r = z1x.real();
         z.x = z1x.imag();
         // Capacitância: determina maior capacitância entre os cabos
         cabo_c1 = MaiorCapacitancia(1);
         ycap    = Ycap_siemens_km(cabo_c1);
      } // switch()

   // Calcula Z1 em ohm
   z1_ohm.r = z.r * comp;
   z1_ohm.x = z.x * comp;

   // Calcula y1 em siemens
   yc1_mho = ycap * comp;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjo::Z1_pu(double Sbase, double Vbase, double comp, strIMP &z1_pu, double &yc1_pu)
   {
   //variáveis locais
   strIMP z1_ohm;
   double yc1_mho;
   double Zbase = (Vbase * Vbase) / Sbase;

   //calcula Z1 em ohm
   Z1_ohm(comp, z1_ohm, yc1_mho);

   //calcula Z1 em pu
   z1_pu.r = z1_ohm.r / Zbase;
   z1_pu.x = z1_ohm.x / Zbase;
   yc1_pu  = yc1_mho * Zbase;

   return(true);
   }

//---------------------------------------------------------------------------
//eof