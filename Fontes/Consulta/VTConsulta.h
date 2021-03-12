//---------------------------------------------------------------------------
#ifndef VTConsultaH
#define VTConsultaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTBarra;
class VTCarga;
class VTCorte;
class VTMRede;
class VTRede;
class VTRedes;
class VTReducao;
class VTSuprimento;
class VTPatamar;

//---------------------------------------------------------------------------
class VTConsulta : public TObject
   {
   public:
                            __fastcall VTConsulta(void) {};
      virtual               __fastcall ~VTConsulta(void) {};
      virtual VTCarga*      __fastcall ExisteCarga(VTReducao *reducao, VTBarra *pbarra) = 0;
      virtual bool          __fastcall ExisteGeracao(VTMRede *mrede) = 0;
      virtual bool          __fastcall ExisteGeracao(VTRede *rede) = 0;
      virtual bool          __fastcall ExisteGeracao(VTReducao *reducao) = 0;
      virtual bool          __fastcall ExisteGeracaoReducaoVizinha(VTMRede *mrede) = 0;
      //virtual bool          __fastcall ExisteGeracaoReducaoVizinha(VTRede *rede) = 0;
      virtual bool          __fastcall ExisteGeracaoReducaoVizinha(VTMRede *rede, VTCorte *corte) = 0;
      //virtual bool          __fastcall ExisteGeracaoReducaoVizinha(VTRede *rede, VTCorte *corte) = 0;
      //virtual VTReducao*    __fastcall ExisteReducao(VTRede *rede_mestre, VTRede *rede_reduzida) = 0;
      virtual VTSuprimento* __fastcall ExisteSuprimento(VTReducao *reducao, VTBarra *pbarra) = 0;
      virtual TList*        __fastcall LisEqpto(VTRede *rede, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqpto(VTReducao *reducao, int tipo=-1) = 0;
      //virtual TList*        __fastcall LisEqptoEsquematicoReducaoVisible(VTRedes *redes) = 0;
      virtual TList*        __fastcall LisEqptoEsquematicoReducao(VTRedes *redes) = 0;
      virtual TList*        __fastcall LisEqptoIsolado(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRede(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRedeVisible(VTRede *rede, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRedeVisible(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRedeReducao(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRedeReducao(VTRedes *redes, VTArea *area, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRedeReducaoVisible(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoRedeReducaoVisible(VTRedes *redes, VTArea *area, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoReducao(TList *lisREDU, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoReducaoCarregada(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoReducaoVisible(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisEqptoReducaoVisible(VTReducao *reducao, int tipo=-1) = 0;
      virtual TList*        __fastcall LisRede(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisRedeCarregada(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisRedeEsquematico(VTRedes *redes) = 0;
      //virtual TList*        __fastcall LisRedeEsquematicoArea(VTRedes *redes, VTArea *area) = 0;
      virtual TList*        __fastcall LisRedeNaoCarregada(VTRedes *redes, int tipo=-1) = 0;
      virtual TList*        __fastcall LisReducao(VTMRede *mrede) = 0;
      virtual TList*        __fastcall LisReducao(VTRedes *redes) = 0;
      virtual TList*        __fastcall LisReducaoCarregada(VTRedes *redes) = 0;
      virtual TList*        __fastcall LisReducaoCarregadaArea(VTRedes *redes, VTArea *area) = 0;
      virtual TList*        __fastcall LisReducaoNaoCarregada(VTRedes *redes) = 0;
      virtual TList*        __fastcall LisReducaoVizinha(VTMRede *rede) = 0;
      virtual TList*        __fastcall LisReducaoVizinha(VTMRede *rede, VTCorte *corte) = 0;
      virtual VTPatamar*    __fastcall PatamarDemandaMaxima(VTRedes *redes) = 0;
      virtual VTPatamar*    __fastcall PatamarDemandaMinima(VTRedes *redes) = 0;
      virtual VTPatamar*    __fastcall PatamarFluxoMaximo(VTRedes *redes) = 0;
      virtual int           __fastcall QtdeEqptoRedeVisible(VTRedes *redes, int tipo=-1) = 0;
   };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTConsulta* __fastcall NewObjConsulta(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
