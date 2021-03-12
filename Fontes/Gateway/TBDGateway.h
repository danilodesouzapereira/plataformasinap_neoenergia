//---------------------------------------------------------------------------
#ifndef TBDGatewayH
#define TBDGatewayH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>
#include "VTBDGateway.h"
#include "SNPSTR.h"
#include "Fontes\BD\Rede\BD_inc.h"

//---------------------------------------------------------------------------
// A constante abaixo representa o número máximo de linhas a serem salvas 
// multiplicado pelo número de parâmetros na Stored Procedure. Esse valor 
// deve ser dividido pelo número de parâmetros para que se possa determinar 
// o número de linhas que pode ser lido por vez.
#define BDGATEWAYMAXARRAYDML 250000L

//---------------------------------------------------------------------------
class TBDfunc;
class VTClasses;
class VTCurvas;
class VTLog;
class VTMonta;
class VTPath;
class VTSNPDBConn;
class VTTipos;
class VTCabo;
class VTSuporte;
class VTArranjo;
class VTArranjos;
class VTCronometro;
class VTProgresso;
class VTRede;
class VTRedes;


//---------------------------------------------------------------------------
// Macros

// Macro para criação de um novo parâmetro
#define GTWDB_PARAM_CRT(list, param, name, varT, doaT) \
                        list->Add(param = new struct TBDGatewayProcParam()); \
                        param->strORAname = name; \
                        param->varType = varT; \
                        param->doaType = doaT

// Macro para transferência de um parâmetro para o array
#define GTWDB_PARAM_SET(list, param, ndxArray, varaux, ndxParam, campoobj) \
                        param = (struct TBDGatewayProcParam*)(list->Items[ndxParam]); \
                        varaux = campoobj; \
                        VarArrayPut(param->arrVar, varaux, &ndxArray, 0)



//---------------------------------------------------------------------------
struct TBDGatewayProcParam : public TObject
   {
   SNPSTR     strORAname;
   Variant    arrVar;
   TVarType   varType;
   int        doaType;

   TBDGatewayProcParam()
      {
      strORAname = _T("");
      arrVar;
      varType = varEmpty;
      doaType = otInteger;
      }
   };

