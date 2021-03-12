// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TAvaliaAcoesChave.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesChave(VTApl *apl)
{
	try
	{
		return (new TAvaliaAcoesChave(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesChave::TAvaliaAcoesChave(VTApl *apl):VTAvaliaAcoesEqpto(apl)
{

}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesChave::~TAvaliaAcoesChave(void)
{

}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesChave::DeterminaTipoSemelhanca(VTAcao *acao)
{
	AnsiString tipoSemelhanca;

	tipoSemelhanca = NomeTipoAcao(acao);

	return tipoSemelhanca;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18710638
AnsiString __fastcall TAvaliaAcoesChave::NomeTipoAcao(VTAcao *acao)
{
	AnsiString tipoAcao = "";
	VTEqpto *eqpto = NULL;

	if (ValidaTipoEqpto(acao, eqptoCHAVE))
	{
		eqpto = DeterminaEqpto(acao);

		//verifica o tipo de acao
		switch (acao->Tipo())
		{
			case acaoINSERE:
				tipoAcao =  StringTipoChave((VTChave*)eqpto, true);
				break;
			case acaoALTERA:
				switch (TipoAlteracao(acao))
				{   //verifica se eh manobra
					case ttarMANOBRA:
						tipoAcao = tipoAaMANOBRA;
						tipoAcao += " " + StringTipoRede((VTChave*)eqpto);
						break;
					//verifica se eh substituicao
					case ttarSUBST:
						tipoAcao = tipoAaSUBST;
                        //adiciona a potencia do inserido
						tipoAcao += " "+ StringTipoChave((VTChave*)eqpto, false);
						break;
					default:
						tipoAcao = "Indefinido";
                    ;
				}
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
bool _fastcall TAvaliaAcoesChave::MesmoEqpto(VTChave *chave1, VTChave *chave2)
{
	bool mesmo = true;

	try
	{
		if (chave1 == NULL)
			{return false;}
		if (chave2 == NULL)
			{return false;}
		//verifica corrente
		if (!IsDoubleZero(chave1->Inom_amp - chave2->Inom_amp))
			{return false;}
		//cap disru
		if (!IsDoubleZero(chave1->CapDisruptiva_amp - chave2->CapDisruptiva_amp))
			{return false;}
		if (chave1->TecnoComunicacao != chave2->TecnoComunicacao)
			{return false;}
		if (chave1->TipoChave != chave2->TipoChave)
			{return false;}
        if (chave1->EloFusivel.AnsiCompare(chave2->EloFusivel) != 0)
			{return false;}
	}
	catch(...)
	{
		mesmo = false;
	}

	return mesmo;
}
// ---------------------------------------------------------------------------
bool _fastcall TAvaliaAcoesChave::MesmoEstado(VTChave *chave1, VTChave *chave2)
{
    bool mesmo = true;

	try
	{
		if (chave1 == NULL)
			{return false;}
		if (chave2 == NULL)
			{return false;}
		if (chave1->Estado != chave2->Estado)
			{return false;}
	}
	catch(...)
	{
		mesmo = false;
	}

	return mesmo;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesChave::StringTipoChave(VTChave *chave, bool nova)
{
	AnsiString tipoChave ="";

	try
	{
		if (chave == NULL)
			{return tipoChave;}

		if (chave->TipoChave != NULL)
		{
			switch(chave->TipoChave->Tipo)
			{
				case chaveDISJUNTOR:
					if (nova)
					{
						tipoChave = "Novo ";
					}
					tipoChave += "Disjuntor";
					break;
				case chaveRELIGADORA:
                    if (nova)
					{
						tipoChave = "Novo ";
					}
					tipoChave += "Religador";
					break;
				case chaveBASE_FUSIVEL:
                    if (nova)
					{
						tipoChave = "Nova ";
					}
					tipoChave += "Base Fusivel";
					break;
				case chaveSECCIONADORA:
                    if (nova)
					{
						tipoChave = "Nova ";
					}
					tipoChave += "Seccionadora";
					break;
				case chaveFACA:
                    if (nova)
					{
						tipoChave = "Nova ";
					}
					tipoChave += "Chave Faca";
					break;
				case chaveDEFINIDA_USUARIO:
				case chaveINDEFINIDA:
				case chaveDESCONECTAVEL:
				case chaveNETWORK_PROTECTOR:
				default:
                    if (nova)
					{
						tipoChave = "Novo ";
					}
					tipoChave += chave->TipoChave->Codigo;
					break;
            }
		}
	}
	catch(...)
	{
		tipoChave ="";
	}

	return tipoChave;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesChave::StringTipoRede(VTChave *chave)
{
	AnsiString tipoChave = "";
	VTRede *rede = NULL;

	try
	{
		if (chave == NULL)
			{return tipoChave;}
		rede = chave->rede;
		if (rede == NULL)
			{return tipoChave;}

		if (rede->TipoRede != NULL)
		{
			switch(rede->TipoRede->Segmento)
			{
				case redeETT:
					tipoChave = "SET";
				case redeETD:
					tipoChave = "SED";
					break;
				case redeSUB:
					tipoChave = "SDAT";
					break;
				case redePRI:
					tipoChave = "SDMT";
					break;
				case redeSEC:
					tipoChave = "SDBT";
					break;
				case redeTRANS:
					tipoChave = "STAT";
					break;
				case redeINDEF:
				case redeDEFINIDA_USUARIO:
				default:
					tipoChave = rede->TipoRede->Codigo;
            }
		}
	}
	catch(...)
	{
		tipoChave ="";
	}
	return tipoChave;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18710638
int __fastcall TAvaliaAcoesChave::TipoAlteracao(VTAcao *acao)
{
	VTChave *chaveAntes, *chaveDepois;
	int tipoAlt = ttarINDEF;

	//protecao
	if (acao == NULL)
		{return tipoAlt;}
	//julga
//	if (ValidaTipoEqpto(acao, eqptoREGULADOR))
	if (ValidaTipoEqpto(acao, eqptoCHAVE))
	{
		//aqui julga diferenças entre faseamento e recondut
		chaveAntes = (VTChave*)acao->CopiaDoEqptoOriginal();
		chaveDepois = (VTChave*)acao->CopiaDoEqptoAlterado();
		//verifica se trocou o equipamento
		if (MesmoEqpto(chaveAntes,chaveDepois))
		{
			//mesmo eqpto, verifica se mudou de lugar
			if (MesmoLocal(chaveAntes,chaveDepois))
			{
				//verifica se mudou estado
				if (MesmoEstado(chaveAntes,chaveDepois))
				{
					//mudou nada...
                    tipoAlt = ttarINDEF;
				}
				else
				{  //mudou estado
					tipoAlt = ttarMANOBRA;
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
// eof
