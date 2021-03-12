//---------------------------------------------------------------------------
#ifndef TRedutorH
#define TRedutorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <time.h>
#include "VTRedutor.h"

//---------------------------------------------------------------------------
class VTCorte;
class VTMNet;
class VTMRede;
class VTRedes;
class VTReduz;
class VTProgresso;

//---------------------------------------------------------------------------
class TRedutor : public VTRedutor
   {
   public:
           __fastcall  TRedutor(VTApl *apl_owner);
           __fastcall ~TRedutor(void);
      bool __fastcall  ExecutaReducaoMNet(void);
      bool __fastcall  ExecutaReducaoAllRede(TList *lisBAR, bool calcular_fluxo);
      bool __fastcall  ExecutaReducaoUmaRede(VTRede *rede, TList *lisBAR, bool calcular_fluxo);

   protected:  //métodos acessados via property
      int   __fastcall PM_GetModeloEqv(void);
      void __fastcall PM_SetAvisoEnabled(bool enabled);
      void __fastcall PM_SetModeloEqv(int modelo_eqv);
      void __fastcall PM_SetProgressoEnabled(bool enabled);

   private: //métodos
      void     __fastcall AtualizaProgresso(AnsiString texto);
      bool     __fastcall ExecutaCalculoFluxo(void);
      VTMRede* __fastcall ExisteMRedeComGeracao(TList *lisMR);
      void     __fastcall HabilitaReducaoTodasMRede(void);
      bool     __fastcall HaNecessidadeDeFazerReducao(void);
      bool     __fastcall HaNecessidadeDeFazerReducao(VTMRede *mrede);
      void     __fastcall ListaOrdenadaCorteMNetCarga(VTMNet *mnet, TList *lisCORTE);
      void     __fastcall ListaOrdenadaCorteMNetGeracao(VTMNet *mnet, TList *lisCORTE);
      void     __fastcall LogReducao(VTMRede *mrede);
      bool     __fastcall MontaReducao(void);
      bool     __fastcall MontaReducaoMNetCarga(VTMNet *mnet);
      bool     __fastcall MontaReducaoMNetGeracao(VTMNet *mnet);
      bool     __fastcall MontaReducaoMNetGeracaoExcecao(VTMNet *mnet);
      bool     __fastcall MontaReducaoMRedeCarga(VTMNet *mnet, VTMRede *mrede, VTCorte *corte);
      bool     __fastcall MontaReducaoMRedeGeracao(VTMNet *mnet, VTMRede *mrede, VTCorte *corte);
      bool     __fastcall RedeVizinhaDoTipoSecundaria(VTMRede *mrede, VTCorte *corte);
      void     __fastcall ReiniciaReducaoMRede(VTRedes *redes);
      bool     __fastcall VerificaExcecaoMNetGeracao(VTMNet *mnet);

   private: //dados externos
      VTApl   *apl;

   private: //dados locais
      VTProgresso *progresso;
      VTReduz     *reduz_barra;
      VTReduz     *reduz_carga;
      VTReduz     *reduz_rede;
      VTReduz     *reduz_suprimento;
      struct   {
               TList *lisEQP;
               TList *lisCOR;
               TList *lisMR;
               }local;
      struct   {//dados acessados via property
               bool aviso_enabled;
               bool progresso_enabled;
               int  modelo_eqv;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

