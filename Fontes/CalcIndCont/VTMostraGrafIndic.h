//---------------------------------------------------------------------------
#ifndef VTMostraGrafIndicH
#define VTMostraGrafIndicH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTResCont;
//---------------------------------------------------------------------------
class VTMostraGrafIndic: public TObject
	{
	public:  //property
		 __property VTResCont* ResCont                  = {read=PM_GetResCont, write=PM_SetResCont};

	public:
									__fastcall VTMostraGrafIndic(void) {};
									__fastcall ~VTMostraGrafIndic(void) {};
			 virtual     void __fastcall MostraContribBloco(int indic, AnsiString titulo, double fatorEsc) = 0;
			 virtual     void __fastcall MostraContribTransf(TList *lisDefeitos) = 0;


	protected: //métodos acessados via property
		 virtual VTResCont*   __fastcall PM_GetResCont(void) = 0;
		 virtual void         __fastcall PM_SetResCont(VTResCont *resCont) = 0;

	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTMostraGrafIndic* __fastcall NewObjMostraGrafIndic(VTApl* apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
