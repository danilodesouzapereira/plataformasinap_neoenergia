//---------------------------------------------------------------------------
#ifndef TReduzRedeH
#define TReduzRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReduz.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTCorte;
class VTGerador;
class VTLigacao;
class VTNet;
class VTNo;
class VTMonta;
class VTMontaEqv;
class VTOrdena;
class VTPatamares;
class VTReducao;
class VTSuprimento;

//---------------------------------------------------------------------------
class TReduzRede : public VTReduz
   {
   public:
                 __fastcall  TReduzRede(VTApl *apl);
                 __fastcall ~TReduzRede(void);
      VTReducao* __fastcall  Executa(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, bool aviso_enabled);
      bool       __fastcall  Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled);
      bool       __fastcall  Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled);

   private: //métodos
      bool          __fastcall CargaDesprezivel(VTCarga* pcarga);
      bool          __fastcall CorteIsolado(VTCorte *corte);
      bool          __fastcall CriaCurva(VTCarga *pcarga);
      bool          __fastcall CriaCurva(VTSuprimento *psup);
      bool          __fastcall ExisteLigacaoConectandoBarras(TList *lisLIG, VTBarra *barra1, VTBarra *barra2);
      bool          __fastcall ExisteDesequilibrio(void);
      void          __fastcall IniciaListasParteRedutivel(VTCorte *corte, TList *lisMR, TList *lisREDU);
      void          __fastcall IniciaLisCarga(TList *lisMR, TList *lisCAR);
      void          __fastcall IniciaLisCorte(VTMNet *mnet, TList *lisCOR);
      void          __fastcall InsereCarga(VTReducao *reducao, TList *lisBAR_GER);
      void          __fastcall InsereCargaEqv(VTReducao *reducao, TList *lisCAR);
      void          __fastcall InsereChaveEqv(VTReducao *reducao);
      void          __fastcall InsereSuprimento(VTReducao *reducao, VTCorte *corte);
      VTSuprimento* __fastcall InsereSuprimentoEqv(VTGerador* pger);
      VTSuprimento* __fastcall InsereSuprimentoEqv(VTSuprimento* psup);
      void          __fastcall InsereTrafoEqv(VTReducao *reducao, TList *lisTRF);
      void          __fastcall InsereTrechoEqv(VTReducao *reducao, TList *lisTRE);
      void          __fastcall InsereYrefEqv(VTReducao *reducao, TList *lisYREF);
      bool          __fastcall MontaCrescimentoEquivalente(VTReducao *reducao, TList *lisCAR);
      //bool          __fastcall MontaModeloCargaEquivalente(VTReducao *reducao, TList *lisCAR);
      bool          __fastcall MontaModeloCargaEquivalente(TList *lisCAR);
      bool          __fastcall MontaRedeEquivalente(bool aviso_enabled);
      VTReducao*    __fastcall MontaReducao(VTCorte *corte);
      VTReducao*    __fastcall MontaReducaoInvalida(void);
      bool          __fastcall RedefineBarraPaiNoDeCorte(VTCorte *corte);
      void          __fastcall SeparaParteRedutivel(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, TList *lisMR, TList *lisREDU);
      void          __fastcall SomaCargasDoNoNaCargaEquivalente(VTCarga *carga_eqv, VTNo *no);
      void          __fastcall TrataBarrasCorteConectadasPorChave(void);

   private: //dados externos
      VTApl       *apl;
      VTPatamares *patamares;

   private: //dados locais
      int        num_ger_sup;
      TList      *lisBAR;
      TList      *lisMR;      //lista de MRedes da parte redutível
      TList      *lisREDU;    //lista de Reducoes de MNet  da parte redutível
      TList      *lisCORTE;
      TList      *lisCAR;     //lista de Cargas
      TList      *lisTMP;     //lista de uso interno
      struct   {
               TList *lisLIG_ALL; //lista de Ligacoes da parte redutível
               TList *lisBAR_ALL; //lista de Barras da parte redutível
               TList *lisBAR_COR; //lista de Barras do Corte
               TList *lisBAR_GER; //lista de Barras de Geradores e Suprimentos da parte redutível
               TList *lisSUP_GER; //lista de Suprimentos e Geradores da parte redutível
               TList *lisMUTUA;   //lista de Mútuas dos trechos da parte redutível
               TList *lisCAR;     //lista de Cargas da parte redutível
               struct   {
                        double Icte_pu;
                        double Scte_pu;
                        double Zcte_pu;
                        } mod_carga;
               }redutivel;
      VTMontaEqv *monta_eqv;
      VTMonta    *monta;
      VTNet      *net;
      VTOrdena   *ordena;
   };

//---------------------------------------------------------------------------
#endif
//eof

