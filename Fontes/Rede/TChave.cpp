//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TChave.h"
#include "VTResFlowLig.h"
#include "VTTipoChave.h"
#include "..\Constante\Fases.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TChave
//---------------------------------------------------------------------------
VTChave* __fastcall NewObjChave(int num_pat)
   {
   return(new TChave(num_pat));
   }
   
//---------------------------------------------------------------------------
__fastcall TChave::TChave(int num_pat)
   {
   //variávei locais
   int num_bar = 2;

   //cria objeto VTResFlowLig p/ armazenar resultados de fluxo
   AlocaResFlowLig(num_bar, num_pat);
   //define tipo do eqpto
   TipoChave = NULL;
   //inicia dados
   Fecha();
   Bloqueada        = false;
   TecnoComunicacao = comNAO_DISPONIVEL;
   Telecomandada    = false;
   ProtecaoAtiva    = false;
   EloFusivel       = "";
   //inicia atributos p/ visualização gráfica
   Show[lig_I_MAX] = true;
   }

//---------------------------------------------------------------------------
__fastcall TChave::~TChave(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TChave::Abre(void)
   {
   //atualiza estado da Chave
   //estado = estado chvABERTA;
   PD.estado &= ~chvFECHADA;
   }

//---------------------------------------------------------------------------
VTChave* __fastcall TChave::Clone(void)
   {
	//variáveis locais
   VTChave *pchave;

   //cria uma nova Chave
   if ((pchave = NewObjChave(resflow->Max_pat)) != NULL)
      {//copia dados desta Chave p/ a nova Chave
      pchave->CopiaAtributosDe(*this);
      }
   return(pchave);
   }

//---------------------------------------------------------------------------
void __fastcall TChave::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TChave &chave = (TChave&)ref;

   //proteção
   if (ref.Tipo() != eqptoCHAVE) return;
   //copia atributos de TLigacao
   TLigacao::CopiaAtributosDe(chave);
   //copia atributos de VTChave
   PD = chave.PD;
   }

//---------------------------------------------------------------------------
int __fastcall TChave::Fases(VTBarra *barra)
	{
	return(faseABC);
	}

//---------------------------------------------------------------------------
void __fastcall TChave::Fecha(void)
   {
   //atualiza estado da Chave
   //estado = chvFECHADA;
   PD.estado |= chvFECHADA;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::MudaEstado(void)
   {
   if (Aberta) Fecha();
   else        Abre();
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetAberta(void)
   {
   //return(estado == chvABERTA);
   return((PD.estado & chvFECHADA) == chvABERTA);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetBloqueada(void)
   {
   return((PD.estado & chvBLOQUEADA) == chvBLOQUEADA);
   }

//---------------------------------------------------------------------------
double __fastcall TChave::PM_GetCapDisruptiva_amp(void)
   {
   return(PD.cap_disruptiva_amp);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TChave::PM_GetEloFusivel(void)
   {
   return(PD.elo_fusivel);
   }

//---------------------------------------------------------------------------
unsigned __fastcall TChave::PM_GetEstado(void)
   {
   return(PD.estado);
   }

//---------------------------------------------------------------------------
unsigned __fastcall TChave::PM_GetEstadoNormal(void)
   {
   return(PD.estado_normal);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetFechada(void)
   {
   //return(estado == chvFECHADA);
   return((PD.estado & chvFECHADA) == chvFECHADA);
   }

//---------------------------------------------------------------------------
double __fastcall TChave::PM_GetInom_amp(void)
   {
   return(PD.corn_amp);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetProtecaoAtiva(void)
   {
   return(PD.protecao_ativa);
   }

//---------------------------------------------------------------------------
int __fastcall TChave::PM_GetTecnoComunicacao(void)
   {
   return(PD.tecno_comunicacao);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetTelecomandada(void)
   {
   return(PD.telecomandada);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetTipoBaseFusivel(void)
   {
   return(PD.tipochave->Tipo == chaveBF);
   }

//---------------------------------------------------------------------------
VTTipoChave* __fastcall TChave::PM_GetTipoChave(void)
   {
   return(PD.tipochave);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetTipoDisjuntor(void)
   {
   return(PD.tipochave->Tipo == chaveDJ);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TChave::PM_GetTipoGIS(void)
   {
   return(PD.tipo_gis);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetTipoReligadora(void)
   {
   return(PD.tipochave->Tipo == chaveRA);
   }

//---------------------------------------------------------------------------
bool __fastcall TChave::PM_GetTipoSeccionadora(void)
   {
   return(PD.tipochave->Tipo == chaveSECCIONADORA);
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetBloqueada(bool bloqueada)
   {
   if (bloqueada)
      {PD.estado |= chvBLOQUEADA;}
   else
      {PD.estado &= ~chvBLOQUEADA;}
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetCapDisruptiva_amp(double i_amp)
   {
   PD.cap_disruptiva_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetEloFusivel(AnsiString elo)
   {
   PD.elo_fusivel = elo;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetEstado(unsigned estado)
   {
   PD.estado = estado;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetEstadoNormal(unsigned estado)
   {
   PD.estado_normal = estado;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetInom_amp(double i_amp)
   {
   PD.corn_amp = i_amp;
   //atualiza capacidade disruptiva, se ela não estiver definida
   if (int(PD.cap_disruptiva_amp) == 0)
      {
      PD.cap_disruptiva_amp = 1.2 * i_amp;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetProtecaoAtiva(bool protecao_ativa)
   {
   PD.protecao_ativa = protecao_ativa;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetTecnoComunicacao(int tecno_comunicacao)
   {
   PD.tecno_comunicacao = tecno_comunicacao;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetTelecomandada(bool telecomandada)
   {
   PD.telecomandada = telecomandada;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetTipoChave(VTTipoChave *tipochave)
   {
   PD.tipochave = tipochave;
   }

//---------------------------------------------------------------------------
void __fastcall TChave::PM_SetTipoGIS(AnsiString tipo_gis)
   {
   PD.tipo_gis = tipo_gis;
   }

//---------------------------------------------------------------------------
int __fastcall TChave::Tipo(void)
   {
   return(eqptoCHAVE);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TChave::TipoAsString(void)
   {
   return("Chave");
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TChave::TipoDisjuntor(void)
   {
   if (tipo_chv.AnsiCompareIC("disjuntor") == 0) return(true);
   if (tipo_chv.AnsiCompareIC("disj") == 0)      return(true);
   return(tipo_chv.AnsiCompareIC("dj") == 0);
   }
*/
//---------------------------------------------------------------------------
//eof
