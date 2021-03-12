//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBateriaC.h"
#include ".\VTGeralC.h"
#include "..\VTData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Função para criar objeto da classe TBateriaC
//---------------------------------------------------------------------------
TBateriaC * __fastcall NewObjBateriaC(TBarra *barra)
   {
   return(new TBateriaC(barra));
   }

//---------------------------------------------------------------------------
__fastcall TBateriaC::TBateriaC(TBarra *barra)
{
// Salva dados fornecidos
this->barra = barra;

// Inicialização
// Dados nominais
eqpto            = NULL;
codigo           = AnsiString("");
id               = -1;
fases            = -1;
ligacao          = -1;
tipo_operacao    = topAUTO;
modelo_carga     = Icte;  // Default
modelo_desc      = Icte;  // Default
pnom_kw          = 0.;
enom_kwh         = 0.;
scap_inv_kva     = 0.;
en_armaz_inic_pu = 0.;
pext_carga_kw    = 0.;
qext_carga_kvar  = 0.;
pext_desc_kw     = 0.;
qext_desc_kvar   = 0.;
p_ociosa_pu      = 0.;
q_ociosa_pu      = 0.;
rend_carga_pu    = 0.99;
rend_desc_pu     = 0.99;
gatilho_carga_pu = 0.01;
gatilho_desc_pu  = 1.;
en_armaz_max_kwh = 0.;
en_armaz_min_kwh = 0.;

// Dados da operação
inverte_modo  = false;
estado        = estDESCARREGADA;
modo_operacao = mopCARGA;
pext_kw       = 0.;
qext_kvar     = 0.;
pcarga_kw     = 0.;
pdesc_kw      = 0.;
en_armaz_kwh  = en_armaz_inic_pu * enom_kwh; // Condição inicial da bateria
}

//---------------------------------------------------------------------------
__fastcall TBateriaC::~TBateriaC(void)
{
}

//---------------------------------------------------------------------------
bool __fastcall TBateriaC::Atualiza(int    ind_pat,
                                    double duracao_h,
                                    VTData *data)
{
if(tipo_operacao == topMANUAL) return(AtualizaTipoMANUAL(ind_pat, duracao_h, data));
else                           return(AtualizaTipoAUTO(ind_pat, duracao_h, data));
}

//---------------------------------------------------------------------------
void __fastcall TBateriaC::AtualizaDados(smcBATERIA *bateria)
{
eqpto            = bateria->eqpto;
codigo           = bateria->codigo;
id               = bateria->id;
tipo_operacao    = bateria->tipo_operacao;
modelo_carga     = bateria->modelo_carga;
modelo_desc      = bateria->modelo_desc;
pnom_kw          = bateria->pnom_kw;
enom_kwh         = bateria->enom_kwh;
scap_inv_kva     = bateria->scap_inv_kva;
en_armaz_inic_pu = bateria->en_armaz_inic_pu;
pext_carga_kw    = bateria->pext_carga_kw;
qext_carga_kvar  = bateria->qext_carga_kvar;
pext_desc_kw     = bateria->pext_desc_kw;
qext_desc_kvar   = bateria->qext_desc_kvar;
p_ociosa_pu      = bateria->p_ociosa_pu;
q_ociosa_pu      = bateria->q_ociosa_pu;
rend_carga_pu    = bateria->rend_carga_pu;
rend_desc_pu     = bateria->rend_desc_pu;
gatilho_carga_pu = bateria->gatilho_carga_pu;
gatilho_desc_pu  = bateria->gatilho_desc_pu;

// Calcula parâmetros derivados
en_armaz_max_kwh = gatilho_desc_pu  * enom_kwh;
en_armaz_min_kwh = gatilho_carga_pu * enom_kwh;

// Limita potências externas de carga e descarga de acordo com
// a capacidade do inversor
VerificaLimitePotenciasExternas(&pext_carga_kw, &qext_carga_kvar);
VerificaLimitePotenciasExternas(&pext_desc_kw,  &qext_desc_kvar);
}

