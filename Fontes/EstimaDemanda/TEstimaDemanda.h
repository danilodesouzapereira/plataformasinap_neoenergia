//---------------------------------------------------------------------------
#ifndef TEstimaDemandaH
#define TEstimaDemandaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstimaDemanda.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCanal;
class VTCarga;
class VTCopia;
class VTEqpto;
class VTFlow;
class VTMedidor;
class VTPatamares;
class VTSuprimento;
class VTDemanda;
class VTFornecimento;
class VTLog;

//---------------------------------------------------------------------------
class TEstimaDemanda : public VTEstimaDemanda
   {
   public:
                __fastcall  TEstimaDemanda(VTApl *apl_owner);
                __fastcall ~TEstimaDemanda(void);
      bool      __fastcall  ExecutaPorRedePrimaria(VTRedes *redes);
      bool      __fastcall  ExecutaRedeUnica(VTRedes *redes);
      TStrings* __fastcall  StrLisRedeEstimada(void);
      TStrings* __fastcall  StrLisRedeNaoEstimada(void);

   protected:  //métodos acessados via property
      void __fastcall PM_SetHoraReferencia(int hora);

   private: //métodos
      bool       __fastcall  AtualizaDemandaCargaClonada(void);
      bool       __fastcall  AtualizaDemandaCargaOriginal(VTRedes *redes, VTRedes *redes_clone);
      bool       __fastcall  AtualizaDemandaCargaOriginal(VTCarga* carga_orig, VTCarga *carga_clone, int hora);
      bool       __fastcall  AtualizaDemandaCargaOriginal(VTCarga* carga, int hora, double fatcor=1.0);
      bool       __fastcall  AtualizaValorEstimadoMedidorOriginal(VTRedes *redes, VTRedes *redes_clone);
      double     __fastcall  CalculaFatorCorrecaoHoraReferencia(VTCarga* carga_orig, VTCarga *carga_clone);
      bool       __fastcall  ConverteCargaTipica(VTRedes *redes);
      bool       __fastcall  ConverteMedidores(void);
      void       __fastcall  DefineDemandaMinima(VTCarga *carga);
      bool       __fastcall  DesabilitaCanaisChaveFechadaMedicaoNula(VTRedes *redes_clone);
      bool       __fastcall  EstimacaoPodeSerFeita(VTRede *rede);
      bool       __fastcall  EstimacaoPodeSerFeita(VTRedes *redes);
      bool       __fastcall  ExecutaEstimacaoDemanda(VTRedes *redes, VTRedes *redes_clone);
      bool       __fastcall  ExecutaEstimacaoEstado(VTRedes *redes_clone);
      VTCarga*   __fastcall  ExisteCarga(TList *lisCAR, int carga_id);
      VTMedidor* __fastcall  ExisteMedidor(TList *lisMED, int medidor_id);
      VTCanal*   __fastcall  ExisteMedidorCanal(VTMedidor *medidor, int eqpto_id, int tipo_canal, int fases);
      void       __fastcall  FechaLog(void);
      void       __fastcall  IniciaLog(void);
      bool       __fastcall  InsereCurvaSuprimento(VTSuprimento *suprimento);
      bool       __fastcall  InsereMedidoresBarrasDeCarga(void);
      bool       __fastcall  InsereSuprimentoRedeClonada(VTRedes *redes_clone);
      void       __fastcall  Log(AnsiString txt, bool data=true);
      bool       __fastcall  MontaRedeEquivalente(void);
      bool       __fastcall  MontaRedeZIP(void);
      void       __fastcall  RetiraCanaisChaveAberta(VTRedes *redes_clone);
      bool       __fastcall  RetiraCanaisChaveFechadaMedicaoNula(VTRedes *redes_clone);
      void       __fastcall  RetiraCanaisDesabilitados(VTRedes *redes_clone);
      void       __fastcall  RetiraCargasIP(VTRedes *redes_clone);
      void       __fastcall  RetiraMedicaoI(void);
      void       __fastcall  SelecionaBarraChave(TList *lisEQP, int estado);
      void       __fastcall  SelecionaBarraEqbar(TList *lisEQP, int tipo_eqbar);
      void       __fastcall  SelecionaBarraEqptoComMedicao(TList *lisEQP);
      void       __fastcall  SelecionaBarraLigacao(TList *lisEQP, int tipo_ligacao);
      void       __fastcall  SelecionaBarraETD(TList *lisEXT);
      bool       __fastcall  ValidaDemandaCargasClonadas(VTRedes *redes_clone);
      // bool       __fastcall  TESTE_GravaArquivoCarga(AnsiString filename);

   private: //dados externos
      VTApl          *apl;
      VTDemanda      *demanda;
      VTFornecimento *fornecimento;

   private: //dados locais
      struct{
            TStrings *StrLisRede_OK;
            TStrings *StrLisRede_NOK;
            }diag;
      TList          *lisBAR;
      TList          *lisEQP;
      TList          *lisTMP;
      VTCopia        *copia;
      VTFlow         *flow;
      VTLog          *log;
	  VTPatamares    *patamares;
   };

#endif
//---------------------------------------------------------------------------
//eof

