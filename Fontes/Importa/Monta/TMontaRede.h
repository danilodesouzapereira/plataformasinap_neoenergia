//---------------------------------------------------------------------------
#ifndef TMontaRedeH
#define TMontaRedeH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTArranjos;
class VTBarra;
class VTCabo;
class VTCarga;
class VTCargaNL;
class VTChave;
class VTClasse;
class VTClasses;
class VTCurva;
class VTCurvas;
class VTDemanda;
class VTFases;
class VTLigacao;
//class VTMercado;
class VTMontaZ;
class VTPatamares;
class VTRede;
class VTRedes;
class VTSuporte;
class VTSuprimento;
class VTTipoChave;
class VTTipoRede;
class VTTipos;
class VTTrecho;
class VTGeral;
class VTLog;
class VTEsquematico;
class VTListeqp;
class VTValEqpto;

//---------------------------------------------------------------------------
class TMontaRede : public VTMonta
   {
   public:
                 __fastcall  TMontaRede(VTApl *apl);
                 __fastcall ~TMontaRede(void);
      bool       __fastcall  DefineBarrraInicial(AnsiString rede_cod, int barra_id);
      VTBarra*   __fastcall  ExisteBarra(int barra_id);
      VTBarra*   __fastcall  ExisteBarra(AnsiString codigo);
      VTBarra*   __fastcall  ExisteBarraCoordUtm(int utm_x, int utm_y);
      VTBarra*   __fastcall  ExisteBarraCoordUtm(int utm_x, int utm_y, double vnom_kv);
      VTBarra*   __fastcall  ExisteBarraCoordEsq(int esq_x1, int esq_y1, int esq_x2, int esq_y2);
      VTEqpto*   __fastcall  ExisteEqpto(int eqpto_tipo, AnsiString codigo);
      int        __fastcall  ExisteFase(AnsiString codigo);
      VTEqpto*   __fastcall  ExisteRede(AnsiString codigo);
      VTEqpto*   __fastcall  ExisteTrecho(int utm_x1, int utm_y1, int utm_x2, int utm_y2);
      //void       __fastcall  IniciaLisRedeDaBaseDado(void);
      VTArranjo* __fastcall  InsereArranjo(strARRANJO2 &str);
      VTEqpto*   __fastcall  InsereBarraEqpto(strBARRA &str);
      void       __fastcall  InsereBarraStart(void);
	  void       __fastcall  InsereBarraStop(void);
	  VTEqpto* 	 __fastcall  InsereBateria(strBATERIA &str);
      VTEqpto*   __fastcall  InsereCabo(strCABO &str);
      VTEqpto*   __fastcall  InsereCapacitor(strCAPACITOR &str);
      VTEqpto*   __fastcall  InsereCapserie(strCAPSERIE &str);
      VTEqpto*   __fastcall  InsereCarga(strCARGA &str);
      VTEqpto*   __fastcall  InsereCargaNL(strCARGANL &str);
      VTEqpto*   __fastcall  InsereChave(strCHAVE &str);
      VTEqpto*   __fastcall  InsereCluster(strCLUSTER &str);
      VTEqpto*   __fastcall  InsereCorte(int corte_id, int mrede_id);
      VTCurva*   __fastcall  InsereCurvaDemanda(VTCarga *carga, strCURVA &str);
      VTCurva*   __fastcall  InsereCurvaTipicaCapacitor(strCURVA_TIPICA &str);
      VTCurva*   __fastcall  InsereCurvaTipicaCarga(strCURVA_TIPICA &str);
      VTCurva*   __fastcall  InsereCurvaTipicaGerador(strCURVA_TIPICA &str);
      VTCurva*   __fastcall  InsereCurvaTipicaIP(strCURVA_TIPICA &str);
      VTCurva*   __fastcall  InsereCurvaTipicaReator(strCURVA_TIPICA &str);
      VTDominio* __fastcall  InsereDominio(strDOMINIO &str);
      bool       __fastcall  InsereEstudo(strPLAN_ACAO &str);
      bool       __fastcall  InsereEstudo(strPLAN_ALTERNATIVA &str);
      VTEstudo*  __fastcall  InsereEstudo(strPLAN_ESTUDO &str);
      bool       __fastcall  InsereEstudo(strPLAN_IPE &str);
      bool       __fastcall  InsereEstudo(strPLAN_OBRA &str);
      VTEqpto*   __fastcall  InsereFiltro(strFILTRO &str);
      VTEqpto*   __fastcall  InsereGerador(strGERADOR &str);
      VTEqpto*   __fastcall  InsereMedidor(strMEDIDOR &str);
      VTEqpto*   __fastcall  InsereMedidorCanal(strCANAL &str);
      VTEqpto*   __fastcall  InsereMutua(strMUTUA &str);
      VTEqpto*   __fastcall  InsereReator(strREATOR &str);
      VTEqpto*   __fastcall  InsereRede(strREDE &str);
      VTEqpto*   __fastcall  InsereReducaoCarga(int mrede_id, strCARGA &str);
      VTEqpto*   __fastcall  InsereReducaoSuprimento(int mrede_id, strSUPRIMENTO &str);
      VTEqpto*   __fastcall  InsereReducaoTrafo(int mrede_id, strTRAFO &str);
      VTEqpto*   __fastcall  InsereReducaoYref(int mrede_id, strYREF &str);
      VTEqpto*   __fastcall  InsereRegulador(strREGULADOR &str);
      VTEqpto*   __fastcall  InsereSuporte(strSUPORTE &str);
      VTEqpto*   __fastcall  InsereSuprimento(strSUPRIMENTO &str);
      bool       __fastcall  InsereTipoChave(strTIPO_CHAVE &str);
      bool       __fastcall  InsereTipoRede(strTIPO_REDE &str);
      VTEqpto*   __fastcall  InsereTrafo(strTRAFO &str);
      VTEqpto*   __fastcall  InsereTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2);
      VTEqpto*   __fastcall  InsereTrafo3E(strTRAFO3E &str);
      VTEqpto*   __fastcall  InsereTrafoZZ(strTRAFOZZ &str);
      VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, strARRANJO1 &str_arr);
      VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, strARRANJO2 &str_arr);
      VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, VTArranjo *arranjo);
      VTEqpto*   __fastcall  InsereTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2, VTArranjo *arranjo);
      void       __fastcall  RedefineId(void);
      void       __fastcall  RedefineVnomBarra(void);
      void       __fastcall  RetiraBarraIsolada(void);
      void       __fastcall  RetiraRede(VTRede *rede);
      //bool       __fastcall  TransformaIlhasEmRedes(void);
      bool       __fastcall  TrataFimDeDados(bool define_vnom);

   private: //métodos
      void         __fastcall AcumulaCarga(VTCarga *carga, strCARGA &str);
      VTArranjo*   __fastcall CriaArranjoProprioPU_SB100(strARRANJO1 &str, double comp_km);
      VTArranjo*   __fastcall CriaArranjoProprioZ0Z1(strARRANJO1 &str);
      VTArranjo*   __fastcall CriaArranjoTipicoCABOxFASE(AnsiString codigo, TDate data, VTSuporte *suporte, VTCabo *cabo[]);
      VTArranjo*   __fastcall CriaArranjoTipicoCABO_Z0Z1(AnsiString codigo, TDate data, VTSuporte *suporte, VTCabo *cabo);
	  VTArranjo*   __fastcall CriaArranjoTipicoZ0Z1(strARRANJO1 &str);
	  VTCurva* 	   __fastcall CriaCurvaBateria(strCURVA &str);
      VTCurva*     __fastcall CriaCurvaCanal(strCURVA &str, int tipo_canal);
      VTCurva*     __fastcall CriaCurvaCapacitor(strCURVA &str);
      VTCurva*     __fastcall  CriaCurvaCarga(strCURVA &str);
      VTCurva*     __fastcall CriaCurvaGerador(strCURVA &str);
      VTCurva*     __fastcall CriaCurvaReator(strCURVA &str);
      VTCurva*     __fastcall CriaCurvaSuprimento(strCURVA &str);
      VTCurva*     __fastcall CriaCurvaSuprimentoReducao(strCURVA &str);
      void         __fastcall DefineBarraInicial(VTBarra *barra);
      AnsiString   __fastcall DefineCodigoArranjo(void);
      VTArranjo*   __fastcall ExisteArranjoTipico(int arranjo_id);
      VTArranjo*   __fastcall ExisteArranjoTipico(AnsiString cod_arranjo, int cfg_arranjo);
      VTArranjo*   __fastcall ExisteArranjoTipico_CABO_FASE(strARRANJO1 &str, VTTipoRede *tiporede);
      VTArranjo*   __fastcall ExisteArranjoTipico_CABO_Z0Z1(strARRANJO1 &str);
      VTArranjo*   __fastcall ExisteArranjoTipico_Z0Z1(strARRANJO1 &str);
      VTCarga*     __fastcall ExisteCarga(VTBarra *pbarra);
      VTCargaNL*   __fastcall ExisteCargaNL(VTBarra *pbarra, int carga_id);
      VTChave*     __fastcall ExisteChave(VTRede *rede, int chave_id);
      VTClasse*    __fastcall ExisteClasse(strCARGA &str);
      VTEqpto*     __fastcall ExisteEqpto(int eqpto_tipo, int eqpto_id);
      int          __fastcall ExisteFaseCarga(AnsiString codigo);
      VTTipoChave* __fastcall ExisteTipoChave(int id, int tipo, AnsiString codigo);
      VTTipoRede*  __fastcall ExisteTipoRede(int id, int segemnto, AnsiString codigo);
      void         __fastcall InverteCoordUTM(void);
      bool         __fastcall LePontosCurva(VTCurva *curva, strCURVA &str);
      bool         __fastcall MontaArranjo(void);
      void         __fastcall MontaEsquematicoReducao(void);
      void         __fastcall OrdenaBarraPorNome(VTLigacao *ligacao);
//      void         __fastcall RedefineId(TList *lisEQP);
      bool         __fastcall RedefineTipoChave(AnsiString &codigo, int &tipo);
      bool         __fastcall ValidaCaboR_GMR(strCABO &str);
      bool         __fastcall ValidaCaboZOZ1(strCABO &str);
      void         __fastcall ValidaComprimento(VTTrecho *trecho);
      void         __fastcall ValidaLtcTrafo(void);
      bool         __fastcall ValidaPotCC(VTSuprimento *suprimento);
      void         __fastcall ValidaEsquematicoEqbar(strESQ_EQBAR &str_esq);
      void         __fastcall ValidaEsquematicoLigacao(strESQ_LIGA &str_esq);
      bool         __fastcall ValidaZeqv(VTSuprimento *suprimento);

   private: //objetos externos
      VTApl         *apl;
      VTClasses     *classes;
      VTLog         *plog;
      VTArranjos    *arranjos;
      VTCurvas      *curvas;
      VTDemanda     *demanda;
      VTFases       *fases;
      VTGeral       *geral;
      //VTMercado     *mercado;
      VTMontaZ      *montaZ;
      VTPatamares   *patamares;
      VTRedes       *redes;
      VTTipos       *tipos;
      VTValEqpto    *valida;

   private: //dados locais
      VTEsquematico *esquematico;
      VTListeqp     *lisBAR;
      TList         *lisTMP;
   };

#endif
//-----------------------------------------------------------------------------
// eof
