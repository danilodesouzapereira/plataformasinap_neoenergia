//---------------------------------------------------------------------------
#ifndef VTBDgeralH
#define VTBDgeralH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;
class VTTipoRede;
class VTCurva;
class VTArranjo;
class VTSNPDBConn;
class VTSegmento;
class VTGeral;

//---------------------------------------------------------------------------
class VTBDgeral : public TObject
   {
   public:
                         __fastcall  VTBDgeral(void) {};
      virtual            __fastcall ~VTBDgeral(void) {};
      virtual bool       __fastcall  LeArranjo(void) = 0;
      virtual VTArranjo* __fastcall  LeArranjo(int intArranjoId, int intTipoArranjoId, bool bolTipico) = 0;
      virtual bool       __fastcall  LeCfgFluxo(void) = 0;
      virtual bool       __fastcall  LeCurvaTipica(void) = 0;
      virtual bool       __fastcall  LeGeral(VTGeral* geral) = 0;
      virtual bool       __fastcall  LeMercado(void) = 0;
      virtual bool       __fastcall  LeTipoChave(void) = 0;
      virtual bool       __fastcall  LeTipoConsumidor(void) = 0;
      virtual bool       __fastcall  LeTipoRede(void) = 0;
      virtual bool       __fastcall  LeTipos(void) = 0;
      virtual bool       __fastcall  LimpezaAuxiliares(void) = 0;
      virtual bool       __fastcall  LimpezaEstudos(void) = 0;
      virtual bool       __fastcall  RetiraCaboSuporteArranjo(void) = 0;
		virtual bool       __fastcall  RetiraSegmentoMercado(VTSegmento *segmento) = 0;
      virtual bool       __fastcall  RetiraTipoChave(void) = 0;
      virtual bool       __fastcall  RetiraTipoRede(void) = 0;

      virtual bool       __fastcall  SalvaCurvaTipica(bool bolResetaStatus = false) = 0;
      virtual bool       __fastcall  SalvaCurvaTipica(int intTipoEquipId, VTCurva *curva) = 0;
      virtual bool       __fastcall  SalvaArranjo(bool bolResetaStatus = false) = 0;
      virtual bool       __fastcall  SalvaArranjo(VTArranjo *arranjo, bool bolTipico, bool bolReloadLastId = false) = 0;
      virtual bool       __fastcall  SalvaMercado(void) = 0;
      virtual bool       __fastcall  SalvaGeral(VTGeral* geral) = 0;
      virtual bool       __fastcall  SalvaTipoChave(bool bolResetaStatus = false) = 0;
      virtual bool       __fastcall  SalvaTipoConsumidor(bool bolResetaStatus = false) = 0;
      virtual bool       __fastcall  SalvaTipoRede(bool bolResetaStatus = false) = 0;
      virtual bool       __fastcall  SalvaTipos(bool bolResetaStatus = false) = 0;
      virtual int        __fastcall  SuporteUtilizado(int intSuporteId) = 0;
      virtual bool       __fastcall  SuporteUtilizado(int intSuporteId, TStringList* arrlist) = 0;
   };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTBDgeral* NewObjBDgeralAccess(VTApl *apl_owner, VTSNPDBConn    *bd_conn);
VTBDgeral* NewObjBDgeralOracle(VTApl *apl_owner, TOracleSession *ptrOracleSession);

#endif
//---------------------------------------------------------------------------
//eof