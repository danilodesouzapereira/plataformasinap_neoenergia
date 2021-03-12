//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TTramo.h"
#include "VTMatZ.h"
#include "VTResFlowLig.h"
#include "VTTrecho.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Constante\Fases.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TTramo
//---------------------------------------------------------------------------
VTTramo* __fastcall NewObjTramo(int num_pat)
   {
   return(new TTramo(num_pat));
   }

//---------------------------------------------------------------------------
__fastcall TTramo::TTramo(int num_pat)
   {
   //variávei locais
   int num_bar = 2;

   //IMPORTANTE:  os Tramos não são criados quando o trecho possui apacitância

   //cria objeto VTResFlowLig p/ armazenar resultados de fluxo
   AlocaResFlowLig(num_bar, num_pat);
   //cria objetos
   matZ   = NewObjMatZ();
   lisBAR = new TList();
   lisTRE = new TList();
   //inicia comprimento e iadm_min
   comp_km  = 0.;
   iadm_min = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TTramo::~TTramo(void)
   {
   //destrói objetos
   if (matZ) {delete matZ; matZ = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisTRE) {delete lisTRE; lisTRE = NULL;}
   //
   //OBS: a classe base TLigacao destrói o objeto VTResFlowLig
   }

//---------------------------------------------------------------------------
VTTramo* __fastcall TTramo::Clone(void)
   {
   //variáveis locais
   VTTramo *tramo;

   //cria um novo Tramo
   if ((tramo = NewObjTramo(resflow->Max_pat)) != NULL)
      {//copia dados deste Tramo p/ o novo Tramo
      tramo->CopiaAtributosDe(*this);
      }
   return(tramo);
   }

//---------------------------------------------------------------------------
void __fastcall TTramo::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TTramo &tramo = (TTramo&)ref;

   //proteção
   if (ref.Tipo() != eqptoTRAMO) return;
   //copia atributos de TLigacao
   TLigacao::CopiaAtributosDe(tramo);
   //copia atributos de VTTramo
   iadm_min = tramo.iadm_min;
   comp_km = tramo.comp_km;
   //copia Trechos
   lisTRE->Clear();
   tramo.LisTrecho(lisTRE);
   }

//---------------------------------------------------------------------------
int __fastcall TTramo::Fases(VTBarra *barra)
	{
	//variáveis locais
	VTTrecho *trecho;
	
	//OBS: os Trechos do Tramo devem ter fases iguais

	//determina primeiro Trecho
	if ((trecho = (VTTrecho*)lisTRE->First()) == NULL) return(faseINV);
	return(trecho->Fases(barra));
	}

