//---------------------------------------------------------------------------
#ifndef TBalanceamentoH
#define TBalanceamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBalanceamento.h"

//---------------------------------------------------------------------------
class VTApl;
class VTDemanda;
class VTRede;
class VTCarga;
class VTEqpto;

//---------------------------------------------------------------------------
class TPotFase
   {
   public:
      __fastcall  TPotFase(int tag, AnsiString codigo) {this->tag = tag; this->codigo = codigo;}
      __fastcall ~TPotFase() {}

   public:
      void Inicia(void) {p = 0.; q = 0.;}

   public:
      int        tag;
      AnsiString codigo;
      double     p, q;    //MW, MVAr
   };

//---------------------------------------------------------------------------
class TBalanceamento : public VTBalanceamento
   {
   public:
             __fastcall  TBalanceamento(VTApl *apl_owner);
             __fastcall ~TBalanceamento(void);
      bool   __fastcall  AlteraRede(TList *lisCAR);
      bool   __fastcall  Executa(VTRede *rede);
      int    __fastcall  IndicePatamarPico(void);
      TList* __fastcall  LisCarga(void);
      TList* __fastcall  LisCloneCargaAlterada(void);
      TList* __fastcall  LisCloneCargaOriginal(void);

   private:
      bool       __fastcall AnalisaFase(VTCarga *carga, int fase_nova);
      bool       __fastcall CriaCargaAlterada(VTCarga *carga, int fase_nova);
      int        __fastcall DeterminaFasesCarga(VTCarga *carga);
      bool       __fastcall DistribuiCargaMonofasica(VTCarga *carga, int fases_carga);
      bool       __fastcall DistribuiCargaBifasica(VTCarga *carga, int fases_carga);
      bool       __fastcall DistribuiCargaTrifasica(VTCarga *carga, int fases_carga);
      bool       __fastcall DistribuiDemandaPorFase(VTCarga *carga, int fases_carga);
      double     __fastcall GetCarregamento(int tag);
      TPotFase*  __fastcall GetFase(int tag);
      TPotFase*  __fastcall GetFaseA(void);
      TPotFase*  __fastcall GetFaseB(void);
      TPotFase*  __fastcall GetFaseC(void);
      TPotFase*  __fastcall GetPotFase(int index);
      void       __fastcall Inicia(void);
      void       __fastcall LimpaLista(TList *lista);
      //AnsiString __fastcall MontaRelCarga(VTCarga *carga);
      AnsiString __fastcall MontaRelFinal(VTRede *rede);
      void       __fastcall OrdenaCargas(TList *lisCarga);
      void       __fastcall OrdenaFases(TList *listFase);
      int        __fastcall PatamarMaiorCarregamento(void);
      int        __fastcall PrimeiraFase(void);
      void       __fastcall RedefineCurvaCargaDefinidaPorFase(VTCarga *carga, int fase_nova);
      bool  	  __fastcall RedeValida(VTRede *rede);
		int        __fastcall SegundaFase(void);
		int        __fastcall TerceiraFase(void);
		AnsiString __fastcall TipoZona(VTRede *rede);

   private: //métodos static da classe usados em ordenação de listas
      static  int __fastcall ComparaCarregamento(void *carga1, void *carga2);
      static  int __fastcall ComparaCarregamentoPorFase(void *fase1, void *fase2);


   private: //dados locais
      TList        *lisEQP;
      TList        *lisCARGA;       //lista de todas Cargas da Rede
      TList        *lisCARGA_ALT;   //lista de Cargas alteradas
      TList        *lisCOPIA_ALT;   //lista de cópias das Cargas alteradas
      TList        *lisCOPIA_ORI;   //lista de cópias das Cargas originais que foram alteradas
      TList        *lisFase;
      //TStringList  *relat;

   private: //objetos externos
      VTApl *apl;

   private: //dados static usados em ordenação de listas
      static int        ind_pat_pico;
      static VTDemanda *demanda;
   };

//---------------------------------------------------------------------------
#endif
//eof
