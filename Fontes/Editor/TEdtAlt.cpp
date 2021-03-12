// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtAlt.h"
#include "TFormEdtBar.h"
#include "TFormEdtBarMS.h"
#include "TFormEdtBateria.h"
#include "TFormEdtCap.h"
#include "TFormEdtCaps.h"
#include "TFormEdtCar.h"
#include "TFormEdtCarNL.h"
#include "TFormEdtCarIP.h"
#include "TFormEdtCarMS.h"
#include "TFormEdtChv.h"
#include "TFormEdtChvMS.h"
#include "TFormEdtEqptos.h"
#include "TFormEdtFiltro.h"
#include "TFormEdtGer.h"
#include "TFormEdtMedidor.h"
#include "TFormEdtMotor.h"
#include "TFormEdtMotorMS.h"
#include "TFormEdtMutua.h"
#include "TFormEdtReator.h"
#include "TFormEdtReguladorMT.h"
#include "TFormEdtReguladorBT.h"
#include "TFormEdtSup.h"
#include "TFormEdtSupMS.h"
#include "TFormEdtSup3.h"
#include "TFormEdtTre.h"
#include "TFormEdtTreMS.h"
#include "TFormEdtTrf3E.h"
#include "TFormEdtTrfZZ.h"
#include "TFormEdtZref.h"
#include "VTEdicao.h"
#include "Marca\TMarca.h"
#include "..\Utils\TVerificaMalhaEdicao.h"
#include "Figura\TFormEdtComentario.h"
#include "Figura\TFormEdtRetangulo.h"
#include "..\Apl\VTApl.h"
#include "..\BDihm\VTInsere.h"
#include "..\Busca\VTBusca.h"
#include "..\Curvas\VTCurva.h"
#include "..\Constante\Cursor.h"
#include "..\Distancia\VTCaminho.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEditor.h"
#include "..\Figura\VTFiguras.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTFigura.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCargaNL.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMotor.h"
#include "..\Rede\VTFiltro.h"
// #include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Rede\VTResflowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// fun��o global p/ criar objetos da classe TEdtAlt
// ---------------------------------------------------------------------------
VTEdt* NewObjEdtAlt(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtAlt(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtAlt::TEdtAlt(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	: TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
{
	// inicia objetos
	figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
	redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	// inicia estado
	estado = estSEL0;
	// cria lisBAR
	lisBAR = new TList();
	// cria objeto Caminho
	caminho = NewObjCaminho(apl);
	// mais nada a fazer: o contrutor de VTEdt faz todas inicia��es
}

// ---------------------------------------------------------------------------
__fastcall TEdtAlt::~TEdtAlt(void)
{
	// destr�i objetos
	if (caminho)
	{
		delete caminho;
		caminho = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	// mais nada a fazer: o destrutor de VTEdt faz todas finaliza��es
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::AvisoEqptoBloqueado(VTEqpto *eqpto)
{
	// vari�veis locais
	AnsiString txt;
	AnsiString tipo = eqpto->TipoAsString();
	AnsiString codigo = AnsiQuotedStr(eqpto->Codigo, '\'');

	Aviso(txt.sprintf("%s %s:\nEquipamento associado a obra posterior n�o pode ser alterado.",
		tipo.c_str(), codigo.c_str()));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::BuscaEqp(void)
{
	// vari�veis locais
	TList *lisEQP;

	// varifica se h� alguma Barra nas coordenadas
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lisEQP = busca->Eqpto(area);
	if (lisEQP->Count == 0)
	{ // n�o encontrou nenhum eqpto: muda formato do mouse
		sel.eqpto = NULL;
		graf->Cursor(TCursor(crDefault));
		return (false);
	}
	// determina primeiro Eqpto da lista
	sel.eqpto = (VTEqpto*)lisEQP->First();
	// muda formato do mouse
	switch (sel.eqpto->Tipo())
	{ // barra
	case eqptoBARRA:
		graf->Cursor(TCursor(curBarra));
		break;
		// eqbar
	case eqptoBATERIA:
		graf->Cursor(TCursor(curBateria));
		break;
	case eqptoCAPACITOR:
		graf->Cursor(TCursor(curCapacitor));
		break;
	case eqptoCARGA:
		graf->Cursor(TCursor(curCarga));
		break;
	case eqptoCARGANL:
		graf->Cursor(TCursor(curCarga));
		break;
	case eqptoFILTRO:
		graf->Cursor(TCursor(curFiltro));
		break;
	case eqptoGERADOR:
		graf->Cursor(TCursor(curGerador));
		break;
	case eqptoMEDIDOR:
		graf->Cursor(TCursor(curMedidor));
		break;
	case eqptoMOTOR:
		graf->Cursor(TCursor(curMotor));
		break;
	case eqptoREATOR:
		graf->Cursor(TCursor(curReator));
		break;
	case eqptoSUPRIMENTO:
		graf->Cursor(TCursor(curSuprimento));
		break;
	case eqptoTRAFOZZ:
		graf->Cursor(TCursor(curTrafoZZ));
		break;
	case eqptoYREF:
		graf->Cursor(TCursor(curYref));
		break;
		// ligacao
	case eqptoCHAVE:
		graf->Cursor(TCursor(curChave));
		break;
	case eqptoCAPSERIE:
		graf->Cursor(TCursor(curCapserie));
		break;
	case eqptoREGULADOR:
		graf->Cursor(TCursor(curRegulador));
		break;
	case eqptoTRAFO:
		graf->Cursor(TCursor(curTrafo));
		break;
	case eqptoTRAFO3E:
		graf->Cursor(TCursor(curTrafo3E));
		break;
	case eqptoTRECHO:
		graf->Cursor(TCursor(curTrecho));
		break;
		// m�tua
	case eqptoMUTUA:
		graf->Cursor(TCursor(curMutua));
		break;
		// rede ou redu��o
	case eqptoREDE:
		graf->Cursor(TCursor(curRede));
		break;
	case eqptoREDUCAO:
		graf->Cursor(TCursor(curRede));
		break;
	default:
		break;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::BuscaEqp(int x1, int y1, int x2, int y2)
{
	// vari�veis locais
	TList *lis;

	// reinicia lista de Eqptos selecionados
	lisEQS->Clear();
	// obt�m lista de Eqptos presentes na �rea
	area->x1 = x1;
	area->y1 = y1;
	area->x2 = x2;
	area->y2 = y2;
	lis = busca->Eqpto(area);
	// copia Eqptos p/ lisEQS
	CopiaTList(lis, lisEQS);
	return (lisEQS->Count > 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::BuscaFig(void)
{
	// vari�veis locais
	TList *lisFIG;

	// define a area como o ponto do evento para fazer a busca
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	// faz a busca de barra
	lisFIG = busca->Figura(area);
	// verifica se encontrou uma Figura
	if (lisFIG->Count == 0)
	{ // n�o encontrou nenhuma figura: muda formato do mouse
		sel.figura = NULL;
		graf->Cursor(crDefault);
		return (false);
	}
	// seleciona primeira Figura
	sel.figura = (VTFigura*)lisFIG->First();
	// muda formato do mouse
	switch (sel.figura->TipoFigura)
	{
	case figCOMENTARIO:
		graf->Cursor(TCursor(curFiltro));
		break;
	case figLINK:
		graf->Cursor(TCursor(curFiltro));
		break;
	case figRETANGULO:
		graf->Cursor(TCursor(curFiltro));
		break;
	default:
		graf->Cursor(crDefault);
		break;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::BuscaFig(int x1, int y1, int x2, int y2)
{
	// vari�veis locais
	TList *lis;

	// reinicia lista de Eqptos selecionados
	lisEQS->Clear();
	// obt�m lista de Eqptos presentes na �rea
	area->x1 = x1;
	area->y1 = y1;
	area->x2 = x2;
	area->y2 = y2;
	lis = busca->Figura(area);
	// copia Figuras p/ lisEQS
	CopiaTList(lis, lisEQS);
	return (lisEQS->Count > 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::BuscaMarca(void)
{
	// atualiza area
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	if ((marca = busca->Marca(lisMAR, area)) != NULL)
	{ // atualiza cursor
		graf->Cursor(TCursor(crHandPoint));
		return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::DefineEstadoSEL0(void)
{
	eqpto = NULL;
	figura = NULL;
	sel.eqpto = NULL;
	sel.figura = NULL;
	lisEQN->Clear();
	lisEQS->Clear();
	MarcaCancela();
	// redefine estado
	estado = estSEL0;
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaBarra(VTBarra *pbarra)
{
	// vari�veis locais
	int resp = mrCancel;
	TForm *Form;
	VTBarra *pbar_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // prote��o: verifica Eqpto bloqueado
		if (VerificaEqptoBloqueado(pbarra))
			return (mrCancel);
		// cria Barra tempor�ria p/ edi��o
		pbar_tmp = DLL_NewObjBarra(patamares->NumPatamar());
		pbar_tmp->CopiaAtributosDe(*pbarra);
		// cria form de edi��o conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtBar(NULL, apl, pbar_tmp, true);
		else
			Form = new TFormEdtBarMS(NULL, apl, pbar_tmp, true);
		// prote��o
		if (Form == NULL)
			return (resp);
		// exibe Form como janela modal
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Barra
			edita->Altera(pbarra, pbar_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Barra
			// retira a Barra e atualiza o gr�fico
			edita->Retira(pbarra);
			// destr�i Barra tempor�ria
			DLL_DeleteEqpto(pbar_tmp);
			break;
		default: // usu�rio n�o alterou dados da Barra
			// destr�i Barra tempor�ria
			DLL_DeleteEqpto(pbar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i TFormEdtBar
	if (Form)
		delete Form;

	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaBateria(VTBateria *pbat)
{
	// vari�veis locais
	int resp;
	TFormEdtBateria *FormEdtBat;
	VTBateria *pbat_tmp;

	try
	{ // cria Bateria tempor�ria p/ edi��o
		pbat_tmp = DLL_NewObjBateria(patamares->NumPatamar());
		pbat_tmp->CopiaAtributosDe(*pbat);
		// cria TFormEdtBateria
		FormEdtBat = new TFormEdtBateria(NULL, apl, pbat_tmp, false);
		switch (resp = FormEdtBat->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Bateria
			edita->Altera(pbat, pbat_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Bateria
			edita->Retira(pbat);
			// destr�i Bateria tempor�rio
			DLL_DeleteEqpto(pbat_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Bateria tempor�rio
			DLL_DeleteEqpto(pbat_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i TFormEdtBateria
	if (FormEdtBat)
		delete FormEdtBat;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCapacitor(VTCapacitor *pcap)
{
	// vari�veis locais
	int resp;
	TFormEdtCap *FormEdtCap;
	VTCapacitor *pcap_tmp;

	try
	{ // cria Capacitor tempor�rio p/ edi��o
		pcap_tmp = DLL_NewObjCapacitor(patamares->NumPatamar());
		pcap_tmp->CopiaAtributosDe(*pcap);
		// cria TFormEdtCapacitor
		FormEdtCap = new TFormEdtCap(NULL, apl, pcap_tmp);
		switch (resp = FormEdtCap->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Capacitor
			edita->Altera(pcap, pcap_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Capacitor
			edita->Retira(pcap);
			// destr�i Capacitor tempor�rio
			DLL_DeleteEqpto(pcap_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Capacitor tempor�rio
			DLL_DeleteEqpto(pcap_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtCapacitor
	if (FormEdtCap)
		delete FormEdtCap;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCarga(VTCarga *pcarga)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTCarga *pcar_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// verifica se � uma Carga equivalente
	// if (pcarga->curva->NumeroValores == nvCURVA_RRCAR)
	// {
	// Aviso("Carga equivalente: edi��o bloqueada");
	// return (mrCancel);
	// }
	// verifica se Carga � IP
	if (pcarga->IP)
		return (EditaCargaIP(pcarga));
	try
	{ // cria Carga tempor�ria p/ edi��o
		pcar_tmp = DLL_NewObjCarga(patamares->NumPatamar());
		pcar_tmp->CopiaAtributosDe(*pcarga);
		// cria form de edi��o conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtCar(NULL, apl, pcar_tmp, true);
		else
			Form = new TFormEdtCarMS(NULL, apl, pcar_tmp, true);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Carga
			edita->Altera(pcarga, pcar_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Carga
			// retira carga e atualiza o gr�fico
			edita->Retira(pcarga);
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcar_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtCarga
	if (Form)
		delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCargaNL(VTCargaNL *pcarga)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTCargaNL *pcar_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // cria Carga tempor�ria p/ edi��o
		pcar_tmp = DLL_NewObjCargaNL();
		// pcar_tmp->CopiaAtributosDe(*pcarga);
		// cria form de edi��o conforme o Editor
		Form = new TFormEdtCarNL(NULL, apl, pcarga, pcar_tmp);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Carga
			// edita->Altera(pcarga, pcar_tmp);
			edita->Retira(pcarga);
			edita->InsereEqbar(pcar_tmp, pcarga->pbarra);
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcarga);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Carga
			// retira carga e atualiza o gr�fico
			edita->Retira(pcarga);
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcar_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtCarga
	if (Form)
		delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/index.cfm#tasks/14887876
int __fastcall TEdtAlt::EditaMotor(VTMotor *pmotor)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTMotor *pmotor_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{
		// cria Carga tempor�ria p/ edi��o
		pmotor_tmp = DLL_NewObjMotor();
		pmotor_tmp->CopiaAtributosDe(*pmotor);
		// cria form de edi��o conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtMotor(NULL, apl, pmotor_tmp);
		else
			Form = new TFormEdtMotorMS(NULL, apl, pmotor_tmp); // motor simplificado
		// Form = new TFormEdtMotor(NULL, apl, pmotor_tmp);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Carga
			edita->Altera(pmotor, pmotor_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Carga
			// retira carga e atualiza o gr�fico
			edita->Retira(pmotor);
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pmotor_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pmotor_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
	// destr�i FormEdtCarga
	if (Form)
		delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCargaIP(VTCarga *pcarga)
{
	// vari�veis locais
	int resp;
	TFormEdtCarIP *FormEdtCarIP;
	VTCarga *pcar_tmp;

	try
	{ // cria Carga tempor�ria p/ edi��o
		pcar_tmp = DLL_NewObjCarga(patamares->NumPatamar());
		pcar_tmp->CopiaAtributosDe(*pcarga);
		// cria TFormEdtCarga
		FormEdtCarIP = new TFormEdtCarIP(NULL, apl, pcar_tmp);
		switch (resp = FormEdtCarIP->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Carga
			edita->Altera(pcarga, pcar_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Carga
			// retira carga e atualiza o gr�fico
			edita->Retira(pcarga);
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcar_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Carga tempor�ria
			DLL_DeleteEqpto(pcar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtCarga
	if (FormEdtCarIP)
		delete FormEdtCarIP;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCapserie(VTCapserie *capserie)
{
	// vari�veis locais
	int resp;
	TFormEdtCaps *FormEdtCaps;
	VTCapserie *caps_tmp;

	try
	{ // cria Capserie tempor�rio p/ edi��o
		caps_tmp = DLL_NewObjCapserie(patamares->NumPatamar());
		caps_tmp->CopiaAtributosDe(*capserie);
		FormEdtCaps = new TFormEdtCaps(NULL, apl, caps_tmp);
		switch (resp = FormEdtCaps->ShowModal())
		{
		case mrOk: // usu�rio alterou dados do Capserie
			edita->Altera(capserie, caps_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o do Capserie
			edita->Retira(capserie);
			// destr�i Capserie tempor�rio
			DLL_DeleteEqpto(caps_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Capserie tempor�ria
			DLL_DeleteEqpto(caps_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	delete FormEdtCaps;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaChave(VTChave *pchave)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTChave *pchv_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));
	TVerificaMalhaEdicao *utilMalha = new TVerificaMalhaEdicao(apl);
	bool malhaIntAntes, malhaIntDepois, malhaExterna;

	malhaIntAntes = malhaIntDepois = malhaExterna = false;
	try
	{ // cria Chave tempor�ria p/ edi��o
		pchv_tmp = DLL_NewObjChave(patamares->NumPatamar());
		pchv_tmp->CopiaAtributosDe(*pchave);
		if (editor->Completo)
			Form = new TFormEdtChv(NULL, apl, pchv_tmp, true);
		else
			Form = new TFormEdtChvMS(NULL, apl, pchv_tmp, true);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Chave
			// verifica se possui malha interna previa
			malhaIntAntes = utilMalha->ExisteAlertaMalha(pchave->rede);
			edita->Altera(pchave, pchv_tmp);
			// verifica se possui malha interna previa
			malhaIntDepois = utilMalha->ExisteAlertaMalha(pchave->rede);
			// verifica se possui malha externa
			malhaExterna = utilMalha->VerificaSeFechaMalhaExterna(pchave);
			// confirma acao, em caso negativo desfaz
			if ((!malhaIntAntes && malhaIntDepois) || (malhaExterna))
			{
				if (Confirma("Esta opera��o fechou malha.", "Deseja confirmar ?") != IDYES)
				{
					edita->ActionUndo->Execute();
				}
			}
			break;
		case mrNo: // usu�rio solicitou exclus�o da Chave
			// retira Chave e atualiza o gr�fico
			edita->Retira(pchave);
			// destr�i Chave tempor�ria
			DLL_DeleteEqpto(pchv_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Chave tempor�ria
			DLL_DeleteEqpto(pchv_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	if (Form)
		delete Form;

	// destro utilitario
	if (utilMalha)
	{
		delete utilMalha;
		utilMalha = NULL;
	}
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaEqptos(void)
{
	// vari�veis locais
	int resp;
	TFormEdtEqptos *FormEdtEqptos;

	try
	{ // cria FormEdtEqptos
		FormEdtEqptos = new TFormEdtEqptos(NULL, apl, lisEQS);
		switch (resp = FormEdtEqptos->ShowModal())
		{
		case mrOk: // usu�rio alterou dados dos Eqptos selecionados
			// Aviso("Falta implementar TEdita::AlteraLisEqpto()");
			// edita->AlteraLisEqpto(lisEQS);
			// reinicia lisEQS e gr�fico
			lisEQS->Clear();
			graf->Redraw();
			break;
		case mrNo: // usu�rio solicitou retirada dos Eqptos selecionados
			edita->RetiraLisEqpto(lisEQS);
			// reinicia lisEQS e gr�fico
			lisEQS->Clear();
			// graf->AreaRedeAltera();
			break;
		default: // usu�rio n�o alterou dados
			// atualiza gr�fico
			graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
			// limpa listas
			lisEQS->Clear();
			break;
		}
		// destr�i TFormEdtEqptos
		delete FormEdtEqptos;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	return (resp);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
int __fastcall TEdtAlt::EditaFigComentario(void)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTFiguraComentario *comentario = (VTFiguraComentario*)figura;

	// cria form de edi��o
	Form = new TFormEdtComentario(NULL, apl, comentario);
	// prote��o
	if (Form == NULL)
		return (mrCancel);
	// exibe Form como janela modal
	resp = Form->ShowModal();
	if (resp == mrNo)
	{ // usu�rio retirou Comentario
		figuras->RetiraComentario(comentario);
		figura = NULL;
	}
	else
	{ // define Obra, caso ainda n�o esteja definida
		if (comentario->Obra == NULL)
			comentario->Obra = edita->Obra;
	}
	// destr�i form
	delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaFigRetangulo(void)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTRetangulo *retangulo = (VTRetangulo*)figura;

	// cria form de edi��o
	Form = new TFormEdtRetangulo(NULL, apl, retangulo);
	// prote��o
	if (Form == NULL)
		return (mrCancel);
	// exibe Form como janela modal
	resp = Form->ShowModal();
	if (resp == mrNo)
	{ // usu�rio retirou Retangulo
		figuras->RetiraRetangulo(retangulo);
		figura = NULL;
	}
	else
	{ // define Obra, caso ainda n�o esteja definida
		if (retangulo->Obra == NULL)
			retangulo->Obra = edita->Obra;
	}
	// destr�i form
	delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaFiltro(VTFiltro *filtro)
{
	// vari�veis locais
	int resp;
	TFormEdtFiltro *FormEdtFiltro;
	VTFiltro *filtro_tmp;

	try
	{ // cria Filtro tempor�rio p/ edi��o
		filtro_tmp = DLL_NewObjFiltro();
		filtro_tmp->CopiaAtributosDe(*filtro);
		// cria TFormEdtFiltro
		FormEdtFiltro = new TFormEdtFiltro(NULL, apl, filtro_tmp);
		switch (resp = FormEdtFiltro->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Filtro
			edita->Altera(filtro, filtro_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Filtro
			edita->Retira(filtro);
			// destr�i Filtro tempor�rio
			DLL_DeleteEqpto(filtro_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Filtro tempor�rio
			DLL_DeleteEqpto(filtro_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtFiltro
	if (FormEdtFiltro)
		delete FormEdtFiltro;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaGerador(VTGerador *pger)
{
	// vari�veis locais
	int resp;
	TFormEdtGer *FormEdtGer;
	VTGerador *pger_tmp;

	try
	{ // cria Gerador tempor�ria p/ edi��o
		pger_tmp = DLL_NewObjGerador(patamares->NumPatamar());
		pger_tmp->CopiaAtributosDe(*pger);
		FormEdtGer = new TFormEdtGer(NULL, apl, pger_tmp);
		switch (resp = FormEdtGer->ShowModal())
		{
		case mrOk: // usu�rio alterou dados do Gerador
			edita->Altera(pger, pger_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o do Gerador
			// retira a Gerador e atualiza o gr�fico
			edita->Retira(pger);
			// destr�i Gerador tempor�ria
			DLL_DeleteEqpto(pger_tmp);
			break;
		default: // usu�rio n�o alterou dados do Gerador
			// destr�i Gerador tempor�ria
			DLL_DeleteEqpto(pger_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
	}
	delete FormEdtGer;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaMedidor(VTMedidor *pmed)
{
	// vari�veis locais
	int resp;
	TFormEdtMedidor *FormEdtMedidor;
	VTMedidor *pmed_tmp;

	try
	{ // cria Medidor tempor�ria p/ edi��o
		pmed_tmp = DLL_NewObjMedidor();
		pmed_tmp->CopiaAtributosDe(*pmed);
		FormEdtMedidor = new TFormEdtMedidor(NULL, apl, pmed_tmp);
		switch (resp = FormEdtMedidor->ShowModal())
		{
		case mrOk: // usu�rio alterou dados do Medidor
			edita->Altera(pmed, pmed_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o do Medidor
			// retira a Gerador e atualiza o gr�fico
			edita->Retira(pmed);
			// destr�i Medidor tempor�ria
			DLL_DeleteEqpto(pmed_tmp);
			break;
		default: // usu�rio n�o alterou dados do Medidor
			// destr�i Medidor tempor�ria
			DLL_DeleteEqpto(pmed_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
	}
	delete FormEdtMedidor;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaMutua(VTMutua *mutua)
{
	// vari�veis locais
	int resp;
	TFormEdtMutua *FormEdtMutua;
	VTMutua *mutua_tmp;

	try
	{ // cria Mutua tempor�rio p/ edi��o
		mutua_tmp = DLL_NewObjMutua();
		mutua_tmp->CopiaAtributosDe(*mutua);
		// cria TFormEdtMutua
		FormEdtMutua = new TFormEdtMutua(NULL, apl, mutua_tmp);
		switch (resp = FormEdtMutua->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Mutua
			edita->Altera(mutua, mutua_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Mutua
			edita->Retira(mutua);
			// destr�i Mutua tempor�rio
			DLL_DeleteEqpto(mutua_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Mutua tempor�rio
			DLL_DeleteEqpto(mutua_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtMutua
	if (FormEdtMutua)
		delete FormEdtMutua;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaReator(VTReator *reator)
{
	// vari�veis locais
	int resp;
	TFormEdtReator *FormEdtReator;
	VTReator *reator_tmp;

	try
	{ // cria Reator tempor�rio p/ edi��o
		reator_tmp = DLL_NewObjReator(patamares->NumPatamar());
		reator_tmp->CopiaAtributosDe(*reator);
		// cria TFormEdtReator
		FormEdtReator = new TFormEdtReator(NULL, apl, reator_tmp);
		switch (resp = FormEdtReator->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Reator
			edita->Altera(reator, reator_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Reator
			edita->Retira(reator);
			// destr�i Reator tempor�rio
			DLL_DeleteEqpto(reator_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Reator tempor�rio
			DLL_DeleteEqpto(reator_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtReator
	if (FormEdtReator)
		delete FormEdtReator;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaRegulador(VTRegulador *regulador)
{
	// vari�veis locais
	int resp;
	TForm *FormEdtRegulador;
	VTRegulador *regulador_tmp;

	try
	{ // cria Regulador tempor�rio p/ edi��o
		regulador_tmp = DLL_NewObjRegulador(patamares->NumPatamar());
		regulador_tmp->CopiaAtributosDe(*regulador);
		// verifica o tipo de regulador
		if (regulador_tmp->modelo == reguENERQCT)
		{ // cria Form p/ edi��o de regulador BT
			FormEdtRegulador = new TFormEdtReguladorBT(NULL, apl, regulador_tmp);
		}
		else
		{ // cria Form p/ edi��o de regulador MT
			FormEdtRegulador = new TFormEdtReguladorMT(NULL, apl, regulador_tmp);
		}
		switch (resp = FormEdtRegulador->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Regulador
			edita->Altera(regulador, regulador_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Regulador
			edita->Retira(regulador);
			// destr�i Regulador tempor�rio
			DLL_DeleteEqpto(regulador_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Regulador tempor�rio
			DLL_DeleteEqpto(regulador_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtRegulador
	if (FormEdtRegulador)
		delete FormEdtRegulador;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaSuprimento(VTSuprimento *psup)
{
	// vari�veis locais
	int resp;
	TForm *Form;
	VTSuprimento *psup_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // cria Suprimento tempor�rio p/ edi��o
		psup_tmp = DLL_NewObjSuprimento(patamares->NumPatamar());
		psup_tmp->CopiaAtributosDe(*psup);
		// verifica se Suprimento desequilibrado
		if (psup_tmp->DefinidoPorFase)
		{
			Form = new TFormEdtSup3(NULL, apl, psup_tmp, true);
			resp = Form->ShowModal();
			delete Form;
		}
		else
		{
			if (editor->Completo)
				Form = new TFormEdtSup(NULL, apl, psup_tmp, true);
			else
				Form = new TFormEdtSupMS(NULL, apl, psup_tmp, true);
			resp = Form->ShowModal();
			delete Form;
		}
		// cria TFormEdtSup
		switch (resp)
		{
		case mrOk: // usu�rio alterou dados do Suprimento
			edita->Altera(psup, psup_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o do Suprimento
			// retira o Suprimento e atualiza o gr�fico
			edita->Retira(psup);
			// destr�i Suprimento tempor�rio
			DLL_DeleteEqpto(psup_tmp);
			break;
		default: // usu�rio n�o alterou dados do Suprimento
			// destr�i Suprimento tempor�rio
			DLL_DeleteEqpto(psup_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
	}
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaTrafo(VTTrafo *ptrafo)
{
	// vari�veis locais
	bool criar_copia = true;
	int resp;
	VTEdicao *edicao = (VTEdicao*)apl->GetObject(__classid(VTEdicao));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	try
	{ // edita Trafo
		switch (resp = edicao->Executa(ptrafo, criar_copia))
		{
		case mrOk: // usu�rio alterou dados do Trafo
			if (ptrafo->Obj)
				edita->Altera(ptrafo, (VTTrafo*)ptrafo->Obj);
			break;
		case mrNo: // usu�rio solicitou exclus�o do Trafo
			edita->Retira(ptrafo);
			break;
		default: // usu�rio n�o alterou dados do Trafo: nada a fazer
			break;
		}
	}
	catch (Exception &e)
	{
	}
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaTrafo3E(VTTrafo3E *ptrafo3e)
{
	// vari�veis locais
	int resp;
	TFormEdtTrf3E *FormEdtTrf3E;
	VTTrafo3E *ptrafo3e_tmp;

	try
	{ // cria Trafo tempor�rio p/ edi��o
		ptrafo3e_tmp = DLL_NewObjTrafo3E(patamares->NumPatamar());
		ptrafo3e_tmp->CopiaAtributosDe(*ptrafo3e);
		// cria TFormEdtTrf p/ edi��o do Trafo3E
		FormEdtTrf3E = new TFormEdtTrf3E(NULL, apl, ptrafo3e_tmp);
		switch (resp = FormEdtTrf3E->ShowModal())
		{
		case mrOk: // usu�rio alterou dados do Trafo
			edita->Altera(ptrafo3e, ptrafo3e_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o do Trafo
			edita->Retira(ptrafo3e);
			// destr�i Trafo tempor�rio
			DLL_DeleteEqpto(ptrafo3e_tmp);
			break;
		default: // usu�rio n�o alterou dados do Trafo
			// destr�i Trafo tempor�rio
			DLL_DeleteEqpto(ptrafo3e_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
	}
	delete FormEdtTrf3E;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaTrafoZZ(VTTrafoZZ *trafoZZ)
{
	// vari�veis locais
	int resp;
	TFormEdtTrfZZ *FormEdtTrfZZ;
	VTTrafoZZ *trafoZZ_tmp;

	try
	{ // cria TrafoZZ tempor�rio p/ edi��o
		trafoZZ_tmp = DLL_NewObjTrafoZZ();
		trafoZZ_tmp->CopiaAtributosDe(*trafoZZ);
		// cria TFormEdtTrafoZZ
		FormEdtTrfZZ = new TFormEdtTrfZZ(NULL, apl, trafoZZ_tmp);
		switch (resp = FormEdtTrfZZ->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da TrafoZZ
			edita->Altera(trafoZZ, trafoZZ_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da TrafoZZ
			edita->Retira(trafoZZ);
			// destr�i TrafoZZ tempor�rio
			DLL_DeleteEqpto(trafoZZ_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i TrafoZZ tempor�rio
			DLL_DeleteEqpto(trafoZZ_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtTrafoZZ
	if (FormEdtTrfZZ)
		delete FormEdtTrfZZ;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaTrecho(VTTrecho *ptrecho)
{
	// vari�veis locais
	int resp;
	double dist_m;
	VTBarra *barra;
	TForm *Form;
	VTTrecho *ptrecho_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // cria Trecho tempor�rio p/ edi��o
		ptrecho_tmp = DLL_NewObjTrecho(patamares->NumPatamar());
		ptrecho_tmp->CopiaAtributosDe(*ptrecho);
		// cria form de edi��o conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtTre(NULL, apl, ptrecho_tmp, true);
		else
			Form = new TFormEdtTreMS(NULL, apl, ptrecho_tmp, true);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usu�rio alterou dados do Trecho
			if (editor->Completo)
			{ // verifica se o Trecho deve ser dividido
				if ((barra = ((TFormEdtTre*)Form)->ExisteDivisao(dist_m)) != NULL)
				{ // altera e divide Trecho
					edita->AlteraDivideTrecho(ptrecho, ptrecho_tmp, barra, dist_m);
				}
				else
				{ // apenas altera Trecho
					edita->Altera(ptrecho, ptrecho_tmp);
				}
			}
			else
			{ // apenas altera Trecho
				edita->Altera(ptrecho, ptrecho_tmp);
			}
			break;
		case mrNo: // usu�rio solicitou exclus�o do Trecho
			// retira o Trecho e atualiza o gr�fico
			edita->Retira(ptrecho);
			// destr�i Trecho tempor�rio
			DLL_DeleteEqpto(ptrecho_tmp);
			break;
		default: // usu�rio n�o alterou dados do Trecho
			// destr�i Trecho tempor�rio
			DLL_DeleteEqpto(ptrecho_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
	}
	delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaYref(VTYref *yref)
{
	// vari�veis locais
	int resp;
	TFormEdtZref *FormEdtZref;
	VTYref *yref_tmp;

	try
	{ // cria Zref tempor�ria p/ edi��o
		yref_tmp = DLL_NewObjYref();
		yref_tmp->CopiaAtributosDe(*yref);
		// cria TFormEdtZref
		FormEdtZref = new TFormEdtZref(NULL, apl, yref_tmp);
		switch (resp = FormEdtZref->ShowModal())
		{
		case mrOk: // usu�rio alterou dados da Zref
			edita->Altera(yref, yref_tmp);
			break;
		case mrNo: // usu�rio solicitou exclus�o da Zref
			// retira carga e atualiza o gr�fico
			edita->Retira(yref);
			// destr�i Zref tempor�ria
			DLL_DeleteEqpto(yref_tmp);
			break;
		default: // usu�rio n�o alterou dados
			// destr�i Zref tempor�ria
			DLL_DeleteEqpto(yref_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destr�i FormEdtZref
	if (FormEdtZref)
		delete FormEdtZref;
	return (resp);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoErro_EvtMouseDown(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoErro_EvtMouseDownMove(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoErro_EvtMouseMove(void)
{
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoErro_EvtMouseUp(void)
{
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMove_EvtMouseDown(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMove_EvtMouseDownMove(void)
{
	// vari�veis locais
	int dx, dy;

	// atualiza gr�fico: apaga Eqpto sendo movidos
	graf->Redraw(redrawEQP_NOVO, this);
	// calcula deslocamento
	dx = evt.x - evt.x_ant;
	dy = evt.y - evt.y_ant;
	// redefine coordenadas das Barras e das coordenadas dos Pontos da polyline dos Trechos
	edita->MoveBarra(lisBAR, dx, dy, true);
	// atualiza gr�fico: exibe Eqpto selecionado
	graf->Redraw(redrawEQP_NOVO, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMove_EvtMouseMove(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMove_EvtMouseUp(void)
{
	// vari�veis locais
	int dx, dy;

	// atualiza gr�fico: apaga Eqpto sendo movidos
	// graf->Redraw(redrawEQP_NOVO, this);
	// limpa listas
	lisEQS->Clear();
	lisEQN->Clear();
	// calcula deslocamento
	dx = evt.x - evt.x_ant;
	dy = evt.y - evt.y_ant;
	// redefine coordenadas das Barras
	edita->MoveBarra(lisBAR, dx, dy, false);
	// limpa lista de Barras selecionadas
	lisBAR->Clear();
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveFig_EvtMouseDownMove(void)
{
	// vari�veis locais
	int dx, dy;

	// atualiza gr�fico: apaga Figura sendo movida
	graf->Redraw(redrawFIG_NOVA, this);
	// calcula deslocamento
	dx = evt.x - evt.x_ant;
	dy = evt.y - evt.y_ant;
	// verifica se deve alterar cordenadas esq ou utm
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{ // atualiza coordenadas utm
		figura->Move_utm(dx, dy);
	}
	else
	{ // atualiza coordenadas esq
		figura->Move_esq(dx, dy);
	}
	// atualiza gr�fico: exibe Eqpto selecionado
	graf->Redraw(redrawFIG_NOVA, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveFig_EvtMouseUp(void)
{
	// limpa listas
	lisEQS->Clear();
	lisEQN->Clear();
	// atualiza gr�fico
	graf->Redraw();
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveMarca_EvtMouseDown(void)
{
	// prote��o
	if (marca == NULL)
		return;
	// atualiza Marca: in�cio de Move
	marca->StartMove();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveMarca_EvtMouseDownMove(void)
{
	// vari�veis locais
	TList *lis;
	VTBarra *pbarra = NULL;

	// varifica se h� uma Barra nas coordenadas
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lis = busca->Barra(area);
	if (lis->Count > 0)
		pbarra = (VTBarra*)lis->First();
	// atualiza gr�fico: apaga Eqptos sendo movidos
	graf->Redraw(redrawMARCA, this);
	graf->Redraw(redrawEQP_NOVO, this);
	// retira Barra alvo da Marca de lisEQN
	if (marca->barra_alvo)
		lisEQN->Remove(marca->barra_alvo);
	// redefine coord.Marca e do Eqpto associado
	marca->Move(evt.x, evt.y, evt.Shift.Contains(ssShift), pbarra, true);
	// insere Barra alvo da Marca em lisEQN
	if (marca->barra_alvo)
		lisEQN->Add(marca->barra_alvo);
	// atualiza gr�fico: exibe Eqptos sendo movidos
	graf->Redraw(redrawEQP_NOVO, this);
	graf->Redraw(redrawMARCA, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveMarca_EvtMouseUp(void)
{
	// variaveis locais
	TList *lis;
	VTBarra *pbarra = NULL;
	VTMarca *marca_nao_usada;

	// atualiza Marca: fim de Move
	marca->StopMove();
	// varifica se h� uma Barra nas coordenadas
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lis = busca->Barra(area);
	if (lis->Count > 0)
		pbarra = (VTBarra*)lis->First();
	// limpa listas
	lisBAR->Clear();
	lisEQS->Clear();
	lisEQN->Clear();
	// redefine coord.Marca e do Eqpto associado
	marca->Move(evt.x, evt.y, evt.Shift.Contains(ssShift), pbarra, false);
	// atualiza gr�fico: apaga Eqptos sendo movidos
	graf->Redraw(redrawMARCA, this);
	// destr�i Marcas n�o utilizadas
	for (int n = lisMAR->Count - 1; n >= 0; n--)
	{
		marca_nao_usada = (VTMarca*)lisMAR->Items[n];
		if (marca_nao_usada != marca)
		{ // retira Marca da lista e destr�i objeto
			lisMAR->Delete(n);
			delete marca_nao_usada;
		}
	}
	// cancela ponteiro p/ Marca
	// marca = NULL;
	// destr�i Marca utilizada que ficou em lisMAR
	// LimpaTList(lisMAR);
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
	// atualiza o gr�fico
	graf->Redraw();
	// executa tratamento externo
	if (pbarra != NULL)
		Done();
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TEdtAlt::EstadoSel0_EvtMouseDown(void)
 {
 // verifica se h� algum Eqpto na �rea
 BuscaEqpto();
 if (eqpto == NULL)
 { // atualiza estado
 estado = estERRO;
 graf->DefMouseOp(mouseAREA);
 return;
 }
 // h� eqpto na �rea: insere eqpto na lista de lisEQS
 lisEQS->Add(eqpto);
 // atualiza gr�fico
 graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TEdtAlt::EstadoSel0_EvtMouseDown(void)
{
	// vari�veis locais
	VTEqpto *eqp_sel;
	VTFigura *fig_sel;
	VTFiguraComentario *comentario;

	// verifica se h� algum Eqpto na �rea
	if (BuscaFig())
	{ // verifica se Figura � um Comentario e a tecla Alt est� apertada
		if ((sel.figura->TipoFigura == figCOMENTARIO) && (evt.Shift.Contains(ssAlt)))
		{ // altera formato do Comentario
			comentario = (VTFiguraComentario*)sel.figura;
			if (comentario->Formato == fmtFLAG)
				comentario->Formato = fmtRETANGULO;
			else
				comentario->Formato = fmtFLAG;
			// atualiza gr�fico
			graf->Redraw();
		}
		else
		{ // salva Figura e insere na lista de lisEQS
			figura = sel.figura;
			lisEQS->Clear();
			lisEQS->Add(figura);
			// atualiza gr�fico
			graf->Redraw(redrawFIG_SEL_COR_UNICA, this);
		}
	}
    else if (BuscaEqp())
	{ // h� eqpto na �rea: salva Eqpto e insere na lista de lisEQS
		eqpto = sel.eqpto;
		lisEQS->Clear();
		lisEQS->Add(eqpto);
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
	else
	{ // redefine estado estSEL0
		// estado = estERRO;
		DefineEstadoSEL0();
		// pode estar sendo iniciada uma sele��o de �rea
		graf->DefMouseOp(mouseAREA);
		return;
	}
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TEdtAlt::EstadoSel0_EvtMouseDownMove(void)
 {
 // prote��o
 if (eqpto == NULL)
 return;
 // verifica se o Eqpto selecionado � um Eqbar
 if (eqpto->TipoEqbar())
 {
 PreparaSpin();
 EstadoSpin_EvtMouseDownMove();
 return;
 }
 // inicia dados p/ 'move' do eqpto selecionado
 PreparaMove();
 EstadoMove_EvtMouseDownMove();
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel0_EvtMouseDownMove(void)
{
	// prote��o
	if (eqpto != NULL)
	{ // verifica se o Eqpto selecionado � um Eqbar
		if (eqpto->TipoEqbar())
		{
			PreparaSpin();
			EstadoSpin_EvtMouseDownMove();
		}
		else
		{ // inicia dados p/ 'move' do eqpto selecionado
			if(PreparaMoveEqpto())
				{EstadoMove_EvtMouseDownMove();}
		}
	}
	else if (figura != NULL)
	{ // inicia dados para 'move' da Figura
		PreparaMoveFigura();
		EstadoMoveFig_EvtMouseDownMove();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel0_EvtMouseMove(void)
{
//	// verifica se h� algum Eqpto na �rea
//	if (BuscaEqp() == NULL)
//	{ // verifica se h� alguma Figura
//		BuscaFig();
//	}
	// verifica se h� algum Figura na �rea
	if (BuscaFig() == false)
	{ // verifica se h� alguma Eqpto
		BuscaEqp();
	}
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TEdtAlt::EstadoSel0_EvtMouseUp(void)
 {
 if (lisEQS->Count == 1)
 { // cria marcas p/ eqpto selecionado e atualiza o gr�fico
 MarcaInicia();
 // atualiza estado
 estado = estSEL1;
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel0_EvtMouseUp(void)
{
	if (lisEQS->Count == 1)
	{
		// atualiza estado
		estado = estSEL1;
		if (eqpto != NULL)
		{ // cria marcas p/ eqpto selecionado e atualiza o gr�fico
			MarcaInicia();
		}
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16270997
void __fastcall TEdtAlt::EstadoSel1_EvtMouseDown(void)
{
	// verifica se est� sendo selecionada uma Marca
	if (marca)
	{ // trata evento para Marca
		EstadoMoveMarca_EvtMouseDown();
		return;
	}
	// destr�i Marcas e atualiza gr�fico
	MarcaCancela();
	// verifica se h� uma Figura selecionada
	if (figura != NULL)
		return;
	// verifica se h� um Eqpto selecionado
	if ((eqpto != NULL) && (sel.eqpto == NULL))
	{ // verifica se a tecla Control ou Shift est� apertada (o que indica sele��o de Eqpto)
		if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
			return;
		// destr�i Marcas e atualiza gr�fico
		// MarcaCancela();
		// atualiza gr�fico
		if (lisEQS->Count > 0)
			graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		// lisEQS->Clear();
		// eqpto  = NULL;
		// figura = NULL;
		// estado = estSEL0;
		DefineEstadoSEL0();
		// pode estar sendo iniciada uma sele��o de �rea
		graf->DefMouseOp(mouseAREA);
		return;
	}
	// verifica se o Eqpto selecionado � igual ao Eqpto
	if ((eqpto != NULL) && (sel.eqpto == eqpto))
	{ // nada a fazer: o evento MouseUp abrir� janela de edi��o
		return;
	}
	// Eqpto selecionado n�o est� em lisEQS: verifica se a tecla Control est� apertada
	if (evt.Shift.Contains(ssCtrl))
	{ // insere eqpto
		if (lisEQS->IndexOf(sel.eqpto) < 0)
			lisEQS->Add(sel.eqpto);
		// cancela Marcas
		// MarcaCancela();
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		// atualiza estado
		estado = estSELN;
	}
	// verifica se a tecla Shift est� apertada
	else if (evt.Shift.Contains(ssShift))
	{ // atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// seleciona todos Ligacoes interligados pelos 2 Eqptos selecionados
		SelecionaCaminhoEletrico(eqpto, sel.eqpto, NULL);
		// cancela Marcas
		// MarcaCancela();
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		// atualiza estado
		estado = estSELN;
	}
	else
	{ // cancela Marcas do Eqpto selecionado anteriormente
		// MarcaCancela();
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// salva o Eqpto selecionado
		eqpto = sel.eqpto;
		// atualiza estado
		estado = estSEL0;
		// reinicia lisEQS c/ Eqpto selecionado
		lisEQS->Clear();
		if(eqpto != NULL)
			lisEQS->Add(eqpto);
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel1_EvtMouseDownMove(void)
{
	// verifica se est� sendo movida uma Marca
	if (marca)
	{ // verifica se � marca de uma Barra
		PreparaMoveMarca();
		EstadoMoveMarca_EvtMouseDownMove();
		return;
	}
	// verifica se h� uma Figura selecionada
	if (figura != NULL)
		return;
	// verifica se o Eqpto selecionado � um Eqbar
	if (eqpto->TipoEqbar())
	{
		PreparaSpin();
		EstadoSpin_EvtMouseDownMove();
		return;
	}
	// inicia dados p/ 'move' do eqpto selecionado
	if(PreparaMoveEqpto())
	{
		estado = estMOVE;
		EstadoMove_EvtMouseDownMove();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel1_EvtMouseMove(void)
{
//	// verifica se h� alguma Marca na �rea
//	if (!BuscaMarca())
//	{ // n�o h� Marca: procura Eqpto
//		if (!BuscaEqp())
//		{ // n�o h� Eqpto: procura figura
//			BuscaFig();
//		}
//	}
	// verifica se h� alguma Marca na �rea
	if (!BuscaMarca())
	{ // n�o h� Marca: procura figura
		if (!BuscaFig())
		{ // n�o h� figura: procura Eqpto
			BuscaEqp();
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel1_EvtMouseUp(void)
{
	// vari�veis locais
	int resp = mrOk;

	// verifica se a tecla Control ou Shift est� apertada (o que indica sele��o de Eqpto)
	if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
		return;
	// cancela Marcas
	MarcaCancela();
	// verifica se n�o h� Eqpto ou Figura selecionado
	if ((eqpto == NULL) && (figura == NULL))
	{ // limpa listas
		// lisEQN->Clear();
		// lisEQS->Clear();
		// estado = estSEL0;
		DefineEstadoSEL0();
		return;
	}
	// verifica se h� uma Figura selecionada
	if (figura != NULL)
	{ // verifica se � mesma Figura atualmente selecionda
		if (figura == sel.figura)
		{ // edita a Figura
			switch (figura->TipoFigura)
			{
			case figCOMENTARIO:
				resp = EditaFigComentario();
				break;
			case figRETANGULO:
				resp = EditaFigRetangulo();
				break;
			default:
				resp = ID_CANCEL;
			}
			switch (resp)
			{
			case mrNo: // Figura foi retirada
			case mrOk: // Figura foi alterada
				// limpa lisEQS antes de atualizar o Grafico
				lisEQS->Clear();
				graf->Redraw();
				// sinaliza t�rmino de opera��o
				Done();
				break;
			case mrCancel: // Figura n�o foi alterada
			default:
				// atualiza gr�fico: reapresenta Figura selecionada c/ sua cor original
				graf->Redraw(redrawFIG_SEL_COR_ORIG, this);
				break;
			}
		}
		else
		{ // atualiza gr�fico: reapresenta Figura selecionada c/ sua cor original
			graf->Redraw(redrawFIG_SEL_COR_ORIG, this);
		}
		// retorna ao estado estSEL0
		// lisEQS->Clear();
		// figura = NULL;
		// eqpto  = NULL;
		// estado = estSEL0;
		DefineEstadoSEL0();
		return;
	}
	// verifica se h� um Eqpto Selecionado
	if (eqpto != NULL)
	{ // verifica se � o mesmo Eqptop selecionado atualmente
		if (eqpto == sel.eqpto)
		{ // edita eqpto selecionado
			switch (eqpto->Tipo())
			{ // barra
			case eqptoBARRA:
				resp = EditaBarra((VTBarra*)eqpto);
				break;
				// eqbar
			case eqptoBATERIA:
				resp = EditaBateria((VTBateria*)eqpto);
				break;
			case eqptoCAPACITOR:
				resp = EditaCapacitor((VTCapacitor*)eqpto);
				break;
			case eqptoCARGA:
				resp = EditaCarga((VTCarga*)eqpto);
				break;
			case eqptoCARGANL:
				resp = EditaCargaNL((VTCargaNL*)eqpto);
				break;
			case eqptoFILTRO:
				resp = EditaFiltro((VTFiltro*)eqpto);
				break;
			case eqptoGERADOR:
				resp = EditaGerador((VTGerador*)eqpto);
				break;
			case eqptoMEDIDOR:
				resp = EditaMedidor((VTMedidor*)eqpto);
				break;
			case eqptoREATOR:
				resp = EditaReator((VTReator*)eqpto);
				break;
			case eqptoSUPRIMENTO:
				resp = EditaSuprimento((VTSuprimento*)eqpto);
				break;
			case eqptoTRAFOZZ:
				resp = EditaTrafoZZ((VTTrafoZZ*)eqpto);
				break;
			case eqptoYREF:
				resp = EditaYref((VTYref*)eqpto);
				break;
			case eqptoMOTOR:
				resp = EditaMotor((VTMotor*)eqpto);
				break;
				// ligacao
			case eqptoCAPSERIE:
				resp = EditaCapserie((VTCapserie*)eqpto);
				break;
			case eqptoCHAVE:
				resp = EditaChave((VTChave*)eqpto);
				break;
			case eqptoREGULADOR:
				resp = EditaRegulador((VTRegulador*)eqpto);
				break;
			case eqptoTRAFO:
				resp = EditaTrafo((VTTrafo*)eqpto);
				break;
			case eqptoTRAFO3E:
				resp = EditaTrafo3E((VTTrafo3E*)eqpto);
				break;
			case eqptoTRECHO:
				resp = EditaTrecho((VTTrecho*)eqpto);
				break;
				// mutua
			case eqptoMUTUA:
				resp = EditaMutua((VTMutua*)eqpto);
				break;
				// Reducao
			case eqptoREDUCAO:
				lisEQS->Clear();
				resp = InsereRede((VTReducao*)eqpto);
				break;
			default:
				break;
			}
			switch (resp)
			{
			case mrNo: // Eqpto foi retirado da Rede
				lisEQS->Clear();
				// sinaliza t�rmino de opera��o
				Done();
				break;
			case mrOk: // Eqpto foi alterado
				// sinaliza t�rmino de opera��o
				Done();
			case mrCancel: // Eqpto n�o foi alterado
			default:
				// atualiza gr�fico: reapresenta eqpto selecionado c/ sua cor original
				graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
				break;
			}
		}
		// atualiza estado
		DefineEstadoSEL0();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSelN_EvtMouseDown(void)
{
	// vari�veis locais
	int index;

	// verifica se h� algum Eqpto na �rea
	BuscaEqp();
	if (sel.eqpto == NULL)
	{ // verifica se a tecla Control ou Shift est� apertada (o que indica sele��o de Eqpto)
		if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
			return;
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		// lisEQS->Clear();
		// pode estar sendo iniciada uma sele��o de �rea
		// estado = estERRO;
		// graf->DefMouseOp(mouseAREA);
		DefineEstadoSEL0();
		// pode estar sendo iniciada uma sele��o de �rea
		graf->DefMouseOp(mouseAREA);
		return;
	}
	// verifica se o Eqpto selecionado j� est� em lisEQS
	if ((index = lisEQS->IndexOf(sel.eqpto)) >= 0)
	{ // Eqpto j� est� em lisEQS: verifica se a tecla Control est� apertada
		if (evt.Shift.Contains(ssCtrl))
		{ // tecla Control est� apertada: retira Eqpto da sele��o
			graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
			// retira Eqpto de lisEQS
			lisEQS->Delete(index);
			// atualiza gr�fico
			graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
			// verifica se deve atualizar estado p/ estSEL0 (pois o evento MouseUp mudar� o estado p/ estSEL1)
			if (lisEQS->Count == 1)
				estado = estSEL0;
		}
		else
		{ // tecla Control n�o est� apertada: edita Eqptos
			// EditaEqptos();
			// atualiza estado
			// estado = estSEL0;
		}
		return;
	}
	// Eqpto selecionado n�o est� em lisEQS: verifica se a tecla Control est� apertada
	if (evt.Shift.Contains(ssCtrl))
	{ // insere Eqpto na sele��o
		lisEQS->Add(sel.eqpto);
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
	else
	{ // reinicia sele��o: atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		lisEQS->Clear();
		// atualiza estado
		estado = estSEL0;
		// insere eqpto na lista de lisEQS
		lisEQS->Add(sel.eqpto);
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSelN_EvtMouseDownMove(void)
{
	// inicia dados p/ 'move' do eqpto selecionado
	if (PreparaMoveEqpto())
	{
		EstadoMove_EvtMouseDownMove();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSelN_EvtMouseMove(void)
{
	// verifica se h� algum Eqpto na �rea
	BuscaEqp();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSelN_EvtMouseUp(void)
{
	// verifica se a tecla Control ou Shift est� apertada (o que indica sele��o de Eqpto)
	if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
		return;
	// tecla Control n�o est� apertada: edita Eqptos
	EditaEqptos();
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSpin_EvtMouseDown(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSpin_EvtMouseDownMove(void)
{
	// atualiza gr�fico: apaga Eqpto sendo reposicionado
	graf->Redraw(redrawEQP_NOVO, this);
	// reposiciona Eqbar
	// edita->MoveEqbar(eqpto, evt.x, evt.y);
	edita->MoveEqbar((VTEqbar*)eqpto, evt.x, evt.y, true);
	// atualiza gr�fico: exibe Eqpto selecionado
	graf->Redraw(redrawEQP_NOVO, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSpin_EvtMouseMove(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSpin_EvtMouseUp(void)
{
	// limpa listas
	lisBAR->Clear();
	lisEQS->Clear();
	lisEQN->Clear();
	// reposiciona Eqbar
	// edita->MoveEqbar(eqpto, evt.x, evt.y);
	edita->MoveEqbar((VTEqbar*)eqpto, evt.x, evt.y, false);
	// atualiza gr�fico
	// graf->Redraw();
	// indica altera��o da rede p/ graf (que atualiza o gr�fico)
	// graf->AreaRedeAltera();
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TEdtAlt::EvtDelete(void)
{
	// vari�veis locais
	VTEqpto *eqpto;
	VTBarra *pbar;

	// prote��o
	if (lisEQS->Count == 0)
	{
		Aviso("Selecione os equipamentos para exclus�o");
		return;
	}
	// verifica se h� um Figura selecionada
	if (figura != NULL)
	{ // retira o Figura
		figuras->RetiraFigura(figura);
		lisEQS->Clear();
		figura = NULL;
		// atualiza o Grafico
		graf->Redraw();
		// redefine estado
		// estado = estSEL0;
		DefineEstadoSEL0();
		return;
	}
	// bloqueia a exclus�o de Barra inicial de uma Rede
	if (ExisteBarraInicialDeRede(lisEQS))
	{
		Aviso("N�o � poss�vel eliminar barras iniciais de redes.");
		return;
	}
	// bloqueia a exclus�o de Barra de Corte com Reducao
	if (ExisteBarraDeCorteComReducao(lisEQS))
	{
		Aviso("N�o � poss�vel eliminar barras que conectam redes equivalentes.");
		return;
	}
	// bloqueia exclus�o de Eqpto envolvidos em Obras posteriores
	if (VerificaEqptoBloqueado(lisEQS))
		return;
	// pede confirma��o do usu�rio
	if (Confirma("Os equipamentos selecionados ser�o exclu�dos.", "Confirma exclus�o ?") != IDYES)
		return;
	/*
	 //OBS: na retirada da Barra s�o retiradas as Liga��es conectadas a ela
	 for (int n = 0; n < lisEQS->Count; n++)
	 {
	 eqpto = (VTEqpto*)lisEQS->Items[n];
	 if (eqpto->Tipo() == eqptoBARRA)
	 {
	 pbar = (VTBarra*)eqpto;
	 edita->Retira(pbar);
	 }
	 }
	 */
	// transfere lisEQS para lisBAR para reiniciar lisEQS antes de chamar edita->RetiraLisEqpto(()
	lisBAR->Clear();
	CopiaTList(lisEQS, lisBAR);
	lisEQS->Clear();
	// IMPORTANTE: destr�i Marcas pois uma Marca associa Eqpbar e Ligacao c/ barras tempor�rias
	MarcaCancela();
	// retira Eqptos
	edita->RetiraLisEqpto(lisBAR);
	lisBAR->Clear();
	// atualiza o gr�fico  (a linha estava comentada por causa da tarefa 14422280, mas trouxe efeito colateral)
	// graf->Redraw();
	// redefine estado em fun��o do n�mero de Eqptos selecionados
	// estado = estSEL0;
	DefineEstadoSEL0();
	// DVK 2016.05.18
	Done();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtEscape(void)
{
	// verifica se h� uma Marca selecionada
	if ((marca) || (eqpto == NULL))
	{ // cancela Marcas
		MarcaCancela();
		// atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		// lisEQS->Clear();
		// estado = estSEL0;
		DefineEstadoSEL0();
		return;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtMouseDown(void)
{
	// trata estado
	switch (estado)
	{
	case estERRO:
		EstadoErro_EvtMouseDown();
		break;
	case estSEL0:
		EstadoSel0_EvtMouseDown();
		break;
	case estSEL1:
		EstadoSel1_EvtMouseDown();
		break;
	case estSELN:
		EstadoSelN_EvtMouseDown();
		break;
	case estMOVE:
		EstadoMove_EvtMouseDown();
		break;
	case estSPIN:
		EstadoSpin_EvtMouseDown();
		break;
		// case estMOVE_MARCA:
		// EstadoMoveMarca_EvtMouseDown();
		// break;
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtMouseDownMove(void)
{
	// trata estado
	switch (estado)
	{
	case estERRO:
		EstadoErro_EvtMouseDownMove();
		break;
	case estSEL0:
		EstadoSel0_EvtMouseDownMove();
		break;
	case estSEL1:
		EstadoSel1_EvtMouseDownMove();
		break;
	case estSELN:
		EstadoSelN_EvtMouseDownMove();
		break;
	case estMOVE:
		EstadoMove_EvtMouseDownMove();
		break;
	case estSPIN:
		EstadoSpin_EvtMouseDownMove();
		break;
	case estMOVE_MARCA:
		EstadoMoveMarca_EvtMouseDownMove();
		break;
	case estMOVE_FIG:
		EstadoMoveFig_EvtMouseDownMove();
		break;
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtMouseMove(void)
{
	// trata estado
	switch (estado)
	{
	case estERRO:
		EstadoErro_EvtMouseMove();
		break;
	case estSEL0:
		EstadoSel0_EvtMouseMove();
		break;
	case estSEL1:
		EstadoSel1_EvtMouseMove();
		break;
	case estSELN:
		EstadoSelN_EvtMouseMove();
		break;
	case estMOVE:
		EstadoMove_EvtMouseMove();
		break;
	case estSPIN:
		EstadoSpin_EvtMouseMove();
		break;
		// case estMOVE_MARCA:
		// EstadoMoveMarca_EvtMouseMove();
		// break;
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtMouseSelAll(void)
{ // DVK 2014.08.01
	// variaveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia lisEQS
	lisEQS->Clear();
	redes->LisEqpto(lisEQS);
	// atualiza gr�fico
	graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	// redefine estado em fun��o do n�mero de Eqptos selecionados
	switch (lisEQS->Count)
	{
	case 0:
		// estado = estSEL0;
		DefineEstadoSEL0();
		break;
	case 1:
		estado = estSEL1;
		break;
	default:
		estado = estSELN;
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtMouseUp(void)
{
	// trata estado
	switch (estado)
	{
	case estERRO:
		EstadoErro_EvtMouseUp();
		break;
	case estSEL0:
		EstadoSel0_EvtMouseUp();
		break;
	case estSEL1:
		EstadoSel1_EvtMouseUp();
		break;
	case estSELN:
		EstadoSelN_EvtMouseUp();
		break;
	case estMOVE:
		EstadoMove_EvtMouseUp();
		break;
	case estMOVE_FIG:
		EstadoMoveFig_EvtMouseUp();
		break;
	case estSPIN:
		EstadoSpin_EvtMouseUp();
		break;
	case estMOVE_MARCA:
		EstadoMoveMarca_EvtMouseUp();
		break;
    default: break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtMove(bool down, bool left, bool right, bool up)
{
	// vari�veis locais
	int delta = 1;
	int dx = 0, dy = 0;
	double escala;
	// VTBarra  *pbar;
	VTEqpto *eqpto;
	// VTMarca *pm;

	// prote��o verifica se h� Eqptos selecionados
	if (lisEQS->Count == 0)
		return;
	// cancela Marcas
	MarcaCancela();
	// define deslocamento
	// if ((escala = graf->Escala()) > 0.) delta = int((1./escala)+0.5) + 2;
	if ((escala = graf->Escala()) > 0.)
		delta = int((1. / escala) + escala);
	if (left)
		dx = -delta;
	else if (right)
		dx = delta;
	if (up)
		dy = delta;
	else if (down)
		dy = -delta;
	// verifica se existe apenas um Eqbar selecionado
	if (lisEQS->Count == 1)
	{
		eqpto = (VTEqpto*)lisEQS->First();
		// prote��o: verifica se h� Eqptos bloqueados
		if (VerificaEqptoBloqueado(eqpto))
			return;
		if (eqpto->TipoEqbar())
		{ // reposiciona Eqbar
			int x1, y1, x2, y2;
			((VTEqbar*)eqpto)->CoordenadasEsquematico(x1, y1, x2, y2);
			edita->MoveEqbar((VTEqbar*)eqpto, x2 + dx, y2 + dy, false);
			// atualiza gr�fico
			// graf->Redraw();
			// graf->AreaRedeAltera(); //indica altera��o da rede p/ graf (que atualiza o gr�fico)
			return;
		}
	}
	// monta lista c/ as Barras existentes em lisEQS
	MontaLisBarraSelecionada();
	// prote��o: verifica se h� Eqptos bloqueados
	if (VerificaEqptoBloqueado(lisBAR))
		return;
	// desloca todas as Barras: redefine coordenadas das Barras
	edita->MoveBarra(lisBAR, dx, dy, false);
	// atualiza gr�fico
	// graf->AreaRedeAltera(); //indica altera��o da rede p/ graf (que atualiza o gr�fico)
	if (lisEQS->Count > 0)
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	if (lisMAR->Count > 0)
		graf->Redraw(redrawMARCA, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtSelArea(int x1, int y1, int x2, int y2)
{
	// redefine tratamento de mouse
	graf->DefMouseOp(mouseEXTERNO);
	// busca Eqptos na �rea
	if (BuscaEqp(x1, y1, x2, y2))
	{ // atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		// redefine estado em fun��o do n�mero de Eqptos selecionados
		switch (lisEQS->Count)
		{
		case 0:
			// estado = estSEL0;
			DefineEstadoSEL0();
			break;
		case 1:
			estado = estSEL1;
			break;
		default:
			estado = estSELN;
			break;
		}
	}
	else if (BuscaFig(x1, y1, x2, y2))
	{
		estado = estFIG_1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtSelEqpto(TList *lisEXT)
{
	// reinicia lisEQS com lisEXT
	lisEQS->Clear();
	CopiaTList(lisEXT, lisEQS);
	// atualiza gr�fico
	graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	// redefine estado em fun��o do n�mero de Eqptos selecionados
	switch (lisEQS->Count)
	{
	case 0:
		// estado = estSEL0;
		DefineEstadoSEL0();
		break;
	case 1:
		estado = estSEL1;
		break;
	default:
		estado = estSELN;
		break;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::ExisteBarraDeCorteComReducao(TList *lisEQP)
{
	// vari�veis locais
	VTBarra *barra;
	VTCorte *corte;
	VTEqpto *eqpto;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCORTE = redes->LisCorte();

	// verifica se existe alguma Barra de Corte na lista lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (!eqpto->TipoBarra())
			continue;
		barra = (VTBarra*)eqpto;
		// verifica se a Barra est� presente em algum Corte
		for (int nc = 0; nc < lisCORTE->Count; nc++)
		{
			corte = (VTCorte*)lisCORTE->Items[nc];
			if (corte->ExisteBarra(barra))
			{ // verifica se o Corte tem alguma Redu��o
				if (corte->ExisteReducao())
					return (true);
			}
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::ExisteBarraInicialDeRede(TList *lisEQP)
{
	// vari�veis locais
	VTBarra *barra;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// verifica se existe alguma Barra de Corte na lista lisEQP
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if ((barra = rede->BarraInicial()) == NULL)
			continue;
		if (lisEQP->IndexOf(barra) >= 0)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::InsereRede(VTReducao *reducao)
{
	// vari�veis locais
	VTInsere *insere = (VTInsere*)apl->GetObject(__classid(VTInsere));

	// prote��o
	if (insere)
	{ // insere Redes da Reducao e faz a atualiza��o do gr�fico)
		insere->Executa(reducao);
	}
	else
	{ // atualiza o gr�fico
		graf->Redraw();
	}
	return (mrCancel);
}

// ---------------------------------------------------------------------------
TList* __fastcall TEdtAlt::LisEqptoSel(void)
{
	// retorna lista de Eqptos selecionados
	return (lisEQS);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::MarcaCancela(void)
{
	// atualiza gr�fico
	graf->Redraw(redrawMARCA, this);
	// destr�i Marcas
	LimpaTList(lisMAR);
	// reinicia ponteiro
	marca = NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::MarcaInicia(void)
{
	// vari�veis locais
	VTBarra *pbar;
	VTEqbar *eqbar;
	VTEqpto *eqpto;
	VTLigacao *pliga;
	VTMarca *marca;

	// reinicia lisMAR
	lisMAR->Clear();
	// prote��o: verifica exist�ncia de Eqpto bloueqado
	if (VerificaEqptoBloqueado(lisEQS))
		return;
	// cria Marcas p/ Eqptos selecionados  em lisEQS(OBS: deve existir apenas um)
	for (int n = 0; n < lisEQS->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQS->Items[n];
		if (eqpto->TipoBarra())
		{
			pbar = (VTBarra*)eqpto;
			if ((marca = NewObjMarcaBar(apl, pbar, true)) != NULL)
				lisMAR->Add(marca);
			if ((marca = NewObjMarcaBar(apl, pbar, false)) != NULL)
				lisMAR->Add(marca);
		}
		if (eqpto->TipoEqbar())
		{
			eqbar = (VTEqbar*)eqpto;
			// cria Marca p/ Barra do Eqbar
			pbar = (VTBarra*)eqpto;
			if ((marca = NewObjMarcaEqbar(apl, eqbar, pbar)) != NULL)
				lisMAR->Add(marca);
		}
		else if (eqpto->TipoLigacao())
		{
			pliga = (VTLigacao*)eqpto;
			// cria Marca p/ Barras da Ligacao
			if ((marca = NewObjMarcaLiga(apl, pliga, pliga->pbarra1)) != NULL)
				lisMAR->Add(marca);
			if ((marca = NewObjMarcaLiga(apl, pliga, pliga->pbarra2)) != NULL)
				lisMAR->Add(marca);
			if (pliga->pbarra3)
			{
				if ((marca = NewObjMarcaLiga(apl, pliga, pliga->pbarra3)) != NULL)
					lisMAR->Add(marca);
			}
		}
	}
	// atualiza gr�fico
	graf->Redraw(redrawMARCA, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::MontaLisBarraSelecionada(void)
{
	// vari�veis locais
	VTEqbar *eqpbar;
	VTEqpto *eqpto;
	VTLigacao *pliga;
	VTRede *rede;
	VTReducao *reducao;

	// monta lista c/ Barras que devem ser movidas
	lisBAR->Clear();
	for (int n = 0; n < lisEQS->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQS->Items[n];
		if (eqpto->TipoBarra())
		{
			if (lisBAR->IndexOf(eqpto) < 0)
				lisBAR->Add(eqpto);
		}
		else if (eqpto->TipoEqbar())
		{
			eqpbar = (VTEqbar*)eqpto;
			if (lisBAR->IndexOf(eqpbar->pbarra) < 0)
				lisBAR->Add(eqpbar->pbarra);
		}
		else if (eqpto->TipoLigacao())
		{
			pliga = (VTLigacao*)eqpto;
			pliga->LisBarra(lisBAR);
		}
		else if (eqpto->Tipo() == eqptoREDE)
		{ // insere Barra fict�cia do esquem�tico da Rede
			rede = (VTRede*)eqpto;
			eqpto = rede->BarraFicticia();
			if ((eqpto != NULL) && (lisBAR->IndexOf(eqpto) < 0))
				lisBAR->Add(eqpto);
		}
		else if (eqpto->Tipo() == eqptoREDUCAO)
		{ // insere Barras fict�cias da Reducao
			reducao = (VTReducao*)eqpto;
			eqpto = reducao->BarraFicticia();
			if (lisBAR->IndexOf(eqpto) < 0)
			{
				lisBAR->Add(eqpto);
				CopiaTList(reducao->LisBarraEsquematico(), lisBAR);
			}
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::PreparaMoveEqpto(void)
{
	// vari�veis locais
	VTBarra *pbar;
	TList *lisLIG;

	// monta lista c/ Barras que devem ser movidas
	MontaLisBarraSelecionada();
	// prote��o
	if (VerificaEqptoBloqueado(lisBAR))
		return false;
	// determina Ligacoes existentes na �rea
	lisLIG = busca->Ligacao(lisBAR);
	// monta lisEQN c/ todos os Eqptos: Ligacoes, Barras e respectivos Eqbar
	lisEQN->Clear();
	if (lisLIG->Count != 0)
	{ // copia Ligacoes
		CopiaTList(lisLIG, lisEQN);
		// verifica se est� modo esquem�tico
		if (redegraf->ModoGrafico == modoESQUEMATICO)
		{ // inclui Barra representadas por segmentos
			for (int nb = 0; nb < lisBAR->Count; nb++)
			{
				pbar = (VTBarra*)lisBAR->Items[nb];
				if ((pbar->esq.x1 != pbar->esq.x2) || (pbar->esq.y1 != pbar->esq.y2))
				{
					lisEQN->Add(pbar);
				}
			}
		}
	}
	else
	{ // n�o h� Ligacoes: insere as Barras
		CopiaTList(lisBAR, lisEQN);
	}
	// inclui Eqbar em lisEQN somente se houver poucos Eqptos
	if (lisEQN->Count < 100)
	{
		for (int n = 0; n < lisBAR->Count; n++)
		{
			pbar = (VTBarra*)lisBAR->Items[n];
			// inclui Eqbar da Barra em lisEQN
			pbar->LisEqbar(lisEQN);
		}
	}
	// prote��o
	if (VerificaEqptoBloqueado(lisEQN))
	{ // atualiza estado
		// estado = estSEL0;
		DefineEstadoSEL0();
		return false;
	}
	// atualiza gr�fico: exibe Eqptos a serem movidos
	graf->Redraw(redrawEQP_NOVO, this);
	// atualiza cursor
	graf->Cursor(TCursor(crHandPoint));
	// atualiza estado
	estado = estMOVE;

    return true;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::PreparaMoveFigura(void)
{
	lisEQN->Clear();
	lisEQN->Add(figura);
	// atualiza estado
	estado = estMOVE_FIG;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::PreparaMoveMarca(void)
{
	// vari�veis locais
	VTBarra *pbar;
	VTEqpto *eqpto;
	VTLigacao *pliga;
	VTEqbar *eqpbar;
	TList *lisTMP;

	if (marca->DeBarra())
	{ // monta listas lisEQN c/ Eqptos a serem movidos c/ a marca
		PreparaMoveEqpto();
	}
	else
	{ // monta lisEQN c/ a Ligacao a ser movida c/ a marca
		lisEQN->Clear();
		CopiaTList(lisEQS, lisEQN);
		// atualiza gr�fico: exibe Eqptos a serem movidos
		graf->Redraw(redrawEQP_NOVO, this);
		// graf->Redraw(redrawMARCA, this);
		// atualiza cursor
		graf->Cursor(TCursor(crHandPoint));
	}
	// atualiza estado
	estado = estMOVE_MARCA;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::PreparaSpin(void)
{
	// prote��o
	if (VerificaEqptoBloqueado(lisEQS))
		return;
	// inicia lisEQN c/ lisEQS (que s� tem o Eqpbar a ser reposicionado)
	lisEQN->Clear();
	CopiaTList(lisEQS, lisEQN);
	// atualiza gr�fico: exibe Eqptos a serem movidos
	graf->Redraw(redrawEQP_NOVO, this);
	// atualiza cursor
	graf->Cursor(TCursor(crHandPoint));
	// atualiza estado
	estado = estSPIN;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::SelecionaCaminhoEletrico(VTEqpto* eqpto1, VTEqpto *eqpto2, TList *lisEQP)
{
	// reinicia lisEQS
	lisEQS->Clear();
	// obt�m Ligacoes entre os dois Eqptos
	caminho->Executa(eqpto1, eqpto2, lisEQS);
	/*
	 //C�DIGO TEMPOR�RIO: SELECIONA TODAS LIGA��ES CONECTADAS
	 caminho->Executa(eqpto2, lisEQP);
	 */
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::VerificaEqptoBloqueado(TList *lisEQP)
{
	// vari�veis locais
	VTEqpto *eqpto;

	// prote��o
	if ((edita->Obra == NULL) || (edita->Obra->Obras == NULL))
		return (false);
	// verifica se existe alguma Barra de Corte na lista lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (edita->Obra->Obras->VerificaEqptoBloqueado(eqpto))
		{ // atualiza grafico
			if (lisEQS->Count > 0)
			{ // atualiza gr�fico
				graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
				// limpa listas
				lisEQS->Clear();
			}
			else if (lisEQN->Count > 0)
			{ // atualiza grafico
				graf->Redraw(redrawEQP_NOVO, this);
				// limpa listas
				lisEQN->Clear();
			}
			// cancela marcas
			// MarcaCancela();
			// atualiza estado
			// estado = estSEL0;
			DefineEstadoSEL0();
			// gera aviso
			AvisoEqptoBloqueado(eqpto);
			return (true);
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::VerificaEqptoBloqueado(VTEqpto *eqpto)
{
	// prote��o
	if ((edita->Obra == NULL) || (edita->Obra->Obras == NULL))
		return (false);
	if (edita->Obra->Obras->VerificaEqptoBloqueado(eqpto))
	{
		AvisoEqptoBloqueado(eqpto);
		return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
// eof
