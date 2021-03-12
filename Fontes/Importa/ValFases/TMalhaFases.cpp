//---------------------------------------------------------------------------

#pragma hdrstop

#include "TMalhaFases.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------

#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Radial\VTMalha.h"
#include "..\..\Radial\VTMalhas.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrecho.h"

#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
__fastcall TMalhaFases::TMalhaFases(void)
{
	lisLIGBARRAINI[indFASE_A] = new TList;
	lisLIGBARRAINI[indFASE_B] = new TList;
	lisLIGBARRAINI[indFASE_C] = new TList;
	lisLIGBARRAINI[indNEUTRO] = new TList;

	lisBARRA[indFASE_A] = new TList;
	lisBARRA[indFASE_B] = new TList;
	lisBARRA[indFASE_C] = new TList;
	lisBARRA[indNEUTRO] = new TList;
}
// ---------------------------------------------------------------------------
__fastcall TMalhaFases::~TMalhaFases(void)
{
	DestroiTObject(lisLIGBARRAINI[indFASE_A]);
	DestroiTObject(lisLIGBARRAINI[indFASE_B]);
	DestroiTObject(lisLIGBARRAINI[indFASE_C]);
	DestroiTObject(lisLIGBARRAINI[indNEUTRO]);

	DestroiTObject(lisBARRA[indFASE_A]);
	DestroiTObject(lisBARRA[indFASE_B]);
	DestroiTObject(lisBARRA[indFASE_C]);
	DestroiTObject(lisBARRA[indNEUTRO]);
}
// ---------------------------------------------------------------------------
void __fastcall  TMalhaFases::Executa(VTBarra *barraInicial, VTMalha *malha)
{
	barraInicio = barraInicial;
    this->malha = malha;
	for (int nf = indFASE_A; nf < MAX_FASE; nf++)
	{
    	Executa(nf);
	}
}
// ---------------------------------------------------------------------------
void __fastcall  TMalhaFases::Executa(int indFase)
{
	// variáveis locais
	VTBarra *barra, *barraPai;
	VTChave *chave;
	VTLigacao *ligacao_filha, *ligacao_pai;
	TList *lisEXT_BAR = new TList;
	TList *lisLigacoesRede = new TList;

	//guarda na lista de ligacoes rede as ligacoes da malha
	lisLigacoesRede->Assign(malha->LisLigacao());
	// inicia lisLIGBARRAINI com as Ligacoes que conectam a Barra inicial
	lisLIGBARRAINI[indFase]->Clear();
	//adiciona o nó pai
	lisEXT_BAR->Add(barraInicio);
//	lisBARRA[indFase]->Add(barraInicio);
	//inicializa as ligacoes que serão analisadas
	for (int nb = 0; nb < lisEXT_BAR->Count; nb++)
	{
		barra = (VTBarra*) lisEXT_BAR->Items[nb];
		for (int nl = lisLigacoesRede->Count - 1; nl >= 0; nl--)
		{
			ligacao_filha = (VTLigacao*)lisLigacoesRede->Items[nl];
			// verifica se é uma Chave aberta
			if (ligacao_filha->Tipo() == eqptoCHAVE)
			{
				chave = (VTChave*)ligacao_filha;
				if (chave->Aberta)
				{ // retira Ligacao de lisEQP
					lisLigacoesRede->Delete(nl);
					continue;
				}
			}
			// verifica se a Ligacao conecta a Barra
			if (ligacao_filha->ConectaBarra(barra))
			{
				if (LigacaoPermiteFase(ligacao_filha,indFase,barra))
				{
					lisLIGBARRAINI[indFase]->Add(ligacao_filha);
				}
			}
		}
	}

	// loop p/ todas Ligacoes pai que estão em lisEXT_LIG
	for (int nlp = 0; nlp < lisLIGBARRAINI[indFase]->Count; nlp++)
	{
		ligacao_pai = (VTLigacao*)lisLIGBARRAINI[indFase]->Items[nlp];
		//remove a ligacao da lista de ligacoes disponiveis
		lisLigacoesRede->Remove(ligacao_pai);
		// loop p/ todas Barras da Ligacao pai
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao_pai->Barra(ind_bar)) == NULL)
				continue;
			// descarta Barras já analisadas
			if (lisEXT_BAR->IndexOf(barra) >= 0)
				{continue;}
			// insere Barra em lisEXT_BAR
			lisEXT_BAR->Add(barra);
			// determina Ligacoes em lisLigacoesRede que se conectam à Barra
			for (int nl = (lisLigacoesRede->Count - 1); nl >= 0; nl--)
			{ // OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
				ligacao_filha = (VTLigacao*)lisLigacoesRede->Items[nl];
				// verifica se a Ligacao conecta a Barra
				if (ligacao_filha->ConectaBarra(barra))
				{
					if (LigacaoPermiteFase(ligacao_filha,indFase,barra))
					{
						lisLIGBARRAINI[indFase]->Add(ligacao_filha);
//						lisBARRA[indFase]->Add(barra);
					}
				}
			}
		}
	}
	//lista todas barras usadas pelas ligacoes que permitem caminho
	for (int nlp = 0; nlp < lisLIGBARRAINI[indFase]->Count; nlp++)
	{
		ligacao_filha = (VTLigacao*)lisLIGBARRAINI[indFase]->Items[nlp];
		// loop p/ todas Barras da Ligacao pai
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao_filha->Barra(ind_bar)) == NULL)
				continue;
			else
			{
            	lisBARRA[indFase]->Add(barra);
            }
        }
	}
	DestroiTObject(lisEXT_BAR);
	DestroiTObject(lisLigacoesRede);
}
// ---------------------------------------------------------------------------
bool __fastcall  TMalhaFases::ExisteCaminho(VTBarra *barra, int indFase)
{
	bool existeBarraIni, existeBarra;

	existeBarra = (lisBARRA[indFase]->IndexOf(barra) >=0);
	existeBarraIni = (lisBARRA[indFase]->IndexOf(barraInicio) >=0);

	return (existeBarraIni && existeBarra);
}
// ---------------------------------------------------------------------------
int __fastcall  TMalhaFases::FaseIndToFaseTag(int indFase)
{
	if (indFase == indFASE_A)
		{return faseA;}
	else if (indFase == indFASE_B)
		{return faseB;}
	else if (indFase == indFASE_C)
		{return faseC;}
	else if (indFase == indNEUTRO)
		{return faseN;}
	return faseINV;
}
// ---------------------------------------------------------------------------
bool __fastcall  TMalhaFases::LigacaoPermiteFase(VTLigacao *lig, int indFase, VTBarra *ref)
{
	int tipo;
	VTChave *chave;
	bool permite = true;
	int faseLig =0;
	int faseTagRequerida =0;

	tipo = lig->Tipo();
    faseTagRequerida = FaseIndToFaseTag(indFase);
	switch (tipo)
	{
		case eqptoCHAVE:  //so nao permite se estiver aberta
			chave = (VTChave*) lig;
			if (chave->Aberta)
				{permite = false;}
			break;
		case eqptoTRAFO:
		case eqptoTRECHO:
        case eqptoREGULADOR:
		default:
			faseLig = lig->Fases(ref);
			permite = ((faseLig & faseTagRequerida) == faseTagRequerida);
			break;
	}

	return permite;
}
// ---------------------------------------------------------------------------
