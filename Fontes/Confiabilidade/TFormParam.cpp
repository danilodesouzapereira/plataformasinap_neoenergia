//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormParam.h"
#include "TEqptoMutua.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Apl\VTApl.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipos.h"
#include "..\Confiabilidade\VTDataEqpto.h"
#include "..\Confiabilidade\VTDadoConfiab.h"
#include "..\Confiabilidade\VTConfiab.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormParam::TFormParam(TComponent* Owner, TWinControl *parent, VTApl *apl)
		  : TForm(Owner)
	{
	this->apl = apl;
	sgridParamInicia(sgridParamTipo);
	sgridParamInicia(sgridParamTrecho);
	sgridParamInicia(sgridParamChave);
	sgridParamInicia(sgridParamTrafo);
	sgridParamInicia(sgridParamGerador);
	sgridParamInicia(sgridParamEqpto); sgridParamEqpto->Cells[0][0] = "Código";
   //hk201711mutua
	sgridParamInicia(sgridParamEqptoMutua); sgridParamEqptoMutua->Cells[0][0] = "Código";
	sgridParamInicia(sgridParamBloco); sgridParamBloco->Cells[0][0] = "Código";

//	this->Width = sgridParamTipo->Width + 16;
	sgridParamTipo->Align = alClient;
	sgridParamTrecho->Align = alClient;
	sgridParamChave->Align = alClient;
	sgridParamTrafo->Align = alClient;
	sgridParamGerador->Align = alClient;
	sgridParamEqpto->Align = alClient;
   //hk201711mutua
   sgridParamEqptoMutua->Align = alClient;
	sgridParamBloco->Align = alClient;
	//Atualiza();


	}
