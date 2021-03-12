//---------------------------------------------------------------------------

#ifndef TCarregamentoH
#define TCarregamentoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class VTApl;
class VTRede;
class VTCarga;
class VTEqpto;
class VTNet;

//---------------------------------------------------------------------------
class TCarregamento
{
	public:
		     __fastcall  TCarregamento(VTApl *apl);
	        __fastcall ~TCarregamento(void);
		bool __fastcall  AnalisaRede(VTRede *rede);
	 double __fastcall  GetCarregamento_kva(VTEqpto *eqpto, int npat=-1);
		void __fastcall  GetCarregamentoABC_kva(complex<double> *sa, complex<double> *sb, complex<double> *sc, int npat=-1);
		void __fastcall  GetCarregamento3F_kva(complex<double> *s3, int npat=-1);
		void __fastcall  GetPotAtiva(double *pa, double *pb, double *pc, int npat=-1);
		void __fastcall  GetPotReativa(double *qa, double *qb, double *qc, int npat=-1);
		int  __fastcall  PatamarMaiorCarregamento(void);

	private:
		void __fastcall AlocaVetoresCargto(void);
		bool __fastcall AnalisaCarga(VTCarga *carga);
		bool __fastcall AcumulaDemPorFase(VTNet *net, VTEqpto *eqpto, int npat);
		bool __fastcall DistribuiDemPorFase(VTCarga *carga, int npat);
      void __fastcall LiberaVetoresCargto(void);

	private:
		VTApl *apl;
      int    num_pat;
		double *p_faseA, *p_faseB, *p_faseC;
		double *q_faseA, *q_faseB, *q_faseC;
      /*
		double p_faseA[24], p_faseB[24], p_faseC[24];
		double q_faseA[24], q_faseB[24], q_faseC[24];
      struct strCARGTO  {
                        double pa, pb, pc;
                        double qa, qb, qc;
                        }cargto*;
      */
};
//---------------------------------------------------------------------------
#endif
