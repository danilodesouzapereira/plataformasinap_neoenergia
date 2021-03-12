//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <string>
#include <iostream>
#include <fstream>
#include <System.IOUtils.hpp>
#include <System.JSON.hpp>
#include <System.JSON.Readers.hpp>
#include <System.Classes.hpp>

#include "TImportaMercado.h"
#include "VTCenario.h"
#include "VTCrescimento.h"
#include "VTMercado.h"
#include "VTSegmento.h"
#include "Struct\VTMercadoJSON.h"
#include "Struct\VTSegmentoJSON.h"
#include "Struct\VTCrescimentoJSON.h"
#include "Struct\VTTaxaJSON.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\..\ModulosOpcionais\Fontes\MercadosBatch\VTBatchMerc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTImportaMercado* __fastcall NewObjImportaMercado(VTApl *apl_owner)
   {
   try{
		return(new TImportaMercado(apl_owner));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

// ---------------------------------------------------------------------------
__fastcall TImportaMercado::TImportaMercado(VTApl *apl_owner)
{
	//salva ponteiro do Apl
	this->apl = apl_owner;
	//cria stringList
	campos = new TStringList();
	linhas = new TStringList();
	//inicia valor
	ind_taxa = 3;
}

// ---------------------------------------------------------------------------
__fastcall TImportaMercado::~TImportaMercado(void)
{
	//destroi stringList
	DestroiTObject(campos);
	DestroiTObject(linhas);
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMercado::DefineCrescimento(VTRede *rede)
{
	//variáveis locais
	bool proprio = true;
	TList *lisCRESCIMENTO;
	VTCrescimento *crescimento;
	VTSegmento *segmento;
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

	//verifica se há segmento para a rede
	if((segmento = mercado->ExisteSegmento(rede, proprio)) == NULL)
		{//cria segmento
		segmento = mercado->CriaSegmentoNova(rede);
		}
	// Segmento foi alterada
	segmento->Alterada = true;
	segmento->Enabled = true;
	//assume mesma taxa de crescimento para todas as classes
	lisCRESCIMENTO = segmento->LisCrescimento();
	for(int nc = 0; nc < lisCRESCIMENTO->Count; nc++)
	{
		crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[nc];
		DefineTaxas(crescimento);
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17673716
bool __fastcall TImportaMercado::DefineTipoSegmento(VTSegmentoJSON *segmentoJSON, AnsiString classeNome)
{
	//variáveis locais
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTClasses     *classes   = (VTClasses*) apl->GetObject(__classid(VTClasses));
	VTBatchMerc *batchMerc = (VTBatchMerc*)apl->GetObject(__classid(VTBatchMerc));
	VTRede *rede;
	VTTipoRede *tiporede;
	VTClasse *classe;
	bool sucesso = true;
	AnsiString tipoRede = "", id;

	tipoRede = segmentoJSON->Tipo;
	if (tipoRede.AnsiCompareIC("Rede")==0)
	{
		id = segmentoJSON->ExternId;
		if((segmentoJSON->ExternId).AnsiCompareIC("null") == 0 )
		{
			rede = ExisteCodigoRede(segmentoJSON->Codigo);
		}
		else
		{
			rede = ExisteRede(segmentoJSON->ExternId);
		}
		if(rede == NULL)
		{
			rede = ExisteCodigoRede(segmentoJSON->Codigo);
		}
		if(rede != NULL)
		{
			if((segmento = mercado->ExisteSegmento(rede, true)) == NULL)
			{
				//cria segmento
				segmento = mercado->CriaSegmentoNova(rede);
			}
			// Segmento foi alterada
			segmento->Alterada = true;
			segmento->Enabled = true;
			classe = classes->ExisteClasse(classeNome);
			if(classe!=NULL)
			{
					crescimento = NewObjCrescimento();
					crescimento->Classe = classe;
					segmento->InsereCrescimento(crescimento);
			}
		}
		else
		{
			//if(batchMerc!=NULL){batchMerc->LogAdd("A rede " + segmentoJSON->ExternId + " não existe.");}
			sucesso = false;
        }
	}
	else if(tipoRede.AnsiCompareIC("Tipo de rede")==0)
	{
		tiporede = ExisteTipoRede(segmentoJSON->Codigo);
		if(tiporede != NULL)
		{
			if((segmento = mercado->ExisteSegmento(tiporede)) == NULL)
			{
				//cria segmento
				segmento = mercado->CriaSegmentoNova(tiporede);
			}
            // Segmento foi alterada
			segmento->Alterada = true;
			segmento->Enabled = true;
			classe = classes->ExisteClasse(classeNome);
			if(classe!=NULL)
			{
					crescimento = NewObjCrescimento();
					crescimento->Classe = classe;
					segmento->InsereCrescimento(crescimento);
			}
		}
	}
	return(sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMercado::DefineTaxasV2(VTCrescimentoJSON *crescimentoJSON)
{
	//variáveis locais
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	double taxa = 0.;
	int ano;
	TList * lisTAXAS;

	lisTAXAS = crescimentoJSON->Taxa;

	for(int n = 0; n < lisTAXAS->Count; n++)
	{
		taxaJSON = (VTTaxaJSON*)lisTAXAS->Items[n];
		ano = taxaJSON->Ano;
		taxa = taxaJSON->Valor_Percent;
		//define a taxa para o ano
		crescimento->Taxa_perc[ano] = taxa;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMercado::DefineTaxas(VTCrescimento *crescimento)
{
	//variáveis locais
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	double taxa = 0.;
	int ind_ano;

	//varre o horizonte de planejamento
	for(int ano = mercado->AnoInicial; ano < mercado->AnoFinal; ano++)
	{
		ind_ano = ano - mercado->AnoInicial;
		taxa = StrToDouble(campos->Strings[ind_taxa + ind_ano]);
		//define a taxa para o ano
		crescimento->Taxa_perc[ano] = taxa;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17789343
bool __fastcall TImportaMercado::Executa(AnsiString arq_mercado)
{
    // ARQUIVO COPEL - VERSÃO 1
    //o layout do arquivo contém 14 campos:
    //NUM_ALIM_TXCA       VAL_DEMAN_ATIVA_TXCA       VAL_DEMAN_KVAR_TXCA ...
    //TAXA_CRESC_PERC_1_TXCA TAXA_CRESC_PERC_2_TXCA TAXA_CRESC_PERC_3_TXCA TAXA_CRESC_PERC_4_TXCA ...
    //TAXA_CRESC_PERC_5_TXCA TAXA_CRESC_PERC_6_TXCA TAXA_CRESC_PERC_7_TXCA TAXA_CRESC_PERC_8_TXCA ...
    //TAXA_CRESC_PERC_9_TXCA TAXA_CRESC_PERC_10_TXCA       INDIC_SIT_TXCA

    //variáveis locais
    AnsiString linha;
    VTRede *rede;
    int count = 0;
    int index_E;

    //limpa linhas
    linhas->Clear();
    //verifica se o arquivo existe
    if(!FileExists(arq_mercado))
    {
        if (!BatchMode)
        {
            Aviso("O arquivo selecionado não existe");
        }
        return false;
    }
    //monta stringList com as linhas do arquivo
    linhas->LoadFromFile(arq_mercado);
    for(int nl = 0; nl < linhas->Count; nl++)
    {
        linha = linhas->Strings[nl];
        //verifica cabeçalho
        if(linha.AnsiPos("INDIC_SIT_TXCA") > 0)
            continue;
        //lê campos (mínimo 5: NUM_ALIM_TXCA,VAL_DEMAN_ATIVA_TXCA,VAL_DEMAN_KVAR_TXCA,TAXA_CRESC_PERC_1_TXCA,INDIC_SIT_TXCA)
        campos->Clear();
        if(ExtraiStrings(linha, "|", campos) < 5)
            continue;
        //verifica se o campo INDIC_SIT_TXCA indica E (efetivado)
        index_E = campos->Count - 1;
        if(campos->Strings[index_E].CompareIC("E") != 0)
            continue;
        //localiza rede
        if((rede = ExisteRede(campos->Strings[0])) == NULL)
            continue;
        //altera, se necessário, o ano final do mercado
        ValidaAnoFinal();
        //associa taxas de crescimento à rede
        DefineCrescimento(rede);
        //incrementa quantidade de redes com taxas de crescimento
        count++;
    }
    //verifica se houve redes com taxas de crescimento
    if(count == 0)
    {
        if (!BatchMode)
        {
            Aviso("Nenhuma rede teve sua taxa de crescimento importada");
        }
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17673716
//https://sinapsisenergia.teamwork.com/#/tasks/17789343
//https://sinapsisenergia.teamwork.com/#/tasks/17803533
bool __fastcall TImportaMercado::ExecutaV2(AnsiString arq_mercado)
{
    // ARQUIVO SINAP - VERSÃO 2 (Realizado para EDP)

    //Variaveis locais
    VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
    bool sucesso = true;
    int numSegmentos = 0;
    TJSONObject *jRoot, *jObjMercado;
    TJSONValue *jMercado, *jSegmento, *jSegmentoValue;
    TJSONArray *jSegmentoArray;
    int ano_ini, ano_fim;
    TStringList *linhasArq;
    VTMercadoJSON *mercadoJSON;
    int segmentos;

    try
    {//verifica se o arquivo existe
        if(!FileExists(arq_mercado))
        {
            if (!BatchMode)
            {
                Aviso("O arquivo selecionado não existe");
            }
            return(false);
        }

        //ifstream arquivoEntrada(arq_mercado.c_str(), ios::in);
    //	if((!arquivoEntrada) && (!BatchMode))
    //	{
    //	   Aviso("Erro ao abrir o arquivo");
    //	   return;
    //	}
        // Cria Lista de linhas
        linhasArq = new TStringList();
        // leitura do arquivo
        linhasArq->LoadFromFile(arq_mercado);

        //Cria mercadoJSON
        mercadoJSON = NewObjMercadoJSON(apl);

        //criação objeto JSON
        jRoot = (TJSONObject*)TJSONObject::ParseJSONValue(linhasArq->Text);

        //verifica se existe "mercado"
        if((jMercado = jRoot->Values["mercado"]) != NULL)
        {
            jObjMercado = dynamic_cast<TJSONObject*>(jMercado);
        }
        else
        {
            jObjMercado = jRoot;
        }
        //determina período
        if(jObjMercado->Values["ano_ini"] == NULL)
        {
            if (!BatchMode)
            {
                Aviso("Campo com ano inicial inválido");
            }
            delete linhasArq;
            delete mercadoJSON;
            return(false);
        }
        mercadoJSON->AnoInicial = StrToInt(jObjMercado->Values["ano_ini"]->Value());
        if(jObjMercado->Values["ano_fim"] == NULL)
        {
            if (!BatchMode)
            {
                Aviso("Campo com ano final inválido");
            }
            delete linhasArq;
            delete mercadoJSON;
            return(false);
        }
        mercadoJSON->AnoFinal = StrToInt(jObjMercado->Values["ano_fim"]->Value());
        //segmentos = mercadoJSON->SegmentoJSON->Count;
        // validação dos anos inseridos pelo usuário no arquivo
        if(!ValidaAnosHorizonte(mercadoJSON))
        {
            if (!BatchMode)
            {
                Aviso("O ano inicial é maior que o ano final");
            }
            delete linhasArq;
            delete mercadoJSON;
            return(false);
        }
        else
        {
        // redefine ano inicial e final do mercado
        mercado->RedefinePeriodo(mercadoJSON->AnoInicial, mercadoJSON->AnoFinal);
        }
        //
        jSegmentoValue = jObjMercado->Values["segmento"];
        if(jSegmentoValue == NULL)
        {
            if (!BatchMode)
            {
                Aviso("Campo com segmento inválido");
            }
            delete linhasArq;
            delete mercadoJSON;
            return false;
        }
        jSegmentoArray = dynamic_cast<TJSONArray*>(jSegmentoValue);

        for(int ns=0; ns < jSegmentoArray->Count; ns++)
        {
            segmentos =  jSegmentoArray->Count;
            jSegmento = jSegmentoArray->Items[ns];
            if(LeituraSegmento(jSegmento, mercadoJSON))
                numSegmentos++;
        }
        if(numSegmentos < jSegmentoArray->Count)
        {
            if (!BatchMode)
            {
                Aviso("Quantidade de segmentos inválida");
            }
            sucesso = false;
        }
       //	delete jRoot;
        delete linhasArq;
        delete mercadoJSON;
    }
    catch(...)
    {
        if (!BatchMode)
            {
                Aviso("Arquivo JSON inválido");
            }
        sucesso = false;
        if(linhasArq)
            delete linhasArq;
        if(mercadoJSON)
            delete mercadoJSON;
    }
    return(sucesso);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TImportaMercado::ExisteCodigoRede(AnsiString codigo_rede)
{
   //variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDES = redes->LisRede();

	//localiza pelo extern_id
	for(int nr = 0; nr < lisREDES->Count; nr++)
	{
		rede = (VTRede*)lisREDES->Items[nr];
		//somente redes do tipo SDMT
		if(rede->TipoRede->Segmento != redePRI)
			continue;
		if(rede->Codigo.AnsiCompareIC(codigo_rede) == 0)
			return(rede);
    }
	//localiza pelo código
	rede = redes->ExisteRede(codigo_rede);
	return(rede);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TImportaMercado::ExisteRede(AnsiString num_rede)
{
	//variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDES = redes->LisRede();

	//localiza pelo extern_id
	for(int nr = 0; nr < lisREDES->Count; nr++)
	{
		rede = (VTRede*)lisREDES->Items[nr];
		//somente redes do tipo SDMT
		if(rede->TipoRede->Segmento != redePRI)
			continue;
		if(rede->Extern_id.AnsiCompareIC(num_rede) == 0)
			return(rede);
	}
	//localiza pelo id
	rede = redes->ExisteRede(StrToInt(num_rede));
	return(rede);
}

// ---------------------------------------------------------------------------
VTTipoRede* __fastcall TImportaMercado::ExisteTipoRede(AnsiString tipo_rede)
{
	// variáveis locais
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTTipoRede *tipo_de_rede;
	TList *lisTIPOSREDES = tipos->LisTipoRede();

	for(int nt = 0; nt < lisTIPOSREDES->Count; nt++)
	{
		 tipo_de_rede = (VTTipoRede*)lisTIPOSREDES->Items[nt];
		// Compara codigo dos tipos de rede
		if(tipo_de_rede->Codigo.AnsiCompareIC(tipo_rede) == 0)
			return(tipo_de_rede); 
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17803533
bool __fastcall TImportaMercado::LeituraCrescimento(TJSONValue* jCrescimento,  VTSegmentoJSON *segmentoJSON)
{
	// Variáveis locais
	bool sucesso = true;
	VTClasse *classe;
	TJSONObject *jObjCrescimento;
	TJSONArray *jTaxaArray;
	TJSONValue *jTaxa, *jTaxaValue;
	VTCrescimentoJSON *crescimentoJSON;
	TList *lisCRESCIMENTO;
	AnsiString nome_classe, classe_JSON;

	jObjCrescimento = dynamic_cast<TJSONObject*>(jCrescimento);
	crescimentoJSON = NewObjCrescimentoJSON();

	segmentoJSON->InsereCrescimento(crescimentoJSON);

    if(jObjCrescimento->Values["classe"] == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo classe do crescimento inválido");
        }
        return false;
    }
	crescimentoJSON->Classe = jObjCrescimento->Values["classe"]->Value();
	classe_JSON = crescimentoJSON->Classe;
	//
	if(!DefineTipoSegmento(segmentoJSON, classe_JSON))
	{
	   return(false);
	}

	jTaxaValue = jObjCrescimento->Values["taxa"];
    if(jTaxaValue == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo taxa do crescimento inválido");
        }
        return false;
    }

	jTaxaArray = dynamic_cast<TJSONArray*>(jTaxaValue);

	for(int ns=0; ns < jTaxaArray->Count; ns++)
	{
		jTaxa = jTaxaArray->Items[ns];
		if(!LeituraTaxa(jTaxa, crescimentoJSON)){sucesso = false;}
	}

	if(segmento!=NULL)
	{
		lisCRESCIMENTO = segmento->LisCrescimento();
		for(int nc = 0; nc < lisCRESCIMENTO->Count; nc++)
		{
			crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[nc];
			classe = (VTClasse*)crescimento->Classe;
			if(classe_JSON.AnsiCompareIC("Todas") == 0)
			{
				 //crescimento = NewObjCrescimento();
				 //segmento->InsereCrescimento(crescimento);
                 DefineTaxasV2(crescimentoJSON);
			}
			else
			{
				nome_classe = classe->Codigo;
				if(nome_classe.AnsiCompareIC(classe_JSON) == 0)
				{
					DefineTaxasV2(crescimentoJSON);
				}
				else{continue;}
			}
		}
	}
    return(sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17803533
bool __fastcall TImportaMercado::LeituraSegmento(TJSONValue* jSegmento, VTMercadoJSON *mercadoJSON)
{
	// Variáveis locais
	bool sucesso = true;
	TJSONObject *jObjSegmento;
	TJSONArray *jCrescimentoArray;
	TJSONValue *jCrescimento, *jCrescimentoValue;
	VTSegmentoJSON *segmentoJSON;

	jObjSegmento = dynamic_cast<TJSONObject*>(jSegmento);
	segmentoJSON = NewObjSegmentoJSON();
	mercadoJSON->InsereSegmentoJSON(segmentoJSON);

    if(jObjSegmento->Values["tipo"] == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo com tipo de segmento inválido");
        }
        return false;
    }
	segmentoJSON->Tipo = jObjSegmento->Values["tipo"]->Value();
    if(jObjSegmento->Values["codigo"] == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo com código do segmento inválido");
        }
        return false;
    }
	segmentoJSON->Codigo = jObjSegmento->Values["codigo"]->Value();
    if(jObjSegmento->Values["id_unico"] == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo com ID do segmento inválido");
        }
        return false;
    }
	segmentoJSON->ExternId = jObjSegmento->Values["id_unico"]->Value();
	//
	jCrescimentoValue = jObjSegmento->Values["crescimento"];
    if(jCrescimentoValue == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo com crescimento inválido");
        }
        return false;
    }
	jCrescimentoArray = dynamic_cast<TJSONArray*>(jCrescimentoValue);

	for(int ns=0; ns < jCrescimentoArray->Count; ns++)
	{
		jCrescimento = jCrescimentoArray->Items[ns];
		if(!LeituraCrescimento(jCrescimento, segmentoJSON))
        {
            sucesso = false;
        }
	}
	return(sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17803533
bool __fastcall TImportaMercado::LeituraTaxa(TJSONValue* jTaxa, VTCrescimentoJSON *crescimentoJSON)
{
	// Variáveis locais
	bool sucesso = true;
	TJSONObject *jObjTaxa;

	jObjTaxa = dynamic_cast<TJSONObject*>(jTaxa);
	taxaJSON = NewObjTaxaJSON();

	crescimentoJSON->InsereTaxa(taxaJSON);
    if(jObjTaxa->Values["ano"] == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo com ano da taxa inválido");
        }
        return false;
    }
	taxaJSON->Ano = StrToInt(jObjTaxa->Values["ano"]->Value());

    if(jObjTaxa->Values["valor"] == NULL)
    {
        if(!BatchMode)
        {
            Aviso("Campo com valor da taxa inválido");
        }
        return false;
    }
	taxaJSON->Valor_Percent = StrToDouble(jObjTaxa->Values["valor"]->Value());

	return(sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TImportaMercado::ValidaAnoFinal(void)
{
	//variáveis locais
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	int ano_final_arq, ano_inicial;

	//guarda ano inicial
	ano_inicial = mercado->AnoInicial;
	//determina ano final do arquivo, considerando que os "ind_taxa" primeiros campos e o último campo
	//não contém as taxas de crescimento
	ano_final_arq = campos->Count - ind_taxa - 1;
	ano_final_arq += mercado->AnoInicial;
	//verifica se o ano final do arquivo é maior que o ano final do mercado
	if(ano_final_arq > mercado->AnoFinal)
	{
		mercado->RedefinePeriodo(ano_inicial, ano_final_arq);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaMercado::ValidaAnosHorizonte(VTMercadoJSON *mercadoJSON)
{
	// Variáveis locais
	VTBatchMerc *batchMerc = (VTBatchMerc*)apl->GetObject(__classid(VTBatchMerc));
	int ano_inicial_arq, ano_final_arq;
	bool sucesso = true;

	//guarda ano inicial e final
	ano_inicial_arq = mercadoJSON->AnoInicial;
	ano_final_arq = mercadoJSON->AnoFinal;
	if(ano_inicial_arq > ano_final_arq)
	{
		sucesso = false;
		if(batchMerc!=NULL){batchMerc->LogAdd("O ano inicial é maior que o ano final.");}
    }
	return(sucesso);
}

// ---------------------------------------------------------------------------
