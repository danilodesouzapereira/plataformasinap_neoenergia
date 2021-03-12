//---------------------------------------------------------------------------
#ifndef VTBarraH
#define VTBarraH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTEqpto.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTLigacao;
class VTNo;
class VTNet;
class VTResFlowBar;
class VTZona;

//---------------------------------------------------------------------------
class VTBarra : public VTEqpto
   {
   public: //property
      __property double vnom           = {read=PM_GetVnom_kv, write=PM_SetVnom_kv};
      __property VTNo*  No[VTNet *net] = {read=PM_GetNo};

   public:
                       __fastcall VTBarra(void) {};
      virtual          __fastcall ~VTBarra(void) {};
      virtual void     __fastcall AreaEsquematico(int &x1, int &y1, int &x2, int &y2) = 0;
      virtual VTBarra* __fastcall Clone(void) = 0;
      virtual void     __fastcall DefineObjZona(VTZona *zona) = 0;
      virtual bool     __fastcall ExisteEqbar(VTEqbar *eqbar) = 0;
      virtual VTEqbar* __fastcall ExisteEqbar(int tipo, int id) = 0;
	  virtual bool     __fastcall ExisteGeracao(bool incluir_reducao = true) = 0;
	  virtual bool     __fastcall ExisteGeracaoPV(void) = 0;
      virtual bool     __fastcall ExisteGeracaoVF(void) = 0;
      virtual void     __fastcall InsereEqbar(VTEqbar *eqbar) = 0;
      virtual void     __fastcall  InsereLigacao(VTLigacao *ligacao) = 0;
      virtual void     __fastcall InsereNo(VTNo *no) = 0;
      virtual TList*   __fastcall LisEqbar(void) = 0;
      virtual void     __fastcall LisEqbar(TList *lisEQP) = 0;
      virtual void     __fastcall LisEqbar(TList *lisEQP, int tipo) = 0;
      virtual TList*   __fastcall LisLigacao(void) = 0;
      virtual void     __fastcall LisLigacao(TList *lisEQP, int tipo=-1) = 0;
      virtual int      __fastcall NumeroDeEqbar(int tipo=-1) = 0;
      virtual void     __fastcall PontoConexao(double posbar, int &x, int &y) = 0;
      virtual void     __fastcall ReiniciaLisNo(void) = 0;
      virtual void     __fastcall RemoveEqbar(VTEqbar *eqbar) = 0;
      virtual void     __fastcall RemoveLigacao(VTLigacao *ligacao) = 0;
      virtual void     __fastcall RemoveNo(VTNo *no) = 0;
      virtual bool     __fastcall Vertical(void) = 0;

   protected: //métodos acessados via property
      virtual VTNo*  __fastcall PM_GetNo(VTNet *net) = 0;
      virtual double __fastcall PM_GetVnom_kv(void) = 0;
      //set
      virtual void   __fastcall PM_SetVnom_kv(double vnom_kv) = 0;

   public:  //dados externos
      //VTNo    *pno;
      VTZona  *zona;

   public:  //dados locais
      //int          index;     //diferenciador p/ barras de coord.utm iguais
      strUTM       utm;       //coordenadas utm em cm
      strESQ_BARRA esq;       //coordenadas p/ diagrama esquemático geral (adimensional)
      strESQ_BARRA esq_orig;  //coordenadas p/ diagrama esquemático (adimensional)
      VTResFlowBar *resflow;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTBarra
//---------------------------------------------------------------------------
VTBarra* __fastcall NewObjBarra(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof



 