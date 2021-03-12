//---------------------------------------------------------------------------
#ifndef TRede3H
#define TRede3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\VTRede3.h"
using namespace std;

class VTApl;

//---------------------------------------------------------------------------
class TRede3 : public VTRede3
   {
   public:  // Métodos
                       __fastcall  TRede3(VTGeralC *geralC);
                       __fastcall ~TRede3(void);
      int              __fastcall  AnalisaIDBarra(int id_sinap);
      int              __fastcall  AnalisaIDTrecho(int id_sinap);
      AnsiString       __fastcall  AtribuiCodigoLigacao(AnsiString codigo,
                                                        TBarra     *bar1,
                                                        TBarra     *bar2=NULL,
                                                        TBarra     *bar3=NULL);
      void             __fastcall  AtribuiIDs(void);
      int              __fastcall  ConverteFasesCarga(int    fases_int,
                                                      TBarra *bar1);
      int              __fastcall  ConverteLigacaoCargaParaEstatico(int lig_carga);
      void             __fastcall  DeletaLista(TList **lista);
      bool             __fastcall  ExisteLigacaoComNeutro(TBarra *bar1,
                                                          TBarra *bar2);
      bool             __fastcall  ExisteMedidorReal(TBarra *bar1,
                                                     int    tipo_eqpto,
                                                     int    tipo_medicao,
                                                     int    id_calc,
                                                     int    fase_int);
      bool             __fastcall  IsBarraExternaSup(TBarra *bar1);
      void             __fastcall  LimpaLista(TList *lista);
      TBarra  *        __fastcall  LocalizaBarraCodigo(AnsiString codigo,
                                                       TList      *lisBAR_AUX=NULL);
      TBarra  *        __fastcall  LocalizaBarraId(int   barra_id,
                                                   TList *lisBAR_AUX=NULL);
      TBarra  *        __fastcall  LocalizaBarraOffset(int   offset,
                                                       TList *lisBAR_AUX=NULL);
      TGrupoLTC12 *    __fastcall  LocalizaGrupoLTC12(TBarra *bar_ref);
      TGrupoLTC32 *    __fastcall  LocalizaGrupoLTC32(TBarra *bar_ref);
      TMedidor *       __fastcall  LocalizaMedidor(void *pvoid);
      TSup     *       __fastcall  LocalizaSuprimentoBarraExt(TBarra *barra);
      TSup     *       __fastcall  LocalizaSuprimentoBarraInt(TBarra *barra);
      TTrafo12 *       __fastcall  LocalizaTrafo12Id(int trafo12_id);
      TTrafo32 *       __fastcall  LocalizaTrafo32Id(int trafo32_id);
      TTrecho  *       __fastcall  LocalizaTrechoId(int   trecho_id,
                                                    TList *lisTRECHO_AUX=NULL);
      bool             __fastcall  TodosMedidoresReaisPotenciaBarra(void);
      bool             __fastcall  VerificaFasesCarga(TBarra *bar1,
                                                      int    fases_carga);

   private:  // Objetos externos
      VTApl    *apl;
      VTGeralC *geralC;

   private:  // Dados
      int             Maior_ID_Barra, Maior_ID_Trecho;
      double          raiz3, raiz3_inverso;
      complex<double> cum, czero;
   };

#endif
//---------------------------------------------------------------------------

