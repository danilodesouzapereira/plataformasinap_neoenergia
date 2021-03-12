//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TCfgPerdaTec.h"
#include "..\VTInfoset.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Flow\VTFlow.h"
#include "..\..\Rede\VTEqpto.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//conjunto de CABO PRODIST
static strCFG_CABO cfg_cabo[] =
                           {//ID   material    codigo      r_ohm_km
                           {  1,  "Al",       "6 AWG",     2.469},
                           {  2,  "Al",       "4 AWG",     1.551},
                           //====================================
                           {100,  "Al",       "10 mm2",     3.514},
                           {101,  "Al",       "16 mm2",     2.179},
                           {102,  "Al",       "25 mm2",     2.179},
                           //====================================
                           {200,  "Cu",       "0.5 mm2",   40.952},
                           {201,  "Cu",       "075 mm2",   27.870},
                           {202,  "Cu",       "1 mm2",     20.590},
                           //====================================
                           {300,  "Cu",       "10 AWG",     3.754},
                           {301,  "Cu",       "9 AWG" ,     2.958},
                           {302,  "Cu",       "8 AWG" ,     2.389}
                           };
//conjunto de PERDA_RAMAL da Empresa
static strCFG_RML cfg_ramal[] =
                           {//Vmin    Vmax  fases  comp_m   r_ohm_km
                           { 0.0  ,  0.220,     1,   15.0,    3.514},
                           { 0.0  ,  0.220,     2,   15.0,    3.514},
                           { 0.0  ,  0.220,     3,   15.0,    2.179},
                           //======================================
                           { 0.220,  0.380,     1,   15.0,    3.514},
                           { 0.220,  0.380,     2,   15.0,    3.514},
                           { 0.220,  0.380,     3,   15.0,    2.179},
                           //======================================
                           { 0.380,  0.440,     1,   15.0,    3.514},
                           { 0.380,  0.440,     2,   15.0,    3.514},
                           { 0.380,  0.440,     3,   15.0,    3.514}
                           };

//conjunto de PERDA_TRF do Prodis
static strCFG_TRF prodist_trf_mono[] =
                           {//Vmin    Vmax   Snom    vazio  total
                           {   0.0,   15.0,   5.0,    35,    140},
                           {   0.0,   15.0,  10.0,    50,    245},
                           {   0.0,   15.0,  15.0,    65,    330},
                           {   0.0,   15.0,  25.0,    90,    480},
                           {   0.0,   15.0,  37.5,   135,    665},
                           {   0.0,   15.0,  50.0,   165,    780},
                           {   0.0,   15.0,  75.0,   205,   1110},
                           {   0.0,   15.0, 100.0,   255,   1445},
                           //=====================================
                           {  15.0,   24.2,   5.0,    40,    155},
                           {  15.0,   24.2,  10.0,    55,    265},
                           {  15.0,   24.2,  15.0,    75,    365},
                           {  15.0,   24.2,  25.0,   100,    520},
                           {  15.0,   24.2,  37.5,   145,    740},
                           {  15.0,   24.2,  50.0,   190,    925},
                           {  15.0,   24.2,  75.0,   225,   1210},
                           {  15.0,   24.2, 100.0,   275,   1495},
                           //=====================================
                           {  24.2,   36.2,   5.0,    45,    160},
                           {  24.2,   36.2,  10.0,    60,    270},
                           {  24.2,   36.2,  15.0,    80,    380},
                           {  24.2,   36.2,  25.0,   105,    545},
                           {  24.2,   36.2,  37.5,   150,    740},
                           {  24.2,   36.2,  50.0,   200,    935},
                           {  24.2,   36.2,  75.0,   240,   1225},
                           {  24.2,   36.2, 100.0,   280,   1480}
                           };
static strCFG_TRF prodist_trf_trif[] =
                           {//Vmin    Vmax   Snom    vazio  total
                           {   0.0,   15.0,  15.0,    85,    410},
                           {   0.0,   15.0,  30.0,   150,    695},
                           {   0.0,   15.0,  45.0,   195,    945},
                           {   0.0,   15.0,  75.0,   295,   1395},
                           {   0.0,   15.0, 112.5,   390,   1890},
                           {   0.0,   15.0, 150.0,   485,   2335},
                           {   0.0,   15.0, 225.0,   650,   3260},
                           {   0.0,   15.0, 300.0,   810,   4060},
                           //=====================================
                           {  15.0,   24.2,  15.0,    95,    470},
                           {  15.0,   24.2,  30.0,   160,    790},
                           {  15.0,   24.2,  45.0,   215,   1055},
                           {  15.0,   24.2,  75.0,   315,   1550},
                           {  15.0,   24.2, 112.5,   425,   2085},
                           {  15.0,   24.2, 150.0,   520,   2610},
                           {  15.0,   24.2, 225.0,   725,   3605},
                           {  15.0,   24.2, 300.0,   850,   4400},
                           //=====================================
                           {  24.2,   36.2,  15.0,   100,    460},
                           {  24.2,   36.2,  30.0,   165,    775},
                           {  24.2,   36.2,  45.0,   230,   1075},
                           {  24.2,   36.2,  75.0,   320,   1580},
                           {  24.2,   36.2, 112.5,   440,   2055},
                           {  24.2,   36.2, 150.0,   540,   2640},
                           {  24.2,   36.2, 225.0,   750,   3600},
                           {  24.2,   36.2, 300.0,   900,   4450}
                           };
