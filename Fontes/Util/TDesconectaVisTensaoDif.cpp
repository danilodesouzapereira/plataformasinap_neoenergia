//---------------------------------------------------------------------------

#pragma hdrstop

#include "TDesconectaVisTensaoDif.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <math.h>
//#include <Math.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipoRede.h>

#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
// ---------------------------------------------------------------------------
VTDesconectaVisTensaoDif* __fastcall NewObjDesconectaVisTensaoDif(VTApl *apl_owner)
{
	try
	{
		return (new TDesconectaVisTensaoDif(apl_owner));
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TDesconectaVisTensaoDif::TDesconectaVisTensaoDif(VTApl *apl)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	// cria listas
	lisEQPTO_MOD = new TList;
	lisBARRA_NOVA = new TList;
}

// ---------------------------------------------------------------------------
__fastcall TDesconectaVisTensaoDif::~TDesconectaVisTensaoDif(void)
{
	if (lisEQPTO_MOD)
	{
		delete lisEQPTO_MOD;
		lisEQPTO_MOD = NULL;
	}
	if (lisBARRA_NOVA)
	{
		delete lisBARRA_NOVA;
		lisBARRA_NOVA = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDesconectaVisTensaoDif::DescobreTensaoRedes(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede;
	AnsiString func = __PRETTY_FUNCTION__;

	try
	{
		lisRede = redes->LisRede();
		//percorre as redes
		for (int nr = 0; nr < lisRede->Count; nr++)
		{
			rede = (VTRede*) lisRede->Items[nr];
			//verifica se é um alimentador
			if ((rede->TipoRede != NULL) && (rede->TipoRede->Segmento == redePRI))
			{
				DescobreTensaoRede(rede);
			}
			else
			{
				continue;
			}

		}
	}
	catch(Exception &e)
	{
		Erro(func + " " + e.Message);
	}
	catch(...)
	{
		Erro(func);
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16526463
void __fastcall TDesconectaVisTensaoDif::DescobreTensaoRede(VTRede *rede)
{
	VTBarra *barraIni;
	VTLigacao *ligacao;
	VTTrafo *trafo;
	TList *lisLig;
	AnsiString func = __PRETTY_FUNCTION__;
	bool conecta_trafo = false;

	try
	{
		if (rede != NULL)
		{
			//verifica a barra inicial
			barraIni = rede->BarraInicial();
			if (barraIni != NULL)
			{
				lisLig = barraIni->LisLigacao();
				//percorre a lista procurando um transformador
				for (int nl = 0; nl < lisLig->Count ; nl++)
				{
					ligacao = (VTLigacao*)lisLig->Items[nl];
					if (ligacao->Tipo() == eqptoTRAFO)
					{
						trafo = (VTTrafo*)ligacao;
						//verifica se é igual à barra do secundario
						if (barraIni == trafo->BarraSecundario())
						{
							mapRedeTensao.insert(std::pair<VTRede*,double>(rede,trafo->sec.vnom));
						}
						else if (barraIni == trafo->BarraPrimario())
						{//igual ao primario (??)
							mapRedeTensao.insert(std::pair<VTRede*,double>(rede,trafo->pri.vnom));
						}
						else
						{//igual a nenhuma (?!?!??!)
							//faz nada
						}
						conecta_trafo = true;
                        break;
					}
				}
				//verifica se a barra não conecta trafo
				if(! conecta_trafo)
				{//assume tensão da barra
					mapRedeTensao.insert(std::pair<VTRede*,double>(rede,barraIni->vnom));
				}
			}
		}
	}
	catch(Exception &e)
	{
		Erro(func + " " + e.Message);
	}
	catch(...)
	{
		Erro(func);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDesconectaVisTensaoDif::DesconectaBarra(VTChave *chave, int indice_barra)
{
	AnsiString func = __PRETTY_FUNCTION__;
	VTChave *chaveDps;
	VTBarra *barraNova;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{   //protecao
		if ((indice_barra < 0) ||(chave == NULL))
		{
            return;
		}
		//verifica se já foi alterada
		if (lisEQPTO_MOD->IndexOf(chave) < 0)
		{
			lisEQPTO_MOD->Add(chave);
		}
		else
		{
			return;
        }
		//verifica qual barra
		barraNova = chave->Barra(indice_barra)->Clone();
		if (barraNova != NULL)
		{
			lisBARRA_NOVA->Add(barraNova);
			barraNova->Id = FalsoId();
            barraNova->Status[sttNOVO] = true;
			redes->InsereBarra(barraNova);
			//altera a barra da chave
			// cria cópia da Ligacao
			if ((chaveDps = chave->Clone()) == NULL)
				{return;}
			//verifica qual barra vai mudar
			switch (indice_barra)
			{
				case 0:
					chaveDps->pbarra1 = barraNova;
					break;
				case 1:
					chaveDps->pbarra2 = barraNova;
					break;
				case 2:
					chaveDps->pbarra3 = barraNova;
					break;
				default:
				//faz nada
				;
			}
			//guarda o alterado no original
			chave->Obj = chaveDps;
		}
	}
	catch(Exception &e)
	{
		Erro(func + " " + e.Message);
	}
	catch(...)
	{
		Erro(func);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TDesconectaVisTensaoDif::Executa(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	TList *lisCHAVE, *lisVAZIA;
	VTChave *chave;
    AnsiString func = __PRETTY_FUNCTION__;
	int index;

	try
	{
        // return true;
		//reinicia as listas e map
		Reinicia();
		//mapeia as tensoes das redes baseada nos trafos ligados às barras iniciais
		DescobreTensaoRedes();
		// lista todas as chaves
		lisCHAVE = new TList;
		lisVAZIA = new TList;
		redes->LisLigacao(lisCHAVE,eqptoCHAVE);
		//percorre todas chaves
		for (int nc = 0; nc < lisCHAVE->Count; nc++)
		{
			chave = (VTChave*)lisCHAVE->Items[nc];
			if (chave->Estado == chvABERTA)
			{   //verifica o indice da barra de fronteira
				if ((index = IndiceBarraFronteiraChaveVis(chave)) >= 0)
				{
					DesconectaBarra(chave, index);
				}
				else
				{ //não é de fronteira
					//faz nada
				}
			}
		}
		//com as listas preenchidas, altera os equiptos
		edita->InsereAlteraRetiraLisEqpto(lisBARRA_NOVA,lisEQPTO_MOD,lisVAZIA);
	}
	catch(Exception &e)
	{
		Erro(func + " " + e.Message);
	}
	catch(...)
	{
		Erro(func);
		return false;
	}
	if (lisCHAVE)
	{
		delete lisCHAVE;
		lisCHAVE = NULL;
	}
	if (lisVAZIA)
	{
		delete lisVAZIA;
		lisVAZIA = NULL;
	}
    return true;
}
// ---------------------------------------------------------------------------
int __fastcall TDesconectaVisTensaoDif::IndiceBarraFronteiraChaveVis(VTChave *chave)
{
	VTLigacao *ligacao;
	VTRede *redeChave;
	AnsiString func = __PRETTY_FUNCTION__;

	try
	{
		//protecao
		if ((chave == NULL) && ((redeChave = chave->rede) == NULL))
		{
			return -1;
		}
		//verifica a barra 1
		if (PossuiLigacaoRedeOutraTensao(chave->pbarra1, chave->rede))
		{
			return 0;
		}
		else if (PossuiLigacaoRedeOutraTensao(chave->pbarra2, chave->rede))
		{
			return 1;
		}
		else if (PossuiLigacaoRedeOutraTensao(chave->pbarra3, chave->rede))
		{
			return 2;
		}
		else
		{
            return -1;
        }
	}
	catch(Exception &e)
	{
		Erro(func + " " + e.Message);
	}
	catch(...)
	{
		Erro(func);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TDesconectaVisTensaoDif::PossuiLigacaoRedeOutraTensao(VTBarra *barra, VTRede *redeOrig)
{
	VTLigacao *ligacao;
	VTRede *redeOutra;
	double tensaoA, tensaoB;
	std::map<VTRede*,double>::iterator it;
	AnsiString func = __PRETTY_FUNCTION__;

	try
	{
		tensaoA = tensaoB = 0;
		if ((barra == NULL) || (redeOrig == NULL))
		{
			return false;
		}
		//verifica a lista de ligacoes da barra
		for (int nl = 0; nl < barra->LisLigacao()->Count ; nl++)
		{
			ligacao = (VTLigacao*)barra->LisLigacao()->Items[nl];
			if ((redeOutra = ligacao->rede) != redeOrig)
			{
				//verifica tensão
				if ((it = mapRedeTensao.find(redeOrig)) !=  mapRedeTensao.end())
				{
					tensaoA = it->second;
				}
				if ((it = mapRedeTensao.find(redeOutra)) !=  mapRedeTensao.end())
				{
					tensaoB = it->second;
				}
				else
				{   //como a rede pode ter nao sido mapeada
					tensaoB = tensaoA;
				}
				if (!IsDoubleZero(tensaoA - tensaoB))
				{
					return true;
				}
			}
		}
		return false;
	}
	catch(Exception &e)
	{
		Erro(func + " " + e.Message);
	}
	catch(...)
	{
		Erro(func);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDesconectaVisTensaoDif::Reinicia(void)
{
	mapRedeTensao.clear();
	lisEQPTO_MOD->Clear();
	lisBARRA_NOVA->Clear();
}
// ---------------------------------------------------------------------------


