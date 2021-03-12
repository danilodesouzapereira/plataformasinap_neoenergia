//---------------------------------------------------------------------------
#ifndef TImpressao1H
#define TImpressao1H

//---------------------------------------------------------------------------
#include "..\Comum13\TImpressao.h"

class TBarra;
class VTRede1;

//---------------------------------------------------------------------------
class TImpressao1 : public TImpressao
   {
   public:  // Métodos
           __fastcall  TImpressao1(VTRede1  *rede1,
                                   VTGeralC *geralC);
           __fastcall ~TImpressao1(void);
      bool __fastcall  FalhaRedundancia(int np);
      bool __fastcall  ImprimeBarrasDesconexas(TList *lisBAR_DESC,
                                               TList *lisBAR_CONEXAS);
      bool __fastcall  ImprimeDados(bool  flag_imprime_tudo,
                                    int   np,
                                    TList *lisBAR_J=NULL,
                                    TList *lisBAR_ESTADO=NULL,
                                    TList *lisBAR_REF=NULL);
      bool __fastcall  ImprimeDadosR1(void);
      bool __fastcall  ImprimeLinhasYnodalJacob(FILE *arq,
                                                TList *lisBAR_J=NULL);
      bool __fastcall  ImprimeResultados(int    np,
                                         double *vet_desv_iter=NULL,
                                         TList  *lisBAR_REF=NULL);
      bool __fastcall  ImprimeResultadosCurto(int             ind_pat,
                                              complex<double> zent0_pu,
                                              complex<double> zent1_pu,
                                              complex<double> s3f_mva,
                                              complex<double> sft_mva,
                                              complex<double> **cf,
                                              complex<double> **cs,
                                              complex<double> **vaf,
                                              complex<double> **vbf,
                                              complex<double> **vcf,
                                              complex<double> **cor_ga,
                                              complex<double> **cor_gb,
                                              complex<double> **cor_gc,
                                              complex<double> **cor_g0,
                                              complex<double> **cor_g1,
                                              complex<double> **cor_g2,
                                              TBarra          *bar_def);
      bool __fastcall  ImprimeResultadosR1(int      np,
                                           VTGeralC *geralC,
                                           TList    *lisBAR,
                                           TList    *lisTRECHO);
      bool __fastcall  ImprimeTensaoBarras(void);

   private:  // Objetos externos
      VTRede1 *rede1;

   private:  // Dados & Estruturas
      AnsiString codigo_tipo_curto[5];
      AnsiString codigo_lig_trafo[6];
      TList      *lisTRAFO2_ORD;

   private:  // Métodos
      FILE *          __fastcall AbreArquivo(int codigo);
      double          __fastcall CalculaFatorPotencia(bool       tipo_carga,
                                                      double     p,
                                                      double     q,
                                                      AnsiString *cod_nat);
      // void            __fastcall ImprimeCargas(void);
      bool            __fastcall ImprimeDados1(TList *lisBAR_REF);
      bool            __fastcall ImprimeDados2(bool  flag_imprime_tudo,
                                               int   np,
                                               TList *lisBAR_J,
                                               TList *lisBAR_ESTADO);
      bool            __fastcall ImprimeDadosESTIM1(int   np,
                                                    TList *lisBAR_ESTADO);
      bool            __fastcall ImprimeDadosMedidores(FILE *arq);
      bool            __fastcall ImprimeLigacoesFluxoRadial(FILE  *arq,
                                                            TList *lisLIG_RADIAL);
      bool            __fastcall ImprimeResultados1(int np);
      bool            __fastcall ImprimeResultados2(void);
      bool            __fastcall ImprimeResultados3(int np);
      bool            __fastcall ImprimeResultadosESTIM1(double *vet_desv_iter,
                                                         TList  *lisBAR_REF);
      bool            __fastcall ImprimeResultadosMedidores(FILE *arq);
      void            __fastcall OrdenaBarrasCodigo(void);
      void            __fastcall OrdenaTrafo2(void);
      void            __fastcall OrdenaTrechos(void);
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TImpressao1
//---------------------------------------------------------------------------
TImpressao1 * __fastcall NewObjImpressao1(VTRede1  *rede1,
                                          VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

