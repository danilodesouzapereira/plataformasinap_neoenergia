// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormExportaInfoCircuito.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include "..\VTCalcIndCont.h"
#include "..\VTResCont.h"
// #include <ProjetosGerais\DLL_Inc\CalcIndCont.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormExportaInfoCircuito *FormExportaInfoCircuito;

// ---------------------------------------------------------------------------
__fastcall TFormExportaInfoCircuito::TFormExportaInfoCircuito(TComponent* Owner, VTApl* apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	// this->apl = stk->NewObjApl(NULL, aplOwner);
	this->apl = NewObjApl(NULL, apl_owner); // hk4.0
	apl->Add(calcIndCont = NewObjCalcIndCont(apl, NULL, true));
	// NULL));//flagVis = true -->executa fluxPot
	calcIndCont->LeArquivosMdb("");
}

// ---------------------------------------------------------------------------
__fastcall TFormExportaInfoCircuito::~TFormExportaInfoCircuito(void)
{
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormExportaInfoCircuito::Button1Click(TObject *Sender)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	strDadoParamRede* paramRede;
	AnsiString dIni, dFim, aux;
	double tmaBT;
	strResRede *resRede;
	AnsiString arq;
	FILE *fout;
	double taux;

	arq = path->DirTmp() + "\\infoRede.txt";
	fout = fopen(arq.c_str(), "a");
	// Atualiza grid de parametros por rede

	for (int np = 0; np < calcIndCont->ParamConf->LisParamRede->Count; np++)          //hk201808: reformula
	{
		paramRede = (strDadoParamRede*)calcIndCont->ParamConf->LisParamRede->Items[np];//hk201808: reformula
		if (paramRede->rede == NULL)
			continue;
		if (!paramRede->rede->Carregada)
			continue;
		resRede = calcIndCont->ResCont->ResRede[paramRede->rede];//hk201808: reformula

		fprintf(fout, "%s; ", resRede->rede->Codigo.c_str());
		fprintf(fout, "%i; ", resRede->ncons);
		fprintf(fout, "%6.2f; ", paramRede->compRede);
		fprintf(fout, "%6.2f; ", paramRede->lambda);
		if (paramRede->trep < 0.0)
			taux = 0.0;
		else
			taux = paramRede->trep;
		fprintf(fout, "%6.2f\n", taux);

	}
	fclose(fout);

}
// ---------------------------------------------------------------------------
