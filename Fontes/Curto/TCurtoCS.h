
//---------------------------------------------------------------------------
#ifndef TCurtoCSH
#define TCurtoCSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include <complex>
using namespace std;
#include ".\VTCurtoCS.h"
#include "..\Rede\VTLigacao.h"
#include "..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------
class  TBarra;
class  TImpressao1;
class  TTrafo2;
class  TTrecho;
class  VTAtualiza1;
class  VTCalculo1;
class  VTCurtoAux;
class  VTGeralC;
class  VTMonta1;
class  VTRede1;
class  VTResultados1;
class  VTSisLin;
class  VTDemanda;

//---------------------------------------------------------------------------
class TCurtoCS : public VTCurtoCS
{
public:
        __fastcall  TCurtoCS(VTApl *apl);
        __fastcall ~TCurtoCS(void);
   bool __fastcall  Curto_3F_Max(VTData *data,
                                 int    *id_barra_sinap,
                                 double *idef_3F_ka);
   bool __fastcall  Executa1(int      ind_pat,
                             VTData   *data,
                             VTBarra  *barra,
                             VTNet    *net,
                             VTRedeCC *redeCC,
                             VTTrecho *trecho,
                             VTTrecho *trecho_fic1,
                             VTTrecho *trecho_fic2);
   bool __fastcall  Executa2(int     tipo_def,
                             strIMP  zdef_fase_ohm,
                             strIMP  zdef_terra_ohm,
                             VTBarra *barra);
   bool __fastcall  Executa3(int      tipo_def,
                             strIMP   zdef_fase_ohm,
                             strIMP   zdef_terra_ohm,
                             VTTrecho *trecho,
                             VTBarra  *barra_ref,
                             double   dist_m);
   bool __fastcall  Iamp(VTLigacao       *ligacao,
                         complex<double> iamp[MAX_FASE+MAX_FASE]);
   bool __fastcall  IampBarDef(double iamp[MAX_FASE],
                               double assim_iamp[MAX_FASE]);
   bool __fastcall  Inicia(VTData *data,
                           VTNet  *net,
                           int    ind_pat);
   bool __fastcall  Vfn_kv(VTBarra         *barra,
                           complex<double> vkv[MAX_FASE]);

private:  // Objetos externos
   TImpressao1 *impr1;
   VTData      *data;
   VTAtualiza1 *atual1;
   VTCalculo1  *calculo1;
   VTCurtoAux  *curto_aux;
   VTGeralC    *geralC;
   VTMonta1    *monta1;
   VTRede1     *rede1;

