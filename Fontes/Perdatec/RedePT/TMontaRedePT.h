//---------------------------------------------------------------------------
#ifndef TMontaRedePTH
#define TMontaRedePTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaRedePT.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTBarra;
class VTCarga;
class VTLigacao;
class VTClasses;
class VTFases;
class VTPatamares;
class VTRamais;
class VTRedes;
class VTTrecho;
class VTGeral;
class VTZonas;

//---------------------------------------------------------------------------
class TMontaRedePT : public VTMontaRedePT
   {
   public:
                      __fastcall  TMontaRedePT(VTApl *apl);
                      __fastcall ~TMontaRedePT(void);
      void            __fastcall  Clear(void);
      bool            __fastcall  Executa(void);
      bool            __fastcall  DeterminaPerdaMedidor(VTCarga *carga, double &p_w);
      PERDA_MEDIDOR*  __fastcall  ExistePerdaMedidor(VTCarga *carga);
      PERDA_RAMAL*    __fastcall  ExistePerdaRamal(VTCarga *carga);
      PERDA_TRAFO*    __fastcall  ExistePerdaTrafo(VTTrafo *trafo);
      PERDA_TRAFO_SED*__fastcall  ExistePerdaTrafoSED(VTTrafo *trafo);
      VTRedePT*       __fastcall  ExisteRedePT(VTRede *rede);

   private: //métodos
      void       __fastcall AlteraNomeRede(void);
	  VTTrecho*  __fastcall ExisteRamalLigacao(VTCarga *carga, TList *lisRAMAL);
      VTRede*    __fastcall ExisteSubestacao(VTRede *rede_pri);
      int        __fastcall IniciaLisRamal(VTRedePT *redePT, TList *lisRAMAL);
      bool       __fastcall InsereEqptosRedePT(void);
      bool       __fastcall InsereEqpto_CargaMedidor(VTRedePT *redePT);
      bool       __fastcall InsereEqpto_TrechoRamal(VTRedePT *redePT);
      VTCarga*   __fastcall MontaCargaMedidor(VTCarga *carga);
      VTLigacao* __fastcall MontaTrechoRamal(double comp_m, VTBarra *bar1, VTBarra *bar2, VTArranjo *arranjo);
      bool       __fastcall RedefineCarga_SemNeutro(void);
      bool       __fastcall RedefineRede_Capacitor(bool enabled);
      bool       __fastcall RedefineTrafoSDBT_PerdaFeCu(void);
      bool       __fastcall RedefineTrafoSDBT_Zat(void);
      bool       __fastcall RedefineTrafoSED_PerdaFeCu(void);
      bool       __fastcall RetiraEqpto_RamalLigacao(VTRedePT  *redePT);

   private: //objetos externos
      VTApl       *apl;
      VTClasses   *classes;
      VTFases     *fases;
      VTGeral     *geral;
      VTPatamares *patamares;
      VTRamais    *ramais;
      VTRedes     *redes;
      VTZonas     *zonas;

   private: //dados locais   
      //bool  rede_alterada;
      TList *lisEQP;
      TList *lisRAMAL;
      TList *lisREDEPT;
   };

#endif
//---------------------------------------------------------------------------
//eof

