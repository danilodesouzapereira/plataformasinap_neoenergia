//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFormCfgGraf.h"
#include "VTCfgGrafConfiab.h"
#include "..\..\DLL_Inc\Funcao.h"   
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormCfgGraf::TFormCfgGraf(TComponent* Owner, VTCfgGrafConfiab *cfg)
	: TForm(Owner)
	{
	//salva ponteiro
	this->cfg = cfg;

	//inicia configuração
	switch(cfg->index)
   	{
			case eDEC: butDec->Checked = true; break;
			case eFEC: butFec->Checked = true; break;
			case eEND: butEnd->Checked = true; break;
			case ePOT: butPot->Checked = true; break;
			default: break;
		}
  //copia valores
  //dec
  dec1->Text = DoubleToStr("%.2f", cfg->dec.limite1);
  dec2->Text = DoubleToStr("%.2f", cfg->dec.limite2);
  //fec
  fec1->Text = DoubleToStr("%.2f", cfg->fec.limite1);
  fec2->Text = DoubleToStr("%.2f", cfg->fec.limite2);
  //end
  end1->Text = DoubleToStr("%.2f", cfg->end.limite1);
  end2->Text = DoubleToStr("%.2f", cfg->end.limite2);
  //pot
  pot1->Text = DoubleToStr("%.2f", cfg->pot.limite1);
  pot2->Text = DoubleToStr("%.2f", cfg->pot.limite2);
  }
//---------------------------------------------------------------------------
__fastcall TFormCfgGraf::~TFormCfgGraf(void)
	{
	//nada a fazer
	}
//---------------------------------------------------------------------------

void __fastcall TFormCfgGraf::ActionCancelarExecute(TObject *Sender)
	{
	ModalResult = mrCancel;
	}
//---------------------------------------------------------------------------

void __fastcall TFormCfgGraf::ActionConfirmarExecute(TObject *Sender)
	{
	if(! LeDados()) return;
	ModalResult = mrOk;;
	}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgGraf::LeDados(void)
	{
	if(butDec->Checked) cfg->index = eDEC;
	if(butFec->Checked) cfg->index = eFEC;
	if(butEnd->Checked) cfg->index = eEND;
	if(butPot->Checked) cfg->index = ePOT;

	try
		{
		//dec
		cfg->dec.limite1 = fabs(StrToDouble(dec1->Text));
		cfg->dec.limite2 = fabs(StrToDouble(dec2->Text));
		//fec
		cfg->fec.limite1 = fabs(StrToDouble(fec1->Text));
		cfg->fec.limite2 = fabs(StrToDouble(fec2->Text));
		//end
		cfg->end.limite1 = fabs(StrToDouble(end1->Text));
		cfg->end.limite2 = fabs(StrToDouble(end2->Text));
		//pot
		cfg->pot.limite1 = fabs(StrToDouble(pot1->Text));
		cfg->pot.limite2 = fabs(StrToDouble(pot2->Text));  

		if((cfg->dec.limite1 >= cfg->dec.limite2) ||
			(cfg->fec.limite1 >= cfg->fec.limite2) ||
			(cfg->end.limite1 >= cfg->end.limite2) ||
			(cfg->pot.limite1 >= cfg->pot.limite2))
			{
				Aviso("Limite inválido !");
				return false; 
         }
			
		}              
		catch(...)
			{
				Aviso("Valor inválido !");
				return false;
			}
	return true;
	}                                             
//---------------------------------------------------------------------------

