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
	public:  //métodos
		__fastcall  TNodeDetalhe(void);
		__fastcall ~TNodeDetalhe(void);

//    protected:  //métodos acessados via property
//		//getters
//		int __fastcall PM_GetEstado(void);
};

//---------------------------------------------------------------------------
#endif
//eof
