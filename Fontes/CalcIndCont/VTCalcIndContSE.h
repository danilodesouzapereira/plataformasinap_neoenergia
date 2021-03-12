//---------------------------------------------------------------------------
#ifndef VTCalcIndContSEH
#define VTCalcIndContSEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTRede;
//---------------------------------------------------------------------------
class VTCalcIndContSE: public TObject
	{
	public:  //property

	public:
									__fastcall VTCalcIndContSE(void) {};
									__fastcall ~VTCalcIndContSE(void) {};
				virtual   bool __fastcall ExecutaIndicadoresSE(void) = 0;


	protected: //métodos acessados via property

	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTCalcIndContSE* __fastcall NewObjCalcIndContSE(VTApl* apl_owner, VTRede *redeSE);

//---------------------------------------------------------------------------
#endif
//eof
