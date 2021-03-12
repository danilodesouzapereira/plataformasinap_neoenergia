//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TReduzCarga.h"
#include "VTRedutor.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Zona\VTZona.h"
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
VTReduz* __fastcall NewObjReduzCarga(VTApl *apl)
   {
   return(new TReduzCarga(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReduzCarga::TReduzCarga(VTApl *apl)
   {
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria objetos
   lisBAR   = new TList();
   lisCAR   = new TList();
   lisLIG   = new TList();
   lisMRV   = new TList();
   lisTMP   = new TList();
   //inicia dados
   ModeloEqv = eqvDESEQUILIBRADO;
   }

//---------------------------------------------------------------------------
__fastcall TReduzCarga::~TReduzCarga(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR)   {delete lisBAR;   lisBAR   = NULL;}
   if (lisCAR)   {delete lisCAR;   lisCAR   = NULL;}
   if (lisLIG)   {delete lisLIG;   lisLIG   = NULL;}
   if (lisMRV)   {delete lisMRV;   lisMRV   = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::CorteIsolado(VTCorte *corte)
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
bool __fastcall TReduzCarga::CriaCurva(VTCarga *pcarga)
   {
   //variáveis locais
   VTCurva   *curva;

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
void __fastcall TReduzCarga::DefineNumeroConsumidoresDaCargaEquivalente(VTCarga *carga_eqv, TList *lisCAR)
   {
   //variáveis locais
   VTCarga *carga;

   //acumula número de consumidores de cada classe
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      carga_eqv->NumConsResidencial = carga_eqv->NumConsResidencial + carga->NumConsResidencial;
      carga_eqv->NumConsComercial   = carga_eqv->NumConsComercial   + carga->NumConsComercial;
      carga_eqv->NumConsIndustrial  = carga_eqv->NumConsIndustrial  + carga->NumConsIndustrial;
      carga_eqv->NumConsRural       = carga_eqv->NumConsRural       + carga->NumConsRural;
      carga_eqv->NumConsOutros      = carga_eqv->NumConsOutros      + carga->NumConsOutros;
      carga_eqv->NumConsA4          = carga_eqv->NumConsA4          + carga->NumConsA4;
      }
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzCarga::Executa(VTMNet *mnet, VTMRede *mrede,
                                           VTCorte *corte, bool viso_enabled)
   {
   //variáveis locais
   //bool      tramo_enabled = false;
   VTReducao *reducao;
   VTOrdena  *ordena;

   //proteção
   if (corte->NumeroBarras() != 1) return(NULL);
   //proteção contra Corte entre redes que estão isoladas (sem geração)
   if (CorteIsolado(corte))
      {//MRede não pode ser reduzida p/ o Corte
      reducao = MontaReducaoInvalida();
      return(reducao);
      }
   //cria Net da MRede
   ordena                  = DLL_NewObjOrdena(apl);
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = false;
   ordena->TramoEnabled    = false;
   ordena->ReguladorBypass = true;
   //net     = ordena->Executa(mnet, tramo_enabled);
   net     = ordena->Executa(mnet);
   //cria Reducao de carga p/ a MRede no Corte
   reducao = MontaReducaoTipoCarga(mrede, corte);
   //destrói objeto net
   delete net;
   delete ordena;
   return(reducao);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::Executa(VTRede *rede, TList *lisBAR,bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::Executa(VTRedes *redes, TList *lisBAR,bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzCarga::ExisteBarraInterface(VTMRede *mrede, VTCorte *corte, TList *lisEQP)
   {
   //variáveis locais
   VTBarra *pbarra;
   TList   *lisBC;   //lista de Barras do Corte

   //reinicia lista recebida c/ as Barras da MRede
   lisEQP->Clear();
   //obtém lista de Barras do Corte
   lisBC = corte->LisBarra();
   //obtém lista de Barras da MRede
   lisTMP->Clear();
   mrede->LisBarra(lisTMP);
   //loop p/ todas Barras do Corte
   for (int nb = 0; nb < lisBC->Count; nb++)
      {
      pbarra = (VTBarra*)lisBC->Items[nb];
      //verifica se a Barra do Corte existe na lista de Barras da MRede
      if (lisTMP->IndexOf(pbarra) >= 0)
         {//insere Barra na lista
         if (lisEQP->IndexOf(pbarra) < 0) lisEQP->Add(pbarra);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzCarga::ExisteLigacao(VTMRede *mrede, VTBarra *pbarra, TList *lisEQP)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //obtém lista de Ligacoes da MRede
   lisTMP->Clear();
   mrede->LisLigacao(lisTMP);
   //mantém em lisEQP somente a Ligacoes que não são Chave e que conectam o No da Barra indicada
   lisEQP->Clear();
   for (int nl = 0; nl < lisTMP->Count; nl++)
      {
      ligacao = (VTLigacao*)lisTMP->Items[nl];
      //decarta Chaves
      if (ligacao->Tipo() == eqptoCHAVE) continue;
      //if (pliga->ConectaBarra(pbarra))
      if (LigacaoConectaNo(ligacao, pbarra->No[net]) >= 0)
         {//insere Ligacao na lista
         if (lisEQP->IndexOf(ligacao) < 0) lisEQP->Add(ligacao);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzCarga::IniciaLisCargaParteRedutivel(VTMRede *mrede, VTCorte *corte, TList *lisCAR)
   {
   //variáveis locais
   VTCorte   *corte_ref = corte;
   VTMRede   *mrede_viz;
   VTReducao *redu_viz;
   struct   {
            TList *lisCORTE;
            TList *lisMR;
            TList *lisREDU;
            }sel, viz;

   //cria listas
   sel.lisCORTE = new TList();
   sel.lisMR    = new TList();
   sel.lisREDU  = new TList();
   viz.lisMR    = new TList();
   //inicia lista sel.lisCORTE c/ todos Cortes da MRede
   mrede->LisCorte(sel.lisCORTE);
   //inicia lista sel.lisMR c/ a MRede indicada
   sel.lisMR->Add(mrede);
   //loop p/ todos os Cortes em lisCORTE
   for (int nc = 0; nc < sel.lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)sel.lisCORTE->Items[nc];
      //IMPORTANTE: descarta o Corte de referência
      if (corte == corte_ref) continue;
      //inicia viz.lisMR c/ as MRede do Corte
      viz.lisMR->Clear();
      corte->LisMRede(viz.lisMR);
      //loop p/ todas MRede do Corte
      for (int n = 0; n < viz.lisMR->Count; n++)
         {
         mrede_viz = (VTMRede*)viz.lisMR->Items[n];
         //verifica se a MRede vizinha já existe em sel.lisMR
         if (sel.lisMR->IndexOf(mrede_viz) >= 0) continue;
         //verifica se a MRede vizinha possui Reducao p/ o Corte
         if ((redu_viz = mrede_viz->ExisteReducao(corte)) != NULL)
            {//insere Reducao em sel.lisREDU
            if (sel.lisREDU->IndexOf(redu_viz) < 0) sel.lisREDU->Add(redu_viz);
            }
         else
            {//insere MRede vizinha em sel.lisMR
            sel.lisMR->Add(mrede_viz);
            //insere em sel.lisCORTE os Cortes da MRede vizinha
            mrede_viz->LisCorte(sel.lisCORTE);
            }
         }
      }
   //insere em lisCARGA as Cargas das Reducoes e Redes selecionadas
   lisCAR->Clear();
   for (int nr = 0; nr < sel.lisREDU->Count; nr++)
      {
      redu_viz = (VTReducao*)sel.lisREDU->Items[nr];
      redu_viz->LisEqpto(lisCAR, eqptoCARGA);
      }
   for (int nr = 0; nr < sel.lisMR->Count; nr++)
      {
      mrede_viz = (VTMRede*)sel.lisMR->Items[nr];
      mrede_viz->LisEqpto(lisCAR, eqptoCARGA);
      }
   //destrói listas criadas
   delete sel.lisCORTE;
   delete sel.lisMR;
   delete sel.lisREDU;
   delete viz.lisMR;
   }
   
//---------------------------------------------------------------------------
int __fastcall TReduzCarga::LigacaoConectaNo(VTLigacao* ligacao, VTNo *no)
   {
   //variáveis locais
   VTBarra *barra;

   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if (barra->No[net] == no) return(ind_bar);
      }
   return(-1);
   }
/*
//---------------------------------------------------------------------------
VTEqbar* __fastcall TReduzCarga::MontaCargaEquivalente(VTMRede *mrede, VTBarra *pbarra)
   {
   //variáveis locais
   double          vft_pu;
   int             max_pat, ind_bar;
   complex<double> s_mva;
   VTCarga         *pcarga;
   VTLigacao       *pliga;
   VTFases         *fases   = (VTFases*)apl->GetObject(__classid(VTFases));
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTListaDemFase  *listaDF = demanda->ListaDemFase();

   //determina lista de Ligacoes da Rede que conectam No da Barra
   ExisteLigacao(mrede, pbarra, lisLIG);
   if (lisLIG->Count == 0) return(NULL);
   //cria uma nova Carga
   if ((pcarga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQ p/ a Carga
   if (! CriaCurva(pcarga)) {delete pcarga; return(NULL);}
   //associa Carga c/ sua Barra
   pcarga->DefineObjBarra(pbarra);
   //inicia dados da carga
   pcarga->Id         = FalsoId();
   pcarga->Codigo     = "";
   pcarga->Reducao    = true;
   pcarga->Icte_pu    = 1.;
   // pcarga->Scte_pu    = 1.;
   // pcarga->Icte_pu    = mod_carga.Icte;
   // pcarga->Scte_pu    = mod_carga.Scte;
   // pcarga->Zcte_pu    = mod_carga.Zcte;
   //JCG 2011.05.12
   //define fases da Carga igual às fases da Zona da Barra
   //pcarga->Fases = pbarra->zona->Fases;
   pcarga->Fases = pbarra->No[net]->Fases;
   pcarga->esq.posbar = 50;
   (pbarra->Vertical()) ? pcarga->esq.dxbar = 4000 : pcarga->esq.dybar = 4000;
   //atualiza Curva da Carga p/ todos os patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//reinicia demandas por fase p/ o patamar
      listaDF->Clear();
      //acumula fluxo das Ligacoes na Carga
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         pliga = (VTLigacao*)lisLIG->Items[nl];
         //verifica se a Ligacao conecta o No da Barra indicada
         //if ((ind_bar = pliga->IndiceBarra(pbarra)) < 0) continue;
         if ((ind_bar = LigacaoConectaNo(pliga, pbarra->No[net])) < 0) continue;
         //loop p/ todas as fases da Ligacao
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            s_mva  = pliga->resflow->Sf_mva[ind_bar][np][nf];
            vft_pu = Abs(pbarra->resflow->Vft_pu[np][nf]);
            if (IsDoubleZero(vft_pu)) vft_pu = 1.0;
            listaDF->Add(fases->IndexToTag[nf], carINDEF, s_mva/vft_pu);
            }
         }
      //atualiza demanda da Carga no patamar
      demanda->Sfase[pcarga][np] = listaDF;
      }
   return(pcarga);
   }
*/
//---------------------------------------------------------------------------
VTCarga* __fastcall TReduzCarga::MontaCargaEquivalente(VTMRede *mrede, VTBarra *pbarra)
   {
   //variáveis locais
   double          vft_pu;
   int             max_pat, ind_bar;
   int             num_fase, fase_tag;
   complex<double> s_mva, s_mva_tmp, vetor_s_mva[MAX_FASE];
   VTCarga         *pcarga;
   VTLigacao       *pliga;
   VTFases         *fases   = (VTFases*)apl->GetObject(__classid(VTFases));
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //determina lista de Ligacoes da Rede que conectam No da Barra
   ExisteLigacao(mrede, pbarra, lisLIG);
   if (lisLIG->Count == 0) return(NULL);
   //cria uma nova Carga
   if ((pcarga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQ p/ a Carga
   if (! CriaCurva(pcarga)) {delete pcarga; return(NULL);}
   //associa Carga c/ sua Barra
   pcarga->DefineObjBarra(pbarra);
   //inicia dados da carga
   pcarga->Id         = FalsoId();
   pcarga->Codigo     = "";
   pcarga->Reducao    = true;
   pcarga->Icte_pu    = 1.;
   // pcarga->Scte_pu    = 1.;
   // pcarga->Icte_pu    = mod_carga.Icte;
   // pcarga->Scte_pu    = mod_carga.Scte;
   // pcarga->Zcte_pu    = mod_carga.Zcte;
   //JCG 2011.05.12
   //define fases da Carga igual às fases da Zona da Barra
   //pcarga->Fases = pbarra->zona->Fases;
   pcarga->Fases = pbarra->No[net]->Fases;
   pcarga->esq.posbar = 50;
   (pbarra->Vertical()) ? pcarga->esq.dxbar = 4000 : pcarga->esq.dybar = 4000;
   //atualiza Curva da Carga p/ todos os patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//reinicia vetor com as demandas das fases
      for (int nf = 0; nf < MAX_FASE; nf++) {vetor_s_mva[nf] = CZero();}
      //acumula fluxo das Ligacoes
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         pliga = (VTLigacao*)lisLIG->Items[nl];
         //verifica se a Ligacao conecta o No da Barra indicada
         //if ((ind_bar = pliga->IndiceBarra(pbarra)) < 0) continue;
         if ((ind_bar = LigacaoConectaNo(pliga, pbarra->No[net])) < 0) continue;
         //loop p/ todas as fases da Ligacao
         s_mva_tmp = CZero();
         num_fase  = 0;
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            s_mva  = pliga->resflow->Sf_mva[ind_bar][np][nf];
            vft_pu = Abs(pbarra->resflow->Vft_pu[np][nf]);
            if (IsDoubleZero(vft_pu)) vft_pu = 1.0;
            //verifica se Carga equivalente possui a fase
            fase_tag = fases->IndexToTag[nf];
            if ((pcarga->Fases & fase_tag) == fase_tag)
               {//insere demanda p/ a fase
               vetor_s_mva[nf] += s_mva / vft_pu;
               }
            else
               {//insere demanda nula p/ a fase
               //vetor_s_mva[nf] = CZero();
               //acumula demanda de fase inexistente
               s_mva_tmp += s_mva;
               //incrementa número de fases inexistentes
               num_fase++;
               }
            }
         //redistribui demanda das fases inexistentes
         if (num_fase != 0)
            {//redefine num_fase como o número de fases existentes
            num_fase   = MAX_FASE - num_fase;
            s_mva_tmp /= double(num_fase);
            for (int nf = 0; nf < MAX_FASE; nf++)
               {
               fase_tag = fases->IndexToTag[nf];
               if ((pcarga->Fases & fase_tag) == fase_tag)
                  {//adiciona demanda p/ a fase
                  vetor_s_mva[nf] += s_mva_tmp;
                  }
               }
            }
         //atualiza demanda da Carga no patamar
         demanda->Set_S3f_mva(pcarga, np, vetor_s_mva);
         }
      }
   return(pcarga);
   }


//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::MontaCrescimentoEquivalente(VTReducao *reducao, TList *lisCAR)
   {
   //variáveis locais
   double     energia_carga, energia_ano0, energia_anoN, taxa;
   VTCarga    *carga;
   VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTMercado  *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //se necessário, cria objeto Crescimento da Reducao
   if (reducao->Crescimento) reducao->Crescimento->Reinicia();
   else                      reducao->Crescimento = DLL_NewObjCrescimento();
   //monta lista de Cargas da parte redutível da rede
   //IniciaLisCargaParteRedutivel(mrede, corte, lisCAR);
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
         taxa = 1.;
//JCG 21/01/2009
//proteção que deveria ser desnecessária: verificar qual Redução está sem crescimento
         if (carga->crescimento != NULL) taxa = 1 + carga->crescimento->Taxa_pu[ano];
//fim 21/01/2009
         energia_anoN += energia_carga * taxa;
         }
      //define taxa de crescimento equivalente no ano N
      taxa = 0.;
      if (energia_ano0 > 0.) taxa = (energia_anoN / energia_ano0) - 1.;
      reducao->Crescimento->InserePeriodo(ano, ano, taxa);
      }
   //IMPORTANTE: inicia taxas acumuladas do Crescimento da Reducao
   //IMPORTANTE 2: não é mais necessário iniciar taxa acumulada
   //reducao->Crescimento->MontaTaxaAcumulada(mercado->AnoInicial, mercado->AnoFinal);
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::MontaModeloCargaEquivalente(VTReducao *reducao, TList *lisCAR)
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


   //inicia modelo de carga como sendo Icte
   modelo.Icte_pu = 1;
   modelo.Scte_pu = 0;
   modelo.Zcte_pu = 0;
   //verifica número de Cargas presentes na parte redutível
   //if (lisCAR->Count == 0) return(true);
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
      if (! IsDoubleZero(energia.total))
         {//calcula parcelas do modelo carga
         modelo.Icte_pu = energia.Icte / energia.total;
         modelo.Scte_pu = energia.Scte / energia.total;
         modelo.Zcte_pu = energia.Zcte / energia.total;
         }
      }
   //proteção
   if (! IsDoubleZero(1. - (modelo.Icte_pu + modelo.Scte_pu + modelo.Zcte_pu)))
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
/*
//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::MontaModeloCargaEquivalente(VTReducao *reducao, TList *lisCAR)
   {
   //variáveis locais
   VTCarga *carga;

   //define modelo de carga p/ todas Cargas equivalentes da Reducao
   lisTMP->Clear();
   reducao->LisEqbar(lisTMP, eqptoCARGA);
   for (int nc = 0; nc < lisTMP->Count; nc++)
      {
      carga = (VTCarga*)lisTMP->Items[nc];
      carga->Icte_pu = 1.0;
      carga->Scte_pu = 0.0;
      carga->Zcte_pu = 0.0;
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TReduzCarga::MontaModeloCargaEquivalente(VTCarga *carga_eqv, TList *lisCAR)
   {
   //define modelo de carga p/ Carga equivalente
   carga_eqv->Icte_pu = 1.0;
   carga_eqv->Scte_pu = 0.0;
   carga_eqv->Zcte_pu = 0.0;

   return(true);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzCarga::MontaReducaoInvalida(void)
   {
   //variáveis locais
   VTReducao *reducao;

   //cria objeto Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //define Reducao como inválida
   reducao->Valida = false;
   return(reducao);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzCarga::MontaReducaoTipoCarga(VTMRede *mrede, VTCorte *corte)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTCarga   *carga_eqv;
   //VTRede    *rede;
   VTReducao *reducao;

   //define MRede como NÃO redutivel
   mrede->Redutivel = false;
   //cria objeto Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //associa MRede e Corte da Reducao
   //mrede = reducao->Rede();
   //corte = reducao->Corte();
   //loop p/ todas Barras da MRede que pertencem ao Corte
   ExisteBarraInterface(mrede, corte, lisBAR);
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      //cria Carga equivalente p/ a Rede na Barra
      if ((carga_eqv = MontaCargaEquivalente(mrede, pbarra)) == NULL)
         {//destrói objeto
         DLL_DeleteEqpto(reducao);
         return(NULL);
         }
      //insere Carga na Reducao
      reducao->InsereEqbar(carga_eqv);
      }
   //monta lista de Cargas da parte redutível da rede
   IniciaLisCargaParteRedutivel(mrede, corte, lisCAR);
   //define número de Cargas representadas pela Carga equivalente
   DefineNumeroConsumidoresDaCargaEquivalente(carga_eqv, lisCAR);
   //modelo de carga: define modelo de carga p/ a Carga equivalente
   MontaModeloCargaEquivalente(carga_eqv, lisCAR);
   //mercado: monta curva de Crescimento a Carga equivalente
   MontaCrescimentoEquivalente(reducao, lisCAR);
   //define Reducao como válida
   reducao->Valida = true;
   //define MRede como redutivel
   mrede->Redutivel = true;
   return(reducao);
   }

//---------------------------------------------------------------------------
//eof

