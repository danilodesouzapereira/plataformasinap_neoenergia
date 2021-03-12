//---------------------------------------------------------------------------
#ifndef TEstoqueH
#define TEstoqueH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTRegulador;
//---------------------------------------------------------------------------
class TEstoque : public TObject
	{
	public:  //property
		__property int          Unidades    = {read=PD.unid, write=PD.unid};
		__property VTRegulador* Regulador   = {read=PD.reg,  write=PD.reg};

   public:
		__fastcall  TEstoque(void);
		__fastcall ~TEstoque(void);

	protected:
		struct {
				 int 	       unid;
				 VTRegulador *reg;
				 }PD;
	};

//---------------------------------------------------------------------------
#endif
//eof

