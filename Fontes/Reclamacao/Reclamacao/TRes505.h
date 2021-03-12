//---------------------------------------------------------------------------
#ifndef TRes505H
#define TRes505H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRes505.h"
//---------------------------------------------------------------------------
class TRes505 : public VTRes505
   {
   public:
			  __fastcall  TRes505(VTApl *apl);
			  __fastcall ~TRes505(void);
		int  __fastcall ClassificaAdequadaPrecariaCritica(double tensao);
		//void __fastcall DefineFaixaTensao(int ra505_vnom, double vff_kv, double vfn_kv);
		void __fastcall DefineFaixaTensao(int ra505_vnom);
      void __fastcall DefineFaixaTensao(double vff_kv, double vfn_kv);

	private: //métodos
		void __fastcall DefineFaixaTensaoDefault(double vff_kv, double vfn_kv);
      bool __fastcall FaixaAdequada(double tensao);
      bool __fastcall FaixaCritica(double tensao);
		bool __fastcall FaixaPrecaria(double tensao);
		bool __fastcall LeDados(void);

	private: //obj externo
		VTApl *apl;
   };

//---------------------------------------------------------------------------
#endif
//eof

