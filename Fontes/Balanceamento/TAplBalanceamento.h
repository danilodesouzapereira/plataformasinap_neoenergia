//---------------------------------------------------------------------------
#ifndef TAplBalanceamentoH
#define TAplBalanceamentoH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplBalanceamento : public TApl
{
	public:
				  __fastcall  TAplBalanceamento(TForm *form_owner, VTApl *apl_owner);
		virtual __fastcall ~TAplBalanceamento(void);

};
//---------------------------------------------------------------------------
#endif
