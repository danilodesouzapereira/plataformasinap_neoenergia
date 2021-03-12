//---------------------------------------------------------------------------
#ifndef TProtocoloH
#define TProtocoloH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
# include <complex>
#include "VTProtocolo.h"

//---------------------------------------------------------------------------
class TProtocolo : public VTProtocolo
   {
   public:
             __fastcall  TProtocolo(void);
             __fastcall ~TProtocolo(void);
		bool   __fastcall  ConverteTipoEqptoProtocoloParaSinap(int tipo_proto, int &tipo_sinap);
		bool   __fastcall  ConverteTipoEqptoSinapParaProtocolo(int tipo_sinap, int &tipo_proto);
		VTMsg* __fastcall  MontaMsgAck(int funcao_acked, bool sucesso);
		VTMsg* __fastcall  MontaMsgAlteraCapacitor(int eqpto_tipo, int eqpto_id, strHMS *hms, bool ligado);
		VTMsg* __fastcall  MontaMsgAlteraTrafo(int eqpto_tipo, int eqpto_id, strHMS *hms, int tap);
		VTMsg* __fastcall  MontaMsgEliminaFalta(strHMS *hms);
      VTMsg* __fastcall  MontaMsgExisteEqpto(int eqpto_tipo, int eqpto_id, AnsiString codigo, strHMS *hms);
      VTMsg* __fastcall  MontaMsgManobraChave(int eqpto_tipo, int eqpto_id, strHMS *hms, bool abrir);
      VTMsg* __fastcall  MontaMsgNAck(int funcao_nacked);
      int    __fastcall  MontaMsgRecebida(strBUF *buf, TList *lisMSG);
      VTMsg* __fastcall  MontaMsgRegistroBarraCurto(VTBarCC *barCC, strHMS *hms, int ind_def);
      VTMsg* __fastcall  MontaMsgRegistroBarraFluxo(VTBarra *barra, strHMS *hms, int ind_pat);
      VTMsg* __fastcall  MontaMsgRegistroCapacitor(VTCapacitor *capacitor, strHMS *hms, int ind_pat);
      VTMsg* __fastcall  MontaMsgRegistroChaveCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def);
      VTMsg* __fastcall  MontaMsgRegistroChaveFluxo(VTChave *chave, strHMS *hms, int ind_pat);
		VTMsg* __fastcall  MontaMsgRegistroTrafoCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def);
		VTMsg* __fastcall  MontaMsgRegistroTrafoFluxo(VTTrafo *trafo, strHMS *hms, int ind_pat);
      VTMsg* __fastcall  MontaMsgRegistroTrechoCurto(VTLigCC *ligCC, VTBarCC *bar1CC, VTBarCC *bar2CC, strHMS *hms, int ind_def);
		VTMsg* __fastcall  MontaMsgRegistroTrechoFluxo(VTTrecho *trecho, strHMS *hms, int ind_pat);
		VTMsg* __fastcall  MontaMsgSimulaFalta(int eqpto_tipo, int eqpto_id, int tipo_falha,
                                             double rdef_ohm, double dist_bar1_pu, bool pre_falta, strHMS *hms);
      VTMsg* __fastcall  MontaMsgSolicitaRegistro(int eqpto_tipo, int eqpto_id, strHMS *hms);
		VTMsg* __fastcall  MontaMsgSupervisaoAutomatica(strHMS *hms, bool enabled);

   private: //métodos

   private:

   };

//---------------------------------------------------------------------------
#endif
//eof

