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
__fastcall TFormEdtMotor::~TFormEdtMotor(void) { // variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói objetos criados
	if (legenda) {
		delete legenda;
		legenda = NULL;
	}
	if (patamares) {
		delete patamares;
		patamares = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP) {
		delete lisEQP;
		lisEQP = NULL;
	}
	// destrói listas e seus obejtos
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
	// lê dados do Eqpto

	if (!LeDadosMotor())
		return;

	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::ActionRetiraExecute(TObject *Sender) {
	// fecha o form indicando exclusão do Eqpto
	ModalResult = mrNo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::ExibeDadosMotor(void) {
	// proteção: verifica se o Motor está incluído
	if (motor == NULL)
		return;
	// lê dados do Motor
	EditCodigo->Text = motor->Codigo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::FormIniciaPosicao(void) {
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral) { // lê parâmetros do Form
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
	vleMotor->InsertRow("Resistência do estator (r1) [pu]", motor->r1_pu, true);
	vleMotor->InsertRow("Reatância do estator (x1) [pu]", motor->x1_pu, true);
	vleMotor->InsertRow("Resistência do rotor (r2) [pu]", motor->r2_pu, true);
	vleMotor->InsertRow("Reatância do rotor (x2) [pu]", motor->x2_pu, true);
	vleMotor->InsertRow("Resistência de perdas no ferro (rp) [pu]", motor->rp_pu, true);
	vleMotor->InsertRow("Reatância de magnetização (xm) [pu]", motor->xm_pu, true);
	vleMotor->InsertRow("Variação da resistência r2 na partida [pu de r2]", motor->dr2_s1_pur2, true);
	vleMotor->InsertRow("Variação da reatância x2 na partida [pu de x2]", motor->dx2_s1_pux2, true);
	vleMotor->InsertRow("Número de polos", motor->num_polos, true);
	vleMotor->InsertRow("Potência mecânica nominal [kW]", motor->pmec_nom_kW, true);
	vleMotor->InsertRow("Tensão de linha nominal [V]", motor->vnom_V, true);
	vleMotor->InsertRow("Frequência [Hz]", motor->freq_Hz, true);
	vleMotor->InsertRow("Escorregamento nominal [pu]", motor->s100_pu, true);
	vleMotor->InsertRow("Corrente nominal [A]", motor->inom_A, true);
	vleMotor->InsertRow("Corrente de partida [A]", motor->ipart_A, true);
	vleMotor->InsertRow("Fator de potência a plena carga [pu]", motor->fpot100_pu, true);
	vleMotor->InsertRow("Fator de potência a 75% de carga [pu]", motor->fpot75_pu, true);
	vleMotor->InsertRow("Fator de potência a 50% de carga [pu]", motor->fpot50_pu, true);
	vleMotor->InsertRow("Rendimento a plena carga [pu]", motor->rend100_pu, true);
	vleMotor->InsertRow("Rendimento a 75% de carga [pu]", motor->rend75_pu, true);
	vleMotor->InsertRow("Rendimento a 50% de carga [pu]", motor->rend50_pu, true);
	vleMotor->InsertRow("Conjugado de partida [Nm]", motor->conj_part_Nm, true);
	vleMotor->InsertRow("Conjugado máximo [Nm]", motor->conj_max_Nm, true);
	vleMotor->InsertRow("Momento de inércia [kgm2]", motor->J_kgm2, true);
	vleMotor->InsertRow("Momento de inércia da carga [kgm2]", motor->J_carga_kgm2, true);
	vleMotor->InsertRow("Conjugado nominal da carga [Nm]", motor->conj_carga_nom_Nm, true);
	vleMotor->InsertRow("Conjugado de partida da carga [Nm]", motor->conj_carga_part_Nm, true);

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtMotor::LeDadosMotor(void) {

	// Variáveis locais
	AnsiString tipo_motor;

	// proteção: verifica se o Motor está incluído
	if (motor == NULL)
		return (true);
	try { // lê dados do Motor
		motor->Codigo = EditCodigo->Text;
		tipo_motor = cmbTipoMotor->Items->Strings[cmbTipoMotor->ItemIndex];
		motor->r1_pu = StrToDouble(vleMotor->Values["Resistência do estator (r1) [pu]"]);
		motor->x1_pu = StrToDouble(vleMotor->Values["Reatância do estator (x1) [pu]"]);
		motor->r2_pu = StrToDouble(vleMotor->Values["Resistência do rotor (r2) [pu]"]);
		motor->x2_pu = StrToDouble(vleMotor->Values["Reatância do rotor (x2) [pu]"]);
		motor->rp_pu = StrToDouble(vleMotor->Values["Resistência de perdas no ferro (rp) [pu]"]);
		motor->xm_pu = StrToDouble(vleMotor->Values["Reatância de magnetização (xm) [pu]"]);
		motor->dr2_s1_pur2 = StrToDouble(vleMotor->Values["Variação da resistência r2 na partida [pu de r2]"]);
		motor->dx2_s1_pux2 = StrToDouble(vleMotor->Values["Variação da reatância x2 na partida [pu de x2]"]);
		motor->num_polos = StrToInt(vleMotor->Values["Número de polos"]);
		motor->pmec_nom_kW = StrToDouble(vleMotor->Values["Potência mecânica nominal [kW]"]);
		motor->vnom_V = StrToDouble(vleMotor->Values["Tensão de linha nominal [V]"]);
		motor->freq_Hz = StrToDouble(vleMotor->Values["Frequência [Hz]"]);
		motor->s100_pu = StrToDouble(vleMotor->Values["Escorregamento nominal [pu]"]);
		motor->inom_A = StrToDouble(vleMotor->Values["Corrente nominal [A]"]);
		motor->ipart_A = StrToDouble(vleMotor->Values["Corrente de partida [A]"]);
		motor->fpot100_pu = StrToDouble(vleMotor->Values["Fator de potência a plena carga [pu]"]);
		motor->fpot75_pu = StrToDouble(vleMotor->Values["Fator de potência a 75% de carga [pu]"]);
		motor->fpot50_pu = StrToDouble(vleMotor->Values["Fator de potência a 50% de carga [pu]"]);
		motor->rend100_pu = StrToDouble(vleMotor->Values["Rendimento a plena carga [pu]"]);
		motor->rend75_pu = StrToDouble(vleMotor->Values["Rendimento a 75% de carga [pu]"]);
		motor->rend50_pu = StrToDouble(vleMotor->Values["Rendimento a 50% de carga [pu]"]);
		motor->conj_part_Nm = StrToDouble(vleMotor->Values["Conjugado de partida [Nm]"]);
		motor->conj_max_Nm = StrToDouble(vleMotor->Values["Conjugado máximo [Nm]"]);
		motor->J_kgm2 = StrToDouble(vleMotor->Values["Momento de inércia [kgm2]"]);
		motor->J_carga_kgm2 = StrToDouble(vleMotor->Values["Momento de inércia da carga [kgm2]"]);
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
		Erro("Dados Inválidos do Motor");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotor::OpcaoRetira(bool enabled) {
	butRetira->Visible = enabled;
}
// ---------------------------------------------------------------------------
