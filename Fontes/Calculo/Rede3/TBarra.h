//---------------------------------------------------------------------------
#ifndef TBarraH
#define TBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBateriaC;
class TFase;
class TMedidor;
class TSup;
class VTEqpto;
class VTGeralC;
class VTMatriz;
class VTSisLin;

//---------------------------------------------------------------------------
class TBarra : public TObject
{
public:  // Dados
   int             id;                   // ID interno da barra (gerado pelo programa)
   VTEqpto         *eqpto;               // Ponteiro para equipamento SINAP
   AnsiString      codigo;               // C�digo da barra (definido pelo usu�rio)
   int             offset;               // Posi��o relativa da barra na lista (primeira = 0)
   int             tipo;                 // Tipo da barra (cf. enum_TIPO_BARRA)
   int             tipo_original;        // Para controlar mudan�as
   int             tipo_ater;            // Tipo do aterramento (atIsolado, atZ ou atSolido)
   int             tipo_zona;            // Tipo da zona (cf. enum_TIPO_ZONA_CALCULO em VTData.h)
   bool            mantida;              // Flag que indica se a barra ser� mantida ou n�o (Redutor3)
   bool            sup_equivalente;      // Flag que indica c�lculo de suprimento equivalente em barra mantida (s� tem sentido para mantida == true)
   bool            flag_conexa;          // Flag que indica se a barra � conexa (true) ou n�o (false)
   TFase           *fases;               // Objeto que armazena as fases da barra e as linhas em Ynodal
   double          vnom_faseA_kv;        // Tensao nominal de fase - fase A (kV)
   double          vnom_faseB_kv;        // Tensao nominal de fase - fase B (kV)
   double          vnom_faseC_kv;        // Tensao nominal de fase - fase C (kV)
   double          vnom_linha_kv;        // Tensao nominal de linha (kV)
   double          deseq_v_pu;           // Grau de desequil�brio da tens�o (pu)
   double          *ang_ref_rad;         // �ngulo de refer�ncia da zona da barra, POR FASE (rad: +/- 30�, etc.)
   double          *vet_qest_mvar;       // Pot�ncia reativa l�quida (Qcap-Qreat) (esYaterr, esYiso ou esTriang) (MVAr)
   complex<double> Ynodal0_pu;           // Admit�ncia NODAL, sequ�ncia zero   (redu��o de rede) (pu)
   complex<double> Ynodal1_pu;           // Admit�ncia NODAL, sequ�ncia direta (redu��o de rede) (pu)
   complex<double> yater_pu;             // Admit�ncia de aterramento do neutro (pu)
   complex<double> *vet_tensao_pu;       // Tens�o fase-terra na barra (em rel. � ref. global ; pu)
   complex<double> *vet_corr_a;          // Corrente de carga, incluindo aterramento (A)
   complex<double> *vet_scarga_mva;      // Pot�ncia complexa de carga por fase ap�s solu��o (MVA)
   complex<double> *admit_carga_CL_s;    // Admit�ncia das carga lineares ([S]; AN, BN, CN, AB, BC, CA)
   VTMatriz        *mat_carga_mva;       // Pot�ncia de carga para tensao 1|0 para cada liga��o/modelo de carga (MVA ; > 0 para carga)
   VTMatriz        *mat_ynodal_reduc_pu; // Matriz de admit�ncias nodais resultante de redu��o da rede (pu)
   TSup            *sup;                 // Ponteiro para suprimento (NULL se n�o for suprimento)
   TList           *lisLIG;              // Lista de liga��es da barra (classe TLigacaoC)
   TList           *lisBATERIA;          // Lista de baterias da barra (classe TBateriaC)

public:  // M�todos
                         __fastcall  TBarra(void);
                         __fastcall ~TBarra(void);
         void            __fastcall  AddBateria(TBateriaC *bateria);
         double          __fastcall  AngRefRadFseInt(int fase_int);
         void            __fastcall  CalculaAdmitanciasCargaLinear(void);
         bool            __fastcall  CalculaCorrentes(complex<double> *vet_tensoes,
                                                      complex<double> *vet_corr_pu,
                                                      VTGeralC        *geralC);
         void            __fastcall  CalculaGrauDesequilibrio(void);
         void            __fastcall  CalculaTensoesPU(complex<double> *vet_tensao_aux_pu,
                                                      AnsiString      *vet_cod_linha=NULL);
         bool            __fastcall  CargaNula(int lig);
         void            __fastcall  CargaTotal(complex<double> *vet_destino);
         void            __fastcall  CopiaAngRef(TBarra *bar_dest);
         void            __fastcall  CopiaCarga(int             lig,
                                                complex<double> *vet_destino);
         void            __fastcall  DefineAterramento(int             tipo_ater_novo,
                                                       VTGeralC        *geralC=NULL,
                                                       complex<double> zater_novo_ohm=complex<double>(0., 0.));
         void            __fastcall  DivideCarga(complex<double> *vet_carga,
                                                 double          fator);
         bool            __fastcall  InsereCarga(VTGeralC *geralC,
                                                 VTSisLin *Ynod);
         bool            __fastcall  InsereCL_H(int      ordemh,
                                                VTGeralC *geralC,
                                                VTSisLin *Ynod);
         bool            __fastcall  InsereEstaticoH(int      ordemh,
                                                     VTGeralC *geralC,
                                                     VTSisLin *Ynod);
         bool            __fastcall  InsereYrefReduc(VTSisLin *Ynod);
         bool            __fastcall  IsNoGeracao(int fase_int);
         bool            __fastcall  IsPV(void);
         bool            __fastcall  TipoCarga(void);
         bool            __fastcall  TipoGeracao(void);
         double          __fastcall  VnomFase_kv(int fase_int);
         double          __fastcall  VnomLinha_kv(void);
         void            __fastcall  Zera(void);
         void            __fastcall  ZeraCapacitorReator(void);
         void            __fastcall  ZeraCarga(void);

private:  // M�todos
         complex<double> __fastcall  AdmitanciaUmaFase_S(double          vnom_carga_kv,
                                                         complex<double> tensao_kv,
                                                         complex<double> *vet_carga_mva);
         bool            __fastcall  CalculaCorrentesCarga_pu(int             fase_int1,
                                                              int             fase_int2,
                                                              double          vnom_carga_kv,
                                                              complex<double> *vet_carga_mva,
                                                              complex<double> *corr1_pu,
                                                              complex<double> *corr2_pu,
                                                              VTGeralC        *geralC,
                                                              complex<double> *vet_tensoes=NULL);
};

#endif
//---------------------------------------------------------------------------

