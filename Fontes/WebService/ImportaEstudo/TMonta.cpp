//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <math.h>
#include <DateUtils.hpp>
#include "TMonta.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
VTMonta* NewObjMonta(VTApl *apl_owner)
   {
   try{//cria objeto Monta
      return(new TMonta(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMonta::TMonta(VTApl *apl_owner)
   {
   //salva ponteiros p/ objetos
   this->apl  = apl_owner;
   web.estudo = (estudo*)apl->GetObject(__classid(estudo));
   }

//---------------------------------------------------------------------------
__fastcall TMonta::~TMonta(void)
   {
   //destr�i objetos
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereAlternativa(strALTERNATIVA &str)
   {
   //vari�veis locais
   int index;

   //prote��o:
   if (web.estudo == NULL)
      {
      Erro("InsereAlternativa(): n�o existe Estudo");
      return(false);
      }
   //=======
   //IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.estudo->alternativa.Length == 0) web.estudo->alternativa = web.estudo->alternativa;
	//=======
   //cria alternativa para o estudo corrente
   index = web.estudo->alternativa.Length;
   web.estudo->alternativa.Length++;
   web.estudo->alternativa[index] = web.alternativa = new NS_ReceberEstudoWebService::alternativa();
   //inicia atributos
   web.alternativa->descricao    = str.descricao;
   web.alternativa->redeOriginal = str.rede_original;
   web.alternativa->justificativa = str.justificativa;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereDemandaPotencia(strDEMANDA_POTENCIA &str)
   {
   //vari�veis locais
   int index;
	NS_ReceberEstudoWebService::demandaPotencia *demanda_potencia;

   //prote��o:
   if (web.rede_alternativa == NULL)
      {
      Erro("InsereDemandaPotencia(): n�o existe RedeAlternativa");
      return(false);
      }
   //=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.rede_alternativa->demandaPotencia.Length == 0) web.rede_alternativa->demandaPotencia = web.rede_alternativa->demandaPotencia;
	//=======
   //cria DemandaPotencia para RedeAlternativa corrente
   index = web.rede_alternativa->demandaPotencia.Length;
   web.rede_alternativa->demandaPotencia.Length++;
   web.rede_alternativa->demandaPotencia[index] = demanda_potencia = new NS_ReceberEstudoWebService::demandaPotencia();
   //inicia atributos
   demanda_potencia->ano   = str.ano;
   demanda_potencia->valor = str.s_mva;

   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall  TMonta::InsereElementoPotencia(strELEMENTO_POTENCIA &str)
	{//DVK 2015.04.28
   //vari�veis locais
	int index;

	//prote��o:
	if (web.rede_alternativa == NULL)
		{
		Erro("InsereElementoPotencia(): n�o existe RedeAlternativa");
		return(false);
		}
	//=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
	if (web.rede_alternativa->elementoPotencia.Length == 0) web.rede_alternativa->elementoPotencia = web.rede_alternativa->elementoPotencia;
	//=======
	//cria Elemento pot�ncia para RedeAlternativa corrente
	index = web.rede_alternativa->elementoPotencia.Length;
	web.rede_alternativa->elementoPotencia.Length++;
	web.rede_alternativa->elementoPotencia[index] = web.elemento_potencia = new NS_ReceberEstudoWebService::elementoPotencia();
	//inicia atributos
	web.elemento_potencia->idExternoElementoPotencia = str.id;
	web.elemento_potencia->codigo                    = str.codigo;

	return(true);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereEquipamento(strEQUIPAMENTO &str)
   {
	//vari�veis locais
	int index;
	NS_ReceberEstudoWebService::equipamento *equipamento;

	//prote��o:
	if (web.obra == NULL)
		{
		Erro("InsereEquipamento(): n�o existe Obra");
		return(false);
		}
	//=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
	if (web.obra->equipamento.Length == 0) web.obra->equipamento = web.obra->equipamento;
	//=======
	//cria Equipamento para Obra corrente
	index = web.obra->equipamento.Length;
	web.obra->equipamento.Length++;
	web.obra->equipamento[index] = equipamento = new NS_ReceberEstudoWebService::equipamento();
	//inicia atributos
	equipamento->idPadraoConstrutivo = str.padrao_construtivo_id;
	equipamento->descricao           = str.descricao;
	equipamento->quantidade          = str.quantidade;
	equipamento->tipoAcao            = str.tipo_acao;
	//atualiza atributos da rede
	equipamento->rede                = new NS_ReceberEstudoWebService::rede();
	equipamento->rede->codigoGIS     = str.rede.codigo;
	equipamento->rede->tipoSistema   = str.rede.tipo_sistema;
	}
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15143999
bool __fastcall TMonta::InsereEquipamento(strEQUIPAMENTO &str)
   {
	//vari�veis locais
	int index;

	//prote��o:
	if (web.obra == NULL)
		{
		Erro("InsereEquipamento(): n�o existe Obra");
		return(false);
		}
	//=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
	if (web.obra->equipamento.Length == 0) web.obra->equipamento = web.obra->equipamento;
	//=======
	//cria Equipamento para Obra corrente
	index = web.obra->equipamento.Length;
	web.obra->equipamento.Length++;
	web.obra->equipamento[index] = web.equipamento = new NS_ReceberEstudoWebService::equipamento();
	//inicia atributos
	web.equipamento->idPadraoConstrutivo = str.padrao_construtivo_id;
	web.equipamento->descricao           = str.descricao;
	web.equipamento->quantidade          = str.quantidade;
	web.equipamento->tipoAcao            = str.tipo_acao;
	//reinicia n�mero de Rede do equipamento
	web.equipamento->rede.Length = 0;

	return true;
	}

//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereMunicipio(strMUNICIPIO &str)
   {
	//vari�veis locais
	int index;

	//prote��o:
	if (web.obra == NULL)
		{
		Erro("InsereMunicipio(): n�o existe Obra");
		return(false);
		}
	//=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
	if (web.obra->municipio.Length == 0) web.obra->municipio = web.obra->municipio;
	//=======
	//cria Municipio para Obra corrente
	index = web.obra->municipio.Length;
	web.obra->municipio.Length++;
	web.obra->municipio[index] = web.municipio = new NS_ReceberEstudoWebService::municipio();
	//inicia atributos
	web.municipio->principal           = str.principal;
	web.municipio->idMunicipio         = str.idMunicipio;

	return true;
	}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15630027
bool __fastcall TMonta::InsereRede(strREDE &str)
   {
   //vari�veis locais
	int index;
	NS_ReceberEstudoWebService::rede *rede;

	//prote��o:
	if (web.equipamento == NULL)
		{
		Erro("InsereRede(): n�o existe Equipamento");
		return(false);
		}
	//=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
	if (web.equipamento->rede.Length == 0) web.equipamento->rede = web.equipamento->rede;
	//=======
	//cria Rede para Equipamento corrente
	index = web.equipamento->rede.Length;
	web.equipamento->rede.Length++;
	web.equipamento->rede[index] = rede = new NS_ReceberEstudoWebService::rede();
	//atualiza atributos da rede
	rede->codigoGIS     = str.codigo;
	rede->tipoSistema   = str.tipo_sistema;
	rede->antesManobra  = str.antes_manobra;
	rede->codigoGisSubestacao  = str.codigo_rede_pai;

	return(true);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
bool __fastcall TMonta::InsereEstudo(strESTUDO &str)
{
    //atualiza atributos do estudo
    web.estudo->idDominio     = str.dominio_id;
    web.estudo->idGrupo       = str.grupo_id;
    web.estudo->usuarioSinap  = str.usuario;
    web.estudo->descricao     = str.descricao;
    web.estudo->justificativa = str.justificativa;
    //reinicia n�mero de Alternativas
    web.estudo->alternativa.Length = 0;
    return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereInformacaoTecnica(strINFORMACAO_TECNICA &str)
   {
   //vari�veis locais
   int index;

   //prote��o:
   if (web.alternativa == NULL)
      {
      Erro("InsereInformacaoTecnica(): n�o existe Alternativa");
      return(false);
      }
   //=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.alternativa->informacaoTecnica.Length == 0) web.alternativa->informacaoTecnica = web.alternativa->informacaoTecnica;
	//=======
   //cria InformacaoTecnica para Alternativa corrente
   index = web.alternativa->informacaoTecnica.Length;
   web.alternativa->informacaoTecnica.Length++;
   web.alternativa->informacaoTecnica[index] = web.informacao_tecnica = new NS_ReceberEstudoWebService::informacaoTecnicaComercial();
   //inicia atributos
   web.informacao_tecnica->tipo = str.tipo;

   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16216629
//https://sinapsisenergia.teamwork.com/#/tasks/16287949
bool __fastcall TMonta::InsereObra(strOBRA &str)
   {
   //vari�veis locais
   int index;

   //prote��o:
   if (web.alternativa == NULL)
      {
      Erro("InsereObra(): n�o existe Alternativa");
      return(false);
      }
   //=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.alternativa->obra.Length == 0) web.alternativa->obra = web.alternativa->obra;
	//=======
   //cria uma obra para alternativa corrente
   index = web.alternativa->obra.Length;
   web.alternativa->obra.Length++;
   web.alternativa->obra[index] = web.obra = new NS_ReceberEstudoWebService::obra();
   //inicia atributos
   web.obra->tipoObra          = str.tipo_obra;
   web.obra->descricao         = str.descricao;
   web.obra->obrigacaoEspecial = str.obrigacao_especial;
   web.obra->dataInicio        = str.data_inicio;
   web.obra->dataAtivacao      = str.data_ativacao;
   web.obra->participacaoOtr   = str.participacao_otr;
   web.obra->croqui            = str.croqui;
   web.obra->codigo            = str.codigo;
   web.obra->programa          = str.programa;
   web.obra->grupo             = str.grupo;
   web.obra->conjunto_obra     = str.conjunto_obra;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::InserePotenciaInstalada(strPOTENCIA_INSTALADA &str)
   {
   //vari�veis locais
   int index;
   NS_ReceberEstudoWebService::potenciaInstalada *potencia_instalada;

   //prote��o:
	if (web.elemento_potencia == NULL)
		{
		Erro("InserePotenciaInstalada(): n�o existe ElementoPotencias");
      return(false);
      }
	//=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
	if (web.elemento_potencia->potenciaInstalada.Length == 0) web.elemento_potencia->potenciaInstalada = web.elemento_potencia->potenciaInstalada;
	//=======
	//cria PotenciaInstalada para elementoPotencia corrente
	index = web.elemento_potencia->potenciaInstalada.Length;
	web.elemento_potencia->potenciaInstalada.Length++;
	web.elemento_potencia->potenciaInstalada[index] = potencia_instalada = new NS_ReceberEstudoWebService::potenciaInstalada();
	//inicia atributos
	potencia_instalada->ano   = str.ano;
   potencia_instalada->valor = str.s_mva;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereRedeAlternativa(strREDE_ALTERNATIVA &str)
   {
   //vari�veis locais
   int index;

   //prote��o:
   if (web.alternativa == NULL)
      {
      Erro("InsereRedeAlternativa(): n�o existe Alternativa");
      return(false);
      }
   //=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.alternativa->rede.Length == 0) web.alternativa->rede = web.alternativa->rede;
	//=======
   //cria RedeAlternativa para Alternativa corrente
   index = web.alternativa->rede.Length;
   web.alternativa->rede.Length++;
   web.alternativa->rede[index] = web.rede_alternativa = new NS_ReceberEstudoWebService::redeAlternativa();
   //inicia atributos
   web.rede_alternativa->codigoGIS            = str.codigoGIS;
   web.rede_alternativa->descricao            = str.descricao;
   web.rede_alternativa->tipoSistema          = str.tipoSistema;
   web.rede_alternativa->taxaCrescimentoAnual = str.taxa_crescimento_anual;

   return(true);
   }

 //---------------------------------------------------------------------------
bool __fastcall TMonta::InsereSolicitacao(strSOLICITACAO &str)
	{/*  //DVK 2015.09.28 solicita��o de acesso removida do VIP
   //vari�veis locais
   int index;
    NS_ReceberEstudoWebService::solicitacaoAcesso *solicitacao;
   //prote��o:
   if (web.obra == NULL)
      {
      Erro("InsereSolicitacao(): n�o existe Obra");
      return(false);
      }
   //=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.obra->solicitacaoAcesso.Length == 0) web.obra->solicitacaoAcesso =  web.obra->solicitacaoAcesso;
	//=======
   //cria RedeAlternativa para Alternativa corrente
   index = web.obra->solicitacaoAcesso.Length;
   web.obra->solicitacaoAcesso.Length++;
   web.obra->solicitacaoAcesso[index] = solicitacao = new NS_ReceberEstudoWebService::solicitacaoAcesso();
   //inicia atributos
   solicitacao->idExternoSolicitacaoAcesso = str.id;
   solicitacao->usuarioSinap               = str.usuario;
   solicitacao->descricao                  = str.descricao;
   solicitacao->responsavel                = str.responsavel;
   solicitacao->subestacao                 = str.subestacao;
   solicitacao->alimentador                = str.alimentador;
   solicitacao->data                       = str.data;
   solicitacao->codigoDocumento            = str.cod_doc;
   solicitacao->codigoReferencia           = str.cod_ref;
   */
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::InsereValorInformacaoTecnica(strVALOR_INFORMACAO_TECNICA &str)
   {
   //vari�veis locais
   int index;
   NS_ReceberEstudoWebService::valorInformacaoTecnicaComercial *valor_informacao_tecnica;

   //prote��o:
   if (web.informacao_tecnica == NULL)
      {
      Erro("InsereValorInformacaoTecnica(): n�o existe InformacaoTecnica");
      return(false);
      }
   //=======
	//IMPORTANTE: instru��o est�pida inclu�da para contornar erro do Builder
   if (web.informacao_tecnica->valor.Length == 0) web.informacao_tecnica->valor = web.informacao_tecnica->valor;
	//=======
   //cria ValorInformacaoTecnica para InformacaoTecnica corrente
   index = web.informacao_tecnica->valor.Length;
   web.informacao_tecnica->valor.Length++;
   web.informacao_tecnica->valor[index] = valor_informacao_tecnica = new NS_ReceberEstudoWebService::valorInformacaoTecnicaComercial();
   //inicia atributos
   valor_informacao_tecnica->ano   = str.ano;
   valor_informacao_tecnica->valor = str.valor;

	return(true);
	}

//-----------------------------------------------------------------------------
// eof



