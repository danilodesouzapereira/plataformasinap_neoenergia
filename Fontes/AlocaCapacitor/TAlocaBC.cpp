//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <algorithm>
#include "TAlocaBC.h"
#include "TFormOrderna.h"
#include "VTBanco.h"
#include "VTBancos.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTTronco.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
Value   Description
   Item1 is less than Item2       > 0 (positive)
   Item1 is greater than Item2    < 0 (negative)
   Item1 is equal to Item2          0           
/---------------------------------------------------------------------------*/
static int __fastcall ComparaQ(void *item1, void *item2)
   {
   VTCapacitor *cap1 = (VTCapacitor*) item1;
   VTCapacitor *cap2 = (VTCapacitor*) item2;

   if(cap1->Q_mvar < cap2->Q_mvar) return ( 1);
   if(cap1->Q_mvar > cap2->Q_mvar) return (-1);
   return (0);
   }

//---------------------------------------------------------------------------
VTAlocaBC* __fastcall NewObjAlocaBC(VTApl *apl)
   {
   return (new TAlocaBC(apl));
   }
//---------------------------------------------------------------------------
__fastcall TAlocaBC::TAlocaBC(VTApl *apl)
   {
	//salva ponteiro
   this->apl = apl;
   //cria listas
   lisBar = new TList();
   lisLig = new TList();
   //cria objeto para montagem das redes radiais
   radial = DLL_NewObjRadial(apl);
   tronco = DLL_NewObjTronco(apl);       //DVK 2013.07.24
   //cria objetos
   bancos = NewObjBancos(apl);
	prog   = DLL_NewObjProgresso(NULL, NULL);
	//inicia dados
   cfg.qmax_definido  = false;
   cfg.qmax_mvar       = 10.;
   cfg.fatpot_definido = false;;
   cfg.fatpot_min      = 0.90;
   cfg.dist_definido   = false;
   cfg.dist_m          = 500.;
   cfg.custo_energia   = 0.;
   cfg.tronco          = true;        //DVK 2013.07.24
   ind_cap             = 1;           //DVK 2013.07.24
   cfg.sempre_ligado   = false;
   cfg.ind_patamar     = -1;
   }

//---------------------------------------------------------------------------
__fastcall TAlocaBC::~TAlocaBC(void)
   {
   //destroi objetos criados
   if (tronco) {delete tronco; tronco = NULL;}
   if (radial) {delete radial; radial = NULL;}
   if (bancos) {delete bancos; bancos = NULL;}
   if (prog)   {delete prog;   prog   = NULL;}
   //destroi listas sem destruir seus objetos
   if (lisBar) {delete lisBar; lisBar = NULL;}
   if (lisLig) {delete lisLig; lisLig = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TAlocaBC::AjustaCurvaCapacitor(VTBanco *banco)
   {
   //vari�veis locais
   double         q_bar_mvar;
   VTCapacitor    *capacitor;
   VTBarra        *barra;
   VTDemanda      *demanda    = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares    *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList          *lisCAP     = banco->LisCapacitor();

   //ordena Capacitores em ordem descrescente de pot.reativa
   barra = banco->Barra;
   if (lisCAP->Count > 1) lisCAP->Sort(ComparaQ);

   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//determina pot�ncia reativa na Barra do Capacitor
      //DVK 2014.11.11 verifica se � capacitor sempre ligado
      if(cfg.sempre_ligado)  q_bar_mvar = Gambi_GetQinj_mvar(barra, cfg.ind_patamar);
      else                   q_bar_mvar = Gambi_GetQinj_mvar(barra, ind_pat);
      //loop p/ todos os Capacitores
      for (int nc = 0; nc < lisCAP->Count; nc++)
         {
         capacitor = (VTCapacitor*)lisCAP->Items[nc];
         //ajusta Curva somente dos capacitores novos
         if (! capacitor->Status[sttNOVO]) continue;
         //verifica se o Capacitor deve ficar ligado ou desligado no patamar
         //DVK 2014.11.11 ou sempre ligado
         if (capacitor->Q_mvar <= q_bar_mvar)
            {//capacitor deve ficar ligado no patamar
            demanda->Q_mvar[capacitor][ind_pat] = capacitor->Q_mvar;
            //diminui a pot�ncia reativa na Barra do Capacitor
            q_bar_mvar -= capacitor->Q_mvar;
            }
         else
            {//DVK 2014.11.11 verifica se � capacitor sempre ligado
            if(cfg.sempre_ligado)
               {//capacitor deve ficar ligado no patamar
               demanda->Q_mvar[capacitor][ind_pat] = capacitor->Q_mvar;
               //diminui a pot�ncia reativa na Barra do Capacitor
               q_bar_mvar -= capacitor->Q_mvar;
               }
            //capacitor deve ficar desligado no patamar
            else demanda->Q_mvar[capacitor][ind_pat] = 0.;
            }
         }
      }
   }

//---------------------------------------------------------------------------
VTBanco* __fastcall TAlocaBC::AlocaBancoBarraFilho(VTLigacao *ligacao, VTBarra* barra)
   {
   //vari�veis locais
   int          ncap;
   double       q_mvar, qbar_mvar, qcap_mvar;
   VTBanco     *banco = NULL;
   VTCapacitor *capacitor;

   //determina pot.reativa na Barra filho, no patamar de demanda m�xima
   qbar_mvar  = Gambi_GetQinj_mvar(barra, indPatMax);
   //loop de modelos de Capacitores dispon�veis, ordenados em ordem decrescente de pot.reativa
   for(int n = 0; n < lisTabCap->Count; n++)
      {
      capacitor = (VTCapacitor*) lisTabCap->Items[n];
      //verifica se h� unidades dispon�veis
      if (capacitor->ligacao == 0) continue;
      qcap_mvar = capacitor->Q_mvar;
      //determina n�mero de unidades necess�rias
      ncap = qbar_mvar / qcap_mvar;
      if (ncap == 0) continue;
      //define o n�mero de unidades a ser alocada (menor valor entre dispon�veis e necess�rios)
      ncap = min(ncap, capacitor->ligacao);
      //verifica se h� limite de reativos
      if (cfg.qmax_definido)
         {//determina quanto de pot.reativa ainda pode ser alocada
         q_mvar = cfg.qmax_mvar - QTotalMvarAlocado();
         //define n�mero de unidade alocadas, considerando limite total de reativo
         if ((ncap = min(ncap, (int)(q_mvar/capacitor->Q_mvar))) == 0) continue;
         }
      //verifica se h� Capacitores no estoque
      //if (capac->ligacao < ncap) continue;
      //insere Capacitores em um Banco associado � Barra
      if ((banco = MontaBancoCapacitor(ligacao, barra, capacitor, ncap)) != NULL)
         {//decrementa unidade dispon�veis para aloca��o
         capacitor->ligacao -= ncap;
         if (capacitor->ligacao < 0) capacitor->ligacao = 0;
         //corrige o valor de pot.reativa a se compensada, p/ continuar a aloca��o de capacitor no mesmo Banco
         qbar_mvar  = Gambi_GetQinj_mvar(barra, indPatMax) - banco->Qtotal_mvar();
         //qbar_mvar  = Gambi_GetQinj_mvar(barra, indPatMax) - banco->Q_mvar(indPatMax, false);
         }
      }
   return(banco);
   }

//---------------------------------------------------------------------------
bool __fastcall TAlocaBC::AlocaBC(VTPrimario *primario)
   {
   //var local
   int         index;
   double      q_mvar;
   VTBanco     *banco;
   VTBarra     *barra_filho, *barra_pai;
   VTLigacao   *ligacao;

   //atualiza barra de progresso
   prog->Add(primario->Rede->Codigo + ": an�lise do tronco e dos ramais");
   //obt�m lista ordenada de Ligacoes e Barras do Primario
   lisBar->Clear();
   lisLig->Clear();
   if(! primario->Ordena(lisBar, lisLig)) return false;
   //inicia objeto                        //DVK 2013.07.24
   tronco->DefinePrimario(primario, indPatMax);
   //reabilita an�lise de todas Ligacoes p/ aloca��o de Bancos
   for (int n = 0; n < lisLig->Count; n++)
      {
      ligacao = (VTLigacao*)lisLig->Items[n];
      ligacao->Status[sttENABLED] = true;
      }
   //desabilita liga��es dos ramais
   DesabilitaLigacaoRamais();              //DVK 2013.07.24
   //desabilita liga��es devido � dist�ncia m�nima entre Bancos
   DesabilitaLigacaoPorDistanciaMinima();
   //calcula pot.reativa excedente em cada Barra da Rede
   if (cfg.fatpot_definido) CalculaQmvarExcedente2();
   else                     CalculaQmvarExcedente1();
   try{//executa an�lise de todas as Barras
      for (index = lisLig->Count-1; index >= 0; index--)
         {
         ligacao = (VTLigacao *) lisLig->Items[index];
         //verifica se a Ligacao pode ser analisada
         if (! ligacao->Status[sttENABLED]) continue;
         //verifica se j� atingiu o limite de Qmax
         if (QMaxLimitado()) return true;
         //come�a pela segunda barra  (pai-filho)
         barra_filho = BarraFilho(ligacao);
         //determina pot.reativa armazenda em ResFlowBar
         if(cfg.sempre_ligado)
            {//DVK 2014.11.11 analisa somente patamar selecionado
            q_mvar = Gambi_GetQinj_mvar(barra_filho, cfg.ind_patamar);
            }
         //DVK 2014.11.11 analisa patamar m�ximo
         else q_mvar = Gambi_GetQinj_mvar(barra_filho, indPatMax);
         //Q > banco => aloca N bancos necess�rios
         if ((q_mvar > 0.) && (q_mvar >= QMin()))
            {//aloca capacitor considerando apenas o patamar de demanda m�xima
            if ((banco = AlocaBancoBarraFilho(ligacao, barra_filho)) == NULL) continue;
            //ajusta curva do capacitor para os demais patamares
            AjustaCurvaCapacitor(banco);
            //desabilita an�lise p/ todas liga��es a montante
            DesabilitaLigacaoMontante(ligacao);
            }
         }
      }catch(Exception &e)
         {
         return false;
         }
   return true;
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TAlocaBC::BarraFilho(VTLigacao *ligacao)
   {
   int index1, index2;
   index1 = lisBar->IndexOf(ligacao->pbarra1);
   index2 = lisBar->IndexOf(ligacao->pbarra2);

   if(index2 > index1) return (ligacao->pbarra2);

   return(ligacao->pbarra1);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TAlocaBC::BarraPai(VTLigacao *ligacao)
   {
   int index1, index2;
   index1 = lisBar->IndexOf(ligacao->pbarra1);
   index2 = lisBar->IndexOf(ligacao->pbarra2);

   if(index1 < index2) return (ligacao->pbarra1);

   return(ligacao->pbarra2);      
   }
//---------------------------------------------------------------------------
complex<double> __fastcall TAlocaBC::CalculaDemandaTotal(VTPatamar *patamar)
   {
   //vari�veis locais
   complex<double> s_mva;
   VTCarga        *carga;
   VTRedes        *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTDemanda      *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //zera os valores
   s_mva = CZero();

   //obt�m lista de Cargas do Primario
   TList *lisCARGA = new TList();
   redes->LisEqbar(lisCARGA, eqptoCARGA);

   //calcula somat�ria das demandas das cargas prim�rias e equivalentes
   for (int nc = 0; nc < lisCARGA->Count; nc++)
      {
      carga = (VTCarga*)lisCARGA->Items[nc];
      //determina demanda da Carga
      s_mva += demanda->S_mva[carga][patamar->Index];
      }
   if(lisCARGA) {delete lisCARGA;}
   return(s_mva);
   }

//---------------------------------------------------------------------------
bool __fastcall TAlocaBC::CalculoFluxo(void)
   {
   //vari�veis locais
   VTFlow  *flow   = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   return(flow->CalculaPorRede(redes));
   }

//---------------------------------------------------------------------------
void __fastcall TAlocaBC::CalculaQmvarExcedente1(void)
   {
   //vari�veis locais
   int             ind_bar;
   complex<double> s_bar_mva;
   VTBarra     *barra;
   VTLigacao   *ligacao;
   VTPatamares *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //atualiza ResFlowBar com a pot.reativa da barra final da Ligacao
   for (int nl = 0; nl < lisLig->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLig->Items[nl];
      barra = BarraFilho(ligacao);
      ind_bar = ligacao->IndiceBarra(barra);
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//detemina fluxo na Ligacao
         s_bar_mva = ligacao->resflow->S_mva[ind_bar][ind_pat];
         //salva na Barra apenas pot.reativa do fluxo na Ligacao
         //barra->resflow->S_mva[ind_pat] =  - complex<double> (0. , s_mva.imag());
         Gambi_SetQinj_mvar(barra, ind_pat, - s_bar_mva.imag());
         }
      }
   }
//---------------------------------------------------------------------------
void __fastcall TAlocaBC::CalculaQmvarExcedente2(void)
   {
   //vari�veis locais
   int             ind_bar;
   int             num_cap;
   double          q_bar_mvar, q_max_mvar, dq_mvar;
   complex<double> s_bar_mva;
   VTBarra         *barra;
   VTLigacao       *ligacao;
   VTPatamares     *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //atualiza ResFlowBar com o saldo da pot.reativa da barra final da Ligacao que dever�
   //ser compensada pelos Capacitores
   for (int nl = 0; nl < lisLig->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLig->Items[nl];
      barra = BarraFilho(ligacao);
      ind_bar = ligacao->IndiceBarra(barra);
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//detemina fluxo na Ligacao
         s_bar_mva  = ligacao->resflow->S_mva[ind_bar][ind_pat];
         q_bar_mvar = -s_bar_mva.imag();
         //determina pot�ncia reativa m�xima admiss�vel, considerando o fator de pot�ncia m�nimo
         q_max_mvar = Abs(s_bar_mva) * Sin(ArcCos(cfg.fatpot_min));
         //calcula a pot.reativa a ser compensada: diferen�a entre a pot.reativa atual e a pot.reativa m�xima
         //if (q_bar_mvar > q_max_mvar) q_bar_mvar -= q_max_mvar;
         //else                         q_bar_mvar  = 0.;
         if (q_bar_mvar <= q_max_mvar)
            {
            dq_mvar  = 0.;
            }
         else
            {//determina a diferen�a entre a pot.reativa na ligacao e a pot.reativa m�xima
            dq_mvar = q_bar_mvar - q_max_mvar;
            //determina n�mero de capacitores de menor pot.reativa que correspondem � diferen�a d� pot.reativa
            //dq_mvar =  (1 + int(dq_mvar/QMin())) * QMin();
            //dq_mvar =  int(dq_mvar/QMin()) * QMin();
            num_cap = int(dq_mvar/QMin());
            //incrfementa n�mero de capacitores caso a pot.reativa do capacitor seja menor que 25% da pot. reativa na ligacao
//            if (QMin() / q_bar_mvar < 0.25)
            if (QMin() / q_bar_mvar < 0.5)
               {num_cap++;}
            dq_mvar = num_cap * QMin();
            }
         //salva na Barra apenas pot.reativa
         Gambi_SetQinj_mvar(barra, ind_pat, dq_mvar);
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TAlocaBC::CriaCurvaPropria(VTCapacitor* cap)
   {
   double       valor[nvCURVA_PQ];
   double       p,q;
   strHM          patHM;
   VTCurva      *curva;
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	curva                = DLL_NewObjCurva();
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Tipica        = false;
   curva->Eqpto         = cap;
   curva->Classe        = NULL;
   //cria pontos da Curva, c/ valores nulos
   curva->Reconfigura(patamares->Escala);
   //define valores dos pontos da Curva pr�pria
   for(int ind_pat = 0 ; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar      = patamares->Patamar[ind_pat];
      patHM.hora   = patamar->HoraIni;
      patHM.minuto = patamar->MinutoIni;
      //obt�m demanda do eqpto no patamar
      valor[0] = 0;
      valor[1] = 0;
      curva->SetPonto(patHM, valor, nvCURVA_PQ);
      }
   //redefine Curva do capcitor
   cap->DefineCurva(curva);

   return true;
   }
   
//---------------------------------------------------------------------------
void __fastcall TAlocaBC::DesabilitaLigacaoMontante(VTLigacao *ligacao)
   {
   //elimina todas as Ligacoes Pai da lista
   while (ligacao != NULL)
      {//altera status da Ligacao
      ligacao->Status[sttENABLED] = false;
      //determina Ligacao pai
      ligacao = ligacao->ligaPai;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAlocaBC::DesabilitaLigacaoPorDistanciaMinima(void)
   {
   //vari�veis locais
   double    dist_m;
   VTBanco   *banco;
   VTLigacao *ligacao;
   TList     *lisBANCO = bancos->LisBanco();

   //verifica se op��o de dist�ncia m�nima est� habilitada
   if (! cfg.dist_definido) return;
   //loop p/ todos Bancos alocados
   for (int n = 0; n < lisBANCO->Count; n++)
      {
      banco = (VTBanco*)lisBANCO->Items[n];
      //reinicia distancia bas Barras � montante
      dist_m = 0;
      //desabilita Ligacoes a montante at� a dist�ncia definida
		ligacao = banco->Ligacao;
      while ((ligacao != NULL) && (dist_m < cfg.dist_m) )
         {//altera status da Ligacao
         ligacao->Status[sttENABLED] = false;
         //atualiza dist�ncia
         if (ligacao->Tipo() == eqptoTRECHO) dist_m += ((VTTrecho*)ligacao)->Comprimento_m;
         //determina Ligacao pai
			ligacao = ligacao->ligaPai;
         }
      }
   }

 //---------------------------------------------------------------------------
void __fastcall TAlocaBC::DesabilitaLigacaoRamais(void)
   {  //DVK 2013.07.24
   //vari�veis locais
   VTLigacao *ligacao;
   TList     *lisLIGR = tronco->LisLigacaoRamal();

   //verifica se op��o de tronco est� habilitada
   if (! cfg.tronco) return;
   //loop p/ todos Bancos alocados
   for (int n = 0; n < lisLIGR->Count; n++)
      {//desabilita Ligacoes dos ramais
      ligacao = (VTLigacao*)lisLIGR->Items[n];
      //altera status da Ligacao
      ligacao->Status[sttENABLED] = false;
      }
   }

//---------------------------------------------------------------------------
int __fastcall TAlocaBC::DeterminaPico(void)
   {
   //var local
   VTPatamares *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat     = patamares->NumPatamar();
   int         indPatMax   = 0;
   double      dem,  demMax = 0.;
   complex<double>  dem3fPQ;

   //loop p/ todos Patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
      //calcula valores de demanda do patamr
      dem3fPQ = CalculaDemandaTotal(patamares->Patamar[ind_pat]);
      dem  = Abs(dem3fPQ);
      if(dem > demMax)
         {
         indPatMax = ind_pat;
         demMax    = dem;
         }
      }
   return(indPatMax);
   }

//---------------------------------------------------------------------------
bool __fastcall TAlocaBC::Executa(TList *lisTabCap)
   {
   //var local
   bool        resp;
	int         iteracao = 1;
	TList 		*lisPri;
   VTPrimario  *pri;
   VTRedes     *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));

   //salva lista de tabela de capacitores definidos
   this->lisTabCap = lisTabCap;
   //reinicia Bancos alocados
   bancos->Reinicia();
   //ordena a lista de modelos de capacitores
   if (lisTabCap->Count > 1) lisTabCap->Sort(ComparaQ);
   //prote��o
   if ((! radial)||(! tronco)) return false;
   //executa a montagem da rede em radiais
   if (! radial->Inicia(redes))return false;
   //loop p/ todos primarios
	lisPri = radial->LisPrimario();
   do {//loop at� n�o inserir mais nenhum capacitor
      prog->Start(progTEXTO);
      prog->Add("Aloca��o de Banco de Capacitores - Itera��o " + IntToStr(iteracao));
      prog->Add("Diagn�stico da Rede");
      //executa fluxo de pot�ncia
      if (! CalculoFluxo())
         {
         Aviso("N�o foi poss�vel calcular o fluxo de pot�ncia da rede");
         prog->Stop();
         return false;
         }
      //determina o pico de carregamento
      this->indPatMax = DeterminaPico();
      for (int n = 0; n < lisPri->Count; n++)
         {
         pri = (VTPrimario*) lisPri->Items[n];
         //verifica se a Rede est� carregada
         if (! pri->Rede->Carregada) continue;
         //aloca capacitor no alimentador
         if(! AlocaBC(pri))
            {
            //prog->Hide();
            //return false;
            }
         }
      //insere na Rede os Capacitores dos Bancos montados
      }while((InsereCapacitoresNaRede() > 0)&&(iteracao++ <= 20));
   prog->Stop();
	//return true;
	if (bancos->NumBanco() == 0) Aviso("Essa rede n�o precisa alocar mais capacitores.");
	return (bancos->NumBanco());
   }

//---------------------------------------------------------------------------
double __fastcall TAlocaBC::Gambi_GetQinj_mvar(VTBarra *barra, int pat)
   {
   //retorna potencia reativa armazenada em ResFlowbar
   return(barra->resflow->Ia_amp[pat].imag());
   }

//---------------------------------------------------------------------------
void __fastcall TAlocaBC::Gambi_SetQinj_mvar(VTBarra* barra, int pat, double q_mvar)
   {
   barra->resflow->Ia_amp[pat] = complex<double>(0., q_mvar);
   }

//---------------------------------------------------------------------------
int __fastcall TAlocaBC::InsereCapacitoresNaRede(void)
   {
   //vari�veis locais
   int         count;
   TList       *lisEQP;
   VTCapacitor *capacitor;
	VTEdita     *edita  = (VTEdita*)apl->GetObject(__classid(VTEdita));

   lisEQP = new TList();
   bancos->LisCapacitor(lisEQP);
   //retira da lista os Capacitores que n�o s�o novos
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      capacitor = (VTCapacitor*)lisEQP->Items[n];
      if (! capacitor->Status[sttNOVO])
         {//retira Capacitor da lista
         lisEQP->Delete(n);
         }
      }
   //insere Capacitores na Rede
   edita->InsereLisEqpto(lisEQP);
   //redefine Status dos Capacitores
   for (int n = (lisEQP->Count-1); n >= 0; n--)
      {
      capacitor = (VTCapacitor*)lisEQP->Items[n];
      capacitor->Status[sttNOVO] = false;
      }
   count = lisEQP->Count;
   delete lisEQP;
   return(count);
   }

//---------------------------------------------------------------------------
TList* __fastcall TAlocaBC::ListaBancoAlocado(void)
   {
   return (bancos->LisBanco());
   }

//---------------------------------------------------------------------------
int __fastcall TAlocaBC::ListaCapacitorAlocado(TList *lisEXT)
   {
   return(bancos->LisCapacitor(lisEXT));
   }

//---------------------------------------------------------------------------
int __fastcall TAlocaBC::ListaCloneCapacitorAlocado(TList *lisEXT)
   {
   return(bancos->LisCloneCapacitor(lisEXT));
   }

//---------------------------------------------------------------------------
VTBanco* __fastcall TAlocaBC::MontaBancoCapacitor(VTLigacao *ligacao, VTBarra* barra, VTCapacitor* capac, int unidades)
   {
   //vari�veis locais
   strHM        patHM;
   double       valor[nvCURVA_PQ];
   VTBanco     *banco;
   VTCapacitor *capNovo;
   VTPatamar   *patamar;
   VTPatamares *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //verifica se existe um Banco associado � Barra
   if ((banco = bancos->ExisteBanco(barra)) == NULL)
      {//cria um novo Banco
      banco = bancos->InsereBanco(barra);
      //define Ligacao de refer�ncia do Banco
      banco->Ligacao = ligacao;
      }
   //cria capacitores novos
   for (int n = 0; n < unidades; n++)
      {//cria um novo Capacitor
      capNovo = DLL_NewObjCapacitor(patamares->NumPatamar());
      capNovo->Id = FalsoId();                              //DVK 2013.07.24
      if(ind_cap < 10) capNovo->Codigo = "Cap_0" + IntToStr(ind_cap); //DVK 2013.07.24
      else capNovo->Codigo = "Cap_" + IntToStr(ind_cap);    //DVK 2013.07.24
      ind_cap++;                                            //DVK 2013.07.24
      capNovo->q = capac->Q_mvar;
      //define esquematico na tela
      capNovo->esq.posbar =   50;
      capNovo->esq.dxbar  =  2000;
      capNovo->esq.dybar  = -2000;
      //cria curva de carga p/ capacitor
      CriaCurvaPropria(capNovo);
      //ajusta a curva de carga
      for(int pat = 0; pat < patamares->NumPatamar(); pat++)
         {
         patamar      = patamares->Patamar[pat];
         patHM.hora   = patamar->HoraIni;
         patHM.minuto = patamar->MinutoIni;
         //define o valor de QCap na curva
         valor[0] = 0.;
         valor[1] = capac->Q_mvar;
         capNovo->curva->SetPonto(patHM, valor, nvCURVA_PQ);
         }
      //insere Capaciotr no Banco
      banco->InsereCapacitor(capNovo);
      }
   return(banco);
   }

//---------------------------------------------------------------------------
bool __fastcall TAlocaBC::QMaxLimitado(void)
   {
   //variaveis
   double qmin, qalocado;

   //n�o h� limite:
   if (! cfg.qmax_definido) return false;

   qalocado = QTotalMvarAlocado();
   qmin     = QMin();

   return( RoundTo(qalocado + qmin, -4) > cfg.qmax_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TAlocaBC::QMax(void)
   {
   VTCapacitor *cap;
   if(lisTabCap->Count == 0) return 0.;        

   //determina o capacitor de menor potencia
   cap = (VTCapacitor *) lisTabCap->First();
   return (cap->Q_mvar);
   }
//---------------------------------------------------------------------------
double __fastcall TAlocaBC::QMin(void)
   {
   VTCapacitor *cap;
   if(lisTabCap->Count == 0) return 0.;
   //determina o capacitor de menor potencia
   cap = (VTCapacitor *) lisTabCap->Last();
   return (cap->Q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TAlocaBC::QTotalMvarAlocado(void)
   {
   return(bancos->Qtotal_mvar());
   }

/*
//---------------------------------------------------------------------------
void __fastcall TAlocaBC::ReiniciaResFlowBar(VTLigacao *ligacao, VTBanco *banco)
   {
   //vari�veis locais
   double          q_bar_mvar, q_cap_mvar;
   VTBarra        *barra;
   VTPatamares    *patamares  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //loop p/ todas Ligacoee, at� Ligacao pai inicial
   while (ligacao != NULL)
      {//atualiza apenas Barra final da Ligacao
      barra = BarraFilho(ligacao);
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//detemina pot.reativa injetada pelo Banco no patamar, sem considerar tens�o da barra
         q_cap_mvar  = banco->Q_mvar(ind_pat);
         if (IsDoubleZero(q_cap_mvar)) continue;
         //determina pot.reativa armazena em ResFlowBar
         q_bar_mvar = Gambi_GetQinj_mvar(barra, ind_pat);
         //atualiza pot.reativa armazenada em ResFlowBar
         q_bar_mvar = q_bar_mvar - q_cap_mvar;
         Gambi_SetQinj_mvar(barra, ind_pat, q_bar_mvar);
         }
      //determina Ligacao pai
      ligacao = ligacao->ligaPai;
      }
   }
 */
//---------------------------------------------------------------------------
void __fastcall TAlocaBC::Reinicia(void)
   {
   bancos->Reinicia();
	}

//---------------------------------------------------------------------------
bool __fastcall TAlocaBC::RetiraBancosAlocadosDaRede(TList *lisCap)
	{ //DVK 2015.07.22
	//vari�veis locais
	VTCapacitor *cap;
	VTEdita 		*edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	//prote��o
	if((! edita) || (! lisCap)) return(false);
   //FKM - 2015.09.24
	//apaga capacitores das listas dos bancos
   for (int nc = 0; nc < lisCap->Count; nc++)
		{
		cap = (VTCapacitor*)lisCap->Items[nc];
		if(! bancos->RetiraCapacitor(cap))
			{
			Aviso("Capacitor " + cap->Codigo + " n�o encontrado");
			}
		}
   //FKM - 2015.09.24
	//apaga capacitores da rede
	edita->RetiraLisEqpto(lisCap);

	return(true);
	}

//---------------------------------------------------------------------------
int __fastcall TAlocaBC::VerificaPrimariosIsolados(TList *lisEXT)
   {
   //vari�veis locais
   TList      *lisPRIMARIO;
   VTPrimario *primario;
   VTRedes    *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));

   //monta lisEQP com Redes prim�rias sem Barra inicial
   lisEXT->Clear();
   //prote��o
   if (! radial) return(0);
   //executa a montagem da rede em radiais
   if (! radial->Inicia(redes))return(0);
   //identifica Primarios em Barra inicial
   lisPRIMARIO = radial->LisPrimario();
   for (int n = 0; n < lisPRIMARIO->Count; n++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[n];
      //verifica se a Rede est� carregada
      if (! primario->Rede->Carregada) continue;
      if (primario->BarraInicial == NULL)
         {
         lisEXT->Add(primario->Rede);
         }
      }
   return(lisEXT->Count);
   }


//---------------------------------------------------------------------------
//eof
