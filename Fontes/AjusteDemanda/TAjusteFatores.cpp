//---------------------------------------------------------------------------
#pragma hdrstop
#include "TAjusteFatores.h"
#include "TAjusteFator.h"
//---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTAjusteFatores* __fastcall NewObjAjusteFatores(VTApl *apl)
{
try{
  return(new TAjusteFatores(apl));
  }catch(Exception &e)
	 {
	 return(NULL);
	 }
}

//---------------------------------------------------------------------------
__fastcall  TAjusteFatores::TAjusteFatores(VTApl *apl)
{
	lisAJUSTEFATOR = new TList;
	lisAJUSTEFATOR_MT = new TList;
	lisAJUSTEFATOR_BT = new TList;
	lisAJUSTEFATOR_SED = new TList;
	this->apl = apl;
}
//---------------------------------------------------------------------------
__fastcall TAjusteFatores::~TAjusteFatores(void)
{
	DestroiTObject(lisAJUSTEFATOR_MT);
	DestroiTObject(lisAJUSTEFATOR_BT);
	DestroiTObject(lisAJUSTEFATOR_SED);
	LimpaTList(lisAJUSTEFATOR);
	DestroiTObject(lisAJUSTEFATOR);
}
//---------------------------------------------------------------------------
void __fastcall TAjusteFatores::CriaFatoresRedes(int tipoSEGMENTO)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisRedes = new TList;
	VTRede *rede;

	redes->LisRede(lisRedes, tipoSEGMENTO);
	for (int nr = 0; nr < lisRedes->Count; nr++)
	{
		rede = (VTRede*) lisRedes->Items[nr];
		InsereFator(rede);
	}

}
//---------------------------------------------------------------------------
VTAjusteFator* __fastcall TAjusteFatores::ExisteFator(VTRede* rede)
{
	TAjusteFator *fator = NULL;

	for (int nf = 0; nf < lisAJUSTEFATOR->Count; nf++)
	{
		fator = (TAjusteFator*) lisAJUSTEFATOR->Items[nf];
		if (fator->Rede == rede)
			{break;}
	}
	return fator;
}
//---------------------------------------------------------------------------
VTAjusteFator* __fastcall TAjusteFatores::InsereFator(VTRede* rede)
{
	TAjusteFator *fator;

	fator = new TAjusteFator();
	fator->Rede = rede;
	lisAJUSTEFATOR->Add(fator);

	switch (rede->TipoRede->Segmento)
	{
	case redePRI:
		lisAJUSTEFATOR_MT->Add(fator);
		break;
	case redeSEC:
		lisAJUSTEFATOR_BT->Add(fator);
		break;
	case redeETD:
		lisAJUSTEFATOR_SED->Add(fator);
		break;
    default:
        ;
	}

	return fator;
}
//---------------------------------------------------------------------------
void		  __fastcall TAjusteFatores::LisFatores(TList *lisEXT)
{
	lisEXT->Clear();
	lisEXT->Assign(lisAJUSTEFATOR, laCopy);
}
//---------------------------------------------------------------------------
