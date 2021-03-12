// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TItemObra.h"
#include <DateUtils.hpp>
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
#include <PlataformaSinap\Fontes\Planejamento\VTIpe.h>
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
VTItemObra* NewObjItemObra(VTApl *apl)
{
	return (new TItemObra(apl));
}

// ---------------------------------------------------------------------------
__fastcall TItemObra::TItemObra(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	PD.lisIpeAtributo = new TList;
	PD.obra = NULL;
	PD.id = FalsoId();
	PD.tipoAcao = acaoINDEFINIDA;
	PD.nome = "";
	PD.configurado = false;
	PD.contabilizar = true;
//	PD.ob_esp = 0.;     //obrigação especial (%)
//	PD.part_otr = 0.;   //participação OTR (%)
//	PD.visible = false;
//	PD.fixa = false;
//	PD.incorporada = false;
//	PD.modal_result = mrCancel;
//	PD.color = clBlack;
//	PD.color_enabled = false;
	PD.altera_taxa_falha = false;
	PD.taxa_falha_insercao = 0.;
	PD.taxa_falha_substituicao =0.;
	PD.lisACAO = new TList;
}

// ---------------------------------------------------------------------------
__fastcall TItemObra::~TItemObra(void)
{
	VTAcao *acao;
	VTItemObra *item;


	if(PD.lisIpeAtributo)
	{
		delete PD.lisIpeAtributo;
		PD.lisIpeAtributo = NULL;
	}
	if(PD.lisACAO)
	{
        //percorre todas as acoes tirando o carimbo
		for (int na = 0; na < PD.lisACAO->Count; na++)
		{
			acao = (VTAcao*)PD.lisACAO->Items[na];
			acao->ItemObra = NULL;
		}
		delete PD.lisACAO;
		PD.lisACAO = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TItemObra::AssociaAcoesClone(VTItemObra *clone)
{
	TList *lisAUX;
	VTAcao *acaoOrig, *acaoClone;

	lisAUX = new TList;

	//copia a lista de acoes originais
	CopiaTList(clone->LisAcao,lisAUX);
	//limpa a lista de acoes do item
	clone->LisAcao->Clear();
	//para todas acoes orignais, adiciona o clone
	//dessa forma a cópia do item deixa de ser shallow e torna-se deep
	for (int nai = 0; nai < lisAUX->Count; nai++)
		{
		acaoOrig = (VTAcao*)lisAUX->Items[nai];
		if ((acaoOrig != NULL) && (acaoOrig->AcaoClone != NULL))
			{
				acaoClone = (VTAcao*)acaoOrig->AcaoClone;
				clone->InsereAcao(acaoClone);
				acaoOrig->AcaoClone = NULL;
			}
		}


	delete lisAUX;
}
// ---------------------------------------------------------------------------
VTItemObra* __fastcall TItemObra::Clone(bool preserveId, bool isDeep)
{
	VTItemObra *clone;
	VTIpeAtributo *atributo;

	try
	{
		clone = NewObjItemObra(apl);
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
void __fastcall  TItemObra::CorrigeValores(int tipoEqpto)
{
    PD.configurado = false;
}
// ---------------------------------------------------------------------------
void __fastcall TItemObra::CopiaAtributosDe(VTItemObra &ref)
{
	PD.obra = ref.Obra;
	PD.id = ref.Id;
	PD.tipoAcao = ref.TipoAcao;
	PD.nome = ref.Nome;
	PD.configurado = ref.Configurado;
	PD.contabilizar = ref.Contabilizar;
//	PD.ob_esp = ref.ObEspecial;     //obrigação especial (%)
//	PD.part_otr = ref.PartOtr;   //participação OTR (%)
//	PD.visible = ref.Visible;
//	PD.fixa = ref.;
//	PD.incorporada = ref.Incorporada;
//	PD.modal_result = ref.ModalResult;
//	PD.color = ref.Color;
//	PD.color_enabled = ref.ColorEnabled;
	PD.reducaoPNT_kwh_mes = ref.ReducaoPNT;
	PD.altera_taxa_falha = ref.AlteraTaxaFalha;
	PD.taxa_falha_insercao = ref.TaxaFalhaInsercao;
	PD.taxa_falha_substituicao = ref.TaxaFalhaSubstituicao;
	//copia lista de acoes?
	PD.lisACAO->Assign(ref.LisAcao, laCopy);
	//copia a lista
	this->LisIpeAtributo->Assign(ref.LisIpeAtributo, laCopy);
}
// ---------------------------------------------------------------------------
VTIpeAtributo* TItemObra::ExisteIpeAtributoSemelhante(VTIpeAtributo *atributo)
{
	VTIpeAtributo *atributoSemelhante = NULL;
	VTIpeAtributo *atributoAux = NULL;

	if (atributo != NULL)
	{
		for (int ni = 0; ni < this->LisIpeAtributo->Count; ni++)
		{
			atributoAux = (VTIpeAtributo*)this->LisIpeAtributo->Items[ni];
			//verifica se ja existe um do mesmo tipo
			if (atributo->Tipo != ipeMUNICIPIO)
			{
				if (atributo->Tipo == atributoAux->Tipo)
				{
					atributoSemelhante = atributoAux;
                    break;
				}
			}
		}
	}

	return  atributoSemelhante;
}
// ---------------------------------------------------------------------------
void __fastcall  TItemObra::IniciaExtras(void)
{
	//faz nada
}
// ---------------------------------------------------------------------------
void    __fastcall 	TItemObra::InsereAcao(VTAcao *acao)
{
	if (acao == NULL)
		{return;}
	if (this->LisAcao->IndexOf(acao) < 0)
	{
		this->LisAcao->Add(acao);
        acao->ItemObra = this;
	}
}
// ---------------------------------------------------------------------------
void    __fastcall TItemObra::InsereIpeAtributo(VTIpeAtributo *atributo)
{
	VTIpeAtributo *atributoAux = NULL;

	if (atributo != NULL)
	{
		if ((atributoAux = ExisteIpeAtributoSemelhante(atributo))!= NULL)
		{
			this->LisIpeAtributo->Remove(atributoAux);
			delete atributoAux;
		}
		this->LisIpeAtributo->Add(atributo);
		//carega pra estrutura
		LeIpeAtributo(atributo);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TItemObra::LeIpeAtributos(void)
{
	VTIpeAtributo *atributo = NULL;

	//le a lista de ipe atributos e atualiza seus atributos
	for (int na = 0; na < PD.lisIpeAtributo->Count; na++)
	{
		atributo = (VTIpeAtributo*)PD.lisIpeAtributo->Items[na];
		LeIpeAtributo(atributo);
	}
    IniciaExtras();
}
// ---------------------------------------------------------------------------
void __fastcall TItemObra::LeIpeAtributo(VTIpeAtributo *atributo)
{
	int valorInt = 0;
	int valorDouble = 0;

	try
	{
		if (atributo != NULL)
		{
			switch (atributo->Tipo)
			{
				case ipeITEM_NOME:
					this->Nome = atributo->Descricao;
					break;
				case ipeITEM_CONFIGURADO:
					valorInt = StrToInt(atributo->Descricao);
					this->Configurado = (valorInt != 0);
					break;
				case ipeITEM_CONTABILIZAR:
					valorInt = StrToInt(atributo->Descricao);
					this->Contabilizar = (valorInt != 0);
					break;
				case ipeITEM_REDUCAOPNT:
					valorDouble = StrToDouble(atributo->Descricao);
					this->ReducaoPNT = valorDouble;
					break;
				case ipeITEM_ALTERATAXAFALHA:
					valorInt = StrToInt(atributo->Descricao);
					this->AlteraTaxaFalha = (valorInt != 0);
					break;
				case ipeITEM_TAXAFALHAINS:
					valorDouble = StrToDouble(atributo->Descricao);
					this->TaxaFalhaInsercao = valorDouble;
					break;
				case ipeITEM_TAXAFALHASUB:
                    valorDouble = StrToDouble(atributo->Descricao);
					this->TaxaFalhaSubstituicao = valorDouble;
					break;
				case ipeITEM_TIPOACAO:
					valorInt = StrToInt(atributo->Descricao);
					this->TipoAcao  = valorInt;
					break;
			default:
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
void __fastcall TItemObra::MantemAtributosComunsDe(VTItemObra &ref)
{
	if (PD.obra != ref.Obra)
		{PD.obra  = NULL;}
	if (PD.id != ref.Id)
		{PD.id = -1;}
	if(PD.tipoAcao != ref.TipoAcao)
		{PD.tipoAcao = acaoINDEFINIDA;}
	if(PD.nome.AnsiCompare(ref.Nome)  == 0)
		{PD.nome = "";}
	if(PD.configurado != ref.Configurado)
		{PD.configurado = false;}
	if(PD.contabilizar != ref.Contabilizar)
		{PD.contabilizar = true;}
//	if (!IsDoubleZero(PD.ob_esp - ref.ObEspecial, 1e-3))
//		{PD.ob_esp = 0.;}
//	if (!IsDoubleZero(PD.part_otr - ref.PartOtr, 1e-3))
//		{PD.part_otr = 0.;}
//	if(PD.visible != ref.Visible)
//		{PD.visible = false;}
//	if(PD.incorporada != ref.Incorporada)
//		{PD.incorporada = false;}
//	if(PD.modal_result != ref.ModalResult)
//		{PD.modal_result = mrCancel;}
//	if(PD.color != ref.Color)
//		{PD.color = clBlack;}
//	if(PD.color_enabled != ref.ColorEnabled)
//		{PD.color_enabled = false;}
	if (!IsDoubleZero(PD.reducaoPNT_kwh_mes - ref.ReducaoPNT, 1e-3))
		{PD.reducaoPNT_kwh_mes = 0.;}
	if(PD.altera_taxa_falha != ref.AlteraTaxaFalha)
		{PD.altera_taxa_falha = false;}
	if (!IsDoubleZero(PD.taxa_falha_insercao - ref.TaxaFalhaInsercao, 1e-5))
		{PD.taxa_falha_insercao = 0.;}
	if (!IsDoubleZero(PD.taxa_falha_substituicao - ref.TaxaFalhaInsercao, 1e-5))
		{PD.taxa_falha_substituicao = 0.;}
}
// ---------------------------------------------------------------------------
void __fastcall TItemObra::SalvaIpeAtributos(void)
{
	// variaveis locais

	try
	{
		this->LisIpeAtributo->Clear();
        SalvaIpeAtributosProprios();
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
void __fastcall TItemObra::SalvaIpeAtributosProprios(void)
{
    // variaveis locais
	AnsiString texto;
	VTIpeAtributo *atributo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_grp = (VTIpeGRP*)planejamento->Ipe;
	int valorInt = 0;
	int valorDouble = 0;
	VTAcao *acao;

	try
	{
		//nome
		texto = this->Nome ;
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_NOME, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		//tipo Acao
		texto = IntToStr(this->TipoAcao);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_TIPOACAO, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		//configurado
		if (this->Configurado == true)
			{texto = "1";}
		else
			{texto = "0";}
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_CONFIGURADO, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		//contabilizar
		if (this->Contabilizar == true)
			{texto = "1";}
		else
			{texto = "0";}
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_CONTABILIZAR, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		//Reducao PNT
		texto = DoubleToStr("%9.8f",this->ReducaoPNT);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_REDUCAOPNT, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
        //altera taxa de falha
		if (this->AlteraTaxaFalha == true)
			{texto = "1";}
		else
			{texto = "0";}
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_ALTERATAXAFALHA, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		// Taxa Falha insercao
		texto = DoubleToStr("%9.8f",this->TaxaFalhaInsercao);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_TAXAFALHAINS, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
		// Taxa Falha substituicao
		texto = DoubleToStr("%9.8f",this->TaxaFalhaSubstituicao);
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeITEM_TAXAFALHASUB, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}

		//finaliza carimbando este item em todas as acoes
		for (int na = 0; na < PD.lisACAO->Count; na++)
		{
			 acao = (VTAcao*)PD.lisACAO->Items[na];
			 if (acao != NULL)
			 {
                acao->ItemObra = this;
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
// eof
