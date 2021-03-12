// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtMotor.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTMotor.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtMotor::TFormEdtMotor(TComponent* Owner, VTApl *apl, VTMotor* motor) : TForm(Owner) {
	// salva ponteiros p/ objetos
	this->apl = apl;
	this->motor = motor;
	// cria lista
	lisVETVAL = new TList();
	lisEQP = new TList();
	// cria objetos
	patamares = DLL_NewObjPatamares(apl);
	legenda = DLL_NewObjLegenda(apl);
	// posiciona o Form
	FormIniciaPosicao();
	IniciaVleMotor();
	IniciaCmbTipoMotor();

}

// ---------------------------------------------------------------------------
__fastcall TFormEdtMotor::~TFormEdtMotor(void) { // vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destr�i objetos criados
	if (legenda) {
		delete legenda;
		legenda = NULL;
	}
	if (patamares) {
		delete patamares;
		patamares = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisEQP) {
		delete lisEQP;
		lisEQP = NULL;
	}
	// destr�i listas e seus obejtos
	if (lisVETVAL) {
		LimpaTList(lisVETVAL);
		delete lisVETVAL;
		lisVETVAL = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::ActionCancelaExecute(TObject *Sender) {
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::ActionConfirmaExecute(TObject *Sender) {
	// l� dados do Eqpto

	if (!LeDadosMotor())
		return;

	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::ActionRetiraExecute(TObject *Sender) {
	// fecha o form indicando exclus�o do Eqpto
	ModalResult = mrNo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::ExibeDadosMotor(void) {
	// prote��o: verifica se o Motor est� inclu�do
	if (motor == NULL)
		return;
	// l� dados do Motor
	EditCodigo->Text = motor->Codigo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::FormIniciaPosicao(void) {
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral) { // l� par�metros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::FormShow(TObject *Sender) {
	// exibe dados do Motor e da Chave
	ExibeDadosMotor();
	// posiciona o Cursor em EditCodigo
	EditCodigo->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::IniciaCmbTipoMotor(void) {

	// inicia as propriedades do comboBox cmbTipoMotor;
	cmbTipoMotor->Items->Add("Gaiola");
	cmbTipoMotor->Items->Add("Bobinado");

	if (motor->TipoMotor == tmGAIOLA || motor->TipoMotor == tmBOBINADO) {
		cmbTipoMotor->Text = cmbTipoMotor->Items->Strings[motor->TipoMotor];
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::IniciaVleMotor(void) {

	// inicia as propriedade de vleMotor com os campos de VTMotor;
	vleMotor->InsertRow("Resist�ncia do estator (r1) [pu]", motor->r1_pu, true);
	vleMotor->InsertRow("Reat�ncia do estator (x1) [pu]", motor->x1_pu, true);
	vleMotor->InsertRow("Resist�ncia do rotor (r2) [pu]", motor->r2_pu, true);
	vleMotor->InsertRow("Reat�ncia do rotor (x2) [pu]", motor->x2_pu, true);
	vleMotor->InsertRow("Resist�ncia de perdas no ferro (rp) [pu]", motor->rp_pu, true);
	vleMotor->InsertRow("Reat�ncia de magnetiza��o (xm) [pu]", motor->xm_pu, true);
	vleMotor->InsertRow("Varia��o da resist�ncia r2 na partida [pu de r2]", motor->dr2_s1_pur2, true);
	vleMotor->InsertRow("Varia��o da reat�ncia x2 na partida [pu de x2]", motor->dx2_s1_pux2, true);
	vleMotor->InsertRow("N�mero de polos", motor->num_polos, true);
	vleMotor->InsertRow("Pot�ncia mec�nica nominal [kW]", motor->pmec_nom_kW, true);
	vleMotor->InsertRow("Tens�o de linha nominal [V]", motor->vnom_V, true);
	vleMotor->InsertRow("Frequ�ncia [Hz]", motor->freq_Hz, true);
	vleMotor->InsertRow("Escorregamento nominal [pu]", motor->s100_pu, true);
	vleMotor->InsertRow("Corrente nominal [A]", motor->inom_A, true);
	vleMotor->InsertRow("Corrente de partida [A]", motor->ipart_A, true);
	vleMotor->InsertRow("Fator de pot�ncia a plena carga [pu]", motor->fpot100_pu, true);
	vleMotor->InsertRow("Fator de pot�ncia a 75% de carga [pu]", motor->fpot75_pu, true);
	vleMotor->InsertRow("Fator de pot�ncia a 50% de carga [pu]", motor->fpot50_pu, true);
	vleMotor->InsertRow("Rendimento a plena carga [pu]", motor->rend100_pu, true);
	vleMotor->InsertRow("Rendimento a 75% de carga [pu]", motor->rend75_pu, true);
	vleMotor->InsertRow("Rendimento a 50% de carga [pu]", motor->rend50_pu, true);
	vleMotor->InsertRow("Conjugado de partida [Nm]", motor->conj_part_Nm, true);
	vleMotor->InsertRow("Conjugado m�ximo [Nm]", motor->conj_max_Nm, true);
	vleMotor->InsertRow("Momento de in�rcia [kgm2]", motor->J_kgm2, true);
	vleMotor->InsertRow("Momento de in�rcia da carga [kgm2]", motor->J_carga_kgm2, true);
	vleMotor->InsertRow("Conjugado nominal da carga [Nm]", motor->conj_carga_nom_Nm, true);
	vleMotor->InsertRow("Conjugado de partida da carga [Nm]", motor->conj_carga_part_Nm, true);

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtMotor::LeDadosMotor(void) {

	// Vari�veis locais
	AnsiString tipo_motor;

	// prote��o: verifica se o Motor est� inclu�do
	if (motor == NULL)
		return (true);
	try { // l� dados do Motor
		motor->Codigo = EditCodigo->Text;
		tipo_motor = cmbTipoMotor->Items->Strings[cmbTipoMotor->ItemIndex];
		motor->r1_pu = StrToDouble(vleMotor->Values["Resist�ncia do estator (r1) [pu]"]);
		motor->x1_pu = StrToDouble(vleMotor->Values["Reat�ncia do estator (x1) [pu]"]);
		motor->r2_pu = StrToDouble(vleMotor->Values["Resist�ncia do rotor (r2) [pu]"]);
		motor->x2_pu = StrToDouble(vleMotor->Values["Reat�ncia do rotor (x2) [pu]"]);
		motor->rp_pu = StrToDouble(vleMotor->Values["Resist�ncia de perdas no ferro (rp) [pu]"]);
		motor->xm_pu = StrToDouble(vleMotor->Values["Reat�ncia de magnetiza��o (xm) [pu]"]);
		motor->dr2_s1_pur2 = StrToDouble(vleMotor->Values["Varia��o da resist�ncia r2 na partida [pu de r2]"]);
		motor->dx2_s1_pux2 = StrToDouble(vleMotor->Values["Varia��o da reat�ncia x2 na partida [pu de x2]"]);
		motor->num_polos = StrToInt(vleMotor->Values["N�mero de polos"]);
		motor->pmec_nom_kW = StrToDouble(vleMotor->Values["Pot�ncia mec�nica nominal [kW]"]);
		motor->vnom_V = StrToDouble(vleMotor->Values["Tens�o de linha nominal [V]"]);
		motor->freq_Hz = StrToDouble(vleMotor->Values["Frequ�ncia [Hz]"]);
		motor->s100_pu = StrToDouble(vleMotor->Values["Escorregamento nominal [pu]"]);
		motor->inom_A = StrToDouble(vleMotor->Values["Corrente nominal [A]"]);
		motor->ipart_A = StrToDouble(vleMotor->Values["Corrente de partida [A]"]);
		motor->fpot100_pu = StrToDouble(vleMotor->Values["Fator de pot�ncia a plena carga [pu]"]);
		motor->fpot75_pu = StrToDouble(vleMotor->Values["Fator de pot�ncia a 75% de carga [pu]"]);
		motor->fpot50_pu = StrToDouble(vleMotor->Values["Fator de pot�ncia a 50% de carga [pu]"]);
		motor->rend100_pu = StrToDouble(vleMotor->Values["Rendimento a plena carga [pu]"]);
		motor->rend75_pu = StrToDouble(vleMotor->Values["Rendimento a 75% de carga [pu]"]);
		motor->rend50_pu = StrToDouble(vleMotor->Values["Rendimento a 50% de carga [pu]"]);
		motor->conj_part_Nm = StrToDouble(vleMotor->Values["Conjugado de partida [Nm]"]);
		motor->conj_max_Nm = StrToDouble(vleMotor->Values["Conjugado m�ximo [Nm]"]);
		motor->J_kgm2 = StrToDouble(vleMotor->Values["Momento de in�rcia [kgm2]"]);
		motor->J_carga_kgm2 = StrToDouble(vleMotor->Values["Momento de in�rcia da carga [kgm2]"]);
		motor->conj_carga_nom_Nm = StrToDouble(vleMotor->Values["Conjugado nominal da carga [Nm]"]);
		motor->conj_carga_part_Nm = StrToDouble(vleMotor->Values["Conjugado de partida da carga [Nm]"]);

		// define tipo do motor
		if (tipo_motor.AnsiCompareIC("bobinado") == 0) {
			motor->TipoMotor = tmBOBINADO;
		}
		else if (tipo_motor.AnsiCompareIC("gaiola") == 0) {
			motor->TipoMotor = tmGAIOLA;
		}
		else {
			motor->TipoMotor = tmBOBINADO;
		}

	}
	catch (Exception &e) {
		Erro("Dados Inv�lidos do Motor");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::OpcaoRetira(bool enabled) {
	butRetira->Visible = enabled;
}
// ---------------------------------------------------------------------------
