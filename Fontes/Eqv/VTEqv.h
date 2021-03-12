//---------------------------------------------------------------------------
#ifndef VTEqvH
#define VTEqvH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTApl;
class VTNet;

//---------------------------------------------------------------------------
enum modeloEQV {modeloEQV_INDEF=0, modeloEQV1=1, modeloEQV3=3};

struct strResBarraAdmit
   {
   int             bar_id;
   complex<double> Ynodal0_pu;
   complex<double> Ynodal1_pu;
   complex<double> mat_ynodal_S[MAX_FASE][MAX_FASE];
   };

struct strResLigacaoAdmit
   {
   int             indice;
   int             bar1_id;
   int             bar2_id;
   int             fase_int_pri;
   int             fase_int_sec;
   AnsiString      codigo;
   complex<double> Ynodal0_ij_pu;                       // Seq. zero,   elemento ij
   complex<double> Ynodal0_ji_pu;                       // Seq. zero,   elemento ji
   complex<double> Ynodal1_ij_pu;                       // Seq. direta, elemento ij
   complex<double> Ynodal1_ji_pu;                       // Seq. direta, elemento ji
   complex<double> mat_ynodal_ij_S[MAX_FASE][MAX_FASE]; // Submatriz ij
   complex<double> mat_ynodal_ji_S[MAX_FASE][MAX_FASE]; // Submatriz ji
   };

//---------------------------------------------------------------------------
class VTEqv : public TObject
   {
   public:  //property
      __property double AdmitanciaCortePU = {read=PD.admit_corte_pu, write=PD.admit_corte_pu};
      __property int    Modelo            = {read=PM_GetModelo};
      __property int    TipoRelatorio     = {read=PD.tipo_relatorio, write=PD.tipo_relatorio};

   public:
                   __fastcall  VTEqv(void) {};
      virtual      __fastcall ~VTEqv(void) {};
      virtual bool __fastcall  AtualizaTensao(int             bar_id,
                                              complex<double> *vet_tensao_pu) = 0;
      virtual bool __fastcall  CalculaNovasCargas(void) = 0;
      virtual void __fastcall  DefineModeloCarga(double Icte,
                                                 double Scte,
                                                 double Zcte) = 0;
      virtual bool __fastcall  DeterminaReducao(void) = 0;
      virtual bool __fastcall  ImprimeRedeReduzida(int np) = 0;
      virtual bool __fastcall  MontaRedeOriginal(VTNet *net) = 0;
      virtual bool __fastcall  ReduzRede(void) = 0;
      virtual bool __fastcall  ResBarraAdmit(strResBarraAdmit *res_barra) = 0;
      virtual bool __fastcall  ResBarraCargaTotal(int             bar_id,
                                                  complex<double> *vet_carga_kVA) = 0;
      virtual bool __fastcall  ResBarraDeltaCarga(int             bar_id,
                                                  complex<double> *vet_carga_kVA) = 0;
      virtual bool __fastcall  ResLigacao(strResLigacaoAdmit *res_ligacao) = 0;
      virtual int  __fastcall  ResLigacaoCount(void) = 0;

   protected:  //métodos acessados via property
      virtual int  __fastcall  PM_GetModelo(void) = 0;

   protected:  //dados acessados via property
      struct{
            int    tipo_relatorio;
            double admit_corte_pu;
            }PD;
   };

//---------------------------------------------------------------------------
// Funções globais para criar objeto da classe VTEqv
//---------------------------------------------------------------------------
VTEqv * __fastcall NewObjEqv1(VTApl *apl);
VTEqv * __fastcall NewObjEqv3(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