//---------------------------------------------------------------------------
class TBDGateway : public VTBDGateway
   {
   public:
             __fastcall  TBDGateway(VTApl *apl_owner);
             __fastcall ~TBDGateway(void);
      bool   __fastcall  Clear(void);
      bool   __fastcall  Commit(void);
      bool   __fastcall  Conecta(void);
      bool   __fastcall  Desconecta(void);
      bool   __fastcall  LeNodesFilho(int node_pai_id);
      bool   __fastcall  LeGrupoRedes(DynamicArray<int> vet_rede_id);
      bool   __fastcall  LeTodasRedes(void);
      bool   __fastcall  LeTipos(void);
      bool   __fastcall  Rollback(void);
      bool   __fastcall  SalvaRedes(void);     // VTRedes no Apl
      bool   __fastcall  SalvaTipos(void);     // VTTipos no Apl
      SNPSTR __fastcall  UltimoErro(void);

   protected:  //métodos acessados via property
      bool __fastcall  PM_GetConectado(void);

   private:  //objetos externos
      VTApl*         apl;
      VTArranjos*    arranjos;
      VTPath*        path;
      VTLog*         plog;
      VTClasses*     classes;
      VTTipos*       tipos;
      VTMonta*       monta;
      VTCronometro*  cronometro;
      VTProgresso*   progresso;
      VTRedes*       redes;
      VTCurvas*      curvas;

      TBDfunc*    bd_func;

   private:  //objetos locais
      TOracleSession *bd_session;
      TADOConnection* dbfConn;
      SNPSTR          strUltimoErro;

   private:  //métodos
      void            __fastcall LimpaTListNaoTObject(TList *lisOBJ);
      TOracleSession* __fastcall IniciaConexaoOracle(AnsiString strTNS, AnsiString strLoginName, AnsiString strLoginPwd);
      void            __fastcall DecodificaUltimoErroDB(SNPSTR strErr);

      bool            __fastcall ArrayVarCria(TList* lstParam, int intMaxItems);
      bool            __fastcall ArrayVarRedim(TList* lstParam, int intNdxMax);
      // bool            __fastcall ArrayVarDBFTransf(struct TBDGatewayProcParam* vecParam, int intVecParamSize, TADODataSet* ptrADODS, int intVerId);
      bool            __fastcall ArrayVarDOADeclVar(TList* lstParam, TOracleQuery* ptrQry);
      bool            __fastcall ArrayVarClean(TList* lstParam);

      void TrataOnArrayError(TOracleQuery* Sender, int Index, int ErrorCode, const System::UnicodeString ErrorMessage, bool& Continue);

      bool            __fastcall LeArranjo(void);
      VTArranjo*      __fastcall LeArranjo(int intArranjoId, int intTipoArranjoId, bool bolTipico);
      bool            __fastcall SalvaArranjos(void);  // VTArranjos no Apl
      bool            __fastcall LeListaRedes(TList* lisRedes);

      bool            __fastcall SPSNP_dlTudo(void);

      VTArranjo*      __fastcall SPSNP_rdArranjo_00_01(int intArranjoId = IDINDEFINIDO);
      VTArranjo*      __fastcall SPSNP_rdArranjo_02_04(int intArranjoId = IDINDEFINIDO);
      VTArranjo*      __fastcall SPSNP_rdArranjo_03(int intArranjoId = IDINDEFINIDO);
      bool            __fastcall SPSNP_rdBarra(int intRedeId);
      bool            __fastcall SPSNP_rdBarraCursor(TOracleQuery* ptrCursor);
      VTCabo*         __fastcall SPSNP_rdCabo(int intCaboId = IDINDEFINIDO);
      bool            __fastcall SPSNP_rdCapacitor(int intRedeId);
      bool            __fastcall SPSNP_rdCapacitorSerie(int intRedeId);
      bool            __fastcall SPSNP_rdCarga(int intRedeId);
      bool            __fastcall SPSNP_rdCargaIP(int intRedeId);
      bool            __fastcall SPSNP_rdCargaNL(int intRedeId);
      bool            __fastcall SPSNP_rdChave(int intRedeId);
      bool            __fastcall SPSNP_rdCurvaTipica(void);
      bool            __fastcall SPSNP_rdFiltro(int intRedeId);
      bool            __fastcall SPSNP_rdGerador(int intRedeId);
      bool            __fastcall SPSNP_rdMutua(int intRedeId);
      bool            __fastcall SPSNP_rdReator(int intTipoRedeId);
      bool            __fastcall SPSNP_rdRede(int intTipoRedeId);
      bool            __fastcall SPSNP_rdRedeNoPai(int intNoPaiId);
      bool            __fastcall SPSNP_rdRedeCursor(TOracleQuery* ptrCursor, TList *lisRedeAux = NULL);
      bool            __fastcall SPSNP_rdRegulador(int intTipoRedeId);
      VTSuporte*      __fastcall SPSNP_rdSuporte(int intSuporteId = IDINDEFINIDO);
      int             __fastcall SPSNP_rdSuporteUtilizado(int intSuporteId);
      bool            __fastcall SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist);
      bool            __fastcall SPSNP_rdSuprimento(int intTipoRedeId);
      bool            __fastcall SPSNP_rdTipoConsumidor(void);
      bool            __fastcall SPSNP_rdTipoRede(void);
      bool            __fastcall SPSNP_rdTipoChave(void);
      bool            __fastcall SPSNP_rdTrafo(int intRedeId);
      bool            __fastcall SPSNP_rdTrafo3E(int intRedeId);
      bool            __fastcall SPSNP_rdTrafoZZ(int intRedeId);
      bool            __fastcall SPSNP_rdTrecho(int intRedeId);

      bool            __fastcall SPSNP_wrEquipamento(TList* lstEqpto);
      bool            __fastcall SPSNP_wrEquipamentoRede(VTRede* rede, TList* lstEqpto);
      bool            __fastcall SPSNP_wrBarra(TList* lstEqpto);
      bool            __fastcall SPSNP_wrCapacitor(TList* lstEqpto);
      bool            __fastcall SPSNP_wrCapacitorSerie(TList* lstEqpto);
      bool            __fastcall SPSNP_wrCarga(TList* lstEqpto);
      bool            __fastcall SPSNP_wrCargaNL(TList* lstEqpto);
      bool            __fastcall SPSNP_wrCargaIP(TList* lstEqpto);
      bool            __fastcall SPSNP_wrChave(TList* lstEqpto);
      bool            __fastcall SPSNP_wrCurvaTipica(void);
      bool            __fastcall SPSNP_wrCurvaPropria(void);
      bool            __fastcall SPSNP_wrFiltro(TList* lstEqpto);
      bool            __fastcall SPSNP_wrGerador(TList* lstEqpto);
      bool            __fastcall SPSNP_wrMutua(TList* lstEqpto);
      bool            __fastcall SPSNP_wrReator(TList* lstEqpto);
      bool            __fastcall SPSNP_wrRede(TList* lstEqpto);
      bool            __fastcall SPSNP_wrRegulador(TList* lstEqpto);
      bool            __fastcall SPSNP_wrSuprimento(TList* lstEqpto);
      bool            __fastcall SPSNP_wrTrafo(TList* lstEqpto);
      bool            __fastcall SPSNP_wrTrafoZZ(TList* lstEqpto);
      bool            __fastcall SPSNP_wrTrafo3E(TList* lstEqpto);
      bool            __fastcall SPSNP_wrTrecho(TList* lstEqpto);
      // Não equipamentos
      bool            __fastcall SPSNP_wrArranjo(VTArranjo *arranjo, bool bolTipico);
      bool            __fastcall SPSNP_wrCabo(VTCabo *cabo);
      bool            __fastcall SPSNP_wrSuporte(VTSuporte *suporte);
      bool            __fastcall SPSNP_wrSuporteFixacao(VTSuporte* suporte);
      bool            __fastcall SPSNP_dlSuporteFixacao(int intSuporteId);
      bool            __fastcall SPSNP_wrTipoChave(TList* lstTipoChave);
      bool            __fastcall SPSNP_wrTipoRede(TList* lstTipoRede);

   };

#endif
//---------------------------------------------------------------------------
//eof

