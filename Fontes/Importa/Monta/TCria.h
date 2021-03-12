//---------------------------------------------------------------------------
#ifndef TCriaH
#define TCriaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTCria.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTArranjos;
class VTBarra;
class VTCabo;
class VTCarga;
class VTChave;
class VTClasse;
class VTClasses;
class VTCurva;
class VTCurvas;
class VTFases;
class VTLigacao;
class VTMercado;
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

//---------------------------------------------------------------------------
class TCria : public VTCria
   {
   public:
                 __fastcall  TCria(VTApl *apl);
                 __fastcall ~TCria(void);
      void       __fastcall  DefLisEqptoObra(TList *lisEXT);
      VTArranjo* __fastcall  NewObjArranjo(strARRANJO2 &str);
      VTEqpto*   __fastcall  NewObjBarra(strBARRA &str);
      VTEqpto*   __fastcall  NewObjCabo(strCABO &str);
      VTEqpto*   __fastcall  NewObjCanal(strCANAL &str);
      VTEqpto*   __fastcall  NewObjCapacitor(strCAPACITOR &str);
      VTEqpto*   __fastcall  NewObjCapserie(strCAPSERIE &str);
      VTEqpto*   __fastcall  NewObjCarga(strCARGA &str);
      VTEqpto*   __fastcall  NewObjCargaNL(strCARGANL &str);
      VTEqpto*   __fastcall  NewObjChave(strCHAVE &str);
      VTEqpto*   __fastcall  NewObjCluster(strCLUSTER &str);
      VTEqpto*   __fastcall  NewObjFiltro(strFILTRO &str);
      VTEqpto*   __fastcall  NewObjGerador(strGERADOR &str);
      VTEqpto*   __fastcall  NewObjMedidor(strMEDIDOR &str);
      VTEqpto*   __fastcall  NewObjMutua(strMUTUA &str);
      VTEqpto*   __fastcall  NewObjReator(strREATOR &str);
      VTEqpto*   __fastcall  NewObjRede(strREDE &str);
      VTEqpto*   __fastcall  NewObjRegulador(strREGULADOR &str);
      VTEqpto*   __fastcall  NewObjSuporte(strSUPORTE &str);
      VTEqpto*   __fastcall  NewObjSuprimento(strSUPRIMENTO &str);
      VTEqpto*   __fastcall  NewObjTrafo(strTRAFO &str);
      VTEqpto*   __fastcall  NewObjTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2);
      VTEqpto*   __fastcall  NewObjTrafo3E(strTRAFO3E &str);
      VTEqpto*   __fastcall  NewObjTrafoZZ(strTRAFOZZ &str);
      VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, strARRANJO1 &str_arr);
      VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, strARRANJO2 &str_arr);
      VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, VTArranjo *arranjo);
      VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2, VTArranjo *arranjo);

   private: //métodos
      VTArranjo*   __fastcall CriaArranjoProprio_Z0Z1(strARRANJO1 &str);
      VTArranjo*   __fastcall CriaArranjoTipicoCABOxFASE(VTSuporte *suporte, VTCabo *cabo[]);
      VTArranjo*   __fastcall CriaArranjoTipicoCABO_Z0Z1(VTSuporte *suporte, VTCabo *cabo);
      VTArranjo*   __fastcall CriaArranjoTipico_Z0Z1(strARRANJO1 &str);
      VTCurva*     __fastcall CriaCurvaCanal(strCANAL &str);
      VTCurva*     __fastcall CriaCurvaCapacitor(strCAPACITOR &str);
      VTCurva*     __fastcall CriaCurvaCarga(strCARGA &str);
      VTCurva*     __fastcall CriaCurvaGerador(strGERADOR &str);
      VTCurva*     __fastcall CriaCurvaRede(strREDE &str);
      VTCurva*     __fastcall CriaCurvaSuprimento(strSUPRIMENTO &str);
      AnsiString   __fastcall DefineCodigoArranjo(void);
      VTArranjo*   __fastcall ExisteArranjoTipico(int arranjo_id);
      VTArranjo*   __fastcall ExisteArranjoTipico(AnsiString cod_arranjo, int cfg_arranjo);
      VTArranjo*   __fastcall ExisteArranjoTipico_CABO_FASE(strARRANJO1 &str, VTTipoRede *tiporede);
      VTArranjo*   __fastcall ExisteArranjoTipico_CABO_Z0Z1(strARRANJO1 &str);
      VTArranjo*   __fastcall ExisteArranjoTipico_Z0Z1(strARRANJO1 &str);
      VTBarra*     __fastcall ExisteBarra(int barra_id);
      VTBarra*     __fastcall ExisteBarra(AnsiString codigo);
      VTCarga*     __fastcall ExisteCarga(VTBarra *pbarra);
      VTChave*     __fastcall ExisteChave(VTRede *rede, int chave_id);
      VTClasse*    __fastcall ExisteClasse(strCARGA &str);
      VTEqpto*     __fastcall ExisteEqpto(int eqpto_tipo, int eqpto_id);
      VTEqpto*     __fastcall ExisteEqpto(int eqpto_tipo, AnsiString codigo);
      int          __fastcall ExisteFase(AnsiString codigo);
      int          __fastcall ExisteFaseCarga(AnsiString codigo);
      VTRede*      __fastcall ExisteRede(int rede_id);
      VTTipoChave* __fastcall ExisteTipoChave(int id, int tipo, AnsiString codigo);
      VTTipoRede*  __fastcall ExisteTipoRede(int id, int segmento, AnsiString codigo);
      bool         __fastcall RedefineTipoChave(AnsiString &codigo, int &tipo);
      bool         __fastcall ValidaCaboR_GMR(strCABO &str);
      bool         __fastcall ValidaCaboZOZ1(strCABO &str);
      void         __fastcall ValidaLtcTrafo(void);
      bool         __fastcall ValidaPotCC(VTSuprimento *suprimento);
      void         __fastcall ValidaEsquematicoEqbar(strESQ_EQBAR &str_esq);
      void         __fastcall ValidaEsquematicoLigacao(strESQ_LIGA &str_esq);
      bool         __fastcall ValidaZeqv(VTSuprimento *suprimento);
      bool         __fastcall MontaArranjo(void);

   private: //objetos externos
      VTApl       *apl;
      VTClasses   *classes;
      VTLog       *plog;
      VTArranjos  *arranjos;
      VTCurvas    *curvas;
      VTFases     *fases;
      VTGeral     *geral;
      VTMercado   *mercado;
      VTPatamares *patamares;
      VTRedes     *redes;
      VTTipos     *tipos;
      TList       *lisEQPTO_OBRA;

   private: //dados locais
      TList         *lisTMP;
   };

#endif
//-----------------------------------------------------------------------------
// eof
