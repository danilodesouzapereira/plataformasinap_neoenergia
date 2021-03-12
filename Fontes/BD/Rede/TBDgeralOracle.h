//---------------------------------------------------------------------------
#ifndef TBDgeralOracleH
#define TBDgeralOracleH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>
#include "VTBDgeral.h"
#include "..\Rede\Estrutura.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------
class VTRede;
class VTTipos;
class VTArranjo;
class VTArranjos;
class VTCurvas;
class VTClasses;
class VTMonta;
class VTProgresso;
//class VTCronometro;
class VTLog;
class VTCabo;
class VTSuporte;
class VTEqpto;
class VTCurva;
class VTMatZ;
class VTMatC;
class TBDfunc;
class VTCrescimento;
class VTCenario;
class VTMercado;
class VTRegiao;
class VTSegmento;
class VTGeral;
struct strPATAMAR;

//---------------------------------------------------------------------------
class TBDgeralOracle : public VTBDgeral
   {
   public:
                 __fastcall  TBDgeralOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession);
                 __fastcall ~TBDgeralOracle(void);
      bool       __fastcall  LeArranjo(void);
      VTArranjo* __fastcall  LeArranjo(int intArranjoId, int intTipoArranjoId, bool bolTipico);
      bool       __fastcall  LeCfgFluxo(void);
      bool       __fastcall  LeCurvaTipica(void);
      bool       __fastcall  LeGeral(VTGeral* geral);
      bool       __fastcall  LeMercado(void);
		bool       __fastcall  LeTipoChave(void);
      bool       __fastcall  LeTipoConsumidor(void);
      bool       __fastcall  LeTipoRede(void);
      bool       __fastcall  LeTipos(void);
      bool       __fastcall  LimpezaAuxiliares(void);
      bool       __fastcall  LimpezaEstudos(void);
      bool       __fastcall  RetiraCaboSuporteArranjo(void);
		bool       __fastcall  RetiraSegmentoMercado(VTSegmento* segmento);
      bool       __fastcall  RetiraTipoChave(void);
      bool       __fastcall  RetiraTipoRede(void);
      bool       __fastcall  SalvaTipoChave(bool bolResetaStatus = false);
      bool       __fastcall  SalvaTipoConsumidor(bool bolResetaStatus = false);
      bool       __fastcall  SalvaTipoRede(bool bolResetaStatus = false);
      bool       __fastcall  SalvaTipos(bool bolResetaStatus = false);
      bool       __fastcall  SalvaArranjo(bool bolResetaStatus = false);
      bool       __fastcall  SalvaArranjo(VTArranjo *arranjo, bool bolTipico, bool bolReloadLastId = false);
      bool       __fastcall  SalvaCurvaTipica(bool bolResetaStatus = false);
      bool       __fastcall  SalvaCurvaTipica(int intTipoEquipId, VTCurva *curva);
      bool       __fastcall  SalvaMercado(void);
      bool       __fastcall  SalvaGeral(VTGeral* geral);
      int        __fastcall  SuporteUtilizado(int intSuporteId);
      bool       __fastcall  SuporteUtilizado(int intSuporteId, TStringList* arrlist);

   private:  //objetos externos
      VTApl*       apl;
      VTTipos*     tipos;
      VTArranjos*  arranjos;
      VTClasses*   classes;
      VTGeral*     geral;

   private: //dados locais
      TOracleSession* bd_session;
      VTMonta*        monta;       // Classe para inserção de objetos de rede
      VTProgresso*    progresso;   // Indicador de progresso das operações
      //VTCronometro*   cronometro;  // Cronometro das operações
      VTLog*          plog;        // Ponteiro para objeto de log
      TBDfunc*        bd_func;

   private:  //métodos locais
     // Acesso à base de dados
      bool         __fastcall SPSNP_rdTipoChave(void);
      bool         __fastcall SPSNP_wrTipoChave(TList* lstTipoChave);
      bool         __fastcall SPSNP_wrTipoConsumidor(void);
      bool         __fastcall SPSNP_rdTipoRede(int intEstudoId = IDINDEFINIDO);
      bool         __fastcall SPSNP_wrTipoRede(TList* lstTipoRede);
      bool         __fastcall SPSNP_rdTipoConsumidor(void);
      bool         __fastcall SPSNP_rdConjCurvaTipica(void);

      VTCabo*      __fastcall SPSNP_rdCabo(int intCaboId = IDINDEFINIDO);
      VTSuporte*   __fastcall SPSNP_rdSuporte(int intSuporteId = IDINDEFINIDO);
      int          __fastcall SPSNP_rdSuporteUtilizado(int intSuporteId);
      bool         __fastcall SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist);
      VTArranjo*   __fastcall SPSNP_rdArranjo_00_01(int intArranjoId = IDINDEFINIDO);
      VTArranjo*   __fastcall SPSNP_rdArranjo_02_04(int intArranjoId = IDINDEFINIDO);
      VTArranjo*   __fastcall SPSNP_rdArranjo_03(int intArranjoId = IDINDEFINIDO);

      bool         __fastcall SPSNP_wrCabo(VTCabo *cabo);
      bool         __fastcall SPSNP_wrSuporte(VTSuporte *suporte);
      bool         __fastcall SPSNP_dlSuporteFixacao(int intSuporteId);
		bool         __fastcall SPSNP_wrSuporteFixacao(VTSuporte* suporte);
      bool         __fastcall SPSNP_wrArranjo(VTArranjo *arranjo, bool bolTipico);
      bool         __fastcall SPSNP_dlArranjoCaboTipico(void);
      bool         __fastcall SPSNP_dlLimpezaAuxiliares(void);
      bool         __fastcall SPSNP_dlLimpezaEstudos(void);

      // Mercado
      bool         __fastcall SPSNP_rdMercado(VTMercado* mercado);
      bool         __fastcall SPSNP_rdAreasGeograficas(void);
      bool         __fastcall SPSNP_wrMercado(VTMercado* mercado);
      bool         __fastcall SPSNP_wrAreaGeografica(VTRegiao* regiao);
		bool         __fastcall SPSNP_wrSegmentoMercado(VTSegmento* segmento);
		bool         __fastcall SPSNP_wrSegmentoCrescimento(VTSegmento* segmento, VTCrescimento* crescimento);
		bool         __fastcall SPSNP_dlSegmentoMercado(VTSegmento* segmento);
      bool         __fastcall SPSNP_wrCenario(VTCenario* cenario);
      bool         __fastcall SPSNP_dlCenario(void);
      bool         __fastcall SPSNP_dlAreaGeografica(void);

		// Mercado
      bool         __fastcall SPSNP_rdGeralCorCirc(VTGeral* geral);
      bool         __fastcall SPSNP_rdGeralPadraoRede(VTGeral* geral);
      bool         __fastcall SPSNP_wrGeralCorCirc(VTGeral* geral);
      bool         __fastcall SPSNP_wrGeralPadraoRede(VTGeral* geral);

      bool         __fastcall SPSNP_wrConjCurvaSegmento(int& intConjCurvaId, int& intCurvaId, int& intSegmentoId,
                                                        AnsiString strDescricao, bool bolTipica, int intStatus, int intTipoCurvaId,
                                                        TLOBLocator* lobLocator = NULL
                                                       );

   };

#endif
//---------------------------------------------------------------------------
//eof