   VTApl       *apl;
   VTBarra     *barra;
   VTDemanda   *demanda;
   VTNet       *net;
	VTRedeCC    *redeCC;
	VTTrecho    *trecho;      // Usado para determinar se a barra de curto é fictícia ou não
   VTTrecho    *trecho_fic1; // Primeiro trecho fictício
   VTTrecho    *trecho_fic2; // Segundo  trecho fictício

private:  // Constantes locais
   double          Raiz3, INVR3;
   complex<double> cmeio, czero, czero_mais, cum, alfa, alfa2, cdois, ctres;

private:  // Dados locais
   bool            flag_Executa_CorBarraDef, flag_Executa_CorTrechoDef,
                   flag_Inicia, flag_Inicia2, flag_alocou_memoria;
   int             ind_pat, nelem_Znod, tipo_def;
   double          dist_pu;
   TBarra          *bar_def;
   TTrecho         *trec_def;
   complex<double> s3f_mva, sft_mva, z0_so_trechos_ohm, z1_so_trechos_ohm,
                   zent0_pu, zent1_pu;
   complex<double> *vet_colZ0, *vet_colZ0_ic, *vet_colZ0_kc,
                   *vet_colZ1, *vet_colZ1_ic, *vet_colZ1_kc;
   complex<double> **Znod0, **Znod1;
   complex<double> **v1f, **v2f, **v0f, **vaf, **vbf, **vcf, // Barras
                   *vet_ycarga;
   complex<double> **cor1f, **cor2f, **cor0f,                // Trechos
                   **coraf, **corbf, **corcf;
   complex<double> **corpriA, **corpriB, **corpriC,          // Trafos2
                   **corpri1, **corpri2, **corpri0,
                   **corsecA, **corsecB, **corsecC,
                   **corsec1, **corsec2, **corsec0,
                   **corterA, **corterB, **corterC,
                   **corter1, **corter2, **corter0;
   complex<double> **cor_g1, **cor_g2, **cor_g0,             // Suprimentos
                   **cor_ga, **cor_gb, **cor_gc;
   complex<double> **cs, **cf, **vs;                         // Matrizes temporárias
   double          **cf_assim;                               // Matriz   temporária

private:  // Métodos
   bool            __fastcall AcertaTensoes(void);
   void            __fastcall AlocaColunasMeioTrecho(void);
   bool            __fastcall AlocaMemoria(void);
   bool            __fastcall CalculaCondicaoInicialComCarga(void);
   bool            __fastcall CalculaCondicaoInicialSemCarga(void);
   bool            __fastcall CalculaCorrente(TBarra          *bar1,
                                              complex<double> *van,
                                              complex<double> *cor1);
   void            __fastcall CalculaPotenciasCurtoCircuito(void);
   void            __fastcall CalculaTensao(TBarra          *bar1,
                                            complex<double> *van);
   complex<double> __fastcall CompFaseA(complex<double> v0,
                                        complex<double> v1,
                                        complex<double> v2);
   complex<double> __fastcall CompFaseB(complex<double> v0,
                                        complex<double> v1,
                                        complex<double> v2);
   complex<double> __fastcall CompFaseC(complex<double> v0,
                                        complex<double> v1,
                                        complex<double> v2);
   void            __fastcall ContribBarras(int cu);
	void            __fastcall ContribSup(int cu);
   void            __fastcall ContribTrafos2(int cu);
   void            __fastcall ContribTrafos3(int cu);
   void            __fastcall ContribTrechosComMutuas(int cu);
   void            __fastcall ContribTrechosSemMutuas(int cu);
   void            __fastcall Contribuicoes(int cu);
   bool            __fastcall CorrentesFaseBarraDefeito(int cu);
   bool            __fastcall CorrentesTrafo2(int nt,
                                              int cu);
   bool            __fastcall CorrentesTrafo3(int nt,
                                              int cu);
   bool            __fastcall CorrentesTrecho(int nt,
                                              int cu);
   void            __fastcall Curto_Circ1(void);
   bool            __fastcall Curto_Circ2(complex<double> zdef_fase_pu,
                                          complex<double> zdef_terra_pu,
                                          complex<double> vth1);
	bool            __fastcall Finaliza(void);
   bool            __fastcall Inicia2(void);
   void            __fastcall IniciaConstantes(void);
   void            __fastcall LiberaMemoria(void);
   bool            __fastcall MontaColunasMeioTrecho(void);
   bool            __fastcall ObtemColunasZnodal(int linY);
   void            __fastcall SalvaResCurto(void);
   void            __fastcall SalvaResIccBarraDef(void);
   void            __fastcall SalvaResIccBarraDef2(double iamp[MAX_FASE],
                                                   double assim_iamp[MAX_FASE]);
   void            __fastcall SalvaResIccBarraSup(void);
   void            __fastcall SalvaResIccLigacao(void);
   bool            __fastcall SalvaResIccTrafo2(int    nt,
                                                complex<double> iamp[MAX_FASE+MAX_FASE]);
   bool            __fastcall SalvaResIccTrecho(int    nt,
                                                complex<double> iamp[MAX_FASE+MAX_FASE]);
   void            __fastcall SalvaResVccBarra(void);
   bool            __fastcall SalvaResVccBarra2(int             nb,
                                                complex<double> vkv[MAX_FASE]);
   bool            __fastcall TensoesBarra(int nb,
                                           int cu);
   bool            __fastcall VerificaTipoDefeito(int tipo_def);
   bool            __fastcall ZentradaSoTrechosRede(void);
   void            __fastcall ZeraEstruturas(STR_FASE ifase[][NBAR_LIG],
                                             STR_SEQ  iseq[][NBAR_LIG]);
};

#endif
//---------------------------------------------------------------------------
//eof

