// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TAvaliaAcoesRede.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTFases.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoRede.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesRede(VTApl *apl)
{
	try
	{
		return (new TAvaliaAcoesRede(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesRede::TAvaliaAcoesRede(VTApl *apl):VTAvaliaAcoesEqpto(apl)
{

}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoesRede::~TAvaliaAcoesRede(void)
{

}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRede::DeterminaTipoSemelhanca(VTAcao *acao)
{
	AnsiString tipoSemelhanca;
	VTArranjo *arranjoFinal;

	tipoSemelhanca = NomeTipoAcao(acao);

	return tipoSemelhanca;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRede::NomeTipoAcao(VTAcao *acao)
{
	AnsiString tipoAcao = "";
	VTEqpto *eqpto = NULL;

	if (ValidaTipoEqpto(acao, eqptoREDE))
	{
		eqpto = DeterminaEqpto(acao);
		//verifica o tipo de acao
		switch (acao->Tipo())
		{
			case acaoINSERE:
				tipoAcao = StringInsereRede((VTRede*) eqpto);
				tipoAcao += " " + StringTensao((VTRede*) eqpto);
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
AnsiString __fastcall TAvaliaAcoesRede::StringInsereRede(VTRede *rede)
{
	AnsiString insere ="";

	try
	{
		if (rede == NULL)
			{return insere;}

		if (rede->TipoRede != NULL)
		{
			switch(rede->TipoRede->Segmento)
			{
				case redeETT:
				case redeETD:
					insere = "Nova subestação";
					break;
				case redeSUB:
					insere = "Novo alimentador SDAT";
					break;
				case redePRI:
					insere = "Novo alimentador SDMT";
					break;
				case redeSEC:
                    insere = "Nova rede SDBT";
					break;
				case redeTRANS:
                    insere = "Novo alimentador STAT";
					break;
				case redeINDEF:
				case redeDEFINIDA_USUARIO:
				default:
					insere = "Nova rede";
            }
		}
	}
	catch(...)
	{
		insere ="";
	}
	return insere;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRede::StringTensao(VTRede *rede)
{
	AnsiString tensao ="";

	try
	{
		if (rede == NULL)
			{return tensao;}

		if (rede->TipoRede != NULL)
		{
			switch(rede->TipoRede->Segmento)
			{
				case redeETT:
				case redeETD:
					tensao = TensaoSubestacao(rede);
					break;
				case redeSUB:
				case redePRI:
				case redeSEC:
				case redeTRANS:
				case redeINDEF:
				case redeDEFINIDA_USUARIO:
				default:
					if(rede->Radial)
					{
						tensao = TensaoRedeRadial(rede);
					}
					break;
            }
		}
	}
	catch(...)
	{
		tensao ="";
	}

	return tensao;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRede::TensaoRedeRadial(VTRede *rede)
{
	AnsiString tensao ="";
	VTBarra *barra = NULL;

	try
	{
		barra = rede->BarraInicial();
		if (barra != NULL)
		{
			if (!IsDoubleZero(barra->vnom))
			{
				tensao = DoubleToStr("%2.1f",barra->vnom)+ " kV";
			}
		}
	}
    catch(...)
	{
		tensao ="";
	}

	return tensao;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoesRede::TensaoSubestacao(VTRede *rede)
{
    AnsiString tensao ="";
	double vnomMax, vnomMin;
	VTBarra *barra;
	bool valorNaoNulo = false;

	try
	{
		//verifica todas barras
		for (int nb = 0; nb < rede->LisBarra()->Count ; nb++)
		{
			barra = (VTBarra*)rede->LisBarra()->Items[nb];
			if (IsDoubleZero(barra->vnom))
			{
                continue;
			}
			if (!valorNaoNulo)
			{
                //seta os valores
				vnomMax = vnomMin = barra->vnom;
				valorNaoNulo = true;
			}
			else
			{	//verifica se eh maior ou menor
				if (barra->vnom > vnomMax)
				{
					vnomMax = barra->vnom;
				}
				if (barra->vnom < vnomMin)
				{
					vnomMin = barra->vnom;
				}
			}
		}
		//monta string
		if (valorNaoNulo)
		{
			tensao = DoubleToStr("%2.1f",vnomMax) + "/" + DoubleToStr("%2.1f",vnomMin) + " kV";
		}
	}
    catch(...)
	{
		tensao ="";
	}

	return tensao;
}
// ---------------------------------------------------------------------------
// eof
