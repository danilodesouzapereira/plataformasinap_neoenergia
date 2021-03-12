//---------------------------------------------------------------------------
#ifndef VTRede3H
#define VTRede3H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;

class TBarra;
class TGrupoLTC12;
class TGrupoLTC32;
class TMedidor;
class TSup;
class TTrafo12;
class TTrafo32;
class TTrecho;
class VTGeralC;

//---------------------------------------------------------------------------
class VTRede3 : public TObject
   {
   public:  // Métodos
                               __fastcall  VTRede3() {};
      virtual                  __fastcall ~VTRede3(void) {};
      virtual int              __fastcall  AnalisaIDBarra(int id_sinap) = 0;
      virtual int              __fastcall  AnalisaIDTrecho(int id_sinap) = 0;
      virtual AnsiString       __fastcall  AtribuiCodigoLigacao(AnsiString codigo,
                                                                TBarra     *bar1,
                                                                TBarra     *bar2=NULL,
                                                                TBarra     *bar3=NULL) = 0;
      virtual void             __fastcall  AtribuiIDs(void) = 0;
      virtual int              __fastcall  ConverteFasesCarga(int    fases_int,
                                                              TBarra *bar1) = 0;
      virtual int              __fastcall  ConverteLigacaoCargaParaEstatico(int lig_carga) = 0;
      virtual void             __fastcall  DeletaLista(TList **lista) = 0;
      virtual bool             __fastcall  ExisteLigacaoComNeutro(TBarra *bar1,
                                                                  TBarra *bar2) = 0;
      virtual bool             __fastcall  ExisteMedidorReal(TBarra *bar1,
                                                             int    tipo_eqpto,
                                                             int    tipo_medicao,
                                                             int    id_calc,
                                                             int    fase_int) = 0;
      virtual bool             __fastcall  IsBarraExternaSup(TBarra *bar1) = 0;
      virtual void             __fastcall  LimpaLista(TList *lista) = 0;
      virtual TBarra  *        __fastcall  LocalizaBarraCodigo(AnsiString codigo,
                                                               TList      *lisBAR_AUX=NULL) = 0;
      virtual TBarra  *        __fastcall  LocalizaBarraId(int   barra_id,
                                                           TList *lisBAR_AUX=NULL) = 0;
      virtual TBarra  *        __fastcall  LocalizaBarraOffset(int   offset,
                                                               TList *lisBAR_AUX=NULL) = 0;
      virtual TGrupoLTC12 *    __fastcall  LocalizaGrupoLTC12(TBarra *bar_ref) = 0;
      virtual TGrupoLTC32 *    __fastcall  LocalizaGrupoLTC32(TBarra *bar_ref) = 0;
      virtual TMedidor *       __fastcall  LocalizaMedidor(void *pvoid) = 0;
      virtual TSup     *       __fastcall  LocalizaSuprimentoBarraExt(TBarra *barra) = 0;
      virtual TSup     *       __fastcall  LocalizaSuprimentoBarraInt(TBarra *barra) = 0;
      virtual TTrafo12 *       __fastcall  LocalizaTrafo12Id(int trafo12_id) = 0;
      virtual TTrafo32 *       __fastcall  LocalizaTrafo32Id(int trafo32_id) = 0;
      virtual TTrecho  *       __fastcall  LocalizaTrechoId(int   trecho_id,
                                                            TList *lisTRECHO_AUX=NULL) = 0;
      virtual bool             __fastcall  TodosMedidoresReaisPotenciaBarra(void) = 0;
      virtual bool             __fastcall  VerificaFasesCarga(TBarra *bar1,
                                                              int    fases_carga) = 0;

   public:  // Dados
      TList *lis12;       // Trafo12 + Reg12
      TList *lis32;       // Trafo32 + Reg32
      // TList *lisATER;
      TList *lisBANCO_DF; // Lista de bancos ligados em Delta-Fechado
      TList *lisBAR;
      TList *lisBATERIA;
      TList *lisCAPS;
      TList *lisCARGA;
      TList *lisCNL;
      TList *lisGRUPO_LTC12;
      TList *lisGRUPO_LTC32;
      TList *lisGRUPO_MUTUA;
      TList *lisMED;
      TList *lisMED_LIGADOS;
      TList *lisREG12;
      TList *lisREG32;
      TList *lisSUP;
      TList *lisTRAFO12;
      TList *lisTRAFO32;
      TList *lisTRAFO33;
      TList *lisTRAFOEQUIV;
      TList *lisTRAFOZZ;
      TList *lisTRECHO;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRede3
//---------------------------------------------------------------------------
VTRede3 * __fastcall NewObjRede3(VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

