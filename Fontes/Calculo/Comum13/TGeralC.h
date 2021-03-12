//---------------------------------------------------------------------------
#ifndef TGeralCH
#define TGeralCH

//---------------------------------------------------------------------------
#include ".\VTGeralC.h"

//---------------------------------------------------------------------------
class TGeralC : public VTGeralC
   {
   public:  // Métodos
                      __fastcall  TGeralC(int    prog,
                                          int    tipo_relatorio,
                                          VTApl  *apl,
                                          VTData *data,
                                          bool   flag_imprime_bar_desc);
                      __fastcall ~TGeralC(void);
      FILE *          __fastcall  AbreArquivo(int codigo);
	   void            __fastcall  CalculaPerdaTotal(void);
      void            __fastcall  DefineParametrosEstimador(void);
	   AnsiString      __fastcall  DirTmp(void);
	   double          __fastcall  DuracaoPatamar(int np);
      complex<double> __fastcall  FiltraZero(complex<double> valor,
                                             double          precisao);
      void            __fastcall  HabilitaSoPrimeiroPatamar(void);
      bool            __fastcall  InsereOrdemHarmonica(int ordem);
      bool            __fastcall  IsProgramaNR1(int indice);
      void            __fastcall  MaxIter(int programa,
                                          int *num_max1,
                                          int *num_max2);
      int             __fastcall  OrdemHarmonica(int indice);
      bool            __fastcall  PatamarHabilitado(int   np,
                                                    TList *lisBATERIA);
      void            __fastcall  ZeraContadoresNR(void);
      void            __fastcall  ZeraEnergia(void);
      void            __fastcall  ZeraTotaisPatamar(void);

   protected: // Métodos acessados via property
      bool __fastcall PM_GetCargaEmYnodal(void);
      bool __fastcall PM_GetCargaInjecoesExt(void);
      bool __fastcall PM_GetCargaNula(void);
	   bool __fastcall PM_GetGravaYnodal(void);
	   int  __fastcall PM_GetModeloCargaImposto(void);
      int  __fastcall PM_GetNumHarmonicas(void);
      int  __fastcall PM_GetTipoAtualizacaoTensoes(void);
      int  __fastcall PM_GetTipoCarga(void);
	   void __fastcall PM_SetGravaYnodal(bool flag);
	   void __fastcall PM_SetModeloCargaImposto(int modelo);
	   void __fastcall PM_SetTipoAtualizacaoTensoes(int tipo);
      void __fastcall PM_SetTipoCarga(int tipo);

   private:  // Dados
	   int             modelo_carga_imposto;     // cf. 'enum_MODELO_CARGA'
      int             tipo_atualizacao_tensoes; // cf. 'enum_ATUALIZA_TENSOES'
      int             tipo_carga;               // cf. 'enum_TIPO_CARGA'
      double          *vet_duracao_pat_h;       // Duração de cada patamar (horas)
      complex<double> czero;

   private:  // Dados para FluxoH3
      bool            flag_grava_ynodal;     // Flag que indica se Ynodal deve sre gravada
      int             NumMaxHarmonicas;      // Dimensão de 'vet_ordem_harmonicas'
      int             num_harmonicas;        // Número de valores harmônicos a serem considerados
      int             *vet_ordem_harmonicas; // Ordem das harmônicas de TODAS AS CARGAS (1, 3, 5, ...)

   private:  // Métodos
      // complex<double> __fastcall FiltraPerdaAtivaNegativa(complex<double> perda);
   };

#endif
//---------------------------------------------------------------------------