//---------------------------------------------------------------------------
bool __fastcall TBateriaC::AtualizaTipoAUTO(int    ind_pat,
                                            double duracao_h,
                                            VTData *data)
{
// Variáveis locais
double            en_final_kwh, p_ociosa_kw;
smcESTADO_BATERIA estado_bateria;

// Obtém estado inicial da bateria (primeiro patamar)
// IMPORTANTE: a função 'TData::EstadoBateria()' já devolve um estado
//             coerente; não é preciso fazer verificações aqui
if(ind_pat == 0)
   {
   estado_bateria.eqpto   = eqpto;
   estado_bateria.ind_pat = ind_pat;
   if(! data->EstadoBateria(&estado_bateria)) return(false);
   // Salva valores obtidos
   modo_operacao = estado_bateria.modo_operacao;
   en_armaz_kwh  = estado_bateria.en_armaz_kwh;
   }

// Obtém o estado de carga da bateria (estCARREGADA, estINTERMEDIARIO ou
// estDESCARREGADA)
estado = estado_bateria.estado;

// Verifica se no patamar anterior foi sinalizada inversão do modo de operação
if(inverte_modo)
   {
   if     (modo_operacao == mopCARGA)    modo_operacao = mopDESCARGA;
   else if(modo_operacao == mopDESCARGA) modo_operacao = mopCARGA;
   inverte_modo = false;  // Desliga flag
   }

// Obtém potências externas (ativa e reativa) e calcula energia armazenada
// na bateria no fim do patamar atual, verificando limite mínimo (DESCARGA)
// ou máximo (CARGA). Caso haja violação do limite, a energia armazenada é
// feita igual ao limite excedido e sinaliza-se a inversão do modo de
// operação para o próximo patamar.
p_ociosa_kw = p_ociosa_pu * pnom_kw;
switch(modo_operacao)
   {
   case mopCARGA:
      pext_kw      = pext_carga_kw;
      qext_kvar    = qext_carga_kvar;
      pcarga_kw    = (pext_kw - p_ociosa_kw) * rend_carga_pu;
      en_final_kwh = en_armaz_kwh + (pcarga_kw * duracao_h);
      if(en_final_kwh > en_armaz_max_kwh) // A bateria estará totalmente carregada
         {
         en_final_kwh = en_armaz_max_kwh;
         estado       = estCARREGADA;
         inverte_modo = true;
         }
      en_armaz_kwh = en_final_kwh;  // Atualiza energia armazenada
      break;

   case mopDESCARGA:
      pext_kw      = pext_desc_kw;
      qext_kvar    = qext_desc_kvar;
      pdesc_kw     = (pext_kw + p_ociosa_kw) / rend_desc_pu;
      en_final_kwh = en_armaz_kwh - (pdesc_kw * duracao_h);
      if(en_final_kwh < en_armaz_min_kwh) // A bateria estará totalmente descarregada
         {
         en_final_kwh = en_armaz_min_kwh;
         estado       = estDESCARREGADA;
         inverte_modo = true;
         }
      en_armaz_kwh = en_final_kwh;  // Atualiza energia armazenada
      break;

   default:  // mopOCIOSA
      pext_kw   = p_ociosa_kw;
      qext_kvar = q_ociosa_pu * pnom_kw;
      break;
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TBateriaC::AtualizaTipoMANUAL(int    ind_pat,
                                              double duracao_h,
                                              VTData *data)
{
// Variáveis locais
double            en_final_kwh, p_ociosa_kw;
smcESTADO_BATERIA estado_bateria;

// Obtém estado da bateria no patamar atual
// IMPORTANTE: a função 'TData::EstadoBateria()' já devolve um estado
//             coerente; não é preciso fazer verificações aqui
estado_bateria.eqpto   = eqpto;
estado_bateria.ind_pat = ind_pat;
if(! data->EstadoBateria(&estado_bateria)) return(false);
// Salva valores obtidos
estado        = estado_bateria.estado;
modo_operacao = estado_bateria.modo_operacao;
en_armaz_kwh  = estado_bateria.en_armaz_kwh;
pext_kw       = estado_bateria.pext_kw;
qext_kvar     = estado_bateria.qext_kvar;

// Limita potências externas de acordo com a capacidade do inversor
VerificaLimitePotenciasExternas(&pext_kw, &qext_kvar);

// Calcula energia armazenada na bateria no fim do patamar atual,
// verificando limite mínimo (DESCARGA) ou máximo (CARGA). Caso haja
// violação do limite, a energia armazenada é feita igual ao limite
// excedido.
p_ociosa_kw = p_ociosa_pu * pnom_kw;
switch(modo_operacao)
   {
   case mopCARGA:
      if(pext_kw < p_ociosa_kw) pext_kw = p_ociosa_kw; // Proteção
      pcarga_kw    = (pext_kw - p_ociosa_kw) * rend_carga_pu;
      en_final_kwh = en_armaz_kwh + (pcarga_kw * duracao_h);
      if(en_final_kwh > en_armaz_max_kwh) // A bateria estará totalmente carregada
         {
         en_final_kwh = en_armaz_max_kwh;
         estado       = estCARREGADA;
         }
      en_armaz_kwh = en_final_kwh;        // Atualiza energia armazenada
      break;

   case mopDESCARGA:
      pdesc_kw     = (pext_kw + p_ociosa_kw) / rend_desc_pu;
      en_final_kwh = en_armaz_kwh - (pdesc_kw * duracao_h);
      if(en_final_kwh < en_armaz_min_kwh) // A bateria estará totalmente descarregada
         {
         en_final_kwh = en_armaz_min_kwh;
         estado       = estDESCARREGADA;
         }
      en_armaz_kwh = en_final_kwh;        // Atualiza energia armazenada
      break;

   default:  // mopOCIOSA: apenas redefine as potências externas
      pext_kw   = p_ociosa_kw;
      qext_kvar = q_ociosa_pu * pnom_kw;
      break;
   }

// Retorna
return(true);
}

/*
//---------------------------------------------------------------------------
void __fastcall TBateriaC::DefineEstadoManual(int ind_pat)
{
// Função para definir operação manual idêntica à operação automática
switch(ind_pat)
   {
   case 0:
   case 1:
   case 2:
   case 6:
   case 7:
      modo_operacao = 0;
      pext_kw       = 20.;
      break;

   case 3:
   case 4:
   case 5:
      modo_operacao = 1;
      pext_kw       = 15.;
      break;

   case 8:
   case 14:
   case 15:
   case 16:
   case 17:
   case 18:
      modo_operacao = 0;
      pext_kw       = 10.;
      break;

   case 9:
   case 10:
   case 11:
   case 12:
   case 13:
   case 19:
   case 20:
   case 21:
   case 22:
   case 23:
      modo_operacao = 1;
      pext_kw       = 7.5;
      break;
   }
}
*/

//---------------------------------------------------------------------------
void __fastcall TBateriaC::VerificaLimitePotenciasExternas(double *px_kw,
                                                           double *qx_kvar)
{
// Variáveis locais
double fmult, sx_kva;

// Proteção
if(scap_inv_kva == 0.) return;  // Erro de dados (nada a fazer)

// Garante que a capacidade do inversor não é excedida
sx_kva = sqrt(((*px_kw) * (*px_kw)) + ((*qx_kvar) * (*qx_kvar)));
if(sx_kva > 0.)
   {
   fmult = scap_inv_kva / sx_kva;
   if(fmult < 1.)  // Capacidade do inversor excedida; reduz P e Q
      {
      *(px_kw)   *= fmult;
      *(qx_kvar) *= fmult;
      }
   }
}

//---------------------------------------------------------------------------


