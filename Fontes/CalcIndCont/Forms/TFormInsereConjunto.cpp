// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include "..\VTCalcIndCont.h"
#include "..\..\BDadoParam\Dados\VTVerifCjto.h"
#include "..\..\BDadoParam\Dados\VTVerifSe.h"
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>

#include <PlataformaSinap\DLL_Inc\Funcao.h>

#include "TFormInsereConjunto.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormInsereConjunto *FormInsereConjunto;

// ---------------------------------------------------------------------------
__fastcall TFormInsereConjunto::TFormInsereConjunto(TComponent* Owner, VTApl* apl, TList *lisVerifSe, VTCalcIndCont* calcIndCont) : TForm(Owner)
{
	this->apl = apl;
	this->calcIndCont = calcIndCont;
	this->lisVerifSe = lisVerifSe;

	ClbVerifSeInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormInsereConjunto::~TFormInsereConjunto(void)
{
	/*if (lisVerifSe)
	{
		LimpaTList(lisVerifSe);
		delete lisVerifSe;
		lisVerifSe = NULL;
	}*/
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereConjunto::ClbVerifSeInicia()
{

	/*
	//Variáveis locais
	TList *lisRedes;
	VTRede *rede;
	VTVerifSe *verifSe;

	// Reinicia a listbox
	clbVerifSe->Clear();

	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	lisRedes = redes->LisRede();

	for(int i = 0; i < lisRedes->Count; i++)
	{
		rede = (VTRede*)lisRedes->Items[i];
		if(rede->TipoRede->Segmento == redeETD)
		{
			verifSe = NewObjVerifSe();
			verifSe->Codigo = rede->Codigo;
			lisVerifSe->Add(verifSe);
			clbVerifSe->Items->AddObject(verifSe->Codigo, verifSe);
		}
	}
	*/

	//Variáveis locais
	VTVerifSe *verifSe;

	for(int i = 0; i < lisVerifSe->Count; i++)
	{
		verifSe = (VTVerifSe*)lisVerifSe->Items[i];
		clbVerifSe->Items->AddObject(verifSe->Codigo, verifSe);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereConjunto::ActionConfirmaCadastroExecute(TObject *Sender)
{

	VTVerifSe* verifSe = NULL;
	VTVerifCjto* verifCjto = calcIndCont->ParamConf->CriaVerifCjto();//hk201805: NewObjVerifCjto();

	codigo = edtCodigo->Text;
	decAjust = StrToDouble(edtDecAjustado->Text);
	decVerif = StrToDouble(edtDecVerif->Text);
	fecAjust = StrToDouble(edtFecAjustado->Text);
	fecVerif = StrToDouble(edtFecVerif->Text);

	verifCjto->Codigo = codigo;
	verifCjto->DecAjust = decAjust;
	verifCjto->DecVerif = decVerif;
	verifCjto->FecAjust = fecAjust;
	verifCjto->FecVerif = fecVerif;

	for(int i = 0; i < clbVerifSe->Count; i++)
	{
		if(clbVerifSe->Checked[i])
		{
			//Item selecionado, seta ele na conjunto.
			verifSe = (VTVerifSe*)clbVerifSe->Items->Objects[i];
			verifCjto->LisVerifSe->Add(verifSe);
			verifSe->CodigoCjto = verifCjto->Codigo;
			verifSe->Conjunto = verifCjto;
		}
	}

	if(!verifSe){
		Aviso("Selecione ao menos uma SE para o Conjunto.");
		return;
	}

	calcIndCont->ParamConf->LisVerifCjto->Add(verifCjto);//hk201808: reformula

	Close();

}

//---------------------------------------------------------------------------
void __fastcall TFormInsereConjunto::ActionCancelaCadastroExecute(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormInsereConjunto::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}

//---------------------------------------------------------------------------
