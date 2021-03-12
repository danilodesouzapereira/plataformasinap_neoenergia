// ---------------------------------------------------------------------------
#pragma hdrstop
#include <System.JSON.hpp>

#include "TValidaArqCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Constante\Const.h"
#include "..\Rede\VTEqpto.h"

#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Curvas.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TValidaArqCurva::TValidaArqCurva(VTApl *apl) {
	// salva ponteiro
	this->apl = apl;
	// cria objetos
	linhas = new TStringList();
	lisCURVA = new TList();
	lisORD = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TValidaArqCurva::~TValidaArqCurva(void) {
	// destrói objetos
	if (linhas) {
		delete linhas;
		linhas = NULL;
	}
	if (lisORD) {
		delete lisORD;
		lisORD = NULL;
	}
	if (lisCURVA) {
		LimpaTList(lisCURVA);
		delete lisCURVA;
		lisCURVA = NULL;
	}
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TValidaArqCurva::CriaCurvaParcial(AnsiString linha_ori) {
	// variáveis locais
	VTCurva *curva;
	VTClasses *classes;
	int tipoCurva;
	TJSONObject *JSON;
	AnsiString Lim_superior = "";
	AnsiString linha;

	linha = TrataLinha(linha_ori);

	try {
		JSON = (TJSONObject*)TJSONObject::ParseJSONValue((UnicodeString)linha);
		tipoCurva = StrToInt(JSON->Get("TIPO_CURVA")->JsonValue->Value());
	}
	catch (Exception &e) {
		// lança uma exceção
		msg = ("Erro ao processar o arquivo! - Verifique a linha '" +
			linha + "'");
		// throw std::runtime_error(msg.c_str());
		return NULL;
	}

	// cria uma nova Curva
	curva = DLL_NewObjCurva();

	// propriedades comuns
	curva->Id = FalsoId();
	curva->Escala = pat01HORA;
	curva->Tipica = true;
	curva->Status[sttNOVO] = true;
	curva->Eqpto = NULL;
	curva->NumeroValores = StrToInt(JSON->Get("PONTOS")->JsonValue->Value());

	if (tipoCurva == curvaPFP_MDP) {
		curva->Tipo = curvaPFP_MDP;
		curva->Unidade = unidMVA;
		curva->NumeroValores = nvCURVA_PFP_MDP;
		curva->DiaMedicao = StrToInt(JSON->Get("MEDICAO")->JsonValue->Value());
		curva->LimiteInferior =
			StrToDouble(JSON->Get("FAIXA_CONSUMO_INFERIOR")
			->JsonValue->Value());
		Lim_superior =
			(JSON->Get("FAIXA_CONSUMO_SUPERIOR")->JsonValue->Value());
		curva->LimiteSuperior = StrToDouble(Lim_superior);
		if (Lim_superior.AnsiCompareIC("-1") == 0) {
			curva->Codigo = (JSON->Get("NOME")->JsonValue->Value()) +
				" [Acima de " +
				(JSON->Get("FAIXA_CONSUMO_INFERIOR")->JsonValue->Value()) + "]";
		}
		else {
			curva->Codigo = (JSON->Get("NOME")->JsonValue->Value()) + " [" +
				(JSON->Get("FAIXA_CONSUMO_INFERIOR")->JsonValue->Value()) +
				" - " + (JSON->Get("FAIXA_CONSUMO_SUPERIOR")->JsonValue->Value
				()) + "]";
		}
		// classe
		classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
		int tagClasse = StrToInt(JSON->Get("CLASSE")->JsonValue->Value());
		curva->Classe = classes->ExisteClasseTag(tagClasse);

	}
	else if (tipoCurva == curvaLD) {
		curva->Codigo = JSON->Get("NOME")->JsonValue->Value();
		curva->Tipo = curvaLD;
		curva->Unidade = unidON_OFF;
		curva->NumeroValores = nvCURVA_LD;
		curva->Classe = NULL;
		curva->LimiteInferior = 0.;
		curva->LimiteSuperior = 0.;
	}
	else {
		curva->Codigo = JSON->Get("NOME")->JsonValue->Value();
	}
	curva->Reconfigura(pat01HORA);

	return curva;

}

// ---------------------------------------------------------------------------
AnsiString __fastcall TValidaArqCurva::Executa(AnsiString arq_curva) {
	// variáveis locais
	AnsiString txt;
	VTCurva *curva;
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	VTClasse *classe;
	TList *lisCLASSE = classes->LisClasse();
	int dias[4] = {DOMINGO, SABADO, DIA_UTIL, SEMANA_COMPLETA};

	// inicia msg
	msg = "";
	// verifica se o arquivo é válido
	if (!FileExists(arq_curva)) {
		msg = "Arquivo inexistente";
		return msg;
	}
	// limpa stringList
	linhas->Clear();
	// carrega arquivo
	linhas->LoadFromFile(arq_curva);
	for (int n = 0; n < linhas->Count; n++) {
		txt = linhas->Strings[n];
		if (txt.IsEmpty())
			continue;
		if ((txt.UpperCase().Pos("\"TIPO_CURVA\"")) > 0)
		{ // realiza a leitura parcial
			curva = CriaCurvaParcial(txt);
			lisCURVA->Add(curva);
		}
	}
	// Organiza curvas por classe e por tipo de medição (dia da semana)
	for (int nc = 0; nc < lisCLASSE->Count; nc++) {
		classe = (VTClasse*)lisCLASSE->Items[nc];
		if (classe->Tag == consIP)
			continue;
		if (classe->Tag == consEQUIVALENTE)
			continue;
		if (classe->Tag == consA2)
			continue;
		for (int ind_dia = 0; ind_dia < 4; ind_dia++) {
			lisORD->Clear();
			Organiza(classe, dias[ind_dia]);
			// valida faixas de consumo nessa lista organizada
			if (!ValidaFaixas())
				return msg;
		}
	}
	return msg;
}

// ---------------------------------------------------------------------------
bool __fastcall TValidaArqCurva::ExisteLimiteInferior(double valor) {
	// variáveis locais
	VTCurva *curva;

	for (int n = 0; n < lisORD->Count; n++) {
		curva = (VTCurva*)lisORD->Items[n];
		if (IsDoubleZero(curva->LimiteInferior - valor))
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TValidaArqCurva::ExisteLimiteSuperior(double valor) {
	// variáveis locais
	VTCurva *curva;

	for (int n = 0; n < lisORD->Count; n++) {
		curva = (VTCurva*)lisORD->Items[n];
		if (IsDoubleZero(curva->LimiteSuperior - valor))
			return true;
	}
	return false;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TValidaArqCurva::Organiza(VTClasse *classe, int dia_medicao) {
	// variáveis locais
	VTCurva *curva;

	for (int n = 0; n < lisCURVA->Count; n++)
	{ // insere na lisORD somente curvas da mesma classe e dia de medição
		curva = (VTCurva*)lisCURVA->Items[n];
		if (curva->Classe != classe)
			continue;
		if (curva->DiaMedicao != dia_medicao)
			continue;
		lisORD->Add(curva);
	}
	if (lisORD->Count == 0)
		msg = "Não existem curvas da classe " + classe->Codigo +
			" e dia da medição " + IntToStr(dia_medicao);
}

/*
 // -------------------------------------------------------------------------------------------------
 AnsiString __fastcall TValidaArqCurva::TrataLinha(AnsiString linha_or)
 {
 //
 int index;
 AnsiString sub1, sub2;
 index = linha_or.AnsiPos("\"\"");
 AnsiString linha, linha_n;
 //
 //	if (index > 0)
 //	{
 //		sub1 = linha_or.SubString(1, index-1);
 //		sub2 = linha_or.SubString(index + 1, linha_or.Length());
 //		linha_n = sub1 + sub2;
 //		linha = linha_n;
 //		TrataLinha(linha);
 //	}
 //	else
 //	{
 //		linha = linha_or;
 //	}
 //	return linha;
 }
 */

// -------------------------------------------------------------------------------------------------
AnsiString __fastcall TValidaArqCurva::TrataLinha(AnsiString linha_or) {
	// Variáveis locais
	
	if ((linha_or[linha_or.Length()] == ';') && (linha_or[linha_or.Length() - 1] == ';') && (linha_or[linha_or.Length() - 2] == ';'))
	{
		linha_or.Delete((linha_or.Length()-2),3);
	}
	if ((linha_or[linha_or.Length()] == '\"') && (linha_or[linha_or.Length() - 1] == '}'))
	{
			linha_or.Delete(linha_or.Length(),1);
	}
		if ((linha_or[1] == '\"') && (linha_or[2] == '{'))
	{
			linha_or.Delete(1,1);
	}
	for (int n = linha_or.Length(); n >= 2; n--)
	{
		if ((linha_or[n] == '\"') && (linha_or[n - 1] == '\"'))
		{
			linha_or.Delete(n,1);
		}
	}
    return linha_or;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TValidaArqCurva::ValidaFaixas(void) {
	// variáveis locais
	VTCurva *curva;

	// verifica se há curvas
	if (!msg.IsEmpty())
		return false;
	// verifica se há somente 1 curva (sem faixa)
	if (lisORD->Count == 1) {
		curva = (VTCurva*)lisORD->First();
		return (ValidaFaixaUnica(curva));
	}
	// verifica limites
	for (int n = 0; n < lisORD->Count; n++) {
		curva = (VTCurva*)lisORD->Items[n];
		// inferior
		if (curva->LimiteInferior <= 0.)
			continue;
		if (!ExisteLimiteSuperior(curva->LimiteInferior)) {
			msg = "Limite inferior inválido ou arquivo incompleto para curva " +
				curva->Codigo + " dia da medição " +
				IntToStr(curva->DiaMedicao);
			return false;
		}
		// superior
		if (curva->LimiteSuperior < 0.)
			continue;
		if (!ExisteLimiteInferior(curva->LimiteSuperior)) {
			msg = "Limite superior inválido ou arquivo incompleto para curva " +
				curva->Codigo + " dia da medição " +
				IntToStr(curva->DiaMedicao);
			return false;
		}
	}
	return true;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TValidaArqCurva::ValidaFaixaUnica(VTCurva *curva) {
	if (curva->LimiteInferior > 0.) {
		msg = "Limite inferior inválido ou arquivo incompleto para curva " +
			curva->Codigo + " dia da medição " + IntToStr(curva->DiaMedicao);
		return false;
	}
	if (curva->LimiteSuperior > 0.) {
		msg = "Limite superior inválido ou arquivo incompleto para curva " +
			curva->Codigo + " dia da medição " + IntToStr(curva->DiaMedicao);
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
