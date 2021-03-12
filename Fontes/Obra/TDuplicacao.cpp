//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDuplicacao.h"
#include "VTAcao.h"
#include "VTObra.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTTrafo.h"
//#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TDuplicacao::TDuplicacao(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl  = apl;
   redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //cria lista
   lisCLONE = new TList();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15532355
__fastcall TDuplicacao::~TDuplicacao(void)
   {
   //redefine a barra inicial das redes suspeitas
   //RedefineBarraIni();
   //destrói lista sem destruir seus objetos
   if (lisCLONE) {delete lisCLONE; lisCLONE = NULL;}
   }
//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::Duplica(VTObra *obra, VTAcao *acao)
   {
   //salva ponteiro p/ Obra
   this->obra = obra;
   //duplica a Acao
   return(Duplica(acao));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
VTAcao* __fastcall  TDuplicacao::Duplica(VTAcao *acao)
   {//variáveis locais
   VTAcao *acao_clone = NULL;

   //verifica o tipo de Acao
   switch (acao->Tipo())
      {
	  case acaoALTERA:
		acao_clone = DuplicaAcaoAltera(acao);
		break;
	  case acaoCOMPOSTA:
		acao_clone = DuplicaAcaoComposta(acao);
		break;
	  case acaoINSERE:
		acao_clone = DuplicaAcaoInsere(acao);
		break;
	  case acaoRETIRA:
		acao_clone = DuplicaAcaoRetira(acao);
		break;
	  default:
		acao_clone = NULL;
		break;
	  }
	  if (acao_clone != NULL)
		acao_clone->Index = acao->Index;

    //associa acao ao clone
	acao->AcaoClone = acao_clone;
   return(acao_clone);
   }


//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAltera(VTAcao *acao)
   {
   //tipos básicos
   if (acao->Eqpto()->TipoBarra())   return(DuplicaAcaoAlteraBarra(acao));
   if (acao->Eqpto()->TipoEqbar())   return(DuplicaAcaoAlteraEqbar(acao));
   if (acao->Eqpto()->TipoLigacao()) return(DuplicaAcaoAlteraLigacao(acao));
   //tipos específicos
   switch(acao->Eqpto()->Tipo())
      {
      case eqptoREDE:  return(DuplicaAcaoAlteraRede(acao));
      case eqptoMUTUA: return(DuplicaAcaoAlteraMutua(acao));
      default:         return(NULL);
      }
   }
/*
//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAlteraBarra(VTAcao *acao)
   {
   //variáveis locais
   VTBarra *barra_original = (VTBarra*)acao->Eqpto();
   VTBarra *barra_alterada = (VTBarra*)acao->CopiaDoEqptoAlterado();

   //determina Barra correspondente
   if ((barra_original = ExisteBarra(barra_original)) == NULL) return(NULL);
   //cria Clone da Barra alterada
   barra_alterada = barra_alterada->Clone();
   //cria uma nova AcaoAltera
   return(NewObjAcaoAltera(barra_original, barra_alterada));
   }
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16335852
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAlteraBarra(VTAcao *acao)
   {
   //variáveis locais
   VTBarra *barra          = (VTBarra*)acao->Eqpto();
   VTBarra *barra_original = (VTBarra*)acao->CopiaDoEqptoOriginal();
   VTBarra *barra_alterada = (VTBarra*)acao->CopiaDoEqptoAlterado();

   //determina Barra correspondente
   if ((barra = ExisteBarra(barra)) == NULL) return(NULL);
   //cria Clone da cópia da Barra original
   barra_original = barra_original->Clone();
   //cria Clone da cópia da Barra alterada
   barra_alterada = barra_alterada->Clone();
   //redefine Obra associada
   barra_original->Obra = obra;
   barra_alterada->Obra = obra;
   //cria uma nova AcaoAltera
   return(NewObjAcaoAltera(barra, barra_alterada, barra_original));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16335852
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAlteraEqbar(VTAcao *acao)
   {
   //variáveis locais
   VTEqbar *eqbar          = (VTEqbar*)acao->Eqpto();
   VTEqbar *eqbar_original = (VTEqbar*)acao->CopiaDoEqptoOriginal();
   VTEqbar *eqbar_alterado = (VTEqbar*)acao->CopiaDoEqptoAlterado();

   //determina Eqbar correspondente
   if ((eqbar = ExisteEqbar(eqbar)) == NULL) return(NULL);
   //cria Clone da cópia do Eqbar original e redefine sua Barra, se necessário
   eqbar_original = eqbar_original->Clone();
   eqbar_original->pbarra = ExisteBarra(eqbar_original->pbarra);
   //cria Clone da cópia do Eqbar alterado e redefine sua Barra, se necessário
   eqbar_alterado = eqbar_alterado->Clone();
   eqbar_alterado->pbarra = ExisteBarra(eqbar_alterado->pbarra);
   //redefine Obra associada
   eqbar_original->Obra = obra;
   eqbar_alterado->Obra = obra;
   //cria uma nova AcaoAltera
   return(NewObjAcaoAltera(eqbar, eqbar_alterado, eqbar_original));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16335852
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAlteraLigacao(VTAcao *acao)
   {
   //variáveias locais
   VTLigacao *ligacao          = (VTLigacao*)acao->Eqpto();
   VTLigacao *ligacao_original = (VTLigacao*)acao->CopiaDoEqptoOriginal();
   VTLigacao *ligacao_alterada = (VTLigacao*)acao->CopiaDoEqptoAlterado();

   //determina Ligacao correspondente
   if ((ligacao = ExisteLigacao(ligacao)) == NULL) return(NULL);
   //cria Clone da cópia da Ligacao original e redefine sua Rede e suas Barras, se necessário
   ligacao_original          = ligacao_original->Clone();
   ligacao_original->rede    = ExisteRede(ligacao_original->rede);
   ligacao_original->pbarra1 = ExisteBarra(ligacao_original->pbarra1);
   ligacao_original->pbarra2 = ExisteBarra(ligacao_original->pbarra2);
   ligacao_original->pbarra3 = ExisteBarra(ligacao_original->pbarra3);
   //cria Clone da cópia da Ligacao alterada e redefine sua Rede e suas Barras, se necessário
   ligacao_alterada          = ligacao_alterada->Clone();
   ligacao_alterada->rede    = ExisteRede(ligacao_alterada->rede);
   ligacao_alterada->pbarra1 = ExisteBarra(ligacao_alterada->pbarra1);
   ligacao_alterada->pbarra2 = ExisteBarra(ligacao_alterada->pbarra2);
   ligacao_alterada->pbarra3 = ExisteBarra(ligacao_alterada->pbarra3);
   //redefine Obra associada
   ligacao_original->Obra = obra;
   ligacao_alterada->Obra = obra;
   //verifica casos específicos
   if (ligacao->Tipo() == eqptoREGULADOR)
      {
      //VTRegulador *regulador          = (VTRegulador*)ligacao;
      VTRegulador *regulador_original     = (VTRegulador*)ligacao_original;
      VTRegulador *regulador_alterado     = (VTRegulador*)ligacao_alterada;
      //redefine barra de referência para sentido do fluxo
      regulador_original->sentido.bar_ref = ExisteBarra(regulador_original->sentido.bar_ref);
      regulador_alterado->sentido.bar_ref = ExisteBarra(regulador_alterado->sentido.bar_ref);
      //redefine barra de referência para ajuste automático
      regulador_original->cfg_auto.bar_ref = ExisteBarra(regulador_original->cfg_auto.bar_ref);
      regulador_alterado->cfg_auto.bar_ref = ExisteBarra(regulador_alterado->cfg_auto.bar_ref);
      }
   else if (ligacao->Tipo() == eqptoTRAFO)
      {
      //VTTrafo *trafo        = (VTTrafo*)ligacao;
      VTTrafo *trafo_original = (VTTrafo*)ligacao_original;
      VTTrafo *trafo_alterado = (VTTrafo*)ligacao_alterada;
      //redefine barra de referência para ajuste automático
		trafo_original->ltc.ajuste_auto.bar_ref = ExisteBarra(trafo_original->ltc.ajuste_auto.bar_ref);
		trafo_alterado->ltc.ajuste_auto.bar_ref = ExisteBarra(trafo_alterado->ltc.ajuste_auto.bar_ref);
      }
   //cria uma nova AcaoAltera
   return(NewObjAcaoAltera(ligacao, ligacao_alterada, ligacao_original));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAlteraMutua(VTAcao *acao)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16335852
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoAlteraRede(VTAcao *acao)
   {
   //variáveis locais
   VTRede  *rede          = (VTRede*)acao->Eqpto();
   VTRede  *rede_original = (VTRede*)acao->CopiaDoEqptoOriginal();
   VTRede  *rede_alterada = (VTRede*)acao->CopiaDoEqptoAlterado();

   //determina Rede correspondente
   if ((rede = ExisteRede(rede)) == NULL) return(NULL);
   //cria Clone da Rede original e redefine sua Barra inicial
   rede_original = rede_original->Clone();
   rede_original->DefineBarraInicial(ExisteBarra(rede_original->BarraInicial()));
   //cria Clone da Rede alterada e redefine sua Barra inicial
   rede_alterada = rede_alterada->Clone();
   rede_alterada->DefineBarraInicial(ExisteBarra(rede_alterada->BarraInicial()));
   //redefine Obra associada
   rede_original->Obra = obra;
   rede_alterada->Obra = obra;
   //cria uma nova AcaoAltera
   return(NewObjAcaoAltera(rede, rede_alterada, rede_original));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoComposta(VTAcao *acao_composta)
   {
   //variáveis locais
   VTAcao *acao_nova, *acao;
   TList *lisACAO = acao_composta->LisAcao();

   //cria uma nova Acao composta
   acao_nova = NewObjAcaoComposta();
   //loop p/ todas Acoes da Acao composta
   for (int n = 0; n < lisACAO->Count; n++)
      {
      acao = (VTAcao*)lisACAO->Items[n];
	  //duplica a Acao
	  if ((acao = Duplica(acao)) == NULL) return(NULL);
      //associa Acao nova com Obra
	  acao->Obra = obra;
      //insere Acao na Acao composta
	  acao_nova->Add(acao);
      }
   return(acao_nova);
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoInsere(VTAcao *acao)
   {
   //tipos básicos
   if (acao->Eqpto()->TipoBarra())   return(DuplicaAcaoInsereBarra(acao));
   if (acao->Eqpto()->TipoEqbar())   return(DuplicaAcaoInsereEqbar(acao));
   if (acao->Eqpto()->TipoLigacao()) return(DuplicaAcaoInsereLigacao(acao));
   //tipos específicos
   switch(acao->Eqpto()->Tipo())
      {
      case eqptoREDE:  return(DuplicaAcaoInsereRede(acao));
      case eqptoMUTUA: return(DuplicaAcaoInsereMutua(acao));
      default:         return(NULL);
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15532355
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoInsereBarra(VTAcao *acao)
   {
   //variáveias locais
   VTBarra *clone;
   VTBarra *barra = (VTBarra*)acao->Eqpto();


   //cria Clone da Barra
   clone  = barra->Clone();
   //verifica se a barra original é barra inicial de uma rede inserida
//   RedefineBarraIni(barra,clone);
   //redefine Obra associada ao Eqpto
   clone->Obra = obra;
   //associa a Barra nova com a Barra original
   clone->Obj = barra;
   //salva a Barra nova em lisCLONE
   if(lisCLONE->IndexOf(clone) <0)
   {
   	  lisCLONE->Add(clone);
   }
   //cria uma nova AcaoInsere (OBS: o construtor da AcaoInsere redefine o ID do Eqpto)
   return(NewObjAcaoInsere(clone));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoInsereEqbar(VTAcao *acao)
   {
   //variáveias locais
   VTBarra *barra;
   VTEqbar *clone;
   VTEqbar *eqbar = (VTEqbar*)acao->Eqpto();

   //determina Barra do Eqbar
   if ((barra = ExisteBarra(eqbar->pbarra)) == NULL) return(NULL);
   //cria Clone do Eqbar
   clone  = eqbar->Clone();
   //redefine Obra associada ao Eqpto
   clone->Obra = obra;
   //associa Eqbar novo com Eqbar original
   clone->Obj = eqbar;
   //redefine Barra do Eqbar
   clone->pbarra = barra;
   //salva Eqbar novo em lisCLONE
   lisCLONE->Add(clone);
   //cria uma nova AcaoInsere
   return(NewObjAcaoInsere(clone));
   }

//https://sinapsisenergia.teamwork.com/#/tasks/16228636
//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoInsereLigacao(VTAcao *acao)
   {
   //variáveias locais
   VTRede    *rede;
   VTBarra   *barra[NBAR_LIG];
   VTLigacao *clone;
   VTLigacao *ligacao = (VTLigacao*)acao->Eqpto();

   //determina Rede da Ligacao
   if ((rede = ExisteRede(ligacao->rede)) == NULL) return(NULL);
   //determina Barras da Ligacao
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra[ind_bar] = ligacao->Barra(ind_bar))     == NULL) continue;
	  if ((barra[ind_bar] = ExisteBarra(barra[ind_bar])) == NULL) return(NULL);
      }
   //cria Clone da Ligacao
   clone = ligacao->Clone();
   //redefine Obra associada
   clone->Obra = obra;
   //associa Ligacao nova com Ligacao original
   clone->Obj = ligacao;
   //redefine Rede da Ligacao (SEM inserir a Ligacao na Rede)
   clone->rede = rede;
   //redefine Barras da Ligacao nova
   clone->pbarra1 = barra[0];
   clone->pbarra2 = barra[1];
   clone->pbarra3 = barra[2];
   //verifica casos específicos
   if (ligacao->Tipo() == eqptoREGULADOR)
      {
      //VTRegulador *regulador          = (VTRegulador*)ligacao;
	  VTRegulador *regulador_clone      = (VTRegulador*)clone;
	  regulador_clone->sentido.bar_ref  = ExisteBarra(regulador_clone->sentido.bar_ref);
	  regulador_clone->cfg_auto.bar_ref = ExisteBarra(regulador_clone->cfg_auto.bar_ref);
	  regulador_clone->cfg_auto.bar_ref_inv = ExisteBarra(regulador_clone->cfg_auto.bar_ref_inv);

      }
   else if (ligacao->Tipo() == eqptoTRAFO)
      {
      //VTTrafo *trafo        = (VTTrafo*)ligacao;
      VTTrafo *trafo_clone    = (VTTrafo*)clone;
	  trafo_clone->ltc.ajuste_auto.bar_ref = ExisteBarra(trafo_clone->ltc.ajuste_auto.bar_ref);
      }
   //salva Ligacao nova em lisCLONE
   lisCLONE->Add(clone);
   //cria uma nova AcaoInsere
   return(NewObjAcaoInsere(clone));
   }
   
//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoInsereMutua(VTAcao *acao)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
//https://sinapsisenergia.teamwork.com/#tasks/15532355
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoInsereRede(VTAcao *acao)
   {
   //variáveias locais
   VTBarra *barra, *barra_ini;
   VTRede  *clone;
   VTRede  *rede = (VTRede*)acao->Eqpto();

   //determina Barra inicial da Rede
   barra = ExisteBarra(barra_ini = rede->BarraInicial());
   //cria Clone da Rede
   clone  = rede->Clone();
//   if ((barra_ini != NULL) && (barra_ini == barra))
//   {
//		mmapRedeParaRevisar.insert (std::pair<VTBarra*,VTRede*>(barra,clone));
//		clone->DefineBarraInicial(NULL);
//   }
//   else
//   {
//		//redefine Barra inicial
//		clone->DefineBarraInicial(barra);
//   }
   //redefine Barra inicial
   clone->DefineBarraInicial(barra);
   //redefine Obra associada
   //rede->Obra = obra;  ==> BUG 2014.03.12
   clone->Obra = obra;
   //associa Rede nova com Rede original
   clone->Obj = rede;
   //salva Rede nova em lisCLONE
   if(lisCLONE->IndexOf(clone) <0)
   {
	  lisCLONE->Add(clone);
   }
   //cria uma nova AcaoInsere
   return(NewObjAcaoInsere(clone));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoRetira(VTAcao *acao)
   {
   //tipos básicos
   if (acao->Eqpto()->TipoBarra())   return(DuplicaAcaoRetiraBarra(acao));
   if (acao->Eqpto()->TipoEqbar())   return(DuplicaAcaoRetiraEqbar(acao));
   if (acao->Eqpto()->TipoLigacao()) return(DuplicaAcaoRetiraLigacao(acao));
   //tipos específicos
   switch(acao->Eqpto()->Tipo())
      {
      case eqptoREDE:  return(DuplicaAcaoRetiraRede(acao));
	  case eqptoMUTUA: return(DuplicaAcaoRetiraMutua(acao));
	  case eqptoBARRA: return(DuplicaAcaoRetiraBarra(acao));
      default:         return(NULL);
      }
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoRetiraBarra(VTAcao *acao)
   {
   //variáveias locais
   VTBarra *barra = (VTBarra*)acao->Eqpto();

   //determina Barra
   if ((barra = ExisteBarra(barra)) == NULL) return(NULL);
   //cria uma nova AcaoRetira
   return(NewObjAcaoRetira(barra));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoRetiraEqbar(VTAcao *acao)
   {
   //variáveias locais
   VTEqbar *eqbar = (VTEqbar*)acao->Eqpto();

   //determina Eqbar
   if ((eqbar = ExisteEqbar(eqbar)) == NULL) return(NULL);
   //cria uma nova AcaoRetira
   return(NewObjAcaoRetira(eqbar));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoRetiraLigacao(VTAcao *acao)
   {
   //variáveias locais
   VTLigacao *ligacao = (VTLigacao*)acao->Eqpto();

   //determina Ligacao
   if ((ligacao = ExisteLigacao(ligacao)) == NULL) return(NULL);
   //cria uma nova AcaoRetira
   return(NewObjAcaoRetira(ligacao));
   }

//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoRetiraRede(VTAcao *acao)
   {
   //variáveias locais
   VTRede *rede = (VTRede*)acao->Eqpto();

   //determina Rede
   if ((rede = ExisteRede(rede)) == NULL) return(NULL);
   //cria uma nova AcaoRetira
   return(NewObjAcaoRetira(rede));
   }
   
//---------------------------------------------------------------------------
VTAcao* __fastcall  TDuplicacao::DuplicaAcaoRetiraMutua(VTAcao *acao)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TDuplicacao::ExisteBarra(VTBarra *barra)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se existe Barra nova em lisEQN associada à Barra indicada
   if ((eqpto = ExisteCloneDeEqptoInserido(barra)) != NULL)
      {
      barra = (VTBarra*)(eqpto);
      }
   return(barra);
   }

//---------------------------------------------------------------------------
VTEqbar*   __fastcall TDuplicacao::ExisteEqbar(VTEqbar *eqbar)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se existe Eqbar novo em lisEQN associado ao Eqbar indicado
   if ((eqpto = ExisteCloneDeEqptoInserido(eqbar)) != NULL)
      {
      eqbar = (VTEqbar*)(eqpto);
      }
   return(eqbar);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TDuplicacao::ExisteCloneDeEqptoInserido(VTEqpto *eqpto_ref)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se existe Eqpto em lisEQN associado ao Eqpto indicado
   for (int n = 0; n < lisCLONE->Count; n++)
      {
      eqpto = (VTEqpto*)lisCLONE->Items[n];
      if (eqpto->Obj == eqpto_ref)
         {
         return(eqpto);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TDuplicacao::ExisteLigacao(VTLigacao *ligacao)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se existe Ligacao nova em lisEQN associada à Ligacao indicada
   if ((eqpto = ExisteCloneDeEqptoInserido(ligacao)) != NULL)
      {
      ligacao = (VTLigacao*)(eqpto);
      }
   return(ligacao);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TDuplicacao::ExisteRede(VTRede *rede)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se existe Rede nova em lisEQN associada à Rede indicada
   if ((eqpto = ExisteCloneDeEqptoInserido(rede)) != NULL)
      {
      rede = (VTRede*)(eqpto);
      }
   return(rede);
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
//https://sinapsisenergia.teamwork.com/#tasks/15532355
//void  __fastcall TDuplicacao::RedefineBarraIni(VTBarra *barra_ini, VTBarra *clone)
//{
//	std::pair <std::multimap<VTBarra*,VTRede*>::iterator, std::multimap<VTBarra*,VTRede*>::iterator> ret;
//	VTRede *rede;
//
//	//verifica no multimap quais redes deveriam possuir essa barra original como barra ini
//	ret = mmapRedeParaRevisar.equal_range(barra_ini);
//	for (std::multimap<VTBarra*,VTRede*> ::iterator it=ret.first; it!=ret.second; ++it)
//	{
//		rede = it->second;
//		//acerta a barra inicial pra barra clonada
//		rede->DefineBarraInicial(clone);
//	}
//	mmapRedeParaRevisar.erase(barra_ini);
//}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
//https://sinapsisenergia.teamwork.com/#tasks/15532355
//void  __fastcall TDuplicacao::RedefineBarraIni(void)
//{
//	std::multimap<VTBarra*,VTRede*>::iterator it;
//	VTRede *rede;
//
//	//se ao fim do processo não houve redefinição da barra inicial é
//	//porque a barra nao foi clonada, então a rede deve manter a barra inicial
//	//original
//	for (it=mmapRedeParaRevisar.begin(); it!=mmapRedeParaRevisar.end(); ++it)
//	{
//		rede = it->second;
//		rede->DefineBarraInicial(it->first);
//	}
//	mmapRedeParaRevisar.clear();
//}
//---------------------------------------------------------------------------
//eof
