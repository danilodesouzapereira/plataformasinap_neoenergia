//---------------------------------------------------------------------------
#ifndef TExportaEqptoH
#define TExportaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaEqpto.h"
#include "TLine.h"

//---------------------------------------------------------------------------
class VTDemanda;
class VTEqpto;
class VTFases;
class VTLog;

//---------------------------------------------------------------------------
class TExportaEqpto : public VTExportaEqpto
   {
   public:
                 __fastcall  TExportaEqpto(VTApl *apl);
                 __fastcall ~TExportaEqpto(void);
		AnsiString __fastcall  MontaArranjo(VTArranjo *arranjo);
		AnsiString __fastcall  MontaCoordenada(VTBarra *barra);
		//AnsiString __fastcall  MontaCapacitor(VTCapacitor *capacitor);
		AnsiString __fastcall  MontaCargaBT(VTRede *rede, VTCarga *carga);
		AnsiString __fastcall  MontaCargaMT(VTRede *rede, VTCarga *carga);
		//AnsiString __fastcall  MontaCargaIP(VTCarga *carga);
		AnsiString __fastcall  MontaChaveBT(VTChave *chave);
		AnsiString __fastcall  MontaChaveMT(VTChave *chave);
		AnsiString __fastcall  MontaCircuitoMT(VTRede *rede_sub, VTRede *rede_pri, double vnom_kv, double vope_pu);
		AnsiString __fastcall  MontaGeradorBT(VTRede *rede, VTGerador *gerador);
		AnsiString __fastcall  MontaGeradorMT(VTRede *rede, VTGerador *gerador);
		AnsiString __fastcall  MontaRamalBT(VTTrecho *trecho);
		AnsiString __fastcall  MontaReguladorMT(VTRegulador *regulador, int seq, int fases_tag);
		AnsiString __fastcall  MontaSegmentoBT(VTTrecho *trecho);
		AnsiString __fastcall  MontaSegmentoMT(VTTrecho *trecho);
		//AnsiString __fastcall  MontaSuprimento(VTSuprimento *suprimento);
		AnsiString __fastcall  MontaTrafoAT_MT(VTRede *rede_sub, VTTrafo *trafo, int seq);
		AnsiString __fastcall  MontaTrafoMT_BT(VTRede *rede_pri, VTTrafo *trafo, int seq);
		AnsiString __fastcall  MontaVersao(void);

	private: //métodos
      AnsiString __fastcall  LigacaoAsStr(int ligacao);
		void       __fastcall  MontaRegulador(VTRegulador *regulador);
		void       __fastcall  VerificaTrafoFases(VTTrafo *trafo, AnsiString &fases_pri,
																AnsiString &fases_sec, AnsiString &fases_ter);
		int        __fastcall  VerificaTrafoMRT(VTTrafo *trafo);
		int        __fastcall  VerificaTrafoTipo(VTTrafo *trafo);

	private: //objetos externos
		VTApl     *apl;
		VTDemanda *demanda;
		VTFases   *fases;
		VTLog     *plog;

   private: //dados
      TLine line;
		#define fmt2_1   "%2.1f"
      #define fmt3_2   "%3.2f"
      #define fmt4_3   "%4.3f"
		#define fmt5_4   "%5.4f"
      #define fmt6_5   "%6.5f"
      #define fmt7_6   "%7.6f"
		#define fmt10_9  "%10.9f"
   };

#endif
//---------------------------------------------------------------------------
//eof

