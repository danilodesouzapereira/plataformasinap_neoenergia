// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TItemObraARIES.h"
//#include <DateUtils.hpp>
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
#include <PlataformaSinap\Fontes\Planejamento\VTIpe.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>


// ---------------------------------------------------------------------------
#include <ModulosOpcionais\Fontes\BaseGRP\VTIpeGRP.h>
#include <ModulosOpcionais\Fontes\BaseGRP\VTPadrao.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// funções globais p/ criar objetos da classe TItemObra
// ---------------------------------------------------------------------------
VTItemObra* NewObjItemObraARIES(VTApl *apl)
{
	return (new TItemObraARIES(apl));
}

// ---------------------------------------------------------------------------
__fastcall TItemObraARIES::TItemObraARIES(VTApl *apl):VTItemObraARIES(apl)
{
	this->Custo     = 0;
	this->qntIns    =0;
    this->qntRet    =0;
	this->padraoIns = NULL;
	this->padraoIns = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TItemObraARIES::~TItemObraARIES(void)
{

}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18710638
bool __fastcall TItemObraARIES::ChaveManobrada(VTAcao* acao)
{
    //variaveis locais
    VTChave *chave, *chave_alt;
    VTEqpto *orig, *final;

    orig = acao->CopiaDoEqptoOriginal();
    final = acao->CopiaDoEqptoAlterado();
    //verifica se é chave
    if(orig->Tipo() != eqptoCHAVE)
        return false;
    //converte
    chave = (VTChave*)orig;
    chave_alt = (VTChave*)final;
    //verifica se houve troca de estado
    if(chave->Estado == chave_alt->Estado)
        return false;
    //sim, manobrada
    return true;
}

// ---------------------------------------------------------------------------
VTItemObra * __fastcall TItemObraARIES::Clone(bool preserveId, bool isDeep)
{
    VTItemObra *clone;

	try
	{
		clone = NewObjItemObraARIES(apl);
		clone->CopiaAtributosDe(*this);
		if (!preserveId)
		{
			clone->Id = FalsoId();
		}
		if (isDeep)
		{
			//associa as acoes clones
			AssociaAcoesClone(clone);
			//limpa atributos
			clone->SalvaIpeAtributos();
		}
	}
	catch(...)
	{
        clone = NULL;
	}

	return clone;
}
// ---------------------------------------------------------------------------
bool __fastcall  TItemObraARIES::ContabilizaCusto(void)
{
	bool sucesso = true;
//	VTPadrao *padrao = NULL;

	try
	{	//zera os valores
		this->Custo = 0.;
		//calcula o custo
		if (padraoIns != NULL)
		{
			this->Custo = qntIns * padraoIns->CustoPorUnidade;
		}
		else if(padraoRet != NULL)
		{
			this->Custo = qntRet * padraoRet->CustoPorUnidade;
        }

	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16917117
//https://sinapsisenergia.teamwork.com/#/tasks/18710638
bool __fastcall  TItemObraARIES::ContabilizaQuantidadeInsercao(void)
{
	bool sucesso = true;
	VTAcao *acao;
	VTEqpto *orig, *final;

	try
	{
		//zera os valores
		qntIns = 0.;
		mapOrigFinalINS.clear();
        mapAcao.clear();

		if (this->PadraoIns != NULL)
		{
			//verifica os padroes e conta a coisas de acordo com o tipo
			for (int na = 0; na < PD.lisACAO->Count; na++)
			{
				acao = (VTAcao*) PD.lisACAO->Items[na];
				switch(acao->Tipo())
                {
                    case acaoINSERE:
                    {
                        orig = acao->Eqpto();
                        final = acao->CopiaDoEqptoOriginal();
                        //insere o par eqpto e sua versao final
                        //sobrescrevendo com a versão mais recente
                        mapOrigFinalINS[orig] = final;
                        //insere no map para comparar no caso de ação retira
                        mapAcao[orig] = acao;
                        break;
                    }
                    case acaoRETIRA:
                    {
                        orig = acao->Eqpto();
                        //verifica se o eqpto está no map
                        if(mapAcao.find(orig) != mapAcao.end())
                        { // retira do mapOrigFinalINS e mapAcao
                            mapOrigFinalINS.erase(orig);
                            mapAcao.erase(orig);
                            break;
                        }
                        break;
                    }
                    case acaoALTERA:
                    {
                        orig = acao->Eqpto();
                        final = acao->CopiaDoEqptoAlterado();
                        //caso especial: manobra de chave
                        if(ChaveManobrada(acao))
                            break;
                        //insere o par eqpto e sua versao final
                        //sobrescrevendo com a versão mais recente
                        mapOrigFinalINS[orig] = final;
                        break;
                    }
                default: break;
                }
			}
			//com o map preenchido, percorre ele todo e contabiliza
			for (std::map<VTEqpto*,VTEqpto*>::iterator it=mapOrigFinalINS.begin(); it != mapOrigFinalINS.end(); ++it)
			{
				qntIns += QuantidadeEqpto(it->second, this->PadraoIns);
			}
		}
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16917117
//https://sinapsisenergia.teamwork.com/#/tasks/18710638
bool __fastcall  TItemObraARIES::ContabilizaQuantidadeRetirada(void)
{
    	bool sucesso = true;
	VTAcao *acao;
	VTEqpto *orig, *final;

	try
	{
		//zera os valores
		qntIns = 0.;
		mapOrigFinalRET.clear();
        mapAcao.clear();

		if (this->PadraoRet != NULL)
		{
			//verifica os padroes e conta a coisas de acordo com o tipo
			for (int na = 0; na < PD.lisACAO->Count; na++)
			{
				acao = (VTAcao*) PD.lisACAO->Items[na];
                switch(acao->Tipo())
                {
                    case acaoINSERE:
                    {
                        orig = acao->Eqpto();
                        mapAcao[orig] = acao;
                        break;
                    }
                    case acaoRETIRA:
                    {
                        orig = acao->Eqpto();
                        //verifica se o eqpto está no map
                        if(mapAcao.find(orig) != mapAcao.end())
                        { // retira do mapAcao e não insere no mapOrigFinalRET
                            mapAcao.erase(orig);
                            break;
                        }
                        final = acao->CopiaDoEqptoOriginal();
                        //insere o par eqpto e sua versao antes da exclusao
                        //sobrescrevendo com a versão mais recente
                        mapOrigFinalRET[orig] = final;
                        break;
                    }
                    case acaoALTERA:
                    {
                        orig = acao->Eqpto();
                        final = acao->CopiaDoEqptoAlterado();
                        //caso especial: manobra de chave
                        if(ChaveManobrada(acao))
                            break;
                        //insere o par eqpto e sua versao final
                        //sobrescrevendo com a versão mais recente
                        mapOrigFinalRET[orig] = final;
                        break;
                    }
                }
			}
			//com o map preenchido, percorre ele todo e contabiliza
			for (std::map<VTEqpto*,VTEqpto*>::iterator it=mapOrigFinalRET.begin(); it != mapOrigFinalRET.end(); ++it)
			{
				qntIns += QuantidadeEqpto(it->second, this->PadraoRet);
			}
		}
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
//// ---------------------------------------------------------------------------
//bool __fastcall  TItemObraARIES::ContabilizaQuantidades(void)
//{
//	bool sucesso = true;
//	VTAcao *acao;
//
//	try
//	{
//		//zera os valores
//		qntIns = 0.;
//		qntRet = 0.;
//		//verifica os padroes e conta a coisas de acordo com o tipo
//		for (int na = 0; na < PD.lisACAO->Count; na++)
//		{
//			acao = (VTAcao*) PD.lisACAO->Items[na];
//			qntIns += QuantidadeEqpto(acao, this->PadraoIns, true);
//			qntRet += QuantidadeEqpto(acao, this->PadraoRet, false);
//		}
//
//	}
//	catch(...)
//	{
//		sucesso = false;
//	}
//
//	return sucesso;
//}// ---------------------------------------------------------------------------
bool __fastcall  TItemObraARIES::ContabilizaQuantidades(void)
{
	bool sucesso = true;

	try
	{
		ContabilizaQuantidadeInsercao();
        ContabilizaQuantidadeRetirada();
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraARIES::CopiaAtributosDe(VTItemObra &ref)
{
	// variáveis locais
	VTItemObraARIES &item = (VTItemObraARIES&)ref;

	// proteção
//	if (!ref.TipoEqbar())
//		return;
	// copia atributos de VTItemObra
	TItemObra::CopiaAtributosDe(ref);
	// copia atributos de VTItemObraARIES
	this->Custo     = item.Custo;
	this->padraoIns = item.PadraoIns;
	this->padraoRet = item.PadraoRet;
	this->QuantidadeIns = item.QuantidadeIns;
	this->QuantidadeRet = item.QuantidadeRet;

}
// ---------------------------------------------------------------------------
void __fastcall  TItemObraARIES::CorrigeValores(int tipoEqpto)
{
	TItemObra::CorrigeValores(tipoEqpto);

	if (tipoEqpto != eqptoINDEF)
	{
        //verifica se o padrao construtivo é do mesmo tipo enviado,
		if ((this->padraoIns != NULL) && (this->padraoIns->TipoEqpto != tipoEqpto))
		{
			this->PadraoIns = NULL;
		}
		if ((this->padraoRet != NULL) && (this->padraoRet->TipoEqpto != tipoEqpto))
		{
			this->PadraoRet = NULL;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall  TItemObraARIES::IniciaExtras(void)
{
	//atualiza custo
	ContabilizaCusto();

	TItemObra::IniciaExtras();
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraARIES::LeIpeAtributo(VTIpeAtributo *atributo)
{
	VTIpeAtributo *tipo_insere, *tipo_retira;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_grp = (VTIpeGRP*)planejamento->Ipe;

	int valorInt = 0;
	double valorDouble = 0;

	try
	{
		if (atributo != NULL)
		{
			switch (atributo->Tipo)
			{
				case padraoINS:
					tipo_insere = ipe_grp->ExisteIpeAtributo(padraoINS, atributo->Id, 0);
					this->padraoIns = (VTPadrao*) tipo_insere;
					break;
				case padraoRET:
					tipo_retira = ipe_grp->ExisteIpeAtributo(padraoRET, atributo->Id, 0);
					this->padraoRet = (VTPadrao*) tipo_retira;
					break;
				case ipeITEMARIES_QNTINS:
					valorDouble = StrToDouble(atributo->Descricao);
					this->QuantidadeIns = valorDouble;
					break;
				case ipeITEMARIES_QNTRET:
					valorDouble = StrToDouble(atributo->Descricao);
					this->QuantidadeRet = valorDouble;
					break;
				case ipeITEM_CUSTO:
					valorDouble = StrToDouble(atributo->Descricao);
					this->Custo = valorDouble;
					break;
			default:
                TItemObra::LeIpeAtributo(atributo);
				;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraARIES::MantemAtributosComunsDe(VTItemObra &ref)
{
	// variáveis locais
	VTItemObraARIES &item = (VTItemObraARIES&)ref;

	// proteção
//	if (!ref.TipoEqbar())
//		return;
	// copia atributos de VTItemObra
	TItemObra::MantemAtributosComunsDe(ref);
	// copia atributos de VTItemObraARIES
	if (!IsDoubleZero(this->Custo - item.Custo, 1e-3))
		{this->Custo = 0.;}
	if (this->PadraoIns != item.PadraoIns)
		{this->padraoIns = NULL;}
	if (this->PadraoRet != item.PadraoRet)
		{this->padraoRet = NULL;}
	if (!IsDoubleZero(this->QuantidadeIns - item.QuantidadeIns, 1e-4))
		{this->QuantidadeIns = 0.;}
	if (!IsDoubleZero(this->QuantidadeRet - item.QuantidadeRet, 1e-4))
		{this->QuantidadeRet = 0.;}
}
// ---------------------------------------------------------------------------
bool __fastcall TItemObraARIES::PadraoesTemTipoEqptoIguais(VTPadrao *padrao1,
	VTPadrao *padrao2)
{
	if (padrao1 == padrao2)
	{
		return true;
	}
	if (padrao1 == NULL)
	{
		return false;
	}
	if (padrao2 == NULL)
	{
		return false;
	}
	if (padrao1->TipoEqpto != padrao2->TipoEqpto)
	{
		return false;
	}

	return true;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
VTPadrao* __fastcall TItemObraARIES::PM_GetPadraoIns(void)
{
	return(this->padraoIns);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
VTPadrao* __fastcall TItemObraARIES::PM_GetPadraoRet(void)
{
	return(this->padraoRet);
}

// ---------------------------------------------------------------------------
void __fastcall TItemObraARIES::PM_SetPadraoIns(VTPadrao *padrao)
{
	VTPadrao *padraoOld;

	padraoOld = this->PadraoIns;
	this->padraoIns = padrao;
	// se houve mudança no tipo de eqpto, é preciso recontar a quantidade
	if (!PadraoesTemTipoEqptoIguais(padraoOld, padrao))
	{
		// tendo tipos de eqpto diferente, eh preciso recontar quantidades
		this->ContabilizaQuantidadeInsercao();
	}
	// e por isso atualiza os custos
	this->ContabilizaCusto();
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraARIES::PM_SetPadraoRet(VTPadrao *padrao)
{
    VTPadrao *padraoOld;

	padraoOld = this->PadraoRet;
	this->padraoRet = padrao;
	// se houve mudança no tipo de eqpto, é preciso recontar a quantidade
	if (!PadraoesTemTipoEqptoIguais(padraoOld, padrao))
	{
		// tendo tipos de eqpto diferente, eh preciso recontar quantidades
		this->ContabilizaQuantidadeRetirada();
	}
}
// ---------------------------------------------------------------------------
double __fastcall TItemObraARIES::QuantidadeEqpto(VTEqpto *eqptoFinal, VTPadrao *padrao)
{
	double quantidade = 0;
	VTTrecho *trecho;

	if (eqptoFinal == NULL)
		{return quantidade;}
	if (padrao == NULL)
		{return quantidade;}
	//verifica se o padrao é do mesmo tipo q o eqptoFinal
	if (padrao->TipoEqpto != eqptoFinal->Tipo())
		{return quantidade;}


	if (eqptoFinal->Tipo() == eqptoTRECHO)
	{ // acumula o comprimento dos trechos
		trecho = (VTTrecho*)eqptoFinal;
		quantidade = trecho->Comprimento_km;
	}
	else
	{
		quantidade = 1.;
	}

	return quantidade;
}
//// ---------------------------------------------------------------------------
//double __fastcall  TItemObraARIES::QuantidadeEqpto(VTAcao *acao, VTPadrao *padrao, bool inclusao)
//{
//	double quantidade = 0;
//	VTTrecho *trecho;
//
//
//	if (acao == NULL)
//		{return quantidade;}
//	if (padrao == NULL)
//		{return quantidade;}
//
//	if(inclusao)
//	{
//		//verifica tipo
//		if ((acao->Tipo() == acaoINSERE) || (acao->Tipo() == acaoALTERA))
//		{
//			if (acao->Eqpto() != NULL)
//			{
//				//verifica se tipo da acao eh igual do padrao
//				if (acao->Eqpto()->Tipo() == padrao->TipoEqpto)
//				{
//					if (acao->Eqpto()->Tipo() == eqptoTRECHO)
//					{ // acumula o comprimento dos trechos
//						trecho = (VTTrecho*)acao->Eqpto();
//						quantidade = trecho->Comprimento_km;
//					}
//					else
//					{
//						quantidade = 1.;
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//        //verifica tipo
//		if (acao->Tipo() == acaoRETIRA)
//		{
//			if (acao->Eqpto() != NULL)
//			{
//				//verifica se tipo da acao eh igual do padrao
//				if (acao->Eqpto()->Tipo() == padrao->TipoEqpto)
//				{
//					if (acao->Eqpto()->Tipo() == eqptoTRECHO)
//					{ // acumula o comprimento dos trechos
//						trecho = (VTTrecho*)acao->Eqpto();
//						quantidade = trecho->Comprimento_km;
//					}
//					else
//					{
//						quantidade = 1.;
//					}
//				}
//			}
//		}
//	}
//
//	return quantidade;
//}
// ---------------------------------------------------------------------------
void __fastcall TItemObraARIES::SalvaIpeAtributosProprios(void)
{
    // variaveis locais
	AnsiString texto;
	VTIpeAtributo *atributo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_grp = (VTIpeGRP*)planejamento->Ipe;
	int valorInt = 0;
	int valorDouble = 0;
	VTIpeAtributo *tipo_insere, *tipo_retira;

	try
	{
		//padraoINS
		if (this->PadraoIns != NULL)
		{
			tipo_insere = ipe_grp->ExisteIpeAtributo(padraoINS, this->PadraoIns->Id, 0);
			if ((tipo_insere != NULL) && (this->LisIpeAtributo->IndexOf(tipo_insere)< 0))
			{
				this->LisIpeAtributo->Add(tipo_insere);
			}
		}
		//padraoRET
		if (this->PadraoRet != NULL)
		{
			tipo_retira = ipe_grp->ExisteIpeAtributo(padraoRET, this->PadraoRet->Id, 0);
			if ((tipo_retira != NULL) && (this->LisIpeAtributo->IndexOf(tipo_retira)< 0))
			{
				this->LisIpeAtributo->Add(tipo_retira);
			}
		}
		//ipeITEMARIES_QNTINS
		texto = DoubleToStr("%9.8f",this->QuantidadeIns);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEMARIES_QNTINS, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		//ipeITEMARIES_QNTRET
		texto = DoubleToStr("%9.8f",this->QuantidadeRet);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEMARIES_QNTRET, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		//ipeITEM_CUSTO
		texto = DoubleToStr("%9.8f",this->Custo);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_CUSTO, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}

		//salva as ifno basicas
        TItemObra::SalvaIpeAtributosProprios();
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
// eof
