//---------------------------------------------------------------------------
#ifndef TReduzSuprimentoH
#define TReduzSuprimentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReduz.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCorte;
class VTEqbar;
class VTPatamares;
class VTMRede;
class VTRedes;
class VTReducao;
class VTReduz;
class VTSuprimento;

//---------------------------------------------------------------------------
class TReduzSuprimento : public VTReduz
   {
   public:
                 __fastcall  TReduzSuprimento(VTApl *apl);
                 __fastcall ~TReduzSuprimento(void);
      VTReducao* __fastcall  Executa(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, bool aviso_enabled);
      bool       __fastcall  Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled);
      bool       __fastcall  Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled);

   private: //métodos
      bool       __fastcall CorteIsolado(VTCorte *corte);
      bool       __fastcall CriaCurva(VTSuprimento *psup);
      void       __fastcall ExisteBarraInterface(VTMRede *mrede, VTCorte *corte, TList *lisEQP);
      void       __fastcall ExisteLigacao(VTMRede *mrede, VTBarra *pbarra, TList *lisEQP);
      VTReducao* __fastcall MontaReducaoInvalida(void);
      VTReducao* __fastcall MontaReducaoTipoSuprimento(VTMRede *mrede, VTCorte *corte);
      VTEqbar*   __fastcall MontaSuprimentoEquivalente(VTMRede *rede, VTBarra *pbarra);

   private: //dados externos
      VTApl       *apl;
      VTPatamares *patamares;

   private: //dados locais
      TList *lisBAR;
      TList *lisLIG;
      TList *lisMRV;
      TList *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof

