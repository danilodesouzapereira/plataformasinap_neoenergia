// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormInsereMedicao.h"
#include "..\VTAjusta.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormInsereMedicao::TFormInsereMedicao(TComponent* Owner, VTApl *apl, VTAjusta *ajusta)
	: TForm(Owner)
{
	// salva ponteiros
	this->apl = apl;
	this->ajusta = ajusta;

	// cria lista de medidores na saida de cada alimentador
	lisMED = new TList();
	// inicia redes no cbox
	CBoxRedeManualInicia();
	// insere medidores de cada alimentador
	IniciaLisMed();
	// acerta canais dos medidores, se necessario
	IniciaMed();
	AtualizaSgrid();
}

// ---------------------------------------------------------------------------
__fastcall TFormInsereMedicao::~TFormInsereMedicao(void)
{
	// destroi lista sem destruir objetos
	DestroiTObject(lisMED);
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::ActionConfirmaExecute(TObject *Sender)
{
	// atualiza ultimas alterações
	if (!AtualizaMedidor())
		return;
	// retira medidores cuja curva medida permaneceu igual à curva estimada
	if (!RetiraMedidoresInalterados())
		return;
	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::ActionFechaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::ActionRedeChangeExecute(TObject *Sender)
{
	// salva rede selecionada
	ult_rede = (VTRede*)cboxRedeManual->Items->Objects[cboxRedeManual->ItemIndex];
	// Atualiza sgrid
	AtualizaSgrid();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormInsereMedicao::AtualizaMedidor(void)
{
	// variáveis locais
	VTMedidor *medidor;
	VTCanal *canal_P, *canal_Q;
	VTChave *chave;
	VTPatamar *patamar;
	double valor;
	int valores_zerados = 0;

	// proteção
	if (ult_rede == NULL)
		return false;
	// determina medidor da rede selecionada
	medidor = ExisteMedDJ(ult_rede);
	// proteção
	if (medidor == NULL)
		return false;
	chave = ExisteChave(ult_rede);
	if (chave == NULL)
		return false;
	// determina canais
	canal_P = medidor->ExisteCanal(chave, canalP, faseABC);
	canal_Q = medidor->ExisteCanal(chave, canalQ, faseABC);
	if ((canal_P == NULL) || (canal_Q == NULL))
		return false;

	// percorre patamares para validar valores
	for (int n = 0; n < sgridPQ->RowCount - 1; n++)
	{
		patamar = (VTPatamar*)sgridPQ->Objects[0][n + 1];
		// potencia ativa
		if (!StrToDouble(sgridPQ->Cells[1][n + 1], valor))
		{
			Aviso("Valor inválido no patamar " + patamar->Periodo + "\n" +
				sgridPQ->Cells[1][n + 1]);
			return false;
		}
		if ((valor < 0) && (!ExisteGerador()))
		{ // aceita potencia negativa se tiver gerador
			Aviso("Potência ativa negativa no patamar " + patamar->Periodo + "\n" +
				sgridPQ->Cells[1][n + 1]);
			return false;
		}
		if (IsDoubleZero(valor))
		{
			valores_zerados++;
		}
		// potencia reativa
		if (!StrToDouble(sgridPQ->Cells[2][n + 1], valor))
		{
			Aviso("Valor inválido no patamar " + patamar->Periodo + "\n" +
				sgridPQ->Cells[1][n + 1]);
			return false;
		}
	}
	if (valores_zerados > 0)
	{
		if (Confirma("Potência ativa zerada em algum patamar.", "Deseja manter zerado ?") != idYes)
			return false;
	}

	// percorre patamares para armazenar valores
	for (int n = 0; n < sgridPQ->RowCount - 1; n++)
	{
		patamar = (VTPatamar*)sgridPQ->Objects[0][n + 1];
		// potencia ativa
		if (StrToDouble(sgridPQ->Cells[1][n + 1], valor))
		{
			canal_P->ValorMedido[patamar] = valor / 1000.; // estava em Mega...
		}
		// potencia reativa
		if (StrToDouble(sgridPQ->Cells[2][n + 1], valor))
		{
			canal_Q->ValorMedido[patamar] = valor / 1000.; // estava em Mega...
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::AtualizaSgrid(void)
{
	// variáveis locais
	AnsiString hora, p_kw, q_kvar;
	VTMedidor *medidor;
	VTRede *rede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	int row_count = 1;

	// determina medidor da rede selecionada
	if (cboxRedeManual->ItemIndex < 0)
		return;
	rede = (VTRede*)cboxRedeManual->Items->Objects[cboxRedeManual->ItemIndex];
	medidor = ExisteMedDJ(rede);
	// proteção
	if (medidor == NULL)
		return;
	// define inicialmente 25 linhas
	sgridPQ->RowCount = 25;
	// cabeçalho
	sgridPQ->Cells[1][0] = "P(kW)";
	sgridPQ->Cells[2][0] = "Q(kvar)";
	for (int n = 0; n < pat01HORA; n++)
	{
		// verifica se patamar está ativo
		if (!ajusta->Patamares[n])
			continue;
		hora.sprintf("%02d:00 a %02d:00", n, n + 1);
		sgridPQ->Cells[0][row_count] = hora;
		// determina patamar
		patamar = patamares->Patamar[n];
		sgridPQ->Objects[0][row_count] = patamar;
		// insere valores
		p_kw = DeterminaP(medidor, n);
		q_kvar = DeterminaQ(medidor, n);
		sgridPQ->Cells[1][row_count] = p_kw;
		sgridPQ->Cells[2][row_count] = q_kvar;
		// incrementa contador de linhas
		row_count++;
	}
	// define quantidade real de linhas
	sgridPQ->RowCount = row_count;
	// define tamanho do form
	this->Height = 140 + (24 * row_count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::CBoxRedeManualInicia(void)
{
	// variáveis locais
	VTRede *rede;
	TList *lisREDE_VAL = ajusta->LisRedeVal();

	// limpa cbox
	cboxRedeManual->Clear();
	// insere um Primario p/ cada item do CBox
	for (int n = 0; n < lisREDE_VAL->Count; n++)
	{
		rede = (VTRede*)lisREDE_VAL->Items[n];
		cboxRedeManual->Items->AddObject(rede->Codigo, rede);
	}
	// pré-seleciona primeiro item e guarda ultima rede selecionada
	if (cboxRedeManual->Items->Count > 0)
	{
		cboxRedeManual->ItemIndex = 0;
		ult_rede = (VTRede*)cboxRedeManual->Items->Objects[cboxRedeManual->ItemIndex];
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::cboxRedeManualClick(TObject * Sender)
{
	// variaveis locais
	int index;
	// TNotifyEvent event;

	// event = cboxRedeManual->OnChange;
	// atualiza curva do medidor
	if (!AtualizaMedidor())
	{ // volta rede anterior
		cboxRedeManual->OnChange = NULL;
		index = cboxRedeManual->Items->IndexOfObject(ult_rede);
		cboxRedeManual->ItemIndex = index;
		return;
	}
	// atualiza
	// cboxRedeManual->OnChange = ActionRedeChange;
	ActionRedeChange->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::CompletaDadosCanal(VTCanal * canal)
{
	// variáveis locais
	VTChave *chave;

	// insere demais dados do canal novo
	canal->Codigo = canal->TipoCanalAsString;
	canal->Enabled = true;
	canal->Curva = DLL_NewObjCurva();
	canal->Curva->Tipica = false;
	canal->Curva->Escala = pat01HORA;
	canal->Curva->NumeroValores = nvCURVA_CANAL;
	canal->Curva->Reconfigura(pat01HORA);
	// verifica se Canal está associada a uma Chave
	if (canal->Eqpto->Tipo() == eqptoCHAVE)
	{ // verifica se a Chave é um Disjuntor
		chave = (VTChave*)canal->Eqpto;
		if (chave->TipoChave->Tipo == chaveDISJUNTOR)
			canal->Tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
		else
			canal->Tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
	}
	switch (canal->TipoCanal)
	{
	case canalP:
		canal->Curva->Unidade = unidKW;
		break;
	case canalQ:
		canal->Curva->Unidade = unidKVAR;
		break;
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::CopiaFluxoParaMedido(void)
{
	// variaveis locais
	VTCanal *canal;
	VTMedidor *medidor;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisCANAL;
	double valor;

	for (int n = 0; n < lisMED->Count; n++)
	{
		medidor = (VTMedidor*)lisMED->Items[n];
		lisCANAL = medidor->LisCanal();
		for (int c = 0; c < lisCANAL->Count; c++)
		{
			canal = (VTCanal*)lisCANAL->Items[c];
			for (int np = 0; np < pat01HORA; np++)
			{
				patamar = patamares->Patamar[np];
				valor = canal->ValorMedido[patamar];
				if (IsDoubleZero(valor))
				{ // copia valor estimado para medido
					canal->ValorMedido[patamar] = canal->ValorEstimado[patamar];
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormInsereMedicao::CurvaEstimadaIgualMedida(VTMedidor * medidor)
{
	// variaveis locais
	VTCanal *canal;
	TList *lisCANAL = medidor->LisCanal();
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;

	for (int n = 0; n < lisCANAL->Count; n++)
	{
		canal = (VTCanal*)lisCANAL->Items[n];
		// compara curva estimada com curva medida
		for (int np = 0; np < pat01HORA; np++)
		{
			patamar = patamares->Patamar[np];
			if (!IsDoubleZero(canal->ValorEstimado[patamar] - canal->ValorMedido[patamar]))
				return false;
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormInsereMedicao::DeterminaP(VTMedidor * medidor, int ind_pat)
{
	// variaveis locais
	double valor = 0.;
	VTCanal *canal;
	TList *lisCANAL = medidor->LisCanal();
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;

	// determina patamar
	patamar = patamares->Patamar[ind_pat];
	// determina canal P trifasico
	for (int n = 0; n < lisCANAL->Count; n++)
	{
		canal = (VTCanal*)lisCANAL->Items[n];
		if (canal->TipoCanal != canalP)
			continue;
		if (canal->Fases != faseABC)
			continue;
		valor = canal->ValorMedido[patamar] * 1000.; // estava em Mega...
		// retorna valor
		return (DoubleToStr("%4.3f", valor));
	}
	return "";
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormInsereMedicao::DeterminaQ(VTMedidor * medidor, int ind_pat)
{
	// variaveis locais
	double valor = 0.;
	VTCanal *canal;
	TList *lisCANAL = medidor->LisCanal();
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;

	// determina patamar
	patamar = patamares->Patamar[ind_pat];
	// determina canal Q trifasico
	for (int n = 0; n < lisCANAL->Count; n++)
	{
		canal = (VTCanal*)lisCANAL->Items[n];
		if (canal->TipoCanal != canalQ)
			continue;
		if (canal->Fases != faseABC)
			continue;
		valor = canal->ValorMedido[patamar] * 1000.; // estava em Mega...
		// retorna valor
		return (DoubleToStr("%4.3f", valor));
	}
	return "";
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TFormInsereMedicao::ExisteChave(VTRede * rede)
{
	// variaveis locais
	VTBarra *barra;
	TList *lisEQP = new TList();
	bool encontrou = false;
	VTEqpto *eqpto;
	VTChave *chave;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se há barra inicial
	if ((barra = rede->BarraInicial()) == NULL)
		return NULL;
	// determina ligações da barra inicial
	redes->LisLigacao(lisEQP, barra);

	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->Tipo() != eqptoCHAVE)
			continue;
		chave = (VTChave*)eqpto;
		// verifica se é da rede
		if (chave->rede == rede)
		{
			encontrou = true;
			break;
		}
	}

	// destroi lista
	delete lisEQP;
	// verifica se encontrou
	if (encontrou)
		return chave;
	return NULL;
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TFormInsereMedicao::ExisteGerador(void)
 {
 // variaveis locais
 TList *lisEQP = new TList();
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 bool existe = false;

 // lista geradores
 redes->LisEqbar(lisEQP, eqptoGERADOR);
 // conta geradores
 existe = (lisEQP->Count > 0);
 if (existe)
 { // verifica se possui na própria rede
 lisEQP->Clear();
 ult_rede->LisEqbar(lisEQP, eqptoGERADOR);
 if (lisEQP->Count == 0)
 {//verifica se existe na MRede
 lisEQP->Clear();
 ult_rede->MRede->LisEqbar(lisEQP, eqptoGERADOR);
 if (lisEQP->Count == 0)
 {//verifica se existe na MNet
 lisEQP->Clear();
 ult_rede->MRede->MNet->LisEqbar(lisEQP, eqptoGERADOR);
 if (lisEQP->Count == 0)
 existe = false;
 }
 }
 }
 // destroi lista
 delete lisEQP;
 // verifica se encontrou
 return existe;
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormInsereMedicao::ExisteGerador(void)
{
	// variaveis locais
	TList *lisEQP = new TList();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool existe = false;

	// lista geradores
	redes->LisEqbar(lisEQP, eqptoGERADOR);
	// conta geradores
	existe = (lisEQP->Count > 0);
	if (existe)
	{ // verifica se existe na MNet
		if (ult_rede->MRede->MNet->LisGerador()->Count == 0)
			existe = false;
	}
	// destroi lista
	delete lisEQP;
	// verifica se encontrou
	return existe;
}

// ---------------------------------------------------------------------------
VTMedidor* __fastcall TFormInsereMedicao::ExisteMedDJ(VTRede * rede)
{
	// variaveis locais
	VTMedidor *medidor;
	VTChave *chave;
	VTCanal *canal;

	// determina chave
	chave = ExisteChave(rede);
	if (chave == NULL)
		return NULL;
	for (int n = 0; n < lisMED->Count; n++)
	{ // verifica se há algum medidor com canal ligado à chave
		medidor = (VTMedidor*)lisMED->Items[n];
		canal = medidor->ExisteCanal(chave, canalP, faseABC);
		if (canal != NULL)
			return medidor;
	}
	return NULL;
}

// ---------------------------------------------------------------------------
VTMedidor* __fastcall TFormInsereMedicao::ExisteMedSemCanal(VTRede * rede)
{
	// variaveis locais
	VTBarra *barra;
	VTLigacao *ligacao;
	VTMedidor *medidor;
	TList *lisLIG = new TList();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// proteção
	barra = rede->BarraInicial();
	if (barra == NULL)
		return NULL;
	// determina ligacoes iniciais da rede
	redes->LisLigacao(lisLIG, barra);
	for (int n = 0; n < lisMED->Count; n++)
	{
		medidor = (VTMedidor*)lisMED->Items[n];
		if (medidor->LisCanal()->Count > 0)
			continue;
		if (medidor->Barra == barra)
			return medidor;
		// verifica se está em uma ligacao da barra inicial
		for (int nl = 0; nl < lisLIG->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			if (ligacao->rede != rede)
				continue;
			if (medidor->Barra == ligacao->pbarra1)
			{
				delete lisLIG;
				return medidor;
			}
			if (medidor->Barra == ligacao->pbarra2)
			{
				delete lisLIG;
				return medidor;
			}
			if (medidor->Barra == ligacao->pbarra3)
			{
				delete lisLIG;
				return medidor;
			}
		}
	}
	// destroi lista
	delete lisLIG;
	return NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::FormClose(TObject * Sender, TCloseAction & Action)
{
	// fecha o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::IniciaLisMed(void)
{
	// variaveis locais
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMED_DJ = medicoes->LisMedicaoDJ();

	// copia medidores
	lisMED->Clear();
	CopiaTList(lisMED_DJ, lisMED);
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::IniciaMed(void)
{
	// variáveis locais
	VTChave *chave;
	VTRede *rede;
	VTMedidor *medidor;
	TList *lisREDE_VAL = ajusta->LisRedeVal();
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	// valida cada medidor de cada rede
	for (int n = 0; n < lisREDE_VAL->Count; n++)
	{
		rede = (VTRede*)lisREDE_VAL->Items[n];
		chave = ExisteChave(rede);
		medidor = ExisteMedDJ(rede);
		// verifica se medidor está nulo
		if (medidor == NULL)
		{ // determina um medidor sem canais na mesma barra que a ligacao inicial da rede
			medidor = ExisteMedSemCanal(rede);
		}
		if (medidor != NULL)
		{
			ValidaCanais(medidor, chave);
		}
	}
	// insere valores da curva estimada
	medicoes->AtualizaMedidores();
	// copia valores do fluxo para ser a medição, caso o valor esteja zerado
	CopiaFluxoParaMedido();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormInsereMedicao::RetiraMedidoresInalterados(void)
{
	// variaveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTMedidor *medidor;
	TList *lisMED = medicoes->LisMedidor();
	TList *lisDEL;

	try
	{ // cria lista
		lisDEL = new TList();
		for (int n = lisMED->Count - 1; n >= 0; n--)
		{ // valida medidor
			medidor = (VTMedidor*)lisMED->Items[n];
			if (CurvaEstimadaIgualMedida(medidor))
			{
				lisMED->Delete(n);
				lisDEL->Add(medidor);
			}
		}
		// retira medidores
		edita->RetiraLisEqpto(lisDEL);
		// destroi lista
		delete lisDEL;
	}
	catch (Exception &e)
	{
		Aviso("Erro ao remover medidores não alterados: " + e.Message);
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsereMedicao::ValidaCanais(VTMedidor * medidor, VTChave * chave)
{
	// variaveis locais
	VTCanal *canal;

	// verifica se existe canal P
	canal = medidor->ExisteCanal(chave, canalP, faseABC);
	if (canal == NULL)
	{ // cria canal
		canal = medidor->InsereCanal(chave, canalP, faseABC);
		CompletaDadosCanal(canal);
	}
	// verifica se existe canal Q
	canal = medidor->ExisteCanal(chave, canalQ, faseABC);
	if (canal == NULL)
	{ // cria canal
		canal = medidor->InsereCanal(chave, canalQ, faseABC);
		CompletaDadosCanal(canal);
	}
}

// ---------------------------------------------------------------------------
