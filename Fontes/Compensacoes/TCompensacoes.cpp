//---------------------------------------------------------------------------
#pragma hdrstop
#include "TCompensacoes.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <DLL_Inc\Funcao.h>
#include <Fontes\Diretorio\VTPath.h>
#include "TConjuntoAneel.h"
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTCarga.h>
//---------------------------------------------------------------------------
VTCompensacoes* __fastcall NewObjCompensacoes(TComponent* Owner, VTApl* AplOwner, AnsiString empresa)
	{
	try{
		return(new TCompensacoes(Owner, AplOwner, empresa));
		}
	catch(Exception &e)
		{
		}
	return(NULL);
	}
//---------------------------------------------------------------------------
__fastcall strLimite::strLimite(void)
	{
	DIC = new strValores;
	FIC = new strValores;

	DIC->Anual = DIC->Trimestral = DIC->Mensal = 0.;
	FIC->Anual = FIC->Trimestral = FIC->Mensal = 0.;
	DMIC = 0.;
	}
//---------------------------------------------------------------------------
__fastcall strLimite::~strLimite(void)
	{
	delete DIC;
	delete FIC;
	}
//---------------------------------------------------------------------------
__fastcall strLimites::strLimites(void)
	{
	BT = new strLimite;
	MT = new strLimite;
	}
//---------------------------------------------------------------------------
__fastcall strLimites::~strLimites(void)
	{
	delete BT;
	delete MT;
	}
//---------------------------------------------------------------------------
__fastcall strFaixa::strFaixa(void)
	{
	Urbano    = new strLimites;
	NaoUrbano = new strLimites;
	}
//---------------------------------------------------------------------------
__fastcall strFaixa::~strFaixa(void)
	{
	delete Urbano;
	delete NaoUrbano;
	}
//---------------------------------------------------------------------------
__fastcall TCompensacoes::TCompensacoes(TComponent* Owner, VTApl* AplOwner, AnsiString empresa)
	{
	VTPath* Path = (VTPath*)AplOwner->GetObject(__classid(VTPath));
	this->Owner    = Owner;
	this->AplOwner = AplOwner;

	LisConjuntos = NULL;
	LisFaixas = NULL;

	TUSD.B1 = TUSD.B2 = TUSD.B3 = TUSD.B4 = TUSD.A4 = 0.;

	for(int i = 0; i < 12; i++)
		CompensacoesMensais[i] = DICnv[i] = FICnv[i] = 0.;

	for(int i = 0; i < 4; i++)
		CompensacoesTrimestrais[i] = 0.;
	CompensacaoAnual = 0.;
	CompensacaoTotal = 0.;
	//empresa = "Coelba";
	//if(!Inicializa(Path->DirDat() + "\\" + empresa + "\\"))

	if(!Inicializa(empresa))
	{
		delete this;
	}
}
//---------------------------------------------------------------------------
__fastcall TCompensacoes::~TCompensacoes(void)
	{
	if(LisConjuntos)
		{
		LimpaTList(LisConjuntos);
		delete LisConjuntos;
		}
	if(LisFaixas)
		{
		LimpaTList(LisFaixas);
		delete LisFaixas;
		}
	}
//---------------------------------------------------------------------------
bool __fastcall TCompensacoes::BuscaLimites(VTSubestacao* SE, VTCarga* Carga, double *DICp, double *FICp, double *DMICp, eTipoPeriodo Periodo)
{
	// Proteções
	if(!SE){return(false);}
	TConjuntoAneel* Conjunto = RetornaConjuntoAneel(SE);
	if(Conjunto == NULL){return(false);}
	strFaixa* FaixaDEC = RetornaFaixaDEC(Conjunto);
	strFaixa* FaixaFEC = RetornaFaixaFEC(Conjunto);
	if((FaixaDEC == NULL)||(FaixaFEC == NULL)){return(false);}

	// Limites rurais MT
	if((Carga->NumConsRural != 0)&&(Carga->NumConsA4 != 0))
	{
		switch(Periodo)
		{
			case etpMensal:
				*DICp  = FaixaDEC->NaoUrbano->MT->DIC->Mensal;
				*FICp  = FaixaFEC->NaoUrbano->MT->FIC->Mensal;
				*DMICp = FaixaDEC->NaoUrbano->MT->DMIC;
				break;
			case etpTrimestral:
				*DICp  = FaixaDEC->NaoUrbano->MT->DIC->Trimestral;
				*FICp  = FaixaFEC->NaoUrbano->MT->FIC->Trimestral;
				break;
			case etpAnual:
				*DICp  = FaixaDEC->NaoUrbano->MT->DIC->Anual;
				*FICp  = FaixaFEC->NaoUrbano->MT->FIC->Anual;
				break;
		}
	}

	// Limites rurais BT
	if((Carga->NumConsRural != 0)&&(Carga->NumConsA4 == 0))
	{
		switch(Periodo)
		{
			case etpMensal:
				*DICp  = FaixaDEC->NaoUrbano->BT->DIC->Mensal;
				*FICp  = FaixaFEC->NaoUrbano->BT->FIC->Mensal;
				*DMICp = FaixaDEC->NaoUrbano->BT->DMIC;
				break;
			case etpTrimestral:
				*DICp  = FaixaDEC->NaoUrbano->BT->DIC->Trimestral;
				*FICp  = FaixaFEC->NaoUrbano->BT->FIC->Trimestral;
				break;
			case etpAnual:
				*DICp  = FaixaDEC->NaoUrbano->BT->DIC->Anual;
				*FICp  = FaixaFEC->NaoUrbano->BT->FIC->Anual;
				break;
		}

	}

	// Limites urbanos MT
	if((Carga->NumConsRural == 0)&&(Carga->NumConsA4 != 0))
	{
		switch(Periodo)
		{
			case etpMensal:
				*DICp  = FaixaDEC->Urbano->MT->DIC->Mensal;
				*FICp  = FaixaFEC->Urbano->MT->FIC->Mensal;
				*DMICp = FaixaDEC->Urbano->MT->DMIC;
				break;
			case etpTrimestral:
				*DICp  = FaixaDEC->Urbano->MT->DIC->Trimestral;
				*FICp  = FaixaFEC->Urbano->MT->FIC->Trimestral;
				break;
			case etpAnual:
				*DICp  = FaixaDEC->Urbano->MT->DIC->Anual;
				*FICp  = FaixaFEC->Urbano->MT->FIC->Anual;
				break;
		}
	}

	// Limites urbanos BT
	if((Carga->NumConsRural == 0)&&(Carga->NumConsA4 == 0))
	{
		switch(Periodo)
		{
			case etpMensal:
				*DICp  = FaixaDEC->Urbano->BT->DIC->Mensal;
				*FICp  = FaixaFEC->Urbano->BT->FIC->Mensal;
				*DMICp = FaixaDEC->Urbano->BT->DMIC;
				break;
			case etpTrimestral:
				*DICp  = FaixaDEC->Urbano->BT->DIC->Trimestral;
				*FICp  = FaixaFEC->Urbano->BT->FIC->Trimestral;
				break;
			case etpAnual:
				*DICp  = FaixaDEC->Urbano->BT->DIC->Anual;
				*FICp  = FaixaFEC->Urbano->BT->FIC->Anual;
				break;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
double __fastcall TCompensacoes::Calcula(VTSubestacao* SE, VTCarga* Carga, double *DICsMensais, double *FICsMensais)
{
	// Inicia Variáveis
	double SDICnv, SFICnv;
	for(int i = 0; i < 12; i++){CompensacoesMensais[i] = DICnv[i] = FICnv[i] = 0.;}
	for(int i = 0; i < 4; i++){CompensacoesTrimestrais[i] = 0.;}
	CompensacaoAnual = 0.;
	CompensacaoTotal = 0.;

	// Calcula compensações Mensais
	for(int i = 0; i < 12; i++)
	{
		CompensacoesMensais[i] = Calcula(SE, Carga, DICsMensais[i], FICsMensais[i], 0., etpMensal, i);
		if(CompensacoesMensais[i] <= 0.01)
			CompensacoesMensais[i] = 0.;
	}

	// Calcula compensações Trimestrais
	for(int i = 0; i < 4; i++)
	{
		// Inicializa indicadores trimestrais
		double DICTri = DICsMensais[3*i] + DICsMensais[3*i+1] + DICsMensais[3*i+2];
		double FICTri = FICsMensais[3*i] + FICsMensais[3*i+1] + FICsMensais[3*i+2];

		// Calcula compensacao
		CompensacoesTrimestrais[i] = Calcula(SE, Carga, DICTri, FICTri, 0., etpTrimestral, i);

		// Checa o caso de valores não violados de DIC e FIC mensais
		SDICnv = DICnv[3*i] + DICnv[3*i+1] + DICnv[3*i+2];
		SFICnv = FICnv[3*i] + FICnv[3*i+1] + FICnv[3*i+2];
		if((SDICnv == 0)||(SFICnv == 0))
			CompensacoesTrimestrais[i] -= (CompensacoesMensais[3*i] + CompensacoesMensais[3*i+1] + CompensacoesMensais[3*i+2]);
		if(CompensacoesTrimestrais[i] <= 0.01)
			CompensacoesTrimestrais[i] = 0.;
	 }

	// Calcula compensações Anuais
	double DICAnu = 0.;
	double FICAnu = 0.;

	// Inicializa indicadores anuais
	for(int i = 0; i < 12; i++)
	{
		DICAnu += DICsMensais[i];
		FICAnu += FICsMensais[i];
	}

	//Calcula Compensacao
	CompensacaoAnual = Calcula(SE, Carga, DICAnu, FICAnu, 0., etpAnual, 0);

	// Checa o caso de valores não violados de DIC e FIC mensais
	SDICnv = SFICnv = 0.;
	double TotalMensal = 0.;
	double TotalTrimestral = 0.;
	for(int i = 0; i < 4; i++){TotalTrimestral += CompensacoesTrimestrais[i];}
	for(int i = 0; i < 12; i++)
	{
		TotalMensal += CompensacoesMensais[i];
		SDICnv += DICnv[i];
		SFICnv += FICnv[i];
	}
	if((SDICnv == 0)||(SFICnv == 0))
	{
		CompensacaoAnual -= TotalMensal;
		CompensacaoAnual -= TotalTrimestral;
	}
	if(CompensacaoAnual <= 0.01)
		CompensacaoAnual = 0.;

	// Calcula compensacao Total
	CompensacaoTotal = TotalMensal + TotalTrimestral + CompensacaoAnual;

	return CompensacaoTotal;
}
//---------------------------------------------------------------------------
double __fastcall TCompensacoes::Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC, double DMIC, eTipoPeriodo Periodo, int index)
{
	// Proteção
	if(!Carga){return(0.);}

	// Busca limites
	double DICp, FICp, DMICp;
	DICp = FICp = DMICp = 0;
	if(!BuscaLimites(SE, Carga, &DICp, &FICp, &DMICp, Periodo)){return(0.);}
	// Calcula TUSD equivalente
	double kei = 15.;
	double TUSD = CalculaTUSD(Carga, &kei);

	// Calcula Transgressão
	double transgressao = CalculaTransgressao(DIC, FIC, DMIC, DICp, FICp, DMICp, DICnv, FICnv, index, Periodo);

	// Calcula Compensação
	double compensacao = CalculaCompensacao(Carga, TUSD,transgressao,kei,Periodo);

	return(compensacao);
}
//---------------------------------------------------------------------------
double __fastcall TCompensacoes::CalculaCompensacao(VTCarga * Carga, double TUSD, double transgressao, double kei, eTipoPeriodo Periodo)
{
	// Calcula a energia mensal
	VTPatamares	*patamares 	= (VTPatamares*)AplOwner->GetObject(__classid(VTPatamares));
	VTDemanda 	*demanda 	= (VTDemanda*)AplOwner->GetObject(__classid(VTDemanda));
	int numPat = patamares->NumPatamar();
	double EnergiaMensal = 0.0;
	for(int np = 0; np < numPat; np++){EnergiaMensal += (demanda->P_mw[Carga][np] * 1000.);}
	EnergiaMensal *= 30.;

	//Calcula a compensação
	double EUSDMedio = TUSD * (EnergiaMensal);
	double compensacao = transgressao * (EUSDMedio/730.) * kei;

	//Aplica limites
	switch(Periodo)
	{
		case etpMensal:
			if(compensacao > (10. * EUSDMedio)){compensacao = 10. * EUSDMedio;}
			break;
		case etpTrimestral:
			if(compensacao > (30. * EUSDMedio)){compensacao = 30. * EUSDMedio;}
			break;
		case etpAnual:
			if(compensacao > (120. * EUSDMedio)){compensacao = 120. * EUSDMedio;}
			break;
	}

	return compensacao;
}
//---------------------------------------------------------------------------
double __fastcall TCompensacoes::CalculaTransgressao(double DICv, double FICv, double DMICv, double DICp, double FICp, double DMICp, double *DICnv, double *FICnv,int index, eTipoPeriodo Periodo)
{
	//Inicializa variáveis
	double TransgressaoDIC, TransgressaoFIC, TransgressaoDMIC;
	double SDICnv, SFICnv;
	TransgressaoDIC = TransgressaoFIC = TransgressaoDMIC = SDICnv = SFICnv = 0.;

	//Calcula transgressões
	TransgressaoDIC 	= DICv - DICp;
	TransgressaoFIC 	= (FICv - FICp) * (DICp / FICp);
	TransgressaoDMIC  = DMICv - DMICp;

	//Clacula ou aplica considerações de DIC e FIC mensais não violados
	switch(Periodo)
	{
		case etpMensal:
			if(DICv < DICp){DICnv[index] = DICv;}
			if(FICv < FICp){FICnv[index] = FICv;}
			break;
		case etpTrimestral:
			SDICnv = DICnv[3*index] + DICnv[3*index+1] + DICnv[3*index+2];
			SFICnv = FICnv[3*index] + FICnv[3*index+1] + FICnv[3*index+2];
			break;
		case etpAnual:
			for(int i = 0; i < 12; i++)
			{
				SDICnv += DICnv[i];
				SFICnv += FICnv[i];
			}
			break;
	}
	if(SDICnv > 0){TransgressaoDIC *= SDICnv/DICv;}
	if(SFICnv > 0){TransgressaoFIC *= SFICnv/FICv;}

	//Calcula maior transgressão para cálculo da compensação
	double Transgressao = TransgressaoDIC;
	if(Transgressao < TransgressaoFIC){Transgressao = TransgressaoFIC;}
	if(Transgressao < TransgressaoDMIC){Transgressao = TransgressaoDMIC;}
	if(Transgressao < 0.){Transgressao = 0.;}

	return Transgressao;
}
//---------------------------------------------------------------------------
double __fastcall TCompensacoes::CalculaTUSD(VTCarga* Carga, double *kei)
{
	//Inicializa vairáveis
	double TUSD_BT = 0;

	//Calcula tusd equivalente para carga equivalente
	if((Carga->NumConsResidencial + Carga->NumConsRural + Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros) != 0)
	{
		TUSD_BT  = TUSD.B1 * Carga->NumConsResidencial;
		TUSD_BT += TUSD.B2 * Carga->NumConsRural;
		TUSD_BT += TUSD.B3 * (Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros);
		TUSD_BT /= Carga->NumConsResidencial + Carga->NumConsRural + Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros;
	}

	//Consideração para cargas MT
	double TUSD_Final = TUSD_BT;
	if(Carga->NumConsA4 != 0)
	{
		*kei  = 20.;
		TUSD_Final = TUSD.A4;
	}

	return TUSD_Final;
}
//---------------------------------------------------------------------------
void __fastcall TCompensacoes::CarregaConjuntosAneel(AnsiString empresa)
	{
	if(LisConjuntos)
		LimpaTList(LisConjuntos);
	else
		LisConjuntos = new TList;

	AnsiString NomeArquivo = empresa + "DEC&FEC.dat";
	TStringList* Arquivo = new TStringList;
	Arquivo->LoadFromFile(NomeArquivo);

	AnsiString Separador = ";";

	TStringList* Cabecalho = new TStringList;
	AnsiString Linha = Arquivo->Strings[0];
	ExtraiStrings(Linha, Separador, Cabecalho);

	TStringList* Valores = new TStringList;
	ConjuntoDefault = NULL;
	for(int i = 1; i < Arquivo->Count; i++)
		{
		Linha = Arquivo->Strings[i];
		Valores->Clear();
		ExtraiStrings(Linha, Separador, Valores);
		TConjuntoAneel* Conjunto;
		if(Valores->Strings[Cabecalho->IndexOf("Cod_SE")].Trim() != "")
			Conjunto = new TConjuntoAneel(Valores->Strings[Cabecalho->IndexOf("Cod_SE")]);
		else
			Conjunto = new TConjuntoAneel("");
		LisConjuntos->Add(Conjunto);
		if(Valores->Strings[Cabecalho->IndexOf("Cod_Conjunto")].Trim() != "")
			Conjunto->Codigo = StrToInt(Valores->Strings[Cabecalho->IndexOf("Cod_Conjunto")]);
		Conjunto->Nome = Valores->Strings[Cabecalho->IndexOf("Nome_Conjunto")];
		Conjunto->DEC  = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DEC")]);
		Conjunto->FEC  = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FEC")]);
		if(Conjunto->Nome.AnsiCompare("DEFAULT") == 0) ConjuntoDefault = Conjunto;
		}
	delete Valores;
	delete Cabecalho;
	delete Arquivo;
	}
//---------------------------------------------------------------------------
void __fastcall TCompensacoes::CarregaLimitesAneel(void)
	{
	if(LisFaixas)
		LimpaTList(LisFaixas);
	else
		LisFaixas = new TList;

	AnsiString NomeArqMTUrbano    = "\\Compensacoes\\MT_Urbano.dat";
	AnsiString NomeArqMTNaoUrbano = "\\Compensacoes\\MT_NaoUrbano.dat";
	AnsiString NomeArqBTUrbano    = "\\Compensacoes\\BT_Urbano.dat";
	AnsiString NomeArqBTNaoUrbano = "\\Compensacoes\\BT_NaoUrbano.dat";

	VTPath* Path = (VTPath*)AplOwner->GetObject(__classid(VTPath));

	TStringList* ArqMTUrbano = new TStringList;
	ArqMTUrbano->LoadFromFile(Path->DirDat() + NomeArqMTUrbano);
	TStringList* ArqMTNaoUrbano = new TStringList;
	ArqMTNaoUrbano->LoadFromFile(Path->DirDat() + NomeArqMTNaoUrbano);
	TStringList* ArqBTUrbano = new TStringList;
	ArqBTUrbano->LoadFromFile(Path->DirDat() + NomeArqBTUrbano);
	TStringList* ArqBTNaoUrbano = new TStringList;
	ArqBTNaoUrbano->LoadFromFile(Path->DirDat() + NomeArqBTNaoUrbano);
	AnsiString Separador = ";";

	TStringList* Cabecalho = new TStringList;
	AnsiString Linha = ArqMTUrbano->Strings[0];
	ExtraiStrings(Linha, Separador, Cabecalho);

	TStringList* Valores = new TStringList;
	for(int i = 1; i < ArqMTUrbano->Count; i++)
		{
		strFaixa* Faixa = new strFaixa;
		LisFaixas->Add(Faixa);

		Linha = ArqMTUrbano->Strings[i];
		Valores->Clear();
		ExtraiStrings(Linha, Separador, Valores);
		Faixa->Referencia                  = Valores->Strings[Cabecalho->IndexOf("Faixa_DEC&FEC")];
		Faixa->Urbano->MT->DMIC            = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DMIC")]);
		Faixa->Urbano->MT->DIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Anu")]);
		Faixa->Urbano->MT->DIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Tri")]);
		Faixa->Urbano->MT->DIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Men")]);
		Faixa->Urbano->MT->FIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Anu")]);
		Faixa->Urbano->MT->FIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Tri")]);
		Faixa->Urbano->MT->FIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Men")]);
		Linha = ArqMTNaoUrbano->Strings[i];
		Valores->Clear();
		ExtraiStrings(Linha, Separador, Valores);
		Faixa->NaoUrbano->MT->DMIC            = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DMIC")]);
		Faixa->NaoUrbano->MT->DIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Anu")]);
		Faixa->NaoUrbano->MT->DIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Tri")]);
		Faixa->NaoUrbano->MT->DIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Men")]);
		Faixa->NaoUrbano->MT->FIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Anu")]);
		Faixa->NaoUrbano->MT->FIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Tri")]);
		Faixa->NaoUrbano->MT->FIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Men")]);

		Linha = ArqBTUrbano->Strings[i];
		Valores->Clear();
		ExtraiStrings(Linha, Separador, Valores);
		Faixa->Urbano->BT->DMIC            = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DMIC")]);
		Faixa->Urbano->BT->DIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Anu")]);
		Faixa->Urbano->BT->DIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Tri")]);
		Faixa->Urbano->BT->DIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Men")]);
		Faixa->Urbano->BT->FIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Anu")]);
		Faixa->Urbano->BT->FIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Tri")]);
		Faixa->Urbano->BT->FIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Men")]);

		Linha = ArqMTNaoUrbano->Strings[i];
		Valores->Clear();
		ExtraiStrings(Linha, Separador, Valores);
		Faixa->NaoUrbano->BT->DMIC            = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DMIC")]);
		Faixa->NaoUrbano->BT->DIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Anu")]);
		Faixa->NaoUrbano->BT->DIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Tri")]);
		Faixa->NaoUrbano->BT->DIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("DIC_Men")]);
		Faixa->NaoUrbano->BT->FIC->Anual      = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Anu")]);
		Faixa->NaoUrbano->BT->FIC->Trimestral = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Tri")]);
		Faixa->NaoUrbano->BT->FIC->Mensal     = StrToDouble(Valores->Strings[Cabecalho->IndexOf("FIC_Men")]);
		}
	delete Valores;
	delete Cabecalho;
	delete ArqMTUrbano;
	delete ArqMTNaoUrbano;
	delete ArqBTUrbano;
	delete ArqBTNaoUrbano;
	}
