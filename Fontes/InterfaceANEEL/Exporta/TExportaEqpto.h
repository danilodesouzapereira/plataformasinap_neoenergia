//---------------------------------------------------------------------------
#ifndef TExportaEqptoH
#define TExportaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaEqpto.h"

//---------------------------------------------------------------------------
class VTBDaneel;
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
		bool __fastcall  MontaCargaBT(VTRede *rede, VTCarga *carga, sdi_CARGA_BT &sdi);
		bool __fastcall  MontaCargaMT(VTRede *rede, VTCarga *carga, sdi_CARGA_MT &sdi);
		bool __fastcall  MontaChaveBT(VTChave *chave, sdi_CHAVE_BT &sdi);
		bool __fastcall  MontaChaveMT(VTChave *chave, sdi_CHAVE_MT &sdi);
		bool __fastcall  MontaCircuitoMT(VTRede *rede_sub, VTRede *rede_pri, sdi_CIRCUITO &sdi);
		bool __fastcall  MontaCondutor(VTArranjo *arranjo, sdi_CONDUTOR &sdi);
		bool __fastcall  MontaCurvaTipica(VTCurva *curva, sdi_CURVA &sdi);
		bool __fastcall  MontaGeradorBT(VTRede *rede, VTGerador *gerador, sdi_GERADOR_BT &sdi);
		bool __fastcall  MontaGeradorMT(VTRede *rede, VTGerador *gerador, sdi_GERADOR_MT &sdi);
		bool __fastcall  MontaRamalBT(VTTrecho *trecho, sdi_RAMAL &sdi);
		bool __fastcall  MontaReguladorMT(VTRegulador *regulador, int seq, int fases_tag, sdi_REGULADOR &sdi);
		bool __fastcall  MontaSegmentoBT(VTTrecho *trecho, sdi_SEGMENTO_BT &sdi);
		bool __fastcall  MontaSegmentoMT(VTTrecho *trecho, sdi_SEGMENTO_MT &sdi);
		bool __fastcall  MontaTrafoAT_MT(VTRede *rede_sub, VTTrafo *trafo, int seq, sdi_TRAFO_AT_MT &sdi);
		bool __fastcall  MontaTrafoMT_BT(VTRede *rede_pri, VTTrafo *trafo, int seq, sdi_TRAFO_MT_BT &sdi);

	private: //métodos
      AnsiString __fastcall  LigacaoAsStr(int ligacao);
		void       __fastcall  MontaRegulador(VTRegulador *regulador);
		bool       __fastcall  MontaVetValoresCurva_kw(VTCurva *curva, double *vetp_kw, int vetp_size);
		bool       __fastcall  TensaoNominal(VTRede *rede_pri, double &vnom_kv);
		bool       __fastcall  TensaoOperacao(VTRede *rede_sub, double &vope_pu);
		void       __fastcall  VerificaTrafoFases(VTTrafo *trafo, AnsiString &fases_pri,
																AnsiString &fases_sec, AnsiString &fases_ter);
		int        __fastcall  VerificaTrafoMRT(VTTrafo *trafo);
		int        __fastcall  VerificaTrafoTipo(VTTrafo *trafo);

	private: //objetos externos
		#define        SIZEOF_VET_96   96
		int       codbase;
		VTApl     *apl;
		VTBDaneel *BDaneel;
		VTDemanda *demanda;
		VTFases   *fases;
		VTLog     *plog;

	private: //dados locais
		TList *lisEQP;
	};

#endif
//---------------------------------------------------------------------------
//eof

