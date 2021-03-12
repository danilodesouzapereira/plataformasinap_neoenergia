//---------------------------------------------------------------------------
#ifndef VTRede1H
#define VTRede1H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;

class TBarra;
class TGrupoLTC;
class TLigacaoC;
class TMedidor;
class TSup;
class TTrafo2;
class TTrecho;
class VTEqpto;
class VTGeralC;

//---------------------------------------------------------------------------
class VTRede1 : public TObject
   {
   public:  // Métodos
                                 __fastcall  VTRede1() {};
      virtual                    __fastcall ~VTRede1(void) {};
      virtual int                __fastcall  AnalisaIDBarra(int id_sinap) = 0;
      virtual int                __fastcall  AnalisaIDTrecho(int id_sinap) = 0;
      virtual AnsiString         __fastcall  AtribuiCodigoLigacao(AnsiString codigo,
                                                                  TBarra     *bar1,
                                                                  TBarra     *bar2,
                                                                  TBarra     *bar3=NULL) = 0;
      virtual int                __fastcall  AtribuiIDBarra(void) = 0;
      virtual void               __fastcall  AtribuiIDs(void) = 0;
      virtual int                __fastcall  AtribuiIDTrecho(void) = 0;
      virtual void               __fastcall  DeletaLista(TList **lista) = 0;
      virtual bool               __fastcall  ExisteMedidorReal(TBarra *bar1,
                                                               int    tipo_eqpto,
                                                               int    tipo_medicao,
                                                               int    id_calc) = 0;
      virtual void               __fastcall  LimpaLista(TList *lista) = 0;
      virtual TBarra       *     __fastcall  LocalizaBarraCodigo(AnsiString codigo,
                                                                 TList      *lisBAR_AUX=NULL) = 0;
      virtual TBarra       *     __fastcall  LocalizaBarraEqpto(VTEqpto *eqpto) = 0;
      virtual TBarra       *     __fastcall  LocalizaBarraId(int   barra_id,
                                                             TList *lisBAR_AUX=NULL) = 0;
      virtual TBarra       *     __fastcall  LocalizaBarraOffset(int   offset,
                                                                 TList *lisBAR_AUX=NULL) = 0;
      virtual TGrupoLTC    *     __fastcall  LocalizaGrupoLTC(TBarra *bar_ref) = 0;
      virtual TLigacaoC    *     __fastcall  LocalizaLigacaoC(TBarra *bar1,
                                                              void   *plig) = 0;
      virtual TMedidor     *     __fastcall  LocalizaMedidor(void *pvoid) = 0;
      virtual TSup         *     __fastcall  LocalizaSuprimentoBarra(TBarra *barra) = 0;
      virtual TSup         *     __fastcall  LocalizaSuprimentoEqpto(VTEqpto *eqpto) = 0;
      virtual TTrafo2      *     __fastcall  LocalizaTrafo2Eqpto(VTEqpto *eqpto) = 0;
      virtual TTrafo2      *     __fastcall  LocalizaTrafo2Id(int trafo2_id) = 0;
      virtual TTrecho      *     __fastcall  LocalizaTrechoCodigo(AnsiString codigo,
                                                                  TList      *lisTRECHO_AUX=NULL) = 0;
      virtual TTrecho      *     __fastcall  LocalizaTrechoEqpto(VTEqpto *eqpto) = 0;
      virtual TTrecho      *     __fastcall  LocalizaTrechoId(int   trecho_id,
                                                              TList *lisTRECHO_AUX=NULL) = 0;
      virtual bool               __fastcall  TodosMedidoresReaisPotenciaBarra(void) = 0;

   public:  // Dados
      TList *lis2;  // Trafo2 + Reg2
      TList *lisBAR;
      TList *lisBATERIA;
      TList *lisCAPS;
      TList *lisCARGA;
      TList *lisCNL;
      TList *lisFILTRO;
      TList *lisGRUPO_LTC;
      TList *lisGRUPO_MUTUA;
      TList *lisMED;
      TList *lisMED_LIGADOS;
      TList *lisREG2;
      TList *lisSUP;
      TList *lisTRAFO2;
      TList *lisTRAFO3;
      TList *lisTRAFOEQUIV;
      TList *lisTRAFOZZ;
      TList *lisTRECHO;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRede1
//---------------------------------------------------------------------------
VTRede1 * __fastcall NewObjRede1(VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