//---------------------------------------------------------------------------
void __fastcall TCompensacoes::CarregaValoresTarifas(AnsiString empresa)
	{
	AnsiString NomeArquivo = empresa + "Tarifas.dat";
	if(!FileExists(NomeArquivo)) return;
	TStringList* Arquivo = new TStringList;
	Arquivo->LoadFromFile(NomeArquivo);

	AnsiString Separador = ";";

	TStringList* Cabecalho = new TStringList;
	AnsiString Linha = Arquivo->Strings[0];
	ExtraiStrings(Linha, Separador, Cabecalho);

	TStringList* Valores = new TStringList;
	Linha = Arquivo->Strings[1];
	ExtraiStrings(Linha, Separador, Valores);
	TUSD.B1 = StrToDouble(Valores->Strings[Cabecalho->IndexOf("B1[R$/kWh]")]);
	TUSD.B2 = StrToDouble(Valores->Strings[Cabecalho->IndexOf("B2[R$/kWh]")]);
	TUSD.B3 = StrToDouble(Valores->Strings[Cabecalho->IndexOf("B3[R$/kWh]")]);
	TUSD.B4 = StrToDouble(Valores->Strings[Cabecalho->IndexOf("B4[R$/kWh]")]);
	TUSD.A4 = StrToDouble(Valores->Strings[Cabecalho->IndexOf("A4[R$/kWh]")]);
	delete Valores;
	delete Cabecalho;
	delete Arquivo;
	}
