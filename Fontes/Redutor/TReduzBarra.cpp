//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TReduzBarra.h"
#include "VTMontaEqv.h"
#include "VTRedutor.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Topologia\VTTopologia.h"
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
VTReduz* __fastcall NewObjReduzBarra(VTApl *apl)
   {
   return(new TReduzBarra(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReduzBarra::TReduzBarra(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria objetos
   monta_eqv  = NewObjMontaEqv(apl);
   ordena     = DLL_NewObjOrdena(apl);
   lisTMP     = new TList();
   //lista p/ os Eqptos da parte redutível da Rede
   redutivel.lisBAR_ALL = new TList();
   redutivel.lisBAR_COR = new TList();
   redutivel.lisBAR_EXC = new TList();
   redutivel.lisBAR_GER = new TList();
   redutivel.lisLIG_ALL = new TList();
   redutivel.lisSUP_GER = new TList();
   redutivel.lisMUTUA   = new TList();
   redutivel.lisCAR     = new TList();
   redutivel.lisREDE    = new TList();
   //inicia dados
   ModeloEqv = eqvEQUILIBRADO;
   }

//---------------------------------------------------------------------------
__fastcall TReduzBarra::~TReduzBarra(void)
   {
   //destrói objetos criados
   if (monta_eqv) {delete monta_eqv;  monta_eqv = NULL;}
   if (ordena)    {delete ordena;     ordena    = NULL;}
   //destrói lista sem destrui seus objetos
   if (lisTMP)    {delete lisTMP;     lisTMP     = NULL;}
   if (redutivel.lisBAR_ALL) {delete redutivel.lisBAR_ALL; redutivel.lisBAR_ALL = NULL;}
   if (redutivel.lisBAR_COR) {delete redutivel.lisBAR_COR; redutivel.lisBAR_COR = NULL;}
   if (redutivel.lisBAR_EXC) {delete redutivel.lisBAR_EXC; redutivel.lisBAR_EXC = NULL;}
   if (redutivel.lisBAR_GER) {delete redutivel.lisBAR_GER; redutivel.lisBAR_GER = NULL;}
   if (redutivel.lisLIG_ALL) {delete redutivel.lisLIG_ALL; redutivel.lisLIG_ALL = NULL;}
   if (redutivel.lisSUP_GER) {delete redutivel.lisSUP_GER; redutivel.lisSUP_GER = NULL;}
   if (redutivel.lisMUTUA)   {delete redutivel.lisMUTUA;   redutivel.lisMUTUA   = NULL;}
   if (redutivel.lisCAR)     {delete redutivel.lisCAR;     redutivel.lisCAR     = NULL;}
   if (redutivel.lisREDE)    {delete redutivel.lisREDE;    redutivel.lisREDE    = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::CargaDesprezivel(VTCarga* pcarga)
   {

   //variáveis locais
   int        max_pat = patamares->NumPatamar();
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //analisa demanda de todos os patamares
   for (int np = 0; np < max_pat; np++)
      {
      //if (Abs(demanda->S_mva[pcarga][np]) > 1.e-10) return(false);
      if (Abs(demanda->S_mva[pcarga][np]) > 1.e-6) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::CriaCurva(VTCarga *pcarga)
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
bool __fastcall TReduzBarra::CriaCurva(VTSuprimento *psup)
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
VTReducao* __fastcall TReduzBarra::Executa(VTMNet *mnet, VTMRede *mrede,
                                           VTCorte *corte, bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled)
   {
   //variáveis locais
   bool sucesso;

   //no cálculo da redução, é necessário:
   //-a) incluir em lisBAR a Barra inicial e as Barras dos Cortes da Rede com as demais Redes
   // a) montar a Net da rede indicada p/ identificar a rede conexa
   // b) identificar conjuntos de Eqptos da parte redutível:
   //    1) conjunto de todas Barras e Ligacoes
   //    2) conjunto de Barras do Corte
   //    3) aconjunto de Barras que será mantida na Reducao: Barras do Corte
   //c) montar a Net com a parte da rede que será reduzida
   //d) criar a rede equivalente

   //-a) inclui Barra inicial e Barras de fronteira na lista de Barras a serem mantidas
   InsereBarraInicial(rede, lisBAR);
   InsereBarraDeFronteira(rede, lisBAR);
   //a) cria VTNet p/ todas as redes
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = true;
   ordena->TramoEnabled    = false;
   ordena->ReguladorBypass = true;
   if ((net = ordena->Executa(rede)) == NULL) return(false);
   //b) identificar conjuntos de Eqptos da parte redutível
   IniciaListasParteRedutivel(lisBAR);
   //proteção: verifica a viabilidade da Reducao
   if (redutivel.lisBAR_ALL->Count <= redutivel.lisBAR_COR->Count)
      {//não é possível fazer nenhuma redução
      return(false);
      }
   //destrói Net
   delete net;
   //c) montar a Net com a parte da rede que será reduzida
   //if ((net = ordena->Executa(redutivel.lisBAR_ALL, redutivel.lisCAR, redutivel.lisLIG_ALL,
   //                           redutivel.lisMUTUA, tramo_enabled)) == NULL) return(false);
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = false;
   ordena->TramoEnabled    = false;
   ordena->ReguladorBypass = true;
   if ((net = ordena->Executa(redutivel.lisBAR_ALL, redutivel.lisCAR, redutivel.lisLIG_ALL,
                              redutivel.lisMUTUA)) == NULL) return(false);
   //redefine Barras mantidas como Barras pai dos Nos
   RedefineBarraPaiNo(lisBAR);
   //determina modelo de carga a ser definido p/ as Cargas equivalentes
   MontaModeloCargaEquivalente(redutivel.lisCAR);
   //d) calcula a rede equivalente
   sucesso = MontaRedeEquivalente(aviso_enabled);
   //destrói net ANTES de reiniciar Redes
   delete net;
   //reinicia Rede para substituir Ligacoes originais pelos equivalentes
   ReiniciaRede(rede);
   //mercado: monta uma única curva de Crescimento p/ todas as Cargas equivalentes
   MontaCrescimentoEquivalente();

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled)
   {
   //variáveis locais
   bool sucesso;

   //no cálculo da redução, é necessário:
   //a) montar a Net da rede completa p/ identificar a rede conexa
   //b) identificar conjuntos de Eqptos da parte redutível:
   //    1) conjunto de todas Barras e Ligacoes
   //    2) conjunto de Barras do Corte
   //    3) aconjunto de Barras que será mantida na Reducao: Barras do Corte
   //c) montar a Net com a parte da rede que será reduzida
   //d) criar a rede equivalente

   //a) cria VTNet p/ todas as redes
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = true;
   ordena->TramoEnabled    = false;
   ordena->ReguladorBypass = true;
   if ((net = ordena->Executa(redes)) == NULL) return(false);
   //b) identificar conjuntos de Eqptos da parte redutível
   IniciaListasParteRedutivel(lisBAR);
   //proteção: verifica a viabilidade da Reducao da MRede p/ o Corte
   if (redutivel.lisBAR_ALL->Count <= redutivel.lisBAR_COR->Count)
      {//não é possível fazer nenhuma redução
      return(false);
      }
   //destrói Net
   delete net;
   //c) montar a Net com a parte da rede que será reduzida
   ordena->ChaveNA_Enabled = false;
   ordena->ChaveNF_Enabled = false;
   ordena->TramoEnabled    = false;
   ordena->ReguladorBypass = true;
   //if ((net = ordena->Executa(redutivel.lisBAR_ALL, redutivel.lisCAR, redutivel.lisLIG_ALL,
   //                           redutivel.lisMUTUA, tramo_enabled)) == NULL) return(false);
   if ((net = ordena->Executa(redutivel.lisBAR_ALL, redutivel.lisCAR, redutivel.lisLIG_ALL,
                              redutivel.lisMUTUA)) == NULL) return(false);
   //redefine Barras mantidas como Barras pai dos Nos
   RedefineBarraPaiNo(lisBAR);
   //determina modelo de carga a ser definido p/ as Cargas equivalentes
   MontaModeloCargaEquivalente(redutivel.lisCAR);
   //d) calcula a rede equivalente
   sucesso = MontaRedeEquivalente(aviso_enabled);
   //destrói net ANTES de reiniciar Redes
   delete net;
   ////reinicia Redes mantendo apenas 1 rede equivalente
   ReiniciaRede(redes);
   //mercado: monta uma única curva de Crescimento p/ todas as Cargas equivalentes
   MontaCrescimentoEquivalente();

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::ExisteDesequilibrio(void)
   {
   //variáveis locais
   VTRede    *rede;

   //verifica se alguma Rede afetada pela redução possui desequilíbrio
   for (int n = 0; n < redutivel.lisREDE->Count; n++)
      {
      rede = (VTRede*)redutivel.lisREDE->Items[n];
      //verifica se a Rede possui desequilíbrio
      if (ExisteDesequilibrio(rede)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::ExisteDesequilibrio(VTRede *rede)
   {
   //variáveis locais
   VTCarga  *carga;
   VTTrafo  *trafo;
   VTTrecho *trecho;

	//verifica se há alguma Carga desequilibrada
   lisTMP->Clear();
   rede->LisEqbar(lisTMP, eqptoCARGA);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      carga = (VTCarga*)lisTMP->Items[n];
      if (! carga->Equilibrada) {return(true);}
      }
   //verifica se há algum Trafo desequilibrado
   lisTMP->Clear();
   rede->LisLigacao(lisTMP, eqptoTRAFO);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      trafo = (VTTrafo*)lisTMP->Items[n];
      if (trafo->Monofasico) {return(true);}
      }
   //verifica se há algum Trecho desequilibrado
   lisTMP->Clear();
   rede->LisLigacao(lisTMP, eqptoTRECHO);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      trecho = (VTTrecho*)lisTMP->Items[n];
      if (! trecho->Equilibrado) {return(true);}
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::IniciaListasParteRedutivel(TList *lisBAR_MAN)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisLIG;

   //reinicia listas da parte redutivel
   redutivel.lisBAR_ALL->Clear();
   redutivel.lisBAR_COR->Clear();
   redutivel.lisBAR_EXC->Clear();
   redutivel.lisBAR_GER->Clear();
   redutivel.lisLIG_ALL->Clear();
   redutivel.lisSUP_GER->Clear();
   redutivel.lisMUTUA->Clear();
   redutivel.lisCAR->Clear();
   redutivel.lisREDE->Clear();
   //inicia lisBAR_EXC c/ todas Barras que serão excluídas: todas Barras da Net,
   //exceto as Barras indicadas em lisBAR_MAN
   CopiaTList(net->LisBarra(), redutivel.lisBAR_EXC);
   for (int n = 0; n < lisBAR_MAN->Count; n++)
      {
      barra = (VTBarra*)lisBAR_MAN->Items[n];
      redutivel.lisBAR_EXC->Remove(barra);
      }
   //inicia lisLIG c/ todas as Ligacoes que conectam pelo menos uma das Barras que
   //serão excluídas
   lisLIG = net->LisLigacao();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (LigacaoSeraExcluida(ligacao))
         {//insere Ligacao em lisLIG_ALL
         redutivel.lisLIG_ALL->Add(ligacao);
         //insere Rede da Ligacao em redutivel.lisREDE
         if (redutivel.lisREDE->IndexOf(ligacao->rede) < 0) {redutivel.lisREDE->Add(ligacao->rede);}
         //verifica se a Ligacao é um Trecho com Mutua
         if (ligacao->Tipo() == eqptoTRECHO)
            {//insere em redutivel.lisMUTUA as Mutuas do Trecho
            ((VTTrecho*)ligacao)->LisMutua(redutivel.lisMUTUA);
            }
         }
      }
   //inicia lisBAR_ALL c/ todas as Barras conectadas pela Ligacoes que serão excluídas
   for (int n = 0; n < redutivel.lisLIG_ALL->Count; n++)
      {
      ligacao = (VTLigacao*)redutivel.lisLIG_ALL->Items[n];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         //verifica se a Barra já existe em lisBAR_ALL
         if (redutivel.lisBAR_ALL->IndexOf(barra) < 0)
            {//insere Barra no conjunto de Barras da parte redutível
            redutivel.lisBAR_ALL->Add(barra);
            }
         }
      }
   //inicia lisBAR_COR c/ as Barras da parte redutível que não serão excluídas
   for (int n = 0; n < redutivel.lisBAR_ALL->Count; n++)
      {
      barra = (VTBarra*)redutivel.lisBAR_ALL->Items[n];
      //verifica se a Barra será eliminada
      if (redutivel.lisBAR_EXC->IndexOf(barra) < 0)
         {//barra não será excluída: insere Barra em lisBAR_COR
         redutivel.lisBAR_COR->Add(barra);
         }
      else
         {//Barra será excluída: insere Cargas em lisCAR
         barra->LisEqbar(redutivel.lisCAR, eqptoCARGA);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereBarraDeFronteira(VTRede *rede, TList *lisBAR)
   {
   //variáveis locais
   VTBarra *barra;
   VTRede  *rede_viz;
   TList   *lisEQP  = rede->LisBarra();
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //loop para todas as Barras da Rede
   for (int nb = 0; nb < lisEQP->Count; nb++)
      {
      barra = (VTBarra*)lisEQP->Items[nb];
      //verifica se a Barra existe em alguma outra Rede
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede_viz = (VTRede*)lisREDE->Items[nr];
         //verifica se a Rede vizinha é a própria rede
         if (rede_viz == rede) continue;
         //verifica se a barra existe na rede vizinha
         if (rede_viz->ExisteBarra(barra))
            {//inclui Barra em lisBAR
            if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereBarraInicial(VTRede *rede, TList *lisBAR)
   {
   //variáveis locais
   VTBarra *barra;

   //inclui a Barra inicial da Rede em lisBAR
   if ((barra = rede->BarraInicial()) != NULL)
      {
      if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereCargaEqv(TList *lisCAR)
   {
   //variáveis locais
   VTCarga   *pcarga, *pc;
   VTNo      *pno;
   VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));

   //insere Cargas na Reducao
   while (lisCAR->Count > 0)
      {//retira primeira Carga da lista
      pcarga   = (VTCarga*)lisCAR->First();
      lisCAR->Delete(0);
      //verifica se a Barra da Carga está definida
      if (pcarga->pbarra == NULL) {delete pcarga; continue;}
      //verifica se a Carga possui valor desprezível
      if (CargaDesprezivel(pcarga)) {delete pcarga; continue;}
      //define Classe da Carga
      pcarga->DefineClasse(classes->ExisteClasseTag(consEQUIVALENTE));
      //redefine modelo de carga
		// pcarga->Icte_pu = redutivel.mod_carga.Icte_pu;
		// pcarga->Scte_pu = redutivel.mod_carga.Scte_pu;
		// pcarga->Zcte_pu = redutivel.mod_carga.Zcte_pu;
      //insere Carga na Barra
      pcarga->pbarra->InsereEqbar(pcarga);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereTrafoEqv(VTRede *rede, TList *lisTRF)
   {
   //variáveis locais
   VTTrafo  *ptrafo;

   //insere Trafos na Reducao
   while (lisTRF->Count > 0)
      {//retira primeiro Trafo da lista
      ptrafo   = (VTTrafo*)lisTRF->First();
      lisTRF->Delete(0);
      //verifica se as Barras do Trafo estão definidias
      if ((ptrafo->pbarra1 == NULL)||(ptrafo->pbarra2 == NULL))
         {
         delete ptrafo;
         continue;
         }
      //insere Trafo na Reducao
      rede->InsereLigacao(ptrafo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereTrechoEqv(VTRede *rede, TList *lisTRE)
   {
   //variáveis locais
   VTTrecho *ptrecho;

   //insere Trechos na Reducao
   while (lisTRE->Count > 0)
      {//retira primeiro Trecho da lista
      ptrecho   = (VTTrecho*)lisTRE->First();
      lisTRE->Delete(0);
      //verifica se as Barras do Trecho estão definidas
      if ((ptrecho->pbarra1 == NULL)||(ptrecho->pbarra2 == NULL))
         {
         delete ptrecho;
         continue;
         }
      //insere Trecho na Reducao
      rede->InsereLigacao(ptrecho);
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereYrefEqv(TList *lisYREF)
   {
   //variáveis locais
   complex<double> y1, y2, z1;
   VTNo           *pno;
   VTYref         *yref1, *yref2;
   //VTCorte        *corte = reducao->Corte;

   //insere Cargas na Reducao
   while (lisYREF->Count > 0)
      {//retira primeira Zref da lista
      yref1   = (VTYref*)lisYREF->First();
      lisYREF->Delete(0);
      //verifica se a Barra da Zref está definida
      if (yref1->pbarra == NULL) {delete yref1; continue;}
      //insere Zref na Reducao
      yref1->pbarra->InsereEqbar(yref1);
      }
   }
 */
//---------------------------------------------------------------------------
void __fastcall TReduzBarra::InsereYrefEqv(TList *lisYREF)
   {
   //variáveis locais
   VTYref  *yref;

   //insere Cargas na Reducao
   for (int n = lisYREF->Count-1; n >= 0; n--)
      {
      yref = (VTYref*)lisYREF->Items[n];
      //verifica se a Barra da Yref está definida
      if (yref->pbarra == NULL) continue;
      //verifica se Yref tem valor desprezivel
      if (YrefDesprezivel(yref)) continue;
      //transfere Yref de lisYREF para a Barra
      lisYREF->Delete(n);
      yref->pbarra->InsereEqbar(yref);
      }
   //destrói Yref que permaneceram em lisYREF
   LimpaTList(lisYREF);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::LigacaoSeraExcluida(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if (redutivel.lisBAR_EXC->IndexOf(barra) >= 0) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::MontaCrescimentoEquivalente(void)
   {
   //variáveis locais
   //VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //assume crescimento default p/ todas Cargas equivalentes

   return(true);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TReduzBarra::MontaModeloCargaEquivalente(void)
	{
	//variáveis locais
	struct	{
				double  carga, total;
				double  Icte, Scte, Zcte;
				} energia;
	VTCarga    *carga;
	VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	//verifica número de Cargas presentes na parte redutível
	if (redutivel.lisCAR->Count == 0) return;
	if (redutivel.lisCAR->Count == 1)
		{//Carga equivalente assume mesmo modelo da Carga da parte redutível
		carga = (VTCarga*)redutivel.lisCAR->First();
		redutivel.modelo.Icte_pu = carga->Icte_pu;
		redutivel.modelo.Scte_pu = carga->Scte_pu;
		redutivel.modelo.Zcte_pu = carga->Zcte_pu;
		}
	else
		{//calcula energia total das Cargas da parte redutível, e parcela de energia p/ cada modelo de carga
		energia.Icte  = 0;
		energia.Scte  = 0;
		energia.Zcte  = 0;
		energia.total = 0;
		//loop p/ todas as Cargas
		for (int nc = 0; nc < redutivel.lisCAR->Count; nc++)
			{
			carga = (VTCarga*)redutivel.lisCAR->Items[nc];
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
		   redutivel.modelo.Icte_pu = energia.Icte / energia.total;
		   redutivel.modelo.Scte_pu = energia.Scte / energia.total;
		   redutivel.modelo.Zcte_pu = energia.Zcte / energia.total;
         }
		}
   //proteção
   if (int(redutivel.modelo.Icte_pu + redutivel.modelo.Scte_pu + redutivel.modelo.Zcte_pu) != 1)
      {
		redutivel.modelo.Icte_pu = 1;
		redutivel.modelo.Scte_pu = 0;
		redutivel.modelo.Zcte_pu = 0;
      }
	}
*/
//---------------------------------------------------------------------------
void __fastcall TReduzBarra::MontaModeloCargaEquivalente(TList *lisCAR)
	{
	//variáveis locais
	struct	{
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
	if (lisCAR->Count == 0) return;
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
		if (IsDoubleZero(energia.total)) return;
		//calcula parcelas do modelo carga
		redutivel.mod_carga.Icte_pu = energia.Icte / energia.total;
		redutivel.mod_carga.Scte_pu = energia.Scte / energia.total;
		redutivel.mod_carga.Zcte_pu = energia.Zcte / energia.total;
		}
   //proteção
   if (int(redutivel.mod_carga.Icte_pu + redutivel.mod_carga.Scte_pu + redutivel.mod_carga.Zcte_pu) != 1)
      {
		redutivel.mod_carga.Icte_pu = 1;
		redutivel.mod_carga.Scte_pu = 0;
		redutivel.mod_carga.Zcte_pu = 0;
      }
	}

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::MontaRedeEquivalente(bool aviso_enabled)
   {
   //variáveis locais
   double Sbase = 100.;
   bool   eqv_equilibrado;

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
   if (! monta_eqv->Executa(net, redutivel.lisLIG_ALL,
                            redutivel.lisBAR_ALL, redutivel.lisBAR_COR,
                            redutivel.lisBAR_GER, ANO_0, Sbase,
                            aviso_enabled)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::RedefineBarraPaiNo(TList *lisBAR_MAN)
   {
   //variáveis locais
   VTBarra *pbarra;
   VTNo    *no;

   for (int n = 0; n < lisBAR_MAN->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR_MAN->Items[n];
      if ((no = pbarra->No[net]) != NULL) no->pai = pbarra;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::ReiniciaRede(VTRede *rede)
   {
   //variáveis locais
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //reinicia objeto Rede, mas antes retira Barras e Ligacoes originais
   //que deverão permanecer na rede reduzida

   //retira Ligacoes originais que não irão permancer na rede equivalente
   for (int n = 0; n < redutivel.lisLIG_ALL->Count; n++)
      {
      ligacao = (VTLigacao*)redutivel.lisLIG_ALL->Items[n];
      //retira Ligacao de Rede
      ligacao->rede->RemoveLigacao(ligacao);
      delete ligacao;
      }
   //retira Ligacoes conectadas pelas Barras que não irão permanecer na rede equivalente
   //IMPORTANTE: isso deve ser feito para eliminar eventuais Chaves da parte redutivel
   for (int n = 0; n < redutivel.lisBAR_EXC->Count; n++)
      {
      barra = (VTBarra*)redutivel.lisBAR_EXC->Items[n];
      lisTMP->Clear();
      rede->LisLigacao(lisTMP, barra);
      for (int n = 0; n < lisTMP->Count; n++)
         {
         ligacao = (VTLigacao*)lisTMP->Items[n];
         //retira Ligacao de Rede
         ligacao->rede->RemoveLigacao(ligacao);
         delete ligacao;
         }
      }
   //reinicia lista de Barras da Rede
   //rede->IniciaLisBarra();
   //retira de Redes as Barras que não irão permancer na rede equivalente
   for (int n = 0; n < redutivel.lisBAR_EXC->Count; n++)
      {
      barra = (VTBarra*)redutivel.lisBAR_EXC->Items[n];
      //retira Barra
      rede->Redes->RemoveBarra(barra);
      delete barra;
      }
   //insere Ligacoes criadas na redução na nova rede
   InsereTrafoEqv(rede,  monta_eqv->LisTrafoEquivalente());
   InsereTrechoEqv(rede, monta_eqv->LisTrechoEquivalente());
   //insere Eqbar criadas na redução na nova rede
   InsereCargaEqv(monta_eqv->LisCargaEquivalente());
   InsereYrefEqv(monta_eqv->LisYrefEquivalente());
   //reconfigura Redes
   topologia->Reconfigura(redes);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzBarra::ReiniciaRede(VTRedes *redes)
   {
   //variáveis locais
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTRede      *rede;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //reinicia objeto Redes, mas antes retira Barras e Ligacoes originais
   //que deverão permanecer na rede reduzida

   //retira Ligacoes originais que não irão permancer na rede equivalente
   for (int n = 0; n < redutivel.lisLIG_ALL->Count; n++)
      {
      ligacao = (VTLigacao*)redutivel.lisLIG_ALL->Items[n];
      //retira Ligacao de Rede
      ligacao->rede->RemoveLigacao(ligacao);
      delete ligacao;
      }
   //retira Ligacoes conectadas pelas Barras que que não irão permancer na rede equivalente
   //IMPORTANTE: isso deve ser feito para eliminar eventuais Chaves da parte redutivel
   for (int n = 0; n < redutivel.lisBAR_EXC->Count; n++)
      {
      barra = (VTBarra*)redutivel.lisBAR_EXC->Items[n];
      lisTMP->Clear();
      redes->LisLigacao(lisTMP, barra);
      for (int n = 0; n < lisTMP->Count; n++)
         {
         ligacao = (VTLigacao*)lisTMP->Items[n];
         //retira Ligacao de Rede
         ligacao->rede->RemoveLigacao(ligacao);
         delete ligacao;
         }
      }
   //retira de Redes as Barras que não irão permancer na rede equivalente
   for (int n = 0; n < redutivel.lisBAR_EXC->Count; n++)
      {
      barra = (VTBarra*)redutivel.lisBAR_EXC->Items[n];
      //retira Barra pai do No
      redes->RemoveBarra(barra);
      delete barra;
      }
   //cria uma nova Rede e insere em Redes
   rede = DLL_NewObjRede(patamares->NumPatamar());
   rede->Carregada = true;
   rede->Estatica  = true;
   rede->Radial    = false;
   rede->Codigo    = "Rede Equivalente";
   rede->TipoRede  = tipos->ExisteTipoRede(1);
   //transfere todoas as Ligacoes que permanceram em Redes para a Rede equivalente
   TransfereTrechoRedeOrigRedeEqv(redes, rede);
   //elimina todas as Redes
   RetiraTodasRedes(redes);
   //insere a Rede equivalente em Redes SOMENTE após retirar todas as redes originais
   redes->InsereRede(rede);
   //insere Ligacoes criadas na redução na nova rede
   InsereTrafoEqv(rede,  monta_eqv->LisTrafoEquivalente());
   InsereTrechoEqv(rede, monta_eqv->LisTrechoEquivalente());
   //insere Eqbar criadas na redução na nova rede
   InsereCargaEqv(monta_eqv->LisCargaEquivalente());
   InsereYrefEqv(monta_eqv->LisYrefEquivalente());
   //reconfigura Redes
   topologia->Reconfigura(redes);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::RetiraTodasRedes(VTRedes *redes)
   {
   //variáves locais
   VTRede *rede;

   //inicia lisTMP com todas as Redes
   lisTMP->Clear();
   redes->LisRede(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      rede = (VTRede*)lisTMP->Items[n];
      redes->RemoveRede(rede);
      }
   lisTMP->Clear();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::TransfereTrechoRedeOrigRedeEqv(VTRedes *redes, VTRede *rede_eqv)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //inicia lisTMP com as Ligacoes originais que permanceram nas redes originais
   lisTMP->Clear();
   redes->LisLigacao(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      ligacao = (VTLigacao*)lisTMP->Items[n];
      //retira Ligacao de sua Rede original
      ligacao->rede->RemoveLigacao(ligacao);
      //insere Ligacao na Rede equivalente
      rede_eqv->InsereLigacao(ligacao);
      }
   lisTMP->Clear();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzBarra::YrefDesprezivel(VTYref *yref)
   {
   return(IsDoubleZero(Abs(yref->Y0_pu), 1.e-15) &&
          IsDoubleZero(Abs(yref->Y1_pu), 1.e-15));
   }

//---------------------------------------------------------------------------
//eof

