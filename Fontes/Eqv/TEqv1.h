//---------------------------------------------------------------------------
#ifndef TEqv1H
#define TEqv1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include ".\VTEqv.h"

//---------------------------------------------------------------------------
class  TImpressao1;
class  VTApl;
class  VTData;
class  VTGeralC;
class  VTMonta1;
class  VTRede1;
class  VTSisLin;

//---------------------------------------------------------------------------
class TEqv1 : public VTEqv
   {
   public:  // Métodos
           __fastcall  TEqv1(VTApl *apl);
           __fastcall ~TEqv1(void);
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
      TImpressao1 *impr1;
      VTApl       *apl;
      VTNet       *net;
      VTGeralC    *geralC;
      VTMonta1    *monta1;
      VTRede1     *rede1;

   private:  // Objetos locais
      VTData *data;
      TList  *lisBAR_REDUC;     // Lista de barras  na rede reduzida
      TList  *lisTRECHO_REDUC;  // Lista de trechos na rede reduzida

      // Dados locais
      double          grau_rad, raiz3, raiz3_inverso;
      complex<double> cum, czero;
      int             ano;
      complex<double> *vet_tensoes;
      VTSisLin        *Ynod_red0;  // Seq. zero
      VTSisLin        *Ynod_red1;  // Seq. direta
      struct {
             double Icte;
             double Scte;
             double Zcte;
             } mod_carga;

   private:  // Métodos
      bool __fastcall AlocaMemoria(void);
      void __fastcall CalculaFracoesCarga(complex<double> *scarga,
                                          double          *fracoes);
      void __fastcall CalculaYpYm(complex<double> Ynodal0,
                                  complex<double> Ynodal1,
                                  complex<double> *ydiag,
                                  complex<double> *yfora);
      void __fastcall DeletaLista(TList **lista);
      bool __fastcall MontaListaBarras(void);
      bool __fastcall MontaListaLigacoes(void);
      bool __fastcall SeparaBarras(void);
   };

#endif
//---------------------------------------------------------------------------
//eof
