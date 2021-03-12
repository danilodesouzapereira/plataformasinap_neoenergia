//---------------------------------------------------------------------------

#pragma hdrstop

#include "TFabricaFormFinaliza.h"

#include "TFormFinalizaObra.h"
#include "TFormFinalizaObraARIES.h"
#include "TFormFinalizaObraCOPEL.h"
//---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//funções globais p/ criar objetos da classe TObra
//---------------------------------------------------------------------------
VTFabricaFormFinaliza* NewObjFabricaFormFinaliza(VTApl *apl)
{
	return(new TFabricaFormFinaliza(apl));
}

//---------------------------------------------------------------------------
__fastcall 	TFabricaFormFinaliza::TFabricaFormFinaliza(VTApl *apl)
{
    this->apl = apl;
}
//---------------------------------------------------------------------------
__fastcall 	TFabricaFormFinaliza::~TFabricaFormFinaliza(void)
{

}
//---------------------------------------------------------------------------
TForm*  __fastcall  TFabricaFormFinaliza::NewFormFinalizaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra)
{
	// variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	TForm *form;

	if (licenca != NULL)
	{
		switch (licenca->Empresa)
		{
			case COPEL:
				form = new TFormFinalizaObraCOPEL(Owner, apl_owner, obra);
				break;
			default: //todas vai ter aries haha - temporario
				form = new TFormFinalizaObraARIES(Owner, apl_owner, obra);
				//form = new TFormFinalizaObra(Owner, apl_owner, obra);
				;
		}
	}

	return (form);

}
//---------------------------------------------------------------------------
