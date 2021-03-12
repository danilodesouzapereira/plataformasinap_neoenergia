//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "VTEdita.h"
#include "TModifica.h"
#include "TRebuild.h"
#include "..\Apl\VTApl.h"
#include "..\AutoTag\VTAutoTag.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTMercado.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTLixeira.h"
#include "..\Obra\VTObra.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRedes.h"
#include "..\Redegraf\VTCelulas.h"
//#include "..\MultiObra\VTMultiObra.h"
//#include "..\MultiObra\VTMultiObraBO.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTModifica* __fastcall NewObjModifica(VTApl *apl)
   {
   return(new TModifica(apl));
   }

//---------------------------------------------------------------------------
__fastcall TModifica::TModifica(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
   //cria lista
   lisTMP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TModifica::~TModifica(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::AlteraBarra(VTBarra* pbarra, VTBarra *copia_original, VTBarra *copia_alterada)
   {
   //marca alteração de área e do Eqpto
   AfetaArea  = true;
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::AlteraCluster(VTCluster *cluster, VTCluster *copia_original, VTCluster *copia_alterada)
   {
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::AlteraEqbar(VTEqbar* eqbar, VTEqbar *copia_original, VTEqbar *copia_alterada)
   {
   //variáveis locais
   //VTBarra *bar_ori = eqbar->pbarra;
   VTBarra *bar_ori = copia_original->pbarra;
   VTBarra *bar_alt = copia_alterada->pbarra;

   //verifica se Eqbar mudou de Barra
   if (bar_ori != bar_alt)
      {//verifica qual a Barra em que o Eqbar deve ser movido
      if (eqbar->pbarra == bar_alt)
         {//transfere Eqbar da barra original p/ Barra alterada
         bar_ori->RemoveEqbar(eqbar);
         bar_alt->InsereEqbar(eqbar);
         }
      else
         {//transfere Eqbar da Barra alterada p/ Barra original
         bar_alt->RemoveEqbar(eqbar);
         bar_ori->InsereEqbar(eqbar);
         }
      //verifica se é uma Carga
      if (eqbar->Tipo() == eqptoCARGA)
         {//indica alteração de Mercado
         AfetaMercado = true;
         }
      //indica alteração de topologia
      //AfetaTopologia = true;
      //indica alteração de area
      AfetaArea = true;
      }
   //verifica tipo do Eqbar
   switch(eqbar->Tipo())
     {
     case eqptoGERADOR:
     case eqptoSUPRIMENTO: //reclassifica Redes (simples ou compostas)
       //topologia->ClassificaRedes();
       AfetaTopologia = true;
       break;
     }
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaArea  = true;
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16818904
bool __fastcall TModifica::AlteraLigacao(VTLigacao* pliga, VTLigacao *copia_original, VTLigacao *copia_alterada)
   {
   //variáveis locais
   int         ind_bar;
   VTBarra     *barra;
   VTRede      *rede_atual, *rede_antes, *rede_depois;
   VTLigacao   *lig;

   //verifica se a Ligacao foi transferida de Rede
   rede_atual  = ((VTLigacao*)pliga)->rede;
   rede_antes  = ((VTLigacao*)copia_original)->rede;
   rede_depois = ((VTLigacao*)copia_alterada)->rede;
   if (rede_antes != rede_depois)
      {//transfere Ligacao de Rede
      if      (rede_atual != rede_antes)  TransfereLigacao(rede_antes,  rede_atual, pliga);
      else if (rede_atual != rede_depois) TransfereLigacao(rede_depois, rede_atual, pliga);
      //remove copia da ligacao original da lista de ligacoes de cada barra
      lig = (VTLigacao*)copia_original;
      if(lig->pbarra1 != NULL)
         {
         lig->pbarra1->LisLigacao()->Remove(lig);
         }
      if(lig->pbarra2 != NULL)
         {
         lig->pbarra2->LisLigacao()->Remove(lig);
         }
      if(lig->pbarra3 != NULL)
         {
         lig->pbarra3->LisLigacao()->Remove(lig);
         }

      //marca necessidade de reconfigurar
      AfetaArea      = true;
      AfetaTopologia = true;
      }
   //verifica se as Barras da Ligacao foram alteradas
   lisTMP->Clear();
   copia_original->LisBarra(lisTMP);
   copia_alterada->LisBarra(lisTMP);
   for (ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = pliga->Barra(ind_bar)) == NULL) continue;
      lisTMP->Remove(barra);
      }
   if (lisTMP->Count == 1)
      {//houve alteração de barras da ligacao: marca necessidade de reconfigurar
      AfetaTopologia = true;
/*
//JCG 2017.02.10 - a alteração na Barra da Ligação não deve alterar a Barra inicial da
      //determina barra que foi desligada
      barra = (VTBarra*)lisTMP->First();
      //verifica se a Barra desligada era a Barra inicial
      if (rede_atual->BarraInicial() == barra)
         {//determina nova Barra inicial
         ind_bar = copia_original->IndiceBarra(barra);
         if (ind_bar < 0) ind_bar = copia_alterada->IndiceBarra(barra);
         barra = pliga->Barra(ind_bar);
         //redefine Barra inicial
         rede_atual->DefineBarraInicial(barra);
         //atualiza status da Rede: alterada
         rede_atual->Status[sttALTERADO] = true;
         }
*/
      }
   //verifica se a Ligacao é uma Chave
   if (pliga->Tipo() == eqptoCHAVE)
      {//verifica se Chave mudou de estado
      if (((VTChave*)copia_original)->Estado != ((VTChave*)copia_alterada)->Estado)
         {//marca necessidade de reconfigurar
         AfetaTopologia = true;
         }
      }
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::AlteraMutua(VTMutua* mutua, VTMutua *mutua_original, VTMutua *mutua_alterada)
   {
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::AlteraRede(VTRede *rede, VTRede *copia_original, VTRede *copia_alterada)
   {
   //verifica se houve mudança de Cluster
   if (copia_original->Cluster != copia_alterada->Cluster)
      {//retira Rede do seu Cluster original
      if (copia_original->Cluster != NULL) copia_original->Cluster->RemoveRede(rede);
      //insere Rede no seu novo Cluster
      if (copia_alterada->Cluster != NULL) copia_alterada->Cluster->InsereRede(rede);
      //sinaliza alteração de rede a ser tratada por outro objetos
	  AfetaTopologia = true;
      }
   //verifica se mudou o atributo estatica
   if (copia_original->Estatica != copia_alterada->Estatica)
      {//sinaliza alteração de rede a ser tratada por outro objetos
	  AfetaTopologia = true;
      }
   //verifica se mudou Barra inicial
   if (copia_original->BarraInicial() != copia_alterada->BarraInicial())
      {//sinaliza alteração de rede a ser tratada por outro objetos
	  AfetaTopologia = true;
      }
   //sinaliza alteração de rede a ser tratada por outro objetos
   AfetaEqpto = true;
   return(true);
   }
   //---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
//void __fastcall TModifica::AtualizaMultiObra(void)
//{
//	VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
//	VTMultiObra *multiObra;
//
//	if (multiObraBO != NULL)
//	{
//		multiObra = multiObraBO->ExisteMultiObra(obra);
//		if (multiObra != NULL)
//		{
//			multiObra->UpToDate = false;
//		}
//	}
//}
//---------------------------------------------------------------------------
void __fastcall TModifica::DefineActionExterna(TList *lisACTION)
   {
   //salva ponteiro p/ lista
   this->lisACTION = lisACTION;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TModifica::DeterminaRede(VTLigacao *pliga)
   {
   //variáveis locais
   VTRede  *rede;
   TList   *lisREDE;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTBarra *pbar[NBAR_LIG] = {pliga->pbarra1, pliga->pbarra2, pliga->pbarra3};

   //loop p/ todas Redes
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      //verifica se a Rede conecta uma das Barras
      for (int nb = 0; nb < NBAR_LIG; nb++)
         {//proteção:
         if (pbar[nb] == NULL) continue;
         if (rede->ExisteBarra(pbar[nb])) return(rede);
         }
      }
   //verifica se Ligacação esstá na rede isolada
   rede = redes->RedeIsolada();
   if (rede->ExisteLigacao(pliga)) return(rede);
   //nenhuma Rede conecta as Barras da Ligacao
   return(NULL);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16993279
// https://sinapsisenergia.teamwork.com/#/tasks/17200660
bool __fastcall TModifica::Do(VTObra *obra, VTAcao *acao)
{
   //variaveis locais
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
   bool sucesso = false;
   bool rodando_fluxo = false;

   //proteção
   if (obra == NULL)return(true);
   //verifica se estava rodando fluxo
   if(sinap_child != NULL)
   {
        if (sinap_child->ButFlow)
        {
            rodando_fluxo = sinap_child->ButFlow->Down;
            if(rodando_fluxo)
                //garante que esteja no modo Topologia
                sinap_child->ActionTopo->Execute();
        }
   }
   //salva objeto Obra
   this->obra = obra;
   //executa acao
   sucesso = Do(acao);
   //se necessário, reexecuta fluxo
   if(rodando_fluxo)
   {
       sinap_child->ActionFlow->Execute();
   }
   return(sucesso);
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
bool __fastcall TModifica::Do(VTAcao *acao)
   {
   //variáveis locais
   bool sucesso;

   //proteção
   if (acao == NULL) return(true);
   //reinicia impacto da Acao desde que ela não pertença a uma Acao composta
   ReiniciaImpactoAcao(acao);
   //trata Acao de acordo c/ seu tipo
   sucesso = DoAcao(acao);
   //verifica impacto da Acao
   if (sucesso)VerificaImpactoAcao(acao);
   //atualiza o multiObra
//   if (sucesso)
//	  {AtualizaMultiObra();}
//==================================
// 2013.07.12
   //proteção: uma Acao de alteração de uma Ligacao pode fazer com que a rede final
   //          associada à Ligacao seja diferente da rede associada à cópia alterada
   //DoVerificaAlteracaoRedeDeLigacao(acao);
//==================================
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::DoAcao(VTAcao *acao)
   {
   //variáveis locais
   bool sucesso = true;
   //teste
   VTEdita * edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //proteção
   if (acao == NULL)return(true);
   //verifica se deve adicionar tag
   if ((edita != NULL) && (edita->AutoTag != NULL))
	  {
	  edita->AutoTag->InsereTagAcao(acao);
	  }
   //trata Acao de acordo c/ seu tipo
   switch(acao->Tipo())
      {
      case acaoALTERA:   sucesso = DoAcaoAltera(acao);   break;
      case acaoINSERE:   sucesso = DoAcaoInsere(acao);   break;
      case acaoRETIRA:   sucesso = DoAcaoRetira(acao);   break;
      case acaoCOMPOSTA: sucesso = DoAcaoComposta(acao); break;
      default: break;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::DoAcaoAltera(VTAcao *acao)
   {
   //variáveis locais
   VTRede  *rede;
   VTEqpto *eqpto, *copia_original, *copia_alterada;

   //determina Eqpto e suas cópias
   eqpto          = acao->Eqpto();
   copia_original = acao->CopiaDoEqptoOriginal();
   copia_alterada = acao->CopiaDoEqptoAlterado();
   /*
   //proteção feita em 2013.03.19 e comentada em  2014.03.13
   if (eqpto->TipoLigacao())
      {//verifica inconsistência da cópia original quando lida da base de dados
      rede = ((VTLigacao*)eqpto)->rede;
      ((VTLigacao*)copia_original)->rede = rede;
      }
   */
   //JCG 2014.05.30
   //proteção: garante que as cópias permanceçam com o mesmo ID do Eqpto original
   //          Os IDs são alterados caso a rede tenha sido salva na base
   copia_original->Id = eqpto->Id;
   copia_alterada->Id = eqpto->Id;
   //atualiza Eqpto a partir de sua cópia alterada
   eqpto->CopiaAtributosDe(*copia_alterada);
   //verifica se Eqpto é uma Ligacao
   if (eqpto->TipoBarra())            return(AlteraBarra((VTBarra*)eqpto, (VTBarra*)copia_original, (VTBarra*)copia_alterada));
   if (eqpto->TipoEqbar())            return(AlteraEqbar((VTEqbar*)eqpto, (VTEqbar*)copia_original, (VTEqbar*)copia_alterada));
   if (eqpto->TipoLigacao())          return(AlteraLigacao((VTLigacao*)eqpto, (VTLigacao*)copia_original, (VTLigacao*)copia_alterada));
   if (eqpto->Tipo() == eqptoMUTUA)   return(AlteraMutua((VTMutua*)eqpto, (VTMutua*)copia_original, (VTMutua*)copia_alterada));
   if (eqpto->Tipo() == eqptoREDE)    return(AlteraRede((VTRede*)eqpto, (VTRede*)copia_original, (VTRede*)copia_alterada));
   if (eqpto->Tipo() == eqptoCLUSTER) return(AlteraCluster((VTCluster*)eqpto, (VTCluster*)copia_original, (VTCluster*)copia_alterada));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::DoAcaoComposta(VTAcao *acao)
   {
   //variáveis locais
   TList *lisACAO = acao->LisAcao();

   //loop p/ todas Acoes
   for (int n = 0; n < lisACAO->Count; n++)
      {
      if (! DoAcao((VTAcao*)lisACAO->Items[n])) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::DoAcaoInsere(VTAcao *acao)
   {
   //variáveis locais
   VTEqpto *eqpto, *copia_original;

   //determina Eqpto e sua cópia do original
   eqpto          = acao->Eqpto();
   copia_original = acao->CopiaDoEqptoOriginal();
   //JCG 2014.05.30
   //proteção: garante que as cópias permanceçam com o mesmo ID do Eqpto original
   //          Os IDs são alterados caso a rede tenha sido salva na base
   copia_original->Id = eqpto->Id;
   //atualiza Eqpto a partir de sua cópia original
   eqpto->CopiaAtributosDe(*copia_original);
   //trata Eqpto de acordo seu tipo
   if (eqpto->TipoBarra())            return(InsereBarra((VTBarra*)eqpto));
   if (eqpto->TipoEqbar())            return(InsereEqbar((VTEqbar*)eqpto));
   if (eqpto->TipoLigacao())          return(InsereLigacao((VTLigacao*)eqpto));
   if (eqpto->Tipo() == eqptoCANAL)   return(InsereCanal((VTCanal*)eqpto));
   if (eqpto->Tipo() == eqptoMUTUA)   return(InsereMutua((VTMutua*)eqpto));
   if (eqpto->Tipo() == eqptoREDE)    return(InsereRede((VTRede*)eqpto));
   if (eqpto->Tipo() == eqptoCLUSTER) return(InsereCluster((VTCluster*)eqpto));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::DoAcaoRetira(VTAcao *acao)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //determina Eqpto a ser inserido na Rede
   eqpto = acao->Eqpto();
   //tratamento distinto p/ Barra, Eqbar, Ligacao e Rede
   if (eqpto->TipoBarra())            return(RetiraBarra((VTBarra*)eqpto));
   if (eqpto->TipoEqbar())            return(RetiraEqbar((VTEqbar*)eqpto));
   if (eqpto->TipoLigacao())          return(RetiraLigacao((VTLigacao*)eqpto));
   if (eqpto->Tipo() == eqptoCANAL)   return(RetiraCanal((VTCanal*)eqpto));
   if (eqpto->Tipo() == eqptoMUTUA)   return(RetiraMutua((VTMutua*)eqpto));
   if (eqpto->Tipo() == eqptoREDE)    return(RetiraRede((VTRede*)eqpto));
   if (eqpto->Tipo() == eqptoCLUSTER) return(RetiraCluster((VTCluster*)eqpto));
   return(true);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TModifica::DoVerificaAlteracaoRedeDeLigacao(VTAcao *acao)
   {
   //variáveis locais
   TList *lisACAO = acao->LisAcao();

   //verifica se é uma Acao composta
   if (acao->Tipo() == acaoCOMPOSTA)
      {//loop p/ todas Acoes
      for (int n = 0; n < lisACAO->Count; n++)
         {
         DoVerificaAlteracaoRedeDeLigacao((VTAcao*)lisACAO->Items[n]);
         }
      }
   else
      {//verifica se é uma Acao de alteração de Ligacao
      if ((acao->Tipo() == acaoALTERA)&&(acao->Eqpto()->TipoLigacao()))
         {
         VTLigacao *ligacao           = (VTLigacao*)acao->Eqpto();
         VTLigacao  *ligacao_alterada = (VTLigacao*)acao->CopiaDoEqptoAlterado();
         if (ligacao->rede != ligacao_alterada->rede)
            {
#ifdef _DEBUG
//teste em modo debug =====================
            Aviso("TModifica::DoVerificaAlteracaoRedeDeLigacao(): redefine rede da cópia alterada");
//fim teste em modo debug =================
#endif
            ligacao_alterada->rede = ligacao->rede;
            }
         }
      }
   }
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
//https://sinapsisenergia.teamwork.com/#tasks/15987292
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TModifica::IdentificaRedesVizinhasAfetadasDo(TList *lisREDE, TList *lisACAO)
   {
   //variáveis locais
   TList     *lisLIG;
   VTAcao    *acao;
   VTBarra   *barra;
   VTEqpto   *eqpto;
   VTChave   *chave;
   VTLigacao *ligacao;
   VTRede    *rede;

   //loop para todas Acoes
   for (int n = 0; n < lisACAO->Count; n++)
	  {
	  acao = (VTAcao*)lisACAO->Items[n];
	  eqpto = acao->Eqpto();
	  //verifica acao composta
	  if (acao->Tipo() == acaoCOMPOSTA)
		{
		IdentificaRedesVizinhasAfetadasDo(lisREDE, acao->LisAcao());
		}
	  else
		{
		  if(eqpto == NULL) continue;
		  if (eqpto->Tipo() == eqptoREDE)
			 {//verifica as Redes conectadas pela Chave
	//		 if (lisREDE->IndexOf(eqpto) < 0)
	//			lisREDE->Add(eqpto);
			 //verifica o tipo de acao
			 if (acao->Tipo() != acaoRETIRA)
				{
				InsereRedeAfetada(lisREDE,(VTRede*)eqpto);
				}
			 else
				{//remove
				lisREDE->Remove(eqpto);
				}
			 }

		  else if (eqpto->Tipo() == eqptoCHAVE)
			 {//verifica as Redes conectadas pela Chave
			 chave = (VTChave*)eqpto;
			 for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
				{
				if ((barra = chave->Barra(ind_bar)) == NULL) continue;
				lisLIG = barra->LisLigacao();
				for (int nl = 0; nl < lisLIG->Count; nl++)
				   {
				   ligacao = (VTLigacao*)lisLIG->Items[nl];
				   if ((rede = ligacao->rede) == NULL) continue;
	//			   if (lisREDE->IndexOf(rede) < 0)
	//				  {
	//				  lisREDE->Add(rede);
	//				  }
					InsereRedeAfetada(lisREDE,rede);
				   }
				}
			 }
		}
	  }
   return(true);
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TModifica::IdentificaRedesVizinhasAfetadasUndo(TList *lisREDE, TList *lisACAO)
   {
   //variáveis locais
   TList     *lisLIG;
   VTAcao    *acao;
   VTBarra   *barra;
   VTEqpto   *eqpto;
   VTChave   *chave;
   VTLigacao *ligacao;
   VTRede    *rede;

   //loop para todas Acoes
   //for (int n = 0; n < lisACAO->Count; n++)//importante, inverte a ordem
   for (int n = lisACAO->Count - 1; n >= 0; n--)//importante, inverte a ordem
	  {
	  acao = (VTAcao*)lisACAO->Items[n];
	  eqpto = acao->Eqpto();
	  //verifica acao composta
	  if (acao->Tipo() == acaoCOMPOSTA)
		{
		IdentificaRedesVizinhasAfetadasUndo(lisREDE, acao->LisAcao());
		}
	  else
		{
		  if(eqpto == NULL) continue;
		  if (eqpto->Tipo() == eqptoREDE)
			 {//verifica as Redes conectadas pela Chave
	//		 if (lisREDE->IndexOf(eqpto) < 0)
	//			lisREDE->Add(eqpto);
			 //verifica o tipo de acao
			 if (acao->Tipo() == acaoINSERE)
				{
				lisREDE->Remove(eqpto);
				}
			 else
				{//remove
				InsereRedeAfetada(lisREDE,(VTRede*)eqpto);
				}
			 }

		  else if (eqpto->Tipo() == eqptoCHAVE)
			 {//verifica as Redes conectadas pela Chave
			 chave = (VTChave*)eqpto;
			 for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
				{
				if ((barra = chave->Barra(ind_bar)) == NULL) continue;
				lisLIG = barra->LisLigacao();
				for (int nl = 0; nl < lisLIG->Count; nl++)
				   {
				   ligacao = (VTLigacao*)lisLIG->Items[nl];
				   if ((rede = ligacao->rede) == NULL) continue;
	//			   if (lisREDE->IndexOf(rede) < 0)
	//				  {
	//				  lisREDE->Add(rede);
	//				  }
					InsereRedeAfetada(lisREDE,rede);
				   }
				}
			 }
		}
	  }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TModifica::InsereBarra(VTBarra* pbarra)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //insere Barra em Redes ou em Rebuild
   if (rebuild != NULL) rebuild->InsereBarra(pbarra);
   else                 redes->InsereBarra(pbarra);
   //remove Eqpto da lixeira
   obra->Lixeira->RemoveEqpto(pbarra);
   //marca que alteração afeta área das redes
   AfetaArea = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::InsereCanal(VTCanal *canal)
   {
   canal->Medidor->InsereCanal(canal);
   //NÃO sinaliza alteração de rede a ser tratada por outro objetos
   //AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::InsereCluster(VTCluster *cluster)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //insere Cluster em Redes ou em Rebuild
   if (rebuild != NULL) rebuild->InsereCluster(cluster);
   else                 redes->InsereCluster(cluster);
   //remove Eqpto da lixeira
   obra->Lixeira->RemoveEqpto(cluster);
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::InsereEqbar(VTEqbar* eqbar)
   {
   //variáveis locais
   VTBarra  *pbarra;
   VTRedes  *redes;

   //insere Eqbar na Barra
   pbarra = eqbar->pbarra;
   pbarra->InsereEqbar(eqbar);
   //remove Eqpto da lixeira
   obra->Lixeira->RemoveEqpto(eqbar);
   //verifica se é uma Carga
   if (eqbar->Tipo() == eqptoCARGA)
      {//redefine Crescimento da Carga porque ela pode ter mudado de Rede
      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
      //sinaliza alteração de Mercado
      AfetaMercado = true;
      }
   //verifica tipo de eqbar
   if ((eqbar->Tipo() == eqptoGERADOR)||(eqbar->Tipo() == eqptoSUPRIMENTO))
      {//reclassifica Redes (simples ou compostas)
      //topologia->ClassificaRedes();
      AfetaTopologia = true;
      }
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::InsereLigacao(VTLigacao* pliga)
   {
   //variáveis locais
   VTRede      *rede;
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //insere Ligacao na Rede
   rede  = pliga->rede;
   //proteção: verifica se a Rede da Ligacao está definida
   if (rede == NULL) rede = DeterminaRede(pliga);
   if (rede == NULL) rede = redes->RedeIsolada();
   if (rede == NULL) return(false);
   //insere Ligacao na Rede ou em Rebuil
   if (rebuild != NULL) rebuild->InsereLigacao(pliga);
   else                 rede->InsereLigacao(pliga);
   //remove Eqpto da lixeira
   obra->Lixeira->RemoveEqpto(pliga);
   /*
   //recalcula área da Rede
   rede->CalculaArea();
   //recalcula área de todas as Redes
   redes->CalculaArea();
   //determina Redes interligadas pela Ligacao
   lisTMP->Clear();
   LisRede(pliga, lisTMP);
   //reconfigura Redes afetadas pela manobra
   //topologia->ReconfiguraRede(lisTMP);
   */
   AfetaArea      = true;
   AfetaTopologia = true;
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::InsereMutua(VTMutua* mutua)
   {
   //variáveis locais
   VTRede *rede = mutua->Rede();

   //insere Mutua na Rede ou em Rebuil
   if (rebuild != NULL) rebuild->InsereMutua(mutua);
   else                 rede->InsereMutua(mutua);
   //remove Eqpto da lixeira
   obra->Lixeira->RemoveEqpto(mutua);
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;

   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
bool __fastcall TModifica::InsereRede(VTRede *rede)
   {
   //variáveis locais
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //proteção: desconsidera a Rede isolada
   if (rede == redes->RedeIsolada()) return(true);
   //inicia atributos Rede como carregada
   rede->Carregada = true;
   rede->MRede     = NULL;
   rede->LisCorte()->Clear();
   //insere Rede em seu Cluster, se for o caso
   if (rede->Cluster) rede->Cluster->InsereRede(rede);
   //insere Cluster em Redes ou em Rebuild
   if (rebuild != NULL) rebuild->InsereRede(rede);
   else                 redes->InsereRede(rede);
   //remove Eqpto da lixeira
   obra->Lixeira->RemoveEqpto(rede);
   obra->AddRede(rede);
   //reconfigura Redes
   //topologia->Reconfigura();
   AfetaTopologia = true;
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TModifica::InsereRedeAfetada(TList *lisREDE, VTRede *rede)
{
	//variáveis locais
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se a rede já está no redes
   if (redes->LisRede()->IndexOf(rede)< 0)
   { //não está no redes, não deve incluir
		//faz nada
		int a = 0;
   }
   else
   {
		if (lisREDE->IndexOf(rede) < 0)
		{
			lisREDE->Add(rede);
		}
   }
}
//---------------------------------------------------------------------------
void __fastcall TModifica::LisRede(VTLigacao *pliga, TList *lisSEL)
   {
   //variáveis locais
   VTRede  *rede;
   TList   *lisREDE;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTBarra *pbar[NBAR_LIG] = {pliga->pbarra1, pliga->pbarra2, pliga->pbarra3};

   //insere em lisSEL a Rede da Ligacao (pois uma Rede sem nenhuma Ligacao ainda não possui Barras)
   if ((pliga->rede)&&(lisSEL->IndexOf(pliga->rede) < 0)) lisSEL->Add(pliga->rede);
   //loop p/ todas Redes
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      //verifica se a Rede conecta uma das Barras
      for (int nb = 0; nb < NBAR_LIG; nb++)
         {//proteção:
         if (pbar[nb] == NULL) continue;
         if (rede->ExisteBarra(pbar[nb]))
            {//insere Rede em lis
            if (lisSEL->IndexOf(rede) < 0) lisSEL->Add(rede);
            }
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::PM_GetAfetaArea(void)
   {
   return(afeta.area);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::PM_GetAfetaEqpto(void)
   {
   return(afeta.eqpto);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::PM_GetAfetaMercado(void)
   {
   return(afeta.mercado);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::PM_GetAfetaTopologia(void)
   {
   return(afeta.topologia);
   }

//---------------------------------------------------------------------------
void __fastcall TModifica::PM_SetAfetaArea(bool afeta_area)
   {
   afeta.area = afeta_area;
   }

//---------------------------------------------------------------------------
void __fastcall TModifica::PM_SetAfetaEqpto(bool afeta_eqpto)
   {
   afeta.eqpto = afeta_eqpto;
   }

//---------------------------------------------------------------------------
void __fastcall TModifica::PM_SetAfetaMercado(bool afeta_mercado)
   {
   afeta.mercado = afeta_mercado;
   }

//---------------------------------------------------------------------------
void __fastcall TModifica::PM_SetAfetaTopologia(bool afeta_topo)
   {
   afeta.topologia = afeta_topo;
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TModifica::Redo(VTAcao *acao)
   {
   //variáveis locais
   bool sucesso;

   //proteção
   if (acao == NULL) return(true);
   //salva ponteio p/ VTObra
   //this->obra = obra;
   //reinicia impacto da Acao desde que ela não pertença a uma Acao composta
   ReiniciaImpactoAcao(acao);
   //trata Acao de acordo c/ seu tipo
   sucesso = RedoAcao(acao);
   //verifica impacto da Acao desde que ela não pertença a uma Acao composta
   if (sucesso) VerificaImpactoAcao(acao);

   return(sucesso);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TModifica::Redo(VTObra *obra, VTAcao *acao)
   {
   //proteção
   if (obra == NULL) return(true);
   //salva ponteio p/ VTObra
   this->obra = obra;
   //executa Acao
   return(Do(acao));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TModifica::Redo(TList *lisREDE, TList *lisACAO)
   {
   //variáveis locais
   VTAcao *acao;

   //proteção
   if (lisACAO == NULL) return(true);
   //complementa lisREDE com redes vizinhas afetadas pelas ações
   IdentificaRedesVizinhasAfetadasDo(lisREDE, lisACAO);
   //cria e inicia ojeto Rebuild
   rebuild = new TRebuild(apl);
   rebuild->IniciaListas(lisREDE);
   //reinicia impacto das Acoes
   ReiniciaImpactoAcao(NULL);
   //loop p/ todas Acoes em lisACAO
   for (int n = 0; n < lisACAO->Count; n++)
      {
      acao = (VTAcao*)lisACAO->Items[n];
      //define ponteio p/ VTObra
      obra = acao->Obra;
      //trata Acao de acordo c/ seu tipo
      //if (! Do(acao)) return(false);
      if (! DoAcao(acao))
         {
#ifdef _DEBUG
//teste em modo debug =====================
         Aviso("TModifica::Redo(lisACAO): Erro em Redo da ação");
         //return(false);
//fim teste em modo debug =================
#endif
         }
      }
   //verifica impacto das Accoes
   VerificaImpactoAcao(NULL);
   //destrói objeto Rebuild
   delete rebuild;
   rebuild = NULL;
//==================================
// 2013.07.12
   //proteção: uma Acao de alteração de uma Ligacao pode fazer com que a rede final
   //          associada à Ligacao seja diferente da rede associada à cópia alterada
   //DoVerificaAlteracaoRedeDeLigacao(acao);
//==================================

#ifdef _DEBUG
//teste em modo debug =====================
   VerificaIntegridade();
//fim teste em modo debug =================
#endif
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TModifica::RedoAcao(VTAcao *acao)
   {
   //variáveis locais
   bool sucesso;

   //proteção
   if (acao == NULL) return(true);
   //salva ponteio p/ VTObra
//   this->obra = obra;
   //trata Acao de acordo c/ seu tipo
   switch(acao->Tipo())
      {
      case acaoALTERA:   sucesso = RedoAcaoAltera(acao);   break;
      case acaoINSERE:   sucesso = RedoAcaoInsere(acao);   break;
      case acaoRETIRA:   sucesso = RedoAcaoRetira(acao);   break;
      case acaoCOMPOSTA: sucesso = RedoAcaoComposta(acao); break;
      default:           sucesso = true;                   break;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RedoAcaoAltera(VTAcao *acao)
   {
   //RedoAcaoAltera(acao) equivale a DoAcaoAltera(acao)
   return(DoAcaoAltera(acao));
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RedoAcaoInsere(VTAcao *acao)
   {
   //RedoAcaoInsere(acao) equivale a DoAcaoInsere(acao)
   return(DoAcaoInsere(acao));
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RedoAcaoRetira(VTAcao *acao)
   {
   //RedoAcaoRetira(acao) equivale a DoAcaoRetira(acao)
   return(DoAcaoRetira(acao));
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RedoAcaoComposta(VTAcao *acao)
   {
   //RedoAcaoComposta(acao) equivale a DoAcaoComposta(acao)
   return(DoAcaoComposta(acao));
   }
*/
//---------------------------------------------------------------------------
void __fastcall TModifica::ReiniciaImpactoAcao(VTAcao *acao)
   {
   //verifica se Acao pertence a uma Acao composta
   if ((acao != NULL)&&(acao->Pai != NULL)) return;
   //reincia controle do impacto da Acao
   AfetaArea      = false;
   AfetaEqpto     = false;
   AfetaMercado   = false;
   AfetaTopologia = false;
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraBarra(VTBarra* pbarra)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //remove Barra de Redes ou de Rebuild
   if (rebuild != NULL) rebuild->RemoveBarra(pbarra);
   else                 redes->RemoveBarra(pbarra);
   //insere Barra na Lixeira de Obra
   obra->Lixeira->InsereEqpto(pbarra);
   //verifica se a Barra eliminada era a Barra inicial de alguma Rede
   //marca que alteração afeta eqptos e área das redes
   AfetaArea  = true;
   AfetaEqpto = true;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraCanal(VTCanal *canal)
   {
   //retira Canal do Medidor  e insere na Lixeira de Obra
   canal->Medidor->RemoveCanal(canal);
   obra->Lixeira->InsereEqpto(canal);
   //NÃO sinaliza alteração de rede a ser tratada por outro objetos
   //AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraCluster(VTCluster *cluster)
   {
   //variáveis locais
   VTRede      *rede;
   TList       *lisREDE;
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //retira Redes do Cluster
   lisREDE = cluster->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->Cluster = NULL;
      }
   //reinicia Redes do Cluster
   cluster->Reinicia();
   //retira Cluster de Redes ou de Rebuild
   if (rebuild != NULL) rebuild->RemoveCluster(cluster);
   else                 redes->RemoveCluster(cluster);
   //insere Cluster na Lixeira da Obra
   obra->Lixeira->InsereEqpto(cluster);
   //reconfigura Redes
   //topologia->Reconfigura();
   AfetaTopologia = true;
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraEqbar(VTEqbar* eqbar)
   {
   //retira Eqbar da Barra e insere na Lixeira de Obra
   eqbar->pbarra->RemoveEqbar(eqbar);
   obra->Lixeira->InsereEqpto(eqbar);
   //verifica tipo de eqbar
   if ((eqbar->Tipo() == eqptoGERADOR)||(eqbar->Tipo() == eqptoSUPRIMENTO))
      {//reclassifica Redes (simples ou compostas)
      //topologia->ClassificaRedes();
      AfetaTopologia = true;
      }
   //sinaliza alteração de rede a ser tratada por outro objetos
   //SinalizaAlteracaoRede();
   AfetaEqpto = true;
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraLigacao(VTLigacao* pliga)
   {
   //proteção
   if (pliga->rede == NULL) return(true);
   //determina Redes conectadas pela Ligacao
   lisTMP->Clear();
   LisRede(pliga, lisTMP);
   //retira Ligacao da Rede
   pliga->rede->RetiraLigacao(pliga);
   //recalcula área Rede
   pliga->rede->CalculaArea();
   //verifica se a Ligacao conectava mais de uma Rede
   if (lisTMP->Count > 1)
      {//reconfigura Redes afetadas pela manobra
      //topologia->ReconfiguraRede(lisTMP);
      AfetaTopologia = true;
      }
   //marca que alteração afeta área e eqpto
   AfetaArea = true;
   AfetaEqpto = true;
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraLigacao(VTLigacao* pliga)
   {
   //variáveis locais
   VTRede *rede;

   //proteção
   if ((rede = pliga->rede) == NULL) return(true);
   if (! rede->ExisteLigacao(pliga))
      {//ligacão não está na rede apontada por ela (CONDIÇÃO ESTRANHA)
#ifdef _DEBUG
//teste em modo debug =====================
      Aviso("TModifica::RetiraLigacao():" + pliga->TipoAsString() + ": " +
            pliga->Codigo + " não pertence à rede " + pliga->rede->Codigo);
//fim teste em modo debug =================
#endif
      //determina Rede da Ligacao
      rede = DeterminaRede(pliga);
#ifdef _DEBUG
//teste em modo debug =====================
      if (rede == NULL)
         {
         Aviso("TModifica::RetiraLigacao():" + pliga->TipoAsString() + ": " +
                pliga->Codigo + " rede = NULL");
         }
//fim teste em modo debug =================
#endif
      if ((rede == NULL) && (rebuild == NULL)) return(false);
      }
   //retira Ligacao da Rede ou de Rebuild e insere na Lixeira de Obra
   if (rebuild != NULL) rebuild->RemoveLigacao(pliga);
   else                 rede->RemoveLigacao(pliga);
   obra->Lixeira->InsereEqpto(pliga);
   //marca que alteração afeta área, eqpto e topologia
   AfetaArea      = true;
   AfetaEqpto     = true;
   AfetaTopologia = true;
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraMutua(VTMutua* mutua)
   {
   //variáveis locais
   VTRede *rede = mutua->Rede();

   //retira Mutua da Rede ou de Rebuild e insere na Lixeira de Obra
   if (rebuild != NULL) rebuild->RemoveMutua(mutua);
   else                 rede->RemoveMutua(mutua);
   obra->Lixeira->InsereEqpto(mutua);
   //marca que alteração afeta eqpto
   AfetaEqpto = true;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::RetiraRede(VTRede *rede)
   {
   //variáveis locais
   VTMRede *mrede;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se Rede é equivalente
   if (! rede->Carregada) return(true);
   //cancela ponteiro para MRede
   rede->MRede = NULL;
   //jcg 2014.03.24 - a retirada ou não dos Eqptos da Rede é tratado em TEdita e não aqui
   //transfere Ligacoes da Rede para Rede isolada
   //TransfereLigacao(rede, redes->RedeIsolada());
   //limpa lista de Ligacoes da Rede (e de Barras)
   rede->IniciaLisLigacao(NULL);
   //retira Rede seu Cluster, se for o caso
   if (rede->Cluster) rede->Cluster->RemoveRede(rede);
   //retira Rede de Redes ou de Rebuild
   if (rebuild != NULL) rebuild->RemoveRede(rede);
   //else                 redes->RemoveRede(rede);
   redes->RemoveRede(rede);
   //insere Rede  na Lixeira de Obra
   obra->Lixeira->InsereEqpto(rede);
   //atualiza flags de impacto
   AfetaArea      = true;
   AfetaEqpto     = true;
   AfetaTopologia = true;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TModifica::TransfereLigacao(VTRede *rede_orig, VTRede *rede_dest)
   {
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisLIG = rede_orig->LisLigacao();

   //verifica se existe o objeto Rebuild
   if (rebuild != NULL)
      {//IMPORTANTE: apenas remove as Ligacoes da rede origem
      lisLIG->Clear();
      return;
      }
   //transfere Ligacoes entre as Redes
   for (int n = lisLIG->Count-1; n >= 0; n--)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      rede_orig->RemoveLigacao(ligacao);
      rede_dest->InsereLigacao(ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TModifica::TransfereLigacao(VTRede *rede_orig, VTRede *rede_dest, VTLigacao *pliga)
   {
   //verifica se existe o objeto Rebuild
   if (rebuild != NULL) return;
   //retira Ligacao de sua Rede original
   if (rede_orig) rede_orig->RemoveLigacao(pliga);
   //insere Ligacao na sua nova Rede
   if (rede_dest) rede_dest->InsereLigacao(pliga);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::Undo(VTObra *obra, VTAcao *acao)
   {
   //proteção
   if (obra == NULL) return(true);
   //salva ponteio p/ VTObra
   this->obra = obra;
   //executa Undo
   return(Undo(acao));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
bool __fastcall TModifica::Undo(TList *lisREDE, TList *lisACAO)
   {
   //variáveis locais
   VTAcao *acao;
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //proteção
   if (lisACAO == NULL) return(true);
   //complementa lisREDE com redes vizinhas afetadas pelas ações
   IdentificaRedesVizinhasAfetadasUndo(lisREDE, lisACAO);
   //cria e inicia ojeto Rebuild
   rebuild = new TRebuild(apl);
   rebuild->IniciaListas(lisREDE);
   //reinicia impacto das Acoes
   ReiniciaImpactoAcao(NULL);
   //loop p/ todas Acoes em lisACAO
   for (int n = 0; n < lisACAO->Count; n++)
      {
      acao = (VTAcao*)lisACAO->Items[n];
      //salva ponteio p/ VTObra
	  obra = acao->Obra;
      //trata Acao de acordo c/ seu tipo
	  //if (! UndoAcao(acao)) return(false);
	  //caso o undo seja de trocar rede estatica, reconfigura o topologia
	  if (UndoAfetaTopologia(acao))
	  {
      	topologia->Reconfigura();
	  }
	  if (! UndoAcao(acao))
         {
#ifdef _DEBUG
//teste em modo debug =====================
         Aviso("TModifica::Undo(): Erro em Undo da ação");
         return(false);
//fim teste em modo debug =================
#endif
         }
	  }
   //verifica impacto das Accoes
   VerificaImpactoAcao(NULL);
   //destrói objeto Rebuild
   delete rebuild;
   rebuild = NULL;
//==================================
// 2014.03.13
   //proteção: uma Acao de alteração de uma Ligacao pode fazer com que a rede final
   //          associada à Ligacao seja diferente da rede associada à cópia original
   //UndoVerificaAlteracaoRedeDeLigacao(acao);
//==================================

#ifdef _DEBUG
//teste em modo debug =====================
   VerificaIntegridade();
//fim teste em modo debug =================
#endif
   //retorna sucesso
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
//https://sinapsisenergia.teamwork.com/#/tasks/16265994
bool __fastcall TModifica::Undo(VTAcao *acao)
   {
   //variáveis locais
   bool sucesso;

   //proteção
   if (acao == NULL) return(true);
   //reinicia impacto da Acao desde que ela não pertença a uma Acao composta
   ReiniciaImpactoAcao(acao);
   //trata Acao de acordo c/ seu tipo
   sucesso = UndoAcao(acao);
   //verifica impacto da Acao
   if (sucesso)VerificaImpactoAcao(acao);
//   //atualiza o multiObra
//   if (sucesso)
//	  {AtualizaMultiObra();}
//==================================
// 2014.03.13
   //proteção: uma Acao de alteração de uma Ligacao pode fazer com que a rede final
   //          associada à Ligacao seja diferente da rede associada à cópia original
   //UndoVerificaAlteracaoRedeDeLigacao(acao);
//==================================
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAcao(VTAcao *acao)
   {
   //variáveis locais
    //teste
   VTEdita * edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
   bool sucesso;

   //proteção
   if (acao == NULL)return(true);
   //verifica se deve adicionar tag
   if ((edita != NULL) && (edita->AutoTag != NULL))
	  {
	  edita->AutoTag->InsereTagAcao(acao,true);
	  }
   //trata Acao de acordo c/ seu tipo
   switch(acao->Tipo())
      {
      case acaoALTERA:   sucesso = UndoAcaoAltera(acao);   break;
      case acaoINSERE:   sucesso = UndoAcaoInsere(acao);   break;
      case acaoRETIRA:   sucesso = UndoAcaoRetira(acao);   break;
      case acaoCOMPOSTA: sucesso = UndoAcaoComposta(acao); break;
      default:           sucesso = true;                   break;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAcaoAltera(VTAcao *acao)
   {
   //variáveis locais
   VTEqpto *eqpto, *copia_original, *copia_alterada;

   //determina Eqpto e suas cópias
   eqpto          = acao->Eqpto();
   copia_original = acao->CopiaDoEqptoOriginal();
   copia_alterada = acao->CopiaDoEqptoAlterado();
   //JCG 2014.05.30
   //proteção: garante que as cópias permanceçam com o mesmo ID do Eqpto original
   //          Os IDs são alterados caso a rede tenha sido salva na base
   copia_original->Id = eqpto->Id;
   copia_alterada->Id = eqpto->Id;
   //atualiza Eqpto a partir de sua cópia original
   eqpto->CopiaAtributosDe(*copia_original);
   //verifica se Eqpto é uma Ligacao
   if (eqpto->TipoBarra())            return(AlteraBarra((VTBarra*)eqpto, (VTBarra*)copia_original, (VTBarra*)copia_alterada));
   if (eqpto->TipoEqbar())            return(AlteraEqbar((VTEqbar*)eqpto, (VTEqbar*)copia_original, (VTEqbar*)copia_alterada));
   if (eqpto->TipoLigacao())          return(AlteraLigacao((VTLigacao*)eqpto, (VTLigacao*)copia_original, (VTLigacao*)copia_alterada));
   if (eqpto->Tipo() == eqptoREDE)    return(AlteraRede((VTRede*)eqpto, (VTRede*)copia_original, (VTRede*)copia_alterada));
   if (eqpto->Tipo() == eqptoCLUSTER) return(AlteraCluster((VTCluster*)eqpto, (VTCluster*)copia_original, (VTCluster*)copia_alterada));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAcaoComposta(VTAcao *acao)
   {
   //variáveis locais
   TList *lisACAO = acao->LisAcao();

   //loop p/ todas Acoes, na ordem inversa
   //for (int n = 0; n < lisACAO->Count; n++)
   for (int n = lisACAO->Count-1; n >= 0; n--)
      {
      if (! Undo((VTAcao*)lisACAO->Items[n])) return(false);
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAcaoInsere(VTAcao *acao)
   {
   //UndoAcaoInsere() equivale a DoAcaoRetira()
   return(DoAcaoRetira(acao));
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAcaoInsere(VTAcao *acao)
   {
   //variáveis locais
   bool    sucesso;
   VTEqpto *eqpto, *copia_original;

   //UndoAcaoInsere() equivale a DoAcaoRetira()
   sucesso = DoAcaoRetira(acao);
   //determina Eqpto e sua cópia do original
   eqpto          = acao->Eqpto();
   copia_original = acao->CopiaDoEqptoOriginal();
   //JCG 2014.05.30
   //proteção: garante que as cópias permanceçam com o mesmo ID do Eqpto original
   //          Os IDs são alterados caso a rede tenha sido salva na base
   copia_original->Id = eqpto->Id;
   //atualiza Eqpto a partir de sua cópia original
   eqpto->CopiaAtributosDe(*copia_original);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAcaoRetira(VTAcao *acao)
   {
   //UndoAcaoRetira() equivale a DoAcaoInsere()
   return(DoAcaoInsere(acao));
   }
//---------------------------------------------------------------------------
bool __fastcall TModifica::UndoAfetaTopologia(VTAcao *acao)
{	//variáveis locais
	bool afetaTopo = false;
	VTEqpto *eqpto;

	//trata Acao de acordo c/ seu tipo
	switch(acao->Tipo())
	{
		case acaoALTERA:
		//determina Eqpto e suas cópias
		eqpto          = acao->Eqpto();
		if (eqpto->Tipo() == eqptoREDE)
		{	//verifica se mudou o atributo estatica
			if (((VTRede*)acao->CopiaDoEqptoOriginal())->Estatica != ((VTRede*) acao->CopiaDoEqptoAlterado())->Estatica)
			{//sinaliza alteração de rede a ser tratada por outro objetos
				afetaTopo = true;
			}
		}
	}
	return(afetaTopo);
}
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
void __fastcall TModifica::UndoVerificaAlteracaoRedeDeLigacao(VTAcao *acao)
   {
   //variáveis locais
   TList *lisACAO = acao->LisAcao();

   //verifica se é uma Acao composta
   if (acao->Tipo() == acaoCOMPOSTA)
      {//loop p/ todas Acoes
      for (int n = 0; n < lisACAO->Count; n++)
         {
         UndoVerificaAlteracaoRedeDeLigacao((VTAcao*)lisACAO->Items[n]);
         }
      }
   else
      {//verifica se é uma Acao de alteração de Ligacao
      if ((acao->Tipo() == acaoALTERA)&&(acao->Eqpto()->TipoLigacao()))
         {
         VTLigacao *ligacao           = (VTLigacao*)acao->Eqpto();
         VTLigacao  *ligacao_original = (VTLigacao*)acao->CopiaDoEqptoOriginal();
         if (ligacao->rede != ligacao_original->rede)
            {
#ifdef _DEBUG
//teste em modo debug =====================
            Aviso("TModifica::UndoVerificaAlteracaoRedeDeLigacao(): redefine rede da cópia do original");
//fim teste em modo debug =================
#endif
            ligacao_original->rede = ligacao->rede;
            }
         }
      }
   }
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TModifica::VerificaImpactoAcao(VTAcao *acao)
   {
   //variáveis locais
   TAction     *action;
   VTGrafico   *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //verifica se Acao pertence a uma Acao composta
   if ((acao != NULL)&&(acao->Pai != NULL)) return;
   //verifica se Acao está fazendo moving de equipamento
   if ((acao != NULL)&&(acao->Moving)) return;
   //verifica se existe o objeto Rebuild
   if (rebuild != NULL)
      {//reconstrói a rede (OBS: Topologia::Reconfigura() já acerta o Mercado)
	  topologia->Rebuild(redes, rebuild->LisRede(), rebuild->LisBarra(), rebuild->LisLigacao());
      }
   else
      {//verifica se a Acao afetou a área das redes
      if (AfetaArea)
         {//recalcula áreas das redes
         redes->CalculaArea();
         }
      //verifica se a Acao afetou a Topologia
      if (AfetaTopologia)
         {//reconfigura Redes (OBS: que também reinicia o Mercado)
         topologia->Reconfigura();
         }
      //verifica se Acao afetou Mercado (OBS: somente se a opologia não foi afetada)
      else if (AfetaMercado)
         {//reinicia taxas de crescimento das Cargas
         mercado->IniciaRedes(redes);
         }
      }
   //verifica se Acao afeta a parte gráfica
   if ((AfetaArea)||(AfetaTopologia)||(AfetaEqpto))
      {//atualiza o gráfico
		if ((grafico != NULL) && (rebuild == NULL)) grafico->AreaRedeAltera();
	  }
   //verifica se a Acao deve ser tratada externamente
   if ((AfetaArea)||(AfetaTopologia)||(AfetaEqpto))
      {//loop p/ todas Action da lista lisACTION
      for (int n = 0; n < lisACTION->Count; n++)
         {
         action = (TAction*)lisACTION->Items[n];
         action->OnExecute(obra);
         }
      }
   }

#ifdef _DEBUG
//teste em modo debug =====================
//---------------------------------------------------------------------------
void __fastcall TModifica::VerificaIntegridade(void)
   {
   //variáveis locais
   VTBarra     *barra;
   VTEqbar     *eqbar;
   VTLigacao   *ligacao;
   VTRede      *rede;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList       *lisBAR    = redes->LisBarra();
   TList       *lisREDE   = redes->LisRede();

   //verifica se as Barras de cada Rede existem em redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede    = (VTRede*)lisREDE->Items[nr];
      lisTMP->Clear();
      rede->LisBarra(lisTMP);
      for (int nb = 0; nb < lisTMP->Count; nb++)
         {
         barra = (VTBarra*)lisTMP->Items[nb];
         if (lisBAR->IndexOf(barra) < 0)
            {
            Aviso("TModifica::VerificaIntegridade() - barra " + barra->Codigo + " não existe na rede");
            }
         }
      }
   //verifica se as Barras dos Eqbar de todas as redes existem em redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede    = (VTRede*)lisREDE->Items[nr];
      lisTMP->Clear();
      rede->LisEqbar(lisTMP);
      for (int n = 0; n < lisTMP->Count; n++)
         {
         eqbar = (VTEqbar*)lisTMP->Items[n];
         if (lisBAR->IndexOf(eqbar->pbarra) < 0)
            {
            Aviso("TModifica::VerificaIntegridade() - barra " + barra->Codigo + " de " +
                   eqbar->TipoAsString() + "=" + eqbar->Codigo + " não existe na rede");
            }
         }
      }
   //verifica se as Barras da Ligacoes de todas as redes existem em redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede    = (VTRede*)lisREDE->Items[nr];
      lisTMP->Clear();
      rede->LisLigacao(lisTMP);
      for (int n = 0; n < lisTMP->Count; n++)
         {
         ligacao = (VTLigacao*)lisTMP->Items[n];
         for (int nb = 0; nb < NBAR_LIG; nb++)
            {
            if ((barra = ligacao->Barra(nb)) == NULL) continue;
            if (lisBAR->IndexOf(barra) < 0)
               {
               Aviso("TModifica::VerificaIntegridade() - barra " + barra->Codigo + " de " +
                     ligacao->TipoAsString() + "=" + ligacao->Codigo + " não existe na rede");
               }
            }
         }
      }
   }
//fim teste em modo debug =================
#endif

//---------------------------------------------------------------------------
//eof