//---------------------------------------------------------------------------
__fastcall TFormParam::~TFormParam(void)
	{
	//nada a fazer
   }
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionCancelaExecute(TObject *Sender)
	{
   ModalResult = mrCancel;
	//Close();
   }
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionDisableProtExecute(TObject *Sender)
	{
	for (int n = 0; n < CLBoxProt->Items->Count; n++)
		CLBoxProt->Checked[n] = false;
	//IniciaProt();
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionEnableProtExecute(TObject *Sender)
	{
	for (int n = 0; n < CLBoxProt->Items->Count; n++)
		CLBoxProt->Checked[n] = true;
	//IniciaProt();
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionConfirmaExecute(TObject *Sender)
	{
	VTConfiab*     confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	//VTDadoConfiab* dataConfiab = confiab->DadoConfiab();
	//hk 10 VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	PAR_CONF pconf;
	TList* lisData;
	VTDataEqpto* dataEqpto;
	TStringGrid* sgrid;

	try
		{
		pconf.tx_falha  = StrToDouble(sgridParamTipo->Cells[1][1]);
		pconf.treparo   = StrToDouble(sgridParamTipo->Cells[2][1]);
		pconf.tx_interr = StrToDouble(sgridParamTipo->Cells[3][1]);
		pconf.tman      = StrToDouble(sgridParamTipo->Cells[4][1]);
		pconf.fat_isola = 1.0;
		pconf.comp      = 1.0;
		dataConfiab->DefParamGeralTrecho(pconf);
		dataConfiab->DefParamGeralTipo(pconf, eqptoTRECHO);

		pconf.tx_falha  = StrToDouble(sgridParamTipo->Cells[1][2]);
		pconf.treparo   = StrToDouble(sgridParamTipo->Cells[2][2]);
		pconf.tx_interr = StrToDouble(sgridParamTipo->Cells[3][2]);
		pconf.tman      = StrToDouble(sgridParamTipo->Cells[4][2]);
		pconf.fat_isola = 1.0;
		pconf.comp      = 1.0;
		dataConfiab->DefParamGeralEqpto(pconf);
		dataConfiab->DefParamGeralTipo(pconf, eqptoGERADOR);

		pconf.tx_falha  = StrToDouble(sgridParamTipo->Cells[1][3]);
		pconf.treparo   = StrToDouble(sgridParamTipo->Cells[2][3]);
		pconf.tx_interr = StrToDouble(sgridParamTipo->Cells[3][3]);
		pconf.tman      = StrToDouble(sgridParamTipo->Cells[4][3]);
		pconf.fat_isola = 1.0;
		pconf.comp      = 1.0;
		dataConfiab->DefParamGeralTipo(pconf, eqptoTRAFO);

		pconf.tx_falha  = StrToDouble(sgridParamTipo->Cells[1][4]);
		pconf.treparo   = StrToDouble(sgridParamTipo->Cells[2][4]);
		pconf.tx_interr = StrToDouble(sgridParamTipo->Cells[3][4]);
		pconf.tman      = StrToDouble(sgridParamTipo->Cells[4][4]);
		pconf.fat_isola = 1.0;
		pconf.comp      = 1.0;
		dataConfiab->DefParamGeralTipo(pconf, eqptoCHAVE);

		pconf.tx_falha  = StrToDouble(sgridParamTipo->Cells[1][5]);
		pconf.treparo   = StrToDouble(sgridParamTipo->Cells[2][5]);
		pconf.tx_interr = StrToDouble(sgridParamTipo->Cells[3][5]);
		pconf.tman      = StrToDouble(sgridParamTipo->Cells[4][5]);
		pconf.fat_isola = 1.0;
		pconf.comp      = 1.0;
		dataConfiab->DefParamGeralTipo(pconf, eqptoBARRA);

		//Atualiza dados particulares
		//Atualiza grid com taxas de falha por cabos por trecho
		lisData = dataConfiab->LisDataTipoTrecho;
		sgrid = sgridParamTrecho;
		for(int n = 1; n < sgrid->RowCount; n++)
			{
			if(sgrid->Objects[0][n] == NULL) continue;
			dataEqpto = (VTDataEqpto*)(sgrid->Objects[0][n]);
			pconf.tx_falha  = StrToDouble(sgrid->Cells[1][n]);
			pconf.treparo   = StrToDouble(sgrid->Cells[2][n]);
			pconf.tx_interr = StrToDouble(sgrid->Cells[3][n]);
			pconf.tman      = StrToDouble(sgrid->Cells[4][n]);
			pconf.fat_isola = 1.0;
			pconf.comp      = 1.0;
			dataEqpto->pconf = pconf;
			}

		lisData = dataConfiab->LisDataTipoChave;
		sgrid = sgridParamChave;
		for(int n = 1; n < sgrid->RowCount; n++)
			{
			if(sgrid->Objects[0][n] == NULL) continue;
			dataEqpto = (VTDataEqpto*)(sgrid->Objects[0][n]);
			pconf.tx_falha  = StrToDouble(sgrid->Cells[1][n]);
			pconf.treparo   = StrToDouble(sgrid->Cells[2][n]);
			pconf.tx_interr = StrToDouble(sgrid->Cells[3][n]);
			pconf.tman      = StrToDouble(sgrid->Cells[4][n]);
			pconf.fat_isola = 1.0;
			pconf.comp      = 1.0;
			dataEqpto->pconf = pconf;
			}

		lisData = dataConfiab->LisDataTipoTrafo;
		sgrid = sgridParamTrafo;
		for(int n = 1; n < sgrid->RowCount; n++)
			{
			if(sgrid->Objects[0][n] == NULL) continue;
			dataEqpto = (VTDataEqpto*)(sgrid->Objects[0][n]);
			pconf.tx_falha  = StrToDouble(sgrid->Cells[1][n]);
			pconf.treparo   = StrToDouble(sgrid->Cells[2][n]);
			pconf.tx_interr = StrToDouble(sgrid->Cells[3][n]);
			pconf.tman      = StrToDouble(sgrid->Cells[4][n]);
			pconf.fat_isola = 1.0;
			pconf.comp      = 1.0;
			dataEqpto->pconf = pconf;
			}

		lisData = dataConfiab->LisDataTipoGerador;
		sgrid = sgridParamGerador;
		for(int n = 1; n < sgrid->RowCount; n++)
			{
			if(sgrid->Objects[0][n] == NULL) continue;
			dataEqpto = (VTDataEqpto*)(sgrid->Objects[0][n]);
			pconf.tx_falha  = StrToDouble(sgrid->Cells[1][n]);
			pconf.treparo   = StrToDouble(sgrid->Cells[2][n]);
			pconf.tx_interr = StrToDouble(sgrid->Cells[3][n]);
			pconf.tman      = StrToDouble(sgrid->Cells[4][n]);
			pconf.fat_isola = 1.0;
			pconf.comp      = 1.0;
			dataEqpto->pconf = pconf;
			}

		sgrid = sgridParamEqpto;
		for(int n = 1; n < sgrid->RowCount; n++)
			{
			if(sgrid->Objects[0][n] == NULL) continue;
			dataEqpto = (VTDataEqpto*)(sgrid->Objects[0][n]);
			pconf.tx_falha  = StrToDouble(sgrid->Cells[1][n]);
			pconf.treparo   = StrToDouble(sgrid->Cells[2][n]);
			pconf.tx_interr = StrToDouble(sgrid->Cells[3][n]);
			pconf.tman      = StrToDouble(sgrid->Cells[4][n]);
			pconf.fat_isola = 1.0;
			pconf.comp      = 1.0;
			dataEqpto->pconf = pconf;
			}

      //hk201711mutua
		sgrid = sgridParamEqptoMutua;
		for(int n = 1; n < sgrid->RowCount; n++)
			{
			if(sgrid->Objects[0][n] == NULL) continue;
			dataEqpto = (VTDataEqpto*)(sgrid->Objects[0][n]);
			pconf.tx_falha  = StrToDouble(sgrid->Cells[1][n]);
			pconf.treparo   = StrToDouble(sgrid->Cells[2][n]);
			pconf.tx_interr = StrToDouble(sgrid->Cells[3][n]);
			pconf.tman      = StrToDouble(sgrid->Cells[4][n]);
			pconf.fat_isola = 1.0;
			pconf.comp      = 1.0;
			dataEqpto->pconf = pconf;
			}

		//Guarda parametros para o calculo de confiabilidade
		dataConfiab->icont   = cbxOrdem->ItemIndex;
		dataConfiab->flag_NA = chkNA->Checked;

		//Reinicia lista de tipos de chave de protecao
		IniciaProt();

		//Reinicia flag de agrupamento de falhas por bloco
      dataConfiab->flag_BLOCO = chkBloco->Checked;
		}
	catch(Exception &e)
		{
		Erro(e.Message);
		return;
		}
	ModalResult = mrOk;
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionIncluirTipoTrechoExecute(TObject *Sender)
	{
	VTCabo      *cabo;
	PAR_CONF    pconf;
	VTDataEqpto  *dataEqpto;
	TList       *lisAux;
	AnsiString  aux;
	TListItem   *item;


	if(lbTipoTrecho->ItemIndex < 0) return;

	cabo = (VTCabo*)lbTipoTrecho->Items->Objects[lbTipoTrecho->ItemIndex];
	if(!dataConfiab->ParamTipoTrecho(pconf, cabo))
		{dataConfiab->ParamGeralTipo(pconf, eqptoTRECHO);
		}
	if(lbTipoTrecho->Checked[lbTipoTrecho->ItemIndex])
		{
		dataEqpto = dataConfiab->DefParamGeralTipoTrecho(pconf, cabo);
		//aux = ((VTEqpto*)(dataEqpto->eqpto_aux))->Codigo;
		//AdicionaReg(sgridParamTrecho, dataEqpto->pconf, aux, dataEqpto);
		}
	else
		{
		for(int n = 0; n < dataConfiab->LisDataTipoTrecho->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)dataConfiab->LisDataTipoTrecho->Items[n];
			if(dataEqpto->eqpto_aux == cabo)
				{
				dataConfiab->LisDataTipoTrecho->Remove(dataEqpto);
				delete dataEqpto;
				break;
				}
			}
		}

	Atualiza(dataConfiab);


	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionMostraEqptoExecute(TObject *Sender)
	{
	VTGrafico  *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TMetaClass *obj_class;
	AnsiString  obj_name;
	VTEqpto    *eqpto;
   TList      *lisAux;

   if(lbEqptoMutua->SelCount == 0) return;

   lisAux = new TList();
	for(int n = 0; n < lbEqptoMutua->Items->Count; n++)
	   {
		if (!lbEqptoMutua->Selected[n])
			continue;
		lisAux->Add(lbEqptoMutua->Items->Objects[n]);
	   }
   grafico->Moldura(lisAux);

   delete lisAux;



	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionInsereMutuaExecute(TObject *Sender)
   {
	PAR_CONF    pconf;
	VTDataEqpto  *dataEqpto;
	TList       *lisAux;
	AnsiString  aux;
	TListItem   *item;
   VTEqpto     *eqpto1, *eqpto2;

   //hk201711mutua: busca eqpto1 e eqpto2 em lbEqptoMutua
   //cria em dataConfiab->lisEqptoMutua novo eqptoMutua
   //cria em dataConfiab novo lisDataEqpto->dataEqpto que aponta para eqptoMutua
   if(lbEqptoMutua->SelCount != 2) return;

   lisAux = new TList();
	for(int n = 0; n < lbEqptoMutua->Items->Count; n++)
	   {
		if (!lbEqptoMutua->Selected[n])
			continue;
		lisAux->Add(lbEqptoMutua->Items->Objects[n]);
	   }
   eqpto1 = (VTEqpto*)lisAux->Items[0];
   eqpto2 = (VTEqpto*)lisAux->Items[1];
   delete lisAux;

   TEqptoMutua *eqptoMutua = NULL;

   for(int n = 0; n < dataConfiab->LisEqptoMutua->Count; n++)
      {
      eqptoMutua = (TEqptoMutua*)dataConfiab->LisEqptoMutua->Items[n];
      if((eqptoMutua->eqpto1 == eqpto1)&&(eqptoMutua->eqpto2 == eqpto2)) break;
      if((eqptoMutua->eqpto1 == eqpto2)&&(eqptoMutua->eqpto2 == eqpto1)) break;
      eqptoMutua = NULL;
      }
   if(eqptoMutua == NULL)
      {
      dataConfiab->LisEqptoMutua->Add(eqptoMutua = new TEqptoMutua());
      eqptoMutua->eqpto1 = eqpto1;
      eqptoMutua->eqpto2 = eqpto2;

		aux.sprintf("(%s)", eqpto1->Codigo.c_str());
		aux.cat_sprintf("(%s)", eqpto2->Codigo.c_str());
      eqptoMutua->Codigo = aux;
      dataConfiab->ParamEqpto(pconf, eqpto1);
      dataConfiab->DefParamEqpto(pconf, eqptoMutua);
      }

	Atualiza(dataConfiab);
   }
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionIncluirTipoGeradorExecute(TObject *Sender)
	{
	PAR_CONF pconf;
	VTDataEqpto  *dataEqpto;
	TList       *lisAux;
	AnsiString  aux;
	TListItem   *item;
	double      vmin, vmax;

	if(lbTipoGerador->ItemIndex < 0) return;

	if(lbTipoGerador->ItemIndex == 0) {vmin = 0.0; vmax = 1.0;}
	if(lbTipoGerador->ItemIndex == 1) {vmin = 1.0; vmax = 10.0;}
	if(lbTipoGerador->ItemIndex == 2) {vmin = 10.0; vmax = 999.0;}


	if(!dataConfiab->ParamTipoGerador(pconf, (vmin + vmax)/2))
		{dataConfiab->ParamGeralTipo(pconf, eqptoGERADOR);
		}

	if(lbTipoGerador->Checked[lbTipoGerador->ItemIndex])
		{
		dataConfiab->DefParamGeralTipoGerador(pconf, vmin, vmax);
		//aux = ((VTEqpto*)(dataEqpto->eqpto_aux))->Codigo;
		//AdicionaReg(sgridParamTrecho, dataEqpto->pconf, aux, dataEqpto);
		}
	else
		{
		for(int n = 0; n < dataConfiab->LisDataTipoGerador->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)dataConfiab->LisDataTipoGerador->Items[n];
			if((dataEqpto->vmin == vmin)&&(dataEqpto->vmax == vmax))
				{
				dataConfiab->LisDataTipoGerador->Remove(dataEqpto);
				delete dataEqpto;
				break;
				}
			}
		}

	Atualiza(dataConfiab);

	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionIncluirTipoTrafoExecute(TObject *Sender)
	{
	PAR_CONF pconf;
	VTDataEqpto  *dataEqpto;
	TList       *lisAux;
	AnsiString  aux;
	TListItem   *item;
	double      vmin, vmax;

	if(lbTipoTrafo->ItemIndex < 0) return;

	if(lbTipoTrafo->ItemIndex == 0) {vmin = 0.0; vmax = 1.0;}
	if(lbTipoTrafo->ItemIndex == 1) {vmin = 1.0; vmax = 10.0;}
	if(lbTipoTrafo->ItemIndex == 2) {vmin = 10.0; vmax = 999.0;}


	if(!dataConfiab->ParamTipoTrafo(pconf, (vmin + vmax)/2))
		{dataConfiab->ParamGeralTipo(pconf, eqptoTRAFO);
		}

	if(lbTipoTrafo->Checked[lbTipoTrafo->ItemIndex])
		{
		dataConfiab->DefParamGeralTipoTrafo(pconf, vmin, vmax);
		//aux = ((VTEqpto*)(dataEqpto->eqpto_aux))->Codigo;
		//AdicionaReg(sgridParamTrafo, dataEqpto->pconf, aux, dataEqpto);
		}
	else
		{
		for(int n = 0; n < dataConfiab->LisDataTipoTrafo->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)dataConfiab->LisDataTipoTrafo->Items[n];
			if((dataEqpto->vmin == vmin)&&(dataEqpto->vmax == vmax))
				{
				dataConfiab->LisDataTipoTrafo->Remove(dataEqpto);
				delete dataEqpto;
				break;
				}
			}
		}

	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionIncluirTipoChaveExecute(TObject *Sender)
	{
	VTTipoChave *ptipoChave;
	VTCabo      *cabo;
	PAR_CONF    pconf;
	VTDataEqpto  *dataEqpto;
	TList       *lisAux;
	AnsiString  aux;
	TListItem   *item;


	if(lbTipoChave->ItemIndex < 0) return;

	ptipoChave = (VTTipoChave*)lbTipoChave->Items->Objects[lbTipoChave->ItemIndex];
	if(!dataConfiab->ParamTipoChave(pconf, ptipoChave))
		{dataConfiab->ParamGeralTipo(pconf, eqptoCHAVE);
		}
	if(lbTipoChave->Checked[lbTipoChave->ItemIndex])
		{
		dataConfiab->DefParamGeralTipoChave(pconf, ptipoChave);
		//aux = ((VTEqpto*)(dataEqpto->eqpto_aux))->Codigo;
		//AdicionaReg(sgridParamTrecho, dataEqpto->pconf, aux, dataEqpto);
		}
	else
		{
		for(int n = 0; n < dataConfiab->LisDataTipoChave->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)dataConfiab->LisDataTipoChave->Items[n];
			if(dataEqpto->eqpto_aux == ptipoChave)
				{
				dataConfiab->LisDataTipoChave->Remove(dataEqpto);
				delete dataEqpto;
				break;
				}
			}
		}

	Atualiza(dataConfiab);

	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionEnableTipoGeradorExecute(TObject *Sender)
	{
	PAR_CONF pconf;
	double vmin, vmax;

	for(int n = 0; n < lbTipoGerador->Items->Count; n++)
		{
		if(!lbTipoGerador->Checked[n])

		if(n == 0) {vmin = 0.0; vmax = 1.0;}
		if(n == 1) {vmin = 1.0; vmax = 10.0;}
		if(n == 2) {vmin = 10.0; vmax = 999.0;}
		if(!dataConfiab->ParamTipoGerador(pconf, (vmin + vmax)/2))
			{dataConfiab->ParamGeralTipo(pconf, eqptoGERADOR);
			}

		dataConfiab->DefParamGeralTipoGerador(pconf, vmin, vmax);
		}
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionEnableTipoTrafoExecute(TObject *Sender)
	{
	PAR_CONF pconf;
	double vmin, vmax;
	
	for(int n = 0; n < lbTipoTrafo->Items->Count; n++)
		{
		if(!lbTipoTrafo->Checked[n])

		if(n == 0) {vmin = 0.0; vmax = 1.0;}
		if(n == 1) {vmin = 1.0; vmax = 10.0;}
		if(n == 2) {vmin = 10.0; vmax = 999.0;}
		if(!dataConfiab->ParamTipoTrafo(pconf, (vmin + vmax)/2))
			{dataConfiab->ParamGeralTipo(pconf, eqptoTRAFO);
			}

		dataConfiab->DefParamGeralTipoTrafo(pconf, vmin, vmax);
		}
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionEnableTipoChaveExecute(TObject *Sender)
	{
	VTTipoChave *ptipoChave;
	PAR_CONF pconf;

	for(int n = 0; n < lbTipoChave->Items->Count; n++)
		{
		if(lbTipoChave->Checked[n]) continue;
		ptipoChave = (VTTipoChave*)lbTipoChave->Items->Objects[n];
		if(!dataConfiab->ParamTipoChave(pconf, ptipoChave))
		  {dataConfiab->ParamGeralTipo(pconf, eqptoCHAVE);
			}
		dataConfiab->DefParamGeralTipoChave(pconf, ptipoChave);
		}
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionDisableTipoGeradorExecute(TObject *Sender)
	{
	LimpaTList(dataConfiab->LisDataTipoGerador);
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionDisableTipoTrafoExecute(TObject *Sender)
	{
	LimpaTList(dataConfiab->LisDataTipoTrafo);
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionDisableTipoChaveExecute(TObject *Sender)
	{
	LimpaTList(dataConfiab->LisDataTipoChave);
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionEnableTipoTrechoExecute(TObject *Sender)
	{
	VTCabo *cabo;
	PAR_CONF pconf;

	dataConfiab->ParamGeralTrecho(pconf);
	for(int n = 0; n < lbTipoTrecho->Items->Count; n++)
		{
		if(lbTipoTrecho->Checked[n]) continue;
		cabo = (VTCabo*)lbTipoTrecho->Items->Objects[n];
		if(!dataConfiab->ParamTipoTrecho(pconf, cabo))
		  {dataConfiab->ParamGeralTipo(pconf, eqptoTRECHO);
			}
		dataConfiab->DefParamGeralTipoTrecho(pconf, cabo);
		}
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionDisableTipoTrechoExecute(TObject *Sender)
	{
	LimpaTList(dataConfiab->LisDataTipoTrecho);
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionIncluirEqptoExecute(TObject *Sender)
	{
	VTEqpto     *eqpto;
	PAR_CONF    pconf;
	VTDataEqpto  *dataEqpto;
	TList       *lisAux;
	AnsiString  aux;
	TListItem   *item;


	if(lbEqpto->ItemIndex < 0) return;

	eqpto = (VTEqpto*)lbEqpto->Items->Objects[lbEqpto->ItemIndex];
	dataConfiab->ParamEqpto(pconf, eqpto);
	//dataConfiab->ParamGeralEqpto(pconf);
	if(lbEqpto->Checked[lbEqpto->ItemIndex])
		{ //if(!dataEqpto) lisDataEqpto->Add(dataEqpto = NewObjDataEqpto(eqpto));
		dataConfiab->DefParamEqpto(pconf, eqpto);
		}
	else
		{
		for(int n = 0; n < dataConfiab->LisDataEqpto->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)dataConfiab->LisDataEqpto->Items[n];
			if(dataEqpto->eqpto == eqpto)
				{
				dataConfiab->LisDataEqpto->Remove(dataEqpto);
				delete dataEqpto;
				break;
				}
			}
		}

	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionEnableEqptoExecute(TObject *Sender)
	{
	VTEqpto *eqpto;
	PAR_CONF pconf;

	//dataConfiab->ParamEqpto(pconf, eqpto);
	//dataConfiab->ParamGeralEqpto(pconf);
	for(int n = 0; n < lbEqpto->Items->Count; n++)
		{
		if(lbEqpto->Checked[n]) continue;
		eqpto = (VTEqpto*)lbEqpto->Items->Objects[n];
		dataConfiab->ParamEqpto(pconf, eqpto);
		dataConfiab->DefParamEqpto(pconf, eqpto);
		}
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::ActionDisableEqptoExecute(TObject *Sender)
	{
	LimpaTList(dataConfiab->LisDataEqpto);
	Atualiza(dataConfiab);
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::AdicionaReg(TStringGrid* sgrid, PAR_CONF pconf, AnsiString aux, TObject* data)
	{
	int iaux;
	sgrid->RowCount++;
	if(sgrid->RowCount == 2) sgrid->FixedRows = 1;
	iaux = sgrid->RowCount - 1;
	sgrid->Objects[0][iaux] = data;
	sgrid->Cells[0][iaux] = aux;
	sgrid->Cells[1][iaux] = pconf.tx_falha;
	sgrid->Cells[2][iaux] = pconf.treparo;
	sgrid->Cells[3][iaux] = pconf.tx_interr;
	sgrid->Cells[4][iaux] = pconf.tman;
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::Atualiza(VTDadoConfiab* dataConfiab)
	{
	//VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	VTConfiab  *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTDataEqpto* dataEqpto;
	AnsiString  aux;
	VTCabo     *pcabo;
	PAR_CONF pconf;
	VTEqpto    *eqpto;
	TList       *lisDataEqpto, *lisEqptos, *lisBlocos, *lisCabos, *lisAux, *lisDataTipoTrecho;
	VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos     *tipos  = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();
	TList* lisDataTipoChave = dataConfiab->LisDataTipoChave;
	VTTipoChave *ptipoChave;




	this->dataConfiab = dataConfiab;
	//VTDadoConfiab* dataConfiab = confiab->DadoConfiab();


	//Limpa sgridParamTipo
	sgridParamTipo->RowCount    = 1;
	sgridParamTrecho->RowCount  = 1;
	sgridParamChave->RowCount   = 1;
	sgridParamTrafo->RowCount   = 1;
	sgridParamGerador->RowCount = 1;
	sgridParamEqpto->RowCount   = 1;
	sgridParamBloco->RowCount   = 1;
   //hk201711mutua
	sgridParamEqptoMutua->RowCount   = 1;

	//Para cada tipo de cabo do trecho associar parametro



	//Trecho
	if(!dataConfiab->ParamGeralTipo(pconf, eqptoTRECHO))
	  {if(!dataConfiab->ParamGeralTrecho(pconf)) {
																pconf.tx_falha  = 0.;
																pconf.tx_interr = 0.;
																pconf.treparo   = 0.;
																pconf.tman      = 0.;
																}
	  }
	AdicionaReg(sgridParamTipo, pconf, "TRECHO*");

	VTDadoConfiab *dataConfAux;
	//Trechos por bloco
	for(int n = 0; n < dataConfiab->LisDataBloco->Count; n++)
		{
		dataConfAux = (VTDadoConfiab*)dataConfiab->LisDataBloco->Items[n];
		dataConfAux->ParamGeralTrecho(pconf);
		AdicionaReg(sgridParamBloco, pconf, IntToStr(n), (TObject*)dataConfAux->Bloco);
		}


	//Gerador
	if(!dataConfiab->ParamGeralTipo(pconf, eqptoGERADOR))
	  {if(!dataConfiab->ParamGeralEqpto(pconf)) {
																pconf.tx_falha  = 0.;
																pconf.tx_interr = 0.;
																pconf.treparo   = 0.;
																pconf.tman      = 0.;
																}
	  }
	AdicionaReg(sgridParamTipo, pconf, "GERADOR**");

	//Trafo
	if(!dataConfiab->ParamGeralTipo(pconf, eqptoTRAFO))
	  {if(!dataConfiab->ParamGeralEqpto(pconf)) {
																pconf.tx_falha  = 0.;
																pconf.tx_interr = 0.;
																pconf.treparo   = 0.;
																pconf.tman      = 0.;
																}
	  }
	AdicionaReg(sgridParamTipo, pconf, "TRAFO**");
	//Chave
	if(!dataConfiab->ParamGeralTipo(pconf, eqptoCHAVE))
	  {if(!dataConfiab->ParamGeralEqpto(pconf)) {
																pconf.tx_falha  = 0.;
																pconf.tx_interr = 0.;
																pconf.treparo   = 0.;
																pconf.tman      = 0.;
																}
	  }
	AdicionaReg(sgridParamTipo, pconf, "CHAVE**");

	if(!dataConfiab->ParamGeralTipo(pconf, eqptoBARRA))
	  {if(!dataConfiab->ParamGeralEqpto(pconf)) {
																pconf.tx_falha  = 0.;
																pconf.tx_interr = 0.;
																pconf.treparo   = 0.;
																pconf.tman      = 0.;
																}
	  }
	AdicionaReg(sgridParamTipo, pconf, "BARRA**");

	//Atualiza grid com taxas de falha por cabos por trecho
	lisDataTipoTrecho = dataConfiab->LisDataTipoTrecho;
	lisCabos = arranjos->LisCabo();
	lisAux = new TList();
	lisAux->Assign(lisCabos, laOr);
	for(int n = 0; n < lisCabos->Count; n++)
		{
		pcabo = (VTCabo*)lisCabos->Items[n];
		dataEqpto = NULL;
		for(int n = 0; n < lisDataTipoTrecho->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)lisDataTipoTrecho->Items[n];
			if(dataEqpto->eqpto_aux == pcabo) break;
			dataEqpto = NULL;
			}
		if(dataEqpto == NULL) continue;
		aux = ((VTCabo*)dataEqpto->eqpto_aux)->Codigo;
		AdicionaReg(sgridParamTrecho, dataEqpto->pconf, aux, (TObject*)dataEqpto);
		lisAux->Remove(dataEqpto->eqpto_aux);
		}

	//Atualiza grid com taxas de falha por cabos por trecho
	lbTipoTrecho->Clear();
	for(int n = 0; n < lisCabos->Count; n++)
		{
		pcabo = (VTCabo*)lisCabos->Items[n];
		lbTipoTrecho->AddItem(pcabo->Codigo, pcabo);
		lbTipoTrecho->Checked[n] = (lisAux->IndexOf(pcabo) == -1);
		}

	//Parametros especificos por blocos
	lbBlocos->Clear();
	lisBlocos = confiab->Blocos->LisBloco();
	lisAux->Clear();
	for(int n = 0; n < dataConfiab->LisDataBloco->Count; n++)
		{
		lisAux->Add(((VTDadoConfiab*)dataConfiab->LisDataBloco->Items[n])->Bloco);
		}

	VTBloco *bloco;
	for(int n = 0; n < lisBlocos->Count; n++)
		{
		bloco = (VTBloco*)lisBlocos->Items[n];
		lbBlocos->AddItem("Bloco: " + IntToStr(n), (TObject*)bloco);
		lbBlocos->Checked[n] = (lisAux->IndexOf(bloco) == -1);
		}

	//Atualiza grid com taxas de falha por equipamento
	lisDataEqpto = dataConfiab->LisDataEqpto;
	lisEqptos = new TList();
	redes->LisEqpto(lisEqptos, eqptoLIGACAO);
	redes->LisEqpto(lisEqptos, eqptoBARRA);

	lisAux->Clear();
	lisAux->Assign(lisEqptos, laOr);
	for(int n = 0; n < lisEqptos->Count; n++)
		{
		eqpto = (VTEqpto*)lisEqptos->Items[n];
		dataEqpto = NULL;
		for(int n = 0; n < lisDataEqpto->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
			if(dataEqpto->eqpto == eqpto) break;
			dataEqpto = NULL;
			}
		if(dataEqpto == NULL) continue;
		aux = (eqpto->Tipo() == eqptoBARRA) ? "Bar:" : "Liga:";
		aux.cat_sprintf("%s", eqpto->Codigo.c_str());
		AdicionaReg(sgridParamEqpto, dataEqpto->pconf, aux, (TObject*)dataEqpto);
		lisAux->Remove(dataEqpto->eqpto);
		}

   //hk201711mutua
   VTEqpto *eqptoMutua;
   for(int n = 0; n < dataConfiab->LisEqptoMutua->Count; n++)
      {
      eqptoMutua = (VTEqpto*)dataConfiab->LisEqptoMutua->Items[n];
		dataEqpto = NULL;
		for(int n = 0; n < lisDataEqpto->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
			if(dataEqpto->eqpto == eqptoMutua) break;
			dataEqpto = NULL;
			}
		if(dataEqpto == NULL) continue;

		aux.sprintf("(%s)", ((TEqptoMutua*)eqptoMutua)->eqpto1->Codigo.c_str());
		aux.cat_sprintf("(%s)", ((TEqptoMutua*)eqptoMutua)->eqpto2->Codigo.c_str());
		AdicionaReg(sgridParamEqptoMutua, dataEqpto->pconf, aux, (TObject*)dataEqpto);
      }

	lbEqpto->Clear();
	for(int n = 0; n < lisEqptos->Count; n++)
		{
		eqpto = (VTEqpto*)lisEqptos->Items[n];
		aux = (eqpto->Tipo() == eqptoBARRA) ? "Bar:" : "Liga:";
		aux.cat_sprintf("%s", eqpto->Codigo.c_str());
		lbEqpto->AddItem(aux, eqpto);
		lbEqpto->Checked[n] = (lisAux->IndexOf(eqpto) == -1);
		}
   //hk201711mutua
   lbEqptoMutua->Clear();
	for(int n = 0; n < lisEqptos->Count; n++)
		{
		eqpto = (VTEqpto*)lisEqptos->Items[n];
		aux = (eqpto->Tipo() == eqptoBARRA) ? "Bar:" : "Liga:";
		aux.cat_sprintf("%s", eqpto->Codigo.c_str());
		lbEqptoMutua->AddItem(aux, eqpto);
		}

	delete lisEqptos;
	//Atualiza grid com taxas de falha por equipamento



	//Atualiza grid com taxas de falha por tipo de chave

	lisAux->Clear();
	lisAux->Assign(lisTIPO, laOr);
	for(int nt = 0; nt < lisTIPO->Count; nt++)
		{
		ptipoChave = (VTTipoChave*)lisTIPO->Items[nt];
		dataEqpto = NULL;
		for(int n = 0; n < lisDataTipoChave->Count; n++)
			{
			dataEqpto = (VTDataEqpto*)lisDataTipoChave->Items[n];
			if(dataEqpto->eqpto_aux == ptipoChave) break;
			dataEqpto = NULL;
			}
		if(dataEqpto == NULL) continue;

		aux = ptipoChave->Codigo;
		AdicionaReg(sgridParamChave, dataEqpto->pconf, aux, (TObject*)dataEqpto);
		lisAux->Remove(dataEqpto->eqpto_aux);
		}

	lbTipoChave->Clear();
	for(int n = 0; n < lisTIPO->Count; n++)
		{
		ptipoChave = (VTTipoChave*)lisTIPO->Items[n];
		aux = ptipoChave->Codigo;
		lbTipoChave->AddItem(aux, ptipoChave);
		lbTipoChave->Checked[n] = (lisAux->IndexOf(ptipoChave) == -1);
		}

	//Atualiza grid com taxas de falha por tipo de trafo
	lisDataEqpto = dataConfiab->LisDataTipoTrafo;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		aux.sprintf("%2.0f < snom (MVA) < %3.0f", dataEqpto->vmin, dataEqpto->vmax);
		AdicionaReg(sgridParamTrafo, dataEqpto->pconf, aux, (TObject*)dataEqpto);
		}

	lbTipoTrafo->Clear();
	aux.sprintf("%2.0f < snom (MVA) < %3.0f", 0.0, 1.0);
	lbTipoTrafo->AddItem(aux, NULL);
	dataEqpto = NULL;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		if((dataEqpto->vmin == 0.0)&&(dataEqpto->vmax == 1.0)) break;
		dataEqpto = NULL;
		}
	lbTipoTrafo->Checked[lbTipoTrafo->Items->Count - 1] = (dataEqpto != NULL);

	aux.sprintf("%2.0f < snom (MVA) < %3.0f", 1.0, 10.0);
	lbTipoTrafo->AddItem(aux, NULL);
	dataEqpto = NULL;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		if((dataEqpto->vmin == 1.0)&&(dataEqpto->vmax == 10.0)) break;
		dataEqpto = NULL;
		}
	lbTipoTrafo->Checked[lbTipoTrafo->Items->Count - 1] = (dataEqpto != NULL);

	aux.sprintf("%2.0f < snom (MVA) < %3.0f", 10.0, 999.0);
	lbTipoTrafo->AddItem(aux, NULL);
	dataEqpto = NULL;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		if((dataEqpto->vmin == 10.0)&&(dataEqpto->vmax == 999.0)) break;
		dataEqpto = NULL;
		}
	lbTipoTrafo->Checked[lbTipoTrafo->Items->Count - 1] = (dataEqpto != NULL);

	//Atualiza grid com taxas de falha por tipo de gerador
	//Insere dados de geradores
	lisDataEqpto = dataConfiab->LisDataTipoGerador;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		aux.sprintf("%2.0f < snom (MVA) < %3.0f", dataEqpto->vmin, dataEqpto->vmax);
		AdicionaReg(sgridParamGerador, dataEqpto->pconf, aux, (TObject*)dataEqpto);
		}

	lbTipoGerador->Clear();
	aux.sprintf("%2.0f < snom (MVA) < %3.0f", 0.0, 1.0);
	lbTipoGerador->AddItem(aux, NULL);
	dataEqpto = NULL;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		if((dataEqpto->vmin == 0.0)&&(dataEqpto->vmax == 1.0)) break;
		dataEqpto = NULL;
		}
	lbTipoGerador->Checked[lbTipoGerador->Items->Count - 1] = (dataEqpto != NULL);

	aux.sprintf("%2.0f < snom (MVA) < %3.0f", 1.0, 10.0);
	lbTipoGerador->AddItem(aux, NULL);
	dataEqpto = NULL;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		if((dataEqpto->vmin == 1.0)&&(dataEqpto->vmax == 10.0)) break;
		dataEqpto = NULL;
		}
	lbTipoGerador->Checked[lbTipoGerador->Items->Count - 1] = (dataEqpto != NULL);

	aux.sprintf("%2.0f < snom (MVA) < %3.0f", 10.0, 999.0);
	lbTipoGerador->AddItem(aux, NULL);
	dataEqpto = NULL;
	for(int n = 0; n < lisDataEqpto->Count; n++)
		{
		dataEqpto = (VTDataEqpto*)lisDataEqpto->Items[n];
		if((dataEqpto->vmin == 10.0)&&(dataEqpto->vmax == 999.0)) break;
		dataEqpto = NULL;
		}
	lbTipoGerador->Checked[lbTipoGerador->Items->Count - 1] = (dataEqpto != NULL);
	//Atualiza box com parametros de calculo
	cbxOrdem->ItemIndex = dataConfiab->icont;
	chkNA->Checked      = dataConfiab->flag_NA;
	//Atualiza combo com tipos de chave utilizados
	CLBoxProtInicia();
	delete lisAux;

	//Atualiza checkBox - agregação de blocos
	chkBloco->Checked = dataConfiab->flag_BLOCO;

	}

//---------------------------------------------------------------------------
void __fastcall TFormParam::CLBoxProtInicia(void)
	{
	//variáveis locais
	//VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	VTConfiab*     confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	//VTDadoConfiab* dataConfiab = confiab->DadoConfiab();
	VTTipoChave *tipochave;
	//VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();

	//reinicia CLBoxPatamar
	CLBoxProt->Clear();
	//loop p/ todos TipoChave
	for (int n = 0; n < lisTIPO->Count; n++)
		{
		tipochave = (VTTipoChave*)lisTIPO->Items[n];
		CLBoxProt->Items->AddObject(tipochave->Codigo, tipochave);
		CLBoxProt->Checked[n] = (dataConfiab->lisTipoChv->IndexOf(tipochave) != -1);
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::CLBoxProtClick(TObject *Sender)
	{
	//Inicia tipos de equipamentos de protecao
	IniciaProt();
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::FormClose(TObject *Sender,
		TCloseAction &Action)
	{
	//Action = caFree;
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::IniciaProt(void)
	{
	//VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	VTConfiab*     confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	//VTDadoConfiab* dataConfiab = confiab->DadoConfiab();
	VTTipoChave *tipochave;

	dataConfiab->lisTipoChv->Clear();
	for(int n = 0; n < CLBoxProt->Items->Count; n++)
		{
		if(!CLBoxProt->Checked[n]) continue;
		tipochave = (VTTipoChave*)(CLBoxProt->Items->Objects[n]);
		dataConfiab->lisTipoChv->Add(tipochave);
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormParam::sgridParamInicia(TStringGrid* sgrid)
	{
	sgrid->RowCount = 2;
	sgrid->ColCount = 5;
	sgrid->Width = 0;
	sgrid->Align = alNone;
	for(int n = 0; n < sgrid->ColCount; n++)
										 {sgrid->ColWidths[n] = 120;
										  sgrid->Width += sgrid->ColWidths[n];
										  }

	sgrid->Cells[0][0] = "TIPO";
	sgrid->Cells[1][0] = "falhas/un.ano";
	sgrid->Cells[2][0] = "h/reparo";
	sgrid->Cells[3][0] = "interrupções/un.ano";
	sgrid->Cells[4][0] = "h/interrupção";
	}
//---------------------------------------------------------------------------







