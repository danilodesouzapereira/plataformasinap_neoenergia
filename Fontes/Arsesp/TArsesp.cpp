//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Complexo\Complexo.h>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Radial\VTTronco.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTResFlowLig.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include "TArsesp.h"
#include "VTBDprodist.h"
#include "VTMedicao.h"
#include "TMedida.h"
#include "..\Demanda\VTAjusta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTArsesp* __fastcall NewObjArsesp(VTApl *apl_owner)
   {
   try{
      return(new TArsesp(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TArsesp::TArsesp(VTApl *apl_owner)
   {
   //salva ponteiro
   apl = apl_owner;
   //cria objetos
   tronco     = DLL_NewObjTronco(apl);
   lisCARGA   = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TArsesp::~TArsesp(void)
   {
   //destrói objetos
   if (tronco)   {delete tronco;   tronco   = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisCARGA) {delete lisCARGA; lisCARGA = NULL;}
   }

//---------------------------------------------------------------------------
double __fastcall TArsesp::CalculaDemandaNoPatamar(int ind_pat, TList *lisCARGA)
   {
   //variáveis locais
   complex<double> s_mva = CZero();
   VTCarga        *carga;
   VTDemanda      *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //calcula somatória das demandas das cargas primárias e equivalentes
   for (int nc = 0; nc < lisCARGA->Count; nc++)
      {
      carga = (VTCarga*)lisCARGA->Items[nc];
      //acumula demanda da Carga
      s_mva += demanda->S_mva[carga][ind_pat];
      }
   return(Abs(s_mva));
   }

//---------------------------------------------------------------------------
bool __fastcall TArsesp::Executa(VTPrimario *primario)
   {
   //salva ponteiro
   this->primario = primario;
   //proteção
   if ((primario == NULL)||(primario->BarraInicial == NULL)) return(false);
   //determina trechos do tronco e dos ramais
   tronco->DefinePrimario(primario, ind_pat_max);
   //inicia resumo
   IniciaResumo();
   //salva identificaçcao
   resumo.rede.cod_primario = primario->Rede->Codigo;
   if (primario->Subestacao)   resumo.rede.cod_etd = primario->Subestacao->Rede->Codigo;
   if (primario->BarraInicial) resumo.rede.vnom_kv = primario->BarraInicial->vnom;
   //determina patamares de maior e menor carregamento
   SelecionaPatamarMaiorMenorCarregamento();
   //salva dados a serem gravados na base de dados
   SalvaDadosTopologia();
   //SalvaDadosMedicao();
   //SalvaDadosAjusteDemanda();
   SalvaDadosFluxo();
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TArsesp::IniciaResumo(void)
   {
   //identificação
   resumo.rede.cod_primario = "";;
   resumo.rede.cod_etd      = "";;
   resumo.rede.vnom_kv      = 0;
   //mediçao
   resumo.medicao.i_faseA = 0;
   resumo.medicao.i_faseB = 0;
   resumo.medicao.i_faseC = 0;
   //ajuste de demanda
   resumo.ajuste.i_faseA = 0;
   resumo.ajuste.i_faseB = 0;
   resumo.ajuste.i_faseC = 0;
   //fluxo no patamar de menor carregamento
   resumo.fluxo_minimo.ind_pat   = 0;
   resumo.fluxo_minimo.v_min_pu  = 0;
   resumo.fluxo_minimo.dv_perc   = 0;
   //fluxo no patamar de maior carregamento
   resumo.fluxo_maximo.ind_pat   = 0;
   resumo.fluxo_maximo.v_min_pu  = 0;
   resumo.fluxo_minimo.dv_perc   = 0;
   //topologia
   resumo.topologia.num_cons_primario = 0;
   resumo.topologia.num_cons_sec_res  = 0;
   resumo.topologia.num_cons_sec_com  = 0;
   resumo.topologia.num_cons_sec_ind  = 0;
   resumo.topologia.num_cons_sec_rur  = 0;
   resumo.topologia.num_cons_sec_out  = 0;
   resumo.topologia.cabo_tronco       = 0;
   resumo.topologia.comp_total        = 0;
   resumo.topologia.comp_tronco       = 0;
   resumo.topologia.comp_ramal        = 0;
   //diagnostico
   resumo.diagnostico.sucesso    = true;
   resumo.diagnostico.observacao = "";
   resumo.diagnostico.t_mseg     = 0;
   }

//---------------------------------------------------------------------------
bool __fastcall TArsesp::SalvaDadosAjusteDemanda(void)
   {
   //variáveis locais
   int      ind_bar = 0;
   VTAjusta *ajusta_demanda = (VTAjusta*)apl->GetObject(__classid(VTAjusta));

   //salva fator de correção
   resumo.ajuste.fatcor_faseA = ajusta_demanda->FatorAjuste[indFASE_A];
   resumo.ajuste.fatcor_faseB = ajusta_demanda->FatorAjuste[indFASE_B];
   resumo.ajuste.fatcor_faseC = ajusta_demanda->FatorAjuste[indFASE_C];
   //salva correntes após ajuste
   resumo.ajuste.i_faseA = Abs(primario->TrechoInicial->resflow->Ia_amp[ind_bar][ind_pat_max]);
   resumo.ajuste.i_faseB = Abs(primario->TrechoInicial->resflow->Ib_amp[ind_bar][ind_pat_max]);
   resumo.ajuste.i_faseC = Abs(primario->TrechoInicial->resflow->Ic_amp[ind_bar][ind_pat_max]);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArsesp::SalvaDadosFluxo(void)
   {
   //variáveis locais
   double v_pu;

   //obtém menor tensão no tronco, no patamar de maior carregamento
   resumo.fluxo_maximo.ind_pat  = ind_pat_max;
   resumo.fluxo_maximo.v_min_pu = ValorReal(tronco->MenorTensaoTronco(ind_pat_max));
   resumo.fluxo_maximo.dv_perc  = (1. - resumo.fluxo_maximo.v_min_pu) * 100.;
   //obtém menor tensão no tronco, no patamar de menor carregamento
   resumo.fluxo_minimo.ind_pat  = ind_pat_min;
   resumo.fluxo_minimo.v_min_pu = ValorReal(tronco->MenorTensaoTronco(ind_pat_min));
   resumo.fluxo_minimo.dv_perc  = (1. - resumo.fluxo_minimo.v_min_pu) * 100.;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArsesp::SalvaDadosMedicao(void)
   {
   //variáveis locais
   TMedida   *medida;
   VTMedicao *medicao = (VTMedicao*)apl->GetObject(__classid(VTMedicao));

   //obtém medida do Primario
   if ((medida = medicao->ExisteMedida(primario)) != NULL)
      {
      resumo.medicao.i_faseA = medida->i_faseA;
      resumo.medicao.i_faseB = medida->i_faseB;
      resumo.medicao.i_faseC = medida->i_faseC;
      }
   else
      {//erro na leitura de medição
      resumo.medicao.i_faseA = 0;
      resumo.medicao.i_faseB = 0;
      resumo.medicao.i_faseC = 0;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArsesp::SalvaDadosTopologia(void)
   {
   //obtém comprimento total, comprimeto do tronco, cabo do tronco
   resumo.topologia.comp_total  = tronco->CompTroncoTrifasico() + tronco->CompRamalTrifasico();
   resumo.topologia.comp_tronco = tronco->CompTroncoTrifasico();
   resumo.topologia.cabo_tronco = tronco->CaboTroncoTrifasico();

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TArsesp::SelecionaPatamarMaiorMenorCarregamento(void)
   {
   //variáveis locais
   double      dem, dem_min, dem_max;
   VTPatamares *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat     = patamares->NumPatamar();

   //obtém lista de Cargas do Primario
   lisCARGA->Clear();
   primario->Rede->LisEqbar(lisCARGA, eqptoCARGA);
   //assume que a demanda máxima e mínima ocorrem no patamar 0
   ind_pat_min = ind_pat_max = 0;
   dem_min     = dem_max     = CalculaDemandaNoPatamar(ind_pat_min, lisCARGA);
   //loop p/ os demais Patamares
   for (int ind_pat = 1; ind_pat < num_pat; ind_pat++)
      {//calcula valores de demanda trifasica
      dem = CalculaDemandaNoPatamar(ind_pat, lisCARGA);
      if (dem > dem_max)
         {//salva demanda máxima
         dem_max     = dem;
         ind_pat_max = ind_pat;
         }
      if (dem < dem_min)
         {//salva demanda mínima
         dem_min     = dem;
         ind_pat_min = ind_pat;
         }
      }
   }

//---------------------------------------------------------------------------
double __fastcall TArsesp::ValorReal(double valor)
   {
   if (IsDoubleZero(valor)) valor = 0.;
   return(valor);
   }
//---------------------------------------------------------------------------
//eof