//---------------------------------------------------------------------------
bool __fastcall TTramo::InsereTrecho(VTTrecho *trecho)
   {
   //variáveis locais
   int    ind_bar;
   double r0_ohm, x0_ohm, ycap0_mho;
   double r1_ohm, x1_ohm, ycap1_mho;

   //verifica se o Trecho já existe na lista lisTRE
   if (lisTRE->IndexOf(trecho) >= 0) return(true);
   //verifica se é o primeiro Trecho
   if (lisTRE->Count == 0)
      {//primeiro Trecho: assume Id igual ao do trecho
      Id = trecho->Id;
      //assume corrente mínima do Trecho
      iadm_min = trecho->Capacidade_amp;
      //insere Trecho na lista lisTRE
      lisTRE->Add(trecho);
      //define as duas Barras
      pbarra1 = trecho->pbarra1;
      pbarra2 = trecho->pbarra2;
      }
   //determina Barra comum entre o Tramo e o Trecho
   else if ((ind_bar = trecho->IndiceBarra(pbarra1)) >= 0)
      {//insere Trecho no início da lista lisTRE
      lisTRE->Insert(0, trecho);
      //Barra inicial passa a ser interna ao Tramo: insere em lisBAR
      lisBAR->Add(pbarra1);
      //redefine Barra inicial do Tramo
      pbarra1 = trecho->Barra(1 - ind_bar);
      }
   else if ((ind_bar = trecho->IndiceBarra(pbarra2)) >= 0)
      {//insere Trecho no final da lista lisTRE
      lisTRE->Add(trecho);
      //Barra final passa a ser interna ao Tramo: insere em lisBAR
      lisBAR->Add(pbarra2);
      //redefine Barra final do Tramo
      pbarra2 = trecho->Barra(1 - ind_bar);
      }
   else
      {//Trecho não possui Barra comum c/ o Tramo
      return(false);
      }
   //atualiza comprimento
   comp_km += trecho->Comprimento_km;
   //atualiza iadm_min
   iadm_min = min(iadm_min, float(trecho->Capacidade_amp));
   //atualiza impedâncias de seq0 do Tramo
   trecho->Z0_ohm(r0_ohm, x0_ohm, ycap0_mho);
   z0.r  += r0_ohm;
   z0.x  += x0_ohm;
   ycap0 += ycap0_mho;
   //atualiza impedâncias de seq1 do Tramo
   trecho->Z1_ohm(r1_ohm, x1_ohm, ycap1_mho);
   z1.r  += r1_ohm;
   z1.x  += x1_ohm;
   ycap1 += ycap1_mho;
   //atualiza MatZ do Tramo
   for (int i = 0; i < matZ->Dimensao; i++)
      {
      for (int j = 0; j < matZ->Dimensao; j++)
         {
         matZ->Z[i][j] = matZ->Z[i][j] + trecho->MatZ_ohm[i][j];
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TTramo::LisBarraInterna(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
void __fastcall TTramo::LisTrecho(TList *lisEXT)
   {
   CopiaTList(lisTRE, lisEXT);
   }

//---------------------------------------------------------------------------
TList* __fastcall TTramo::LisTrecho(void)
   {
   return(lisTRE);
   }

//---------------------------------------------------------------------------
double __fastcall TTramo::PM_GetCapacidade_amp(void)
   {
   return(iadm_min);
   }

//---------------------------------------------------------------------------
double __fastcall TTramo::PM_GetComprimento_m(void)
   {
   return(Comprimento_km * 1000.);
   }

//---------------------------------------------------------------------------
double __fastcall TTramo::PM_GetComprimento_km(void)
   {
   return(comp_km);
   }

//---------------------------------------------------------------------------
bool __fastcall TTramo::PM_GetEquilibrado(void)
   {
   //variáveis locais
   VTTrecho *trecho;

   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      if (! trecho->Equilibrado) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TTramo::PM_GetIadm_amp(int nf)
   {
   //variáveis locais
   double   iadm = 0.;
   VTTrecho *trecho;

   //proteçao
   if (lisTRE->Count == 0) return(0);
   //determina corrente admissível do primeiro Trecho
   trecho = (VTTrecho*)lisTRE->First();
   iadm   = trecho->Iadm_amp[nf];
   //determina corrente admissível dos demais Trechos
   for (int n = 1; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      iadm   = min(trecho->Iadm_amp[nf], iadm);
      }
   return(iadm);
   }

//---------------------------------------------------------------------------
VTMatZ* __fastcall TTramo::PM_GetMatZ(void)
   {
   return(matZ);
   }

//---------------------------------------------------------------------------
int __fastcall TTramo::PM_GetTipoCfg(void)
   {
   //variáveis locais
   VTTrecho *trecho;

   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      if (trecho->arranjo->TipoCfg == arranjoMAT_Z) return(arranjoMAT_Z);
      }
   return(arranjoZ0Z1);
   }

//---------------------------------------------------------------------------
bool __fastcall TTramo::PM_GetUtilizaNeutro(void)
   {
   //variáveis locais
   VTTrecho *trecho;

   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      if (! trecho->UtilizaNeutro) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTramo::PM_GetZ_ohm(int i, int j)
   {
   return(matZ->Z[i][j]);
   }

//---------------------------------------------------------------------------
bool __fastcall TTramo::Z0_ohm(double &r0, double &x0, double &yc0)
   {
   //copia valores
   r0  = z0.r;
   x0  = z0.x;
   yc0 = ycap0;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTramo::Z0_pu(double Sbase, double Vbase, double &r0, double &x0, double &yc0)
   {
   //variáveis locais
   double Zbase = (Vbase * Vbase) / Sbase;

   //copia valores, convertendo p/ pu
   r0  = z0.r  / Zbase;
   x0  = z0.x  / Zbase;
   yc0 = ycap0 * Zbase;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTramo::Z1_ohm(double &r1, double &x1, double &yc1)
   {
   //copia valores
   r1  = z1.r;
   x1  = z1.x;
   yc1 = ycap0;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTramo::Z1_pu(double Sbase, double Vbase, double &r1, double &x1, double &yc1)
   {
   //variáveis locais
   double Zbase = (Vbase * Vbase) / Sbase;

   //copia valores, convertendo p/ pu
   r1  = z1.r  / Zbase;
   x1  = z1.x  / Zbase;
   yc1 = ycap1 * Zbase;
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TTramo::Tipo(void)
   {
   return(eqptoTRAMO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTramo::TipoAsString(void)
   {
   return("Tramo");
   }
//---------------------------------------------------------------------------
//eof
