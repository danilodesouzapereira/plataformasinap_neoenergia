//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "TFormCompensacoes.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
#include "VTCompensacoes.h"
#include <DLL_Inc\Radial.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Radial\VTPrimario.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Rede\VTCarga.h>
#include <DLL_Inc\Funcao.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Patamar\VTPatamares.h>

//---------------------------------------------------------------------------
__fastcall TFormCompensacoes::TFormCompensacoes(TComponent* Owner, VTApl *apl, TWinControl *parent, AnsiString empresa)
									  : TFormEdt(Owner, parent)
	{
	try{
		DefParent(parent);
		janela.top    = 0;
		janela.left   = 0;
		janela.width  = Width;
		janela.height = Height;
		}
	catch(Exception &e)
		{
		}
	this->apl = apl;
	PageControl->ActivePageIndex = 0;
	TabSheetResultados->TabVisible = false;

	Compensacoes = NewObjCompensacoes(Owner, apl, empresa);
	VTPath* Path = (VTPath*)apl->GetObject(__classid(VTPath));
//	if(!Compensacoes->Inicializa(Path->DirDat() + "\\" + empresa + "\\"))
//		this->Enabled = false;

	VTRedes* redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCargas = new TList;
	redes->LisEqbar(lisCargas, eqptoCARGA);
	for(int i = 0; i < lisCargas->Count; i++)
		{
			VTCarga* carga = (VTCarga*)lisCargas->Items[i];
			ComboBoxCargas->AddItem(carga->Codigo, carga);
		}

	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	if(!radial)
		{
		radial = DLL_NewObjRadial(apl);
		apl->Add(radial);
		radial->Inicia(redes);
		}
	for(int i = 0; i < radial->LisSubestacao()->Count; i++)
		{
		VTSubestacao *se = (VTSubestacao*)radial->LisSubestacao()->Items[i];
		ComboBoxSubestacoes->AddItem(se->Rede->Codigo, se);
		}
	}

