//---------------------------------------------------------------------------
#ifndef TRede1H
#define TRede1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\VTRede1.h"
using namespace std;

class VTApl;
class VTGeralC;

//---------------------------------------------------------------------------
class TRede1 : public VTRede1
   {
   public:  // Métodos
                         __fastcall  TRede1(VTGeralC *geralC);
                         __fastcall ~TRede1(void);
      int                __fastcall  AnalisaIDBarra(int id_sinap);
      int                __fastcall  AnalisaIDTrecho(int id_sinap);
      AnsiString         __fastcall  AtribuiCodigoLigacao(AnsiString codigo,
                                                          TBarra     *bar1,
                                                          TBarra     *bar2,
                                                          TBarra     *bar3=NULL);
      int                __fastcall  AtribuiIDBarra(void);
      void               __fastcall  AtribuiIDs(void);
      int                __fastcall  AtribuiIDTrecho(void);
      void               __fastcall  DeletaLista(TList **lista);
      bool               __fastcall  ExisteMedidorReal(TBarra *bar1,
                                                       int    tipo_eqpto,
                                                       int    tipo_medicao,
                                                       int    id_calc);
      void               __fastcall  LimpaLista(TList *lista);
      TBarra       *     __fastcall  LocalizaBarraCodigo(AnsiString codigo,
                                                         TList      *lisBAR_AUX=NULL);
      TBarra       *     __fastcall  LocalizaBarraEqpto(VTEqpto *eqpto);
      TBarra       *     __fastcall  LocalizaBarraId(int   barra_id,
                                                     TList *lisBAR_AUX=NULL);
      TBarra       *     __fastcall  LocalizaBarraOffset(int   offset,
                                                         TList *lisBAR_AUX=NULL);
      TGrupoLTC    *     __fastcall  LocalizaGrupoLTC(TBarra *bar_ref);
      TLigacaoC    *     __fastcall  LocalizaLigacaoC(TBarra *bar1,
                                                      void   *plig);
      TMedidor     *     __fastcall  LocalizaMedidor(void *pvoid);
      TSup         *     __fastcall  LocalizaSuprimentoBarra(TBarra *barra);
      TSup         *     __fastcall  LocalizaSuprimentoEqpto(VTEqpto *eqpto);
      TTrafo2      *     __fastcall  LocalizaTrafo2Eqpto(VTEqpto *eqpto);
      TTrafo2      *     __fastcall  LocalizaTrafo2Id(int trafo2_id);
      TTrecho      *     __fastcall  LocalizaTrechoCodigo(AnsiString codigo,
                                                          TList      *lisTRECHO_AUX=NULL);
      TTrecho      *     __fastcall  LocalizaTrechoEqpto(VTEqpto *eqpto);
      TTrecho      *     __fastcall  LocalizaTrechoId(int   trecho_id,
                                                      TList *lisTRECHO_AUX=NULL);
      bool               __fastcall  TodosMedidoresReaisPotenciaBarra(void);

   private:  // Objetos externos
      VTApl    *apl;
      VTGeralC *geralC;

   private:  // Dados
      int             Maior_ID_Barra, Maior_ID_Trecho;
      complex<double> cum, czero, zerov;
   };

#endif
//---------------------------------------------------------------------------

