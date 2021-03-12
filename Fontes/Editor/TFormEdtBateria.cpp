// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtBateria.h"
#include "TFormEditaCurvaBateria3F.h"
#include "TFormExibeCurvaBateria.h"
#include "TImportaCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Curvas.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtBateria::TFormEdtBateria(TComponent* Owner, VTApl *apl, VTBateria *pbateria,
	bool op_retira) : TForm(Owner)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	this->pbateria = pbateria;
	// cria objetos
	peakShaving.listaCargas = new TList();
	// inicia dados default p/ Eqpto novo
	IniciaDadosBateriaNova();
	IniciaCBoxFase();
	IniciaCBoxPeakShaving();
	// cria TFormEditaCurva p/ edição da Curva
	FormEditaCurvaBateria3F = new TFormEditaCurvaBateria3F(NULL, this, apl, TabSheetCurvaCargaDescarga);
	FormEditaCurvaBateria3F->DefineCurva(curva.propria);
	FormEditaCurvaBateria3F->Show();

	// cria TFormExibeCurva p/ exibição da Curva
	FormExibeCurvaBateria = new TFormExibeCurvaBateria(this, apl, TabSheetGraficoCargaDescarga);
	FormExibeCurvaBateria->gbTipo->Visible = true;
	FormExibeCurvaBateria->Show();


	//*perfumaria* fazer no final
	// posiciona o Form
	// FormIniciaPosicao();

}

