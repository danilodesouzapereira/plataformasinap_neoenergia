//---------------------------------------------------------------------------
#ifndef TEqv3H
#define TEqv3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include ".\VTEqv.h"

//---------------------------------------------------------------------------
class TImpressao3;
class TBarra;
class TFase;
class TLigacaoEquiv;
class TTrecho;
class VTApl;
class VTData;
class VTGeralC;
class VTMatriz;
class VTMonta3;
class VTRede3;
class VTSisLin;

//---------------------------------------------------------------------------
class TEqv3 : public VTEqv
   {
   public:  // Métodos
           __fastcall  TEqv3(VTApl *apl);
           __fastcall ~TEqv3(void);
      bool __fastcall  AtualizaTensao(int             bar_id,
                                      complex<double> *vet_tensao_pu);
      bool __fastcall  CalculaNovasCargas(void);
      void __fastcall  DefineModeloCarga(double Icte,
                                         double Scte,
                                         double Zcte);
      bool __fastcall  DeterminaReducao(void);
      bool __fastcall  ImprimeRedeReduzida(int np);
      bool __fastcall  MontaRedeOriginal(VTNet *net);
      bool __fastcall  ReduzRede(void);
      bool __fastcall  ResBarraAdmit(strResBarraAdmit *res_barra);
      bool __fastcall  ResBarraCargaTotal(int             bar_id,
                                          complex<double> *vet_carga_kVA);
      bool __fastcall  ResBarraDeltaCarga(int             bar_id,
                                          complex<double> *vet_carga_kVA);
      bool __fastcall  ResLigacao(strResLigacaoAdmit *res_ligacao);
      int  __fastcall  ResLigacaoCount(void);

   protected:  //métodos acessados via property
      int  __fastcall PM_GetModelo(void);

   private: //objetos externos
      TImpressao3 *impr3;
      VTApl       *apl;
      VTNet       *net;
      VTGeralC    *geralC;
      VTMonta3    *monta3;
      VTRede3     *rede3;

   private:  // Objetos locais
      VTData   *data;
      TList    *lisBAR_REDUC;      // Lista de barras  na rede reduzida
      TList    *lisLIGACAO_REDUC;  // Lista de trechos na rede reduzida
      VTMatriz *mat_ij;            // Usada em CriaLigacaoEquivalente()
      VTMatriz *mat_ji;            // Usada em CriaLigacaoEquivalente()

      // Dados locais
      double          raiz3, raiz3_inverso;
      complex<double> calfa, calfa2, cum, czero, zerov;
      int             ano;
      complex<double> *vet_tensoes;
      VTSisLin        *Ynod_red;
      struct {
             double Icte;
             double Scte;
             double Zcte;
             } mod_carga;

   private:  // Métodos
      bool            __fastcall AlocaMemoria(void);
      void            __fastcall CalculaFracoesCarga(complex<double> *scarga,
                                                     double          *fracoes);
     complex<double>  __fastcall CompSeqDireta(complex<double> va,
                                               complex<double> vb,
                                               complex<double> vc);
     complex<double>  __fastcall CompSeqZero(complex<double> va,
                                             complex<double> vb,
                                             complex<double> vc);
      TLigacaoEquiv * __fastcall CriaLigacaoEquivalente(TBarra *bar1,
                                                        TBarra *bar2);
      void            __fastcall DeletaLista(TList **lista);
      bool            __fastcall InicializaAdmitReducBarra(TBarra *bar1,
                                                           TBarra *bar2);
      bool            __fastcall MontaListaBarras(void);
      bool            __fastcall MontaListaLigacoes(void);
      bool            __fastcall SeparaBarras(void);
   };

#endif
//---------------------------------------------------------------------------
//eof
