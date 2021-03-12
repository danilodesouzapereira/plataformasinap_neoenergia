// ---------------------------------------------------------------------------

#pragma hdrstop
#include <DateUtils.hpp>
#include <System.Math.hpp>
#include <math.h>
#include "TRelRegulador.h"
// ------------------------------------------------------------PlataformaSinap
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTRegulador.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Constante\Fases.h"
#include <DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TRelRegulador::TRelRegulador(VTApl *apl)
{
	// salva ponteiro para objeto
	mApl = apl;

	// cria lista
	linhas = new TStringList;
	//no inicio do arquivo, coloca o cabeçalho
	linhas->Add(Header());
}

// ---------------------------------------------------------------------------
__fastcall TRelRegulador::~TRelRegulador(void)
{
	if (linhas != NULL)
	{
		delete linhas;
		linhas = NULL;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::Bypass(VTRegulador *regulador)
{
	AnsiString byp = "FALSO";

	if ((regulador != NULL) && (regulador->by_pass))
	{
        byp = "VERDADEIRO";
	}
	return byp;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::Configuracao(VTRegulador *regulador)
{
	AnsiString config;

	switch (regulador->modelo)
	{
		case reguPADRAO:
            config = "modelo padrao";
			break;
		case reguENERQCT:
			config = "modelo enerq";
			break;
		case reguAUTO_BI:
			config = "Auto: bidirecional";
			break;
		case reguAUTO_UNI:
			config = "Auto: unidirecional";
			break;
		case reguAUTO_FIXA:
			config = "Auto: barra fixa";
			break;
		case reguFIXO:
			config = "Tap fixo";
			break;
		default:
			config = "Indeterminado";
			;
	}

	return config;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::EnergiaMensalporFase(VTRegulador *regulador,
																	int indFase)
{
	VTPatamares *patamares = (VTPatamares*)mApl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	AnsiString energiaMensalStr;
	double energiaMW = 0;
	double duracao_horas =0;
	int numPat = patamares->NumPatamar();
	int indiceBarra;


	//percorrer todos patamares do dia para consolidar a energia diaria
	for (int np = 0; np < numPat; np++)
	{
		indiceBarra = IndiceBarraMontante(regulador);
		patamar = patamares->Patamar[np];
		duracao_horas = patamar->Duracao; //em horas?
		energiaMW += fabs(regulador->resflow->Pf_mw[indiceBarra][np][indFase]*duracao_horas);
	}
	//calcula o valor mensal
	energiaMW = energiaMW*nDiasMes;
    //converter numero em string
	energiaMensalStr = DoubleToStr("%4.3f",energiaMW);

	return energiaMensalStr;
}
// ---------------------------------------------------------------------------
bool __fastcall TRelRegulador::ExistePmwNegativo(VTRegulador *regulador, int indFase)
{
	VTPatamares *patamares = (VTPatamares*)mApl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	int numPat = 0;
	double pMW = 0;

	numPat = patamares->NumPatamar();
    //percorrer todos patamares do dia para consolidar a energia diaria
	for (int np = 0; np < numPat; np++)
	{
		for (int nf = indFASE_A; nf < indFASE_C; nf++)
		{
			pMW = regulador->resflow->Pf_mw[indFase][np][nf];
			if (pMW < 0)
			{
				return true;
			}
		}
	}

    return false;
}
// ---------------------------------------------------------------------------
void __fastcall TRelRegulador::Executa(TDateTime data)
{
   // iniciando objetos
	VTRedes *redes = (VTRedes*)mApl->GetObject(__classid(VTRedes));
	VTRegulador *regulador;
    TList *lisRegulador;
	AnsiString line;

    mDate = data;
	nDiasMes = DaysInMonth(data);
	if (LicencaOk())
	{
        lisRegulador = new TList;
		redes->LisLigacao(lisRegulador,eqptoREGULADOR);
		for(int nr = 0; nr < lisRegulador->Count; nr++)
        {
			regulador = (VTRegulador*) lisRegulador->Items[nr];
			//preenche a linha com a informacao do regulador
			try
			{
				line = LinhaRegulador(regulador);
			}
			catch(...)
			{
				if (regulador != NULL)
				{
					line = "Erro ao inserir info do regulador id: " +IntToStr(regulador->Id);
				}
            }
			linhas->Add(line);
		}
		delete lisRegulador;
   }
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::Fases(VTRegulador *regulador)
{
	VTFases *fases = (VTFases*)mApl->GetObject(__classid(VTFases));
	AnsiString faseStr = "";

	if (regulador->ligacao == lgDELTA_ABERTO)
	{
        faseStr = "Vértice na fase ";
	}
	if (fases != NULL)
	{
		faseStr += fases->AsStrABCN[regulador->fases];
	}
	return faseStr;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::Header(void)
{
	AnsiString cabecalho;
//    "MES" - mês
//    "COD_ID" - Extern_Id do regulador
//    "COD_SINAP"- codigo do regulador
//    "BYPASS"- codigo do regulador
//    "LIGACAO"- lstring da ligacao
//    "FASES"- Faseamento
//    "CONFIG"- string da configuracao
//    "REDE"- codigo da rede do regulador
//	"ENE_A" - Energia passante por mês na fase A
//	"ENE_B" - Energia passante por mês na fase B
//	"ENE_C" - Energia passante por mês na fase C
//	"ENE_N" - Energia passante por mês no neutro

	cabecalho = "MES;COD_ID;COD_SINAP;BYPASS;LIGACAO;FASES;CONFIG;REDE;ENE_A[MWh_mes];ENE_B[MWh_mes];ENE_C[MWh_mes];ENE_N[MWh_mes]";

    return cabecalho;
}
// ---------------------------------------------------------------------------
int __fastcall TRelRegulador::IndiceBarraMontante(VTRegulador *regulador)
{
	int indice = 0;
//	VTBarra *barra;

//	barra = regulador->cfg_auto.bar_ref_inv;
//	if (barra != NULL)
//	{
//		indice = regulador->IndiceBarra(barra);
//	}
//	else
//	{   //olha o resutlado do fluxo
//		if (ExistePmwNegativo(regulador,0))
//		{
//			indice = 1;
//		}
//		else
//		{
//            indice = 0;
//        }
//    }
	//olha o resutlado do fluxo
	if (ExistePmwNegativo(regulador,0))
	{
		indice = 1;
	}
	else
	{
		indice = 0;
	}

	return indice;
}
// ---------------------------------------------------------------------------
bool __fastcall TRelRegulador::LicencaOk(void)
{
	VTLicencas *licencas = (VTLicencas*)mApl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca;
	bool licencaOk = false;

	// verifica se a licenca atende as condicoes
	if (licencas != NULL)
	{
		licenca = licencas->ExisteLicenca();
		//se possuir licença e for do tipo Master e de Rondonia ou Acre
		if ((licenca != NULL)&& ((licenca->Empresa == ELETROBRAS_ACRE) ||
		(licenca->Empresa == ELETROBRAS_RONDONIA))&&(licenca->Tipo == tipoMASTER))
			{licencaOk = true;}
	}

	return licencaOk;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::Ligacao(VTRegulador *regulador)
{
	AnsiString config;

	switch (regulador->ligacao)
	{
		case lgEST_AT:
			config = "Estrela aterrada";
			break;
		case lgTRIANG:
			config = "Delta";
			break;
		case lgDELTA_FECHADO:
			config = "Delta Fechado";
			break;
		case lgDELTA_ABERTO:
			config = "Delta Aberto";
			break;
		case lgMONOFASICO:
			config = "Monofasico";
			break;
		case lgEST_ISO:
			config = "Estrela Isolada";
			break;
		default:
			config = "Indeterminado";
			;
	}

	return config;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TRelRegulador::LinhaRegulador(VTRegulador *regulador)
{
	AnsiString linhaRegulador;

	//    "MES" - mês
	linhaRegulador = MesStr(mDate)+ ";";
	//    "COD_ID" - Extern_Id do regulador
	linhaRegulador += regulador->Extern_id + ";";
	//    "COD_SINAP"- codigo do regulador
	linhaRegulador += regulador->Codigo + ";";
	//    "BYPASS"- codigo do regulador
	linhaRegulador += Bypass(regulador) + ";";
	//    "LIGACAO"- lstring da ligacao
	linhaRegulador += Ligacao(regulador) + ";";
	//    "FASES"- Faseamento
	linhaRegulador += Fases(regulador) + ";";
	//    "CONFIG"- string da configuracao
	linhaRegulador += Configuracao(regulador) + ";";
	//    "REDE"- codigo da rede do regulador
    linhaRegulador += regulador->rede->Codigo + ";";
	//	"ENE_A" - Energia passante por mês na fase A
	linhaRegulador += EnergiaMensalporFase(regulador,indFASE_A) + ";";
	//	"ENE_B" - Energia passante por mês na fase B
	linhaRegulador += EnergiaMensalporFase(regulador,indFASE_B) + ";";
	//	"ENE_C" - Energia passante por mês na fase C
	linhaRegulador += EnergiaMensalporFase(regulador,indFASE_C) + ";";
	//	"ENE_N" - Energia passante por mês no neutro
	linhaRegulador += EnergiaMensalporFase(regulador,indNEUTRO);

	return linhaRegulador;
}
// ---------------------------------------------------------------------------
AnsiString  __fastcall TRelRegulador::MesStr(TDateTime data)
{
	AnsiString mesStr;

	unsigned short ano,mes,dia;

	data.DecodeDate(&ano,&mes,&dia);

	switch (mes)
	{
		case 1:
			mesStr = "01jan";
			break;
		case 2:
			mesStr = "02fev";
			break;
		case 3:
			mesStr = "03mar";
			break;
		case 4:
			mesStr = "04abr";
			break;
		case 5:
			mesStr = "05mai";
			break;
		case 6:
			mesStr = "06jun";
			break;
		case 7:
			mesStr = "07jul";
			break;
		case 8:
			mesStr = "08ago";
			break;
		case 9:
			mesStr = "09set";
			break;
		case 10:
			mesStr = "10out";
			break;
		case 11:
			mesStr = "11nov";
			break;
		case 12:
			mesStr = "12dez";
			break;
		default:
			mesStr = "";
	}

	return mesStr;
}
// ---------------------------------------------------------------------------
void __fastcall TRelRegulador::SalvaArquivo(AnsiString filename)
{
	VTPath *path = (VTPath*)mApl->GetObject(__classid(VTPath));
	AnsiString filename_aux, filepath, filedir;
	int indice = 1;

	//define um nome de arquivo
	filename_aux = filename;
//	filename.SetLength(filename.Length() - 4);
	filename_aux += ".csv";
	//define onde salvar o arquivo
	filedir = path->DirExporta() +"\\";

	filepath = filedir + filename_aux;
	while(FileExists(filepath))
	{
		filename_aux = filename + "_" + IntToStr(indice);
		filename_aux += ".csv";
		filepath = filedir + filename_aux;
		indice++;
    }
	//salva arquivo
	linhas->SaveToFile(filepath);
}
// ---------------------------------------------------------------------------