// ---------------------------------------------------------------------------
__fastcall TFormEdtBateria::~TFormEdtBateria(void)
{
	if (peakShaving.listaCargas != NULL)
		delete peakShaving.listaCargas; peakShaving.listaCargas = NULL;
    delete[]peakShaving.vet_pot_kw;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionAlignDownExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada em cima
	if (Align == alTop)
		Align = alNone;
	// alinha janela em baixo
	Align = alBottom;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionAlignLeftExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada à direita
	if (Align == alRight)
		Align = alNone;
	// alinha janela à esquerda
	Align = alLeft;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionAlignNoneExecute(TObject *Sender)
{
	// cancela alinhamento da janela (flutuante)
	Align = alNone;
	Top = janela.top;
	Left = janela.left;
	Width = janela.width;
	Height = janela.height;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionAlignRightExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada à esquerda
	if (Align == alLeft)
		Align = alNone;
	// alinha janela à direita
	Align = alRight;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionAlignUpExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada em baixo
	if (Align == alBottom)
		Align = alNone;
	// alinha janela em cima
	Align = alTop;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionConfirmaExecute(TObject *Sender)
{
	// validas campos com entrada numérica
	if (!ValidaDadosNumericos())
		return;
	// le os dados inseridos pelo usuário e salva-os no objeto bateria
	if (!LeDadosBateria())
		return;
	// le os dados da curva
	if (!LeDadosCurva())
		return;
	// verifica se o modo escolhido foi o peak shaving
	if (RGroupPeakShaving->ItemIndex == 0 && RGroupModoOperacao->ItemIndex == 0)
	{
		// le as informações referentes ao peakshaving
		if (!LePeakShaving())
			return;
		// realiza o peak shaving
		if (!RealizaPeakShaving())
			return;
	}
	// preenche os vetores requisitados pelo cálculo
	if (!PreencheVetoresPotencia())
		return;

	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionEdtExecute(TObject *Sender)
{
	// OBS: método a ser redefinido nas classes derivadas

	// sinaliza mouse ativo
	// AtualizaIndicacaoEditorOn(butEdt);
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionImportaExecute(TObject *Sender)
{
	// variaveis locais
	AnsiString filename;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	TImportaCurva *importa = new TImportaCurva(apl, pbateria->curvaCargaDescarga);

	// inicia opendialog
	OpenDialog->InitialDir = path->DirImporta();
	OpenDialog->Filter = "Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*";
	if (OpenDialog->Execute())
	{
		if (OpenDialog->Files->Count != 1)
			return;
		filename = OpenDialog->Files->Strings[0];
		if (importa->Executa(filename))
		{ // importa curva a partir do arquivo
			FormEditaCurvaBateria3F->DefineCurva(pbateria->curvaCargaDescarga);
			PageControlBateria->ActivePage = TabSheetCurvaCargaDescarga;
		}
	}
	if (importa)
		delete importa;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionMouseOffExecute(TObject *Sender)
{
	// liga indicação de mouse ativo e força botão Down
	AtualizaIndicacaoEditorOff();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionMouseOnExecute(TObject *Sender)
{
	// liga indicação de mouse ativo e força botão Down
	AtualizaIndicacaoEditorOn(button_down);
	// executa método Ativo
	Ativo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ActionSelModoOperacaoExecute(TObject *Sender)
{
	// verifica o modo de operação selecionado
	switch (RGroupModoOperacao->ItemIndex)
	{
	case 0: // curva horária
		RGroupPeakShaving->Enabled = true;
//		RGroupPeakShaving->ItemIndex = 1; // não
		if (RGroupPeakShaving->ItemIndex == 0) // sim
		{
			GBoxPeakShaving->Visible = true;
			GBoxCurvaCargaDescarga->Visible = false;
			pnlCurvaPropria->Visible = false;
			TabSheetModoOper->TabVisible = true;
			TabSheetModoOper->Caption = "Modo de Operação - Peak Shaving";
			GBoxCurvaCargaDescarga->TabOrder = 1;
			GBoxPeakShaving->TabOrder = 0;
			DefineTipOperManual();
			TabSheetCurvaCargaDescarga->TabVisible = false;
			TabSheetGraficoCargaDescarga->TabVisible = false;
			TabSheetCurvaEst->TabVisible = false;
			// Dica do FKM - Ajusta o tamanho do form \\
			this->AutoSize = true;
			this->AutoSize = false;
			break;
		}
		else
		{
			GBoxPeakShaving->Visible = false;
			GBoxCurvaCargaDescarga->Visible = true;
			GBoxCurvaCargaDescarga->TabOrder = 0;
			TabSheetModoOper->TabVisible = true;
			TabSheetModoOper->Caption = "Modo de Operação - Curva Horária";
			GBoxPeakShaving->TabOrder = 1;
			pnlCurvaPropria->Visible = true;
			DefineTipOperManual();
			TabSheetCurvaCargaDescarga->TabVisible = true;
			TabSheetGraficoCargaDescarga->TabVisible = true;
			TabSheetCurvaEst->TabVisible = true;
			// Dica do FKM - Ajusta o tamanho do form \\
			this->AutoSize = true;
			this->AutoSize = false;
			break;
		}
	case 1:  // automático
		RGroupPeakShaving->Enabled = false;
		GBoxPeakShaving->Visible = false;
		GBoxCurvaCargaDescarga->Visible = false;
		pnlCurvaPropria->Visible = false;
		DefineTipOperAuto();
		TabSheetCurvaCargaDescarga->TabVisible = false;
		TabSheetGraficoCargaDescarga->TabVisible = false;
		TabSheetModoOper->TabVisible = false;
		TabSheetCurvaEst->TabVisible = true;
		// Dica do FKM - Ajusta o tamanho do form \\
		this->AutoSize = true;
		this->AutoSize = false;
		break;
	default:
		Aviso("Selecione um modo de operação para o armazenamento de energia");
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::Ativo(void)
{
	// método a ser redefinido nas classes derivadas
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::Atualiza(void)
{
	// método a ser redefinido nas classes derivadas
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::AtualizaIndicacaoEditorOff(void)
{
	// indica mouse inativo
	ImageMouseOff->Visible = true;
	ImageMouseOn->Visible = false;
	// verifica se há um Button "abaixado"
	if (button_down)
	{ // "levanta" Button que está abaixado
		button_down->Down = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::AtualizaIndicacaoEditorOn(TToolButton *button)
{
	// salva ponteiro do Button
	button_down = button;
	if (button)
	{ // "abaixa" o Button
		button->Down = true;
		// indica mouse ativo
		ImageMouseOn->Visible = true;
		ImageMouseOff->Visible = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::CalculaEnergiaInicialPeakShaving(void)
{
	// calcula a duração do pré-carregamento, que começa após o horário de pico e vai até o dia acabar
	int duracaoPreCarregamento = pat01HORA - peakShaving.inicioDescarga_hr - peakShaving.duracaoDescarga_hr;
	//Calcula a potência durante o pré-carregamento
	double potenciaPreCarregamento = (peakShaving.taxaCarregamento_kW+pbateria->P_ociosa_pu*pbateria->Pnom_kw);
	// corrige ela usando o rendimento do carregamento
	double potenciaCorrigida =  potenciaPreCarregamento/pbateria->Rend_carga_pu;
	// calcula a energia em kWh
	double energiaPreCarregamento_kWh = potenciaCorrigida*duracaoPreCarregamento;
    // calcula a energia em pu
	double energiaPreCarregamento_pu = energiaPreCarregamento_kWh/pbateria->Enom_kwh;
	// armazena a quantidade de energia do pré carregamento na bateria como valor de inicio
	pbateria->Earmaz_inic_pu = energiaPreCarregamento_pu;

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::CalculaEnergiaPeakShaving(void)
{
	// variáveis locais
	double energia_kWh = 0;
	bool calculou;
	// percorre o vetor de potências a serem descarregadas no horário de pico
	for (int nv = 0; nv < peakShaving.duracaoDescarga_hr; nv++)
	{
		// calcula a energia somando as potências porque cada patamar tem uma hora
		energia_kWh += peakShaving.vet_pot_kw[nv];
	}
	// verifica se a energia demandada para o peak shaving está adequada
	if (ValidaEnergiaPeakShaving(energia_kWh))
	{
		// Se estiver, armazena o valor na estrutura do peakShaving
		peakShaving.energiaPeakShaving_kWh = energia_kWh;
		calculou = true;
	}
	else
	{
		// Se não estiver, para o processo e avisa o usuário
		Aviso("Energia necessária para o Peak Shaving maior do que a capacidade da bateria!");
		calculou = false;
	}

	return calculou;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::CalculaPotênciaCarregamentoPeakShaving(void)
{
	// calcula a potência necessária, a ser distribuida ao longo do dia, para garantir a energia para realizar o peak shaving
	peakShaving.taxaCarregamento_kW = (peakShaving.energiaPeakShaving_kWh/(24-peakShaving.duracaoDescarga_hr)+pbateria->P_ociosa_pu*pbateria->Pnom_kw)/pbateria->Rend_carga_pu;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::CBoxMBTChange(TObject *Sender)
{
	// reinicia CBoxFase
	IniciaCBoxFase();
	FormEditaCurvaBateria3F->SGridInicia();

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::CBoxFaseChange(TObject *Sender)
{
	FormEditaCurvaBateria3F->SGridInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::Centraliza(void)
{
	Left = (Parent->Width - Width) / 2;
	Top = (Parent->Height - Height) / 2;
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtBateria::CriaCurva(void)
{
	// variáveis locais
	VTCurva *curva;
	double valor[nvCURVA_PQ];

	// cria Curva
	curva = DLL_NewObjCurva();
	curva->Tipica = false;
	curva->Escala = patEDITAVEL;
	curva->Tipo = curvaPQ;
	curva->Unidade = unidKVA;
	curva->NumeroValores = nvCURVA_PQ;
	curva->Reconfigura(patEDITAVEL);

	return (curva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::DefineTipOperAuto(void)
{
	pbateria->Tipo_operacao = toAUTO;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::DefineTipOperManual(void)
{
	pbateria->Tipo_operacao = toMANUAL;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::DefineModoOper(int np, double valorP)
{
	if (IsDoubleZero(valorP,1e-2)) 	   pbateria->Modo_operacao[np] = moOCIOSA;
	else if (valorP > 0) 			   pbateria->Modo_operacao[np] = moCARGA;
	else 							   pbateria->Modo_operacao[np] = moDESCARGA;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::DefParent(TWinControl *parent)
{
	if (parent)
		Parent = parent;
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::editEArmazIniChange(TObject *Sender)
{
	if (StrToDouble(editEArmazIni->Text) > StrToDouble(editCapkWh->Text))
	{
		Aviso("A energia armazenada inicialmente não pode ser maior que a capacidade da bateria!");
		editEArmazIni->Text = "0.000";
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::EscondeAbas(void)
{
	if (pbateria->Tipo_operacao == toMANUAL)
	{
		// mostra as abas de curva e gráfica de carga e descarga inicialmente
		TabSheetCurvaCargaDescarga->TabVisible = true;
		TabSheetGraficoCargaDescarga->TabVisible = true;
	}
	else
	{
		// esconde as abas de curva e gráfica de carga e descarga inicialmente
		TabSheetCurvaCargaDescarga->TabVisible = false;
		TabSheetGraficoCargaDescarga->TabVisible = false;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::EscondeSeries(void)
{
	// esconde todas as Series
	for (int ns = 0; ns < Chart->SeriesCount(); ns++)
	{
		Chart->Series[ns]->Visible = false;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::EstadoBateriaInicia(void)
{
	// o default é não mostra a curva para o usuário não ver coisa que não deve
	Chart->Visible = false;
	if (!(pbateria->EstadoBateria()))
	{
		// avisa o usuário para rodar o fluxo de potência
		Aviso("Realize o fluxo de potência para gerar a curva de estado da bateria");
		// volta para a aba inicial
		PageControlBateria->ActivePage = TabSheetBateria;
		return;
	}
	else
	{
		// mostra o gráfico
		Chart->Visible = true;
		// preenche a curva de estado da bateria
		PreencheCurvaEstado();

	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ExibeDadosAvancado(void)
{
	// apresenta capacidade do inversor
	editCapkVA->Text = DoubleToStr("%4.3f", pbateria->Cap_inv_kva);
	// apresenta energia armazenada inicialmente
	editEArmazIni->Text = DoubleToStr("%4.3f", (pbateria->Earmaz_inic_pu*pbateria->Enom_kwh));
	// apresenta reserva mínima
	editReserva->Text = DoubleToStr("%4.3f", pbateria->Gatilho_carga_pu);
	// apresenta eficiencia de carga
	editEficienciaCarga->Text = DoubleToStr("%4.3f", pbateria->Rend_carga_pu);
	// apresenta eficiencia de descarga
	editEficienciaDescarga->Text = DoubleToStr("%4.3f", pbateria->Rend_desc_pu);
	// apresenta perdas ativas por inatividade
	editPerdasAtivasInatividade->Text = DoubleToStr("%4.3f", pbateria->P_ociosa_pu);
	// apresenta perdas reativas por inativade
	editPerdasReativasInatividade->Text = DoubleToStr("%4.3f", pbateria->Q_ociosa_pu);

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ExibeDadosBateria(void)
{
	// exibe dados Informações Gerais
	ExibeDadosInformacoesGerais();
	// exibe dados Avançado
	ExibeDadosAvancado();

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ExibeDadosFases(void)
{
	// inicia CBoxMBT(monofásica, bifásica, trifásica)
	CBoxMBT->ItemIndex = MBTfasica(pbateria->Fases) - cargaMONOFASICA;
	// inicia CBoxFase c/ as possíveis fases da Bateria
	IniciaCBoxFase();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ExibeDadosInformacoesGerais(void)
{
	// apresenta código
	editCodigo->Text = pbateria->Codigo;
	// apresenta capacidade
	editCapkWh->Text = DoubleToStr("%4.3f", pbateria->Enom_kwh);
	// apresenta potência nominal
	editPotNomkW->Text = DoubleToStr("%4.3f", pbateria->Pnom_kw);
	// apresenta fases
	ExibeDadosFases();
	// apresenta modelo de carga e descarga
	ExibeDadosModCargaDescarga();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::ExibeDadosModCargaDescarga(void)
{
	// inicia CBoxModCarga p/ identificar o modelo de carga da Carga
	switch (pbateria->ModeloCarga)
	{
	case cargaICTE:
		CBoxModCargaDescarga->ItemIndex = 0;
		break;
	case cargaSCTE:
		CBoxModCargaDescarga->ItemIndex = 1;
		break;
	case cargaZCTE:
		CBoxModCargaDescarga->ItemIndex = 2;
		break;
	default:
		CBoxModCargaDescarga->ItemIndex = 0;
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::FormShow(TObject *Sender)
{
	// exibe dados da Bateria
	ExibeDadosBateria();
	// seleciona TabSheet
	PageControlBateria->ActivePage = TabSheetCurvaCargaDescarga;
	PageControlBateria->ActivePage = TabSheetModoOper;
	PageControlBateria->ActivePage = TabSheetBateria;
	EscondeAbas();
	// arruma a aba de modo de operação para a primeira visualização
	IniciaMododeOperacao();
	// posiciona o Cursor em EditCodigo
	editCodigo->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaCBoxFase(void)
{
	// variáveis locais
	TFase *fase;
	TList *lisFASE;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// inicia lisFASE c/ o conjunto de fases definidas pelo CBoxMBT
	switch (CBoxMBT->ItemIndex)
	{
	case 0:
		lisFASE = fases->LisFasesCarga(cargaMONOFASICA);
		break;
	case 1:
		lisFASE = fases->LisFasesCarga(cargaBIFASICA);
		break;
	default:
		lisFASE = fases->LisFasesCarga(cargaTRIFASICA);
		break;
	}
	// reinicia CBoxFase
	CBoxFase->Clear();
	for (int n = 0; n < lisFASE->Count; n++)
	{
		fase = (TFase*)lisFASE->Items[n];
		CBoxFase->Items->AddObject(fase->codigo, fase);
	}
	// pré-seleciona fases da Carga
	if (CBoxFase->Items->Count < 0)
		return;
	CBoxFase->ItemIndex = CBoxFase->Items->IndexOf(fases->AsStrUpper[pbateria->Fases]);
	if (CBoxFase->ItemIndex < 0)
		CBoxFase->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaCBoxPeakShaving(void)
{
	// inicia os combo boxes para a realização do peak shaving
	IniciaCBoxPeakShavingCarga();
	IniciaCBoxPeakShavingHoraIni();
	IniciaCBoxPeakShavingDuracao();

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaCBoxPeakShavingCarga(void)
{
	//variáveis locais
	VTCarga *pcarga;

	//monta a lista de eqbares associados a barra a qual a bateria está associada
	MontaListaEqbar();
	//monta a lista de cargas associadas a barra a qual a bateria está associada
	MontaListaCargas();
	// reinicia CBoxPeakShavingCarga
	CBoxPeakShavingCarga->Clear();
	//proteção
	if (peakShaving.listaCargas->Count == NULL)
		return;
	//percorre todas as cargas associadas a barra a qual a bateria está associada e adciona ao combo box
	for (int nc = 0; nc < peakShaving.listaCargas->Count; nc++)
	{
		pcarga = (VTCarga*)peakShaving.listaCargas->Items[nc];
		CBoxPeakShavingCarga->Items->AddObject(pcarga->Codigo, pcarga);
	}
	// pré-seleciona a primeira Carga
	if (CBoxPeakShavingCarga->Items->Count < 0)
		return;
	CBoxPeakShavingCarga->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaCBoxPeakShavingDuracao(void)
{
	// reinicia CBoxPeakShavingDuracao
	CBoxPeakShavingDuracao->Clear();

	// como são só três items e isso vai ser assim pra sempre (a não ser que ANEEL mude as regras, o que é improvável)
	// vou inserir os itens na mão mesmo
	CBoxPeakShavingDuracao->Items->Add("1 hora");
	CBoxPeakShavingDuracao->Items->Add("2 horas");
	CBoxPeakShavingDuracao->Items->Add("3 horas");

	// pré-seleciona terceira duração (normalmente vai querer cobrir todo horario de pico)
	if (CBoxPeakShavingDuracao->Items->Count > 0)
		CBoxPeakShavingDuracao->ItemIndex = 2;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaCBoxPeakShavingHoraIni(void)
{
    // variáveis locais
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisPAT = patamares->LisPatamar();

	// reinicia CBoxPeakShavingHoraIni
	CBoxPeakShavingHoraIni->Clear();

	// percorre a lista de patamares
	for (int np = 0; np < lisPAT->Count; np++)
	{
		patamar = (VTPatamar*)lisPAT->Items[np];
		if (patamar->Enabled)
		{
			// insere o patamar no combo box
			CBoxPeakShavingHoraIni->Items->AddObject(patamar->Nome, patamar);
		}
	}
	// pré-seleciona o patamar das 18h
	if (CBoxPeakShavingHoraIni->Items->Count > 0)
		CBoxPeakShavingHoraIni->ItemIndex = 18;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaDadosBateriaNova(void)
{
	// variáveis locais
	// verifica se a Bateria ainda não tem uma Curva associada
	if (pbateria->curvaCargaDescarga == NULL)
	{ // cria Curva própria p/ a Bateria e salva o ponteiro
		pbateria->DefineCurva(CriaCurva());
		curva.propria = pbateria->curvaCargaDescarga;
	}
	else
	{ // salva ponteiro p/ Curva própria
		curva.propria = pbateria->curvaCargaDescarga;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaMododeOperacao(void)
{
	switch (pbateria->Tipo_operacao)
	{
		case toMANUAL:
			//peak shaving e curva horária
			RGroupPeakShaving->Enabled = true;
			GBoxCurvaCargaDescarga->TabOrder = 0;
			GBoxPeakShaving->Visible = false;
			RGroupModoOperacao->ItemIndex = 0;
			RGroupPeakShaving->ItemIndex = 1;
			// Dica do FKM - Ajusta o tamanho do form \\
			this->AutoSize = true;
			this->AutoSize = false;
			break;

		case toAUTO:
			// automático
			RGroupPeakShaving->Enabled = false;
			RGroupModoOperacao->ItemIndex = 1;
			TabSheetModoOper->TabVisible = false;
            break;
	};

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::IniciaSGrid(void)
{
	FormEditaCurvaBateria3F->SGridInicia();

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LeAvancado(void)
{
	// le dados inseridos pelo usuário no menu avançado
	double Enom = StrToDouble(editCapkWh->Text);
	pbateria->Cap_inv_kva = StrToDouble(editCapkVA->Text);
	pbateria->Earmaz_inic_pu = (StrToDouble(editEArmazIni->Text))/Enom;
	pbateria->Gatilho_carga_pu = StrToDouble(editReserva->Text);
	//em uma primeira análise, o gatilho de descarga é a maxima capacidade de armazenamento
	pbateria->Gatilho_desc_pu = (StrToDouble(editCapkWh->Text))/Enom;
	pbateria->Rend_carga_pu = StrToDouble(editEficienciaCarga->Text);
	pbateria->Rend_desc_pu = StrToDouble(editEficienciaDescarga->Text);
	pbateria->P_ociosa_pu = (StrToDouble(editPerdasAtivasInatividade->Text));
	pbateria->Q_ociosa_pu = (StrToDouble(editPerdasReativasInatividade->Text));

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LeDadosBateria(void)
{
	pbateria->Codigo = editCodigo->Text;
	// lê fases, mod.carga e descarga, le a aba informações gerais e a aba avançado
	if (!LeFases())
		return (false);
	if (!LeValorModCargaDescarga())
		return (false);
	if (!LeInformacaoGerais())
		return (false);
	if (!LeAvancado())
		return (false);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LeDadosCurva(void)
{
	bool leuDadosCurva = true;
	// lê dados da Curva própria
	if (!FormEditaCurvaBateria3F->LePontosCurva())
		leuDadosCurva = false;

	// seleciona Curva
	if (!SelecionaCurva())
		leuDadosCurva = false;

	return leuDadosCurva;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LeFases(void)
{
	// variáveis locais
	TFase *fase;

	// verifica se está selecionada uma fase
	if (CBoxFase->ItemIndex < 0)
	{
		Erro("Defina as fases da carga");
		return (false);
	}
	if ((fase = (TFase*)CBoxFase->Items->Objects[CBoxFase->ItemIndex]) == NULL)
		return (false);
	// atualiza fase da Carga
	pbateria->Fases = fase->tag;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LeInformacaoGerais(void)
{
	// le dados inseridos pelo usuário na aba informações gerais
	pbateria->Enom_kwh = StrToDouble(editCapkWh->Text);
	pbateria->Pnom_kw = StrToDouble(editPotNomkW->Text);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LePeakShaving(void)
{
	bool leuPeakShaving = true;
	// le dados inseridos pelo usuário referente as informações para o peak-shaving
	peakShaving.inicioDescarga_hr = CBoxPeakShavingHoraIni->ItemIndex;
	peakShaving.duracaoDescarga_hr = CBoxPeakShavingDuracao->ItemIndex + 1;
	peakShaving.vet_pot_kw = new double[peakShaving.duracaoDescarga_hr];
	// proteção: verifica se realmente existe alguma carga ligada à barra
	if (peakShaving.listaCargas->Count != NULL)
	{
		peakShaving.carga = (VTCarga*)CBoxPeakShavingCarga->Items->Objects[CBoxPeakShavingCarga->ItemIndex];
	}
	else
	{
		// avisa que não vai ser possível realizar o peak shaving
		Aviso("Não há nenhuma carga ligada à barra. Não é possível realizar o peak shaving");
        leuPeakShaving = false;
    }
	return(leuPeakShaving);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::LeValorModCargaDescarga(void)
{
	// variáveis locais
	double icte = 0., scte = 0., zcte = 0.;

	switch (CBoxModCargaDescarga->ItemIndex)
	{
	case 0:
		icte = 1.;
		break;
	case 1:
		scte = 1.;
		break;
	case 2:
		zcte = 1.;
		break;
	default:
		icte = 1.;
		break;
	}
	// atualiza Carga
	pbateria->Icte_pu = icte;
	pbateria->Scte_pu = scte;
	pbateria->Zcte_pu = zcte;
	return (true);
}

// ---------------------------------------------------------------------------
int __fastcall TFormEdtBateria::MBTfasica(int Fases)
{
	//observação: eu acredito que esse método se encaixa bem aqui, contudo, é possivel imaginar que ele deveria estar na classe TBateria //DSF
	switch (Fases)
	{
	case faseA:
	case faseB:
	case faseC:
		return (cargaMONOFASICA);
	case faseAN:
	case faseBN:
	case faseCN:
		return (cargaMONOFASICA);
	case faseAT:
	case faseBT:
	case faseCT:
		return (cargaMONOFASICA);
	case faseAB:
	case faseBC:
	case faseCA:
		return (cargaBIFASICA);
	case faseABN:
	case faseBCN:
	case faseCAN:
		return (cargaBIFASICA);
	case faseABC:
	case faseABCN:
		return (cargaTRIFASICA);
	default:
		return (cargaTRIFASICA);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::MontaListaCargas(void)
{
	VTEqbar *eqbar;
	// percorre a lista de eqbares
	for (int neqbar = 0; neqbar < peakShaving.listaEqbar->Count; neqbar++)
	{
		// pega o neqbar-ésimo elemento da lista
		eqbar = (VTEqbar*)peakShaving.listaEqbar->Items[neqbar];
		// se for uma carga, adciona a lista de cargas
		if (eqbar->Tipo() == eqptoCARGA)
		{
			peakShaving.listaCargas->Add(eqbar);
        }
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::MontaListaEqbar(void)
{
	// variável local
	VTBarra *pbarra;
	// pega a barra a qual a bateria (e logo a carga) está ligada
	pbarra = pbateria->pbarra;
	// pega a lista de equibares ligados a barra
	peakShaving.listaEqbar = pbarra->LisEqbar();

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::PageControlBateriaChange(TObject *Sender)
{
	if (PageControlBateria->ActivePage != TabSheetBateria)
	{
		FormEditaCurvaBateria3F->SGridInicia();
	}
	if (PageControlBateria->ActivePage == TabSheetGraficoCargaDescarga)
	{
		// atualiza FormExibeCurvaBateria
		FormExibeCurvaBateria->DefineCurva(pbateria, pbateria->curvaCargaDescarga);
		FormExibeCurvaBateria->CBoxTipoCurva->ItemIndex = 0;
		FormExibeCurvaBateria->HabilitaCurva(bat_P);
		FormExibeCurvaBateria->HabilitaCurva(bat_Q);
	}
	// se o usuário clicar na aba de curva de estado da bateria, verifica se é possível exibí-la
	if (PageControlBateria->ActivePage == TabSheetCurvaEst)
	{
		EstadoBateriaInicia();
	}
	if (PageControlBateria->ActivePage == TabSheetModoOper)
	{
		if (RGroupPeakShaving->ItemIndex == 0)
		{
			GBoxCurvaCargaDescarga->Visible = false;
			GBoxPeakShaving->Visible = true;
		}
		else
		{
			GBoxCurvaCargaDescarga->Visible = true;
			GBoxPeakShaving->Visible = false;
        }
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::PageControlBateriaChanging(TObject *Sender, bool &AllowChange)
{
	// verifica se a bateria está associada a uma Curva
	if (pbateria->curvaCargaDescarga == NULL)
	{
		Aviso("Selecione a curva da carga e descarga");
		AllowChange = false;
		return;
	}
	// verifica se deve ler dados editados
	if (PageControlBateria->ActivePage == TabSheetCurvaCargaDescarga)
	{ // lê os dados editados
		AllowChange = (LeDadosBateria()) && (LeDadosCurva());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::PreencheCurvaEstado(void)
{
	// variáveis locais
	double valor;
	TChartSeries *Serie;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	// garante que ele vá mostrar apenas a série que eu quero
	EscondeSeries();
	Serie = Chart->Series[0];
	// não sei se ta funcionando mas to deixando
	Serie->Title = "Energia (kWh)";
	Serie->Clear();
	Serie->Visible = true;

	for (int np = 0; np < patamares->NumPatamar(); np++)
	{
		//determina o valor no patamar
		valor = pbateria->Earmaz_atual_kwh[np];
		// insere ponto da curva na Serie
		Serie->AddXY(np, valor, IntToStr(np + 1), Serie->Color);
		Serie->AddXY(np + 1, valor, IntToStr(np + 1), Serie->Color);
	}

	// redimensiona a serie para ela ficar agradavel aos olhos
	RedimensionaSerie(Serie);

}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::PreencheCurvaPeakShaving(void)
{
	// variaveis locais
	double valor[nvCURVA_PQ];
	int nhps = 0;
	strHM hm_ini, hm_fim;
	hm_ini.minuto = 0;
	// reconfigura a curva de carga e descarga para 24 pontos de 01 horas
	pbateria->curvaCargaDescarga->Reconfigura(pat01HORA);
	// percorre os pontos
	for (int np = 0; np < pat01HORA; np++)
	{
		// pega a np-ésima hora para o inicio do patamar
		hm_ini.hora = np;
		// uma hora depois é o fim do patamar
		hm_fim.hora = np+1;
		// verifica se alcançou o horário de pico
		if (np == (peakShaving.inicioDescarga_hr+nhps))
		{
			// armazena o valor da potência ativa e reativa
			valor[0] = -(peakShaving.vet_pot_kw[nhps]); //P Ativa
			valor[1] = 0; // Q reativa
			// incrementa nhps, que é o contador do número de horas dedicadas ao peak shaving
			nhps++;
			// verifica se o número de horas dedicadas ao peak shaving é igual a duração esperada do peak shaving
			if (nhps == peakShaving.duracaoDescarga_hr)
				// zera o contador
				nhps = 0;
		}
		else
		{
            // preenche com o valor calculado para o carregamento
			valor[0] = peakShaving.taxaCarregamento_kW;; //P Ativa
			valor[1] = 0; // Q reativa
		}
		//adciona o ponto
		pbateria->curvaCargaDescarga->SetPonto(hm_ini,hm_fim, valor, nvCURVA_PQ);

	}
	return true;

}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::PreencheVetorPeakShaving(void)
{
	// variáveis locais
	VTDemanda	*demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
    // proteção: reconfigura a curva para 24 pontos
	peakShaving.carga->curva->Reconfigura(pat01HORA);
	// percorre todos os pontos da curva procurando o período de pico
	for (int np = 0, nhps = 0; (np < pat01HORA) && (nhps < peakShaving.duracaoDescarga_hr) ; np++) //nhps número de horas do horário de pico
	{
		// procura pelo patamar que corresponde ao patamar em que se inicia o peak shaving
		if (np == (peakShaving.inicioDescarga_hr+nhps))
		{
			// Armazena o valor da potência ativa do nhps-ésimo ponto do horário de pico
			peakShaving.vet_pot_kw[nhps] = (demanda->P_mw[peakShaving.carga][np])*1000;  //converte de MW para kW
            // incrementa o contador
			nhps++;

		}

	}
	return true;

}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::PreencheVetoresPotencia(void)
{
	// variaveis locais
	strHM      hm_ini, hm_fim;
	double     valor[nvCURVA_PQ];
	int passo = 0;
	int numPontosCurva = pbateria->curvaCargaDescarga->NumeroPontos(); // é também o número de grupos
	int numValores = pbateria->curvaCargaDescarga->NumeroValores;
	int pontosGrupo = pat01HORA/numPontosCurva; //número de pontos por grupo
	for (int npc = 0; npc < numPontosCurva; npc++)  // npc: número de pontos da curva
	{
		// pega o npc-ésimo ponto
		pbateria->curvaCargaDescarga->GetPonto(npc,hm_ini,hm_fim, valor, numValores);
		for (int npv = 0; npv < pontosGrupo; npv++) // npv: número de pontos do vetor
		{
			// define o modo de operação dependendo do valor de potência ativa do ponto
			DefineModoOper(npv+passo, valor[indP]);
			// preenche o vetor Pext_kw com os valores de potência ativa e reativa
			pbateria->Pext_kw[npv+passo] = fabs(valor[indP]);
			pbateria->Qext_kvar[npv+passo] = valor[indQ];
		}
        // incrementa o passo
		passo += pontosGrupo;
	}
	return true;

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::RealizaCalculosPeakShaving(void)
{
	// Usando as informações da carga, preenche o vetor de potências a serem descarregadas durante o horário de pico
	if (!PreencheVetorPeakShaving())
		return false;
	// Verifica se as taxas de descarregamento do vetor de potências estão de acordo com o que a bateria pode fornecer
	VerificaTaxaDescarregamentoPeakShaving();
	// Com o vetor em mãos, calcula a energia total necessária para realizar o peak shaving e valida ela
	if (!CalculaEnergiaPeakShaving())
		return false;
	// Sabendo a energia necessária para realizar o peak shaving, calcula o quanto de energia a bateria precisa armazenar e portanto
	// quanto potência precisa se distribuir ao longo do dia
	CalculaPotênciaCarregamentoPeakShaving();
	// Valida a taxa de carregamento
	if (!ValidaTaxaCarregamentoPeakShaving())
		return false;
	// Calcula o quanto será a energia inicial da bateria para que ela possa realizar o peak shaving
	CalculaEnergiaInicialPeakShaving();

	return true;
}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::RealizaPeakShaving(void)
{
	bool realizouPeakShaving = true;
	// Realiza o cálculo das grandezas que serão utilizadas no peak shaving
	if (!RealizaCalculosPeakShaving())
		realizouPeakShaving = false;
	// preenche a curva para a realização do peak shaving
	if (!PreencheCurvaPeakShaving())
		realizouPeakShaving = false;
	return realizouPeakShaving;
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::RedimensionaSerie(TChartSeries* Serie)
{
	// variáveis locais
	double val_min, val_max, delta;

	// reinicia dimensões do eixo Y
	if (Chart->LeftAxis->Minimum > 0)
	{
		Chart->LeftAxis->Minimum = 0;
		Chart->LeftAxis->Maximum = 0;
	}
	else
	{
		Chart->LeftAxis->Maximum = 0;
		Chart->LeftAxis->Minimum = 0;
	}

	val_max = Serie->MaxYValue();
	val_min = Serie->MinYValue();
	// inclui uma margem adicional nos valores máximo e mínimo
	delta = fabs(val_max) * 0.05;
	val_max += delta;
	val_min -= delta;
	// redimensiona o eixo Y
	if (val_max > 0)
	{
		Chart->LeftAxis->Maximum = val_max;
		Chart->LeftAxis->Minimum = val_min;
	}
	else
	{
		Chart->LeftAxis->Minimum = val_min;
		Chart->LeftAxis->Maximum = val_max;
	}

}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::SelecionaCurva(void)
{
	if ((pbateria->curvaCargaDescarga == NULL) || (pbateria->curvaCargaDescarga != curva.propria))
		{ // redefine Curva própria da Carga
			pbateria->curvaCargaDescarga = curva.propria;
		}
	return (pbateria->curvaCargaDescarga != NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::ValidaDadosNumericos(void)
{
    // verifica e valida dados numéricos dos edits
	if (!VerificaPorcentagem(StrToDouble(editReserva->Text)))
	{
		Aviso("Valor de reserva mínima inválido!");
		return false;
	}
	if (!VerificaPorcentagem(StrToDouble(editEficienciaCarga->Text)))
	{
		Aviso("Valor de eficiência de carga inválido!");
		return false;
	}
	if (!VerificaPorcentagem(StrToDouble(editEficienciaDescarga->Text)))
	{
		Aviso("Valor de eficiência de descarga inválido!");
		return false;
	}
	if (!VerificaPorcentagem(StrToDouble(editPerdasAtivasInatividade->Text)))
	{
		Aviso("Valor de perdas ativas por inatividade inválido!");
		return false;
	}
	if (!VerificaPorcentagem(StrToDouble(editPerdasReativasInatividade->Text)))
	{
		Aviso("Valor de perdas reativas por inatividade inválido!");
		return false;
	}
	return true;

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::ValidaEnergiaPeakShaving(double energia_kWh)
{
	// verifica se a energia demandada para o peak shaving é maior que a capacidade nominal da bateria
	if (energia_kWh > pbateria->Enom_kwh)
		return false;
	else
		return true;

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::ValidaTaxaCarregamentoPeakShaving(void)
{
	// difícil de acontecer, mas caso a taxa de carregamento for maior que a máxima potência que a bateria pode receber, o processo para
	if (!VerificaTaxaCarregamentoPeakShaving())
	{
		Aviso("Taxa de carregamento maior que o limite!");
		return false;
	}
	return true;

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::VerificaTaxaCarregamentoPeakShaving(void)
{
	// Conforme o explicado no método VerificaTaxaDescarregamentoPeakShaving,
	// a máxima potência que pode entrar ou sair da bateria é dada por S² >= P² + Q², mas para peak shaving Q = 0
    // Verifica se é o caso de P > S, se for, o processo é parado e o usuário será avisado
	if (peakShaving.taxaCarregamento_kW > pbateria->Cap_inv_kva)
	{
		return false;
	}
	return true;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtBateria::VerificaTaxaDescarregamentoPeakShaving(void)
{
	// percorre o vetor de potências a serem descarregas e verifica se a bateria é capaz de fornecer essa potência
	// a capacidade do inversor S² >= P² + Q², mas como no peak shaving Q = 0
	// Portanto S >= P!
	// Assim, caso a potência a ser fornecida ultrapasse a capacidade do inversor, a bateria só vai conseguir fornecer a máxima potência
	// que no caso é igual ao módulo de S

	for (int nv = 0; nv < peakShaving.duracaoDescarga_hr; nv++)
	{
        // É como se o inversor saturasse o tanto de potência que ele pode fornecer
		if (peakShaving.vet_pot_kw[nv] > pbateria->Cap_inv_kva)
			peakShaving.vet_pot_kw[nv] = pbateria->Cap_inv_kva;
	}

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtBateria::VerificaPorcentagem(double valor)
{
	// verifica se o valor é zero (aceitável)
	if (IsDoubleZero(valor))
	{
		return true;
	}
	// verifica se o valor é 1 (aceitável)
	if (IsDoubleZero(valor - 1))
	{
		return true;
	}
	// verifica se o valor é um número entre zero e um (aceitável)
	if ((valor < 1.) && valor > 0.)
	{
		return true;
	}
	// caso não seja nenhuma das alternativas acima, o valor nao é aceitavel
	else
	{
		return false;
	}
}


// ---------------------------------------------------------------------------
// eof





void __fastcall TFormEdtBateria::ActionSelPeakShavingExecute(TObject *Sender)
{
	switch (RGroupPeakShaving->ItemIndex)
	{
	case 0: // sim
		GBoxPeakShaving->Visible = true;
		GBoxCurvaCargaDescarga->Visible = false;
		pnlCurvaPropria->Visible = false;
		TabSheetModoOper->TabVisible = true;
		TabSheetModoOper->Caption = "Modo de Operação - Peak Shaving";
		GBoxCurvaCargaDescarga->TabOrder = 1;
		GBoxPeakShaving->TabOrder = 0;
		DefineTipOperManual();
		TabSheetCurvaCargaDescarga->TabVisible = false;
		TabSheetGraficoCargaDescarga->TabVisible = false;
		TabSheetCurvaEst->TabVisible = false;
		// Dica do FKM - Ajusta o tamanho do form \\
		this->AutoSize = true;
		this->AutoSize = false;
		break;
	case 1: // não
		GBoxPeakShaving->Visible = false;
		GBoxCurvaCargaDescarga->Visible = true;
		GBoxCurvaCargaDescarga->TabOrder = 0;
		TabSheetModoOper->TabVisible = true;
		TabSheetModoOper->Caption = "Modo de Operação - Curva Horária";
		GBoxPeakShaving->TabOrder = 1;
		pnlCurvaPropria->Visible = true;
		DefineTipOperManual();
		TabSheetCurvaCargaDescarga->TabVisible = true;
		TabSheetGraficoCargaDescarga->TabVisible = true;
		TabSheetCurvaEst->TabVisible = true;
		// Dica do FKM - Ajusta o tamanho do form \\
		this->AutoSize = true;
		this->AutoSize = false;
		break;

	default:
		Aviso("Selecione se deseja realizar o Peak Shaving!");
		break;
	}

}
//---------------------------------------------------------------------------

