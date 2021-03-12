// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TValMalhas.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Edita\VTEdita.h"
#include "..\..\Radial\VTMalha.h"
#include "..\..\Radial\VTMalhas.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTTipoChave.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Topologia\VTTopologia.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Radial.h"

// ---------------------------------------------------------------------------
VTValMalhas* __fastcall NewObjValMalhas(VTApl *apl)
{
	return (new TValMalhas(apl));
}

// ---------------------------------------------------------------------------
__fastcall TValMalhas::TValMalhas(VTApl *apl)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	// cria objetos
	lisEQP = new TList();
	// configura��o default: tudo false
	PD.abre_chave_ext = false;
	PD.abre_chave_int = false;
	PD.desloca_trecho_ext = false;
	PD.desloca_trecho_int = false;
	// cria objeto malhas
	malhas = DLL_NewObjMalhas(apl);
}

// ---------------------------------------------------------------------------
__fastcall TValMalhas::~TValMalhas(void)
{
	// destr�i listas sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (malhas)
	{
		delete malhas;
		malhas = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TValMalhas::AbreChaveExt(VTMalha *malha)
{
	// vari�veis locais
	VTBarra *barra, *clone;
	VTChave *chave;
	VTLigacao *ligacao, *liga_ant;
	TList *lisLIGACAO = malha->LisLigacao();

	try
	{ // procura uma chave que esteja entre 2 redes (vis)
		for (int nl = 1; nl < lisLIGACAO->Count; nl++)
		{ // determina liga��o e liga��o anterior
			liga_ant = (VTLigacao*)lisLIGACAO->Items[nl - 1];
			ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
			// verifica se liga_ant � de rede diferente
			if (liga_ant->rede != ligacao->rede)
			{ // verifica se liga_ant � chave
				if (liga_ant->Tipo() == eqptoCHAVE)
				{
					chave = (VTChave*)liga_ant;
					if ((chave->TipoChave->Tipo != chaveRA) && (chave->TipoChave->Tipo != chaveDJ))
					{ // abre somente chaves que n�o forem religadores ou disjuntores
						chave->Abre();
						// insere no log
						InsereLog(chave);
						return true;
					}

				}
				// verifica se ligacao � chave
				if (ligacao->Tipo() == eqptoCHAVE)
				{
					chave = (VTChave*)ligacao;
					if ((chave->TipoChave->Tipo != chaveRA) && (chave->TipoChave->Tipo != chaveDJ))
					{ // abre somente chaves que n�o forem religadores ou disjuntores
						chave->Abre();
						// insere no log
						InsereLog(chave);
						return true;
					}
				}
			}
		}
		// s� tem religador ou disjuntor ou nenhuma chave: n�o abre
		return false;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TValMalhas::AbreChaveInt(VTMalha *malha, bool so_chave)
{
	// vari�veis locais
	VTBarra *barra, *clone;
	VTChave *chave;
	VTLigacao *ligacao;
	TList *lisLIGACAO = malha->LisLigacao();

	try
	{ // verifica se s� existem chaves na malha
		if (so_chave)
		{ // loop entre chaves
			for (int n = 0; n < lisLIGACAO->Count; n++)
			{
				chave = (VTChave*)lisLIGACAO->Items[n];
				if ((chave->TipoChave->Tipo != chaveRA) && (chave->TipoChave->Tipo != chaveDJ))
				{ // abre somente chaves que n�o forem religadores ou disjuntores
					chave->Abre();
					// insere no log
					InsereLog(chave);
					return true;
				}
			}
			// s� tem religador ou disjuntor: n�o abre
			return false;
			// chave->Abre();
			// return true;
		}
		// tem outros eqptos na lista: procura uma chave
		for (int n = 0; n < lisLIGACAO->Count; n++)
		{
			ligacao = (VTLigacao*)lisLIGACAO->Items[n];
			if (ligacao->Tipo() != eqptoCHAVE)
				continue;
			chave = (VTChave*)ligacao;
			if ((chave->TipoChave->Tipo != chaveRA) && (chave->TipoChave->Tipo != chaveDJ))
			{ // abre somente chaves que n�o forem religadores ou disjuntores
				chave->Abre();
				// insere no log
				InsereLog(chave);
				return true;
			}
		}
		// s� tem religador ou disjuntor ou nenhuma chave: n�o abre
		return false;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17113673
bool __fastcall TValMalhas::DeslocaLigacao(VTBarra *barra, VTLigacao *ligacao,
    bool redes_diferentes)
{
// jss20191122
    // vari�veis locais
    VTBarra   *clone; //, *outra_barra;
    VTLigacao *liga2,
              *ligacao_alt;
    VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    TList     *lisLIG,
              *lisEQP_ALT,
              *lisEQP_INS;
    int        ind_barra; //, count_rede;
    VTEdita   *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

    // limpa listas sem destruir objetos
    lisEQP_ALT = new TList();
    lisEQP_INS = new TList();
    lisEQP_ALT->Clear();
    lisEQP_INS->Clear();

    try
    {
        // abre a malha, copiando uma barra e alterando a liga��o
        clone = barra->Clone();
        clone->Id = FalsoId();
//      redes->InsereBarra(clone);
        if (! redes_diferentes)
        {
            // prepara para editar
            ligacao_alt = ligacao->Clone();
            // verifica qual indice da barra � a barra
            ind_barra = ligacao_alt->IndiceBarra(barra);
            // determina outra barra
//          outra_barra = ligacao->Barra(1 - ind_barra);
//          ligacao->DefineObjBarra(clone, outra_barra);
            if (ind_barra == 0)
                ligacao_alt->pbarra1 = clone;
            else
                ligacao_alt->pbarra2 = clone;
            // insere Barra e Regulador em lisEQP_NOVO
            lisEQP_INS->Add(clone);
            // insere Ligacao que ser� alterada em lisEQP_ALT
            ligacao->Obj = ligacao_alt;
            lisEQP_ALT->Add(ligacao);
            // executa Edita para incluir e alterar Eqptos
            edita->InsereAlteraRetiraLisEqpto(lisEQP_INS, lisEQP_ALT, NULL);
            // insere liga�ao no log
            InsereLog(ligacao);
            // destroi aslistas
            delete lisEQP_INS;
            delete lisEQP_ALT;
            return true;
        }
        // determina todas as liga��es na barra comum
        lisLIG = new TList();
        lisLIG->Clear();
        redes->LisLigacao(lisLIG, barra);
        // desloca todas as liga��es da mesma rede da ligacao
        for (int n = 0; n < lisLIG->Count; n++)
        {
            liga2 = (VTLigacao*)lisLIG->Items[n];
            if (liga2->rede != ligacao->rede)
                continue;
            // prepara para editar
            ligacao_alt = liga2->Clone();
            // verifica qual indice da barra � a barra
            ind_barra = ligacao_alt->IndiceBarra(barra);
            // determina outra barra
//          outra_barra = liga2->Barra(1 - ind_barra);
//          liga2->DefineObjBarra(clone, outra_barra);
            if (ind_barra == 0)
                ligacao_alt->pbarra1 = clone;
            else
                ligacao_alt->pbarra2 = clone;
            // insere Barra e Regulador em lisEQP_NOVO
            lisEQP_INS->Add(clone);
            // insere Ligacao que ser� alterada em lisEQP_ALT
            liga2->Obj = ligacao_alt;
            lisEQP_ALT->Add(liga2);
            // insere liga�ao no log
            InsereLog(liga2);
        }
        // executa Edita para incluir e alterar Eqptos
        edita->InsereAlteraRetiraLisEqpto(lisEQP_INS, lisEQP_ALT, NULL);
        // destroi listas
        delete lisLIG;
        delete lisEQP_INS;
        delete lisEQP_ALT;
    }
    catch (Exception &e)
    {
        if(lisLIG)
            delete lisLIG;
        if(lisEQP_INS)
            delete lisEQP_INS;
        if(lisEQP_ALT)
            delete lisEQP_ALT;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TValMalhas::DeslocaLigacao(VTMalha *malha, bool redes_diferentes)
{
	// vari�veis locais
	bool sucesso = false;
	VTBarra *barra, *clone;
	VTLigacao *ligacao, *liga_ant;
	TList *lisLIGACAO = malha->LisLigacao();

	try
	{
		for (int n = 1; n < lisLIGACAO->Count; n++)
		{ // analisa liga��es duas a duas
			liga_ant = (VTLigacao*)lisLIGACAO->Items[n - 1];
			ligacao = (VTLigacao*)lisLIGACAO->Items[n];
			if (redes_diferentes)
			{ // verifica se s�o de redes diferentes
				if (liga_ant->rede == ligacao->rede)
					continue;
				//verifica se s�o do mesmo tipo de rede (para nao separar trafo MTBT da rede MT)
				if(liga_ant->rede->TipoRede != ligacao->rede->TipoRede)
					continue;
			}
			// determina barra comum
			barra = ligacao->BarraComum(liga_ant);
			//prote��o
			if(barra == NULL)
				continue;
			// desloca ligacao para clone dessa barra
			sucesso = DeslocaLigacao(barra, ligacao, redes_diferentes);
			if(sucesso)
                break;
		}
		//verifica primeira e ultima liga��o
		if(! sucesso)
		{
			liga_ant = (VTLigacao*)lisLIGACAO->First();
			ligacao = (VTLigacao*)lisLIGACAO->Last();
			if (redes_diferentes)
			{ // verifica se s�o de redes diferentes
				if (liga_ant->rede == ligacao->rede)
					return(sucesso);
				//verifica se s�o do mesmo tipo de rede (para nao separar trafo MTBT da rede MT)
				if(liga_ant->rede->TipoRede != ligacao->rede->TipoRede)
					return(sucesso);
			}
			// determina barra comum
			barra = ligacao->BarraComum(liga_ant);
			//prote��o
			if(barra == NULL)
				return(sucesso);
			// desloca ligacao para clone dessa barra
			sucesso = DeslocaLigacao(barra, ligacao, redes_diferentes);
        }
	}
	catch (Exception &e)
	{
		return false;
	}
	return(sucesso);
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TValMalhas::Executa(void)
 {
 // vari�veis locais
 TList *lisLIGACAO;
 TList *lisMALHA;
 VTChave *chave;
 VTLigacao *ligacao;
 VTMalha *malha;
 VTMalhas *malhas;

 // cria objeto malhas
 if ((malhas = DLL_NewObjMalhas(apl)) == NULL)
 return (false);
 // executa Malhas e obt�m lista de malhas internas �s redes
 if (malhas->Inicia())
 {
 lisMALHA = malhas->LisMalhaInterna();
 for (int n = 0; n < lisMALHA->Count; n++)
 {
 malha = (VTMalha*)lisMALHA->Items[n];
 // obt�m Ligacoes da Malha
 lisLIGACAO = malha->LisLigacao();
 chave = ExisteChaveNF(lisLIGACAO);
 if(chave != NULL)
 {// abra a Chave para eliminar a Malha
 chave->Abre();
 continue;
 }
 else
 {// abre a malha, copiando uma barra e alterando uma liga��o para ela

 }
 }
 }
 return (true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TValMalhas::Executa(void)
{
	// vari�veis locais
	bool sucesso = true;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	//reinicia campos para o log
	ReiniciaLog();
	//verifica se h� chaves em paralelo com disjuntor/religador (abre chave)
	ValidaChavesParalelo();
	//reconfigura topologia
    if(topologia)
		topologia->Reconfigura(redes);
	// verifica se h� mais de 1 rede importada (malhas externas)
	if (redes->LisRede()->Count > 1)
	{
		sucesso = ExecutaMalhaExterna();
	}
	if (sucesso)
	{ // abre malhas internas
		sucesso = ExecutaMalhaInterna();
	}
	//reinicia campos para o log
	ReiniciaLog();
	return sucesso;
}

// -----------------------------------------------------------------------------
bool __fastcall TValMalhas::ExecutaMalhaExterna(void)
{
	// vari�veis locais
	bool abriu_malha = false;
	TList *lisMALHA;
	VTMalha *malha;

	// verifica op��es selecionadas
	if ((!PD.abre_chave_ext) && (!PD.desloca_trecho_ext))
		return (true);
	try
	{ // protecao
		if (malhas == NULL)
			return (false);
		do
		{ // repita enquanto alguma malha for aberta
			abriu_malha = false;
			// executa Malhas e obt�m lista de malhas externas �s redes
			malhas->Inicia();
			// obt�m lista de todas as malhas externas
			lisMALHA = malhas->LisMalhaExterna();
			// analisa todas as malhas
			for (int n = 0; n < lisMALHA->Count; n++)
			{
				malha = (VTMalha*)lisMALHA->Items[n];
				// verifica se pode abrir chave
				if (PD.abre_chave_ext && AbreChaveExt(malha))
				{ // indica abertura de malha e interrompe o loop, pois as demais
					// malhas podem ter eqptos da malha que foi aberta
					abriu_malha = true;
//					continue;
					 break;
				}
				// verifica se pode deslocar liga��o (redes diferentes = true)
				if (PD.desloca_trecho_ext && DeslocaLigacao(malha, true))
				{ // indica abertura de malha e interrompe o loop, pois as demais
					// malhas podem ter eqptos da malha que foi aberta
					abriu_malha = true;
//					continue;
					 break;
				}
			}
		}
		while (abriu_malha);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TValMalhas::ExecutaMalhaInterna(void)
{
	// vari�veis locais
	bool abriu_malha = false;
	bool so_chave = false;
	TList *lisMALHA;
	VTMalha *malha;

	// verifica op��es selecionadas
	if ((!PD.abre_chave_int) && (!PD.desloca_trecho_int))
		return (true);
	try
	{ // protecao
		if (malhas == NULL)
			return (false);
		do
		{ // repita enquanto alguma malha for aberta
			abriu_malha = false;
			// executa Malhas e obt�m lista de malhas internas �s redes
			malhas->Inicia();
			// obt�m lista de todas as malhas internas
			lisMALHA = malhas->LisMalhaInterna();
			// analisa todas as malhas
			for (int n = 0; n < lisMALHA->Count; n++)
			{
				malha = (VTMalha*)lisMALHA->Items[n];
				// verifica se n�o � banco de trafos
				if (SoTrafos(malha))
					continue;
				// verifica se s� tem chaves na malha
				so_chave = SoChaves(malha);
				// verifica se pode abrir chave
				if (PD.abre_chave_int && AbreChaveInt(malha, so_chave))
				{ // indica abertura de malha e interrompe o loop, pois as demais
					// malhas podem ter eqptos da malha que foi aberta
					abriu_malha = true;
//					continue;
					 break;
				}
				// verifica se pode deslocar liga��o (redes diferentes = false)
				if (PD.desloca_trecho_int && DeslocaLigacao(malha, false))
				{ // indica abertura de malha e interrompe o loop, pois as demais
					// malhas podem ter eqptos da malha que foi aberta
					abriu_malha = true;
//					continue;
					 break;
				}
			}
		}
		while (abriu_malha);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
VTChave* __fastcall TValMalhas::ExisteChaveParalela(TList *lisCHV, VTChave *chave1)
{
	// vari�veis locais
	VTChave *chave2;

	for(int n = 0; n < lisCHV->Count; n++)
	{
		chave2 = (VTChave*)lisCHV->Items[n];
		if(chave2 == chave1)
			continue;
		if((chave2->ConectaBarra(chave1->pbarra1)) && (chave2->ConectaBarra(chave1->pbarra2)))
		{
			return chave2;
		}
	}
    return NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TValMalhas::InsereLog(VTLigacao *ligacao)
{
	// atualiza log
	if (plog)
	{
		plog->DefineValor("Eqpto", ligacao->TipoAsString());
		plog->DefineValor("C�digo", ligacao->Codigo);
        plog->DefineValor("ID", ligacao->Id);
		plog->DefineValor("Rede", ligacao->rede->Codigo);
		switch (ligacao->rede->TipoRede->Segmento)
		{
		case redeETD:
			plog->DefineValor("SED", ligacao->rede->Codigo);
			plog->DefineValor("SDMT", "");
			plog->DefineValor("SDBT", "");
			break;
		case redePRI:
			plog->DefineValor("SDMT", ligacao->rede->Codigo);
			plog->DefineValor("SED", "");
			plog->DefineValor("SDBT", "");
			break;
		case redeSEC:
			plog->DefineValor("SDBT", ligacao->rede->Codigo);
			plog->DefineValor("SDMT", "");
			plog->DefineValor("SED", "");
			break;
		default:
			break;
		}
		plog->Add("Malha aberta");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TValMalhas::ReiniciaLog(void)
{
	// obt�m objeto VTLog
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	// cancela informa��o de arquivo e rede
	if (plog)
	{
		plog->DefineValor("Arquivo", "");
		plog->DefineValor("ID", "");
		plog->DefineValor("Rede", "");
		plog->DefineValor("SED", "");
		plog->DefineValor("SDMT", "");
		plog->DefineValor("SDBT", "");
		plog->DefineValor("Eqpto", "");
		plog->DefineValor("C�digo", "");
		plog->DefineValor("Descri��o", "");
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TValMalhas::SoChaves(VTMalha *malha)
{
	// vari�veis locais
	TList *lisLIG = malha->LisLigacao();
	VTLigacao *ligacao;
	int count_chave = 0;

	for (int n = 0; n < lisLIG->Count; n++)
	{ // conta quantas chaves tem na malha
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoCHAVE)
			count_chave++;
	}
	// se todas as liga��es forem chaves, s� tem chaves
	return (count_chave == lisLIG->Count);
}

// -----------------------------------------------------------------------------
bool __fastcall TValMalhas::SoTrafos(VTMalha *malha)
{
	// vari�veis locais
	TList *lisLIG = malha->LisLigacao();
	VTLigacao *ligacao;
	int count_trafo = 0;

	for (int n = 0; n < lisLIG->Count; n++)
	{ // conta quantos trafos tem na malha
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoTRAFO)
			count_trafo++;
	}
	// se todas as liga��es forem trafos, � banco de trafos
	return (count_trafo == lisLIG->Count);
}

// -----------------------------------------------------------------------------
void __fastcall TValMalhas::ValidaChavesParalelo(void)
{
	// vari�veis locais
	VTChave *chave1, *chave2;
	VTLigacao *ligacao;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCHV = new TList();

	redes->LisLigacao(lisCHV, eqptoCHAVE);
	for(int n = 0; n < lisCHV->Count; n++)
	{
		chave1 = (VTChave*)lisCHV->Items[n];
        if((chave1->TipoChave->Tipo == chaveRA) || (chave1->TipoChave->Tipo == chaveDJ))
		{
			if((chave2 = ExisteChaveParalela(lisCHV, chave1)) != NULL)
			{//abre chave em paralelo
				chave2->Abre();
			}
		}
    }

	delete lisCHV;
}

/*
// -----------------------------------------------------------------------------
void __fastcall TValMalhas::ValidaChavesParalelo(void)
{
	// vari�veis locais
	VTChave *chave1, *chave2;
	VTLigacao *ligacao;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCHV = new TList();
	TList *lisDEL = new TList();

	redes->LisLigacao(lisCHV, eqptoCHAVE);
	for(int n = lisCHV->Count - 1; n >= 0; n--)
	{
		chave1 = (VTChave*)lisCHV->Items[n];
		if((chave2 = ExisteChaveParalela(lisCHV, chave1)) != NULL)
		{//verifica se uma delas � religador ou disjuntor
			if((chave2->TipoChave->Tipo == chaveRA) || (chave2->TipoChave->Tipo == chaveDJ))
			{//insere chave1 na lista para apagar
				lisDEL->Add(chave1);
			}
		}
    }
	//remove chaves em lote
	edita->RetiraLisEqpto(lisDEL);
	delete lisCHV;
	delete lisDEL;
}
*/
// -----------------------------------------------------------------------------
// eof
