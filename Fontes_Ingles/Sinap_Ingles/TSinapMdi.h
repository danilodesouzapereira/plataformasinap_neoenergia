//---------------------------------------------------------------------------
#ifndef TSinapMdiH
#define TSinapMdiH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSinapMdi.h"

//---------------------------------------------------------------------------
class TSinapMdi : public VTSinapMdi
   {
	public:  //métodos
					  __fastcall  TSinapMdi(VTApl *apl);
					  __fastcall ~TSinapMdi(void);
		AnsiString __fastcall  UsuarioEmpresa(void);
		AnsiString __fastcall  UsuarioNome(void);
		bool       __fastcall  UsuarioPrivilegiado(void);
		int        __fastcall  UsuarioTipo(void);

	protected:  //métodos acessados via property
		TAction*    __fastcall PM_GetActionNewChild(void);
		TMainMenu*  __fastcall PM_GetMainMenu(void);
		TPopupMenu* __fastcall PM_GetPopupMenuImporta(void);
		//
		void __fastcall PM_SetActionNewChild(TAction *Action);
		void __fastcall PM_SetMainMenu(TMainMenu *MainMenu);
		void __fastcall PM_SetPopupMenuImporta(TPopupMenu *PopupMenu);

	private:	//objetos externos
		VTApl *apl;

	private:	//dados locais
		struct   {
					TAction    *ActionNewChild;
					TMainMenu  *MainMenu;
					TPopupMenu *PopupMenuImporta;
					}PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
