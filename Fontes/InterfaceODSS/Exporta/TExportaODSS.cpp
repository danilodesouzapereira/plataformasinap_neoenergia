//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Ordena\VTOrdena.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Potencia\VTFornecimento.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCapacitor.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTMatC.h>
#include <Fontes\Rede\VTMatZ.h>
#include <Fontes\Rede\VTNet.h>
#include <Fontes\Rede\VTNo.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrecho.h>
#include <Fontes\Zona\VTZona.h>
#include <Fontes\Zona\VTZonas.h>
#include <DLL_Inc\Curvas.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Ordena.h>
#include <DLL_Inc\Rede.h>
#include "TExportaODSS.h"
#include "VTMonta.h"
//teste
#include <Fontes\Estabilidade\Eqpto\VTEqptos.h>
#include <Fontes\Estabilidade\Eqpto\VTMaq.h>
#include <Fontes\Estabilidade\Evento\VTEvento.h>
#include <Fontes\Estabilidade\Evento\VTEventos.h>

#pragma package(smart_init)

//---------------------------------------------------------------------------
VTExportaODSS* NewObjExportaODSS(VTApl *apl)
   {
   return(new TExportaODSS(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExportaODSS::TExportaODSS(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   //redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //cria objetos
   lines         = new TStringList();
   monitores     = new TStringList();
   lisREDE       = new TList();
   lisARR        = new TList();
   lisCAR        = new TList();
   lisEQP        = new TList();
   lisLIG        = new TList();
   lisZONA       = new TList();
   //inicia dados
   comp_min_km = 0.001;
   }

//---------------------------------------------------------------------------
__fastcall TExportaODSS::~TExportaODSS(void)
   {
   //destr�i objetos
   if (net)   {delete net;   net   = NULL;}
   if (monta) {delete monta; monta = NULL;}
   if (lines) {delete lines; lines = NULL;}
   if (monitores) {delete monitores; monitores = NULL;}
   //destr�i lista sem destruir objetos
   if (lisREDE) {delete lisREDE; lisREDE = NULL;}
   if (lisARR)  {delete lisARR;  lisARR  = NULL;}
   if (lisCAR)  {delete lisCAR;  lisCAR  = NULL;}
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
   if (lisZONA) {delete lisZONA; lisZONA = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::AtualizaCargaEquivalente(void)
   {
   //vari�veis locais
   VTBarra   *barra;
   VTCarga   *carga, *carga_eqv;
   VTEdita   *edita  = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTRedes   *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisBAR = redes->LisBarra();

   //reinciia lisCAR
   lisCAR->Clear();
   //loop p/ todas as Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //obt�m lista de Cargas da Barra
      lisEQP->Clear();
      barra->LisEqbar(lisEQP, eqptoCARGA);
      if (lisEQP->Count > 1)
         {//cria Carga equivalente
         if ((carga_eqv = CriaCargaEquivalente(lisEQP)) != NULL)
            {//elimina todas Cargas da Barra
            for (int n = 0; n < lisEQP->Count; n++)
               {
               carga = (VTCarga*)lisEQP->Items[n];
               barra->RemoveEqbar(carga);
               }
            //destr�i Cargas retiradas da Barra
            LimpaTList(lisEQP);
            //insere Carga equivalente em lisCAR
            carga_eqv->pbarra = barra;
            lisCAR->Add(carga_eqv);
            barra->InsereEqbar(carga_eqv);
            }
         }
      }
   //atualiza a Rede
   if (lisCAR->Count > 0) edita->InsereLisEqpto(lisCAR);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::AtualizaFasesBarraDosReguladores(void)
   { //DVK 2014.09.24
   //vari�veis locais
   VTBarra     *bar_pri, *bar_sec;
   VTNo        *no_pri, *no_sec;
   VTRegulador *regulador;
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//atualiza fases das Barras para que barras conectadas por reg monof�sicos
      //sejam sempre aterradas mesmo que o enrolamento do reg n�o seja aterrado
      lisEQP->Clear();
      redes->LisEqpto(lisEQP, eqptoREGULADOR);
      for (int n = 0; n < lisEQP->Count; n++)
         {
         regulador = (VTRegulador*)lisEQP->Items[n];
         if (NumeroDeFases(regulador) > 1) continue;
         //determina Barra do prim�rio
         bar_pri = regulador->sentido.bar_ref;
         //determina Barra do secund�rio
         if (bar_pri == regulador->pbarra1) bar_sec = regulador->pbarra2;
         else                               bar_sec = regulador->pbarra1;
         //determina Nos das Baras
         no_pri = bar_pri->No[NULL];
         no_sec = bar_sec->No[NULL];
         //verifica o tipo de ligacao do Regulador
         switch (regulador->ligacao)
            {
            case lgEST_AT:    //3 unidades em estrela aterrado
               //inclui faseNT nos Nos das duas Barras para que o neutro do trecho seja conectado ao Node 0 (refer�ncia)
               no_pri->ConfiguraFasesNT();
               no_sec->ConfiguraFasesNT();
               break;
            case lgEST_ISO:   //3 unidades em estrela isolado
               //nada a fazer
               no_sec->ConfiguraFasesNT();
               break;
            case lgMONOFASICO:   //uma unidade fase/neutro
               /*
               //verifica se utiliza o neutro
               if (regulador->fases & faseN)
                  {//inclui faseNT nos Nos das duas Barras para que o neutro do trecho seja conectado ao Node 0 (refer�ncia)
                  no_pri->Fases = no_pri->Fases | faseNT;
                  no_sec->Fases = no_sec->Fases | faseNT;
                  }
                  */
               //nada a fazer
               break;
            case lgDELTA_ABERTO: //duas unidades fase/neutro
               //nada a fazer
               break;
            case lgTRIANG:
            case lgDELTA_FECHADO: //3 unidades em delta fechado
               //nada a fazer
               break;
            default:
               return(false);
            }
         }
      }catch(Exception &e)
         {
         Erro("Erro na atualiza��o das fases das barras dos reguladores");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::AtualizaFasesBarraDosTrafos(void)
   {
   //vari�veis locais
   VTBarra *bar_pri, *bar_sec;
   VTNo    *no_pri, *no_sec;
   VTTrafo *trafo;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//atualiza fases das Barras conectadas por trafos monof�sicos ligados em delta aberto ou fechado
      lisEQP->Clear();
      redes->LisEqpto(lisEQP, eqptoTRAFO);
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trafo = (VTTrafo*)lisEQP->Items[n];
         if (! trafo->Monofasico) continue;
         bar_pri = trafo->BarraPrimario();
         bar_sec = trafo->BarraSecundario();
         if ((bar_sec->zona->Tipo == zonaDELTA_ABERTO) || (bar_sec->zona->Tipo == zonaDELTA_FECHADO))
            {//inclui faseNT nos Nos das duas Barras para que o neutro do trecho seja conectado ao Node 0 (refer�ncia)
            no_pri = bar_pri->No[NULL];
            no_sec = bar_sec->No[NULL];
            no_pri->ConfiguraFasesNT();
            no_sec->ConfiguraFasesNT();
            //verifica se a Zona � delta aberto
            if (bar_sec->zona->Tipo == zonaDELTA_ABERTO)
               {//Trafos em delta/aberto: insere faseNT do enrolamento prim�rio
               trafo->pri.fase  = trafo->pri.fase | faseNT;
               }
            else
               {//Trafos em delta/fechado: insere faseN e elimina faseT do enrolamento prim�rio
               trafo->pri.fase  = trafo->pri.fase | faseN;
               trafo->pri.fase  = trafo->pri.fase & faseABCN;
               }
            }
         }
      }catch(Exception &e)
         {
         Erro("Erro na atualiza��o das fases das barras dos trafos");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaODSS::AtualizaLigacaoTrafoDy1(void)
   {
   //vari�veis locais
   VTTrafo *trafo;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia lista de todos Trafos
   lisLIG->Clear();
   redes->LisLigacao(lisLIG, eqptoTRAFO);
   for (int n = 0; n < lisLIG->Count; n++)
      {
      trafo = (VTTrafo*)lisLIG->Items[n];
      if (trafo->Monofasico) continue;
      //verifica se Trafo D/Y
      if ((trafo->pri.ligacao == lgTRIANG) && (trafo->sec.ligacao != lgTRIANG))
         {
         trafo->sec.defasagem = -(fabs(trafo->sec.defasagem));
         }
      }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaODSS::CodigoOdssEqpto(VTEqpto *eqpto)
   {
   //vari�veis locais
   AnsiString codigo = eqpto->Codigo;
   int        length = codigo.Length();

   //substitui todos caracteres n�o alfa-num�ricos por '_'
   for (int n = 1; n <= length; n++)
      {
      if (! isalnum(codigo[n])) codigo[n] = '_';
      }
   return(codigo + "_id" + IntToStr(eqpto->Id));
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TExportaODSS::CriaCargaEquivalente(TList *lisCARGA)
   {
   //vari�veis locais
   complex<double> s_mva;
   VTCarga         *carga, *carga_eqv;
   VTClasses       *classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//cria uma nova Carga com curva pr�pria
      carga_eqv                 = DLL_NewObjCarga(patamares->NumPatamar());
      carga_eqv->Id             = FalsoId();
      carga_eqv->Codigo         = "";
      carga_eqv->Energia_mwhmes = 0.;
      carga_eqv->Icte_pu        = 1.;
      carga_eqv->Scte_pu        = 0.;
      carga_eqv->Zcte_pu        = 0.;
      carga_eqv->Fases          = faseINV;
      carga_eqv->classe         = classes->ExisteClasseTag(consEQUIVALENTE);
      //cria e inicia Curva de demanda da Carga
      carga_eqv->curva                 = DLL_NewObjCurva();
      carga_eqv->curva->Id             = FalsoId();
      carga_eqv->curva->Codigo         = "";
      carga_eqv->curva->Tipo           = curvaPQ;
      carga_eqv->curva->Unidade        = unidMVA;
      carga_eqv->curva->Escala         = pat01HORA;
      carga_eqv->curva->NumeroValores  = nvCURVA_PQ;
      carga_eqv->curva->Tipica         = false;
      carga_eqv->curva->DiaMedicao     = SEMANA_COMPLETA;
      carga_eqv->curva->Eqpto          = carga_eqv;
      carga_eqv->curva->Classe         = NULL;
      carga_eqv->curva->LimiteInferior = 0;
      carga_eqv->curva->LimiteSuperior = 0;
      //define fases da Carga equivalente
      for (int n = 0; n < lisCARGA->Count; n++)
         {
         carga = (VTCarga*)lisCARGA->Items[n];
         //atualiza fases da Carga equivalente
         if (carga_eqv->Fases == faseINV) carga_eqv->Fases  = carga->Fases;
         else                             carga_eqv->Fases |= carga->Fases;
         }
      //incorpora demanda da Carga na demanda da Carga equivalente
      for (int n = 0; n < lisCARGA->Count; n++)
         {
         carga = (VTCarga*)lisCARGA->Items[n];
         for (int np = 0; np < patamares->NumPatamar(); np++)
            {//determina demanda da Carga equivalente da Et no patamar
            s_mva  = demanda->S_mva[carga_eqv][np];
            //soma demanda da Carga no patamar
            s_mva += demanda->S_mva[carga][np];
            //atualiza demanda da Carga equivalente da Et no patamar
            demanda->S_mva[carga_eqv][np] = s_mva;
            }
         }
      }catch(Exception &e)
         {
         carga_eqv = NULL;
         }
   return(carga_eqv);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaODSS::EliminaRedesComeErroNoFluxo(void)
   {
   //vari�veis locais
   VTRede *rede;
   VTEdita *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTFlow  *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //configura para fluxo desequilibrado
   flow->AvisoEnabled       = true;
   flow->TramoEnabled       = true;
   flow->UpToDate           = false;
   flow->PerdaFerroEnabled  = true;
   flow->MultiThreadEnabled = true;
   flow->ModeloCarga        = cargaAUTO;
   flow->TipoFluxo          = fluxoAUTO;
   flow->TipoRelatorio      = relNENHUM;
   //executa fluxo
//Aviso("C�lculo de fluxo para eliminar redes com erro");
   flow->CalculaPorRede(redes);

//marca que o fluxo n�o foi calculado: eliminar esta linha ap�s corre��o dos Trafos delta/fechado
flow->UpToDate  = false;

   //monta lista com todas as redes que n�o foram calculadas
   lisEQP->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->FluxoOK) lisEQP->Add(rede);
      }
   //verifica se h� redes n�o calculadas
   if (lisEQP->Count > 0)
      {
      edita->RetiraLisRede(lisEQP);
      }
   }

//---------------------------------------------------------------------------
VTZona* __fastcall TExportaODSS::ExisteZona(TList *lisZONA, double vff_kv)
   {
   //vari�veis locais
   VTZona *zona;

   for (int n = 0; n < lisZONA->Count; n++)
      {
      zona = (VTZona*)lisZONA->Items[n];
      if (IsDoubleZero(zona->Vff_kv - vff_kv)) return(zona);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall  TExportaODSS::ExportaCenarioEstabilidade(AnsiString arq_txt)
{
	strOPCAO_ODSS  opcao;

	opcao.rede_aneel = false;
	opcao.trafoDY = 1;
	opcao.carga_eqv = 0;
	try{//destr�i eventual objeto VTMonta criado anteriormente
	  if (monta) {delete monta;}
	  //cria objeto auxiliar na montagem do arquivo
		if (opcao.rede_aneel) monta = NewObjMontaAneel(lines);
		else                  monta = NewObjMonta(lines);
	  //verifica se trafos trif�sicos devem ser configurados em Dy1
	  if (opcao.trafoDY == 1)
		 {//configura todos os Trafos delta/estrela em Dy1
		 AtualizaLigacaoTrafoDy1();
		 }
	  //verifica se Cargas de uma mesma Barra devem ser substituidas por Carga equivalente
	  if (opcao.carga_eqv == 1)
		 {//cria Cargas equivalentes
		 AtualizaCargaEquivalente();
		 }
	  //elimina Redes que n�o podem ser calculadas pelo fluxo
	  //DVK 2016.12.14 comentado
//      EliminaRedesComeErroNoFluxo();
      //cria objeto Net
      if (! IniciaNet()) return(false);
      //IMPORTANTE: adapta��o para OpenDSS:
	  //Inclui faseNT nas Barras conectadas por Trafos monof�sicos em delta aberto ou fechado
      //Isto � feito para que o objeto Line que conecta o Bus do prim�rio seja conectado ao
      //Node 0 (refer�ncia).
      //Os enrolamentos dos prim�rios dos Trafos monof�sicos ser�o conectados
      //ao Node 9, formando uma liga��o estrela isolada
	  AtualizaFasesBarraDosTrafos();
      //DVK 2014.09.24 o mesmo � feito para os reguladores de tens�o
      AtualizaFasesBarraDosReguladores();
      //reinicia lines
      lines->Clear();
		//insere coment�rio indicando modelo de rede: padr�o ANEEL ou rede real
		//if (opcao.rede_aneel) InsereHeader("Modelo de rede: padr�o ANEEL");
		//else                  InsereHeader("Modelo de rede: cadastro GIS");
		InsereHeader(monta->ModeloDeRede());
		//insere coment�rio indicando ligac��o dos trafos DY
	  if (opcao.trafoDY == 1) InsereHeader("Transformadores trif�sicos: ligaca��o Dy1");
      else                    InsereHeader("Transformadores trif�sicos: ligaca��o definida pelo equipamento");
      //insere bloco com comando Clear
      MontaBlocoClear();
	  //insere bloco com comando New Circuit
      if(! MontaBlocoNewCircuit()) return(false);
      //insere bloco com comando New LineCode
      MontaBlocoNewLineCode();
		//insere bloco com comando New LoadShape
	  MontaBlocoNewLoadShapeTipica();
      MontaBlocoNewLoadShapeTipicaIP();
      MontaBlocoNewLoadShapePropria();
      //insere bloco com comando Set ctkmodel
      //if (rede_equilibrada) MontaBlocoSet_ctkmodel();
	  //insere bloco com comando New Line
      MontaBlocoNewLine();
      //insere blocos com comando New Transformer
      MontaBlocoNewTransformer();
      //DVK 2014.09.19 insere bloco com comando New Regcontrol (regulador)
	  MontaBlocoNewRegControl();
      //insere bloco comando New Generator
	  MontaBlocoNewGeneratorPV();
	  //insere bloco comando New Generator
	  MontaBlocoNewIndMach012();
	   //insere bloco faltas
	   MontaBlocoNewFault();
      //insere bloco comando Load
      MontaBlocoNewLoad();
	  //insere bloco com comando New Capacitor
	  MontaBlocoNewCapacitor();
      //insere bloco comando New monitor
	  MontaBlocoNewMonitor();    ///hardcoded
      //insere bloco com comando Set voltagebases
	  MontaBlocoSetVoltagebases();
      //insere bloco com comando Solve e demais op��es de resultados
	  MontaBlocoSolveEstabilidade();
	  //monta bloco de plot
	  MontaBlocoPlotMonitors(); //hardcoded
	  //fim
      InsereHeader("fim");
      //salva dados no arquivo
      lines->SaveToFile(arq_txt);
	  }catch(Exception &e)
         {
         return(false);
         }
   return(true);
}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::ExportaRedes(AnsiString arq_txt, strOPCAO_ODSS &opcao)
   {
   try{//destr�i eventual objeto VTMonta criado anteriormente
	  if (monta) {delete monta;}
	  //cria objeto auxiliar na montagem do arquivo
		if (opcao.rede_aneel) monta = NewObjMontaAneel(lines);
		else                  monta = NewObjMonta(lines);
	  //verifica se trafos trif�sicos devem ser configurados em Dy1
	  if (opcao.trafoDY == 1)
		 {//configura todos os Trafos delta/estrela em Dy1
		 AtualizaLigacaoTrafoDy1();
		 }
	  //verifica se Cargas de uma mesma Barra devem ser substituidas por Carga equivalente
	  if (opcao.carga_eqv == 1)
		 {//cria Cargas equivalentes
		 AtualizaCargaEquivalente();
		 }
	  //elimina Redes que n�o podem ser calculadas pelo fluxo
	  //DVK 2016.12.14 comentado
//      EliminaRedesComeErroNoFluxo();
      //cria objeto Net
      if (! IniciaNet()) return(false);
      //IMPORTANTE: adapta��o para OpenDSS:
	  //Inclui faseNT nas Barras conectadas por Trafos monof�sicos em delta aberto ou fechado
      //Isto � feito para que o objeto Line que conecta o Bus do prim�rio seja conectado ao
      //Node 0 (refer�ncia).
      //Os enrolamentos dos prim�rios dos Trafos monof�sicos ser�o conectados
      //ao Node 9, formando uma liga��o estrela isolada
	  AtualizaFasesBarraDosTrafos();
      //DVK 2014.09.24 o mesmo � feito para os reguladores de tens�o
      AtualizaFasesBarraDosReguladores();
      //reinicia lines
      lines->Clear();
		//insere coment�rio indicando modelo de rede: padr�o ANEEL ou rede real
		//if (opcao.rede_aneel) InsereHeader("Modelo de rede: padr�o ANEEL");
		//else                  InsereHeader("Modelo de rede: cadastro GIS");
		InsereHeader(monta->ModeloDeRede());
		//insere coment�rio indicando ligac��o dos trafos DY
	  if (opcao.trafoDY == 1) InsereHeader("Transformadores trif�sicos: ligaca��o Dy1");
      else                    InsereHeader("Transformadores trif�sicos: ligaca��o definida pelo equipamento");
      //insere bloco com comando Clear
      MontaBlocoClear();
	  //insere bloco com comando New Circuit
      if(! MontaBlocoNewCircuit()) return(false);
      //insere bloco com comando New LineCode
      MontaBlocoNewLineCode();
		//insere bloco com comando New LoadShape
	  MontaBlocoNewLoadShapeTipica();
      MontaBlocoNewLoadShapeTipicaIP();
      MontaBlocoNewLoadShapePropria();
      //insere bloco com comando Set ctkmodel
      //if (rede_equilibrada) MontaBlocoSet_ctkmodel();
	  //insere bloco com comando New Line
      MontaBlocoNewLine();
      //insere blocos com comando New Transformer
      MontaBlocoNewTransformer();
      //DVK 2014.09.19 insere bloco com comando New Regcontrol (regulador)
	  MontaBlocoNewRegControl();
	  //insere bloco comando New Generator
	  MontaBlocoNewGeneratorPV();
      //insere bloco comando Load
      MontaBlocoNewLoad();
	  //insere bloco com comando New Capacitor
      MontaBlocoNewCapacitor();
      //insere bloco com comando Set voltagebases
      MontaBlocoSetVoltagebases();
      //insere bloco com comando Solve e demais op��es de resultados
	  MontaBlocoSolve();
      //fim
      InsereHeader("fim");
      //salva dados no arquivo
      lines->SaveToFile(arq_txt);
	  }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::ExportaUmaRede(VTRede *rede, AnsiString arq_txt, strOPCAO_ODSS &opcao)
   {

   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::FiltraLisArranjo(TList *lisORIG, TList *lisDEST, int tipocfg)
   {
   //vari�veis locais
   VTArranjo *arranjo;

   //N�O reinicia lisDEST
   //lisDEST->Clear();
   for (int n = 0; n < lisORIG->Count; n++)
      {
      arranjo = (VTArranjo*)lisORIG->Items[n];
      if (arranjo->TipoCfg == tipocfg)
         {//insere Arranjo em lisDEST
         if (lisDEST->IndexOf(arranjo) < 0)
            {
            lisDEST->Add(arranjo);
            }
         }
      }
   return(lisDEST->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::FiltraLisTrecho(TList *lisORIG, TList *lisDEST, bool tipico)
   {
   //vari�veis locais
   VTTrecho  *trecho;

   //loop p/ todos trechos em lisORIG
   for (int n = 0; n < lisORIG->Count; n++)
      {
      trecho = (VTTrecho*)lisORIG->Items[n];
      if (trecho->ArranjoTipico == tipico)
         {//insere Trecho em lisDEST
         if (lisDEST->IndexOf(trecho) < 0)
            {
            lisDEST->Add(trecho);
            }
         }
      }
   return(lisDEST->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::IniciaBus(odssBUS &bus, VTBarra *barra)
   {
   //prote��o
   if (barra == NULL) return(false);
   //inicia id e fases do bus
   //bus.sinap_id    = barra->Id;
   bus.sinap_id    = barra->No[net]->pai->Id;
   bus.sinap_fases = barra->No[net]->Fases;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::IniciaNet(void)
   {
   //vari�veis locais
   VTOrdena *ordena;
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTZonas  *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));

   try{//destr�i eventual objeto Net criado anteriormente
      if (net) {delete net; net = NULL;}
      //inicia Zonas
      if (! zonas->Executa()) return(false);
      //cria e configura objeto Ordena
      if ((ordena = DLL_NewObjOrdena(apl)) == NULL) return(false);
      ordena->ChaveNA_Enabled = false;
      ordena->ChaveNF_Enabled = false;
      ordena->TramoEnabled    = false;
      ordena->ReguladorBypass = false;
      //cria objeto Net
      if ((net = ordena->Executa(redes)) == NULL) return(false);
      }catch(Exception &e)
         {
         Erro("Erro na montagem da Net");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TExportaODSS::InsereHeader(AnsiString header)
   {
	lines->Add(AnsiString("//") + AnsiString::StringOfChar('-', 80));
   lines->Add(AnsiString("//") + header);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::LisArranjoTipico(TList *lisDEST)
   {
   //vari�veis locais
   VTTrecho *trecho;
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisDEST com todos Arranjos t�picos usados pelo Trechos
   lisDEST->Clear();
   //obt�m lista de Trechos
   lisLIG->Clear();
   redes->LisEqpto(lisLIG, eqptoTRECHO);
   //insere em lisDEST os Arranjos t�picos usados pelos Trechos
   for (int n = 0; n < lisLIG->Count; n++)
      {
      trecho = (VTTrecho*)lisLIG->Items[n];
      if (trecho->ArranjoTipico)
         {//insere Arranjo em lisDEST
         if (lisDEST->IndexOf(trecho->arranjo) < 0)
            {
            lisDEST->Add(trecho->arranjo);
            }
         }
      }
   return(lisDEST->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::LisCargaCurvaPropria(TList *lisDEST)
   {
   //vari�veis locais
   VTCarga *carga;

   //obt�m lista de Cargas
   if (LisEqbar(lisDEST, eqptoCARGA) > 0)
      {//elimina Cargas com curva t�pica
      for (int n = lisDEST->Count-1; n >= 0; n--)
         {
         carga = (VTCarga*)lisDEST->Items[n];
         if (carga->curva->Tipica)
            {//retira Carga da lista
            lisDEST->Delete(n);
            }
         }
      }
   return(lisDEST->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::LisEqbar(TList *lisDEST, int tipo_eqpto)
   {
   //N�O reinicia lisDEST
   //lisDEST->Clear();
   net->LisEqbar(lisDEST, tipo_eqpto);
   return(lisDEST->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::LisGerador(int tiposup)
   {
   //vari�veis locais
   VTGerador *gerador;

   //reinicia lisEQP com todos Geradores
   lisEQP->Clear();
   LisEqbar(lisEQP, eqptoGERADOR);
   //filtra os Geradores do tipo indicado
   for (int n = lisEQP->Count - 1; n >= 0; n--)
      {
      gerador = (VTGerador*)lisEQP->Items[n];
      if (gerador->tiposup != tiposup)
         {//retira gerador da lista
         lisEQP->Delete(n);
         }
      }
   return(lisEQP->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::LisLigacao(TList *lisDEST, int tipo_eqpto)
   {
   //N�O reinicia lisDEST
   //lisDEST->Clear();
   net->LisLigacao(lisDEST, tipo_eqpto);
   return(lisDEST->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::LisSuprimento(int tiposup)
   {
   //vari�veis locais
   VTSuprimento *suprimento;

   //reinicia lisEQP com todos Geradores
   lisEQP->Clear();
   LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //filtra os Suprimento do tipo indicado
   for (int n = lisEQP->Count - 1; n >= 0; n--)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[n];
      if (suprimento->tiposup != tiposup)
         {//retira Suprimento da lista
         lisEQP->Delete(n);
         }
      }
   return(lisEQP->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoClear(void)
   {
   InsereHeader("Reinicia o programa");
   lines->Add("Clear");

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewCapacitor(void)
   {
   //vari�veis locais
   double        q_kvar;
   odssCAPACITOR odss;
   VTCapacitor   *capacitor;
   VTDemanda     *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //inicia lista de Capacitores
   lisEQP->Clear();
   if (LisEqbar(lisEQP, eqptoCAPACITOR) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de capacitores");
   //loop p/ todos Capacitore
   for (int n = 0; n < lisEQP->Count; n++)
      {
      capacitor          = (VTCapacitor*)lisEQP->Items[n];
      odss.sinap_id      = capacitor->Id;
      odss.sinap_ligacao = capacitor->ligacao;
		odss.codigo        = CodigoOdssEqpto(capacitor);
      IniciaBus(odss.bus1, capacitor->pbarra);
      IniciaBus(odss.bus2, capacitor->pbarra);
		odss.phases = 3;
      odss.q_kvar = capacitor->Q_mvar * 1000.;
      odss.vff_kv = capacitor->pbarra->zona->Vff_kv;
      //define o tipo de liga��o
      switch(capacitor->ligacao)
         {
         case lgTRIANG:  odss.conn = "delta"; break;
         case lgEST_ISO: odss.conn = "wye";   break;
         case lgEST_AT:  odss.conn = "wye";   break;
         default:        odss.conn = "wye";   break;
         }
      if (! monta->NewCapacitor(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewCircuit(void)
   {
   //vari�veis locais
   AnsiString      texto;
   int             ind_pat   = 0;
   odssCIRCUIT     circuit;
   VTSuprimento    *suprimento;
   VTPath          *path         = (VTPath*)apl->GetObject(__classid(VTPath));
   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //inicia lista de Suprimentos
   lisEQP->Clear();
   LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //filtra os Suprimento do tipo V/T
   for (int n = lisEQP->Count - 1; n >= 0; n--)
	  {
	  suprimento = (VTSuprimento*)lisEQP->Items[n];
	  if (suprimento->tiposup != supVF)
		 {//retira Suprimento da lista
         lisEQP->Delete(n);
         }
      }
   //prote��o
   if (lisEQP->Count == 0)
      {
		Aviso("Rede sem suprimento");
      return(false);
      }
   //prote��o: por enquanto, insere apenas 1 Suprimento
   if (lisEQP->Count > 1)
      {
		Aviso(texto.sprintf("Rede com %d barras swing (suprimentos 'VF').\nO que fazer nesse caso?\nPor hora, aborta.", lisEQP->Count));
	  return(false);
      }
   //determina Suprimento
	suprimento = (VTSuprimento*)lisEQP->First();
   //prote��o: verifica pot�ncia de curto nula
   if (IsDoubleZero(suprimento->pcc_3f.p)) suprimento->pcc_3f.p = suprimento->pcc_3f.q;
   if (IsDoubleZero(suprimento->pcc_3f.q)) suprimento->pcc_3f.q = suprimento->pcc_3f.p;
   if (IsDoubleZero(suprimento->pcc_3f.p) || IsDoubleZero(suprimento->pcc_3f.q))
      {
      Aviso(texto.sprintf("Pot�ncia de curto trif�sico com valor nulo"));
      return(false);
	  }
   //inicia odssCIRCUIT com os dados do Suprimento
   IniciaBus(circuit.bus1, suprimento->pbarra);
   circuit.codigo     = Trim(ChangeFileExt(ExtractFileName(path->ArqRede), ""));
   circuit.phases     = 3;
   circuit.vbase_kv   = suprimento->vnom;
   circuit.vref_pu    = fornecimento->V_pu[suprimento][ind_pat];
   circuit.pcc3f_mw   = suprimento->pcc_3f.p;
   circuit.qcc3f_mvar = suprimento->pcc_3f.q;
   circuit.pcc1f_mw   = suprimento->pcc_ft.p;
   circuit.qcc1f_mvar = suprimento->pcc_ft.q;
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de fronteira");
   //insere comando New Circuit
   return(monta->NewCircuit(circuit));
	}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewIndMach012(void)
{
	VTEqptos      *eqptos = (VTEqptos*)apl->GetObject(__classid(VTEqptos));
	VTMaq         *maq;
	TList		  *lisMaq;
	odssINDMACH012    indMach012;
	bool	sucesso = false;

	if(eqptos ==NULL)
		{return false;}
	try
	{
		lisMaq = eqptos->LisMaq();
		//prote��o
		if (lisMaq->Count == 0) return(true);
		//insere coment�rio identificando o bloco
		InsereHeader("Bloco com dados das maquinas de inducao");
		//loop p/ todos geradores
		for (int n = 0; n < lisMaq->Count; n++)
		{
			maq = (VTMaq*)lisMaq->Items[n];
			if (maq->Gerador == NULL)
				{continue;}
			//obt�m dados do Gerador
			IniciaBus(indMach012.bus1, maq->Gerador->pbarra);
			indMach012.codigo  = CodigoOdssEqpto(maq->Gerador);
			indMach012.sinap_ligacao = maq->Gerador->ligacao;
			indMach012.vnom_kv = maq->Gerador->vnom;
			indMach012.s_kva = maq->Gerador->snom;
			if (!IsDoubleZero(maq->cosfi))
			{
				indMach012.p_kw = maq->Gerador->snom*maq->cosfi;
			}
			else
			{
				indMach012.p_kw = maq->Gerador->snom*0.92;
            }
			indMach012.h_minercia = maq->hrot;
			//valores default
			indMach012.slip = 0.02;
			indMach012.rs_pu = 0.048;
			indMach012.xs_pu = 0.075;
			indMach012.rr_pu = 0.018;
			indMach012.xr_pu = 0.12;
			indMach012.xm_pu = 3.8;
			//insere comando New IndMach012
			monta->NewIndMach012(indMach012);
		}
	   sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewFault(void)
{
	VTEventos *eventos = (VTEventos*)apl->GetObject(__classid(VTEventos));
	VTEvento  *evento;
	TList	  *lisEventos;
	odssFAULT   fault;
	bool	sucesso = false;
	VTBarra *barra;

	if(eventos ==NULL)
		{return false;}
	try
	{
		lisEventos = eventos->LisEvento();
		//prote��o
		if (lisEventos->Count == 0) return(true);
		//insere coment�rio identificando o bloco
		InsereHeader("Bloco com dados das faltas");
		//loop p/ todos geradores
		for (int n = 0; n < lisEventos->Count; n++)
		{
			evento = (VTEvento*)lisEventos->Items[n];
			if (evento == NULL)
				{continue;}
//			if (evento->Tipo != evtCURTO_FT)
//				{continue;}

			barra = (VTBarra*)evento->Eqpto;
			if (barra == NULL)
				{continue;}
			//obt�m dados do Gerador
			IniciaBus(fault.bus1,barra);
			IniciaBus(fault.bus2,barra);
			fault.codigo  = "FaltaFTfaseA";
			fault.r = 0.0001;
			fault.phases = 1;
			fault.onTime = (double)eventos->InstanteInicialEvento(evento)/1000;
			//insere comando New IndMach012
			monta->NewFault(fault);
		}
	   sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewGeneratorPV(void)
   {
   //vari�veis locais
   int            ind_pat   = 0;
   odssGENERATOR  generator;
   VTGerador      *gerador;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   //VTMaq         *maq;
   VTEqptos      *eqptos = (VTEqptos*)apl->GetObject(__classid(VTEqptos));

   //inicia lista de Geradores
   lisEQP->Clear();
   if (LisEqbar(lisEQP, eqptoGERADOR) == 0) return(true);
   //mant�m na lista somente Geradores P/V
   for (int n = lisEQP->Count - 1; n >= 0; n--)
      {
      gerador = (VTGerador*)lisEQP->Items[n];
      if (gerador->tiposup != supPV)
         {//retira gerador da lista
         lisEQP->Delete(n);
         }
      }
   //prote��o
   if (lisEQP->Count == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de gerador P/V");
   //loop p/ todos geradores
   for (int n = 0; n < lisEQP->Count; n++)
      {
	  gerador = (VTGerador*)lisEQP->Items[n];
	  //verifica se possui equivalente do Estabilidade
	  if (eqptos)
	  {     //caso possua uma maquina esse gerador nao entra
			if(eqptos->ExisteMaq(gerador) != NULL)
				{continue;}
	  }
      //obt�m dados do Gerador
      IniciaBus(generator.bus1, gerador->pbarra);
      generator.codigo  = CodigoOdssEqpto(gerador);
      generator.phases  = 3;
      generator.model   = gerP_V;
      generator.vnom_kv = gerador->vnom;
      generator.vref_pu = fornecimento->V_pu[gerador][ind_pat];
      generator.p_kw    = fornecimento->P_mw[gerador][ind_pat] * 1000.;
      generator.pvfactor = 0.5;
      //verifica se h� limite de fator de pot�ncia
      if (IsDoubleZero(gerador->fplim))
         {//assum valor default
         generator.qmax_kvar =  0.60 * generator.p_kw;
         generator.qmin_kvar = -0.45 * generator.p_kw;
         }
      else
         {
         generator.qmax_kvar =  generator.p_kw * Tan(ArcCos(gerador->fplim));
         generator.qmin_kvar = -generator.qmax_kvar;
         }
      //insere comando New Generator
      if (! monta->NewGeneratorPV(generator)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLine(void)
   {
   //inicia lista de Trechos
   lisLIG->Clear();
   if (LisLigacao(lisLIG, eqptoTRECHO) == 0) return(true);
   if (! MontaBlocoNewLineArranjoProprio()) return(false);
   if (! MontaBlocoNewLineArranjoTipico())  return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLineArranjoProprio(void)
   {
   //vari�veis locais
   bool       tipico;
   odssLINE   line;
   strIMP     z0_ohm_km, z1_ohm_km;
   double     c0_nf_km,  c1_nf_km;
   VTTrecho   *trecho;

   //filtra Trechos com arranjo pr�prio (que est�o em lisLIG)
   lisEQP->Clear();
   if (FiltraLisTrecho(lisLIG, lisEQP, tipico=false) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de trechos com arranjo pr�prio");
   //loop p/ todos trechos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      if (trecho->ArranjoTipico) continue;
      //obt�m dados do Trecho
      trecho->arranjo->Z0_ohm(1.0, z0_ohm_km, c0_nf_km);
      trecho->arranjo->Z1_ohm(1.0, z1_ohm_km, c1_nf_km);
      //inicia dados do Trecho
      IniciaBus(line.bus1, trecho->pbarra1);
      IniciaBus(line.bus2, trecho->pbarra2);
      line.codigo      = CodigoOdssEqpto(trecho);
      line.sinap_fases = trecho->arranjo->Fases;
      line.phases      = NumeroDeFases(trecho->arranjo);
      line.comp_km     = trecho->Comprimento_km;
      //prote��o
      if (line.comp_km < comp_min_km) line.comp_km = comp_min_km;
      //cancela linecode
      line.linecode  = "";
		//define imped�ncias
		line.r1_ohm_km = z1_ohm_km.r;
		line.x1_ohm_km = z1_ohm_km.x;
		line.c1_nF_km  = c1_nf_km;
		line.r0_ohm_km = z0_ohm_km.r;
		line.x0_ohm_km = z0_ohm_km.x;
		line.c0_nF_km  = c0_nf_km;
		//insere comando New Line
      if (! monta->NewLine_SemLineCode(line)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLineArranjoTipico(void)
   {
   //vari�veis locais
   bool       tipico;
   odssLINE   line;
   strIMP     z0_ohm_km, z1_ohm_km;
   double     c0_nf_km,  c1_nf_km;
   VTTrecho   *trecho;

   //filtra Trechos com arranjo pr�prio (que est�o em lisLIG)
   lisEQP->Clear();
   if (FiltraLisTrecho(lisLIG, lisEQP, tipico=true) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de trechos com arranjo t�pico");

//Aviso("MontaBlocoNewLineArranjoTipico(): trechos alterados p/ m�nimo de 50 m");

   //loop p/ todos trechos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      if (! trecho->ArranjoTipico) continue;
      //obt�m dados do Trecho
      trecho->arranjo->Z0_ohm(1.0, z0_ohm_km, c0_nf_km);
      trecho->arranjo->Z1_ohm(1.0, z1_ohm_km, c1_nf_km);
      //inicia dados do Trecho
      IniciaBus(line.bus1, trecho->pbarra1);
      IniciaBus(line.bus2, trecho->pbarra2);
      line.codigo      = CodigoOdssEqpto(trecho);
      line.sinap_fases = trecho->arranjo->Fases;
      line.phases      = NumeroDeFases(trecho->arranjo);
      line.comp_km     = trecho->Comprimento_km;
      //prote��o
		if (line.comp_km < comp_min_km) line.comp_km = comp_min_km;
		//define line code
      line.linecode    = IntToStr(trecho->arranjo->Id);
		//cancela dados de imped�ncia
		line.r1_ohm_km = 0.;
		line.x1_ohm_km = 0.;
		line.c1_nF_km  = 0.;
		line.r0_ohm_km = 0.;
		line.x0_ohm_km = 0.;
		line.c0_nF_km  = 0.;
		//insere comando New Line
      if (! monta->NewLine_ComLineCode(line)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLineCode(void)
   {
   //determina conjunto de Arranjos t�picos usados na rede
   lisARR->Clear();
   if (LisArranjoTipico(lisARR) == 0) return(true);
   //monta bloco com arranjos definidos por Z0/Z1
   if (! MontaBlocoNewLineCodeZ0Z1()) return(false);
   //monta bloco com arranjos definidos por suporte/cabo/fase
   if (! MontaBlocoNewLineCodeMatZ()) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLineCodeMatZ(void)
   {
   //vari�veis locais
   int          num_fases;
   strIMP       z0_ohm_km, z1_ohm_km;
   double       c0_nf_km,  c1_nf_km;
   double       dummy;
   odssLINECODE linecode;
   VTArranjo    *arranjo;
   VTMatC       *matC;
   VTMatZ       *matZ;

   //filtra Arranjos definidos por Cabo por Fase (que est�o em lisARR)
   lisEQP->Clear();
   if (FiltraLisArranjo(lisARR, lisEQP, arranjoCABOxFASE) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de arranjos Cabo/Fase");
   //loop p/ todos Arranjos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      if (arranjo->TipoCfg != arranjoCABOxFASE) continue;
      arranjo->Z0_ohm(1.0, z0_ohm_km, dummy);
      arranjo->Z1_ohm(1.0, z1_ohm_km, dummy);
      //c�digo tempor�rio: n�o considera capacit�ncia
      c0_nf_km = c1_nf_km = 0.;
      matC = arranjo->MatC;
      matZ = arranjo->MatZ;
      //inicia dados do Arranjo
      linecode.id        = arranjo->Id;
      linecode.codigo    = CodigoOdssEqpto(arranjo);
      linecode.units     = "km";
      linecode.basefreq  = 60;
      linecode.normamps  = arranjo->Iadm_amp;
      //inicia dados do arranjo Z0/Z1
      linecode.r1_ohm_km = z1_ohm_km.r;
      linecode.x1_ohm_km = z1_ohm_km.x;
      linecode.c1_nF_km  = c1_nf_km;
      linecode.r0_ohm_km = z0_ohm_km.r;
      linecode.x0_ohm_km = z0_ohm_km.x;
      linecode.c0_nF_km  = c0_nf_km;
      //determina n�mero de fases
      linecode.nphases = NumeroDeFases(arranjo);
      //desabilita redu��o de kron
      linecode.kron = false;
      //preenche valores das matrizes
      for (int i = 0; (i < linecode.nphases)&&(i < MAX_NODE); i++)
         {
         for (int j = 0; (j < linecode.nphases)&&(j < MAX_NODE); j++)
            {
            linecode.cmatrix[i][j] = matC->Cap[i][j];
            linecode.rmatrix[i][j] = matZ->R[i][j];
            linecode.xmatrix[i][j] = matZ->X[i][j];
            }
         }
      //insere comando New LineCode
      if (! monta->NewLineCode_MatZ(linecode)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLineCodeZ0Z1(void)
   {
   //vari�veis locais
   bool         first = true;
   strIMP       z0_ohm_km, z1_ohm_km;
   double       c0_nf_km,  c1_nf_km;
   odssLINECODE linecode;
   VTArranjo    *arranjo;

   //filtra Arranjos definidos por Z0Z1 e por Cabo Z0Z1 (que est�o em lisARR)
   lisEQP->Clear();
   FiltraLisArranjo(lisARR, lisEQP, arranjoZ0Z1);
   if (FiltraLisArranjo(lisARR, lisEQP, arranjoCABO_Z0Z1) == 0) return(true);
   //loop p/ todos Arranjos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      if (arranjo->TipoCfg == arranjoZ0Z1)
         {//obt�m imped�ncias do Arranjo
         z0_ohm_km = arranjo->z0;
         z1_ohm_km = arranjo->z1;
         c0_nf_km  = arranjo->c0;
         c1_nf_km  = arranjo->c1;
         }
      else if (arranjo->TipoCfg == arranjoCABO_Z0Z1)
         {//obt�m imped�ncias do CaboZ0Z1
         z0_ohm_km = arranjo->caboZ0Z1->z0;
         z1_ohm_km = arranjo->caboZ0Z1->z1;
         c0_nf_km  = arranjo->caboZ0Z1->c0;
         c1_nf_km  = arranjo->caboZ0Z1->c1;
         }
      else
         {continue;}
      //inicia dados do Arranjo
      linecode.id        = arranjo->Id;
      linecode.codigo    = CodigoOdssEqpto(arranjo);
      linecode.units     = "km";
      linecode.basefreq  = 60;
      linecode.normamps  = arranjo->Iadm_amp;
      //dados do arranjo Z0/Z1
      linecode.r1_ohm_km = z1_ohm_km.r;
      linecode.x1_ohm_km = z1_ohm_km.x;
      linecode.c1_nF_km  = c1_nf_km;
      linecode.r0_ohm_km = z0_ohm_km.r;
      linecode.x0_ohm_km = z0_ohm_km.x;
      linecode.c0_nF_km  = c0_nf_km;
      //cancela dados de arranjo definido por matriz
      linecode.nphases   = 3;
      linecode.kron      = false;
      linecode.neutral   = -1;
      for (int i = 0; i < MAX_NODE; i++)
         {
         for (int j = 0; j < MAX_NODE; j++)
            {
            linecode.cmatrix[i][j] = 0;
            linecode.rmatrix[i][j] = 0;
            linecode.xmatrix[i][j] = 0;
            }
         }
      //verifica se � o primeiro Arranjo sendo inserido
      if (first)
         {//insere coment�rio identificando o bloco
         InsereHeader("Bloco com dados de arranjos Z0/Z1");
         first = false;
         }
      //insere comando New LineCode
		if (! monta->NewLineCode_Z0Z1(linecode)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLoad(void)
   {
	//vari�veis locais
	AnsiString txt;
	double     s_mva, p_mw, q_mvar;
	odssLOAD   load;
	VTCarga    *carga;
	VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //inicia lista de Cargas
   lisEQP->Clear();
   if (LisEqbar(lisEQP, eqptoCARGA) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de cargas");
   //loop p/ todos Cargas
   for (int n = 0; n < lisEQP->Count; n++)
      {
		carga = (VTCarga*)lisEQP->Items[n];
		//verifica se a Carga tem neutro e est� em uma zona monof�sica
      //if ((carga->Fases & faseN)&&(carga->pbarra->zona->Tipo == zonaMONOFASICA))
      //verifica se a Carga tem neutro e n�o est� em uma zona trif�sica
      if ((carga->Fases & faseN)&&(carga->pbarra->zona->Tipo != zonaTRIFASICA))
         {//divide a Carga em N Cargas fase/neutro
         MontaBlocoNewLoadMonofasica(carga);
         }
      else
         {//insere uma �nica Carga
         IniciaBus(load.bus1, carga->pbarra);
         load.codigo        = CodigoOdssEqpto(carga);
         load.sinap_numfase = NumeroDeFases(carga);
         load.sinap_fases   = carga->Fases;
         load.sinap_zona    = carga->pbarra->zona->Tipo;
         load.model         = TraduzModeloCarga(carga->ModeloCarga);
         load.vff_kv        = carga->pbarra->zona->Vff_kv;
         //define tens�o de fase
         if      (carga->Fases & faseA) load.vfn_kv = carga->pbarra->zona->Vfn_kv[indFASE_A];
         else if (carga->Fases & faseB) load.vfn_kv = carga->pbarra->zona->Vfn_kv[indFASE_B];
         else if (carga->Fases & faseC) load.vfn_kv = carga->pbarra->zona->Vfn_kv[indFASE_C];
         else                           load.vfn_kv = carga->pbarra->zona->Vfn_kv[-1];
         //define tens�o m�nima em pu
         load.vmin_pu = 0.8;
         //verifica se � uma Carga de IP
         if (carga->IP)
            {//determina demanda p/ um ponto n�o nulo
            if (! demanda->GetSmax_mva(carga, s_mva, p_mw, q_mvar)) return(false);
            }
         else
            {
            if (! demanda->GetSmedia_mva(carga, s_mva, p_mw, q_mvar)) return(false);
            }
         //n�o inclui Carga com demanda m�dia nula
         if (IsDoubleZero(s_mva)) continue;
         load.p_kw     = p_mw   * 1000.;
         load.q_kvar   = q_mvar * 1000.;
         //define loadshape
         load.loadshape_id = carga->curva->Id;
			//if (! monta->NewLoad(load)) return(false);
			if (! monta->NewLoad(load))
				{
				Alerta(txt.sprintf("Interface OpenDSS: Carga n�o inclu�da (ID= %d)", carga->Id));
				}
			}
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLoadMonofasica(VTCarga *carga)
   {
   //vari�veis locais
   int       num_fase;
   double    s_mva, p_mw, q_mvar;
   odssLOAD  load;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   int       vet_fase[] = {faseA, faseB, faseC};

   //verifica se � uma Carga de IP
   if (carga->IP)
      {//determina demanda p/ um ponto n�o nulo
      if (! demanda->GetSmax_mva(carga, s_mva, p_mw, q_mvar)) return(false);
      }
   else
      {
      if (! demanda->GetSmedia_mva(carga, s_mva, p_mw, q_mvar)) return(false);
      }
   //n�o inclui Carga com demanda m�dia nula
   if (IsDoubleZero(s_mva)) return(true);
   num_fase      = fases->NumeroDeFases(carga->Fases);
   //divide a demanda m�dia Carga em N Cargas fase/neutro
   load.p_kw     = (p_mw   * 1000.) / num_fase;
   load.q_kvar   = (q_mvar * 1000.) / num_fase;
   //loop p/ todas fases
   for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//verifica se a Carga tem a fase
      if ((carga->Fases & vet_fase[ind_fase]) == 0) continue;
      //insere uma �nica Carga
      IniciaBus(load.bus1, carga->pbarra);
      load.codigo        = CodigoOdssEqpto(carga) + fases->AsStrABCN[vet_fase[ind_fase]];
      load.sinap_numfase = 1;
      load.sinap_fases   = vet_fase[ind_fase] | faseN;
      load.sinap_zona    = carga->pbarra->zona->Tipo;
      load.model         = TraduzModeloCarga(carga->ModeloCarga);
      load.vff_kv        = carga->pbarra->zona->Vff_kv;
      load.vfn_kv        = carga->pbarra->zona->Vfn_kv[ind_fase];
      //define tens�o m�nima em pu
      load.vmin_pu       = 0.8;
      //define loadshape
      load.loadshape_id = carga->curva->Id;
      if (! monta->NewLoad(load)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLoadShapePropria(void)
   {
   //vari�veis locais
   double        p_mw, q_mvar;
   double        pmed_mw, qmed_mvar, smed_mva;
   odssLOADSHAPE odss;
   VTCarga       *carga;
   VTDemanda     *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //obt�m lista de Carga com curvas pr�prias de demanda
   lisEQP->Clear();
   if (LisCargaCurvaPropria(lisEQP) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de curvas pr�prias de demanda");
   //loop p/ todas Curvas
   for (int n = 0; n < lisEQP->Count; n++)
      {
      carga     = (VTCarga*)lisEQP->Items[n];
      //determina demanda m�dia da carga
      if (! demanda->GetSmedia_mva(carga, smed_mva, pmed_mw, qmed_mvar)) return(false);
      //prote��o
      if (pmed_mw   == 0) pmed_mw   = 1.0;
      if (qmed_mvar == 0) qmed_mvar = 1.0;
      //preenche estrutura odssLOADSHAPE
      odss.id       = carga->curva->Id;
      odss.codigo   = carga->Codigo;
      odss.npts     = 24;
      //habilita valores de p e q
      odss.curva_p.enabled = true;
      odss.curva_q.enabled = true;
      //loop para montar os pontos da curva
      for (int np = 0; np < odss.npts; np++)
         {//obt�m demanda da Carga na hora
         p_mw   = demanda->P_mw[carga][np];
         q_mvar = demanda->Q_mvar[carga][np];
         //salva pontos das duas curvas
         odss.curva_p.valor[np] = p_mw   / pmed_mw;
         odss.curva_q.valor[np] = q_mvar / qmed_mvar;
         }
      //insere comando New LoadShape
      if (! monta->NewLoadShape_pq(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLoadShapeTipica(void)
   {
   //vari�veis locais
   double        vet_valor[32];
   int           vet_size = sizeof(vet_valor)/sizeof(double);
   strHM         hm_ini, hm_fim;
   odssLOADSHAPE odss;
   VTCurva       *curva;
   VTCurvas      *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //obt�m lista de Curvas t�picas de demanda
   lisEQP->Clear();
   if ((curvas->LisCurvaTipicaCarga(NULL, SEMANA_COMPLETA, lisEQP)) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de curvas t�picas de demanda");
   //loop p/ todas Curvas
   for (int n = 0; n < lisEQP->Count; n++)
      {
      curva     = (VTCurva*)lisEQP->Items[n];
      //determina n�mero de pontos da curva e verifica se maior que o limite da estrutrua
      odss.id       = curva->Id;
      odss.codigo   = curva->Codigo;
      odss.npts     = curva->NumeroPontos();
      //prote��o: verifica se n�mero de pontos da curva � maior que o limite da estrutura
      if (odss.npts > sizeof(odss.curva_p.valor)/sizeof(double)) continue;
      //verifica se Curva t�pica com valor em pu da demanda m�xima
      if (curva->Unidade == unidPU_MAX)
         {//determina Curva com valores em pu da m�dia
         curva = curva->CurvaSiamesaPUmedia;
         }
      //habilita apenas a curva de potencia ativa
      odss.curva_p.enabled = true;
      odss.curva_q.enabled = false;
      //loop para todos pontos da Curva
      for (int np = 0; np < odss.npts; np++)
         {//obt�m valores do ponto
         if (! curva->GetPonto(np, hm_ini, hm_fim, vet_valor, vet_size)) return(false);
         //DVK 2016.12.12 verifica se a curva est� em pu
         if (curva->Unidade == unidMVA)
            {//converte para pu da m�dia
            vet_valor[0] /= curva->Media(0);
            }
         //salva apenas o primeiro valor do ponto
         odss.curva_p.valor[np] = vet_valor[0];
         }
      //insere comando New LoadShape
      if (! monta->NewLoadShape(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewLoadShapeTipicaIP(void)
   {
   //vari�veis locais
   double        vet_valor[32];
   int           vet_size = sizeof(vet_valor)/sizeof(double);
   strHM         hm_ini, hm_fim;
   odssLOADSHAPE odss;
   VTCurva       *curva;
   VTCurvas      *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //obt�m lista de Curvas t�picas de IP
   lisEQP->Clear();
   CopiaTList(curvas->ListaCurvaTipicaIP(), lisEQP);
   if (lisEQP->Count == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de curvas t�picas de IP");
   //loop p/ todas Curvas
   for (int n = 0; n < lisEQP->Count; n++)
      {
      curva     = (VTCurva*)lisEQP->Items[n];
      //determina n�mero de pontos da curva e verifica se maior que o limite da estrutrua
      odss.id       = curva->Id;
      odss.codigo   = curva->Codigo;
      odss.npts     = 24;
      //habilita apenas a curva de potencia ativa
      odss.curva_p.enabled = true;
      odss.curva_q.enabled = false;
      //loop para todos pontos da Curva
      for (int np = 0; np < curva->NumeroPontos(); np++)
         {//obt�m valores do ponto
         if (! curva->GetPonto(np, hm_ini, hm_fim, vet_valor, vet_size)) return(false);
         if (hm_ini.hora < hm_fim.hora)
            {
            for (int hora = hm_ini.hora; hora < hm_fim.hora; hora++)
               {//salva apenas o primeiro valor do ponto
               odss.curva_p.valor[hora] = vet_valor[0];
               }
            }
         else
            {
            for (int hora = hm_ini.hora; hora < 24; hora++)
               {//salva apenas o primeiro valor do ponto
               odss.curva_p.valor[hora] = vet_valor[0];
               }
            for (int hora = 0; hora < hm_fim.hora; hora++)
               {//salva apenas o primeiro valor do ponto
               odss.curva_p.valor[hora] = vet_valor[0];
               }
            }
         }
	  //insere comando New LoadShape
	  if (! monta->NewLoadShape(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewMonitor(void)
{
    VTEqptos      *eqptos = (VTEqptos*)apl->GetObject(__classid(VTEqptos));
	VTMaq         *maq;
	TList		  *lisMaq;
	odssINDMACH012    indMach012;
	bool	sucesso = false;
	AnsiString nomeElement;
	odssMONITOR monitor;

	monitores->Clear();
	try
	{
		//criar um monitor pro suprimento

		//criar um pra falta
		monitor.codigo = "FaltaV";
		monitor.element = "Fault.FaltaFTfaseA";
		monitor.terminal = 1;
		monitor.mode = 0;
		monitor.ppolar = true;
		//insere comando New LoadShape
		monta->NewMonitor(monitor);

		monitor.codigo = "FaltaP";
		monitor.element = "Fault.FaltaFTfaseA";
		monitor.terminal = 1;
		monitor.mode = 1;
		monitor.ppolar = false;
		//insere comando New LoadShape
		monta->NewMonitor(monitor);

		//criar pras maquinas
		if(eqptos !=NULL)
		{
			lisMaq = eqptos->LisMaq();
			//loop p/ todos geradores
			for (int n = 0; n < lisMaq->Count; n++)
			{
				maq = (VTMaq*)lisMaq->Items[n];
				if (maq->Gerador == NULL)
					{continue;}
				//obt�m dados do Gerador
				nomeElement = NomeElementMonitor(maq->Gerador);
				monitor.codigo = "Monitor_R" + maq->Gerador->Codigo;
				monitor.element = nomeElement;
				monitor.terminal = 1;
				monitor.mode = 3;
				monitor.ppolar = true;
				monitores->Add(monitor.codigo);
				//insere comando New LoadShape
				monta->NewMonitor(monitor);

				monitor.codigo = "Monitor_Power1" + maq->Gerador->Codigo;
				monitor.element = nomeElement;
				monitor.terminal = 1;
				monitor.mode = 1;
				monitor.ppolar = false;
				monitores->Add(monitor.codigo);
				//insere comando New LoadShape
				monta->NewMonitor(monitor);

//				monitor.codigo = "Monitor_Power2" + maq->Gerador->Codigo;
//				monitor.element = nomeElement;
//				monitor.terminal = 2;
//				monitor.mode = 1;
//				monitor.ppolar = false;
//				monitores->Add(monitor.codigo);
//				//insere comando New LoadShape
//				monta->NewMonitor(monitor);
			}
			sucesso = true;
		}


	}
	catch (Exception &e)
	{
		sucesso = false;
	}

    return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewRegControl(void)
   {
   //vari�veis locais
   VTRegulador    *regulador;

   //inicia lista de todos Reguladores
   lisLIG->Clear();
   if (LisLigacao(lisLIG, eqptoREGULADOR) == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de reguladores");
   for (int n = 0; n < lisLIG->Count; n++)
      {
      regulador = (VTRegulador*)lisLIG->Items[n];
      //verifica o tipo de liga��o
      switch(regulador->ligacao)
         {
         case lgMONOFASICO:   // 1 unidade monof�sica
            MontaUM_Regulador(regulador, regulador->fases);
            break;
         case lgDELTA_ABERTO: //2 unidades monof�sicas
            switch(regulador->fases & faseABC)
               {
               case faseA:
                  MontaUM_Regulador(regulador, faseAB);
                  MontaUM_Regulador(regulador, faseCA);
                  break;
               case faseB:
                  MontaUM_Regulador(regulador, faseAB);
                  MontaUM_Regulador(regulador, faseBC);
                  break;
               case faseC:
                  MontaUM_Regulador(regulador, faseBC);
                  MontaUM_Regulador(regulador, faseCA);
                  break;
               default:
                  return(false);
               }
            break;
         case lgDELTA_FECHADO: //3 unidades monof�sicas
            MontaUM_Regulador(regulador, faseAB);
            MontaUM_Regulador(regulador, faseBC);
            MontaUM_Regulador(regulador, faseCA);
            break;
         case lgEST_AT: //3 unidades monof�sicas
            MontaUM_Regulador(regulador, faseAN);
            MontaUM_Regulador(regulador, faseBN);
            MontaUM_Regulador(regulador, faseCN);
            break;
         case lgEST_ISO:
            MontaUM_Regulador(regulador, faseA);
            MontaUM_Regulador(regulador, faseB);
            MontaUM_Regulador(regulador, faseC);
            break;
         default:
            return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewTransformer(void)
   {
   //vari�veis locais
   VTTrafo *trafo;

   //inicia lista de todos Trafos
   lisLIG->Clear();
   if (LisLigacao(lisLIG, eqptoTRAFO) == 0) return(true);
   //inicia lisEQP com Trafos trif�sicos
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      trafo = (VTTrafo*)lisLIG->Items[n];
      if (! trafo->Monofasico) lisEQP->Add(trafo);
      }
   //monta bloco com Trafos trif�sicos
   if (! MontaBlocoNewTransformer_3F2E()) return(false);
   //inicia lisEQP com Trafos monof�sicos com fase/neutro ou 2 fases sem neutro
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      trafo = (VTTrafo*)lisLIG->Items[n];
      if (! trafo->Monofasico) continue;
      switch(fases->NumeroDeFases(trafo->sec.fase))
         {
         case 1:
            lisEQP->Add(trafo);
            break;
         case 2:
            if ((trafo->sec.fase & faseN) == 0) lisEQP->Add(trafo);
            break;
         }
      }
   //monta bloco com Trafos monof�sicos de 2 enrolamentos
   if (! MontaBlocoNewTransformer_Monofasico2E()) return(false);
   //monta lisEQP com Trafos monof�sicos com 2 fases e neutro no secund�rio
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      trafo = (VTTrafo*)lisLIG->Items[n];
      if (! trafo->Monofasico) continue;
      if ((fases->NumeroDeFases(trafo->sec.fase) == 2) && (trafo->sec.fase & faseN))
         {
         lisEQP->Add(trafo);
         }
      }
   //monta bloco com Trafos monof�sicos de 3 enrolamentos
   if (! MontaBlocoNewTransformer_Monofasico3E()) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewTransformer_Monofasico2E(void)
   {
   //vari�veis locais
   odssTRANSFORMER odss;
   VTTrafo         *trafo;

   //inicia lista de Trafos
   if (lisEQP->Count == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de transformadores monof�sicos, 2 enrolamentos");
   //loop p/ todos Trafos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      //inicia dados do trafo
      odss.codigo      = CodigoOdssEqpto(trafo);
      odss.phases      = NumeroDeFases(trafo);
      odss.xhl         = trafo->z1.x * 100.;
      odss.loadloss    = trafo->z1.r * 100.;
      odss.noloadloss  = trafo->perda_ferro;
      //inicia dados winding 1
      IniciaBus(odss.win1.bus, trafo->BarraPrimario());
      odss.win1.sinap_ligacao = trafo->pri.ligacao;
      odss.win1.sinap_fases   = trafo->pri.fase;
      odss.win1.vnom_kv       = trafo->pri.vnom;
      odss.win1.vtap_pu       = trafo->pri.tap / trafo->pri.vnom;
      odss.win1.snom_kva      = trafo->snom * 1000.;
      odss.win1.r_perc        = odss.loadloss * 0.5;
      //inicia dados winding 2
      IniciaBus(odss.win2.bus, trafo->BarraSecundario());
      odss.win2.sinap_ligacao = trafo->sec.ligacao;
      odss.win2.sinap_fases   = trafo->sec.fase;
      odss.win2.vnom_kv       = trafo->sec.vnom;
      odss.win2.vtap_pu       = trafo->sec.tap / trafo->sec.vnom;
      odss.win2.snom_kva      = trafo->snom * 1000.;
      odss.win2.r_perc        = odss.loadloss * 0.5;
      //insere comando New Line
      if (! monta->NewTransformer_1F2E(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewTransformer_Monofasico3E(void)
   {
   //vari�veis locais
   int             fases_sec, fases_ter;
   odssTRANSFORMER odss;
   VTTrafo         *trafo;

   //inicia lista de Trafos
   if (lisEQP->Count == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de transformadores monof�sicos, 3 enrolamentos");
   //loop p/ todos Trafos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      //determina fases para secund�rio e terci�rio
      if ((trafo->sec.fase & faseAB) == faseAB)
         {
         fases_sec = faseANT;
         fases_ter = faseBNT;
         }
      else if ((trafo->sec.fase & faseBC) == faseBC)
         {
         fases_sec = faseBNT;
         fases_ter = faseCNT;
         }
      else if ((trafo->sec.fase & faseCA) == faseCA)
         {
         fases_sec = faseCNT;
         fases_ter = faseANT;
         }
      else
         {
         continue;
         }
      //inicia dados do trafo
      odss.codigo      = CodigoOdssEqpto(trafo) ;
      odss.phases      = NumeroDeFases(trafo);
      odss.xhl         = (trafo->z1.x * 100.) * 0.75;
      odss.xht         = (trafo->z1.x * 100.) * 0.75;
      odss.xlt         = (trafo->z1.x * 100.) * 0.5;
      odss.loadloss    = trafo->z1.r * 100.;
      odss.noloadloss  = trafo->perda_ferro;
      //inicia dados winding 1
      IniciaBus(odss.win1.bus, trafo->BarraPrimario());
      odss.win1.sinap_ligacao = trafo->pri.ligacao;
      odss.win1.sinap_fases   = trafo->pri.fase;
      odss.win1.vnom_kv       = trafo->pri.vnom;
      odss.win1.vtap_pu       = trafo->pri.tap / trafo->pri.vnom;
      odss.win1.snom_kva      = trafo->snom * 1000.;
      odss.win1.r_perc        = odss.loadloss * 0.4;
      //inicia dados winding 2
      IniciaBus(odss.win2.bus, trafo->BarraSecundario());
      odss.win2.sinap_ligacao = trafo->sec.ligacao;
      odss.win2.sinap_fases   = fases_sec;
      odss.win2.vnom_kv       = trafo->sec.vnom * 0.5;
      odss.win2.vtap_pu       = trafo->sec.tap / trafo->sec.vnom;
      odss.win2.snom_kva      = (trafo->snom * 0.5) * 1000.;
      odss.win2.r_perc        = odss.loadloss * 0.4;
      //inicia dados winding 3
      IniciaBus(odss.win3.bus, trafo->BarraSecundario());
      odss.win3.sinap_ligacao = trafo->sec.ligacao;
      odss.win3.sinap_fases   = fases_ter;
      odss.win3.vnom_kv       = trafo->sec.vnom * 0.5;
      odss.win3.vtap_pu       = trafo->sec.tap / trafo->sec.vnom;
      odss.win3.snom_kva      = (trafo->snom * 0.5) * 1000.;
      odss.win3.r_perc        = odss.loadloss * 0.2;
      //insere comando New Line
      if (! monta->NewTransformer_1F3E(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoNewTransformer_3F2E(void)
   {
   //vari�veis locais
   odssTRANSFORMER odss;
   VTTrafo         *trafo;

   //inicia lista de Trafos
   if (lisEQP->Count == 0) return(true);
   //insere coment�rio identificando o bloco
   InsereHeader("Bloco com dados de transformadores trif�sicos, 2 enrolamentos");
   //loop p/ todos Trafos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      //inicia dados do trafo
      odss.codigo      = CodigoOdssEqpto(trafo);
      odss.phases      = NumeroDeFases(trafo);
      odss.xhl         = trafo->z1.x * 100.;
      odss.loadloss    = trafo->z1.r * 100.;
      odss.noloadloss  = trafo->perda_ferro;
      //inicia dados winding 1
      IniciaBus(odss.win1.bus, trafo->BarraPrimario());
      odss.win1.sinap_ligacao = trafo->pri.ligacao;
      odss.win1.sinap_fases   = trafo->pri.fase;
      odss.win1.vnom_kv       = trafo->pri.vnom;
      odss.win1.vtap_pu       = trafo->pri.tap / trafo->pri.vnom;
      odss.win1.snom_kva      = trafo->snom * 1000.;
      odss.win1.r_perc        = odss.loadloss * 0.5;
      //inicia dados winding 2
      IniciaBus(odss.win2.bus, trafo->BarraSecundario());
      odss.win2.sinap_ligacao = trafo->sec.ligacao;
      odss.win1.sinap_fases   = trafo->sec.fase;
      odss.win2.vnom_kv       = trafo->sec.vnom;
      odss.win2.vtap_pu       = trafo->sec.tap / trafo->sec.vnom;
      odss.win2.snom_kva      = trafo->snom * 1000.;
      odss.win2.r_perc        = odss.loadloss * 0.5;
      //insere comando New Line
      if (! monta->NewTransformer_3F2E(odss)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool       __fastcall TExportaODSS::MontaBlocoPlotMonitors(void)
{
	AnsiString string;
	//insere comandos de configura��o
   InsereHeader("Op��es de apresenta��o de resultados");

   lines->Add("Export monitors FaltaV");
   lines->Add("Plot monitor object= FaltaV channels=(1 ) bases=[7967.43]");


   //monta pra cada maquina
   for (int nm = 0; nm < monitores->Count; nm++)
   {
		string =  monitores->Strings[nm];
		lines->Add("Export monitors " + string );
		lines->Add("Plot monitor object= "+ string +" channels=(1 3 5 )");
   }

   return(true);
}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoSet_ctkmodel(void)
   {
   InsereHeader("Configura c�lculo de sequ�ncia positiva");
   lines->Add("set cktmodel=pos");

   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoSetVoltagebases(void)
   {
   //vari�veis locais
   AnsiString    texto, bases;
   double       vbase_kv;
   VTSuprimento *suprimento, *sup_ref;

   //obt�m lista de Suprimento do tipo V/F
   if (LisSuprimento(supVF) == 0) return(false);
   //mant�m na lista somente Suprimentos com tens�o diferente
   for (int i = 0; i < lisEQP->Count; i++)
      {
      sup_ref = (VTSuprimento*)lisEQP->Items[i];
      //compara tens�es com demais Suprimentos da lista (percorrida no sentido inverso)
      for (int j = lisEQP->Count - 1; j > i; j--)
         {
         suprimento = (VTSuprimento*)lisEQP->Items[j];
         if (IsDoubleZero(suprimento->vnom - sup_ref->vnom))
            {//retira suprimento da lista
            lisEQP->Delete(j);
            }
         }
      }
   //monta string com as tens�es de base
   for (int n = 0; n < lisEQP->Count; n++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[n];
      vbase_kv   = suprimento->vnom;
      if (bases.IsEmpty()) bases  = DoubleToStr("%4.3f",  vbase_kv);
      else                 bases += DoubleToStr(" %4.3f", vbase_kv);
      }
   InsereHeader("Configura tens�o de base");
   lines->Add(texto.sprintf("Set voltagebases=[%s]", bases.c_str()));

   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoSetVoltagebases(void)
   {
   //vari�veis locais
   AnsiString    texto, bases;
   double       vbase_kv;
   VTZona       *zona;

   //monta lisZONA com Zonas da Net
   lisZONA->Clear();
   net->LisZona(lisZONA);
   if (lisZONA->Count == 0) return(false);
   //salva em lisEQP as Zonas com tens�es diferenres
   lisEQP->Clear();
   //loop p/ todas Zonas
   for (int n = 0; n < lisZONA->Count; n++)
      {
      zona = (VTZona*)lisZONA->Items[n];
      //verifica se j� existe com a mesma tens�o de linha
      if (ExisteZona(lisEQP, zona->Vff_kv) != NULL) continue;
      //salva Zona em lisEQP
      lisEQP->Add(zona);
      //inclui tens�o de base
      vbase_kv   = zona->Vff_kv;
      if (bases.IsEmpty()) bases  = DoubleToStr("%4.3f",  vbase_kv);
      else                 bases += DoubleToStr(" %4.3f", vbase_kv);
      }
   InsereHeader("Configura tens�o de base");
   lines->Add(texto.sprintf("Set voltagebases=[%s]", bases.c_str()));
   lines->Add("calcv");

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoSolve(void)
   {
   //insere comandos de configura��o
   InsereHeader("Op��es de c�lculo");
   lines->Add("set maxiterations=50");
   lines->Add("set mode=daily");
	lines->Add("set algorithm=" + monta->AlgoritmoDeCalculo());
   //insere comando de c�lculo
   InsereHeader("C�lculo");
   lines->Add("solve");
   //insere comandos de apresenta��o de resultados
   InsereHeader("Op��es de apresenta��o de resultados");
   //lines->Add("Show Mismatch");
   //lines->Add("Show Convergence");
   //lines->Add("Show Power KVA");
   //lines->Add("Show Powers KVA Elements");
   lines->Add("Show Voltage LN Nodes");
   lines->Add("Show Current Elements");

   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaBlocoSolveEstabilidade(void)
{
	//insere comandos de configura��o
   InsereHeader("Op��es de c�lculo");
   lines->Add("Set Maxcontroliter=20");
   lines->Add("set maxiterations=20");
   lines->Add("set mode=snapshot");
   lines->Add("solve");
   lines->Add("Set mode=dynamics ");
   lines->Add("Set stepsize=0.000166667s");
   lines->Add("Set number=1000");
	//lines->Add("set algorithm=" + monta->AlgoritmoDeCalculo());
   //insere comando de c�lculo
   InsereHeader("C�lculo");
   lines->Add("solve");
   //insere comandos de apresenta��o de resultados
   InsereHeader("Op��es de apresenta��o de resultados");
   //lines->Add("Show Mismatch");
   //lines->Add("Show Convergence");
   //lines->Add("Show Power KVA");
   //lines->Add("Show Powers KVA Elements");
   lines->Add("Show Voltage LN Nodes");
   lines->Add("Show Current Elements");

   return(true);
}
//---------------------------------------------------------------------------
bool __fastcall TExportaODSS::MontaUM_Regulador(VTRegulador *regulador, int fases)
   {
   //vari�veis locais
   odssREGCONTROL odss;
   double         vnom_kv;
   VTBarra        *bar_pri, *bar_sec;

   //determina Barra do prim�rio
   bar_pri = regulador->sentido.bar_ref;
   //determina Barra do secund�rio
   if (bar_pri == regulador->pbarra1) bar_sec = regulador->pbarra2;
   else                               bar_sec = regulador->pbarra1;
   //define tens�o nominal em fun��o do n�meor de fases
   vnom_kv = bar_pri->zona->Vff_kv;
   if (NumeroDeFases(fases) == 1) vnom_kv /= sqrt(3.);
   //inicia dados do regulador
   odss.codigo      = CodigoOdssEqpto(regulador) + IntToStr(fases);
   odss.phases      = NumeroDeFases(fases);
   //imped�ncia j0.1 pu na base 100 MVA.
   odss.xhl         = (0.1 * (regulador->Spas_mva / 100.))*100.;//regulador->z1.x * 100.;
   odss.loadloss    = regulador->z1.r * 100.;
   odss.noloadloss  = 0.0;//regulador->perda_ferro; ?
//   odss.automatico  = regulador->ajuste_auto;
   odss.num_tap     = (double)regulador->num_passo;
   odss.min_tap     = 1. - (regulador->var_tensao/100.);
   odss.max_tap     = 1. + (regulador->var_tensao/100.);
   //inicia dados winding 1
   IniciaBus(odss.win1.bus, bar_pri);
   odss.win1.sinap_fases   = fases;
   odss.win1.vnom_kv       = vnom_kv;
   odss.win1.vtap_pu       = 1.0;
   odss.win1.snom_kva      = regulador->snom * 1000.;
   odss.win1.r_perc        = odss.loadloss   * 0.5;
   odss.win1.sinap_ligacao = lgMONOFASICO;
   //inicia dados winding 2
   IniciaBus(odss.win2.bus, bar_sec);
   odss.win2.sinap_fases   = odss.win1.sinap_fases;
   odss.win2.vnom_kv       = odss.win1.vnom_kv;
   odss.win2.vtap_pu       = odss.win1.vtap_pu;
   odss.win2.snom_kva      = odss.win1.snom_kva;
   odss.win2.r_perc        = odss.win1.r_perc;
   odss.win2.sinap_ligacao = odss.win1.sinap_ligacao;
   //verifica se � ajuste autom�tico ou tap fixo
   if ((regulador->modelo != reguENERQCT) && (regulador->modelo != reguFIXO))
      {//define tens�o alvo
      odss.vreg    = (int)(regulador->cfg_auto.v_pu * 100);
      odss.ptratio = vnom_kv * 10.;
      }
   else
      {//redefine tap do secund�rio
      odss.win2.vtap_pu = 1. + (regulador->cfg_fixo.passo *
                 ((regulador->var_tensao * 0.01) / regulador->num_passo));
      }
   //insere comando New Line
   if (! monta->NewRegulador(odss)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaODSS::NomeElementMonitor(VTEqpto *eqpto)
{
	AnsiString nome = "";

	if (eqpto == NULL)
		{return "";}
	switch(eqpto->Tipo())
	{
		case eqptoGERADOR:
			nome = "IndMach012." + CodigoOdssEqpto(eqpto);
			break;

		default:
			break;
	}
	return nome;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TExportaODSS::NomeElementSuprimento(void)
{
//		//vari�veis locais
//   AnsiString      nome;
//   VTSuprimento    *suprimento;
//   VTPath          *path         = (VTPath*)apl->GetObject(__classid(VTPath));
//   VTRedes         *redes         = (VTRedes*)apl->GetObject(__classid(VTRedes));
//   VTFornecimento  *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
//   VTBarra *barraSup;
//   VTLigacao *ligacao;
//
//   //inicia lista de Suprimentos
//   lisEQP->Clear();
//   LisEqbar(lisEQP, eqptoSUPRIMENTO);
//   //filtra os Suprimento do tipo V/T
//   for (int n = lisEQP->Count - 1; n >= 0; n--)
//	  {
//	  suprimento = (VTSuprimento*)lisEQP->Items[n];
//	  if (suprimento->tiposup != supVF)
//		 {//retira Suprimento da lista
//		 lisEQP->Delete(n);
//         }
//	  }
//   //prote��o
//   if (lisEQP->Count == 0)
//      {
//		Aviso("Rede sem suprimento");
//	  return(false);
//	  }
//   //prote��o: por enquanto, insere apenas 1 Suprimento
//   if (lisEQP->Count > 1)
//	  {
//		Aviso(texto.sprintf("Rede com %d barras swing (suprimentos 'VF').\nO que fazer nesse caso?\nPor hora, aborta.", lisEQP->Count));
//	  return(false);
//	  }
//   //determina Suprimento
//	suprimento = (VTSuprimento*)lisEQP->First();
//	barraSup = suprimento->pbarra;
//	//lista ligacoes na barra do supriment
//	lisEQP->Clear();
//	redes->LisLigacao(lisEQP,barraSup);
//	//percorre todas
	return"";
}
//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(int fases_tag)
   {
   return(fases->NumeroDeFases(fases_tag));
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTArranjo *arranjo)
   {
   //vari�veis locais
   int num_fases;

   num_fases = fases->NumeroDeFases(arranjo->Fases);
   //verifica se o Trecho possui neutro
   if (arranjo->Fases & faseN) num_fases++;
   return(num_fases);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTCarga *carga)
   {
   //vari�veis locais
   int num_fases;

   num_fases = fases->NumeroDeFases(carga->Fases);
   //verifica se o Trecho possui neutro
   if (carga->Fases & faseN) num_fases++;
   //decrementa 1
   return(num_fases - 1);
   }
*/
/*
//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTCarga *carga)
   {
   return(fases->NumeroDeFases(carga->Fases));
   }
*/
//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTCarga *carga)
   {
   //vari�veis locais
   int num_fases;

   num_fases = fases->NumeroDeFases(carga->Fases);
/*
   //verifica se h� apenas 2 fases sem neutro
   if ((num_fases == 2)&&((carga->Fases & faseN) == 0))
      {//considera apenas uma fase
      num_fases = 1;
      }
*/
   return(num_fases);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTRegulador *regulador)
   {
   //vari�veis locais
   int num_fases = 3;

   //determina n�mero de fases do prim�rio
   num_fases = fases->NumeroDeFases(regulador->fases);
   //verifica se o prim�rio possui duas fases sem neutro
   if ((num_fases == 2) && ((regulador->fases & faseN) == 0))
      {//decrementa n�mero de fases
      num_fases = 1;
      }
   return(num_fases);
   }

/*
//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTTrafo *trafo)
   {
   //vari�veis locais
   int num_fases = 3;

   //verifica se Trafo monof�sico
   if (trafo->Monofasico)
      {
      //num_fases = max(fases->NumeroDeFases(trafo->pri.fase), fases->NumeroDeFases(trafo->sec.fase));
      //num_fases = min(fases->NumeroDeFases(trafo->pri.fase), fases->NumeroDeFases(trafo->sec.fase));
      num_fases = fases->NumeroDeFases(trafo->pri.fase);
      }
   return(num_fases);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeFases(VTTrafo *trafo)
   {
   //vari�veis locais
   int num_fases = 3;

   //verifica se Trafo monof�sico
   if (trafo->Monofasico)
      {//determina n�mero de fases do prim�rio
      num_fases = fases->NumeroDeFases(trafo->pri.fase);
      //verifica se o prim�rio possui duas fases sem neutro
      if ((num_fases == 2) && ((trafo->pri.fase & faseN) == 0))
         {//decrementa n�mero de fases
         num_fases = 1;
         }
      }
   return(num_fases);
   }

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::NumeroDeTrafosMonofasicos(TList *lisTRF, VTBarra *barra1, VTBarra *barra2)
   {
   //vari�veis locais
   VTTrafo *trafo;
   int      count = 0;

   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      if (! trafo->Monofasico) continue;
      if (trafo->ConectaBarra(barra1) && trafo->ConectaBarra(barra2))
         {
         count++;
         }
      }
   return(count);
   }

/*
//---------------------------------------------------------------------------
AnsiString __fastcall TExportaODSS::Trim(AnsiString codigo)
   {
   //vari�veis locais
   int length = codigo.Length();

   //substitui todos caracteres n�o alfa-num�ricos por '_'
   for (int n = 1; n <= length; n++)
      {
      if (! isalnum(codigo[n])) codigo[n] = '_';
      }
   return(codigo);
   }
*/

//---------------------------------------------------------------------------
int __fastcall TExportaODSS::TraduzModeloCarga(int modelo_sinap)
   {
//   return(loadScte);
   switch(modelo_sinap)
      {
      case cargaICTE: return(loadIcte);
      case cargaSCTE: return(loadScte);
      case cargaZCTE: return(loadZcte);
      default:        return(loadDEFAULT);
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TExportaODSS::SetBusNodes(odssBUS &bus, int fases)
   {
   bus.node[0] = ((fases & faseA) == faseA);
   bus.node[1] = ((fases & faseB) == faseB);
   bus.node[2] = ((fases & faseC) == faseC);
   bus.node[3] = ((fases & faseN) == faseN);
   }
*/

//-----------------------------------------------------------------------------
// eof


