//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TExportaEqpto.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Arranjo\VTPtoFixacao.h>
#include <Fontes\Arranjo\VTSuporte.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTBateria.h>
#include <Fontes\Rede\VTCanal.h>
#include <Fontes\Rede\VTCapacitor.h>
#include <Fontes\Rede\VTCapserie.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTCluster.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTFiltro.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTMedidor.h>
#include <Fontes\Rede\VTMutua.h>
#include <Fontes\Rede\VTReator.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTTrafoZZ.h>
#include <Fontes\Rede\VTTrecho.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTExportaEqpto* NewObjExportaEqpto(VTApl *apl)
   {
   return(new TExportaEqpto(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExportaEqpto::TExportaEqpto(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   //inicia dados
   Versao    = 5;
   Separador = ';';
   }

//---------------------------------------------------------------------------
__fastcall TExportaEqpto::~TExportaEqpto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::LigacaoAsStr(int ligacao)
   {
   switch(ligacao)
      {
      case lgEST_AT:        return("Yaterrado");
      case lgTRIANG:        return("Delta");
      case lgEST_ISO:       return("Yisolado");
      case lgMONOFASICO:    return("Monofasico");
      case lgDELTA_ABERTO:  return("DeltaAberto");
      case lgDELTA_FECHADO: return("DeltaFechado");
      default:              return("Indefinido");
      }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaArranjoCaboPorFase(VTArranjo *arranjo)
   {
   //variáveis locais
   VTPtoFixacao *pto_fixacao;
   int           fase_tag[MAX_FASE] = {faseA, faseB, faseC, faseN};
   //TList        *lisPtoFixacao     = arranjo->suporte->LisPtoFixacao();

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Arranjo
   if (arranjo->TipoCfg == arranjoCABOxFASE)
      {
      line.InsereCampo(arranjo->Obj);
      line.InsereCampo(arranjo->Codigo);
      line.InsereCampo(arranjo->Extern_id);
      line.InsereCampo(fases->AsStrABCN[arranjo->Fases]);
      line.InsereCampo(arranjo->suporte->Obj);
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {//identificação da fase como string
         line.InsereCampo(fases->AsStrABCN[fase_tag[ind_fase]]);
         //verifica se existe a fase
         if ((arranjo->Fases & fase_tag[ind_fase]) == fase_tag[ind_fase])
            {//ID do PtoFixação (número sequencial a partir de 0 que equivale ao índice do PtoFixacao na lista)
            //line.InsereCampo(lisPtoFixacao->IndexOf(arranjo->fase[ind_fase].pto_fixacao));
            line.InsereCampo(arranjo->fase[ind_fase].pto_fixacao->id);
            //Id do cabo
            line.InsereCampo(arranjo->fase[ind_fase].cabo->Obj);
            }
         else
            {//índice do PtoFixacao para a fase: inválido
            line.InsereCampo(-1);
            //Id do cabo para a fase: inválido
            line.InsereCampo(-1);
            }
         }
      }
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaArranjoCaboZ0Z1(VTArranjo *arranjo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Arranjo
   if (arranjo->TipoCfg == arranjoCABO_Z0Z1)
      {
      line.InsereCampo(arranjo->Obj);
      line.InsereCampo(arranjo->Codigo);
      line.InsereCampo(arranjo->Extern_id);
      line.InsereCampo(fases->AsStrABCN[arranjo->Fases]);
      line.InsereCampo(arranjo->caboZ0Z1->Obj);
      }
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaArranjoPUSb100(VTArranjo *arranjo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Arranjo
   if (arranjo->TipoCfg == arranjoPU_SB100)
      {
      line.InsereCampo(arranjo->Obj);
      line.InsereCampo(arranjo->Codigo);
      line.InsereCampo(arranjo->Extern_id);
      line.InsereCampo(fases->AsStrABCN[arranjo->Fases]);
      line.InsereCampo(arranjo->Iadm_amp, fmt2_1);
      line.InsereCampo(arranjo->z0.r,     fmt6_5);
      line.InsereCampo(arranjo->z0.x,     fmt6_5);
      line.InsereCampo(arranjo->c0,       fmt6_5);
      line.InsereCampo(arranjo->z1.r,     fmt6_5);
      line.InsereCampo(arranjo->z1.x,     fmt6_5);
      line.InsereCampo(arranjo->c1,       fmt6_5);
      }
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaArranjoZ0Z1(VTArranjo *arranjo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Arranjo
   if (arranjo->TipoCfg == arranjoZ0Z1)
      {
      line.InsereCampo(arranjo->Obj);
      line.InsereCampo(arranjo->Codigo);
      line.InsereCampo(arranjo->Extern_id);
      line.InsereCampo(fases->AsStrABCN[arranjo->Fases]);
      line.InsereCampo(arranjo->Iadm_amp, fmt2_1);
      line.InsereCampo(arranjo->z0.r,     fmt6_5);
      line.InsereCampo(arranjo->z0.x,     fmt6_5);
      line.InsereCampo(arranjo->c0,       fmt6_5);
      line.InsereCampo(arranjo->z1.r,     fmt6_5);
      line.InsereCampo(arranjo->z1.x,     fmt6_5);
      line.InsereCampo(arranjo->c1,       fmt6_5);
      }
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaBarra(VTBarra *barra)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Barra
   line.InsereCampo(barra->Obj);
   line.InsereCampo(barra->Codigo);
   line.InsereCampo(barra->Extern_id);
   line.InsereCampo(barra->vnom, fmt4_3);
   line.InsereCampo(double(barra->utm.x)/100., fmt3_2);
   line.InsereCampo(double(barra->utm.y)/100., fmt3_2);
   line.InsereCampo(barra->esq.x1);
   line.InsereCampo(barra->esq.y1);
   line.InsereCampo(barra->esq.x2);
   line.InsereCampo(barra->esq.y2);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaBateria(VTBateria *bateria)
   {
   //variáveis locais
   double valor[nvCURVA_PQ];
   strHM  hm_ini, hm_fim;
   int num_pto, dim_valor;

   if (bateria->curvaCargaDescarga != NULL)
   {
		num_pto   = bateria->curvaCargaDescarga->NumeroPontos();
		dim_valor = bateria->curvaCargaDescarga->NumeroValores;
   }

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Bateria
   line.InsereCampo(bateria->Obj);
   line.InsereCampo(bateria->Codigo);
   line.InsereCampo(bateria->Extern_id);
   line.InsereCampo(bateria->pbarra->Obj);
   line.InsereCampo(fases->AsStrABCN[bateria->Fases]);
   line.InsereCampo(bateria->Tipo_operacao);
   line.InsereCampo(bateria->Icte_pu, fmt2_1);
   line.InsereCampo(bateria->Scte_pu, fmt2_1);
   line.InsereCampo(bateria->Zcte_pu, fmt2_1);
   line.InsereCampo(bateria->ModeloCarga);
   line.InsereCampo(bateria->ModeloDesc);
   line.InsereCampo(bateria->Pnom_kw);
   line.InsereCampo(bateria->Enom_kwh);
   line.InsereCampo(bateria->Cap_inv_kva);
   line.InsereCampo(bateria->P_ociosa_pu);
   line.InsereCampo(bateria->Q_ociosa_pu);
   line.InsereCampo(bateria->Rend_carga_pu);
   line.InsereCampo(bateria->Rend_desc_pu);
   line.InsereCampo(bateria->Gatilho_carga_pu);
   line.InsereCampo(bateria->Gatilho_desc_pu);
   //monta pontos da curva de carga e descarga
   //insere número de pontos e número de valores por ponto
   line.InsereCampo(num_pto);
   line.InsereCampo(dim_valor);
   for (int index = 0; index < num_pto; index++)
      {
	  bateria->curvaCargaDescarga->GetPonto(index, hm_ini, hm_fim, valor, dim_valor);
      line.InsereCampo(hm_ini.hora);
      line.InsereCampo(hm_ini.minuto);
      line.InsereCampo(hm_fim.hora);
      line.InsereCampo(hm_fim.minuto);
      for (int ind_valor = 0; ind_valor < dim_valor; ind_valor++)
         {
         line.InsereCampo(valor[ind_valor] , fmt3_2); //acredito que ja está em kVA, irei verificar
         }
	  }
   //esquemático
   line.InsereCampo(bateria->esq.posbar, fmt2_1);
   line.InsereCampo(bateria->esq.dxbar);
   line.InsereCampo(bateria->esq.dybar);

   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCaboUnipolar(VTCabo *cabo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Cabo
   if (cabo->TipoCfg == caboR_GMR)
      {
      line.InsereCampo(cabo->Obj);
      line.InsereCampo(cabo->Codigo);
      line.InsereCampo(cabo->Extern_id);
      line.InsereCampo(cabo->Iadm_A, fmt2_1);
      line.InsereCampo(cabo->raio * 1000,   fmt4_3);   //mm
      line.InsereCampo(cabo->gmr  * 1000,   fmt4_3);   //mm
      line.InsereCampo(cabo->r,             fmt6_5);
      }
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCaboZ0Z1(VTCabo *cabo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Cabo
   if (cabo->TipoCfg == caboZ0Z1)
      {
      line.InsereCampo(cabo->Obj);
      line.InsereCampo(cabo->Codigo);
      line.InsereCampo(cabo->Extern_id);
      line.InsereCampo(cabo->Iadm_A, fmt2_1);
      line.InsereCampo(cabo->z0.r,   fmt6_5);
      line.InsereCampo(cabo->z0.x,   fmt6_5);
      line.InsereCampo(cabo->c0,     fmt6_5);
      line.InsereCampo(cabo->z1.r,   fmt6_5);
      line.InsereCampo(cabo->z1.x,   fmt6_5);
      line.InsereCampo(cabo->c1,     fmt6_5);
	  }
   return(line.Texto());
   }



//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCapacitor(VTCapacitor *capacitor)
   {
   //variáveis locais
   bool add_atr_tipica = true;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(capacitor->Obj);
   line.InsereCampo(capacitor->pbarra->Obj);
   line.InsereCampo(capacitor->Codigo);
   line.InsereCampo(capacitor->Extern_id);
   line.InsereCampo(capacitor->vnom, fmt4_3);
   line.InsereCampo(capacitor->Q_mvar * 1000., fmt2_1);
   line.InsereCampo(LigacaoAsStr(capacitor->ligacao));
   //monta dados da Curva
   MontaCurva(capacitor->curva, add_atr_tipica);
   //esquemático
   line.InsereCampo(capacitor->esq.posbar, fmt2_1);
   line.InsereCampo(capacitor->esq.dxbar);
   line.InsereCampo(capacitor->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCapserie(VTCapserie *capserie)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(capserie->Obj);
   line.InsereCampo(capserie->pbarra1->Obj);
   line.InsereCampo(capserie->pbarra2->Obj);
   line.InsereCampo(capserie->Codigo);
   line.InsereCampo(capserie->Extern_id);
   line.InsereCampo(capserie->vnom,         fmt4_3);
   line.InsereCampo(capserie->snom * 1000., fmt2_1);
   line.InsereCampo(capserie->z.r,          fmt5_4);
   line.InsereCampo(capserie->z.x,          fmt5_4);
   //esquemático
   line.InsereCampo(capserie->esq.posbar1, fmt2_1);
   line.InsereCampo(capserie->esq.posbar2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCargaComMedicao(VTCarga *carga)
   {
   //variáveis locais
   double valor[32];
   strHM  hm_ini, hm_fim;
   int    num_pto   = carga->curva->NumeroPontos();
   int    dim_valor = carga->curva->NumeroValores;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(carga->Obj);
   line.InsereCampo(carga->pbarra->Obj);
   line.InsereCampo(carga->Codigo);
   line.InsereCampo(carga->Extern_id);
   //grupo tarifário
   line.InsereCampo((carga->pbarra->vnom > 1.) ? "A" : "B");
   //classe de consumidor
   line.InsereCampo(carga->classe->Codigo);
   //fases e modelo de carga
   line.InsereCampo(fases->AsStrABCN[carga->Fases]);
   line.InsereCampo(carga->Icte_pu, fmt2_1);
   line.InsereCampo(carga->Scte_pu, fmt2_1);
   line.InsereCampo(carga->Zcte_pu, fmt2_1);
   //consumo em kWh/mês
   line.InsereCampo(carga->Energia_kwhmes, fmt2_1);
   //monta pontos da curva de fornecimento
   //MontaPontosCurva(carga->curva);
   //insere número de pontos e número de valores por ponto
   line.InsereCampo(num_pto);
   line.InsereCampo(dim_valor);
   for (int index = 0; index < num_pto; index++)
      {
      carga->curva->GetPonto(index, hm_ini, hm_fim, valor, dim_valor);
      line.InsereCampo(hm_ini.hora);
      line.InsereCampo(hm_ini.minuto);
      line.InsereCampo(hm_fim.hora);
      line.InsereCampo(hm_fim.minuto);
      for (int ind_valor = 0; ind_valor < dim_valor; ind_valor++)
         {
         line.InsereCampo(valor[ind_valor] * 1000., fmt3_2);    //M  ==> k
         }
      }
   //esquemático
   line.InsereCampo(carga->esq.posbar, fmt2_1);
   line.InsereCampo(carga->esq.dxbar);
   line.InsereCampo(carga->esq.dybar);
   //quantidade de consumidores por classe
   line.InsereCampo(carga->NumConsResidencial);
   line.InsereCampo(carga->NumConsComercial);
   line.InsereCampo(carga->NumConsIndustrial);
   line.InsereCampo(carga->NumConsRural);
   line.InsereCampo(carga->NumConsOutros);
   line.InsereCampo(carga->NumConsA4);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCargaIP(VTCarga *carga)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(carga->Obj);
   line.InsereCampo(carga->pbarra->Obj);
   line.InsereCampo(carga->Codigo);
   line.InsereCampo(carga->Extern_id);
   line.InsereCampo(fases->AsStrABCN[carga->Fases]);
   //número de lâmpadas, potência em W, e fator de potência em pu
   line.InsereCampo(carga->IP_NumLamp);
   line.InsereCampo(carga->IP_Pot_kw * 1000., fmt2_1);
   line.InsereCampo(carga->IP_Fatpot, fmt4_3);
   //esquemático
   line.InsereCampo(carga->esq.posbar, fmt2_1);
   line.InsereCampo(carga->esq.dxbar);
   line.InsereCampo(carga->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCargaSemMedicao(VTCarga *carga)
   {
   //variáveis locais
   strHM  hm;
   double valor[nvCURVA_PQ];

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(carga->Obj);
   line.InsereCampo(carga->pbarra->Obj);
   line.InsereCampo(carga->Codigo);
   line.InsereCampo(carga->Extern_id);
   //grupo tarifário
   line.InsereCampo((carga->pbarra->vnom > 1.) ? "A" : "B");
   //classe de consumidor
   line.InsereCampo(carga->classe->Codigo);
   //fases e modelo de carga
   line.InsereCampo(fases->AsStrABCN[carga->Fases]);
   line.InsereCampo(carga->Icte_pu, fmt2_1);
   line.InsereCampo(carga->Scte_pu, fmt2_1);
   line.InsereCampo(carga->Zcte_pu, fmt2_1);
   //consumo em kWh/mês
   line.InsereCampo(carga->Energia_kwhmes, fmt2_1);
   //esquemático
   line.InsereCampo(carga->esq.posbar, fmt2_1);
   line.InsereCampo(carga->esq.dxbar);
   line.InsereCampo(carga->esq.dybar);
   //quantidade de consumidores por classe
   line.InsereCampo(carga->NumConsResidencial);
   line.InsereCampo(carga->NumConsComercial);
   line.InsereCampo(carga->NumConsIndustrial);
   line.InsereCampo(carga->NumConsRural);
   line.InsereCampo(carga->NumConsOutros);
   line.InsereCampo(carga->NumConsA4);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaChave(VTChave *chave)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Chave
   line.InsereCampo(chave->Obj);
   line.InsereCampo(chave->pbarra1->Obj);
   line.InsereCampo(chave->pbarra2->Obj);
   line.InsereCampo(chave->TipoChave->Obj);
   line.InsereCampo(chave->TipoChave->Tipo);
   line.InsereCampo(chave->TipoChave->Codigo);
   line.InsereCampo(chave->Codigo);
   line.InsereCampo(chave->Extern_id);
   line.InsereCampo(chave->Inom_amp, fmt2_1);
   line.InsereCampo(int(chave->Estado));
   //JCG@2013.08.02
   line.InsereCampo(int(chave->ProtecaoAtiva));
   line.InsereCampo(int(chave->Telecomandada));
   line.InsereCampo(chave->TecnoComunicacao);
   //esquemático
   line.InsereCampo(chave->esq.posbar1, fmt2_1);
   line.InsereCampo(chave->esq.posbar2, fmt2_1);
   //insere tipo_gis e elo_fusivel
   line.InsereCampo(chave->TipoGIS);
   line.InsereCampo(chave->EloFusivel);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCurva(VTCurva *curva, bool add_atr_tipica)
   {
   //verifica versão
   if (Versao >= 2) return(MontaCurva_Versao2(curva));
   else             return(MontaCurva_Versao1(curva, add_atr_tipica));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCurva_Versao1(VTCurva *curva, bool add_atr_tipica)
   {
   //monta linha com dados da Curva típica
   line.InsereCampo(curva->Obj);
   line.InsereCampo(curva->Codigo);
   line.InsereCampo(curva->Extern_id);
   if (add_atr_tipica)
      {//inclui campo indicando curva típica ou não
      if (curva->Tipica) line.InsereCampo("tipica");
      else               line.InsereCampo("propria");
      }
   //tipo e unidade
   line.InsereCampo(curva->Tipo);
   line.InsereCampo(curva->TipoAsStr);
   line.InsereCampo(curva->Unidade);
   //verifica se existe Classe associada
   if (curva->Classe != NULL) line.InsereCampo(curva->Classe->Codigo);
   else                       line.InsereCampo("");
   line.InsereCampo(curva->LimiteInferior);
   line.InsereCampo(curva->LimiteSuperior);
   //monta pontos da curva de fornecimento
   MontaPontosCurva(curva);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCurva_Versao2(VTCurva *curva)
   {
   //monta linha com dados da Curva típica
   line.InsereCampo(curva->Obj);
   line.InsereCampo(curva->Codigo);
   line.InsereCampo(curva->Extern_id);
   //inclui campo indicando curva típica ou não
   if (curva->Tipica) line.InsereCampo("tipica");
   else               line.InsereCampo("propria");
   //dia da medição: campo incluído na Versao 2
   line.InsereCampo(curva->DiaMedicao);
   //tipo e unidade
   line.InsereCampo(curva->Tipo);
   line.InsereCampo(curva->TipoAsStr);
   line.InsereCampo(curva->Unidade);
   //verifica se existe Classe associada
   if (curva->Classe != NULL) line.InsereCampo(curva->Classe->Codigo);
   else                       line.InsereCampo("");
   line.InsereCampo(curva->LimiteInferior);
   line.InsereCampo(curva->LimiteSuperior);
   //monta pontos da curva de fornecimento
   MontaPontosCurva(curva);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCurvaTipica(VTCurva *curva)
   {
   //variáveis locais
   bool add_atr_tipica=false;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   return(MontaCurva(curva, add_atr_tipica));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15928772
AnsiString __fastcall TExportaEqpto::MontaDiagnostico(double limites[27])
   {
   int nc = 0;
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados dos limites de diag. para fluxo
   //carrega valores redeSEC
   line.InsereCampo(limites[nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   //carrega valores redePRI
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   //carrega valores redeSUB
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   line.InsereCampo(limites[++nc],  fmt4_3);
   //cores
   line.InsereCampo(limites[++nc],  fmt2_1);
   line.InsereCampo(limites[++nc],  fmt2_1);
   line.InsereCampo(limites[++nc],  fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaFiltro(VTFiltro *filtro)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(filtro->Obj);
   line.InsereCampo(filtro->pbarra->Obj);
   line.InsereCampo(filtro->Codigo);
   line.InsereCampo(filtro->Extern_id);
   line.InsereCampo(filtro->vnom,         fmt4_3);
   line.InsereCampo(filtro->snom * 1000., fmt2_1);
   line.InsereCampo(filtro->z0.r,         fmt6_5);
   line.InsereCampo(filtro->z0.x,         fmt6_5);
   //esquemático
   line.InsereCampo(filtro->esq.posbar, fmt2_1);
   line.InsereCampo(filtro->esq.dxbar);
   line.InsereCampo(filtro->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaGerador(VTGerador *gerador)
   {
   //variáveis locais
   bool add_atr_tipica = true;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Gerador
   line.InsereCampo(gerador->Obj);
   line.InsereCampo(gerador->pbarra->Obj);
   line.InsereCampo(gerador->Codigo);
   line.InsereCampo(gerador->Extern_id);
   line.InsereCampo(gerador->FonteEnergia);
   line.InsereCampo(gerador->vnom, fmt4_3);
   line.InsereCampo(gerador->snom * 1000., fmt4_3);
   line.InsereCampo(gerador->fplim,        fmt4_3);
   line.InsereCampo(gerador->z0.r, fmt6_5);
   line.InsereCampo(gerador->z0.x, fmt6_5);
   line.InsereCampo(gerador->z1.r, fmt6_5);
   line.InsereCampo(gerador->z1.x, fmt6_5);
   line.InsereCampo(gerador->ligacao);
   line.InsereCampo(gerador->tiposup);
   //insere Curva do Gerador
   MontaCurva(gerador->curva, add_atr_tipica);
   //esquemático
   line.InsereCampo(gerador->esq.posbar, fmt2_1);
   line.InsereCampo(gerador->esq.dxbar);
   line.InsereCampo(gerador->esq.dybar);
   //fases
   line.InsereCampo(gerador->Fases);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaMedidor(VTMedidor *medidor)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Gerador
   line.InsereCampo(medidor->Obj);
   line.InsereCampo(medidor->pbarra->Obj);
   line.InsereCampo(medidor->Codigo);
   line.InsereCampo(medidor->Extern_id);
   //esquemático
   line.InsereCampo(medidor->esq.posbar, fmt2_1);
   line.InsereCampo(medidor->esq.dxbar);
   line.InsereCampo(medidor->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaMedidorCanal(VTCanal *canal)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Canal
   line.InsereCampo(canal->Obj);
   line.InsereCampo(canal->Medidor->Obj);
   line.InsereCampo(canal->Eqpto->Tipo());
   line.InsereCampo(canal->Eqpto->Obj);
   line.InsereCampo(canal->Codigo);
   line.InsereCampo(canal->Numero);
   line.InsereCampo(canal->TipoCanal);
   line.InsereCampo(canal->Fases);
   line.InsereCampo(canal->Tolerancia_perc);
   //insere pontos da Curva do Canal
   MontaCurva(canal->Curva);

   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaMutua(VTMutua *mutua)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Gerador
   line.InsereCampo(mutua->Obj);
   line.InsereCampo(mutua->ptre1->Obj);
   line.InsereCampo(mutua->pbar_tre1->Obj);
   line.InsereCampo(mutua->ptre2->Obj);
   line.InsereCampo(mutua->pbar_tre2->Obj);
   line.InsereCampo(mutua->Codigo);
   line.InsereCampo(mutua->Extern_id);
   line.InsereCampo(mutua->z0.r, fmt6_5);
   line.InsereCampo(mutua->z0.x, fmt6_5);
   line.InsereCampo(mutua->z1.r, fmt6_5);
   line.InsereCampo(mutua->postre1, fmt2_1);
   line.InsereCampo(mutua->postre2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaPontosCurva(VTCurva *curva)
   {
   //variáveis locais
   double valor[32];
   strHM  hm_ini, hm_fim;
   int    num_pto   = curva->NumeroPontos();
   int    dim_valor = curva->NumeroValores;

   try{//insere número de pontos e número de valores por ponto
      line.InsereCampo(num_pto);
      line.InsereCampo(dim_valor);
      for (int index = 0; index < num_pto; index++)
         {
         curva->GetPonto(index, hm_ini, hm_fim, valor, dim_valor);
         line.InsereCampo(hm_ini.hora);
         line.InsereCampo(hm_ini.minuto);
         line.InsereCampo(hm_fim.hora);
         line.InsereCampo(hm_fim.minuto);
         for (int n = 0; n < dim_valor; n++)
            {
            line.InsereCampo(valor[n], fmt6_5);
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaReator(VTReator *reator)
   {
   //variáveis locais
   bool add_atr_tipica = true;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(reator->Obj);
   line.InsereCampo(reator->pbarra->Obj);
   line.InsereCampo(reator->Codigo);
   line.InsereCampo(reator->Extern_id);
   line.InsereCampo(reator->ligacao);
   line.InsereCampo(reator->vnom);
   line.InsereCampo(reator->Q_mvar * 1000., fmt2_1);
   //monta dados da Curva
   MontaCurva(reator->curva, add_atr_tipica);
   //esquemático
   line.InsereCampo(reator->esq.posbar, fmt2_1);
   line.InsereCampo(reator->esq.dxbar);
   line.InsereCampo(reator->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaRede(VTRede *rede)
   {
   //variáveis locais
   int     valor;
   VTBarra *barra;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Rede
   line.InsereCampo(rede->Obj);
   line.InsereCampo(rede->TipoRede->Obj);
   line.InsereCampo(rede->TipoRede->Segmento);
   line.InsereCampo(rede->TipoRede->Codigo);
   line.InsereCampo(rede->Codigo);
   line.InsereCampo(rede->Extern_id);
   line.InsereCampo("Área única");
   //define TColor nas componentes RGB
   line.InsereCampo((rede->Color      ) & 0xff);    //r
   line.InsereCampo((rede->Color >>  8) & 0xff);    //g
   line.InsereCampo((rede->Color >> 16) & 0xff);    //b
   //cluster
   if (rede->cluster != NULL) line.InsereCampo(rede->cluster->Codigo);
   else                       line.InsereCampo("");
   //barra inicial
   barra = rede->BarraInicial();
   valor = (barra != NULL) ? int(barra->Obj) : 0;
   line.InsereCampo(valor);
   //radial
   valor = rede->Radial ? 1 : 2;
   line.InsereCampo(valor);
   //estática
   valor = rede->Estatica ? 1 : 2;
   line.InsereCampo(valor);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
void __fastcall TExportaEqpto::MontaRegulador(VTRegulador *regulador)
   {
   //monta linha com dados do Regulador
   line.InsereCampo(regulador->Obj);
   line.InsereCampo(regulador->pbarra1->Obj);
   line.InsereCampo(regulador->pbarra2->Obj);
   //barra de entrada
   line.InsereCampo(regulador->sentido.bar_ref->Obj);
   //modelo
   line.InsereCampo(regulador->modelo);
   line.InsereCampo(regulador->Codigo);
   line.InsereCampo(regulador->Extern_id);
   line.InsereCampo(regulador->snom * 1000., fmt2_1);
   line.InsereCampo(regulador->z0.r, fmt5_4);
   line.InsereCampo(regulador->z0.x, fmt5_4);
   line.InsereCampo(regulador->z1.r, fmt5_4);
   line.InsereCampo(regulador->z1.x, fmt5_4);
   //número de unidades, tipo de ligação e fases
   line.InsereCampo(regulador->NumUM());
   line.InsereCampo(LigacaoAsStr(regulador->ligacao));
   line.InsereCampo(fases->AsStrABCN[regulador->fases]);
   //
   //variação máxima de tensão
   line.InsereCampo(regulador->var_tensao, fmt2_1);
   //número de passos
   line.InsereCampo(regulador->num_passo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaReguladorAutomatico(VTRegulador *regulador)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta dados do Regulador
   MontaRegulador(regulador);
   //dados do ajuste automático: barra e tensão controlada
   line.InsereCampo(regulador->cfg_auto.bar_ref->Obj);
   line.InsereCampo(regulador->cfg_auto.v_pu, fmt4_3);
   //sentido inverso
   line.InsereCampo(regulador->cfg_auto.bar_ref_inv->Obj);
   line.InsereCampo(regulador->cfg_auto.vinv_pu, fmt4_3);
   //dado da banda morta
   line.InsereCampo(regulador->cfg_auto.larg_bandamorta_pu, fmt4_3);
   line.InsereCampo(regulador->cfg_auto.larg_bandamorta_inv_pu, fmt4_3);
   //esquemático
   line.InsereCampo(regulador->esq.posbar1, fmt2_1);
   line.InsereCampo(regulador->esq.posbar2, fmt2_1);
   //relação de tap
   line.InsereCampo(regulador->rel_tap);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaReguladorTapFixo(VTRegulador *regulador)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta dados do Regulador
   MontaRegulador(regulador);
   //dados do ajsute fixo: passo
   line.InsereCampo(regulador->cfg_fixo.passo);
   //esquemático
   line.InsereCampo(regulador->esq.posbar1, fmt2_1);
   line.InsereCampo(regulador->esq.posbar2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaSuporte(VTSuporte *suporte)
   {
   //variáveis locais
   VTPtoFixacao *pto_fixacao;
   TList        *lisPtoFixacao = suporte->LisPtoFixacao();

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Arranjo
   line.InsereCampo(suporte->Obj);
   line.InsereCampo(suporte->Codigo);
   line.InsereCampo(suporte->Extern_id);
   line.InsereCampo(suporte->TipoDeSuporteAsString());
   //OBS: este campo não existe no XML mas é obtido da lista
   line.InsereCampo(lisPtoFixacao->Count);
   for (int index = 0; index < lisPtoFixacao->Count; index++)
      {
      pto_fixacao = (VTPtoFixacao*)lisPtoFixacao->Items[index];
      //ID do PtoFixação (número sequencial a partir de 0 que equivale ao índice do PtoFixacao na lista)
      //line.InsereCampo(index);
      line.InsereCampo(pto_fixacao->id);
      //OBS: este campo não existe no XML
      line.InsereCampo(pto_fixacao->codigo);
      line.InsereCampo(pto_fixacao->coord.x, fmt4_3);
      line.InsereCampo(pto_fixacao->coord.y, fmt4_3);
      }
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaSuprimento(VTSuprimento *suprimento)
   {
   //variáveis locais
   double valor[32];
   strHM  hm_ini, hm_fim;
   int    num_pto   = suprimento->curva->NumeroPontos();
   int    dim_valor = suprimento->curva->NumeroValores;

   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Gerador
   line.InsereCampo(suprimento->Obj);
   line.InsereCampo(suprimento->pbarra->Obj);
   line.InsereCampo(suprimento->Codigo);
   line.InsereCampo(suprimento->Extern_id);
   line.InsereCampo(suprimento->tiposup);
   line.InsereCampo(fases->AsStrABCN[suprimento->Fases]);
   line.InsereCampo(suprimento->vnom, fmt4_3);
   line.InsereCampo(suprimento->smax * 1000., fmt4_3);
   line.InsereCampo(suprimento->zeq0.r, fmt6_5);
   line.InsereCampo(suprimento->zeq0.x, fmt6_5);
   line.InsereCampo(suprimento->zeq1.r, fmt6_5);
   line.InsereCampo(suprimento->zeq1.x, fmt6_5);
   //monta pontos da curva de forneceimento
   //MontaPontosCurva(suprimento->curva);
   //insere número de pontos e número de valores por ponto
   line.InsereCampo(num_pto);
   line.InsereCampo(dim_valor);
   for (int index = 0; index < num_pto; index++)
      {
      suprimento->curva->GetPonto(index, hm_ini, hm_fim, valor, dim_valor);
      line.InsereCampo(hm_ini.hora);
      line.InsereCampo(hm_ini.minuto);
      line.InsereCampo(hm_fim.hora);
      line.InsereCampo(hm_fim.minuto);
      line.InsereCampo(valor[indP] * 1000.  , fmt3_2);    //MW ==> kW
      line.InsereCampo(valor[indQ] * 1000.  , fmt3_2);    //MW ==> kW
      line.InsereCampo(valor[indV]          , fmt6_5);    //pu
      line.InsereCampo(RadToDeg(valor[indT]), fmt6_5);    //rad => graus
      }
   //esquemático
   line.InsereCampo(suprimento->esq.posbar, fmt2_1);
   line.InsereCampo(suprimento->esq.dxbar);
   line.InsereCampo(suprimento->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTipoChave(VTTipoChave *tipochave)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Classe
   line.InsereCampo(tipochave->Obj);
   line.InsereCampo(tipochave->Tipo);
   line.InsereCampo(tipochave->Codigo);
   line.InsereCampo(tipochave->Extern_id);
   switch(tipochave->Operacao)
      {
      case chaveOP_MANUAL:         line.InsereCampo("Manual");        break;
      case chaveOP_TELECOMANDADA:  line.InsereCampo("Telecomandada"); break;
      case chaveOP_AUTOMATICA:     line.InsereCampo("Automatica");    break;
      default:                     line.InsereCampo("Indefinida");    break;
      }
   line.InsereCampo((tipochave->Color      ) & 0xff);    //r
   line.InsereCampo((tipochave->Color >>  8) & 0xff);    //g
   line.InsereCampo((tipochave->Color >> 16) & 0xff);    //b
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTipoConsumidor(VTClasse *classe)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do TipoChave
   line.InsereCampo(-(abs(classe->Id)));
   line.InsereCampo(classe->Tag);
   line.InsereCampo(classe->Codigo);
   //cor associada à classe de consumdior
   line.InsereCampo((classe->Color      ) & 0xff);    //r
   line.InsereCampo((classe->Color >>  8) & 0xff);    //g
   line.InsereCampo((classe->Color >> 16) & 0xff);    //b
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTipoRede(VTTipoRede *tiporede)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do ArranjoTipoRede
   line.InsereCampo(tiporede->Obj);
   line.InsereCampo(tiporede->Segmento);
   line.InsereCampo(tiporede->Codigo);
   line.InsereCampo(tiporede->Extern_id);
   line.InsereCampo(tiporede->Descricao);
   line.InsereCampo(tiporede->Vmin_kv, fmt4_3);
   line.InsereCampo(tiporede->Vmax_kv, fmt4_3);
   line.InsereCampo((tiporede->Color      ) & 0xff);    //r
   line.InsereCampo((tiporede->Color >>  8) & 0xff);    //g
   line.InsereCampo((tiporede->Color >> 16) & 0xff);    //b
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrafoMonofasico(VTTrafo *trafo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Trafo
   line.InsereCampo(trafo->Obj);
   line.InsereCampo(trafo->Codigo);
   line.InsereCampo(trafo->Extern_id);
   line.InsereCampo(trafo->snom * 1000., fmt2_1);
//   line.InsereCampo(trafo->PerdaNoFerroMW() * 1e-6, fmt4_3);  //DVK 2015.10.23
   line.InsereCampo(trafo->PerdaNoFerroMW() * 1e6, fmt4_3);
   line.InsereCampo(trafo->snom * 1000., fmt2_1);
   line.InsereCampo(trafo->z1.r, fmt5_4);
   line.InsereCampo(trafo->z1.x, fmt5_4);
   //primário
   line.InsereCampo(trafo->pbarra1->Obj);
   line.InsereCampo(fases->AsStrABCN[trafo->pri.fase]);
   line.InsereCampo(trafo->pri.vnom, fmt4_3);
   line.InsereCampo(trafo->pri.tap, fmt4_3);
   line.InsereCampo(trafo->pri.zterra.r, fmt2_1);
   line.InsereCampo(trafo->pri.zterra.x, fmt2_1);
   //secundário
   line.InsereCampo(trafo->pbarra2->Obj);
   line.InsereCampo(fases->AsStrABCN[trafo->sec.fase]);
   line.InsereCampo(trafo->sec.vnom, fmt4_3);
   line.InsereCampo(trafo->sec.tap, fmt4_3);
   line.InsereCampo(trafo->sec.zterra.r, fmt2_1);
   line.InsereCampo(trafo->sec.zterra.x, fmt2_1);
   //ltc
   line.InsereCampo(trafo->ltc.tipo);
   if (trafo->ltc.tipo == ltcFIXO)
      {line.InsereCampo(1.0, fmt4_3);}
   else
      {line.InsereCampo(trafo->ltc.ajuste_auto.v_pu, fmt4_3);}
   //esquemático
   line.InsereCampo(trafo->esq.posbar1, fmt2_1);
   line.InsereCampo(trafo->esq.posbar2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrafoTrifasico2E(VTTrafo *trafo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Trafo
   line.InsereCampo(trafo->Obj);
   line.InsereCampo(trafo->Codigo);
   line.InsereCampo(trafo->Extern_id);
   line.InsereCampo(trafo->snom * 1000., fmt2_1);
   line.InsereCampo(trafo->snom * 1000., fmt2_1);
//   line.InsereCampo(trafo->PerdaNoFerroMW() * 1e-6, fmt4_3);  //DVK 2015.10.23
   line.InsereCampo(trafo->PerdaNoFerroMW() * 1e6, fmt4_3);
   line.InsereCampo(trafo->z0.r, fmt5_4);
   line.InsereCampo(trafo->z0.x, fmt5_4);
   line.InsereCampo(trafo->z1.r, fmt5_4);
   line.InsereCampo(trafo->z1.x, fmt5_4);
   //primário
   line.InsereCampo(trafo->pbarra1->Obj);
   line.InsereCampo(LigacaoAsStr(trafo->pri.ligacao));
   line.InsereCampo(trafo->pri.vnom, fmt4_3);
   line.InsereCampo(trafo->pri.tap, fmt4_3);
   line.InsereCampo(trafo->pri.zterra.r, fmt2_1);
   line.InsereCampo(trafo->pri.zterra.x, fmt2_1);
   //secundário
   line.InsereCampo(trafo->pbarra2->Obj);
   line.InsereCampo(LigacaoAsStr(trafo->sec.ligacao));
   line.InsereCampo(trafo->sec.vnom, fmt4_3);
   line.InsereCampo(trafo->sec.tap, fmt4_3);
   line.InsereCampo(trafo->sec.zterra.r, fmt2_1);
   line.InsereCampo(trafo->sec.zterra.x, fmt2_1);
   //ltc
   line.InsereCampo(trafo->ltc.tipo);
   if (trafo->ltc.tipo == ltcFIXO)
      {line.InsereCampo(1.0, fmt4_3);}
   else
      {line.InsereCampo(trafo->ltc.ajuste_auto.v_pu, fmt4_3);}
   //esquemático
   line.InsereCampo(trafo->esq.posbar1, fmt2_1);
   line.InsereCampo(trafo->esq.posbar2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrafoTrifasico3E(VTTrafo3E *trafo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Trafo
   line.InsereCampo(trafo->Obj);
   line.InsereCampo(trafo->Codigo);
   line.InsereCampo(trafo->Extern_id);
   //primário
   line.InsereCampo(trafo->pbarra1->Obj);
   line.InsereCampo(LigacaoAsStr(trafo->pri.ligacao));
   line.InsereCampo(trafo->pri.snom * 1000., fmt2_1);
   line.InsereCampo(trafo->pri.vnom, fmt4_3);
   line.InsereCampo(trafo->pri.tap, fmt4_3);
   line.InsereCampo(trafo->pri.zterra.r, fmt2_1);
   line.InsereCampo(trafo->pri.zterra.x, fmt2_1);
   //secundário
   line.InsereCampo(trafo->pbarra2->Obj);
   line.InsereCampo(LigacaoAsStr(trafo->sec.ligacao));
   line.InsereCampo(trafo->sec.snom * 1000., fmt2_1);
   line.InsereCampo(trafo->sec.vnom, fmt4_3);
   line.InsereCampo(trafo->sec.tap, fmt4_3);
   line.InsereCampo(trafo->sec.zterra.r, fmt2_1);
   line.InsereCampo(trafo->sec.zterra.x, fmt2_1);
   //terciário
   line.InsereCampo(trafo->pbarra3->Obj);
   line.InsereCampo(LigacaoAsStr(trafo->ter.ligacao));
   line.InsereCampo(trafo->ter.snom * 1000., fmt2_1);
   line.InsereCampo(trafo->ter.vnom, fmt4_3);
   line.InsereCampo(trafo->ter.tap, fmt4_3);
   line.InsereCampo(trafo->ter.zterra.r, fmt2_1);
   line.InsereCampo(trafo->ter.zterra.x, fmt2_1);
   //perda no ferro
//   line.InsereCampo(trafo->PerdaNoFerroMW() * 1e-6, fmt4_3);  //DVK 2015.10.23
   line.InsereCampo(trafo->PerdaNoFerroMW() * 1e6, fmt4_3);
   //potência de base para valores em pu
   line.InsereCampo(trafo->Sbase * 1000., fmt2_1);
   //impedâncias de sequência
   line.InsereCampo(trafo->zps0.r, fmt5_4);
   line.InsereCampo(trafo->zps0.x, fmt5_4);
   line.InsereCampo(trafo->zps1.r, fmt5_4);
   line.InsereCampo(trafo->zps1.x, fmt5_4);
   line.InsereCampo(trafo->zpt0.r, fmt5_4);
   line.InsereCampo(trafo->zpt0.x, fmt5_4);
   line.InsereCampo(trafo->zpt1.r, fmt5_4);
   line.InsereCampo(trafo->zpt1.x, fmt5_4);
   line.InsereCampo(trafo->zst0.r, fmt5_4);
   line.InsereCampo(trafo->zst0.x, fmt5_4);
   line.InsereCampo(trafo->zst1.r, fmt5_4);
   line.InsereCampo(trafo->zst1.x, fmt5_4);
   //esquemático
   line.InsereCampo(trafo->esq.posbar1, fmt2_1);
   line.InsereCampo(trafo->esq.posbar2, fmt2_1);
   line.InsereCampo(trafo->esq.posbar3, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrafoZZ(VTTrafoZZ *trafo)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados da Carga
   line.InsereCampo(trafo->Obj);
   line.InsereCampo(trafo->pbarra->Obj);
   line.InsereCampo(trafo->Codigo);
   line.InsereCampo(trafo->Extern_id);
   line.InsereCampo(trafo->vnom, fmt4_3);
   line.InsereCampo(trafo->snom * 1000., fmt2_1);
   line.InsereCampo(trafo->z0.r, fmt6_5);
   line.InsereCampo(trafo->z0.x, fmt6_5);
   //esquemático
   line.InsereCampo(trafo->esq.posbar, fmt2_1);
   line.InsereCampo(trafo->esq.dxbar);
   line.InsereCampo(trafo->esq.dybar);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrechoArranjoTipico(VTTrecho *trecho)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Trafo
   line.InsereCampo(trecho->Obj);
   line.InsereCampo(trecho->pbarra1->Obj);
   line.InsereCampo(trecho->pbarra2->Obj);
   line.InsereCampo(trecho->Codigo);
   line.InsereCampo(trecho->Extern_id);
   line.InsereCampo(trecho->Comprimento_m, fmt2_1);
   //arranjo típico
   line.InsereCampo(trecho->arranjo->Obj);
   //esquemático
   line.InsereCampo(trecho->esq.posbar1, fmt2_1);
   line.InsereCampo(trecho->esq.posbar2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrechoZ0Z1(VTTrecho *trecho)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;
   //monta linha com dados do Trafo
   line.InsereCampo(trecho->Obj);
   line.InsereCampo(trecho->pbarra1->Obj);
   line.InsereCampo(trecho->pbarra2->Obj);
   line.InsereCampo(trecho->Codigo);
   line.InsereCampo(trecho->Extern_id);
   line.InsereCampo(trecho->Comprimento_m, fmt3_2);
   //arranjo proprio
   line.InsereCampo(fases->AsStrABCN[trecho->arranjo->Fases]);
   line.InsereCampo(trecho->arranjo->Iadm_amp, fmt2_1);
   line.InsereCampo(trecho->arranjo->z0.r, fmt10_9);
   line.InsereCampo(trecho->arranjo->z0.x, fmt10_9);
   line.InsereCampo(trecho->arranjo->c0,   fmt10_9);
   line.InsereCampo(trecho->arranjo->z1.r, fmt10_9);
   line.InsereCampo(trecho->arranjo->z1.x, fmt10_9);
   line.InsereCampo(trecho->arranjo->c1,   fmt10_9);
   //esquemático
   line.InsereCampo(trecho->esq.posbar1, fmt2_1);
   line.InsereCampo(trecho->esq.posbar2, fmt2_1);
   return(line.Texto());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaVersao(void)
   {
   //reinicia Line
   line.Clear();
   line.Separador    = Separador;

   line.InsereCampo("Sinap");
   line.InsereCampo("Interface Texto");
   line.InsereCampo("Rev" + IntToStr(Versao));
   line.InsereCampo(DateToStr(Now()));
   return(line.Texto());
   }

//---------------------------------------------------------------------------
//eof
