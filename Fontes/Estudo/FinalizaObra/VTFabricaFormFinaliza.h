// ---------------------------------------------------------------------------

#ifndef VTFabricaFormFinalizaH
#define VTFabricaFormFinalizaH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <System.Classes.hpp>
#include <Vcl.Forms.hpp>
// ---------------------------------------------------------------------------
class VTApl;
class VTObra;
// ---------------------------------------------------------------------------
class VTFabricaFormFinaliza : public TObject
{
	public:

					__fastcall 	VTFabricaFormFinaliza(void){};
	virtual			__fastcall 	~VTFabricaFormFinaliza(void){};
	virtual TForm*  __fastcall  NewFormFinalizaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra) = 0;

	protected:
		VTApl *apl;
};
 //---------------------------------------------------------------------------
//função global p/ criar objetos da classe VTObra
//---------------------------------------------------------------------------
VTFabricaFormFinaliza* NewObjFabricaFormFinaliza(VTApl *apl);
#endif
// ---------------------------------------------------------------------------
// eof
