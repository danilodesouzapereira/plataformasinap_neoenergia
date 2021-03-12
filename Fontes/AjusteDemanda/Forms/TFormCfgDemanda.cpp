// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgDemanda.h"
#include "VTAjusta.h"
#include "..\Constante\Const.h"
#include "..\Flow\VTFlow.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCfgDemanda::TFormCfgDemanda(TComponent* Owner, VTAjusta *ajusta_demanda)
	: TForm(Owner)
{
	// salva ponteiro do objeto ajusta
	ajusta = ajusta_demanda;
	// inicia variavel
	opFluxoAlterado = false;
	// salva op��o original do fluxo
	incluir_perda_orig = ajusta_demanda->IncluirPerda;
	tipo_fluxo_orig = ajusta_demanda->TipoFluxo;
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgDemanda::~TFormCfgDemanda(void)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::ActionCancelarExecute(TObject *Sender)
{
	// cancela
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::ActionConfirmarExecute(TObject *Sender)
{
	// vari�veis locais
	double ajuste_max, erro_max, tempo;

	// valida valores de toler�ncia
	erro_max = ajuste_max = 0.;
	if ((CheckBoxLimite->Checked) && (!StrToDouble(CBoxAjusteMax->Text, ajuste_max)))
	{
		Aviso("Defina o valor para a diferen�a admiss�vel antes do ajuste");
		return;
	}
	if ((CheckBoxLimite->Checked) && (!StrToDouble(CBoxErroMax->Text, erro_max)))
	{
		Aviso("Defina o valor para a diferen�a admiss�vel depois do ajuste");
		return;
	}
	if ((CheckBoxTempo->Checked) && (!StrToDouble(CboxTempo->Text, tempo)))
	{
		Aviso("Defina o valor para o tempo m�ximo de execu��o do ajuste");
		return;
	}
	if ((CheckBoxTempo->Checked) && (IsDoubleZero(tempo)))
	{
		Aviso("Defina um valor n�o nulo para o tempo m�ximo de execu��o do ajuste");
		return;
	}
	//valida patamares selecionados
	if(! PatamaresSelecionados())
	{
		Aviso("Selecione, pelo menos, um patamar para o ajuste");
		return;
    }
	// if(! LeFatorAjusteUnico(ajuste_max)) return;
	// configura op��es de Ajusta
	ajusta->TipoFluxo = (CheckBoxRedeEquilibrada->Checked) ? fluxoEQUILIBRADO : fluxoAUTO;
	ajusta->IncluirConsA4 = CheckBoxConsA4->Checked;
	ajusta->IncluirPerda = CheckBoxPerdaFe->Checked;
	ajusta->LimitaAjuste = CheckBoxLimite->Checked;
	ajusta->AjusteMaximo = ajuste_max * 0.01;
	ajusta->ErroMaximo = erro_max * 0.01;
	if (CheckBoxTempo->Checked)
		ajusta->TempoMinuto = tempo * 60. * 1000.; // min => ms
	else
		ajusta->TempoMinuto = 0.;
	// verifica se houve altera��o das op��es do fluxo
	if (ajusta->TipoFluxo != tipo_fluxo_orig)
		opFluxoAlterado = true;
	if (ajusta->IncluirPerda != incluir_perda_orig)
		opFluxoAlterado = true;
	// guarda patamares habilitados
	for (int n = 0; n < pat01HORA; n++)
	{
		ajusta->Patamares[n] = CLBoxPatamares->Checked[n];
	}
	// fecha
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::ActionSelAllExecute(TObject *Sender)
{
	for (int n = 0; n < CLBoxPatamares->Items->Count; n++)
	{
		CLBoxPatamares->Checked[n] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::ActionSelNoneExecute(TObject *Sender)
{
	for (int n = 0; n < CLBoxPatamares->Items->Count; n++)
	{
		CLBoxPatamares->Checked[n] = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::CheckBoxLimiteClick(TObject *Sender)
{
	// redefine tamanho do form
	/*
	 if(CheckBoxLimite->Checked)
	 {
	 if(CheckBoxTempo->Checked)   this->Height = 170 + 50 + 50;
	 else                         this->Height = 170 + 50;
	 }
	 else
	 {
	 if(CheckBoxTempo->Checked)   this->Height = 170 + 50;
	 else                         this->Height = 170;
	 }
	 */
	// mostra/esconde limites do ajuste
	// GBoxLimite->Visible = CheckBoxLimite->Checked;
	CBoxAjusteMax->Enabled = CheckBoxLimite->Checked;
	CBoxErroMax->Enabled = CheckBoxLimite->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::CheckBoxTempoClick(TObject *Sender)
{
	// redefine tamanho do form
	/*
	 if(CheckBoxTempo->Checked)
	 {
	 if(CheckBoxLimite->Checked)  this->Height = 170 + 50 + 50;
	 else                         this->Height = 170 + 50;
	 }
	 else
	 {
	 if(CheckBoxLimite->Checked)  this->Height = 170 + 50;
	 else                         this->Height = 170;
	 }
	 */
	// mostra/esconde tempo do ajuste
	// GBoxTempo->Visible = CheckBoxTempo->Checked;
	CboxTempo->Enabled = CheckBoxTempo->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::FormClose(TObject *Sender, TCloseAction &Action)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgDemanda::FormShow(TObject *Sender)
{
	// variaveis locais
	// double fat_cor;

	// prote��o
	if (!ajusta)
	{
		Aviso("N�o existe objeto VTAjusta");
		return;
	}
	// configura op��es de Ajusta
	CheckBoxRedeEquilibrada->Checked = (ajusta->TipoFluxo == fluxoEQUILIBRADO);
	CheckBoxConsA4->Checked = ajusta->IncluirConsA4;
	CheckBoxLimite->Checked = ajusta->LimitaAjuste;
	CheckBoxPerdaFe->Checked = ajusta->IncluirPerda;
	CBoxAjusteMax->Text = DoubleToStr("%2.0f", ajusta->AjusteMaximo / 0.01, ',');
	CBoxErroMax->Text = DoubleToStr("%2.0f", ajusta->ErroMaximo / 0.01, ',');
	// simula click
	CheckBoxLimiteClick(NULL);
	CheckBoxTempoClick(NULL);
	// inicia patamares habilitados
	for (int n = 0; n < pat01HORA; n++)
	{
		CLBoxPatamares->Checked[n] = ajusta->Patamares[n];
	}
	// //verifica se � ajuste por medi��o ou fator �nico
	// if(IsDoubleZero(ajusta->FatorUnico)) RadioGroupTipo->ItemIndex = 0;
	// else
	// {
	// RadioGroupTipo->ItemIndex = 1;
	// fat_cor = (ajusta->FatorUnico - 1.) * 100.;
	// CBoxFatcor->Text            = DoubleToStr("%2.0f", fat_cor, '.');
	// }
	// RadioGroupTipoClick(NULL);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormCfgDemanda::LeFatorAjusteUnico(double ajuste_max)
 {
 //vari�veis locais
 double valor, fatcor_unico;

 //verifica se o ajsute � do tipo fator de corre��o
 if(RadioGroupTipo->ItemIndex == 0)
 {//medi��o: define fator 0
 ajusta->FatorUnico = 0.;
 return(true);
 }
 try{//reinicia fatcor_unico
 fatcor_unico = 0.;
 //verifica se foi definido valor para o fator �nico
 if (CBoxFatcor->Text.IsEmpty())
 {
 Aviso("Fator de corre��o n�o definido.");
 return(false);
 }
 //l� o fator �nico
 if(! StrToDouble(CBoxFatcor->Text, valor))
 {
 Aviso("Fator de corre��o com valor inv�lido.");
 return(false);
 }
 //verifica se foi definido valor nulo
 if (IsDoubleZero(valor))
 {
 //         return(Confirma("Fator de corre��o nulo", "O valor est� correto ?") == IDYES);
 Aviso("Fator de corre��o nulo.");
 return(false);
 }
 //verifica se foi definido valor negativo
 if (valor <= -100)
 {
 Aviso("Fator de corre��o negativo, inferior a -100%.");
 return(false);
 }
 //verifica se o fator �nico � compat�vel com ajuste m�ximo
 if (((ajuste_max > 0)&&(valor >= ajuste_max)) ||
 ((ajuste_max < 0)&&(valor <= ajuste_max)))
 {
 Aviso("O fator de corre��o � incompat�vel com o valor\n"
 "da diferen�a entre demanda da medi��o e demanda calculada\n"
 "antes do ajuste");
 return(false);
 }
 //converte valor de % para pu
 fatcor_unico = 1.0 + (valor * 0.01);
 ajusta->FatorUnico = fatcor_unico;
 }catch(Exception &e)
 {
 return(false);
 }
 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TFormCfgDemanda::PatamaresSelecionados(void)
{
	//verifica se h�, pelo menos, 1 patamar selecionado
	for(int n = 0; n < pat01HORA; n++)
	{
		if(CLBoxPatamares->Checked[n])
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------


