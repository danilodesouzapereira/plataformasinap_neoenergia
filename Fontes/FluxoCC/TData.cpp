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
//função global p/ criar objeto da classe VTData
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
   //destrói lista e seus objetos
   if (lisBAR) {LimpaTList(lisBAR); delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {LimpaTList(lisLIG); delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
TBar* __fastcall TData::CriaNovaBar(void *eqpto, int bar_id, AnsiString codigo)
   {
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
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
   //NÃO utiliza capacitor
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcCAPSERIE &capserie)
   {
   //variáveis locais
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
   //variáveis locais
   TBar *bar;

   //determina Bar
   if ((bar = ExisteBar(carga.barra_id)) == NULL) return(false);
   //atualiza potência ativa de carga de Bar
   bar->InsereCarga(carga.set.s_mva.real() / Sbase_mva);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcFILTRO &filtro)
   {
   //NÃO utiliza filtro
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcGERADOR &gerador)
   {
   //variáveis locais
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
      {//atualiza potência ativa de geração de Bar
      bar->InsereGeracao(gerador.set.sup.p_mw/Sbase_mva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcMUTUA &mutua)
   {
   //NÃO utiliza mútua
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcREATOR &reator)
   {
   //NÃO utiliza reator
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcREGULADOR &regulador)
   {
   //variáveis locais
   double xreg_pu;
   TLig   *lig;

   //cria uma nova Ligacao
   if ((lig = CriaNovaLig(regulador.eqpto, regulador.barra1_id, regulador.barra2_id)) == NULL) return(false);
   // Define reatância do regulador em pu (base 100 MVA)
   xreg_pu   = 0.0001 * Sbase_mva / regulador.snom_mva;  // 0,01% na própria base
   lig->bser = - 1. / xreg_pu;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcSUPRIMENTO &suprimento)
   {
   //variáveis locais
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
      {//atualiza potência ativa de geração de Bar
      bar->InsereGeracao(suprimento.set.sup.p_mw/Sbase_mva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcTRAFO2E &trafo2E)
   {
   //variáveis locais
   double fat_conv;
   TLig   *lig;

   //cria uma nova Ligacao
   if ((lig = CriaNovaLig(trafo2E.eqpto, trafo2E.barra1_id, trafo2E.barra2_id)) == NULL) return(false);
   //determina susceptância do trafo em pu de Sbase
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
   //NÃO utiliza trafoZZ
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcTRECHO &trecho)
   {
   //variáveis locais
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
   //NÃO utiliza Yref
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Insere(smcZREF &zref)
   {
   //NÃO utiliza Zref
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
   //variáveis locais
   double p_pu = 0.;

   //verifica se Bar é de carga
   if (bar->tipo == barCARGA)
      {//Bar de Carga: retorna sua potência de geração
      p_pu = bar->p_ger;
      }
   else
      {//Bar swing: soma a potência de carga e geração de todas as Bar (exceto potência de geração da swing)
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
   //variáveis locais
   TBar *bar = ExisteBar(barra.eqpto);

   //proteção
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
   TLig *lig = ExisteLig(trafo2E.eqpto);

   //proteção
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
   //variáveis locais
   TLig *lig = ExisteLig(trecho.eqpto);

   //proteção
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
   //proteção
   if ((r_pu == 0.)&&(x_pu == 0.)) return(0.);
   //retorna susceptância em pu: -x/(r*r + x*x)
   return((-x_pu)/((r_pu*r_pu) + (x_pu*x_pu)));
   }

//---------------------------------------------------------------------------
double __fastcall TData::Susceptancia_pu(double r_ohm, double x_ohm, double Vbase_kv)
   {
   //variáveis locais
   double Zbase_ohm = (Vbase_kv * Vbase_kv) / Sbase_mva;

   //calcula susceptância em pu
   return(Susceptancia_pu(r_ohm/Zbase_ohm, x_ohm/Zbase_ohm));
   }

//---------------------------------------------------------------------------
//eof
