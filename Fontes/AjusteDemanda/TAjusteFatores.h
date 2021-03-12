//---------------------------------------------------------------------------
#ifndef TAjusteFatoresH
#define TAjusteFatoresH
//---------------------------------------------------------------------------  ,
#include <Classes.hpp>
#include "VTAjusteFatores.h"
//---------------------------------------------------------------------------
class VTApl;
class TAjusteFator;
class VTRede;
//---------------------------------------------------------------------------
class TAjusteFatores : public VTAjusteFatores
{
	public:
		   __fastcall  TAjusteFatores(VTApl *apl);
		   __fastcall ~TAjusteFatores(void);

	public:
		void 		  __fastcall CriaFatoresRedes(int tipoSEGMENTO);
		VTAjusteFator* __fastcall ExisteFator(VTRede* rede);
		VTAjusteFator* __fastcall InsereFator(VTRede* rede);
        void		  __fastcall LisFatores(TList *lisEXT);
	private:


};
//---------------------------------------------------------------------------
#endif
