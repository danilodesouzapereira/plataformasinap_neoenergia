//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TSinapUser.h"
#include "..\Protocolo\VTMsg.h"
#include "..\Protocolo\TMsgExisteEqpto.h"
#include "..\Protocolo\TMsgRegistroChave.h"
#include "..\Protocolo\VTProtocolo.h"

//-----------------------------------------------------------------------------
VTSinapUser* __fastcall NewObjSinapUser(VTApl *apl_owner)
   {
   try{
		return(new TSinapUser(apl_owner));
      }catch(Exception &e)
         {//nada a fazer
			}
   //erro na criação do objeto
	return(NULL);
   }

//-----------------------------------------------------------------------------
__fastcall TSinapUser::TSinapUser(VTApl *apl_owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   }

//-----------------------------------------------------------------------------
__fastcall TSinapUser::~TSinapUser(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TSinapUser::HabilitaLog(TStrings *lines_rx, TStrings *lines_tx, TStatusPanel *nome_patamar)
   {
	//salva ponteiros para objetos
	log.nome_patamar = nome_patamar;
	log.lines_rx     = lines_rx;
	log.lines_tx     = lines_tx;
   }

//---------------------------------------------------------------------------
void __fastcall TSinapUser::LogPatamar(AnsiString txt)
	{
	if (log.nome_patamar)
		{
		log.nome_patamar->Text = "Patamar supervisão automática: " + txt;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::LogRx(AnsiString txt)
	{
	if (log.lines_rx)
		{//verifica limite de mensagens
      if (log.lines_rx->Count > LOG_COUNT_MAX)
         {//remove linha mais antiga
         log.lines_rx->Delete(log.lines_rx->Count - 1);
         }
      //insere a nova linha
      log.lines_rx->Insert(0, txt);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSinapUser::LogTx(AnsiString txt)
	{
   if (log.lines_tx)
      {//verifica limite de mensagens
		if (log.lines_tx->Count > LOG_COUNT_MAX)
         {//remove linha mais antiga
         log.lines_tx->Delete(log.lines_tx->Count - 1);
         }
      //insere a nova linha
      log.lines_tx->Insert(0, txt);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapCapacitorEstado(AnsiString codigo, int estado)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapCapacitorQmvar(AnsiString codigo, double q_mvar)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveCBR(AnsiString codigo, int cbtl)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveCBTL(AnsiString codigo, int cbtl)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveCorrente(AnsiString codigo, int fase, double i_amp)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveEstado(AnsiString codigo, int estado)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveLockout(AnsiString codigo, int lockout)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveS1f_mva(AnsiString codigo, int fase, double p_mw, double q_mvar)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveS3f_mva(AnsiString codigo, double p_mw, double q_mvar)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapChaveTensao(AnsiString codigo, int fase, double vfn_kv)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapUser::TrataMsg(AnsiString eqpto_codigo, VTMsg *msg)
	{
	//este método não é implementado nesta classe derivada
   return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapTrafoCorrente(AnsiString codigo, int enrol, int fase, double i_amp)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapTrafoTap(AnsiString codigo, int tap)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
void __fastcall TSinapUser::SinapTrafoTensao(AnsiString codigo, int enrol, int fase, double vfn_kv)
	{
	//este método não é implementado nesta classe derivada
	}

//---------------------------------------------------------------------------
//eof
