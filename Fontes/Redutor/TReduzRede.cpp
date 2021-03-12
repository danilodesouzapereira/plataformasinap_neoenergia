//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TReduzRede.h"
#include "VTMontaEqv.h"
#include "VTRedutor.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\Monta\VTMonta.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Rede\VTReducao.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReduz
//---------------------------------------------------------------------------
VTReduz* __fastcall NewObjReduzRede(VTApl *apl)
   {
   return(new TReduzRede(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReduzRede::TReduzRede(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //inicia dados
   num_ger_sup = 0;
   //cria objetos
   monta_eqv  = NewObjMontaEqv(apl);
   monta      = DLL_NewObjMonta(apl);
   ordena     = DLL_NewObjOrdena(apl);
   lisMR      = new TList();
   lisREDU    = new TList();
   lisCORTE   = new TList();
   lisBAR     = new TList();
   lisCAR     = new TList();
   lisTMP     = new TList();
   //lista p/ os Eqptos da parte redutível da Rede
   redutivel.lisBAR_ALL = new TList();
   redutivel.lisBAR_COR = new TList();
   redutivel.lisBAR_GER = new TList();
   redutivel.lisLIG_ALL = new TList();
   redutivel.lisSUP_GER = new TList();
   redutivel.lisMUTUA   = new TList();
   redutivel.lisCAR     = new TList();
   //inicia dados
   ModeloEqv = eqvEQUILIBRADO;
   }

//---------------------------------------------------------------------------
__fastcall TReduzRede::~TReduzRede(void)
   {
   //destrói objetos criados
   if (monta_eqv)  {delete monta_eqv;  monta_eqv  = NULL;}
   if (monta)      {delete monta;      monta      = NULL;}
   if (ordena)     {delete ordena;     ordena     = NULL;}
   //destrói lista sem destrui seus objetos
   if (lisMR)      {delete lisMR;      lisMR      = NULL;}
   if (lisREDU)    {delete lisREDU;    lisREDU    = NULL;}
   if (lisCORTE)   {delete lisCORTE;   lisCORTE   = NULL;}
   if (lisBAR)     {delete lisBAR;     lisBAR     = NULL;}
   if (lisCAR)     {delete lisCAR;     lisCAR     = NULL;}
   if (lisTMP)     {delete lisTMP;     lisTMP     = NULL;}
   if (redutivel.lisBAR_ALL) {delete redutivel.lisBAR_ALL; redutivel.lisBAR_ALL = NULL;}
   if (redutivel.lisBAR_COR) {delete redutivel.lisBAR_COR; redutivel.lisBAR_COR = NULL;}
   if (redutivel.lisBAR_GER) {delete redutivel.lisBAR_GER; redutivel.lisBAR_GER = NULL;}
   if (redutivel.lisLIG_ALL) {delete redutivel.lisLIG_ALL; redutivel.lisLIG_ALL = NULL;}
   if (redutivel.lisSUP_GER) {delete redutivel.lisSUP_GER; redutivel.lisSUP_GER = NULL;}
   if (redutivel.lisMUTUA)   {delete redutivel.lisMUTUA;   redutivel.lisMUTUA   = NULL;}
   if (redutivel.lisCAR)     {delete redutivel.lisCAR;     redutivel.lisCAR     = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::CargaDesprezivel(VTCarga* pcarga)
   {

   //variáveis locais
   int        max_pat = patamares->NumPatamar();
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //analisa demanda de todos os patamares
   for (int np = 0; np < max_pat; np++)
      {
      if (Abs(demanda->S_mva[pcarga][np]) > 1.e-10) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::CorteIsolado(VTCorte *corte)
   {
   //variáveis locais
   int     ind_pat = 0;
   VTBarra *barra;
   TList   *lisBAR_COR = corte->LisBarra();

   //loop p/ todas Barras do Corte
   for (int nb = 0; nb < lisBAR_COR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_COR->Items[nb];
      if (barra->resflow->Diagnostico[ind_pat] == diagISOLADO) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::CriaCurva(VTCarga *pcarga)
   {
   //variáveis locais
   VTCurva *curva;

   //cria uma nova Curva
   if ((curva = DLL_NewObjCurva()) == NULL) return(false);
   //associa Curva c/ a Carga
   pcarga->DefineCurva(curva);
   //define dados da Curva
   curva->Codigo        = pcarga->Codigo;
   curva->Id            = pcarga->Id;    //MUITO IMPORTANTE
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patamares->NumPatamar();
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRCAR;  //{p,q} por fase
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::CriaCurva(VTSuprimento *psup)
   {
   //variáveis locais
   VTCurva *curva;

   //cria uma nova Curva
   if ((curva = DLL_NewObjCurva()) == NULL) return(false);
   //associa Curva c/ a Carga
   psup->DefineCurva(curva);
   //define dados da Curva
   curva->Codigo        = psup->Codigo;
   curva->Id            = psup->Id;      //MUITO IMPORTANTE
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidPU;
   curva->Escala        = patamares->NumPatamar();
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRSUP;  //{p,q,v,t} por fase
   return(true);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzRede::Executa(VTMNet *mnet, VTMRede *mrede,
                                          VTCorte *corte, bool aviso_enabled)
   {
   //variáveis locais
   //bool      tramo_enabled = false;
   VTReducao *reducao;

   //define MRede como NÃO redutivel
   mrede->Redutivel = false;

//TESTE
//VTEqpto *rede = (VTEqpto*)(mrede->LisRede()->First());
//AnsiString codigo = rede->Codigo;

   //proteção contra Corte entre redes que estão isoladas (sem geração)
   if (CorteIsolado(corte))
      {//MRede não pode ser reduzida p/ o Corte
      reducao = MontaReducaoInvalida();
      return(reducao);
      }
   //determina número de geradores e suprimentos existentes na MNet
   lisTMP->Clear();
   mnet->LisEqbar(lisTMP, eqptoSUPRIMENTO);
   mnet->LisEqbar(lisTMP, eqptoGERADOR);
   num_ger_sup = lisTMP->Count;
   //no cálculo da redução, é necessário:
   //a) identificar conjunto de MRedes e Reducoes da parte redutível:
   //b) identificar conjuntos de Eqptos da parte redutível:
   //    1) conjunto de todas Barras, Ligacoes, Geradores e Suprimentos
   //    2) conjunto de Barras do Corte
   //    3) conjunto de Barras que será mantida na Reducao: Barras do Corte e Barras c/ geração
   //c) montar a Net p/ redução: isto é feito pelo objeto Ordena
   //d) ajustar os Nos das Barras dos Cortes: para Nos envolvendo Barras
   //   de Cortes, a Barra pai deve ser uma Barra do Corte
   //e) calcular a rede equivalente
   //f) criar a Reducao da rede equivalente, a partir das Barras notáveis

   //a) identificar conjunto de MRedes e de Reducoes de MNet que formam a parte redutível
   SeparaParteRedutivel(mnet, mrede, corte, lisMR, lisREDU);
   //b) identificar conjuntos de Eqptos da parte redutível
   IniciaListasParteRedutivel(corte, lisMR, lisREDU);
   //proteção: verifica a viabilidade da Reducao da MRede p/ o Corte
   if (redutivel.lisBAR_ALL->Count <= (redutivel.lisBAR_COR->Count + redutivel.lisBAR_GER->Count))
      {//MRede não pode ser reduzida p/ o Corte
      reducao = MontaReducaoInvalida();
      return(reducao);
      }
   //c) criar VTNet p/ a MNet
   //if ((net = ordena->Executa(mnet, tramo_enabled)) == NULL) return(NULL);
   //c) cria VTNet somente com a parte redutível da rede
   //if ((net = ordena->Executa(redutivel.lisBAR_ALL, redutivel.lisCAR, redutivel.lisLIG_ALL,
   //                           redutivel.lisMUTUA, tramo_enabled)) == NULL) return(NULL);
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = false;
   ordena->TramoEnabled    = false;
   ordena->ReguladorBypass = true;
   if ((net = ordena->Executa(redutivel.lisBAR_ALL, redutivel.lisCAR, redutivel.lisLIG_ALL,
                              redutivel.lisMUTUA)) == NULL) return(NULL);
   //d) ajustar Nos das Barras dos Cortes
   //RedefineBarraPaiNoDeCorte(mnet);
   RedefineBarraPaiNoDeCorte(corte);
   //tratamento adicional p/ Barras de Corte conectadas por Chave
   //EM DESENVOLVIMENTO
   //TrataBarrasCorteConectadasPorChave();
   //define modelo de carga p/ as Cargas equivalentes que serão criadas
   MontaModeloCargaEquivalente(redutivel.lisCAR);
   //f) calcula a rede equivalente
   if (MontaRedeEquivalente(aviso_enabled))
      {//g) cria a Reducao da rede equivalente
      reducao = MontaReducao(corte);
      //mercado: monta uma única curva de Crescimento p/ todas as Cargas equivalentes da Reducao
      MontaCrescimentoEquivalente(reducao, redutivel.lisCAR);
      //define MRede como redutivel
      mrede->Redutivel = true;
      }
   else
      {//MRede não pode ser reduzida p/ o Corte
      reducao = MontaReducaoInvalida();
      return(reducao);
      }
   return(reducao);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::ExisteLigacaoConectandoBarras(TList *lisLIG, VTBarra *barra1, VTBarra *barra2)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;

   //monta lisTMP c/ as Ligacoes do No em lisLIG que pertencem à parte redutivel da rede
   lisTMP->Clear();
   for (int nc = 0; nc < lisLIG->Count; nc++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nc];
      //verifica se a Chave pertence à parte redutível
      if (redutivel.lisLIG_ALL->IndexOf(ligacao) >= 0) lisTMP->Add(ligacao);
      }
   //verifica se as Ligacoes em lisTMP estabelecem um caminho elétrico entre as Barras indicadas
   lisBAR->Clear();
   lisBAR->Add(barra1);
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra1 = (VTBarra*)lisBAR->Items[nb];
      //loop p/ todas as Ligacoes em lisTMP
      for (int nc = lisTMP->Count-1; nc >= 0; nc--)
         {
         ligacao = (VTLigacao*)lisTMP->Items[nc];
         //verifica se a Chave conecta a Barra analisada
         if (! ligacao->ConectaBarra(barra1)) continue;
         //retira Ligacao de lisTMP
         lisTMP->Delete(nc);
         //determina barra oposta da Barra
         barra = (ligacao->pbarra1 == barra) ? ligacao->pbarra2 : ligacao->pbarra1;
         //verifica se a Barra oposta da Chave é a Barra2
         if (barra == barra2) return(true);
         //insere Barra oposta da Chave em lisBAR
         lisBAR->Add(barra);
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::ExisteDesequilibrio(void)
   {
   return(net->ExisteDesequilibrio());
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::IniciaListasParteRedutivel(VTCorte *corte, TList *lisMR, TList *lisREDU)
   {
   //variáveis locais
   bool      barra_isolada = false;
   TList     *lisBAR_CORTE;
   VTBarra   *barra;
   VTEqbar   *eqbar;
   VTMRede   *mrede;
   VTReducao *reducao;

   //reinicia listas da parte redutivel
   redutivel.lisBAR_ALL->Clear();
   redutivel.lisBAR_COR->Clear();
   redutivel.lisBAR_GER->Clear();
   redutivel.lisLIG_ALL->Clear();
   redutivel.lisSUP_GER->Clear();
   redutivel.lisMUTUA->Clear();
   redutivel.lisCAR->Clear();
   //inicia listas da parte redutível c/ os eqptos das MRede em lisMREDE
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //insere as Barras da MRede em lisBAR_ALL
      mrede->LisBarra(redutivel.lisBAR_ALL, barra_isolada);
      //insere as Ligacoes da MRede em lisLIG_ALL
      mrede->LisLigacao(redutivel.lisLIG_ALL);
      //insere em lisSUP_GER os Geradores e Suprimentos da MRede
      mrede->LisEqbar(redutivel.lisSUP_GER, eqptoSUPRIMENTO);
      mrede->LisEqbar(redutivel.lisSUP_GER, eqptoGERADOR);
      //insere em lisCAR as Cargas da MRede
      mrede->LisEqbar(redutivel.lisCAR, eqptoCARGA);
      //insere Mutuas em  lisMUTUA
      mrede->LisMutua(redutivel.lisMUTUA);
      }
   //atualiza listas da parte redutível c/ os eqptos das Reducoes em lisREDU
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      //insere as Barras da Reducao em lisBAR_ALL
      reducao->LisBarra(redutivel.lisBAR_ALL);
      //insere as Ligacoes da Reducao em lisLIG_ALL
      reducao->LisLigacao(redutivel.lisLIG_ALL);
      //insere em lisSUP_ALL os Suprimentos da MRede
      reducao->LisEqbar(redutivel.lisSUP_GER, eqptoSUPRIMENTO);
      //insere em lisCAR as Cargas da Reducao
      reducao->LisEqbar(redutivel.lisCAR, eqptoCARGA);
      //OBS: redução não possui mútuas
      }
   //inicia lisBAR_COR c/ Barras do Corte
   //CopiaTList(corte->LisBarra(), redutivel.lisBAR_COR);
   //inicia lisBAR_COR c/ Barras do Corte que existem na parte redtível
   lisBAR_CORTE = corte->LisBarra();
   for (int n = 0; n < lisBAR_CORTE->Count; n++)
      {
      barra = (VTBarra*)lisBAR_CORTE->Items[n];
      if (redutivel.lisBAR_ALL->IndexOf(barra) >= 0) redutivel.lisBAR_COR->Add(barra);
      }
   //inicia lisBAR_GER c/ Barras de Geradores e de Suprimentos em lisSUP_GER
   for (int n = 0; n < redutivel.lisSUP_GER->Count; n++)
      {
      eqbar = (VTEqbar*)redutivel.lisSUP_GER->Items[n];
      //verifica se a Barra do Eqbar já existe em lisBAR_COR ou em lisBAR_GER
      barra = eqbar->pbarra;
      if (redutivel.lisBAR_COR->IndexOf(barra) >= 0) continue;
      if (redutivel.lisBAR_GER->IndexOf(barra) >= 0) continue;
      //insere Barra em lisBAR_GER
      redutivel.lisBAR_GER->Add(barra);
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TReduzRede::IniciaLisCarga(TList *lisMR, TList *lisCAR)
   {
   //variáveis locais
   VTMRede   *mrede;

   //reincia lisCAR
   lisCAR->Clear();
   //loop p/ todas MRede da parte redutível
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //obtém lista de Cargas da MRede
      mrede->LisEqbar(lisCAR, eqptoCARGA);
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TReduzRede::IniciaLisCargaParteRedutivel(TList *lisMR, TList *lisREDU, TList *lisCAR)
   {
   //variáveis locais
   VTMRede   *mrede;
   VTReducao *reducao;

   //reincia lisCAR
   lisCAR->Clear();
   //loop p/ todas MRede da parte redutível
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //obtém lista de Cargas da MRede
      mrede->LisEqbar(lisCAR, eqptoCARGA);
      }
   //proteção: lisREDU pode ser NULL
   if (lisREDU == NULL) return;
   //loop p/ todas Reducao em lisREDU
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      //obtém lista de Cargas da Reducao
      reducao->LisEqbar(lisCAR, eqptoCARGA);
      }
   }
 */
//---------------------------------------------------------------------------
void __fastcall TReduzRede::IniciaLisCorte(VTMNet *mnet, TList *lisCOR)
   {
   //variáveis locais
   VTMRede *mrede;
   TList   *lisMR;

   //reinicia lisCOR c/ os Cortes da MNet
   lisCOR->Clear();
   mnet->LisCorte(lisCOR);
   //insere em lisCOR os Cortes internos da MNet: Cortes entre as MRede da MNet
   lisMR = mnet->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //inclui em lisCOR a lista de Cortes da MRede
      mrede->LisCorte(lisCOR);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereCarga(VTReducao *reducao, TList *lisBAR_GER)
   {
   //variáveis locais
   int             max_pat = patamares->NumPatamar();
   complex<double> s_mva, vetor_s_mva[MAX_FASE];
   VTBarra         *barra;
   VTCarga         *carga, *carga_eqv;
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop p/ todas Barras de greação
   for (int nb = 0; nb < lisBAR_GER->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_GER->Items[nb];
      //obtém lista de Crgas da Barra
      lisCAR->Clear();
      barra->LisEqbar(lisCAR, eqptoCARGA);
      //loop todas Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //cria Carga equivalente
         carga_eqv = carga->Clone();
         //associa Carga c/ Barra pai do No
         carga_eqv->DefineObjBarra(barra->No[net]->pai);
         //define que é uma Carga de Reducao
         carga_eqv->Id      = FalsoId();
         carga_eqv->Reducao = true;
         //cria uma nova Curva de demanda da Carga quivalente
         if (! CriaCurva(carga_eqv)) continue;
         //copia curva de demanda
         for (int np = 0; np < max_pat; np++)
            {
            //determina demandas da carga por fase
            s_mva = demanda->S_mva[carga][np] / 3.;
            for (int nf = 0; nf < MAX_FASE-1; nf++)
               {
               vetor_s_mva[nf] = s_mva;
               }
            vetor_s_mva[MAX_FASE-1] = CZero();
            demanda->Set_S3f_mva(carga_eqv, np, vetor_s_mva);
            }
         //insere Carga na Reducao
         reducao->InsereEqbar(carga_eqv);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereCargaEqv(VTReducao *reducao, TList *lisCAR)
   {
   //variáveis locais
   VTCarga *pcarga, *pc;

   //insere Cargas na Reducao
   while (lisCAR->Count > 0)
      {//retira primeira Carga da lista
      pcarga   = (VTCarga*)lisCAR->First();
      lisCAR->Delete(0);
      //verifica se a Barra da Carga está definida
      if (pcarga->pbarra == NULL) {delete pcarga; continue;}
      //verifica se a Carga possui valor desprezível
      if (CargaDesprezivel(pcarga)) {delete pcarga; continue;}
      //NÃO redefine modelo de carga como Icte
      //pcarga->Icte_pu = 1.;
      //pcarga->Scte_pu = 0.;
      //pcarga->Zcte_pu = 0.;
      /*
      //<JCG> 16/10/2009
      1) As Cargas equivalentes geradas nas barras de Corte
         contém apenas o delta de carga (não incluem as Cargas pré-existentes na Barra )
      2) As Cargas equivalentes geradas nas barras com gerador/suprimento
         contém  apenas o delta de carga (não incluem as Cargas pré-existentes na Barra )
      //verifica se a Barra da Carga equivalente pertence a um No de geração
      if (redutivel.lisBAR_GER->IndexOf(pcarga->pbarra->No[net]->pai) >= 0)
         {//soma as demandas das Cargas existentes no No na Carga equivalente
         SomaCargasDoNoNaCargaEquivalente(pcarga, pcarga->pbarra->No[net]);
         }
      */
      //insere Carga na Reducao
      reducao->InsereEqbar(pcarga);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereChaveEqv(VTReducao *reducao)
   {
   //variáveis locais
   VTArranjo *arranjo;
   VTBarra   *barra1, *barra2;
   VTTrecho  *trecho;

   //loop p/ todas Barras do Corte
   for (int i = 0; i < redutivel.lisBAR_COR->Count; i++)
      {
      barra1 = (VTBarra*)redutivel.lisBAR_COR->Items[i];
      //loop p/ as demais Barras (i+1)
      for (int j = i+1; j < redutivel.lisBAR_COR->Count; j++)
         {
         barra2 = (VTBarra*)redutivel.lisBAR_COR->Items[j];
         //verifica se as duas Barras pertencem ao mesmo No
         if (barra1->No[net] == barra2->No[net])
            {//verifica se alguma Ligacao do No pertence à parte redutível e conecta as duas Barras
            if (! ExisteLigacaoConectandoBarras(barra1->No[net]->lisLIG, barra1, barra2)) continue;
            //cria uma Chave fechada entre as 2 barras
            if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL) continue;
            if ((arranjo = DLL_NewObjArranjo(trecho)) == NULL) continue;
            //inicia dados do Trecho
            trecho->Id             = FalsoId();
            trecho->Comprimento_km = 1;
            //inicia dados do Arranjo do Trecho
            arranjo->TipoCfg  = arranjoZ0Z1;
            arranjo->z1.r     = 1e-6;
            arranjo->z1.x     = 0.;
            arranjo->z0       = arranjo->z1;
            //define posição nas Barras
            trecho->esq.posbar1 = 50;
            trecho->esq.posbar2 = 50;
            //associa Trecho c/ Barra pai do Nos
            trecho->DefineObjBarra(barra1, barra2);
            //associa Trecho c/ seu Arranjo
            trecho->DefineObjArranjo(arranjo);
            //insere Trecho na Reducao
            reducao->InsereLigacao(trecho);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereSuprimento(VTReducao *reducao, VTCorte *corte)
   {
   //variáveis locais
   VTEqbar *eqbar;
   VTBarra *pbarra;

   //cria Suprimentos correspondentes aos Suprimentos salvos em lisSUP_GER
   for (int n = 0; n < redutivel.lisSUP_GER->Count; n++)
      {
      eqbar  = (VTEqbar*)redutivel.lisSUP_GER->Items[n];
      pbarra = eqbar->pbarra;
      //verifica se a Barra pertence ao Corte
      if (corte->ExisteBarra(pbarra)) continue;
      //verifica se é Suprimento ou Gerador
      if (eqbar->Tipo() == eqptoSUPRIMENTO)
         {//cria Suprimento equivalente
         if ((eqbar = InsereSuprimentoEqv((VTSuprimento*)eqbar)) != NULL)
            {//insere Suprimento na Reducao
            reducao->InsereEqbar(eqbar);
            }
         }
      else if (eqbar->Tipo() == eqptoGERADOR)
         {//cria Suprimento equivalente
         if ((eqbar = InsereSuprimentoEqv((VTGerador*)eqbar)) != NULL)
            {//insere Suprimento na Reducao
            reducao->InsereEqbar(eqbar);
            }
         }
      }
   }
   
//---------------------------------------------------------------------------
VTSuprimento* __fastcall TReduzRede::InsereSuprimentoEqv(VTGerador* pger)
   {
   //variáveis locais
   int            max_pat;
   strSUP         vet_sup[MAX_FASE];
   VTSuprimento   *psup_eqv;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //cria um Novo Suprimento equivalente
   if ((psup_eqv = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQVT p/ o Suprimento equivalente
   if (! CriaCurva(psup_eqv)) {delete psup_eqv; return(NULL);}
   //associa Suprimento equivalente c/ Barra pai do No
   psup_eqv->DefineObjBarra(pger->pbarra->No[net]->pai);
   //inicia dados do Suprimento
   psup_eqv->Id       = FalsoId();
   psup_eqv->Reducao  = true;
   psup_eqv->esq      = pger->esq;
   psup_eqv->tiposup  = pger->tiposup;
   psup_eqv->smax     = pger->snom;
   psup_eqv->pcc_3f.p = 0.;
   psup_eqv->pcc_3f.q = 0.;
   psup_eqv->pcc_ft.p = 0.;
   psup_eqv->pcc_ft.q = 0.;
   psup_eqv->zeq0     = pger->z0;
   psup_eqv->zeq1     = pger->z1;
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for(int np = 0; np < max_pat; np++)
      {//obtém dados de fornecimento do Gerador no patamar (valores trifásicos)
      fornecimento->Get_PQVT_porfase(pger, np, vet_sup);
      // Salva dados em 'psup_eqv'
      fornecimento->Set_PQVT_porfase(psup_eqv, np, vet_sup);
      }
   return(psup_eqv);
   }

//---------------------------------------------------------------------------
VTSuprimento* __fastcall TReduzRede::InsereSuprimentoEqv(VTSuprimento* psup)
   {
   //variáveis locais
   int            max_pat;
   strSUP         vet_sup[MAX_FASE];
   VTSuprimento   *psup_eqv;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //cria um Novo Suprimento equivalente
   if ((psup_eqv = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQVT p/ o Suprimento equivalente
   if (! CriaCurva(psup_eqv)) {delete psup_eqv; return(NULL);}
   //associa Suprimento equivalente c/ Barra pai do No
   psup_eqv->DefineObjBarra(psup->pbarra->No[net]->pai);
   //inicia dados do Suprimento
   psup_eqv->Id      = FalsoId();
   psup_eqv->Reducao = true;
   psup_eqv->tiposup = psup->tiposup;
   psup_eqv->smax    = psup->smax;
   psup_eqv->pcc_3f  = psup->pcc_3f;
   psup_eqv->pcc_ft  = psup->pcc_ft;
   psup_eqv->zeq0    = psup->zeq0;
   psup_eqv->zeq1    = psup->zeq1;
   psup_eqv->esq     = psup->esq;
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//obtém dados de fornecimento do Suprimento no patamar (valores trifásicos)
      fornecimento->Get_PQVT_porfase(psup, np, vet_sup);
      // Salva dados em 'psup_eqv'
      fornecimento->Set_PQVT_porfase(psup_eqv, np, vet_sup);
      }
   return(psup_eqv);
   }

/*
//---------------------------------------------------------------------------
VTSuprimento* __fastcall TReduzRede::InsereSuprimentoEqv(VTGerador* pger)
   {
   //variáveis locais
   int             max_pat;
   VTSupFase      *sup_fase;
   VTSuprimento   *psup_eqv;
   VTListaSupFase *listaSF;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //cria um Novo Suprimento equivalente
   if ((psup_eqv = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQVT p/ o Suprimento equivalente
   if (! CriaCurva(psup_eqv)) {delete psup_eqv; return(NULL);}
   //associa Suprimento equivalente c/ Barra pai do No
   psup_eqv->DefineObjBarra(pger->pbarra->No[net]->pai);
   //inicia dados do Suprimento
   psup_eqv->Id       = FalsoId();
   psup_eqv->Reducao  = true;
   psup_eqv->esq      = pger->esq;
   psup_eqv->tiposup  = pger->tiposup;
   psup_eqv->smax     = pger->snom;
   psup_eqv->pcc_3f.p = 0.;
   psup_eqv->pcc_3f.q = 0.;
   psup_eqv->pcc_ft.p = 0.;
   psup_eqv->pcc_ft.q = 0.;
   psup_eqv->zeq0     = pger->z0;
   psup_eqv->zeq1     = pger->z1;
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//obtém dados de fornecimento do Gerador no patamar (valores trifásicos)
      listaSF = fornecimento->PQVT[pger][np];
      for (int nf = 0; nf < listaSF->Count; nf++)
         {
         sup_fase = listaSF->Items[nf];
         //verifica se existe um único Suprimento/Gerador na MNet
         if (num_ger_sup == 1)
            {//inclui no Suprimento equivalente o ângulo da Zona
            sup_fase->T_rad = sup_fase->T_rad + pger->pbarra->zona->Defasagem_rad;
            }
         else
            {//NÃO inclui no Suprimento equivalente o ângulo da Zona
            sup_fase->T_rad = sup_fase->T_rad;
            }
         }
      //inicia fornecimento do Suprimento no patamar
      fornecimento->PQVT[psup_eqv][np] = listaSF;
      }
   return(psup_eqv);
   }

//---------------------------------------------------------------------------
VTSuprimento* __fastcall TReduzRede::InsereSuprimentoEqv(VTSuprimento* psup)
   {
   //variáveis locais
   int             max_pat;
   VTSupFase      *sup_fase;
   VTListaSupFase *listaSF;
   VTSuprimento   *psup_eqv;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //cria um Novo Suprimento equivalente
   if ((psup_eqv = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQVT p/ o Suprimento equivalente
   if (! CriaCurva(psup_eqv)) {delete psup_eqv; return(NULL);}
   //associa Suprimento equivalente c/ Barra pai do No
   psup_eqv->DefineObjBarra(psup->pbarra->No[net]->pai);
   //inicia dados do Suprimento
   psup_eqv->Id      = FalsoId();
   psup_eqv->Reducao = true;
   psup_eqv->tiposup = psup->tiposup;
   psup_eqv->smax    = psup->smax;
   psup_eqv->pcc_3f  = psup->pcc_3f;
   psup_eqv->pcc_ft  = psup->pcc_ft;
   psup_eqv->zeq0    = psup->zeq0;
   psup_eqv->zeq1    = psup->zeq1;
   psup_eqv->esq     = psup->esq;
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//obtém dados de fornecimento do Suprimento no patamar (valores trifásicos)
      listaSF = fornecimento->PQVT[psup][np];
      //verifica se o Suprimento original não é de uma Reducao
      if (! psup->Reducao)
         {
         for (int nf = 0; nf < listaSF->Count; nf++)
            {
            sup_fase = listaSF->Items[nf];
            //verifica se existe um único Suprimento/Gerador na MNet
            if (num_ger_sup == 1)
               {//inclui no Suprimento equivalente o ângulo da Zona
               sup_fase->T_rad = sup_fase->T_rad + psup->pbarra->zona->Defasagem_rad;
               }
            else
               {//NÃO inclui no Suprimento equivalente o ângulo da Zona
               sup_fase->T_rad = sup_fase->T_rad;
               }
            }
         }
      //inicia fornecimento do Suprimento equivalente no patamar
      fornecimento->PQVT[psup_eqv][np] = listaSF;
      }
   return(psup_eqv);
   }
*/

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereTrafoEqv(VTReducao *reducao, TList *lisTRF)
   {
   //variáveis locais
   VTTrafo  *ptrafo;

   //insere Trafos na Reducao
   while (lisTRF->Count > 0)
      {//retira primeiro Trafo da lista
      ptrafo = (VTTrafo*)lisTRF->First();
      lisTRF->Delete(0);
      //verifica se as Barras do Trafo estão definidas
      if ((ptrafo->pbarra1 == NULL)||(ptrafo->pbarra2 == NULL))
         {
         delete ptrafo;
         continue;
         }
      //insere Trafo na Reducao
      reducao->InsereLigacao(ptrafo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereTrechoEqv(VTReducao *reducao, TList *lisTRE)
   {
   //variáveis locais
   VTTrecho *ptrecho;

   //insere Trechos na Reducao
   while (lisTRE->Count > 0)
      {//retira primeiro Trecho da lista
      ptrecho   = (VTTrecho*)lisTRE->First();
      lisTRE->Delete(0);
      //verifica se as Barras do Trecho estão definidias
      if ((ptrecho->pbarra1 == NULL)||(ptrecho->pbarra2 == NULL))
         {
         delete ptrecho;
         continue;
         }
      //insere Trecho na Reducao
      reducao->InsereLigacao(ptrecho);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzRede::InsereYrefEqv(VTReducao *reducao, TList *lisYREF)
   {
   //variáveis locais
   complex<double> y1, y2, z1;
   VTNo           *pno;
   VTYref         *yref1, *yref2;
   //VTCorte        *corte = reducao->Corte;

   //insere Cargas na Reducao
   while (lisYREF->Count > 0)
      {//retira primeira Yref da lista
      yref1   = (VTYref*)lisYREF->First();
      lisYREF->Delete(0);
      //verifica se a Barra da Zref está definida
      if (yref1->pbarra == NULL) {delete yref1; continue;}

/*
// DESABILITADA EM 2014.04.27 - HPS
// Já está protegido em 'TReduzBarra::YrefDesprezivel()'
      //descarta Yref c/ valores muito pequenos
      if (IsDoubleZero(Abs(yref1->Y1_pu), 1e-10))
         {delete yref1; continue;}
*/

     //insere Zref na Reducao
      reducao->InsereEqbar(yref1);
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TReduzRede::LigacaoSeraMantida(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if (lisBAR_NOT->IndexOf(barra) < 0) return(false);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TReduzRede::MontaCrescimentoEquivalente(VTReducao *reducao, TList *lisCAR)
   {
   //variáveis locais
   double    energia_carga, energia_ano0, energia_anoN, taxa;
   VTCarga   *carga;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //se necessário, cria objeto Crescimento da Reducao
   if   (reducao->Crescimento) reducao->Crescimento->Reinicia();
   else                        reducao->Crescimento = DLL_NewObjCrescimento();
   //loop p/ todos os anos do período do mercado
   for (int ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++)
      {//inicia somatoria das demandas no ano 0 e no ano N
      energia_ano0 = 0.;
      energia_anoN = 0.;
      //loop p/ todas as Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //determina Energia diária da Carga no ano 0
         energia_carga = demanda->Edia_mwh[carga];
         //acumula energia diária da Carga no ano 0
         energia_ano0 += energia_carga;
         //acumula energia diária da Carga no ano N
         //energia_anoN += energia_carga * (1. + carga->crescimento->Taxa_pu[ano]);
//JCG 21/01/2009
//proteção desnecessária: toda Redução tem Crescimento
         taxa = 1.;
         if (carga->crescimento != NULL) taxa = 1 + carga->crescimento->Taxa_pu[ano];
//fim 21/01/2009
         energia_anoN += energia_carga * taxa;
         }
      //define taxa de crescimento equivalente no ano N
      taxa = 0.;
      if (energia_ano0 > 0.) taxa = (energia_anoN / energia_ano0) - 1.;
      reducao->Crescimento->InserePeriodo(ano, ano, taxa);
      }
   //IMPORTANTE: não é necessário iniciar taxa acumulada
   //reducao->Crescimento->MontaTaxaAcumulada(mercado->AnoInicial, mercado->AnoFinal);
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TReduzRede::MontaModeloCargaEquivalente(VTReducao *reducao, TList *lisCAR)
   {
   //variáveis locais
   struct   {
            double  Icte_pu, Scte_pu, Zcte_pu;
            } modelo;
   struct   {
            double  carga, total;
            double  Icte, Scte, Zcte;
            } energia;
   VTCarga    *carga;
   VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //verifica número de Cargas presentes na parte redutível
   if (lisCAR->Count == 0) return(true);
   if (lisCAR->Count == 1)
      {//Carga equivalente assume mesmo modelo da Carga da parte redutível
      carga = (VTCarga*)lisCAR->First();
      modelo.Icte_pu = carga->Icte_pu;
      modelo.Scte_pu = carga->Scte_pu;
      modelo.Zcte_pu = carga->Zcte_pu;
      }
   else
      {//calcula energia total das Cargas da parte redutível, e parcela de energia p/ cada modelo de carga
      energia.Icte = 0;
      energia.Scte = 0;
      energia.Zcte = 0;
      energia.total = 0;
      //loop p/ todas as Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //determina Energia diária da Carga no ano 0
         energia.carga = demanda->Edia_mwh[carga];
         //acumula energia total e energia por modelo de carga
         energia.total += energia.carga;
         energia.Icte  += (energia.carga * carga->Icte_pu);
         energia.Scte  += (energia.carga * carga->Scte_pu);
         energia.Zcte  += (energia.carga * carga->Zcte_pu);
         }
      //proteção
      if (IsDoubleZero(energia.total)) return(true);
      //calcula parcelas do modelo carga
      modelo.Icte_pu = energia.Icte / energia.total;
      modelo.Scte_pu = energia.Scte / energia.total;
      modelo.Zcte_pu = energia.Zcte / energia.total;
      }
   //proteção
   if (int(modelo.Icte_pu + modelo.Scte_pu + modelo.Zcte_pu) != 1)
      {
      modelo.Icte_pu = 1;
      modelo.Scte_pu = 0;
      modelo.Zcte_pu = 0;
      }
   //define modelo de carga p/ todas Cargas equivalentes da Reducao
   lisTMP->Clear();
   reducao->LisEqbar(lisTMP, eqptoCARGA);
   for (int nc = 0; nc < lisTMP->Count; nc++)
      {
      carga = (VTCarga*)lisTMP->Items[nc];
      carga->Icte_pu = modelo.Icte_pu;
      carga->Scte_pu = modelo.Scte_pu;
      carga->Zcte_pu = modelo.Zcte_pu;
      }
   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::MontaModeloCargaEquivalente(TList *lisCAR)
   {
   //variáveis locais
   struct   {
            double  carga, total;
            double  Icte, Scte, Zcte;
            } energia;
   VTCarga    *carga;
   VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //inicia modelo de carga como sendo Icte
   redutivel.mod_carga.Icte_pu = 1;
   redutivel.mod_carga.Scte_pu = 0;
   redutivel.mod_carga.Zcte_pu = 0;
   //verifica número de Cargas presentes na parte redutível
   if (lisCAR->Count == 0) return(true);
   if (lisCAR->Count == 1)
      {//Carga equivalente assume mesmo modelo da única Carga da parte redutível
      carga = (VTCarga*)lisCAR->First();
      redutivel.mod_carga.Icte_pu = carga->Icte_pu;
      redutivel.mod_carga.Scte_pu = carga->Scte_pu;
      redutivel.mod_carga.Zcte_pu = carga->Zcte_pu;
      }
   else
      {//calcula energia total das Cargas da parte redutível, e parcela de energia p/ cada modelo de carga
      energia.Icte  = 0;
      energia.Scte  = 0;
      energia.Zcte  = 0;
      energia.total = 0;
      //loop p/ todas as Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //determina Energia diária da Carga no ano 0
         energia.carga = demanda->Edia_mwh[carga];
         //acumula energia total e energia por modelo de carga
         energia.total += energia.carga;
         energia.Icte  += (energia.carga * carga->Icte_pu);
         energia.Scte  += (energia.carga * carga->Scte_pu);
         energia.Zcte  += (energia.carga * carga->Zcte_pu);
         }
      //proteção
      if (IsDoubleZero(energia.total)) return(true);
      //calcula parcelas do modelo carga
      redutivel.mod_carga.Icte_pu = energia.Icte / energia.total;
      redutivel.mod_carga.Scte_pu = energia.Scte / energia.total;
      redutivel.mod_carga.Zcte_pu = energia.Zcte / energia.total;
      }
   //proteção
   if (! IsDoubleZero(1. - (redutivel.mod_carga.Icte_pu + redutivel.mod_carga.Scte_pu + redutivel.mod_carga.Zcte_pu)))
      {
      redutivel.mod_carga.Icte_pu = 1;
      redutivel.mod_carga.Scte_pu = 0;
      redutivel.mod_carga.Zcte_pu = 0;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::MontaRedeEquivalente(bool aviso_enabled)
   {
   //variáveis locais
   bool   eqv_equilibrado;
   double Sbase = 100.;

   //define modelo de equivalente a ser utilizado
   switch(ModeloEqv)
      {
      case eqvEQUILIBRADO:     eqv_equilibrado = true;  break;
      case eqvDESEQUILIBRADO : eqv_equilibrado = false; break;
      case eqvAUTO:
      default:                 eqv_equilibrado = ! ExisteDesequilibrio(); break;
      }
   //configura modelo de quivalente: equilibrado ou desequilibrado
   monta_eqv->DefineModeloEqv(eqv_equilibrado);
   //define modelo de carga que as cargas equivalentes devem assumir
   monta_eqv->DefineModeloCarga(redutivel.mod_carga.Icte_pu,
                                redutivel.mod_carga.Scte_pu,
                                redutivel.mod_carga.Zcte_pu);
   //reduz Net eliminando os Nos em lisNO_EXC
   if (! monta_eqv->Executa(net, redutivel.lisLIG_ALL, redutivel.lisBAR_ALL,
                            redutivel.lisBAR_COR, redutivel.lisBAR_GER, ANO_0,
                            Sbase, aviso_enabled)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzRede::MontaReducao(VTCorte *corte)
   {
   //variáveis locais
   VTReducao *reducao;

   //cria objeto Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //monta a Reducao c/:
   //1) Suprimentos pré-existentes nas Barras de geração
   //2) Cargas equivalentes pré-existentes nas barras de greação
   //3) Cargas equivalentes criadas pelo processo de redução
   //4) Zref  equivalentes criadas pelo processo de redução
   //5) Ligacoes da parte redutível mantidas pelo processo de redução
   //6) Ligacoes criadas pelo processo de redução
   InsereSuprimento(reducao, corte);
   InsereCarga(reducao,     redutivel.lisBAR_GER);
   InsereCargaEqv(reducao,  monta_eqv->LisCargaEquivalente());
   InsereYrefEqv(reducao,   monta_eqv->LisYrefEquivalente());
   //InsereLigacaoMantidaEqv(reducao,  lisLIG_INC);
   InsereTrafoEqv(reducao,  monta_eqv->LisTrafoEquivalente());
   InsereTrechoEqv(reducao, monta_eqv->LisTrechoEquivalente());
   InsereChaveEqv(reducao);
   //define Reducao como válida
   reducao->Valida = true;
   return(reducao);
   }
   
//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzRede::MontaReducaoInvalida(void)
   {
   //variáveis locais
   VTReducao *reducao;

   //cria objeto Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //define Reducao como inválida
   reducao->Valida = false;
   return(reducao);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TReduzRede::RedefineBarraPaiNoDeCorte(VTMNet *mnet)
   {
   //variáveis locais
   VTBarra *barra;
   VTCorte *corte;
   TList   *lisBAR;

   //obtém cotenjunto de Cortes externos e internos da MNet
   IniciaLisCorte(mnet, lisCORTE);
   //loop p/ todos Cortes
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //loop p/ todas Barras do Corte
      lisBAR = corte->LisBarra();
      for (int nb = 0; nb < lisBAR->Count; nb++)
         {
         barra = (VTBarra*)lisBAR->Items[nb];
         //redefine Barra pai do No desta Barra
         if (barra->No[net]) barra->No[net]->pai = barra;
         }
      }
   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TReduzRede::RedefineBarraPaiNoDeCorte(VTCorte *corte)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR;

   //loop p/ todas Barras do Corte
   lisBAR = corte->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //redefine Barra do Corte como Barra pai do seu No
      if (barra->No[net]) barra->No[net]->pai = barra;
      }
   return(true);
   }

   //---------------------------------------------------------------------------
void __fastcall TReduzRede::SeparaParteRedutivel(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, TList *lisMR, TList *lisREDU)
   {
   //variáveis locais
   VTCorte   *corte_ref = corte;
   VTReducao *reducao;

   //reinicia lisREDU
   lisREDU->Clear();
   //reinicia lisMR c/ a MRede indicada
   lisMR->Clear();
   lisMR->Add(mrede);
   //inicia lisCORTE c/ os Cortes da MRede
   lisCORTE->Clear();
   mrede->LisCorte(lisCORTE);
   //analisa os Cortes em lisCORTE:
   //a) inclui em lisMR as MRedes da MNet conectadas ao Corte
   //b) inclui em lisREDU as Reducoes das MNets vizinhas
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //IMPORTANTE: descarta Corte de referência
      if (corte == corte_ref) continue;
      //obtém lista de MRedes do Corte
      lisTMP->Clear();
      corte->LisMRede(lisTMP);
      //analisa cada MRede do Corte
      for (int n = 0; n < lisTMP->Count; n++)
         {
         mrede = (VTMRede*)lisTMP->Items[n];
         //verifica se a MRede pertence à mesma MNet
         if (mrede->MNet == mnet)
            {//verifica se a MRede já existe em lisMR
            if (lisMR->IndexOf(mrede) >= 0) continue;
            //insere MRede em lisMR
            lisMR->Add(mrede);
            //insere em lisCORTE os Cortes da MRede
            mrede->LisCorte(lisCORTE);
            }
         else
            {//insere em lisREDU a Reducao da MRede ou da sua MNet
            if ((reducao = mrede->ExisteReducao(corte)) == NULL)
               {//obtém Reducao da MNet da MRede
               if ((reducao = mrede->MNet->ExisteReducao(corte)) == NULL) continue;
               //Reducao de uma MNet não tem Crescimento: é necessário criá-lo
               if (reducao->Crescimento == NULL)
                  {//determina conjunto de Cargas da MNet
                  //MontaCrescimentoEquivalente(reducao, mrede->MNet->LisMRede(), NULL);
                  IniciaLisCarga(mrede->MNet->LisMRede(), lisCAR);
                  MontaCrescimentoEquivalente(reducao, lisCAR);
                  }
               }
            if (lisREDU->IndexOf(reducao)) lisREDU->Add(reducao);
            }
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TReduzRede::SomaCargasDoNoNaCargaEquivalente(VTCarga *carga_eqv, VTNo *no)
   {
   //variáveis locais
   int             max_pat;
   complex<double> s_mva;
   strDEM          str_dem[MAX_FASE];
   VTCarga        *carga;
   VTDemFase      *dem_fase;
   VTListaDemFase *listaDF;
   VTDemanda      *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTFases        *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //verifica se o No possui Cargas
   if (no->lisCAR->Count == 0) return;
   //soma na Carga as demandas das Cargas existentes no No
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//determina demanda por fase da Carga equivalente
      listaDF = demanda->Sfase[carga_eqv][np];
      for (int nf = 0; nf < MAX_FASE; nf ++)
         {
         dem_fase      = listaDF->Items[nf];
         str_dem[nf].p = dem_fase->P_mw;
         str_dem[nf].q = dem_fase->Q_mvar;
         }
      //loop p/ todas Cargas do No
      for (int nc = 0; nc < no->lisCAR->Count; nc++)
         {
         carga = (VTCarga*)no->lisCAR->Items[nc];
         //verifica se é uma Carga de redução de rede
         if (carga->Reducao)
            {//soma as demandas de cada patamar
            listaDF = demanda->Sfase[carga][np];
            for (int nf = 0; nf < MAX_FASE; nf++)
               {
               dem_fase       = listaDF->Items[nf];
               str_dem[nf].p += dem_fase->P_mw;
               str_dem[nf].q += dem_fase->Q_mvar;
               }
            }
         else
            {//Carga normal: divide sua demanda entre as 3 fases
            s_mva = demanda->S_mva[carga][np] / 3.;
            for (int nf = 0; nf < 3; nf++)
               {//soma as demandas de cada patamar
               str_dem[nf].p += s_mva.real();
               str_dem[nf].q += s_mva.imag();
               }
            }
         }
      //atualiza demanda da Carga equivalente no patamar
      listaDF->Clear();
      for (int nf = 0; nf < MAX_FASE; nf ++)
         {
         listaDF->Add(fases->IndexToTag[nf], cargaAUTO, str_dem[nf].p, str_dem[nf].q);
         }
      demanda->Sfase[carga_eqv][np] = listaDF;
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TReduzRede::TrataBarrasCorteConectadasPorChave(void)
   {
   //variáveis locais
   VTBarra *barra;
   VTNo    *no = NULL;

   //verifica se as Barras de Corte pertencem a um único Nó
   for (int nb = 0; nb < redutivel.lisBAR_COR->Count; nb++)
      {
      barra = (VTBarra*)redutivel.lisBAR_COR->Items[nb];
      if (no == NULL) no = barra->No[net];
      if ( barra->No[net] != no) return;
      }
   //barras do Corte interligadas por Chave:
   }

//---------------------------------------------------------------------------
//eof

