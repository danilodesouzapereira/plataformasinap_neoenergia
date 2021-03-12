// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TAvaliaAcoesTrafo.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesTrafo(VTApl *apl)
{
	try
	{
		return (new TAvaliaAcoesTrafo(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesTrafo::TAvaliaAcoesTrafo(VTApl *apl):VTAvaliaAcoesEqpto(apl)
{

}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesTrafo::~TAvaliaAcoesTrafo(void)
{

}

// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrafo::DeterminaTipoSemelhanca(VTAcao *acao)
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
bool __fastcall TAvaliaAcoesTrafo::MesmoAjuste(VTTrafo *trafo1, VTTrafo *trafo2)
{
	if (trafo1 == NULL)
		{return false;}
	if (trafo2 == NULL)
		{return false;}

	//verifica primario
	//verifica se possui mesma tap no primario
	if (!IsDoubleZero(trafo1->pri.tap - trafo2->pri.tap))
		{return false;}

	//verifica secundario
	//verifica se possui mesma tap no secundario
	if (!IsDoubleZero(trafo1->sec.tap - trafo2->sec.tap))
		{return false;}

	//verifica ltc
	if (trafo1->ltc.tipo != trafo2->ltc.tipo)   //isso eh ajuste?
		{return false;}
	if (!IsDoubleZero(trafo1->ltc.dv_perc - trafo2->ltc.dv_perc))
		{return false;}
	if (trafo1->ltc.num_passo != trafo2->ltc.num_passo)   //isso eh ajuste?
		{return false;}
	if (trafo1->ltc.ajuste_fixo.passo != trafo2->ltc.ajuste_fixo.passo)
		{return false;}
	if (trafo1->ltc.ajuste_auto.bar_ref != trafo2->ltc.ajuste_auto.bar_ref)
		{return false;}
	if (!IsDoubleZero(trafo1->ltc.ajuste_auto.v_pu  - trafo2->ltc.ajuste_auto.v_pu))
		{return false;}
	if (!IsDoubleZero(trafo1->ltc.ajuste_auto.dv_pu  - trafo2->ltc.ajuste_auto.dv_pu))
		{return false;}

	//se tiver o mesmo numero de fases, mas fases diferentes nao tem o mesmo ajuste
	if (MesmoNumFases(trafo1, trafo2))
	{
		if (trafo1->pri.fase != trafo2->pri.fase)
			{return false;}
		if (trafo1->sec.fase != trafo2->sec.fase)
			{return false;}
	}
	else
	{
        return false;
    }

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrafo::MesmoEqpto(VTTrafo *trafo1, VTTrafo *trafo2)
{
//	bool iguais = false;


	if (trafo1 == NULL)
		{return false;}
	if (trafo2 == NULL)
		{return false;}

	//reducao
	if (trafo1->reducao != trafo2->reducao)
		{return false;}
	//autotrafo
	if (trafo1->auto_trafo != trafo2->auto_trafo)
		{return false;}
	//verifica se ambos trafos possuem a mesma potencia
	if (!IsDoubleZero(trafo1->snom - trafo2->snom))
		{return false;}
	//perda no ferro
	if (!IsDoubleZero(trafo1->perda_ferro - trafo2->perda_ferro))
		{return false;}
	//verifica primario
	//verifica se possui mesma ligacao no primario
	if (trafo1->pri.ligacao != trafo2->pri.ligacao )
		{return false;}
	//verifica se possui mesma vnom no primario
	if (!IsDoubleZero(trafo1->pri.vnom - trafo2->pri.vnom))
		{return false;}
	//verifica se possui mesma defasagem no primario
	if (!IsDoubleZero(trafo1->pri.defasagem - trafo2->pri.defasagem))
		{return false;}
	//verifica secundario
	//verifica se possui mesma ligacao no secundario
	if (trafo1->sec.ligacao != trafo2->sec.ligacao )
		{return false;}
	//verifica se possui mesma vnom no secundario
	if (!IsDoubleZero(trafo1->sec.vnom - trafo2->sec.vnom))
		{return false;}
	//verifica se possui mesma defasagem no secundario
	if (!IsDoubleZero(trafo1->sec.defasagem - trafo2->sec.defasagem))
		{return false;}

	//se nao tiver o mesmo numero de fases
	if (!MesmoNumFases(trafo1, trafo2))
		{return false;}

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrafo::MesmoNumFases(VTTrafo *trafo1, VTTrafo *trafo2)
{
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	int numFase1_sec, numFase2_sec;
	int numFase1_pri, numFase2_pri;

	//protecao
	if ((trafo1 == NULL) || (trafo2 == NULL))
		{return false;}
	if (fases == NULL)
		{return false;}
	//verifica
	numFase1_pri = fases->NumeroDeFases(trafo1->pri.fase);
	numFase1_sec = fases->NumeroDeFases(trafo1->sec.fase);
	numFase2_pri = fases->NumeroDeFases(trafo2->pri.fase);
	numFase2_sec = fases->NumeroDeFases(trafo2->sec.fase);

	//verifica num sentido
	if ((numFase1_pri == numFase2_pri) && (numFase1_sec == numFase2_sec))
	{   //exemplo AB/ABC com BC/ABC
		{return true;}
	}
	//verifica no sentido contrario
    if ((numFase1_pri == numFase2_sec) && (numFase1_sec == numFase2_pri))
	{   //exemplo  AB/ABC com ABC/BC
		{return true;}
	}

	return false;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesTrafo::MesmoNumUnidades(VTTrafo *trafo1, VTTrafo *trafo2)
{
	//somente verifica os grupos de unidades monofasicas
	if ((trafo1->Ligacao == lgDELTA_ABERTO)&&(trafo2->Ligacao== lgDELTA_ABERTO))
	{
		return true;
	}
	if ((trafo1->Ligacao == lgDELTA_FECHADO)&&(trafo2->Ligacao == lgDELTA_FECHADO))
	{
		return true;
	}
	if ((trafo1->Ligacao == lgMONOFASICO)&&(trafo2->Ligacao == lgMONOFASICO))
	{
		return true;
	}
	//nenhuma dessas combinacoes
	return false;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrafo::NomeTipoAcao(VTAcao *acao)
{
	AnsiString tipoAcao = "";
	VTEqpto *eqpto = NULL;

	if (ValidaTipoEqpto(acao, eqptoTRAFO))
	{
		eqpto = DeterminaEqpto(acao);
		//verifica o tipo de acao
		switch (acao->Tipo())
		{
			case acaoINSERE:
				tipoAcao = tipoAaINSERIR;
				//adiciona a potencia do inserido
				tipoAcao += " "+ StringPotencia((VTTrafo*)eqpto);
				break;
			case acaoALTERA:
				switch (TipoAlteracao(acao))
				{   //verifica se eh troca de ajuste
					case ttarAJUSTE:
						tipoAcao = tipoAaAJUSTE;
						break;
					//verifica se eh substituicao
					case ttarSUBST:
						tipoAcao = tipoAaSUBST;
                        //adiciona a potencia do inserido
                		tipoAcao += " "+ StringPotencia((VTTrafo*)eqpto);
						break;
					default:
                        tipoAcao = "Outras: ";
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
int __fastcall TAvaliaAcoesTrafo::TipoAlteracao(VTAcao *acao)
{
	VTTrafo *trafoAntes, *trafoDepois;
	int tipoAlt = ttarINDEF;

	//protecao
	if (acao == NULL)
		{return tipoAlt;}
	//julga
	if (ValidaTipoEqpto(acao, eqptoTRAFO))
	{
		//aqui julga diferenças entre faseamento e recondut
		trafoAntes = (VTTrafo*)acao->CopiaDoEqptoOriginal();
		trafoDepois = (VTTrafo*)acao->CopiaDoEqptoAlterado();
		//verifica se trocou o equipamento
		if (MesmoEqpto(trafoAntes,trafoDepois))
		{
			//mesmo eqpto, verifica se mudou de lugar
			if (MesmoLocal(trafoAntes,trafoDepois))
			{
				//verifica se mudou ajuste
				if (MesmoAjuste(trafoAntes,trafoDepois))
				{
					//mudou nada...
                    tipoAlt = ttarINDEF;
				}
				else
				{  //mudou ajuste
					tipoAlt = ttarAJUSTE;
                }
			}
			else
			{   //mudou de lugar, substituicao
				tipoAlt = ttarSUBST;
			}
		}
		else
		{   //trocou o eqpto
			tipoAlt = ttarSUBST;
		}
	}

	return tipoAlt;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesTrafo::StringPotencia(VTTrafo *trafo)
{
	AnsiString potencia = "";

	if (trafo == NULL)
	{
		return potencia;
	}
	//tenta converter
	try
	{
		//verifica se é nulo
		if (!IsDoubleZero(trafo->snom))
		{
            //verifica a ordem de grandeza
			if (trafo->snom < 1.0)     //mva
			{
				potencia = DoubleToStr("%1.0f",trafo->snom*1000) + " kVA";
			}
			else
			{
				potencia = DoubleToStr("%1.0f",trafo->snom) + " MVA";
			}
		}
	}
	catch(...)
	{

	}

	return potencia;

}
// ---------------------------------------------------------------------------
// eof
