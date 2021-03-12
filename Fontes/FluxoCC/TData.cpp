//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TData.h"
#include ".\TBar.h"
#include ".\TLig.h"
#include <Fontes\Complexo\Complexo.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTData
//---------------------------------------------------------------------------
VTData* __fastcall NewObjData(VTApl *apl)
   {
   return(new TData(apl));
   }

//---------------------------------------------------------------------------
__fastcall TData::TData(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   lisBAR = new TList();
   lisLIG = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TData::~TData(void)
   {
   //destr�i lista e seus objetos
   if (lisBAR) {LimpaTList(lisBAR); delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {LimpaTList(lisLIG); delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
TBar* __fastcall TData::CriaNovaBar(void *eqpto, int bar_id, AnsiString codigo)
   {
   //vari�veis locais
   TBar *bar;

   try{//cria uma nova Bar e insere em lisBAR
      lisBAR->Add(bar = new TBar(eqpto, bar_id, codigo));
      }catch(Exception &e)
         {
         bar = NULL;
         }
   return(bar);
   }

//---------------------------------------------------------------------------
TLig* __fastcall TData::CriaNovaLig(void *eqpto, int bar1_id, int bar2_id)
   {
   //vari�veis locais
   TLig *lig;
   TBar *bar1, *bar2;

   //determina Bar da Lig
   if ((bar1 = ExisteBar(bar1_id)) == NULL) return(NULL);
   if ((bar2 = ExisteBar(bar2_id)) == NULL) return(NULL);
   try{//cria uma nova Lig e insere em lisLIG
      lisLIG->Add(lig = new TLig(eqpto, bar1, bar2));
      }catch(Exception &e)
         {
         lig = NULL;
         }
   return(lig);
   }

//---------------------------------------------------------------------------
TBar* __fastcall TData::ExisteBar(int bar_id)
   {
   //vari�veis locais
   TBar *bar;

   //procura Bar em lisBAR c/ o Id indicado
   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar*)lisBAR->Items[n];
      if (bar->id == bar_id) return(bar);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TBar* __fastcall TData::ExisteBar(void *eqpto)
   {
   //vari�veis locais
   TBar *bar;

   //procura Bar em lisBAR c/ o Id indicado
   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar*)lisBAR->Items[n];
      if (bar->eqpto == eqpto) return(bar);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TLig* __fastcall TData::ExisteLig(int bar1_id, int bar2_id)
   {
   //vari�veis locais
   TLig *lig;

   //procura Lig em lisLIG c/ as mesmas barras
   for (int n = 0; n < lisLIG->Count; n++)
      {
      lig = (TLig*)lisLIG->Items[n];
      if ((lig->bar1->id == bar1_id)&&(lig->bar2->id == bar2_id)) return(lig);
      if ((lig->bar1->id == bar2_id)&&(lig->bar2->id == bar1_id)) return(lig);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TLig* __fastcall TData::ExisteLig(void *eqpto)
   {
   //vari�veis locais
   TLig *lig;

   //procura Lig em lisLIG c/ as mesmas barras
   for (int n = 0; n < lisLIG->Count; n++)
      {
      lig = (TLig*)lisLIG->Items[n];
      if (lig->eqpto == eqpto) return(lig);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Inicia(double sbase)
   {
   //salva Sbase
   Sbase_mva = sbase;
   //reinicia listas
   LimpaTList(lisBAR);
   LimpaTList(lisLIG);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcBARRA &barra)
   {
   //vari�veis locais
   TBar *bar;

   //cria uma nova Bar
   if ((bar = CriaNovaBar(barra.eqpto, barra.id, barra.codigo)) == NULL) return(false);
   //atualiza dados de Bar
   bar->vnom_kv = barra.set.vnom_kv;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcCAPACITOR &capacitor)
   {
   //N�O utiliza capacitor
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcCAPSERIE &capserie)
   {
   //vari�veis locais
   TLig *lig;

   //cria uma nova Lig
   if ((lig = CriaNovaLig(capserie.eqpto, capserie.barra1_id, capserie.barra2_id)) == NULL) return(false);
   //atualiza dados de Lig
   lig->bser = Susceptancia_pu(0., capserie.set.x_ohm, lig->bar1->vnom_kv);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcCARGA &carga)
   {
   //vari�veis locais
   TBar *bar;

   //determina Bar
   if ((bar = ExisteBar(carga.barra_id)) == NULL) return(false);
   //atualiza pot�ncia ativa de carga de Bar
   bar->InsereCarga(carga.set.s_mva.real() / Sbase_mva);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcFILTRO &filtro)
   {
   //N�O utiliza filtro
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcGERADOR &gerador)
   {
   //vari�veis locais
   TBar *bar;

   //determina Bar
   if ((bar = ExisteBar(gerador.barra_id)) == NULL) return(false);
   //verifica o tipo de suprimento
   if (gerador.set.sup.tipo == tiposupVF)
      {//swing
      bar->tipo           = barSWING;
      bar->res.v_pu       = gerador.set.sup.v_pu;
      bar->res.angulo_rad = gerador.set.sup.t_rad ;
      }
   else
      {//atualiza pot�ncia ativa de gera��o de Bar
      bar->InsereGeracao(gerador.set.sup.p_mw/Sbase_mva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcMUTUA &mutua)
   {
   //N�O utiliza m�tua
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcREATOR &reator)
   {
   //N�O utiliza reator
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcREGULADOR &regulador)
   {
   //vari�veis locais
   double xreg_pu;
   TLig   *lig;

   //cria uma nova Ligacao
   if ((lig = CriaNovaLig(regulador.eqpto, regulador.barra1_id, regulador.barra2_id)) == NULL) return(false);
   // Define reat�ncia do regulador em pu (base 100 MVA)
   xreg_pu   = 0.0001 * Sbase_mva / regulador.snom_mva;  // 0,01% na pr�pria base
   lig->bser = - 1. / xreg_pu;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcSUPRIMENTO &suprimento)
   {
   //vari�veis locais
   TBar *bar;

   //determina Bar
   if ((bar = ExisteBar(suprimento.barra_id)) == NULL) return(false);
   //verifica o tipo de suprimento
   if (suprimento.set.sup.tipo == tiposupVF)
      {//swing
      bar->tipo = barSWING;
      bar->res.v_pu       = suprimento.set.sup.v_pu;
      bar->res.angulo_rad = suprimento.set.sup.t_rad ;
      }
   else
      {//atualiza pot�ncia ativa de gera��o de Bar
      bar->InsereGeracao(suprimento.set.sup.p_mw/Sbase_mva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcTRAFO2E &trafo2E)
   {
   //vari�veis locais
   double fat_conv;
   TLig   *lig;

   //cria uma nova Ligacao
   if ((lig = CriaNovaLig(trafo2E.eqpto, trafo2E.barra1_id, trafo2E.barra2_id)) == NULL) return(false);
   //determina suscept�ncia do trafo em pu de Sbase
   fat_conv  = Sbase_mva / trafo2E.set.snom_mva;
   lig->bser = Susceptancia_pu(trafo2E.set.z1_pu.real() * fat_conv,
                               trafo2E.set.z1_pu.imag() * fat_conv);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcTRAFO3E &trafo3E)
   {
   int PENDENCIA_INSERE_TRAFO3E=1;
   
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcTRAFOZZ &trafoZZ)
   {
   //N�O utiliza trafoZZ
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcTRECHO &trecho)
   {
   //vari�veis locais
   TLig *lig;

   //cria uma nova Ligacao
   if ((lig = CriaNovaLig(trecho.eqpto, trecho.barra1_id, trecho.barra2_id)) == NULL) return(false);
   //atualiza dados de Lig
   lig->bser = Susceptancia_pu(trecho.set.z1_ohm.real(), trecho.set.z1_ohm.imag(), lig->bar1->vnom_kv);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcYREF &yref)
   {
   //N�O utiliza Yref
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcZREF &zref)
   {
   //N�O utiliza Zref
   return(true);
   }
//---------------------------------------------------------------------------
TList* __fastcall TData::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TData::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
double __fastcall TData::Pinjetada_pu(TBar *bar)
   {
   //vari�veis locais
   double p_pu = 0.;

   //verifica se Bar � de carga
   if (bar->tipo == barCARGA)
      {//Bar de Carga: retorna sua pot�ncia de gera��o
      p_pu = bar->p_ger;
      }
   else
      {//Bar swing: soma a pot�ncia de carga e gera��o de todas as Bar (exceto pot�ncia de gera��o da swing)
      for (int nb = 0; nb < lisBAR->Count; nb++)
         {
         bar   = (TBar*)lisBAR->Items[nb];
         p_pu -= bar->p_car;
         if (bar->tipo == barCARGA) p_pu += bar->p_ger;
         }
      //inverte o sinal
      p_pu = - p_pu;
      }
   return(p_pu);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumBar(void)
   {
   return(lisBAR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumLig(void)
   {
   return(lisLIG->Count);
   }

//---------------------------------------------------------------------------
double __fastcall TData::PM_GetSbaseMVA(void)
   {
   return(Sbase_mva);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Resultado(smcBARRA &barra)
   {
   //vari�veis locais
   TBar *bar = ExisteBar(barra.eqpto);

   //prote��o
   if (bar == NULL) return(false);
   //copia resultados
   barra.get.Van_pu = polar(bar->res.v_pu, bar->res.angulo_rad);
   barra.get.Vbn_pu = barra.get.Van_pu * Alfa2();
   barra.get.Vcn_pu = barra.get.Van_pu * Alfa();
   barra.get.Vnt_pu = complex<double>(0., 0.);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Resultado(smcGERADOR &gerador)
   {
   //vari�veis locais
   double p_pu;
   TBar *bar;

   //determina Bar
   if ((bar = ExisteBar(gerador.barra_id)) == NULL) return(false);
   //determina pot.ativa injetada na Bar
   p_pu = Pinjetada_pu(bar);
   //atualiza smc
   gerador.get.Sa_pu = complex<double>(p_pu/3., 0.);
   gerador.get.Sb_pu = complex<double>(p_pu/3., 0.);
   gerador.get.Sc_pu = complex<double>(p_pu/3., 0.);
   gerador.get.Sn_pu = complex<double>(0., 0.);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Resultado(smcSUPRIMENTO &suprimento)
   {
   //vari�veis locais
   double p_pu;
   TBar *bar;

   //determina Bar
   if ((bar = ExisteBar(suprimento.barra_id)) == NULL) return(false);
   //determina pot.ativa injetada na Bar
   p_pu = Pinjetada_pu(bar);
   //atualiza smc
   suprimento.get.Sa_pu = complex<double>(p_pu/3., 0.);
   suprimento.get.Sb_pu = complex<double>(p_pu/3., 0.);
   suprimento.get.Sc_pu = complex<double>(p_pu/3., 0.);
   suprimento.get.Sn_pu = complex<double>(0., 0.);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Resultado(smcTRAFO2E &trafo2E)
   {
   //vari�veis locais
   TLig *lig = ExisteLig(trafo2E.eqpto);

   //prote��o
   if (lig == NULL) return(false);
   //copia resultados
   trafo2E.get.Sa1_pu = complex<double>(lig->res.fluxo12_pu / 3., 0.);
   trafo2E.get.Sb1_pu = complex<double>(lig->res.fluxo12_pu / 3., 0.);
   trafo2E.get.Sc1_pu = complex<double>(lig->res.fluxo12_pu / 3., 0.);
   trafo2E.get.Sn1_pu = complex<double>(0., 0.);
   trafo2E.get.Sa2_pu = - trafo2E.get.Sa1_pu;
   trafo2E.get.Sb2_pu = - trafo2E.get.Sb1_pu;
   trafo2E.get.Sc2_pu = - trafo2E.get.Sc1_pu;
   trafo2E.get.Sn2_pu = - trafo2E.get.Sn1_pu;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Resultado(smcTRECHO &trecho)
   {
   //vari�veis locais
   TLig *lig = ExisteLig(trecho.eqpto);

   //prote��o
   if (lig == NULL) return(false);
   //copia resultados
   trecho.get.Sa1_pu = complex<double>(lig->res.fluxo12_pu / 3., 0.);
   trecho.get.Sb1_pu = complex<double>(lig->res.fluxo12_pu / 3., 0.);
   trecho.get.Sc1_pu = complex<double>(lig->res.fluxo12_pu / 3., 0.);
   trecho.get.Sn1_pu = complex<double>(0., 0.);
   trecho.get.Sa2_pu = - trecho.get.Sa1_pu;
   trecho.get.Sb2_pu = - trecho.get.Sb1_pu;
   trecho.get.Sc2_pu = - trecho.get.Sc1_pu;
   trecho.get.Sn2_pu = - trecho.get.Sn1_pu;
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TData::Susceptancia_pu(double r_pu, double x_pu)
   {
   //prote��o
   if ((r_pu == 0.)&&(x_pu == 0.)) return(0.);
   //retorna suscept�ncia em pu: -x/(r*r + x*x)
   return((-x_pu)/((r_pu*r_pu) + (x_pu*x_pu)));
   }

//---------------------------------------------------------------------------
double __fastcall TData::Susceptancia_pu(double r_ohm, double x_ohm, double Vbase_kv)
   {
   //vari�veis locais
   double Zbase_ohm = (Vbase_kv * Vbase_kv) / Sbase_mva;

   //calcula suscept�ncia em pu
   return(Susceptancia_pu(r_ohm/Zbase_ohm, x_ohm/Zbase_ohm));
   }

//---------------------------------------------------------------------------
//eof
