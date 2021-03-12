//---------------------------------------------------------------------------
#ifndef TDragdropH
#define TDragdropH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDragdrop.h"

//---------------------------------------------------------------------------
class TDragdrop : public VTDragdrop
   {
   public:
      __fastcall TDragdrop(void);
      __fastcall ~TDragdrop(void);

	protected:  //métodos acessados via property
		void __fastcall InsEdtCopia(VTEdt *edt);
      void __fastcall InsEdtCola(VTEdt *edt);
      void __fastcall RetEdtCola(VTEdt *edt);
      void __fastcall RetEdtCopia(VTEdt *edt);

	private:  //dados acessados via property
		struct	{
					VTEdt  *edt_copia;
					}PD;

	private:  //dados locais
		TList  *lisEDT_COLA;
	};

#endif
//---------------------------------------------------------------------------
//eof