//---------------------------------------------------------------------------
bool __fastcall TCompensacoes::Inicializa(AnsiString empresa)
{
	CarregaLimitesAneel();
	if(empresa.AnsiCompare("") == 0)
	{
		Erro("Diretório com arquivos contendo valores das tarifas e dos limites de DEC/FEC por conjunto não foi encontrado!");
		return(false);
	}
	else
	{

		CarregaConjuntosAneel(empresa);

		CarregaValoresTarifas(empresa);
	}
	return(true);
}
//---------------------------------------------------------------------------
void __fastcall TCompensacoes::Parametros(VTSubestacao* SE, VTCarga* Carga, strParamComp &param)
	{
	if(!SE)
		return;

	if(!Carga)
		return;

	TConjuntoAneel* Conjunto = RetornaConjuntoAneel(SE);
	strFaixa*       FaixaDEC = RetornaFaixaDEC(Conjunto);
	strFaixa*       FaixaFEC = RetornaFaixaFEC(Conjunto);

	if((FaixaDEC == NULL)||(FaixaFEC == NULL)) return;

	double TUSD_BT = 0;
	if((Carga->NumConsResidencial + Carga->NumConsRural + Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros) != 0)
		{
		TUSD_BT = TUSD.B1 * Carga->NumConsResidencial;
		TUSD_BT += TUSD.B2 * Carga->NumConsRural;
		TUSD_BT += TUSD.B3 * (Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros);
		TUSD_BT /= Carga->NumConsResidencial + Carga->NumConsRural + Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros;
		}
	double kei  = 15.;
	double TUSD_Final = TUSD_BT;
	if(Carga->NumConsA4 != 0)
		{
		kei  = 20.;
		TUSD_Final = TUSD.A4;
		}

	VTPatamares	 *patamares = (VTPatamares*)AplOwner->GetObject(__classid(VTPatamares));
	VTDemanda *demanda = (VTDemanda*)AplOwner->GetObject(__classid(VTDemanda));
	int  numPat = patamares->NumPatamar();
	double energia = 0.0;

//	if(Carga->NumConsA4 != 0)
//	  energia = Carga->Energia;
//	else
//	  {
	  for(int np = 0; np < numPat; np++)
		{energia += demanda->P_mw[Carga][np];
		}
	  //energia *= 30.;
//	  }



	param.TUSD_B1 =TUSD.B1;
	param.TUSD_B2 = TUSD.B2;
	param.TUSD_B3 =TUSD.B3 ;
	param.TUSD_BT =TUSD_BT;
	param.NaoUrbanoDICAnual =FaixaDEC->NaoUrbano->MT->DIC->Anual;
	param.NaoUrbanoFICAnual =FaixaFEC->NaoUrbano->MT->FIC->Anual;
	param.UrbanoDICAnual =FaixaDEC->Urbano->MT->DIC->Anual;
	param.UrbanoFICAnual =FaixaFEC->Urbano->MT->DIC->Anual;
	param.TUSD_Final = TUSD_Final;
	param.kei = kei;
	param.energia =energia;



	/*
	TUSD.B1
	TUSD.B2
	TUSD.B3
	TUSD_BT
	FaixaDEC->NaoUrbano->MT->DIC->Anual
	FaixaFEC->NaoUrbano->MT->FIC->Anual
	TransgressaoDIC
	TransgressaoFIC
	kei
	energia
	*/
//if(Carga->NumConsRural != 0) &&
//	  if(Carga->NumConsA4 != 0)
	//TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->MT->DIC->Anual;
	//TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->MT->FIC->Anual) * FaixaDEC->NaoUrbano->MT->DIC->Anual / FaixaFEC->NaoUrbano->MT->FIC->Anual;
//	  if(Carga->NumConsA4 == 0)
	//TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->BT->DIC->Anual;
	//TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->BT->FIC->Anual) * FaixaDEC->NaoUrbano->BT->DIC->Anual / FaixaFEC->NaoUrbano->BT->FIC->Anual;

//if(Carga->NumConsRural == 0) &&
//	  if(Carga->NumConsA4 != 0)
	//TransgressaoDIC = DIC - FaixaDEC->Urbano->MT->DIC->Anual;
	//TransgressaoFIC = (FIC - FaixaFEC->Urbano->MT->FIC->Anual) * FaixaDEC->Urbano->MT->DIC->Anual / FaixaFEC->Urbano->MT->FIC->Anual;
//	  if(Carga->NumConsA4 == 0)
	//TransgressaoDIC = DIC - FaixaDEC->Urbano->BT->DIC->Anual;
	//TransgressaoFIC = (FIC - FaixaFEC->Urbano->BT->FIC->Anual) * FaixaDEC->Urbano->BT->DIC->Anual / FaixaFEC->Urbano->BT->FIC->Anual;

//	double Transgressao = TransgressaoDIC;
//	if(Transgressao < TransgressaoFIC)
//		Transgressao = TransgressaoFIC;
	//compensacao
	//return(Transgressao * TUSD_Final * 30. * energia * kei / 730);

	}
//---------------------------------------------------------------------------
TConjuntoAneel* __fastcall TCompensacoes::RetornaConjuntoAneel(VTSubestacao* SE)
	{
	if(!SE)
		return(NULL);
	if(!LisConjuntos)
		return(NULL);
	for(int i = 0; i < LisConjuntos->Count; i++)
		{
		TConjuntoAneel* Conjunto = (TConjuntoAneel*)LisConjuntos->Items[i];
		Conjunto->CodSE = Conjunto->CodSE.Trim();
		if(Conjunto->CodSE.AnsiCompareIC(SE->Rede->Codigo)==0)
			return(Conjunto);
		}
	return(ConjuntoDefault);
	//return(NULL);
	}
//---------------------------------------------------------------------------
strFaixa* __fastcall TCompensacoes::RetornaFaixaDEC(TConjuntoAneel* Conjunto)
	{
	if(!Conjunto)
		return(NULL);
	if(!LisFaixas)
		return(NULL);
	for(int i = 0; i < LisFaixas->Count; i++)
		{
		strFaixa* Faixa = (strFaixa*)LisFaixas->Items[i];
		if(Faixa->Referencia == "Maior")
			return(Faixa);
		else if(StrToDouble(Faixa->Referencia) >= Conjunto->DEC)
			return(Faixa);
		}
	return(NULL);
	}
//---------------------------------------------------------------------------
strFaixa* __fastcall TCompensacoes::RetornaFaixaFEC(TConjuntoAneel* Conjunto)
	{
	if(!Conjunto)
		return(NULL);
	if(!LisFaixas)
		return(NULL);
	for(int i = 0; i < LisFaixas->Count; i++)
		{
		strFaixa* Faixa = (strFaixa*)LisFaixas->Items[i];
		if(Faixa->Referencia == "Maior")
			return(Faixa);
		else if(StrToDouble(Faixa->Referencia) >= Conjunto->FEC)
			return(Faixa);
		}
	return(NULL);
	}
//---------------------------------------------------------------------------
//eof
//---------------------------------------------------------------------------
// Codigo Antigo
//---------------------------------------------------------------------------
//double __fastcall TCompensacoes::Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC, double DMIC, double DICRI, eTipoPeriodo Periodo)
//	{
//	if(!SE)
//		return(0.);
//
//	if(!Carga)
//		return(0.);
//
//	TConjuntoAneel* Conjunto = RetornaConjuntoAneel(SE);
//	if(Conjunto == NULL)
//		return(0.);
//
//	strFaixa* FaixaDEC = RetornaFaixaDEC(Conjunto);
//	strFaixa* FaixaFEC = RetornaFaixaFEC(Conjunto);
//	if((FaixaDEC == NULL)||(FaixaFEC == NULL))
//		return(0.);
//
//	double TUSD_BT = 0;
//	if((Carga->NumConsResidencial + Carga->NumConsRural + Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros) != 0)
//		{
//		TUSD_BT  = TUSD.B1 * Carga->NumConsResidencial;
//		TUSD_BT += TUSD.B2 * Carga->NumConsRural;
//		TUSD_BT += TUSD.B3 * (Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros);
//		TUSD_BT /= Carga->NumConsResidencial + Carga->NumConsRural + Carga->NumConsComercial + Carga->NumConsIndustrial + Carga->NumConsOutros;
//		}
//
//	double TransgressaoDIC, TransgressaoFIC, TransgressaoDMIC, TransgressaoDICRI;
//	TransgressaoDIC = TransgressaoFIC = TransgressaoDMIC = TransgressaoDICRI = 0.;
//	if(Carga->NumConsRural != 0)
//		{
//		if(Carga->NumConsA4 != 0)
//			{
//			switch(Periodo)
//				{
//				case etpMensal:
//					TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->MT->DIC->Mensal;
//					TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->MT->FIC->Mensal) * FaixaDEC->NaoUrbano->MT->DIC->Mensal / FaixaFEC->NaoUrbano->MT->FIC->Mensal;
//
//					//Correção 2 - Compensações de DMIC e DICRI são calculadas somente mensalmente
//					TransgressaoDMIC  = DMIC - FaixaDEC->NaoUrbano->MT->DMIC;
////					TransgressaoDICRI = DICRI - (RetornaLimiteDICRI(etlNaoUrbano, etntMT));
//
//					break;
//				case etpTrimestral:
//					TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->MT->DIC->Trimestral;
//					TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->MT->FIC->Trimestral) * FaixaDEC->NaoUrbano->MT->DIC->Trimestral / FaixaFEC->NaoUrbano->MT->FIC->Trimestral;
//					break;
//				case etpAnual:
//					TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->MT->DIC->Anual;
//					TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->MT->FIC->Anual) * FaixaDEC->NaoUrbano->MT->DIC->Anual / FaixaFEC->NaoUrbano->MT->FIC->Anual;
//					break;
//				}
////			TransgressaoDMIC  = DMIC - FaixaDEC->NaoUrbano->MT->DMIC;
////			double LimiteDICRI = RetornaLimiteDICRI(etlNaoUrbano, etntMT);
////			TransgressaoDICRI = DICRI - LimiteDICRI;
//			}
//		else
//			{
//			switch(Periodo)
//				{
//				case etpMensal:
//					TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->BT->DIC->Mensal;
//					TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->BT->FIC->Mensal) * FaixaDEC->NaoUrbano->BT->DIC->Mensal / FaixaFEC->NaoUrbano->BT->FIC->Mensal;
//
//					//Correção 2 - Compensações de DMIC e DICRI são calculadas somente mensalmente
//					TransgressaoDMIC  = DMIC - FaixaDEC->NaoUrbano->BT->DMIC;
////					TransgressaoDICRI = DICRI - (RetornaLimiteDICRI(etlNaoUrbano, etntBT));
//
//					break;
//				case etpTrimestral:
//					TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->BT->DIC->Trimestral;
//					TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->BT->FIC->Trimestral) * FaixaDEC->NaoUrbano->BT->DIC->Trimestral / FaixaFEC->NaoUrbano->BT->FIC->Trimestral;
//					break;
//				case etpAnual:
//					TransgressaoDIC = DIC - FaixaDEC->NaoUrbano->BT->DIC->Anual;
//					TransgressaoFIC = (FIC - FaixaFEC->NaoUrbano->BT->FIC->Anual) * FaixaDEC->NaoUrbano->BT->DIC->Anual / FaixaFEC->NaoUrbano->BT->FIC->Anual;
//					break;
//				}
////			TransgressaoDMIC  = DMIC - FaixaDEC->NaoUrbano->BT->DMIC;
////			double LimiteDICRI = RetornaLimiteDICRI(etlNaoUrbano, etntBT);
////			TransgressaoDICRI = DICRI - LimiteDICRI;
//			}
//		}
//	else
//		{
//		if(Carga->NumConsA4 != 0)
//			{
//			switch(Periodo)
//				{
//				case etpMensal:
//					TransgressaoDIC = DIC - FaixaDEC->Urbano->MT->DIC->Mensal;
//					TransgressaoFIC = (FIC - FaixaFEC->Urbano->MT->FIC->Mensal) * FaixaDEC->Urbano->MT->DIC->Mensal / FaixaFEC->Urbano->MT->FIC->Mensal;
//
//					//Correção 2 - Compensações de DMIC e DICRI são calculadas somente mensalmente
//					TransgressaoDMIC  = DMIC - FaixaDEC->Urbano->MT->DMIC;
////					TransgressaoDICRI = DICRI - (RetornaLimiteDICRI(etlUrbano, etntMT));
//
//					break;
//				case etpTrimestral:
//					TransgressaoDIC = DIC - FaixaDEC->Urbano->MT->DIC->Trimestral;
//					TransgressaoFIC = (FIC - FaixaFEC->Urbano->MT->FIC->Trimestral) * FaixaDEC->Urbano->MT->DIC->Trimestral / FaixaFEC->Urbano->MT->FIC->Trimestral;
//					break;
//				case etpAnual:
//					TransgressaoDIC = DIC - FaixaDEC->Urbano->MT->DIC->Anual;
//					TransgressaoFIC = (FIC - FaixaFEC->Urbano->MT->FIC->Anual) * FaixaDEC->Urbano->MT->DIC->Anual / FaixaFEC->Urbano->MT->FIC->Anual;
//					break;
//				}
////			TransgressaoDMIC  = DMIC - FaixaDEC->Urbano->MT->DMIC;
////			double LimiteDICRI = RetornaLimiteDICRI(etlUrbano, etntMT);
////			TransgressaoDICRI = DICRI - LimiteDICRI;
//			}
//		else
//			{
//			switch(Periodo)
//				{
//				case etpMensal:
//					TransgressaoDIC = DIC - FaixaDEC->Urbano->BT->DIC->Mensal;
//					TransgressaoFIC = (FIC - FaixaFEC->Urbano->BT->FIC->Mensal) * FaixaDEC->Urbano->BT->DIC->Mensal / FaixaFEC->Urbano->BT->FIC->Mensal;
//
//					//Correção 2 - Compensações de DMIC e DICRI são calculadas somente mensalmente
//					TransgressaoDMIC  = DMIC - FaixaDEC->Urbano->BT->DMIC;
////					TransgressaoDICRI = DICRI - (RetornaLimiteDICRI(etlUrbano, etntBT));
//
//					break;
//				case etpTrimestral:
//					TransgressaoDIC = DIC - FaixaDEC->Urbano->BT->DIC->Trimestral;
//					TransgressaoFIC = (FIC - FaixaFEC->Urbano->BT->FIC->Trimestral) * FaixaDEC->Urbano->BT->DIC->Trimestral / FaixaFEC->Urbano->BT->FIC->Trimestral;
//					break;
//				case etpAnual:
//					TransgressaoDIC = DIC - FaixaDEC->Urbano->BT->DIC->Anual;
//					TransgressaoFIC = (FIC - FaixaFEC->Urbano->BT->FIC->Anual) * FaixaDEC->Urbano->BT->DIC->Anual / FaixaFEC->Urbano->BT->FIC->Anual;
//					break;
//				}
////			TransgressaoDMIC  = DMIC - FaixaDEC->Urbano->BT->DMIC;
////			double LimiteDICRI = RetornaLimiteDICRI(etlUrbano, etntBT);
////			TransgressaoDICRI = DICRI - LimiteDICRI;
//			}
//		}
//	double Transgressao = TransgressaoDIC;
//	if(Transgressao < TransgressaoFIC)
//		Transgressao = TransgressaoFIC;
//	if(Transgressao < TransgressaoDMIC)
//		Transgressao = TransgressaoDMIC;
//
//	// Correção 3 - Não se compara infração de DICRI, soma-se a compensação final (Falta implementar)
////	if(Transgressao < TransgressaoDICRI)
////		Transgressao = TransgressaoDICRI;
//	if(Transgressao < 0.)
//		Transgressao = 0.;
//
//	double kei  = 15.;
//	double TUSD_Final = TUSD_BT;
//	if(Carga->NumConsA4 != 0)
//		{
//		kei  = 20.;
//		TUSD_Final = TUSD.A4;
//		}
//
//	VTPatamares	*patamares = (VTPatamares*)AplOwner->GetObject(__classid(VTPatamares));
//	int numPat = patamares->NumPatamar();
//	VTDemanda *demanda = (VTDemanda*)AplOwner->GetObject(__classid(VTDemanda));
//	double EnergiaMensal = 0.0;
//	for(int np = 0; np < numPat; np++)
//		EnergiaMensal += (demanda->P_mw[Carga][np] * 1000.);
//	EnergiaMensal *= 30.;
//
//	// Correção 1 EUSDmédio não se divide por 730
//	double EUSDMedio = TUSD_Final * (EnergiaMensal);
//	double Compensacao = Transgressao * (EUSDMedio/730.) * kei;
////	double EUSDMedio = TUSD_Final * (EnergiaMensal/730.);
////	double Compensacao = Transgressao * EUSDMedio * kei;
//
//	switch(Periodo)
//		{
//		case etpMensal:
//			if(Compensacao > (10. * EUSDMedio))
//				Compensacao = 10. * EUSDMedio;
//			break;
//		case etpTrimestral:
//			if(Compensacao > (30. * EUSDMedio))
//				Compensacao = 30. * EUSDMedio;
//			break;
//		case etpAnual:
//			if(Compensacao > (120. * EUSDMedio))
//				Compensacao = 120. * EUSDMedio;
//			break;
//		}
//
//	return(Compensacao);
//	}
////---------------------------------------------------------------------------
//double __fastcall TCompensacoes::RetornaLimiteDICRI(eTipoLocal Local, eTipoNivelTensao NivelTensao)
//	{
//	if(!LisFaixas)
//		return(0.);
//	double LimiteDICRI = 0.;
//	for(int i = 0; i < LisFaixas->Count; i++)
//		{
//		strFaixa* Faixa = (strFaixa*)LisFaixas->Items[i];
//		if(Local == etlUrbano)
//			{
//			if(NivelTensao == etntBT)
//				{
//				if(LimiteDICRI < Faixa->Urbano->BT->DMIC)
//					LimiteDICRI = Faixa->Urbano->BT->DMIC;
//				}
//			else if(NivelTensao == etntMT)
//				{
//				if(LimiteDICRI < Faixa->Urbano->MT->DMIC)
//					LimiteDICRI = Faixa->Urbano->MT->DMIC;
//				}
//			}
//		else if(Local == etlNaoUrbano)
//			{
//			if(NivelTensao == etntBT)
//				{
//				if(LimiteDICRI < Faixa->NaoUrbano->BT->DMIC)
//					LimiteDICRI = Faixa->NaoUrbano->BT->DMIC;
//				}
//			else if(NivelTensao == etntMT)
//				{
//				if(LimiteDICRI < Faixa->NaoUrbano->MT->DMIC)
//					LimiteDICRI = Faixa->NaoUrbano->MT->DMIC;
//				}
//			}
//		}
//	return(LimiteDICRI);
//	}
////---------------------------------------------------------------------------

