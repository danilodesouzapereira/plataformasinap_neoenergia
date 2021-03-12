//---------------------------------------------------------------------------
#ifndef TMontaMNetH
#define TMontaMNetH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaMNet.h"

//---------------------------------------------------------------------------
class VTCarga;
class VTCorte;
class VTEqpto;
class VTMalha;
class VTMNet;
class VTPatamares;
class VTMRede;
class VTRede;
class VTReducao;
class VTSuprimento;

//---------------------------------------------------------------------------
class TMontaMNet : public VTMontaMNet
   {
   public:
           __fastcall  TMontaMNet(VTApl *apl);
           __fastcall ~TMontaMNet(void);
      void __fastcall  Executa(VTRedes *redes);
      void __fastcall  ExecutaLisMNetOrdenada(VTRedes *redes, TList *lisEXT_ORD, TList *lisEXT_ISO);

   private: //métodos
      VTCarga*      __fastcall CriaCargaDeReducao(void);
      bool          __fastcall CriaCurva(VTCarga *carga);
      bool          __fastcall CriaCurva(VTSuprimento *suprimento);
      VTReducao*    __fastcall CriaReducaoComCarga(VTMNet *mnet, VTCorte *corte);
      VTReducao*    __fastcall CriaReducaoComSuprimento(VTMNet *mnet, VTCorte *corte);
      VTSuprimento* __fastcall CriaSuprimentoDeReducao(void);
      void          __fastcall FundeMNetsDoCorte(TList *lisMNET, VTCorte *corte);
      VTMNet*       __fastcall ExisteMNetSoDeMRedeReduzida(TList *lisMR);
      VTMRede*      __fastcall ExisteMRedeDeGeracao(TList *lisORIG);
      bool          __fastcall ExisteReducaoComGeracao(TList *lisREDU);
      int           __fastcall IdentificaMalhaDeGeracao(VTMRede *mrede, TList *lisMREDE, TList *lisMRM);
      void          __fastcall IniciaCurvaCargaNula(VTCarga *carga);
      void          __fastcall IniciaCurvaSuprimento1pu(VTSuprimento *suprimento);
      void          __fastcall LimpaListas(void);
      void          __fastcall MontaMNetFluxo(void);
      void          __fastcall MontaMNetOrdenada(TList *lisEXT_ORD, TList *lisEXT_ISO);
      void          __fastcall MontaReducaoMNet(void);

   private: //dados externos
      VTApl       *apl;
      VTRedes     *redes;
      VTPatamares *patamares;

   private: //dados locais
      TList      *lisMNET;    //lista de objetos VTNet p/ cálculo parcial de fluxo
      TList      *lisMR;      //lista de objetos VTMRede
      TList      *lisMR_CAR;  //lista de objetos VTMRede de carga
      TList      *lisMR_GER;  //lista de objetos VTMRede de geração
      TList      *lisORD;     //lista ordenada
      TList      *lisTMP;     //lista de uso temporário
   };

//---------------------------------------------------------------------------
#endif
//eof

