//---------------------------------------------------------------------------
#ifndef VTProtocoloH
#define VTProtocoloH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//PORT 
#define SERVER_PORT 1024

//---------------------------------------------------------------------------
//tipo de cálculo
enum proTIPO_CALCULO {
                     calculoFLUXO = 0,
                     calculoCURTO = 1
                     };

//---------------------------------------------------------------------------
//tipo de equipamento
enum proFUNCAO {
               funcaoINDEFINIDA         =  -1,
               funcaoDIA_SEMANA         =   0,
               funcaoSOLICITA_REGISTRO  =   1,
               funcaoENVIO_REGISTRO     =   2,
               funcaoMANOBRA_CHAVE      =   3,
               funcaoSIMULA_FALTA       =   4,
               funcaoELIMINA_FALTA      =   5,
					funcaoALTERA_TRAFO       =   6,
					funcaoALTERA_CARGA       =   7,
					funcaoALTERA_SUPRIMENTO  =   8,
					funcaoALTERA_GERADOR     =   9,
					funcaoALTERA_CAPACITOR   =  10,
					funcaoEXISTE_EQPTO       =  99,
					funcaoACK                = 100,
					funcaoNACK               = 101,
					funcaoSUPERVISAO_AUTO    = 102
               };

//---------------------------------------------------------------------------
//tipo de equipamento
enum proTIPO_EQPTO   {
                     teINDEFINIDO     = -1,
                     teBARRA          =  1,
                     teTRECHO         =  2,
                     teCHAVE          =  3,
                     teTRANSFORMADOR  =  4,
                     teREGULADOR      =  5,
                     teCAPACITORSERIE =  6,
                     teCAPACITOR      =  7,
                     teGERADOR        =  8,
                     teSUPRIMENTO     =  9,
                     teCARGA          = 10,
                     teFILTRO         = 11,
                     teREATOR         = 12
                     };
                     
//---------------------------------------------------------------------------
//tipo de equipamento
enum proTIPO_FALTA   {
                     falta3F=0,
                     falta2F_AB,
                     falta1FT_AT,
                     falta1FTz_AT,
                     falta2FT_BCT
                     };

//---------------------------------------------------------------------------
//estuturas das mensagens
struct strBUF
      {
      char *memory;
      int  length;
      };

struct strHMS
      {
      char hora;
      char minuto;
      char segundo;
      };
//---------------------------------------------------------------------------
//dimensão máxima da mensagem
#define MAX_MSG_LEN 10240

//---------------------------------------------------------------------------
class VTBarra;
class VTCapacitor;
class VTChave;
class VTBarCC;
class VTLigCC;
class VTTrafo;
class VTTrecho;
class VTMsg;

//---------------------------------------------------------------------------
class VTProtocolo : public TObject
   {
   public:
                     __fastcall  VTProtocolo(void) {};
                     __fastcall ~VTProtocolo(void) {};
		virtual bool   __fastcall  ConverteTipoEqptoProtocoloParaSinap(int tipo_proto, int &tipo_sinap) = 0;
		virtual bool   __fastcall  ConverteTipoEqptoSinapParaProtocolo(int tipo_sinap, int &tipo_proto) = 0;
		virtual VTMsg* __fastcall  MontaMsgAck(int funcao_acked, bool sucesso) = 0;
		virtual VTMsg* __fastcall  MontaMsgAlteraCapacitor(int eqpto_tipo, int eqpto_id, strHMS *hms, bool ligado) = 0;
		virtual VTMsg* __fastcall  MontaMsgAlteraTrafo(int eqpto_tipo, int eqpto_id, strHMS *hms, int tap) = 0;
		virtual VTMsg* __fastcall  MontaMsgEliminaFalta(strHMS *hms) = 0;
      virtual VTMsg* __fastcall  MontaMsgExisteEqpto(int eqpto_tipo, int eqpto_id, AnsiString codigo, strHMS *hms) = 0;
      virtual VTMsg* __fastcall  MontaMsgManobraChave(int eqpto_tipo, int eqpto_id, strHMS *hms, bool abrir) = 0;
      virtual VTMsg* __fastcall  MontaMsgNAck(int funcao_nacked) = 0;
      virtual int    __fastcall  MontaMsgRecebida(strBUF *buf, TList *lisMSG) = 0;
      virtual VTMsg* __fastcall  MontaMsgRegistroBarraCurto(VTBarCC *barCC, strHMS *hms, int ind_def) = 0;
      virtual VTMsg* __fastcall  MontaMsgRegistroBarraFluxo(VTBarra *barra, strHMS *hms, int ind_pat) = 0;
      virtual VTMsg* __fastcall  MontaMsgRegistroCapacitor(VTCapacitor *capacitor, strHMS *hms, int ind_pat) = 0;
      virtual VTMsg* __fastcall  MontaMsgRegistroChaveCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def) = 0;
		virtual VTMsg* __fastcall  MontaMsgRegistroChaveFluxo(VTChave *chave, strHMS *hms, int ind_pat) = 0;
		virtual VTMsg* __fastcall  MontaMsgRegistroTrafoCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def) = 0;
		virtual VTMsg* __fastcall  MontaMsgRegistroTrafoFluxo(VTTrafo *trafo, strHMS *hms, int ind_pat) = 0;
      virtual VTMsg* __fastcall  MontaMsgRegistroTrechoCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def) = 0;
		virtual VTMsg* __fastcall  MontaMsgRegistroTrechoFluxo(VTTrecho *trecho, strHMS *hms, int ind_pat) = 0;
		virtual VTMsg* __fastcall  MontaMsgSimulaFalta(int eqpto_tipo, int eqpto_id, int tipo_falha,
                                                     double rdef_ohm, double dist_bar1_pu, bool pre_falta, strHMS *hms) = 0;
		virtual VTMsg* __fastcall  MontaMsgSolicitaRegistro(int eqpto_tipo, int eqpto_id, strHMS *hms) = 0;
		virtual VTMsg* __fastcall  MontaMsgSupervisaoAutomatica(strHMS *hms, bool enabled) = 0;
	};

//---------------------------------------------------------------------------
VTProtocolo* __fastcall NewObjProtocolo(void);

//---------------------------------------------------------------------------
#endif
//eof

