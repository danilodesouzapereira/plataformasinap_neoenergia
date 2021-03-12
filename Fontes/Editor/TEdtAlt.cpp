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
// função global p/ criar objetos da classe TEdtAlt
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
	// mais nada a fazer: o contrutor de VTEdt faz todas iniciações
}

// ---------------------------------------------------------------------------
__fastcall TEdtAlt::~TEdtAlt(void)
{
	// destrói objetos
	if (caminho)
	{
		delete caminho;
		caminho = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	// mais nada a fazer: o destrutor de VTEdt faz todas finalizações
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::AvisoEqptoBloqueado(VTEqpto *eqpto)
{
	// variáveis locais
	AnsiString txt;
	AnsiString tipo = eqpto->TipoAsString();
	AnsiString codigo = AnsiQuotedStr(eqpto->Codigo, '\'');

	Aviso(txt.sprintf("%s %s:\nEquipamento associado a obra posterior não pode ser alterado.",
		tipo.c_str(), codigo.c_str()));
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::BuscaEqp(void)
{
	// variáveis locais
	TList *lisEQP;

	// varifica se há alguma Barra nas coordenadas
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lisEQP = busca->Eqpto(area);
	if (lisEQP->Count == 0)
	{ // não encontrou nenhum eqpto: muda formato do mouse
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
		// mútua
	case eqptoMUTUA:
		graf->Cursor(TCursor(curMutua));
		break;
		// rede ou redução
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
	// variáveis locais
	TList *lis;

	// reinicia lista de Eqptos selecionados
	lisEQS->Clear();
	// obtém lista de Eqptos presentes na área
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
	// variáveis locais
	TList *lisFIG;

	// define a area como o ponto do evento para fazer a busca
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	// faz a busca de barra
	lisFIG = busca->Figura(area);
	// verifica se encontrou uma Figura
	if (lisFIG->Count == 0)
	{ // não encontrou nenhuma figura: muda formato do mouse
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
	// variáveis locais
	TList *lis;

	// reinicia lista de Eqptos selecionados
	lisEQS->Clear();
	// obtém lista de Eqptos presentes na área
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
	// variáveis locais
	int resp = mrCancel;
	TForm *Form;
	VTBarra *pbar_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // proteção: verifica Eqpto bloqueado
		if (VerificaEqptoBloqueado(pbarra))
			return (mrCancel);
		// cria Barra temporária p/ edição
		pbar_tmp = DLL_NewObjBarra(patamares->NumPatamar());
		pbar_tmp->CopiaAtributosDe(*pbarra);
		// cria form de edição conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtBar(NULL, apl, pbar_tmp, true);
		else
			Form = new TFormEdtBarMS(NULL, apl, pbar_tmp, true);
		// proteção
		if (Form == NULL)
			return (resp);
		// exibe Form como janela modal
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usuário alterou dados da Barra
			edita->Altera(pbarra, pbar_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Barra
			// retira a Barra e atualiza o gráfico
			edita->Retira(pbarra);
			// destrói Barra temporária
			DLL_DeleteEqpto(pbar_tmp);
			break;
		default: // usuário não alterou dados da Barra
			// destrói Barra temporária
			DLL_DeleteEqpto(pbar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói TFormEdtBar
	if (Form)
		delete Form;

	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaBateria(VTBateria *pbat)
{
	// variáveis locais
	int resp;
	TFormEdtBateria *FormEdtBat;
	VTBateria *pbat_tmp;

	try
	{ // cria Bateria temporária p/ edição
		pbat_tmp = DLL_NewObjBateria(patamares->NumPatamar());
		pbat_tmp->CopiaAtributosDe(*pbat);
		// cria TFormEdtBateria
		FormEdtBat = new TFormEdtBateria(NULL, apl, pbat_tmp, false);
		switch (resp = FormEdtBat->ShowModal())
		{
		case mrOk: // usuário alterou dados da Bateria
			edita->Altera(pbat, pbat_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Bateria
			edita->Retira(pbat);
			// destrói Bateria temporário
			DLL_DeleteEqpto(pbat_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Bateria temporário
			DLL_DeleteEqpto(pbat_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói TFormEdtBateria
	if (FormEdtBat)
		delete FormEdtBat;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCapacitor(VTCapacitor *pcap)
{
	// variáveis locais
	int resp;
	TFormEdtCap *FormEdtCap;
	VTCapacitor *pcap_tmp;

	try
	{ // cria Capacitor temporário p/ edição
		pcap_tmp = DLL_NewObjCapacitor(patamares->NumPatamar());
		pcap_tmp->CopiaAtributosDe(*pcap);
		// cria TFormEdtCapacitor
		FormEdtCap = new TFormEdtCap(NULL, apl, pcap_tmp);
		switch (resp = FormEdtCap->ShowModal())
		{
		case mrOk: // usuário alterou dados da Capacitor
			edita->Altera(pcap, pcap_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Capacitor
			edita->Retira(pcap);
			// destrói Capacitor temporário
			DLL_DeleteEqpto(pcap_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Capacitor temporário
			DLL_DeleteEqpto(pcap_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtCapacitor
	if (FormEdtCap)
		delete FormEdtCap;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCarga(VTCarga *pcarga)
{
	// variáveis locais
	int resp;
	TForm *Form;
	VTCarga *pcar_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// verifica se é uma Carga equivalente
	// if (pcarga->curva->NumeroValores == nvCURVA_RRCAR)
	// {
	// Aviso("Carga equivalente: edição bloqueada");
	// return (mrCancel);
	// }
	// verifica se Carga é IP
	if (pcarga->IP)
		return (EditaCargaIP(pcarga));
	try
	{ // cria Carga temporária p/ edição
		pcar_tmp = DLL_NewObjCarga(patamares->NumPatamar());
		pcar_tmp->CopiaAtributosDe(*pcarga);
		// cria form de edição conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtCar(NULL, apl, pcar_tmp, true);
		else
			Form = new TFormEdtCarMS(NULL, apl, pcar_tmp, true);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usuário alterou dados da Carga
			edita->Altera(pcarga, pcar_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Carga
			// retira carga e atualiza o gráfico
			edita->Retira(pcarga);
			// destrói Carga temporária
			DLL_DeleteEqpto(pcar_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Carga temporária
			DLL_DeleteEqpto(pcar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtCarga
	if (Form)
		delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCargaNL(VTCargaNL *pcarga)
{
	// variáveis locais
	int resp;
	TForm *Form;
	VTCargaNL *pcar_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // cria Carga temporária p/ edição
		pcar_tmp = DLL_NewObjCargaNL();
		// pcar_tmp->CopiaAtributosDe(*pcarga);
		// cria form de edição conforme o Editor
		Form = new TFormEdtCarNL(NULL, apl, pcarga, pcar_tmp);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usuário alterou dados da Carga
			// edita->Altera(pcarga, pcar_tmp);
			edita->Retira(pcarga);
			edita->InsereEqbar(pcar_tmp, pcarga->pbarra);
			// destrói Carga temporária
			DLL_DeleteEqpto(pcarga);
			break;
		case mrNo: // usuário solicitou exclusão da Carga
			// retira carga e atualiza o gráfico
			edita->Retira(pcarga);
			// destrói Carga temporária
			DLL_DeleteEqpto(pcar_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Carga temporária
			DLL_DeleteEqpto(pcar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtCarga
	if (Form)
		delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/index.cfm#tasks/14887876
int __fastcall TEdtAlt::EditaMotor(VTMotor *pmotor)
{
	// variáveis locais
	int resp;
	TForm *Form;
	VTMotor *pmotor_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{
		// cria Carga temporária p/ edição
		pmotor_tmp = DLL_NewObjMotor();
		pmotor_tmp->CopiaAtributosDe(*pmotor);
		// cria form de edição conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtMotor(NULL, apl, pmotor_tmp);
		else
			Form = new TFormEdtMotorMS(NULL, apl, pmotor_tmp); // motor simplificado
		// Form = new TFormEdtMotor(NULL, apl, pmotor_tmp);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usuário alterou dados da Carga
			edita->Altera(pmotor, pmotor_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Carga
			// retira carga e atualiza o gráfico
			edita->Retira(pmotor);
			// destrói Carga temporária
			DLL_DeleteEqpto(pmotor_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Carga temporária
			DLL_DeleteEqpto(pmotor_tmp);
			break;
		}
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
	// destrói FormEdtCarga
	if (Form)
		delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCargaIP(VTCarga *pcarga)
{
	// variáveis locais
	int resp;
	TFormEdtCarIP *FormEdtCarIP;
	VTCarga *pcar_tmp;

	try
	{ // cria Carga temporária p/ edição
		pcar_tmp = DLL_NewObjCarga(patamares->NumPatamar());
		pcar_tmp->CopiaAtributosDe(*pcarga);
		// cria TFormEdtCarga
		FormEdtCarIP = new TFormEdtCarIP(NULL, apl, pcar_tmp);
		switch (resp = FormEdtCarIP->ShowModal())
		{
		case mrOk: // usuário alterou dados da Carga
			edita->Altera(pcarga, pcar_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Carga
			// retira carga e atualiza o gráfico
			edita->Retira(pcarga);
			// destrói Carga temporária
			DLL_DeleteEqpto(pcar_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Carga temporária
			DLL_DeleteEqpto(pcar_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtCarga
	if (FormEdtCarIP)
		delete FormEdtCarIP;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaCapserie(VTCapserie *capserie)
{
	// variáveis locais
	int resp;
	TFormEdtCaps *FormEdtCaps;
	VTCapserie *caps_tmp;

	try
	{ // cria Capserie temporário p/ edição
		caps_tmp = DLL_NewObjCapserie(patamares->NumPatamar());
		caps_tmp->CopiaAtributosDe(*capserie);
		FormEdtCaps = new TFormEdtCaps(NULL, apl, caps_tmp);
		switch (resp = FormEdtCaps->ShowModal())
		{
		case mrOk: // usuário alterou dados do Capserie
			edita->Altera(capserie, caps_tmp);
			break;
		case mrNo: // usuário solicitou exclusão do Capserie
			edita->Retira(capserie);
			// destrói Capserie temporário
			DLL_DeleteEqpto(caps_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Capserie temporária
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
	// variáveis locais
	int resp;
	TForm *Form;
	VTChave *pchv_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));
	TVerificaMalhaEdicao *utilMalha = new TVerificaMalhaEdicao(apl);
	bool malhaIntAntes, malhaIntDepois, malhaExterna;

	malhaIntAntes = malhaIntDepois = malhaExterna = false;
	try
	{ // cria Chave temporária p/ edição
		pchv_tmp = DLL_NewObjChave(patamares->NumPatamar());
		pchv_tmp->CopiaAtributosDe(*pchave);
		if (editor->Completo)
			Form = new TFormEdtChv(NULL, apl, pchv_tmp, true);
		else
			Form = new TFormEdtChvMS(NULL, apl, pchv_tmp, true);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usuário alterou dados da Chave
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
				if (Confirma("Esta operação fechou malha.", "Deseja confirmar ?") != IDYES)
				{
					edita->ActionUndo->Execute();
				}
			}
			break;
		case mrNo: // usuário solicitou exclusão da Chave
			// retira Chave e atualiza o gráfico
			edita->Retira(pchave);
			// destrói Chave temporária
			DLL_DeleteEqpto(pchv_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Chave temporária
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
	// variáveis locais
	int resp;
	TFormEdtEqptos *FormEdtEqptos;

	try
	{ // cria FormEdtEqptos
		FormEdtEqptos = new TFormEdtEqptos(NULL, apl, lisEQS);
		switch (resp = FormEdtEqptos->ShowModal())
		{
		case mrOk: // usuário alterou dados dos Eqptos selecionados
			// Aviso("Falta implementar TEdita::AlteraLisEqpto()");
			// edita->AlteraLisEqpto(lisEQS);
			// reinicia lisEQS e gráfico
			lisEQS->Clear();
			graf->Redraw();
			break;
		case mrNo: // usuário solicitou retirada dos Eqptos selecionados
			edita->RetiraLisEqpto(lisEQS);
			// reinicia lisEQS e gráfico
			lisEQS->Clear();
			// graf->AreaRedeAltera();
			break;
		default: // usuário não alterou dados
			// atualiza gráfico
			graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
			// limpa listas
			lisEQS->Clear();
			break;
		}
		// destrói TFormEdtEqptos
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
	// variáveis locais
	int resp;
	TForm *Form;
	VTFiguraComentario *comentario = (VTFiguraComentario*)figura;

	// cria form de edição
	Form = new TFormEdtComentario(NULL, apl, comentario);
	// proteção
	if (Form == NULL)
		return (mrCancel);
	// exibe Form como janela modal
	resp = Form->ShowModal();
	if (resp == mrNo)
	{ // usuário retirou Comentario
		figuras->RetiraComentario(comentario);
		figura = NULL;
	}
	else
	{ // define Obra, caso ainda não esteja definida
		if (comentario->Obra == NULL)
			comentario->Obra = edita->Obra;
	}
	// destrói form
	delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaFigRetangulo(void)
{
	// variáveis locais
	int resp;
	TForm *Form;
	VTRetangulo *retangulo = (VTRetangulo*)figura;

	// cria form de edição
	Form = new TFormEdtRetangulo(NULL, apl, retangulo);
	// proteção
	if (Form == NULL)
		return (mrCancel);
	// exibe Form como janela modal
	resp = Form->ShowModal();
	if (resp == mrNo)
	{ // usuário retirou Retangulo
		figuras->RetiraRetangulo(retangulo);
		figura = NULL;
	}
	else
	{ // define Obra, caso ainda não esteja definida
		if (retangulo->Obra == NULL)
			retangulo->Obra = edita->Obra;
	}
	// destrói form
	delete Form;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaFiltro(VTFiltro *filtro)
{
	// variáveis locais
	int resp;
	TFormEdtFiltro *FormEdtFiltro;
	VTFiltro *filtro_tmp;

	try
	{ // cria Filtro temporário p/ edição
		filtro_tmp = DLL_NewObjFiltro();
		filtro_tmp->CopiaAtributosDe(*filtro);
		// cria TFormEdtFiltro
		FormEdtFiltro = new TFormEdtFiltro(NULL, apl, filtro_tmp);
		switch (resp = FormEdtFiltro->ShowModal())
		{
		case mrOk: // usuário alterou dados da Filtro
			edita->Altera(filtro, filtro_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Filtro
			edita->Retira(filtro);
			// destrói Filtro temporário
			DLL_DeleteEqpto(filtro_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Filtro temporário
			DLL_DeleteEqpto(filtro_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtFiltro
	if (FormEdtFiltro)
		delete FormEdtFiltro;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaGerador(VTGerador *pger)
{
	// variáveis locais
	int resp;
	TFormEdtGer *FormEdtGer;
	VTGerador *pger_tmp;

	try
	{ // cria Gerador temporária p/ edição
		pger_tmp = DLL_NewObjGerador(patamares->NumPatamar());
		pger_tmp->CopiaAtributosDe(*pger);
		FormEdtGer = new TFormEdtGer(NULL, apl, pger_tmp);
		switch (resp = FormEdtGer->ShowModal())
		{
		case mrOk: // usuário alterou dados do Gerador
			edita->Altera(pger, pger_tmp);
			break;
		case mrNo: // usuário solicitou exclusão do Gerador
			// retira a Gerador e atualiza o gráfico
			edita->Retira(pger);
			// destrói Gerador temporária
			DLL_DeleteEqpto(pger_tmp);
			break;
		default: // usuário não alterou dados do Gerador
			// destrói Gerador temporária
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
	// variáveis locais
	int resp;
	TFormEdtMedidor *FormEdtMedidor;
	VTMedidor *pmed_tmp;

	try
	{ // cria Medidor temporária p/ edição
		pmed_tmp = DLL_NewObjMedidor();
		pmed_tmp->CopiaAtributosDe(*pmed);
		FormEdtMedidor = new TFormEdtMedidor(NULL, apl, pmed_tmp);
		switch (resp = FormEdtMedidor->ShowModal())
		{
		case mrOk: // usuário alterou dados do Medidor
			edita->Altera(pmed, pmed_tmp);
			break;
		case mrNo: // usuário solicitou exclusão do Medidor
			// retira a Gerador e atualiza o gráfico
			edita->Retira(pmed);
			// destrói Medidor temporária
			DLL_DeleteEqpto(pmed_tmp);
			break;
		default: // usuário não alterou dados do Medidor
			// destrói Medidor temporária
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
	// variáveis locais
	int resp;
	TFormEdtMutua *FormEdtMutua;
	VTMutua *mutua_tmp;

	try
	{ // cria Mutua temporário p/ edição
		mutua_tmp = DLL_NewObjMutua();
		mutua_tmp->CopiaAtributosDe(*mutua);
		// cria TFormEdtMutua
		FormEdtMutua = new TFormEdtMutua(NULL, apl, mutua_tmp);
		switch (resp = FormEdtMutua->ShowModal())
		{
		case mrOk: // usuário alterou dados da Mutua
			edita->Altera(mutua, mutua_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Mutua
			edita->Retira(mutua);
			// destrói Mutua temporário
			DLL_DeleteEqpto(mutua_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Mutua temporário
			DLL_DeleteEqpto(mutua_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtMutua
	if (FormEdtMutua)
		delete FormEdtMutua;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaReator(VTReator *reator)
{
	// variáveis locais
	int resp;
	TFormEdtReator *FormEdtReator;
	VTReator *reator_tmp;

	try
	{ // cria Reator temporário p/ edição
		reator_tmp = DLL_NewObjReator(patamares->NumPatamar());
		reator_tmp->CopiaAtributosDe(*reator);
		// cria TFormEdtReator
		FormEdtReator = new TFormEdtReator(NULL, apl, reator_tmp);
		switch (resp = FormEdtReator->ShowModal())
		{
		case mrOk: // usuário alterou dados da Reator
			edita->Altera(reator, reator_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Reator
			edita->Retira(reator);
			// destrói Reator temporário
			DLL_DeleteEqpto(reator_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Reator temporário
			DLL_DeleteEqpto(reator_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtReator
	if (FormEdtReator)
		delete FormEdtReator;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaRegulador(VTRegulador *regulador)
{
	// variáveis locais
	int resp;
	TForm *FormEdtRegulador;
	VTRegulador *regulador_tmp;

	try
	{ // cria Regulador temporário p/ edição
		regulador_tmp = DLL_NewObjRegulador(patamares->NumPatamar());
		regulador_tmp->CopiaAtributosDe(*regulador);
		// verifica o tipo de regulador
		if (regulador_tmp->modelo == reguENERQCT)
		{ // cria Form p/ edição de regulador BT
			FormEdtRegulador = new TFormEdtReguladorBT(NULL, apl, regulador_tmp);
		}
		else
		{ // cria Form p/ edição de regulador MT
			FormEdtRegulador = new TFormEdtReguladorMT(NULL, apl, regulador_tmp);
		}
		switch (resp = FormEdtRegulador->ShowModal())
		{
		case mrOk: // usuário alterou dados da Regulador
			edita->Altera(regulador, regulador_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Regulador
			edita->Retira(regulador);
			// destrói Regulador temporário
			DLL_DeleteEqpto(regulador_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Regulador temporário
			DLL_DeleteEqpto(regulador_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtRegulador
	if (FormEdtRegulador)
		delete FormEdtRegulador;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaSuprimento(VTSuprimento *psup)
{
	// variáveis locais
	int resp;
	TForm *Form;
	VTSuprimento *psup_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // cria Suprimento temporário p/ edição
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
		case mrOk: // usuário alterou dados do Suprimento
			edita->Altera(psup, psup_tmp);
			break;
		case mrNo: // usuário solicitou exclusão do Suprimento
			// retira o Suprimento e atualiza o gráfico
			edita->Retira(psup);
			// destrói Suprimento temporário
			DLL_DeleteEqpto(psup_tmp);
			break;
		default: // usuário não alterou dados do Suprimento
			// destrói Suprimento temporário
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
	// variáveis locais
	bool criar_copia = true;
	int resp;
	VTEdicao *edicao = (VTEdicao*)apl->GetObject(__classid(VTEdicao));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	try
	{ // edita Trafo
		switch (resp = edicao->Executa(ptrafo, criar_copia))
		{
		case mrOk: // usuário alterou dados do Trafo
			if (ptrafo->Obj)
				edita->Altera(ptrafo, (VTTrafo*)ptrafo->Obj);
			break;
		case mrNo: // usuário solicitou exclusão do Trafo
			edita->Retira(ptrafo);
			break;
		default: // usuário não alterou dados do Trafo: nada a fazer
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
	// variáveis locais
	int resp;
	TFormEdtTrf3E *FormEdtTrf3E;
	VTTrafo3E *ptrafo3e_tmp;

	try
	{ // cria Trafo temporário p/ edição
		ptrafo3e_tmp = DLL_NewObjTrafo3E(patamares->NumPatamar());
		ptrafo3e_tmp->CopiaAtributosDe(*ptrafo3e);
		// cria TFormEdtTrf p/ edição do Trafo3E
		FormEdtTrf3E = new TFormEdtTrf3E(NULL, apl, ptrafo3e_tmp);
		switch (resp = FormEdtTrf3E->ShowModal())
		{
		case mrOk: // usuário alterou dados do Trafo
			edita->Altera(ptrafo3e, ptrafo3e_tmp);
			break;
		case mrNo: // usuário solicitou exclusão do Trafo
			edita->Retira(ptrafo3e);
			// destrói Trafo temporário
			DLL_DeleteEqpto(ptrafo3e_tmp);
			break;
		default: // usuário não alterou dados do Trafo
			// destrói Trafo temporário
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
	// variáveis locais
	int resp;
	TFormEdtTrfZZ *FormEdtTrfZZ;
	VTTrafoZZ *trafoZZ_tmp;

	try
	{ // cria TrafoZZ temporário p/ edição
		trafoZZ_tmp = DLL_NewObjTrafoZZ();
		trafoZZ_tmp->CopiaAtributosDe(*trafoZZ);
		// cria TFormEdtTrafoZZ
		FormEdtTrfZZ = new TFormEdtTrfZZ(NULL, apl, trafoZZ_tmp);
		switch (resp = FormEdtTrfZZ->ShowModal())
		{
		case mrOk: // usuário alterou dados da TrafoZZ
			edita->Altera(trafoZZ, trafoZZ_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da TrafoZZ
			edita->Retira(trafoZZ);
			// destrói TrafoZZ temporário
			DLL_DeleteEqpto(trafoZZ_tmp);
			break;
		default: // usuário não alterou dados
			// destrói TrafoZZ temporário
			DLL_DeleteEqpto(trafoZZ_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtTrafoZZ
	if (FormEdtTrfZZ)
		delete FormEdtTrfZZ;
	return (resp);
}

// ---------------------------------------------------------------------------
int __fastcall TEdtAlt::EditaTrecho(VTTrecho *ptrecho)
{
	// variáveis locais
	int resp;
	double dist_m;
	VTBarra *barra;
	TForm *Form;
	VTTrecho *ptrecho_tmp;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	try
	{ // cria Trecho temporário p/ edição
		ptrecho_tmp = DLL_NewObjTrecho(patamares->NumPatamar());
		ptrecho_tmp->CopiaAtributosDe(*ptrecho);
		// cria form de edição conforme o Editor
		if (editor->Completo)
			Form = new TFormEdtTre(NULL, apl, ptrecho_tmp, true);
		else
			Form = new TFormEdtTreMS(NULL, apl, ptrecho_tmp, true);
		switch (resp = Form->ShowModal())
		{
		case mrOk: // usuário alterou dados do Trecho
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
		case mrNo: // usuário solicitou exclusão do Trecho
			// retira o Trecho e atualiza o gráfico
			edita->Retira(ptrecho);
			// destrói Trecho temporário
			DLL_DeleteEqpto(ptrecho_tmp);
			break;
		default: // usuário não alterou dados do Trecho
			// destrói Trecho temporário
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
	// variáveis locais
	int resp;
	TFormEdtZref *FormEdtZref;
	VTYref *yref_tmp;

	try
	{ // cria Zref temporária p/ edição
		yref_tmp = DLL_NewObjYref();
		yref_tmp->CopiaAtributosDe(*yref);
		// cria TFormEdtZref
		FormEdtZref = new TFormEdtZref(NULL, apl, yref_tmp);
		switch (resp = FormEdtZref->ShowModal())
		{
		case mrOk: // usuário alterou dados da Zref
			edita->Altera(yref, yref_tmp);
			break;
		case mrNo: // usuário solicitou exclusão da Zref
			// retira carga e atualiza o gráfico
			edita->Retira(yref);
			// destrói Zref temporária
			DLL_DeleteEqpto(yref_tmp);
			break;
		default: // usuário não alterou dados
			// destrói Zref temporária
			DLL_DeleteEqpto(yref_tmp);
			break;
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// destrói FormEdtZref
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
	// variáveis locais
	int dx, dy;

	// atualiza gráfico: apaga Eqpto sendo movidos
	graf->Redraw(redrawEQP_NOVO, this);
	// calcula deslocamento
	dx = evt.x - evt.x_ant;
	dy = evt.y - evt.y_ant;
	// redefine coordenadas das Barras e das coordenadas dos Pontos da polyline dos Trechos
	edita->MoveBarra(lisBAR, dx, dy, true);
	// atualiza gráfico: exibe Eqpto selecionado
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
	// variáveis locais
	int dx, dy;

	// atualiza gráfico: apaga Eqpto sendo movidos
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
	// variáveis locais
	int dx, dy;

	// atualiza gráfico: apaga Figura sendo movida
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
	// atualiza gráfico: exibe Eqpto selecionado
	graf->Redraw(redrawFIG_NOVA, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveFig_EvtMouseUp(void)
{
	// limpa listas
	lisEQS->Clear();
	lisEQN->Clear();
	// atualiza gráfico
	graf->Redraw();
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveMarca_EvtMouseDown(void)
{
	// proteção
	if (marca == NULL)
		return;
	// atualiza Marca: início de Move
	marca->StartMove();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoMoveMarca_EvtMouseDownMove(void)
{
	// variáveis locais
	TList *lis;
	VTBarra *pbarra = NULL;

	// varifica se há uma Barra nas coordenadas
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lis = busca->Barra(area);
	if (lis->Count > 0)
		pbarra = (VTBarra*)lis->First();
	// atualiza gráfico: apaga Eqptos sendo movidos
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
	// atualiza gráfico: exibe Eqptos sendo movidos
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
	// varifica se há uma Barra nas coordenadas
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
	// atualiza gráfico: apaga Eqptos sendo movidos
	graf->Redraw(redrawMARCA, this);
	// destrói Marcas não utilizadas
	for (int n = lisMAR->Count - 1; n >= 0; n--)
	{
		marca_nao_usada = (VTMarca*)lisMAR->Items[n];
		if (marca_nao_usada != marca)
		{ // retira Marca da lista e destrói objeto
			lisMAR->Delete(n);
			delete marca_nao_usada;
		}
	}
	// cancela ponteiro p/ Marca
	// marca = NULL;
	// destrói Marca utilizada que ficou em lisMAR
	// LimpaTList(lisMAR);
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
	// atualiza o gráfico
	graf->Redraw();
	// executa tratamento externo
	if (pbarra != NULL)
		Done();
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TEdtAlt::EstadoSel0_EvtMouseDown(void)
 {
 // verifica se há algum Eqpto na área
 BuscaEqpto();
 if (eqpto == NULL)
 { // atualiza estado
 estado = estERRO;
 graf->DefMouseOp(mouseAREA);
 return;
 }
 // há eqpto na área: insere eqpto na lista de lisEQS
 lisEQS->Add(eqpto);
 // atualiza gráfico
 graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TEdtAlt::EstadoSel0_EvtMouseDown(void)
{
	// variáveis locais
	VTEqpto *eqp_sel;
	VTFigura *fig_sel;
	VTFiguraComentario *comentario;

	// verifica se há algum Eqpto na área
	if (BuscaFig())
	{ // verifica se Figura é um Comentario e a tecla Alt está apertada
		if ((sel.figura->TipoFigura == figCOMENTARIO) && (evt.Shift.Contains(ssAlt)))
		{ // altera formato do Comentario
			comentario = (VTFiguraComentario*)sel.figura;
			if (comentario->Formato == fmtFLAG)
				comentario->Formato = fmtRETANGULO;
			else
				comentario->Formato = fmtFLAG;
			// atualiza gráfico
			graf->Redraw();
		}
		else
		{ // salva Figura e insere na lista de lisEQS
			figura = sel.figura;
			lisEQS->Clear();
			lisEQS->Add(figura);
			// atualiza gráfico
			graf->Redraw(redrawFIG_SEL_COR_UNICA, this);
		}
	}
    else if (BuscaEqp())
	{ // há eqpto na área: salva Eqpto e insere na lista de lisEQS
		eqpto = sel.eqpto;
		lisEQS->Clear();
		lisEQS->Add(eqpto);
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
	else
	{ // redefine estado estSEL0
		// estado = estERRO;
		DefineEstadoSEL0();
		// pode estar sendo iniciada uma seleção de área
		graf->DefMouseOp(mouseAREA);
		return;
	}
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TEdtAlt::EstadoSel0_EvtMouseDownMove(void)
 {
 // proteção
 if (eqpto == NULL)
 return;
 // verifica se o Eqpto selecionado é um Eqbar
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
	// proteção
	if (eqpto != NULL)
	{ // verifica se o Eqpto selecionado é um Eqbar
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
//	// verifica se há algum Eqpto na área
//	if (BuscaEqp() == NULL)
//	{ // verifica se há alguma Figura
//		BuscaFig();
//	}
	// verifica se há algum Figura na área
	if (BuscaFig() == false)
	{ // verifica se há alguma Eqpto
		BuscaEqp();
	}
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TEdtAlt::EstadoSel0_EvtMouseUp(void)
 {
 if (lisEQS->Count == 1)
 { // cria marcas p/ eqpto selecionado e atualiza o gráfico
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
		{ // cria marcas p/ eqpto selecionado e atualiza o gráfico
			MarcaInicia();
		}
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16270997
void __fastcall TEdtAlt::EstadoSel1_EvtMouseDown(void)
{
	// verifica se está sendo selecionada uma Marca
	if (marca)
	{ // trata evento para Marca
		EstadoMoveMarca_EvtMouseDown();
		return;
	}
	// destrói Marcas e atualiza gráfico
	MarcaCancela();
	// verifica se há uma Figura selecionada
	if (figura != NULL)
		return;
	// verifica se há um Eqpto selecionado
	if ((eqpto != NULL) && (sel.eqpto == NULL))
	{ // verifica se a tecla Control ou Shift está apertada (o que indica seleção de Eqpto)
		if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
			return;
		// destrói Marcas e atualiza gráfico
		// MarcaCancela();
		// atualiza gráfico
		if (lisEQS->Count > 0)
			graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		// lisEQS->Clear();
		// eqpto  = NULL;
		// figura = NULL;
		// estado = estSEL0;
		DefineEstadoSEL0();
		// pode estar sendo iniciada uma seleção de área
		graf->DefMouseOp(mouseAREA);
		return;
	}
	// verifica se o Eqpto selecionado é igual ao Eqpto
	if ((eqpto != NULL) && (sel.eqpto == eqpto))
	{ // nada a fazer: o evento MouseUp abrirá janela de edição
		return;
	}
	// Eqpto selecionado não está em lisEQS: verifica se a tecla Control está apertada
	if (evt.Shift.Contains(ssCtrl))
	{ // insere eqpto
		if (lisEQS->IndexOf(sel.eqpto) < 0)
			lisEQS->Add(sel.eqpto);
		// cancela Marcas
		// MarcaCancela();
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		// atualiza estado
		estado = estSELN;
	}
	// verifica se a tecla Shift está apertada
	else if (evt.Shift.Contains(ssShift))
	{ // atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// seleciona todos Ligacoes interligados pelos 2 Eqptos selecionados
		SelecionaCaminhoEletrico(eqpto, sel.eqpto, NULL);
		// cancela Marcas
		// MarcaCancela();
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		// atualiza estado
		estado = estSELN;
	}
	else
	{ // cancela Marcas do Eqpto selecionado anteriormente
		// MarcaCancela();
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// salva o Eqpto selecionado
		eqpto = sel.eqpto;
		// atualiza estado
		estado = estSEL0;
		// reinicia lisEQS c/ Eqpto selecionado
		lisEQS->Clear();
		if(eqpto != NULL)
			lisEQS->Add(eqpto);
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel1_EvtMouseDownMove(void)
{
	// verifica se está sendo movida uma Marca
	if (marca)
	{ // verifica se é marca de uma Barra
		PreparaMoveMarca();
		EstadoMoveMarca_EvtMouseDownMove();
		return;
	}
	// verifica se há uma Figura selecionada
	if (figura != NULL)
		return;
	// verifica se o Eqpto selecionado é um Eqbar
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
//	// verifica se há alguma Marca na área
//	if (!BuscaMarca())
//	{ // não há Marca: procura Eqpto
//		if (!BuscaEqp())
//		{ // não há Eqpto: procura figura
//			BuscaFig();
//		}
//	}
	// verifica se há alguma Marca na área
	if (!BuscaMarca())
	{ // não há Marca: procura figura
		if (!BuscaFig())
		{ // não há figura: procura Eqpto
			BuscaEqp();
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSel1_EvtMouseUp(void)
{
	// variáveis locais
	int resp = mrOk;

	// verifica se a tecla Control ou Shift está apertada (o que indica seleção de Eqpto)
	if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
		return;
	// cancela Marcas
	MarcaCancela();
	// verifica se não há Eqpto ou Figura selecionado
	if ((eqpto == NULL) && (figura == NULL))
	{ // limpa listas
		// lisEQN->Clear();
		// lisEQS->Clear();
		// estado = estSEL0;
		DefineEstadoSEL0();
		return;
	}
	// verifica se há uma Figura selecionada
	if (figura != NULL)
	{ // verifica se é mesma Figura atualmente selecionda
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
				// sinaliza término de operação
				Done();
				break;
			case mrCancel: // Figura não foi alterada
			default:
				// atualiza gráfico: reapresenta Figura selecionada c/ sua cor original
				graf->Redraw(redrawFIG_SEL_COR_ORIG, this);
				break;
			}
		}
		else
		{ // atualiza gráfico: reapresenta Figura selecionada c/ sua cor original
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
	// verifica se há um Eqpto Selecionado
	if (eqpto != NULL)
	{ // verifica se é o mesmo Eqptop selecionado atualmente
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
				// sinaliza término de operação
				Done();
				break;
			case mrOk: // Eqpto foi alterado
				// sinaliza término de operação
				Done();
			case mrCancel: // Eqpto não foi alterado
			default:
				// atualiza gráfico: reapresenta eqpto selecionado c/ sua cor original
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
	// variáveis locais
	int index;

	// verifica se há algum Eqpto na área
	BuscaEqp();
	if (sel.eqpto == NULL)
	{ // verifica se a tecla Control ou Shift está apertada (o que indica seleção de Eqpto)
		if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
			return;
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		// lisEQS->Clear();
		// pode estar sendo iniciada uma seleção de área
		// estado = estERRO;
		// graf->DefMouseOp(mouseAREA);
		DefineEstadoSEL0();
		// pode estar sendo iniciada uma seleção de área
		graf->DefMouseOp(mouseAREA);
		return;
	}
	// verifica se o Eqpto selecionado já está em lisEQS
	if ((index = lisEQS->IndexOf(sel.eqpto)) >= 0)
	{ // Eqpto já está em lisEQS: verifica se a tecla Control está apertada
		if (evt.Shift.Contains(ssCtrl))
		{ // tecla Control está apertada: retira Eqpto da seleção
			graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
			// retira Eqpto de lisEQS
			lisEQS->Delete(index);
			// atualiza gráfico
			graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
			// verifica se deve atualizar estado p/ estSEL0 (pois o evento MouseUp mudará o estado p/ estSEL1)
			if (lisEQS->Count == 1)
				estado = estSEL0;
		}
		else
		{ // tecla Control não está apertada: edita Eqptos
			// EditaEqptos();
			// atualiza estado
			// estado = estSEL0;
		}
		return;
	}
	// Eqpto selecionado não está em lisEQS: verifica se a tecla Control está apertada
	if (evt.Shift.Contains(ssCtrl))
	{ // insere Eqpto na seleção
		lisEQS->Add(sel.eqpto);
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	}
	else
	{ // reinicia seleção: atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		lisEQS->Clear();
		// atualiza estado
		estado = estSEL0;
		// insere eqpto na lista de lisEQS
		lisEQS->Add(sel.eqpto);
		// atualiza gráfico
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
	// verifica se há algum Eqpto na área
	BuscaEqp();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EstadoSelN_EvtMouseUp(void)
{
	// verifica se a tecla Control ou Shift está apertada (o que indica seleção de Eqpto)
	if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
		return;
	// tecla Control não está apertada: edita Eqptos
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
	// atualiza gráfico: apaga Eqpto sendo reposicionado
	graf->Redraw(redrawEQP_NOVO, this);
	// reposiciona Eqbar
	// edita->MoveEqbar(eqpto, evt.x, evt.y);
	edita->MoveEqbar((VTEqbar*)eqpto, evt.x, evt.y, true);
	// atualiza gráfico: exibe Eqpto selecionado
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
	// atualiza gráfico
	// graf->Redraw();
	// indica alteração da rede p/ graf (que atualiza o gráfico)
	// graf->AreaRedeAltera();
	// atualiza estado
	// estado = estSEL0;
	DefineEstadoSEL0();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TEdtAlt::EvtDelete(void)
{
	// variáveis locais
	VTEqpto *eqpto;
	VTBarra *pbar;

	// proteção
	if (lisEQS->Count == 0)
	{
		Aviso("Selecione os equipamentos para exclusão");
		return;
	}
	// verifica se há um Figura selecionada
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
	// bloqueia a exclusão de Barra inicial de uma Rede
	if (ExisteBarraInicialDeRede(lisEQS))
	{
		Aviso("Não é possível eliminar barras iniciais de redes.");
		return;
	}
	// bloqueia a exclusão de Barra de Corte com Reducao
	if (ExisteBarraDeCorteComReducao(lisEQS))
	{
		Aviso("Não é possível eliminar barras que conectam redes equivalentes.");
		return;
	}
	// bloqueia exclusão de Eqpto envolvidos em Obras posteriores
	if (VerificaEqptoBloqueado(lisEQS))
		return;
	// pede confirmação do usuário
	if (Confirma("Os equipamentos selecionados serão excluídos.", "Confirma exclusão ?") != IDYES)
		return;
	/*
	 //OBS: na retirada da Barra são retiradas as Ligações conectadas a ela
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
	// IMPORTANTE: destrói Marcas pois uma Marca associa Eqpbar e Ligacao c/ barras temporárias
	MarcaCancela();
	// retira Eqptos
	edita->RetiraLisEqpto(lisBAR);
	lisBAR->Clear();
	// atualiza o gráfico  (a linha estava comentada por causa da tarefa 14422280, mas trouxe efeito colateral)
	// graf->Redraw();
	// redefine estado em função do número de Eqptos selecionados
	// estado = estSEL0;
	DefineEstadoSEL0();
	// DVK 2016.05.18
	Done();
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::EvtEscape(void)
{
	// verifica se há uma Marca selecionada
	if ((marca) || (eqpto == NULL))
	{ // cancela Marcas
		MarcaCancela();
		// atualiza gráfico
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
	// atualiza gráfico
	graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	// redefine estado em função do número de Eqptos selecionados
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
	// variáveis locais
	int delta = 1;
	int dx = 0, dy = 0;
	double escala;
	// VTBarra  *pbar;
	VTEqpto *eqpto;
	// VTMarca *pm;

	// proteção verifica se há Eqptos selecionados
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
		// proteção: verifica se há Eqptos bloqueados
		if (VerificaEqptoBloqueado(eqpto))
			return;
		if (eqpto->TipoEqbar())
		{ // reposiciona Eqbar
			int x1, y1, x2, y2;
			((VTEqbar*)eqpto)->CoordenadasEsquematico(x1, y1, x2, y2);
			edita->MoveEqbar((VTEqbar*)eqpto, x2 + dx, y2 + dy, false);
			// atualiza gráfico
			// graf->Redraw();
			// graf->AreaRedeAltera(); //indica alteração da rede p/ graf (que atualiza o gráfico)
			return;
		}
	}
	// monta lista c/ as Barras existentes em lisEQS
	MontaLisBarraSelecionada();
	// proteção: verifica se há Eqptos bloqueados
	if (VerificaEqptoBloqueado(lisBAR))
		return;
	// desloca todas as Barras: redefine coordenadas das Barras
	edita->MoveBarra(lisBAR, dx, dy, false);
	// atualiza gráfico
	// graf->AreaRedeAltera(); //indica alteração da rede p/ graf (que atualiza o gráfico)
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
	// busca Eqptos na área
	if (BuscaEqp(x1, y1, x2, y2))
	{ // atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		// redefine estado em função do número de Eqptos selecionados
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
	// atualiza gráfico
	graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	// redefine estado em função do número de Eqptos selecionados
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
	// variáveis locais
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
		// verifica se a Barra está presente em algum Corte
		for (int nc = 0; nc < lisCORTE->Count; nc++)
		{
			corte = (VTCorte*)lisCORTE->Items[nc];
			if (corte->ExisteBarra(barra))
			{ // verifica se o Corte tem alguma Redução
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
	// variáveis locais
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
	// variáveis locais
	VTInsere *insere = (VTInsere*)apl->GetObject(__classid(VTInsere));

	// proteção
	if (insere)
	{ // insere Redes da Reducao e faz a atualização do gráfico)
		insere->Executa(reducao);
	}
	else
	{ // atualiza o gráfico
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
	// atualiza gráfico
	graf->Redraw(redrawMARCA, this);
	// destrói Marcas
	LimpaTList(lisMAR);
	// reinicia ponteiro
	marca = NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::MarcaInicia(void)
{
	// variáveis locais
	VTBarra *pbar;
	VTEqbar *eqbar;
	VTEqpto *eqpto;
	VTLigacao *pliga;
	VTMarca *marca;

	// reinicia lisMAR
	lisMAR->Clear();
	// proteção: verifica existência de Eqpto bloueqado
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
	// atualiza gráfico
	graf->Redraw(redrawMARCA, this);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAlt::MontaLisBarraSelecionada(void)
{
	// variáveis locais
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
		{ // insere Barra fictícia do esquemático da Rede
			rede = (VTRede*)eqpto;
			eqpto = rede->BarraFicticia();
			if ((eqpto != NULL) && (lisBAR->IndexOf(eqpto) < 0))
				lisBAR->Add(eqpto);
		}
		else if (eqpto->Tipo() == eqptoREDUCAO)
		{ // insere Barras fictícias da Reducao
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
	// variáveis locais
	VTBarra *pbar;
	TList *lisLIG;

	// monta lista c/ Barras que devem ser movidas
	MontaLisBarraSelecionada();
	// proteção
	if (VerificaEqptoBloqueado(lisBAR))
		return false;
	// determina Ligacoes existentes na área
	lisLIG = busca->Ligacao(lisBAR);
	// monta lisEQN c/ todos os Eqptos: Ligacoes, Barras e respectivos Eqbar
	lisEQN->Clear();
	if (lisLIG->Count != 0)
	{ // copia Ligacoes
		CopiaTList(lisLIG, lisEQN);
		// verifica se está modo esquemático
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
	{ // não há Ligacoes: insere as Barras
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
	// proteção
	if (VerificaEqptoBloqueado(lisEQN))
	{ // atualiza estado
		// estado = estSEL0;
		DefineEstadoSEL0();
		return false;
	}
	// atualiza gráfico: exibe Eqptos a serem movidos
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
	// variáveis locais
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
		// atualiza gráfico: exibe Eqptos a serem movidos
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
	// proteção
	if (VerificaEqptoBloqueado(lisEQS))
		return;
	// inicia lisEQN c/ lisEQS (que só tem o Eqpbar a ser reposicionado)
	lisEQN->Clear();
	CopiaTList(lisEQS, lisEQN);
	// atualiza gráfico: exibe Eqptos a serem movidos
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
	// obtém Ligacoes entre os dois Eqptos
	caminho->Executa(eqpto1, eqpto2, lisEQS);
	/*
	 //CÓDIGO TEMPORÁRIO: SELECIONA TODAS LIGAÇÕES CONECTADAS
	 caminho->Executa(eqpto2, lisEQP);
	 */
}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAlt::VerificaEqptoBloqueado(TList *lisEQP)
{
	// variáveis locais
	VTEqpto *eqpto;

	// proteção
	if ((edita->Obra == NULL) || (edita->Obra->Obras == NULL))
		return (false);
	// verifica se existe alguma Barra de Corte na lista lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (edita->Obra->Obras->VerificaEqptoBloqueado(eqpto))
		{ // atualiza grafico
			if (lisEQS->Count > 0)
			{ // atualiza gráfico
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
	// proteção
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
