//---------------------------------------------------------------------------

#pragma hdrstop
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicenca.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicencas.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
//---------------------------------------------------------------------------
#include "TAutoTag.h"
#include "TTagFormatter.h"
#include "TTaggerBase.h"
#include "TTaggerChaveManobrada.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTAutoTag* __fastcall NewObjAutoTag(VTApl *apl)
{
	try
	{
		return (new TAutoTag(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}
//---------------------------------------------------------------------------
__fastcall TAutoTag::TAutoTag(VTApl *apl)
{
	this->apl_ = apl;

	//inicializa configuracao
	cfg.enabled = false;
	cfg.tag_chave_manobra = false;
	tagger_ = NULL;
	tag_formatter_ = NULL;

}
//---------------------------------------------------------------------------
__fastcall TAutoTag::~TAutoTag(void)
{
	if (tagger_)
	{
		delete tagger_;
		tagger_ = NULL;
	}
	if (tag_formatter_)
	{
		delete tag_formatter_;
		tag_formatter_ = NULL;
	}
}
//---------------------------------------------------------------------------
VTTagFormatter* __fastcall TAutoTag::CriaFormatter(void)
{
    // variáveis locais
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl_->GetObject(__classid(VTLicencas));
	VTTagFormatter *formatter = NULL;

	// verifica licenca
	licenca = licencas->ExisteLicenca();
	if(licenca != NULL)
	{
		switch (licenca->Empresa)
		{
			case COPEL:
				formatter = NewObjTagFormatterCOPEL(apl_);
				break;
			default:
				formatter = NewObjTagFormatter(apl_);
				break;
		}

	}

	return formatter;
}
//---------------------------------------------------------------------------
VTTagger* __fastcall TAutoTag::CriaTagger(void)
{
    // variáveis locais
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl_->GetObject(__classid(VTLicencas));
	VTTagger *tagger = NULL;
	VTTagger *taggerBase = NULL;

	//cria o tagger base
	taggerBase = new TTaggerBase(apl_,this);
	// verifica licenca
	licenca = licencas->ExisteLicenca();
	//adiciona funcoes de acordo com a empresa
	if(licenca != NULL)
	{
		switch (licenca->Empresa)
		{
			case COPEL:
				//se estiver
				if (cfg.tag_chave_manobra)
				{
					tagger = new TTaggerChaveManobrada(apl_,taggerBase,tag_formatter_);
				}
				break;
			default:
				tagger = taggerBase;
				break;
		}

	}

	return tagger;
}
//---------------------------------------------------------------------------
bool __fastcall TAutoTag::Inicializa(void)
{
	//objeto que formata as tags
	if (tag_formatter_ != NULL)
	{
		delete tag_formatter_;
	}
	tag_formatter_ = CriaFormatter();

	//objeto que avalia acoes
	if (tagger_ != NULL)
	{
		delete tagger_;
	}
	tagger_  = CriaTagger();

	//inicializa
	tagger_->InicializaComentariosAutomaticos();
}
//---------------------------------------------------------------------------
bool __fastcall TAutoTag::InsereTagAcao(VTAcao *acao, bool undo)
{
	bool sucesso = true;
	VTEqpto *eqpto = NULL;
	VTAcao *acaoAux = NULL;

	//verifica o tipo de eqpto para avaliar
	try
	{   //protecao
		if (acao == NULL)
			{return false;}
		if (!cfg.enabled)
            {return false;}
		switch(acao->Tipo())
		{
			case acaoCOMPOSTA:
				//faz nada!
				break;
			default:
				if (acao->Eqpto() == NULL)
					{return false;}
				eqpto = acao->Eqpto();
				//dispara o tagger
				if (tagger_ != NULL)
				{
					sucesso &= tagger_->InsereTags(acao, undo);
				}
				break;
		}

	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TAutoTag::PM_SetChaveManobra(bool enabled)
{
	//se não muda o estado, faz nada
	if (enabled == cfg.tag_chave_manobra)
		{return;}
	else
	{
		cfg.tag_chave_manobra = enabled;
		//reinicializa o tagger
        Inicializa();
    }
}
//---------------------------------------------------------------------------
