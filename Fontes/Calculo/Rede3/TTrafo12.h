//---------------------------------------------------------------------------
#ifndef TTrafo12H
#define TTrafo12H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG3.h"
#include <complex>
using namespace std;
class TBancoDF;
class TBarra;
class TFase;
// class TTrafo12Radial;
class VTEqpto;
class VTGeralC;
class VTMatriz;
class VTRede3;
class VTSisLin;

//---------------------------------------------------------------------------
class TTrafo12 : public VTLigacaoG3
{
public:  // Properties
   __property int NumTaps = {read=PM_GetNumTaps, write=PM_SetNumTaps};

protected:  // Métodos acessados via property
   int  __fastcall PM_GetNumTaps(void);
	void __fastcall PM_SetNumTaps(int num_taps);

public:  // Dados locais
   bool            flag_reg12;      // false para TTrafo12 ou true para TReg12
   int             id;              // ID interno do trafo (gerado pelo programa)
   VTEqpto         *eqpto;          // Ponteiro para equipamento SINAP
   AnsiString      codigo;          // Codigo do trafo
   int             id_bar1_orig;    // ID da barra "de" no SINAP (sentido do fluxo)
	int             tipo;            // Tipo do trafo (cf. 'enum_TIPO_TRAFO12' em VTGeralC.h)
	int             tipo_ater1;      // Tipo do aterramento do primário   (cf.'enum_TIPO_ATERR' em VTGeralC.h)
	int             tipo_ater2;      // Tipo do aterramento do secundário (cf.'enum_TIPO_ATERR' em VTGeralC.h)
   int             fase_comum;      // Fase comum (para uso com AUTOTRAFO4 e AUTOTRAFO6)
   int             num_linhas_Y;    // Número de linhas/colunas em Ynodal
   int             *vet_fases_ord;  // Vetor de fases ordenadas
   double          vnom1_kv;        // Tensão nominal do primário   (kV)
   double          vnom2_kv;        // Tensão nominal do secundário (kV)
   double          snom_mva;        // Potência nominal do trafo (MVA)
   double          perda_energia_diaria_kwh; // Perda diária em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda diária em energia (parcela de perdas no ferro) (kWh)
	double          vet_tap[2];      // Tap 'a' do trafo (Vtap2/Vtap1 em kV/kV) (modelo 1:a). O segundo tap só é usado em AUTOTRAFO6_2T.
	complex<double> ztrafo_ohm;      // Impedância do transformador (ohm ref. ao primário ou secundário, dependendo do tipo)
   complex<double> ypar_S;          // Admitância do ramo paralelo (S ref. ao primário ou secundário, dependendo do tipo)
   complex<double> yater1_pu;       // Admitância de aterramento do primário (pu)
   complex<double> yater2_pu;       // Admitância de aterramento do secundário (pu)
   complex<double> perda_kva;       // Perda complexa no trafo (kVA)
   complex<double> *vet_corr_a;     // Corrente nas pernas do trafo (A)
   complex<double> *vet_s_kva;      // Potência nas pernas do trafo (kVA)
   TBarra          *bar1;           // Ponteiro para a barra "de" na lista de barras
   TBarra          *bar2;           // Ponteiro para a barra "para" na lista de barras
   TFase           *fases_pri;      // Fases do primário
   TFase           *fases_sec;      // Fases do secundário
   TBancoDF        *bancoDF;        // Ponteiro para o banco Delta-Fechado do trafo (NULL se não for parte de um banco DF)
   VTMatriz        *Ynodal_pu;      // Matriz Ynodal do trafo (pu)

public:  // Métodos
			    __fastcall  TTrafo12(VTGeralC *geralC,
                                  TBarra   *bar1,
                                  TBarra   *bar2,
                                  int      fases_int_pri,
                                  int      fases_int_sec);
			    __fastcall ~TTrafo12(void);
        void __fastcall  AnalisaAterramento(bool    flag_ater_comuns,
                                            VTRede3 *rede3);
        int  __fastcall  Barras(TBarra **bar1,
                                TBarra **bar2,
                                TBarra **bar3);
        void __fastcall  Calcula(int      np,
                                 VTGeralC *geralC);
        void __fastcall  CompatibilizaAterramentos(void);
        void __fastcall  DefineAterramento(bool            flagT_pri,
                                           bool            flagT_sec,
                                           complex<double> zater1_ohm,
                                           complex<double> zater2_ohm,
                                           VTGeralC        *geralC);
        void __fastcall  FasesOrdenadas(int enrolamento,
                                        int *f1,
                                        int *f2,
                                        int *f3=NULL);
        int  __fastcall  IndiceGlobal(int indice_local);
        bool __fastcall  InsereYnodal(int      ordemh,
                                      VTGeralC *geralC,
                                      VTSisLin *Ynod,
                                      bool     flag_zpequena=false);
        // A função abaixo é pública porque é usada pela classe TBancoDF
        bool __fastcall  MontaYnodal(int      ordemh,
                                     VTGeralC *geralC,
                                      bool     flag_zpequena);
        bool __fastcall  NeutroPri(void);
        bool __fastcall  NeutroSec(void);
        int  __fastcall  NumBarras(void);
        int  __fastcall  TipoLigacao(void);
        void __fastcall  TrocaBarras(void);
		  void __fastcall  ZeraPerdaEnergia(void);

private:  // Dados
	int num_taps;  // Número de fases com tap (todos os tipos têm 1 tap só,
                  // exceto AUTOTRAFO6_2T que tem 2 taps)

private:  // Métodos
        void __fastcall AcumulaPerdas(int      np,
                                      double   pfe_kw,
                                      VTGeralC *geralC);
        void __fastcall AdicionaAdmitanciaPequena_TRAFO4(void);
        void __fastcall Calcula_AUTOTRAFO4(int      np,
                                           VTGeralC *geralC);
        void __fastcall Calcula_AUTOTRAFO6(int      np,
                                           VTGeralC *geralC);
        void __fastcall Calcula_TRAFO4(int      np,
                                       VTGeralC *geralC);
        void __fastcall Calcula_TRAFO5(int      np,
                                       VTGeralC *geralC);
        void __fastcall CopiaTriangSupTriangInf(void);
        bool __fastcall DefineNumLinhasY(void);
        bool __fastcall MontaYnodal_AUTOTRAFO4(int      ordemh,
                                               VTGeralC *geralC,
                                               bool     flag_zpequena);
        bool __fastcall MontaYnodal_AUTOTRAFO6(int      ordemh,
                                               VTGeralC *geralC,
                                               bool     flag_zpequena);
        bool __fastcall MontaYnodal_TRAFO4(int      ordemh,
                                           VTGeralC *geralC,
                                           bool     flag_zpequena);
        bool __fastcall MontaYnodal_TRAFO5(int      ordemh,
                                           VTGeralC *geralC,
                                           bool     flag_zpequena);
        bool __fastcall MontaYnodal_TRAFO5_Como_Trafo3E(int      ordemh,
                                                        VTGeralC *geralC,
                                                        bool     flag_zpequena);

};

#endif
//---------------------------------------------------------------------------

