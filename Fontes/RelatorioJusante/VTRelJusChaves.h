// ---------------------------------------------------------------------------
#ifndef VTRelJusChavesH
#define VTRelJusChavesH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include <complex>
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
class VTRelJusChaves : public TObject
{

public:
				 __fastcall VTRelJusChaves(void){};
	virtual 	 __fastcall ~VTRelJusChaves(void){};
	virtual bool __fastcall Inicia(void)=0;
	virtual void __fastcall AgregaValores(int n_dias, int patamar)=0;

public:  //property
	//__property TColor CorDestaque = {read=colorDestaque, write=colorDestaque};

protected:
	//TColor	colorDestaque;

};

VTRelJusChaves* NewObjRelJusChaves(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
