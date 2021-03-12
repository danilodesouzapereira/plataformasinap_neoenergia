//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include ".\TData.h"
#include ".\VTResCurto.h"
#include ".\VTResEstimador.h"
#include ".\VTResFluxo.h"
#include ".\Comum13\VTGeralC.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Complexo\Complexo.h"
#include "..\Diretorio\VTPath.h"
#include "..\Eqv\VTEqv.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTCargaNL.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTMatY.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTramo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTYref.h"
#include "..\Zona\VTZona.h"

#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\MontaZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTData
//---------------------------------------------------------------------------
VTData* __fastcall NewObjData(void)
   {
   return(new TData());
   }

//---------------------------------------------------------------------------
__fastcall TData::TData(void)
   {
   // Impedância de default para reguladores e trafos
   // IMPORTANTE: parte real igual a zero na impedância abaixo causa
   //             convergência lenta em algumas redes em NR3. Evitar
   //             zerar este valor. HPS, 2018.09.05.
   zreg_trafo_pu = complex<double>(0.0008, 0.0080); // Na base nominal do eqpto. HPS, 2018.08.17.
   //assume frequencia de 60 Hz e Sbase de 100 MVA
   FrequenciaHz = 60.0;
   Sbase        = 100.;
   //assume que a rede não irá incluir as Chaves
   IncluirChave = false;
   //assume modelo de carga automático (definido por cada carga)
   RedeComCarga = true;
   ModeloCarga  = cargaAUTO;
   //assume perda no ferro desabilitada
   PerdaFerroEnabled = false;
   //cria StringList
   PD.harmonicas = new TStringList();
   PD.relatorios = new TStringList();
   //cria lista p/ armazenar todas as demais listas
   PD.lisLIST  = new TList();
   //cria listas e armazena em lisLIST
   PD.lisLIST->Add(PD.lisATERRAMENTO = new TList());
   PD.lisLIST->Add(PD.lisBATERIA     = new TList());
   PD.lisLIST->Add(PD.lisCAPACITOR   = new TList());
   PD.lisLIST->Add(PD.lisCAPSERIE    = new TList());
   PD.lisLIST->Add(PD.lisCARGA       = new TList());
   PD.lisLIST->Add(PD.lisCHAVE       = new TList());
   PD.lisLIST->Add(PD.lisCNL         = new TList());
   PD.lisLIST->Add(PD.lisFILTRO      = new TList());
   PD.lisLIST->Add(PD.lisGERADOR     = new TList());
   PD.lisLIST->Add(PD.lisLIGACAO     = new TList());
   PD.lisLIST->Add(PD.lisMUTUA       = new TList());
   PD.lisLIST->Add(PD.lisMEDIDOR     = new TList());
   PD.lisLIST->Add(PD.lisNO          = new TList());
   PD.lisLIST->Add(PD.lisREATOR      = new TList());
   PD.lisLIST->Add(PD.lisREGULADOR   = new TList());
   PD.lisLIST->Add(PD.lisSUPRIMENTO  = new TList());
   PD.lisLIST->Add(PD.lisTRAFO2E     = new TList());
   PD.lisLIST->Add(PD.lisTRAFO3E     = new TList());
   PD.lisLIST->Add(PD.lisTRAFOZZ     = new TList());
   PD.lisLIST->Add(PD.lisTRAMO       = new TList());
   PD.lisLIST->Add(PD.lisTRECHO      = new TList());
   PD.lisLIST->Add(PD.lisYREF        = new TList());
   PD.lisLIST->Add(PD.lisEQP         = new TList());
   }

//---------------------------------------------------------------------------
__fastcall TData::~TData(void)
   {
   //destrói objetos
   if (montaZ)        {delete montaZ;        montaZ        = NULL;}
   if (rescurto)      {delete rescurto;      rescurto      = NULL;}
   if (resestimador)  {delete resestimador;  resestimador  = NULL;}
   if (resfluxo)      {delete resfluxo;      resfluxo      = NULL;}
   if (PD.harmonicas) {delete PD.harmonicas; PD.harmonicas = NULL;}
   if (PD.relatorios) {delete PD.relatorios; PD.relatorios = NULL;}
   //destrói lista e todos seus objetos
   if (PD.lisLIST) {LimpaTList(PD.lisLIST); delete PD.lisLIST; PD.lisLIST = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TData::CanalRedundante(VTCanal *canal)
   {
   //variáveis locais
   VTMedidor *medidor = canal->Medidor;

   //verifica se o Medidor possui Canal P e Q para as mesmas fases
   if (medidor->ExisteCanal(canal->Eqpto, canalP, canal->Fases) &&
       medidor->ExisteCanal(canal->Eqpto, canalQ, canal->Fases)) return(true);
   //verifica se o Medidor possui Canal P e Q trifásicos
   if (medidor->ExisteCanal(canal->Eqpto, canalP, faseABC) &&
       medidor->ExisteCanal(canal->Eqpto, canalQ, faseABC)) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TData::DefineNomeRede(void)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se a Net tem código válido
   NomeDaRede = net->Codigo;
   if (NomeDaRede.IsEmpty())
      {
      NomeDaRede = ExtractFileName(path->ArqRede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TData::Demanda(smcDEMANDA *smc_demanda, int modelo_carga, int fases_tag, complex<double> s_mva)
   {
   //variáveis locais
   int    ind_modelo;
   double p_mw, q_mvar;

   //define smcDEMANDA com apenas um valor
   smc_demanda->count = 1;
   smc_demanda->dem[0].fases_int = fases_tag;
   smc_demanda->dem[0].fases_str = fases->AsStrABCN[fases_tag];
   smc_demanda->dem[0].modelo    = modelo_carga;
   switch (modelo_carga)
      {
      case cargaSCTE:
         ind_modelo = Scte;
         smc_demanda->dem[0].s_mva[ind_modelo] = s_mva;
         break;
      case cargaZCTE:
         ind_modelo = Zcte;
         smc_demanda->dem[0].s_mva[ind_modelo] = s_mva;
         break;
      case cargaANEEL: //Pot.Ativa: 50% Scte/50% Zcte, Pot.Reativa: 100% Zcte
         p_mw   = s_mva.real() * 0.5;
         q_mvar = s_mva.imag();
         //preenche parcela Scte: (50% pot.ativa, 0.0)
         ind_modelo = Scte;
         smc_demanda->dem[0].s_mva[ind_modelo] = complex<double> (p_mw, 0.);
         //preenche parcela Zcte: (50% pot.ativa, 100% pot.reativa)
         ind_modelo = Zcte;
         smc_demanda->dem[0].s_mva[ind_modelo] = complex<double> (p_mw, q_mvar);
         break;
      case cargaICTE:
      default:
         ind_modelo = Icte;
         smc_demanda->dem[0].s_mva[ind_modelo] = s_mva;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TData::Demanda(smcDEMANDA *smc_demanda, int modelo_carga, complex<double> sf_mva[MAX_FASE])
   {
   //variáveis locais
   int    ind_modelo;
   int    fases_tag;
   double p_mw, q_mvar;

   //define smcDEMANDA com um valor por fase
   smc_demanda->count = MAX_FASE;
   for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
      {
      fases_tag = fases->IndexToTag[ind_fase];
      smc_demanda->dem[ind_fase].fases_int = fases_tag;
      smc_demanda->dem[ind_fase].fases_str = fases->AsStrABCN[fases_tag];
      smc_demanda->dem[ind_fase].modelo    = modelo_carga;
      switch (modelo_carga)
         {
         case cargaSCTE:
            ind_modelo = Scte;
            smc_demanda->dem[ind_fase].s_mva[ind_modelo] = sf_mva[ind_fase];
            break;
         case cargaZCTE:
            ind_modelo = Zcte;
            smc_demanda->dem[ind_fase].s_mva[ind_modelo] = sf_mva[ind_fase];
            break;
         case cargaANEEL: //Pot.Ativa: 50% Scte/50% Zcte, Pot.Reativa: 100% Zcte
            p_mw   = sf_mva[ind_fase].real() * 0.5;
            q_mvar = sf_mva[ind_fase].imag();
            //preenche parcela Scte: (50% pot.ativa, 0.0)
            ind_modelo = Scte;
            smc_demanda->dem[ind_fase].s_mva[ind_modelo] = complex<double> (p_mw, 0.);
            //preenche parcela Zcte: (50% pot.ativa, 100% pot.reativa)
            ind_modelo = Zcte;
            smc_demanda->dem[ind_fase].s_mva[ind_modelo] = complex<double> (p_mw, q_mvar);
            break;
         case cargaICTE:
         default:
            ind_modelo = Icte;
            smc_demanda->dem[ind_fase].s_mva[ind_modelo] = sf_mva[ind_fase];
            break;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Demanda(VTEqpto *eqpto, int ind_pat, smcDEMANDA *smc_demanda)
   {
   //variáveis locais
   int             modelo_carga;
   int             fases_int, ind_modelo;
   complex<double> s_mva, sf_mva[MAX_FASE];
   VTCapacitor     *capacitor;
   VTCarga         *carga;
   VTReator        *reator;

   //inicia smcDEMANDA
   smc_demanda->count = 0;
   for (int n = 0; n < smc_demanda->capacity; n++)
      {
      smc_demanda->dem[n].fases_int = faseINV;
      smc_demanda->dem[n].fases_str = "";
      smc_demanda->dem[n].s_mva[0]  = CZero();
      smc_demanda->dem[n].s_mva[1]  = CZero();
      smc_demanda->dem[n].s_mva[2]  = CZero();
      }
   //determina demanda do Eqpto
   if (eqpto->Tipo() == eqptoCARGA)
      {
      carga = (VTCarga*)eqpto;
      //verifica se está usando modelo de carga único p/ todas Cargas
      modelo_carga = (ModeloCarga != cargaAUTO) ? ModeloCarga : carga->ModeloCarga;
      // Verifica se carga é definida por demanda total ou demanda por fase
      if (carga->DefinidaPorFase)
         {//obtém vetor de demanda por fase
         if (! demanda->Get_S3f_mva(eqpto, ind_pat, sf_mva)) return(false);
         //preenche smcDEMANDA com valores de cada fase
         Demanda(smc_demanda, modelo_carga, sf_mva);
//teste
//LogDemandaCarga(carga, sf_mva);
         }
      else
         {//obtém demanda total
         s_mva = demanda->S_mva[eqpto][ind_pat];
         //preenche smcDEMANDA com valor total
         Demanda(smc_demanda, modelo_carga, carga->Fases, s_mva);
//teste
//LogDemandaCarga(carga, s_mva);
         }
      return(true);
      }
   if (eqpto->Tipo() == eqptoCAPACITOR)
      {
      capacitor = (VTCapacitor*)eqpto;
      //determina fasess
      if      (capacitor->ligacao == lgEST_AT)  fases_int = faseABCNT; // Yaterr
      else if (capacitor->ligacao == lgEST_ISO) fases_int = faseABCN;  // Yiso
      else                                      fases_int = faseABC;   // Delta
      //determina demanda total
      s_mva = demanda->S_mva[eqpto][ind_pat];
      //preenche smcDEMANDA com valor total da demanda
      Demanda(smc_demanda, cargaZCTE, fases_int, s_mva);
      return(true);
      }
   if (eqpto->Tipo() == eqptoREATOR)
      {
      reator = (VTReator*)eqpto;
      //determina fasess
      if      (reator->ligacao == lgEST_AT)  fases_int = faseABCNT; // Yaterr
      else if (reator->ligacao == lgEST_ISO) fases_int = faseABCN;  // Yiso
      else                                   fases_int = faseABC;   // Delta
      //determina demanda total
      s_mva = demanda->S_mva[eqpto][ind_pat];
      //preenche smcDEMANDA com valor total da demanda
      Demanda(smc_demanda, cargaZCTE, fases_int, s_mva);
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::DemandaPorFase(VTEqpto *eqpto, int ind_pat, smcDEMANDA *smc_demanda)
   {
   //variáveis locais
   int             modelo_carga;
   int             fases_int, ind_modelo;
   complex<double> s_mva, sf_mva[MAX_FASE];
   VTCapacitor     *capacitor;
   VTCarga         *carga;
   VTReator        *reator;

   //inicia smcDEMANDA
   smc_demanda->count = 0;
   for (int n = 0; n < smc_demanda->capacity; n++)
      {
      smc_demanda->dem[n].fases_int = faseINV;
      smc_demanda->dem[n].fases_str = "";
      smc_demanda->dem[n].s_mva[0]  = CZero();
      smc_demanda->dem[n].s_mva[1]  = CZero();
      smc_demanda->dem[n].s_mva[2]  = CZero();
      }
   //determina demanda do Eqpto
   if (eqpto->Tipo() == eqptoCARGA)
      {
      carga = (VTCarga*)eqpto;
      //verifica se está usando modelo de carga único p/ todas Cargas
      modelo_carga = (ModeloCarga != cargaAUTO) ? ModeloCarga : carga->ModeloCarga;
      //obtém vetor de demanda por fase
      if (! demanda->Get_S3f_mva(eqpto, ind_pat, sf_mva)) return(false);
      //preenche smcDEMANDA com valores de cada fase
      Demanda(smc_demanda, modelo_carga, sf_mva);
      return(true);
      }
   if (eqpto->Tipo() == eqptoCAPACITOR)
      {
      //capacitor = (VTCapacitor*)eqpto;
      //obtém vetor de demanda por fase
      if (! demanda->Get_S3f_mva(eqpto, ind_pat, sf_mva)) return(false);
      //preenche smcDEMANDA com valores de cada fase
      Demanda(smc_demanda, cargaZCTE, sf_mva);
      return(true);
      }
   if (eqpto->Tipo() == eqptoREATOR)
      {
      //reator = (VTReator*)eqpto;
      //obtém vetor de demanda por fase
      if (! demanda->Get_S3f_mva(eqpto, ind_pat, sf_mva)) return(false);
      //preenche smcDEMANDA com valores de cada fase
      Demanda(smc_demanda, cargaZCTE, sf_mva);
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TData::DeterminaGrupoLigacao(smcTRAFO3E *smc_trafo3e)
   {
   // Variáveis locais
   int    grupo_ligacao, ligacao, rot12, rot13;
   double rotacao_pri_sec, rotacao_pri_ter;

   // Inicialização
   grupo_ligacao   = glINDEF;
   rotacao_pri_sec = smc_trafo3e->sec.defasagem - smc_trafo3e->pri.defasagem;
   rotacao_pri_ter = smc_trafo3e->ter.defasagem - smc_trafo3e->pri.defasagem;

   // Calcula defasagens como valores inteiros
   if     (fabs(rotacao_pri_sec + 60.) < 0.0001) rot12 = -60;
   else if(fabs(rotacao_pri_sec + 30.) < 0.0001) rot12 = -30;
   else if(fabs(rotacao_pri_sec)       < 0.0001) rot12 =   0;
   else if(fabs(rotacao_pri_sec - 30.) < 0.0001) rot12 =  30;
   else                                          rot12 =  60;

   if     (fabs(rotacao_pri_ter + 60.) < 0.0001) rot13 = -60;
   else if(fabs(rotacao_pri_ter + 30.) < 0.0001) rot13 = -30;
   else if(fabs(rotacao_pri_ter)       < 0.0001) rot13 =   0;
   else if(fabs(rotacao_pri_ter - 30.) < 0.0001) rot13 =  30;
   else                                          rot13 =  60;

   // Passo 1: descobre ligação dos enrolamentos (D ou Y, sem considerar defasagens)
   ligacao = 0;
   if (smc_trafo3e->pri.ligacao == lgTRIANG) ligacao  = 1;
   if (smc_trafo3e->sec.ligacao == lgTRIANG) ligacao += 2;
   if (smc_trafo3e->ter.ligacao == lgTRIANG) ligacao += 4;

   // Passo 2: descobre o grupo de ligação levando em conta as defasagens
   switch(ligacao)
      {
      case 0:  // YYY
         grupo_ligacao = glYY0Y0;  // 07
         break;

      case 1:  // DYY
         if     ((rot12 == -30) && (rot13 == -30)) grupo_ligacao = glDY1Y1;   // 03
         else if((rot12 == -30) && (rot13 ==  30)) grupo_ligacao = glDY1Y11;  // 04
         else if((rot12 ==  30) && (rot13 == -30)) grupo_ligacao = glDY11Y1;  // 05
         else if((rot12 ==  30) && (rot13 ==  30)) grupo_ligacao = glDY11Y11; // 06
         break;

      case 2:  // YDY
         if     ((rot12 ==  30) && (rot13 ==   0)) grupo_ligacao = glYD11Y0;  // 13
         else if((rot12 == -30) && (rot13 == -60)) grupo_ligacao = glYD1Y2;   // 15
         else if((rot12 ==  30) && (rot13 ==  60)) grupo_ligacao = glYD11Y10; // 17
         else if((rot12 == -30) && (rot13 ==   0)) grupo_ligacao = glYD1Y0;   // 19
         break;

      case 3:  // DDY
         if     ((rot12 ==   0) && (rot13 == -30)) grupo_ligacao = glDD0Y1;   // 01
         else if((rot12 ==   0) && (rot13 ==  30)) grupo_ligacao = glDD0Y11;  // 02
         break;

      case 4:  // YYD
         if     ((rot12 ==   0) && (rot13 ==  30)) grupo_ligacao = glYY0D11;  // 12
         else if((rot12 ==  60) && (rot13 ==  30)) grupo_ligacao = glYY10D11; // 14
         else if((rot12 == -60) && (rot13 == -30)) grupo_ligacao = glYY2D1;   // 16
         else if((rot12 ==   0) && (rot13 == -30)) grupo_ligacao = glYY0D1;   // 18
         break;

      case 5:  // DYD
         if     ((rot12 == -30) && (rot13 ==   0)) grupo_ligacao = glDY1D0;   // 08
         else if((rot12 ==  30) && (rot13 ==   0)) grupo_ligacao = glDY11D0;  // 10
         break;

      case 6:  // YDD
         if     ((rot12 ==  30) && (rot13 ==  30)) grupo_ligacao = glYD11D11; // 09
         else if((rot12 == -30) && (rot13 == -30)) grupo_ligacao = glYD1D1;   // 11
         break;

      default:  // 7 = DDD: nada a fazer
         break;
      }

return(grupo_ligacao);
}

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoAterramento(int index, smcATERRAMENTO *smc_ater)
   {
   // Variáveis locais

   // Proteção
   if(index >= PD.lisATERRAMENTO->Count)
      {
      smc_ater->barraCALC = NULL;
      smc_ater->z_ohm     = complex<double>(0., 0.);
      return(false);
      }

   // A implementar...


   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoBarra(int index, smcBARRA *smc_barra, void *barraCALC)
   {
   //variáveis locais
   VTBarra *barra;
   VTNo    *no;

   //proteção
   if (index >= PD.lisNO->Count)
      {
      smc_barra->eqpto = NULL;
      smc_barra->id    =-1;
      return(false);
      }
   //determina No e sua Barra pai
   no     = (VTNo*)PD.lisNO->Items[index];
   barra  = no->pai;
   //salva no No o ponteiro para a barra de cálculo
   no->Obj = (TObject*)barraCALC;
   //devolve ponteiro para objeto No
   smc_barra->eqpto         = no;
   //copia dados da Barra pai do No p/ smcBARRA
   smc_barra->codigo        = barra->Codigo;
   smc_barra->id            = barra->Id;
   smc_barra->fases         = barra->No[net]->Fases;
   smc_barra->tipo_zona     = barra->zona->Tipo;
   smc_barra->vff_kv        = barra->zona->Vff_kv;
   smc_barra->van_kv        = barra->zona->Vfn_kv[indFASE_A];
   smc_barra->vbn_kv        = barra->zona->Vfn_kv[indFASE_B];
   smc_barra->vcn_kv        = barra->zona->Vfn_kv[indFASE_C];

   //smc_barra->fase_zona_rad = barra->zona->T_rad;  // Implementação antiga
   smc_barra->fase_zona_rad[indFASE_A] = barra->zona->T_rad[indFASE_A];
   smc_barra->fase_zona_rad[indFASE_B] = barra->zona->T_rad[indFASE_B];
   smc_barra->fase_zona_rad[indFASE_C] = barra->zona->T_rad[indFASE_C];
   smc_barra->fase_zona_rad[indNEUTRO] = barra->zona->T_rad[indNEUTRO];
   smc_barra->mantida                  = (! no->redutivel);
   // SUP_EQV - Default temporário (2014.08.12)
   smc_barra->sup_equivalente          = false;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoBateria(int index, smcBATERIA *smc_bateria)
   {
   // Variáveis locais
   VTBarra   *barra;
   VTBateria *bateria;

   // Proteção
   if(index >= PD.lisBATERIA->Count)
      {
      smc_bateria->eqpto = NULL;
      smc_bateria->id    = -1;
      return(false);
      }

   // Determina Bateria solicitada
   bateria = (VTBateria *) PD.lisBATERIA->Items[index];

   //copia dados da Bateria para smcBATERIA
   smc_bateria->eqpto     = bateria;
   smc_bateria->codigo    = bateria->Codigo;
   smc_bateria->id        = bateria->Id;
   smc_bateria->barraCALC = bateria->pbarra->No[net]->Obj;
   smc_bateria->fases     = bateria->Fases;
   if(bateria->Tipo_operacao == toMANUAL)     smc_bateria->tipo_operacao = topMANUAL;
   else                                       smc_bateria->tipo_operacao = topAUTO;
   if     (bateria->ModeloCarga == cargaICTE) smc_bateria->modelo_carga  = Icte;
   else if(bateria->ModeloCarga == cargaSCTE) smc_bateria->modelo_carga  = Scte;
   else if(bateria->ModeloCarga == cargaZCTE) smc_bateria->modelo_carga  = Zcte;
   else                                       smc_bateria->modelo_carga  = Scte; // Default
   if     (bateria->ModeloDesc == cargaICTE)  smc_bateria->modelo_desc   = Icte;
   else if(bateria->ModeloDesc == cargaSCTE)  smc_bateria->modelo_desc   = Scte;
   else if(bateria->ModeloDesc == cargaZCTE)  smc_bateria->modelo_desc   = Zcte;
   else                                       smc_bateria->modelo_desc   = Scte; // Default
   smc_bateria->pnom_kw          = bateria->Pnom_kw;
   smc_bateria->enom_kwh         = bateria->Enom_kwh;
   smc_bateria->scap_inv_kva     = bateria->Cap_inv_kva;
   smc_bateria->en_armaz_inic_pu = bateria->Earmaz_inic_pu;
   smc_bateria->pext_carga_kw    = bateria->Pext_carga_kw;
   smc_bateria->qext_carga_kvar  = bateria->Qext_carga_kvar;
   smc_bateria->pext_desc_kw     = bateria->Pext_desc_kw;
   smc_bateria->qext_desc_kvar   = bateria->Qext_desc_kvar;
   smc_bateria->p_ociosa_pu      = bateria->P_ociosa_pu;
   smc_bateria->q_ociosa_pu      = bateria->Q_ociosa_pu;
   smc_bateria->rend_carga_pu    = bateria->Rend_carga_pu;
   smc_bateria->rend_desc_pu     = bateria->Rend_desc_pu;
   smc_bateria->gatilho_carga_pu = bateria->Gatilho_carga_pu;
   smc_bateria->gatilho_desc_pu  = bateria->Gatilho_desc_pu;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoCapacitor(int index, smcCAPACITOR *smc_capacitor)
   {
   //variáveis locais
   VTCapacitor *capacitor;

   //proteção
   if (index >= PD.lisCAPACITOR->Count)
      {
      smc_capacitor->eqpto = NULL;
      smc_capacitor->id    =-1;
      return(false);
      }
   //determina Capacitor solicitado
   capacitor = (VTCapacitor*)PD.lisCAPACITOR->Items[index];
   //copia dados do Capacitor p/ smcCAPACITOR
   smc_capacitor->eqpto     = capacitor;
   smc_capacitor->codigo    = capacitor->Codigo;
   smc_capacitor->id        = capacitor->Id;
   smc_capacitor->barraCALC = capacitor->pbarra->No[net]->Obj;
   smc_capacitor->ligacao   = capacitor->ligacao;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoCapserie(int index, smcCAPSERIE *smc_capserie)
   {
   //variáveis locais
   VTCapserie *capserie;

   //proteção
   if (index >= PD.lisCAPSERIE->Count)
      {
      smc_capserie->eqpto = NULL;
      smc_capserie->id    =-1;
      return(false);
      }
   //determina Capserie solicitado
   capserie = (VTCapserie*)PD.lisCAPSERIE->Items[index];
   //copia dados do Capserie p/ smcCAPSERIE
   smc_capserie->eqpto       = capserie;
   smc_capserie->codigo      = capserie->Codigo;
   smc_capserie->id          = capserie->Id;
   smc_capserie->barraCALC_1 = capserie->pbarra1->No[net]->Obj;
   smc_capserie->barraCALC_2 = capserie->pbarra2->No[net]->Obj;
   smc_capserie->r_ohm       = capserie->z.r;
   smc_capserie->x_ohm       = capserie->z.x;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoCarga(int index, smcCARGA *smc_carga)
   {
   //variáveis locais
   VTCarga *carga;

   //proteção
   if (index >= PD.lisCARGA->Count)
      {
      smc_carga->eqpto = NULL;
      smc_carga->id    =-1;
      return(false);
      }
   //determina Carga solicitada
   carga = (VTCarga*)PD.lisCARGA->Items[index];
   //copia dados da Carga p/ smcCARGA
   smc_carga->eqpto     = carga;
   smc_carga->codigo    = carga->Codigo;
   smc_carga->id        = carga->Id;
   smc_carga->barra_id  = carga->pbarra->No[net]->pai->Id; // Remover após acerto de FluxoH3
   smc_carga->barraCALC = carga->pbarra->No[net]->Obj;
   smc_carga->fases     = carga->Fases;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoChave(int index, smcCHAVE *smc_chave)
   {
   //variáveis locais
   complex<double> z_ohm = complex<double>(0., 4.e-4);
   double          Vbase, Zbase;
   VTChave         *chave;

   //proteção
   if (index >= PD.lisCHAVE->Count)
      {
      smc_chave->eqpto = NULL;
      smc_chave->id    =-1;
      return(false);
      }
   //determina Chave solicitada
   chave = (VTChave*)PD.lisCHAVE->Items[index];
   //proteção
   if (chave->pbarra1->No[net]->pai->zona == NULL) return(false);
   //determina Zbase
   Vbase = chave->pbarra1->No[net]->pai->zona->Vff_kv;
   Zbase = (Vbase * Vbase) / Sbase;
   //copia dados de Chave p/ smcCHAVE
   smc_chave->eqpto       = chave;
   smc_chave->codigo      = chave->Codigo;
   smc_chave->id          = chave->Id;
   smc_chave->barraCALC_1 = chave->pbarra1->No[net]->Obj;
   smc_chave->barraCALC_2 = chave->pbarra2->No[net]->Obj;
   smc_chave->corn_amp    = chave->Inom_amp;
   smc_chave->estado      = (chave->Aberta) ? 0 : 1;
   //smc_chave->fases       = faseABC;
   smc_chave->fases       = chave->pbarra1->No[net]->pai->zona->Fases;
   //define impedâncias de sequencia
   smc_chave->z1_pu       = z_ohm / Zbase;
   smc_chave->z0_pu       = smc_chave->z1_pu;
   //zera matriz de impedância
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         smc_chave->matZ[i][j] = CZero();
         }
      }
   //preenche diagonal da matriz de impedância (inclusive o neutro)
   for (int i = 0; i < MAX_FASE; i++)
      {
      smc_chave->matZ[i][i] = z_ohm;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoCNL(int index, smcCNL *smc_cnl)
   {
   // ATENÇÃO: Na frequência fundamental, as magnitudes em 'smc_cnl' deverão
   //          estar em kVA; nas demais frequências, deverão estar em
   //          pu da fundamental

   // Variáveis locais
   int       ind;
   strFUN    *str_fun;
   strHRM    *str_hrm;
   VTCargaNL *cargaNL;

   // Proteção
   if (index >= PD.lisCNL->Count)
      {
      smc_cnl->eqpto = NULL;
      smc_cnl->id    =-1;
      return(false);
      }

   // Determina CNL solicitada
   cargaNL = (VTCargaNL *) PD.lisCNL->Items[index];

   // Copia dados de CNL p/ smcCNL
   smc_cnl->eqpto          = cargaNL;
   smc_cnl->codigo         = cargaNL->Codigo;
   smc_cnl->id             = cargaNL->Id;
   smc_cnl->barraCALC      = cargaNL->pbarra->No[net]->Obj;
   smc_cnl->num_harmonicas = cargaNL->NumHarmonicas + 1; // Inclui a fundamental
   smc_cnl->cos_fi         = cargaNL->FatPot_pu;

   // Transfere dados da fundamental
   str_fun = cargaNL->LeFundamental();
   smc_cnl->ordem[0]       = 1;
   smc_cnl->pot_kva[0][0]  = str_fun->sa_mod_kva;
   smc_cnl->ang_grau[0][0] = str_fun->sa_ang_grau;
   smc_cnl->pot_kva[0][1]  = str_fun->sb_mod_kva;
   smc_cnl->ang_grau[0][1] = str_fun->sb_ang_grau;
   smc_cnl->pot_kva[0][2]  = str_fun->sc_mod_kva;
   smc_cnl->ang_grau[0][2] = str_fun->sc_ang_grau;
   smc_cnl->pot_kva[0][3]  = 0.;
   smc_cnl->ang_grau[0][3] = 0.;

   // Transfere dados das harmônicas
   for(int nh=0; nh < cargaNL->NumHarmonicas; nh++)
      {
      str_hrm                   = cargaNL->LeHarmonica(nh);
      ind                       = nh + 1;
      smc_cnl->ordem[ind]       = str_hrm->ordem;
      smc_cnl->pot_kva[ind][0]  = str_hrm->sa_mod_pu;
      smc_cnl->ang_grau[ind][0] = str_hrm->sa_ang_grau;
      smc_cnl->pot_kva[ind][1]  = str_hrm->sb_mod_pu;
      smc_cnl->ang_grau[ind][1] = str_hrm->sb_ang_grau;
      smc_cnl->pot_kva[ind][2]  = str_hrm->sc_mod_pu;
      smc_cnl->ang_grau[ind][2] = str_hrm->sc_ang_grau;
      smc_cnl->pot_kva[ind][3]  = 0.;
      smc_cnl->ang_grau[ind][3] = 0.;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoFiltro(int index, smcFILTRO *smc_filtro)
   {
   //variáveis locais
   VTFiltro *filtro;

   //proteção
   if (index >= PD.lisFILTRO->Count)
      {
      smc_filtro->eqpto = NULL;
      smc_filtro->id    =-1;
      return(false);
      }
   //determina Filtro solicitado
   filtro = (VTFiltro*)PD.lisFILTRO->Items[index];
   //copia dados de Filtro p/ smcFILTRO
   smc_filtro->eqpto     = filtro;
   smc_filtro->codigo    = filtro->Codigo;
   smc_filtro->id        = filtro->Id;
   smc_filtro->barraCALC = filtro->pbarra->No[net]->Obj;
   smc_filtro->vnom_kv   = filtro->vnom;
   smc_filtro->snom_mva  = filtro->snom;
   smc_filtro->z0_pu     = complex<double>(filtro->z0.r, filtro->z0.x);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoGerador(int index, smcGERADOR *smc_gerador)
   {
   // Variáveis locais
   double          sbase_mva;
   complex<double> z0_pu, z1_pu;
   VTGerador       *gerador;

   //proteção
   if (index >= PD.lisGERADOR->Count)
      {
      smc_gerador->eqpto = NULL;
      smc_gerador->id    =-1;
      return(false);
      }
   //determina Gerador solicitado
   gerador = (VTGerador*)PD.lisGERADOR->Items[index];
   //copia dados de Gerador p/ smcGERADOR
   smc_gerador->eqpto     = gerador;
   smc_gerador->codigo    = gerador->Codigo;
   smc_gerador->id        = gerador->Id;
   smc_gerador->barra_id  = gerador->pbarra->No[net]->pai->Id; // Remover após acerto de FluxoH3
   smc_gerador->barraCALC = gerador->pbarra->No[net]->Obj;

   // smc_gerador->fases     = (gerador->pbarra->resflow->Fases == 0) ? gerador->pbarra->No[net]->Fases : gerador->pbarra->resflow->Fases;
   // Por enquanto, Gerador sempre é trifásico com faseNT
   // smc_gerador->fases     = faseABCNT;
   smc_gerador->fases    = gerador->Fases;
   smc_gerador->ligacao  = ((smc_gerador->fases & faseABC) == faseABC) ? gerador->ligacao : lgMONOFASICO;
   smc_gerador->vnom_kv  = gerador->vnom;
   smc_gerador->snom_mva = gerador->snom;
   smc_gerador->fp_min   = gerador->fplim;
   //JCG 2010.05.19
   //smc_gerador->tipo_sup = gerador->tiposup;
   smc_gerador->tipo_sup = gerador->pbarra->No[net]->TipoDeBarra();
   smc_gerador->z0_pu    = complex<double>(gerador->z0.r, gerador->z0.x);
   smc_gerador->z1_pu    = complex<double>(gerador->z1.r, gerador->z1.x);
   //potências de curto
   //assume valor padrão (caso não seja definido no gerador)
   smc_gerador->scc.fase_terra_mva = complex<double>(0., 5e6);
   smc_gerador->scc.trifasico_mva  = complex<double>(0., 5e6);
   switch(fases->NumeroDeFases(gerador->Fases))
      {
      case 1: //monofásico
         if(Abs(smc_gerador->z1_pu) > 0.)
            {
            sbase_mva = gerador->snom;  // Potência de base de FASE
            z1_pu     = smc_gerador->z1_pu;
            smc_gerador->scc.fase_terra_mva = sbase_mva / conj(z1_pu);
            }
         break;
      case 2: //bifásico
         if((Abs(smc_gerador->z0_pu) > 0.) && (Abs(smc_gerador->z1_pu) > 0.))
            {
            // Mudança de base: considera-se que as impedâncias foram fornecidas
            // na base {Snom, Vnom} [potência bifásica, tensão de linha], e o
            // modelo foi concebido na base {Snom/2, Vnom/2}
            sbase_mva = gerador->snom / 2.;      // Potência de base de FASE
            z0_pu     = 2. * smc_gerador->z0_pu; // [pu] na base {Snom/2, Vnom/2}
            z1_pu     = 2. * smc_gerador->z1_pu; // [pu] na base {Snom/2, Vnom/2}
            smc_gerador->scc.fase_terra_mva = 4. * sbase_mva / conj(z0_pu + z1_pu);
            smc_gerador->scc.trifasico_mva  = 2. * sbase_mva / conj(z1_pu);
            }
         break;
      default:// assume trifásico, independente do tipo de ligação
         if((Abs(smc_gerador->z0_pu) > 0.) && (Abs(smc_gerador->z1_pu) > 0.))
            {
            sbase_mva = gerador->snom;  // Potência de base TRIFÁSICA
            z0_pu     = smc_gerador->z0_pu;
            z1_pu     = smc_gerador->z1_pu;
            smc_gerador->scc.fase_terra_mva =
                                  sbase_mva * (3. / conj(z0_pu + (2. * z1_pu)));
            smc_gerador->scc.trifasico_mva  = sbase_mva / conj(z1_pu);
            }
         break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoMedidor(int index, smcMEDIDOR *smc_medidor)
   {
   // VERSÃO NOVA: ESTIM1/ESTIM3 - 2015.10.02
   // Variáveis locais
   int       eqpto_id, ind1, tipo_eqpto, tipo_medicao;
   VTBarra   *barra;
   VTMedidor *medidor;
   VTCanal   *canal;
   VTChave   *chave;
   TList     *lisCANAL;

   // Proteção
   if (index >= PD.lisMEDIDOR->Count)
      {
      smc_medidor->medidor = NULL;
      smc_medidor->id      = -1;
      return(false);
      }

   // Determina Medidor solicitado
   medidor = (VTMedidor *) PD.lisMEDIDOR->Items[index];

   // Copia dados de Medidor p/ smcMEDIDOR
   smc_medidor->medidor   = medidor;
   smc_medidor->codigo    = medidor->Codigo;
   smc_medidor->id        = medidor->Id;
   smc_medidor->barraCALC = medidor->pbarra->No[net]->Obj;

   // Obtém lista de Medicao do Medidor
   lisCANAL = medidor->LisCanal();

   // Verifica e salva número de canais
   if(lisCANAL->Count > SIZEOF_CANAL) return(false);  // Erro
   smc_medidor->numero_canal = 0;

   // Copia dados de Canal
   for(int n = 0; n < lisCANAL->Count; n++)
      {
      canal = (VTCanal *) lisCANAL->Items[n];
      //descarta Canal associado a uma Chave aberta
      if (canal->Eqpto->Tipo() == eqptoCHAVE)
         {
         chave = (VTChave*)canal->Eqpto;
         if (chave->Aberta) continue;
         }
      //verifica se o Canal é de corrente
      if (canal->TipoCanal == canalI)
         {//verifica se o Canal é redundante, isto é, existem Canais P e Q para mesma fase
         if (CanalRedundante(canal)) continue;
         }
      //incrementa número de Canais válidos
      ind1 = smc_medidor->numero_canal; // Para salvar corretamente dados do canal (cf. abaixo)
      smc_medidor->numero_canal++;

      tipo_eqpto   = -1;  // Inicialização
      tipo_medicao = -1;  // Inicialização

      // Determina tipo de equipamento e tipo de medição
      switch(canal->Eqpto->Tipo())
         {
         case eqptoBARRA:
            if     (canal->TipoCanal == canalV) tipo_medicao = medMOD_V;
            else if(canal->TipoCanal == canalP) tipo_medicao = medP;
            else if(canal->TipoCanal == canalQ) tipo_medicao = medQ;
            else continue;
            barra      = (VTBarra *) canal->Eqpto;
            tipo_eqpto = teBARRA;
            eqpto_id   = barra->No[net]->pai->Id;
            break;

         case eqptoCARGA:
            if     (canal->TipoCanal == canalP) tipo_medicao = medP;
            else if(canal->TipoCanal == canalQ) tipo_medicao = medQ;
            else continue;
            tipo_eqpto = teBARRA;
            eqpto_id   = ((VTEqbar *) canal->Eqpto)->pbarra->No[net]->pai->Id;
            break;

         case eqptoGERADOR:
         case eqptoSUPRIMENTO:
            if     (canal->TipoCanal == canalV) tipo_medicao = medMOD_V;
            else if(canal->TipoCanal == canalP) tipo_medicao = medP;
            else if(canal->TipoCanal == canalQ) tipo_medicao = medQ;
            else continue;
            tipo_eqpto = teBARRA;
            eqpto_id   = ((VTEqbar *) canal->Eqpto)->pbarra->No[net]->pai->Id;
            break;

         case eqptoCHAVE:
         case eqptoTRECHO:
            if     (canal->TipoCanal == canalI) tipo_medicao = medMOD_I;
            else if(canal->TipoCanal == canalS) tipo_medicao = medS;
            else if(canal->TipoCanal == canalP) tipo_medicao = medP;
            else if(canal->TipoCanal == canalQ) tipo_medicao = medQ;
            else continue;
            tipo_eqpto = teTRECHO;
            eqpto_id   = canal->Eqpto->Id;
            break;

         case eqptoTRAFO:
            if     (canal->TipoCanal == canalI) tipo_medicao = medMOD_I;
            else if(canal->TipoCanal == canalS) tipo_medicao = medS;
            else if(canal->TipoCanal == canalP) tipo_medicao = medP;
            else if(canal->TipoCanal == canalQ) tipo_medicao = medQ;
            else continue;
            tipo_eqpto = teTRAFO;
            eqpto_id   = canal->Eqpto->Id;
            break;

         default:
            continue;
         } // switch()

      smc_medidor->smc_canal[ind1].id              = canal->Numero;
      smc_medidor->smc_canal[ind1].tipo            = canal->TipoCanal;
      smc_medidor->smc_canal[ind1].eqpto_id        = eqpto_id;
      smc_medidor->smc_canal[ind1].canal           = (void *) canal;
      smc_medidor->smc_canal[ind1].codigo          = canal->Codigo;
      smc_medidor->smc_canal[ind1].tipo_eqpto      = tipo_eqpto;
      smc_medidor->smc_canal[ind1].tipo_medicao    = tipo_medicao;
      smc_medidor->smc_canal[ind1].fase_int        = canal->Fases;
      smc_medidor->smc_canal[ind1].tolerancia_perc = canal->Tolerancia_perc;

      // A IMPLEMENTAR
      smc_medidor->smc_canal[ind1].valor_fe        = 0.;
      // smc_medidor->smc_canal[ind1].valor_fe        = canal->Valor_fe;

      // smc_medidor->smc_canal[ind1].descartavel     = canal->Descartavel;

      smc_medidor->smc_canal[ind1].enabled         = canal->Enabled;
      } // for(n)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoMutua(int index, smcMUTUA *smc_mutua)
   {
   //variáveis locais
   double  compr;
   VTMutua *mutua;

   //proteção
   if (index >= PD.lisMUTUA->Count)
      {
      smc_mutua->eqpto = NULL;
      smc_mutua->id    =-1;
      return(false);
      }
   //determina Mutua solicitada
   mutua = (VTMutua*)PD.lisMUTUA->Items[index];
   //copia dados de Mutua p/ smcMUTUA
   smc_mutua->eqpto                  = mutua;
   smc_mutua->codigo                 = mutua->Codigo;
   smc_mutua->id                     = mutua->Id;
   smc_mutua->trecho1_id             = mutua->ptre1->Id;
   smc_mutua->trecho2_id             = mutua->ptre2->Id;
   smc_mutua->sentido.trecho1_bar_id = mutua->pbar_tre1->Id;
   smc_mutua->sentido.trecho2_bar_id = mutua->pbar_tre2->Id;
   compr             = min(mutua->ptre1->Comprimento_km,
                           mutua->ptre1->Comprimento_km);
   smc_mutua->z0_ohm = complex<double>(mutua->z0.r, mutua->z0.x) * compr;
   smc_mutua->z1_ohm = complex<double>(mutua->z1.r, mutua->z1.x) * compr;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoReator(int index, smcREATOR *smc_reator)
   {
   //variáveis locais
   VTReator *reator;

   //proteção
   if (index >= PD.lisREATOR->Count)
      {
      smc_reator->eqpto = NULL;
      smc_reator->id    =-1;
      return(false);
      }
   //determina Reator solicitado
   reator = (VTReator*)PD.lisREATOR->Items[index];
   //copia dados de Reator p/ smcREATOR
   smc_reator->eqpto     = reator;
   smc_reator->codigo    = reator->Codigo;
   smc_reator->id        = reator->Id;
   smc_reator->barraCALC = reator->pbarra->No[net]->Obj;
   smc_reator->ligacao   = reator->ligacao;
   smc_reator->q_mvar    = reator->q;
   return(true);
   }
   
 //---------------------------------------------------------------------------
bool __fastcall TData::EqptoRegulador(int index, smcTRAFO2E *smc_trafo2e)
   {
   //variáveis locais
   int             ind_fase, modelo;
   double          var_tap_pu; //, x_pu;
   complex<double> z_pu;
   VTBarra         *barra1, *barra2;
   VTRegulador     *regulador, *regu_rede;

   //proteção
   if (index >= PD.lisREGULADOR->Count)
      {
      smc_trafo2e->eqpto = NULL;
      smc_trafo2e->id    =-1;
      return(false);
      }
   //determina Regulador solicitado
   regulador = (VTRegulador*)PD.lisREGULADOR->Items[index];
   //IMPORTANTE: o regulador usado no cálculo é um clone do regulador da rede.
   //            Ele aponta p/ o Regulador da rede
   regu_rede   = (VTRegulador*)(regulador->ligaPai);
   //a Barra 1 deve ser a barra de entrada
   if ((barra1 = regu_rede->sentido.bar_ref) == NULL) barra1 = regu_rede->pbarra1;
   //define Barra2 diferente de Barra1
   barra2 = (regu_rede->pbarra1 == barra1) ? regu_rede->pbarra2 : regu_rede->pbarra1;
   //garante Barras ordenadas para o Regulador passado p/ o cálculo
   regulador->pbarra1 = barra1;
   regulador->pbarra2 = barra2;
   regu_rede->pbarra1 = barra1;
   regu_rede->pbarra2 = barra2;
   //IMPORTANTE: em smcTRAFO2E deve ser passado o Regulador da rede p/ efeito de retorno dos resultados
   smc_trafo2e->eqpto       = regu_rede;
   //define tipo do regulador (monofásico ou trifásico)
   smc_trafo2e->monofasico  = (regulador->ligacao == lgMONOFASICO);
   //copia demais dados de Regulador p/ smcTRAFO2E
   smc_trafo2e->codigo      = regulador->Codigo;
   smc_trafo2e->id          = regulador->Id;
   smc_trafo2e->barraCALC_1 = barra1->No[net]->Obj;
   smc_trafo2e->barraCALC_2 = barra2->No[net]->Obj;
   smc_trafo2e->auto_trafo  = true;
   smc_trafo2e->fase_comum  = (regulador->fases >> 8) & 0xff;
   smc_trafo2e->snom_mva    = regulador->Spas_mva;
   smc_trafo2e->perda_ferro = 0.;
   smc_trafo2e->z0_pu       = complex<double>(regulador->z0.r, regulador->z0.x);
   smc_trafo2e->z1_pu       = complex<double>(regulador->z1.r, regulador->z1.x);

   // Aplica valor de default se necessário. Foram testados diversos valores...
   // O problema é a não-convergência em NR1/NR3 para valores muito baixos da
   // impedância.
   // x_pu = 0.01  * smc_trafo2e->snom_mva; // [1   * snom_mva / 100] ; j1   pu em 100 MVA
   // x_pu = 0.001 * smc_trafo2e->snom_mva; // [0.1 * snom_mva / 100] ; j0.1 pu em 100 MVA
   // x_pu = 0.01;                          // 0.01 pu na base nominal do equipamento (2014.08.15)
   // x_pu = 0.05;                          // 0.05 pu na base nominal do equipamento (2016.05.09)
   // Valor atualmente utilizado
   // IMPORTANTE: este valor é redefinido em TMonta1::MontaRedeNR1() e
   //             TMonta3::MontaRedeNR3()
   // x_pu = 0.001 * smc_trafo2e->snom_mva; // [0.1 * snom_mva / 100] ; j0.1 pu em 100 MVA

   z_pu = zreg_trafo_pu;  // Implementado em 2018.03.12
   if(Abs(smc_trafo2e->z0_pu) == 0.) smc_trafo2e->z0_pu = z_pu;
   if(Abs(smc_trafo2e->z1_pu) == 0.) smc_trafo2e->z1_pu = z_pu;
   //primário
   smc_trafo2e->pri.ligacao = regulador->ligacao;
   smc_trafo2e->pri.fase    = regulador->fases & 0xff;
   if(fases->NumeroDeFases(smc_trafo2e->pri.fase) == 1)
      {
      ind_fase                 = fases->TagToIndex[smc_trafo2e->pri.fase];
      smc_trafo2e->pri.vnom_kv = barra1->zona->Vfn_kv[ind_fase];
      }
   else
      {
      smc_trafo2e->pri.vnom_kv = barra1->zona->Vff_kv;
      }
   smc_trafo2e->pri.tap_kv    = smc_trafo2e->pri.vnom_kv;
   smc_trafo2e->pri.defasagem = 0.;
   smc_trafo2e->pri.zater_ohm = CZero();
   //secundário
   smc_trafo2e->sec.ligacao = smc_trafo2e->pri.ligacao;
   smc_trafo2e->sec.fase    = smc_trafo2e->pri.fase;
   if(fases->NumeroDeFases(smc_trafo2e->sec.fase) == 1)
      {
      ind_fase                 = fases->TagToIndex[smc_trafo2e->sec.fase];
      smc_trafo2e->sec.vnom_kv = barra2->zona->Vfn_kv[ind_fase];
      }
   else
      {
      smc_trafo2e->sec.vnom_kv = barra2->zona->Vff_kv;
      }
   smc_trafo2e->sec.tap_kv    = smc_trafo2e->sec.vnom_kv;
   smc_trafo2e->sec.defasagem = 0.;
   smc_trafo2e->sec.zater_ohm = CZero();

   // LTC
   // Inicialização geral (modificada depois, na análise de cada modelo)
   smc_trafo2e->ltc.pri_sec     = ltcPRIMARIOX;
   smc_trafo2e->ltc.modelo      = regINDEF1;
   smc_trafo2e->ltc.sub_modelo  = regINDEF2;
   smc_trafo2e->ltc.barra_id_12 = -1;  // ID inválido
   smc_trafo2e->ltc.barra_id_21 = -1;  // ID inválido
   if(regulador->cfg_auto.bar_ref != NULL)
      {
      if(regulador->cfg_auto.bar_ref->No[net] != NULL)
         {
         smc_trafo2e->ltc.barra_id_12 = regulador->cfg_auto.bar_ref->No[net]->pai->Id;
         }
      }
   if(regulador->cfg_auto.bar_ref_inv != NULL)
      {
      if(regulador->cfg_auto.bar_ref_inv->No[net] != NULL)
         {
         smc_trafo2e->ltc.barra_id_21 = regulador->cfg_auto.bar_ref_inv->No[net]->pai->Id;
         }
      }
   smc_trafo2e->ltc.v_pu_12            = regulador->cfg_auto.v_pu;
   smc_trafo2e->ltc.v_pu_21            = regulador->cfg_auto.vinv_pu;
   smc_trafo2e->ltc.banda_morta_pu     = regulador->LarguraBandaMorta_pu();
   smc_trafo2e->ltc.banda_morta_inv_pu = regulador->LarguraBandaMortaInv_pu();
   smc_trafo2e->ltc.var_tensao         = regulador->var_tensao;
   smc_trafo2e->ltc.num_passo          = regulador->num_passo;
   for(int nt=0; nt < (MAX_FASE-1); nt++)
      {
      smc_trafo2e->ltc.passo_calc[nt] = 0;  // Valor a ser calculado pelo fluxo
      }


// INÍCIO DE CÓDIGO TEMPORÁRIO ENERQCT (HPS, 2019.08.21)
// O Editor não permite mais definir reguladores ENERQCT
// regulador->modelo = reguENERQCT;
// FIM DE CÓDIGO TEMPORÁRIO ENERQCT


   // Analisa modelo
   switch(regulador->modelo)
      {
      case reguPADRAO:
         smc_trafo2e->ltc.modelo = regPADRAO;
         break;

      case reguENERQCT:
         smc_trafo2e->ltc.modelo = regENERQCT1;
         // Neste caso é possível reduzir a impedância do modelo
         z_pu               = complex<double>(0., (0.1 * zreg_trafo_pu.imag()));
         smc_trafo2e->z0_pu = z_pu;
         smc_trafo2e->z1_pu = z_pu;
         break;

      case reguAUTO_BI: // Caso 2 Copel
         smc_trafo2e->ltc.modelo     = regDOIS_SENTIDOS;
         smc_trafo2e->ltc.sub_modelo = regAUTO_BI;
         break;

      case reguAUTO_UNI: // Caso 4 Copel: habilita bypass em caso de fluxo reverso
         smc_trafo2e->ltc.modelo     = regDOIS_SENTIDOS;
         smc_trafo2e->ltc.sub_modelo = regAUTO_UNI;
         break;

      case reguAUTO_FIXA: // Caso 3 Copel: mantém os dados do sentido direto (12)
         smc_trafo2e->ltc.modelo      = regDOIS_SENTIDOS;
         smc_trafo2e->ltc.sub_modelo  = regAUTO_FIXA;
         smc_trafo2e->ltc.barra_id_21 = smc_trafo2e->ltc.barra_id_12;
         smc_trafo2e->ltc.v_pu_21     = smc_trafo2e->ltc.v_pu_12;
         break;

      case reguFIXO: // Caso 1 Copel: tap fixo
         smc_trafo2e->ltc.pri_sec = ltcNAO_USADOX;
         smc_trafo2e->ltc.v_pu_12 =  1.;
         smc_trafo2e->ltc.v_pu_21 =  1.;
         // Calcula valor do tap secundário em função do passo selecionado
         var_tap_pu = (double(regulador->cfg_fixo.passo) /
                      double(regulador->num_passo)) * (regulador->var_tensao/100.);
         smc_trafo2e->sec.tap_kv = (1. + var_tap_pu) * smc_trafo2e->sec.vnom_kv;
         break;

      default:  // Erro
         return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoSuprimento(int index, smcSUPRIMENTO *smc_suprimento)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   //proteção
   if (index >= PD.lisSUPRIMENTO->Count)
      {
      smc_suprimento->eqpto = NULL;
      smc_suprimento->id    =-1;
      return(false);
      }
   //determina Suprimento solicitado
   suprimento = (VTSuprimento*)PD.lisSUPRIMENTO->Items[index];
   //copia dados de Suprimento p/ smcSUPRIMENTO
   smc_suprimento->eqpto              = suprimento;
   smc_suprimento->codigo             = suprimento->Codigo;
   smc_suprimento->id                 = suprimento->Id;
   smc_suprimento->barra_id           = suprimento->pbarra->No[net]->pai->Id; // Remover após acerto de FluxoH3
   smc_suprimento->barraCALC          = suprimento->pbarra->No[net]->Obj;
   smc_suprimento->smax_mva           = suprimento->smax;
   //smc_suprimento->fases              = (suprimento->pbarra->resflow->Fases == 0) ? suprimento->pbarra->No[net]->Fases : suprimento->pbarra->resflow->Fases;
   smc_suprimento->fases              = suprimento->Fases;
   smc_suprimento->ligacao            = ((smc_suprimento->fases & faseABC) == faseABC) ? lgEST_AT : lgMONOFASICO;
   //JCG 2010.05.19
   //smc_suprimento->tipo_sup           = suprimento->tiposup;
   smc_suprimento->tipo_sup           = suprimento->pbarra->No[net]->TipoDeBarra();
   smc_suprimento->scc.fase_terra_mva = complex<double>(suprimento->pcc_ft.p, suprimento->pcc_ft.q);
   smc_suprimento->scc.trifasico_mva  = complex<double>(suprimento->pcc_3f.p, suprimento->pcc_3f.q);
   smc_suprimento->eqv.z0_pu          = complex<double>(suprimento->zeq0.r, suprimento->zeq0.x);
   smc_suprimento->eqv.z1_pu          = complex<double>(suprimento->zeq1.r, suprimento->zeq1.x);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoTrafo2E(int index, smcTRAFO2E *smc_trafo2e)
   {
   //variáveis locais
   bool            teste_DY, teste_YD;
   int             grupo_lig, ind_fase;
   double          rotacao_pri_sec;
   complex<double> z_pu;
   VTMatY          *pij, *pji;
   VTTrafo         *trafo;

   //proteção
   if (index >= PD.lisTRAFO2E->Count)
      {
      smc_trafo2e->eqpto = NULL;
      smc_trafo2e->id    =-1;
      return(false);
      }
   //determina Trafo solicitado
   trafo = (VTTrafo*)PD.lisTRAFO2E->Items[index];
   //copia dados de Trafo p/ smcTRAFO2E
   smc_trafo2e->eqpto       = trafo;
   smc_trafo2e->codigo      = trafo->Codigo;
   smc_trafo2e->id          = trafo->Id;
   smc_trafo2e->barraCALC_1 = trafo->pbarra1->No[net]->Obj;
   smc_trafo2e->barraCALC_2 = trafo->pbarra2->No[net]->Obj;
   smc_trafo2e->snom_mva    = trafo->snom;
   smc_trafo2e->perda_ferro = trafo->perda_ferro;
   smc_trafo2e->auto_trafo  = trafo->auto_trafo;
   smc_trafo2e->fase_comum  = -1;  // TEMPORÁRIO
   smc_trafo2e->monofasico  = trafo->Monofasico;
   smc_trafo2e->reducao     = trafo->Reducao;
   smc_trafo2e->modelo_eqv  = modeloEQV1;  // Default
   // Tratamento distinto de trafos equivalentes e de trafos não-equivalentes
   if(smc_trafo2e->reducao)  // Trafo equivalente
      {
      // Preenche dados do trafo equivalente
      pij = trafo->eqv.matYij_s;
      pji = trafo->eqv.matYji_s;
      if((pij == NULL) || (pji == NULL)) return(false);
      smc_trafo2e->modelo_eqv    = trafo->eqv.modelo;
      smc_trafo2e->Ynodal0_ij_pu = complex<double>(trafo->eqv.y0_ij_pu.g,
                                                   trafo->eqv.y0_ij_pu.b);
      smc_trafo2e->Ynodal0_ji_pu = complex<double>(trafo->eqv.y0_ji_pu.g,
                                                   trafo->eqv.y0_ji_pu.b);
      smc_trafo2e->Ynodal1_ij_pu = complex<double>(trafo->eqv.y1_ij_pu.g,
                                                   trafo->eqv.y1_ij_pu.b);
      smc_trafo2e->Ynodal1_ji_pu = complex<double>(trafo->eqv.y1_ji_pu.g,
                                                   trafo->eqv.y1_ji_pu.b);
      for(int i=0; i < MAX_FASE; i++)
         {
         for(int j=0; j < MAX_FASE; j++)
            {
            smc_trafo2e->mat_ynodal_ij_S[i][j] = pij->Y[i][j];
            smc_trafo2e->mat_ynodal_ji_S[i][j] = pji->Y[i][j];
            }
         }
      }
   else  // Trafo não-equivalente
      {
      smc_trafo2e->z0_pu = complex<double>(trafo->z0.r, trafo->z0.x);
      smc_trafo2e->z1_pu = complex<double>(trafo->z1.r, trafo->z1.x);

      // Foram testados diversos valores...
      // Aplica valores de default se necessário: calcula valor em pu na base
      // 'snom_mva' de tal forma que a impedância resulte j1 pu em 100 MVA.
      // Este valor foi escolhido depois de várias tentativas - o problema é
      // a não-convergência em NR1 para valores muito baixos da impedância
      // (HPS, 2014_02_19).
      // x_pu = 0.01  * smc_trafo2e->snom_mva; // [1   * snom_mva / 100] ; j1   pu em 100 MVA
      // x_pu = 0.001 * smc_trafo2e->snom_mva; // [0.1 * snom_mva / 100] ; j0.1 pu em 100 MVA
      // VALOR DEFINIDO EM 2014.08.15:
      // x_pu = 0.01;  // 0.01 pu na base nominal do equipamento

      z_pu = zreg_trafo_pu;  // Implementado em 2018.03.12
      if(Abs(smc_trafo2e->z0_pu) == 0.) smc_trafo2e->z0_pu = z_pu;
      if(Abs(smc_trafo2e->z1_pu) == 0.) smc_trafo2e->z1_pu = z_pu;
      }
   //primário
   smc_trafo2e->pri.ligacao = trafo->pri.ligacao;
   smc_trafo2e->pri.fase    = trafo->pri.fase;
   if(fases->NumeroDeFases(smc_trafo2e->pri.fase) == 1)
      {
      ind_fase                 = fases->TagToIndex[smc_trafo2e->pri.fase];
      smc_trafo2e->pri.vnom_kv = trafo->pbarra1->zona->Vfn_kv[ind_fase];
      }
   else
      {
      smc_trafo2e->pri.vnom_kv = trafo->pbarra1->zona->Vff_kv;
      }
   //assume tensão de tap igual a tensão nominal
   smc_trafo2e->pri.tap_kv = smc_trafo2e->pri.vnom_kv;
   //proteção contra vnom nulo
   if (! IsDoubleZero(trafo->pri.vnom))
      {
      smc_trafo2e->pri.tap_kv = smc_trafo2e->pri.vnom_kv * trafo->pri.tap / trafo->pri.vnom;
      }
   smc_trafo2e->pri.defasagem = trafo->pri.defasagem;
   smc_trafo2e->pri.zater_ohm = complex<double>(trafo->pri.zterra.r, trafo->pri.zterra.x);
   //secundário
   smc_trafo2e->sec.ligacao = trafo->sec.ligacao;
   smc_trafo2e->sec.fase    = trafo->sec.fase;
   if(fases->NumeroDeFases(smc_trafo2e->sec.fase) == 1)
      {
      ind_fase                 = fases->TagToIndex[smc_trafo2e->sec.fase];
      smc_trafo2e->sec.vnom_kv = trafo->pbarra2->zona->Vfn_kv[ind_fase];
      }
   else
      {
      smc_trafo2e->sec.vnom_kv = trafo->pbarra2->zona->Vff_kv;
      }
   //assume tensão de tap igual a tensão nominal
   smc_trafo2e->sec.tap_kv = smc_trafo2e->sec.vnom_kv;
   //proteção contra vnom nulo
   if (! IsDoubleZero(trafo->sec.vnom))
      {
      smc_trafo2e->sec.tap_kv = smc_trafo2e->sec.vnom_kv * trafo->sec.tap / trafo->sec.vnom;
      }
   smc_trafo2e->sec.defasagem = trafo->sec.defasagem;
   smc_trafo2e->sec.zater_ohm = complex<double>(trafo->sec.zterra.r, trafo->sec.zterra.x);

   //determina grupo da ligação (somente DY e YD)
   grupo_lig = glINDEF;
   //verifica se não é um Trafo equivalente
   if (! smc_trafo2e->reducao)
      {//determina tipo da ligação em função da defasagem
      rotacao_pri_sec = smc_trafo2e->sec.defasagem - smc_trafo2e->pri.defasagem;
      teste_DY = ((smc_trafo2e->pri.ligacao == ligacaoTRIANGULO) ||
                  (smc_trafo2e->pri.ligacao == ligacaoDELTA_FECHADO)) &&
                 ((smc_trafo2e->sec.ligacao == ligacaoESTRELA) ||
                  (smc_trafo2e->sec.ligacao == ligacaoESTRELA_ATERRADO));
      teste_YD = ((smc_trafo2e->pri.ligacao == ligacaoESTRELA) ||
                  (smc_trafo2e->pri.ligacao == ligacaoESTRELA_ATERRADO)) &&
                 ((smc_trafo2e->sec.ligacao == ligacaoTRIANGULO) ||
                  (smc_trafo2e->sec.ligacao == ligacaoDELTA_FECHADO));
      if(teste_DY)
         {
         if     (Abs(rotacao_pri_sec - 30.) < 0.001) grupo_lig = glDy11;
         else if(Abs(rotacao_pri_sec + 30.) < 0.001) grupo_lig = glDy1;
         else return(false);  // Erro
         }
      else if(teste_YD)
         {
         if     (Abs(rotacao_pri_sec - 30.) < 0.001) grupo_lig = glYd11;
         else if(Abs(rotacao_pri_sec + 30.) < 0.001) grupo_lig = glYd1;
         else return(false);  // Erro
         }
      }
   // Define grupo da ligação
   smc_trafo2e->grupo_lig = grupo_lig;

   //LTC
   smc_trafo2e->ltc.pri_sec = trafo->ltc.tipo;
   if(trafo->ltc.tipo != 0)  // O trafo tem LTC
      {
      smc_trafo2e->ltc.modelo             = regPADRAO;
      smc_trafo2e->ltc.sub_modelo         = regINDEF2;
      smc_trafo2e->ltc.var_tensao         = trafo->ltc.dv_perc;
      smc_trafo2e->ltc.num_passo          = trafo->ltc.num_passo;
      smc_trafo2e->ltc.barra_id_12        = trafo->ltc.ajuste_auto.bar_ref->No[net]->pai->Id;
      smc_trafo2e->ltc.v_pu_12            = trafo->ltc.ajuste_auto.v_pu;
      smc_trafo2e->ltc.banda_morta_pu     = 0.;
      smc_trafo2e->ltc.banda_morta_inv_pu = 0.;
      for(int nt=0; nt < (MAX_FASE-1); nt++)
         {
         smc_trafo2e->ltc.passo_calc[nt] = 0; // Valor a ser calculado pelo fluxo
         }
      // Define barra de referência para fluxo reverso: barra do primário
      VTBarra *pbar1               = trafo->BarraPrimario();
      smc_trafo2e->ltc.barra_id_21 = pbar1->No[net]->pai->Id;
      smc_trafo2e->ltc.v_pu_21     = 1.;  // [pu]
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoTrafo3E(int index, smcTRAFO3E *smc_trafo3e)
   {
   //variáveis locais
   VTTrafo3E *trafo;

   //proteção
   if (index >= PD.lisTRAFO3E->Count)
      {
      smc_trafo3e->eqpto =  NULL;
      smc_trafo3e->id    = -1;
      return(false);
      }
   //determina Trafo3E solicitado
   trafo = (VTTrafo3E*)PD.lisTRAFO3E->Items[index];
   //copia dados de Trafo3E p/ smcTRAFO3E
   smc_trafo3e->eqpto          = trafo;
   smc_trafo3e->codigo         = trafo->Codigo;
   smc_trafo3e->id             = trafo->Id;
   smc_trafo3e->barraCALC_1    = trafo->pbarra1->No[net]->Obj;
   smc_trafo3e->barraCALC_2    = trafo->pbarra2->No[net]->Obj;
   smc_trafo3e->barraCALC_3    = trafo->pbarra3->No[net]->Obj;
   smc_trafo3e->perda_ferro    = trafo->perda_ferro;
   smc_trafo3e->zps0_pu        = complex<double>(trafo->zps0.r, trafo->zps0.x);
   smc_trafo3e->zps1_pu        = complex<double>(trafo->zps1.r, trafo->zps1.x);
   smc_trafo3e->zpt0_pu        = complex<double>(trafo->zpt0.r, trafo->zpt0.x);
   smc_trafo3e->zpt1_pu        = complex<double>(trafo->zpt1.r, trafo->zpt1.x);
   smc_trafo3e->zst0_pu        = complex<double>(trafo->zst0.r, trafo->zst0.x);
   smc_trafo3e->zst1_pu        = complex<double>(trafo->zst1.r, trafo->zst1.x);
   //primário
   smc_trafo3e->pri.enrol_orig = 0; // IMPORTANTE: não será alterado por eventual rotação
   smc_trafo3e->pri.ligacao    = trafo->pri.ligacao;
   smc_trafo3e->pri.snom_mva   = trafo->pri.snom;
   smc_trafo3e->pri.vnom_kv    = trafo->pri.vnom;
   smc_trafo3e->pri.tap_kv     = trafo->pri.tap;
   smc_trafo3e->pri.defasagem  = trafo->pri.defasagem;
   smc_trafo3e->pri.zater_ohm  = complex<double>(trafo->pri.zterra.r, trafo->pri.zterra.x);
   //secundário
   smc_trafo3e->sec.enrol_orig = 1; // IMPORTANTE: não será alterado por eventual rotação
   smc_trafo3e->sec.ligacao    = trafo->sec.ligacao;
   smc_trafo3e->sec.snom_mva   = trafo->sec.snom;
   smc_trafo3e->sec.vnom_kv    = trafo->sec.vnom;
   smc_trafo3e->sec.tap_kv     = trafo->sec.tap;
   smc_trafo3e->sec.defasagem  = trafo->sec.defasagem;
   smc_trafo3e->sec.zater_ohm  = complex<double>(trafo->sec.zterra.r, trafo->sec.zterra.x);
   //terciário
   smc_trafo3e->ter.enrol_orig = 2; // IMPORTANTE: não será alterado por eventual rotação
   smc_trafo3e->ter.ligacao    = trafo->ter.ligacao;
   smc_trafo3e->ter.snom_mva   = trafo->ter.snom;
   smc_trafo3e->ter.vnom_kv    = trafo->ter.vnom;
   smc_trafo3e->ter.tap_kv     = trafo->ter.tap;
   smc_trafo3e->ter.defasagem  = trafo->ter.defasagem;
   smc_trafo3e->ter.zater_ohm  = complex<double>(trafo->ter.zterra.r, trafo->ter.zterra.x);

   // Determina grupo da ligação do trafo (DDY1, DDY11, DY1Y1, ...). Ele é
   // necessário para a correta rotação dos enrolamentos (cf. abaixo).
   smc_trafo3e->grupo_lig = DeterminaGrupoLigacao(smc_trafo3e);

   // Mapeia a configuração do trafo (uma das 19 possíveis) em uma das 7
   // configurações básicas implementadas no Cálculo (DDY1, DDY11, DY1Y1,
   // DY1Y11, DY11Y1, DY11Y11 ou YYY - vide emcGRUPO_LIGACAO em Interface.h)
   //    - Trafo com 1 enrolamento  em Delta: enrolamento em Delta fica no primário
   //    - Trafo com 2 enrolamentos em Delta: os enrolamentos em Delta ficam no
   //                                         primário e no secundário
   RedefineDeltaComoPrimario(smc_trafo3e);

   // Determina novamente o grupo da ligação (pode ter sido alterado pela
   // redefinição acima)
   smc_trafo3e->grupo_lig = DeterminaGrupoLigacao(smc_trafo3e);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoTrafoZZ(int index, smcTRAFOZZ *smc_trafozz)
   {
   //variáveis locais
   VTTrafoZZ *trafoZZ;

   //proteção
   if (index >= PD.lisTRAFOZZ->Count)
      {
      smc_trafozz->eqpto = NULL;
      smc_trafozz->id    =-1;
      return(false);
      }
   //determina TrafoZZ solicitado
   trafoZZ = (VTTrafoZZ*)PD.lisTRAFOZZ->Items[index];
   //copia dados de TrafoZZ p/ smcTRAFOZZ
   smc_trafozz->eqpto     = trafoZZ;
   smc_trafozz->codigo    = trafoZZ->Codigo;
   smc_trafozz->id        = trafoZZ->Id;
   smc_trafozz->barraCALC = trafoZZ->pbarra->No[net]->Obj;
   smc_trafozz->vnom_kv   = trafoZZ->vnom;
   smc_trafozz->snom_mva  = trafoZZ->snom;
   smc_trafozz->z0_pu     = complex<double>(trafoZZ->z0.r, trafoZZ->z0.x);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoTramo(int index, smcTRAMO *smc_tramo)
   {
   //variáveis locais
   double  r0_pu, x0_pu, yc0_pu;
   double  r1_pu, x1_pu, yc1_pu;
   VTTramo *tramo;

   //proteção
   if (index >= PD.lisTRAMO->Count)
      {
      smc_tramo->eqpto = NULL;
      smc_tramo->id    =-1;
      return(false);
      }
   //determina Tramo solicitado
   tramo = (VTTramo*)PD.lisTRAMO->Items[index];
   //copia dados de Tramo p/ smcTRAMO
   smc_tramo->eqpto          = tramo;
   smc_tramo->codigo         = tramo->Codigo;
   smc_tramo->id             = tramo->Id;
   smc_tramo->barraCALC_1    = tramo->pbarra1->No[net]->Obj;
   smc_tramo->barraCALC_2    = tramo->pbarra2->No[net]->Obj;
   smc_tramo->fases          = tramo->Fases(tramo->pbarra1);
   smc_tramo->comprimento_km = tramo->Comprimento_km;
   //corrente máxima por fase
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      smc_tramo->vet_iadm[nf] = tramo->Iadm_amp[nf];
      }
   //impedância série de seq. admitância paralela de seq.
   tramo->Z0_pu(Sbase, tramo->pbarra1->vnom, r0_pu, x0_pu, yc0_pu);
   tramo->Z1_pu(Sbase, tramo->pbarra1->vnom, r1_pu, x1_pu, yc1_pu);
   smc_tramo->tipo_cfg             = tipo_Z0Z1_Y0Y1;
   smc_tramo->z0_pu                = complex<double>(r0_pu, x0_pu);
   smc_tramo->z1_pu                = complex<double>(r1_pu, x1_pu);
   smc_tramo->modelo_pi.y0cap_pu   = 0.;  // Tramo não tem capacitância
   smc_tramo->modelo_pi.y1cap_pu   = 0.;  // Tramo não tem capacitância
   //monta matZ e zera matC do Tramo
   smc_tramo->tipo_cfg = tipo_MATZ_MATC;
   montaZ->Executa(tramo);
   for(int i = 0; i < MAX_FASE; i++)
      {
      for(int j = 0; j < MAX_FASE; j++)
         {
         smc_tramo->matZ[i][j] = tramo->Z_ohm[i][j];
         smc_tramo->matC[i][j] = 0.;  // Tramo não tem capacitância
         }
      }
   //proteção: se smc_tramo->matZ[3][3] é nula, assume  valor de smc_tramo->matZ[0][0]
   if (Abs(smc_tramo->matZ[MAX_FASE-1][MAX_FASE-1]) == 0.)
      {
      smc_tramo->matZ[MAX_FASE-1][MAX_FASE-1] = smc_tramo->matZ[0][0];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoTrecho(int index, smcTRECHO *smc_trecho)
   {
   //variáveis locais
   double          r0_pu, x0_pu, yc0_pu;
   double          r1_pu, x1_pu, yc1_pu;
   //complex<double> z0_pu, z1_pu, zm_pu, zp_pu;
   VTTrecho *trecho;

   //proteção
   if (index >= PD.lisTRECHO->Count)
      {
      smc_trecho->eqpto = NULL;
      smc_trecho->id    =-1;
      return(false);
      }
   //determina Trecho solicitado
   trecho = (VTTrecho*)PD.lisTRECHO->Items[index];
   //copia dados de Trecho p/ smcTRECHO
   smc_trecho->eqpto          = trecho;
   smc_trecho->codigo         = trecho->Codigo;
   smc_trecho->id             = trecho->Id;
   smc_trecho->barra1_id      = trecho->pbarra1->No[net]->pai->Id; // Remover após acerto de FluxoH3
   smc_trecho->barra2_id      = trecho->pbarra2->No[net]->pai->Id; // Remover após acerto de FluxoH3
   smc_trecho->barraCALC_1    = trecho->pbarra1->No[net]->Obj;
   smc_trecho->barraCALC_2    = trecho->pbarra2->No[net]->Obj;
   smc_trecho->fases          = trecho->arranjo->Fases;
   smc_trecho->comprimento_km = trecho->Comprimento_km;
   //corrente máxima por fase
   //smc_trecho->capacidade_amp       = trecho->Capacidade_amp;
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      smc_trecho->vet_iadm[nf] = trecho->Iadm_amp[nf];
      }
   //impedância série de seq. admitância paralela de seq.
   trecho->Z0_pu(Sbase, trecho->pbarra1->vnom, r0_pu, x0_pu, yc0_pu);
   trecho->Z1_pu(Sbase, trecho->pbarra1->vnom, r1_pu, x1_pu, yc1_pu);
   smc_trecho->z0_pu                = complex<double>(r0_pu, x0_pu);
   smc_trecho->z1_pu                = complex<double>(r1_pu, x1_pu);
   smc_trecho->modelo_pi.y0cap_pu   = yc0_pu;
   smc_trecho->modelo_pi.y1cap_pu   = yc1_pu;
   //monta matZ e matC do Trecho
   smc_trecho->tipo_cfg = tipo_MATZ_MATC;
   montaZ->Executa(trecho);
   //preenche matriz de impedância c/ MatZ e MatC do Trecho
   for(int i = 0; i < MAX_FASE; i++)
      {
      for(int j = 0; j < MAX_FASE; j++)
         {
         smc_trecho->matZ[i][j] = trecho->MatZ_ohm[i][j];
         smc_trecho->matC[i][j] = trecho->MatC_siemens[i][j];
         }
      }
   //proteção: se smc_trecho->matZ[3][3] é nula, assume  valor de smc_trecho->matZ[0][0]
   if (Abs(smc_trecho->matZ[MAX_FASE-1][MAX_FASE-1]) == 0.)
      {
      smc_trecho->matZ[MAX_FASE-1][MAX_FASE-1] = smc_trecho->matZ[0][0];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EqptoYref(int index, smcYREF *smc_yref)
   {
   //variáveis locais
   VTYref *yref;

   //proteção
   if (index >= PD.lisYREF->Count)
      {
      smc_yref->eqpto = NULL;
      smc_yref->id    =-1;
      return(false);
      }
   //determina Yref solicitado
   yref = (VTYref*)PD.lisYREF->Items[index];
   //copia dados de Yref p/ smcYREF
   smc_yref->eqpto     = yref;
   smc_yref->codigo    = yref->Codigo;
   smc_yref->id        = yref->Id;
   smc_yref->barraCALC = yref->pbarra->No[net]->Obj;
   smc_yref->y0_pu     = yref->Y0_pu;
   smc_yref->y1_pu     = yref->Y1_pu;
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         smc_yref->maty_S[i][j] = yref->Y_pu[i][j]; // É [S] mesmo, e não [pu]
         }
     }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::EstadoBateria(smcESTADO_BATERIA *estado_bateria)
{
// Para um determinado patamar de índice 'ind_pat', esta função retorna:
//    - estado:           estCARREGADA, estINTERMEDIARIO ou estDESCARREGADA
//    - modo de operação: mopCARGA, mopDESCARGA ou mopOCIOSA
//    - energia atualmente armazenada
//    - potências externas (ativa e reativa, absorvidas ou fornecidas)

// Variáveis locais
int       estado, ind_pat, modo_operacao;
double    en_armaz_max_kwh, en_armaz_min_kwh;
VTBateria *bateria;

// Proteção
if(estado_bateria == NULL) return(false);

// Obtém argumentos para Sinap
bateria = (VTBateria *) estado_bateria->eqpto;
ind_pat = estado_bateria->ind_pat;

// Obtém modo de operação da bateria no patamar atual
modo_operacao = bateria->Modo_operacao[ind_pat];
if     (modo_operacao == moCARGA)    estado_bateria->modo_operacao = mopCARGA;
else if(modo_operacao == moDESCARGA) estado_bateria->modo_operacao = mopDESCARGA;
else                                 estado_bateria->modo_operacao = mopOCIOSA;

// Obtém potências externas no patamar atual
estado_bateria->pext_kw   = bateria->Pext_kw[ind_pat];
estado_bateria->qext_kvar = bateria->Qext_kvar[ind_pat];

// Obtém energia atualmente armazenada
if(ind_pat == 0)  // Primeiro patamar
   {
   estado_bateria->en_armaz_kwh = bateria->Earmaz_inic_pu * bateria->Enom_kwh;
   }
else  // Demais patamares: pega a energia do patamar anterior
   {
   estado_bateria->en_armaz_kwh = bateria->Earmaz_atual_kwh[ind_pat-1];
   }

// Determina estado da bateria no patamar atual, através da energia
// atualmente armazenada. Corrige modo de operação e potências externas
// se necessário.
en_armaz_min_kwh = bateria->Gatilho_carga_pu * bateria->Enom_kwh;
en_armaz_max_kwh = bateria->Gatilho_desc_pu  * bateria->Enom_kwh;
if(estado_bateria->en_armaz_kwh <= en_armaz_min_kwh)
   {
   estado_bateria->estado = estDESCARREGADA;
   if(bateria->Tipo_operacao == toMANUAL)
      {
      if(estado_bateria->modo_operacao == mopDESCARGA)
         {
         // Foi especificada operação de descarga em bateria já descarregada
         estado_bateria->modo_operacao = mopOCIOSA;
         estado_bateria->pext_kw       = bateria->P_ociosa_pu * bateria->Pnom_kw;
         estado_bateria->qext_kvar     = bateria->Q_ociosa_pu * bateria->Pnom_kw;
         }
      }
   else  // toAUTO
      {
      estado_bateria->modo_operacao = mopCARGA;
      }
   }
else if(estado_bateria->en_armaz_kwh >= en_armaz_max_kwh)
   {
   estado_bateria->estado = estCARREGADA;
   if(bateria->Tipo_operacao == toMANUAL)
      {
      if(estado_bateria->modo_operacao == mopCARGA)
         {
         // Foi especificada operação de carga em bateria já carregada
         estado_bateria->modo_operacao = mopOCIOSA;
         estado_bateria->pext_kw       = bateria->P_ociosa_pu * bateria->Pnom_kw;
         estado_bateria->qext_kvar     = bateria->Q_ociosa_pu * bateria->Pnom_kw;
         }
      }
   else  // toAUTO
      {
      estado_bateria->modo_operacao = mopDESCARGA;
      }
   }
else
   {
   estado_bateria->estado = estINTERMEDIARIO;
   }

return(true);
}

//---------------------------------------------------------------------------
VTEqpto* __fastcall TData::ExisteEqpto(TList *lisEQP, int eqpto_id)
   {
   //variávies locais
   VTEqpto *eqpto;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Id == eqpto_id) return(eqpto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::ExisteGeracaoPV_VF(VTEqpto *eqpto, int ind_pat)
   {
   //variáveis locais
   VTNo         *no;
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   //determina No
   no = (VTNo*)eqpto;
   //loop p/ todos Suprimentos
   for (int n = 0; n < PD.lisSUPRIMENTO->Count; n++)
      {
      suprimento = (VTSuprimento*)PD.lisSUPRIMENTO->Items[n];
      //verifica se o Suprimento está associado a Barra indicada
      if (suprimento->pbarra->No[net] != no) continue;
      //verifica se o Suprimento não é do tipo PV ou VF
      if ((suprimento->tiposup != supPV)&&(suprimento->tiposup != supVF)) continue;
      //verifica se o Suprimento está ativo no patamar
      if (fornecimento->Ativo[suprimento][ind_pat]) return(true);
      }
   //loop p/ todos Geradores
   for (int n = 0; n < PD.lisGERADOR->Count; n++)
      {
      gerador = (VTGerador*)PD.lisGERADOR->Items[n];
      //verifica se o Gerador está associado a Barra indicada
      if (gerador->pbarra->No[net] != no) continue;
      //verifica se o Gerador não é do tipo PV ou VF
      if ((gerador->tiposup != supPV)&&(gerador->tiposup != supVF)) continue;
      //verifica se o Gerador está ativo no patamar
      if (fornecimento->Ativo[gerador][ind_pat]) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Geracao(VTEqpto    *eqpto,
                               int        ind_pat,
                               bool       zona_unica,
                               smcGERACAO *smc_geracao)
   {
   //variáveis locais
   double  defasagem[MAX_FASE] = {0., -2*M_PI/3, 2*M_PI/3, 0.};
   strSUP  vet_sup[MAX_FASE], str_sup;
   VTEqbar *eqbar = (VTEqbar*)eqpto;

   //reinicia  smcGERACAO
   smc_geracao->ativa = false;
   for(int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      smc_geracao->ger[ind_fase].p_mw   = 0.;
      smc_geracao->ger[ind_fase].q_mvar = 0.;
      if(ind_fase < (MAX_FASE-1)) smc_geracao->ger[ind_fase].v_pu = 1.; // A, B ou C
      else                        smc_geracao->ger[ind_fase].v_pu = 0.; // Neutro
      smc_geracao->ger[ind_fase].t_rad = defasagem[ind_fase];
      }
   //proteção: verifica se eqpto é Gerador ou Suprimento
   if ((eqpto->Tipo() != eqptoSUPRIMENTO) && (eqpto->Tipo() != eqptoGERADOR)) return(false);
   // Retorna se o Gerador ou Suprimento está inativo no patamar
   smc_geracao->ativa = fornecimento->Ativo[eqpto][ind_pat];
   if (! smc_geracao->ativa) return(true);
   //verifica se está usando fluxo equilibrado
   if (zona_unica)
      {//obtém valores de PQVT trifásico
      if (! fornecimento->Get_PQVT_trifasico(eqpto, ind_pat, &str_sup)) return(false);
      //transforma valores trifásicos em valore por fase
      for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {
         smc_geracao->ger[ind_fase].p_mw   = str_sup.p / 3.;
         smc_geracao->ger[ind_fase].q_mvar = str_sup.q / 3.;
         smc_geracao->ger[ind_fase].v_pu   = str_sup.v;
         smc_geracao->ger[ind_fase].t_rad  = str_sup.t + defasagem[ind_fase];
         }
      return(true);
      }
   else
      {//obtém valores de PQVT por fase
      if (! fornecimento->Get_PQVT_porfase(eqpto, ind_pat, vet_sup)) return(false);
      //copia os valores por fase para smc_geracao
      for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {
         smc_geracao->ger[ind_fase].p_mw   = vet_sup[ind_fase].p;
         smc_geracao->ger[ind_fase].q_mvar = vet_sup[ind_fase].q;
         smc_geracao->ger[ind_fase].v_pu   = vet_sup[ind_fase].v;
         smc_geracao->ger[ind_fase].t_rad  = vet_sup[ind_fase].t + eqbar->pbarra->zona->Defasagem_rad;
         }
      return(true);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Inicia(VTApl *apl, VTNet *net)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->net    = net;
   demanda      = (VTDemanda *)      apl->GetObject(__classid(VTDemanda));
   fases        = (VTFases *)        apl->GetObject(__classid(VTFases));
   fornecimento = (VTFornecimento *) apl->GetObject(__classid(VTFornecimento));
   patamares    = (VTPatamares *)    apl->GetObject(__classid(VTPatamares));

   //cria objetos locais
   montaZ              = DLL_NewObjMontaZ(apl);
   rescurto            = NewObjResCurto(apl);
   resestimador        = NewObjResEstimador(apl);
   resfluxo            = NewObjResFluxo(apl);
   rescurto->Sbase_mva = Sbase;
   resfluxo->Sbase_mva = Sbase;
   //define nome default da rede
   DefineNomeRede();
   //inicia dados c/ valores default
   ErroFluxo               = rfSemErro; // Indicador de erro no cálculo de fluxo de potência
   AnoCarga                = 0;
   AterramentosComuns      = false;
   BarraInternaSuprimentos = false;
   CapacitanciaLinhas      = true;
   GravaYnodal             = false;
   IncluiCNL               = true;
   MaxIteracao             = 100;
   MaxIteracaoVDNRM        = 10;
   // FluxoH3
   NumeroHarmonicas        = 0;
   // Alterado em 2013.08.28
   //        Precisao         = 1.e-8;
   Precisao                = 1.e-6;
   UsaPiEquivalente        = false;
   UsaVDNRM                = true;
   //barras
   IniciaLisNo(PD.lisNO);
   //chaves
   IniciaLisChave(PD.lisCHAVE);
   //carga: insere Carga somente se RedeComCarga
   PD.lisCARGA->Clear();
   if(RedeComCarga) {IniciaLisEqbar(PD.lisCARGA, eqptoCARGA);}

   //demais eqbar
   IniciaLisEqbar(PD.lisBATERIA,     eqptoBATERIA);
   IniciaLisEqbar(PD.lisCAPACITOR,   eqptoCAPACITOR);
   IniciaLisEqbar(PD.lisCNL,         eqptoCARGANL);
   IniciaLisEqbar(PD.lisFILTRO,      eqptoFILTRO);
   IniciaLisEqbar(PD.lisGERADOR,     eqptoGERADOR);
   IniciaLisEqbar(PD.lisMEDIDOR,     eqptoMEDIDOR);  // ESTIM1/ESTIM3
   IniciaLisEqbar(PD.lisREATOR,      eqptoREATOR);
   IniciaLisEqbar(PD.lisSUPRIMENTO,  eqptoSUPRIMENTO);
   IniciaLisEqbar(PD.lisYREF,        eqptoYREF);
   IniciaLisEqbar(PD.lisTRAFOZZ,     eqptoTRAFOZZ);
   //ligacao
   IniciaLisLigacao(PD.lisCAPSERIE,  eqptoCAPSERIE);
   IniciaLisRegulador(PD.lisREGULADOR);
   IniciaLisLigacao(PD.lisTRAFO2E,   eqptoTRAFO);
   IniciaLisLigacao(PD.lisTRAFO3E,   eqptoTRAFO3E);
   IniciaLisLigacao(PD.lisTRAMO,     eqptoTRAMO);
   IniciaLisLigacao(PD.lisTRECHO,    eqptoTRECHO);
   //arranjos (OBS: deve ser iniciado após iniciar lista de Trechos)
   //IniciaLisArranjo(PD.lisARRANJO);
   //mutua (OBS: deve ser iniciado após iniciar lista de Trechos)
   IniciaLisMutua(PD.lisMUTUA);
   //harmônicas
   IniciaStrListHarmonica();

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaLisChave(TList *lisCHV)
   {
   //reinicia lista
   lisCHV->Clear();
   //verifica se deve incluir as Chaves
   if (IncluirChave)
      {//inicia lisCHV c/ todas as Chaves
      net->LisLigacao(lisCHV, eqptoCHAVE);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaLisEqbar(TList *lisEQB, int tipo)
   {
   //reinicia lista
   lisEQB->Clear();
   //inicia lisEQB c/ todos Eqbar
   net->LisEqbar(lisEQB, tipo);
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaLisNo(TList *lisNO)
   {
   //reinicia lista
   lisNO->Clear();
   //copia lista de Nos
   CopiaTList(net->LisNo(), lisNO);
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaLisRegulador(TList *lisREG)
   {
   //variáveis locais
   VTRegulador *regulador;

   //inicia lisEQP c/ todos os Reguladores
   IniciaLisLigacao(PD.lisEQP, eqptoREGULADOR);
   //reinicia lista
   lisREG->Clear();
   //inicia lisLIG com as unidades monopolares
   for (int n = 0; n < PD.lisEQP->Count; n++)
      {
      regulador = (VTRegulador*)PD.lisEQP->Items[n];
      regulador->IniciaLisUM();
      CopiaTList(regulador->LisUM(), lisREG);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaLisLigacao(TList *lisLIG, int tipo)
   {
   //reinicia lista
   lisLIG->Clear();
   //inicia lisLIG c/ todas Ligacoes
   net->LisLigacao(lisLIG, tipo);
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaLisMutua(TList *lisMUT)
   {
   //reinicia lista
   lisMUT->Clear();
   //inicia lisMUT c/ todas Mútuas
   CopiaTList(net->LisMutua(), lisMUT);
   }

//---------------------------------------------------------------------------
void __fastcall TData::IniciaStrListHarmonica(void)
   {
   //variáveis locais
   // VTCarga *carga;
   TList   *lisCAR = PD.lisCARGA;

   PD.harmonicas->Clear();
   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      // carga = (VTCarga*)lisCAR->Items[nc];
      //verifica se a Carga possui harmônica
int PENDENCIA_CARGA_COM_HARMONICA=1;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TData::InsereRelatorio(AnsiString arq_relatorio)
   {
   //verifica se o arquivo do relatório já existe
   if (PD.relatorios->IndexOf(arq_relatorio) < 0)
      {//insere relatório
      PD.relatorios->Add(arq_relatorio);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TData::Medicao(int ind_pat, smcMEDICAO &smc_medicao)
{
// Variáveis locais
VTCanal   *canal;
VTPatamar *patamar = patamares->Patamar[ind_pat];

// Zera dados de medição
smc_medicao.valor = 0.;

// Determina canal
canal = (VTCanal *) smc_medicao.canal;
if(canal == NULL) return(false);  // Proteção
smc_medicao.valor = canal->ValorMedido[patamar];

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetAnoCarga(void)
   {
   return(PD.ano_carga);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetAterramentosComuns(void)
   {
   return(PD.aterr_comuns);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetBarraInternaSuprimentos(void)
   {
   return(PD.bar_int_sup);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetCapacitanciaLinhas(void)
   {
   return(PD.capac_LT);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetCargas_Ynodal(void)
   {
   return(ModeloCarga == cargaZCTE);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetErroFluxo(void)
   {
   return(PD.erro_fluxo);
   }

//---------------------------------------------------------------------------
double __fastcall TData::PM_GetFrequenciaHz(void)
   {
   return(PD.frequencia_hz);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetGravaYnodal(void)
   {
   return(PD.grava_ynodal);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetHarmonica(int index)
   {
   //proteção
   if (index >= PD.harmonicas->Count) return(-1);
   return(StrToInteger(PD.harmonicas->Strings[index]));
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetIncluiCNL(void)
   {
   return(PD.inclui_CNL);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetIncluirChave(void)
   {
   return(PD.incluir_chave);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetMaxIteracao(void)
   {
   return(PD.max_iteracao);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetMaxIteracaoVDNRM(void)
   {
   return(PD.max_iteracao_vdnrm);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetModeloCarga(void)
   {
   return(PD.modelo_carga);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroArranjo(void)
   {
   return(PD.lisARRANJO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroAterramento(void)
   {
   return(PD.lisATERRAMENTO->Count);
   }
   
//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroBarra(void)
   {
   return(PD.lisNO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroBateria(void)
   {
   return(PD.lisBATERIA->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroCapacitor(void)
   {
   return(PD.lisCAPACITOR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroCapserie(void)
   {
   return(PD.lisCAPSERIE->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroCarga(void)
   {
   return(PD.lisCARGA->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroChave(void)
   {
   return(PD.lisCHAVE->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroCNL(void)
   {
   return(PD.lisCNL->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroFase(void)
   {
   return(MAX_FASE);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroFiltro(void)
   {
   return(PD.lisFILTRO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroGerador(void)
   {
   return(PD.lisGERADOR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroHarmonicas(void)
   {
   return(PD.num_harmonicas);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroLigacao(void)
   {
   return(PD.lisLIGACAO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroMedidor(void)
   {
   return(PD.lisMEDIDOR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroMutua(void)
   {
   return(PD.lisMUTUA->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroPatamar(void)
   {
   return(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroReator(void)
   {
   return(PD.lisREATOR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroRegulador(void)
   {
   return(PD.lisREGULADOR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroSuprimento(void)
   {
   return(PD.lisSUPRIMENTO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroTrafo2E(void)
   {
   return(PD.lisTRAFO2E->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroTrafo3E(void)
   {
   return(PD.lisTRAFO3E->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroTrafoZZ(void)
   {
   return(PD.lisTRAFOZZ->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroTramo(void)
   {
   return(PD.lisTRAMO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroTrecho(void)
   {
   return(PD.lisTRECHO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TData::PM_GetNumeroYref(void)
   {
   return(PD.lisYREF->Count);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TData::PM_GetNomeDaRede(void)
   {
   return(PD.nome_rede);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetPerdaFerroEnabled(void)
   {
   return(PD.perda_ferro_enabled);
   }

//---------------------------------------------------------------------------
double __fastcall TData::PM_GetPrecisao(void)
   {
   return(PD.precisao);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetRedeComCarga(void)
   {
   return(PD.rede_com_carga);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetRede4Fios(void)
   {
   return(net->Rede4Fios);
   }

//---------------------------------------------------------------------------
VTResCurto * __fastcall TData::PM_GetResCurto(void)
   {
   return(rescurto);
   }

//---------------------------------------------------------------------------
VTResEstimador * __fastcall TData::PM_GetResEstimador(void)
   {
   return(resestimador);
   }

//---------------------------------------------------------------------------
VTResFluxo * __fastcall TData::PM_GetResFluxo(void)
   {
   return(resfluxo);
   }

//---------------------------------------------------------------------------
double __fastcall TData::PM_GetSbase(void)
   {
   return(PD.sbase);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetUsaPiEquivalente(void)
   {
   return(PD.usa_pi_equiv);
   }

//---------------------------------------------------------------------------
bool __fastcall TData::PM_GetUsaVDNRM(void)
   {
   return(PD.usa_vdnrm);
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetAnoCarga(int ano_carga)
   {
   PD.ano_carga = ano_carga;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetAterramentosComuns(bool aterr_comuns)
   {
   PD.aterr_comuns = aterr_comuns;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetBarraInternaSuprimentos(bool bar_int_sup)
   {
   PD.bar_int_sup = bar_int_sup;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetCapacitanciaLinhas(bool capac_LT)
   {
   PD.capac_LT = capac_LT;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetErroFluxo(int erro_fluxo)
   {
   PD.erro_fluxo = erro_fluxo;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetFrequenciaHz(double frequencia_hz)
   {
   PD.frequencia_hz = frequencia_hz;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetGravaYnodal(bool grava_ynodal)
   {
   PD.grava_ynodal = grava_ynodal;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetIncluiCNL(bool inclui_CNL)
   {
   PD.inclui_CNL = inclui_CNL;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetIncluirChave(bool incluir_chave)
   {
   PD.incluir_chave = incluir_chave;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetMaxIteracao(int max_iteracao)
   {
   PD.max_iteracao = max_iteracao;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetMaxIteracaoVDNRM(int max_iteracao_vdnrm)
   {
   PD.max_iteracao_vdnrm = max_iteracao_vdnrm;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetModeloCarga(int modelo_carga)
   {
   PD.modelo_carga = modelo_carga;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetNomeDaRede(AnsiString nome_rede)
   {
   PD.nome_rede = nome_rede;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetNumeroHarmonicas(int num_harmonicas)
   {
   PD.num_harmonicas = num_harmonicas;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetPerdaFerroEnabled(bool enabled)
   {
   PD.perda_ferro_enabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetPrecisao(double precisao)
   {
   PD.precisao = precisao;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetRedeComCarga(bool rede_com_carga)
   {
   PD.rede_com_carga = rede_com_carga;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetSbase(double sbase)
   {
   PD.sbase = sbase;
   //atualiza objetos VTResCalc
   if (rescurto) rescurto->Sbase_mva = sbase;
   if (resfluxo) resfluxo->Sbase_mva = sbase;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetUsaPiEquivalente(bool usa_pi_equiv)
   {
   PD.usa_pi_equiv = usa_pi_equiv;
   }

//---------------------------------------------------------------------------
void __fastcall TData::PM_SetUsaVDNRM(bool usa_vdnrm)
   {
   PD.usa_vdnrm = usa_vdnrm;
   }

//---------------------------------------------------------------------------
void __fastcall TData::RedefineDeltaComoPrimario(smcTRAFO3E *smc_trafo3e)
   {
   switch(smc_trafo3e->grupo_lig)
      {
      // Configurações que devem ser rotacionadas uma vez à esquerda (ou
      // duas vezes à direita)
      case glYD11D11:   // 09
      case glYD1D1:     // 11
      case glYD11Y0:    // 13
      case glYD1Y2:     // 15
      case glYD11Y10:   // 17
      case glYD1Y0:     // 19
         RotacionaPriSecTer(smc_trafo3e);
         // NB: SEM "break" !!!

      // Configurações que devem ser rotacionadas uma vez à direita
      case glDY1D0:     // 08
      case glDY11D0:    // 10
      case glYY0D11:    // 12
      case glYY10D11:   // 14
      case glYY2D1:     // 16
      case glYY0D1:     // 18
         RotacionaPriSecTer(smc_trafo3e);
         break;

      default:  // Configurações básicas (01 a 07): nada a fazer
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TData::RotacionaPriSecTer(smcTRAFO3E *smc_trafo3e)
   {
   //variáveis locais
   struct {
          void            *barraCALC_1, *barraCALC_2, *barraCALC_3;
          strENROLAMENTO  pri, sec, ter;
          complex<double> zps0_pu, zps1_pu, zpt0_pu, zpt1_pu, zst0_pu, zst1_pu;
          } tmp;

   //rotaciona Pri=>Sec, Sec=>Ter, Ter=>Pri
   tmp.barraCALC_1 = smc_trafo3e->barraCALC_3;
   tmp.barraCALC_2 = smc_trafo3e->barraCALC_1;
   tmp.barraCALC_3 = smc_trafo3e->barraCALC_2;
   tmp.pri         = smc_trafo3e->ter;
   tmp.sec         = smc_trafo3e->pri;
   tmp.ter         = smc_trafo3e->sec;
   tmp.zps0_pu     = smc_trafo3e->zpt0_pu;
   tmp.zst0_pu     = smc_trafo3e->zps0_pu;
   tmp.zpt0_pu     = smc_trafo3e->zst0_pu;
   tmp.zps1_pu     = smc_trafo3e->zpt1_pu;
   tmp.zst1_pu     = smc_trafo3e->zps1_pu;
   tmp.zpt1_pu     = smc_trafo3e->zst1_pu;

   //salva nova configuração
   smc_trafo3e->barraCALC_1 = tmp.barraCALC_1;
   smc_trafo3e->barraCALC_2 = tmp.barraCALC_2;
   smc_trafo3e->barraCALC_3 = tmp.barraCALC_3;
   smc_trafo3e->pri         = tmp.pri;
   smc_trafo3e->sec         = tmp.sec;
   smc_trafo3e->ter         = tmp.ter;
   smc_trafo3e->zps0_pu     = tmp.zps0_pu;
   smc_trafo3e->zst0_pu     = tmp.zst0_pu;
   smc_trafo3e->zpt0_pu     = tmp.zpt0_pu;
   smc_trafo3e->zps1_pu     = tmp.zps1_pu;
   smc_trafo3e->zst1_pu     = tmp.zst1_pu;
   smc_trafo3e->zpt1_pu     = tmp.zpt1_pu;
   }

/* teste
//
//---------------------------------------------------------------------------
void __fastcall TData::LogDemandaCarga(VTEqpto *eqpto, complex<double> sf_mva[MAX_FASE])
   {
   //variáveis locais
   AnsiString txt;

   txt = eqpto->Codigo;
   for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
      {
      txt += "; " + ComplexToStr("%5.4f", sf_mva[ind_fase].real(), sf_mva[ind_fase].imag());
      }
   if (plog) plog->Add(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TData::LogDemandaCarga(VTEqpto *eqpto, complex<double> s_mva)
   {
   //variáveis locais
   AnsiString txt;

   txt = eqpto->Codigo;
   txt += "; " + ComplexToStr("%5.4f", s_mva.real(), s_mva.imag());
   if (plog) plog->Add(txt);
   }
*/

//---------------------------------------------------------------------------
//eof
