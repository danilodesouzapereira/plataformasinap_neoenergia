//---------------------------------------------------------------------------
#ifndef TReduzCargaH
#define TReduzCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReduz.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTCorte;
class VTLigacao;
class VTMRede;
class VTNet;
class VTNo;
class VTRedes;
class VTReducao;
class VTPatamares;
class VTReduz;

//---------------------------------------------------------------------------
class TReduzCarga : public VTReduz
   {
   public:
                 __fastcall  TReduzCarga(VTApl *apl);
                 __fastcall ~TReduzCarga(void);
      VTReducao* __fastcall  Executa(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, bool aviso_enabled);
      bool       __fastcall  Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled);
      bool       __fastcall  Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled);

   private: //métodos
      bool       __fastcall CorteIsolado(VTCorte *corte);
      bool       __fastcall CriaCurva(VTCarga *pcarga);
      void       __fastcall DefineNumeroConsumidoresDaCargaEquivalente(VTCarga *carga_eqv, TList *lisCAR);
      void       __fastcall ExisteBarraInterface(VTMRede *mrede, VTCorte *corte, TList *lisEQP);
      void       __fastcall ExisteLigacao(VTMRede *mrede, VTBarra *pbarra, TList *lisEQP);
      void       __fastcall IniciaLisCargaParteRedutivel(VTMRede *mrede, VTCorte *corte, TList *lisCAR);
      int        __fastcall LigacaoConectaNo(VTLigacao* ligacao, VTNo *no);
      VTCarga*   __fastcall MontaCargaEquivalente(VTMRede *mrede, VTBarra *pbarra);
      bool       __fastcall MontaCrescimentoEquivalente(VTReducao *reducao, TList *lisCAR);
      bool       __fastcall MontaModeloCargaEquivalente(VTCarga *carga_eqv, TList *lisCAR);
      VTReducao* __fastcall MontaReducaoInvalida(void);
      VTReducao* __fastcall MontaReducaoTipoCarga(VTMRede *mrede, VTCorte *corte);

   private: //dados externos
      VTApl       *apl;
      VTPatamares *patamares;

   private: //dados locais
      VTNet *net;
      TList *lisBAR;
      TList *lisCAR;
      TList *lisLIG;
      TList *lisMRV;
      TList *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof

