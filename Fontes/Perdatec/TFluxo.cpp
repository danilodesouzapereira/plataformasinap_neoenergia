//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "ConstantesPerda.h"
#include "TFluxo.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxo(VTApl *apl)
   {
   try{
      return(new TFluxo(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TFluxo::TFluxo(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisBAR   = new TList();
   lisEQP   = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFluxo::~TFluxo(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }
/*
//---------------------------------------------------------------------------
double __fastcall TFluxo::CalculaEnergia_kwh(double p_mw, int num_pat)
   {
   //variáveis locais
   int     dias_mes = 30;
   double  periodo_pat  = 24./num_pat;

   //converte mw ==> kw e calcula energia em 30 dias
   return(((p_mw * 1000.) * periodo_pat) * dias_mes);
   }
*/

//---------------------------------------------------------------------------
VTCorte* __fastcall TFluxo::BarraPertenceCorte(VTRede *rede, VTBarra *barra)
   {
   //variáveis locais
   VTCorte *corte;
   TList   *lisCORTE = rede->LisCorte();

   //verifica se Barra do Eqbar pertence a um Corte
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte  = (VTCorte*)lisCORTE->Items[nc];
      if (corte->ExisteBarra(barra)) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::CalculaEnergia_mwh(double p_mw, int num_pat)
   {
   //variáveis locais
   double  periodo_pat  = 24./num_pat;

   //converte mw ==> mwh
   return(RoundTo(p_mw * periodo_pat, ROUND_PRECISION));
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::CalculaEnergiaCarga(VTRede *rede, TList *lisBAR_CAR)
   {
   //variáveis locais
   TList     *lisREDE_CORTE;
   VTBarra   *barra;
   VTCorte   *corte;
   VTLigacao *ligacao;
   VTRede    *rede_viz;
   int       ind_bar;
   double    p_mw, p_liga_mw;
   int       num_pat = patamares->NumPatamar();

   //inicia energia consumida pelas Cargas da Rede
   energia.carga_mwh = 0.;
   if (lisBAR_CAR->Count == 0) return;
   //inicia potência ativa das Cargas
   p_mw = 0.;
   //loop p/ todas Barras com Cargas em lisCAR
   for (int nb = 0; nb < lisBAR_CAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_CAR->Items[nb];
      //determina Ligacoes da Rede que conectam a Barra
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, barra);
//JCG 2015/10/13
      //verifica se a Barra pertence a algum Corte da Rede
      if ((corte = BarraPertenceCorte(rede, barra)) != NULL)
         {//barra associada a um Corte: verifica se é a Barra inicial da Rede
         if (barra == rede->BarraInicial())
            {//IMPORTANTE: as Cargas desta Barra são alimentadas pela Rede pai
            continue;
            }
         //obtém lista de redes do Corte
         lisREDE_CORTE = corte->LisRede();
         //loop p/ todas redes do Corte
         for (int nr = 0; nr < lisREDE_CORTE->Count; nr++)
            {
            rede_viz = (VTRede*)lisREDE_CORTE->Items[nr];
            //verifica se Rede vizinha é igual à Rede em análise
            if (rede_viz == rede) continue;
            //determina Ligacoes da Rede vizinha que conectam a Barra
            rede_viz->LisLigacao(lisEQP, barra);
            }
         }
//JCG fim de alteração
      //calcula somatória das potências das Ligacoes na Barra
      for (int nl = 0; nl < lisEQP->Count; nl++)
         {
		 ligacao = (VTLigacao*)lisEQP->Items[nl];
		 //determina índice da Barra que conecta a Ligacao
		 ind_bar = ligacao->IndiceBarra(barra);
		 //loop p/ todos os Patamares
		 p_liga_mw = 0.;
		 for (int np = 0; np < num_pat; np++)
			{//soma potencia ativa da Ligacao no patamar
			p_liga_mw += ligacao->resflow->P_mw[ind_bar][np];
			}
		 //acumula potência da Ligacao
		 p_mw += p_liga_mw;
         }
      }
   //calcula energia de Carga (deve ser valor negativo)
   energia.carga_mwh = CalculaEnergia_mwh(p_mw, num_pat);
   }
/*
//---------------------------------------------------------------------------
double __fastcall TFluxo::CalculaEnergaCargasDoCorte(VTCorte *corte)
   {
   //variáveis locais
   int         ind_bar;
   double      p_mw, p_liga_mw;
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTRede      *rede;
   TList       *lisREDE_CORTE = corte->LisRede();
   int         num_pat         = patamares->NumPatamar();

   //determina Barras do Corte que tem Carga
   IniciaLisBarraCarga(corte->LisBarra(), lisBAR);
   //determina energia das Cargas conectadas nas Barras do Corte
   if (lisBAR->Count == 0) return(0.);
   //inicia potência ativa das Cargas
   p_mw = 0.;
   //loop p/ todas Barras com Cargas em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //determina Ligacoes das Redes do Corte que conectam a Barra
      lisEQP->Clear();
      for (int nr = 0; nr < lisREDE_CORTE->Count; nr++)
         {
         rede = (VTRede*)lisREDE_CORTE->Items[nr];
         rede->LisLigacao(lisEQP, barra);
         }
      //calcula somatória das potências das Ligacoes na Barra
      for (int nl = 0; nl < lisEQP->Count; nl++)
         {
         ligacao = (VTLigacao*)lisEQP->Items[nl];
         //determina índice da Barra que conecta a Ligacao
         ind_bar = ligacao->IndiceBarra(barra);
         //loop p/ todos os Patamares
         p_liga_mw = 0.;
         for (int np = 0; np < num_pat; np++)
            {//soma potencia ativa da Ligacao no patamar
            p_liga_mw += ligacao->resflow->P_mw[ind_bar][np];
            }
         //acumula potência da Ligacao
         p_mw += p_liga_mw;
         }
      }
   //calcula energia de Carga (deve ser valor negativo)
   return(CalculaEnergia_mwh(p_mw, num_pat));
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFluxo::CalculaEnergiaGerador(TList *lisGER)
   {
   //variáveis locais
   VTGerador *gerador;
   double    p_mw;
   int       num_pat = patamares->NumPatamar();

   //inicia energia fornecida pelos Geradores da Rede
   energia.gerador_mwh = 0.;
   //loop p/ todos Geradores em lisGER
   for (int n = 0; n < lisGER->Count; n++)
      {
      gerador = (VTGerador*)lisGER->Items[n];
      //calcula energia do Gerador
      p_mw = 0.;
      for (int np = 0; np < num_pat; np++)
         {
		 p_mw += gerador->resflow->P_mw[np];
         }
      energia.gerador_mwh += CalculaEnergia_mwh(p_mw, num_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::CalculaEnergiaRecebidaEntregue(VTRede *rede)
   {
   //variáveis locais
   strLKC    lkc;
   VTCorte   *corte;
   VTLigacao *ligacao;
   TList     *lisCORTE;

   //inicia energia recebida, entregue, trocada pela Rede
   energia.recebida_int_mwh = 0.;
   energia.entregue_ext_mwh = 0.;
   //loop p/ todos Cortes da Rede
   lisCORTE = rede->LisCorte();
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //calcula energia transferida pela Rede no Corte
      FluxoEnergiaDaRedeNoCorte_mwh(rede, corte, lkc);
      //verifica se a potência transferida no Corte é nula
      if (IsDoubleZero(fabs(lkc.Eext_mwh)+fabs(lkc.Eint_mwh), DOUBLE_ZERO_VALUE)) continue;
      //verifica se a energia transferida p/ Rede externa no Corte é positiva ou negativa
      if (lkc.Eext_mwh > 0)
         {//energia recebida (sinal positivo)
		 energia.recebida_ext_mwh += lkc.Eext_mwh;
         }
      else
         {//energia entregue (sinal negativo)
         energia.entregue_ext_mwh += lkc.Eext_mwh;
         }
      //verifica se a energia transferida p/ Rede interna no Corte é positiva ou negativa
      if (lkc.Eint_mwh > 0)
         {//energia recebida (sinal positivo)
         energia.recebida_int_mwh += lkc.Eint_mwh;
         }
      else
         {//energia entregue (sinal negativo)
         energia.entregue_int_mwh += lkc.Eint_mwh;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::CalculaEnergiaSuprimento(TList *lisSUP)
   {
   //variáveis locais
   VTSuprimento *suprimento;
   double    p_mw;
   int       num_pat = patamares->NumPatamar();

   //inicia energia fornecida pelos Suprimentoes da Rede
   energia.suprimento_mwh = 0.;
   //loop p/ todos Suprimentos em lisSUP
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suprimento = (VTSuprimento*)lisSUP->Items[n];
      //calcula energia do Suprimento
      p_mw = 0.;
      for (int np = 0; np < num_pat; np++)
         {
         p_mw += suprimento->resflow->P_mw[np];
         }
      energia.suprimento_mwh += CalculaEnergia_mwh(p_mw, num_pat);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFluxo::CalculaEnergiaTrafo(VTTrafo *trafo)
{
	int       ind_bar;
	double    p_liga_mw;
	int       num_pat = patamares->NumPatamar();
	VTBarra   *barra;

//	energia.recebida_ext_mwh; //valor positivo
//	energia.entregue_ext_mwh; //valor negativo

	//determina índice da Barra do primario
	barra   = trafo->BarraPrimario();
	ind_bar = trafo->IndiceBarra(barra);
	//loop p/ todos os Patamares
	p_liga_mw = 0.;
	for (int np = 0; np < num_pat; np++)
	{	//soma potencia ativa da Ligacao no patamar
		p_liga_mw += trafo->resflow->P_mw[ind_bar][np];
	}
    //calcula energia injetada no trafo
	energia.recebida_ext_mwh = CalculaEnergia_mwh(p_liga_mw, num_pat);

	//determina índice da Barra do secundario
	barra   = trafo->BarraSecundario();
	ind_bar = trafo->IndiceBarra(barra);
	//loop p/ todos os Patamares
	p_liga_mw = 0.;
	for (int np = 0; np < num_pat; np++)
	{	//soma potencia ativa da Ligacao no patamar
		p_liga_mw += trafo->resflow->P_mw[ind_bar][np];
	}
    //calcula energia injetada no trafo
	energia.entregue_ext_mwh = CalculaEnergia_mwh(p_liga_mw, num_pat);
}
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
void __fastcall TFluxo::Executa(TList *lisCORTE)
   {
   //inicia energias
   energia.suprimento_mwh = 0.;
   energia.gerador_mwh    = 0.;
   energia.recebida_mwh   = 0.;
   energia.entregue_mwh   = 0.;
   energia.carga_mwh      = 0.;
   //inicia lisEQP com Suprimentos existentes nas Barras de Corte
   IniciaLisEqbar(lisCORTE, eqptoSUPRIMENTO, lisEQP);
   //calcula energia dos Suprimentos
   CalculaEnergiaSuprimento(lisEQP);
   //inicia lisEQP com Geradores existentes nas Barras de Corte
   IniciaLisEqbar(lisCORTE, eqptoGERADOR, lisEQP);
   //calcula energia dos Geradores
   CalculaEnergiaGerador(lisEQP);
   //inicia lisBAR com as barras do Corte que possuem Carga
   IniciaLisBarraCarga(corte->LisBarra(), lisBAR);
   CalculaEnergiaCarga(lisEQP);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFluxo::Executa(VTRede *rede)
   {
   //variáveis locais
   double energia_erro_fluxo;

   //inicia ponteiros para objetos
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //inicia energias
   energia.suprimento_mwh   = 0.;
   energia.gerador_mwh      = 0.;
   energia.entregue_ext_mwh = 0.;
   energia.entregue_int_mwh = 0.;
   energia.recebida_ext_mwh = 0.;
   energia.recebida_int_mwh = 0.;
   energia.carga_mwh        = 0.;
   //inicia lista de Suprimentos da Rede, excluindo os localizados nas Barras de Corte
   IniciaLisEqbar(rede, eqptoSUPRIMENTO, lisEQP);
   CalculaEnergiaSuprimento(lisEQP);
   //inicia lista de Geradores da Rede, excluindo os localizados nas Barras de Corte
   IniciaLisEqbar(rede, eqptoGERADOR, lisEQP);
   CalculaEnergiaGerador(lisEQP);
   //inicia lista de Barras da Rede que tem Carga
   IniciaLisBarraCarga(rede, lisBAR);
   CalculaEnergiaCarga(rede, lisBAR);
   //calcula energia recebida/entregue
   CalculaEnergiaRecebidaEntregue(rede);
   //limpa listas
   lisBAR->Clear();
   lisEQP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::Executa(VTReducao *reducao)
   {
   //variáveis locais
   double energia_mwhm;

   //inicia ponteiros para objetos
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //inicia energias
   energia.suprimento_mwh   = 0.;
   energia.gerador_mwh      = 0.;
   energia.entregue_ext_mwh = 0.;
   energia.entregue_int_mwh = 0.;
   energia.recebida_ext_mwh = 0.;
   energia.recebida_int_mwh = 0.;
   energia.carga_mwh        = 0.;
   //calcula energia transferida pela Reducao no Corte
   energia_mwhm = FluxoEnergiaDaReducaoNoCorte_mwh(reducao);
   //verifica se a potência transferida no Corte é nula
   if (IsDoubleZero(energia_mwhm, DOUBLE_ZERO_VALUE)) return;
   //verifica se a potência transferida pela Rede no Corte é positiva ou negativa
   else if (energia_mwhm > 0)
      {//energia recebida (sinal positivo)
      energia.recebida_ext_mwh += energia_mwhm;
      }
   else
      {//energia entregue (sinal negativo)
      energia.entregue_ext_mwh += energia_mwhm;
      }
   //limpa listas
   lisBAR->Clear();
   lisEQP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::Executa(VTTrafo *trafo)
   {
   //variáveis locais
   double energia_erro_fluxo;

   //inicia ponteiros para objetos
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //inicia energias
   energia.suprimento_mwh   = 0.;
   energia.gerador_mwh      = 0.;
   energia.entregue_ext_mwh = 0.;
   energia.entregue_int_mwh = 0.;
   energia.recebida_ext_mwh = 0.;
   energia.recebida_int_mwh = 0.;
   energia.carga_mwh        = 0.;

   //calcula energia recebida/entregue
   CalculaEnergiaTrafo(trafo);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFluxo::FluxoEnergiaDaRedeNoCorte_mwh(VTRede *rede, VTCorte *corte, double &Eext_mwh, double &Eint_mwh)
   {
   //variáveis locais
   int         ind_bar;
   double      p_mw, p_viz_ext_mw, p_viz_int_mw, parcela_mwh;
   TList       *lisBAR_CORTE;
   TList       *lisREDE_CORTE;
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTRede      *rede_viz;
   VTNode      *node;
   TList       *lisREDE;
   int         num_pat = patamares->NumPatamar();

   //determina Node da Rede
   node = arvore->ExisteNode(rede);
   //determina lista de Redes do mesmo Node (redes que fazem parte de um mesmo Grupo)
   lisREDE = node->LisObjectNode();
   //inicia potencia transferida pela Rede no Corte
   p_mw =  p_viz_ext_mw = p_viz_int_mw = 0;
   //determina conjunto de Barras do Corte
   lisBAR_CORTE = corte->LisBarra();
   //obtém lista de redes do Corte
   lisREDE_CORTE = corte->LisRede();
   //loop p/ todas redes do Corte
   for (int nr = 0; nr < lisREDE_CORTE->Count; nr++)
      {
      rede_viz = (VTRede*)lisREDE_CORTE->Items[nr];
      //verifica se o fluxo calculado p/ a rede vizinha
      if (! rede_viz->FluxoOK) continue;
      //loop p/ todas Barras do Corte
      for (int nb = 0; nb < lisBAR_CORTE->Count; nb++)
         {
         barra = (VTBarra*)lisBAR_CORTE->Items[nb];
         //determina conjunto de Ligacoes da rede vizinha que conectam a Barra do Corte
         lisEQP->Clear();
         if (rede_viz->Carregada)
            {//obtém Ligacoes da rede
            LisLigacao(rede_viz->LisLigacao(), barra, lisEQP);
            }
         else
            {//obtém Ligacoes do esquemático (que também tem informação de fluxo)
            VTReducao* reducao = (VTReducao*)(rede_viz->MRede->LisReducao()->First());
            LisLigacao(reducao->LisLigacaoEsquematico(), barra, lisEQP);
            }
         //acumula a potencia de cada Ligacao na Barra em todos patamares
         for (int nl = 0; nl < lisEQP->Count; nl++)
            {
            ligacao = (VTLigacao*)lisEQP->Items[nl];
            //determina indice da Barra conectada pela Ligacao
            ind_bar = ligacao->IndiceBarra(barra);
            //loop p/ todos patamares
            for (int np = 0; np < num_pat; np++)
               {//verifica se a rede_viz e própria rede
               if (rede_viz == rede)
                  {//acumula a potencia da Ligacao da Rede na Barra no patamar
                  p_mw += ligacao->resflow->P_mw[ind_bar][np];
                  }
               //verifica se Rede vizinha é interna (pertence à mesma MRede)
               //else if (rede->MRede->MNet == rede_viz->MRede->MNet)
               //else if (rede->MRede == rede_viz->MRede)
               else if (lisREDE->IndexOf(rede_viz) >= 0)
                  {//acumula a potencia da Ligacao da Rede vizinha interna na Barra no patamar
                  p_viz_int_mw += ligacao->resflow->P_mw[ind_bar][np];
                  }
               else
                  {//acumula a potencia da Ligacao da Rede vizinha externa na Barra no patamar
                  p_viz_ext_mw += ligacao->resflow->P_mw[ind_bar][np];
                  }
               }
            }
         }
      }
   //filtra valores muito pequenos
   p_mw         = RoundTo(p_mw,         ROUND_PRECISION);
   p_viz_ext_mw = RoundTo(p_viz_ext_mw, ROUND_PRECISION);
   p_viz_int_mw = RoundTo(p_viz_int_mw, ROUND_PRECISION);
   //zera valores muito pequenos
   if (IsDoubleZero(p_mw,         DOUBLE_ZERO_VALUE)) p_mw = 0.;
   if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE)) p_viz_ext_mw = 0.;
   if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE)) p_viz_int_mw = 0.;
   //análise dos possíveis casos
   if (IsDoubleZero(p_mw, DOUBLE_ZERO_VALUE))
      {//caso 1: não há fluxo da Rede no Corte
      Eext_mwh = Eint_mwh = 0.;
      return(true);
      }
   //verifica se a Rede é supridora no Corte
   if (p_mw < 0)
      {
      if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
         {//caso 2: fluxo da Rede é igual ao inverso do fluxo interno
         Eext_mwh = 0.;
         Eint_mwh = - CalculaEnergia_mwh(p_viz_int_mw, num_pat);
         return(true);
         }
      if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
         {//caso 3: fluxo da Rede é igual ao inverso do fluxo externo
         Eext_mwh = - CalculaEnergia_mwh(p_viz_ext_mw, num_pat);
         Eint_mwh = 0.;
         return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw > 0))
         {//caso 4: fluxo da Rede é igual ao inverso do fluxo externo e interno
		 Eext_mwh = - CalculaEnergia_mwh(p_viz_ext_mw, num_pat);
         Eint_mwh = - CalculaEnergia_mwh(p_viz_int_mw, num_pat);
         return(true);
         }
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw > 0))
         {//caso 5: fluxo da Rede é igual ao inverso de uma parcela do fluxo interno
         parcela_mwh = fabs(p_mw * (p_viz_int_mw / (p_mw + p_viz_ext_mw)));
         Eext_mwh    = 0;
         Eint_mwh    = - CalculaEnergia_mwh(parcela_mwh, num_pat);
		 return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw < 0))
         {//caso 6: fluxo da Rede é igual ao inverso de uma parcela do fluxo externo
         parcela_mwh = fabs(p_mw * (p_viz_ext_mw / (p_mw + p_viz_int_mw)));
         Eext_mwh    = - CalculaEnergia_mwh(parcela_mwh, num_pat);
         Eint_mwh    = 0.;
         return(true);
         }
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw < 0))
         {//caso 7: não há fluxo da Rede no Corte
         Eext_mwh = 0.;
         Eint_mwh = 0.;
         return(true);
         }
      Aviso("FluxoEnergiaDaRedeNoCorte_mwh() - caso supridora não previsto");
      }
   //Rede é consumidora no Corte
   else
      {
      if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
         {//caso 8: fluxo da Rede é igual ao seu fluxo
         Eext_mwh = 0.;
         Eint_mwh = CalculaEnergia_mwh(p_mw, num_pat);
         return(true);
         }
      if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
         {//caso 9: fluxo da Rede é igual ao seu fluxo
         Eext_mwh = CalculaEnergia_mwh(p_mw, num_pat);
         Eint_mwh = 0.;
         return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw > 0))
         {//caso 10: não há fluxo da Rede no Corte (ocorre se houver gerador)
         Eext_mwh = 0.;
         Eint_mwh = 0.;
         return(true);
         }
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw > 0))
         {//caso 11: fluxo da Rede é igual ao inverso de uma parcela do fluxo interno
         parcela_mwh = fabs(p_viz_ext_mw * (p_mw / (p_mw + p_viz_int_mw)));
         Eext_mwh    = p_mw;
         Eint_mwh    = 0;

         return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw < 0))
         {//caso 12: fluxo da Rede é igual ao inverso de uma parcela do fluxo externo
         parcela_mwh = fabs(p_viz_int_mw * (p_mw / (p_mw + p_viz_ext_mw)));
         Eext_mwh    = CalculaEnergia_mwh(parcela_mwh, num_pat);
         Eint_mwh    = 0.;
         return(true);
         }
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw < 0))
         {//caso 13: calcula parcelas
         parcela_mwh = fabs(p_mw * (p_viz_ext_mw / (p_viz_ext_mw + p_viz_int_mw)));
         Eext_mwh    = 0.;
         parcela_mwh = fabs(p_mw * (p_viz_int_mw / (p_viz_ext_mw + p_viz_int_mw)));
         Eint_mwh    = 0.;
         return(true);
         }
      Aviso("FluxoEnergiaDaRedeNoCorte_mwh() - caso consumidora não previsto");
      }
   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TFluxo::FluxoEnergiaDaRedeNoCorte_mwh(VTRede *rede, VTCorte *corte, strLKC &lkc)
   {
   //variáveis locais
   int         ind_bar;
   double      p_mw, p_viz_ext_mw, p_viz_int_mw, parcela_mw;
   TList       *lisBAR_CORTE;
   TList       *lisREDE_CORTE;
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTRede      *rede_viz;
   VTNode      *node;
   TList       *lisREDE;
   int         num_pat = patamares->NumPatamar();

   //determina Node da Rede
   node = arvore->ExisteNode(rede);
   //determina lista de Redes do mesmo Node (redes que fazem parte de um mesmo Grupo)
   lisREDE = node->LisObjectNode();
   //inicia potencia transferida pela Rede no Corte
   p_mw =  p_viz_ext_mw = p_viz_int_mw = 0;
   //determina conjunto de Barras do Corte
   lisBAR_CORTE = corte->LisBarra();
   //obtém lista de redes do Corte
   lisREDE_CORTE = corte->LisRede();
   //loop p/ todas redes do Corte
   for (int nr = 0; nr < lisREDE_CORTE->Count; nr++)
      {
      rede_viz = (VTRede*)lisREDE_CORTE->Items[nr];
      //verifica se o fluxo calculado p/ a rede vizinha
      if (! rede_viz->FluxoOK) continue;
      //loop p/ todas Barras do Corte
      for (int nb = 0; nb < lisBAR_CORTE->Count; nb++)
         {
         barra = (VTBarra*)lisBAR_CORTE->Items[nb];
         //determina conjunto de Ligacoes da rede vizinha que conectam a Barra do Corte
         lisEQP->Clear();
         if (rede_viz->Carregada)
            {//obtém Ligacoes da rede
            LisLigacao(rede_viz->LisLigacao(), barra, lisEQP);
            }
         else
            {//obtém Ligacoes do esquemático (que também tem informação de fluxo)
            VTReducao* reducao = (VTReducao*)(rede_viz->MRede->LisReducao()->First());
            LisLigacao(reducao->LisLigacaoEsquematico(), barra, lisEQP);
            }
         //acumula a potencia de cada Ligacao na Barra em todos patamares
         for (int nl = 0; nl < lisEQP->Count; nl++)
            {
            ligacao = (VTLigacao*)lisEQP->Items[nl];
            //determina indice da Barra conectada pela Ligacao
            ind_bar = ligacao->IndiceBarra(barra);
            //loop p/ todos patamares
            for (int np = 0; np < num_pat; np++)
               {//verifica se a rede_viz e própria rede
               if (rede_viz == rede)
                  {//acumula a potencia da Ligacao da Rede na Barra no patamar
                  p_mw += ligacao->resflow->P_mw[ind_bar][np];
                  }
               //verifica se Rede vizinha é interna (pertence à mesma MRede)
               //else if (rede->MRede->MNet == rede_viz->MRede->MNet)
               //else if (rede->MRede == rede_viz->MRede)
               else if (lisREDE->IndexOf(rede_viz) >= 0)
                  {//acumula a potencia da Ligacao da Rede vizinha interna na Barra no patamar
                  p_viz_int_mw += ligacao->resflow->P_mw[ind_bar][np];
                  }
               else
                  {//acumula a potencia da Ligacao da Rede vizinha externa na Barra no patamar
                  p_viz_ext_mw += ligacao->resflow->P_mw[ind_bar][np];
                  }
               }
            }
         }
      }
   //filtra valores muito pequenos
   p_mw         = RoundTo(p_mw,         ROUND_PRECISION);
   p_viz_ext_mw = RoundTo(p_viz_ext_mw, ROUND_PRECISION);
   p_viz_int_mw = RoundTo(p_viz_int_mw, ROUND_PRECISION);
   //zera valores muito pequenos
   if (IsDoubleZero(p_mw,         DOUBLE_ZERO_VALUE)) p_mw         = 0.;
   if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE)) p_viz_ext_mw = 0.;
   if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE)) p_viz_int_mw = 0.;
   //análise dos possíveis casos
   if (IsDoubleZero(p_mw, DOUBLE_ZERO_VALUE))
      {//caso 1: não há fluxo da Rede no Corte
      lkc.Eext_mwh = lkc.Eint_mwh = 0.;
      return(true);
      }
   //verifica se a Rede é supridora no Corte
   if (p_mw < 0)
      {
      //alterado em 2016.05.20
      if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
         {//caso 2: fluxo da Rede é igual ao inverso do fluxo interno
         lkc.Eext_mwh = 0.;
         if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
            {lkc.Eint_mwh = 0.;}
         else if (p_viz_int_mw  < 0.)
            {lkc.Eint_mwh = 0.;}
         else
            {lkc.Eint_mwh = - CalculaEnergia_mwh(p_viz_int_mw, num_pat);}
         return(true);
         }
      //alterado em 2016.05.20
      if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
         {//caso 3: fluxo da Rede é igual ao inverso do fluxo externo
         lkc.Eint_mwh = 0.;
         if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
            {lkc.Eext_mwh = 0.;}
         else if (p_viz_ext_mw < 0.)
            {lkc.Eext_mwh = 0.;}
         else
            {lkc.Eext_mwh = - CalculaEnergia_mwh(p_viz_ext_mw, num_pat);}
         return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw > 0))
         {//caso 4: fluxo da Rede é igual ao inverso do fluxo externo e interno
		 lkc.Eext_mwh = - CalculaEnergia_mwh(p_viz_ext_mw, num_pat);
         lkc.Eint_mwh = - CalculaEnergia_mwh(p_viz_int_mw, num_pat);
         return(true);
         }
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw > 0))
         {//caso 5: fluxo da Rede é igual ao inverso de uma parcela do fluxo interno
         parcela_mw   = fabs(p_mw * (p_viz_int_mw / (p_mw + p_viz_ext_mw)));
         lkc.Eext_mwh = 0;
         lkc.Eint_mwh = - CalculaEnergia_mwh(parcela_mw, num_pat);
         return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw < 0))
         {//caso 6: fluxo da Rede é igual ao inverso de uma parcela do fluxo externo
         parcela_mw   = fabs(p_mw * (p_viz_ext_mw / (p_mw + p_viz_int_mw)));
         lkc.Eext_mwh = - CalculaEnergia_mwh(parcela_mw, num_pat);
         lkc.Eint_mwh = 0.;
         return(true);
         }
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw < 0))
         {//caso 7: não há fluxo da Rede no Corte
         lkc.Eext_mwh = 0.;
         lkc.Eint_mwh = 0.;
         return(true);
         }
      Aviso("FluxoEnergiaDaRedeNoCorte_mwh() - caso supridora não previsto");
      }
   //Rede é consumidora no Corte
   else
      {
      //alterado em 2016.05.20
      if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
         {//caso 8: fluxo da Rede é igual ao seu fluxo
         lkc.Eext_mwh = 0.;
         if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
            {lkc.Eint_mwh = 0.;}
         else if (p_viz_int_mw > 0)
            {lkc.Eint_mwh = 0.;}
         else
            {lkc.Eint_mwh = CalculaEnergia_mwh(p_mw, num_pat);}
         return(true);
         }
      //alterado em 2016.05.20
      if (IsDoubleZero(p_viz_int_mw, DOUBLE_ZERO_VALUE))
         {//caso 9: fluxo da Rede é igual ao seu fluxo
         lkc.Eint_mwh = 0.;
         if (IsDoubleZero(p_viz_ext_mw, DOUBLE_ZERO_VALUE))
            {lkc.Eext_mwh = 0.;}
         else if (p_viz_ext_mw > 0.)
            {lkc.Eext_mwh = 0.;}
         else
            {lkc.Eext_mwh = CalculaEnergia_mwh(p_mw, num_pat);}
         return(true);
         }
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw > 0))
		 {//caso 10: não há fluxo da Rede no Corte (ocorre se houver gerador)
         lkc.Eext_mwh = 0.;
         lkc.Eint_mwh = 0.;
         return(true);
         }
      //alterado em 2016.05.20
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw > 0))
         {//caso 11: fluxo da Rede é igual ao inverso de uma parcela do fluxo interno
         lkc.Eext_mwh = CalculaEnergia_mwh(p_mw, num_pat);
         lkc.Eint_mwh = 0;
         return(true);
         }
      //alterado em 2016.05.20
      if ((p_viz_ext_mw > 0) && (p_viz_int_mw < 0))
         {//caso 12: fluxo da Rede é igual ao inverso de uma parcela do fluxo externo
         lkc.Eext_mwh = 0.;
         lkc.Eint_mwh = CalculaEnergia_mwh(p_mw, num_pat);
         return(true);
         }
      //alterado em 2016.05.20
      if ((p_viz_ext_mw < 0) && (p_viz_int_mw < 0))
         {//caso 13: calcula parcelas
         parcela_mw   = fabs(p_mw * (p_viz_ext_mw / (p_viz_ext_mw + p_viz_int_mw)));
         lkc.Eext_mwh = CalculaEnergia_mwh(parcela_mw, num_pat);
         parcela_mw   = fabs(p_mw * (p_viz_int_mw / (p_viz_ext_mw + p_viz_int_mw)));
         lkc.Eint_mwh = CalculaEnergia_mwh(parcela_mw, num_pat);
         return(true);
         }
      Aviso("FluxoEnergiaDaRedeNoCorte_mwh() - caso consumidora não previsto");
      }
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::FluxoEnergiaDaReducaoNoCorte_mwh(VTReducao *reducao)
   {
   //variáveis locais
   int         ind_bar;
   double      p_mw;
   VTBarra     *barra;
   VTCorte     *corte;
   VTLigacao   *ligacao;
   TList       *lisBAR;
   TList       *lisLIG;
   int         num_pat = patamares->NumPatamar();

   //detemina Corte da Reducao
   corte = reducao->Corte;
   //determina conjunto de Ligacoes ficticas da Reducao
   lisLIG = reducao->LisLigacaoEsquematico();
   //inicia potencia transferida pela Reducao no Corte
   p_mw = 0;
   //determina conjunto de Barras do Corte
   lisBAR = corte->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //acumula a potencia de cada Ligacao na Barra em todos patamares
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //determina indice da Barra conectada pela Ligacao
         if ((ind_bar = ligacao->IndiceBarra(barra)) < 0) continue;
         //loop p/ todos patamares
         for (int np = 0; np < num_pat; np++)
            {//acumula a potencia da Ligacao na Barra no patamar
            p_mw += ligacao->resflow->P_mw[ind_bar][np];
            }
         }
      }
   return(CalculaEnergia_mwh(p_mw, num_pat));
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::IniciaLisBarraCarga(VTRede *rede, TList *lisBAR_DEST)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR_ORIG = rede->LisBarra();

   //reinicia lisBAR_DEST
   lisBAR_DEST->Clear();
   //loop p/ todas as Barras em lisBAR_ORIG
   for (int n = 0; n < lisBAR_ORIG->Count; n++)
      {
      barra = (VTBarra*)lisBAR_ORIG->Items[n];
//JCG 2015.10.13
      //verifica se é a Barra inicial da Rede
      if (barra == rede->BarraInicial())
         {//verifica se a Barra inicial pertence a um Corte da Rede
         if (BarraPertenceCorte(rede, barra) != NULL) continue;
         }
//JCG fim de alteração
      //verifica se a Barra possui Carga
      if (barra->NumeroDeEqbar(eqptoCARGA) > 0)
         {
         if (lisBAR_DEST->IndexOf(barra) < 0) lisBAR_DEST->Add(barra);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::IniciaLisEqbar(TList *lisCORTE, int tipo_eqbar, TList *lisEQB)
   {
   //variáveis locais
   VTCorte *corte;

   //reinicia lisEQB
   lisEQB->Clear();
   //loop p/ todos Cortes
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte  = (VTCorte*)lisCORTE->Items[nc];
      //obtém Eqbar do Corte
      corte->LisEqbar(lisEQB, tipo_eqbar);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::IniciaLisEqbar(VTRede *rede, int tipo_eqbar, TList *lisEQB)
   {
   //variáveis locais
   VTEqbar *eqbar;
   VTCorte *corte;
   TList   *lisCORTE;

   //reinicia lisEQB
   lisEQB->Clear();
   //obtém lista de Cortes da Rede
   lisCORTE = rede->LisCorte();
   //obtém lista de Eqbar da Rede
   rede->LisEqbar(lisEQB, tipo_eqbar, false);
   //elimina Eqbar localizados nas Barras de Corte
   for (int n = lisEQB->Count-1; n >= 0; n--)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      //verifica se Barra do Eqbar pertence a um Corte
      for (int nc = 0; nc < lisCORTE->Count; nc++)
         {
         corte  = (VTCorte*)lisCORTE->Items[nc];
         if (corte->ExisteBarra(eqbar->pbarra))
            {//retira Eqbar de lisEQB
            lisEQB->Delete(n);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxo::LisLigacao(TList *lisLIG, VTBarra *barra, TList *lisEQP)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //determina as Ligacoes em lisLIG que conectam a Barra
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->ConectaBarra(barra))
         {//insere Ligacao em lisEQP
         if (lisEQP->IndexOf(ligacao) < 0) lisEQP->Add(ligacao);
         }
      }
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaCarga_mwh(void)
   {
   return(energia.carga_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaEntregueExt_mwh(void)
   {
   return(energia.entregue_ext_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaEntregueInt_mwh(void)
   {
   return(energia.entregue_int_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaGerador_mwh(void)
   {
   return(energia.gerador_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaRecebidaExt_mwh(void)
   {
   return(energia.recebida_ext_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaRecebidaInt_mwh(void)
   {
   return(energia.recebida_int_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxo::PM_GetEnergiaSuprimento_mwh(void)
   {
   return(energia.suprimento_mwh);
   }

//---------------------------------------------------------------------------
//eof



