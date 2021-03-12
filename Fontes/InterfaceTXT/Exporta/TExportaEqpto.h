//---------------------------------------------------------------------------
#ifndef TExportaEqptoH
#define TExportaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaEqpto.h"
#include "TLine.h"

//---------------------------------------------------------------------------
class VTEqpto;
class VTFases;

//---------------------------------------------------------------------------
class TExportaEqpto : public VTExportaEqpto
   {
   public:
                 __fastcall  TExportaEqpto(VTApl *apl);
                 __fastcall ~TExportaEqpto(void);
      AnsiString __fastcall  MontaArranjoCaboPorFase(VTArranjo *arranjo);
      AnsiString __fastcall  MontaArranjoCaboZ0Z1(VTArranjo *arranjo);
      AnsiString __fastcall  MontaArranjoZ0Z1(VTArranjo *arranjo);
      AnsiString __fastcall  MontaArranjoPUSb100(VTArranjo *arranjo);
	  AnsiString __fastcall  MontaBarra(VTBarra *barra);
	  AnsiString __fastcall  MontaBateria(VTBateria *bateria);
      AnsiString __fastcall  MontaCaboUnipolar(VTCabo *cabo);
      AnsiString __fastcall  MontaCaboZ0Z1(VTCabo *cabo);
      AnsiString __fastcall  MontaCapacitor(VTCapacitor *capacitor);
      AnsiString __fastcall  MontaCapserie(VTCapserie *capserie);
      AnsiString __fastcall  MontaCargaComMedicao(VTCarga *carga);
      AnsiString __fastcall  MontaCargaIP(VTCarga *carga);
      AnsiString __fastcall  MontaCargaSemMedicao(VTCarga *carga);
      AnsiString __fastcall  MontaChave(VTChave *chave);
	  AnsiString __fastcall  MontaCurvaTipica(VTCurva *curva);
	  AnsiString __fastcall  MontaDiagnostico(double limites[27]);
	  AnsiString __fastcall  MontaFiltro(VTFiltro *filtro);
      AnsiString __fastcall  MontaGerador(VTGerador *gerador);
      AnsiString __fastcall  MontaMedidor(VTMedidor *medidor);
      AnsiString __fastcall  MontaMedidorCanal(VTCanal *canal);
      AnsiString __fastcall  MontaMutua(VTMutua *mutua);
      AnsiString __fastcall  MontaReator(VTReator *reator);
      AnsiString __fastcall  MontaRede(VTRede *rede);
      AnsiString __fastcall  MontaReguladorAutomatico(VTRegulador *regulador);
      AnsiString __fastcall  MontaReguladorTapFixo(VTRegulador *regulador);
      AnsiString __fastcall  MontaSuporte(VTSuporte *suporte);
      AnsiString __fastcall  MontaTipoChave(VTTipoChave *tipochave);
      AnsiString __fastcall  MontaTipoConsumidor(VTClasse *classe);
      AnsiString __fastcall  MontaTipoRede(VTTipoRede *tiporede);
      AnsiString __fastcall  MontaSuprimento(VTSuprimento *suprimento);
      AnsiString __fastcall  MontaTrafoMonofasico(VTTrafo *trafo);
      AnsiString __fastcall  MontaTrafoTrifasico2E(VTTrafo *trafo);
      AnsiString __fastcall  MontaTrafoTrifasico3E(VTTrafo3E *trafo);
      AnsiString __fastcall  MontaTrafoZZ(VTTrafoZZ *trafo);
      AnsiString __fastcall  MontaTrechoArranjoTipico(VTTrecho *trecho);
      AnsiString __fastcall  MontaTrechoZ0Z1(VTTrecho *trecho);
      AnsiString __fastcall  MontaVersao(void);

   private: //métodos
      AnsiString __fastcall  LigacaoAsStr(int ligacao);
      AnsiString __fastcall  MontaCurva(VTCurva *curva, bool add_atr_tipica=false);
      AnsiString __fastcall  MontaCurva_Versao1(VTCurva *curva, bool add_atr_tipica);
      AnsiString __fastcall  MontaCurva_Versao2(VTCurva *curva);
      bool       __fastcall  MontaPontosCurva(VTCurva *curva);
      void       __fastcall  MontaRegulador(VTRegulador *regulador);

   private: //objetos externos
      VTApl   *apl;
      VTFases *fases;

   private: //dados
      TLine line;
      #define fmt2_1   "%2.1f"
      #define fmt3_2   "%3.2f"
      #define fmt4_3   "%4.3f"
      #define fmt5_4   "%5.4f"
      #define fmt6_5   "%6.5f"
      #define fmt10_9  "%10.9f"
   };

#endif
//---------------------------------------------------------------------------
//eof

