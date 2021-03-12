//---------------------------------------------------------------------------
#ifndef TImpressao3H
#define TImpressao3H

//---------------------------------------------------------------------------
#include "..\Comum13\TImpressao.h"
#include "..\Comum13\VTGeralC.h"

class TBarra;
class TCurtoAux3;
class TMedidor;
class VTRede3;

//---------------------------------------------------------------------------
class TImpressao3 : public TImpressao
   {
   public:  // Métodos
                 __fastcall  TImpressao3(VTRede3    *rede3,
                                         VTGeralC   *geralC,
                                         TCurtoAux3 *curto_aux3=NULL);
                 __fastcall ~TImpressao3(void);
      AnsiString __fastcall  CodigoFasesCarga(int lig);
      bool       __fastcall  FalhaRedundancia(int np);
      bool       __fastcall  ImprimeBarrasDesconexas(TList *lisBAR_DESC,
                                                     TList *lisBAR_CONEXAS);
      bool       __fastcall  ImprimeBarrasSemConvergencia(int    num_bar_sem_conv,
                                                          TBarra **vet_bar_sem_conv);
      bool       __fastcall  ImprimeBarrasVnomZero(TList *lisBAR_VNZ);
      bool       __fastcall  ImprimeDados(int   np,
                                          TList *lisBAR_J=NULL,
                                          TList *lisBAR_ESTADO=NULL,
                                          TList *lisBAR_REF=NULL);
      bool       __fastcall  ImprimeDadosR3(void);
      bool       __fastcall  ImprimeLinhasYnodalJacob(FILE  *arq,
                                                      TList *lisBAR_J);
      bool       __fastcall  ImprimeLisYJ(TList *lisYJ);
      bool       __fastcall  ImprimeResultados(int    np,
                                               TList  *lisBAR_SEM_CONV=NULL,
                                               double *vet_desv_iter=NULL,
                                               TList  *lisBAR_REF=NULL);
      bool       __fastcall  ImprimeResultadosH3_Globais(int             np,
                                                         TList           *lisBAR_H3,
                                                         complex<double> *vet_perda_total_2);
      bool       __fastcall  ImprimeResultadosH3_Harmonica(int             np,
                                                           int             indfreq,
                                                           TList           *lisBAR_H3,
                                                           complex<double> *vet_perda_total_2);
      bool       __fastcall  ImprimeResultadosR3(int      np,
                                                 VTGeralC *geralC,
                                                 TList    *lisBAR,
                                                 TList    *lisLIGACAO);

   private:  // Objetos externos
      VTRede3    *rede3;
      TCurtoAux3 *curto_aux3;

   private:  // Dados
      TList *lisSUP_CODIGO;
      TList *lisTRAFO12_ORD;
      TList *lisTRAFO32_ORD;

   private:  // Dados & Estruturas
      char       codigo_ater[NUM_TIPO_ATERR + 1];
      AnsiString codigo_tipo_curto[5];
      AnsiString codigo_fases_curto[18];
      AnsiString codigo_estatico[NUM_LIG_ESTATICO];
      AnsiString codigo_fases[NUM_LIG_CARGA];
      char       codigo_lig_enrol[3];
      AnsiString codigo_tipo_12[5];
      AnsiString codigo_tipo_32[8];

   private:  // Métodos
      // void            __fastcall ImprimeCargas(void);
      bool            __fastcall ImprimeDados1(TList *lisBAR_REF);
      bool            __fastcall ImprimeDados2(int   np,
                                               TList *lisBAR_J,
                                               TList *lisBAR_ESTADO);
      bool            __fastcall ImprimeDadosESTIM3(int   np,
                                                    TList *lisBAR_ESTADO);
      bool            __fastcall ImprimeDadosMedidores(FILE *arq);
      bool            __fastcall ImprimeResultados1(int   np,
                                                    TList *lisBAR_SEM_CONV);
      bool            __fastcall ImprimeResultados2(void);
      bool            __fastcall ImprimeResultados3(int np);
      bool            __fastcall ImprimeResultadosESTIM3(double *vet_desv_iter,
                                                         TList  *lisBAR_REF);
      bool            __fastcall ImprimeResultadosMedidores(FILE *arq);
      bool            __fastcall ImprimeResultadosMedidoresExcel(void);
      void            __fastcall MontaSuprimentosCodigo(void);
      void            __fastcall OrdenaBarrasCodigo(void);
      void            __fastcall OrdenaTrafo12(void);
      void            __fastcall OrdenaTrafo32(void);
      void            __fastcall OrdenaTrechos(void);
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TImpressao3
//---------------------------------------------------------------------------
TImpressao3 * __fastcall NewObjImpressao3(VTRede3    *rede3,
                                          VTGeralC   *geralC,
                                          TCurtoAux3 *curto_aux3=NULL);
#endif
//---------------------------------------------------------------------------

