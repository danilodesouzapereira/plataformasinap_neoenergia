// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TAvaliaAcoesCapacitor.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTCapacitor.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesCapacitor(VTApl *apl)
{
	try
	{
		return (new TAvaliaAcoesCapacitor(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesCapacitor::TAvaliaAcoesCapacitor(VTApl *apl):VTAvaliaAcoesEqpto(apl)
{

}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesCapacitor::~TAvaliaAcoesCapacitor(void)
{

}

// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesCapacitor::DeterminaTipoSemelhanca(VTAcao *acao)
{
	AnsiString tipoSemelhanca;
	VTArranjo *arranjoFinal;

	tipoSemelhanca = NomeTipoAcao(acao);

	return tipoSemelhanca;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesCapacitor::MesmoAjuste(VTCapacitor *cap1, VTCapacitor *cap2)
{
	if (cap1 == NULL)
		{return false;}
	if (cap2 == NULL)
		{return false;}

	//verifica curva

	if (MesmaCurva(cap1->curva, cap2->curva))
		{return false;}

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoesCapacitor::MesmoEqpto(VTCapacitor *cap1, VTCapacitor *cap2)
{
 	if (cap1 == NULL)
		{return false;}
	if (cap2 == NULL)
		{return false;}
	//verifica se tem o mesma ligacao
	if (cap1->ligacao != cap2->ligacao)
		{return false;}
	//verifica se ambos capacitores possuem a mesma potencia
	if (!IsDoubleZero(cap1->q - cap2->q))
		{return false;}

	return true;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesCapacitor::NomeTipoAcao(VTAcao *acao)
{
	AnsiString tipoAcao = "";
	VTCapacitor *capAntes, *capDepois;

	VTEqpto *eqpto = NULL;

	if (ValidaTipoEqpto(acao, eqptoCAPACITOR))
	{
		eqpto = DeterminaEqpto(acao);
		//verifica o tipo de acao
		switch (acao->Tipo())
		{
			case acaoINSERE:
				tipoAcao = tipoAaINSERIR;
				//adiciona a potencia do inserido
				tipoAcao += " "+ StringPotencia((VTCapacitor*)eqpto);
				break;
			case acaoALTERA:
				switch (TipoAlteracao(acao))
				{   //verifica se eh troca de ajuste
					case ttatAJUSTE:
						tipoAcao = tipoAaAJUSTE;
						break;
					//verifica se eh substituicao
					case ttatSUBST:
						tipoAcao = tipoAaSUBST;
						//adiciona a potencia do inserido
						tipoAcao += " "+ StringPotencia((VTCapacitor*)eqpto);
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
AnsiString __fastcall TAvaliaAcoesCapacitor::StringPotencia(VTCapacitor *cap)
{
	AnsiString potencia = "";

	if (cap == NULL)
	{
		return potencia;
	}
	//tenta converter
	try
	{
		//verifica a ordem de grandeza
		if (cap->q < 1.0)     //mva
		{
			potencia = DoubleToStr("%1.0f",cap->q*1000) + " kvar";
		}
		else
		{
			potencia = DoubleToStr("%1.0f",cap->q) + " Mvar";
		}
	}
	catch(...)
	{

	}

	return potencia;

}
// ---------------------------------------------------------------------------
int __fastcall TAvaliaAcoesCapacitor::TipoAlteracao(VTAcao *acao)
{
	VTCapacitor *capAntes, *capDepois;
	int tipoAlt = ttatINDEF;

	//protecao
	if (acao == NULL)
		{return tipoAlt;}
	//julga
	if (ValidaTipoEqpto(acao, eqptoCAPACITOR))
	{
		//aqui julga diferenças entre faseamento e recondut
		capAntes = (VTCapacitor*)acao->CopiaDoEqptoOriginal();
		capDepois = (VTCapacitor*)acao->CopiaDoEqptoAlterado();
		//verifica se trocou o equipamento
		if (MesmoEqpto(capAntes,capDepois))
		{
			//mesmo eqpto, verifica se mudou de lugar
			if (MesmoLocal(capAntes,capDepois))
			{
				//verifica se mudou ajuste
				if (MesmoAjuste(capAntes,capDepois))
				{
					//mudou nada...
					tipoAlt = ttatINDEF;
				}
				else
				{  //mudou ajuste
					tipoAlt = ttatAJUSTE;
				}
			}
			else
			{   //mudou de lugar, substituicao
				tipoAlt = ttatSUBST;
			}
		}
		else
		{   //trocou o eqpto
			tipoAlt = ttatSUBST;
		}
	}

	return tipoAlt;
}
// ---------------------------------------------------------------------------
// eof
