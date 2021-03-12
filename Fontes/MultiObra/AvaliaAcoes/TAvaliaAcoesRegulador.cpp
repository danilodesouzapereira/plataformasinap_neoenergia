// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TAvaliaAcoesRegulador.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTRegulador.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesRegulador(VTApl *apl)
{
	try
	{
		return (new TAvaliaAcoesRegulador(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesRegulador::TAvaliaAcoesRegulador(VTApl *apl):VTAvaliaAcoesEqpto(apl)
{

}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesRegulador::~TAvaliaAcoesRegulador(void)
{

}

// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRegulador::DeterminaTipoSemelhanca(VTAcao *acao)
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
bool __fastcall TAvaliaAcoesRegulador::MesmoAjuste(VTRegulador *reg1, VTRegulador *reg2)
{
	if (reg1 == NULL)
		{return false;}
	if (reg2 == NULL)
		{return false;}

	//verifica config
	if (reg1->by_pass != reg2->by_pass)
		{return false;}
	if (!IsDoubleZero(reg1->var_tensao - reg2->var_tensao))
		{return false;}
	if (reg1->rel_tap != reg2->rel_tap)
		{return false;}
	//barra de ref
	if (reg1->sentido.bar_ref != reg2->sentido.bar_ref )
		{return false;}
	//passo do fixo
	if (reg1->cfg_fixo.passo != reg2->cfg_fixo.passo)
		{return false;}
	//config auto
	if (reg1->cfg_auto.bar_ref != reg2->cfg_auto.bar_ref)
		{return false;}
    if (reg1->cfg_auto.bar_ref_inv != reg2->cfg_auto.bar_ref_inv)
		{return false;}
    if (!IsDoubleZero(reg1->cfg_auto.v_pu - reg2->cfg_auto.v_pu))
		{return false;}
	if (!IsDoubleZero(reg1->cfg_auto.vinv_pu - reg2->cfg_auto.vinv_pu))
		{return false;}
	if (!IsDoubleZero(reg1->cfg_auto.dv_pu - reg2->cfg_auto.dv_pu))
		{return false;}
	if (!IsDoubleZero(reg1->cfg_auto.larg_bandamorta_pu - reg2->cfg_auto.larg_bandamorta_pu))
		{return false;}
	if (!IsDoubleZero(reg1->cfg_auto.larg_bandamorta_inv_pu - reg2->cfg_auto.larg_bandamorta_inv_pu))
		{return false;}
	
	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesRegulador::MesmoEqpto(VTRegulador *reg1, VTRegulador *reg2)
{
//	bool iguais = false;


	if (reg1 == NULL)
		{return false;}
	if (reg2 == NULL)
		{return false;}
	//verifica se tem o mesmo modelo
	if (reg1->modelo != reg2->modelo)
		{return false;}
	if (reg1->ligacao != reg2->ligacao)
	{
		//verifica se manteve o numero de unidades
		if (!MesmoNumUnidades(reg1, reg2))
			{return false;}
	}
	//verifica se ambos reguladores possuem a mesma potencia
	if (!IsDoubleZero(reg1->snom - reg2->snom))
		{return false;}
	//verifica o numero de passos (?)
	if (reg1->num_passo != reg2->num_passo)
		{return false;}

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesRegulador::MesmoNumUnidades(VTRegulador *reg1, VTRegulador *reg2)
{
	//verifica se 1 eh delta e o outro estrela
	if ((reg1->ligacao == lgDELTA_FECHADO)&&(reg2->ligacao == lgEST_ISO))
	{
		return true;
	}
	//verifica se 1 eh delta e o outro estrela
	if ((reg1->ligacao == lgDELTA_FECHADO)&&(reg2->ligacao == lgEST_AT))
	{
		return true;
	}
	if ((reg1->ligacao == lgTRIANG)&&(reg2->ligacao == lgEST_ISO))
	{
		return true;
	}
	//verifica se 1 eh delta e o outro estrela
	if ((reg1->ligacao == lgTRIANG)&&(reg2->ligacao == lgEST_AT))
	{
		return true;
	}
	//verifica o contrario
	//verifica se 1 eh delta e o outro estrela
	if ((reg2->ligacao == lgDELTA_FECHADO)&&(reg1->ligacao == lgEST_ISO))
	{
		return true;
	}
	//verifica se 1 eh delta e o outro estrela
	if ((reg2->ligacao == lgDELTA_FECHADO)&&(reg1->ligacao == lgEST_AT))
	{
		return true;
	}
	if ((reg2->ligacao == lgTRIANG)&&(reg1->ligacao == lgEST_ISO))
	{
		return true;
	}
	//verifica se 1 eh delta e o outro estrela
	if ((reg2->ligacao == lgTRIANG)&&(reg1->ligacao == lgEST_AT))
	{
		return true;
	}

	//nenhuma dessas combinacoes
    return false;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRegulador::NomeTipoAcao(VTAcao *acao)
{
	AnsiString tipoAcao = "";
	VTRegulador *reguladorAntes, *reguladorDepois;
	VTEqpto *eqpto = NULL;

	if (ValidaTipoEqpto(acao, eqptoREGULADOR))
	{
		eqpto = DeterminaEqpto(acao);
		//verifica o tipo de acao
		switch (acao->Tipo())
		{
			case acaoINSERE:
				tipoAcao = tipoAaINSERIR;
				//adiciona a potencia do inserido
				tipoAcao += " "+ StringPotencia((VTRegulador*)eqpto);
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
                		tipoAcao += " "+ StringPotencia((VTRegulador*)eqpto);
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
int __fastcall TAvaliaAcoesRegulador::TipoAlteracao(VTAcao *acao)
{
	VTRegulador *regAntes, *regDepois;
	int tipoAlt = ttarINDEF;

	//protecao
	if (acao == NULL)
		{return tipoAlt;}
	//julga
	if (ValidaTipoEqpto(acao, eqptoREGULADOR))
	{
		//aqui julga diferenças entre faseamento e recondut
		regAntes = (VTRegulador*)acao->CopiaDoEqptoOriginal();
		regDepois = (VTRegulador*)acao->CopiaDoEqptoAlterado();
		//verifica se trocou o equipamento
		if (MesmoEqpto(regAntes,regDepois))
		{
			//mesmo eqpto, verifica se mudou de lugar
			if (MesmoLocal(regAntes,regDepois))
			{
				//verifica se mudou ajuste
				if (MesmoAjuste(regAntes,regDepois))
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
AnsiString __fastcall TAvaliaAcoesRegulador::StringPotencia(VTRegulador *regulador)
{
	AnsiString potencia = "";

	if (regulador == NULL)
	{
		return potencia;
	}
	//tenta converter
	try
	{
		//verifica a ordem de grandeza
		if (regulador->snom < 1.0)     //mva
		{
			potencia = DoubleToStr("%1.0f",regulador->snom*1000) + " kVA";
		}
		else
		{
            potencia = DoubleToStr("%1.0f",regulador->snom) + " MVA";
        }
	}
	catch(...)
	{

	}

	return potencia;

}
// ---------------------------------------------------------------------------
// eof
