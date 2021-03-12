//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop    
#include "TFormEditaCurvaCarga.h"
#include "TFormEditaCurvaCarga3F.h"
#include "TFormEditaCurva.h"
#include "TFormExibeCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamares.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Curvas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormEditaCurvaCarga::TFormEditaCurvaCarga(TComponent* Owner, VTApl* apl, VTCurva* curva)
	: TForm(Owner)
	{ //DVK 2014.01.18
	//salva ponteiros
	this->apl = apl;
	this->curva_orig = curva;
	//inicia dados default p/ Eqpto novo
	IniciaDadosCargaNova();
	//cria TFormEditaCurva p/ edição da Curva
	FormEditaCurvaCarga3F = new TFormEditaCurvaCarga3F(NULL, this, apl, TabSheetEdicao);
//	FormEditaCurvaCarga3F->DefineCurva(this->curva.propria);
//	FormEditaCurvaCarga3F->Show();

//   FormEditaCurva = new TFormEditaCurva(this, apl, TabSheetEdicao);
//   FormEditaCurva->InsereColuna(indP, "P(MW)");
//   FormEditaCurva->InsereColuna(indQ, "Q(MVAr)");
//	FormEditaCurva->DefineCurva(this->curva.propria);
//	FormEditaCurva->Show();
	}

//---------------------------------------------------------------------------
__fastcall TFormEditaCurvaCarga::~TFormEditaCurvaCarga(void)
	{
   //destrói objetos criados
   if (curva.propria) {delete curva.propria; curva.propria = NULL;}
	//destrói Forms
	if (FormEditaCurvaCarga3F) {delete FormEditaCurvaCarga3F; FormEditaCurvaCarga3F = NULL;}
//	if (FormExibeCurva) {delete FormExibeCurva; FormExibeCurva = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaCarga::ActionCancelaExecute(TObject *Sender)
	{
	//fecha
	ModalResult = mrCancel;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaCarga::ActionConfirmaExecute(TObject *Sender)
	{
	//lê dados da Curva própria
	if (! FormEditaCurvaCarga3F->LePontosCurva()) return;
	ModalResult = mrOk;  
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TFormEditaCurvaCarga::Curva(void)
	{
	return(this->curva.propria);
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TFormEditaCurvaCarga::CriaCurva(void)
   {
   //variáveis locais
   VTCurva *curva;

   //cria Curva
   curva                = DLL_NewObjCurva();
   curva->Tipica        = false;
   curva->Escala        = patEDITAVEL;
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->NumeroValores = 2;
   curva->Reconfigura(patEDITAVEL);
   return(curva);
	}

//---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaCarga::FormClose(TObject *Sender,
		TCloseAction &Action)
	{
	Action = caHide;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaCarga::IniciaDadosCargaNova(void)
	{
	//variáveis locais
	strHM      hm_ini, hm_fim;
   double     valor[nvCURVA_CAR];
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//cria curva
	curva.propria = CriaCurva();
	for(int np=0; (np < patamares->NumPatamar()); np++)
		{//obtém ponto da Curva p/ o Patamar
		if (! curva_orig->GetPonto(np, hm_ini, hm_fim, valor, curva_orig->NumeroValores)) break;
		//copia pontos para curva.propria
		if (! curva.propria->SetPonto(hm_ini, hm_fim, valor, curva_orig->NumeroValores)) break;
		}   
	}

//---------------------------------------------------------------------------  
void __fastcall TFormEditaCurvaCarga::PageControlChange(TObject *Sender)
	{  /*
	//verifica se está selecionado visualização da Curva
	if (PageControl->ActivePage == TabSheetGrafico)
		{//atualiza FormExibeCurva
		FormExibeCurva->DefineCurva(this->curva.propria);
		FormExibeCurva->HabilitaCurva(car_P);
		FormExibeCurva->HabilitaCurva(car_Q);
		}  */
	}

//---------------------------------------------------------------------------

