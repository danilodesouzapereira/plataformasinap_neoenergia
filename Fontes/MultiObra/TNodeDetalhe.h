//---------------------------------------------------------------------------
#ifndef TNodeDetalheH
#define TNodeDetalheH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTNodeDetalhe.h"
//---------------------------------------------------------------------------
class VTItemObra;
//---------------------------------------------------------------------------
class TNodeDetalhe : public VTNodeDetalhe
{
	public:  //m�todos
		__fastcall  TNodeDetalhe(void);
		__fastcall ~TNodeDetalhe(void);

//    protected:  //m�todos acessados via property
//		//getters
//		int __fastcall PM_GetEstado(void);
};

//---------------------------------------------------------------------------
#endif
//eof
