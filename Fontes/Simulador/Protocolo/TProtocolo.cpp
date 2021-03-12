//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <PlataformaSinap\Fontes\Complexo\Complexo.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCapacitor.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTResFlowBar.h>
#include <PlataformaSinap\Fontes\Rede\VTResFlowLig.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\RedeCC\VTBarCC.h>
#include <PlataformaSinap\Fontes\RedeCC\VTLigCC.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TProtocolo.h"
#include "VTMsg.h"
#include "TMsgAck.h"
#include "TMsgAlteraCapacitor.h"
#include "TMsgAlteraTrafo.h"
#include "TMsgEliminaFalta.h"
#include "TMsgExisteEqpto.h"
#include "TMsgManobraChave.h"
#include "TMsgNAck.h"
#include "TMsgRegistroBarra.h"
#include "TMsgRegistroCapacitor.h"
#include "TMsgRegistroChave.h"
#include "TMsgRegistroTrafo.h"
#include "TMsgRegistroTrecho.h"
#include "TMsgSimulaFalta.h"
#include "TMsgSolicitaRegistro.h"
#include "TMsgSupervisaoAuto.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTProtocolo* __fastcall NewObjProtocolo(void)
   {
   try{//cria objeto TProtocolo
      return(new TProtocolo());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TProtocolo::TProtocolo(void)
   {
   //cria lista de Msg
   //lisMSG = new TList();
   }

 //---------------------------------------------------------------------------
__fastcall  TProtocolo::~TProtocolo(void)
   {
   //destrói lista e seus objetos
   //if (lisMSG) {LimpaTList(lisMSG); delete lisMSG; lisMSG = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TProtocolo::ConverteTipoEqptoProtocoloParaSinap(int tipo_proto, int &tipo_sinap)
	{
	switch(tipo_proto)
		{
		case teBARRA         :  tipo_sinap = eqptoBARRA     ; break;
		case teTRECHO        :  tipo_sinap = eqptoTRECHO    ; break;
		case teCHAVE         :  tipo_sinap = eqptoCHAVE     ; break;
		case teTRANSFORMADOR :  tipo_sinap = eqptoTRAFO     ; break;
		case teREGULADOR     :  tipo_sinap = eqptoREGULADOR ; break;
		case teCAPACITORSERIE:  tipo_sinap = eqptoCAPSERIE  ; break;
		case teCAPACITOR     :  tipo_sinap = eqptoCAPACITOR ; break;
		case teGERADOR       :  tipo_sinap = eqptoGERADOR   ; break;
		case teSUPRIMENTO    :  tipo_sinap = eqptoSUPRIMENTO; break;
		case teCARGA         :  tipo_sinap = eqptoCARGA     ; break;
		case teFILTRO        :  tipo_sinap = eqptoFILTRO    ; break;
		case teREATOR        :  tipo_sinap = eqptoREATOR    ; break;
		default:  return(false);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TProtocolo::ConverteTipoEqptoSinapParaProtocolo(int tipo_sinap, int &tipo_proto)
	{
	switch(tipo_sinap)
		{
		case eqptoBARRA     :  tipo_proto = teBARRA         ; break;
		case eqptoTRECHO    :  tipo_proto = teTRECHO        ; break;
		case eqptoCHAVE     :  tipo_proto = teCHAVE         ; break;
		case eqptoTRAFO     :  tipo_proto = teTRANSFORMADOR ; break;
		case eqptoREGULADOR :  tipo_proto = teREGULADOR     ; break;
		case eqptoCAPSERIE  :  tipo_proto = teCAPACITORSERIE; break;
		case eqptoCAPACITOR :  tipo_proto = teCAPACITOR     ; break;
		case eqptoGERADOR   :  tipo_proto = teGERADOR       ; break;
		case eqptoSUPRIMENTO:  tipo_proto = teSUPRIMENTO    ; break;
		case eqptoCARGA     :  tipo_proto = teCARGA         ; break;
		case eqptoFILTRO    :  tipo_proto = teFILTRO        ; break;
		case eqptoREATOR    :  tipo_proto = teREATOR        ; break;
		default:  return(false);
		}
	return(true);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgAck(int funcao_acked, bool sucesso)
   {
   //variáveis locais
   TMsgAck *msg;

   //cria Msg
   if ((msg = NewMsgAck()) != NULL)
      {//complementa dados da Msg
      msg->pcte.funcao_acked = funcao_acked;
      msg->pcte.sucesso      = sucesso;
      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgAlteraCapacitor(int eqpto_tipo, int eqpto_id, strHMS *hms, bool ligado)
	{
	//variáveis locais
	TMsgAlteraCapacitor *msg;

	//cria Msg
	if ((msg = NewMsgAlteraCapacitor()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora         = hms->hora;
		msg->pcte.minuto       = hms->minuto;
		msg->pcte.segundo      = hms->segundo;
		msg->pcte.eqpto_tipo   = eqpto_tipo;
		msg->pcte.eqpto_id     = eqpto_id;
		msg->pcte.ligado       = (ligado) ? 1 : 0;
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgAlteraTrafo(int eqpto_tipo, int eqpto_id, strHMS *hms, int variacao_tap)
	{
	//variáveis locais
	TMsgAlteraTrafo *msg;

	//cria Msg
	if ((msg = NewMsgAlteraTrafo()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora         = hms->hora;
		msg->pcte.minuto       = hms->minuto;
		msg->pcte.segundo      = hms->segundo;
		msg->pcte.eqpto_tipo   = eqpto_tipo;
		msg->pcte.eqpto_id     = eqpto_id;
		msg->pcte.variacao_tap = variacao_tap;
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgEliminaFalta(strHMS *hms)
	{
	//variáveis locais
   TMsgEliminaFalta *msg;

   //cria Msg
   if ((msg = NewMsgEliminaFalta()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora       = hms->hora;
      msg->pcte.minuto     = hms->minuto;
      msg->pcte.segundo    = hms->segundo;
      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
   }

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgExisteEqpto(int eqpto_tipo, int eqpto_id, AnsiString codigo, strHMS *hms)
   {
   //variáveis locais
   TMsgExisteEqpto *msg;

   //cria Msg
   if ((msg = NewMsgExisteEqpto()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora       = hms->hora;
      msg->pcte.minuto     = hms->minuto;
      msg->pcte.segundo    = hms->segundo;
      msg->pcte.eqpto_tipo = eqpto_tipo;
      msg->pcte.eqpto_id   = eqpto_id;
      strncpy(msg->pcte.codigo, codigo.c_str(), sizeof(msg->pcte.codigo));
      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
   }

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgManobraChave(int eqpto_tipo, int eqpto_id, strHMS *hms, bool abrir)
	{
	//variáveis locais
	TMsgManobraChave *msg;

	//cria Msg
	if ((msg = NewMsgManobraChave()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora       = hms->hora;
		msg->pcte.minuto     = hms->minuto;
		msg->pcte.segundo    = hms->segundo;
		msg->pcte.eqpto_tipo = eqpto_tipo;
		msg->pcte.eqpto_id   = eqpto_id;
		msg->pcte.estado     = (abrir) ? 0 : 1;
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgNAck(int funcao_nacked)
   {
   //variáveis locais
   TMsgNAck *msg;

   //cria Msg
   if ((msg = NewMsgNAck()) != NULL)
      {//complementa dados da Msg
      msg->pcte.funcao_nacked = funcao_nacked;
      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
   }

//---------------------------------------------------------------------------
int __fastcall TProtocolo::MontaMsgRecebida(strBUF *buf, TList *lisMSG)
   {
   //variáveis locais
   VTMsg  *msg;
   int    funcao;
   int    index  = 0;

   do {//verifica função solicitada
      funcao = buf->memory[index];
      switch(funcao)
         {
         case funcaoACK :
            msg = NewMsgAck();
            msg->ConverteBufToMsg(buf, index);
            break;
			case funcaoALTERA_CAPACITOR:
				msg = NewMsgAlteraCapacitor();
				msg->ConverteBufToMsg(buf, index);
				break;
			case funcaoALTERA_CARGA:      msg = NULL; break;
			case funcaoALTERA_GERADOR:    msg = NULL; break;
			case funcaoALTERA_SUPRIMENTO: msg = NULL; break;
			case funcaoALTERA_TRAFO:
				msg = NewMsgAlteraTrafo();
				msg->ConverteBufToMsg(buf, index);
				break;
			case funcaoELIMINA_FALTA:
				msg = NewMsgEliminaFalta();
				msg->ConverteBufToMsg(buf, index);
				break;
         case funcaoENVIO_REGISTRO:
            //identifica tipo do Eqpto
            switch(buf->memory[index+7])
               {
               case teBARRA: //cria Msg e inicia seus dados a partir do Buf recebido
                  msg = NewMsgRegistroBarra();
                  msg->ConverteBufToMsg(buf, index);
                  break;
               case teCAPACITOR: //cria Msg e inicia seus dados a partir do Buf recebido
                  msg = NewMsgRegistroCapacitor();
                  msg->ConverteBufToMsg(buf, index);
                  break;
               case teCHAVE: //cria Msg e inicia seus dados a partir do Buf recebido
                  msg = NewMsgRegistroChave();
                  msg->ConverteBufToMsg(buf, index);
                  break;
					case teTRANSFORMADOR: //cria Msg e inicia seus dados a partir do Buf recebido
						msg = NewMsgRegistroTrafo();
                  msg->ConverteBufToMsg(buf, index);
                  break;
					case teTRECHO: //cria Msg e inicia seus dados a partir do Buf recebido
						msg = NewMsgRegistroTrecho();
                  msg->ConverteBufToMsg(buf, index);
                  break;
					default:
                  msg = NULL;
                  break;
               }
            break;
         case funcaoMANOBRA_CHAVE:
            msg = NewMsgManobraChave();
            msg->ConverteBufToMsg(buf, index);
            break;
         case funcaoNACK:
            msg = NewMsgNAck();
            msg->ConverteBufToMsg(buf, index);
            break;
         case funcaoSIMULA_FALTA:
            msg = NewMsgSimulaFalta();
            msg->ConverteBufToMsg(buf, index);
            break;
         case funcaoSOLICITA_REGISTRO: //cria Msg e inicia seus dados a partir do Buf recebido
            msg = NewMsgSolicitaRegistro();
            msg->ConverteBufToMsg(buf, index);
            break;
         case funcaoEXISTE_EQPTO:
            msg = NewMsgExisteEqpto();
            msg->ConverteBufToMsg(buf, index);
            break;
			case funcaoSUPERVISAO_AUTO:
            msg = NewMsgSupervisaoAuto();
            msg->ConverteBufToMsg(buf, index);
            break;
			default:
            msg   = NULL;
            break;
         }
      //verifica se foi montada uma Msg
      if (msg != NULL)
         {//salva Msg na lista
         lisMSG->Add(msg);
         //atualiza index para apontar próxima Msg no Buffer
         index += msg->NumBytes;
         }
      } while((msg != NULL)&&(index < buf->length));
   return(lisMSG->Count);
   }

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroBarraCurto(VTBarCC *barCC, strHMS *hms, int ind_def)
   {
   //variáveis locais
   complex<double>   v_kv;
   TMsgRegistroBarra *msg;

   //cria Msg
   if ((msg = NewMsgRegistroBarra()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora        = hms->hora;
      msg->pcte.minuto      = hms->minuto;
      msg->pcte.segundo     = hms->segundo;
      msg->pcte.fluxo_curto = calculoCURTO;
      msg->pcte.eqpto_tipo  = teBARRA;
      msg->pcte.eqpto_id    = barCC->Barra->Id;
      //tensões de fase
      for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
         {
         v_kv                            = barCC->Vfas_kv[ind_def][ind_fase];
         msg->pcte.vf[ind_fase].mod_volt = Abs(v_kv) * 1000.;
         msg->pcte.vf[ind_fase].ang_grau = RadToDeg(Arg(v_kv));
         }
      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
   }

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroBarraFluxo(VTBarra *barra, strHMS *hms, int ind_pat)
   {
   //variáveis locais
   complex<double>   v_kv;
   TMsgRegistroBarra *msg;

   //cria Msg
   if ((msg = NewMsgRegistroBarra()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora        = hms->hora;
      msg->pcte.minuto      = hms->minuto;
      msg->pcte.segundo     = hms->segundo;
      msg->pcte.fluxo_curto = calculoFLUXO;
      msg->pcte.eqpto_tipo = teBARRA;
      msg->pcte.eqpto_id   = barra->Id;
      //tensão fase A
      v_kv                             = barra->resflow->Van_kv[ind_pat];
      msg->pcte.vf[indFASE_A].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.vf[indFASE_A].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase B
      v_kv                             = barra->resflow->Vbn_kv[ind_pat];
      msg->pcte.vf[indFASE_B].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.vf[indFASE_B].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase C
      v_kv                             = barra->resflow->Vcn_kv[ind_pat];
      msg->pcte.vf[indFASE_C].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.vf[indFASE_C].ang_grau = RadToDeg(Arg(v_kv));
      //tensão neutro
      v_kv                             = barra->resflow->Vnt_kv[ind_pat];
      msg->pcte.vf[indNEUTRO].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.vf[indNEUTRO].ang_grau = RadToDeg(Arg(v_kv));

      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
   }

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroCapacitor(VTCapacitor *capacitor, strHMS *hms, int ind_pat)
	{
	//variáveis locais
	TMsgRegistroCapacitor *msg;

	//cria Msg
	if ((msg = NewMsgRegistroCapacitor()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora        = hms->hora;
		msg->pcte.minuto      = hms->minuto;
		msg->pcte.segundo     = hms->segundo;
		msg->pcte.fluxo_curto = calculoFLUXO;
		msg->pcte.eqpto_tipo  = teCAPACITOR;
		msg->pcte.eqpto_id    = capacitor->Id;
		msg->pcte.estado      = (capacitor->Status[sttENABLED]) ? 1 : 0;
		msg->pcte.q_mvar      = capacitor->q;
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroChaveCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def)
	{
	//variáveis locais
	int               ind_bar = 0;
	complex<double>   v_kv, i_amp, s_mva;
	TMsgRegistroChave *msg;
	VTChave           *chave  = (VTChave*)ligCC->Ligacao;

	//cria Msg
	if ((msg = NewMsgRegistroChave()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora        = hms->hora;
		msg->pcte.minuto      = hms->minuto;
		msg->pcte.segundo     = hms->segundo;
		msg->pcte.fluxo_curto = calculoCURTO;
		msg->pcte.eqpto_tipo  = teCHAVE;
		msg->pcte.eqpto_id    = chave->Id;
		msg->pcte.estado      = (chave->Fechada                 ) ? 1 : 0;
		msg->pcte.cbr         = (chave->Status[sttCHAVE_CBR]    ) ? 1 : 0;
		msg->pcte.cbtl        = (chave->Status[sttCHAVE_CBTL]   ) ? 1 : 0;
		msg->pcte.lockout     = (chave->Status[sttCHAVE_LOCKOUT]) ? 1 : 0;
		//índice da Barra para correntes e potências
		ind_bar = 0;
		//tensões de fase da Barra 1
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			v_kv                                  = bar1CC->Vfas_kv[ind_def][ind_fase];
			msg->pcte.bar1_vf[ind_fase].mod_volt = Abs(v_kv) * 1000;
			msg->pcte.bar1_vf[ind_fase].ang_grau = RadToDeg(Arg(v_kv));
			}
		//tensões de fase da Barra 2
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			v_kv                                  = bar2CC->Vfas_kv[ind_def][ind_fase];
			msg->pcte.bar2_vf[ind_fase].mod_volt = Abs(v_kv) * 1000;
			msg->pcte.bar2_vf[ind_fase].ang_grau = RadToDeg(Arg(v_kv));
			}
		//correntes de fase
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			i_amp                                = ligCC->Ifas_amp[ind_def][ind_bar][ind_fase];
			msg->pcte.chv_if[ind_fase].mod_amp  = Abs(i_amp);
			msg->pcte.chv_if[ind_fase].ang_grau = RadToDeg(Arg(i_amp));
         }
		//potência ativa e reativa em relação à barra 1
		s_mva   = ligCC->S_mva[ind_def][ind_bar];
		msg->pcte.p3f_mw        = s_mva.real();
		msg->pcte.q3f_mvar      = s_mva.imag();
		//potência ativa e reativa da fase A
		msg->pcte.pa_mw     = 0;
		msg->pcte.qa_mvar   = 0;
		//potência ativa e reativa da fase B
		msg->pcte.pb_mw     = 0;
		msg->pcte.qb_mvar   = 0;
		//potência ativa e reativa da fase C
		msg->pcte.pc_mw     = 0;
		msg->pcte.qc_mvar   = 0;
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroChaveFluxo(VTChave *chave, strHMS *hms, int ind_pat)
	{
	//variáveis locais
	int               ind_bar = 0;
	complex<double>   v_kv, i_amp;
	TMsgRegistroChave *msg;
	VTBarra           *barra;

   //cria Msg
   if ((msg = NewMsgRegistroChave()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora        = hms->hora;
      msg->pcte.minuto      = hms->minuto;
      msg->pcte.segundo     = hms->segundo;
      msg->pcte.fluxo_curto = calculoFLUXO;
		msg->pcte.eqpto_tipo  = teCHAVE;
      msg->pcte.eqpto_id    = chave->Id;
		msg->pcte.estado      = (chave->Fechada                 ) ? 1 : 0;
		msg->pcte.cbr         = (chave->Status[sttCHAVE_CBR]    ) ? 1 : 0;
		msg->pcte.cbtl        = (chave->Status[sttCHAVE_CBTL]   ) ? 1 : 0;
		msg->pcte.lockout     = (chave->Status[sttCHAVE_LOCKOUT]) ? 1 : 0;
		//índice da Barra para correntes e potências
		ind_bar = 0;
		//tensões da Barra 1
      barra = chave->pbarra1;
      //tensão fase A
      v_kv                                  = barra->resflow->Van_kv[ind_pat];
      msg->pcte.bar1_vf[indFASE_A].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar1_vf[indFASE_A].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase B
      v_kv                                  = barra->resflow->Vbn_kv[ind_pat];
      msg->pcte.bar1_vf[indFASE_B].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar1_vf[indFASE_B].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase C
      v_kv                                  = barra->resflow->Vcn_kv[ind_pat];
      msg->pcte.bar1_vf[indFASE_C].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar1_vf[indFASE_C].ang_grau = RadToDeg(Arg(v_kv));
      //tensão neutro
      v_kv                                  = barra->resflow->Vnt_kv[ind_pat];
      msg->pcte.bar1_vf[indNEUTRO].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar1_vf[indNEUTRO].ang_grau = RadToDeg(Arg(v_kv));
      //tensões da Barra 2
      barra = chave->pbarra2;
      //tensão fase A
      v_kv                                  = barra->resflow->Van_kv[ind_pat];
      msg->pcte.bar2_vf[indFASE_A].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar2_vf[indFASE_A].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase B
      v_kv                                  = barra->resflow->Vbn_kv[ind_pat];
      msg->pcte.bar2_vf[indFASE_B].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar2_vf[indFASE_B].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase C
      v_kv                                  = barra->resflow->Vcn_kv[ind_pat];
      msg->pcte.bar2_vf[indFASE_C].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar2_vf[indFASE_C].ang_grau = RadToDeg(Arg(v_kv));
      //tensão neutro
      v_kv                                  = barra->resflow->Vnt_kv[ind_pat];
      msg->pcte.bar2_vf[indNEUTRO].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar2_vf[indNEUTRO].ang_grau = RadToDeg(Arg(v_kv));
      //corrente fase A
		i_amp                                = chave->resflow->Ia_amp[ind_bar][ind_pat];
      msg->pcte.chv_if[indFASE_A].mod_amp  = Abs(i_amp);
      msg->pcte.chv_if[indFASE_A].ang_grau = RadToDeg(Arg(i_amp));
      //corrente fase B
      i_amp                                = chave->resflow->Ib_amp[ind_bar][ind_pat];
      msg->pcte.chv_if[indFASE_B].mod_amp  = Abs(i_amp);
      msg->pcte.chv_if[indFASE_B].ang_grau = RadToDeg(Arg(i_amp));
      //corrente fase C
      i_amp                                = chave->resflow->Ic_amp[ind_bar][ind_pat];
      msg->pcte.chv_if[indFASE_C].mod_amp  = Abs(i_amp);
      msg->pcte.chv_if[indFASE_C].ang_grau = RadToDeg(Arg(i_amp));
      //corrente neutro
      i_amp                                = chave->resflow->In_amp[ind_bar][ind_pat];
      msg->pcte.chv_if[indNEUTRO].mod_amp  = Abs(i_amp);
      msg->pcte.chv_if[indNEUTRO].ang_grau = RadToDeg(Arg(i_amp));
		//potência ativa e reativa trifásica
		msg->pcte.p3f_mw    = chave->resflow->P_mw[ind_bar][ind_pat];
		msg->pcte.q3f_mvar  = chave->resflow->Q_mvar[ind_bar][ind_pat];
		//potência ativa e reativa da fase A
		msg->pcte.pa_mw     = chave->resflow->Pa_mw[ind_bar][ind_pat];
		msg->pcte.qa_mvar   = chave->resflow->Qa_mvar[ind_bar][ind_pat];
		//potência ativa e reativa da fase B
		msg->pcte.pb_mw     = chave->resflow->Pb_mw[ind_bar][ind_pat];
		msg->pcte.qb_mvar   = chave->resflow->Qb_mvar[ind_bar][ind_pat];
		//potência ativa e reativa da fase C
		msg->pcte.pc_mw     = chave->resflow->Pc_mw[ind_bar][ind_pat];
		msg->pcte.qc_mvar   = chave->resflow->Qc_mvar[ind_bar][ind_pat];
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
      }
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroTrafoCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def)
	{
	//variáveis locais
	int               ind_bar = 0;
	complex<double>   v_kv, i_amp;
	TMsgRegistroTrafo *msg;
	VTTrafo           *trafo  = (VTTrafo*)ligCC->Ligacao;

	//cria Msg
	if ((msg = NewMsgRegistroTrafo()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora        = hms->hora;
		msg->pcte.minuto      = hms->minuto;
		msg->pcte.segundo     = hms->segundo;
		msg->pcte.fluxo_curto = calculoCURTO;
		msg->pcte.eqpto_tipo  = teTRANSFORMADOR;
		msg->pcte.eqpto_id    = trafo->Id;
		msg->pcte.tap         = trafo->Tap;
		//tensões de fase da Barra 1
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			v_kv                                 = bar1CC->Vfas_kv[ind_def][ind_fase];
			msg->pcte.bar1_vf[ind_fase].mod_volt = Abs(v_kv) * 1000;
			msg->pcte.bar1_vf[ind_fase].ang_grau = RadToDeg(Arg(v_kv));
			}
		//tensões de fase da Barra 2
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			v_kv                                  = bar2CC->Vfas_kv[ind_def][ind_fase];
			msg->pcte.bar2_vf[ind_fase].mod_volt = Abs(v_kv) * 1000;
			msg->pcte.bar2_vf[ind_fase].ang_grau = RadToDeg(Arg(v_kv));
			}
		//correntes de fase do primário
		ind_bar  = trafo->IndiceBarra(trafo->BarraPrimario());
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			i_amp                               = ligCC->Ifas_amp[ind_def][ind_bar][ind_fase];
			msg->pcte.pri_if[ind_fase].mod_amp  = Abs(i_amp);
			msg->pcte.pri_if[ind_fase].ang_grau = RadToDeg(Arg(i_amp));
			}
		//correntes de fase do secundário
		ind_bar = trafo->IndiceBarra(trafo->BarraSecundario());
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			i_amp                               = ligCC->Ifas_amp[ind_def][ind_bar][ind_fase];
			msg->pcte.pri_if[ind_fase].mod_amp  = Abs(i_amp);
			msg->pcte.pri_if[ind_fase].ang_grau = RadToDeg(Arg(i_amp));
			}
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroTrafoFluxo(VTTrafo *trafo, strHMS *hms, int ind_pat)
 	{
	//variáveis locais
	int               ind_bar = 0;
	complex<double>   v_kv, i_amp;
	TMsgRegistroTrafo *msg;
	VTBarra           *barra;

	//cria Msg
	if ((msg = NewMsgRegistroTrafo()) != NULL)
      {//complementa dados da Msg
		msg->pcte.hora        = hms->hora;
		msg->pcte.minuto      = hms->minuto;
		msg->pcte.segundo     = hms->segundo;
		msg->pcte.fluxo_curto = calculoFLUXO;
		msg->pcte.eqpto_tipo  = teTRANSFORMADOR;
		msg->pcte.eqpto_id    = trafo->Id;
		msg->pcte.tap        = trafo->Tap;
		//tensões da Barra 1
		barra = trafo->pbarra1;
		//tensão fase A
		v_kv                                  = barra->resflow->Van_kv[ind_pat];
		msg->pcte.bar1_vf[indFASE_A].mod_volt = Abs(v_kv) * 1000;
		msg->pcte.bar1_vf[indFASE_A].ang_grau = RadToDeg(Arg(v_kv));
		//tensão fase B
		v_kv                                  = barra->resflow->Vbn_kv[ind_pat];
		msg->pcte.bar1_vf[indFASE_B].mod_volt = Abs(v_kv) * 1000;
		msg->pcte.bar1_vf[indFASE_B].ang_grau = RadToDeg(Arg(v_kv));
		//tensão fase C
		v_kv                                  = barra->resflow->Vcn_kv[ind_pat];
		msg->pcte.bar1_vf[indFASE_C].mod_volt = Abs(v_kv) * 1000;
		msg->pcte.bar1_vf[indFASE_C].ang_grau = RadToDeg(Arg(v_kv));
		//tensão neutro
		v_kv                                  = barra->resflow->Vnt_kv[ind_pat];
      msg->pcte.bar1_vf[indNEUTRO].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar1_vf[indNEUTRO].ang_grau = RadToDeg(Arg(v_kv));
      //tensões da Barra 2
      barra = trafo->pbarra2;
		//tensão fase A
      v_kv                                  = barra->resflow->Van_kv[ind_pat];
      msg->pcte.bar2_vf[indFASE_A].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar2_vf[indFASE_A].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase B
      v_kv                                  = barra->resflow->Vbn_kv[ind_pat];
      msg->pcte.bar2_vf[indFASE_B].mod_volt = Abs(v_kv) * 1000;
		msg->pcte.bar2_vf[indFASE_B].ang_grau = RadToDeg(Arg(v_kv));
      //tensão fase C
      v_kv                                  = barra->resflow->Vcn_kv[ind_pat];
      msg->pcte.bar2_vf[indFASE_C].mod_volt = Abs(v_kv) * 1000;
      msg->pcte.bar2_vf[indFASE_C].ang_grau = RadToDeg(Arg(v_kv));
      //tensão neutro
      v_kv                                  = barra->resflow->Vnt_kv[ind_pat];
      msg->pcte.bar2_vf[indNEUTRO].mod_volt = Abs(v_kv) * 1000;
		msg->pcte.bar2_vf[indNEUTRO].ang_grau = RadToDeg(Arg(v_kv));
		//correntes do primário
		ind_bar  = trafo->IndiceBarra(trafo->BarraPrimario());
		//corrente fase A
		i_amp                                = trafo->resflow->Ia_amp[ind_bar][ind_pat];
		msg->pcte.pri_if[indFASE_A].mod_amp  = Abs(i_amp);
		msg->pcte.pri_if[indFASE_A].ang_grau = RadToDeg(Arg(i_amp));
		//corrente fase B
		i_amp                                = trafo->resflow->Ib_amp[ind_bar][ind_pat];
		msg->pcte.pri_if[indFASE_B].mod_amp  = Abs(i_amp);
		msg->pcte.pri_if[indFASE_B].ang_grau = RadToDeg(Arg(i_amp));
		//corrente fase C
		i_amp                                = trafo->resflow->Ic_amp[ind_bar][ind_pat];
		msg->pcte.pri_if[indFASE_C].mod_amp  = Abs(i_amp);
		msg->pcte.pri_if[indFASE_C].ang_grau = RadToDeg(Arg(i_amp));
		//corrente neutro
		i_amp                                = trafo->resflow->In_amp[ind_bar][ind_pat];
		msg->pcte.pri_if[indNEUTRO].mod_amp  = Abs(i_amp);
		msg->pcte.pri_if[indNEUTRO].ang_grau = RadToDeg(Arg(i_amp));
		//correntes do secundário
		ind_bar  = trafo->IndiceBarra(trafo->BarraSecundario());
		//corrente fase A
		i_amp                                = trafo->resflow->Ia_amp[ind_bar][ind_pat];
		msg->pcte.sec_if[indFASE_A].mod_amp  = Abs(i_amp);
		msg->pcte.sec_if[indFASE_A].ang_grau = RadToDeg(Arg(i_amp));
		//corrente fase B
		i_amp                                = trafo->resflow->Ib_amp[ind_bar][ind_pat];
		msg->pcte.sec_if[indFASE_B].mod_amp  = Abs(i_amp);
		msg->pcte.sec_if[indFASE_B].ang_grau = RadToDeg(Arg(i_amp));
		//corrente fase C
		i_amp                                = trafo->resflow->Ic_amp[ind_bar][ind_pat];
		msg->pcte.sec_if[indFASE_C].mod_amp  = Abs(i_amp);
		msg->pcte.sec_if[indFASE_C].ang_grau = RadToDeg(Arg(i_amp));
		//corrente neutro
		i_amp                                = trafo->resflow->In_amp[ind_bar][ind_pat];
		msg->pcte.sec_if[indNEUTRO].mod_amp  = Abs(i_amp);
		msg->pcte.sec_if[indNEUTRO].ang_grau = RadToDeg(Arg(i_amp));

      //salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroTrechoCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def)
	{
	//variáveis locais
	int                ind_bar = 0;
	complex<double>    v_kv, i_amp, s_mva;
	TMsgRegistroTrecho *msg;
	VTTrecho           *trecho  = (VTTrecho*)ligCC->Ligacao;

	//cria Msg
   if ((msg = NewMsgRegistroTrecho()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora        = hms->hora;
		msg->pcte.minuto      = hms->minuto;
		msg->pcte.segundo     = hms->segundo;
		msg->pcte.fluxo_curto = calculoCURTO;
		msg->pcte.eqpto_tipo  = teTRECHO;
		msg->pcte.eqpto_id    = trecho->Id;
		//índice da Barra para correntes e potências
		ind_bar = 0;
		//correntes de fase
		for (int ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
			{
			i_amp                               = ligCC->Ifas_amp[ind_def][ind_bar][ind_fase];
			msg->pcte.tre_if[ind_fase].mod_amp  = Abs(i_amp);
			msg->pcte.tre_if[ind_fase].ang_grau = RadToDeg(Arg(i_amp));
         }
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgRegistroTrechoFluxo(VTTrecho *trecho, strHMS *hms, int ind_pat)
	{
	//variáveis locais
	int                ind_bar = 0;
	complex<double>    v_kv, i_amp;
	TMsgRegistroTrecho *msg;
	VTBarra            *barra;

   //cria Msg
   if ((msg = NewMsgRegistroTrecho()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora        = hms->hora;
      msg->pcte.minuto      = hms->minuto;
      msg->pcte.segundo     = hms->segundo;
      msg->pcte.fluxo_curto = calculoFLUXO;
		msg->pcte.eqpto_tipo  = teTRECHO;
      msg->pcte.eqpto_id    = trecho->Id;
		//índice da Barra para correntes
		ind_bar = 0;
      //corrente fase A
		i_amp                                = trecho->resflow->Ia_amp[ind_bar][ind_pat];
      msg->pcte.tre_if[indFASE_A].mod_amp  = Abs(i_amp);
      msg->pcte.tre_if[indFASE_A].ang_grau = RadToDeg(Arg(i_amp));
      //corrente fase B
      i_amp                                = trecho->resflow->Ib_amp[ind_bar][ind_pat];
      msg->pcte.tre_if[indFASE_B].mod_amp  = Abs(i_amp);
      msg->pcte.tre_if[indFASE_B].ang_grau = RadToDeg(Arg(i_amp));
      //corrente fase C
      i_amp                                = trecho->resflow->Ic_amp[ind_bar][ind_pat];
      msg->pcte.tre_if[indFASE_C].mod_amp  = Abs(i_amp);
      msg->pcte.tre_if[indFASE_C].ang_grau = RadToDeg(Arg(i_amp));
      //corrente neutro
      i_amp                                = trecho->resflow->In_amp[ind_bar][ind_pat];
      msg->pcte.tre_if[indNEUTRO].mod_amp  = Abs(i_amp);
      msg->pcte.tre_if[indNEUTRO].ang_grau = RadToDeg(Arg(i_amp));
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
      }
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgSimulaFalta(int eqpto_tipo, int eqpto_id, int tipo_falha,
																  double rdef_ohm, double dist_bar1_pu, bool pre_falta, strHMS *hms)
   {
   //variáveis locais
   TMsgSimulaFalta *msg;

   //cria Msg
   if ((msg = NewMsgSimulaFalta()) != NULL)
      {//complementa dados da Msg
      msg->pcte.hora         = hms->hora;
      msg->pcte.minuto       = hms->minuto;
		msg->pcte.segundo      = hms->segundo;
      msg->pcte.eqpto_tipo   = eqpto_tipo;
      msg->pcte.eqpto_id     = eqpto_id;
      msg->pcte.tipo_falha   = tipo_falha;
      msg->pcte.dist_bar1_pu = dist_bar1_pu;
      msg->pcte.rdef_ohm     = rdef_ohm;
      msg->pcte.pre_falta    = (pre_falta) ? 1 : 0;
      //salva Msg em lisMSG
      //lisMSG->Add(msg);
      }
   return(msg);
   }

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgSolicitaRegistro(int eqpto_tipo, int eqpto_id, strHMS *hms)
	{
	//variáveis locais
	TMsgSolicitaRegistro *msg;

	//cria Msg
	if ((msg = NewMsgSolicitaRegistro()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora       = hms->hora;
		msg->pcte.minuto     = hms->minuto;
		msg->pcte.segundo    = hms->segundo;
		msg->pcte.eqpto_tipo = eqpto_tipo;
		msg->pcte.eqpto_id   = eqpto_id;
		//salva Msg em lisMSG
		//lisMSG->Add(msg);
		}
	return(msg);
	}

//---------------------------------------------------------------------------
VTMsg* __fastcall TProtocolo::MontaMsgSupervisaoAutomatica(strHMS *hms, bool enabled)
	{
	//variáveis locais
	TMsgSupervisaoAuto *msg;

	//cria Msg
	if ((msg = NewMsgSupervisaoAuto()) != NULL)
		{//complementa dados da Msg
		msg->pcte.hora       = hms->hora;
		msg->pcte.minuto     = hms->minuto;
		msg->pcte.segundo    = hms->segundo;
		msg->pcte.enabled    = (enabled) ? 1 : 0;
		}
	return(msg);
	}

//---------------------------------------------------------------------------
//eof
