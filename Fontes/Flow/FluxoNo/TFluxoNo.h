//---------------------------------------------------------------------------
#ifndef TFluxoNoH
#define TFluxoNoH
using namespace std;


//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "VTFluxoNo.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCapacitor;
class VTCarga;
class VTLigacao;
class VTDemanda;
class VTPatamares;
class VTGerador;
class VTLigacao;
class VTNo;
class VTReator;
class VTSuprimento;
class VTYref;

//---------------------------------------------------------------------------
class TFluxoNo : public VTFluxoNo
   {
   public:
            __fastcall  TFluxoNo(VTApl *apl);
            __fastcall ~TFluxoNo(void);
       bool __fastcall  Executa(VTNet *net, double Sbase);

   private: //métodos
      void     __fastcall Executa(VTNo *pno);
      void     __fastcall ExecutaFluxoLigacao(VTBarra *pbarra, VTLigacao *pchave, TList *lisEQP);
      bool     __fastcall ExecutaFluxoLigacoesParalelas(VTBarra *pbarra, TList *lisPAR, TList *lisEQP);
      bool     __fastcall ExisteBarraComLigacoesParalelas(VTBarra **pbarra, TList *lisDEST);
      bool     __fastcall ExisteBarraComApenasUmaLigacao(VTNo *pno, VTBarra **pbarra, VTLigacao **pligacao);
      void     __fastcall ReiniciaDadosFlow(TList *lisLIG);
      void     __fastcall SelecionaEqbarBarra(VTBarra *pbarra, TList *lisORIG, TList *lisDEST);
      void     __fastcall SelecionaLigacoesBarra(VTBarra *pbarra, TList *lisORIG, TList *lisDEST);
      int      __fastcall SentidoFluxo(double fluxo);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTCapacitor *pcap);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTCarga *pcar);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTGerador *pger);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTLigacao *pliga);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTSuprimento *psup);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTReator *reator);
      void     __fastcall SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTYref *yref);
      void     __fastcall TensoesBarras(VTNo *pno);

   private: //objetos externos
      VTApl       *apl;
      VTDemanda   *demanda;
      VTPatamares *patamares;

   private: //dados
      double   Sbase;
      VTNet    *net;
      TList    *lisBAR;    //lista de Barras de um No
      TList    *lisLIG;    //lista de Ligacoes de um No
      TList    *lisPAR;    //lista de Ligacoes paralelas ligadas em um mesma barra
      TList    *lisEQB;    //lista de Eqbar de uma Barra ou No
      TList    *lisTMP;    //lista p/ uso temporário dentro de uma função
   };

#endif
//---------------------------------------------------------------------------
//eof

 