//---------------------------------------------------------------------------
#ifndef TBarraH
#define TBarraH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTBarra.h"

//---------------------------------------------------------------------------
class TBarra : public VTBarra
   {
   public:
					  __fastcall  TBarra(int num_pat);
					  __fastcall ~TBarra(void);
		void       __fastcall  AreaEsquematico(int &x1, int &y1, int &x2, int &y2);
		VTBarra*   __fastcall  Clone(void);
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
		void       __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
		void       __fastcall  CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
																	int &x2, int &y2, int &x3, int &y3);
		bool       __fastcall  CoordenadasUtm_cm(int &x, int &y);
		bool       __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
		bool       __fastcall  CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
															 int &x2, int &y2, int &x3, int &y3);
		bool       __fastcall  CoordenadasUtm_m(int &x, int &y);
		bool       __fastcall  CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2);
      bool       __fastcall  CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
															int &x2, int &y2, int &x3, int &y3);
		void       __fastcall  DefineObjZona(VTZona *zona);
		bool       __fastcall  ExisteEqbar(VTEqbar *eqbar);
		VTEqbar*   __fastcall  ExisteEqbar(int tipo, int id);
		bool       __fastcall  ExisteGeracao(bool incluir_reducao);
		bool       __fastcall  ExisteGeracaoPV(void);
		bool       __fastcall  ExisteGeracaoVF(void);
		void       __fastcall  InsereEqbar(VTEqbar *eqbar);
      void       __fastcall  InsereLigacao(VTLigacao *ligacao);
		void       __fastcall  InsereNo(VTNo *no);
		bool       __fastcall  IsBarra(void);
        bool       __fastcall LigacaoReducao(VTLigacao *ligacao);
		TList*     __fastcall  LisEqbar(void);
		void       __fastcall  LisEqbar(TList *lisEQP);
		void       __fastcall  LisEqbar(TList *lisEQP, int tipo);
      TList*     __fastcall  LisLigacao(void);
      void       __fastcall  LisLigacao(TList *lisEQP, int tipo);
		int        __fastcall  NumeroDeEqbar(int tipo);
		void       __fastcall  PontoConexao(double posbar, int &x, int &y);
		void       __fastcall  ReiniciaLisNo(void);
		void       __fastcall  RemoveEqbar(VTEqbar *eqbar);
      void       __fastcall  RemoveLigacao(VTLigacao *ligacao);
      void       __fastcall  RemoveNo(VTNo *no);
		int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
		bool       __fastcall  TipoBarra(void);
		bool       __fastcall  Vertical(void);

	protected: //métodos acessados via property
		VTNo*  __fastcall PM_GetNo(VTNet *net);
		double __fastcall PM_GetVnom_kv(void);
		//set
      void   __fastcall PM_SetNo(VTNo *no, VTNet *net);
      void   __fastcall PM_SetVnom_kv(double vnom_kv);

	protected: //dados acessados via property
		struct   {
					float vnom_kv;
					}PD;

	public:  //dados locais
      TList  *lisEQBAR;    //lista de objetos VTEqbar
      TList  *lisLIG;      //lista de objetos VTLigacao
      TList  *lisNO;
   };

//---------------------------------------------------------------------------
#endif
//eof
