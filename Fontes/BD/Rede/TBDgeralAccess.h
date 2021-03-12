//---------------------------------------------------------------------------
#ifndef TBDgeralAccessH
#define TBDgeralAccessH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBDgeral.h"

//---------------------------------------------------------------------------
class TBDcurvaAccess;
class TBDfunc;
class VTArranjo;
class VTArranjos;
class VTCabo;
class VTCenario;
class VTClasses;
class VTCrescimento;
class VTLog;
class VTMercado;
class VTMonta;
class VTPath;
class VTProgresso;
class VTRegiao;
class VTSuporte;
class VTTipos;

//---------------------------------------------------------------------------
// Estrutura auxiliar para geração dos identificadores sem consultar a base de dados
struct sttIndexGeral
         {
         int ndxArranjoId;
         int ndxCaboId;
         int ndxCenarioId;
         int ndxEstudoId;
			int ndxSegmentoId;
         int ndxSuporteId;
         int ndxSuporteFixacaoId;
         int ndxTipoChaveId;
         int ndxTipoConsId;
         int ndxTipoRedeId;
         };

//---------------------------------------------------------------------------
class TBDgeralAccess : public VTBDgeral
   {
   public:
                 __fastcall  TBDgeralAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn);
                 __fastcall ~TBDgeralAccess(void);
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
      bool       __fastcall  SalvaArranjo(bool bolResetaStatus = false);
      bool       __fastcall  SalvaArranjo(VTArranjo *arranjo, bool bolTipico, bool bolReloadLastId = false);
      bool       __fastcall  SalvaCurvaTipica(bool bolResetaStatus = false);
      bool       __fastcall  SalvaCurvaTipica(int intTipoEquipId, VTCurva *curva);
      bool       __fastcall  SalvaMercado(void);
      bool       __fastcall  SalvaTipoChave(bool bolResetaStatus = false);
      bool       __fastcall  SalvaTipoConsumidor(bool bolResetaStatus = false);
      bool       __fastcall  SalvaTipoRede(bool bolResetaStatus = false);
      bool       __fastcall  SalvaTipos(bool bolResetaStatus = false);
      bool       __fastcall  SalvaGeral(VTGeral* geral);
      int        __fastcall  SuporteUtilizado(int intSuporteId);
      bool       __fastcall  SuporteUtilizado(int intSuporteId, TStringList* arrlist);

   private:  //métodos
      bool __fastcall Conecta(void);
      void __fastcall Desconecta(void);

   private:
      // Leitura
      int        __fastcall FNSNP_rdArranjoIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      int        __fastcall FNSNP_rdCaboIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      int        __fastcall FNSNP_rdCenarioIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
		int        __fastcall FNSNP_rdSegmentoIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
		int        __fastcall FNSNP_rdSuporteIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      //int        __fastcall FNSNP_rdSuporteFixacaoItemProx(VTSuporte* suporte, bool bolLocaIndex = true, bool bolBeginNext = false);
      int        __fastcall FNSNP_rdTipoChaveIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      int        __fastcall FNSNP_rdTipoConsumidorIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      int        __fastcall FNSNP_rdTipoRedeIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      VTArranjo* __fastcall SPSNP_rdArranjo_00_01(int intArranjoId = IDINDEFINIDO);
      VTArranjo* __fastcall SPSNP_rdArranjo_02_04(int intArranjoId = IDINDEFINIDO);
      VTArranjo* __fastcall SPSNP_rdArranjo_03(int intArranjoId = IDINDEFINIDO);
      VTCabo*    __fastcall SPSNP_rdCabo(int intCaboId = IDINDEFINIDO);
      bool       __fastcall SPSNP_rdConjCurvaTipica(void);
      bool       __fastcall SPSNP_rdGeralCorCirc(VTGeral* geral);
      bool       __fastcall SPSNP_rdGeralPadraoRede(VTGeral* geral);
      bool       __fastcall SPSNP_rdMercado(VTMercado* mercado);
      bool       __fastcall SPSNP_rdAreasGeograficas(void);
      VTSuporte* __fastcall SPSNP_rdSuporte(int intSuporteId = IDINDEFINIDO);
      int        __fastcall SPSNP_rdSuporteUtilizado(int intSuporteId);
      bool       __fastcall SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist);
      bool       __fastcall SPSNP_rdTipoChave(void);
      bool       __fastcall SPSNP_rdTipoConsumidor(void);
      bool       __fastcall SPSNP_rdTipoRede(int intEstudoId = IDINDEFINIDO);
      // Escrita
      bool       __fastcall SPSNP_dlAreaGeografica(void);
      bool       __fastcall SPSNP_dlArranjoCaboTipico(void);
      bool       __fastcall SPSNP_dlCenario(void);
      bool       __fastcall SPSNP_dlGeralCorCirc(void);
      bool       __fastcall SPSNP_dlGeralPadraoRede(void);
		bool       __fastcall SPSNP_dlSegmentoMercado(VTSegmento* segmento);
		bool       __fastcall SPSNP_dlSuporteFixacao(int intSuporteId);
      bool       __fastcall SPSNP_wrArranjo(VTArranjo *arranjo, bool bolTipico);
      bool       __fastcall SPSNP_wrArranjo00(VTArranjo *arranjo);
      bool       __fastcall SPSNP_wrArranjo01(VTArranjo *arranjo);
      bool       __fastcall SPSNP_wrArranjo02(VTArranjo *arranjo);
      bool       __fastcall SPSNP_wrArranjo03(VTArranjo *arranjo);
      bool       __fastcall SPSNP_wrArranjo04(VTArranjo *arranjo);
      bool       __fastcall SPSNP_wrCabo(VTCabo *cabo);
      bool       __fastcall SPSNP_wrCaboZ0Z1(VTCabo *cabo);
      bool       __fastcall SPSNP_wrCaboRGMR(VTCabo *cabo);
      bool       __fastcall SPSNP_wrCenario(VTCenario* cenario);
      bool       __fastcall SPSNP_wrGeralCorCirc(VTGeral* geral);
      bool       __fastcall SPSNP_wrGeralPadraoRede(VTGeral* geral);
      bool       __fastcall SPSNP_wrMercado(VTMercado* mercado);
      bool       __fastcall SPSNP_wrAreaGeografica(VTRegiao* regiao);
		bool       __fastcall SPSNP_wrSegmentoMercado(VTSegmento* segmento);
		bool       __fastcall SPSNP_wrSegmentoCrescimento(VTSegmento* segmento, VTCrescimento* crescimento);
      bool       __fastcall SPSNP_wrSuporte(VTSuporte *suporte);
      bool       __fastcall SPSNP_wrSuporteFixacao(VTSuporte* suporte, bool bolInsert);
      bool       __fastcall SPSNP_wrTipoChave(TList* lstTipoChave);
      bool       __fastcall SPSNP_wrTipoConsumidor(void);
      bool       __fastcall SPSNP_wrTipoRede(TList* lstTipoRede);
      //
      bool       __fastcall RenovaStatus(TList *lisEQP, int ult_id_base);
      bool       __fastcall RenovaStatusTipoChave(TList *lisTIPO, bool bolResetaStatus);
      bool       __fastcall RenovaStatusTipoConsumidor(TList *lisCLASSE);
      bool       __fastcall RenovaStatusTipoRede(TList *lisTIPO);

   private:  //objetos externos
      VTApl                *apl;         // Almighty Apl
      VTArranjos           *arranjos;    // Arranjos de cabos
      VTSNPDBConn          *bd_conn;     // Conexão com a base de dados
      VTMonta              *monta;       // Classe para inserção de objetos de rede
      VTProgresso          *progresso;   // Indicador de progresso das operações
      VTPath               *path;        // Diretórios do sistema
      VTTipos              *tipos;       // Tipos de uso geral
      VTClasses            *classes;     // Classess de consumidores

   private:  //dados locais
      VTLog                *plog;        // Ponteiro para objeto de log
      TBDfunc              *bd_func;     // Classe para manipulação de dados
      TBDcurvaAccess       *bd_curva;    // Funções para gravações de curvas no Access
      struct sttIndexGeral index;        // Indexação local dos identificadores
   };

#endif
//---------------------------------------------------------------------------
//eof