//---------------------------------------------------------------------------
__fastcall TFormCompensacoes::~TFormCompensacoes(void)
	{
	delete Compensacoes;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCompensacoes::ButtonCalculaClick(TObject *Sender)
	{
	double DICsMensais[12];
	DICsMensais[0]  = StrToDouble(EditDICJan->Text);
	DICsMensais[1]  = StrToDouble(EditDICFev->Text);
	DICsMensais[2]  = StrToDouble(EditDICMar->Text);
	DICsMensais[3]  = StrToDouble(EditDICAbr->Text);
	DICsMensais[4]  = StrToDouble(EditDICMai->Text);
	DICsMensais[5]  = StrToDouble(EditDICJun->Text);
	DICsMensais[6]  = StrToDouble(EditDICJul->Text);
	DICsMensais[7]  = StrToDouble(EditDICAgo->Text);
	DICsMensais[8]  = StrToDouble(EditDICSet->Text);
	DICsMensais[9]  = StrToDouble(EditDICOut->Text);
	DICsMensais[10] = StrToDouble(EditDICNov->Text);
	DICsMensais[11] = StrToDouble(EditDICDez->Text);

	double FICsMensais[12];
	FICsMensais[0]  = StrToDouble(EditFICJan->Text);
	FICsMensais[1]  = StrToDouble(EditFICFev->Text);
	FICsMensais[2]  = StrToDouble(EditFICMar->Text);
	FICsMensais[3]  = StrToDouble(EditFICAbr->Text);
	FICsMensais[4]  = StrToDouble(EditFICMai->Text);
	FICsMensais[5]  = StrToDouble(EditFICJun->Text);
	FICsMensais[6]  = StrToDouble(EditFICJul->Text);
	FICsMensais[7]  = StrToDouble(EditFICAgo->Text);
	FICsMensais[8]  = StrToDouble(EditFICSet->Text);
	FICsMensais[9]  = StrToDouble(EditFICOut->Text);
	FICsMensais[10] = StrToDouble(EditFICNov->Text);
	FICsMensais[11] = StrToDouble(EditFICDez->Text);

	if(ComboBoxSubestacoes->ItemIndex < 0)
		return;
	VTSubestacao *se = (VTSubestacao*)ComboBoxSubestacoes->Items->Objects[ComboBoxSubestacoes->ItemIndex];
	if(ComboBoxCargas->ItemIndex < 0)
		return;
	VTCarga *carga = (VTCarga*)ComboBoxCargas->Items->Objects[ComboBoxCargas->ItemIndex];
	double compTot = Compensacoes->Calcula(se, carga, DICsMensais, FICsMensais);

	EditCompJan->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[0]);
	EditCompFev->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[1]);
	EditCompMar->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[2]);
	EditCompAbr->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[3]);
	EditCompMai->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[4]);
	EditCompJun->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[5]);
	EditCompJul->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[6]);
	EditCompAgo->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[7]);
	EditCompSet->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[8]);
	EditCompOut->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[9]);
	EditCompNov->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[10]);
	EditCompDez->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesMensais[11]);

	EditComp1Tri->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesTrimestrais[0]);
	EditComp2Tri->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesTrimestrais[1]);
	EditComp3Tri->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesTrimestrais[2]);
	EditComp4Tri->Text = DoubleToStr("%5.2f", Compensacoes->CompensacoesTrimestrais[3]);

	EditCompAnu->Text = DoubleToStr("%5.2f", Compensacoes->CompensacaoAnual);

	EditCompTotal->Text = DoubleToStr("%5.2f", Compensacoes->CompensacaoTotal);

	TabSheetResultados->TabVisible = true;
	PageControl->ActivePageIndex = 1;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCompensacoes::ComboBoxSubestacoesChange(TObject *Sender)
	{
	VTSubestacao   *se       = (VTSubestacao*)ComboBoxSubestacoes->Items->Objects[ComboBoxSubestacoes->ItemIndex];
	TConjuntoAneel *conjunto = Compensacoes->RetornaConjuntoAneel(se);
	strFaixa       *faixaDEC = Compensacoes->RetornaFaixaDEC(conjunto);
	strFaixa       *faixaFEC = Compensacoes->RetornaFaixaFEC(conjunto);

	EditMtUrbAnuDIC->Text = DoubleToStr("%3.2f", faixaDEC->Urbano->MT->DIC->Anual);
	EditMtUrbTriDIC->Text = DoubleToStr("%3.2f", faixaDEC->Urbano->MT->DIC->Trimestral);
	EditMtUrbMenDIC->Text = DoubleToStr("%3.2f", faixaDEC->Urbano->MT->DIC->Mensal);
	EditMtUrbDMIC->Text   = DoubleToStr("%3.2f", faixaDEC->Urbano->MT->DMIC);

	EditMtRurAnuDIC->Text = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->MT->DIC->Anual);
	EditMtRurTriDIC->Text = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->MT->DIC->Trimestral);
	EditMtRurMenDIC->Text = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->MT->DIC->Mensal);
	EditMtRurDMIC->Text   = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->MT->DMIC);

	EditBtUrbAnuDIC->Text = DoubleToStr("%3.2f", faixaDEC->Urbano->BT->DIC->Anual);
	EditBtUrbTriDIC->Text = DoubleToStr("%3.2f", faixaDEC->Urbano->BT->DIC->Trimestral);
	EditBtUrbMenDIC->Text = DoubleToStr("%3.2f", faixaDEC->Urbano->BT->DIC->Mensal);
	EditBtUrbDMIC->Text   = DoubleToStr("%3.2f", faixaDEC->Urbano->BT->DMIC);

	EditBtRurAnuDIC->Text = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->BT->DIC->Anual);
	EditBtRurTriDIC->Text = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->BT->DIC->Trimestral);
	EditBtRurMenDIC->Text = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->BT->DIC->Mensal);
	EditBtRurDMIC->Text   = DoubleToStr("%3.2f", faixaDEC->NaoUrbano->BT->DMIC);

	EditMtUrbAnuFIC->Text = DoubleToStr("%3.2f", faixaFEC->Urbano->MT->FIC->Anual);
	EditMtUrbTriFIC->Text = DoubleToStr("%3.2f", faixaFEC->Urbano->MT->FIC->Trimestral);
	EditMtUrbMenFIC->Text = DoubleToStr("%3.2f", faixaFEC->Urbano->MT->FIC->Mensal);

	EditMtRurAnuFIC->Text = DoubleToStr("%3.2f", faixaFEC->NaoUrbano->MT->FIC->Anual);
	EditMtRurTriFIC->Text = DoubleToStr("%3.2f", faixaFEC->NaoUrbano->MT->FIC->Trimestral);
	EditMtRurMenFIC->Text = DoubleToStr("%3.2f", faixaFEC->NaoUrbano->MT->FIC->Mensal);

	EditBtUrbAnuFIC->Text = DoubleToStr("%3.2f", faixaFEC->Urbano->BT->FIC->Anual);
	EditBtUrbTriFIC->Text = DoubleToStr("%3.2f", faixaFEC->Urbano->BT->FIC->Trimestral);
	EditBtUrbMenFIC->Text = DoubleToStr("%3.2f", faixaFEC->Urbano->BT->FIC->Mensal);

	EditBtRurAnuFIC->Text = DoubleToStr("%3.2f", faixaFEC->NaoUrbano->BT->FIC->Anual);
	EditBtRurTriFIC->Text = DoubleToStr("%3.2f", faixaFEC->NaoUrbano->BT->FIC->Trimestral);
	EditBtRurMenFIC->Text = DoubleToStr("%3.2f", faixaFEC->NaoUrbano->BT->FIC->Mensal);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCompensacoes::ComboBoxCargasChange(TObject *Sender)
{
	VTCarga *carga = (VTCarga*)ComboBoxCargas->Items->Objects[ComboBoxCargas->ItemIndex];

	EditConsRes->Text = IntToStr(carga->NumConsResidencial);
	EditConsCom->Text = IntToStr(carga->NumConsComercial);
	EditConsInd->Text = IntToStr(carga->NumConsIndustrial);
	EditConsRur->Text = IntToStr(carga->NumConsRural);
	EditConsOut->Text = IntToStr(carga->NumConsOutros);
	EditConsA4->Text  = IntToStr(carga->NumConsA4);

	VTPatamares	*patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int numPat = patamares->NumPatamar();
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	double EnergiaMensal = 0.0;
	for(int np = 0; np < numPat; np++)
		EnergiaMensal += demanda->P_mw[carga][np] * 1000.;
	EnergiaMensal *= 30.;
	EditEnergia->Text = DoubleToStr("%5.2f", EnergiaMensal);
}

//---------------------------------------------------------------------------
//eof

