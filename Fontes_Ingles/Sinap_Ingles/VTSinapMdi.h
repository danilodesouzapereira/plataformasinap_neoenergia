//-----------------------------------------------------------------------------
#ifndef VTSinapMdiH
#define VTSinapMdiH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTSinapMdi : public TObject
   {
	public:  //property
		__property TAction*     ActionNewChild   = {read=PM_GetActionNewChild,   write=PM_SetActionNewChild  };
		__property TMainMenu*   MainMenu         = {read=PM_GetMainMenu,         write=PM_SetMainMenu        };
		__property TPopupMenu*  PopupMenuImporta = {read=PM_GetPopupMenuImporta, write=PM_SetPopupMenuImporta};

	public:
								 __fastcall  VTSinapMdi(void){};
								 __fastcall ~VTSinapMdi(void){};
		virtual AnsiString __fastcall  UsuarioEmpresa(void) = 0;
		virtual AnsiString __fastcall  UsuarioNome(void) = 0;
		virtual bool       __fastcall  UsuarioPrivilegiado(void) = 0;
		virtual int        __fastcall  UsuarioTipo(void) = 0;

   protected:  //métodos acessados via property
		virtual TAction*    __fastcall PM_GetActionNewChild(void) = 0;
		virtual TMainMenu*  __fastcall PM_GetMainMenu(void) = 0;
		virtual TPopupMenu* __fastcall PM_GetPopupMenuImporta(void) = 0;
		//
		virtual void __fastcall PM_SetActionNewChild(TAction *Action) = 0;
		virtual void __fastcall PM_SetMainMenu(TMainMenu *MainMenu) = 0;
		virtual void __fastcall PM_SetPopupMenuImporta(TPopupMenu *PopupMenu) = 0;
	 };

//-----------------------------------------------------------------------------
//função global
VTSinapMdi* __fastcall NewObjSinapMdi(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
