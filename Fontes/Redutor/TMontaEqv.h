//---------------------------------------------------------------------------
#ifndef TMontaEqvH
#define TMontaEqvH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Constante\Fases.h"
#include "VTMontaEqv.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTNo;
class VTEqv;
class VTPatamares;
class VTDemanda;
class VTFornecimento;
class VTTrafo;

//---------------------------------------------------------------------------
class TMontaEqv : public VTMontaEqv
   {
   public:
             __fastcall  TMontaEqv(VTApl *apl);
             __fastcall ~TMontaEqv(void);
      void   __fastcall  DefineModeloCarga(double Icte,
                                           double Scte,
                                           double Zcte);
      void   __fastcall  DefineModeloEqv(bool equilibrado);
      bool   __fastcall  Executa(VTNet *net, TList *lisLIG, TList *lisBAR,
                                 TList *lisBAR_COR, TList *lisBAR_GER, int ano_carga,
                                 double Sbase, bool aviso_enabled);
      TList* __fastcall  LisCargaEquivalente(void);
      TList* __fastcall  LisTrafoEquivalente(void);
      TList* __fastcall  LisTrechoEquivalente(void);
      TList* __fastcall  LisYrefEquivalente(void);

   private: //métodos
      bool     __fastcall CriaCurva(VTCarga *pcarga);
      void     __fastcall DefineNoRedutivel(TList *lisNO, bool  redutivel);
      VTBarra* __fastcall ExisteBarra(TList *lisNO, int bar_id);
      VTCarga* __fastcall ExisteCarga(VTBarra *pbarra);
      bool     __fastcall EqvAtualizaTensoes(int np);
      bool     __fastcall EqvResBarra(int np);
      bool     __fastcall EqvResCarga(VTNo *pno, int  np, complex<double> sf_kva[]);
      bool     __fastcall EqvResLigacao(void);
      bool     __fastcall EqvResYref(VTNo            *pno,
                                     complex<double> y0_pu,
                                     complex<double> y1_pu,
												 complex<double> maty_pu[MAX_FASE][MAX_FASE]);
      bool     __fastcall EqvZeraCarga(void);
      void     __fastcall IniciaLisGrupoMutua(TList *lisLIG);
      bool     __fastcall IniciaLisNo(TList *lisBAR, TList *lisNO);
	  bool     __fastcall MatrizAdmitanciaNula(VTTrafo *trafo);
	  void     __fastcall RemoveLigDaBarra(TList *lisLIG);
      int      __fastcall SentidoFluxo(double fluxo);

   private: //objetos externos
      VTApl          *apl;
      VTPatamares    *patamares;
      VTDemanda      *demanda;
      VTFornecimento *fornecimento;
      VTNet          *net;

   private: //objetos locais
      VTEqv    *eqv;
      TList    *lisCAR;
      TList    *lisSUP;
      TList    *lisTRE;
      TList    *lisTRF;
      TList    *lisYREF;
      TList    *lisGRUPO;  //lista de Grupos de Mutuas
      TList    *lisEQP;
      TList    *lisNO;     //lista de Nos
      TList    *lisNO_COR; //lista de Nos do Corte
      TList    *lisNO_GER; //lista de Nos de geração
      //dados locais
      int     ano;
      //int     modelo_eqv;  // Eqv1: 1 ; Eqv3: 3
      double  Sbase;
      bool    aviso_enabled;
      struct {
             double Icte;
             double Scte;
             double Zcte;
             } mod_carga;
   };

#endif
//---------------------------------------------------------------------------
//eof
