//---------------------------------------------------------------------------
#ifndef TReduzBarraH
#define TReduzBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReduz.h"

//---------------------------------------------------------------------------
class VTCarga;
class VTGerador;
class VTLigacao;
class VTNet;
class VTMontaEqv;
class VTOrdena;
class VTPatamares;
class VTRede;
class VTSuprimento;
class VTYref;

//---------------------------------------------------------------------------
class TReduzBarra : public VTReduz
   {
   public:
                 __fastcall  TReduzBarra(VTApl *apl);
                 __fastcall ~TReduzBarra(void);
      VTReducao* __fastcall  Executa(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, bool aviso_enabled);
      bool       __fastcall  Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled);
      bool       __fastcall  Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled);

   private: //métodos
      bool          __fastcall CargaDesprezivel(VTCarga* pcarga);
      bool          __fastcall CriaCurva(VTCarga *pcarga);
      bool          __fastcall CriaCurva(VTSuprimento *psup);
      bool          __fastcall ExisteDesequilibrio(void);
      bool          __fastcall ExisteDesequilibrio(VTRede *rede);
      void          __fastcall IniciaLisLigacaoMantida(void);
      void          __fastcall IniciaListasParteRedutivel(TList *lisBAR);
      void          __fastcall InsereBarraDeFronteira(VTRede *rede, TList *lisBAR);
      void          __fastcall InsereBarraInicial(VTRede *rede, TList *lisBAR);
      void          __fastcall InsereCargaEqv(TList *lisCAR);
      void          __fastcall InsereTrafoEqv(VTRede *rede, TList *lisTRF);
      void          __fastcall InsereTrechoEqv(VTRede *rede, TList *lisTRE);
      void          __fastcall InsereYrefEqv(TList *lisYREF);
      bool          __fastcall LigacaoSeraExcluida(VTLigacao *ligacao);
      bool          __fastcall MontaCrescimentoEquivalente(void);
      void          __fastcall MontaModeloCargaEquivalente(TList *lisCAR);
      bool          __fastcall MontaRedeEquivalente(bool _aviso_enabled);
      void          __fastcall RedefineBarraPaiNo(TList *lisBAR_MAN);
      void          __fastcall ReiniciaRede(VTRede *rede);
      void          __fastcall ReiniciaRede(VTRedes *redes);
      bool          __fastcall RetiraTodasRedes(VTRedes *redes);
      bool          __fastcall TransfereTrechoRedeOrigRedeEqv(VTRedes *redes, VTRede *rede_eqv);
      bool          __fastcall YrefDesprezivel(VTYref *yref);

   private: //dados externos
      VTApl       *apl;
      VTPatamares *patamares;

   private: //dados locais
      VTOrdena   *ordena;
      VTMontaEqv *monta_eqv;
      VTNet      *net;
      TList      *lisTMP;     //lista de uso interno
      struct   {
               TList *lisLIG_ALL; //lista de Ligacoes da parte redutível
               TList *lisBAR_ALL; //lista de Barras da parte redutível
               TList *lisBAR_COR; //lista de Barras do Corte da parte redutível
               TList *lisBAR_EXC; //lista de Barras que serão eliminadas
               TList *lisBAR_GER; //lista de Barras com geração
               TList *lisSUP_GER; //lista de Suprimentos e Geradores da parte redutível
               TList *lisMUTUA;   //lista de Mútuas da parte redutível
               TList *lisCAR;     //lista de Cargas da parte redutível
               TList *lisREDE;    //lista de Redes afetadas com pelo menos 1 ligacao na parte redutivel
               struct {
                      double  Icte_pu, Scte_pu, Zcte_pu;
                      }mod_carga;
               }redutivel;
   };

//---------------------------------------------------------------------------
#endif
//eof

