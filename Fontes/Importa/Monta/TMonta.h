//---------------------------------------------------------------------------
#ifndef TMontaH
#define TMontaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"

//---------------------------------------------------------------------------
class TMonta : public VTMonta
   {
   public:
                 __fastcall  TMonta(VTApl *apl);
                 __fastcall ~TMonta(void);
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
      void       __fastcall  IniciaLisRedeDaBaseDado(void);
      VTArranjo* __fastcall  InsereArranjo(strARRANJO2 &str);
      VTEqpto*   __fastcall  InsereBarraEqpto(strBARRA &str);
      void       __fastcall  InsereBarraStart(void);
	  void       __fastcall  InsereBarraStop(void);
	  VTEqpto*   __fastcall  InsereBateria(strBATERIA &str);
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
      //
      //bool       __fastcall  InsereTipoChave(strTIPO_EQPTO &str);
      //bool       __fastcall  InsereTipoRede(strTIPO_EQPTO &str);
      //
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

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      struct   {
               VTMonta *estudo;
               VTMonta *rede;
               }monta;
   };

#endif
//-----------------------------------------------------------------------------
// eof
