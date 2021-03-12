//---------------------------------------------------------------------------
#ifndef TExportaANEELH
#define TExportaANEELH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaANEEL.h"

//---------------------------------------------------------------------------
class VTBDaneel;
class VTCurva;
class VTFases;
class VTRede;
class VTTrecho;
class VTRadial;
class VTPrimario;
class VTExportaEqpto;

//---------------------------------------------------------------------------
class TExportaANEEL : public VTExportaANEEL
   {
   public:
           __fastcall  TExportaANEEL(VTApl *apl);
			  __fastcall ~TExportaANEEL(void);
		bool __fastcall  ExportaRedes(AnsiString filename, strOPCAO_ANEEL &opcao);

   private: //métodos
		void       __fastcall AtualizaCargaSemNeutro(TList *lisCARGA);
		void       __fastcall AtualizaTrafoLigacaoDy1(TList *lisTRF);
		void       __fastcall AtualizaTrafoSemNeutro(TList *lisTRF);
		bool       __fastcall ExportaRedes(void);
		void       __fastcall IniciaCodigoBarraComCoordenadas(bool xy_seq);
		void       __fastcall IniciaObjAsIdArranjos(void);
      void       __fastcall IniciaObjAsIdCurvasTipicas(void);
		void       __fastcall IniciaObjAsIdCurvas(TList *lisCURVA);
		void       __fastcall IniciaObjAsIdEqptos(void);
      void       __fastcall IniciaObjAsIdEqptos(TList *lisEQP);
		void       __fastcall IniciaObjAsIdEqptos(VTRede *rede);
      void       __fastcall IniciaObjAsIdTipos(void);
		TList*     __fastcall LisArranjo(void);
		TList*     __fastcall LisEqpto(TList *lisORIG, int tipo_eqpto);
      TList*     __fastcall LisEqpto(VTRede *rede, int tipo_eqpto);
		int        __fastcall LisLigacao(TList *lisORIG, TList *lisDEST, VTRede *rede);
		bool       __fastcall MontaPlanilhaCarga_BT(void);
		bool       __fastcall MontaPlanilhaCarga_MT(void);
		bool       __fastcall MontaPlanilhaChave_BT(void);
		bool       __fastcall MontaPlanilhaChave_MT(void);
		bool       __fastcall MontaPlanilhaCircuito_MT(void);
		bool       __fastcall MontaPlanilhaCondutor(void);
		bool       __fastcall MontaPlanilhaCurvaTipica(void);
		bool       __fastcall MontaPlanilhaGerador_BT(void);
		bool       __fastcall MontaPlanilhaGerador_MT(void);
		bool       __fastcall MontaPlanilhaRegulador_MT(void);
		bool       __fastcall MontaPlanilhaRamal_BT(void);
		bool       __fastcall MontaPlanilhaSegmento_BT(void);
		bool       __fastcall MontaPlanilhaSegmento_MT(void);
		bool       __fastcall MontaPlanilhaTrafoMT_BT(void);
		bool       __fastcall MontaPlanilhaTrafoAT_MT(void);


		/*
		bool       __fastcall MontaBlocoArranjo(TList *lisARRANJO);
		bool       __fastcall MontaBlocoEqptoBarra(TList *lisEQP);
		bool       __fastcall MontaBlocoEqptoCapacitor(TList *lisEQP);
		bool       __fastcall MontaBlocoEqptoCapserie(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCargaIP(TList *lisEQP);
		bool       __fastcall MontaBlocoEqptoCargaComMedicao(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCargaSemMedicao(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoChave(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoFiltro(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoGerador(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoMutua(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoReator(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoRede(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoReguladorAutomatico(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoReguladorTapFixo(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoSuprimento(TList *lisEQP);
		bool       __fastcall MontaBlocoEqptoTrafo(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafoMonofasico(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafo3E(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafoZZ(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrechoArranjoProprio(TList *lisEQP);
		bool       __fastcall MontaBlocoEqptoTrechoArranjoTipico(TList *lisEQP);
		*/

   private: //objetos externos
      VTApl   *apl;
      VTFases *fases;

	private: //dados locais
		strOPCAO_ANEEL  opcao;
		VTBDaneel      *BDaneel;
		VTExportaEqpto *exporta_eqpto;
		VTRadial       *radial;
      TList          *lisREDE;
      TList          *lisLIG;
		TList          *lisEQP;
	};

#endif
//-----------------------------------------------------------------------------
// eof


