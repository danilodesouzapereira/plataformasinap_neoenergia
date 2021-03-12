//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBaseAtivo.h"
#include "VTAtivo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBaseAtivo* __fastcall NewObjBaseAtivo(void)
   {
   return(new TBaseAtivo());
   }

//---------------------------------------------------------------------------
__fastcall TBaseAtivo::TBaseAtivo(void)
   {
   //cria objetos
   lisATIVO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBaseAtivo::~TBaseAtivo(void)
   {
   //destrói lista e seus objetos
   if (lisATIVO) {LimpaTList(lisATIVO); delete lisATIVO; lisATIVO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TBaseAtivo::AlteracaoAfetaBaseAtivo(VTEqpto *eqpto_ori, VTEqpto *eqpto_alt)
   {
   //variáveis locais
   VTTrecho *trecho_ori, *trecho_alt;

   //verifica se os Eqptos envolvidos são Trechos
   if (eqpto_ori->Tipo() == eqptoTRECHO)
      {//verifica se houve troca de Arranjo
      trecho_ori = (VTTrecho*)eqpto_ori;
      trecho_alt = (VTTrecho*)eqpto_alt;
      if (trecho_ori->arranjo == trecho_alt->arranjo) return(false);
      //verifica se são Trechos de rede primária
      //if (trecho_ori->rede->TipoRede->Segmento != redePRI) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTAtivo* __fastcall TBaseAtivo::CriaAtivo(int tipo_acao, VTObra *obra, VTEqpto *eqpto)
   {
   //variáveis locais
   int       tipo_ativo;
   double    comp_m = 0;
   VTAtivo   *ativo;
   VTLigacao *ligacao;
   VTTrecho  *trecho;
   VTRede    *rede;

   //proteção: verifica se o Eqpto é de uma rede representada na base de ativos
   if ((rede = DeterminaRedeDoEqpto(eqpto)) == NULL) return(NULL);
   //proteção: verifica se o Eqpto é representado na base de ativos
   if ((tipo_ativo = DeterminaTipoDoAtivo(eqpto)) == ativoNAO_DEFINIDO) return(NULL);
   //verifica se o Eqpto é um Trecho
   if (eqpto->Tipo() == eqptoTRECHO)
      {//determina Trecho e seu comprimento
      trecho = (VTTrecho*)eqpto;
      comp_m = trecho->Comprimento_km;
      //redefine Eqpto como o Arranjo do Trecho
      eqpto = trecho->arranjo;
      //verifica se já existe um Ativo p/ o mesmo Arranjo
      if ((ativo = ExisteAtivo(tipo_acao, obra, eqpto)) != NULL)
         {//atualiza comprimento
         ativo->Comprimento_km = ativo->Comprimento_km + comp_m;
         return(ativo);
         }
      }
   //proteção: não inclui Ativo p/ um Eqpto quando existe um Ativo (mesmo tipo de ação) p/ a Rede do Eqpto
   if (ExisteAtivo(tipo_acao, obra, rede) != NULL) return(NULL);
   try{//cria novo Ativo
      if ((ativo = NewObjAtivo()) != NULL)
         {
         ativo->Enabled       = true;
         ativo->TipoRede      = rede->TipoRede->Segmento;
         ativo->TipoAcao      = tipo_acao;
         ativo->TipoAtivo     = tipo_ativo;
         ativo->Obra          = obra;
         ativo->Eqpto         = eqpto;
         ativo->RedeDoEqpto   = rede;
         ativo->Ano           = obra->AnoInicio;
         ativo->Padrao        = true;
         ativo->bdDescr       = eqpto->Padrao;
         ativo->bdId          = "";
         ativo->IA            = 0.;
         ativo->Comprimento_km = comp_m;
         //insere Ativo em lisATIVO
         lisATIVO->Add(ativo);
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(ativo);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TBaseAtivo::DeterminaRedeDaBarra(VTBarra *barra)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)alternativa->Apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteBarra(barra)) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TBaseAtivo::DeterminaRedeDoEqpto(VTEqpto *eqpto)
   {
   //variáveis locais
   VTRede *rede = NULL;

   //verifica se Eqpto é uma Rede
   if (eqpto->Tipo() == eqptoREDE)
      {
      rede = (VTRede*)eqpto;
      }
   //verifica se Eqpto é uma Ligacao
   else if (eqpto->TipoLigacao())
      {
      rede = ((VTLigacao*)eqpto)->rede;
      }
   //verifica se Eqpto é uma Ligacao Eqbar
   else if (eqpto->TipoEqbar())
      {//determina uma Rede que inclui a Barra do Eqbar
      rede = DeterminaRedeDaBarra(((VTEqbar*)eqpto)->pbarra);
      }
   //proteção
   //if (rede == NULL) return(NULL);
   //somente redePRI e redeETD são representadas na base de ativos
   //if (rede->TipoRede->Segmento == redePRI) return(rede);
   //if (rede->TipoRede->Segmento == redeETD) return(rede);
   //rede não representada na base de ativos
   return(rede);
   }

//---------------------------------------------------------------------------
int __fastcall TBaseAtivo::DeterminaTipoDoAtivo(VTEqpto *eqpto)
   {
   //variáveis locais
   VTChave   *chave;
   VTRede    *rede;
   VTLigacao *ligacao;

   switch(eqpto->Tipo())
      {
      case eqptoBARRA:     return(ativoNAO_DEFINIDO);
      case eqptoCAPACITOR: return(ativoCAPACITOR);
      case eqptoCAPSERIE:  return(ativoCAPSERIE);
      case eqptoCARGA:     return(ativoCARGA);
      case eqptoCHAVE:  //verifica o tipo da Chave
         chave = (VTChave*)eqpto;
         if (chave->TipoDisjuntor)  return(ativoCHAVE_DISJUNTOR);
         if (chave->TipoReligadora) return(ativoCHAVE_RELIGADORA);
         return(ativoCHAVE);
      case eqptoGERADOR:    return(ativoGERADOR);
      case eqptoSUPRIMENTO: return(ativoSUPRIMENTO);
      case eqptoREDE:  //verifica o tipo da Rede
         rede = (VTRede*)eqpto;
         if (rede->TipoRede->Segmento == redeETD) return(ativoREDE_SED);
         if (rede->TipoRede->Segmento == redePRI) return(ativoREDE_SDMT);
         if (rede->TipoRede->Segmento == redeSEC) return(ativoREDE_SDBT);
         return(ativoREDE);
      case eqptoREATOR:     return(ativoREATOR);
      case eqptoREGULADOR:  return(ativoREGULADOR);
      case eqptoTRAFO:      return(ativoTRAFO);
      case eqptoTRAFO3E:    return(ativoTRAFO3E);
      case eqptoTRAFOZZ:    return(ativoTRAFOZZ);
      case eqptoTRECHO:     return(ativoTRECHO_REDE);
      }
   return(ativoNAO_DEFINIDO);
   }

//---------------------------------------------------------------------------
VTAtivo* __fastcall TBaseAtivo::ExisteAtivo(int tipo_acao, VTObra *obra, VTEqpto *eqpto)
   {
   //variáveis locais
   VTAtivo *ativo;

   for (int n = 0; n < lisATIVO->Count; n++)
      {
      ativo = (VTAtivo*)lisATIVO->Items[n];
      if ((ativo->Obra      == obra      )&&
          (ativo->TipoAcao  == tipo_acao )&&
          (ativo->Eqpto     == eqpto     ))  return(ativo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TBaseAtivo::Inicia(VTAlternativa *alternativa)
   {
   //variáveis locais
   TList  *lisOBRA;
   TList  *lisACAO;
   VTObra *obra;
	VTAcao *acao, *acao_comp;

   //salva ponteiro p/ Alternativa
   this->alternativa = alternativa;
   //loop p/ todas Obras da Alternativa
	lisOBRA = alternativa->Obras->LisObra();
	//DVK 2015.09.03 limpa todos os ativos da lista
	LimpaTList(lisATIVO);
   for (int i = 0; i < lisOBRA->Count; i++)
      {
      obra = (VTObra*)lisOBRA->Items[i];
      //loop p/ todas as Acoes
      lisACAO = obra->LisAcao;
		for (int j = 0; j < lisACAO->Count; j++)
         {
			acao = (VTAcao*)lisACAO->Items[j];
			//DVK 2015.09.03 verifica o tipo de Ação
			if(acao->Tipo() == acaoCOMPOSTA)
				{//considera todas as ações da ação composta
				for(int k = 0; k < acao->LisAcao()->Count; k++)
					{
					acao_comp = (VTAcao*)acao->LisAcao()->Items[k];
					TrataTipoAcao(acao_comp, obra);
					}
            }
			else TrataTipoAcao(acao, obra);
			}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBaseAtivo::InsereAtivo(VTAtivo *ativo)
   {
   //insere Ativo em lisATIVO
   if (lisATIVO->IndexOf(ativo) < 0) lisATIVO->Add(ativo);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBaseAtivo::LisAtivo(void)
   {
   return(lisATIVO);
   }

//---------------------------------------------------------------------------
int __fastcall TBaseAtivo::NumeroDeAtivos(void)
	{
	return(lisATIVO->Count);
	}

//---------------------------------------------------------------------------
void __fastcall TBaseAtivo::TrataTipoAcao(VTAcao *acao, VTObra *obra)
	{//DVK 2015.09.03
	//proteção
	if(! acao) return;
	switch(acao->Tipo())
		{
		case acaoALTERA:
			//descarta alteração que não se reflete na base de ativo
			if (AlteracaoAfetaBaseAtivo(acao->CopiaDoEqptoOriginal(), acao->CopiaDoEqptoAlterado()))
				{//cria Ativo a ser retirado da base
				CriaAtivo(acaoRETIRA_ATIVO, obra, acao->CopiaDoEqptoOriginal());
				//cria Ativo a ser incluído na base
				CriaAtivo(acaoINSERE_ATIVO,  obra, acao->CopiaDoEqptoAlterado());
				}
			break;
		case acaoINSERE:
			//cria Ativo do tipo ativoINSERE
			CriaAtivo(acaoINSERE_ATIVO, obra, acao->CopiaDoEqptoOriginal());
			break;
		case acaoRETIRA:
			//cria Ativo a ser retirado da base
			CriaAtivo(acaoRETIRA_ATIVO, obra, acao->CopiaDoEqptoOriginal());
			break;
		default: //DVk 2015.09.03
			break;
		}
	}

//---------------------------------------------------------------------------
//eof

