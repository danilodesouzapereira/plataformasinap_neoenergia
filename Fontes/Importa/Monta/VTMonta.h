//-----------------------------------------------------------------------------
#ifndef VTMontaH
#define VTMontaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TStruct.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTBarra;
class VTCarga;
class VTCurva;
class VTEqpto;
class VTRede;
class VTEstudo;
class VTDominio;

//-----------------------------------------------------------------------------
class VTMonta : public TObject
   {
   public:
                         __fastcall  VTMonta(void) {};
      virtual            __fastcall ~VTMonta(void) {};
      virtual bool       __fastcall  DefineBarrraInicial(AnsiString rede_cod, int barra_id)=0;
      //virtual int        __fastcall  ExisteFase(AnsiString codigo)=0;
      virtual VTBarra*   __fastcall  ExisteBarra(int barra_id)=0;
      virtual VTBarra*   __fastcall  ExisteBarra(AnsiString codigo)=0;
      virtual VTBarra*   __fastcall  ExisteBarraCoordUtm(int utm_x, int utm_y)=0;
      virtual VTBarra*   __fastcall  ExisteBarraCoordUtm(int utm_x, int utm_y, double vnom_kv) = 0;
      virtual VTBarra*   __fastcall  ExisteBarraCoordEsq(int esq_x1, int esq_y1, int esq_x2, int esq_y2)=0;
      virtual VTEqpto*   __fastcall  ExisteEqpto(int eqpto_tipo, AnsiString codigo)=0;
      virtual int        __fastcall  ExisteFase(AnsiString codigo) = 0;
      virtual VTEqpto*   __fastcall  ExisteTrecho(int utm_x1, int utm_y1, int utm_x2, int utm_y2)=0;
      virtual VTEqpto*   __fastcall  ExisteRede(AnsiString codigo)=0;
      virtual VTArranjo* __fastcall  InsereArranjo(strARRANJO2 &str)=0;
      virtual VTEqpto*   __fastcall  InsereBarraEqpto(strBARRA &str)=0;
      virtual void       __fastcall  InsereBarraStart(void)=0;
	  virtual void       __fastcall  InsereBarraStop(void)=0;
	  virtual VTEqpto*   __fastcall  InsereBateria(strBATERIA &str)=0;
      virtual VTEqpto*   __fastcall  InsereCabo(strCABO &str)=0;
      virtual VTEqpto*   __fastcall  InsereCapacitor(strCAPACITOR &str)=0;
      virtual VTEqpto*   __fastcall  InsereCapserie(strCAPSERIE &str)=0;
      virtual VTEqpto*   __fastcall  InsereCarga(strCARGA &str)=0;
      virtual VTEqpto*   __fastcall  InsereCargaNL(strCARGANL &str)=0;
      virtual VTEqpto*   __fastcall  InsereChave(strCHAVE &str)=0;
      virtual VTEqpto*   __fastcall  InsereCluster(strCLUSTER &str)=0;
      virtual VTEqpto*   __fastcall  InsereCorte(int corte_id, int mrede_id)=0;
      virtual VTCurva*   __fastcall  InsereCurvaDemanda(VTCarga *carga, strCURVA &str)=0;
      virtual VTCurva*   __fastcall  InsereCurvaTipicaCapacitor(strCURVA_TIPICA &str)=0;
      virtual VTCurva*   __fastcall  InsereCurvaTipicaCarga(strCURVA_TIPICA &str)=0;
      virtual VTCurva*   __fastcall  InsereCurvaTipicaGerador(strCURVA_TIPICA &str)=0;
      virtual VTCurva*   __fastcall  InsereCurvaTipicaIP(strCURVA_TIPICA &str)=0;
      virtual VTCurva*   __fastcall  InsereCurvaTipicaReator(strCURVA_TIPICA &str)=0;
      virtual VTDominio* __fastcall  InsereDominio(strDOMINIO &str)=0;
      virtual bool       __fastcall  InsereEstudo(strPLAN_ACAO &str)=0;
      virtual bool       __fastcall  InsereEstudo(strPLAN_ALTERNATIVA &str)=0;
      virtual VTEstudo*  __fastcall  InsereEstudo(strPLAN_ESTUDO &str)=0;
      virtual bool       __fastcall  InsereEstudo(strPLAN_IPE &str)=0;
      virtual bool       __fastcall  InsereEstudo(strPLAN_OBRA &str)=0;
      virtual VTEqpto*   __fastcall  InsereFiltro(strFILTRO &str)=0;
      virtual VTEqpto*   __fastcall  InsereGerador(strGERADOR &str)=0;
      virtual VTEqpto*   __fastcall  InsereMedidor(strMEDIDOR &str) = 0;
      virtual VTEqpto*   __fastcall  InsereMedidorCanal(strCANAL &str)=0;
      virtual VTEqpto*   __fastcall  InsereMutua(strMUTUA &str) = 0;
      virtual VTEqpto*   __fastcall  InsereReator(strREATOR &str)=0;
      virtual VTEqpto*   __fastcall  InsereRede(strREDE &str)=0;
      virtual VTEqpto*   __fastcall  InsereReducaoCarga(int mrede_id, strCARGA &str)=0;
      virtual VTEqpto*   __fastcall  InsereReducaoSuprimento(int mrede_id, strSUPRIMENTO &str)=0;
      virtual VTEqpto*   __fastcall  InsereReducaoTrafo(int mrede_id, strTRAFO &str)=0;
      virtual VTEqpto*   __fastcall  InsereReducaoYref(int mrede_id, strYREF &str)=0;
      virtual VTEqpto*   __fastcall  InsereRegulador(strREGULADOR &str)=0;
      virtual VTEqpto*   __fastcall  InsereSuporte(strSUPORTE &str)=0;
      virtual VTEqpto*   __fastcall  InsereSuprimento(strSUPRIMENTO &str)=0;
      virtual bool       __fastcall  InsereTipoChave(strTIPO_CHAVE &str) = 0;
      virtual bool       __fastcall  InsereTipoRede(strTIPO_REDE &str) = 0;
      virtual VTEqpto*   __fastcall  InsereTrafo(strTRAFO &str)=0;
      virtual VTEqpto*   __fastcall  InsereTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2)=0;
      virtual VTEqpto*   __fastcall  InsereTrafo3E(strTRAFO3E &str)=0;
      virtual VTEqpto*   __fastcall  InsereTrafoZZ(strTRAFOZZ &str)=0;
      virtual VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, strARRANJO1 &str_arr)=0;
      virtual VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, strARRANJO2 &str_arr)=0;
      virtual VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, VTArranjo *arranjo)=0;
      virtual VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2, VTArranjo *arranjo)=0;
      virtual void       __fastcall  RedefineId(void)=0;
      virtual void       __fastcall  RedefineVnomBarra(void)=0;
      virtual void       __fastcall  RetiraBarraIsolada(void)=0;
      virtual void       __fastcall  RetiraRede(VTRede *rede)=0;
      virtual bool       __fastcall  TrataFimDeDados(bool define_vnom=true)=0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTMonta* NewObjMonta(VTApl *apl);
VTMonta* NewObjMontaRede(VTApl *apl);
VTMonta* NewObjMontaEstudo(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
