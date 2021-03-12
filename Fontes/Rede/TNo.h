//---------------------------------------------------------------------------
#ifndef TNoH
#define TNoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTNo.h"

//---------------------------------------------------------------------------
class TNo : public VTNo
   {
   public:
                 __fastcall  TNo(VTBarra *pbarra, VTNet *net);
                 __fastcall ~TNo(void);
      void       __fastcall  ConfiguraFases(VTEqpto *eqpto, int fases);
      void       __fastcall  ConfiguraFasesNT(void);
      bool       __fastcall  ExisteGeracao(void);
      bool       __fastcall  ExisteGeracaoPV(void);
      bool       __fastcall  ExisteGeracaoVF(void);
      void       __fastcall  InsereBarra(VTBarra *pbarra);
      void       __fastcall  InsereLigacao(VTLigacao *ligacao);
      void       __fastcall  InsereEqbar(VTEqbar *eqbar);
      void       __fastcall  LisEqbar(VTBarra *pbarra, TList *lisEQP);
      void       __fastcall  LisGerador(TList *lisEQP);
      void       __fastcall  LisSuprimento(TList *lisEQP);
      int        __fastcall  NumEqbar(void);
      int        __fastcall  NumEqbar(VTBarra *pbarra);
      int        __fastcall  NumSupGerPQ(void);
      int        __fastcall  NumSupGerPV(void);
      int        __fastcall  NumSupGerPV_VF(void);
		int        __fastcall  NumSupGerVF(void);
      void       __fastcall  RemoveEqbar(VTEqbar *eqbar);
      bool       __fastcall  Swing(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      int        __fastcall  TipoDeBarra(void);
      bool       __fastcall  ValidaFasesTrafo(void);
      bool       __fastcall  Y0ref_pu(double Sbase, double Vbase, complex<double> &y0);
      bool       __fastcall  Y1ref_pu(double Sbase, double Vbase, complex<double> &y1);
      bool       __fastcall  Z0ref_pu(double Sbase, double Vbase, complex<double> &z0);
      bool       __fastcall  Z1ref_pu(double Sbase, double Vbase, complex<double> &z1);
      bool       __fastcall  Zeq(double Sbase, complex<double> &z0, complex<double> &z1);

   protected: //métodos acessados via property
		int  __fastcall PM_GetFases(void);
		int  __fastcall PM_GetModeloCarga(void);
		//void __fastcall PM_SetFases(int fases);

	private: //métodos
		bool __fastcall ExisteGeracao(int tipo_sup);

	private: //dados
		struct 	{
					int fases_lig, fases_trf;
				 	} PD;
	};

//---------------------------------------------------------------------------
#endif
//eof

