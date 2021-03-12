//---------------------------------------------------------------------------
#ifndef TFluxoBarraH
#define TFluxoBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\..\Fluxo\VTFluxo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Rede\Estrutura.h"

using namespace std;

//---------------------------------------------------------------------------
class  VTBarra;
class  VTDemanda;
class  VTGerador;
class  VTSuprimento;
class  VTNo;

//---------------------------------------------------------------------------
class TFluxoBarra : public VTFluxo
   {
   public:
                 __fastcall TFluxoBarra(VTApl *apl);
                 __fastcall ~TFluxoBarra(void);
      void       __fastcall CfgAvisoErro(bool enabled);
      bool       __fastcall Executa(VTNet *net, double Sbase,
                                    AnsiString arqRelFlow, AnsiString arqRelRede);
      AnsiString __fastcall Modelo(void);
      bool       __fastcall UtilizaBarraPV(void);
      bool       __fastcall ValidaAderencia(VTNet *net, VTLog *plog);

   protected:  //métodos acessados via property
      bool       __fastcall  PM_GetEquilibrado(void);

   private: //métodos
      double __fastcall CapacitoresNo(VTDemanda *demanda,
                                      VTNo      *pno,
                                      int       np);
      bool   __fastcall CargaNo(VTDemanda       *demanda,
                                VTNo            *pno,
                                int             np,
                                complex<double> scarga_mva[MAX_FASE]);
      bool   __fastcall ExecutaCalculoFornecimento(VTNo *pno);
      bool   __fastcall ExecutaCalculoTensao(VTNo *pno);
      double __fastcall FatorModeloCarga(VTBarra *barra, int modelo_carga, int ind_pat, int ind_fase);
      double __fastcall ReatoresNo(VTDemanda *demanda,
                                   VTNo      *pno,
                                   int       np);
      void   __fastcall ReiniciaDadosFlow(VTNo *pno);

      // Funções temporárias, para análise por JCG
      void   __fastcall No_Dados(VTNo *pno, bool *def_por_fase, int *fases);
      bool   __fastcall No_GetPQVTNo(VTNo *pno, int np, strSUP vet_sup[MAX_FASE]);

   private: //objetos externos
      VTApl  *apl;
      VTNet  *net;

   private: //dados
      bool     aviso_enabled;
      double   Sbase;
      TList    *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof

 