//---------------------------------------------------------------------------
#ifndef TExportaODSSH
#define TExportaODSSH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaODSS.h"

//---------------------------------------------------------------------------
class VTFases;
class VTArranjo;
class VTBarra;
class VTCarga;
class VTRede;
class VTRedes;
class VTRegulador;
class VTTrafo;
class VTMonta;
class VTNet;
class VTZona;
struct odssBUS;

//---------------------------------------------------------------------------
class TExportaODSS : public VTExportaODSS
   {
   public:
           __fastcall  TExportaODSS(VTApl *apl);
           __fastcall ~TExportaODSS(void);
      bool __fastcall  ExportaRedes(AnsiString arq_txt, strOPCAO_ODSS &opcao);
      bool __fastcall  ExportaUmaRede(VTRede *rede, AnsiString arq_txt, strOPCAO_ODSS &opcao);
	  bool __fastcall  ExportaCenarioEstabilidade(AnsiString arq_txt);

   private: //métodos
      bool       __fastcall AtualizaCargaEquivalente(void);
      bool       __fastcall AtualizaFasesBarraDosReguladores(void);
      bool       __fastcall AtualizaFasesBarraDosTrafos(void);
      void       __fastcall AtualizaLigacaoTrafoDy1(void);
      AnsiString __fastcall CodigoOdssEqpto(VTEqpto *eqpto);
      VTCarga*   __fastcall CriaCargaEquivalente(TList *lisCARGA);
      void       __fastcall EliminaRedesComeErroNoFluxo(void);
      VTZona*    __fastcall ExisteZona(TList *lisZONA, double vff_kv);
      bool       __fastcall ExportaRedes(void);
      bool       __fastcall ExportaUmaRede(VTRede *rede);
      int        __fastcall FiltraLisArranjo(TList *lisORIG, TList *lisDEST, int tipocfg);
      int        __fastcall FiltraLisTrecho(TList *lisORIG, TList *lisDEST, bool tipico);
      bool       __fastcall IniciaBus(odssBUS &bus, VTBarra *barra);
      bool       __fastcall IniciaNet(void);
		void       __fastcall InsereHeader(AnsiString header);
      int        __fastcall LisArranjoTipico(TList *lisDEST);
      int        __fastcall LisCargaCurvaPropria(TList *lisDEST);
      int        __fastcall LisEqbar(TList *lisDEST, int tipo_eqpto);
      int        __fastcall LisGerador(int tiposup);
      int        __fastcall LisLigacao(TList *lisDEST, int tipo_eqpto);
      int        __fastcall LisSuprimento(int tiposup);
      bool       __fastcall MontaBlocoClear(void);
      bool       __fastcall MontaBlocoNewCapacitor(void);
	  bool       __fastcall MontaBlocoNewCircuit(void);
	  bool       __fastcall MontaBlocoNewIndMach012(void);
	  bool       __fastcall MontaBlocoNewFault(void);
      bool       __fastcall MontaBlocoNewGeneratorPV(void);
      bool       __fastcall MontaBlocoNewLine(void);
      bool       __fastcall MontaBlocoNewLineArranjoProprio(void);
      bool       __fastcall MontaBlocoNewLineArranjoTipico(void);
      bool       __fastcall MontaBlocoNewLineCode(void);
      bool       __fastcall MontaBlocoNewLineCodeMatZ(void);
      bool       __fastcall MontaBlocoNewLineCodeZ0Z1(void);
      bool       __fastcall MontaBlocoNewLoad(void);
      bool       __fastcall MontaBlocoNewLoadMonofasica(VTCarga *carga);
      bool       __fastcall MontaBlocoNewLoadShapePropria(void);
      bool       __fastcall MontaBlocoNewLoadShapeTipica(void);
	  bool       __fastcall MontaBlocoNewLoadShapeTipicaIP(void);
	  bool       __fastcall MontaBlocoNewMonitor(void);
      bool       __fastcall MontaBlocoNewRegControl(void);
      bool       __fastcall MontaBlocoNewTransformer(void);
      bool       __fastcall MontaBlocoNewTransformer_Monofasico2E(void);
      bool       __fastcall MontaBlocoNewTransformer_Monofasico3E(void);
	  bool       __fastcall MontaBlocoNewTransformer_3F2E(void);
	  bool       __fastcall MontaBlocoPlotMonitors(void);
      bool       __fastcall MontaBlocoSet_ctkmodel(void);
      bool       __fastcall MontaBlocoSetVoltagebases(void);
	  bool       __fastcall MontaBlocoSolve(void);
	  bool       __fastcall MontaBlocoSolveEstabilidade(void);
	  bool       __fastcall MontaUM_Regulador(VTRegulador *regulador, int fases);
	  AnsiString __fastcall NomeElementMonitor(VTEqpto *eqpto);
      int        __fastcall NumeroDeFases(int fases_tag);
      int        __fastcall NumeroDeFases(VTArranjo *arranjo);
      int        __fastcall NumeroDeFases(VTCarga   *carga);
      int        __fastcall NumeroDeFases(VTRegulador *regulador);
      int        __fastcall NumeroDeFases(VTTrafo   *trafo);
      int        __fastcall NumeroDeTrafosMonofasicos(TList *lisTRF, VTBarra *barra1, VTBarra *barra2);
	  int        __fastcall TraduzModeloCarga(int modelo_sinap);

	  //get nomes criados...
	  AnsiString __fastcall NomeElementSuprimento(void);

   private: //objetos externos
      VTApl   *apl;
      VTFases *fases;
      //VTRedes *redes;

   private: //dados locais
      double         comp_min_km;
      VTMonta        *monta;
      VTNet          *net;
      TList          *lisREDE;
      TList          *lisARR;
      TList          *lisCAR;
      TList          *lisEQP;
      TList          *lisLIG;
      TList          *lisZONA;
	  TStrings       *lines;
	  TStrings       *monitores;
   };

#endif
//-----------------------------------------------------------------------------
// eof


