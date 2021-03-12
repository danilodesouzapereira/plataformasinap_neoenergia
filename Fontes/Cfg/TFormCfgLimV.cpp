//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormCfgLimV.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTTipoRede.h"
//---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Geral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCfgLimV *FormCfgLimV;
//---------------------------------------------------------------------------
__fastcall TFormCfgLimV::TFormCfgLimV(TComponent* Owner, VTApl *apl_owner)
	: TForm(Owner)
{
	this->apl =  apl_owner;
	geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	LeGeral();
}
//---------------------------------------------------------------------------
__fastcall TFormCfgLimV::~TFormCfgLimV(void)
{
	//teste
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::ActionCancelarExecute(TObject *Sender)
{
	LeGeral();
}
//---------------------------------------------------------------------------

void __fastcall TFormCfgLimV::ActionConfirmarExecute(TObject *Sender)
{
	SalvaGeral();
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::ActionDefaultExecute(TObject *Sender)
{
	//Seta os valores padrão
	edtLimInfBT->Text = 0.0;
	edtLimSupBT->Text = DoubleToStr("%5.3f",REDESEC_V_LIM_SUP_KV);
	edtLimInfMT->Text = DoubleToStr("%5.3f",REDESEC_V_LIM_SUP_KV);
	edtLimSupMT->Text = DoubleToStr("%5.3f",REDEPRI_V_LIM_SUP_KV);
	edtLimInfAT->Text = DoubleToStr("%5.3f",REDEPRI_V_LIM_SUP_KV);
	edtLimSupAT->Text = DoubleToStr("%5.3f",REDESUB_V_LIM_SUP_KV);

	ConvertBooleanToCombo(true,cboxLimInfBT);
	ConvertBooleanToCombo(true,cboxLimInfMT);
	ConvertBooleanToCombo(false,cboxLimInfAT);
	ConvertBooleanToCombo(false,cboxLimSupBT);
	ConvertBooleanToCombo(true,cboxLimSupMT);
    ConvertBooleanToCombo(true,cboxLimSupAT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::AjustaCampoIrmao(TComboBox *combo)
{
	TComboBox *comboIrmao;

	//verifica comobo
	if(combo == cboxLimSupBT)
		{comboIrmao = cboxLimInfMT;}
	else if(combo == cboxLimSupMT)
		{comboIrmao = cboxLimInfAT;}
	else if(combo == cboxLimInfMT)
		{comboIrmao = cboxLimSupBT;}
	else if(combo == cboxLimInfAT)
		{comboIrmao = cboxLimSupMT;}

	//seta valores
	ConvertBooleanToCombo( !ConvertComboToBoolean(combo) , comboIrmao);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::AjustaCampoIrmao(TEdit *edit)
{
	TEdit     *editIrmao;

	//verifica edit
	if(edit == edtLimSupBT)
		{editIrmao = edtLimInfMT;}
	else if(edit == edtLimSupMT)
		{editIrmao = edtLimInfAT;}
	else if(edit == edtLimInfMT)
		{editIrmao = edtLimSupBT;}
	else if(edit == edtLimInfAT)
		{editIrmao = edtLimSupMT;}

	//seta valores
	editIrmao->Text = edit->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::cboxLimInfATChange(TObject *Sender)
{
	AjustaCampoIrmao(cboxLimInfAT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::cboxLimInfMTChange(TObject *Sender)
{
	AjustaCampoIrmao(cboxLimInfMT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::cboxLimSupBTChange(TObject *Sender)
{
	AjustaCampoIrmao(cboxLimSupBT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::cboxLimSupMTChange(TObject *Sender)
{
	AjustaCampoIrmao(cboxLimSupMT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::ConvertBooleanToCombo(bool booleano, TComboBox *combo)
{
	if(booleano == true)
	{
		combo->ItemIndex = 0;
	}
	else
	{
    	combo->ItemIndex = 1;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgLimV::ConvertComboToBoolean(TComboBox *combo)
{
	bool resposta;

	if(combo->ItemIndex == 0)
		{resposta = true;}
	else if(combo->ItemIndex == 1)
		{resposta = false;}
	else
		{resposta = false;}

	return resposta;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::edtLimInfATChange(TObject *Sender)
{
	AjustaCampoIrmao(edtLimInfAT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::edtLimInfMTChange(TObject *Sender)
{
	AjustaCampoIrmao(edtLimInfMT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::edtLimSupBTChange(TObject *Sender)
{
	AjustaCampoIrmao(edtLimSupBT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::edtLimSupMTChange(TObject *Sender)
{
	AjustaCampoIrmao(edtLimSupMT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::IniciaComboBox(void)
{
	//preenche cboxLimInf
	IniciaComboBoxLimInf(cboxLimInfBT);
	IniciaComboBoxLimInf(cboxLimInfMT);
	IniciaComboBoxLimInf(cboxLimInfAT);
	//preenche cboxLimSup
	IniciaComboBoxLimSup(cboxLimSupBT);
	IniciaComboBoxLimSup(cboxLimSupMT);
	IniciaComboBoxLimSup(cboxLimSupAT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::IniciaComboBoxLimInf(TComboBox *combo)
{
	combo->Clear();
	combo->AddItem(textMAIOR,NULL);
	combo->AddItem(textMAIORIGUAL,NULL);
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::IniciaComboBoxLimSup(TComboBox *combo)
{

	combo->Clear();
	combo->AddItem(textMENOR, NULL);
	combo->AddItem(textMENORIGUAL, NULL);

}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::LeGeral(void)
{
	geral->LeLimitesTensao();

	edtLimInfBT->Text = DoubleToStr("%5.3f", geral->LIM_V.LIM_KV[0][0]);
	edtLimSupBT->Text = DoubleToStr("%5.3f", geral->LIM_V.LIM_KV[0][1]);
	edtLimInfMT->Text = DoubleToStr("%5.3f", geral->LIM_V.LIM_KV[1][0]);
	edtLimSupMT->Text = DoubleToStr("%5.3f", geral->LIM_V.LIM_KV[1][1]);
	edtLimInfAT->Text = DoubleToStr("%5.3f", geral->LIM_V.LIM_KV[2][0]);
	edtLimSupAT->Text = DoubleToStr("%5.3f", geral->LIM_V.LIM_KV[2][1]);
	//inicia combo
	IniciaComboBox();
	//verifica se os intervalos sao abertos
	//intervalos inferiores
	ConvertBooleanToCombo(geral->LIM_V.LIM_ISABERTO[0][0],cboxLimInfBT);
	ConvertBooleanToCombo(geral->LIM_V.LIM_ISABERTO[1][0],cboxLimInfMT);
	ConvertBooleanToCombo(geral->LIM_V.LIM_ISABERTO[2][0],cboxLimInfAT);
	ConvertBooleanToCombo(geral->LIM_V.LIM_ISABERTO[0][1],cboxLimSupBT);
	ConvertBooleanToCombo(geral->LIM_V.LIM_ISABERTO[1][1],cboxLimSupMT);
	ConvertBooleanToCombo(geral->LIM_V.LIM_ISABERTO[2][1],cboxLimSupAT);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgLimV::SalvaGeral(void)
{
	if(ValidaValores())
	{
		geral->LIM_V.LIM_KV[0][0] = StrToDouble(edtLimInfBT->Text);
		geral->LIM_V.LIM_KV[0][1] = StrToDouble(edtLimSupBT->Text);
		geral->LIM_V.LIM_KV[1][0] = StrToDouble(edtLimInfMT->Text);
		geral->LIM_V.LIM_KV[1][1] = StrToDouble(edtLimSupMT->Text);
		geral->LIM_V.LIM_KV[2][0] = StrToDouble(edtLimInfAT->Text);
		geral->LIM_V.LIM_KV[2][1] = StrToDouble(edtLimSupAT->Text);
		geral->LIM_V.LIM_ISABERTO[0][0] = ConvertComboToBoolean(cboxLimInfBT);
		geral->LIM_V.LIM_ISABERTO[0][1] = ConvertComboToBoolean(cboxLimSupBT);
		geral->LIM_V.LIM_ISABERTO[1][0] = ConvertComboToBoolean(cboxLimInfMT);
		geral->LIM_V.LIM_ISABERTO[1][1] = ConvertComboToBoolean(cboxLimSupMT);
		geral->LIM_V.LIM_ISABERTO[2][0] = ConvertComboToBoolean(cboxLimInfAT);
		geral->LIM_V.LIM_ISABERTO[2][1] = ConvertComboToBoolean(cboxLimSupAT);
	}
	geral->SalvaLimitesTensao();
}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgLimV::ValidaValores(void)
{
	//valida se os campos possuem numeros
	if(!ValidaValoresDouble(edtLimInfBT))
		{return false;}
	if(!ValidaValoresDouble(edtLimSupBT))
		{return false;}
	if(!ValidaValoresDouble(edtLimInfMT))
		{return false;}
	if(!ValidaValoresDouble(edtLimSupMT))
		{return false;}
	if(!ValidaValoresDouble(edtLimInfAT))
		{return false;}
	if(!ValidaValoresDouble(edtLimSupAT))
		{return false;}
	if(!VerificaCoerenciaLimites())
		{return false;}

	return true;

}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgLimV::ValidaValoresDouble(TEdit *edit)
{
	double valor;
	AnsiString edit_name ="";
	bool sucesso = false;

	//verifica qual tedit esta sendo analisado
	if(edit == edtLimInfBT)
		{edit_name = "limite inferior BT";}
	else if(edit == edtLimSupBT)
		{edit_name = "limite superior BT";}
	else if(edit == edtLimInfMT)
		{edit_name = "limite inferior MT";}
	else if(edit == edtLimSupMT)
		{edit_name = "limite superior MT";}
	else if(edit == edtLimInfAT)
		{edit_name = "limite inferior AT";}
	else if(edit == edtLimSupAT)
		{edit_name = "limite superior AT";}
	try
	{
		StrToDouble(edit->Text,valor);
		sucesso = true;
	}
	catch (Exception &e)
	{
		if(edit_name.IsEmpty())
			{Aviso("Valor inválido.");}
		else
		{
			Aviso("Valor do campo do " + edit_name +" é inválido.");
			edit->SetFocus();
			sucesso = false;
		}
	}
	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgLimV::VerificaCoerenciaLimites(void)
{
	double valores[3][2];
	bool   isaberto[3][2];

	valores[0][0] = StrToDouble(edtLimInfBT->Text);
	valores[0][1] = StrToDouble(edtLimSupBT->Text);
	valores[1][0] = StrToDouble(edtLimInfMT->Text);
	valores[1][1] = StrToDouble(edtLimSupMT->Text);
	valores[2][0] = StrToDouble(edtLimInfAT->Text);
	valores[2][1] = StrToDouble(edtLimSupAT->Text);
	isaberto[0][0] = ConvertComboToBoolean(cboxLimInfBT);
	isaberto[0][1] = ConvertComboToBoolean(cboxLimSupBT);
	isaberto[1][0] = ConvertComboToBoolean(cboxLimInfMT);
	isaberto[1][1] = ConvertComboToBoolean(cboxLimSupMT);
	isaberto[2][0] = ConvertComboToBoolean(cboxLimInfAT);
	isaberto[2][1] = ConvertComboToBoolean(cboxLimSupAT);

	//verifica os booleanos são coerentes
	if(isaberto[0][1] == isaberto[1][0])
		{return false;}
	if(isaberto[1][1] == isaberto[2][0])
		{return false;}
	//verifica se inferior é menor que superior
	if(valores[0][0] >  valores[0][1])
		{return false;}
	if(valores[1][0] >  valores[1][1])
		{return false;}
	if(valores[2][0] >  valores[2][1])
		{return false;}
	//os limites entre faixas devem ser iguais
	if(IsDoubleZero(valores[0][1] - valores[1][0]) == false)
		{return false;}
	if(IsDoubleZero(valores[1][1] - valores[2][0]) == false)
		{return false;}

	return true;
}
//---------------------------------------------------------------------------



