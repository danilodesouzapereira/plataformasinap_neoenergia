//---------------------------------------------------------------------------
#ifndef TConsultaH
#define TConsultaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTConsulta.h"

//---------------------------------------------------------------------------
class TConsulta : public VTConsulta
   {
   public:
                    __fastcall TConsulta(VTApl *apl);
                    __fastcall ~TConsulta(void);
      VTCarga*      __fastcall ExisteCarga(VTReducao *reducao, VTBarra *pbarra);
      bool          __fastcall ExisteGeracao(VTMRede *mrede);
      bool          __fastcall ExisteGeracao(VTRede *rede);
      bool          __fastcall ExisteGeracao(VTReducao *reducao);
      bool          __fastcall ExisteGeracaoReducaoVizinha(VTMRede *mrede);
      bool          __fastcall ExisteGeracaoReducaoVizinha(VTMRede *rede, VTCorte *corte);
      VTSuprimento* __fastcall ExisteSuprimento(VTReducao *reducao, VTBarra *pbarra);
      TList*        __fastcall LisEqpto(VTRede *rede, int tipo);
      TList*        __fastcall LisEqpto(VTReducao *reducao, int tipo);
      TList*        __fastcall LisEqptoEsquematicoReducao(VTRedes *redes);
      TList*        __fastcall LisEqptoIsolado(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoRede(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoRedeVisible(VTRede *rede, int tipo);
      TList*        __fastcall LisEqptoRedeVisible(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoRedeReducao(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoRedeReducao(VTRedes *redes, VTArea *area, int tipo);
      TList*        __fastcall LisEqptoRedeReducaoVisible(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoRedeReducaoVisible(VTRedes *redes, VTArea *area, int tipo);
      TList*        __fastcall LisEqptoReducao(TList *lisREDU, int tipo);
      TList*        __fastcall LisEqptoReducaoCarregada(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoReducaoVisible(VTRedes *redes, int tipo);
      TList*        __fastcall LisEqptoReducaoVisible(VTReducao *reducao, int tipo);
      TList*        __fastcall LisRede(VTRedes *redes, int tipo);
      TList*        __fastcall LisRedeCarregada(VTRedes *redes, int tipo);
      TList*        __fastcall LisRedeEsquematico(VTRedes *redes);
      //TList*        __fastcall LisRedeEsquematicoArea(VTRedes *redes, VTArea *area);
      TList*        __fastcall LisRedeNaoCarregada(VTRedes *redes, int tipo);
      TList*        __fastcall LisReducao(VTMRede *mrede);
      TList*        __fastcall LisReducao(VTRedes *redes);
      TList*        __fastcall LisReducaoCarregada(VTRedes *redes);
      TList*        __fastcall LisReducaoCarregadaArea(VTRedes *redes, VTArea *area);
      TList*        __fastcall LisReducaoNaoCarregada(VTRedes *redes);
      TList*        __fastcall LisReducaoVizinha(VTMRede *mrede);
      TList*        __fastcall LisReducaoVizinha(VTMRede *rede, VTCorte *corte);
      VTPatamar*    __fastcall PatamarDemandaMaxima(VTRedes *redes);
      VTPatamar*    __fastcall PatamarDemandaMinima(VTRedes *redes);
      VTPatamar*    __fastcall PatamarFluxoMaximo(VTRedes *redes);
      int           __fastcall QtdeEqptoRedeVisible(VTRedes *redes, int tipo);

   private: //métodos
      TList*        __fastcall LisGeradorSuprimento(VTRedes *redes);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisEQP;
      TList *lisTMP;
   };

#endif
//---------------------------------------------------------------------------
//eof
