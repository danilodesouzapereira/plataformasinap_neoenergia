//---------------------------------------------------------------------------
#ifndef VTItemObraCOPELH
#define VTItemObraCOPELH

//---------------------------------------------------------------------------
#include <Classes.hpp>
 #include "TItemObraARIES.h"
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class VTItemObraCOPEL : public TItemObraARIES
{
	public:  //property
		__property AnsiString  Croqui = {read=copel.filepathCroqui, write =copel.filepathCroqui };
//		__property AnsiString  MunicipioPrincipal = {read=copel.municipioPrincipal, write =copel.municipioPrincipal };
//		__property TStrings*   LisMunicipios = {read=copel.lisMunicipiosExtras};
//		__property AnsiString  Programa = {read=copel.programa, write =copel.programa };
//		__property AnsiString  SiglaObra = {read=copel.siglaObra, write =copel.siglaObra };
//		__property AnsiString  SiglaRegiao = {read=copel.siglaRegiao, write =copel.siglaRegiao };


	public:
				__fastcall  VTItemObraCOPEL(VTApl *apl) :TItemObraARIES(apl){};
		virtual __fastcall ~VTItemObraCOPEL(void) {};
		virtual VTItemObra*	__fastcall  Clone(bool preserveId, bool isDeep)=0;

	protected:  //
		struct
		{
//			AnsiString siglaObra;
//			AnsiString siglaRegiao;
//			AnsiString programa;
			AnsiString filepathCroqui;
//			AnsiString municipioPrincipal;
//			TStrings   *lisMunicipiosExtras;
		}copel;
};

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe VTItemObraCOPEL
//---------------------------------------------------------------------------
VTItemObra* NewObjItemObraCOPEL(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof