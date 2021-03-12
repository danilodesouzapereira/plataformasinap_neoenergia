//---------------------------------------------------------------------------
#ifndef VTAjusteFatoresH
#define VTAjusteFatoresH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTAjusteFator;
//---------------------------------------------------------------------------
class VTAjusteFatores : public TObject
{
	public:
		   __fastcall  VTAjusteFatores(void){};
		   __fastcall ~VTAjusteFatores(void){};

	public:
		virtual void 		  __fastcall CriaFatoresRedes(int tipoSEGMENTO)=0;
		virtual VTAjusteFator* __fastcall ExisteFator(VTRede* rede)=0;
		virtual VTAjusteFator* __fastcall InsereFator(VTRede* rede)=0;
		virtual void		  __fastcall LisFatores(TList *lisEXT)=0;

	public:  //property
	  __property TList* ListaFatorBT = {read=lisAJUSTEFATOR_BT};
	  __property TList* ListaFatorMT = {read=lisAJUSTEFATOR_MT};
	  __property TList* ListaFatorSED = {read=lisAJUSTEFATOR_SED};

	protected:
		TList *lisAJUSTEFATOR;
		TList *lisAJUSTEFATOR_MT;
		TList *lisAJUSTEFATOR_BT;
		TList *lisAJUSTEFATOR_SED;
        VTApl *apl;
};
//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTAjusteFatores* __fastcall NewObjAjusteFatores(VTApl *apl);
//---------------------------------------------------------------------------
#endif
