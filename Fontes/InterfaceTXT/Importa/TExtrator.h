//---------------------------------------------------------------------------
#ifndef TExtratorH
#define TExtratorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExtrator.h"

//---------------------------------------------------------------------------
class VTPreMonta;
class VTLog;
struct sdi_CURVA;

//---------------------------------------------------------------------------
class TExtrator : public VTExtrator
   {
   public:
           __fastcall  TExtrator(VTApl *apl);
           __fastcall ~TExtrator(void);
      bool __fastcall  InsereArranjoCaboPorfase(TStrings *campos);
      bool __fastcall  InsereArranjoCaboZ0Z1(TStrings *campos);
      bool __fastcall  InsereArranjoPUSB100(TStrings *campos);
      bool __fastcall  InsereArranjoZ0Z1(TStrings *campos);
      bool __fastcall  InsereBarra(TStrings *campos);
      void __fastcall  InsereBarraStart(void);
	  void __fastcall  InsereBarraStop(void);
	  bool __fastcall  InsereBateria(TStrings *campos);
      bool __fastcall  InsereCaboUnipolar(TStrings *campos);
      bool __fastcall  InsereCaboZ0Z1(TStrings *campos);
      bool __fastcall  InsereCapacitor(TStrings *campos);
      bool __fastcall  InsereCapacitorSerie(TStrings *campos);
      bool __fastcall  InsereCargaComMedicao(TStrings *campos);
      bool __fastcall  InsereCargaIP(TStrings *campos);
      bool __fastcall  InsereCargaTipica(TStrings *campos);
      bool __fastcall  InsereChave(TStrings *campos);
      bool __fastcall  InsereCurvaCapacitor(TStrings *campos);
      bool __fastcall  InsereCurvaCarga(TStrings *campos);
      bool __fastcall  InsereCurvaCargaIP(TStrings *campos);
      bool __fastcall  InsereCurvaGerador(TStrings *campos);
	  bool __fastcall  InsereCurvaReator(TStrings *campos);
	  bool __fastcall  InsereDiagnostico(TStrings *campos);
      bool __fastcall  InsereFiltro(TStrings *campos);
      bool __fastcall  InsereGerador(TStrings *campos);
      bool __fastcall  InsereMedidor(TStrings *campos);
      bool __fastcall  InsereMedidorCanal(TStrings *campos);
      bool __fastcall  InsereMutua(TStrings *campos);
      bool __fastcall  InsereReator(TStrings *campos);
      bool __fastcall  InsereRede(TStrings *campos);
      bool __fastcall  InsereReguladorAutomatico(TStrings *campos);
      bool __fastcall  InsereReguladorTapFixo(TStrings *campos);
      bool __fastcall  InsereSuporte(TStrings *campos);
      bool __fastcall  InsereSuprimento(TStrings *campos);
      bool __fastcall  InsereTipoChave(TStrings *campos);
      bool __fastcall  InsereTipoConsumidor(TStrings *campos);
      bool __fastcall  InsereTipoRede(TStrings *campos);
      bool __fastcall  InsereTrafo2E(TStrings *campos);
      bool __fastcall  InsereTrafo3E(TStrings *campos);
      bool __fastcall  InsereTrafoMono(TStrings *campos);
      bool __fastcall  InsereTrafoZZ(TStrings *campos);
      bool __fastcall  InsereTrechoTipico(TStrings *campos);
      bool __fastcall  InsereTrechoZ0Z1(TStrings *campos);
      bool __fastcall  InsereVersao(TStrings *campos);
      //
      bool __fastcall  Sucesso(void);
      bool __fastcall  TrataFimDados(void);
      bool __fastcall  TrataIniciaDados(AnsiString cod_dominio);

   protected: //métodos
      bool __fastcall InsereReguladorAutomaticoVersao2(TStrings *campos);
	  bool __fastcall InsereReguladorAutomaticoVersao3(TStrings *campos);
	  bool __fastcall InsereReguladorAutomaticoVersao4(TStrings *campos);
      bool __fastcall LeCurva(sdi_CURVA &sdi, TStrings *campos, int &nc);
      bool __fastcall LeCurva_Versao1(sdi_CURVA &sdi, TStrings *campos, int &nc);
      bool __fastcall LeCurva_Versao2(sdi_CURVA &sdi, TStrings *campos, int &nc);
      bool __fastcall LeCurvaTipica(TStrings *campos, sdi_CURVA  &sdi);
      bool __fastcall LeCurvaTipica_Versao1(TStrings *campos, sdi_CURVA  &sdi);
      bool __fastcall LeCurvaTipica_Versao2(TStrings *campos, sdi_CURVA  &sdi);
      bool __fastcall LePontosCurva(sdi_CURVA &sdi, TStrings *campos, int &nc);

   protected: //objetos externos
      VTApl       *apl;
      VTPreMonta  *pre_monta;
      VTLog       *plog;

   protected: //dados locais
      bool     sucesso;
      int      versao;
   };

#endif
//-----------------------------------------------------------------------------
// eof