static strCFG_TRF empresa_trf_mono[] =
                           {//Vmin    Vmax   Snom    vazio  total
                           {   0.0,   15.0,   5.0,    35,    140},
                           {   0.0,   15.0,  10.0,    50,    245},
                           {   0.0,   15.0,  15.0,    65,    330},
                           {   0.0,   15.0,  25.0,    90,    480},
                           {   0.0,   15.0,  37.5,   135,    665},
                           {   0.0,   15.0,  50.0,   165,    780},
                           {   0.0,   15.0,  75.0,   205,   1110},
                           {   0.0,   15.0, 100.0,   255,   1445},
                           //=====================================
                           {  15.0,   24.2,   5.0,    40,    155},
                           {  15.0,   24.2,  10.0,    55,    265},
                           {  15.0,   24.2,  15.0,    75,    365},
                           {  15.0,   24.2,  25.0,   100,    520},
                           {  15.0,   24.2,  37.5,   145,    740},
                           {  15.0,   24.2,  50.0,   190,    925},
                           {  15.0,   24.2,  75.0,   225,   1210},
                           {  15.0,   24.2, 100.0,   275,   1495},
                           //=====================================
                           {  24.2,   36.2,   5.0,    45,    160},
                           {  24.2,   36.2,  10.0,    60,    270},
                           {  24.2,   36.2,  15.0,    80,    380},
                           {  24.2,   36.2,  25.0,   105,    545},
                           {  24.2,   36.2,  37.5,   150,    740},
                           {  24.2,   36.2,  50.0,   200,    935},
                           {  24.2,   36.2,  75.0,   240,   1225},
                           {  24.2,   36.2, 100.0,   280,   1480}
                           };
static strCFG_TRF empresa_trf_trif[] =
                           {//Vmin    Vmax   Snom    vazio  total
                           {   0.0,   15.0,  15.0,    85,    410},
                           {   0.0,   15.0,  30.0,   150,    695},
                           {   0.0,   15.0,  45.0,   195,    945},
                           {   0.0,   15.0,  75.0,   295,   1395},
                           {   0.0,   15.0, 112.5,   390,   1890},
                           {   0.0,   15.0, 150.0,   485,   2335},
                           {   0.0,   15.0, 225.0,   650,   3260},
                           {   0.0,   15.0, 300.0,   810,   4060},
                           //=====================================
                           {  15.0,   24.2,  15.0,    95,    470},
                           {  15.0,   24.2,  30.0,   160,    790},
                           {  15.0,   24.2,  45.0,   215,   1055},
                           {  15.0,   24.2,  75.0,   315,   1550},
                           {  15.0,   24.2, 112.5,   425,   2085},
                           {  15.0,   24.2, 150.0,   520,   2610},
                           {  15.0,   24.2, 225.0,   725,   3605},
                           {  15.0,   24.2, 300.0,   850,   4400},
                           //=====================================
                           {  24.2,   36.2,  15.0,   100,    460},
                           {  24.2,   36.2,  30.0,   165,    775},
                           {  24.2,   36.2,  45.0,   230,   1075},
                           {  24.2,   36.2,  75.0,   320,   1580},
                           {  24.2,   36.2, 112.5,   440,   2055},
                           {  24.2,   36.2, 150.0,   540,   2640},
                           {  24.2,   36.2, 225.0,   750,   3600},
                           {  24.2,   36.2, 300.0,   900,   4450}
                           };
static strCFG_TRF_SED trf_sed[] =
                           {//Snom    vazio  total
                           { 20.0,    0.20,    0.80},
                           {  0.0,    0.15,    0.60}
                           };

