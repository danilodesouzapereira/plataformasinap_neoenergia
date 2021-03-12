// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TAvaliaAcoesTrecho.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesTrecho(VTApl *apl)
{
	try
	{
		return (new TAvaliaAcoesTrecho(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesTrecho::TAvaliaAcoesTrecho(VTApl *apl):VTAvaliaAcoesEqpto(apl)
{

}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesTrecho::~TAvaliaAcoesTrecho(void)
{

}

//// ---------------------------------------------------------------------------
//VTArranjo* __fastcall TAvaliaAcoesTrecho::ArranjoFinal(VTAcao *acao)
//{
//	VTArranjo *arranjoAntes, *arranjoDepois;
//	VTTrecho *trechoAntes, *trechoDepois;
//	VTArranjo *arranjo;
//
//	arranjoAntes = arranjoDepois = NULL;
//	//protecao
//	if (acao == NULL)
//		{return NULL;}
//	//julga
//	if (ValidaTipoEqpto(acao, eqptoTRECHO))
//	{
//		trechoAntes = (VTTrecho*)acao->CopiaDoEqptoOriginal();
//		trechoDepois = (VTTrecho*)acao->CopiaDoEqptoAlterado();
//		if (trechoAntes != NULL)
//		{
//			arranjoAntes = trechoAntes->arranjo;
//		}
//		if (trechoDepois != NULL)
//		{
//			arranjoDepois = trechoDepois->arranjo;
//		}
//	}
//	//primeiro preenche com original
//	if (arranjoAntes != NULL)
//	{
//		arranjo = arranjoAntes;
//	}
//	//caso exista o alterado troca pelo alterado
//	if (trechoDepois != NULL)
//	{
//		arranjo = arranjoDepois;
//	}
//
//	return arranjo;
//}// ---------------------------------------------------------------------------
VTArranjo* __fastcall TAvaliaAcoesTrecho::ArranjoFinal(VTAcao *acao)
{
	VTArranjo *arranjoAntes, *arranjoDepois;
	VTTrecho *trechoAntes, *trechoDepois;
	VTArranjo *arranjo = NULL;

	arranjoAntes = arranjoDepois = NULL;
	//protecao
	if (acao == NULL)
		{return NULL;}
	//julga
	if (ValidaTipoEqpto(acao, eqptoTRECHO))
	{
		//se acao for de insercao
		if ((acao->Tipo() == acaoINSERE) || (acao->Tipo() == acaoRETIRA))
		{
			trechoDepois = (VTTrecho*) acao->Eqpto();
		}
		else if(acao->Tipo() == acaoALTERA)
		{
			trechoDepois = (VTTrecho*) acao->CopiaDoEqptoAlterado();
		}

        if (trechoDepois != NULL)
		{
			arranjoDepois = trechoDepois->arranjo;
		}
	}
	if (arranjoDepois != NULL)
	{
		arranjo = arranjoDepois;
	}

	return arranjo;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrecho::DeterminaTipoSemelhanca(VTAcao *acao)
{
	AnsiString tipoSemelhanca;
//	VTArranjo *arranjoFinal;

	tipoSemelhanca = NomeTipoAcao(acao);
//	arranjoFinal =  ArranjoFinal(acao);
	//concatena o nome do arranjo ao nome da acao
//	tipoSemelhanca += NomeArranjo(arranjoFinal);

	return tipoSemelhanca;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrecho::MesmoArranjo(VTTrecho *trecho1, VTTrecho *trecho2)
{
	VTArranjo *arranjo1, *arranjo2;

	arranjo1 = trecho1->arranjo;
	arranjo2 = trecho2->arranjo;
	//protecao
	if (arranjo1 == NULL)
		{return false;}
	if (arranjo2 == NULL)
		{return false;}
	//se for o mesmo mesmo
	if (arranjo1 == arranjo2)
		{return true;}    //São IGUAIS!!
	//ve se ambos sao do mesmo tipo
	if (arranjo1->TipoCfg != arranjo2->TipoCfg)
		{return false;}
	//compara capacidade
	if (!IsDoubleZero(arranjo1->Iadm_amp - arranjo2->Iadm_amp))
		{return false;}
	//se tiver o mesmo nome (tirando as info de fase)
	if (TrataNomeArranjo(arranjo1).AnsiCompare(TrataNomeArranjo(arranjo2))== 0 )
		{return true;}   //São IGUAIS!!
	//compara impedancia
	if (!IsDoubleZero(arranjo1->z0.r - arranjo2->z0.r))
		{return false;}
	if (!IsDoubleZero(arranjo1->z0.x - arranjo2->z0.x))
		{return false;}
	if (!IsDoubleZero(arranjo1->z1.r - arranjo2->z1.r))
		{return false;}
	if (!IsDoubleZero(arranjo1->z1.x - arranjo2->z1.x))
		{return false;}

	//não encontrou diferenças
	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrecho::MesmoComprimento(VTTrecho *trecho1, VTTrecho *trecho2)
{
	int fase1, fase2;

	//protecao
	if ((trecho1 == NULL) || (trecho2 == NULL))
		{return false;}
	//verifica
	return (IsDoubleZero(trecho1->Comprimento_m - trecho2->Comprimento_m, 1e-2));
}

// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrecho::MesmaFase(VTTrecho *trecho1, VTTrecho *trecho2)
{
	int fase1, fase2;

	//protecao
	if ((trecho1 == NULL) || (trecho2 == NULL))
		{return false;}
	if ((trecho1 != NULL) && (trecho1->arranjo == NULL))
		{return false;}
	if ((trecho2 != NULL) && (trecho2->arranjo == NULL))
		{return false;}
	//verifica
	fase1 = trecho1->arranjo->Fases;
	fase2 = trecho2->arranjo->Fases;

	if (fase1 != fase2)
		{return false;}

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrecho::MesmoNumFase(VTTrecho *trecho1, VTTrecho *trecho2)
{
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	int numFase1, numFase2;

	//protecao
	if ((trecho1 == NULL) || (trecho2 == NULL))
		{return false;}
	if ((trecho1 != NULL) && (trecho1->arranjo == NULL))
		{return false;}
	if ((trecho2 != NULL) && (trecho2->arranjo == NULL))
		{return false;}
	if (fases == NULL)
		{return false;}
	//verifica
	numFase1 = fases->NumeroDeFases(trecho1->arranjo->Fases);
	numFase2 = fases->NumeroDeFases(trecho2->arranjo->Fases);

	if (numFase1 != numFase2)
		{return false;}

    return true;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrecho::NomeArranjo(VTArranjo *arranjo)
{
    AnsiString cod2 = "";

	if (arranjo == NULL)
		{return cod2;}

	cod2 = arranjo->Codigo;
    cod2 = cod2.UpperCase();
	cod2 = StringReplace(cod2, "ABCN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "ABC", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "ABN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "BCN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "CAN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "ACN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*AB", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*BC", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*CA", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*AC", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*AN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*BN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*CN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*AT", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*BT", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*CT", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*A", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*B", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "*C", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_AB", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_BC", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_CA", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_AC", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_AN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_BN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_CN", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_AT", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_BT", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_CT", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_A", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_B", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_C", "", TReplaceFlags() << rfReplaceAll);

	return cod2;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrecho::NomeRefaseamento(VTAcao *acao)
{
	AnsiString faseamento ="";
	VTTrecho *trechoAntes, *trechoDepois;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTArranjo *arranjoAntes, *arranjoDepois;

    //protecao
	if (acao == NULL)
		{return faseamento;}

	trechoAntes = (VTTrecho*)acao->Eqpto();
	trechoDepois = (VTTrecho*)acao->CopiaDoEqptoAlterado();

	if ((trechoAntes == NULL) || (trechoDepois == NULL))
		{return faseamento;}

	//
	arranjoAntes = trechoAntes->arranjo;
	arranjoDepois = trechoDepois->arranjo;
	if ((arranjoAntes == NULL) || (arranjoDepois == NULL))
		{return faseamento;}

	faseamento = " " + fases->AsStrABCN[arranjoAntes->Fases];
	faseamento += " -> " +fases->AsStrABCN[arranjoDepois->Fases];

	return faseamento;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16531557
AnsiString __fastcall TAvaliaAcoesTrecho::NomeTipoAcao(VTAcao *acao)
{
	AnsiString tipoAcao = "";
//	VTTrecho *trechoAntes, *trechoDepois;
	VTArranjo *arranjoFinal = NULL;

	if (ValidaTipoEqpto(acao, eqptoTRECHO))
	{
		arranjoFinal =  ArranjoFinal(acao);
		//verifica o tipo de acao
		switch (acao->Tipo())
		{
			case acaoINSERE:
				tipoAcao = tipoAaINSERIR;
                //concatena o nome do arranjo ao nome da acao
				tipoAcao += NomeArranjo(arranjoFinal);
				break;
			case acaoALTERA:
				switch (TipoTrocaDeArranjo(acao))
				{   //verifica se eh recondutoramento
					case ttatRECOND:
						tipoAcao = tipoAaRECONDUT;
						//concatena o nome do arranjo ao nome da acao
						tipoAcao += NomeArranjo(arranjoFinal);
						break;
					//verifica se eh refaseamento
					case ttatGIROFASE:
						tipoAcao = tipoAaREFASEAM;
						//concatena o nome do arranjo ao nome da acao
						tipoAcao += NomeArranjo(arranjoFinal);
						//concatena a mudanca de fase
						tipoAcao +=  NomeRefaseamento(acao);
						break;
					//verifica se eh insercao
					case ttatINSERIR:
						tipoAcao = tipoAaINSERIR;
                        //concatena o nome do arranjo ao nome da acao
						tipoAcao += NomeArranjo(arranjoFinal);
						break;
					default:
                        tipoAcao = "Indefinido";
						break;
                    ;
				}
				break;
			case acaoRETIRA:
				tipoAcao = tipoAaRETIRAR;
				break;
			default:
                tipoAcao = "Outras: "; //teste
				break;
			;
		}
	}

	return tipoAcao;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrecho::TrataNomeArranjo(VTArranjo *arranjo)
{
	AnsiString cod2 = "";

	if (arranjo == NULL)
		{return cod2;}

	cod2 = NomeArranjo(arranjo);
    //remove umas sujeiras
	cod2 = StringReplace(cod2, "*", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, " ", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_", "", TReplaceFlags() << rfReplaceAll);

	return cod2;
}
// ---------------------------------------------------------------------------
int __fastcall TAvaliaAcoesTrecho::TipoTrocaDeArranjo(VTAcao *acao)
{
	VTArranjo *arranjoAntes, *arranjoDepois;
	VTTrecho *trechoAntes, *trechoDepois;
	int tipoTroca = ttatINDEF;

	//protecao
	if (acao == NULL)
		{return tipoTroca;}
	//julga
	if (ValidaTipoEqpto(acao, eqptoTRECHO))
	{
		//aqui julga diferenças entre faseamento e recondut
		trechoAntes = (VTTrecho*)acao->CopiaDoEqptoOriginal();
		trechoDepois = (VTTrecho*)acao->CopiaDoEqptoAlterado();
		//se não possuir mesmo arranjo, é recondutoramento
		if (!MesmoArranjo(trechoAntes,trechoDepois))
		{
			tipoTroca = ttatRECOND;
		}
		else
		{//pode ser q seja só um refaseamento
			if(!MesmaFase(trechoAntes,trechoDepois))
			{//nao possuindo a mesma fase pode ser q seja só um giro
				if(MesmoNumFase(trechoAntes,trechoDepois))
				{
					tipoTroca = ttatGIROFASE;
				}
				else
				{ //considera que inseriu novos cabos
					tipoTroca = ttatINSERIR; //aqui se diminui um numero de fases , podia ser uma remocao...
                }
			}
			else
			{   //tem mesmo arranjo e fases iguais... acho q nao mudou coisa relevante?
				//verifica se mudou o comprimento
				if (!MesmoComprimento(trechoAntes, trechoDepois))
				{   //inseriu mais cabos
					tipoTroca = ttatINSERIR;
				}
				else
				{
					tipoTroca = ttatINDEF;
                }
            }
		}
	}

	return tipoTroca;
}
// ---------------------------------------------------------------------------
// eof
