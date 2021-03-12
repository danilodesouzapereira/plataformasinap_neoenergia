// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaCurva.h"
#include <DLL_Inc\Curvas.h>
#include <Dll_Inc\Funcao.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Constante\Const.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCanal.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTMedidor.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TImportaCurva::TImportaCurva(VTApl *apl, VTCurva *curva)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	if (curva)
		this->curva = curva;
	// inicializa variaveis
	medI = medPfase = medPtrif = 0;
	por_fase = false;
	tipo_canal = canalINDEF;
	try
	{ // cria objeto TStringList
		campos = new TStringList();
		lines = new TStringList();
		ActionCancela = new TAction(NULL);
		lisCANAL = new TList();
		ActionCancela->OnExecute = ActionCancelaExecute;
	}
	catch (Exception &e)
	{
		// nada
	}
}

// ---------------------------------------------------------------------------
__fastcall TImportaCurva::~TImportaCurva(void)
{
	// destrói objetos criados
	if (ActionCancela)
	{
		delete ActionCancela;
		ActionCancela = NULL;
	}
	if (lines)
	{
		delete lines;
		lines = NULL;
	}
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	if (lisCANAL)
	{
		delete lisCANAL;
		lisCANAL = NULL;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TImportaCurva::ActionCancelaExecute(TObject *Sender)
{
	// aborta a leitura
	// cancela = true;
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TImportaCurva::AtualizaMedidor(VTPatamar *patamar, double valor[MAX_FASE], VTChave *chave)
 {
 //variaveis locais
 VTCanal *canal;
 VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

 for(int ind = indFASE_A; ind <= indFASE_C; ind++)
 {//verifica se há valores em cada fase
 if(! IsDoubleZero(valor[ind]))
 {//verifica se existe canal
 canal = medidor->ExisteCanal(chave, tipo_canal, fases->IndexToTag[ind]);
 if(! canal)          canal = medidor->InsereCanal(chave, tipo_canal, fases->IndexToTag[ind]);
 if(! canal->Enabled) canal->Enabled = true;
 canal->Codigo = fases->AsStrABCN[fases->IndexToTag[ind]];
 canal->Curva->Escala        = pat01HORA;
 canal->Curva->NumeroValores = 2;
 canal->Curva->Reconfigura(pat01HORA);
 switch(canal->TipoCanal)
 {
 case canalV:
 case canalI: canal->ValorMedido[patamar] = valor[ind];         break;
 case canalP:
 case canalQ: //converte unidade
 case canalS: canal->ValorMedido[patamar] = valor[ind] / 1000.; break;
 default: break;
 }
 }
 }
 if(! IsDoubleZero(valor[indNEUTRO]))
 {//verifica se existe canal trifásico
 canal = medidor->ExisteCanal(chave, tipo_canal, faseABC);
 if(! canal) canal = medidor->InsereCanal(chave, tipo_canal, faseABC);
 if(! canal->Enabled) canal->Enabled = true;
 canal->Codigo = fases->AsStrABCN[fases->IndexToTag[indNEUTRO]];
 canal->Curva->Escala        = pat01HORA;
 canal->Curva->NumeroValores = 2;
 canal->Curva->Reconfigura(pat01HORA);
 switch(canal->TipoCanal)
 {
 case canalV:
 case canalI: canal->ValorMedido[patamar] = valor[indNEUTRO];         break;
 case canalP:
 case canalQ: //converte unidade
 case canalS: canal->ValorMedido[patamar] = valor[indNEUTRO] / 1000.; break;
 default: break;
 }
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TImportaCurva::ConfiguraCanal(void)
{
	// variaveis locais
	VTCanal *canal;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// proteção
	if (!eqpto)
		return;
	// determina canais do medidor associados ao eqpto
	lisCANAL->Clear();
	medidor->LisCanal(lisCANAL, eqpto);
	for (int nc = 0; nc < lisCANAL->Count; nc++)
	{
		canal = (VTCanal*)lisCANAL->Items[nc];
		// configura curva do canal
		if (canal->Curva)
		{
			canal->Curva->NumeroValores = nvCURVA_CANAL; // 2;
			canal->Curva->Reconfigura(patamares->Escala);
		}
		// verifica o código do canal
		if (canal->Codigo.IsEmpty())
		{
			canal->Codigo = canal->TipoCanalAsString + fases->AsStrABCN[canal->Fases];
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TImportaCurva::ConverteUnidade(double valor[MAX_FASE])
{
	// verifica o tipo do canal
	if (((tipo_canal == canalP) || (tipo_canal == canalQ)) || (tipo_canal == canalS))
	{
		valor[indFASE_A] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
		valor[indFASE_B] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
		valor[indFASE_C] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
		valor[indNEUTRO] /= 1000.; // kW => MW  ou kvar => Mvar ou kVA => MVA
	}
}

// -----------------------------------------------------------------------------
void __fastcall TImportaCurva::DesativaCanais(TList *lisCanal)
{
	// variaveis locais
	VTCanal *canal;

	for (int nc = 0; nc < lisCanal->Count; nc++)
	{ // desativa todos canais da lista
		canal = (VTCanal*)lisCanal->Items[nc];
		canal->Enabled = false;
	}
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TImportaCurva::Executa(AnsiString arq_txt, VTMedidor *medidor)
 {
 //variáveis locais
 bool resp   = true;

 //obtém objetos
 plog      = (VTLog*)apl->GetObject(__classid(VTLog));
 progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
 //lê arquivo
 lines->LoadFromFile(arq_txt);
 //confira progresso, habilitando a opção de cancelar
 //if (progresso) progresso->HabilitaUsuarioCancelar(ActionCancela);
 //exibe janela de progresso
 if (progresso) progresso->Start(progTEXTO);
 if (progresso) progresso->Add(arq_txt);
 //atualiza linha/Bloco no Resumo
 if (plog) plog->DefineValor("Linha", "");
 if (plog) plog->DefineValor("Bloco", "");
 for (int nl = 0; (resp)&&(! cancela)&&(nl < lines->Count); nl++)
 {//atualiza janela de progresso
 //atualiza número da linha em plog
 if (plog) plog->DefineValor("Linha", IntToStr(nl + 1));
 //verifica se há medidor e se há linha de comentário
 if(medidor && VerificaComentario(lines->Strings[nl])) continue;
 //extrai campos da linha de texto
 ExtraiStrings(lines->Strings[nl], ":;", campos);
 if (campos->Count == 0) continue;
 if(medidor) resp = InsereCurvaMedidor(campos, medidor);
 else        resp = InsereCurva(campos);
 }
 //verifica se a importação foi cancelada
 if (cancela) resp = false;
 //configura progresso, desabilitando a opção de cancelar
 //if (progresso) progresso->HabilitaUsuarioCancelar(NULL);
 if (progresso) progresso->Stop();
 //atualiza linha/Bloco no Resumo
 if (plog) plog->DefineValor("Linha", "");
 if (plog) plog->DefineValor("Bloco", "");

 return(resp);
 }
 */  // DVK 2015.12.18 reescrito considerando eqpto
// -----------------------------------------------------------------------------
bool __fastcall TImportaCurva::Executa(AnsiString arq_txt, VTEqpto *eqpto, VTMedidor *medidor)
{
	// variáveis locais
	bool resp = true;
	AnsiString txt;

	// salva ponteiros
	this->eqpto = eqpto;
	this->medidor = medidor;
	// inicializa variaveis
	medI = medPfase = medPtrif = 0;
	// configura canais
	if (medidor)
		ConfiguraCanal();
	// obtém objetos
	progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	// lê arquivo
	lines->LoadFromFile(arq_txt);
	// confira progresso, habilitando a opção de cancelar
	// if (progresso) progresso->HabilitaUsuarioCancelar(ActionCancela);
	// exibe janela de progresso
	if (progresso)
		progresso->Start(progTEXTO);
	if (progresso)
		progresso->Add(arq_txt);
	for (int nl = 0; (resp) && (nl < lines->Count); nl++)
	{ // verifica se há canal e se há linha de comentário
		if (medidor && VerificaComentario(lines->Strings[nl]))
			continue;
		// extrai campos da linha de texto
		ExtraiStrings(lines->Strings[nl], ":;", campos);
		if (campos->Count == 0)
			continue;
		if (medidor)
			resp = InsereCurvaMedidor(campos);
		else
			resp = InsereCurva(campos);
	}
	// marca/desmarca canais, conforme prioridade: P fase > I fase > P trif
	if (medidor)
		PriorizaCanais();
	// configura progresso, desabilitando a opção de cancelar
	// if (progresso) progresso->HabilitaUsuarioCancelar(NULL);
	if (progresso)
		progresso->Stop();
	// verifica se há medições do mesmo tipo no mesmo arquivo
	if (medI > 1)
		txt += " Corrente\n";
	if (medPfase > 1)
		txt += " Potência por fase\n";
	if (medPtrif > 1)
		txt += " Potência trifásica";
	if (!txt.IsEmpty())
	{
		Aviso("O arquivo com medições possui mais de uma medição do mesmo tipo:\n" + txt);
	}
	return (resp);
}

/*
 //---------------------------------------------------------------------------
 VTChave* __fastcall TImportaCurva::ExisteChave(AnsiString cod_chave)
 {
 //variaveis locais
 VTChave    *chave;
 VTEqpto    *eqpto;
 TList        *lisEQP;

 //verifica se há um medidor
 if(! medidor) return(NULL);
 //cria lista
 lisEQP = new TList();
 medidor->LisEqptoDosCanais(lisEQP);

 for(int ne = 0; ne < lisEQP->Count; ne++)
 {//procura chave na lista de eqptos do medidor
 eqpto = (VTEqpto*)lisEQP->Items[ne];
 if(eqpto->Tipo() != eqptoCHAVE) continue;
 if(eqpto->Codigo.AnsiCompareIC(cod_chave) != 0) continue;
 chave = (VTChave*)eqpto;
 break;
 }
 //destroi lista
 delete lisEQP;
 return(chave);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TImportaCurva::InsereCurva(TStrings *campos)
{
	// variáveis locais
	bool importou;
	// verifica se os valores a serem inseridos pertencem a uma curva PQ ou PQVT
	switch (campos->Count)
	{
	case 6: // número de valores para uma curva PQ
		if (InsereCurvaPQ(campos))
			importou = true;
		break;
	case 8: // número de valores para uma curva PQVT
		InsereCurvaPQVT(campos);
		importou = true;
		break;
	default:
		importou = false; ;
	}
	if (importou)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaCurva::InsereCurvaMedidor(TStrings *campos)
{
	// variáveis locais
	VTCanal *canal;
	VTPatamar *patamar;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	AnsiString cod_etd, cod_pri, cod_chave;
	int hora, minuto;
	double valor[MAX_FASE];
	double valor_tri;

	// proteçao
	if (campos->Count < 9)
		return (true); // return(false);
	try
	{ // define pontos da Curva
		// determina código da etd, primário e chave
		cod_etd = campos->Strings[0].Trim();
		cod_pri = campos->Strings[1].Trim();
		cod_chave = campos->Strings[2].Trim();
		// determina patamar
		hora = StrToInt(campos->Strings[3]);
		minuto = StrToInt(campos->Strings[4]);
		patamar = patamares->ExistePatamar(hora, minuto);
		// determina valores
		valor[indFASE_A] = StrToDouble(campos->Strings[5].Trim());
		valor[indFASE_B] = StrToDouble(campos->Strings[6].Trim());
		valor[indFASE_C] = StrToDouble(campos->Strings[7].Trim());
		valor[indNEUTRO] = StrToDouble(campos->Strings[8].Trim()); // faseABC
		ConverteUnidade(valor);
		for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
			if (IsDoubleZero(valor[ind_fase]))
				continue;
			// determina canal por fase
			canal = medidor->ExisteCanal(eqpto, tipo_canal, fases->IndexToTag[ind_fase]);
			// indica que o canal está ativo e insere valor
			if (!canal->Enabled)
				canal->Enabled = true;
			canal->ValorMedido[patamar] = valor[ind_fase];
		}
		if (!IsDoubleZero(valor[indNEUTRO]))
		{ // determina canal trifásico
			canal = medidor->ExisteCanal(eqpto, tipo_canal, faseABC);
			// indica que o canal está ativo e insere valor
			if (!canal->Enabled)
				canal->Enabled = true;
			canal->ValorMedido[patamar] = valor[indNEUTRO];
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaCurva::InsereCurvaPQ(TStrings *campos)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PQ];
	int nc = -1;

	// proteçao
	if (campos->Count < 6)
		return (false);

	try
	{ // define pontos da Curva
		// filtra linha de comentário //DVK 2015.10.30
		if ((campos->Strings[0][1] == '/') && (campos->Strings[0][2] == '/'))
			return (true);
		// verifica se tem medidor
		hm_ini.hora = StrToInt(campos->Strings[++nc]);
		hm_ini.minuto = StrToInt(campos->Strings[++nc]);
		hm_fim.hora = StrToInt(campos->Strings[++nc]);
		hm_fim.minuto = StrToInt(campos->Strings[++nc]);
		for (int ind_val = 0; ind_val < nvCURVA_PQ; ind_val++)
		{
			valor[ind_val] = StrToDouble(campos->Strings[++nc]);
		}
		if (!curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQ))
			return (false);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TImportaCurva::InsereCurvaPQVT(TStrings *campos)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_PQVT];
	int nc = -1;

	// proteçao
	if (campos->Count < 8)
		return (false);
	try
	{ // define pontos da Curva
		// filtra linha de comentário //DVK 2015.10.30
		if ((campos->Strings[0][1] == '/') && (campos->Strings[0][2] == '/'))
			return (true);
		// verifica se tem medidor
		hm_ini.hora = StrToInt(campos->Strings[++nc]);
		hm_ini.minuto = StrToInt(campos->Strings[++nc]);
		hm_fim.hora = StrToInt(campos->Strings[++nc]);
		hm_fim.minuto = StrToInt(campos->Strings[++nc]);
		for (int ind_val = 0; ind_val < nvCURVA_PQVT; ind_val++)
		{
			valor[ind_val] = StrToDouble(campos->Strings[++nc]);
		}
		if (!curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQVT))
			return (false);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TImportaCurva::InsereValorFase(double valor[MAX_FASE], VTChave *chave,
 VTMedidor *medidor, VTPatamar *patamar)
 {
 //variaveis locais
 VTCanal     *canal;
 VTFases     *fases      = (VTFases*)apl->GetObject(__classid(VTFases));
 VTPatamares *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //proteção
 if(! chave)   return(false);
 if(! medidor) return(false);
 if(! patamar) return(false);
 //converte unidade
 ConverteUnidade(valor);
 for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
 {//verifica se existe canal da grandeza com cada uma das fases
 canal = medidor->ExisteCanal(chave, tipo_canal, fases->IndexToTag[ind_fase]);
 if(! canal) continue;
 if(! canal->Enabled) canal->Enabled = true;
 //verifica se existe um código para o canal
 if(canal->Codigo.IsEmpty()) canal->Codigo = canal->TipoCanalAsString + fases->AsStrABCN[canal->Fases];
 //verifica se a escala da curva é a mesma
 if(patamares->Escala != canal->Curva->Escala) canal->Curva->Reconfigura(patamares->Escala);
 canal->ValorMedido[patamar] = valor[ind_fase];
 }
 return(true);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TImportaCurva::InsereValorTri(double valor_tri, VTChave *chave,
 VTMedidor *medidor, VTPatamar *patamar)
 {
 //variaveis locais
 VTCanal     *canal;
 VTFases     *fases      = (VTFases*)apl->GetObject(__classid(VTFases));
 VTPatamares *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //proteção
 if(! chave)   return(false);
 if(! medidor) return(false);
 if(! patamar) return(false);
 //verifica o tipo do canal para converter unidade
 if(((tipo_canal == canalP) || (tipo_canal == canalQ)) || (tipo_canal == canalS))
 {
 valor_tri /= 1000.; //kW => MW ou kvar => Mvar
 }
 //verifica se existe canal da grandeza trifásica
 canal = medidor->ExisteCanal(chave, tipo_canal, faseABC);
 if(! canal) return(false);
 if(! canal->Enabled) canal->Enabled = true;
 //verifica se existe um código para o canal
 if(canal->Codigo.IsEmpty()) canal->Codigo = canal->TipoCanalAsString + fases->AsStrABCN[canal->Fases];
 //verifica se a escala da curva é a mesma
 if(patamares->Escala != canal->Curva->Escala) canal->Curva->Reconfigura(patamares->Escala);
 canal->ValorMedido[patamar] = valor_tri;
 return(true);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TImportaCurva::PriorizaCanais(void)
{
	// variaveis locais
	VTCanal *canal;
	TList *lisI, *lisPfase, *lisPtri;

	// cria listas
	lisI = new TList();
	lisPfase = new TList();
	lisPtri = new TList();
	for (int nc = 0; nc < lisCANAL->Count; nc++)
	{ // separa todos os canais ativos do eqpto
		canal = (VTCanal*)lisCANAL->Items[nc];
		if (!canal->Enabled)
			continue;
		switch (canal->TipoCanal)
		{ // insere na lista de cada tipo
		case canalI:
			lisI->Add(canal);
			break;
		case canalP:
		case canalQ:
			if (canal->Fases == faseABC)
				lisPtri->Add(canal);
			else
				lisPfase->Add(canal);
			break;
		default:
			break;
		}
	}
	// prioriza
	if ((lisPfase->Count > 0) && (lisI->Count > 0))
	{
		DesativaCanais(lisI);
		DesativaCanais(lisPtri);
	}
	if ((lisI->Count > 0) && (lisPtri->Count > 0))
		DesativaCanais(lisPtri);
	if ((lisPfase->Count > 0) && (lisPtri->Count > 0))
		DesativaCanais(lisPtri);
	// destroi listas
	delete lisI;
	delete lisPfase;
	delete lisPtri;
}

// ---------------------------------------------------------------------------
int __fastcall TImportaCurva::TraduzTipoMedicao(AnsiString tipo_med)
{
	// verifica o tipo de medição
	if (tipo_med.AnsiCompareIC("I") == 0)
		return (canalI);
	if (tipo_med.AnsiCompareIC("S") == 0)
		return (canalS);
	if (tipo_med.AnsiCompareIC("P3f") == 0)
	{
		por_fase = false;
		return (canalP);
	}
	if (tipo_med.AnsiCompareIC("Pabc") == 0)
	{
		por_fase = true;
		return (canalP);
	}
	if (tipo_med.AnsiCompareIC("Q3f") == 0)
	{
		por_fase = false;
		return (canalQ);
	}
	if (tipo_med.AnsiCompareIC("Qabc") == 0)
	{
		por_fase = true;
		return (canalQ);
	}
	if (tipo_med.AnsiCompareIC("VF") == 0)
	{
		por_fase = true;
		return (canalV);
	}
	if (tipo_med.AnsiCompareIC("VL") == 0)
	{
		por_fase = false;
		return (canalV);
	}
	return (canalINDEF);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaCurva::VerificaComentario(AnsiString linha)
{
	// limpa campos
	campos->Clear();
	if (linha.IsEmpty())
		return (true);
	if (linha.Length() < 2)
		return (false);
	if ((linha[1] == '/') && (linha[2] == '/'))
	{ // é linha de comentário e pode conter o tipo da medição
		if (ExtraiStrings(linha, ":;", campos) < 2)
			return (true);
		// guarda tipo de medição
		tipo_canal = TraduzTipoMedicao(campos->Strings[1].Trim());
		// conta quantas medições do mesmo tipo existem no arquivo
		switch (tipo_canal)
		{
		case canalI:
			medI++;
			break;
		case canalP:
			if (por_fase)
				medPfase++;
			else
				medPtrif++;
			break;
		default:
			break;
		}
		return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
