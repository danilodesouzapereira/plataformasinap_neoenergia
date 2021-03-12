//---------------------------------------------------------------------------
#ifndef TCurvaENERQCTH
#define TCurvaENERQCTH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TCurvaENERQCT : public TObject
{
public:  // Métodos
				    __fastcall  TCurvaENERQCT(void);
				    __fastcall ~TCurvaENERQCT(void);
			double __fastcall  AjustaTap(int    modelo,
		                                double vf_pu,
                                      double var_unit,
                                      TBarra *bar_ref,
                                      TBarra *bar2,
                                      int    *passo_int);
};

#endif
//---------------------------------------------------------------------------

