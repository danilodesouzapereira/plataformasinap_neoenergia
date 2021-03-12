//---------------------------------------------------------------------------
#ifndef VTNoH
#define VTNoH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
//#include "Estrutura.h"
#include "VTEqpto.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTEqbar;
class VTEqpto;
class VTLigacao;
class VTNet;

//---------------------------------------------------------------------------
class VTNo : public VTEqpto
   {
   public:  //property
		__property int    ModeloCarga = {read=PM_GetModeloCarga};
		__property int    Fases       = {read=PM_GetFases};

	public:
                   __fastcall  VTNo(void)  {};
      virtual      __fastcall ~VTNo(void) {};
      virtual void __fastcall  ConfiguraFases(VTEqpto *eqpto, int fases) = 0;
      virtual void __fastcall  ConfiguraFasesNT(void) = 0;
      virtual bool __fastcall  ExisteGeracao(void) = 0;
      virtual bool __fastcall  ExisteGeracaoPV(void) = 0;
      virtual bool __fastcall  ExisteGeracaoVF(void) = 0;
      virtual void __fastcall  InsereBarra(VTBarra *pbarra) = 0;
      virtual void __fastcall  InsereLigacao(VTLigacao *ligacao) = 0;
      virtual void __fastcall  InsereEqbar(VTEqbar *eqbar) = 0;
      virtual void __fastcall  LisEqbar(VTBarra *pbarra, TList *lisEQP) = 0;
      virtual void __fastcall  LisGerador(TList *lisEQP) = 0;
      virtual void __fastcall  LisSuprimento(TList *lisEQP) = 0;
      virtual int  __fastcall  NumEqbar(void) = 0;
      virtual int  __fastcall  NumEqbar(VTBarra *pbarra) = 0;
      virtual int  __fastcall  NumSupGerPQ(void) = 0;
      virtual int  __fastcall  NumSupGerPV(void) = 0;
      virtual int  __fastcall  NumSupGerPV_VF(void) = 0;
      virtual int  __fastcall  NumSupGerVF(void) = 0;
      virtual void __fastcall  RemoveEqbar(VTEqbar *eqbar) = 0;
      virtual bool __fastcall  Swing(void) = 0;
      virtual int  __fastcall  TipoDeBarra(void) = 0;
      virtual bool __fastcall  ValidaFasesTrafo(void) = 0;
      virtual bool __fastcall  Y0ref_pu(double Sbase, double Vbase, complex<double> &y0) = 0;
      virtual bool __fastcall  Y1ref_pu(double Sbase, double Vbase, complex<double> &y1) = 0;
      virtual bool __fastcall  Z0ref_pu(double Sbase, double Vbase, complex<double> &z0) = 0;
      virtual bool __fastcall  Z1ref_pu(double Sbase, double Vbase, complex<double> &z1) = 0;
		virtual bool __fastcall  Zeq(double Sbase, complex<double> &z0, complex<double> &z1) = 0;

   protected: //métodos acessados via property
		virtual int  __fastcall PM_GetFases(void) = 0;
		virtual int  __fastcall PM_GetModeloCarga(void) = 0;
		//virtual void __fastcall PM_SetFases(int fases) = 0;

   public: //dados
      struct   {
               int tipo_barra;
               }flow;
      bool    redutivel; //pode ser descartado na montagem da rede equivalente
      VTNet   *net;
      VTBarra *pai;      //Barra de referência do Nó
      TList   *lisBAR;   //lista de Barras do Nó (pai e filhas)
      TList   *lisLIG;   //lista de Ligacoes internas do Nó
      TList   *lisCAP;
      TList   *lisCAR;
      TList   *lisGER;
      TList   *lisMED;
		TList   *lisREA;
		TList   *lisSUP;
		TList   *lisZT; //lista de eqptos Filtro, TrafoZZ, Zref
	  TList	  *lisBAT;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTNo* __fastcall NewObjNo(VTBarra *pbarra,  VTNet *net);


#endif
//---------------------------------------------------------------------------//eof

