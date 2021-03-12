//---------------------------------------------------------------------------
#ifndef VTLigaCritH
#define VTLigaCritH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTVisRede;
//---------------------------------------------------------------------------
class VTLigaCrit: public TObject
	{
	public:  //property
//		__property VTBlocoRad*   BlocoRadMon         = {read=PM_GetBlocoRadMon, write=PM_SetBlocoRadMon};


	public:
									__fastcall VTLigaCrit(void) {};
									__fastcall ~VTLigaCrit(void) {};

	protected: //métodos acessados via property
//	  virtual VTBlocoRad*  __fastcall PM_GetBlocoRadMon(void) = 0;
//	  virtual void         __fastcall PM_SetBlocoRadMon(VTBlocoRad* blocoRadMon) = 0;

	public:
		VTVisRede *visRede;
		TList    *lisDelta; //Lista de objetos links criticos 1 até npat: TLigaCrit
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTLigaCrit* __fastcall NewObjLigaCrit(VTVisRede* visRede);

//---------------------------------------------------------------------------
#endif

