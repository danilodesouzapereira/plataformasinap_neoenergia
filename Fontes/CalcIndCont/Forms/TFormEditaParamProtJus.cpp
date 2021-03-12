// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFormEditaParamProtJus.h"
#include "..\VTCalcIndCont.h"
#include "..\VTBuscaProt.h"
#include "..\VTDefeito.h"
#include "..\VTBlocoRad.h"
#include "..\..\BDadoParam\Dados\VTParamConf.h"

#include <PlataformaSinap\Fontes\Classes\VTClasse.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEditaParamProtJus *FormEditaParamProtJus;

// ---------------------------------------------------------------------------
__fastcall TFormEditaParamProtJus::TFormEditaParamProtJus(TComponent* Owner, VTApl* apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	this->apl = apl_owner;
	lisParamEqpto = new TList(); // lista de strParamEqpto
	lisChavesProt = new TList(); // lista de chaves de protecao das redes carregadas
	IniciaLisChavesProt(); // inicia lista local lisChavesProt
	// Inicia listView em funcao de lista de Defeitos por bloco(MT) ou por carga(BT)
	IniciaTListParamChaves();
	IniciaTListParamCargas();
	IniciaTVReligadas();
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaParamProtJus::~TFormEditaParamProtJus(void)
{
	// Grava parameqpto em arquivo
	if (lisParamEqpto)
	{
		LimpaTList(lisParamEqpto);
		delete lisParamEqpto;
		lisParamEqpto = NULL;
	}
	if (lisChavesProt)
	{
		delete lisChavesProt;
		lisChavesProt = NULL;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEditaParamProtJus::LeDado(FILE* fp, bool &flagEOF)
{
	AnsiString x;
	char aux;

	flagEOF = false;

	x = "";
	aux = fgetc(fp);
	while ((aux != ';') && (aux != '\n') && (aux != EOF))
	{
		x += AnsiString(aux);
		aux = fgetc(fp);
	}
	if (aux == EOF)
		flagEOF = true;

	return (x);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::GravaArqParamEqpto(void)
{
	FILE *fout;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq;
	VTChave *pchave;
	VTCarga *carga;
	VTRede *rede;
	strDadoParamEqpto *paramEqpto;
	AnsiString tipoChave, codChave;
	double compBlocos, taxa, trep;
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	TList *lisAux;
	VTBloco *bloco;
	double compBloco, nfalhaAux, nfalha, treparoAux, treparo;
	VTDefeito *pdef;

	// Funcao que grava parametros em pamBlocoProt.txt e popula lista de parametros
	// por equipamento de protecao para ser usado no módulo de cálculo de indicadores

	lisAux = new TList();

	// Reinicia lista de parametros por equipamento
	LimpaTList(lisParamEqpto);

	for (int n = 1; n < sgridBlocosProt->RowCount; n++)
	{
		if (sgridBlocosProt->Cells[4][n].Compare("") == 0)
			continue;
		if (sgridBlocosProt->Cells[5][n].Compare("") == 0)
			continue;

		pchave = (VTChave*)sgridBlocosProt->Objects[0][n];

		lisAux->Clear();
		compBloco = 0.0;
		calcIndCont->BuscaProt->ListaZonaProtJus(pchave, lisAux);//hk201808: reformula: LisZonaProtJus(pchave, lisAux);
		nfalha = 0.0;
		treparo = 0.0;
		for (int nb = 0; nb < lisAux->Count; nb++)
		{
			bloco = (VTBloco*)lisAux->Items[nb];
			// compBloco += ComprimentoBloco(bloco);
			pdef = calcIndCont->Defeito[bloco];
			if (pdef == NULL)
				continue;
			compBloco += pdef->ComprimentoBloco;
			nfalhaAux = (pdef->TaxaFalha * pdef->ComprimentoBloco);
			nfalha += nfalhaAux;
			treparo += (nfalhaAux * pdef->TReparo);
		}
		if (IsDoubleZero(nfalha))
			treparo = 0.0;
		else
			treparo /= nfalha;
		nfalha /= compBloco;

		nfalhaAux = StrToDouble(sgridBlocosProt->Cells[4][n]);
		treparoAux = StrToDouble(sgridBlocosProt->Cells[5][n]);

		// Verifica se houve alterações nas taxas
		if (IsDoubleZero(nfalha - nfalhaAux, 1e-2) && IsDoubleZero(treparo - treparoAux, 1e-2))
			continue;
		// if(((int)(treparo * 100)) == ((int)(treparoAux * 100))) continue;

		tipoChave = sgridBlocosProt->Cells[0][n];
		codChave = sgridBlocosProt->Cells[2][n];
		// fprintf(fout, "%s;", tipoChave); //tipo (religador, BF, ..)
		// fprintf(fout, "%s;", codChave); //codChave

		// fprintf(fout, "%s;", sgridBlocosProt->Cells[2][n]); //comprimento blocos
		compBlocos = StrToDouble(sgridBlocosProt->Cells[3][n]);

		// fprintf(fout, "%s;", sgridBlocosProt->Cells[3][n]); //taxa falhas/km/ano
		taxa = StrToDouble(sgridBlocosProt->Cells[4][n]);
		// fprintf(fout, "%s\n", sgridBlocosProt->Cells[4][n]);
		trep = StrToDouble(sgridBlocosProt->Cells[5][n]);

		lisParamEqpto->Add(paramEqpto = new strDadoParamEqpto);

		paramEqpto->rede = pchave->rede;
		paramEqpto->eqpto = (VTEqpto*)pchave;
		paramEqpto->codEqpto = pchave->Codigo;
		paramEqpto->codRede = paramEqpto->rede->Codigo;
		paramEqpto->carga = NULL;
		// hk0515a paramEqpto->paramRede = NULL;
		paramEqpto->tipoRede = "MT";
		// hkpc paramEqpto->flagEncontrado = true;
		paramEqpto->nFalhasEquip = taxa * compBlocos;
		paramEqpto->durTotal = trep * paramEqpto->nFalhasEquip;
		paramEqpto->ndiasObs = 365;
	}

	for (int n = 1; n < sgridCargasBT->RowCount; n++)
	{
		if (sgridCargasBT->Cells[2][n].Compare("") == 0)
			continue;
		if (sgridCargasBT->Cells[3][n].Compare("") == 0)
			continue;
		carga = (VTCarga*)sgridCargasBT->Objects[0][n];
		rede = (VTRede*)sgridCargasBT->Objects[1][n];

		// fprintf(fout, "%s;", sgridBlocosProt->Cells[3][n]); //taxa falhas/km/ano
		taxa = StrToDouble(sgridCargasBT->Cells[2][n]);
		// fprintf(fout, "%s\n", sgridBlocosProt->Cells[4][n]);
		trep = StrToDouble(sgridCargasBT->Cells[3][n]);

		lisParamEqpto->Add(paramEqpto = new strDadoParamEqpto);
		paramEqpto->rede = rede;
		paramEqpto->eqpto = NULL;
		paramEqpto->codEqpto = carga->Codigo;
		paramEqpto->codRede = rede->Codigo;
		paramEqpto->carga = carga;
		// hk0515a paramEqpto->paramRede = NULL;
		paramEqpto->tipoRede = "BT";
		// hkpc paramEqpto->flagEncontrado = true;
		paramEqpto->nFalhasEquip = taxa;
		paramEqpto->durTotal = trep * paramEqpto->nFalhasEquip;
		paramEqpto->ndiasObs = 365;
	}

	delete lisAux;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::IniciaTVReligadas(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRadial *radial;
	VTSubestacao *se;
	VTPrimario *pri;
	TList *lisSe, *lisPrimario;
	TTreeNode *node;

	tvReligadas->Items->Clear();
	radial = DLL_NewObjRadial(this->apl);
	radial->Inicia(redes);
	lisSe = radial->LisSubestacao();

	if (lisSe->Count > 0)
	{
		for (int nse = 0; nse < lisSe->Count; nse++)
		{
			se = (VTSubestacao*)lisSe->Items[nse];
			node = tvReligadas->Items->AddChildObject(NULL, se->Rede->Codigo, NULL);
			lisPrimario = se->LisPrimario();
			for (int np = 0; np < lisPrimario->Count; np++)
			{
				pri = (VTPrimario*)lisPrimario->Items[np];
				if (!pri->Rede->Carregada)
					continue;
				IniciaTVReligadasRede(node, pri->Rede);
			}
		}
	}
	else
	{
		lisPrimario = radial->LisPrimario();
		for (int np = 0; np < lisPrimario->Count; np++)
		{
			pri = (VTPrimario*)lisPrimario->Items[np];
			if (!pri->Rede->Carregada)
				continue;
			IniciaTVReligadasRede(NULL, pri->Rede);
		}
	}

	delete radial;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::IniciaTVReligadasRede(TTreeNode *nodeRede, VTRede *rede)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTBuscaProt *buscaProt;
	TList *lisRedes, *lisAux, *lisBlocoRad, *lisChaves, *lisBlocosReliga;
	VTChave *chaveReliga, *chave;
	VTBlocoRad *blocoRad, *blocoRadReliga, *blocoRadAux;
	TTreeNode *node, *node0;
	bool flagAbre;
	double nfalhasTot, nfalhasPisca;
	AnsiString aux;

	buscaProt = calcIndCont->BuscaProt;
	lisChaves = new TList();

	///////////////////////////////////////////////////////////////
	// Reinicia parametro de profundidade dos religamentos
	///////////////////////////////////////////////////////////////
	VTParamConf *paramConf = (VTParamConf*)apl->GetObject(__classid(VTParamConf));
	buscaProt->IProfReliga = paramConf->ParamGerais->iProfReliga;

	// Busca os religadores e disjuntores
	lisRedes = redes->LisRede();
	node0 = tvReligadas->Items->AddChildObject(nodeRede, rede->Codigo, NULL);

	// Cria primeira lista com chaves blocosRad de chaves religadoras
	lisAux = new TList();
	lisBlocoRad = new TList();
	lisBlocosReliga = new TList();
	// Gera lista de disjuntores
	lisChaves->Clear();
	buscaProt->LisChaveOrd(lisChaves);
	// Retira chaves que não estao na rede
	for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
	{
		chave = (VTChave*)lisChaves->Items[nc];
		if (chave->rede == rede)
			continue;
		lisChaves->Delete(nc);
	}

	for (int nc = lisChaves->Count - 1; nc >= 0; nc--)
	{
		chave = (VTChave*)lisChaves->Items[nc];
		if ((chave->TipoChave->Tipo == chaveDISJUNTOR) ||
			(chave->TipoChave->Tipo == chaveRELIGADORA))
			continue;
		lisChaves->Delete(nc);
	}

	for (int nc = 0; nc < lisChaves->Count; nc++)
	{
		chave = (VTChave*)lisChaves->Items[nc];
		lisBlocosReliga->Clear();
		buscaProt->ListaZonaReligaJus(chave, lisBlocosReliga);
		if ((blocoRadReliga = buscaProt->IdentificaBlocoRad(chave)) == NULL)
			continue;
		GeraPiscadas(blocoRadReliga, nfalhasTot, nfalhasPisca);
		aux.sprintf("%s (nf:%6.1f, np:%6.1f)", blocoRadReliga->Chave->Codigo.c_str(), nfalhasTot,
			nfalhasPisca);
		node = tvReligadas->Items->AddChildObject(node0, aux, blocoRadReliga);

		do
		{
			flagAbre = false;
			for (int nt = 0; nt < tvReligadas->Items->Count; nt++)
			{
				node = tvReligadas->Items->Item[nt];
				if (node->Count > 0)
					continue;
				blocoRad = (VTBlocoRad*)node->Data;
				if (lisBlocosReliga->IndexOf(blocoRad->Bloco) == -1)
					continue;
				if (blocoRad->LisBlocoRad->Count == 0)
					continue;
				for (int nb = 0; nb < blocoRad->LisBlocoRad->Count; nb++)
				{
					blocoRadAux = (VTBlocoRad*)blocoRad->LisBlocoRad->Items[nb];
					if (lisBlocosReliga->IndexOf(blocoRadAux->Bloco) == -1)
						continue;
					flagAbre = true;
					GeraPiscadas(blocoRadAux, nfalhasTot, nfalhasPisca);
					aux.sprintf("%s (nf:%6.1f, np:%6.1f)", blocoRadAux->Chave->Codigo.c_str(), nfalhasTot,
						nfalhasPisca);
					tvReligadas->Items->AddChildObject(node, aux, blocoRadAux);
				}
			}
		}
		while (flagAbre == true);

	}
	delete lisBlocosReliga;
	delete lisChaves;
	delete lisAux;
	delete lisBlocoRad;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::GeraPiscadas(VTBlocoRad* blocoRadReliga, double &nfalhasTot,
	double &nfalhasPisca)
{
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTBuscaProt *buscaProt;
	TList *lisBlocosReliga;
	VTBloco *bloco;
	strDadoParamBloco *paramBloco;

	nfalhasTot = nfalhasPisca = 0.0;

	lisBlocosReliga = new TList();
	buscaProt = calcIndCont->BuscaProt;
	buscaProt->ListaZonaReligaJus(blocoRadReliga->Chave, lisBlocosReliga);
	for (int n = 0; n < lisBlocosReliga->Count; n++)
	{
		bloco = (VTBloco*)lisBlocosReliga->Items[n];
		// if((paramBloco = calcIndCont->ParametroConfBloco(bloco)) == NULL) continue;
		if ((paramBloco = calcIndCont->ParamConf->ParamBloco[bloco]) == NULL)
			continue;
		nfalhasTot += paramBloco->nFalhas;
	}

	nfalhasPisca = nfalhasTot * calcIndCont->ParamConf->ParamGerais->percDefTrans;//hk201808: reformula: calcIndCont->PercDefTrans;

	delete lisBlocosReliga;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::IniciaLisChavesProt(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTChave *pchave;
	VTCarga *carga;
	TList *lisAux;

	redes->LisEqpto(lisChavesProt, eqptoCHAVE);
	for (int n = lisChavesProt->Count - 1; n >= 0; n--)
	{
		pchave = (VTChave*)lisChavesProt->Items[n];
		if ((pchave->Fechada) && (calcIndCont->BuscaProt->LisTipoEqProt->IndexOf(pchave->TipoChave) != -1))  //hk201808: reformula
			continue;
		////hkRevAloca
		// if((pchave->Estado == chvFECHADA)&&(calcIndCont->LisTipoEqProt->IndexOf(pchave->TipoChave) != -1)) continue; ////hkRevAloca
		lisChavesProt->Delete(n);
	}

	// paramEqpto = NULL;
	// for(int np = 0; np < lisParamEqpto->Count; np++)
	// {
	// paramEqpto = (strParamEqpto*)lisParamEqpto->Items[np];
	// if((paramEqpto->codEqpto.AnsiCompare(codEqpto) == 0)&&(paramEqpto->codRede.AnsiCompare(codRede) == 0))
	// {break;}
	// paramEqpto = NULL;
	// }
	// if(paramEqpto == NULL) lisParamEqpto->Add(paramEqpto = new strParamEqpto);
	// paramEqpto->eqpto = NULL;
	// paramEqpto->carga = NULL;
	// paramEqpto->paramRede = NULL;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::IniciaTListParamCargas(void)
{
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTCarga *carga;
	strDadoParamEqpto *paramEqpto;
	strDadoParamRede *paramRede;
	TList *lisSE, *lisPrimario, *lisCargasPrimario;
	VTSubestacao *pse;
	VTPrimario *primario;

	// Inicia sgridBlocosProt em funcao de lista de Defeitos por bloco(MT)

	// Lista cargas equivalentes
	sgridCargasBT->RowCount = 2;
	sgridCargasBT->ColCount = 4;

	sgridCargasBT->Cells[0][0] = "Carga";
	sgridCargasBT->Cells[1][0] = "Rede MT";
	sgridCargasBT->Cells[2][0] = "Taxa(f/ano)";
	sgridCargasBT->Cells[3][0] = "Trep(min)";

	lisSE = calcIndCont->Radial->LisSubestacao();
	lisPrimario = new TList();
	for (int nse = 0; nse < lisSE->Count; nse++)
	{
		pse = (VTSubestacao*)lisSE->Items[nse];
		lisPrimario->Assign(pse->LisPrimario(), laOr);
	}

	lisCargasPrimario = new TList();
	for (int np = 0; np < lisPrimario->Count; np++)
	{
		primario = (VTPrimario*)lisPrimario->Items[np];
		lisCargasPrimario->Clear();
		primario->Rede->LisEqbar(lisCargasPrimario, eqptoCARGA);

		for (int nc = 0; nc < lisCargasPrimario->Count; nc++)
		{
			carga = (VTCarga*)lisCargasPrimario->Items[nc];
			// if(carga->Codigo.AnsiCompare("PAC38956-4533") == 0)
			// int kkk = 0;
			// if(carga->classe->Tag == consEQUIVALENTE) continue;

			// busca paramEqpto
			paramEqpto = calcIndCont->ParamConf->ParametroDadoEqpto(carga);//hk201808: reformula
			sgridCargasBT->Cells[0][sgridCargasBT->RowCount - 1] = carga->Codigo;
			sgridCargasBT->Cells[1][sgridCargasBT->RowCount - 1] = primario->Rede->Codigo;
			if (paramEqpto != NULL)
			{
				sgridCargasBT->Cells[2][sgridCargasBT->RowCount - 1] =
					DoubleToStr("%6.0f", paramEqpto->nFalhasEquip);
				sgridCargasBT->Cells[3][sgridCargasBT->RowCount - 1] =
					DoubleToStr("%6.2f", paramEqpto->trep);
			}
			else
			{
				// paramRede = calcIndCont->ParametroConf(primario->Rede);
				paramRede = calcIndCont->ParamConf->ParamRede[primario->Rede];
				if (paramRede != NULL)
				{
					// sgridCargasBT->Cells[2][sgridCargasBT->RowCount - 1] = DoubleToStr("%6.0f", paramRede->taxa_BT);

					// if(!IsDoubleZero(paramRede->cli_bt))
					// sgridCargasBT->Cells[3][sgridCargasBT->RowCount - 1] = DoubleToStr("%6.2f", (paramRede->clih_bt * 60./ paramRede->cli_bt));
					// else
					// sgridCargasBT->Cells[3][sgridCargasBT->RowCount - 1] = "";
				}
				else
				{
					sgridCargasBT->Cells[2][sgridCargasBT->RowCount - 1] = "";
					sgridCargasBT->Cells[3][sgridCargasBT->RowCount - 1] = "";
				}
			}
			sgridCargasBT->Objects[0][sgridCargasBT->RowCount - 1] = carga;
			sgridCargasBT->Objects[1][sgridCargasBT->RowCount - 1] = primario->Rede;
			sgridCargasBT->RowCount++;
		}
	}
	if (sgridCargasBT->RowCount > 1)
		sgridCargasBT->RowCount--;
	delete lisPrimario;
	delete lisCargasPrimario;

}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::IniciaTListParamChaves(void)
{
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTTipoChave* tipoChave;
	VTChave *pchave;
	TList *lisAux, *lisBloco;
	TList *lisCargas;
	TList *lisTrec;
	VTBloco *bloco;
	double compBloco, nfalhaAux, nfalha, treparo;
	VTDefeito *pdef;
	int ncons;

	// Inicia sgridBlocosProt em funcao de lista de Defeitos por bloco(MT)

	sgridBlocosProt->RowCount = 2;
	sgridBlocosProt->ColCount = 7;
	sgridBlocosProt->Cells[0][0] = "Tipo";
	sgridBlocosProt->Cells[1][0] = "Rede";
	sgridBlocosProt->Cells[2][0] = "Código";
	sgridBlocosProt->Cells[3][0] = "Extensão(km)";
	sgridBlocosProt->Cells[4][0] = "Taxa(f/km/ano)";
	sgridBlocosProt->Cells[5][0] = "Trep(min)";
	sgridBlocosProt->Cells[6][0] = "ConsJustante";

	lisAux = new TList();
	lisBloco = new TList();
	lisCargas = new TList();
	lisTrec = new TList();

	for (int nc = 0; nc < calcIndCont->BuscaProt->LisTipoEqProt->Count; nc++)//hk201808: reformula
	{
		tipoChave = (VTTipoChave*)calcIndCont->BuscaProt->LisTipoEqProt->Items[nc];      //hk201808: reformula
		for (int n = 0; n < lisChavesProt->Count; n++)
		{
			pchave = (VTChave*)lisChavesProt->Items[n];
			if (pchave->TipoChave != tipoChave)
				continue;
			lisAux->Clear();
			compBloco = 0.0;
			calcIndCont->BuscaProt->ListaZonaProtJus(pchave, lisAux);//hk201808: reformula
			nfalha = 0.0;
			treparo = 0.0;
			for (int nb = 0; nb < lisAux->Count; nb++)
			{
				bloco = (VTBloco*)lisAux->Items[nb];
				// compBloco += ComprimentoBloco(bloco);
				pdef = calcIndCont->Defeito[bloco];
				if (pdef == NULL)
					continue;
				compBloco += pdef->ComprimentoBloco;
				nfalhaAux = (pdef->TaxaFalha * pdef->ComprimentoBloco);
				nfalha += nfalhaAux;
				treparo += (nfalhaAux * pdef->TReparo);
			}
			if (IsDoubleZero(nfalha))
				treparo = 0.0;
			else
				treparo /= nfalha;
			if (IsDoubleZero(compBloco))
				nfalha = 0.0;
			else
				nfalha /= compBloco;

			// Numero de consumidores a jusante da chave de protecao
			lisBloco->Clear();
			calcIndCont->BuscaProt->LisBlJus(pchave, lisBloco);//hk201808: reformula
			ncons = 0;
			lisCargas->Clear();
			for (int nbb = 0; nbb < lisBloco->Count; nbb++)
			{
				bloco = (VTBloco*)lisBloco->Items[nbb];
				bloco->LisEqbar(lisCargas, eqptoCARGA);
			}

			ncons = NCons(lisCargas);

			sgridBlocosProt->Cells[0][sgridBlocosProt->RowCount - 1] = pchave->TipoChave->Codigo;
			sgridBlocosProt->Cells[1][sgridBlocosProt->RowCount - 1] = pchave->rede->Codigo;
			sgridBlocosProt->Cells[2][sgridBlocosProt->RowCount - 1] = pchave->Codigo;
			sgridBlocosProt->Cells[3][sgridBlocosProt->RowCount - 1] =
				DoubleToStr("%6.2f", compBloco);
			sgridBlocosProt->Cells[4][sgridBlocosProt->RowCount - 1] = DoubleToStr("%6.2f", nfalha);
			sgridBlocosProt->Cells[5][sgridBlocosProt->RowCount - 1] =
				DoubleToStr("%6.2f", treparo);
			sgridBlocosProt->Cells[6][sgridBlocosProt->RowCount - 1] = IntToStr(ncons);
			sgridBlocosProt->Objects[0][sgridBlocosProt->RowCount - 1] = pchave;

			// Gera valores de taxas dos blocos de pchave
			// lisBloco->Clear();
			// calcIndCont->LisZonaProtJus(pchave, lisBloco);

			sgridBlocosProt->RowCount++;
		}
	}
	if (sgridBlocosProt->RowCount > 1)
		sgridBlocosProt->RowCount--;

	// Busca informações em arquivo texto
	// IniciaLisParamEqpto();

	delete lisBloco;
	delete lisAux;
	delete lisTrec;
	delete lisCargas;
	// calcIndCont->LisZonaProtJus(pchave, lisBlocosJusDef);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEditaParamProtJus::ComprimentoBloco(VTBloco *bloco)
{
	TList *lisLiga = bloco->LisLigacao();
	VTEqpto *pliga;
	double comp_km = 0.0;

	for (int n = 0; n < lisLiga->Count; n++)
	{
		pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
		comp_km += ((VTTrecho*)pliga)->Comprimento_km;
	}
	return (comp_km);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::ActionAtualizaParamEqptoExecute(TObject *Sender)
{
	/* VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	 //Grava parametros e popula lista de parametros por equipamento
	 //para equipamentos com taxa e treparo diferente de ""
	 GravaArqParamEqpto();

	 //Reinicia parametros por bloco
	 calcIndCont->ReiniciaLisParamEqptoExterno(lisParamEqpto);

	 Close();
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::ActionMostraCargasExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTCarga *carga;
	AnsiString className = Sender->ClassName();
	int index;

	if (className.AnsiCompare("TStringGrid") != 0)
		return;
	if (((TStringGrid*)Sender)->Name.Compare("sgridCargasBT") != 0)
		return;
	if ((index = sgridCargasBT->Row) < 1)
		return;

	if (sgridCargasBT->Objects[0][index] == NULL)
		return;

	carga = (VTCarga*)sgridCargasBT->Objects[0][index];

	grafico->Moldura(carga);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::ActionMostraBlocosPiscadasExecute(TObject *Sender)

{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node;
	VTBloco *bloco;
	VTLigacao *pliga;
	VTBlocoRad *blocoRad;
	TList *lisLiga, *lisBlocosReliga;
	TColor color;
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTBuscaProt *buscaProt = calcIndCont->BuscaProt;

	if ((node = tvReligadas->Selected) == NULL)
		return;
	if (node->Data == NULL)
		return;

	blocoRad = (VTBlocoRad*)node->Data;
	lisLiga = new TList();
	// lisBlocosReliga = new TList();
	lisBlocosReliga = buscaProt->LisBlJusChave(blocoRad->Chave);
	// buscaProt->ListaZonaReligaJus(blocoRad->Chave, lisBlocosReliga);
	for (int nb = 0; nb < lisBlocosReliga->Count; nb++)
	{
		bloco = (VTBloco*)lisBlocosReliga->Items[nb];
		lisLiga->Assign(bloco->LisLigacao(), laOr);
	}

	if (lisLiga->Count == 0)
		return;
	color = ((VTLigacao*)lisLiga->First())->rede->Color;

	grafico->DestacaEqpto(lisLiga, color, 10);
	delete lisLiga;
	// delete lisBlocosReliga;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::ActionMostraBlocoReligaExecute(TObject *Sender)

{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node;
	VTBloco *bloco;
	VTLigacao *pliga;
	VTBlocoRad *blocoRad;
	TList *lisLiga, *lisBlocosReliga;
	TColor color;
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTBuscaProt *buscaProt = calcIndCont->BuscaProt;

	if ((node = tvReligadas->Selected) == NULL)
		return;
	if (node->Data == NULL)
		return;

	blocoRad = (VTBlocoRad*)node->Data;
	lisLiga = new TList();
	lisBlocosReliga = new TList();

	buscaProt->ListaZonaReligaJus(blocoRad->Chave, lisBlocosReliga);
	for (int nb = 0; nb < lisBlocosReliga->Count; nb++)
	{
		bloco = (VTBloco*)lisBlocosReliga->Items[nb];
		lisLiga->Assign(bloco->LisLigacao(), laOr);
	}

	if (lisLiga->Count == 0)
		return;
	color = ((VTLigacao*)lisLiga->First())->rede->Color;

	grafico->DestacaEqpto(lisLiga, color, 10);
	delete lisLiga;
	delete lisBlocosReliga;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::ActionMostraTaxasExecute(TObject *Sender)
{
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisBlocosAux;
	VTChave *pchave, *pchaveMax, *pchaveAux;
	VTBloco *bloco;
	double taxa, taxaMin, taxaMax;
	TColor cor, cor0, cor1, cor2, corNULL;
	double valor0, valor1, valor2;
	TList *lisAux, *lisAux00;
	bool flag;

	cor0 = clRed;
	cor1 = clYellow;
	cor2 = clGreen;
	corNULL = clGray;

	// ordena chaves por taxa de falhas
	lisAux = new TList();
	lisAux00 = new TList();

	for (int n = 1; n < sgridBlocosProt->RowCount; n++)
	{
		taxa = StrToDouble(sgridBlocosProt->Cells[4][n]);
		if (!IsDoubleZero(taxa))
			continue;
		lisAux00->Add(sgridBlocosProt->Objects[0][n]);
	}

	taxaMax = -1;
	flag = true;
	while (flag)
	{
		flag = false;
		taxaMax = -1;
		for (int n = 1; n < sgridBlocosProt->RowCount; n++)
		{
			pchave = (VTChave*)sgridBlocosProt->Objects[0][n];
			if (lisAux00->IndexOf(pchave) != -1)
				continue;
			if (lisAux->IndexOf(pchave) != -1)
				continue;
			taxa = StrToDouble(sgridBlocosProt->Cells[4][n]);

			if (taxa >= taxaMax)
			{
				taxaMax = taxa;
				pchaveMax = pchave;
				flag = true;
			}
		}
		if (flag)
			lisAux->Add(pchaveMax);
	}

	lisBlocosAux = new TList();
	// Blocos com taxa zero
	for (int n = 0; n < lisAux00->Count; n++)
	{
		pchave = (VTChave*)lisAux00->Items[n];
		lisBlocosAux->Clear();
		calcIndCont->BuscaProt->ListaZonaProtJus(pchave, lisBlocosAux);  //hk201808: reformula
		for (int nb = 0; nb < lisBlocosAux->Count; nb++)
		{
			bloco = (VTBloco*)lisBlocosAux->Items[nb];
			bloco->Color = corNULL;
		}
	}

	int iconta, icontaLim;
	iconta = 0;
	icontaLim = 0;
	cor = cor0;

	for (int n = 0; n < lisAux->Count; n++)
	{
		pchave = (VTChave*)lisAux->Items[n];
		pchaveAux = NULL;
		for (int nn = 1; nn < sgridBlocosProt->RowCount; nn++)
		{
			taxa = StrToDouble(sgridBlocosProt->Cells[4][nn]);
			pchaveAux = (VTChave*)sgridBlocosProt->Objects[0][nn];
			if (pchaveAux == pchave)
				break;
			pchaveAux = NULL;
		}
		if (pchaveAux == NULL)
			continue;

		if (n == 0)
			valor2 = taxa;

		lisBlocosAux->Clear();
		calcIndCont->BuscaProt->ListaZonaProtJus(pchave, lisBlocosAux);//hk201808: reformula
		for (int nb = 0; nb < lisBlocosAux->Count; nb++)
		{
			bloco = (VTBloco*)lisBlocosAux->Items[nb];
			bloco->Color = cor;
		}

		iconta++;
		if (iconta > lisAux->Count / 3)
		{
			icontaLim++;
			iconta = 0;
			if (icontaLim == 1)
			{
				cor = cor1;
				valor1 = taxa;
			}
			else
			{
				cor = cor2;
				valor0 = taxa;
			}
		}
	}

	grafico->Show(grafCONFIAB);
	delete lisBlocosAux;
	delete lisAux;
	delete lisAux00;

	AnsiString aux;
	aux.sprintf("Taxa de Falhas (f/km/ano)\n");
	aux.cat_sprintf("Verde\t: %6.1f até %6.1f\n", 0.0, valor0);
	aux.cat_sprintf("Amarelo\t: %6.1f até %6.1f\n", valor0, valor1);
	aux.cat_sprintf("Vermelho\t: %6.1f até %6.1f\n", valor1, valor2);
	Aviso(aux);
	/*
	 //---------------------------------------------------------------------------
	 void __fastcall TFormCalcIndCont::ActionViewCLIExecute(TObject *Sender)
	 {
	 VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	 TList *lisAux, *lisAux00;
	 TListItem *item;
	 VTDefeito *pdef, *pdefMax;//pdef->DCons
	 TColor    cor, cor0, cor1, cor2, corNULL;
	 double    valor0, valor1, valor2;
	 VTBloco   *bloco;
	 bool flag;
	 double dcMax;

	 cor0 = clRed;
	 cor1 = clYellow;
	 cor2 = clGreen;
	 corNULL = clGray;

	 lisAux  = new TList();
	 lisAux00= new TList();

	 for(int n = 0; n < lvResultDefeito->Items->Count; n++)
	 {
	 item = lvResultDefeito->Items->Item[n];
	 pdef = (VTDefeito*)item->Data;
	 if(!IsDoubleZero(pdef->DCons)) continue;
	 lisAux00->Add(pdef);
	 }

	 flag = true;
	 while(flag)
	 {
	 flag = false;
	 dcMax = -1;
	 for(int n = 0; n < lvResultDefeito->Items->Count; n++)
	 {
	 item = lvResultDefeito->Items->Item[n];
	 if(lisAux00->IndexOf(item->Data) != -1) continue;
	 if(lisAux->IndexOf(item->Data) != -1) continue;
	 pdef = (VTDefeito*)item->Data;
	 if(pdef->DCons > dcMax) {pdefMax = pdef;
	 flag = true;
	 }
	 }
	 if(flag) lisAux->Add(pdefMax);
	 }

	 //Blocos com taxa zero
	 for(int n = 0; n < lisAux00->Count; n++)
	 {
	 pdef = (VTDefeito*)lisAux00->Items[n];
	 for(int nb = 0; nb < pdef->LisBlocosAgrega->Count; nb++)
	 {
	 bloco = (VTBloco*)pdef->LisBlocosAgrega->Items[nb];
	 bloco->CorBloco = corNULL;
	 }
	 }

	 int iconta, icontaLim;
	 iconta = 0; icontaLim = 0;
	 cor = cor0;
	 for(int n = 0; n < lisAux->Count; n++)
	 {
	 pdef = (VTDefeito*)lisAux->Items[n];
	 if(n == 0) valor2 = pdef->DCons;
	 for(int nb = 0; nb < pdef->LisBlocosAgrega->Count; nb++)
	 {
	 bloco = (VTBloco*)pdef->LisBlocosAgrega->Items[nb];
	 bloco->CorBloco = cor;
	 }
	 iconta++;
	 if(iconta > lisAux->Count / 3) {icontaLim++;
	 iconta = 0;
	 if(icontaLim == 1) {cor = cor1;
	 valor1 = pdef->DCons;}
	 else {cor = cor2;
	 valor0 = pdef->DCons;}
	 }
	 }

	 grafico->Show(grafCONFIAB);
	 delete lisAux;
	 delete lisAux00;

	 AnsiString aux;
	 aux.sprintf    ("Verde\t: %6.2f até %6.2f\n", 0.0, valor0);
	 aux.cat_sprintf("Amarelo\t: %6.2f até %6.2f\n", valor0, valor1);
	 aux.cat_sprintf("Vermelho\t: %6.2f até %6.2f\n", valor1, valor2);
	 Aviso(aux);
	 }
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::ActionMostraBlocosExecute(TObject *Sender)
{
	VTCalcIndCont *calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	VTBloco *bloco;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	AnsiString className = Sender->ClassName();
	TList *lisBloco, *lisLiga;
	VTChave *pchave;
	int index;

	if (className.AnsiCompare("TStringGrid") != 0)
		return;
	if (((TStringGrid*)Sender)->Name.Compare("sgridBlocosProt") != 0)
		return;
	if ((index = sgridBlocosProt->Row) < 1)
		return;

	if (sgridBlocosProt->Objects[0][index] == NULL)
		return;

	lisBloco = new TList();
	lisLiga = new TList();
	pchave = (VTChave*)sgridBlocosProt->Objects[0][index];
	calcIndCont->BuscaProt->ListaZonaProtJus(pchave, lisBloco);//hk201808: reformula
	for (int nb = 0; nb < lisBloco->Count; nb++)
	{
		bloco = (VTBloco*)lisBloco->Items[nb];
		lisLiga->Assign(bloco->LisLigacao(), laOr);
	}
	grafico->Moldura(pchave);
	if (lisLiga->Count > 0)
		grafico->DestacaEqpto(lisLiga, bloco->Rede->Color, 10);
	else
		grafico->DestacaEqpto(NULL, clBlue, 10);

	delete lisBloco;
	delete lisLiga;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaParamProtJus::FormClose(TObject *Sender, TCloseAction &Action)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->DestacaEqpto(NULL, clBlue, 10);
	grafico->Moldura();
	grafico->Show(grafTOPO);
	Action = caFree;
}

// ---------------------------------------------------------------------------
int __fastcall TFormEditaParamProtJus::NCons(TList *lisCargas)
{
	VTCarga *carga;
	int ncargas;
	int naux;

	ncargas = 0;
	for (int n = 0; n < lisCargas->Count; n++)
	{
		carga = (VTCarga*)lisCargas->Items[n];
		// if((carga->Codigo.Trim()).AnsiCompare("916-C-8285-3-75") == 0)
		// int kkk = 0;
		naux = 0;
		naux += carga->NumConsResidencial;
		naux += carga->NumConsComercial;
		naux += carga->NumConsIndustrial;
		naux += carga->NumConsRural;
		naux += carga->NumConsOutros; // MISTURADO COM IP!!!!
		naux += carga->NumConsA4;

		ncargas += naux;
	}
	if (ncargas == 0)
		ncargas = lisCargas->Count;

	return (ncargas);
}
// ---------------------------------------------------------------------------