//---------------------------------------------------------------------------
VTCfgPerdaTec* __fastcall NewObjCfgPerdaTec(VTApl *apl)
   {
   return(new TCfgPerdaTec(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCfgPerdaTec::TCfgPerdaTec(VTApl *apl)
   {
   //salva objetos
   this->apl = apl;
   //cria listas
   lisPerdaCabo         = new TList();
   lisPerdaRamal        = new TList();
   lisPerdaMedidor      = new TList();
   lisPerdaTrafoProdist = new TList();
   lisPerdaTrafoEmpresa = new TList();
   lisPerdaTrafoSED     = new TList();
   lisTMP               = new TList();
   //lê configuração de perdas dos eqptos salvo em infoset
   LeCfgPerdas();
   //habilita progresso
   fluxo.progresso = true;
   //se necessário, inicia dados de perdas em cabo, ramal, medidor e trafo
   PD.alterada     = false;
   IniciaPerdasCabo();
   IniciaPerdasMedidor();
   IniciaPerdasRamal();
   IniciaPerdasTrafo();
   //inicia opções de valor único para perdas dos eqptos
   IniciaOpcaoValorUnico();
   //inicia valores default dos parâmetros
   IniciaConfiguracaoGis();
   }

//---------------------------------------------------------------------------
__fastcall TCfgPerdaTec::~TCfgPerdaTec(void)
   {
   //verifica se foi feita alguma alteração dos dados
   if (PD.alterada)
      {//salva dados da configuração de perdas
      SalvaCfgPerdas();
      }
   //destrói listas sem destruir  seus objetos
   if(lisTMP) {delete lisTMP; lisTMP = NULL;}
   //destrói listas e seus objetos
   if(lisPerdaCabo)         {LimpaTList(lisPerdaCabo);         delete lisPerdaCabo;         lisPerdaCabo         = NULL;}
   if(lisPerdaRamal)        {LimpaTList(lisPerdaRamal);        delete lisPerdaRamal;        lisPerdaRamal        = NULL;}
   if(lisPerdaMedidor)      {LimpaTList(lisPerdaMedidor);      delete lisPerdaMedidor;      lisPerdaMedidor      = NULL;}
   if(lisPerdaTrafoEmpresa) {LimpaTList(lisPerdaTrafoEmpresa); delete lisPerdaTrafoEmpresa; lisPerdaTrafoEmpresa = NULL;}
   if(lisPerdaTrafoProdist) {LimpaTList(lisPerdaTrafoProdist); delete lisPerdaTrafoProdist; lisPerdaTrafoProdist = NULL;}
   if(lisPerdaTrafoSED)     {LimpaTList(lisPerdaTrafoSED);     delete lisPerdaTrafoSED;     lisPerdaTrafoSED     = NULL;}
   }

//---------------------------------------------------------------------------
PERDA_CABO* __fastcall TCfgPerdaTec::ExistePerdaCabo(int id)
   {
   //variáveis locais
   PERDA_CABO *perda_cabo;

   //loop p/ todos PERDA_CABO
   for(int n = 0; n < lisPerdaCabo->Count; n++)
      {
      perda_cabo = (PERDA_CABO*)lisPerdaCabo->Items[n];
      if (perda_cabo->id != id) return(perda_cabo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
PERDA_MEDIDOR* __fastcall TCfgPerdaTec::ExistePerdaMedidor(void)
   {
   //verifica opção
   if (opcao.perda_medidor == opMEDIDOR_NAO_USAR) return(NULL);
   //retorna PERDA_MEDIDOR único
   return(unico.perda_medidor);
   }

//---------------------------------------------------------------------------
PERDA_RAMAL* __fastcall TCfgPerdaTec::ExistePerdaRamal(int num_fases, double v_kv)
   {
   //variáveis locais
   PERDA_RAMAL *perda_ramal;

   //verifica opção
   if (opcao.perda_ramal == opRAMAL_NAO_USAR) return(NULL);
   if (opcao.perda_ramal == opRAMAL_GIS)      return(NULL);
   if (opcao.perda_ramal == opRAMAL_UNICO)    return((unico.perda_ramal));
   //proteção
   if (lisPerdaRamal->Count == 0) {IniciaPerdasRamal();}
   //loop p/ todos PERDA_RAMAL
   for(int nc = 0; nc < lisPerdaRamal->Count; nc++)
      {
      perda_ramal = (PERDA_RAMAL*)lisPerdaRamal->Items[nc];
      if (perda_ramal->num_fases != num_fases) continue;
      if ((v_kv > perda_ramal->vmin_kv) && ( v_kv <= perda_ramal->vmax_kv)) return(perda_ramal);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
PERDA_TRAFO* __fastcall TCfgPerdaTec::ExistePerdaTrafo(int tipo, double snom_kva, double v_kv)
   {
   //variáveis locais
   double      erro, erro_max;
   TList       *lisPerdaTrafo;
   PERDA_TRAFO *perda_trafo, *perda_trafo_sel;;

   //verifica opção
   if (opcao.perda_trafo == opTRAFO_GIS)      return(NULL);
   if (opcao.perda_trafo == opTRAFO_UNICO)    return(unico.perda_trafo);
   if (opcao.perda_trafo == opTRAFO_PRODIST)
      {//utiliza conjunto de PERDAS_TRAFO de lisPerdaTrafoProdist
      lisPerdaTrafo = lisPerdaTrafoProdist;
      }
   else
      {//utiliza conjunto de PERDAS_TRAFO de lisPerdaTrafoEmpresa
      lisPerdaTrafo = lisPerdaTrafoEmpresa;
      }
   //proteção
   if (lisPerdaTrafo->Count == 0) {IniciaPerdasTrafo();}
   //monta lisTMP com todos PERDA_TRAFO que atendem ao tipo e à tensão
   lisTMP->Clear();
   for (int n = 0; n < lisPerdaTrafo->Count; n++)
      {
      perda_trafo = (PERDA_TRAFO*)lisPerdaTrafo->Items[n];
      if (perda_trafo->tipo != tipo) continue;
      //verifica faixa de tensão
      if ((v_kv > perda_trafo->vmin_kv) && ( v_kv <= perda_trafo->vmax_kv))
         {//insere perda_trafo em lisTMP
         lisTMP->Add(perda_trafo);
         }
      }
   //determina PERDA_TRAFO em lisTMP com Snom mais próxima da indicada
   perda_trafo_sel = NULL;
   for (int n = 0; n < lisTMP->Count; n++)
      {
      perda_trafo = (PERDA_TRAFO*)lisTMP->Items[n];
      erro = fabs(perda_trafo->snom_kva - snom_kva);
      if (perda_trafo_sel == NULL)
         {
         erro_max        = erro;
         perda_trafo_sel = perda_trafo;
         }
      else if (erro < erro_max)
         {
         erro_max        = erro;
         perda_trafo_sel = perda_trafo;
         }
      }
   return(perda_trafo_sel);
   }

//---------------------------------------------------------------------------
PERDA_TRAFO_SED* __fastcall TCfgPerdaTec::ExistePerdaTrafoSED(double snom_mva)
   {
   //variáveis locais
   double          erro, erro_max;
   PERDA_TRAFO_SED *perda, *perda_sel, *perda_default;

   //verifica opção
   if (opcao.perda_trafo_sed == opTRAFO_GIS) return(NULL);
   //proteção
   if (lisPerdaTrafoSED->Count == 0) {IniciaPerdasTrafoSED();}
   //determina o PERDA_TRAFO_SED com Snom menor e mais próximo se Snom indicado
   perda_sel     = NULL;
   perda_default = NULL;
   for (int n = 0; n < lisPerdaTrafoSED->Count; n++)
      {
      perda = (PERDA_TRAFO_SED*)lisPerdaTrafoSED->Items[n];
      //verifica se snom_mva nulo ou  negativo
      if (perda->snom_mva <= 0)
         {//salva PERDA_TRAFO_SED default
         perda_default = perda;
         continue;
         }
      //verifica se snom_mva é menor do que o perda->snom_mva
      if (snom_mva <= perda->snom_mva)
         {//determina erro
         erro = perda->snom_mva - snom_mva;
         if (perda_sel == NULL)
            {
            perda_sel = perda;
            erro_max = erro;
            }
         else if (erro < erro_max)
            {
            perda_sel = perda;
            erro_max  = erro;
            }
         }
      }
   //verifica se foi selecionado um PERDA_TRAFO_SED
   if (perda_sel == NULL)
      {//assume PERDA_TRAFO_SED default
      perda_sel = perda_default;
      }
   return(perda_sel);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaOpcaoValorUnico(void)
   {
   //assume que o primeiro PERDA_MEDIDOR de lisPerdaMedidor é o padrão único selecionado
   if (lisPerdaMedidor->Count == 0) unico.perda_medidor = NULL;
   else                             unico.perda_medidor = (PERDA_MEDIDOR*)lisPerdaMedidor->First();
   //assume que o primeiro PERDA_RAMAL de lisPerdaRamal é o padrão único selecionado
   if (lisPerdaRamal->Count == 0) unico.perda_ramal = NULL;
   else                           unico.perda_ramal = (PERDA_RAMAL*)(lisPerdaRamal->First());
   //assume que o primeiro PERDA_TRAFO de lisPerdaTrafoProdist é o padrão único selecionado
   if (lisPerdaTrafoProdist->Count == 0) unico.perda_trafo = NULL;
   else                                  unico.perda_trafo = (PERDA_TRAFO*)(lisPerdaTrafoProdist->First());
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaPerdasCabo(void)
   {
   //variáveis locais
   PERDA_CABO *perda_cabo;

   //verifica se lisPerdaCabo não está está preenchida
   if (lisPerdaCabo->Count == 0)
      {//loop para todos elementos em cfg_ramal
      for (int n = 0; n < sizeof(cfg_cabo)/sizeof(strCFG_CABO); n++)
         {//cria um novo objeto PERDA_CABO
         lisPerdaCabo->Add(perda_cabo = new PERDA_CABO);
         perda_cabo->id       = cfg_cabo[n].id;
         perda_cabo->material = cfg_cabo[n].material;
         perda_cabo->codigo   = cfg_cabo[n].codigo;
         perda_cabo->r_ohm_km = cfg_cabo[n].r_ohm_km;
         }
      //seta indicação de alteração
      PD.alterada = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaPerdasMedidor(void)
   {
   //variáveis locais
   PERDA_MEDIDOR *perda_medidor;

   //verifica se lisPerdaMedidor não está está preenchida
   if (lisPerdaMedidor->Count == 0)
      {//cria PERDA_MEDIDOR eletrônico
      lisPerdaMedidor->Add(perda_medidor = new PERDA_MEDIDOR);
      perda_medidor->codigo   = "Eletrônico";
      perda_medidor->perda_w  = 0.5;
      //cria PERDA_MEDIDOR eletromecânico
      lisPerdaMedidor->Add(perda_medidor = new PERDA_MEDIDOR);
      perda_medidor->codigo   = "Eletro-mecânico";
      perda_medidor->perda_w  = 1.0;
      //cria PERDA_MEDIDOR específico
      lisPerdaMedidor->Add(perda_medidor = new PERDA_MEDIDOR);
      perda_medidor->codigo   = "Alternativo";
      perda_medidor->perda_w  = 0.8;
      //seta indicação de alteração
      PD.alterada = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaPerdasRamal(void)
   {
   //variáveis locais
   PERDA_RAMAL *perda_ramal;

   //verifica se lisPerdaRamal não está está preenchida
   if (lisPerdaRamal->Count == 0)
      {//loop para todos elementos em cfg_ramal
      for (int n = 0; n < sizeof(cfg_ramal)/sizeof(strCFG_RML); n++)
         {//cria um novo objeto PERDA_TRAFO
         lisPerdaRamal->Add(perda_ramal = new PERDA_RAMAL);
         perda_ramal->vmin_kv       = cfg_ramal[n].vmin_kv;
         perda_ramal->vmax_kv       = cfg_ramal[n].vmax_kv;
         perda_ramal->num_fases     = cfg_ramal[n].num_fases;
         perda_ramal->cabo          = NULL;
         perda_ramal->comp_m        = cfg_ramal[n].comp_m;
         perda_ramal->r_ohm_km      = cfg_ramal[n].r_ohm_km;
         }
      //seta indicação de alteração
      PD.alterada = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaPerdasTrafo(void)
   {
   //verifica se lisPerdaTrafoProdist já está preenchida
   if (lisPerdaTrafoProdist->Count == 0)
      {
      IniciaPerdasTrafo(trfMONOFASICO, prodist_trf_mono, sizeof(prodist_trf_mono)/sizeof(strCFG_TRF), lisPerdaTrafoProdist);
      IniciaPerdasTrafo(trfTRIFASICO,  prodist_trf_trif, sizeof(prodist_trf_trif)/sizeof(strCFG_TRF), lisPerdaTrafoProdist);
      //seta indicação de alteração
      PD.alterada = true;
      }
   //verifica se lisPerdaTrafoEmpresa já está preenchida
   if (lisPerdaTrafoEmpresa->Count == 0)
      {
      IniciaPerdasTrafo(trfMONOFASICO, empresa_trf_mono, sizeof(empresa_trf_mono)/sizeof(strCFG_TRF), lisPerdaTrafoEmpresa);
      IniciaPerdasTrafo(trfTRIFASICO,  empresa_trf_trif, sizeof(empresa_trf_trif)/sizeof(strCFG_TRF), lisPerdaTrafoEmpresa);
      //seta indicação de alteração
      PD.alterada = true;
      }
   //verifica se lisPerdaTrafoSED já está preenchida
   if (lisPerdaTrafoSED->Count == 0)
      {
      IniciaPerdasTrafoSED();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaPerdasTrafo(int tipo, strCFG_TRF *cfg_trf, int count, TList *lisPerdaTrafo)
   {
   //variáveis locais
   PERDA_TRAFO *perda_trafo;

   //loop para todos PERDA_TRAFO em lisPerdaTrafo
   for (int n = 0; n < count; n++)
      {//cria um novo objeto PERDA_TRAFO
      lisPerdaTrafo->Add(perda_trafo = new PERDA_TRAFO);
      perda_trafo->tipo          = tipo;
      perda_trafo->vmin_kv       = cfg_trf[n].vmin_kv;
      perda_trafo->vmax_kv       = cfg_trf[n].vmax_kv;
      perda_trafo->snom_kva      = cfg_trf[n].snom_kva;
      perda_trafo->perda_vazio_w = cfg_trf[n].perda_vazio_w;
      perda_trafo->perda_total_w = cfg_trf[n].perda_total_w;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaPerdasTrafoSED(void)
   {
   //variáveis locais
   PERDA_TRAFO_SED *perda_trafo_sed;
   int             count = sizeof(trf_sed)/sizeof(strCFG_TRF_SED);

   //loop para todos PERDA_TRAFO em strCFG_TRF_SED
   for (int n = 0; n < count; n++)
      {//cria um novo objeto PERDA_TRAFO
      lisPerdaTrafoSED->Add(perda_trafo_sed = new PERDA_TRAFO_SED);
      perda_trafo_sed->snom_mva         = trf_sed[n].snom_mva;
      perda_trafo_sed->perda_vazio_perc = trf_sed[n].perda_vazio_perc;
      perda_trafo_sed->perda_total_perc = trf_sed[n].perda_total_perc;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaConfiguracaoGis(int opcao)
   {
   //cálculo de fluxo
   fluxo.tipo          = fluxoDESEQUILIBRADO;
   fluxo.modelo_carga  = cargaAUTO;
   fluxo.com_neutro    = true;
   fluxo.com_capacitor = true;
   fluxo.zat_mrt       = false;
   fluxo.zat_ohm       = 0.;
   //configura conforme opção
   switch(opcao)
      {
      case 1:
         //não inclui perda para medidor, ramal
         OpcaoPerdaMedidor  = opMEDIDOR_NAO_USAR;
         OpcaoPerdaRamal    = opRAMAL_NAO_USAR;
         //perda no trafo conforme cadastro no GIS
         OpcaoPerdaTrafo    = opTRAFO_GIS;
         OpcaoPerdaTrafoSED = opTRAFO_GIS;
         //cálculo por fora
         FerroPorFora   = true;
         MedidorPorFora = true;
         RamalPorFora   = true;
         //perdas adicionais
         PerdaAdicional_perc = 0.0;
         break;
      default:
         //inclui perda para medidor, ramal
         OpcaoPerdaMedidor  = opMEDIDOR_UNICO;
         OpcaoPerdaRamal    = opRAMAL_UNICO;
         //perda no trafo conforme cadastro no GIS
         OpcaoPerdaTrafo    = opTRAFO_GIS;
         OpcaoPerdaTrafoSED = opTRAFO_GIS;
         //cálculo por fora
         FerroPorFora   = false;
         MedidorPorFora = false;
         RamalPorFora   = false;
         //perdas adicionais
         PerdaAdicional_perc = 5.0;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::IniciaConfiguracaoProdist(void)
   {
   //cálculo de fluxo
   fluxo.tipo          = fluxoDESEQUILIBRADO;
   fluxo.modelo_carga  = cargaANEEL;
   fluxo.com_neutro    = false;
   fluxo.com_capacitor = false;
   fluxo.zat_mrt       = true;
   fluxo.zat_ohm       = 15.0;
   //configura opção de perda para medidor, ramal e trafo
   OpcaoPerdaMedidor   = opMEDIDOR_UNICO;
   OpcaoPerdaRamal     = opRAMAL_UNICO;
   OpcaoPerdaTrafo     = opTRAFO_PRODIST;
   OpcaoPerdaTrafoSED  = opTRAFO_EMPRESA;
   //cálculo por fora
   FerroPorFora   = false;
   MedidorPorFora = false;
   RamalPorFora   = false;
   //perdas adicionais
   PerdaAdicional_perc = 5.0;
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::LeCfgPerdas(void)
   {
   //cria objeto VTInfoSet
   if ((infoset = NewObjInfoset(apl)) != NULL)
      {
      LeCfgPerdasCabo();
      LeCfgPerdasMedidor();
      LeCfgPerdasRamal();
      LeCfgPerdasTrafo(ARQ_CFG_PERDA_TRAFO_EMPRESA, lisPerdaTrafoEmpresa);
      LeCfgPerdasTrafo(ARQ_CFG_PERDA_TRAFO_PRODIST, lisPerdaTrafoProdist);
      LeCfgPerdasTrafoSED(ARQ_CFG_PERDA_TRAFO_SED,  lisPerdaTrafoSED);
      //destrói objeto infoset
      delete infoset;
      }
  }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::LeCfgPerdasCabo(void)
   {
   //variáveis locais
   PERDA_CABO *perda_cabo;

   //proteção
   if (infoset == NULL) return;
   //abre arquivo com dados de configuração de perdas dos cabos
   if (! infoset->LoadFromFile(ARQ_CFG_PERDA_CABO)) return;
   //verifica se há dados disponíveis
   if (infoset->Count == 0) return;
   //reinicia lisPerdaCabo
   LimpaTList(lisPerdaCabo);
   //lê todos os registros do Infoset
   for (int index = 0; index < infoset->Count; index++)
      {
      if ( ! infoset->SelectLine(index)) return;
      //cria um novo objeto PERDA_CABO
      lisPerdaCabo->Add(perda_cabo = new PERDA_CABO);
      infoset->GetInfo("id"      ,  perda_cabo->id);
      infoset->GetInfo("material",  perda_cabo->material);
      infoset->GetInfo("codigo"  ,  perda_cabo->codigo);
      infoset->GetInfo("r_ohm_km",  perda_cabo->r_ohm_km);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::LeCfgPerdasMedidor(void)
   {
   //variáveis locais
   PERDA_MEDIDOR *perda_medidor;

   //proteção
   if (infoset == NULL) return;
   //abre arquivo com dados de configuração de perdas dos medidores
   if (! infoset->LoadFromFile(ARQ_CFG_PERDA_MEDIDOR)) return;
   //verifica se há dados disponíveis
   if (infoset->Count == 0) return;
   //reinicia lisPerdaCabo
   LimpaTList(lisPerdaMedidor);
   //lê todos os registros do Infoset
   for (int index = 0; index < infoset->Count; index++)
      {
      if ( ! infoset->SelectLine(index)) return;
      //cria um novo objeto PERDA_MEDIDOR
      lisPerdaMedidor->Add(perda_medidor = new PERDA_MEDIDOR);
      infoset->GetInfo("codigo" ,  perda_medidor->codigo);
      infoset->GetInfo("perda_w",  perda_medidor->perda_w);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::LeCfgPerdasRamal(void)
   {
   //variáveis locais
   PERDA_RAMAL *perda_ramal;

   //proteção
   if (infoset == NULL) return;
   //abre arquivo com dados de configuração de perdas de ramal
   if (! infoset->LoadFromFile(ARQ_CFG_PERDA_RAMAL)) return;
   //verifica se há dados disponíveis
   if (infoset->Count == 0) return;
   //reinicia lisPerdaRamal
   LimpaTList(lisPerdaRamal);
   //lê todos os registros do Infoset
   for (int index = 0; index < infoset->Count; index++)
      {
      if ( ! infoset->SelectLine(index)) return;
      //cria um novo objeto PERDA_RAMAL
      lisPerdaRamal->Add(perda_ramal = new PERDA_RAMAL);
      infoset->GetInfo("vmin_kv"  , perda_ramal->vmin_kv  );
      infoset->GetInfo("vmax_kv"  , perda_ramal->vmax_kv  );
      infoset->GetInfo("num_fases", perda_ramal->num_fases);
      infoset->GetInfo("comp_m"   , perda_ramal->comp_m   );
      infoset->GetInfo("r_ohm_km",  perda_ramal->r_ohm_km );
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::LeCfgPerdasTrafo(AnsiString ArqCfgPerdaTrafo, TList *lisPerdaTrafo)
   {
   //variáveis locais
   PERDA_TRAFO *perda_trafo;

   //proteção
   if (infoset == NULL) return;
   //abre arquivo com dados de configuração de perdas de trafo
   if (! infoset->LoadFromFile(ArqCfgPerdaTrafo)) return;
   //verifica se há dados disponíveis
   if (infoset->Count == 0) return;
   //reinicia lisPerdaRamal
   LimpaTList(lisPerdaTrafo);
   //lê todos os registros do Infoset
   for (int index = 0; index < infoset->Count; index++)
      {
      if ( ! infoset->SelectLine(index)) return;
      //cria um novo objeto PERDA_TRAFO
      lisPerdaTrafo->Add(perda_trafo = new PERDA_TRAFO);
      infoset->GetInfo("tipo"         , perda_trafo->tipo);
      infoset->GetInfo("vmin_kv"      , perda_trafo->vmin_kv);
      infoset->GetInfo("vmax_kv"      , perda_trafo->vmax_kv);
      infoset->GetInfo("snom_kva"     , perda_trafo->snom_kva);
      infoset->GetInfo("perda_vazio_w", perda_trafo->perda_vazio_w);
      infoset->GetInfo("perda_total_w", perda_trafo->perda_total_w);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::LeCfgPerdasTrafoSED(AnsiString ArqCfgPerdaTrafoSED, TList *lisPerdaTrafoSED)
   {
   //variáveis locais
   PERDA_TRAFO_SED *perda_trafo_sed;

   //proteção
   if (infoset == NULL) return;
   //abre arquivo com dados de configuração de perdas de trafo
   if (! infoset->LoadFromFile(ArqCfgPerdaTrafoSED)) return;
   //verifica se há dados disponíveis
   if (infoset->Count == 0) return;
   //reinicia lisPerdaTrafoSED
   LimpaTList(lisPerdaTrafoSED);
   //lê todos os registros do Infoset
   for (int index = 0; index < infoset->Count; index++)
      {
      if ( ! infoset->SelectLine(index)) return;
      //cria um novo objeto PERDA_TRAFO_SED
      lisPerdaTrafoSED->Add(perda_trafo_sed = new PERDA_TRAFO_SED);
      infoset->GetInfo("snom_mva"        , perda_trafo_sed->snom_mva);
      infoset->GetInfo("perda_vazio_perc", perda_trafo_sed->perda_vazio_perc);
      infoset->GetInfo("perda_total_perc", perda_trafo_sed->perda_total_perc);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TCfgPerdaTec::PM_GetLisPerdasCabo(void)
   {
   return(lisPerdaCabo);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCfgPerdaTec::PM_GetLisPerdasMedidor(void)
   {
   return(lisPerdaMedidor);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCfgPerdaTec::PM_GetLisPerdasRamal(void)
   {
   //verifica se a lista de perdas em ramal já foi iniciada
   if (lisPerdaRamal->Count == 0)
      {//inicia lista
      IniciaPerdasRamal();
      }
   return(lisPerdaRamal);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCfgPerdaTec::PM_GetLisPerdasTrafoEmpresa(void)
   {
   return(lisPerdaTrafoEmpresa);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCfgPerdaTec::PM_GetLisPerdasTrafoProdist(void)
   {
   return(lisPerdaTrafoProdist);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCfgPerdaTec::PM_GetLisPerdasTrafoSED(void)
   {
   return(lisPerdaTrafoSED);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::SalvaCfgPerdas(void)
   {
   //cria objeto VTInfoSet
   if ((infoset = NewObjInfoset(apl)) != NULL)
      {
      SalvaCfgPerdasCabo();
      SalvaCfgPerdasMedidor();
      SalvaCfgPerdasRamal();
      SalvaCfgPerdasTrafo(ARQ_CFG_PERDA_TRAFO_EMPRESA, lisPerdaTrafoEmpresa);
      SalvaCfgPerdasTrafo(ARQ_CFG_PERDA_TRAFO_PRODIST, lisPerdaTrafoProdist);
      SalvaCfgPerdasTrafoSED(ARQ_CFG_PERDA_TRAFO_SED,  lisPerdaTrafoSED);
      //destrói objeto infoset
      delete infoset;
      }
  }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::SalvaCfgPerdasCabo(void)
   {
   //variáveis locais
   PERDA_CABO *perda_cabo;

   //proteção
   if (infoset == NULL) return;
   infoset->Clear();
   //salva todos os objetos em Infoset
   for (int n = 0; n < lisPerdaCabo->Count; n++)
      {
      perda_cabo = (PERDA_CABO*)lisPerdaCabo->Items[n];
      //insere nova linha no infoset
      if ( ! infoset->InsertNewLine()) return;
      infoset->AddInfo("id"      ,  perda_cabo->id);
      infoset->AddInfo("material",  perda_cabo->material);
      infoset->AddInfo("codigo"  ,  perda_cabo->codigo);
      infoset->AddInfo("r_ohm_km",  perda_cabo->r_ohm_km);
      }
   //salva arquivo com dados de configuração de perdas dos cabos
   infoset->SaveToFile(ARQ_CFG_PERDA_CABO);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::SalvaCfgPerdasMedidor(void)
   {
   //variáveis locais
   PERDA_MEDIDOR *perda_medidor;

   //proteção
   if (infoset == NULL) return;
   infoset->Clear();
   //salva todos os objetos em Infoset
   for (int n = 0; n < lisPerdaMedidor->Count; n++)
      {
      perda_medidor = (PERDA_MEDIDOR*)lisPerdaMedidor->Items[n];
      //insere nova linha no infoset
      if ( ! infoset->InsertNewLine()) return;
      infoset->AddInfo("codigo" ,  perda_medidor->codigo);
      infoset->AddInfo("perda_w",  perda_medidor->perda_w);
      }
   //salva arquivo com dados de configuração de perdas dos medidores
   infoset->SaveToFile(ARQ_CFG_PERDA_MEDIDOR);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::SalvaCfgPerdasRamal(void)
   {
   //variáveis locais
   PERDA_RAMAL *perda_ramal;

   //proteção
   if (infoset == NULL) return;
   infoset->Clear();
   //salva todos os objetos em Infoset
   for (int n = 0; n < lisPerdaRamal->Count; n++)
      {
      perda_ramal = (PERDA_RAMAL*)lisPerdaRamal->Items[n];
      //insere nova linha no infoset
      if ( ! infoset->InsertNewLine()) return;
      infoset->AddInfo("vmin_kv"  , perda_ramal->vmin_kv  );
      infoset->AddInfo("vmax_kv"  , perda_ramal->vmax_kv  );
      infoset->AddInfo("num_fases", perda_ramal->num_fases);
      infoset->AddInfo("comp_m"   , perda_ramal->comp_m   );
      infoset->AddInfo("r_ohm_km" ,  perda_ramal->r_ohm_km );
      }
   //salva arquivo com dados de configuração de perdas dos medidores
   infoset->SaveToFile(ARQ_CFG_PERDA_RAMAL);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::SalvaCfgPerdasTrafo(AnsiString ArqCfgPerdaTrafo, TList *lisPerdaTrafo)
   {
   //variáveis locais
   PERDA_TRAFO *perda_trafo;

   //proteção
   if (infoset == NULL) return;
   infoset->Clear();
   //salva todos os objetos em Infoset
   for (int n = 0; n < lisPerdaTrafo->Count; n++)
      {
      perda_trafo = (PERDA_TRAFO*)lisPerdaTrafo->Items[n];
      //insere nova linha no infoset
      if ( ! infoset->InsertNewLine()) return;
      infoset->AddInfo("tipo"         , perda_trafo->tipo);
      infoset->AddInfo("vmin_kv"      , perda_trafo->vmin_kv);
      infoset->AddInfo("vmax_kv"      , perda_trafo->vmax_kv);
      infoset->AddInfo("snom_kva"     , perda_trafo->snom_kva);
      infoset->AddInfo("perda_vazio_w", perda_trafo->perda_vazio_w);
      infoset->AddInfo("perda_total_w", perda_trafo->perda_total_w);
      }
   //salva arquivo com dados de configuração de perdas dos trafos
   infoset->SaveToFile(ArqCfgPerdaTrafo);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgPerdaTec::SalvaCfgPerdasTrafoSED(AnsiString ArqCfgPerdaTrafoSED, TList *lisPerdaTrafoSED)
   {
   //variáveis locais
   PERDA_TRAFO_SED *perda_trafo_sed;

   //proteção
   if (infoset == NULL) return;
   infoset->Clear();
   //salva todos os objetos em Infoset
   for (int n = 0; n < lisPerdaTrafoSED->Count; n++)
      {
      perda_trafo_sed = (PERDA_TRAFO_SED*)lisPerdaTrafoSED->Items[n];
      //insere nova linha no infoset
      if ( ! infoset->InsertNewLine()) return;
      infoset->AddInfo("snom_mva"        , perda_trafo_sed->snom_mva);
      infoset->AddInfo("perda_vazio_perc", perda_trafo_sed->perda_vazio_perc);
      infoset->AddInfo("perda_total_perc", perda_trafo_sed->perda_total_perc);
      }
   //salva arquivo com dados de configuração de perdas dos trafos
   infoset->SaveToFile(ArqCfgPerdaTrafoSED);
   }

//---------------------------------------------------------------------------
//eof
